#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include "CommonDefine.h"

#include <QGuiApplication>
#include <QVariant>
#include <QMap>
#include <QRect>

class ConfigInfo {
public:
    typedef enum {
        ConfigTypeInvalid = 0,

        // General
        ConfigTypeScreenInfo,
        ConfigTypeAltonClientPath,

        // ==========================================================================
        // Do not save config file : The setting information is not saved as a file.
        // ==========================================================================
        ConfigTypeMaxDoNotSave,
        ConfigTypeInit,
        ConfigTypeMode,

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
        mConfigInfoData[ConfigTypeScreenInfo] =
            QPair<QString, QVariant>("ConfigTypeScreenInfo", QVariant(QRect(0, 0, 1440, 900)));
        mConfigInfoData[ConfigTypeAltonClientPath] =
            QPair<QString, QVariant>("ConfigTypeAltonClientPath", QVariant("./alton_client"));

        // Common : The setting information is not saved as a file. (Do not save config file)
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant(1));
    }

private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif  // CONFIG_INFO_H
