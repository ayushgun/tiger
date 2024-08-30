#pragma once

#include <immintrin.h>
#include <cstddef>
#include <iostream>
#include <string_view>

/**
 * @file fast_string.hpp
 * @brief A compact, SIMD-optimized string class with a fixed maximum size of 16
 * bytes.
 *
 * The `fast_string` class provides a high-performance string implementation
 * The string is stored in a fixed-size `__m256i` aligned AVX2 register, with a
 * maximum size of 32 bytes.
 * - `fast_string()`: Constructs an empty `fast_string`.
 * - `fast_string(std::string_view sv)`: Constructs a `fast_string` from a
 * string view.
 * - `fast_string(const char* str)`: Constructs a `fast_string` from a C-style
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
 * - `clear()`: Clears the contents of the string.
 * - `push_back(char c)`: Appends a character to the string.
 * - `pop_back()`: Removes the last character from the string.
 * - `append(std::string_view sv)`: Appends a string view to the `fast_string`.
 * - `operator+=(std::string_view sv)`: Appends a string view to the
 * `fast_string`.
 * - `operator==(const fast_string& other)`: Compares two `fast_string`
 * instances for equality.
 * - `operator<(const fast_string& other)`: Compares two `fast_string` instances
 * lexicographically.
 * - `operator+(const fast_string& other)`: Concatenates two `fast_string`
 * instances.
 * - `operator<<`: Outputs the string to an output stream.
 * - `operator>>`: Reads a string from an input stream.
 */

namespace tgr {

class fast_string {
 public:
  fast_string();
  fast_string(std::string_view sv);
  fast_string(const char* str);

  auto operator=(std::string_view sv) -> fast_string&;
  auto operator=(const char* str) -> fast_string&;
  auto assign(std::string_view sv) -> fast_string&;
  auto assign(const char* str) -> fast_string&;

  auto at(std::size_t pos) const -> const char&;
  auto at(std::size_t pos) -> char&;
  auto operator[](std::size_t pos) const -> const char&;
  auto operator[](std::size_t pos) -> char&;
  auto front() const -> const char&;
  auto front() -> char&;
  auto back() const -> const char&;
  auto back() -> char&;
  auto data() const -> const char*;
  auto data() -> char*;

  auto cbegin() const noexcept -> const char*;
  auto begin() const -> const char*;
  auto begin() -> char*;
  auto cend() const noexcept -> const char*;
  auto end() const -> const char*;
  auto end() -> char*;

  auto empty() const -> bool;
  auto size() const -> std::size_t;

  auto clear() -> void;
  auto push_back(char c) -> void;
  auto pop_back() -> void;
  auto append(std::string_view sv) -> fast_string&;
  auto operator+=(std::string_view sv) -> fast_string&;

  auto operator==(const fast_string& other) const -> bool;
  auto operator<(const fast_string& other) const -> bool;
  auto operator+(const fast_string& other) const -> fast_string;

  friend auto operator<<(std::ostream& os,
                         const fast_string& fs) -> std::ostream&;
  friend auto operator>>(std::istream& is, fast_string& fs) -> std::istream&;

 private:
  alignas(32) __m256i _data;
  std::size_t _size;
};

}  // namespace tgr
