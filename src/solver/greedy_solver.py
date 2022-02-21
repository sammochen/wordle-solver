from typing import List

from overrides import overrides

from ..io import get_answer_words, get_guesses_words
from ..logic.wordle import Wordle, calc_wordle
from ..memo.memo import Memo
from ..utils.word_filter import WordFilter
from .solver import Solver

answer_words = get_answer_words()
guesses_words = get_guesses_words()


def memoised_make_guess(results: List[str], answer_words_left: List[str], memo: Memo):
    result_key = "".join(results)
    if result_key in memo.memo:
        return memo.memo[result_key]

    if len(answer_words_left) == 1:
        memo.memo[result_key] = answer_words_left[0]
        return answer_words_left[0]

    best_guess = ""
    lowest_expected_num_words_left = 1e9  # We want to lower this!!

    # Hypothetical - what if we guessed "guess"
    for guess in guesses_words:  # 10_000
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

        expected_num_words_left = sum / cnt
        if expected_num_words_left <= lowest_expected_num_words_left:
            lowest_expected_num_words_left = expected_num_words_left
            best_guess = guess

    if len(answer_words_left) == len(answer_words):
        print(f"root: {best_guess}")

    memo.memo[result_key] = best_guess  # memoise
    return best_guess


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
            guess = memoised_make_guess(self.results, self.word_filter.words, self.memo)
            result = self.wordle.guess(guess)

            self.guesses.append(guess)
            self.results.append(result)
            self.word_filter.add_filter(guess, result)

            if result == "GGGGG":  # 5 Greens!
                return num_guesses

            if num_guesses >= 10:
                raise RuntimeError("Dumbass")
