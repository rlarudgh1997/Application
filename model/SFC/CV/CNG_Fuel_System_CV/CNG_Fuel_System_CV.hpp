/**
 * @file CNG_Fuel_System_CV.hpp
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
#ifndef SFSS_CNG_Fuel_System_CV_H
#define SFSS_CNG_Fuel_System_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "CNG_Fuel_System_CVBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.09
// Reference : [CV790] CNG_Fuel_System_CV. Contains : Telltale, Constant. Recommend: VALUE_CHANGED
class CNG_Fuel_System_CV : public CNG_Fuel_System_CVBase {
public:
    CNG_Fuel_System_CV() = default;
    ~CNG_Fuel_System_CV() override = default;
    CNG_Fuel_System_CV(const CNG_Fuel_System_CV& other) = delete;
    CNG_Fuel_System_CV(CNG_Fuel_System_CV&& other) noexcept = delete;
    CNG_Fuel_System_CV& operator=(const CNG_Fuel_System_CV& other) = delete;
    CNG_Fuel_System_CV& operator=(CNG_Fuel_System_CV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltale();
        updateConstant();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltale();
        updateConstant();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltale();
        updateConstant();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltale();
        updateConstant();
    }

    void onFuelWarningStatusChanged(const ArgumentsFuelWarningStatusChanged& args) {
        mEngineLowFuelWarnStatus = args.mInput_EngineLowFuelWarnStatus;
        updateTelltale();
    }

    void onCngEngineFuelGaugeChanged(const ArgumentsCngEngineFuelGaugeChanged& args) {
        mEngineFuelGauge_CNG = args.mInput_EngineFuelGauge_CNG;
        updateConstant();
    }
    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::CNG) {
            return;
        }
        SFCCNG_Fuel_System_CVTelltaleLowFuelWarnStat stat = SFCCNG_Fuel_System_CVTelltaleLowFuelWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            stat = SFCCNG_Fuel_System_CVTelltaleLowFuelWarnStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mEngineLowFuelWarnStatus == EngineLowFuelWarnStatus::ON) {
            stat = SFCCNG_Fuel_System_CVTelltaleLowFuelWarnStat::ON;
        } else {
            // defualt OFF
        }
        setSFCCNG_Fuel_System_CVTelltaleLowFuelWarnStat(stat);
    }

    void updateConstant() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::CNG) {
            return;
        }

        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mEngineFuelGauge_CNG >= 0 && mEngineFuelGauge_CNG <= 250) {
                value = mEngineFuelGauge_CNG;
            }
        }
        setSFCCNG_Fuel_System_CVConstantCNGFuelGaugeValue(value);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using EngineLowFuelWarnStatus = ArgumentsFuelWarningStatusChanged::Input_EngineLowFuelWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    bool mIsIgnOn = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    EngineLowFuelWarnStatus mEngineLowFuelWarnStatus = EngineLowFuelWarnStatus::OFF;
    HUInt64 mEngineFuelGauge_CNG = 0;
};

}  // namespace ccos

#endif  // SFSS_CNG_Fuel_System_CV_H
