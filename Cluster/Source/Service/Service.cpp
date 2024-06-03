#include "Service.h"

#include <regex>
#include <QDebug>

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

void Service::init() {
    qDebug() << "Service - Init";

    getVehicleSignalModel();

    subscribeConstantSignals();
    subscribeTelltaleSignals();
    subscribeEventSignals();
    subscribeSoundSignals();
    subscribeEtcSignals();
}

ccos::vehicle::vsm::HVehicleSignalModel& Service::getVehicleSignalModel() {
    static ccos::vehicle::vsm::HVehicleSignalModel* gVehicleSignalModel = nullptr;
    if (gVehicleSignalModel == nullptr) {
        gVehicleSignalModel = new ccos::vehicle::vsm::HVehicleSignalModel;
    }
    return *gVehicleSignalModel;
}

void Service::addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc) {
    auto subscription = std::make_shared<ccos::vehicle::vsm::HSubscription>(std::vector<std::string>{nodeAddress},
                                                                            ccos::vehicle::vsm::HSubscriptionType::VALUE_CHANGED,
                                                                            std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    if (result != ccos::HResult::OK) {
        qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
    }
}

void Service::addSubscriptionGroup(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc) {
    auto subscription = std::make_shared<ccos::vehicle::vsm::HSubscription>(
        nodePaths, ccos::vehicle::vsm::HSubscriptionType::VALUE_CHANGED, std::make_shared<VehicleListener>(handlingFunc));
    auto result = getVehicleSignalModel().subscribe(subscription);

    // qDebug() << "\t addSubscriptionGroup :" << ((result == ccos::HResult::OK) ? ("Success") : ("Fail"));
    if (result != ccos::HResult::OK) {
        for (const std::string& nodeAddress : nodePaths) {
            qDebug() << "Fail to subscribe :" << static_cast<int>(result) << nodeAddress.c_str();
        }
    }
}

void Service::subscribeConstantSignals() {
    subscribeSignals<Constant>(
        DataType::Constant,
        {
            {Constant::SpeedAnalogStat, {SFC.Speed_Gauge.Constant.SpeedAnalog.Stat}},
            {Constant::SpeedAnalogValue, {SFC.Speed_Gauge.Constant.SpeedAnalog.Value}},
            {Constant::SpeedDigitalStat, {SFC.Speed_Gauge.Constant.SpeedDigital.Stat}},
            {Constant::SpeedDigitalValue, {SFC.Speed_Gauge.Constant.SpeedDigital.Value}},
            {Constant::SpeedSubDigitalStat, {SFC.Speed_Gauge.Constant.SpeedSubDigital.Stat}},
            {Constant::SpeedSubDigitalValue, {SFC.Speed_Gauge.Constant.SpeedSubDigital.Value}},
            {Constant::SpeedMainDisplayUnitStat, {SFC.Speed_Gauge.Constant.SpeedMainDisplayUnit.Stat}},
            {Constant::SpeedAuxDisplayUnitStat, {SFC.Speed_Gauge.Constant.SpeedAuxDisplayUnit.Stat}},
            {Constant::SpeedSubDisplayStat, {SFC.Speed_Gauge.Constant.SpeedSubDisplay.Stat}},
            {Constant::SpeedScaleMaximumStat, {SFC.Speed_Gauge.Constant.SpeedScaleMaximum.Stat}},
            {Constant::NaviSpeedLimitStat, {SFC.Speed_Gauge.Constant.NaviSpeedLimit.Stat}},
            {Constant::NaviSpeedLimitOver1ColorValue, {SFC.Speed_Gauge.Constant.NaviSpeedLimitOver1Color.Value}},
            {Constant::NaviSpeedLimitOver2ColorValue, {SFC.Speed_Gauge.Constant.NaviSpeedLimitOver2Color.Value}},
            {Constant::RpmValue, {SFC.Tachometer.Constant.Rpm.Value}},
            {Constant::RedZoneExceptNbrandStat, {SFC.Tachometer.Constant.RedZoneExceptNbrand.Stat}},
            {Constant::RedZoneNbrandStat, {SFC.Tachometer.Constant.RedZoneNbrand.Stat}},
            {Constant::MaxRpmStat, {SFC.Tachometer.Constant.MaxRpm.Stat}},
            {Constant::RpmDampStat, {SFC.Tachometer.Constant.RpmDamp.Stat}},
            {Constant::ResvChargeStat, {SFC.Intro_Outro.Constant.ResvCharge.Stat}},
            {Constant::ResvClimateStat, {SFC.Intro_Outro.Constant.ResvClimate.Stat}},
            {Constant::PurificationAirStat, {SFC.Intro_Outro.Constant.PurificationAir.Stat}},
            {Constant::PurificationAirValue, {SFC.Intro_Outro.Constant.PurificationAir.Value}},
            {Constant::CO2ReductionStat, {SFC.Intro_Outro.Constant.CO2Reduction.Stat}},
            {Constant::CO2ReductionValue, {SFC.Intro_Outro.Constant.CO2Reduction.Value}},
            {Constant::OutTempDisplayStat, {SFC.OAT.Constant.OutTempDisplay.Stat}},
            {Constant::OutTempDisplayUnitStat, {SFC.OAT.Constant.OutTempDisplayUnit.Stat}},
            {Constant::ViewFrontVehicle,
             {SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Stat,
              SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value,
              SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat,
              SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value,
              SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Stat,
              SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value,
              SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat,
              SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value}},
        });
}

void Service::subscribeTelltaleSignals() {
    subscribeSignals<Telltale>(
        DataType::Telltale,
        {
            {Telltale::FrontFogStat, {SFC.Lamp_Indicator.Telltale.FrontFog.Stat}},
            {Telltale::HighBeamStat, {SFC.Lamp_Indicator.Telltale.HighBeam.Stat}},
            {Telltale::RearFogStat, {SFC.Lamp_Indicator.Telltale.RearFog.Stat}},
            {Telltale::TailLampStat, {SFC.Lamp_Indicator.Telltale.TailLamp.Stat}},
            {Telltale::TurnSignalLeftStat, {SFC.Lamp_Indicator.Telltale.TurnSignalLeft.Stat}},
            {Telltale::TurnSignalRightStat, {SFC.Lamp_Indicator.Telltale.TurnSignalRight.Stat}},
            {Telltale::LowBeamStat, {SFC.Lamp_Indicator.Telltale.LowBeam.Stat}},
            {Telltale::IceWarnStat, {SFC.OAT.Telltale.IceWarn.Stat, SFC.OAT.Telltale.IceWarn.StatOptional}},
            {Telltale::DMSStat, {SFC.Drive_Mode.Telltale.Drive_Mode.DMS.Stat}},
            {Telltale::DMSStatOptional, {SFC.Drive_Mode.Telltale.Drive_Mode.DMS.StatOptional}},
            {Telltale::DimmedStat, {SFC.Drive_Mode.Telltale.Drive_Mode.Dimmed.Stat}},
            {Telltale::DimmedStatOptional, {SFC.Drive_Mode.Telltale.Drive_Mode.Dimmed.StatOptional}},
            {Telltale::CreepOffStat, {SFC.High_Performance_For_N_Brand.Telltale.CreepOff.Stat}},
            {Telltale::ShiftLightStat, {SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.Stat}},
            {Telltale::ShiftLightBlinkValueA, {SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueA}},
            {Telltale::ShiftLightBlinkValueB, {SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueB}},
            {Telltale::ShiftLightBlinkValueBOptional, {SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.BlinkValueBOptional}},
            {Telltale::ShiftLightStatOptional, {SFC.High_Performance_For_N_Brand.Telltale.ShiftLight.StatOptional}},
            {Telltale::EnduranceRaceStat, {SFC.High_Performance_For_N_Brand.Telltale.EnduranceRace.Stat}},
            {Telltale::EnduranceRaceStatOptional, {SFC.High_Performance_For_N_Brand.Telltale.EnduranceRace.StatOptional}},
            {Telltale::FCAStat, {SFC.ADAS_Driving_New.Telltale.FCA.Stat}},
            {Telltale::ELKStat, {SFC.ADAS_Driving_New.Telltale.ELK.Stat}},
            {Telltale::LKAStat, {SFC.ADAS_Driving_New.Telltale.LKA.Stat}},
            {Telltale::HBAStat, {SFC.ADAS_Driving_New.Telltale.HBA.Stat}},
            {Telltale::LFAStat, {SFC.ADAS_Driving_New.Telltale.LFA.Stat}},
            {Telltale::LCALeftStat, {SFC.ADAS_Driving_New.Telltale.LCALeft.Stat}},
            {Telltale::LCARightStat, {SFC.ADAS_Driving_New.Telltale.LCARight.Stat}},
            {Telltale::HandsOnOffStat,
             {SFC.ADAS_Driving_New.Telltale.HandsOnOff.Stat, SFC.ADAS_Driving_New.Telltale.HandsOnOff.StatOptional}},
            {Telltale::DAWStat, {SFC.ADAS_Driving_New.Telltale.DAW.Stat}},
            {Telltale::Gauge,
             {SFC.High_Performance_Gauge.Telltale.LaunchControl.Stat,
              SFC.High_Performance_Gauge.Telltale.LaunchControl.StatOptional,
              SFC.High_Performance_Gauge.Telltale.LaunchControl.BlinkValueAOptional,
              SFC.High_Performance_Gauge.Telltale.LaunchControl.BlinkValueA,
              SFC.High_Performance_Gauge.Telltale.LaunchControl.BlinkValueB}},
        });
}

void Service::subscribeEventSignals() {
    subscribeSignals<Event>(
        DataType::Event,
        {
            {Event::WelcomeID, {SFC.Intro_Outro.Event.Welcome.ID}},
            {Event::WelcomeStat, {SFC.Intro_Outro.Event.Welcome.Stat}},
            {Event::WelcomeLinkedSoundID, {SFC.Intro_Outro.Event.Welcome.LinkedSound.ID}},
            {Event::WelcomeLinkedSoundType, {SFC.Intro_Outro.Event.Welcome.LinkedSound.Type}},
            {Event::WelcomeLinkedSoundRepeatCount, {SFC.Intro_Outro.Event.Welcome.LinkedSound.RepeatCount}},
            {Event::SystemCheckID, {SFC.Intro_Outro.Event.SystemCheck.ID}},
            {Event::SystemCheckStat, {SFC.Intro_Outro.Event.SystemCheck.Stat}},
            {Event::GoodbyeID, {SFC.Intro_Outro.Event.Goodbye.ID}},
            {Event::GoodbyeStat, {SFC.Intro_Outro.Event.Goodbye.Stat}},
            {Event::GoodbyeLinkedSoundID, {SFC.Intro_Outro.Event.Goodbye.LinkedSound.ID}},
            {Event::GoodbyeLinkedSoundType, {SFC.Intro_Outro.Event.Goodbye.LinkedSound.Type}},
            {Event::GoodbyeLinkedSoundRepeatCount, {SFC.Intro_Outro.Event.Goodbye.LinkedSound.RepeatCount}},
            {Event::IceWarnID, {SFC.OAT.Event.IceWarn.ID}},
            {Event::IceWarnStat_, {SFC.OAT.Event.IceWarn.Stat}},
            {Event::IceWarnLinkedSoundID, {SFC.OAT.Event.IceWarn.LinkedSound.ID}},
            {Event::IceWarnLinkedSoundType, {SFC.OAT.Event.IceWarn.LinkedSound.Type}},
            {Event::IceWarnLinkedSoundRepeatCount, {SFC.OAT.Event.IceWarn.LinkedSound.RepeatCount}},
            {Event::TransmissionIndicatorID, {SFC.Transmission_Indicator.Event.Transmission_Indicator.ID}},
            {Event::TransmissionIndicatorStat, {SFC.Transmission_Indicator.Event.Transmission_Indicator.Stat}},
            {Event::TransmissionPaddleShiftID, {SFC.Transmission_Indicator.Event.Transmission_PaddleShift.ID}},
            {Event::TransmissionPaddleShiftStat, {SFC.Transmission_Indicator.Event.Transmission_PaddleShift.Stat}},
            {Event::TransmissionSBWID, {SFC.Transmission_Indicator.Event.Transmission_SBW.ID}},
            {Event::TransmissionSBWStat, {SFC.Transmission_Indicator.Event.Transmission_SBW.Stat}},
            {Event::DrivingModeChangeID, {SFC.Drive_Mode.Event.DrivingModeChange.ID}},
            {Event::DrivingModeChangeStat, {SFC.Drive_Mode.Event.DrivingModeChange.Stat}},
            {Event::TerrainModeExitNotificationID, {SFC.Drive_Mode.Event.TerrainModeExitNotification.ID}},
            {Event::TerrainModeExitNotificationStat, {SFC.Drive_Mode.Event.TerrainModeExitNotification.Stat}},
            {Event::TerrainModeExitNotificationLinkedSoundID, {SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.ID}},
            {Event::TerrainModeExitNotificationLinkedSoundType,
             {SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.Type}},
            {Event::TerrainModeExitNotificationLinkedSoundRepeatCount,
             {SFC.Drive_Mode.Event.TerrainModeExitNotification.LinkedSound.RepeatCount}},
            {Event::ReconfirmNCustomModeID, {SFC.Drive_Mode.Event.ReconfirmNCustomMode.ID}},
            {Event::ReconfirmNCustomModeStat, {SFC.Drive_Mode.Event.ReconfirmNCustomMode.Stat}},
            {Event::ReconfirmNCustomModeLinkedSoundID, {SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.ID}},
            {Event::ReconfirmNCustomModeLinkedSoundType, {SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.Type}},
            {Event::ReconfirmNCustomModeLinkedSoundRepeatCount,
             {SFC.Drive_Mode.Event.ReconfirmNCustomMode.LinkedSound.RepeatCount}},
            {Event::ModeSwitchingImpossibleID, {SFC.Drive_Mode.Event.ModeSwitchingImpossible.ID}},
            {Event::ModeSwitchingImpossibleStat, {SFC.Drive_Mode.Event.ModeSwitchingImpossible.Stat}},
            {Event::ModeSwitchingImpossibleLinkedSoundID, {SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.ID}},
            {Event::ModeSwitchingImpossibleLinkedSoundType, {SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.Type}},
            {Event::ModeSwitchingImpossibleLinkedSoundRepeatCount,
             {SFC.Drive_Mode.Event.ModeSwitchingImpossible.LinkedSound.RepeatCount}},
            {Event::ReconfirmGTMYModeID, {SFC.Drive_Mode.Event.ReconfirmGT_MYMode.ID}},
            {Event::ReconfirmGTMYModeStat, {SFC.Drive_Mode.Event.ReconfirmGT_MYMode.Stat}},
            {Event::ReconfirmGTMYModeLinkedSoundID, {SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.ID}},
            {Event::ReconfirmGTMYModeLinkedSoundType, {SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.Type}},
            {Event::ReconfirmGTMYModeLinkedSoundRepeatCount, {SFC.Drive_Mode.Event.ReconfirmGT_MYMode.LinkedSound.RepeatCount}},
            {Event::Event4GroupID, {SFC.ADAS_PARKING_NEW.Event.Event4Group.ID}},
            {Event::Event4GroupStat, {SFC.ADAS_PARKING_NEW.Event.Event4Group.Stat}},
            {Event::Event7GroupFailure10ID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.ID}},
            {Event::Event7GroupFailure10Stat, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.Stat}},
            {Event::Event7GroupFailure10LinkedSoundID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.ID}},
            {Event::Event7GroupFailure10LinkedSoundType, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.Type}},
            {Event::Event7GroupFailure10LinkedSoundRepeatCount,
             {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.LinkedSound.RepeatCount}},
            {Event::Event7GroupFailure11ID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.ID}},
            {Event::Event7GroupFailure11Stat, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.Stat}},
            {Event::Event7GroupFailure11LinkedSoundID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.ID}},
            {Event::Event7GroupFailure11LinkedSoundType, {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.Type}},
            {Event::Event7GroupFailure11LinkedSoundRepeatCount,
             {SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.LinkedSound.RepeatCount}},
            {Event::Event7GroupCCWFailureID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.ID}},
            {Event::Event7GroupCCWFailureStat, {SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.Stat}},
            {Event::Event7GroupCCWFailureLinkedSoundID, {SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.ID}},
            {Event::Event7GroupCCWFailureLinkedSoundType, {SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.Type}},
            {Event::Event7GroupCCWFailureLinkedSoundRepeatCount,
             {SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.LinkedSound.RepeatCount}},
            {Event::Event2GroupID, {SFC.ADAS_PARKING_NEW.Event.Event2Group.ID}},
            {Event::Event2GroupStat, {SFC.ADAS_PARKING_NEW.Event.Event2Group.Stat}},
            {Event::BrakeOverrideID, {SFC.High_Performance_For_N_Brand.Event.BrakeOverride.ID}},
            {Event::BrakeOverrideStat, {SFC.High_Performance_For_N_Brand.Event.BrakeOverride.Stat}},
            {Event::BrakeOverrideLinkedSoundID, {SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.ID}},
            {Event::BrakeOverrideLinkedSoundType, {SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.Type}},
            {Event::BrakeOverrideLinkedSoundRepeatCount,
             {SFC.High_Performance_For_N_Brand.Event.BrakeOverride.LinkedSound.RepeatCount}},
            {Event::Ngs1ID, {SFC.High_Performance_For_N_Brand.Event.Ngs1.ID}},
            {Event::Ngs1Stat, {SFC.High_Performance_For_N_Brand.Event.Ngs1.Stat}},
            {Event::Ngs2ID, {SFC.High_Performance_For_N_Brand.Event.Ngs2.ID}},
            {Event::Ngs2Stat, {SFC.High_Performance_For_N_Brand.Event.Ngs2.Stat}},
            {Event::Ngs2LinkedSoundID, {SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.ID}},
            {Event::Ngs2LinkedSoundType, {SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.Type}},
            {Event::Ngs2LinkedSoundRepeatCount, {SFC.High_Performance_For_N_Brand.Event.Ngs2.LinkedSound.RepeatCount}},
            {Event::NRoadSenseID, {SFC.High_Performance_For_N_Brand.Event.NRoadSense.ID}},
            {Event::NRoadSenseStat, {SFC.High_Performance_For_N_Brand.Event.NRoadSense.Stat}},
            {Event::NRoadSenseLinkedSoundID, {SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.ID}},
            {Event::NRoadSenseLinkedSoundType, {SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.Type}},
            {Event::NRoadSenseLinkedSoundRepeatCount,
             {SFC.High_Performance_For_N_Brand.Event.NRoadSense.LinkedSound.RepeatCount}},
            {Event::BrakepadCheckID, {SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.ID}},
            {Event::BrakepadCheckStat, {SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.Stat}},
            {Event::BrakepadCheckLinkedSoundID, {SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.ID}},
            {Event::BrakepadCheckLinkedSoundType, {SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.Type}},
            {Event::BrakepadCheckLinkedSoundRepeatCount,
             {SFC.High_Performance_For_N_Brand.Event.BrakepadCheck.LinkedSound.RepeatCount}},
            {Event::ElectricKickDriftID, {SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.ID}},
            {Event::ElectricKickDriftStat, {SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.Stat}},
            {Event::ElectricKickDriftLinkedSoundID, {SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.ID}},
            {Event::ElectricKickDriftLinkedSoundType,
             {SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.Type}},
            {Event::ElectricKickDriftLinkedSoundRepeatCount,
             {SFC.High_Performance_For_N_Brand.Event.ElectricKickDrift.LinkedSound.RepeatCount}},
            {Event::DrivingAssistSummaryID, {SFC.ADAS_Driving_New.Event.DrivingAssistSummary.ID}},
            {Event::DrivingAssistSummaryStat, {SFC.ADAS_Driving_New.Event.DrivingAssistSummary.Stat}},
            {Event::Group1FullPopup1,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup1_1.ID, SFC.ADAS_Driving_New.Event.Group1FullPopup1_1.Stat}},
            {Event::Group1FullPopup1_2ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.ID}},
            {Event::Group1FullPopup1_2Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.Stat}},
            {Event::Group1FullPopup1_2LinkedSoundID, {SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.ID}},
            {Event::Group1FullPopup1_2LinkedSoundType, {SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.Type}},
            {Event::Group1FullPopup1_2LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup1_2.LinkedSound.RepeatCount}},
            {Event::Group1FullPopup2_1ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.ID}},
            {Event::Group1FullPopup2_1Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.Stat}},
            {Event::Group1FullPopup2_1LinkedSoundID, {SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.ID}},
            {Event::Group1FullPopup2_1LinkedSoundType, {SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.Type}},
            {Event::Group1FullPopup2_1LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.LinkedSound.RepeatCount}},
            {Event::Group1FullPopup3_1_E52099ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.ID}},
            {Event::Group1FullPopup3_1_E52099Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.Stat}},
            {Event::Group1FullPopup3_1_E52099LinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.LinkedSound.ID}},
            {Event::Group1FullPopup3_1_E52099LinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52099.LinkedSound.Type}},
            {Event::Group1FullPopup3_1_E52100ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52100.ID}},
            {Event::Group1FullPopup3_1_E52100Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52100.Stat}},
            {Event::Group1FullPopup3_1_E52101ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.ID}},
            {Event::Group1FullPopup3_1_E52101Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.Stat}},
            {Event::Group1FullPopup3_1_E52101LinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.ID}},
            {Event::Group1FullPopup3_1_E52101LinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.Type}},
            {Event::Group1FullPopup3_1_E52101LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52101.LinkedSound.RepeatCount}},
            {Event::Group1FullPopup3_1_E52105ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.ID}},
            {Event::Group1FullPopup3_1_E52105Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.Stat}},
            {Event::Group1FullPopup3_1_E52105LinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.LinkedSound.ID}},
            {Event::Group1FullPopup3_1_E52105LinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52105.LinkedSound.Type}},
            {Event::Group1FullPopup3_1_E52106ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52106.ID}},
            {Event::Group1FullPopup3_1_E52106Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52106.Stat}},
            {Event::Group1FullPopup3_1_E52107ID, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.ID}},
            {Event::Group1FullPopup3_1_E52107Stat, {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.Stat}},
            {Event::Group1FullPopup3_1_E52107LinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.ID}},
            {Event::Group1FullPopup3_1_E52107LinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.Type}},
            {Event::Group1FullPopup3_1_E52107LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group1FullPopup3_1_E52107.LinkedSound.RepeatCount}},
            {Event::Group4FullPopup1_1ID, {SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.ID}},
            {Event::Group4FullPopup1_1Stat, {SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.Stat}},
            {Event::Group4FullPopup1_1LinkedSoundID, {SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.ID}},
            {Event::Group4FullPopup1_1LinkedSoundType, {SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.Type}},
            {Event::Group4FullPopup1_1LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.LinkedSound.RepeatCount}},
            {Event::Group7FullPopupBlindSpotSafetyFailureID,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.ID}},
            {Event::Group7FullPopupBlindSpotSafetyFailureStat,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.Stat}},
            {Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.ID}},
            {Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.Type}},
            {Event::Group7FullPopupBlindSpotSafetyFailureLinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.LinkedSound.RepeatCount}},
            {Event::Group7FullPopupOutsideMirrorSymbolFailureID,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.ID}},
            {Event::Group7FullPopupOutsideMirrorSymbolFailureStat,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.Stat}},
            {Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.ID}},
            {Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.Type}},
            {Event::Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupOutsideMirrorSymbolFailure.LinkedSound.RepeatCount}},
            {Event::Group7FullPopupHdpFailureID, {SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.ID}},
            {Event::Group7FullPopupHdpFailureStat, {SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.Stat}},
            {Event::Group7FullPopupHdpFailureLinkedSoundID,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.ID}},
            {Event::Group7FullPopupHdpFailureLinkedSoundType,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.Type}},
            {Event::Group7FullPopupHdpFailureLinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.LinkedSound.RepeatCount}},
            {Event::Group2MiniPopup1_1ID, {SFC.ADAS_Driving_New.Event.Group2MiniPopup1_1.ID}},
            {Event::Group2MiniPopup1_1Stat, {SFC.ADAS_Driving_New.Event.Group2MiniPopup1_1.Stat}},
            {Event::PuMGroup2AdasWarning1_2StatusID, {SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_2Status.ID}},
            {Event::PuMGroup2AdasWarning1_2StatusStat, {SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_2Status.Stat}},
            {Event::DriverAssistFailure1ID, {SFC.ADAS_Driving_New.Event.DriverAssistFailure1.ID}},
            {Event::DriverAssistFailure1Stat, {SFC.ADAS_Driving_New.Event.DriverAssistFailure1.Stat}},
            {Event::DriverAssistFailure1LinkedSoundID, {SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.ID}},
            {Event::DriverAssistFailure1LinkedSoundType, {SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.Type}},
            {Event::DriverAssistFailure1LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.DriverAssistFailure1.LinkedSound.RepeatCount}},
            {Event::DriverAssistFailure2ID, {SFC.ADAS_Driving_New.Event.DriverAssistFailure2.ID}},
            {Event::DriverAssistFailure2Stat, {SFC.ADAS_Driving_New.Event.DriverAssistFailure2.Stat}},
            {Event::DriverAssistFailure2LinkedSoundID, {SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.ID}},
            {Event::DriverAssistFailure2LinkedSoundType, {SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.Type}},
            {Event::DriverAssistFailure2LinkedSoundRepeatCount,
             {SFC.ADAS_Driving_New.Event.DriverAssistFailure2.LinkedSound.RepeatCount}},
            {Event::HDPOperProhibitID, {SFC.ADAS_Driving_New.Event.HDPOperProhibit.ID}},
            {Event::HDPOperProhibitStat, {SFC.ADAS_Driving_New.Event.HDPOperProhibit.Stat}},
        });
}

void Service::subscribeSoundSignals() {
    subscribeSignals<Sound>(
        DataType::Sound,
        {
            // {Sound::AAAAAAAAAAAA, {SFC.AAAAAAAAAAAA}},
        });
}

void Service::subscribeEtcSignals() {
    subscribeSignals<Etc>(
        DataType::Etc,
        {
            {Etc::InterDisplaySpeedUnit, {SFC.Speed_Gauge.Inter_DisplaySpeedUnit}},
            {Etc::InterDisplaySpeedValueKPH, {SFC.Speed_Gauge.Inter_DisplaySpeedValueKPH}},
            {Etc::InterDisplaySpeedValueMPH, {SFC.Speed_Gauge.Inter_DisplaySpeedValueMPH}},
            {Etc::InterDriveModeSelectStatus, {SFC.Drive_Mode.Inter_DriveModeSelectStatus}},
            {Etc::InterTerrainModeSelectStatus, {SFC.Drive_Mode.Inter_TerrainModeSelectStatus}},
            {Etc::InterNModeSelectStatus, {SFC.Drive_Mode.Inter_NModeSelectStatus}},
            {Etc::InterGtModeSelectStatus, {SFC.Drive_Mode.Inter_GtModeSelectStatus}},
            {Etc::InterAdasViewFixStatus, {SFC.ADAS_Driving_New.Inter_AdasViewFixStatus}},
            {Etc::InterHDPMasterWarningStatus, {SFC.ADAS_Driving_New.Inter_HDPMasterWarningStatus}},
            {Etc::TimerOneShotTimerPriorityISLAStat, {SFC.ADAS_Driving_New.Timer.OneShotTimerPriorityISLA.Stat}},
            {Etc::TimerOneShotTimerPriorityDISStat, {SFC.ADAS_Driving_New.Timer.OneShotTimerPriorityDIS.Stat}},
            {Etc::InterAdasOnStatus, {SFC.ADAS_Driving_New.Inter_AdasOnStatus}},
        });
}

template <typename TYPE>
void Service::subscribeSignals(const DataType& dataType,
                               const std::vector<std::pair<TYPE, std::vector<std::string>>>& signalsToSubscribe) {
    for (const auto& signalPair : signalsToSubscribe) {
        const TYPE& signalType = signalPair.first;
        const std::vector<std::string>& nodePaths = signalPair.second;
        SignalHandlingFunc handlingFunc = [this, dataType,
                                           signalType](const std::vector<ccos::vehicle::vsm::HVehicleSignal>& vehicleSignals) {
            onSignalChanged(dataType, signalType, vehicleSignals);
        };
        addSubscriptionGroup(nodePaths, handlingFunc);
    }
}

QVariant Service::isSignalValue(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                QHash<QString, QVariant>& values) {
    QVariant isValue = QVariant("value not found");
    QString nodePath = vehicleSignal.getNodePath().c_str();
    if (nodePath.isEmpty() == false) {
        switch (vehicleSignal.getValueType()) {
            case ccos::vehicle::vsm::HVehicleSignalValueType::INT64:
                isValue = vehicleSignal.getValue<ccos::HInt64>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::UINT64:
                isValue = vehicleSignal.getValue<ccos::HUInt64>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::DOUBLE:
                isValue = vehicleSignal.getValue<ccos::HDouble>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::BOOL:
                isValue = vehicleSignal.getValue<ccos::HBool>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::TRISTATE:
                isValue = vehicleSignal.getValue<ccos::HUInt64>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::SWITCHSTATE:
                isValue = vehicleSignal.getValue<ccos::HUInt64>();
                break;
            case ccos::vehicle::vsm::HVehicleSignalValueType::STRING:
                isValue = vehicleSignal.getValue<std::string>().c_str();
                break;
            default:
                qDebug() << "Fail to vehicle value type :" << isValue;
                break;
        }
        values[nodePath] = isValue;
    }
    return isValue;
}

template <typename TYPE>
void Service::onSignalChanged(const DataType& dataType, const TYPE& signalType,
                              const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    QHash<QString, QVariant> values;
    QVariant isValue;
    for (const auto& vehicleSignal : signalList) {
        isValue = isSignalValue(vehicleSignal, values);
    }

    if (values.size() >= 1) {
#if 0
        QString multiValueInfo = QString();
        for (auto iter = signalValues.cbegin(); iter != signalValues.cend(); ++iter) {
            QString sfcName = iter.key();
            QVariant sfcValue = iter.value();
            multiValueInfo.append(QString("%1 : %2\n").arg(sfcName).arg(sfcValue.toString()));
        }
        emit signalServiceDataChanged(static_cast<int>(dataType), static_cast<int>(signalType), multiValueInfo);
#else
        emit signalServiceDatasChanged(static_cast<int>(dataType), static_cast<int>(signalType), values);
#endif
    } else {
        emit signalServiceDataChanged(static_cast<int>(dataType), static_cast<int>(signalType), isValue);
    }
}
