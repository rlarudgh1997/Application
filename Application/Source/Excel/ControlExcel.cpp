#include "ControlExcel.h"
#include "HandlerExcel.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ExcelDataManager.h"
#include "SignalDataManager.h"
#include "TestCase.h"

QSharedPointer<ControlExcel>& ControlExcel::instance() {
    static QSharedPointer<ControlExcel> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlExcel>(new ControlExcel());
    }
    return gControl;
}

ControlExcel::ControlExcel() {
    isHandler();
}

AbstractHandler* ControlExcel::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerExcel::instance().data());
    }
    return mHandler;
}

bool ControlExcel::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlExcel::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, false);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
}

void ControlExcel::initNormalData() {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelView, ivis::common::ViewTypeEnum::ExcelTypeInvalid);

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd));

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTypeList));

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeManualCycleMode,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeManualCycleMode));

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QStringList vehicleTypeList = QStringList();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toStringList();
    } else {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toStringList();
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType, vehicleTypeList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAppMode, appMode);
    updateNodeAddress();

    QVariantList keywordTypeInfo;
    for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        QStringList keywordString = QStringList({STRING_SELECT_KEYWORD});
        for (const auto& infoPair : ExcelUtil::instance().data()->isKeywordPatternInfo(index)) {
            keywordString.append(infoPair.first);
        }
        keywordTypeInfo.append(keywordString);
    }
    // updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUseOnlyKeywordInfo, keywordTypeInfo);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo, keywordTypeInfo);
}

void ControlExcel::initControlData() {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString defaultFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    defaultFilePath.append((appMode == ivis::common::AppModeEnum::AppModeTypePV) ? ("/SFC") : ("/SFC/CV"));
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath, defaultFilePath);

    QVariant lastSavedFile = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, lastSavedFile);

#if 0
    // Construct : SFC, Vehicle SignalName
    auto sfcSignal = SignalDataManager::instance().data()->isSignalListInfo(true);
    auto vehiclSignal = SignalDataManager::instance().data()->isSignalListInfo(false);
#endif
}

void ControlExcel::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(getData(ivis::common::PropertyTypeEnum::PropertyTypeMode).toInt(),
                       getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt());
        initNormalData();
        initControlData();
        qDebug() << "\t Reset - ControlExcel";
    }
}

void ControlExcel::controlConnect(const bool& state) {
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
        connect(TestCase::instance().data(), &TestCase::signalTestCaseCompleted, [=](const int& type, const bool& sheetReload) {
            // TestCase 가 별도 쓰레드로 분리되어 동작함.
            QMetaObject::invokeMethod(this, "slotTestCaseCompleted", Q_ARG(int, type), Q_ARG(bool, sheetReload));
        });
        connect(TestCase::instance().data(), &TestCase::signalGenTCInfo,
            [=](const int& resultType, const int& current, const int& total, const QStringList& info) {
            // TestCase 가 별도 쓰레드로 분리되어 동작함.
            QMetaObject::invokeMethod(this, "slotGenTCInfo", Q_ARG(int, resultType), Q_ARG(int, current), Q_ARG(int, total),
                                     Q_ARG(QStringList, info));
        });
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
        disconnect(TestCase::instance().data());
    }
}

void ControlExcel::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlExcel::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)

    if (ControlManager::instance()->getCurrentMode() != ivis::common::ScreenEnum::DisplayTypeExcel) {
        // Used Terminla Mode : Ctrl+C, Ctrl+ ..
        return;
    }

    if (inputType == ivis::common::KeyTypeEnum::KeyInputTypePress) {
        bool keySkip = (inputValue == ivis::common::KeyTypeEnum::KeyInputValueCtrl);
        updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip, keySkip);
    } else if (inputType == ivis::common::KeyTypeEnum::KeyInputTypeRelease) {
        if (getData(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip).toBool()) {
            return;
        }

        if (inputValue == ivis::common::KeyTypeEnum::KeyInputValueCtrl) {
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip, false);
        } else if (inputValue == Qt::Key::Key_Z) {
            // updateShortcutInfo(ivis::common::EventTypeEnum::EventTypeEditUndo);
        } else if (inputValue == Qt::Key::Key_Y) {
            // updateShortcutInfo(ivis::common::EventTypeEnum::EventTypeEditRedo);
        } else {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKey, inputValue, true);
        }
    }
}

void ControlExcel::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
#endif
}

void ControlExcel::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlExcel::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlExcel::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                     destination, eventType, eventValue);
}

void ControlExcel::updateSheetData(const int& propertyType, const QVariantList& sheetData, const bool& edit) {
    if (getData(propertyType).toList() == sheetData) {
        // qDebug() << "There are no changes to the data :" << propertyType << sheetData.size();
        return;
    }

    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    bool reconstructDescriptio = false;
    QList<int> mergeColumnIndex;

    switch (propertyType) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription: {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescConfigVisible).toBool() == false) {
                reconstructDescriptio = edit;
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs: {
            mergeColumnIndex = QList<int>({
                static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName),
                static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal),
            });
            break;
        }
        default: {
            mergeColumnIndex = QList<int>({
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
            });
            break;
        }
    }

    QVariantList originSheetData;
    QMap<int, QString> titleText;
    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        int rowIndex = 0;

        if (reconstructDescriptio) {
            rowData.resize(static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max));

            int columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal);
            rowData[columnIndex] = ExcelUtil::instance().data()->isCurrentCellText(propertyType, rowIndex, columnIndex);
            columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data);
            rowData[columnIndex] = ExcelUtil::instance().data()->isCurrentCellText(propertyType, rowIndex, columnIndex);
        } else {
            for (const auto& columnIndex : mergeColumnIndex) {
                if (columnIndex >= rowData.size()) {
                    continue;
                }
                QString readColumnText = rowData.at(static_cast<int>(columnIndex));
                if (ivis::common::isContainsString(readColumnText, mergeStart)) {
                    ivis::common::getRemoved(readColumnText, mergeStart);
                    titleText[columnIndex] = readColumnText;
                    // qDebug() << columnIndex << ". Start :" << readColumnText;
                } else if (ivis::common::isContainsString(readColumnText, mergeEnd)) {
                    ivis::common::getRemoved(readColumnText, mergeEnd);
                    if (readColumnText.size() == 0) {
                        rowData[static_cast<int>(columnIndex)] = QString("%1%2").arg(mergeEnd).arg(titleText[columnIndex]);
                    }
                    // qDebug() << columnIndex << ". End   :" << readColumnText;
                    titleText[columnIndex].clear();
                } else {
                }
            }
        }
        rowIndex++;
        originSheetData.append(rowData);
    }

    updateDataHandler(propertyType, originSheetData);
    updateSheetDataToExcelData(propertyType, originSheetData);
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);

    int originSize = ExcelData::instance().data()->getSheetData(propertyType).toList().size();
    qDebug() << "\033[32m";
    qDebug() << (QString(100, '>').toLatin1().data());
    qDebug() << "updateSheetData[" << propertyType << "] : data changed !!! -> Size :" << originSize;
#if 0
    int rowIndex = 0;
    for (const auto& rowDataList : originSheetData.toList()) {
        qDebug() << "\t Data[" << rowIndex++ << "] :" << rowDataList.toStringList();
    }
    qDebug() << (QString(100, '<').toLatin1().data());
#endif
    qDebug() << "\033[0m";
}

void ControlExcel::updateSheetDataToExcelData(const int& propertyType, const QVariantList& sheetData) {
    bool updateAll = ((propertyType == 0) && (sheetData.toList().size() == 0));
    if (updateAll) {
        const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
        const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
        for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
            ExcelData::instance().data()->setSheetData(sheetIndex, getData(sheetIndex).toList());
        }
    } else {
        ExcelData::instance().data()->setSheetData(propertyType, sheetData);
    }
}

void ControlExcel::updateExcelSheet(const QList<QVariantList>& openSheetData) {
    auto sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    auto descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    auto configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle).toStringList();
    auto dependentOnTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDependentOnTitle).toStringList();
    auto manualTCTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeManualTCTitle).toStringList();
    auto otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    ivis::common::CheckTimer checkTimer;
    QList<QVariantList> sheetDataList = openSheetData;
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    int excelView = ivis::common::ViewTypeEnum::ExcelTypeInvalid;

    QStringList tcNameList;
    QStringList configNameList;

    if (openSheetData.size() == 0) {    // Excel New
        const int rowMax = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
        for (const auto& sheet : sheetName) {
            int columnMax;
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                columnMax = descTitle.size();
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                columnMax = configTitle.size();
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) {
                columnMax = dependentOnTitle.size();
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) {
                columnMax = manualTCTitle.size();
            } else {
                columnMax = otherTitle.size();
            }
            QVariantList rowDataList;
            for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
                rowDataList.append(QStringList(columnMax));
            }
            sheetDataList.append(rowDataList);
            sheetIndex++;
        }
        excelView = ivis::common::ViewTypeEnum::ExcelTypeNew;
    } else {    // Excel Open
        for (const auto& sheetData : sheetDataList) {
            for (const auto& rowDataList : sheetData) {
                if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
                    (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) ||
                    (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC)) {
                    continue;
                }
                auto columnDataList = rowDataList.toStringList();
                if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                    configNameList.append(columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)));
                } else {
                    tcNameList.append(columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                }
            }
            sheetIndex++;
        }
        excelView = ivis::common::ViewTypeEnum::ExcelTypeOpen;
    }

    sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    for (const auto& sheetData : sheetDataList) {
        updateSheetData(sheetIndex++, sheetData, false);
    }
    checkTimer.check("updateExcelSheet : sheet data");

    updateAutoCompleteName();
    checkTimer.check("updateExcelSheet : auto complete data");

    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescConfigVisible).toBool() == false) {
        descTitle.resize(static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal));
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle, configTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDependentOnTitle, dependentOnTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelManualTCTitle, manualTCTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    checkTimer.check("updateExcelSheet : title, sheet name");

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelView, excelView, true);
    checkTimer.check("updateExcelSheet : excel view");
}

bool ControlExcel::writeExcelFile(const QVariant& filePath) {
    if (ExcelUtil::instance().data()->isCheckPythonLibrary() == false) {
        return false;
    }
    // ivis::common::CheckTimer checkTimer;
    ExcelUtil::instance().data()->writeExcelSheet(filePath.toString(), false);
    // qDebug() << "writeExcelFile :" << filePath;
    // checkTimer.check("openExcelFile");
    return true;
}

bool ControlExcel::openExcelFile(const QVariant& filePath) {
    if (ExcelUtil::instance().data()->isCheckPythonLibrary() == false) {
        return false;
    }
    // ivis::common::CheckTimer checkTimer;
    QList<QVariantList> sheetDataList = ExcelUtil::instance().data()->openExcelFile(filePath.toString());
    // qDebug() << "openExcelFile :" << filePath;
    // checkTimer.check("openExcelFile");

    if (sheetDataList.size() > 0) {
        // QVariantList text = QVariantList({STRING_POPUP_OPENING_FILE, STRING_POPUP_OPENING_FILE_TIP});
        // QVariant popupData = QVariant();
        // ivis::common::Popup::drawPopup(ivis::common::PopupType::OpeningFile, isHandler(), popupData, QVariant(text));
        updateExcelSheet(sheetDataList);
        return true;
    }
    return false;
}

void ControlExcel::loadExcelFile(const int& eventType) {
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeFileNew: {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
                ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
                QVariantList text = QVariantList(
                    {STRING_POPUP_EXCEL_EDIT, STRING_POPUP_EXCEL_EDIT_TIP, STRING_POPUP_CONFIRM, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::New, isHandler(), popupData, QVariant(text));
                if (button == ivis::common::PopupButton::Cancel) {
                    return;
                }
            }
            updateExcelSheet();

            QVariant allModule = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAllModule);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, allModule);

            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, QVariant());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, QVariant());

            // last - folder, file 로딩시 정보 못 가져옴
            // ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, QVariant());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileOpen: {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
                ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
                QVariantList text = QVariantList(
                    {STRING_POPUP_EXCEL_EDIT, STRING_POPUP_ANOTHER_FILE_OPEN_TIP, STRING_POPUP_CONFIRM, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::AnotherFileOpen, isHandler(), popupData, text);
                if (button == ivis::common::PopupButton::Cancel) {
                    return;
                }
            }

            QVariant defaultFilePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath);
            QVariant filePath = QVariant();
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), filePath,
                                               QVariantList({STRING_FILE_OPEN, defaultFilePath})) ==
                ivis::common::PopupButton::OK) {
                QStringList moduleTemp = QStringList();
                QStringList module = QStringList();
                int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();

                if (openExcelFile(filePath)) {
                    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, filePath);
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath);
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);

                    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
                        moduleTemp = filePath.toString().split("/model/SFC/");
                    } else {
                        moduleTemp = filePath.toString().split("/model/SFC/CV/");
                    }
                }

                if (moduleTemp.size() == 2) {
                    module = moduleTemp[1].split("/");
                }

                if (module.size() == 2) {
                    QVariant selectModuleList = QVariant(QVariantList({module[0]}));
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, selectModuleList);
                    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, ivis::common::EventTypeEnum::EventTypeSelectModule,
                                  selectModuleList);
                }
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFolder: {
            QVariant filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
            QFileInfo fileInfo(filePath.toString());
            QString directory = fileInfo.absolutePath();
            QString fileName = fileInfo.fileName();
            if (directory.size() == 0) {
                directory = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath).toString();
            }

            filePath.clear();
            ivis::common::PopupButton button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(),
                                               filePath, QVariantList({STRING_FILE_OPEN, directory}));
            if (button == ivis::common::PopupButton::OK) {
                openExcelFile(filePath);
                updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, filePath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile: {
            QVariant filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
            ivis::common::PopupButton button = ivis::common::PopupButton::OK;
            if (filePath.toString().size() == 0) {
                QVariant defaultFilePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath);
                filePath.clear();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), filePath,
                         QVariantList({STRING_FILE_OPEN, defaultFilePath}));
            }

            if (button == ivis::common::PopupButton::OK) {
                openExcelFile(filePath);
                updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, filePath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
            }
            break;
        }
        default: {
            break;
        }
    }
}

void ControlExcel::saveExcelFile(const bool& saveAs) {
    bool fileSave = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();
    if ((fileSave == false) && (saveAs == false)) {
        qDebug() << "The file is not saved because the contents of the excel have not changed :" << saveAs << fileSave;
        return;
    }

    // last - folder, file 로딩시 정보 못 가져옴
    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelView).toInt() == ivis::common::ViewTypeEnum::ExcelTypeNew) {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, QVariant());
    }

    ivis::common::PopupButton button = ivis::common::PopupButton::OK;
    QVariant saveFilePath =
        (saveAs) ? (QVariant()) : (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath));

    if (saveFilePath.toString().size() == 0) {
        QVariant filePath = QVariant();
        saveFilePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath);
        button =
            ivis::common::Popup::drawPopup(ivis::common::PopupType::Save, isHandler(), filePath, QVariantList({saveFilePath}));
        if (button == ivis::common::PopupButton::OK) {
            saveFilePath = filePath;
        }
    }

    if (button == ivis::common::PopupButton::OK) {
        if (writeExcelFile(saveFilePath)) {
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, saveFilePath);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, saveFilePath);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, saveFilePath.toString());
        }
    }
}

void ControlExcel::updateShortcutInfo(const int& eventType) {
    int shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInvalid;

    if (eventType == ivis::common::EventTypeEnum::EventTypeEditCut) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeCut;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditCopy) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeCopy;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditPaste) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypePaste;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditInsert) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInsert;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditDelete) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeDelete;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditMergeSplit) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditUndo) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeUndo;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditRedo) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeRedo;
    } else {
        qDebug() << "Fail to shortcut eventType :" << eventType;
        return;
    }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShortcutType, shortcutType, true);
}

void ControlExcel::updateGenType(const int& genType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeGenType, genType, true);
}

void ControlExcel::updateTCCheck(const int& allCheck) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCCheck, allCheck, true);
}

void ControlExcel::updateNodeAddress() {
    // Update : SFC, Vehicle
    QString nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath).toString();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        nodeAddressPath.append("/PV");
    } else {
        nodeAddressPath.append("/CV");
    }
    auto nodeAddressInfo = QList<QPair<int, QString>>({
        qMakePair(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC, QString("NodeAddressSFC.info")),
        qMakePair(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM, QString("NodeAddressVSM.info")),
    });
    for (auto nodeAddress : nodeAddressInfo) {
        QStringList list = ivis::common::FileInfo::readFile(QString("%1/%2").arg(nodeAddressPath).arg(nodeAddress.second));
        list.sort();
        list.removeDuplicates();
        updateDataHandler(nodeAddress.first, list, true);
    }
}

void ControlExcel::updateAutoCompleteName() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QStringList tcNameList;
    QStringList cofigNameList;
    QStringList dependentNameList;
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        for (const auto& rowDataList : getData(sheetIndex).toList()) {
            if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
                (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC)) {
                continue;
            }
            auto columnDataList = rowDataList.toStringList();
            QString text;
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                text = columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
                ivis::common::getRemoved(text, mergeInfos);
                cofigNameList.append(text);
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) {
                text = columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::TCName));
                ivis::common::getRemoved(text, mergeInfos);
                dependentNameList.append(text);
            } else {
                text = columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                ivis::common::getRemoved(text, mergeInfos);
                tcNameList.append(text);
            }
        }
    }

    // qDebug() << "updateAutoCompleteName";
    // qDebug() << "\t TCName        :" << tcNameList;
    // qDebug() << "\t ConfigName    :" << cofigNameList;
    // qDebug() << "\t DependentName :" << dependentNameList;

    // Update : TCName
    tcNameList.sort();
    tcNameList.removeAll("");
    tcNameList.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName, tcNameList, true);

    // Update : ConfigName
    cofigNameList.sort();
    cofigNameList.removeAll("");
    cofigNameList.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName, cofigNameList, true);

    // Update : DependentName
    dependentNameList.sort();
    dependentNameList.removeAll("");
    dependentNameList.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressDependentName, dependentNameList, true);
}

void ControlExcel::updateAutoCompleteSignal(const QString& signalName, const QString& vehicleType, const int& columnIndex) {
    QStringList signalData = QStringList();
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
    QMap<int, QStringList> dataInfo =
        SignalDataManager::instance().data()->isSignalDataList(signalName, signalData, vehicleType, dataType);
    bool update = false;

    for (int index = 0; index < ivis::common::InputDataTypeEnum::InputDataTypeMax; ++index) {
        QStringList suggestionsDataInfo = dataInfo[index];
        if (index == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) {
            update = (suggestionsDataInfo.size() > 0);
        }
        updateDataHandler((ivis::common::PropertyTypeEnum::PropertyTypeInputDataValueEnum + index), suggestionsDataInfo);
    }

    if (update) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeValueEnum, QVariantList({signalName, columnIndex}), true);
    }
}

void ControlExcel::updateAutoCompleteTCName(const QString& signalName, const QString& vehicleType, const int& keywordType) {
    if ((keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) == false) {
        return;  // Sheet 키워드가 아닌 경우 -> 자동완성 리스트 구성 하지 않음
    }

#if 1
    QStringList suggestionsDataInfo;
    int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs;

    ExcelDataManager::instance().data()->reloadExcelData();
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        auto resultName = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, signalName);
        if (resultName.size() > 0) {
            // qDebug() << "AutoCompleteInfo :" << signalName  << resultName;
            suggestionsDataInfo = resultName;
            break;
        }
    }

    if (suggestionsDataInfo.size() > 0) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult, suggestionsDataInfo, true);
    }
#else
    bool update = false;
    QStringList tcNameList = getData(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName).toStringList();
    for (const auto& tcName : tcNameList) {
        if (tcName.compare(signalName) == 0) {
            update = true;
            break;
        }
    }

    if (update) {
        QStringList suggestionsDataInfo;
        int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
        int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs;

        ExcelDataManager::instance().data()->reloadExcelData();
        for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
            for (const auto& tcName : ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true)) {
                for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                    // qDebug() << "AutoCompleteInfo :" << signalName << tcName << resultName;
                    if (signalName == tcName) {
                        suggestionsDataInfo.append(resultName);
                    }
                }
                if (suggestionsDataInfo.size() > 0) {
                    sheetIndex = endIndex;
                    break;
                }
            }
        }
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult, suggestionsDataInfo, true);
    }
#endif
}

void ControlExcel::updateAutoCompleteSuggestions(const QVariantList& inputData) {
    if (inputData.size() != 3) {
        qDebug() << "Fail to auto complete input data size :" << inputData.size();
        return;
    }
    int sheetIndex = inputData.at(0).toInt();
    int rowIndex = inputData.at(1).toInt();
    int columnIndex = inputData.at(2).toInt();
    int signalIndex = (-1);

    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal);
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputSignal);
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionSignal);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputSignal);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitSignal);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputSignal);
            } else {
            }
            break;
        }
        default: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal);
            } else {
            }

            // // InputData, OutputValue 자동완성시 입력된 vehicleType 이 아닌 전체 vehicleType 을 사용해야함.
            // int vehicleIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType);
            // vehicleType = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, vehicleIndex);
            break;
        }
    }

    if (signalIndex >= 0) {
        auto vehicleTypeList =
            isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList();
        QString vehicleType = vehicleTypeList.join(", ");
        QString signalName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, signalIndex);

        int keywordType = ExcelUtil::instance().data()->isKeywordType(signalIndex, signalName);
        int signalType = SignalDataManager::instance().data()->isSignalType(signalName);

        qDebug() << "updateAutoCompleteSuggestions :" << keywordType << sheetIndex << signalType << columnIndex << signalName;
        if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
            updateAutoCompleteTCName(signalName, vehicleType, keywordType);
        } else {
            updateAutoCompleteSignal(signalName, vehicleType, columnIndex);
        }
    }
}

void ControlExcel::updateAutoCompleteDescription(const QVariantList& inputData) {
    if (inputData.size() != 3) {
        qDebug() << "Fail to auto desc input data size :" << inputData.size();
        return;
    }
    int sheetIndex = inputData.at(0).toInt();
    int rowIndex = inputData.at(1).toInt();
    int columnIndex = inputData.at(2).toInt();

    if (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
        return;
    }

    QString moduleName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, columnIndex);
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, true, true);
    auto fileName = moduleInfo[moduleName.toUpper()].second;

    // qDebug() << "updateAutoCompleteDescription :" << moduleName << moduleName.toUpper() << fileName;
    if (fileName.size() == 0) {
        qDebug() << "Not found matching module name :" << moduleName << fileName;
        return;
    }

    QStringList readData = ivis::common::FileInfo::readFile(fileName);
    QSet<QString> foundStr({"  sfcVersion: ", "  description: "});
    QStringList descInfo;
    for (const auto& data : readData) {
        for (const auto& str : foundStr) {
            if (data.startsWith(str)) {  // contains() 대신 startsWith() 사용 (더 정확한 비교)
                descInfo.append(data.mid(str.length()).remove('\"'));  // 불필요한 변수 제거 및 최적화
            }
        }
        if (descInfo.size() == foundStr.size()) {
            break;
        }
    }

    if (descInfo.size() > 0) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo, descInfo, true);
    }
}

void ControlExcel::updateDataValidation(const QVariantList& cellInfo) {
#if 0    // 현재 사용 하지 않음 : 추후 입력 데이터 검증 관련 코드 추가 필요
    if (cellInfo.size() != 3) {
        qDebug() << "Fail to cell data info size :" << cellInfo.size();
        return;
    }
    int sheetIndex = cellInfo.at(0).toInt();
    int rowIndex = cellInfo.at(1).toInt();
    int columnIndex = cellInfo.at(2).toInt();
    int signalColumnIndex = 0;

    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                signalColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal);
            }
            break;
        }
        default: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
                signalColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
                signalColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal);
            } else {
            }
            break;
        }
    }
    QString signalName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, signalColumnIndex);
    QString inputData = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, columnIndex);
    int signalType = SignalDataManager::instance().data()->isSignalType(signalName);

    qDebug() << "updateDataValidation :" << signalName << inputData;

    if (inputData.size() == 0) {
        // qDebug() << "Fail to input data info size : 0";
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QVariant vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV);
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV);
    }

    QString vehicleType = QString();
    for (const auto& vehicle : vehicleTypeList.toStringList()) {
        if (vehicleType.size() > 0) {
            vehicleType.append(", ");
        }
        vehicleType.append(vehicle);
    }

    QStringList signalData = inputData.remove(" ").split(",");
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
    QMap<int, QStringList> dataInfo =
        SignalDataManager::instance().data()->isSignalDataList(signalName, signalData, vehicleType, dataType);
    int keywordType = ExcelUtil::instance().data()->isKeywordType(columnIndex, signalName);

    if ((keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) ||
        (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) ||
        (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString))) {
        return;
    }

    QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    int validCount = 0;
    QString errorInfo = QString();
    int validDataIndex = 0;

    if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
        (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
        validDataIndex = 1;
    }
    for (const auto& data : signalData) {
        if (data.size() == 0) {
            errorInfo = QString(STRING_POPUP_DATA_VALIDATION_TIP).arg("spaces");
        } else if (data.toLower().trimmed().startsWith("0x")) {
            errorInfo = QString(STRING_POPUP_DATA_VALIDATION_TIP).arg("hex values");
        } else {
        }

        if (errorInfo.size() > 0) {
            break;
        }

        for (auto& originData : valueEnum) {
            originData.remove("\"");
            QStringList splitData = originData.split(":");
            if (splitData.size() < 2) {
                continue;
            }
            if (splitData.at(validDataIndex).toUpper().compare(data.toUpper()) == 0) {
                qDebug() << "\t Matching[" << validCount << "] :" << data << originData;
                validCount++;
                break;
            }
        }
    }

    if (validCount != signalData.size()) {
        if (errorInfo.size() == 0) {
            errorInfo = QString(STRING_POPUP_DATA_VALIDATION_TIP).arg("invalid values");
        }
        qDebug() << "\t -> ErrorInfo :" << validCount << errorInfo << ", Cell :" << sheetIndex << rowIndex << columnIndex;

        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::DataValidation, isHandler(), popupData,
                                       QVariantList({STRING_POPUP_DATA_VALIDATION, errorInfo}));
    }
#endif
}

void ControlExcel::updateStartTestCase(const QStringList& selectModule) {
    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenerateStart).toBool()) {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::TestCaseRunning, isHandler(), popupData,
                                       QVariantList({STRING_POPUP_TEST_CASE_RUNNING,  STRING_POPUP_TEST_CASE_RUNNING_TIP}));
        return;
    }
    if (SignalDataManager::instance().data()->isExcelDataValidation() == false) {
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    int excelView = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelView).toInt();
    QFileInfo fileInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile).toString());
    QString directory = QFileInfo(fileInfo.path()).fileName();
    QString editingModule;
    QStringList optionInfo;
    QString moduleName;

    switch (excelView) {
        case ivis::common::ViewTypeEnum::ExcelTypeOpen: {
            if (selectModule.size() == 0) {
                auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, false);
                for (const auto& module : moduleInfo.keys()) {
                    if (module == directory) {
                        moduleName = directory;
                        break;
                    }
                }
                if (moduleName.size() == 0) {
                    // moduleName = QString("/%1/%2").arg(directory).arg(fileInfo.baseName());
                    moduleName = fileInfo.filePath();
                }

                // 현재 엑셀 파일이 편집중인 상태 전달
                if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
                    editingModule = moduleName;
                }
                optionInfo.append(moduleName);
            } else {
                bool check = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();
                for (const auto& module : selectModule) {
                    qDebug() << "\t Check :" << check << module << directory;
                    if (module == directory) {
                        editingModule = directory;
                        break;
                    }
                }
                optionInfo.append(selectModule);
            }
            fileInfo = QFileInfo();
            break;
        }
        case ivis::common::ViewTypeEnum::ExcelTypeNew: {
            if (selectModule.size() == 0) {
                moduleName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewModule).toString();
                optionInfo.append(moduleName);
            } else {
                optionInfo.append(selectModule);
            }
            fileInfo = QFileInfo();
            break;
        }
        default: {
            optionInfo.append(selectModule);
            fileInfo = QFileInfo();
            break;
        }
    }

#if 1
    qDebug() << (QString(120, '='));
    qDebug() << "updateStartTestCase :" << excelView << selectModule.size();
    qDebug() << "\t selectModule   :" << selectModule;
    qDebug() << "\t EditingModule  :" << editingModule;
    qDebug() << "\t Option         :" << optionInfo;
    // qDebug() << "\t FilePath       :" << fileInfo.filePath();
    qDebug() << "\t FilePath       :" << fileInfo.path();
    qDebug() << "\t                :" << fileInfo.fileName();
    qDebug() << "\t                :" << directory;
    qDebug() << "\n\n";
    // return;
#endif

    if (optionInfo.size() > 0) {
        if (editingModule.size() > 0) {
            // 편집중인 엑셀과 다른 모듈을 선택하여 동작시 ExcelData::getSheetData() 의 데이터를
            // 사용 불가(파일 오픈시 ExcelData::setSheetData() 으로 데이터 저장)로 인해
            // 편집중인 데이터는 ExcelData::setEditingSheetData() 에 저장 후
            // TestCase::readSheetData() 함수에서 예외 처리 하여 사용함.
            const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
            const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
            for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
                ExcelData::instance().data()->setEditingSheetData(sheetIndex, getData(sheetIndex));
            }
            TestCase::instance().data()->setEditingModule(editingModule);
        }

        QString appModeInfo = (appMode == ivis::common::AppModeEnum::AppModeTypeCV) ? ("CV") : ("PV");
        optionInfo.append(appModeInfo);
        TestCase::instance().data()->start(optionInfo, fileInfo);
    }
}

void ControlExcel::updateSelectModuleList() {
    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenerateStart).toBool()) {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::TestCaseRunning, isHandler(), popupData,
                                       QVariantList({STRING_POPUP_TEST_CASE_RUNNING,  STRING_POPUP_TEST_CASE_RUNNING_TIP}));
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, false);
    QStringList moduleList = moduleInfo.keys();
    QStringList selectModuleList;

    int excelView = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelView).toInt();

    switch (excelView) {
        case ivis::common::ViewTypeEnum::ExcelTypeOpen: {
            QFileInfo fileInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile).toString());
            QString directory = QFileInfo(fileInfo.path()).fileName();
            bool existsModule = false;
            for (const auto& module : moduleInfo.keys()) {
                if (module == directory) {
                    existsModule = true;
                    break;
                }
            }
            if (existsModule == false) {
                // moduleList.prepend(directory);
                QString filePath = fileInfo.filePath();
                moduleList.prepend(filePath);
                selectModuleList.append(filePath);
            }
            selectModuleList.append(directory);
            break;
        }
        case ivis::common::ViewTypeEnum::ExcelTypeNew: {
            QString newModule = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewModule).toString();
            moduleList.prepend(newModule);
            selectModuleList.append(newModule);
            break;
        }
        default: {
            break;
        }
    }

    // qDebug() << "updateShowSelectModule :" << excelView << selectModuleList << moduleList.size();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, moduleList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, selectModuleList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule, true, true);
}

void ControlExcel::slotTestCaseCompleted(const int& type, const bool& sheetReload) {
    switch (type) {
        case TestCase::ExcuteTypeCompleted: {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
                int excelView = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelView).toInt();
                QVariantList info({
                    STRING_POPUP_TEST_CASE_COMPLETE,
                    STRING_POPUP_TEST_CASE_COMPLETE_TIP,
                    STRING_POPUP_CONFIRM,
                    STRING_POPUP_CANCEL
                });
                QVariant popupData;
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::TestCaseComplete, isHandler(), popupData, info) ==
                    ivis::common::PopupButton::Confirm) {
                    bool saveAs = (excelView == ivis::common::ViewTypeEnum::ExcelTypeNew);
                    saveExcelFile(saveAs);
                }
            }
            break;
        }
        case TestCase::ExcuteTypeStop:
        case TestCase::ExcuteTypeFailed: {
            break;
        }
        default: {
            break;
        }
    }

    if (sheetReload) {
        // AMS 파일 오픈 상태
        // GenTC - SelectModule 로 Air_Bag_CV 모듈만 선택
        // GenTC - Completed, Failed, Stop 시 Air_Bag_CV 모듈 파싱 정보가 저장됨
        // TestCase::writeSheetData() 함수 내부 ExcelData::instance().data()->setSheetData() 함수에서 저장됨
        // 이후 AMS - Event 시트에서 데이터 자동완성 화면 클릭시 Air_Bag_CV 의 자동완성이 표시됨
        // ExcelUtil::isCurrentCellText() 함수 내부에서 ExcelData::instance().data()->getSheetData() 사용하여서 문제 발생
        // 수정 : GenTC 완료후 ControlExcel 내부에 저장되어 있는 Property 로 ExcelData::instance().data()->setSheetData() 업데이트
        updateSheetDataToExcelData();
    }
}

void ControlExcel::slotGenTCInfo(const int& resultType, const int& current, const int& total, const QStringList& info) {
    QVariantList eventInfo({resultType, current, total, info});
    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeUpdateGenTCInfo, eventInfo);
}

void ControlExcel::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlExcel::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeExcelMergeStart:
        case ConfigInfo::ConfigTypeExcelMergeEnd:
        case ConfigInfo::ConfigTypeExcelMerge: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge, value);
            break;
        }
        case ConfigInfo::ConfigTypeScreenInfo: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                              ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
            break;
        }
        default: {
            break;
        }
    }
}

void ControlExcel::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeOpenExcel: {
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteNameDescription: {
            updateAutoCompleteDescription(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteName: {
            updateAutoCompleteName();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData: {
            updateAutoCompleteSuggestions(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeCheckValidation: {
            updateDataValidation(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeWarningMergeSplit:
        case ivis::common::EventTypeEnum::EventTypeWarningCopyCut: {
            ivis::common::PopupType popupType = ivis::common::PopupType::CellSelectionError;
            QString title = STRING_POPUP_CELL_SELECTION_ERROR;
            QString tips = STRING_POPUP_CELL_SELECTION_ERROR_TIP_0;
            if (type == ivis::common::EventTypeEnum::EventTypeWarningMergeSplit) {
                if (value.toInt() == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) {
                    tips = STRING_POPUP_CELL_SELECTION_ERROR_TIP_2;
                } else if (value.toInt() == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)) {
                    tips = STRING_POPUP_CELL_SELECTION_ERROR_TIP_3;
                } else {
                    tips = STRING_POPUP_CELL_SELECTION_ERROR_TIP_1;
                }
            }
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(popupType, isHandler(), popupData, QVariantList({title, tips}));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModule: {
            updateStartTestCase(value.toStringList());
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
            if ((type > ivis::common::EventTypeEnum::EventTypeList) && (type < ivis::common::EventTypeEnum::EventTypeListMax)) {
                QVariantList data = value.toList();
                if (data.size() == 2) {
                    int propertyType = data.at(0).toInt();
                    QVariantList sheetData = data.at(1).toList();
                    updateSheetData(propertyType, sheetData, true);
                }
            }
            break;
        }
    }
}

void ControlExcel::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & displayType) == false) {
        return;
    }

    // qDebug() << "ControlExcel::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose: {
            ControlManager::instance().data()->raise(displayType);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFolder:
        case ivis::common::EventTypeEnum::EventTypeLastFile:
        case ivis::common::EventTypeEnum::EventTypeFileNew:
        case ivis::common::EventTypeEnum::EventTypeFileOpen: {
            loadExcelFile(eventType);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileSave:
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs: {
            saveExcelFile(eventType == ivis::common::EventTypeEnum::EventTypeFileSaveAs);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditUndo:
        case ivis::common::EventTypeEnum::EventTypeEditRedo:
        case ivis::common::EventTypeEnum::EventTypeEditCut:
        case ivis::common::EventTypeEnum::EventTypeEditCopy:
        case ivis::common::EventTypeEnum::EventTypeEditPaste:
        case ivis::common::EventTypeEnum::EventTypeEditInsert:
        case ivis::common::EventTypeEnum::EventTypeEditDelete:
        case ivis::common::EventTypeEnum::EventTypeEditMergeSplit: {
            updateShortcutInfo(eventType);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditGenType: {
            updateGenType(eventValue.toInt());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditTCCheck: {
            updateTCCheck(eventValue.toInt());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenSelectModule: {
            updateSelectModuleList();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenCurrentModule: {
            updateStartTestCase();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenerateCancel: {
            TestCase::instance().data()->stop();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTest: {
#if 1
            SignalDataManager::instance().data()->testCode();
#endif
#if 0
            ExcelDataManager::instance().data()->isManualDataList();
            ExcelUtil::instance().data()->readPreset("Preset_003");
            ExcelUtil::instance().data()->readPreset("SFC.Extension.Param.Par_SpeedToleranceMph3");
            ExcelUtil::instance().data()->readPreset("SFC.Extension.Memory.TEST_02");
            ExcelUtil::instance().data()->readPreset("SFC.Extension.Memory_IGN.TEST_12");
            ExcelUtil::instance().data()->readPreset("TEST");
#endif
#if 0
            ExcelDataManager::instance().data()->reloadExcelData();
            ExcelDataManager::instance().data()->isDependentDataList("IMG_TelltaleABSAmberLamp_stat", "ON_ABS_Spec");

            ExcelDataManager::instance().data()->isDependentDataList("Dependt_01", "On");
            ExcelDataManager::instance().data()->isDependentDataList("Dependt_01", "Off");
            ExcelDataManager::instance().data()->isDependentDataList("Dependt_02", "Ok");

            ExcelDataManager::instance().data()->isDependentDataList("Dependt_02", "Fail");
            ExcelDataManager::instance().data()->isDependentDataList("Dependt_02", "Error");
            ExcelDataManager::instance().data()->isDependentDataList("Dependt_02", "TEST");
#endif
            break;
        }
        default: {
            break;
        }
    }
}
