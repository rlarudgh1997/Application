INCLUDEPATH += \
    $$PWD\


DEFINES += __MODULE_SERVICE__


PKGCONFIG += ssfs hvehicle

contains(DEFINES, TARGET_BUILD) {
    PKGCONFIG += hmiappcommon
}


HEADERS += \
    $$PWD/Service.h\
    $$PWD/ServiceEnum.h\


SOURCES += \
    $$PWD/Service.cpp\
