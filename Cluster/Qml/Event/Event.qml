// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
import TelltaleEnum 1.0
// import QtQuick.Controls 2.12

import "../DataModel"


DataModelEvent {
    id: dataModelEvent

    Text {
        anchors.fill: parent
        visible: false
        text: "Event - 구현중"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        font.pointSize: 30
    }

}
