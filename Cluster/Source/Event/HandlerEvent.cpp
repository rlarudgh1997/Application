#include "HandlerEvent.h"
#include "CommonEnum.h"

#define QML QString("qrc:/Event/Event.qml")
#define QML_OBJECT QString("dataModelEvent")  // DataModelEvent.qml : ojbectName

QSharedPointer<HandlerEvent>& HandlerEvent::instance() {
    static QSharedPointer<HandlerEvent> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerEvent>(new HandlerEvent());
    }
    return gHandler;
}

HandlerEvent::HandlerEvent() : AbstractHandler(ivis::common::DisplayEnum::DisplayTypeEvent, QML, QML_OBJECT) {
}

void HandlerEvent::initPropertyInfo() {
    // Common
    registerProperty(ivis::common::PropertyEnum::CommonDisplay, QString("display"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::CommonVisible, QString("visible"), QVariant(true));

    // Event
    registerProperty(ivis::common::PropertyEnum::EventType, QString("eventType"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::PopupColorType, QString("popupColorType"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::PopupColorStatus, QString("popupColorStatus"), QVariant(false));
    registerProperty(ivis::common::PropertyEnum::PopupInfo, QString("popupInfo"), QVariant());
}

void HandlerEvent::controlConnect(const bool& state) {
    if (state) {
    } else {
        disconnect(this);
    }
}

void HandlerEvent::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerEventTimerStart)) {
        // do nothing
    }
}
