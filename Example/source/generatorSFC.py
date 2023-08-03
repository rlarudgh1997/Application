#!/usr/bin/env python3

# Copyright 2021 IVIS Inc.
#
# The right to copy, distribute, modify, or otherwise make use
# of this software shall be licensed only pursuant to the terms
# of an applicable IVIS license agreement.

import os
import shutil
import sys

from common.helper import parse_version_string
from common.message import ask, debug, error, verbose, warning


class Generator:
    def __init__(self, parser, output_directory_path, force_answer_yes=False):
        self.parser = parser
        self.output_directory_path = output_directory_path

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_template.hpp"), 'r')
        self.sfc_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_base_class_template.hpp"), 'r')
        self.sfc_base_class_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_arguments_template.hpp"), 'r')
        self.sfc_arguments_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_top_class_template.h"), 'r')
        self.sfc_helper_top_class_header_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_root_class_template.h"), 'r')
        self.sfc_helper_root_class_header_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_inner_class_template.h"), 'r')
        self.sfc_helper_inner_class_header_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_top_class_template.cppin"), 'r')
        self.sfc_helper_top_class_source_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_root_class_template.cppin"), 'r')
        self.sfc_helper_root_class_source_template = _sfc_template.read()
        _sfc_template.close()

        _sfc_template = open(os.path.join(sys.path[0], "template/sfc_helper_inner_class_template.cppin"), 'r')
        self.sfc_helper_inner_class_source_template = _sfc_template.read()
        _sfc_template.close()

        self.force_answer_yes = force_answer_yes

        self.helper_header_buffer = {}
        self.helper_source_buffer = {}

        self.node_address = list()

    def generate_snippet(self):
        verbose("  generating snippet codes")

        _raw_data = self.parser.raw_data
        _keys = list(_raw_data.keys())
        _sfc_name = _keys[0]

        major, minor, patch = parse_version_string(_raw_data["sfcVersion"])
        self.output_directory_path = os.path.join(self.output_directory_path+"/", self.parser.sfc_spec[_sfc_name], _sfc_name)

        _src_file_path = os.path.join(self.output_directory_path, _sfc_name + ".hpp")
        _src_base_class_file_path = os.path.join(self.output_directory_path+"/", "generated/", _sfc_name + "Base.hpp")
        verbose("    SFC name : %s\n    snippet path : %s" % (_sfc_name, _src_file_path))
        _sfc = _raw_data
        _src_buffer = self._generate_sfc(_sfc)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_FILENAME__", (_sfc_name + ".hpp"))
        _src_base_buffer = self._generate_sfc_base(_sfc)
        _src_base_buffer = _src_base_buffer.replace("__PLACEHOLDER_FILENAME__", (_sfc_name + "Base.hpp"))

        if not self._ensure_file(_src_base_class_file_path):
            return True

        if not os.path.isfile(_src_file_path):
            _src_file = open(_src_file_path, "w")
            _src_buffer = _src_buffer.rstrip()
            _src_file.write(_src_buffer)
            _src_file.close()

        _src_file = open(_src_base_class_file_path, "w")
        _src_buffer = _src_base_buffer.rstrip()
        _src_file.write(_src_base_buffer)
        _src_file.close()
        # debug("====================================================")
        # debug(_src_buffer)
        # debug("====================================================")
        return True

    def _generate_sfc_base(self, sfc):
        _src_buffer = self.sfc_base_class_template

        _keys = list(sfc.keys())
        _sfc_name = _keys[0]
        _sfc_version_ext = ""

        # make comment from description
        if "description" in _keys:
            if "sfcVersion" not in sfc:
                _sfc_version = "Not defined"
            else:
                _sfc_version = sfc["sfcVersion"]
                _sfc_description = "// SFC Version : %s\n" % _sfc_version
                _sfc_version_ext = "." + _sfc_version

            if "specVersion" not in sfc:
                error("spec version tag is not defined in %s" % _sfc_name)
                _spec_version = "Not defined"
            else:
                _spec_version = sfc["specVersion"]
                _sfc_description = _sfc_description + "// Spec Version : %s\n" % _spec_version
            _sfc_description = _sfc_description + "// " + sfc["description"]
            _src_buffer = _src_buffer.replace("__PLACEHOLDER_COMMENT__", _sfc_description)

        # make sfc log tag using sfc name and version extention
        _sfc_log_tag = "#ifndef LOG_TAG\n"
        _sfc_log_tag += "#define LOG_TAG \"SFC(%s%s)\"\n" % (_sfc_name, _sfc_version_ext)
        _sfc_log_tag += "#endif"

        # subscriptions
        if "subscriptions" not in _keys:
            error("subscription is not defineed SFC")
            sys.exit()
        if "outputs" not in _keys:
            error("outputs is not defineed SFC")
            sys.exit()

        _subscription_source = ""
        _sfc_inner_classes_source = ""
        _sfc_getter_functions_source = ""
        _generated_getter_function_address = list()
        for _subscription in sfc["subscriptions"]:
            _sfc_inner_classes_source += self._generate_arguments_class(_subscription)
            # _sfc_inner_classes_source = _sfc_inner_classes_source + "\n"
            _subscription_source += self._generate_subscription_callback(_subscription)
            _sfc_getter_functions_source += self._generate_getter_functions(_subscription,
                                                                            _generated_getter_function_address)
        _sfc_setter_functions_source = self._generate_setter_functions(sfc["outputs"])

        _sfc_helper_functions_source = ""

        __group_initialize_source = self._generate_group_initialize_source(sfc["outputs"])

        _sfc_inner_classes_source = self._add_tabs(_sfc_inner_classes_source)
        _sfc_setter_functions_source = self._add_tabs(_sfc_setter_functions_source)
        _sfc_getter_functions_source = self._add_tabs(_sfc_getter_functions_source)
        _subscription_source = self._add_tabs(_subscription_source)
        __group_initialize_source = self._add_tabs(__group_initialize_source)
        __group_initialize_source = self._add_tabs(__group_initialize_source)

        _sfc_keyword_source = self._generate_keyword_source(sfc["outputs"])
        _sfc_keyword_source = self._add_tabs(_sfc_keyword_source)

        # replace place holder
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_SFC_LOG_TAG__", _sfc_log_tag)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CLASSNAME__", _sfc_name)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_INNERCLASS__", _sfc_inner_classes_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_SETTER__", _sfc_setter_functions_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_GETTER__", _sfc_getter_functions_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_HELPER__", _sfc_helper_functions_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_SUBSCRIPTION_CALLBACK__", _subscription_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_GROUP_INITIALIZER__", __group_initialize_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_KEYWORD__", _sfc_keyword_source)

        _src_buffer = self._fallback_placeholder(_src_buffer)
        return _src_buffer

    # iterating each SFC item.
    def _generate_sfc(self, sfc):
        _src_buffer = self.sfc_template

        _keys = list(sfc.keys())
        _sfc_name = _keys[0]

        # make comment from description
        if "description" in _keys:
            if "sfcVersion" not in sfc:
                _sfc_version = "Not defined"
            else:
                _sfc_version = sfc["sfcVersion"]
                _sfc_description = "// SFC Version : %s\n" % _sfc_version

            if "specVersion" not in sfc:
                error("spec version tag is not defined in %s" % _sfc_name)
                _spec_version = "Not defined"
            else:
                _spec_version = sfc["specVersion"]
                _sfc_description = _sfc_description + "// Spec Version : %s\n" % _spec_version
            _sfc_description = _sfc_description + "// " + sfc["description"]
            _src_buffer = _src_buffer.replace("__PLACEHOLDER_COMMENT__", _sfc_description)

        # subscriptions
        if "subscriptions" not in _keys:
            error("subscription is not defineed SFC")
            sys.exit()
        if "outputs" not in _keys:
            error("outputs is not defineed SFC")
            sys.exit()

        _subscription_source = ""
        for _subscription in sfc["subscriptions"]:
            _subscription_source += self._generate_subscription_callback(_subscription)

        _subscription_source = self._add_tabs(_subscription_source)

        # replace place holder
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CLASSNAME__", _sfc_name)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_SUBSCRIPTION_CALLBACK__", _subscription_source)

        _src_buffer = self._fallback_placeholder(_src_buffer)
        return _src_buffer

    def _generate_getter_functions(self, subscription: dict, generated_address: list):
        _src_buffer = ""
        _keys = list(subscription.keys())
        _subscription_name = _keys[0]
        _inner_class_name = "Arguments" + _subscription_name[0].upper() + _subscription_name[1:]
        _values = subscription["values"]

        for _value in _values:
            _node_address = list(_value.keys())[0]
            _node_address = self._normalize(_node_address)
            _var_name = None
            _raw_data_type = None

            _var_name = _value[_node_address]
            if _var_name[0] == "m":
                _var_name = _var_name[1:]

            _var_name = _var_name[0].upper() + _var_name[1:]

            _source_name = "Unknown"
            if _node_address in self.parser.vsm_address_data_type:
                _raw_data_type = self.parser.vsm_address_data_type[_node_address]
                _source_name = "VSM"
            elif _node_address in self.parser.sfc_address_data:
                _raw_data_type = self.parser.sfc_address_data[_node_address]["dataType"]
                _raw_data_type = self.get_ccos_type_string(_raw_data_type)
                _source_name = "SFC"
            elif _node_address in self.parser.external_address_data:
                _raw_data_type = self.parser.external_address_data[_node_address]["dataType"]
                _raw_data_type = self.get_ccos_type_string(_raw_data_type)
                _source_name = "External SFC"
            if _raw_data_type is None:
                warning("%s is not exist in address map" % _node_address)
                continue
            if _node_address in generated_address:
                warning("%s is subscribed more than once." % _node_address)
                continue

            if _raw_data_type == "HString":
                _raw_data_type = "std::string"

            if _node_address in self.parser.vsm_address_data_type:
                if self.parser.vsm_address_has_value_enum[_node_address]:
                    _data_type = "%s::%s" % (_inner_class_name, _value[_node_address][1:])
                else:
                    _data_type = _raw_data_type
            elif _node_address in self.parser.sfc_address_data:
                if "valueEnum" in self.parser.sfc_address_data[_node_address]:
                    _data_type = "%s::%s" % (_inner_class_name, _value[_node_address][1:])
                else:
                    _data_type = _raw_data_type
            elif _node_address in self.parser.external_address_data:
                if "valueEnum" in self.parser.external_address_data[_node_address]:
                    _data_type = "%s::%s" % (_inner_class_name, _value[_node_address][1:])
                else:
                    _data_type = _raw_data_type

            generated_address.append(_node_address)
            _src_buffer += "HResult get%s(/* OUT */ %s& value) {\n" % (_var_name, _data_type)
            _src_buffer += "    return getCachedValue<%s>(\"%s\", reinterpret_cast<%s&>(value));\n" \
                           % (_raw_data_type, _node_address, _raw_data_type)
            _src_buffer += "}\n\n"
        return _src_buffer

    def _generate_subscription_callback(self, subscription: dict):
        _src_buffer = ""
        _keys = list(subscription.keys())
        _subscription_name = _keys[0]
        _inner_class_name = "Arguments" + _subscription_name[0].upper() + _subscription_name[1:]

        _function_name = "on" + _subscription_name[0].upper() + _subscription_name[1:]

        _src_buffer = _src_buffer + "void "
        _src_buffer = _src_buffer + _function_name
        _src_buffer = _src_buffer + "("
        _src_buffer = _src_buffer + "const " + _inner_class_name + "& args"
        _src_buffer = _src_buffer + ") {\n"
        _src_buffer = _src_buffer + "    // TODO : Write your code here.\n\n"
        _src_buffer = _src_buffer + "    // Important.\n"
        _src_buffer = _src_buffer + "    // - enqueue() is automatically called after this function.\n"
        _src_buffer = _src_buffer + "    //   The service collects queued signals from SFCs and publishes at the same time.\n"
        _src_buffer = _src_buffer + "    // - flush() sends signals immediately when called.\n"
        _src_buffer = _src_buffer + "    // Choose between flush and enqueue and use them appropriately.\n"
        _src_buffer = _src_buffer + "    // Please delete this comment before implement.\n"
        _src_buffer = _src_buffer + "}\n\n"
        return _src_buffer

    def _generate_arguments_class(self, subscription: dict):
        _src_buffer = self.sfc_arguments_template
        _keys = list(subscription.keys())
        _subscription_name = _keys[0]
        _inner_class_name = "Arguments" + _subscription_name[0].upper() + _subscription_name[1:]

        _values = subscription["values"]
        _variable_source = ""
        _constructor_arguments_source = ""
        _constructor_initializer_source = ""
        _constructor_body_source = ""
        _getter_function_source = ""
        _typedef_source = ""
        _argument_count = 0
        for _value in _values:
            _node_address = list(_value.keys())[0]
            _node_address = self._normalize(_node_address)
            _var_name = None
            _raw_data_type = None
            _init_value = None

            _var_name = _value[_node_address]
            _source_name = None
            if _node_address in self.parser.vsm_address_data_type:
                _raw_data_type = self.parser.vsm_address_data_type[_node_address]
                _source_name = "VSM"
            elif _node_address in self.parser.sfc_address_data:
                _raw_data_type = self.parser.sfc_address_data[_node_address]["dataType"]
                _raw_data_type = self.get_ccos_type_string(_raw_data_type)
                _source_name = "SFC"
            elif _node_address in self.parser.external_address_data:
                _raw_data_type = self.parser.external_address_data[_node_address]["dataType"]
                _raw_data_type = self.get_ccos_type_string(_raw_data_type)
                _source_name = "External SFC"

            if _raw_data_type == "HString":
                _raw_data_type = "std::string"

            if _raw_data_type == "std::string":
                _init_value = None
            else:
                _init_value = "0"

            if _source_name is None:
                warning("%s is not exist in address map" % _node_address)
                _source_name = "Unknown"
                continue

            if _node_address in self.parser.vsm_address_has_value_enum:
                if self.parser.vsm_address_has_value_enum[_node_address]:
                    _value_enum = self.parser.vsm_address_value_enum[_node_address]
                    _data_type = "%s" % _value[_node_address][1:]
                    # _typedef_source += "using %s = decltype(%s)::TYPE;  // NOLINT\n" % (_data_type, _node_address)
                    _unhandled_timeout_enum_defined = False
                    _typedef_source += "enum class %s : uint64_t {\n" % _data_type
                    for _value_enum_value in _value_enum.keys():
                        _typedef_source += "    " + _value_enum[_value_enum_value].upper()
                        _typedef_source += " = " + hex(_value_enum_value) + ",\n"
                        if "UNHANDLED_TIMEOUT" == _value_enum[_value_enum_value].upper():
                            _unhandled_timeout_enum_defined = True

                    # init to UNHANDLED_TIMEOUT when vsm address has value_eunm, which is for N/A node case
                    # : don't care "MESSAGE_TIMEOUT" in parser.vsm_address_value_enum.values()
                    # :        nor "TIMEOUT" in parser.vsm_address_matching_table.keys()
                    if not _unhandled_timeout_enum_defined:
                        _typedef_source += "    UNHANDLED_TIMEOUT = kTimeOut\n"
                    _typedef_source += "};\n"
                    _init_value = _data_type + "::UNHANDLED_TIMEOUT"
                else:
                    _data_type = _raw_data_type
            elif _node_address in self.parser.sfc_address_data:
                if "valueEnum" in self.parser.sfc_address_data[_node_address]:
                    _value_enum = self.parser.sfc_address_data[_node_address]["valueEnum"]
                    _data_type = "%s" % _value[_node_address][1:]
                    _first_key = None

                    _typedef_source += "enum class %s : uint64_t {\n" % _data_type
                    for _value_enum_value in _value_enum.keys():
                        _typedef_source += "    " + _value_enum_value.upper()
                        if _first_key is None:
                            _first_key = _value_enum_value.upper()
                        _typedef_source += " = " + hex(_value_enum[_value_enum_value]) + ",\n"
                    _typedef_source += "};\n"
                    _init_value = _data_type + "::" + _first_key
                else:
                    _data_type = _raw_data_type
            elif _node_address in self.parser.external_address_data:
                if "valueEnum" in self.parser.external_address_data[_node_address]:
                    _value_enum = self.parser.external_address_data[_node_address]["valueEnum"]
                    _data_type = "%s" % _value[_node_address][1:]
                    _first_key = None

                    _typedef_source += "enum class %s : uint64_t {\n" % _data_type
                    for _value_enum_value in _value_enum.keys():
                        _typedef_source += "    " + _value_enum_value.upper()
                        if _first_key is None:
                            _first_key = _value_enum_value.upper()
                        _typedef_source += " = " + hex(_value_enum[_value_enum_value]) + ",\n"
                    _typedef_source += "};\n"
                    _init_value = _data_type + "::" + _first_key
                else:
                    _data_type = _raw_data_type
            else:
                _data_type = _raw_data_type

            if _init_value is None:
                _variable_source += _data_type + " " + _var_name + "{};  // from " + _source_name + " - " + _node_address + "  // NOLINT\n"
            else:
                _variable_source += _data_type + " " + _var_name + " = " + _init_value + ";  // from " + _source_name + " - " + _node_address + "  // NOLINT\n"
            _variable_source += "const SignalValueRef " + _var_name + "_Signal;  // from " + _source_name + " - " + _node_address + "  // NOLINT\n"
            _constructor_arguments_source += ", const SFCPlugIn::SFCArgument& arg%s" % (str(_argument_count))
            _constructor_initializer_source += ",\n              %s_Signal" % _var_name
            _constructor_initializer_source += "(std::get<1>(arg%d))" % _argument_count

            _constructor_body_source += "if (std::get<0>(arg%d) != nullptr) {\n" % _argument_count
            if _raw_data_type == "HUInt64" or _raw_data_type == "ccos::HUInt64":
                _constructor_body_source += "    uint64_t value = 0UL;\n"
                _constructor_body_source += "    NodePtr node_ptr = std::get<0>(arg%d);\n" % _argument_count
                _constructor_body_source += "    node_ptr->translate(*%s_Signal, value);\n" % _var_name
            elif _raw_data_type == "HInt64" or _raw_data_type == "ccos::HInt64":
                _constructor_body_source += "    int64_t value = 0L;\n"
                _constructor_body_source += "    NodePtr node_ptr = std::get<0>(arg%d);\n" % _argument_count
                _constructor_body_source += "    node_ptr->translate(*%s_Signal, value);\n" % _var_name
            else:
                _constructor_body_source += "    %s value = %s_Signal->getValue<%s>();\n" % \
                                            (_raw_data_type, _var_name, _raw_data_type)
            _constructor_body_source += "    %s = static_cast<%s>(value);\n" % (_var_name, _data_type)
            _constructor_body_source += "}"
            if (_argument_count+1) < len(_values):
                _constructor_body_source += "\n"

            _argument_count += 1

        _generated_address = list()

        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CLASSNAME__", _inner_class_name)
        _variable_source = self._add_tabs(_variable_source)
        _variable_source = _variable_source.rstrip("\n")
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_VARIABLES__", _variable_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CONSTRUCTOR_ARGUMENTS__", _constructor_arguments_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CONSTRUCTOR_INITIALIZER__", _constructor_initializer_source)
        _constructor_body_source = self._add_tabs(_constructor_body_source)
        _constructor_body_source = self._add_tabs(_constructor_body_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_CONSTRUCTOR_BODY__", _constructor_body_source)
        _getter_function_source = self._add_tabs(_getter_function_source)
        _getter_function_source = _getter_function_source.rstrip("\n")
        _src_buffer = _src_buffer.replace("__PLACEHOLDER__GETTER__", _getter_function_source)
        _typedef_source = self._add_tabs(_typedef_source)
        _src_buffer = _src_buffer.replace("__PLACEHOLDER_TYPEDEFINE__", _typedef_source)
        return _src_buffer

    def _fallback_placeholder(self, src_buffer):
        # fallback
        src_buffer = src_buffer.replace("__PLACEHOLDER_COMMENT__", "// SFC Class")
        return src_buffer

    # add tabs
    def _add_tabs(self, source_buffer):
        _source_buffer = ""
        source_lines = source_buffer.split("\n")
        first = True
        for buf in source_lines:
            if buf == "":
                _source_buffer = _source_buffer + "\n"
                continue
            if not first:
                _source_buffer = _source_buffer + "    "
            _source_buffer = _source_buffer + buf
            _source_buffer = _source_buffer + "\n"
            first = False
        _source_buffer = _source_buffer.rstrip("\n")
        return _source_buffer

    def _ensure_file(self, file_path):
        if not self.force_answer_yes and os.path.isfile(file_path):
            if not ask("    %s is already exist. Overwrite(y/N)" % file_path, False):
                verbose("      skipping.")
                return False
        _src_file_directory = os.path.dirname(file_path)
        try:
            os.makedirs(_src_file_directory)
        except FileExistsError:
            pass
        return True

    def _generate_setter_functions(self, outputs):
        _source_buffer = ""
        generated_addresses = list()
        generated_functions = list()

        for _output in outputs:
            _keys = list(_output.keys())
            _output_address = _keys[0]
            _address_element = _output_address.split(".")

            _abstract_name = ""
            for component in _address_element:
                _abstract_name += component[0].upper() + component[1:]
            _function_name = "set" + _abstract_name
            _raw_variable_type = self.get_ccos_type_string(_output["dataType"])
            _variable_type = _raw_variable_type

            if "valueEnum" in _output:
                _value_enum = _output["valueEnum"]
                _source_buffer += "enum class %s : uint64_t {\n" % _abstract_name
                for _value_enum_name in _value_enum.keys():
                    _source_buffer += "    " + _value_enum_name.upper()
                    _source_buffer += " = " + hex(_value_enum[_value_enum_name]) + ",\n"
                _source_buffer += "};\n"
                _variable_type = "%s" % _abstract_name

            if _output_address in generated_addresses:
                error("output adddress %s is duplicated." % _output_address)
                continue
            generated_addresses.append(_output_address)

            suffix = 2
            while _function_name in generated_functions:
                _function_name = "set" + _abstract_name + str(suffix)
                suffix += 1

            generated_functions.append(_function_name)

            _source_buffer += "std::string " + _function_name + "(const " + _variable_type + "& arg) {\n"
            _return_value = ""
            if "group" in _output and len(_output["group"]) > 0:
                _source_buffer += "    // group procedure for %s\n" % _output["group"]
                _source_buffer += "    mTargetGroupSet.emplace(\"%s\");\n" % _output["group"]
                _source_buffer += "    if (updated(%s, reinterpret_cast<const %s&>(arg))) {\n" % (
                                self._normalize_sfc_output_address_to_keyword(_output_address), _raw_variable_type)
                _source_buffer += "        mUpdatedGroupSet.emplace(\"%s\");\n" % _output["group"]
                _source_buffer += "    }\n"
                _return_value = _output["group"]
            _source_buffer += "    updateValue(%s, static_cast<%s>(arg));\n" % (
                                self._normalize_sfc_output_address_to_keyword(_output_address), _raw_variable_type)
            _source_buffer += "    mLastChangedGroup = \"%s\";\n" % _return_value
            _source_buffer += "    return \"%s\";\n" % _return_value
            _source_buffer += "}\n\n"
        return _source_buffer

    def _generate_helper_class(self):
        # generate SFC helper header
        self._generate_helper_top_class_header(self.parser.sfc_address)
        for _header_name in self.helper_header_buffer.keys():
            _helper_class_header_path = os.path.join(self.output_directory_path + "/include/generated/", _header_name + ".h")
            if not self._ensure_file(_helper_class_header_path):
                return True
            _header_file = open(_helper_class_header_path, "w")
            _header_buffer = self.helper_header_buffer[_header_name]
            _header_buffer = _header_buffer.rstrip()
            _header_buffer = _header_buffer.replace("__PLACEHOLDER_FILENAME__", (_header_name + ".h"))
            _header_file.write(_header_buffer)
            _header_file.close()

        # generate SFC helper source
        self._generate_helper_top_class_source(self.parser.sfc_address)
        for _source_name in self.helper_source_buffer.keys():
            _helper_class_source_path = os.path.join(self.output_directory_path + "/src/generated/", _source_name + ".cpp")
            if not self._ensure_file(_helper_class_source_path):
                return True
            _source_file = open(_helper_class_source_path, "w")
            _source_buffer = self.helper_source_buffer[_source_name]
            _source_buffer = _source_buffer.rstrip()
            _source_buffer = _source_buffer.replace("__PLACEHOLDER_FILENAME__", (_source_name + ".h"))
            _source_file.write(_source_buffer)
            _source_file.close()

        return True

    def _generate_helper_top_class_header(self, node: dict):
        _header_buffer = self.sfc_helper_top_class_header_template
        _public_variable_buffer = ""
        _class_name = "SFC" + node["name"]
        _include_list = list()
        if "childs" in node:
            for _child_node in node["childs"]:
                _public_variable_buffer += "SFC" + node["childs"][_child_node]["name"]
                _public_variable_buffer += " " + node["childs"][_child_node]["name"] + ";\n"
                _include_list.append("\n#include \"SFC." + node["childs"][_child_node]["name"] + ".h\"")
                self._generate_helper_root_class_header(node["childs"][_child_node])
        _public_variable_buffer = self._add_tabs(_public_variable_buffer)

        _include_buffer = ""
        for _include_file in _include_list:
            _include_buffer += _include_file
        _header_buffer = _header_buffer.replace("##INCLUDE##", _include_buffer)
        _header_buffer = _header_buffer.replace("##NODENAME##", node["name"])
        _header_buffer = _header_buffer.replace("##CLASSNAME##", _class_name)
        _header_buffer = _header_buffer.replace("##ADDRESS##", node["address"])
        _header_buffer = _header_buffer.replace("##SIGNALNAME##", "")
        _header_buffer = _header_buffer.replace("##PUBLIC_VARIABLE##", _public_variable_buffer)
        _header_buffer = _header_buffer.replace("##COMMENT_INNERCLASS##", "")
        self.helper_header_buffer[node["name"]] = _header_buffer

    def _generate_helper_top_class_source(self, node: dict):
        _source_buffer = self.sfc_helper_top_class_source_template
        _class_name = "SFC" + node["name"]
        _constructor_buffer = "    "
        _include_list = list()
        if "childs" in node:
            for _child_node in node["childs"]:
                self._generate_helper_root_class_source(node["childs"][_child_node])
                _constructor_buffer += "mChildNodes.push_back(&" + node["childs"][_child_node]["name"] + ");\n"

        _include_buffer = "#ifdef SSFS_VALIDATOR\n"
        _include_buffer += "#include \"ValidatorSignalValue.hpp\"\n"
        _include_buffer += "#endif\n"
        _include_buffer += "#include \"" + node["name"] + ".h\""
        _source_buffer = _source_buffer.replace("##INCLUDE##", _include_buffer)
        _source_buffer = _source_buffer.replace("##CLASSNAME##", _class_name)
        _source_buffer = _source_buffer.replace("##ADDRESS##", node["address"])
        _source_buffer = _source_buffer.replace("##SIGNALNAME##", "")
        if _constructor_buffer != "":
            _constructor_buffer = "\n" + _constructor_buffer
            _constructor_buffer = self._add_tabs(_constructor_buffer)
        _source_buffer = _source_buffer.replace("##CONSTRUCTOR_FUNCTION_IMPLEMENTATION##", _constructor_buffer)
        _source_buffer = _source_buffer.replace("##COMMENT_INNERCLASS##", "")
        self.helper_source_buffer[node["name"]] = _source_buffer

    def _generate_helper_root_class_header(self, node: dict):
        _header_buffer = self.sfc_helper_root_class_header_template
        _childs_buffer = ""
        _public_variable_buffer = ""
        _class_name = "SFC" + node["name"]
        if "childs" in node:
            for _child_node in node["childs"]:
                _childs_buffer += self._generate_helper_inner_class_header(node["childs"][_child_node])
                _childs_buffer += "\n"
                _public_variable_buffer += "SFC" + node["childs"][_child_node]["name"]
                _public_variable_buffer += " " + node["childs"][_child_node]["name"] + ";\n"
        _childs_buffer = self._add_tabs(_childs_buffer)
        _public_variable_buffer = self._add_tabs(_public_variable_buffer)

        _header_buffer = _header_buffer.replace("##INCLUDE##", "")
        _header_buffer = _header_buffer.replace("##NODENAME##", node["name"])
        _header_buffer = _header_buffer.replace("##CLASSNAME##", _class_name)
        _header_buffer = _header_buffer.replace("##ADDRESS##", node["address"])
        _header_buffer = _header_buffer.replace("##SIGNALNAME##", "")
        _header_buffer = _header_buffer.replace("##INNERCLASS##", _childs_buffer)
        _header_buffer = _header_buffer.replace("##PUBLIC_VARIABLE##", _public_variable_buffer)
        _header_buffer = _header_buffer.replace("##COMMENT_INNERCLASS##", "")
        self.helper_header_buffer["SFC." + node["name"]] = _header_buffer


    def _generate_helper_root_class_source(self, node: dict):
        _source_buffer = self.sfc_helper_root_class_source_template
        _childs_buffer = ""
        _root_class_name = "SFC" + node["name"]
        _constructor_buffer = ""
        _current_scope = _root_class_name + "::"
        if "childs" in node:
            _constructor_buffer = "    "
            for _child_node in node["childs"]:
                _childs_buffer += self._generate_helper_inner_class_source(node["childs"][_child_node], _current_scope)
                _constructor_buffer += "mChildNodes.push_back(&" + node["childs"][_child_node]["name"] + ");\n"
        _source_buffer = _source_buffer.replace("##SIGNALNAME##", "")
        if _constructor_buffer != "":
            _constructor_buffer = "\n" + _constructor_buffer
            _constructor_buffer = self._add_tabs(_constructor_buffer)
        _include_buffer = "#ifdef SSFS_VALIDATOR\n"
        _include_buffer += "#include \"ValidatorSignalValue.hpp\"\n"
        _include_buffer += "#endif\n"
        _include_buffer += "#include \"" + "SFC." + node["name"] + ".h\""

        _source_buffer = _source_buffer.replace("##INCLUDE##", _include_buffer)
        _source_buffer = _source_buffer.replace("##NODENAME##", node["name"])
        _source_buffer = _source_buffer.replace("##CURRENTSCOPE##", _current_scope)
        _source_buffer = _source_buffer.replace("##CLASSNAME##", _root_class_name)
        _source_buffer = _source_buffer.replace("##ADDRESS##", node["address"])
        _source_buffer = _source_buffer.replace("##CONSTRUCTOR_FUNCTION_IMPLEMENTATION##", _constructor_buffer)
        _source_buffer = _source_buffer.replace("##INNERCLASS##", _childs_buffer)
        _source_buffer = _source_buffer.replace("##COMMENT_INNERCLASS##", "")
        self.helper_source_buffer["SFC." + node["name"]] = _source_buffer

    def _generate_helper_inner_class_header(self, node: dict):
        _source_buffer = self.sfc_helper_inner_class_header_template
        _childs_buffer = ""
        _public_variable_buffer = ""
        _class_name = "SFC" + node["name"]
        if "childs" in node:
            _public_variable_buffer = "\n    "
            for _child_node in node["childs"]:
                _childs_buffer += self._generate_helper_inner_class_header(node["childs"][_child_node])
                _childs_buffer += "\n"
                _public_variable_buffer += "SFC" + node["childs"][_child_node]["name"]
                _public_variable_buffer += " " + node["childs"][_child_node]["name"] + ";\n"
        if node["address"] in self.parser.sfc_address_data:
            _public_variable_buffer += self._generate_type_helper_header(self.parser.sfc_address_data[node["address"]])
        _childs_buffer += "\n"
        _childs_buffer = self._add_tabs(_childs_buffer)
        _public_variable_buffer = self._add_tabs(_public_variable_buffer)

        _source_buffer = _source_buffer.replace("##CLASSNAME##", _class_name)
        _source_buffer = _source_buffer.replace("##ADDRESS##", node["address"])
        _source_buffer = _source_buffer.replace("##SIGNALNAME##", "")
        if len(_childs_buffer) == 0:
            _childs_buffer = "// Leaf node - no child\n"
            self.node_address.append(node["address"])
        _source_buffer = _source_buffer.replace("##INNERCLASS##", _childs_buffer)
        _source_buffer = _source_buffer.replace("##PUBLIC_VARIABLE##", _public_variable_buffer)
        _source_buffer = _source_buffer.replace("##COMMENT_INNERCLASS##", "")

        return _source_buffer

    def _generate_helper_inner_class_source(self, node: dict, parent_scope: str):
        _source_buffer = self.sfc_helper_inner_class_source_template
        _childs_buffer = ""
        _public_variable_buffer = ""
        _constructor_buffer = ""
        _class_name = "SFC" + node["name"]
        _current_scope = parent_scope + _class_name + "::"
        if "childs" in node:
            _constructor_buffer = "    "
            for _child_node in node["childs"]:
                _childs_buffer += self._generate_helper_inner_class_source(node["childs"][_child_node], _current_scope)
                _constructor_buffer += "mChildNodes.push_back(&" + node["childs"][_child_node]["name"] + ");\n"
        if node["address"] in self.parser.sfc_address_data:
            _public_variable_buffer += self._generate_type_helper_source(self.parser.sfc_address_data[node["address"]], _current_scope)
        if _constructor_buffer != "":
            _constructor_buffer = self._add_tabs(_constructor_buffer)
            _constructor_buffer = "\n" + _constructor_buffer
        if _public_variable_buffer != "":
            _public_variable_buffer = "\n" + _public_variable_buffer

        _source_buffer = _source_buffer.replace("##CURRENTSCOPE##", _current_scope)
        _source_buffer = _source_buffer.replace("##CLASSNAME##", _class_name)
        _source_buffer = _source_buffer.replace("##ADDRESS##", node["address"])
        _source_buffer = _source_buffer.replace("##SIGNALNAME##", "")
        _source_buffer = _source_buffer.replace("##CONSTRUCTOR_FUNCTION_IMPLEMENTATION##", _constructor_buffer)
        _source_buffer = _source_buffer.replace("##INNERCLASS##", _childs_buffer)
        _source_buffer = _source_buffer.replace("##PUBLIC_VARIABLE##", _public_variable_buffer)
        _source_buffer = _source_buffer.replace("##COMMENT_INNERCLASS##", "")

        return _source_buffer

    def _generate_type_helper_header(self, address_data: dict):
        _type_buffer = ""
        _using_type =""

        if "valueEnum" in address_data:
            _type_buffer += "enum class Type : uint64_t {\n"
            for _enum in address_data["valueEnum"].keys():
                _type_buffer += "    " + _enum.upper() + " = " + hex(address_data["valueEnum"][_enum]) + ",\n"
            _type_buffer += "};\n\n"
            _using_type = "Type"
        else:
            if "dataType" in address_data:
                _using_type = self.get_ccos_type_string(address_data["dataType"])
            else:
                error("%s does not have data type filed" % address_data)
                return _type_buffer
        _type_buffer += "using TYPE = " + _using_type + ";\n"
        _type_buffer += "#ifndef SSFS_INTERNAL\n"
        _type_buffer += "TYPE value(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) const;\n"
        _type_buffer += "ccos::vehicle::vsm::HVehicleSignalValueType getVehicleSignalValueType() override;\n"
        _type_buffer += "#endif\n"

        # print(_type_buffer)
        return _type_buffer

    def _generate_type_helper_source(self, address_data: dict, current_scope: str):
        _type_buffer = ""
        _getter_type_list = {"HInt32":"HInt64", "HInt16":"HInt64", "HInt8":"HInt64", "HTriState":"HUInt64",
                             "HSwtichState":"HUInt64"}
        _getter_type = ""

        if "valueEnum" in address_data:
            _getter_type = self.get_ccos_type_string("HUInt64")
        else:
            if "dataType" in address_data:
                if address_data["dataType"] in _getter_type_list:
                    _getter_type = self.get_ccos_type_string(_getter_type_list[address_data["dataType"]])
                else:
                    _getter_type = self.get_ccos_type_string(address_data["dataType"])
            else:
                error("%s does not have data type filed" % address_data)
                return _type_buffer

        _type_buffer += "#ifndef SSFS_INTERNAL\n"
        _type_buffer += current_scope + "TYPE "
        _type_buffer += current_scope + "value(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) const {\n"
        _type_buffer += "    return static_cast<TYPE>(vehicleSignal.getValue<"
        _type_buffer += _getter_type + ">());\n}\n"
        _type_buffer += "\nccos::vehicle::vsm::HVehicleSignalValueType " + current_scope + "getVehicleSignalValueType() {\n"
        _type_buffer += "    return " + self.get_vehicle_signal_value_type(address_data["dataType"]) + ";\n"
        _type_buffer += "}\n"
        _type_buffer += "#endif\n"

        # print(_type_buffer)
        return _type_buffer

    def get_ccos_type_string(self, type_string: str):
        if type_string == "HTriState":
            return "ccos::vehicle::HTriState"
        elif type_string == "HSwitchState":
            return "ccos::vehicle::HSwitchState"
        elif type_string == "HUInt" or type_string == "HUInt8" or type_string == "HUInt16" or type_string == "HUInt32" or type_string == "HUInt64":
            return "ccos::HUInt64"
        elif type_string == "HInt" or type_string == "HInt8" or type_string == "HInt16" or type_string == "HInt32" or type_string == "HInt64":
            return "ccos::HInt64"
        elif type_string == "HFloat" or type_string == "HDouble":
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

    def get_vehicle_signal_value_type(self, type_string: str):
        if type_string == "HTriState":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::UINT64"
        elif type_string == "HSwitchState":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::UINT64"
        elif type_string == "HUInt" or type_string == "HUInt8" or type_string == "HUInt16" or type_string == "HUInt32" or type_string == "HUInt64":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::UINT64"
        elif type_string == "HInt" or type_string == "HInt8" or type_string == "HInt16" or type_string == "HInt32" or type_string == "HInt64":
            return "ccos::vehicle::vsm::HVehicleSignalValueType::INT64"
        elif type_string == "HFloat" or type_string == "HDouble":
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
            warning(self, "  # %s is not handled" % type_string)
            return "ccos::vehicle::vsm::HVehicleSignalValueType::MAX"

    @staticmethod
    def _normalize(buffer: str):
        buffer = buffer.replace("&", "_")
        return buffer

    def _generate_group_initialize_source(self, outputs):
        _source_buffer = "// Group Initialize codes\n"
        _group_map = dict()
        for _output in outputs:
            _keys = list(_output.keys())
            _node_address = _keys[0]

            if "group" not in _output:
                continue
            _group_name = _output["group"]
            if _group_name not in _group_map:
                _group_map[_group_name] = list()
            _group_map[_group_name].append(self._normalize_sfc_output_address_to_keyword(_node_address))

        for (_group_name, _address_list) in _group_map.items():
            _source_buffer += "mGroupToSignalNameMap.emplace(std::make_pair(std::string(\"%s\"), std::list<SignalName>({" % _group_name
            for _node_address in _address_list:
                _source_buffer += "SignalName(%s), " % _node_address

            _source_buffer = _source_buffer.rstrip()
            _source_buffer = _source_buffer.rstrip(",")
            _source_buffer += "})));\n"

        _source_buffer = _source_buffer.replace("[", "")
        _source_buffer = _source_buffer.replace("]", "")
        _source_buffer = _source_buffer.replace("\'", "\"")

        return _source_buffer

    def _normalize_sfc_output_address_to_keyword(self, _output_address):
        _address_element = _output_address.split(".")
        _keyword = "k"
        for component in _address_element:
            _keyword += component[0].upper() + component[1:]
        return _keyword

    def _generate_keyword_source(self, outputs):
        _source_buffer = ""
        for _output in outputs:
            _keys = list(_output.keys())
            _output_address = _keys[0]
            _source_buffer += "static constexpr const char* " + self._normalize_sfc_output_address_to_keyword(_output_address)
            _source_buffer += " = \"" + _output_address + "\";\n"
        return _source_buffer

    def _generate_write_node_address(self):
        _node_address_path = os.path.join(self.output_directory_path + "/include/generated/NodeAddressSFC.info")
        # print("NodeAddressPath :", _node_address_path)
        with open(_node_address_path, 'w', encoding = 'UTF-8') as nodeFile:
            for address in self.node_address:
                nodeFile.write(address+'\n')
