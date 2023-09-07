#ifndef CONTROL_MENU_H
#define CONTROL_MENU_H

#include "AbstractControl.h"

// #include <QLoggingCategory>
// Q_DECLARE_LOGGING_CATEGORY(C_TOP)




class ControlMenu : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlMenu>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);


private:
    explicit ControlMenu();


protected:
    virtual AbstractHandler* isHandler();
    virtual bool initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode);
    virtual void initNormalData();
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
};


#endif    // CONTROL_MENU_H
