// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import GaugeEnum 1.0

import "../DataModel"


DataModelGauge {
    id: dataModelGauge

    Component.onCompleted: {
        speedNiddle.visible = true
        rpmNiddle.visible = true
    }


    Item {
        id: speedMain

        GaugeSpeedBg {
            id: speedBg
        }
        Image {
            id: speedNiddle
            x: 334; y: 74; visible: false
            transform: Rotation {
                origin.x: 26; origin.y: 286; angle: gaugeSpeedAngle
                Behavior on angle { NumberAnimation{duration: 300}}
            }
            source: "qrc:/Image/Gauge/NORMAL_IMG_NEEDLE_B.png"
        }
    }

    Item {
        id: rpmMain

        GaugeRpmBg {
            id: rpmBg
        }
        Image {
            id: rpmNiddle
            x: 1534; y: 74; visible: false
            transform: Rotation {
                origin.x: 26; origin.y: 286; angle: gaugeRpmAngle
                Behavior on angle { NumberAnimation{duration: 300}}
            }
            source: "qrc:/Image/Gauge/NORMAL_IMG_NEEDLE_B.png"
        }
    }

    Text {
        anchors.fill: parent
        text: "Gauge - 구현중"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        font.pointSize: 30
    }
}
