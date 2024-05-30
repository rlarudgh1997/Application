// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelContent"

    onModeChanged: print("Content    : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Content    : onCompleted   =", show, mode, display, z)

    property int contentType: 0
    property var contentInfo: ""
}
