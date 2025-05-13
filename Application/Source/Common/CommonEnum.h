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
        PropertyTypeExcelDependentOn,
        PropertyTypeExcelOtherTitle,
        PropertyTypeExcelView,
        PropertyTypeExcelMergeStart,
        PropertyTypeExcelMerge,
        PropertyTypeExcelMergeEnd,
        PropertyTypeShortcutType,
        PropertyTypeGenTypeList,
        PropertyTypeTerminalInfo,

        PropertyTypeTestReport,
        PropertyTypeKey,
        PropertyTypeRunScriptState,
        PropertyTypeViewTCFileList,
        PropertyTypeViewLogInfo,
        PropertyTypeViewLogFileList,
        PropertyTypeViewFileInfo,
        PropertyTypeKeywordTypeInfo,

        PropertyTypeTestButtonVisible,

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
        PropertyTypeOriginSheetDependentOn,
        PropertyTypeOriginSheetMax,  // 3011
        PropertyTypeConvertSheetDescription,
        PropertyTypeConvertSheetPrivates,
        PropertyTypeConvertSheetTelltales,
        PropertyTypeConvertSheetConstants,
        PropertyTypeConvertSheetEvents,
        PropertyTypeConvertSheetSounds,
        PropertyTypeConvertSheetInters,
        PropertyTypeConvertSheetOutputs,
        PropertyTypeConvertSheetConfigs,
        PropertyTypeConvertSheetDependentOn,
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
        EventTypeTerminalMode,
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
        EventTypeUpdateGenTCInfo,
        EventTypeGenerateCancel,

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
    enum CenterViewType {
        CenterViewTypeInvalid = 0,
        CenterViewTypeConfig,
        CenterViewTypeNode,
        CenterViewTypeTerminal,
    };
    enum ExcelType {
        ExcelTypeInvalid = 0,
        ExcelTypeNew,
        ExcelTypeOpen,
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
        ValueCount,
        PreconditionCount,
        ConfigSignal,
        Data,
        Max,
    };
    enum class Config {
        ConfigName = 0,
        AndGroup,
        InputSignal,
        InputData,
        Max,
    };
    enum class DependentOn {
        DependentName = 0,
        Result,
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
    // enum class TesCaseType {
    //     Invalid = 0,
    //     Parsing,
    //     Generate,
    // };
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
    enum class KeywordGroup {
        Normal = 0,
        Single,
        Multi,
    };
    enum class KeywordType : uint64_t {
        Invalid = 0,
        Sheet,
        Cal,
        Range,
        Collect,
        Over,  // 5
        Under,
        Other,
        NotTrigger,
        NotTriggerSheet,
        Preset,  // 10
        ValueChanged,
        Flow,
        FlowNot,
        Not,
        NotFlow,  // 15
        NotRange,
        DontCare,
        Timeout,
        Crc,
        TwoWay,  // 20
        DependentOn,
        MoreThanEqual,
        LessThanEqual,
        Config,  // 24

        CustomNotUsed,
        CustomNotTrigger,
        CustomNotTriggerSheet,  // used interpreted Sheet keyword only in Negative & Positive
        CustomNot,              // used int, uint, double type only (except enum type)
        CustomDontCare,         // used int, uint, double type only (except enum type)
        CustomNotFlow,          // used int, uint, double type only (except enum type) -> TODO: 케이스 존재 시 개발 예정
        CustomNotRange,         // used int, uint, double type only (except enum type) -> TODO: 케이스 존재 시 개발 예정
        CustomFlowNot,
        CustomOver,
        CustomUnder,
        CustomRange,  // 35
        CustomTwoWay,
        CustomFlow,
        CustomIgn,
        CustomMoreThanEqual,
        CustomLessThanEqual,  // 40
        CustomConfig,
        CustomNotDefined,
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
        SfcCommonCV,
        SfcException,
        Vehicle,
        VehicleSystem,
    };
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_ENUM_H
