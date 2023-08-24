#ifndef ABSTRACT_HANDLER_H
#define ABSTRACT_HANDLER_H

#include <QWidget>
#include <QSharedPointer>
#include <QTimerEvent>
#include <QMap>
#include <QVariant>


class AbstractHandler : public QObject {
    Q_OBJECT

public:
    enum {
        AbstractTimerTypeInvalid = 0,
        AbstractTimerStart,
    };
    enum {
        AbstractdrawDisplayDepth0 = 0,
        AbstractdrawDisplayDepth1,
        AbstractdrawDisplayDepth2,
    };


public:
    explicit AbstractHandler(const int& displayType, const QString& objcetName);
    bool init();
    QWidget* getScreen();
    int controlTimer(const int& timerType, const bool& start = false, const int& interval = 0);
    int getTimerId(const int& timerType);
    void registerProperty(const int& type, const QVariant& value);
    QVariant getProperty(const int& type);


private:
    Q_INVOKABLE void setProperty(const int& type, const QVariant& value, const bool& alwaysUpdate);

    virtual void controlConnect(const bool& state = true);
    virtual void timerFunc(const int& timerId) = 0;
    virtual void initPropertyInfo() = 0;


protected:
    void timerEvent(QTimerEvent *event);


signals:
    void signalUpdateDataModel(const int& type, const QVariant& value, const bool& alwaysUpdate);
    void signalPropertyChanged(const int& type, const QVariant& value);
    void signalHandlerEvent(const int& type, const QVariant& value);

private:
    QWidget* mScreen = nullptr;
    QMap<int, QVariant> mProperty = QMap<int, QVariant>();
    QMap<int, int> mTimer = QMap<int, int>();
    bool mUpdateState = false;
};

#endif    // ABSTRACT_QML_HANDLER_H
