/**
 * @file Emission_Test_Mode.hpp
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
#ifndef SFSS_Emission_Test_Mode_H
#define SFSS_Emission_Test_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Emission_Test_ModeBase.hpp"

namespace ccos {

// SFC Version : 0.0.2
// Reference : [PT110] Emission_Test_Mode. Contains : Event. Recommend: VALUE_CHANGED
class Emission_Test_Mode : public Emission_Test_ModeBase {
public:
    Emission_Test_Mode() = default;
    ~Emission_Test_Mode() override = default;
    Emission_Test_Mode(const Emission_Test_Mode& other) = delete;
    Emission_Test_Mode(Emission_Test_Mode&& other) noexcept = delete;
    Emission_Test_Mode& operator=(const Emission_Test_Mode& other) = delete;
    Emission_Test_Mode& operator=(Emission_Test_Mode&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEmission_Test_ModeEventHevEmissionTestModeID("E20601");
        setSFCEmission_Test_ModeEventHevEmissionTestModeStat(SFCEmission_Test_ModeEventHevEmissionTestModeStat::OFF);
        setSFCEmission_Test_ModeEventHevEmissionTestModeLinkedSoundID("SND_PopUpInform2");
        setSFCEmission_Test_ModeEventHevEmissionTestModeLinkedSoundType(
            SFCEmission_Test_ModeEventHevEmissionTestModeLinkedSoundType::REPEAT_COUNT);
        setSFCEmission_Test_ModeEventHevEmissionTestModeLinkedSoundRepeatCount(1);
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

    void onHevEmissionTestModeChanged(const ArgumentsHevEmissionTestModeChanged& args) {
        mHevEmissionTestModeStatus = args.mInput_HevEmissionTestModeStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mHevEmissionTestModeStatus == HevEmissionTestModeStatus::ON) {
            setSFCEmission_Test_ModeEventHevEmissionTestModeStat(SFCEmission_Test_ModeEventHevEmissionTestModeStat::ON);
        } else {
            setSFCEmission_Test_ModeEventHevEmissionTestModeStat(SFCEmission_Test_ModeEventHevEmissionTestModeStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using HevEmissionTestModeStatus = ArgumentsHevEmissionTestModeChanged::Input_HevEmissionTestModeStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HevEmissionTestModeStatus mHevEmissionTestModeStatus = HevEmissionTestModeStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Emission_Test_Mode_H
