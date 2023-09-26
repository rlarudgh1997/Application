// import QtQuick
import QtQuick 2.12
import CommonEnum 1.0
import HandlerHomeEnum 1.0

Item {
    // objectName: "dataModel";
    x: 0; y: 0; width: Display.Width; height: Display.Height

    Component.onCompleted: {
        print(this);
        print("[QML] ScreenInfo(", objectName, "):", this.x, this.y, this.width, this.height, this.z)
    }
//    onZChanged: print("onZChanged - ScreenInfo(", objectName, "):", this.x, this.y, this.width, this.height, this.z)

    onDisplayChanged: {
        ScreenInfoInstance.loadComplete(display)
//        displayZ = this.z
    }
    onModeChanged: {
        show = (display === mode)
    }
    onThemeChanged: {
        fontColorDark = (theme === "qrc:/Image/ThemeDark")
        fontColorNormal = (theme === "qrc:/Image/ThemeNormal")
    }



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // ScreenInfo::controlDisplay(mode, true)
//    property int displayZ: 0
//    onZChanged: {
//        if ((displayZ !== 0) && (z !== displayZ)) {
//            show = true
//        }
//    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    signal signalQmlClick(int propertyType, int touchType)
    // onSignalQmlClick: print("signalQmlClick=", propertyType, touchType)
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    property int display: Display.DisplayTypeInvalid
    property int vehicleType: 0
    property var rect: {0, 0, Display.Width, Display.Height}
    property int mode: Display.DisplayTypeInvalid
    property string theme: "qrc:/Image/ThemeNormal"     //Normal : ThemeNormal,    Dark : ThemeDark
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    property bool show: false
    property bool fontColorDark: false
    property bool fontColorNormal: false
//    readonly property string fontName: fontPayboocMedium.name
//    readonly property string fontBoldName: fontPayboocBold.name
//    readonly property string fontExtraBoldName: fontPayboocExtraBold.name
    // onFontColorDarkChanged: print("onFontColorDarkChanged=", fontColorDark)
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//    // Font
//    FontLoader{id:fontPayboocMedium; source:"qrc:/Font/payboocMedium.ttf"}
//    FontLoader{id:fontPayboocBold; source:"qrc:/Font/payboocBold.ttf"}
//    FontLoader{id:fontPayboocExtraBold; source:"qrc:/Font/payboocExtraBold.ttf"}

    MouseArea{anchors.fill: parent; onPressed: print("[", parent.objectName, "] Press : ", display, mouseX, mouseY, )}
}
