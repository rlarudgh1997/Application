#include "AbstractControl.h"

AbstractControl::AbstractControl() {
}

bool AbstractControl::init(const int& currentMode) {
    if (mInit == false) {
        initControl(currentMode);
        mInit = true;
    }

    return mInit;
}

bool AbstractControl::isInitComplete() {
    return mInit;
}

void AbstractControl::timerEvent(QTimerEvent *event) {
    timerFunc(event->timerId());
}

qint32 AbstractControl::controlTimer(const qint32 &timerType, const bool &start, const qint32 &interval) {
    if (mTimer[timerType]) {
        killTimer(mTimer[timerType]);
        mTimer[timerType] = 0;
    }

    if ((start) && (interval > 0)) {
        mTimer[timerType] = startTimer(interval);
    }

    return mTimer[timerType];
}

qint32 AbstractControl::getTimerId(const qint32 &timerType) {
    return mTimer[timerType];
}

const QVariant AbstractControl::getData(const int& propertyType) {
    return mData[propertyType];
}

bool AbstractControl::setData(const int& propertyType, const QVariant& value) {
    bool save = (mData[propertyType] != value);

    if (save) {
        mData[propertyType] = value;
    }

    if (propertyType >= AbstractDataStart) {
        save = false;
    }

    return save;
}


