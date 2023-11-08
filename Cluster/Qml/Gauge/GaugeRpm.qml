// import QtQuick
import QtQuick 2.12
import GaugeEnum 1.0

import "../DataModel"

Item {
    readonly property real defautlRpmAngle:    dataModelGauge.gaugeDefaultAngle - 3
    readonly property real angleMargin:        20.5

    readonly property real rpmNum0Focus:       defautlRpmAngle + (angleMargin * 1)
    readonly property real rpmNum1Focus:       defautlRpmAngle + (angleMargin * 3)
    readonly property real rpmNum2Focus:       defautlRpmAngle + (angleMargin * 5)
    readonly property real rpmNum3Focus:       defautlRpmAngle + (angleMargin * 7)
    readonly property real rpmNum4Focus:       defautlRpmAngle + (angleMargin * 9)
    readonly property real rpmNum5Focus:       defautlRpmAngle + (angleMargin * 11)
    readonly property real rpmNum6Focus:       defautlRpmAngle + (angleMargin * 13)

    property real rpmAngle: dataModelGauge.gaugeRpmAngle

    // Component.onCompleted: {
    //     rpmNiddleRotation.angle = rpmAngle
    // }
    onRpmAngleChanged: {
        if (rpmGaugeAnimation.running) {
            rpmGaugeAnimation.stop()
        }
        rpmGaugeAnimation.to = rpmAngle
        rpmGaugeAnimation.start()
    }

    PropertyAnimation {
        id: rpmGaugeAnimation
        target: rpmNiddleRotation
        running: false
        duration: 200
        property: "angle"
    }

    // Rpm Gauge : BG, Unit, Text
    Item {
        id: rpmBg
        visible: true
        Image {
            id: rpmMainBg; x: 54; y: 54;
            source: "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_image_10.png"
        }
        Image {
            id: rpmSubBg; x: 54; y: 54;
            source: "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_graduation_card_03.png"
        }
    }


    // Rpm Gauge : Digit Number
    Item {
        id: rpmNum
        visible: true
        Image {
            id: rpmnum0; x: 108; y: 435
            source: {
                if (rpmAngle <= rpmNum0Focus) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_0_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_0_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum1; x: 80; y: 288
            source: {
                if (rpmAngle > rpmNum0Focus && rpmAngle <= rpmNum1Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_1_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_1_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum2; x: 155; y: 154
            source: {
                if (rpmAngle > rpmNum1Focus && rpmAngle <= rpmNum2Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_2_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_2_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum3; x: 300; y: 103
            source: {
                if (rpmAngle > rpmNum2Focus && rpmAngle <= rpmNum3Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_3_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_3_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum4; x: 445; y: 154
            source: {
                if (rpmAngle > rpmNum3Focus && rpmAngle <= rpmNum4Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_4_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_4_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum5; x: 520; y: 288
            source: {
                if (rpmAngle > rpmNum4Focus && rpmAngle <= rpmNum5Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_5_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_5_card_01.png"
                }
            }
        }
        Image {
            id: rpmnum6; x: 492; y: 445
            source: {
                if (rpmAngle > rpmNum5Focus && rpmAngle <= rpmNum6Focus ) {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_6_card_02.png"
                } else {
                    "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_card_01_6_card_01.png"
                }
            }
        }
    }


    Image {
        id: rpmNiddlePointer; x: 338; y: 338;
        source: "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_image_09.png"
    }
    Image {
        id: rpmNiddle; x: 344; y: 170
        transform: Rotation {
            id: rpmNiddleRotation; origin.x: 6; origin.y: 180; angle: -123
        }
        source: "qrc:/Image/Gauge/RPM/T1_THEME_A_thm1_COMFORT/t1_thm1_tacho_image_01.png"
    }
}
