#include "il2.h"

namespace ss {

auto std2il(const char* data) -> app::String* {
  return reinterpret_cast<app::String*>(il2cpp_string_new(data));
}
auto std2il(std::string data) -> app::String* {
  return reinterpret_cast<app::String*>(il2cpp_string_new(data.c_str()));
}
auto il2std(app::String* app_str) -> std::string {
  return il2cppi_to_string(app_str);
}
auto il2std(Il2CppString* app_str) -> std::string {
  return il2cppi_to_string(app_str);
}

}  // namespace ss