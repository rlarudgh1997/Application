/**
 * @file Energy_Flow.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2020-2022  Hyundai Motor Company,
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
#ifndef SFSS_Energy_Flow_H
#define SFSS_Energy_Flow_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "Energy_FlowBase.hpp"

namespace ccos {

// SFC Version : 9.0.0
// Reference : [PT120] Energy_Flow. Contains : Constant. Recommend: VALUE_CHANGED
class Energy_Flow : public Energy_FlowBase {
public:
    Energy_Flow() = default;
    ~Energy_Flow() override = default;
    Energy_Flow(const Energy_Flow& other) = delete;
    Energy_Flow(Energy_Flow&& other) noexcept = delete;
    Energy_Flow& operator=(const Energy_Flow& other) = delete;
    Energy_Flow& operator=(Energy_Flow&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateConstantEnergyFlow();
        updateConstantEnergyFlowHEVeAWD();
        updateConstantEnergyFlowHEVeAWDTitle();
        updateConstantBattery();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateConstantEnergyFlow();
        updateConstantEnergyFlowHEVeAWD();
        updateConstantEnergyFlowHEVeAWDTitle();
        updateConstantBattery();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateConstantEnergyFlow();
        updateConstantEnergyFlowHEVeAWD();
        updateConstantEnergyFlowHEVeAWDTitle();
    }

    void onEvEnergyFlowStatusChanged(const ArgumentsEvEnergyFlowStatusChanged& args) {
        mEvEnergyFlowStatus = args.mInput_EvEnergyFlowStatus;

        updateConstantEnergyFlow();
        updateConstantEnergyFlowHEVeAWDTitle();
    }

    void onEawdRearEnergyFlowStatusChanged(const ArgumentsEawdRearEnergyFlowStatusChanged& args) {
        mEawdRearEnergyFlowStatus = args.mInput_EawdRearEnergyFlowStatus;
        updateConstantEnergyFlowHEVeAWD();
        updateConstantEnergyFlowHEVeAWDTitle();
    }

    void onSocGaugeValueChanged(const ArgumentsSocGaugeValueChanged& args) {
        isEvSocDisplayValue_Timeout = ISTIMEOUT(args.mInput_EvSocDisplayValue);
        mSocGaugeValue = args.mSOCGaugeValue;
        updateConstantBattery();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantEnergyFlow() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mConfigVehicleType == ConfigVehicleType::HEV) || (mConfigVehicleType == ConfigVehicleType::PHEV)) {
                updateEnergyFlowHEVStat();
            } else if (mConfigVehicleType == ConfigVehicleType::EV) {
                updateEnergyFlowEVStat();
            } else if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                updateEnergyFlowFCEVStat();
            } else {
                // No process any.
            }
        }
    }

    void updateConstantEnergyFlowHEVeAWD() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV)) {
            switch (mEawdRearEnergyFlowStatus) {
                case EawdRearEnergyFlowStatus::EAWD_STOP:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_STOP);
                    mEnergyFlowHEVeAWDStat = SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_STOP;
                    break;
                case EawdRearEnergyFlowStatus::EAWD_MOTORING:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_MOTORING);
                    mEnergyFlowHEVeAWDStat = SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_MOTORING;
                    break;
                case EawdRearEnergyFlowStatus::EAWD_GENERATING:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_GENERATING);
                    mEnergyFlowHEVeAWDStat = SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_GENERATING;
                    break;
                default:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::VEHICLE_STOP);
                    mEnergyFlowHEVeAWDStat = SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::VEHICLE_STOP;
                    break;
            }
        }
    }

    template <typename T1, typename T2>
    inline void getEnergyFlowConstantValue(const T1& inputStat, const std::map<T1, T2>& valueMap, T2& valueStat) {
        const auto iter = valueMap.find(inputStat);
        if (iter != valueMap.end()) {
            valueStat = iter->second;
        }
    }

    inline void updateEnergyFlowHEVStat() {
        SFCEnergy_FlowConstantEnergyFlowHEVStat stat = SFCEnergy_FlowConstantEnergyFlowHEVStat::VEHICLE_STOP;
        getEnergyFlowConstantValue(mEvEnergyFlowStatus, mEnergyFlowHEVStatMap, stat);
        setSFCEnergy_FlowConstantEnergyFlowHEVStat(stat);
        mEnergyFlowHEVStat = stat;
    }

    inline void updateEnergyFlowEVStat() {
        SFCEnergy_FlowConstantEnergyFlowEVStat stat = SFCEnergy_FlowConstantEnergyFlowEVStat::VEHICLE_STOP;
        getEnergyFlowConstantValue(mEvEnergyFlowStatus, mEnergyFlowEVStatMap, stat);
        setSFCEnergy_FlowConstantEnergyFlowEVStat(stat);
    }

    inline void updateEnergyFlowFCEVStat() {
        SFCEnergy_FlowConstantEnergyFlowFCEVStat stat = SFCEnergy_FlowConstantEnergyFlowFCEVStat::VEHICLE_STOP;
        getEnergyFlowConstantValue(mEvEnergyFlowStatus, mEnergyFlowFCEVStatMap, stat);
        setSFCEnergy_FlowConstantEnergyFlowFCEVStat(stat);
    }

    void updateConstantEnergyFlowHEVeAWDTitle() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV)) {
            switch (mEnergyFlowHEVStat) {
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::VEHICLE_STOP:
                    if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_STOP ||
                        mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::VEHICLE_STOP) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::READY);
                    } else if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_MOTORING) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::MOTOR);
                    } else if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_GENERATING) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::CHARGING);
                    } else {
                        // no operation
                    }
                    break;
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::EV_PROPULSION:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::MOTOR);
                    break;
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::POWER_ASSIST:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::HYBRID);
                    break;
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_ONLY_PROPULSION:
                    if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_STOP ||
                        mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::VEHICLE_STOP) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::ENGINE);
                    } else if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_MOTORING) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::HYBRID);
                    } else if (mEnergyFlowHEVeAWDStat == SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::EAWD_GENERATING) {
                        setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                            SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::CHARGING);
                    } else {
                        // no operation
                    }
                    break;
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION:
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::REGENERATION:
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::POWER_RESEARVE:
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION_MOTOR_DRIVE:
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION_REGENERATION:
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_BRAKERE_GENERATION:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(
                        SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::CHARGING);
                    break;
                case SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_BRAKE:
                    setSFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat(SFCEnergy_FlowConstantEnergyFlowHEVeAWDTitleStat::BRAKE);
                    break;
                default:
                    break;
            }
        }
    }

    void updateConstantBattery() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            AddressValueList list;
            OutputSocDisplayLevelStatus levelStatus = OutputSocDisplayLevelStatus::INVALID;

            if (isEvSocDisplayValue_Timeout == false) {
                setSFCEnergy_FlowConstantBatteryBarValue(mSocGaugeValue);
                if ((mSocGaugeValue >= 0) && (mSocGaugeValue <= 11)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG0;
                } else if ((mSocGaugeValue >= 12) && (mSocGaugeValue <= 24)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG1;
                } else if ((mSocGaugeValue >= 25) && (mSocGaugeValue <= 39)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG2;
                } else if ((mSocGaugeValue >= 40) && (mSocGaugeValue <= 49)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG3;
                } else if ((mSocGaugeValue >= 50) && (mSocGaugeValue <= 59)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG4;
                } else if ((mSocGaugeValue >= 60) && (mSocGaugeValue <= 69)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG5;
                } else if ((mSocGaugeValue >= 70) && (mSocGaugeValue <= 79)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG6;
                } else if ((mSocGaugeValue >= 80) && (mSocGaugeValue <= 91)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG7;
                } else if ((mSocGaugeValue >= 92) && (mSocGaugeValue <= 100)) {
                    levelStatus = OutputSocDisplayLevelStatus::SEG8;
                } else {
                    // No process any.
                }
            } else {
                setSFCEnergy_FlowConstantBatteryBarValue(0);
            }

            list.emplace_back(Vehicle.PT.Energy_Flow.Output_SocDisplayLevelStatus, static_cast<ccos::HUInt64>(levelStatus));

            setValue(list);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using EvEnergyFlowStatus = ArgumentsEvEnergyFlowStatusChanged::Input_EvEnergyFlowStatus;
    using EawdRearEnergyFlowStatus = ArgumentsEawdRearEnergyFlowStatusChanged::Input_EawdRearEnergyFlowStatus;
    using OutputSocDisplayLevelStatus = decltype(Vehicle.PT.Energy_Flow.Output_SocDisplayLevelStatus)::TYPE;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EvEnergyFlowStatus mEvEnergyFlowStatus = EvEnergyFlowStatus::NONE;
    EawdRearEnergyFlowStatus mEawdRearEnergyFlowStatus = EawdRearEnergyFlowStatus::NONE;
    HUInt64 mSocGaugeValue = 0;
    HBool isEvSocDisplayValue_Timeout = false;
    SFCEnergy_FlowConstantEnergyFlowHEVStat mEnergyFlowHEVStat = SFCEnergy_FlowConstantEnergyFlowHEVStat::NONE;
    SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat mEnergyFlowHEVeAWDStat = SFCEnergy_FlowConstantEnergyFlowHEVeAWDStat::NONE;

    const std::map<EvEnergyFlowStatus, SFCEnergy_FlowConstantEnergyFlowHEVStat> mEnergyFlowHEVStatMap = {
        {EvEnergyFlowStatus::EV_PROPULSION, SFCEnergy_FlowConstantEnergyFlowHEVStat::EV_PROPULSION},
        {EvEnergyFlowStatus::POWER_ASSIST, SFCEnergy_FlowConstantEnergyFlowHEVStat::POWER_ASSIST},
        {EvEnergyFlowStatus::ENGINE_ONLY_PROPULSION, SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_ONLY_PROPULSION},
        {EvEnergyFlowStatus::ENGINE_GENERATION, SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION},
        {EvEnergyFlowStatus::REGENERATION, SFCEnergy_FlowConstantEnergyFlowHEVStat::REGENERATION},
        {EvEnergyFlowStatus::ENGINE_BRAKE, SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_BRAKE},
        {EvEnergyFlowStatus::POWER_RESEARVE, SFCEnergy_FlowConstantEnergyFlowHEVStat::POWER_RESEARVE},
        {EvEnergyFlowStatus::ENGINE_GENERATION_MOTOR_DRIVE,
         SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION_MOTOR_DRIVE},
        {EvEnergyFlowStatus::ENGINE_GENERATION_REGENERATION,
         SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_GENERATION_REGENERATION},
        {EvEnergyFlowStatus::ENGINE_BRAKERE_GENERATION, SFCEnergy_FlowConstantEnergyFlowHEVStat::ENGINE_BRAKERE_GENERATION}};

    const std::map<EvEnergyFlowStatus, SFCEnergy_FlowConstantEnergyFlowEVStat> mEnergyFlowEVStatMap = {
        {EvEnergyFlowStatus::EV_FRONT_MOTOR, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_FRONT_MOTOR},
        {EvEnergyFlowStatus::EV_REAR_MOTOR, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_REAR_MOTOR},
        {EvEnergyFlowStatus::EV_DUAL_MOTOR, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_DUAL_MOTOR},
        {EvEnergyFlowStatus::EV_FRONT_GENERATION, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_FRONT_GENERATION},
        {EvEnergyFlowStatus::EV_REAR_GENERATION, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_REAR_GENERATION},
        {EvEnergyFlowStatus::EV_DUAL_GENERATION, SFCEnergy_FlowConstantEnergyFlowEVStat::EV_DUAL_GENERATION}};

    const std::map<EvEnergyFlowStatus, SFCEnergy_FlowConstantEnergyFlowFCEVStat> mEnergyFlowFCEVStatMap = {
        {EvEnergyFlowStatus::FCEV_IDLE, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_IDLE},
        {EvEnergyFlowStatus::FCEV_CHARGE_IDLE, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_CHARGE_IDLE},
        {EvEnergyFlowStatus::FCEV_ASSIST_NORMAL, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_ASSIST_NORMAL},
        {EvEnergyFlowStatus::FCEV_CHARGE_NORMAL, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_CHARGE_NORMAL},
        {EvEnergyFlowStatus::FCEV_BATTERY, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_BATTERY},
        {EvEnergyFlowStatus::FCEV_REGENERATION, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_REGENERATION},
        {EvEnergyFlowStatus::FCEV_FUELCELL, SFCEnergy_FlowConstantEnergyFlowFCEVStat::FCEV_FUELCELL}};
};

}  // namespace ccos

#endif  // SFSS_Energy_Flow_H
