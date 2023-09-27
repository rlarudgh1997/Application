// import QtQuick
import QtQuick 2.12
import TelltaleEnum 1.0

DataModel {
    objectName: "dataModelTelltale"

    onModeChanged: print("Telltale   : onModeChanged =", show, mode, display, z)
    Component.onCompleted: print("Telltale   : onCompleted   =", show, mode, display, z)

    property int telltaleType: 0

    property int telltaleDualPowerStat: 0
    property int telltaleDualPowerOptional: 0
    property int telltaleLiftgateStat: 0
    property int telltaleBEQStat: 0
    property int telltaleTachographStat: 0
    property int telltaleHeatedMirrorStat: 0
    property int telltaleWorkingLampStat: 0
    property int telltaleASRStat: 0
    property int telltaleASRBlinkValueA: 0
    property int telltaleASRBlinkValueB: 0
    property int telltaleEBSAmberLampStat: 0
    property int telltaleEBSRedLampStat: 0
    property int telltaleLWSStat: 0
    property int telltaleLWSBlinkValueA: 0
    property int telltaleLWSBlinkValueB: 0
    property int telltaleECASStat: 0
    property int telltaleECASOptionalStat: 0
    property int telltaleECASBlinkValueA: 0
    property int telltaleECASBlinkValueAOptional: 0
    property int telltaleECASBlinkValueB: 0
    property int telltaleECASAxleMotionStat: 0
    property int telltaleECASAxleMotionStatOptional: 0
    property int telltaleBrakeAirLowLampStat: 0
    property int telltaleEHSAmberLampStat: 0
    property int telltaleEHSAmberLampBlinkValueA: 0
    property int telltaleEHSAmberLampBlinkValueB: 0
    property int telltaleEAPULampStat: 0
    property int telltaleAutoGreaseLowLampStat: 0
    property int telltaleCabTiltingStat: 0
    property int telltaleTMPTOStat: 0
    property int telltaleDifferencialLockStat: 0
    property int telltaleTMHighStat: 0
    property int telltaleWheelLockStat: 0
    property int telltaleRetarderLampStat: 0
    property int telltaleRetarderMalLampStat: 0
    property int telltaleHighVoltageBatteryChargeStat: 0
    property int telltaleTPMSCVStat: 0
    property int telltaleTPMSCVBlinkValueA: 0
    property int telltaleTPMSCVBlinkValueB: 0
    property int telltaleABSTrailerLampStat: 0
    property int telltaleNoABSTrailerLamp: 0
    property int telltaleABSCVStat: 0
    property int telltaleVDCStat: 0
    property int telltaleVDCBlinkValueA: 0
    property int telltaleVDCBlinkValueB: 0
    property int telltaleVDCOFFStat: 0
    property int telltaleStreeingWarningLampStat: 0
    property int telltaleParkingBrakeCVStat: 0
    property int telltaleSrvLmpEvStat: 0
    property int telltaleSrvLmpHevStat: 0
    property int telltaleSrvLmpFcevStat: 0
    property int telltaleEcallStat: 2
    property int telltaleSeatbeltStat: 2
    property int telltaleOATIceWarnStat: 0
    property int telltaleOATIceWarnStatOptional: 0
    property int telltaleAirBagStat: 2
    property int telltaleAirBagBlinkValueA: 0
    property int telltaleAirBagBlinkValueB: 0
    property int telltaleLampFailWarningStat: 2
    property int telltaleIFSStat: 194
    property int telltalexEVSystemReadyStat: 2
    property int telltaleKeySystemStat: 0
    property int telltaleLampIndicatorFrontFogStat: 0
    property int telltaleLampIndicatorHighBeamStat: 0
    property int telltaleLampIndicatorRearFogStat: 0
    property int telltaleLampIndicatorTailLampStat: 0
    property int telltaleLampIndicatorTurnSignalLeftStat: 0
    property int telltaleLampIndicatorTurnSignalRightStat: 0
    property int telltaleLampIndicatorLowBeamStat: 0
    property int telltaleH2FaultStat: 0
    property int telltaleH2LeakStat: 0
    property int telltaleAFSDBLStat: 0
    property int telltaleAFSDBLBlinkValueA: 0
    property int telltaleAFSDBLBlinkValueB: 0
    property int telltaleEvPwrDnStat: 0
    property int telltaleLDWLKAStat: 0
    property int telltaleLDWLKABlinkValueA: 0
    property int telltaleLDWLKABlinkValueB: 0
    property int telltaleFCAStat: 193
    property int telltaleFCABlinkValueA: 0
    property int telltaleFCABlinkValueB: 0
    property int telltaleELKStat: 193
    property int telltaleELKBlinkValueA: 0
    property int telltaleELKBlinkValueB: 0
    property int telltaleLowFuelWarnStat: 0
    property int telltaleLowFuelWarnStatOptional: 0
    property int telltaleLowFuelWarnBlinkValueA: 0
    property int telltaleLowFuelWarnBlinkValueB: 0
    property int telltaleADASDrivingNewELKStat: 196
    property int telltaleADASDrivingNewFCAStat: TelltaleStat.RED
    property int telltaleADASDrivingNewLKAStat: 0
    property int telltaleADASDrivingNewHBAStat: 199
    property int telltaleADASDrivingNewLCALeftStat: 0
    property int telltaleADASDrivingNewLCARightStat: 0
    property int telltaleADASDrivingNewLFAStat: 0
    property int telltalexEVServiceLampCVSrvLmpEvStat: 0
    property int telltalexEVServiceLampCVSrvLmpFcevStat: 0
    property int telltalexEVPowerLimitEvPwrDnStat: 0
    property int telltaleGFDWarningGFDStat: 0
}
