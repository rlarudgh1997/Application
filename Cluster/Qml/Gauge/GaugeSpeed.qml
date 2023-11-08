// import QtQuick
import QtQuick 2.12
import GaugeEnum 1.0

import "../DataModel"

Item {
    readonly property real defautlSpeedAngle:    dataModelGauge.gaugeDefaultAngle - 3
    readonly property real angleMargin:          11.18
    readonly property real angleMarginMile:      15.375

    readonly property real speedNum0Focus:       defautlSpeedAngle + (angleMargin * 1)
    readonly property real speedNum20Focus:      defautlSpeedAngle + (angleMargin * 3)
    readonly property real speedNum40Focus:      defautlSpeedAngle + (angleMargin * 5)
    readonly property real speedNum60Focus:      defautlSpeedAngle + (angleMargin * 7)
    readonly property real speedNum80Focus:      defautlSpeedAngle + (angleMargin * 9)
    readonly property real speedNum100Focus:     defautlSpeedAngle + (angleMargin * 11)
    readonly property real speedNum120Focus:     defautlSpeedAngle + (angleMargin * 13)
    readonly property real speedNum140Focus:     defautlSpeedAngle + (angleMargin * 15)
    readonly property real speedNum170Focus:     defautlSpeedAngle + (angleMargin * 17)
    readonly property real speedNum200Focus:     defautlSpeedAngle + (angleMargin * 19)
    readonly property real speedNum230Focus:     defautlSpeedAngle + (angleMargin * 21)
    readonly property real speedNum260Focus:     defautlSpeedAngle + (angleMargin * 23)

    readonly property real speedNumMile0Focus:   defautlSpeedAngle + (angleMarginMile * 1)
    readonly property real speedNumMile20Focus:  defautlSpeedAngle + (angleMarginMile * 3)
    readonly property real speedNumMile40Focus:  defautlSpeedAngle + (angleMarginMile * 5)
    readonly property real speedNumMile60Focus:  defautlSpeedAngle + (angleMarginMile * 7)
    readonly property real speedNumMile80Focus:  defautlSpeedAngle + (angleMarginMile * 9)
    readonly property real speedNumMile100Focus: defautlSpeedAngle + (angleMarginMile * 11)
    readonly property real speedNumMile120Focus: defautlSpeedAngle + (angleMarginMile * 13)
    readonly property real speedNumMile140Focus: defautlSpeedAngle + (angleMarginMile * 15)
    readonly property real speedNumMile160Focus: defautlSpeedAngle + (angleMarginMile * 17)

    property real speedAngle: dataModelGauge.gaugeSpeedAngle

    // Component.onCompleted: {
    //     speedNiddleRotation.angle = speedAngle
    // }
    onSpeedAngleChanged: {
        if (speedGaugeAnimation.running) {
            speedGaugeAnimation.stop()
        }
        speedGaugeAnimation.to = speedAngle
        speedGaugeAnimation.start()
    }

    PropertyAnimation {
        id: speedGaugeAnimation
        target: speedNiddleRotation
        running: false
        duration: 200
        property: "angle"
    }

    // Speed Gauge : BG, Unit, Text
    Item {
        id: speedBg
        visible: ((dataModelGauge.gaugeSpeedUnit === SpeedUnitType.KM_PER_HOUR) || (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.MILE_PER_HOUR))
        Image {
            id: speedMainBg; x: 54; y: 54;
            source: "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_image_05.png"
        }
        Image {
            id: speedSubBg; x: 54; y: 54;
            source: "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_graduation_card_07.png"
        }
        Image {
            id: speedUnit; x: 314; y: 404;
            source: {
                if (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.KM_PER_HOUR) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_mainspd_unit_card_01.png"
                } else if (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.MILE_PER_HOUR) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_mainspd_unit_card_02.png"
                } else {
                    ""
                }
            }
        }
        Text {
            id: subSpeedDisital
            x: 53; y: 587; width: 160; height: 50; font.pixelSize: 20; color: "white"
            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            text: {
                if (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.KM_PER_HOUR) {
                    dataModelGauge.gaugeSpeed + " km/h"
                } else if (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.MILE_PER_HOUR) {
                    dataModelGauge.gaugeSpeed + " MPH"
                } else {
                    ""
                }
            }
        }
    }


    // Speed Gauge : Digit Number - KM
    Item {
        id: speedNumKm
        visible: (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.KM_PER_HOUR)
        Image {
            id: speednum0; x: 108; y: 435
            source: {
                if (speedAngle <= speedNum0Focus) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_0_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_0_card_01.png"
                }
            }
        }
        Image {
            id: speednum20; x: 82; y: 357
            source: {
                if (speedAngle > speedNum0Focus && speedAngle <= speedNum20Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_20_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_20_card_01.png"
                }
            }
        }
        Image {
            id: speednum40; x: 82; y: 279
            source: {
                if (speedAngle > speedNum20Focus && speedAngle <= speedNum40Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_40_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_40_card_01.png"
                }
            }
        }
        Image {
            id: speednum60; x: 111; y: 203
            source: {
                if (speedAngle > speedNum40Focus && speedAngle <= speedNum60Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_60_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_60_card_01.png"
                }
            }
        }
        Image {
            id: speednum80; x: 176; y: 140
            source: {
                if (speedAngle > speedNum60Focus && speedAngle <= speedNum80Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_80_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_80_card_01.png"
                }
            }
        }
        Image {
            id: speednum100; x: 254; y: 103
            source: {
                if (speedAngle > speedNum80Focus && speedAngle <= speedNum100Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_100_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_100_card_01.png"
                }
            }
        }
        Image {
            id: speednum120; x: 344; y: 103
            source: {
                if (speedAngle > speedNum100Focus && speedAngle <= speedNum120Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_120_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_120_card_01.png"
                }
            }
        }
        Image {
            id: speednum140; x: 424; y: 140
            source: {
                if (speedAngle > speedNum120Focus && speedAngle <= speedNum140Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_140_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_140_card_01.png"
                }
            }
        }
        Image {
            id: speednum170; x: 489; y: 203
            source: {
                if (speedAngle > speedNum140Focus && speedAngle <= speedNum170Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_170_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_170_card_01.png"
                }
            }
        }
        Image {
            id: speednum200; x: 518; y: 279
            source: {
                if (speedAngle > speedNum170Focus && speedAngle <= speedNum200Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_200_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_200_card_01.png"
                }
            }
        }
        Image {
            id: speednum230; x: 518; y: 357
            source: {
                if (speedAngle > speedNum200Focus && speedAngle <= speedNum230Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_230_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_230_card_01.png"
                }
            }
        }
        Image {
            id: speednum260; x: 492; y: 435
            source: {
                if (speedAngle > speedNum230Focus && speedAngle <= speedNum260Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_260_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_260_card_01.png"
                }
            }
        }
    }

    // Speed Gauge : Digit Number - MPH
    Item {
        id: speedNumMPH
        visible: (dataModelGauge.gaugeSpeedUnit === SpeedUnitType.SpeedUnitType)
        Image {
            id: speednum0MPH; x: 108; y: 435
            source: {
                if (speedAngle <= speedNumMile0Focus) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_0_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_0_card_01.png"
                }
            }
        }
        Image {
            id: speednum20MPH; x: 82; y: 326
            source: {
                if (speedAngle > speedNumMile0Focus && speedAngle <= speedNumMile20Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_20_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_20_card_01.png"
                }
            }
        }
        Image {
            id: speednum40MPH; x: 108; y: 213
            source: {
                if (speedAngle > speedNumMile20Focus && speedAngle <= speedNumMile40Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_40_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_40_card_01.png"
                }
            }
        }
        Image {
            id: speednum60MPH; x: 186; y: 133
            source: {
                if (speedAngle > speedNumMile40Focus && speedAngle <= speedNumMile60Focus ) {

                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_60_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_60_card_01.png"
                }
            }
        }
        Image {
            id: speednum80MPH; x: 300; y: 103
            source: {
                if (speedAngle > speedNumMile60Focus && speedAngle <= speedNumMile80Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_80_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_80_card_01.png"
                }
            }
        }
        Image {
            id: speednum100MPH; x: 414; y: 133
            source: {
                if (speedAngle > speedNumMile80Focus && speedAngle <= speedNumMile100Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_100_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_100_card_01.png"
                }
            }
        }
        Image {
            id: speednum120MPH; x: 492; y: 213
            source: {
                if (speedAngle > speedNumMile100Focus && speedAngle <= speedNumMile120Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_120_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_04_120_card_01.png"
                }
            }
        }
        Image {
            id: speednum140MPH; x: 518; y: 326
            source: {
                if (speedAngle > speedNumMile120Focus && speedAngle <= speedNumMile140Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_07_140_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_07_140_card_01.png"
                }
            }
        }
        Image {
            id: speednum160MPH; x: 492; y: 435
            source: {
                if (speedAngle > speedNumMile140Focus && speedAngle <= speedNumMile160Focus ) {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_07_160_card_02.png"
                } else {
                    "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_card_07_160_card_01.png"
                }
            }
        }

    }



    Image {
        id: speedNiddlePointer; x: 338; y: 338;
        source: "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_image_04.png"
    }
    Image {
        id: speedNiddle; x: 344; y: 170
        transform: Rotation {
            id: speedNiddleRotation; origin.x: 6; origin.y: 180; angle: -120
        }
        source: "qrc:/Image/Gauge/Speed/T1_THEME_A_thm1_COMFORT/t1_thm1_speedo_image_01.png"
    }
}
