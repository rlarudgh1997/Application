#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QWidget>


class AbstractHandler;


class AbstractGui : public QWidget {
public:
    explicit AbstractGui(AbstractHandler* handler) : mHandler(handler) {}

private:
    virtual AbstractHandler* isHandler() = 0;
    virtual QWidget* isItem(const int& type) = 0;
    virtual bool createSignal(const int& type, const QVariant& value) = 0;
    virtual void drawDisplay(const int& depth) = 0;

public:
    AbstractHandler* mHandler = nullptr;
};

#endif    // ABSTRACT_GUI_H
