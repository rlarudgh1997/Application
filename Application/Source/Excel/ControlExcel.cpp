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

    QVariant nodeAddressPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath);
    QStringList sfcList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressSFC.info");
    QStringList vsmList = ivis::common::FileInfo::readFile(nodeAddressPath.toString() + "/NodeAddressVSM.info");
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll, (sfcList + vsmList), true);
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
    static bool shortcutCtrl = false;

    if (inputType == ivis::common::KeyTypeEnum::KeyInputTypePress) {
        // if (inputValue == Qt::Key_Control) {
        //     shortcutCtrl = true;
        // }
    } else if (inputType == ivis::common::KeyTypeEnum::KeyInputTypeRelease) {
        switch (inputValue) {
#if defined(PLATFORM_X86)
            case ivis::common::KeyTypeEnum::KeyInputValueNumOK :
            case ivis::common::KeyTypeEnum::KeyInputValueNumOK2 :
#endif
            case ivis::common::KeyTypeEnum::KeyInputValueOK : {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 0, true);
                break;
            }
            case Qt::Key_Escape : {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 2, true);
                break;
            }
            case Qt::Key_F2 : {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete, 1, true);
                break;
            }
            case Qt::Key_I :
            case Qt::Key_D :
            case Qt::Key_M :
            case Qt::Key_S : {
                if (shortcutCtrl) {
                    qDebug() << "ShortcutKey :" << inputValue;
                    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeShortcutKey, inputValue, true);
                }
                break;
            }
            default : {
                break;
            }
        }
        shortcutCtrl = false;
    } else {
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

void ControlExcel::updateExcelSheet(const bool& excelOpen, const QVariant& dirPath) {
    qDebug() << "ControlExcel::updateExcelSheet() ->" << excelOpen << "," << dirPath;

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

void ControlExcel::writeExcelFile(const QVariant& filePath) {
    if (checkPythonLibrary() == false) {
        qDebug() << "Faitl to python lib not install.";
        return;
    }

    if (writeExcelSheet(filePath)) {
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

    QString cmd = QString("python %1/ExcelParser.py %2 %3 %4").arg(APP_PWD).arg(dirPath).arg(fileName).arg(cmdType);
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
#if defined(USE_PYTHON_LIB_CHECK_READ_WRITE)
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
#else
    return true;
#endif
}

void ControlExcel::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeExcelMergeTextStart :
        case ConfigInfo::ConfigTypeExcelMergeTextEnd :
        case ConfigInfo::ConfigTypeExcelMergeText : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText, value);
            break;
        }
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
                    updateExcelSheet(true, dirPath);
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
        case ivis::common::EventTypeEnum::EventTypeSaveFromReadExcelSheet : {
            writeExcelFile(getData(ivis::common::PropertyTypeEnum::PropertyTypeSaveFilePath));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                    QVariantList({STRING_POPUP_CELL_COLUMN, STRING_POPUP_CELL_COLUMN_TIP}));
            break;
        }
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
            updateExcelSheet(false, QVariant());
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
        case ivis::common::EventTypeEnum::EventTypeReadExcelInfo : {
            slotHandlerEvent(ivis::common::EventTypeEnum::EventTypeReadExcelInfo, QVariant());
            break;
        }
        default : {
            break;
        }
    }
}
