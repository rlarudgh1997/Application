#include "ControlManager.h"

#include "CommonEnum.h"

#include "ScreenInfo.h"
#include "ControlMain.h"


QSharedPointer<ControlManager> ControlManager::instance() {
    static QSharedPointer<ControlManager> gControl;

    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlManager>(new ControlManager());
    }

    return gControl;
}

ControlManager::ControlManager() {
    createControl(ScreenEnum::DisplayTypeMain);
}

void ControlManager::keyInput(const int& inputType, const int& inputValue) {
    qDebug() << "ControlManager::keyInput : " << inputType << ", " << inputValue;

#if defined(USE_SCREEN_CAPTURE)
    if ((inputType == KeyTypeEnum::KeyInputTypeRelease) && (inputValue == KeyTypeEnum::KeyInputValueCapture)) {
        ScreenInfo::instance().data()->captureScreen();
    }
#endif

#if defined(USE_KEY_EVENT)
    // getCurrentMode();
    if (mControlInfo[ScreenEnum::DisplayTypeMain]) {
        mControlInfo[ScreenEnum::DisplayTypeMain]->keyInput(inputType, inputValue);
    }
#endif
}

void ControlManager::mouseInput(const int& inputType, const int& inputValue) {
}

void ControlManager::createControl(const int& displayType) {
    if (mControlInfo[displayType]) {
        // nothing to do
    } else {
        switch (displayType) {
            case ScreenEnum::DisplayTypeMain : {
                ControlMain::instance().data()->init(displayType);
                mControlInfo[displayType] = static_cast<AbstractControl*>(ControlMain::instance().data());
                break;
            }
            default : {
                break;
            }
        }
    }
}

