#include "command_handler.h"

#include "handlers/dump_file_prefs.h"
#include "handlers/dump_localization.h"
#include "handlers/dump_voice.h"
#include "il2.h"

namespace stunned_swallow {

auto HandleCommand(const std::string& command) -> std::string {
  if (command == "dump_file_based_prefs") {
    return stunned_swallow::handler::dump_file_prefs();
  } else if (command == "dump_localization") {
    return stunned_swallow::handler::dump_localization();
  } else if (command == "dump_voice_words") {
    return stunned_swallow::handler::dump_voice();
  } else {
    il2cppi_log_write(std::string("Unknown Command: ") + command);
    return std::string("no-op");
  }
}

}  // namespace stunned_swallow