#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QApplication>
#include <QVariant>
#include <QMap>


class ConfigInfo {
public:
    typedef enum {
        // Common : The setting information is not saved as a file.
        ConfigTypeInvalid = 0,
        ConfigTypeInit,
        ConfigTypeMode,

        ConfigTypeStartSaveFile,

        // General
        ConfigTypeDefaultPath,
        ConfigTypeSheetName,
        ConfigTypeContentTitle,
        ConfigTypeDescTitle,
        ConfigTypeNewSheetRowCount,

        // Python
        ConfigTypePythonRequiredLib1,
        ConfigTypePythonRequiredLib2,
        ConfigTypePythonRequiredLib3,


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

        // Common : The setting information is not saved as a file.
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant("Invalid"));
        mConfigInfoData[ConfigTypeStartSaveFile] = QPair<QString, QVariant>("ConfigTypeStartSaveFile", QVariant("Invalid"));

        // General
        QString defaultPath = QString("%1%2").arg(QApplication::applicationDirPath()).arg("/SFC");
        mConfigInfoData[ConfigTypeDefaultPath] = QPair<QString, QVariant>("ConfigTypeDefaultPath", QVariant(defaultPath));

        QVariantList listValue = {"Description", "Privates", "Telltales", "Constants", "Events", "Sounds",  "Inters", "Outputs"};
        mConfigInfoData[ConfigTypeSheetName] = QPair<QString, QVariant>("ConfigTypeSheetName", QVariant(listValue));
        listValue = {"TCName", "VehicleType", "Result", "Case", "Input_Signal", "Input_Data", "Output_Signal",
                    "isInitialize", "Output_Value", "Config_Signal", "Data", "Negative", "Test"};
        mConfigInfoData[ConfigTypeContentTitle] = QPair<QString, QVariant>("ConfigTypeContentTitle", QVariant(listValue));
        listValue = {"test", "version", "description", "Config_Signal", "Data"};
        mConfigInfoData[ConfigTypeDescTitle] = QPair<QString, QVariant>("ConfigTypeDescTitle", QVariant(listValue));

        // Python
        mConfigInfoData[ConfigTypePythonRequiredLib1] = QPair<QString, QVariant>("ConfigTypePythonRequiredLib1", QVariant(false));
        mConfigInfoData[ConfigTypePythonRequiredLib2] = QPair<QString, QVariant>("ConfigTypePythonRequiredLib2", QVariant(false));
        mConfigInfoData[ConfigTypePythonRequiredLib3] = QPair<QString, QVariant>("ConfigTypePythonRequiredLib3", QVariant(false));
    }


private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
