class Wordle:
    def __init__(self, targetWord: str):
        self.targetWord: str = targetWord.lower()

    def guess(self, guessWord: str) -> str:
        guessWord = guessWord.lower()

        answer = ["-" for _ in guessWord]

        # Helper dict that tracks the frequency of each letter
        targetLetterFreq = {}
        for letter in self.targetWord:
            targetLetterFreq[letter] = targetLetterFreq.get(letter, 0) + 1

        # First sweep - checks whether any letters are green
        for i in range(5):
            if self.targetWord[i] == guessWord[i]:
                answer[i] = "G"  # Green
                targetLetterFreq[self.targetWord[i]] -= 1

        # Second sweep - if there are any letters left that are also in the targetWord
        for i in range(5):
            if self.targetWord[i] == guessWord[i]:
                continue

            if targetLetterFreq.get(guessWord[i], 0) != 0:
                answer[i] = "Y"  # Yellow
                targetLetterFreq[guessWord[i]] -= 1

        return "".join(answer)
