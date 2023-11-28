#include "cvService/AppService.h"

#include <regex>
#include "Logger.h"

AppService::AppService(QObject* parent) : QObject(parent), mVehicleSignalModel(nullptr) {
    HDebug() << Q_FUNC_INFO;
    mVehicleSignalModel = new HVehicleSignalModel;
    subscribeToCluster();
}

AppService::~AppService() {
#ifndef X86_64
    if (mVehicleSignalModel) {
        delete mVehicleSignalModel;
        mVehicleSignalModel = nullptr;
    }
#endif
}

void AppService::addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc) {
#ifndef X86_64
    std::vector<std::string> nodePaths{nodeAddress};
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<ClusterSignalListener>(handlingFunc));
    auto result = mVehicleSignalModel->subscribe(subscription);
    if (result != ccos::HResult::OK) {
        HWarning() << "Subscribe is Invalid : " << nodeAddress << ", result : " << static_cast<int>(result);
    }
#endif
}

void AppService::addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc) {
#ifndef X86_64
    auto subscription = std::make_shared<HSubscription>(nodePaths, HSubscriptionType::VALUE_CHANGED,
                                                        std::make_shared<ClusterSignalListener>(handlingFunc));
    auto result = mVehicleSignalModel->subscribe(subscription);
    if (result != ccos::HResult::OK) {
        for (const std::string& nodeAddress : nodePaths) {
            HWarning() << "Subscribe is Invalid : " << nodeAddress << ", result : " << static_cast<int>(result);
        }
    }
#endif
}

HVehicleSignalModel* AppService::getVehicleSignalModel() const {
    return mVehicleSignalModel;
}

void AppService::subscribeToCluster() {
    subscribeTelltale();
    subscribeEvent();
    subscribeGauge();
    subscribeContents();
    subscribeMainView();
}

void AppService::subscribeTelltale() {
    /* ECall */
    auto eCallHandler = std::bind(&AppService::onTelltaleECallStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.E_Call.Telltale.Ecall.Stat, eCallHandler);

    /* SeatbeltWarning */
    auto seatbeltWarningHandler = std::bind(&AppService::onTelltaleSeatbeltWarningStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Seatbelt_Warning.Telltale.Seatbelt.Stat, seatbeltWarningHandler);

    /* OAT */
    auto telltaleOATHandler = std::bind(&AppService::onTelltaleOATChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleOATHandlerNodes{SFC.OAT.Telltale.IceWarn.Stat, SFC.OAT.Telltale.IceWarn.StatOptional};
    addSubscriptions(telltaleOATHandlerNodes, telltaleOATHandler);

    /* Air_Bag */
    auto telltaleAirBagHandler = std::bind(&AppService::onTelltaleAirBagStatChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleAirBagHandlerNodes{SFC.Air_Bag.Telltale.AirBag.Stat, SFC.Air_Bag.Telltale.AirBag.BlinkValueA,
                                                        SFC.Air_Bag.Telltale.AirBag.BlinkValueB};
    addSubscriptions(telltaleAirBagHandlerNodes, telltaleAirBagHandler);

    /* LampFailWarning */
    auto lampFailWarningHandler = std::bind(&AppService::onTelltaleLampFailWarningStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Fail_Warning.Telltale.LedLampFail.Stat, lampFailWarningHandler);

    /* ADAS_Driving_HBA*/
    auto telltaleHBAHandler = std::bind(&AppService::onTelltaleIFSStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_HBA.Telltale.IFS.Stat, telltaleHBAHandler);

    /* xEV System Ready */
    auto xEVSystemReadyHandler = std::bind(&AppService::onTelltalexEVSystemReadyChanged, this, std::placeholders::_1);
    addSubscription(SFC.xEV_System_Ready.Telltale.XevRdy.Stat, xEVSystemReadyHandler);

    /* Key System */
    auto keySystemHandler = std::bind(&AppService::onTelltaleKeySystemChanged, this, std::placeholders::_1);
    addSubscription(SFC.Key_System.Telltale.Key_System.Stat, keySystemHandler);

    /* Lamp Indicator */
    auto lampIndicatorFrontFogHandler =
        std::bind(&AppService::onTelltaleLampIndicatorFrontFogChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.FrontFog.Stat, lampIndicatorFrontFogHandler);

    auto lampIndicatorHighBeamHandler =
        std::bind(&AppService::onTelltaleLampIndicatorHighBeamChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.HighBeam.Stat, lampIndicatorHighBeamHandler);

    auto lampIndicatorRearFogHandler = std::bind(&AppService::onTelltaleLampIndicatorRearFogChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.RearFog.Stat, lampIndicatorRearFogHandler);

    auto lampIndicatorTailLampHandler =
        std::bind(&AppService::onTelltaleLampIndicatorTailLampChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TailLamp.Stat, lampIndicatorTailLampHandler);

    auto lampIndicatorTurnSignalLeftHandler =
        std::bind(&AppService::onTelltaleLampIndicatorTurnSignalLeftChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat, lampIndicatorTurnSignalLeftHandler);

    auto lampIndicatorTurnSignalRightHandler =
        std::bind(&AppService::onTelltaleLampIndicatorTurnSignalRightChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat, lampIndicatorTurnSignalRightHandler);

    auto lampIndicatorLowBeamHandler = std::bind(&AppService::onTelltaleLampIndicatorLowBeamChanged, this, std::placeholders::_1);
    addSubscription(SFC.Lamp_Indicator.Telltale.LowBeam.Stat, lampIndicatorLowBeamHandler);

    /* FCEV_Warning*/
    auto telltaleH2FaultHandler = std::bind(&AppService::onTelltaleH2FaultChanged, this, std::placeholders::_1);
    addSubscription(SFC.FCEV_Warning.Telltale.TelltaleH2Fault.Stat, telltaleH2FaultHandler);

    auto telltaleH2LeakHandler = std::bind(&AppService::onTelltaleH2LeakChanged, this, std::placeholders::_1);
    addSubscription(SFC.FCEV_Warning.Telltale.TelltaleH2Leak.Stat, telltaleH2LeakHandler);

    /* AFS_DBL */
    auto telltaleAFSDBLHandler = std::bind(&AppService::onTelltaleAFSDBLStatChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleAFSDBLHandlerNodes{
        SFC.AFS_DBL.Telltale.AFS_DBL.Stat, SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueA, SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueB};
    addSubscriptions(telltaleAFSDBLHandlerNodes, telltaleAFSDBLHandler);

    /* ADAS_Driving */
    auto telltaleLDWLKAHandler = std::bind(&AppService::onTelltaleLDWLKAChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleLDWLKAHandlerNodes{SFC.ADAS_Driving.Telltale.LDWLKA.Stat,
                                                        SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueA,
                                                        SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueB};
    addSubscriptions(telltaleLDWLKAHandlerNodes, telltaleLDWLKAHandler);

    auto telltaleFCAHandler = std::bind(&AppService::onTelltaleFCAChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleFCAHandlerNodes{
        SFC.ADAS_Driving.Telltale.FCA.Stat, SFC.ADAS_Driving.Telltale.FCA.BlinkValueA, SFC.ADAS_Driving.Telltale.FCA.BlinkValueB};
    addSubscriptions(telltaleFCAHandlerNodes, telltaleFCAHandler);

    auto telltaleELKHandler = std::bind(&AppService::onTelltaleELKChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleELKHandlerNodes{
        SFC.ADAS_Driving.Telltale.ELK.Stat, SFC.ADAS_Driving.Telltale.ELK.BlinkValueA, SFC.ADAS_Driving.Telltale.ELK.BlinkValueB};
    addSubscriptions(telltaleELKHandlerNodes, telltaleELKHandler);

    /* Fuel_System */
    auto telltaleLowFuelWarnHandler = std::bind(&AppService::onTelltaleLowFuelWarnChanged, this, std::placeholders::_1);
    std::vector<std::string> telltaleLowFuelWarnHandlerNodes{
        SFC.Fuel_System.Telltale.LowFuelWarn.Stat, SFC.Fuel_System.Telltale.LowFuelWarn.StatOptional,
        SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueA, SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueB};
    addSubscriptions(telltaleLowFuelWarnHandlerNodes, telltaleLowFuelWarnHandler);

    /* ADAS_Driving_New */
    auto telltaleADASDrivingNewELK = std::bind(&AppService::onTelltaleADASDrivingNewELKStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.ELK.Stat, telltaleADASDrivingNewELK);

    auto telltaleADASDrivingNewFCA = std::bind(&AppService::onTelltaleADASDrivingNewFCAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.FCA.Stat, telltaleADASDrivingNewFCA);

    auto telltaleADASDrivingNewLKA = std::bind(&AppService::onTelltaleADASDrivingNewLKAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LKA.Stat, telltaleADASDrivingNewLKA);

    auto telltaleADASDrivingNewHBA = std::bind(&AppService::onTelltaleADASDrivingNewHBAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.HBA.Stat, telltaleADASDrivingNewHBA);

    auto telltaleADASDrivingNewLCALeft =
        std::bind(&AppService::onTelltaleADASDrivingNewLCALeftStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCALeft.Stat, telltaleADASDrivingNewLCALeft);

    auto telltaleADASDrivingNewLCARight =
        std::bind(&AppService::onTelltaleADASDrivingNewLCARightStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LCARight.Stat, telltaleADASDrivingNewLCARight);

    auto telltaleADASDrivingNewLFA = std::bind(&AppService::onTelltaleADASDrivingNewLFAStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Telltale.LFA.Stat, telltaleADASDrivingNewLFA);
}

void AppService::subscribeEvent() {
    /* Event Group Subscribe */
    std::vector<NodeAddress> addressList = sfc::searchAddress("SFC.*.Event.*");
    mVehicleSignalModel->subscribe(std::make_shared<HSubscription>(
        addressList, HSubscriptionType::VALUE_CHANGED,
        std::make_shared<sfc::SubscriptionListener>(std::bind(&AppService::onEventFunc, this, std::placeholders::_1))));

    /* Lamp Fail Warning Constant*/
    auto eventLampFailWarningBiStatHandler =
        std::bind(&AppService::onEventLampFailWarningBiStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningBiStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailBiLh.Stat,
                                                             SFC.Lamp_Fail_Warning.Constant.LampFailBiRh.Stat};
    addSubscriptions(eventLampFailWarningBiStatNodes, eventLampFailWarningBiStatHandler);

    auto eventLampFailWarningDrlStatHandler =
        std::bind(&AppService::onEventLampFailWarningDrlStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningDrlStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailDrlLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailDrlRh.Stat};
    addSubscriptions(eventLampFailWarningDrlStatNodes, eventLampFailWarningDrlStatHandler);

    auto eventLampFailWarningFogStatHandler =
        std::bind(&AppService::onEventLampFailWarningFogStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningFogStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogRh.Stat};
    addSubscriptions(eventLampFailWarningFogStatNodes, eventLampFailWarningFogStatHandler);

    auto eventLampFailWarningHighStatHandler =
        std::bind(&AppService::onEventLampFailWarningHighStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningHighStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailHighLh.Stat,
                                                               SFC.Lamp_Fail_Warning.Constant.LampFailHighRh.Stat};
    addSubscriptions(eventLampFailWarningHighStatNodes, eventLampFailWarningHighStatHandler);

    auto eventLampFailWarningLowStatHandler =
        std::bind(&AppService::onEventLampFailWarningLowStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningLowStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailLowLh.Stat,
                                                              SFC.Lamp_Fail_Warning.Constant.LampFailLowRh.Stat};
    addSubscriptions(eventLampFailWarningLowStatNodes, eventLampFailWarningLowStatHandler);

    auto eventLampFailWarningCornerStatHandler =
        std::bind(&AppService::onEventLampFailWarningCornerStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningCornerStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailCornerLh.Stat,
                                                                 SFC.Lamp_Fail_Warning.Constant.LampFailCornerRh.Stat};
    addSubscriptions(eventLampFailWarningCornerStatNodes, eventLampFailWarningCornerStatHandler);

    auto eventLampFailWarningStopStatHandler =
        std::bind(&AppService::onEventLampFailWarningStopStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningStopStatNodes{SFC.Lamp_Fail_Warning.Constant.LampFailStopLh.Stat,
                                                               SFC.Lamp_Fail_Warning.Constant.LampFailStopRh.Stat};
    addSubscriptions(eventLampFailWarningStopStatNodes, eventLampFailWarningStopStatHandler);

    auto eventLampFailWarningTurnStatHandler =
        std::bind(&AppService::onEventLampFailWarningBiStatChanged, this, std::placeholders::_1);
    std::vector<std::string> eventLampFailWarningTurnStatNodes{
        SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnLh.Stat, SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnRh.Stat,
        SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnLh.Stat, SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnRh.Stat};
    addSubscriptions(eventLampFailWarningTurnStatNodes, eventLampFailWarningTurnStatHandler);

    /*Intro Outro*/
    auto eventIntroOutroGoodbyeDriveCompleteDistanceStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteDistanceStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Stat,
                    eventIntroOutroGoodbyeDriveCompleteDistanceStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteDistanceValueHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Value,
                    eventIntroOutroGoodbyeDriveCompleteDistanceValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteTimeStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteTimeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Stat, eventIntroOutroGoodbyeDriveCompleteTimeStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteTimeValueHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteTimeValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Value,
                    eventIntroOutroGoodbyeDriveCompleteTimeValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteSpeedStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Stat,
                    eventIntroOutroGoodbyeDriveCompleteSpeedStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteSpeedValueHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Value,
                    eventIntroOutroGoodbyeDriveCompleteSpeedValueHandler);

    auto eventIntroOutroGoodbyeDriveCompleteFuelEcoStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteFuelEcoStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Stat,
                    eventIntroOutroGoodbyeDriveCompleteFuelEcoStatHandler);

    auto eventIntroOutroGoodbyeDriveCompleteFuelEcoValueHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeDriveCompleteFuelEcoValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Value,
                    eventIntroOutroGoodbyeDriveCompleteFuelEcoValueHandler);

    auto eventIntroOutroGoodbyeResvChargeStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeResvChargeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.ResvCharge.Stat, eventIntroOutroGoodbyeResvChargeStatHandler);

    auto eventIntroOutroGoodbyeResvClimateStatHandler =
        std::bind(&AppService::onEventIntroOutroGoodbyeResvClimateStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Intro_Outro.Constant.ResvClimate.Stat, eventIntroOutroGoodbyeResvClimateStatHandler);

    /* Illumination */
    auto eventIlluminationRheostatLevelHandler =
        std::bind(&AppService::onEventIlluminationRheostatLevelChanged, this, std::placeholders::_1);
    addSubscription(SFC.Illumination.Constant.RheostatLevel.Stat, eventIlluminationRheostatLevelHandler);

    auto eventIlluminationClusterLightingLevelHandler =
        std::bind(&AppService::onEventIlluminationClusterLightingLevelChanged, this, std::placeholders::_1);
    addSubscription(SFC.Illumination.Constant.ClusterLightingLevelStat.value, eventIlluminationClusterLightingLevelHandler);

    /* Service Required Warning */
    auto eventServiceRequiredWarningHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindChanged, this, std::placeholders::_1);
    std::vector<std::string> eventServiceRequiredWarningNodes{SFC.Service_Required_Warning.Event.ServiceRemind.ID,
                                                              SFC.Service_Required_Warning.Event.ServiceRemind.Stat};
    addSubscriptions(eventServiceRequiredWarningNodes, eventServiceRequiredWarningHandler);

    /* Service Required Warning Constant*/
    auto eventServiceRequiredWarningServiceRemindDayStatHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDayStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Stat,
                    eventServiceRequiredWarningServiceRemindDayStatHandler);

    auto eventServiceRequiredWarningServiceRemindDayValueHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDayValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Value,
                    eventServiceRequiredWarningServiceRemindDayValueHandler);

    auto eventServiceRequiredWarningServiceRemindDistKmStatHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDistKmStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Stat,
                    eventServiceRequiredWarningServiceRemindDistKmStatHandler);

    auto eventServiceRequiredWarningServiceRemindDistKmValueHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDistKmValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Value,
                    eventServiceRequiredWarningServiceRemindDistKmValueHandler);

    auto eventServiceRequiredWarningServiceRemindDistMiStatHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDistMiStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Stat,
                    eventServiceRequiredWarningServiceRemindDistMiStatHandler);

    auto eventServiceRequiredWarningServiceRemindDistMiValueHandler =
        std::bind(&AppService::onEventServiceRequiredWarningServiceRemindDistMiValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Value,
                    eventServiceRequiredWarningServiceRemindDistMiValueHandler);

    /* ADAS_Parking_BVM_New */
    auto evnetAdasParkingBVMNewLeftOutMirrorFoldStatHandler =
        std::bind(&AppService::onEventADASParkingBVMNEwLeftOutMirrorFoldStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Parking_BVM_NEW.Constant.BVMLeftOutMirrorFold.Stat,
                    evnetAdasParkingBVMNewLeftOutMirrorFoldStatHandler);

    auto evnetAdasParkingBVMNewRightOutMirrorFoldStatHandler =
        std::bind(&AppService::onEventADASParkingBVMNEwRightOutMirrorFoldStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Parking_BVM_NEW.Constant.BVMRightOutMirrorFold.Stat,
                    evnetAdasParkingBVMNewRightOutMirrorFoldStatHandler);

    /* ADAS_PARKING_NEW */
    auto contentsADASParkingNEWParkCarBodyDisplayHandler =
        std::bind(&AppService::onEventADASParkingNEWParkCarBodyDisplayChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkCarBodyDisplay.Stat, contentsADASParkingNEWParkCarBodyDisplayHandler);

    auto contentsADASParkingNEWParkFrontLeftWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkFrontLeftWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontLeftWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontLeftWarning3PartHandler);

    auto contentsADASParkingNEWParkFrontCenterWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkFrontCenterWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontCenterWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontCenterWarning3PartHandler);

    auto contentsADASParkingNEWParkRearLeftWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearLeftWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning3Part.Stat,
                    contentsADASParkingNEWParkRearLeftWarning3PartHandler);

    auto contentsADASParkingNEWParkFrontRightWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkFrontRightWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontRightWarning3Part.Stat,
                    contentsADASParkingNEWParkFrontRightWarning3PartHandler);

    auto contentsADASParkingNEWParkRearCenterWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearCenterWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearCenterWarning3Part.Stat,
                    contentsADASParkingNEWParkRearCenterWarning3PartHandler);

    auto contentsADASParkingNEWParkRearRightWarning3PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearRightWarning3PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning3Part.Stat,
                    contentsADASParkingNEWParkRearRightWarning3PartHandler);

    auto contentsADASParkingNEWParkRearLeftWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearLeftWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning2Part.Stat,
                    contentsADASParkingNEWParkRearLeftWarning2PartHandler);

    auto contentsADASParkingNEWParkRearRightWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearRightWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning2Part.Stat,
                    contentsADASParkingNEWParkRearRightWarning2PartHandler);

    auto contentsADASParkingNEWParkLeftSideWarning1PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkLeftSideWarning1PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideWarning1Part.Stat,
                    contentsADASParkingNEWParkLeftSideWarning1PartHandler);

    auto contentsADASParkingNEWParkRightSideWarning1PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRightSideWarning1PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideWarning1Part.Stat,
                    contentsADASParkingNEWParkRightSideWarning1PartHandler);

    auto contentsADASParkingNEWParkLeftSideFrontWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkLeftSideFrontWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideFrontWarning2Part.Stat,
                    contentsADASParkingNEWParkLeftSideFrontWarning2PartHandler);

    auto contentsADASParkingNEWParkRightSideFrontWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRightSideFrontWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideFrontWarning2Part.Stat,
                    contentsADASParkingNEWParkRightSideFrontWarning2PartHandler);

    auto contentsADASParkingNEWParkLeftSideRearWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkLeftSideRearWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideRearWarning2Part.Stat,
                    contentsADASParkingNEWParkLeftSideRearWarning2PartHandler);

    auto contentsADASParkingNEWParkRightSideRearWarning2PartHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRightSideRearWarning2PartChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideRearWarning2Part.Stat,
                    contentsADASParkingNEWParkRightSideRearWarning2PartHandler);

    auto contentsADASParkingNEWParkFrontMasterWarningHandler =
        std::bind(&AppService::onEventADASParkingNEWParkFrontMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkFrontMasterWarning.Stat,
                    contentsADASParkingNEWParkFrontMasterWarningHandler);

    auto contentsADASParkingNEWParkRearMasterWarningHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRearMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRearMasterWarning.Stat, contentsADASParkingNEWParkRearMasterWarningHandler);

    auto contentsADASParkingNEWParkLeftSideMasterWarningHandler =
        std::bind(&AppService::onEventADASParkingNEWParkLeftSideMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideMasterWarning.Stat,
                    contentsADASParkingNEWParkLeftSideMasterWarningHandler);

    auto contentsADASParkingNEWParkRightSideMasterWarningHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRightSideMasterWarningChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideMasterWarning.Stat,
                    contentsADASParkingNEWParkRightSideMasterWarningHandler);

    auto contentsADASParkingNEWParkPcaFrontBoundaryLaneHandler =
        std::bind(&AppService::onEventADASParkingNEWParkPcaFrontBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaFrontBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaFrontBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaRearBoundaryLaneHandler =
        std::bind(&AppService::onEventADASParkingNEWParkPcaRearBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRearBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaRearBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaleftBoundaryLaneHandler =
        std::bind(&AppService::onEventADASParkingNEWParkPcaleftBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaleftBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaleftBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPcaRightBoundaryLaneHandler =
        std::bind(&AppService::onEventADASParkingNEWParkPcaRightBoundaryLaneChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRightBoundaryLane.Stat,
                    contentsADASParkingNEWParkPcaRightBoundaryLaneHandler);

    auto contentsADASParkingNEWParkPCAWarningPhraseHandler =
        std::bind(&AppService::onEventADASParkingNEWParkPCAWarningPhraseChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkPCAWarningPhrase.Stat, contentsADASParkingNEWParkPCAWarningPhraseHandler);

    auto contentsADASParkingNEWParkRCCWLeftSymbolHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRCCWLeftSymbolChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWLeftSymbol.Stat, contentsADASParkingNEWParkRCCWLeftSymbolHandler);

    auto contentsADASParkingNEWParkRCCWRightSymbolHandler =
        std::bind(&AppService::onEventADASParkingNEWParkRCCWRightSymbolChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWRightSymbol.Stat, contentsADASParkingNEWParkRCCWRightSymbolHandler);
}

void AppService::subscribeGauge() {
    /* Speed Gauge */
    auto speedGaugeDisplaySpeedValueKPHHandler =
        std::bind(&AppService::onSpeedGaugeDisplaySpeedValueKPHChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH, speedGaugeDisplaySpeedValueKPHHandler);

    auto speedGaugeDisplaySpeedValueMPHHandler =
        std::bind(&AppService::onSpeedGaugeDisplaySpeedValueMPHChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH, speedGaugeDisplaySpeedValueMPHHandler);

    auto speedGaugeSpeedAnalogStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedAnalogStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat, speedGaugeSpeedAnalogStatHandler);

    auto speedGaugeSpeedAnalogValueHandler =
        std::bind(&AppService::onSpeedGaugeSpeedAnalogValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAnalog.Value, speedGaugeSpeedAnalogValueHandler);

    auto speedGaugeSpeedDigitalStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedDigitalStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Stat, speedGaugeSpeedDigitalStatHandler);

    auto speedGaugeSpeedDigitalValueHandler =
        std::bind(&AppService::onSpeedGaugeSpeedAnalogValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedDigital.Value, speedGaugeSpeedDigitalValueHandler);

    auto speedGaugeSpeedSubDigitalStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedSubDigitalStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat, speedGaugeSpeedSubDigitalStatHandler);

    auto speedGaugeSpeedSubDigitalValueHandler =
        std::bind(&AppService::onSpeedGaugeSpeedSubDigitalValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value, speedGaugeSpeedSubDigitalValueHandler);

    auto speedGaugeSpeedMainDisplayUnitStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedMainDisplayUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat, speedGaugeSpeedMainDisplayUnitStatHandler);

    auto speedGaugeSpeedAuxDisplayUnitStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedAuxDisplayUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat, speedGaugeSpeedAuxDisplayUnitStatHandler);

    auto speedGaugeSpeedSubDisplayStatHandler =
        std::bind(&AppService::onSpeedGaugeSpeedSubDisplayStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat, speedGaugeSpeedSubDisplayStatHandler);

    auto speedGaugeDisplaySpeedValueKPHRangeStatHandler =
        std::bind(&AppService::onSpeedGaugeDisplaySpeedValueKPHRangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Private.Speed_Gauge.DisplaySpeedValueKPHRange.Stat, speedGaugeDisplaySpeedValueKPHRangeStatHandler);

    auto speedGaugeDisplaySpeedUnitHandler =
        std::bind(&AppService::onSpeedGaugeDisplaySpeedUnitChanged, this, std::placeholders::_1);
    addSubscription(SFC.Speed_Gauge.Inter_DisplaySpeedUnit, speedGaugeDisplaySpeedUnitHandler);

    /* Fuel System */
    auto fuelSystemGaugeHandler = std::bind(&AppService::onFuelSystemFuelGaugeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Fuel_System.Constant.FuelGauge.Value, fuelSystemGaugeHandler);
}

void AppService::subscribeContents() {
#ifdef VSM_Working
    /* OAT */
    auto contentsOATHandler = std::bind(&AppService::onContentsOATChanged, this, std::placeholders::_1);
    std::vector<std::string> constentsOATHandlerNodes{SFC.OAT.Constant.OutTempDisplay.Stat,
                                                      SFC.OAT.Constant.OutTempDisplayUnit.Stat};
    addSubscriptions(constentsOATHandlerNodes, contentsOATHandler);

    /* DIS */
    auto contentsDISMainSpeedHandler = std::bind(&AppService::onContentsDISMainSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Constant.DISMainSpeed.Stat, contentsDISMainSpeedHandler);

    auto contentsDISMainSpeedValueHandler =
        std::bind(&AppService::onContentsDISMainSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Constant.DISMainSpeed.Value, contentsDISMainSpeedValueHandler);

    auto contentsDISAddtnlSignHandler = std::bind(&AppService::onContentsDISAddtnlSignChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Inter_DISAddtnlSign, contentsDISAddtnlSignHandler);

    auto contentsDISConfigNaviDisCountryHandler =
        std::bind(&AppService::onContentsDISNaviDisCountryChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_DIS.Inter_ConfigNaviDisCountry, contentsDISConfigNaviDisCountryHandler);

    /* ADAS_Driving_SEA */
    auto contentsSEALeftWarnSymbolHandler =
        std::bind(&AppService::onContentsSEASEALeftWarnSymbolStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_SEA.Constant.SEALeftWarnSymbol.Stat, contentsSEALeftWarnSymbolHandler);

    auto contentsSEARightWarnSymbolHandler =
        std::bind(&AppService::onContentsSEASEARightWarnSymbolStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_SEA.Constant.SEARightWarnSymbol.Stat, contentsSEARightWarnSymbolHandler);

    /* SOC_Gauge */
    auto contentsSOCGaugeValueHandler = std::bind(&AppService::onContentsSOCGaugeValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeValue, contentsSOCGaugeValueHandler);

    auto contentsSOCGaugeLowAlertStatHandler =
        std::bind(&AppService::onContentsSOCGaugeLowAlertStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeLowAlertStat, contentsSOCGaugeLowAlertStatHandler);

    /* ADAS_Driving */
    auto contentsCruiseHDASymbolStatHandler =
        std::bind(&AppService::onContentsCruiseHDASymbolStatChanged, this, std::placeholders::_1);
    std::vector<std::string> constentsCruiseHDASymbolStatHandlerNodes{SFC.ADAS_Driving.Constant.CruiseHDASymbol.Stat,
                                                                      SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueA,
                                                                      SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueB};
    addSubscriptions(constentsCruiseHDASymbolStatHandlerNodes, contentsCruiseHDASymbolStatHandler);

    auto contentsSettingSpeedValueStatHandler =
        std::bind(&AppService::onConstantSettingSpeedValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeed.ValueStat, contentsSettingSpeedValueStatHandler);

    auto contentsSettingSpeedValueHandler =
        std::bind(&AppService::onConstantSettingSpeedValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeed.Value, contentsSettingSpeedValueHandler);

    auto contentsSettingSpeedColorStatHandler =
        std::bind(&AppService::onConstantSettingSpeedColorStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsSettingSpeedColorStatHandlerNodes{SFC.ADAS_Driving.Constant.SettingSpeed.Color.Stat,
                                                                       SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueA,
                                                                       SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueB};
    addSubscriptions(contentsSettingSpeedColorStatHandlerNodes, contentsSettingSpeedColorStatHandler);

    auto contentsSettingSpeedUnitStatHandler =
        std::bind(&AppService::onConstantSettingSpeedUnitStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.SettingSpeedUnit.Stat, contentsSettingSpeedUnitStatHandler);

    auto contentsSettingSpeedBlinkingStatHandler =
        std::bind(&AppService::onConstantSettingSpeedBlinkingStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsSettingSpeedBlinkingStatHandlerNodes{
        SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.Stat, SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueA,
        SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueB};
    addSubscriptions(contentsSettingSpeedBlinkingStatHandlerNodes, contentsSettingSpeedBlinkingStatHandler);

    auto contentsFrontVehicleStatHandler = std::bind(&AppService::onConstantFrontVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.FrontVehicle.Stat, contentsFrontVehicleStatHandler);

    auto contentsDistanceFromFrontVehicleStatHandler =
        std::bind(&AppService::onConstantDistanceFromFrontVehicleStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsDistanceFromFrontVehicleStatHandlerNodes{
        SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.Stat, SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueA,
        SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueB};
    addSubscriptions(contentsDistanceFromFrontVehicleStatHandlerNodes, contentsDistanceFromFrontVehicleStatHandler);

    auto contentsNaviSetSymbolStatHandler =
        std::bind(&AppService::onConstantNaviSetSymbolStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsNaviSetSymbolStatHandlerNodes{SFC.ADAS_Driving.Constant.NaviSetSymbol.Stat,
                                                                   SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueA,
                                                                   SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueB};
    addSubscriptions(contentsNaviSetSymbolStatHandlerNodes, contentsNaviSetSymbolStatHandler);

    auto contentsISLADirectionStatHandler =
        std::bind(&AppService::onConstantISLADirectionStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.ISLADirection.Stat, contentsISLADirectionStatHandler);

    auto contentsLCAStatHandler = std::bind(&AppService::onConstantLCAStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsLCAStatHandlerNodes{
        SFC.ADAS_Driving.Constant.LCA.Stat, SFC.ADAS_Driving.Constant.LCA.BlinkValueA, SFC.ADAS_Driving.Constant.LCA.BlinkValueB};
    addSubscriptions(contentsLCAStatHandlerNodes, contentsLCAStatHandler);

    auto contentsLFAStatHandler = std::bind(&AppService::onConstantLFAStatChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsLFAStatHandlerNodes{
        SFC.ADAS_Driving.Constant.LFA.Stat, SFC.ADAS_Driving.Constant.LFA.BlinkValueA, SFC.ADAS_Driving.Constant.LFA.BlinkValueB};
    addSubscriptions(contentsLFAStatHandlerNodes, contentsLFAStatHandler);

    auto contentsAdasOnStatusHandler = std::bind(&AppService::onConstnatAdasOnStatusChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Inter_AdasOnStatus, contentsAdasOnStatusHandler);

    /* ADAS_Driving_ISLA */
    auto contentsISLAMainSpeedStatHandler =
        std::bind(&AppService::onContentsISLAMainSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed.Stat, contentsISLAMainSpeedStatHandler);

    auto contentsISLAMainSpeedBlinkHandler =
        std::bind(&AppService::onContentsISLAMainSpeedBlinkChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAMainSpeedBlinkNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Stat,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueA,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueB};
    addSubscriptions(contentsISLAMainSpeedBlinkNodes, contentsISLAMainSpeedBlinkHandler);

    auto contentsISLAMainSpeedBlinkColorHandler =
        std::bind(&AppService::onContentsISLAMainSpeedBlinkColorChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Color, contentsISLAMainSpeedBlinkColorHandler);

    auto contentsISLAMainSpeedValueHandler =
        std::bind(&AppService::onContentsISLAMainSpeedValueChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAMainSpeedValueNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Stat,
                                                             SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Value};
    addSubscriptions(contentsISLAMainSpeedValueNodes, contentsISLAMainSpeedValueHandler);

    auto contentsISLAFinalAddtnlSignHandler =
        std::bind(&AppService::onContentsISLAFinalAddtnlSignChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAFinalAddtnlSignNodes{SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.Stat,
                                                              SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.ISLAAddtnlSignValue,
                                                              SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.DISAddtnlSignValue};
    addSubscriptions(contentsISLAFinalAddtnlSignNodes, contentsISLAFinalAddtnlSignHandler);

    auto contentsISLAOverlapSignStatHandler =
        std::bind(&AppService::onContentsISLAOverlapSignStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSign.Stat, contentsISLAOverlapSignStatHandler);

    auto contentsISLAOverlapSignBlinkHandler =
        std::bind(&AppService::onContentsISLAOverlapSignBlinkChanged, this, std::placeholders::_1);
    std::vector<std::string> contentsISLAOverlapSignBlinkNodes{SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.Stat,
                                                               SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueA,
                                                               SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueB};
    addSubscriptions(contentsISLAOverlapSignBlinkNodes, contentsISLAOverlapSignBlinkHandler);
#endif
}

void AppService::subscribeMainView() {
#ifdef VSM_Working
    /* TBT */
    auto tbtNaviAliveHandler = std::bind(&AppService::onMainViewTBTNaviAliveStatusChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Inter_NaviAliveStatus, tbtNaviAliveHandler);

    auto tbtConfigNaviMapCountryHandler =
        std::bind(&AppService::onMainViewTBTConfigNaviMapCountryChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Inter_ConfigNaviMapCountry, tbtConfigNaviMapCountryHandler);

    auto tbtNaviTbtStatHandler = std::bind(&AppService::onMainViewTBTNaviTBTStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.NaviTbt.Stat, tbtNaviTbtStatHandler);

    auto tbtDirectionTypeHandler = std::bind(&AppService::onMainViewTBTDirectionTypeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.DirectionType.Stat, tbtDirectionTypeHandler);

    auto tbtSideStreetBitMaskHandler = std::bind(&AppService::onMainViewTBTSideStreetBitMaskChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.SideStreet.Bitmask, tbtSideStreetBitMaskHandler);

    auto tbtCombinedSideStreetStatHandler =
        std::bind(&AppService::onMainViewTBTCombinedSideStreetChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.CombinedSideStreet.Stat, tbtCombinedSideStreetStatHandler);

    auto tbtNaviFlagTypeHandler = std::bind(&AppService::onMainViewTBTNaviPointFlagTypeChanged, this, std::placeholders::_1);
    addSubscription(SFC.Navigation_TBT.Constant.NaviPointFlagType.Stat, tbtNaviFlagTypeHandler);

    auto tbtNaviDistToDestHandler = std::bind(&AppService::onMainViewTBTNaviDistToDestChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtNaviDistToDestNodes{SFC.Navigation_TBT.Constant.NaviDistToDest.Stat,
                                                    SFC.Navigation_TBT.Constant.NaviDistToDest.Value,
                                                    SFC.Navigation_TBT.Constant.NaviDistUnit.Stat};
    addSubscriptions(tbtNaviDistToDestNodes, tbtNaviDistToDestHandler);

    auto tbtDistToTurnPointHandler = std::bind(&AppService::onMainViewTBTNaviDistToTurnPointChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurnPointNodes{SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Stat,
                                                     SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Value,
                                                     SFC.Navigation_TBT.Constant.TbtDistanceUnit.Stat};
    addSubscriptions(tbtDistToTurnPointNodes, tbtDistToTurnPointHandler);

    auto tbtBarGraphHandler = std::bind(&AppService::onMainViewTBTBarGraphChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtBarGraphNodes{SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Stat,
                                              SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Value};
    addSubscriptions(tbtBarGraphNodes, tbtBarGraphHandler);

    auto tbtDistToTurn1Handler = std::bind(&AppService::onMainViewTBTNaviDistToTurn1Changed, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurn1Nodes{SFC.Navigation_TBT.Constant.NaviDistToTurn1.Stat,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurn1.Value,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurnUnit1.Stat};
    addSubscriptions(tbtDistToTurn1Nodes, tbtDistToTurn1Handler);

    auto tbtDistToTurn2Handler = std::bind(&AppService::onMainViewTBTNaviDistToTurn2Changed, this, std::placeholders::_1);
    std::vector<std::string> tbtDistToTurn2Nodes{SFC.Navigation_TBT.Constant.NaviDistToTurn2.Stat,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurn2.Value,
                                                 SFC.Navigation_TBT.Constant.NaviDistToTurnUnit2.Stat};
    addSubscriptions(tbtDistToTurn2Nodes, tbtDistToTurn2Handler);

    auto tbtNaviLaneRemainDistHandler =
        std::bind(&AppService::onMainViewTBTNaviLaneRemainDistChanged, this, std::placeholders::_1);
    std::vector<std::string> tbtNaviLaneRemainDistNodes{SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Stat,
                                                        SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Value,
                                                        SFC.Navigation_TBT.Constant.NaviLaneRemainDistUnit.Stat};
    addSubscriptions(tbtNaviLaneRemainDistNodes, tbtNaviLaneRemainDistHandler);

    /* ADAS_Driving*/
    auto leftSideLaneColorHandler = std::bind(&AppService::onMainViewADASLeftSideLaneColorChanged, this, std::placeholders::_1);
    std::vector<std::string> leftSideLaneColorNodes{SFC.ADAS_Driving.Constant.LeftSideLane.Color.Stat,
                                                    SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueA,
                                                    SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueB};
    addSubscriptions(leftSideLaneColorNodes, leftSideLaneColorHandler);

    auto leftSideLaneLateralValueHandler =
        std::bind(&AppService::onMainViewADASLeftSideLaneLateralValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.Value, leftSideLaneLateralValueHandler);

    auto leftSideLaneLateralValueStatHandler =
        std::bind(&AppService::onMainViewADASLeftSideLaneLateralValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.ValueStat, leftSideLaneLateralValueStatHandler);

    auto leftSideLaneProgressInLaneChangeStatHandler =
        std::bind(&AppService::onMainViewADASLeftSideLaneProgressInLaneChangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftSideLane.ProgressInLaneChange.Stat,
                    leftSideLaneProgressInLaneChangeStatHandler);

    auto rightSideLaneColorHandler = std::bind(&AppService::onMainViewADASRightSideLaneColorChanged, this, std::placeholders::_1);
    std::vector<std::string> rightSideLaneColorNodes{SFC.ADAS_Driving.Constant.RightSideLane.Color.Stat,
                                                     SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueA,
                                                     SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueB};
    addSubscriptions(rightSideLaneColorNodes, rightSideLaneColorHandler);

    auto rightSideLaneLateralValueHandler =
        std::bind(&AppService::onMainViewADASRightSideLaneLateralValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.Value, rightSideLaneLateralValueHandler);

    auto rightSideLaneLateralValueStatHandler =
        std::bind(&AppService::onMainViewADASRightSideLaneLateralValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.ValueStat, rightSideLaneLateralValueStatHandler);

    auto rightSideLaneProgressInLaneChangeStatHandler =
        std::bind(&AppService::onMainViewADASRightSideLaneProgressInLaneChangeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RightSideLane.ProgressInLaneChange.Stat,
                    rightSideLaneProgressInLaneChangeStatHandler);

    auto leftRightSideLaneCurvationValueHandler =
        std::bind(&AppService::onMainViewADASLeftRightSideLaneCurvationValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.Value, leftRightSideLaneCurvationValueHandler);

    auto leftRightSideLaneCurvationValueStatHandler =
        std::bind(&AppService::onMainViewADASLeftRightSideLaneCurvationValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.ValueStat, leftRightSideLaneCurvationValueStatHandler);

    auto vehicleFrontVehicleStatusStatHandler =
        std::bind(&AppService::onMainViewADASVehicleFrontVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.VehicleStatus.Stat, vehicleFrontVehicleStatusStatHandler);

    auto vehicleFrontLongitudinalDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleFrontLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.Value,
                    vehicleFrontLongitudinalDistanceValueHandler);

    auto vehicleFrontLongitudinalDistanceValueStatHandler =
        std::bind(&AppService::onMainViewADASVehicleFrontLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.ValueStat,
                    vehicleFrontLongitudinalDistanceValueStatHandler);

    auto vehicleFrontLateralDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleFrontLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.Value, vehicleFrontLateralDistanceValueHandler);

    auto vehicleFrontLateralDistanceValueStatHandler =
        std::bind(&AppService::onMainViewADASVehicleFrontLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.ValueStat,
                    vehicleFrontLateralDistanceValueStatHandler);

    auto vehicleLeftForegroundVehicleStatusStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftForegroundVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.Stat,
                    vehicleLeftForegroundVehicleStatusStatHandler);

    auto vehicleLeftForegroundVehicleStatusFadeStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftForegroundVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.FadeStat,
                    vehicleLeftForegroundVehicleStatusFadeStatHandler);

    auto vehicleLeftForegroundLongitudinalDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.Value,
                    vehicleLeftForegroundLongitudinalDistanceValueHandler);

    auto vehicleLeftForegroundLongitudinalDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.ValueStat,
                    vehicleLeftForegroundLongitudinalDistanceValueStatHandler);

    auto vehicleLeftForegroundLateralDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftForegroundLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.Value,
                    vehicleLeftForegroundLateralDistanceValueHandler);

    auto vehicleLeftForegroundLateralDistanceValueStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftForegroundLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.ValueStat,
                    vehicleLeftForegroundLateralDistanceValueStatHandler);

    auto vehicleRightForegroundVehicleStatusStatHandler =
        std::bind(&AppService::onMainViewADASVehicleRightForegroundVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.Stat,
                    vehicleRightForegroundVehicleStatusStatHandler);

    auto vehicleRightForegroundVehicleStatusFadeStatHandler =
        std::bind(&AppService::onMainViewADASVehicleRightForegroundVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.FadeStat,
                    vehicleRightForegroundVehicleStatusFadeStatHandler);

    auto vehicleRightForegroundLongitudinalDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.Value,
                    vehicleRightForegroundLongitudinalDistanceValueHandler);

    auto vehicleRightForegroundLongitudinalDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.ValueStat,
                    vehicleRightForegroundLongitudinalDistanceValueStatHandler);

    auto vehicleRightForegroundLateralDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleRightForegroundLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.Value,
                    vehicleRightForegroundLateralDistanceValueHandler);

    auto vehicleRightForegroundLateralDistanceValueStatHandler =
        std::bind(&AppService::onMainViewADASVehicleRightForegroundLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.ValueStat,
                    vehicleRightForegroundLateralDistanceValueStatHandler);

    auto vehicleLeftRearwardVehicleStatusStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftRearwardVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.Stat,
                    vehicleLeftRearwardVehicleStatusStatHandler);

    auto vehicleLeftRearwardVehicleStatusFadeStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftRearwardVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.FadeStat,
                    vehicleLeftRearwardVehicleStatusFadeStatHandler);

    auto vehicleLeftRearwardLongitudinalDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.Value,
                    vehicleLeftRearwardLongitudinalDistanceValueHandler);

    auto vehicleLeftRearwardLongitudinalDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.ValueStat,
                    vehicleLeftRearwardLongitudinalDistanceValueStatHandler);

    auto vehicleLeftRearwardLateralDistanceValueHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftRearwardLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.Value,
                    vehicleLeftRearwardLateralDistanceValueHandler);

    auto vehicleLeftRearwardLateralDistanceValueStatHandler =
        std::bind(&AppService::onMainViewADASVehicleLeftRearwardLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.ValueStat,
                    vehicleLeftRearwardLateralDistanceValueStatHandler);

    auto rearRightHandSideVehicleVehicleStatusStatHandler =
        std::bind(&AppService::onMainViewADASRearRightHandSideVehicleVehicleStatusStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.Stat,
                    rearRightHandSideVehicleVehicleStatusStatHandler);

    auto rearRightHandSideVehicleVehicleStatusFadeStatHandler =
        std::bind(&AppService::onMainViewADASRearRightHandSideVehicleVehicleStatusFadeStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.FadeStat,
                    rearRightHandSideVehicleVehicleStatusFadeStatHandler);

    auto rearRightHandSideVehicleLongitudinalDistanceValueHandler = std::bind(
        &AppService::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.Value,
                    rearRightHandSideVehicleLongitudinalDistanceValueHandler);

    auto rearRightHandSideVehicleLongitudinalDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.ValueStat,
                    rearRightHandSideVehicleLongitudinalDistanceValueStatHandler);

    auto rearRightHandSideVehicleLateralDistanceValueHandler =
        std::bind(&AppService::onMainViewADASRearRightHandSideVehicleLateralDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.Value,
                    rearRightHandSideVehicleLateralDistanceValueHandler);

    auto rearRightHandSideVehicleLateralDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASRearRightHandSideVehicleLateralDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.ValueStat,
                    rearRightHandSideVehicleLateralDistanceValueStatHandler);

    auto theRearLeftHandSideVehicleStatHandler =
        std::bind(&AppService::onMainViewADASTheRearLeftHandSideVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TheRearLeftHandSideVehicle.Stat, theRearLeftHandSideVehicleStatHandler);

    auto theRearRightHandSideVehicleStatHandler =
        std::bind(&AppService::onMainViewADASTheRearRightHandSideVehicleStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TheRearRightHandSideVehicle.Stat, theRearRightHandSideVehicleStatHandler);

    auto leftRearSideCrashAlarmWaveHandler =
        std::bind(&AppService::onMainViewADASLeftRearSideCrashAlarmWaveChanged, this, std::placeholders::_1);
    std::vector<std::string> leftRearSideCrashAlarmWaveNodes{SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.Stat,
                                                             SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueA,
                                                             SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueB};
    addSubscriptions(leftRearSideCrashAlarmWaveNodes, leftRearSideCrashAlarmWaveHandler);

    auto leftRearSideCrashAlarmMasterSymbolHandler =
        std::bind(&AppService::onMainViewADASLeftRearSideCrashAlarmMasterSymbolChanged, this, std::placeholders::_1);
    std::vector<std::string> leftRearSideCrashAlarmMasterSymbolNodes{
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.Stat,
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueA,
        SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueB};
    addSubscriptions(leftRearSideCrashAlarmMasterSymbolNodes, leftRearSideCrashAlarmMasterSymbolHandler);

    auto rightRearSideCrashAlarmWaveHandler =
        std::bind(&AppService::onMainViewADASRightRearSideCrashAlarmWaveChanged, this, std::placeholders::_1);
    std::vector<std::string> rightRearSideCrashAlarmWaveNodes{SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.Stat,
                                                              SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueA,
                                                              SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueB};
    addSubscriptions(rightRearSideCrashAlarmWaveNodes, rightRearSideCrashAlarmWaveHandler);

    auto rightRearSideCrashAlarmMasterSymbolHandler =
        std::bind(&AppService::onMainViewADASRightRearSideCrashAlarmMasterSymbolChanged, this, std::placeholders::_1);
    std::vector<std::string> rightRearSideCrashAlarmMasterSymbolNodes{
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.Stat,
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueA,
        SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueB};
    addSubscriptions(rightRearSideCrashAlarmMasterSymbolNodes, rightRearSideCrashAlarmMasterSymbolHandler);

    auto targetIntervalDistanceStaHandler =
        std::bind(&AppService::onMainViewADASTargetIntervalDistanceStaChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.Stat, targetIntervalDistanceStaHandler);

    auto targetIntervalDistanceLongitudinalDistanceValueHandler =
        std::bind(&AppService::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.Value,
                    targetIntervalDistanceLongitudinalDistanceValueHandler);

    auto targetIntervalDistanceLongitudinalDistanceValueStatHandler = std::bind(
        &AppService::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.ValueStat,
                    targetIntervalDistanceLongitudinalDistanceValueStatHandler);

    auto centerLineStatHandler = std::bind(&AppService::onMainViewADASCenterLineStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving.Constant.CenterLine.Stat, centerLineStatHandler);

    /* ADAS_Driving_NEW */

    auto adasDrivingNewFrontVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewFrontVehicleNodes{SFC.ADAS_Driving_New.Constant.FrontVehicle.Color.Stat,
                                                             SFC.ADAS_Driving_New.Constant.FrontVehicle.Level.Value};
    addSubscriptions(adasDrivingNewFrontVehicleNodes, adasDrivingNewFrontVehicleHandler);

    auto adasDrivingNewHeadwayDistanceHandler =
        std::bind(&AppService::onMainViewADASDrivingNewHeadwayDistanceChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewHeadwayDistanceNodes{SFC.ADAS_Driving_New.Constant.HeadwayDistance.Color.Stat,
                                                                SFC.ADAS_Driving_New.Constant.HeadwayDistance.Level.Value};
    addSubscriptions(adasDrivingNewHeadwayDistanceNodes, adasDrivingNewHeadwayDistanceHandler);

    auto adasDrivingNewSubVehicleColorStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewSubVehicleColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.SubVehicle.Color.Stat, adasDrivingNewSubVehicleColorStatHandler);

    auto adasDrivingNewSetSpeedHandler =
        std::bind(&AppService::onMainViewADASDrivingNewSetSpeedChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewSetSpeedNodes{SFC.ADAS_Driving_New.Constant.SetSpeed.Color.Stat,
                                                         SFC.ADAS_Driving_New.Constant.SetSpeed.Value,
                                                         SFC.ADAS_Driving_New.Constant.SetSpeed.Stat};
    addSubscriptions(adasDrivingNewSetSpeedNodes, adasDrivingNewSetSpeedHandler);

    auto adasDrivingNewIslaManualSetSpeedStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewISLAManualSetSpeedStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ISLAManualSetSpeed.Stat, adasDrivingNewIslaManualSetSpeedStatHandler);

    auto adasDrivingNewLaneCentralLineStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewLaneCentralLineStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.LaneCentralLine.Stat, adasDrivingNewLaneCentralLineStatHandler);

    auto adasDrivingNewLeftLaneHandler =
        std::bind(&AppService::onMainViewADASDrivingNewLeftLaneChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewLeftLaneNodes{SFC.ADAS_Driving_New.Constant.LeftLane.Color.Stat,
                                                         SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Value,
                                                         SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Stat};
    addSubscriptions(adasDrivingNewLeftLaneNodes, adasDrivingNewLeftLaneHandler);

    auto adasDrivingNewRightLaneHandler =
        std::bind(&AppService::onMainViewADASDrivingNewRightLaneChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRightLaneNodes{SFC.ADAS_Driving_New.Constant.RightLane.Color.Stat,
                                                          SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Value,
                                                          SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Stat};
    addSubscriptions(adasDrivingNewRightLaneNodes, adasDrivingNewRightLaneHandler);

    auto adasDrivingNewCvrtLvlHandler =
        std::bind(&AppService::onMainViewADASDrivingNewLaneCvrtLvlChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCvrtLvlNodes{SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Value,
                                                        SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Stat};
    addSubscriptions(adasDrivingNewCvrtLvlNodes, adasDrivingNewCvrtLvlHandler);

    auto adasDrivingNewHeadwayDistHandler =
        std::bind(&AppService::onMainViewADASDrivingNewHeadwayDistChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewHeadwayDistNodes{SFC.ADAS_Driving_New.Constant.HeadwayDist.Color.Stat,
                                                            SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Value,
                                                            SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Stat};
    addSubscriptions(adasDrivingNewHeadwayDistNodes, adasDrivingNewHeadwayDistHandler);

    auto adasDrivingNewCenterRoadHandler =
        std::bind(&AppService::onMainViewADASDrivingNewCenterRoadChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadNodes{SFC.ADAS_Driving_New.Constant.CenterRoad.Color.Stat,
                                                           SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Value,
                                                           SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Stat};
    addSubscriptions(adasDrivingNewCenterRoadNodes, adasDrivingNewCenterRoadHandler);

    auto adasDrivingNewLeftRoadColorHandler =
        std::bind(&AppService::onMainViewADASDrivingNewLeftRoadColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.LeftRoad.Color.Stat, adasDrivingNewLeftRoadColorHandler);

    auto adasDrivingNewRightRoadColorHandler =
        std::bind(&AppService::onMainViewADASDrivingNewRightRoadColorStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.RightRoad.Color.Stat, adasDrivingNewRightRoadColorHandler);

    auto adasDrivingNewViewFrontVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontVehicleNodes, adasDrivingNewViewFrontVehicleHandler);

    auto adasDrivingNewViewFrontLeftVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewFrontVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontLeftVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.Color.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontLeftVehicleNodes, adasDrivingNewViewFrontLeftVehicleHandler);

    auto adasDrivingNewViewFrontRightVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewFrontRightVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewFrontRightVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.Color.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewFrontRightVehicleNodes, adasDrivingNewViewFrontRightVehicleHandler);

    auto adasDrivingNewViewLeftVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewLeftVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewLeftVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewLeftVehicleNodes, adasDrivingNewViewLeftVehicleHandler);

    auto adasDrivingNewViewRightVehicleHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewRightVehicleChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewViewRightVehicleNodes{
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.Color.Stat, SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Stat, SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value,
        SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Stat};
    addSubscriptions(adasDrivingNewViewRightVehicleNodes, adasDrivingNewViewRightVehicleHandler);

    auto adasDrivingNewRearLeftWarnStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewRearLeftWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRearLeftWarn.Stat, adasDrivingNewRearLeftWarnStatHandler);

    auto adasDrivingNewRearRightWarnStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewRearRightWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRearRightWarn.Stat, adasDrivingNewRearRightWarnStatHandler);

    auto adasDrivingNewRearLeftArrowStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewRearLeftArrowStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewLeftArrow.Stat, adasDrivingNewRearLeftArrowStatHandler);

    auto adasDrivingNewRearRightArrowStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewRearRightArrowStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewRightArrow.Stat, adasDrivingNewRearRightArrowStatHandler);

    auto adasDrivingNewISLAMainHandler =
        std::bind(&AppService::onContentsADASDrivingNewISLAMainChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewISLAMainNodes{SFC.ADAS_Driving_New.Constant.ISLAMainSpeed.Stat,
                                                         SFC.ADAS_Driving_New.Constant.ISLAMainSign.Value,
                                                         SFC.ADAS_Driving_New.Constant.ISLAMainSign.Stat};
    addSubscriptions(adasDrivingNewISLAMainNodes, adasDrivingNewISLAMainHandler);

    auto adasDrivingNewFinalAddtnlSignHandler =
        std::bind(&AppService::onContentsADASDrivingNewFinalAddtnlSignChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewFinalAddtnlSignNodes{SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat,
                                                                SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat,
                                                                SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat};
    addSubscriptions(adasDrivingNewFinalAddtnlSignNodes, adasDrivingNewFinalAddtnlSignHandler);

    auto adasDrivingNewISLAAddSignStatHandler =
        std::bind(&AppService::onContentsADASDrivingNewISLAAddSignStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ISLAAddSign.Stat, adasDrivingNewISLAAddSignStatHandler);

    auto adasDrivingNewHDPStateDisplayBarStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewHDPStatDisplayBarStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.HDPStateDisplayBar.Stat, adasDrivingNewHDPStateDisplayBarStatHandler);

    auto adasDrivingNewCenterRoadLeftLatPosHandler =
        std::bind(&AppService::onMainViewADASDrivingNewCenterRoadLeftLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadLeftLatPosNodes{SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Stat,
                                                                     SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Value};
    addSubscriptions(adasDrivingNewCenterRoadLeftLatPosNodes, adasDrivingNewCenterRoadLeftLatPosHandler);

    auto adasDrivingNewCenterRoadRightLatPosHandler =
        std::bind(&AppService::onMainViewADASDrivingNewCenterRoadRightLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewCenterRoadRightLatPosNodes{SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Stat,
                                                                      SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Value};
    addSubscriptions(adasDrivingNewCenterRoadRightLatPosNodes, adasDrivingNewCenterRoadRightLatPosHandler);

    auto adasDrivingNewRoadCvrtLvlHandler =
        std::bind(&AppService::onMainViewADASDrivingNewRoadCvrtLvlChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRoadCvrtLvlNodes{SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Stat,
                                                            SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Value};
    addSubscriptions(adasDrivingNewRoadCvrtLvlNodes, adasDrivingNewRoadCvrtLvlHandler);

    auto adasDrivingNewLeftRoadLatPosHandler =
        std::bind(&AppService::onMainViewADASDrivingNewLeftRoadLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewLeftRoadLatPosNodes{SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Stat,
                                                               SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Value};
    addSubscriptions(adasDrivingNewLeftRoadLatPosNodes, adasDrivingNewLeftRoadLatPosHandler);

    auto adasDrivingNewRightRoadLatPosHandler =
        std::bind(&AppService::onMainViewADASDrivingNewRightRoadLatPosChanged, this, std::placeholders::_1);
    std::vector<std::string> adasDrivingNewRightRoadLatPosNodes{SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Stat,
                                                                SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Value};
    addSubscriptions(adasDrivingNewRightRoadLatPosNodes, adasDrivingNewRightRoadLatPosHandler);

    auto adasDrivingNewHViewFrontWarnStatHandler =
        std::bind(&AppService::onMainViewADASDrivingNewViewFrontWarnStatChanged, this, std::placeholders::_1);
    addSubscription(SFC.ADAS_Driving_New.Constant.ViewFrontWarn.Stat, adasDrivingNewHViewFrontWarnStatHandler);
#endif
}

void AppService::onTelltaleECallStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.E_Call.Telltale.Ecall.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleEcallStat,
                                            static_cast<ccos::HUInt64>(SFC.E_Call.Telltale.Ecall.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleSeatbeltWarningStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Seatbelt_Warning.Telltale.Seatbelt.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleSeatbeltStat,
                static_cast<ccos::HUInt64>(SFC.Seatbelt_Warning.Telltale.Seatbelt.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define TELLTALE_OAT_COUNT 2
void AppService::onTelltaleOATChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.OAT.Telltale.IceWarn.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleOATIceWarnStat,
                                            static_cast<ccos::HUInt64>(SFC.OAT.Telltale.IceWarn.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.OAT.Telltale.IceWarn.StatOptional) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleOATIceWarnStatOptional,
                static_cast<ccos::HUInt64>(SFC.OAT.Telltale.IceWarn.StatOptional.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == TELLTALE_OAT_COUNT) {
            break;
        }
    }
}
qstring getSfcnodeaddress(int type) {
    switch (type) {
        case TelltaleType::TelltaleAirBagStat:
            return SFC.Air_Bag.Telltale.AirBag.Stat;
        case 2:
            return SFC.Air_Bag.Telltale.AirBag.event;
        case 3:
            return SFC.Lamp_Fail_Warning.Telltale.LedLampFail.Stat;
    }
}

void AppService::onTelltaleAirBagStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Air_Bag.Telltale.AirBag.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleAirBagStat,
                                            static_cast<ccos::HUInt64>(SFC.Air_Bag.Telltale.AirBag.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampFailWarningStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Telltale.LedLampFail.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampFailWarningStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Telltale.LedLampFail.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleIFSStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_HBA.Telltale.IFS.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleIFSStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_HBA.Telltale.IFS.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltalexEVSystemReadyChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_System_Ready.Telltale.XevRdy.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltalexEVSystemReadyStat,
                static_cast<ccos::HUInt64>(SFC.xEV_System_Ready.Telltale.XevRdy.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleKeySystemChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Key_System.Telltale.Key_System.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleKeySystemStat,
                static_cast<ccos::HUInt64>(SFC.Key_System.Telltale.Key_System.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorFrontFogChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.FrontFog.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorFrontFogStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.FrontFog.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorHighBeamChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.HighBeam.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorHighBeamStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.HighBeam.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorRearFogChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.RearFog.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorRearFogStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.RearFog.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorTailLampChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.TailLamp.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorTailLampStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TailLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorTurnSignalLeftChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorTurnSignalLeftStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorTurnSignalRightChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorTurnSignalRightStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleLampIndicatorLowBeamChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Indicator.Telltale.LowBeam.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLampIndicatorLowBeamStat,
                static_cast<ccos::HUInt64>(SFC.Lamp_Indicator.Telltale.LowBeam.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleH2FaultChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.FCEV_Warning.Telltale.TelltaleH2Fault.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleH2FaultStat,
                static_cast<ccos::HUInt64>(SFC.FCEV_Warning.Telltale.TelltaleH2Fault.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleH2LeakChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.FCEV_Warning.Telltale.TelltaleH2Leak.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleH2LeakStat,
                static_cast<ccos::HUInt64>(SFC.FCEV_Warning.Telltale.TelltaleH2Leak.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define AFSDBL_VALUE_COUNT 3
void AppService::onTelltaleAFSDBLStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.AFS_DBL.Telltale.AFS_DBL.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleAFSDBLStat,
                                            static_cast<ccos::HUInt64>(SFC.AFS_DBL.Telltale.AFS_DBL.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueA) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleAFSDBLBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueB) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleAFSDBLBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.AFS_DBL.Telltale.AFS_DBL.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == AFSDBL_VALUE_COUNT) {
            return;
        }
    }
}

#define LDWLKA_VALUE_COUNT 3
void AppService::onTelltaleLDWLKAChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.LDWLKA.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLDWLKAStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.LDWLKA.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueA) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLDWLKABlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueB) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLDWLKABlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.LDWLKA.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LDWLKA_VALUE_COUNT) {
            return;
        }
    }
}

#define FCA_VALUE_COUNT 3
void AppService::onTelltaleFCAChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.FCA.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleFCAStat,
                                            static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.FCA.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.FCA.BlinkValueA) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleFCABlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.FCA.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.FCA.BlinkValueB) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleFCABlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.FCA.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == FCA_VALUE_COUNT) {
            return;
        }
    }
}

#define ELK_VALUE_COUNT 3
void AppService::onTelltaleELKChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.ELK.Stat) {
            emit signalTelltaleValueChanged(TelltaleType::TelltaleELKStat,
                                            static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.ELK.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.ELK.BlinkValueA) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleELKBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.ELK.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Telltale.ELK.BlinkValueB) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleELKBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Telltale.ELK.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ELK_VALUE_COUNT) {
            return;
        }
    }
}

#define LOWFUELWARN_VALUE_COUNT 4
void AppService::onTelltaleLowFuelWarnChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Fuel_System.Telltale.LowFuelWarn.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLowFuelWarnStat,
                static_cast<ccos::HUInt64>(SFC.Fuel_System.Telltale.LowFuelWarn.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Fuel_System.Telltale.LowFuelWarn.StatOptional) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLowFuelWarnStatOptional,
                static_cast<ccos::HUInt64>(SFC.Fuel_System.Telltale.LowFuelWarn.StatOptional.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueA) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLowFuelWarnBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueB) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleLowFuelWarnBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.Fuel_System.Telltale.LowFuelWarn.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LOWFUELWARN_VALUE_COUNT) {
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewLKAStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.LKA.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewLKAStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.LKA.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewFCAStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.FCA.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewFCAStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.FCA.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewELKStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.ELK.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewELKStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.ELK.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewHBAStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.HBA.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewHBAStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.HBA.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewLCALeftStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.LCALeft.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewLCALeftStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.LCALeft.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewLCARightStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.LCARight.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewLCARightStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.LCARight.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onTelltaleADASDrivingNewLFAStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Telltale.LFA.Stat) {
            emit signalTelltaleValueChanged(
                TelltaleType::TelltaleADASDrivingNewLFAStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Telltale.LFA.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define EVENT_VALUE_COUNT 2
#define EVENT_MASTERWARNING_VALUE_COUNT 3
void AppService::onEventFunc(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    QString eventId = "";
    ccos::HUInt64 eventStat = 0;
    ccos::HBool isMasterWarn = false;

    std::regex reId("(SFC\\.[0-9a-zA-Z_]+\\.Event\\.[0-9a-zA-Z_]+\\.ID)");
    std::regex reStat("(SFC\\.[0-9a-zA-Z_]+\\.Event\\.[0-9a-zA-Z_]+\\.Stat)");
    std::regex reMasterWarn("(SFC\\.[0-9a-zA-Z_]+\\.Event\\.[0-9a-zA-Z_]+\\.MasterWarning)");

    for (const auto& vehicleSignal : signalList) {
        if (std::regex_match(vehicleSignal.getNodePath(), reId)) {
            eventId = QString::fromStdString(vehicleSignal.getValue<std::string>());
        }

        if (std::regex_match(vehicleSignal.getNodePath(), reStat)) {
            eventStat = vehicleSignal.getValue<ccos::HUInt64>();
        }

        if (std::regex_match(vehicleSignal.getNodePath(), reMasterWarn)) {
            isMasterWarn = vehicleSignal.getValue<ccos::HUInt64>();
        }

        if (valueCnt == EVENT_MASTERWARNING_VALUE_COUNT) {
            return;
        }
    }

    emit signalPopupValueChanged(eventId, eventStat, isMasterWarn);
}

#define LAMP_FAIL_STAT_COUNT 2
#define LAMP_FAIL_TURN_STAT_COUNT 2
void AppService::onEventLampFailWarningBiStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailBiLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Bi_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailBiLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailBiRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Bi_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailBiRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningDrlStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailDrlLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Drl_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailDrlLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailDrlRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Drl_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailDrlRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningFogStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Fog_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Fog_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailFrontFogRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningHighStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailHighLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_High_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailHighLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailHighRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_High_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailHighRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningLowStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailLowLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Low_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailLowLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailLowRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Low_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailLowRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningCornerStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailCornerLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Corner_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailCornerLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailCornerRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Corner_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailCornerRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningStopStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailStopLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Stop_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailStopLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailStopRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Stop_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailStopRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventLampFailWarningTurnStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Turn_FR_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Turn_FR_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailFrontTurnRh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnLh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Turn_RR_LH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnLh.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnRh.Stat) {
            emit signalLampFailStatChanged(
                LampFailWarningStatType::LampFailWarningStat_Turn_RR_RH,
                static_cast<ccos::HUInt64>(SFC.Lamp_Fail_Warning.Constant.LampFailRearTurnRh.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == LAMP_FAIL_TURN_STAT_COUNT) {
            break;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteDistanceStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteDistanceStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Value) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteDistanceValue,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteTimeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteTimeStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteTimeValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Value) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteTimeValue,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteTime.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteSpeedStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteSpeedStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteSpeedValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Value) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteSpeedValue,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteSpeed.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteFuelEcoStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteFuelEcoStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeDriveCompleteFuelEcoValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Value) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroDriveCompleteFuelEcoValue,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.Goodbye.DriveCompleteFuelEco.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeResvChargeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.ResvCharge.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroResvChargeStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.ResvCharge.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIntroOutroGoodbyeResvClimateStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Intro_Outro.Constant.ResvClimate.Stat) {
            emit signalIntroOutroDataChanged(
                IntroOutroType::IntroOutroResvClimateStat,
                static_cast<ccos::HUInt64>(SFC.Intro_Outro.Constant.ResvClimate.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    QString eventId = "";
    ccos::HUInt64 eventStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Event.ServiceRemind.ID) {
            eventId = QString::fromStdString(SFC.Service_Required_Warning.Event.ServiceRemind.ID.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Event.ServiceRemind.Stat) {
            eventStat = static_cast<ccos::HUInt64>(SFC.Service_Required_Warning.Event.ServiceRemind.Stat.value(vehicleSignal));
            valueCnt++;
        }

        if (valueCnt == EVENT_VALUE_COUNT) {
            break;
        }
    }
    emit signalPopupValueChanged(eventId, eventStat);
}

void AppService::onEventServiceRequiredWarningServiceRemindDayStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDay.Stat) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDayStat,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindDayValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDay.Value) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDayValue,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDay.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindDistKmStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Stat) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDistKmStat,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindDistMiStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Stat) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDistMiStat,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindDistKmValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Value) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDistKmValue,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDistKm.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventServiceRequiredWarningServiceRemindDistMiValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Value) {
            emit signalServiceRequiredWarningStatChanged(
                ServiceRequiredWarningType::ServiceRemindDistMiValue,
                static_cast<ccos::HInt64>(SFC.Service_Required_Warning.Constant.ServiceRemindDistMi.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventADASParkingBVMNEwLeftOutMirrorFoldStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Parking_BVM_NEW.Constant.BVMLeftOutMirrorFold.Stat) {
            emit signalADASParkingBVMNewStatChanged(
                ADASParkingBVMNewType::LEFT,
                static_cast<ccos::HUInt64>(SFC.ADAS_Parking_BVM_NEW.Constant.BVMLeftOutMirrorFold.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventADASParkingBVMNEwRightOutMirrorFoldStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Parking_BVM_NEW.Constant.BVMRightOutMirrorFold.Stat) {
            emit signalADASParkingBVMNewStatChanged(
                ADASParkingBVMNewType::RIGHT,
                static_cast<ccos::HUInt64>(SFC.ADAS_Parking_BVM_NEW.Constant.BVMRightOutMirrorFold.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkCarBodyDisplayChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkCarBodyDisplay.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkCarBodyDisplayStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkCarBodyDisplay.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ADAS_FRONT_VEHICLE_COUNT 2
void AppService::onMainViewADASDrivingNewFrontVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.FrontVehicle.Color.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewFrontVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.FrontVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.FrontVehicle.Level.Value) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewFrontVehicleLevelValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.FrontVehicle.Level.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_FRONT_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkFrontLeftWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkFrontLeftWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkFrontLeftWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkFrontLeftWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewHeadwayDistanceChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HeadwayDistance.Color.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHeadwayDistanceColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HeadwayDistance.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HeadwayDistance.Level.Value) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHeadwayDistanceLevelValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HeadwayDistance.Level.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_FRONT_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkFrontCenterWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkFrontCenterWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkFrontCenterWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkFrontCenterWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewSubVehicleColorStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.SubVehicle.Color.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewSubVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.SubVehicle.Color.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkFrontRightWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkFrontRightWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkFrontRightWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkFrontRightWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ADAS_SPEED_COUNT 3
void AppService::onMainViewADASDrivingNewSetSpeedChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.SetSpeed.Color.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewSetSpeedColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.SetSpeed.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.SetSpeed.Value) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewSetSpeedValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.SetSpeed.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.SetSpeed.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewSetSpeedStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.SetSpeed.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_SPEED_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRearLeftWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearLeftWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewISLAManualSetSpeedStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAManualSetSpeed.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewISLAManualSetSpeedStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAManualSetSpeed.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkRearCenterWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearCenterWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearCenterWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearCenterWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewHDAHDPStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HDAHDP.Stat) {
            emit signalContentsADASDrivingNewSummaryInfoChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHDAHDPStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HDAHDP.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkRearRightWarning3PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning3Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearRightWarning3PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning3Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewHDPStatDisplayBarStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HDPStateDisplayBar.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHDPStatDisplayBarStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HDPStateDisplayBar.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkRearLeftWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearLeftWarning2PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearLeftWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewLaneCentralLineStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LaneCentralLine.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLaneCentralLineStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LaneCentralLine.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkRearRightWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearRightWarning2PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearRightWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ADAS_LANE_COUNT 3
void AppService::onMainViewADASDrivingNewLeftLaneChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftLane.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftLaneColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftLane.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftLineLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftLineLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftLine.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_LANE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkLeftSideWarning1PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideWarning1Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkLeftSideWarning1PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideWarning1Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewRightLaneChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightLane.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightLaneColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightLane.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightLineLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightLineLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightLine.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_LANE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRightSideWarning1PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRightSideWarning1Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRightSideWarning1PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideWarning1Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ADAS_LANE_CVRTLVL_COUNT 2
void AppService::onMainViewADASDrivingNewLaneCvrtLvlChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLaneCvrtLvlValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLaneCvrtLvlStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.Lane.CvrtLvl.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_LANE_CVRTLVL_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkLeftSideFrontWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideFrontWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkLeftSideFrontWarning2PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideFrontWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewHeadwayDistChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HeadwayDist.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHeadwayDistColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HeadwayDist.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHeadwayDistLongPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewHeadwayDistLongPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.HeadwayDist.LongPos.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_LANE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRightSideFrontWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRightSideFrontWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRightSideFrontWarning2PartStat,
                static_cast<ccos::HInt64>(
                    SFC.ADAS_PARKING_NEW.Constant.ParkRightSideFrontWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewCenterRoadChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoad.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoad.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadLongPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadLongPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoad.LongPos.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_LANE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkLeftSideRearWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideRearWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkLeftSideRearWarning2PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideRearWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewLeftRoadColorStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftRoad.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftRoadColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftRoad.Color.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkRightSideRearWarning2PartChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRightSideRearWarning2Part.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRightSideRearWarning2PartStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideRearWarning2Part.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewRightRoadColorStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightRoad.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightRoadColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightRoad.Color.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventADASParkingNEWParkFrontMasterWarningChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkFrontMasterWarning.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkFrontMasterWarningStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkFrontMasterWarning.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ADAS_VEHICLE_COUNT 5
void AppService::onMainViewADASDrivingNewViewFrontVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontVehicleLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontVehicleLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontVehicleLongPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontVehicleLongPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRearMasterWarningChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRearMasterWarning.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRearMasterWarningStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRearMasterWarning.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewFrontLeftVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontLeftVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontLeftVehicleLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontLeftVehicleLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontLeftVehicleLongPosValue,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontLeftVehicleLongPosStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkLeftSideMasterWarningChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideMasterWarning.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkLeftSideMasterWarningStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkLeftSideMasterWarning.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewFrontRightVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontRightVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontRightVehicleLatPosValue,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontRightVehicleLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontRightVehicleLongPosValue,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontRightVehicleLongPosStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRightSideMasterWarningChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRightSideMasterWarning.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRightSideMasterWarningStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRightSideMasterWarning.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewLeftVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftVehicleLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftVehicleLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftVehicleLongPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftVehicleLongPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkPcaFrontBoundaryLaneChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkPcaFrontBoundaryLane.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkPcaFrontBoundaryLaneStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkPcaFrontBoundaryLane.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewRightVehicleChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightVehicle.Color.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightVehicleColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightVehicle.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightVehicleLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightVehicleLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightVehicleLongPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightVehicleLongPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightVehicle.LongPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_VEHICLE_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkPcaRearBoundaryLaneChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkPcaRearBoundaryLane.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkPcaRearBoundaryLaneStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRearBoundaryLane.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewRearLeftWarnStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRearLeftWarn.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRearLeftWarnStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRearLeftWarn.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onMainViewADASDrivingNewViewRearRightWarnStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRearRightWarn.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRearRightWarnStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRearRightWarn.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onMainViewADASDrivingNewViewRearLeftArrowStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewLeftArrow.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewLeftArrowStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewLeftArrow.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onMainViewADASDrivingNewViewRearRightArrowStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewRightArrow.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewRightArrowStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewRightArrow.Stat.value(vehicleSignal)));
        }
    }
}

#define ADAS_ISLA_DIS_COUNT 3
void AppService::onContentsADASDrivingNewISLAMainChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAMainSpeed.Stat) {
            emit signalContentsADASDrivingNewISLAChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewISLAMainSpeedStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAMainSpeed.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAMainSign.Value) {
            emit signalContentsADASDrivingNewISLAChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewISLAMainSignValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAMainSign.Value.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAMainSign.Stat) {
            emit signalContentsADASDrivingNewISLAChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewISLAMainSignStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAMainSign.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ISLA_DIS_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkPcaleftBoundaryLaneChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkPcaleftBoundaryLane.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkPcaleftBoundaryLaneStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkPcaleftBoundaryLane.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsADASDrivingNewFinalAddtnlSignChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HUInt64 islaAddSignValue = 0;
    ccos::HUInt64 disAddSignValue = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat) {
            islaAddSignValue = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat) {
            disAddSignValue = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat.value(vehicleSignal));
            valueCnt++;
        }

        if (valueCnt == ADAS_ISLA_DIS_COUNT) {
            break;
        }
    }

    emit signalContentsISLAFinalAddtnSignChanged(stat, islaAddSignValue, disAddSignValue);
}

void AppService::onContentsADASDrivingNewISLAAddSignStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ISLAAddSign.Stat) {
            emit signalContentsADASDrivingNewISLAChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewISLAAddSignStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ISLAAddSign.Stat.value(vehicleSignal)));
        }
    }
}

#define ADAS_ROAD_COUNT 2
void AppService::onMainViewADASDrivingNewCenterRoadLeftLatPosChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadLeftLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadLeftLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoadLeftLatPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ROAD_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkPcaRightBoundaryLaneChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkPcaRightBoundaryLane.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkPcaRightBoundaryLaneStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkPcaRightBoundaryLane.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewCenterRoadRightLatPosChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadRightLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewCenterRoadRightLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.CenterRoadRightLatPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ROAD_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkPCAWarningPhraseChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkPCAWarningPhrase.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkPCAWarningPhraseStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkPCAWarningPhrase.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewRoadCvrtLvlChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRoadCvrtLvlStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRoadCvrtLvlValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RoadCvrtLvl.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ROAD_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRCCWLeftSymbolChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRCCWLeftSymbol.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRCCWLeftSymbolStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWLeftSymbol.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewLeftRoadLatPosChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftRoadLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewLeftRoadLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.LeftRoadLatPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ROAD_COUNT) {
            return;
        }
    }
}

void AppService::onEventADASParkingNEWParkRCCWRightSymbolChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_PARKING_NEW.Constant.ParkRCCWRightSymbol.Stat) {
            emit signalADASParkingNEWChanged(
                ADASParkingNEWType::ADASParkingNEWParkRCCWRightSymbolStat,
                static_cast<ccos::HInt64>(SFC.ADAS_PARKING_NEW.Constant.ParkRCCWRightSymbol.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewRightRoadLatPosChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightRoadLatPosStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Value) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewRightRoadLatPosValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.RightRoadLatPos.Value.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == ADAS_ROAD_COUNT) {
            return;
        }
    }
}

void AppService::onMainViewADASDrivingNewViewFrontWarnStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_New.Constant.ViewFrontWarn.Stat) {
            emit signalMainViewADASDrivingNewValueChanged(
                MainViewADASDrivingNewType::MainViewADASDrivingNewViewFrontWarnStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_New.Constant.ViewFrontWarn.Stat.value(vehicleSignal)));
        }
    }
}

void AppService::onEventIlluminationRheostatLevelChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Illumination.Constant.RheostatLevel.Stat) {
            emit signalIlluminationLevelChanged(
                EventIlluminationType::RHEOSTATLEVEL,
                static_cast<ccos::HUInt64>(SFC.Illumination.Constant.RheostatLevel.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onEventIlluminationClusterLightingLevelChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Illumination.Constant.ClusterLightingLevelStat.value) {
            emit signalIlluminationLevelChanged(
                EventIlluminationType::CLUSTERLIGHTINGLEVEL,
                static_cast<ccos::HUInt64>(SFC.Illumination.Constant.ClusterLightingLevelStat.value.value(vehicleSignal)));
            return;
        }
    }
}

#define CONTENTS_OAT_COUNT 2
void AppService::onContentsOATChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.OAT.Constant.OutTempDisplay.Stat) {
            emit signalContentsOATValueChanged(QString::fromStdString(SFC.OAT.Constant.OutTempDisplay.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.OAT.Constant.OutTempDisplayUnit.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsOATUnit,
                static_cast<ccos::HUInt64>(SFC.OAT.Constant.OutTempDisplayUnit.Stat.value(vehicleSignal)));
            valueCnt++;
        }

        if (valueCnt == CONTENTS_OAT_COUNT) {
            break;
        }
    }
}

void AppService::onContentsDISMainSpeedStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_DIS.Constant.DISMainSpeed.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsDISMainSpeedStat,
                static_cast<ccos::HUInt64>(SFC.Navigation_DIS.Constant.DISMainSpeed.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsDISMainSpeedValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_DIS.Constant.DISMainSpeed.Value) {
            emit signalContentsValueChanged(
                ContentsType::ContentsDISMainSpeedValue,
                static_cast<ccos::HUInt64>(SFC.Navigation_DIS.Constant.DISMainSpeed.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsDISAddtnlSignChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_DIS.Inter_DISAddtnlSign) {
            emit signalContentsValueChanged(
                ContentsType::ContentsDISAddtnlSign,
                static_cast<ccos::HUInt64>(SFC.Navigation_DIS.Inter_DISAddtnlSign.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsDISNaviDisCountryChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_DIS.Inter_ConfigNaviDisCountry) {
            emit signalContentsValueChanged(
                ContentsType::ContentsDISNaviDisCountry,
                static_cast<ccos::HUInt64>(SFC.Navigation_DIS.Inter_ConfigNaviDisCountry.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsSEASEALeftWarnSymbolStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_SEA.Constant.SEALeftWarnSymbol.Stat) {
            emit signalSEAWarnSymbolChanged(
                SEAWarnSymbolType::SEALeftWarnSymbolType,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_SEA.Constant.SEALeftWarnSymbol.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsSEASEARightWarnSymbolStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_SEA.Constant.SEARightWarnSymbol.Stat) {
            emit signalSEAWarnSymbolChanged(
                SEAWarnSymbolType::SEARightWarnSymbolType,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_SEA.Constant.SEARightWarnSymbol.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsSOCGaugeValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeValue) {
            emit signalContentsValueChanged(
                ContentsType::ContentsSOCGaugeValue,
                static_cast<ccos::HUInt64>(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeValue.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsSOCGaugeLowAlertStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeLowAlertStat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsSOCGaugeLowAlert,
                static_cast<ccos::HUInt64>(SFC.SOC_Gauge.Constant.SOC_Gauge.SOCGaugeLowAlertStat.value(vehicleSignal)));
            return;
        }
    }
}

#define CRUISE_HDA_COUNT 3
void AppService::onContentsCruiseHDASymbolStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.CruiseHDASymbol.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsCruiseHDASymbolstat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.CruiseHDASymbol.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ContentsCruiseHDASymbolBlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ContentsCruiseHDASymbolBlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.CruiseHDASymbol.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == CRUISE_HDA_COUNT) {
            return;
        }
    }
}

void AppService::onConstantSettingSpeedValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Value) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onConstantSettingSpeedValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.ValueStat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedValueStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

#define SPEED_COLOR_COUNT 3
void AppService::onConstantSettingSpeedColorStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Color.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedColorBlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedColorBlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Color.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == SPEED_COLOR_COUNT) {
            return;
        }
    }
}

void AppService::onConstantSettingSpeedUnitStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeedUnit.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedUnitStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeedUnit.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define SPEED_BLINK_COUNT 3
void AppService::onConstantSettingSpeedBlinkingStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedBlinkingStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedBlinkingBlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantSettingSpeedBlinkingBlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.SettingSpeed.Blinking.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == SPEED_BLINK_COUNT) {
            return;
        }
    }
}

void AppService::onConstantFrontVehicleStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.FrontVehicle.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantFrontVehicleStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.FrontVehicle.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define FRONT_VEHICLE_COUNT 3
void AppService::onConstantDistanceFromFrontVehicleStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantDistanceFromFrontVehicleStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantDistanceFromFrontVehicleBlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantDistanceFromFrontVehicleBlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.DistanceFromFrontVehicle.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == FRONT_VEHICLE_COUNT) {
            return;
        }
    }
}

#define NAVI_SYMBOL_COUNT 3
void AppService::onConstantNaviSetSymbolStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.NaviSetSymbol.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantNaviSetSymbolStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.NaviSetSymbol.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantNaviSetSymbolBlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantNaviSetSymbolBlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.NaviSetSymbol.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == NAVI_SYMBOL_COUNT) {
            return;
        }
    }
}

void AppService::onConstantISLADirectionStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.ISLADirection.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ConstantISLADirectionStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.ISLADirection.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define LCA_COUNT 3
void AppService::onConstantLCAStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LCA.Stat) {
            emit signalContentsValueChanged(ContentsType::ConstantLCAStat,
                                            static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LCA.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LCA.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantLCABlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LCA.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LCA.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantLCABlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LCA.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LCA_COUNT) {
            return;
        }
    }
}

#define LFA_COUNT 3
void AppService::onConstantLFAStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LFA.Stat) {
            emit signalContentsValueChanged(ContentsType::ConstantLFAStat,
                                            static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LFA.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LFA.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ConstantLFABlinkA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LFA.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LFA.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ConstantLFABlinkB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LFA.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LFA_COUNT) {
            return;
        }
    }
}

void AppService::onConstnatAdasOnStatusChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Inter_AdasOnStatus) {
            emit signalContentsValueChanged(ContentsType::ConstantAdasOnStatus,
                                            static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Inter_AdasOnStatus.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsISLAMainSpeedStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ISLA_BLINK_COUNT 3
void AppService::onContentsISLAMainSpeedBlinkChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedBlinkStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ISLA_BLINK_COUNT) {
            return;
        }
    }
}

void AppService::onContentsISLAMainSpeedBlinkColorChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Color) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedBlinkColor,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeedBlink.Color.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsISLAMainSpeedValueChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedValueStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Value) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAMainSpeedValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAMainSpeed_Value.Value.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ISLA_BLINK_COUNT) {
            return;
        }
    }
}

void AppService::onContentsISLAFinalAddtnlSignChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HUInt64 islaAddSignValue = 0;
    ccos::HUInt64 disAddSignValue = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.ISLAAddtnlSignValue) {
            islaAddSignValue = static_cast<ccos::HUInt64>(
                SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.ISLAAddtnlSignValue.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.DISAddtnlSignValue) {
            disAddSignValue = static_cast<ccos::HUInt64>(
                SFC.ADAS_Driving_ISLA.Constant.FinalAddtnlSign.DISAddtnlSignValue.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ISLA_BLINK_COUNT) {
            break;
        }
    }

    emit signalContentsISLAFinalAddtnSignChanged(stat, islaAddSignValue, disAddSignValue);
}

void AppService::onContentsISLAOverlapSignStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSign.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAOverlapSignStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSign.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onContentsISLAOverlapSignBlinkChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.Stat) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAOverlapSignBlinkStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueA) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAOverlapSignBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueB) {
            emit signalContentsValueChanged(
                ContentsType::ContentsISLAOverlapSignBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving_ISLA.Constant.ISLAOverlapSignBlink.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ISLA_BLINK_COUNT) {
            return;
        }
    }
}

void AppService::onMainViewTBTNaviAliveStatusChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Inter_NaviAliveStatus) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::NAVI_ALIVE_STATUS,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Inter_NaviAliveStatus.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTConfigNaviMapCountryChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Inter_ConfigNaviMapCountry) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::CONFIG_NAVI_MAP_COUNTRY,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Inter_ConfigNaviMapCountry.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTNaviTBTStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviTbt.Stat) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::NAVI_TBT_STAT,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviTbt.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTDirectionTypeChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.DirectionType.Stat) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::DIRECTION_TYPE_STAT,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.DirectionType.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTSideStreetBitMaskChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.SideStreet.Bitmask) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::SIDE_STREET_BIT_MASK,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.SideStreet.Bitmask.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTCombinedSideStreetChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.CombinedSideStreet.Stat) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::COMBINED_SIDE_STREET_STAT,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.CombinedSideStreet.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewTBTNaviPointFlagTypeChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviPointFlagType.Stat) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::NAVI_POINT_FLAG_TYPE_STAT,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviPointFlagType.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define TBT_DIST_COUNT 3
void AppService::onMainViewTBTNaviDistToDestChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HDouble value = 0;
    ccos::HUInt64 unitStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToDest.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToDest.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToDest.Value) {
            value = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToDest.Value.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistUnit.Stat) {
            unitStat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistUnit.Stat.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_DIST_COUNT) {
            emit signalMainViewNaviValueChanged(MainViewNaviDistType::NAVI_DIST_TO_DEST_TYPE, stat, value, unitStat);
            return;
        }
    }
}

void AppService::onMainViewTBTNaviDistToTurnPointChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HDouble value = 0;
    ccos::HUInt64 unitStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Value) {
            value = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.TbtDistToTurnPoint.Value.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.TbtDistanceUnit.Stat) {
            unitStat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.TbtDistanceUnit.Stat.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_DIST_COUNT) {
            emit signalMainViewNaviValueChanged(MainViewNaviDistType::NAVI_DIST_TO_TURN_POINT_TYPE, stat, value, unitStat);
            return;
        }
    }
}

#define TBT_BAR_GRAPH_COUNT 2
void AppService::onMainViewTBTBarGraphChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Stat) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::TBT_BAR_GRAPH_LEVEL_STAT,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Value) {
            emit signalMainViewTBTValueChanged(
                MainViewTBTType::TBT_BAT_GRAPH_LEVEL_VALUE,
                static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.TbtBarGraphLevel.Value.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_BAR_GRAPH_COUNT) {
            return;
        }
    }
}

void AppService::onMainViewTBTNaviDistToTurn1Changed(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HDouble value = 0;
    ccos::HUInt64 unitStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurn1.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurn1.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurn1.Value) {
            value = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurn1.Value.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurnUnit1.Stat) {
            unitStat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurnUnit1.Stat.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_DIST_COUNT) {
            emit signalMainViewNaviValueChanged(MainViewNaviDistType::NAVI_DIST_TO_TURN1_TYPE, stat, value, unitStat);
            return;
        }
    }
}

void AppService::onMainViewTBTNaviDistToTurn2Changed(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HDouble value = 0;
    ccos::HUInt64 unitStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurn2.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurn2.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurn2.Value) {
            value = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurn2.Value.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviDistToTurnUnit2.Stat) {
            unitStat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviDistToTurnUnit2.Stat.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_DIST_COUNT) {
            emit signalMainViewNaviValueChanged(MainViewNaviDistType::NAVI_DIST_TO_TURN2_TYPE, stat, value, unitStat);
            return;
        }
    }
}

void AppService::onMainViewTBTNaviLaneRemainDistChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    ccos::HUInt64 stat = 0;
    ccos::HDouble value = 0;
    ccos::HUInt64 unitStat = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Stat) {
            stat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Stat.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Value) {
            value = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviLaneRemainDist.Value.value(vehicleSignal));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.Navigation_TBT.Constant.NaviLaneRemainDistUnit.Stat) {
            unitStat = static_cast<ccos::HUInt64>(SFC.Navigation_TBT.Constant.NaviLaneRemainDistUnit.Stat.value(vehicleSignal));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TBT_DIST_COUNT) {
            emit signalMainViewNaviValueChanged(MainViewNaviDistType::NAVI_LANE_REMAIN_DIST_TYPE, stat, value, unitStat);
            return;
        }
    }
}

#define LEFT_SIDE_LANE_COLOR 3
void AppService::onMainViewADASLeftSideLaneColorChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.Color.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftSideLaneColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftSideLane.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftSideLaneColorBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftSideLaneColorBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftSideLane.Color.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LEFT_SIDE_LANE_COLOR) {
            return;
        }
    }
}

void AppService::onMainViewADASLeftSideLaneLateralValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::LeftSideLaneLateralValue,
                static_cast<ccos::HDouble>(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASLeftSideLaneLateralValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftSideLaneLateralValueStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftSideLane.Lateral.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASLeftSideLaneProgressInLaneChangeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftSideLane.ProgressInLaneChange.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftSideLaneProgressInLaneChangeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftSideLane.ProgressInLaneChange.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define RIGHT_SIDE_LANE_COLOR 3
void AppService::onMainViewADASRightSideLaneColorChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.Color.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightSideLaneColorStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.RightSideLane.Color.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightSideLaneColorBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightSideLaneColorBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.RightSideLane.Color.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == RIGHT_SIDE_LANE_COLOR) {
            return;
        }
    }
}

void AppService::onMainViewADASRightSideLaneLateralValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.Lateral.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::RightSideLaneLateralValue,
                static_cast<ccos::HDouble>(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRightSideLaneLateralValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.Lateral.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightSideLaneLateralValueStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.RightSideLane.Lateral.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRightSideLaneProgressInLaneChangeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightSideLane.ProgressInLaneChange.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightSideLaneProgressInLaneChangeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightSideLane.ProgressInLaneChange.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASLeftRightSideLaneCurvationValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.Value) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRightSideLaneCurvationValue,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASLeftRightSideLaneCurvationValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRightSideLaneCurvationValueStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftRightSideLane.Curvation.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleFrontVehicleStatusStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleFront.VehicleStatus.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleFrontVehicleStatusStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.VehicleFront.VehicleStatus.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleFrontLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleFrontLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleFrontLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleFrontLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleFront.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleFrontLateralDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleFrontLateralDistanceValue,
                static_cast<ccos::HDouble>(SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleFrontLateralDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleFrontLateralDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleFront.LateralDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundVehicleStatusStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftForegroundVehicleStatusStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.FadeStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftForegroundVehicleStatusFadeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.VehicleStatus.FadeStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleLeftForegroundLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftForegroundLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundLateralDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleLeftForegroundLateralDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftForegroundLateralDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftForegroundLateralDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftForeground.LateralDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundVehicleStatusStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleRightForegroundVehicleStatusStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.FadeStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleRightForegroundVehicleStatusFadeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.VehicleStatus.FadeStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleRightForegroundLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleRightForegroundLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundLateralDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleRightForegroundLateralDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleRightForegroundLateralDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleRightForegroundLateralDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleRightForeground.LateralDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardVehicleStatusStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftRearwardVehicleStatusStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.FadeStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftRearwardVehicleStatusFadeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.VehicleStatus.FadeStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleLeftRearwardLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftRearwardLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardLateralDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::VehicleLeftRearwardLateralDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASVehicleLeftRearwardLateralDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::VehicleLeftRearwardLateralDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.VehicleLeftRearward.LateralDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleVehicleStatusStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RearRightHandSideVehicleVehicleStatusStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.FadeStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RearRightHandSideVehicleVehicleStatusFadeStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.VehicleStatus.FadeStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::RearRightHandSideVehicleLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueStatChanged(
    const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RearRightHandSideVehicleLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleLateralDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::RearRightHandSideVehicleLateralDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASRearRightHandSideVehicleLateralDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RearRightHandSideVehicleLateralDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RearRightHandSideVehicle.LateralDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASTheRearLeftHandSideVehicleStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.TheRearLeftHandSideVehicle.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::TheRearLeftHandSideVehicleStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.TheRearLeftHandSideVehicle.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASTheRearRightHandSideVehicleStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.TheRearRightHandSideVehicle.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::TheRearRightHandSideVehicleStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.TheRearRightHandSideVehicle.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define LEFT_REAR_ALARM_COLOR 3
void AppService::onMainViewADASLeftRearSideCrashAlarmWaveChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmWaveStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmWaveBlinkValueA,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmWaveBlinkValueB,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmWave.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LEFT_REAR_ALARM_COLOR) {
            return;
        }
    }
}

#define LEFT_REAR_ALARM_MASTER_COLOR 3
void AppService::onMainViewADASLeftRearSideCrashAlarmMasterSymbolChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmMasterSymbolStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmMasterSymbolBlinkValueA,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::LeftRearSideCrashAlarmMasterSymbolBlinkValueB,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.LeftRearSideCrashAlarmMasterSymbol.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LEFT_REAR_ALARM_MASTER_COLOR) {
            return;
        }
    }
}

#define RIGHT_REAR_ALARM_COLOR 3
void AppService::onMainViewADASRightRearSideCrashAlarmWaveChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmWaveStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmWaveBlinkValueA,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmWaveBlinkValueB,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmWave.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == RIGHT_REAR_ALARM_COLOR) {
            return;
        }
    }
}

#define RIGHT_REAR_ALARM_MASTER_COLOR 3
void AppService::onMainViewADASRightRearSideCrashAlarmMasterSymbolChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmMasterSymbolStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueA) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmMasterSymbolBlinkValueA,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueB) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::RightRearSideCrashAlarmMasterSymbolBlinkValueB,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.RightRearSideCrashAlarmMasterSymbol.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == RIGHT_REAR_ALARM_MASTER_COLOR) {
            return;
        }
    }
}

void AppService::onMainViewADASTargetIntervalDistanceStaChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.TargetIntervalDistance.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::TargetIntervalDistanceStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.TargetIntervalDistance.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.Value) {
            emit signalMainViewADASValueChanged(
                MainViewADASType::TargetIntervalDistanceLongitudinalDistanceValue,
                static_cast<ccos::HDouble>(
                    SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.ValueStat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::TargetIntervalDistanceLongitudinalDistanceValueStat,
                static_cast<ccos::HUInt64>(
                    SFC.ADAS_Driving.Constant.TargetIntervalDistance.LongitudinalDistance.ValueStat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onMainViewADASCenterLineStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ADAS_Driving.Constant.CenterLine.Stat) {
            emit signalMainViewADASStatChanged(
                MainViewADASType::CenterLineStat,
                static_cast<ccos::HUInt64>(SFC.ADAS_Driving.Constant.CenterLine.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeDisplaySpeedValueKPHChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH) {
            emit signalGaugeValueChanged(GaugeType::SpeedGaugeDisplaySpeedValueKPH,
                                         SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH.value(vehicleSignal));
            return;
        }
    }
}

void AppService::onSpeedGaugeDisplaySpeedValueMPHChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH) {
            emit signalGaugeValueChanged(GaugeType::SpeedGaugeDisplaySpeedValueMPH,
                                         SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH.value(vehicleSignal));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedAnalogStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAnalog.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedAnalogStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAnalog.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedAnalogValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAnalog.Value) {
            emit signalGaugeValueChanged(GaugeType::SpeedGaugeSpeedAnalogDisplayValue,
                                         SFC.Speed_Gauge.Constant.SpeedAnalog.Value.value(vehicleSignal));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedDigitalStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedDigital.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedDigitalDisplayStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedDigitalValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedDigital.Value) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedSubDigitalValue,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedDigital.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedSubDigitalStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedSubDigitalStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedSubDigitalValueChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDigital.Value) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedSubDigitalValue,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDigital.Value.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedMainDisplayUnitStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedSubDisplayStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedAuxDisplayUnitStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedAuxDisplayUnitStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeSpeedSubDisplayStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeSpeedSubDisplayStat,
                static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeDisplaySpeedValueKPHRangeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Private.Speed_Gauge.DisplaySpeedValueKPHRange.Stat) {
            emit signalGaugeStatChanged(
                GaugeType::SpeedGaugeDisplaySpeedValueKPHRangeStat,
                static_cast<ccos::HUInt64>(SFC.Private.Speed_Gauge.DisplaySpeedValueKPHRange.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onSpeedGaugeDisplaySpeedUnitChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Speed_Gauge.Inter_DisplaySpeedUnit) {
            emit signalGaugeStatChanged(GaugeType::SpeedGaugeDisplaySpeedUnit,
                                        static_cast<ccos::HUInt64>(SFC.Speed_Gauge.Inter_DisplaySpeedUnit.value(vehicleSignal)));
            return;
        }
    }
}

void AppService::onFuelSystemFuelGaugeChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Fuel_System.Constant.FuelGauge.Value) {
            emit signalFuelGaugeValueChanged(
                static_cast<ccos::HDouble>(SFC.Fuel_System.Constant.FuelGauge.Value.value(vehicleSignal)));
            return;
        }
    }
}
