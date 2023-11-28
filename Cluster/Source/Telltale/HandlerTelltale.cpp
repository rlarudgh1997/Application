#include "HandlerTelltale.h"
#include "CommonEnum.h"

#define QML QString("qrc:/Telltale/Telltale.qml")
#define QML_OBJECT QString("dataModelTelltale")  // DataModelTelltale.qml : ojbectName

QSharedPointer<HandlerTelltale>& HandlerTelltale::instance() {
    static QSharedPointer<HandlerTelltale> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerTelltale>(new HandlerTelltale());
    }
    return gHandler;
}

HandlerTelltale::HandlerTelltale() : AbstractHandler(ivis::common::DisplayEnum::DisplayTypeTelltale, QML, QML_OBJECT) {
}

void HandlerTelltale::initPropertyInfo() {
    // Common
    registerProperty(ivis::common::PropertyEnum::CommonDisplay, QString("display"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::CommonVisible, QString("visible"), QVariant(true));
    registerProperty(ivis::common::PropertyEnum::CommonRect, QString("rect"), QVariant());
    registerProperty(ivis::common::PropertyEnum::CommonVehicleType, QString("vehicleType"), QVariant(0));

    // Telltale
    registerProperty(ivis::common::PropertyEnum::TelltaleType, QString("telltaleType"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleDualPowerStat, QString("telltaleDualPowerStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleDualPowerOptional, QString("telltaleDualPowerOptional"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLiftgateStat, QString("telltaleLiftgateStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleBEQStat, QString("telltaleBEQStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTachographStat, QString("telltaleTachographStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleHeatedMirrorStat, QString("telltaleHeatedMirrorStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleWorkingLampStat, QString("telltaleWorkingLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleASRStat, QString("telltaleASRStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleASRBlinkValueA, QString("telltaleASRBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleASRBlinkValueB, QString("telltaleASRBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEBSAmberLampStat, QString("telltaleEBSAmberLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEBSRedLampStat, QString("telltaleEBSRedLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLWSStat, QString("telltaleLWSStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLWSBlinkValueA, QString("telltaleLWSBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLWSBlinkValueB, QString("telltaleLWSBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASStat, QString("telltaleECASStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASOptionalStat, QString("telltaleECASOptionalStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASBlinkValueA, QString("telltaleECASBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASBlinkValueAOptional, QString("telltaleECASBlinkValueAOptional"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASBlinkValueB, QString("telltaleECASBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASAxleMotionStat, QString("telltaleECASAxleMotionStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleECASAxleMotionStatOptional,
                     QString("telltaleECASAxleMotionStatOptional"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleBrakeAirLowLampStat, QString("telltaleBrakeAirLowLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEHSAmberLampStat, QString("telltaleEHSAmberLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEHSAmberLampBlinkValueA, QString("telltaleEHSAmberLampBlinkValueA"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEHSAmberLampBlinkValueB, QString("telltaleEHSAmberLampBlinkValueB"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEAPULampStat, QString("telltaleEAPULampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAutoGreaseLowLampStat, QString("telltaleAutoGreaseLowLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleCabTiltingStat, QString("telltaleCabTiltingStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTMPTOStat, QString("telltaleTMPTOStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleDifferencialLockStat, QString("telltaleDifferencialLockStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTMHighStat, QString("telltaleTMHighStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleWheelLockStat, QString("telltaleWheelLockStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleRetarderLampStat, QString("telltaleRetarderLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleRetarderMalLampStat, QString("telltaleRetarderMalLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleHighVoltageBatteryChargeStat,
                     QString("telltaleHighVoltageBatteryChargeStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTPMSCVStat, QString("telltaleTPMSCVStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTPMSCVBlinkValueA, QString("telltaleTPMSCVBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleTPMSCVBlinkValueB, QString("telltaleTPMSCVBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleABSTrailerLampStat, QString("telltaleABSTrailerLampStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleNoABSTrailerLamp, QString("telltaleNoABSTrailerLamp"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleABSCVStat, QString("telltaleABSCVStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleVDCStat, QString("telltaleVDCStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleVDCBlinkValueA, QString("telltaleVDCBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleVDCBlinkValueB, QString("telltaleVDCBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleVDCOFFStat, QString("telltaleVDCOFFStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleStreeingWarningLampStat, QString("telltaleStreeingWarningLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleParkingBrakeCVStat, QString("telltaleParkingBrakeCVStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpEvStat, QString("telltaleSrvLmpEvStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpHevStat, QString("telltaleSrvLmpHevStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpFcevStat, QString("telltaleSrvLmpFcevStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEcallStat, QString("telltaleEcallStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSeatbeltStat, QString("telltaleSeatbeltStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleOATIceWarnStat, QString("telltaleOATIceWarnStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleOATIceWarnStatOptional, QString("telltaleOATIceWarnStatOptional"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAirBagStat, QString("telltaleAirBagStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAirBagBlinkValueA, QString("telltaleAirBagBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAirBagBlinkValueB, QString("telltaleAirBagBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampFailWarningStat, QString("telltaleLampFailWarningStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleIFSStat, QString("telltaleIFSStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltalexEVSystemReadyStat, QString("telltalexEVSystemReadyStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleKeySystemStat, QString("telltaleKeySystemStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorFrontFogStat, QString("telltaleLampIndicatorFrontFogStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorHighBeamStat, QString("telltaleLampIndicatorHighBeamStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorRearFogStat, QString("telltaleLampIndicatorRearFogStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTailLampStat, QString("telltaleLampIndicatorTailLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalLeftStat,
                     QString("telltaleLampIndicatorTurnSignalLeftStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalRightStat,
                     QString("telltaleLampIndicatorTurnSignalRightStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorLowBeamStat, QString("telltaleLampIndicatorLowBeamStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleH2FaultStat, QString("telltaleH2FaultStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleH2LeakStat, QString("telltaleH2LeakStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAFSDBLStat, QString("telltaleAFSDBLStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAFSDBLBlinkValueA, QString("telltaleAFSDBLBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAFSDBLBlinkValueB, QString("telltaleAFSDBLBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEvPwrDnStat, QString("telltaleEvPwrDnStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLDWLKAStat, QString("telltaleLDWLKAStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLDWLKABlinkValueA, QString("telltaleLDWLKABlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLDWLKABlinkValueB, QString("telltaleLDWLKABlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleFCAStat, QString("telltaleFCAStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleFCABlinkValueA, QString("telltaleFCABlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleFCABlinkValueB, QString("telltaleFCABlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleELKStat, QString("telltaleELKStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleELKBlinkValueA, QString("telltaleELKBlinkValueA"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleELKBlinkValueB, QString("telltaleELKBlinkValueB"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarnStat, QString("telltaleLowFuelWarnStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarnStatOptional, QString("telltaleLowFuelWarnStatOptional"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarnBlinkValueA, QString("telltaleLowFuelWarnBlinkValueA"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarnBlinkValueB, QString("telltaleLowFuelWarnBlinkValueB"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewELKStat, QString("telltaleADASDrivingNewELKStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewFCAStat, QString("telltaleADASDrivingNewFCAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLKAStat, QString("telltaleADASDrivingNewLKAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewHBAStat, QString("telltaleADASDrivingNewHBAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCALeftStat, QString("telltaleADASDrivingNewLCALeftStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCARightStat,
                     QString("telltaleADASDrivingNewLCARightStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLFAStat, QString("telltaleADASDrivingNewLFAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltalexEVServiceLampCVSrvLmpEvStat,
                     QString("telltalexEVServiceLampCVSrvLmpEvStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltalexEVServiceLampCVSrvLmpFcevStat,
                     QString("telltalexEVServiceLampCVSrvLmpFcevStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltalexEVPowerLimitEvPwrDnStat, QString("telltalexEVPowerLimitEvPwrDnStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleGFDWarningGFDStat, QString("telltaleGFDWarningGFDStat"), QVariant(0));
}

void HandlerTelltale::controlConnect(const bool& state) {
    if (state) {
    } else {
        disconnect(this);
    }
}

void HandlerTelltale::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerTelltaleTimerStart)) {
        // do nothing
    }
}
