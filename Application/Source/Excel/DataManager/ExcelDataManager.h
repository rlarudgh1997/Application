#ifndef EXCEL_DATA_MANAGER_H
#define EXCEL_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

// using QMapIntStringList = QMap<int, QStringList>;
using QPairStringList = QPair<QStringList, QStringList>;

class InsertData {
    REGISTER_WRITABLE_VALUE(QString, TCName, QString())
    REGISTER_WRITABLE_VALUE(QString, Check, QString())
    REGISTER_WRITABLE_VALUE(QString, GenType, QString())
    REGISTER_WRITABLE_VALUE(QString, VehicleType, QString())
    REGISTER_WRITABLE_VALUE(QString, Config, QString())
    REGISTER_WRITABLE_VALUE(QString, ResultName, QString())
    REGISTER_WRITABLE_VALUE(QString, CaseName, QString())
    REGISTER_WRITABLE_VALUE(QPairStringList, InputList, QPairStringList())
    REGISTER_WRITABLE_VALUE(QList<QStringList>, OutputList, QList<QStringList>())

public:
    InsertData(const QString& tcName, const QString& check, const QString& genType, const QString& vehicleType,
               const QString& config, const QString& resultName, const QString& caseName,
               const QPairStringList& inputList, const QList<QStringList>& outputList) {
        setTCName(tcName);
        setCheck(check);
        setGenType(genType);
        setVehicleType(vehicleType);
        setConfig(config);
        setResultName(resultName);
        setCaseName(caseName);
        setInputList(inputList);
        setOutputList(outputList);
    }
    InsertData() = default;
    InsertData(const InsertData& other) = default;
    InsertData& operator=(const InsertData& other) = default;

    bool operator==(const InsertData& other) const {
        return ((getTCName() == other.getTCName()) && (getCheck() == other.getCheck()) && (getGenType() == other.getGenType()) &&
                (getVehicleType() == other.getVehicleType()) && (getConfig() == other.getConfig()) &&
                (getResultName() == other.getResultName()) && (getCaseName() == other.getCaseName()) &&
                (getInputList() == other.getInputList()) && (getOutputList() == other.getOutputList()));
    }
    bool operator!=(const InsertData& other) const {
        return !(*this == other);
    }
};

class ExcelDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())
    REGISTER_WRITABLE_VALUE(bool, ReadStateNewData, true)
    REGISTER_WRITABLE_LIST(QList, InsertData, NewSheetData)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QStringList, ExcelDataOther)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QStringList, ExcelDataConfig)

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

    void initExcelData();
    void updateExcelData(const int& sheetIndex, const QVariantList& sheetData);
    void updateInputDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                             const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName = QString(),
                             const bool& insertBefore = false);
    void updateOutputDataInfo(const QString& tcName, const QString& resultName, const QList<QStringList>& list);
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
