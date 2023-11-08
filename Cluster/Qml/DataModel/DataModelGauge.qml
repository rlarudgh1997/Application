// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelGauge"

    onModeChanged: print("Gauge      : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Gauge      : onCompleted   =", show, mode, display, z)

    property real gaugeDefaultAngle: 0

    property int gaugeType: 0
    property int gaugeSpeed: 0
    property int gaugeRpm: 0
    property int gaugeFuel: 0
    property int gaugeTemperature: 0

    property real gaugeSpeedAngle: 0
    property real gaugeRpmAngle: 0
    property real gaugeFuelAngle: 0
    property real gaugeTemperatureAngle: 0
    property int gaugeSpeedUnit: 0
}
