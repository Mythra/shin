#pragma once

#include "helpers.h"
#include "il2cpp-appdata.h"

namespace stunned_swallow {

auto std2il(const char* data) -> app::String*;
auto std2il(std::string data) -> app::String*;
auto il2std(app::String* app_str) -> std::string;
auto il2std(Il2CppString* app_str) -> std::string;

}  // namespace stunned_swallow