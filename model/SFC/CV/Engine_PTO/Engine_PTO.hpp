/**
 * @file Engine_PTO.hpp
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
#ifndef SFSS_Engine_PTO_H
#define SFSS_Engine_PTO_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_PTOBase.hpp"

namespace ccos {

// SFC Version : 0.0.1
// Spec Version : v0.08
// Reference : [CV740] Engine_PTO. Contains : Telltale. Recommend: VALUE_CHANGED
class Engine_PTO : public Engine_PTOBase {
public:
    Engine_PTO() = default;
    ~Engine_PTO() override = default;
    Engine_PTO(const Engine_PTO& other) = delete;
    Engine_PTO(Engine_PTO&& other) noexcept = delete;
    Engine_PTO& operator=(const Engine_PTO& other) = delete;
    Engine_PTO& operator=(Engine_PTO&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleEnginePTOStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleEnginePTOStat();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltaleEnginePTOStat();
    }

    void onEnginePTOStateChanged(const ArgumentsEnginePTOStateChanged& args) {
        mEnginePTOState = args.mInput_EnginePTOState;
        updateTelltaleEnginePTOStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleEnginePTOStat() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        SFCEngine_PTOTelltaleEngine_PTOStat stat = SFCEngine_PTOTelltaleEngine_PTOStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms && mEnginePTOState == EnginePTOState::ON) {
            stat = SFCEngine_PTOTelltaleEngine_PTOStat::ON;
        }
        setSFCEngine_PTOTelltaleEngine_PTOStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using EnginePTOState = ArgumentsEnginePTOStateChanged::Input_EnginePTOState;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    EnginePTOState mEnginePTOState = EnginePTOState::OFF;
};

}  // namespace ccos

#endif  // SFSS_Engine_PTO_H
