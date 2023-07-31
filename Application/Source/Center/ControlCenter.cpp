#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"
#include "CommonPopup.h"

#include <QProcess>


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

void ControlCenter::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        controlConnect(true);
        initBaseData();
        initCommonData(currentMode, ScreenEnum::DisplayTypeCenter);
    }
}

void ControlCenter::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, false);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDepth, ScreenEnum::DisplayDepthDepth0);
}

void ControlCenter::initBaseData() {
    resetControl(false);

    updateDataHandler(PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, 0);
    updateDataHandler(PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, 0);
}

void ControlCenter::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlCenter::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerCenter::signalHandlerEvent,
                this,                              &ControlCenter::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlCenter::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlCenter::slotEventInfoChanged,
                Qt::UniqueConnection);
#if defined(USE_RESIZE_SIGNAL)
        connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
                updateDataHandler(PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
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
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlCenter::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlCenter::updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlCenter::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlCenter::updateSheetInfo(const int& propertyType, const QVariant& dirPath) {
    qDebug() << "ControlCenter::updateSheetInfo() ->" << propertyType << "," << dirPath;
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
    int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int initPropertyType = PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
    int rowCount = 0;

    switch (propertyType) {
        case PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew : {
            initPropertyType = PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
            rowCount = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
            foreach(const auto& sheet, sheetName) {
                QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
                QVariant title = QVariant();

                dataInfo[ListInfoEnum::ListInfoExcel::Sheet] = sheet;
                if (sheetIndex == PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
                } else {
                    title = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle);
                }
                dataInfo[ListInfoEnum::ListInfoExcel::Count] = QVariant(QVariantList({rowCount, title.toList().count()}));
                dataInfo[ListInfoEnum::ListInfoExcel::Title] = title;

                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    int columnCount = dataInfo[ListInfoEnum::ListInfoExcel::Title].toList().count();
                    QVariantList columnInfo = QVariantList();
                    for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                        // columnInfo.append("");
                        columnInfo.append(QString("%1[%2_%3]").arg(sheet.toString()).arg(rowIndex).arg(columnIndex));
                    }
                    dataInfo[ListInfoEnum::ListInfoExcel::Data + rowIndex] = columnInfo;
                }
                sheetDetailInfo[sheetIndex] = dataInfo.values();
                sheetIndex++;
            }
            break;
        }
        case PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            initPropertyType = PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
            foreach(const auto& sheet, sheetName) {
                int fileIndex = sheetIndex - PropertyTypeEnum::PropertyTypeDetailInfoDescription;
                QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
                QVariant title = QVariant();
                QString filePath = QString("%1/%2_%3.txt").arg(dirPath.toString()).arg(fileIndex).arg(sheet.toString());
                QStringList readData = FileInfo::readFile(filePath);
                rowCount = readData.size();

                if (rowCount == 0) {
                    qDebug() << "Fail to read file length :" << rowCount << ", path :" << filePath;
                    continue;
                }

                dataInfo[ListInfoEnum::ListInfoExcel::Sheet] = sheet;
                for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
                    QStringList columnInfo = readData[rowIndex].split("\t");
                    if (rowIndex == 0) {
                        dataInfo[ListInfoEnum::ListInfoExcel::Count] =
                                                            QVariant(QVariantList({rowCount - 1, columnInfo.count()}));
                        dataInfo[ListInfoEnum::ListInfoExcel::Title] = columnInfo;
                    } else {
                        dataInfo[ListInfoEnum::ListInfoExcel::Data + rowIndex - 1] = columnInfo;
                    }
                }
                sheetDetailInfo[sheetIndex] = dataInfo.values();
                sheetIndex++;
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
        updateDataHandler(initPropertyType, 0);
        updateDataHandler(propertyType, sheetDetailInfo.size(), true);
        updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, true);
    }
}

bool ControlCenter::editSheetInfo(const QVariant& value) {
    if (value.toList().size() < 4) {
        return false;
    }

    int sheetIndex = value.toList()[0].toInt();
    int row = value.toList()[1].toInt() + ListInfoEnum::ListInfoExcel::Data;
    int column = value.toList()[2].toInt();
    QString text = value.toList()[3].toString();

    // qDebug() << "Sheet[" << sheetIndex << "] :" << row << "," << column << "," << text;

    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QVariantList detailInfo = getData(sheetIndex).toList();
    // qDebug() << "\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "detailInfo :" << detailInfo;
    // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    int rowIndex = 0;
    QVariantList updateData = QVariantList();
    foreach(const auto& detail, detailInfo) {
        int columnIndex = 0;
        if (rowIndex < ListInfoEnum::ListInfoExcel::Data) {
            updateData.append(detail);
        } else {
            QVariantList rowData = QVariantList();
            foreach(QVariant info, detail.toList()) {
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
        // qDebug() << "detail :" << detail;
    }
    updateDataHandler(sheetIndex, updateData);
    // qDebug() << "updateData :" << updateData;
    return true;
}

bool ControlCenter::writeSheetInfo(const QVariant& filePath) {
    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
    int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int writeSize = 0;

    foreach(const auto& sheet, sheetName) {
        sheetDetailInfo[sheetIndex++] = getData(sheetIndex).toList();
    }

    QString file = QString();
    sheetIndex = 0;
    foreach(const auto& detailInfo, sheetDetailInfo) {
        int index = 0;
        QString writeData = QString();
        foreach(const auto& detail, detailInfo) {
            if (index == ListInfoEnum::ListInfoExcel::Sheet) {
                file = QString("%1_%2.ini").arg(sheetIndex++).arg(detail.toString());
            } else if (index >= ListInfoEnum::ListInfoExcel::Title) {
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
            QStringList fileInfo = filePath.toString().split("/");
            QString path = QString();
            for (int index = 0; index < (fileInfo.size() - 1); index++) {
                path.append(fileInfo[index]);
                path.append("/");
            }
            path.append("TC/");

            writeSize = FileInfo::writeFile(QString("%1%2").arg(path).arg(file), writeData);
            if (writeSize == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << path << file;
            }
        }
    }
    return (writeSize > 0);
}

QString ControlCenter::sytemCall(const int& type, const QVariant& filePath) {
    qDebug() << "ControlCenter::sytemCall() ->" << type << "," << filePath;
    QString cmdType = ((type == EventTypeEnum::EventTypeOpenExcel) ? ("read") : ("write"));
    QStringList fileInfo = filePath.toString().split("/");
    QString fileName = fileInfo[fileInfo.size() - 1];
    QString dirPath = QString();

    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        dirPath.append(fileInfo[index]);
        dirPath.append("/");
    }

    int result = 0;
    QString cmd = QString("python ../Example/excel_parsing.py %1 %2 %3").arg(dirPath).arg(fileName).arg(cmdType);
#if defined(USE_EXTERNAL_PROGRAM_START_PROCESS)
    static QProcess* process = new QProcess(isHandler());
    // process->start("ping", QStringList() << "-c" << "4" << "google.com");
    // process->start("python", QStringList("../Example/excel_parsing.py"));
    process->start(cmd);
    connect(process, &QProcess::readyReadStandardOutput, [=]() {
        QString readAllData = QString(process->readAllStandardOutput());
        QString logData = QString();
        foreach(const QString& data, readAllData) {
            if (data.compare("\n") == false) {
                logData.clear();
             } else {
                logData.append(data);
             }
        }
    });
    // connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
    //     this, &ControlCenter::handleFinished);
#else
    result = system(cmd.toLatin1());
#endif

    dirPath.append("TC");
    if (result != 0) {
        dirPath.clear();
    }

    qDebug() << "*************************************************************************************************";
    qDebug() << "System :" << ((result == 0) ? ("sucess[") : ("fail[")) << result << "] -" << cmd;
    qDebug() << "FilePath :" << filePath;
    qDebug() << "DirPath  :" << dirPath;
    qDebug() << "*************************************************************************************************\n";

    return dirPath;
}

void ControlCenter::slotConfigChanged(const int& type, const QVariant& value) {
    Q_UNUSED(type)
    Q_UNUSED(value)
    // qDebug() << "ControlCenter::slotConfigChanged() ->" << type << "," << value;
    // switch (type) {
    //     case ConfigInfo::ConfigTypeDefaultPath : {
    //         break;
    //     }
    //     default : {
    //         break;
    //     }
    // }
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlCenter::slotHandlerEvent() ->" << type << "," << value;
    CheckTimer checkTimer;

    switch (type) {
        case EventTypeEnum::EventTypeOpenExcel : {
            QVariant filePath = value;
            QString dirPath = sytemCall(EventTypeEnum::EventTypeOpenExcel, filePath);
            if (dirPath.size() > 0) {
                updateSheetInfo(PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen, dirPath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeLastFileInfo, filePath);
            } else {
                Popup::drawPopup(PopupType::OpenFail, isHandler(), 0, QVariantList({STRING_FILE_OPEN, STRING_FILE_OPEN_FAIL}));
            }
            checkTimer.check("Open Excel");
            break;
        }
        case EventTypeEnum::EventTypeSaveExcel : {
            QVariant filePath = value;
            if (writeSheetInfo(filePath)) {
                sytemCall(EventTypeEnum::EventTypeSaveExcel, filePath);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeLastFileInfo, filePath);
            }
            checkTimer.check("Save Excel");
            break;
        }
        case EventTypeEnum::EventTypeUpdateSheetInfo : {
            if (editSheetInfo(value)) {
                int fileSaveType = (getData(PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen).toInt() > 0) ?
                                    (EventTypeEnum::EventTypeFileSave) : (EventTypeEnum::EventTypeFileSaveAs);
                sendEventInfo(ScreenEnum::DisplayTypeTop, EventTypeEnum::EventTypeFileSaveType, fileSaveType);
            }
            checkTimer.check("Update Sheet");
            break;
        }
        default : {
            break;
        }
    }
}

void ControlCenter::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if (getData(PropertyTypeEnum::PropertyTypeDisplay) != QVariant(displayType)) {
        return;
    }

    qDebug() << "ControlCenter::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case EventTypeEnum::EventTypeLastFile : {
            QString lastFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeLastFileInfo).toString();
            slotHandlerEvent(EventTypeEnum::EventTypeOpenExcel, lastFilePath);
            break;
        }
        case EventTypeEnum::EventTypeFileNew : {
            updateSheetInfo(PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew, QVariant());
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeLastFileInfo, QVariant());
            sendEventInfo(ScreenEnum::DisplayTypeTop, EventTypeEnum::EventTypeFileSaveType, true);
            break;
        }
        case EventTypeEnum::EventTypeFileOpen : {
            QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
            Popup::drawPopup(PopupType::Open, isHandler(), EventTypeEnum::EventTypeOpenExcel,
                                                            QVariantList({STRING_FILE_OPEN, defaultPath}));
            break;
        }
        case EventTypeEnum::EventTypeFileSave :
        case EventTypeEnum::EventTypeFileSaveAs : {
            if ((getData(PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew).toInt() == 0)
                && (getData(PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen).toInt() == 0)) {
                qDebug() << "Fail to file save, Nothing to save content.";
                return;
            }

            QString filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeLastFileInfo).toString();
            if (eventType == EventTypeEnum::EventTypeFileSaveAs) {
                filePath.clear();
            }

            if (filePath.size() == 0) {
                Popup::drawPopup(PopupType::Save, isHandler(), EventTypeEnum::EventTypeSaveExcel);
            } else {
                slotHandlerEvent(EventTypeEnum::EventTypeSaveExcel, filePath);
            }
            break;
        }
        default : {
            break;
        }
    }
}
