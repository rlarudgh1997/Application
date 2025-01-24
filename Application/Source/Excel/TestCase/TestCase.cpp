#include "TestCase.h"

#include "ConfigSetting.h"

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
}

void TestCase::start(const QStringList& data) {
    int excuteType = ExcuteTypeStart;
    setExcuteData(data);

    while (true) {
        int nextType = excuteTestCase(excuteType);

        if ((nextType == ExcuteTypeCompleted) || (nextType == ExcuteTypeFailed)) {
            emit signalTestCaseCompleted(getExcuteType(), (nextType != ExcuteTypeFailed));
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
            if (graphicsMode) {
                nextType = ExcuteTypeGenConvertData;
            } else {
                terminateApplicaton();
                nextType = ExcuteTypeParsingMode;
            }
            break;
        }
        case ExcuteTypeParsingMode:
        case ExcuteTypeParsingModule: {
            if (inputArguments(excuteType)) {
                nextType = (excuteType == ExcuteTypeParsingMode) ? (ExcuteTypeParsingModule) : (ExcuteTypeCompleted);
            }
            break;
        }
        case ExcuteTypeExcelOpen: {
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
                nextType = ExcuteTypeCompleted;
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
        setSelectMode(selectMode);
    } else if (excuteType == ExcuteTypeParsingModule) {
        itemList.append(ExcelUtil::instance().data()->isModuleListFromJson(true));
        for (const auto& item : arguments) {
            if (itemList.indexOf(item) >= 0) {
                selectedItems.append(item);
            }
        }
        if (selectedItems.size() == 0) {
            selectedItems = selectMultipleOptionsWithNumbers(excuteType, itemList);
        }
        setSelectMoudles(selectedItems);
    } else {
        return false;
    }

    bool result = false;
    bool exitProgram = ((selectedItems.size() == 1) && (selectedItems.at(0) == "0"));
    if (exitProgram) {
        emit ControlManager::instance().data()->signalExitProgram();
    } else {
        result = true;
        QTextStream output(stdout);
        output << "\nSelected Items : " << selectedItems.join(", ") << "\n\n" << Qt::endl;
    }

    // ./gen_tc.sh -c [CV/PV/CONN] -m ["ABS_CV ABS_NO_ABS_Trailer ...."]

    return result;
}

void TestCase::drawTerminalMenu(const int& excuteType, const QStringList& itemList) {
    // 고정 폭 정의 (줄당 4개 표시, 최대폭 140 기준으로 정렬)
    const int itemsPerLine = 4;
    const int lineCount = 140;
    const int fixedWidth = ((lineCount - 10) * (1.0 / itemsPerLine));
    const bool multipleSelect = (excuteType == ExcuteTypeParsingModule);

    QString displayText;

    if (excuteType == ExcuteTypeInvalidSelectItem) {
        displayText.append(QString("Invalid choice.\n"));
        displayText.append(QString("Please enter valid numbers between : 1 ~ %1\n").arg(itemList.size() - 1));
    } else {
        // system("clear");

        displayText.append(QString(lineCount, '*') + QString("\n"));

        if (excuteType == ExcuteTypeParsingMode) {
            displayText.append("\033[32m[Mode]\033[0m\n");
        } else if (excuteType == ExcuteTypeParsingModule) {
            displayText.append("\033[32m[Module]\033[0m\n");
        } else {
            return;
        }

        for (int index = 1; index < itemList.size(); ++index) {                         // itemList.at(0) = Exit
            displayText.append(QString("%1. %2").arg(index, 3)                          // 번호를 3자리 폭으로 맞춤
                                                .arg(itemList[index], -fixedWidth));    // 옵션 이름을 왼쪽 정렬, 고정 폭 설정

            if ((((index) % itemsPerLine) == 0) || (index == (itemList.size() - 1))) {
                displayText.append("\n");
            }
        }
        displayText.append(QString(lineCount, '-') + QString("\n"));
        if (excuteType == ExcuteTypeParsingModule) {
            displayText.append(
                QString("\033[33m%1\033[0m. \033[33m%2\033[0m\n").arg(999, 3).arg("Select All Modules", -fixedWidth));
        }
        displayText.append(QString("\033[31m%1\033[0m. \033[31m%2\033[0m\n").arg(0, 3).arg(itemList.at(0), -fixedWidth));

        displayText.append(QString(lineCount, '*') + QString("\n\n\n"));
    }
    displayText.append(QString("Enter the numbers of your choices %1: ").arg((multipleSelect) ? ("separated by spaces ") : ("")));

    QTextStream output(stdout);
    output << displayText << Qt::flush;
}

QStringList TestCase::selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& itemList) {
    QTextStream input(stdin);
    // QTextStream output(stdout);

    bool multipleSelect = (excuteType == ExcuteTypeParsingModule);

    drawTerminalMenu(excuteType, itemList);

    while (true) {
        QStringList selecteItems;
        QString inputLine = input.readLine().trimmed();
        QStringList choices({inputLine});

        if (multipleSelect) {
            choices = inputLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        }

        for (const QString& choice : choices) {
            bool valid;
            int selectIndex = choice.toInt(&valid);

            if (valid) {
                if (selectIndex == 0) {
                    selecteItems.append(choice);
                    break;
                } else if (selectIndex == 999) {
                    for (int index = 1; index < itemList.size(); ++index) {
                        selecteItems.append(itemList[index]);
                    }
                    break;
                } else if ((selectIndex > 0) && (selectIndex < itemList.size())) {
                    selecteItems.append(itemList[selectIndex]);
                } else {
                }
            }
        }

        if (selecteItems.size() == 0) {
            drawTerminalMenu(ExcuteTypeInvalidSelectItem, itemList);
            continue;
        }

        return selecteItems;
    }
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
