#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"

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
}

void ControlCenter::initNormalData() {
    resetControl(false);

    QStringList sfcModules = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toStringList();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(sfcModules));
}

void ControlCenter::initControlData() {
}

void ControlCenter::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlCenter::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerCenter::signalHandlerEvent, this, &ControlCenter::slotHandlerEvent, Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlCenter::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlCenter::slotEventInfoChanged, Qt::UniqueConnection);
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
    for (int type = (ConfigInfo::ConfigTypeInvalid + 1); type < ConfigInfo::ConfigTypeReportResult; type++) {
        QVariant name = ConfigSetting::instance().data()->isConfigName(type);
        QVariant value = ConfigSetting::instance().data()->readConfig(type);
        allConfigData.append(QVariant(QVariantList({type, name, value})));
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::ViewTypeConfig);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo, QVariant(allConfigData), true);
}

void ControlCenter::updateTestReport() {
    for (int index = ivis::common::ReportTypeEnum::ReportTypeResult; index < ivis::common::ReportTypeEnum::ReportTypeAll;
         index++) {
        int configStart = ConfigInfo::ConfigTypeReportResult;
        int configEnd = ConfigInfo::ConfigTypeReportResultExcel;
        int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportResultInfo;

        if (index == ivis::common::ReportTypeEnum::ReportTypeCoverage) {
            configStart = ConfigInfo::ConfigTypeReportCoverage;
            configEnd = ConfigInfo::ConfigTypeReportCoverageBranch;
            propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportCoverageInfo;
        }

        QVariantList reportData = QVariantList();
        for (int configType = configStart; configType <= configEnd; configType++) {
            QVariant configValue = ConfigSetting::instance().data()->readConfig(configType);
            reportData.append(QVariant(QVariantList({configType, configValue})));
            // qDebug() << "readConfig :" << configType << configValue;
        }
        updateDataHandler(propertyType, QVariant(reportData));
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::ViewTypeReport);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestReportType, true, true);
}

bool ControlCenter::checkNodeAddress(const QVariant& vsmPath, const QVariantList& vsmFile) {
    QMap<int, QStringList> vsmInfo = QMap<int, QStringList>();
    bool fileNotFound = (vsmFile.size() == 0);

    foreach (const auto& file, vsmFile) {
        QFile filePath = QString("%1/%2").arg(vsmPath.toString()).arg(file.toString());
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
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeSettingVsmPath);
        }
    }

    return fileNotFound;
}

QStringList ControlCenter::isNodeAddressAll(const QVariant& vsmPath, const QVariantList& vsmFile) {
    QStringList vsmList = QStringList();
    QMap<int, QStringList> vsmInfo = QMap<int, QStringList>();

    foreach (const auto& file, vsmFile) {
        QStringList readData = ivis::common::FileInfo::readFile(vsmPath.toString() + "/" + file.toString());
        QStringList list = QStringList();
        foreach (QString lineStr, readData) {
            if ((lineStr.split(".").size() == 2) && (lineStr.contains("-")) && (lineStr.contains(":"))) {
                lineStr.remove(" ");
                lineStr.remove("-");
                lineStr.remove(":");
                list.append(lineStr);
                vsmList.append(lineStr);
            }
        }
        vsmInfo[vsmInfo.size()].append(list);
    }

    for (int index = 0; index < vsmList.size(); index++) {
        QString vsmSignal = vsmList[index];
        QString vehicleType = QString();
        for (int listIndex = 0; listIndex < vsmInfo.size(); listIndex++) {
            if (vsmInfo[listIndex].contains(vsmSignal)) {
                if (vehicleType.size() > 0) {
                    vehicleType.append(", ");
                }

                if (listIndex == static_cast<int>(ivis::common::VsmTypeEnum::VsmTypeEV)) {
                    vehicleType.append("EV");
                } else if (listIndex == static_cast<int>(ivis::common::VsmTypeEnum::VsmTypeFCEV)) {
                    vehicleType.append("FCEV");
                } else {
                    vehicleType.append("ICV");
                }
            }
        }
        vsmList[index] = QString("%1\t%2").arg(vsmSignal).arg(vehicleType);
    }
    vsmList.sort();  // qSort(vsmList);

    return vsmList;
}

QStringList ControlCenter::isNodeAddressMatchingModule(const QStringList& vsmList) {
    QStringList vsmMatchingList = vsmList;
    QVariantList selectModule = (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toList());

    if (selectModule.size() > 0) {
        QStringList vsmTemp = QStringList();
        foreach (const auto& moudleName, selectModule) {
            foreach (const auto& vsmInfo, vsmList) {
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
    QVariant vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmPath);
    QVariantList vsmFile = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmNodeAddress).toList();

    if ((check) && (checkNodeAddress(vsmPath, vsmFile))) {
        qDebug() << "Fail to vsm file not found.";
        return;
    }

    QStringList vsmListAll = isNodeAddressAll(vsmPath, vsmFile);
    QStringList vsmList = isNodeAddressMatchingModule(vsmListAll);

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewType, ivis::common::ViewTypeEnum::ViewTypeNodeAddress);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll, QVariant(vsmList), true);

    updateSelectModueList(false);
}

void ControlCenter::updateSelectModueList(const bool& show) {
    QStringList moduleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toStringList();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(moduleList));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule, show, true);
}

void ControlCenter::updateSelectModueNodeAddress(const bool& update, const QVariantList& selectModule) {
    if (selectModule.size() > 0) {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, selectModule);
    }

    if (update) {
        updateNodeAddress(false);
    } else {
        updateSelectModueList(false);
    }
}

void ControlCenter::slotConfigChanged(const int& type, const QVariant& value) {
    int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();

    switch (viewType) {
        case ivis::common::ViewTypeEnum::ViewTypeConfig: {
            if ((type == ConfigInfo::ConfigTypeInit) || (type == ConfigInfo::ConfigTypeDefaultPath)) {
                updateConfigInfo();
            }
            break;
        }
        case ivis::common::ViewTypeEnum::ViewTypeReport: {
            updateTestReport();
            break;
        }
        default: {
        }
    }
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlCenter::slotHandlerEvent() ->" << type << "," << value;
    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, ivis::common::EventTypeEnum::EventTypeViewInfoClose, "");
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
            int viewType = getData(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
            if (viewType == ivis::common::ViewTypeEnum::ViewTypeConfig) {
                ConfigSetting::instance().data()->resetConfig(ConfigSetting::ConfigResetTypeNormal);
            } else if (viewType == ivis::common::ViewTypeEnum::ViewTypeReport) {
                ConfigSetting::instance().data()->resetConfig(ConfigSetting::ConfigResetTypeReport);
            } else {
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTestReportReset: {
            updateTestReport();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeShowModule: {
            updateSelectModueList(true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModule: {
            updateSelectModueNodeAddress(true, value.toList());
            break;
        }
        default: {
            break;
        }
    }
}

void ControlCenter::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlCenter::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewConfig: {
            updateConfigInfo();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress: {
            updateNodeAddress(true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModule: {
            updateSelectModueNodeAddress(false, eventValue.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingTestReport:
        case ivis::common::EventTypeEnum::EventTypeReportResult:
        case ivis::common::EventTypeEnum::EventTypeReportCoverage: {
            updateTestReport();
            break;
        }
        default: {
            break;
        }
    }
}
