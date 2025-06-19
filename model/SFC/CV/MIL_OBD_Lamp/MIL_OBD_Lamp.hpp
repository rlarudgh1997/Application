/**
 * @file MIL_OBD_Lamp.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022 Hyundai Motor Company,
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
#ifndef SFSS_MIL_OBD_Lamp_H
#define SFSS_MIL_OBD_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "MIL_OBD_LampBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.06
// Reference : [CV560] MIL_OBD_Lamp. Contains : Telltale. Recommend: VALUE_CHANGED
class MIL_OBD_Lamp : public MIL_OBD_LampBase {
public:
    MIL_OBD_Lamp() = default;
    ~MIL_OBD_Lamp() override = default;
    MIL_OBD_Lamp(const MIL_OBD_Lamp& other) = delete;
    MIL_OBD_Lamp(MIL_OBD_Lamp&& other) noexcept = delete;
    MIL_OBD_Lamp& operator=(const MIL_OBD_Lamp& other) = delete;
    MIL_OBD_Lamp& operator=(MIL_OBD_Lamp&& other) noexcept = delete;

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
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void onMilLampStatusChanged(const ArgumentsMilLampStatusChanged& args) {
        mMILLampStatus = args.mInput_MILLampStatus;
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
        SFCMIL_OBD_LampTelltaleMILStat stat = SFCMIL_OBD_LampTelltaleMILStat::OFF;
        if ((mIgnElapsed == IgnElapsed::ON_500ms) && (mMILLampStatus == MILLampStatus::ON)) {
            stat = SFCMIL_OBD_LampTelltaleMILStat::ON;
        }
        setSFCMIL_OBD_LampTelltaleMILStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;

    using MILLampStatus = ArgumentsMilLampStatusChanged::Input_MILLampStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    MILLampStatus mMILLampStatus = MILLampStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_MIL_OBD_Lamp_H
