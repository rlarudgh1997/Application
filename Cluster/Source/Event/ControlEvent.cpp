#include "ControlEvent.h"
#include "HandlerEvent.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#include "Service.h"

QSharedPointer<ControlEvent>& ControlEvent::instance() {
    static QSharedPointer<ControlEvent> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlEvent>(new ControlEvent());
    }
    return gControl;
}

ControlEvent::ControlEvent() {
    isHandler();
}

AbstractHandler* ControlEvent::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerEvent::instance().data());
    }
    return mHandler;
}

bool ControlEvent::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlEvent::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlEvent::initNormalData() {
    updateDataHandler(ivis::common::PropertyEnum::EventType, 0);
    updateDataHandler(ivis::common::PropertyEnum::PopupInfo, QVariant());
    updateDataHandler(ivis::common::PropertyEnum::PopupColorType,
                      static_cast<int>(ivis::common::PopupColorType::PopupColor::None));
    updateDataHandler(ivis::common::PropertyEnum::PopupColorStatus, false);
}

void ControlEvent::initControlData() {
}

void ControlEvent::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(0, getData(ivis::common::PropertyEnum::CommonDisplay).toInt());
        initNormalData();
        initControlData();
    }
}

void ControlEvent::controlConnect(const bool& state) {
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
        connect(Service::instance().data(), &Service::signalServiceDataChanged,
                [=](const int& dataType, const int& signalType, const QVariant& signalValue) {
                    slotServiceDataChanged(dataType, signalType, signalValue);
                });
        connect(Service::instance().data(), &Service::signalServiceDatasChanged,
                [=](const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues) {
                    slotServiceDatasChanged(dataType, signalType, signalValues);
                });
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
        disconnect(Service::instance().data());
    }
}

void ControlEvent::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (getTimerId(AbstractControl::AbstractTimerStart) == timerId) {
        controlTimer(AbstractControl::AbstractTimerStart);
        updateDataHandler(ivis::common::PropertyEnum::EventType, 0);
        updateDataHandler(ivis::common::PropertyEnum::PopupColorType, 0);
        updateDataHandler(ivis::common::PropertyEnum::PopupColorStatus, false);
        updateDataHandler(ivis::common::PropertyEnum::PopupInfo, QVariant());
    }
}

void ControlEvent::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlEvent::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlEvent::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlEvent::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlEvent::updateDataService(const int& type, const QVariant& value) {
    // updateDataService
}

void ControlEvent::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlEvent::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlEvent::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlEvent::slotHandlerEvent() ->" << type << "," << value;
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

void ControlEvent::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlEvent::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlEvent::slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue) {
    QHash<int, QVariant> propertyData = QHash<int, QVariant>();

    switch (static_cast<DataType>(dataType)) {
        case DataType::Event: {
            Event eventType = static_cast<Event>(signalType);
            if (eventType == Event::Group1FullPopup1) {
                propertyData[ivis::common::PropertyEnum::EventType] = static_cast<int>(eventType);
                propertyData[ivis::common::PropertyEnum::PopupColorStatus] = true;
                propertyData[ivis::common::PropertyEnum::PopupColorType] =
                    static_cast<int>(ivis::common::PopupColorType::PopupColor::WHITE);
                propertyData[ivis::common::PropertyEnum::PopupInfo] = signalValue;
                controlTimer(AbstractControl::AbstractTimerStart, true, 3000);
            }
            break;
        }
        default: {
            break;
        }
    }

    for (auto iter = propertyData.cbegin(); iter != propertyData.cend(); ++iter) {
        updateDataHandler(iter.key(), iter.value());
    }
}

void ControlEvent::slotServiceDatasChanged(const int& dataType, const int& signalType,
                                           const QHash<QString, QVariant>& signalValues) {
    if (signalValues.size() == 1) {
        slotServiceDataChanged(dataType, signalType, signalValues.value(signalValues.keys().first()));
    } else {
        QString multiValueInfo = QString();
        for (auto iter = signalValues.cbegin(); iter != signalValues.cend(); ++iter) {
            QString sfcName = iter.key();
            QVariant sfcValue = iter.value();
            bool event = (static_cast<DataType>(dataType) == DataType::Event);
            if ((event) && (static_cast<Event>(signalType) == Event::Group1FullPopup1)) {
                if (sfcName.contains("ID")) {
                    multiValueInfo.append(sfcValue.toString() + "\nGroup1FullPopup1_1");
                }
            } else {
                multiValueInfo.append(QString("%1 : %2\n").arg(sfcName).arg(sfcValue.toString()));
            }
        }
        slotServiceDataChanged(dataType, signalType, multiValueInfo);
    }
}
