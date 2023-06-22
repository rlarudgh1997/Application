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



#define PROPETRY_START_NORMAL            1000






class ScreenEnum {
public:
    enum DisplayType {
        DisplayTypeInvalid             = 0x000000,
        DisplayTypeMain                = 0x000001,
        DisplayTypeRadio               = 0x000002,
        DisplayTypePhone               = 0x000004,
        DisplayTypeMedia               = 0x000008,
        DisplayTypeCamera              = 0x000010,
        DisplayTypeTractor             = 0x000020,
        DisplayTypeAutonomousDriving   = 0x000040,
        DisplayTypeSetup               = 0x000080,
        DisplayTypeContent             = 0x000100,

        DisplayTypeModeMax             = 0x100000,
        DisplayTypeTop                 = DisplayTypeModeMax,
#if defined(USE_SHORTCUTS)
        DisplayTypeShortcuts           = 0x200000,
#endif
        DisplayTypeToastPopup          = 0x400000,
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
        PropertyTypeDisplay = 0,
        PropertyTypeMode,
        PropertyTypeDepth,

        PropertyTypeNormal = PROPETRY_START_NORMAL,
        PropertyTypeDefaultPath,
        PropertyTypeSheetName,
        PropertyTypeContextName,
    };
};

class EventTypeEnum {
public:
    enum EventType {
        PropertyTypeExitProgram = 0,
        PropertyTypeChangeDepth,
        PropertyTypeFileNew,
        PropertyTypeFileOpen,
        PropertyTypeFileSave,
        PropertyTypeFileSaveAs,
        PropertyTypeEditCut,
        PropertyTypeEditCopy,
        PropertyTypeEditPaste,
        PropertyTypeSettingDevPath,
        PropertyTypeSettingTestReport,
        PropertyTypeSettingTestResult,
        PropertyTypeSettingTestCoverage,
        PropertyTypeHelpAbout,
        PropertyTypeHelpAboutQt,
    };
};































#endif  // COMMON_ENUM_H
