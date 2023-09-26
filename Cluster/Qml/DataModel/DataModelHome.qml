// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelHome"

    onModeChanged: print("Home       : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Home       : onCompleted   =", show, mode, display, z)

    property int homeType: 0
}
