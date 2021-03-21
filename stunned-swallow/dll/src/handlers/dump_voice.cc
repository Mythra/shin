#include "handlers/dump_voice.h"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "csv.h"
#include "il2.h"
#include "phasmo-api/bindings.h"

namespace stunned_swallow::handler {

auto dump_voice() -> std::string {
  try {
    nlohmann::json resulting_json;

    auto as_string = phasmophobia::GetTextResource("localisation");

    // Extract important phrases.
    auto parsed_csv = csv::parse(as_string);
    std::vector<std::string> listen_phrases;
    std::vector<std::string> ouija_phrases;
    std::vector<std::string> spirit_box_questions;
    for (auto& row : parsed_csv) {
      auto key = row["key"].get<std::string>();

      // Use LocalisationSystem for dumping important phrases as that'll give
      // the ones the user cares about (those in their currently configured
      // language).

      auto* phrase_list_to_add_to = [&]() -> std::vector<std::string>* {
        if (key.starts_with("Phrase_")) {
          return &listen_phrases;
        } else if (key.starts_with("Ouija")) {
          return &ouija_phrases;
        } else if (key.starts_with("Q_")) {
          return &spirit_box_questions;
        }
        return nullptr;
      }();
      if (phrase_list_to_add_to != nullptr) {
        phrase_list_to_add_to->emplace_back(
            phasmophobia::GetLocalizedVoiceString(key));
      }
    }
    resulting_json["phrases"] = listen_phrases;
    resulting_json["ouija"] = ouija_phrases;
    resulting_json["spirit_box"] = spirit_box_questions;

    // These words are hardocded into the binary, and as such are not translated
    // or present in "localisation".
    resulting_json["no_no_words"] = {
        "Fuck",         "Bitch",    "Shit", "Cunt",  "Ass",      "Bastard",
        "Motherfucker", "Arsehole", "Crap", "Pussy", "Dickhead", "Bloody Mary"};

    return resulting_json.dump();
  } catch (const std::exception& err) {
    nlohmann::json error_json = {{"error", err.what()}};
    return error_json.dump();
  } catch (Il2CppExceptionWrapper& e) {
    auto msg = il2std(e.ex->message);
    nlohmann::json error_json = {{"error", msg}};
    return error_json.dump();
  }
}

}  // namespace stunned_swallow::handler