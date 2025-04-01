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
        KeyInputValueDelete = Qt::Key::Key_Delete,
        KeyInputValueCapture = Qt::Key::Key_F12,
        KeyInputValueCtrl = Qt::Key::Key_Control,
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
        PropertyTypeNodeAddressTCName,
        PropertyTypeNodeAddressConfigName,
        PropertyTypeNodeAddressModule,
        PropertyTypeAppMode,
        PropertyTypeGenType,
        PropertyTypeTCCheck,
        PropertyTypeAppModeList,
        PropertyTypeAllModuleList,
        PropertyTypeValueEnum,
        PropertyTypeInputDataValueEnum,
        PropertyTypeInputDataMatchingTableICV,
        PropertyTypeInputDataMatchingTableEV,
        PropertyTypeInputDataMatchingTableFCEV,
        PropertyTypeInputDataMatchingTablePHEV,
        PropertyTypeInputDataMatchingTableHEV,
        PropertyTypeInputDataMatchingTableSystem,
        PropertyTypeInputData,
        PropertyTypeTCNameResult,
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
        PropertyTypeExcelConfigTitle,
        PropertyTypeExcelOtherTitle,
        PropertyTypeExcelOpen,
        PropertyTypeExcelMergeStart,
        PropertyTypeExcelMerge,
        PropertyTypeExcelMergeEnd,
        PropertyTypeShortcutType,
        PropertyTypeGenTypeList,

        PropertyTypeTestReport,
        PropertyTypeKey,
        PropertyTypeRunScriptState,
        PropertyTypeViewTCFileList,
        PropertyTypeViewLogInfo,
        PropertyTypeViewLogFileList,
        PropertyTypeViewFileInfo,
        PropertyTypeKeywordTypeInfo,

        PropertyTypeList = PROPETRY_START_LIST,
        PropertyTypeOriginSheetDescription,
        PropertyTypeOriginSheetPrivates,
        PropertyTypeOriginSheetTelltales,
        PropertyTypeOriginSheetConstants,
        PropertyTypeOriginSheetEvents,
        PropertyTypeOriginSheetSounds,
        PropertyTypeOriginSheetInters,
        PropertyTypeOriginSheetOutputs,
        PropertyTypeOriginSheetConfigs,
        PropertyTypeOriginSheetMax,
        PropertyTypeConvertSheetDescription,
        PropertyTypeConvertSheetPrivates,
        PropertyTypeConvertSheetTelltales,
        PropertyTypeConvertSheetConstants,
        PropertyTypeConvertSheetEvents,
        PropertyTypeConvertSheetSounds,
        PropertyTypeConvertSheetInters,
        PropertyTypeConvertSheetOutputs,
        PropertyTypeConvertSheetConfigs,
        PropertyTypeConvertSheetMax,
        PropertyTypeListMax,

        PropertyTypeOnlyControl = PROPETRY_START_CONTROL,
        PropertyTypeLastSavedFile,
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
        EventTypeLastFolder,
        EventTypeLastFile,

        EventTypeEditCut = 1100,
        EventTypeEditCopy,
        EventTypeEditPaste,
        EventTypeEditUndo,
        EventTypeEditRedo,
        EventTypeEditInsert,
        EventTypeEditDelete,
        EventTypeEditMergeSplit,
        EventTypeEditGenType,
        EventTypeEditTCCheck,

        EventTypeViewConfig = 1200,
        EventTypeViewNodeAddress,
        EventTypeViewTCFile,
        EventTypeViewLogFile,
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
        EventTypeUpdateAutoCompleteName,
        EventTypeUpdateAutoCompleteData,
        EventTypeCheckValidation,
        EventTypeUpdateAutoCompleteNameDescription,
        EventTypeUpdateSheetTextInfo,
        EventTypeUpdateSheetCellInfo,
        EventTypeUpdateSheetInfo,
        EventTypeSheetRowInsert,
        EventTypeSheetRowDelete,
        EventTypeCellMergeSplit,
        EventTypeViewInfoClose,
        EventTypeUpdateConfig,
        EventTypeConfigReset,
        EventTypeTestReportReset,
        EventTypeGenTCModule,
        EventTypeRunTC,
        EventTypeGenTC,
        EventTypeTestReportResult,
        EventTypeTestReportCoverage,
        EventTypeEnterScriptText,
        EventTypeViewTCDisplay,
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
        EventTypeWarningMergeSplit,
        EventTypeWarningCopyCut,

        EventTypeTest = 3000,

        EventTypeList = 4000,
        EventTypeListDescription,
        EventTypeListPrivates,
        EventTypeListTelltales,
        EventTypeListConstants,
        EventTypeListEvents,
        EventTypeListSounds,
        EventTypeListInters,
        EventTypeListOutputs,
        EventTypeListConfig,
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

class ThreadEnum {
public:
    enum class ControlType {
        Invalid = 0,
        Terminate,
        Start,
        Resume,
        Wait,
    };
};

class AutoCompleteEnum {
public:
    enum class AutoComplete {
        Invalid = 0,
        GenType,
        VehicleType,
        ConfigName,
        Signal,
        Data,
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

class ExcelSheetTitle {
public:
    enum class Description {
        Test = 0,
        Version,
        Description,
#if 1   // defined(USE_SHEET_COLUMN_OLD)
        ConfigSignal,
        Data,
#endif
        Max,
    };
    enum class Config {
        ConfigName = 0,
        AndGroup,
        InputSignal,
        InputData,
        Max,
    };
    enum class Other {
        Check = 0,
        TCName,
        GenType,
        VehicleType,
        Config,
        Result,
        Case,
        InputSignal,
        InputData,
        OutputSignal,
        IsInitialize,
        OutputValue,
#if defined(USE_SHEET_COLUMN_OLD)
        ConfigSignal,
        Data,
        NegativeTest,
#endif
        Max,
    };
};

enum class CheckState {
    Invalid = 0,
    Unchecked,
    Checked,
};

class GenTypeEnum {
public:
    enum GenType {
        GenTypeInvalid = 0,
        GenTypeDefault,
        GenTypeNegativePositive,
        GenTypePositive,
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
        InputDataTypeInputData,
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

class DataTypeEnum {
public:
    enum class DataType {
        Invalid = 0,
        HUInt64,
        HInt64,
        HDouble,
        HBool,
        HString,
        Max,
    };
};

class KeywordTypeEnum {
public:
    enum class KeywordType : uint64_t {
        Invalid = 0,
        Sheet,
        Cal,
        Range,
        Collect,
        Over,  // 5
        Under,
        // Equal,
        Other,
        // Or,
        NotTrigger,
        Preset,  // 10
        ValueChanged,
        Flow,  // 12
        Not,
        DontCare,  //  14
        Timeout,
        Crc,
        TwoWay,  // 17
        DependentOn,
        MoreThanEqual,  // 19
        LessThanEqual,  // 20
        Config,

        CustomNotUsed,
        CustomNotTrigger,
        CustomOver,
        CustomUnder,
        CustomRange,
        CustomTwoWay,
        CustomFlow,
        CustomIgn,
        CustomMoreThanEqual,
        CustomLessThanEqual,
        CustomConfig,
        CustomNotDefined,

        // Invalid = 0x00000000,
        // Sheet = 0x00000001,          // InputSignal : [Sheet]
        // Cal = 0x00000002,            // OutputValue : [Cal]
        // Range = 0x00000004,          // InputData : ~
        // Collect = 0x00000008,        // OutputSignal : collect
        // Over = 0x00000010,           // InputData : >
        // Under = 0x00000020,          // InputData : <
        // // Equal = 0x00000040,          // InputData : =
        // Other = 0x00000080,          // Case : others, other
        // Or = 0x00000100,             // ConfigData : |
        // NotTrigger = 0x00000200,     // InputSignal, InputData: [Not_Trigger]
        // Preset = 0x00000400,         // InputSignal : [Preset]
        // ValueChanged = 0x00000800,   // InputData : Value Changed, ValueChanged
        // Flow = 0x00001000,           // InputData : =>
        // Not = 0x00002000,            // InputData : !
        // DontCare = 0x00004000,       // InputData : D`, D'
        // Timeout = 0x00008000,        // InputData : timeout, MESSAGE_TIMEOUT
        // Crc = 0x00010000,            // InputData : crc, CRC_ERROR
        // TwoWay = 0x00020000,         // InputData : <=>
        // DependentOn = 0x00040000,    // InputSignal : [Dependent_On]
        // MoreThanEqual = 0x00080000,  // InputData : >=
        // LessThanEqual = 0x00100000,  // InputData : <=

        // CustomNotTrigger = 0x01000000,
        // CustomOver = 0x02000000,
        // CustomUnder = 0x04000000,
        // CustomRange = 0x08000000,
        // CustomTwoWay = 0x10000000,
        // CustomFlow = 0x20000000,
        // CustomIgn = 0x80000000,
        // CustomMoreThanEqual = 0x100000000,
        // CustomLessThanEqual = 0x200000000,

#if 0  // 아래는 Keyword enum의 전체 List임, 값이 쓰였다면 주석처리 하였음
       // Invalid = 0x00000000,
       // Sheet = 0x00000001,
       // Cal = 0x00000002,
       // Range = 0x00000004,
        Collect = 0x00000008,
        // GreaterThan = 0x00000010,
        // LessThan = 0x00000020,
        // Equal = 0x00000040,
        // Other = 0x00000080,
        // Or = 0x00000100,
        // NotTrigger = 0x00000200,
        // Preset = 0x00000400,
        DependentOn = 0x00000800,
        // ValueChanged = 0x00001000,
        Comma = 0x00002000,
        // VehicleSignal = 0x00004000,
        // SFCSignal = 0x00008000,
        ReservedEnum1 = 0x00010000,
        GreaterThanOrEqual = 0x00020000,
        LessThanOrEqual = 0x00040000,
        // Flow = 0x00080000,
        // Not = 0x00100000,
        // DontCare = 0x00200000,
        // Timeout = 0x00400000,
        // Crc = 0x00800000,
        ReservedEnum2 = 0x01000000,
        ReservedEnum3 = 0x02000000,
        ReservedEnum4 = 0x04000000,
        ReservedEnum5 = 0x08000000,
        ReservedEnum6 = 0x10000000,
        ReservedEnum7 = 0x20000000,
        ReservedEnum8 = 0x40000000,
        ReservedEnum9 = 0x80000000,
#endif
        Max,
    };
};

class ValidationTypeEnum {
public:
    enum class ValidationType {
        Invalid = 0,
        ExcelData,  // TCName <- Result <- Case 이름 유효성 확인 : 중복 사용 불가(상관 관계 참고하여)
        CheckKeywordWithinData,  // 시트 키워드로 참조 하는 데이터에서 키워드 사용 확인 : 사용 불가
        DataNull,                // Signal.size > 0 && (inputData, outputValue, configData).size == 0 입력값 유무 확인
        SignalNull,              // Signal.size == 0 && (inputData, outputValue, configData).size > 0 입력값 유무 확인
        Max,
    };
};

class MergeKeywordEnum {
public:
    enum class MergeKeywordType {
        NoMergeType = 0,
        MergeStart,
        Merge,
        MergeEnd,
        Max,
    };
};

class IGNElapsedTypeEnum {
public:
    enum class IGNElapsedType {
        ElapsedOn0ms = 0,
        ElapsedOn500ms,
        ElapsedOn3000ms,
        ElapsedOn3500ms,
        ElapsedOn4000ms,
        ElapsedOn10s,

        ElapsedOff0ms = 10,
        ElapsedOff500ms,
        ElapsedOff700ms,
        ElapsedOff1000ms,
        ElapsedOff1500ms,
        Max,
    };
};

class SignalTypeEnum {
public:
    enum class SignalType {
        Invalid = 0,
        Sfc,
        SfcExternal,
        SfcEvent,
        SfcCommon,
        SfcException,
        Vehicle,
        VehicleSystem,
    };
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_ENUM_H
