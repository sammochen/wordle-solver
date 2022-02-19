from typing import List

from .logic.wordle import Wordle


class Solver:
    def __init__(self, wordle: Wordle, answer_words: List[str], guess_words: List[str]):
        self.wordle = wordle
        self.answer_words = answer_words
        self.guess_words = guess_words

    def solve(self) -> int:
        for num_guess in range(1, 100):
            # Make a guess
            result = self.wordle.guess("")
            if result == "GGGGG":
                return num_guess

        raise RuntimeError("Stupid")
