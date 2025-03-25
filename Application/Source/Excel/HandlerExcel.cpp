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
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeValueEnum, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataValueEnum, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableICV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableFCEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTablePHEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableHEV, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableSystem, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputData, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeShortcutType, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenType, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTCCheck, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeKey, QVariant());
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo, QVariant());

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConstants, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetEvents, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetSounds, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetInters, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetOutputs, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs, QVariant(""));
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
