include($$PWD/DataManger/DataManger.pri)
include($$PWD/Util/Util.pri)


INCLUDEPATH += \
    $$PWD\


DEFINES += __MODULE_EXCEL__


HEADERS += \
    $$PWD/GuiExcel.h\
    $$PWD/HandlerExcel.h\
    $$PWD/ControlExcel.h\


SOURCES += \
    $$PWD/GuiExcel.cpp\
    $$PWD/HandlerExcel.cpp\
    $$PWD/ControlExcel.cpp\


FORMS += \
    $$PWD/GuiExcel.ui\
