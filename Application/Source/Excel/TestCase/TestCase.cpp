#include "TestCase.h"

// #include "CommonDefine.h"
// #include "CommonEnum.h"
#include "ConfigSetting.h"

#include "ConvertDataManager.h"
#include "GenerateCaseData.h"

// #include <iostream>

QSharedPointer<TestCase>& TestCase::instance() {
    static QSharedPointer<TestCase> gInstance;
    if (gInstance.isNull()) {
        gInstance = QSharedPointer<TestCase>(new TestCase());
    }
    return gInstance;
}

TestCase::TestCase() {
}

void TestCase::excuteTestCase(const int& type, const QStringList& data) {
    bool graphicsMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGraphicsMode).toBool();
    qDebug() << "excuteTestCase :" << type << graphicsMode;

    bool nextStep = false;
    switch (type) {
        case ExcuteTypeGenConvertData: {
            if (parsingExcuteOption(graphicsMode, data)) {
                nextStep = ConvertDataManager::instance().data()->excuteConvertDataManager();
            }
            break;
        }
        case ExcuteTypeGenTC: {
            if (getNextStep()) {
                nextStep = GenerateCaseData::instance().data()->excuteGenerateCaseData();
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
    setNextStep(nextStep);
    setExcuteType(type);
}

bool TestCase::parsingExcuteOption(const bool& graphicsMode, const QStringList& data) {
    bool result = false;
    setExcuteData(data);

    if (graphicsMode) {
        result = true;
    } else {
        // AppMode : PV, CV
        // ModuleList : ABS_CV, .....
    }

    qDebug() << "parsingExcuteOption :" << graphicsMode << data << result;

    return result;
}

void TestCase::inputOption(const QStringList& args) {
    QTextStream input(stdin);
    QTextStream output(stdout);

    QString selectedMode;
    QString selectedModule;

    QStringList option;
    for (const auto& arg : args) {
        option.append(arg.toUpper());
    }

    if (option.contains("CV") || option.contains("PV")) {
        selectedMode = (option.contains("CV")) ? (QString("CV")) : (QString("PV"));
    } else {
        output << "Select mode : " << Qt::endl;
        selectedMode = selectOptionWithNumbers("[Mode Option]", QStringList({"Exit", "CV", "PV"}));
    }
    output << "Selected mode : " << selectedMode << Qt::endl;


    QStringList moduleList = {"Previous Option", "ModuleA", "ModuleB", "ModuleC"};
    output << "Available modules :" << Qt::endl;
    selectedModule = selectOptionWithNumbers("[Module Option]", moduleList);
    output << "Selected module : " << selectedModule << Qt::endl;
}

QString TestCase::selectOptionWithNumbers(const QString& prompt, const QStringList& options) {
    QTextStream input(stdin);
    QTextStream output(stdout);

    system("clear");

    QString displayText = prompt + "\n";
    for (int index = 0; index < options.size(); ++index) {
        displayText.append(QString("    %1. %2\n").arg(index + 1).arg(options[index]));
    }
    displayText.append("Enter the number of your choice : ");
    output << displayText << Qt::flush;

    while (true) {
        QString choice = input.readLine().trimmed();

        bool valid;
        int selectIndex = choice.toInt(&valid);

        if ((valid) && (selectIndex >= 0) && (selectIndex < options.size())) {
            return options[selectIndex];
        } else {
            output << "Invalid choice. Please enter a number between 1 and " << options.size() << "." << Qt::endl;
            output << "Enter the number of your choice : " << Qt::flush;
        }
    }
}
