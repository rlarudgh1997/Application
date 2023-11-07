#include "ControlManager.h"

#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlMenu.h"
#include "ControlCenter.h"
#include "ControlExcel.h"
// #include "CommonUtil.h"


QSharedPointer<ControlManager>& ControlManager::instance() {
    static QSharedPointer<ControlManager> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlManager>(new ControlManager());
    }
    return gControl;
}

ControlManager::ControlManager() {
}

void ControlManager::init() {
    setCurrentMode(ivis::common::ScreenEnum::DisplayTypeMenu);
    createControl(ivis::common::ScreenEnum::DisplayTypeMenu);
}

void ControlManager::sendEventInfo(const int& source, const int& destination, const int& eventType, const QVariant& eventValue) {
    qDebug() << "[sendEventInfo] Direction :" << source << " -> " << destination << ", Type :" << eventType;
    if (destination != ivis::common::ScreenEnum::DisplayTypeInvalid) {
        createControl(destination);
        emit signalEventInfoChanged(destination, eventType, eventValue);
    }

    if (source == ivis::common::ScreenEnum::DisplayTypeMenu) {
        ivis::common::CheckTimer checkTimer;
        raise(destination);
        checkTimer.check("RaiseScreen");
    }
}

void ControlManager::raise(const int& displayType) {
    ScreenInfo::instance().data()->controlScreen(displayType);
}

void ControlManager::keyEvent(const int& inputType, QKeyEvent* keyEvent) {
    int inputValue = keyEvent->key();
#if defined(PLATFORM_X86)
    switch (inputValue) {
        case ivis::common::KeyTypeEnum::KeyInputValueNumUp    : {
            inputValue = ivis::common::KeyTypeEnum::KeyInputValueUp;
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueNumDown  : {
            inputValue = ivis::common::KeyTypeEnum::KeyInputValueDown;
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueNumLeft  : {
            inputValue = ivis::common::KeyTypeEnum::KeyInputValueLeft;
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueNumRight : {
            inputValue = ivis::common::KeyTypeEnum::KeyInputValueRight;
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueNumOK    :
        case ivis::common::KeyTypeEnum::KeyInputValueNumOK2   : {
            inputValue = ivis::common::KeyTypeEnum::KeyInputValueOK;
            break;
        }
        default : {
            break;
        }
    }
#endif

#if defined(USE_SCREEN_CAPTURE)
    if ((inputType == ivis::common::KeyTypeEnum::KeyInputTypeRelease)
        && (inputValue == ivis::common::KeyTypeEnum::KeyInputValueCapture)) {
        ScreenInfo::instance().data()->captureScreen();
    }
#endif

#if defined(USE_KEY_EVENT)
    int displayType = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeMode).toInt();
    // qDebug() << "ControlManager::keyEvent(" << inputType << "," << inputValue << ") :" << displayType;
    if (mControlInfo[displayType]) {
        mControlInfo[displayType]->keyEvent(inputType, inputValue);
    }
#endif
}

void ControlManager::mouseEvent(const int& inputType, const int& inputValue) {
}

void ControlManager::resizeEvent(QResizeEvent& resizeEvent) {
#if defined(USE_RESIZE_SIGNAL)
    setScreenSize(QSize(resizeEvent.size().width(), resizeEvent.size().height()));
    // connect(this, &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
    //     qDebug() << "ControlManager::ScreeSize:" << screenSize;
    // });
#else
    int displayType = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeMode).toInt();
    if (mControlInfo[displayType]) {
        mControlInfo[displayType]->resizeEvent(resizeEvent->size().width(), resizeEvent->size().height());
    }
#endif
}

void ControlManager::createControl(const int& displayType) {
    if (mControlInfo[displayType] == nullptr) {
        switch (displayType) {
            case ivis::common::ScreenEnum::DisplayTypeMenu : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlMenu::instance().data());
                break;
            }
            case ivis::common::ScreenEnum::DisplayTypeCenter : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlCenter::instance().data());
                break;
            }
            case ivis::common::ScreenEnum::DisplayTypeExcel : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlExcel::instance().data());
                break;
            }
            default : {
                qDebug() << "Fail to create control instnace - displayType :" << displayType;
                break;
            }
        }

        if (mControlInfo[displayType]) {
            // qDebug() << "ControlManager::createControl :" << getCurrentMode() << displayType;
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeMode, displayType);
            mControlInfo[displayType]->init(getCurrentMode(), displayType);
#if defined(USE_RESIZE_SIGNAL)
            emit signalScreenSizeChanged(getScreenSize());
#else
            mControlInfo[displayType]->resizeEvent(getScreenSize().width(), getScreenSize().height());
#endif
            setCurrentMode(displayType);
        }
    }
}

void ControlManager::exitProgram(const bool& check) {
    if (check) {
        sendEventInfo(ivis::common::ScreenEnum::DisplayTypeInvalid, ivis::common::ScreenEnum::DisplayTypeMenu,
                                                ivis::common::EventTypeEnum::EventTypeExitProgram, QVariant());
    } else {
        emit signalExitProgram();
    }
}
