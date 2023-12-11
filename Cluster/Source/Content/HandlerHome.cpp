#include "HandlerContent.h"
#include "CommonEnum.h"

#define QML QString("qrc:/Content/Content.qml")
#define QML_OBJECT QString("dataModelContent")  // DataModelContent.qml : ojbectName

QSharedPointer<HandlerContent>& HandlerContent::instance() {
    static QSharedPointer<HandlerContent> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerContent>(new HandlerContent());
    }
    return gHandler;
}

HandlerContent::HandlerContent() : AbstractHandler(ivis::common::DisplayEnum::DisplayTypeContent, QML, QML_OBJECT) {
}

void HandlerContent::initPropertyInfo() {
    // Common
    registerProperty(ivis::common::PropertyEnum::CommonDisplay, QString("display"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::CommonVisible, QString("visible"), QVariant(true));
    registerProperty(ivis::common::PropertyEnum::CommonRect, QString("rect"), QVariant());
    registerProperty(ivis::common::PropertyEnum::CommonVehicleType, QString("vehicleType"), QVariant(0));

    // Content
    registerProperty(ivis::common::PropertyEnum::ContentType, QString("ContentType"), QVariant(0));
}

void HandlerContent::controlConnect(const bool& state) {
    if (state) {
    } else {
        disconnect(this);
    }
}

void HandlerContent::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerContentTimerStart)) {
        // do nothing
    }
}
