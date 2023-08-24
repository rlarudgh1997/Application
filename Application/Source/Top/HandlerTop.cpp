#include "HandlerTop.h"
#include "CommonEnum.h"

#if defined(USE_GUI_MODULE)
#include "GuiTop.h"
#endif


QSharedPointer<HandlerTop>& HandlerTop::instance() {
    static QSharedPointer<HandlerTop> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerTop>(new HandlerTop());
    }
    return gHandler;
}

HandlerTop::HandlerTop() : AbstractHandler(ivis::common::ScreenEnum::DisplayTypeTop, QString("HandlerTop")) {
#if defined(USE_GUI_MODULE)
    GuiTop::instance(this);
#endif
}

void HandlerTop::initPropertyInfo() {
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay,                           QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize,                       QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin,                 QVariant(QSize(0, 0)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode,                              QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible,                           QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth,                             QVariant(-1));


    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath,                       QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeFileSaveType,                      QVariant(false));
}

void HandlerTop::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                      &HandlerTop::signalPropertyChanged,
                GuiTop::instance().data(), &GuiTop::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerTop::signalPropertyChanged, [=](const int& type, const QVariant& value) {
            GuiTop::instance().data()->slotPropertyChanged(type, value);
        });
#endif
#endif
    } else {
        disconnect(this);
    }
}

void HandlerTop::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerTopTimerStart)) {
        // do nothing
    }
}
