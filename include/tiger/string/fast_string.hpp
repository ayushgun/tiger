#pragma once

#include <arm_neon.h>
#include <cstddef>
#include <iostream>
#include <string_view>

/**
 * @file fstring.hpp
 * @brief Fixed-size string container optimized for ARM Neon.
 *
 * `fstring` provides a lightweight, fixed-size string container
 * designed for 16-byte strings that fit into ARM NEON 128-bit ultrawide
 * registers.
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

  auto operator=(std::string_view sv) -> fstring&;
  auto assign(std::string_view sv) -> fstring&;

  auto constexpr at(std::size_t pos) const -> const char&;
  auto constexpr at(std::size_t pos) -> char&;
  auto operator[](std::size_t pos) const -> const char&;
  auto operator[](std::size_t pos) -> char&;
  auto constexpr front() const -> const char&;
  auto constexpr front() -> char&;
  auto constexpr back() const -> const char&;
  auto constexpr back() -> char&;
  auto data() const -> const char*;
  auto data() -> char*;

  auto constexpr cbegin() const noexcept -> const char*;
  auto constexpr begin() const -> const char*;
  auto constexpr begin() -> char*;
  auto constexpr cend() const noexcept -> const char*;
  auto constexpr end() const -> const char*;
  auto constexpr end() -> char*;

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
