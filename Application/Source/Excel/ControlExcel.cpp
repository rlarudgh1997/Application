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

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextStart));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeText));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextEnd));

    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QStringList vehicleTypeList = QStringList();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toStringList();
    } else {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toStringList();
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType, vehicleTypeList);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAppMode, appMode);
    updateNodeAddress(true, QStringList());

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

void ControlExcel::updateNodeAddress(const bool& all, const QStringList& list) {
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

    QStringList tcNameList = QStringList();
    if (list.size() > 0) {
        QVariant excelMergeTextStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextStart);
        QVariant excelMergeTextEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextEnd);
        QVariant excelMergeText = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeText);
        for (auto tcName : list) {
            tcName.remove(excelMergeTextStart.toString());
            tcName.remove(excelMergeTextEnd.toString());
            tcName.remove(excelMergeText.toString());
            if (tcName.size() > 0) {
                tcNameList.append(tcName);
            }
        }
    }
    tcNameList.sort();
    tcNameList.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName, tcNameList, true);
    // qDebug() << "\t TCNameList :" << tcNameList;
}

void ControlExcel::updateSheetData(const int& propertyType, const QVariantList& sheetData) {
    if (getData(propertyType).toList() == sheetData) {
        return;
    }
    updateDataHandler(propertyType, sheetData);

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "updateSheetData : Sheet[" << propertyType << "] -> data changed !!!";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
}

void ControlExcel::updateExcelSheet(const bool& excelOpen, const QVariant& dirPath) {
    qDebug() << "ControlExcel::updateExcelSheet() ->" << excelOpen << "," << dirPath;

    QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    QStringList otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QVariantList rowCount = QVariantList();
    QStringList tcNameList = QStringList();

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);

    if (excelOpen) {
        int sheetIndex = 0;

        for (const auto& sheet : sheetName) {
            QVariantList sheetData = QVariantList();
            QString filePath = QString("%1/%2_%3.fromExcel").arg(dirPath.toString()).arg(sheetIndex).arg(sheet);
            QStringList readData = ivis::common::FileInfo::readFile(filePath);

            for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
                if (rowIndex < 2) {
                    continue;  // RowIndex[0] : column 인덱스 정보,  RowIndex[1] : title(desc, other) 정보
                }
                QStringList rowDataList = readData[rowIndex].split("\t");
                sheetData.append(rowDataList);
                // qDebug() << sheetIndex << ". rowDataList[" << rowIndex << "] :" << rowDataList.size() << "," << rowDataList;
                if ((sheet.compare("Description") != false) && (rowDataList.size() > 0)) {
                    tcNameList.append(rowDataList.at(0));
                }
            }
            rowCount.append(sheetData.size());
            updateSheetData((ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription + sheetIndex), sheetData);

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
        for (const auto& sheet : sheetName) {
            rowCount.append(rowMax);
        }
    }
    updateNodeAddress(false, tcNameList);
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
    int writeSize = 0;
    int sheetIndex = 0;
    int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    if (backup) {
        propertyType = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    }

    for (const auto& sheet : sheetName) {
        QString file = QString("%1_%2.toExcel").arg(sheetIndex++).arg(sheet);
        QString writeData = QString();
        QVariantList sheetData = QVariantList();
        // Title - Append
        if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
            (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
            sheetData.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle));
        } else {
            sheetData.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle));
        }
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
    QString moduleName = QString();
    QStringList signalSplit = signalName.split(".");
    if (signalSplit.size() >= 2) {
        // signalName : SFC.{MODULE_NAME}.*
        moduleName = signalSplit.at(1);
        if ((moduleName.compare("Extension") == false) || (moduleName.compare("Private") == false)) {
            // signalName : SFC.Extension.{MODULE_NAME}.*  or  SFC.Private.{MODULE_NAME}.*
            moduleName = signalSplit.at(2);
        }
    } else {
        // signalName : {MODULE_NAME}
        moduleName = signalName;
    }
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QStringList sfcTypeList = QStringList();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModePV = (appMode == ivis::common::AppModeEnum::AppModeTypePV);
    if (appModePV) {
        sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
    } else {
        sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
        // Input Signal : SFC.ADAS_Driving_New.Telltale.FCA.Stat
        // Found Yml    : /model/SFC/ADAS_Driving_CV/ADAS_Driving_CV.yml
        // Module Name  : ADAS_Driving_New -> ADAS_Driving_CV
        // moduleName.remove("_New");
        moduleName.replace("_New", "_CV");
    }

    QString ymlFile = QString();
    for (const auto& sfc : sfcTypeList) {
        QString file = QString("%1/SFC/%2/%3/%4.yml").arg(sfcModelPath).arg(sfc).arg(moduleName).arg(moduleName);
        bool fileExists = QFile::exists(file);
        if (fileExists) {
            ymlFile = file;
        }
        // qDebug() << ((appModePV) ? ("\t PV yml :") : ("\t CV yml :")) << fileExists << file;

        if (appModePV == false) {
            file = QString("%1/SFC/%2/%3_CV/%4_CV.yml").arg(sfcModelPath).arg(sfc).arg(moduleName).arg(moduleName);
            if (QFile::exists(file)) {
                ymlFile = file;
                qDebug() << "\t -> yml :" << true << file;
            }
        }
    }
    // qDebug() << "Found yml :" << (ymlFile.size() > 0) << ymlFile;
    return ymlFile;
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

QMap<int, QStringList> ControlExcel::isSignalDataInfo(const bool& isDataType, const QString& signalName,
                                                      const QMap<int, QStringList>& fileList) {
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

    QMap<int, QStringList> inputDataInfo = QMap<int, QStringList>();
    bool sfcSignal = signalName.trimmed().startsWith("SFC.");
    bool vehicleSignal = signalName.trimmed().startsWith("Vehicle.");

    if ((sfcSignal == false) && (vehicleSignal == false)) {
        qDebug() << "Input signal name is incorrect :" << signalName;
        return inputDataInfo;
    }

    QString signal = QString();
    bool vehicleSystem = signalName.contains("Vehicle.System.");
    int startAppendIndex = ((vehicleSystem) ? (3) : (2));
    int index = 0;
    for (const auto& splitText : signalName.split(PREFIX_DOT)) {
        if (index >= startAppendIndex) {
            signal.append((signal.size() > 0) ? (".") : (""));
            signal.append(splitText);
        }
        index++;
    }
    if (signal.size() == 0) {
        qDebug() << "Fail to signal name size : 0";
        return inputDataInfo;
    }

    // qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "Input  Signal      :" << signalName;
    // qDebug() << "Parser Signal      :" << signal;

    QMapIterator<int, QStringList> iter(fileList);
    while (iter.hasNext()) {
        iter.next();
        int inputDataType = iter.key();
        QStringList fileName = iter.value();
        // qDebug() << inputDataType << ". Size :" << fileName.size() << ", File :" << fileName;
        QString dataType = QString();
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
                        foundSignal = ((sfcSignal) ? (true) : (lineStr.compare(signal) == false));
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
                    dataType = info;
                    dataType.remove(PREFIX_DATA_TYPE);
                    continue;
                }

                if (foundMatchingTable) {
                    foundValueEnum = false;
                    matchingTable.append(info);
                    // qDebug() << "\t MatchingTable :" << info;
                } else {
                    foundMatchingTable = (info.contains(PREFIX_MATCHING_TABLE));
                }

                if ((vehicleSystem) || (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum)) {
                    if (foundValueEnum) {
                        if (info.contains(PREFIX_MATCHING_TABLE)) {
                            continue;
                        }
                        valueEunm.append(info);
                        // qDebug() << "\t ValueEnum     :" << info;
                    } else {
                        foundValueEnum = info.contains(PREFIX_VALUE_ENUM);
                    }
                }
            }
        }

        if (isDataType) {  // DataType prepend
            valueEunm.prepend(dataType);
            matchingTable.prepend(dataType);
        }

        if (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) {
            inputDataInfo[inputDataType] = valueEunm;
            // qDebug() << "\t Value    Size :" << valueEunm.size();
        } else {
            inputDataInfo[inputDataType] = matchingTable;
            // qDebug() << "\t Matching Size :" << matchingTable.size();
            if ((valueEunm.size() > 0) && (vehicleSystem)) {
                inputDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = valueEunm;
                // qDebug() << "\t Value    Size :" << valueEunm.size();
            }
        }
    }
    // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    return inputDataInfo;
}

QMap<int, QStringList> ControlExcel::isSignalFileList(const QString& signalName, const QString& vehicleType) {
    QMap<int, QStringList> fileList = QMap<int, QStringList>();

    if (signalName.trimmed().startsWith("SFC.")) {
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum].append(isSfcFileInfo(signalName));
    } else if (signalName.trimmed().startsWith("Vehicle.")) {
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
    }

    return fileList;
}

QMap<int, QStringList> ControlExcel::isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                                      const bool& convert, const bool& mergeInfoErase) {
    const QVariant excelMergeTextStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextStart);
    const QVariant excelMergeTextEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeTextEnd);
    const QVariant excelMergeText = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeText);
    const int startIndex = (convert) ? (ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)
                                       : (ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates);
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QPair<int, int> tcNamerowInfo = QPair<int, int>((-1), (-1));
    QPair<int, int> resultRowInfo = QPair<int, int>((-1), (-1));
    int foundSheetIndex = 0;

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        int rowIndex = 0;
        bool foundTCName = false;
        for (const auto& rowDataList : getData(sheetIndex).toList()) {
            QVariantList rowData = rowDataList.toList();
            if (rowData.size() != (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                qDebug() << "Fail to sheet data list size :" << rowData.size();
                continue;
            }

            QString text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
            if (text.contains(tcName)) {
                foundTCName = true;
                if (text.contains(excelMergeTextEnd.toString())) {
                    tcNamerowInfo = QPair<int, int>(tcNamerowInfo.first, rowIndex);
                } else if (text.contains(excelMergeText.toString())) {
                    // Skip
                } else {
                    tcNamerowInfo = QPair<int, int>(rowIndex, rowIndex);
                }
            }

            if ((foundTCName) && (result.size() > 0)) {
                text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).toString();
                if (text.contains(result)) {
                    if (text.contains(excelMergeTextEnd.toString())) {
                        resultRowInfo = QPair<int, int>(resultRowInfo.first, rowIndex);
                    } else if (text.contains(excelMergeText.toString())) {
                        // Skip
                    } else {
                        resultRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
            rowIndex++;
        }

        if ((tcNamerowInfo.first >= 0) && (tcNamerowInfo.second >= 0)) {
            foundSheetIndex = sheetIndex;
            break;
        }
    }

    if (foundSheetIndex == 0) {
        qDebug() << "Not found tcName :" << tcName;
        return QMap<int, QStringList>();
    }


    QMap<int, QStringList> tcNameDataInfo = QMap<int, QStringList>();
    int searchRowIndex = 0;
    QPair<int, int> rowInfo = (result.size() == 0) ? (tcNamerowInfo) : (resultRowInfo);

    qDebug() << "\n\n\n==========================================================================================";
    qDebug() << "Sheet :" << foundSheetIndex;
    qDebug() << "RowInfo[TCName] :" << tcNamerowInfo.first << tcNamerowInfo.second;
    qDebug() << "RowInfo[Result] :" << resultRowInfo.first << resultRowInfo.second;
    qDebug() << "isTCNameDataInfo :" << tcName << result;

    for (const auto& rowDataList : getData(foundSheetIndex).toList()) {
        QVariantList rowData = rowDataList.toList();
        if ((searchRowIndex >= rowInfo.first) && (searchRowIndex <= rowInfo.second)) {
            for (const auto& columnIndex : columnList) {
                if ((result.size() > 0) && (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))) {
                    // Skip - [reulst value valid] & [columnList contains Result]
                    continue;
                }
                QString dataInfo = rowData.at(columnIndex).toString();
                tcNameDataInfo[columnIndex].append(dataInfo);
            }
        }
        searchRowIndex++;
    }

    if (mergeInfoErase) {
        QMap<int, QStringList> tempDataInfo = QMap<int, QStringList>();
        for (auto iter = tcNameDataInfo.cbegin(); iter != tcNameDataInfo.cend(); ++iter) {
            for (auto dataInfo : iter.value()) {
                dataInfo.remove(excelMergeTextStart.toString());
                dataInfo.remove(excelMergeTextEnd.toString());
                dataInfo.remove(excelMergeText.toString());
                tempDataInfo[iter.key()].append(dataInfo);
            }
            tempDataInfo[iter.key()].removeAll("");
            tempDataInfo[iter.key()].removeDuplicates();

            qDebug() << "\t TCNameDataInfo[" << iter.key() << "] :" << tempDataInfo[iter.key()];
        }
        tcNameDataInfo = tempDataInfo;
    }

#if 0
    qDebug() << "\n\n\n==========================================================================================";
    qDebug() << "Sheet :" << foundSheetIndex;
    qDebug() << "RowInfo[TCName] :" << tcNamerowInfo.first << tcNamerowInfo.second;
    qDebug() << "RowInfo[Result] :" << resultRowInfo.first << resultRowInfo.second;
    qDebug() << "RowInfo         :" << rowInfo.first << rowInfo.second;
    qDebug() << "DataInfo        :" << tcNameDataInfo;
#endif

    return tcNameDataInfo;
}

void ControlExcel::updateAutoCompleteSignal(const QVariantList& inputData) {
    if (inputData.size() != 4) {
        qDebug() << "Fail to signal input data size :" << inputData.size();
        return;
    }

    bool sfcSignal = inputData.at(0).toBool();
    bool outputState = inputData.at(1).toInt();
    QString vehicleType = inputData.at(2).toString();
    QString signalName = inputData.at(3).toString();

    qDebug() << "updateAutoCompleteSignal :" << sfcSignal << outputState << vehicleType << signalName;

    QMap<int, QStringList> dataInfo = isSignalDataInfo(false, signalName, isSignalFileList(signalName, vehicleType));
    for (int index = ivis::common::InputDataTypeEnum::InputDataTypeValueEnum;
         index < ivis::common::InputDataTypeEnum::InputDataTypeMax; index++) {
        updateDataHandler((ivis::common::PropertyTypeEnum::PropertyTypeInputDataValuEnum + index), dataInfo[index]);
    }

    if (dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum].size() > 0) {  // ValueEunm data is not null
        QVariantList info = QVariantList({sfcSignal, outputState});
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeValueEnum, info, true);
    }
}

void ControlExcel::updateAutoCompleteEtc(const QVariantList& inputData) {
    if (inputData.size() != 2) {
        qDebug() << "Fail to etc input data size :" << inputData.size();
        return;
    }
    ivis::common::CheckTimer checkTimer;

    QString vehicleType = inputData.at(0).toString();
    QString tcName = inputData.at(1).toString();
    int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
    QStringList tcNameList = getData(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName).toStringList();

    for (const auto& name : tcNameList) {
        if (tcName.trimmed().endsWith(name)) {
            keywordType = isKeywordType(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal), tcName);
            break;
        }
    }

    if ((keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) == false) {
        return;   // Sheet 키워드가 아닌 경우 -> 자동완성 리스트 구성 하지 않음
    }


    QList<int> columnList = QList<int>({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
    });

    QMap<int, QStringList> tcNameDataInfo = isTCNameDataInfo(tcName, QString(), columnList, false, true);
    QStringList resultInfo = tcNameDataInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)];
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult, resultInfo, true);

    checkTimer.check("updateAutoCompleteEtc");
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

    if (fileName.size() == 0) {
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

void ControlExcel::updateExcelSheetEditInfo(const bool& tcNameEdit) {
    if (tcNameEdit == false) {
        return;
    }
    ivis::common::CheckTimer checkTimer;

    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QStringList tcNameList = QStringList();
    for (int updateSheetIndex = startIndex; updateSheetIndex < endIndex; ++updateSheetIndex) {
        bool titleSkip = true;
        for (const auto& sheetDataList : getData(updateSheetIndex).toList()) {
            if (titleSkip) {
                titleSkip = false;
                continue;
            }
            QVariantList sheetData = sheetDataList.toList();
            QString tcName = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
            if (tcName.size() > 0) {
                tcNameList.append(tcName);
            }
        }
    }
    updateNodeAddress(false, tcNameList);

    checkTimer.check("updateExcelSheetEditInfo");
}

void ControlExcel::updateInputDataValidation(const QVariantList& cellDataInfo) {
    if (cellDataInfo.size() != 5) {
        qDebug() << "Fail to cell data infoetc input data size :" << cellDataInfo.size();
    }

    QString signal = cellDataInfo.at(0).toString();
    QString inputData = cellDataInfo.at(1).toString();
    int sheetIndex = cellDataInfo.at(2).toInt();
    int rowIndex = cellDataInfo.at(3).toInt();
    int columnIndex = cellDataInfo.at(4).toInt();

    qDebug() << "updateInputDataValidation :" << signal << inputData;

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

    QMap<int, QStringList> dataInfo = isSignalDataInfo(true, signal, isSignalFileList(signal, vehicleType));
    int dataType = TestCase::instance().data()->isDataType(dataInfo);

    if ((dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) ||
        (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString))) {
        return;
    }

    QString inputDataTemp = inputData;
    inputDataTemp.replace(", ", ",");
    QStringList dataList = inputDataTemp.split(",");

    QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    int validCount = 0;
    QString errorInfo = QString();
    for (const auto& data : dataList) {
        if (data.size() == 0) {
            errorInfo = QString(STRING_POPUP_DATA_VALIDATION_TIP).arg("spaces");
        } else if (data.toLower().trimmed().startsWith("0x")) {
            errorInfo = QString(STRING_POPUP_DATA_VALIDATION_TIP).arg("hex values");
        } else {
        }

        if (errorInfo.size() > 0) {
            break;
        }

        for (const auto& originData : valueEnum) {
            QStringList splitData = originData.split(":");
            if (splitData.size() < 2) {
                continue;
            }
            if (splitData.at(0).toUpper().compare(data.toUpper()) == false) {
                qDebug() << "\t Matching[" << validCount << "] :" << data << originData;
                validCount++;
                break;
            }
        }
    }

    if (validCount != dataList.size()) {
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
    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeCreateExcelSheeet).toBool() == false) {
        qDebug() << "No exist excel sheet !!!!!!";
        return;
    }

    ivis::common::CheckTimer checkTimer;

    if (replaceGenDataInfo()) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/test.excel";
            if (writeExcelSheet(filePath, true)) {
                QString dirPath = sytemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }

        constructGenDataInfo();
    }

    checkTimer.check("updateGenDataInfo");
}

QMap<QString, int> ControlExcel::isKeywordPatternInfo(const int& columnIndex) {
    QMap<QString, int> keywordPattern = QMap<QString, int>();

    if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"Other", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"[Cal]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal)},
            {"~", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)},
            {"=", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal)},
            {"<", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)},
            {">", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"[Collect]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"[Cal]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal)},
            {"~", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)},
            {"=", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal)},
            {"<", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)},
            {">", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)) {
        keywordPattern = {
            {"|", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Or)},
        };
    } else {
    }

    return keywordPattern;
}

int ControlExcel::isKeywordType(const int& columnIndex, QString& signalName) {
    int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
    QMap<QString, int> keywordPattern = isKeywordPatternInfo(columnIndex);
    QString keywordText = signalName;

    for (auto iter = keywordPattern.constBegin(); iter != keywordPattern.constEnd(); ++iter) {
        if (keywordText.contains(iter.key())) {
            keywordText.remove(iter.key());
            keywordType |= iter.value();
        }
    }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        // qDebug() << "isKeywordType :" << keywordType << ", Text :" << signalName << "->" << keywordText;
        signalName = keywordText;
    }

    return keywordType;
}

QMap<int, QList<KeywordInfo>> ControlExcel::isKeywordTypeInfo() {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const QList<int> columnList = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue),
        // static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal),
    });

    QMap<int, QList<KeywordInfo>> keywordInfo;
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        int rowIndex = 0;
        for (const auto& sheetDataList : getData(sheetIndex).toList()) {
            if (sheetDataList.toList().size() != (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                qDebug() << "Fail to sheet data list size :" << sheetDataList.toList().size();
                continue;
            }

            QVariantList sheetData = sheetDataList.toList();
            for (const auto& columnIndex : columnList) {
                QString text = sheetData.at(columnIndex).toString();
                int keywordType = isKeywordType(columnIndex, text);
                QString data = QString();

                if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                    continue;
                }

                if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) {
                    data = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)).toString();
                } else if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
                    data = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)).toString();
                } else if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)) {
                    text = QString("Other");
                } else if (keywordType & (static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal) |
                           static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                    // [Cal][Sheet] 키워드에 대한 동작 처리
                } else {
                }

                keywordInfo[sheetIndex].append(KeywordInfo(rowIndex, columnIndex, text, keywordType, data));
            }
            rowIndex++;
        }
    }

#if 1
    for (auto iter = keywordInfo.cbegin(); iter != keywordInfo.cend(); ++iter) {
        auto sheet = iter.key();
        for (KeywordInfo info : iter.value()) {
            qDebug() << "\t Keyword[" << sheet << "] :" << info.isRow() << info.isColumn() << info.isKeyword() << info.isText()
                     << info.isData();
        }
    }
#endif

    return keywordInfo;
}

bool ControlExcel::replaceGenDataInfo() {
    bool result = true;

    QMap<int, QList<KeywordInfo>> keywordInfo = isKeywordTypeInfo();


#if 1   // TestCode : TCName & Result 값을 가지고 데이터(Case, InputSinnal, InputData) 확인
    QMap<int, QList<KeywordInfo>> tempKeywordInfo = QMap<int, QList<KeywordInfo>>();
    QList<QMap<int, QStringList>> tcNameDataInfo = QList<QMap<int, QStringList>>();

    for (auto iter = keywordInfo.cbegin(); iter != keywordInfo.cend(); ++iter) {
        int sheet = iter.key();
        QList<KeywordInfo> keywordInfoList = iter.value().toList();
        for (KeywordInfo keyword : keywordInfoList) {
            if ((keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) == false) {
                continue;
            }

            QList<int> columnList = QList<int>({
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
                static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
            });
            QMap<int, QStringList> convertData = isTCNameDataInfo(keyword.isText(), keyword.isData(), columnList, false, false);
            tcNameDataInfo.append(convertData);
            keyword.updateConvertData(convertData);
            qDebug() << "keyword :" << keyword.isRow() << keyword.isColumn() << keyword.isKeyword() << keyword.isText()
                     << keyword.isData() << keyword.isConvertData();
        }
        // tempKeywordInfo[sheet] = keywordInfoList;
    }
    qDebug() << "\n\n\n==========================================================================================";
    // qDebug() << "tempKeywordInfo :" << tempKeywordInfo;
    qDebug() << "\n==========================================================================================\n\n\n";
#endif

#if 1   // TestCode : Convert 데이터 저장
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    int convertIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;

    for (int originIndex = startIndex; originIndex < endIndex; ++originIndex) {
        updateDataControl(convertIndex, getData(originIndex));
        convertIndex++;
    }
#endif

    return result;
}

void ControlExcel::constructGenDataInfo() {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    QStringList signalNameList = QStringList();

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        // bool titleSkip = true;
        for (const auto& sheetDataList : getData(sheetIndex).toList()) {
            if (sheetDataList.toList().size() != (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                qDebug() << "Fail to sheet data list size :" << sheetDataList.toList().size();
                continue;
            }
            // if (titleSkip) {
            //     titleSkip = false;
            //     continue;
            // }

            QVariantList rowDataList = sheetDataList.toList();
            QVariant inputSignal = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            QVariant outputSignal = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
            signalNameList.append(inputSignal.toString());
            signalNameList.append(outputSignal.toString());
        }
    }
    signalNameList.removeAll("");
    signalNameList.removeDuplicates();

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

    TestCase::instance().data()->clearSignalDataInfo();
    for (const auto& signalName : signalNameList) {
        QMap<int, QStringList> dataInfo = isSignalDataInfo(true, signalName, isSignalFileList(signalName, vehicleType));
        if (dataInfo.size() > 0) {
            TestCase::instance().data()->setSignalDataInfo(signalName, dataInfo);
        }

#if 0
        QString dataType;
        QMap<int, QStringList> signalDataInfo = TestCase::instance().data()->getSignalDataInfo(signalName, dataType);
#endif
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
        case ConfigInfo::ConfigTypeExcelMergeTextStart:
        case ConfigInfo::ConfigTypeExcelMergeTextEnd:
        case ConfigInfo::ConfigTypeExcelMergeText: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText, value);
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
        case ivis::common::EventTypeEnum::EventTypeEditExcelSheet: {
            updateExcelSheetEditInfo(value.toBool());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeAutoInputDescriptionInfo: {
            updateAutoInputDescriptionInfo(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                           QVariantList({STRING_POPUP_CELL_COLUMN, STRING_POPUP_CELL_COLUMN_TIP}));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeAutoCompleteSignal: {
            updateAutoCompleteSignal(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeAutoCompleteEtc: {
            updateAutoCompleteEtc(value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateCellDataInfo: {
            updateInputDataValidation(value.toList());
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
