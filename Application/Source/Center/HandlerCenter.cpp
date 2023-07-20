#include "HandlerCenter.h"
#include "CommonEnum.h"

#if defined(USE_GUI_MODULE)
#include "GuiCenter.h"
#endif


QSharedPointer<HandlerCenter>& HandlerCenter::instance() {
    static QSharedPointer<HandlerCenter> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerCenter>(new HandlerCenter());
    }
    return gHandler;
}

HandlerCenter::HandlerCenter() : AbstractHandler(ScreenEnum::DisplayTypeTop, QString("HandlerCenter"), true) {
#if defined(USE_GUI_MODULE)
    GuiCenter::instance(this);
#endif
}

void HandlerCenter::initPropertyInfo() {
    registerProperty(PropertyTypeEnum::PropertyTypeDisplay,                QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeDisplaySize,            QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeMode,                   QVariant(-1));
    registerProperty(PropertyTypeEnum::PropertyTypeVisible,                QVariant(false));
    registerProperty(PropertyTypeEnum::PropertyTypeDepth,                  QVariant(-1));


    registerProperty(PropertyTypeEnum::PropertyTypeDefaultPath,            QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeSheetName,              QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeContentTitle,           QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDescTitle,              QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeUpdateSheetInfo,        QVariant(""));


    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoDescription,  QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoPrivates,     QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoTelltales,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoConstants,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoEvents,       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoSounds,       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoInters,       QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoOutputs,      QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved1,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved2,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved3,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved4,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved5,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved6,    QVariant(""));
    registerProperty(PropertyTypeEnum::PropertyTypeDetailInfoReserved7,    QVariant(""));
}

void HandlerCenter::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                         &HandlerCenter::signalPropertyChanged,
                GuiCenter::instance().data(), &GuiCenter::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerCenter::signalPropertyChanged, [=](const int& dataType, const QVariant& value) {
            GuiCenter::instance().data()->updateGuiProperty(dataType, value);
        });
#endif
#endif
    } else {
        disconnect(this);
    }
}

void HandlerCenter::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerCenterTimerStart)) {
        // do nothing
    }
}
