#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QWidget>
#include <QSharedPointer>
#include <QMap>
#include <QDebug>


class AbstractHandler;


class AbstractGui : public QWidget {
public:
    explicit AbstractGui(AbstractHandler* handler) : mHandler(handler) {}
    AbstractHandler* isHandler() {
        return mHandler;
    }
    virtual void initItem() = 0;
    virtual QWidget* isItem(const int& type) = 0;
    virtual bool createSignal(const int& type, const QVariant& value) = 0;
    // virtual void drawDisplay(const int& depth) = 0;
    virtual void drawDisplayDepth0() = 0;
    virtual void drawDisplayDepth1() = 0;
    virtual void drawDisplayDepth2() = 0;

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value) = 0;

public:
    AbstractHandler* mHandler = nullptr;
};

#endif    // ABSTRACT_GUI_H
