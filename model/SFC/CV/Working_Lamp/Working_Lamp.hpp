/**
 * @file Working_Lamp.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Working_Lamp_H
#define SFSS_Working_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "Working_LampBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.35
// Reference : [CV700] Working Lamp. Contains : Telltale. Recommend: VALUE_CHANGED
class Working_Lamp : public Working_LampBase {
public:
    Working_Lamp() = default;
    ~Working_Lamp() override = default;
    Working_Lamp(const Working_Lamp& other) = delete;
    Working_Lamp(Working_Lamp&& other) noexcept = delete;
    Working_Lamp& operator=(const Working_Lamp& other) = delete;
    Working_Lamp& operator=(Working_Lamp&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleWorkingLampStat();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleWorkingLampStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleWorkingLampStat();
    }

    void onWorkingLampStatusChanged(const ArgumentsWorkingLampStatusChanged& args) {
        mWorkingLampStatus = args.mInput_WorkingLampStatus;
        updateTelltaleWorkingLampStat();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltaleWorkingLampStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleWorkingLampStat() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        SFCWorking_LampTelltaleWorkingLampStat stat = SFCWorking_LampTelltaleWorkingLampStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mWorkingLampStatus == WorkingLampStatus::ON) {
                stat = SFCWorking_LampTelltaleWorkingLampStat::ON;
            }
        }
        setSFCWorking_LampTelltaleWorkingLampStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using WorkingLampStatus = ArgumentsWorkingLampStatusChanged::Input_WorkingLampStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    WorkingLampStatus mWorkingLampStatus = WorkingLampStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
};

}  // namespace ccos

#endif  // SFSS_Working_Lamp_H
