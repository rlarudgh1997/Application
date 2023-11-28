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

    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun, QVariant(0));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo, QVariant(""));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText, QVariant(false));
    registerProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReport, QVariant(0));
}

void HandlerMenu::controlConnect(const bool& state) {
    if (state) {
#if defined(USE_GUI_MODULE)
#if 1
        connect(this, &HandlerMenu::signalPropertyChanged, GuiMenu::instance().data(), &GuiMenu::slotPropertyChanged,
                Qt::UniqueConnection);
#else
        connect(this, &HandlerMenu::signalPropertyChanged,
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
