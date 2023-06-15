#ifndef CONTROL_MAIN_H
#define CONTROL_MAIN_H


#include "AbstractControl.h"

class AbstractHandler;



class ControlMain : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlMain> instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);


private:
    explicit ControlMain();


protected:
    virtual void initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initDataCommon(const int& currentMode, const int& displayType);
    virtual void initDataModule();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataHandler(const int& propertyType, const QVariant& value);
    virtual void updateDataHandler(const int& propertyType, const QVariantList& value);


public slots:
    virtual void slotHandlerEvent(const int& propertyType, const int& touchType);
    virtual void slotConfigChanged(const int& type, const QVariant& value);

private:
    AbstractHandler* mHandler = nullptr;
};


#endif    // CONTROL_MAIN_H
