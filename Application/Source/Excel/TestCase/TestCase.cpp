#include "TestCase.h"

// #include "CommonDefine.h"
// #include "CommonEnum.h"
// #include "ConfigSetting.h"

#include "ConvertDataManager.h"
#include "GenerateCaseData.h"

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
    switch (type) {
        case ExcuteTypeGenConvertData: {
            parsingExcuteOption(data);
            ConvertDataManager::instance().data()->excuteConvertDataManager();
            break;
        }
        case ExcuteTypeGenTC: {
            GenerateCaseData::instance().data()->excuteGenerateCaseData();
            break;
        }
        case ExcuteTypeRunTC: {
            break;
        }
        default: {
            break;
        }
    }
    setExcuteType(type);
}

void TestCase::parsingExcuteOption(const QStringList& data) {
    setExcuteData(data);
}
