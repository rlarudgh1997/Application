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

void ControlCenter::updateDataHandler(const int& type, const QVariant& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, value);
    }
}

void ControlCenter::updateDataHandler(const int& type, const QVariantList& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, getData(type));
    }
}

void ControlCenter::slotConfigChanged(const int& type, const QVariant& value) {
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
        case EventTypeEnum::EventTypeFileNew :
        case EventTypeEnum::EventTypeFileOpen : {
            QStringList excelData = QStringList();
            QVariantList sheetName = QVariantList();
            QVariantList contentTitle = QVariantList();

            if (eventType == EventTypeEnum::EventTypeFileNew) {
                sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
                contentTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle).toList();
                int rowCount = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNewSheetRowCount).toInt();
                foreach(const auto& name, sheetName) {
                    qDebug() << "sheetName:" << name;
                    excelData.append(QString("SheetName:%1").arg(name.toString()));
                    for (int index = 0; index < rowCount; index++) {
                        excelData.append("");
                    }
                }
            } else {
                QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
                qDebug() << "FilePath :" << eventValue;
                QStringList readData = FileInfo::parsingFile(eventValue.toString());
                if (readData.size() > 0) {
                    sheetName = {"sheet1"};
                    QStringList titleInfo = readData[0].split("\t");
                    foreach(const auto& title, titleInfo) {
                        contentTitle.append(title);
                    }
                    qDebug() << "Read :" << readData[0] << ", " << contentTitle;

                    for (int index = 1; index < readData.size(); index++) {
                        excelData.append(readData[index]);
                    }
                }
            }
#if 0
             QVariantList temp, temp2;
             temp.append(contentTitle);
             temp.append("\n");

             int index = 0;
             for (QVariant name : contentTitle) {
                 temp2.append(QString("item %1").arg(index));
                 index++;
             }
             temp.append(temp2);
             qDebug() << "Sheet1 :" << temp;

             QString value;
             foreach(const QVariant& t, temp) {
                 if (t.toString().compare("\n") == 0) {
                     qDebug() << "NewLine :" << t;
                 } else {
                     value.append(t.toString());
                     value.append("/");

                     qDebug() << "Item :" << t;
                 }
             }
#endif
            if (excelData.size() > 0) {
                QVariantList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toList();
                updateDataHandler(PropertyTypeEnum::PropertyTypeSheetName, sheetName);
                updateDataHandler(PropertyTypeEnum::PropertyTypeContentTitle, contentTitle);
                updateDataHandler(PropertyTypeEnum::PropertyTypeDescTitle, descTitle);
                updateDataHandler(PropertyTypeEnum::PropertyTypeUpdateSheetInfo, excelData);
                updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, true);

                if (eventType == EventTypeEnum::EventTypeFileOpen) {
                    checkTimer.check("Excel - Open");
                } else {
                    checkTimer.check("Excel - New");
                }
            }
            break;
        }
        case EventTypeEnum::EventTypeParsingExcel : {
#if 1
            int result = system("python ../Example/excel_parsing.py");
            qDebug() << "system call" << ((result == 0) ? ("sucess :") : ("fail :")) << result;
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
            checkTimer.check("Excel - Parsing");
            break;
        }
        default : {
            break;
        }
    }
}

void ControlCenter::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case EventTypeEnum::EventTypeTest1 : {
            break;
        }
        default : {
            break;
        }
    }
}
