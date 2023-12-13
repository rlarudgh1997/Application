#include "Service.h"

#include <regex>
#include <QDebug>

#define USE_SERVICE_CODE

QSharedPointer<Service>& Service::instance() {
    static QSharedPointer<Service> gService;
    if (gService.isNull()) {
        gService = QSharedPointer<Service>(new Service());
    }
    return gService;
}

Service::Service() {
    qDebug() << "Service - Start";
    init();
}

HVehicleSignalModel& Service::getVehicleSignalModel() {
    static HVehicleSignalModel* gVehicleSignalModel = nullptr;
    if (gVehicleSignalModel == nullptr) {
        gVehicleSignalModel = new HVehicleSignalModel;
    }
    return *gVehicleSignalModel;
}

void Service::init() {
    qDebug() << "Service - Init";

    getVehicleSignalModel();

    subscribeConstant();
    subscribeTelltale();
    subscribeEvent();
    subscribeSound();
    subscribeEtc();
}

void Service::addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc) {
    std::vector<std::string> nodePaths{nodeAddress};
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    qDebug() << "addSubscription :" << ((result == ccos::HResult::OK) ? ("Sucess") : ("Fail"));
    if (result != ccos::HResult::OK) {
        qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
    }
}

void Service::addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc) {
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    qDebug() << "addSubscriptions :" << ((result == ccos::HResult::OK) ? ("Sucess") : ("Fail"));
    if (result != ccos::HResult::OK) {
        for (const std::string& nodeAddress : nodePaths) {
            qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
        }
    }
}

// ==================================================================================================================
// ==================================================================================================================
//    Constant
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isConstantSpeedGauge(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath  == SFC.Speed_Gauge.Constant.SpeedAnalog.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedAnalog.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Value.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedDigital.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedDigital.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Value.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedSubDigital.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value.value(vehicleSignal));
    } else if (nodePath  == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isConstantTachometer(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath  == SFC.Tachometer.Constant.Rpm.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.Rpm.Value.value(vehicleSignal));
    } else if (nodePath  == SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Tachometer.Constant.RedZoneNbrand.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RedZoneNbrand.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Tachometer.Constant.MaxRpm.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.MaxRpm.Stat.value(vehicleSignal));
    } else if (nodePath  == SFC.Tachometer.Constant.RpmDamp.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RpmDamp.Stat.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isConstantIntroOutro(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    // if (nodePath  == AAAAAAAAAAAAAAAA) {
    //     value = static_cast<ccos::HUInt64>(AAAAAAAAAAAAAAAA.value(vehicleSignal));
    // } else if (nodePath  == AAAAAAAAAAAAAAAA) {
    //     value = static_cast<ccos::HUInt64>(AAAAAAAAAAAAAAAA.value(vehicleSignal));
    // } else {
    //     value = QVariant();
    // }

    return value;
}

void Service::onConstantChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onConstantChanged :" << signalType << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Constant::ConstantSpeedGaugeStart) && (signalType < Constant::ConstantSpeedGaugeEnd)) {
            signalValue = isConstantSpeedGauge(signalType, vehicleSignal);
        } else if ((signalType > Constant::ConstantTachometerStart) && (signalType < Constant::ConstantTachometerEnd)) {
            signalValue = isConstantTachometer(signalType, vehicleSignal);
        } else if ((signalType > Constant::ConstantIntroOutroStart) && (signalType < Constant::ConstantIntroOutroEnd)) {
            signalValue = isConstantIntroOutro(signalType, vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceConstantChanged(signalType, signalValue);
    }
}

void Service::subscribeConstantSpeedGauge() {
    // SpeedAnalog
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedAnalogStat, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedAnalogValue, std::placeholders::_1));

    // SpeedDigital
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedDigitalStat, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedDigitalValue, std::placeholders::_1));

    // SpeedSubDigital
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedSubDigitalStat, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedSubDigitalValue, std::placeholders::_1));

    // SpeedMainDisplayUnit
    addSubscription(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedMainDisplayUnitStat, std::placeholders::_1));

    // SpeedAuxDisplayUnit
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedAuxDisplayUnitStat, std::placeholders::_1));

    // SpeedSubDisplay
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedSubDisplayStat, std::placeholders::_1));

    // SpeedScaleMaximum
    addSubscription(SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::SpeedScaleMaximumStat, std::placeholders::_1));

    // NaviSpeedLimit
    addSubscription(SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::NaviSpeedLimitStat, std::placeholders::_1));

    // NaviSpeedLimitOver1Color
    addSubscription(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::NaviSpeedLimitOver1ColorValue, std::placeholders::_1));

    // NaviSpeedLimitOver2Color
    addSubscription(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::NaviSpeedLimitOver2ColorValue, std::placeholders::_1));
}

void Service::subscribeConstantTachometer() {
    addSubscription(SFC.Tachometer.Constant.Rpm.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmValue, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmValue, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.RedZoneNbrand.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmValue, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.MaxRpm.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmValue, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.RpmDamp.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmValue, std::placeholders::_1));
}

void Service::subscribeConstantIntroOutron() {
}

void Service::subscribeConstant() {
    qDebug() << "Service - subscribeConstant";

    subscribeConstantSpeedGauge();
    subscribeConstantTachometer();
    subscribeConstantIntroOutron();
}

// ==================================================================================================================
// ==================================================================================================================
//    Telltale
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isTelltaleLampIndicator(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    // if (nodePath  == AAAAAAAAAAAAAAAA) {
    //     value = static_cast<ccos::HUInt64>(AAAAAAAAAAAAAAAA.value(vehicleSignal));
    // } else if (nodePath  == AAAAAAAAAAAAAAAA) {
    //     value = static_cast<ccos::HUInt64>(AAAAAAAAAAAAAAAA.value(vehicleSignal));
    // } else {
    //     value = QVariant();
    // }

    return value;
}


void Service::onTelltaleChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onTelltaleChanged :" << signalType << signalList.size();
    QVariant signalValue = QVariant();

//    for (const auto& vehicleSignal : signalList) {
//        if ((signalType > Telltale::ConstantSpeedGaugeStart) && (signalType < Telltale::ConstantSpeedGaugeEnd)) {
//            signalValue = isTelltaleLampIndicator(signalType, vehicleSignal);
//        // } else if ((signalType > Telltale::ConstantTachometerStart) && (signalType < Telltale::ConstantTachometerEnd)) {
//        //     signalValue = isTelltaleLampIndicator(signalType, vehicleSignal);
//        } else {
//            signalValue = QVariant();
//        }

//        if (signalValue.isValid()) {
//            break;
//        }
//    }

    if (signalValue.isValid()) {
        emit signalServiceTelltaleChanged(signalType, signalValue);
    }
}

void Service::subscribeTelltaleLampIndicator() {
}


void Service::subscribeTelltale() {
    qDebug() << "Service - subscribeTelltale";

    subscribeTelltaleLampIndicator();
}

// ==================================================================================================================
// ==================================================================================================================
//    Event
// ==================================================================================================================
// ==================================================================================================================
void Service::onEventChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEventChanged :" << signalType << signalList.size();
}

void Service::subscribeSoundLampIndicator() {
}

void Service::subscribeEvent() {
    qDebug() << "Service - subscribeEvent";

    subscribeSoundLampIndicator();
}

// ==================================================================================================================
// ==================================================================================================================
//    Sound
// ==================================================================================================================
// ==================================================================================================================
void Service::onSoundChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onSoundChanged :" << signalType << signalList.size();
}

void Service::subscribeSound() {
    qDebug() << "Service - subscribeSound";
}

// ==================================================================================================================
// ==================================================================================================================
//    Etc
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isEtcSpeedGauge(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedUnit) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedUnit.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH) {
        value = static_cast<ccos::HDouble>(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}
void Service::onEtcChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEtcChanged :" << signalType << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Etc::EtcSpeedGaugeStart) && (signalType < Etc::EtcSpeedGaugeEnd)) {
            signalValue = isEtcSpeedGauge(signalType, vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceEtcChanged(signalType, signalValue);
    }
}

void Service::subscribeEtcSpeedGauge() {
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedUnit,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedUnit, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedValueKPH, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedValueMPH, std::placeholders::_1));
}

void Service::subscribeEtc() {
    qDebug() << "Service - subscribeEtc";

    subscribeEtcSpeedGauge();
}
