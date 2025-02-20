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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, QVariantList());
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
    updateNodeAddress(true, QStringList(), QStringList());

    QVariantList keywordTypeInfo;
    for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        QStringList keywordString = QStringList({STRING_SELECT_KEYWORD});
        for (const auto& infoPair : ExcelUtil::instance().data()->isKeywordPatternInfo(index)) {
            keywordString.append(infoPair.first);
        }
        keywordTypeInfo.append(keywordString);
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo, keywordTypeInfo);

#if defined(USE_SHOW_NEW_EXCEL_SHEET_AFTER_BOOTING)  // First Booting : new excel sheet
    updateExcelSheet(false, QVariant());
#endif
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
            slotTestCaseCompleted(type, result);
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
        updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip, (inputValue == Qt::Key_Control));
    } else if (inputType == ivis::common::KeyTypeEnum::KeyInputTypeRelease) {
        if (getData(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip).toBool()) {
            return;
        }

        if (inputValue == Qt::Key_Control) {
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeKeySkip, false);
        } else if (((inputValue >= Qt::Key::Key_A) && (inputValue <= Qt::Key::Key_Z)) || (inputValue == Qt::Key::Key_Escape) ||
                   (inputValue == Qt::Key::Key_Delete) || (inputValue == ivis::common::KeyTypeEnum::KeyInputValueOK)) {
            if (inputValue == Qt::Key::Key_C) {
                qDebug() << "ControlExcel : Ctrl + C";
            }
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKey, inputValue, true);
        } else if ((inputValue >= Qt::Key::Key_Left) && (inputValue <= Qt::Key::Key_Down)) {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeArrowKey, inputValue, true);
        } else {
            // qDebug() << "Excel Key Value :" << inputValue << std::hex << inputValue;
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

void ControlExcel::updateNodeAddress(const bool& all, const QStringList& tcNameList, const QStringList& cofigNameList) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    // Update : SFC, Vehicle
    if (all) {
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

    // Update : TCName
    QStringList tcNameInfo;
    for (auto tcName : tcNameList) {
        ivis::common::getRemoved(tcName, mergeInfos);
        if (tcName.size() > 0) {
            tcNameInfo.append(tcName);
        }
    }
    tcNameInfo.sort();
    tcNameInfo.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName, tcNameInfo, true);

    // Update : ConfigName
    QStringList configNameInfo;
    for (auto cofigName : cofigNameList) {
        ivis::common::getRemoved(cofigName, mergeInfos);
        if (cofigName.size() > 0) {
            configNameInfo.append(cofigName);
        }
    }
    configNameInfo.sort();
    configNameInfo.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName, configNameInfo, true);
}

void ControlExcel::updateSheetData(const int& propertyType, const QVariantList& sheetData) {
    if (getData(propertyType).toList() == sheetData) {
        return;
    }
    ivis::common::CheckTimer checkTimer;
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    // const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

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

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    checkTimer.check(QString("updateSheetData[%1] : data changed !!!").arg(propertyType));
#if 0
    int rowIndex = 0;
    for (const auto& rowDataList : originSheetData.toList()) {
        qDebug() << "\t Data[" << rowIndex++ << "] :" << rowDataList.toStringList();
    }
#endif
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n\n";
}

void ControlExcel::updateExcelSheet(const bool& excelOpen, const QVariant& dirPath) {
    qDebug() << "updateExcelSheet() ->" << excelOpen << "," << dirPath;

    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    QStringList configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle).toStringList();
    QStringList otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QVariantList rowCount = QVariantList();
    QStringList tcNameList = QStringList();
    QStringList configNameList = QStringList();
    int properytType = 0;

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle, configTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);

    if (excelOpen) {
        int sheetIndex = 0;
        for (const auto& sheet : sheetName) {
            QVariantList sheetData = QVariantList();
            QString filePath = QString("%1/%2_%3.fromExcel").arg(dirPath.toString()).arg(sheetIndex).arg(sheet);
            QStringList readData = ivis::common::FileInfo::readFile(filePath);
            QStringList titleList;
            bool checkTitle = false;

            properytType = sheetIndex + ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
            if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                titleList = descTitle;
            } else if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                titleList = configTitle;
            } else {
                titleList = otherTitle;
                checkTitle = true;
            }

            int columnMax = titleList.size();
            QList<int> notSameTitleIndex;
            QList<int> readTitleIndex;
            for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
                QStringList rowData = readData[rowIndex].split("\t");
                if (rowIndex < 2) {
                    // RowIndex[0] : column 인덱스(0, 1, 2, 3....) 정보

#if !defined(USE_SHEET_COLUMN_OLD)
                    if ((rowIndex == 1) && (checkTitle)) {  // RowIndex[1] : title(desc, other) 정보
                        for (const auto& title : titleList) {
                            if (rowData.contains(title) == false) {
                                notSameTitleIndex.append(titleList.indexOf(title));
                            }
                        }
                        if (notSameTitleIndex.size() > 0) {
                            std::reverse(notSameTitleIndex.begin(), notSameTitleIndex.end());
                            qDebug() << "The title list is not the same :" << notSameTitleIndex << sheet;
                        }
                    }
#endif
                    continue;
                }

#if !defined(USE_SHEET_COLUMN_OLD)
                if (notSameTitleIndex.size() > 0) {
                    QStringList temp = rowData;
                    for (const auto& index : notSameTitleIndex) {
                        QString appendText;
                        int insertIndex = 0;
                        if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)) {
                            insertIndex = 0;             // Befor index : TCName
                            appendText = rowData.at(0);  // Read index : TCName
                        } else if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)) {
                            insertIndex = 1;             // After index : TCName
                            appendText = rowData.at(0);  // Read index : TCName
                        } else if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
                            insertIndex = 2;             // Befor index : Result
                            appendText = rowData.at(0);  // Read index : Result
                        } else {
                            continue;
                        }

                        if (appendText.contains(mergeStart)) {
                            appendText = mergeStart;
                        } else if (appendText.contains(mergeEnd)) {
                            appendText = mergeEnd;
                        } else if (appendText.contains(merge)) {
                            appendText = merge;
                        } else {
                            appendText.clear();
                        }
                        temp.insert(insertIndex, appendText);
                    }
                    rowData = temp;
                }
#endif

                // 최대 사이즈 기준으로 정렬
                if (rowData.size() > columnMax) {
                    // qDebug() << "The row data sizes are not same :" << properytType << rowData.size() << columnMax;
                    rowData.resize(columnMax);
                }

                // 자동완성 데이터 구성 : TCName, ConfigName
                if (rowData.size() == columnMax) {
                    QString readText;
                    if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                        // Description : 자동완성 구성 불필요
                    } else if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                        readText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
                        configNameList.append(readText);
                    } else {
                        readText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                        tcNameList.append(readText);
                    }
                }

                // Sheet 데이터 구성
                sheetData.append(rowData);
            }

            rowCount.append(sheetData.size());
            updateSheetData(properytType, sheetData);

            // qDebug() << "File Open :" << filePath << ", Length :" << sheetData.size();
            // qDebug() << sheet << ":" << sheetData;
            // qDebug() << "==================================================================================================\n";
            sheetIndex++;
        }

        // Delete : Folder(TC)
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDeleteFileTC).toBool()) {
            QStringList log;
            ivis::common::ExcuteProgram process(false);
            process.start(QString("rm -rf %1").arg(dirPath.toString()), log);  // Delete : /TC/*.fromExcel
        }
    } else {
        int rowMax = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
        properytType = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
        for (const auto& sheet : sheetName) {
            rowCount.append(rowMax);
            updateSheetData(properytType, QVariantList());
            properytType++;
        }
    }
    updateNodeAddress(false, tcNameList, configNameList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, rowCount);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, excelOpen, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
}

bool ControlExcel::writeExcelFile(const QVariant& filePath) {
    if (ExcelUtil::instance().data()->isCheckPythonLibrary() == false) {
        return false;
    }

    // qDebug() << "writeExcelFile :" << filePath;

    bool result = false;
    if (ExcelUtil::instance().data()->writeExcelSheet(filePath, false)) {
        QString dirPath = ExcelUtil::instance().data()->systemCall(false, filePath);
        if (dirPath.size() > 0) {
            result = true;
            // Delete : Folder(TC)
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDeleteFileTC).toBool()) {
                QStringList log;
                ivis::common::ExcuteProgram process(false);
                process.start(QString("rm -rf %1").arg(dirPath), log);  // Delete : /TC/*.toExcel
            }
        }
    }
    return result;
}

bool ControlExcel::writeSheetInfo(const QVariant& filePath) {
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> excelDataInfo = QMap<int, QVariantList>();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    int writeSize = 0;

    for (const auto& sheet : sheetName) {
        excelDataInfo[sheetIndex++] = getData(sheetIndex).toList();
    }

    QStringList fileInfo = filePath.toString().split("/");
    QString savePath = QString();
    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        savePath.append(fileInfo[index]);
        savePath.append("/");
    }
    savePath.append("TC");

    QDir dir(savePath);
    if (dir.exists() == false) {
        dir.mkdir(savePath);
    }
    // qDebug() << "ControlExcel::writeSheetInfo() -> savePath :" << savePath;

    QString file = QString();
    sheetIndex = 0;
    for (const auto& detailInfo : excelDataInfo) {
        int index = 0;
        QString writeData = QString();
        for (const auto& detail : detailInfo) {
            if (index == ivis::common::CellInfoEnum::ListInfoExcel::Sheet) {
                file = QString("%1_%2.toExcel").arg(sheetIndex++).arg(detail.toString());
            } else if (index >= ivis::common::CellInfoEnum::ListInfoExcel::Title) {
                QString infoData = QString();
                int count = 0;
                for (QVariant info : detail.toList()) {
                    infoData.append(info.toString());
                    if (count++ < (detail.toList().size() - 1)) {
                        infoData.append("\t");
                    }
                }
                infoData.append("\n");
                writeData.append(infoData);
                // qDebug() << "String :" << infoData;
            } else {
                // nothing to do
            }
            index++;
        }

        if (writeData.size() > 0) {
            QString saveFilePath = QString("%1/%2").arg(savePath).arg(file);
            writeSize = ivis::common::FileInfo::writeFile(saveFilePath, writeData, false);
            if (writeSize == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << saveFilePath;
            }
        }
    }
    return (writeSize > 0);
}

bool ControlExcel::openExcelFile(const QVariant& filePath) {
    if (ExcelUtil::instance().data()->isCheckPythonLibrary() == false) {
        return false;
    }

    // qDebug() << "openExcelFile :" << filePath;

    bool result = false;
    QString dirPath = ExcelUtil::instance().data()->systemCall(true, filePath);
    if (dirPath.size() > 0) {
        updateExcelSheet(true, dirPath);
        result = true;
    } else {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenFail, isHandler(), popupData,
                                       QVariantList({STRING_FILE_OPEN, STRING_FILE_OPEN_FAIL}));
    }
    return result;
}

bool ControlExcel::updateExcelDataInfo(const QString& filePath) {
    const QVariant sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName);
    const QVariant descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
    const QVariant configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle);
    const QVariant otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle);
    QVariantList rowCount;

    bool excelOpen = filePath.size();
    QList<QVariantList> sheetDataList;

    if (excelOpen) {
        sheetDataList = ExcelUtil::instance().data()->openExcelFile(filePath);
        for (const auto& sheetData : sheetDataList) {
            rowCount.append(sheetData.size());
        }
    } else {
        int rowMax = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
        for (const auto& sheet : sheetName.toStringList()) {
            rowCount.append(rowMax);
            sheetDataList.append(QVariantList());
        }
    }

    QStringList tcNameList;
    QStringList configNameList;
    updateNodeAddress(false, tcNameList, configNameList);

    int properytType = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    for (const auto& sheetData : sheetDataList) {
        updateSheetData(properytType++, sheetData);
    }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle, configTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, rowCount);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, excelOpen, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);

    return true;
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
            updateExcelSheet(false, QVariant());
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
            // QVariant filePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile);
            QFileInfo fileInfo(filePath.toString());
            QString directory = fileInfo.absolutePath();
            QString fileName = fileInfo.fileName();
            if (directory.size() == 0) {
                directory = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath).toString();
            }

            filePath.clear();
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), filePath,
                                               QVariantList({STRING_FILE_OPEN, directory})) == ivis::common::PopupButton::OK) {
                openExcelFile(filePath);
            }
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath.toString());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile: {
            QVariant filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
            // QVariant filePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile);
            if (filePath.toString().size() == 0) {
                QVariant defaultFilePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath);
                filePath.clear();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), filePath,
                                                   QVariantList({STRING_FILE_OPEN, defaultFilePath})) ==
                    ivis::common::PopupButton::OK) {
                    openExcelFile(filePath);
                }
            } else {
                openExcelFile(filePath);
            }
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, filePath);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
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
        qDebug() << "The file is not saved because the contents of the excel have not changed.";
        return;
    }

    ivis::common::PopupButton button = ivis::common::PopupButton::OK;
    QVariant saveFilePath = (saveAs) ? (QVariant()) : (getData(ivis::common::PropertyTypeEnum::PropertyTypeLastSavedFile));
        // (saveAs) ? (QVariant()) : (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath));

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

void ControlExcel::updateClipboardInfo(const int& eventType) {
    int clipboardType = ivis::common::ShortcutTypeEnum::ShortcutTypeInvalid;
    if (eventType == ivis::common::EventTypeEnum::EventTypeEditCut) {
        clipboardType = ivis::common::ShortcutTypeEnum::ShortcutTypeCut;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditCopy) {
        clipboardType = ivis::common::ShortcutTypeEnum::ShortcutTypeCopy;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditPaste) {
        clipboardType = ivis::common::ShortcutTypeEnum::ShortcutTypePaste;
    } else {
        qDebug() << "Fail to clipboard eventType :" << eventType;
        return;
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeClipboardType, clipboardType, true);
}

void ControlExcel::updateShortcutInfo(const int& eventType) {
    int shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInvalid;
    if (eventType == ivis::common::EventTypeEnum::EventTypeEditCellInsert) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInsert;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditCellDelete) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeDelete;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeEditCellMergeSplit) {
        shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit;
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
        for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
            ExcelDataManager::instance().data()->updateExcelData(sheetIndex, getData(sheetIndex).toList());
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
    if (inputData.size() != 4) {
        qDebug() << "Fail to auto complete input data size :" << inputData.size();
        return;
    }

    QString signalName = inputData.at(0).toString();
    QString vehicleType = inputData.at(1).toString();
    int columnIndex = inputData.at(2).toInt();
    int signalIndex = inputData.at(3).toInt();

    if (signalName.size() == 0) {
        return;
    }

    int keywordType = ExcelUtil::instance().data()->isKeywordType(signalIndex, signalName);
    int signalType = SignalDataManager::instance().data()->isSignalType(signalName);

    qDebug() << "updateAutoCompleteSuggestions :" << keywordType << signalType << columnIndex << signalName;
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        updateAutoCompleteTCName(signalName, vehicleType, keywordType);
    } else {
        updateAutoCompleteSignal(signalName, vehicleType, columnIndex);
    }
}

void ControlExcel::updateAutoInputDescriptionInfo(const QVariantList& autoInputInfo) {
    if (autoInputInfo.size() != 3) {
        qDebug() << "Fail to auto input info size :" << autoInputInfo.size();
        return;
    }

    // int sheetIndex = autoInputInfo.at(0).toInt();
    // int row = autoInputInfo.at(1).toInt();
    QString moduleName = autoInputInfo.at(2).toString();
    QString fileName = SignalDataManager::instance().data()->isSfcFileInfo(moduleName);
    // qDebug() << "updateAutoInputDescriptionInfo :" << moduleName << fileName;

    if (fileName.size() == 0) {
        qDebug() << "Not found matching module name :" << moduleName;
        return;
    }

    QStringList readData = ivis::common::FileInfo::readFile(fileName);
    QStringList foundStr = QStringList({"  sfcVersion: ", "  description: "});
    QVariantList descInfo = QVariantList();
    for (const auto& data : readData) {
        for (const auto& str : foundStr) {
            if (data.contains(str)) {
                QString temp = data;
                temp.remove(str);
                temp.remove("\"");
                descInfo.append(temp);
            }
        }
        if (descInfo.size() == foundStr.size()) {
            break;
        }
    }

    if (descInfo.size() > 0) {
        // descInfo.prepend(sheetIndex);
        // descInfo.prepend(row);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo, descInfo, true);
    }
}

void ControlExcel::updateAutoCompleteData(const QVariantList& inputData) {
    if (inputData.size() != 2) {
        return;
    }
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    int sheetIndex = inputData.at(0).toBool();
    bool columnIndex = inputData.at(1).toBool();

    if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
        return;
    }

    bool tcNameEdit = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    bool configNameEdit = ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
                           (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)));

    QStringList tcNameList = QStringList();
    if (tcNameEdit) {
        for (int updateSheetIndex = startIndex; updateSheetIndex < endIndex; ++updateSheetIndex) {
            for (const auto& sheetDataList : getData(updateSheetIndex).toList()) {
                QVariantList sheetData = sheetDataList.toList();
                QString tcName = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
                if (tcName.size() > 0) {
                    tcNameList.append(tcName);
                }
            }
        }
    }

    QStringList configNameList = getData(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName).toStringList();
    if (configNameEdit) {
        QVariantList sheetData = getData(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs).toList();
        QString configName = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)).toString();
        if (configName.size() > 0) {
            configNameList.append(configName);
        }
    }

    // qDebug() << "updateAutoCompleteData :" << tcNameList << configNameList;
    updateNodeAddress(false, tcNameList, configNameList);
}

void ControlExcel::updateInputDataValidation(const QVariantList& cellDataInfo) {
    if (cellDataInfo.size() != 5) {
        qDebug() << "Fail to cell data info size :" << cellDataInfo.size();
        return;
    }

    QString signalName = cellDataInfo.at(0).toString();
    QString inputData = cellDataInfo.at(1).toString();
    int sheetIndex = cellDataInfo.at(2).toInt();
    int rowIndex = cellDataInfo.at(3).toInt();
    int columnIndex = cellDataInfo.at(4).toInt();
    int signalType = SignalDataManager::instance().data()->isSignalType(signalName);
    qDebug() << "updateInputDataValidation :" << signalName << inputData;

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

void ControlExcel::updateGenDataInfo(const int& eventType) {
    if (SignalDataManager::instance().data()->isExcelDataValidation() == false) {
        qDebug() << "Fail to excel data validation.";
        return;
    }

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString appModeInfo = (appMode == ivis::common::AppModeEnum::AppModeTypeCV) ? ("CV") : ("PV");
    QString filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath).toString();
    QFileInfo fileInfo(filePath);
    QString moduleName = QDir(fileInfo.path()).dirName();

    TestCase::instance().data()->start(QStringList({appModeInfo, moduleName}));
}

void ControlExcel::slotTestCaseCompleted(const int& type, const bool& result) {
    qDebug() << "\n\t slotTestCaseCompleted :" << type << result;
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
        case ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData: {
            updateAutoCompleteData(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeAutoInputDescriptionInfo: {
            updateAutoInputDescriptionInfo(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeWarningMergeSplit: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                           QVariantList({STRING_POPUP_CELL_COLUMN, STRING_POPUP_CELL_COLUMN_TIP}));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeAutoCompleteSuggestions: {
            updateAutoCompleteSuggestions(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateCellDataInfo: {
            updateInputDataValidation(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeShortcutInsert: {
            QVariantList insertInfo = value.toList();
            if (insertInfo.size() == 3) {
                int sheetIndex = insertInfo.at(0).toInt();
                int rowStart = insertInfo.at(1).toInt();
                int rowCount = insertInfo.at(1).toInt();

                qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
                qDebug() << "EventTypeShortcutInsert :" << sheetIndex << rowStart << rowCount;
#if 0
                int rowIndex = 0;
                QVariantList newSheetDataList;
                QVariantList currentSheetDataList = getData(sheetIndex).toList();

                int rowEnd = currentSheetDataList.size() + rowCount;

                QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
                for (const auto& sheetDataList : getData(sheetIndex).toList()) {
                    if ((rowIndex >= rowStart) && (rowIndex < rowEnd)) {
                        newSheetDataList.append(dataInfo);
                        qDebug() << "\t Append Data[" << rowIndex << "] :" << dataInfo;
                    } else {
                        newSheetDataList.append(sheetDataList.toStringList());
                        qDebug() << "\t Origin Data[" << rowIndex << "] :" << sheetDataList.toStringList();
                    }
                    rowIndex++;
                }

                rowIndex = 0;
                for (const auto& sheetDataList : newSheetDataList.toList()) {
                    qDebug() << "\t Data[" << rowIndex << "] :" << sheetDataList.toStringList();
                    rowIndex++;
                }
#endif
                qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n\n";
            }
            break;
        }
        default: {
            if ((type > ivis::common::EventTypeEnum::EventTypeList) && (type < ivis::common::EventTypeEnum::EventTypeListMax)) {
                int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription +
                                   (type - ivis::common::EventTypeEnum::EventTypeListDescription);
                updateSheetData(propertyType, value.toList());
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
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeReceiveKeyFocus, true, true);
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
        case ivis::common::EventTypeEnum::EventTypeEditPaste: {
            updateClipboardInfo(eventType);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditCellInsert:
        case ivis::common::EventTypeEnum::EventTypeEditCellDelete:
        case ivis::common::EventTypeEnum::EventTypeEditCellMergeSplit: {
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
        case ivis::common::EventTypeEnum::EventTypeRunMultiDocker: {
            updateGenDataInfo(eventType);
            break;
        }
        default: {
            break;
        }
    }
}
