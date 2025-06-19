/**
 * @file Exhaust_Brake.hpp
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
#ifndef SFSS_Exhaust_Brake_H
#define SFSS_Exhaust_Brake_H

#define DLOG_ENABLED gEnableSFCLog

#include "Exhaust_BrakeBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.06
// Reference : [CV530] Exhaust_Brake. Contains : Telltale. Recommend: VALUE_CHANGED
class Exhaust_Brake : public Exhaust_BrakeBase {
public:
    Exhaust_Brake() = default;
    ~Exhaust_Brake() override = default;
    Exhaust_Brake(const Exhaust_Brake& other) = delete;
    Exhaust_Brake(Exhaust_Brake&& other) noexcept = delete;
    Exhaust_Brake& operator=(const Exhaust_Brake& other) = delete;
    Exhaust_Brake& operator=(Exhaust_Brake&& other) noexcept = delete;

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

    void onExhaustBrakeStatusChanged(const ArgumentsExhaustBrakeStatusChanged& args) {
        mExhaustBrakeStatus = args.mInput_ExhaustBrakeStatus;
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
        SFCExhaust_BrakeTelltaleExhaust_BrakeStat stat = SFCExhaust_BrakeTelltaleExhaust_BrakeStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mExhaustBrakeStatus == ExhaustBrakeStatus::ON) {
            stat = SFCExhaust_BrakeTelltaleExhaust_BrakeStat::ON;
        }

        setSFCExhaust_BrakeTelltaleExhaust_BrakeStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;

    using ExhaustBrakeStatus = ArgumentsExhaustBrakeStatusChanged::Input_ExhaustBrakeStatus;

    ExhaustBrakeStatus mExhaustBrakeStatus = ExhaustBrakeStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Exhaust_Brake_H
