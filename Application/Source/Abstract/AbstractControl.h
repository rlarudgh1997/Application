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
    qint32 controlTimer(const qint32& timerType, const bool& start = false, const qint32& interval = 0);
    qint32 getTimerId(const qint32& timerType);
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
    QMap<qint32, qint32> mTimer = QMap<qint32, qint32>();
    QMap<int, QVariant> mData = QMap<int, QVariant>();
    bool mInit = false;
};

#endif   // ABSTRACT_CONTROL_H
