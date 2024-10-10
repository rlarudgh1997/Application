#!/usr/bin/env python
import os
import time

from sfc_case_gen.SignalCollection import SignalCollection
from sfc_case_gen.TextWriter import TextWriter

# Example input string
test_input_str = """
InputSignalName   : Signal_A
InputDataType     : HUInt64
InputData         : ON
InputValueEnum    : 0x0:OFF, 0x1:ON, 0x2:MESSAGE_TIMEOUT


InputSignalName   : Signal_B
InputDataType     : HUInt64
InputData         : OFF
InputValueEnum    : OFF:0x0, TYPE_1:0x1, TYPE_2:0x2


InputSignalName   : Signal_C
InputDataType     : HUInt64
InputData         : ABC, ABCD, ABCDE
InputValueEnum    : ABC:0x0, ABCD:0x1, ABCDE:0x2, ABCDEF:0x3, ABCDEFG:0x4, ABCDEFGH:0x5
"""

def test_run(str = ""):
    # Check if an input string is provided as an argument
    if not str:
        input_str = test_input_str
    else:
        input_str = str

    # Create a SignalCollection object
    signal_collection = SignalCollection()
    signal_collection.parse_input_string(input_str)

    # Display the signals
    signal_collection.display_signals()
    start_time = time.time()

    # Generate combinations if needed
    signal_collection.generate_combinations()

    end_time = time.time()
    duration_time = end_time - start_time
    print(f"Time taken by the generate_combinations() function (in milliseconds): {(duration_time * 1000):.0f}")

    # 파일에 결과 쓰기
    writer = TextWriter(signal_collection.get_signals(), signal_collection.get_all_value_enum_combinations(), signal_collection.get_all_data_combinations(), signal_collection.get_other_combinations())
    buffer = writer.make_string_buffer()

    # 현재 실행 중인 파일의 경로 가져오기
    current_file_path = os.path.dirname(os.path.abspath(__file__))
    output_file_path = os.path.join(current_file_path, 'ItertoolsTest.txt')
    # 파일 열기 (덮어쓰기 모드 'w')
    try:
        with open(output_file_path, 'w', encoding='utf-8') as file:
            file.write(buffer)
    except Exception as e:
        print(f"Error occurred: {e}")

    file_size = os.path.getsize(output_file_path)
    print(f'File size: {file_size} bytes')