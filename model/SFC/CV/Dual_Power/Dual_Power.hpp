/**
 * @file Dual_Power.hpp
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
#ifndef SFSS_Dual_Power_H
#define SFSS_Dual_Power_H

#define DLOG_ENABLED gEnableSFCLog

#include "Dual_PowerBase.hpp"

namespace ccos {

// SFC Version : 6.0.0
// Spec Version : v0.27
// Reference : [CV010] Dual Power. Contains : Telltale. Recommend: VALUE_CHANGED
class Dual_Power : public Dual_PowerBase {
public:
    Dual_Power() = default;
    ~Dual_Power() override = default;
    Dual_Power(const Dual_Power& other) = delete;
    Dual_Power(Dual_Power&& other) noexcept = delete;
    Dual_Power& operator=(const Dual_Power& other) = delete;
    Dual_Power& operator=(Dual_Power&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateTelltale();
        updateTelltaleSmart();
        updateTelltalePower();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateTelltale();
        updateTelltaleSmart();
        updateTelltalePower();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
        updateTelltaleSmart();
        updateTelltalePower();
    }

    void onConfigFuelTypeChanged(const ArgumentsConfigFuelTypeChanged& args) {
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltale();
    }

    void onConfigTMTypeChanged(const ArgumentsConfigTMTypeChanged& args) {
        mConfigTMType = args.mInter_ConfigTMType;
        updateTelltaleSmart();
    }

    void onConfigCVPRJTNameChanged(const ArgumentsConfigCVPRJTNameChanged& args) {
        mConfigCVPRJTName = args.mInter_ConfigCVPRJTName;
        updateTelltaleSmart();
    }

    void onEcoModeStatusChanged(const ArgumentsEcoModeStatusChanged& args) {
        mEcoModeStatus = args.mInput_EcoModeStatus;
        updateTelltale();
    }

    void onSmartModeStatusChanged(const ArgumentsSmartModeStatusChanged& args) {
        mSmartModeStatus = args.mInput_SmartModeStatus;
        updateTelltaleSmart();
    }

    void onEngineEcoLampStatusChanged(const ArgumentsEngineEcoLampStatusChanged& args) {
        mEngineEcoLampStatus = args.mInput_EngineEcoLampStatus;
        updateTelltale();
    }

    void onPowerModeStatusChanged(const ArgumentsPowerModeStatusChanged& args) {
        mPowerModeStatus = args.mInput_PowerModeStatus;
        updateTelltalePower();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCDual_PowerTelltaleDual_PowerStat stat = SFCDual_PowerTelltaleDual_PowerStat::OFF;
        if (mIsIgnOn == true) {
            if (((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
                 (mEcoModeStatus == EcoModeStatus::ECO)) ||
                (mConfigVehicleType == ConfigVehicleType::ICV && mConfigFuelTypeCV == ConfigFuelTypeCV::DSL &&
                 mEngineEcoLampStatus == EngineEcoLampStatus::ON)) {
                stat = SFCDual_PowerTelltaleDual_PowerStat::ON;
            }
        }

        setSFCDual_PowerTelltaleDual_PowerStat(stat);
    }

    void updateTelltaleSmart() {
        SFCDual_PowerTelltaleSmartModeStat stat = SFCDual_PowerTelltaleSmartModeStat::OFF;
        SFCDual_PowerTelltaleSmartModeStatOptional statOpt = SFCDual_PowerTelltaleSmartModeStatOptional::OFF;

        if (mIsIgnOn == true) {
            if (mConfigVehicleType == ConfigVehicleType::ICV && mConfigCVPRJTName == ConfigCVPRJTName::QZ &&
                mConfigTMType == ConfigTMType::AMT_ZF) {
                if (mSmartModeStatus == SmartModeStatus::PERFORMANCE) {
                    stat = SFCDual_PowerTelltaleSmartModeStat::OPTIONAL;
                    statOpt = SFCDual_PowerTelltaleSmartModeStatOptional::PERFORMANCE;
                } else if (mSmartModeStatus == SmartModeStatus::ECO) {
                    stat = SFCDual_PowerTelltaleSmartModeStat::OPTIONAL;
                    statOpt = SFCDual_PowerTelltaleSmartModeStatOptional::ECO;
                } else if (mSmartModeStatus == SmartModeStatus::SMART) {
                    stat = SFCDual_PowerTelltaleSmartModeStat::OPTIONAL;
                    statOpt = SFCDual_PowerTelltaleSmartModeStatOptional::SMART;
                } else {
                    // no operation
                }
            }
        }

        setSFCDual_PowerTelltaleSmartModeStat(stat);
        setSFCDual_PowerTelltaleSmartModeStatOptional(statOpt);
    }

    void updateTelltalePower() {
        SFCDual_PowerTelltalePowerModeStat stat = SFCDual_PowerTelltalePowerModeStat::OFF;

        if (mIsIgnOn == true) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV && mPowerModeStatus == PowerModeStatus::ON) {
                stat = SFCDual_PowerTelltalePowerModeStat::ON;
            }
        }

        setSFCDual_PowerTelltalePowerModeStat(stat);
    }

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigFuelTypeChanged::Inter_ConfigFuelTypeCV;
    using ConfigCVPRJTName = ArgumentsConfigCVPRJTNameChanged::Inter_ConfigCVPRJTName;
    using ConfigTMType = ArgumentsConfigTMTypeChanged::Inter_ConfigTMType;
    using EcoModeStatus = ArgumentsEcoModeStatusChanged::Input_EcoModeStatus;
    using SmartModeStatus = ArgumentsSmartModeStatusChanged::Input_SmartModeStatus;
    using EngineEcoLampStatus = ArgumentsEngineEcoLampStatusChanged::Input_EngineEcoLampStatus;
    using PowerModeStatus = ArgumentsPowerModeStatusChanged::Input_PowerModeStatus;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    ConfigCVPRJTName mConfigCVPRJTName = ConfigCVPRJTName::QZ;
    ConfigTMType mConfigTMType = ConfigTMType::MTM;
    EcoModeStatus mEcoModeStatus = EcoModeStatus::NORMAL;
    SmartModeStatus mSmartModeStatus = SmartModeStatus::OFF;
    EngineEcoLampStatus mEngineEcoLampStatus = EngineEcoLampStatus::OFF;
    PowerModeStatus mPowerModeStatus = PowerModeStatus::OFF;

    HBool mIsIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_Dual_Power_H
