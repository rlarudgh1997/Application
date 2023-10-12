#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QApplication>
#include <QVariant>
#include <QMap>
#include <QRect>


#define SCREEN_POSITION_X                50
#define SCREEN_POSITION_Y                100
#define SCREEN_SIZE_WIDTH                1440
#define SCREEN_SIZE_HEIGHT               900


class ConfigInfo {
public:
    typedef enum {
        ConfigTypeInvalid = 0,

        // General
        ConfigTypeScreenInfo,
        ConfigTypeDefaultPath,
        ConfigTypeNodeAddressPath,
        ConfigTypeVsmPath,
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

        // Report
        ConfigTypeReportResult,
        ConfigTypeReportResultSplit,
        ConfigTypeReportResultConfig,
        ConfigTypeReportResultExcel,
        ConfigTypeReportCoverage,
        ConfigTypeReportCoverageLine,
        ConfigTypeReportCoverageFunction,
        ConfigTypeReportCoverageBranch,


        // ==========================================================================
        // Do not save config file : The setting information is not saved as a file.
        // ==========================================================================
        ConfigTypeMaxDoNotSave,
        ConfigTypeInit,
        ConfigTypeMode,
        ConfigTypeCheckLibOpenpyxl,
        ConfigTypeCheckLibPandas,
        ConfigTypeDoFileSave,
        ConfigTypeSelectModule,

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
        mConfigInfoData[ConfigTypeScreenInfo] = QPair<QString, QVariant>("ConfigTypeScreenInfo",
                                                        QVariant(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y,
                                                                        SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT)));
        mConfigInfoData[ConfigTypeDefaultPath] = QPair<QString, QVariant>("ConfigTypeDefaultPath",
                                                        QVariant(QApplication::applicationDirPath() + QString("/SFC")));
        mConfigInfoData[ConfigTypeNodeAddressPath] = QPair<QString, QVariant>("ConfigTypeNodeAddressPath",
                                                        QVariant(QApplication::applicationDirPath() + QString("/NodeAddress")));
        mConfigInfoData[ConfigTypeVsmPath] = QPair<QString, QVariant>("ConfigTypeVsmPath",
                                                        QVariant(QApplication::applicationDirPath()
                                                                                            + QString("/../SFC/model/VSM")));
        mConfigInfoData[ConfigTypeNewSheetRowCount] = QPair<QString, QVariant>("ConfigTypeNewSheetRowCount", QVariant(10));
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>("ConfigTypeSheetName",
                                                        QVariant(QVariantList({"Description", "Privates", "Telltales",
                                                                        "Constants", "Events", "Sounds",  "Inters", "Outputs"})));
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>("ConfigTypeDescTitle",
                                                        QVariant(QVariantList({"test", "version", "description", "Config_Signal",
                                                                        "Data"})));
        mConfigInfoData[ConfigTypeOtherTitle] = QPair<QString, QVariant>("ConfigTypeOtherTitle",
                                                        QVariant(QVariantList({"TCName", "VehicleType", "Result", "Case",
                                                                        "Input_Signal", "Input_Data", "Output_Signal",
                                                                        "isInitialize", "Output_Value", "Config_Signal",
                                                                        "Data", "Negative Test"})));



        // File
        mConfigInfoData[ConfigTypeLastSavedFilePath] = QPair<QString, QVariant>("ConfigTypeLastSavedFilePath", QVariant(""));
        mConfigInfoData[ConfigTypeDeleteFileTC] = QPair<QString, QVariant>("ConfigTypeDeleteFileTC", QVariant(true));
        mConfigInfoData[ConfigTypeExcelMergeTextStart] = QPair<QString, QVariant>("ConfigTypeExcelMergeTextStart",
                                                        QVariant("ExcelMergeTextStart"));
        mConfigInfoData[ConfigTypeExcelMergeText] = QPair<QString, QVariant>("ConfigTypeExcelMergeText",
                                                        QVariant("ExcelMergeText"));
        mConfigInfoData[ConfigTypeExcelMergeTextEnd] = QPair<QString, QVariant>("ConfigTypeExcelMergeTextEnd",
                                                        QVariant("ExcelMergeTextEnd"));



        // Report
        mConfigInfoData[ConfigTypeReportResult] = QPair<QString, QVariant>("ConfigTypeReportResult",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportResultSplit] = QPair<QString, QVariant>("ConfigTypeReportResultSplit",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportResultConfig] = QPair<QString, QVariant>("ConfigTypeReportResultConfig",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportResultExcel] = QPair<QString, QVariant>("ConfigTypeReportResultExcel",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverage] = QPair<QString, QVariant>("ConfigTypeReportCoverage",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageLine] = QPair<QString, QVariant>("ConfigTypeReportCoverageLine",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageFunction] = QPair<QString, QVariant>("ConfigTypeReportCoverageFunction",
                                                                                    QVariant(false));
        mConfigInfoData[ConfigTypeReportCoverageBranch] = QPair<QString, QVariant>("ConfigTypeReportCoverageBranch",
                                                                                    QVariant(false));



        // Common : The setting information is not saved as a file. (Do not save config file)
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant(1));
        mConfigInfoData[ConfigTypeCheckLibOpenpyxl] = QPair<QString, QVariant>("ConfigTypeCheckLibOpenpyxl", QVariant(false));
        mConfigInfoData[ConfigTypeCheckLibPandas] = QPair<QString, QVariant>("ConfigTypeCheckLibPandas", QVariant(false));
        mConfigInfoData[ConfigTypeDoFileSave] = QPair<QString, QVariant>("ConfigTypeDoFileSave", QVariant(false));
    }


private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
