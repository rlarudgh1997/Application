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

    void updateExcelSheet(const bool& excelOpen, const QVariant& dirPath);
    bool writeExcelSheet(const QVariant& filePath);
    void writeExcelFile(const QVariant& filePath);
    bool writeSheetInfo(const QVariant& filePath);
    QString sytemCall(const int& type, const QVariant& filePath);
    bool checkPythonLibrary();


protected:
    virtual AbstractHandler* isHandler();
    virtual bool initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode);
    virtual void initNormalData();
    virtual void initControlData();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataControl(const int& type, const QVariant& value);
    virtual void updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate = false);
    virtual void sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue = QVariant());


public slots:
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess =
            QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(true), &QObject::deleteLater);
};


#endif    // CONTROL_EXCEL_H
