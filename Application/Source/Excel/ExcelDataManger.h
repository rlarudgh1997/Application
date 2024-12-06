#ifndef EXCEL_DATA_MANGER_H
#define EXCEL_DATA_MANGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class InsertData {
public:
    InsertData(const QString& tcName, const QString& resultName, const QString& caseName,
               const QPair<QStringList, QStringList>& inputList) {
        updateInsertData(tcName, resultName, caseName, inputList);
    }
    InsertData() = default;
    InsertData(const InsertData& other) = default;
    InsertData& operator=(const InsertData& other) = default;

    QString isTCName() const {
        return mTCName;
    }
    QString isResultName() const {
        return mResultName;
    }
    QString isCaseName() const {
        return mCaseName;
    }
    QPair<QStringList, QStringList> isInputList() const {
        return mInputList;
    }

private:
    void updateInsertData(const QString& tcName, const QString& resultName, const QString& caseName,
                          const QPair<QStringList, QStringList>& inputList) {
        mTCName = tcName;
        mResultName = resultName;
        mCaseName = caseName;
        mInputList = inputList;
    }

private:
    QString mTCName;
    QString mResultName;
    QString mCaseName;
    QPair<QStringList, QStringList> mInputList;
};

class ExcelDataManger : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QVariantList, SheetData, QVariantList(), false)

public:
    static QSharedPointer<ExcelDataManger>& instance();

    QPair<QStringList, QStringList> isCaseInputList(const QString& tcName, const QString& resultName, const QString& caseName);
    QList<QStringList> isSheetDataInfo();
    int isTCNameIndex(const QString& tcName, const bool& relationship);
    int isResultIndex(const QString& tcName, const QString& resultName, const bool& relationship);
    int isCaseIndex(const QString& caseName, const bool& relationship);
    void updateExcelData(const QVariantList& sheetData);
    void updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputList);
    void clear(const bool& all = false);
    void printData(const bool& relationship = false);

private:
    explicit ExcelDataManger();

    void updateRelatonshipTCName(const int& rowMax, const QMap<int, QStringList>& excelData);
    void updateRelationshipResult(const int& rowMax, const QMap<int, QStringList>& excelData);
    void updateRelationshipCase(const int& rowMax, const QMap<int, QStringList>& excelData);

private:
    QList<InsertData> mExcelData;

    // QMap<TCNameIndex, QPair<TCName, ResultNameList>>
    QMap<int, QPair<QString, QStringList>> mTCNameRelationship;
    // QMap<TCNameIndex, QPair<VehicleType, Config>>
    QMap<int, QPair<QString, QString>> mConfigRelationship;
    // QMap<ResultIndex, QPair<ResultName, CaseNameList>>
    QMap<int, QPair<QString, QStringList>> mResultRelationship;
    // QMap<CaseIndex, QPair<CaseName, QPair<InputSignal, InputData>>>
    QMap<int, QPair<QString, QPair<QStringList, QStringList>>> mCaseRelationship;

    QMap<int, QPair<QString, QStringList>> mTCNameData;
    QMap<int, QPair<QString, QString>> mConfigData;
    QMap<int, QPair<QString, QStringList>> mResultData;
    QMap<int, QPair<QString, QPair<QStringList, QStringList>>> mCaseData;
};


#endif  // EXCEL_DATA_MANGER_H
