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
    QString excuteGenerateCaseData();

    REGISTER_WRITABLE_VALUE(QString, GenerateCaseDataState, QStringLiteral("READY"))
    REGISTER_WRITABLE_VALUE(int, CheckedTestCaseCount, 0)
    REGISTER_WRITABLE_VALUE(int, CompletedCaseCount, 0)

private:
    explicit GenerateCaseData();

    bool genCase();
    bool isGenerateStop(const QString& info = QString());
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
    QMap<QString, int> getFlowKeywordIdxMap(const QJsonObject& inputSignalList);
    QMap<QString, int> getConfigIdxMap(const QJsonObject& inputSignalList);
    QMap<QString, QMap<QString, QString>> getConfigSigHexEnumMap(const QJsonObject& inputSignalList);
    QStringList qJsonArrayToQStringList(const QJsonArray& jsonArray);
    QStringList getReplaceValueAtList(const QStringList& originList, const int& idx, const QString& value);
    QString getPreconditionStr(const QStringList& preconditionList, const int& triggerSigIndex = -1,
                               const QString& preconditionValue = "");
    QString getFinalStateStr(const QStringList& preconditionList, const int& triggerSigIndex = -1,
                             const QString& inputValue = "");
    QString getInputStr(const QString& triggerSigName, const QString& caseValue);
    QString getTcLine(const QString& tag, const QString& precondition, const QString& input);
    QString getFinalStateLine(const long long unsigned int& cnt, const QString& finalStateValues);
    QString getInitStr(const QJsonObject& inputSignalList);
    void checkNegativeAndPositive(const QString& genType, const QString& caseName, const int& caseNumber,
                                  const QString& resultName, const int& resultNumber, const QString& tcName,
                                  const int& tcNameNumber, const int& sheetNumber);
    void removeMatchingKeysNegative(QJsonObject& caseJson, const QJsonObject& positiveFinalStateCases,
                                    const QJsonArray& inputEmptyList);
    void eraseNotUsefulTC(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                          const int& resultNumber, const QString& tcName, const int& tcNameNumber, const int& sheetNumber,
                          const int& maxTriggerCnt);
    void removeTcByCnt(QJsonObject& caseJson, const QString& titleGenType, const int& maxTriggerCnt);
    QJsonArray copyQJsonArrayUpToIndex(const QJsonArray& array, const int& maxCnt);
    void setPreconditionValues(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                               const int& resultNumber, const QString& tcName, const int& tcNameNumber, const int& sheetNumber,
                               const int& preconditionLimitCnt);
    void applyPrecondition(QJsonObject& caseJson, const int& preconditionLimitCnt);
    void setConfigTag(const QString& caseName, const int& caseNumber, const QString& resultName, const int& resultNumber,
                      const QString& tcName, const int& tcNameNumber, const int& sheetNumber, const QString& configName,
                      const bool& isOther);
    void applyConfig(const QString& tcName, const QString& configName, QJsonObject& caseJson, const bool& isOther);
    QString getConfigTagStr(const QString& tcName, const QString& configName, const QMap<QString, int>& configIdxMap,
                            const QMap<QString, QMap<QString, QString>>& configHexEnumMap, const QStringList& preconditionList,
                            const int& triggerSigIndex, const QString& triggerSigValue, const bool& isOther);
    void cleanIntermediateDataFromJson(const QString& caseName, const int& caseNumber, const QString& resultName,
                                       const int& resultNumber, const QString& tcName, const int& tcNameNumber,
                                       const int& sheetNumber);
    void appendManualTcToJson();
    void setTotalTcCnt();
    void printCaseSize(const QString& genType) const;

private:
    QMap<QString, int> mCaseSizeMap;
    QJsonObject mIntermediateDefaultJson;
    QJsonObject mAllCaseJson;
    int mTotalTestCaseCount = 0;
};

#endif  // GENERATE_CASE_DATA_H
