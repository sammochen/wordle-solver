from typing import List

from overrides import overrides
from tqdm import tqdm

from ..logic.wordle import Wordle, calc_wordle
from ..utils.word_filter import WordFilter
from .solver import Solver


class GreedySolver(Solver):
    """Greedily choose the guesses_word that minimises the number of answer_words"""

    def __init__(
        self,
        wordle: Wordle,
        answer_words: List[str],
        guesses_words: List[str],
        verbose: bool = False,
    ):
        self.wordle = wordle
        self.answer_words = answer_words
        self.guesses_words = guesses_words
        self.verbose = verbose

        self.word_filter = WordFilter(self.answer_words)

        self.num_guesses = 0

    def print_if_verbose(self, arg):
        if self.verbose:
            print(arg)

    def make_guess(self):
        # Choose a word from guesses_words such that it minimises the expected number of answer_words left
        answer_words_left = self.word_filter.words

        # If there are two words left, ev = 1.5 no matter what you choose
        if len(answer_words_left) <= 2:
            return answer_words_left[0]

        # Hypothetical - what if we guessed "guess"
        best_guess = ""
        lowest_expected_num_words_left = 1e9  # We want to lower this!!
        for guess in tqdm(self.guesses_words) if self.verbose else self.guesses_words:
            # If we did, these are the possible words left for each answer
            num_words_left_possibilities = []
            for possible_answer in answer_words_left:
                result = calc_wordle(possible_answer, guess)
                # See which word from answer_words_left would also have the same outcome
                num_words_left = sum(
                    [
                        1 if calc_wordle(fake_answer, guess) == result else 0
                        for fake_answer in answer_words_left
                    ]
                )
                num_words_left_possibilities.append(num_words_left)

            expected_num_words_left = sum(num_words_left_possibilities) / len(
                num_words_left_possibilities
            )
            if expected_num_words_left < lowest_expected_num_words_left:
                lowest_expected_num_words_left = expected_num_words_left
                best_guess = guess

        if self.verbose:
            print(f"{best_guess=} {lowest_expected_num_words_left}")
        return best_guess

    @overrides
    def solve(self) -> int:
        while True:
            # Make a guess
            self.num_guesses += 1

            guess = "salet" if self.num_guesses == 1 else self.make_guess()
            result = self.wordle.guess(guess)

            self.word_filter.add_filter(guess, result)

            self.print_if_verbose(f"Guess:  {guess}")
            self.print_if_verbose(f"Result: {result}")

            if result == "GGGGG":  # 5 Greens!
                return self.num_guesses

            if self.num_guesses >= 10:
                raise RuntimeError("Dumbass")
