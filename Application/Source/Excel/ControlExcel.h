#ifndef CONTROL_EXCEL_H
#define CONTROL_EXCEL_H

#include "AbstractControl.h"
#include "CommonUtil.h"
#include "CommonDefine.h"

class KeywordInfo {
public:
    KeywordInfo() {
    }
    KeywordInfo(const int& row, const int& column, const QString& text, const int& keyword, const QVariant& data)
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
    QVariant isData() const {
        return mData;
    }

private:
    int mRow = 0;
    int mColumn = 0;
    QString mText = QString();
    int mKeyword = 0;
    QVariant mData = QVariant();
};

class ControlExcel : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlExcel>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    explicit ControlExcel();

    void updateNodeAddress(const bool& all, const QStringList& list);
    void updateSheetData(const int& propertyType, const QVariantList& sheetData);
    void updateExcelSheet(const bool& excelOpen, const QVariant& dirPath);
    bool writeExcelSheet(const QVariant& filePath);
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
    QStringList isVsmFileInfo(const QString& vehicleName, const QStringList& specType);
    QMap<int, QStringList> isSignalDataInfo(const bool& isDataType, const QString& signalName,
                                            const QMap<int, QStringList>& fileList);
    QMap<int, QStringList> isSignalFileList(const QString& signalName, const QString& vehicleType);
    void updateAutoCompleteSignal(const QVariantList& inputData);
    void updateAutoCompleteEtc(const QVariantList& inputData);
    void updateAutoInputDescriptionInfo(const QVariantList& autoInputInfo);
    void updateExcelSheetEditInfo(const bool& tcNameEdit);
    void updateInputDataValidation(const QVariantList& cellDataInfo);
    void updateGenDataInfo(const bool& readData);
    QMap<QString, int> isKeywordPatternInfo(const int& columnIndex);
    int isKeywordType(const int& columnIndex, QString& signalName);
    QMap<int, QList<KeywordInfo>> isKeywordTypeInfo();
    bool replaceGenDataInfo();
    void constructGenDataInfo();

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
