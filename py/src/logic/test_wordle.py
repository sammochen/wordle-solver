from .wordle import Wordle


def test_wordle_abcde():
    wordle = Wordle("ABCDE")

    assert wordle.guess("FGHIJ") == "-----"
    assert wordle.guess("AAAAA") == "G----"
    assert wordle.guess("ABCDE") == "GGGGG"
    assert wordle.guess("EDCBA") == "YYGYY"
    assert wordle.guess("XAAAA") == "-Y---"


def test_wordle_ababc():
    wordle = Wordle("ABABC")

    assert wordle.guess("BABAC") == "YYYYG"
    assert wordle.guess("CCAAA") == "Y-GY-"
