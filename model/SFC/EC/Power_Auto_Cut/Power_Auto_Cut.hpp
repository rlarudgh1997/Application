/**
 * @file Power_Auto_Cut.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2021  Hyundai Motor Company,
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
#ifndef SFSS_Power_Auto_Cut_H
#define SFSS_Power_Auto_Cut_H

#define DLOG_ENABLED gEnableSFCLog

#include "Power_Auto_CutBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [EC130] Power_Auto_Cut. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Power_Auto_Cut : public Power_Auto_CutBase {
public:
    Power_Auto_Cut() = default;
    ~Power_Auto_Cut() override = default;
    Power_Auto_Cut(const Power_Auto_Cut& other) = delete;
    Power_Auto_Cut(Power_Auto_Cut&& other) noexcept = delete;
    Power_Auto_Cut& operator=(const Power_Auto_Cut& other) = delete;
    Power_Auto_Cut& operator=(Power_Auto_Cut&& other) noexcept = delete;

    void onInitialize() override {
        setSFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusID("E61103");
        setSFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusLinkedSoundID("SND_PopUpInform2");
        setSFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusLinkedSoundType(
            SFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusLinkedSoundType::REPEAT_COUNT);
        setSFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateBatterySocDisplayStatus();
        updateWarningEvent();
        updateIgnitionAutoOffWarningReqEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateBatterySocDisplayStatus();
        updateWarningEvent();
        updateIgnitionAutoOffWarningReqEvent();
    }

    void onPowerAutoCutModeChanged(const ArgumentsPowerAutoCutModeChanged& args) {
        mPowerAutoCutState = args.mInput_PowerAutoCutModeStatus;
        updateWarningEvent();
    }

    void onBatterySocValueChanged(const ArgumentsBatterySocValueChanged& args) {
        mInput_BatterySocValueFromPDC = args.mInput_BatterySocValueFromPDC;
        mInput_BatterySocValueFromBMS = args.mInput_BatterySocValueFromBMS;
        updateBatterySocDisplayStatus();
    }

    void onIgnitionAutoOffWarnReqStatusChanged(const ArgumentsIgnitionAutoOffWarnReqStatusChanged& args) {
        mIgnitionAutoOffWarnReqStatus = args.mInput_IgnitionAutoOffWarnReqStatus;
        updateIgnitionAutoOffWarningReqEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateBatterySocDisplayStatus() {
        SFCPower_Auto_CutConstantBatterySOCStat stat = SFCPower_Auto_CutConstantBatterySOCStat::DISPLAY_DASH;
        HUInt64 socValue = 0;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (1 <= mInput_BatterySocValueFromPDC && mInput_BatterySocValueFromPDC <= 100) {
                socValue = mInput_BatterySocValueFromPDC;
                stat = SFCPower_Auto_CutConstantBatterySOCStat::DISPLAY_VALUE;
            } else if (1 <= mInput_BatterySocValueFromBMS && mInput_BatterySocValueFromBMS <= 100) {
                socValue = mInput_BatterySocValueFromBMS;
                stat = SFCPower_Auto_CutConstantBatterySOCStat::DISPLAY_VALUE;
            } else {
                // no operation
            }
        }
        setSFCPower_Auto_CutConstantBatterySOCStat(stat);
        setSFCPower_Auto_CutConstantBatterySOCValue(socValue);
    }

    void updateWarningEvent() {
        std::string prevEventID;
        std::string eventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mPowerAutoCutState) {
                case PowerAutoCutModeStatus::FACTORY_MODE:
                    eventID = "E61101";
                    break;
                case PowerAutoCutModeStatus::DEALER_MODE:
                    eventID = "E61102";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.Power_Auto_Cut.Event.DispWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCPower_Auto_CutEventDispWarningStat(SFCPower_Auto_CutEventDispWarningStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCPower_Auto_CutEventDispWarningID(eventID);
            setSFCPower_Auto_CutEventDispWarningStat(SFCPower_Auto_CutEventDispWarningStat::ON);
        }
    }

    void updateIgnitionAutoOffWarningReqEvent() {
        SFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusStat stat = SFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mIgnitionAutoOffWarnReqStatus == IgnitionAutoOffWarnReqStatus::WARN) {
            stat = SFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusStat::ON;
        }
        setSFCPower_Auto_CutEventIgnitionAutoOffWarnReqStatusStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using PowerAutoCutModeStatus = ArgumentsPowerAutoCutModeChanged::Input_PowerAutoCutModeStatus;
    using IgnitionAutoOffWarnReqStatus = ArgumentsIgnitionAutoOffWarnReqStatusChanged::Input_IgnitionAutoOffWarnReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    PowerAutoCutModeStatus mPowerAutoCutState = PowerAutoCutModeStatus::CUSTOMER_MODE;
    IgnitionAutoOffWarnReqStatus mIgnitionAutoOffWarnReqStatus = IgnitionAutoOffWarnReqStatus::OFF;
    HUInt64 mInput_BatterySocValueFromPDC = 0;
    HUInt64 mInput_BatterySocValueFromBMS = 0;
};

}  // namespace ccos

#endif  // SFSS_Power_Auto_Cut_H
