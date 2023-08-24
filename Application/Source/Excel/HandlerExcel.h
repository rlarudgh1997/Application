#ifndef HANDLER_EXCEL_H
#define HANDLER_EXCEL_H


#include "AbstractHandler.h"


class HandlerExcel : public AbstractHandler {
private:
    enum {
        HandlerExcelTimerStart = AbstractHandler::AbstractTimerStart,
    };


public:
    static QSharedPointer<HandlerExcel>& instance();


private:
    explicit HandlerExcel();

    virtual void controlConnect(const bool& state = true);
    virtual void initPropertyInfo();
    virtual void timerFunc(const int& timerId);
};

#endif    // HANDLER_EXCEL_H
