/**
 * @file Jake_Brake_CV.hpp
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
#ifndef SFSS_Jake_Brake_CV_H
#define SFSS_Jake_Brake_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Jake_Brake_CVBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.16
// Reference : [CV540] Jake_Brake_CV. Contains : Telltale. Recommend: VALUE_CHANGED
class Jake_Brake_CV : public Jake_Brake_CVBase {
public:
    Jake_Brake_CV() = default;
    ~Jake_Brake_CV() override = default;
    Jake_Brake_CV(const Jake_Brake_CV& other) = delete;
    Jake_Brake_CV(Jake_Brake_CV&& other) noexcept = delete;
    Jake_Brake_CV& operator=(const Jake_Brake_CV& other) = delete;
    Jake_Brake_CV& operator=(Jake_Brake_CV&& other) noexcept = delete;

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
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltale();
    }

    void onExhaustBrakeStatusChanged(const ArgumentsExhaustBrakeStatusChanged& args) {
        mJakeBrakeStatus = args.mInput_JakeBrakeStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigFuelTypeCV != ConfigFuelTypeCV::DSL || mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        SFCJake_Brake_CVTelltaleJake_Brake_CVStat stat = SFCJake_Brake_CVTelltaleJake_Brake_CVStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mJakeBrakeStatus == JakeBrakeStatus::ON) {
            stat = SFCJake_Brake_CVTelltaleJake_Brake_CVStat::ON;
        }

        setSFCJake_Brake_CVTelltaleJake_Brake_CVStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using JakeBrakeStatus = ArgumentsExhaustBrakeStatusChanged::Input_JakeBrakeStatus;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;

    JakeBrakeStatus mJakeBrakeStatus = JakeBrakeStatus::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
};

}  // namespace ccos

#endif  // SFSS_Jake_Brake_CV_H
