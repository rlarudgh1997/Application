import sys
import os
import itertools
import time
import copy

class SignalData:
    def __init__(self, name, data_type, data, value_enum):
        self.InputSignalName = name
        self.InputDataType = data_type
        self.InputData = data  # Keep this as list
        self.InputValueEnum = value_enum  # Keep this as list
        # Extract hex values only from InputValueEnum
        self.InputValueEnumHex = self.extract_hex_values(value_enum)
        # Map InputData to corresponding hex from InputValueEnum
        self.InputDataHex = self.convert_data_to_hex(data, value_enum)

    def extract_hex_values(self, value_enum):
        hex_values = []
        for entry in value_enum:
            parts = entry.split(":")
            # Check both parts for hex values and add to the list
            for part in parts:
                part = part.strip()
                if part.startswith("0x"):
                    hex_values.append(part)
                    break  # Only add the first hex value found
        return hex_values

    def convert_data_to_hex(self, data, value_enum):
        # Create a dictionary from InputValueEnum
        value_enum_dict = {}
        for entry in value_enum:
            parts = entry.split(":")
            key = None
            hex_value = None
            # Find the hex and key values regardless of their position
            for part in parts:
                part = part.strip()
                if part.startswith("0x"):
                    hex_value = part
                else:
                    key = part
            if key is not None and hex_value is not None:
                value_enum_dict[key.upper()] = hex_value  # Lowercase for case-insensitive matching
        # Map data inputs to their corresponding hex values
        return [value_enum_dict.get(item.upper(), "Unknown") for item in data]

# Define a class to hold a collection of SignalData
class SignalCollection:
    def __init__(self, signal_objects):
        self.signals = signal_objects
        self.all_value_enum_combinations = []
        self.all_data_combinations = []
        self.other_combinations = []

    def display_signals(self):
        # Print InputValueEnumHex and InputDataHex for each signal object
        idx = 0
        for signal in self.signals:
            print("--------------------------------------------------------------------------")
            print(f"Signal{idx}: {signal.InputSignalName}")
            # print(f"  InputValueEnum: {signal.InputValueEnum}")  # Display the original InputValueEnum
            # print(f"  InputValueEnumHex: {signal.InputValueEnumHex}")
            print(f"  InputData: {signal.InputData}")  # Display the original InputData
            print(f"  InputDataHex: {signal.InputDataHex}")
            print("--------------------------------------------------------------------------\n")
            idx += 1

    def generate_combinations(self):
        # Generate combinations from InputValueEnumHex and InputDataHex across all signals
        value_enum_hex_lists = [signal.InputValueEnumHex for signal in self.signals if signal.InputValueEnumHex]
        data_hex_lists = [signal.InputDataHex for signal in self.signals if signal.InputDataHex]
        # Create all combinations
        if value_enum_hex_lists and data_hex_lists:
            self.all_value_enum_combinations = list(itertools.product(*value_enum_hex_lists))
            self.all_data_combinations = list(itertools.product(*data_hex_lists))
            self.other_combinations = copy.deepcopy(self.all_value_enum_combinations)
            for combo in self.all_data_combinations:
                if combo in self.other_combinations:
                    self.other_combinations.remove(combo)
                else:
                    print("[Error] There is no enum matching with : ", combo)
            print(f"전체 조합수: {len(self.all_value_enum_combinations)}")
            print(f"조건 만족 조합수: {len(self.all_data_combinations)}")
            print(f"조건 이외 조합수: {len(self.other_combinations)}")
# # print results
#             print("Combinations of InputValueEnumHex:")
#             for combo in self.all_value_enum_combinations:
#                 print("   ", combo)
#             print("Combinations of InputDataHex:")
#             for combo in self.all_data_combinations:
#                 print("   ", combo)
#             print("Combinations of Other:")
#             for combo in self.other_combinations:
#                 print("   ", combo)
        else:
            print("[Error] Not a available input data set: ")

    def get_all_value_enum_combinations(self):
        return self.all_value_enum_combinations

    def get_all_data_combinations(self):
        return self.all_data_combinations

    def get_other_combinations(self):
        return self.other_combinations

# Define a class to hold a collection of SignalData
class TextWriter:
    def __init__(self, signal_objects, all_value_enum_combinations, all_data_combinations, other_combinations):
        if all_value_enum_combinations and all_data_combinations:
            self.signals = signal_objects
            self.all_case = all_value_enum_combinations
            self.satisfy_case = all_data_combinations
            self.others_case = other_combinations
            self.string_buffer = ""
        else:
            print("[Error] Invalid combinations")

    def make_string_buffer(self):
        if self.all_case and self.satisfy_case:
            # 조합에 쓰여진 신호들 명시
            signal_order_string = "Signal List: ("
            for idx, sig in enumerate(self.signals):
                signal_order_string += sig.InputSignalName
                if idx != len(self.signals) - 1:
                    signal_order_string += ", "
            signal_order_string += ")\n"
            self.string_buffer += signal_order_string
            # 전체 경우의 수
            all_combinations_str = ', '.join([str(tup) for tup in self.all_case])
            self.string_buffer += "All_case:\n"
            self.string_buffer += all_combinations_str
            self.string_buffer += "\n"
            # 조건 만족 경우의 수
            satisfy_combinations_str = ', '.join([str(tup) for tup in self.satisfy_case])
            self.string_buffer += "Satisfy_case:\n"
            self.string_buffer += satisfy_combinations_str
            self.string_buffer += "\n"
            # 조건 이외 경우의 수
            others_combinations_str = ', '.join([str(tup) for tup in self.others_case])
            self.string_buffer += "Others_case:\n"
            self.string_buffer += others_combinations_str
            self.string_buffer += "\n"
            # print("string_buffer: ", self.string_buffer)
            return self.string_buffer
        else:
            return "[Error] Invalid combinations"

# Function to parse the input string and create SignalData objects
def parse_input_string(input_str):
    signal_objects = []
    lines = input_str.strip().splitlines()
    i = 0
    while i < len(lines):
        if "InputSignalName" in lines[i]:
            name = lines[i].split(":")[1].strip()
            data_type = lines[i + 1].split(":")[1].strip()
            # Split data and value_enum into lists
            tmp_data = ':'.join(lines[i + 2].split(":")[1:]).strip()
            data = [item.strip() for item in tmp_data.split(",")]
            tmp_value_enum = ':'.join(lines[i + 3].split(":")[1:]).strip()
            value_enum = [item.strip() for item in tmp_value_enum.split(",")]
            signal_objects.append(SignalData(name, data_type, data, value_enum))
            i += 4
        else:
            i += 1
    return signal_objects

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

if __name__ == "__main__":
    # Check if an input string is provided as an argument
    if len(sys.argv) > 1:
        input_str = sys.argv[1]
    else:
        print("Please provide the input string as an argument.")
        input_str = test_input_str

    # Parse the input string and create SignalData objects
    signal_objects = parse_input_string(input_str)

    # Create a SignalCollection object
    signal_collection = SignalCollection(signal_objects)

    # Display the signals
    signal_collection.display_signals()
    start_time = time.time()

    # Generate combinations if needed
    signal_collection.generate_combinations()

    end_time = time.time()
    duration_time = end_time - start_time
    print(f"Time taken by the generate_combinations() function (in milliseconds): {(duration_time * 1000):.0f}")

    # 파일에 결과 쓰기
    writer = TextWriter(signal_objects, signal_collection.get_all_value_enum_combinations(), signal_collection.get_all_data_combinations(), signal_collection.get_other_combinations())
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