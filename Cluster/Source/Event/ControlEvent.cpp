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
    resetControl(false);
    updateDataHandler(ivis::common::PropertyEnum::EventID, 0);
}

void ControlEvent::initControlData() {
}

void ControlEvent::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlEvent::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerEvent::signalHandlerEvent, this, &ControlEvent::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlEvent::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlEvent::slotEventInfoChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceDataChanged, this, &ControlEvent::slotServiceDataChanged,
                Qt::UniqueConnection);
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
        disconnect(Service::instance().data());
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
    if (dataType == static_cast<int>(DataType::Constant)) {
        // Constant
    }
}