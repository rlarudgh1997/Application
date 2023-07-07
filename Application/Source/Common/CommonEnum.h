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
        PropertyTypeDisplay = 0,
        PropertyTypeMode,
        PropertyTypeVisible,
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
        EventTypeExitProgram = 0,
        EventTypeDisplayChange,

        EventTypeTopStart = 1000,
        EventTypeFileNew,
        EventTypeFileOpen,
        EventTypeFileSave,
        EventTypeFileSaveAs,
        EventTypeEditCut,
        EventTypeEditCopy,
        EventTypeEditPaste,
        EventTypeSettingDevPath,
        EventTypeSettingTestReport,
        EventTypeSettingTestResult,
        EventTypeSettingTestCoverage,

        EventTypeCenterStart = 2000,
        EventTypeCenterVisible,
        EventTypeItemChange,
    };
};


class ItemTypeEnum {
public:
    enum ItemType {
        Widget = 0,
        MainWindow,
        Menu,
        ToolBar,
        TabeWidget,
    };
};





























#endif  // COMMON_ENUM_H
