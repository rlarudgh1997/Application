#include "ConfigSetting.h"


#include <QApplication>
#include <QDir>
#include <QFile>


#include "CommonDefine.h"

#define CONFIG_PATH QApplication::applicationDirPath().toLatin1().data()
#define CONFIG_NAME "Application.ini"
#define CONFIG_FILE (QString("%1/%2").arg(CONFIG_PATH).arg(CONFIG_NAME))

#define GROUP_NAME_GENERAL "Gernal"
#define GROUP_NAME_HOME "Home"
#define GROUP_NAME_RADIO "Radio"
#define GROUP_NAME_MEDIA "Media"
#define GROUP_NAME_PHONE "Phone"
#define GROUP_NAME_CAMERA "Camera"
#define GROUP_NAME_TRACTOR "Tractor"
#define GROUP_NAME_SETTING "Setting"




QSharedPointer<ConfigSetting> ConfigSetting::instance() {
    static QSharedPointer<ConfigSetting> gConfigSetting;

    if (gConfigSetting.isNull()) {
        gConfigSetting = QSharedPointer<ConfigSetting>(new ConfigSetting());
    }

    return gConfigSetting.constCast<ConfigSetting>();
}


ConfigSetting::ConfigSetting()
    : mSetting(new QSettings(CONFIG_FILE, QSettings::IniFormat)) {
    init();
}

ConfigSetting::~ConfigSetting() {
    mThreadRun = false;
    delete mSetting;
}

void ConfigSetting::init() {
    QFile configSettingFile(CONFIG_FILE);
    bool fileExists = configSettingFile.exists();

    // qDebug() << "\t ConfigInfo=" << fileExists << ", " << configSettingFile.fileName().toLatin1().data();

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
    QVariant value = QVariant(0);

    if (configType < ConfigInfo::ConfigTypeMax) {
        value = mConfigData[configType];
    }

    return value;
}

void ConfigSetting::writeConfig(const int& configType, const QVariant& configValue) {
    if (mConfigData[configType] != configValue) {
        mMutex.lock();
        mConfigData[configType] = configValue;
        mThreadDataSave = true;
        mMutex.unlock();

        emit signalConfigChanged(configType, configValue);
    } else {
        if ((configType == ConfigInfo::ConfigTypeSettingVolume) || (configType == ConfigInfo::ConfigTypeSettingVolumeCalling)) {
            emit signalConfigChanged(configType, configValue);
        }
    }
}

void ConfigSetting::readConfig() {
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::eConfigType>(configType),
                                                        ConfigInfo::ConfigGetTypeName).toString();

        if (configType >= ConfigInfo::ConfigTypeSetting) {
            mSetting->beginGroup(GROUP_NAME_SETTING);
        } else if (configType >= ConfigInfo::ConfigTypeTractor) {
            mSetting->beginGroup(GROUP_NAME_TRACTOR);
        } else if (configType >= ConfigInfo::ConfigTypeCamera) {
            mSetting->beginGroup(GROUP_NAME_CAMERA);
        } else if (configType >= ConfigInfo::ConfigTypePhone) {
            mSetting->beginGroup(GROUP_NAME_PHONE);
        } else if (configType >= ConfigInfo::ConfigTypeMedia) {
            mSetting->beginGroup(GROUP_NAME_MEDIA);
        } else if (configType >= ConfigInfo::ConfigTypeRadio) {
            mSetting->beginGroup(GROUP_NAME_RADIO);
        } else if (configType >= ConfigInfo::ConfigTypeHome) {
            mSetting->beginGroup(GROUP_NAME_HOME);
        } else {
            mSetting->beginGroup(GROUP_NAME_GENERAL);
        }

        QVariant configValue = mSetting->value(configName);
        mConfigData[configType] = configValue;
        mConfigTemp[configType] = mConfigData[configType];
        mSetting->endGroup();
    }
}

void ConfigSetting::writeConfig() {
    mMutex.lock();
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (mConfigData[configType] != mConfigTemp[configType]) {
            QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::eConfigType>(configType),
                                                            ConfigInfo::ConfigGetTypeName).toString();

            if (configType >= ConfigInfo::ConfigTypeSetting) {
                mSetting->beginGroup(GROUP_NAME_SETTING);
            } else if (configType >= ConfigInfo::ConfigTypeTractor) {
                mSetting->beginGroup(GROUP_NAME_TRACTOR);
            } else if (configType >= ConfigInfo::ConfigTypeCamera) {
                mSetting->beginGroup(GROUP_NAME_CAMERA);
            } else if (configType >= ConfigInfo::ConfigTypePhone) {
                mSetting->beginGroup(GROUP_NAME_PHONE);
            } else if (configType >= ConfigInfo::ConfigTypeMedia) {
                mSetting->beginGroup(GROUP_NAME_MEDIA);
            } else if (configType >= ConfigInfo::ConfigTypeRadio) {
                mSetting->beginGroup(GROUP_NAME_RADIO);
            } else if (configType >= ConfigInfo::ConfigTypeHome) {
                mSetting->beginGroup(GROUP_NAME_HOME);
            } else {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            }

            QVariant configValue = mConfigData[configType];
            mConfigTemp[configType] = configValue;
            mSetting->setValue(configName, configValue);
            mSetting->endGroup();
        }
    }
    mMutex.unlock();
    mSetting->sync();
    // FILE* file = fopen(filepath.c_str(), "wb");
    // fsync(fileno(file));
}

void ConfigSetting::resetConfig() {
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if ((configType != ConfigInfo::ConfigTypeUsbConnect)) {
            QVariant configValue = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::eConfigType>(configType),
                                                                ConfigInfo::ConfigGetTypeValue);
            mMutex.lock();
            mConfigData[configType] = configValue;
            mMutex.unlock();
        }
    }
    writeConfig();
    emit signalConfigChanged(ConfigInfo::ConfigTypeInit, true);
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



