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
#define GROUP_NAME_PYTHON    "Python"


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
    if (configType < ConfigInfo::ConfigTypeMax) {
        return mConfigData[configType];
    } else {
        return QVariant();
    }
}

void ConfigSetting::writeConfig(const int& configType, const QVariant& configValue) {
    if (mConfigData[configType] != configValue) {
        mMutex.lock();
        mConfigData[configType] = configValue;
        mThreadDataSave = (configType > ConfigInfo::ConfigTypeStartSaveFile);
        mMutex.unlock();

        emit signalConfigChanged(configType, configValue);
    }
}

void ConfigSetting::readConfig() {
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                        ConfigInfo::ConfigGetTypeName).toString();

        if (configType >= ConfigInfo::ConfigTypeDefaultPath) {
            mSetting->beginGroup(GROUP_NAME_GENERAL);
        } else if (configType >= ConfigInfo::ConfitTypeLastFileInfo) {
            mSetting->beginGroup(GROUP_NAME_FILE);
        } else if (configType >= ConfigInfo::ConfigTypePythonRequiredLib1) {
            mSetting->beginGroup(GROUP_NAME_PYTHON);
        } else {
            mSetting->beginGroup(GROUP_NAME_COMMON);
        }

        QVariant configValue = mSetting->value(configName);
        mConfigData[configType] = configValue;
        mConfigBackup[configType] = mConfigData[configType];
        mSetting->endGroup();
    }
}

void ConfigSetting::writeConfig() {
    mMutex.lock();
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        if (mConfigData[configType] != mConfigBackup[configType]) {
            QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                            ConfigInfo::ConfigGetTypeName).toString();

            if (configType >= ConfigInfo::ConfigTypeDefaultPath) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType >= ConfigInfo::ConfitTypeLastFileInfo) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else if (configType >= ConfigInfo::ConfigTypePythonRequiredLib1) {
                mSetting->beginGroup(GROUP_NAME_PYTHON);
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
    // FILE* file = fopen(filepath.c_str(), "wb");
    // fsync(fileno(file));
}

void ConfigSetting::resetConfig() {
    for (int configType = 0; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QVariant configValue = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                            ConfigInfo::ConfigGetTypeValue);
        mMutex.lock();
        mConfigData[configType] = configValue;
        mMutex.unlock();
    }
    writeConfig();
    emit signalConfigChanged(ConfigInfo::ConfigTypeInit, true);
}

QVariant ConfigSetting::allConfig() {
    QMap<QString, QVariant> allConfigInfo;
    for (int configType = ConfigInfo::ConfigTypeStartSaveFile + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QString configName = mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType),
                                                        ConfigInfo::ConfigGetTypeName).toString();
        QVariant configValue = readConfig(configType);
        allConfigInfo[configName] = configValue;
        // qDebug() << "Config[" << configName << "] :" << allConfigInfo[configName];
    }
    return QVariant(allConfigInfo);
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
