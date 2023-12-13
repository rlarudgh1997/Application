// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import GaugeEnum 1.0

import "../DataModel"


DataModelGauge {
    id: dataModelGauge

    // Component.onCompleted: {
    //     speedNiddle.visible = true
    //     rpmNiddle.visible = true
    // }
    // Item {
    //     id: speedMain
    //     GaugeSpeedBg { id: speedBg }
    //     Image {
    //         id: speedNiddle; x: 334; y: 74; visible: false
    //         transform: Rotation {
    //             origin.x: 26; origin.y: 286; angle: gaugeSpeedAngle
    //             Behavior on angle { NumberAnimation{duration: 300}}
    //         }
    //         source: "qrc:/Image/Gauge/NORMAL_IMG_NEEDLE_B.png"
    //     }
    // }

    // Item {
    //     id: rpmMain
    //     GaugeRpmBg { id: rpmBg }
    //     Image {
    //         id: rpmNiddle; x: 1534; y: 74; visible: false
    //         transform: Rotation {
    //             origin.x: 26; origin.y: 286; angle: gaugeRpmAngle
    //             Behavior on angle { NumberAnimation{duration: 300}}
    //         }
    //         source: "qrc:/Image/Gauge/NORMAL_IMG_NEEDLE_B.png"
    //     }
    // }

    GaugeSpeed { id: gaugeSpeedItem; x:8; y:2 }
    GaugeRpm { id: gaugeRpmItem; x: 1212; y:2 }


    Text {
        x: 700; y: 400; width: 200; height: 100; font.pixelSize: 50; color: "red"
        text: gaugeSpeed
    }
    Text {
        x: 1100; y: 400; width: 200; height: 100; font.pixelSize: 50; color: "red"
        text: gaugeRpm
    }

}
