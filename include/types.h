#pragma once

#include <assert.h>

namespace types {
template <int Base> struct base_int {
  int key = 0;

  base_int(int key) : key(key) {}
  base_int(int n, int v) {
    for (int i = 0; i < n; i++) {
      set(i, v);
    }
  }
  base_int(const std::vector<int> &arr) {
    for (int i = 0; i < arr.size(); i++) {
      set(i, arr[i]);
    }
  }

  int get(int index) const { return (key / base_pow(index)) % Base; }
  void set(int index, int value) {
    assert(value >= 0 && value < Base);
    key -= get(index) * base_pow(index);
    key += value * base_pow(index);
  }

  bool operator==(const base_int<Base> &rhs) const { return key == rhs.key; }

  int base_pow(int p) const {
    if (p == 0)
      return 1;
    int half_pow = base_pow(p / 2);
    return p % 2 ? half_pow * half_pow * Base : half_pow * half_pow;
  }
};

using guess_t = int;  // index to guess_words
using target_t = int; // index to target_words
using result_t = base_int<3>;
using result_key_t = int;

const int RESULT_GREEN = 0;
const int RESULT_YELLOW = 1;
const int RESULT_GREY = 2;

const int GGGGG_KEY = result_t(5, RESULT_GREEN).key;
} // namespace types
