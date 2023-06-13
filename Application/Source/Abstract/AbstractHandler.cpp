#include "AbstractHandler.h"
#include "ScreenInfo.h"

#include <QDebug>

AbstractHandler::AbstractHandler(const int& displayType, const QString& objcetName, const bool& show) {
    mScreen = ScreenInfo::instance().data()->drawScreen(displayType, objcetName, show);
}

bool AbstractHandler::init() {
    if (mUpdateState == false) {
        AbstractHandler::controlConnect(true);
        mUpdateState = true;

        initPropertyInfo();
        controlConnect(true);
    }
    return mUpdateState;
}

void AbstractHandler::controlConnect(const bool& state) {
    if (state) {
        connect(this, SIGNAL(signalUpdateDataModel(const int&, const QVariant&)),
                this, SLOT(slotUpdateDataModel(const int&, const QVariant&)),
                Qt::UniqueConnection);
    } else {
        disconnect(this);
    }
}

QWidget* AbstractHandler::getScreen() {
    // qDebug() << "AbstractHandler::getScreen :" << mScreen;
    return mScreen;
}

void AbstractHandler::timerEvent(QTimerEvent *event) {
    timerFunc(event->timerId());
}

int AbstractHandler::controlTimer(const int& timerType, const bool& start, const int& interval) {
    if (mTimer[timerType]) {
        killTimer(mTimer[timerType]);
        mTimer[timerType] = AbstractTimerTypeInvalid;
    }

    if (start) {
        mTimer[timerType] = startTimer(interval);
    }

    return mTimer[timerType];
}

int AbstractHandler::getTimerId(const int& timerType) {
    return mTimer[timerType];
}

void AbstractHandler::registerProperty(const int& propertyType, const QVariant& value) {
    setProperty(propertyType, value);
}

QVariant AbstractHandler::getProperty(const int& propertyType) {
    return mProperty[propertyType];
}

void AbstractHandler::setProperty(const int& propertyType, const QVariant& value) {
    if (mUpdateState) {
        if (mProperty[propertyType] != value) {
            mProperty[propertyType] = value;
            emit signalPropertyChanged(propertyType, value);
        }
    }
}

// Function - SLOT
void AbstractHandler::slotUpdateDataModel(const int& propertyType, const QVariant& value) {
    QMetaObject::invokeMethod(this, "setProperty", Q_ARG(int, propertyType), Q_ARG(QVariant, value));
    // qDebug() << "Property :" << getProperty(propertyType);
}


