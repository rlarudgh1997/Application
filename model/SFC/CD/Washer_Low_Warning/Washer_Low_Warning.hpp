/**
 * @file Washer_Low_Warning.hpp
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
#ifndef SFSS_Washer_Low_Warning_H
#define SFSS_Washer_Low_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Washer_Low_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 2.1.5
// Reference : [CD200] Washer Low Warning. Contains : Event. Recommend: VALUE_CHANGED
class Washer_Low_Warning : public Washer_Low_WarningBase {
public:
    Washer_Low_Warning() = default;
    ~Washer_Low_Warning() override = default;
    Washer_Low_Warning(const Washer_Low_Warning& other) = delete;
    Washer_Low_Warning(Washer_Low_Warning&& other) noexcept = delete;
    Washer_Low_Warning& operator=(const Washer_Low_Warning& other) = delete;
    Washer_Low_Warning& operator=(Washer_Low_Warning&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerLowWasherSwStatus.create(kTimerLowWasherSwStatusInterval10s, this,
                                                  &Washer_Low_Warning::onOneShotTimerLowWasherSwStatusChanged, false) == false) {
            DWarning() << "[Washer_Low_Warning] mOneShotTimerLowWasherSwStatus Create Failed";
        }
        setSFCWasher_Low_WarningEventWasher_Low_WarningID("E00301");
        setSFCWasher_Low_WarningEventWasher_Low_WarningLinkedSoundID("SND_PopUpInform2");
        setSFCWasher_Low_WarningEventWasher_Low_WarningLinkedSoundRepeatCount(1);
        setSFCWasher_Low_WarningEventWasher_Low_WarningLinkedSoundType(
            SFCWasher_Low_WarningEventWasher_Low_WarningLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateWasherLowWarningStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateWasherLowWarningStat();
    }

    void onWasherLowWarningChanged(const ArgumentsWasherLowWarningChanged& args) {
        mLowWasherWarnCANStatus = args.mInput_LowWasherWarnCANStatus;
        updateWasherLowWarningStat();
    }

private:
    void onOneShotTimerLowWasherSwStatusChanged() {
        setSFCWasher_Low_WarningEventWasher_Low_WarningStat(SFCWasher_Low_WarningEventWasher_Low_WarningStat::ON);
        flush();
    }

    void updateWasherLowWarningStat() {
        mOneShotTimerLowWasherSwStatus.stop();

        if (mLowWasherWarnCANStatus == LowWasherWarnCANStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOneShotTimerLowWasherSwStatus.start() == false) {
                DWarning() << "[Washer_Low_Warning] mOneShotTimerLowWasherSwStatus Start Failed";
            }
        } else {
            setSFCWasher_Low_WarningEventWasher_Low_WarningStat(SFCWasher_Low_WarningEventWasher_Low_WarningStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using LowWasherWarnCANStatus = ArgumentsWasherLowWarningChanged::Input_LowWasherWarnCANStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    LowWasherWarnCANStatus mLowWasherWarnCANStatus = LowWasherWarnCANStatus::OFF;

    static constexpr uint32_t kTimerLowWasherSwStatusInterval10s = 10000u;
    ssfs::SFCTimer<Washer_Low_Warning> mOneShotTimerLowWasherSwStatus;
};

}  // namespace ccos

#endif  // SFSS_Washer_Low_Warning_H
