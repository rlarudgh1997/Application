#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QWidget>


class AbstractHandler;


class AbstractGui : public QWidget {
public:
    explicit AbstractGui(AbstractHandler* handler) : mHandler(handler) {}
    AbstractHandler* isHandler() {
        return mHandler;
    }

private:
    virtual bool createSignal(const int& type, const QVariant& value) = 0;
    virtual void drawDisplay(const QVariant& depth) = 0;
    virtual void drawDisplayDepth0() = 0;
    virtual void drawDisplayDepth1() = 0;
    virtual void drawDisplayDepth2() = 0;
    virtual void updateDisplay(const bool& first, const int& type = 0) = 0;

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value) = 0;

private:
    AbstractHandler* mHandler = nullptr;
};

#endif    // ABSTRACT_GUI_H
