#include "ControlManager.h"

#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlHome.h"
#include "ControlTelltale.h"


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
    setCurrentMode(ivis::common::DisplayEnum::DisplayTypeHome);
    createControl(ivis::common::DisplayEnum::DisplayTypeHome);
    createControl(ivis::common::DisplayEnum::DisplayTypeTelltale);
}

void ControlManager::sendEventInfo(const int& source, const int& destination, const int& eventType, const QVariant& eventValue) {
    qDebug() << "[sendEventInfo] Direction :" << source << " -> " << destination << ", Type :" << eventType;
    if (destination != ivis::common::DisplayEnum::DisplayTypeInvalid) {
        createControl(destination);
        emit signalEventInfoChanged(destination, eventType, eventValue);
    }

    if (source == ivis::common::DisplayEnum::DisplayTypeHome) {
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
        case ivis::common::KeyEnum::KeyInputValueNumUp    : {
            inputValue = ivis::common::KeyEnum::KeyInputValueUp;
            break;
        }
        case ivis::common::KeyEnum::KeyInputValueNumDown  : {
            inputValue = ivis::common::KeyEnum::KeyInputValueDown;
            break;
        }
        case ivis::common::KeyEnum::KeyInputValueNumLeft  : {
            inputValue = ivis::common::KeyEnum::KeyInputValueLeft;
            break;
        }
        case ivis::common::KeyEnum::KeyInputValueNumRight : {
            inputValue = ivis::common::KeyEnum::KeyInputValueRight;
            break;
        }
        case ivis::common::KeyEnum::KeyInputValueNumOK    :
        case ivis::common::KeyEnum::KeyInputValueNumOK2   : {
            inputValue = ivis::common::KeyEnum::KeyInputValueOK;
            break;
        }
        default : {
            break;
        }
    }
#endif

#if defined(USE_SCREEN_CAPTURE)
    if ((inputType == ivis::common::KeyEnum::KeyInputTypeRelease)
        && (inputValue == ivis::common::KeyEnum::KeyInputValueCapture)) {
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
            case ivis::common::DisplayEnum::DisplayTypeHome : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlHome::instance().data());
                break;
            }
            case ivis::common::DisplayEnum::DisplayTypeTelltale : {
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlTelltale::instance().data());
                break;
            }
            default : {
                qDebug() << "Fail to create control instnace - displayType :" << displayType;
                break;
            }
        }

        if (mControlInfo[displayType]) {
            // qDebug() << "ControlManager::createControl :" << displayType;
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeMode, displayType);
            mControlInfo[displayType]->init(displayType);
#if defined(USE_RESIZE_SIGNAL)
//            emit signalScreenSizeChanged(getScreenSize());
#else
            mControlInfo[displayType]->resizeEvent(getScreenSize().width(), getScreenSize().height());
#endif
            setCurrentMode(displayType);
        }
    }
}

void ControlManager::exitProgram() {
    emit signalExitProgram();
}