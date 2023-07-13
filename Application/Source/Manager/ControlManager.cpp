#include "ControlManager.h"

#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlTop.h"
#include "ControlCenter.h"


QSharedPointer<ControlManager> ControlManager::instance() {
    static QSharedPointer<ControlManager> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlManager>(new ControlManager());
    }
    return gControl;
}

ControlManager::ControlManager() {
}

void ControlManager::init() {
    createControl(ScreenEnum::DisplayTypeTop);
}

void ControlManager::sendEventInfo(const int& eventType, const QVariant& eventValue) {
    int displayType = isDisplay(eventType);
    if (displayType != ScreenEnum::DisplayTypeInvalid) {
        createControl(displayType);
        emit signalEventInfoChanged(displayType, eventType, eventValue);
    }
}

void ControlManager::keyEvent(const int& inputType, const int& inputValue) {
#if defined(USE_SCREEN_CAPTURE)
    if ((inputType == KeyTypeEnum::KeyInputTypeRelease) && (inputValue == KeyTypeEnum::KeyInputValueCapture)) {
        ScreenInfo::instance().data()->captureScreen();
    }
#endif

#if defined(USE_KEY_EVENT)
    int displayType = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeMode).toInt();
    qDebug() << "ControlManager::keyEvent(" << inputType << "," << inputValue << ") :" << displayType;
    if (mControlInfo[displayType]) {
        mControlInfo[displayType]->keyEvent(inputType, inputValue);
    }
#endif
}

void ControlManager::mouseEvent(const int& inputType, const int& inputValue) {
}

int ControlManager::isDisplay(const int& eventType) {
    int displayType = ScreenEnum::DisplayTypeInvalid;
    switch (eventType) {
        case EventTypeEnum::EventTypeFileNew :
        case EventTypeEnum::EventTypeCenterVisible : {
            displayType = ScreenEnum::DisplayTypeCenter;
            break;
        }
        default : {
            displayType = ScreenEnum::DisplayTypeInvalid;
            break;
        }
    }
    return displayType;
}

void ControlManager::createControl(const int& displayType) {
    if (mControlInfo[displayType] == nullptr) {
        switch (displayType) {
            case ScreenEnum::DisplayTypeTop : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlTop::instance().data());
                break;
            }
            case ScreenEnum::DisplayTypeCenter : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlCenter::instance().data());
                break;
            }
            default : {
                break;
            }
        }

        if (mControlInfo[displayType]) {
            qDebug() << "ControlManager::createControl : " << displayType;
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeMode, displayType);
            mControlInfo[displayType]->init(displayType);
            setCurrentMode(displayType);
        }
    }
}

void ControlManager::setCurrentMode(const int& displayType) {
    mCurrentMode = displayType;
}

int ControlManager::getCurrentMode() {
    return mCurrentMode;
}

void ControlManager::exitProgram() {
    emit signalExitProgram();
}
