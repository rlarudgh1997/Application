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

HandlerTop::HandlerTop() : AbstractHandler(ScreenEnum::DisplayTypeTop, QString("HandlerTop"), true) {
#if defined(USE_GUI_MODULE)
    GuiTop::instance(this);
#endif
}

void HandlerTop::initPropertyInfo() {
    registerProperty(PropertyTypeEnum::PropertyTypeDisplay,                             QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeDisplaySize,                         QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeDisplaySizeMargin,                   QVariant(QSize(0, 0)));
    registerProperty(PropertyTypeEnum::PropertyTypeMode,                                QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeVisible,                             QVariant(false));
    registerProperty(PropertyTypeEnum::PropertyTypeDepth,                               QVariant(-1));


    registerProperty(PropertyTypeEnum::PropertyTypeAllConfigInfo,                       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDefaultPath,                         QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeSignalListAll,                       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeSignalListSFC,                       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeSignalListVSM,                       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeSignalListToMicom,                   QVariant(""));
}

void HandlerTop::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                      &HandlerTop::signalPropertyChanged,
                GuiTop::instance().data(), &GuiTop::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerTop::signalPropertyChanged, [=](const int& dataType, const QVariant& value) {
            GuiTop::instance().data()->updateGuiProperty(dataType, value);
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
