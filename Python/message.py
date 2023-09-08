# Copyright 2021 IVIS Inc.
#
# The right to copy, distribute, modify, or otherwise make use
# of this software shall be licensed only pursuant to the terms
# of an applicable IVIS license agreement.

DBG = True
from inspect import getframeinfo, stack
import os


def debug(message: str):
    if not DBG:
        return
    caller = getframeinfo(stack()[1][0])
    print(u"DEBUG : %s:%d %s" % (os.path.basename(caller.filename), caller.lineno, message))


def warning(message: str):
    print(u"\u001b[33m%s\u001b[0m" % message)


def error(message: str):
    print(u"\u001b[31m%s\u001b[0m" % message)


def verbose(message: str):
    print(u"%s" % message)


def ask(question, preferY = True):
    answer = input("%s (%s) : " % (question, "Y/n" if preferY else "y/N"))
    if not preferY:
        if answer == "Y" or answer == "y":
            return True
        else:
            return False
    else:
        if answer == "N" or answer == "n":
            return False
        else:
            return True
