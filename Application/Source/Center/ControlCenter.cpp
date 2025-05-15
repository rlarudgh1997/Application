#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"
#include "ExcelUtil.h"

QSharedPointer<ControlCenter>& ControlCenter::instance() {
    static QSharedPointer<ControlCenter> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlCenter>(new ControlCenter());
    }
    return gControl;
}

ControlCenter::ControlCenter() {
    isHandler();
}

AbstractHandler* ControlCenter::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerCenter::instance().data());
    }
    return mHandler;
}

bool ControlCenter::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlCenter::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
}

void ControlCenter::initNormalData() {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeInvalid);
    updateAllModuleList();
}

void ControlCenter::initControlData() {
}

void ControlCenter::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(getData(ivis::common::PropertyTypeEnum::PropertyTypeMode).toInt(),
                       getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt());
        initNormalData();
        initControlData();
        qDebug() << "\t Reset - ControlCenter";
    }
}

void ControlCenter::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &AbstractHandler::signalHandlerEvent,
                [=](const int& type, const QVariant& value) { slotHandlerEvent(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged,
                [=](const int& type, const QVariant& value) { slotConfigChanged(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigReset,
                [=](const bool& resetAll) { resetControl(resetAll); });
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                [=](const int& displayType, const int& eventType, const QVariant& eventValue) {
                    slotEventInfoChanged(displayType, eventType, eventValue);
                });
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

void ControlCenter::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlCenter::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlCenter::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlCenter::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlCenter::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlCenter::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                     destination, eventType, eventValue);
}

void ControlCenter::updateConfigInfo() {
    QVariantList allConfigData = QVariantList();
    for (int type = (ConfigInfo::ConfigTypeInvalid + 1); type < ConfigInfo::ConfigTypeMaxDoNotSave; type++) {
        QVariant name = ConfigSetting::instance().data()->isConfigName(type);
        QVariant value = ConfigSetting::instance().data()->readConfig(type);
        allConfigData.append(QVariant(QVariantList({type, name, value})));
    }
    QVariantList previousConfig = getData(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfoPrevious, previousConfig);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo, QVariant(allConfigData), true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeConfig);
}

void ControlCenter::updateAllModuleList() {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode);
    QStringList moduleList = moduleInfo.keys();

    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAllModule, moduleList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(moduleList));
}

bool ControlCenter::checkNodeAddress(const QString& vsmPath, const QVariantList& vsmFile) {
    QMap<int, QStringList> vsmInfo = QMap<int, QStringList>();
    bool fileNotFound = (vsmFile.size() == 0);

    for (const auto& file : vsmFile) {
        QFile filePath = QString("%1/%2").arg(vsmPath).arg(file.toString());
        if (filePath.exists() == false) {
            fileNotFound = true;
            break;
        }
    }

    if (fileNotFound) {
        ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
        QVariantList text = QVariantList(
            {STRING_POPUP_FILE_NOT_EXIST, STRING_POPUP_FILE_NOT_EXIST_TIP, STRING_POPUP_CONFIRM, STRING_POPUP_CANCEL});
        QVariant popupData = QVariant();
        button = ivis::common::Popup::drawPopup(ivis::common::PopupType::FileNotExist, isHandler(), popupData, QVariant(text));
        if (button == ivis::common::PopupButton::Confirm) {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath);
        }
    }

    return fileNotFound;
}

int ControlCenter::isVehicleType(const QString& file) {
    QList<QPair<int, QString>> vehicleInfo = {
        {ivis::common::VehicleTypeEnum::VehicleTypeICV, QString("_ICV.")},
        {ivis::common::VehicleTypeEnum::VehicleTypeEV, QString("_EV.")},
        {ivis::common::VehicleTypeEnum::VehicleTypeFCEV, QString("_FCEV.")},
        {ivis::common::VehicleTypeEnum::VehicleTypePHEV, QString("_PHEV.")},
        {ivis::common::VehicleTypeEnum::VehicleTypeHEV, QString("_HEV.")},
    };
    int vehicleType = ivis::common::VehicleTypeEnum::VehicleTypeInvalid;
    for (const auto& info : vehicleInfo) {
        if (file.contains(info.second)) {
            vehicleType = info.first;
            break;
        }
    }
    if (vehicleType == ivis::common::VehicleTypeEnum::VehicleTypeInvalid) {
        qDebug() << "Fail to vehicle type : invalid";
    }
    return vehicleType;
}

QStringList ControlCenter::isNodeAddressAll(const QString& vsmPath, const QVariantList& vsmFile) {
    QStringList vsmList = QStringList();
    QMap<int, QStringList> vsmInfo = QMap<int, QStringList>();

    int vehicleType = ivis::common::VehicleTypeEnum::VehicleTypeInvalid;
    for (const auto& file : vsmFile) {
        QStringList readData = ivis::common::FileInfo::readFile(vsmPath + "/" + file.toString());
        QStringList list = QStringList();
        for (QString lineStr : readData) {
            if ((lineStr.split(".").size() == 2) && (lineStr.trimmed().startsWith("-")) && (lineStr.trimmed().endsWith(":"))) {
                lineStr.remove(" ");
                lineStr.remove("-");
                lineStr.remove(":");
                list.append(lineStr);
                vsmList.append(lineStr);
            }
        }
        vehicleType = isVehicleType(file.toString());
        if (vehicleType != ivis::common::VehicleTypeEnum::VehicleTypeInvalid) {
            vsmInfo[vehicleType].append(list);
        }
    }

    for (int index = 0; index < vsmList.size(); index++) {
        QString vsmSignal = vsmList[index];
        QString vehicleType = QString();
        for (int listIndex = 0; listIndex < vsmInfo.size(); listIndex++) {
            if (vsmInfo[listIndex].contains(vsmSignal)) {
                if (vehicleType.size() > 0) {
                    vehicleType.append(", ");
                }

                if (listIndex == static_cast<int>(ivis::common::VehicleTypeEnum::VehicleTypeICV)) {
                    vehicleType.append("ICV");
                } else if (listIndex == static_cast<int>(ivis::common::VehicleTypeEnum::VehicleTypeEV)) {
                    vehicleType.append("EV");
                } else if (listIndex == static_cast<int>(ivis::common::VehicleTypeEnum::VehicleTypeFCEV)) {
                    vehicleType.append("FCEV");
                } else if (listIndex == static_cast<int>(ivis::common::VehicleTypeEnum::VehicleTypePHEV)) {
                    vehicleType.append("PHEV");
                } else if (listIndex == static_cast<int>(ivis::common::VehicleTypeEnum::VehicleTypeHEV)) {
                    vehicleType.append("HEV");
                } else {
                    vehicleType.append("");
                    qDebug() << "Fail to append vehicle type : null";
                }
            }
        }
        vsmList[index] = QString("%1\t%2").arg(vsmSignal).arg(vehicleType);
    }

    vsmList.sort();  // qSort(vsmList);
    vsmList.removeDuplicates();

    return vsmList;
}

QStringList ControlCenter::isNodeAddressMatchingModule(const QStringList& vsmList) {
    QStringList vsmMatchingList = vsmList;
    QVariantList selectModule = (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toList());

    qDebug() << "MatchingModule Count :" << vsmList.size() << selectModule.size();
    if (selectModule.size() > 0) {
        QStringList vsmTemp = QStringList();
        for (const auto& moudleName : selectModule) {
            for (const auto& vsmInfo : vsmList) {
                if (vsmInfo.contains(moudleName.toString())) {
                    vsmTemp.append(vsmInfo);
                }
            }
        }
        vsmMatchingList = vsmTemp;
    }

    return vsmMatchingList;
}

void ControlCenter::updateNodeAddress(const bool& check) {
    QString vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    vsmPath.append("/VSM");
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString vsmBase = QString();
    QStringList vehicleTypeList = QStringList();
    QStringList vsmSpecList = QStringList();

    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        vsmBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBasePV).toString();
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toStringList();
        vsmSpecList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmSpecTypePV).toStringList();
    } else {
        vsmBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBaseCV).toString();
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toStringList();
        vsmSpecList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmSpecTypeCV).toStringList();
    }

    QVariantList vsmFile = QVariantList();
    for (const auto& vehicle : vehicleTypeList) {
        for (const auto& spec : vsmSpecList) {
            QString fileName = QString(vsmBase).arg(vehicle).arg(spec);
            vsmFile.append(fileName);
            // qDebug() << "VSM File :" << fileName;
        }
    }

    if ((check) && (checkNodeAddress(vsmPath, vsmFile))) {
        qDebug() << "Fail to vsm file not found :" << vsmPath;
        return;
    }

    QStringList vsmListAll = isNodeAddressAll(vsmPath, vsmFile);
    QStringList vsmList = isNodeAddressMatchingModule(vsmListAll);

    // qDebug() << "VSM List Count :" << vsmListAll.size() << vsmList.size();
    updateSelectModuleList(false);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll, QVariant(vsmList), true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeNode);
}

void ControlCenter::updateSelectModuleList(const bool& show) {
    QStringList moduleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toStringList();

    // qDebug() << "\t moduleList :" << moduleList.size();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(moduleList));
    if (show) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule, true, true);
    }
}

void ControlCenter::updateSelectModueNodeAddress(const bool& update, const QVariantList& selectModule) {
    if (selectModule.size() > 0) {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, selectModule);
    }

    // qDebug() << "updateSelectModueNodeAddress :" << update << selectModule;
    if (update) {
        updateNodeAddress(false);
    } else {
        updateSelectModuleList(false);
    }
}

void ControlCenter::controlProcess(const int& type, const QString& command) {
    static bool checkPWD = false;
    switch (type) {
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Start): {
            if (mProcess.isNull()) {
                mProcess = QSharedPointer<QProcess>(new QProcess(this), &QObject::deleteLater);
            }
            mProcess.data()->start("/bin/bash");  // "/usr/bin/zsh" ... 원하는 쉘 입력

            connect(mProcess.data(), &QProcess::readyReadStandardOutput, [&]() {
                QByteArray readData = mProcess.data()->readAllStandardOutput();
                QString info = QString::fromUtf8(readData);

                if (checkPWD) {
                    checkPWD = false;
                    // qDebug() << "Path :" << info;
                    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo, info);
                } else {
                    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo, info, true);
                }
            });
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input): {
            if (mProcess.isNull() == false) {
                bool normalCommand = (command.trimmed().startsWith("pwd") == false);
                if (normalCommand) {
                    mProcess.data()->write((command + "\n").toUtf8());
                }
                QTimer::singleShot(50, this, [=]() {
                    checkPWD = (mProcess.data()->write("pwd\n"));
                });
            }
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Clear): {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo, QString());
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Stop): {
            if (mProcess.isNull() == false) {
                qDebug() << "Process is running -> stop";
                disconnect(mProcess.data());
                if (mProcess->state() != QProcess::NotRunning) {
                    mProcess->terminate();                          // 종료 요청
                    if (!mProcess->waitForFinished(3000)) {         // 최대 3초 대기
                        mProcess->kill();                           // 강제 종료
                        mProcess->waitForFinished();
                    }
                }
                mProcess.reset();                                   // 안전하게 제거
            }
            break;
        }
        default: {
            break;
        }
    }
}

void ControlCenter::updateTerminalMode() {
    controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Start), QString());
    controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), QString("pwd"));

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeTerminal);
}

void ControlCenter::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlCenter::slotConfigChanged(const int& type, const QVariant& value) {
    int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
    switch (type) {
        case ConfigInfo::ConfigTypeInit:
        case ConfigInfo::ConfigTypeSfcModelPath:
        case ConfigInfo::ConfigTypeScreenInfo: {
            if (viewType == ivis::common::ViewTypeEnum::CenterViewTypeConfig) {
                updateConfigInfo();
            }
            if (type == ConfigInfo::ConfigTypeScreenInfo) {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                                  ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
            }
            break;
        }
        default: {
            break;
        }
    }
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlCenter::slotHandlerEvent() ->" << type << "," << value;

    int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, ivis::common::EventTypeEnum::EventTypeViewInfoClose, "");
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
            updateDataHandler(
                ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeInvalid);
            if (viewType == ivis::common::ViewTypeEnum::CenterViewTypeTerminal) {
                controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Stop), QString());
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateConfig: {
            QVariantList configInfo = value.toList();
            if (configInfo.size() == 2) {
                int configType = configInfo.at(0).toInt();
                QVariant configValue = configInfo.at(1);
                ConfigSetting::instance().data()->editConfig(configType, configValue);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeConfigReset: {
            if (viewType == ivis::common::ViewTypeEnum::CenterViewTypeConfig) {
                ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
                QVariantList text = QVariantList(
                    {STRING_POPUP_CONFIG_RESET, STRING_POPUP_CONFIG_RESET_TIP, STRING_POPUP_CONFIRM, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::RestConfigValue, isHandler(), popupData,
                                                   QVariant(text)) == ivis::common::PopupButton::Confirm) {
                    ConfigSetting::instance().data()->resetConfig(ConfigSetting::ConfigResetTypeNormal);
                }
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeShowModule: {
            updateSelectModuleList(true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModule: {
            updateSelectModueNodeAddress(true, value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTerminalCommand: {
            QString command = value.toString();
            if (command.size() > 0) {
                controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), command);
            } else {
                controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Clear), QString());
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleError: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(
                ivis::common::PopupType::ModuleSelectError, isHandler(), popupData,
                QVariantList({STRING_POPUP_MODULE_SELECT_ERROR, STRING_POPUP_MODULE_SELECT_ERROR_TIP}));
            break;
        }
        default: {
            break;
        }
    }
}

void ControlCenter::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & displayType) == false) {
        return;
    }

    // qDebug() << "ControlCenter::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewConfig: {
            updateConfigInfo();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress: {
            updateNodeAddress(true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeInitModule: {
            updateAllModuleList();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModule: {
            updateSelectModueNodeAddress(false, eventValue.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTerminalMode: {
            updateTerminalMode();
            break;
        }
        default: {
            break;
        }
    }
}
