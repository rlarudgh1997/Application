/**
 * @file RANC.hpp
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
#ifndef SFSS_RANC_H
#define SFSS_RANC_H

#define DLOG_ENABLED gEnableSFCLog

#include "RANCBase.hpp"

namespace ccos {

// SFC Version : 1.1.0
// Reference : [AV050] RANC. Contains : Event. Recommend: VALUE_CHANGED
class RANC : public RANCBase {
public:
    RANC() = default;
    ~RANC() override = default;
    RANC(const RANC& other) = delete;
    RANC(RANC&& other) noexcept = delete;
    RANC& operator=(const RANC& other) = delete;
    RANC& operator=(RANC&& other) noexcept = delete;

    void onInitialize() override {
        setSFCRANCEventRancWarnID("E40501");
        setSFCRANCEventRancWarnStat(SFCRANCEventRancWarnStat::OFF);
        setSFCRANCEventRancWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCRANCEventRancWarnLinkedSoundType(SFCRANCEventRancWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRANCEventRancWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onRancWarnChanged(const ArgumentsRancWarnChanged& args) {
        mRancWarnStatus = args.mInput_RancWarnStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mRancWarnStatus == RancWarnStatus::ON) {
            setSFCRANCEventRancWarnStat(SFCRANCEventRancWarnStat::ON);
        } else {
            setSFCRANCEventRancWarnStat(SFCRANCEventRancWarnStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using RancWarnStatus = ArgumentsRancWarnChanged::Input_RancWarnStatus;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    RancWarnStatus mRancWarnStatus = RancWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_RANC_H
