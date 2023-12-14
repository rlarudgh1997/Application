#ifndef HANDLER_HOME_H
#define HANDLER_HOME_H

#include "AbstractHandler.h"

class HandlerHome : public AbstractHandler {
private:
    enum {
        HandlerHomeTimerStart = AbstractHandler::AbstractTimerStart,
    };

public:
    static QSharedPointer<HandlerHome>& instance();

private:
    explicit HandlerHome();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_HOME_H
