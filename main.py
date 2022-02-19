from tqdm import tqdm

from src.io import get_answer_words, get_guesses_words
from src.logic.wordle import Wordle
from src.solver.greedy_solver import GreedySolver
from src.solver.solver import Solver

answer_words = get_answer_words()
guesses_words = get_guesses_words()


def ev():
    result_dict = {}

    for word in tqdm(answer_words):
        wordle = Wordle(word)
        solver: Solver = GreedySolver(wordle, answer_words, guesses_words)
        result = solver.solve()

        result_dict[result] = result_dict.get(result, 0) + 1

    sum = 0
    cnt = 0
    for result, freq in sorted(result_dict.items()):
        print(f"In {result} guesses: {freq} times")
        sum += result * freq
        cnt += freq

    print(f"EV: {sum / cnt:.3f}")


def single(word: str):
    wordle = Wordle(word)
    solver: Solver = GreedySolver(wordle, answer_words, guesses_words, verbose=True)
    result = solver.solve()
    print(result)


def main():
    ev()
    # single("horse")


if __name__ == "__main__":
    main()
