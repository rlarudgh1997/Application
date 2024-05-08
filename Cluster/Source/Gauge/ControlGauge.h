#ifndef CONTROL_GAUGE_H
#define CONTROL_GAUGE_H

#include "AbstractControl.h"

class ControlGauge : public AbstractControl {
    Q_OBJECT

private:
    enum {
        ControlGaugeTimerSpeed = AbstractControl::AbstractTimerStart,
        ControlGaugeTimerRpm,
    };

public:
    static QSharedPointer<ControlGauge>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);

private:
    explicit ControlGauge();

    qreal isGaugeAngle(const int& gaugeType, const QVariant& gaugeValue);
    void updateGaugeInfo(const int& dataType, const QVariant& dataValue);

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
    virtual void updateDataService(const int& type, const QVariant& value);

public slots:
    virtual void slotControlUpdate(const int& type, const QVariant& value = QVariant());
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);
    virtual void slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);

private:
    AbstractHandler* mHandler = nullptr;
    const qreal mDefaultAngle = (-120);
};

#endif  // CONTROL_GAUGE_H
