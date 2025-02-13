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
}

void TestCase::start(const QStringList& arguments) {
    const bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int excuteType = ExcuteTypeStart;

    parsingOptions(arguments);

    while (true) {
        int nextType = excuteTestCase(excuteType);

        excuteType = nextType;

        if ((nextType == ExcuteTypeCompleted) || (nextType == ExcuteTypeFailed)) {
            emit signalTestCaseCompleted(getExcuteType(), (nextType != ExcuteTypeFailed));

            // if (graphicsMode == false) {
            //     emit ControlManager::instance().data()->signalExitProgram();
            // }
            break;
        }
    }
}

int TestCase::excuteTestCase(const int& excuteType) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int nextType = ExcuteTypeFailed;

    setExcuteType(excuteType);

    switch (excuteType) {
        case ExcuteTypeStart: {
            // nextType = (graphicsMode) ? (ExcuteTypeParsingModule) : (ExcuteTypeParsingAppMode);
            nextType = ExcuteTypeParsingAppMode;
            break;
        }
        case ExcuteTypeParsingAppMode:
        case ExcuteTypeParsingModule: {
            if (parsingInputArguments(excuteType)) {
                nextType = (excuteType == ExcuteTypeParsingAppMode) ? (ExcuteTypeParsingModule) : (ExcuteTypeExcelOpen);
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
            if (ConvertDataManager::instance().data()->excuteConvertDataManager()) {
                nextType = ExcuteTypeGenTC;
            }
            break;
        }
        case ExcuteTypeGenTC: {
            if (GenerateCaseData::instance().data()->excuteGenerateCaseData()) {
                QStringList selectModules = getSelectModules();
                if (selectModules.size() == 0) {
                    // nextType = ExcuteTypeCompleted;
                    nextType = (graphicsMode) ? (ExcuteTypeCompleted) : (ExcuteTypeParsingModule);
                } else {
                    nextType = ExcuteTypeExcelOpen;
                }
            }
            break;
        }
        case ExcuteTypeRunTC: {
            break;
        }
        default: {
            break;
        }
    }

    // qDebug() << (QString(120, '='));
    qDebug() << "ExcuteTestCase :" << excuteType << "->" << nextType;
    // qDebug() << (QString(120, '='));

    return nextType;
}

void TestCase::parsingOptions(const QStringList& arguments) {
    const QString tcCheckInfo("ALL");
    const QMap<QString, int> genTypeInfo = QMap<QString, int>({
        {"DEFAULT", ivis::common::GenTypeEnum::GenTypeDefault},
        {"NEGATIVE", ivis::common::GenTypeEnum::GenTypeNegativePositive},
        {"POSITIVE", ivis::common::GenTypeEnum::GenTypePositive},
    });

    // ./gen_tc.sh -c [CV/PV/CONN] -m ["ABS_CV AEM ...."]
    // ./Applicaton gen [cv/pv] all [default/negative/positive] [module1 moduel2 .....]

    QStringList currArguments = arguments;

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
        for (const auto& item : selectedItems) {
            if (appModeInfo.indexOf(item) >= 0) {
                selectMode = item;
                break;
            }
        }
    }

    int appMode = (selectMode == appModeInfo.at(1)) ? (ivis::common::AppModeEnum::AppModeTypePV)
                                                    : (ivis::common::AppModeEnum::AppModeTypeCV);
    setSelectAppMode(appMode);
    setArguments(currArguments);

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
    setSelectModules(selectedItems);
    setArguments(arguments);

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

    bool result = false;
    if (selectedItems.size() > 0) {
        if (selectedItems.indexOf(mStrExit) >= 0) {
            emit ControlManager::instance().data()->signalExitProgram();
        } else {
            result = true;
        }
    }

    // qDebug() << "Parsing Arg :" << result << getTCCheck() << getGenType() << getSelectAppMode();

    return result;
}

void TestCase::drawTerminalMenu(const int& excuteType, const QStringList& itemList) {
    // 고정 폭 정의 (줄당 4개 표시, 최대폭 140 기준으로 정렬)
    const int itemsPerLine = 4;
    const int lineCount = 140;
    const int fixedWidth = ((lineCount - 10) * (1.0 / itemsPerLine));

    bool subTips = true;
    QString displayText;

    if (excuteType == ExcuteTypeInvalidSelectItem) {
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

    for (const QString &pid : pidList) {
        if (pid.toLongLong() != currentPid) {
            qDebug() << "\t Terminate PID:" << pid;
            QProcess::execute("kill", QStringList() << "-9" << pid);
        }
    }
}

QStringList TestCase::isModuleList() {
    QMap<QString, QString> moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(getSelectAppMode(), false);
    QStringList moduleList = moduleInfo.keys();
    writeModuleList(moduleInfo);

    // for (const auto& key : moduleInfo.keys()) {
    //     qDebug() << "ModuleList :" << getModuleList(key);
    // }

    return moduleList;
}

QList<QVariantList> TestCase::isSheetData() {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    QList<QVariantList> sheetDataList;
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        QVariantList sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        sheetDataList.append(sheetData);
    }

    qDebug() << "isSheetData :" << sheetDataList.size();

    return sheetDataList;
}

void TestCase::updateSheetData(const QList<QVariantList>& sheetDataList) {
    qDebug() << "updateSheetData :" << sheetDataList.size();

    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    for (const auto& sheetData : sheetDataList) {
        ExcelData::instance().data()->setSheetData(sheetIndex++, sheetData);
    }
}

bool TestCase::openExcelFile() {
    QStringList selectModules = getSelectModules();
    QString currModule = (selectModules.size() > 0) ? (selectModules.at(0)) : ("");

    if (currModule.size() == 0) {
        qDebug() << "Fail to select module size : 0";
        return false;
    }

    const bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    const bool sheetEditState = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDoFileSave).toBool();
    QString filePath;

    QList<QVariantList> sheetDataList;

    if ((graphicsMode) || (sheetEditState)) {   // 임시코드 : 경로 이상 문제 수정
        sheetDataList = isSheetData();
        filePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath).toString();
    } else {
        sheetDataList = ExcelUtil::instance().data()->openExcelFile(filePath);
        filePath = getModuleList(currModule);
    }

    bool result = (sheetDataList.size() > 0);
    if (result) {
        updateSheetData(sheetDataList);
        selectModules.removeAll(currModule);
        setSelectModules(selectModules);
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeTCFilePath, filePath);
    }

    qDebug() << "openExcelFile :" << sheetEditState << result << currModule << filePath;

    return result;
}
