/**
 * @file LVA.hpp
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
#ifndef SFSS_LVA_H
#define SFSS_LVA_H

#define DLOG_ENABLED gEnableSFCLog

#include "LVABase.hpp"

namespace ccos {

// SFC Version : 0.1.0
// Reference : [CH110] LVA. Contains : Event. Recommend: VALUE_CHANGED
class LVA : public LVABase {
public:
    LVA() = default;
    ~LVA() override = default;
    LVA(const LVA& other) = delete;
    LVA(LVA&& other) noexcept = delete;
    LVA& operator=(const LVA& other) = delete;
    LVA& operator=(LVA&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLVAEventLowVacuumAssistFailID("E30801");
        setSFCLVAEventLowVacuumAssistFailLinkedSoundID("SND_PopUpWarn1");
        setSFCLVAEventLowVacuumAssistFailLinkedSoundType(SFCLVAEventLowVacuumAssistFailLinkedSoundType::REPEAT_COUNT);
        setSFCLVAEventLowVacuumAssistFailLinkedSoundRepeatCount(1);
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

    void onAssistFailChanged(const ArgumentsAssistFailChanged& args) {
        mLowVacuumAssistFailStatus = args.mInput_LowVacuumAssistFailStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mLowVacuumAssistFailStatus == LowVacuumAssistFailStatus::FAILURE) {
            setSFCLVAEventLowVacuumAssistFailStat(SFCLVAEventLowVacuumAssistFailStat::ON);
        } else {
            setSFCLVAEventLowVacuumAssistFailStat(SFCLVAEventLowVacuumAssistFailStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using LowVacuumAssistFailStatus = ArgumentsAssistFailChanged::Input_LowVacuumAssistFailStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    LowVacuumAssistFailStatus mLowVacuumAssistFailStatus = LowVacuumAssistFailStatus::NORMAL;
};

}  // namespace ccos

#endif  // SFSS_LVA_H
