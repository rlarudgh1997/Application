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
        ConfigTypeNewSheetRowCount,
        ConfigTypeSheetName,
        ConfigTypeDescTitle,
        ConfigTypeContentTitle,

        // File
        ConfitTypeLastFileInfo,
        ConfitTypeDeleteFileTC,

        ConfigTypeMax,

        // ==========================================================================
        // Do not save config file : The setting information is not saved as a file.
        // ==========================================================================
        ConfigTypeInit,
        ConfigTypeMode,
        ConfigTypeCheckLibOpenpyxl,
        ConfigTypeCheckLibPandas,

        ConfigTypeMaxDoNotSave,
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
        mConfigInfoData[ConfigTypeNewSheetRowCount] = QPair<QString, QVariant>("ConfigTypeNewSheetRowCount", QVariant(10));
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>("ConfigTypeSheetName",
                                                        QVariant(QVariantList({"Description", "Privates", "Telltales",
                                                                        "Constants", "Events", "Sounds",  "Inters", "Outputs"})));
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>("ConfigTypeDescTitle",
                                                        QVariant(QVariantList({"test", "version", "description", "Config_Signal",
                                                                        "Data"})));
        mConfigInfoData[ConfigTypeContentTitle] = QPair<QString, QVariant>("ConfigTypeContentTitle",
                                                        QVariant(QVariantList({"TCName", "VehicleType", "Result", "Case",
                                                                        "Input_Signal", "Input_Data", "Output_Signal",
                                                                        "isInitialize", "Output_Value", "Config_Signal",
                                                                        "Data", "Negative", "Test"})));


        // File
        mConfigInfoData[ConfitTypeLastFileInfo] = QPair<QString, QVariant>("ConfitTypeLastFileInfo", QVariant(""));
        mConfigInfoData[ConfitTypeDeleteFileTC] = QPair<QString, QVariant>("ConfitTypeDeleteFileTC", QVariant(false));




        // Common : The setting information is not saved as a file. (Do not save config file)
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant("Invalid"));
        mConfigInfoData[ConfigTypeCheckLibOpenpyxl] = QPair<QString, QVariant>("ConfigTypeCheckLibOpenpyxl", QVariant(false));
        mConfigInfoData[ConfigTypeCheckLibPandas] = QPair<QString, QVariant>("ConfigTypeCheckLibPandas", QVariant(false));
    }


private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
