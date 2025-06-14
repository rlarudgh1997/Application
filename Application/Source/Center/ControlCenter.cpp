#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"
#include "ExcelUtil.h"

#include <QRegularExpression>
#include <signal.h>

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

ControlCenter::~ControlCenter() {
    controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Exit), QString());
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
    updateDataHandler(
        ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeConfig, true);
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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeNode, true);
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
    switch (type) {
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Start): {
            if (mProcess.isNull()) {
                mProcess = QSharedPointer<QProcess>(new QProcess(this), &QObject::deleteLater);
                mProcess.data()->setProcessChannelMode(QProcess::MergedChannels);
                mProcess.data()->start("/bin/bash", QStringList() << "-i" << "-l");
                mProcess.data()->write("export PS1=\"\\u@\\h:\\w$ \"\n");   // 경로 정보에서 git 정보 표시 하지 않도록 설정
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo, QString());
            }
            mProcess.data()->write((command + "\n").toUtf8());

            connect(mProcess.data(), &QProcess::readyReadStandardOutput, [&]() {
                QByteArray readData = mProcess.data()->readAllStandardOutput();
                QString info = QString::fromUtf8(readData);
                info.remove(QRegularExpression("\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])"));

                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo, info, true);

                // 마지막 줄만 추출
                QString pathInfoLine = info.section('\n', -1);
                QRegularExpression regex(R"(:~(/[^$]*)\$)");
                QRegularExpressionMatch match = regex.match(pathInfoLine);

                if (match.hasMatch()) {
                    QString relativePath = match.captured(1);
                    QString absolutePath;
                    if (relativePath.size() > 0) {
                        absolutePath = QString("%1%2/").arg(QDir::homePath()).arg(relativePath);
                    }
                    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo, absolutePath);
                }
            });
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input): {
            if (mProcess.isNull() == false) {
                mProcess.data()->write((command + "\n").toUtf8());
            }
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Clear): {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo, QString());
            break;
        }
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Stop):
        case static_cast<int>(ivis::common::ProcessEnum::CommonadType::Exit): {
            if (mProcess.isNull() == false) {
                if (type == static_cast<int>(ivis::common::ProcessEnum::CommonadType::Exit)) {
                    disconnect(mProcess.data());
                    controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), QString("exit"));
                    if (mProcess->state() != QProcess::NotRunning) {
                        mProcess->terminate();
                        if (mProcess->waitForFinished(3000) == false) {
                            mProcess->kill();
                            mProcess->waitForFinished();
                        }
                    }
                    mProcess.reset();
                } else {
                    // if (mProcess->state() != QProcess::NotRunning) {
                    //     mProcess->kill();
                    //     // mProcess->terminate();
                    // }
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}

void ControlCenter::updateTerminalMode(const int& dockerCount) {
    int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
    bool terminalType = getData(ivis::common::PropertyTypeEnum::PropertyTypeTerminalType).toBool();
    bool multiDocker = (dockerCount >= 0);

    if ((viewType == ivis::common::ViewTypeEnum::CenterViewTypeTerminal) && (multiDocker == terminalType)) {
        // qDebug() << "\t Skip - updateTerminalMode :" << multiDocker << terminalType;
        return;
    }

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QString workingDir = QDir(QString("%1/../validator/multi-docker-test").arg(defaultPath)).absolutePath();
    QString command;

    if (multiDocker) {
        if (terminalType == false) {
            command = QString("cd %1").arg(workingDir);
            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Start), command);

            QString option = (dockerCount == 0) ? (QString()) : (QString(" -d %1").arg(dockerCount));
            command = QString("./multi-docker-test.sh -t CV%1").arg(option);

            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), command);
        }
    } else {
        if (terminalType) {
            multiDocker = true;
        } else {
            workingDir = ivis::common::APP_PWD();
            command = QString("cd %1").arg(workingDir);
            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Start), command);
        }
    }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo, QString());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalType, multiDocker);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalBufferSize,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTerminalBufferSize));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::CenterViewTypeTerminal,
                      true);

    if (workingDir.size() > 0) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo, workingDir, true);
    }
}

void ControlCenter::updateTerminalModeInfo(const bool& multiDocker) {
    QVariant popupData;
    QVariantList textInfo;

    if (multiDocker) {
        textInfo = QVariantList({STRING_POPUP_STOP_MULTI_DCKER, STRING_POPUP_STOP_MULTI_DCKER_TIP, STRING_POPUP_CONFIRM,
                                 STRING_POPUP_CANCEL});
        if (ivis::common::Popup::drawPopup(ivis::common::PopupType::StopMutliDocker, isHandler(), popupData, textInfo)
                                           == ivis::common::PopupButton::Confirm) {
            QString command("./remove-docker-containers.sh");
            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), command);
            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Exit), QString());

            slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeViewInfoClose, QVariant());

            textInfo = QVariantList({STRING_POPUP_STOP_MULTI_DCKER, STRING_POPUP_STOP_MULTI_DCKER_TIP2});
            ivis::common::Popup::drawPopup(ivis::common::PopupType::StopMutliDockerCompleted, isHandler(), popupData, textInfo);
        }

    } else {
        QVariant path = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo);
        textInfo = QVariantList({STRING_TERMINAL_PATH, path});
        if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData, textInfo)
                                           == ivis::common::PopupButton::OK) {
            QString command = QString("cd %1").arg(popupData.toString());
            controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Input), command);
        }
    }
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
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTerminalType, false);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType,
                              ivis::common::ViewTypeEnum::CenterViewTypeInvalid);
            if (viewType == ivis::common::ViewTypeEnum::CenterViewTypeTerminal) {
                controlProcess(static_cast<int>(ivis::common::ProcessEnum::CommonadType::Exit), QString());
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
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::ResetConfigValue, isHandler(), popupData,
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
        case ivis::common::EventTypeEnum::EventTypeTerminalInfo: {
            const bool multiDocker = getData(ivis::common::PropertyTypeEnum::PropertyTypeTerminalType).toBool();
            updateTerminalModeInfo(multiDocker);
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
            updateTerminalMode(-1);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeStartMultiDocker: {
            updateTerminalMode(eventValue.toInt());
            break;
        }
        default: {
            break;
        }
    }
}
