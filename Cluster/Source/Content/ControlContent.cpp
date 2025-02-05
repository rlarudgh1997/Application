#include "ControlContent.h"
#include "HandlerContent.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#if defined(__MODULE_SERVICE__)
#include "Service.h"
#endif

QSharedPointer<ControlContent>& ControlContent::instance() {
    static QSharedPointer<ControlContent> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlContent>(new ControlContent());
    }
    return gControl;
}

ControlContent::ControlContent() {
    isHandler();
}

AbstractHandler* ControlContent::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerContent::instance().data());
    }
    return mHandler;
}

bool ControlContent::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlContent::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlContent::initNormalData() {
    updateDataHandler(ivis::common::PropertyEnum::ContentType, 0);
    updateDataHandler(ivis::common::PropertyEnum::ContentInfo, QVariant());
}

void ControlContent::initControlData() {
}

void ControlContent::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(0, getData(ivis::common::PropertyEnum::CommonDisplay).toInt());
        initNormalData();
        initControlData();
    }
}

void ControlContent::controlConnect(const bool& state) {
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

void ControlContent::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlContent::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlContent::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlContent::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlContent::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlContent::updateDataService(const int& type, const QVariant& value) {
    // updateDataService
}

void ControlContent::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlContent::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlContent::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlContent::slotHandlerEvent() ->" << type << "," << value;
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

void ControlContent::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & displayType) == false) {
        return;
    }

    qDebug() << "ControlContent::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlContent::slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue) {
    QHash<int, QVariant> propertyData = QHash<int, QVariant>();

#if defined(__MODULE_SERVICE__)
    switch (static_cast<DataType>(dataType)) {
        case DataType::Constant: {
            Constant constantType = static_cast<Constant>(signalType);
            if (constantType == Constant::ViewFrontVehicle) {
                propertyData[ivis::common::PropertyEnum::ContentInfo] = signalValue;
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

void ControlContent::slotServiceDatasChanged(const int& dataType, const int& signalType,
                                             const QHash<QString, QVariant>& signalValues) {
#if defined(__MODULE_SERVICE__)
    if (signalValues.size() == 1) {
        slotServiceDataChanged(dataType, signalType, signalValues.value(signalValues.keys().first()));
    } else {
        if ((static_cast<DataType>(dataType) == DataType::Constant) &&
            (static_cast<Constant>(signalType) == Constant::ViewFrontVehicle)) {
            QStringList keyList = {"SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Stat",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Stat",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat",
                                   "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value"};
            QHash<int, QString> info;
            int count = 0;
            for (QString key : keyList) {
                int index;
                QString title;
                int value = signalValues[key].toInt();
                if (count < 4) {
                    index = 0;
                    title = key.remove("SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.");
                } else {
                    index = 1;
                    title = key.remove("SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.");
                }
                count++;

                // if ((title.contains("Value")) && (value == 0)) {
                //     continue;
                // }
                if (title.isEmpty() == false) {
                    info[index].append(QString("%1 : %2\n").arg(title).arg(value));
                }
            }
            updateDataHandler(ivis::common::PropertyEnum::ViewFrontLeftVehicle, info[0]);
            updateDataHandler(ivis::common::PropertyEnum::ViewFrontRightVehicle, info[1]);
            return;
        }
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
