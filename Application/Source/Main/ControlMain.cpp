#include "ControlMain.h"
#include "HandlerMain.h"
#include "CommonEnum.h"


QSharedPointer<ControlMain> ControlMain::instance() {
    static QSharedPointer<ControlMain> gControl;

    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlMain>(new ControlMain());
    }

    return gControl;
}

ControlMain::ControlMain() {
}

void ControlMain::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        HandlerMain::instance().data()->init();
        controlConnect(true);

        initDataCommon(currentMode, ScreenEnum::DisplayTypeMain);
        initDataModule();
    }
}

void ControlMain::initDataCommon(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
}

void ControlMain::initDataModule() {
    resetControl(false);
}

void ControlMain::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMain::controlConnect(const bool& state) {
    if (state) {
        connect(HandlerMain::instance().data(), &HandlerMain::signalHandlerEvent,
                this,                           &ControlMain::slotHandlerEvent,
                Qt::UniqueConnection);
    } else {
        disconnect(HandlerMain::instance().data());
    }
}

void ControlMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlMain::keyInput(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlMain::updateDataHandler(const int& propertyType, const QVariant& value) {
    if (setData(propertyType, value)) {
        emit HandlerMain::instance().data()->signalUpdateDataModel(propertyType, value);
    }
}


void ControlMain::slotHandlerEvent(const int& propertyType, const int& touchType) {
    qDebug("\nControlMain::slotHandlerEvent(%d, %d)", propertyType, touchType);

    switch (propertyType) {
        default : {
            break;
        }
    }
}

void ControlMain::slotConfigChanged(const int& type, const QVariant& value) {
}
