#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"

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

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);

    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QVariantList contentTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle).toList();

    updateDataHandler(PropertyTypeEnum::PropertyTypeSheetName, sheetName);
    updateDataHandler(PropertyTypeEnum::PropertyTypeContentTitle, contentTitle);
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
        emit isHandler()->signalUpdateDataModel(type, value, alwaysUpdate);
    }
}

void ControlCenter::updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        emit isHandler()->signalUpdateDataModel(type, getData(type), alwaysUpdate);
    }
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

    switch (type) {
        case EventTypeEnum::EventTypeUpdateSheetInfo : {
            if (value.toList().size() < 4) {
                return;
            }

            int sheetIndex = value.toList()[0].toInt();
            int row = value.toList()[1].toInt() + ListInfoEnum::ListInfoExcel::Data;
            int column = value.toList()[2].toInt();
            QString text = value.toList()[3].toString();

            qDebug() << "Sheet[" << sheetIndex << "] :" << row << "," << column << "," << text;

            QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
            QVariantList detailInfo = getData(sheetIndex).toList();
            qDebug() << "\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
            qDebug() << "detailInfo :" << detailInfo;
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

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
            qDebug() << "updateData :" << updateData;
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

    CheckTimer checkTimer;

    switch (eventType) {
        case EventTypeEnum::EventTypeCenterVisible : {
            bool visible = (getData(PropertyTypeEnum::PropertyTypeVisible).toBool()) ? (false) : (true);
            updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, visible);
            checkTimer.check("Center - Visible");
            break;
        }
        case EventTypeEnum::EventTypeOpenExcel :
        case EventTypeEnum::EventTypeSaveExcel : {
#if 1
            QStringList fileInfo = eventValue.toString().split("/");
            QString path = QString();
            for (int index = 0; index < (fileInfo.size() - 1); index++) {
                path.append(fileInfo[index]);
                path.append("/");
            }
            QString file = fileInfo[fileInfo.size() - 1];
            QString cmdType = ((eventType == EventTypeEnum::EventTypeOpenExcel) ? ("read") : ("write"));
            QString cmd = QString("python ../Example/excel_parsing.py %1 %2 %3").arg(path).arg(file).arg(cmdType);
            int result = system(cmd.toLatin1());

            qDebug() << "\n\n";
            qDebug() << "===========================================================";
            qDebug() << "FilePath :" << eventValue;
            qDebug() << "Commnad :" << cmd;
            qDebug() << "System :" << ((result == 0) ? ("sucess[") : ("fail[")) << result << "]";

            if ((eventType == EventTypeEnum::EventTypeOpenExcel) && (result == 0)) {
                ControlManager::instance().data()->sendEventInfo(getData(PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                                    ScreenEnum::DisplayTypeCenter,
                                                                    EventTypeEnum::EventTypeFileOpen,
                                                                    path);
            }
#else
            QProcess* process = new QProcess(isHandler());
            // QProcess* process = new QProcess(this);
            // connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::handleOutput);
            // connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            //     this, &MainWindow::handleFinished);
            connect(process, &QProcess::readyReadStandardOutput, [=]() {
                QByteArray data = process->readAllStandardOutput();
                qDebug() << "Data :" << data;
            });
            // process->start("ping", QStringList() << "-c" << "4" << "google.com");
            process->start("python", QStringList("../Example/excel_parsing.py"));
#endif
            if (eventType == EventTypeEnum::EventTypeOpenExcel) {
                checkTimer.check("Read Excel");
            } else {
                checkTimer.check("Write Excel");
            }
            break;
        }
        case EventTypeEnum::EventTypeFileNew :
        case EventTypeEnum::EventTypeFileOpen : {
            QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
            QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
            int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription;
            int updateSheetInfo = PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;

            if (eventType == EventTypeEnum::EventTypeFileNew) {
                updateSheetInfo = PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew;
                int rowCount = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();

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
            } else {
                updateSheetInfo = PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen;
                foreach(const auto& sheet, sheetName) {
                    int fileIndex = sheetIndex - PropertyTypeEnum::PropertyTypeDetailInfoDescription;
                    QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
                    QVariant title = QVariant();
                    QString filePath = QString("%1TC/%2_%3.txt").arg(eventValue.toString()).arg(fileIndex).arg(sheet.toString());
                    QStringList readData = FileInfo::readFile(filePath);
                    int rowCount = readData.size();

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
            }
            if (sheetDetailInfo.size() > 0) {
                    QMapIterator<int, QVariantList> iter(sheetDetailInfo);
                    while (iter.hasNext()) {
                        iter.next();
                        // qDebug() << "DetailInfo :" << iter.key() << "," << iter.value() << "\n\n";
                        updateDataHandler(iter.key(), iter.value());
                    }

                    updateDataHandler(updateSheetInfo, sheetDetailInfo.size(), true);
                    updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, true);
            }

            if (eventType == EventTypeEnum::EventTypeFileNew) {
                checkTimer.check("Update Excel - New");
            } else {
                checkTimer.check("Update Excel - Open");
            }
            break;
        }
        case EventTypeEnum::EventTypeFileSave : {
            break;
        }
        case EventTypeEnum::EventTypeFileSaveAs : {
            QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
            QMap<int, QVariantList> sheetDetailInfo = QMap<int, QVariantList>();
            int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription;
            bool sendEvent = true;

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
                    QStringList fileInfo = eventValue.toString().split("/");
                    QString path = QString();
                    for (int index = 0; index < (fileInfo.size() - 1); index++) {
                        path.append(fileInfo[index]);
                        path.append("/");
                    }
                    path.append("TC/");

                    int writeSize = FileInfo::writeFile(QString("%1%2").arg(path).arg(file), writeData);
                    if (writeSize == 0) {
                        sendEvent = false;
                        qDebug() << "Fail to write size 0, filePath :" << path << file;
                    }
                }
                qDebug() << "\n\n";
            }

            if (sendEvent) {
                ControlManager::instance().data()->sendEventInfo(getData(PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                                    ScreenEnum::DisplayTypeCenter,
                                                                    EventTypeEnum::EventTypeSaveExcel,
                                                                    eventValue);
            }
            break;
        }
        default : {
            break;
        }
    }
}
