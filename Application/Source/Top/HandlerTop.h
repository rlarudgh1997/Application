#ifndef HANDLER_TOP_H
#define HANDLER_TOP_H


#include "AbstractHandler.h"


class HandlerTop : public AbstractHandler {
private:
    enum {
        HandlerTopTimerStart = AbstractHandler::AbstractTimerStart,
    };


public:
    static QSharedPointer<HandlerTop>& instance();


private:
    explicit HandlerTop();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif    // HANDLER_TOP_H
