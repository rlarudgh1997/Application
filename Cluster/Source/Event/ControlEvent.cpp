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
    int propertyType = 0;

    switch (static_cast<DataType>(dataType)) {
        case DataType::Event: {
            Event eventType = static_cast<Event>(signalType);
            if (eventType == Event::Group1FullPopup1) {
                propertyType = ivis::common::PropertyEnum::EventType::PopupInfo;
            } else {
            }
            break;
        }
        default: {
            break;
        }
    }

    if (propertyType > 0) {
        updateDataHandler(propertyType, signalValue);
    }
}

void ControlEvent::slotServiceDatasChanged(const int& dataType, const int& signalType,
                                           const QHash<QString, QVariant>& signalValues) {
    if (signalValues.size() == 1) {
        // auto it = signalValues.constBegin();
        // slotServiceDataChanged(dataType, signalType, it.value());
        slotServiceDataChanged(dataType, signalType, signalValues.value(signalValues.keys().first()));
    } else {
        QString text = QString();
        QHashIterator<QString, QVariant> iter(signalValues);
        while (iter.hasNext()) {
            iter.next();
            text.append(QString("%1 : %2\n").arg(iter.key()).arg(iter.value().toString()));
        }
        slotServiceDataChanged(dataType, signalType, text);
    }
}
