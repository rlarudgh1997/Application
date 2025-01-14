#ifndef SIGNAL_DATA_MANGER_H
#define SIGNAL_DATA_MANGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class SignalData {
public:
    SignalData(const QString& signalName, const int& dataType, const int& initialize, const int& keywordType,
               const QStringList& originData, const QStringList& convertData, const QStringList& valueEnum,
               const QStringList& notUsedEnum, const QStringList& precondition)
        : mDataType(dataType),
          mInitialize(initialize),
          mKeywordType(keywordType),
          mOriginData(originData),
          mConvertData(convertData),
          mValueEnum(valueEnum),
          mNotUsedEnum(notUsedEnum),
          mPrecondition(precondition) {
    }
    SignalData() = default;
    SignalData(const SignalData& other) = default;
    SignalData& operator=(const SignalData& other) = default;

    bool operator==(const SignalData& other) const {
        return ((mDataType == other.mDataType) && (mInitialize == other.mInitialize) && (mKeywordType == other.mKeywordType) &&
                (mOriginData == other.mOriginData) && (mConvertData == other.mConvertData) && (mValueEnum == other.mValueEnum) &&
                (mNotUsedEnum == other.mNotUsedEnum) && (mPrecondition == other.mPrecondition));
    }
    bool operator!=(const SignalData& other) const {
        return !(*this == other);
    }

    int isDataType() const {
        return mDataType;
    }
    bool isInitialize() const {
        return mInitialize;
    }
    int isKeywordType() const {
        return mKeywordType;
    }
    QStringList isOriginData() const {
        return mOriginData;
    }
    QStringList isConvertData() const {
        return mConvertData;
    }
    QStringList isValueEnum() const {
        return mValueEnum;
    }
    QStringList isNotUsedEnum() const {
        return mNotUsedEnum;
    }
    QStringList isPrecondition() const {
        return mPrecondition;
    }

private:
    int mDataType = 0;
    bool mInitialize = false;
    int mKeywordType = 0;
    QStringList mOriginData = QStringList();
    QStringList mConvertData = QStringList();
    QStringList mValueEnum = QStringList();
    QStringList mNotUsedEnum = QStringList();
    QStringList mPrecondition = QStringList();
};

class SignalDataManger : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, MergeStart, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, Merge, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, MergeEnd, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MergeInfos, QStringList(), false)

public:
    static QSharedPointer<SignalDataManger>& instance();

    int isDataType(const QString& dataTypeStr);
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
    QMap<QString, SignalData> isNormalInputSignalDataInfo(const QPair<QStringList, QStringList>& list);
    QMap<QString, SignalData> isTestCaseInputSignalDataInfo(const QPair<QStringList, QStringList>& list);
    QMap<QString, SignalData> isOtherInputSignalDataInfo(const QPair<QStringList, QStringList>& list);
    QMap<QString, SignalData> isOutputSignalDataInfo(const QList<QStringList>& list);
    QMap<QString, SignalData> isConfigSignalDataInfo(const QPair<QStringList, QStringList>& list);
    bool isExcelDataValidation();

private:
    explicit SignalDataManger();

    bool isExceptionSignal(const QString& signalName);
    QString isCheckBothExceptionValue(const QMap<int, QStringList>& dataInfo, const QString& origintStr, const QString& checkStr);
    QPair<QStringList, QStringList> isCheckExceptionValueEnum(const QString& signalName, const QMap<int, QStringList>& dataInfo);
};


#endif  // SIGNAL_DATA_MANGER_H
