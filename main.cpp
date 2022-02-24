#include <assert.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

bool is_lower_case(const std::string &s) {
  for (auto &c : s) {
    if (!(c >= 'a' && c <= 'z'))
      return false;
  }
  return true;
}

std::string calc_wordle(const std::string &target, const std::string &guess) {
  assert(is_lower_case(target) && is_lower_case(guess));
  assert(target.size() == guess.size());

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
  auto targets = get_targets();
  auto allowed = get_allowed();
  targets.insert(targets.end(), allowed.begin(), allowed.end());
  return targets;
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

  state(const std::vector<std::string> &guesses,
        const std::vector<std::string> results)
      : guesses(guesses), results(results),
        possible_targets(get_possible_targets(guesses, results)) {}

  static std::vector<std::string>
  get_possible_targets(const std::vector<std::string> &guesses,
                       const std::vector<std::string> results) {
    std::vector<std::string> possible_targets;
    const int n = guesses.size();
    for (const auto &target : io::targets) {
      bool ok = true;
      for (int i = 0; i < n; i++) {
        if (calc_wordle(target, guesses[i]) != results[i]) {
          ok = false;
          break;
        }
      }
      if (ok)
        possible_targets.push_back(target);
    }
    return possible_targets;
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
  double sum = 0, cnt = 0;
  for (auto &[result, targets] : partitions) {
    sum += targets.size();
    cnt += 1;
  }
  return sum / cnt;
}

evaluation get_evaluation(state s) {
  std::vector<std::pair<double, std::string>> partitions_guesses;
  int cnt = 0;
  for (const auto &guess : io::guesses) {
    double expected_partition_size =
        get_expected_partition_size(guess, s.possible_targets);
    partitions_guesses.push_back({expected_partition_size, guess});
  }

  std::sort(partitions_guesses.begin(), partitions_guesses.end());

  const auto best = partitions_guesses[0];
  std::cout << best.first << ' ' << best.second << std::endl;
  // Given the state, try guesses
  return {5, "salet"};
}

int main() {
  const state initial_state({}, {});

  auto ans = get_evaluation(initial_state);
  std::cout << ans.ev << std::endl;
}
