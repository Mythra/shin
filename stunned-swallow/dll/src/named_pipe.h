#pragma once

#include <string>
#include <string_view>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ss {

/**
 * A generic NamedPipe on windows that is used to create a local listen
 * address.
 *
 * Operates off a protocol where every message is comprised of 3 parts:
 *
 *  - 1 byte signature {0x2B}
 *  - 4 byte (uint32_t) length
 *  - message
 */
class NamedPipe {
 public:
  /**
   * Construct a new named pipe given just the name of the pipe.
   */
  explicit NamedPipe(std::string name)
      : _pipe(INVALID_HANDLE_VALUE), _pipe_name(std::move(name)) {}

  /**
   * Start listening on this named pipe, further calls to read should be made
   * to actually read data out.
   */
  void listen();
  /**
   * Read a message from the named pipe.
   */
  auto read_message() -> std::string;
  /**
   * Write a message to this named pipe.
   */
  void write_message(std::string_view data);

 private:
  HANDLE _pipe;
  bool _setup;
  std::string _pipe_name;
};

}  // namespace ss