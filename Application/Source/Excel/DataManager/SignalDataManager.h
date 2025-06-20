#ifndef SIGNAL_DATA_MANAGER_H
#define SIGNAL_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class SignalData {
    REGISTER_WRITABLE_VALUE(int, DataType, 0)
    REGISTER_WRITABLE_VALUE(bool, Initialize, false)
    REGISTER_WRITABLE_VALUE(int, KeywordType, 0)
    REGISTER_WRITABLE_VALUE(QStringList, OriginData, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, ConvertData, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, ValueEnum, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, NotUsedEnum, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, Precondition, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, AllConvertData, QStringList())

public:
    SignalData(const QString& signalName, const int& dataType, const int& initialize, const int& keywordType,
               const QStringList& originData, const QStringList& convertData, const QStringList& valueEnum,
               const QStringList& notUsedEnum, const QStringList& precondition,
               const QStringList& allConvertData = QStringList()) {
        setDataType(dataType);
        setInitialize(initialize);
        setKeywordType(keywordType);
        setOriginData(originData);
        setConvertData(convertData);
        setValueEnum(valueEnum);
        setNotUsedEnum(notUsedEnum);
        setPrecondition(precondition);
        setAllConvertData(allConvertData);
    }
    SignalData() = default;
    SignalData(const SignalData& other) = default;
    SignalData& operator=(const SignalData& other) = default;

    bool operator==(const SignalData& other) const {
        return ((getDataType() == other.getDataType()) && (getInitialize() == other.getInitialize()) &&
                (getKeywordType() == other.getKeywordType()) && (getOriginData() == other.getOriginData()) &&
                (getConvertData() == other.getConvertData()) && (getValueEnum() == other.getValueEnum()) &&
                (getNotUsedEnum() == other.getNotUsedEnum()) && (getPrecondition() == other.getPrecondition()) &&
                (getAllConvertData() == other.getAllConvertData()));
    }
    bool operator!=(const SignalData& other) const {
        return !(*this == other);
    }
};

class SignalDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())

public:
    static QSharedPointer<SignalDataManager>& instance();

    int isDataType(const QString& dataTypeStr);
    int isSignalType(const QString& signalName);
    QMap<int, QStringList> isSignalDataList(const QString& signalName, const QStringList& inputData, const QString& vehicleType,
                                            int& dataType);
    QString isSignalValueEnum(const QString& signalName, const QString& value);
    QStringList isSignalValueEnum(const bool& toEnum, const QString& signalName);
    QStringList isConvertedSignalDataValueEnum(const bool& toEnum, const QString& signalName, const QStringList& valueEnum,
                                               QString& matchingValue);

    QMap<int, QPair<QString, SignalData>> isNormalInputSignalDataInfo(const int& sheetIndex,
                                                                      const QPair<QStringList, QStringList>& list);
    QMap<int, QPair<QString, SignalData>> isTestCaseInputSignalDataInfo(const int& sheetIndex,
                                                                        const QPair<QStringList, QStringList>& allList,
                                                                        const QPair<QStringList, QStringList>& list,
                                                                        QMap<QString, SignalData>& newSignalDataInfo);
    QMap<int, QPair<QString, SignalData>> isOtherInputSignalDataInfo(const int& sheetIndex,
                                                                     const QPair<QStringList, QStringList>& allList,
                                                                     QMap<QString, SignalData>& newSignalDataInfo);
    QMap<int, QPair<QString, SignalData>> isOutputSignalDataInfo(const int& sheetIndex, const QList<QStringList>& list);
    // QMap<int, QPair<QString, SignalData>> isDependSignalDataInfo(const int& sheetIndex, const QList<QStringList>& list);
    bool isExcelDataValidation();
    void testCode(const QVariantList& arg = QVariantList());
    // QStringList isSignalListInfo(const bool& sfcSignal);

private:
    explicit SignalDataManager();

    QString isSfcFileInfo(const QString& signalName);
    QStringList isVsmFileInfo(const QString& vehicleName, const QStringList& specType);
    QMap<int, QStringList> isSignalFileList(const QString& signalName, const QString& vehicleType);
    QString isSFCCommonEnum(const QString& info, const QString& prefix);
    QMap<int, QStringList> isParsingFileDataInfo(const QString& signalName, const QStringList& inputData,
                                                 const QMap<int, QStringList>& fileList, int& dataType);
    QStringList isValidValueEnum(const QString& signalName, const QMap<int, QStringList>& dataInfo);
    QStringList isConvertedSignalDataMatchingTable(const bool& toHex, const QString& signalName,
                                               const QMap<int, QStringList>& dataInfo);
    QList<QStringList> parsingKeywordBlocks(const QStringList& originData);
    QMap<int, QStringList> parsingKeywordData(const QStringList& originData);
    QMap<int, QStringList> isCustomValueInfo(const QStringList& originData, const bool& normal);
    QList<qint64> isNumber(const QStringList& numberStrList, const QMap<QString, qint64>& exception = QMap<QString, qint64>());
    QPair<qint64, qint64> isMinMaxValue(const QStringList& inputList, const bool& maxSkip = false);
    QPair<QStringList, QStringList> isValidValueList(const int& dataType, const int& notTriggerType, const QStringList& allData,
                                                     const QMap<int, QStringList>& dataInfo);
    QStringList isValidUniqueValue(const int& dataType, const QMap<int, QStringList>& dataInfo);
    QStringList isMultiValidUniqueValue(const int& dataType, const QMap<int, QStringList>& dataInfo);
    bool isExceptionSignal(const QString& signalName);
    int isContainConfigSignal(const QString& signalName, const int& keywordType, const QPair<QStringList, QStringList>& allList);
    int isSignalDataType(const QString& signalName, const SignalData& signalData, QString& maxValue);
    QMap<QString, SignalData> isSignalDataInfo(const QStringList& signalList, const QStringList& dataList,
                                               QMap<QString, QMap<int, QStringList>>& dataInfo);
    QPair<QStringList, QStringList> isConvertedValueData(const QString& signalName, const int& dataType, const bool& normal,
                                                         const QStringList& originData, const QStringList& otherAllData);
    QMap<int, QPair<QStringList, QStringList>> isConvertedEnumData(const QString& signalName, const SignalData& signalData,
                                                                   const bool& configState);
    QStringList isConvertedExceptionData(const QString& signalName, const QMap<int, QStringList>& dataInfo,
                                         const QStringList& inputData);
    QMap<int, QPair<QString, SignalData>> isSortingInputSignalList(const QMap<QString, SignalData>& dataInfo,
                                                                   const QStringList& signalList);
    // QStringList extractMatchingSignal(const bool& sfcSignal, const QString& filePath, const QString& prefix);
};

#endif  // SIGNAL_DATA_MANAGER_H
