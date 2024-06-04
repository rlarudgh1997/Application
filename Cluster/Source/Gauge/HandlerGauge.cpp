#include "HandlerGauge.h"
#include "CommonEnum.h"

#define QML QString("qrc:/Gauge/Gauge.qml")
#define QML_OBJECT QString("dataModelGauge")  // DataModelGauge.qml : ojbectName

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
    registerProperty(ivis::common::PropertyEnum::CommonDisplay, QString("display"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::CommonVisible, QString("visible"), QVariant(true));

    // Gauge
    registerProperty(ivis::common::PropertyEnum::GaugeDefaultAngle, QString("gaugeDefaultAngle"), QVariant(0));

    //// Speed_Gauge
    registerProperty(ivis::common::PropertyEnum::GaugeSpeed, QString("gaugeSpeed"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedAngle, QString("gaugeSpeedAngle"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedAngleState, QString("gaugeSpeedAngleState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedUnit, QString("gaugeSpeedUnit"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedState, QString("gaugeSpeedState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedSubDigitalState, QString("gaugeSpeedSubDigitalState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedSubDigital, QString("gaugeSpeedSubDigital"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedAuxDisplayUnit, QString("gaugeSpeedAuxDisplayUnit"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedSubDisplayState, QString("gaugeSpeedSubDisplayState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedScaleMaxState, QString("gaugeSpeedScaleMaxState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitState, QString("gaugeSpeedNaviSpeedLimitState"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitOver1ColorValue,
                     QString("gaugeSpeedNaviSpeedLimitOver1ColorValue"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeSpeedNaviSpeedLimitOver2ColorValue,
                     QString("gaugeSpeedNaviSpeedLimitOver2ColorValue"), QVariant(0));

    //// Tachometer(Rpm Gauge)
    registerProperty(ivis::common::PropertyEnum::GaugeRpmState, QString("gaugeRpmState"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpm, QString("gaugeRpm"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpmAngle, QString("gaugeRpmAngle"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRedZoneExceptNbrandStat, QString("gaugeRedZoneExceptNbrandStat"),
                     QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRedZoneNbrandStat, QString("gaugeRedZoneNbrandStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeMaxRpmStat, QString("gaugeMaxRpmStat"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeRpmDampStat, QString("gaugeRpmDampStat"), QVariant(0));

    //// Fuel
    registerProperty(ivis::common::PropertyEnum::GaugeFuel, QString("gaugeFuel"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeFuelAngle, QString("gaugeFuelAngle"), QVariant(0));

    //// Temperature
    registerProperty(ivis::common::PropertyEnum::GaugeTemperature, QString("gaugeTemperature"), QVariant(0));
    registerProperty(ivis::common::PropertyEnum::GaugeTemperatureAngle, QString("gaugeTemperatureAngle"), QVariant(0));
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
