#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace utils {
template <typename T>
std::vector<T> concat(std::vector<T> A, const std::vector<T> &B) {
  A.insert(A.end(), B.begin(), B.end());
  return A;
}

template <typename T>
std::string join(const std::vector<T> &arr, const char &sep) {
  std::stringstream s;
  bool first = true;
  for (const auto &e : arr) {
    if (!first)
      s << sep;

    first = false;
    s << e;
  }
  return s.str();
}
} // namespace utils
