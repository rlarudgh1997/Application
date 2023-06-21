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

int AbstractControl::controlTimer(const int &timerType, const bool &start, const int &interval) {
    if (mTimer[timerType]) {
        killTimer(mTimer[timerType]);
        mTimer[timerType] = 0;
    }

    if ((start) && (interval > 0)) {
        mTimer[timerType] = startTimer(interval);
    }

    return mTimer[timerType];
}

int AbstractControl::getTimerId(const int &timerType) {
    return mTimer[timerType];
}

const QVariant AbstractControl::getData(const int& type) {
    return mData[type];
}

bool AbstractControl::setData(const int& type, const QVariant& value) {
    if ((mData[type] != value)) {
        mData[type] = value;
        return true;
    }

    return false;
}


