#ifndef CONVERT_DATA_MANAGER_H
#define CONVERT_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"
#include "CommonEnum.h"

class KeywordInfo;

struct ConvertKeywordInfo {
    ivis::common::KeywordTypeEnum::KeywordType keywordType = ivis::common::KeywordTypeEnum::KeywordType::Invalid;
    QString inputSignal;
    QString inputValue;
    QString validInputData;
};

struct CaseDataInfo {
    QString caseName;
    QList<ConvertKeywordInfo> convertInputDataInfo;

    bool operator==(const CaseDataInfo& other) const {
        if (caseName != other.caseName || convertInputDataInfo.size() != other.convertInputDataInfo.size()) {
            return false;
        }
        for (int i = 0; i < convertInputDataInfo.size(); ++i) {
            const auto& a = convertInputDataInfo[i];
            const auto& b = other.convertInputDataInfo[i];
            if (a.keywordType != b.keywordType || a.inputSignal != b.inputSignal || a.inputValue != b.inputValue ||
                a.validInputData != b.validInputData) {
                return false;
            }
        }
        return true;
    }
};

struct OutputDataInfo {
    QString outputSignal;
    QString initialize;
    QString outputValue;
};

struct ResultInfo {
    QString resultName;
    QList<CaseDataInfo> caseDataInfoList;
    QList<OutputDataInfo> outputDataInfoList;
};

class ConvertDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())

public:
    static QSharedPointer<ConvertDataManager>& instance();

    bool excuteConvertDataManager();

private:
    explicit ConvertDataManager();

    bool convertKeywordData();
    bool appendConvertConfigSignalSet();
    bool appendConvertAllTCSignalSet();

    bool convertInputSignalSheetKeyword();
    bool convertNonInputSignalKeyword();
    bool convertOutputDataKeyword();

private:
    QList<OutputDataInfo> convertOutputStructData(const QList<QStringList>& outputList);
    void updateResultDataInfo(const int& sheetIndex, const QString& tcName, const QList<ResultInfo>& resultDataList);
    void displayResultDataInfo(const ResultInfo& result);
    ConvertKeywordInfo interpretInputValueKeyword(const QString& signal, const QString& value);
    QString findVehicleSignalElseTimeoutCrcValue(const QString& vehicleSignal, const QString& elseTimeoutCrc);
    QList<QPair<QString, QPair<QStringList, QStringList>>> convertSheetKeyword(
        const QPair<QStringList, QStringList>& inputDataPairInfo, const QString& genTypeStr);
    QList<QPair<QString, QPair<QStringList, QStringList>>> getSheetSignalDataInfo(const QString& name, const QString& data);
    QString convertCustomKeywordType(const int& keywordType);
    void appendCurSheetData(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputDataInfo,
                            QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData);
    void updateCurSheetData(const int& sheetIndex,
                            const QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData);

    QString constructConvertKeywordDataInfo(const int& keywordType, const QString& inputData);
    QList<QList<QStringList>> constructConvertConfigSignalSet(const QString& configName);

    QMap<int, QStringList> isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                            const bool& convert, const bool& mergeInfoErase, QList<QStringList>& convertData);
    QPair<int, int> isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2, const QString& input3,
                                      const bool& normal = true);
    QPair<QStringList, QList<CaseDataInfo>> generateCombinations(
        const QString& templateExpr, const QMap<QString, QStringList>& valueMap, const QStringList& keys,
        const QList<int>& keywords, const QStringList& inputValues, const QStringList& validInputData, const QString& caseName,
        const QString& notTriggerStr, const QString& customNotTrigger, int& calValArrCnt, int index = 0,
        QMap<QString, QString> current = {}, const QString& maxValue = QString::number(static_cast<uint64_t>(UINT32_MAX) + 1));
    QList<ResultInfo> interpretCalKeywordAndRedefineResultInfo(const ResultInfo& resultInfo);
    bool decideSameCaseList(const QList<CaseDataInfo>& list1, const QList<CaseDataInfo>& list2);
    QList<ResultInfo> mergeAndCleanResultList(const QList<ResultInfo>& resultList);
    int getMergeKeywordType(const QString& data);
    inline QString constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const;
    QPair<QStringList, QStringList> getMergedInputDataInfo(const QString& genTypeStr,
                                                           const QPair<QStringList, QStringList>& origin,
                                                           const QPair<QStringList, QStringList>& sheet);
    QString mergeCaseName(const QString& prefix, const QString& suffix);
    QString getIgnPrefixString(const QString& signal);
    QStringList getSheetHasMultiResult(const QString& tcName, const QString& result);
    QString deleteEasterEggKeyword(const QString& keyword);
};

#endif  // CONVERT_DATA_MANAGER_H
