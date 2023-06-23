#include "ControlManager.h"

#include "CommonEnum.h"

#include "ScreenInfo.h"
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
    qDebug() << "ControlManager";
}

void ControlManager::keyEvent(const int& inputType, const int& inputValue) {
    qDebug() << "ControlManager::keyEvent : " << inputType << ", " << inputValue;

#if defined(USE_SCREEN_CAPTURE)
    if ((inputType == KeyTypeEnum::KeyInputTypeRelease) && (inputValue == KeyTypeEnum::KeyInputValueCapture)) {
        ScreenInfo::instance().data()->captureScreen();
    }
#endif

#if defined(USE_KEY_EVENT)
    int displayType = getCurrentMode();
    if (mControlInfo[displayType]) {
        mControlInfo[displayType]->keyEvent(inputType, inputValue);
    }
#endif
}

void ControlManager::mouseEvent(const int& inputType, const int& inputValue) {
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
            mCurrentMode = displayType;
            mControlInfo[displayType]->init(displayType);
        }
    }
}

void ControlManager::requestDisplayChange(const int& displayType) {
    qDebug() << "ControlManager::requestDisplayChange : " << displayType << ", " << mControlInfo[displayType];
    if (mControlInfo[displayType] == nullptr) {
        createControl(displayType);
    }
    emit signalDisplayChanged(displayType);
}

int ControlManager::getCurrentMode() {
    return mCurrentMode;
}

void ControlManager::exitProgram() {
    emit signalExitProgram();
}











