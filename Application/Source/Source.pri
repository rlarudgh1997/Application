include($$PWD/Abstract/Abstract.pri)
include($$PWD/Common/Common.pri)
include($$PWD/Screen/Screen.pri)
include($$PWD/Config/Config.pri)
include($$PWD/Manager/Manager.pri)
include($$PWD/Menu/Menu.pri)
include($$PWD/Excel/Excel.pri)
include($$PWD/Center/Center.pri)
include($$PWD/Dialog/Dialog.pri)

INCLUDEPATH+=\
    $$PWD\


HEADERS += \
    $$PWD/MainWindow.h\


SOURCES += \
    $$PWD/main.cpp\
    $$PWD/MainWindow.cpp\
