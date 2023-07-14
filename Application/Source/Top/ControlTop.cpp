#include "ControlTop.h"
#include "HandlerTop.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"

Q_LOGGING_CATEGORY(C_TOP, "ControlTop")


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
    QStringList sfcList = FileInfo::parsingFile(defaultPath + "/NodeAddressSFC.info");
    QStringList vsmList = FileInfo::parsingFile(defaultPath + "/NodeAddressVSM.info");
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
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlTop::slotEventInfoChanged,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlTop::slotConfigChanged,
                Qt::UniqueConnection);
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

void ControlTop::updateDataHandler(const int& type, const QVariant& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, value);
    }
}

void ControlTop::updateDataHandler(const int& type, const QVariantList& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, getData(type));
    }
}

void ControlTop::slotConfigChanged(const int& type, const QVariant& value) {
    // qDebug(C_TOP) << "ControlTop::slotConfigChanged() ->" << type << "," << value;
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


void ControlTop::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug(C_TOP) << "ControlTop::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;

    switch (eventType) {
        default : {
            break;
        }
    }
}

void ControlTop::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case EventTypeEnum::EventTypeExitProgram : {
            ControlManager::instance().data()->exitProgram();
            break;
        }
        case EventTypeEnum::EventTypeCenterVisible : {
            ControlManager::instance().data()->sendEventInfo(type, QVariant());
            break;
        }
        case EventTypeEnum::EventTypeFileNew : {
            ControlManager::instance().data()->sendEventInfo(type, QVariant());
            break;
        }
        case EventTypeEnum::EventTypeFileOpen : {
            qDebug(C_TOP) << "File - Open";
            break;
        }
        case EventTypeEnum::EventTypeFileSave : {
            qDebug(C_TOP) << "File - Save";
            break;
        }
        case EventTypeEnum::EventTypeFileSaveAs : {
            qDebug(C_TOP) << "File - Save As";
            break;
        }
        case EventTypeEnum::EventTypeEditCut : {
            qDebug(C_TOP) << "Edit - Cut";
            break;
        }
        case EventTypeEnum::EventTypeEditCopy : {
            qDebug(C_TOP) << "Edit - Copy";
            break;
        }
        case EventTypeEnum::EventTypeEditPaste : {
            qDebug(C_TOP) << "Edit - Paste";
            break;
        }
        case EventTypeEnum::EventTypeSettingDevPath : {
            updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, value);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultPath, value);
            break;
        }
        case EventTypeEnum::EventTypeSettingTestReport : {
            qDebug(C_TOP) << "Setting - Test Report";
            break;
        }
        case EventTypeEnum::EventTypeSettingTestResult : {
            qDebug(C_TOP) << "Setting - Test Result";
            break;
        }
        case EventTypeEnum::EventTypeSettingTestCoverage : {
            qDebug(C_TOP) << "Setting - Test Coverage";
            break;
        }
        default : {
            break;
        }
    }
}
