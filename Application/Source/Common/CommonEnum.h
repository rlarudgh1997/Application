#ifndef COMMON_ENUM_H
#define COMMON_ENUM_H

#include "CommonDefine.h"
#include <QObject>


namespace ivis {
namespace common {


#define SCREEN_MINIMUM_WIDTH             1200
#define SCREEN_MINIMUM_HEIGHT            700
#define SCREEN_MAXIMUM_WIDTH             2100
#define SCREEN_MAXIMUM_HEIGHT            1300
#define SCREEN_MARGIN_HEIGHT             60

#define PROPETRY_START_COMMON            0
#define PROPETRY_START_NORMAL            1000
#define PROPETRY_START_LIST              3000





class ScreenEnum {
public:
    enum DisplayType {
        DisplayTypeInvalid             = 0x000000,
        DisplayTypeTop                 = 0x000001,
        DisplayTypeCenter              = 0x000002,
        DisplayTypeExcel               = 0x000004,
        // DisplayTypeBottom              = 0x000008,
        // DisplayTypeLeft                = 0x000010,
        // DisplayTypeRight               = 0x000020,

        // DisplayTypePopup               = 0x800000,

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
        PropertyTypeInvalid = PROPETRY_START_COMMON,
        PropertyTypeCommon = PropertyTypeInvalid,
        PropertyTypeDisplay,
        PropertyTypeDisplaySize,
        PropertyTypeDisplaySizeMargin,
        PropertyTypeMode,
        PropertyTypeVisible,
        PropertyTypeDepth,

        PropertyTypeNormal = PROPETRY_START_NORMAL,
        PropertyTypeViewType,
        PropertyTypeConfigInfo,
        PropertyTypeDefaultPath,
        PropertyTypeSignalListAll,  // SFC + VSM
        PropertyTypeSignalListSFC,
        PropertyTypeSignalListVSM,
        PropertyTypeSignalListToMicom,
        PropertyTypeUpdateSheetInfoNew,
        PropertyTypeUpdateSheetInfoOpen,
        PropertyTypeUpdateEditSheet,
        PropertyTypeExcelBlankText,
        PropertyTypeFileSaveType,

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
    enum PropertyValue{
        PropertyValueInvalid = 0,
        PropertyValueInt,
        PropertyValueString,
        PropertyValueList,
    };
};

class EventTypeEnum {
public:
    enum EventType {
        EventTypeInvalid = 0,
        EventTypeExitProgram,

        EventTypeNormal = 1000,
        EventTypeEditCut,
        EventTypeEditCopy,
        EventTypeEditPaste,
        EventTypeViewSignal,
        EventTypeViewPythonLib,
        EventTypeSettingDevPath,
        EventTypeSettingNodePath,
        EventTypeSettingTestReport,
        EventTypeSettingTestResult,
        EventTypeSettingTestCoverage,
        EventTypeHelpAbout,
        EventTypeHelpAboutQt,
        EventTypeOpenExcel,
        EventTypeSaveExcel,
        EventTypeUpdateSheetTextInfo,
        EventTypeUpdateSheetCellInfo,
        EventTypeUpdateSheetInfo,
        EventTypeSheetRowInsert,
        EventTypeSheetRowDelete,
        EventTypeCellMergeSplit,
        EventTypeCellMergeSplitWarning,
        EventTypeViewInfoClose,

        EventTypeSendOtherControl = 2000,
        EventTypeLastFile,
        EventTypeFileNew,
        EventTypeFileOpen,
        EventTypeFileSave,
        EventTypeFileSaveAs,
        EventTypeFileSaveType,
        EventTypeViewConfig,
        EventTypeWriteConfig,
    };
    enum EventValue{
        EventValueInvalid = 0,
        EventValueInt,
        EventValueString,
        EventValueList,
    };
};

class ViewTypeEnum {
public:
    enum ViewType {
        ViewTypeConfig = 0,
        ViewTypeSignal,
    };
};

class CellInfoEnum {
public:
    enum ListInfoExcel {
        Sheet = 0,
        Count,  // row, column
        Title,
        Data,   // Data 1 ~ Data n-1
    };
};

class EditCellEnum {
public:
    enum EditCellInfo {
        Invalid = 0,
        Insert,
        Delete,
        Merge,
        Split,
    };
    // enum class MenuItemRight {
    //     Invalid = 0,
    //     RowInsert,
    //     RowDelete,
    //     CellMergeSplit,
    // };
};























}  // end of namespace common
}  // end of namespace ivis




#endif  // COMMON_ENUM_H
