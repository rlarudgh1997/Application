"""
TextWriter.py

This module provides a class of case string writer
"""

import os
import json
import re

from .SignalCollection import SignalCollection


class TextWriter:
    def __init__(
        self, signal_collection_instance, file_path, config_path="Application.ini"
    ):
        self.instance = signal_collection_instance
        self.output_file_path = file_path
        self.string_buffer = ""
        self.json_data = {}
        self.config_path = config_path
        self.config_info = {}
        self.column_title_list = []
        # self._read_config_setting()

    def _read_config_setting(self):
        current_path = os.path.dirname(os.path.realpath(__file__))
        config_file = os.path.join(current_path, self.config_path)

        with open(config_file, "r") as file:
            for data in file.readlines():
                config = data.split("=")
                if len(config) == 2:
                    key, value = config[0].strip(), config[1].strip()
                    self.config_info[key] = value

        # self.sheet_names = self.config_info["ConfigTypeSheetName"].split(", ")
        self.column_title_list = self.config_info["ConfigTypeOtherTitle"].split(", ")

    def check_instance_type(self):
        if isinstance(self.instance, SignalCollection):
            return True
            # print("The instance is of type SignalCollection.")
        else:
            return False

    def get_string_buffer(self):
        if self.string_buffer:
            return self.string_buffer
        else:
            return "File Write Fail: Buffer is empty..."

    def make_string_buffer(self):
        if self.check_instance_type():
            if self.instance.all_case and self.instance.satisfy_case:
                self.string_buffer += self.get_signal_name_list()
                self.string_buffer += self.get_all_case_str()
                self.string_buffer += self.get_satisfy_case_str()
                self.string_buffer += self.get_other_case_str()
            return True
        else:
            print("The instance is NOT of type SignalCollection.")
            return False

    def get_signal_info(self):
        sig_list = dict()
        user_input_order_list = []
        input_empty_list = []
        for idx, key in enumerate(self.instance.signals):
            formatted_index = str(idx).zfill(3)
            sig = self.instance.signals[key]
            sig_list[formatted_index] = {
                "SignalName": sig.InputSignalName,
                "DataType": int(sig.InputDataType),
                "KeywordType": int(sig.InputKeywordType),
                "InputData": sig.InputDataHex,
                "Precondition": sig.InputPreconditionHex,
                "ValueEnum": sig.InputValueEnumDict,
            }
            user_input_order_list.append(sig.PreconditionIdx)
            if sig.InputDataHex == ['[Empty]']:
                input_empty_list.append("EmptyInputSignal")
            else:
                input_empty_list.append("")
        return sig_list, user_input_order_list, input_empty_list

    def get_all_case_str(self):
        # 전체 경우의 수
        all_combinations_str = ", ".join([str(tup) for tup in self.instance.all_case])
        ret = ""
        ret += "All_case:\n"
        ret += all_combinations_str
        ret += "\n"
        return ret

    def get_satisfy_case_str(self):
        # 조건 만족 경우의 수
        satisfy_combinations_str = ", ".join(
            [str(tup) for tup in self.instance.satisfy_case]
        )
        ret = ""
        ret += "Satisfy_case:\n"
        ret += satisfy_combinations_str
        ret += "\n"
        return ret

    def get_other_case_str(self):
        # 조건 만족 경우의 수
        other_combinations_str = ", ".join(
            [str(tup) for tup in self.instance.others_case]
        )
        ret = ""
        ret += "Other_case:\n"
        ret += other_combinations_str
        ret += "\n"
        return ret

    def make_Json_buffer(self):
        if self.check_instance_type():
            signal_name_list, user_input_order_list, input_empty_list = self.get_signal_info()
            # self.json_data["cases"] = [list(item) for item in self.instance.satisfy_case]
            self.json_data["cases"] = {
                ", ".join(item): list(item) for item in self.instance.satisfy_case
            }
            self.json_data["notTriggerPreconditionSet"] = {
                ", ".join(item): list(item) for item in self.instance.not_trigger_case
            }
            self.json_data["NotTriggerPreconditionSetSize"] = self.instance.not_trigger_case_size
            self.json_data["CaseSize"] = self.instance.satisfy_case_size
            self.json_data["InputSignalList"] = signal_name_list
            self.json_data["PreconditionOrder"] = user_input_order_list
            self.json_data["InputEmptyList"] = input_empty_list
            return True
        else:
            print("The instance is NOT of type SignalCollection.")
            return False

    def write_json(self):
        # 파일 열기 (덮어쓰기 모드 'w')
        try:
            json_str = json.dumps(self.json_data, indent=4)

            # 정규식을 사용하여 list의 들여쓰기를 제거하고 한 줄로 변환
            json_str = re.sub(
                r"(\[\n\s+)(.*?)(\n\s+\])",
                lambda match: "["
                + match.group(2).replace("\n    ", ", ").replace("\n", ", ")
                + "]",
                json_str,
                flags=re.DOTALL,
            )

            # 리스트 항목 사이의 중복 쉼표 제거
            json_str = re.sub(r",\s*,", ",", json_str)

            # 리스트 요소들 사이의 공백 줄이기 (모든 요소에 적용)
            json_str = re.sub(
                r"\[\s*([^]]*?)\s*\]",
                lambda m: "["
                + ",".join(item.strip() for item in m.group(1).split(",") if item)
                + "]",
                json_str,
            )

            # 단일 Case 결과만을 저정
            with open(self.output_file_path, "w", encoding="utf-8") as f:
                f.write(json_str)

        except Exception as e:
            print(f"Error occurred: {e}")

        file_size = os.path.getsize(self.output_file_path)
        if __debug__:
            print(f"File size: {file_size} bytes")
