// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import HomeEnum 1.0

import "../DataModel"


DataModelHome {
    id: dataModelHome

    Image {
        anchors.fill: parent
        source: "qrc:/Image/BG_PA.png"
    }

    Text {
        anchors.fill: parent
        visible: false
        text: "Test Application - 구현중"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        font.pointSize: 30
    }


//    onHomeDisplayTypeChanged: swipeViewItem.currentIndex=homeDisplayType-CommonEnum.DisplayTypeMain

//    SwipeView {
//        id: swipeViewItem; anchors.fill: parent; currentIndex: 0

//        onCurrentIndexChanged: {
//            currentHomeIndex=CommonEnum.DisplayTypeMain+currentIndex
//            signalQmlClick(HomeEnum.PropertyHomeControlTopMain+currentIndex, TouchType.TouchTypeClick)
//        }

//        Item {
//            Loader{ anchors.fill: parent; source: "HomeMain.qml" }
//        }
//        Item {
//            Loader{ anchors.fill: parent; source: "HomeMenu.qml" }
//        }
//    }

}
