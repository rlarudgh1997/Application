#include "ControlMenu.h"
#include "HandlerMenu.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonResource.h"
#include "CommonPopup.h"

#include <QFileSystemWatcher>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
}

void ControlMenu::initNormalData() {
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeSelectModule, QVariant());
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeDoFileSave, false);
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QStringList vehicleTypeList = QStringList();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toStringList();
    } else {
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toStringList();
    }
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType, vehicleTypeList);

    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath, sfcModelPath);
    updateAllModuleList(QString(".hpp"));  // yml, hpp 파일이 있으면 모듈로 인식

#if defined(USE_TEST_RESULT_TEMP)
    controlTimer(AbstractControl::AbstractTimerStart, true, 200);
    updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeStart, 30);
#endif
}

void ControlMenu::initControlData() {
}

void ControlMenu::resetControl(const bool& reset) {
    if (reset) {
        initCommonData(getData(ivis::common::PropertyTypeEnum::PropertyTypeMode).toInt(),
                       getData(ivis::common::PropertyTypeEnum::PropertyTypeDisplay).toInt());
        initNormalData();
        initControlData();
        qDebug() << "\t Reset - ControlMenu";
    }
}

void ControlMenu::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &AbstractHandler::signalHandlerEvent,
                [=](const int& type, const QVariant& value) { slotHandlerEvent(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged,
                [=](const int& type, const QVariant& value) { slotConfigChanged(type, value); });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigReset,
                [=](const bool& resetAll) { resetControl(resetAll); });
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                [=](const int& displayType, const int& eventType, const QVariant& eventValue) {
                    slotEventInfoChanged(displayType, eventType, eventValue);
                });
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
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                      ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
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

void ControlMenu::updateSelectAppMode() {
    QVariant appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode);
    QVariantList appModeList = QVariantList({"Vehicle CV", "Vehicle PV"});
#if defined(USE_APP_MODE_TAV)
    appModeList.append("TAV");
#endif

    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAppModeList, QVariant(appModeList));
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeAppMode, QVariant(appMode), true);
}

QStringList ControlMenu::isModuleListFromJson() {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModeCV = (appMode == ivis::common::AppModeEnum::AppModeTypeCV);
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QString jsonFile = QString("%1/SFC/config/%2.json").arg(sfcModelPath).arg((appModeCV) ? ("CV") : ("platform"));
    QByteArray jsonData = ivis::common::FileInfo::readFileByteArray(jsonFile);

    // Json Parsing
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Fail to json parsing error :" << error.errorString();
        return QStringList();
    }
    if (jsonDoc.isObject() == false) {
        qDebug() << "Fail to json format invalid";
        return QStringList();
    }

    // Read : Json Data
    QStringList moduleList = QStringList();
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.contains("SFCConfiguration") && jsonObj["SFCConfiguration"].isObject()) {  // Read : SFCConfiguration
        QJsonObject sfcConfig = jsonObj["SFCConfiguration"].toObject();
        // if (sfcConfig.contains("Version") && sfcConfig["Version"].isString()) {    // Read : Version
        //     QString version = sfcConfig["Version"].toString();
        // }
        // if (sfcConfig.contains("Description") && sfcConfig["Description"].isString()) {   // Read : Description
        //     QString description = sfcConfig["Description"].toString();
        // }
        if (sfcConfig.contains("SFCs") && sfcConfig["SFCs"].isArray()) {  // Read : SFCs
            for (const QJsonValue& module : sfcConfig["SFCs"].toArray()) {
                if (module.isString()) {
                    // qDebug() << "\t Module Json :" << module.toString();
                    moduleList.append(module.toString());
                }
            }
        }
    }
    if (appModeCV) {
        QDir commonModuleDir(QString("%1/SFC/CV/Common_Module").arg(sfcModelPath));
        for (const auto& commonModule : commonModuleDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            moduleList.append(commonModule);
            // qDebug() << "\t Module  Dir :" << commonModule;
        }
    }
    qDebug() << "Moudle Json File :" << jsonFile;

    moduleList.sort();
    moduleList.removeDuplicates();
    return moduleList;
}

void ControlMenu::updateAllModuleList(const QString& filter) {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModePV = (appMode == ivis::common::AppModeEnum::AppModeTypePV);
#if defined(USE_DEFAULT_MODULE_INFO_FILE)
    QStringList sfcModules = QStringList();
    QStringList findPath = QStringList();
    QString path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    if (appModePV) {
        QVariant sfcSpecList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV);
        path.append("/model/SFC");
        for (const auto& spec : sfcSpecList.toStringList()) {
            findPath.append(QString("%1/%2").arg(path).arg(spec));
        }
    } else {
        path.append("/model/SFC/CV");
        findPath.append(path);
    }

    // Find SFC Path List
    for (const auto& sfcPath : findPath) {
        QDir directory(sfcPath);
        QStringList sfcDirectory = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        // qDebug() << "Path :" << sfcPath << ", Filter :" << filter;
        for (const auto& sfc : sfcDirectory) {
            if (filter.size() == 0) {
                // qDebug() << "\t Module :" << sfc;
                sfcModules.append(sfc);
                continue;
            }
            QDir subDirectory(QString("%1/%2").arg(sfcPath).arg(sfc));
            for (const auto& file : subDirectory.entryList(QDir::Files)) {
                if (file.contains(filter)) {
                    // qDebug() << "\t Module[" << filter << "] :" << sfc << file;
                    sfcModules.append(sfc);
                    break;
                }
            }
        }
    }

    // Not Found : Default Module List
    if ((filter.size() == 0) && (sfcModules.size() == 0)) {
        path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeNodeAddressPath).toString();
        if (appModePV) {
            path.append("/PV");
        } else {
            path.append("/CV");
        }
        sfcModules = ivis::common::FileInfo::readFile(QString("%1/DefaultModule.info").arg(path));
    }
#else
    QStringList sfcModules = isModuleListFromJson();
#endif

    qDebug() << ((appModePV) ? ("[PV]") : ("[CV]")) << "SFC Module :" << sfcModules.size() << ", Filter :" << filter;
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAllModule, sfcModules);
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
    updateAllModuleList(filter);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun, runType, true);
}

void ControlMenu::updateTestReportInfo(const int& eventType) {
    QVariantList reportInfo = QVariantList();
    if (eventType == ivis::common::EventTypeEnum::EventTypeTestReportResult) {
        reportInfo.append(static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeTC));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResult));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultSplit));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultConfig));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportResultExcel));
    } else {
        reportInfo.append(static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeGCOV));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverage));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageFunction));
        reportInfo.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageBranch));
        reportInfo.append(true);  // ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeReportCoverageLine)
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
            titleInfo = QVariantList({"Test Case : Start", ""});
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
                titleInfo.append({QString("Test Case : Completed(%1)").arg(completeString), errorString});
            } else {
                titleInfo.append({"Test Case : Progressing", ""});
            }
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeCancel: {
            QVariantList readInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
            countInfo = QVariantList({0, totalCount, true});
            titleInfo = QVariantList({"Test Case : Completed(FAIL)", "ERROR_INFO : User request cancel"});
            moduleStateInfo = readInfo.at(2).toList();
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeError: {
            QVariantList readInfo = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
            countInfo = QVariantList({0, totalCount, true});
            titleInfo = QVariantList({"Test Case : Completed(FAIL)", "ERROR_INFO : Script running error"});
            moduleStateInfo = readInfo.at(2).toList();
            break;
        }
        case ivis::common::TestResultTypeEnum::TestResultTypeCheckError: {
            countInfo = QVariantList({0, totalCount, true});
            QString detailInfo =
                (infoData.size() == 1) ? (infoData.at(0)) : (QString("ERROR_INFO : Detailed error information is not defined"));
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
        // Calling the main thread function from watcher thread : Error to set parent
        QMetaObject::invokeMethod(this, "slotControlUpdate",
                                  Q_ARG(int, ivis::common::ControlUpdateTypeEnum::ControlUpdateTypeScriptRunnigCompleted));
    }

    return complete;
}

void ControlMenu::updateViewLogFile() {
    QFileInfoList fileList = QFileInfoList();
    QStringList fileInfo = ivis::common::FileInfo::isFileListInfo(ivis::common::APP_PWD(), QString(".log"), fileList);
    QString baseName = QString("%1_RunScript.log");
    QVariantList logFileInfo = QVariantList();
    QStringList logFileName = QStringList();
    for (const auto& file : fileInfo) {
        QPair<int, QString> title = QPair<int, QString>();
        if (file.compare(baseName.arg(0)) == false) {
            title = QPair<int, QString>(0, QString("Generate TC"));
        } else if (file.compare(baseName.arg(1)) == false) {
            title = QPair<int, QString>(1, QString("Run TC"));
        } else if (file.compare(baseName.arg(2)) == false) {
            title = QPair<int, QString>(2, QString("TC Report"));
        } else if (file.compare(baseName.arg(3)) == false) {
            title = QPair<int, QString>(3, QString("GCOV Report"));
        } else if (file.compare(baseName.arg(4)) == false) {
            title = QPair<int, QString>(4, QString("Enter Script Text"));
        } else if (file.compare(baseName.arg(5)) == false) {
            title = QPair<int, QString>(5, QString("Gen SSFS"));
        } else {
            continue;
        }
        logFileInfo.append(QVariant(QVariantList({title.first, title.second})));
        logFileName.append(title.second);
        // qDebug() << "File :" << file << "->" << title.first << title.second;
    }
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileListInfo, logFileInfo);
    updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileList, logFileName, true);
}

void ControlMenu::updateViewLogDisplay(const QString& titleName) {
    QString fileName = QString();
    for (const auto& logFileInfo : getData(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileListInfo).toList()) {
        QVariantList title = logFileInfo.toList();
        if (title.size() != 2) {
            continue;
        }
        // qDebug() << "updateViewLogDisplay :" << titleName << title.at(0).toInt() << title.at(1).toString();
        if (titleName.compare(title.at(1).toString()) == false) {
            fileName = QString("%1_RunScript.log").arg(title.at(0).toInt());
            break;
        }
    }
    if (fileName.size() > 0) {
        QString readFilePath = QString("%1/%2").arg(ivis::common::APP_PWD()).arg(fileName);
        QStringList detailLog = ivis::common::FileInfo::readFile(readFilePath);
        QVariantList logInfo = QVariantList({titleName, detailLog});
        // qDebug() << "\t View Log Info :" << fileName << titleName << detailLog.size();
        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileInfo, QVariant(logInfo), true);
    }
}

void ControlMenu::startWatcherFile(const int& type, const QString& watcherFile, const int& totalCount) {
    if (QFile::exists(watcherFile)) {
        bool deleteResult = QFile::remove(watcherFile);
        qDebug() << ((deleteResult) ? ("Success") : ("Fail")) << "delete file :" << watcherFile;
    }

    stopWatcherFile(type);
    if (type == ivis::common::WatcherTypeEnum::WatcherTypeRunScript) {
        mWatcherRunScript =
            QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(watcherFile, 30));
        mWatcherRunScript.data()->start();
        connect(mWatcherRunScript.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
                [=](const bool& init, const QStringList& data) {
                    QStringList previousData =
                        getData(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfoPrevious).toStringList();
                    if ((data.size() - previousData.size()) > 0) {
                        QStringList selectedData = data.mid(previousData.size(), data.size());
                        updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfo, selectedData);
                    }
                    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfoPrevious, data);
                });
        connect(mWatcherRunScript.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError,
                [=](const QString& errorFile) { qDebug() << "\t WatcherRunScript File Error :" << errorFile; });
        connect(mWatcherRunScript.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileState, [=](const int& state) {
            qDebug() << "\t WatcherRunScript File State :" << state << ((state >= 0) ? ("-> Complete") : ("-> Fail"));
        });
    } else if (type == ivis::common::WatcherTypeEnum::WatcherTypeTestResult) {
        mWatcherTestResult =
            QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(watcherFile, 0));
        mWatcherTestResult.data()->start();
        connect(mWatcherTestResult.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
                [=](const bool& init, const QStringList& data) {
                    int testResultType = ivis::common::TestResultTypeEnum::TestResultTypeUpdate;
                    if (getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel).toBool()) {
                        testResultType = ivis::common::TestResultTypeEnum::TestResultTypeCancel;
                        // updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel, false);
                    }
                    if (updateTestResultInfo(testResultType, totalCount, data)) {
                        if (mWatcherRunScript.isNull() == false) {
                            mWatcherRunScript.data()->readFinalData();
                        }
                    }
                });
        connect(mWatcherTestResult.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError,
                [=](const QString& errorFile) { qDebug() << "\t WatcherTestResult File Error :" << errorFile; });
        connect(mWatcherTestResult.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileState, [=](const int& state) {
            qDebug() << "\t WatcherTestResult File State :" << state << ((state >= 0) ? ("-> Complete") : ("-> Fail"));
        });
    } else {
    }
}

void ControlMenu::startProcess(const QString& command, const QString& arg, const int& totalCount) {
    stopProcess();

    QString argument = (arg.size() == 0) ? (QString("")) : (QString(" > %1").arg(arg));
    mProcess =
        QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(false), &QObject::deleteLater);
    mProcess.data()->setCommandInfo(command, argument);
    mProcess.data()->start();
    connect(
        mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramInfo, [=](const bool& start, const bool& result) {
            bool genSSFS = command.contains("./gen_ssfs.sh");
            bool requestCancel = getData(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel).toBool();
            int testResultType = ivis::common::TestResultTypeEnum::TestResultTypeInvalid;
            QStringList infoData = QStringList();
            QString sfcModelPath = QString();

            if (start) {
                testResultType = ivis::common::TestResultTypeEnum::TestResultTypeStart;
                if (genSSFS) {
                    testResultType = ivis::common::TestResultTypeEnum::TestResultTypeUpdate;
                    infoData.append(QString("Script running : %1").arg(command));
                }
            } else if (requestCancel) {
                testResultType = ivis::common::TestResultTypeEnum::TestResultTypeCancel;
                // updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel, false);
            } else if (result == false) {
                testResultType = ivis::common::TestResultTypeEnum::TestResultTypeError;
            } else {
                if (genSSFS) {
                    sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
                    testResultType = ivis::common::TestResultTypeEnum::TestResultTypeUpdate;
                    infoData.append("Generated file : NodeAddressSFC.info, NodeAddressVSM.info");
                    infoData.append("COMPLETE : PASS");
                }
            }

            if (testResultType != ivis::common::TestResultTypeEnum::TestResultTypeInvalid) {
                updateTestResultInfo(testResultType, totalCount, infoData);
            }

            if (sfcModelPath.size() > 0) {
                ivis::common::ExcuteProgram process;
                QStringList log = QStringList();
                QString nodeAddressPath = QString("cp -rf %1/ssfs/include/generated/*.info %2/../Python/NodeAddress/%3")
                                              .arg(sfcModelPath)
                                              .arg(ivis::common::APP_PWD())
                                              .arg((command.contains(" PV") ? ("PV") : ("CV")));
                QString nodeAddressPathRun = QString("cp -rf %1/ssfs/include/generated/*.info %2/NodeAddress/%3")
                                                 .arg(sfcModelPath)
                                                 .arg(ivis::common::APP_PWD())
                                                 .arg((command.contains(" PV") ? ("PV") : ("CV")));
                process.start(nodeAddressPath, log);
                process.start(nodeAddressPathRun, log);
            }

            if (start == false) {
                qDebug() << "*************************************************************************************************";
                qDebug() << "Commnad :" << command;
                qDebug() << "Result  :" << ((result) ? ("Success") : ("fail")) << ", RequestCancel :" << requestCancel;
                qDebug() << "*************************************************************************************************\n";
            }
        });
}

bool ControlMenu::excuteScript(const int& runType, const bool& state, const QVariantList& infoList) {
    static int totalCount = 10;
    QString fileName = QString("TCResult.info");
    QString cmd = QString();
    QString subPath = QString();
    QString currentPWD = ivis::common::APP_PWD();
    QStringList checkBinary = QStringList();
    QStringList moduleList = QStringList();
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QString pvCvName = (appMode == ivis::common::AppModeEnum::AppModeTypePV) ? ("PV") : ("CV");

    if (runType == ivis::common::RunTypeEnum::RunTypeEnterScriptText) {
        cmd = (infoList.size() == 1) ? (infoList.at(0).toString()) : (QString());

        if (cmd.contains("gen_tc.sh")) {
            subPath = QString("tc_generator");
        } else if (cmd.contains("run_tc.sh")) {
            subPath = QString("validator");
        } else if (cmd.contains("gen_tcreport.sh")) {
            fileName = QString("TCReport.info");
            subPath = QString("validator");
        } else if (cmd.contains("gen_gcov_report.sh")) {
            fileName = QString("GCOVReport.info");
            subPath = QString("validator");
        } else {
            qDebug() << "Input text does not contain script commands :" << cmd;
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::RunPathError, isHandler(), popupData,
                                           QVariantList({STRING_POPUP_INPUT_TEXT_ERROR, STRING_POPUP_INPUT_TEXT_ERROR_TIP}));
            return false;
        }
        qDebug() << "CMD :" << subPath << cmd;
    } else if ((runType == ivis::common::RunTypeEnum::RunTypeTCReport) ||
               (runType == ivis::common::RunTypeEnum::RunTypeGcovReport)) {
        QVariantList options = infoList;
        if (options.size() != 3) {
            qDebug() << "Fail to report info options size :" << options.size();
            return false;
        }

        fileName = QString((runType == ivis::common::RunTypeEnum::RunTypeTCReport) ? ("TCReport.info") : ("GCOVReport.info"));
        subPath = QString("validator");
        // ./gen_tcreport.sh -c pV, CV -s S -o C -t E    (-s : Split,  -o : Config,   -t : Excel)
        // ./gen_gcov_report.sh -c PV, CV -b ON -f ON    (-b : Branch, -f : Function, -n : Line )
        cmd = QString("./%1.sh -c %2")
                  .arg((runType == ivis::common::RunTypeEnum::RunTypeGcovReport) ? ("gen_gcov_report") : ("gen_tcreport"))
                  .arg(pvCvName);
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
    } else if (runType == ivis::common::RunTypeEnum::RunTypeGenSSFS) {
        QStringList log;
        ivis::common::ExcuteProgram process;
        process.start(QString("rm -rf %1/ssfs/include/generated").arg(sfcModelPath), log);
        process.start(QString("rm -rf %1/ssfs/src/generated").arg(sfcModelPath), log);
        totalCount = 10;
        cmd = QString("./gen_ssfs.sh %1").arg(pvCvName);
    } else {
        if (infoList.size() != 2) {
            qDebug() << "Fail to select info list size :" << infoList.size();
            return false;
        }

        totalCount = infoList[0].toList().size();
        if (totalCount == 0) {
            qDebug() << "Fail to select module list : 0";
            return false;
        }

        QString moduleInfo = QString();
        moduleList = infoList[0].toStringList();
        for (const auto& module : moduleList) {
            if (moduleInfo.size() > 0) {
                moduleInfo.append(" ");
            }
            moduleInfo.append(module);
        }

        QString checkList = QString();
        for (const auto& check : infoList[1].toList()) {
            checkList.append(QString("%1%2").arg((checkList.size() == 0) ? ("") : (" ")).arg(check.toString()));
        }

        if (runType == ivis::common::RunTypeEnum::RunTypeGenTC) {
            QString negative = (state) ? (QString(" -o Negative")) : (QString());
            cmd = QString("./gen_tc.sh -c %1 -m \"%2\"%3").arg(pvCvName).arg(moduleInfo).arg(negative);
            subPath = QString("tc_generator");
        } else if (runType == ivis::common::RunTypeEnum::RunTypeRunTC) {
            QString altonPath = (state) ? (QString("/usr/local/bin/altonservice"))
                                        : (QString("%1%2").arg(currentPWD).arg("/../Alton/altonservice"));
            QString docker = (state) ? (QString("-d ")) : (QString());
            QString ptList = (checkList.size() == 0) ? (QString()) : (QString(" %1").arg(checkList));
            cmd = QString("./run_tc.sh -b %1 -c %2 %3-g -m \"%4\"%5")
                      .arg(altonPath)
                      .arg(pvCvName)
                      .arg(docker)
                      .arg(moduleInfo)
                      .arg(ptList);
            subPath = QString("validator");
            int ptCount = infoList[1].toList().size();
            if (ptCount == 0) {
                QVariantList vehicleTypeList = QVariantList();
                if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
                    vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toList();
                } else {
                    vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toList();
                }
                ptCount = vehicleTypeList.size();
            }

            totalCount = totalCount * ((ptCount == 0) ? (3) : (ptCount));  // No Select PT -> All(EV, FCEV, ICV) = 3

            checkBinary.append(altonPath);
            checkBinary.append("/build/sfc_validator");
        } else {
            qDebug() << "Fail to excute script - runType :" << runType;
            return false;
        }
    }

    QString runScriptPath = QString("%1/../%2").arg(sfcModelPath).arg(subPath);
    if (runType == ivis::common::RunTypeEnum::RunTypeGenSSFS) {
        runScriptPath = QString("%1/..").arg(sfcModelPath);
    }
    QVariant popupData = QVariant();
    if (QDir::setCurrent(runScriptPath) == false) {
        qDebug() << "Fail to change folder :" << runScriptPath;
        ivis::common::Popup::drawPopup(ivis::common::PopupType::RunPathError, isHandler(), popupData,
                                       QVariantList({STRING_POPUP_DEFAULT_PATH_ERROR, STRING_POPUP_BINARY_NOT_EXISTS_TIP}));
        return false;
    }

    if (checkBinary.size() > 0) {
        int index = 0;
        for (const auto& binary : checkBinary) {
            QString checkFile = (index == 0) ? (binary) : (QString("%1%2").arg(runScriptPath).arg(binary));
            if (QFile::exists(checkFile) == false) {
                updateTestResultInfo(ivis::common::TestResultTypeEnum::TestResultTypeCheckError, totalCount,
                                     QStringList({STRING_BINARY_NOT_EXISTS_ERROR + binary}));
                qDebug() << "Fail to binary not exists :" << checkFile;
                return false;
            }
            index++;
        }
    }

    qDebug() << "PWD        :" << ivis::common::APP_PWD();
    qDebug() << "SFC Model  :" << sfcModelPath;
    qDebug() << "Run Script :" << runScriptPath;
    qDebug() << "Command    :" << cmd;

    QString runScriptFile = QString("%1/%2_%3").arg(currentPWD).arg(runType).arg("RunScript.log");
    QString testResultFile = QString("%1/%2").arg(runScriptPath).arg(fileName);

    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel, false);
    updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeProcessStartPath, testResultFile);
    if (runType != ivis::common::RunTypeEnum::RunTypeGenSSFS) {
        startWatcherFile(ivis::common::WatcherTypeEnum::WatcherTypeTestResult, testResultFile, totalCount);
    }
    startWatcherFile(ivis::common::WatcherTypeEnum::WatcherTypeRunScript, runScriptFile, totalCount);
    startProcess(cmd, runScriptFile, totalCount);

    return true;
}

void ControlMenu::stopWatcherFile(const int& type) {
    QString infoText = QString();
    if (type == ivis::common::WatcherTypeEnum::WatcherTypeRunScript) {
        if (mWatcherRunScript.isNull() == false) {
            qDebug() << "WatcherRunScript is running -> stop";
            disconnect(mWatcherRunScript.data());
            mWatcherRunScript.reset();
            updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfoPrevious, QVariant());
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfo, QVariant());
        }
    } else if (type == ivis::common::WatcherTypeEnum::WatcherTypeTestResult) {
        if (mWatcherTestResult.isNull() == false) {
            qDebug() << "WatcherTestResult is running -> stop";
            disconnect(mWatcherTestResult.data());
            mWatcherTestResult.reset();
        }
    } else {
        return;
    }
}

void ControlMenu::stopProcess() {
    if (mProcess.isNull() == false) {
        qDebug() << "Process is running -> stop";
        disconnect(mProcess.data());
        mProcess.reset();
    }
}

void ControlMenu::cancelScript(const bool& script, const bool& watcher) {
    if (script) {
        const QStringList killProcess = QStringList({
            "python",
            "python3",
            "gen_tc.sh",
            "run_tc.sh",
            "gen_ssfs.sh",
            "gen_tcreport.sh",
            "gen_gcov_report.sh",
            "sfc_validator",
            "altonservice",
        });
        for (const auto& info : killProcess) {
            QStringList log;
            ivis::common::ExcuteProgram process(false);
            bool result = process.start(QString("pkill -9 -ef %1").arg(info), log);
            qDebug() << "Terminate Process :" << info << ", Result :" << ((result) ? ("Success") : ("fail"));
        }
    }

    if (watcher) {
        stopWatcherFile(ivis::common::WatcherTypeEnum::WatcherTypeRunScript);
        stopWatcherFile(ivis::common::WatcherTypeEnum::WatcherTypeTestResult);
        stopProcess();
    }
}

int ControlMenu::saveTestReportInfo(const int& reportType, const QList<bool>& value) {
    int count = 0;
    int configType = (reportType == ivis::common::TestReportTypeEnum::TestReportTypeTC) ? (ConfigInfo::ConfigTypeReportResult)
                                                                                        : (ConfigInfo::ConfigTypeReportCoverage);
    for (const auto& configValue : value) {
        ConfigSetting::instance().data()->writeConfig((configType + count), configValue);
        count++;
    }
    return count;
}

void ControlMenu::slotControlUpdate(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::ControlUpdateTypeEnum::ControlUpdateTypeScriptRunnigCompleted: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, false);
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::ScriptRunnigCompleted, isHandler(), popupData,
                                           QVariantList({STRING_SCRIPT_RUNNIG_COMPLETED, STRING_SCRIPT_RUNNIG_COMPLETED_TIP}));
            break;
        }
        default: {
            break;
        }
    }
}

void ControlMenu::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ConfigInfo::ConfigTypeSfcModelPath: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath, value);
            break;
        }
        case ConfigInfo::ConfigTypeScreenInfo: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo,
                              ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect());
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

            bool runScriptState = getData(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();
            if (runScriptState) {
                cancelScript(true, true);
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
        case ivis::common::EventTypeEnum::EventTypeViewNodeAddress: {
            sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, type, value);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath:
        case ivis::common::EventTypeEnum::EventTypeSettingNodePath: {
            QString text = STRING_MODEL_PATH;
            int configType = ConfigInfo::ConfigTypeSfcModelPath;
            if (type == ivis::common::EventTypeEnum::EventTypeSettingNodePath) {
                text = STRING_NODE_PATH;
                configType = ConfigInfo::ConfigTypeNodeAddressPath;
            }
            QVariant path = ConfigSetting::instance().data()->readConfig(configType);
            QVariant popupData = QVariant();

            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, isHandler(), popupData,
                                               QVariantList({text, path})) == ivis::common::PopupButton::OK) {
                ConfigSetting::instance().data()->writeConfig(configType, popupData);
                // qDebug() << "SFC Model Path :" << ConfigSetting::instance().data()->readConfig(configType);
                sendEventInfo(ivis::common::ScreenEnum::DisplayTypeCenter, ivis::common::EventTypeEnum::EventTypeInitModule);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSettingAppMode: {
            updateSelectAppMode();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectAppMode: {
            int appModePrevious = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
            int appMode = value.toInt();
            if (appModePrevious != appMode) {
                qDebug() << "EventTypeSelectAppMode :" << appMode;
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAppMode, appMode);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeInit, true);
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
            if (testReportType == static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeGCOV)) {
                runType = ivis::common::RunTypeEnum::RunTypeGcovReport;
            }
            bool state = reportInfo.at(1).toBool();
            bool option1 = reportInfo.at(2).toBool();
            bool option2 = reportInfo.at(3).toBool();
            bool option3 = reportInfo.at(4).toBool();
            if (saveTestReportInfo(testReportType, QList({state, option1, option2, option3})) == 4) {
                bool runScriptState = excuteScript(runType, state, QVariantList({option1, option2, option3}));
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, runScriptState);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptText: {
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText, true, true);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeEnterScriptTextCompleted: {
            bool runScriptState = excuteScript(ivis::common::RunTypeEnum::RunTypeEnterScriptText, false, QVariantList({value}));
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, runScriptState);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewLogFile: {
            updateViewLogFile();
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeViewLogDisplay: {
            updateViewLogDisplay(value.toString());
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenSSFS: {
            bool runScriptState = excuteScript(ivis::common::RunTypeEnum::RunTypeGenSSFS, false, QVariantList());
            updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, runScriptState);
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun: {
            if (value.toList().size() == 4) {
                int runType = value.toList().at(0).toInt();
                bool state = value.toList().at(1).toBool();
                QVariantList selectInfoList = QVariantList({value.toList().at(2), value.toList().at(3)});
                updateSelectModueList(0, selectInfoList);
                bool runScriptState = excuteScript(runType, state, selectInfoList);
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, runScriptState);
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenRunTCCancel: {
            bool runScriptState = getData(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();
            if (runScriptState) {
                updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, false);
                updateDataControl(ivis::common::PropertyTypeEnum::PropertyTypeTestResultCancel, true);
                cancelScript(true, false);
            } else {
                qDebug() << "Skip Cancel : Script is not running !!!!!!";
            }
            break;
        }
        case ivis::common::EventTypeEnum::EventTypeGenRunTCCompleted: {
            bool runScriptState = getData(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();
            if (runScriptState == false) {
                cancelScript(false, true);
            }
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
        case ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath: {
            slotHandlerEvent(eventType, eventValue);
            break;
        }
        default: {
            break;
        }
    }
}
