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

int get_guess_index(const std::string &guess) {
  for (int i = 0; i < guess_words.size(); i++) {
    if (guess == guess_words[i])
      return i;
  }
  assert(false);
}

int get_target_index(const std::string &target) {
  for (int i = 0; i < target_words.size(); i++) {
    if (target == target_words[i])
      return i;
  }
  assert(false);
}

const int NUM_TARGETS = target_words.size();
const int NUM_GUESSES = guess_words.size();
} // namespace io
