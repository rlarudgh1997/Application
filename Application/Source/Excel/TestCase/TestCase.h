#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>

#include "CommonDefine.h"
#include "CommonEnum.h"
#include "CommonUtil.h"
#include "ConfigSetting.h"
#include "ControlExcel.h"  // 임시방편임. 사라질 예정
#include "ExcelDataManger.h"
#include "SignalDataManger.h"
#include "TestCaseWriter.h"

class TestCase : public QObject {
    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QVariant, SheetData, false)

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

public:
    static QSharedPointer<TestCase>& instance();

    void excuteTestCase(const int& type = ExcuteTypeGenTC);

private:
    QString genCase();
    void saveHistory();
    void eraseMergeTag(QString& str);
    void appendCase(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                    const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                    const int& sheetNumber);
    QString getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName, const QString& resultName,
                                const QString& caseName, const bool& isOther);
    void callPython(const QString& str);
    QJsonObject readJson(const QString& filePath = "");
    void printJson(const QJsonObject& jsonObj);
    void appendCaseJson(QJsonObject& fileJson, QJsonObject& caseJson, const QString& caseName, const int& caseNumber,
                        const QString& resultName, const int& resultNumber, const QString& vehicleType, const QString& tcName,
                        const int& tcNameNumber, const int& sheetNumber, const QString& genType);
    template <typename T>
    QJsonArray toJsonArray(const QList<T>& list);
    QString getGenType();
    QJsonObject getConfigSig(const int& sheetIdx, const QStringList& strList);
    QJsonObject getOutputSig(const int& sheetIdx, const QString& tcName, const QString& resultName);
    void saveJsonToFile(const QJsonObject& json, const QString& filePath);
    void appendOtherCaseJson(QJsonObject& fileJson, const QString& caseName, const int& caseNumber, const QString& resultName,
                             const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                             const int& sheetNumber);
    void removeMatchingKeys(QJsonObject& otherJson, const QJsonObject& validArray);
    QJsonObject getCaseInfoJson(const QString& genType, const QString& tcName, const QJsonObject& caseJsonObject,
                                const bool& isOther);
    QString getConfigTagStr(const bool& isOther, const QString& tcName, const QMap<QString, int>& configIdxMap,
                            const QJsonArray& caseValues, const int& triggerSigIndex, const QString& triggerSigValue);
    QString getPreconditionStr(const QJsonArray& caseValues, const int& triggerSigIndex, const QJsonValue& preconditionValue);
    QString getPreconditionStr(const QJsonArray& caseValues);
    QString getInputStr(const QString& triggerSigName, const QString& caseValue);
    QString getTcLine(const QString& tag, const QString& precondition, const QString& input);
    void printCaseSize(const QString& genType);
    explicit TestCase();

private:
    QMap<QString, int> mCaseSizeMap;
    QJsonObject mIntermediateDefaultJson;
    QJsonObject mAllCaseJson;
};

#endif  // TEST_CASE_H
