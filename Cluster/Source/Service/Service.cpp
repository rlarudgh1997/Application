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
}

ccos::vehicle::vsm::HVehicleSignalModel& Service::getVehicleSignalModel() {
    static ccos::vehicle::vsm::HVehicleSignalModel* gVehicleSignalModel = nullptr;
    if (gVehicleSignalModel == nullptr) {
        gVehicleSignalModel = new ccos::vehicle::vsm::HVehicleSignalModel;
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
    auto subscription = std::make_shared<ccos::vehicle::vsm::HSubscription>(
        nodePaths, ccos::vehicle::vsm::HSubscriptionType::VALUE_CHANGED, std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    // qDebug() << "\t addSubscription :" << ((result == ccos::HResult::OK) ? ("Sucess ->") : ("Fail   ->"))
    //          << nodeAddress.c_str();
    if (result != ccos::HResult::OK) {
        qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
    }
}

void Service::addSubscriptionGroup(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc) {
    auto subscription = std::make_shared<ccos::vehicle::vsm::HSubscription>(
        nodePaths, ccos::vehicle::vsm::HSubscriptionType::VALUE_CHANGED, std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    // qDebug() << "\t addSubscriptionGroup :" << ((result == ccos::HResult::OK) ? ("Sucess") : ("Fail"));
    if (result != ccos::HResult::OK) {
        for (const std::string& nodeAddress : nodePaths) {
            qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
        }
    }
}

// void Service::addSubscriptionEventPopup() {
//     std::vector<std::string> nodePaths = sfc::searchAddress("SFC.*.Event.*");
//     auto subscription = std::make_shared<ccos::vehicle::vsm::HSubscription>(
//         nodePaths, ccos::vehicle::vsm::HSubscriptionType::VALUE_CHANGED, std::make_shared<VehicleListener>(handlingFunc));

//     auto result = getVehicleSignalModel().subscribe(subscription);

//     getVehicleSignalModel().subscribe(std::make_shared<HSubscription>(
//         addressList, HSubscriptionType::VALUE_CHANGED,
//         std::make_shared<sfc::SubscriptionListener>(std::bind(&AppService::onEventFunc, this, std::placeholders::_1))));

//     qDebug() << "\t addSubscriptionGroup :" << ((result == ccos::HResult::OK) ? ("Sucess") : ("Fail"));
//     if (result != ccos::HResult::OK) {
//         for (const std::string& nodeAddress : nodePaths) {
//             qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
//         }
//     }
// }

// ==================================================================================================================
// ==================================================================================================================
//    Constant
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isConstantSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Speed_Gauge.Constant.SpeedAnalog.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedAnalog.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedDigital.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedDigital.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedSubDigital.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isConstantTachometer(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Tachometer.Constant.Rpm.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.Rpm.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Tachometer.Constant.RedZoneNbrand.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RedZoneNbrand.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Tachometer.Constant.MaxRpm.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.MaxRpm.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Tachometer.Constant.RpmDamp.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Tachometer.Constant.RpmDamp.Stat.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isConstantIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Intro_Outro.Constant.ResvCharge.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.ResvCharge.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Intro_Outro.Constant.ResvClimate.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.ResvClimate.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Intro_Outro.Constant.PurificationAir.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.PurificationAir.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Intro_Outro.Constant.PurificationAir.Value) {
        value = static_cast<ccos::HDouble>(SFC.Intro_Outro.Constant.PurificationAir.Value.value(vehicleSignal));
    } else if (nodePath == SFC.Intro_Outro.Constant.CO2Reduction.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.CO2Reduction.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Intro_Outro.Constant.CO2Reduction.Value) {
        value = static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.CO2Reduction.Value.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isConstantOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    // if (nodePath == SFC.OAT.Constant.OutTempDisplay.Stat) {
    //     value = static_cast<ccos::HUInt64>(SFC.OAT.Constant.OutTempDisplay.Stat.value(vehicleSignal));
    // } else if (nodePath == SFC.OAT.Constant.OutTempDisplayUnit.Stat) {
    //     value = static_cast<ccos::HUInt64>(SFC.OAT.Constant.OutTempDisplayUnit.Stat.value(vehicleSignal));
    // } else {
    //     value = QVariant();
    // }

    return value;
}

QVariant Service::isConstantTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isConstantDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isConstantADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isConstantHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isConstantADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

void Service::onConstantChanged(const Constant& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onConstantChanged :" << static_cast<int>(signalType) << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Constant::SpeedGaugeStart) && (signalType < Constant::SpeedGaugeEnd)) {
            signalValue = isConstantSpeedGauge(vehicleSignal);
        } else if ((signalType > Constant::TachometerStart) && (signalType < Constant::TachometerEnd)) {
            signalValue = isConstantTachometer(vehicleSignal);
        } else if ((signalType > Constant::IntroOutroStart) && (signalType < Constant::IntroOutroEnd)) {
            signalValue = isConstantIntroOutro(vehicleSignal);
        } else if ((signalType > Constant::OATStart) && (signalType < Constant::OATEnd)) {
            signalValue = isConstantOAT(vehicleSignal);
        } else if ((signalType > Constant::TransmissionIndicatorStart) && (signalType < Constant::TransmissionIndicatorEnd)) {
            signalValue = isConstantTransmissionIndicator(vehicleSignal);
        } else if ((signalType > Constant::DriveModeStart) && (signalType < Constant::DriveModeEnd)) {
            signalValue = isConstantDriveMode(vehicleSignal);
        } else if ((signalType > Constant::ADASParkingNewStart) && (signalType < Constant::ADASParkingNewEnd)) {
            signalValue = isConstantADASParkingNew(vehicleSignal);
        } else if ((signalType > Constant::HighPerformanceForNBrandStart) &&
                   (signalType < Constant::HighPerformanceForNBrandEnd)) {
            signalValue = isConstantHighPerformanceForNBrand(vehicleSignal);
        } else if ((signalType > Constant::ADASDrivingNewStart) && (signalType < Constant::ADASDrivingNewEnd)) {
            signalValue = isConstantADASDrivingNew(vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceDataChanged(static_cast<int>(DataType::Constant), static_cast<int>(signalType), signalValue);
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
                    std::bind(&Service::onConstantChanged, this, Constant::RedZoneExceptNbrandStat, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.RedZoneNbrand.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RedZoneNbrandStat, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.MaxRpm.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::MaxRpmStat, std::placeholders::_1));
    addSubscription(SFC.Tachometer.Constant.RpmDamp.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::RpmDampStat, std::placeholders::_1));
}

void Service::subscribeConstantIntroOutro() {
    addSubscription(SFC.Intro_Outro.Constant.ResvCharge.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::ResvChargeStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Constant.ResvClimate.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::ResvClimateStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Constant.PurificationAir.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::PurificationAirStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Constant.PurificationAir.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::PurificationAirValue, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Constant.CO2Reduction.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::CO2ReductionStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Constant.CO2Reduction.Value,
                    std::bind(&Service::onConstantChanged, this, Constant::CO2ReductionValue, std::placeholders::_1));
}

void Service::subscribeConstantOAT() {
    addSubscription(SFC.OAT.Constant.OutTempDisplay.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::OutTempDisplayStat, std::placeholders::_1));
    addSubscription(SFC.OAT.Constant.OutTempDisplayUnit.Stat,
                    std::bind(&Service::onConstantChanged, this, Constant::OutTempDisplayUnitStat, std::placeholders::_1));
}

void Service::subscribeConstant() {
    qDebug() << "Service - subscribeConstant";

    subscribeConstantSpeedGauge();
    subscribeConstantTachometer();
    subscribeConstantIntroOutro();
    subscribeConstantOAT();
    // subscribeConstant();
    // subscribeConstant();
    // subscribeConstant();
    // subscribeConstant();
    // subscribeConstant();
}

// ==================================================================================================================
// ==================================================================================================================
//    Telltale
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isTelltaleLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Lamp_Indicator.Telltale.FrontFog.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.FrontFog.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.HighBeam.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.HighBeam.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.RearFog.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.RearFog.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.TailLamp.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TailLamp.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.Lamp_Indicator.Telltale.LowBeam.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.LowBeam.Stat.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isTelltaleOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.OAT.Telltale.IceWarn.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.OAT.Telltale.IceWarn.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.OAT.Telltale.IceWarn.StatOptional) {
        value = static_cast<ccos::HUInt64>(SFC.OAT.Telltale.IceWarn.StatOptional.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isTelltaleDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isTelltaleHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isTelltaleADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.ADAS_Driving_New.Telltale.HandsOnOff.Stat) {
        value = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.HandsOnOff.Stat.value(vehicleSignal));
    } else if (nodePath == SFC.ADAS_Driving_New.Telltale.HandsOnOff.StatOptional) {
        value = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.HandsOnOff.StatOptional.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

void Service::onTelltaleChanged(const Telltale& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onTelltaleChanged :" << static_cast<int>(signalType) << signalList.size();
    QMap<int, QVariant> signalValues = QMap<int, QVariant>();
    int type = static_cast<int>(signalType);

    for (const auto& vehicleSignal : signalList) {
        QVariant isValue = QVariant();
        if ((signalType > Telltale::LampIndicatorStart) && (signalType < Telltale::LampIndicatorEnd)) {
            isValue = isTelltaleLampIndicator(vehicleSignal);
        } else if ((signalType > Telltale::OATStart) && (signalType < Telltale::OATEnd)) {    // Group
            isValue = isTelltaleOAT(vehicleSignal);
        } else if ((signalType > Telltale::DriveModeStart) && (signalType < Telltale::DriveModeEnd)) {
            isValue = isTelltaleDriveMode(vehicleSignal);
        } else if ((signalType > Telltale::HighPerformanceForNBrandStart) &&
                (signalType < Telltale::HighPerformanceForNBrandEnd)) {
            isValue = isTelltaleHighPerformanceForNBrand(vehicleSignal);
        } else if ((signalType > Telltale::ADASDrivingNewStart) && (signalType < Telltale::ADASDrivingNewEnd)) {
            isValue = isTelltaleADASDrivingNew(vehicleSignal);
        } else {
        }

        signalValues[type] = isValue;
        type++;
    }

    QMapIterator<int, QVariant> iter(signalValues);
    while (iter.hasNext()) {
        iter.next();
        emit signalServiceDataChanged(static_cast<int>(DataType::Telltale), iter.key(), iter.value());
    }
    // if (signalValue.isValid()) {
    //     emit signalServiceDataChanged(static_cast<int>(DataType::Telltale), static_cast<int>(signalType), signalValue);
    // }
}

void Service::subscribeTelltaleLampIndicator() {
    addSubscription(SFC.Lamp_Indicator.Telltale.FrontFog.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::FrontFogStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.HighBeam.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::HighBeamStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.RearFog.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::RearFogStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.TailLamp.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::TailLampStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::TurnSignalLeftStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::TurnSignalRightStat, std::placeholders::_1));
    addSubscription(SFC.Lamp_Indicator.Telltale.LowBeam.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::LowBeamStat, std::placeholders::_1));
}

void Service::subscribeTelltaleOAT() {
#if 0
    addSubscription(SFC.OAT.Telltale.IceWarn.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::IceWarnStat, std::placeholders::_1));
    addSubscription(SFC.OAT.Telltale.IceWarn.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::IceWarnStatOptional, std::placeholders::_1));
#else
    addSubscriptionGroup(std::vector<std::string>({SFC.OAT.Telltale.IceWarn.Stat, SFC.OAT.Telltale.IceWarn.StatOptional}),
                     std::bind(&Service::onTelltaleChanged, this, Telltale::IceWarnStat, std::placeholders::_1));
#endif
}

void Service::subscribeTelltaleDriveMode() {
    addSubscription(SFC.Drive_Mode.Telltale.Drive_Mode.DMS.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::DMSStat, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Telltale.Drive_Mode.DMS.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::DMSStatOptional, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Telltale.Drive_Mode.Dimmed.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::DimmedStat, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Telltale.Drive_Mode.Dimmed.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::DimmedStatOptional, std::placeholders::_1));
}

void Service::subscribeTelltaleHighPerformanceForNBrand() {
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.CreepOff.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::CreepOffStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ShiftLightStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueA,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ShiftLightBlinkValueA, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueB,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ShiftLightBlinkValueB, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueBOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ShiftLightBlinkValueBOptional, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ShiftLightStatOptional, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.EnduranceRace.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::EnduranceRaceStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Telltale.EnduranceRace.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::EnduranceRaceStatOptional, std::placeholders::_1));
}

void Service::subscribeTelltaleADASDrivingNew() {
    addSubscription(SFC.ADAS_Driving_New.Telltale.FCA.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::FCAStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.ELK.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::ELKStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.LKA.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::LKAStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.HBA.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::HBAStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.LFA.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::LFAStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCALeft.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::LCALeftStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCARight.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::LCARightStat, std::placeholders::_1));
#if 0
    addSubscription(SFC.ADAS_Driving_New.Telltale.HandsOnOff.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::HandsOnOffStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Telltale.HandsOnOff.StatOptional,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::HandsOnOffStatOptional, std::placeholders::_1));
#else
    addSubscriptionGroup(std::vector<std::string>({SFC.ADAS_Driving_New.Telltale.HandsOnOff.Stat,
                                                   SFC.ADAS_Driving_New.Telltale.HandsOnOff.StatOptional}),
                         std::bind(&Service::onTelltaleChanged, this, Telltale::HandsOnOffStat, std::placeholders::_1));
#endif
    addSubscription(SFC.ADAS_Driving_New.Telltale.DAW.Stat,
                    std::bind(&Service::onTelltaleChanged, this, Telltale::DAWStat, std::placeholders::_1));
}

void Service::subscribeTelltale() {
    qDebug() << "Service - subscribeTelltale";

    subscribeTelltaleLampIndicator();
    subscribeTelltaleOAT();
    subscribeTelltaleDriveMode();
    subscribeTelltaleHighPerformanceForNBrand();
    subscribeTelltaleADASDrivingNew();
}

// ==================================================================================================================
// ==================================================================================================================
//    Event
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isEventIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEventADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

void Service::onEventChanged(const Event& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEventChanged :" << static_cast<int>(signalType) << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Event::IntroOutroStart) && (signalType < Event::IntroOutroEnd)) {
            signalValue = isEventIntroOutro(vehicleSignal);
        } else if ((signalType > Event::OATStart) && (signalType < Event::OATEnd)) {
            signalValue = isEventOAT(vehicleSignal);
        } else if ((signalType > Event::TransmissionIndicatorStart) && (signalType < Event::TransmissionIndicatorEnd)) {
            signalValue = isEventTransmissionIndicator(vehicleSignal);
        } else if ((signalType > Event::DriveModeStart) && (signalType < Event::DriveModeEnd)) {
            signalValue = isEventDriveMode(vehicleSignal);
        } else if ((signalType > Event::ADASParkingNewStart) && (signalType < Event::ADASParkingNewEnd)) {
            signalValue = isEventADASParkingNew(vehicleSignal);
        } else if ((signalType > Event::HighPerformanceForNBrandStart) && (signalType < Event::HighPerformanceForNBrandEnd)) {
            signalValue = isEventHighPerformanceForNBrand(vehicleSignal);
        } else if ((signalType > Event::ADASDrivingNewStart) && (signalType < Event::ADASDrivingNewEnd)) {
            signalValue = isEventADASDrivingNew(vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceDataChanged(static_cast<int>(DataType::Event), static_cast<int>(signalType), signalValue);
    }
}

void Service::subscribeEventIntroOutro() {
    addSubscription(SFC.Intro_Outro.Event.Welcome.ID,
                    std::bind(&Service::onEventChanged, this, Event::WelcomeID, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Welcome.Stat,
                    std::bind(&Service::onEventChanged, this, Event::WelcomeStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Welcome.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::WelcomeLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Welcome.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::WelcomeLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Welcome.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::WelcomeLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.SystemCheck.ID,
                    std::bind(&Service::onEventChanged, this, Event::SystemCheckID, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.SystemCheck.Stat,
                    std::bind(&Service::onEventChanged, this, Event::SystemCheckStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Goodbye.ID,
                    std::bind(&Service::onEventChanged, this, Event::GoodbyeID, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Goodbye.Stat,
                    std::bind(&Service::onEventChanged, this, Event::GoodbyeStat, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Goodbye.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::GoodbyeLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Goodbye.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::GoodbyeLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.Intro_Outro.Event.Goodbye.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::GoodbyeLinkedSoundRepeatCount, std::placeholders::_1));
}

void Service::subscribeEventOAT() {
    addSubscription(SFC.OAT.Event.IceWarn.ID, std::bind(&Service::onEventChanged, this, Event::IceWarnID, std::placeholders::_1));
    addSubscription(SFC.OAT.Event.IceWarn.Stat,
                    std::bind(&Service::onEventChanged, this, Event::IceWarnStat_, std::placeholders::_1));
    addSubscription(SFC.OAT.Event.IceWarn.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::IceWarnLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.OAT.Event.IceWarn.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::IceWarnLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.OAT.Event.IceWarn.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::IceWarnLinkedSoundRepeatCount, std::placeholders::_1));
}

void Service::subscribeEventTransmissionIndicator() {
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_Indicator.ID,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionIndicatorID, std::placeholders::_1));
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_Indicator.Stat,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionIndicatorStat, std::placeholders::_1));
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_PaddleShift.ID,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionPaddleShiftID, std::placeholders::_1));
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_PaddleShift.Stat,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionPaddleShiftStat, std::placeholders::_1));
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_SBW.ID,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionSBWID, std::placeholders::_1));
    addSubscription(SFC.Transmission_Indicator.Event.Transmission_SBW.Stat,
                    std::bind(&Service::onEventChanged, this, Event::TransmissionSBWStat, std::placeholders::_1));
}

void Service::subscribeEventDriveMode() {
    addSubscription(SFC.Drive_Mode.Event.DrivingModeChange.ID,
                    std::bind(&Service::onEventChanged, this, Event::DrivingModeChangeID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.DrivingModeChange.Stat,
                    std::bind(&Service::onEventChanged, this, Event::DrivingModeChangeStat, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.TerrainModeExitNotification.ID,
                    std::bind(&Service::onEventChanged, this, Event::TerrainModeExitNotificationID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.TerrainModeExitNotification.Stat,
                    std::bind(&Service::onEventChanged, this, Event::TerrainModeExitNotificationStat, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::TerrainModeExitNotificationLinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::TerrainModeExitNotificationLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::TerrainModeExitNotificationLinkedSoundRepeatCount,
                              std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmNCustomMode.ID,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmNCustomModeID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmNCustomMode.Stat,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmNCustomModeStat, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmNCustomModeLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmNCustomModeLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::ReconfirmNCustomModeLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ModeSwitchingImpossible.ID,
                    std::bind(&Service::onEventChanged, this, Event::ModeSwitchingImpossibleID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ModeSwitchingImpossible.Stat,
                    std::bind(&Service::onEventChanged, this, Event::ModeSwitchingImpossibleStat, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::ModeSwitchingImpossibleLinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::ModeSwitchingImpossibleLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::ModeSwitchingImpossibleLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmGT_MYMode.ID,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmGTMYModeID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmGT_MYMode.Stat,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmGTMYModeStat, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmGTMYModeLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::ReconfirmGTMYModeLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::ReconfirmGTMYModeLinkedSoundRepeatCount, std::placeholders::_1));
}

void Service::subscribeEventADASParking_New() {
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event4Group.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event4GroupID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event4Group.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Event4GroupStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure10ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure10Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure10LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure10LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure10LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure11ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure11Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure11LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure11LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Event7GroupFailure11LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupCCWFailureID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupCCWFailureStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event7GroupCCWFailureLinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Event7GroupCCWFailureLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Event7GroupCCWFailureLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event2Group.ID,
                    std::bind(&Service::onEventChanged, this, Event::Event2GroupID, std::placeholders::_1));
    addSubscription(SFC.ADAS_PARKING_NEW.Event.Event2Group.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Event2GroupStat, std::placeholders::_1));
}

void Service::subscribeEventHighPerformanceForNBrand() {
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakeOverride.ID,
                    std::bind(&Service::onEventChanged, this, Event::BrakeOverrideID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakeOverride.Stat,
                    std::bind(&Service::onEventChanged, this, Event::BrakeOverrideStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::BrakeOverrideLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::BrakeOverrideLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::BrakeOverrideLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs1.ID,
                    std::bind(&Service::onEventChanged, this, Event::Ngs1ID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Ngs1Stat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs2.ID,
                    std::bind(&Service::onEventChanged, this, Event::Ngs2ID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs2.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Ngs2Stat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Ngs2LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Ngs2LinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::Ngs2LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.NRoadSense.ID,
                    std::bind(&Service::onEventChanged, this, Event::NRoadSenseID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.NRoadSense.Stat,
                    std::bind(&Service::onEventChanged, this, Event::NRoadSenseStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::NRoadSenseLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::NRoadSenseLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::NRoadSenseLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.ID,
                    std::bind(&Service::onEventChanged, this, Event::BrakepadCheckID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.Stat,
                    std::bind(&Service::onEventChanged, this, Event::BrakepadCheckStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::BrakepadCheckLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::BrakepadCheckLinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::BrakepadCheckLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.ID,
                    std::bind(&Service::onEventChanged, this, Event::ElectricKickDriftID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.Stat,
                    std::bind(&Service::onEventChanged, this, Event::ElectricKickDriftStat, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::ElectricKickDriftLinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::ElectricKickDriftLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::ElectricKickDriftLinkedSoundRepeatCount, std::placeholders::_1));
}

void Service::subscribeEventADASDrivingNew() {
    addSubscription(SFC.ADAS_Driving_New.Event.DrivingAssistSummary.ID,
                    std::bind(&Service::onEventChanged, this, Event::DrivingAssistSummaryID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DrivingAssistSummary.Stat,
                    std::bind(&Service::onEventChanged, this, Event::DrivingAssistSummaryStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_1.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_1ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_1Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_2ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_2Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_2LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_2LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup1_2LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup2_1ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup2_1Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup2_1LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup2_1LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup2_1LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52099ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52099Stat, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52099LinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52099LinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52100.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52100ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52100.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52100Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52101ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52101Stat, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52101LinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52101LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52101LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52105ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52105Stat, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52105LinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52105LinkedSoundType, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52106.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52106ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52106.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52106Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52107ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52107Stat, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52107LinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52107LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group1FullPopup3_1_E52107LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group4FullPopup1_1ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group4FullPopup1_1Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group4FullPopup1_1LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Group4FullPopup1_1LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group4FullPopup1_1LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.ID,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupBlindSpotSafetyFailureID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.Stat,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupBlindSpotSafetyFailureStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundID,
                              std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundType,
                              std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundRepeatCount,
                              std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.ID,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupOutsideMirrorSymbolFailureID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.Stat,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupOutsideMirrorSymbolFailureStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundID,
                              std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundType,
                              std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.RepeatCount,
                    std::bind(&Service::onEventChanged, this,
                              Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupHdpFailureID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group7FullPopupHdpFailureStat, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.ID,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupHdpFailureLinkedSoundID, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.Type,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupHdpFailureLinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::Group7FullPopupHdpFailureLinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group2MiniPopup1_1.ID,
                    std::bind(&Service::onEventChanged, this, Event::Group2MiniPopup1_1ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.Group2MiniPopup1_1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::Group2MiniPopup1_1Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_2Status.ID,
                    std::bind(&Service::onEventChanged, this, Event::PuMGroup2AdasWarning1_2StatusID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_2Status.Stat,
                    std::bind(&Service::onEventChanged, this, Event::PuMGroup2AdasWarning1_2StatusStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure1.ID,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure1ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure1.Stat,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure1Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure1LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure1LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure1LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure2.ID,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure2ID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure2.Stat,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure2Stat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.ID,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure2LinkedSoundID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.Type,
                    std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure2LinkedSoundType, std::placeholders::_1));
    addSubscription(
        SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.RepeatCount,
        std::bind(&Service::onEventChanged, this, Event::DriverAssistFailure2LinkedSoundRepeatCount, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.HDPOperProhibit.ID,
                    std::bind(&Service::onEventChanged, this, Event::HDPOperProhibitID, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Event.HDPOperProhibit.Stat,
                    std::bind(&Service::onEventChanged, this, Event::HDPOperProhibitStat, std::placeholders::_1));
}

void Service::subscribeEvent() {
    qDebug() << "Service - subscribeEvent";

    void subscribeEventIntroOutro();
    void subscribeEventOAT();
    void subscribeEventTransmissionIndicator();
    void subscribeEventDriveMode();
    void subscribeEventADASParking_New();
    void subscribeEventHighPerformanceForNBrand();
    void subscribeEventADASDrivingNew();
}

// ==================================================================================================================
// ==================================================================================================================
//    Sound
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isSoundLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isSoundIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isSoundTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isSoundADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isSoundHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isSoundADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

void Service::onSoundChanged(const Sound& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onSoundChanged :" << static_cast<int>(signalType) << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Sound::LampIndicatorStart) && (signalType < Sound::LampIndicatorEnd)) {
            signalValue = isSoundLampIndicator(vehicleSignal);
        } else if ((signalType > Sound::IntroOutroStart) && (signalType < Sound::IntroOutroEnd)) {
            signalValue = isSoundIntroOutro(vehicleSignal);
        } else if ((signalType > Sound::TransmissionIndicatorStart) && (signalType < Sound::TransmissionIndicatorEnd)) {
            signalValue = isSoundTransmissionIndicator(vehicleSignal);
        } else if ((signalType > Sound::SADASParkingNewtart) && (signalType < Sound::ADASParkingNewEnd)) {
            signalValue = isSoundADASParkingNew(vehicleSignal);
        } else if ((signalType > Sound::HighPerformanceForNBrandStart) && (signalType < Sound::HighPerformanceForNBrandEnd)) {
            signalValue = isSoundHighPerformanceForNBrand(vehicleSignal);
        } else if ((signalType > Sound::ADASDrivingNewStart) && (signalType < Sound::ADASDrivingNewEnd)) {
            signalValue = isSoundADASDrivingNew(vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceDataChanged(static_cast<int>(DataType::Sound), static_cast<int>(signalType), signalValue);
    }
}

void Service::subscribeSoundLampIndicator() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSoundIntroOutro() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSoundTransmissionIndicator() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSoundADASParkingNew() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSoundHighPerformanceForNBrand() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSoundADASDrivingNew() {
    // addSubscription(AAAAAAAAA,
    //                 std::bind(&Service::onSoundChanged, this, Sound::AAAAAAAAA, std::placeholders::_1));
}

void Service::subscribeSound() {
    qDebug() << "Service - subscribeSound";

    subscribeSoundLampIndicator();
    subscribeSoundIntroOutro();
    subscribeSoundTransmissionIndicator();
    subscribeSoundADASParkingNew();
    subscribeSoundHighPerformanceForNBrand();
    subscribeSoundADASDrivingNew();
}

// ==================================================================================================================
// ==================================================================================================================
//    Etc
// ==================================================================================================================
// ==================================================================================================================
QVariant Service::isEtcSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedUnit) {
        value = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedUnit.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH) {
        value = static_cast<ccos::HDouble>(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH.value(vehicleSignal));
    } else if (nodePath == SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH) {
        value = static_cast<ccos::HDouble>(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH.value(vehicleSignal));
    } else {
        value = QVariant();
    }

    return value;
}

QVariant Service::isEtcDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

QVariant Service::isEtcADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal) {
    QVariant value = QVariant();
    std::string nodePath = vehicleSignal.getNodePath();

    return value;
}

void Service::onEtcChanged(const Etc& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEtcChanged :" << static_cast<int>(signalType) << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if ((signalType > Etc::SpeedGaugeStart) && (signalType < Etc::SpeedGaugeEnd)) {
            signalValue = isEtcSpeedGauge(vehicleSignal);
        } else if ((signalType > Etc::DriveModeStart) && (signalType < Etc::DriveModeEnd)) {
            signalValue = isEtcDriveMode(vehicleSignal);
        } else if ((signalType > Etc::ADASDrivingNewStart) && (signalType < Etc::ADASDrivingNewEnd)) {
            signalValue = isEtcADASDrivingNew(vehicleSignal);
        } else {
            signalValue = QVariant();
        }

        if (signalValue.isValid()) {
            break;
        }
    }

    if (signalValue.isValid()) {
        emit signalServiceDataChanged(static_cast<int>(DataType::Etc), static_cast<int>(signalType), signalValue);
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

void Service::subscribeEtcDriveMode() {
    addSubscription(SFC.Drive_Mode.Inter_DriveModeSelectStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDriveModeSelectStatus, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Inter_TerrainModeSelectStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterTerrainModeSelectStatus, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Inter_NModeSelectStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterNModeSelectStatus, std::placeholders::_1));
    addSubscription(SFC.Drive_Mode.Inter_GtModeSelectStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterGtModeSelectStatus, std::placeholders::_1));
}

void Service::subscribeEtcADASDrivingNew() {
    addSubscription(SFC.ADAS_Driving_New.Inter_AdasViewFixStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterAdasViewFixStatus, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Inter_HDPMasterWarningStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterHDPMasterWarningStatus, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Timer.OneShotTimerPriorityISLA.Stat,
                    std::bind(&Service::onEtcChanged, this, Etc::TimerOneShotTimerPriorityISLAStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Timer.OneShotTimerPriorityDIS.Stat,
                    std::bind(&Service::onEtcChanged, this, Etc::TimerOneShotTimerPriorityDISStat, std::placeholders::_1));
    addSubscription(SFC.ADAS_Driving_New.Inter_AdasOnStatus,
                    std::bind(&Service::onEtcChanged, this, Etc::InterAdasOnStatus, std::placeholders::_1));
}

void Service::subscribeEtc() {
    qDebug() << "Service - subscribeEtc";

    // subscribeEtcSpeedGauge();
    // subscribeEtcDriveMode();
    // subscribeEtcADASDrivingNew();
}
