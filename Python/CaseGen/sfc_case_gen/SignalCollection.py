"""
SignalCollection.py

This module provides Signal list
"""
import itertools
import copy
from .SignalData import SignalData

class SignalCollection:
    def __init__(self, signal_objects = []):
        self.signals = signal_objects
        self.all_case = []
        self.satisfy_case = []
        self.others_case = []

    def parse_input_string(self, input_str):
        signal_objects = []
        lines = input_str.strip().splitlines()
        i = 0
        while i < len(lines):
            if "InputSignalName" in lines[i]:
                name = lines[i].split(":")[1].strip()
                data_type = lines[i + 1].split(":")[1].strip()
                keyword_type = lines[i + 2].split(":")[1].strip()
                # Split data and value_enum into lists
                tmp_data = ':'.join(lines[i + 3].split(":")[1:]).strip()
                data = [item.strip() for item in tmp_data.split(",")]
                tmp_precondition = ':'.join(lines[i + 4].split(":")[1:]).strip()
                precondition = [item.strip() for item in tmp_precondition.split(",")]
                tmp_value_enum = ':'.join(lines[i + 5].split(":")[1:]).strip()
                value_enum = [item.strip() for item in tmp_value_enum.split(",")]
                signal_objects.append(SignalData(name, data_type, keyword_type, data, precondition, value_enum))
                i += 5
            else:
                i += 1
        self.signals = signal_objects

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
            # self.all_case = list(itertools.product(*value_enum_hex_lists))
            self.satisfy_case = list(itertools.product(*data_hex_lists))
            # self.others_case = copy.deepcopy(self.all_case)
            # for combo in self.satisfy_case:
            #     if combo in self.others_case:
            #         self.others_case.remove(combo)
            #     else:
            #         print("[Error] There is no enum matching with : ", combo)
            # print(f"전체 조합수: {len(self.all_case)}")
            print(f"조건 만족 조합수: {len(self.satisfy_case)}")
            # print(f"조건 이외 조합수: {len(self.others_case)}")
        else:
            print("[Error] Not a available input data set: ")

    def get_signals(self):
        return self.signals

    def get_all_case(self):
        return self.all_case

    def get_satisfy_case(self):
        return self.satisfy_case

    def get_others_case(self):
        return self.others_case