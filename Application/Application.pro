message("..................................................................................................")

TEMPLATE = app

CONFIG += c++17

QT += \
    widgets\
    qml\
#    xml webenginewidgets\
#    core\
#    gui\
#    serialbus\
#    network\



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
    $$PWD/Resource/Image.qrc\
#    $$PWD/Resource/Font.qrc\
#    $$PWD/Resource/Language.qrc\


# FORMS += \
#    test.ui

# Delete : File
# QMAKE_CLEAN += \
#      $$PWD/../TAV/*.info \
# QMAKE_DISTCLEAN = $${QMAKE_CLEAN}

# Delete : Folder
# QMAKE_CLEAN_DIRS += \
#     $$PWD/../deploy_* \
# QMAKE_DISTCLEAN_DIRS += $${QMAKE_CLEAN_DIRS}

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
    python.path = $$INSTALL_PATH
    python.files = $$PWD/../Python/ExcelParser.py $$PWD/../Python/CaseGen/
    node.path = $$INSTALL_PATH
    node.files = $$PWD/../Python/NodeAddress
} else {
    target.path = $$OUT_PWD
    python.path = $$OUT_PWD
    python.files = $$PWD/../Python/ExcelParser.py $$PWD/../Python/CaseGen/
    node.path = $$OUT_PWD
    node.files = $$PWD/../Python/NodeAddress
}


message("DEFINES=$$DEFINES")
message("target.path=$${target.path}")
message("python.path=$${python.path}")
message("python.files=$${python.files}")
message("node.path=$${node.path}")
message("node.files=$${node.files}")

!isEmpty(target.path): INSTALLS += target
!isEmpty(python.path): INSTALLS += python
!isEmpty(node.path): INSTALLS += node
!isEmpty(module.path): INSTALLS += module

message("INSTALLS=$$INSTALLS")
message("PWD=$$PWD")
