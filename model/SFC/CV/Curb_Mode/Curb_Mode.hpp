/**
 * @file Curb_Mode.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Curb_Mode_H
#define SFSS_Curb_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Curb_ModeBase.hpp"

namespace ccos {

// SFC Version : 0.0.1
// Spec Version : v0.08
// Reference : [CV760] Curb_Mode. Contains : Telltale. Recommend: VALUE_CHANGED
class Curb_Mode : public Curb_ModeBase {
public:
    Curb_Mode() = default;
    ~Curb_Mode() override = default;
    Curb_Mode(const Curb_Mode& other) = delete;
    Curb_Mode(Curb_Mode&& other) noexcept = delete;
    Curb_Mode& operator=(const Curb_Mode& other) = delete;
    Curb_Mode& operator=(Curb_Mode&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void onCurbModeStateChanged(const ArgumentsCurbModeStateChanged& args) {
        mEngineCurbModeState = args.mInput_EngineCurbModeState;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if ((mConfigVehicleType == ConfigVehicleType::ICV && mConfigFuelTypeCV == ConfigFuelTypeCV::DSL) == false) {
            return;
        }
        SFCCurb_ModeTelltaleEngineCurbModeStat telltaleStat = SFCCurb_ModeTelltaleEngineCurbModeStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mEngineCurbModeState == EngineCurbModeState::ON) {
            telltaleStat = SFCCurb_ModeTelltaleEngineCurbModeStat::ON;
        }
        setSFCCurb_ModeTelltaleEngineCurbModeStat(telltaleStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using EngineCurbModeState = ArgumentsCurbModeStateChanged::Input_EngineCurbModeState;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    EngineCurbModeState mEngineCurbModeState = EngineCurbModeState::OFF;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
};

}  // namespace ccos

#endif  // SFSS_Curb_Mode_H
