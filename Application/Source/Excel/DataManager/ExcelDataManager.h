#ifndef EXCEL_DATA_MANAGER_H
#define EXCEL_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class InsertData {
public:
    InsertData(const QString& tcName, const QString& check, const QString& genType, const QString& vehicleType,
               const QString& config, const QString& resultName, const QString& caseName,
               const QPair<QStringList, QStringList>& inputList, const QList<QStringList>& outputList = QList<QStringList>()) {
        mTCName = tcName;
        mCheck = check;
        mGenType = genType;
        mVehicleType = vehicleType;
        mConfig = config;
        mResultName = resultName;
        mCaseName = caseName;
        mInputList = inputList;
        mOutputList = outputList;
    }
    InsertData() = default;
    InsertData(const InsertData& other) = default;
    InsertData& operator=(const InsertData& other) = default;

    bool operator==(const InsertData& other) const {
        return ((mTCName == other.mTCName) && (mCheck == other.mCheck) && (mGenType == other.mGenType) &&
                (mVehicleType == other.mVehicleType) && (mConfig == other.mConfig) && (mResultName == other.mResultName) &&
                (mCaseName == other.mCaseName) && (mInputList == other.mInputList) && (mOutputList == other.mOutputList));
    }
    bool operator!=(const InsertData& other) const {
        return !(*this == other);
    }

    QString isTCName() const {
        return mTCName;
    }
    QString isCheck() const {
        return mCheck;
    }
    QString isGenType() const {
        return mGenType;
    }
    QString isVehicleType() const {
        return mVehicleType;
    }
    QString isConfig() const {
        return mConfig;
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
    QList<QStringList> isOutputList() const {
        return mOutputList;
    }

private:
    QString mTCName;
    QString mCheck;
    QString mGenType;
    QString mVehicleType;
    QString mConfig;
    QString mResultName;
    QString mCaseName;
    QPair<QStringList, QStringList> mInputList;
    QList<QStringList> mOutputList;
};

class ExcelDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())
    REGISTER_WRITABLE_PROPERTY(bool, ReadStateNewData, true, false)
    REGISTER_WRITABLE_PROPERTY_LIST(QList, InsertData, NewSheetData, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QStringList, ExcelDataOther, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QStringList, ExcelDataConfig, false)

public:
    static QSharedPointer<ExcelDataManager>& instance();

    QList<QStringList> isSheetDataInfo();
    QStringList isTCNameDataList(const bool& all);
    bool isCheckData(const QString& tcName);
    int isGenTypeData(const QString& tcName, QString& genTypeStr);
    QString isVehicleTypeData(const QString& tcName);
    QString isConfigData(const QString& tcName);
    QStringList isResultDataList(const QString& tcName);
    QStringList isCaseDataList(const QString& tcName, const QString& resultName);
    QPair<QStringList, QStringList> isInputDataList(const QString& tcName, const QString& resultName, const QString& caseName,
                                                    const bool& removeWhitespace, const bool& checkOthers = true);
    QPair<QStringList, QStringList> isInputDataWithoutCaseList(const QString& tcName, const QString& resultName,
                                                               const QString& caseName);
    QList<QStringList> isOutputDataList(const QString& tcName, const QString& resultName);
    QList<QStringList> isConfigDataList(const QString& configName, const bool& allData = true);

    void updateExcelData(const int& sheetIndex, const QVariantList& sheetData);
    void updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName = QString(),
                            const bool& insertBefore = false);
    bool isValidConfigCheck(const bool& other, const QString& configName, const QMap<QString, QString>& inputList);

private:
    explicit ExcelDataManager();

    void updateExcelDataOther(const QVariantList& sheetData);
    void updateExcelDataConfig(const QVariantList& sheetData);
    QMap<int, QStringList> isConvertedExcelData();
    QStringList isExcelDataOther(const int& columnIndex);
    QStringList isExcelDataConfig(const int& columnIndex);
    QPair<int, int> isIndexOf(const QStringList& dataList, const QString& foundStr);
    QStringList isParsingDataList(const QStringList& data, const bool& removeWhitespace);
    QPair<int, int> isRowIndexInfo(const QString& tcName, const QString& resultName, const QString& caseName);
    int isCaseIndex(const QString& tcName, const QString& resultName, const QString& caseName);
};

#endif  // EXCEL_DATA_MANAGER_H
