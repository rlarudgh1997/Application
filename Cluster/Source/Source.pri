include($$PWD/Abstract/Abstract.pri)
include($$PWD/Common/Common.pri)
include($$PWD/Screen/Screen.pri)
include($$PWD/Config/Config.pri)
include($$PWD/Manager/Manager.pri)
include($$PWD/Home/Home.pri)
include($$PWD/Gauge/Gauge.pri)
include($$PWD/Telltale/Telltale.pri)
include($$PWD/Content/Content.pri)
include($$PWD/Event/Event.pri)
include($$PWD/Service/Service.pri)
include($$PWD/SubWindow/SubWindow.pri)



INCLUDEPATH+=\
    $$PWD\


HEADERS += \
    $$PWD/MainWindow.h\


SOURCES += \
    $$PWD/main.cpp\
    $$PWD/MainWindow.cpp\
