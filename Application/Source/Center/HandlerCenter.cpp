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

HandlerCenter::HandlerCenter() : AbstractHandler(ivis::common::ScreenEnum::DisplayTypeTop, QString("HandlerCenter"), true) {
#if defined(USE_GUI_MODULE)
    GuiCenter::instance(this);
#endif
}

void HandlerCenter::initPropertyInfo() {
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay,                              QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize,                          QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin,                    QVariant(QSize(0, 120)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode,                                 QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible,                              QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth,                                QVariant(-1));


    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew,                   QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen,                  QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditCell,                       QVariant(false));


    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription,                QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoPrivates,                   QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoTelltales,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoConstants,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoEvents,                     QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoSounds,                     QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoInters,                     QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoOutputs,                    QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved1,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved2,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved3,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved4,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved5,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved6,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved7,                  QVariant(""));
}

void HandlerCenter::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                         &HandlerCenter::signalPropertyChanged,
                GuiCenter::instance().data(), &GuiCenter::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerCenter::signalPropertyChanged, [=](const int& type, const QVariant& value) {
            GuiCenter::instance().data()->slotPropertyChanged(type, value);
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
