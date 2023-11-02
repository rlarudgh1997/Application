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
    controlTimer(AbstractControl::AbstractTimerStart, true, 200);

    QVariantList countInfo = QVariantList({-1, 30, true});
    QVariantList titleInfo = QVariantList({"Test Case : Start", "ERROR_INFO : To Do Test Code"});
    QVariantList moduleStateInfo = QVariantList({});
    QVariantList testResultInfo = QVariantList({countInfo, titleInfo, moduleStateInfo});

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo));
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
        QVariantList titleInfo = testResultInfo.at(1).toList();
        QVariantList moduleStateInfo = testResultInfo.at(2).toList();
        moduleStateInfo.append(tempState.at(current % tempState.size()));

        testResultInfo = QVariantList({countInfo, titleInfo, moduleStateInfo});

        // qDebug() << "\t 2 TestResultInfo :" << testResultInfo;
        // qDebug() << "\t   2-0 :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
        // qDebug() << "\t   2-1 :" << testResultInfo.at(1).toList().size() << testResultInfo.at(1);
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

void ControlMenu::updateSelectModueList(const int& eventType, const QVariantList& selectModule) {
    QString filter = QString();
    int runType = 0;

    if (eventType == ivis::common::EventTypeEnum::EventTypeGenTC) {
        filter = QString(".xlsx");
        runType = ivis::common::RunTypeEnum::RunTypeGenTC;
    } else if (eventType == ivis::common::EventTypeEnum::EventTypeRunTC) {
        filter = QString(".tc");
        runType = ivis::common::RunTypeEnum::RunTypeRunTC;
    } else {
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(selectModule));
        return;
    }

    QStringList moduleList = getData(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule).toStringList();
    // if (moduleList.size() == 0) {
    //     moduleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSelectModule).toStringList();
    // }

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(moduleList));
    updateAllModueList(filter);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun, runType, true);
}

void ControlMenu::excuteScript(const int& runType, const QVariantList& selectInfoList) {
    QString cmd = QString();
    QString subPath = QString();
    if (runType == ivis::common::RunTypeEnum::RunTypeEnterScriptText) {
        if (selectInfoList.size() == 1) {
            cmd = selectInfoList.at(0).toString();
        }

        if (cmd.contains("gen_tc.sh")) {
            subPath = QString("/../../../tc_generator");
        } else if (cmd.contains("run_tc.sh")) {
            subPath = QString("/../../../validator");
        } else {
            qDebug() << "Input text does not contain script commands :" << cmd;
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                    QVariantList({STRING_POPUP_INPUT_TEXT_ERROR, STRING_POPUP_INPUT_TEXT_ERROR_TIP}));
            return;
        }
        qDebug() << "CMD :" << subPath << cmd;
    } else {
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

        if (runType == ivis::common::RunTypeEnum::RunTypeGenTC) {
            QString negative = (checkList.size() == 0) ? (QString()) : (QString(" -o %1").arg(checkList));
            cmd = QString("./gen_tc.sh -c CV -m \"%1\"%2").arg(moduleList).arg(negative);
            subPath = QString("/../../../tc_generator");
        } else if (runType == ivis::common::RunTypeEnum::RunTypeRunTC) {
            QString altonPath = QString("/usr/local/bin/altonservice");
            QString ptList = (checkList.size() == 0) ? (QString()) : (QString(" %1").arg(checkList));
            cmd = QString("./run_tc.sh -b %1 -c CV -d -g -m \"%2\"%3").arg(altonPath).arg(moduleList).arg(ptList);
            subPath = QString("/../../../validator");
        } else {
            qDebug() << "Fail to excute script - runType :" << runType;
            return;
        }
    }

    QString currentPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    QString defaultRunPath = QString("%1%2").arg(currentPath).arg(subPath);
    if (QDir::setCurrent(defaultRunPath) == false) {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::SelectCellColumnError, isHandler(), popupData,
                                QVariantList({STRING_POPUP_DEFAULT_PATH_ERROR, STRING_POPUP_DEFAULT_PATH_ERROR_TIP}));
        qDebug() << "Fail to change folder :" << defaultRunPath;
        return;
    }
    qDebug() << "Default Run Path :" << defaultRunPath;

#if 1
    if (mWatcher.isNull() == false) {
        qDebug() << "Running Watcher File -> Request Stop";
        mWatcher.reset();
    }
    QString filePath = QString("%1/TCResult.Info").arg(defaultRunPath);
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultRunPath, filePath);
    mWatcher = QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(filePath));
    mWatcher.data()->start();
    connect(mWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
                                                                    [=](const QStringList& fileData) {
        int current = 0;
        int total = 0;
        bool complete = false;
        QString errorString = QString();
        QString completeString = QString();
        QVariantList titleInfo = QVariantList();
        QVariantList moduleStateInfo = QVariantList();
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
                completeString = info.at(1);
            } else if (id.compare("ERROR_INFO") == false) {
                errorString = fileInfo;
            } else {
                moduleStateInfo.append(fileInfo);
            }
        }

        QVariantList countInfo = QVariantList({current, total, complete});
        if (complete) {
            titleInfo.append(QString("Test Case : Completed(%1)").arg(completeString));
            titleInfo.append(errorString);
        } else {
            titleInfo.append("Test Case : Progressing");
            titleInfo.append("");
        }
        QVariantList testResultInfo = QVariantList({countInfo, titleInfo, moduleStateInfo});
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
        QVariantList titleInfo = QVariantList({"Test Case : Start", ""});
        QVariantList moduleStateInfo = QVariantList({});
        QVariantList testResultInfo = QVariantList({countInfo, titleInfo, moduleStateInfo});

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

void ControlMenu::cancelScript(const bool& complete) {
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

    if (complete == false) {
        QString defaultRunPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultRunPath).toString();
        qDebug() << "cancelScript - file write path :" << defaultRunPath;
        ivis::common::FileInfo::writeFile(defaultRunPath, QString("ERROR_INFO : User Request  - Process Exit"), true);
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
        case ivis::common::EventTypeEnum::EventTypeGenTC :
        case ivis::common::EventTypeEnum::EventTypeRunTC : {
            updateSelectModueList(type, QVariantList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenerateReport : {
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptText : {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText, true, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptTextCompleted : {
            excuteScript(ivis::common::RunTypeEnum::RunTypeEnterScriptText, QVariantList({value}));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun : {
            if (value.toList().size() == 3) {
                int runType = value.toList().at(0).toInt();
                QVariantList selectInfoList = QVariantList({value.toList().at(1), value.toList().at(2)});
                updateSelectModueList(0, selectInfoList);
                excuteScript(runType, selectInfoList);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenRunTCCancel : {
            cancelScript(value.toBool());
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
