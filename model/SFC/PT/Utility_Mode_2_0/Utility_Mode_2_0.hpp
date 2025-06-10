/**
 * @file Utility_Mode_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Utility_Mode_2_0_H
#define SFSS_Utility_Mode_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Utility_Mode_2_0Base.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT491] Utility_Mode_2_0. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Utility_Mode_2_0 : public Utility_Mode_2_0Base {
public:
    Utility_Mode_2_0() = default;
    ~Utility_Mode_2_0() override = default;
    Utility_Mode_2_0(const Utility_Mode_2_0& other) = delete;
    Utility_Mode_2_0(Utility_Mode_2_0&& other) noexcept = delete;
    Utility_Mode_2_0& operator=(const Utility_Mode_2_0& other) = delete;
    Utility_Mode_2_0& operator=(Utility_Mode_2_0&& other) noexcept = delete;

    void onInitialize() override {
        // Utility Mode popup
        setSFCUtility_Mode_2_0EventUtilityModeID("E25301");
        // Stay Mode Full Range Popup
        setSFCUtility_Mode_2_0EventStayModeFullRangePopupID("E25302");
        // Stay Mode End Popup
        setSFCUtility_Mode_2_0EventStayModeEndPopupLinkedSoundID("SND_PopUpWarn1");
        setSFCUtility_Mode_2_0EventStayModeEndPopupLinkedSoundType(
            SFCUtility_Mode_2_0EventStayModeEndPopupLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_Mode_2_0EventStayModeEndPopupLinkedSoundRepeatCount(1);
        // Stay Mode Shifting Popup
        setSFCUtility_Mode_2_0EventStayModeShiftingPopupID("E25305");
        setSFCUtility_Mode_2_0EventStayModeShiftingPopupLinkedSoundID("SND_PopUpWarn1");
        setSFCUtility_Mode_2_0EventStayModeShiftingPopupLinkedSoundType(
            SFCUtility_Mode_2_0EventStayModeShiftingPopupLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_Mode_2_0EventStayModeShiftingPopupLinkedSoundRepeatCount(1);
        // Stay Mode Charge Control
        setSFCUtility_Mode_2_0EventStayModeChargeControlID("E25306");
        setSFCUtility_Mode_2_0EventStayModeChargeControlLinkedSoundID("SND_PopUpInform2");
        setSFCUtility_Mode_2_0EventStayModeChargeControlLinkedSoundType(
            SFCUtility_Mode_2_0EventStayModeChargeControlLinkedSoundType::REPEAT_COUNT);
        setSFCUtility_Mode_2_0EventStayModeChargeControlLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        updateUtilityMode();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateUtilityMode();
        updateEventStayModeFullRangePopup();
        updateEventStayModeEndPopup();
        updateEventStayModeShiftingPopup();
        updateEventStayModeChargeControl();
    }

    void onEvUtilModeStatusChanged(const ArgumentsEvUtilModeStatusChanged& args) {
        mEvUtilModeStatus = args.mInput_EvUtilModeStatus;
        updateUtilityMode();
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

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateUtilityMode() {
        SFCUtility_Mode_2_0TelltaleUtilityModeLmpStat telltaleStat = SFCUtility_Mode_2_0TelltaleUtilityModeLmpStat::OFF;
        SFCUtility_Mode_2_0EventUtilityModeStat eventStat = SFCUtility_Mode_2_0EventUtilityModeStat::OFF;
        if (mIgnitionElapsed == IgnitionElapsed::ON_500ms && mEvUtilModeStatus == EvUtilModeStatus::ON) {
            telltaleStat = SFCUtility_Mode_2_0TelltaleUtilityModeLmpStat::ON;
            eventStat = SFCUtility_Mode_2_0EventUtilityModeStat::ON;
        }
        setSFCUtility_Mode_2_0TelltaleUtilityModeLmpStat(telltaleStat);
        setSFCUtility_Mode_2_0EventUtilityModeStat(eventStat);
    }

    void updateConstantStayModeUsingTimeMin() {
        SFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat stat = SFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat::OFF;
        HUInt64 value = 0;
        if (mStayModeUsingTimeMinValue == 0 || mStayModeUsingTimeMinValue == 65535) {
            stat = SFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat::ON_UNMEASURABLE;
        } else if (mStayModeUsingTimeMinValue >= 1 && mStayModeUsingTimeMinValue <= 59999) {
            stat = SFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat::ON;
            value = mStayModeUsingTimeMinValue;
        } else if (mStayModeUsingTimeMinValue >= 60000 && mStayModeUsingTimeMinValue <= 65534) {
            stat = SFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat::ON;
            value = 59999;
        } else {
            // no operation
        }
        setSFCUtility_Mode_2_0ConstantStayModeUsingTimeMinStat(stat);
        setSFCUtility_Mode_2_0ConstantStayModeUsingTimeMinValue(value);
    }

    void updateConstantStayModePower() {
        SFCUtility_Mode_2_0ConstantStayModePowerStat stat = SFCUtility_Mode_2_0ConstantStayModePowerStat::DISPLAY_OFF;
        HDouble value = 0.0;
        if (mStayModePowerValue >= 0 && mStayModePowerValue <= 255) {
            stat = SFCUtility_Mode_2_0ConstantStayModePowerStat::ON;
            value = static_cast<HDouble>(mStayModePowerValue) * 0.1;
            value *= kFactor_PosiToNeg;
        }
        setSFCUtility_Mode_2_0ConstantStayModePowerStat(stat);
        setSFCUtility_Mode_2_0ConstantStayModePowerValue(value);
    }

    void updateConstantUtilityModeForConnect() {
        SFCUtility_Mode_2_0ConstantUtilityModeForConnectStat stat = SFCUtility_Mode_2_0ConstantUtilityModeForConnectStat::OFF;

        if (mEvUtilModeStatus == EvUtilModeStatus::ON) {
            stat = SFCUtility_Mode_2_0ConstantUtilityModeForConnectStat::ON;
        }
        setSFCUtility_Mode_2_0ConstantUtilityModeForConnectStat(stat);
    }

    void updateConstantStayModeForConnect() {
        SFCUtility_Mode_2_0ConstantStayModeForConnectStat stat = SFCUtility_Mode_2_0ConstantStayModeForConnectStat::OFF;

        if (mStayModeStatus == StayModeStatus::ON_NORMAL || mStayModeStatus == StayModeStatus::ON_HOLD ||
            mStayModeStatus == StayModeStatus::ON_ENGINE) {
            stat = SFCUtility_Mode_2_0ConstantStayModeForConnectStat::ON;
        }
        setSFCUtility_Mode_2_0ConstantStayModeForConnectStat(stat);
    }

    // 5.3.2 StayMode의 event팝업이 사양상 1개의 표로 묶여 있지만,
    // 문의한 결과 진입 팝업과 종료 팝업이 동시에 송출될 수 있다는 답변을 받아 함수를 4개로 분리하였습니다. (ESTRACK20-122 참조)
    // functions : updateEventStayModeFullRangePopup, updateEventStayModeEndPopup, updateEventStayModeShiftingPopup,
    // updateEventStayModeChargeControl
    void updateEventStayModeFullRangePopup() {
        SFCUtility_Mode_2_0EventStayModeFullRangePopupStat stat = SFCUtility_Mode_2_0EventStayModeFullRangePopupStat::OFF;
        if ((mIgnitionElapsed == IgnitionElapsed::ON_0ms || mIgnitionElapsed == IgnitionElapsed::ON_500ms) &&
            (mStayModeStatus == StayModeStatus::ON_NORMAL || mStayModeStatus == StayModeStatus::ON_HOLD ||
             mStayModeStatus == StayModeStatus::ON_ENGINE)) {
            stat = SFCUtility_Mode_2_0EventStayModeFullRangePopupStat::ON;
        }
        setSFCUtility_Mode_2_0EventStayModeFullRangePopupStat(stat);
    }

    void updateEventStayModeEndPopup() {
        std::string eventID;
        std::string linkedSoundID;
        std::string prevEventID;

        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms || mIgnitionElapsed == IgnitionElapsed::ON_500ms) {
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

        GETCACHEDVALUE(SFC.Utility_Mode_2_0.Event.StayModeEndPopup.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCUtility_Mode_2_0EventStayModeEndPopupStat(SFCUtility_Mode_2_0EventStayModeEndPopupStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCUtility_Mode_2_0EventStayModeEndPopupID(eventID);
            setSFCUtility_Mode_2_0EventStayModeEndPopupStat(SFCUtility_Mode_2_0EventStayModeEndPopupStat::ON);
        }
    }

    void updateEventStayModeShiftingPopup() {
        SFCUtility_Mode_2_0EventStayModeShiftingPopupStat stat = SFCUtility_Mode_2_0EventStayModeShiftingPopupStat::OFF;
        if ((mIgnitionElapsed == IgnitionElapsed::ON_0ms || mIgnitionElapsed == IgnitionElapsed::ON_500ms) &&
            mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::STAY_MODE) {
            stat = SFCUtility_Mode_2_0EventStayModeShiftingPopupStat::ON;
        }
        setSFCUtility_Mode_2_0EventStayModeShiftingPopupStat(stat);
    }

    void updateEventStayModeChargeControl() {
        SFCUtility_Mode_2_0EventStayModeChargeControlStat stat = SFCUtility_Mode_2_0EventStayModeChargeControlStat::OFF;
        if ((mIgnitionElapsed == IgnitionElapsed::ON_0ms || mIgnitionElapsed == IgnitionElapsed::ON_500ms) &&
            mStayModeChargeControlStatus == StayModeChargeControlStatus::ON) {
            stat = SFCUtility_Mode_2_0EventStayModeChargeControlStat::ON;
        }
        setSFCUtility_Mode_2_0EventStayModeChargeControlStat(stat);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using EvUtilModeStatus = ArgumentsEvUtilModeStatusChanged::Input_EvUtilModeStatus;
    using StayModeStatus = ArgumentsStayModeStatusChanged::Input_StayModeStatus;
    using SbwWarnStatusFromSCU = ArgumentsSbwWarnStatusFromSCUChanged::Input_SbwWarnStatusFromSCU;
    using StayModeChargeControlStatus = ArgumentsStayModeChargeControlStatusChanged::Input_StayModeChargeControlStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    EvUtilModeStatus mEvUtilModeStatus = EvUtilModeStatus::OFF;
    HUInt64 mStayModeUsingTimeMinValue = 0;
    HUInt64 mStayModePowerValue = 0;
    StayModeStatus mStayModeStatus = StayModeStatus::OFF;
    SbwWarnStatusFromSCU mSbwWarnStatusFromSCU = SbwWarnStatusFromSCU::OFF;
    StayModeChargeControlStatus mStayModeChargeControlStatus = StayModeChargeControlStatus::OFF;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
    // Factor_PositiveToNegative
    static constexpr HDouble kFactor_PosiToNeg = -1.0;
};

}  // namespace ccos

#endif  // SFSS_Utility_Mode_2_0_H
