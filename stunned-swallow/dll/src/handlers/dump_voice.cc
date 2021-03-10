#include "handlers/dump_voice.h"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "csv.h"
#include "il2.h"

namespace ss::handler {

auto dump_voice() -> std::string {
#if !defined(PHASMO_BID)
  nlohmann::json error_json = {
      {"error",
       "Build ID not recognized, please ensure no-no words "
       "haven't been localized."}};
  return error_json.dump();
#endif

  try {
    nlohmann::json resulting_json;

    auto* material = app::Resources_Load_7(ss::std2il("localisation"),
                                           *app::Resources_Load_5__MethodInfo);
    auto* as_app_str =
        app::TextAsset_ToString((app::TextAsset*)material, nullptr);
    auto as_string = ss::il2std(as_app_str);

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

      if (key.starts_with("Phrase_")) {
        listen_phrases.push_back(
            ss::il2std(app::LocalisationSystem_GetLocalisedVoiceValue(
                ss::std2il(key), nullptr)));
      } else if (key.starts_with("Ouija")) {
        ouija_phrases.push_back(
            ss::il2std(app::LocalisationSystem_GetLocalisedVoiceValue(
                ss::std2il(key), nullptr)));
      } else if (key.starts_with("Q_")) {
        spirit_box_questions.push_back(
            ss::il2std(app::LocalisationSystem_GetLocalisedVoiceValue(
                ss::std2il(key), nullptr)));
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
  }
}

}  // namespace ss::handler