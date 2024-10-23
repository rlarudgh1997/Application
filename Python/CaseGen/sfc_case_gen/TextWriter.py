"""
TextWriter.py

This module provides a class of case string writer
"""

import os
import json

from .SignalCollection import SignalCollection

class TextWriter:
    def __init__(self, signal_collection_instance, file_path, history_output_file_path):
        self.instance = signal_collection_instance
        self.output_file_path = file_path
        self.history_output_file_path = history_output_file_path
        self.string_buffer = ""
        self.json_data = {}

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

    def get_signal_name_list(self):
        sig_list = list()
        for idx, sig in enumerate(self.instance.signals):
            sig_list.append(sig.InputSignalName)
        return sig_list

    def get_all_case_str(self):
        # 전체 경우의 수
        all_combinations_str = ', '.join([str(tup) for tup in self.instance.all_case])
        ret = ""
        ret += "All_case:\n"
        ret += all_combinations_str
        ret += "\n"
        return ret

    def get_satisfy_case_str(self):
        # 조건 만족 경우의 수
        satisfy_combinations_str = ', '.join([str(tup) for tup in self.instance.satisfy_case])
        ret = ""
        ret += "Satisfy_case:\n"
        ret += satisfy_combinations_str
        ret += "\n"
        return ret

    def get_other_case_str(self):
        # 조건 만족 경우의 수
        other_combinations_str = ', '.join([str(tup) for tup in self.instance.others_case])
        ret = ""
        ret += "Other_case:\n"
        ret += other_combinations_str
        ret += "\n"
        return ret

    def make_Json_buffer(self):
        if self.check_instance_type():
            signal_name_list = self.get_signal_name_list()
            self.json_data["Input_Signal_List"] = signal_name_list
            self.json_data["Cases"] = [list(item) for item in self.instance.satisfy_case]
            return True
        else:
            print("The instance is NOT of type SignalCollection.")
            return False

    def write_json(self):
        # 파일 열기 (덮어쓰기 모드 'w')
        try:
            # 단일 Case 결과만을 ItertoolsTest.Json에 저정
            with open(self.output_file_path, 'w', encoding='utf-8') as f:
                json.dump(self.json_data, f, ensure_ascii=False, indent=4)

            # # history JSON 파일에 현재 Case 결과물을 추가하여 저장
            # existing_data = []
            # if os.path.exists(self.history_output_file_path):
            #     with open(self.history_output_file_path, 'r', encoding='utf-8') as f:
            #         existing_data = json.load(f)
            # existing_data.append(self.json_data)
            # with open(self.history_output_file_path, 'w', encoding='utf-8') as f:
            #     json.dump(existing_data, f, ensure_ascii=False, indent=4)

        except Exception as e:
            print(f"Error occurred: {e}")

        file_size = os.path.getsize(self.output_file_path)
        print(f'File size: {file_size} bytes')
