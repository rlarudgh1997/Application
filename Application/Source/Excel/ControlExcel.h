#ifndef CONTROL_EXCEL_H
#define CONTROL_EXCEL_H

#include "AbstractControl.h"
#include "CommonUtil.h"
#include "CommonDefine.h"

class ControlExcel : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlExcel>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    explicit ControlExcel();

    void updateNodeAddress(const bool& all, const QStringList& tcNameList, const QStringList& cofigNameList);
    void updateSheetData(const int& propertyType, const QVariantList& sheetData);
    void updateExcelSheet(const bool& excelOpen, const QVariant& dirPath);
    bool writeExcelFile(const QVariant& filePath);
    bool writeSheetInfo(const QVariant& filePath);
    bool openExcelFile(const QVariant& filePath);
    bool updateExcelDataInfo(const QString& filePath);
    void loadExcelFile(const int& eventType);
    void saveExcelFile(const bool& saveAs);
    void updateClipboardInfo(const int& eventType);
    void updateShortcutInfo(const int& eventType);
    void updateGenType(const int& genType);
    void updateTCCheck(const int& allCheck);
    void updateAutoCompleteSignal(const QString& signalName, const QString& vehicleType, const int& columnIndex);
    void updateAutoCompleteTCName(const QString& signalName, const QString& vehicleType, const int& keywordType);
    void updateAutoCompleteSuggestions(const QVariantList& inputData);
    void updateAutoInputDescriptionInfo(const QVariantList& autoInputInfo);
    void updateAutoCompleteData(const QVariantList& inputData);
    void updateInputDataValidation(const QVariantList& cellDataInfo);
    void updateGenDataInfo(const int& eventType);

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
    void slotTestCaseCompleted(const int& type, const bool& result);
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
