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

std::vector<std::string> get_targets() {
  return read_words_from_file("../words/targets.txt");
}

std::vector<std::string> get_allowed() {
  return read_words_from_file("../words/allowed.txt");
}

const std::vector<std::string> targets = get_targets();
const std::vector<std::string> allowed = get_allowed();
const std::vector<std::string> guesses =
    utils::concat(get_targets(), get_allowed());
} // namespace io
