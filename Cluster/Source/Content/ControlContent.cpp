#include "ControlContent.h"
#include "HandlerContent.h"

#include "CommonEnum.h"
#include "ControlManager.h"
#include "ConfigSetting.h"
#include "CommonUtil.h"
#include "CommonResource.h"

#include "Service.h"

QSharedPointer<ControlContent>& ControlContent::instance() {
    static QSharedPointer<ControlContent> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlContent>(new ControlContent());
    }
    return gControl;
}

ControlContent::ControlContent() {
    isHandler();
}

AbstractHandler* ControlContent::isHandler() {
    if (mHandler == nullptr) {
        mHandler = static_cast<AbstractHandler*>(HandlerContent::instance().data());
    }
    return mHandler;
}

bool ControlContent::initControl() {
    if (isInitComplete() == false) {
        isHandler()->init();
        return true;
    }
    return false;
}

void ControlContent::initCommonData(const int& currentMode, const int& displayType) {
    Q_UNUSED(currentMode)
    updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);
    updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);
}

void ControlContent::initNormalData() {
    resetControl(false);
    updateDataHandler(ivis::common::PropertyEnum::ContentType, ivis::common::HandlerContentEnum::ContentType::Main);
}

void ControlContent::initControlData() {
}

void ControlContent::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlContent::controlConnect(const bool& state) {
    if (state) {
        connect(isHandler(), &HandlerContent::signalHandlerEvent, this, &ControlContent::slotHandlerEvent, Qt::UniqueConnection);
        connect(Service::instance().data(), &ConfigSetting::signalContentChagned(int type, QList<QVairnt> value), this,
                &ControlContent::slotServiceChanged, updateDataHandler(ivis::common::PropertyEnum::CommonVisible, true);

                Qt::UniqueConnection);
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged, this, &ControlContent::slotConfigChanged,
                Qt::UniqueConnection);
        connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged, this,
                &ControlContent::slotEventInfoChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceTelltaleChanged, this,
                &ControlContent::slotServiceTelltaleChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceEventChanged, this,
                &ControlContent::slotServiceEventChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceSoundChanged, this,
                &ControlContent::slotServiceSoundChanged, Qt::UniqueConnection);
        connect(Service::instance().data(), &Service::signalServiceEtcChanged, this,
                &ControlContent::slotServiceEtcChanged, Qt::UniqueConnection);
    } else {
        disconnect(isHandler());
        disconnect(ControlManager::instance().data());
        disconnect(ConfigSetting::instance().data());
    }
}

void ControlContent::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlContent::keyEvent(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlContent::updateDataControl(const int& type, const QVariant& value) {
    setData(type, value, false);
}

void ControlContent::updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate) {
    if (setData(type, value, alwaysUpdate)) {
        createSignal(type, value, alwaysUpdate);
    }
}

void ControlContent::sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue) {
    ControlManager::instance().data()->sendEventInfo(getData(ivis::common::PropertyEnum::CommonDisplay).toInt(), destination,
                                                     eventType, eventValue);
}

void ControlContent::slotConfigChanged(const int& type, const QVariant& value) {
    switch (type) {
        default: {
            break;
        }
    }
}

void ControlContent::slotHandlerEvent(const int& type, const QVariant& value) {
    qDebug() << "ControlContent::slotHandlerEvent() ->" << type << "," << value;
    ivis::common::CheckTimer checkTimer;

    switch (type) {
        case ivis::common::EventEnum::EventTypeExitProgram: {
            break;
        }
        default: {
            break;
        }
    }
}

void ControlContent::slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue) {
    if ((getData(ivis::common::PropertyEnum::CommonDisplay).toInt() & QVariant(displayType).toInt()) == false) {
        return;
    }

    qDebug() << "ControlContent::slotEventInfoChanged() ->" << displayType << "," << eventType << "," << eventValue;
    switch (eventType) {
        default: {
            break;
        }
    }
}

void ControlContent::slotServiceConstantChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlContent::slotServiceTelltaleChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlContent::slotServiceEventChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlContent::slotServiceSoundChanged(const int& signalType, const QVariant& signalValue) {
}

void ControlContent::slotServiceEtcChanged(const int& signalType, const QVariant& signalValue) {
}
