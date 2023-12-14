#ifndef HANDLER_CONTENT_H
#define HANDLER_CONTENT_H

#include "AbstractHandler.h"

class HandlerContent : public AbstractHandler {
private:
    enum {
        HandlerContentTimerStart = AbstractHandler::AbstractTimerStart,
    };

public:
    static QSharedPointer<HandlerContent>& instance();

private:
    explicit HandlerContent();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_CONTENT_H
