#ifndef COMMON_ENUM_H
#define COMMON_ENUM_H

#include "CommonDefine.h"
#include <QObject>

namespace ivis {
namespace common {

#define SCREEN_MINIMUM_WIDTH 1200
#define SCREEN_MINIMUM_HEIGHT 700
#define SCREEN_MAXIMUM_WIDTH 2100
#define SCREEN_MAXIMUM_HEIGHT 1300
#define SCREEN_HEIGHT_MARGIN 60

#define PROPETRY_START_COMMON 0
#define PROPETRY_START_NORMAL 1000
#define PROPETRY_START_LIST 3000
#define PROPETRY_START_ONLY_CONTROL 5000  // Only used inside the controller

class ScreenEnum {
public:
    enum DisplayType {
        DisplayTypeInvalid = 0x000000,
        DisplayTypeMenu = 0x000001,
        DisplayTypeCenter = 0x000002,
        DisplayTypeExcel = 0x000004,

        DisplayTypeAll = 0xFFFFFF,
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
        KeyInputValueInvalid = 0,
        KeyInputValueUp = Qt::Key_Up,
        KeyInputValueDown = Qt::Key_Down,
        KeyInputValueLeft = Qt::Key_Left,
        KeyInputValueRight = Qt::Key_Right,
        KeyInputValueOK = Qt::Key_Enter,
        KeyInputValueCancel = Qt::Key_Escape,
        KeyInputValueCapture = Qt::Key_F12,
#if defined(PLATFORM_X86)
        KeyInputValueNumUp = Qt::Key_8,       // KeyInputValueUp
        KeyInputValueNumDown = Qt::Key_2,     // KeyInputValueDown
        KeyInputValueNumLeft = Qt::Key_4,     // KeyInputValueLeft
        KeyInputValueNumRight = Qt::Key_6,    // KeyInputValueRight
        KeyInputValueNumOK = Qt::Key_Return,  // KeyInputValueOK
        KeyInputValueNumOK2 = Qt::Key_Alt,    // KeyInputValueOK
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
        PropertyTypeNodeAddressAll,
        PropertyTypeNodeAddressSFC,
        PropertyTypeNodeAddressVSM,
        PropertyTypeNodeAddressModule,
        PropertyTypeAllModuleList,
        PropertyTypeUpdateSelectModule,
        PropertyTypeEnterScriptText,
        PropertyTypeShowSelectModule,
        PropertyTypeSelectModuleOfRun,
        PropertyTypeTestResultInfo,
        PropertyTypeTestReportResultInfo,
        PropertyTypeTestReportCoverageInfo,
        PropertyTypeTestReportType,
        PropertyTypeDefaultPath,
        PropertyTypeSignalListToMicom,
        PropertyTypeExcelSheetName,
        PropertyTypeExcelDescTitle,
        PropertyTypeExcelOtherTitle,
        PropertyTypeExcelSheetCount,
        PropertyTypeExcelOpen,
        PropertyTypeExcelMergeTextStart,
        PropertyTypeExcelMergeText,
        PropertyTypeExcelMergeTextEnd,
        PropertyTypeAutoComplete,
        PropertyTypeReadExcelSheetBeforeSave,
        PropertyTypeClipboardType,
        PropertyTypeShortcutType,
        PropertyTypeTestReport,

        PropertyTypeList = PROPETRY_START_LIST,
        PropertyTypeDetailInfoDescription,
        PropertyTypeDetailInfoPrivates,
        PropertyTypeDetailInfoTelltales,
        PropertyTypeDetailInfoConstants,
        PropertyTypeDetailInfoEvents,
        PropertyTypeDetailInfoSounds,
        PropertyTypeDetailInfoInters,
        PropertyTypeDetailInfoOutputs,
        PropertyTypeDetailInfoReserved0,
        PropertyTypeDetailInfoReserved1,
        PropertyTypeDetailInfoReserved2,
        PropertyTypeDetailInfoReserved3,
        PropertyTypeDetailInfoReserved4,
        PropertyTypeDetailInfoReserved5,
        PropertyTypeDetailInfoReserved6,
        PropertyTypeDetailInfoReserved7,
        PropertyTypeDetailInfoReserved8,
        PropertyTypeDetailInfoReserved9,
        PropertyTypeListMax,

        PropertyTypeOnlyControl = PROPETRY_START_ONLY_CONTROL,
        PropertyTypeSaveFilePath,
        PropertyTypeOnlyControlMax,
    };
    enum PropertyValue {
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
        EventTypeFileNew,
        EventTypeFileOpen,
        EventTypeFileSave,
        EventTypeFileSaveAs,

        EventTypeEditCut = 1100,
        EventTypeEditCopy,
        EventTypeEditPaste,
        EventTypeEditUndo,
        EventTypeEditRedo,
        EventTypeEditCellInsert,
        EventTypeEditCellDelete,
        EventTypeEditCellMergeSplit,

        EventTypeViewConfig = 1200,
        EventTypeViewNodeAddress,
        EventTypeViewPythonLib,

        EventTypeReportResult = 1300,
        EventTypeReportCoverage,

        EventTypeSettingDevPath = 1400,
        EventTypeSettingNodePath,
        EventTypeSettingVsmPath,
        EventTypeSettingTestReport,
        EventTypeSettingTestResult,
        EventTypeSettingTestCoverage,

        EventTypeHelpAbout = 1500,
        EventTypeHelpAboutQt,

        EventTypeOpenExcel = 1600,
        EventTypeSaveExcel,
        EventTypeUpdateExcelSheet,
        EventTypeEditExcelSheet,
        EventTypeSaveFromReadExcelSheet,
        EventTypeUpdateSheetTextInfo,
        EventTypeUpdateSheetCellInfo,
        EventTypeUpdateSheetInfo,
        EventTypeSheetRowInsert,
        EventTypeSheetRowDelete,
        EventTypeCellMergeSplit,
        EventTypeCellMergeSplitWarning,
        EventTypeViewInfoClose,
        EventTypeUpdateConfig,
        EventTypeConfigReset,
        EventTypeTestReportReset,
        EventTypeGenTC,
        EventTypeRunTC,
        EventTypeTestReportResult,
        EventTypeTestReportCoverage,
        EventTypeEnterScriptText,
        EventTypeShowModule,
        EventTypeSelectModule,
        EventTypeSelectModuleOfRun,
        EventTypeGenRunTCCancel,
        EventTypeEnterScriptTextCompleted,
        EventTypeRunTestReport,

        EventTypeTest = 3000,
        EventTypeLastFile,

        EventTypeList = 4000,
        EventTypeListDescription,
        EventTypeListPrivates,
        EventTypeListTelltales,
        EventTypeListConstants,
        EventTypeListEvents,
        EventTypeListSounds,
        EventTypeListInters,
        EventTypeListOutputs,
        EventTypeListReserved0,
        EventTypeListReserved1,
        EventTypeListReserved2,
        EventTypeListReserved3,
        EventTypeListReserved4,
        EventTypeListReserved5,
        EventTypeListReserved6,
        EventTypeListReserved7,
        EventTypeListReserved8,
        EventTypeListReserved9,
        EventTypeListMax,
    };
    enum EventValue {
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
        ViewTypeReport,
        ViewTypeNodeAddress,
    };
};

class ReportTypeEnum {
public:
    enum ReportType {
        ReportTypeResult = 0,
        ReportTypeCoverage,
        ReportTypeAll,
    };
};

class CellInfoEnum {
public:
    enum ListInfoExcel {
        Sheet = 0,
        Count,  // row, column
        Title,
        Data,  // Data 1 ~ Data n-1
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
    enum class AutoComplete {
        Draw = 0,
        Update,
        Show,
        Hide,
        Cancel,
    };
};

class VsmTypeEnum {
public:
    enum VsmType {
        VsmTypeEV = 0,
        VsmTypeFCEV,
        VsmTypeICV,
    };
};

class RunTypeEnum {
public:
    enum RunType {
        RunTypeGenTC = 0,
        RunTypeRunTC,
        RunTypeTCReport,
        RunTypeGcovReport,
        RunTypeEnterScriptText,
    };
};

class TestReultTypeEnum {
public:
    enum TestReultType {
        TestReultTypeStart = 0,
        TestReultTypeUpdate,
        TestReultTypeCompleted,
        TestReultTypeCancel,
        TestReultTypeError,
        TestReultTypeTest,
    };
};

class TestReportTypeEnum {
public:
    enum TestReportType {
        TestReportTypeResult = 0,
        TestReportTypeCoverage,
    };
};

class ShortcutTypeEnum {
public:
    enum ShortcutType {
        ShortcutTypeInvalid = 0,
        ShortcutTypeNew,
        ShortcutTypeSave,
        ShortcutTypeSaveAs,
        ShortcutTypeCopy,
        ShortcutTypePaste,
        ShortcutTypeCut,
        ShortcutTypeExit,
        ShortcutTypeInsert,
        ShortcutTypeDelete,
        ShortcutTypeMergeSplit,
        ShortcutTypeUndo,
        ShortcutTypeRedo,
    };
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_ENUM_H
