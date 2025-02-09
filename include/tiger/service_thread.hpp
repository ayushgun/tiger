#pragma once

#include <sys/qos.h>
#include <thread>

/**
 * @file service_thread.hpp
 * @brief A QoS-enhanced thread wrapper to influence macOS core scheduling.
 *
 * The `service_thread` class template wraps a standard `std::thread` to allow
 * explicit manipulation of thread scheduling on macOS through QoS classes. By
 * specifying a `qos_class_t` template parameter, the class can trick the macOS
 * scheduler into pinning the thread to either performance cores or schedulign
 * on efficiency cores.
 *
 * Two type aliases are provided:
 * - `performance_thread`: Uses `QOS_CLASS_USER_INTERACTIVE` to pin the
 * thread on p-cores.
 * - `efficiency_thread`: Uses `QOS_CLASS_BACKGROUND` to schedule the thread on
 * e-cores.
 *
 * Supported operations:
 * - `service_thread()`: Constructs an empty `service_thread`.
 * - `service_thread(F&& f, Args&&... args)`: Launches a new thread running the
 * given function with arguments.
 * - `operator=(service_thread&& other)`: Move-assigns from another
 * `service_thread`.
 * - `joinable()`: Checks if the thread is joinable.
 * - `get_id()`: Retrieves the thread's unique identifier.
 * - `native_handle()`: Returns the underlying native thread handle.
 * - `join()`: Waits for the thread to complete.
 * - `detach()`: Detaches the thread, allowing it to run independently.
 * - `swap(service_thread& other)`: Swaps the contents with another
 * `service_thread`.
 */

namespace tgr {

template <qos_class_t S>
class service_thread {
 public:
  using native_handle_type = std::thread::native_handle_type;

  template <typename F, typename... Args>
  explicit service_thread(F&& f, Args&&... args)
      : _thread(
            [c = std::forward<F>(f)](Args&&... innerArgs) {
              pthread_set_qos_class_self_np(S, 0);
              std::invoke(c, std::forward<Args>(innerArgs)...);
            },
            std::forward<Args>(args)...) {}

  service_thread() noexcept : _thread() {}

  service_thread(const service_thread&) = delete;

  auto operator=(service_thread&& other) noexcept -> service_thread& {
    if (this != &other) {
      _thread = std::move(other._thread);
    }
    return *this;
  }

  auto joinable() const noexcept -> bool {
    return _thread.joinable();
  }

  auto get_id() const noexcept -> std::thread::id {
    return _thread.get_id();
  }

  auto native_handle() -> native_handle_type {
    return _thread.native_handle();
  }

  auto join() -> void {
    return _thread.join();
  }

  auto detach() -> void {
    return _thread.detach();
  }

  auto swap(service_thread& other) noexcept -> void {
    return _thread.swap(other);
  }

 private:
  std::thread _thread;
};

using performance_thread = service_thread<QOS_CLASS_USER_INTERACTIVE>;
using efficiency_thread = service_thread<QOS_CLASS_BACKGROUND>;

}  // namespace tgr
