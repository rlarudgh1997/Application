#include "GuiHandlerMain.h"
#include "CommonMacro.h"
#include "CommonEnum.h"

#define QML_NAME "qrc:/Home/Home.qml"
#define QML_DATA_MODEL_OBJECT_NAME "dataModelHome"

QSharedPointer<GuiHandlerMain> GuiHandlerMain::instance() {
    static QSharedPointer<GuiHandlerMain> gGuiHandler;

    if (gGuiHandler.isNull()) {
        gGuiHandler = QSharedPointer<GuiHandlerMain>(new GuiHandlerMain());
    }

    return gGuiHandler;
}

GuiHandlerMain::GuiHandlerMain()
    : AbstractQmlHandler(ScreenEnum::DisplayTypeHome, QML_NAME, QML_DATA_MODEL_OBJECT_NAME) {
}

void GuiHandlerMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(GuiHandlerMainTimerStart)) {
        // do nothing
    }
}

void GuiHandlerMain::initPropertyInfo() {
    registerProperty(PropertyTypeEnum::PropertyTypeDisplay,                   "display");
    registerProperty(PropertyTypeEnum::PropertyTypeMode,                      "mode");
    registerProperty(PropertyTypeEnum::PropertyTypeTheme,                     "theme");

    registerProperty(HandlerEnumHome::PropertyHomeDisplayType,                "homeDisplayType");
    registerProperty(HandlerEnumHome::PropertyHomeModelCPG,                   "modelCPG");
    registerProperty(HandlerEnumHome::PropertyHomeUsbConnect,                 "usbConnect");
    registerProperty(HandlerEnumHome::PropertyHomeBtConnect,                  "btConnect");
    registerProperty(HandlerEnumHome::PropertyHomeAudioMode,                  "audioMode");

    registerProperty(HandlerEnumHome::PropertyHomeMediaType,                  "mediaType");
    registerProperty(HandlerEnumHome::PropertyHomeMusicPlay,                  "musicPlay");
    registerProperty(HandlerEnumHome::PropertyHomeMusicPlayTime,              "musicPlayTime");
    registerProperty(HandlerEnumHome::PropertyHomeMusicTotalTime,             "musicTotalTime");
    registerProperty(HandlerEnumHome::PropertyHomeMusicTitle,                 "musicTitle");
    registerProperty(HandlerEnumHome::PropertyHomeMusicArtist,                "musicArtist");
    registerProperty(HandlerEnumHome::PropertyHomeMusicThumbnailPath,         "musicThumbnailPath");

    registerProperty(HandlerEnumHome::PropertyHomeRadioPower,                 "radioPower");
    registerProperty(HandlerEnumHome::PropertyHomeRadioBand,                  "radioBand");
    registerProperty(HandlerEnumHome::PropertyHomeRadioFrequency,             "radioFrequency");
    registerProperty(HandlerEnumHome::PropertyHomeRadioStationName,           "radioSationName");

    registerProperty(HandlerEnumHome::PropertyHomeDate,                       "date");
    registerProperty(HandlerEnumHome::PropertyHomeDateMonth,                  "dateMonth");
    registerProperty(HandlerEnumHome::PropertyHomeDateDay,                    "dateDay");
    registerProperty(HandlerEnumHome::PropertyHomeTime,                       "time");
    registerProperty(HandlerEnumHome::PropertyHomeAmPm,                       "amPm");
    registerProperty(HandlerEnumHome::PropertyHomeDayOfWeek,                  "dayOfWeek");

    registerProperty(HandlerEnumHome::PropertyHomeVehicleSpeed,               "vehicleSpeed");
    registerProperty(HandlerEnumHome::PropertyHomeEngineSpeed,                "engineSpeed");
    registerProperty(HandlerEnumHome::PropertyHomeFuelLevel,                  "fuelLevel");
    registerProperty(HandlerEnumHome::PropertyHomeShiftOilTemperature,        "shiftOilTemperature");
}



