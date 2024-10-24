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
        QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
        QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
        QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);
        for (auto tcName : list) {
            tcName.remove(excelMergeStart.toString());
            tcName.remove(excelMergeEnd.toString());
            tcName.remove(excelMerge.toString());
            if (tcName.size() > 0) {
                tcNameList.append(tcName);
            }
        }
    }
    tcNameList.sort();
    tcNameList.removeDuplicates();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName, tcNameList, true);
    // qDebug() << "\t updateNodeAddress :" << all << list << tcNameList;
}

void ControlExcel::updateSheetData(const int& propertyType, const QVariantList& sheetData) {
    if (getData(propertyType).toList() == sheetData) {
        return;
    }
    updateDataHandler(propertyType, sheetData);

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "updateSheetData[" << propertyType << "] : data changed !!!";
#if 1
    int rowIndex = 0;
    for (const auto& sheetDataList : sheetData.toList()) {
        qDebug() << "\t Data[" << rowIndex++ << "] :" << sheetDataList.toStringList();
    }
#endif
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n\n";
}

void ControlExcel::updateExcelSheet(const bool& excelOpen, const QVariant& dirPath) {
    qDebug() << "updateExcelSheet() ->" << excelOpen << "," << dirPath;

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
                                                      const QStringList& signalData, const QMap<int, QStringList>& fileList) {
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

    if (signalData.size() > 0) {
        inputDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = signalData;
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
                                                      const bool& convert, const bool& mergeInfoErase,
                                                      QList<QStringList>& convertData) {
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

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
                    dataInfo.remove(excelMergeStart.toString());
                    dataInfo.remove(excelMergeEnd.toString());
                    dataInfo.remove(excelMerge.toString());
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
        if (caseInfo.contains(excelMergeStart.toString())) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(excelMerge.toString())) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(excelMergeEnd.toString())) {
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
            caseInfo.remove(excelMergeStart.toString());
            caseInfo.remove(excelMergeEnd.toString());
            caseInfo.remove(excelMerge.toString());

            for (int index = 0; index < tempList.size(); ++index) {
                QString mergeText;
                if (index == 0) {
                    mergeText = excelMergeStart.toString();
                } else if (index == (tempList.size() - 1)) {
                    mergeText = excelMergeEnd.toString();
                } else {
                    mergeText = excelMerge.toString();
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

QPair<int, int> ControlExcel::isContainsRowInfo(const int& sheetIndex, const QString& tcName, const QString& result,
                                                const QString& caseInfo) {
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

    // const QList<QPair<QString, int>> columnInfoList = QList({
    //     QPair<QString, int>(tcName, static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)),
    //     QPair<QString, int>(result, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)),
    //     QPair<QString, int>(caseInfo, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)),
    // });
    // QPair<int, int> tempRowInfo((-1), (-1));
    QPair<int, int> tcNameRowInfo((-1), (-1));
    QPair<int, int> resultRowInfo((-1), (-1));
    QPair<int, int> caseRowInfo((-1), (-1));
    int rowIndex = 0;

    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }
#if 1
        QString text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
        if (text.contains(tcName)) {
            if (text.contains(excelMergeEnd.toString())) {
                tcNameRowInfo = QPair<int, int>(tcNameRowInfo.first, rowIndex);
            } else {
                if (text.contains(excelMerge.toString()) == false) {
                    tcNameRowInfo = QPair<int, int>(rowIndex, rowIndex);
                }
            }

            if (result.size() > 0) {
                text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                if (text.contains(result)) {
                    if (text.contains(excelMergeEnd.toString())) {
                        resultRowInfo = QPair<int, int>(resultRowInfo.first, rowIndex);
                    } else {
                        if (text.contains(excelMerge.toString()) == false) {
                            resultRowInfo = QPair<int, int>(rowIndex, rowIndex);
                        }
                    }
                }

                if (caseInfo.size() > 0) {
                    text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
                    if (text.contains(caseInfo)) {
                        if (text.contains(excelMergeEnd.toString())) {
                            caseRowInfo = QPair<int, int>(caseRowInfo.first, rowIndex);
                        } else {
                            if (text.contains(excelMerge.toString()) == false) {
                                caseRowInfo = QPair<int, int>(rowIndex, rowIndex);
                            }
                        }
                    }

                    // qDebug() << rowIndex << ". [Contain] :" << text.contains(caseInfo);
                    // qDebug() << "\t CaseInfo   :" << caseInfo << "=" << text;
                    // qDebug() << "\t RowInfo    :" << caseRowInfo;
                    // qDebug() << "\t MergeStart :" << text.contains(excelMergeStart.toString());
                    // qDebug() << "\t Merge      :" << text.contains(excelMerge.toString());
                    // qDebug() << "\t MergeEnd   :" << text.contains(excelMergeEnd.toString());
                    // qDebug() << "\n";
                }
            }
        }
#else
#if 0
        for (const auto& columnInfo : columnInfoList) {
            bool validCheck = (columnInfo.second == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) ?
                                (true) : (columnInfo.first.size() > 0);
            if (validCheck == false) {
                continue;
            }
            QString readText = rowData.at(columnInfo.second);
            if (readText.contains(columnInfo.first)) {
                if (readText.contains(excelMergeEnd.toString())) {
                    if (columnInfo.second == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) {
                        tcNameRowInfo = QPair<int, int>(tempRowInfo.first, rowIndex);
                    } else if (columnInfo.second == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)) {
                        resultRowInfo = QPair<int, int>(tempRowInfo.first, rowIndex);
                    } else if (columnInfo.second == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
                        caseRowInfo = QPair<int, int>(tempRowInfo.first, rowIndex);
                    } else {
                    }
                    tempRowInfo = QPair<int, int>((-1), (-1));
                } else {
                    if (readText.contains(excelMerge.toString()) == false) {
                        tempRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
        }
#else
        QString text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
        if (text.contains(tcName)) {
            if (text.contains(excelMergeEnd.toString())) {
                tcNameRowInfo = QPair<int, int>(tcNameRowInfo.first, rowIndex);
            } else {
                if (text.contains(excelMerge.toString()) == false) {
                    tcNameRowInfo = QPair<int, int>(rowIndex, rowIndex);
                }
            }
        }

        if (result.size() > 0) {
            text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
            if (text.contains(result)) {
                if (text.contains(excelMergeEnd.toString())) {
                    resultRowInfo = QPair<int, int>(resultRowInfo.first, rowIndex);
                } else {
                    if (text.contains(excelMerge.toString()) == false) {
                        resultRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
        }

        if (caseInfo.size() > 0) {
            text = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
            if (text.contains(caseInfo)) {
                if (text.contains(excelMergeEnd.toString())) {
                    caseRowInfo = QPair<int, int>(caseRowInfo.first, rowIndex);
                } else {
                    if (text.contains(excelMerge.toString()) == false) {
                        caseRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
        }
#endif
#endif
        rowIndex++;
    }

    QPair<int, int> rowInfo((-1), (-1));

    if (caseInfo.size() > 0) {
        rowInfo = caseRowInfo;
    } else if (result.size() > 0) {
        rowInfo = resultRowInfo;
    } else {
        rowInfo = tcNameRowInfo;
    }
    // qDebug() << "isContainsRowInfo :" << tcNameRowInfo << resultRowInfo << caseRowInfo << "->" << rowInfo;

    return rowInfo;
}

QList<QStringList> ControlExcel::isRowDataInfo(const int& sheetIndex, const QPair<int, int>& rowInfo,
                                               const QPair<int, int>& columnInfo) {
    const int columnStart = columnInfo.first;
    const int columnEnd = columnInfo.second + 1;

    QList<QStringList> dataInfo;
    int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex < ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max);
    }

    // qDebug() << "isRowDataInfo :" << sheetIndex << rowInfo << columnInfo;

    int rowIndex = 0;
    for (const auto& rowDataList : getData(sheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < columnMax) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        QStringList data(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
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

QStringList ControlExcel::isSignalDataEnum(const QString& signalName, const QStringList& valueEnum) {
    QStringList data;
    bool sfcSignal = signalName.trimmed().startsWith("SFC.");
    // bool vehicleSignal = signalName.trimmed().startsWith("Vehicle.");

    // [sfc]
    // on : 0x01, off : 0x02

    // [vehicle]
    // 0x01 : on
    // 0x02 : off

    return data;
}

QStringList ControlExcel::isSignalDataValue(const QString& signalName, const QStringList& valueEnum) {
    QStringList data;
    bool sfcSignal = signalName.trimmed().startsWith("SFC.");
    // bool vehicleSignal = signalName.trimmed().startsWith("Vehicle.");
    return data;
}

QMap<QString, InputSignalDataInfo> ControlExcel::isInputSignalDataInfo(const bool& usedTC, const int& sheetIndex,
                                                                       const QString& tcName, const QString& result,
                                                                       const QString& caseInfo) {
    QList<QStringList> tcNameDataInfo = isInputDataInfo(sheetIndex, tcName, QString(), QString());
    QList<QStringList> caseDataInfo = isInputDataInfo(sheetIndex, tcName, result, caseInfo);

    qDebug() << "isInputSignalDataInfo :"  << usedTC << sheetIndex << tcName << result << caseInfo
             << ", Size :" << tcNameDataInfo.size() << caseDataInfo.size();

    QMap<QString, QStringList> inputDataInfo;
    for (const auto& tcNameData : tcNameDataInfo) {
        QString singalInfo = tcNameData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
        if (singalInfo.size() == 0) {
            continue;
        }
        QString resultInfo = tcNameData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
        QString caseInfo = tcNameData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
        QString inputData = tcNameData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
        QStringList dataInfo = inputData.remove(" ").split(",");
        inputDataInfo[singalInfo].append(dataInfo);
        qDebug() << "[" << singalInfo << "] :" << dataInfo;
    }

    qDebug() << "=================================================================================================\n\n";
    QMap<QString, QStringList> allSignal;
    for (auto iter = inputDataInfo.cbegin(); iter != inputDataInfo.cend(); ++iter) {
        QString singalInfo = iter.key();
        QStringList dataInfo = iter.value();
        dataInfo.removeAll("");
        dataInfo.removeDuplicates();

        // qDebug() << "[" << singalInfo << "]";
        // qDebug() << "\t OriginData  :" << iter.value();
        // qDebug() << "\t ConvertData :" << dataInfo;

        if (caseInfo.size() > 0) {
            for (const auto& caseData : caseDataInfo) {
                QString caseSingalInfo = caseData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                if (caseSingalInfo.compare(singalInfo) == false) {
                    singalInfo.clear();
                    break;
                }
            }
        }
        if (singalInfo.size() > 0) {
            allSignal[singalInfo] = dataInfo;
        }
    }

    // SignalData 구성
    QMap<QString, InputSignalDataInfo> signalDataInfo;

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

    for (auto iter = allSignal.cbegin(); iter != allSignal.cend(); ++iter) {
        QString signalName = iter.key();
        QStringList signalData = iter.value();
        QMap<int, QStringList> dataInfo =
            isSignalDataInfo(false, signalName, signalData, isSignalFileList(signalName, vehicleType));

        if (dataInfo.size() > 0) {
            QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
            QStringList inputData = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
            QString checkText = (signalData.size() == 0) ? (QString()) : (signalData.at(0));   // 0 : 키워드 포함 데이터
            int keywordType = isKeywordType(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData), checkText);
            signalDataInfo[signalName] = InputSignalDataInfo(dataInfo, keywordType, QString());


        }
    }




    // qDebug() << "=================================================================================================\n\n";
    // for (auto iter = signalDataInfo.cbegin(); iter != signalDataInfo.cend(); ++iter) {
    //     qDebug() << "[" << iter.key() << "]";
    //     qDebug() << "\t CurrentData :" << iter.value();
    // }
    // qDebug() << "=================================================================================================\n\n";

    return signalDataInfo;
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
    QStringList signalData = QStringList();

    qDebug() << "updateAutoCompleteSignal :" << sfcSignal << outputState << vehicleType << signalName << signalData;

    QMap<int, QStringList> dataInfo = isSignalDataInfo(false, signalName, signalData, isSignalFileList(signalName, vehicleType));
    for (int index = ivis::common::InputDataTypeEnum::InputDataTypeValueEnum;
         index < ivis::common::InputDataTypeEnum::InputDataTypeMax; index++) {
        updateDataHandler((ivis::common::PropertyTypeEnum::PropertyTypeInputDataValueEnum + index), dataInfo[index]);
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
        return;  // Sheet 키워드가 아닌 경우 -> 자동완성 리스트 구성 하지 않음
    }

    QList<int> columnList = QList<int>({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
    });

    QList<QStringList> convertData = QList<QStringList>();
    QMap<int, QStringList> tcNameDataInfo = isTCNameDataInfo(tcName, QString(), columnList, false, true, convertData);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult,
                      tcNameDataInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)], true);
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

    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QStringList tcNameList = QStringList();
    for (int updateSheetIndex = startIndex; updateSheetIndex < endIndex; ++updateSheetIndex) {
        for (const auto& sheetDataList : getData(updateSheetIndex).toList()) {
            QVariantList sheetData = sheetDataList.toList();
            QString tcName = sheetData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
            if (tcName.size() > 0) {
                tcNameList.append(tcName);
            }
        }
    }
    updateNodeAddress(false, tcNameList);
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
    // bool sfcSignal = signalName.trimmed().startsWith("SFC.");
    bool vehicleSignal = signalName.trimmed().startsWith("Vehicle.");
    int validDataIndex = (vehicleSignal) ? (1) : (0);

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
    QMap<int, QStringList> dataInfo = isSignalDataInfo(true, signalName, signalData, isSignalFileList(signalName, vehicleType));
    int keywordType = isKeywordType(columnIndex, signalName);
    int dataType = TestCase::instance().data()->isDataType(dataInfo, keywordType);

    if ((dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) ||
        (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString))) {
        return;
    }

    QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    int validCount = 0;
    QString errorInfo = QString();
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

    // TODO(csh): IterTools 수행 전, 최종 signal 조합 set 구성 후 두번째 Convert Excel파일 생성 예정
    if (appendConvertDataInfo() == true) {
        constructGenDataInfo();

        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_002";
            if (writeExcelSheet(filePath, true)) {
                QString dirPath = sytemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }

#if 0    // itertool test
        TestCase::instance().data()->excuteTestCase(TestCase::ExcuteTypeGenTC);
#endif
    }

    checkTimer.check("updateGenDataInfo");
}

QMap<QString, int> ControlExcel::isKeywordPatternInfo(const int& columnIndex) {
    QMap<QString, int> keywordPattern = QMap<QString, int>();

    if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"[Dependent_On]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DependentOn)},
            {"[Not_Trigger]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)},
            {"[Preset]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Preset)},
            {"Other", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
        keywordPattern = {
            {"ValueChanged", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)},
            {"~", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)},
            {"=", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal)},
            {">", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)},
            {"<", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)},
            {"=>", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)},
            {"<=>", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)},
            {"!", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not)},
            {"D`", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)},
            {"Timeout", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)},
            {"Crc", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)},
            {"[Not_Trigger]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) {
        keywordPattern = {
            {"[Sheet]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)},
            {"Collect", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)},
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
        keywordPattern = {
            {"[Cal]", static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal)},
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
        QString text = iter.key();
        if (text.compare("Value Changed", Qt::CaseInsensitive) == 0) {
            text = text.trimmed();
        }
        bool compareState =
            ((text.compare("Timeout", Qt::CaseInsensitive) == 0) || (text.compare("Crc", Qt::CaseInsensitive) == 0) ||
             (text.compare("Collect", Qt::CaseInsensitive) == 0) || (text.compare("ValueChanged", Qt::CaseInsensitive) == 0));
        if (compareState) {
            if (keywordText.compare(text, Qt::CaseInsensitive) == 0) {
                keywordType |= iter.value();
            }
        } else {
            if (keywordText.contains(text, Qt::CaseInsensitive)) {
                int keyword = iter.value();
                if (keyword == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
                    keywordText.remove(" ");
                    keywordText.replace("~", ", ");
                }
                keywordText.remove(text);
                keywordType |= iter.value();
            }
        }
    }

    //  >=, <=, =>, <=> 확인 조건 추가 예정 코드
    // else {
    //     if (keywordText.contains(text, Qt::CaseInsensitive)) {
    //         if ((keywordText.contains("=", Qt::CaseInsensitive) == 0) || (keywordText.contains(">", Qt::CaseInsensitive) == 0)
    //         ||
    //             (keywordText.contains("<", Qt::CaseInsensitive) == 0)) {
    //             if (keywordText.contains("=>", Qt::CaseInsensitive) == 0) {
    //             } else if (keywordText.contains("<=>", Qt::CaseInsensitive) == 0) {
    //             } else {
    //             }
    //             keywordType |= iter.value();
    //             break;
    //         } else {
    //             int keyword = iter.value();
    //             if (keyword == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
    //                 keywordText.remove(" ");
    //                 keywordText.replace("~", ", ");
    //             }
    //             keywordText.remove(text);
    //         }
    //         keywordType |= iter.value();
    //     }
    // }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        // qDebug() << "isKeywordType :" << keywordType << ", Text :" << signalName << "->" << keywordText;
        signalName = keywordText;
    }

    return keywordType;
}

QList<KeywordInfo> ControlExcel::isKeywordTypeInfo(const int& sheetIndex) {
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);
    const QList<int> columnList = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue),
        // static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal),
    });

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

            if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
            } else if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            } else if (keywordType & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)) {
                text = QString("Other");
            } else if (keywordType & (static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under) |
                                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal))) {
                // [Cal][Sheet] 키워드에 대한 동작 처리
            } else {
            }
            keywordInfo.append(KeywordInfo(rowIndex, columnIndex, text, keywordType, data));
        }

        QString caseText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

        if (caseText.contains(excelMergeStart.toString())) {
            rowInfo = QPair<int, int>(rowIndex, (-1));
        } else if (caseText.contains(excelMergeEnd.toString())) {
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
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

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
                if ((text.contains(excelMerge.toString()) == false) && (text.contains(excelMergeEnd.toString()) == false)) {
                    text.remove(excelMergeStart.toString());
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
// #define ENABLE_DEBUG_LOG_KEYWORD
// #define ENABLE_DEBUG_LOG_OUTPUT
bool ControlExcel::replaceGenDataInfo() {
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    bool result = true;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    qDebug() << "\n\n=========================================================================================================";

    for (int originIndex = originStart; originIndex < originEnd; ++originIndex) {
        int convertIndex = convertStart + (originIndex - originStart);
        updateDataControl(convertIndex, getData(originIndex).toList());
    }
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForSheet = constructKeywordTypeInfoList(originStart, originEnd);
    constructConvertSheetDataInfo(keywordTypeInfoListForSheet);

    qDebug() << "\n\n=========================================================================================================";

    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForNonSheet = constructKeywordTypeInfoList(convertStart, convertEnd);
    constructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

    qDebug() << "\n=========================================================================================================\n\n";
    // constructAllCasesSignalSet();
    qDebug() << "\n=========================================================================================================\n\n";
    return result;
}

void ControlExcel::constructGenDataInfo() {
    ivis::common::CheckTimer checkTimer;

#if 1
    // isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants,
    //                       QString("Constant_TCName"), QString("Result1"), QString("Case1_INVALID1"));

    isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants,
                          QString("Constant_TCName"), QString(""), QString(""));

#else
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    QList<QPair<QString, QString>> signalList = QList<QPair<QString, QString>>();
    QList<QPair<QString, QString>> signalListOutput = QList<QPair<QString, QString>>();
    // QStringList signalNameList = QStringList();

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        for (const auto& sheetDataList : getData(sheetIndex).toList()) {
            if (sheetDataList.toList().size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                qDebug() << "Fail to sheet data list size :" << sheetDataList.toList().size();
                continue;
            }
            QStringList rowDataList = sheetDataList.toStringList();
            QString inputSignal = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            QString inputData = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
            QString outputSignal = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
            QString outputData = rowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
            if (inputSignal.size() > 0) {
                signalList.append(QPair<QString, QString>(inputSignal, inputData));
            }
            if (outputSignal.size() > 0) {
                signalListOutput.append(QPair<QString, QString>(outputSignal, outputData));
            }
            // signalNameList.append(inputSignal);
            // signalNameList.append(outputSignal);
        }
    }
    // signalNameList.removeAll("");
    // signalNameList.removeDuplicates();

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
    for (const auto& signal : signalList) {
        QString signalName = signal.first;
        QString signalDataTemp = signal.second;
        QStringList signalData = signalDataTemp.remove(" ").split(",");
        QMap<int, QStringList> dataInfo =
            isSignalDataInfo(true, signalName, signalData, isSignalFileList(signalName, vehicleType));

        if (dataInfo.size() > 0) {
            int keywordType = isKeywordType(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData), signalDataTemp);
            TestCase::instance().data()->setSignalDataInfo(signalName, dataInfo, keywordType);
        }

#if 0
        QString dataType;
        int keywordType = 0;
        QMap<int, QStringList> signalDataInfo = TestCase::instance().data()->getSignalDataInfo(signalName, keywordType, dataType);
#endif
    }
#endif

    checkTimer.check("constructGenDataInfo");
}

bool ControlExcel::appendConvertDataInfo() {
    bool result = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
    qDebug() << "@@@@ appendConvertDataInfo";
    qDebug() << "@@@@ 1 "
             << isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants, "Constant_TCName",
                                      "Result1", "Case1_INVALID1");
    qDebug() << "@@@@ 2 "
             << isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants, "Constant_TCName",
                                      "Result1", "Case1_INVALID2");
    qDebug() << "@@@@ 3 "
             << isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants, "Constant_TCName",
                                      "Result1", "Case1_INVALID3");
    qDebug() << "@@@@ 4 "
             << isInputSignalDataInfo(false, ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConstants, "Constant_TCName",
                                       "", "");
#endif
    return result;
}

QMap<int, QList<KeywordInfo>> ControlExcel::constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex) {
    QMap<int, QList<KeywordInfo>> keywordTypeInfoList;

    for (int sheetIndex = startSheetIndex; sheetIndex < endSheetIndex; ++sheetIndex) {
        QList<KeywordInfo> keywordTypeInfo = isKeywordTypeInfo(sheetIndex);
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
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

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
                    originRowDataStr.remove(excelMergeStart.toString());
                    originRowDataStr.remove(excelMergeEnd.toString());
                    originRowDataStr.remove(excelMerge.toString());
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
                        vehicleTypeStr.remove(excelMergeStart.toString());
                        vehicleTypeStr.remove(excelMergeEnd.toString());
                        vehicleTypeStr.remove(excelMerge.toString());
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

    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        int originIndex = iter.key();
        int convertIndex = 0;
        int rowIndex = 0;
        QVariantList convertRowData;

        bool isNotAppendOriginTCNameMergeStart = false;
        bool isNotAppendOriginResultMergeStart = false;
        QStringList tmpNotAppendMergeKeywordOriginList;

        for (const auto& rowDataList : getData(originIndex).toList()) {
            QList<QStringList> convertData;
            QList<QStringList> rowData;
            KeywordInfo curKeywordInfo;

            bool isEqualData = false;

            const bool isOriginTCNameMergeStart = rowDataList.toList()
                                                      .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                                      .toString()
                                                      .trimmed()
                                                      .startsWith(excelMergeStart.toString());
            const bool isOriginTCNameMergeEnd = rowDataList.toList()
                                                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                                    .toString()
                                                    .trimmed()
                                                    .startsWith(excelMergeEnd.toString());
            const bool isOriginVehicleTypeMergeStart =
                rowDataList.toList()
                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                    .toString()
                    .trimmed()
                    .startsWith(excelMergeStart.toString());
            const bool isOriginVehicleTypeMergeEnd = rowDataList.toList()
                                                         .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                                         .toString()
                                                         .trimmed()
                                                         .startsWith(excelMergeEnd.toString());
            const bool isOriginResultMergeStart = rowDataList.toList()
                                                      .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                      .toString()
                                                      .trimmed()
                                                      .startsWith(excelMergeStart.toString());
            const bool isOriginResultMerge = rowDataList.toList()
                                                 .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                 .toString()
                                                 .trimmed()
                                                 .startsWith(excelMerge.toString());
            const bool isOriginResultMergeEnd = rowDataList.toList()
                                                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                    .toString()
                                                    .trimmed()
                                                    .startsWith(excelMergeEnd.toString());

            // Origin Row Data가 [Sheet] Keyword에 존재 여부 판단 로직
            for (KeywordInfo keyword : iter.value().toList()) {
                if ((rowIndex == keyword.isRow()) &&
                    (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                    convertData = keyword.isConvertData();
                } else {
                    isEqualData =
                        isDataAlreadyExistInKeywordInfoList(rowDataList.toStringList(), keyword, originIndex, isEqualData);
                }
            }

            if (convertData.size() == 0) {
#if 0
                if (originIndex != static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription)) {
                    for (auto rowData : curKeywordInfo.isRowData()) {
                        qDebug() << "########################################### [11111] #########################################################";
                        qDebug() << "> TC Name     : "<< rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                        qDebug() << "> VehicleType : " << rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                        qDebug() << "> Result      : " << rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                        qDebug() << "> Case        : " << rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
                        qDebug() << "> InputSignal : " << rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                        qDebug() << "> InputData   : " << rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
                        qDebug() << "########################################### [22222] #########################################################";
                        qDebug() << "> TC Name     : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                        qDebug() << "> VehicleType : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                        qDebug() << "> Result      : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                        qDebug() << "> Case        : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
                        qDebug() << "> InputSignal : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                        qDebug() << "> InputData   : " << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
                    }
                }
                qDebug() << "> " << curKeywordInfo.isRowData();
                qDebug() << "############################################################################################################";
#endif
                if (isEqualData == false) {
                    convertRowData.append(rowDataList);
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 append rowDataList (not in keyword info list) : " << rowDataList;
#endif
                } else {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 [PASS] not append rowDataList : " << rowDataList;
#endif
                    // [Sheet] Keyword가 [MergeStart]TCName || [MergeStart]Result 이후 row_data에 존재하는 경우에 처리하는 로직
                    if (isOriginTCNameMergeStart == true) {
                        isNotAppendOriginTCNameMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList
                            << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString()
                            << rowDataList.toList()
                                   .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                   .toString()
                            << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).toString();
                    } else if (isOriginResultMergeStart == true) {
                        isNotAppendOriginResultMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList
                            << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString()
                            << rowDataList.toList()
                                   .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                   .toString()
                            << rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).toString();
                    } else {
                        // no operation
                    }

                    if (convertRowData.length() > 0) {
                        QStringList tmpConvertList = convertRowData[convertRowData.length() - 1].toStringList();
                        if (isOriginTCNameMergeEnd == true) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                                rowDataList.toList()
                                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                    .toString();
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }
                        if (isOriginVehicleTypeMergeEnd == true) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                                rowDataList.toList()
                                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                    .toString();
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }
                        if (isOriginResultMergeEnd == true) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "OriginResult before >>>>> Result [MergeEnd] : "
                                     << convertRowData[convertRowData.length() - 1]
                                            .toList()[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)]
                                     << " <= "
                                     << rowDataList.toList()
                                            .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                            .toString();
#endif
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                                rowDataList.toList()
                                    .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                    .toString();
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
                            if ((isOriginTCNameMergeStart == true) && (isOriginVehicleTypeMergeStart == true) && (idx == 0)) {
                                tcNameStr = createMergeInfoString(
                                    "[MergeStart]", rowDataList.toList()
                                                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                                        .toString());
                                vehicleTypeStr = createMergeInfoString(
                                    "[MergeStart]", rowDataList.toList()
                                                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                                        .toString());
                            } else if ((isOriginTCNameMergeEnd == true) && (isOriginVehicleTypeMergeEnd == true) &&
                                       (idx == convertDataListLength - 1)) {
                                tcNameStr = createMergeInfoString(
                                    "[MergeEnd]", rowDataList.toList()
                                                      .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                                      .toString());
                                vehicleTypeStr = createMergeInfoString(
                                    "[MergeEnd]", rowDataList.toList()
                                                      .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                                      .toString());
                            } else {
                                tcNameStr = createMergeInfoString(
                                    "[Merge]", rowDataList.toList()
                                                   .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))
                                                   .toString());
                                vehicleTypeStr = createMergeInfoString(
                                    "[Merge]", rowDataList.toList()
                                                   .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                                   .toString());
                            }

                            // Result Merge 기능
                            if (((isOriginResultMergeStart == true) ||
                                 ((isOriginResultMergeEnd == false && isOriginResultMerge == false))) &&
                                (idx == 0)) {
                                resultStr = createMergeInfoString(
                                    "[MergeStart]", rowDataList.toList()
                                                        .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                        .toString());
                            } else if (((isOriginResultMergeEnd == true) ||
                                        (isOriginResultMerge == false && isOriginResultMergeStart == false)) &&
                                       (idx == convertDataListLength - 1)) {
                                resultStr = createMergeInfoString(
                                    "[MergeEnd]", rowDataList.toList()
                                                      .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                      .toString());
                            } else {
                                resultStr = createMergeInfoString(
                                    "[Merge]", rowDataList.toList()
                                                   .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))
                                                   .toString());
                            }
                        }
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = tcNameStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = vehicleTypeStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = resultStr;

                        // [Sheet] 키워드 사용하는 Case이름과 [Sheet] 키워드의 Case 이름을 조합하는 로직
                        // e.g)
                        // Private Sheet의 Case Name         ->  VALID1
                        // Constant [Sheet]Private Case Name -> [MergeStart]Case1
                        // 결과 : [MergeStart]Case1_VALID1
                        QString originRowDataCaseStr =
                            rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)).toString();
                        originRowDataCaseStr.remove(excelMergeStart.toString());
                        originRowDataCaseStr.remove(excelMergeEnd.toString());
                        originRowDataCaseStr.remove(excelMerge.toString());
                        QString prefix =
                            tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].section("]", 0,
                                                                                                                     0) +
                            "]";  // prefix : [MergeStart]
                        QString suffix = tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].section(
                            "]", 1);  // suffix : Case Column String
                        QString resultCaseStr = prefix + originRowDataCaseStr + "_" +
                                                suffix;  // case   : [MergeStart] + Origin_Row_Case_String + Case_Column_String
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = resultCaseStr;

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
                            rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                            rowDataList.toList()
                                .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType))
                                .toString();
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                            rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).toString();

                        QString originRowDataCaseStr =
                            rowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)).toString();
                        originRowDataCaseStr.remove(excelMergeStart.toString());
                        originRowDataCaseStr.remove(excelMergeEnd.toString());
                        originRowDataCaseStr.remove(excelMerge.toString());
                        QString suffix = tmpConvertDataList[static_cast<int>(
                            ivis::common::ExcelSheetTitle::Other::Case)];  // suffix : Case Column String
                        QString resultCaseStr =
                            originRowDataCaseStr + "_" + suffix;  // case : Origin_Row_Case_String + Case_Column_String

                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = resultCaseStr;
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
            qDebug() << "[Convert Keyword] Origin[" << originIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowData.size();
            for (const auto& rowData : convertRowData) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }

    // output signal & config signal 처리
    for (int convertSheetIndex = convertSheetStartIndex; convertSheetIndex < convertSheetEndIndex; ++convertSheetIndex) {
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
        qDebug() << "=========================================================================================================";
        qDebug() << "> convertSheetIndex : " << convertSheetIndex << "\n";
#endif
        int outputDataListIndex = 0;
        int configDataListIndex = 0;

        bool isConvertTCNameMergeStart = false;
        bool isConvertTCNameMergeEnd = false;
        bool isConvertResultMergeStart = false;
        bool isConvertResultMergeEnd = false;

        QString currentTCNameStr;
        QString currentResultStr;
        QVariantList convertRowData;

        for (const auto& convertRowDataList : getData(convertSheetIndex).toList()) {
            QStringList tmpOutputConvertData;
            QString convertTCNameDataStr =
                convertRowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).toString();
            QString convertResultDataStr =
                convertRowDataList.toList().at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).toString();

            // TCName [MergeStart] ~
            if (isConvertTCNameMergeStart == false) {
                isConvertTCNameMergeStart = convertTCNameDataStr.trimmed().startsWith(excelMergeStart.toString());
                if (isConvertTCNameMergeStart == true) {
                    convertTCNameDataStr.remove(excelMergeStart.toString());
                    isConvertTCNameMergeEnd = false;
                    currentTCNameStr = convertTCNameDataStr;
                }
            }
            // TCName [MergeStart] ~ TCName [MergeEnd]
            if (isConvertTCNameMergeStart == true && isConvertTCNameMergeEnd == false) {
                isConvertTCNameMergeEnd = convertTCNameDataStr.trimmed().startsWith(excelMergeEnd.toString());
                if (isConvertTCNameMergeEnd == true) {
                    convertTCNameDataStr.remove(excelMergeEnd.toString());
                    isConvertTCNameMergeStart = false;
                    configDataListIndex = 0;
                }
            }

            // Result [MergeStart] ~
            if (isConvertResultMergeStart == false) {
                isConvertResultMergeStart = convertResultDataStr.trimmed().startsWith(excelMergeStart.toString());
                if (isConvertResultMergeStart == true) {
                    convertResultDataStr.remove(excelMergeStart.toString());
                    isConvertResultMergeEnd = false;
                    currentResultStr = convertResultDataStr;
                }
            }
            // Result [MergeStart] ~ Result [MergeEnd]
            if (isConvertResultMergeStart == true && isConvertResultMergeEnd == false) {
                isConvertResultMergeEnd = convertResultDataStr.trimmed().startsWith(excelMergeEnd.toString());
                if (isConvertResultMergeEnd == true) {
                    convertResultDataStr.remove(excelMergeEnd.toString());
                    isConvertResultMergeStart = false;
                    outputDataListIndex = 0;
                }
            }

            // TCName [MergeStart] && (Result [Mergestart] ~ [MergeEnd])인 경우에 Output(Signal, IsInitialize, Value) 정보 존재
            if (isConvertTCNameMergeStart == true && isConvertResultMergeStart == true) {
                int convertIndexToOriginIndex =
                    convertSheetIndex - static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) +
                    static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
                QList<QStringList> tmpOutputRowData =
                    isOutputDataInfo(convertIndexToOriginIndex, currentTCNameStr, currentResultStr);
                QList<QStringList> tmpConfigRowData =
                    isConfigDataInfo(convertIndexToOriginIndex, currentTCNameStr, currentResultStr);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                qDebug() << convertRowDataList.toList();
#endif
                tmpOutputConvertData = convertRowDataList.toStringList();
                if (outputDataListIndex < tmpOutputRowData.length()) {
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                    qDebug() << "1 Output Data List : " << tmpOutputRowData.at(outputDataListIndex);
#endif
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

                // TCName [MergeStart] 인 경우에 Config(Signal, Data) 정보 존재
                if (configDataListIndex < tmpConfigRowData.length()) {
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                    qDebug() << "2 Config Data List : " << tmpConfigRowData.at(configDataListIndex);
#endif
                    tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)] =
                        tmpConfigRowData.at(configDataListIndex)
                            .at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal));
                    tmpOutputConvertData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)] =
                        tmpConfigRowData.at(configDataListIndex).at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
                    configDataListIndex++;
                }
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

void ControlExcel::constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        // int originIndex = iter.key();
        int convertIndex = iter.key();
        int rowIndex = 0;
        QVariantList convertRowDataList;

        for (const auto& rowDataList : getData(convertIndex).toList()) {
            QStringList convertRowData;
            for (KeywordInfo keyword : iter.value().toList()) {
                if ((rowIndex == keyword.isRow()) &&
                    (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over))) {
                    convertRowData = rowDataList.toStringList();
                    QString overValue = QString("%1%2, %3")
                                            .arg(">")
                                            .arg(QString::number(keyword.isText().toULongLong()))
                                            .arg(QString::number(keyword.isText().toULongLong() + 1));
                    convertRowData[keyword.isColumn()] = overValue;
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                    qDebug() << "[Over] keyword Type    : " << keyword.isKeyword();
                    qDebug() << "[Over] keyword Data    : " << keyword.isText();
                    qDebug() << "[Over] Keyword RowData : " << convertRowData;
                    qDebug() << "[Over] Converted Value : " << overValue;
#endif
                } else if ((rowIndex == keyword.isRow()) &&
                           (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under))) {
                    convertRowData = rowDataList.toStringList();
                    QString underValue = QString("%1%2, %3")
                                             .arg("<")
                                             .arg(QString::number(keyword.isText().toULongLong()))
                                             .arg(QString::number(keyword.isText().toULongLong() - 1));
                    convertRowData[keyword.isColumn()] = underValue;
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                    qDebug() << "[Under] keyword Type    : " << keyword.isKeyword();
                    qDebug() << "[Under] keyword Data    : " << keyword.isText();
                    qDebug() << "[Under] Keyword RowData : " << convertRowData;
                    qDebug() << "[Under] Converted Value : " << underValue;
#endif
                } else if ((rowIndex == keyword.isRow()) &&
                           (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range))) {
                    convertRowData = rowDataList.toStringList();
                    QStringList rangeValue = keyword.isText().remove(" ").split(",");
                    if (rangeValue.length() == 2) {
                        quint64 rangeLValue = rangeValue.at(0).toULongLong();
                        quint64 rangeRValue = rangeValue.at(1).toULongLong();
                        QString rangeValue = QString("%1%2, %3, %4, %5")
                                                 .arg("~")
                                                 .arg(QString::number(rangeLValue))
                                                 .arg(QString::number(rangeLValue + 1))
                                                 .arg(QString::number(rangeRValue - 1))
                                                 .arg(QString::number(rangeRValue));
                        convertRowData[keyword.isColumn()] = rangeValue;
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
                        qDebug() << "[Range] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Range] keyword Data    : " << keyword.isText();
                        qDebug() << "[Range] Keyword RowData : " << convertRowData;
                        qDebug() << "[Range] Converted Value : " << rangeValue;
#endif
                    } else {
                        qDebug() << "[Warning] Range Value Format Error : " << keyword.isText();
                    }
                } else {
                    // no operation
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
            qDebug() << "[Convert Keyword] Convert [" << convertIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowDataList.size();
            for (const auto& rowData : convertRowDataList) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
}

// TODO(csh): keyword(부등호, 연산 ...) 기능 구현 완료 후 개발 예정
void ControlExcel::constructAllCasesSignalSet() {
    // NOTE(csh):
    // 각 Test Case마다 겹치지 않게 가지고 있는 signal들의 조합 set을 구성하는 함수
    // e.g) format
    //      > 1280 ----> > 1280, 1281
}

inline QString ControlExcel::createMergeInfoString(const QString& mergeKeyword, const QString& data) const {
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);

    QString tmpStr = data;

    tmpStr.remove(excelMergeStart.toString());
    tmpStr.remove(excelMergeEnd.toString());
    tmpStr.remove(excelMerge.toString());
    return mergeKeyword + tmpStr;
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
