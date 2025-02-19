#ifndef CONVERT_DATA_MANAGER_H
#define CONVERT_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class KeywordInfo;

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
    bool replaceGenDataInfo();  // To be deleted (replaced to convertKeywordData())
    bool appendConvertConfigSignalSet();
    bool appendConvertAllTCSignalSet();

    bool convertInputSignalKeyword();
    bool convertInputDataKeyword();

private:
    QList<QPair<QString, QPair<QStringList, QStringList>>> convertSheetKeyword(
        const QPair<QStringList, QStringList>& inputDataPairInfo, const int& currentSheetIndex);
    QList<QPair<QString, QPair<QStringList, QStringList>>> getSheetSignalDataInfo(const QString& name, const QString& data);

    QMap<int, QList<KeywordInfo>> constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex,
                                                               const QList<int>& columnList);
    bool isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                             const int& originSheetIndex, const bool& isEqualData);
    void constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    void constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    QList<QList<QStringList>> constructConvertConfigSignalSet(const QString& configName);

    QStringList deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex);
    int getMergeKeywordType(const QString& data);
    QString constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName);

    inline QString constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const;

    QMap<int, QStringList> isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                            const bool& convert, const bool& mergeInfoErase, QList<QStringList>& convertData);
    QPair<int, int> isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2, const QString& input3,
                                      const bool& normal = true);

    QPair<QStringList, QStringList> getMergedInputDataInfo(const QPair<QStringList, QStringList>& origin,
                                                           const QPair<QStringList, QStringList>& sheet);
    QString mergeCaseName(const QString& prefix, const QString& suffix);
    void appendCurSheetData(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputDataInfo,
                            QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData);
    void updateCurSheetData(const int& sheetIndex,
                            const QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData);
};

#endif  // CONVERT_DATA_MANAGER_H
