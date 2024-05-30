#ifndef SERIVCE_ENUM_H
#define SERIVCE_ENUM_H

enum class DataType : u_int32_t {
    Constant = 0,
    Telltale,
    Event,
    Sound,
    Etc,
};

enum class ValueType : u_int32_t {
    Invalid = 0,
    ID,
    Stat,
    Value,
    StatAfterBlink,
    BlinkDuration,
    BlinkValueA,
    BlinkValueB,
    StatOptional,
    BlinkValueAOptional,
    BlinkValueBOptional,
    LinkedSoundID,
    LinkedSoundType,
    LinkedSoundRepeatCount,
    LinkedSoundRepeatMin,
    LinkedSoundRepeatMax,
    LinkedSoundDuration,
    LinkedSoundBlinkOnDuration,
    LinkedSoundBlinkOffDuration,
    LinkedVoiceID,
    Etc,
};

enum class Constant : u_int32_t {
    Invalid = 0,
    // SpeedGauge
    SpeedGaugeStart,
    SpeedAnalogStat,
    SpeedAnalogValue,
    SpeedDigitalStat,
    SpeedDigitalValue,
    SpeedSubDigitalStat,
    SpeedSubDigitalValue,
    SpeedMainDisplayUnitStat,
    SpeedAuxDisplayUnitStat,
    SpeedSubDisplayStat,
    SpeedScaleMaximumStat,
    NaviSpeedLimitStat,
    NaviSpeedLimitOver1ColorValue,
    NaviSpeedLimitOver2ColorValue,
    SpeedGaugeEnd,

    // Tachometer
    TachometerStart,
    RpmValue,
    RedZoneExceptNbrandStat,
    RedZoneNbrandStat,
    MaxRpmStat,
    RpmDampStat,
    TachometerEnd,

    // IntroOutro
    IntroOutroStart,
    ResvChargeStat,
    ResvClimateStat,
    PurificationAirStat,
    PurificationAirValue,
    CO2ReductionStat,
    CO2ReductionValue,
    IntroOutroEnd,

    // OAT
    OATStart,
    OutTempDisplayStat,
    OutTempDisplayUnitStat,
    OATEnd,

    // TransmissionIndicator
    TransmissionIndicatorStart,
    TransmissionIndicatorEnd,

    // DriveMode
    DriveModeStart,
    DriveModeEnd,

    // ADASParkingNew
    ADASParkingNewStart,
    ADASParkingNewEnd,

    // HighPerformanceForNBrand
    HighPerformanceForNBrandStart,
    HighPerformanceForNBrandEnd,

    // ADASDrivingNew
    ADASDrivingNewStart,
    ViewFrontVehicle,
    ADASDrivingNewEnd,
};

enum class Telltale : u_int32_t {
    Invalid = 0,
    // LampIndicator
    LampIndicatorStart,
    FrontFogStat,
    HighBeamStat,
    RearFogStat,
    TailLampStat,
    TurnSignalLeftStat,
    TurnSignalRightStat,
    LowBeamStat,
    LampIndicatorEnd,

    // OAT
    OATStart,
    IceWarnStat,
    IceWarnStatOptional,
    OATEnd,

    // DriveMode
    DriveModeStart,
    DMSStat,
    DMSStatOptional,
    DimmedStat,
    DimmedStatOptional,
    DriveModeEnd,

    // HighPerformanceForNBrand
    HighPerformanceForNBrandStart,
    CreepOffStat,
    ShiftLightStat,
    ShiftLightBlinkValueA,
    ShiftLightBlinkValueB,
    ShiftLightBlinkValueBOptional,
    ShiftLightStatOptional,
    EnduranceRaceStat,
    EnduranceRaceStatOptional,
    HighPerformanceForNBrandEnd,

    // ADASDrivingNew
    ADASDrivingNewStart,
    FCAStat,
    ELKStat,
    LKAStat,
    HBAStat,
    LFAStat,
    LCALeftStat,
    LCARightStat,
    HandsOnOffStat,
    HandsOnOffStatOptional,
    DAWStat,
    ADASDrivingNewEnd,

    // ADASDrivingNew
    HighPerformanceStart,
    Gauge,
    HighPerformanceEnd,
};

enum class Event : u_int32_t {
    Invalid = 0,
    // IntroOutro
    IntroOutroStart,
    WelcomeID,
    WelcomeStat,
    WelcomeLinkedSoundID,
    WelcomeLinkedSoundType,
    WelcomeLinkedSoundRepeatCount,
    SystemCheckID,
    SystemCheckStat,
    GoodbyeID,
    GoodbyeStat,
    GoodbyeLinkedSoundID,
    GoodbyeLinkedSoundType,
    GoodbyeLinkedSoundRepeatCount,
    IntroOutroEnd,

    // OAT
    OATStart,
    IceWarnID,
    IceWarnStat_,
    IceWarnLinkedSoundID,
    IceWarnLinkedSoundType,
    IceWarnLinkedSoundRepeatCount,
    OATEnd,

    // TransmissionIndicator
    TransmissionIndicatorStart,
    TransmissionIndicatorID,
    TransmissionIndicatorStat,
    TransmissionPaddleShiftID,
    TransmissionPaddleShiftStat,
    TransmissionSBWID,
    TransmissionSBWStat,
    TransmissionIndicatorEnd,

    // DriveMode
    DriveModeStart,
    DrivingModeChangeID,
    DrivingModeChangeStat,
    TerrainModeExitNotificationID,
    TerrainModeExitNotificationStat,
    TerrainModeExitNotificationLinkedSoundID,
    TerrainModeExitNotificationLinkedSoundType,
    TerrainModeExitNotificationLinkedSoundRepeatCount,
    ReconfirmNCustomModeID,
    ReconfirmNCustomModeStat,
    ReconfirmNCustomModeLinkedSoundID,
    ReconfirmNCustomModeLinkedSoundType,
    ReconfirmNCustomModeLinkedSoundRepeatCount,
    ModeSwitchingImpossibleID,
    ModeSwitchingImpossibleStat,
    ModeSwitchingImpossibleLinkedSoundID,
    ModeSwitchingImpossibleLinkedSoundType,
    ModeSwitchingImpossibleLinkedSoundRepeatCount,
    ReconfirmGTMYModeID,
    ReconfirmGTMYModeStat,
    ReconfirmGTMYModeLinkedSoundID,
    ReconfirmGTMYModeLinkedSoundType,
    ReconfirmGTMYModeLinkedSoundRepeatCount,
    DriveModeEnd,

    // ADASParkingNew
    ADASParkingNewStart,
    Event4GroupID,
    Event4GroupStat,
    Event7GroupFailure10ID,
    Event7GroupFailure10Stat,
    Event7GroupFailure10LinkedSoundID,
    Event7GroupFailure10LinkedSoundType,
    Event7GroupFailure10LinkedSoundRepeatCount,
    Event7GroupFailure11ID,
    Event7GroupFailure11Stat,
    Event7GroupFailure11LinkedSoundID,
    Event7GroupFailure11LinkedSoundType,
    Event7GroupFailure11LinkedSoundRepeatCount,
    Event7GroupCCWFailureID,
    Event7GroupCCWFailureStat,
    Event7GroupCCWFailureLinkedSoundID,
    Event7GroupCCWFailureLinkedSoundType,
    Event7GroupCCWFailureLinkedSoundRepeatCount,
    Event2GroupID,
    Event2GroupStat,
    ADASParkingNewEnd,

    // HighPerformanceForNBrand
    HighPerformanceForNBrandStart,
    BrakeOverrideID,
    BrakeOverrideStat,
    BrakeOverrideLinkedSoundID,
    BrakeOverrideLinkedSoundType,
    BrakeOverrideLinkedSoundRepeatCount,
    Ngs1ID,
    Ngs1Stat,
    Ngs2ID,
    Ngs2Stat,
    Ngs2LinkedSoundID,
    Ngs2LinkedSoundType,
    Ngs2LinkedSoundRepeatCount,
    NRoadSenseID,
    NRoadSenseStat,
    NRoadSenseLinkedSoundID,
    NRoadSenseLinkedSoundType,
    NRoadSenseLinkedSoundRepeatCount,
    BrakepadCheckID,
    BrakepadCheckStat,
    BrakepadCheckLinkedSoundID,
    BrakepadCheckLinkedSoundType,
    BrakepadCheckLinkedSoundRepeatCount,
    ElectricKickDriftID,
    ElectricKickDriftStat,
    ElectricKickDriftLinkedSoundID,
    ElectricKickDriftLinkedSoundType,
    ElectricKickDriftLinkedSoundRepeatCount,
    HighPerformanceForNBrandEnd,

    // ADASDrivingNew
    ADASDrivingNewStart,
    DrivingAssistSummaryID,
    DrivingAssistSummaryStat,
    Group1FullPopup1,
    Group1FullPopup1_2ID,
    Group1FullPopup1_2Stat,
    Group1FullPopup1_2LinkedSoundID,
    Group1FullPopup1_2LinkedSoundType,
    Group1FullPopup1_2LinkedSoundRepeatCount,
    Group1FullPopup2_1ID,
    Group1FullPopup2_1Stat,
    Group1FullPopup2_1LinkedSoundID,
    Group1FullPopup2_1LinkedSoundType,
    Group1FullPopup2_1LinkedSoundRepeatCount,
    Group1FullPopup3_1_E52099ID,
    Group1FullPopup3_1_E52099Stat,
    Group1FullPopup3_1_E52099LinkedSoundID,
    Group1FullPopup3_1_E52099LinkedSoundType,
    Group1FullPopup3_1_E52100ID,
    Group1FullPopup3_1_E52100Stat,
    Group1FullPopup3_1_E52101ID,
    Group1FullPopup3_1_E52101Stat,
    Group1FullPopup3_1_E52101LinkedSoundID,
    Group1FullPopup3_1_E52101LinkedSoundType,
    Group1FullPopup3_1_E52101LinkedSoundRepeatCount,
    Group1FullPopup3_1_E52105ID,
    Group1FullPopup3_1_E52105Stat,
    Group1FullPopup3_1_E52105LinkedSoundID,
    Group1FullPopup3_1_E52105LinkedSoundType,
    Group1FullPopup3_1_E52106ID,
    Group1FullPopup3_1_E52106Stat,
    Group1FullPopup3_1_E52107ID,
    Group1FullPopup3_1_E52107Stat,
    Group1FullPopup3_1_E52107LinkedSoundID,
    Group1FullPopup3_1_E52107LinkedSoundType,
    Group1FullPopup3_1_E52107LinkedSoundRepeatCount,
    Group4FullPopup1_1ID,
    Group4FullPopup1_1Stat,
    Group4FullPopup1_1LinkedSoundID,
    Group4FullPopup1_1LinkedSoundType,
    Group4FullPopup1_1LinkedSoundRepeatCount,
    Group7FullPopupBlindSpotSafetyFailureID,
    Group7FullPopupBlindSpotSafetyFailureStat,
    Group7FullPopupBlindSpotSafetyFailureLinkedSoundID,
    Group7FullPopupBlindSpotSafetyFailureLinkedSoundType,
    Group7FullPopupBlindSpotSafetyFailureLinkedSoundRepeatCount,
    Group7FullPopupOutsideMirrorSymbolFailureID,
    Group7FullPopupOutsideMirrorSymbolFailureStat,
    Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundID,
    Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundType,
    Group7FullPopupOutsideMirrorSymbolFailureLinkedSoundRepeatCount,
    Group7FullPopupHdpFailureID,
    Group7FullPopupHdpFailureStat,
    Group7FullPopupHdpFailureLinkedSoundID,
    Group7FullPopupHdpFailureLinkedSoundType,
    Group7FullPopupHdpFailureLinkedSoundRepeatCount,
    Group2MiniPopup1_1ID,
    Group2MiniPopup1_1Stat,
    PuMGroup2AdasWarning1_2StatusID,
    PuMGroup2AdasWarning1_2StatusStat,
    DriverAssistFailure1ID,
    DriverAssistFailure1Stat,
    DriverAssistFailure1LinkedSoundID,
    DriverAssistFailure1LinkedSoundType,
    DriverAssistFailure1LinkedSoundRepeatCount,
    DriverAssistFailure2ID,
    DriverAssistFailure2Stat,
    DriverAssistFailure2LinkedSoundID,
    DriverAssistFailure2LinkedSoundType,
    DriverAssistFailure2LinkedSoundRepeatCount,
    HDPOperProhibitID,
    HDPOperProhibitStat,
    ADASDrivingNewEnd,
};

enum class Sound : u_int32_t {
    Invalid = 0,
    // LampIndicator
    LampIndicatorStart,
    LampIndicatorEnd,

    // IntroOutro
    IntroOutroStart,
    IntroOutroEnd,

    // TransmissionIndicator
    TransmissionIndicatorStart,
    TransmissionIndicatorEnd,

    // ADASParkingNew
    ADASParkingNewStart,
    ADASParkingNewEnd,

    // HighPerformanceForNBrand
    HighPerformanceForNBrandStart,
    HighPerformanceForNBrandEnd,

    // ADASDrivingNew
    ADASDrivingNewStart,
    ADASDrivingNewEnd,
};

enum class Etc : u_int32_t {
    Invalid = 0,
    // SpeedGauge
    SpeedGaugeStart,
    InterDisplaySpeedUnit,
    InterDisplaySpeedValueKPH,
    InterDisplaySpeedValueMPH,
    SpeedGaugeEnd,

    // DriveMode
    DriveModeStart,
    InterDriveModeSelectStatus,
    InterTerrainModeSelectStatus,
    InterNModeSelectStatus,
    InterGtModeSelectStatus,
    DriveModeEnd,

    // ADASDrivingNew
    ADASDrivingNewStart,
    InterAdasViewFixStatus,
    InterHDPMasterWarningStatus,
    TimerOneShotTimerPriorityISLAStat,
    TimerOneShotTimerPriorityDISStat,
    InterAdasOnStatus,
    ADASDrivingNewEnd,
};

#endif  // SERIVCE_ENUM_H
