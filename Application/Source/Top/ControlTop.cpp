#include "ControlTop.h"
#include "HandlerTop.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QMessageBox>
#include <QFileDialog>


QSharedPointer<ControlTop> ControlTop::instance() {
    static QSharedPointer<ControlTop> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlTop>(new ControlTop());
    }
    return gControl;
}

ControlTop::ControlTop() {
    isHandler();
}

AbstractHandler* ControlTop::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerTop::instance().data());
    }
    return mHandler;
}

void ControlTop::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();

        controlConnect(true);
        initBaseData();
        initCommonData(currentMode, ScreenEnum::DisplayTypeTop);
    }
}

void ControlTop::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDepth, ScreenEnum::DisplayDepthDepth0);
}

void ControlTop::initBaseData() {
    resetControl(false);

    QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);
}

void ControlTop::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlTop::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerTop::signalHandlerEvent,
                this,        &ControlTop::slotHandlerEvent,
                Qt::UniqueConnection);
        // connect(ControlManager::instance().data(), &ControlManager::signalDisplayChange, [=](const int& displayType) {
        //     updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
        // });
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
    }
}

void ControlTop::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlTop::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlTop::updateDataHandler(const int& type, const QVariant& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, value);
    }
}

void ControlTop::updateDataHandler(const int& type, const QVariantList& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, getData(type));
    }
}

void ControlTop::slotConfigChanged(const int& type, const QVariant& value) {
}

void ControlTop::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case EventTypeEnum::PropertyTypeExitProgram : {
            ControlManager::instance().data()->exitProgram();
            break;
        }
        case EventTypeEnum::PropertyTypeDisplayChange : {
            ControlManager::instance().data()->changeDisplay(ScreenEnum::DisplayTypeCenter);
            break;
        }
        case EventTypeEnum::PropertyTypeFileNew : {
            qDebug() << "File - New";
            break;
        }
        case EventTypeEnum::PropertyTypeFileOpen : {
            qDebug() << "File - Open";
            break;
        }
        case EventTypeEnum::PropertyTypeFileSave : {
            qDebug() << "File - Save";
            break;
        }
        case EventTypeEnum::PropertyTypeFileSaveAs : {
            qDebug() << "File - Save As";
            break;
        }
        case EventTypeEnum::PropertyTypeEditCut : {
            qDebug() << "Edit - Cut";
            break;
        }
        case EventTypeEnum::PropertyTypeEditCopy : {
            qDebug() << "Edit - Copy";
            break;
        }
        case EventTypeEnum::PropertyTypeEditPaste : {
            qDebug() << "Edit - Paste";
            break;
        }
        case EventTypeEnum::PropertyTypeSettingDevPath : {
            QString defaultPath = QFileDialog::getExistingDirectory(qobject_cast<QWidget*>(isHandler()),
                                            tr("Default Path"),
                                            getData(PropertyTypeEnum::PropertyTypeDefaultPath).toString(),
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (defaultPath.size() == 0) {
                defaultPath = QApplication::applicationDirPath();
            }
            updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultPath, defaultPath);
            break;
        }
        case EventTypeEnum::PropertyTypeSettingTestReport : {
            qDebug() << "Setting - Test Report";
            break;
        }
        case EventTypeEnum::PropertyTypeSettingTestResult : {
            qDebug() << "Setting - Test Result";
            break;
        }
        case EventTypeEnum::PropertyTypeSettingTestCoverage : {
            qDebug() << "Setting - Test Coverage";
            break;
        }
        case EventTypeEnum::PropertyTypeHelpAbout : {
            QMessageBox::about(qobject_cast<QWidget*>(isHandler()),
                            tr("About Application"),
                            tr("This <b>Application</b> is a program for <b>SFC Test</b> (Excel conversion, parsing, etc.)"));
            break;
        }
        case EventTypeEnum::PropertyTypeHelpAboutQt : {
            QApplication::aboutQt();
            break;
        }
        default : {
            break;
        }
    }
}



