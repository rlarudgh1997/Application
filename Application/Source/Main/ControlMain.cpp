#include "ControlMain.h"
#include "HandlerMain.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>


QSharedPointer<ControlMain> ControlMain::instance() {
    static QSharedPointer<ControlMain> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlMain>(new ControlMain());
    }
    return gControl;
}

ControlMain::ControlMain() {
    isHandler();
}

AbstractHandler* ControlMain::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerMain::instance().data());
    }
    return mHandler;
}

void ControlMain::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();

        controlConnect(true);
        initDataModule();
        initDataCommon(currentMode, ScreenEnum::DisplayTypeMain);
    }
}

void ControlMain::initDataCommon(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDepth, ScreenEnum::DisplayDepthDepth0);
}

void ControlMain::initDataModule() {
    resetControl(false);

    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QStringList contextName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContextName).toStringList();
    QVariant defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();

    updateDataHandler(PropertyTypeEnum::PropertyTypeSheetName, sheetName);
    updateDataHandler(PropertyTypeEnum::PropertyTypeContextName, contextName);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);

    // qDebug() << "sheetName :" << getData(PropertyTypeEnum::PropertyTypeSheetName);
    // qDebug() << "sheetName :" << getData(PropertyTypeEnum::PropertyTypeContextName);
    // qDebug() << "DefaultPath :" << getData(PropertyTypeEnum::PropertyTypeDefaultPath);
}

void ControlMain::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMain::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerMain::signalHandlerEvent,
                this,        &ControlMain::slotHandlerEvent,
                Qt::UniqueConnection);
    } else {
        disconnect(isHandler());
    }
}

void ControlMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlMain::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlMain::updateDataHandler(const int& type, const QVariant& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, value);
    }
}

void ControlMain::updateDataHandler(const int& type, const QVariantList& value) {
    if (setData(type, value)) {
        emit isHandler()->signalUpdateDataModel(type, getData(type));
    }
}

void ControlMain::slotConfigChanged(const int& type, const QVariant& value) {
}

void ControlMain::slotHandlerEvent(const int& type, const QVariant& value) {
    switch (type) {
        case EventTypeEnum::PropertyTypeExitProgram : {
            // QApplication::closeAllWindows();
            ControlManager::instance().data()->exitProgram();
            break;
        }
        case EventTypeEnum::PropertyTypeChangeDepth : {
            QVariant changeDepth = (getData(PropertyTypeEnum::PropertyTypeDepth) == ScreenEnum::DisplayDepthDepth0) ?
                                                            (ScreenEnum::DisplayDepthDepth1) : (ScreenEnum::DisplayDepthDepth0);
            qDebug() << "Depth :"<< getData(PropertyTypeEnum::PropertyTypeDepth) << " ->" << changeDepth;
            updateDataHandler(PropertyTypeEnum::PropertyTypeDepth, changeDepth);
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

            qDebug() << "Default Path :" << defaultPath;

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




