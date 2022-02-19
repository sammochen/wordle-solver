from typing import List

from overrides import overrides

from ..logic.wordle import Wordle
from .solver import Solver


class RandomSolver(Solver):
    """Randomly chooses next guess from answer_words. The guess will always be a possible candidate."""
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

        self.filtered_answer_words = answer_words
        self.filtered_guesses_words = guesses_words

        self.guesses = []
        self.results = []

        self.num_guesses = 0

    def possible_word(self, word: str) -> bool:
        fake_wordle = Wordle(word)
        for i in range(len(self.guesses) - 1, -1, -1):
            if fake_wordle.guess(self.guesses[i]) != self.results[i]:
                return False
        return True

    def possible_answer_words(self) -> List[str]:
        self.filtered_answer_words = list(
            filter(self.possible_word, self.filtered_answer_words)
        )
        return self.filtered_answer_words

    def possible_guesses_words(self) -> List[str]:
        self.filtered_guesses_words = list(
            filter(self.possible_word, self.filtered_guesses_words)
        )
        return self.filtered_guesses_words

    def make_guess(self) -> str:
        L = self.possible_answer_words()

        def unique_chars(s):
            return len(set([c for c in s]))

        # Prioritise knowledge. Prefer words that have many unique letters
        for num in range(5, 0, -1):
            for guess in L:
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

            guess = self.make_guess()
            result = self.wordle.guess(guess)

            self.print_if_verbose(f"Guess:  {guess}")
            self.print_if_verbose(f"Result: {result}")

            self.guesses.append(guess)
            self.results.append(result)

            if result == "GGGGG":  # 5 Greens!
                return self.num_guesses

            if self.num_guesses >= 50:
                raise RuntimeError("Dumbass")
