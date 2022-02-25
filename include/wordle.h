#pragma once

#include "io.h"
#include "types.h"

namespace wordle {

types::result_t get_result(const types::target_t target,
                           const types::guess_t guess) {

  const std::string target_str = io::target_words[target];
  const std::string guess_str = io::guess_words[guess];
  const int n = target_str.size();

  types::result_t result(5, types::RESULT_GREY);

  std::vector<int> target_freq(26);
  for (auto &c : target_str) {
    target_freq[c - 'a']++;
  }

  // First sweep - greens
  for (int i = 0; i < n; i++) {
    if (target_str[i] == guess_str[i]) {
      result.set(i, types::RESULT_GREEN);
      target_freq[target_str[i] - 'a']--;
    }
  }

  // Second sweep - yellows
  for (int i = 0; i < n; i++) {
    if (target_str[i] == guess_str[i])
      continue;
    if (target_freq[guess_str[i] - 'a']) {
      result.set(i, types::RESULT_YELLOW);
      target_freq[guess_str[i] - 'a']--;
    }
  }
  return result;
}
} // namespace wordle
