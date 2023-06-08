#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "AbstractControl.h"



class ControlManager : public QObject {
    Q_OBJECT

public:
    static QSharedPointer<ControlManager> instance();
    void keyInput(const int& inputType, const int& inputValue);
    void mouseInput(const int& inputType, const int& inputValue);
    void createControl(const int& displayType);

private:
    explicit ControlManager();

private:
    QMap<int, AbstractControl*> mControlInfo = QMap<int, AbstractControl*>();
};


#endif    // CONTROL_MANAGER_H
