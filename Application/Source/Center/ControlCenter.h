#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H

#include "AbstractControl.h"
#include "CommonUtil.h"

class ControlCenter : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlCenter>& instance();
    virtual void keyEvent(const int& inputType, const int& inputValue);
    virtual void resizeEvent(const int& width, const int& height);

private:
    explicit ControlCenter();

    void updateConfigInfo();
    void updateTestReport();
    bool checkNodeAddress(const QVariant& vsmPath, const QVariantList& vsmFile);
    QStringList isNodeAddressAll(const QVariant& vsmPath, const QVariantList& vsmFile);
    QStringList isNodeAddressMatchingModule(const QStringList& vsmList);
    void updateNodeAddress(const bool& check);
    void updateSelectModueList(const bool& show);
    void updateSelectModueNodeAddress(const bool& update, const QVariantList& selectModule);

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
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    virtual void slotEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);
    virtual void slotHandlerEvent(const int& type, const QVariant& value);

private:
    AbstractHandler* mHandler = nullptr;
    // QSharedPointer<ivis::common::ExcuteProgramThread> mProcess =
    //     QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(true), &QObject::deleteLater);
};

#endif  // CONTROL_CENTER_H
