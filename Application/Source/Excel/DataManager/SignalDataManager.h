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

    int getDataType(const QString& dataTypeStr);
    int isSignalType(const QString& signalName);
    QString isSfcFileInfo(const QString& signalName);
    QStringList isVsmFileInfo(const QString& vehicleName, const QStringList& specType);
    QMap<int, QStringList> isSignalFileList(const QString& signalName, const QString& vehicleType);
    QMap<int, QStringList> isParsingFileDataInfo(const QString& signalName, const QStringList& inputData,
                                                 const QMap<int, QStringList>& fileList, int& dataType);
    QMap<int, QStringList> isSignalDataList(const QString& signalName, const QStringList& inputData, const QString& vehicleType,
                                            int& dataType);
    QString isSignalValueEnum(const QString& signalName, const QString& value);
    QStringList isSignalValueEnum(const bool& toEnum, const QString& signalName);
    QStringList isConvertedSignalData(const bool& toEnum, const QString& signalName, const QStringList& valueEnum,
                                      QString& matchingValue);
    QMap<QString, SignalData> isSignalDataInfo(const QStringList& signalList, const QStringList& dataList,
                                               QMap<QString, QMap<int, QStringList>>& dataInfo);
    QMap<int, QPair<QString, SignalData>> isNormalInputSignalDataInfo(const QPair<QStringList, QStringList>& list);
    QMap<int, QPair<QString, SignalData>> isTestCaseInputSignalDataInfo(const QPair<QStringList, QStringList>& list,
                                                                        QMap<QString, SignalData>& newSignalDataInfo);
    QMap<int, QPair<QString, SignalData>> isOtherInputSignalDataInfo(const QPair<QStringList, QStringList>& list,
                                                                     QMap<QString, SignalData>& newSignalDataInfo);
    QMap<int, QPair<QString, SignalData>> isOutputSignalDataInfo(const QList<QStringList>& list);
    QMap<int, QPair<QString, SignalData>> isDependSignalDataInfo(const QList<QStringList>& list);
    bool isExcelDataValidation();

private:
    explicit SignalDataManager();

    bool isExceptionSignal(const QString& signalName);
    QString isCheckBothExceptionValue(const QMap<int, QStringList>& dataInfo, const QString& origintStr, const QString& checkStr);
    QPair<QStringList, QStringList> isCheckExceptionValueEnum(const QString& signalName, const QMap<int, QStringList>& dataInfo);
    QString isCheckExceptionSpecialText(const QMap<int, QStringList>& dataInfo, const QString& origintStr,
                                        const QString& checkStr);
    QMap<int, QPair<QString, SignalData>> isSortingInputSignalList(const QMap<QString, SignalData>& dataInfo,
                                                                   const QStringList& signalList);
};

#endif  // SIGNAL_DATA_MANAGER_H
