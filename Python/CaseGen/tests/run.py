#!/usr/bin/env python
import sys
from tests.RunSfcCaseGen import case_gen

if __name__ == "__main__":
    # Check if an input string is provided as an argument
    if len(sys.argv) > 1:
        input_str = sys.argv[1]
        case_gen(input_str)
    else:
        # case_gen("")
        print("Please provide the input string as an argument.")