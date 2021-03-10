#include "command_handler.h"

#include "handlers/dump_localization.h"
#include "handlers/dump_voice.h"
#include "il2.h"

namespace ss {

auto HandleCommand(const std::string& command) -> std::string {
  if (command == "dump_localization") {
    return ss::handler::dump_localization();
  } else if (command == "dump_voice_words") {
    return ss::handler::dump_voice();
  } else {
    il2cppi_log_write(std::string("Unknown Command: ") + command);
    return std::string("no-op");
  }
}

}  // namespace ss