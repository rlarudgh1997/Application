include($$PWD/Abstract/Abstract.pri)
include($$PWD/Common/Common.pri)
include($$PWD/Screen/Screen.pri)
include($$PWD/Config/Config.pri)
include($$PWD/Manager/Manager.pri)
include($$PWD/Home/Home.pri)
include($$PWD/Telltale/Telltale.pri)
#include($$PWD/Service/Service.pri)



INCLUDEPATH+=\
    $$PWD\


HEADERS += \
    $$PWD/MainWindow.h\


SOURCES += \
    $$PWD/main.cpp\
    $$PWD/MainWindow.cpp\
