#!/usr/bin/env python
import sys
from tests.run_sfc_case_gen import case_gen

if __name__ == "__main__":
    # Check if an input string is provided as an argument
    if len(sys.argv) > 1:
        input_str = sys.argv[1]
    else:
        print("Please provide the input string as an argument.")
        input_str = ""
    case_gen(input_str)