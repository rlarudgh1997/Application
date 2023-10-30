#include "ControlMenu.h"
#include "HandlerMenu.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"

#include <QFileSystemWatcher>


// Q_LOGGING_CATEGORY(C_TOP, "ControlMenu")


// #define USE_TEST_RESULT_TEMP


QSharedPointer<ControlMenu>& ControlMenu::instance() {
    static QSharedPointer<ControlMenu> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlMenu>(new ControlMenu());
    }
    return gControl;
}

ControlMenu::ControlMenu() {
    isHandler();
}

AbstractHandler* ControlMenu::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerMenu::instance().data());
    }
    return mHandler;
}

bool ControlMenu::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlMenu::initCommonData(const int& currentMode) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, ivis::common::ScreenEnum::DisplayTypeMenu);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeMode, currentMode);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVisible, true);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDepth, ivis::common::ScreenEnum::DisplayDepthDepth0);
}

void ControlMenu::initNormalData() {
    resetControl(false);

    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, defaultPath);

    updateAllModueList(QString());


#if defined(USE_TEST_RESULT_TEMP)
    controlTimer(AbstractControl::AbstractTimerStart, true, 1000);

    QVariantList countInfo = QVariantList({-1, 10, true});
    QVariant errorInfo = QVariant("Test Case : Start \nERROR_INFO : Test Result Info");
    QVariantList moduleStateInfo = QVariantList({});
    QVariantList testResultInfo = QVariantList({countInfo, errorInfo, moduleStateInfo});

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo));

    testResultInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
#endif
}

void ControlMenu::initControlData() {
}

void ControlMenu::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMenu::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerMenu::signalHandlerEvent,
                this,                              &ControlMenu::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlMenu::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlMenu::slotEventInfoChanged,
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

void ControlMenu::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)

#if defined(USE_TEST_RESULT_TEMP)
    static QVariantList tempState = QVariantList({
        "ABS_CV : PASS",
        "ABS_NO_ABS_Trailer : PASS",
        "ADAS_Driving_CV : PASS",
        "ADAS_PARKING_CV : PASS",
        "Air_Bag_CV : PASS",
        "Brake_Air : PASS",
        "Brake_System_Malfunction : PASS",
        "CNG_Fuel_System_CV : PASS",
        "DEA : PASS",
        "DSW : PASS",
    });

    if (getTimerId(AbstractControl::AbstractTimerStart) == timerId) {
        QVariantList testResultInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
        if (testResultInfo.size() != 3) {
            qDebug() << "Fail to test result info size :" << testResultInfo.size();
            return;
        }

        QVariantList countInfo = testResultInfo.at(0).toList();
        if (countInfo.size() != 3) {
            qDebug() << "Fail to count info size :" << countInfo.size() << countInfo;
            return;
        }
        int current = countInfo.at(0).toInt() + 1;
        int total = countInfo.at(1).toInt();
        bool complete = (current > total);
        countInfo.clear();
        countInfo = QVariantList({current, total, complete});

        QVariant errorInfo = testResultInfo.at(1);

        QVariantList moduleStateInfo = testResultInfo.at(2).toList();
        if (current < tempState.size()) {
            moduleStateInfo.append(tempState.at(current));
        }

        testResultInfo = QVariantList({countInfo, errorInfo, moduleStateInfo});

        // qDebug() << "\t 2 TestResultInfo :" << testResultInfo;
        // qDebug() << "\t   2-0 :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
        // qDebug() << "\t   2-1 :" << testResultInfo.at(1).toString().size() << testResultInfo.at(1);
        // qDebug() << "\t   2-2 :" << testResultInfo.at(2).toList().size() << testResultInfo.at(2);

        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo));

        if (complete) {
            controlTimer(AbstractControl::AbstractTimerStart);
        }
    }
#endif
}

void ControlMenu::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlMenu::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QSize(width, height));
#endif
}

void ControlMenu::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlMenu::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlMenu::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlMenu::updateAllModueList(const QString& filter) {
    QString defaultPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    QString path = defaultPath;
    QStringList sfcModules = QStringList();
    if (path.contains("/model/SFC/CV")) {
        QDir directory(path);
        QStringList sfcDirectory = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if (filter.size() == 0) {
            sfcModules = sfcDirectory;
        } else {
            foreach(const auto& sfc, sfcDirectory) {
                QDir subDirectory(QString("%1/%2").arg(defaultPath).arg(sfc));
                // subDirectory.setNameFilters(QStringList({".tc", ".xlsx"}));
                bool fileFound = false;
                foreach(const auto& file, subDirectory.entryList(QDir::Files)) {
                    if ((fileFound = file.contains(filter)) == true) {
                        // qDebug() << "  FileFound :" << sfc << file;
                        sfcModules.append(sfc);
                        break;
                    }
                }
            }
        }
    } else {
        path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath).toString();
        sfcModules = ivis::common::FileInfo::readFile(path + "/DefaultModule.info");
    }
    // qDebug() << "\t SFC modules load path :" << sfcModules.size() << path;
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, sfcModules);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(sfcModules));
}

void ControlMenu::updateSelectModueList(const int& type, const QVariantList& selectModule) {
    if ((type == 0) && (selectModule.size() > 0)) {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(selectModule));
        return;
    }

    int eventType = ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfGenTC;
    int clearType = ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRunTC;
    QString filter = QString(".xlsx");

    if (type == ivis::common::EventTypeEnum::EventTypeRunTC) {
        eventType = ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRunTC;
        clearType = ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfGenTC;
        filter = QString(".tc");
    }

    QStringList moduleList = getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule).toStringList();
    // if (moduleList.size() == 0) {
    //     moduleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toStringList();
    // }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(moduleList));
    updateAllModueList(filter);
    updateDataHandler(clearType, false);
    updateDataHandler(eventType, true, true);
}

void ControlMenu::excuteScript(const int& type, const QVariantList& selectInfoList) {
    if (selectInfoList.size() != 2) {
        qDebug() << "Fail to select info list size :" << selectInfoList.size();
        return;
    }

    QString moduleList = QString();
    foreach(const auto& module, selectInfoList[0].toList()) {
        moduleList.append(QString("%1%2").arg((moduleList.size() == 0) ? ("") : (" ")).arg(module.toString()));
    }
    if (moduleList.size() == 0) {
        qDebug() << "Fail to select module list : 0";
        return;
    }

    QString checkList = QString();
    foreach(const auto& check, selectInfoList[1].toList()) {
        checkList.append(QString("%1%2").arg((checkList.size() == 0) ? ("") : (" ")).arg(check.toString()));
    }
    QString currentPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    if (type == ivis::common::EventTypeEnum::EventTypeSelectModuleOfGenTC) {
        currentPath.append("/../../../tc_generator");
    } else if (type == ivis::common::EventTypeEnum::EventTypeSelectModuleOfRunTC) {
        currentPath.append("/../../../validator");
    } else {
        qDebug() << "Fail to excute script type :" << type;
        return;
    }

    QString cmd = QString();
    if (QDir::setCurrent(currentPath)) {    //  && (currentPath.contains("/model/SFC/CV"))
        if (type == ivis::common::EventTypeEnum::EventTypeSelectModuleOfGenTC) {
            QString negative = (checkList.size() == 0) ? (QString()) : (QString(" -o %1").arg(checkList));
            cmd = QString("./gen_tc.sh -c CV -m \"%1\"%2").arg(moduleList).arg(negative);
        } else if (type == ivis::common::EventTypeEnum::EventTypeSelectModuleOfRunTC) {
            QString altonPath = QString("/usr/local/bin/altonservice");
            QString ptList = (checkList.size() == 0) ? (QString()) : (QString(" %1").arg(checkList));
            cmd = QString("./run_tc.sh -b %1 -c CV -d -g -m \"%2\"%3").arg(altonPath).arg(moduleList).arg(ptList);
        } else {
            // nothing to do
        }
    } else {
        qDebug() << "Fail to change file folder :" << currentPath;
        return;
    }

#if 1
    if (mWatcher.isNull() == false) {
        qDebug() << "Running Watcher File -> Request Stop";
        mWatcher.reset();
    }

    QString filePath = QString("%1/TCResult.Info").arg(currentPath);
    mWatcher = QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(filePath));
    mWatcher.data()->start();
    connect(mWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
                                                                    [=](const QStringList& fileData) {
        int current = 0;
        int total = 0;
        bool complete = false;
        bool completeState = false;
        QVariant errorInfo = QVariant("");
        QVariantList moduleStateInfo = QVariantList();
        // qDebug() << "\t fileData :" << fileData;
        foreach(const auto& fileInfo, fileData) {
            QStringList info = fileInfo.split(" : ");
            if (info.size() != 2) {
                continue;
            }
            QString id = info.at(0);
            if (id.compare("CURRENT") == false) {
                current = info.at(1).toInt();
            } else if (id.compare("TOTAL") == false) {
                total = info.at(1).toInt();
            } else if (id.compare("COMPLETE") == false) {
                complete = true;
                completeState = (info.at(1).compare("PASS") == false);
            } else if (id.compare("ERROR_INFO") == false) {
                errorInfo = fileInfo;
            } else {
                moduleStateInfo.append(fileInfo);
            }
        }
        if (complete) {
            QString temp = QString("Test Case : %1").arg((completeState ? ("Pass") : ("Fail")));
            if (errorInfo.toString().size() > 0) {
                temp.append("\n");
                temp.append(errorInfo.toString());
            }
            errorInfo = temp;
        } else {
            errorInfo = QVariant("Test Case : Progressing");
        }
        QVariantList countInfo = QVariantList({current, total, complete});
        QVariantList testResultInfo = QVariantList({countInfo, errorInfo, moduleStateInfo});
        // qDebug() << "\t " << testResultInfo;
        // qDebug() << "\t [0] :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
        // qDebug() << "\t [1] :" << testResultInfo.at(1).toList().toString() << testResultInfo.at(1);
        // qDebug() << "\t [2] :" << testResultInfo.at(2).toList().size() << testResultInfo.at(2);
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo));

        if (complete) {
            emit mWatcher.data()->signalWatcherFileFail(10);
        }
    });
    connect(mWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileFail, [=](const int& count) {
        qDebug() << "\t signalWatcherFileFail :" << count;
        disconnect(mWatcher.data());
        mWatcher.reset();
    });
    // connect(mWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError, [=](const bool& error) {
    //     qDebug() << "\t FileOpenError :" << error;
    // });
#endif

#if 1
    if (mProcess.isNull() == false) {
        qDebug() << "Running Test Case -> Request Stop";
        mProcess.reset();
    }

    mProcess = QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(false),
                                                                                            &QObject::deleteLater);
    mProcess.data()->setCommandInfo(cmd);
    mProcess.data()->start();

    connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramStarted, [=]() {
        QVariantList countInfo = QVariantList({0, 10, false});
        QVariant errorInfo = QVariant("Test Case : Start");
        QVariantList moduleStateInfo = QVariantList({});
        QVariantList testResultInfo = QVariantList({countInfo, errorInfo, moduleStateInfo});

        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo));
    });

    connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramCompleted, [=](const bool& result) {
        qDebug() << "*************************************************************************************************";
        qDebug() << "PWD      :" << currentPath;
        qDebug() << "System   :" << cmd << ((result) ? ("=> <sucess>") : ("=> <fail>"));
        qDebug() << "*************************************************************************************************\n";

        disconnect(mProcess.data());
        mProcess.reset();
    });
#endif
}

void ControlMenu::cancelScript() {
    QStringList killProcess = QStringList({
        "python",
        "python3",
        "gen_tc.sh",
        "run_tc.sh",
    });
    foreach(const auto& process, killProcess) {
        int result = system((QString("pkill -9 -ef %1").arg(process)).toLatin1().data());
        qDebug() << "cancelScript - process :" << process << result;
    }

    QString currentPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    bool genTC = getData(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfGenTC).toBool();
    bool runTC = getData(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRunTC).toBool();
    if ((genTC) && (runTC == false)) {
        currentPath.append("/../../../tc_generator");
    } else if ((runTC) && (genTC == false)) {
        currentPath.append("/../../../validator");
    } else {
        currentPath.clear();
        qDebug() << "Fail to run script type error :" << genTC << genTC;
    }

    QString filePath = QString("%1/TCResult.Info").arg(currentPath);
    if (QDir::setCurrent(currentPath)) {
        QString errorInfo = QString("ERROR_INFO : User Request  - Process Exit");
        ivis::common::FileInfo::writeFile(filePath, errorInfo, true);
    }

    if (mWatcher.isNull() == false) {
        // emit mWatcher.data()->signalWatcherFileFail(10);
        disconnect(mWatcher.data());
        mWatcher.reset();
    }

    if (mProcess.isNull() == false) {
        // emit mProcess.data()->signalExcuteProgramCompleted(true);
        disconnect(mProcess.data());
        mProcess.reset();
    }
}

void ControlMenu::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeDefaultPath : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, value);
            break;
        }
        default : {
            break;
        }
    }
}

void ControlMenu::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlMenu::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram : {
            ivis::common::PopupButton button = ivis::common::PopupButton::Discard;
            bool fileSave = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();

            if (fileSave) {
                QVariantList text = QVariantList({STRING_POPUP_SAVE_FILE, STRING_POPUP_SAVE_FILE_TIP,
                                                    STRING_POPUP_SAVE, STRING_POPUP_DISCARD, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Exit, isHandler(), popupData, QVariant(text));
                if (button == ivis::common::PopupButton::OK) {
                    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel,
                                    ivis::common::EventTypeEnum::EventTypeFileSave, QVariant());
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                }
            }

            if (button != ivis::common::PopupButton::Cancel) {
                ControlManager::instance().data()->exitProgram(false);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAbout : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::About, isHandler(), popupData,
                                            QVariant(QVariantList({STRING_POPUP_ABOUT, STRING_POPUP_ABOUT_TIP})));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAboutQt : {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::AboutQt, isHandler(), popupData);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile :
        case ivis::common::EventTypeEnum::EventTypeFileNew :
        case ivis::common::EventTypeEnum::EventTypeFileOpen : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileSave :
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditCut :
        case ivis::common::EventTypeEnum::EventTypeEditCopy :
        case ivis::common::EventTypeEnum::EventTypeEditPaste : {
            qDebug() << "ControlMenu -> Edit :" << value;
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewConfig :
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress :
        case ivis::common::EventTypeEnum::EventTypeSettingTestReport :
        case ivis::common::EventTypeEnum::EventTypeReportResult :
        case ivis::common::EventTypeEnum::EventTypeReportCoverage : {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingDevPath :
        case ivis::common::EventTypeEnum::EventTypeSettingNodePath :
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath : {
            QString text = STRING_DEFAULT_PATH;
            int configType = ConfigInfo::ConfigTypeDefaultPath;

            if (type == ivis::common::EventTypeEnum::EventTypeSettingNodePath) {
                text = STRING_NODE_PATH;
                configType = ConfigInfo::ConfigTypeNodeAddressPath;
            } else if (type == ivis::common::EventTypeEnum::EventTypeSettingVsmPath) {
                text = STRING_VSM_PATH;
                configType = ConfigInfo::ConfigTypeVsmPath;
            } else {
                // ivis::common::EventTypeEnum::EventTypeSettingDevPath
            }

            QVariant path = ConfigSetting::instance().data()->readConfig(configType);
            QVariant popupData = QVariant();

            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                    QVariantList({text, path})) == ivis::common::PopupButton::OK) {
                ConfigSetting::instance().data()->writeConfig(configType, popupData);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenerateTC :
        case ivis::common::EventTypeEnum::EventTypeRunTC : {
            updateSelectModueList(type, QVariantList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenerateReport : {
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleOfGenTC :
        case ivis::common::EventTypeEnum::EventTypeSelectModuleOfRunTC : {
            updateSelectModueList(0, value.toList());
            excuteScript(type, value.toList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTestResultClick : {
            cancelScript();
            break;
        }
        default : {
            break;
        }
    }
}

void ControlMenu::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlMenu::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram :
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath : {
            slotHandlerEvent(eventType, eventValue);
            break;
        }
        default : {
            break;
        }
    }
}
