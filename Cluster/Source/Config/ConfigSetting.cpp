#include "ConfigSetting.h"

#include "CommonDefine.h"

#include <QGuiApplication>
#include <QDir>
#include <QFile>

#include <QDebug>

// Q_LOGGING_CATEGORY(CONFIG, "ConfigSetting")
// #define USE_MULTI_GROUP_NAME

#define CONFIG_PATH QGuiApplication::applicationDirPath().toLatin1().data()
#define CONFIG_NAME "Cluster.ini"
#define CONFIG_FILE (QString("%1/%2").arg(CONFIG_PATH).arg(CONFIG_NAME))

#define GROUP_NAME_COMMON "Common"
#if defined(USE_MULTI_GROUP_NAME)
#define GROUP_NAME_GENERAL "Gernal"
#define GROUP_NAME_VEHICLE "Etc"
#endif

QSharedPointer<ConfigSetting>& ConfigSetting::instance() {
    static QSharedPointer<ConfigSetting> gConfigSetting;
    if (gConfigSetting.isNull()) {
        gConfigSetting = QSharedPointer<ConfigSetting>(new ConfigSetting());
    }
    return gConfigSetting;
}

ConfigSetting::ConfigSetting() : mSetting(new QSettings(CONFIG_FILE, QSettings::IniFormat)) {
    init();
}

// ConfigSetting::~ConfigSetting() {
//     mThreadRun = false;
//     delete mSetting;
//     qDebug() << "~ConfigSetting";
// }

void ConfigSetting::init() {
    QFile configSettingFile(CONFIG_FILE);
    bool fileExists = configSettingFile.exists();

    if ((fileExists == false) && (configSettingFile.open(QIODevice::WriteOnly | QIODevice::Text))) {
        resetConfig();
        configSettingFile.close();
    }

    readConfig();

    // Thread Init
    this->moveToThread(mThread);
    connect(mThread, &QThread::finished, this, &QObject::deleteLater);
    connect(mThread, &QThread::started, this, &ConfigSetting::threadFunc);
    mThread->start();
}

QVariant ConfigSetting::readConfig(const int& configType) {
    if ((configType == ConfigInfo::ConfigTypeInvalid) || (configType == ConfigInfo::ConfigTypeMaxDoNotSave) ||
        (configType == ConfigInfo::ConfigTypeMax)) {
        return QVariant();
    } else {
        return mConfigData[configType];
    }
}

void ConfigSetting::writeConfig(const int& configType, const QVariant& configValue) {
    if (mConfigData[configType] != configValue) {
        QMutexLocker lock(&mMutex);
        mConfigData[configType] = configValue;
        if (mThreadDataSave == false) {
            mThreadDataSave = (configType < ConfigInfo::ConfigTypeMaxDoNotSave);
        }

        emit signalConfigChanged(configType, configValue);
    }

    // if (configType == ConfigInfo::ConfigTypeInit) {
    //     emit signalUpdateWindowTitle(QString(), mConfigData[ConfigInfo::ConfigTypeAppMode].toInt());
    //     emit signalConfigReset(true);
    // }
}

QVariant ConfigSetting::isConfigName(const int& configType) {
    if ((configType == ConfigInfo::ConfigTypeInvalid) || (configType == ConfigInfo::ConfigTypeMaxDoNotSave) ||
        (configType == ConfigInfo::ConfigTypeMax)) {
        return QVariant();
    } else {
        QString configName =
            mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeName).toString();
        QStringList temp = configName.split("ConfigType");
        if (temp.size() == 2) {
            return temp.at(1);
        } else {
            return QVariant();
        }
    }
}

void ConfigSetting::editConfig(const int& configType, const QVariant& configValue) {
    // QVariant editValue = QVariant();
    // writeConfig(configType, editValue);
}

void ConfigSetting::readConfig() {
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (configType >= ConfigInfo::ConfigTypeMaxDoNotSave) {
            mConfigData[configType] =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeValue);
        } else {
#if defined(USE_MULTI_GROUP_NAME)
            if (configType < ConfigInfo::ConfigTypeMaxDoNotSave) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_VEHICLE);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }
#else
            mSetting->beginGroup(GROUP_NAME_COMMON);
#endif
            QVariant configName =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeName);
            mConfigData[configType] = mSetting->value(configName.toString());
            mSetting->endGroup();
        }
    }
    mConfigBackup = mConfigData;
}

void ConfigSetting::writeConfig() {
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (configType >= ConfigInfo::ConfigTypeMaxDoNotSave) {
            continue;
        }

        if (mConfigData[configType] != mConfigBackup[configType]) {
            QString configName =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeName)
                    .toString();
#if defined(USE_MULTI_GROUP_NAME)
            if (configType < ConfigInfo::ConfigTypeMaxDoNotSave) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_VEHICLE);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }
#else
            mSetting->beginGroup(GROUP_NAME_COMMON);
#endif

            qDebug() << "\t WriteConfig[" << configType << "] :" << mConfigBackup[configType] << "->" << mConfigData[configType];
            mConfigBackup[configType] = mConfigData[configType];
            mSetting->setValue(configName, mConfigData[configType]);
            mSetting->endGroup();
        }
    }
    mSetting->sync();
}

void ConfigSetting::resetConfig(const int& resetType) {
    int start = ConfigInfo::ConfigTypeInvalid + 1;
    int end = ConfigInfo::ConfigTypeMax;
    int startSkip = (-1);
    int endSkip = (-1);

    for (int configType = start; configType < end; configType++) {
        if ((configType >= startSkip) && (configType <= endSkip)) {
            continue;
        }
        mConfigData[configType] =
            mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeValue);
    }
    writeConfig();
    emit signalConfigChanged(ConfigInfo::ConfigTypeInit, true);
}

void ConfigSetting::threadFunc() {
    while (mThreadRun) {
        if (mThreadDataSave) {
            QMutexLocker lock(&mMutex);
            writeConfig();
            mThreadDataSave = false;
        }
        QThread::msleep(100);
    }
    QThread::currentThread()->quit();
    QThread::currentThread()->wait();
}
