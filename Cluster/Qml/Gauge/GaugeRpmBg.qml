// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
// import GaugeEnum 1.0

Item {
    Image { id: rpmBG; x: 1254; y: 54; source: "qrc:/Image/Gauge/NORMAL_IMG_GAUGE_BG.png" }
    Image { id: rpmBgDSL; x: 1254; y: 54; source: "qrc:/Image/Gauge/NORMAL_IMG_RPM_BG_DSL.png" }
    Image { id: rpmPointer; x: 1494; y: 290; source: "qrc:/Image/Gauge/NORMAL_IMG_GAUGE_POINTER.png" }

    Item {
        Image { id: rpmnum0; x: 1317; y: 435; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_0_SEL.png" }
        Image { id: rpmnum1; x: 1283; y: 275; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_1_SEL.png" }
        Image { id: rpmnum2; x: 1362; y: 146; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_2_SEL.png" }
        Image { id: rpmnum3; x: 1506; y: 94; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_3_SEL.png" }
        Image { id: rpmnum4; x: 1651; y: 146; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_4_SEL.png" }
        Image { id: rpmnum5; x: 1730; y: 276; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_5_SEL_RED.png" }
        Image { id: rpmnum6; x: 1694; y: 437; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_RPM_6_SEL_RED.png" }
    }
}
