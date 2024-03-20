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
        ConfigTypeAltonClient,
        ConfigTypeAltonClientPath,
        ConfigTypeTavPath,
        ConfigTypeVsmPath,
        ConfigTypeVsmFileBase,

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
        mConfigInfoData[ConfigTypeAltonClient] = QPair<QString, QVariant>("ConfigTypeAltonClient", QVariant("alton_client"));
        mConfigInfoData[ConfigTypeAltonClientPath] = QPair<QString, QVariant>("ConfigTypeAltonClientPath", QVariant("../Alton"));
        mConfigInfoData[ConfigTypeTavPath] = QPair<QString, QVariant>("ConfigTypeTavPath", QVariant("../TAV"));
        mConfigInfoData[ConfigTypeVsmPath] = QPair<QString, QVariant>("ConfigTypeVsmPath", QVariant("../../model/VSM"));
        mConfigInfoData[ConfigTypeVsmFileBase] =
            QPair<QString, QVariant>("ConfigTypeVsmFileBase", QVariant("CLU_VSM_%1.Vehicle.%2.vsm"));

        // Common : The setting information is not saved as a file. (Do not save config file)
        mConfigInfoData[ConfigTypeInit] = QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] = QPair<QString, QVariant>("ConfigTypeMode", QVariant(1));
    }

private:
    QMap<ConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif  // CONFIG_INFO_H
