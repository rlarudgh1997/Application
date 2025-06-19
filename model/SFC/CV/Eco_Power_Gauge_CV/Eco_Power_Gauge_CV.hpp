/**
 * @file Eco_Power_Gauge_CV.hpp
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
#ifndef SFSS_Eco_Power_Gauge_CV_H
#define SFSS_Eco_Power_Gauge_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Eco_Power_Gauge_CVBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.16
// Reference : [CV990] Eco_Power_Gauge_CV. Contains : Constant. Recommend: VALUE_CHANGED
class Eco_Power_Gauge_CV : public Eco_Power_Gauge_CVBase {
public:
    Eco_Power_Gauge_CV() = default;
    ~Eco_Power_Gauge_CV() override = default;
    Eco_Power_Gauge_CV(const Eco_Power_Gauge_CV& other) = delete;
    Eco_Power_Gauge_CV(Eco_Power_Gauge_CV&& other) noexcept = delete;
    Eco_Power_Gauge_CV& operator=(const Eco_Power_Gauge_CV& other) = delete;
    Eco_Power_Gauge_CV& operator=(Eco_Power_Gauge_CV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        DDebug() << "IgnElapsedOn0ms";
        mIsIgnOn = true;
        updateEcoPwrChgGaugeValue();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        DDebug() << "IgnElapseOff0ms";
        mIsIgnOn = false;
        updateEcoPwrChgGaugeValue();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        DDebug() << "Inter_ConfigVehicleType " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEcoPwrChgGaugeValue();
    }

    void onPowerGaugeInputValueChanged(const ArgumentsPowerGaugeInputValueChanged& args) {
        if (ISTIMEOUT(args.mInput_PowerGaugeInputValue) == true) {
            mPowerGaugeInputValue_timeout = true;
        } else {
            mPowerGaugeInputValue_timeout = false;
        }
        mPowerGaugeInputValue = args.mInput_PowerGaugeInputValue;
        DDebug() << "Input_PowerGaugeInputValue " << args.mInput_PowerGaugeInputValue;
        DDebug() << "Input_PowerGaugeInputValue_timeout " << mPowerGaugeInputValue_timeout;
        updateEcoPwrChgGaugeValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEcoPwrChgGaugeValue() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        HInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mPowerGaugeInputValue_timeout == true) {
                DDebug() << "mInput_PowerGaugeInputValue_timeout " << mPowerGaugeInputValue_timeout;
                value = 0;
            } else {
                if (mPowerGaugeInputValue <= 255) {
                    if (mPowerGaugeInputValue >= 200) {
                        value = 100;
                    } else {
                        value = static_cast<HInt64>(mPowerGaugeInputValue) - 100;
                    }
                }
            }
        }
        setSFCEco_Power_Gauge_CVConstantEcoPwrChgGaugeValue(value);
    }

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;

    HBool mIsIgnOn = false;
    HUInt64 mPowerGaugeInputValue = 0;
    HBool mPowerGaugeInputValue_timeout = false;
};

}  // namespace ccos

#endif  // SFSS_Eco_Power_Gauge_CV_H
