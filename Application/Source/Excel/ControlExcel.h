#ifndef CONTROL_EXCEL_H
#define CONTROL_EXCEL_H

#include "AbstractControl.h"
#include "CommonUtil.h"
#include "CommonDefine.h"

class KeywordInfo {
public:
    KeywordInfo() {
    }
    KeywordInfo(const int& row, const int& column, const QString& text, const int& keyword, const QString& data)
        : mRow(row), mColumn(column), mText(text), mKeyword(keyword), mData(data) {
    }
    int isRow() const {
        return mRow;
    }
    int isColumn() const {
        return mColumn;
    }
    QString isText() const {
        return mText;
    }
    int isKeyword() const {
        return mKeyword;
    }
    QString isData() const {
        return mData;
    }
    QList<QStringList> isRowData() const {
        return mRowData;
    }
    void updateRowData(const QList<QStringList>& rowData) {
        mRowData = rowData;
    }
    QList<QStringList> isConvertData() const {
        return mConvertData;
    }
    void updateConvertData(const QList<QStringList>& convertData) {
        mConvertData = convertData;
    }

private:
    int mRow = 0;
    int mColumn = 0;
    QString mText = QString();
    int mKeyword = 0;
    QString mData = QString();
    QList<QStringList> mConvertData = QList<QStringList>();
    QList<QStringList> mRowData = QList<QStringList>();
};

class SignalDataInfo {
public:
    SignalDataInfo() {
    }
    SignalDataInfo(const QString& signalName, const int& dataType, const int& initialize, const int& keywordType,
                   const QStringList& originData, const QStringList& convertData, const QStringList& valueEnum,
                   const QStringList& notUsedEnum, const QStringList& precondition)
        : mDataType(dataType),
          mInitialize(initialize),
          mKeywordType(keywordType),
          mOriginData(originData),
          mConvertData(convertData),
          mValueEnum(valueEnum),
          mNotUsedEnum(notUsedEnum),
          mPrecondition(precondition) {
    }
    int isDataType() const {
        return mDataType;
    }
    bool isInitialize() const {
        return mInitialize;
    }
    int isKeywordType() const {
        return mKeywordType;
    }
    QStringList isOriginData() const {
        return mOriginData;
    }
    QStringList isConvertData() const {
        return mConvertData;
    }
    QStringList isValueEnum() const {
        return mValueEnum;
    }
    QStringList isNotUsedEnum() const {
        return mNotUsedEnum;
    }
    QStringList isPrecondition() const {
        return mPrecondition;
    }

private:
    int mDataType = 0;
    bool mInitialize = false;
    int mKeywordType = 0;
    QStringList mOriginData = QStringList();
    QStringList mConvertData = QStringList();
    QStringList mValueEnum = QStringList();
    QStringList mNotUsedEnum = QStringList();
    QStringList mPrecondition = QStringList();
};

class SheetConfigurationInfo {
public:
    SheetConfigurationInfo(const QString& tcName, const QString& result, const QStringList& cases) {
        updateSheetConfigurationInfo(tcName, result, cases);
    }
    SheetConfigurationInfo() = default;
    SheetConfigurationInfo(const SheetConfigurationInfo& other) = default;
    SheetConfigurationInfo& operator=(const SheetConfigurationInfo& other) = default;

    bool isSheetContainsCases(const QStringList& contatinsList) {
        QStringList caseList;
        for (const auto& sheetData : mSheetData) {
            // QString tcName = sheetData.first;
            // QString result = sheetData.second.first;
            caseList.append(sheetData.second.second);
        }
        for (const auto& str : caseList) {
            for (const auto& compareStr : contatinsList) {
                if (str.compare(compareStr, Qt::CaseInsensitive) == false) {
                    return true;
                }
            }
        }
        return false;
    }
    QList<QPair<QString, QPair<QString, QStringList>>> isSheetDataInfo() {
        printData();
        return mSheetData;
    }
    void updateSheetConfigurationInfo(const QString& tcName, const QString& result, const QStringList& cases) {
        for (const auto& singleCase : cases) {
            updateSheetConfigurationInfo(tcName, result, singleCase);
        }
    }

private:
    void updateSheetConfigurationInfo(const QString& tcName, const QString& result, const QString& singleCase) {
        for (auto& pairInfo : mSheetData) {
            if ((pairInfo.first == tcName) && (pairInfo.second.first == result)) {
                if (pairInfo.second.second.contains(singleCase) == false) {
                    pairInfo.second.second.append(singleCase);
                }
                return;
            }
        }
        mSheetData.append({tcName, {result, {singleCase}}});
    }
    void printData() const {
        qDebug() << "\n ----------------------------------------------------------------------------------------------------";
        for (const auto& sheetData : mSheetData) {
            QString tcName = sheetData.first;
            QString result = sheetData.second.first;
            QStringList caseList = sheetData.second.second;
            qDebug() << "SheetData :" << tcName << result << caseList;
        }
    }

private:
    QList<QPair<QString, QPair<QString, QStringList>>> mSheetData;
};

class ControlExcel : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlExcel>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

    QMap<QString, SignalDataInfo> isInputSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo,
                                                        const bool& caseRemove);
    QMap<QString, SignalDataInfo> isOutputSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo);
    QMap<QString, SignalDataInfo> isConfigSignalDataInfo(const int& sheetIndex, const QStringList& columnDataInfo);
    SheetConfigurationInfo isSheetConfigurationInfo(const int& sheetIndex);
    QString isIGNElapsedName(const int& ignType);
    QStringList isDescriptionDataInfo();
    QString isKeywordString(const int keywordType);

private:
    explicit ControlExcel();

    void updateNodeAddress(const bool& all, const QStringList& tcNameList, const QStringList& cofigNameList);
    void updateSheetData(const int& propertyType, const QVariantList& sheetData);
    void updateExcelSheet(const bool& excelOpen, const QVariant& dirPath);
    bool writeExcelSheet(const QVariant& filePath, const bool& backup);
    bool writeExcelFile(const QVariant& filePath);
    bool writeSheetInfo(const QVariant& filePath);
    QString sytemCall(const bool& readFile, const QVariant& filePath);
    bool checkPythonLibrary();
    bool openExcelFile(const QVariant& filePath);
    void loadExcelFile(const int& eventType);
    void saveExcelFile(const bool& saveAs);
    void updateClipboardInfo(const int& eventType);
    void updateShortcutInfo(const int& eventType);
    QString isSfcFileInfo(const QString& signalName);
    int isDataType(const QString& dataTypeStr);
    QPair<int, int> isIGNElapsedType(const QString& singalName);
    QPair<QStringList, QStringList> isConvertedIGNElapsedInfo(const QStringList& ignOriginData);
    QStringList isVsmFileInfo(const QString& vehicleName, const QStringList& specType);
    QMap<int, QStringList> isParsingFileDataInfo(const QString& signalName, const QStringList& inputData,
                                                 const QMap<int, QStringList>& fileList, int& dataType);
    QMap<int, QStringList> isSignalDataInfo(const QString& signalName, const QStringList& inputData, const QString& vehicleType,
                                            int& dataType);
    QMap<int, QStringList> isSignalFileList(const QString& signalName, const QString& vehicleType);
    QMap<int, QStringList> isTCNameDataInfo(const QString& tcName, const QString& result, const QList<int>& columnList,
                                            const bool& convert, const bool& mergeInfoErase, QList<QStringList>& convertData);
    QPair<int, int> isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2, const QString& input3,
                                      const bool& normal = true);
    QList<QStringList> isRowDataInfo(const int& sheetIndex, const QPair<int, int>& rowInfo, const QPair<int, int>& columnInfo);
    QList<QStringList> isDataInfo(const int& sheetIndex, const QString& tcName, const QString& result, const QString& caseInfo,
                                  const QPair<int, int>& columnInfo, const int& checkColumnIndex = 0);
    QList<QStringList> isInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& result,
                                       const QString& caseInfo);
    QList<QStringList> isOutputDataInfo(const int& sheetIndex, const QString& tcName, const QString& result);
    QList<QStringList> isConfigDataInfo(const int& sheetIndex, const QString& tcName, const QString& result);
    QList<QStringList> isConfigDataInfo(const QString& configName);
    QStringList isSignalValueEnum(const bool& toEnum, const QString& signalName);
    QStringList isConvertedSignalData(const bool& toEnum, const QString& signalName, const QStringList& valueEnum,
                                      QString& matchingValue);
    bool isPreconditionMaxValue(const QString& signalName, const int& dataType, const int& keywordType,
                                const QStringList& inputData, const QStringList& valueEnum);
    int isConvertedKeywordType(const bool& toCustom, const int& keywordType);
    int isSignalType(const QString& signalName);
    QString isCheckBothExceptionValue(const QMap<int, QStringList>& dataInfo, const QString& origintStr, const QString& checkStr);
    QPair<QStringList, QStringList> isCheckExceptionValueEnum(const QString& signalName, const QMap<int, QStringList>& dataInfo);
    QMap<QString, SignalDataInfo> isMatchingSignalDataInfo(const int& dataInfoType, const int& sheetIndex,
                                                           const QStringList& columnDataInfo);
    void updateAutoCompleteSignal(const QString& signalName, const QString& vehicleType, const int& columnIndex);
    void updateAutoCompleteTCName(const QString& signalName, const QString& vehicleType, const int& keywordType);
    void updateAutoCompleteSuggestions(const QVariantList& inputData);
    void updateAutoInputDescriptionInfo(const QVariantList& autoInputInfo);
    void updateAutoCompleteData(const QVariantList& inputData);
    void updateInputDataValidation(const QVariantList& cellDataInfo);
    void updateGenDataInfo(const int& eventType);
    QList<QPair<QString, int>> isKeywordPatternInfo(const int& columnIndex);
    int isKeywordType(const int& columnIndex, QString& inputData);
    QList<KeywordInfo> isKeywordTypeInfo(const int& sheetIndex, const QList<int>& inputColumnList);
    bool isExcelDataValidation();
    bool replaceGenDataInfo();

    QMap<int, QList<KeywordInfo>> constructKeywordTypeInfoList(const int& startSheetIndex, const int& endSheetIndex,
                                                               const QList<int>& columnList);
    bool isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                             const int& originSheetIndex, const bool& isEqualData);
    void constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    void constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList);
    void constructOutputConfigColumnDataInfo(const QList<int>& convertSheetIndexList);
    QList<QList<QStringList>> constructConvertConfigSignalSet(const QString& configName);
    bool appendConvertConfigSignalSet();
    bool appendConvertAllTCSignalSet();

    QStringList deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex);
    int getMergeKeywordType(const QString& data);
    QString constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName);

    inline QString constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const;

    void constrtuctSheetTest(const int& sheetIndex);

protected:
    virtual AbstractHandler* isHandler();
    virtual bool initControl();
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode, const int& displayType);
    virtual void initNormalData();
    virtual void initControlData();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataControl(const int& type, const QVariant& value);
    virtual void updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate = false);
    virtual void sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue = QVariant());

public slots:
    virtual void slotControlUpdate(const int& type, const QVariant& value = QVariant());
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);

private:
    AbstractHandler* mHandler = nullptr;
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess =
        QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(true), &QObject::deleteLater);
};

#endif  // CONTROL_EXCEL_H
