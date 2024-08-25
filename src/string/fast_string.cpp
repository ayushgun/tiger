#include <arm_neon.h>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <tiger/string/fast_string.hpp>

namespace tgr {

fstring::fstring() : _data(vdupq_n_s32(0)) {}

fstring::fstring(std::string_view sv) {
  assign(sv);
}

auto fstring::operator=(std::string_view sv) -> fstring& {
  return assign(sv);
}

auto fstring::assign(std::string_view sv) -> fstring& {
  _data = vdupq_n_s32(0);
  std::memcpy(reinterpret_cast<char*>(&_data), sv.data(),
              std::min(sv.size(), std::size_t(16)));
  return *this;
}

auto fstring::operator[](std::size_t pos) const -> const char& {
  return reinterpret_cast<const char*>(&_data)[pos];
}

auto fstring::operator[](std::size_t pos) -> char& {
  return reinterpret_cast<char*>(&_data)[pos];
}

auto constexpr fstring::at(std::size_t pos) const -> const char& {
  if (pos >= size()) {
    throw std::out_of_range("fstring::at: position out of range");
  }
  return operator[](pos);
}

auto constexpr fstring::at(std::size_t pos) -> char& {
  if (pos >= size()) {
    throw std::out_of_range("fstring::at: position out of range");
  }
  return operator[](pos);
}

auto constexpr fstring::front() const -> const char& {
  return operator[](0);
}

auto constexpr fstring::front() -> char& {
  return operator[](0);
}

auto constexpr fstring::back() const -> const char& {
  return operator[](size() - 1);
}

auto constexpr fstring::back() -> char& {
  return operator[](size() - 1);
}

auto fstring::data() const -> const char* {
  return reinterpret_cast<const char*>(&_data);
}

auto fstring::data() -> char* {
  return reinterpret_cast<char*>(&_data);
}

auto constexpr fstring::cbegin() const noexcept -> const char* {
  return data();
}

auto constexpr fstring::begin() const -> const char* {
  return data();
}

auto constexpr fstring::begin() -> char* {
  return data();
}

auto constexpr fstring::cend() const noexcept -> const char* {
  return data() + size();
}

auto constexpr fstring::end() const -> const char* {
  return data() + size();
}

auto constexpr fstring::end() -> char* {
  return data() + size();
}

auto fstring::empty() const -> bool {
  return size() == 0;
}

auto fstring::size() const -> std::size_t {
  uint8x16_t data = vreinterpretq_u8_s32(_data);
  uint8x16_t cmp = vceqq_u8(data, vdupq_n_u8(0));

  uint64_t mask_low = vgetq_lane_u64(vreinterpretq_u64_u8(cmp), 0);
  uint64_t mask_high = vgetq_lane_u64(vreinterpretq_u64_u8(cmp), 1);

  if (mask_low != 0) {
    return __builtin_ctzll(mask_low) / 8;
  }

  if (mask_high != 0) {
    return 8 + (__builtin_ctzll(mask_high) / 8);
  }

  return 16;
}

auto fstring::clear() -> void {
  _data = vdupq_n_s32(0);
}

auto fstring::push_back(char c) -> void {
  auto len = size();
  if (len < 16) {
    reinterpret_cast<char*>(&_data)[len] = c;
    reinterpret_cast<char*>(&_data)[len + 1] = '\0';
  } else {
    throw std::overflow_error("fstring::push_back: string is full");
  }
}

auto fstring::pop_back() -> void {
  auto len = size();
  if (len > 0) {
    reinterpret_cast<char*>(&_data)[len - 1] = '\0';
  }
}

auto fstring::append(std::string_view sv) -> fstring& {
  auto len = size();
  std::size_t remaining_space = 16 - len;
  std::size_t append_size = std::min(sv.size(), remaining_space);

  std::memcpy(reinterpret_cast<char*>(&_data) + len, sv.data(), append_size);
  reinterpret_cast<char*>(&_data)[len + append_size] = '\0';

  return *this;
}

auto fstring::operator+=(std::string_view sv) -> fstring& {
  return append(sv);
}

auto fstring::find(std::string_view sv, std::size_t pos) const -> std::size_t {
  auto it = std::search(begin() + pos, end(), sv.begin(), sv.end());
  return (it != end()) ? std::distance(begin(), it) : std::string_view::npos;
}

auto fstring::contains(std::string_view sv) const -> bool {
  return find(sv) != std::string_view::npos;
}

auto fstring::operator==(const fstring& other) const -> bool {
  return std::memcmp(&_data, &other._data, 16) == 0;
}

auto fstring::operator+(const fstring& other) const -> fstring {
  fstring result = *this;
  result += std::string_view(other.data(), other.size());
  return result;
}

auto operator<<(std::ostream& os, const fstring& fs) -> std::ostream& {
  return os.write(fs.data(), fs.size());
}

auto operator>>(std::istream& is, fstring& fs) -> std::istream& {
  std::string input;
  is >> input;
  fs.assign(input);
  return is;
}

}  // namespace tgr
