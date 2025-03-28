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

const QString VEHICLE_TYPE_ICV = QString("ICV");
const QString VEHICLE_TYPE_EV = QString("EV");
const QString VEHICLE_TYPE_FCEV = QString("FCEV");
const QString VEHICLE_TYPE_PHEV = QString("PHEV");
const QString VEHICLE_TYPE_HEV = QString("HEV");
const QString SFC_IGN_ELAPSED = QString("SFC.Private.IGNElapsed.Elapsed");
const QString SFC_DONT_CARE = QString("D'");

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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, QVariant(false));

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd));

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTypeList));

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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo, keywordTypeInfo);
}

void ControlExcel::initControlData() {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString defaultFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    defaultFilePath.append((appMode == ivis::common::AppModeEnum::AppModeTypePV) ? ("/SFC") : ("/SFC/CV"));
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath, defaultFilePath);

    QVariant lastSavedFile = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, lastSavedFile);
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
        connect(TestCase::instance().data(), &TestCase::signalTestCaseCompleted, [=](const int& type, const bool& result) {
            // TestCase 가 별도 쓰레드로 분리되어 동작함.
            QMetaObject::invokeMethod(this, "slotTestCaseCompleted", Q_ARG(int, type), Q_ARG(bool, result));
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

void ControlExcel::updateSheetData(const int& propertyType, const QVariantList& sheetData) {
    if (getData(propertyType).toList() == sheetData) {
        // qDebug() << "There are no changes to the data :" << propertyType << sheetData.size();
        return;
    }

    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    QList<int> mergeColumnIndex;
    if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
    } else if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
               (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        mergeColumnIndex = QList<int>({
            static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName),
            static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal),
        });
    } else {
        mergeColumnIndex = QList<int>({
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName),
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType),
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config),
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
        });
    }

    QVariantList originSheetData;
    QMap<int, QString> titleText;
    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
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
        originSheetData.append(rowData);
    }

    updateDataHandler(propertyType, originSheetData);
    ExcelData::instance().data()->setSheetData(propertyType, originSheetData);
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

void ControlExcel::updateExcelSheet(const QList<QVariantList>& openSheetData) {
    const QVariant sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName);
    const QVariant descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
    const QVariant configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle);
    const QVariant otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle);

    ivis::common::CheckTimer checkTimer;
    bool excelOpen = (openSheetData.size() > 0);
    QList<QVariantList> sheetDataList = openSheetData;
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;

    QStringList tcNameList;
    QStringList configNameList;

    if (excelOpen == false) {
        const int rowMax = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
        for (const auto& sheet : sheetName.toStringList()) {
            int columnMax;
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                columnMax = descTitle.toStringList().size();
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                columnMax = configTitle.toStringList().size();
            } else {
                columnMax = otherTitle.toStringList().size();
            }
            QVariantList rowDataList;
            for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
                rowDataList.append(QStringList(columnMax));
            }
            sheetDataList.append(rowDataList);
            sheetIndex++;
        }
    } else {
        for (const auto& sheetData : sheetDataList) {
            for (const auto& rowDataList : sheetData) {
                if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
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
    }

    sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    for (const auto& sheetData : sheetDataList) {
        updateSheetData(sheetIndex++, sheetData);
    }
    checkTimer.check("updateExcelSheet : sheet data");

    updateAutoCompleteName();
    checkTimer.check("updateExcelSheet : auto complete data");

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle, configTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);
    checkTimer.check("updateExcelSheet : info");

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, excelOpen, true);
    checkTimer.check("updateExcelSheet : visible");
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
            // Open, Edit 시 사용된 정보 초기화
            QVariant allModule = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAllModule);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, allModule);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, QVariant());
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile, QVariant());
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
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath.toString());
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
            } else {
                // openExcelFile(filePath);
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
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        for (const auto& rowDataList : getData(sheetIndex).toList()) {
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                continue;
            }
            auto columnDataList = rowDataList.toStringList();
            QString text;
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                text = columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
                ivis::common::getRemoved(text, mergeInfos);
                cofigNameList.append(text);
            } else {
                text = columnDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                ivis::common::getRemoved(text, mergeInfos);
                tcNameList.append(text);
            }
        }
    }

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

    // qDebug() << "updateAutoCompleteName";
    // qDebug() << "\t TCName     :" << tcNameList;
    // qDebug() << "\t ConfigName :" << cofigNameList;
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
                    suggestionsDataInfo.append(resultName);
                }
                if (suggestionsDataInfo.size() > 0) {
                    sheetIndex = endIndex;
                    break;
                }
            }
        }
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult, suggestionsDataInfo, true);
    }
}

void ControlExcel::updateAutoCompleteSuggestions(const QVariantList& inputData) {
    if (inputData.size() != 3) {
        qDebug() << "Fail to auto complete input data size :" << inputData.size();
        return;
    }
    int sheetIndex = inputData.at(0).toInt();
    int rowIndex = inputData.at(1).toInt();
    int columnIndex = inputData.at(2).toInt();

    int signalIndex = 0;
    QString signalName;
    QString vehicleType;

    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal);
                signalName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, signalIndex);
            }
            break;
        }
        default: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
                signalName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, signalIndex);
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal);
                signalName = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, signalIndex);
            } else {
            }
            int vehicleIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType);
            vehicleType = ExcelUtil::instance().data()->isCurrentCellText(sheetIndex, rowIndex, vehicleIndex);
            break;
        }
    }

    if (signalName.size() == 0) {
        return;
    }

    if (vehicleType.isEmpty()) {
        QVariant vehicleTypeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType);
        vehicleType = vehicleTypeList.toStringList().join(", ");
    }

    int keywordType = ExcelUtil::instance().data()->isKeywordType(signalIndex, signalName);
    int signalType = SignalDataManager::instance().data()->isSignalType(signalName);

    // qDebug() << "updateAutoCompleteSuggestions :" << keywordType << signalType << columnIndex << signalName;
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        updateAutoCompleteTCName(signalName, vehicleType, keywordType);
    } else {
        updateAutoCompleteSignal(signalName, vehicleType, columnIndex);
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
}

void ControlExcel::updateStartTestCase(const QStringList& selectModule) {
    if (ExcelUtil::instance().data()->isExistsExcelSheet() == false) {
        return;
    }
    if (SignalDataManager::instance().data()->isExcelDataValidation() == false) {
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString appModeInfo = (appMode == ivis::common::AppModeEnum::AppModeTypeCV) ? ("CV") : ("PV");
    QStringList optionInfo(QStringList({appModeInfo}));

    if (selectModule.size() == 0) {
        bool excelOpen = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
        QString moduleName;

        if (excelOpen) {
            QFileInfo fileInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile).toString());
            moduleName = QFileInfo(fileInfo.path()).fileName();
        } else {
            moduleName = TestCase::instance().data()->getNewModule();
        }
        optionInfo.append(moduleName);
    } else {
        optionInfo.append(selectModule);
    }

    qDebug() << "updateStartTestCase :" << optionInfo;
    TestCase::instance().data()->start(optionInfo);
}

void ControlExcel::updateSelectModuleList() {
    if (ExcelUtil::instance().data()->isExistsExcelSheet() == false) {
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, false);
    QStringList moduleList = moduleInfo.keys();
    QStringList selectModuleList;

    bool excelOpen = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    if (excelOpen) {
        QFileInfo fileInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile).toString());
        QString moduleName = QFileInfo(fileInfo.path()).fileName();
        selectModuleList.append(moduleName);
    } else {
        QString newModule = TestCase::instance().data()->getNewModule();
        moduleList.prepend(newModule);
        selectModuleList.append(newModule);
    }

    qDebug() << "updateShowSelectModule :" << selectModuleList << moduleList.size();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, moduleList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, selectModuleList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule, true, true);
}

void ControlExcel::slotTestCaseCompleted(const int& type, const bool& result) {
    switch (type) {
        case TestCase::ExcuteTypeCompleted: {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool()) {
                QVariantList info({
                    STRING_POPUP_TEST_CASE_COMPLETE,
                    STRING_POPUP_TEST_CASE_COMPLETE_TIP,
                    STRING_POPUP_CONFIRM,
                    STRING_POPUP_CANCEL
                });
                QVariant popupData;
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::TestCaseComplete, isHandler(), popupData, info) ==
                    ivis::common::PopupButton::Confirm) {
                    saveExcelFile(true);
                }
            }
            break;
        }
        case TestCase::ExcuteTypeFailed: {
            break;
        }
        default: {
            break;
        }
    }
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
                    updateSheetData(propertyType, sheetData);
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
        case ivis::common::EventTypeEnum::EventTypeGenTC: {
            updateSelectModuleList();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeRunMultiDocker: {
            updateStartTestCase();
            break;
        }
        default: {
            break;
        }
    }
}
