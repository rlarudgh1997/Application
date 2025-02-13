#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <QObject>

#include "AbstractControl.h"
#include "CommonDefine.h"
#include "CommonUtil.h"

class ControlManager : public QObject {
    Q_OBJECT

#if defined(USE_RESIZE_SIGNAL)
    REGISTER_WRITABLE_PROPERTY(QSize, ScreenSize, QSize(), true)
#else
    REGISTER_WRITABLE_VALUE(QSize, ScreenSize, QSize())
#endif
    REGISTER_WRITABLE_VALUE(int, PreviousMode, 0)
    REGISTER_WRITABLE_VALUE(int, CurrentMode, 0)

public:
    static QSharedPointer<ControlManager>& instance();
    void init();
    void sendEventInfo(const int& source, const int& destination, const int& eventType, const QVariant& eventValue);
    void raise(const int& displayType);
    void keyEvent(const int& inputType, QKeyEvent* keyEvent);
    void mouseEvent(const int& inputType, const int& inputValue);
    void resizeEvent(QResizeEvent& resizeEvent);
    void exitProgram(const bool& check);

private:
    explicit ControlManager();
    void createControl(const int& displayType);

signals:
    void signalExitProgram();
    void signalEventInfoChanged(const int& displayType, const int& eventType, const QVariant& eventValue);

private:
    QMap<int, AbstractControl*> mControlInfo = QMap<int, AbstractControl*>();
};

#endif  // CONTROL_MANAGER_H
