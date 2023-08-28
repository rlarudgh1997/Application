#include "HandlerExcel.h"
#include "CommonEnum.h"

#if defined(USE_GUI_MODULE)
#include "GuiExcel.h"
#endif


QSharedPointer<HandlerExcel>& HandlerExcel::instance() {
    static QSharedPointer<HandlerExcel> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerExcel>(new HandlerExcel());
    }
    return gHandler;
}

HandlerExcel::HandlerExcel() : AbstractHandler(ivis::common::ScreenEnum::DisplayTypeExcel, QString("HandlerExcel")) {
#if defined(USE_GUI_MODULE)
    GuiExcel::instance(this);
#endif
}

void HandlerExcel::initPropertyInfo() {
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay,                           QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize,                       QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin,                 QVariant(QSize(0,
                                                                                                        SCREEN_MARGIN_HEIGHT)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode,                              QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible,                           QVariant(true));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth,                             QVariant(-1));


    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew,                QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen,               QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet,                   QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSplitText,                    QVariant(""));


    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription,             QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoPrivates,                QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoTelltales,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoConstants,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoEvents,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoSounds,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoInters,                  QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoOutputs,                 QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved1,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved2,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved3,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved4,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved5,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved6,               QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoReserved7,               QVariant(""));
}

void HandlerExcel::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this,                         &HandlerExcel::signalPropertyChanged,
                GuiExcel::instance().data(), &GuiExcel::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerExcel::signalPropertyChanged, [=](const int& type, const QVariant& value) {
            GuiExcel::instance().data()->slotPropertyChanged(type, value);
        });
#endif
#endif
    } else {
        disconnect(this);
    }
}

void HandlerExcel::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerExcelTimerStart)) {
        // do nothing
    }
}
