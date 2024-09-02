#include <immintrin.h>
#include <cstddef>
#include <cstring>
#include <string_view>
#include <tiger/simd/short_string.hpp>

namespace tgr {

short_string::short_string() : _data(_mm256_setzero_si256()), _size(0) {}

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
  if (_size > 31) {
    _size = 31;
  }

  alignas(32) char buffer[32] = {};
  std::memcpy(buffer, sv.data(), _size);

  _data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer));
  return *this;
}

auto short_string::assign(const char* str) -> short_string& {
  _size = std::strlen(str);
  if (_size > 31) {
    _size = 31;
  }

  alignas(32) char buffer[32] = {};
  std::memcpy(buffer, str, _size);

  _data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(buffer));
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
    throw std::out_of_range("short_string::at: position out of range");
  }
  return operator[](pos);
}

auto short_string::at(std::size_t pos) -> char& {
  if (pos >= _size) {
    throw std::out_of_range("short_string::at: position out of range");
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
  _data = _mm256_setzero_si256();
  _size = 0;
}

auto short_string::push_back(char c) -> void {
  if (_size < 31) {
    operator[](_size++) = c;
    operator[](_size) = '\0';
  } else {
    throw std::out_of_range("short_string::push_back: string is full");
  }
}

auto short_string::pop_back() -> void {
  if (_size > 0) {
    operator[](--_size) = '\0';
  }
}

auto short_string::append(std::string_view sv) -> short_string& {
  std::size_t available_space = 31 - _size;
  std::size_t to_copy = std::min(available_space, sv.size());

  std::memcpy(data() + _size, sv.data(), to_copy);
  _size += to_copy;
  operator[](_size) = '\0';

  return *this;
}

auto short_string::operator+=(std::string_view sv) -> short_string& {
  return append(sv);
}

auto short_string::operator==(const short_string& other) const -> bool {
  if (_size != other._size) {
    return false;
  }

  __m256i cmp = _mm256_cmpeq_epi8(_data, other._data);
  return _mm256_movemask_epi8(cmp) == -1;
}

auto short_string::operator<(const short_string& other) const -> bool {
  __m256i cmp = _mm256_cmpeq_epi8(_data, other._data);
  int eq_mask = _mm256_movemask_epi8(cmp);

  if (eq_mask != -1) {
    __m256i lt_cmp = _mm256_cmpgt_epi8(other._data, _data);
    int lt_mask = _mm256_movemask_epi8(lt_cmp);
    int diff_index = _tzcnt_u32(lt_mask | ~eq_mask);

    alignas(32) char this_data[32], other_data[32];
    _mm256_store_si256(reinterpret_cast<__m256i*>(this_data), _data);
    _mm256_store_si256(reinterpret_cast<__m256i*>(other_data), other._data);

    return this_data[diff_index] < other_data[diff_index];
  }

  return _size < other._size;
}

auto short_string::operator+(const short_string& other) const -> short_string {
  short_string result;

  result._size = _size;
  std::memcpy(result.data(), data(), _size);

  std::size_t available_space = 31 - _size;
  std::size_t to_copy = std::min(available_space, other._size);

  std::memcpy(result.data() + _size, other.data(), to_copy);
  result._size += to_copy;

  result.data()[result._size] = '\0';
  return result;
}

auto operator<<(std::ostream& os, const short_string& fs) -> std::ostream& {
  return os.write(fs.data(), fs.size());
}

auto operator>>(std::istream& is, short_string& fs) -> std::istream& {
  fs.clear();
  char buffer[32];
  is.get(buffer, 32);
  fs.assign(buffer);
  return is;
}

}  // namespace tgr
