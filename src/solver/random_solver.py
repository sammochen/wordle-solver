from typing import List

from overrides import overrides

from src.utils.word_filter import WordFilter

from ..logic.wordle import Wordle
from .solver import Solver


class RandomSolver(Solver):
    """Randomly chooses next guess from answer_words. The guess will always be a possible candidate."""

    def __init__(
        self,
        wordle: Wordle,
        answer_words: List[str],
        verbose: bool = False,
    ):
        self.wordle = wordle
        self.answer_words = answer_words
        self.verbose = verbose

        self.word_filter = WordFilter(self.answer_words)
        self.num_guesses = 0

    def make_guess(self) -> str:
        words = self.word_filter.words

        # TODO choose also based on frequency
        def unique_chars(s):
            return len(set([c for c in s]))

        # Prioritise knowledge. Prefer words that have many unique letters
        for num in range(5, 0, -1):
            for guess in words:
                if unique_chars(guess) == num:
                    return guess
        raise RuntimeError("Shouldn't be possible")

    def print_if_verbose(self, arg):
        if self.verbose:
            print(arg)

    @overrides
    def solve(self) -> int:
        while True:
            # Make a guess
            self.num_guesses += 1

            guess = "SALET" if self.num_guesses == 1 else self.make_guess()
            result = self.wordle.guess(guess)

            self.print_if_verbose(f"Guess:  {guess}")
            self.print_if_verbose(f"Result: {result}")

            self.word_filter.add_filter(guess, result)

            if result == "GGGGG":  # 5 Greens!
                return self.num_guesses

            if self.num_guesses >= 50:
                raise RuntimeError("Dumbass")
