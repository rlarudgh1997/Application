#ifndef CONTROL_MAIN_H
#define CONTROL_MAIN_H



#include "AbstractControl.h"




class ControlMain : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlMain> instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);


private:
    explicit ControlMain();

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
    virtual void slotHandlerEvent(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
};


#endif    // CONTROL_MAIN_H
