#pragma once

#include "io.h"
#include "types.h"
#include <unordered_map>

namespace wordle {

std::vector<std::vector<types::result_key_t>> memo;

types::result_t get_result(const types::target_t target,
                           const types::guess_t guess) {
  if (memo.size() == 0) {
    // memo was uninitialised
    memo.assign(io::NUM_GUESSES,
                std::vector<types::result_key_t>(io::NUM_TARGETS, -1));
  }

  types::result_key_t &memo_value = memo[guess][target];
  if (memo_value != -1)
    // target/guess combo has been calculated
    return types::result_t(memo_value);

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

  memo_value = result.key;
  return result;
}

// Given a guess and possible_targets, return how the guess could partition the
// targets
std::unordered_map<types::result_key_t, std::vector<types::target_t>>
get_partitions(const types::guess_t &guess,
               const std::vector<types::target_t> &possible_targets) {

  std::unordered_map<types::result_key_t, std::vector<types::target_t>>
      partitions;
  for (const auto &possible_target : possible_targets) {
    const auto result = wordle::get_result(possible_target, guess);
    partitions[result.key].push_back(possible_target);
  }
  return partitions;
}
} // namespace wordle
