// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
import TelltaleEnum 1.0
// import QtQuick.Controls 2.12

import "../DataModel"


DataModelContent {
    id: dataModelContent

    onContentInfoChanged: {
        // print("onContentInfoChanged :", contentInfo)
        var text = ""
        for (var index = 0; index < contentInfo.length; index++) {
            text += contentInfo[index]
        }
        contentText.text = text
    }

    Text {
        id: contentText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20;  color: "red"
        text: ""
    }
}
