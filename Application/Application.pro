message("..................................................................................................")

TEMPLATE = app

CONFIG += c++17

QT += \
    widgets\
#    core\
#    gui\
#    serialbus\
#    network\
#    xml\


# Define
equals(TARGET_BUILD, 1) {
    DEFINES += __BUILD_TYPE_TARGET__
} else {
    DEFINES -= __BUILD_TYPE_TARGET__
}


# Project
include(deployment.pri)
include($$PWD/Source/Source.pri)


# Resource
RESOURCES += \
#    $$PWD/Resource/Image.qrc\
#    $$PWD/Gui/Gui.qrc\
#    $$PWD/Resource/Font.qrc\
#    $$PWD/Resource/Language.qrc\




message("==================================")
message("==== Top-level qmake settings ====")
message("==================================")

# Environment variables
message("TARGET_BUILD=$$TARGET_BUILD")
message("PLATFORM=$$PLATFORM")
message("CONFIG=$$CONFIG")
message("QMAKE_CXXFLAGS=$$QMAKE_CXXFLAGS")
message("TARGET=$$TARGET")
message("DESTDIR=$$DESTDIR")
message("INSTALL_PATH=$$INSTALL_PATH")

!isEmpty(INSTALL_PATH) {
    target.path = $$INSTALL_PATH
} else {
	target.path = $$PWD/Application
}

message("DEFINES=$$DEFINES")
message("target.path=$${target.path}")

!isEmpty(target.path): INSTALLS += target
message("INSTALLS=$$INSTALLS")


message("PWD=$$PWD")
