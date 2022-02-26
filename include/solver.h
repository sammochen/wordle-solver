#pragma once

#include "heuristic.h"
#include "types.h"
#include "wordle.h"
#include <iostream>
#include <vector>

namespace solver {
struct state {
  const std::vector<types::guess_t> guesses;
  const std::vector<types::result_t> results;
  const std::vector<types::target_t> possible_targets;

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

struct search_result {
  double ev;
  types::guess_t guess;
};
search_result search(const state &cur_state,
                     const std::vector<int> &explorations) {
  const int depth = cur_state.guesses.size();
  const auto &possible_targets = cur_state.possible_targets;

  // Base case - exactly 1 word left
  if (possible_targets.size() == 1) {
    return {1, possible_targets[0]};
  }

  const int max_size =
      depth < explorations.size() ? explorations[depth] : explorations.back();

  search_result best_search_result = {1e9, -1};
  for (const auto &guess :
       heuristic::get_best_guesses(cur_state.possible_targets, max_size)) {
    if (depth == 0) {
      std::cout << io::guess_words[guess] << std::endl;
    }

    const auto partitions = wordle::get_partitions(guess, possible_targets);
    double next_ev = 1; // ev of 1, the current guess
    for (const auto &[result_key, targets] : partitions) {
      if (result_key == types::GGGGG_KEY)
        continue;

      const auto next_state =
          cur_state.make_next_state(guess, types::result_t(result_key));
      const auto next_evaluation = search(next_state, explorations);

      next_ev += next_evaluation.ev * (double)targets.size() /
                 (double)possible_targets.size();
    }

    if (next_ev < best_search_result.ev) {
      best_search_result = {next_ev, guess};
    }
  }
  return best_search_result;
}

} // namespace solver
