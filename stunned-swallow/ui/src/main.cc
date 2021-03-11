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

int main(int argc, const char** argv) {
  try {
    using namespace nana;

    auto dll_injection_path = file_picker(false);
    // Inject our DLL so it spins up a Named Pipe.
    ss::inject_dll(dll_injection_path);
    // Give time for our DLL to spin up.
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto srv = ss::NamedPipe("stunned-swallow");
    srv.connect();

    form fm;
    fm.caption("Stunned-Swallow");
    fm.size(nana::size(680, 420));
    button dump_localization{fm,"Dump - localization.csv"};
    button dump_words{fm, "Dump - Important Voice Triggers"};

    dump_localization.events().click([&]() {
      srv.write_message(std::string_view("dump_localization"));
      std::this_thread::sleep_for(std::chrono::seconds(2));
      auto read_msg = srv.read_message();
      std::cout << "Read Message: " << read_msg << std::endl;
      auto parsed = nlohmann::json::parse(read_msg);
      if (parsed.contains("error")) {
        throw std::runtime_error(parsed["error"].get<std::string>());
      }
      auto localization_csv = file_picker(true);
      write_file(localization_csv, parsed["localisation.csv"].get<std::string>());
    });
    dump_words.events().click([&]() {
      srv.write_message(std::string_view("dump_voice_words"));
      std::this_thread::sleep_for(std::chrono::seconds(2));
      auto read_msg = srv.read_message();
      auto parsed = nlohmann::json::parse(read_msg);
      if (parsed.contains("error")) {
        throw std::runtime_error(parsed["error"].get<std::string>());
      }
      auto voice_json = file_picker(true);
      write_file(voice_json, parsed.dump());
    });

    place layout(fm);
    layout.div("vertical <local margin=10>");
    layout["local"] << dump_localization << dump_words;
    layout.collocate();

    fm.show();
    exec();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    getchar();
  }
}