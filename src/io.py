from pathlib import Path


def getWords(path: Path):
    with open(path) as f:
        body = f.read()
    return body.split()


def getAnswerWords():
    return getWords(Path(__file__).parent.parent / "words" / "answers.txt")


def getGuessesWords():
    return getWords(Path(__file__).parent.parent / "words" / "answers.txt")
