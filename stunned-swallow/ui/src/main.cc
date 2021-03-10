#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

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
    throw new std::runtime_error("Failed to File Pick!");
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
  /*
  auto dll_injection_path = file_picker(false);
  // Inject our DLL so it spins up a Named Pipe.
  ss::inject_dll(dll_injection_path);
  // Give time for our DLL to spin up.
  std::this_thread::sleep_for(std::chrono::seconds(1));
  auto srv = ss::NamedPipe("stunned-swallow");
  srv.connect();
*/

/*
srv.write_message(std::string_view("dump_localization"));
        auto read_msg = srv.read_message();
        auto parsed = nlohmann::json::parse(read_msg);
        if (parsed.contains("error")) {
          throw new std::runtime_error(parsed["error"].get<std::string>());
        }
        auto localization_csv = file_picker(true);
        write_file(localization_csv,
                   parsed["localisation.csv"].get<std::string>());
        srv.write_message(std::string_view("dump_voice_words"));
        auto read_msg = srv.read_message();
        auto parsed = nlohmann::json::parse(read_msg);
        if (parsed.contains("error")) {
          throw new std::runtime_error(parsed["error"].get<std::string>());
        }
        auto voice_json = file_picker(true);
        write_file(voice_json, parsed.dump());
*/
}