// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import EventEnum 1.0

import "../DataModel"


DataModelEvent {
    id: dataModelEvent

    Text {
        id: popupText
        x: 1277; y: 66; width: 572; height: 572
        verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20;  color: "red"
        text: "Popup :" + eventType
    }

    onPopupInfoChanged: {
        // print("onPopupInfoChanged :", popupInfo)
        var text = ""
        for (var index = 0; index < popupInfo.length; index++) {
            text += popupInfo[index]
        }
        popupText.text = text
    }
}
