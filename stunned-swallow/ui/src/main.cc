#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>

#include "injector.h"
#include "named_pipe.h"
#include "nfd.h"

auto file_picker(bool use_save) -> std::string {
  std::string path;

  nfdchar_t* selected_path;
  nfdresult_t result;
  if (use_save) {
    result = NFD_SaveDialog(nullptr, nullptr, &selected_path);
  } else {
    result = NFD_OpenDialog(nullptr, nullptr, &selected_path);
  }
  if (selected_path != nullptr) {
    path = std::string(selected_path);
  }

  if (result != NFD_OKAY) {
    throw std::runtime_error("Failed to File Pick!");
  }

  return path;
}

void write_file(const std::string& file_path, const std::string& file_data) {
  std::ofstream myfile;
  myfile.open(file_path);
  myfile << file_data << std::endl;
  myfile.close();
}

auto send_and_wait(stunned_swallow::NamedPipe& pipe, const char* command) -> std::string {
  pipe.write_message(std::string_view(command));
  // Sleep so we don't read our own message before server can pick up on it.
  std::this_thread::sleep_for(std::chrono::seconds(2));
  auto read_msg = pipe.read_message();
  return read_msg;
}

void write_error(std::string error_msg) {
  std::cerr << "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
  std::cerr << error_msg << std::endl;
  std::cerr << "Note! Stunned-Swallow May be completely broken, best to restart!" << std::endl;
}

int main(int argc, const char** argv) {
  try {
    using namespace nana;

    auto dll_injection_path = file_picker(false);
    // Inject our DLL so it spins up a Named Pipe.
    stunned_swallow::inject_dll(dll_injection_path);
    // Give time for our DLL to spin up.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto srv = stunned_swallow::NamedPipe("stunned-swallow");
    srv.connect();

    form fm;
    fm.caption("Stunned-Swallow");
    fm.size(nana::size(680, 420));

    label dump_lb{fm, true};
    dump_lb.caption("<bold size=16>Dump Tools:</>");
    dump_lb.format(true);

    button dump_config{fm, "Dump - Save File"};
    button dump_localization{fm, "Dump - localisation.csv"};
    button dump_words{fm, "Dump - Important Voice Triggers"};

    dump_config.events().click([&]() {
      auto read_msg = send_and_wait(srv, "dump_file_based_prefs");
      auto parsed = nlohmann::json::parse(read_msg);
      if (parsed.contains("error")) {
        write_error(parsed["error"].get<std::string>());
      } else {
        auto write_config_where = file_picker(true);
        write_file(write_config_where, parsed.dump());
      }
    });
    dump_localization.events().click([&]() {
      auto read_msg = send_and_wait(srv, "dump_localization");
      auto parsed = nlohmann::json::parse(read_msg);
      if (parsed.contains("error")) {
        write_error(parsed["error"].get<std::string>());
      } else {
        auto write_locale_csv_where = file_picker(true);
        write_file(write_locale_csv_where, parsed["localisation.csv"].get<std::string>());
      }
    });
    dump_words.events().click([&]() {
      auto read_msg = send_and_wait(srv, "dump_voice_words");
      auto parsed = nlohmann::json::parse(read_msg);
      if (parsed.contains("error")) {
        write_error(parsed["error"].get<std::string>());
      } else {
        auto write_voice_json_where = file_picker(true);
        write_file(write_voice_json_where, parsed.dump());
      }
    });

    place layout(fm);
    layout.div(
      "<><weight=80% vertical <><weight=70% vertical <vertical gap=10 dump_grp arrange=[30, 25, 25, 25]> ><>><>"
    );
    layout["dump_grp"] << dump_lb << dump_localization << dump_words << dump_config;
    layout.collocate();

    fm.show();
    exec();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    getchar();
  }
}