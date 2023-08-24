#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
//#include "CommonUtil.h"
#include "CommonResource.h"
#include "CommonPopup.h"

//#include <QProcess>


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

void ControlCenter::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        controlConnect(true);
        initBaseData();
        initCommonData(currentMode, ivis::common::ScreenEnum::DisplayTypeCenter);
    }
}

void ControlCenter::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlCenter::initBaseData() {
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
    // qDebug() << "ControlTop::slotConfigChanged(" << type << "," << value << ")";
    // switch (type) {
    //     case ConfigInfo::ConfigTypeScreenInfo : {
    //         QRect screenInfo = value.toRect();
    //         QSize screenSize = QSize(screenInfo.width(), screenInfo.height());
    //         updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
    //         break;
    //     }
    //     default : {
    //         break;
    //     }
    // }
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlCenter::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
            break;
        }
        default : {
            break;
        }
    }
}

void ControlCenter::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug() << "ControlCenter::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewConfig : {
            QVariantList allConfig = QVariantList();
            QMapIterator<int, QPair<QString, QVariant>> iter(ConfigSetting::instance().data()->allConfig());
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
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo, QVariant(allConfig), true);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewSignal : {
            QVariant nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath);
            QStringList sfcList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressSFC.info");
            QStringList vsmList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressVSM.info");
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll, (sfcList + vsmList), true);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
            break;
        }
        default : {
            break;
        }
    }
}
