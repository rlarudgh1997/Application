#ifndef HANDLER_GAUGE_H
#define HANDLER_GAUGE_H

#include "AbstractHandler.h"

class HandlerGauge : public AbstractHandler {
private:
    enum {
        HandlerTelltaleTimerStart = AbstractHandler::AbstractTimerStart,
    };

public:
    static QSharedPointer<HandlerGauge>& instance();

private:
    explicit HandlerGauge();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_GAUGE_H
