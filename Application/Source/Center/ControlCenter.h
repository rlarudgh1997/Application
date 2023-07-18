#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H



#include "AbstractControl.h"




class ControlCenter : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlCenter>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);


private:
    explicit ControlCenter();


protected:
    virtual AbstractHandler* isHandler();
    virtual void initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode, const int& displayType);
    virtual void initBaseData();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataHandler(const int& type, const QVariant& value);
    virtual void updateDataHandler(const int& type, const QVariantList& value);


public slots:
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
};


#endif    // CONTROL_CENTER_H
