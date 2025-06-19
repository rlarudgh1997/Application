#ifndef EXCEL_DATA_MANAGER_H
#define EXCEL_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

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
    InsertData(const int& sheetIndex, const int& rowIndex, const QString& tcName, const QString& check, const QString& genType,
               const QString& vehicleType, const QString& config, const QString& resultName, const QString& caseName,
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
#if 0
        qDebug() << "   InsertData :" << sheetIndex;
        qDebug() << "\t Info   :" << rowIndex << check << tcName << genType << vehicleType << config << resultName << caseName
                 << inputList.first.size() << outputList.size();
        for (int index = 0; index < inputList.first.size(); ++index) {
            qDebug() << "\t Input  :" << inputList.first.at(index) << inputList.second.at(index);
        }
        for (const auto& outputData : outputList) {
            qDebug() << "\t Output :" << outputData.at(0) << outputData.at(1) << outputData.at(2);
        }
#endif
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

class ManualData {
    REGISTER_WRITABLE_VALUE(QString, TCName, QString())
    REGISTER_WRITABLE_VALUE(QString, VehicleType, QString())
    REGISTER_WRITABLE_VALUE(bool, RunnableOpt, false)
    REGISTER_WRITABLE_VALUE(bool, ConfigOpt, false)
    REGISTER_WRITABLE_VALUE(bool, CycleOption, false)
    REGISTER_WRITABLE_VALUE(int, CyclePeriod, 0)
    REGISTER_WRITABLE_VALUE(int, CycleDelta, 0)
    REGISTER_WRITABLE_VALUE(int, CycleDuration, 0)
    REGISTER_WRITABLE_VALUE(QString, CycleMode, QString())
    REGISTER_WRITABLE_VALUE(QPairStrList, PreconditionList, QPairStrList())
    REGISTER_WRITABLE_VALUE(QPairStrList, InputList, QPairStrList())
    REGISTER_WRITABLE_VALUE(QPairStrList, InitList, QPairStrList())
    REGISTER_WRITABLE_VALUE(QPairStrList, OutputList, QPairStrList())

public:
    ManualData(const QString& tcName, const QString& vehicleType, const bool& runnableOpt, const bool& configOpt,
               const bool& cycleOption, const int& cyclePeriod, const int& cycleDelta, const int& cycleDuration,
               const QString& cycleMode, const QPairStrList& preconditionList, const QPairStrList& inputList,
               const QPairStrList& initList, const QPairStrList& outputList) {
        setTCName(tcName);
        setVehicleType(vehicleType);
        setRunnableOpt(runnableOpt);
        setConfigOpt(configOpt);
        setCycleOption(cycleOption);
        setCyclePeriod(cyclePeriod);
        setCycleDelta(cycleDelta);
        setCycleDuration(cycleDuration);
        setCycleMode(cycleMode);
        setPreconditionList(preconditionList);
        setInputList(inputList);
        setInitList(initList);
        setOutputList(outputList);
    }
    ManualData() = default;
    ManualData(const ManualData& other) = default;
    ManualData& operator=(const ManualData& other) = default;

    bool operator==(const ManualData& other) const {
        return ((getTCName() == other.getTCName()) && (getVehicleType() == other.getVehicleType()) &&
                (getRunnableOpt() == other.getRunnableOpt()) && (getConfigOpt() == other.getConfigOpt()) &&
                (getCycleOption() == other.getCycleOption()) && (getCyclePeriod() == other.getCyclePeriod()) &&
                (getCycleDelta() == other.getCycleDelta()) && (getCycleDuration() == other.getCycleDuration()) &&
                (getCycleMode() == other.getCycleMode()) && (getPreconditionList() == other.getPreconditionList()) &&
                (getInputList() == other.getInputList()) && (getInitList() == other.getInitList()) &&
                (getOutputList() == other.getOutputList()));
    }
    bool operator!=(const ManualData& other) const {
        return !(*this == other);
    }
};

class ExcelDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())
    REGISTER_WRITABLE_CONTAINER(QMap, int, QMapIntStrList, ExcelSheetData)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QList<InsertData>, InsertSheetData)

public:
    static QSharedPointer<ExcelDataManager>& instance();

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
    QList<QStringList> isConfigDataList(const QString& configName, const bool& allData = true, const bool& removeMerge = false);
    QPair<QStringList, QStringList> isDependentDataList(const QString& tcName, const QString& resultName);
    QList<ManualData> isManualDataList();

    void reloadExcelData(const int& sheetIndex = 0);
    void writeExcelSheetData(const int& sheetIndex = 0);
    void updateInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName, const QString& caseName,
                             const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName = QString(),
                             const bool& insertBefore = false);
    void updateOutputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                              const QList<QStringList>& outputList);
    bool isValidConfigCheck(const QString& configName, const QMap<QString, QString>& inputList);

private:
    explicit ExcelDataManager();

    QVariantList isSheetDataInfo(const int& sheetIndex, const bool& appendMergeInfo = true);
    QMap<int, QStringList> isConvertedExcelData(const int& sheetIndex, const bool& appendMergeInfo);
    QStringList isOriginSheetData(const int& sheetIndex, const int& columnIndex);
    QStringList isInsertSheetData(const int& sheetIndex, const int& columnIndex);
    QStringList isExcelSheetData(const int& sheetIndex, const int& columnIndex, const bool& origin, const bool& log = false);
    QPair<int, int> isIndexOf(const QStringList& dataList, const QString& foundStr);
    QStringList isParsingDataList(const QStringList& data, const bool& removeWhitespace);
    QPair<int, int> isRowIndexInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                   const QString& caseName, const bool& origin, const bool& log = false);
    QPair<QStringList, QStringList> isConvertedToEnum(const QStringList& signalList, const QStringList& dataList);
    int isCaseIndex(const int& sheetIndex, const QString& tcName, const QString& resultName, const QString& caseName);
    QMapIntStrList updateParsingExcelData(const int& sheetIndex, const QVariantList& sheetData);
    QPair<QStringList, QStringList> reconstructInputData(const QPair<QStringList, QStringList>& inputList,
                                                         const QList<QStringList>& outputList);
};

#endif  // EXCEL_DATA_MANAGER_H
