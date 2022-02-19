class Wordle:
    def __init__(self, target_word: str):
        self.target_word: str = target_word.lower()

    def guess(self, guess_word: str) -> str:
        guess_word = guess_word.lower()

        answer = ["-" for _ in guess_word]

        # Helper dict that tracks the frequency of each letter
        target_letter_freq = {}
        for letter in self.target_word:
            target_letter_freq[letter] = target_letter_freq.get(letter, 0) + 1

        # First sweep - checks whether any letters are green
        for i in range(5):
            if self.target_word[i] == guess_word[i]:
                answer[i] = "G"  # Green
                target_letter_freq[self.target_word[i]] -= 1

        # Second sweep - if there are any letters left that are also in the targetWord
        for i in range(5):
            if self.target_word[i] == guess_word[i]:
                continue

            if target_letter_freq.get(guess_word[i], 0) != 0:
                answer[i] = "Y"  # Yellow
                target_letter_freq[guess_word[i]] -= 1

        return "".join(answer)
