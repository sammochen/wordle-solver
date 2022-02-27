#include "include/heuristic.h"
#include "include/io.h"
#include "include/persist.h"
#include "include/search.h"
#include "include/wordle.h"
#include <iostream>
#include <numeric>
#include <vector>

void obj() {
  const std::vector<int> explorations = {1};

  std::vector<types::target_t> targets(io::NUM_TARGETS);
  std::iota(targets.begin(), targets.end(), 0);

  const search::state initial_state(targets);
  auto ans = search::search(initial_state, explorations);
  std::cout << io::guess_words[ans.guess] << " " << ans.ev << std::endl;
}

void test() {
  std::vector<types::target_t> targets(io::NUM_TARGETS);
  std::iota(targets.begin(), targets.end(), 0);

  const search::state initial_state(targets);
  auto best_guesses = heuristic::get_best_guesses(targets, 20);
}

int main() { obj(); }
