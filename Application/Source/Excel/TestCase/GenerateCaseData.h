#ifndef GENERATE_CASE_DATA_H
#define GENERATE_CASE_DATA_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <QSet>

#include "CommonDefine.h"
#include "CommonEnum.h"
#include "CommonUtil.h"
#include "ConfigSetting.h"

class GenerateCaseData : public QObject {
public:
    static QSharedPointer<GenerateCaseData>& instance();

    bool excuteGenerateCaseData();

private:
    explicit GenerateCaseData();

    QString genCase();
    void saveHistory();
    void eraseMergeTag(QString& str);
    void appendCase(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                    const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                    const int& sheetNumber);
    QPair<QString, QString> getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName,
                                                const QString& resultName, const QString& caseName, const bool& isOther);
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
    QStringList getPreconditionList(const QJsonObject& inputSignalList, const QJsonArray& caseValues);
    QMap<QString, int> getFlowKeywordIdxMap(const QJsonObject& inputSignalList);
    QMap<QString, int> getConfigIdxMap(const QJsonObject& inputSignalList);
    QMap<QString, QMap<QString, QString>> getCongigSigHexEnumMap(const QJsonObject& inputSignalList);
    QString getConfigTagStr(const bool& isOther, const QString& tcName, const QMap<QString, int>& configIdxMap,
                            const QMap<QString, QMap<QString, QString>>& configHexEnumMap, const QStringList& preconditionList,
                            const int& triggerSigIndex, const QString& triggerSigValue);
    QString getPreconditionStr(const QStringList& preconditionList, const int& triggerSigIndex = -1,
                               const QJsonValue& preconditionValue = "");
    // QString getPreconditionStr(const QStringList& preconditionList);
    QString getInputStr(const QString& triggerSigName, const QString& caseValue);
    QString getTcLine(const QString& tag, const QString& precondition, const QString& input);
    QString getInitStr(const QJsonObject& inputSignalList);
    void checkNegativeAndPositive(const QString& genType, const QString& caseName, const int& caseNumber,
                                  const QString& resultName, const int& resultNumber, const QString& tcName,
                                  const int& tcNameNumber, const int& sheetNumber);
    void printCaseSize(const QString& genType);

private:
    QMap<QString, int> mCaseSizeMap;
    QJsonObject mIntermediateDefaultJson;
    QJsonObject mAllCaseJson;
    int mTotalTestCaseCount = 0;
};

#endif  // GENERATE_CASE_DATA_H
