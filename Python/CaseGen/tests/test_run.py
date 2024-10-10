#!/usr/bin/env python
import sys
from tests.test_sfc_case_gen import test_run

if __name__ == "__main__":
    # Check if an input string is provided as an argument
    if len(sys.argv) > 1:
        input_str = sys.argv[1]
    else:
        print("Please provide the input string as an argument.")
        input_str = ""
    test_run(input_str)