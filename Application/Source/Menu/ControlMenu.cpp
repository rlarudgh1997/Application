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

bool ControlMenu::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlMenu::initCommonData(const int& currentMode, const int& displayType) {
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, displayType);
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
    updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeStart, 30);
#endif
}

void ControlMenu::initControlData() {
}

void ControlMenu::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMenu::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerMenu::signalHandlerEvent, this, &ControlMenu::slotHandlerEvent, Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlMenu::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlMenu::slotEventInfoChanged, Qt::UniqueConnection);
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
    if (getTimerId(AbstractControl::AbstractTimerStart) == timerId) {
        if (updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeTest, 30)) {
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
            for (const auto& sfc : sfcDirectory) {
                QDir subDirectory(QString("%1/%2").arg(defaultPath).arg(sfc));
                // subDirectory.setNameFilters(QStringList({".tc", ".xlsx"}));
                bool fileFound = false;
                for (const auto& file : subDirectory.entryList(QDir::Files)) {
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
    qDebug() << "SFC modules load path :" << sfcModules.size() << path;
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAllModule, sfcModules);
    // ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, sfcModules);
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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(moduleList));
    updateAllModueList(filter);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun, runType, true);
}

void ControlMenu::updateTestReportInfo(const int& eventType) {
    QVariantList reportInfo = QVariantList();
    if (eventType == ivis::common::EventTypeEnum::EventTypeTestReportResult) {
        reportInfo.append(static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeResult));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResult));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultSplit));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultConfig));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultExcel));
    } else {
        reportInfo.append(static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeCoverage));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverage));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageLine));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageFunction));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageBranch));
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestReport, reportInfo, true);
}

bool ControlMenu::updateTestResultInfo(const int& testReultType, const int& totalCount, const QStringList& infoData) {
    QVariantList countInfo = QVariantList();
    QVariantList titleInfo = QVariantList();
    QVariantList moduleStateInfo = QVariantList();
    bool complete = false;

    switch (testReultType) {
        case ivis::common::TestResultTypeEnum::TestResultTypeStart: {
            countInfo = QVariantList({0, totalCount, false});
            titleInfo = QVariantList({"Test Case : Start"});
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeUpdate: {
            int currentCount = 0;
            QString errorString = QString();
            QString completeString = QString();
            for (const auto& data : infoData) {
                QStringList info = data.split(" : ");
                if (info.size() != 2) {
                    continue;
                }
                QString id = info.at(0);
                if (id.compare("CURRENT") == false) {
                    currentCount += info.at(1).toInt();
                } else if (id.compare("TOTAL") == false) {
                    // totalCount = info.at(1).toInt();
                } else if (id.compare("COMPLETE") == false) {
                    complete = true;
                    completeString = info.at(1);
                } else if (id.compare("ERROR_INFO") == false) {
                    errorString = data;
                } else {
                    if (id.compare("CURRENT_POWER_TRAIN") == false) {
                        moduleStateInfo.append("---------------------------------------------------------------");
                    }
                    moduleStateInfo.append(data);
                }
            }
            countInfo = QVariantList({currentCount, totalCount, complete});
            if (complete) {
                titleInfo.append(QString("Test Case : Completed(%1)").arg(completeString));
                titleInfo.append(errorString);
            } else {
                titleInfo.append("Test Case : Progressing");
                titleInfo.append("");
            }
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeCancel:
        case ivis::common::TestResultTypeEnum::TestResultTypeError: {
            QVariantList readInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
            countInfo = QVariantList({0, totalCount, true});
            if (testReultType == ivis::common::TestResultTypeEnum::TestResultTypeCancel) {
                titleInfo = QVariantList({"Test Case : Completed(FAIL)", "ERROR_INFO : Request cancel"});
            } else {
                titleInfo = QVariantList({"Test Case : Completed(FAIL)", "ERROR_INFO : Script running error"});
            }
            moduleStateInfo = readInfo.at(2).toList();
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeCheckError: {
            countInfo = QVariantList({0, totalCount, true});
            QString detailInfo =
                ((infoData.size() == 1) ? (infoData.at(0)) : (QString("ERROR_INFO : Detailed error information is not defined")));
            titleInfo = QVariantList({"Test Case : Completed(FAIL)", detailInfo});
            break;
        }
#if defined(USE_TEST_RESULT_TEMP)
        case ivis::common::TestResultTypeEnum::TestResultTypeTest: {
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
                "DSW : FAIL",
            });

            QVariantList readInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
            countInfo = readInfo.at(0).toList();
            int current = countInfo.at(0).toInt() + 1;
            int total = countInfo.at(1).toInt();
            complete = (current > total);
            countInfo.clear();
            countInfo = QVariantList({current, total, complete});
            titleInfo = readInfo.at(1).toList();
            if (complete) {
                titleInfo.append("ERROR_INFO : To Do Test Code");
            }
            moduleStateInfo = readInfo.at(2).toList();
            moduleStateInfo.append(tempState.at(current % tempState.size()));
            break;
        }
#endif
        default: {
            break;
        }
    }

    QVariantList testResultInfo = QVariantList({countInfo, titleInfo, moduleStateInfo});
    // qDebug() << "\t TestResultInfo :" << testResultInfo;
    // qDebug() << "\t   0 :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
    // qDebug() << "\t   1 :" << testResultInfo.at(1).toList().size() << testResultInfo.at(1);
    // qDebug() << "\t   2 :" << testResultInfo.at(2).toList().size() << testResultInfo.at(2);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(testResultInfo), true);

    if (complete) {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::ScriptRunnigCompleted, isHandler(), popupData,
                                       QVariantList({STRING_SCRIPT_RUNNIG_COMPLETED, STRING_SCRIPT_RUNNIG_COMPLETED_TIP}));
    }
    return complete;
}

void ControlMenu::excuteScript(const int& runType, const bool& state, const QVariantList& infoList) {
    static int totalCount = 10;
    QString fileName = QString("TCResult.info");
    QString cmd = QString();
    QString subPath = QString();
    QStringList checkBinary = QStringList();

    if (runType == ivis::common::RunTypeEnum::RunTypeEnterScriptText) {
        cmd = (infoList.size() == 1) ? (infoList.at(0).toString()) : (QString());

        if (cmd.contains("gen_tc.sh")) {
            subPath = QString("/../../../tc_generator");
        } else if (cmd.contains("run_tc.sh")) {
            subPath = QString("/../../../validator");
        } else if (cmd.contains("gen_tcreport.sh")) {
            fileName = QString("TCReport.info");
            subPath = QString("/../../../validator");
        } else if (cmd.contains("gen_gcov_report.sh")) {
            fileName = QString("GCOVReport.info");
            subPath = QString("/../../../validator");
        } else {
            qDebug() << "Input text does not contain script commands :" << cmd;
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::RunPathError, isHandler(), popupData,
                                           QVariantList({STRING_POPUP_INPUT_TEXT_ERROR, STRING_POPUP_INPUT_TEXT_ERROR_TIP}));
            return;
        }
        qDebug() << "CMD :" << subPath << cmd;
    } else if ((runType == ivis::common::RunTypeEnum::RunTypeTCReport) ||
               (runType == ivis::common::RunTypeEnum::RunTypeGcovReport)) {
        QVariantList options = infoList;
        if (options.size() != 3) {
            qDebug() << "Fail to report info options size :" << options.size();
            return;
        }

        fileName = QString((runType == ivis::common::RunTypeEnum::RunTypeTCReport) ? ("TCReport.info") : ("GCOVReport.info"));
        subPath = QString("/../../../validator");
        // ./gen_tcreport.sh -c CV -s S -o C -t E    (-s : Split,  -o : Config,   -t : Excel)
        // ./gen_gcov_report.sh -c CV -b ON -f ON    (-b : Branch, -f : Function, -n : Line )
        cmd = QString("./%1.sh -c CV")
                  .arg((runType == ivis::common::RunTypeEnum::RunTypeGcovReport) ? (QString("gen_gcov_report"))
                                                                                 : (QString("gen_tcreport")));
        if (state) {
            QString option1 = (options.at(0).toBool()) ? (" -s S") : ("");
            QString option2 = (options.at(1).toBool()) ? (" -o C") : ("");
            QString option3 = (options.at(2).toBool()) ? (" -t E") : ("");
            if (runType == ivis::common::RunTypeEnum::RunTypeGcovReport) {
                option1 = (options.at(0).toBool()) ? (" -b ON") : (" -b OFF");
                option2 = (options.at(1).toBool()) ? (" -f ON") : (" -f OFF");
                option3 = QString();  // (line) ? (" -n ON") : ("");     -> Not define
            }
            cmd.append(QString("%1%2%3").arg(option1).arg(option2).arg(option3));
        }
        qDebug() << "RunTypeTC CMD :" << subPath << cmd << ", State :" << state << ", Options :" << options;
    } else {
        if (infoList.size() != 2) {
            qDebug() << "Fail to select info list size :" << infoList.size();
            return;
        }

        QString moduleList = QString();
        totalCount = infoList[0].toList().size();
        if (totalCount == 0) {
            qDebug() << "Fail to select module list : 0";
            return;
        }
        for (const auto& module : infoList[0].toList()) {
            moduleList.append(QString("%1%2").arg((moduleList.size() == 0) ? ("") : (" ")).arg(module.toString()));
        }

        QString checkList = QString();
        for (const auto& check : infoList[1].toList()) {
            checkList.append(QString("%1%2").arg((checkList.size() == 0) ? ("") : (" ")).arg(check.toString()));
        }

        if (runType == ivis::common::RunTypeEnum::RunTypeGenTC) {
            QString negative = (state) ? (QString(" -o Negative")) : (QString());
            cmd = QString("./gen_tc.sh -c CV -m \"%1\"%2").arg(moduleList).arg(negative);
            subPath = QString("/../../../tc_generator");
        } else if (runType == ivis::common::RunTypeEnum::RunTypeRunTC) {
            QString altonPath = (state) ? (QString("/usr/local/bin/altonservice"))
                                        : (QString("%1%2").arg(ivis::common::APP_PWD()).arg("/../Alton/altonservice"));
            QString docker = (state) ? (QString("-d ")) : (QString());
            QString ptList = (checkList.size() == 0) ? (QString()) : (QString(" %1").arg(checkList));
            cmd = QString("./run_tc.sh -b %1 -c CV %2-g -m \"%3\"%4").arg(altonPath).arg(docker).arg(moduleList).arg(ptList);
            subPath = QString("/../../../validator");
            int ptCount = infoList[1].toList().size();
            totalCount = totalCount * ((ptCount == 0) ? (3) : (ptCount));  // No Select PT -> All(EV, FCEV, ICV) = 3

            checkBinary.append(altonPath);
            checkBinary.append("/build/sfc_validator");
        } else {
            qDebug() << "Fail to excute script - runType :" << runType;
            return;
        }
    }

    QString currentPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    QString defaultRunPath = QString("%1%2").arg(currentPath).arg(subPath);
    QVariant popupData = QVariant();
    if (QDir::setCurrent(defaultRunPath) == false) {
        ivis::common::Popup::drawPopup(ivis::common::PopupType::RunPathError, isHandler(), popupData,
                                       QVariantList({STRING_POPUP_DEFAULT_PATH_ERROR, STRING_POPUP_BINARY_NOT_EXISTS_TIP}));
        qDebug() << "Fail to change folder :" << defaultRunPath;
        return;
    }

    if (checkBinary.size() > 0) {
        int index = 0;
        for (const auto& binary : checkBinary) {
            QString checkFile = (index == 0) ? (binary) : (QString("%1%2").arg(defaultRunPath).arg(binary));
            if (QFile::exists(checkFile) == false) {
                updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeCheckError, totalCount,
                                     QStringList({STRING_BINARY_NOT_EXISTS_ERROR + binary}));
                qDebug() << "Fail to binary not exists :" << checkFile;
                return;
            }
            index++;
        }
    }

    qDebug() << "Default :" << defaultRunPath;
    qDebug() << "Command :" << cmd;

    if (mWatcher.isNull() == false) {
        qDebug() << "Running Watcher File -> Request Stop";
        mWatcher.reset();
    }
    QString filePath = QString("%1/%2").arg(defaultRunPath).arg(fileName);
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultRunPath, filePath);
    mWatcher = QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(filePath));
    mWatcher.data()->start();
    connect(mWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
            [=](const QStringList& fileData) {
                if (updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeUpdate, totalCount, fileData)) {
                    emit mWatcher.data()->signalWatcherFileFail(20);
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

    if (mProcess.isNull() == false) {
        qDebug() << "Running Test Case -> Request Stop";
        mProcess.reset();
    }
    mProcess =
        QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(false), &QObject::deleteLater);
    mProcess.data()->setCommandInfo(cmd);
    mProcess.data()->start();

    connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramStarted,
            [=]() { updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeStart, totalCount); });

    connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramCompleted, [=](const bool& result) {
        qDebug() << "*************************************************************************************************";
        qDebug() << "PWD      :" << currentPath;
        qDebug() << "System   :" << cmd << ((result) ? ("=> <sucess>") : ("=> <fail>"));
        qDebug() << "*************************************************************************************************\n";

        disconnect(mProcess.data());
        mProcess.reset();

        if (result == false) {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultRunPath).toString().size() == 0) {
                updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeCancel, totalCount);
            } else {
                updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeError, totalCount);
            }
        }
    });
}

void ControlMenu::cancelScript(const bool& complete) {
    const QStringList killProcess = QStringList({
        "python",
        "python3",
        "gen_tc.sh",
        "run_tc.sh",
        "gen_tcreport.sh",
        "gen_gcov_report.sh",
        "sfc_validator",
        "altonservice",
    });
    QString defaultRunPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultRunPath).toString();
    if (complete == false) {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDefaultRunPath, "");
    }
    for (const auto& info : killProcess) {
        QStringList log;
        ivis::common::ExcuteProgram process(false);
        bool result = process.start(QString("pkill -9 -ef %1").arg(info), log);
        qDebug() << "Terminate - process :" << info << ", result :" << result;
    }

    if (complete == false) {
        qDebug() << "Terminate - file write path :" << defaultRunPath;
        ivis::common::FileInfo::writeFile(defaultRunPath, QString("ERROR_INFO : User Request  - Process Exit"), true);
    }

    if (mWatcher.isNull() == false) {
        // emit mWatcher.data()->signalWatcherFileFail(20);
        disconnect(mWatcher.data());
        mWatcher.reset();
    }

    if (mProcess.isNull() == false) {
        // emit mProcess.data()->signalExcuteProgramCompleted(false);
        disconnect(mProcess.data());
        mProcess.reset();
    }
}

int ControlMenu::saveTestReportInfo(const int& reportType, const QList<bool>& value) {
    int count = 0;
    int configType = (reportType == ivis::common::TestReportTypeEnum::TestReportTypeResult)
                         ? (ConfigInfo::ConfigTypeReportResult)
                         : (ConfigInfo::ConfigTypeReportCoverage);
    for (const auto& configValue : value) {
        ConfigSetting::instance().data()->writeConfig((configType + count), configValue);
        count++;
    }
    return count;
}

void ControlMenu::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeDefaultPath: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, value);
            break;
        }
        default: {
            break;
        }
    }
}

void ControlMenu::slotHandlerEvent(const int& type, const QVariant& value) {
    // qDebug() << "ControlMenu::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram: {
            ivis::common::PopupButton button = ivis::common::PopupButton::Discard;
            bool fileSave = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();

            if (fileSave) {
                QVariantList text = QVariantList({STRING_POPUP_SAVE_FILE, STRING_POPUP_SAVE_FILE_TIP, STRING_POPUP_SAVE,
                                                  STRING_POPUP_DISCARD, STRING_POPUP_CANCEL});
                QVariant popupData = QVariant();
                button = ivis::common::Popup::drawPopup(ivis::common::PopupType::Exit, isHandler(), popupData, QVariant(text));
                if (button == ivis::common::PopupButton::OK) {
                    sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, ivis::common::EventTypeEnum::EventTypeFileSave,
                                  QVariant());
                    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
                }
            }

            if (button != ivis::common::PopupButton::Cancel) {
                ControlManager::instance().data()->exitProgram(false);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAbout: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::About, isHandler(), popupData,
                                           QVariant(QVariantList({STRING_POPUP_ABOUT, STRING_POPUP_ABOUT_TIP})));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeHelpAboutQt: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::AboutQt, isHandler(), popupData);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeLastFile:
        case ivis::common::EventTypeEnum::EventTypeFileNew:
        case ivis::common::EventTypeEnum::EventTypeFileOpen: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeFileSave:
        case ivis::common::EventTypeEnum::EventTypeFileSaveAs: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEditCut:
        case ivis::common::EventTypeEnum::EventTypeEditCopy:
        case ivis::common::EventTypeEnum::EventTypeEditPaste:
        case ivis::common::EventTypeEnum::EventTypeEditUndo:
        case ivis::common::EventTypeEnum::EventTypeEditRedo:
        case ivis::common::EventTypeEnum::EventTypeEditCellInsert:
        case ivis::common::EventTypeEnum::EventTypeEditCellDelete:
        case ivis::common::EventTypeEnum::EventTypeEditCellMergeSplit: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeExcel, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewConfig:
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress:
        case ivis::common::EventTypeEnum::EventTypeSettingTestReport:
        case ivis::common::EventTypeEnum::EventTypeReportResult:
        case ivis::common::EventTypeEnum::EventTypeReportCoverage: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingDevPath:
        case ivis::common::EventTypeEnum::EventTypeSettingNodePath:
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath: {
            QString text = STRING_DEFAULT_PATH;
            int configType = ConfigInfo::ConfigTypeDefaultPath;

            if (type == ivis::common::EventTypeEnum::EventTypeSettingNodePath) {
                text = STRING_NODE_PATH;
                configType = ConfigInfo::ConfigTypeNodeAddressPath;
            } else if (type == ivis::common::EventTypeEnum::EventTypeSettingVsmPath) {
                text = STRING_VSM_PATH;
                configType = ConfigInfo::ConfigTypeVsmPath;
            } else {
            }

            QVariant path = ConfigSetting::instance().data()->readConfig(configType);
            QVariant popupData = QVariant();

            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                               QVariantList({text, path})) == ivis::common::PopupButton::OK) {
                ConfigSetting::instance().data()->writeConfig(configType, popupData);
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, ivis::common::EventTypeEnum::EventTypeInitModule);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenTC:
        case ivis::common::EventTypeEnum::EventTypeRunTC: {
            updateSelectModueList(type, QVariantList());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeTestReportResult:
        case ivis::common::EventTypeEnum::EventTypeTestReportCoverage: {
            updateTestReportInfo(type);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeRunTestReport: {
            QVariantList reportInfo = value.toList();
            if (reportInfo.size() != 5) {
                qDebug() << "Fail to report info size :" << reportInfo.size();
                return;
            }
            int testReportType = reportInfo.at(0).toInt();
            int runType = ivis::common::RunTypeEnum::RunTypeTCReport;
            if (testReportType == static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeCoverage)) {
                runType = ivis::common::RunTypeEnum::RunTypeGcovReport;
            }
            bool state = reportInfo.at(1).toBool();
            bool option1 = reportInfo.at(2).toBool();
            bool option2 = reportInfo.at(3).toBool();
            bool option3 = reportInfo.at(4).toBool();
            if (saveTestReportInfo(testReportType, QList({state, option1, option2, option3})) == 4) {
                excuteScript(runType, state, QVariantList({option1, option2, option3}));
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptText: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText, true, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptTextCompleted: {
            excuteScript(ivis::common::RunTypeEnum::RunTypeEnterScriptText, false, QVariantList({value}));
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun: {
            if (value.toList().size() == 4) {
                int runType = value.toList().at(0).toInt();
                bool state = value.toList().at(1).toBool();
                QVariantList selectInfoList = QVariantList({value.toList().at(2), value.toList().at(3)});
                updateSelectModueList(0, selectInfoList);
                excuteScript(runType, state, selectInfoList);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenRunTCCancel: {
            cancelScript(value.toBool());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleError: {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(
                ivis::common::PopupType::ModuleSelectError, isHandler(), popupData,
                QVariantList({STRING_POPUP_MODULE_SELECT_ERROR, STRING_POPUP_MODULE_SELECT_ERROR_TIP}));
            break;
        }
        default: {
            break;
        }
    }
}

void ControlMenu::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    // qDebug() << "ControlMenu::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        case ivis::common::EventTypeEnum::EventTypeExitProgram:
        case ivis::common::EventTypeEnum::EventTypeSettingVsmPath: {
            slotHandlerEvent(eventType, eventValue);
            break;
        }
        default: {
            break;
        }
    }
}
