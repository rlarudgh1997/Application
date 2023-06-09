#include "AbstractHandler.h"
#include "ScreenInfo.h"


AbstractHandler::AbstractHandler(const int& displayType, const QString& objcetName, const bool& show) {
    mScreen = ScreenInfo::instance().data()->drawScreen(displayType, objcetName, show);
}

bool AbstractHandler::init() {
    if (mUpdateState == false) {
        mUpdateState = true;
        initPropertyInfo();
        drawDisplay(AbstractdrawDisplayMain);
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
    // qDebug("AbstractHandler::getScreen()->Screen : 0x%X", mScreen);
    return mScreen;
}


void AbstractHandler::drawDisplay(const int& drawType) {
    qDebug("AbstractHandler::drawDisplay(%d)", drawType);
    switch (drawType) {
        case AbstractdrawDisplayMain : {
            drawDisplayMain();
            break;
        }
        case AbstractdrawDisplayDepth1 : {
            drawDisplayDepth1();
            break;
        }
        case AbstractdrawDisplayDepth2 : {
            drawDisplayDepth2();
            break;
        }
        default : {
            break;
        }
    }
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
        }
    }
}



// Function - SLOT
void AbstractHandler::slotUpdateDataModel(const int& propertyType, const QVariant& value) {
    qDebug("AbstractHandler::slotUpdateDataModel(%d, %s)", propertyType, value.toString().toLatin1().data());
    QMetaObject::invokeMethod(this, "setProperty", Q_ARG(int, propertyType), Q_ARG(QVariant, value));
}


