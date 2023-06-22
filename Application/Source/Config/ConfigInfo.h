#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QApplication>
#include <QVariant>
#include <QMap>


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

        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", false);

        QString defaultPath = QString("%1%2").arg(QApplication::applicationDirPath()).arg("/SFC");
        mConfigInfoData[ConfigTypeDefaultPath] = QPair<QString, QVariant>("ConfigTypeDefaultPath", defaultPath);


        QVariantList listValue = QVariantList();

        listValue = {"Privates", "Telltales", "Constants", "Events", "Sounds",  "Inters", "Outputs"};
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>("ConfigTypeSheetName", listValue);

        listValue = {"TCName", "VehicleType", "Result", "Case", "Input_Signal", "Input_Data", "Output_Signal",
                    "isInitialize", "Output_Value", "Config_Signal", "Data", "Negative", "Test"};
        mConfigInfoData[ConfigTypeContextName] = QPair<QString, QVariant>("ConfigTypeContextName", listValue);
    }


private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
