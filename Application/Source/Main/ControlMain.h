#ifndef CONTROL_HOME_H
#define CONTROL_HOME_H


#include "AbstractControl.h"
#include <QProcess>


class ControlHome : public AbstractControl {
    Q_OBJECT

public:
    static QSharedPointer<ControlHome> instance();
    virtual void keyInput(const int& inputType, const int& inputValue);


private:
    explicit ControlHome();
    void controlProcess(const bool& start, const QString& processInfo = QString());


protected:
    virtual void initControl(const int& currentMode);
    virtual void controlConnect(const bool& state = true);
    virtual void initDataCommon(const int& currentMode, const int& displayType);
    virtual void initDataModule();
    virtual void resetControl(const bool& reset);
    virtual void timerFunc(const int& timerId);
    virtual void updateDataHandler(const int& propertyType, const QVariant& value);


public slots:
    virtual void slotTouchEvent(const int& propertyType, const int& touchType);
#if defined(USE_MODE_CHANGE_SIGNAL)
    virtual void slotModeChanged(const int& previousMode, const int& currentMode);
#endif
#if defined(USE_COMMON_UPDATE_NEW)
    virtual void slotUpdateData(const int& dataType, const QVariant& value);
#else
    virtual void slotCommonUpdateData(const int& sendType, const int& dataType, const QVariant& value);
#endif
    virtual void slotControlOtherController(const int& receiveMode, const int& dataType, const QVariant& value);
    virtual void slotConfigChanged(const int& type, const QVariant& value);
    void slotPlayingInfoChanged(const int& mediaType, const bool& play, const int& position, const int& playTime,
                                const int& totalTime, const QString& artist, const QString& title, const QString& thumbnail);
    void slotTractorVehicleDataChanged(const int& type, const QVariant& data);
    void slotProcessStandardOutput();
    void slotProcessFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus);


private:
    QProcess* mProcess = new QProcess(this);
    int mProcessID = 0;
};

#endif    // CONTROL_HOME_H
