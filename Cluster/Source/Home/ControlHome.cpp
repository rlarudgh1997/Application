#include "ControlHome.h"
#include "HandlerHome.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#if defined(__MODULE_SERVICE__)
#include "Service.h"
#endif

QSharedPointer<ControlHome>& ControlHome::instance() {
    static QSharedPointer<ControlHome> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlHome>(new ControlHome());
    }
    return gControl;
}

ControlHome::ControlHome() {
    isHandler();
}

AbstractHandler* ControlHome::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerHome::instance().data());
    }
    return mHandler;
}

bool ControlHome::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlHome::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlHome::initNormalData() {
    updateDataHandler(ivis::common::PropertyEnum::HomeType, ivis::common::HandlerHomeEnum::HomeType::Main);
}

void ControlHome::initControlData() {
}

void ControlHome::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(0, getData(ivis::common::PropertyEnum::CommonDisplay).toInt());
        initNormalData();
        initControlData();
    }
}

void ControlHome::controlConnect(const bool& state) {
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

void ControlHome::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlHome::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlHome::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlHome::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlHome::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlHome::updateDataService(const int& type, const QVariant& value) {
    // updateDataService
}

void ControlHome::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlHome::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlHome::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlHome::slotHandlerEvent() ->" << type << "," << value;
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

void ControlHome::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & displayType) == false) {
        return;
    }

    qDebug() << "ControlHome::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlHome::slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue) {
    QHash<int, QVariant> propertyData = QHash<int, QVariant>();

#if defined(__MODULE_SERVICE__)
    switch (static_cast<DataType>(dataType)) {
        case DataType::Constant: {
            Constant constantType = static_cast<Constant>(signalType);
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

void ControlHome::slotServiceDatasChanged(const int& dataType, const int& signalType,
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
