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
    registerProperty(ivis::common::PropertyEnum::TelltaleKeySystemStat, QString("telltaleKeySystemStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSeatbeltStat, QString("telltaleSeatbeltStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltalexEVSystemReadyStat, QString("telltalexEVSystemReadyStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEcallStat, QString("telltaleEcallStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAirBagStat, QString("telltaleAirBagStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampFailWarningStat, QString("telltaleLampFailWarningStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleIFSStat, QString("telltaleIFSStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewHBAStat, QString("telltaleADASDrivingNewHBAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleELKStat, QString("telltaleELKStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewELKStat, QString("telltaleADASDrivingNewELKStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleFCAStat, QString("telltaleFCAStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewFCAStat, QString("telltaleADASDrivingNewFCAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorFrontFogStat, QString("telltaleLampIndicatorFrontFogStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorRearFogStat, QString("telltaleLampIndicatorRearFogStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTailLampStat, QString("telltaleLampIndicatorTailLampStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorHighBeamStat, QString("telltaleLampIndicatorHighBeamStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorLowBeamStat, QString("telltaleLampIndicatorLowBeamStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalLeftStat,
                     QString("telltaleLampIndicatorTurnSignalLeftStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalRightStat,
                     QString("telltaleLampIndicatorTurnSignalRightStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleH2LeakStat, QString("telltaleH2LeakStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleH2FaultStat, QString("telltaleH2FaultStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleAFSDBL, QString("telltaleAFSDBL"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLDWLKAStat, QString("telltaleLDWLKAStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLKAStat, QString("telltaleADASDrivingNewLKAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarn, QString("telltaleLowFuelWarn"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleLowFuelWarnOptional, QString("telltaleLowFuelWarnOptional"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCALeftStat, QString("telltaleADASDrivingNewLCALeftStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCARightStat,
                     QString("telltaleADASDrivingNewLCARightStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleADASDrivingNewLFAStat, QString("telltaleADASDrivingNewLFAStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpEvStat, QString("telltaleSrvLmpEvStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpHevStat, QString("telltaleSrvLmpHevStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleSrvLmpFcevStat, QString("telltaleSrvLmpFcevStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::TelltaleEvPwrDnStat, QString("telltaleEvPwrDnStat"), QVariant(0));
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
