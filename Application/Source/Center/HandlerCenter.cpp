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

HandlerCenter::HandlerCenter() : AbstractHandler(ivis::common::ScreenEnum::DisplayTypeCenter, QString("HandlerCenter")) {
#if defined(USE_GUI_MODULE)
    GuiCenter::instance(this);
#endif
}

void HandlerCenter::initPropertyInfo() {
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin, QVariant(QSize(0, SCREEN_HEIGHT_MARGIN)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible, QVariant(true));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth, QVariant(-1));

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressModule, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReportResultInfo, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReportCoverageInfo, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReportType, QVariant(0));
}

void HandlerCenter::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if defined(USE_SIGNAL_SLOT)
        connect(this, &AbstractHandler::signalPropertyChanged, GuiCenter::instance().data(), &GuiCenter::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &AbstractHandler::signalPropertyChanged,
                [=](const int& type, const QVariant& value) { GuiCenter::instance().data()->slotPropertyChanged(type, value); });
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
