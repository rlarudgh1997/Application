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
    bool replaceGenDataInfo();  // will be deleted (replaced with convertKeywordData())
    bool appendConvertConfigSignalSet();
    bool appendConvertAllTCSignalSet();

    bool convertOutputDataKeyword();
    bool convertInputSignalKeyword();
    bool convertInputDataKeyword();
    bool convertNonInputSignalKeyword();

private:
    QList<OutputDataInfo> convertOutputStructData(const QList<QStringList>& outputList);
    void updateResultDataInfo(const int& sheetIndex, const QString& tcName, const QList<ResultInfo>& resultDataList);
    void displayResultDataInfo(const ResultInfo& result);
    ConvertKeywordInfo interpretInputValueKeyword(const QString& signal, const QString& value);
    QString findVehicleSignalElseTimeoutCrcValue(const QString& vehicleSignal, const QString& elseTimeoutCrc);
    QList<QPair<QString, QPair<QStringList, QStringList>>> convertSheetKeyword(
        const QPair<QStringList, QStringList>& inputDataPairInfo, const int& currentSheetIndex);
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
    QPair<QStringList, QList<CaseDataInfo>> generateCombinations(const QString& templateExpr,
                                                                 const QMap<QString, QStringList>& valueMap,
                                                                 const QStringList& keys, const QList<int>& keywords,
                                                                 const QStringList& inputValues, const QString& caseName,
                                                                 int index = 0, QMap<QString, QString> current = {});
    QList<ResultInfo> interpretCalKeywordAndRedefineResultInfo(const ResultInfo& resultInfo);
    bool decideSameCaseList(const QList<CaseDataInfo>& list1, const QList<CaseDataInfo>& list2);
    QList<ResultInfo> mergeAndCleanResultList(const QList<ResultInfo>& resultList);

    /******************************** will be deleted ********************************/
    void duplicatedConstructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    QString constructConvertKeywordDataInfoBackup(const int& keywordType, const QString& inputData);
    void constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    QMap<int, QList<KeywordInfo>> constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex,
                                                               const QList<int>& columnList);
    bool isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                             const int& originSheetIndex, const bool& isEqualData);

    QStringList deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex);
    int getMergeKeywordType(const QString& data);
    QString constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName);

    inline QString constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const;

    QPair<QStringList, QStringList> getMergedInputDataInfo(const QPair<QStringList, QStringList>& origin,
                                                           const QPair<QStringList, QStringList>& sheet);
    QString mergeCaseName(const QString& prefix, const QString& suffix);
};

#endif  // CONVERT_DATA_MANAGER_H
