#include "include/heuristic.h"
#include "include/io.h"
#include "include/persist.h"
#include "include/search.h"
#include "include/wordle.h"
#include <iostream>
#include <numeric>
#include <vector>

void play(const std::vector<int> &explores) {
  std::vector<types::target_t> targets(io::NUM_TARGETS);
  std::iota(targets.begin(), targets.end(), 0);

  const search::state initial_state(targets, explores);
  const search::state game_state =
      initial_state
          .make_next_state(io::get_guess_index("house"),
                           types::base_int<3>(std::string("-Y---")))
          .make_next_state(io::get_guess_index("bairn"),
                           types::base_int<3>(std::string("---Y-")))
          .make_next_state(io::get_guess_index("pluot"),
                           types::base_int<3>(std::string("---Y-")))
          .make_next_state(io::get_guess_index("frock"),
                           types::base_int<3>(std::string("-GGY-")));

  auto ans = search::search(game_state);
  if (ans.guess == -1) {
    std::cout << "Something went wrong - failed to search, ans.guess=1"
              << std::endl;
  } else {
    std::cout << io::guess_words[ans.guess] << " " << ans.ev << std::endl;
  }
}

int main(int argc, char *argv[]) {
  std::vector<int> explores;
  for (int i = 1; i < argc; i++) {
    explores.push_back(std::stoi(argv[i]));
  }
  play(explores);
}
