#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "CommonUtil.h"
#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "SignalDataInfo.h"
#include "ControlExcel.h"  // 임시방편임. 사라질 예정

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
    QString getSignalInfoString(const QMap<QString, QMap<QString, QString>>& singleCase);
    QString genCase();
    void eraseMergeTag(QString& str);
    void callPython(const QString& str);
    QJsonObject readJson(const QString& filePath);
    void printJson(const QJsonObject& jsonObj, int indent = 0);
    void printArray(const QJsonArray& jsonArray, int indent);
    void appendCaseJson(QJsonObject& sumJson, const QJsonObject& newJson, int caseNumber);
    void addTcStr(const QJsonObject& TcNameJson);
    void saveJsonToFile(const QJsonObject& json, const QString& filePath);
    void appendTCNameJson(const QString& newKey, const QJsonObject& newData, const QString& filePath);

private:
    explicit TestCase();

private:
    QMap<QString, QSharedPointer<SignalDataInfo>> mSignalDataInfo = QMap<QString, QSharedPointer<SignalDataInfo>>();
    QString mBuffer;
};

#endif  // TEST_CASE_H
