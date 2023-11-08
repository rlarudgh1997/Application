// import QtQuick
import QtQuick 2.12
// import CommonEnum 1.0
import TelltaleEnum 1.0
// import QtQuick.Controls 2.12

import "../DataModel"


DataModelTelltale {
    id: dataModelTelltale

    // Loader {
    //     source: {
    //         switch (dataModelTelltale.vehicleType) {
    //         case VehicleType.VehicleTypePV:
    //         "qrc:/TelltalePV.qml"
    //             break
    //         case VehicleType.VehicleTypeCV:
    //         "qrc:/TelltaleCV.qml"
    //             break
    //         default:
    //             ""
    //             break
    //         }
    //     }
    // }

    // DummyTelltale {
    //     id: keySystem
    //     xPosition: 0
    //     yPosition: 340
    //     telltaleName: "KeySystem"
    //     fontsize: 15
    //     visible: dataModelTelltale.telltaleKeySystemStat === TelltaleStat.ON
    // }

    Image {
        id: seatbelt
        x: 24; y: 500
        source: "qrc:/Image/Telltale/Telltale_Seatbelt.png"
        visible: (dataModelTelltale.telltaleSeatbeltStat === TelltaleStat.ON)
    }

    Image {
        id: xEVSystemReady
        x: 1844; y: 538
        source: "qrc:/Image/Telltale/Telltale_xEV_System_Ready.png"
        visible: (dataModelTelltale.telltalexEVSystemReadyStat === TelltaleStat.ON)
    }

    Image {
        id: e_Call
        x: 735; y: 0
        source: "qrc:/Image/Telltale/Telltale_E_Call.png"
        visible: (dataModelTelltale.telltaleEcallStat === TelltaleStat.ON)
    }

    Image {
        id: airBag
        x: 626; y: 0
        source: "qrc:/Image/Telltale/Telltale_AirBag.png"
        visible: (dataModelTelltale.telltaleAirBagStat === TelltaleStat.ON)
    }

    Image {
        id: lampFailWarning
        x: 787; y: 0
        source: "qrc:/Image/Telltale/Telltale_Lamp_Fail_Warning.png"
        visible: (dataModelTelltale.telltaleLampFailWarningStat === TelltaleStat.ON)
    }

    Image {
        id: hba_IFS
        x: 1050; y: 0
        source: {
            switch (dataModelTelltale.telltaleIFSStat) {
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_HBA_0.png"
                break
            case TelltaleStat.WHITE:
                "qrc:/Image/Telltale/Telltale_HBA_1.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewHba
        x: 1050; y: 0
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewHBAStat) {
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_HBA_0.png"
                break
            case TelltaleStat.WHITE:
                "qrc:/Image/Telltale/Telltale_HBA_1.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: elk
        x: 1052; y: 0
        source: {
            switch (dataModelTelltale.telltaleELKStat) {
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_ELK_0.png"
                break
            case TelltaleStat.RED:
                "qrc:/Image/Telltale/Telltale_ELK_1.png"
                break
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_ELK_2.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewElk
        x: 1052; y: 0
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewELKStat) {
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_ELK_0.png"
                break
            case TelltaleStat.RED:
                "qrc:/Image/Telltale/Telltale_ELK_1.png"
                break
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_ELK_2.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: fca
        x: 1110; y: 0
        source: {
            switch (dataModelTelltale.telltaleFCAStat) {
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_FCA_0.png"
                break
            case TelltaleStat.RED:
                "qrc:/Image/Telltale/Telltale_FCA_1.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewFca
        x: 1110; y: 0
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewFCAStat) {
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_FCA_0.png"
                break
            case TelltaleStat.RED:
                "qrc:/Image/Telltale/Telltale_FCA_1.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: lampIndicatorFrontFog
        anchors.left: lampFailWarning.right
        anchors.leftMargin: 1
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_FrontFog.png"
        visible: (dataModelTelltale.telltaleLampIndicatorFrontFogStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorRearFog
        anchors.left: lampIndicatorFrontFog.right
        anchors.leftMargin: 1
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_RearFog.png"
        visible: (dataModelTelltale.telltaleLampIndicatorRearFogStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorTailLamp
        anchors.left: lampIndicatorRearFog.right
        anchors.leftMargin: 1
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_TailLamp.png"
        visible: (dataModelTelltale.telltaleLampIndicatorTailLampStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorHighBeam
        anchors.left: lampIndicatorTailLamp.right
        anchors.leftMargin: 1
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_Highbeam.png"
        visible: (dataModelTelltale.telltaleLampIndicatorHighBeamStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorLowBeam
        anchors.left: lampIndicatorTailLamp.right
        anchors.leftMargin: 1
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_LowBeam.png"
        visible: (dataModelTelltale.telltaleLampIndicatorLowBeamStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorTurnSignalLeft
        x: 465; y: 0
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_TurnSignalLeft.png"
        visible: (dataModelTelltale.telltaleLampIndicatorTurnSignalLeftStat === TelltaleStat.ON)
    }

    Image {
        id: lampIndicatorTurnSignalRight
        x: 1403; y: 0
        source: "qrc:/Image/Telltale/Telltale_LampIndicator_TurnSignalRight.png"
        visible: (dataModelTelltale.telltaleLampIndicatorTurnSignalRightStat === TelltaleStat.ON)
    }

    Item {
        id: fcevWarning
        x: 64; y: 500
        Image {
            source: "qrc:/Image/Telltale/Telltale_H2_Leak.png"
            visible: (dataModelTelltale.telltaleH2LeakStat === TelltaleStat.ON)
        }
        Image {
            source: "qrc:/Image/Telltale/Telltale_H2-Sensor-Fault.png"
            visible: (dataModelTelltale.telltaleH2FaultStat === TelltaleStat.ON)
        }
    }

    // AFS_DBL
    Image {
        id: afsDBL
        x: 520; y: 0
        source: "qrc:/Image/Telltale/Telltale_AFS.png"
        visible: (TelltaleStat.telltaleAFSDBL === TelltaleStat.ON)
    }

    Image {
        id: ldwlka
        x: 1844; y: 420
        source: {
            switch (dataModelTelltale.telltaleLDWLKAStat) {
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_LDWLKA_0.png"
                break
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_LDWLKA_1.png"
                break
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_LDWLKA_2.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewLdwlka
        x: 1844; y: 420
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewLKAStat) {
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_LDWLKA_0.png"
                break
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_LDWLKA_1.png"
                break
            case TelltaleStat.YELLOW:
                "qrc:/Image/Telltale/Telltale_LDWLKA_2.png"
                break
            default:
                ""
                break
            }
        }
    }

    // Fuel_System
    Image{
        id: fuelSystem
        x: 210; y: 524
        source: {
            switch (dataModelTelltale.telltaleLowFuelWarn) {
            case TelltaleStat.ON:
                "qrc:/Image/Telltale/Telltale_LowFuel_Yellow.png"
                break
            case TelltaleStat.OFF:
                "qrc:/Image/Telltale/Telltale_LowFuel_White.png"
                break
            case TelltaleStat.OPTIONAL:
                if (dataModelTelltale.telltaleLowFuelWarnOptional === TelltaleLowFuelWarnStatOptional.ON_LPI) {
                    "qrc:/Image/Telltale/Telltale_LowFuel_Red.png"
                }
                else {
                    ""
                }
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewLCALeft
        x: 435; y: 630
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewLCALeftStat) {
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_LCALeft_Gray.png"
                break
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_LCALeft_Green.png"
                break
            case TelltaleStat.WHITE:
                "qrc:/Image/Telltale/Telltale_LCALeft_White.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewLCARight
        x: 435; y: 630
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewLCARightStat) {
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_LCARight_Gray.png"
                break
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_LCARight_Green.png"
                break
            case TelltaleStat.WHITE:
                "qrc:/Image/Telltale/Telltale_LCARight_White.png"
                break
            default:
                ""
                break
            }
        }
    }

    Image {
        id: adasDrivingNewLFA
        anchors.top: adasDrivingNewLCARight.top
        anchors.left: adasDrivingNewLCARight.right
        anchors.leftMargin: 1
        source: {
            switch (dataModelTelltale.telltaleADASDrivingNewLFAStat) {
            case TelltaleStat.GRAY:
                "qrc:/Image/Telltale/Telltale_LFA_2.png"
                break
            case TelltaleStat.GREEN:
                "qrc:/Image/Telltale/Telltale_LFA_1.png"
                break
            case TelltaleStat.WHITE:
                "qrc:/Image/Telltale/Telltale_LFA_0.png"
                break
            default:
                ""
                break
            }
        }
    }


    // TelltaleCV {
    //     anchors.fill: parent
    // }

    // TelltalePV {
    //     anchors.fill: parent
    // }
}
