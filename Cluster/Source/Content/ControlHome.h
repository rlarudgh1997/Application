#ifndef CONTROL_CONTENT_H
#define CONTROL_CONTENT_H

#include "AbstractControl.h"

class ControlContent : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlContent>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);

private:
    explicit ControlContent();

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
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);
    virtual void slotServiceConstantChanged(const int& signalType, const QVariant& signalValue);
    virtual void slotServiceTelltaleChanged(const int& signalType, const QVariant& signalValue);
    virtual void slotServiceEventChanged(const int& signalType, const QVariant& signalValue);
    virtual void slotServiceSoundChanged(const int& signalType, const QVariant& signalValue);
    virtual void slotServiceEtcChanged(const int& signalType, const QVariant& signalValue);

private:
    AbstractHandler* mHandler = nullptr;
};

#endif  // CONTROL_CONTENT_H
