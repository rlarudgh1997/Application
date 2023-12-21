// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelEvent"

    onModeChanged: print("Event      : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Event      : onCompleted   =", show, mode, display, z)

    property int eventID: 0
}
