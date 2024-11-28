#include "ControlExcel.h"
#include "HandlerExcel.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"

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
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, QVariant());

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
        for (const auto& infoPair : isKeywordPatternInfo(index)) {
            keywordString.append(infoPair.first);
        }
        keywordTypeInfo.append(keywordString);
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo, keywordTypeInfo);

#if defined(USE_SHOW_NEW_EXCEL_SHEET_AFTER_BOOTING)  // Firt Booting : new excel sheet
    updateExcelSheet(false, QVariant());
#endif
}

void ControlExcel::initControlData() {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString defaultFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    defaultFilePath.append((appMode == ivis::common::AppModeEnum::AppModeTypePV) ? ("/SFC") : ("/SFC/CV"));
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath, defaultFilePath);
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
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
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
        QStringList sfcList = ivis::common::FileInfo::readFile(QString("%1/NodeAddressSFC.info").arg(nodeAddressPath));
        QStringList vsmList = ivis::common::FileInfo::readFile(QString("%1/NodeAddressVSM.info").arg(nodeAddressPath));
        sfcList.sort();
        vsmList.sort();
        sfcList.removeDuplicates();
        vsmList.removeDuplicates();
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC, sfcList, true);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM, vsmList, true);
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
    TestCase::instance().data()->setSheetData(propertyType, originSheetData);

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
            QStringList originTitleList;
            QStringList titleList;

            properytType = sheetIndex + ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;

            if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                titleList = descTitle;
            } else if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                titleList = configTitle;
            } else {
                titleList = otherTitle;
            }
            int columnMax = titleList.size();

            for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
                QStringList rowData = readData[rowIndex].split("\t");
                // qDebug() << properytType << ". rowData[" << rowIndex << "] :" << rowData.size() << "," << rowData;

                if ((rowIndex < 2) || (rowData.size() > columnMax)) {
                    // RowIndex[0] : column 인덱스 정보,  RowIndex[1] : title(desc, other) 정보
                    // columnMax 보다 큰 경우경우 유효하지 않은 데이터로 판단함
                    if ((rowIndex == 1) && (rowData.size() != columnMax)) {
                        originTitleList = rowData;
                        // qDebug() << "[ProperytType] :" << properytType;
                        // qDebug() << "\t Title Origin :" << originTitleList.size() << originTitleList;
                        // qDebug() << "\t Title New    :" << otherTitle.size() << otherTitle;
                    }
                    continue;
                }

#if 1  // KKH_EDIT_ADD_CONFIGS
                if (originTitleList.size() > 0) {
                    int insertIndex = (-1);
                    for (int index = 0; index < titleList.size(); ++index) {
                        if ((index >= originTitleList.size()) || (titleList[index] != originTitleList[index])) {
                            insertIndex = index;
                            break;
                        }
                    }
                    if (insertIndex > 0) {
                        QString appendText = rowData.at(0);
                        QString mergeText;
                        if (ivis::common::isContainsString(appendText, mergeStart)) {
                            mergeText = mergeStart;
                        } else if (ivis::common::isContainsString(appendText, mergeEnd)) {
                            mergeText = mergeEnd;
                        } else if (ivis::common::isContainsString(appendText, merge)) {
                            mergeText = merge;
                        } else {
                        }
                        // qDebug() << rowIndex << ". AppendText["  << insertIndex << "] :" << appendText << "->" << mergeText;
                        rowData.insert(insertIndex, mergeText);
                    }
                }
#endif

                // Sheet 데이터 구성
                sheetData.append(rowData);

                // 자동완성 데이터 구성 : TCName, ConfigName
                if (rowData.size() == 0) {
                    continue;
                }

                if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                    // Description : 자동완성 구성 불필요
                } else if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                    configNameList.append(rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)));
                } else {
                    tcNameList.append(rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                }
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
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeCreateExcelSheeet, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, rowCount);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, excelOpen, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
}

bool ControlExcel::writeExcelSheet(const QVariant& filePath, const bool& backup) {
    // Set Path : file, directory
    QStringList fileInfo = filePath.toString().split("/");
    QString writePath = QString();
    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        writePath.append(fileInfo[index]);
        writePath.append("/");
    }
    writePath.append("TC");

    QDir dir(writePath);
    if (dir.exists() == false) {
        dir.mkdir(writePath);
    }

    QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    QStringList configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle).toStringList();
    QStringList otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    int writeSize = 0;
    int sheetIndex = 0;
    int propertyType = (backup) ? (ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)
                                : (ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    for (const auto& sheet : sheetName) {
        QString file = QString("%1_%2.toExcel").arg(sheetIndex++).arg(sheet);
        QString writeData = QString();
        QVariantList sheetData = QVariantList();

        // Title - Append
        QStringList contentTitle;
        if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
            (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
            contentTitle = descTitle;
        } else if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
                   (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            contentTitle = configTitle;
        } else {
            contentTitle = otherTitle;
        }
        sheetData.append(contentTitle);

        // Data - Append
        sheetData.append(getData(propertyType++).toList());

        for (const auto& dataInfo : sheetData) {
            QString rowData = QString();
            int count = 0;
            for (QVariant info : dataInfo.toList()) {
                rowData.append(info.toString());
                if (count++ < (dataInfo.toList().size() - 1)) {
                    rowData.append("\t");
                }
            }
            rowData.append("\n");
            writeData.append(rowData);
            // qDebug() << "RowData :" << rowData;
        }

        if (writeData.size() > 0) {
            QString writeFielPath = QString("%1/%2").arg(writePath).arg(file);
            int size = ivis::common::FileInfo::writeFile(writeFielPath, writeData, false);
            writeSize += size;
            if (size == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << writeFielPath;
            }
        }
    }
    return (writeSize > 0);
}

bool ControlExcel::writeExcelFile(const QVariant& filePath) {
    bool result = false;
    if (checkPythonLibrary() == false) {
        qDebug() << "Fail to python lib not install.";
        return result;
    }

    if (writeExcelSheet(filePath, false)) {
        QString dirPath = sytemCall(false, filePath);
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

QString ControlExcel::sytemCall(const bool& readFile, const QVariant& filePath) {
    qDebug() << "ControlExcel::sytemCall() ->" << readFile << "," << filePath;
    QString cmdType = ((readFile) ? ("read") : ("write"));
    QStringList fileInfo = filePath.toString().split("/");

    if (fileInfo.size() == 0) {
        qDebug() << "Fail to input file path (size : 0)";
    }

    QString dirPath = QString();
    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        dirPath.append(fileInfo[index]);
        dirPath.append("/");
    }

    QString fileName = fileInfo[fileInfo.size() - 1];
    if ((fileName.contains(".xlsx", Qt::CaseInsensitive) == false) || (fileName.contains(".xls", Qt::CaseInsensitive) == false)) {
        fileName.append(".xlsx");
    }

    QString cmd =
        QString("python3 %1/ExcelParser.py %2 %3 %4").arg(ivis::common::APP_PWD()).arg(dirPath).arg(fileName).arg(cmdType);
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);

    if (result) {
        dirPath.append("TC");
    } else {
        dirPath.clear();
    }

    qDebug() << "*************************************************************************************************";
    qDebug() << "PWD      :" << ivis::common::APP_PWD();
    qDebug() << "System   :" << ((result) ? ("<Success>") : ("<fail>")) << cmd;
    qDebug() << "FilePath :" << filePath;
    qDebug() << "DirPath  :" << dirPath;
    for (const auto& d : log) {
        qDebug() << "LogData  :" << d;
    }
    qDebug() << "*************************************************************************************************\n";

    return dirPath;
}

bool ControlExcel::checkPythonLibrary() {
#if defined(USE_PYTHON_LIB_CHECK_READ_WRITE)
    bool openpyxl = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl).toBool();
    bool pandas = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibPandas).toBool();
    bool checkLib = ((openpyxl) && (pandas));

    if (checkLib == false) {
        ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
        QVariantList text =
            QVariantList({STRING_POPUP_LIB, STRING_POPUP_LIB_INSTALL_TIP, STRING_POPUP_INSTALL, STRING_POPUP_CANCEL});
        QVariant popupData = QVariant();
        button = ivis::common::Popup::drawPopup(ivis::common::PopupType::NoInstallLib, isHandler(), popupData, QVariant(text));
        if (button == ivis::common::PopupButton::Install) {
            mProcess.data()->setCommandInfo(QString("pip install openpyxl pandas"));
            mProcess.data()->start();
            connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramCompleted, [&](const bool& result) {
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl, true);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibPandas, true);
                ivis::common::Popup::drawPopup(ivis::common::PopupType::InstallComplete, isHandler(), popupData,
                                               QVariant(QVariantList({STRING_POPUP_LIB, STRING_POPUP_NOW_INSTALLING_TIP})));
            });
            ivis::common::Popup::drawPopup(ivis::common::PopupType::NowInstalling, isHandler(), popupData,
                                           QVariant(QVariantList({STRING_POPUP_LIB, STRING_POPUP_INSTALL_COMPLETE_TIP})));
        }
    }
    qDebug() << "Check lib - openpyxl :" << openpyxl << ", pandas :" << pandas;
    return checkLib;
#else
    return true;
#endif
}

bool ControlExcel::openExcelFile(const QVariant& filePath) {
    bool result = false;
    if (checkPythonLibrary()) {
        QString dirPath = sytemCall(true, filePath);
        if (dirPath.size() > 0) {
            updateExcelSheet(true, dirPath);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, filePath);
            result = true;
        } else {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenFail, isHandler(), popupData,
                                           QVariantList({STRING_FILE_OPEN, STRING_FILE_OPEN_FAIL}));
        }
    }
    return result;
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
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastSavedFilePath, QVariant());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeWindowTitle, QVariant());
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
        case ivis::common::EventTypeEnum::EventTypeLastFile: {
            QVariant lastFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath);
            if (lastFilePath.toString().size() == 0) {
                QVariant defaultFilePath = getData(ivis::common::PropertyTypeEnum::PropertyTypeDefaultFilePath);
                QVariant filePath = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), filePath,
                                                   QVariantList({STRING_FILE_OPEN, defaultFilePath})) ==
                    ivis::common::PopupButton::OK) {
                    openExcelFile(filePath);
                }
            } else {
                openExcelFile(lastFilePath);
            }
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
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

QString ControlExcel::isSfcFileInfo(const QString& signalName) {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModePV = (appMode == ivis::common::AppModeEnum::AppModeTypePV);
    int signalType = isSignalType(signalName);
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QStringList signalSplit = signalName.split(".");
    QString moduleName;
    QStringList fileList;


    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal)) {
        fileList.append(QString("%1/SFC/extension/external.yml").arg(sfcModelPath));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent)) {
        fileList.append(QString("%1/SFC/extension/Event.yml").arg(sfcModelPath));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon)) {
        QString sfcCommonEnum = QString("SFC_Common_Enum%1").arg((appModePV) ? ("") : ("_CV"));
        fileList.append(QString("%1/SFC/extension/%2.yml").arg(sfcModelPath).arg(sfcCommonEnum));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException)) {
        moduleName = signalSplit.at(2);  // signalName : SFC.Extension.{MODULE_NAME}.* | SFC.Private.{MODULE_NAME}.*
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc)) {
        if (signalSplit.size() > 1) {
            moduleName = signalSplit.at(1);  // signalName : SFC.{MODULE_NAME}.*
        } else {
            moduleName = signalSplit.at(0).toUpper();  // signalName : ABS_CV ->  Description 자동완성 참조 파일(대문자 변경)
        }
    } else {
    }

    if (moduleName.size() > 0) {
        QStringList sfcTypeList = QStringList();

        if (appModePV) {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        } else {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
            moduleName.replace("_New", "_CV");  // ModuleName : ADAS_Driving_New -> ADAS_Driving_CV
        }

        for (const auto& type : sfcTypeList) {
            fileList.append(QString("%1/SFC/%2/%3/%4.yml").arg(sfcModelPath).arg(type).arg(moduleName).arg(moduleName));
            if (appModePV == false) {
                fileList.append(QString("%1/SFC/%2/%3_CV/%4_CV.yml").arg(sfcModelPath).arg(type).arg(moduleName).arg(moduleName));
            }
        }
    }

    QString foundYml;
    for (const auto& file : fileList) {
        if (QFile::exists(file)) {
            foundYml = file;
            break;
        }
    }

    // if (foundYml.size() > 0) {
    //     qDebug() << "Found yml :" << foundYml;
    // } else {
    //     qDebug() << "Not found yml :" << signalName;
    // }

    return foundYml;
}

int ControlExcel::isDataType(const QString& dataTypeStr) {
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);

    if (dataTypeStr.compare("HUInt64") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
    } else if (dataTypeStr.compare("HInt64") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
    } else if (dataTypeStr.compare("HString") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HString);
    } else {
        // qDebug() << "isDataType -> DataType is incorrect :" << dataTypeStr;
    }
    return dataType;
}

QPair<int, int> ControlExcel::isIGNElapsedType(const QString& singalName) {
    QPair<int, int> ignInfo;
    if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn0ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn4000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn4000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn10s") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn10s),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff0ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff700ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff700ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else {
        ignInfo = QPair<int, int>(-1, -1);
    }
    return ignInfo;
}

QString ControlExcel::isIGNElapsedName(const int& ignType) {
    QString signalName;
    if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn0ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn3000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn3500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn4000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn4000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn10s)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn10s");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff0ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff700ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff700ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff1000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff1500ms");
    } else {
        signalName = QString("IGNElapsed type error");
    }
    return signalName;
}

QPair<QStringList, QStringList> ControlExcel::isConvertedIGNElapsedInfo(const QStringList& ignOriginData) {
    int ignValue = 0;
    bool foundOn = false;
    bool foundOn0ms = false;
    bool foundOff = false;
    bool foundOff0ms = false;

    for (const auto& ign : ignOriginData) {
        ignValue = ign.toInt();
        if (ignValue < static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            if (foundOn == false) {
                foundOn = true;
            }
            if (ignValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                foundOn0ms = true;
            }
        } else {
            if (foundOff == false) {
                foundOff = true;
            }
            if (ignValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
                foundOff0ms = true;
            }
        }
    }

    QList<int> ignAppend;
    bool appendOnOms = ((foundOn == false) && (foundOff0ms == false)) || ((foundOn == true) && (foundOff0ms == true));
    bool appendOffOms = ((foundOn0ms == false) && (foundOff == false)) || ((foundOn0ms == true) && (foundOff == true));
    bool appendOn0msOffOms = ((foundOn == true) && (foundOn0ms == false) && (foundOff == true) && (foundOff0ms == false));

    // Append - ElapsedOn0ms
    if ((appendOnOms) || (appendOn0msOffOms)) {
        ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    }
    // Append - ElapsedOff0ms
    if ((appendOffOms) || (appendOn0msOffOms)) {
        ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    }

    // Update - ConvertData
    QStringList ignConvertData = ignOriginData;
    for (const auto& ign : ignAppend) {
        QString ignStr = QString("%1").arg(ign);
        if (ignConvertData.contains(ignStr) == false) {
            ignConvertData.append(QString("%1").arg(ignStr));
        }
    }

    // Update - Precondition
    QStringList ignPrecondition;
    ignAppend.clear();
    for (const auto& ign : ignConvertData) {
        ignValue = ign.toInt();
        if (ignValue < static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
        } else {
            ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
        }
    }
    for (const auto& ign : ignAppend) {
        ignPrecondition.append(QString("%1").arg(ign));
    }

#if 0
    qDebug() << "[isConvertedIGNElapsedInfo]";
    qDebug() << "\t On :" << foundOn << ", On0ms :" << foundOn0ms << ", Off :" << foundOff<< ", Off0ms :"  << foundOff0ms;
    qDebug() << "\t OriginData   :" << ignOriginData;
    qDebug() << "\t ConvertData  :" << ignConvertData;
    qDebug() << "\t Precondition :" << ignPrecondition;
#endif

    return QPair<QStringList, QStringList>(ignConvertData, ignPrecondition);
}

QStringList ControlExcel::isVsmFileInfo(const QString& vehicleName, const QStringList& specType) {
    QStringList fileName = QStringList();
    QString vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    vsmPath.append("/VSM");
    QString fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBaseCV).toString();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBasePV).toString();
    }

    for (const auto& spec : specType) {
        if (vehicleName.compare("System") == false) {
            fileName.append(QString("%1/%2.%3.vsm").arg(vsmPath).arg(vehicleName).arg(spec));
        } else {
            fileName.append(QString("%1/%2").arg(vsmPath).arg(fileNameBase.arg(vehicleName).arg(spec)));
        }
    }
    return fileName;
}

QMap<int, QStringList> ControlExcel::isParsingFileDataInfo(const QString& signalName, const QStringList& inputData,
                                                           const QMap<int, QStringList>& fileList, int& dataType) {
    const QString PREFIX_TYPE = QString("type:");
    const QString PREFIX_SIGNAL_NAME = QString("signalName:");
    const QString PREFIX_DATA_TYPE = QString("dataType:");
    const QString PREFIX_DESCRIPTION = QString("description:");
    const QString PREFIX_ABSTRACTION_NAME = QString("abstractionName:");
    const QString PREFIX_VALUE_ENUM = QString("valueEnum:");
    const QString PREFIX_MATCHING_TABLE = QString("matchingTable:");
    const QString PREFIX_CODE_COMMENTING = QString("# ");
    const QString PREFIX_HYPHEN = QString("-");
    const QString PREFIX_DOT = QString(".");
    const QString PREFIX_COLON = QString(":");
    const QString PREFIX_SPACE = QString(" ");
    // const QString PREFIX_MCAN = QString("_MCAN");
    // const QString PREFIX_CCAN = QString("_CCAN");

    const int signalType = isSignalType(signalName);
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        qDebug() << "Input signal type is invalid";
        return QMap<int, QStringList>();
    }

    QString signal = QString();
    int index = 0;
    int startAppendIndex = 2;
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))  {
        startAppendIndex = 3;
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon))  {
        startAppendIndex = 1;
    } else {
    }
    for (const auto& splitText : signalName.split(PREFIX_DOT)) {
        if (index >= startAppendIndex) {
            signal.append((signal.size() > 0) ? (".") : (""));
            signal.append(splitText);
        }
        index++;
    }
    if (signal.size() == 0) {
        qDebug() << "Signal name is null";
        return QMap<int, QStringList>();
    }

    // qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "Input  Signal      :" << signalName;
    // qDebug() << "Parser Signal      :" << signal;

    QMap<int, QStringList> sfcDataInfo = QMap<int, QStringList>();
    QString dataTypeStr = QString();
    QMapIterator<int, QStringList> iter(fileList);
    while (iter.hasNext()) {
        iter.next();
        int inputDataType = iter.key();
        QStringList fileName = iter.value();
        // qDebug() << inputDataType << ". Size :" << fileName.size() << ", File :" << fileName;
        QStringList valueEunm = QStringList();
        QStringList matchingTable = QStringList();
        for (const auto& file : fileName) {
            if (QFile::exists(file) == false) {
                // qDebug() << "\t\t Fail to file not exists :" << file;
                continue;
            }
            QStringList readData = ivis::common::FileInfo::readFile(file);
            QStringList vsmInfo = QStringList();
            bool foundSignal = false;
            for (QString lineStr : readData) {
                if (foundSignal) {
                    if ((lineStr.trimmed().startsWith(PREFIX_HYPHEN)) && (lineStr.trimmed().endsWith(PREFIX_COLON))) {
                        // if ((lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        // qDebug() << "\t Next  Signal[" << inputDataType << "] :" << lineStr;
                        break;
                    } else if ((lineStr.size() == 0) || (lineStr.contains(PREFIX_TYPE)) ||
                               // (lineStr.contains(PREFIX_DATA_TYPE)) ||   // Skip : dataType
                               (lineStr.contains(PREFIX_SIGNAL_NAME)) || (lineStr.contains(PREFIX_ABSTRACTION_NAME)) ||
                               (lineStr.contains(PREFIX_DESCRIPTION)) || (lineStr.contains(PREFIX_CODE_COMMENTING))) {
                        // Skip : type, signalName, description, abstractionName, #(주석)
                        continue;
                    } else {
                        // Append : ValueEnum, MatchingTable
                        lineStr.remove(PREFIX_SPACE);
                        vsmInfo.append(lineStr);
                    }
                } else {
                    if ((lineStr.contains(signal)) && (lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        // Input Signal : (Vehicle.Speed_Gauge.Output_DisplaySpeedUnit)
                        // Read  Signal : (- Speed_Gauge.Output_DisplaySpeedUnit:)
                        lineStr.remove(PREFIX_HYPHEN);
                        lineStr.remove(PREFIX_COLON);
                        lineStr.remove(PREFIX_SPACE);

                        foundSignal = true;
                        if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
                            (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
                            foundSignal = (lineStr.compare(signal) == false);
                        }
                        // qDebug() << ((foundSignal) ? ("\t Found") : ("\t Skip ")) << "Signal[" << inputDataType
                        //          << "] :" << lineStr;
                    }
                }
            }

            bool foundValueEnum = false;
            bool foundMatchingTable = false;
            bool foundDataType = false;
            for (const auto& info : vsmInfo) {
                if (info.contains(PREFIX_DATA_TYPE)) {
                    dataTypeStr = info;
                    dataTypeStr.remove(PREFIX_DATA_TYPE);
                    continue;
                }
                if (foundMatchingTable) {
                    foundValueEnum = false;
                    matchingTable.append(info);
                    // qDebug() << "\t MatchingTable :" << info;
                } else {
                    foundMatchingTable = (info.contains(PREFIX_MATCHING_TABLE));
                }

                if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)) ||
                    (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum)) {
                    if (foundValueEnum) {
                        if (info.contains(PREFIX_MATCHING_TABLE)) {
                            continue;
                        }
                        valueEunm.append(info);
                        // qDebug() << "\t ValueEnum     :" << info;
                    } else {
                        QStringList exceptionValueEnum = info.split(PREFIX_VALUE_ENUM);
                        QString exceptionSignal = (exceptionValueEnum.size() == 2) ? (exceptionValueEnum.at(1)) : ("");
                        if (exceptionSignal.size() > 0) {
                            // "valueEnum: SFC_Common_Enum_CV.Telltale.Stat"
                            valueEunm.append(exceptionValueEnum[1].remove(" "));
                        } else {
                            // valueEnum:
                            //   NONE: 0x0
                            //   OFF: 0x1
                            //   ON: 0x2
                            foundValueEnum = info.contains(PREFIX_VALUE_ENUM);
                        }
                    }
                }
            }
        }

        if (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) {
            sfcDataInfo[inputDataType] = valueEunm;
            // qDebug() << "\t Value    Size :" << valueEunm.size();
        } else {
            sfcDataInfo[inputDataType] = matchingTable;
            // qDebug() << "\t Matching Size :" << matchingTable.size();
            if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)) &&
                (valueEunm.size() > 0)) {
                sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = valueEunm;
                // qDebug() << "\t Value    Size :" << valueEunm.size();
            }
        }
    }

    if (inputData.size() > 0) {
        sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = inputData;
    }
    dataType = isDataType(dataTypeStr);
    // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    return sfcDataInfo;
}

QMap<int, QStringList> ControlExcel::isSignalDataInfo(const QString& signalName, const QStringList& inputData,
                                                      const QString& vehicleType, int& dataType) {
    const int signalType = isSignalType(signalName);
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        // ex) collected
        return QMap<int, QStringList>();
    }

    QMap<int, QStringList> fileList = isSignalFileList(signalName, vehicleType);
    QMap<int, QStringList> sfcDataInfo = isParsingFileDataInfo(signalName, inputData, fileList, dataType);
    QStringList valueEnum = sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    QString sfcCommonSignal = ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc)) &&
                               (valueEnum.size() == 1)) ? (valueEnum.at(0)) : ("");
    if (sfcCommonSignal.size() > 0) {
        fileList = isSignalFileList(sfcCommonSignal, vehicleType);
        QMap<int, QStringList> sfcCommonDataInfo = isParsingFileDataInfo(sfcCommonSignal, inputData, fileList, dataType);
        QStringList sfcCommonValueEnum = sfcCommonDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
        sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = sfcCommonValueEnum;
#if 0
        qDebug() << "=================================================================================================";
        qDebug() << "Signal :" << signalName << "->" << sfcCommonSignal;
        qDebug() << "\t DataType   :" << dataType;
        qDebug() << "\t FileList   :" << fileList;
        qDebug() << "\t 1 ValueEnum :" << valueEnum;
        qDebug() << "\t 2 ValueEnum :" << sfcCommonValueEnum;
        qDebug() << "=================================================================================================\n\n";
#endif
    }
    return sfcDataInfo;
}

QMap<int, QStringList> ControlExcel::isSignalFileList(const QString& signalName, const QString& vehicleType) {
    QMap<int, QStringList> fileList = QMap<int, QStringList>();
    int signalType = isSignalType(signalName);

    if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
        (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
        QStringList typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
        int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
        if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
            typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        }

        // ValueEnum
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = isVsmFileInfo(QString("SKEL"), typeList);

        // MatchinigTable : CV(ICV, EV, FCEV)        PV(ICV, EV, FCEV, PHEV, HEV)
        for (const auto& vehicle : vehicleType.split(", ")) {
            int inputDataType = 0;
            if (vehicle.compare(VEHICLE_TYPE_ICV) == false) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV;
            } else if (vehicle.compare(VEHICLE_TYPE_EV) == false) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV;
            } else if (vehicle.compare(VEHICLE_TYPE_FCEV) == false) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV;
            } else if (vehicle.compare(VEHICLE_TYPE_PHEV) == false) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV;
            } else if (vehicle.compare(VEHICLE_TYPE_HEV) == false) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV;
            } else {
                continue;
            }
            fileList[inputDataType] = isVsmFileInfo(vehicle, typeList);
        }

        // MatchinigTable : System
        // typeList = QStringList({"Config", "Engineering", "Extra", "Gateway", "HardWire", "Micom", "TP", "Undefined"});
        typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSystemTypePV).toStringList();
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = isVsmFileInfo(QString("System"), typeList);
    } else {
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum].append(isSfcFileInfo(signalName));
    }

    // qDebug() << "isSignalFileList :" << signalName << vehicleType << fileList;
    return fileList;
}

QMap<int, QStringList> ControlExcel::isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                                      const bool& convert, const bool& mergeInfoErase,
                                                      QList<QStringList>& convertData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    QPair<int, int> rowInfo((-1), (-1));
    int foundSheetIndex = 0;

    if (convert) {
        startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
        endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;
    }

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        rowInfo = isContainsRowInfo(sheetIndex, tcName, result, QString());
        if ((rowInfo.first >= 0) && (rowInfo.second >= 0)) {
            foundSheetIndex = sheetIndex;
            break;
        }
    }

    if (foundSheetIndex == 0) {
        qDebug() << "Fail to found tcName :" << tcName << " or result :" << result;
        return QMap<int, QStringList>();
    }

    QMap<int, QStringList> tcNameDataInfo = QMap<int, QStringList>();
    QMap<int, QMap<int, QString>> tempTcNameDataInfo;
    int searchRowIndex = 0;

    qDebug() << "Found TCName :" << foundSheetIndex << tcName << result << rowInfo;

    const auto sheetData = getData(foundSheetIndex).toList();
    for (const auto& rowDataList : sheetData) {
        QVariantList rowData = rowDataList.toList();
        if ((searchRowIndex >= rowInfo.first) && (searchRowIndex <= rowInfo.second)) {
            QMap<int, QString> columnData;
            for (const auto& columnIndex : columnList) {
                if ((result.size() > 0) && (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))) {
                    // Skip - [reulst value valid] & [columnList contains Result]
                    continue;
                }
                QString dataInfo = rowData.at(columnIndex).toString();
                if (mergeInfoErase) {
                    ivis::common::getRemoved(dataInfo, mergeInfos);
                }
                tcNameDataInfo[columnIndex].append(dataInfo);
                tcNameDataInfo[columnIndex].removeAll("");
                tcNameDataInfo[columnIndex].removeDuplicates();

                columnData[columnIndex] = dataInfo;
            }

            tempTcNameDataInfo[searchRowIndex] = columnData;
        }
        searchRowIndex++;
    }

    QList<QStringList> foundRowData;
    for (auto iter = tempTcNameDataInfo.cbegin(); iter != tempTcNameDataInfo.cend(); ++iter) {
        QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
        for (auto iterData = iter.value().cbegin(); iterData != iter.value().cend(); ++iterData) {
            int columnIndex = iterData.key();
            if (columnIndex < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max)) {
                dataInfo[columnIndex] = iterData.value();
            }
        }
        foundRowData.append(dataInfo);
    }

    bool appendState = false;
    QList<QStringList> tempList;
    QList<QStringList> rowData = convertData;
    convertData.clear();

    for (auto& foundRowInfo : foundRowData) {
        QString caseInfo = foundRowInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
        if (caseInfo.contains(mergeStart)) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(merge)) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(mergeEnd)) {
            tempList.append(foundRowInfo);
            appendState = true;
        } else {
            if (foundRowData.size() == 1) {
                if (rowData.size() == 0) {
                    convertData.append(rowData + foundRowData);
                    break;
                } else {
                    tempList.append(foundRowInfo);
                    appendState = true;
                }
            }
        }

        if (appendState) {
            appendState = false;
            for (auto& rowInfo : rowData) {
                QString rowSignalInfo = rowInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                auto iter = std::remove_if(tempList.begin(), tempList.end(),
                                           [&](const QStringList& row) { return row.contains(rowSignalInfo); });
                tempList.erase(iter, tempList.end());

                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = QString();
                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = QString();
                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = QString();
            }

            tempList = rowData + tempList;
            ivis::common::getRemoved(caseInfo, mergeInfos);

            for (int index = 0; index < tempList.size(); ++index) {
                QString mergeText;
                if (index == 0) {
                    mergeText = mergeStart;
                } else if (index == (tempList.size() - 1)) {
                    mergeText = mergeEnd;
                } else {
                    mergeText = merge;
                }
                tempList[index][static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = (mergeText + caseInfo);
            }
            convertData.append(tempList);
            tempList.clear();
        }
    }

#if 0
    qDebug() << "\n\n\n >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "========================================================================================";
    qDebug() << "isTCNameDataInfo :" << tcName << result << ", ColumnIndex :" << columnList;
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : rowData) {
        qDebug() << "\t RowInfo     :" << dataInfo;
    }
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : foundRowData) {
        qDebug() << "\t FoundInfo   :" << dataInfo;
    }
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : convertData) {
        qDebug() << "\t ConvertInfo :" << dataInfo;
    }
    qDebug() << "========================================================================================";

#if 0
    qDebug() << "isTCNameDataInfo :" << tcName << result << ", ColumnIndex :" << columnList;
    qDebug() << "Sheet :" << foundSheetIndex;
    qDebug() << "RowInfo[TCName] :" << tcNamerowInfo.first << tcNamerowInfo.second;
    qDebug() << "RowInfo[Result] :" << resultRowInfo.first << resultRowInfo.second;
    qDebug() << "TCNameDataInfo  :" << tcName << result;
    qDebug() << "RowData         :" << rowData;
    qDebug() << "FoundData       :" << foundRowData;
    qDebug() << "ConvertData     :" << convertData;
#endif
    qDebug() << "\n <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif

    return tcNameDataInfo;
}

QPair<int, int> ControlExcel::isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2,
                                                const QString& input3, const bool& normal) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    int columnMax = 0;
    QList<QPair<int, QString>> compareDataInfo;

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        // not support sheet : description
        return QPair<int, int>((-1), (-1));
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
               (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName), input1),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal), input2),
        });
    } else {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName), input1),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result), input2),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), input3),
        });
    }

    if (normal == false) {
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), input3),
        });
    }

    QMap<int, QStringList> readColumnDatas;
    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < columnMax) {
            continue;
        }
        for (const auto& dataInfo : compareDataInfo) {
            int readColumnIndex = dataInfo.first;
            QString readColumnText = rowData.at(static_cast<int>(readColumnIndex));
            readColumnDatas[readColumnIndex].append(readColumnText);
        }
    }

    QMap<int, QPair<int, int>> rowIndexs;
    int previousIndex = (-1);
    for (const auto& dataInfo : compareDataInfo) {
        QString compareColumnText = dataInfo.second;
        if (compareColumnText.size() == 0) {
            continue;
        }
        ivis::common::getRemoved(compareColumnText, mergeInfos);
        int columnIndex = dataInfo.first;
        QStringList originColumnText = readColumnDatas[columnIndex];
        QPair<int, int> foundRowInfo((-1), (-1));
        int rowIndex = 0;

        for (const auto& originColumnText : originColumnText) {
            QString removeOriginText = originColumnText;
            ivis::common::getRemoved(removeOriginText, mergeInfos);
            bool state = ivis::common::isCompareString(removeOriginText, compareColumnText);
            // qDebug() << "\t\t State[" << columnIndex << "] :" << state << compareColumnText << removeOriginText
            //          << originColumnText;
            if (state) {
                if (ivis::common::isContainsString(originColumnText, mergeEnd)) {
                    foundRowInfo = QPair<int, int>(foundRowInfo.first, rowIndex);
                    break;
                } else {
                    if (ivis::common::isContainsString(originColumnText, merge) == false) {
                        foundRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
            rowIndex++;
        }

        if (foundRowInfo == QPair<int, int>((-1), (-1))) {
            // qDebug() << "\t Not found matching column["<< columnIndex << "] text :" << compareColumnText << originColumnText;
            break;
        }

        QPair<int, int> previousRowInfo = (rowIndexs.size() == 0) ? (foundRowInfo) : (rowIndexs[previousIndex]);
        bool update = ((foundRowInfo.first >= previousRowInfo.first) && (foundRowInfo.second <= previousRowInfo.second));
        // qDebug() << "\t RowInfo[" << previousIndex << "] :" << update << previousRowInfo << ">" << foundRowInfo;

        if (update) {
            previousIndex = columnIndex;
            rowIndexs[columnIndex] = foundRowInfo;
            // qDebug() << "\t RowInfo[" << columnIndex << "] :" << foundRowInfo;
            // qDebug() << "\t ---------------------------------------------------------------------------------";
        }
    }

    // input3 -> input2 -> input1 순서로 사이즈 > 0 이면, 해당 리스트에서 rowIndex 리턴, 0 이면 초기값 리턴
    QPair<int, int> rowInfo((-1), (-1));
    for (auto iter = compareDataInfo.rbegin(); iter != compareDataInfo.rend(); ++iter) {
        QString columnText = iter->second;
        if (columnText.size() > 0) {
            int columnIndex = iter->first;
            rowInfo = (rowIndexs.contains(columnIndex)) ? (rowIndexs[columnIndex]) : (QPair<int, int>((-1), (-1)));
            break;
        }
    }
    // qDebug() << "\t isContainsRowInfo :" << columnMax << input1 << input2 << input3;
    // qDebug() << "\t\t RowInfo :"  << rowInfo << ", All :" << rowIndexs;

    return rowInfo;
}

QList<QStringList> ControlExcel::isRowDataInfo(const int& sheetIndex, const QPair<int, int>& rowInfo,
                                               const QPair<int, int>& columnInfo) {
    const int columnStart = columnInfo.first;
    const int columnEnd = columnInfo.second + 1;

    int columnMax = 0;
    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max);
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
               (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);
    } else {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    }

    QList<QStringList> dataInfo;
    int rowIndex = 0;
    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < columnMax) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        QStringList data(columnMax);
        for (int columnIndex = columnStart; columnIndex < columnEnd; ++columnIndex) {
            data[columnIndex] = rowData[columnIndex];
        }

        if ((rowIndex >= rowInfo.first) && (rowIndex <= rowInfo.second)) {
            dataInfo.append(data);
            if (rowIndex == rowInfo.second) {
                break;
            }
        }
        rowIndex++;
    }
    // qDebug() << "isRowDataInfo :" << sheetIndex << rowInfo << columnInfo << dataInfo.size();
    return dataInfo;
}

QList<QStringList> ControlExcel::isDataInfo(const int& sheetIndex, const QString& tcName, const QString& result,
                                            const QString& caseInfo, const QPair<int, int>& columnInfo,
                                            const int& checkColumnIndex) {
    QList<QStringList> dataInfo;
    QPair<int, int> rowInfo = isContainsRowInfo(sheetIndex, tcName, result, caseInfo);

    if ((rowInfo.first >= 0) && (rowInfo.second >= 0)) {
        dataInfo = isRowDataInfo(sheetIndex, rowInfo, columnInfo);
    }

    // Output, Config : 시그널 공백인 경우 제거
    if (checkColumnIndex > 0) {  // signal data : null -> remove
        QList<QStringList> tempDataInfo;
        for (auto& info : dataInfo) {
            if (info.size() < checkColumnIndex) {
                // qDebug() << "Fail to sheet data list size :" << rowData.size();
                continue;
            }

            QString signal = info.at(checkColumnIndex);
            if (signal.size() > 0) {
                tempDataInfo.append(info);
            }
        }
        dataInfo = tempDataInfo;
    }

    return dataInfo;
}

QList<QStringList> ControlExcel::isInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& result,
                                                 const QString& caseInfo) {
    const QPair<int, int> columnInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName),
                                     static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
    QList<QStringList> dataInfo = isDataInfo(sheetIndex, tcName, result, caseInfo, columnInfo, 0);

    // if (dataInfo.size() > 0) {
    //     qDebug() << "\t isInputDataInfo :" << dataInfo;
    //     qDebug() << "\n";
    // }

    return dataInfo;
}

QList<QStringList> ControlExcel::isOutputDataInfo(const int& sheetIndex, const QString& tcName, const QString& result) {
    const QPair<int, int> columnInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
                                     static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    QList<QStringList> dataInfo = isDataInfo(sheetIndex, tcName, result, QString(), columnInfo, columnInfo.first);

    // if (dataInfo.size() > 0) {
    //     qDebug() << "\t isOutputDataInfo :" << dataInfo;
    //     qDebug() << "\n";
    // }

    return dataInfo;
}

QList<QStringList> ControlExcel::isConfigDataInfo(const int& sheetIndex, const QString& tcName, const QString& result) {
    const QPair<int, int> columnInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal),
                                     static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
    QList<QStringList> dataInfo = isDataInfo(sheetIndex, tcName, result, QString(), columnInfo, columnInfo.first);

    if (dataInfo.size() == 0) {
        int descSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
        if ((sheetIndex >= ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) &&
            (sheetIndex < ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax)) {
            descSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
        }

        QPair<int, int> descRowInfo(0, 1);
        QPair<int, int> descColumnInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal),
                                       static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data));
        QList<QStringList> descDataInfo = isRowDataInfo(descSheetIndex, descRowInfo, descColumnInfo);
        for (const auto& descData : descDataInfo) {
            QStringList data(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
            QString configSignal = descData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal));
            QString configData = descData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data));

            data[static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal)] = configSignal;
            data[static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data)] = configData;
            dataInfo.append(data);
        }
    }

    // if (dataInfo.size() > 0) {
    //     qDebug() << "\t isConfigDataInfo :" << dataInfo;
    //     qDebug() << "\n";
    // }

    return dataInfo;
}

QList<QStringList> ControlExcel::isConfigDataInfo(const QString& configName) {
    const QPair<int, int> columnInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName),
                                     static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs;
    QString foundConfigName = (configName.size() == 0) ? (QString("Default")) : (configName);
    QList<QStringList> dataInfo = isDataInfo(sheetIndex, foundConfigName, QString(), QString(), columnInfo, false);

    // if (dataInfo.size() > 0) {
    //     qDebug() << "\t isConfigDataInfo :" << dataInfo;
    //     qDebug() << "\n";
    // }

    return dataInfo;
}

QStringList ControlExcel::isSignalValueEnum(const bool& toEnum, const QString& signalName) {
    QString matchingValue;    // not used
    QStringList valueEnum = isConvertedSignalData(toEnum, signalName, QStringList(), matchingValue);
    // qDebug() << "isSignalValueEnum :" << toEnum << signalName << valueEnum;
    return valueEnum;
}

QStringList ControlExcel::isConvertedSignalData(const bool& toEnum, const QString& signalName, const QStringList& valueEnum,
                                                QString& matchingValue) {
    int signalType = isSignalType(signalName);
    bool vehicleState = ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
                         (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)));
    int enumIndex = (vehicleState) ? (1) : (0);
    int valueIndex = (vehicleState) ? (0) : (1);

    QStringList currentValueEnum = valueEnum;
    if (currentValueEnum.size() == 0) {
        QStringList signalData = QStringList();
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
        QMap<int, QStringList> dataInfo = isSignalDataInfo(signalName, signalData, QString(), dataType);
        currentValueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    }

    QStringList convertDataInfo;
    QString compareMatchingValue = matchingValue;
    matchingValue.clear();
    for (const auto& v : currentValueEnum) {
        QString tempValueEnum = v;
        QStringList splitData = tempValueEnum.remove("\"").split(":");
        if (splitData.size() < 2) {
            continue;
        }
        QString data = ((toEnum) ? (splitData.at(enumIndex)) : (splitData.at(valueIndex)));
        convertDataInfo.append(data);
        if (compareMatchingValue.size() > 0) {
            QString matchingData = ((toEnum) ? (splitData.at(valueIndex)) : (splitData.at(enumIndex)));
            // qDebug() << "Matching :" << matchingData << compareMatchingValue << data;
            if (matchingData.compare(compareMatchingValue) == false) {
                matchingValue = data;
            }
        }
    }
    return convertDataInfo;
}

QStringList ControlExcel::isDescriptionDataInfo() {
    const int descriptSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    // const int descriptSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;

    QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max));
    int rowIndex = 0;
    for (const auto& rowDataList : getData(descriptSheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max)) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        for (int columnIndex = 0; columnIndex < dataInfo.size(); ++columnIndex) {
            dataInfo[columnIndex] = rowData[columnIndex];
        }

        rowIndex++;
    }
    return dataInfo;
}

bool ControlExcel::isPreconditionMaxValue(const QString& signalName, const int& dataType, const int& keywordType,
                                          const QStringList& inputData, const QStringList& valueEnum) {
    // qDebug() << "\t isPreconditionMaxValue :" << dataType << keywordType << inputData.size() << valueEnum.size();
    if (signalName.trimmed().startsWith(SFC_IGN_ELAPSED)) {
        return false;
    }

    if ((dataType != static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) &&
        (dataType != static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64))) {
        return false;
    }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        return false;
    }

    if (inputData.size() != 1) {
        return false;
    }

    if (valueEnum.size() > 0) {
        return false;
    }

    return true;
}

int ControlExcel::isConvertedKeywordType(const bool& toCustom, const int& keywordType) {
    int convertKeywordType = keywordType;

    if (toCustom) {
        if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow);
            // } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn)) {
            //     keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn);
        } else {
        }
    } else {
        if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow);
            // } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Ign)) {
            //     keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn);
        } else {
        }
    }

    return convertKeywordType;
}

int ControlExcel::isSignalType(const QString& signalName) {
    int signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid);

#if 1
    if (signalName.trimmed().startsWith("SFC.")) {
        if (signalName.trimmed().startsWith("SFC.Event.")) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent);
        } else if ((signalName.trimmed().startsWith("SFC.Extension.")) || (signalName.trimmed().startsWith("SFC.Private."))) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException);
        } else {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc);
        }
    } else if (signalName.trimmed().startsWith("Vehicle.")) {
        if (signalName.trimmed().startsWith("Vehicle.System.")) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem);
        } else {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle);
        }
    } else if (signalName.trimmed().startsWith("SFCFuel.Fuel_System.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal);
    } else if (signalName.trimmed().startsWith("SFC_Common_Enum")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon);
    } else {
    }
#else
    if (signalName.trimmed().startsWith("SFCFuel.Fuel_System.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal);
    } else if (signalName.trimmed().startsWith("SFC.Event.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent);
    } else if (signalName.trimmed().startsWith("SFC_Common_Enum")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon);
    } else if ((signalName.trimmed().startsWith("SFC.Extension.")) || (signalName.trimmed().startsWith("SFC.Private."))) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException);
    } else if (signalName.trimmed().startsWith("SFC.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc);
    } else if (signalName.trimmed().startsWith("Vehicle.System.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem);
    } else if (signalName.trimmed().startsWith("Vehicle.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle);
    } else {
    }
#endif

    return signalType;
}


class SheetDataInfo {
public:
    SheetDataInfo(const QString& tcName, const QString& vehicleType, const QString& configName) {
        updateHeaderInfo(tcName, vehicleType, configName);
    }
    SheetDataInfo() = default;
    SheetDataInfo(const SheetDataInfo& other) = default;
    SheetDataInfo& operator=(const SheetDataInfo& other) = default;

    void updateSheetDataInfo(const QString& resultName, const QString& caseName, const QStringList& inputSignalList,
                        const QStringList& inputDataList, const int& appendIndex = (-1)) {
        updateCaseInputInfo(caseName, inputSignalList, inputDataList);
        updateCaseInfo(caseName, appendIndex);
        updateResultInfo(resultName, caseName);
    }
    void updateSheetDataInfo(const QString& caseName, const QStringList& inputSignalList, const QStringList& inputDataList,
                        const int& appendIndex = (-1)) {
        updateCaseInputInfo(caseName, inputSignalList, inputDataList);
        updateCaseInfo(caseName, appendIndex);
        // updateResultInfo(QString(), caseName);
    }
    QList<QStringList> isSheetDataInfo() {
        QList<QStringList> sheetData;
        return sheetData;
    }
    int isCaseIndex(const QString& caseName) {
        for (auto iter = mCaseInfo.cbegin(); iter != mCaseInfo.cend(); ++iter) {
            if (caseName.compare(iter.value()) == false) {
                return iter.key();
            }
        }
        return mCaseInfo.size();
    }
    void printData() {
        for (auto iter = mCaseInfo.cbegin(); iter != mCaseInfo.cend(); ++iter) {
            QString caseName = iter.value();
            qDebug() << "CaseInputInfo[" << iter.key() << "] :" << mCaseInputInfo[caseName].first.size() << caseName;
        }
        qDebug() << "\n";
    }

private:
    void updateHeaderInfo(const QString& tcName, const QString& vehicleType, const QString& configName) {
        mTCName = tcName;
        mVehicleType = vehicleType;
        mConfigName = configName;
    }
    void updateCaseInputInfo(const QString& caseName, const QStringList& inputSignalList, const QStringList& inputDataList) {
        mCaseInputInfo[caseName] = QPair<QStringList, QStringList>(inputSignalList, inputDataList);
    }
    void updateCaseInfo(const QString& caseName, const int& appendIndex) {
        if (appendIndex == (-1)) {
            mCaseInfo[mCaseInfo.size()] = caseName;
        } else {
            // int caseIndex = isCaseIndex(caseName);
            QMap<int, QString> caseInfo;
            for (auto iter = mCaseInfo.cbegin(); iter != mCaseInfo.cend(); ++iter) {
                if (appendIndex == iter.key()) {
                    caseInfo[appendIndex] = caseName;
                }
                caseInfo[caseInfo.size()] = iter.value();
            }
            mCaseInfo = caseInfo;
        }
    }
    void updateResultInfo(const QString& resultName, const QString& caseName) {
    }

#if 0
    void updateResultInfo() {
        // Row 기준으로 mResultInfo 업데이트
        mResultInfo.clear();

        int rowIndex = 0;
        for (const auto& caseName : mCaseInfo) {
            // Result 정보를 추출 (예시: CaseName에 따른 ResultName 추출 규칙이 있다고 가정)
            QString resultName = caseName.section("_", 0, 0);    // "_" 기준으로 첫 번째 부분을 ResultName으로 간주
            mResultInfo[rowIndex] = resultName;
            rowIndex++;
        }
    }
    void updateDataInfo() {
        // Row 기준으로 mDataInfo 업데이트
        mDataInfo.clear();

        QMap<QString, int> resultNameToIndex;    // ResultName -> ResultIndex 매핑

        // mResultInfo 기반으로 ResultName -> ResultIndex 매핑 생성
        for (auto it = mResultInfo.begin(); it != mResultInfo.end(); ++it) {
            resultNameToIndex[it.value()] = it.key();
        }

        for (auto it = mCaseInfo.begin(); it != mCaseInfo.end(); ++it) {
            int caseIndex = it.key();
            const QString& caseName = it.value();

            // CaseName에 대응하는 ResultIndex 가져오기
            QString resultName = caseName.section("_", 0, 0);    // "_" 기준으로 첫 번째 부분을 ResultName으로 간주
            int resultIndex = resultNameToIndex.value(resultName, -1);    // 기본값 -1 (존재하지 않을 경우)

            if (resultIndex != -1) {
                // mDataInfo에 ResultIndex에 대응하는 CaseIndex 리스트 추가
                mDataInfo[resultIndex].append(QString::number(caseIndex));
            }
        }
    }
#endif

private:
    QString mTCName;
    QString mVehicleType;
    QString mConfigName;

    // QMap<CaseName, QPair<InputSignal, InputData>>
    QMap<QString, QPair<QStringList, QStringList>> mCaseInputInfo;

    // QMap<CaseIndex, QString<CaseName>>
    QMap<int, QString> mCaseInfo;

    // QMap<ResultIndex, QString<ResultName>>
    QMap<int, QString> mResultInfo;

    // QMap<ResultIndex, QStringList<CaseIndex>>
    QMap<int, QStringList> mDataInfo;
};


QMap<int, QSet<QPair<QString, int>>> ControlExcel::isSheetMergeInfo(const int& sheetIndex) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QVariantList sheetData = getData(sheetIndex).toList();
    QMap<int, QSet<QPair<QString, int>>> mergeInfo;
    int rowIndex = 0;

#if 1
    int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData) + 1;

    QMap<int, QStringList> tempSheetData;

    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        // columnMax = rowData.size();
        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            tempSheetData[columnIndex].append(readColumnText);
        }
    }

    // isContainsRowInfo
    QMap<int, QPair<int, QString>> caseRowInfo;
    QPair<int, QString> foundRow;
    for (const auto& text : tempSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]) {
        if (ivis::common::isContainsString(text, mergeStart)) {
            QString caseName = text;
            ivis::common::getRemoved(caseName, mergeInfos);
            foundRow = QPair<int, QString>(rowIndex, caseName);
        } else if (ivis::common::isContainsString(text, merge)) {
        } else if (ivis::common::isContainsString(text, mergeEnd)) {
            int count = rowIndex - foundRow.first + 1;
            caseRowInfo[caseRowInfo.size()] = QPair<int, QString>(count, foundRow.second);
        } else {
            caseRowInfo[caseRowInfo.size()] = QPair<int, QString>(1, text);
        }
        rowIndex++;
    }

    QStringList inputSignalList = tempSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)];
    QStringList inputDataList = tempSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)];
    int rowStart = 0;
    int rowEnd = 0;
    SheetDataInfo sheetDataInfo;

    for (auto iter = caseRowInfo.cbegin(); iter != caseRowInfo.cend(); ++iter) {
        QPair<int, QString> caseInfo = iter.value();
        rowEnd = rowStart + caseInfo.first;
        QStringList inputSignals = (rowEnd < inputSignalList.size()) ? (inputSignalList.mid(rowStart, rowEnd)) : (QStringList());
        QStringList inputDatas = (rowEnd < inputDataList.size()) ? (inputDataList.mid(rowStart, rowEnd)) : (QStringList());
        if (inputSignals.size() > 0) {
            sheetDataInfo.updateSheetDataInfo(caseInfo.second, inputSignals, inputDatas);
        }
        rowStart = rowEnd;
    }

    sheetDataInfo.printData();

    QString foundCaseName = tempSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].at(3);
    ivis::common::getRemoved(foundCaseName, mergeInfos);
    int caseIndex = sheetDataInfo.isCaseIndex(foundCaseName);
    sheetDataInfo.updateSheetDataInfo("TEST_CASE", {"Signal1", "Signal2", "Signal3"}, {"3", "2", "1"}, caseIndex);
    sheetDataInfo.printData();

#else
    QMap<int, QPair<QString, int>> foundRowInfo;
    int columnMax = 0;

    if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
        // columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test) + 1;
    } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
        // columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName) + 1;
    } else {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case) + 1;
    }

    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        // columnMax = rowData.size();
        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            if (ivis::common::isContainsString(readColumnText, mergeStart)) {
                ivis::common::getRemoved(readColumnText, mergeInfos);
                foundRowInfo[columnIndex] = QPair<QString, int>(readColumnText, rowIndex);

                qDebug() << "\t" << columnIndex << ". Start  :" << rowIndex << readColumnText;
            } else if (ivis::common::isContainsString(readColumnText, mergeEnd)) {
                int count = rowIndex - foundRowInfo[columnIndex].second + 1;
                mergeInfo[columnIndex].insert(QPair<QString, int>(foundRowInfo[columnIndex].first, count));

                qDebug() << "\t" << columnIndex << ". End    :" << count << rowIndex << foundRowInfo[columnIndex].first;
            } else if (ivis::common::isContainsString(readColumnText, merge)) {
            } else {
                ivis::common::getRemoved(readColumnText, mergeInfos);
                mergeInfo[columnIndex].insert(QPair<QString, int>(readColumnText, 1));

                qDebug() << "\t" << columnIndex << ". Normal :" << 1 << rowIndex << readColumnText;
            }
        }
        rowIndex++;
    }

    for (auto iter = mergeInfo.cbegin(); iter != mergeInfo.cend(); ++iter) {
        qDebug() << sheetIndex << ". MergeInfo[" << iter.key() << "] :" << iter.value();
    }
    qDebug() << "\n";
#endif

    return mergeInfo;
}

QPair<QStringList, QStringList> ControlExcel::isCheckExceptionValueEnum(const QString& signalName,
                                                                        const QMap<int, QStringList>& dataInfo) {
    const QString originTimeOut = isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout));
    const QString convertTimeOut = QString("timeout");
    const QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    QStringList originData = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    QPair<QStringList, QStringList> exceptionData(originData, originData);

    int signalType = isSignalType(signalName);
    if ((signalType > static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) &&
        (signalType < static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle))) {
        // not support sfc signal
        return exceptionData;
    }
    if (originData.contains(originTimeOut) == false) {
        return exceptionData;
    }
    for (auto value : valueEnum) {
        if (value.contains(originTimeOut)) {
            return exceptionData;
        }
    }

    const int startIndex = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV;
    const QStringList keywordList = QStringList({"TIMEOUT:", "CRC:", "ELSE:"});
    QString matchingValue;

    for (const auto& keyword : keywordList) {
        for (int index = startIndex; index < ivis::common::InputDataTypeEnum::InputDataTypeMax; ++index) {
            for (auto& matchingTable : dataInfo[index]) {
                if (matchingTable.contains(keyword)) {
                    matchingValue = matchingTable.remove(keyword);
                    break;
                }
            }
        }
    }

    QString matchingEnum = matchingValue;
    isConvertedSignalData(true, signalName, valueEnum, matchingEnum);
    if (matchingEnum.size() > 0) {
        originData.removeAll(originTimeOut);
        originData.removeAll(convertTimeOut);
        originData.removeAll(matchingEnum);
    } else {
        for (auto& origin : originData) {
            if (origin.contains(originTimeOut)) {
                origin.replace(originTimeOut, convertTimeOut);
                // qDebug() << signalName << ":" << originTimeOut << "->" << convertTimeOut;
            }
        }
    }

    exceptionData = QPair<QStringList, QStringList>(QStringList({matchingEnum}), originData);

#if 0
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "isCheckExceptionValueEnum :" << signalName;
    qDebug() << "\t Matching        :" << matchingValue << matchingEnum;
    qDebug() << "\t InputData       :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    qDebug() << "\t OriginData      :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    qDebug() << "\t ExceptionFirst  :" << exceptionData.first;
    qDebug() << "\t ExceptionSecond :" << exceptionData.second;
    qDebug() << "\t ValueEnum       :" << valueEnum;
    // qDebug() << "\t 1 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV];
    // qDebug() << "\t 2 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV];
    // qDebug() << "\t 3 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV];
    // qDebug() << "\t 4 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV];
    // qDebug() << "\t 5 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV];
    // qDebug() << "\t 6 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif

    return exceptionData;
}

QMap<QString, SignalDataInfo> ControlExcel::isMatchingSignalDataInfo(const int& dataInfoType, const int& sheetIndex,
                                                                     const QStringList& columnDataInfo) {
    if (columnDataInfo.size() != 3) {
        qDebug() << "Fail to column data info size :" << columnDataInfo.size();
        return QMap<QString, SignalDataInfo>();
    }
    QString tcNameInfo = columnDataInfo.at(0);
    QString resultInfo = columnDataInfo.at(1);
    QString caseInfo = columnDataInfo.at(2);

    QList<QStringList> readDataInfo;
    QList<QStringList> removeDataInfo;
    int signalIndex = 0;
    int signalDataIndex = 0;
    int etcDataIndex = 0;
    bool tcNameBaseLine = (dataInfoType == (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputTCName)));
    bool caseBaseLine = (dataInfoType == (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCase)));
    bool allDataContains = (tcNameBaseLine || caseBaseLine);

    switch (dataInfoType) {
        case (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputTCName)):
        case (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCaseRemove)): {
            signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            signalDataIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData);
            readDataInfo = isInputDataInfo(sheetIndex, tcNameInfo, QString(), QString());
            if (dataInfoType == (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCaseRemove))) {
                removeDataInfo = isInputDataInfo(sheetIndex, tcNameInfo, resultInfo, caseInfo);
            }
            break;
        }
        case (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCase)): {
            signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            signalDataIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData);
            readDataInfo = isInputDataInfo(sheetIndex, tcNameInfo, resultInfo, caseInfo);
            break;
        }
        case (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Ouput)): {
            signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal);
            signalDataIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue);
            etcDataIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize);
            readDataInfo = isOutputDataInfo(sheetIndex, tcNameInfo, resultInfo);
            break;
        }
        case (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Config)): {
            signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal);
            signalDataIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data);
            readDataInfo = isConfigDataInfo(sheetIndex, tcNameInfo, resultInfo);
            break;
        }
        default: {
            break;
        }
    }
    // qDebug() << "isMatchingSignalDataInfo :" << sheetIndex << readDataInfo.size() << removeDataInfo.size() << signalIndex
    //          << signalDataIndex << ", allDataContains :" << allDataContains;

    if ((signalIndex == 0) && (signalDataIndex == 0)) {
        return QMap<QString, SignalDataInfo>();
    }

    // Custom Keyword String
    QStringList keywordString;
    for (int index = 0; index <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        for (const auto& infoPair : isKeywordPatternInfo(index)) {
            keywordString.append(infoPair.first);
        }
    }

    QMap<QString, QStringList> tempReadDataInfo;
    QMap<QString, QString> etcDataInfo;
    for (const auto& readData : readDataInfo) {
        QString singalInfo = readData.at(signalIndex);
        if (singalInfo.size() == 0) {
            continue;
        }
        QString dataInfoList = readData.at(signalDataIndex);
        QStringList dataInfo = dataInfoList.remove(" ").split(",");
        tempReadDataInfo[singalInfo].append(dataInfo);
        if (etcDataIndex > 0) {
            etcDataInfo[singalInfo].append(readData.at(etcDataIndex));
        }
        // qDebug() << "\t [" << singalInfo << "] :" << dataInfo;
    }

    // qDebug() << "=================================================================================================\n\n";
    QMap<QString, QStringList> matchingDataInfo;

    for (auto iter = tempReadDataInfo.cbegin(); iter != tempReadDataInfo.cend(); ++iter) {
        QString singalInfo = iter.key();
        QStringList dataInfo = iter.value();
        dataInfo.removeAll("");
        dataInfo.removeDuplicates();

        if (removeDataInfo.size() > 0) {
            for (const auto& removeData : removeDataInfo) {
                QString removeSingalInfo = removeData.at(signalIndex);
                if (removeSingalInfo.compare(singalInfo) == false) {
                    singalInfo.clear();
                    break;
                }
            }
        }

        if (singalInfo.size() > 0) {
            matchingDataInfo[singalInfo] = dataInfo;
        }
    }

    // SignalData 구성
    QMap<QString, SignalDataInfo> signalDataInfo;

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

    QStringList ignOriginData;
    for (auto iter = matchingDataInfo.cbegin(); iter != matchingDataInfo.cend(); ++iter) {
        QString signalName = iter.key();
        QStringList signalData = iter.value();
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
        QMap<int, QStringList> dataInfo = isSignalDataInfo(signalName, signalData, vehicleType, dataType);
        bool initialize = (etcDataInfo[signalName].size() > 0);
        bool ignElaspedSingal = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);
        int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
        QStringList originData;
        QStringList valueEnum;
        QStringList convertData;
        QStringList notUsedEnum;
        QStringList precondition;

        if (dataInfo.size() == 0) {
            if (ivis::common::isCompareString(signalName, QString("collect"))) {
                dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
                originData = signalData;
                convertData = originData;
                signalDataInfo[signalName] = SignalDataInfo(signalName, dataType, initialize, keywordType, originData,
                                                            convertData, valueEnum, notUsedEnum, precondition);
            }
        } else {
            QString checkText = (signalData.size() == 0) ? (QString()) : (signalData.at(0));  // 0 : 키워드 포함 데이터
            QString matchingValue;    // not used
            originData = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
            valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
            keywordType = isKeywordType(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max), checkText);
            convertData = originData;
            notUsedEnum = isConvertedSignalData(true, signalName, valueEnum, matchingValue);
            precondition.clear();

            // ConvertData, NotUsedEnum
            for (auto& data : convertData) {
                for (const auto& removeKeyword : keywordString) {  // convertData 에서 키워드 정보만 삭제
                    data.remove(removeKeyword);
                }
                data.remove("[");
                data.remove("]");
                notUsedEnum.removeAll(data);
            }

            if (ignElaspedSingal) {
                QPair<int, int> ignInfo = isIGNElapsedType(signalName);
                signalName = SFC_IGN_ELAPSED;
                if (caseBaseLine) {
                    convertData = QStringList({QString("%1").arg(ignInfo.first)});
                    precondition = QStringList({QString("%1").arg(ignInfo.second)});
                } else if (tcNameBaseLine) {
                    ignOriginData.append(QString("%1").arg(ignInfo.first));
                } else {
                    continue;
                }
            } else if (allDataContains) {
                if (isPreconditionMaxValue(signalName, dataType, keywordType, convertData, valueEnum)) {
                    QString preconditionMaxValue = QString("%1").arg(static_cast<quint64>(UINT32_MAX) + 1);
                    precondition = QStringList({preconditionMaxValue});
                } else {
                    switch (keywordType) {
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotUsed): {
                            originData.clear();
                            convertData.clear();
                            notUsedEnum.clear();
                            precondition.clear();
                            break;
                        }
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger): {
                            convertData.clear();
                            precondition = notUsedEnum;
                            break;
                        }
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual):
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined): {
                            int splitSize = convertData.size() * 0.5;
                            precondition = convertData.mid(0, splitSize);
                            convertData = convertData.mid(splitSize, convertData.size());
                            break;
                        }
                        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn): {
                            break;
                        }
                        default: {
                            precondition = notUsedEnum;
                            break;
                        }
                    }
                    notUsedEnum.clear();
                    keywordType = isConvertedKeywordType(true, keywordType);

                    // Data Check : MESSAGE_TIMEOUT, timeout
#if 1
                    // qDebug() << "isMatchingSignalDataInfo :" << dataInfoType << allDataContains;
                    QPair<QStringList, QStringList> exceptionData = isCheckExceptionValueEnum(signalName, dataInfo);
                    if (exceptionData.first != exceptionData.second) {
                        convertData = exceptionData.first;
                        precondition = exceptionData.second;
                    }
#else
                    originData = isCheckExceptionValueEnum(signalName, dataInfo);
                    if (originData.contains("timeout") && originData.size() == 1) {
                        convertData = originData;
                    }
#endif
                }
#if 0  // NotUsedEnum 인 경우 ValueEnum 이 있는경우 추가 않하는 코드
            } else {
                bool notExistData = ((valueEnum.size() > 1) && (notUsedEnum.size() == 0) &&
                                     ((dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) ||
                                      (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64))));
                if (notExistData) {
                    continue;
                }
#endif
            }
            signalDataInfo[signalName] = SignalDataInfo(signalName, dataType, initialize, keywordType, originData, convertData,
                                                        valueEnum, notUsedEnum, precondition);
        }
    }

    if (ignOriginData.size() > 0) {  // ign 전체 정보 재구성
        QPair<QStringList, QStringList> ingDataIfo = isConvertedIGNElapsedInfo(ignOriginData);
        int ignDataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
        QStringList ignConvertData = ingDataIfo.first;
        QStringList ignPrecondition = ingDataIfo.second;
        signalDataInfo[SFC_IGN_ELAPSED] = SignalDataInfo(SFC_IGN_ELAPSED, ignDataType, false, 0, ignOriginData, ignConvertData,
                                                         QStringList(), QStringList(), ignPrecondition);
    }

#if 0
    qDebug() << "=================================================================================================\n\n";
    for (auto iter = signalDataInfo.cbegin(); iter != signalDataInfo.cend(); ++iter) {
        SignalDataInfo dataInfo = iter.value();
        qDebug() << "[" << iter.key() << "] :" << dataInfoType;
        qDebug() << "\t DataType     :" << dataInfo.isDataType();
        qDebug() << "\t Initialize   :" << dataInfo.isInitialize();
        qDebug() << "\t KeywordType  :" << dataInfo.isKeywordType();
        qDebug() << "\t OriginData   :" << dataInfo.isOriginData().size() << dataInfo.isOriginData();
        qDebug() << "\t ConvertData  :" << dataInfo.isConvertData().size() << dataInfo.isConvertData();
        qDebug() << "\t ValueEnum    :" << dataInfo.isValueEnum().size() << dataInfo.isValueEnum();
        qDebug() << "\t NotUsedEnum  :" << dataInfo.isNotUsedEnum().size() << dataInfo.isNotUsedEnum();
        qDebug() << "\t Precondition :" << dataInfo.isPrecondition().size() << dataInfo.isPrecondition();
        qDebug() << "\n\n";
    }
    qDebug() << "=================================================================================================\n\n";
#endif

    return signalDataInfo;
}

QMap<QString, SignalDataInfo> ControlExcel::isInputSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo,
                                                                  const bool& caseRemove) {
    if (columnDataInfo.size() != 3) {
        qDebug() << "Fail to column data info size :" << columnDataInfo.size();
        return QMap<QString, SignalDataInfo>();
    }
    int dataInfoType = static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Invalid);
    QString tcNameInfo = columnDataInfo.at(0);
    QString resultInfo = columnDataInfo.at(1);
    QString caseInfo = columnDataInfo.at(2);

    if (tcNameInfo.size() > 0) {
        if (caseRemove) {
            dataInfoType = static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCaseRemove);
        } else {
            if ((resultInfo.size() == 0) && (caseInfo.size() == 0)) {
                dataInfoType = static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputTCName);
            } else if ((resultInfo.size() > 0) && (caseInfo.size() > 0)) {
                dataInfoType = static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputCase);
            } else {
            }
        }
    }
    if (dataInfoType == static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Invalid)) {
        return QMap<QString, SignalDataInfo>();
    }

    QMap<QString, SignalDataInfo> signalDataInfo = isMatchingSignalDataInfo(dataInfoType, sheetIndex, columnDataInfo);

    // case - other 가 존재하지 않는 경우 tcName 기준 리턴 null
    if (dataInfoType == static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::InputTCName)) {
        SheetConfigurationInfo sheetDataList = isSheetConfigurationInfo(sheetIndex);
        QString otherStr = isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
        if (sheetDataList.isSheetContainsCases(QStringList({otherStr})) == false) {
            signalDataInfo.clear();
        }
    }

    // for (auto iter = signalDataInfo.cbegin(); iter != signalDataInfo.cend(); ++iter) {
    //     qDebug() << "InputSignal :" << iter.key();
    // }

    return signalDataInfo;
}
QMap<QString, SignalDataInfo> ControlExcel::isOutputSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo) {
    int dataInfoType = (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Ouput));
    QMap<QString, SignalDataInfo> signalDataInfo = isMatchingSignalDataInfo(dataInfoType, sheetIndex, columnDataInfo);

    // for (auto iter = signalDataInfo.cbegin(); iter != signalDataInfo.cend(); ++iter) {
    //     qDebug() << "OutputSignal :" << iter.key();
    // }

    return signalDataInfo;
}

QMap<QString, SignalDataInfo> ControlExcel::isConfigSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo) {
    int dataInfoType = (static_cast<int>(ivis::common::DataInfoTypeEnum::DataInfoType::Config));
    QMap<QString, SignalDataInfo> signalDataInfo = isMatchingSignalDataInfo(dataInfoType, sheetIndex, columnDataInfo);
    return signalDataInfo;
}

SheetConfigurationInfo ControlExcel::isSheetConfigurationInfo(const int& sheetIndex) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    SheetConfigurationInfo sheetInfoList;
    QString tcName;
    QString result;
    QStringList caseList;

    int updateIndex = 0;
    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
            continue;
        }

        bool update = false;
        // TCName
        QString readText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
        if (ivis::common::isContainsString(readText, merge) == false) {
            tcName = readText;  // TCName : Merge 미포함 -> MergeStart 포함 or Merge, MergeEnd 미포함
            update = ((ivis::common::isContainsString(readText, mergeStart)) == false);  // TCName : Merge 미포함
        }
        // Result
        readText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
        if (ivis::common::isContainsString(readText, mergeEnd)) {
            update = true;  // TCName : MergeEnd 포함
        } else {
            if (ivis::common::isContainsString(readText, merge) == false) {
                result = readText;
            }
        }
        // Case
        readText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
        if (ivis::common::isContainsString(readText, merge) == false) {
            caseList.append(readText);
        }

        if (update) {
            ivis::common::getRemoved(tcName, mergeInfos);
            ivis::common::getRemoved(result, mergeInfos);
            for (auto& cases : caseList) {
                ivis::common::getRemoved(cases, mergeInfos);
            }
            caseList.removeDuplicates();

            // qDebug() << updateIndex++ << ". Update :" << tcName << result << caseList;
            sheetInfoList.updateSheetConfigurationInfo(tcName, result, caseList);
            result.clear();
            caseList.clear();
        }
    }

    return sheetInfoList;
}

void ControlExcel::updateAutoCompleteSignal(const QString& signalName, const QString& vehicleType, const int& columnIndex) {
    QStringList signalData = QStringList();
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
    QMap<int, QStringList> dataInfo = isSignalDataInfo(signalName, signalData, vehicleType, dataType);
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
        if (tcName.compare(signalName) == false) {
            update = true;
            break;
        }
    }

    if (update) {
        QList<int> columnList = QList<int>({
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
        });
        QList<QStringList> convertData = QList<QStringList>();
        QMap<int, QStringList> dataInfo = isTCNameDataInfo(signalName, QString(), columnList, false, true, convertData);
        QStringList suggestionsDataInfo = dataInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)];
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

    int keywordType = isKeywordType(signalIndex, signalName);
    int signalType = isSignalType(signalName);

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
    QString fileName = isSfcFileInfo(moduleName);

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

    updateNodeAddress(false, tcNameList, configNameList);
}

void ControlExcel::updateInputDataValidation(const QVariantList& cellDataInfo) {
    if (cellDataInfo.size() != 5) {
        qDebug() << "Fail to cell data info size :" << cellDataInfo.size();
    }

    QString signalName = cellDataInfo.at(0).toString();
    QString inputData = cellDataInfo.at(1).toString();
    int sheetIndex = cellDataInfo.at(2).toInt();
    int rowIndex = cellDataInfo.at(3).toInt();
    int columnIndex = cellDataInfo.at(4).toInt();
    int signalType = isSignalType(signalName);

    qDebug() << "updateInputDataValidation :" << signalName << inputData;

    if (inputData.size() == 0) {
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
    QMap<int, QStringList> dataInfo = isSignalDataInfo(signalName, signalData, vehicleType, dataType);
    int keywordType = isKeywordType(columnIndex, signalName);

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
            if (splitData.at(validDataIndex).toUpper().compare(data.toUpper()) == false) {
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
    ivis::common::CheckTimer checkTimer;

    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeCreateExcelSheeet).toBool() == false) {
        qDebug() << "No exist excel sheet !!!!!!";
        return;
    }

    if (isExcelDataValidation() == false) {
        qDebug() << "Fail to excel data validation.";
        return;
    }

    // NOTE(csh): [Sheet] Keyword 기능 수행(row data append) -> 나머지 Keyword 기능 수행(cell data changed) + 001 excel 파일 생성
    if (replaceGenDataInfo() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_001";
            if (writeExcelSheet(filePath, true)) {
                QString dirPath = sytemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }
    }
    checkTimer.check("updateGenDataInfo : Convert.excel_001");

    // NOTE(csh): 최종 signal 조합 set 구성(row data append) + 002 excel 파일 생성
    if (appendConvertAllTCSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_002";
            if (writeExcelSheet(filePath, true)) {
                QString dirPath = sytemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }
        // TestCase::instance().data()->excuteTestCase(TestCase::ExcuteTypeGenTC);
    }
    checkTimer.check("updateGenDataInfo : Convert.excel_002");

#if 1
    qDebug() << "==============================================================================================";
    qDebug() << "**********************************************************************************************";
    qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";

#if 0
    SheetConfigurationInfo sheetDataList =
        isSheetConfigurationInfo(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConstants);
    QList<QPair<QString, QPair<QString, QStringList>>> sheetDataInfo = sheetDataList.isSheetDataInfo();
#endif

#if 0   // Origin Sheet
    // qDebug() << "0. TEST ===========================================================================";
    // isInputSignalDataInfo(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales,
    //                         QStringList({"IMG_TelltaleABSAmberLamp_stat",
    //                                     "ON",
    //                                     "IGN1 off ⇒ on 3sec after 500ms, BrakeAirType == OFF"}),
    //                         true);

    // qDebug() << "1. TEST ===========================================================================";
    // isInputSignalDataInfo(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales,
    //                         QStringList({"IMG_TelltaleABSAmberLamp_stat",
    //                                     "",
    //                                     ""}),
    //                         false);

    // qDebug() << "2. TEST ===========================================================================";
    // isOutputSignalDataInfo(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales,
    //                         QStringList({"IMG_TelltaleABSAmberLamp_stat",
    //                                     "ON(with collect)",
    //                                     ""}));
#endif

#if 0
    qDebug() << "3. TEST ===========================================================================";
    isConvertedIGNElapsedInfo(QStringList({"1", "2", "10", "11", "12"}));
    qDebug() << "\t ConvertData  :" << QStringList({"1",  "2",  "10", "11", "12", "0"});
    qDebug() << "\t Precondition :" << QStringList({"10", "10", "0",  "0",  "0",  "10"});
    qDebug() << "\n";
#endif

#if 1   // Convert Sheet
    qDebug() << "4. TEST ===========================================================================";
    isInputSignalDataInfo(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetTelltales,
                            QStringList({"IMG_TelltaleABSAmberLamp_stat",
                                        "",
                                        ""}),
                            false);

    // qDebug() << "5. TEST ===========================================================================";
    // isOutputSignalDataInfo(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetTelltales,
    //                         QStringList({"IMG_TelltaleABSAmberLamp_stat",
    //                                     "ON",
    //                                     ""}));

    // qDebug() << "6. TEST ===========================================================================";
    // isConfigDataInfo("Config1");

    // qDebug() << "7. TEST ===========================================================================";
    // isSignalValueEnum(true, "Vehicle.System.Undefined.Inter_ConfigEBS");


    qDebug() << "8. TEST ===========================================================================";
    isSheetMergeInfo(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales);
#endif
#endif
}

QList<QPair<QString, int>> ControlExcel::isKeywordPatternInfo(const int& columnIndex) {
    QList<QPair<QString, int>> keywordPattern;

    if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
        keywordPattern = {
            // qMakePair(QString("Other"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
            qMakePair(QString("others"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
        keywordPattern = {
            qMakePair(QString("[Sheet]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)),
            qMakePair(QString("[Dependent_On]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DependentOn)),
            qMakePair(QString("[Not_Trigger]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)),
            qMakePair(QString("[Preset]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Preset)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
        keywordPattern = {
            qMakePair(QString("valuechanged"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            // qMakePair(QString("Value Changed"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            qMakePair(QString("~"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)),
            // qMakePair(QString("="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal)),
            qMakePair(QString("<=>"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)),
            qMakePair(QString(">="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual)),
            qMakePair(QString("<="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual)),
            qMakePair(QString("=>"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)),
            qMakePair(QString(">"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)),
            qMakePair(QString("<"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)),
            qMakePair(QString("!"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not)),
            // qMakePair(QString("timeout"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("MESSAGE_TIMEOUT"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            // qMakePair(QString("Time out"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("CRC_ERROR"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            // qMakePair(QString("crc"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            qMakePair(QString("[Not_Trigger]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)),
            // qMakePair(QString("D’"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            // qMakePair(QString("D`"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("D'"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) {
        keywordPattern = {
            qMakePair(QString("[Sheet]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)),
            qMakePair(QString("collect"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
        keywordPattern = {
            qMakePair(QString("[Cal]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)) {
        keywordPattern = {
            qMakePair(QString("|"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Or)),
        };
    } else {
        keywordPattern = {
            qMakePair(QString("[CustomNotTrigger]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)),
            qMakePair(QString("[CustomOver]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver)),
            qMakePair(QString("[CustomUnder]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder)),
            qMakePair(QString("[CustomRange]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange)),
            qMakePair(QString("[CustomTwoWay]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay)),
            qMakePair(QString("[CustomIgn]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn)),
            qMakePair(QString("[CustomFlow]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow)),
            qMakePair(QString("[CustomMoreThanEqual]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual)),
            qMakePair(QString("[CustomLessThanEqual]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual)),
            qMakePair(QString("[CustomNotDefined]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined)),
        };
    }
    return keywordPattern;
}

QString ControlExcel::isKeywordString(const int keywordType) {
    QMap<int, QString> keywordPatternInfo;
    QStringList keywordString;
    for (int index = 0; index <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        for (const auto& infoPair : isKeywordPatternInfo(index)) {
            keywordPatternInfo[infoPair.second] = infoPair.first;
            keywordString.append(infoPair.first);
        }
    }
    return keywordPatternInfo[keywordType];
}

int ControlExcel::isKeywordType(const int& columnIndex, QString& inputData) {
    int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
    QString tempInputData = inputData;

    QList<QPair<QString, int>> keywordPatternList = isKeywordPatternInfo(columnIndex);
    QStringList compareKeywwordList = QStringList({
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
    });

    // [입력된 inputData 중 아래에 텍스트에 해당 하는 경우 처리 방법]
    //     [방법 1] 대소문자 구분 없이 비교 (compare(str, Qt::CaseInsensitive))
    //     [방법 2] 선언된 키워드 패턴 스트링으로 변경
    //         1. "Value Changed"  =>> "ValueChanged"
    //         2. "CRC", "crc"  =>> "Crc"
    //         3. "Time out", "TimeOut"  =>> "Timeout"
    //         4. "COLLECT", "collect", "" =>> "Collect"
    //         5. ""D’", "D`", "" =>> "D'"
    //         6. "Others", "Other" | input_Signal 없으면 => "Others"

    for (const auto& pair : keywordPatternList) {
        QString currentKeyword = pair.first;
        int currentKeywordType = pair.second;
        bool compareState = false;

        for (const auto& str : compareKeywwordList) {
            if (currentKeyword.compare(str, Qt::CaseInsensitive) == 0) {
                compareState = true;
                break;
            }
        }

        if (compareState) {
            if (tempInputData.compare(currentKeyword, Qt::CaseInsensitive) == 0) {
                keywordType = currentKeywordType;
                tempInputData = isKeywordString(keywordType);
            }
        } else {
            if (tempInputData.contains(currentKeyword, Qt::CaseInsensitive) == true) {
                if ((currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) ||
                    (currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) ||
                    (currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow))) {
                    tempInputData.remove(" ");
                    tempInputData.replace(currentKeyword, ", ");
                } else {
                    // no operation
                }
                keywordType = currentKeywordType;
                tempInputData.remove(currentKeyword);
            }
        }
    }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        // qDebug() << "isKeywordType :" << keywordType << ", InputData :" << inputData << "->" << tempInputData;
        inputData = tempInputData;
    } else {
        QList<QPair<QString, int>> nonKeywordPatternList = {
            qMakePair(QString("D’"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("D`"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("Value Changed"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            qMakePair(QString("Other"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
            qMakePair(QString("Time Out"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            // qMakePair(QString("MESSAGE_TIMEOUT"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("timeout"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("crc"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            // qMakePair(QString("CRC_ERROR"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
        };
        for (const auto& pair : nonKeywordPatternList) {
            QString currentKeyword = pair.first;
            int currentKeywordType = pair.second;
            QString tmpValue = tempInputData;
            QStringList enumValueList = tmpValue.remove(" ").split(",");
            for (int i = 0; i < enumValueList.size(); ++i) {
                QString val = enumValueList.at(i);
                if (val.compare(currentKeyword, Qt::CaseInsensitive) == 0) {
                    keywordType = currentKeywordType;
                    enumValueList[i] = isKeywordString(keywordType);
                }
            }
            inputData = enumValueList.join(", ");
            if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                break;
            }
        }
    }

    return keywordType;
}

QList<KeywordInfo> ControlExcel::isKeywordTypeInfo(const int& sheetIndex, const QList<int>& inputColumnList) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QList<int> columnList = inputColumnList;

    QList<KeywordInfo> keywordInfo;
    int rowIndex = 0;
    QList<QPair<int, int>> caseRowInfo;
    QPair<int, int> rowInfo = QPair<int, int>((-1), (-1));

    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        for (const auto& columnIndex : columnList) {
            QString text = rowData.at(columnIndex);
            int keywordType = isKeywordType(columnIndex, text);
            QString data = QString();

            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                continue;
            }

            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
            } else if ((keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not))) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            } else if ((keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout))) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
            } else {
                // no operation
            }
            keywordInfo.append(KeywordInfo(rowIndex, columnIndex, text, keywordType, data));
        }

        QString caseText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

        if (caseText.contains(mergeStart)) {
            rowInfo = QPair<int, int>(rowIndex, (-1));
        } else if (caseText.contains(mergeEnd)) {
            rowInfo = QPair<int, int>(rowInfo.first, rowIndex);
        } else {
        }

        if ((rowInfo.first >= 0) && (rowInfo.second >= 0)) {
            caseRowInfo.append(rowInfo);
            rowInfo = QPair<int, int>((-1), (-1));
        }

        rowIndex++;
    }

    for (auto& keyword : keywordInfo) {
        // if ((keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) == false) {
        //     continue;
        // }

        QPair<int, int> rowInfo = QPair<int, int>((-1), (-1));
        for (const auto& row : caseRowInfo) {
            if ((keyword.isRow() < row.first) || (keyword.isRow() > row.second)) {
                continue;
            }
            rowInfo = row;
            break;
        }

        QList<QStringList> rowData;
        int getRowIndex = 0;
        const auto sheetData = getData(sheetIndex).toList();
        // Keyword 에 해당하는 Row 데이터 전체 구성
        for (const auto& rowDataList : sheetData) {
            if ((getRowIndex >= rowInfo.first) && (getRowIndex <= rowInfo.second)) {
                QStringList rowDataInfo = rowDataList.toStringList();
                QString inputSignalInfo = rowDataInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                if (inputSignalInfo.contains(keyword.isText()) == false) {
                    QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
                    for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal); ++index) {
                        dataInfo[index] = rowDataInfo.at(index);
                    }
                    rowData.append(dataInfo);
                }
            }
            getRowIndex++;
        }
        keyword.updateRowData(rowData);
    }

#if 0
    for (const auto& keyword : keywordInfo) {
        qDebug() << "-----------------------------------------------------------------------------------------";
        qDebug() << "Keyword[" << sheetIndex << "]";
        qDebug() << "\t Info        :" << keyword.isRow() << keyword.isColumn() << keyword.isKeyword() << keyword.isText();
        qDebug() << "\t Data        :" << keyword.isData();
        qDebug() << "\t RowData     :" << keyword.isRowData();
        qDebug() << "\t ConvertData :" << keyword.isConvertData();
    }
#endif

    return keywordInfo;
}

bool ControlExcel::isExcelDataValidation() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    bool validCheck = true;
    QMap<int, QStringList> checkDataList;

    // [동작 조건]
    // 1. TCName 은 중복 되면 안됨
    // 2. TCName 기준으로 Result 는 중복 되면 안됨
    // 3. Reuslt 기준으로 Case 는 중복 되면 안됨
    // 4. 현재 동작 기준 : TCName, Result, Case 별 중복 되는지만 체크 하고 있음
    // 5. 추후 함수 동작 조건 수정 필요함

    for (int sheetIndex = originStart; sheetIndex < originEnd; ++sheetIndex) {
        for (const auto& rowDataList : getData(sheetIndex).toList()) {
            QStringList rowData = rowDataList.toStringList();
            if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                // qDebug() << "Fail to sheet data list size :" << rowData.size();
                continue;
            }

            QMap<int, QString> infoText =
                QMap<int, QString>({{static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))},
                                    {static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))},
                                    {static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case))}});

            for (auto iter = infoText.cbegin(); iter != infoText.cend(); ++iter) {
                int key = iter.key();
                QString text = iter.value();
                if ((text.contains(merge) == false) && (text.contains(mergeEnd) == false)) {
                    text.remove(mergeStart);
                    text = text.toUpper();
                    checkDataList[key].append(text);
                }
            }
        }
    }

    for (auto iterCheck = checkDataList.cbegin(); iterCheck != checkDataList.cend(); ++iterCheck) {
        int key = iterCheck.key();
        QStringList textList = iterCheck.value();

        textList.removeAll("");
        int duplicateCount = textList.removeDuplicates();

        // 임시로 주석 처리함 -> 추후 체크 조건에 맞게 수정 필요
        // if ((duplicateCount > 0) || (textList.size() == 0)) {
        //     qDebug() << "Fail to excel data validation[" << key << "] :" << textList.size() << duplicateCount;
        //     validCheck = false;
        //     break;
        // }
    }

    return validCheck;
}

// TODO(csh): 최종 pr update 시에 debug log 삭제 예정
// #define ENABLE_DEBUG_LOG_OUTPUT
// #define ENABLE_DEBUG_LOG_KEYWORD
bool ControlExcel::replaceGenDataInfo() {
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    bool result = true;

    qDebug() << "\n\n=========================================================================================================";

    for (int originIndex = originStart; originIndex < originEnd; ++originIndex) {
        int convertIndex = convertStart + (originIndex - originStart);
        updateDataControl(convertIndex, getData(originIndex).toList());
        TestCase::instance().data()->setSheetData(convertIndex, getData(originIndex).toList());
    }
    const QList<int> columnListForSheetKeyword = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
    });
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForSheet =
        constructKeywordTypeInfoList(originStart, originEnd, columnListForSheetKeyword);
    constructConvertSheetDataInfo(keywordTypeInfoListForSheet);

    qDebug() << "\n\n=========================================================================================================";

    const QList<int> columnListForNonSheetKeyword = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
    });
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForNonSheet =
        constructKeywordTypeInfoList(convertStart, convertEnd, columnListForNonSheetKeyword);
    constructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

    qDebug() << "\n=========================================================================================================\n\n";
    return result;
}

QMap<int, QList<KeywordInfo>> ControlExcel::constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex,
                                                                         const QList<int>& columnList) {
    QMap<int, QList<KeywordInfo>> keywordTypeInfoList;

    for (int sheetIndex = startSheetIndex; sheetIndex < endSheetIndex; ++sheetIndex) {
        QList<KeywordInfo> keywordTypeInfo = isKeywordTypeInfo(sheetIndex, columnList);
        for (auto& keyword : keywordTypeInfo) {
            if ((keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                QList<int> columnList = QList<int>({
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
                });

                QList<QStringList> convertData = keyword.isRowData();
                isTCNameDataInfo(keyword.isText(), keyword.isData(), columnList, false, false, convertData);
                keyword.updateConvertData(convertData);
            } else {
                // no operation
            }
        }
        if (keywordTypeInfo.size() > 0) {
            keywordTypeInfoList[sheetIndex] = keywordTypeInfo;
        }

#if 0
        for (const auto& keyword : keywordTypeInfo) {
            qDebug() << "-----------------------------------------------------------------------------------------";
            qDebug() << "Keyword[" << sheetIndex << "]";
            qDebug() << "\t Info        :" << keyword.isRow() << keyword.isColumn() << keyword.isKeyword() << keyword.isText();
            qDebug() << "\t Data        :" << keyword.isData();
            qDebug() << "\t RowData     :" << keyword.isRowData();
            qDebug() << "\t ConvertData :" << keyword.isConvertData();
        }
#endif
    }

    return keywordTypeInfoList;
}

bool ControlExcel::isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                                       const int& originSheetIndex, const bool& isEqualData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    bool isAllDataEqual = isEqualData;

    if (originSheetIndex != static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
        isAllDataEqual == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
        qDebug() << "1 rowDataList : " << rowDataList;
        qDebug() << "1 keywordInfo.isRowData : " << keywordInfo.isRowData();
#endif
        for (int index = 0; index < keywordInfo.isRowData().length(); index++) {
            QStringList keywordRowData = keywordInfo.isRowData().at(index);
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
            qDebug() << "2 keyword.isRowData : " << keywordRowData;
#endif
            for (int columnIdx = static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName);
                 columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData); columnIdx++) {
                QString originRowDataStr = rowDataList.at(columnIdx);
                if (static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName) <= columnIdx &&
                    columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
                    originRowDataStr.remove(mergeStart);
                    originRowDataStr.remove(mergeEnd);
                    originRowDataStr.remove(merge);
                    if (originRowDataStr.isEmpty() == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "==============================================================";
                        qDebug() << "3 origin  : " << originRowDataStr;
                        qDebug() << "3 keyword : " << keywordRowData.at(columnIdx);
#endif
                        if (keywordRowData.at(columnIdx).contains(originRowDataStr) == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 : Not Contains";
#endif
                            isAllDataEqual = false;
                            break;
                        } else {
                            isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 : Contains";
#endif
                        }
                    } else {
                        // VehicleType empty
                        QString vehicleTypeStr = keywordRowData.at(columnIdx);
                        ivis::common::getRemoved(vehicleTypeStr, mergeInfos);
                        if (vehicleTypeStr.compare(originRowDataStr) != 0) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 VehicleType : Not Equal";
#endif
                            isAllDataEqual = false;
                            break;
                        } else {
                            isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 VehicleType : Equal";
#endif
                        }
                    }
                } else if (static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal) <= columnIdx &&
                           columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "==============================================================";
                    qDebug() << "3 origin  : " << rowDataList.at(columnIdx);
                    qDebug() << "3 keyword : " << keywordRowData.at(columnIdx);
#endif
                    if (keywordRowData.at(columnIdx).compare(originRowDataStr) != 0) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "4 : Not Equal";
#endif
                        isAllDataEqual = false;
                        break;
                    } else {
                        isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "4 : Equal";
#endif
                    }
                } else {
                    // no operation
                }
            }
            if (isAllDataEqual == true) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                qDebug() << "5 [Equal] find equal row data in keyword info";
#endif
                break;
            } else {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                qDebug() << "5 [Not Equal] diff row data in keyword info";
#endif
            }
        }
    }

    return isAllDataEqual;
}

void ControlExcel::constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
    const int originSheetStartIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int convertSheetStartIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertSheetEndIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    QList<int> usedSheetIndexList;

    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        int originIndex = iter.key();
        int convertIndex = 0;
        int rowIndex = 0;
        QVariantList convertRowData;

        bool isNotAppendOriginTCNameMergeStart = false;
        bool isNotAppendOriginResultMergeStart = false;
        QStringList tmpNotAppendMergeKeywordOriginList;
        int convertSheetIndex = originIndex -
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) +
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
        usedSheetIndexList.append(convertSheetIndex);
        for (const auto& rowDataList : getData(originIndex).toList()) {
            QList<QStringList> convertData;
            QList<QStringList> rowData;
            KeywordInfo curKeywordInfo;
            QStringList tmpRowDataList = rowDataList.toStringList();

            bool isEqualData = false;

            QString originTCName = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).trimmed();
            QString originVehicleType =
                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)).trimmed();
            QString originResult = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).trimmed();

            const int originTCNameMergeType = getMergeKeywordType(originTCName);
            const int originVehicleTypeMergeType = getMergeKeywordType(originVehicleType);
            const int originResultMergeType = getMergeKeywordType(originResult);

            // Origin Row Data가 [Sheet] Keyword에 존재 여부 판단 로직
            for (KeywordInfo keyword : iter.value().toList()) {
                if ((rowIndex == keyword.isRow()) &&
                    (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                    convertData = keyword.isConvertData();
                } else {
                    isEqualData = isDataAlreadyExistInKeywordInfoList(tmpRowDataList, keyword, originIndex, isEqualData);
                }
            }

            if (convertData.size() == 0) {
                if (isEqualData == false) {
                    convertRowData.append(QVariant(tmpRowDataList));
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 append rowDataList (not in keyword info list) : " << tmpRowDataList;
#endif
                } else {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 [PASS] not append rowDataList : " << tmpRowDataList;
#endif
                    // [Sheet] Keyword가 [MergeStart]TCName || [MergeStart]Result 이후 row_data에 존재하는 경우에 처리하는
                    // 로직
                    if (originTCNameMergeType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) {
                        isNotAppendOriginTCNameMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList << originTCName << originVehicleType << originResult;
                    } else if (originResultMergeType ==
                               static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) {
                        isNotAppendOriginResultMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList << originTCName << originVehicleType << originResult;
                    } else {
                        // no operation
                    }

                    if (convertRowData.length() > 0) {
                        QStringList tmpConvertList = convertRowData[convertRowData.length() - 1].toStringList();
                        if (originTCNameMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }

                        if (originVehicleTypeMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }

                        if (originResultMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "OriginResult before >>>>> Result [MergeEnd] : "
                                     << convertRowData[convertRowData.length() - 1]
                                            .toList()[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)]
                                     << " <= "
                                     << tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
#endif
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "OriginResult after  >>>>> Result [MergeEnd] : "
                                     << convertRowData[convertRowData.length() - 1]
                                            .toList()[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)];
#endif
                        }
                    }
                }
            } else {
                if (convertIndex == 0) {
                    convertIndex = convertSheetStartIndex + (originIndex - originSheetStartIndex);
                }

                // TODO(csh): TCName, Result, Case Data 누락 시 Error keyword 추가 예정
                // TCName / VehicleType / Result 까지의 병합을 위한 조건 처리 로직
                int convertDataListLength = convertData.length();
                if (convertDataListLength > 1) {
                    for (int idx = 0; idx < convertDataListLength; ++idx) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "============================================================================================"
                                    "==========";
                        qDebug() << "1. convlenth : (" << idx << "/" << convertDataListLength << ")";
                        qDebug() << "2. rowIndex  : " << rowIndex;
                        qDebug() << "3. covtData  : " << convertData[idx];
                        qDebug() << "4. ori_Data  : " << rowDataList;
                        qDebug() << "5. prevData : " << tmpNotAppendMergeKeywordOriginList;
#endif
                        QStringList tmpConvertDataList = convertData[idx];
                        QString tcNameStr;
                        QString vehicleTypeStr;
                        QString resultStr;
                        if ((idx == 0) && (isNotAppendOriginTCNameMergeStart == true)) {
                            isNotAppendOriginTCNameMergeStart = false;
                            if (tmpNotAppendMergeKeywordOriginList.length() == 3) {
                                tcNameStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                                vehicleTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                                resultStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                                tmpNotAppendMergeKeywordOriginList.clear();
                            }
                        } else if (isNotAppendOriginResultMergeStart == true) {
                            isNotAppendOriginResultMergeStart = false;
                            if (tmpNotAppendMergeKeywordOriginList.length() == 3) {
                                tcNameStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                                vehicleTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                                resultStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                                tmpNotAppendMergeKeywordOriginList.clear();
                            }
                        } else {
                            // TCName && VehicleType Merge 기능
                            if ((originTCNameMergeType ==
                                 static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) &&
                                (originVehicleTypeMergeType ==
                                 static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) &&
                                (idx == 0)) {
                                tcNameStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            } else if ((originTCNameMergeType ==
                                        static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) &&
                                       (originVehicleTypeMergeType ==
                                        static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) &&
                                       (idx == convertDataListLength - 1)) {
                                tcNameStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    mergeEnd,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            } else {
                                tcNameStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    merge,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            }

                            // Result Merge 기능
                            if (((originResultMergeType ==
                                  static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) ||
                                 ((originResultMergeType ==
                                       static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType) ||
                                   originResultMergeType ==
                                       static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)))) &&
                                (idx == 0)) {
                                resultStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            } else if (((originResultMergeType ==
                                         static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) ||
                                        (originResultMergeType ==
                                             static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType) ||
                                         originResultMergeType ==
                                             static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd))) &&
                                       (idx == convertDataListLength - 1)) {
                                resultStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            } else {
                                resultStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            }
                        }
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = tcNameStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = vehicleTypeStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = resultStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] =
                            constructKeywordCaseName(
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)),
                                tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]);

#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "5 append keyword convert DataList : " << tmpConvertDataList;
#endif
                        convertRowData.append(QVariant(tmpConvertDataList));
                        tmpConvertDataList.clear();
                    }
                } else {
                    // [Sheet] Keyword 내부에 Input_Signal이 1개만 존재하는 경우
                    if (convertDataListLength == 1) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug()
                            << "============================================================================================";
                        qDebug() << "1. convlenth : (" << 0 << "/" << convertDataListLength << ")";
                        qDebug() << "2. rowIndex  : " << rowIndex;
                        qDebug() << "3. covtData  : " << convertData[0];
                        qDebug() << "4. ori_Data  : " << rowDataList;
                        qDebug() << "5. prevData : " << tmpNotAppendMergeKeywordOriginList;
#endif
                        QStringList tmpConvertDataList = convertData[0];
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] =
                            constructKeywordCaseName(
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)),
                                tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]);

                        convertRowData.append(QVariant(tmpConvertDataList));
                        tmpConvertDataList.clear();
                    }
                }
            }
            rowIndex++;
        }
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
        qDebug() << "8 convertRowData : \n" << convertRowData;
#endif
        if ((convertIndex != 0) && (convertRowData.size() > 0)) {
            updateDataControl(convertIndex, convertRowData);
            TestCase::instance().data()->setSheetData(convertIndex, convertRowData);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Sheet Keyword] Origin[" << originIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowData.size();
            for (const auto& rowData : convertRowData) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
    constructOutputConfigColumnDataInfo(usedSheetIndexList);
}

void ControlExcel::constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        // int originIndex = iter.key();
        int convertIndex = iter.key();
        int rowIndex = 0;
        QVariantList convertRowDataList;

        for (const auto& rowDataList : getData(convertIndex).toList()) {
            QStringList convertRowData;
            for (KeywordInfo keyword : iter.value().toList()) {
                QString customKeyword;

                if (rowIndex != keyword.isRow()) {
                    continue;
                }
                switch (keyword.isKeyword()) {
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword = isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver));
                        QString overValue = QString("%1[%2], [%3]")
                                                .arg(customKeyword)
                                                .arg(QString::number(keyword.isText().toULongLong()))
                                                .arg(QString::number(keyword.isText().toULongLong() + 1));
                        convertRowData[keyword.isColumn()] = overValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Over] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Over] keyword Data    : " << keyword.isText();
                        qDebug() << "[Over] Keyword RowData : " << convertRowData;
                        qDebug() << "[Over] Converted Value : " << overValue;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword =
                            isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder));
                        QString underValue = QString("%1[%2], [%3]")
                                                 .arg(customKeyword)
                                                 .arg(QString::number(keyword.isText().toULongLong()))
                                                 .arg(QString::number(keyword.isText().toULongLong() - 1));
                        convertRowData[keyword.isColumn()] = underValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Under] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Under] keyword Data    : " << keyword.isText();
                        qDebug() << "[Under] Keyword RowData : " << convertRowData;
                        qDebug() << "[Under] Converted Value : " << underValue;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range): {
                        convertRowData = rowDataList.toStringList();
                        QStringList rangeValueList = keyword.isText().remove(" ").split(",");
                        if (rangeValueList.length() == 2) {
                            customKeyword =
                                isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange));
                            quint64 preconditionMaxValue = static_cast<quint64>(UINT32_MAX) + 1;
                            quint64 rangeLValue = rangeValueList.at(0).toULongLong();
                            quint64 rangeRValue = rangeValueList.at(1).toULongLong();
                            QStringList preconditionValueList = {QString::number(preconditionMaxValue)};
                            QStringList inputValueList = {QString::number(rangeLValue), QString::number(rangeLValue + 1),
                                                          QString::number(rangeRValue - 1), QString::number(rangeRValue)};
                            if (preconditionValueList.size() == 1 && preconditionValueList.size() < inputValueList.size()) {
                                preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
                            }
                            QString rangeValue = QString("%1[%2], [%3]")
                                                     .arg(customKeyword)
                                                     .arg(preconditionValueList.join(", "))
                                                     .arg(inputValueList.join(", "));
                            convertRowData[keyword.isColumn()] = rangeValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "[Range] keyword Type    : " << keyword.isKeyword();
                            qDebug() << "[Range] keyword Data    : " << keyword.isText();
                            qDebug() << "[Range] Keyword RowData : " << convertRowData;
                            qDebug() << "[Range] Converted Value : " << rangeValue;
#endif
                        } else {
                            qDebug() << "[Warning] Range Value Format Error : " << keyword.isText();
                        }
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword = isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow));
                        QStringList flowValueList = keyword.isText().remove(" ").split(",");
                        QStringList preconditionValueList;
                        QStringList inputValueList;
                        for (int index = 0; index < flowValueList.size(); index++) {
                            if (index % 2 == 0) {
                                preconditionValueList.append(flowValueList.at(index));
                            } else {
                                inputValueList.append(flowValueList.at(index));
                            }
                        }
                        if ((preconditionValueList.size() == 1) && (preconditionValueList.size() < inputValueList.size())) {
                            preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
                        }
                        QString flowValue = QString("%1[%2], [%3]")
                                                .arg(customKeyword)
                                                .arg(preconditionValueList.join(", "))
                                                .arg(inputValueList.join(", "));
                        convertRowData[keyword.isColumn()] = flowValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword =
                            isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay));
                        QStringList twoWayValueList = keyword.isText().remove(" ").split(",");
                        QStringList preconditionValueList;
                        QStringList inputValueList;
                        for (int index = 0; index < twoWayValueList.size(); index++) {
                            preconditionValueList.append(twoWayValueList.at(index));
                            QString inputValue;
                            int calIndex = 0;
                            if (index % 2 == 0) {
                                calIndex = index + 1;
                            } else {
                                calIndex = index - 1;
                            }
                            if (calIndex >= 0 && calIndex < twoWayValueList.size()) {
                                inputValueList.append(twoWayValueList.at(calIndex));
                            } else {
                                qDebug() << "[Warning] TwoWay Keyword Value Format Error";
                            }
                        }
                        QString twoWayValue = QString("%1[%2], [%3]")
                                                  .arg(customKeyword)
                                                  .arg(preconditionValueList.join(", "))
                                                  .arg(inputValueList.join(", "));
                        convertRowData[keyword.isColumn()] = twoWayValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword =
                            isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual));
                        QString moreThanEqualValue = QString("%1[%2], [%3]")
                                                         .arg(customKeyword)
                                                         .arg(QString::number(keyword.isText().toULongLong() - 1))
                                                         .arg(QString::number(keyword.isText().toULongLong()));
                        convertRowData[keyword.isColumn()] = moreThanEqualValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual): {
                        convertRowData = rowDataList.toStringList();
                        customKeyword =
                            isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual));
                        QString lessThanEqualValue = QString("%1[%2], [%3]")
                                                         .arg(customKeyword)
                                                         .arg(QString::number(keyword.isText().toULongLong() + 1))
                                                         .arg(QString::number(keyword.isText().toULongLong()));
                        convertRowData[keyword.isColumn()] = lessThanEqualValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare): {
                        convertRowData = rowDataList.toStringList();
                        if (keyword.isData().contains(SFC_IGN_ELAPSED) == true) {
                            convertRowData[keyword.isColumn()] = keyword.isText();
                        } else {
                            // Not IGN Signal Don't care 조건
                            QStringList enumDontCareValueList;
                            int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
                            QString signalName = keyword.isData();
                            QString vehicleType = QString();
                            QStringList signalData = QStringList();
                            int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
                            QVariant vehicleTypeList =
                                ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV);
                            if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
                                vehicleTypeList =
                                    ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV);
                            }

                            for (const auto& vehicle : vehicleTypeList.toStringList()) {
                                if (vehicleType.size() > 0) {
                                    vehicleType.append(", ");
                                }
                                vehicleType.append(vehicle);
                            }
                            // TODO(csh): 추후 toEnum API로 변경 예정
                            QMap<int, QStringList> dataInfoFromSingnalName =
                                isSignalDataInfo(signalName, signalData, vehicleType, dataType);
                            QStringList valueEnum =
                                dataInfoFromSingnalName[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
                            for (QString enumString : valueEnum) {
                                QStringList enumSplitList = enumString.split(":");
                                for (QString enumSplit : enumSplitList) {
                                    if (enumSplit.contains("0x") == false) {
                                        enumSplit.remove('\"');
                                        enumDontCareValueList.append(enumSplit);
                                        break;
                                    }
                                }
                            }
                            convertRowData[keyword.isColumn()] = enumDontCareValueList.join(", ");
                        }
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not): {
                        convertRowData = rowDataList.toStringList();
                        QString signalName = keyword.isData();
                        QStringList keywordValueList = keyword.isText().split(",");
                        QStringList allSignalEnumList = isSignalValueEnum(true, signalName);
                        QStringList notKeywordConvertDataList;
                        for (const QString& value1 : allSignalEnumList) {
                            if (!keywordValueList.contains(value1)) {  // keywordValueList에 없는 경우
                                notKeywordConvertDataList.append(value1);
                            }
                        }
                        // for (const QString& value1 : allSignalEnumList) {
                        //     for (const QString& value2 : keywordValueList) {
                        //         if (value1.compare(value2) == true) {
                        //             notKeywordConvertDataList.append(value1);
                        //         }
                        //     }
                        // }
                        convertRowData[keyword.isColumn()] = notKeywordConvertDataList.join(", ");
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout): {
                        convertRowData = rowDataList.toStringList();
                        convertRowData[keyword.isColumn()] = keyword.isText();
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other): {
                        if (keyword.isData().isEmpty() == true) {
                            // Others 조건 : Input_Signal column이 비어있는 경우
                            convertRowData = rowDataList.toStringList();
                            convertRowData[keyword.isColumn()] = keyword.isText();
                        }
                        break;
                    }
                    default: {
                        // no operation
                        break;
                    }
                }
            }
            if (convertRowData.size() == 0) {
                convertRowDataList.append(rowDataList);
            } else {
                convertRowDataList.append(QVariant(convertRowData));
            }
            rowIndex++;
        }

        if ((convertIndex != 0) && (convertRowDataList.size() > 0)) {
            updateDataControl(convertIndex, convertRowDataList);
            TestCase::instance().data()->setSheetData(convertIndex, convertRowDataList);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Non Sheet Keyword] Convert [" << convertIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowDataList.size();
            for (const auto& rowData : convertRowDataList) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
}

// output signal & config signal 처리
void ControlExcel::constructOutputConfigColumnDataInfo(const QList<int>& convertSheetIndexList) {
    for (int index = 0; index < convertSheetIndexList.size(); index++) {
        int convertSheetIndex = convertSheetIndexList.at(index);
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
        qDebug() << "=================================== [Append Output & Config Data] ===================================";
        qDebug() << "> convertSheetIndex : " << convertSheetIndex << "\n";
#endif
        int outputDataListIndex = 0;
        int configDataListIndex = 0;
        QVariantList convertRowData;

        QList<QStringList> tmpOutputRowData;
        QList<QStringList> tmpConfigRowData;
        int convertIndexToOriginIndex = convertSheetIndex -
                                        static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) +
                                        static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
        for (const auto& convertRowDataList : getData(convertSheetIndex).toList()) {
            QStringList tmpOutputConvertData = convertRowDataList.toStringList();

            QString currentTCNameStr = tmpOutputConvertData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            QString currentResultStr = tmpOutputConvertData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));

            const int curTCNameMergeKeywordType = getMergeKeywordType(currentTCNameStr);
            const int curResultMergeKeywordType = getMergeKeywordType(currentResultStr);

            currentTCNameStr = constructMergeKeywords("", currentTCNameStr);
            currentResultStr = constructMergeKeywords("", currentResultStr);

            if ((curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) ||
                (curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))) {
                tmpConfigRowData = isConfigDataInfo(convertIndexToOriginIndex, currentTCNameStr, "");
            }

            if ((curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart) ||
                 curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType)) ||
                (curResultMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart) ||
                 curResultMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))) {
                tmpOutputRowData = isOutputDataInfo(convertIndexToOriginIndex, currentTCNameStr, currentResultStr);
            }

            if (tmpOutputConvertData.isEmpty() == false && tmpOutputRowData.isEmpty() == false &&
                outputDataListIndex < tmpOutputRowData.length()) {
                tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)] =
                    tmpOutputRowData.at(outputDataListIndex)
                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
                tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)] =
                    tmpOutputRowData.at(outputDataListIndex)
                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
                tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)] =
                    tmpOutputRowData.at(outputDataListIndex)
                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
                outputDataListIndex++;
            }

            if (tmpOutputConvertData.isEmpty() == false && tmpConfigRowData.isEmpty() == false &&
                configDataListIndex < tmpConfigRowData.length()) {
                tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)] =
                    tmpConfigRowData.at(configDataListIndex)
                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal));
                tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)] =
                    tmpConfigRowData.at(configDataListIndex).at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
                configDataListIndex++;
            }

#if defined(ENABLE_DEBUG_LOG_KEYWORD)
            qDebug() << "=========================================================================================";
            qDebug() << "1 currentTCNameStr : " << currentTCNameStr << ", currentResultStr : " << currentResultStr;
            qDebug() << "1 all tmpConfigRowData : " << tmpConfigRowData;
            if (tmpOutputRowData.isEmpty() == false && outputDataListIndex < tmpOutputRowData.length()) {
                qDebug() << "1 Output Data List : " << tmpOutputRowData.at(outputDataListIndex);
            }
            qDebug() << "\n";
            qDebug() << "2 all tmpOutputRowData : " << tmpOutputRowData;
            if (tmpConfigRowData.isEmpty() == false && configDataListIndex < tmpConfigRowData.length()) {
                qDebug() << "2 Config Data List : " << tmpConfigRowData.at(configDataListIndex);
            }
            qDebug() << "\n";
            if (tmpOutputConvertData.isEmpty() == false) {
                qDebug() << "> Append tmpOutputConvertData : " << tmpOutputConvertData;
            }
            qDebug() << "=========================================================================================";
#endif
            if (curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd) ||
                curTCNameMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType)) {
                configDataListIndex = 0;
                currentTCNameStr.clear();
                tmpConfigRowData.clear();
            }

            if (curResultMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd) ||
                curResultMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType)) {
                outputDataListIndex = 0;
                currentResultStr.clear();
                tmpOutputRowData.clear();
            }

            // TODO(csh): Output 관련 Data 누락 시 Error keyword 추가 예정
            if (tmpOutputConvertData.size() == 0) {
                convertRowData.append(convertRowDataList);
            } else {
                convertRowData.append(tmpOutputConvertData);
            }
        }

        if ((convertSheetIndex != 0) && (convertRowData.size() > 0)) {
            updateDataControl(convertSheetIndex, convertRowData);
            TestCase::instance().data()->setSheetData(convertSheetIndex, convertRowData);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Output/Config] Before Convert[" << convertSheetIndex << "] -> After Convert["
                     << convertSheetIndex << "] :" << convertRowData.size();
            for (const auto& rowData : convertRowData) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
}

bool ControlExcel::appendConvertAllTCSignalSet() {
    bool result = true;

    const int convertStart = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int convertEnd = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    QList<int> usedSheetIndexList;
    for (int sheetIndex = convertStart; sheetIndex < convertEnd; sheetIndex++) {
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "Not support sheet : " << sheetIndex;
            continue;
        }

        QVariantList convertRowData;
        usedSheetIndexList.append(sheetIndex);
        for (const auto& rowDataList : getData(sheetIndex).toList()) {
            const QList<int> deleteDataColumnList = QList({
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data),
                // static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest),
            });
            QStringList convertRowDataList = deleteColumnRowData(rowDataList.toStringList(), deleteDataColumnList);
            QList<QStringList> tmpAppendConvertRowDataList;

            QString tcNameStr = convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            QString vehicleTypeStr = convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
            QString resultStr = convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
            QString caseStr = convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

            const int curTCNameMergeKeywordType = getMergeKeywordType(tcNameStr);
            const int curVehicleTypeMergeKeywordType = getMergeKeywordType(vehicleTypeStr);
            const int curResultMergeKeywordType = getMergeKeywordType(resultStr);
            const int curCaseMergeKeywordType = getMergeKeywordType(caseStr);

            const int columnDataMaxSize = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);

            // others 에서는 처리하지 않기 위함
            if (convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)).isEmpty() == false &&
                (curCaseMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd) ||
                 curCaseMergeKeywordType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))) {
                QMap<QString, SignalDataInfo> appendInputSignalDataInfoMap = isInputSignalDataInfo(
                    sheetIndex,
                    QStringList({constructMergeKeywords("", tcNameStr), constructMergeKeywords("", resultStr),
                                 constructMergeKeywords("", caseStr)}),
                    true);

                if (appendInputSignalDataInfoMap.isEmpty() == false) {
                    // 기존 Case의 MergeEnd의 rowData를 Merge로 변환하여 추가
                    QStringList tmpConvertRowData(columnDataMaxSize, "");
                    QString tcNameMergeStr = merge;
                    QString vehicleTypeMergeStr = merge;
                    QString resultMergeStr = (curCaseMergeKeywordType ==
                                              static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))
                                                 ? mergeStart
                                                 : merge;
                    QString caseMergeStr = (curCaseMergeKeywordType ==
                                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))
                                               ? mergeStart
                                               : merge;

                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = constructMergeKeywords(
                        tcNameMergeStr, convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                        constructMergeKeywords(
                            vehicleTypeMergeStr,
                            convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = constructMergeKeywords(
                        resultMergeStr, convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = constructMergeKeywords(
                        caseMergeStr, convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)));
                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] =
                        convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                    tmpConvertRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] =
                        convertRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
                    tmpAppendConvertRowDataList.append(tmpConvertRowData);

                    for (auto iter = appendInputSignalDataInfoMap.begin(); iter != appendInputSignalDataInfoMap.end(); ++iter) {
                        QStringList tmpConvertAppendRowData(columnDataMaxSize, "");
                        QString tmpTCNameStr;
                        QString tmpVehicleTypeStr;
                        QString tmpResultStr;
                        QString tmpCaseStr;
                        bool isLastIndex = (std::next(iter) == appendInputSignalDataInfoMap.end());
                        if (isLastIndex == true) {
                            tmpTCNameStr = tcNameStr;
                            tmpVehicleTypeStr = vehicleTypeStr;
                            tmpResultStr = (curCaseMergeKeywordType ==
                                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))
                                               ? constructMergeKeywords(mergeEnd, resultStr)
                                               : resultStr;
                            tmpCaseStr = (curCaseMergeKeywordType ==
                                          static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType))
                                             ? constructMergeKeywords(mergeEnd, caseStr)
                                             : caseStr;
                        } else {
                            tmpTCNameStr = constructMergeKeywords(merge, tcNameStr);
                            tmpVehicleTypeStr = constructMergeKeywords(merge, vehicleTypeStr);
                            tmpResultStr = constructMergeKeywords(merge, resultStr);
                            tmpCaseStr = constructMergeKeywords(merge, caseStr);
                        }

                        QString inputDataStr =
                            isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger));
                        SignalDataInfo tmpInfo = iter.value();
                        if (tmpInfo.isDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) {
                            QStringList tmpOriginData = tmpInfo.isOriginData();
                            if (tmpOriginData.size() > 1) {
                                inputDataStr = tmpOriginData.join(", ");
                                inputDataStr.replace(isKeywordString(tmpInfo.isKeywordType()),
                                                     isKeywordString(static_cast<int>(
                                                         ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined)));
                            } else if (tmpOriginData.size() == 1) {
                                inputDataStr.clear();
                                inputDataStr = QString("%1[%2], [%3]")
                                                   .arg(isKeywordString(static_cast<int>(
                                                       ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined)))
                                                   .arg(tmpOriginData.at(0))
                                                   .arg(tmpOriginData.at(0));
                            } else {
                                // no operation
                            }
                        } else if ((tmpInfo.isValueEnum().isEmpty() == false) && (tmpInfo.isNotUsedEnum().isEmpty() == false)) {
                            // Enum Value
                            inputDataStr += tmpInfo.isNotUsedEnum().join(", ");
                        } else if (tmpInfo.isValueEnum().isEmpty() == true &&
                                   tmpInfo.isDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) {
                            if (tmpInfo.isKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver) &&
                                tmpInfo.isKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder) &&
                                tmpInfo.isKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual) &&
                                tmpInfo.isKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual)) {
                                uint64_t maxValue = static_cast<uint64_t>(UINT32_MAX) + 1;
                                inputDataStr += QString::number(maxValue);
                            } else {
                                QStringList tmpOriginData = tmpInfo.isOriginData();
                                if (tmpOriginData.size() > 1) {
                                    inputDataStr = tmpOriginData.join(", ");
                                    inputDataStr.replace(isKeywordString(tmpInfo.isKeywordType()),
                                                         isKeywordString(static_cast<int>(
                                                             ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined)));
                                }
                            }
                        } else {
                            // no operation
                        }
                        if (inputDataStr.isEmpty() == false) {
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                                tmpTCNameStr;
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                                tmpVehicleTypeStr;
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                                tmpResultStr;
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = tmpCaseStr;
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] =
                                iter.key();
                            tmpConvertAppendRowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] =
                                inputDataStr;
                            tmpAppendConvertRowDataList.append(tmpConvertAppendRowData);
                        } else {
                            tmpAppendConvertRowDataList.clear();
                        }
                    }
                }
            }

            if (tmpAppendConvertRowDataList.size() == 0) {
                convertRowData.append(QVariant(convertRowDataList));
            } else {
                for (int index = 0; index < tmpAppendConvertRowDataList.size(); index++) {
                    convertRowData.append(QVariant(tmpAppendConvertRowDataList.at(index)));
                }
            }
        }
        if ((sheetIndex != 0) && (convertRowData.size() > 0)) {
            updateDataControl(sheetIndex, convertRowData);
            TestCase::instance().data()->setSheetData(sheetIndex, convertRowData);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Append Signal Set] Origin[" << sheetIndex << "] -> Convert[" << sheetIndex
                     << "] :" << convertRowData.size();
            for (const auto& rowData : convertRowData) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
    constructOutputConfigColumnDataInfo(usedSheetIndexList);

    return result;
}

inline QString ControlExcel::constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QString returnStr = baseKeyword;
    ivis::common::getRemoved(returnStr, mergeInfos);
    if (additionalKeyword.isEmpty() == false) {
        returnStr = additionalKeyword + returnStr;
    }
    return returnStr;
}

QStringList ControlExcel::deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex) {
    QStringList deleteDataStringList = rowData;

    for (int index = 0; index < deleteColumnIndex.size(); index++) {
        deleteDataStringList[deleteColumnIndex.at(index)] = "";
    }

    return deleteDataStringList;
}

int ControlExcel::getMergeKeywordType(const QString& data) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    int mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType);

    if (data.trimmed().startsWith(mergeStart) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart);
    } else if (data.trimmed().startsWith(merge) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::Merge);
    } else if (data.trimmed().startsWith(mergeEnd) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd);
    } else {
        // NoMergeType
    }

    return mergeKeywordType;
}

QString ControlExcel::constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName) {
    QString prefix;
    QString suffix;
    QString returnStr;

    bool isMergeKeywordExist =
        (getMergeKeywordType(convertCaseName) != static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType));

    QString data = constructMergeKeywords("", originCaseName);
    if (isMergeKeywordExist == true) {
        // [Sheet] 키워드 사용하는 Case이름과 [Sheet] 키워드의 Case 이름을 조합하는 로직
        // e.g)
        // Private Sheet의 Case Name         ->  VALID1
        // Constant [Sheet]Private Case Name -> [MergeStart]Case1
        // 결과 : [MergeStart]Case1_VALID1
        prefix = convertCaseName.section("]", 0, 0) + "]";  // prefix : [MergeStart]
        suffix = convertCaseName.section("]", 1);           // suffix : Case Column String

        returnStr = prefix + data + "_" + suffix;  // case : [MergeStart] + Origin_Row_Case_String + Case_Column_String
    } else {
        prefix = data;
        suffix = convertCaseName;
        returnStr = prefix + "_" + suffix;
    }

    return returnStr;
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
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);
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
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    // qDebug() << "ControlExcel::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose: {
            ControlManager::instance().data()->raise(displayType);
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeReceiveKeyFocus, true, true);
            break;
        }
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
        case ivis::common::EventTypeEnum::EventTypeRunMultiDocker: {
            updateGenDataInfo(eventType);
            break;
        }
        default: {
            break;
        }
    }
}
