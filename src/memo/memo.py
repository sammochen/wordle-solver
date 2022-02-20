import json
import os


class Memo:
    def __init__(self):
        self.memo_path = "./memo.json"
        self.memo = {}  # result concatenated str -> appropriate guess

    def __enter__(self):
        if not os.path.exists(self.memo_path):
            self.memo = {}
            print(
                f"Warning: {self.memo_path=} does not exist. Starting fresh with {self.memo=}"
            )
            return self

        with open(self.memo_path) as f:
            self.memo = json.load(f)
            return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        with open(self.memo_path, "w") as f:
            json.dump(self.memo, f, sort_keys=True)
