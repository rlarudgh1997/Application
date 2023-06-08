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
    void drawSubItem();


private:
    explicit HandlerMain();
     virtual void timerFunc(const int& timerId);
     virtual void initPropertyInfo();
};

#endif    // HANDLER_MAIN_H
