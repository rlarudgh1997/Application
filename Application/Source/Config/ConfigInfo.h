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
        mConfigInfoData.clear();

        mConfigInfoData[ConfigTypeSheetName] =
                QPair<QString, QVariant>("ConfigTypeSheetName", QVariant(false));
        mConfigInfoData[ConfigTypeContextName] =
                QPair<QString, QVariant>("ConfigTypeContextName", QVariant(false));
    }

private:
    QMap<eConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
