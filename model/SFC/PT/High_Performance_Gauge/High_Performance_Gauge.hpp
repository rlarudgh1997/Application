/**
 * @file High_Performance_Gauge.hpp
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
#ifndef SFSS_High_Performance_Gauge_H
#define SFSS_High_Performance_Gauge_H
#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include <list>

#include "High_Performance_GaugeBase.hpp"

namespace ccos {

// SFC Version : 16.0.1
// Reference : [PT280] High_Performance_Gauge. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class High_Performance_Gauge : public High_Performance_GaugeBase {
public:
    High_Performance_Gauge() = default;
    ~High_Performance_Gauge() override = default;
    High_Performance_Gauge(const High_Performance_Gauge& other) = delete;
    High_Performance_Gauge(High_Performance_Gauge&& other) noexcept = delete;
    High_Performance_Gauge& operator=(const High_Performance_Gauge& other) = delete;
    High_Performance_Gauge& operator=(High_Performance_Gauge&& other) noexcept = delete;

    void onInitialize() override {
        setSFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueA(
            SFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueA::OPTIONAL);
        setSFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueB(
            SFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueB::OFF);
        setSFCHigh_Performance_GaugeEventLaunchControlType1LinkedSoundType(
            SFCHigh_Performance_GaugeEventLaunchControlType1LinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_GaugeEventLaunchControlType1LinkedSoundRepeatCount(1);
        setSFCHigh_Performance_GaugeEventLaunchControlType2LinkedSoundType(
            SFCHigh_Performance_GaugeEventLaunchControlType2LinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_GaugeEventLaunchControlType2LinkedSoundRepeatCount(1);
        setSFCHigh_Performance_GaugeEventLaunchControlType3LinkedSoundID("SND_PopUpInform2");
        setSFCHigh_Performance_GaugeEventLaunchControlType3LinkedSoundType(
            SFCHigh_Performance_GaugeEventLaunchControlType3LinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_GaugeEventLaunchControlType3LinkedSoundRepeatCount(1);
        setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusLinkedSoundType(
            SFCHigh_Performance_GaugeEventHighPerformanceWarnStatusLinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateTelltale();

        processPrivate_EngineOilTemp();
        processSinkConstantEngineOilTemp();

        processPrivate_EngineCoolantTemp();
        processSinkConstantEngineCoolantTemp();

        processPrivate_Turbo();
        processSinkConstantTurbo();

        processPrivate_Torque();
        processSinkConstantTorque();
        processSinkConstantTorqueEv();

        processPrivate_MotorTemp();
        processSinkConstantMotorTemp();

        processPrivate_EvBatteryTemp();
        processSinkConstantEvBatteryTemp();

        updateConstantVgsOnOff();

        updateEventLaunchControl1();
        updateEventLaunchControl2();
        updateEventLaunchControl3();
        updateEventHighPerformanceWarnStatus();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;

        mQueueTurbo.clear();

        updateTelltale();

        processPrivate_EngineOilTemp();
        processSinkConstantEngineOilTemp();

        processPrivate_EngineCoolantTemp();
        processSinkConstantEngineCoolantTemp();

        processPrivate_Turbo();
        processSinkConstantTurbo();

        processPrivate_Torque();
        processSinkConstantTorque();
        processSinkConstantTorqueEv();

        processPrivate_MotorTemp();
        processSinkConstantMotorTemp();

        processPrivate_EvBatteryTemp();
        processSinkConstantEvBatteryTemp();

        updateConstantVgsOnOff();

        updateEventLaunchControl1();
        updateEventLaunchControl2();
        updateEventLaunchControl3();
        updateEventHighPerformanceWarnStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateTelltale();

        processPrivate_EngineOilTemp();
        processSinkConstantEngineOilTemp();

        processPrivate_EngineCoolantTemp();
        processSinkConstantEngineCoolantTemp();

        processPrivate_Turbo();
        processSinkConstantTurbo();

        processPrivate_Torque();
        processSinkConstantTorque();
        processSinkConstantTorqueEv();

        processPrivate_MotorTemp();
        processSinkConstantMotorTemp();

        processPrivate_EvBatteryTemp();
        processSinkConstantEvBatteryTemp();

        updateConstantVgsOnOff();

        updateEventLaunchControl1();
        updateEventLaunchControl2();
        updateEventLaunchControl3();
        updateEventHighPerformanceWarnStatus();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;

        updateEventLaunchControl3();
        updateEventHighPerformanceWarnStatus();
    }

    void onTempChanged(const ArgumentsTempChanged& args) {
        mEngineOilTempValue = args.mInput_EngineOilTempValue;
        mOutTempUnitStatus = args.mInput_OutTempUnitStatus;
        mEngineCoolantTempValue = args.mInput_EngineCoolantTempValue;
        mEvMotorFrontTempValue = args.mInput_EvMotorFrontTempValue;
        mEvMotorFrontTempValue_timeout = ISTIMEOUT(args.mInput_EvMotorFrontTempValue);
        mEvMotorRearTempValue = args.mInput_EvMotorRearTempValue;
        mEvMotorRearTempValue_timeout = ISTIMEOUT(args.mInput_EvMotorRearTempValue);
        mEvBatteryTempValue = args.mInput_EvBatteryTempValue;

        processPrivate_EngineOilTemp();
        processSinkConstantEngineOilTemp();

        processPrivate_EngineCoolantTemp();
        processSinkConstantEngineCoolantTemp();

        processPrivate_Turbo();
        processSinkConstantTurbo();

        processPrivate_Torque();
        processSinkConstantTorque();
        processSinkConstantTorqueEv();

        processPrivate_MotorTemp();
        processSinkConstantMotorTemp();

        processPrivate_EvBatteryTemp();
        processSinkConstantEvBatteryTemp();
    }

    void onTurboChanged(const ArgumentsTurboChanged& args) {
        mAtmosphericPressureValue = static_cast<HDouble>(args.mInput_AtmosphericPressureValue);
        mTurboBoostPressureValue = static_cast<HDouble>(args.mInput_TurboBoostPressureValue);
        mTurboBoostPressureValue_timeout = ISTIMEOUT(args.mInput_TurboBoostPressureValue);
        mEngineOnStatus = args.mInter_EngineOnStatus;
        mTurboPressureUnitSetStatus = args.mInter_TurboPressureUnitSetStatus;

        processPrivate_Turbo();
        processSinkConstantTurbo();
    }

    void onTorqueChanged(const ArgumentsTorqueChanged& args) {
        mEngineTorqueAfterCorrectionValue = args.mInput_EngineTorqueAfterCorrectionValue;
        mEngineTorqueOfFrictionLossValue = args.mInput_EngineTorqueOfFrictionLossValue;
        mEvMotorTorqueFrontValue = args.mInput_EvMotorTorqueFrontValue;
        mEvMotorTorqueRearValue = args.mInput_EvMotorTorqueRearValue;
        mEvMotorTorqueFrontValue_timeout = ISTIMEOUT(args.mInput_EvMotorTorqueFrontValue);
        mEvMotorTorqueRearValue_timeout = ISTIMEOUT(args.mInput_EvMotorTorqueRearValue);
        mTorqueUnitSetStatus = args.mInter_TorqueUnitSetStatus;

        processPrivate_Torque();
        processSinkConstantTorque();
        processSinkConstantTorqueEv();
    }

    void onLaunchControl1Changed(const ArgumentsLaunchControl1Changed& args) {
        mLaunchControlType1Status = args.mInput_LaunchControlType1Status;

        updateEventLaunchControl1();
    }

    void onLaunchControl2Changed(const ArgumentsLaunchControl2Changed& args) {
        mLaunchControlType2Status = args.mInput_LaunchControlType2Status;

        updateEventLaunchControl2();
    }

    void onLaunchControl3Changed(const ArgumentsLaunchControl3Changed& args) {
        mLaunchControlType3Status = args.mInput_LaunchControlType3Status;

        updateTelltale();

        updateEventLaunchControl3();
    }

    void onVgsOnOffStatusChanged(const ArgumentsVgsOnOffStatusChanged& args) {
        mVgsOnOffStatus = args.mInput_VgsOnOffStatus;

        updateConstantVgsOnOff();
    }

    void onHighPerformanceWarnStatusChanged(const ArgumentsHighPerformanceWarnStatusChanged& args) {
        mHighPerformanceWarnStatusChanged = args.mInput_HighPerformanceWarnStatus;

        updateEventHighPerformanceWarnStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    inline HDouble calculateEngineOilCelsius(const HUInt64 engineOilTempValue) {
        return (static_cast<HDouble>(engineOilTempValue) * 0.75) - 40.0;
    }

    inline HDouble toFahrenheit(const HDouble celsius) {
        return (2.0 * (celsius + 15.0)) + ((10.0 - celsius) / 5.0);
    }

    void processPrivate_EngineOilTemp() {
        if (mIgnElapsed < IgnElapsed::ON_0ms) {
            return;
        }

        if (mEngineOilTempValue == 255) {
            mPrivate_EngineOilTemp_value = 0;
        } else if (mEngineOilTempValue >= 0 && mEngineOilTempValue <= 254) {
            if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                mPrivate_EngineOilTemp_value = std::lround(calculateEngineOilCelsius(mEngineOilTempValue));
            } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                mPrivate_EngineOilTemp_value = std::lround(toFahrenheit(calculateEngineOilCelsius(mEngineOilTempValue)));
            } else if (mOutTempUnitStatus == OutTempUnitStatus::ERROR) {
                DError() << "Engine Oil Temp is ERROR";
            } else {
                // 0
            }
        } else {
            mPrivate_EngineOilTemp_value = 0;
        }
    }

    inline HDouble calculateEngineCoolantCelsius(const HUInt64 engineCoolantTempValue) {
        return (static_cast<HDouble>(engineCoolantTempValue) * 0.75) - 48.0;
    }

    void processPrivate_EngineCoolantTemp() {
        if (mIgnElapsed < IgnElapsed::ON_0ms) {
            return;
        }

        if (mEngineCoolantTempValue == 255) {
            mPrivate_EngineCoolantTemp_value = 0;
        } else if (mEngineCoolantTempValue >= 0 && mEngineCoolantTempValue <= 254) {
            if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                mPrivate_EngineCoolantTemp_value = std::lround(calculateEngineCoolantCelsius(mEngineCoolantTempValue));
            } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                mPrivate_EngineCoolantTemp_value =
                    std::lround(toFahrenheit(calculateEngineCoolantCelsius(mEngineCoolantTempValue)));
            } else if (mOutTempUnitStatus == OutTempUnitStatus::ERROR) {
                DError() << "Engine Coolant Temp is ERROR";
            } else {
                // 0
            }
        } else {
            mPrivate_EngineCoolantTemp_value = 0;
        }
    }

    void processPrivate_Turbo() {
        HDouble private_Turbo_value = 0.0;
        HDouble private_TurbohPa_value = 0.0;
        HBool hasToPush = true;

        // Private_TurbohPa_value
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            private_TurbohPa_value = ((1.322 * static_cast<HDouble>(mTurboBoostPressureValue)) -
                                      (1.333 * ((10.731613 * static_cast<HDouble>(mAtmosphericPressureValue)) + 458.98)));
        }

        // Private_Turbo_value
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEngineOnStatus == EngineOnStatus::OFF) {
                mQueueTurbo.clear();
                private_Turbo_value = 0.0;
            } else if (mEngineOnStatus == EngineOnStatus::ON) {
                if (mTurboBoostPressureValue_timeout) {
                    mQueueTurbo.clear();
                    private_Turbo_value = 0.0;
                } else if (mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::KPA ||
                           mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::PSI ||
                           mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::BAR) {
                    private_Turbo_value = private_TurbohPa_value;
                } else {
                    DError() << "Turbo ERROR";
                    hasToPush = false;
                }
            } else {
                hasToPush = false;
            }
        } else {
            hasToPush = false;
        }

        if (hasToPush) {
            if (mQueueTurbo.size() >= kQueueSize) {
                mQueueTurbo.pop_front();
            }
            mQueueTurbo.push_back(private_Turbo_value);
        }
    }

    inline HDouble calculateNMValue(const HUInt64 value) {
        HDouble calculatedValue = 0.0;
        if (0 <= value && value <= 2047) {
            calculatedValue = static_cast<HDouble>(value);  // positive
        } else if (2048 <= value && value <= 4095) {
            calculatedValue = 2048.0 - static_cast<HDouble>(value);  // negative
        } else {
            // no operation
        }
        return calculatedValue;
    }

    void processPrivate_Torque() {
        HDouble private_Nmval = 0.0;
        HDouble private_Torque_value_Nm = 0.0;
        HDouble private_TorqueEvFront_value_Nm = 0.0;
        HDouble private_TorqueEvRear_value_Nm = 0.0;
        HBool hasToPush = true;

        // Private_Nmval
        if ((mIgnElapsed >= IgnElapsed::ON_0ms) &&
            (mEngineTorqueAfterCorrectionValue <= 4095 && mEngineTorqueOfFrictionLossValue <= 4095)) {
            private_Nmval =
                calculateNMValue(mEngineTorqueAfterCorrectionValue) - calculateNMValue(mEngineTorqueOfFrictionLossValue);
        }

        // private_Torque_value_Nm
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mQueueTorqueNm.clear();
            private_Torque_value_Nm = 0.0;
            mQueueTorqueEvFrontNm.clear();
            private_TorqueEvFront_value_Nm = 0.0;
            mQueueTorqueEvRearNm.clear();
            private_TorqueEvRear_value_Nm = 0.0;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEngineOnStatus == EngineOnStatus::OFF) {
                mQueueTorqueNm.clear();
                private_Torque_value_Nm = 0.0;
                mQueueTorqueEvFrontNm.clear();
                private_TorqueEvFront_value_Nm = 0.0;
                mQueueTorqueEvRearNm.clear();
                private_TorqueEvRear_value_Nm = 0.0;
            } else if (mEngineOnStatus == EngineOnStatus::ON &&
                       (mTorqueUnitSetStatus == TorqueUnitSetStatus::N_M || mTorqueUnitSetStatus == TorqueUnitSetStatus::LB_FT)) {
                private_Torque_value_Nm = private_Nmval;
                if (mEvMotorTorqueFrontValue_timeout == true) {
                    mQueueTorqueEvFrontNm.clear();
                    private_TorqueEvFront_value_Nm = 0.0;
                } else {
                    if (mEvMotorTorqueFrontValue <= 8191) {
                        private_TorqueEvFront_value_Nm = static_cast<HDouble>(mEvMotorTorqueFrontValue) * 0.125;
                    } else {
                        private_TorqueEvFront_value_Nm = (static_cast<HDouble>(mEvMotorTorqueFrontValue) - 16384.0) * 0.125;
                    }
                }
                if (mEvMotorTorqueRearValue_timeout == true) {
                    mQueueTorqueEvRearNm.clear();
                    private_TorqueEvRear_value_Nm = 0.0;
                } else {
                    if (mEvMotorTorqueRearValue <= 8191) {
                        private_TorqueEvRear_value_Nm = static_cast<HDouble>(mEvMotorTorqueRearValue) * 0.125;
                    } else {
                        private_TorqueEvRear_value_Nm = (static_cast<HDouble>(mEvMotorTorqueRearValue) - 16384.0) * 0.125;
                    }
                }
            } else {
                hasToPush = false;
            }
        } else {
            hasToPush = false;
        }

        if (hasToPush) {
            if (mQueueTorqueNm.size() >= kQueueSize) {
                mQueueTorqueNm.pop_front();
            }
            if (mQueueTorqueEvFrontNm.size() >= kQueueSize) {
                mQueueTorqueEvFrontNm.pop_front();
            }
            if (mQueueTorqueEvRearNm.size() >= kQueueSize) {
                mQueueTorqueEvRearNm.pop_front();
            }
            mQueueTorqueNm.push_back(private_Torque_value_Nm);
            mQueueTorqueEvFrontNm.push_back(private_TorqueEvFront_value_Nm);
            mQueueTorqueEvRearNm.push_back(private_TorqueEvRear_value_Nm);
        }
    }

    inline HDouble calculateTempFahrenheit(const HInt64 TempValue) {
        return static_cast<HDouble>(TempValue) * (9.0 / 5.0) + 32.0;
    }

    void processPrivate_MotorTemp() {
        mPrivate_MotorFrontTemp_value = 0;
        mPrivate_MotorRearTemp_value = 0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mEvMotorFrontTempValue != 255 && mEvMotorFrontTempValue_timeout == false) {
                if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                    mPrivate_MotorFrontTemp_value = mEvMotorFrontTempValue - 40;
                } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                    mPrivate_MotorFrontTemp_value = std::lround(calculateTempFahrenheit(mEvMotorFrontTempValue - 40));
                } else {
                    // default
                }
            }
            if (mEvMotorRearTempValue != 255 && mEvMotorRearTempValue_timeout == false) {
                if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                    mPrivate_MotorRearTemp_value = mEvMotorRearTempValue - 40;
                } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                    mPrivate_MotorRearTemp_value = std::lround(calculateTempFahrenheit(mEvMotorRearTempValue - 40));
                } else {
                    // default
                }
            }
        }
    }

    void processPrivate_EvBatteryTemp() {
        mPrivate_EvBatteryTemp_value = 0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mEvBatteryTempValue >= 0 && mEvBatteryTempValue <= 120) {
                if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                    mPrivate_EvBatteryTemp_value = mEvBatteryTempValue;
                } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                    mPrivate_EvBatteryTemp_value = std::lround(calculateTempFahrenheit(mEvBatteryTempValue));
                } else {
                    // default
                }
            } else if (mEvBatteryTempValue >= 206 && mEvBatteryTempValue <= 255) {
                if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                    mPrivate_EvBatteryTemp_value = mEvBatteryTempValue - 256;
                } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                    mPrivate_EvBatteryTemp_value = std::lround(calculateTempFahrenheit(mEvBatteryTempValue - 256));
                } else {
                    // default
                }
            } else {
                // default
            }
        }
    }

    void updateTelltale() {
        SFCHigh_Performance_GaugeTelltaleLaunchControlStat stat = SFCHigh_Performance_GaugeTelltaleLaunchControlStat::OFF;
        SFCHigh_Performance_GaugeTelltaleLaunchControlStatOptional statOptional =
            SFCHigh_Performance_GaugeTelltaleLaunchControlStatOptional::OFF;
        SFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueAOptional blinkAOptional =
            SFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueAOptional::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            switch (mLaunchControlType3Status) {
                case LaunchControlType3Status::UNAVAILABLE:
                    stat = SFCHigh_Performance_GaugeTelltaleLaunchControlStat::OPTIONAL;
                    statOptional = SFCHigh_Performance_GaugeTelltaleLaunchControlStatOptional::DEACTIVATE;
                    break;
                case LaunchControlType3Status::CONDITION_MET:
                    stat = SFCHigh_Performance_GaugeTelltaleLaunchControlStat::OPTIONAL;
                    statOptional = SFCHigh_Performance_GaugeTelltaleLaunchControlStatOptional::ACTIVATE;
                    break;
                case LaunchControlType3Status::READY:
                case LaunchControlType3Status::ACT:
                    stat = SFCHigh_Performance_GaugeTelltaleLaunchControlStat::BLINK2;
                    blinkAOptional = SFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueAOptional::ACTIVATE;
                    break;
                case LaunchControlType3Status::OFF:
                default:
                    // no operation
                    break;
            }
        }

        setSFCHigh_Performance_GaugeTelltaleLaunchControlStat(stat);
        setSFCHigh_Performance_GaugeTelltaleLaunchControlStatOptional(statOptional);
        setSFCHigh_Performance_GaugeTelltaleLaunchControlBlinkValueAOptional(blinkAOptional);
    }

    void processSinkConstantEngineOilTemp() {
        HInt64 mEngineOilTemp_value = 0;
        SFCHigh_Performance_GaugeConstantEngineOilTempUnitStat tempUnitStat{
            SFCHigh_Performance_GaugeConstantEngineOilTempUnitStat::DISPLAY_OFF};

        if (mIgnElapsed == IgnElapsed::ON_500ms &&
            (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS || mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT)) {
            mEngineOilTemp_value = mPrivate_EngineOilTemp_value;
        }

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            switch (mOutTempUnitStatus) {
                case OutTempUnitStatus::CELSIUS:
                    tempUnitStat = SFCHigh_Performance_GaugeConstantEngineOilTempUnitStat::CELSIUS;
                    break;
                case OutTempUnitStatus::FAHRENHEIT:
                    tempUnitStat = SFCHigh_Performance_GaugeConstantEngineOilTempUnitStat::FAHRENHEIT;
                    break;
                default:
                    break;
            }
        }

        setSFCHigh_Performance_GaugeConstantEngineOilTempValue(mEngineOilTemp_value);
        setSFCHigh_Performance_GaugeConstantEngineOilTempUnitStat(tempUnitStat);
    }

    void processSinkConstantEngineCoolantTemp() {
        HInt64 mEngineCoolantTemp_value = 0;
        SFCHigh_Performance_GaugeConstantEngineCoolantTempUnitStat tempUnitStat{
            SFCHigh_Performance_GaugeConstantEngineCoolantTempUnitStat::DISPLAY_OFF};

        if (mIgnElapsed == IgnElapsed::ON_500ms &&
            (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS || mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT)) {
            mEngineCoolantTemp_value = mPrivate_EngineCoolantTemp_value;
        }

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            switch (mOutTempUnitStatus) {
                case OutTempUnitStatus::CELSIUS:
                    tempUnitStat = SFCHigh_Performance_GaugeConstantEngineCoolantTempUnitStat::CELSIUS;
                    break;
                case OutTempUnitStatus::FAHRENHEIT:
                    tempUnitStat = SFCHigh_Performance_GaugeConstantEngineCoolantTempUnitStat::FAHRENHEIT;
                    break;
                default:
                    break;
            }
        }

        setSFCHigh_Performance_GaugeConstantEngineCoolantTempValue(mEngineCoolantTemp_value);
        setSFCHigh_Performance_GaugeConstantEngineCoolantTempUnitStat(tempUnitStat);
    }

    void processSinkConstantTurbo() {
        auto iter = mQueueTurbo.begin();
        HDouble queueSize = static_cast<HDouble>(mQueueTurbo.size());
        HDouble sum = 0.0;
        HDouble ret = 0.0;
        while (iter != mQueueTurbo.end()) {
            sum += *iter;
            iter++;
        }

        if (queueSize > 0.0) {
            ret = sum / queueSize;
        }

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::KPA) {
                ret = static_cast<HDouble>(std::lround(ret * 0.1));
                setSFCHigh_Performance_GaugeConstantTurboUnitStat(SFCHigh_Performance_GaugeConstantTurboUnitStat::KPA);
            } else if (mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::PSI) {
                ret = static_cast<HDouble>(std::lround(ret * 0.0145));
                setSFCHigh_Performance_GaugeConstantTurboUnitStat(SFCHigh_Performance_GaugeConstantTurboUnitStat::PSI);
            } else if (mTurboPressureUnitSetStatus == TurboPressureUnitSetStatus::BAR) {
                ret = ret * 0.001;
                ret = std::floor(ret * 100.0) / 100.0;
                setSFCHigh_Performance_GaugeConstantTurboUnitStat(SFCHigh_Performance_GaugeConstantTurboUnitStat::BAR);
            } else {
                // No process any.
            }
        } else {
            ret = 0.0;
        }

        if (ret <= 0.0) {
            setSFCHigh_Performance_GaugeConstantTurboGaugeValue(0.0);
        } else if (0.0 < ret && ret <= 999.0) {
            setSFCHigh_Performance_GaugeConstantTurboGaugeValue(ret);
        } else {
            setSFCHigh_Performance_GaugeConstantTurboGaugeValue(999.0);
        }
    }

    void processSinkConstantTorque() {
        auto iter = mQueueTorqueNm.begin();
        HDouble sum = 0.0;
        while (iter != mQueueTorqueNm.end()) {
            sum += *iter;
            iter++;
        }
        HDouble ret = sum / 20.0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mTorqueUnitSetStatus == TorqueUnitSetStatus::N_M) {
                setSFCHigh_Performance_GaugeConstantTorqueUnitStat(SFCHigh_Performance_GaugeConstantTorqueUnitStat::N_M);
            } else if (mTorqueUnitSetStatus == TorqueUnitSetStatus::LB_FT) {
                ret = ret * 0.738;
                setSFCHigh_Performance_GaugeConstantTorqueUnitStat(SFCHigh_Performance_GaugeConstantTorqueUnitStat::LB_FT);
            } else {
                // No process any.
            }
        } else {
            ret = 0.0;
        }

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            setSFCHigh_Performance_GaugeConstantTorqueGaugeValue(std::lround(ret));
        } else {
            setSFCHigh_Performance_GaugeConstantTorqueGaugeValue(0.0);
        }
    }

    void processSinkConstantTorqueEv() {
        auto iter_front = mQueueTorqueEvFrontNm.begin();
        HDouble sum_front = 0.0;
        while (iter_front != mQueueTorqueEvFrontNm.end()) {
            sum_front += *iter_front;
            iter_front++;
        }
        HDouble ret_front = sum_front / 20.0;

        auto iter_rear = mQueueTorqueEvRearNm.begin();
        HDouble sum_rear = 0.0;
        while (iter_rear != mQueueTorqueEvRearNm.end()) {
            sum_rear += *iter_rear;
            iter_rear++;
        }
        HDouble ret_rear = sum_rear / 20.0;

        HDouble ret_2wd = 0.0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mTorqueUnitSetStatus == TorqueUnitSetStatus::LB_FT) {
                ret_front = ret_front * 0.738;
                ret_rear = ret_rear * 0.738;
            }
            if (mEvMotorTorqueRearValue_timeout) {
                ret_2wd = ret_front;
            } else if (mEvMotorTorqueFrontValue_timeout) {
                ret_2wd = ret_rear;
            } else if (mEvMotorTorqueRearValue_timeout == false && mEvMotorTorqueFrontValue_timeout == false) {
                ret_2wd = ret_front;
            } else {
                // nothing
            }
        } else {
            ret_front = 0.0;
            ret_rear = 0.0;
        }

        setSFCHigh_Performance_GaugeConstantTorqueEvFrontValue(std::lround(ret_front));
        setSFCHigh_Performance_GaugeConstantTorqueEvRearValue(std::lround(ret_rear));
        setSFCHigh_Performance_GaugeConstantTorqueEv2wdValue(std::lround(ret_2wd));
    }

    void processSinkConstantMotorTemp() {
        HInt64 mMotorFrontTemp_value = 0;
        HInt64 mMotorRearTemp_value = 0;
        HInt64 mMotor2wdTemp_value = 0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            mMotorFrontTemp_value = mPrivate_MotorFrontTemp_value;
            mMotorRearTemp_value = mPrivate_MotorRearTemp_value;
            if (mEvMotorFrontTempValue_timeout) {
                mMotor2wdTemp_value = mPrivate_MotorRearTemp_value;
            } else if (mEvMotorRearTempValue_timeout) {
                mMotor2wdTemp_value = mPrivate_MotorFrontTemp_value;
            } else if (mEvMotorFrontTempValue_timeout == false && mEvMotorRearTempValue_timeout == false) {
                mMotor2wdTemp_value = mPrivate_MotorFrontTemp_value;
            } else {
                // nothing
            }
        }

        setSFCHigh_Performance_GaugeConstantMotorFrontTempValue(mMotorFrontTemp_value);
        setSFCHigh_Performance_GaugeConstantMotorRearTempValue(mMotorRearTemp_value);
        setSFCHigh_Performance_GaugeConstantMotor2wdTempValue(mMotor2wdTemp_value);
    }

    void processSinkConstantEvBatteryTemp() {
        HInt64 mEvBatteryTemp_value = 0;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            mEvBatteryTemp_value = mPrivate_EvBatteryTemp_value;
        }

        setSFCHigh_Performance_GaugeConstantEvBatteryTempValue(mEvBatteryTemp_value);
    }

    void updateEventLaunchControl1() {
        std::string eventID;
        std::string linkedSoundID;
        std::string prevEventID;

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            switch (mLaunchControlType1Status) {
                case LaunchControlType1Status::READY:
                    eventID = "E24802";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case LaunchControlType1Status::ACT:
                    eventID = "E24803";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case LaunchControlType1Status::NOT_AVAILABLE:
                    eventID = "E24804";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.High_Performance_Gauge.Event.LaunchControlType1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCHigh_Performance_GaugeEventLaunchControlType1Stat(SFCHigh_Performance_GaugeEventLaunchControlType1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCHigh_Performance_GaugeEventLaunchControlType1ID(eventID);
            setSFCHigh_Performance_GaugeEventLaunchControlType1Stat(SFCHigh_Performance_GaugeEventLaunchControlType1Stat::ON);
            setSFCHigh_Performance_GaugeEventLaunchControlType1LinkedSoundID(linkedSoundID);
        }
    }

    void updateEventLaunchControl2() {
        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            switch (mLaunchControlType2Status) {
                case LaunchControlType2Status::READY:
                    eventID = "E24805";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case LaunchControlType2Status::ACT:
                    eventID = "E24806";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case LaunchControlType2Status::LC_CONDITION_NOT_MET:
                    eventID = "E24822";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.High_Performance_Gauge.Event.LaunchControlType2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCHigh_Performance_GaugeEventLaunchControlType2Stat(SFCHigh_Performance_GaugeEventLaunchControlType2Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCHigh_Performance_GaugeEventLaunchControlType2ID(eventID);
            setSFCHigh_Performance_GaugeEventLaunchControlType2Stat(SFCHigh_Performance_GaugeEventLaunchControlType2Stat::ON);
            setSFCHigh_Performance_GaugeEventLaunchControlType2LinkedSoundID(linkedSoundID);
        }
    }

    void updateEventLaunchControl3() {
        std::string eventID;
        std::string prevEventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mConfigHighPerformance == ConfigHighPerformance::N &&
                mLaunchControlType3Status == LaunchControlType3Status::READY) {
                eventID = "E24817";
            } else if (mConfigHighPerformance == ConfigHighPerformance::N &&
                       mLaunchControlType3Status == LaunchControlType3Status::ACT) {
                eventID = "E24818";
            } else if (mConfigHighPerformance != ConfigHighPerformance::N &&
                       mLaunchControlType3Status == LaunchControlType3Status::READY) {
                eventID = "E24824";
            } else if (mConfigHighPerformance != ConfigHighPerformance::N &&
                       mLaunchControlType3Status == LaunchControlType3Status::ACT) {
                eventID = "E24825";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.High_Performance_Gauge.Event.LaunchControlType3.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCHigh_Performance_GaugeEventLaunchControlType3Stat(SFCHigh_Performance_GaugeEventLaunchControlType3Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCHigh_Performance_GaugeEventLaunchControlType3ID(eventID);
            setSFCHigh_Performance_GaugeEventLaunchControlType3Stat(SFCHigh_Performance_GaugeEventLaunchControlType3Stat::ON);
        }
    }

    void updateConstantVgsOnOff() {
        if (mIgnElapsed >= IgnElapsed::ON_0ms && mVgsOnOffStatus == VgsOnOffStatus::VGS_ON) {
            setSFCHigh_Performance_GaugeConstantVgsOnOffStat(SFCHigh_Performance_GaugeConstantVgsOnOffStat::VGS_ON);
        } else {
            setSFCHigh_Performance_GaugeConstantVgsOnOffStat(SFCHigh_Performance_GaugeConstantVgsOnOffStat::OFF);
        }
    }

    void updateEventHighPerformanceWarnStatus() {
        std::string eventID;
        std::string prevEventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mConfigHighPerformance != ConfigHighPerformance::N &&
                mHighPerformanceWarnStatusChanged == HighPerformanceWarnStatus::VGS_CONDITION_NOT_MET) {
                eventID = "E24819";
            } else if (mConfigHighPerformance == ConfigHighPerformance::N &&
                       mHighPerformanceWarnStatusChanged == HighPerformanceWarnStatus::VGS_CONDITION_NOT_MET) {
                eventID = "E24820";
            } else if (mConfigHighPerformance != ConfigHighPerformance::N &&
                       mHighPerformanceWarnStatusChanged == HighPerformanceWarnStatus::LC_CONDITION_NOT_MET) {
                eventID = "E24823";
            } else if (mConfigHighPerformance == ConfigHighPerformance::N &&
                       mHighPerformanceWarnStatusChanged == HighPerformanceWarnStatus::LC_CONDITION_NOT_MET) {
                eventID = "E24821";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.High_Performance_Gauge.Event.HighPerformanceWarnStatus.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusStat(
                SFCHigh_Performance_GaugeEventHighPerformanceWarnStatusStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusID(eventID);
            setSFCHigh_Performance_GaugeEventHighPerformanceWarnStatusStat(
                SFCHigh_Performance_GaugeEventHighPerformanceWarnStatusStat::ON);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using ConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using OutTempUnitStatus = ArgumentsTempChanged::Input_OutTempUnitStatus;
    using EngineOnStatus = ArgumentsTurboChanged::Inter_EngineOnStatus;
    using TurboPressureUnitSetStatus = ArgumentsTurboChanged::Inter_TurboPressureUnitSetStatus;
    using TorqueUnitSetStatus = ArgumentsTorqueChanged::Inter_TorqueUnitSetStatus;
    using LaunchControlType1Status = ArgumentsLaunchControl1Changed::Input_LaunchControlType1Status;
    using LaunchControlType2Status = ArgumentsLaunchControl2Changed::Input_LaunchControlType2Status;
    using LaunchControlType3Status = ArgumentsLaunchControl3Changed::Input_LaunchControlType3Status;
    using VgsOnOffStatus = ArgumentsVgsOnOffStatusChanged::Input_VgsOnOffStatus;
    using HighPerformanceWarnStatus = ArgumentsHighPerformanceWarnStatusChanged::Input_HighPerformanceWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;

    HUInt64 mEngineOilTempValue = 0;
    OutTempUnitStatus mOutTempUnitStatus = OutTempUnitStatus::ERROR;
    HUInt64 mEngineCoolantTempValue = 0;
    HUInt64 mEvMotorFrontTempValue = 0;
    HBool mEvMotorFrontTempValue_timeout = false;
    HUInt64 mEvMotorRearTempValue = 0;
    HBool mEvMotorRearTempValue_timeout = false;
    HUInt64 mEvBatteryTempValue = 0;

    HDouble mAtmosphericPressureValue = 0.0;
    HDouble mTurboBoostPressureValue = 0.0;
    HBool mTurboBoostPressureValue_timeout = false;
    EngineOnStatus mEngineOnStatus = EngineOnStatus::MAX;
    TurboPressureUnitSetStatus mTurboPressureUnitSetStatus = TurboPressureUnitSetStatus::BAR;

    HUInt64 mEngineTorqueAfterCorrectionValue = 0;
    HUInt64 mEngineTorqueOfFrictionLossValue = 0;
    HUInt64 mEvMotorTorqueFrontValue = 0;
    HUInt64 mEvMotorTorqueRearValue = 0;
    HBool mEvMotorTorqueFrontValue_timeout = false;
    HBool mEvMotorTorqueRearValue_timeout = false;
    TorqueUnitSetStatus mTorqueUnitSetStatus = TorqueUnitSetStatus::MAX;

    HInt64 mPrivate_EngineOilTemp_value = 0;
    HInt64 mPrivate_EngineCoolantTemp_value = 0;
    HInt64 mPrivate_MotorFrontTemp_value = 0;
    HInt64 mPrivate_MotorRearTemp_value = 0;
    HInt64 mPrivate_EvBatteryTemp_value = 0;
    std::list<HDouble> mQueueTurbo = {
        0.0,
    };
    std::list<HDouble> mQueueTorqueNm = {
        0.0,
    };
    std::list<HDouble> mQueueTorqueEvFrontNm = {
        0.0,
    };
    std::list<HDouble> mQueueTorqueEvRearNm = {
        0.0,
    };

    LaunchControlType1Status mLaunchControlType1Status = LaunchControlType1Status::OFF;
    LaunchControlType2Status mLaunchControlType2Status = LaunchControlType2Status::OFF;
    LaunchControlType3Status mLaunchControlType3Status = LaunchControlType3Status::OFF;

    VgsOnOffStatus mVgsOnOffStatus = VgsOnOffStatus::OFF;

    HighPerformanceWarnStatus mHighPerformanceWarnStatusChanged = HighPerformanceWarnStatus::OFF;

    // interval for input is 10ms, average duration is 200ms
    static constexpr HUInt64 kQueueSize = 20;
};

}  // namespace ccos

#endif  // SFSS_High_Performance_Gauge_H
