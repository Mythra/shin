#include "named_pipe.h"

#include <cstdint>
#include <exception>
#include <memory>
#include <stdexcept>

#include "il2.h"

namespace stunned_swallow {

// Max uint32_t - 4 for message len - 1 for magic byte.
constexpr uint32_t kMaxMessageSize = 0xFFFFFFFF - sizeof(uint32_t) - 1;

void NamedPipe::listen() {
  auto pipe_path = std::string("\\\\.\\pipe\\") + _pipe_name;
  _pipe = CreateNamedPipe(
      static_cast<LPCSTR>(pipe_path.data()), PIPE_ACCESS_DUPLEX,
      PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0xFFFFFFFF,
      0xFFFFFFFF, NMPWAIT_USE_DEFAULT_WAIT, nullptr);

  if (_pipe == INVALID_HANDLE_VALUE) {
    auto error_str = std::string("Failed to create named pipe: '") +
                     _pipe_name + "', " + std::to_string(GetLastError());
    throw std::runtime_error(error_str);
  }

  il2cppi_log_write(std::string("Listening on Pipe Named: {") + _pipe_name +
                    "}");
  if (!ConnectNamedPipe(_pipe, nullptr)) {
    auto error_str = std::string("Failed to ConnectNamedPipe: ") +
                     std::to_string(GetLastError());
    throw std::runtime_error(error_str);
  }
  _setup = true;
}

auto NamedPipe::read_message() -> std::string {
  uint32_t message_bytes;
  DWORD bytes_received;
  char signature_byte;

  if (!_setup) {
    throw std::runtime_error("NamedPipe read_message called before listen()");
  }

  if (!ReadFile(_pipe, (LPVOID)&signature_byte, sizeof(char), &bytes_received,
                nullptr)) {
    auto error_str = std::string("Failed to read signature byte ReadFile: ") +
                     std::to_string(GetLastError());
    throw std::runtime_error(error_str);
  }

  if (signature_byte != static_cast<char>(0x2B)) {
    throw std::runtime_error("Client did not send special byte '0x2B'");
  }

  if (!ReadFile(_pipe, (LPVOID)&message_bytes, sizeof(uint32_t),
                &bytes_received, NULL)) {
    auto error_str = std::string("Failed to read message length ReadFile: ") +
                     std::to_string(GetLastError());
    throw std::runtime_error(error_str);
  }
  if (message_bytes > kMaxMessageSize) {
    throw std::runtime_error("Message size is too large!");
  }

  std::unique_ptr<char[]> buffer(new char[message_bytes + 1]);
  uint32_t total_bytes_received = 0;

  while (total_bytes_received < message_bytes) {
    if (!ReadFile(_pipe, buffer.get() + total_bytes_received,
                  message_bytes - total_bytes_received, &bytes_received,
                  NULL)) {
      auto error_str = std::string("Failed to read message of length ") +
                       std::to_string(message_bytes) + " from pipe '" +
                       _pipe_name + "': " + std::to_string(GetLastError());
      throw std::runtime_error(error_str);
    }
    total_bytes_received += bytes_received;
  }
  buffer.get()[message_bytes] = '\0';
  return std::string(buffer.get());
}

void NamedPipe::write_message(std::string_view data) {
  uint32_t data_as_u32 = static_cast<uint32_t>(data.size());
  if (data_as_u32 > kMaxMessageSize) {
    auto error_str = std::string(
                         "Can't write something bigger than max message size! "
                         "Size writing was: ") +
                     std::to_string(data.size());
    throw std::runtime_error(error_str);
  }

  auto new_size = data.size() + sizeof(uint32_t) + 1;

  std::unique_ptr<char[]> buffer(new char[new_size]);
  buffer.get()[0] = 0x2B;
  memcpy((void *)&buffer.get()[1], &data_as_u32, sizeof(uint32_t));
  memcpy((void *)&buffer.get()[1 + sizeof(uint32_t)], data.data(), data_as_u32);

  if (!WriteFile(_pipe, buffer.get(), new_size, nullptr, nullptr)) {
    auto error_str = std::string("Failed to write message to pipe '") +
                     _pipe_name + "': " + std::to_string(GetLastError());
    throw std::runtime_error(error_str);
  }
}

}  // namespace stunned_swallow