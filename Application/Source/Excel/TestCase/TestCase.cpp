#include "TestCase.h"

#include "ConfigSetting.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ConvertDataManager.h"
#include "GenerateCaseData.h"

#include "ControlManager.h"

QSharedPointer<TestCase>& TestCase::instance() {
    static QSharedPointer<TestCase> gInstance;
    if (gInstance.isNull()) {
        gInstance = QSharedPointer<TestCase>(new TestCase());
    }
    return gInstance;
}

TestCase::TestCase() {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    if (graphicsMode == false) {
        terminateApplicaton();
    }

#if defined (USE_TEST_CASE_THREAD)
    mThread = QSharedPointer<QThread>::create();
    moveToThread(mThread.data());
    connect(mThread.data(), &QThread::finished, this, &QObject::deleteLater);
    connect(mThread.data(), &QThread::started, this, &TestCase::runThread);
    // controlThread(mThread.data(), mWaitCondition, mMutex, static_cast<int>(ivis::common::ThreadEnum::ControlType::Start));
#endif
}

TestCase::~TestCase() {
    stop(true);

#if defined (USE_TEST_CASE_THREAD)
    qDebug() << "TestCase::terminateThread 1";
    if (mThread) {
        setThreadRunState(false);
        controlThread(mThread.data(), mWaitCondition, mMutex, static_cast<int>(ivis::common::ThreadEnum::ControlType::Terminate));
        // if (mThread->isRunning()) {
        //     mThread->quit();
        //     mThread->wait();
        // }
        mThread.clear();
    }
    qDebug() << "TestCase::terminateThread 2";
#endif
}

bool TestCase::start(const QStringList& arguments, const QFileInfo& fileInfo) {
    if (parsingOptions(arguments)) {
        system("clear");
        drawTerminalMenu(ExcuteTypeHelpMode, QStringList());
        return false;
    }
    setOpenFile(fileInfo);
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeGenerateStart, true);

#if defined (USE_TEST_CASE_THREAD)
    controlThread(mThread.data(), mWaitCondition, mMutex, static_cast<int>(ivis::common::ThreadEnum::ControlType::Resume));
#else
    int excuteType = ExcuteTypeStart;

    while (true) {
        int nextType = excuteTestCase(excuteType);
        excuteType = nextType;
        if ((nextType == ExcuteTypeCompleted) || (nextType == ExcuteTypeFailed)) {
            emit signalTestCaseCompleted(getExcuteType(), (nextType != ExcuteTypeFailed));
            break;
        } else if (nextType == ExcuteTypeExit) {
            emit ControlManager::instance().data()->signalExitProgram();
            break;
        }
    }
#endif

    return true;
}

void TestCase::stop(const bool& appExit) {
    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeGenerateStart).toBool() == false) {
        return;
    } else {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeGenerateStart, false);
    }
    qDebug() << "TestCase::stop()";

    if (appExit) {
        setThreadRunState(false);
        return;
    }

    excuteTestCase(ExcuteTypeStop);

    QStringList processList = {
        QString("%1/CaseGen/tests/run.py").arg(ivis::common::APP_PWD()),
        // QString("python -O %1/CaseGen/tests/run.py").arg(ivis::common::APP_PWD()),
        "run.py",
        "python",
        "python3",
    };

    QString scriptPath = processList.at(0);
    QProcess process;
    process.start("pgrep", QStringList() << "-f" << scriptPath);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList pidList = output.split("\n", Qt::SkipEmptyParts);

    for (const QString& pid : pidList) {
        qDebug() << "\t Running Process :" << pid;
        QProcess::execute("kill", QStringList() << "-9" << pid);
    }
}

#if defined (USE_TEST_CASE_THREAD)
void TestCase::runThread() {
    // const bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int excuteType = ExcuteTypeStart;

    while (getThreadRunState()) {
        if (excuteType == ExcuteTypeInvalid) {
            qDebug() << "TestCase thread is waiting :" << excuteType;
            controlThread(mThread.data(), mWaitCondition, mMutex, static_cast<int>(ivis::common::ThreadEnum::ControlType::Wait));
            excuteType = ExcuteTypeStart;
        }

        int nextType = excuteTestCase(excuteType);

        switch (nextType) {
            case ExcuteTypeFailed:
            case ExcuteTypeStop:
            case ExcuteTypeCompleted: {
                excuteType = ExcuteTypeInvalid;
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfitTypeGenerateStart, false);
                emit signalTestCaseCompleted(nextType, (nextType == ExcuteTypeCompleted));
                break;
            }
            case ExcuteTypeExit: {
                excuteType = ExcuteTypeExit;
                setThreadRunState(false);
                emit ControlManager::instance().data()->signalExitProgram();
                break;
            }
            default: {
                excuteType = nextType;
                break;
            }
        }
    }
}

void TestCase::controlThread(QThread* thread, QWaitCondition& waitCondition, QMutex& mutex, const int& type) {
    if (thread == nullptr) {
        return;
    }

    QMutexLocker lock(&mutex);
    switch (type) {
        case static_cast<int>(ivis::common::ThreadEnum::ControlType::Terminate): {
            if (thread->isRunning()) {
                thread->quit();
                // if (QThread::currentThread() != thread) {
                    thread->wait();
                // }
            }
            break;
        }
        case static_cast<int>(ivis::common::ThreadEnum::ControlType::Start):
        case static_cast<int>(ivis::common::ThreadEnum::ControlType::Resume): {
            if (thread->isRunning() == false) {
                thread->start();
            }
            if (type == static_cast<int>(ivis::common::ThreadEnum::ControlType::Resume)) {
                waitCondition.wakeOne();
            }
            break;
        }
        case static_cast<int>(ivis::common::ThreadEnum::ControlType::Wait): {
            // if ((thread->isRunning()) && (QThread::currentThread() != thread)) {
            if (thread->isRunning()) {
                waitCondition.wait(&mutex);
            }
            break;
        }
        default: {
            break;
        }
    }
}
#endif

int TestCase::excuteTestCase(const int& excuteType) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int nextType = ExcuteTypeFailed;
    QString text;

    setExcuteType(excuteType);

    switch (excuteType) {
        case ExcuteTypeStart: {
            nextType = ExcuteTypeParsingAppMode;
            break;
        }
        case ExcuteTypeParsingAppMode:
        case ExcuteTypeParsingModule: {
            if (parsingInputArguments(excuteType)) {
                nextType = (excuteType == ExcuteTypeParsingAppMode) ? (ExcuteTypeParsingModule) : (ExcuteTypeExcelOpen);
                if (excuteType == ExcuteTypeParsingAppMode) {
                    nextType = ExcuteTypeParsingModule;
                } else {
                    nextType = ExcuteTypeExcelOpen;
                }
            } else {
                nextType = ExcuteTypeExit;
            }
            break;
        }
        case ExcuteTypeExcelOpen: {
            if (openExcelFile()) {
                nextType = ExcuteTypeGenConvertData;
            }
            break;
        }
        case ExcuteTypeGenConvertData: {
            text = ConvertDataManager::instance().data()->excuteConvertDataManager();
            if (text.size() == 0) {
                nextType = ExcuteTypeGenTC;
            }
            break;
        }
        case ExcuteTypeGenTC: {
            text = GenerateCaseData::instance().data()->excuteGenerateCaseData();
            nextType = ExcuteTypeGenTCComplted;
            break;
        }
        case ExcuteTypeGenTCComplted: {
            if (getRemainingModules().size() == 0) {
                nextType = (graphicsMode) ? (ExcuteTypeCompleted) : (ExcuteTypeParsingModule);
            } else {
                nextType = ExcuteTypeExcelOpen;
            }
            break;
        }
        case ExcuteTypeRunTC: {
            break;
        }
        case ExcuteTypeStop: {
            updateTestCaseExcuteInfo(ExcuteTypeStop, QString());
            nextType = ExcuteTypeStop;
            break;
        }
        default: {
            break;
        }
    }

    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeGenerateStart).toBool()) {
        updateTestCaseExcuteInfo(nextType, text);
    }

    // qDebug() << (QString(120, '='));
    qDebug() << "ExcuteTestCase :" << excuteType << "->" << nextType;
    // qDebug() << (QString(120, '='));

    return nextType;
}

void TestCase::updateTestCaseExcuteInfo(const int& excuteType, const QString& text) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    if (graphicsMode == false) {
        return;
    }

    QStringList totalModules = getSelectModules();
    QStringList remainingModules = getRemainingModules();
    QString currentModule = (remainingModules.size() > 0) ? (remainingModules.at(0)) : ("");

    int totalCount = totalModules.size();
    int remainingCount = remainingModules.size();
    int currentCount = totalCount - remainingCount;

    QStringList genTCInfo = getGenTCInfo();
    int testResultType = ivis::common::TestResultTypeEnum::TestResultTypeUpdate;

    switch (excuteType) {
        case ExcuteTypeParsingAppMode: {
            testResultType = ivis::common::TestResultTypeEnum::TestResultTypeStart;
            setGenTCResult(true);
            break;
        }
        case ExcuteTypeExcelOpen: {
            genTCInfo.append(QString("[%1]").arg(currentModule));
            genTCInfo.append(QString("        - Opening Excel file."));
            break;
        }
        case ExcuteTypeGenConvertData: {
            currentCount--;
            genTCInfo.append(QString("        - Converting Excel data."));
            break;
        }
        case ExcuteTypeGenTC: {
            currentCount--;
            genTCInfo.append(QString("        - Generating TC file."));
            break;
        }
        case ExcuteTypeGenTCComplted: {
            if (text.size() == 0) {
                setGenTCResult(false);
            }
            currentCount--;
            genTCInfo.append(QString("        - File : %1").arg(text));
            genTCInfo.append(QString("        - Result : %1").arg(((text.size() == 0)) ? ("FAIL") : ("PASS")));
            genTCInfo.append(QString(170, '-'));
            break;
        }
        case ExcuteTypeCompleted: {
            genTCInfo.append(QString("TC generation complete."));
            genTCInfo.append(QString("COMPLETE : %1").arg((getGenTCResult()) ? ("PASS") : ("FAIL")));
            break;
        }
        case ExcuteTypeStop: {
            currentCount--;
            testResultType = ivis::common::TestResultTypeEnum::TestResultTypeCancel;
            break;
        }
        case ExcuteTypeFailed: {
            currentCount--;
            genTCInfo.append(QString("ERROR_INFO : %1").arg(text));
            genTCInfo.append(QString("COMPLETE : FAIL"));
            break;
        }
        default: {
            testResultType = ivis::common::TestResultTypeEnum::TestResultTypeInvalid;
            break;
        }
    }

    if (testResultType != ivis::common::TestResultTypeEnum::TestResultTypeInvalid) {
        // qDebug() << "\n\n\n";
        // qDebug() << (QString(120, '*'));
        // qDebug() << "updateTestCaseExcuteInfo :" << excuteType << currentCount << "/" << totalCount;

        emit signalGenTCInfo(testResultType, currentCount, totalCount, genTCInfo);

        if ((excuteType == ExcuteTypeCompleted) || (excuteType == ExcuteTypeStop) || (excuteType == ExcuteTypeFailed)) {
            genTCInfo.clear();
        }
        setGenTCInfo(genTCInfo);
    }
}

bool TestCase::parsingOptions(const QStringList& arguments) {
    const QString helpInfo("HELP");
    const QString tcCheckInfo("ALL");
    const QMap<QString, int> genTypeInfo = QMap<QString, int>({
        {"DEFAULT", ivis::common::GenTypeEnum::GenTypeDefault},
        {"NEGATIVE", ivis::common::GenTypeEnum::GenTypeNegativePositive},
        {"POSITIVE", ivis::common::GenTypeEnum::GenTypePositive},
    });

    // ./gen_tc.sh -c [CV/PV/CONN] -m ["ABS_CV AEM ...."]
    // ./Applicaton gen [cv/pv] all [default/negative/positive] [module1 moduel2 .....]

    QStringList currArguments = arguments;

    // CLI Mode : help
    if (currArguments.indexOf(helpInfo) >= 0) {
        return true;
    }

    // CLI Mode : all
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCLIModeTCCheck, false);
    if (currArguments.indexOf(tcCheckInfo) >= 0) {
        currArguments.removeAll(tcCheckInfo);
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCLIModeTCCheck, true);
    }

    // CLI Mode : default/negative/positive
    int genType = ivis::common::GenTypeEnum::GenTypeInvalid;
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCLIModeGenType, genType);
    for (const auto& key : genTypeInfo.keys()) {
        if (currArguments.indexOf(key) >= 0) {
            currArguments.removeAll(key);
            if (genType == ivis::common::GenTypeEnum::GenTypeInvalid) {
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCLIModeGenType, genTypeInfo[key]);
            }
        }
    }
    qDebug() << "Arguments :" << arguments << "->" << currArguments;

    setArguments(currArguments);

    return false;
}

QStringList TestCase::parsingAppMode(const QStringList& arguments) {
    QStringList currArguments = arguments;
    QStringList itemList(mStrExit);
    QStringList appModeInfo({"CV", "PV"});
    QString selectMode;

    for (const auto& mode : appModeInfo) {
        if ((currArguments.indexOf(mode) >= 0) && (selectMode.size() == 0)) {
            selectMode = mode;
        }
        currArguments.removeAll(mode);
    }
    if (selectMode.size() == 0) {
        itemList.append(appModeInfo);
        QStringList selectedItems = selectMultipleOptionsWithNumbers(ExcuteTypeParsingAppMode, itemList);
        if (selectedItems.size() > 0) {
            selectMode = selectedItems.at(0);
        }
    }

    if (selectMode.size() > 0) {
        int appMode = (selectMode == appModeInfo.at(1)) ? (ivis::common::AppModeEnum::AppModeTypePV)
                                                        : (ivis::common::AppModeEnum::AppModeTypeCV);
        setSelectAppMode(appMode);
        setArguments(currArguments);
    }

    return QStringList(selectMode);
}

QStringList TestCase::parsingModules(const QStringList& arguments) {
    const bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    QStringList itemList(mStrExit);
    QStringList selectedItems;

    itemList.append(isModuleList());

    if (graphicsMode) {
        selectedItems = arguments;
    } else {
        for (const auto& arg : arguments) {
            for (const auto& item : itemList) {
                if (arg.compare(item, Qt::CaseInsensitive) == 0) {    // 대소문자 구분 없이 비교
                    selectedItems.append(item);
                    break;
                }
            }
        }

        if (selectedItems.size() == 0) {
            QStringList tempSelItems;
            selectedItems = selectMultipleOptionsWithNumbers(ExcuteTypeParsingModule, itemList);
            for (const auto& item : selectedItems) {
                if (itemList.indexOf(item) >= 0) {
                    tempSelItems.append(item);
                }
            }
            selectedItems = tempSelItems;
        }
    }
    qDebug() << "parsingModules :" << selectedItems;

    setSelectModules(selectedItems);
    setRemainingModules(selectedItems);
    setArguments(QStringList());    // setArguments(arguments);  [Application gen cv abs_cv ams -> gen tc 완료후 계속 반복됨]

    return selectedItems;
}

bool TestCase::parsingInputArguments(const int& excuteType) {
    QStringList selectedItems;

    if (excuteType == ExcuteTypeParsingAppMode) {
        selectedItems = parsingAppMode(getArguments());
    } else if (excuteType == ExcuteTypeParsingModule) {
        selectedItems = parsingModules(getArguments());
    } else {
    }

    bool result = (selectedItems.size() > 0);
    if (selectedItems.indexOf(mStrExit) >= 0) {
        result = false;
    }
    qDebug() << "parsingInputArguments :" << excuteType << result << selectedItems;

    return result;
}

void TestCase::drawTerminalMenu(const int& excuteType, const QStringList& itemList) {
    // 고정 폭 정의 (줄당 4개 표시, 최대폭 140 기준으로 정렬)
    const int itemsPerLine = 4;
    const int lineCount = 140;
    const int fixedWidth = ((lineCount - 10) * (1.0 / itemsPerLine));

    bool subTips = true;
    QString displayText;

    if (excuteType == ExcuteTypeHelpMode) {
        displayText.append(QString("\n") + QString(lineCount, '=') + QString("\n"));
        displayText.append(QString("Usage: \033[32mApplication gen [AppMode] all [GenType] [Modules]\033[0m\n"));
        displayText.append(QString("Options:\n"));
        displayText.append(QString("    gen                CLI Mode Start\n"));
        displayText.append(QString("    [AppMode]          App mode(cv, pv) selection\n"));
        displayText.append(QString("    all                Select all TC names\n"));
        displayText.append(QString("    [GenType]          TC gen type(default/negative/positive) Specify all and create\n"));
        displayText.append(QString("    [Modules]          Enter multiple modules(abs_cv aem ...)\n"));
        displayText.append(QString("\n\n") + QString(lineCount, '=') + QString("\n\n\n"));
    } else if (excuteType == ExcuteTypeInvalidSelectItem) {
        displayText.append(QString("\n\033[31m[Input Invalid]\033[0m\n"));
        displayText.append(QString("Please enter valid numbers between : 1 ~ %1\n").arg(itemList.size() - 1));
        displayText.append(QString("Enter the numbers of your choices separated by spaces : "));
    } else if (excuteType == ExcuteTypeManualInput) {
        displayText.append(QString("Please enter manual module name : "));
    } else {
        displayText.append(QString(lineCount, '*') + QString("\n"));
        if (excuteType == ExcuteTypeParsingAppMode) {
            subTips = false;
            displayText.append("\033[32m[Select App Mode]\n\033[0m\n");
        } else if (excuteType == ExcuteTypeParsingModule) {
            QString appMode = (getSelectAppMode() == ivis::common::AppModeEnum::AppModeTypePV) ? ("PV") : ("CV");
            displayText.append(QString("\033[32m[Select Module : %1]\n\033[0m\n").arg(appMode));
        } else {
            return;
        }

        // system("clear");

        for (int index = 1; index < itemList.size(); ++index) {                         // itemList.at(0) = Exit
            displayText.append(QString("%1. %2").arg(index, 3)                          // 번호를 3자리 폭으로 맞춤
                                                .arg(itemList[index], -fixedWidth));    // 옵션 이름을 왼쪽 정렬, 고정 폭 설정

            if (((index % itemsPerLine) == 0) || (index == (itemList.size() - 1))) {
                displayText.append("\n");
            }
        }
        displayText.append(QString(lineCount, '-') + QString("\n"));

        if (excuteType == ExcuteTypeParsingModule) {
            displayText.append(
                QString("\033[33m%1\033[0m. \033[33m%2\033[0m\n").arg(mNumSelectAll, 3).arg("Select All", -fixedWidth));
            displayText.append(
                QString("\033[33m%1\033[0m. \033[33m%2\033[0m\n").arg(mNumManualInput, 3).arg("Manual Input", -fixedWidth));
        }
        displayText.append(QString("\033[31m%1\033[0m. \033[31m%2\033[0m\n").arg(mNumExit, 3).arg(itemList.at(0), -fixedWidth));

        displayText.append(QString(lineCount, '*') + QString("\n\n\n"));
        displayText.append(QString("Enter the numbers of your choices %1: ").arg((subTips) ? ("separated by spaces ") : ("")));
    }

    QTextStream output(stdout);
    output << displayText << Qt::flush;
}

QStringList TestCase::selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& itemList) {
    QTextStream input(stdin);

    int currExcuteType = excuteType;
    QStringList selecteItems;
    drawTerminalMenu(excuteType, itemList);

    while (true) {
        QString inputLine = input.readLine().trimmed();
        QStringList inputList({inputLine});

        if ((currExcuteType == ExcuteTypeParsingModule) || (currExcuteType == ExcuteTypeManualInput)) {
            inputList = inputLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        } else {
            currExcuteType = ExcuteTypeInvalidSelectItem;
        }

        for (const QString& input : inputList) {
            if (currExcuteType == ExcuteTypeManualInput) {
                selecteItems.append(input.toUpper());
            } else {
                bool valid = false;
                int selectIndex = input.toInt(&valid);

                if (valid) {
                    if (selectIndex == mNumExit) {
                        selecteItems.append(mStrExit);
                    } else if (selectIndex == mNumSelectAll) {
                        for (int index = 1; index < itemList.size(); ++index) {
                            selecteItems.append(itemList[index]);
                        }
                    } else if (selectIndex == mNumManualInput) {
                        currExcuteType = ExcuteTypeManualInput;
                    } else if ((selectIndex > 0) && (selectIndex < itemList.size())) {
                        selecteItems.append(itemList[selectIndex]);
                    } else {
                    }
                }
            }
        }

        if (selecteItems.size() == 0) {
            drawTerminalMenu(currExcuteType, itemList);
        } else {
            selecteItems.sort();
            selecteItems.removeDuplicates();
            // qDebug() << "Selected Items :" << selecteItems;
            break;
        }
    }

    return selecteItems;
}

void TestCase::terminateApplicaton() {
    QString appFilePath = QCoreApplication::applicationFilePath();
    QString appName = QFileInfo(appFilePath).fileName();

    QProcess process;
    process.start("pgrep", QStringList() << "-x" << appName);
    process.waitForFinished();

    QString outputInfo = process.readAllStandardOutput();
    QStringList pidList = outputInfo.split("\n", Qt::SkipEmptyParts);
    qint64 currentPid = QCoreApplication::applicationPid();

    qDebug() << "Terminate Applicaton :" << appName << appFilePath;
    qDebug() << "\t PID :" << currentPid << pidList;

    for (const QString& pid : pidList) {
        if (pid.toLongLong() != currentPid) {
            qDebug() << "\t Terminate PID :" << pid;
            QProcess::execute("kill", QStringList() << "-9" << pid);
        }
    }
}

QStringList TestCase::isModuleList() {
    auto moduleInfoList = ExcelUtil::instance().data()->isModuleListFromJson(getSelectAppMode());
    QStringList moduleList = moduleInfoList.keys();
    QMap<QString, QString> moduleInfo;

    for (const auto& key : moduleInfoList.keys()) {
        // qDebug() << "ModuleList :" << getModuleList(key);
        moduleInfo[key] = moduleInfoList[key].first;    // Excel File
    }
    writeModuleList(moduleInfo);

    return moduleList;
}

QList<QVariantList> TestCase::readSheetData(const bool& editingModule) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QList<QVariantList> sheetDataList;
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        QVariantList sheetData;
        if (editingModule) {
            sheetData = ExcelData::instance().data()->getEditingSheetData(sheetIndex).toList();
        } else {
            sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        }
        if ((sheetIndex == startIndex) && (sheetData.size() > 0)) {
            qDebug() << "readSheetData :" << editingModule << sheetData.at(0).toList();
        }
        sheetDataList.append(sheetData);
    }
    return sheetDataList;
}

void TestCase::writeSheetData(const QList<QVariantList>& sheetDataList) {
    const int startOriginIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int startConvertIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    int originIndex = startOriginIndex;

    for (const auto& sheetData : sheetDataList) {
        int convertIndex = originIndex - startOriginIndex + startConvertIndex;
        // qDebug() << "writeSheetData :" << originIndex << convertIndex << sheetData.size();
        ExcelData::instance().data()->setSheetData(originIndex, sheetData);
        ExcelData::instance().data()->setSheetData(convertIndex, sheetData);
        originIndex++;
    }
}

bool TestCase::openExcelFile() {
    QStringList remainingModules = getRemainingModules();
    QString currentModule = (remainingModules.size() > 0) ? (remainingModules.at(0)) : ("");

    if (currentModule.size() == 0) {
        qDebug() << "Fail to select module size 0 :" << remainingModules;
        return false;
    }

    QString newModule = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeNewModule).toString();
    QList<QVariantList> sheetDataList;
    QString filePath;
    bool openState = false;
    QFileInfo openFileInfo = getOpenFile();

    if (currentModule == newModule) {
        filePath = ivis::common::APP_PWD() + "/" + currentModule + ".xlsx";    // 파일 저장 하지 않은 경우 임시 엑셀 파일 지정
        sheetDataList = readSheetData();
    } else {
        auto moduleList = readModuleList();
        for (const auto& module : moduleList.keys()) {
            if (currentModule == module) {
                filePath = getModuleList(module);
                break;
            }
        }
        if (filePath.size() == 0) {
            filePath = currentModule;    // ./Temp/test.xlsx 인 경우 TestCast Start 시 파일 경로 넘겨주는거 그대로 사용
        }

        QString openFilePath = openFileInfo.filePath();
        if ((openFilePath.size() > 0) && (filePath != openFilePath)) {
            // 엑셀 파일 오픈한 경로와 cv.json 파일에서 모듈정보, 파일경로 정보로 구성한 파일경로가 다른경우
            // -> 해당 조건인 경우 파일 오픈한 경로를 사용하도록
            filePath = openFilePath;
        }

        if (currentModule == getEditingModule()) {    // 엑셀 파일 열어서 편집중인 상태에서 GenTC 실행시
            setEditingModule("");
            sheetDataList = readSheetData(true);
        } else {
            openState = true;
            sheetDataList = ExcelUtil::instance().data()->openExcelFile(filePath);
        }
    }

    bool result = (sheetDataList.size() > 0);

    qDebug() << "\n\033[32m";
    int currStep = (getSelectModules().size() - getRemainingModules().size() + 1);
    qDebug() << (QString(120, '>').toLatin1().data());
    qDebug() << (QString("[Test Case - Excel Open] : %1/%2").arg(currStep).arg(getSelectModules().size()).toLatin1().data());
    qDebug() << "\t Selected Modules   :" << getSelectModules();
    qDebug() << "\t Remaining Modules  :" << getRemainingModules();
    qDebug() << "\t Gen TC Module      :" << currentModule.toLatin1().data();
    if (openState) {
        qDebug() << "\t File Open          :" << filePath.toLatin1().data();
        qDebug() << "\t                    :" << openFileInfo.filePath().toLatin1().data();
        qDebug() << "\t                    :" << openFileInfo.path().toLatin1().data();
        qDebug() << "\t                    :" << openFileInfo.fileName().toLatin1().data();
    } else {
        qDebug() << "\t Read Sheet         :" << currentModule;
    }
    qDebug() << "\t Result             :" << result;
    qDebug() << "\t Save TC File       :" << filePath.toLatin1().data();
    qDebug() << (QString(120, '<').toLatin1().data());
    qDebug() << "\n\033[0m";

    if (result) {
        writeSheetData(sheetDataList);
        remainingModules.removeAll(currentModule);
        setRemainingModules(remainingModules);
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeTCFilePath, filePath);
    }

    return result;
}
