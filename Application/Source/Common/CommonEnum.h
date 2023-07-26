#ifndef COMMON_ENUM_H
#define COMMON_ENUM_H

#include "CommonDefine.h"
#include <QObject>



#define SCREEN_POSITION_X                50
#define SCREEN_POSITION_Y                100
#define SCREEN_SIZE_WIDTH                1280
#define SCREEN_SIZE_HEIGHT               720
#define SCREEN_MINIMUM_WIDTH             640
#define SCREEN_MINIMUM_HEIGHT            480
#define SCREEN_MAXIMUM_WIDTH             2100
#define SCREEN_MAXIMUM_HEIGHT            1300



#define PROPETRY_START_COMMON            0
#define PROPETRY_START_NORMAL            1000
#define PROPETRY_START_LIST              3000





class ScreenEnum {
public:
    enum DisplayType {
        DisplayTypeInvalid             = 0x000000,
        DisplayTypeTop                 = 0x000001,
        DisplayTypeCenter              = 0x000002,
        DisplayTypeBottom              = 0x000004,
        DisplayTypeLeft                = 0x000008,
        DisplayTypeRight               = 0x000010,

        DisplayTypePopup               = 0x800000,

        DisplayTypeAll                 = 0xFFFFFF,
    };
    enum DisplayDepth {
        DisplayDepthDepth0 = 0,
        DisplayDepthDepth1,
        DisplayDepthDepth2,
    };
};

class KeyTypeEnum {
public:
    enum KeyInputType {
        KeyInputTypeInvalid = 0,
        KeyInputTypePress,
        KeyInputTypeRelease,
    };

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
};

class PropertyTypeEnum {
public:
    enum PropertyType {
        PropertyTypeCommon = PROPETRY_START_COMMON,
        PropertyTypeDisplay,
        PropertyTypeDisplaySize,
        PropertyTypeDisplaySizeMargin,
        PropertyTypeMode,
        PropertyTypeVisible,
        PropertyTypeDepth,

        PropertyTypeNormal = PROPETRY_START_NORMAL,
        PropertyTypeDefaultPath,
        PropertyTypeSignalListAll,  // SFC + VSM
        PropertyTypeSignalListSFC,
        PropertyTypeSignalListVSM,
        PropertyTypeSignalListToMicom,
        PropertyTypeUpdateSheetInfoNew,
        PropertyTypeUpdateSheetInfoOpen,
        PropertyTypeSaveFilePath,

        PropertyTypeList = PROPETRY_START_LIST,
        PropertyTypeDetailInfoDescription,
        PropertyTypeDetailInfoPrivates,
        PropertyTypeDetailInfoTelltales,
        PropertyTypeDetailInfoConstants,
        PropertyTypeDetailInfoEvents,
        PropertyTypeDetailInfoSounds,
        PropertyTypeDetailInfoInters,
        PropertyTypeDetailInfoOutputs,
        PropertyTypeDetailInfoReserved1,
        PropertyTypeDetailInfoReserved2,
        PropertyTypeDetailInfoReserved3,
        PropertyTypeDetailInfoReserved4,
        PropertyTypeDetailInfoReserved5,
        PropertyTypeDetailInfoReserved6,
        PropertyTypeDetailInfoReserved7,
    };
};

class EventTypeEnum {
public:
    enum EventType {
        EventTypeInvalid = 0,
        EventTypeExitProgram,
        EventTypeDisplayChange,

        EventTypeNormal = 1000,
        EventTypeEditCut,
        EventTypeEditCopy,
        EventTypeEditPaste,
        EventTypeSettingDevPath,
        EventTypeSettingTestReport,
        EventTypeSettingTestResult,
        EventTypeSettingTestCoverage,
        EventTypeHelpAbout,
        EventTypeHelpAboutQt,
        EventTypeOpenExcel,
        EventTypeSaveExcel,
        EventTypeUpdateDevPath,
        EventTypeUpdateSheetInfo,

        EventTypeSendOtherControl = 2000,
        EventTypeCenterVisible,
        EventTypeFileNew,
        EventTypeFileOpen,
        EventTypeFileSave,
        EventTypeFileSaveAs,
    };
};

class ListInfoEnum {
public:
    enum ListInfoExcel {
        Sheet = 0,
        Count,  // row, column
        Title,
        Data,   // Data 1 ~ Data n-1
    };
};



























#endif  // COMMON_ENUM_H
