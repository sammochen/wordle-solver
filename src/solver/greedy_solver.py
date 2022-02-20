from typing import List

from overrides import overrides

from ..logic.wordle import Wordle, calc_wordle
from ..utils.word_filter import WordFilter
from .solver import Solver


def memoised_make_guess(guesses, results, answer_words_left, guesses_words, memo):
    answer_words_left_set = set(answer_words_left)  # Improve runtime

    # The results alone will fully determine answer_words_left
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
        result_to_answer_dict = {}  # result -> List of answers that it could be
        for possible_answer_word in answer_words_left:  # 2_000
            result = calc_wordle(possible_answer_word, guess)
            result_to_answer_dict[result] = result_to_answer_dict.get(result, 0) + 1

        # It could proportionately be any of those results
        sum = 0
        cnt = 0
        for result, num_answers in result_to_answer_dict.items():
            sum += num_answers
            cnt += 1

        expected_num_words_left = sum / cnt
        if expected_num_words_left < lowest_expected_num_words_left:
            lowest_expected_num_words_left = expected_num_words_left
            best_guess = guess
        elif (
            abs(expected_num_words_left - lowest_expected_num_words_left) <= 1e-2
            and guess in answer_words_left_set
        ):
            # Prefer to guess a word that is in answer_words_left_set if similar
            lowest_expected_num_words_left = expected_num_words_left
            best_guess = guess

    memo.memo[result_key] = best_guess  # memoise
    return best_guess


class GreedySolver(Solver):
    """Greedily choose the guesses_word that minimises the number of answer_words"""

    def __init__(
        self,
        wordle: Wordle,
        answer_words: List[str],
        guesses_words: List[str],
        memo,
        verbose: bool = False,
    ):
        self.wordle = wordle
        self.answer_words = answer_words
        self.guesses_words = guesses_words
        self.memo = memo
        self.verbose = verbose

        self.guesses = []
        self.results = []

        self.word_filter = WordFilter(self.answer_words)

    def print_if_verbose(self, arg):
        if self.verbose:
            print(arg)

    @overrides
    def solve(self) -> int:
        while True:
            # Make a guess
            guess = memoised_make_guess(
                self.guesses,
                self.results,
                self.word_filter.words,
                self.guesses_words,
                self.memo,
            )
            result = self.wordle.guess(guess)

            self.guesses.append(guess)
            self.results.append(result)
            self.word_filter.add_filter(guess, result)

            self.print_if_verbose(f"Guess:  {guess}")
            self.print_if_verbose(f"Result: {result}")

            if result == "GGGGG":  # 5 Greens!
                return self.wordle.num_guesses

            if self.wordle.num_guesses >= 10:
                raise RuntimeError("Dumbass")
