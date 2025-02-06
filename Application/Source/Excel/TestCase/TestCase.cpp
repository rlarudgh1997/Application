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

void TestCase::start(const QStringList& data) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int excuteType = ExcuteTypeStart;
    setExcuteData(data);

    while (true) {
        int nextType = excuteTestCase(excuteType);

        if ((nextType == ExcuteTypeCompleted) || (nextType == ExcuteTypeFailed)) {
            emit signalTestCaseCompleted(getExcuteType(), (nextType != ExcuteTypeFailed));

            // if (graphicsMode == false) {
            //     emit ControlManager::instance().data()->signalExitProgram();
            // }
            break;
        }
        excuteType = nextType;
    }
}

int TestCase::excuteTestCase(const int& excuteType) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    int nextType = ExcuteTypeFailed;

    setExcuteType(excuteType);

    switch (excuteType) {
        case ExcuteTypeStart: {
            nextType = (graphicsMode) ? (ExcuteTypeGenConvertData) : (ExcuteTypeParsingMode);
            break;
        }
        case ExcuteTypeParsingMode:
        case ExcuteTypeParsingModule: {
            if (inputArguments(excuteType)) {
                nextType = (excuteType == ExcuteTypeParsingMode) ? (ExcuteTypeParsingModule) : (ExcuteTypeExcelOpen);
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

    qDebug() << (QString(120, '*'));
    qDebug() << "\t excuteTestCase :" << excuteType << "->" << nextType;
    qDebug() << (QString(120, '-'));
    qDebug() << "\n\n\n\n";

    return nextType;
}

bool TestCase::inputArguments(const int& excuteType) {
    QStringList arguments = getExcuteData();
    QStringList selectedItems;
    QStringList itemList("Exit");

    if (excuteType == ExcuteTypeParsingMode) {
        QStringList appMode({"CV", "PV"});
        QString selectMode;
        for (const auto& mode : appMode) {
            if ((arguments.indexOf(mode) >= 0) && (selectMode.size() == 0)) {
                selectMode = mode;
            }
            arguments.removeAll(mode);
        }
        if (selectMode.size() == 0) {
            itemList.append(appMode);
            selectedItems = selectMultipleOptionsWithNumbers(excuteType, itemList);
            selectMode = (selectedItems.size() == 1) ? (selectedItems.at(0)) : ("CV");
        } else {
            selectedItems.append(selectMode);
        }
        setSelectAppMode(selectMode);
    } else if (excuteType == ExcuteTypeParsingModule) {
        itemList.append(isModuleList());

        for (const auto& arg : arguments) {
            for (const auto& item : itemList) {
                if (arg.compare(item, Qt::CaseInsensitive) == 0) {    // 대소문자 구분 없이 비교
                    selectedItems.append(item);
                    break;
                }
            }
        }
        if (selectedItems.size() == 0) {
            selectedItems = selectMultipleOptionsWithNumbers(excuteType, itemList);
        }
        setSelectModules(selectedItems);
    } else {
        return false;
    }
    setExcuteData(arguments);

    bool result = false;
    bool exitProgram = ((selectedItems.size() == 1) && (selectedItems.at(0) == "0"));
    if (exitProgram) {
        emit ControlManager::instance().data()->signalExitProgram();
    } else {
        result = true;
        QTextStream output(stdout);
        output << "\nSelected Items : " << selectedItems.join(", ") << "\n\n" << Qt::endl;
    }

    // ./gen_tc.sh -c [CV/PV/CONN] -m ["ABS_CV AEM ...."]
    // ../deploy_x86/Application gen cv abs_cv aem

    return result;
}

void TestCase::drawTerminalMenu(const int& excuteType, const QStringList& itemList, const bool& clear) {
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
        if (excuteType == ExcuteTypeParsingMode) {
            subTips = false;
            displayText.append("\033[32m[Select App Mode]\n\033[0m\n");
        } else if (excuteType == ExcuteTypeParsingModule) {
            displayText.append(QString("\033[32m[Select Module : %1]\n\033[0m\n").arg(getSelectAppMode()));
        } else {
            return;
        }

        if (clear) {
            // system("clear");
        }

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

    QStringList selecteItems;
    bool inputState = true;
    bool manualInput = false;

    drawTerminalMenu(excuteType, itemList);

    while (inputState) {
        QString inputLine = input.readLine().trimmed();
        QStringList choices({inputLine});

        if ((excuteType == ExcuteTypeParsingModule) || (manualInput)) {
            choices = inputLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        }

        if (manualInput) {
            qDebug() << "Manual Input :" << choices;
            selecteItems.append(choices);
        } else {
            for (const QString& choice : choices) {
                bool valid;
                int selectIndex = choice.toInt(&valid);

                if (valid) {
                    if (selectIndex == mNumExit) {
                        selecteItems.append(choice);
                    } else if (selectIndex == mNumSelectAll) {
                        for (int index = 1; index < itemList.size(); ++index) {
                            selecteItems.append(itemList[index]);
                        }
                    } else if (selectIndex == mNumManualInput) {
                        manualInput = true;
                    } else if ((selectIndex > 0) && (selectIndex < itemList.size())) {
                        selecteItems.append(itemList[selectIndex]);
                    } else {
                    }
                }
            }
        }

        if (selecteItems.size() == 0) {
            drawTerminalMenu((manualInput) ? (ExcuteTypeManualInput) : (ExcuteTypeInvalidSelectItem), itemList);
        } else {
            selecteItems.sort();
            selecteItems.removeDuplicates();
            inputState = false;
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
    int appMode = ((getSelectAppMode() == QString("CV")) ? (ivis::common::AppModeEnum::AppModeTypeCV)
                                                         : (ivis::common::AppModeEnum::AppModeTypePV));
    QMap<QString, QString> moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, false);
    QStringList moduleList = moduleInfo.keys();
    writeModuleList(moduleInfo);
    return moduleList;
}

bool TestCase::openExcelFile() {
    QStringList selectModules = getSelectModules();
    QString currModule = (selectModules.size() > 0) ? (selectModules.at(0)) : ("");
    if (currModule.size() == 0) {
        qDebug() << "Fail to select module size : 0";
        return false;
    }

    QString filePath = getModuleList(currModule);
    QList<QVariantList> sheetDataList = ExcelUtil::instance().data()->openExcelFile(filePath);
    bool result = (sheetDataList.size() > 0);
    if (result) {
        int propertyType = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
        for (const auto& sheetData : sheetDataList) {
            ExcelData::instance().data()->setSheetData(propertyType++, sheetData);
        }

        selectModules.removeAll(currModule);
        setSelectModules(selectModules);
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeTCFilePath, filePath);
    }

    return result;
}
