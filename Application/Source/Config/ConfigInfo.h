#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QApplication>
#include <QVariant>
#include <QMap>
#include <QRect>

#define SCREEN_POSITION_X 50
#define SCREEN_POSITION_Y 100
#define SCREEN_SIZE_WIDTH 1350
#define SCREEN_SIZE_HEIGHT 900

class ConfigInfo {
public:
    typedef enum {
        ConfigTypeInvalid = 0,

        // General
        ConfigTypeScreenInfo,
        ConfigTypeAppMode,
        ConfigTypeSfcModelPath,
        ConfigTypeNodeAddressPath,
        ConfigTypeNewSheetRowCount,
        ConfigTypeGenTypeList,
        ConfigTypeSheetName,
        ConfigTypeDescTitle,
        ConfigTypeOtherTitle,
        ConfigTypeConfigTitle,
        ConfigTypeDescConfigVisible,
        ConfitTypeGeneratePython,

        // File
        ConfigTypeLastSavedFilePath,
        ConfigTypeDeleteFileTC,
        ConfigTypeSaveConvertExcel,
        ConfigTypeExcelMergeStart,
        ConfigTypeExcelMerge,
        ConfigTypeExcelMergeEnd,
        ConfigTypeTCFileSplitSize,
        ConfigTypeGenTCParsingCycle,
        ConfigTypeWindowTitle,

        // Preset
        ConfigTypePreset001,
        ConfigTypePreset002,
        ConfigTypePreset003,

        // Vehicle
        ConfigTypeVsmFileNameBaseCV,
        ConfigTypeVehicleTypeCV,
        ConfigTypeSfcSpecTypeCV,
        ConfigTypeVsmSpecTypeCV,
        ConfigTypeVsmFileNameBasePV,
        ConfigTypeVehicleTypePV,
        ConfigTypeSfcSpecTypePV,
        ConfigTypeVsmSpecTypePV,
        ConfigTypeSystemTypePV,

        // ==========================================================================
        // Do not save config file : The setting information is not saved as a file.
        // ==========================================================================
        ConfigTypeMaxDoNotSave,

        // Report
        ConfigTypeReportResult,
        ConfigTypeReportResultSplit,
        ConfigTypeReportResultConfig,
        ConfigTypeReportResultExcel,
        ConfigTypeReportCoverage,
        ConfigTypeReportCoverageLine,
        ConfigTypeReportCoverageFunction,
        ConfigTypeReportCoverageBranch,

        // Common
        ConfigTypeInit,
        ConfigTypeMode,
        ConfigTypeGraphicsMode,
        ConfigTypeTCFilePath,
        ConfigTypeCLIModeTCCheck,
        ConfigTypeCLIModeGenType,
        ConfigTypeCheckLibOpenpyxl,
        ConfigTypeCheckLibPandas,
        ConfigTypeDoFileSave,
        ConfigTypeAllModule,
        ConfigTypeSelectModule,
        ConfitTypeNewModule,
        ConfitTypeGenerateStart,

        ConfigTypeMax,
    } ConfigType;
    typedef enum {
        ConfigGetTypeName = 0,
        ConfigGetTypeValue,
    } ConfigGetType;

public:
    ConfigInfo() {
        initConfigInfo();
    }
    QVariant getConfigInfo(const ConfigType& configType, const ConfigGetType& getType) const {
        return ((getType == ConfigGetTypeName) ? (QVariant(mConfigInfoData[configType].first))
                                               : (QVariant(mConfigInfoData[configType].second)));
    }

private:
    void initConfigInfo() {
        mConfigInfoData.clear();

        // General
        mConfigInfoData[ConfigTypeScreenInfo] = QPair<QString, QVariant>(
            "ConfigTypeScreenInfo", QVariant(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT)));
        mConfigInfoData[ConfigTypeAppMode] = QPair<QString, QVariant>("ConfigTypeAppMode", QVariant(0));
        mConfigInfoData[ConfigTypeSfcModelPath] = QPair<QString, QVariant>(
            "ConfigTypeSfcModelPath", QVariant(QApplication::applicationDirPath() + QString("/../../../model")));
        mConfigInfoData[ConfigTypeNodeAddressPath] = QPair<QString, QVariant>(
            "ConfigTypeNodeAddressPath", QVariant(QApplication::applicationDirPath() + QString("/NodeAddress")));
        mConfigInfoData[ConfigTypeNewSheetRowCount] = QPair<QString, QVariant>("ConfigTypeNewSheetRowCount", QVariant(10));
        mConfigInfoData[ConfigTypeGenTypeList] = QPair<QString, QVariant>(
            "ConfigTypeGenTypeList", QVariant(QVariantList({"Default", "Negative/Positive", "Positive"})));
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>(
            "ConfigTypeSheetName", QVariant(QVariantList({"Description", "Privates", "Telltales", "Constants", "Events", "Sounds",
                                                          "Inters", "Outputs", "Configs"})));
#if defined(USE_SHEET_COLUMN_OLD)
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>(
            "ConfigTypeDescTitle", QVariant(QVariantList({"test", "version", "description", "Config_Signal", "Data"})));
        mConfigInfoData[ConfigTypeOtherTitle] = QPair<QString, QVariant>(
            "ConfigTypeOtherTitle", QVariant(QVariantList({"Check", "TCName", "GenType", "VehicleType", "Config", "Result",
                                                           "Case", "Input_Signal", "Input_Data", "Output_Signal", "isInitialize",
                                                           "Output_Value", "Config_Signal", "Data", "Negative Test"})));
#else
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>(
            // "ConfigTypeDescTitle", QVariant(QVariantList({"test", "version", "description"})));
            "ConfigTypeDescTitle",
            QVariant(QVariantList({"test", "version", "description", "Value_Count", "Config_Signal", "Data"})));
        mConfigInfoData[ConfigTypeOtherTitle] = QPair<QString, QVariant>(
            "ConfigTypeOtherTitle",
            QVariant(QVariantList({"Check", "TCName", "GenType", "VehicleType", "Config", "Result", "Case", "Input_Signal",
                                   "Input_Data", "Output_Signal", "isInitialize", "Output_Value"})));
#endif
        mConfigInfoData[ConfigTypeConfigTitle] = QPair<QString, QVariant>(
            "ConfigTypeConfigTitle", QVariant(QVariantList({"ConfigName", "AndGroup", "InputSignal", "InputData"})));
        mConfigInfoData[ConfigTypeDescConfigVisible] = QPair<QString, QVariant>("ConfigTypeDescConfigVisible", QVariant(false));
        mConfigInfoData[ConfitTypeGeneratePython] = QPair<QString, QVariant>("ConfitTypeGeneratePython", QVariant(false));

        // File
        mConfigInfoData[ConfigTypeLastSavedFilePath] = QPair<QString, QVariant>("ConfigTypeLastSavedFilePath", QVariant(""));
        mConfigInfoData[ConfigTypeDeleteFileTC] = QPair<QString, QVariant>("ConfigTypeDeleteFileTC", QVariant(true));
        mConfigInfoData[ConfigTypeSaveConvertExcel] = QPair<QString, QVariant>("ConfigTypeSaveConvertExcel", QVariant(true));

        mConfigInfoData[ConfigTypeExcelMergeStart] =
            QPair<QString, QVariant>("ConfigTypeExcelMergeStart", QVariant("[MergeStart]"));
        mConfigInfoData[ConfigTypeExcelMerge] = QPair<QString, QVariant>("ConfigTypeExcelMerge", QVariant("[Merge]"));
        mConfigInfoData[ConfigTypeExcelMergeEnd] = QPair<QString, QVariant>("ConfigTypeExcelMergeEnd", QVariant("[MergeEnd]"));
        mConfigInfoData[ConfigTypeTCFileSplitSize] = QPair<QString, QVariant>("ConfigTypeTCFileSplitSize", QVariant(100));
        mConfigInfoData[ConfigTypeGenTCParsingCycle] = QPair<QString, QVariant>("ConfigTypeGenTCParsingCycle", QVariant(10));

        // Vehicle
        mConfigInfoData[ConfigTypeVsmFileNameBaseCV] =
            QPair<QString, QVariant>("ConfigTypeVsmFileNameBaseCV", QVariant("CLU_VSM_CV_%1.Vehicle.%2.vsm"));
        mConfigInfoData[ConfigTypeVehicleTypeCV] =
            QPair<QString, QVariant>("ConfigTypeVehicleTypeCV", QVariant(QVariantList({"ICV", "EV", "FCEV"})));
        mConfigInfoData[ConfigTypeSfcSpecTypeCV] = QPair<QString, QVariant>(
            "ConfigTypeSfcSpecTypeCV", QVariant(QVariantList({"AV", "CD", "CV", "EC", "PT", "ETC", "extension"})));
        mConfigInfoData[ConfigTypeVsmSpecTypeCV] = QPair<QString, QVariant>("ConfigTypeVsmSpecTypeCV", QVariant("CV"));
        mConfigInfoData[ConfigTypeVsmFileNameBasePV] =
            QPair<QString, QVariant>("ConfigTypeVsmFileNameBasePV", QVariant("CLU_VSM_%1.Vehicle.%2.vsm"));
        mConfigInfoData[ConfigTypeVehicleTypePV] =
            QPair<QString, QVariant>("ConfigTypeVehicleTypePV", QVariant(QVariantList({"ICV", "EV", "FCEV", "PHEV", "HEV"})));
        mConfigInfoData[ConfigTypeSfcSpecTypePV] = QPair<QString, QVariant>(
            "ConfigTypeSfcSpecTypePV", QVariant(QVariantList({"AD", "AV", "CD", "CH", "EC", "HD", "PT", "ETC", "extension"})));
        mConfigInfoData[ConfigTypeVsmSpecTypePV] = QPair<QString, QVariant>(
            "ConfigTypeVsmSpecTypePV", QVariant(QVariantList({"AD", "AV", "CD", "CH", "EC", "HD", "PT", "CS"})));
        mConfigInfoData[ConfigTypeSystemTypePV] = QPair<QString, QVariant>(
            "ConfigTypeSystemTypePV",
            QVariant(QVariantList({"Config", "Engineering", "Extra", "Gateway", "HardWire", "Micom", "TP", "Undefined"})));

        // Preset
        mConfigInfoData[ConfigTypePreset001] = QPair<QString, QVariant>("ConfigTypePreset001", QVariant("d@'"));
        mConfigInfoData[ConfigTypePreset002] = QPair<QString, QVariant>("ConfigTypePreset002", QVariant("\0\n\0\0Z\0"));
        mConfigInfoData[ConfigTypePreset003] = QPair<QString, QVariant>("ConfigTypePreset003", QVariant("\0\0(\0F\0"));

        // ==========================================================================
        // Do not save config file : The setting information is not saved as a file.
        // ==========================================================================
        mConfigInfoData[ConfigTypeMaxDoNotSave] = QPair<QString, QVariant>("ConfigTypeMaxDoNotSave", QVariant());

        // Report
        mConfigInfoData[ConfigTypeReportResult] = QPair<QString, QVariant>("ConfigTypeReportResult", QVariant(false));
        mConfigInfoData[ConfigTypeReportResultSplit] = QPair<QString, QVariant>("ConfigTypeReportResultSplit", QVariant(false));
        mConfigInfoData[ConfigTypeReportResultConfig] = QPair<QString, QVariant>("ConfigTypeReportResultConfig", QVariant(false));
        mConfigInfoData[ConfigTypeReportResultExcel] = QPair<QString, QVariant>("ConfigTypeReportResultExcel", QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverage] = QPair<QString, QVariant>("ConfigTypeReportCoverage", QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageLine] = QPair<QString, QVariant>("ConfigTypeReportCoverageLine", QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageFunction] =
            QPair<QString, QVariant>("ConfigTypeReportCoverageFunction", QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageBranch] =
            QPair<QString, QVariant>("ConfigTypeReportCoverageBranch", QVariant(false));

        // Common : The setting information is not saved as a file. (Do not save config file)
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant(1));
        mConfigInfoData[ConfigTypeGraphicsMode] = QPair<QString, QVariant>("ConfigTypeGraphicsMode", QVariant(true));
        mConfigInfoData[ConfigTypeTCFilePath] = QPair<QString, QVariant>("ConfigTypeTCFilePath", QVariant(""));
        mConfigInfoData[ConfigTypeCLIModeTCCheck] = QPair<QString, QVariant>("ConfigTypeCLIModeTCCheck", QVariant(false));
        mConfigInfoData[ConfigTypeCLIModeGenType] = QPair<QString, QVariant>("ConfigTypeCLIModeGenType", QVariant(0));
        mConfigInfoData[ConfigTypeCheckLibOpenpyxl] = QPair<QString, QVariant>("ConfigTypeCheckLibOpenpyxl", QVariant(false));
        mConfigInfoData[ConfigTypeCheckLibPandas] = QPair<QString, QVariant>("ConfigTypeCheckLibPandas", QVariant(false));
        mConfigInfoData[ConfigTypeDoFileSave] = QPair<QString, QVariant>("ConfigTypeDoFileSave", QVariant(false));
        mConfigInfoData[ConfigTypeAllModule] = QPair<QString, QVariant>("ConfigTypeAllModule", QVariant(""));
        mConfigInfoData[ConfigTypeSelectModule] = QPair<QString, QVariant>("ConfigTypeSelectModule", QVariant(""));
        mConfigInfoData[ConfigTypeWindowTitle] = QPair<QString, QVariant>("ConfigTypeWindowTitle", QVariant(""));
        mConfigInfoData[ConfitTypeNewModule] = QPair<QString, QVariant>("ConfitTypeNewModule", QVariant("New_Module"));
        mConfigInfoData[ConfitTypeGenerateStart] = QPair<QString, QVariant>("ConfitTypeGenerateStart", QVariant(false));
    }

private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif  // CONFIG_INFO_H
