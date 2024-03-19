#ifndef CONFIG_SETTING_H
#define CONFIG_SETTING_H

#include "ConfigInfo.h"

#include <QSharedPointer>
#include <QVariant>
#include <QSettings>
#include <QMap>
#include <QThread>
#include <QMutex>

// #include <QLoggingCategory>
// Q_DECLARE_LOGGING_CATEGORY(CONFIG)

class ConfigSetting : public QObject {
    Q_OBJECT

public:
    enum {
        ConfigResetTypeAll = 0,
        ConfigResetTypeNormal,
        ConfigResetTypeReport,
    };

private:
    enum {
        ConfigDataTypeCurrent = 0,
        ConfigDataTypePrevious,
        ConfigDataTypeMax,
    };

public:
    static QSharedPointer<ConfigSetting>& instance();
    // ~ConfigSetting();
    QVariant readConfig(const int& configType);
    void writeConfig(const int& configType, const QVariant& configValue);
    QVariant isConfigName(const int& configType);
    void editConfig(const int& configType, const QVariant& configValue);
    void resetConfig(const int& resetType = ConfigResetTypeAll);

private:
    explicit ConfigSetting();
    void init();
    void readConfig();
    void writeConfig();
    void threadFunc();

signals:
    void signalConfigChanged(const int& type, const QVariant& value);

private:
    QSettings* mSetting = nullptr;
    ConfigInfo mConfigInfo;
    QThread* mThread = new QThread();
    QMutex mMutex;
    QMap<int, QVariant> mConfigData = QMap<int, QVariant>();
    QMap<int, QVariant> mConfigBackup = QMap<int, QVariant>();
    bool mThreadRun = true;
    bool mThreadDataSave = false;
};

#endif  // CONFIG_SETTING_H
