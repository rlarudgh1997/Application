#ifndef CONFIG_SETTING_H
#define CONFIG_SETTING_H

#include <QSharedPointer>
#include <QVariant>
#include <QSettings>
#include <QMap>
#include <QThread>
#include <QMutex>


#include "ConfigInfo.h"


class ConfigSetting : public QObject {
    Q_OBJECT

private:
    enum {
        ConfigDataTypeCurrent = 0,
        ConfigDataTypePrevious,
        ConfigDataTypeMax,
    } ConfigDataType;

public:
    static QSharedPointer<ConfigSetting> instance();
    ~ConfigSetting();
    QVariant readConfig(const int& configType);
    void writeConfig(const int& configType, const QVariant& configValue);
    void resetConfig();


private:
    explicit ConfigSetting();
    void init();
    void readConfig();
    void writeConfig();
    void threadFunc();


signals:
    void signalConfigChanged(const int& type, const QVariant& value);


private:
    QSettings *mSetting = nullptr;
    ConfigInfo mConfigInfo;
    QThread* mThread = new QThread();
    QMutex mMutex;
    QMap<int, QVariant> mConfigData = QMap<int, QVariant>();
    QMap<int, QVariant> mConfigTemp = QMap<int, QVariant>();
    bool mThreadRun = true;
    bool mThreadDataSave = false;
};

#endif    // CONFIG_SETTING_H
