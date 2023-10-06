// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
import TelltaleEnum 1.0

import "../DataModel"

Item {
    // anchors.fill: parent
    Text {
        x: 0; y: 600; width: parent.width; height: 100
        text: "Telltale - PV"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        font.pointSize: 30
    }
}
