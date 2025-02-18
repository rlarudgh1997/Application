#ifndef CONFIG_SETTING_H
#define CONFIG_SETTING_H

#include "ConfigInfo.h"

#include <QSharedPointer>
#include <QVariant>
#include <QSettings>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

// #define USE_CONFIG_SETTING_NEW

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
    ~ConfigSetting();

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
    void runThread();
#if defined(USE_CONFIG_SETTING_NEW)
    void controlThread(QThread* thread, QWaitCondition& waitCondition, QMutex& mutex, const int& type);
#endif

signals:
    void signalConfigReset(const bool& resetAll);
    void signalConfigChanged(const int& type, const QVariant& value);
    void signalUpdateWindowTitle(const QString& title, const int& appMode);

private:
    QSettings* mSetting = nullptr;
    ConfigInfo mConfigInfo;
#if defined(USE_CONFIG_SETTING_NEW)
    QScopedPointer<QThread> mThread;
    QWaitCondition mWaitCondition;
#else
    QThread* mThread = new QThread();
    bool mThreadDataSave = false;
#endif
    QMutex mMutex;
    QMap<int, QVariant> mConfigData = QMap<int, QVariant>();
    QMap<int, QVariant> mConfigBackup = QMap<int, QVariant>();
};

#endif  // CONFIG_SETTING_H
