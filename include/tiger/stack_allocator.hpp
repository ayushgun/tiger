#pragma once

#include <array>
#include <cstddef>
#include <memory_resource>

/**
 * @file stack_allocator.hpp
 * @brief A custom allocator using a fixed-size stack buffer for fast memory
 * allocation.
 *
 * The `stack_allocator` class provides a custom memory allocator backend that
 * uses a fixed-size buffer allocated on the stack. This is useful for
 * optimizing memory allocations in standard containers by avoiding dynamic
 * memory allocations when the required memory fits within the stack buffer.
 *
 * - `T`: The type of objects that the allocator allocates.
 * - `Nm`: The size multiplier for the internal stack buffer.
 * - `stack_allocator()`: Constructs a `stack_allocator` with its own stack
 * buffer.
 * - `stack_allocator(const stack_allocator<U, _Nm>& other)`: Constructs a
 * `stack_allocator` from another allocator of possibly different type `U` and
 * buffer size `_Nm`.
 * - `stack_allocator(const stack_allocator<T, Nm>& other)`: Copy constructor.
 * - `allocate(std::size_t n) -> T*`: Allocates memory for `n` objects of type
 * `T`.
 * - `deallocate(T* p, std::size_t n) -> void`: Deallocates memory for `n`
 * objects of type `T`. *(Note: This is a no-op since
 * `monotonic_buffer_resource` handles memory release automatically.)*
 *
 * Notes:
 * - The allocator is stateless, and each instance manages its own buffer
 * independently.
 */

namespace tgr {

template <typename T, std::size_t Nm>
class stack_allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;

  // Rebind struct is required to allow the allocator to be used with
  // different types.
  //
  // In STL containers, internal data structures may require allocations for
  // types other than the container's element type (e.g., for node management
  // in `std::list` or temporary storage in `std::vector`). The `rebind`
  // struct allows the allocator to be adapted to allocate memory for types
  // other than `T` (the allocator's main type).
  //
  // The `rebind<U>::other` alias enables the creation of a `stack_allocator`
  // that can allocate objects of type `U` while maintaining the same buffer
  // size (`Nm`).
  template <typename U>
  struct rebind {
    using other = stack_allocator<U, Nm>;
  };

  template <typename U, std::size_t _Nm>
  stack_allocator(const stack_allocator<U, _Nm>& /*other*/)
      : buffer_(stack_memory_.data(),
                stack_memory_.size(),
                std::pmr::null_memory_resource()) {}

  stack_allocator(const stack_allocator<T, Nm>& /*other*/)
      : buffer_(stack_memory_.data(),
                stack_memory_.size(),
                std::pmr::null_memory_resource()) {}

  stack_allocator()
      : buffer_(stack_memory_.data(),
                stack_memory_.size(),
                std::pmr::null_memory_resource()) {}

  auto allocate(std::size_t n) -> T* {
    return static_cast<T*>(buffer_.allocate(n * sizeof(T), alignof(T)));
  }

  auto deallocate(T* /*p*/, std::size_t /*n*/) -> void {
    // No-op: monotonic_buffer_resource automatically handles memory release.
  }

 private:
  std::array<std::byte, Nm * (sizeof(T) + alignof(T))> stack_memory_;
  std::pmr::monotonic_buffer_resource buffer_;
};

}  // namespace tgr
