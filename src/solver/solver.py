from abc import ABC, abstractmethod
from typing import List

from ..logic.wordle import Wordle


class Solver(ABC):
    @abstractmethod
    def solve(self) -> int:
        raise NotImplementedError()
