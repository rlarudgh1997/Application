#include "ControlHome.h"
#include "QmlHandlerHome.h"
#include "CommonMacro.h"
#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "CommonFileCtrl.h"
#include "ControllerManager.h"
#include "ControlPopup.h"
#include "ControlMedia.h"
#if defined(USE_HOME_PLAYING_INFO_NEW)
#include "QmlHandlerMedia.h"
#endif

QSharedPointer<ControlHome> ControlHome::instance() {
    static QSharedPointer<ControlHome> gControl;

    if (gControl.isNull()) {
        gControl = QSharedPointer<ControlHome>(new ControlHome());
    }

    return gControl;
}

ControlHome::ControlHome() {
}

void ControlHome::initControl(const int& currentMode) {
    if (isInitComplete() == false) {
        QmlHandlerHome::instance().data()->init();
        controlConnect(true);

        initDataCommon(currentMode, ScreenEnum::DisplayTypeHome);
        initDataModule();
    }
}

void ControlHome::initDataCommon(const int& currentMode, const int& displayType) {
    updateDataHandler(PropertyTypeEnum::PropertyTypeDisplay, displayType);
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);

#if defined(USE_INIT_THEME)
    int theme = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSettingThemeType).toInt();
    updateDataHandler(PropertyTypeEnum::PropertyTypeTheme,
                (theme == HandlerEnumSetup::ThemeTypeDark) ? (THEME_DARK) : (THEME_NORMAL));
#endif
}

void ControlHome::initDataModule() {
    updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType, HandlerEnumCommon::DisplayTypeMenu);
#if defined(USE_QML_DATA_MODEL_OBJECT_NAME_CPG)
    updateDataHandler(HandlerEnumHome::PropertyHomeModelCPG, true);
#else
    updateDataHandler(HandlerEnumHome::PropertyHomeModelCPG, false);
#endif

#if defined(USE_BOOTING_AUDIO_RADIO)
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAudioMode, HandlerEnumCommon::AudioModeRadio);
#else
    CommonFileCtrl checkFile;
    if (checkFile.bootCheckUsbConnect()) {
        int audioMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAudioMode).toInt();
        if (audioMode == HandlerEnumCommon::AudioModeUsbMusic) {
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAudioMode,
                                                            HandlerEnumCommon::AudioModeUsbMusic);
        } else if (audioMode == HandlerEnumCommon::AudioModeUsbVideo) {
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAudioMode,
                                                            HandlerEnumCommon::AudioModeUsbVideo);
        } else {
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAudioMode,
                                                            HandlerEnumCommon::AudioModeRadio);
        }
    } else {
        ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeAudioMode, HandlerEnumCommon::AudioModeRadio);
    }
#endif
    updateDataHandler(HandlerEnumHome::PropertyHomeAudioMode,
                ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAudioMode));

#if !defined(USE_BOOTING_STEP)
    // Init Class - Top. Radio
    ControllerManager::instance().data()->slotTouchEvent(HandlerEnumCommon::PropertyStartDateTime, 0);
    ControllerManager::instance().data()->slotTouchEvent(HandlerEnumCommon::PropertyInitControl, 0);
#endif

    resetControl(false);
}

void ControlHome::resetControl(const bool& reset) {
    Q_UNUSED(reset)
}

void ControlHome::controlConnect(const bool& state) {
    if (state) {
        connect(QmlHandlerHome::instance().data(), &QmlHandlerHome::signalTouchEvent,
                this, &ControlHome::slotTouchEvent,
                Qt::UniqueConnection);
#if defined(USE_HOME_PLAYING_INFO_NEW)
        connect(QmlHandlerMedia::instance().data()->getQmlInstance(),
                SIGNAL(signalPlayingInfoChanged(int, bool, int, int, int, QString, QString, QString)),
                this,
                SLOT(slotPlayingInfoChanged(int, bool, int, int, int, QString, QString, QString)),
                Qt::UniqueConnection);
#endif
#if defined(USE_MODE_CHANGE_SIGNAL)
        connect(ControllerManager::instance().data(), &ControllerManager::signalModeChanged,
                this, &ControlHome::slotModeChanged);
#endif
#if defined(USE_COMMON_UPDATE_NEW)
        connect(ControllerManager::instance().data(), &ControllerManager::signalUpdateDataHome,
                this, &ControlHome::slotUpdateData);
#else
        connect(ControllerManager::instance().data(), &ControllerManager::signalCommonUpdateData,
                this, &ControlHome::slotCommonUpdateData);
#endif
        connect(ControllerManager::instance().data(), &ControllerManager::signalControlOtherController,
                this, &ControlHome::slotControlOtherController);
        connect(ControllerManager::tractorInstance().data(), &Tractor::signalTractorVehicleDataChanged,
                this, &ControlHome::slotTractorVehicleDataChanged);
#if defined(TRACK_AUTODRIVE_GUI_OUTPUT_START_STOP)
        connect(mProcess, &QProcess::readyReadStandardOutput,
                this, &ControlHome::slotProcessStandardOutput);
#endif
        connect(mProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &ControlHome::slotProcessFinished);
    } else {
        disconnect(QmlHandlerHome::instance().data());
        disconnect(ControllerManager::instance().data());
        disconnect(ControllerManager::tractorInstance().data());
        disconnect(mProcess);
    }
}

void ControlHome::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
}

void ControlHome::keyInput(const int& inputType, const int& inputValue) {
    Q_UNUSED(inputType)
    Q_UNUSED(inputValue)
}

void ControlHome::updateDataHandler(const int& propertyType, const QVariant& value) {
    if (setData(propertyType, value)) {
        emit QmlHandlerHome::instance().data()->signalUpdateDataModel(propertyType, value);
    }
}

void ControlHome::controlProcess(const bool& start, const QString& processInfo) {
    if (start) {
        if (mProcessID == 0) {
            mProcess->start(processInfo);
        }
    } else {
#if defined(TRACK_AUTODRIVE_GUI_OUTPUT_START_STOP)
        if (STR_COMPARE(processInfo, QString("[Start] AutonomousDriving"))) {
            mProcessID = mProcess->processId();
        } else if (STR_COMPARE(processInfo, QString("[Exit] AutonomousDriving"))) {
            mProcess->terminate();
            // mProcess->kill();
            mProcessID = 0;
        } else {
            // do nothing
        }
        PRINT_HOME("ControlHome::controlProcess(%d, %s)->Process=0x%X", start, processInfo.toLatin1().data(), mProcessID);
#endif
    }
}


void ControlHome::slotTouchEvent(const int& propertyType, const int& touchType) {
    PRINT_HOME("\nControlHome::slotTouchEvent(%d, %d)", propertyType, touchType);

    switch (propertyType) {
        case HandlerEnumHome::PropertyHomeModeHome : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeHome);
            break;
        }
        case HandlerEnumHome::PropertyHomeModeRadio : {
            if (ControllerManager::mediaInstance().data()->getPlayState()) {
                ControllerManager::mediaInstance().data()->setPlayMode(HandlerEnumMedia::PropertyMediaMusicPlay, false);
                ControllerManager::mediaInstance().data()->setPlayMode(HandlerEnumMedia::PropertyMediaBtPlay, false);
                ControllerManager::mediaInstance().data()->setPlayMode(HandlerEnumMedia::PropertyMediaMoviePlay, false);
            }
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeRadio);
            break;
        }
        case HandlerEnumHome::PropertyHomeModePhone : {
            if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeBtConnect).toBool()) {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypePhone);
            } else {
                ControlPopup::instance().data()->drawPopup(HandlerEnumPopup::PopupTypeCommonBtNotConnected);
            }
            break;
        }
        case HandlerEnumHome::PropertyHomeModeMedia : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeMedia);
            break;
        }
        case HandlerEnumHome::PropertyHomeModeCamera : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeCamera);
            break;
        }
        case HandlerEnumHome::PropertyHomeModeTractor : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeTractor);
            break;
        }
        case HandlerEnumHome::PropertyHomeModeAutonomousDriving : {
                ControllerManager::instance().data()->execAutodriveApp();
                // controlProcess(true, AUTODRVIE_GUI_PATHNAME); // remarked by hgkim
                // { [hgkim]
                // autodrive_gui is "Single Application"
                // The second instance will raise the first instance and exit
                //
                // QProcess process;
                // process.startDetached(AUTODRVIE_GUI_PATHNAME);
                // }
            break;
        }
        case HandlerEnumHome::PropertyHomeModeSetup : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeSetup);
            break;
        }
        case HandlerEnumHome::PropertyHomeModeUsm : {
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeContent);
            break;
        }
        case HandlerEnumHome::PropertyHomeControlRadio : {
            updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType, HandlerEnumCommon::DisplayTypeMenu);
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeRadio);
            break;
        }
        case HandlerEnumHome::PropertyHomeControlMedia : {
            updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType, HandlerEnumCommon::DisplayTypeMenu);
            ControllerManager::instance().data()->modeChange(ScreenEnum::DisplayTypeMedia);
            break;
        }
        case HandlerEnumHome::PropertyHomeControlTopMain : {
            updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType, HandlerEnumCommon::DisplayTypeMain);
#if defined(USE_COMMON_UPDATE_NEW)
            emit ControllerManager::instance().data()->signalUpdateDataTop(HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                          HandlerEnumCommon::DisplayTypeMain);
#else
            emit ControllerManager::instance().data()->signalCommonUpdateData(ScreenEnum::DisplayTypeTop,
                                                                          HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                          HandlerEnumCommon::DisplayTypeMain);
#endif
            break;
        }
        case HandlerEnumHome::PropertyHomeControlTopHome : {
            updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType, HandlerEnumCommon::DisplayTypeMenu);
#if defined(USE_COMMON_UPDATE_NEW)
            emit ControllerManager::instance().data()->signalUpdateDataTop(HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                            HandlerEnumCommon::DisplayTypeMenu);
#else
            emit ControllerManager::instance().data()->signalCommonUpdateData(ScreenEnum::DisplayTypeTop,
                                                                            HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                            HandlerEnumCommon::DisplayTypeMenu);
#endif
            break;
        }
        case HandlerEnumCommon::PropertyMediaPrevious :
        case HandlerEnumCommon::PropertyMediaNext :
        case HandlerEnumCommon::PropertyMediaPlay : {
    #if defined(USE_HOME_PLAYING_INFO_NEW)
                ControlMedia::instance().data()->controlMedia(getData(HandlerEnumHome::PropertyHomeMediaType).toInt(),
                                                                propertyType,
                                                                getData(HandlerEnumHome::PropertyHomeMusicPlay).toInt());
    #else
                ControllerManager::instance().data()->slotTouchEvent(propertyType, touchType);
    #endif
            break;
        }
        // HandlerEnumCommon
        case HandlerEnumCommon::PropertySeekDown :
        case HandlerEnumCommon::PropertySeekUp :
        case HandlerEnumCommon::PropertyRadioPower :
        case HandlerEnumCommon::PropertyFrequencyDown :
        case HandlerEnumCommon::PropertyFrequencyUp :
        case HandlerEnumCommon::PropertyVolumeDown :
        case HandlerEnumCommon::PropertyVolumeMute :
        case HandlerEnumCommon::PropertyVolumeUp : {
            ControllerManager::instance().data()->slotTouchEvent(propertyType, 0);
            break;
        }
        default : {
            if (propertyType == HandlerEnumCommon::PropertyBack) {
                updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType,  HandlerEnumCommon::DisplayTypeMain);
#if defined(USE_COMMON_UPDATE_NEW)
                emit ControllerManager::instance().data()->signalUpdateDataTop(HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                                HandlerEnumCommon::DisplayTypeMain);
#else
                emit ControllerManager::instance().data()->signalCommonUpdateData(ScreenEnum::DisplayTypeTop,
                                                                                HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                                HandlerEnumCommon::DisplayTypeMain);
#endif
            } else if (propertyType == HandlerEnumCommon::PropertyHome) {
                updateDataHandler(HandlerEnumHome::PropertyHomeDisplayType,  HandlerEnumCommon::DisplayTypeMenu);
#if defined(USE_COMMON_UPDATE_NEW)
                emit ControllerManager::instance().data()->signalUpdateDataTop(HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                                HandlerEnumCommon::DisplayTypeMenu);
#else
                emit ControllerManager::instance().data()->signalCommonUpdateData(ScreenEnum::DisplayTypeTop,
                                                                                HandlerEnumTop::PropertyTopHomeDisplayType,
                                                                                HandlerEnumCommon::DisplayTypeMenu);
#endif
            } else {
                // nothing to do
            }
            break;
        }
    }
}

#if defined(USE_MODE_CHANGE_SIGNAL)
void ControlHome::slotModeChanged(const int& previousMode, const int& currentMode) {
    Q_UNUSED(previousMode)
    updateDataHandler(PropertyTypeEnum::PropertyTypeMode, currentMode);
}
#endif

#if defined(USE_COMMON_UPDATE_NEW)
void ControlHome::slotUpdateData(const int& dataType, const QVariant& value) {
    updateDataHandler(dataType, value);
}
#else
void ControlHome::slotCommonUpdateData(const int& sendType, const int& dataType, const QVariant& value) {
//    PRINT_HOME("ControlHome::slotCommonUpdateData(%d, %d, %s)", sendType, dataType, value.toString().toLatin1().data());
    if (sendType == getData(PropertyTypeEnum::PropertyTypeDisplay).toInt()) {
        updateDataHandler(dataType, value);
    }
}
#endif

void ControlHome::slotControlOtherController(const int& receiveMode, const int& dataType, const QVariant& value) {
    int currentMode = ControllerManager::instance().data()->getCurrentMode();

    if ((receiveMode & ScreenEnum::DisplayTypeHome) && (currentMode == ScreenEnum::DisplayTypeHome)) {
        slotTouchEvent(dataType, 0);
    }
}

void ControlHome::slotConfigChanged(const int& type, const QVariant& value) {
//    PRINT_HOME("ControlHome::slotConfigChanged(%d, %s)",  type, value.toString().toLatin1().data());
    if ((type == ConfigInfo::ConfigTypeInit)&&(value.toBool())) {
        updateDataHandler(HandlerEnumHome::PropertyHomeAudioMode, HandlerEnumCommon::AudioModeRadio);
    } else if (type == ConfigInfo::ConfigTypeAudioMode) {
        updateDataHandler(HandlerEnumHome::PropertyHomeAudioMode, value);
    } else if (type == ConfigInfo::ConfigTypeAudioPlayState) {
        if (value.toBool()) {
            updateDataHandler(HandlerEnumHome::PropertyHomeAudioMode, HandlerEnumCommon::AudioModeRadio);
        }
    } else if (type == ConfigInfo::ConfigTypeUsbConnect) {
        bool connect = value.toBool();
        int soundPath = ControllerManager::audioInstance().data()->getSoundPath();

        if ((connect == false)
#if defined (USE_FEATURE_BT_PHONE_MUSIC_CONTROL)
            && (soundPath == Drv_Sound_ctl::SOUND_TYPE_MEDIA)
#endif
            ) {
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicPlay, false);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicTitle, QString(""));
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicArtist, QString(""));
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicThumbnailPath, QString(""));
        }

        updateDataHandler(HandlerEnumHome::PropertyHomeUsbConnect, connect);
    } else if (type == ConfigInfo::ConfigTypeBtConnect) {
        updateDataHandler(HandlerEnumHome::PropertyHomeBtConnect, value.toBool());
    } else {
        // do nothing
    }
}

void ControlHome::slotPlayingInfoChanged(const int& mediaType, const bool& play, const int& position, const int& playTime,
                                const int& totalTime, const QString& artist, const QString& title, const QString& thumbnail) {
    bool usbConnect = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeUsbConnect).toBool();
    bool btConnect = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeBtConnect).toBool();
    int soundPath = ControllerManager::audioInstance().data()->getSoundPath();
    int audioMode = getData(HandlerEnumHome::PropertyHomeAudioMode).toInt();

    if ((usbConnect) || (btConnect)) {
        updateDataHandler(HandlerEnumHome::PropertyHomeMediaType, mediaType);

        if (audioMode == HandlerEnumCommon::AudioModeBtMusic) {
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicThumbnailPath, thumbnail);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicPlay, play);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicPlayTime, position);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicTotalTime, totalTime);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicArtist, artist);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicTitle, title);
        } else {
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicPlay, play);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicTotalTime, totalTime);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicArtist, artist);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicTitle, title);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicThumbnailPath, thumbnail);
            updateDataHandler(HandlerEnumHome::PropertyHomeMusicPlayTime, position);
        }
    }
}

void ControlHome::slotTractorVehicleDataChanged(const int& type, const QVariant& data) {
    int dataType = HandlerEnumTractor::PropertyTractorVehicleDataEngineSpeed+type-Tractor::DataTypeVehicleDataStart;
    int propertyType = HandlerEnumHome::PropertyStartQml;

    switch (dataType) {
        case HandlerEnumTractor::PropertyTractorVehicleDataVehicleSpeed : {
            propertyType = HandlerEnumHome::PropertyHomeVehicleSpeed;
            break;
        }
        case HandlerEnumTractor::PropertyTractorVehicleDataEngineSpeed : {
            propertyType = HandlerEnumHome::PropertyHomeEngineSpeed;
            break;
        }
        case HandlerEnumTractor::PropertyTractorVehicleDataFuelLevel : {
            propertyType = HandlerEnumHome::PropertyHomeFuelLevel;
            break;
        }
        case HandlerEnumTractor::PropertyTractorVehicleDataShiftOilTemperature : {
            propertyType = HandlerEnumHome::PropertyHomeShiftOilTemperature;
            break;
        }
        default : {
            break;
        }
    }

    if (propertyType != HandlerEnumHome::PropertyStartQml) {
        updateDataHandler(propertyType, data);
    }
}

void ControlHome::slotProcessStandardOutput() {
    QString log = QString(mProcess->readAllStandardOutput());
    log.resize(log.size()-1);
    controlProcess(false, log);
}

void ControlHome::slotProcessFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus) {
    PRINT_HOME("ControlHome::slotProcessFinished(%d, %d)", exitCode, exitStatus);
}
