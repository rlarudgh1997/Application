#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QWidget>
#include "AbstractHandler.h"
#include "CommonEnum.h"


class AbstractGui : public QWidget {
public:
    explicit AbstractGui(AbstractHandler* handler) : mHandler(handler) {}
    AbstractHandler* isHandler() {
        return mHandler;
    }
    bool createSignal(const int& type, const QVariant& value) {
        if (isHandler()) {
            emit isHandler()->signalHandlerEvent(type, value);
            return true;
        }
        qDebug("Fail to create signal - handler is nullptr");
        return false;
    }
    void drawDisplay(const QVariant& depth) {
        if (depth == QVariant(ivis::common::ScreenEnum::DisplayDepthDepth0)) {
            drawDisplayDepth0();
        } else if (depth == QVariant(ivis::common::ScreenEnum::DisplayDepthDepth1)) {
            drawDisplayDepth1();
        } else if (depth == QVariant(ivis::common::ScreenEnum::DisplayDepthDepth2)) {
            drawDisplayDepth2();
        } else {
            // nothing to do
        }
    }

private:
    virtual void drawDisplayDepth0() = 0;
    virtual void drawDisplayDepth1() = 0;
    virtual void drawDisplayDepth2() = 0;
    virtual void updateDisplaySize() = 0;
    virtual void updateDisplayVisible() = 0;

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value) = 0;

private:
    AbstractHandler* mHandler = nullptr;
};

#endif    // ABSTRACT_GUI_H
