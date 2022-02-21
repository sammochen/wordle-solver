from functools import lru_cache


@lru_cache
def calc_wordle(answer_word: str, guess_word: str) -> str:
    answer_word = answer_word.lower()
    guess_word = guess_word.lower()

    answer = ["-" for _ in guess_word]

    # Helper dict that tracks the frequency of each letter
    target_letter_freq = {}
    for letter in answer_word:
        target_letter_freq[letter] = target_letter_freq.get(letter, 0) + 1

    # First sweep - checks whether any letters are green
    for i in range(5):
        if answer_word[i] == guess_word[i]:
            answer[i] = "G"  # Green
            target_letter_freq[answer_word[i]] -= 1

    # Second sweep - if there are any letters left that are also in the targetWord
    for i in range(5):
        if answer_word[i] == guess_word[i]:
            continue

        if target_letter_freq.get(guess_word[i], 0) != 0:
            answer[i] = "Y"  # Yellow
            target_letter_freq[guess_word[i]] -= 1

    return "".join(answer)


class Wordle:
    def __init__(self, answer_word: str):
        self.answer_word: str = answer_word.lower()

    def guess(self, guess_word: str) -> str:
        return calc_wordle(self.answer_word, guess_word)


class UnknownWordle:
    def guess(self, guess_word: str) -> str:
        print(f"Guessing: {guess_word}")
        result = input("> Result: ")
        return result
