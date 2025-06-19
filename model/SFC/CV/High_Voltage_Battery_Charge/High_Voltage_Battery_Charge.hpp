/**
 * @file High_Voltage_Battery_Charge.hpp
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
#ifndef SFSS_High_Voltage_Battery_Charge_H
#define SFSS_High_Voltage_Battery_Charge_H

#define DLOG_ENABLED gEnableSFCLog

#include "High_Voltage_Battery_ChargeBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.06
// Reference : [CV260] High Voltage Battery Charge. Contains : Telltale. Recommend: VALUE_CHANGED
class High_Voltage_Battery_Charge : public High_Voltage_Battery_ChargeBase {
public:
    High_Voltage_Battery_Charge() = default;
    ~High_Voltage_Battery_Charge() override = default;
    High_Voltage_Battery_Charge(const High_Voltage_Battery_Charge& other) = delete;
    High_Voltage_Battery_Charge(High_Voltage_Battery_Charge&& other) noexcept = delete;
    High_Voltage_Battery_Charge& operator=(const High_Voltage_Battery_Charge& other) = delete;
    High_Voltage_Battery_Charge& operator=(High_Voltage_Battery_Charge&& other) noexcept = delete;

    void onInitialize() override {
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onHighVoltBatChrStatusChanged(const ArgumentsHighVoltBatChrStatusChanged& args) {
        mHighVoltBatChrStatus = args.mInput_HighVoltBatChrStatus;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTelltale() {
        SFCHigh_Voltage_Battery_ChargeTelltaleHiVoltBatChrLampStat telltaleHiVoltBatChrLampStat =
            SFCHigh_Voltage_Battery_ChargeTelltaleHiVoltBatChrLampStat::OFF;

        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        if (mHighVoltBatChrStatus == HighVoltBatChrStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            telltaleHiVoltBatChrLampStat = SFCHigh_Voltage_Battery_ChargeTelltaleHiVoltBatChrLampStat::ON;
        }
        setSFCHigh_Voltage_Battery_ChargeTelltaleHiVoltBatChrLampStat(telltaleHiVoltBatChrLampStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using HighVoltBatChrStatus = ArgumentsHighVoltBatChrStatusChanged::Input_HighVoltBatChrStatus;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HighVoltBatChrStatus mHighVoltBatChrStatus = HighVoltBatChrStatus::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
};

}  // namespace ccos

#endif  // SFSS_High_Voltage_Battery_Charge_H
