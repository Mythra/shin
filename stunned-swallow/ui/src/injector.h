#pragma once

#include <string>

namespace ss {

/**
 * Inject a DLL into `Phasmophobia.exe`. Uses just a simple method to inject
 * ***USE WITH CAUTION AS PRETTY MUCH ANY ANTICHEAT CAN CATCH THIS.***
 *
 * @param dll_path
 *  The path to the DLL to inject.
 */
void inject_dll(std::string dll_path);

}  // namespace ss