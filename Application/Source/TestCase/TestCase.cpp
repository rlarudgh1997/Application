#include "TestCase.h"

QSharedPointer<TestCase>& TestCase::instance() {
    static QSharedPointer<TestCase> gInstance;
    if (gInstance.isNull()) {
        gInstance = QSharedPointer<TestCase>(new TestCase());
    }
    return gInstance;
}

TestCase::TestCase() {
}

void TestCase::excuteTestCase(const int& type) {
    if (getExcuteType() == type) {
        qDebug() << "Running excute tc type :" << type;
        return;
    }

    switch (type) {
        case ExcuteTypeGenTC: {
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

int TestCase::isDataType(QMap<int, QStringList>& dataInfo) {
    SignalDataInfo info = SignalDataInfo(dataInfo, QString());
    int dataType = info.isDataType();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = info.getValueEnum();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = info.getMatchingTableICV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = info.getMatchingTableEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = info.getMatchingTableFCEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = info.getMatchingTablePHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = info.getMatchingTableHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = info.getMatchingTableSystem();
    return dataType;
}

void TestCase::clearSignalDataInfo(const QString& signalName) {
    qDebug() << "clearSignalDataInfo :" << mSignalDataInfo.size() << signalName;

    if (signalName.size() == 0) {
        mSignalDataInfo.clear();
    } else {
        mSignalDataInfo.remove(signalName);
    }
}

void TestCase::setSignalDataInfo(const QString& signalName, const QMap<int, QStringList>& dataInfo, const QString& dataType) {
    if (dataInfo.size() == 0) {
        return;
    }

    mSignalDataInfo[signalName] = QSharedPointer<SignalDataInfo>::create(dataInfo, dataType);
#if 0
    qDebug() << "=============================================================================================";
    for (auto iter = mSignalDataInfo.cbegin(); iter != mSignalDataInfo.cend(); ++iter) {
        QString signal = iter.key();
        if (signal.compare(signalName) != false) {
            continue;
        }

        QSharedPointer<SignalDataInfo> signalInfo = iter.value();
        if (signalInfo) {
            qDebug() << "\n    [Write] :" << signalName.toLatin1().data()
                    << "\n\t     DataType            :" << signalInfo->getDataType()
                    << "\n\t     ValueEnum           :" << signalInfo->getValueEnum()
                    << "\n\t     MatchingTableICV    :" << signalInfo->getMatchingTableICV()
                    << "\n\t     MatchingTableEV     :" << signalInfo->getMatchingTableEV()
                    << "\n\t     MatchingTableFCEV   :" << signalInfo->getMatchingTableFCEV()
                    << "\n\t     MatchingTablePHEV   :" << signalInfo->getMatchingTablePHEV()
                    << "\n\t     MatchingTableHEV    :" << signalInfo->getMatchingTableHEV()
                    << "\n\t     MatchingTableSystem :" << signalInfo->getMatchingTableSystem();
        }
    }
    qDebug() << "=============================================================================================\n\n";
#endif
}

QMap<int, QStringList> TestCase::getSignalDataInfo(const QString& signalName, QString& dataType) {
    QMap<int, QStringList> signalInfo = QMap<int, QStringList>();
    QSharedPointer<SignalDataInfo> dataInfo = mSignalDataInfo[signalName];

    if (dataInfo.isNull()) {
        qDebug() << "Fail to read signal data(invalid) info : " << signalName;
        return signalInfo;
    }

    dataType = dataInfo->getDataType();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = dataInfo->getValueEnum();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = dataInfo->getMatchingTableICV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = dataInfo->getMatchingTableEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = dataInfo->getMatchingTableFCEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = dataInfo->getMatchingTablePHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = dataInfo->getMatchingTableHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = dataInfo->getMatchingTableSystem();
#if 1
    qDebug() << "=============================================================================================";
    qDebug() << "\n    [Read] :" << signalName.toLatin1().data() << "\n\t     DataType            :" << dataType
             << "\n\t     ValueEnum           :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum]
             << "\n\t     MatchingTableICV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV]
             << "\n\t     MatchingTableEV     :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV]
             << "\n\t     MatchingTableFCEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV]
             << "\n\t     MatchingTablePHEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV]
             << "\n\t     MatchingTableHEV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV]
             << "\n\t     MatchingTableSystem :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    qDebug() << "=============================================================================================\n\n";
#endif

    return signalInfo;
}
