#ifndef HANDLER_EVENT_H
#define HANDLER_EVENT_H

#include "AbstractHandler.h"

class HandlerEvent : public AbstractHandler {
private:
    enum { HandlerEventTimerStart = AbstractHandler::AbstractTimerStart, };

public:
    static QSharedPointer<HandlerEvent>& instance();

private:
    explicit HandlerEvent();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_EVENT_H
