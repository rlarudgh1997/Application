#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "AbstractControl.h"



class ControlManager : public QObject {
    Q_OBJECT

public:
    static QSharedPointer<ControlManager> instance();
    void init();
    void sendEventInfo(const int& eventType, const QVariant& eventValue);
    void keyEvent(const int& inputType, const int& inputValue);
    void mouseEvent(const int& inputType, const int& inputValue);
    void exitProgram();

private:
    explicit ControlManager();
    int isDisplay(const int& eventType);
    void createControl(const int& displayType);
    void setCurrentMode(const int& displayType);
    int getCurrentMode();


signals:
    void signalExitProgram();
    void signalEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);


private:
    QMap<int, AbstractControl*> mControlInfo = QMap<int, AbstractControl*>();
    int mCurrentMode = 0;
};


#endif    // CONTROL_MANAGER_H
