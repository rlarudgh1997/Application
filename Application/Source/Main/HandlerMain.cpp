#include "HandlerMain.h"
#include "CommonEnum.h"

#if defined(USE_GUI_MODULE)
#include "GuiMain.h"
#endif


QSharedPointer<HandlerMain> HandlerMain::instance() {
    static QSharedPointer<HandlerMain> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerMain>(new HandlerMain());
    }
    return gHandler;
}

HandlerMain::HandlerMain() : AbstractHandler(ScreenEnum::DisplayTypeMain, QString("HandlerMain"), true) {
#if defined(USE_GUI_MODULE)
    GuiMain::instance(this);
#endif
}

void HandlerMain::initPropertyInfo() {
    registerProperty(PropertyTypeEnum::PropertyTypeDisplay,                   QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeMode,                      QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeDepth,                     QVariant(-1));
}

void HandlerMain::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                       &HandlerMain::signalPropertyChanged,
                GuiMain::instance().data(), &GuiMain::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerMain::signalPropertyChanged, [=](const int& dataType, const QVariant& value) {
            GuiMain::instance().data()->updateGuiProperty(dataType, value);
        });
#endif
#endif
    } else {
        disconnect(this);
    }
}

void HandlerMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerMainTimerStart)) {
        // do nothing
    }
}

