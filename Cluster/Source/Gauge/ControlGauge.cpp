#include "ControlGauge.h"
#include "HandlerGauge.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#if defined(__MODULE_SERVICE__)
#include "Service.h"
#endif

// #define USE_GAUGE_TEMP_VALUE

QSharedPointer<ControlGauge>& ControlGauge::instance() {
    static QSharedPointer<ControlGauge> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlGauge>(new ControlGauge());
    }
    return gControl;
}

ControlGauge::ControlGauge() {
    isHandler();
}

AbstractHandler* ControlGauge::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerGauge::instance().data());
    }
    return mHandler;
}

bool ControlGauge::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlGauge::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlGauge::initNormalData() {
    // Handler Data
    updateDataHandler(ivis::common::PropertyEnum::GaugeDefaultAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeed, 260);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngle, 260);
    updateDataHandler(ivis::common::PropertyEnum::GaugeFuel, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeTemperature, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeFuelAngle, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeTemperatureAngle, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedUnit,
                      static_cast<int>(ivis::common::SpeedUnitType::SpeedUnit::KM_PER_HOUR));

    // Tachometer Contant
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpmState, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpm, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpmAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRedZoneExceptNbrandStat, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRedZoneNbrandStat, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeMaxRpmStat, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpmDampStat, 0);
}

void ControlGauge::initControlData() {
    updateDataControl(ivis::common::PropertyEnum::GaugeSpeedState,
                      static_cast<int>(ivis::common::SpeedStateType::SpeedState::VALUE));
    updateDataControl(ivis::common::PropertyEnum::GaugeSpeedSubDigitalState,
                      static_cast<int>(ivis::common::SpeedSubDigitalStateType::SpeedSubDigitalState::VALUE));
    updateDataControl(ivis::common::PropertyEnum::GaugeRpmState, static_cast<int>(ivis::common::RpmStateType::RpmState::VALUE));

#if defined(USE_GAUGE_TEMP_VALUE)
    controlTimer(ControlGaugeTimerSpeed, true, 200);
    controlTimer(ControlGaugeTimerRpm, true, 150);
#endif
}

void ControlGauge::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(0, getData(ivis::common::PropertyEnum::CommonDisplay).toInt());
        initNormalData();
        initControlData();
    }
}

void ControlGauge::controlConnect(const bool& state) {
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

void ControlGauge::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)

#if defined(USE_GAUGE_TEMP_VALUE)
    if (timerId == getTimerId(ControlGaugeTimerSpeed)) {
        int speed = getData(ivis::common::PropertyEnum::GaugeSpeed).toInt();
        ivis::common::REVOLVE_P(speed, 10, 0, 260);
        slotServiceDataChanged(static_cast<int>(DataType::Constant), static_cast<int>(Contant::SpeedAnalogStat), speed);
    } else if (timerId == getTimerId(ControlGaugeTimerRpm)) {
        int rpm = getData(ivis::common::PropertyEnum::GaugeRpm).toInt();
        ivis::common::REVOLVE_P(rpm, 100, 0, 6000);
        slotServiceDataChanged(static_cast<int>(DataType::Constant), static_cast<int>(Contant::SpeedDigitalStat), rpm);
    } else {
    }
#endif
}

void ControlGauge::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlGauge::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlGauge::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlGauge::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlGauge::updateDataService(const int& type, const QVariant& value) {
    // updateDataService
}

qreal ControlGauge::isGaugeAngle(const int& gaugeType, const QVariant& gaugeValue) {
    const double speedFactor140 = 1.11;     //  153/140
    const double speedFactor260 = 0.745;    //   87/120   -> Base : 0.725, New : 0.745
    const double speedFactorMile = 1.5375;  //  246/160
    const double rpmFactor = 0.0405;        // 240/6000
    const double rpmFactor6000 = 0.039;     // RPM : 3000 ~ 6000 이상에서 좌표 안 맞아서 수정 적용

    qreal gaugeAngle = 0.0;
    int value = gaugeValue.toInt();

    switch (gaugeType) {
        case ivis::common::GaugeTypeEnum::GaugeTypeSpeed: {
            if (value > 140) {
                gaugeAngle = (33 + ((value - 140) * speedFactor260));
            } else {
                gaugeAngle = mDefaultAngle + (value * speedFactor140);
            }
            break;
        }
        case ivis::common::GaugeTypeEnum::GaugeTypeSpeedMile: {
            if (value >= 0 && value <= 160) {
                gaugeAngle = (mDefaultAngle - 3) + (value * speedFactorMile);
            }
            break;
        }
        case ivis::common::GaugeTypeEnum::GaugeTypeRpm: {
            if (value >= 3000) {
                gaugeAngle = mDefaultAngle + (value * rpmFactor);
            } else {
                gaugeAngle = mDefaultAngle + (value * rpmFactor6000);
            }
            break;
        }
        default: {
            qDebug() << "default";
            break;
        }
    }

    return gaugeAngle;
}

void ControlGauge::updateGaugeInfo(const int& dataType, const QVariant& dataValue) {
    switch (dataType) {
        // Speed_Gauge
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeAnalogSpeedState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngleState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeAnalogSpeed: {
            int speedUnit = getData(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedMainDisplayUnitState).toInt();
            int gaugeType = ((speedUnit == static_cast<int>(ivis::common::SpeedUnitType::SpeedUnit::MILE_PER_HOUR))
                                 ? (ivis::common::GaugeTypeEnum::GaugeTypeSpeedMile)
                                 : (ivis::common::GaugeTypeEnum::GaugeTypeSpeed));
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngle, isGaugeAngle(gaugeType, dataValue));
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeed: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeed, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDigitalState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedSubDigitalState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDigital: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedSubDigital, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedMainDisplayUnitState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedUnit, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedAuxDisplayUnitState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAuxDisplayUnit, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDisplayState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedSubDisplayState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedScaleMaximumState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedScaleMaxState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitState: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitState, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitOver1ColorValue: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitOver1ColorValue, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitOver2ColorValue: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitOver2ColorValue, dataValue);
            break;
        }
        // Tachometer(Rpm Gauge)
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeRpm: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeRpm, dataValue);
            updateDataHandler(ivis::common::PropertyEnum::GaugeRpmAngle,
                              isGaugeAngle(ivis::common::GaugeTypeEnum::GaugeTypeRpm, dataValue));
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataRedZoneExceptNbrandStat: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeRedZoneExceptNbrandStat, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataRedZoneNbrandStat: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeRedZoneNbrandStat, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataMaxRpmStat: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeMaxRpmStat, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataRpmDampStat: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeRpmDampStat, dataValue);
            break;
        }
        // Fuel
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeFuel: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeFuel, dataValue);
            break;
        }
        // Temperature
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeTemperature: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeTemperature, dataValue);
            break;
        }
        default: {
            break;
        }
    }
}

void ControlGauge::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlGauge::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlGauge::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlGauge::slotHandlerEvent() ->" << type << "," << value;
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

void ControlGauge::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlGauge::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlGauge::slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue) {
    QHash<int, QVariant> propertyData = QHash<int, QVariant>();

#if defined(__MODULE_SERVICE__)
    switch (static_cast<DataType>(dataType)) {
        case DataType::Constant: {
            Constant constantType = static_cast<Constant>(signalType);
            if (constantType == Constant::SpeedAnalogStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeAnalogSpeedState, signalValue);
            } else if (constantType == Constant::SpeedAnalogValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeAnalogSpeed, signalValue);
            } else if (constantType == Constant::SpeedDigitalStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedState, signalValue);
            } else if (constantType == Constant::SpeedDigitalValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeed, signalValue);
            } else if (constantType == Constant::SpeedSubDigitalStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDigitalState, signalValue);
            } else if (constantType == Constant::SpeedSubDigitalValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDigital, signalValue);
            } else if (constantType == Constant::SpeedMainDisplayUnitStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedMainDisplayUnitState, signalValue);
            } else if (constantType == Constant::SpeedAuxDisplayUnitStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedAuxDisplayUnitState, signalValue);
            } else if (constantType == Constant::SpeedSubDisplayStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedSubDisplayState, signalValue);
            } else if (constantType == Constant::SpeedScaleMaximumStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedScaleMaximumState, signalValue);
            } else if (constantType == Constant::NaviSpeedLimitStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitState, signalValue);
            } else if (constantType == Constant::NaviSpeedLimitOver1ColorValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitOver1ColorValue, signalValue);
            } else if (constantType == Constant::NaviSpeedLimitOver2ColorValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeNaviSpeedLimitOver2ColorValue, signalValue);
            } else if (constantType == Constant::RpmValue) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeRpm, signalValue);
            } else if (constantType == Constant::RedZoneExceptNbrandStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataRedZoneExceptNbrandStat, signalValue);
            } else if (constantType == Constant::RedZoneNbrandStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataRedZoneNbrandStat, signalValue);
            } else if (constantType == Constant::MaxRpmStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataMaxRpmStat, signalValue);
            } else if (constantType == Constant::RpmDampStat) {
                updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataRpmDampStat, signalValue);
            } else {
            }
            break;
        }
        case DataType::Event: {
            Event eventType = static_cast<Event>(signalType);
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

void ControlGauge::slotServiceDatasChanged(const int& dataType, const int& signalType,
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
