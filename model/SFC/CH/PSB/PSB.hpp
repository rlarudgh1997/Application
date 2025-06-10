/**
 * @file PSB.hpp
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
#ifndef SFSS_PSB_H
#define SFSS_PSB_H

#define DLOG_ENABLED gEnableSFCLog

#include "PSBBase.hpp"

namespace ccos {

// SFC Version : 0.1.1
// Reference : [CH140] PSB. Contains : Event. Recommend: VALUE_CHANGED
class PSB : public PSBBase {
public:
    PSB() = default;
    ~PSB() override = default;

    void onInitialize() override {
        setSFCPSBEventPSBFailWarningID("E31001");
        setSFCPSBEventPSBFailWarningStat(SFCPSBEventPSBFailWarningStat::OFF);
        setSFCPSBEventPSBFailWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCPSBEventPSBFailWarningLinkedSoundType(SFCPSBEventPSBFailWarningLinkedSoundType::REPEAT_COUNT);
        setSFCPSBEventPSBFailWarningLinkedSoundRepeatCount(1);
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

    void onConfigPSBChanged(const ArgumentsConfigPSBChanged& args) {
        mConfigPSB = args.mInter_ConfigPSB;
        updateEvent();
    }

    void onFailStatusChanged(const ArgumentsFailStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PsbRhFailStatus) == true) {
            mPsbRhFailStatus = PsbRhFailStatus::UNHANDLED_TIMEOUT;
        } else {
            mPsbRhFailStatus = args.mInput_PsbRhFailStatus;
        }
        if (ISTIMEOUT(args.mInput_PsbLhFailStatus) == true) {
            mPsbLhFailStatus = PsbLhFailStatus::UNHANDLED_TIMEOUT;
        } else {
            mPsbLhFailStatus = args.mInput_PsbLhFailStatus;
        }
        updateEvent();
    }

private:
    void updateEvent() {
        if (mConfigPSB == ConfigPSB::ON && mIgnElapsed == IgnElapsed::ON_0ms &&
            (mPsbRhFailStatus != PsbRhFailStatus::NORMAL || mPsbLhFailStatus != PsbLhFailStatus::NORMAL)) {
            setSFCPSBEventPSBFailWarningStat(SFCPSBEventPSBFailWarningStat::ON);
            setSFCPSBEventPSBFailWarningLinkedSoundRepeatCount(1);
        } else {
            setSFCPSBEventPSBFailWarningStat(SFCPSBEventPSBFailWarningStat::OFF);
            setSFCPSBEventPSBFailWarningLinkedSoundRepeatCount(0);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigPSB = ArgumentsConfigPSBChanged::Inter_ConfigPSB;
    using PsbRhFailStatus = ArgumentsFailStatusChanged::Input_PsbRhFailStatus;
    using PsbLhFailStatus = ArgumentsFailStatusChanged::Input_PsbLhFailStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigPSB mConfigPSB = ConfigPSB::OFF;
    PsbRhFailStatus mPsbRhFailStatus = PsbRhFailStatus::NORMAL;
    PsbLhFailStatus mPsbLhFailStatus = PsbLhFailStatus::NORMAL;
};

}  // namespace ccos

#endif  // SFSS_PSB_H
