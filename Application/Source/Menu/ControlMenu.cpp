#include "ControlMenu.h"
#include "HandlerMenu.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"
#include "CommonPopup.h"

// Q_LOGGING_CATEGORY(C_TOP, "ControlMenu")


QSharedPointer<ControlMenu>& ControlMenu::instance() {
    static QSharedPointer<ControlMenu> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlMenu>(new ControlMenu());
    }
    return gControl;
}

ControlMenu::ControlMenu() {
    isHandler();
}

AbstractHandler* ControlMenu::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerMenu::instance().data());
    }
    return mHandler;
}

bool ControlMenu::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlMenu::initCommonData(const int& currentMode) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, ivis::common::ScreenEnum::DisplayTypeMenu);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlMenu::initNormalData() {
    resetControl(false);

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);
}

void ControlMenu::initControlData() {
}

void ControlMenu::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMenu::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerMenu::signalHandlerEvent,
                this,                              &ControlMenu::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlMenu::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlMenu::slotEventInfoChanged,
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

void ControlMenu::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlMenu::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlMenu::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlMenu::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlMenu::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlMenu::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlMenu::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeDefaultPath : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, value);
            break;
        }
        default : {
            break;
        }
    }
}

void ControlMenu::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlMenu::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram : {
            ivis::common::PopupButton button = ivis::common::PopupButton::Discard;
            bool fileSave = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();

            if (fileSave) {
                QVariantList text = QVariantList({STRING_POPUP_SAVE_FILE, STRING_POPUP_SAVE_FILE_TIP,
                                                    STRING_POPUP_SAVE, STRING_POPUP_DISCARD, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Exit, isHandler(), popupData, QVariant(text));
                if (button == ivis::common::PopupButton::OK) {
                    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel,
                                    ivis::common::EventTypeEnum::EventTypeFileSave, QVariant());
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                }
            }

            if (button != ivis::common::PopupButton::Cancel) {
                ControlManager::instance().data()->exitProgram(false);
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
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
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
        case ivis::common::EventTypeEnum::EventTypeEditCut :
        case ivis::common::EventTypeEnum::EventTypeEditCopy :
        case ivis::common::EventTypeEnum::EventTypeEditPaste : {
            qDebug() << "Edit :" << value;
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewConfig :
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress :
        case ivis::common::EventTypeEnum::EventTypeSettingTestReport :
        case ivis::common::EventTypeEnum::EventTypeReportResult :
        case ivis::common::EventTypeEnum::EventTypeReportCoverage : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingDevPath :
        case ivis::common::EventTypeEnum::EventTypeSettingNodePath :
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath : {
            QString text = STRING_DEFAULT_PATH;
            int configType = ConfigInfo::ConfigTypeDefaultPath;

            if (type == ivis::common::EventTypeEnum::EventTypeSettingNodePath) {
                text = STRING_NODE_PATH;
                configType = ConfigInfo::ConfigTypeNodeAddressPath;
            } else if (type == ivis::common::EventTypeEnum::EventTypeSettingVsmPath) {
                text = STRING_VSM_PATH;
                configType = ConfigInfo::ConfigTypeVsmPath;
            } else {
                // ivis::common::EventTypeEnum::EventTypeSettingDevPath
            }

            QVariant path = ConfigSetting::instance().data()->readConfig(configType);
            QVariant popupData = QVariant();

            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                    QVariantList({text, path})) == ivis::common::PopupButton::OK) {
                ConfigSetting::instance().data()->writeConfig(configType, popupData);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenerateTC :
        case ivis::common::EventTypeEnum::EventTypeRunTC :
        case ivis::common::EventTypeEnum::EventTypeGenerateReport : {
            qDebug() << "TC - Menu Select";
            break;
        }

        default : {
            break;
        }
    }
}

void ControlMenu::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlMenu::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram :
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath : {
            slotHandlerEvent(eventType, eventValue);
            break;
        }
        default : {
            break;
        }
    }
}
