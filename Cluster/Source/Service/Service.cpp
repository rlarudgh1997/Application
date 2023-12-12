#include "Service.h"

#include <regex>
#include <QDebug>

// #define USE_SERVICE_CODE

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

void Service::init() {
    qDebug() << "Service - Init";
    // mVehicleSignalModel = new HVehicleSignalModel;

    subscribeTelltale();
    subscribeEvent();
    subscribeGauge();
    subscribeContents();
    subscribeMainView();
}

HVehicleSignalModel* Service::getVehicleSignalModel() const {
    return mVehicleSignalModel;
}

void Service::addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc) {
    std::vector<std::string> nodePaths{nodeAddress};
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<ClusterSignalListener>(handlingFunc));
    auto result = mVehicleSignalModel->subscribe(subscription);
    if (result != ccos::HResult::OK) {
        qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress;
    }
}

void Service::addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc) {
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<ClusterSignalListener>(handlingFunc));
    auto result = mVehicleSignalModel->subscribe(subscription);
    if (result != ccos::HResult::OK) {
        for (const std::string& nodeAddress : nodePaths) {
            qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress;
        }
    }
}

void Service::onGaugeChanged(const int& signalType, const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH) {
            emit signalGaugeValueChanged(GaugeType::SpeedGaugeDisplaySpeedValueKPH,
                                         SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH.value(vehicleSignal));
            return;
        }
    }

    swtich(signalType) {
        case 0: {
            break;
        }
        case 1: {
            break;
        }
        default: {
            qDebug() << "Fail to signal type :" << signalType;
        }
    }
}


void Service::subscribeGauge() {
    qDebug() << "Service - subscribeGauge";

    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH,
                    std::bind(&Service::onGaugeChanged, this, 0, std::placeholders::_1));


#if defined(USE_SERVICE_CODE)
    /* Speed Gauge */
    auto speedGaugeDisplaySpeedValueKPHHandler =
        std::bind(&Service::onSpeedGaugeDisplaySpeedValueKPHChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH, speedGaugeDisplaySpeedValueKPHHandler);

    auto speedGaugeDisplaySpeedValueMPHHandler =
        std::bind(&Service::onSpeedGaugeDisplaySpeedValueMPHChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH, speedGaugeDisplaySpeedValueMPHHandler);

    auto speedGaugeSpeedAnalogStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedAnalogStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat, speedGaugeSpeedAnalogStatHandler);

    auto speedGaugeSpeedAnalogValueHandler =
        std::bind(&Service::onSpeedGaugeSpeedAnalogValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Value, speedGaugeSpeedAnalogValueHandler);

    auto speedGaugeSpeedDigitalStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedDigitalStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Stat, speedGaugeSpeedDigitalStatHandler);

    auto speedGaugeSpeedDigitalValueHandler =
        std::bind(&Service::onSpeedGaugeSpeedAnalogValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Value, speedGaugeSpeedDigitalValueHandler);

    auto speedGaugeSpeedSubDigitalStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedSubDigitalStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat, speedGaugeSpeedSubDigitalStatHandler);

    auto speedGaugeSpeedSubDigitalValueHandler =
        std::bind(&Service::onSpeedGaugeSpeedSubDigitalValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value, speedGaugeSpeedSubDigitalValueHandler);

    auto speedGaugeSpeedMainDisplayUnitStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedMainDisplayUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat, speedGaugeSpeedMainDisplayUnitStatHandler);

    auto speedGaugeSpeedAuxDisplayUnitStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedAuxDisplayUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat, speedGaugeSpeedAuxDisplayUnitStatHandler);

    auto speedGaugeSpeedSubDisplayStatHandler =
        std::bind(&Service::onSpeedGaugeSpeedSubDisplayStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat, speedGaugeSpeedSubDisplayStatHandler);

    auto speedGaugeDisplaySpeedValueKPHRangeStatHandler =
        std::bind(&Service::onSpeedGaugeDisplaySpeedValueKPHRangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Private.Speed_Gauge.DisplaySpeedValueKPHRange.Stat, speedGaugeDisplaySpeedValueKPHRangeStatHandler);

    auto speedGaugeDisplaySpeedUnitHandler =
        std::bind(&Service::onSpeedGaugeDisplaySpeedUnitChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedUnit, speedGaugeDisplaySpeedUnitHandler);

    /* Fuel System */
    auto fuelSystemGaugeHandler = std::bind(&Service::onFuelSystemFuelGaugeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Fuel_System.Constant.FuelGauge.Value, fuelSystemGaugeHandler);
#endif
}

void Service::subscribeTelltale() {
    qDebug() << "Service - subscribeTelltale";

#if defined(USE_SERVICE_CODE)
    /* ECall */
    auto eCallHandler = std::bind(&Service::onTelltaleECallStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.E_Call.Telltale.Ecall.Stat, eCallHandler);

    /* SeatbeltWarning */
    auto seatbeltWarningHandler = std::bind(&Service::onTelltaleSeatbeltWarningStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Seatbelt_Warning.Telltale.Seatbelt.Stat, seatbeltWarningHandler);

    /* OAT */
    auto telltaleOATHandler = std::bind(&Service::onTelltaleOATChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleOATHandlerNodes{SFC.OAT.Telltale.IceWarn.Stat, SFC.OAT.Telltale.IceWarn.StatOptional};
    addSubscriptions(telltaleOATHandlerNodes, telltaleOATHandler);
    // addSubscriptions(std::vector<std::string>({SFC.OAT.Telltale.IceWarn.Stat, SFC.OAT.Telltale.IceWarn.StatOptional}),
    //                  std::bind(&Service::onTelltaleOATChanged, this, 100, std::placeholders::_1));


    /* Air_Bag */
    auto telltaleAirBagHandler = std::bind(&Service::onTelltaleAirBagStatChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleAirBagHandlerNodes{SFC.Air_Bag.Telltale.AirBag.Stat, SFC.Air_Bag.Telltale.AirBag.BlinkValueA,
                                                        SFC.Air_Bag.Telltale.AirBag.BlinkValueB};
    addSubscriptions(telltaleAirBagHandlerNodes, telltaleAirBagHandler);

    /* LampFailWarning */
    auto lampFailWarningHandler = std::bind(&Service::onTelltaleLampFailWarningStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Fail_Warning.Telltale.LedLampFail.Stat, lampFailWarningHandler);

    /* ADAS_Driving_HBA*/
    auto telltaleHBAHandler = std::bind(&Service::onTelltaleIFSStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_HBA.Telltale.IFS.Stat, telltaleHBAHandler);

    /* xEV System Ready */
    auto xEVSystemReadyHandler = std::bind(&Service::onTelltalexEVSystemReadyChanged, this, std::placeholders::_1);
    addSubscription(SFC.xEV_System_Ready.Telltale.XevRdy.Stat, xEVSystemReadyHandler);

    /* Key System */
    auto keySystemHandler = std::bind(&Service::onTelltaleKeySystemChanged, this, std::placeholders::_1);
    addSubscription(SFC.Key_System.Telltale.Key_System.Stat, keySystemHandler);

    /* Lamp Indicator */
    auto lampIndicatorFrontFogHandler =
        std::bind(&Service::onTelltaleLampIndicatorFrontFogChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.FrontFog.Stat, lampIndicatorFrontFogHandler);

    auto lampIndicatorHighBeamHandler =
        std::bind(&Service::onTelltaleLampIndicatorHighBeamChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.HighBeam.Stat, lampIndicatorHighBeamHandler);

    auto lampIndicatorRearFogHandler = std::bind(&Service::onTelltaleLampIndicatorRearFogChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.RearFog.Stat, lampIndicatorRearFogHandler);

    auto lampIndicatorTailLampHandler =
        std::bind(&Service::onTelltaleLampIndicatorTailLampChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TailLamp.Stat, lampIndicatorTailLampHandler);

    auto lampIndicatorTurnSignalLeftHandler =
        std::bind(&Service::onTelltaleLampIndicatorTurnSignalLeftChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat, lampIndicatorTurnSignalLeftHandler);

    auto lampIndicatorTurnSignalRightHandler =
        std::bind(&Service::onTelltaleLampIndicatorTurnSignalRightChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat, lampIndicatorTurnSignalRightHandler);

    auto lampIndicatorLowBeamHandler = std::bind(&Service::onTelltaleLampIndicatorLowBeamChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.LowBeam.Stat, lampIndicatorLowBeamHandler);

    /* FCEV_Warning*/
    auto telltaleH2FaultHandler = std::bind(&Service::onTelltaleH2FaultChanged, this, std::placeholders::_1);
    addSubscription(SFC.FCEV_Warning.Telltale.TelltaleH2Fault.Stat, telltaleH2FaultHandler);

    auto telltaleH2LeakHandler = std::bind(&Service::onTelltaleH2LeakChanged, this, std::placeholders::_1);
    addSubscription(SFC.FCEV_Warning.Telltale.TelltaleH2Leak.Stat, telltaleH2LeakHandler);

    /* AFS_DBL */
    auto telltaleAFSDBLHandler = std::bind(&Service::onTelltaleAFSDBLStatChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleAFSDBLHandlerNodes{
        SFC.AFS_DBL.Telltale.AFS_DBL.Stat, SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueA, SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueB};
    addSubscriptions(telltaleAFSDBLHandlerNodes, telltaleAFSDBLHandler);

    /* ADAS_Driving */
    auto telltaleLDWLKAHandler = std::bind(&Service::onTelltaleLDWLKAChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleLDWLKAHandlerNodes{SFC.ADAS_Driving.Telltale.LDWLKA.Stat,
                                                        SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueA,
                                                        SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueB};
    addSubscriptions(telltaleLDWLKAHandlerNodes, telltaleLDWLKAHandler);

    auto telltaleFCAHandler = std::bind(&Service::onTelltaleFCAChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleFCAHandlerNodes{
        SFC.ADAS_Driving.Telltale.FCA.Stat, SFC.ADAS_Driving.Telltale.FCA.BlinkValueA, SFC.ADAS_Driving.Telltale.FCA.BlinkValueB};
    addSubscriptions(telltaleFCAHandlerNodes, telltaleFCAHandler);

    auto telltaleELKHandler = std::bind(&Service::onTelltaleELKChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleELKHandlerNodes{
        SFC.ADAS_Driving.Telltale.ELK.Stat, SFC.ADAS_Driving.Telltale.ELK.BlinkValueA, SFC.ADAS_Driving.Telltale.ELK.BlinkValueB};
    addSubscriptions(telltaleELKHandlerNodes, telltaleELKHandler);

    /* Fuel_System */
    auto telltaleLowFuelWarnHandler = std::bind(&Service::onTelltaleLowFuelWarnChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleLowFuelWarnHandlerNodes{
        SFC.Fuel_System.Telltale.LowFuelWarn.Stat, SFC.Fuel_System.Telltale.LowFuelWarn.StatOptional,
        SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueA, SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueB};
    addSubscriptions(telltaleLowFuelWarnHandlerNodes, telltaleLowFuelWarnHandler);

    /* ADAS_Driving_New */
    auto telltaleADASDrivingNewELK = std::bind(&Service::onTelltaleADASDrivingNewELKStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.ELK.Stat, telltaleADASDrivingNewELK);

    auto telltaleADASDrivingNewFCA = std::bind(&Service::onTelltaleADASDrivingNewFCAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.FCA.Stat, telltaleADASDrivingNewFCA);

    auto telltaleADASDrivingNewLKA = std::bind(&Service::onTelltaleADASDrivingNewLKAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LKA.Stat, telltaleADASDrivingNewLKA);

    auto telltaleADASDrivingNewHBA = std::bind(&Service::onTelltaleADASDrivingNewHBAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.HBA.Stat, telltaleADASDrivingNewHBA);

    auto telltaleADASDrivingNewLCALeft =
        std::bind(&Service::onTelltaleADASDrivingNewLCALeftStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCALeft.Stat, telltaleADASDrivingNewLCALeft);

    auto telltaleADASDrivingNewLCARight =
        std::bind(&Service::onTelltaleADASDrivingNewLCARightStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCARight.Stat, telltaleADASDrivingNewLCARight);

    auto telltaleADASDrivingNewLFA = std::bind(&Service::onTelltaleADASDrivingNewLFAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LFA.Stat, telltaleADASDrivingNewLFA);
#endif
}

void Service::subscribeEvent() {
    qDebug() << "Service - subscribeEvent";

#if defined(USE_SERVICE_CODE)
    /* Event Group Subscribe */
    std::vector<NodeAddress> addressList = sfc::searchAddress("SFC.*.Event.*");
    mVehicleSignalModel->subscribe(std::make_shared<HSubscription>(
        addressList, HSubscriptionType::VALUE_CHANGED,
        std::make_shared<sfc::SubscriptionListener>(std::bind(&Service::onEventFunc, this, std::placeholders::_1))));

    /* Lamp Fail Warning Constant*/
    auto eventLampFailWarningBiStatHandler =
        std::bind(&Service::onEventLampFailWarningBiStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningBiStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailBiLh.Stat,
                                                             SFC.Lamp_Fail_Warning.Constant.LampFailBiRh.Stat};
    addSubscriptions(eventLampFailWarningBiStatNodes, eventLampFailWarningBiStatHandler);

    auto eventLampFailWarningDrlStatHandler =
        std::bind(&Service::onEventLampFailWarningDrlStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningDrlStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailDrlLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailDrlRh.Stat};
    addSubscriptions(eventLampFailWarningDrlStatNodes, eventLampFailWarningDrlStatHandler);

    auto eventLampFailWarningFogStatHandler =
        std::bind(&Service::onEventLampFailWarningFogStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningFogStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogRh.Stat};
    addSubscriptions(eventLampFailWarningFogStatNodes, eventLampFailWarningFogStatHandler);

    auto eventLampFailWarningHighStatHandler =
        std::bind(&Service::onEventLampFailWarningHighStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningHighStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailHighLh.Stat,
                                                               SFC.Lamp_Fail_Warning.Constant.LampFailHighRh.Stat};
    addSubscriptions(eventLampFailWarningHighStatNodes, eventLampFailWarningHighStatHandler);

    auto eventLampFailWarningLowStatHandler =
        std::bind(&Service::onEventLampFailWarningLowStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningLowStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailLowLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailLowRh.Stat};
    addSubscriptions(eventLampFailWarningLowStatNodes, eventLampFailWarningLowStatHandler);

    auto eventLampFailWarningCornerStatHandler =
        std::bind(&Service::onEventLampFailWarningCornerStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningCornerStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailCornerLh.Stat,
                                                                 SFC.Lamp_Fail_Warning.Constant.LampFailCornerRh.Stat};
    addSubscriptions(eventLampFailWarningCornerStatNodes, eventLampFailWarningCornerStatHandler);

    auto eventLampFailWarningStopStatHandler =
        std::bind(&Service::onEventLampFailWarningStopStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningStopStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailStopLh.Stat,
                                                               SFC.Lamp_Fail_Warning.Constant.LampFailStopRh.Stat};
    addSubscriptions(eventLampFailWarningStopStatNodes, eventLampFailWarningStopStatHandler);

    auto eventLampFailWarningTurnStatHandler =
        std::bind(&Service::onEventLampFailWarningBiStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningTurnStatNodes{
        SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnLh.Stat, SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnRh.Stat,
        SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnLh.Stat, SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnRh.Stat};
    addSubscriptions(eventLampFailWarningTurnStatNodes, eventLampFailWarningTurnStatHandler);

    /*Intro Outro*/
    auto eventIntroOutroGoodbyeDriveCompleteDistanceStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteDistanceStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Stat,
                    eventIntroOutroGoodbyeDriveCompleteDistanceStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteDistanceValueHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Value,
                    eventIntroOutroGoodbyeDriveCompleteDistanceValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteTimeStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteTimeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Stat, eventIntroOutroGoodbyeDriveCompleteTimeStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteTimeValueHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteTimeValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Value,
                    eventIntroOutroGoodbyeDriveCompleteTimeValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteSpeedStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Stat,
                    eventIntroOutroGoodbyeDriveCompleteSpeedStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteSpeedValueHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Value,
                    eventIntroOutroGoodbyeDriveCompleteSpeedValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteFuelEcoStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteFuelEcoStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Stat,
                    eventIntroOutroGoodbyeDriveCompleteFuelEcoStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteFuelEcoValueHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeDriveCompleteFuelEcoValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Value,
                    eventIntroOutroGoodbyeDriveCompleteFuelEcoValueHandler);

    auto eventIntroOutroGoodbyeResvChargeStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeResvChargeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.ResvCharge.Stat, eventIntroOutroGoodbyeResvChargeStatHandler);

    auto eventIntroOutroGoodbyeResvClimateStatHandler =
        std::bind(&Service::onEventIntroOutroGoodbyeResvClimateStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.ResvClimate.Stat, eventIntroOutroGoodbyeResvClimateStatHandler);

    /* Illumination */
    auto eventIlluminationRheostatLevelHandler =
        std::bind(&Service::onEventIlluminationRheostatLevelChanged, this, std::placeholders::_1);
    addSubscription(SFC.Illumination.Constant.RheostatLevel.Stat, eventIlluminationRheostatLevelHandler);

    auto eventIlluminationClusterLightingLevelHandler =
        std::bind(&Service::onEventIlluminationClusterLightingLevelChanged, this, std::placeholders::_1);
    addSubscription(SFC.Illumination.Constant.ClusterLightingLevelStat.value, eventIlluminationClusterLightingLevelHandler);

    /* Service Required Warning */
    auto eventServiceRequiredWarningHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindChanged, this, std::placeholders::_1);
    std::vector<std::string> eventServiceRequiredWarningNodes{SFC.Service_Required_Warning.Event.ServiceRemind.ID,
                                                              SFC.Service_Required_Warning.Event.ServiceRemind.Stat};
    addSubscriptions(eventServiceRequiredWarningNodes, eventServiceRequiredWarningHandler);

    /* Service Required Warning Constant*/
    auto eventServiceRequiredWarningServiceRemindDayStatHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDayStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Stat,
                    eventServiceRequiredWarningServiceRemindDayStatHandler);

    auto eventServiceRequiredWarningServiceRemindDayValueHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDayValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Value,
                    eventServiceRequiredWarningServiceRemindDayValueHandler);

    auto eventServiceRequiredWarningServiceRemindDistKmStatHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDistKmStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Stat,
                    eventServiceRequiredWarningServiceRemindDistKmStatHandler);

    auto eventServiceRequiredWarningServiceRemindDistKmValueHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDistKmValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Value,
                    eventServiceRequiredWarningServiceRemindDistKmValueHandler);

    auto eventServiceRequiredWarningServiceRemindDistMiStatHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDistMiStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Stat,
                    eventServiceRequiredWarningServiceRemindDistMiStatHandler);

    auto eventServiceRequiredWarningServiceRemindDistMiValueHandler =
        std::bind(&Service::onEventServiceRequiredWarningServiceRemindDistMiValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Value,
                    eventServiceRequiredWarningServiceRemindDistMiValueHandler);

    /* ADAS_Parking_BVM_New */
    auto evnetAdasParkingBVMNewLeftOutMirrorFoldStatHandler =
        std::bind(&Service::onEventADASParkingBVMNEwLeftOutMirrorFoldStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Parking_BVM_NEW.Constant.BVMLeftOutMirrorFold.Stat,
                    evnetAdasParkingBVMNewLeftOutMirrorFoldStatHandler);

    auto evnetAdasParkingBVMNewRightOutMirrorFoldStatHandler =
        std::bind(&Service::onEventADASParkingBVMNEwRightOutMirrorFoldStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Parking_BVM_NEW.Constant.BVMRightOutMirrorFold.Stat,
                    evnetAdasParkingBVMNewRightOutMirrorFoldStatHandler);

    /* ADAS_PARKING_NEW */
    auto contentsADASParkingNEWParkCarBodyDisplayHandler =
        std::bind(&Service::onEventADASParkingNEWParkCarBodyDisplayChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkCarBodyDisplay.Stat, contentsADASParkingNEWParkCarBodyDisplayHandler);

    auto contentsADASParkingNEWParkFrontLeftWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkFrontLeftWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontLeftWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontLeftWarning3PartHandler);

    auto contentsADASParkingNEWParkFrontCenterWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkFrontCenterWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontCenterWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontCenterWarning3PartHandler);

    auto contentsADASParkingNEWParkRearLeftWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearLeftWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning3Part.Stat,
                    contentsADASParkingNEWParkRearLeftWarning3PartHandler);

    auto contentsADASParkingNEWParkFrontRightWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkFrontRightWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontRightWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontRightWarning3PartHandler);

    auto contentsADASParkingNEWParkRearCenterWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearCenterWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearCenterWarning3Part.Stat,
                    contentsADASParkingNEWParkRearCenterWarning3PartHandler);

    auto contentsADASParkingNEWParkRearRightWarning3PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearRightWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning3Part.Stat,
                    contentsADASParkingNEWParkRearRightWarning3PartHandler);

    auto contentsADASParkingNEWParkRearLeftWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearLeftWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning2Part.Stat,
                    contentsADASParkingNEWParkRearLeftWarning2PartHandler);

    auto contentsADASParkingNEWParkRearRightWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearRightWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning2Part.Stat,
                    contentsADASParkingNEWParkRearRightWarning2PartHandler);

    auto contentsADASParkingNEWParkLeftSideWarning1PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkLeftSideWarning1PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideWarning1Part.Stat,
                    contentsADASParkingNEWParkLeftSideWarning1PartHandler);

    auto contentsADASParkingNEWParkRightSideWarning1PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRightSideWarning1PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideWarning1Part.Stat,
                    contentsADASParkingNEWParkRightSideWarning1PartHandler);

    auto contentsADASParkingNEWParkLeftSideFrontWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkLeftSideFrontWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideFrontWarning2Part.Stat,
                    contentsADASParkingNEWParkLeftSideFrontWarning2PartHandler);

    auto contentsADASParkingNEWParkRightSideFrontWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRightSideFrontWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideFrontWarning2Part.Stat,
                    contentsADASParkingNEWParkRightSideFrontWarning2PartHandler);

    auto contentsADASParkingNEWParkLeftSideRearWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkLeftSideRearWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideRearWarning2Part.Stat,
                    contentsADASParkingNEWParkLeftSideRearWarning2PartHandler);

    auto contentsADASParkingNEWParkRightSideRearWarning2PartHandler =
        std::bind(&Service::onEventADASParkingNEWParkRightSideRearWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideRearWarning2Part.Stat,
                    contentsADASParkingNEWParkRightSideRearWarning2PartHandler);

    auto contentsADASParkingNEWParkFrontMasterWarningHandler =
        std::bind(&Service::onEventADASParkingNEWParkFrontMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontMasterWarning.Stat,
                    contentsADASParkingNEWParkFrontMasterWarningHandler);

    auto contentsADASParkingNEWParkRearMasterWarningHandler =
        std::bind(&Service::onEventADASParkingNEWParkRearMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearMasterWarning.Stat, contentsADASParkingNEWParkRearMasterWarningHandler);

    auto contentsADASParkingNEWParkLeftSideMasterWarningHandler =
        std::bind(&Service::onEventADASParkingNEWParkLeftSideMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideMasterWarning.Stat,
                    contentsADASParkingNEWParkLeftSideMasterWarningHandler);

    auto contentsADASParkingNEWParkRightSideMasterWarningHandler =
        std::bind(&Service::onEventADASParkingNEWParkRightSideMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideMasterWarning.Stat,
                    contentsADASParkingNEWParkRightSideMasterWarningHandler);

    auto contentsADASParkingNEWParkPcaFrontBoundaryLaneHandler =
        std::bind(&Service::onEventADASParkingNEWParkPcaFrontBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaFrontBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaFrontBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaRearBoundaryLaneHandler =
        std::bind(&Service::onEventADASParkingNEWParkPcaRearBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRearBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaRearBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaleftBoundaryLaneHandler =
        std::bind(&Service::onEventADASParkingNEWParkPcaleftBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaleftBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaleftBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaRightBoundaryLaneHandler =
        std::bind(&Service::onEventADASParkingNEWParkPcaRightBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRightBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaRightBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPCAWarningPhraseHandler =
        std::bind(&Service::onEventADASParkingNEWParkPCAWarningPhraseChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPCAWarningPhrase.Stat, contentsADASParkingNEWParkPCAWarningPhraseHandler);

    auto contentsADASParkingNEWParkRCCWLeftSymbolHandler =
        std::bind(&Service::onEventADASParkingNEWParkRCCWLeftSymbolChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWLeftSymbol.Stat, contentsADASParkingNEWParkRCCWLeftSymbolHandler);

    auto contentsADASParkingNEWParkRCCWRightSymbolHandler =
        std::bind(&Service::onEventADASParkingNEWParkRCCWRightSymbolChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWRightSymbol.Stat, contentsADASParkingNEWParkRCCWRightSymbolHandler);
#endif
}

void Service::subscribeContents() {
    qDebug() << "Service - subscribeContents";

#if defined(USE_SERVICE_CODE)
#ifdef VSM_Working
    /* OAT */
    auto contentsOATHandler = std::bind(&Service::onContentsOATChanged, this, std::placeholders::_1);
    std::vector<std::string> constentsOATHandlerNodes{SFC.OAT.Constant.OutTempDisplay.Stat,
                                                      SFC.OAT.Constant.OutTempDisplayUnit.Stat};
    addSubscriptions(constentsOATHandlerNodes, contentsOATHandler);

    /* DIS */
    auto contentsDISMainSpeedHandler = std::bind(&Service::onContentsDISMainSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Constant.DISMainSpeed.Stat, contentsDISMainSpeedHandler);

    auto contentsDISMainSpeedValueHandler =
        std::bind(&Service::onContentsDISMainSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Constant.DISMainSpeed.Value, contentsDISMainSpeedValueHandler);

    auto contentsDISAddtnlSignHandler = std::bind(&Service::onContentsDISAddtnlSignChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Inter_DISAddtnlSign, contentsDISAddtnlSignHandler);

    auto contentsDISConfigNaviDisCountryHandler =
        std::bind(&Service::onContentsDISNaviDisCountryChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Inter_ConfigNaviDisCountry, contentsDISConfigNaviDisCountryHandler);

    /* ADAS_Driving_SEA */
    auto contentsSEALeftWarnSymbolHandler =
        std::bind(&Service::onContentsSEASEALeftWarnSymbolStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_SEA.Constant.SEALeftWarnSymbol.Stat, contentsSEALeftWarnSymbolHandler);

    auto contentsSEARightWarnSymbolHandler =
        std::bind(&Service::onContentsSEASEARightWarnSymbolStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_SEA.Constant.SEARightWarnSymbol.Stat, contentsSEARightWarnSymbolHandler);

    /* SOC_Gauge */
    auto contentsSOCGaugeValueHandler = std::bind(&Service::onContentsSOCGaugeValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeValue, contentsSOCGaugeValueHandler);

    auto contentsSOCGaugeLowAlertStatHandler =
        std::bind(&Service::onContentsSOCGaugeLowAlertStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeLowAlertStat, contentsSOCGaugeLowAlertStatHandler);

    /* ADAS_Driving */
    auto contentsCruiseHDASymbolStatHandler =
        std::bind(&Service::onContentsCruiseHDASymbolStatChanged, this, std::placeholders::_1);
    std::vector<std::string> constentsCruiseHDASymbolStatHandlerNodes{SFC.ADAS_Driving.Constant.CruiseHDASymbol.Stat,
                                                                      SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueA,
                                                                      SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueB};
    addSubscriptions(constentsCruiseHDASymbolStatHandlerNodes, contentsCruiseHDASymbolStatHandler);

    auto contentsSettingSpeedValueStatHandler =
        std::bind(&Service::onConstantSettingSpeedValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeed.ValueStat, contentsSettingSpeedValueStatHandler);

    auto contentsSettingSpeedValueHandler =
        std::bind(&Service::onConstantSettingSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeed.Value, contentsSettingSpeedValueHandler);

    auto contentsSettingSpeedColorStatHandler =
        std::bind(&Service::onConstantSettingSpeedColorStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsSettingSpeedColorStatHandlerNodes{SFC.ADAS_Driving.Constant.SettingSpeed.Color.Stat,
                                                                       SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueA,
                                                                       SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueB};
    addSubscriptions(contentsSettingSpeedColorStatHandlerNodes, contentsSettingSpeedColorStatHandler);

    auto contentsSettingSpeedUnitStatHandler =
        std::bind(&Service::onConstantSettingSpeedUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeedUnit.Stat, contentsSettingSpeedUnitStatHandler);

    auto contentsSettingSpeedBlinkingStatHandler =
        std::bind(&Service::onConstantSettingSpeedBlinkingStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsSettingSpeedBlinkingStatHandlerNodes{
        SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.Stat, SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueA,
        SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueB};
    addSubscriptions(contentsSettingSpeedBlinkingStatHandlerNodes, contentsSettingSpeedBlinkingStatHandler);

    auto contentsFrontVehicleStatHandler = std::bind(&Service::onConstantFrontVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.FrontVehicle.Stat, contentsFrontVehicleStatHandler);

    auto contentsDistanceFromFrontVehicleStatHandler =
        std::bind(&Service::onConstantDistanceFromFrontVehicleStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsDistanceFromFrontVehicleStatHandlerNodes{
        SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.Stat, SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueA,
        SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueB};
    addSubscriptions(contentsDistanceFromFrontVehicleStatHandlerNodes, contentsDistanceFromFrontVehicleStatHandler);

    auto contentsNaviSetSymbolStatHandler =
        std::bind(&Service::onConstantNaviSetSymbolStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsNaviSetSymbolStatHandlerNodes{SFC.ADAS_Driving.Constant.NaviSetSymbol.Stat,
                                                                   SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueA,
                                                                   SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueB};
    addSubscriptions(contentsNaviSetSymbolStatHandlerNodes, contentsNaviSetSymbolStatHandler);

    auto contentsISLADirectionStatHandler =
        std::bind(&Service::onConstantISLADirectionStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.ISLADirection.Stat, contentsISLADirectionStatHandler);

    auto contentsLCAStatHandler = std::bind(&Service::onConstantLCAStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsLCAStatHandlerNodes{
        SFC.ADAS_Driving.Constant.LCA.Stat, SFC.ADAS_Driving.Constant.LCA.BlinkValueA, SFC.ADAS_Driving.Constant.LCA.BlinkValueB};
    addSubscriptions(contentsLCAStatHandlerNodes, contentsLCAStatHandler);

    auto contentsLFAStatHandler = std::bind(&Service::onConstantLFAStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsLFAStatHandlerNodes{
        SFC.ADAS_Driving.Constant.LFA.Stat, SFC.ADAS_Driving.Constant.LFA.BlinkValueA, SFC.ADAS_Driving.Constant.LFA.BlinkValueB};
    addSubscriptions(contentsLFAStatHandlerNodes, contentsLFAStatHandler);

    auto contentsAdasOnStatusHandler = std::bind(&Service::onConstnatAdasOnStatusChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Inter_AdasOnStatus, contentsAdasOnStatusHandler);

    /* ADAS_Driving_ISLA */
    auto contentsISLAMainSpeedStatHandler =
        std::bind(&Service::onContentsISLAMainSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed.Stat, contentsISLAMainSpeedStatHandler);

    auto contentsISLAMainSpeedBlinkHandler =
        std::bind(&Service::onContentsISLAMainSpeedBlinkChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAMainSpeedBlinkNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Stat,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueA,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueB};
    addSubscriptions(contentsISLAMainSpeedBlinkNodes, contentsISLAMainSpeedBlinkHandler);

    auto contentsISLAMainSpeedBlinkColorHandler =
        std::bind(&Service::onContentsISLAMainSpeedBlinkColorChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Color, contentsISLAMainSpeedBlinkColorHandler);

    auto contentsISLAMainSpeedValueHandler =
        std::bind(&Service::onContentsISLAMainSpeedValueChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAMainSpeedValueNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Stat,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Value};
    addSubscriptions(contentsISLAMainSpeedValueNodes, contentsISLAMainSpeedValueHandler);

    auto contentsISLAFinalAddtnlSignHandler =
        std::bind(&Service::onContentsISLAFinalAddtnlSignChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAFinalAddtnlSignNodes{SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.Stat,
                                                              SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.ISLAAddtnlSignValue,
                                                              SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.DISAddtnlSignValue};
    addSubscriptions(contentsISLAFinalAddtnlSignNodes, contentsISLAFinalAddtnlSignHandler);

    auto contentsISLAOverlapSignStatHandler =
        std::bind(&Service::onContentsISLAOverlapSignStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSign.Stat, contentsISLAOverlapSignStatHandler);

    auto contentsISLAOverlapSignBlinkHandler =
        std::bind(&Service::onContentsISLAOverlapSignBlinkChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAOverlapSignBlinkNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.Stat,
                                                               SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueA,
                                                               SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueB};
    addSubscriptions(contentsISLAOverlapSignBlinkNodes, contentsISLAOverlapSignBlinkHandler);
#endif
#endif
}

void Service::subscribeMainView() {
    qDebug() << "Service - subscribeMainView";

#if defined(USE_SERVICE_CODE)
#ifdef VSM_Working
    /* TBT */
    auto tbtNaviAliveHandler = std::bind(&Service::onMainViewTBTNaviAliveStatusChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Inter_NaviAliveStatus, tbtNaviAliveHandler);

    auto tbtConfigNaviMapCountryHandler =
        std::bind(&Service::onMainViewTBTConfigNaviMapCountryChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Inter_ConfigNaviMapCountry, tbtConfigNaviMapCountryHandler);

    auto tbtNaviTbtStatHandler = std::bind(&Service::onMainViewTBTNaviTBTStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.NaviTbt.Stat, tbtNaviTbtStatHandler);

    auto tbtDirectionTypeHandler = std::bind(&Service::onMainViewTBTDirectionTypeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.DirectionType.Stat, tbtDirectionTypeHandler);

    auto tbtSideStreetBitMaskHandler = std::bind(&Service::onMainViewTBTSideStreetBitMaskChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.SideStreet.Bitmask, tbtSideStreetBitMaskHandler);

    auto tbtCombinedSideStreetStatHandler =
        std::bind(&Service::onMainViewTBTCombinedSideStreetChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.CombinedSideStreet.Stat, tbtCombinedSideStreetStatHandler);

    auto tbtNaviFlagTypeHandler = std::bind(&Service::onMainViewTBTNaviPointFlagTypeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.NaviPointFlagType.Stat, tbtNaviFlagTypeHandler);

    auto tbtNaviDistToDestHandler = std::bind(&Service::onMainViewTBTNaviDistToDestChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtNaviDistToDestNodes{SFC.Navigation_TBT.Constant.NaviDistToDest.Stat,
                                                    SFC.Navigation_TBT.Constant.NaviDistToDest.Value,
                                                    SFC.Navigation_TBT.Constant.NaviDistUnit.Stat};
    addSubscriptions(tbtNaviDistToDestNodes, tbtNaviDistToDestHandler);

    auto tbtDistToTurnPointHandler = std::bind(&Service::onMainViewTBTNaviDistToTurnPointChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurnPointNodes{SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Stat,
                                                     SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Value,
                                                     SFC.Navigation_TBT.Constant.TbtDistanceUnit.Stat};
    addSubscriptions(tbtDistToTurnPointNodes, tbtDistToTurnPointHandler);

    auto tbtBarGraphHandler = std::bind(&Service::onMainViewTBTBarGraphChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtBarGraphNodes{SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Stat,
                                              SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Value};
    addSubscriptions(tbtBarGraphNodes, tbtBarGraphHandler);

    auto tbtDistToTurn1Handler = std::bind(&Service::onMainViewTBTNaviDistToTurn1Changed, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurn1Nodes{SFC.Navigation_TBT.Constant.NaviDistToTurn1.Stat,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurn1.Value,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurnUnit1.Stat};
    addSubscriptions(tbtDistToTurn1Nodes, tbtDistToTurn1Handler);

    auto tbtDistToTurn2Handler = std::bind(&Service::onMainViewTBTNaviDistToTurn2Changed, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurn2Nodes{SFC.Navigation_TBT.Constant.NaviDistToTurn2.Stat,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurn2.Value,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurnUnit2.Stat};
    addSubscriptions(tbtDistToTurn2Nodes, tbtDistToTurn2Handler);

    auto tbtNaviLaneRemainDistHandler =
        std::bind(&Service::onMainViewTBTNaviLaneRemainDistChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtNaviLaneRemainDistNodes{SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Stat,
                                                        SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Value,
                                                        SFC.Navigation_TBT.Constant.NaviLaneRemainDistUnit.Stat};
    addSubscriptions(tbtNaviLaneRemainDistNodes, tbtNaviLaneRemainDistHandler);

    /* ADAS_Driving*/
    auto leftSideLaneColorHandler = std::bind(&Service::onMainViewADASLeftSideLaneColorChanged, this, std::placeholders::_1);
    std::vector<std::string> leftSideLaneColorNodes{SFC.ADAS_Driving.Constant.LeftSideLane.Color.Stat,
                                                    SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueA,
                                                    SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueB};
    addSubscriptions(leftSideLaneColorNodes, leftSideLaneColorHandler);

    auto leftSideLaneLateralValueHandler =
        std::bind(&Service::onMainViewADASLeftSideLaneLateralValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.Value, leftSideLaneLateralValueHandler);

    auto leftSideLaneLateralValueStatHandler =
        std::bind(&Service::onMainViewADASLeftSideLaneLateralValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.ValueStat, leftSideLaneLateralValueStatHandler);

    auto leftSideLaneProgressInLaneChangeStatHandler =
        std::bind(&Service::onMainViewADASLeftSideLaneProgressInLaneChangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.ProgressInLaneChange.Stat,
                    leftSideLaneProgressInLaneChangeStatHandler);

    auto rightSideLaneColorHandler = std::bind(&Service::onMainViewADASRightSideLaneColorChanged, this, std::placeholders::_1);
    std::vector<std::string> rightSideLaneColorNodes{SFC.ADAS_Driving.Constant.RightSideLane.Color.Stat,
                                                     SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueA,
                                                     SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueB};
    addSubscriptions(rightSideLaneColorNodes, rightSideLaneColorHandler);

    auto rightSideLaneLateralValueHandler =
        std::bind(&Service::onMainViewADASRightSideLaneLateralValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.Value, rightSideLaneLateralValueHandler);

    auto rightSideLaneLateralValueStatHandler =
        std::bind(&Service::onMainViewADASRightSideLaneLateralValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.ValueStat, rightSideLaneLateralValueStatHandler);

    auto rightSideLaneProgressInLaneChangeStatHandler =
        std::bind(&Service::onMainViewADASRightSideLaneProgressInLaneChangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.ProgressInLaneChange.Stat,
                    rightSideLaneProgressInLaneChangeStatHandler);

    auto leftRightSideLaneCurvationValueHandler =
        std::bind(&Service::onMainViewADASLeftRightSideLaneCurvationValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.Value, leftRightSideLaneCurvationValueHandler);

    auto leftRightSideLaneCurvationValueStatHandler =
        std::bind(&Service::onMainViewADASLeftRightSideLaneCurvationValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.ValueStat, leftRightSideLaneCurvationValueStatHandler);

    auto vehicleFrontVehicleStatusStatHandler =
        std::bind(&Service::onMainViewADASVehicleFrontVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.VehicleStatus.Stat, vehicleFrontVehicleStatusStatHandler);

    auto vehicleFrontLongitudinalDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleFrontLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.Value,
                    vehicleFrontLongitudinalDistanceValueHandler);

    auto vehicleFrontLongitudinalDistanceValueStatHandler =
        std::bind(&Service::onMainViewADASVehicleFrontLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.ValueStat,
                    vehicleFrontLongitudinalDistanceValueStatHandler);

    auto vehicleFrontLateralDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleFrontLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.Value, vehicleFrontLateralDistanceValueHandler);

    auto vehicleFrontLateralDistanceValueStatHandler =
        std::bind(&Service::onMainViewADASVehicleFrontLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.ValueStat,
                    vehicleFrontLateralDistanceValueStatHandler);

    auto vehicleLeftForegroundVehicleStatusStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftForegroundVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.Stat,
                    vehicleLeftForegroundVehicleStatusStatHandler);

    auto vehicleLeftForegroundVehicleStatusFadeStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftForegroundVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.FadeStat,
                    vehicleLeftForegroundVehicleStatusFadeStatHandler);

    auto vehicleLeftForegroundLongitudinalDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.Value,
                    vehicleLeftForegroundLongitudinalDistanceValueHandler);

    auto vehicleLeftForegroundLongitudinalDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.ValueStat,
                    vehicleLeftForegroundLongitudinalDistanceValueStatHandler);

    auto vehicleLeftForegroundLateralDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleLeftForegroundLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.Value,
                    vehicleLeftForegroundLateralDistanceValueHandler);

    auto vehicleLeftForegroundLateralDistanceValueStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftForegroundLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.ValueStat,
                    vehicleLeftForegroundLateralDistanceValueStatHandler);

    auto vehicleRightForegroundVehicleStatusStatHandler =
        std::bind(&Service::onMainViewADASVehicleRightForegroundVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.Stat,
                    vehicleRightForegroundVehicleStatusStatHandler);

    auto vehicleRightForegroundVehicleStatusFadeStatHandler =
        std::bind(&Service::onMainViewADASVehicleRightForegroundVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.FadeStat,
                    vehicleRightForegroundVehicleStatusFadeStatHandler);

    auto vehicleRightForegroundLongitudinalDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.Value,
                    vehicleRightForegroundLongitudinalDistanceValueHandler);

    auto vehicleRightForegroundLongitudinalDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.ValueStat,
                    vehicleRightForegroundLongitudinalDistanceValueStatHandler);

    auto vehicleRightForegroundLateralDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleRightForegroundLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.Value,
                    vehicleRightForegroundLateralDistanceValueHandler);

    auto vehicleRightForegroundLateralDistanceValueStatHandler =
        std::bind(&Service::onMainViewADASVehicleRightForegroundLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.ValueStat,
                    vehicleRightForegroundLateralDistanceValueStatHandler);

    auto vehicleLeftRearwardVehicleStatusStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftRearwardVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.Stat,
                    vehicleLeftRearwardVehicleStatusStatHandler);

    auto vehicleLeftRearwardVehicleStatusFadeStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftRearwardVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.FadeStat,
                    vehicleLeftRearwardVehicleStatusFadeStatHandler);

    auto vehicleLeftRearwardLongitudinalDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.Value,
                    vehicleLeftRearwardLongitudinalDistanceValueHandler);

    auto vehicleLeftRearwardLongitudinalDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.ValueStat,
                    vehicleLeftRearwardLongitudinalDistanceValueStatHandler);

    auto vehicleLeftRearwardLateralDistanceValueHandler =
        std::bind(&Service::onMainViewADASVehicleLeftRearwardLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.Value,
                    vehicleLeftRearwardLateralDistanceValueHandler);

    auto vehicleLeftRearwardLateralDistanceValueStatHandler =
        std::bind(&Service::onMainViewADASVehicleLeftRearwardLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.ValueStat,
                    vehicleLeftRearwardLateralDistanceValueStatHandler);

    auto rearRightHandSideVehicleVehicleStatusStatHandler =
        std::bind(&Service::onMainViewADASRearRightHandSideVehicleVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.Stat,
                    rearRightHandSideVehicleVehicleStatusStatHandler);

    auto rearRightHandSideVehicleVehicleStatusFadeStatHandler =
        std::bind(&Service::onMainViewADASRearRightHandSideVehicleVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.FadeStat,
                    rearRightHandSideVehicleVehicleStatusFadeStatHandler);

    auto rearRightHandSideVehicleLongitudinalDistanceValueHandler = std::bind(
        &Service::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.Value,
                    rearRightHandSideVehicleLongitudinalDistanceValueHandler);

    auto rearRightHandSideVehicleLongitudinalDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.ValueStat,
                    rearRightHandSideVehicleLongitudinalDistanceValueStatHandler);

    auto rearRightHandSideVehicleLateralDistanceValueHandler =
        std::bind(&Service::onMainViewADASRearRightHandSideVehicleLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.Value,
                    rearRightHandSideVehicleLateralDistanceValueHandler);

    auto rearRightHandSideVehicleLateralDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASRearRightHandSideVehicleLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.ValueStat,
                    rearRightHandSideVehicleLateralDistanceValueStatHandler);

    auto theRearLeftHandSideVehicleStatHandler =
        std::bind(&Service::onMainViewADASTheRearLeftHandSideVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TheRearLeftHandSideVehicle.Stat, theRearLeftHandSideVehicleStatHandler);

    auto theRearRightHandSideVehicleStatHandler =
        std::bind(&Service::onMainViewADASTheRearRightHandSideVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TheRearRightHandSideVehicle.Stat, theRearRightHandSideVehicleStatHandler);

    auto leftRearSideCrashAlarmWaveHandler =
        std::bind(&Service::onMainViewADASLeftRearSideCrashAlarmWaveChanged, this, std::placeholders::_1);
    std::vector<std::string> leftRearSideCrashAlarmWaveNodes{SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.Stat,
                                                             SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueA,
                                                             SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueB};
    addSubscriptions(leftRearSideCrashAlarmWaveNodes, leftRearSideCrashAlarmWaveHandler);

    auto leftRearSideCrashAlarmMasterSymbolHandler =
        std::bind(&Service::onMainViewADASLeftRearSideCrashAlarmMasterSymbolChanged, this, std::placeholders::_1);
    std::vector<std::string> leftRearSideCrashAlarmMasterSymbolNodes{
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.Stat,
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueA,
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueB};
    addSubscriptions(leftRearSideCrashAlarmMasterSymbolNodes, leftRearSideCrashAlarmMasterSymbolHandler);

    auto rightRearSideCrashAlarmWaveHandler =
        std::bind(&Service::onMainViewADASRightRearSideCrashAlarmWaveChanged, this, std::placeholders::_1);
    std::vector<std::string> rightRearSideCrashAlarmWaveNodes{SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.Stat,
                                                              SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueA,
                                                              SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueB};
    addSubscriptions(rightRearSideCrashAlarmWaveNodes, rightRearSideCrashAlarmWaveHandler);

    auto rightRearSideCrashAlarmMasterSymbolHandler =
        std::bind(&Service::onMainViewADASRightRearSideCrashAlarmMasterSymbolChanged, this, std::placeholders::_1);
    std::vector<std::string> rightRearSideCrashAlarmMasterSymbolNodes{
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.Stat,
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueA,
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueB};
    addSubscriptions(rightRearSideCrashAlarmMasterSymbolNodes, rightRearSideCrashAlarmMasterSymbolHandler);

    auto targetIntervalDistanceStaHandler =
        std::bind(&Service::onMainViewADASTargetIntervalDistanceStaChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.Stat, targetIntervalDistanceStaHandler);

    auto targetIntervalDistanceLongitudinalDistanceValueHandler =
        std::bind(&Service::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.Value,
                    targetIntervalDistanceLongitudinalDistanceValueHandler);

    auto targetIntervalDistanceLongitudinalDistanceValueStatHandler = std::bind(
        &Service::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.ValueStat,
                    targetIntervalDistanceLongitudinalDistanceValueStatHandler);

    auto centerLineStatHandler = std::bind(&Service::onMainViewADASCenterLineStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.CenterLine.Stat, centerLineStatHandler);

    /* ADAS_Driving_NEW */

    auto adasDrivingNewFrontVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewFrontVehicleNodes{SFC.ADAS_Driving_New.Constant.FrontVehicle.Color.Stat,
                                                             SFC.ADAS_Driving_New.Constant.FrontVehicle.Level.Value};
    addSubscriptions(adasDrivingNewFrontVehicleNodes, adasDrivingNewFrontVehicleHandler);

    auto adasDrivingNewHeadwayDistanceHandler =
        std::bind(&Service::onMainViewADASDrivingNewHeadwayDistanceChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewHeadwayDistanceNodes{SFC.ADAS_Driving_New.Constant.HeadwayDistance.Color.Stat,
                                                                SFC.ADAS_Driving_New.Constant.HeadwayDistance.Level.Value};
    addSubscriptions(adasDrivingNewHeadwayDistanceNodes, adasDrivingNewHeadwayDistanceHandler);

    auto adasDrivingNewSubVehicleColorStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewSubVehicleColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.SubVehicle.Color.Stat, adasDrivingNewSubVehicleColorStatHandler);

    auto adasDrivingNewSetSpeedHandler =
        std::bind(&Service::onMainViewADASDrivingNewSetSpeedChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewSetSpeedNodes{SFC.ADAS_Driving_New.Constant.SetSpeed.Color.Stat,
                                                         SFC.ADAS_Driving_New.Constant.SetSpeed.Value,
                                                         SFC.ADAS_Driving_New.Constant.SetSpeed.Stat};
    addSubscriptions(adasDrivingNewSetSpeedNodes, adasDrivingNewSetSpeedHandler);

    auto adasDrivingNewIslaManualSetSpeedStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewISLAManualSetSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ISLAManualSetSpeed.Stat, adasDrivingNewIslaManualSetSpeedStatHandler);

    auto adasDrivingNewLaneCentralLineStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewLaneCentralLineStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.LaneCentralLine.Stat, adasDrivingNewLaneCentralLineStatHandler);

    auto adasDrivingNewLeftLaneHandler =
        std::bind(&Service::onMainViewADASDrivingNewLeftLaneChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewLeftLaneNodes{SFC.ADAS_Driving_New.Constant.LeftLane.Color.Stat,
                                                         SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Value,
                                                         SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Stat};
    addSubscriptions(adasDrivingNewLeftLaneNodes, adasDrivingNewLeftLaneHandler);

    auto adasDrivingNewRightLaneHandler =
        std::bind(&Service::onMainViewADASDrivingNewRightLaneChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRightLaneNodes{SFC.ADAS_Driving_New.Constant.RightLane.Color.Stat,
                                                          SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Value,
                                                          SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Stat};
    addSubscriptions(adasDrivingNewRightLaneNodes, adasDrivingNewRightLaneHandler);

    auto adasDrivingNewCvrtLvlHandler =
        std::bind(&Service::onMainViewADASDrivingNewLaneCvrtLvlChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCvrtLvlNodes{SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Value,
                                                        SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Stat};
    addSubscriptions(adasDrivingNewCvrtLvlNodes, adasDrivingNewCvrtLvlHandler);

    auto adasDrivingNewHeadwayDistHandler =
        std::bind(&Service::onMainViewADASDrivingNewHeadwayDistChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewHeadwayDistNodes{SFC.ADAS_Driving_New.Constant.HeadwayDist.Color.Stat,
                                                            SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Value,
                                                            SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Stat};
    addSubscriptions(adasDrivingNewHeadwayDistNodes, adasDrivingNewHeadwayDistHandler);

    auto adasDrivingNewCenterRoadHandler =
        std::bind(&Service::onMainViewADASDrivingNewCenterRoadChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadNodes{SFC.ADAS_Driving_New.Constant.CenterRoad.Color.Stat,
                                                           SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Value,
                                                           SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Stat};
    addSubscriptions(adasDrivingNewCenterRoadNodes, adasDrivingNewCenterRoadHandler);

    auto adasDrivingNewLeftRoadColorHandler =
        std::bind(&Service::onMainViewADASDrivingNewLeftRoadColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.LeftRoad.Color.Stat, adasDrivingNewLeftRoadColorHandler);

    auto adasDrivingNewRightRoadColorHandler =
        std::bind(&Service::onMainViewADASDrivingNewRightRoadColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.RightRoad.Color.Stat, adasDrivingNewRightRoadColorHandler);

    auto adasDrivingNewViewFrontVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontVehicleNodes, adasDrivingNewViewFrontVehicleHandler);

    auto adasDrivingNewViewFrontLeftVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontLeftVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.Color.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontLeftVehicleNodes, adasDrivingNewViewFrontLeftVehicleHandler);

    auto adasDrivingNewViewFrontRightVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewFrontRightVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontRightVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.Color.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontRightVehicleNodes, adasDrivingNewViewFrontRightVehicleHandler);

    auto adasDrivingNewViewLeftVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewLeftVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewLeftVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewLeftVehicleNodes, adasDrivingNewViewLeftVehicleHandler);

    auto adasDrivingNewViewRightVehicleHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewRightVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewRightVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewRightVehicleNodes, adasDrivingNewViewRightVehicleHandler);

    auto adasDrivingNewRearLeftWarnStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewRearLeftWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRearLeftWarn.Stat, adasDrivingNewRearLeftWarnStatHandler);

    auto adasDrivingNewRearRightWarnStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewRearRightWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRearRightWarn.Stat, adasDrivingNewRearRightWarnStatHandler);

    auto adasDrivingNewRearLeftArrowStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewRearLeftArrowStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewLeftArrow.Stat, adasDrivingNewRearLeftArrowStatHandler);

    auto adasDrivingNewRearRightArrowStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewRearRightArrowStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRightArrow.Stat, adasDrivingNewRearRightArrowStatHandler);

    auto adasDrivingNewISLAMainHandler =
        std::bind(&Service::onContentsADASDrivingNewISLAMainChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewISLAMainNodes{SFC.ADAS_Driving_New.Constant.ISLAMainSpeed.Stat,
                                                         SFC.ADAS_Driving_New.Constant.ISLAMainSign.Value,
                                                         SFC.ADAS_Driving_New.Constant.ISLAMainSign.Stat};
    addSubscriptions(adasDrivingNewISLAMainNodes, adasDrivingNewISLAMainHandler);

    auto adasDrivingNewFinalAddtnlSignHandler =
        std::bind(&Service::onContentsADASDrivingNewFinalAddtnlSignChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewFinalAddtnlSignNodes{SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat,
                                                                SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat,
                                                                SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat};
    addSubscriptions(adasDrivingNewFinalAddtnlSignNodes, adasDrivingNewFinalAddtnlSignHandler);

    auto adasDrivingNewISLAAddSignStatHandler =
        std::bind(&Service::onContentsADASDrivingNewISLAAddSignStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ISLAAddSign.Stat, adasDrivingNewISLAAddSignStatHandler);

    auto adasDrivingNewHDPStateDisplayBarStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewHDPStatDisplayBarStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.HDPStateDisplayBar.Stat, adasDrivingNewHDPStateDisplayBarStatHandler);

    auto adasDrivingNewCenterRoadLeftLatPosHandler =
        std::bind(&Service::onMainViewADASDrivingNewCenterRoadLeftLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadLeftLatPosNodes{SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Stat,
                                                                     SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Value};
    addSubscriptions(adasDrivingNewCenterRoadLeftLatPosNodes, adasDrivingNewCenterRoadLeftLatPosHandler);

    auto adasDrivingNewCenterRoadRightLatPosHandler =
        std::bind(&Service::onMainViewADASDrivingNewCenterRoadRightLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadRightLatPosNodes{SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Stat,
                                                                      SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Value};
    addSubscriptions(adasDrivingNewCenterRoadRightLatPosNodes, adasDrivingNewCenterRoadRightLatPosHandler);

    auto adasDrivingNewRoadCvrtLvlHandler =
        std::bind(&Service::onMainViewADASDrivingNewRoadCvrtLvlChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRoadCvrtLvlNodes{SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Stat,
                                                            SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Value};
    addSubscriptions(adasDrivingNewRoadCvrtLvlNodes, adasDrivingNewRoadCvrtLvlHandler);

    auto adasDrivingNewLeftRoadLatPosHandler =
        std::bind(&Service::onMainViewADASDrivingNewLeftRoadLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewLeftRoadLatPosNodes{SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Stat,
                                                               SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Value};
    addSubscriptions(adasDrivingNewLeftRoadLatPosNodes, adasDrivingNewLeftRoadLatPosHandler);

    auto adasDrivingNewRightRoadLatPosHandler =
        std::bind(&Service::onMainViewADASDrivingNewRightRoadLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRightRoadLatPosNodes{SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Stat,
                                                                SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Value};
    addSubscriptions(adasDrivingNewRightRoadLatPosNodes, adasDrivingNewRightRoadLatPosHandler);

    auto adasDrivingNewHViewFrontWarnStatHandler =
        std::bind(&Service::onMainViewADASDrivingNewViewFrontWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewFrontWarn.Stat, adasDrivingNewHViewFrontWarnStatHandler);
#endif
#endif
}
