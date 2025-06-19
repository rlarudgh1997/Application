/**
 * @file Stop_Lamp.hpp
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
#ifndef SFSS_Stop_Lamp_H
#define SFSS_Stop_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "Stop_LampBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.12
// Reference : [CV620] Stop_Lamp. Contains : Telltale. Recommend: VALUE_CHANGED
class Stop_Lamp : public Stop_LampBase {
public:
    Stop_Lamp() = default;
    ~Stop_Lamp() override = default;
    Stop_Lamp(const Stop_Lamp& other) = delete;
    Stop_Lamp(Stop_Lamp&& other) noexcept = delete;
    Stop_Lamp& operator=(const Stop_Lamp& other) = delete;
    Stop_Lamp& operator=(Stop_Lamp&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleStop();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleStop();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltaleStop();
    }

    void onStopStatusChanged(const ArgumentsStopStatusChanged& args) {
        mStopStatus = args.mInput_StopStatus;
        updateTelltaleStop();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleStop() {
        SFCStop_LampTelltaleStopStat telltaleStat = SFCStop_LampTelltaleStopStat::OFF;

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::BUS) {
            return;
        }

        if (mStopStatus == StopStatus::ON && mIgnElapsed == IgnElapsed::ON_500ms) {
            telltaleStat = SFCStop_LampTelltaleStopStat::ON;
        }

        setSFCStop_LampTelltaleStopStat(telltaleStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using StopStatus = ArgumentsStopStatusChanged::Input_StopStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    StopStatus mStopStatus = StopStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::BUS;
};

}  // namespace ccos

#endif  // SFSS_Stop_Lamp_H
