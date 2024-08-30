#ifndef CONTROL_EXCEL_H
#define CONTROL_EXCEL_H

#include "AbstractControl.h"
#include "CommonUtil.h"
#include "CommonDefine.h"

class KeywordInfo {
public:
    KeywordInfo() {}
    KeywordInfo(int row, int column, QString text, int keyword) {
        mRow = row;
        mColumn = column;
        mText = text;
        mKeyword = keyword;
    }
    int isRow() {
        return mRow;
    }
    int isColumn() {
        return mColumn;
    }
    QString isText() {
        return mText;
    }
    int isKeyword() {
        return mKeyword;
    }

private:
    int mRow = 0;
    int mColumn = 0;
    QString mText = QString();
    int mKeyword = 0;
};

class ControlExcel : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlExcel>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    explicit ControlExcel();

    void updateNodeAddress(const bool& all, const QStringList& privateList, const QStringList& interList);
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
    void updateAutoCompleteInputData(const bool& sfcSignal, const int& outputState, const QString& vehicleType,
                                     QString& signalName);
    void updateAutoInputDescriptionInfo(const QVariantList& autoInputInfo);
    void updateInputDataValidation(const QString& signal, const QString& inputData);
    void updateGenDataInfo(const bool& readData);
    QMap<int, QList<KeywordInfo>> isKeywordInfo();
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
