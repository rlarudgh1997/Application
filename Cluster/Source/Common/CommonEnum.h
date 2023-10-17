#ifndef COMMON_ENUM_CLUSTER_H
#define COMMON_ENUM_CLUSTER_H

#include "CommonDefine.h"
#include <QObject>


namespace ivis {
namespace common {


#define SCREEN_POSITION_X                50
#define SCREEN_POSITION_Y                100
#define SCREEN_SIZE_WIDTH                1920
#define SCREEN_SIZE_HEIGHT               720
#define SCREEN_MINIMUM_WIDTH             1200
#define SCREEN_MINIMUM_HEIGHT            700
#define SCREEN_MAXIMUM_WIDTH             2100
#define SCREEN_MAXIMUM_HEIGHT            1300
#define SCREEN_HEIGHT_MARGIN             60

#define PROPETRY_START_COMMON            0
#define PROPETRY_START_NORMAL            1000
#define PROPETRY_START_LIST              2000
#define PROPETRY_START_HOME              3000
#define PROPETRY_START_GAUGE             4000
#define PROPETRY_START_CONTENT           5000
#define PROPETRY_START_TELLTALE          6000
#define PROPETRY_START_POPUP             7000




class DisplayEnum : public QObject {
    Q_OBJECT

public:
    enum DisplaySize {
        Width  = SCREEN_SIZE_WIDTH,
        Height = SCREEN_SIZE_HEIGHT,
    };
    Q_ENUMS(DisplaySize)

    enum DisplayType {
        DisplayTypeInvalid             = 0x000000,
        DisplayTypeHome                = 0x000001,
        DisplayTypeGauge               = 0x000002,
        DisplayTypeTelltale            = 0x000004,
        DisplayTypePopup               = 0x000008,

        DisplayTypeAll                 = 0xFFFFFF,
    };
    Q_ENUMS(DisplayType)

    enum DisplayDepth {
        DisplayDepthDepth0 = 0,
        DisplayDepthDepth1,
        DisplayDepthDepth2,
    };
    Q_ENUMS(DisplayDepth)
};

class KeyEnum : public QObject {
    Q_OBJECT

public:
    enum KeyInputType {
        KeyInputTypeInvalid = 0,
        KeyInputTypePress,
        KeyInputTypeRelease,
    };
    Q_ENUMS(KeyInputType)

    enum KeyInputValue {
        KeyInputValueInvalid          = 0,
        KeyInputValueUp               = Qt::Key_Up,
        KeyInputValueDown             = Qt::Key_Down,
        KeyInputValueLeft             = Qt::Key_Left,
        KeyInputValueRight            = Qt::Key_Right,
        KeyInputValueOK               = Qt::Key_Enter,
        KeyInputValueCancel           = Qt::Key_Escape,
        KeyInputValueCapture          = Qt::Key_F12,
#if defined(PLATFORM_X86)
        KeyInputValueNumUp            = Qt::Key_8,           // KeyInputValueUp
        KeyInputValueNumDown          = Qt::Key_2,           // KeyInputValueDown
        KeyInputValueNumLeft          = Qt::Key_4,           // KeyInputValueLeft
        KeyInputValueNumRight         = Qt::Key_6,           // KeyInputValueRight
        KeyInputValueNumOK            = Qt::Key_Return,      // KeyInputValueOK
        KeyInputValueNumOK2           = Qt::Key_Alt,         // KeyInputValueOK
#endif
    };
    Q_ENUMS(KeyInputValue)
};

class PropertyEnum : public QObject {
    Q_OBJECT

public:
    enum  CommonType {
        CommonInvalid = PROPETRY_START_COMMON,
        CommonDisplay,
        CommonVisible,
        CommonRect,
        CommonVehicleType,

        CommonNormal = PROPETRY_START_NORMAL,

        CommonList = PROPETRY_START_LIST,
        CommonMax,
    };
    Q_ENUMS(CommonType)

    enum HomeType  {
        HomeInvalid = PROPETRY_START_HOME,
        HomeType,
    };
    Q_ENUMS(HomeType)

    enum GaugeType  {
        GaugeInvalid = PROPETRY_START_GAUGE,
        GaugeType,
    };
    Q_ENUMS(GaugeType)

    enum ContentType  {
        ContentInvalid = PROPETRY_START_CONTENT,
        ContentType,
    };
    Q_ENUMS(GaugeType)

    enum TelltaleType  {
        TelltaleInvalid = PROPETRY_START_TELLTALE,
        TelltaleType,
        TelltaleDualPowerStat,
        TelltaleDualPowerOptional,
        TelltaleLiftgateStat,
        TelltaleBEQStat,
        TelltaleTachographStat,
        TelltaleHeatedMirrorStat,
        TelltaleWorkingLampStat,
        TelltaleASRStat,
        TelltaleASRBlinkValueA,
        TelltaleASRBlinkValueB,
        TelltaleEBSAmberLampStat,
        TelltaleEBSRedLampStat,
        TelltaleLWSStat,
        TelltaleLWSBlinkValueA,
        TelltaleLWSBlinkValueB,
        TelltaleECASStat,
        TelltaleECASOptionalStat,
        TelltaleECASBlinkValueA,
        TelltaleECASBlinkValueAOptional,
        TelltaleECASBlinkValueB,
        TelltaleECASAxleMotionStat,
        TelltaleECASAxleMotionStatOptional,
        TelltaleBrakeAirLowLampStat,
        TelltaleEHSAmberLampStat,
        TelltaleEHSAmberLampBlinkValueA,
        TelltaleEHSAmberLampBlinkValueB,
        TelltaleEAPULampStat,
        TelltaleAutoGreaseLowLampStat,
        TelltaleCabTiltingStat,
        TelltaleTMPTOStat,
        TelltaleDifferencialLockStat,
        TelltaleTMHighStat,
        TelltaleWheelLockStat,
        TelltaleRetarderLampStat,
        TelltaleRetarderMalLampStat,
        TelltaleHighVoltageBatteryChargeStat,
        TelltaleTPMSCVStat,
        TelltaleTPMSCVBlinkValueA,
        TelltaleTPMSCVBlinkValueB,
        TelltaleABSTrailerLampStat,
        TelltaleNoABSTrailerLamp,
        TelltaleABSCVStat,
        TelltaleVDCStat,
        TelltaleVDCBlinkValueA,
        TelltaleVDCBlinkValueB,
        TelltaleVDCOFFStat,
        TelltaleStreeingWarningLampStat,
        TelltaleParkingBrakeCVStat,
        TelltaleSrvLmpEvStat,
        TelltaleSrvLmpHevStat,
        TelltaleSrvLmpFcevStat,
        TelltaleEcallStat,
        TelltaleSeatbeltStat,
        TelltaleOATIceWarnStat,
        TelltaleOATIceWarnStatOptional,
        TelltaleAirBagStat,
        TelltaleAirBagBlinkValueA,
        TelltaleAirBagBlinkValueB,
        TelltaleLampFailWarningStat,
        TelltaleIFSStat,
        TelltalexEVSystemReadyStat,
        TelltaleKeySystemStat,
        TelltaleLampIndicatorFrontFogStat,
        TelltaleLampIndicatorHighBeamStat,
        TelltaleLampIndicatorRearFogStat,
        TelltaleLampIndicatorTailLampStat,
        TelltaleLampIndicatorTurnSignalLeftStat,
        TelltaleLampIndicatorTurnSignalRightStat,
        TelltaleLampIndicatorLowBeamStat,
        TelltaleH2FaultStat,
        TelltaleH2LeakStat,
        TelltaleAFSDBLStat,
        TelltaleAFSDBLBlinkValueA,
        TelltaleAFSDBLBlinkValueB,
        TelltaleEvPwrDnStat,
        TelltaleLDWLKAStat,
        TelltaleLDWLKABlinkValueA,
        TelltaleLDWLKABlinkValueB,
        TelltaleFCAStat,
        TelltaleFCABlinkValueA,
        TelltaleFCABlinkValueB,
        TelltaleELKStat,
        TelltaleELKBlinkValueA,
        TelltaleELKBlinkValueB,
        TelltaleLowFuelWarnStat,
        TelltaleLowFuelWarnStatOptional,
        TelltaleLowFuelWarnBlinkValueA,
        TelltaleLowFuelWarnBlinkValueB,
        TelltaleADASDrivingNewELKStat,
        TelltaleADASDrivingNewFCAStat,
        TelltaleADASDrivingNewLKAStat,
        TelltaleADASDrivingNewHBAStat,
        TelltaleADASDrivingNewLCALeftStat,
        TelltaleADASDrivingNewLCARightStat,
        TelltaleADASDrivingNewLFAStat,
        TelltalexEVServiceLampCVSrvLmpEvStat,
        TelltalexEVServiceLampCVSrvLmpFcevStat,
        TelltalexEVPowerLimitEvPwrDnStat,
        TelltaleGFDWarningGFDStat,
    };
    Q_ENUMS(TelltaleType)

    enum PopupType  {
        PopupInvalid = PROPETRY_START_POPUP,
        PopupType,
    };
    Q_ENUMS(PopupType)
};

class EventEnum : public QObject {
    Q_OBJECT

public:
    enum EventType {
        EventTypeInvalid = 0,
        EventTypeExitProgram,

        EventTypeNormal = 1000,

        EventTypeSendOtherControl = 2000,

        EventTypeList = 3000,
        EventTypeListMax,
    };
    Q_ENUMS(EventType)
};



class HandlerHomeEnum : public QObject {
    Q_OBJECT

public:
    enum HomeType {
        Invalid = 0,
        Main,
        Menu,
        Sub,
    };
    Q_ENUMS(HomeType)
};


class TelltaleStatType : public QObject {
    Q_OBJECT

public:
    enum class TelltaleStat : uint64_t {
        NONE = 0x0,
        OFF = 0x1,
        ON = 0x2,
        BLINK1 = 0xa1,  // 300, 200
        BLINK2 = 0xa2,  // 600, 400
        BLINK3 = 0xa3,  // 120, 120
        BLINK4 = 0xa4,  // 1000, 1000
        BLINK5 = 0xa5,  // 3000, 3000
        RED = 0xc1,
        GREEN = 0xc2,
        BLUE = 0xc3,
        YELLOW = 0xc4,
        AMBER = 0xc5,
        GRAY = 0xc6,
        WHITE = 0xc7,
        OPTIONAL = 0xc8,
        MAX = 0xc9,
    };
    Q_ENUMS(TelltaleStat)
};

class TelltaleECASStatOptionalType : public QObject {
    Q_OBJECT

public:
    enum class TelltaleECASStatOptional : uint64_t {
        NONE = 0x0,
        OFF = 0x1,
        FR_UP_ON = 0x2,
        FR_LOW_ON = 0x3,
        RR_UP_ON = 0x4,
        RR_LOW_ON = 0x5,
        ALL_UP_ON = 0x6,
        ALL_LOW_ON = 0x7,
        FR_UP_RR_LOW_ON = 0x8,
        FR_LOW_RR_UP_ON = 0x9,
        TO_NORMAL_ON = 0xa,
        ABOVE_SPEED_LIMIT = 0xb,
        EXCEED_AXLE_LOAD = 0xc,
        LIMIT_AXLE_LOAD = 0xd,
        ECAS_CHECK = 0xe,
        MAX = 0xf,
    };
    Q_ENUM(TelltaleECASStatOptional)
};

class TelltaleECASAxleMotionStatOptionalType : public QObject {
    Q_OBJECT

public:
    enum class TelltaleECASAxleMotionStatOptional : uint64_t {
        NONE = 0x0,
        OFF = 0x1,
        AXLE_UP_ON = 0x2,
        TRACTION_HELP_ON = 0x3,
        MAX = 0x4,
    };
    Q_ENUM(TelltaleECASAxleMotionStatOptional)
};

class TelltaleOATIceWarnStatOptionalType : public QObject {
    Q_OBJECT

public:
    enum class TelltaleOATIceWarnStatOptional : u_int64_t {
        NONE = 0x0,
        OFF,
        WARN_ON,
        MAX,
    };
    Q_ENUM(TelltaleOATIceWarnStatOptional)
};

class TelltaleLowFuelWarnStatOptionalType : public QObject {
    Q_OBJECT

public:
    enum class TelltaleLowFuelWarnStatOptional : uint64_t {
        NONE = 0x0,
        OFF = 0x1,
        ON_LPI = 0x2,
        MAX = 0x3,
    };
    Q_ENUM(TelltaleLowFuelWarnStatOptional)
};












}  // end of namespace common
}  // end of namespace ivis




#endif  // COMMON_ENUM_CLUSTER_H
