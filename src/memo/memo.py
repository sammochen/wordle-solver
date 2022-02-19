import json
import os

memo_path = "./memo.json"


class Memo:
    def __init__(self):
        self.memo = {}  # result concatenated str -> appropriate guess

    def __enter__(self):
        print("__enter__")
        if not os.path.exists(memo_path):
            self.memo = {"": "salet"}
            return self

        with open(memo_path) as f:
            self.memo = json.load(f)
            return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        print("__exit__")
        with open(memo_path, "w") as f:
            json.dump(self.memo, f)
