#include "AbstractHandler.h"
#include "ScreenInfo.h"

#include <QDebug>

AbstractHandler::AbstractHandler(const int& displayType, const QString& qml, const QString& objcetName) {
    mScreen = ScreenInfo::instance().data()->drawScreen(displayType, qml, objcetName);
}

bool AbstractHandler::init() {
    if (mUpdateState == false) {
        mUpdateState = true;

        initPropertyInfo();
        AbstractHandler::controlConnect(true);
        controlConnect(true);
    }
    return mUpdateState;
}

void AbstractHandler::controlConnect(const bool& state) {
    if (state) {
        connect(this, &AbstractHandler::signalUpdateDataModel,
                [=](const int& type, const QVariant& value, const bool& alwaysUpdate) {
                    QMetaObject::invokeMethod(this, "setProperty", Q_ARG(int, type), Q_ARG(QVariant, value),
                                              Q_ARG(bool, alwaysUpdate));
                });
    } else {
        disconnect(this);
    }
}

QQuickItem* AbstractHandler::getScreen() {
    // qDebug() << "AbstractHandler -> this :" << this << ", Screen :" << mScreen;
    return mScreen;
}

void AbstractHandler::timerEvent(QTimerEvent* event) {
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

void AbstractHandler::registerProperty(const int& type, const QString& name, const QVariant& value) {
    mProperty[type] = QPair<QString, QVariant>(name, value);
}

QVariant AbstractHandler::getProperty(const int& type) {
    return mProperty[type].second;
}

void AbstractHandler::setProperty(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (mProperty.contains(type) == false) {
        qDebug() << "[Not register property - info :" << type << "]\n\n";
        return;
    }

    if (mUpdateState) {
        bool update = (alwaysUpdate) ? (true) : (mProperty[type].second != value);
        if ((update) && (mScreen)) {
            QString name = mProperty[type].first;
            if (mScreen->setProperty(name.toStdString().c_str(), value)) {
                mProperty[type] = QPair<QString, QVariant>(name, value);
                emit signalPropertyChanged(type, value);
            }
        }
    }
}
