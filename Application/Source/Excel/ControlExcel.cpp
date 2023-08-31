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

void ControlExcel::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        controlConnect(true);
        initBaseData();
        initCommonData(currentMode, ivis::common::ScreenEnum::DisplayTypeExcel);
    }
}

void ControlExcel::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlExcel::initBaseData() {
    resetControl(false);

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, 0);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, 0);

    QString excelBlankText = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelBlankText).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelBlankText, excelBlankText);

    // updateSheetInfo(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, QVariant());
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
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlExcel::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlExcel::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlExcel::updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlExcel::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlExcel::updateSheetInfo(const int& propertyType, const QVariant& dirPath) {
    qDebug() << "ControlExcel::updateSheetInfo() ->" << propertyType << "," << dirPath;
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
    int rowCount = 0;

    switch (propertyType) {
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew : {
            initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
            rowCount = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
            foreach(const auto& sheet, sheetName) {
                QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
                QVariant title = QVariant();

                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet] = sheet;
                if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
                } else {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle);
                }
                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count] =
                                                    QVariant(QVariantList({rowCount, title.toList().count()}));
                dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title] = title;

                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    int columnCount = dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title].toList().count();
                    QVariantList columnInfo = QVariantList();
                    for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
#if defined(CELL_INFO_TEMP)
                        columnInfo.append(QString("%1[%2_%3]").arg(sheet.toString()).arg(rowIndex).arg(columnIndex));
#else
                        columnInfo.append("");
#endif
                    }
                    dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex] = columnInfo;
                }
                sheetDetailInfo[sheetIndex] = dataInfo.values();
                sheetIndex++;
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            initPropertyType = ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
            foreach(const auto& sheet, sheetName) {
                int fileIndex = sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
                QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
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
                sheetDetailInfo[sheetIndex] = dataInfo.values();
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

    if (sheetDetailInfo.size() > 0) {
        QMapIterator<int, QVariantList> iter(sheetDetailInfo);
        while (iter.hasNext()) {
            iter.next();
            updateDataHandler(iter.key(), iter.value());
        }
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet, false);
        updateDataHandler(initPropertyType, 0);
        updateDataHandler(propertyType, sheetDetailInfo.size(), true);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    }
}

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

bool ControlExcel::writeSheetInfo(const QVariant& filePath) {
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int writeSize = 0;

    foreach(const auto& sheet, sheetName) {
        sheetDetailInfo[sheetIndex++] = getData(sheetIndex).toList();
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
    foreach(const auto& detailInfo, sheetDetailInfo) {
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
            title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle);
        }

        QVariantList rowData = QVariantList();
        foreach(const auto& t, title.toList()) {
#if defined(CELL_INFO_TEMP)
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
#if defined(CELL_INFO_TEMP)
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
#if defined(CELL_INFO_TEMP)
    QString str = (editType == ivis::common::EditCellEnum::EditCellInfo::Merge) ? ("") : (excelBlankText);
#else
    QString str = (editType == ivis::common::EditCellEnum::EditCellInfo::Merge) ? ("ExcelMergeText") : (excelBlankText);
#endif
    int rowIndex = 0;
    int startIndex = 0;
    foreach(const auto& detail, detailInfo) {
        if (rowIndex < ivis::common::CellInfoEnum::ListInfoExcel::Data) {
            updateData.append(detail);
        } else {
#if defined(CELL_INFO_TEMP)
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
#else
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
#endif
        }
        rowIndex++;
    }

    return updateData;
}

void ControlExcel::slotConfigChanged(const int& type, const QVariant& value) {
    // qDebug() << "ControlTop::slotConfigChanged(" << type << "," << value << ")";
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
                    updateSheetInfo(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, dirPath);
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
        case ivis::common::EventTypeEnum::EventTypeSaveExcel : {
            if (checkPythonLibrary()) {
                QVariant filePath = value;
                if (writeSheetInfo(filePath)) {
                    if (sytemCall(ivis::common::EventTypeEnum::EventTypeSaveExcel, filePath) > 0) {
                        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastFileInfo, filePath);
                        sendEventInfo(ivis::common::ScreenEnum::DisplayTypeTop,
                                        ivis::common::EventTypeEnum::EventTypeFileSaveType, false);
                    }
                }
                checkTimer.check("Save Excel");
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateSheetTextInfo : {
            if (updateSheetTextInfo(value)) {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeTop, ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
            }
            checkTimer.check("Update Sheet Text Info");
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo : {
            if (updateSheetCellInfo(value)) {
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeTop, ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
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
        default : {
            break;
        }
    }
}

void ControlExcel::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug() << "ControlExcel::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
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
            updateSheetInfo(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, QVariant());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeLastFileInfo, QVariant());
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeTop, ivis::common::EventTypeEnum::EventTypeFileSaveType, true);
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
        case ivis::common::EventTypeEnum::EventTypeReadExcelInfo : {
            qDebug() << "Read Excel Info";
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeReadExcelInfo, QVariant(), true);
            break;
        }
        default : {
            break;
        }
    }
}
