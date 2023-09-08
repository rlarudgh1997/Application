#include "ControlExcel.h"
#include "HandlerExcel.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"


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

bool ControlExcel::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlExcel::initCommonData(const int& currentMode) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, ivis::common::ScreenEnum::DisplayTypeExcel);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlExcel::initNormalData() {
    resetControl(false);

#if defined(USE_EXCEL_FUNCTION_NEW)
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, QStringList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, QVariantList());
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, QVariant(false));
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, 0);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, 0);
#endif

    QString excelBlankText = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelBlankText).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelBlankText, excelBlankText);
    QVariant nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath);
    QStringList sfcList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressSFC.info");
    QStringList vsmList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressVSM.info");
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll, (sfcList + vsmList),  true);
}

void ControlExcel::initControlData() {
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeSaveFilePath, "");
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeEditExcelSheet, false);
}

void ControlExcel::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlExcel::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerExcel::signalHandlerEvent,
                this,                              &ControlExcel::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlExcel::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlExcel::slotEventInfoChanged,
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

void ControlExcel::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlExcel::keyEvent(const int& inputType, const int& inputValue) {
    if (inputType == ivis::common::KeyTypeEnum::KeyInputTypeRelease) {
        if (inputValue == Qt::Key_F2) {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 1, true);
        } else if ((inputValue == ivis::common::KeyTypeEnum::KeyInputValueOK)
#if defined(PLATFORM_X86)
            || (inputValue == ivis::common::KeyTypeEnum::KeyInputValueNumOK)
            || (inputValue == ivis::common::KeyTypeEnum::KeyInputValueNumOK2)
#endif
            ) {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 0, true);
        } else if (inputValue == Qt::Key_Escape) {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 2, true);
        } else {
        }
    }
}

void ControlExcel::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
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

#if defined(USE_EXCEL_FUNCTION_NEW)
void ControlExcel::updateExcelSheet(const int& propertyType, const QVariant& dirPath) {
    qDebug() << "ControlExcel::updateExcelSheet() ->" << propertyType << "," << dirPath;

    bool excelOpen = (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen);
    QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    QStringList otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QVariantList rowCount = QVariantList();

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, sheetName);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, descTitle);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, otherTitle);

    if (excelOpen) {
        int sheetIndex = 0;

        foreach(const auto& sheet, sheetName) {
            QVariantList sheetData = QVariantList();
            QString filePath = QString("%1/%2_%3.fromExcel").arg(dirPath.toString()).arg(sheetIndex).arg(sheet);
            QStringList readData = ivis::common::FileInfo::readFile(filePath);

            for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
                if (rowIndex <= 1) {
                    // RowIndex 0 : column 인덱스 정보
                    // RowIndex 1 : title(desc, other) 정보
                    continue;
                }
                QStringList rowDataList = readData[rowIndex].split("\t");
                // qDebug() << "rowDataList :" << rowDataList.size() << "," << rowDataList;
                sheetData.append(rowDataList);
            }
            rowCount.append(sheetData.size());
            updateDataHandler((ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription + sheetIndex++), sheetData);

            // qDebug() << "File Open :" << filePath << ", Length :" << sheetData.size();
            // qDebug() << sheet << ":" << sheetData;
            // qDebug() << "==================================================================================================\n";
        }

        // Delete Folder
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDeleteFileTC).toBool()) {
            QStringList log;
            ivis::common::ExcuteProgram process(false);
            process.start(QString("rm -rf %1/*").arg(dirPath.toString()), log);
        }
    } else {
        int rowMax = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
        foreach(const auto& sheet, sheetName) {
            rowCount.append(rowMax);
        }
    }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, rowCount);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, excelOpen, true);
}
#else
void ControlExcel::updateSheetInfo(const int& propertyType, const QVariant& dirPath) {
    qDebug() << "ControlExcel::updateSheetInfo() ->" << propertyType << "," << dirPath;
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> excelDataInfo = QMap<int, QVariantList>();
    QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
    int rowCount = 0;

    switch (propertyType) {
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew : {
            initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
            rowCount = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
            foreach(const auto& sheet, sheetName) {
                QVariant title = QVariant();

                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet] = sheet;
                if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
                } else {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle);
                }
                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count] =
                                                    QVariant(QVariantList({rowCount, title.toList().count()}));
                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title] = title;

                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    int columnCount = dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title].toList().count();
                    QVariantList columnInfo = QVariantList();
                    for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
#if defined(USE_EXCEL_CELL_INFO_TEMP)
                        columnInfo.append(QString("%1[%2_%3]").arg(sheet.toString()).arg(rowIndex).arg(columnIndex));
#else
                        columnInfo.append("");
#endif
                    }
                    dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex] = columnInfo;
                }
                excelDataInfo[sheetIndex] = dataInfo.values();
                sheetIndex++;
#if 0
                qDebug() << "\n\n=====================================================================================";
                qDebug() << "Sheet :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet];
                qDebug() << "Count :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count];
                qDebug() << "Title :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title];
                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    qDebug() << "Data :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex];
                }
#endif
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
            foreach(const auto& sheet, sheetName) {
                int fileIndex = sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
                QVariant title = QVariant();
                QString filePath = QString("%1/%2_%3.fromExcel").arg(dirPath.toString()).arg(fileIndex).arg(sheet.toString());
                QStringList readData = ivis::common::FileInfo::readFile(filePath);
                rowCount = readData.size();

                if (rowCount == 0) {
                    qDebug() << "Fail to read file length :" << rowCount << ", path :" << filePath;
                    continue;
                }

                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet] = sheet;
                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    QStringList columnInfo = readData[rowIndex].split("\t");
                    if (rowIndex == 0) {
                        // Row[0] = column index number info -> do not used
                    } else if (rowIndex == 1) {
                        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count] = QVariant(QVariantList({
                                                                        rowCount - 2,    // RowCount = 1 ~ (Max - 2)
                                                                        columnInfo.count()}));
                        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title] = columnInfo;
                    } else {
                        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex - 1] = columnInfo;
                    }
                }
                excelDataInfo[sheetIndex] = dataInfo.values();
                sheetIndex++;

                qDebug() << "Delete Path Info :" << dirPath << ", " << filePath;
            }

            bool deleteFile = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDeleteFileTC).toBool();
            if (deleteFile) {
                ivis::common::ExcuteProgram process(false);
                QStringList log;
                process.start(QString("rm -rf %1/*").arg(dirPath.toString()), log);
            }
            break;
        }
        default : {
            break;
        }
    }

    if (excelDataInfo.size() > 0) {
        QMapIterator<int, QVariantList> iter(excelDataInfo);
        while (iter.hasNext()) {
            iter.next();
            updateDataHandler(iter.key(), iter.value());
        }
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet, false);
        updateDataHandler(initPropertyType, 0);
        updateDataHandler(propertyType, excelDataInfo.size(), true);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    }
}
#endif

bool ControlExcel::updateSheetTextInfo(const QVariant& textInfo) {
    if (textInfo.toList().size() != 4) {
        qDebug() << "Fail to update sheet text info size :" << textInfo.toList().size();
        return false;
    }

    int sheetIndex = textInfo.toList()[0].toInt();
    int column = textInfo.toList()[1].toInt();
    int row = textInfo.toList()[2].toInt() + ivis::common::CellInfoEnum::ListInfoExcel::Data;
    QString text = textInfo.toList()[3].toString();

    QVariantList detailInfo = getData(sheetIndex).toList();
    QVariantList updateData = QVariantList();
    int rowIndex = 0;

    foreach(const auto& detail, detailInfo) {
        int columnIndex = 0;
        if (rowIndex < ivis::common::CellInfoEnum::ListInfoExcel::Data) {
            updateData.append(detail);
        } else {
            QVariantList rowData = QVariantList();
            foreach(const auto& info, detail.toList()) {
                if ((row == rowIndex) && (column == columnIndex)) {
                    rowData.append(text);
                } else {
                    rowData.append(info.toString());
                }
                columnIndex++;
            }
            updateData.append(QVariant(rowData));
        }
        rowIndex++;
    }

    updateDataHandler(sheetIndex, updateData);

    return true;
}

bool ControlExcel::updateSheetCellInfo(const QVariant& cellInfo) {
    if (cellInfo.toList().size() != 6) {
        qDebug() << "Fail to update sheet cell info size :" << cellInfo.toList().size();
        return false;
    }

    int initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeInvalid;
    int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInvalid;

    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew).toInt() > 0) {
        initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
        propertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
    } else if (getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen).toInt() > 0) {
        initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
        propertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
    } else {
        qDebug() << "Fail to previous sheet info size : 0";
        return false;
    }

    int editType = cellInfo.toList()[0].toInt();
    int sheetIndex = cellInfo.toList()[1].toInt();
    int columnSelect = cellInfo.toList()[2].toInt();
    int columnEnd = cellInfo.toList()[3].toInt();
    int rowStart = cellInfo.toList()[4].toInt() + ivis::common::CellInfoEnum::ListInfoExcel::Data;
    int rowEnd = cellInfo.toList()[5].toInt();
    QVariantList updateData = QVariantList();

    switch (editType) {
        case ivis::common::EditCellEnum::EditCellInfo::Insert :
        case ivis::common::EditCellEnum::EditCellInfo::Delete : {
            updateData = isInsertDeleteCellInfo(editType, sheetIndex, columnSelect, rowStart, rowEnd);
            break;
        }
        case ivis::common::EditCellEnum::EditCellInfo::Merge :
        case ivis::common::EditCellEnum::EditCellInfo::Split : {
            rowEnd = (rowEnd + rowStart);
            updateData = isMergeSplitCellInfo(editType, sheetIndex, columnSelect, rowStart, rowEnd);
            break;
        }
        default : {
            break;
        }
    }

    bool update = (updateData.size() > 0);

    qDebug() << editType << ". Sheet[" << sheetIndex << "] :" << columnSelect << "," << columnEnd
                         << "," << rowStart << "," << rowEnd << ", Update :" << update << "," << updateData.size();
#if 0
    foreach(const auto& data, updateData) {
        qDebug() << "\t Data :" << data;
    }
#endif

    if (update) {
        updateDataHandler(sheetIndex, updateData);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet, true);
        updateDataHandler(initPropertyType, 0);
        updateDataHandler(propertyType, getData(propertyType).toInt(), true);
    }
    return update;
}

#if defined(USE_EXCEL_FUNCTION_NEW)
bool ControlExcel::writeExcelSheet(const QVariant& filePath) {
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
    int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int writeSize = 0;

    foreach(const auto& sheet, sheetName) {
        int sheetIndex = (propertyType - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription);
        QString file = QString("%1_%2.toExcel").arg(sheetIndex).arg(sheet);
        QString writeData = QString();
        QVariantList sheetData = getData(propertyType++).toList();

        qDebug() << "File :" << file;

        foreach(const auto& dataInfo, sheetData) {
            QString rowData = QString();
            int count = 0;
            foreach(QVariant info, dataInfo.toList()) {
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
            int size = ivis::common::FileInfo::writeFile(writeFielPath, writeData);
            writeSize += size;
            if (size == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << writeFielPath;
            }
        }
    }
    return (writeSize > 0);
}
#endif

void ControlExcel::writeExcelFile(const QVariant& filePath) {
    if (checkPythonLibrary() == false) {
        qDebug() << "Faitl to python lib not install.";
        return;
    }

#if defined(USE_EXCEL_FUNCTION_NEW)
    bool write = writeExcelSheet(filePath);
#else
    bool write = writeSheetInfo(filePath);
#endif
    if (write) {
        if (sytemCall(ivis::common::EventTypeEnum::EventTypeSaveExcel, filePath) > 0) {
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastFileInfo, filePath);
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeEditExcelSheet, false);
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeFileSaveType, false);
        }
    }
}

bool ControlExcel::writeSheetInfo(const QVariant& filePath) {
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> excelDataInfo = QMap<int, QVariantList>();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int writeSize = 0;

    foreach(const auto& sheet, sheetName) {
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
    foreach(const auto& detailInfo, excelDataInfo) {
        int index = 0;
        QString writeData = QString();
        foreach(const auto& detail, detailInfo) {
            if (index == ivis::common::CellInfoEnum::ListInfoExcel::Sheet) {
                file = QString("%1_%2.toExcel").arg(sheetIndex++).arg(detail.toString());
            } else if (index >= ivis::common::CellInfoEnum::ListInfoExcel::Title) {
                QString infoData = QString();
                int count = 0;
                foreach(QVariant info, detail.toList()) {
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
            QString saveFilePaht = QString("%1/%2").arg(savePath).arg(file);
            writeSize = ivis::common::FileInfo::writeFile(saveFilePaht, writeData);
            if (writeSize == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << saveFilePaht;
            }
        }
    }
    return (writeSize > 0);
}

QString ControlExcel::sytemCall(const int& type, const QVariant& filePath) {
    qDebug() << "ControlExcel::sytemCall() ->" << type << "," << filePath;
    QString cmdType = ((type == ivis::common::EventTypeEnum::EventTypeOpenExcel) ? ("read") : ("write"));
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
    if ((fileName.contains(".xlsx", Qt::CaseInsensitive) == false)
        || (fileName.contains(".xls", Qt::CaseInsensitive) == false)) {
        fileName.append(".xlsx");
    }

    QString cmd = QString("python %1/excel_parsing.py %2 %3 %4").arg(APP_PWD).arg(dirPath).arg(fileName).arg(cmdType);
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);

    if (result) {
        dirPath.append("TC");
    } else {
        dirPath.clear();
    }

    qDebug() << "*************************************************************************************************";
    qDebug() << "PWD      :" << APP_PWD;
    qDebug() << "System   :" << ((result) ? ("<sucess>") : ("<fail>")) << cmd;
    qDebug() << "FilePath :" << filePath;
    qDebug() << "DirPath  :" << dirPath;
    foreach(const auto& d, log) {
        qDebug() << "LogData  :" << d;
    }
    qDebug() << "*************************************************************************************************\n";

    return dirPath;
}

bool ControlExcel::checkPythonLibrary() {
    bool openpyxl = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl).toBool();
    bool pandas = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibPandas).toBool();
    bool checkLib = ((openpyxl) && (pandas));

    if (checkLib == false) {
        ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
        QVariantList text = QVariantList({STRING_POPUP_LIB, STRING_POPUP_CONFIRM,
                                            STRING_POPUP_INSTALL, STRING_POPUP_CONFIRM});
        QVariant popupData = QVariant();
        button = ivis::common::Popup::drawPopup(ivis::common::PopupType::NoInstallLib, isHandler(), popupData, QVariant(text));
        if (button == ivis::common::PopupButton::Install) {
            mProcess.data()->setCommandInfo(QString("pip install openpyxl pandas"));
            mProcess.data()->start();
            connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramCompleted,
                                                                                    [&](const bool& result) {
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
}

QVariantList ControlExcel::isInsertDeleteCellInfo(const int& editType, const int& sheetIndex, const int& columnSelect,
                                                                const int& rowStart, const int& rowEnd) {
    QVariantList detailInfo = getData(sheetIndex).toList();
    QVariantList updateData = QVariantList();
    int columnIndex = 0;
    int rowIndex = 0;

    if ((detailInfo.size() == ivis::common::CellInfoEnum::ListInfoExcel::Data)
        && (editType == ivis::common::EditCellEnum::EditCellInfo::Insert)) {
        QVariant title = QVariant();
        QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
        if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
            title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
        } else {
            title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle);
        }

        QVariantList rowData = QVariantList();
        foreach(const auto& t, title.toList()) {
#if defined(USE_EXCEL_CELL_INFO_TEMP)
            rowData.append(QVariant(QString("CellAdd[0, %2]").arg(columnIndex)));
#else
            rowData.append(QVariant(""));
#endif
            columnIndex++;
        }

        int sheetNameIndex = sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
        updateData.append(QVariant(sheetName[sheetNameIndex]));
        updateData.append(QVariant(QVariantList({1, title.toList().count()})));
        updateData.append(title);
        updateData.append(QVariant(rowData));
    } else {
        foreach(const auto& detail, detailInfo) {
            if (rowIndex < ivis::common::CellInfoEnum::ListInfoExcel::Data) {
                updateData.append(detail);
            } else {
                if (editType == ivis::common::EditCellEnum::EditCellInfo::Insert) {
                    if (rowIndex == rowStart) {
                        for (int index = 0; index < rowEnd; index++) {
                            QVariantList rowData = QVariantList();
                            foreach(const auto& info, detail.toList()) {
#if defined(USE_EXCEL_CELL_INFO_TEMP)
                                rowData.append(QVariant(QString("CellAdd[%1, %2]").arg(index).arg(columnIndex)));
#else
                                rowData.append(QVariant(""));
#endif
                                columnIndex++;
                            }
                            updateData.append(QVariant(rowData));
                        }
                    }
                    updateData.append(detail);
                } else {
                    if ((rowIndex < rowStart) || (rowIndex >= (rowStart + rowEnd))) {
                        updateData.append(detail);
                    }
                }
            }
            rowIndex++;
        }

        QVariantList count = updateData[ivis::common::CellInfoEnum::ListInfoExcel::Count].toList();
        int rowCount = updateData.size() - ivis::common::CellInfoEnum::ListInfoExcel::Data;
        int columnCount = count[1].toInt();
        updateData[ivis::common::CellInfoEnum::ListInfoExcel::Count] = QVariant(QVariantList({rowCount, columnCount}));
    }

    return updateData;
}

QVariantList ControlExcel::isMergeSplitCellInfo(const int& editType, const int& sheetIndex, const int& columnSelect,
                                                                const int& rowStart, const int& rowEnd) {
    if ((rowEnd - rowStart) == 1) {
        qDebug() << "Fail to select cell - count : 1";
        return QVariantList();
    }

    QVariantList detailInfo = getData(sheetIndex).toList();
    QVariantList updateData = QVariantList();
    QString excelBlankText = getData(ivis::common::PropertyTypeEnum::PropertyTypeExcelBlankText).toString();
#if defined(USE_EXCEL_CELL_INFO_TEMP)
    QString str = (editType == ivis::common::EditCellEnum::EditCellInfo::Merge) ? ("") : (excelBlankText);
#else
    QString str = (editType == ivis::common::EditCellEnum::EditCellInfo::Merge) ? ("ExcelMergeText") : (excelBlankText);
#endif
    int rowIndex = 0;
    int startIndex = 0;

#if defined(USE_EXCEL_CELL_INFO_TEMP)
    foreach(const auto& detail, detailInfo) {
        if (rowIndex < ivis::common::CellInfoEnum::ListInfoExcel::Data) {
            updateData.append(detail);
        } else {
            if ((rowIndex > rowStart) && (rowIndex < rowEnd)) {
                int columnIndex = 0;
                QVariantList rowData = QVariantList();
                foreach(const auto& info, detail.toList()) {
                    rowData.append((columnSelect == columnIndex) ? (str) : (info));
                    columnIndex++;
                }
                updateData.append(QVariant(rowData));
            } else {
                updateData.append(detail);
            }
        }
        rowIndex++;
    }
#else
    foreach(const auto& detail, detailInfo) {
        if (rowIndex < ivis::common::CellInfoEnum::ListInfoExcel::Data) {
            updateData.append(detail);
        } else {
            if (editType == ivis::common::EditCellEnum::EditCellInfo::Merge) {
                if ((rowIndex >= rowStart) && (rowIndex < rowEnd)) {
                    int columnIndex = 0;
                    QVariantList rowData = QVariantList();
                    QString text = QString();
                    foreach(const auto& info, detail.toList()) {
                            if (columnSelect == columnIndex) {
                                if (rowIndex == rowStart) {
                                    startIndex = (rowIndex - ivis::common::CellInfoEnum::ListInfoExcel::Data);
                                    text = QString("%1[=]%2[=]%3[=]%4").arg(str).arg(columnIndex)
                                                                        .arg(startIndex).arg(info.toString());
                                } else if (rowIndex == (rowEnd - 1)) {
                                    int endIndex = (rowIndex - ivis::common::CellInfoEnum::ListInfoExcel::Data - startIndex + 1);
                                    text = QString("%1[=]%2[=]%3").arg(str).arg(columnIndex).arg(endIndex);
                                } else {
                                    text = QString("%1[=]").arg(str);
                                }
                            } else {
                                text = info.toString();
                            }
                        rowData.append(text);
                        columnIndex++;
                    }
                    updateData.append(QVariant(rowData));
                    qDebug() << "Text["  << rowIndex << "] :" << rowData;
                } else {
                    updateData.append(detail);
                }
            } else {
                if ((rowIndex > rowStart) && (rowIndex < rowEnd)) {
                    int columnIndex = 0;
                    QVariantList rowData = QVariantList();
                    foreach(const auto& info, detail.toList()) {
                        rowData.append((columnSelect == columnIndex) ? (str) : (info));
                        columnIndex++;
                    }
                    updateData.append(QVariant(rowData));
                } else {
                    updateData.append(detail);
                }
            }
        }
        rowIndex++;
    }
#endif

    return updateData;
}

void ControlExcel::slotConfigChanged(const int& type, const QVariant& value) {
    // qDebug() << "ControlMenu::slotConfigChanged(" << type << "," << value << ")";
    switch (type) {
        case ConfigInfo::ConfigTypeExcelBlankText : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelBlankText, value);
            break;
        }
        // case ConfigInfo::ConfigTypeScreenInfo : {
        //     QRect screenInfo = value.toRect();
        //     QSize screenSize = QSize(screenInfo.width(), screenInfo.height());
        //     updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
        //     break;
        // }
        default : {
            break;
        }
    }
}

void ControlExcel::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlExcel::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeOpenExcel : {
            if (checkPythonLibrary()) {
                QVariant filePath = value;
                QString dirPath = sytemCall(ivis::common::EventTypeEnum::EventTypeOpenExcel, filePath);
                if (dirPath.size() > 0) {
#if defined(USE_EXCEL_FUNCTION_NEW)
                    updateExcelSheet(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, dirPath);
#else
                    updateSheetInfo(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, dirPath);
#endif
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastFileInfo, filePath);
                } else {
                    QVariant popupData = QVariant();
                    ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenFail, isHandler(), popupData,
                                                    QVariantList({STRING_FILE_OPEN, STRING_FILE_OPEN_FAIL}));
                }
                checkTimer.check("Open Excel");
            }
            break;
        }
#if defined(USE_EXCEL_FUNCTION_NEW)
        case ivis::common::EventTypeEnum::EventTypeEditExcelSheet : {
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeEditExcelSheet, true);
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
            checkTimer.check("Edit Excel Sheet");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeReadExcelInfo : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheet, QVariant(), true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeReadExcelSheetComplete : {
            writeExcelFile(getData(ivis::common::PropertyTypeEnum::PropertyTypeSaveFilePath));
            break;
        }
#else
        case ivis::common::EventTypeEnum::EventTypeSaveExcel : {
            writeExcelFile(value);
            checkTimer.check("Save Excel");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateSheetTextInfo : {
            if (updateSheetTextInfo(value)) {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu,
                                ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
            }
            checkTimer.check("Update Sheet Text Info");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo : {
            if (updateSheetCellInfo(value)) {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu,
                                ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
            }
            checkTimer.check("Update Sheet Cell Info");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateSheetInfo : {
            checkTimer.check("Update Sheet Info");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                    QVariantList({STRING_POPUP_CELL_COLUMN, STRING_POPUP_CELL_COLUMN_TIP}));
            break;
        }
#endif
        default : {
            if ((type > ivis::common::EventTypeEnum::EventTypeList) && (type < ivis::common::EventTypeEnum::EventTypeListMax)) {
                QVariantList sheetData = value.toList();
                int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription +
                                                                (type - ivis::common::EventTypeEnum::EventTypeListDescription);
                updateDataHandler(propertyType, sheetData);
                // qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
                // qDebug() << "EvetnType :" << type << ", Length :" << sheetData.size();
                // qDebug() << "SheetData :" << sheetData;
                // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
            }
            break;
        }
    }
}

void ControlExcel::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlExcel::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeViewInfoClose : {
            // updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRaise, true, true);
            ControlManager::instance().data()->raise(displayType);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile : {
            QString lastFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastFileInfo).toString();
            if (lastFilePath.size() == 0) {
                QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
                QVariant popupData = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), popupData,
                                        QVariantList({STRING_FILE_OPEN, defaultPath})) == ivis::common::PopupButton::OK) {
                    slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeOpenExcel, popupData);
                }
            } else {
                slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeOpenExcel, lastFilePath);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileNew : {
#if defined(USE_EXCEL_FUNCTION_NEW)
            updateExcelSheet(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, QVariant());
#else
            updateSheetInfo(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, QVariant());
#endif
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeEditExcelSheet, true);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastFileInfo, QVariant());
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeMenu, ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileOpen : {
            QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
            QVariant popupData = QVariant();
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Open, isHandler(), popupData,
                                    QVariantList({STRING_FILE_OPEN, defaultPath})) == ivis::common::PopupButton::OK) {
                slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeOpenExcel, popupData);
            }
            break;
        }
#if defined(USE_EXCEL_FUNCTION_NEW)
        case ivis::common::EventTypeEnum::EventTypeFileSave :
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs : {
            if (getData(ivis::common::PropertyTypeEnum::PropertyTypeEditExcelSheet).toBool() == false) {
                qDebug() << "Fail to file save, Nothing to save content.";
                return;
            }

            QString filePath = (eventType == ivis::common::EventTypeEnum::EventTypeFileSaveAs) ? (QString())
                                : (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastFileInfo).toString());
            if (filePath.size() == 0) {
                QVariant popupData = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Save, isHandler(), popupData)
                                                                                            == ivis::common::PopupButton::OK) {
                    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeSaveFilePath, popupData);
                }
            } else {
                updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeSaveFilePath, filePath);
            }
            slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeReadExcelInfo, QVariant());
            break;
        }
#else
        case ivis::common::EventTypeEnum::EventTypeFileSave :
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs : {
            if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew).toInt() == 0)
                && (getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen).toInt() == 0)) {
                qDebug() << "Fail to file save, Nothing to save content.";
                return;
            }

            QString filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastFileInfo).toString();
            if (eventType == ivis::common::EventTypeEnum::EventTypeFileSaveAs) {
                filePath.clear();
            }

            if (filePath.size() == 0) {
                QVariant popupData = QVariant();
                if (ivis::common::Popup::drawPopup(ivis::common::PopupType::Save, isHandler(), popupData)
                    == ivis::common::PopupButton::OK) {
                    slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeSaveExcel, popupData);
                }
            } else {
                slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeSaveExcel, filePath);
            }
            break;
        }
#endif
        case ivis::common::EventTypeEnum::EventTypeReadExcelInfo : {
            slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeReadExcelInfo, QVariant());
            break;
        }
        default : {
            break;
        }
    }
}
