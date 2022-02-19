from typing import List

from overrides import overrides

from ..logic.wordle import Wordle
from .solver import Solver


class GreedySolver(Solver):
    def __init__(
        self, wordle: Wordle, answer_words: List[str], guesses_words: List[str]
    ):
        self.wordle = wordle
        self.answer_words = answer_words
        self.guesses_words = guesses_words

        self.filtered_answer_words = answer_words
        self.filtered_guesses_words = guesses_words

        self.guesses = []
        self.results = []

    def possible_word(self, word: str) -> bool:
        fake_wordle = Wordle(word)
        for i in range(len(self.guesses) - 1, -1, -1):
            if fake_wordle.guess(self.guesses[i]) != self.results[i]:
                return False
        return True

    def possible_answer_words(self) -> List[str]:
        self.filtered_answer_words = filter(
            self.possible_word, self.filtered_answer_words
        )
        return self.filtered_answer_words

    def possible_guesses_words(self) -> List[str]:
        self.filtered_guesses_words = filter(
            self.possible_word, self.filtered_guesses_words
        )
        return self.filtered_guesses_words

    def make_guess(self) -> str:
        """Uses the current state and chooses the best word to guess next

        Returns:
            str: The chosen 5 letter word as the next best guess
        """
        return next(self.possible_answer_words())

    @overrides
    def solve(self) -> int:
        for num_guess in range(1, 100):
            # Make a guess
            guess = self.make_guess()
            result = self.wordle.guess(guess)

            if result == "GGGGG":  # 5 Greens!
                return num_guess

            self.guesses.append(guess)
            self.results.append(result)

        raise RuntimeError("Stupid")
