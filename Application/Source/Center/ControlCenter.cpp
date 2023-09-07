#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
//#include "CommonUtil.h"
#include "CommonResource.h"
#include "CommonPopup.h"


QSharedPointer<ControlCenter>& ControlCenter::instance() {
    static QSharedPointer<ControlCenter> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlCenter>(new ControlCenter());
    }
    return gControl;
}

ControlCenter::ControlCenter() {
    isHandler();
}

AbstractHandler* ControlCenter::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerCenter::instance().data());
    }
    return mHandler;
}

bool ControlCenter::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlCenter::initCommonData(const int& currentMode) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, ivis::common::ScreenEnum::DisplayTypeCenter);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlCenter::initNormalData() {
    resetControl(false);
}

void ControlCenter::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlCenter::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerCenter::signalHandlerEvent,
                this,                              &ControlCenter::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlCenter::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlCenter::slotEventInfoChanged,
                Qt::UniqueConnection);
#if defined(USE_RESIZE_SIGNAL)
        connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
        });
#endif
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
    }
}

void ControlCenter::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlCenter::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlCenter::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlCenter::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlCenter::updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlCenter::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlCenter::slotConfigChanged(const int& type, const QVariant& value) {
    qDebug() << "ControlCenter::slotConfigChanged(" << type << "," << value << ")";
#if 1
    int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
    if (viewType == ivis::common::ViewTypeEnum::ViewTypeConfig) {
        sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, ivis::common::EventTypeEnum::EventTypeViewConfig, "");
    }
#else
    switch (type) {
        case ConfigInfo::ConfigTypeInit :
        case ConfigInfo::ConfigTypeCheckLibOpenpyxl :
        case ConfigInfo::ConfigTypeCheckLibPandas :
        case ConfigInfo::ConfigTypeDefaultPath : {
            int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
            if (viewType == ivis::common::ViewTypeEnum::ViewTypeConfig) {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, ivis::common::EventTypeEnum::EventTypeViewConfig, "");
            }
        }
        default : {
            break;
        }
    }
#endif
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlCenter::slotHandlerEvent() ->" << type << "," << value;
    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, ivis::common::EventTypeEnum::EventTypeViewInfoClose, "");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeWriteConfig : {
            QVariantList configInfo = value.toList();
            if (configInfo.size() == 2) {
                int configType = configInfo.at(0).toInt();
                QVariant configValue = configInfo.at(1);
                ConfigSetting::instance().data()->editConfig(configType, configValue);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeConfigReset : {
            ConfigSetting::instance().data()->resetConfig();
            break;
        }
        default : {
            break;
        }
    }
}

void ControlCenter::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlCenter::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewConfig : {
            QMapIterator<int, QPair<QString, QVariant>> iter(ConfigSetting::instance().data()->allConfig());
            QVariantList allConfig = QVariantList();
            while (iter.hasNext()) {
                iter.next();
                QStringList temp = iter.value().first.split("ConfigType");
                if (temp.size() != 2) {
                    continue;
                }
                int type = iter.key();
                QString name = temp.at(1);
                QVariant value = iter.value().second;
                allConfig.append(QVariant(QVariantList({type, name, value})));
            }
            if (allConfig.size() > 0) {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType,
                                                                    ivis::common::ViewTypeEnum::ViewTypeConfig);
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo, QVariant(allConfig), true);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress : {
            QVariant nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath);
            QStringList sfcList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressSFC.info");
            QStringList vsmList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressVSM.info");
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::ViewTypeSignal);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll, (sfcList + vsmList), true);
            break;
        }
        default : {
            break;
        }
    }
}
