#include "HandlerMenu.h"
#include "CommonEnum.h"

#if defined(USE_GUI_MODULE)
#include "GuiMenu.h"
#endif

QSharedPointer<HandlerMenu>& HandlerMenu::instance() {
    static QSharedPointer<HandlerMenu> gHandler;
    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerMenu>(new HandlerMenu());
    }
    return gHandler;
}

HandlerMenu::HandlerMenu() : AbstractHandler(ivis::common::ScreenEnum::DisplayTypeMenu, QString("HandlerMenu")) {
#if defined(USE_GUI_MODULE)
    GuiMenu::instance(this);
#endif
}

void HandlerMenu::initPropertyInfo() {
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplay, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin, QVariant(QSize(0, 0)));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMode, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDepth, QVariant(-1));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo, QVariant(QRect()));

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppModeList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReport, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewTCFileList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewFileInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeMultiDockerCount, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunningInDocker, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestButtonVisible, QVariant(false));
}

void HandlerMenu::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if defined(USE_SIGNAL_SLOT)
        connect(this, &AbstractHandler::signalPropertyChanged, GuiMenu::instance().data(), &GuiMenu::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &AbstractHandler::signalPropertyChanged,
                [=](const int& type, const QVariant& value) { GuiMenu::instance().data()->slotPropertyChanged(type, value); });
#endif
#endif
    } else {
        disconnect(this);
    }
}

void HandlerMenu::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerMenuTimerStart)) {
        // do nothing
    }
}
