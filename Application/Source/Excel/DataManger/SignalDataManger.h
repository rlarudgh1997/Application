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

    QMap<QString, SignalData> isSignalDataInfo(const QPair<QStringList, QStringList>& list);

private:
    explicit SignalDataManger();

};


#endif  // SIGNAL_DATA_MANGER_H
