#pragma once

#include "utils.h"
#include <fstream>
#include <string>
#include <vector>

namespace io {
std::vector<std::string> read_words_from_file(const std::string &&filename) {
  std::ifstream fh(filename);
  std::vector<std::string> words;

  for (std::string temp; fh >> temp;)
    words.push_back(temp);

  return words;
}

std::vector<std::string> get_target_words() {
  return read_words_from_file("../words/targets.txt");
}

std::vector<std::string> get_allowed_words() {
  return read_words_from_file("../words/allowed.txt");
}

const std::vector<std::string> target_words = get_target_words();
const std::vector<std::string> allowed_words = get_allowed_words();
const std::vector<std::string> guess_words =
    utils::concat(target_words, allowed_words);

const int NUM_TARGETS = target_words.size();
const int NUM_GUESSES = guess_words.size();
} // namespace io
