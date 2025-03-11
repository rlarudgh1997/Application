#ifndef CONTROL_MENU_H
#define CONTROL_MENU_H

#include "AbstractControl.h"
#include "CommonUtil.h"

// #include <QLoggingCategory>
// Q_DECLARE_LOGGING_CATEGORY(C_TOP)

class ControlMenu : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlMenu>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    ControlMenu();

    void updateSelectAppMode();
    void updateAllModuleList(const QString& filter);
    void updateSelectModuleList(const int& eventType, const QVariantList& selectModule = QVariantList());
    void updateTestReportInfo(const int& eventType);
    bool updateTestResultInfo(const int& testReultType, const int& totalCount, const QStringList& infoData = QStringList());
    void updateViewTCFile();
    void updateViewTCDisplay(const QString& moduleName);
    void updateViewLogFile();
    void updateViewLogDisplay(const QString& titleName);
    void startWatcherFile(const int& type, const QString& watcherFile, const int& totalCount);
    void startProcess(const QString& command, const QString& arg, const int& totalCount);
    bool excuteScript(const int& runType, const bool& state, const QVariantList& infoList);
    void stopWatcherFile(const int& type);
    void stopProcess();
    void cancelScript(const bool& script, const bool& watcher);
    int saveTestReportInfo(const int& reportType, const QList<bool>& value);

protected:
    virtual AbstractHandler* isHandler();
    virtual bool initControl();
    virtual void controlConnect(const bool& state = true);
    virtual void initCommonData(const int& currentMode, const int& displayType);
    virtual void initNormalData();
    virtual void initControlData();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataControl(const int& type, const QVariant& value);
    virtual void updateDataHandler(const int& type, const QVariant& value, const bool& alwaysUpdate = false);
    virtual void sendEventInfo(const int& destination, const int& eventType, const QVariant& eventValue = QVariant());

public slots:
    virtual void slotControlUpdate(const int& type, const QVariant& value = QVariant());
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);

private:
    AbstractHandler* mHandler = nullptr;
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess = nullptr;
    QSharedPointer<ivis::common::FileSystemWatcherThread> mWatcherRunScript = nullptr;
    QSharedPointer<ivis::common::FileSystemWatcherThread> mWatcherTestResult = nullptr;
};

#endif  // CONTROL_MENU_H
