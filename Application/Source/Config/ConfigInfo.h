#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QApplication>
#include <QVariant>
#include <QMap>
#include <QRect>

#define SCREEN_POSITION_X 50
#define SCREEN_POSITION_Y 100
#define SCREEN_SIZE_WIDTH 1440
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
        ConfigTypeSheetName,
        ConfigTypeDescTitle,
        ConfigTypeOtherTitle,

        // File
        ConfigTypeLastSavedFilePath,
        ConfigTypeDeleteFileTC,
        ConfigTypeExcelMergeTextStart,
        ConfigTypeExcelMergeText,
        ConfigTypeExcelMergeTextEnd,

        // Vehicle
        ConfigTypeVsmFileNameBaseCV,
        ConfigTypeVehicleTypeCV,
        ConfigTypeSfcSpecTypeCV,
        ConfigTypeVsmSpecTypeCV,
        ConfigTypeVsmFileNameBasePV,
        ConfigTypeVehicleTypePV,
        ConfigTypeSfcSpecTypePV,
        ConfigTypeVsmSpecTypePV,

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
        ConfigTypeCheckLibOpenpyxl,
        ConfigTypeCheckLibPandas,
        ConfigTypeDoFileSave,
        ConfigTypeAllModule,
        ConfigTypeSelectModule,
        ConfigTypeWindowTitle,

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
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>(
            "ConfigTypeSheetName", QVariant(QVariantList({"Description", "Privates", "Telltales", "Constants", "Events", "Sounds",
                                                          "Inters", "Outputs"})));
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>(
            "ConfigTypeDescTitle", QVariant(QVariantList({"test", "version", "description", "Config_Signal", "Data"})));
        mConfigInfoData[ConfigTypeOtherTitle] = QPair<QString, QVariant>(
            "ConfigTypeOtherTitle",
            QVariant(QVariantList({"TCName", "VehicleType", "Result", "Case", "Input_Signal", "Input_Data", "Output_Signal",
                                   "isInitialize", "Output_Value", "Config_Signal", "Data", "Negative Test"})));

        // File
        mConfigInfoData[ConfigTypeLastSavedFilePath] = QPair<QString, QVariant>("ConfigTypeLastSavedFilePath", QVariant(""));
        mConfigInfoData[ConfigTypeDeleteFileTC] = QPair<QString, QVariant>("ConfigTypeDeleteFileTC", QVariant(true));
        mConfigInfoData[ConfigTypeExcelMergeTextStart] =
            QPair<QString, QVariant>("ConfigTypeExcelMergeTextStart", QVariant("ExcelMergeTextStart"));
        mConfigInfoData[ConfigTypeExcelMergeText] =
            QPair<QString, QVariant>("ConfigTypeExcelMergeText", QVariant("ExcelMergeText"));
        mConfigInfoData[ConfigTypeExcelMergeTextEnd] =
            QPair<QString, QVariant>("ConfigTypeExcelMergeTextEnd", QVariant("ExcelMergeTextEnd"));

        // Vehicle
        mConfigInfoData[ConfigTypeVsmFileNameBaseCV] =
            QPair<QString, QVariant>("ConfigTypeVsmFileNameBaseCV", QVariant("CLU_VSM_CV_%1.Vehicle.%2.vsm"));
        mConfigInfoData[ConfigTypeVehicleTypeCV] =
            QPair<QString, QVariant>("ConfigTypeVehicleTypeCV", QVariant(QVariantList({"ICV", "EV", "FCEV"})));
        mConfigInfoData[ConfigTypeSfcSpecTypeCV] =
            QPair<QString, QVariant>("ConfigTypeSfcSpecTypeCV", QVariant(QVariantList({"AV", "CD", "CV", "EC", "PT"})));
        mConfigInfoData[ConfigTypeVsmSpecTypeCV] = QPair<QString, QVariant>("ConfigTypeVsmSpecTypeCV", QVariant("CV"));
        mConfigInfoData[ConfigTypeVsmFileNameBasePV] =
            QPair<QString, QVariant>("ConfigTypeVsmFileNameBasePV", QVariant("CLU_VSM_%1.Vehicle.%2.vsm"));
        mConfigInfoData[ConfigTypeVehicleTypePV] =
            QPair<QString, QVariant>("ConfigTypeVehicleTypePV", QVariant(QVariantList({"ICV", "EV", "FCEV", "PHEV", "HEV"})));
        mConfigInfoData[ConfigTypeSfcSpecTypePV] = QPair<QString, QVariant>(
            "ConfigTypeSfcSpecTypePV", QVariant(QVariantList({"AD", "AV", "CD", "CH", "EC", "HD", "PT", "ETC"})));
        mConfigInfoData[ConfigTypeVsmSpecTypePV] = QPair<QString, QVariant>(
            "ConfigTypeVsmSpecTypePV", QVariant(QVariantList({"AD", "AV", "CD", "CH", "EC", "HD", "PT", "CS"})));

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
        mConfigInfoData[ConfigTypeCheckLibOpenpyxl] = QPair<QString, QVariant>("ConfigTypeCheckLibOpenpyxl", QVariant(false));
        mConfigInfoData[ConfigTypeCheckLibPandas] = QPair<QString, QVariant>("ConfigTypeCheckLibPandas", QVariant(false));
        mConfigInfoData[ConfigTypeDoFileSave] = QPair<QString, QVariant>("ConfigTypeDoFileSave", QVariant(false));
        mConfigInfoData[ConfigTypeAllModule] = QPair<QString, QVariant>("ConfigTypeAllModule", QVariant(""));
        mConfigInfoData[ConfigTypeSelectModule] = QPair<QString, QVariant>("ConfigTypeSelectModule", QVariant(""));
        mConfigInfoData[ConfigTypeWindowTitle] = QPair<QString, QVariant>("ConfigTypeWindowTitle", QVariant(""));
    }

private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif  // CONFIG_INFO_H
