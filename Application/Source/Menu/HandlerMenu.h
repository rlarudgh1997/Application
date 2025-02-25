#ifndef HANDLER_MENU_H
#define HANDLER_MENU_H

#include "AbstractHandler.h"

class HandlerMenu : public AbstractHandler {
private:
    enum {
        HandlerMenuTimerStart = AbstractHandler::AbstractTimerStart,
    };

public:
    static QSharedPointer<HandlerMenu>& instance();

private:
    explicit HandlerMenu();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif  // HANDLER_MENU_H
