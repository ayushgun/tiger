#pragma once

#include <arm_neon.h>
#include <cstddef>
#include <iostream>
#include <string_view>

/**
 * @file fast_string.hpp
 * @brief String container optimized for strings that can grow up
 * to 16 bytes.
 *
 * `fstring` is a 16 byte SIMD string fit into ARM NEON 128 bit register.
 *
 * - Constructors: Initialize from a default state or a `std::string_view`.
 * - Element access: Access characters via `at`, `operator[]`, `front`,
 *   `back`, and `data`.
 * - Iterators: Obtain iterators with `begin`, `end`, `cbegin`, `cend`.
 * - Capacity: Check if the string is empty or get its size.
 * - Modifiers: Modify the string with `clear`, `push_back`, `pop_back`,
 *   `assign`, `append`, and `operator+=`.
 * - Lookup: Search with `find` and check for substrings with `contains`.
 * - Comparison: Check for equality with `operator==`.
 * - Concatenation: Concatenate two `fstring` objects with `operator+`.
 * - Stream Operations: Stream input and output with `operator<<` and
 * `operator>>`.
 */

namespace tgr {

class fstring {
 public:
  fstring();
  fstring(std::string_view sv);
  fstring(const char* str);

  auto operator=(std::string_view sv) -> fstring&;
  auto operator=(const char* str) -> fstring&;
  auto assign(std::string_view sv) -> fstring&;
  auto assign(const char* str) -> fstring&;

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
  auto append(std::string_view sv) -> fstring&;
  auto operator+=(std::string_view sv) -> fstring&;

  auto find(std::string_view sv, std::size_t pos = 0) const -> std::size_t;
  auto contains(std::string_view sv) const -> bool;

  auto operator==(const fstring& other) const -> bool;
  auto operator+(const fstring& other) const -> fstring;

  friend auto operator<<(std::ostream& os, const fstring& fs) -> std::ostream&;
  friend auto operator>>(std::istream& is, fstring& fs) -> std::istream&;

 private:
  int32x4_t _data;
};

}  // namespace tgr
