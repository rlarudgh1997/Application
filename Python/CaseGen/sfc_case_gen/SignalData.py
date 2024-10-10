"""
SignalData.py

This module provides Data for VSM or SFC Signal
"""

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
