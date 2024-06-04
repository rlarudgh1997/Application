message("..................................................................................................")

TEMPLATE = app
CONFIG += c++17 link_pkgconfig
QT += qml quick widgets


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
    $$PWD/Qml/Qml.qrc\
    $$PWD/Resource/Image.qrc\

# Delete : File
QMAKE_CLEAN += \
    $$PWD/../TAV/*.info \
    $$PWD/../TAV/*.sh \
QMAKE_DISTCLEAN = $${QMAKE_CLEAN}

# Delete : Folder
# QMAKE_CLEAN_DIRS += \
#     $$PWD/../deploy_* \
# QMAKE_DISTCLEAN_DIRS += $${QMAKE_CLEAN_DIRS}


contains(DEFINES, TARGET_BUILD) {
    PKGCONFIG += hmiappcommon ssfs hvehicle
} else {
    PKGCONFIG += ssfs hvehicle
#    PKGCONFIG += hmiappcommon
}

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
    target.path = $$OUT_PWD
}


message("DEFINES=$$DEFINES")
message("target.path=$${target.path}")

!isEmpty(target.path): INSTALLS += target

message("INSTALLS=$$INSTALLS")
message("PWD=$$PWD")
