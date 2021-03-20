#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string_view>
#include <thread>

#include "command_handler.h"
#include "concurrentqueue.h"
#include "il2.h"
#include "named_pipe.h"

extern const LPCWSTR LOG_FILE = L"stunned-swallog-log.txt";

void Run() {
  il2cpp_thread_attach(il2cpp_domain_get());

  try {
    auto* inbound_queue = new moodycamel::ConcurrentQueue<std::string>(16);
    auto* outbound_queue = new moodycamel::ConcurrentQueue<std::string>(16);

    std::thread processing_thread([inbound_queue, outbound_queue]() {
      il2cpp_thread_attach(il2cpp_domain_get());

      try {
        while (true) {
          std::string msg;
          auto found = inbound_queue->try_dequeue(msg);
          if (found) {
            outbound_queue->enqueue(stunned_swallow::HandleCommand(msg));
          }
          std::this_thread::yield();
        }
      } catch (const std::exception& e) {
        il2cppi_log_write(std::string("Exception in processing_thread(): ") +
                          e.what());
      } catch (Il2CppExceptionWrapper& e) {
        auto ex = e.ex;
        il2cppi_log_write(stunned_swallow::il2std(ex->message));
      }
    });
    processing_thread.detach();

    std::thread listen_thread([inbound_queue, outbound_queue]() {
      il2cpp_thread_attach(il2cpp_domain_get());

      try {
        auto srv = stunned_swallow::NamedPipe("stunned-swallow");
        srv.listen();

        while (true) {
          auto msg = srv.read_message();
          inbound_queue->enqueue(std::move(msg));

          std::string resp;
          while (true) {
            auto found = outbound_queue->try_dequeue(resp);
            if (found) {
              break;
            }
            std::this_thread::yield();
          }
          srv.write_message(std::string_view(resp));
        }
      } catch (const std::exception& e) {
        il2cppi_log_write(std::string("Exception in listen_thread(): ") +
                          e.what());
      } catch (Il2CppExceptionWrapper& e) {
        auto ex = e.ex;
        il2cppi_log_write(stunned_swallow::il2std(ex->message));
      }
    });
    listen_thread.detach();
  } catch (const std::exception& e) {
    il2cppi_log_write(std::string("Exception in Run(): ") + e.what());
  } catch (Il2CppExceptionWrapper& e) {
    auto ex = e.ex;
    il2cppi_log_write(stunned_swallow::il2std(ex->message));
  }
}