// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import EventEnum 1.0

import "../DataModel"


DataModelEvent {
    id: dataModelEvent
    visible: (dataModelEvent.eventType > 0)

    onPopupInfoChanged: {
        // print("onPopupInfoChanged :", popupInfo)
        var text = ""
        for (var index = 0; index < popupInfo.length; index++) {
            text += popupInfo[index]
        }
        popupText.text = text
    }

    Item {
        x: 1212; y: 2; width: 700; height: 700
        Image {
            visible: ((dataModelEvent.popupColorType === PopupColor.WHITE) ||
                       (dataModelEvent.popupColorType === PopupColor.YELLOW) ||
                       (dataModelEvent.popupColorType === PopupColor.RED))
            source: "qrc:/Image/Popup/FullPopupBG_12H/thm_fpbg_card_01.PNG"

        }
        Image {
            visible: dataModelEvent.popupColorStatus
            source: {
                if (dataModelEvent.popupColorType === PopupColor.WHITE) {
                    "qrc:/Image/Popup/FullPopupBorder_12H/thm_fpborder_card_01.PNG"
                } else if (dataModelEvent.popupColorType === PopupColor.YELLOW) {
                    "qrc:/Image/Popup/FullPopupBorder_12H/thm_fpborder_card_02.PNG"
                } else if (dataModelEvent.popupColorType === PopupColor.RED) {
                    "qrc:/Image/Popup/FullPopupBorder_12H/thm_fpborder_card_03.PNG"
                } else {
                    ""
                }
            }
        }
        Text {
            id: popupText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20;  color: "red"
            text: ""
        }
        // Text {
        //     id: popupText
        //     x: 100; y: 100; width: 480; height: 236
        //     verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
        //     font.pixelSize: 35; color: "white"
        //     text: dataModelEvent.popupInfo
        // }
    }
}
