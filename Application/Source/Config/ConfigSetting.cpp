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
#define GROUP_NAME_GENERAL "General"
#define GROUP_NAME_REPORT "Report"
#define GROUP_NAME_FILE "File"
#define GROUP_NAME_VEHICLE "Vehicle"

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

        if (configType == ConfigInfo::ConfigTypeWindowTitle) {
            emit signalUpdateWindowTitle(configValue.toString(), mConfigData[ConfigInfo::ConfigTypeAppMode].toInt());
        } else {
            emit signalConfigChanged(configType, configValue);
        }
    }

    if (configType == ConfigInfo::ConfigTypeInit) {
        emit signalUpdateWindowTitle(QString(), mConfigData[ConfigInfo::ConfigTypeAppMode].toInt());
        emit signalConfigReset(true);
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
    if (readConfig(configType) == configValue) {
        qDebug() << "Skip - Config value is the same value. :" << configValue;
        return;
    }
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
        case ConfigInfo::ConfigTypeOtherTitle:
        case ConfigInfo::ConfigTypeSfcSpecTypePV:
        case ConfigInfo::ConfigTypeVsmSpecTypePV: {
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
        if (configType >= ConfigInfo::ConfigTypeMaxDoNotSave) {
            mConfigData[configType] =
                mConfigInfo.getConfigInfo(static_cast<ConfigInfo::ConfigType>(configType), ConfigInfo::ConfigGetTypeValue);
        } else {
            if (configType < ConfigInfo::ConfigTypeLastSavedFilePath) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeVsmFileNameBaseCV) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else if (configType < ConfigInfo::ConfigTypeReportResult) {
                mSetting->beginGroup(GROUP_NAME_VEHICLE);
            } else if (configType < ConfigInfo::ConfigTypeMax) {
                mSetting->beginGroup(GROUP_NAME_REPORT);
            } else {
                mSetting->beginGroup(GROUP_NAME_COMMON);
            }
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
            if (configType < ConfigInfo::ConfigTypeLastSavedFilePath) {
                mSetting->beginGroup(GROUP_NAME_GENERAL);
            } else if (configType < ConfigInfo::ConfigTypeVsmFileNameBaseCV) {
                mSetting->beginGroup(GROUP_NAME_FILE);
            } else if (configType < ConfigInfo::ConfigTypeReportResult) {
                mSetting->beginGroup(GROUP_NAME_VEHICLE);
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
    mSetting->sync();
}

void ConfigSetting::resetConfig(const int& resetType) {
    int start = ConfigInfo::ConfigTypeInvalid + 1;
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
