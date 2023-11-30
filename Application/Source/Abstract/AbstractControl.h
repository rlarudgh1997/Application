#ifndef ABSTRACT_CONTROL_H
#define ABSTRACT_CONTROL_H

#include <QObject>
#include <QSharedPointer>
#include <QTimerEvent>
#include <QKeyEvent>

#include "AbstractHandler.h"

class AbstractControl : public QObject {
    Q_OBJECT

public:
    enum {
        AbstractTimerTypeInvalid = 0,
        AbstractTimerStart,
    };

public:
    explicit AbstractControl();
    bool init(const int& currentMode, const int& displayType);
    bool isInitComplete();
    int controlTimer(const int& timerType, const bool& start = false, const int& interval = 0);
    int getTimerId(const int& timerType);
    const QVariant getData(const int& type);
    bool setData(const int& type, const QVariant& value, const bool& alwaysUpdate = false);
    bool createSignal(const int& type, const QVariant& value, const bool& alwaysUpdate);

    virtual void keyEvent(const int& inputType, const int& inputValue) = 0;
    virtual void resizeEvent(const int& width, const int& height) = 0;

protected:
    void timerEvent(QTimerEvent* event);

private:
    virtual AbstractHandler* isHandler() = 0;
    virtual bool initControl() = 0;
    virtual void controlConnect(const bool& state = true) = 0;
    virtual void initCommonData(const int& currentMode, const int& displayType) = 0;
    virtual void initNormalData() = 0;
    virtual void initControlData() = 0;
    virtual void resetControl(const bool& reset) = 0;
    virtual void timerFunc(const int& timerId) = 0;
    virtual void updateDataControl(const int& type, const QVariant& value) = 0;
    virtual void updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate = false) = 0;
    virtual void sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue = QVariant()) = 0;

private slots:
    virtual void slotConfigChanged(const int& type, const QVariant& value) = 0;
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) = 0;
    virtual void slotHandlerEvent(const int& type, const QVariant& value) = 0;

private:
    QMap<int, int> mTimer = QMap<int, int>();
    QMap<int, QVariant> mData = QMap<int, QVariant>();
    bool mInit = false;
};

#endif  // ABSTRACT_CONTROL_H
