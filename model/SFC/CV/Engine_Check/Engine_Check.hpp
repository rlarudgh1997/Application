/**
 * @file Engine_Check.hpp
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
#ifndef SFSS_Engine_Check_H
#define SFSS_Engine_Check_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_CheckBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.08
// Reference : [CV770] Engine_Check. Contains : Telltale. Recommend: VALUE_CHANGED
class Engine_Check : public Engine_CheckBase {
public:
    Engine_Check() = default;
    ~Engine_Check() override = default;
    Engine_Check(const Engine_Check& other) = delete;
    Engine_Check(Engine_Check&& other) noexcept = delete;
    Engine_Check& operator=(const Engine_Check& other) = delete;
    Engine_Check& operator=(Engine_Check&& other) noexcept = delete;

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

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void onEngineCheckLampStatusChanged(const ArgumentsEngineCheckLampStatusChanged& args) {
        mEngineCheckLampStatus = args.mInput_EngineCheckLampStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        SFCEngine_CheckTelltaleEngineCheckLampStat stat = SFCEngine_CheckTelltaleEngineCheckLampStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mEngineCheckLampStatus == EngineCheckLampStatus::ON) {
            stat = SFCEngine_CheckTelltaleEngineCheckLampStat::ON;
        }

        setSFCEngine_CheckTelltaleEngineCheckLampStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_500ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    using EngineCheckLampStatus = ArgumentsEngineCheckLampStatusChanged::Input_EngineCheckLampStatus;
    EngineCheckLampStatus mEngineCheckLampStatus = EngineCheckLampStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Engine_Check_H
