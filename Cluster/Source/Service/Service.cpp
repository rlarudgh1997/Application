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

void Service::onConstantChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onConstantChanged :" << signalType << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        // SpeedAnalog
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAnalog.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat.value(vehicleSignal));
            break;
        }
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAnalog.Value) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Value.value(vehicleSignal));
            break;
        }

        // SpeedDigital
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedDigital.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Stat.value(vehicleSignal));
            break;
        }
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedDigital.Value) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Value.value(vehicleSignal));
            break;
        }

        // SpeedSubDigital
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat.value(vehicleSignal));
            break;
        }
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDigital.Value) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value.value(vehicleSignal));
            break;
        }

        // SpeedMainDisplayUnit
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat.value(vehicleSignal));
            break;
        }

        // SpeedAuxDisplayUnit
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat.value(vehicleSignal));
            break;
        }

        // SpeedSubDisplay
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat.value(vehicleSignal));
            break;
        }

        // SpeedScaleMaximum
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat.value(vehicleSignal));
            break;
        }

        // NaviSpeedLimit
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat) {
            signalValue = static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat.value(vehicleSignal));
            break;
        }

        // NaviSpeedLimitOver1Color
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value) {
            signalValue =
            static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value.value(vehicleSignal));
            break;
        }

        // NaviSpeedLimitOver2Color
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value) {
            signalValue =
            static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value.value(vehicleSignal));
            break;
        }
    }

    // emit signal
    if (signalValue.isValid()) {
        emit signalServiceConstantChanged(signalType, signalValue);
    }
}

void Service::subscribeConstant() {
    qDebug() << "Service - subscribeConstant";

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

void Service::onTelltaleChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onTelltaleChanged :" << signalType << signalList.size();
}

void Service::subscribeTelltale() {
    qDebug() << "Service - subscribeTelltale";
}

void Service::onEventChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEventChanged :" << signalType << signalList.size();
}

void Service::subscribeEvent() {
    qDebug() << "Service - subscribeEvent";
}

void Service::onSoundChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onSoundChanged :" << signalType << signalList.size();
}

void Service::subscribeSound() {
    qDebug() << "Service - subscribeSound";
}

void Service::onEtcChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    qDebug() << "onEtcChanged :" << signalType << signalList.size();
    QVariant signalValue = QVariant();

    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedUnit) {
            signalValue =
            static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedUnit.value(vehicleSignal));
            break;
        }
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH) {
            signalValue =
            static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH.value(vehicleSignal));
            break;
        }
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH) {
            signalValue =
            static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH.value(vehicleSignal));
            break;
        }
    }

    // emit signal
    if (signalValue.isValid()) {
        emit signalServiceEtcChanged(signalType, signalValue);
    }
}


void Service::subscribeEtc() {
    qDebug() << "Service - subscribeEtc";

    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedUnit,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedUnit, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedValueKPH, std::placeholders::_1));
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH,
                    std::bind(&Service::onEtcChanged, this, Etc::InterDisplaySpeedValueMPH, std::placeholders::_1));
}
