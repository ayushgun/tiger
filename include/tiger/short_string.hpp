#pragma once

#include <arm_neon.h>
#include <cstddef>
#include <iostream>
#include <string_view>

/**
 * @file short_string.hpp
 * @brief A compact, SIMD-optimized string class with a fixed maximum size of 16
 * bytes.
 *
 * The `short_string` class provides a high-performance string implementation.
 * The string is stored in a fixed-size `int8x16_t` aligned ARM NEON register,
 * with a maximum size of 16 bytes.
 *
 * Supported operations:
 * - `short_string()`: Constructs an empty `short_string`.
 * - `short_string(std::string_view sv)`: Constructs a `short_string` from a
 * string view.
 * - `short_string(const char* str)`: Constructs a `short_string` from a C-style
 * string.
 * - `assign(std::string_view sv)`: Assigns a new value from a string view.
 * - `assign(const char* str)`: Assigns a new value from a C-style string.
 * - `at(size_t pos)`: Accesses a character at a specific position, with bounds
 * checking.
 * - `operator[]`: Accesses a character at a specific position without bounds
 * checking.
 * - `front()`: Accesses the first character of the string.
 * - `back()`: Accesses the last character of the string.
 * - `data()`: Returns a pointer to the underlying string data.
 * - `empty()`: Checks if the string is empty.
 * - `size()`: Returns the size of the string.
 * - `capacity()`: Returns the capacity of the string.
 * - `clear()`: Clears the contents of the string.
 * - `push_back(char c)`: Appends a character to the string.
 * - `pop_back()`: Removes the last character from the string.
 * - `append(std::string_view sv)`: Appends a string view to the `short_string`.
 * - `operator+=(std::string_view sv)`: Appends a string view to the
 * `short_string`.
 * - `operator==(const short_string& other)`: Compares two `short_string`
 * instances for equality.
 * - `operator<(const short_string& other)`: Compares two `short_string`
 * instances lexicographically.
 * - `operator+(const short_string& other)`: Concatenates two `short_string`
 * instances.
 * - `operator<<`: Outputs the string to an output stream.
 * - `operator>>`: Reads a string from an input stream.
 */

namespace tgr {

class short_string {
 public:
  using value_type = char;
  using size_type = std::size_t;

  short_string() noexcept;
  short_string(std::string_view sv);
  short_string(const char* str);

  auto operator=(std::string_view sv) -> short_string&;
  auto operator=(const char* str) -> short_string&;

  auto assign(std::string_view sv) -> short_string&;
  auto assign(const char* str) -> short_string&;

  auto at(size_type pos) const -> const char&;
  auto at(size_type pos) -> char&;

  auto operator[](size_type pos) const -> const char&;
  auto operator[](size_type pos) -> char&;

  auto front() const -> const char&;
  auto front() -> char&;
  auto back() const -> const char&;
  auto back() -> char&;

  auto data() const noexcept -> const char*;
  auto data() noexcept -> char*;

  auto cbegin() const noexcept -> const char*;
  auto begin() const noexcept -> const char*;
  auto begin() noexcept -> char*;
  auto cend() const noexcept -> const char*;
  auto end() const noexcept -> const char*;
  auto end() noexcept -> char*;

  auto empty() const noexcept -> bool;
  auto size() const noexcept -> size_type;
  auto capacity() const noexcept -> size_type;

  auto clear() noexcept -> void;
  auto push_back(char c) -> void;
  auto pop_back() -> void;

  auto append(std::string_view sv) -> short_string&;
  auto operator+=(std::string_view sv) -> short_string&;

  auto operator==(const short_string& other) const noexcept -> bool;
  auto operator<(const short_string& other) const noexcept -> bool;

  auto operator+(const short_string& other) const -> short_string;

  friend auto operator<<(std::ostream& os, const short_string& fs)
      -> std::ostream&;
  friend auto operator>>(std::istream& is, short_string& fs) -> std::istream&;

 private:
  alignas(16) int8x16_t _data;
  size_type _size;

  static constexpr size_type _capacity{15};
  static constexpr size_type _limit{sizeof(_data)};
};

}  // namespace tgr
