#ifndef HANDLER_CENTER_H
#define HANDLER_CENTER_H


#include "AbstractHandler.h"


class HandlerCenter : public AbstractHandler {
private:
    enum {
        HandlerCenterTimerStart = AbstractHandler::AbstractTimerStart,
    };


public:
    static QSharedPointer<HandlerCenter> instance();


private:
    explicit HandlerCenter();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif    // HANDLER_CENTER_H
