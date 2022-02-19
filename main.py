import random

from tqdm import tqdm

from src.io import get_answer_words, get_guesses_words
from src.logic.wordle import Wordle
from src.solver.greedy_solver import GreedySolver
from src.solver.random_solver import RandomSolver
from src.solver.solver import Solver

answer_words = get_answer_words()
guesses_words = get_guesses_words()


def ev():
    result_dict = {}
    results = []

    tqdm_bar = tqdm(answer_words)
    for word in tqdm_bar:
        wordle = Wordle(word)
        solver: Solver = GreedySolver(wordle, answer_words, guesses_words)
        result = solver.solve()

        result_dict[result] = result_dict.get(result, 0) + 1
        results.append(result)
        tqdm_bar.set_description("EV: %.3f" % (sum(results) / len(results)))

    for result, freq in sorted(result_dict.items()):
        print(f"In {result} guesses: {freq} times")

    print(f"EV: {(sum(results) / len(results)):.3f}")


def single(word: str):
    wordle = Wordle(word)
    solver: Solver = GreedySolver(wordle, answer_words, guesses_words, verbose=True)
    result = solver.solve()
    print(result)


def main():
    # ev()
    single("tacit")


if __name__ == "__main__":
    main()
