#ifndef ABSTRACT_CONTROL_H
#define ABSTRACT_CONTROL_H

#include <QObject>
#include <QTimerEvent>
#include <QKeyEvent>

#include "CommonDefine.h"

class AbstractControl : public QObject {
    Q_OBJECT

public:
    enum {
        AbstractDataInvalid = 9999,
        AbstractDataStart,
    };
    enum {
        AbstractTimerTypeInvalid = 0,
        AbstractTimerStart ,
    };

public:
    explicit AbstractControl();
    bool init(const int& currentMode);
    bool isInitComplete();
    int controlTimer(const int& timerType, const bool& start = false, const int& interval = 0);
    int getTimerId(const int& timerType);
    const QVariant getData(const int& propertyType);
    bool setData(const int& propertyType, const QVariant& value);

    virtual void keyInput(const int& inputType, const int& inputValue) = 0;


protected:
    void timerEvent(QTimerEvent *event);


private:
    virtual void initControl(const int& currentMode) = 0;
    virtual void controlConnect(const bool& state = true) = 0;
    virtual void initDataCommon(const int& currentMode, const int& displayType) = 0;
    virtual void initDataModule() = 0;
    virtual void resetControl(const bool& reset) = 0;
    virtual void timerFunc(const int& timerId) = 0;
    virtual void updateDataHandler(const int& propertyType, const QVariant& value) = 0;


signals:


private slots:
    virtual void slotHandlerEvent(const int& propertyType, const int& touchType) = 0;
    virtual void slotConfigChanged(const int& type, const QVariant& value) = 0;


private:
    QMap<int, int> mTimer = QMap<int, int>();
    QMap<int, QVariant> mData = QMap<int, QVariant>();
    bool mInit = false;
};

#endif   // ABSTRACT_CONTROL_H
