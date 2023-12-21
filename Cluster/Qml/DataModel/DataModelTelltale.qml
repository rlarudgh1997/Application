// import QtQuick
import QtQuick 2.12

DataModel {
    objectName: "dataModelTelltale"

    onModeChanged: print("Telltale   : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Telltale   : onCompleted   =", show, mode, display, z)

    property int telltaleKeySystemStat: 0
    property int telltaleSeatbeltStat: 0
    property int telltalexEVSystemReadyStat: 0
    property int telltaleEcallStat: 0
    property int telltaleAirBagStat: 0
    property int telltaleLampFailWarningStat: 0
    property int telltaleIFSStat: 0
    property int telltaleADASDrivingNewHBAStat: 0
    property int telltaleELKStat: 0
    property int telltaleADASDrivingNewELKStat: 0
    property int telltaleFCAStat: 0
    property int telltaleADASDrivingNewFCAStat: 0
    property int telltaleLampIndicatorFrontFogStat: 0
    property int telltaleLampIndicatorRearFogStat: 0
    property int telltaleLampIndicatorTailLampStat: 0
    property int telltaleLampIndicatorHighBeamStat: 0
    property int telltaleLampIndicatorLowBeamStat: 0
    property int telltaleLampIndicatorTurnSignalLeftStat: 0
    property int telltaleLampIndicatorTurnSignalRightStat: 0
    property int telltaleH2LeakStat: 0
    property int telltaleH2FaultStat: 0
    property int telltaleAFSDBL: 0
    property int telltaleLDWLKAStat: 0
    property int telltaleADASDrivingNewLKAStat: 0
    property int telltaleLowFuelWarn: 0
    property int telltaleLowFuelWarnOptional: 0
    property int telltaleADASDrivingNewLCALeftStat: 0
    property int telltaleADASDrivingNewLCARightStat: 0
    property int telltaleADASDrivingNewLFAStat: 0
    property int telltaleSrvLmpEvStat: 0
    property int telltaleSrvLmpHevStat: 0
    property int telltaleSrvLmpFcevStat: 0
    property int telltaleEvPwrDnStat: 0
}
