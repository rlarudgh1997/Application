/**
 * @file Active_Air_Flap.hpp
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
#ifndef SFSS_Active_Air_Flap_H
#define SFSS_Active_Air_Flap_H

#define DLOG_ENABLED gEnableSFCLog

#include "Active_Air_FlapBase.hpp"

namespace ccos {

// SFC Version : 1.0.1
// Reference : [CH020] ACTIVE AIR FLAP. Contains : Event. Recommend: VALUE_CHANGED
class Active_Air_Flap : public Active_Air_FlapBase {
public:
    Active_Air_Flap() = default;
    ~Active_Air_Flap() override = default;
    Active_Air_Flap(const Active_Air_Flap& other) = delete;
    Active_Air_Flap(Active_Air_Flap&& other) noexcept = delete;
    Active_Air_Flap& operator=(const Active_Air_Flap& other) = delete;
    Active_Air_Flap& operator=(Active_Air_Flap&& other) noexcept = delete;

    void onInitialize() override {
        setSFCActive_Air_FlapEventActive_Air_FlapID("E30101");
        setSFCActive_Air_FlapEventActive_Air_FlapLinkedSoundID("SND_PopUpWarn1");
        setSFCActive_Air_FlapEventActive_Air_FlapLinkedSoundRepeatCount(1);
        setSFCActive_Air_FlapEventActive_Air_FlapLinkedSoundType(
            SFCActive_Air_FlapEventActive_Air_FlapLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onEventChanged(const ArgumentsEventChanged& args) {
        mEventChangedStat = args.mInput_AafWarnStatus;
        mEventMCU1Stat = args.mInput_AafWarnStatusFromMCU1;
        mEventMCU2Stat = args.mInput_AafWarnStatusFromMCU2;
        updateDifferentEventChanged();
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateDifferentEventChanged();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateDifferentEventChanged();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

    void updateDifferentEventChanged() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            (mEventChangedStat == EventChangedStat::ON || mEventMCU1Stat == EventMCU1ChangedStat::ON ||
             mEventMCU2Stat == EventMCU2ChangedStat::ON)) {
            setSFCActive_Air_FlapEventActive_Air_FlapStat(SFCActive_Air_FlapEventActive_Air_FlapStat::ON);
            return;
        }
        setSFCActive_Air_FlapEventActive_Air_FlapStat(SFCActive_Air_FlapEventActive_Air_FlapStat::OFF);
    }

private:
    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using EventChangedStat = ArgumentsEventChanged::Input_AafWarnStatus;
    using EventMCU1ChangedStat = ArgumentsEventChanged::Input_AafWarnStatusFromMCU1;
    using EventMCU2ChangedStat = ArgumentsEventChanged::Input_AafWarnStatusFromMCU2;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    EventChangedStat mEventChangedStat = EventChangedStat::OFF;
    EventMCU1ChangedStat mEventMCU1Stat = EventMCU1ChangedStat::OFF;
    EventMCU2ChangedStat mEventMCU2Stat = EventMCU2ChangedStat::OFF;
};

}  // namespace ccos

#endif  // SFSS_Active_Air_Flap_H
