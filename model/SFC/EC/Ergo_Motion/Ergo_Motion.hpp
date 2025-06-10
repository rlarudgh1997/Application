/**
 * @file Ergo_Motion.hpp
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
#ifndef SFSS_Ergo_Motion_H
#define SFSS_Ergo_Motion_H

#define DLOG_ENABLED gEnableSFCLog

#include "Ergo_MotionBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [EC040] Ergo_Motion. Contains : Event. Recommend: VALUE_CHANGED
class Ergo_Motion : public Ergo_MotionBase {
public:
    Ergo_Motion() = default;
    ~Ergo_Motion() override = default;
    Ergo_Motion(const Ergo_Motion& other) = delete;
    Ergo_Motion(Ergo_Motion&& other) noexcept = delete;
    Ergo_Motion& operator=(const Ergo_Motion& other) = delete;
    Ergo_Motion& operator=(Ergo_Motion&& other) noexcept = delete;

    void onInitialize() override {
        setSFCErgo_MotionEventErgo_MotionID("E60301");
        setSFCErgo_MotionEventErgo_MotionLinkedSoundID("SND_PopUpInform1");
        setSFCErgo_MotionEventErgo_MotionLinkedSoundType(SFCErgo_MotionEventErgo_MotionLinkedSoundType::REPEAT_COUNT);
        setSFCErgo_MotionEventErgo_MotionLinkedSoundRepeatCount(1);
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

    void onErgoMotionStatusChanged(const ArgumentsErgoMotionStatusChanged& args) {
        mErgoMotionStatus = args.mInput_SeatBodySupportFuncDisplayReqStatus;
        updateEvent();
    }

    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mErgoMotionStatus == ErgoMotionStatus::ON) {
            setSFCErgo_MotionEventErgo_MotionStat(SFCErgo_MotionEventErgo_MotionStat::ON);
        } else {
            setSFCErgo_MotionEventErgo_MotionStat(SFCErgo_MotionEventErgo_MotionStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using ErgoMotionStatus = ArgumentsErgoMotionStatusChanged::Input_SeatBodySupportFuncDisplayReqStatus;
    ErgoMotionStatus mErgoMotionStatus = ErgoMotionStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Ergo_Motion_H
