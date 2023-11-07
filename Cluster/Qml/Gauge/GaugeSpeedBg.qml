// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
// import GaugeEnum 1.0

Item {
    Image { id: speedBG; x: 54; y: 54; source: "qrc:/Image/Gauge/NORMAL_IMG_GAUGE_BG.png" }
    Image { id: speedBgKPH; x: 54; y: 54; source: "qrc:/Image/Gauge/NORMAL_IMG_SPEED_BG_KPH.png" }

    Item {
        Image { id: speednum0; x: 116; y: 432; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_0_SEL.png" }
        Image { id: speednum20; x: 88; y: 350; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_20_SEL.png" }
        Image { id: speednum40; x: 91; y: 263; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_40_SEL.png" }
        Image { id: speednum60; x: 126; y: 184; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_60_SEL.png" }
        Image { id: speednum80; x: 184; y: 126; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_80_SEL.png" }
        Image { id: speednum100; x: 260; y: 90; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_100_SEL.png" }
        Image { id: speednum120; x: 353; y: 90; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_120_SEL.png" }
        Image { id: speednum140; x: 430; y: 126; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_140_SEL.png" }
        Image { id: speednum170; x: 488; y: 184; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_170_SEL.png" }
        Image { id: speednum200; x: 524; y: 263; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_200_SEL.png" }
        Image { id: speednum230; x: 527; y: 350; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_230_SEL.png" }
        Image { id: speednum260; x: 497; y: 432; scale: 0.7; source: "qrc:/Image/Gauge/GaugeNum/NORMAL_IMG_NUM_SPEED_260_SEL.png" }
    }

    Image { id: speedGaugePointer; x: 290; y: 290; source: "qrc:/Image/Gauge/NORMAL_IMG_GAUGE_POINTER.png" }
}
