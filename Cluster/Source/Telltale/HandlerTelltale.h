#ifndef HANDLER_TELLTALE_H
#define HANDLER_TELLTALE_H

#include "AbstractHandler.h"

class HandlerTelltale : public AbstractHandler {
private:
    enum {
        HandlerTelltaleTimerStart = AbstractHandler::AbstractTimerStart,
    };

public:
    static QSharedPointer<HandlerTelltale>& instance();

private:
    explicit HandlerTelltale();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_TELLTALE_H
