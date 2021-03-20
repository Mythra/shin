#include "injector.h"

#include <Windows.h>
#include <tlhelp32.h>

#include <memory>
#include <optional>
#include <stdexcept>

namespace stunned_swallow {

auto char_to_wchar(char* car) -> std::unique_ptr<wchar_t[]> {
  size_t car_size = strlen(car) + 1;
  wchar_t* wcar = new wchar_t[car_size];
  mbstowcs(wcar, car, car_size);
  return std::unique_ptr<wchar_t[]>(wcar);
}

std::optional<HANDLE> find_process() {
  auto process_name = L"Phasmophobia.exe";

  HANDLE process;
  PROCESSENTRY32 pe32;
  DWORD dw_priority_class;

  HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (process_snap == INVALID_HANDLE_VALUE) {
    return {};
  }
  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(process_snap, &pe32)) {
    CloseHandle(process_snap);
    return {};
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  do {
    auto as_wide = char_to_wchar(pe32.szExeFile);
    if (!wcscmp(as_wide.get(), process_name)) {
      process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
      if (process != NULL) {
        return process;
      } else {
        return {};
      }
    }

  } while (Process32Next(process_snap, &pe32));

  return {};
}

void load_remote_dll(HANDLE process, const char* dll_path) {
  LPVOID addr_in_remote =
      VirtualAllocEx(process, nullptr, strlen(dll_path),
                     MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (addr_in_remote == nullptr) {
    throw std::runtime_error(
        "Failed to allocate memory in Phasmophobia.exe");
  }

  BOOL succeededWriting = WriteProcessMemory(process, addr_in_remote, dll_path,
                                             strlen(dll_path), nullptr);
  if (!succeededWriting) {
    throw std::runtime_error("Failed to write dll into Phasmophobia.exe");
  }

  LPVOID loadLibraryAddress =
      (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
  if (loadLibraryAddress == nullptr) {
    throw std::runtime_error(
        "Failed to find function LoadLibraryA to later use to load DLL");
  }
  HANDLE remoteThread = CreateRemoteThread(
      process, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress,
      addr_in_remote, 0, nullptr);
  if (remoteThread == nullptr) {
    throw std::runtime_error(
        "Failed to jump Phasmophobia.exe to LoadLibraryA");
  }

  CloseHandle(process);
}

void inject_dll(std::string dll_path) {
  auto find_phasmo = find_process();
  if (!find_phasmo.has_value()) {
    throw std::runtime_error(
        "Failed to Locate Phasmophobia.exe, did you start the game?");
  }
  auto found_phasmo = *find_phasmo;

  try {
    load_remote_dll(found_phasmo, dll_path.c_str());
  } catch (const std::exception& e) {
    throw std::runtime_error(
        std::string(
            "Failed to inject into Phasmophobia.exe, perhaps not admin?: ") +
        e.what());
  }
}

}  // namespace stunned_swallow