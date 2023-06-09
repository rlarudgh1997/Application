#ifndef HANDLER_MAIN_H
#define HANDLER_MAIN_H


#include "AbstractHandler.h"


class HandlerMain : public AbstractHandler {
private:
    enum {
        HandlerMainTimerStart = AbstractHandler::AbstractTimerStart,
    };


public:
    static QSharedPointer<HandlerMain> instance();


private:
    explicit HandlerMain();

    virtual void timerFunc(const int& timerId);
    virtual void initPropertyInfo();
    virtual void drawDisplayMain();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
};

#endif    // HANDLER_MAIN_H
