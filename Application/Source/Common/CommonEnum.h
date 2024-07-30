#ifndef COMMON_ENUM_H
#define COMMON_ENUM_H

#include "CommonDefine.h"
#include <QObject>
#include <qnamespace.h>

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
#define PROPETRY_START_CONTROL 5000  // Only used inside the controller

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
        KeyInputValueUp = Qt::Key::Key_Up,
        KeyInputValueDown = Qt::Key::Key_Down,
        KeyInputValueLeft = Qt::Key::Key_Left,
        KeyInputValueRight = Qt::Key::Key_Right,
        KeyInputValueOK = Qt::Key::Key_Enter,
        KeyInputValueCancel = Qt::Key::Key_Escape,
        KeyInputValueCapture = Qt::Key::Key_F12,
#if defined(PLATFORM_X86)
        KeyInputValueNumUp = Qt::Key::Key_8,       // Numlock Key_8      - KeyInputValueUp
        KeyInputValueNumDown = Qt::Key::Key_2,     // Numlock Key_2      - KeyInputValueDown
        KeyInputValueNumLeft = Qt::Key::Key_4,     // Numlock Key_4      - KeyInputValueLeft
        KeyInputValueNumRight = Qt::Key::Key_6,    // Numlock Key_6      - KeyInputValueRight
        KeyInputValueNumOK = Qt::Key::Key_Return,  // Numlock Key_Return - KeyInputValueOK
        KeyInputValueNumOK2 = Qt::Key::Key_Alt,    // Numlock Key_Alt    -  KeyInputValueOK
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
        PropertyTypeScreenInfo,

        PropertyTypeNormal = PROPETRY_START_NORMAL,
        PropertyTypeViewType,
        PropertyTypeConfigInfoPrevious,
        PropertyTypeConfigInfo,
        PropertyTypeVehicleType,
        PropertyTypeDefaultFilePath,
        PropertyTypeNodeAddressAll,
        PropertyTypeNodeAddressSFC,
        PropertyTypeNodeAddressVSM,
        PropertyTypeNodeAddressPrivate,
        PropertyTypeNodeAddressInter,
        PropertyTypeNodeAddressModule,
        PropertyTypeAppMode,
        PropertyTypeAppModeList,
        PropertyTypeAllModuleList,
        PropertyTypeValueEnum,
        PropertyTypeInputDataValuEnum,
        PropertyTypeInputDataMatchingTableICV,
        PropertyTypeInputDataMatchingTableEV,
        PropertyTypeInputDataMatchingTableFCEV,
        PropertyTypeInputDataMatchingTablePHEV,
        PropertyTypeInputDataMatchingTableHEV,
        PropertyTypeInputDataMatchingTableSystem,
        PropertyTypeAutoInputDescriptionInfo,
        PropertyTypeUpdateSelectModule,
        PropertyTypeEnterScriptText,
        PropertyTypeShowSelectModule,
        PropertyTypeSelectModuleOfRun,
        PropertyTypeTestResultInfo,
        PropertyTypeSfcModelPath,
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
        PropertyTypeReadExcelSheetBeforeSave,
        PropertyTypeClipboardType,
        PropertyTypeShortcutType,
        PropertyTypeTestReport,
        PropertyTypeKey,
        PropertyTypeArrowKey,
        PropertyTypeReceiveKeyFocus,
        PropertyTypeRunScriptState,
        PropertyTypeViewLogInfo,
        PropertyTypeViewLogFileList,
        PropertyTypeViewLogFileInfo,

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

        PropertyTypeOnlyControl = PROPETRY_START_CONTROL,
        PropertyTypeViewLogFileListInfo,
        PropertyTypeKeySkip,
        PropertyTypeSaveFilePath,
        PropertyTypeViewLogInfoPrevious,
        PropertyTypeProcessStartPath,
        PropertyTypeTestResultCancel,
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

        EventTypeSettingSfcModelPath = 1400,
        EventTypeSettingNodePath,
        EventTypeSettingAppMode,

        EventTypeMultiDocker = 1500,

        EventTypeHelpAbout = 1600,
        EventTypeHelpAboutQt,

        EventTypeOpenExcel = 1700,
        EventTypeSaveExcel,
        EventTypeUpdateExcelSheet,
        EventTypeEditExcelSheet,
        EventTypeAutoInputDescriptionInfo,
        EventTypeSaveFromReadExcelSheet,
        EventTypeUpdateSheetTextInfo,
        EventTypeUpdateSheetCellInfo,
        EventTypeUpdateSheetInfo,
        EventTypeSheetRowInsert,
        EventTypeSheetRowDelete,
        EventTypeAutoCompleteInputData,
        EventTypeCellMergeSplit,
        EventTypeCellMergeSplitWarning,
        EventTypeViewInfoClose,
        EventTypeUpdateConfig,
        EventTypeConfigReset,
        EventTypeTestReportReset,
        EventTypeGenTC,
        EventTypeRunTC,
        EventTypeRunMultiDocker,
        EventTypeTestReportResult,
        EventTypeTestReportCoverage,
        EventTypeEnterScriptText,
        EventTypeViewLogFile,
        EventTypeViewLogDisplay,
        EventTypeGenSSFS,
        EventTypeInitModule,
        EventTypeShowModule,
        EventTypeSelectModule,
        EventTypeSelectModuleOfRun,
        EventTypeGenRunTCCancel,
        EventTypeGenRunTCCompleted,
        EventTypeEnterScriptTextCompleted,
        EventTypeRunTestReport,
        EventTypeSelectModuleError,
        EventTypeSelectAppMode,

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

class ControlUpdateTypeEnum {
public:
    enum ControlUpdateType {
        ControlUpdateTypeInvalid = 0,
        ControlUpdateTypeScriptRunnigCompleted,
    };
};

class ViewTypeEnum {
public:
    enum ViewType {
        ViewTypeInvalid = 0,
        ViewTypeConfig,
        ViewTypeNode,
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

class ExcelSheetTitle {
public:
    enum class Description {
        Test = 0,
        Version,
        Description,
        ConfigSignal,
        Data,
    };
    enum class Other {
        TCName = 0,
        VehicleType,
        Result,
        Case,
        InputSignal,
        InputData,
        OutputSignal,
        IsInitialize,
        OutputValue,
        ConfigSignal,
        Data,
        NegativeTest,
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
    enum AutoComplete {
        Draw = 0,
        Update,
        Show,
        Hide,
        Cancel,
    };
};

class VehicleTypeEnum {
public:
    enum VehicleType {
        VehicleTypeInvalid = 0,
        VehicleTypeICV,
        VehicleTypeEV,
        VehicleTypeFCEV,
        VehicleTypePHEV,
        VehicleTypeHEV,
    };
};

class InputDataTypeEnum {
public:
    enum InputDataType {
        InputDataTypeValueEnum = 0,
        InputDataTypeMatchingTableICV,
        InputDataTypeMatchingTableEV,
        InputDataTypeMatchingTableFCEV,
        InputDataTypeMatchingTablePHEV,
        InputDataTypeMatchingTableHEV,
        InputDataTypeMatchingTableSystem,
        InputDataTypeMax,
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
        RunTypeGenSSFS,
        RunTypeViewRunScriptLog,
    };
};

class WatcherTypeEnum {
public:
    enum WatcherType {
        WatcherTypeRunScript = 0,
        WatcherTypeTestResult,
    };
};

class TestResultTypeEnum {
public:
    enum TestResultType {
        TestResultTypeInvalid = 0,
        TestResultTypeStart,
        TestResultTypeUpdate,
        TestResultTypeCompleted,
        TestResultTypeCancel,
        TestResultTypeError,
        TestResultTypeCheckError,
        TestResultTypeTest,
    };
};

class TestReportTypeEnum {
public:
    enum TestReportType {
        TestReportTypeTC = 0,
        TestReportTypeGCOV,
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

class AppModeEnum {
public:
    enum AppModeType {
        AppModeTypeCV = 0,
        AppModeTypePV,
    };
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_ENUM_H
