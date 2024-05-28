INCLUDEPATH += \
    $$PWD\


DEFINES += __MODULE_SUB_WINDOW__ #__SUB_WINDOW_ONLY__


HEADERS += \
    $$PWD/SubWindowEnum.h\
    $$PWD/SubWindow.h\
    $$PWD/LogWatcher.h\


SOURCES += \
    $$PWD/SubWindow.cpp\
    $$PWD/LogWatcher.cpp\


FORMS += \
    $$PWD/SubWindow.ui\
