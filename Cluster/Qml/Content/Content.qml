// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
//import TelltaleEnum 1.0
// import QtQuick.Controls 2.12

import "../DataModel"


DataModelContent {
    id: dataModelContent

    onViewFrontLeftVehicleChanged: {
        var text = ""
        for (var index = 0; index < viewFrontLeftVehicle.length; index++) {
            text += viewFrontLeftVehicle[index]
        }
        if (text !== "") {
            viewFrontLeftVehicleText.text = "[ViewFrontLeftVehicle]\n\n" + text
        }
    }
    onViewFrontRightVehicleChanged: {
        var text = ""
        for (var index = 0; index < viewFrontRightVehicle.length; index++) {
            text += viewFrontRightVehicle[index]
        }
        if (text !== "") {
            viewFrontRightVehicleText.text = "[ViewFrontRightVehicle]\n\n" + text
        }
    }

    Item {
        x: 600; y: 0; width: 720; height: 720

        Image {
            x: 235; y: 416
            source: "qrc:/Image/MainView/ADASMain_12H/adasmain_card_04_image_01.png"
        }
        Rectangle {
            visible: (viewFrontLeftVehicleText.text !== "")
            x: 100; y: 250; width: 200; height: 130
            color: "#00FFFFFF"; border.color: "gray"; border.width: 1
            Text {
                id: viewFrontLeftVehicleText
                anchors.fill: parent
                anchors.topMargin: 20
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 15;  color: "white"
            }
        }
        Rectangle {
            visible: (viewFrontRightVehicleText.text !== "")
            x: 420; y: 250; width: 200; height: 130
            color: "#00FFFFFF"; border.color: "gray"; border.width: 1
            Text {
                id: viewFrontRightVehicleText
                anchors.fill: parent
                anchors.topMargin: 20
                verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 15;  color: "white"
            }
        }
    }
}
