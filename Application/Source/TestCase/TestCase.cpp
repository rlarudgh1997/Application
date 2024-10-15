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
            QString str = getSignalInfoString();
            QString cmd = QString("python3 %1/../Python/ItertoolsTest.py \"%2\"").arg(ivis::common::APP_PWD()).arg(str);
            ivis::common::ExcuteProgram process(false);
            QStringList log;
            bool result = process.start(cmd, log);

            if (result) {
                qDebug() << "success!: " << log;
            } else {
                qDebug() << "fail!: " << log;
            }

            QStringList readData =
                ivis::common::FileInfo::readFile(QString("%1/../Python/ItertoolsTest.txt").arg(ivis::common::APP_PWD()));

            for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
                // qDebug() << "rowIndex" << rowIndex << ": " << readData[rowIndex];
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
    setExcuteType(type);
}

int TestCase::isKeywordType(const QString& signalName) {
    QSharedPointer<SignalDataInfo> dataInfo = mSignalDataInfo[signalName];
    return dataInfo->getKeywordType();
}

int TestCase::isDataType(QMap<int, QStringList>& dataInfo, const int& keywordType) {
    SignalDataInfo info = SignalDataInfo(dataInfo, keywordType, QString());
    int dataType = info.isDataType();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = info.getValueEnum();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = info.getMatchingTableICV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = info.getMatchingTableEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = info.getMatchingTableFCEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = info.getMatchingTablePHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = info.getMatchingTableHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = info.getMatchingTableSystem();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = info.getInputData();
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

void TestCase::setSignalDataInfo(const QString& signalName, const QMap<int, QStringList>& dataInfo, const int& keywordType,
                                 const QString& dataType) {
    if (dataInfo.size() == 0) {
        return;
    }

    mSignalDataInfo[signalName] = QSharedPointer<SignalDataInfo>::create(dataInfo, keywordType, dataType);
#if 0
    qDebug() << "=============================================================================================";
    for (auto iter = mSignalDataInfo.cbegin(); iter != mSignalDataInfo.cend(); ++iter) {
        QString signal = iter.key();
        if (signal.compare(signalName) != false) {
            continue;
        }

        QSharedPointer<SignalDataInfo> signalInfo = iter.value();
        if (signalInfo) {
            qDebug() << "\n\t [Write] :" << signalName.toLatin1().data()
                     << "\n\t\t KeywordType         :" << signalInfo->getKeywordType()
                     << "\n\t\t DataType            :" << signalInfo->getDataType()
                     << "\n\t\t InputData           :" << signalInfo->getInputData()
                     << "\n\t\t ValueEnum           :" << signalInfo->getValueEnum()
                     << "\n\t\t MatchingTableICV    :" << signalInfo->getMatchingTableICV()
                     << "\n\t\t MatchingTableEV     :" << signalInfo->getMatchingTableEV()
                     << "\n\t\t MatchingTableFCEV   :" << signalInfo->getMatchingTableFCEV()
                     << "\n\t\t MatchingTablePHEV   :" << signalInfo->getMatchingTablePHEV()
                     << "\n\t\t MatchingTableHEV    :" << signalInfo->getMatchingTableHEV()
                     << "\n\t\t MatchingTableSystem :" << signalInfo->getMatchingTableSystem();
        }
    }
    qDebug() << "=============================================================================================\n\n";
#endif
}

QMap<int, QStringList> TestCase::getSignalDataInfo(const QString& signalName, int& keywordType, QString& dataType) {
    QMap<int, QStringList> signalInfo = QMap<int, QStringList>();
    QSharedPointer<SignalDataInfo> dataInfo = mSignalDataInfo[signalName];

    if (dataInfo.isNull()) {
        qDebug() << "Fail to read signal data(invalid) info : " << signalName;
        return signalInfo;
    }

    dataType = dataInfo->getDataType();
    keywordType = dataInfo->getKeywordType();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = dataInfo->getInputData();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = dataInfo->getValueEnum();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = dataInfo->getMatchingTableICV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = dataInfo->getMatchingTableEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = dataInfo->getMatchingTableFCEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = dataInfo->getMatchingTablePHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = dataInfo->getMatchingTableHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = dataInfo->getMatchingTableSystem();
#if 0
    qDebug() << "=============================================================================================";
    qDebug() << "\n\t [Read] :" << signalName.toLatin1().data() << "\n\t\t KeywordType          :" << keywordType
             << "\n\t\t DataType            :" << dataType
             << "\n\t\t InputData           :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData]
             << "\n\t\t ValueEnum           :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum]
             << "\n\t\t MatchingTableICV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV]
             << "\n\t\t MatchingTableEV     :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV]
             << "\n\t\t MatchingTableFCEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV]
             << "\n\t\t MatchingTablePHEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV]
             << "\n\t\t MatchingTableHEV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV]
             << "\n\t\t MatchingTableSystem :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    qDebug() << "=============================================================================================\n\n";
#endif

    return signalInfo;
}

QString TestCase::getSignalInfoString() {
    QString ret;
    for (auto iter = mSignalDataInfo.constBegin(); iter != mSignalDataInfo.constEnd(); ++iter) {
        // signal name
        ret += "InputSignalName   : " + iter.key() + "\n";
        // data type
        int dataType = iter.value().data()->isDataType();
        if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) {
            ret += "InputDataType     : HUInt64\n";
        } else if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64)) {
            ret += "InputDataType     : HInt64\n";
        } else if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString)) {
            ret += "InputDataType     : HString\n";
        } else {
            ret += "InputDataType     : Invalid\n";
        }
        // Input
        ret += "InputData         : " + iter.value().data()->getInputData().join(", ").replace("\\", "").replace("\"", "") + "\n";
        // qDebug() << "iter.value().data()->getInputData(): " << iter.value().data()->getInputData();
        // All enum list
        ret += "InputValueEnum    : " + iter.value().data()->getValueEnum().join(", ").replace("\\", "").replace("\"", "") + "\n";
        // qDebug() << "iter.value().data()->getValueEnum(): " << iter.value().data()->getValueEnum();
    }
#if 0
    qDebug() << "getSignalInfoString ret: " << ret;
#endif
    return ret;
}