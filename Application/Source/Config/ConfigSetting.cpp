#include "ConfigSetting.h"

#include "CommonDefine.h"

#include <QApplication>
#include <QDir>
#include <QFile>

#include <QDebug>

// Q_LOGGING_CATEGORY(CONFIG, "ConfigSetting")


#define CONFIG_PATH QApplication::applicationDirPath().toLatin1().data()
#define CONFIG_NAME "Application.ini"
#define CONFIG_FILE (QString("%1/%2").arg(CONFIG_PATH).arg(CONFIG_NAME))

#define GROUP_NAME_COMMON    "Common"
#define GROUP_NAME_GENERAL   "Gernal"
#define GROUP_NAME_FILE      "File"


QSharedPointer<ConfigSetting>& ConfigSetting::instance() {
    static QSharedPointer<ConfigSetting> gConfigSetting;
    if (gConfigSetting.isNull()) {
        gConfigSetting = QSharedPointer<ConfigSetting>(new ConfigSetting());
    }
    return gConfigSetting;
}


ConfigSetting::ConfigSetting()
    : mSetting(new QSettings(CONFIG_FILE, QSettings::IniFormat)) {
    init();
}

ConfigSetting::~ConfigSetting() {
    mThreadRun = false;
    delete mSetting;
    qDebug() << "~ConfigSetting";
}

void ConfigSetting::init() {
    QFile configSettingFile(CONFIG_FILE);
    bool fileExists = configSettingFile.exists();

    // qDebug() << "ConfigInfo=" << fileExists << ", " << configSettingFile.fileName().toLatin1().data();

    if ((fileExists == false) && (configSettingFile.open(QIODevice::WriteOnly|QIODevice::Text))) {
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
    if ((configType == ConfigInfo::ConfigTypeInvalid) || (configType == ConfigInfo::ConfigTypeMaxDoNotSave)
        ||(configType == ConfigInfo::ConfigTypeMax)) {
        return QVariant();
    } else {
        return mConfigData[configType];
    }
}

void ConfigSetting::writeConfig(const int& configType, const QVariant& configValue) {
    if (mConfigData[configType] != configValue) {
        mMutex.lock();
        mConfigData[configType] = configValue;
        mThreadDataSave = (configType < ConfigInfo::ConfigTypeMax);
        mMutex.unlock();

        emit signalConfigChanged(configType, configValue);
    }
}

void ConfigSetting::readConfig() {
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                        ConfigInfo::ConfigGetTypeName).toString();
        if (configType > ConfigInfo::ConfigTypeMaxDoNotSave) {
            mConfigData[configType] = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                                ConfigInfo::ConfigGetTypeValue);
        } else {
            if (configType < ConfigInfo::ConfigTypeLastFileInfo) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }
            mConfigData[configType] = mSetting->value(configName);
            mSetting->endGroup();
        }
    }
    mConfigBackup = mConfigData;
}

void ConfigSetting::writeConfig() {
    mMutex.lock();
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (mConfigData[configType] != mConfigBackup[configType]) {
            QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                            ConfigInfo::ConfigGetTypeName).toString();

            if (configType < ConfigInfo::ConfigTypeLastFileInfo) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }

            QVariant configValue = mConfigData[configType];
            mConfigBackup[configType] = configValue;
            mSetting->setValue(configName, configValue);
            mSetting->endGroup();
        }
    }
    mMutex.unlock();
    mSetting->sync();
}

void ConfigSetting::resetConfig() {
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        mMutex.lock();
        mConfigData[configType] = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                            ConfigInfo::ConfigGetTypeValue);
        mMutex.unlock();
    }
    writeConfig();
    emit signalConfigChanged(ConfigInfo::ConfigTypeInit, true);
}

QMap<int, QPair<QString, QVariant>> ConfigSetting::allConfig() {
    QMap<int, QPair<QString, QVariant>> allConfig = QMap<int, QPair<QString, QVariant>>();
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (configType == ConfigInfo::ConfigTypeMaxDoNotSave) {
            continue;
        }
        QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                        ConfigInfo::ConfigGetTypeName).toString();
        allConfig[configType] = QPair<QString, QVariant>(configName, readConfig(configType));
    }
    // qDebug() << "allConfig :" << allConfig;
    return allConfig;
}

void ConfigSetting::threadFunc() {
    while (mThreadRun) {
        if (mThreadDataSave) {
            writeConfig();
            mMutex.lock();
            mThreadDataSave = false;
            mMutex.unlock();
        }
        QThread::msleep(100);
    }
    QThread::currentThread()->quit();
    QThread::currentThread()->wait();
}
