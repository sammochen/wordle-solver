#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

#include "include/io.h"
#include "include/wordle.h"

struct state {
  const std::vector<types::guess_t> guesses;
  const std::vector<types::target_t> possible_targets;
  const std::vector<types::result_t> results;

  state(const std::vector<types::target_t> &possible_targets)
      : possible_targets(possible_targets) {}

  state(std::vector<types::guess_t> &guesses,
        std::vector<types::result_t> &results,
        std::vector<types::target_t> &possible_targets)
      : guesses(std::move(guesses)), results(std::move(results)),
        possible_targets(std::move(possible_targets)) {}

  state make_next_state(const types::guess_t &guess,
                        const types::result_t &result) const {
    auto next_guesses = guesses;
    auto next_results = results;
    std::vector<types::target_t> next_possible_targets;

    next_guesses.push_back(guess);
    next_results.push_back(result);

    for (types::target_t possible_target : possible_targets) {
      if (wordle::get_result(possible_target, guess) == result) {
        next_possible_targets.push_back(possible_target);
      }
    }
    return state(next_guesses, next_results, next_possible_targets);
  }
};

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

double get_expected_partition_size(
    const types::guess_t &guess,
    const std::vector<types::target_t> &possible_targets) {
  const auto partitions = get_partitions(guess, possible_targets);
  double sum = 0;
  for (auto &[result_key, targets] : partitions) {
    if (result_key == types::GGGGG_KEY)
      continue;
    sum += targets.size();
  }
  return sum / partitions.size();
}

struct evaluation {
  double ev;
  types::guess_t next_guess;
};
evaluation get_evaluation(const state &cur_state,
                          const std::vector<int> &explorations) {
  const int depth = cur_state.guesses.size();
  const auto &possible_targets = cur_state.possible_targets;

  // Base case - exactly 1 word left
  if (possible_targets.size() == 1) {
    return {1, possible_targets[0]};
  }

  const int max_size =
      depth < explorations.size() ? explorations[depth] : explorations.back();

  std::set<std::pair<double, types::guess_t>> partitions_guesses;
  for (types::guess_t guess = 0; guess < io::NUM_GUESSES; guess++) {
    partitions_guesses.insert(
        {get_expected_partition_size(guess, possible_targets), guess});

    if (partitions_guesses.size() > max_size) {
      partitions_guesses.erase(--partitions_guesses.end());
    }
  }

  evaluation best_evaluation = {1e9, -1};

  for (const auto &[_expected_partition_size, guess] : partitions_guesses) {
    if (depth == 0) {
      std::cout << io::guess_words[guess] << std::endl;
    }

    const auto partitions = get_partitions(guess, possible_targets);
    double next_ev = 1; // ev of 1, the current guess
    for (const auto &[result_key, targets] : partitions) {
      if (result_key == types::GGGGG_KEY)
        continue;

      const auto next_state =
          cur_state.make_next_state(guess, types::result_t(result_key));
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
  const std::vector<int> explorations = {3, 7, 2};

  std::vector<types::target_t> targets;
  for (types::target_t target = 0; target < io::NUM_TARGETS; target++) {
    targets.push_back(target);
  }
  const state initial_state(targets);
  auto ans = get_evaluation(initial_state, explorations);
  std::cout << io::guess_words[ans.next_guess] << " " << ans.ev << std::endl;
}
