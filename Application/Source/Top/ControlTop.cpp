#include "ControlTop.h"
#include "HandlerTop.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonPopup.h"

// Q_LOGGING_CATEGORY(C_TOP, "ControlTop")


QSharedPointer<ControlTop>& ControlTop::instance() {
    static QSharedPointer<ControlTop> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlTop>(new ControlTop());
    }
    return gControl;
}

ControlTop::ControlTop() {
    isHandler();
}

AbstractHandler* ControlTop::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerTop::instance().data());
    }
    return mHandler;
}

void ControlTop::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();

        controlConnect(true);
        initBaseData();
        initCommonData(currentMode, ScreenEnum::DisplayTypeTop);
    }
}

void ControlTop::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDepth, ScreenEnum::DisplayDepthDepth0);
}

void ControlTop::initBaseData() {
    resetControl(false);

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);

    CheckTimer checkTimer;
    QStringList sfcList = FileInfo::readFile(defaultPath + "/NodeAddressSFC.info");
    QStringList vsmList = FileInfo::readFile(defaultPath + "/NodeAddressVSM.info");
    updateDataHandler(PropertyTypeEnum::PropertyTypeSignalListAll, (sfcList + vsmList));
    updateDataHandler(PropertyTypeEnum::PropertyTypeSignalListSFC, sfcList);
    updateDataHandler(PropertyTypeEnum::PropertyTypeSignalListVSM, vsmList);
    checkTimer.check("NodeAddress");
}

void ControlTop::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlTop::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerTop::signalHandlerEvent,
                this,                              &ControlTop::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlTop::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlTop::slotEventInfoChanged,
                Qt::UniqueConnection);
#if defined(USE_RESIZE_SIGNAL)
        connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
                updateDataHandler(PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
        });
#endif
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
    }
}

void ControlTop::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlTop::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlTop::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlTop::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlTop::updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlTop::slotConfigChanged(const int& type, const QVariant& value) {
    // qDebug() << "ControlTop::slotConfigChanged() ->" << type << "," << value;
    // switch (type) {
    //     case ConfigInfo::ConfigTypeDefaultPath : {
    //         updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, value);
    //         break;
    //     }
    //     default : {
    //         break;
    //     }
    // }
}

void ControlTop::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlTop::slotHandlerEvent() ->" << type << "," << value;
    CheckTimer checkTimer;

    switch (type) {
        case EventTypeEnum::EventTypeExitProgram : {
            ControlManager::instance().data()->exitProgram();
            break;
        }
        case EventTypeEnum::EventTypeHelpAbout : {
            Popup::drawPopupAbout(isHandler());
            break;
        }
        case EventTypeEnum::EventTypeHelpAboutQt : {
            Popup::drawPopupAboutQt(isHandler());
            break;
        }
        case EventTypeEnum::EventTypeCenterVisible :
        case EventTypeEnum::EventTypeFileNew :
        case EventTypeEnum::EventTypeFileOpen :
        case EventTypeEnum::EventTypeFileSave :
        case EventTypeEnum::EventTypeFileSaveAs : {
            ControlManager::instance().data()->sendEventInfo(getData(PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                                ScreenEnum::DisplayTypeCenter,
                                                                type,
                                                                value);
            break;
        }
        case EventTypeEnum::EventTypeEditCut : {
            qDebug() << "Edit - Cut";
            break;
        }
        case EventTypeEnum::EventTypeEditCopy : {
            qDebug() << "Edit - Copy";
            break;
        }
        case EventTypeEnum::EventTypeEditPaste : {
            qDebug() << "Edit - Paste";
            break;
        }
        case EventTypeEnum::EventTypeSettingDevPath : {
            QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
            Popup::drawPopupDevPath(isHandler(), EventTypeEnum::EventTypeUpdateDevPath, defaultPath);
            break;
        }
        case EventTypeEnum::EventTypeUpdateDevPath : {
            updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, value);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultPath, value);
            break;
        }
        case EventTypeEnum::EventTypeSettingTestReport : {
            qDebug() << "Setting - Test Report";
            break;
        }
        case EventTypeEnum::EventTypeSettingTestResult : {
            qDebug() << "Setting - Test Result";
            break;
        }
        case EventTypeEnum::EventTypeSettingTestCoverage : {
            qDebug() << "Setting - Test Coverage";
            break;
        }
        default : {
            break;
        }
    }
}

void ControlTop::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug() << "ControlTop::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;

    switch (eventType) {
        default : {
            break;
        }
    }
}
