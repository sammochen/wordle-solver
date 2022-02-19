from src.io import get_answer_words, get_guesses_words
from src.logic.wordle import Wordle
from src.solver import Solver


def main():
    result_dict = {}

    answer_words = get_answer_words()
    guesses_words = get_guesses_words()

    for word in answer_words[:10]:
        wordle = Wordle(word)
        bot = Solver(wordle, answer_words, guesses_words)
        result = bot.solve()

        result_dict[result] = result_dict.get(result, 0) + 1

    for result, freq in result_dict.items():
        print(f"In {result} guesses: {freq} times")


if __name__ == "__main__":
    main()
