"""
SignalData.py

This module provides Data for VSM or SFC Signal
"""
import numpy as np

class SignalData:
    def __init__(self, gen_type, name, data_type, keyword_type, data, precondition, value_enum):
        self.gen_type = gen_type
        self.InputSignalName = name
        self.InputDataType = data_type
        self.InputKeywordType = keyword_type
        self.InputData = data  # Keep this as list
        self.InputPrecondition = precondition
        self.InputValueEnum = value_enum  # Keep this as list
        # Extract hex values only from InputValueEnum
        self.InputValueEnumHex = self.extract_hex_values(value_enum)
        # Map InputData to corresponding hex from InputValueEnum
        self.InputDataHex = self.convert_data_to_hex(data, value_enum)
        self.InputPreconditionHex = self.convert_data_to_hex(precondition, value_enum)
        self.InputValueEnumDict = self.getValueEnumHexKeyDict(value_enum)
        self.NegativeTriggerHex = self.getNegativeTriggerHex(data, value_enum)

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
        # data가 비어 있으면 [Empty] 반환
        if len(data) == 1:
            if data[0] == "":
                data[0] = "[Empty]"
                return data
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
        # print("len(value_enum): ", len(value_enum))
        if len(value_enum) == 1:
            if value_enum[0] == "":
                return data
        for item in data:
            key = item.upper()
            if key in value_enum_dict:
                # print(value_enum_dict[key])  # 키가 있을 경우 해당 값을 출력
                pass
            else:
                i = 0
                for item in data:
                    i+=1
                    # print(self.InputSignalName, f"[{i}]", item)
                print(self.InputSignalName, f"Not Defined enum key: {key}")  # 키가 없을 경우 키를 출력
        # Map data inputs to their corresponding hex values
        return [value_enum_dict.get(item.upper(), item) for item in data]

    def getValueEnumHexKeyDict(self, value_enum):
        value_enum_dict_hex_key = {}
        if len(value_enum) == 1:
            if value_enum[0] == "":
                return value_enum_dict_hex_key
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
                value_enum_dict_hex_key[hex_value] = key.upper() # Lowercase for case-insensitive matching
        return value_enum_dict_hex_key

    def getNegativeTriggerHex(self, data, value_enum):
        # data가 비어 있으면 [Empty] 반환
        if len(data) == 1:
            if data[0] == "[Empty]":
                return data

        # Create a dictionary from InputValueEnum
        if len(value_enum) == 1:
            if value_enum[0] == "":
                if len(data) >= 1:
                    flag = True
                    ret = np.iinfo(np.uint32).max
                    while (flag == True):
                        for elem in data:
                            ret += 1
                            if ret == elem:
                                flag = True
                                break
                            if ret != elem:
                                flag = False
                    return ret

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
        # print("len(value_enum): ", len(value_enum))
        if len(value_enum) == 1:
            if value_enum[0] == "":
                return data
        # Map data inputs to their corresponding hex values
        return [
            value_enum_dict.get(key, "Unknown") for key in value_enum_dict.keys()
            if key not in {item.upper() for item in data}
        ]
