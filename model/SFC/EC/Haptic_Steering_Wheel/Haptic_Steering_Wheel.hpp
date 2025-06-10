/**
 * @file Haptic_Steering_Wheel.hpp
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
#ifndef SFSS_Haptic_Steering_Wheel_H
#define SFSS_Haptic_Steering_Wheel_H

#define DLOG_ENABLED gEnableSFCLog

#include "Haptic_Steering_WheelBase.hpp"

namespace ccos {

// SFC Version : 1.1.0
// Reference : [EC050] Haptic_Steering_Wheel. Contains : Event. Recommend: VALUE_CHANGED
class Haptic_Steering_Wheel : public Haptic_Steering_WheelBase {
public:
    Haptic_Steering_Wheel() = default;
    ~Haptic_Steering_Wheel() override = default;

    void onInitialize() override {
        setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelID("E60401");
        setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelLinkedSoundID("SND_PopUpWarn1");
        setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelLinkedSoundType(
            SFCHaptic_Steering_WheelEventHaptic_Steering_WheelLinkedSoundType::REPEAT_COUNT);
        setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onHapticSteeringSysFailStatus(const ArgumentsHapticSteeringSysFailStatus& args) {
        mHapticSteeringSysFailStatusFromICU = args.mInput_HapticSteeringSysFailStatusFromICU;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mHapticSteeringSysFailStatusFromICU == HapticSteeringSysFailStatusFromICU::FAIL &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelStat(SFCHaptic_Steering_WheelEventHaptic_Steering_WheelStat::ON);
        } else {
            setSFCHaptic_Steering_WheelEventHaptic_Steering_WheelStat(
                SFCHaptic_Steering_WheelEventHaptic_Steering_WheelStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using HapticSteeringSysFailStatusFromICU = ArgumentsHapticSteeringSysFailStatus::Input_HapticSteeringSysFailStatusFromICU;
    HapticSteeringSysFailStatusFromICU mHapticSteeringSysFailStatusFromICU = HapticSteeringSysFailStatusFromICU::FAIL;
};

}  // namespace ccos

#endif  // SFSS_Haptic_Steering_Wheel_H
