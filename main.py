from src.bot import WordleBot
from src.io import getAnswerWords
from src.logic.wordle import Wordle


def main():
    sum = 0
    cnt = 0

    for word in getAnswerWords()[:10]:
        wordle = Wordle(word)
        bot = WordleBot(wordle)
        sum += bot.solve()
        cnt += 1

    if sum == 0:
        return 0
    return sum / cnt


if __name__ == "__main__":
    main()
