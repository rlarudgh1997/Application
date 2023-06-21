#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include <QVariant>
#include <QMap>

#include "CommonDefine.h"


#define DEFAULT_RADIO_FM 9430

class ConfigInfo {
public:
    typedef enum {
        // Common
        ConfigTypeInvalid = 0,
        ConfigTypeInit,

        // General
        ConfigTypeDefaultPath,
        ConfigTypeSheetName,
        ConfigTypeContextName,

        ConfigTypeMax,
    } eConfigType;

    typedef enum {
        ConfigGetTypeName = 0,
        ConfigGetTypeValue,
    } eConfigGetType;

public:
    ConfigInfo() {
        initConfigInfo();
    }

    QVariant getConfigInfo(const eConfigType& configType, const eConfigGetType& getType) const {
        return ((getType == ConfigGetTypeName) ? (QVariant(mConfigInfoData[configType].first))
                                                : (QVariant(mConfigInfoData[configType].second)));
    }

private:
    void initConfigInfo() {
        QVariantList value = QVariantList();

        mConfigInfoData.clear();

        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", false);

        mConfigInfoData[ConfigTypeDefaultPath] = QPair<QString, QVariant>("ConfigTypeDefaultPath", "/SFC");
        value = {"Privates", "Telltales", "Constants", "Events", "Sounds",  "Inters", "Outputs"};
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>("ConfigTypeSheetName", value);
        value = {"TCName", "VehicleType", "Result", "Case", "Input_Signal", "Input_Data", "Output_Signal",
                 "isInitialize", "Output_Value", "Config_Signal", "Data", "Negative", "Test"};
        mConfigInfoData[ConfigTypeContextName] = QPair<QString, QVariant>("ConfigTypeContextName", value);
    }

private:
    QMap<eConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
