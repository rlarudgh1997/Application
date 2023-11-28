#include "ControlTelltale.h"
#include "HandlerTelltale.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

QSharedPointer<ControlTelltale>& ControlTelltale::instance() {
    static QSharedPointer<ControlTelltale> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlTelltale>(new ControlTelltale());
    }
    return gControl;
}

ControlTelltale::ControlTelltale() {
    isHandler();
}

AbstractHandler* ControlTelltale::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerTelltale::instance().data());
    }
    return mHandler;
}

bool ControlTelltale::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlTelltale::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlTelltale::initNormalData() {
    resetControl(false);
    updateDataHandler(ivis::common::PropertyEnum::TelltaleType, 0);  // 0 : PV, 1 : CV

#if 1
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSeatbeltStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltalexEVSystemReadyStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEcallStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAirBagStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampFailWarningStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleIFSStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GREEN));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewHBAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::WHITE));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleELKStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::NONE));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewELKStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::NONE));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleFCAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::RED));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewFCAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::YELLOW));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorFrontFogStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorRearFogStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorTailLampStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    // updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorHighBeamStat,
    //                     static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorLowBeamStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalLeftStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalRightStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleH2LeakStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleH2FaultStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    // updateDataHandler(ivis::common::PropertyEnum::TelltaleAFSDBL,
    //                     static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLDWLKAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GRAY));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewLKAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GREEN));
    // updateDataHandler(ivis::common::PropertyEnum::TelltaleLowFuelWarn,
    //                     static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GREEN));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCALeftStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::WHITE));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewLCARightStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GREEN));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleADASDrivingNewLFAStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::GREEN));

#if 0  // QML 에서 사용하지 않음
    updateDataHandler(ivis::common::PropertyEnum::TelltaleDualPowerStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleDualPowerOptional,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLiftgateStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleBEQStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTachographStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleHeatedMirrorStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleWorkingLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleASRStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleASRBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleASRBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEBSAmberLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEBSRedLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLWSStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLWSBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLWSBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASOptionalStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASBlinkValueAOptional,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASAxleMotionStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleECASAxleMotionStatOptional,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleBrakeAirLowLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEHSAmberLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEHSAmberLampBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEHSAmberLampBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEAPULampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAutoGreaseLowLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleCabTiltingStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTMPTOStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleDifferencialLockStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTMHighStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleWheelLockStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleRetarderLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleRetarderMalLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleHighVoltageBatteryChargeStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTPMSCVStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTPMSCVBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleTPMSCVBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleABSTrailerLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleNoABSTrailerLamp,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleABSCVStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleVDCStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleVDCBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleVDCBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleVDCOFFStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleStreeingWarningLampStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleParkingBrakeCVStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpEvStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpHevStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpFcevStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleOATIceWarnStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleOATIceWarnStatOptional,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAirBagBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAirBagBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleKeySystemStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAFSDBLStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAFSDBLBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleAFSDBLBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEvPwrDnStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLDWLKABlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLDWLKABlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleFCABlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleFCABlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleELKBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleELKBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLowFuelWarnStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLowFuelWarnStatOptional,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLowFuelWarnBlinkValueA,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleLowFuelWarnBlinkValueB,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltalexEVServiceLampCVSrvLmpEvStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltalexEVServiceLampCVSrvLmpFcevStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltalexEVPowerLimitEvPwrDnStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleGFDWarningGFDStat,
                        static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
#endif
#endif
}

void ControlTelltale::initControlData() {
}

void ControlTelltale::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlTelltale::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerTelltale::signalHandlerEvent, this, &ControlTelltale::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlTelltale::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlTelltale::slotEventInfoChanged, Qt::UniqueConnection);
#if defined(USE_RESIZE_SIGNAL)
        connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged,
                [=](const QSize& screenSize) { updateDataHandler(ivis::common::PropertyEnum::CommonRect, screenSize); });
#endif
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
    }
}

void ControlTelltale::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlTelltale::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlTelltale::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyEnum::CommonRect, QSize(width, height));
#endif
}

void ControlTelltale::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlTelltale::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlTelltale::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlTelltale::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlTelltale::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlTelltale::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventEnum::EventTypeExitProgram: {
            break;
        }
        default: {
            break;
        }
    }
}

void ControlTelltale::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlTelltale::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlTelltale::slotServiceDataChanged(const int& dataType, const QVariant& dataValue) {
}
