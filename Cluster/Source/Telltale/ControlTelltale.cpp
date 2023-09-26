#include "ControlTelltale.h"
#include "HandlerTelltale.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"



QSharedPointer<ControlTelltale>& ControlTelltale::instance() {
    static QSharedPointer<ControlTelltale> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlTelltale>(new ControlTelltale());
    }
    return gControl;
}

ControlTelltale::ControlTelltale() {
    isHandler();
}

AbstractHandler* ControlTelltale::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerTelltale::instance().data());
    }
    return mHandler;
}

bool ControlTelltale::initControl(const int& currentMode) {
    Q_UNUSED(currentMode)
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlTelltale::initCommonData(const int& currentMode) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, ivis::common::DisplayEnum::DisplayTypeTelltale);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlTelltale::initNormalData() {
    resetControl(false);
    updateDataHandler(ivis::common::PropertyEnum::TelltaleType, 0);
}

void ControlTelltale::initControlData() {
}

void ControlTelltale::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlTelltale::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerTelltale::signalHandlerEvent,
                this,                              &ControlTelltale::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlTelltale::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlTelltale::slotEventInfoChanged,
                Qt::UniqueConnection);
#if defined(USE_RESIZE_SIGNAL)
        connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
                updateDataHandler(ivis::common::PropertyEnum::CommonRect, screenSize);
        });
#endif
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
    }
}

void ControlTelltale::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlTelltale::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlTelltale::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyEnum::CommonRect, QSize(width, height));
#endif
}

void ControlTelltale::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlTelltale::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlTelltale::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlTelltale::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default : {
            break;
        }
    }
}

void ControlTelltale::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlTelltale::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventEnum::EventTypeExitProgram : {
            break;
        }
        default : {
            break;
        }
    }
}

void ControlTelltale::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlTelltale::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default : {
            break;
        }
    }
}
