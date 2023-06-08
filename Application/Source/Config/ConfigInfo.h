#ifndef CONFIG_INFO_H
#define CONFIG_INFO_H

#include <QVariant>
#include <QMap>

#include "CommonDefine.h"
#include "HandlerEnum.h"


#define DEFAULT_RADIO_FM 9430

class ConfigInfo {
public:
    typedef enum {
        // Main
        ConfigTypeSheetName,
        ConfigTypeContextName,

        // Common
        ConfigTypeInit,
        ConfigTypeMode,
        ConfigTypeAudioMode,
        ConfigTypeAudioPlayState,
        ConfigTypeUsbConnect,
        ConfigTypeBtConnect,
        ConfigTypeError,
        ConfigTypeMute,

        // Home
        ConfigTypeHome,

        // Radio
        ConfigTypeRadio,
        ConfigTypeRadioBand,
        ConfigTypeRadioFrequencyFM,
        ConfigTypeRadioFrequencyAM,
        ConfigTypeRadioPresetList01,
        ConfigTypeRadioPresetList02,
        ConfigTypeRadioPresetList03,
        ConfigTypeRadioPresetList04,
        ConfigTypeRadioPresetList05,
        ConfigTypeRadioPresetList06,
        ConfigTypeRadioPresetList07,
        ConfigTypeRadioPresetList08,
        ConfigTypeRadioPresetList09,
        ConfigTypeRadioPresetList10,
        ConfigTypeRadioPresetList11,
        ConfigTypeRadioPresetList12,
        ConfigTypeRadioList01,
        ConfigTypeRadioList02,
        ConfigTypeRadioList03,
        ConfigTypeRadioList04,
        ConfigTypeRadioList05,
        ConfigTypeRadioList06,
        ConfigTypeRadioList07,
        ConfigTypeRadioList08,
        ConfigTypeRadioList09,
        ConfigTypeRadioList10,
        ConfigTypeRadioList11,
        ConfigTypeRadioList12,
        ConfigTypeRadioList13,
        ConfigTypeRadioList14,
        ConfigTypeRadioList15,
        ConfigTypeRadioList16,
        ConfigTypeRadioList17,
        ConfigTypeRadioList18,
        ConfigTypeRadioList19,
        ConfigTypeRadioList20,
        ConfigTypeRadioUseBSM,

        // Media
        ConfigTypeMedia,
        ConfigTypeMusicSupportCodec,

        // Phone
        ConfigTypePhone,
        ConfigTypePhoneConnectDeviceInfo,
        ConfigTypePhoneParingDeviceInfo1,
        ConfigTypePhoneParingDeviceInfo2,
        ConfigTypePhoneParingDeviceInfo3,
        ConfigTypePhoneParingDeviceInfo4,
        ConfigTypePhoneParingDeviceInfo5,
        ConfigTypePhoneFavorite01,
        ConfigTypePhoneFavorite02,
        ConfigTypePhoneFavorite03,
        ConfigTypePhoneFavorite04,
        ConfigTypePhoneFavorite05,
        ConfigTypePhoneFavorite06,
        ConfigTypePhoneFavorite07,
        ConfigTypePhoneFavorite08,
        ConfigTypePhoneFavorite09,
        ConfigTypePhoneFavorite10,
        ConfigTypePhoneFavorite11,
        ConfigTypePhoneFavorite12,
        ConfigTypePhoneFavorite13,
        ConfigTypePhoneFavorite14,
        ConfigTypePhoneFavorite15,
        ConfigTypePhoneFavorite16,
        ConfigTypePhoneFavorite17,
        ConfigTypePhoneFavorite18,
        ConfigTypePhoneFavorite19,
        ConfigTypePhoneFavorite20,
        ConfigTypePhoneFavorite21,
        ConfigTypePhoneFavorite22,
        ConfigTypePhoneFavorite23,
        ConfigTypePhoneFavorite24,
        ConfigTypePhoneFavorite25,
        ConfigTypePhoneFavorite26,
        ConfigTypePhoneFavorite27,
        ConfigTypePhoneFavorite28,
        ConfigTypePhoneFavorite29,
        ConfigTypePhoneFavorite30,
        ConfigTypePhoneFavorite31,
        ConfigTypePhoneFavorite32,
        ConfigTypePhoneFavorite33,
        ConfigTypePhoneFavorite34,
        ConfigTypePhoneFavorite35,
        ConfigTypePhoneFavorite36,
        ConfigTypePhoneFavorite37,
        ConfigTypePhoneFavorite38,
        ConfigTypePhoneFavorite39,
        ConfigTypePhoneFavorite40,
        ConfigTypePhoneFavorite41,
        ConfigTypePhoneFavorite42,
        ConfigTypePhoneFavorite43,
        ConfigTypePhoneFavorite44,
        ConfigTypePhoneFavorite45,
        ConfigTypePhoneFavorite46,
        ConfigTypePhoneFavorite47,
        ConfigTypePhoneFavorite48,
        ConfigTypePhoneFavorite49,
        ConfigTypePhoneFavorite50,

        ConfigTypePhoneEmergencyName01,
        ConfigTypePhoneEmergencyName02,
        ConfigTypePhoneEmergencyName03,
        ConfigTypePhoneEmergencyName04,
        ConfigTypePhoneEmergencyNumber01,
        ConfigTypePhoneEmergencyNumber02,
        ConfigTypePhoneEmergencyNumber03,
        ConfigTypePhoneEmergencyNumber04,

        ConfigTypePhoneEmergencyUserNumber01,
        ConfigTypePhoneEmergencyUserNumber02,
        ConfigTypePhoneEmergencyUserNumber03,
        ConfigTypePhoneEmergencyUserNumber04,
        ConfigTypePhoneEmergencyUserNumber05,
        ConfigTypePhoneEmergencyUserNumber06,
        ConfigTypePhoneEmergencyUserNumber07,
        ConfigTypePhoneEmergencyUserNumber08,
        ConfigTypePhoneEmergencyUserNumber09,
        ConfigTypePhoneEmergencyUserNumber10,

        // Camera
        ConfigTypeCamera,

        // Tractor
        ConfigTypeTractor,

        // Setting
        ConfigTypeSetting,
        ConfigTypeSettingLanguage,
        ConfigTypeSettingVolume,
        ConfigTypeSettingVolumeCalling,
        ConfigTypeSettingVolumeBackup,
        ConfigTypeSettingInternalSpeak,
        ConfigTypeSettingBrightnessType,
        ConfigTypeSettingThemeType,
        ConfigTypeSettingBrightness,
        ConfigTypeSettingBrightnessBackup,
        ConfigTypeSettingDateTimeGps,
        ConfigTypeSettingFrontCamera,

        ConfigTypeMax,
    } eConfigType;

    typedef enum {
        ConfigGetTypeName = 0,
        ConfigGetTypeValue,
    } eConfigGetType;

public:
    ConfigInfo() {
        initConfigInfo();
    }

    QVariant getConfigInfo(const eConfigType& configType, const eConfigGetType& getType) const {
        return ((getType == ConfigGetTypeName) ? (QVariant(mConfigInfoData[configType].first))
                                                : (QVariant(mConfigInfoData[configType].second)));
    }

private:
    void initConfigInfo() {
        mConfigInfoData.clear();

        mConfigInfoData[ConfigTypeSheetName] =
                QPair<QString, QVariant>("ConfigTypeSheetName", QVariant(false));
        mConfigInfoData[ConfigTypeContextName] =
                QPair<QString, QVariant>("ConfigTypeContextName", QVariant(false));

        // Common
        mConfigInfoData[ConfigTypeInit] =
                QPair<QString, QVariant>("ConfigTypeInit", QVariant(false));
        mConfigInfoData[ConfigTypeMode] =
                QPair<QString, QVariant>("ConfigTypeMode", QVariant(0));
        mConfigInfoData[ConfigTypeAudioMode] =
                QPair<QString, QVariant>("ConfigTypeAudioMode", QVariant(HandlerEnumCommon::AudioModeRadio));
        mConfigInfoData[ConfigTypeAudioPlayState] =
                QPair<QString, QVariant>("ConfigTypeAudioPlayState", QVariant(false));
        mConfigInfoData[ConfigTypeUsbConnect] =
                QPair<QString, QVariant>("ConfigTypeUsbConnect", QVariant(false));
        mConfigInfoData[ConfigTypeBtConnect] =
                QPair<QString, QVariant>("ConfigTypeBtConnect", QVariant(false));
        mConfigInfoData[ConfigTypeError] =
                QPair<QString, QVariant>("ConfigTypeError", QVariant(false));
        mConfigInfoData[ConfigTypeMute] =
                QPair<QString, QVariant>("ConfigTypeMute", QVariant(false));


        // Home
        mConfigInfoData[ConfigTypeHome] =
                QPair<QString, QVariant>("ConfigTypeHome", QVariant(0));


        // Radio
        mConfigInfoData[ConfigTypeRadioBand] =
                QPair<QString, QVariant>("ConfigTypeRadioBand", QVariant(0));
        mConfigInfoData[ConfigTypeRadioBand] =
                QPair<QString, QVariant>("ConfigTypeRadioBand", QVariant(HandlerEnumRadio::BandFM));
        mConfigInfoData[ConfigTypeRadioFrequencyFM] =
                QPair<QString, QVariant>("ConfigTypeRadioFrequencyFM", QVariant(DEFAULT_RADIO_FM));
        mConfigInfoData[ConfigTypeRadioFrequencyAM] =
                QPair<QString, QVariant>("ConfigTypeRadioFrequencyAM", QVariant(1610));
        mConfigInfoData[ConfigTypeRadioPresetList01] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList01", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList02] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList02", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList03] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList03", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList04] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList04", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList05] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList05", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList06] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList06", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList07] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList07", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList08] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList08", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList09] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList09", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList10] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList10", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList11] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList11", QVariant(""));
        mConfigInfoData[ConfigTypeRadioPresetList12] =
                QPair<QString, QVariant>("ConfigTypeRadioPresetList12", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList01] =
                QPair<QString, QVariant>("ConfigTypeRadioList01", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList02] =
                QPair<QString, QVariant>("ConfigTypeRadioList02", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList03] =
                QPair<QString, QVariant>("ConfigTypeRadioList03", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList04] =
                QPair<QString, QVariant>("ConfigTypeRadioList04", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList05] =
                QPair<QString, QVariant>("ConfigTypeRadioList05", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList06] =
                QPair<QString, QVariant>("ConfigTypeRadioList06", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList07] =
                QPair<QString, QVariant>("ConfigTypeRadioList07", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList08] =
                QPair<QString, QVariant>("ConfigTypeRadioList08", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList09] =
                QPair<QString, QVariant>("ConfigTypeRadioList09", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList10] =
                QPair<QString, QVariant>("ConfigTypeRadioList10", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList11] =
                QPair<QString, QVariant>("ConfigTypeRadioList11", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList12] =
                QPair<QString, QVariant>("ConfigTypeRadioList12", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList13] =
                QPair<QString, QVariant>("ConfigTypeRadioList13", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList14] =
                QPair<QString, QVariant>("ConfigTypeRadioList14", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList15] =
                QPair<QString, QVariant>("ConfigTypeRadioList15", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList16] =
                QPair<QString, QVariant>("ConfigTypeRadioList16", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList17] =
                QPair<QString, QVariant>("ConfigTypeRadioList17", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList18] =
                QPair<QString, QVariant>("ConfigTypeRadioList18", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList19] =
                QPair<QString, QVariant>("ConfigTypeRadioList19", QVariant(""));
        mConfigInfoData[ConfigTypeRadioList20] =
                QPair<QString, QVariant>("ConfigTypeRadioList20", QVariant(""));
        mConfigInfoData[ConfigTypeRadioUseBSM] =
                QPair<QString, QVariant>("ConfigTypeRadioUseBSM", QVariant(false));


        // Media
        mConfigInfoData[ConfigTypeMedia] =
                QPair<QString, QVariant>("ConfigTypeMedia", QVariant(0));
        mConfigInfoData[ConfigTypeMusicSupportCodec] =
                QPair<QString, QVariant>("ConfigTypeMusicSupportCodec", QVariant("mp3/aac/ogg/flac/m4a"));


        // Phone
        mConfigInfoData[ConfigTypePhone] =
                QPair<QString, QVariant>("ConfigTypePhone", QVariant(0));


        // Phone - Favorite
        mConfigInfoData[ConfigTypePhoneConnectDeviceInfo] =
                QPair<QString, QVariant>("ConfigTypePhoneConnectDeviceInfo", QVariant(""));
        mConfigInfoData[ConfigTypePhoneParingDeviceInfo1] =
                QPair<QString, QVariant>("ConfigTypePhoneParingDeviceInfo1", QVariant(""));
        mConfigInfoData[ConfigTypePhoneParingDeviceInfo2] =
                QPair<QString, QVariant>("ConfigTypePhoneParingDeviceInfo2", QVariant(""));
        mConfigInfoData[ConfigTypePhoneParingDeviceInfo3] =
                QPair<QString, QVariant>("ConfigTypePhoneParingDeviceInfo3", QVariant(""));
        mConfigInfoData[ConfigTypePhoneParingDeviceInfo4] =
                QPair<QString, QVariant>("ConfigTypePhoneParingDeviceInfo4", QVariant(""));
        mConfigInfoData[ConfigTypePhoneParingDeviceInfo5] =
                QPair<QString, QVariant>("ConfigTypePhoneParingDeviceInfo5", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite01] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite01", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite02] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite02", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite03] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite03", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite04] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite04", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite05] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite05", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite06] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite06", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite07] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite07", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite08] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite08", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite09] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite09", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite10] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite10", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite11] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite11", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite12] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite12", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite13] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite13", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite14] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite14", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite15] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite15", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite16] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite16", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite17] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite17", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite18] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite18", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite19] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite19", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite20] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite20", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite21] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite21", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite22] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite22", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite23] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite23", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite24] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite24", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite25] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite25", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite26] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite26", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite27] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite27", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite28] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite28", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite29] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite29", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite30] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite30", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite31] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite31", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite32] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite32", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite33] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite33", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite34] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite34", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite35] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite35", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite36] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite36", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite37] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite37", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite38] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite38", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite39] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite39", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite40] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite40", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite41] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite41", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite42] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite42", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite43] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite43", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite44] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite44", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite45] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite45", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite46] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite46", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite47] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite47", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite48] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite48", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite49] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite49", QVariant(""));
        mConfigInfoData[ConfigTypePhoneFavorite50] =
                QPair<QString, QVariant>("ConfigTypePhoneFavorite50", QVariant(""));


        // Phone - Emercency
        mConfigInfoData[ConfigTypePhoneEmergencyName01] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyName01", QVariant("경찰서(112)"));
        mConfigInfoData[ConfigTypePhoneEmergencyName02] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyName02", QVariant("화재구조구급(119)"));
        mConfigInfoData[ConfigTypePhoneEmergencyName03] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyName03", QVariant("TYM 고객센터"));
        mConfigInfoData[ConfigTypePhoneEmergencyName04] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyName04", QVariant("사용자 지정 번호"));
        mConfigInfoData[ConfigTypePhoneEmergencyNumber01] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyNumber01", QVariant("112"));
        mConfigInfoData[ConfigTypePhoneEmergencyNumber02] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyNumber02", QVariant("119"));
        mConfigInfoData[ConfigTypePhoneEmergencyNumber03] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyNumber03", QVariant("1111-2222"));
        mConfigInfoData[ConfigTypePhoneEmergencyNumber04] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyNumber04", QVariant("UserNumber"));


        // Phone - UserSettingNumber
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber01] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber01", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber02] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber02", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber03] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber03", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber04] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber04", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber05] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber05", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber06] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber06", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber07] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber07", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber08] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber08", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber09] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber09", QVariant(""));
        mConfigInfoData[ConfigTypePhoneEmergencyUserNumber10] =
                QPair<QString, QVariant>("ConfigTypePhoneEmergencyUserNumber10", QVariant(""));


        // Camera
        mConfigInfoData[ConfigTypeCamera] =
                QPair<QString, QVariant>("ConfigTypeCamera", QVariant(0));


        // ConfigTypeTractor
        mConfigInfoData[ConfigTypeTractor] =
                QPair<QString, QVariant>("ConfigTypeTractor", QVariant(0));


        // Setting
        mConfigInfoData[ConfigTypeSetting] =
                QPair<QString, QVariant>("ConfigTypeSetting", QVariant(0));
        mConfigInfoData[ConfigTypeSettingLanguage] =
                QPair<QString, QVariant>("ConfigTypeSettingLanguage", QVariant(HandlerEnumSetup::LanguageTypeKorea));
        mConfigInfoData[ConfigTypeSettingVolume] =
                QPair<QString, QVariant>("ConfigTypeSettingVolume", QVariant(6));
        mConfigInfoData[ConfigTypeSettingVolumeCalling] =
                QPair<QString, QVariant>("ConfigTypeSettingVolumeCalling", QVariant(6));
        mConfigInfoData[ConfigTypeSettingVolumeBackup] =
                QPair<QString, QVariant>("ConfigTypeSettingVolumeBackup", QVariant(6));
        mConfigInfoData[ConfigTypeSettingInternalSpeak] =
                QPair<QString, QVariant>("ConfigTypeSettingInternalSpeak", QVariant(false));
        mConfigInfoData[ConfigTypeSettingBrightnessType] =
                QPair<QString, QVariant>("ConfigTypeSettingBrightnessType", QVariant(HandlerEnumSetup::BrightnessTypeAuto));
        mConfigInfoData[ConfigTypeSettingThemeType] =
                QPair<QString, QVariant>("ConfigTypeSettingThemeType", QVariant(HandlerEnumSetup::ThemeTypeAuto));
        mConfigInfoData[ConfigTypeSettingBrightness] =
                QPair<QString, QVariant>("ConfigTypeSettingBrightness", QVariant(100));
        mConfigInfoData[ConfigTypeSettingBrightnessBackup] =
                QPair<QString, QVariant>("ConfigTypeSettingBrightnessBackup", QVariant(100));
        mConfigInfoData[ConfigTypeSettingDateTimeGps] =
                QPair<QString, QVariant>("ConfigTypeSettingDateTimeGps", QVariant(true));
        mConfigInfoData[ConfigTypeSettingFrontCamera] =
                QPair<QString, QVariant>("ConfigTypeSettingFrontCamera", QVariant(false));
    }

private:
    QMap<eConfigType, QPair<QString, QVariant>> mConfigInfoData;
};

#endif    // CONFIG_INFO_H
