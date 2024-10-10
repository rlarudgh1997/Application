"""
TextWriter.py

This module provides a class of case string writer
"""

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