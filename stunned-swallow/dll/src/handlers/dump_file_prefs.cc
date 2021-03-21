#include "handlers/dump_file_prefs.h"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "il2.h"
#include "phasmo-api/bindings.h"

namespace stunned_swallow::handler {

auto dump_file_prefs() -> std::string {
  try {
    nlohmann::json resulting_json = {
        {"data", phasmophobia::GetSaveFileContents()}};

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