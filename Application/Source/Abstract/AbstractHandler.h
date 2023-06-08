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
        AbstractTimerStart ,
    };


public:
    AbstractHandler(const int& displayType, const QString& objcetName, const bool& show);
    bool init();
    QWidget* getComponent();
    qint32 controlTimer(const qint32& timerType, const bool& start = false, const qint32& interval = 0);
    qint32 getTimerId(const qint32& timerType);
    void registerProperty(const qint32& propertyType, const QVariant& value);
    QVariant getProperty(const qint32& propertyType);
    Q_INVOKABLE void setProperty(const qint32& propertyType, const QVariant& value);


private:
    void controlConnect(const bool& state = true);

    virtual void timerFunc(const int& timerId) = 0;
    virtual void initPropertyInfo() = 0;


protected:
    void timerEvent(QTimerEvent *event);


signals:
    // [Control -> Handler -> QML(dataModel)]
    void signalUpdateDataModel(const int& dataType, const QVariant& value);
    // [Handler -> Control]
    void signalHandlerEvent(const int& propertyType, const int& touchType);

private slots:
    void slotUpdateDataModel(const int& propertyType, const QVariant& value);


private:
    QWidget* mWidget = nullptr;
    QMap<qint32, QVariant> mProperty = QMap<qint32, QVariant>();
    QMap<qint32, qint32> mTimer = QMap<qint32, qint32>();
    bool mUpdateState = false;
};

#endif    // ABSTRACT_QML_HANDLER_H
