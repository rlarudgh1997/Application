/**
 * @file EV_Charging_System_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
 * All Rights Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include the above copyright notice, this
 * paragraph and the one after it. Any distribution of this software or
 * derivative works must comply with all applicable laws.
 *
 * This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
 * ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
 * LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
 * EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
 * NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGES.
 */
#ifndef SFSS_EV_Charging_System_CV_H
#define SFSS_EV_Charging_System_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "EV_Charging_System_CVBase.hpp"

namespace ccos {

// SFC Version : 8.0.0
// Spec Version : v0.27
// Reference : [CV720] EV_Charging_System_CV. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class EV_Charging_System_CV : public EV_Charging_System_CVBase {
public:
    EV_Charging_System_CV() = default;
    ~EV_Charging_System_CV() override = default;
    EV_Charging_System_CV(const EV_Charging_System_CV& other) = delete;
    EV_Charging_System_CV(EV_Charging_System_CV&& other) noexcept = delete;
    EV_Charging_System_CV& operator=(const EV_Charging_System_CV& other) = delete;
    EV_Charging_System_CV& operator=(EV_Charging_System_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEV_Charging_System_CVEventChargingFailLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_CVEventChargingFailLinkedSoundType(
            SFCEV_Charging_System_CVEventChargingFailLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_CVEventChargingFailLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_CVEventChargingLinkedSoundID("SND_PopUpInform2");
        setSFCEV_Charging_System_CVEventChargingLinkedSoundType(
            SFCEV_Charging_System_CVEventChargingLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_CVEventChargingLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleEvChgCable();
        updateConstantChargingRemainTimeValue();
        updateConstantChargingCurrentSocValue();
        updateConstantBmsChargingPhrasesStatus();
        updateConstantChargingPwrValue();
        updateConstantV2XMinimumSocValue();
        updateEventChargingFail();
        updateEventCharging();
    }

    void onVcmsPDStatusChanged(const ArgumentsVcmsPDStatusChanged& args) {
        mVCMSPDStatus = args.mInput_VCMSPDStatus;
        updateTelltaleEvChgCable();
    }

    void onVcmsPlugConnectStatusChanged(const ArgumentsVcmsPlugConnectStatusChanged& args) {
        mVCMSPlugConnectStatus = args.mInput_VCMSPlugConnectStatus;
        updateTelltaleEvChgCable();
    }

    void onBmsChgRemainedTimeMinChanged(const ArgumentsBmsChgRemainedTimeMinChanged& args) {
        mBmsChgRemainedTimeMin = args.mInput_BmsChgRemainedTimeMin;
        updateConstantChargingRemainTimeValue();
    }

    void onBmsDisplaySocValueChanged(const ArgumentsBmsDisplaySocValueChanged& args) {
        mBmsDisplaySocValue = args.mInput_BmsDisplaySocValue;
        updateConstantChargingCurrentSocValue();
    }

    void onBmsChgStatusPhrasesChanged(const ArgumentsBmsChgStatusPhrasesChanged& args) {
        mBmsStdChgStatus = args.mInput_BmsStdChgStatus;
        mBmsFstChgStatus = args.mInput_BmsFstChgStatus;
        mBmsFulChgStatus = args.mInput_BmsFulChgStatus;
        mBmsChgFinStatus = args.mInput_BmsChgFinStatus;
        updateConstantBmsChargingPhrasesStatus();
    }

    void onVcmsChgAlarmStatusChanged(const ArgumentsVcmsChgAlarmStatusChanged& args) {
        mVCMSChgAlarmStatus = args.mInput_VCMSChgAlarmStatus;
        mVCMSChgAlarmSubStatus = args.mInput_VCMSChgAlarmSubStatus;
        updateEventChargingFail();
    }

    void onV2xMinimumOperatingValueChanged(const ArgumentsV2xMinimumOperatingValueChanged& args) {
        mV2XMsgAppliedStatus = args.mInput_V2XMsgAppliedStatus;
        mV2XMinimumSocFromVcmsValue = args.mInput_V2XMinimumSocFromVcmsValue;
        updatePrivateV2XMinimumSocFromVcmsValue();
        updateConstantV2XMinimumSocValue();
    }

    void onEvsePwrDisValueChanged(const ArgumentsEvsePwrDisValueChanged& args) {
        mEvsePwrDisValue = args.mInput_EvsePwrDisValue;
        updateConstantChargingPwrValue();
    }

    void onV2xDisplayOnFromVCMSChanged(const ArgumentsV2xDisplayOnFromVCMSChanged& args) {
        mV2XDisplayOnFromVCMS = args.mInput_V2XDisplayOnFromVCMS;
        updateEventCharging();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateV2XMinimumSocFromVcmsValue() {
        if (mV2XMsgAppliedStatus == V2XMsgAppliedStatus::ON) {
            mPrivateV2XMinimumSocFromVcmsValue = mV2XMinimumSocFromVcmsValue;
        } else {
            mPrivateV2XMinimumSocFromVcmsValue = 0;
        }
    }

    void updateTelltaleEvChgCable() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCEV_Charging_System_CVTelltaleEvChgCableStat stat = SFCEV_Charging_System_CVTelltaleEvChgCableStat::OFF;

        if (mVCMSPDStatus == VCMSPDStatus::ON || mVCMSPlugConnectStatus == VCMSPlugConnectStatus::ON) {
            stat = SFCEV_Charging_System_CVTelltaleEvChgCableStat::ON;
        }
        setSFCEV_Charging_System_CVTelltaleEvChgCableStat(stat);
    }

    void updateConstantChargingRemainTimeValue() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat stat =
            SFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat::UNMEASURABLE;

        if (mBmsChgRemainedTimeMin == 0) {
            stat = SFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat::MINIMUM;
        } else if (1 <= mBmsChgRemainedTimeMin && mBmsChgRemainedTimeMin <= 5999) {
            stat = SFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat::VALUE;
            setSFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinValue(mBmsChgRemainedTimeMin);
        } else if (6000 <= mBmsChgRemainedTimeMin && mBmsChgRemainedTimeMin <= 65534) {
            stat = SFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat::MAXIMUM;
        } else {
            // no operation
        }
        setSFCEV_Charging_System_CVConstantBmsChgRemainedTimeMinStat(stat);
    }

    void updateConstantChargingCurrentSocValue() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCEV_Charging_System_CVConstantBmsDisplaySocStat stat = SFCEV_Charging_System_CVConstantBmsDisplaySocStat::UNMEASURABLE;

        if (0 <= mBmsDisplaySocValue && mBmsDisplaySocValue <= 200) {
            HUInt64 value = 0;
            stat = SFCEV_Charging_System_CVConstantBmsDisplaySocStat::VALUE;
            value = round(static_cast<HDouble>(mBmsDisplaySocValue) * 0.5);
            setSFCEV_Charging_System_CVConstantBmsDisplaySocValue(value);
        }
        setSFCEV_Charging_System_CVConstantBmsDisplaySocStat(stat);
    }

    void updateConstantBmsChargingPhrasesStatus() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat stat =
            SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat::DISPLAY_OFF;

        if (mBmsStdChgStatus == BmsStdChgStatus::STD_CHARGING && mBmsFstChgStatus == BmsFstChgStatus::OFF &&
            mBmsFulChgStatus == BmsFulChgStatus::OFF && mBmsChgFinStatus == BmsChgFinStatus::OFF) {
            stat = SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat::STD_CHARGING;
        } else if (mBmsStdChgStatus == BmsStdChgStatus::OFF && mBmsFstChgStatus == BmsFstChgStatus::FST_CHARGING &&
                   mBmsFulChgStatus == BmsFulChgStatus::OFF && mBmsChgFinStatus == BmsChgFinStatus::OFF) {
            stat = SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat::FST_CHARGING;
        } else if (mBmsFulChgStatus == BmsFulChgStatus::FULL_CHARGING) {
            stat = SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat::FULL_CHARGING;
        } else if (mBmsFulChgStatus == BmsFulChgStatus::OFF && mBmsChgFinStatus == BmsChgFinStatus::FIN_CHARGING) {
            stat = SFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat::FIN_CHARGING;
        } else {
            // no operation
        }
        setSFCEV_Charging_System_CVConstantBmsChgStatusPhrasesStat(stat);
    }

    void updateConstantChargingPwrValue() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        HUInt64 value = 0;
        if (mEvsePwrDisValue >= 0 && mEvsePwrDisValue <= 511) {
            value = mEvsePwrDisValue;
        }
        setSFCEV_Charging_System_CVConstantChargingPwrValue(value);
    }

    void updateConstantV2XMinimumSocValue() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        HUInt64 value = 101;
        if (mPrivateV2XMinimumSocFromVcmsValue >= 1 && mPrivateV2XMinimumSocFromVcmsValue <= 200) {
            value = static_cast<HUInt64>(floor(static_cast<HDouble>(mPrivateV2XMinimumSocFromVcmsValue) * 0.5));
        } else if (mPrivateV2XMinimumSocFromVcmsValue >= 201 && mPrivateV2XMinimumSocFromVcmsValue <= 254) {
            value = 100;
        } else {
            // no operation
        }
        setSFCEV_Charging_System_CVConstantV2XMinimumSocValue(value);
    }

    void updateEventChargingFail() {
        std::string prevEventID;
        std::string eventID;

        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        if (mVCMSChgAlarmStatus == VCMSChgAlarmStatus::AC_CHARGING_FAIL) {
            switch (mVCMSChgAlarmSubStatus) {
                case VCMSChgAlarmSubStatus::CHARGER_ERROR:
                    eventID = "E77201";
                    break;
                case VCMSChgAlarmSubStatus::CONNECT_ERROR:
                    eventID = "E77202";
                    break;
                case VCMSChgAlarmSubStatus::CONTROLLER_ERROR:
                    eventID = "E77203";
                    break;
                case VCMSChgAlarmSubStatus::BAT_LOWTEMP:
                    eventID = "E77205";
                    break;
                case VCMSChgAlarmSubStatus::CONNECT_HIGHTEMP:
                    eventID = "E77206";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.EV_Charging_System_CV.Event.ChargingFail.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_CVEventChargingFailStat(SFCEV_Charging_System_CVEventChargingFailStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_CVEventChargingFailID(eventID);
            setSFCEV_Charging_System_CVEventChargingFailStat(SFCEV_Charging_System_CVEventChargingFailStat::ON);
        }
    }

    void updateEventCharging() {
        std::string prevEventID;
        std::string eventID;

        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        if (mV2XDisplayOnFromVCMS == V2XDisplayOnFromVCMS::CHARGING) {
            eventID = "E77204";
        } else if (mV2XDisplayOnFromVCMS == V2XDisplayOnFromVCMS::V2X_DISPLAY_ON) {
            eventID = "E77207";
        } else {
            // no operation
        }

        GETCACHEDVALUE(SFC.EV_Charging_System_CV.Event.Charging.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_CVEventChargingStat(SFCEV_Charging_System_CVEventChargingStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_CVEventChargingID(eventID);
            setSFCEV_Charging_System_CVEventChargingStat(SFCEV_Charging_System_CVEventChargingStat::ON);
        }
    }

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using VCMSPDStatus = ArgumentsVcmsPDStatusChanged::Input_VCMSPDStatus;
    using VCMSPlugConnectStatus = ArgumentsVcmsPlugConnectStatusChanged::Input_VCMSPlugConnectStatus;
    using VCMSChgAlarmStatus = ArgumentsVcmsChgAlarmStatusChanged::Input_VCMSChgAlarmStatus;
    using VCMSChgAlarmSubStatus = ArgumentsVcmsChgAlarmStatusChanged::Input_VCMSChgAlarmSubStatus;
    using BmsStdChgStatus = ArgumentsBmsChgStatusPhrasesChanged::Input_BmsStdChgStatus;
    using BmsFstChgStatus = ArgumentsBmsChgStatusPhrasesChanged::Input_BmsFstChgStatus;
    using BmsFulChgStatus = ArgumentsBmsChgStatusPhrasesChanged::Input_BmsFulChgStatus;
    using BmsChgFinStatus = ArgumentsBmsChgStatusPhrasesChanged::Input_BmsChgFinStatus;
    using V2XMsgAppliedStatus = ArgumentsV2xMinimumOperatingValueChanged::Input_V2XMsgAppliedStatus;
    using V2XDisplayOnFromVCMS = ArgumentsV2xDisplayOnFromVCMSChanged::Input_V2XDisplayOnFromVCMS;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    VCMSPDStatus mVCMSPDStatus = VCMSPDStatus::OFF;
    VCMSPlugConnectStatus mVCMSPlugConnectStatus = VCMSPlugConnectStatus::OFF;
    VCMSChgAlarmStatus mVCMSChgAlarmStatus = VCMSChgAlarmStatus::OFF;
    VCMSChgAlarmSubStatus mVCMSChgAlarmSubStatus = VCMSChgAlarmSubStatus::OFF;
    HUInt64 mBmsChgRemainedTimeMin = 0;
    HUInt64 mBmsDisplaySocValue = 0;
    BmsStdChgStatus mBmsStdChgStatus = BmsStdChgStatus::OFF;
    BmsFstChgStatus mBmsFstChgStatus = BmsFstChgStatus::OFF;
    BmsFulChgStatus mBmsFulChgStatus = BmsFulChgStatus::OFF;
    BmsChgFinStatus mBmsChgFinStatus = BmsChgFinStatus::OFF;
    V2XMsgAppliedStatus mV2XMsgAppliedStatus = V2XMsgAppliedStatus::OFF;
    HUInt64 mV2XMinimumSocFromVcmsValue = 0;
    HUInt64 mPrivateV2XMinimumSocFromVcmsValue = 0;
    HUInt64 mEvsePwrDisValue = 0;
    V2XDisplayOnFromVCMS mV2XDisplayOnFromVCMS = V2XDisplayOnFromVCMS::OFF;
};

}  // namespace ccos

#endif  // SFSS_EV_Charging_System_CV_H
