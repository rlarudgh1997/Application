#include "ControlTelltale.h"
#include "HandlerTelltale.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#if defined(__MODULE_SERVICE__)
#include "Service.h"
#endif

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
#if 0
    updateDataHandler(ivis::common::PropertyEnum::TelltaleKeySystemStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat.ON));
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
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpEvStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat.ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpHevStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleSrvLmpFcevStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
    updateDataHandler(ivis::common::PropertyEnum::TelltaleEvPwrDnStat,
                      static_cast<int>(ivis::common::TelltaleStatType::TelltaleStat::ON));
#endif
}

void ControlTelltale::initControlData() {
}

void ControlTelltale::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(0, getData(ivis::common::PropertyEnum::CommonDisplay).toInt());
        initNormalData();
        initControlData();
    }
}

void ControlTelltale::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &AbstractHandler::signalHandlerEvent,
                [=](const int& type, const QVariant& value) { slotHandlerEvent(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged,
                [=](const int& type, const QVariant& value) { slotConfigChanged(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigReset,
                [=](const bool& resetAll) { resetControl(resetAll); });
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                [=](const int& displayType, const int& eventType, const QVariant& eventValue) {
                    slotEventInfoChanged(displayType, eventType, eventValue);
                });
#if defined(__MODULE_SERVICE__)
        connect(Service::instance().data(), &Service::signalServiceDataChanged,
                [=](const int& dataType, const int& signalType, const QVariant& signalValue) {
                    slotServiceDataChanged(dataType, signalType, signalValue);
                });
        connect(Service::instance().data(), &Service::signalServiceDatasChanged,
                [=](const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues) {
                    slotServiceDatasChanged(dataType, signalType, signalValues);
                });
#endif
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
#if defined(__MODULE_SERVICE__)
        disconnect(Service::instance().data());
#endif
    }
}

void ControlTelltale::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlTelltale::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
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

void ControlTelltale::updateDataService(const int& type, const QVariant& value) {
    // updateDataService
}

void ControlTelltale::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
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

void ControlTelltale::slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue) {
    QHash<int, QVariant> propertyData = QHash<int, QVariant>();

#if defined(__MODULE_SERVICE__)
    switch (static_cast<DataType>(dataType)) {
        case DataType::Constant: {
            Constant constantType = static_cast<Constant>(signalType);
            break;
        }
        case DataType::Telltale: {
            Telltale telltaleType = static_cast<Telltale>(signalType);
            if (telltaleType == Telltale::FrontFogStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorFrontFogStat] = signalValue;
            } else if (telltaleType == Telltale::HighBeamStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorHighBeamStat] = signalValue;
            } else if (telltaleType == Telltale::RearFogStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorRearFogStat] = signalValue;
            } else if (telltaleType == Telltale::TailLampStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorTailLampStat] = signalValue;
            } else if (telltaleType == Telltale::TurnSignalLeftStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalLeftStat] = signalValue;
            } else if (telltaleType == Telltale::TurnSignalRightStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorTurnSignalRightStat] = signalValue;
            } else if (telltaleType == Telltale::LowBeamStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleLampIndicatorLowBeamStat] = signalValue;
            } else if (telltaleType == Telltale::LFAStat) {
                propertyData[ivis::common::PropertyEnum::TelltaleADASDrivingNewLFAStat] = signalValue;
            } else if ((telltaleType == Telltale::IceWarnStat) || (telltaleType == Telltale::IceWarnStatOptional)) {
            } else if ((telltaleType == Telltale::HandsOnOffStat) || (telltaleType == Telltale::HandsOnOffStatOptional)) {
            } else {
            }
            break;
        }
        default: {
            break;
        }
    }
#endif

    for (auto iter = propertyData.cbegin(); iter != propertyData.cend(); ++iter) {
        updateDataHandler(iter.key(), iter.value());
    }
}

void ControlTelltale::slotServiceDatasChanged(const int& dataType, const int& signalType,
                                              const QHash<QString, QVariant>& signalValues) {
#if defined(__MODULE_SERVICE__)
    if (signalValues.size() == 1) {
        slotServiceDataChanged(dataType, signalType, signalValues.value(signalValues.keys().first()));
    } else {
        QString multiValueInfo = QString();
        for (auto iter = signalValues.cbegin(); iter != signalValues.cend(); ++iter) {
            QString sfcName = iter.key();
            QVariant sfcValue = iter.value();
            multiValueInfo.append(QString("%1 : %2\n").arg(sfcName).arg(sfcValue.toString()));
        }
        slotServiceDataChanged(dataType, signalType, multiValueInfo);
    }
#endif
}
