#include "ControlGauge.h"
#include "HandlerGauge.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"
#include "Service.h"

#include "Service.h"

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
    resetControl(false);

    // Handler Data
    updateDataHandler(ivis::common::PropertyEnum::GaugeDefaultAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeed, 260);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpm, 2000);
    updateDataHandler(ivis::common::PropertyEnum::GaugeFuel, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeTemperature, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeRpmAngle, mDefaultAngle);
    updateDataHandler(ivis::common::PropertyEnum::GaugeFuelAngle, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeTemperatureAngle, 0);
    updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedUnit,
                      static_cast<int>(ivis::common::SpeedUnitType::SpeedUnit::KM_PER_HOUR));
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
    Q_UNUSED(reset)
}

void ControlGauge::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerGauge::signalHandlerEvent, this, &ControlGauge::slotHandlerEvent, Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlGauge::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlGauge::slotEventInfoChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceConstantChanged, this,
                &ControlGauge::slotServiceConstantChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceTelltaleChanged, this,
                &ControlGauge::slotServiceTelltaleChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceEventChanged, this,
                &ControlGauge::slotServiceEventChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceSoundChanged, this,
                &ControlGauge::slotServiceSoundChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceEtcChanged, this,
                &ControlGauge::slotServiceEtcChanged, Qt::UniqueConnection);
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
        disconnect(Service::instance().data());
    }
}

void ControlGauge::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)

#if defined(USE_GAUGE_TEMP_VALUE)
    if (timerId == getTimerId(ControlGaugeTimerSpeed)) {
        int speed = getData(ivis::common::PropertyEnum::GaugeSpeed).toInt();
        ivis::common::REVOLVE_P(speed, 10, 0, 260);
        slotServiceConstantChanged(Service::Contant::SpeedAnalogStat, speed);
    } else if (timerId == getTimerId(ControlGaugeTimerRpm)) {
        int rpm = getData(ivis::common::PropertyEnum::GaugeRpm).toInt();
        ivis::common::REVOLVE_P(rpm, 100, 0, 6000);
        slotServiceConstantChanged(Service::Contant::SpeedDigitalStat, rpm);
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
            break;
        }
    }

    return gaugeAngle;
}

void ControlGauge::updateGaugeInfo(const int& dataType, const QVariant& dataValue) {
    switch (dataType) {
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeed: {
            int speedUnit = getData(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeedUnit).toInt();
            int gaugeType = ((speedUnit == static_cast<int>(ivis::common::SpeedUnitType::SpeedUnit::MILE_PER_HOUR)) ?
                             (ivis::common::GaugeTypeEnum::GaugeTypeSpeedMile) : (ivis::common::GaugeTypeEnum::GaugeTypeSpeed));
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeed, dataValue);
            updateDataHandler(ivis::common::PropertyEnum::GaugeSpeedAngle, isGaugeAngle(gaugeType, dataValue));
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeRpm: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeRpm, dataValue);
            updateDataHandler(ivis::common::PropertyEnum::GaugeRpmAngle, isGaugeAngle(ivis::common::GaugeTypeEnum::GaugeTypeRpm,
                              dataValue));
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeFuel: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeFuel, dataValue);
            // updateDataHandler(ivis::common::PropertyEnum::GaugeFuelAngle, dataValue);
            break;
        }
        case ivis::common::ServiceDataTypeEnum::ServiceDataTypeTemperature: {
            updateDataHandler(ivis::common::PropertyEnum::GaugeTemperature, dataValue);
            // updateDataHandler(ivis::common::PropertyEnum::GaugeTemperatureAngle, dataValue);
            break;
        }
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

void ControlGauge::slotServiceConstantChanged(const int& signalType, const QVariant& signalValue) {
    qDebug() << "slotServiceConstantChanged :" << signalType << signalValue.type() << signalValue;

    if (signalType == Service::Constant::SpeedAnalogStat) {
        updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeSpeed, signalValue);
    } else if (signalType == Service::Constant::SpeedDigitalStat) {
        updateGaugeInfo(ivis::common::ServiceDataTypeEnum::ServiceDataTypeRpm, signalValue);
    } else {
    }
}

void ControlGauge::slotServiceTelltaleChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlGauge::slotServiceEventChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlGauge::slotServiceSoundChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlGauge::slotServiceEtcChanged(const int& signalType, const QVariant& signalValue) {
}
