#include "ControlHome.h"
#include "HandlerHome.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"



QSharedPointer<ControlHome>& ControlHome::instance() {
    static QSharedPointer<ControlHome> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlHome>(new ControlHome());
    }
    return gControl;
}

ControlHome::ControlHome() {
    isHandler();
}

AbstractHandler* ControlHome::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerHome::instance().data());
    }
    return mHandler;
}

bool ControlHome::initControl(const int& currentMode) {
    Q_UNUSED(currentMode)
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlHome::initCommonData(const int& currentMode) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, ivis::common::DisplayEnum::DisplayTypeHome);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlHome::initNormalData() {
    resetControl(false);
    updateDataHandler(ivis::common::PropertyEnum::HomeType, ivis::common::HandlerHomeEnum::HomeType::Main);
}

void ControlHome::initControlData() {
}

void ControlHome::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlHome::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(),                       &HandlerHome::signalHandlerEvent,
                this,                              &ControlHome::slotHandlerEvent,
                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
                this,                              &ControlHome::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
                this,                              &ControlHome::slotEventInfoChanged,
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

void ControlHome::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlHome::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlHome::resizeEvent(const int& width, const int& height) {
#if defined(USE_RESIZE_SIGNAL)
    Q_UNUSED(width)
    Q_UNUSED(height)
#else
    updateDataHandler(ivis::common::PropertyEnum::CommonRect, QSize(width, height));
#endif
}

void ControlHome::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlHome::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlHome::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(),
                                                        destination, eventType, eventValue);
}

void ControlHome::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default : {
            break;
        }
    }
}

void ControlHome::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlHome::slotHandlerEvent() ->" << type << "," << value;
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

void ControlHome::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlHome::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default : {
            break;
        }
    }
}