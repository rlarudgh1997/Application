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

#define GROUP_NAME_COMMON "Common"
#define GROUP_NAME_GENERAL "Gernal"
#define GROUP_NAME_REPORT "Report"
#define GROUP_NAME_FILE "File"

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

ConfigSetting::~ConfigSetting() {
    mThreadRun = false;
    delete mSetting;
    qDebug() << "~ConfigSetting";
}

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
        mMutex.lock();
        mConfigData[configType] = configValue;
        mThreadDataSave = (configType < ConfigInfo::ConfigTypeMaxDoNotSave);
        mMutex.unlock();

        emit signalConfigChanged(configType, configValue);
    }

    if (configType == ConfigInfo::ConfigTypeWindowTitle) {
        emit signalUpdateWindowTitle(configValue.toString());
    }
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
    QVariant editValue = QVariant();
    switch (configType) {
        case ConfigInfo::ConfigTypeNewSheetRowCount: {
            editValue = (configValue.toInt() >= 1000) ? (1000) : (configValue.toInt());
            break;
        }
        case ConfigInfo::ConfigTypeScreenInfo: {
            QStringList value = configValue.toString().split(", ");
            if (value.size() == 4) {
                QRect rect(value.at(0).toInt(), value.at(1).toInt(), value.at(2).toInt(), value.at(3).toInt());
                editValue = QVariant(rect);
            }
            break;
        }
        case ConfigInfo::ConfigTypeSheetName:
        case ConfigInfo::ConfigTypeDescTitle:
        case ConfigInfo::ConfigTypeOtherTitle: {
            QStringList value = configValue.toString().split(", ");
            QVariantList list = QVariantList();
            for (const auto& v : value) {
                list.append(v);
            }
            editValue = QVariant(list);
            break;
        }
        case ConfigInfo::ConfigTypeDeleteFileTC:
        case ConfigInfo::ConfigTypeReportResult:
        case ConfigInfo::ConfigTypeReportResultSplit:
        case ConfigInfo::ConfigTypeReportResultConfig:
        case ConfigInfo::ConfigTypeReportResultExcel:
        case ConfigInfo::ConfigTypeReportCoverage:
        case ConfigInfo::ConfigTypeReportCoverageLine:
        case ConfigInfo::ConfigTypeReportCoverageFunction:
        case ConfigInfo::ConfigTypeReportCoverageBranch: {
            editValue = configValue.toBool();
            break;
        }
        default: {
            editValue = configValue;
            break;
        }
    }
    writeConfig(configType, editValue);
}

void ConfigSetting::readConfig() {
    for (int configType = ConfigInfo::ConfigTypeInvalid + 1; configType < ConfigInfo::ConfigTypeMax; configType++) {
        QString configName =
            mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeName).toString();
        if (configType > ConfigInfo::ConfigTypeMaxDoNotSave) {
            mConfigData[configType] =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeValue);
        } else {
            if (configType < ConfigInfo::ConfigTypeLastSavedFilePath) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeReportResult) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_REPORT);
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
        if (configType > ConfigInfo::ConfigTypeMaxDoNotSave) {
            continue;
        }

        if (mConfigData[configType] != mConfigBackup[configType]) {
            QString configName =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeName)
                    .toString();
            if (configType < ConfigInfo::ConfigTypeLastSavedFilePath) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeReportResult) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_REPORT);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }
            mConfigBackup[configType] = mConfigData[configType];
            mSetting->setValue(configName, mConfigData[configType]);
            mSetting->endGroup();
        }
    }
    mMutex.unlock();
    mSetting->sync();
}

void ConfigSetting::resetConfig(const int& resetType) {
    int start = ConfigInfo::ConfigTypeInvalid;
    int end = ConfigInfo::ConfigTypeMax;
    int startSkip = (-1);
    int endSkip = (-1);

    if (resetType == ConfigResetTypeNormal) {
        startSkip = ConfigInfo::ConfigTypeReportResult;
        endSkip = ConfigInfo::ConfigTypeReportCoverageBranch;
    } else if (resetType == ConfigResetTypeReport) {
        start = ConfigInfo::ConfigTypeReportResult;
        end = ConfigInfo::ConfigTypeReportCoverageBranch + 1;
    } else {
    }

    for (int configType = start; configType < end; configType++) {
        if ((configType >= startSkip) && (configType <= endSkip)) {
            continue;
        }
        mMutex.lock();
        mConfigData[configType] =
            mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeValue);
        mMutex.unlock();
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
