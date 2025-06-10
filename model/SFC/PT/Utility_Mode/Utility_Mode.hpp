/**
 * @file Utility_Mode.hpp
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
#ifndef SFSS_Utility_Mode_H
#define SFSS_Utility_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Utility_ModeBase.hpp"

namespace ccos {

// SFC Version : 6.0.0
// Reference : [PT490] Utility_Mode. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Utility_Mode : public Utility_ModeBase {
public:
    Utility_Mode() = default;
    ~Utility_Mode() override = default;
    Utility_Mode(const Utility_Mode& other) = delete;
    Utility_Mode(Utility_Mode&& other) noexcept = delete;
    Utility_Mode& operator=(const Utility_Mode& other) = delete;
    Utility_Mode& operator=(Utility_Mode&& other) noexcept = delete;

    void onInitialize() override {
        // Stay Mode Full Range Popup
        setSFCUtility_ModeEventStayModeFullRangePopupID("E25302");
        // Stay Mode End Popup
        setSFCUtility_ModeEventStayModeEndPopupLinkedSoundID("SND_PopUpWarn1");
        setSFCUtility_ModeEventStayModeEndPopupLinkedSoundType(SFCUtility_ModeEventStayModeEndPopupLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_ModeEventStayModeEndPopupLinkedSoundRepeatCount(1);
        // Stay Mode Shifting Popup
        setSFCUtility_ModeEventStayModeShiftingPopupID("E25305");
        setSFCUtility_ModeEventStayModeShiftingPopupLinkedSoundID("SND_PopUpWarn1");
        setSFCUtility_ModeEventStayModeShiftingPopupLinkedSoundType(
            SFCUtility_ModeEventStayModeShiftingPopupLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_ModeEventStayModeShiftingPopupLinkedSoundRepeatCount(1);
        // Stay Mode Charge Control
        setSFCUtility_ModeEventStayModeChargeControlID("E25306");
        setSFCUtility_ModeEventStayModeChargeControlLinkedSoundID("SND_PopUpInform2");
        setSFCUtility_ModeEventStayModeChargeControlLinkedSoundType(
            SFCUtility_ModeEventStayModeChargeControlLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_ModeEventStayModeChargeControlLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltaleUtilityModeLmpStat();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltaleUtilityModeLmpStat();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltaleUtilityModeLmpStat();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEventUtilityMode();
    }

    void onEvUtilModeStatusChanged(const ArgumentsEvUtilModeStatusChanged& args) {
        mEvUtilModeStatus = args.mInput_EvUtilModeStatus;
        updateTelltaleUtilityModeLmpStat();
        updateConstantUtilityModeForConnect();
    }

    void onStayModeUsingTimeMinValueChanged(const ArgumentsStayModeUsingTimeMinValueChanged& args) {
        if (ISTIMEOUT(args.mInput_StayModeUsingTimeMinValue) == true) {
            mStayModeUsingTimeMinValue = kTimeOut;
        } else {
            mStayModeUsingTimeMinValue = args.mInput_StayModeUsingTimeMinValue;
        }
        updateConstantStayModeUsingTimeMin();
    }

    void onStayModePowerValueChanged(const ArgumentsStayModePowerValueChanged& args) {
        if (ISTIMEOUT(args.mInput_StayModePowerValue) == true) {
            mStayModePowerValue = kTimeOut;
        } else {
            mStayModePowerValue = args.mInput_StayModePowerValue;
        }
        updateConstantStayModePower();
    }

    void onStayModeStatusChanged(const ArgumentsStayModeStatusChanged& args) {
        mStayModeStatus = args.mInput_StayModeStatus;
        updateConstantStayModeForConnect();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
    }

    void onSbwWarnStatusFromSCUChanged(const ArgumentsSbwWarnStatusFromSCUChanged& args) {
        mSbwWarnStatusFromSCU = args.mInput_SbwWarnStatusFromSCU;
        updateEventStayModeShiftingPopup();
    }

    void onStayModeChargeControlStatusChanged(const ArgumentsStayModeChargeControlStatusChanged& args) {
        mStayModeChargeControlStatus = args.mInput_StayModeChargeControlStatus;
        updateEventStayModeChargeControl();
    }

    void onSocRealValueChanged(const ArgumentsSocRealValueChanged& args) {
        mSocRealValue = args.mInput_SocRealValue;
        updateConstantCurrentRealSocValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleUtilityModeLmpStat() {
        SFCUtility_ModeTelltaleUtilityModeLmpStat stat = SFCUtility_ModeTelltaleUtilityModeLmpStat::OFF;

        if (mIgnitionElapsed == IgnitionElapsed::ON_500ms && mEvUtilModeStatus == EvUtilModeStatus::ON) {
            stat = SFCUtility_ModeTelltaleUtilityModeLmpStat::ON;
        }
        setSFCUtility_ModeTelltaleUtilityModeLmpStat(stat);
        mTelltaleUtilityModeLmpStat = stat;
        updateEventUtilityMode();
    }

    void updateEventUtilityMode() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true && mTelltaleUtilityModeLmpStat == SFCUtility_ModeTelltaleUtilityModeLmpStat::ON) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                eventID = "E25308";
            } else {
                eventID = "E25301";
            }
        }

        GETCACHEDVALUE(SFC.Utility_Mode.Event.UtilityMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCUtility_ModeEventUtilityModeStat(SFCUtility_ModeEventUtilityModeStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCUtility_ModeEventUtilityModeID(eventID);
            setSFCUtility_ModeEventUtilityModeStat(SFCUtility_ModeEventUtilityModeStat::ON);
        }
    }

    void updateConstantStayModeUsingTimeMin() {
        SFCUtility_ModeConstantStayModeUsingTimeMinStat stat = SFCUtility_ModeConstantStayModeUsingTimeMinStat::OFF;
        HUInt64 value = 0;
        if (mStayModeUsingTimeMinValue == 0 || mStayModeUsingTimeMinValue == 65535) {
            stat = SFCUtility_ModeConstantStayModeUsingTimeMinStat::ON_UNMEASURABLE;
        } else if (mStayModeUsingTimeMinValue >= 1 && mStayModeUsingTimeMinValue <= 59999) {
            stat = SFCUtility_ModeConstantStayModeUsingTimeMinStat::ON;
            value = mStayModeUsingTimeMinValue;
        } else if (mStayModeUsingTimeMinValue >= 60000 && mStayModeUsingTimeMinValue <= 65534) {
            stat = SFCUtility_ModeConstantStayModeUsingTimeMinStat::ON;
            value = 59999;
        } else {
            // no operation
        }
        setSFCUtility_ModeConstantStayModeUsingTimeMinStat(stat);
        setSFCUtility_ModeConstantStayModeUsingTimeMinValue(value);
    }

    void updateConstantStayModePower() {
        SFCUtility_ModeConstantStayModePowerStat stat = SFCUtility_ModeConstantStayModePowerStat::DISPLAY_OFF;
        HDouble value = 0;
        if (mStayModePowerValue >= 0 && mStayModePowerValue <= 255) {
            stat = SFCUtility_ModeConstantStayModePowerStat::ON;
            value = static_cast<HDouble>(mStayModePowerValue) * 0.1;
            value *= kFactor_PosiToNeg;
        }
        setSFCUtility_ModeConstantStayModePowerStat(stat);
        setSFCUtility_ModeConstantStayModePowerValue(value);
    }

    void updateConstantCurrentRealSocValue() {
        SFCUtility_ModeConstantCurrentRealSocStat stat = SFCUtility_ModeConstantCurrentRealSocStat::UNMEASURABLE;
        HUInt64 value = 0;
        if (mSocRealValue >= 0 && mSocRealValue <= 200) {
            stat = SFCUtility_ModeConstantCurrentRealSocStat::VALUE;
            value = static_cast<HUInt64>(floor(static_cast<HDouble>(mSocRealValue) * 0.5));
        }
        setSFCUtility_ModeConstantCurrentRealSocStat(stat);
        setSFCUtility_ModeConstantCurrentRealSocValue(value);
    }

    void updateConstantUtilityModeForConnect() {
        SFCUtility_ModeConstantUtilityModeForConnectStat stat = SFCUtility_ModeConstantUtilityModeForConnectStat::OFF;

        if (mEvUtilModeStatus == EvUtilModeStatus::ON) {
            stat = SFCUtility_ModeConstantUtilityModeForConnectStat::ON;
        }
        setSFCUtility_ModeConstantUtilityModeForConnectStat(stat);
    }

    void updateConstantStayModeForConnect() {
        SFCUtility_ModeConstantStayModeForConnectStat stat = SFCUtility_ModeConstantStayModeForConnectStat::OFF;

        if (mStayModeStatus == StayModeStatus::ON_NORMAL || mStayModeStatus == StayModeStatus::ON_HOLD ||
            mStayModeStatus == StayModeStatus::ON_ENGINE) {
            stat = SFCUtility_ModeConstantStayModeForConnectStat::ON;
        }
        setSFCUtility_ModeConstantStayModeForConnectStat(stat);
    }

    // 5.3.2 StayMode의 event팝업이 사양상 1개의 표로 묶여 있지만,
    // 문의한 결과 진입 팝업과 종료 팝업이 동시에 송출될 수 있다는 답변을 받아 함수를 4개로 분리하였습니다. (ESTRACK20-122 참조)
    // functions : updateEventStayModeFullRangePopup, updateEventStayModeEndPopup, updateEventStayModeShiftingPopup,
    // updateEventStayModeChargeControl
    void updateEventStayModeFullRangePopup() {
        SFCUtility_ModeEventStayModeFullRangePopupStat stat = SFCUtility_ModeEventStayModeFullRangePopupStat::OFF;
        if (mIsIgnOn == true && (mStayModeStatus == StayModeStatus::ON_NORMAL || mStayModeStatus == StayModeStatus::ON_HOLD ||
                                 mStayModeStatus == StayModeStatus::ON_ENGINE)) {
            stat = SFCUtility_ModeEventStayModeFullRangePopupStat::ON;
        }
        setSFCUtility_ModeEventStayModeFullRangePopupStat(stat);
    }

    void updateEventStayModeEndPopup() {
        std::string eventID;
        std::string linkedSoundID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            switch (mStayModeStatus) {
                case StayModeStatus::ON_HOLD:
                    eventID = "E25303";
                    break;
                case StayModeStatus::ON_ENGINE:
                    eventID = "E25304";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.Utility_Mode.Event.StayModeEndPopup.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCUtility_ModeEventStayModeEndPopupStat(SFCUtility_ModeEventStayModeEndPopupStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCUtility_ModeEventStayModeEndPopupID(eventID);
            setSFCUtility_ModeEventStayModeEndPopupStat(SFCUtility_ModeEventStayModeEndPopupStat::ON);
        }
    }

    void updateEventStayModeShiftingPopup() {
        SFCUtility_ModeEventStayModeShiftingPopupStat stat = SFCUtility_ModeEventStayModeShiftingPopupStat::OFF;
        if (mIsIgnOn == true && mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::STAY_MODE) {
            stat = SFCUtility_ModeEventStayModeShiftingPopupStat::ON;
        }
        setSFCUtility_ModeEventStayModeShiftingPopupStat(stat);
    }

    void updateEventStayModeChargeControl() {
        SFCUtility_ModeEventStayModeChargeControlStat stat = SFCUtility_ModeEventStayModeChargeControlStat::OFF;
        if (mIsIgnOn == true && mStayModeChargeControlStatus == StayModeChargeControlStatus::ON) {
            stat = SFCUtility_ModeEventStayModeChargeControlStat::ON;
        }
        setSFCUtility_ModeEventStayModeChargeControlStat(stat);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using EvUtilModeStatus = ArgumentsEvUtilModeStatusChanged::Input_EvUtilModeStatus;
    using StayModeStatus = ArgumentsStayModeStatusChanged::Input_StayModeStatus;
    using SbwWarnStatusFromSCU = ArgumentsSbwWarnStatusFromSCUChanged::Input_SbwWarnStatusFromSCU;
    using StayModeChargeControlStatus = ArgumentsStayModeChargeControlStatusChanged::Input_StayModeChargeControlStatus;

    HBool mIsIgnOn = false;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    SFCUtility_ModeTelltaleUtilityModeLmpStat mTelltaleUtilityModeLmpStat = SFCUtility_ModeTelltaleUtilityModeLmpStat::OFF;
    EvUtilModeStatus mEvUtilModeStatus = EvUtilModeStatus::OFF;
    HUInt64 mStayModeUsingTimeMinValue = 0;
    HUInt64 mStayModePowerValue = 0;
    StayModeStatus mStayModeStatus = StayModeStatus::OFF;
    SbwWarnStatusFromSCU mSbwWarnStatusFromSCU = SbwWarnStatusFromSCU::OFF;
    StayModeChargeControlStatus mStayModeChargeControlStatus = StayModeChargeControlStatus::OFF;
    HUInt64 mSocRealValue = 0;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
    // Factor_PositiveToNegative
    static constexpr HDouble kFactor_PosiToNeg = -1;
};

}  // namespace ccos

#endif  // SFSS_Utility_Mode_H
