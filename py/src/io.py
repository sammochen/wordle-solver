from pathlib import Path


def get_words(path: Path):
    with open(path) as f:
        body = f.read()
    return body.split()


def get_answer_words():
    return get_words(Path(__file__).parent.parent.parent / "words" / "answers.txt")


def get_guess_words():
    return get_answer_words() + get_words(
        Path(__file__).parent.parent.parent / "words" / "answers.txt"
    )