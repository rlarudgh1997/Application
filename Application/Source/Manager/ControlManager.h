#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "AbstractControl.h"



class ControlManager : public QObject {
    Q_OBJECT

public:
    static QSharedPointer<ControlManager> instance();
    void keyEvent(const int& inputType, const int& inputValue);
    void mouseEvent(const int& inputType, const int& inputValue);
    void createControl(const int& displayType);
    int getCurrentMode();
    void exitProgram();
    void changeDisplay(const int& displayType);


private:
    explicit ControlManager();


signals:
    void signalExitProgram();
    void signalDisplayChange(const int& displayType);


private:
    QMap<int, AbstractControl*> mControlInfo = QMap<int, AbstractControl*>();
    int mCurrentMode = 0;
};


#endif    // CONTROL_MANAGER_H
