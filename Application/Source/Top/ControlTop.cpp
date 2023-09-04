#include "ControlTop.h"
#include "HandlerTop.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"
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
        initCommonData(currentMode, ivis::common::ScreenEnum::DisplayTypeTop);
        initBaseData();
    }
}

void ControlTop::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlTop::initBaseData() {
    resetControl(false);

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType, false);
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
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
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

void ControlTop::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlTop::slotConfigChanged(const int& type, const QVariant& value) {
    // qDebug() << "ControlTop::slotConfigChanged(" << type << "," << value << ")";
    switch (type) {
        case ConfigInfo::ConfigTypeDefaultPath : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, value);
            break;
        }
        // case ConfigInfo::ConfigTypeScreenInfo : {
        //     QRect screenInfo = value.toRect();
        //     QSize screenSize = QSize(screenInfo.width(), screenInfo.height());
        //     updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
        //     break;
        // }
        default : {
            break;
        }
    }
}

void ControlTop::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlTop::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram : {
            ivis::common::PopupButton button = ivis::common::PopupButton::Discard;
            bool fileSaveType = getData(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType).toBool();
            if (fileSaveType) {
                QVariantList text = QVariantList({STRING_POPUP_SAVE_FILE, STRING_POPUP_SAVE_FILE_TIP,
                                                    STRING_POPUP_SAVE, STRING_POPUP_DISCARD, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Exit, isHandler(), popupData, QVariant(text));
                if (button == ivis::common::PopupButton::OK) {
                    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel,
                                    ivis::common::EventTypeEnum::EventTypeFileSave, QVariant());
                    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType, false);
                }
            }

            if (button != ivis::common::PopupButton::Cancel) {
                ControlManager::instance().data()->exitProgram();
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAbout : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::About, isHandler(), popupData,
                                            QVariant(QVariantList({STRING_POPUP_ABOUT, STRING_POPUP_ABOUT_TIP})));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAboutQt : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::AboutQt, isHandler(), popupData);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile :
        case ivis::common::EventTypeEnum::EventTypeFileNew :
        case ivis::common::EventTypeEnum::EventTypeFileOpen : {
#if 1
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
#else
            if (getData(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType).toBool()) {
                QVariant popupData = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Save, isHandler(), popupData)
                    == ivis::common::PopupButton::OK) {
                    slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeFileSave, popupData);
                }
            } else {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            }
#endif
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileSave :
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditCut : {
            qDebug() << "Edit - Cut";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditCopy : {
            qDebug() << "Edit - Copy";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditPaste : {
            qDebug() << "Edit - Paste";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewConfig :
        case ivis::common::EventTypeEnum::EventTypeViewSignal : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingDevPath : {
            QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
            QVariant popupData = QVariant();
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                    QVariantList({STRING_DEFAULT_PATH, defaultPath})) == ivis::common::PopupButton::OK) {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, popupData);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultPath, popupData);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingNodePath : {
            QVariant nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath);
            QVariant popupData = QVariant();
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                    QVariantList({STRING_NODE_PATH, nodeAddressPath})) == ivis::common::PopupButton::OK) {
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeNodeAddressPath, popupData);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingTestReport : {
            qDebug() << "Setting - Test Report";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingTestResult : {
            qDebug() << "Setting - Test Result";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingTestCoverage : {
            qDebug() << "Setting - Test Coverage";
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeReadExcelInfo : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        default : {
            break;
        }
    }
}

void ControlTop::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug() << "ControlTop::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeFileSaveType : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType, eventValue);
            break;
        }
        default : {
            break;
        }
    }
}
