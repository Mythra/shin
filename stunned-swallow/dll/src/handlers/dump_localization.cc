#include "handlers/dump_localization.h"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "il2.h"
#include "phasmo-api/bindings.h"

namespace stunned_swallow::handler {

auto dump_localization() -> std::string {
  try {
    nlohmann::json resulting_json = {
        {"localisation.csv", phasmophobia::GetTextResource("localisation")}};

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