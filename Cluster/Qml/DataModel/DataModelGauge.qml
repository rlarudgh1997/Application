// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelGauge"

    onModeChanged: print("Gauge      : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Gauge      : onCompleted   =", show, mode, display, z)

    // onGaugeSpeedChanged:                print("    gaugeSpeed                :", gaugeSpeed)
    // onGaugeRpmChanged:                  print("    gaugeRPM                  :", gaugeRPM)
    // onGaugeFuelChanged:                 print("    gaugeFuel                 :", gaugeFuel)
    // onGaugeTemperatureChanged:          print("    gaugeTemperature          :", gaugeTemperature)
    // onGaugeSpeedUnitChanged:            print("    gaugeSpeedUnit            :", gaugeSpeedUnit)
    // onGaugeSpeedStateChanged:           print("    gaugeSpeedState           :", gaugeSpeedState)
    // onGaugeSpeedSubDigitalStateChanged: print("    gaugeSpeedSubDigitalState :", gaugeSpeedSubDigitalState)
    // onGaugeRpmStateChanged:             print("    gaugeRpmState             :", gaugeRpmState)

    property int gaugeType: 0
    property int gaugeSpeed: 0
    property int gaugeRpm: 0
    property int gaugeFuel: 0
    property int gaugeTemperature: 0

    property real gaugeSpeedAngle: -120
    property real gaugeRpmAngle: -120
    property real gaugeFuelAngle: 0
    property real gaugeTemperatureAngle: 0

    property int gaugeSpeedUnit: 0
    property int gaugeSpeedState: 0
    property int gaugeSpeedSubDigitalState: 0
    property int gaugeRpmState: 0

}
