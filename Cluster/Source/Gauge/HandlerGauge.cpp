#include "HandlerGauge.h"
#include "CommonEnum.h"


#define QML          QString("qrc:/Gauge/Gauge.qml")
#define QML_OBJECT   QString("dataModelGauge")   // DataModelGauge.qml : ojbectName


QSharedPointer<HandlerGauge>& HandlerGauge::instance() {
    static QSharedPointer<HandlerGauge> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerGauge>(new HandlerGauge());
    }
    return gHandler;
}

HandlerGauge::HandlerGauge() : AbstractHandler(ivis::common::DisplayEnum::DisplayTypeGauge, QML, QML_OBJECT) {
}

void HandlerGauge::initPropertyInfo() {
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

    // Gauge
    registerProperty(ivis::common::PropertyEnum::GaugeType,
                                                QString("gaugeType"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeed,
                                                QString("gaugeSpeed"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpm,
                                                QString("gaugeRpm"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeFuel,
                                                QString("gaugeFuel"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeTemperature,
                                                QString("gaugeTemperature"),
                                                QVariant(0));


    registerProperty(ivis::common::PropertyEnum::GaugeSpeedAngle,
                                                QString("gaugeSpeedAngle"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpmAngle,
                                                QString("gaugeRpmAngle"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeFuelAngle,
                                                QString("gaugeFuelAngle"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeTemperatureAngle,
                                                QString("gaugeTemperatureAngle"),
                                                QVariant(0));

    registerProperty(ivis::common::PropertyEnum::GaugeSpeedUnit,
                                                QString("gaugeSpeedUnit"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedState,
                                                QString("gaugeSpeedState"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedSubDigitalState,
                                                QString("gaugeSpeedSubDigitalState"),
                                                QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpmState,
                                                QString("gaugeRpmState"),
                                                QVariant(0));
}

void HandlerGauge::controlConnect(const bool& state) {
    if (state) {
    } else {
        disconnect(this);
    }
}

void HandlerGauge::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerTelltaleTimerStart)) {
        // do nothing
    }
}
