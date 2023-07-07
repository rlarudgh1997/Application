#include "ControlTop.h"
#include "HandlerTop.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"


QSharedPointer<ControlTop> ControlTop::instance() {
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

    QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);
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

void ControlTop::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case EventTypeEnum::EventTypeExitProgram : {
            ControlManager::instance().data()->exitProgram();
            break;
        }
        case EventTypeEnum::EventTypeCenterVisible : {
            ControlManager::instance().data()->sendEventInfo(type, QVariant());
            int result = system("python ../Example/excel_parsing.py");
            qDebug() << "system call" << ((result == 0) ? ("sucess :") : ("fail :")) << result;
            break;
        }
        case EventTypeEnum::EventTypeFileNew : {
            qDebug() << "File - New";
            break;
        }
        case EventTypeEnum::EventTypeFileOpen : {
            qDebug() << "File - Open";
            break;
        }
        case EventTypeEnum::EventTypeFileSave : {
            qDebug() << "File - Save";
            break;
        }
        case EventTypeEnum::EventTypeFileSaveAs : {
            qDebug() << "File - Save As";
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
