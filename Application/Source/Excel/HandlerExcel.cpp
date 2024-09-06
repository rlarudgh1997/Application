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
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin, QVariant(QSize(0, SCREEN_HEIGHT_MARGIN)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible, QVariant(true));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo, QVariant(QRect()));

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressPrivate, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressInter, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeValueEnum, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataValuEnum, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableICV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableFCEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTablePHEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableHEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableSystem, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeReadForStorage, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeClipboardType, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeShortcutType, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeKey, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeArrowKey, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeReadForDataGeneration, false);

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoPrivates, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoTelltales, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoConstants, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoEvents, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoSounds, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoInters, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoOutputs, QVariant(""));

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeReceiveKeyFocus, QVariant(false));
}

void HandlerExcel::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if defined(USE_SIGNAL_SLOT)
        connect(this, &AbstractHandler::signalPropertyChanged, GuiExcel::instance().data(), &GuiExcel::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &AbstractHandler::signalPropertyChanged,
                [=](const int& type, const QVariant& value) { GuiExcel::instance().data()->slotPropertyChanged(type, value); });
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
