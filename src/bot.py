from .logic.wordle import Wordle


class WordleBot:
    def __init__(self, wordle: Wordle):
        self.wordle = wordle

    def solve(self) -> int:
        for numGuess in range(1, 100):
            # Make a guess
            result = self.wordle.guess("MYGUESS")
            if result == "GGGGG":
                return numGuess

        raise RuntimeError("Stupid")
