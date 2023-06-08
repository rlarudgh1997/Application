#include "AbstractHandler.h"
#include "ScreenInfo.h"

//#include <QDebug>


AbstractHandler::AbstractHandler(const int& displayType, const QString& objcetName, const bool& show) {
    mWidget = ScreenInfo::instance().data()->drawDisplay(displayType, objcetName, show);
}

bool AbstractHandler::init() {
    if (mUpdateState == false) {
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


void AbstractHandler::timerEvent(QTimerEvent *event) {
    timerFunc(event->timerId());
}

QWidget* AbstractHandler::getComponent() {
//    qDebug() << "getComponent : " << mWidget;
    return mWidget;
}

qint32 AbstractHandler::controlTimer(const qint32& timerType, const bool& start, const qint32& interval) {
    if (mTimer[timerType]) {
        killTimer(mTimer[timerType]);
        mTimer[timerType] = AbstractTimerTypeInvalid;
    }

    if (start) {
        mTimer[timerType] = startTimer(interval);
    }

    return mTimer[timerType];
}

qint32 AbstractHandler::getTimerId(const qint32& timerType) {
    return mTimer[timerType];
}

void AbstractHandler::registerProperty(const qint32& propertyType, const QVariant& value) {
    setProperty(propertyType, value);
}

QVariant AbstractHandler::getProperty(const qint32& propertyType) {
    return mProperty[propertyType];
}

void AbstractHandler::setProperty(const qint32& propertyType, const QVariant& value) {
    if (mUpdateState) {
        if (mProperty[propertyType] != value) {
            mProperty[propertyType] = value;
        }
    }
}



// Function - SLOT
void AbstractHandler::slotUpdateDataModel(const int& propertyType, const QVariant& value) {
    // qDebug("QmlHandlerHome::slotUpdateDataModel(%d, %s)", dataType, value.toString().toLatin1().data());
    QMetaObject::invokeMethod(this, "setProperty", Q_ARG(qint32, propertyType), Q_ARG(QVariant, value));
}


