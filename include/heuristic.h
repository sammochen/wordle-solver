#pragma once

#include "wordle.h"
#include <iostream>
#include <math.h>
#include <set>
#include <vector>

namespace heuristic {

double get_guess_score(const types::guess_t &guess,
                       const std::vector<types::target_t> &possible_targets) {
  const auto partitions = wordle::get_partitions(guess, possible_targets);
  double sum = 0;
  for (auto &[result_key, targets] : partitions) {
    // if (result_key == types::GGGGG_KEY)
    //   continue;
    double p = targets.size() / (double)partitions.size();
    sum += -p; // Find the average size of the partitions
  }
  return sum;
}

std::vector<types::guess_t>
get_best_guesses(const std::vector<types::target_t> &possible_targets,
                 int max_size) {
  std::set<std::pair<double, types::guess_t>> guess_scores; // {score, guess}
  for (types::guess_t guess = 0; guess < io::NUM_GUESSES; guess++) {
    guess_scores.insert({get_guess_score(guess, possible_targets), guess});

    if (guess_scores.size() > max_size) {
      guess_scores.erase(guess_scores.begin());
    }
  }

  std::vector<types::guess_t> guesses;
  guesses.reserve(max_size);
  for (const auto &[score, guess] : guess_scores) {
    guesses.push_back(guess);
  }
  return guesses;
}
} // namespace heuristic
