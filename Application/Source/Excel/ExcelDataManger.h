#ifndef EXCEL_DATA_MANGER_H
#define EXCEL_DATA_MANGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class InsertData {
public:
    InsertData(const QString& tcName, const QString& vehicleType, const QString& config, const QString& resultName,
               const QString& caseName, const QPair<QStringList, QStringList>& inputList) {
        updateInsertData(tcName, vehicleType, config, resultName, caseName, inputList);
    }
    InsertData() = default;
    InsertData(const InsertData& other) = default;
    InsertData& operator=(const InsertData& other) = default;

    bool operator==(const InsertData& other) const {
        return ((mTCName == other.mTCName) && (mVehicleType == other.mVehicleType) && (mConfig == other.mConfig) &&
                (mResultName == other.mResultName) && (mCaseName == other.mCaseName) && (mInputList == other.mInputList));
    }
    bool operator!=(const InsertData& other) const {
        return !(*this == other);
    }

    QString isTCName() const {
        return mTCName;
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

private:
    void updateInsertData(const QString& tcName, const QString& vehicleType, const QString& config, const QString& resultName,
                          const QString& caseName, const QPair<QStringList, QStringList>& inputList) {
        mTCName = tcName;
        mVehicleType = vehicleType;
        mConfig = config;
        mResultName = resultName;
        mCaseName = caseName;
        mInputList = inputList;
    }

private:
    QString mTCName;
    QString mVehicleType;
    QString mConfig;
    QString mResultName;
    QString mCaseName;
    QPair<QStringList, QStringList> mInputList;
};

class ExcelDataManger : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, MergeStart, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, Merge, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, MergeEnd, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MergeInfos, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(bool, ReadStateNewData, true, false)
    REGISTER_WRITABLE_PROPERTY_LIST(QList, InsertData, NewSheetData, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QStringList, ExcelSheetData, false)

public:
    static QSharedPointer<ExcelDataManger>& instance();

    QList<QStringList> isSheetDataInfo();
    QStringList isTCNameDataList();
    QString isConfigData(const QString& tcName);
    QStringList isResultDataList(const QString& tcName);
    QStringList isCaseDataList(const QString& tcName, const QString& resultName);
    QPair<QStringList, QStringList> isInputDataList(const QString& tcName, const QString& resultName, const QString& caseName);
    QList<QStringList> isOutputDataList(const QString& tcName, const QString& resultName);

    void updateExcelData(const QVariantList& sheetData);
    void updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputList);
    void insertCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                            const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName,
                            const bool& insertBefore);
    void clear();

private:
    explicit ExcelDataManger();

    QMap<int, QStringList> isConvertedExcelData();
    QStringList isContainsMergeData(const int& columnIndex, const QPair<QStringList, QStringList> dataList);
    QStringList isExcelSheetData(const int& columnIndex);
    QPair<int, int> isIndexOf(const QStringList& dataList, const QString& foundStr);
    QStringList isParsingDataList(const QStringList& data, const bool& removeWhitespace = false);
    QPair<int, int> isRowIndexInfo(const QString& tcName, const QString& resultName, const QString& caseName);
    QString isVehicleTypeData(const QString& tcName);
    int isCaseIndex(const QString& tcName, const QString& resultName, const QString& caseName);
};


#endif  // EXCEL_DATA_MANGER_H
