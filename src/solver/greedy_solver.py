from typing import List, Tuple

from overrides import overrides

from ..io import get_answer_words, get_guess_words
from ..logic.wordle import Wordle, calc_wordle
from ..memo.memo import Memo
from ..utils.word_filter import WordFilter
from .solver import Solver

answer_words = get_answer_words()
guess_words = get_guess_words()


def split_answer_words(answer_words_left: List[str], guess: str):
    result_dict = {}
    for answer_word in answer_words_left:
        result = calc_wordle(answer_word, guess)
        if result not in result_dict:
            result_dict[result] = []
        result_dict[result].append(answer_word)
    return result_dict


def memoised_make_guess(
    guesses: List[str], results: List[str], answer_words_left: List[str], memo: Memo
) -> Tuple[float, str]:
    assert len(guesses) == len(results)

    memo_key = ",".join(guesses) + "+" + ",".join(results)
    if memo_key in memo.memo:
        tup = tuple(memo.memo[memo_key])
        return tup

    if len(answer_words_left) == 1:
        memo.memo[memo_key] = (1, answer_words_left[0])
        return (1, answer_words_left[0])

    # Hypothetical - what if we guessed "guess"
    candidates = []
    for guess in guess_words:  # 10_000
        result_freq_dict = {}  # result -> List of answers that it could be
        for possible_answer_word in answer_words_left:  # 2_000
            result = calc_wordle(possible_answer_word, guess)
            result_freq_dict[result] = result_freq_dict.get(result, 0) + 1

        # It could proportionately be any of those results
        sum = 0
        cnt = 0

        for result, result_freq in result_freq_dict.items():
            if not result == "GGGGG":
                sum += result_freq
            cnt += 1

        expected_group_size = sum / cnt
        candidates.append((expected_group_size, guess))

    candidates.sort(key=lambda x: x[0])

    num_candidates = 4
    best_candidates = candidates[:num_candidates]

    # Now that we have our best k guesses, we can actually try them
    best_ev, best_guess = (1e9, "")
    for _, candidate in best_candidates:
        ev = 1  # The cost to make one guess

        result_dict = split_answer_words(answer_words_left, candidate)
        for result, answer_words in result_dict.items():
            if result == "GGGGG":
                # No more cost
                continue

            next_ev, _ = memoised_make_guess(
                guesses + [candidate], results + [result], answer_words, memo
            )
            ev += next_ev * (len(answer_words) / len(answer_words_left))

        if ev < best_ev:
            best_ev, best_guess = ev, candidate

    if len(answer_words_left) == len(answer_words):
        print(f"root: {best_guess}")

    memo.memo[memo_key] = best_ev, best_guess  # memoise
    return best_ev, best_guess


class GreedySolver(Solver):
    """Greedily choose the guesses_word that minimises the number of answer_words"""

    def __init__(self, wordle: Wordle, memo):
        self.wordle = wordle
        self.word_filter = WordFilter(answer_words)
        self.memo = memo

        self.guesses = []
        self.results = []

    @overrides
    def solve(self) -> int:
        num_guesses = 0
        while True:
            num_guesses += 1

            # Make a guess
            ev, guess = memoised_make_guess(
                self.guesses, self.results, self.word_filter.words, self.memo
            )
            result = self.wordle.guess(guess)

            self.guesses.append(guess)
            self.results.append(result)
            self.word_filter.add_filter(guess, result)

            if result == "GGGGG":  # 5 Greens!
                return num_guesses

            if num_guesses >= 10:
                raise RuntimeError("Dumbass")
