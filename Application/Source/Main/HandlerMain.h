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

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif    // HANDLER_MAIN_H
