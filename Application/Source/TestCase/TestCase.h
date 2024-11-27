#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>

#include "CommonUtil.h"
#include "CommonEnum.h"
#include "ConfigSetting.h"
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

    enum {
        JsonCasesName = 0,
        JsonCaseSizeName,
        JsonAllCaseSizeName,
        JsonOtherCaseSizeName,
        Max,
    };

    QMap<int, QString> mJsonProperty;

public:
    static QSharedPointer<TestCase>& instance();

    void excuteTestCase(const int& type = ExcuteTypeGenTC);
    QString genCase();
    void saveHistory();
    void printCase(const QMap<QString, QMap<QString, QString>>& tmpSignalMap);
    void eraseMergeTag(QString& str);
    QString getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName, const QString& resultName,
                                const QString& caseName);
    void callPython(const QString& str);
    QJsonObject readJson(const QString& filePath = "");
    void printJson(const QJsonObject& jsonObj);
    void appendCaseJson(QJsonObject& fileJson, QJsonObject& caseJson, const QString& caseName, const int& caseNumber,
                        const QString& resultName, const int& resultNumber, const QString& vehicleType, const QString& tcName,
                        const int& tcNameNumber, const int& sheetNumber, const QString& genType);
    void appendCase(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                    const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                    const int& sheetNumber);
    QString getGenType();
    QJsonObject getConfigSig(const int& sheetIdx, const QStringList& strList);
    QJsonObject getOutputSig(const int& sheetIdx, const QStringList& strList);
    void saveJsonToFile(const QJsonObject& json, const QString& filePath);
    void appendOtherCaseJson(QJsonObject& fileJson, const QString& caseName, const int& caseNumber, const QString& resultName,
                             const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                             const int& sheetNumber);
    void removeMatchingKeys(QJsonObject& otherJson, const QJsonObject& validArray);
    void genTestCaseFile(const QJsonObject& json);
    void printCaseSize(const QString& genType);

private:
    template <typename T>
    QJsonArray toJsonArray(const QList<T>& list);
    explicit TestCase();

private:
    QMap<QString, QSharedPointer<SignalDataInfo>> mSignalDataInfo = QMap<QString, QSharedPointer<SignalDataInfo>>();
    QMap<QString, int> mCaseSizeMap;
    QJsonObject mDefaultFileJson;
    QJsonObject mNegativeFileJson;
    QJsonObject mPositiveFileJson;
    QJsonObject mAllFileJson;
};

#endif  // TEST_CASE_H
