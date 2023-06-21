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
        AbstractdrawDisplayMain = 0,
        AbstractdrawDisplayDepth1,
        AbstractdrawDisplayDepth2,
    };


public:
    AbstractHandler(const int& displayType, const QString& objcetName, const bool& show);
    bool init();
    QWidget* getScreen();
    int controlTimer(const int& timerType, const bool& start = false, const int& interval = 0);
    int getTimerId(const int& timerType);
    void registerProperty(const int& type, const QVariant& value);
    QVariant getProperty(const int& type);
    Q_INVOKABLE void setProperty(const int& type, const QVariant& value);


private:
    virtual void controlConnect(const bool& state = true);
    virtual void timerFunc(const int& timerId) = 0;
    virtual void initPropertyInfo() = 0;


protected:
    void timerEvent(QTimerEvent *event);


signals:
    void signalUpdateDataModel(const int& type, const QVariant& value);
    void signalPropertyChanged(const int& type, const QVariant& value);
    void signalHandlerEvent(const int& type, const QVariant& value);

private:
    QWidget* mScreen = nullptr;
    QMap<int, QVariant> mProperty = QMap<int, QVariant>();
    QMap<int, int> mTimer = QMap<int, int>();
    bool mUpdateState = false;
};

#endif    // ABSTRACT_QML_HANDLER_H
