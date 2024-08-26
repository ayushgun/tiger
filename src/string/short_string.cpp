#include <algorithm>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <tiger/string/short_string.hpp>

namespace tgr {

short_string::short_string() : _data(vdupq_n_s32(0)), _size(0) {}

short_string::short_string(std::string_view sv) {
  assign(sv);
}

short_string::short_string(const char* str) {
  assign(str);
}

auto short_string::operator=(std::string_view sv) -> short_string& {
  return assign(sv);
}

auto short_string::operator=(const char* str) -> short_string& {
  return assign(str);
}

auto short_string::assign(std::string_view sv) -> short_string& {
  _size = sv.size();
  _data = vld1q_s32(reinterpret_cast<const int32_t*>(sv.data()));
  return *this;
}

auto short_string::assign(const char* str) -> short_string& {
  _size = strnlen(str, 16);
  std::memcpy(reinterpret_cast<char*>(&_data), str, _size);
  if (_size < 16) {
    reinterpret_cast<char*>(&_data)[_size] = '\0';
  }
  return *this;
}

auto short_string::operator[](std::size_t pos) const -> const char& {
  return reinterpret_cast<const char*>(&_data)[pos];
}

auto short_string::operator[](std::size_t pos) -> char& {
  return reinterpret_cast<char*>(&_data)[pos];
}

auto short_string::at(std::size_t pos) const -> const char& {
  if (pos >= _size) {
    throw std::out_of_range("fstring::at: position out of range");
  }
  return operator[](pos);
}

auto short_string::at(std::size_t pos) -> char& {
  if (pos >= _size) {
    throw std::out_of_range("fstring::at: position out of range");
  }
  return operator[](pos);
}

auto short_string::front() const -> const char& {
  return operator[](0);
}

auto short_string::front() -> char& {
  return operator[](0);
}

auto short_string::back() const -> const char& {
  return operator[](_size - 1);
}

auto short_string::back() -> char& {
  return operator[](_size - 1);
}

auto short_string::data() const -> const char* {
  return reinterpret_cast<const char*>(&_data);
}

auto short_string::data() -> char* {
  return reinterpret_cast<char*>(&_data);
}

auto short_string::cbegin() const noexcept -> const char* {
  return data();
}

auto short_string::begin() const -> const char* {
  return data();
}

auto short_string::begin() -> char* {
  return data();
}

auto short_string::cend() const noexcept -> const char* {
  return data() + _size;
}

auto short_string::end() const -> const char* {
  return data() + _size;
}

auto short_string::end() -> char* {
  return data() + _size;
}

auto short_string::empty() const -> bool {
  return _size == 0;
}

auto short_string::size() const -> std::size_t {
  return _size;
}

auto short_string::clear() -> void {
  _data = vdupq_n_s32(0);
  _size = 0;
}

auto short_string::push_back(char c) -> void {
  if (_size < 16) {
    reinterpret_cast<char*>(&_data)[_size] = c;
    _size++;

    if (_size == 16) {
      uint8x16_t data = vld1q_u8(reinterpret_cast<const uint8_t*>(&_data));
      data = vsetq_lane_u8('\0', data, 15);
      vst1q_u8(reinterpret_cast<uint8_t*>(&_data), data);
    }
  } else {
    throw std::overflow_error("short_string::push_back: string is full");
  }
}

auto short_string::pop_back() -> void {
  if (_size > 0) {
    --_size;
    reinterpret_cast<char*>(&_data)[_size] = '\0';
  }
}

auto short_string::append(std::string_view sv) -> short_string& {
  std::size_t remaining_space = 16 - _size;
  std::size_t append_size = std::min(sv.size(), remaining_space);

  std::memcpy(reinterpret_cast<char*>(&_data) + _size, sv.data(), append_size);
  _size += append_size;
  if (_size < 16) {
    reinterpret_cast<char*>(&_data)[_size] = '\0';
  }

  return *this;
}

auto short_string::operator+=(std::string_view sv) -> short_string& {
  return append(sv);
}

auto short_string::operator==(const short_string& other) const -> bool {
  return _size == other._size && std::memcmp(&_data, &other._data, _size) == 0;
}

auto short_string::operator+(const short_string& other) const -> short_string {
  short_string result = *this;
  result += std::string_view(other.data(), other._size);
  return result;
}

auto operator<<(std::ostream& os, const short_string& fs) -> std::ostream& {
  return os.write(fs.data(), fs.size());
}

auto operator>>(std::istream& is, short_string& fs) -> std::istream& {
  fs.clear();
  char c;

  while (is.get(c) && !std::isspace(c)) {
    if (fs.size() < 16) {
      fs.push_back(c);
    } else {
      is.setstate(std::ios::failbit);
      break;
    }
  }
  return is;
}

}  // namespace tgr
