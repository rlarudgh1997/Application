#include "ControlMain.h"
#include "HandlerMain.h"
#include "CommonEnum.h"
#include "ConfigSetting.h"


QSharedPointer<ControlMain> ControlMain::instance() {
    static QSharedPointer<ControlMain> gControl;

    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlMain>(new ControlMain());
    }

    return gControl;
}

ControlMain::ControlMain() : mHandler(static_cast<AbstractHandler*>(HandlerMain::instance().data())) {
}

void ControlMain::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        mHandler->init();

        controlConnect(true);
        initDataCommon(currentMode, ScreenEnum::DisplayTypeMain);
        initDataModule();
    }
}

void ControlMain::initDataCommon(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay,           displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode,              currentMode);
    updateDataHandler(PropertyTypeEnum::PropertyTypeDepth,             ScreenEnum::DisplayDepthMain);

    updateDataHandler(PropertyTypeEnum::PropertyTypeListTest1,         {0, 1, 2, 3, 4});
    updateDataHandler(PropertyTypeEnum::PropertyTypeListTest2,         {"5", "6", "7", "8", "9"});

    QVariantList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toList();
    QStringList contextName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeContextName).toStringList();
    qDebug() << "sheetName :" << sheetName;
    qDebug() << "contextName :" << contextName;
}

void ControlMain::initDataModule() {
    resetControl(false);
}

void ControlMain::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlMain::controlConnect(const bool& state) {
    if (state) {
        connect(mHandler, &HandlerMain::signalHandlerEvent,
                this,     &ControlMain::slotHandlerEvent,
                Qt::UniqueConnection);
    } else {
        disconnect(mHandler);
    }
}

void ControlMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlMain::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlMain::updateDataHandler(const int& propertyType, const QVariant& value) {
    if (setData(propertyType, value)) {
        emit mHandler->signalUpdateDataModel(propertyType, value);
    }
}

void ControlMain::updateDataHandler(const int& propertyType, const QVariantList& value) {
    if (setData(propertyType, value)) {
        emit mHandler->signalUpdateDataModel(propertyType, getData(propertyType));
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
