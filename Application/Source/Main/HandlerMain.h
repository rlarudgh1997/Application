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
    void drawDisplayMain();

    virtual void controlConnect(const bool& state = true);
    virtual void timerFunc(const int& timerId);
    virtual void initPropertyInfo();


private:
};

#endif    // HANDLER_MAIN_H
