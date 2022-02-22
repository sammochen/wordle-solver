from typing import List

from ..logic.wordle import Wordle


class WordFilter:
    """Keeps a candidate of words and keeps tracks of guesses/results"""

    def __init__(self, words: List[str]):
        self.words = words

    def add_filter(self, guess: str, result: str):
        def possible_word(word: str) -> bool:
            fake_wordle = Wordle(word)
            return fake_wordle.guess(guess) == result

        self.words = list(filter(possible_word, self.words))
