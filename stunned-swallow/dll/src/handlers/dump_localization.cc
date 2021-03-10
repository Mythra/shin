#include "handlers/dump_localization.h"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "il2.h"

namespace ss::handler {

auto dump_localization() -> std::string {
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
    resulting_json["localisation.csv"] = as_string;
    return resulting_json.dump();
  } catch (const std::exception& err) {
    nlohmann::json error_json = {{"error", err.what()}};
    return error_json.dump();
  }
}

}  // namespace ss::handler