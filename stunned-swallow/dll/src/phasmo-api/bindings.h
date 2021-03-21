#pragma once

#include <string>
#include <string_view>

#include "il2.h"

namespace phasmophobia {

/**
 * Get a particular text asset resource from Phasmophobia.
 */
auto GetTextResource(std::string_view) -> std::string;
/**
 * Get a voice localized string.
 */
auto GetLocalizedVoiceString(std::string_view) -> std::string;
/**
 * Get the save file for the particular build of phasmophobia.
 */
auto GetSaveFileContents() -> std::string;

}  // namespace phasmophobia