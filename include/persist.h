#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

namespace persist {
template <typename K, typename V> struct persist_map {
  const std::string name;
  std::unordered_map<K, V> data;
  std::string get_file_path() { return name + ".txt"; }

  persist_map(const std::string &name) : name(name) { load(); }
  ~persist_map() { persist(); }
  persist_map(const persist_map &rhs) = delete;

  V &operator[](const K &key) { return data[key]; }
  bool exist(const K &key) { return data.count(key); }

  void load() {
    std::ifstream fh(get_file_path());
    K k;
    V v;
    while (fh >> k >> v) {
      data[k] = v;
    }
  }

  void persist() {
    std::ofstream fh(get_file_path());
    for (const auto &[k, v] : data) {
      fh << k << ' ' << v << '\n';
    }
  }
};
} // namespace persist
