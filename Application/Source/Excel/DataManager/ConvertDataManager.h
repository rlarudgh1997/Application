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
    QMap<int, QList<KeywordInfo>> constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex,
                                                               const QList<int>& columnList);
    bool isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                             const int& originSheetIndex, const bool& isEqualData);
    void constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    void constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    QList<QList<QStringList>> constructConvertConfigSignalSet(const QString& configName);
    bool appendConvertConfigSignalSet();
    bool appendConvertAllTCSignalSet();

    QStringList deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex);
    int getMergeKeywordType(const QString& data);
    QString constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName);

    inline QString constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const;

    QMap<int, QStringList> isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                            const bool& convert, const bool& mergeInfoErase, QList<QStringList>& convertData);
    QPair<int, int> isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2, const QString& input3,
                                      const bool& normal = true);
};

#endif  // CONVERT_DATA_MANAGER_H
