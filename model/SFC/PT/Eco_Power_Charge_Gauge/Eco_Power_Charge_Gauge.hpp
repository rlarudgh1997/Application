/**
 * @file Eco_Power_Charge_Gauge.hpp
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
#ifndef SFSS_Eco_Power_Charge_Gauge_H
#define SFSS_Eco_Power_Charge_Gauge_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "Eco_Power_Charge_GaugeBase.hpp"

namespace ccos {

// SFC Version : 12.0.0
// Reference : [PT100] Eco Power Charge Gauge. Contains : Constant. Recommend: VALUE_CHANGED
class Eco_Power_Charge_Gauge : public Eco_Power_Charge_GaugeBase {
public:
    Eco_Power_Charge_Gauge() = default;
    ~Eco_Power_Charge_Gauge() override = default;
    Eco_Power_Charge_Gauge(const Eco_Power_Charge_Gauge& other) = delete;
    Eco_Power_Charge_Gauge(Eco_Power_Charge_Gauge&& other) noexcept = delete;
    Eco_Power_Charge_Gauge& operator=(const Eco_Power_Charge_Gauge& other) = delete;
    Eco_Power_Charge_Gauge& operator=(Eco_Power_Charge_Gauge&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        DDebug() << "IgnitionElapsed::ON_0ms";
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateEcoPwrChgGaugeValue();
        updateEvBoostGaugeValue();
        updatePwrChgGaugeDeactivateAreaValue();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        DDebug() << "IgnitionElapsed::OFF_0ms";
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateEcoPwrChgGaugeValue();
        updateEvBoostGaugeValue();
        updatePwrChgGaugeDeactivateAreaValue();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        DDebug() << "Inter_ConfigVehicleType " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        DDebug() << "Inter_ConfigBoostMode " << static_cast<HUInt64>(args.mInter_ConfigBoostMode);
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigBoostMode = args.mInter_ConfigBoostMode;
        updateEcoPwrChgGaugeValue();
        updateEvBoostGaugeValue();
        updatePwrChgGaugeDeactivateAreaValue();
    }

    void onInputHevEcoPowerGaugeInputValueChanged(const ArgumentsInputHevEcoPowerGaugeInputValueChanged& args) {
        mHevEcoPowerGaugeInputValue = args.mInput_HevEcoPowerGaugeInputValue;
        DDebug() << "Input_HevEcoPowerGaugeInputValue " << args.mInput_HevEcoPowerGaugeInputValue;
        updateEcoPwrChgGaugeValue();
    }

    void onInputPowerGaugeInputValueChanged(const ArgumentsInputPowerGaugeInputValueChanged& args) {
        mPowerGaugeInputValue = args.mInput_PowerGaugeInputValue;
        mPowerGaugeInputValue_timeout = ISTIMEOUT(args.mInput_PowerGaugeInputValue);
        DDebug() << "Input_PowerGaugeInputValue " << args.mInput_PowerGaugeInputValue;
        DDebug() << "Input_PowerGaugeInputValue_timeout " << mPowerGaugeInputValue_timeout;
        updateEcoPwrChgGaugeValue();
    }

    void onEvBoostGaugeValueChanged(const ArgumentsEvBoostGaugeValueChanged& args) {
        DDebug() << "Input_EvBoostModeAvailableStatus " << static_cast<HUInt64>(args.mInput_EvBoostModeAvailableStatus);
        DDebug() << "Input_EvBoostModeOperationStatus " << static_cast<HUInt64>(args.mInput_EvBoostModeOperationStatus);
        mEvBoostModeAvailableStatus = args.mInput_EvBoostModeAvailableStatus;
        mEvBoostModeOperationStatus = args.mInput_EvBoostModeOperationStatus;
        updateEvBoostGaugeValue();
    }

    void onPowerGaugeDeactivateValueChanged(const ArgumentsPowerGaugeDeactivateValueChanged& args) {
        DDebug() << "Input_PowerGaugeDeactivateValue " << static_cast<HUInt64>(args.mInput_PowerGaugeDeactivateValue);
        if (updateValueIfValid(mPowerGaugeDeactivateValue, args.mInput_PowerGaugeDeactivateValue) == true) {
            if (ISTIMEOUT(args.mInput_PowerGaugeDeactivateValue) == true) {
                mPowerGaugeDeactivateValue = PowerGaugeDeactivateValue::UNHANDLED_TIMEOUT;
            }
            updatePwrChgGaugeDeactivateAreaValue();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEcoPwrChgGaugeValue() {
        switch (mConfigVehicleType) {
            case ConfigVehicleType::HEV:
            case ConfigVehicleType::PHEV:
                updateHevEcoPwrChgGaugeValue();
                break;
            case ConfigVehicleType::EV:
            case ConfigVehicleType::FCEV:
            case ConfigVehicleType::EREV:
                updateEvEcoPwrChgGaugeValue();
                break;
            default:
                DError() << "mConfigVehicleType : " << static_cast<HUInt64>(mConfigVehicleType);
                break;
        }
    }

    void updateHevEcoPwrChgGaugeValue() {
        HDouble value = 0.0;
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms) {
            if (mHevEcoPowerGaugeInputValue == 0) {
                value = 0.0;
            } else if (mHevEcoPowerGaugeInputValue < 85) {
                value = (50.0 / 84.0) * (1.0 - static_cast<HDouble>(mHevEcoPowerGaugeInputValue)) + 100.0;
            } else if (mHevEcoPowerGaugeInputValue <= 211) {
                value = (25.0 / 63.0) * (211.0 - static_cast<HDouble>(mHevEcoPowerGaugeInputValue));
            } else if (mHevEcoPowerGaugeInputValue <= 255) {
                value = (25.0 / 11.0) * (211.0 - static_cast<HDouble>(mHevEcoPowerGaugeInputValue));
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCEco_Power_Charge_GaugeConstantHevEcoPwrChgGaugeValue(round((value)*100.0) / 100.0);
    }

    void updateEvEcoPwrChgGaugeValue() {
        HInt64 value = 0;
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms) {
            if (mPowerGaugeInputValue_timeout == true) {
                setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeStat(EvEcoPwrChgGaugeStat::BLINK1);
                setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeBlinkValueA(EvEcoPwrChgGaugeBlinkValueA::ON);
                setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeBlinkValueB(EvEcoPwrChgGaugeBlinkValueB::OFF);
            } else {
                if (mConfigBoostMode == ConfigBoostMode::OFF) {
                    setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeStat(EvEcoPwrChgGaugeStat::VALUE_BOOST_MODE_OFF);
                } else if (mConfigBoostMode == ConfigBoostMode::ON) {
                    setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeStat(EvEcoPwrChgGaugeStat::VALUE_BOOST_MODE_ON);
                } else {
                    // nothing
                }
                if (mPowerGaugeInputValue < 128) {
                    if (mPowerGaugeInputValue > 99) {
                        value = 100;
                    } else {
                        value = static_cast<HInt64>(mPowerGaugeInputValue);
                    }
                } else {
                    if (mPowerGaugeInputValue < 157) {
                        value = -100;
                    } else {
                        value = static_cast<HInt64>(mPowerGaugeInputValue) - 256;
                    }
                }
            }
        } else {
            // no operation
        }
        setSFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeValue(value);
    }

    void updateEvBoostGaugeValue() {
        EvBoostGaugeValue value = EvBoostGaugeValue::DISPLAY_OFF;
        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnitionElapsed == IgnitionElapsed::ON_0ms && mEvBoostModeAvailableStatus == EvBoostModeAvailableStatus::ACTIVATE) {
            if (mEvBoostModeOperationStatus == EvBoostModeOperationStatus::ACTIVATE) {
                value = EvBoostGaugeValue::ON;
            } else if (mEvBoostModeOperationStatus == EvBoostModeOperationStatus::READY) {
                value = EvBoostGaugeValue::READY;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCEco_Power_Charge_GaugeConstantEvBoostGaugeValue(value);
    }

    void updatePwrChgGaugeDeactivateAreaValue() {
        SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue value =
            SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::OFF;
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms && mConfigVehicleType == ConfigVehicleType::FCEV) {
            switch (mPowerGaugeDeactivateValue) {
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM40:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM40;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM45:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM45;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM50:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM50;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM55:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM55;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM60:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM60;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM65:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM65;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM70:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM70;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM75:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM75;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM80:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM80;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM85:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM85;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM90:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM90;
                    break;
                case PowerGaugeDeactivateValue::DEACTIVATE_FROM95:
                    value = SFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue::DEACTIVATE_FROM95;
                    break;
                default:
                    // OFF
                    break;
            }
        }
        setSFCEco_Power_Charge_GaugeConstantPwrChgGaugeDeactivateAreaValue(value);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigBoostMode = ArgumentsConfigChanged::Inter_ConfigBoostMode;
    using EvBoostModeAvailableStatus = ArgumentsEvBoostGaugeValueChanged::Input_EvBoostModeAvailableStatus;
    using EvBoostModeOperationStatus = ArgumentsEvBoostGaugeValueChanged::Input_EvBoostModeOperationStatus;
    using EvEcoPwrChgGaugeStat = SFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeStat;
    using EvEcoPwrChgGaugeBlinkValueA = SFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeBlinkValueA;
    using EvEcoPwrChgGaugeBlinkValueB = SFCEco_Power_Charge_GaugeConstantEvEcoPwrChgGaugeBlinkValueB;
    using EvBoostGaugeValue = SFCEco_Power_Charge_GaugeConstantEvBoostGaugeValue;
    using PowerGaugeDeactivateValue = ArgumentsPowerGaugeDeactivateValueChanged::Input_PowerGaugeDeactivateValue;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigBoostMode mConfigBoostMode = ConfigBoostMode::OFF;
    HUInt64 mHevEcoPowerGaugeInputValue = 0;
    HUInt64 mPowerGaugeInputValue = 0;
    HBool mPowerGaugeInputValue_timeout = false;
    EvBoostModeAvailableStatus mEvBoostModeAvailableStatus = EvBoostModeAvailableStatus::OFF;
    EvBoostModeOperationStatus mEvBoostModeOperationStatus = EvBoostModeOperationStatus::OFF;
    PowerGaugeDeactivateValue mPowerGaugeDeactivateValue = PowerGaugeDeactivateValue::NO_DATA;
};

}  // namespace ccos

#endif  // SFSS_Eco_Power_Charge_Gauge_H
