#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"
#include "CommonEnum.h"
#include "SignalDataInfo.h"

class TestCase : public QObject {
    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER2(QMap, int, QVariant, SheetData, false)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeGenTC,
        ExcuteTypeRunTC,
    };

public:
    static QSharedPointer<TestCase>& instance();

    void excuteTestCase(const int& type = ExcuteTypeGenTC);
    int isKeywordType(const QString& signalName);
    int isDataType(QMap<int, QStringList>& dataInfo, const int& keywordType);
    void clearSignalDataInfo(const QString& signalName = QString());
    void setSignalDataInfo(const QString& signalName, const QMap<int, QStringList>& dataInfo, const int& keywordType,
                           const QString& dataType = QString());
    QMap<int, QStringList> getSignalDataInfo(const QString& signalName, int& keywordType, QString& dataType);
    QString getSignalInfoString();

private:
    explicit TestCase();

private:
    QMap<QString, QSharedPointer<SignalDataInfo>> mSignalDataInfo = QMap<QString, QSharedPointer<SignalDataInfo>>();
};

#endif  // TEST_CASE_H
