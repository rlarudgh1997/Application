#include "HandlerHome.h"
#include "CommonEnum.h"


#define QML          QString("qrc:/Home/Home.qml")
#define QML_OBJECT   QString("dataModelHome")   // DataModelHome.qml : ojbectName


QSharedPointer<HandlerHome>& HandlerHome::instance() {
    static QSharedPointer<HandlerHome> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerHome>(new HandlerHome());
    }
    return gHandler;
}

HandlerHome::HandlerHome() : AbstractHandler(ivis::common::DisplayEnum::DisplayTypeHome, QML, QML_OBJECT) {
}

void HandlerHome::initPropertyInfo() {
    // Common
    registerProperty(ivis::common::PropertyEnum::CommonDisplay,
                                                QString("display"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::CommonVisible,
                                                QString("visible"),
                                                QVariant(true));
    registerProperty(ivis::common::PropertyEnum::CommonRect,
                                                QString("rect"),
                                                QVariant());
    registerProperty(ivis::common::PropertyEnum::CommonVehicleType,
                                                QString("vehicleType"),
                                                QVariant(0));

    // Home
    registerProperty(ivis::common::PropertyEnum::HomeType,
                                                QString("homeType"),
                                                QVariant(0));
}

void HandlerHome::controlConnect(const bool& state) {
    if (state) {
    } else {
        disconnect(this);
    }
}

void HandlerHome::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerHomeTimerStart)) {
        // do nothing
    }
}
