#include <arm_neon.h>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <tiger/short_string.hpp>

namespace tgr {

inline short_string::short_string() noexcept : _data(vdupq_n_s8(0)), _size(0) {}

inline short_string::short_string(std::string_view sv) : short_string() {
  assign(sv);
}

inline short_string::short_string(const char* str) : short_string() {
  assign(str);
}

inline auto short_string::operator=(std::string_view sv) -> short_string& {
  return assign(sv);
}

inline auto short_string::operator=(const char* str) -> short_string& {
  return assign(str);
}

inline auto short_string::assign(std::string_view sv) -> short_string& {
  _size = (sv.size() > _capacity) ? _capacity : sv.size();
  _data = vdupq_n_s8(0);
  std::memcpy(data(), sv.data(), _size);

  if (_size < _limit) {
    data()[_size] = '\0';
  }
  return *this;
}

inline auto short_string::assign(const char* str) -> short_string& {
  if (!str) {
    clear();
    return *this;
  }
  return assign(std::string_view(str));
}

inline auto short_string::operator[](size_type pos) const -> const char& {
  return data()[pos];
}

inline auto short_string::operator[](size_type pos) -> char& {
  return data()[pos];
}

inline auto short_string::at(size_type pos) const -> const char& {
  if (pos >= _size) {
    throw std::out_of_range("short_string::at: position out of range");
  }
  return (*this)[pos];
}

inline auto short_string::at(size_type pos) -> char& {
  if (pos >= _size) {
    throw std::out_of_range("short_string::at: position out of range");
  }
  return (*this)[pos];
}

inline auto short_string::front() const -> const char& {
  return (*this)[0];
}

inline auto short_string::front() -> char& {
  return (*this)[0];
}

inline auto short_string::back() const -> const char& {
  return (*this)[_size - 1];
}

inline auto short_string::back() -> char& {
  return (*this)[_size - 1];
}

inline auto short_string::data() const noexcept -> const char* {
  return reinterpret_cast<const char*>(&_data);
}

inline auto short_string::data() noexcept -> char* {
  return reinterpret_cast<char*>(&_data);
}

inline auto short_string::cbegin() const noexcept -> const char* {
  return data();
}

inline auto short_string::begin() const noexcept -> const char* {
  return data();
}

inline auto short_string::begin() noexcept -> char* {
  return data();
}

inline auto short_string::cend() const noexcept -> const char* {
  return data() + _size;
}

inline auto short_string::end() const noexcept -> const char* {
  return data() + _size;
}

inline auto short_string::end() noexcept -> char* {
  return data() + _size;
}

inline auto short_string::empty() const noexcept -> bool {
  return (_size == 0);
}

inline auto short_string::size() const noexcept -> size_type {
  return _size;
}

inline auto short_string::capacity() const noexcept -> size_type {
  return _capacity;
}

inline auto short_string::clear() noexcept -> void {
  _size = 0;
  data()[0] = '\0';
}

inline auto short_string::push_back(char c) -> void {
  if (_size >= _capacity) {
    throw std::out_of_range("short_string::push_back: string is full");
  }
  data()[_size++] = c;
  if (_size < _limit) {
    data()[_size] = '\0';
  }
}

inline auto short_string::pop_back() -> void {
  if (_size > 0) {
    data()[--_size] = '\0';
  }
}

inline auto short_string::append(std::string_view sv) -> short_string& {
  const size_type available = _capacity - _size;
  const size_type to_copy = (sv.size() > available) ? available : sv.size();

  std::memcpy(data() + _size, sv.data(), to_copy);
  _size += to_copy;

  if (_size < _limit) {
    data()[_size] = '\0';
  }
  return *this;
}

inline auto short_string::operator+=(std::string_view sv) -> short_string& {
  return append(sv);
}

inline auto short_string::operator==(const short_string& other) const noexcept
    -> bool {
  if (_size != other._size) {
    return false;
  }
  return std::memcmp(&_data, &other._data, _limit) == 0;
}

inline auto short_string::operator<(const short_string& other) const noexcept
    -> bool {
  const int cmp = std::memcmp(&_data, &other._data, _limit);
  if (cmp < 0) {
    return true;
  }
  if (cmp > 0) {
    return false;
  }
  return _size < other._size;
}

inline auto short_string::operator+(const short_string& other) const
    -> short_string {
  short_string result;
  result._size = _size;
  std::memcpy(result.data(), data(), _size);

  const size_type available = _capacity - result._size;
  const size_type to_copy = (other._size > available) ? available : other._size;
  std::memcpy(result.data() + result._size, other.data(), to_copy);
  result._size += to_copy;

  if (result._size < _limit) {
    result.data()[result._size] = '\0';
  }
  return result;
}

inline auto operator<<(std::ostream& os, const short_string& fs)
    -> std::ostream& {
  return os.write(fs.data(), static_cast<std::streamsize>(fs.size()));
}

inline auto operator>>(std::istream& is, short_string& fs) -> std::istream& {
  fs.clear();
  char buffer[short_string::_limit] = {};
  is.get(buffer, short_string::_limit);
  fs.assign(buffer);
  return is;
}

}  // namespace tgr
