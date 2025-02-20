#ifndef EXCEL_DATA_MANAGER_H
#define EXCEL_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

// #define USE_EXCEL_DATA_MANAGER_OLD

using QPairStrList = QPair<QStringList, QStringList>;
using QMapIntStrList = QMap<int, QStringList>;

class InsertData {
    REGISTER_WRITABLE_VALUE(QString, TCName, QString())
    REGISTER_WRITABLE_VALUE(QString, Check, QString())
    REGISTER_WRITABLE_VALUE(QString, GenType, QString())
    REGISTER_WRITABLE_VALUE(QString, VehicleType, QString())
    REGISTER_WRITABLE_VALUE(QString, Config, QString())
    REGISTER_WRITABLE_VALUE(QString, ResultName, QString())
    REGISTER_WRITABLE_VALUE(QString, CaseName, QString())
    REGISTER_WRITABLE_VALUE(QPairStrList, InputList, QPairStrList())
    REGISTER_WRITABLE_VALUE(QList<QStringList>, OutputList, QList<QStringList>())

public:
    InsertData(const QString& tcName, const QString& check, const QString& genType, const QString& vehicleType,
               const QString& config, const QString& resultName, const QString& caseName,
               const QPairStrList& inputList, const QList<QStringList>& outputList) {
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
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    REGISTER_WRITABLE_LIST(QList, InsertData, NewSheetData)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QStringList, ExcelDataOther)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QStringList, ExcelDataConfig)
#else
    REGISTER_WRITABLE_CONTAINER(QMap, int, QMapIntStrList, ExcelSheetData)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QMapIntStrList, ExcelSheetDataOrigin)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QMapIntStrList, ExcelSheetDataConvert)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QList<InsertData>, InsertSheetData)
#endif

public:
    static QSharedPointer<ExcelDataManager>& instance();

    QList<QStringList> isSheetDataInfo(const int& sheetIndex);
    QStringList isTCNameDataList(const int& sheetIndex, const bool& all);
    bool isCheckData(const int& sheetIndex, const QString& tcName);
    int isGenTypeData(const int& sheetIndex, const QString& tcName, QString& genTypeStr);
    QString isVehicleTypeData(const int& sheetIndex, const QString& tcName);
    QString isConfigData(const int& sheetIndex, const QString& tcName);
    QStringList isResultDataList(const int& sheetIndex, const QString& tcName);
    QStringList isCaseDataList(const int& sheetIndex, const QString& tcName, const QString& resultName);
    QPair<QStringList, QStringList> isInputDataList(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                                    const QString& caseName, const bool& removeWhitespace,
                                                    const bool& checkOthers = true);
    QPair<QStringList, QStringList> isInputDataWithoutCaseList(const int& sheetIndex, const QString& tcName,
                                                               const QString& resultName, const QString& caseName);
    QList<QStringList> isOutputDataList(const int& sheetIndex, const QString& tcName, const QString& resultName);
    QList<QStringList> isConfigDataList(const QString& configName, const bool& allData = true);

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    void updateExcelData(const int& sheetIndex, const QVariantList& sheetData);
#else
    void resetExcelData(const bool& convertState);
#endif
    void updateInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName, const QString& caseName,
                             const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName = QString(),
                             const bool& insertBefore = false);
    void updateOutputDataInfo(const QString& tcName, const QString& resultName, const QList<QStringList>& list);
    bool isValidConfigCheck(const bool& other, const QString& configName, const QMap<QString, QString>& inputList);

private:
    explicit ExcelDataManager();

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    void updateExcelDataOther(const int& sheetIndex, const QVariantList& sheetData);
    void updateExcelDataConfig(const QVariantList& sheetData);
    QStringList isExcelDataOther(const int& sheetIndex, const int& columnIndex);
    QStringList isExcelDataConfig(const int& sheetIndex, const int& columnIndex);
#else
    QMapIntStrList updateParsingExcelData(const int& sheetIndex, const QVariantList& sheetData);
    QStringList isOriginSheetData(const int& sheetIndex, const int& columnIndex);
    QStringList isInsertSheetData(const int& sheetIndex, const int& columnIndex);
    QStringList isExcelSheetData(const int& sheetIndex, const int& columnIndex, const bool& origin);
#endif
    QMap<int, QStringList> isConvertedExcelData(const int& sheetIndex);
    QPair<int, int> isIndexOf(const QStringList& dataList, const QString& foundStr);
    QStringList isParsingDataList(const QStringList& data, const bool& removeWhitespace);
    QPair<int, int> isRowIndexInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                   const QString& caseName, const bool& origin);
    int isCaseIndex(const int& sheetIndex, const QString& tcName, const QString& resultName, const QString& caseName);
};

#endif  // EXCEL_DATA_MANAGER_H
