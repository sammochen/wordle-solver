#pragma once

#include "heuristic.h"
#include "persist.h"
#include "types.h"
#include "utils.h"
#include "wordle.h"
#include <iostream>
#include <vector>

namespace search {
struct state {
  const std::vector<types::guess_t> guesses;
  const std::vector<types::result_key_t> result_keys;
  const std::vector<types::target_t> possible_targets;

  // The number of best guesses to explore. Reverse order.
  const std::vector<int> explores;

  state(const std::vector<types::target_t> &possible_targets,
        const std::vector<int> &explores)
      : possible_targets(possible_targets), explores(explores) {}

  state(std::vector<types::guess_t> &guesses,
        std::vector<types::result_key_t> &result_keys,
        std::vector<types::target_t> &possible_targets,
        std::vector<int> &explores)
      : guesses(std::move(guesses)), result_keys(std::move(result_keys)),
        possible_targets(std::move(possible_targets)),
        explores(std::move(explores)) {}

  std::string key() const { // return some key of guesses, results, and explores
    std::vector<std::string> ids;
    ids.push_back(utils::join(guesses, ','));

    ids.push_back(utils::join(result_keys, ','));
    ids.push_back(utils::join(explores, ','));
    return utils::join(ids, '-');
  }

  state make_next_state(const types::guess_t &guess,
                        const types::result_t &result) const {
    auto next_guesses = guesses;
    auto next_result_keys = result_keys;
    auto next_explores = explores;
    std::vector<types::target_t> next_possible_targets;

    next_guesses.push_back(guess);
    next_result_keys.push_back(result.key);

    if (next_explores.size() >= 2) {
      next_explores.pop_back();
    }

    for (types::target_t possible_target : possible_targets) {
      if (wordle::get_result(possible_target, guess) == result) {
        next_possible_targets.push_back(possible_target);
      }
    }
    return state(next_guesses, next_result_keys, next_possible_targets,
                 next_explores);
  }
};

struct search_result {
  double ev;
  types::guess_t guess;
};

persist::persist_map<std::string, double> ev_map("ev");
persist::persist_map<std::string, types::guess_t> guess_map("guess");

search_result search(const state &cur_state) {
  const int depth = cur_state.guesses.size();
  const auto &possible_targets = cur_state.possible_targets;
  const auto key = cur_state.key();

  if (ev_map.exist(key) && guess_map.exist(key)) {
    return {ev_map[key], guess_map[key]};
  }

  // Base case - exactly 1 word left
  if (possible_targets.size() == 1) {
    return {1, possible_targets[0]};
  }

  search_result best_search_result = {1e9, -1};
  for (const auto &guess : heuristic::get_best_guesses(
           cur_state.possible_targets, cur_state.explores.back())) {
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
      const auto next_evaluation = search(next_state);

      next_ev += next_evaluation.ev * (double)targets.size() /
                 (double)possible_targets.size();
    }

    if (next_ev < best_search_result.ev) {
      best_search_result = {next_ev, guess};
    }
  }
  ev_map[key] = best_search_result.ev;
  guess_map[key] = best_search_result.guess;
  return best_search_result;
}

} // namespace search
