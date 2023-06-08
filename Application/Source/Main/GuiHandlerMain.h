#ifndef GUI_HANDLER_MAIN_H
#define GUI_HANDLER_MAIN_H


class GuiHandlerMain {
private:
    enum {
        QmlHandlerHomeTimerStart = AbstractQmlHandler::AbstractTimerStart,
    };


public:
    static QSharedPointer<GuiHandlerMain> instance();


private:
    explicit GuiHandlerMain();
    // virtual void timerFunc(const int& timerId);
    // virtual void initPropertyInfo();
};

#endif    // GUI_HANDLER_MAIN_H
