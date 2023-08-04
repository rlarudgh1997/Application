#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H



#include "AbstractControl.h"
#include "CommonUtil.h"



class ControlCenter : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlCenter>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);


private:
    explicit ControlCenter();
    void updateSheetInfo(const int& propertyType, const QVariant& dirPath);
    bool editSheetInfo(const QVariant& value);
    bool writeSheetInfo(const QVariant& filePath);
    QString sytemCall(const int& type, const QVariant& filePath);
    bool checkPythonLibrary();


protected:
    virtual AbstractHandler* isHandler();
    virtual void initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode, const int& displayType);
    virtual void initBaseData();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate = false);
    virtual void updateDataHandler(const int& type, const QVariantList& value, const bool& alwaysUpdate = false);
    virtual void sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue);


public slots:
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess =
            QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(true), &QObject::deleteLater);
};


#endif    // CONTROL_CENTER_H
