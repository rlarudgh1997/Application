/**
 * @file Active_Hood.hpp
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
#ifndef SFSS_Active_Hood_H
#define SFSS_Active_Hood_H

#define DLOG_ENABLED gEnableSFCLog

#include "Active_HoodBase.hpp"

namespace ccos {

// SFC Version : 0.1.1
// Reference : [CH030] Active_Hood. Contains : Event. Recommend: VALUE_CHANGED
class Active_Hood : public Active_HoodBase {
public:
    Active_Hood() = default;
    ~Active_Hood() override = default;
    Active_Hood(const Active_Hood& other) = delete;
    Active_Hood(Active_Hood&& other) noexcept = delete;
    Active_Hood& operator=(const Active_Hood& other) = delete;
    Active_Hood& operator=(Active_Hood&& other) noexcept = delete;

    void onInitialize() override {
        setSFCActive_HoodEventActiveHoodWarningID("E30201");
        setSFCActive_HoodEventActiveHoodWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCActive_HoodEventActiveHoodWarningLinkedSoundType(SFCActive_HoodEventActiveHoodWarningLinkedSoundType::REPEAT_COUNT);
        setSFCActive_HoodEventActiveHoodWarningLinkedSoundRepeatCount(1);
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

    void onConfigAHLSChanged(const ArgumentsConfigAHLSChanged& args) {
        mConfigAHLS = args.mInter_ConfigAHLS;
        updateEvent();
    }

    void onActiveHoodWarnChanged(const ArgumentsActiveHoodWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_ActiveHoodWarnStatus) == true) {
            mActiveHoodWarnStatus = ActiveHoodWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mActiveHoodWarnStatus = args.mInput_ActiveHoodWarnStatus;
        }
        updateEvent();
    }

private:
    void updateEvent() {
        if (mConfigAHLS == ConfigAHLS::ON && mIgnElapsed == IgnElapsed::ON_0ms &&
            (mActiveHoodWarnStatus == ActiveHoodWarnStatus::ON ||
             mActiveHoodWarnStatus == ActiveHoodWarnStatus::UNHANDLED_TIMEOUT)) {
            setSFCActive_HoodEventActiveHoodWarningStat(SFCActive_HoodEventActiveHoodWarningStat::ON);
        } else {
            setSFCActive_HoodEventActiveHoodWarningStat(SFCActive_HoodEventActiveHoodWarningStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigAHLS = ArgumentsConfigAHLSChanged::Inter_ConfigAHLS;
    using ActiveHoodWarnStatus = ArgumentsActiveHoodWarnChanged::Input_ActiveHoodWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigAHLS mConfigAHLS = ConfigAHLS::OFF;
    ActiveHoodWarnStatus mActiveHoodWarnStatus = ActiveHoodWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Active_Hood_H
