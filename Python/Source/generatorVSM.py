from ast import Str
from xmlrpc.client import Boolean
from .parser import Parser
from .progress import Progress
import lib.defines as DEFINE

import os, sys
import shutil
import time
import re
from collections import OrderedDict
from typing import NamedTuple, Any
class TranslateSpec(NamedTuple):
    value_type: str             # uint64_t, int64_t
    value_suffix: str           # UL, L
    default_timeout_value: str  # kTimeOut, static_cast<int64_t>(kTimeOut)
    value_formatter: Any        # hexa_formatter, decimal_formatter
    signalvalue_getter: str     # SignalValue::'getValueUInt64()', 'getValueInt64()'
    shift_rule_applicable: bool

def hexa_formatter(value):
    return "0x%x" % value

def decimal_formatter(value):
    return "%d" % value

def is_number(s):
    try:
        float(s)
    except:
        return False
    else:
        return True


def error(self, message: str):
    self.progress.erase_prev_progress_message()
    print("\u001b[31m%s\u001b[0m" % message)
    sys.exit(-1)


def warning(self, message: str):
    self.progress.erase_prev_progress_message()
    print("\u001b[33m%s\u001b[0m" % message)


class Generator:
    translateSpecs = {
        "unsigned" :
            TranslateSpec(value_type="uint64_t", value_suffix="UL", default_timeout_value="kTimeOut",
            value_formatter=hexa_formatter, signalvalue_getter="getRawValue()", shift_rule_applicable=True),
        "signed" :
            TranslateSpec(value_type="int64_t", value_suffix="L", default_timeout_value="static_cast<int64_t>(kTimeOut)",
            value_formatter=decimal_formatter, signalvalue_getter="getValueInt64()", shift_rule_applicable=False)}

    def __init__(self, parser):
        if type(parser) is not Parser:
            print("Not a Parser object")
            return
        self.parser = parser
        self.source_storage = {}
        self.feature = "full"

        template_root_header_file = open(os.path.join(sys.path[0], DEFINE.TEMPLATE_ROOT_CLASS_FILE_PATH + ".h"), 'r')
        self.top_template_header_root_class = template_root_header_file.read()
        template_root_header_file.close()

        template_header_file = open(os.path.join(sys.path[0], DEFINE.TEMPLATE_CLASS_FILE_PATH + ".h"), 'r')
        self.template_header_root_class = template_header_file.read()
        template_header_file.close()

        inner_template_header_file = open(os.path.join(sys.path[0], DEFINE.TEMPLATE_INNERCLASS_FILE_PATH + ".h"), 'r')
        self.template_header_inner_class = inner_template_header_file.read()
        inner_template_header_file.close()

        template_source_file = open(os.path.join(sys.path[0], DEFINE.TEMPLATE_CLASS_FILE_PATH + ".cpp"), 'r')
        self.template_source_root_class = template_source_file.read()
        template_source_file.close()

        inner_template_source_file = open(os.path.join(sys.path[0], DEFINE.TEMPLATE_INNERCLASS_FILE_PATH + ".cpp"), 'r')
        self.template_source_inner_class = inner_template_source_file.read()
        inner_template_source_file.close()

        self.generate_buffer = ""
        self.node_address = list()
        self.node_address_title = list()
        self.node_address_duplicate = list()
        self.count = 0

    @staticmethod # get TranslateSpecs for data_type
    def get_translate_spec(data_type: str):
        # data_type to spec key
        _translate_key_map = {"HUInt64": "unsigned", "HUInt32": "unsigned", "HUInt16": "unsigned", "HUInt8": "unsigned",
                         "HInt64": "signed", "HInt32": "signed", "HInt16": "signed", "HInt8": "signed"}
        if data_type not in _translate_key_map:
            return None
        _translate_key = _translate_key_map[data_type]
        if _translate_key not in Generator.translateSpecs:
            return None
        return Generator.translateSpecs[_translate_key]

    def generate(self, target_dir, feature):
        data = self.parser.data
        self.feature = feature

        try:
            header_folder_path = target_dir + "/include/generated/"
            if not (os.path.isdir(header_folder_path)):
                os.makedirs(os.path.join(header_folder_path))
            else:
                os.remove(os.path.join(header_folder_path, "V*"))
        except OSError:
            pass

        try:
            source_folder_path = target_dir + "/src/generated/"
            if not (os.path.isdir(source_folder_path)):
                os.makedirs(os.path.join(source_folder_path))
            else:
                os.remove(os.path.join(source_folder_path, "*.cpp"))
        except OSError:
            pass

        progress_max = self.parser.get_node_count(data)
        self.progress = Progress("Generating code slices for %d nodes " % progress_max, progress_max)
        self.progress.set_progress(0, "")

        self.do_generate(target_dir=target_dir, data=data, progress=0)
        self.progress.set_progress(progress_max, "")
        print("")

        progress_max = progress_max * 2
        self.progress = Progress("Generating sources for %d nodes " % progress_max, progress_max)
        self.merge_progress = 0
        self.progress.set_progress(0, "")

        self.do_merge_inner_class_header(node_data=data)
        self.do_merge_inner_class_source(node_data=data, parent_scope="")
        self.write_src_buffer(target_dir=target_dir)

        self.progress.set_progress(progress_max, "")
        print("")
        print("Done")

        self.write_node_address(target_dir)

    def get_ccos_type_string(self, type_string: str):
        if type_string == "HTriState":
            return "HTriState"
        elif type_string == "HSwitchState":
            return "HSwitchState"
        elif type_string == "HUInt" or type_string == "HUInt8" or type_string == "HUInt16" or type_string == "HUInt32" or type_string == "HUInt64":
            return "ccos::HUInt64"
        elif type_string == "HInt" or type_string == "HInt8" or type_string == "HInt16" or type_string == "HInt32" or type_string == "HInt64":
            return "ccos::HInt64"
        elif type_string == "HFloat":
            return "ccos::HDouble"
        elif type_string == "HBool":
            return "ccos::HBool"
        elif type_string == "HString":
            return "std::string"
        elif type_string == "N/A":
            return None
        else:
            warning(self, "  # %s is not handled" % type_string)
            return "ccos::%s" % type_string

    def get_raw_value_type_string(self, type_string: str):
        if type_string == "TWOS_COMPLEMENT":
            return "RawValueType::TWOS"
        elif type_string == "ONES_COMPLEMENT":
            return "RawValueType::ONES"
        elif type_string == "SIGN_MAGNITUDE":
            return "RawValueType::SIGN"
        else:
            warning(self, "  # %s is not handled" % type_string)
            return "RawValueType::NA"

    def covert_vehicle_signal_value_type(self, type_string):
        if type_string == "HTriState":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::TRISTATE"
        elif type_string == "HSwitchState":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::SWITCHSTATE"
        elif type_string == "HUInt" or type_string == "HUInt8" or type_string == "HUInt16" or type_string == "HUInt32" or type_string == "HUInt64":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::UINT64"
        elif type_string == "HInt" or type_string == "HInt8" or type_string == "HInt16" or type_string == "HInt32" or type_string == "HInt64":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::INT64"
        elif type_string == "HFloat":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::DOUBLE"
        elif type_string == "HBool":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::BOOL"
        elif type_string == "HString":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::STRING"
        elif type_string == "std::string":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::STRING"
        elif type_string == "N/A":
            return None
        else:
            warning(self, "  # %s is not handled for VehicleSignalValueType" % type_string)
            return None

    # get comment valueEnum
    def get_comment_innerclass(self, node_data: dict):
        buffer = ""
        if "valueEnum" in node_data.keys():
            count = 0
            for value in node_data["valueEnum"].keys():
                strData = str(value)
                result = strData.find('~')
                if count == 0:
                    buffer = "\n/**\n * Value Description"
                if result == -1:
                    if is_number(value) is False:
                        error(self,
                              "valueEnum check dataType!! can not proceed. [signalName : %s, %s]\n       %s : \"%s\""
                              % (node_data["signalName"], node_data["address"], strData, node_data["valueEnum"][value]))
                    description = node_data["valueEnum"][value]
                    field_list = description.split("->")
                    buffer = buffer + "\n *  0x%X: \"%s\"" % (value, field_list[0].strip())
                else:
                    splitData = strData.split('~')
                    description = node_data["valueEnum"][value]
                    field_list = description.split("->")
                    try:
                        if is_number(int(splitData[0], 16)) is False or is_number(int(splitData[1], 16)) is False: {}
                    except:
                        error(self,
                              "valueEnum check dataType!! can not proceed. [signalName : %s, %s]\n       %s : \"%s\""
                              % (node_data["signalName"], node_data["address"], strData, node_data["valueEnum"][value]))
                    buffer = buffer + "\n *  0x%X~0x%X: \"%s\"" % (int(splitData[0], 16),
                                                                   int(splitData[1], 16),
                                                                   field_list[0].strip())
                count += 1
            buffer = buffer + "\n */"
        return buffer

    # get public function
    def get_public_field_header(self, node_data: dict):
        buffer = ""
        if "dataType" in node_data.keys() and self.get_ccos_type_string(node_data["dataType"]) is not None:
            if "min" in node_data.keys() and node_data["min"] is not None and len(str(node_data["min"]).strip()) > 0:
                buffer = buffer + "\n"
                buffer = buffer + "\n    ccos::HInt64 getMinValue() override;"

            if "max" in node_data.keys() and node_data["max"] is not None and len(str(node_data["max"]).strip()) > 0:
                buffer = buffer + "\n"
                buffer = buffer + "\n    ccos::HInt64 getMaxValue() override;"

            if ("max" in node_data.keys() and "min" not in node_data.keys()):
                warning(self, "  # min is not defined.     [signalName : %s, %s]" % (node_data["signalName"],
                                                                                     node_data["address"]))
            if ("min" in node_data.keys() and "max" not in node_data.keys()):
                warning(self, "  # max is not defined.     [signalName : %s, %s]" % (node_data["signalName"],
                                                                                     node_data["address"]))

            if ("min" in node_data.keys() and node_data["min"] is None or "max" in node_data.keys()
                    and node_data["max"] is None):
                warning(self, "  # check min or max value. [signalName : %s, %s] " % (node_data["signalName"],
                                                                                      node_data["address"]))
                if "min" in node_data.keys() and node_data["min"] is not None:
                    warning(self, "        min : %s" % node_data["min"])
                else:
                    warning(self, "        min :")

                if "max" in node_data.keys() and node_data["max"] is not None:
                    warning(self, "        max : %s" % node_data["max"])
                else:
                    warning(self, "        max :")

        buffer = buffer + "\n"
        # buffer = buffer + "    std::string getDescription() const override;"
        # buffer = buffer + "\n"
        buffer = buffer + "    std::string getNodeType() const override;"

        if "dataType" in node_data.keys():
            if self.covert_vehicle_signal_value_type(node_data["dataType"]) is not None:
                buffer = buffer + "\n"
                buffer = buffer + "    ccos::vehicle::vsm::HVehicleSignalValueType getVehicleSignalValueType() override;"
            elif "valueEnum" in node_data.keys():
                warning(self, "  # %s (type : %s) have valueEnum but does not have dataType (dataType is %s)" % (
                    node_data["address"], node_data["type"], node_data["dataType"]))
            else:
                pass
        elif node_data["type"] == "branch" or node_data["type"] == "tagged_branch" or node_data["type"] == "tag":
            pass
        else:
            warning(self, "  # %s (type : %s) does have data type" % (node_data["address"], node_data["type"]))

        if node_data["type"] == "tagged_branch" or node_data["type"] == "tag":
            pass
        elif node_data["type"] == "branch":  # try for 'pseudo actuator branch node' case
            type_helper = self.get_type_helper_header_for_branch(node_data)
            if type_helper != "":
                buffer += "\n"
                buffer += type_helper
        else:
            buffer += "\n"
            buffer += self.get_type_helper_header(node_data)
            buffer += self.get_translate_function_header(node_data)
            buffer += self.get_reverse_translate_function_header(node_data)
            buffer += self.get_rawtype_function_header(node_data)
        return buffer

    # get public function
    def get_public_field_source(self, node_data: dict, parent_scope):
        buffer = ""
        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"

        if "dataType" in node_data.keys() and self.get_ccos_type_string(node_data["dataType"]) is not None:
            if "min" in node_data.keys() and node_data["min"] is not None and len(str(node_data["min"]).strip()) > 0:
                buffer = buffer + "\n"
                buffer = buffer + "\nccos::HInt64 " + current_scope + "getMinValue() {"
                buffer = buffer + "\nreturn mMin;"
                buffer = buffer + "\n}"

            if "max" in node_data.keys() and node_data["max"] is not None and len(str(node_data["max"]).strip()) > 0:
                buffer = buffer + "\n"
                buffer = buffer + "\nccos::HInt64 " + current_scope + "getMaxValue() {"
                buffer = buffer + "\n        return mMax;"
                buffer = buffer + "\n    }"

        # buffer = buffer + "\n"
        # buffer = buffer + "std::string " + current_scope + "getDescription() const {"
        # if "description" in node_data.keys():
        #     buffer = buffer + "\n    return mDescription;"
        # else:
        #     buffer = buffer + "\n    return nullptr;"
        # buffer = buffer + "\n}"

        buffer = buffer + "\n"
        buffer = buffer + "std::string " + current_scope + "getNodeType() const {"
        if "type" in node_data.keys():
            buffer = buffer + "\n    return mType;"
        else:
            buffer = buffer + "\n    return \"branch\";"
        buffer = buffer + "\n}"

        if "dataType" in node_data.keys():
            if self.covert_vehicle_signal_value_type(node_data["dataType"]) is not None:
                buffer = buffer + "\n"
                buffer = buffer + "ccos::vehicle::vsm::HVehicleSignalValueType\n"
                buffer = buffer + "        " + current_scope + "getVehicleSignalValueType() {"
                buffer = buffer + "\n    return %s;" % self.covert_vehicle_signal_value_type(node_data["dataType"])
                buffer = buffer + "\n}"
            else:
                pass

        if node_data["type"] != "branch" and node_data["type"] != "tagged_branch" and node_data["type"] != "tag":
            buffer += "\n"
            buffer += self.get_type_helper_source(node_data, parent_scope)
            buffer += self.get_translate_function_source(node_data, parent_scope)
            buffer += self.get_reverse_translate_function_source(node_data, parent_scope)
            buffer += self.get_rawtype_function_source(node_data, parent_scope)
        return buffer

    def get_type_helper_header(self, node_data: dict):
        if node_data["type"] == "branch" or node_data["type"] == "tagged_branch" or node_data["type"] == "tag":
            return ""
        _type_buffer = ""
        _using_type = ""
        if "valueEnum" in node_data:
            # print( node_data["valueEnum"])
            _type_buffer += "    enum class Type : uint64_t {\n"
            for _value in node_data["valueEnum"].keys():
                _type_buffer += "        " + node_data["valueEnum"][_value].upper() + " = " + hex(_value) + ",\n"
            if "TIMEOUT" not in node_data["valueEnum"].keys() and node_data["type"] is not "actuator":
                if "matchingTable" not in node_data.keys() \
                        or node_data["matchingTable"] is None \
                        or ("TIMEOUT" not in node_data["matchingTable"] and "ELSE" not in node_data["matchingTable"]):
                    _type_buffer += "        UNHANDLED_TIMEOUT = kTimeOut,  // Inserted by auto-generation(nomatchingTable).\n"
            _type_buffer += "    };\n"
            _using_type = "Type"
        else:
            if "dataType" in node_data:
                _using_type = self.get_ccos_type_string(node_data["dataType"])
            else:
                error(self, "%s does not have data type filed" % node_data["name"])
                return _type_buffer
        _type_buffer += "    using TYPE = " + _using_type + ";\n"
        _type_buffer += "    #ifndef SSFS_INTERNAL\n"
        _type_buffer += "    TYPE value(const ccos::vehicle::vsm::HVehicleSignal& hvehicleSignal) const;\n"
        _type_buffer += "    #endif\n"
        return _type_buffer

    def get_type_helper_header_for_branch(self, node_data: dict):
        # it process for pseudo actuator node/ branch type node with valueEnum or dataType
        if node_data["type"] != "branch":
            return ""
        if "valueEnum" not in node_data and "dataType" not in node_data:
            return ""
        _using_type = ""
        _type_buffer = "\n    // provide TYPE info for pseudo actuator branch node\n"
        if "valueEnum" in node_data:
            _type_buffer += "    enum class Type : uint64_t {\n"
            for _value in node_data["valueEnum"].keys():
                _type_buffer += "        " + node_data["valueEnum"][_value].upper() + " = " + hex(_value) + ",\n"
            _type_buffer += "    };\n"
            _using_type = "Type"
        else:  # "dataType" in node_data:
            _using_type = self.get_ccos_type_string(node_data["dataType"])
        _type_buffer += "    using TYPE = " + _using_type + ";\n"
        return _type_buffer

    def get_type_helper_source(self, node_data: dict, parent_scope):
        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        if node_data["type"] == "branch" or node_data["type"] == "tagged_branch" or node_data["type"] == "tag":
            return ""
        _type_buffer = ""
        _getter_type_list = {"HUInt32": "HUInt64", "HUInt16": "HUInt64", "HUInt8": "HUInt64",
                             "HInt32": "HInt64", "HInt16": "HInt64", "HInt8": "HInt64",
                             "HTriState": "HUInt64", "HSwtichState": "HUInt64"}
        _getter_type = ""
        if "valueEnum" in node_data:
            _getter_type = self.get_ccos_type_string("HUInt64")
        else:
            if "dataType" in node_data:
                if node_data["dataType"] in _getter_type_list:
                    _getter_type = self.get_ccos_type_string(_getter_type_list[node_data["dataType"]])
                else:
                    _getter_type = self.get_ccos_type_string(node_data["dataType"])
            else:
                error(self, "%s does not have data type filed" % node_data["name"])
                return _type_buffer
        _type_buffer += "#ifndef SSFS_INTERNAL\n"
        _type_buffer += current_scope + "TYPE\n"
        _type_buffer += "        " + current_scope + "value(\n"
        _type_buffer += "        const ccos::vehicle::vsm::HVehicleSignal& hvehicleSignal) const {\n"
        _type_buffer += "    return static_cast<" + current_scope + "TYPE>(\n"
        _type_buffer += "        hvehicleSignal.getValue<"
        _type_buffer += _getter_type + ">());\n"
        _type_buffer += "}\n"
        _type_buffer += "#endif\n"
        return _type_buffer

    def get_translate_function_header(self, node_data: dict):
        if "matchingTable" not in node_data:
            return "    // No matching table for translate.\n"
        if "dataType" not in node_data:
            return "    // No dataType defined for translate. (Incompatible Node) \n"
        _spec = self.get_translate_spec(node_data["dataType"])
        if _spec is None:
            return "    // Incompatible dataType (%s) for translate \n" % node_data["dataType"]

        _matching_table = node_data["matchingTable"]
        if _matching_table is None:
            if "node_tag" not in node_data or node_data["node_tag"] != "SKEL":
                pass
                #warning(self, "%s has matchingTableField but None." % node_data["address"])
            _buffer = "    // No matching table in VSM. fallback translate function.\n"
            _buffer += "    #if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
            _buffer += "    bool translate(const SignalValue& signalValue, " + _spec.value_type + "& value) override;\n"
            _buffer += "    #endif\n"
            return _buffer

        _buffer = "    #if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "    bool translate(const SignalValue& signalValue, " + _spec.value_type + "& value) override;\n"
        _buffer += "    #endif\n"
        return _buffer

    def get_reverse_translate_function_header(self, node_data: dict):
        if "matchingTable" not in node_data:
            return "    // No matching table for reverse translate.\n"
        _data_type = self.get_ccos_type_string(node_data["dataType"])
        _matching_table = node_data["matchingTable"]
        if _matching_table is None:
            if "node_tag" not in node_data or node_data["node_tag"] != "SKEL":
                pass
                #warning(self, "%s has matchingTableField but None." % node_data["address"])
            return "    // No matching table.(wrong format in VSM)\n"

        _buffer = "    #if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "    bool reverseTranslate(const uint64_t& value, uint64_t& rawValue) override;\n"
        _buffer += "    #endif\n"
        return _buffer

    def get_translate_function_source(self, node_data: dict, parent_scope):
        if "matchingTable" not in node_data or "dataType" not in node_data:
            return ""
        _spec = self.get_translate_spec(node_data["dataType"])
        if _spec is None:
            return ""

        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        _matching_table = node_data["matchingTable"]
        _shift_rule_applicable = _spec.shift_rule_applicable and "mask" in node_data.keys() and "shift" in node_data.keys()

        _buffer = ""
        if _matching_table is None:
            _buffer += "// No matching table in VSM. using fallback\n"
            _buffer += "#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
            _buffer += "bool " + current_scope + "translate(\n"
            _buffer += "        const SignalValue& signalValue, " + _spec.value_type + "& value) {\n"
            _buffer += "    if (signalValue.isTimeOut()) {\n"
            _buffer += "        value = " + _spec.default_timeout_value + ";\n"
            _buffer += "        return true;\n"
            _buffer += "    }\n"
            _buffer += "    value = signalValue." + _spec.signalvalue_getter + ";\n"
            if _shift_rule_applicable:
                _buffer += "    if (usingMask()) {\n"
                _buffer += "        value = value & getMask();\n"
                _buffer += "        auto shift = getShift();\n"
                _buffer += "        if (shift > 0) {\n"
                _buffer += "            value = value << staic_cast<std::make_unsigned_t<decltype(shift)>>(std::abs(shift));\n"
                _buffer += "        } else if (shift < 0) {\n"
                _buffer += "            value = value >> staic_cast<std::make_unsigned_t<decltype(shift)>>(std::abs(shift));\n"
                _buffer += "        } else {\n"
                _buffer += "            // nothing\n"
                _buffer += "        }\n"
                _buffer += "    }\n"
            _buffer += "    return true;\n"
            _buffer += "}\n"
            _buffer += "#endif\n"
            return _buffer

        _has_old_value = False
        for _condition in _matching_table.keys():
            if type(_matching_table[_condition]) == OrderedDict and "OLDVALUE" in _matching_table[_condition]:
                _has_old_value = True
                break
        _buffer += "#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "bool " + current_scope + "translate(\n"
        _buffer += "        const SignalValue& signalValue, " + _spec.value_type + "& value) {\n"
        if _has_old_value:
            _buffer += "    static " + _spec.value_type + " oldValue = 0" + _spec.value_suffix + ";  // dummy old value\n"
            _buffer += "    static bool isOldValueValid = false;\n"

        if "TIMEOUT" in _matching_table:
            _buffer += "    if (signalValue.isTimeOut()) {\n"
            if type(_matching_table["TIMEOUT"]) is OrderedDict and "OLDVALUE" in _matching_table["TIMEOUT"]:
                if "ELSE" in _matching_table and type(_matching_table["ELSE"]) is int:
                    _buffer += "        if (!isOldValueValid) {\n"
                    _buffer += "            value = %s;\n" % _spec.value_formatter(_matching_table["ELSE"])
                    _buffer += "            return true;\n"
                    _buffer += "        }\n"
                else:
                    _buffer += "        if (!isOldValueValid) {\n"
                    _buffer += "            value = " + _spec.default_timeout_value + ";  // No old value and timeout.\n"
                    _buffer += "            return true;\n"
                    _buffer += "        }\n"
                _buffer += "        value = oldValue;\n"
                _buffer += "        return true;\n"
            else:
                if type(_matching_table["TIMEOUT"]) is str:
                    warning(self, "[address:%s] matchingTable(%s) is not suitable." % \
                          (node_data["address"], _matching_table["TIMEOUT"]))
                    _buffer += "        value = " + _spec.default_timeout_value + ";  // timeout is not usable.\n"
                    _buffer += "        return true;\n"
                else:
                    _buffer += "        value = %s;\n" % _spec.value_formatter(_matching_table["TIMEOUT"])
                    _buffer += "        return true;\n"
            _buffer += "    }\n"
        elif _has_old_value:
            _buffer += "    if (signalValue.isTimeOut()) {\n"
            _buffer += "        if (!isOldValueValid) {\n"
            if "ELSE" in _matching_table and type(_matching_table["ELSE"]) is int:
                _buffer += "            value = %s;  // Inserted by auto-generation(ELSE) for invalid old value\n" % _spec.value_formatter(_matching_table["ELSE"])
            else:
                _buffer += "            value = " + _spec.default_timeout_value + ";  // no timeout rule defined and can not use else for invalid old value.\n"
            _buffer += "            return true;\n"
            _buffer += "        }\n"
            _buffer += "        value = oldValue;\n"
            _buffer += "        return true;\n"
            _buffer += "    }\n"
        else:
            _buffer += "    if (signalValue.isTimeOut()) {\n"
            if "ELSE" in _matching_table and type(_matching_table["ELSE"]) is int:
                _buffer += "        value = %s;  // Inserted by auto-generation(ELSE)\n" % _spec.value_formatter(_matching_table["ELSE"])
            else:
                _buffer += "        value = " + _spec.default_timeout_value + ";  // no timeout rule defined and can not use else.\n"
            _buffer += "        return true;\n"
            _buffer += "    }\n"
        if "CRCERROR" in _matching_table:
            _buffer += "    if (signalValue.isCRCError()) {\n"
            if type(_matching_table["CRCERROR"]) is OrderedDict and "OLDVALUE" in _matching_table["CRCERROR"]:
                _buffer += "        if (!isOldValueValid) {\n"
                _buffer += "            return false;\n"
                _buffer += "        }\n"
                _buffer += "        value = oldValue;\n"
                _buffer += "        return true;\n"
            else:
                _buffer += "        value = %s;\n" % _matching_table["CRCERROR"]
                _buffer += "        return true;\n"
            _buffer += "    }\n"

        _signal_value_buffer = ""
        if _shift_rule_applicable:
            _signal_value_buffer += "    auto signal_value = signalValue." + _spec.signalvalue_getter + ";\n"
            _signal_value_buffer += "    if (usingMask()) {\n"
            _signal_value_buffer += "        signal_value = signal_value & getMask();\n"
            _signal_value_buffer += "        auto shift = getShift();\n"
            _signal_value_buffer += "        if (shift > 0) {\n"
            _signal_value_buffer += "            signal_value = signal_value << static_cast<std::make_unsigned_t<decltype(shift)>>(std::abs(shift));\n"
            _signal_value_buffer += "        } else if (shift < 0) {\n"
            _signal_value_buffer += "            signal_value = signal_value >> static_cast<std::make_unsigned_t<decltype(shift)>>(std::abs(shift));\n"
            _signal_value_buffer += "        } else {\n"
            _signal_value_buffer += "           // nothing\n"
            _signal_value_buffer += "        }\n"
            _signal_value_buffer += "    }\n"
        else:
            _signal_value_buffer += "    const auto& signal_value = signalValue." + _spec.signalvalue_getter + ";\n"

        _switch_clause_has_case = False
        _switch_clause_buffer = ""
        _switch_clause_buffer += "    bool ret = true;\n"
        _switch_clause_buffer += "    switch (signal_value) {\n"
        for _condition in _matching_table.keys():
            if _condition == "TIMEOUT" or _condition == "ELSE" or _condition == "VALUE" or _condition == "CRCERROR":
                continue
            if type(_condition) is str:
                _splited = _condition.split('~')
                if len(_splited) == 1:
                    error(self, "[address:%s] condition(%s) is not suitable." % (node_data["address"], _condition))

                for _condition_range_element in range(int(_splited[0], 16), int(_splited[1], 16)):
                    _switch_clause_buffer += "        case %s:  // fall through\n" % _spec.value_formatter(_condition_range_element)
                _switch_clause_buffer += "        case %s: {\n" % _spec.value_formatter(int(_splited[1], 16))
            else:
                _switch_clause_buffer += "        case %s: {\n" % _spec.value_formatter(_condition)
            _switch_clause_has_case = True
            if type(_matching_table[_condition]) is OrderedDict and "OLDVALUE" in _matching_table[_condition]:
                _switch_clause_buffer += "            if (!isOldValueValid) {\n"
                _switch_clause_buffer += "                ret = false;\n"
                _switch_clause_buffer += "            } else {\n"
                _switch_clause_buffer += "                value = oldValue;\n"
                _switch_clause_buffer += "            }\n"
            else:
                if type(_matching_table[_condition]) is str:
                    error(self,
                          "[address:%s] matchingTable(%s) is not suitable." % (node_data["address"], _matching_table[_condition]))
                elif type(_matching_table[_condition]) is list:
                    _switch_clause_buffer += "            value = %s;\n" % _spec.value_formatter(_matching_table[_condition][0])  # use first item
                else:
                    _switch_clause_buffer += "            value = %s;\n" % _spec.value_formatter(_matching_table[_condition])
                if _has_old_value:
                    _switch_clause_buffer += "            oldValue = value;\n"
                    _switch_clause_buffer += "            isOldValueValid = true;\n"
            _switch_clause_buffer += "            break;\n"
            _switch_clause_buffer += "        }\n"
        _switch_clause_buffer += "        default: {\n"
        if "ELSE" in _matching_table:
            if "type" in node_data.keys():
                if node_data["type"] == "actuator":
                    error(self, "[address:%s] actuator type is not support! ELSE matchingTable." % node_data["address"])

            if type(_matching_table["ELSE"]) is OrderedDict and "OLDVALUE" in _matching_table["ELSE"]:
                _switch_clause_buffer += "            if (!isOldValueValid) {\n"
                _switch_clause_buffer += "                ret = false;\n"
                _switch_clause_buffer += "            } else {\n"
                _switch_clause_buffer += "                value = oldValue;\n"
                _switch_clause_buffer += "            }\n"
            else:
                _switch_clause_buffer += "            value = %s;\n" % _spec.value_formatter(_matching_table["ELSE"])
                if _has_old_value:
                    _switch_clause_buffer += "            oldValue = value;\n"
                    _switch_clause_buffer += "            isOldValueValid = true;\n"
        else:
            _switch_clause_buffer += "            value = signal_value;\n"
            if _has_old_value:
                _switch_clause_buffer += "            oldValue = value;\n"
                _switch_clause_buffer += "            isOldValueValid = true;\n"
        _switch_clause_buffer += "            break;\n"
        _switch_clause_buffer += "        }\n"
        _switch_clause_buffer += "    }\n"
        _switch_clause_buffer += "    return ret;\n"

        if _switch_clause_has_case:
            _buffer += _signal_value_buffer
            _buffer += _switch_clause_buffer
        else:
            if "ELSE" in _matching_table:
                if "type" in node_data.keys():
                    if node_data["type"] == "actuator":
                        error(self, "[address:%s] actuator type does not support! ELSE matchingTable." % node_data["address"])

                if type(_matching_table["ELSE"]) is OrderedDict and "OLDVALUE" in _matching_table["ELSE"]:
                    error(self, "[address:%s] ELSE with OLDVALUE w/o other case clause is non-sense!" % node_data["address"])
                    _buffer += "    return false;\n"
                else:
                    _buffer += "    value = %s;\n" % _spec.value_formatter(_matching_table["ELSE"])
                    if _has_old_value:
                        _buffer += "    oldValue = value;\n"
                        _buffer += "    isOldValueValid = true;\n"
                    _buffer += "    return true;\n"
            else:
                _buffer += _signal_value_buffer
                _buffer += "    value = signal_value;\n"
                if _has_old_value:
                    _buffer += "    oldValue = value;\n"
                    _buffer += "    isOldValueValid = true;\n"
                _buffer += "    return true;\n"
        _buffer += "}\n"
        _buffer += "#endif\n"
        return _buffer

    def get_reverse_translate_function_source(self, node_data: dict, parent_scope):
        if "matchingTable" not in node_data:
            return ""
        _matching_table = node_data["matchingTable"]
        if _matching_table is None:
            return ""

        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        _buffer = "#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "bool " + current_scope + "reverseTranslate(\n"
        _buffer += "        const uint64_t& value, uint64_t& rawValue) {\n"

        has_else = False
        condition_values = list() # except ELSE
        reverse_values = list()
        for _condition in _matching_table.keys():
            if _condition == "TIMEOUT" or _condition == "VALUE" or _condition == "CRCERROR":
                continue
            if type(_matching_table[_condition]) is OrderedDict and "OLDVALUE" in _matching_table[_condition]:
                continue
            if _condition == "ELSE":
                has_else = True
            elif not _matching_table[_condition] in condition_values:
                # reverse match 'int or [list]' into 'key'
                if type(_matching_table[_condition]) is list:
                    value_list = _matching_table[_condition]
                else:
                    value_list = []
                    value_list.append(_matching_table[_condition])
                for value_item in value_list:
                    _buffer += "    if (value == 0x%x) {\n" % value_item
                    condition_values.append(value_item)
                    if type(_condition) is str and _condition.find('~') > 0:
                        _buffer += "        rawValue = 0x%x;\n" % int(_condition.split('~')[0], 16)
                        _splited = _condition.split('~')
                        if len(_splited) == 1:
                            error(self, "[address:%s] condition(%s) is not suitable." % (node_data["address"], _condition))
                        for _condition_range_element in range(int(_splited[0], 16), int(_splited[1], 16) + 1):
                            reverse_values.append(_condition_range_element)
                    else:
                        _buffer += "        rawValue = 0x%x;\n" % _condition
                        reverse_values.append(_condition)
                    _buffer += "        return true;\n"
                    _buffer += "    }\n"
            else:
                if type(_condition) is str and _condition.find('~') > 0:
                    _splited = _condition.split('~')
                    if len(_splited) == 1:
                        error(self, "[address:%s] condition(%s) is not suitable." % (node_data["address"], _condition))
                    for _condition_range_element in range(int(_splited[0], 16), int(_splited[1], 16) + 1):
                        reverse_values.append(_condition_range_element)
                else:
                    reverse_values.append(_condition)
        if has_else:
            else_condition_value = _matching_table["ELSE"]
            if not else_condition_value in condition_values:
                _buffer += "    if (value == 0x%x) {\n" % else_condition_value
                _buffer += "        rawValue = %sU;\n" % self.get_default_value_for_reverse_translate(reverse_values)
                _buffer += "        return true;\n"
                _buffer += "    }\n"
            else:
                _buffer += "    // revserse ELSE rule is not available\n"

        _buffer += "    rawValue = value;\n"
        _buffer += "    return true;\n"
        _buffer += "}\n"
        _buffer += "#endif\n"
        return _buffer

    def get_default_value_for_reverse_translate(self, reverse_values: list):
        for _number in range(10000):
            if not _number in reverse_values:
                return hex(_number)
        error(self, "Cannot find default value for reverse translate!!")
        return "0x0000;  // please check this out!!"

    def get_rawtype_function_header(self, node_data: dict):
        if "rawType" not in node_data:
            return ""
        _buffer = "    #if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "    RawValueType getRawValueType() const override;\n"
        _buffer += "    uint32_t getRawValueSize() const override;\n"
        _buffer += "    #endif\n"
        return _buffer

    def get_rawtype_function_source(self, node_data: dict, parent_scope):
        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        if "rawType" not in node_data:
            return ""
        _raw_value_type = self.get_raw_value_type_string(node_data["rawType"])
        _raw_value_size = 64
        if "rawSize" in node_data:
            _raw_value_size = node_data["rawSize"]

        _buffer = "#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)\n"
        _buffer += "RawValueType " + current_scope + "getRawValueType() const {\n"
        _buffer += "    return %s;\n" % _raw_value_type
        _buffer += "}\n"
        _buffer += "uint32_t " + current_scope + "getRawValueSize() const {\n"
        _buffer += "    return %d;\n" % _raw_value_size
        _buffer += "}\n"
        _buffer += "#endif\n"

        return _buffer

    # get protected function
    def get_protected_field_header(self, node_data: dict):
        buffer = ""
        # make bit
        if "description" in node_data.keys() and 'Bit' in node_data["description"]:
            buffer = buffer + "\n    ccos::HUInt64 calculateValue(ccos::HUInt64 &value) override;"

        # check factor & offset
        if "offset" in node_data.keys() and "factor" not in node_data.keys():
            warning(self, "  # factor is not defined.  [signalName : %s, %s] " % (node_data["signalName"],
                                                                                  node_data["address"]))
        if "factor" in node_data.keys() and "offset" not in node_data.keys():
            pass
            # warning(self, "  # offset is not defined.  [signalName : %s, %s] " % (node_data["signalName"],
            #                                                                       node_data["address"]))

        if "mask" in node_data.keys():
            if node_data["mask"] is not None:
                buffer = buffer + "\n    ccos::HUInt64 getMask() override;"

        if "shift" in node_data.keys():
            if node_data["shift"] is not None:
                buffer = buffer + "\n    ccos::HInt64 getShift() override;"

        if "factor" in node_data.keys():
            if node_data["factor"] is not None:
                if str(node_data["factor"]) != "1":
                    buffer = buffer + "\n    ccos::HDouble getFactor() override;"

        if "offset" in node_data.keys():
            if node_data["offset"] is not None:
                if str(node_data["offset"]) != "0":
                    buffer = buffer + "\n    ccos::HInt64 getOffset() override;"

        if "factor" in node_data.keys() and "offset" in node_data.keys():
            if node_data["factor"] is not None and node_data["offset"] is not None:
                if str(node_data["factor"]) != "1" or str(node_data["offset"]) != "0":
                    buffer = buffer + "\n"
                    buffer = buffer + "\n    bool usingCorrection() override;"

        if "mask" in node_data.keys() and "shift" in node_data.keys():
            if node_data["mask"] is not None and node_data["shift"] is not None:
                buffer = buffer + "\n"
                buffer = buffer + "\n    bool usingMask() override;"

        # check min max value
        if "min" in node_data.keys() and node_data["min"] is not None and len(str(node_data["min"]).strip()) > 0 \
                and "max" in node_data.keys() and node_data["max"] is not None \
                and len(str(node_data["max"]).strip()) > 0:
            buffer = buffer + "\n    bool hasRange() override;"

        if len(buffer) > 0:
            buffer = "// private codes\n    #if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)" + buffer
            buffer = buffer + "\n    #endif  // SSFS_INTERNAL"
        else:
            buffer = "// no private codes available"
        return buffer

    def get_protected_field_source(self, node_data: dict, parent_scope):
        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        buffer = ""
        # make bit
        if "description" in node_data.keys() and 'Bit' in node_data["description"]:
            pos = node_data["description"].find('Bit')
            bits_data = node_data["description"][pos + 3:]
            is_range = node_data["description"].find('~')
            try:
                if is_range == -1:
                    calculate_value = self.calculate_range(bits_data[0], bits_data[0])
                else:
                    calculate_value = self.calculate_range(bits_data[0], bits_data[2])
                buffer = buffer + "\nccos::HUInt64 " + current_scope + "calculateValue(ccos::HUInt64 &value) {"
                if bits_data[0] == "0":
                    buffer = buffer + "\n    ccos::HUInt64 rValue = value & %s;" % calculate_value
                else:
                    buffer = buffer + "\n    ccos::HUInt64 rValue = (value >> %s) & %s;" % (bits_data[0], calculate_value)
                buffer = buffer + "\n    return rValue;"
                buffer = buffer + "\n}"
            except:
                warning(self, "error while generating protected field source for %s\n%s" % \
                      (node_data["address"], bits_data))
                buffer = buffer + "\nccos::HUInt64 " + current_scope + "calculateValue(ccos::HUInt64 &value) {"
                buffer = buffer + "\n    return value;  // parse failed for calculation (check description field)"
                buffer = buffer + "\n}"

        if "mask" in node_data.keys():
            if node_data["mask"] is not None:
                buffer = buffer + "\nccos::HUInt64 " + current_scope + "getMask() {"
                buffer = buffer + "\n    return " + str(hex(node_data["mask"])) + ";"
                buffer = buffer + "\n}"

        if "shift" in node_data.keys():
            if node_data["shift"] is not None:
                buffer = buffer + "\nccos::HInt64 " + current_scope + "getShift() {"
                buffer = buffer + "\n    return " + str(node_data["shift"]) + ";"
                buffer = buffer + "\n}"

        if "factor" in node_data.keys():
            if node_data["factor"] is not None:
                if str(node_data["factor"]) != "1":
                    buffer = buffer + "\nccos::HDouble " + current_scope + "getFactor() {"
                    buffer = buffer + "\n    return " + str(node_data["factor"]) + ";"
                    buffer = buffer + "\n}"

        if "offset" in node_data.keys():
            if node_data["offset"] is not None:
                if str(node_data["offset"]) != "0":
                    buffer = buffer + "\nccos::HInt64 " + current_scope + "getOffset() {"
                    buffer = buffer + "\n    return " + str(node_data["offset"]) + ";"
                    buffer = buffer + "\n}"

        if "factor" in node_data.keys() and "offset" in node_data.keys():
            if node_data["factor"] is not None and node_data["offset"] is not None:
                if str(node_data["factor"]) != "1" or str(node_data["offset"]) != "0":
                    buffer = buffer + "\nbool " + current_scope + "usingCorrection() {"
                    buffer = buffer + "\n    return true;"
                    buffer = buffer + "\n}"

        if "mask" in node_data.keys() and "shift" in node_data.keys():
            if node_data["mask"] is not None and node_data["shift"] is not None:
                buffer = buffer + "\nbool " + current_scope + "usingMask() {"
                buffer = buffer + "\n    return true;"
                buffer = buffer + "\n}"

        # check min max value
        if "min" in node_data.keys() and node_data["min"] is not None and len(str(node_data["min"]).strip()) > 0 \
                and "max" in node_data.keys() and node_data["max"] is not None \
                and len(str(node_data["max"]).strip()) > 0:
            buffer = buffer + "\nbool " + current_scope + "hasRange() {"
            buffer = buffer + "\n    return true;"
            buffer = buffer + "\n}"

        if len(buffer) > 0:
            buffer = "// private codes\n#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)" + buffer
            buffer = buffer + "\n#endif  // SSFS_INTERNAL"
        else:
            buffer = "// no private codes available"

        return buffer

    # get private function
    def get_private_field_header(self, node_data: dict):
        buffer = ""
        if "type" in node_data.keys():
            if node_data["type"] != "branch" and node_data["type"] != "tagged_branch" and node_data["type"] != "tag":
                if "dataType" in node_data.keys():
                    buffer = buffer + "    // Data Type is %s\n" % node_data["dataType"]
                else:
                    warning(self, "  # %s (type : %s) is not branch but does not have dataType key" % (
                        node_data["address"], node_data["type"]))
            buffer = buffer + "    std::string mType = \"" + node_data["type"] + "\";"
        else:
            pass
        if "unit" in node_data.keys():
            buffer = buffer + "\n    std::string mUnit = \"" + node_data["unit"] + "\";"
        if "min" in node_data.keys():
            if is_number(node_data["min"]):
                buffer = buffer + "\n    ccos::HInt64 mMin = " + str(
                    node_data["min"]) + ";"
            else:
                buffer = buffer + "\n    // No numeric value"
                if "valueEnum" in node_data.keys():
                    for value_key in node_data["valueEnum"].keys():
                        if node_data["valueEnum"][value_key] == node_data["min"]:
                            buffer = buffer + "\n    ccos::HInt64 mMin = %s; // from %s" % (
                                str(value_key), node_data["min"])
                else:
                    buffer = buffer + "\n    // No min value in valueEnum"
        else:
            buffer = buffer + "\n    // No min value"
        if "max" in node_data.keys():
            if is_number(node_data["max"]):
                buffer = buffer + "\n    ccos::HInt64 mMax = " + str(
                    node_data["max"]) + ";"
            else:
                if "valueEnum" in node_data.keys():
                    for value_key in node_data["valueEnum"].keys():
                        if node_data["valueEnum"][value_key] == node_data["max"]:
                            buffer = buffer + "\n    ccos::HInt64 mMax = %s; // from %s" % (
                                str(value_key), node_data["max"])
                else:
                    buffer = buffer + "\n    // No max value in valueEnum"
        else:
            buffer = buffer + "\n    // No max value"
        if "description" in node_data.keys():
            description = node_data["description"]
            description = description.replace("\r", "")
            description = description.replace("<", "")
            description = description.replace("\n", "")
            description = description.replace("\"", "")
            description = description.strip()
            buffer = buffer + "\n    // description : " + description + " // NOLINT"
        return buffer

    def do_merge_inner_class_header(self, node_data: dict):
        for child_node_name in node_data["children"].keys():
            self.do_merge_inner_class_header(node_data["children"][child_node_name])

        if len(node_data["children"].keys()) == 0:
            # Leaf Node
            parent_node = node_data["parent"]
            if "inner_class_header" not in parent_node.keys():
                parent_node["inner_class_header"] = []
            header_buffer = node_data["header"]

            if "signalName" not in node_data.keys():
                warning(self, "%s is branch but there is no child node. Can not create %s node's class." % (
                    node_data["name"], node_data["name"]))
                self.merge_progress = self.merge_progress + 1
                self.progress.set_progress(self.merge_progress)
                return

            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_CLASS_NAME, DEFINE.NODE_PREFIX + node_data["name"])
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_ADDRESS, node_data["address"])

            if self.node_address.__contains__(node_data["address"]):
                self.node_address_duplicate.append(node_data["address"])
                # print("\t\t 0 conaint address:", node_data["address"], "\n")
            else:
                self.node_address.append(node_data["address"])
                # print("\t\t 1 cnt:", self.count, " address:", node_data["address"], "\n")
                self.count += 1

            if node_data["signalName"] is None:
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")
            else:
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, node_data["signalName"])
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_INNERCLASS_IMPL, "")
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_COMMENT_INNERCLASS,
                                                  self.get_comment_innerclass(node_data=node_data))
            if self.feature == "light":
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE,
                                                      self.get_type_helper_header(node_data=node_data))
            else:
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE,
                                                      self.get_public_field_header(node_data=node_data))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PROTECTED_VARIABLE,
                                                  self.get_protected_field_header(node_data=node_data))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PRIVATE_VARIABLE,
                                                  self.get_private_field_header(node_data=node_data))

            node_data["header"] = header_buffer
            parent_node["inner_class_header"].append(header_buffer)
        else:
            self.progress.erase_prev_progress_message()
            # print("Branch Node : node addr = %s" % (node_data["address"]))
            header_buffer = node_data["header"]

            header_member_variable_list = []
            header_inner_class_list = []
            header_include_list = []

            if node_data["type"] == "tagged_branch":
                _first_child_name = list(node_data["children"].keys())[0]
                _first_child = node_data["children"][_first_child_name]
                for child_node_name in _first_child["children"].keys():
                    child_node = _first_child["children"][child_node_name]
                    buffer = "    %s::%s %s;" % (DEFINE.NODE_PREFIX + _first_child_name,
                                                 DEFINE.NODE_PREFIX + child_node["name"],
                                                 child_node["name"])
                    # print("    ## child child member var : %s" % buffer)
                    header_member_variable_list.append(buffer)

            for child_node_name in node_data["children"].keys():
                # # print("  Child : %s" % child_node_name)
                child_node = node_data["children"][child_node_name]
                _child_child_node_list = list(child_node["children"].keys())

                if node_data["type"] != "tagged_branch":
                    # tagged branch 의 경우 자식의 첫번째 class 의 멤버들로 채운다.
                    buffer = "    %s %s;" % (DEFINE.NODE_PREFIX + child_node["name"], child_node["name"])
                    # print("    Member var : %s" % buffer)
                    header_member_variable_list.append(buffer)

                if child_node["vsm_file"] is not None:
                    # print("    child has header file path")
                    header_file_path = child_node["vsm_file"] + ".h"
                    header_file_name = os.path.basename(header_file_path)

                    buffer = "#include \"%s\"" % header_file_name
                    header_include_list.append(buffer)
                elif child_node["type"] == "tagged_branch":
                    # print("    child has header file path (tagged)")
                    for _child_child_node_name in _child_child_node_list:
                        # print("    child child node : %s" % _child_child_node_name)
                        _child_child_node = child_node["children"][_child_child_node_name]
                        if _child_child_node["vsm_file"] is not None:
                            _header_file_path = _child_child_node["vsm_file"] + ".h"
                            _header_file_name = os.path.basename(_header_file_path)
                            if not _child_child_node["vsm_file"].endswith("_SKEL"):
                                header_include_list.append("#if (defined(SSFS_INTERNAL) && defined(TAG_%s)) || defined(SSFS_VALIDATOR)" % _child_child_node["node_tag"])
                            _buffer = "#include \"%s\"" % _header_file_name
                            header_include_list.append(_buffer)
                            if not _child_child_node["vsm_file"].endswith("_SKEL"):
                                header_include_list.append("#endif")
                    buffer = child_node["header"]
                    header_inner_class_list.append(buffer)
                else:
                    buffer = child_node["header"]
                    header_inner_class_list.append(buffer)

            # public Class member
            header_member_variable = ""
            for buf in header_member_variable_list:
                header_member_variable = header_member_variable + "\n" + buf

            if self.feature == "light":
                header_member_variable = header_member_variable + self.get_type_helper_header(node_data=node_data)
            else:
                header_member_variable = header_member_variable + self.get_public_field_header(node_data=node_data)

            header_inner_class = ""
            # add inner_class tab
            for header in header_inner_class_list:
                header_lines = header.split("\n")
                for buf in header_lines:
                    if buf == "":
                        header_inner_class += "\n"
                        continue
                    header_inner_class += "\n    " + buf

            header_include = ""
            for header in header_include_list:
                header_lines = header.split("\n")
                for buf in header_lines:
                    header_include = header_include + "\n" + buf

            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_INNERCLASS_IMPL, header_inner_class)
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_COMMENT_INNERCLASS,
                                                  self.get_comment_innerclass(node_data=node_data))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE, header_member_variable)
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PROTECTED_VARIABLE,
                                                  self.get_protected_field_header(node_data=node_data))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_PRIVATE_VARIABLE,
                                                  self.get_private_field_header(node_data=node_data))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_INCLUDE, header_include)

            node_data["header"] = header_buffer

            if node_data["vsm_file"] is not None:
                self.source_storage[node_data["vsm_file"]]["header"] = header_buffer
            else:
                parent_node = node_data["parent"]
                if "inner_class_header" not in parent_node.keys():
                    parent_node["inner_class_header"] = []
                parent_node["inner_class_header"].append(header_buffer)

            # print("\t\t 2 address:", node_data["address"], "\n")
            self.node_address_title.append(node_data["address"])

        self.merge_progress = self.merge_progress + 1
        self.progress.set_progress(self.merge_progress)

    def do_merge_inner_class_source(self, node_data: dict, parent_scope):
        if node_data["vsm_file"] is not None:
            parent_scope = ""

        current_scope = parent_scope + DEFINE.NODE_PREFIX + node_data["name"] + "::"
        for child_node_name in node_data["children"].keys():
            self.do_merge_inner_class_source(node_data["children"][child_node_name], current_scope)

        if len(node_data["children"].keys()) == 0:
            # Leaf Node
            parent_node = node_data["parent"]
            if "inner_class_source" not in parent_node.keys():
                parent_node["inner_class_source"] = []
            source_buffer = node_data["source"]

            if "signalName" not in node_data.keys():
                warning(self, "%s is branch but there is no child node. Can not create %s node's class." % (
                    node_data["name"], node_data["name"]))
                self.merge_progress = self.merge_progress + 1
                self.progress.set_progress(self.merge_progress)
                return
            class_name = current_scope
            class_name += "\n        " + DEFINE.NODE_PREFIX + node_data["name"]
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_CLASS_NAME, class_name)
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_ADDRESS, node_data["address"])
            if node_data["signalName"] is None:
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")
            else:
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, node_data["signalName"])
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_CONSTRUCTOR_FUNC_IMPL, "")
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_INNERCLASS_IMPL, "")
            if self.feature == "light":
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE,
                                                      self.get_type_helper_source(node_data=node_data,
                                                                                  parent_scope=parent_scope))
            else:
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE,
                                                      self.get_public_field_source(node_data=node_data,
                                                                                   parent_scope=parent_scope))
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_PROTECTED_VARIABLE,
                                                  self.get_protected_field_source(node_data=node_data,
                                                                                  parent_scope=parent_scope))

            node_data["source"] = source_buffer
            parent_node["inner_class_source"].append(source_buffer)
        else:
            self.progress.erase_prev_progress_message()
            # print("Branch Node : node addr = %s" % (node_data["address"]))
            source_buffer = node_data["source"]

            class_name = current_scope
            class_name += "\n        " + DEFINE.NODE_PREFIX + node_data["name"]
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_CLASS_NAME, class_name)
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_ADDRESS, node_data["address"])
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")

            source_constructor_list = []
            source_inner_class_list = []

            for child_node_name in node_data["children"].keys():
                # print("  Child : %s" % child_node_name)
                child_node = node_data["children"][child_node_name]
                _child_child_node_list = list(child_node["children"].keys())
                if node_data["type"] == "multi":
                    buffer = "    mMultiTypes.insert(std::make_pair<std::string, Node*>(%s, &%s));" % (
                        child_node["name"], child_node["name"])
                    source_constructor_list.append(buffer)
                else:
                    if node_data["type"] == "tagged_branch":
                        is_normal_node = True
                        # if child_node["name"].endswith("_SKEL"):
                        #     is_normal_node = False
                        if is_normal_node:
                            source_constructor_list.append("    #if (defined(SSFS_INTERNAL) && defined(TAG_%s)) || defined(SSFS_VALIDATOR)" % child_node["node_tag"])
                        buffer = "    static %s%s %s;" % (
                            DEFINE.NODE_PREFIX,
                            child_node["name"],
                            child_node["name"])
                        source_constructor_list.append(buffer)
                        buffer = "    mMultiTypes.insert(std::make_pair<std::string, Node*>(\"%s\", &%s));" % (
                            child_node["node_tag"], child_node["name"])
                        source_constructor_list.append(buffer)
                        if is_normal_node:
                            source_constructor_list.append("    #endif")
                    else:
                        buffer = "    mChildNodes.push_back(&%s);" % child_node["name"]
                        source_constructor_list.append(buffer)

                if child_node["vsm_file"] is None:
                    buffer = child_node["source"]
                    source_inner_class_list.append(buffer)

            source_constructor = ""
            for buf in source_constructor_list:
                source_constructor = source_constructor + "\n" + buf

            # public Class member
            source_member_variable = ""
            if self.feature == "light":
                source_member_variable = source_member_variable + self.get_type_helper_source(node_data=node_data,
                                                                                              parent_scope=parent_scope)
            else:
                source_member_variable = source_member_variable + self.get_public_field_source(node_data=node_data,
                                                                                               parent_scope=parent_scope)

            source_inner_class = ""
            # add inner_class tab
            for source in source_inner_class_list:
                source_lines = source.split("\n")
                for buf in source_lines:
                    if buf == "":
                        source_inner_class += "\n"
                        continue
                    source_inner_class += "\n" + buf

            source_include = ""
            if node_data["vsm_file"] is not None:
                source_include = "#ifdef SSFS_VALIDATOR\n"
                source_include += "#include \"ValidatorSignalValue.hpp\"\n"
                source_include += "#endif\n"
                header_file_name = node_data["address"] + ".h"
                source_include += "#include \"%s\"" % header_file_name

            if self.feature == "light":
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_CONSTRUCTOR_FUNC_IMPL, "")
            else:
                source_buffer = source_buffer.replace(DEFINE.TEMPLATE_CONSTRUCTOR_FUNC_IMPL, source_constructor)
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_INNERCLASS_IMPL, source_inner_class)
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_PUBLIC_VARIABLE, source_member_variable)
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_PROTECTED_VARIABLE,
                                                  self.get_protected_field_source(node_data=node_data,
                                                                                  parent_scope=parent_scope))
            source_buffer = source_buffer.replace(DEFINE.TEMPLATE_INCLUDE, source_include)

            node_data["source"] = source_buffer

            if node_data["vsm_file"] is not None:
                self.source_storage[node_data["vsm_file"]]["src"] = source_buffer
            else:
                parent_node = node_data["parent"]
                if "inner_class_source" not in parent_node.keys():
                    parent_node["inner_class_source"] = []
                parent_node["inner_class_source"].append(source_buffer)

        self.merge_progress = self.merge_progress + 1
        self.progress.set_progress(self.merge_progress)

    def do_generate(self, target_dir, data, progress):
        self.progress.erase_prev_progress_message()

        node_name = data["node_prefix"]
        if "new_class_file" in data.keys():
            node_name = data["new_class_file"]

        if len(node_name) == 0:
            node_name = "Vehicle"
        vsm_file = node_name

        if not vsm_file in self.source_storage.keys():
            self.source_storage[vsm_file] = {}
            is_begin_of_file = True
            self.progress.erase_prev_progress_message()
            # print("  begin of file : %s" % data["address"])
            # print("    vsm_file = %s" % vsm_file)
        else:
            is_begin_of_file = False

        if is_begin_of_file:
            if node_name == data["name"]:
                header_buffer = self.top_template_header_root_class
            else:
                header_buffer = self.template_header_root_class

            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_INCLUDE_GUARD, DEFINE.NODE_PREFIX + data["name"]
                                                  + "_" + data["address"].replace(".", "_"))
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_NODENAME, data["name"])
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_CLASS_NAME, DEFINE.NODE_PREFIX + data["name"])
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_ADDRESS, data["address"])

            # delete tab
            header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_INNERCLASS_IMPL,
                                                  DEFINE.TEMPLATE_INNERCLASS_IMPL)
            header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_PUBLIC_VARIABLE,
                                                  DEFINE.TEMPLATE_PUBLIC_VARIABLE)
            header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_PRIVATE_VARIABLE,
                                                  DEFINE.TEMPLATE_PRIVATE_VARIABLE)
            if "signalName" in data.keys():
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, data["signalName"])
            else:
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")
            data["header"] = header_buffer
            data["source"] = self.template_source_root_class
            data["vsm_file"] = vsm_file
        else:
            header_buffer = self.do_generate_inner_class(node_data=data)
            data["header"] = header_buffer
            data["source"] = self.template_source_inner_class
            data["vsm_file"] = None

        progress = progress + 1
        self.progress.set_progress(progress)

        if data["children"] is not None and len(data["children"]) > 0:
            for node_name in data["children"].keys():
                child_node_data = data["children"][node_name]
                progress = self.do_generate(target_dir=target_dir, data=child_node_data, progress=progress)

        self.progress.set_progress(progress)
        time.sleep(0.000)
        return progress

    def do_generate_inner_class(self, node_data: dict) -> object:
        # print("Generating inner class : %s" % node_data["address"])
        header_buffer = self.template_header_inner_class
        header_buffer = header_buffer.replace(DEFINE.TEMPLATE_CLASS_NAME, DEFINE.NODE_PREFIX + node_data["name"])
        header_buffer = header_buffer.replace(DEFINE.TEMPLATE_ADDRESS, node_data["address"])
        # delete tab
        header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_PRIVATE_VARIABLE,
                                              DEFINE.TEMPLATE_PRIVATE_VARIABLE)
        header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_INNERCLASS_IMPL, DEFINE.TEMPLATE_INNERCLASS_IMPL)
        header_buffer = header_buffer.replace("    " + DEFINE.TEMPLATE_PUBLIC_VARIABLE, DEFINE.TEMPLATE_PUBLIC_VARIABLE)

        if "signalName" in node_data.keys():
            if node_data["signalName"] is None:
                # ignore this warning message
                # in cluster platform skel branch does not have signalName field.
                # if node_data["type"] == "tagged_branch" or node_data["type"] == "tag":
                if "node_tag" not in node_data or node_data["node_tag"] != "SKEL":
                    warning(self, "%s's signalName field is None." % node_data["address"])
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")
            else:
                header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, node_data["signalName"])
        else:
            header_buffer = header_buffer.replace(DEFINE.TEMPLATE_SIGNALNAME, "")
        node_data["header"] = header_buffer
        node_data["source"] = self.template_source_inner_class
        node_data["vsm_file"] = None
        return header_buffer

    def append_source(self, src, newstr):
        src[0] += newstr + "\n"

    def dump_src_buffer(self):
        for vsm_file in self.source_storage.keys():
            print("\nHeader FILE : %s.h" % vsm_file)
            buffer = self.source_storage[vsm_file]["header"]
            print(buffer)
            print("\nSource FILE : %s.cpp" % vsm_file)
            buffer = self.source_storage[vsm_file]["src"]
            print(buffer)

    def write_src_buffer(self, target_dir):
        file_path = target_dir + "/include/public/VSMNode.hpp"
        shutil.copy(os.path.join(sys.path[0], DEFINE.TEMPLATE_BASECLASS_FILE_PATH), file_path)

        for vsm_file in self.source_storage.keys():
            # write header
            vsm_header_file = open(target_dir + "/include/generated/" + vsm_file + ".h", "w")
            header_buffer = self.source_storage[vsm_file]["header"]
            header_buffer = header_buffer.rstrip()
            header_buffer = header_buffer + "\n"
            self.source_storage[vsm_file]["header"] = header_buffer
            vsm_header_file.write(header_buffer)
            vsm_header_file.close()
            # write cpp
            vsm_source_file = open(target_dir + "/src/generated/" + vsm_file + ".cpp", "w")
            source_buffer = self.source_storage[vsm_file]["src"]
            source_buffer = source_buffer.rstrip()
            source_buffer = source_buffer + "\n"
            self.source_storage[vsm_file]["src"] = source_buffer
            vsm_source_file.write(source_buffer)
            vsm_source_file.close()

    def calculate_range(self, start_bit, end_bit):
        bit_value = ""
        cal_value = (1 << ((int(end_bit, 10) + 1) - int(start_bit, 10))) - 1
        bit_value += "0x%X" % cal_value
        return bit_value

    def write_node_address(self, target_dir):
        _node_address_path = os.path.join(target_dir + "/include/generated/NodeAddressVSM.info")
        print("NodeAddressPath :", _node_address_path)
        with open(_node_address_path, 'w', encoding = 'UTF-8') as nodeFile:
            for address in self.node_address:
                nodeFile.write(address+'\n')

        _node_address_path1 = os.path.join(target_dir + "/include/generated/NodeAddressVSM_Title.info")
        with open(_node_address_path1, 'w', encoding = 'UTF-8') as nodeFile:
            for address in self.node_address_title:
                nodeFile.write(address+'\n')

        _node_address_path2 = os.path.join(target_dir + "/include/generated/NodeAddressVSM_Duplicate.info")
        with open(_node_address_path2, 'w', encoding = 'UTF-8') as nodeFile:
            for address in self.node_address_duplicate:
                nodeFile.write(address+'\n')
