#include <assert.h>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace utils {
template <typename T>
std::vector<T> concat(std::vector<T> A, const std::vector<T> &&B) {
  A.insert(A.end(), B.begin(), B.end());
  return A;
}
bool is_lower_case(const std::string &s) {
  for (auto &c : s) {
    if (!(c >= 'a' && c <= 'z'))
      return false;
  }
  return true;
}
} // namespace utils

std::string calc_wordle(const std::string &target, const std::string &guess) {
  // assert(utils::is_lower_case(target) && utils::is_lower_case(guess));
  // assert(target.size() == guess.size());

  const int n = target.size();

  std::string result(n, '-');
  std::vector<int> target_freq(26);
  for (auto &c : target) {
    target_freq[c - 'a']++;
  }

  // First sweep - greens
  for (int i = 0; i < n; i++) {
    if (target[i] == guess[i]) {
      result[i] = 'G';
      target_freq[target[i] - 'a']--;
    }
  }

  // Second sweep - yellows
  for (int i = 0; i < n; i++) {
    if (target[i] == guess[i])
      continue;
    if (target_freq[guess[i] - 'a']) {
      result[i] = 'Y';
      target_freq[guess[i] - 'a']--;
    }
  }
  return result;
}

namespace io {
std::vector<std::string> read_words_from_file(const std::string &&filename) {
  std::ifstream fh(filename);
  std::vector<std::string> words;
  for (std::string temp; fh >> temp;) {
    words.push_back(temp);
  }
  return words;
}

std::vector<std::string> get_targets() {
  return read_words_from_file("words/targets.txt");
}

std::vector<std::string> get_allowed() {
  return read_words_from_file("words/allowed.txt");
}

std::vector<std::string> get_guesses() {
  return utils::concat(get_targets(), get_allowed());
}

const std::vector<std::string> targets = get_targets();
const std::vector<std::string> guesses = get_guesses();
} // namespace io

struct evaluation {
  double ev;
  std::string next_guess;
};

struct state {
  const std::vector<std::string> guesses, results, possible_targets;

  state() : possible_targets(io::targets) {}
  state(std::vector<std::string> &guesses, std::vector<std::string> &results,
        std::vector<std::string> &possible_targets)
      : guesses(std::move(guesses)), results(std::move(results)),
        possible_targets(std::move(possible_targets)) {}

  state make_next_state(const std::string &guess,
                        const std::string &result) const {
    auto next_guesses = guesses;
    auto next_results = results;
    std::vector<std::string> next_possible_targets;

    next_guesses.push_back(guess);
    next_results.push_back(result);

    for (const auto &possible_target : possible_targets) {
      if (calc_wordle(possible_target, guess) == result) {
        next_possible_targets.push_back(possible_target);
      }
    }
    return state(next_guesses, next_results, next_possible_targets);
  }
};

std::unordered_map<std::string, std::vector<std::string>>
get_partitions(const std::string &guess,
               const std::vector<std::string> &possible_targets) {

  std::unordered_map<std::string, std::vector<std::string>> partitions;
  for (const auto &possible_target : possible_targets) {
    const auto result = calc_wordle(possible_target, guess);
    partitions[result].push_back(possible_target);
  }
  return partitions;
}

double
get_expected_partition_size(const std::string &guess,
                            const std::vector<std::string> &possible_targets) {
  const auto partitions = get_partitions(guess, possible_targets);
  double sum = 0;
  for (auto &[result, targets] : partitions) {
    if (result == "GGGGG")
      continue;
    sum += targets.size();
  }
  return sum / partitions.size();
}

evaluation get_evaluation(const state &cur_state,
                          const std::vector<int> &explorations) {
  const int depth = cur_state.guesses.size();
  const auto &possible_targets = cur_state.possible_targets;

  // Base case - exactly 1 word left
  if (possible_targets.size() == 1) {
    return {1, possible_targets[0]};
  }

  std::set<std::pair<double, std::string>> partitions_guesses;
  for (const auto &guess : io::guesses) {
    partitions_guesses.insert(
        {get_expected_partition_size(guess, possible_targets), guess});

    if (partitions_guesses.size() > explorations[depth]) {
      partitions_guesses.erase(--partitions_guesses.end());
    }
  }

  evaluation best_evaluation = {1e9, ""};

  for (const auto &[_expected_partition_size, guess] : partitions_guesses) {
    if (depth == 0) {
      std::cout << "In root, guessing: " << guess << std::endl;
    }

    const auto partitions = get_partitions(guess, possible_targets);
    double next_ev = 1; // ev of 1, the current guess
    for (const auto &[result, targets] : partitions) {
      if (result == "GGGGG")
        continue;

      const auto next_state = cur_state.make_next_state(guess, result);
      const auto next_evaluation = get_evaluation(next_state, explorations);

      next_ev += next_evaluation.ev * (double)targets.size() /
                 (double)possible_targets.size();
    }

    if (next_ev < best_evaluation.ev) {
      best_evaluation = {next_ev, guess};
    }
  }
  return best_evaluation;
}

int main() {
  const std::vector<int> explorations = {5, 5, 5, 5, 5, 5, 5, 5};

  const state initial_state;
  auto ans = get_evaluation(initial_state, explorations);
  std::cout << ans.next_guess << " " << ans.ev << std::endl;
}
