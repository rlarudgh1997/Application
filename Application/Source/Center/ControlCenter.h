#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H

#include "AbstractControl.h"
#include "CommonUtil.h"

class ControlCenter : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlCenter>& instance();
    ~ControlCenter();

    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    explicit ControlCenter();

    void updateConfigInfo();
    void updateAllModuleList();
    bool checkNodeAddress(const QString& vsmPath, const QVariantList& vsmFile);
    int isVehicleType(const QString& file);
    QStringList isNodeAddressAll(const QString& vsmPath, const QVariantList& vsmFile);
    QStringList isNodeAddressMatchingModule(const QStringList& vsmList);
    void updateNodeAddress(const bool& check);
    void updateSelectModuleList(const bool& show);
    void updateSelectModueNodeAddress(const bool& update, const QVariantList& selectModule);
    void controlProcess(const int& type, const QString& command);
    void updateTerminalMode();

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
    QSharedPointer<QProcess> mProcess = nullptr;
};

#endif  // CONTROL_CENTER_H
