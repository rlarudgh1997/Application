/**
 * @file Different_Tire_Warning.hpp
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
#ifndef SFSS_Different_Tire_Warning_H
#define SFSS_Different_Tire_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Different_Tire_WarningBase.hpp"

namespace ccos {

// SFC Version : 0.1.0
// Reference :  [CH060] Different_Tire_Warning. Contains : Event. Recommend: VALUE_CHANGED
class Different_Tire_Warning : public Different_Tire_WarningBase {
public:
    Different_Tire_Warning() = default;
    ~Different_Tire_Warning() override = default;
    Different_Tire_Warning(const Different_Tire_Warning& other) = delete;
    Different_Tire_Warning(Different_Tire_Warning&& other) noexcept = delete;
    Different_Tire_Warning& operator=(const Different_Tire_Warning& other) = delete;
    Different_Tire_Warning& operator=(Different_Tire_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDifferent_Tire_WarningEventTireWarningID("E30301");
        setSFCDifferent_Tire_WarningEventTireWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCDifferent_Tire_WarningEventTireWarningLinkedSoundRepeatCount(1);
        setSFCDifferent_Tire_WarningEventTireWarningLinkedSoundType(
            SFCDifferent_Tire_WarningEventTireWarningLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateDifferentWarnStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateDifferentWarnStat();
    }

    void onTireSizeDifferentWarnStatChanged(const ArgumentsTireSizeDifferentWarnStatChanged& args) {
        mTireState = args.mInput_TireSizeDifferentWarnStatusFromAWD;
        updateDifferentWarnStat();
    }

    void onElsdWarnStatusChanged(const ArgumentsElsdWarnStatusChanged& args) {
        mELsdWarnStatus = args.mInput_ELsdWarnStatus;
        updateDifferentWarnStat();
    }

    void updateDifferentWarnStat() {
        if ((mIgnElapsed == IgnElapsed::ON_0ms) &&
            ((mTireState == TireSizeDifferentStatus::ON) || (mELsdWarnStatus == ELsdWarnStatus::DIFF_TIRE))) {
            setSFCDifferent_Tire_WarningEventTireWarningStat(SFCDifferent_Tire_WarningEventTireWarningStat::ON);
        } else {
            setSFCDifferent_Tire_WarningEventTireWarningStat(SFCDifferent_Tire_WarningEventTireWarningStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using TireSizeDifferentStatus = ArgumentsTireSizeDifferentWarnStatChanged::Input_TireSizeDifferentWarnStatusFromAWD;
    using ELsdWarnStatus = ArgumentsElsdWarnStatusChanged::Input_ELsdWarnStatus;

    TireSizeDifferentStatus mTireState = TireSizeDifferentStatus::OFF;
    ELsdWarnStatus mELsdWarnStatus = ELsdWarnStatus::FAIL;

    IgnElapsed mIgnElapsed = IgnElapsed::MAX;
};

}  // namespace ccos

#endif  // SFSS_Different_Tire_Warning_H
