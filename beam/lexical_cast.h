#pragma once

#include <sstream>
#include <string>

namespace beam {

template <typename T, typename CharT, typename Traits, typename Alloc>
T lexical_cast(const std::basic_string<CharT, Traits, Alloc>& str) {
  std::basic_istringstream<CharT, Traits, Alloc> iss(str);
  T val;
  iss >> val;
  return val;
}

template <typename T, typename CharT>
T lexical_cast(const CharT* str) {
  std::basic_istringstream<CharT> iss(str);
  T val;
  iss >> val;
  return val;
}

template <typename String, typename T>
String lexical_cast(const T& val) {
  std::basic_ostringstream<typename String::value_type,
                           typename String::traits_type,
                           typename String::allocator_type> oss;
  oss << val;
  return oss.str();
}

}  // namespace beam
