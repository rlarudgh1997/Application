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
    void exitProgram();

private:
    explicit ControlManager();


signals:
    void signalExitProgram();


private:
    QMap<int, AbstractControl*> mControlInfo = QMap<int, AbstractControl*>();
};


#endif    // CONTROL_MANAGER_H
