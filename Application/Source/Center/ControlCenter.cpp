#include "ControlCenter.h"
#include "HandlerCenter.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"

#include <QProcess>


QSharedPointer<ControlCenter> ControlCenter::instance() {
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

    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QVariantList contextName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle).toList();

    updateDataHandler(PropertyTypeEnum::PropertyTypeSheetName, sheetName);
    updateDataHandler(PropertyTypeEnum::PropertyTypeContentTitle, contextName);
}

void ControlCenter::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlCenter::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerCenter::signalHandlerEvent,
                this,                              &ControlCenter::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlCenter::slotEventInfoChanged,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlCenter::slotConfigChanged,
                Qt::UniqueConnection);
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

    switch (eventType) {
        case EventTypeEnum::EventTypeCenterVisible : {
            updateDataHandler(PropertyTypeEnum::PropertyTypeVisible,
                                (getData(PropertyTypeEnum::PropertyTypeVisible).toBool()) ? (false) : (true));
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
            break;
        }
        case EventTypeEnum::EventTypeFileNew : {
            qDebug() << "File - New";
            QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
            QVariantList contextName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContentTitle).toList();

            QVariantList temp, temp2;
            temp.append(contextName);
            temp.append("\n");

            int index = 0;
            for (QVariant name : contextName) {
                temp2.append(QString("item %1").arg(index));
                index++;
            }
            temp.append(temp2);
//            qDebug() << "Sheet1 :" << temp;

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




            updateDataHandler(PropertyTypeEnum::PropertyTypeSheetName, sheetName);
            updateDataHandler(PropertyTypeEnum::PropertyTypeContentTitle, QVariant());
            updateDataHandler(PropertyTypeEnum::PropertyTypeContentItemSheet0, QVariant(temp));
            updateDataHandler(PropertyTypeEnum::PropertyTypeVisible, true);
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
