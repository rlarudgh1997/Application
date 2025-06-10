/**
 * @file Tachometer.hpp
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
#ifndef SFSS_Tachometer_H
#define SFSS_Tachometer_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "TachometerBase.hpp"

namespace ccos {

// SFC Version : 16.0.1
// Reference : [PT400] Tachometer Contains : Constant. Recommend: VALUE_CHANGED
class Tachometer : public TachometerBase {
public:
    Tachometer() = default;
    ~Tachometer() override = default;
    Tachometer(const Tachometer& other) = delete;
    Tachometer(Tachometer&& other) noexcept = delete;
    Tachometer& operator=(const Tachometer& other) = delete;
    Tachometer& operator=(Tachometer&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        calculateMaxRPMStat();
        updateRpmAndRedZoneConstant();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mRpm = 0;
        updateRpmAndRedZoneConstant();
    }

    void onVehicleInfoChanged(const ArgumentsVehicleInfoChanged& args) {
        // 4.2 입력 신호 공차
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;
        mConfigFuelType = args.mInter_ConfigFuelType;
        calculateMaxRPMStat();
        updateRpmAndRedZoneConstant();
    }

    void onDmsTypeChanged(const ArgumentsDmsTypeChanged& args) {
        mTelltaleDmsStat = args.mDMSStat;
        mTelltaleDmsType = args.mDMSStatOptional;
        updateRpmAndRedZoneConstant();
    }

    void onEngineRpmChanged(const ArgumentsEngineRpmChanged& args) {
        mEngineRpm = args.mInput_EngineRpmValue;
        mEngineRpmErrorStatus = args.mInput_EngineRpmErrorStatus;
        mEngineRpmDisplayReqValue = args.mInput_EngineRpmDisplayReqValue;
        updateRpmAndRedZoneConstant();
    }

    void onEmsVirtualRpmChanged(const ArgumentsEmsVirtualRpmChanged& args) {
        mEmsVirtualRpm = static_cast<HDouble>(args.mInput_EmsVirtualRpmValue);
        if (ISTIMEOUT(args.mInput_EmsVirtualRpmEnableStatus) == true) {
            mEmsVirtualRpmEnableStatus = EmsVirtualRpmEnableStatus::UNHANDLED_TIMEOUT;
        } else {
            mEmsVirtualRpmEnableStatus = args.mInput_EmsVirtualRpmEnableStatus;
        }
        updateRpmAndRedZoneConstant();
    }

    void onTcuRpmChanged(const ArgumentsTcuRpmChanged& args) {
        mTcuRpmFromTCU = args.mInput_TcuRpmValueFromTCU;
        mTcuRpmFromHDCT = args.mInput_TcuRpmValueFromHDCT;
        mTcuRpmEnableStatus = args.mInput_TcuRpmEnableStatus;
        updateRpmAndRedZoneConstant();
    }

    void onEngineTempChanged(const ArgumentsEngineTempChanged& args) {
        mEngineOilTempValue = args.mInput_EngineOilTempValue;
        mEngineOilTempValueTimeout = ISTIMEOUT(args.mInput_EngineOilTempValue);
        updateRpmAndRedZoneConstant();
    }

    void onPtLogicControlTypeChanged(const ArgumentsPtLogicControlTypeChanged& args) {
        mPTLogicControlType = args.mInter_PTLogicControlType;
        calculateMaxRPMStat();
        updateRpmAndRedZoneConstant();
    }

    void onEngineRpmNewErrorStatusChanged(const ArgumentsEngineRpmNewErrorStatusChanged& args) {
        mEngineRpmNewErrorStatus = args.mInput_EngineRpmNewErrorStatus;
        updateRpmAndRedZoneConstant();
    }

    void onRpmGaugeMaxReqStatusChanged(const ArgumentsRpmGaugeMaxReqStatusChanged& args) {
        mRpmGaugeMaxReqStatus = args.mInput_RpmGaugeMaxReqStatus;
        calculateMaxRPMStat();
        updateRpmAndRedZoneConstant();
    }

    void onRpmGaugeRedStartPointReqStatusChanged(const ArgumentsRpmGaugeRedStartPointReqStatusChanged& args) {
        mRpmGaugeRedStartPointReqStatus = args.mInput_RpmGaugeRedStartPointReqStatus;
        updateRpmAndRedZoneConstant();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateRpmAndRedZoneConstant() {
        updateRpmConstant();             // IMG_Rpm_value ( Private_RpmValue + IMG_MaxRpm_stat )
        updateRedZoneDisplay();          // Private_RedZoneDisplay
        updateConstantRedZoneDisplay();  // IMG_RpmRedZone_stat

        if ((mConfigHighPerformance != ConfigHighPerformance::N &&
             (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV)) ||
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV)) {
            updateRpmRedZoneRangeStat();
        } else {
            updateRpmRedZoneRangeStat_EngineOilTemp();
        }
    }

    void updateRpmConstant() {
        // 5.2.1 타코미터 표시
        if (mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCTachometerConstantRpmValue(mRpm);
            calculateRPMDampingStat();
            return;
        }

        calculateRPM();
        if (mRpm > mRpmMax) {
            mRpm = mRpmMax;
        }
        setSFCTachometerConstantRpmValue(mRpm);
        calculateRPMDampingStat();
    }

    // 5.2.2.1 레드존 표시 여부
    void updateConstantRedZoneDisplay() {
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            mPrivate_RedZoneDisplay = SFCTachometerConstantRedZoneExceptNbrandStat::OFF;
            return;
        }
        SFCTachometerConstantRedZoneExceptNbrandStat stat = SFCTachometerConstantRedZoneExceptNbrandStat::OFF;

        // IMG_RpmRedZone_stat
        if (mConfigHighPerformance != ConfigHighPerformance::N) {
            if (mTelltaleDmsType == TelltaleDms::SPORT && mTelltaleDmsStat != TelltaleDmsStat::OFF) {
                stat = SFCTachometerConstantRedZoneExceptNbrandStat::ON;
            } else {
                stat = mPrivate_RedZoneDisplay;
            }
        } else {
            stat = SFCTachometerConstantRedZoneExceptNbrandStat::ON;
        }
        setSFCTachometerConstantRedZoneExceptNbrandStat(stat);
    }

    void updateRpmRedZoneRangeStat() {
        // 4.4.2 레드존 범위 (EV or FCEV || Not EV, FCEV && Not N Brand)  - Private_VariableTachoRedZone
        // 5.2.2.2 레드존 범위 - IMG_RpmRedZoneRange_stat

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            SFCTachometerConstantRedZoneNbrandStat stat = SFCTachometerConstantRedZoneNbrandStat::NONE;
            switch (mRpmGaugeRedStartPointReqStatus) {
                case RpmGaugeRedStartPointReqStatus::STEP_FROM4000:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4000;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM4250:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4250;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM4500:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4500;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM4750:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4750;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM5000:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5000;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM5250:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5250;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM5500:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5500;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM5750:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5750;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM6000:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6000;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM6250:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6250;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM6500:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6500;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM6750:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6750;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM7000:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM7000;
                    break;
                case RpmGaugeRedStartPointReqStatus::STEP_FROM7750:
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM7750;
                    break;
                default:
                    // STEP_FIXED, MESSAGE_TIMEOUT
                    if (almostEqual(mRpmMax, kDefaultRpmMax) == true) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6500;
                    } else if (almostEqual(mRpmMax, kDieselRpmMax) == true) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4500;
                    } else {
                        // no operation
                    }
                    break;
            }
            if (stat != SFCTachometerConstantRedZoneNbrandStat::NONE) {
                setSFCTachometerConstantRedZoneNbrandStat(stat);
            }
        }
    }

    void updateRpmRedZoneRangeStat_EngineOilTemp() {
        // 4.4.2 레드존 범위 (Not EV, FCEV & N Brand) - Private_VariableTachoRedZone
        // 5.2.2.2 레드존 범위 - IMG_RpmRedZoneRange_stat
        calculateEngineOilTempValue();

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivateEngineOilCelsiusCurrentTempvalue != mPrivateEngineOilCelsiusNewTempvalue) {
                mIncreasesTemp = mPrivateEngineOilCelsiusCurrentTempvalue < mPrivateEngineOilCelsiusNewTempvalue;
            }

            SFCTachometerConstantRedZoneNbrandStat stat = SFCTachometerConstantRedZoneNbrandStat::NONE;
            // EXNCP-33006, Input_EngineOilTempValue가 Timeout이 아닐 때 수행, 아닐경우 STEP_FIXED로 간주.
            if (mEngineOilTempValueTimeout == false) {
                if (mIncreasesTemp == true) {
                    if (mPrivateEngineOilCelsiusNewTempvalue >= 60) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6750;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 53) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6500;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 45) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6250;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 35) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6000;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 25) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5750;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 17) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5500;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue >= 10) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5250;
                    } else {
                        // STEP_FIXED
                    }
                } else {
                    if (mPrivateEngineOilCelsiusNewTempvalue < 0) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5000;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 7) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5250;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 15) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5500;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 25) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM5750;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 35) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6000;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 43) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6250;
                    } else if (mPrivateEngineOilCelsiusNewTempvalue < 50) {
                        stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6500;
                    } else {
                        // STEP_FIXED
                    }
                }
            }

            if (stat == SFCTachometerConstantRedZoneNbrandStat::NONE) {
                if (almostEqual(mRpmMax, kDefaultRpmMax) == true) {
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM6500;
                } else if (almostEqual(mRpmMax, kDieselRpmMax) == true) {
                    stat = SFCTachometerConstantRedZoneNbrandStat::STEP_FROM4500;
                } else {
                    // no operation
                }
            }

            if (stat != SFCTachometerConstantRedZoneNbrandStat::NONE) {
                setSFCTachometerConstantRedZoneNbrandStat(stat);
            }
        }
    }

    void calculateRPM() {
        // 4.1 RPM 입력 계산 - 수정할것 없음
        HDouble newRpm = 0.0;
        HDouble engineRpmDisplayReqValueDouble = static_cast<HDouble>(mEngineRpmDisplayReqValue);

        if (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (engineRpmDisplayReqValueDouble < kVgsEvRpmLimit) {
                newRpm = static_cast<HDouble>(mEngineRpmDisplayReqValue);
            }
        } else {
            if (mPTLogicControlType == PTLogicControlType::OFF) {
                if (mEmsVirtualRpmEnableStatus == EmsVirtualRpmEnableStatus::ON) {
                    if (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                        mConfigVehicleType == ConfigVehicleType::PHEV) {
                        newRpm = mEmsVirtualRpm * kFactorRpm13bitForEmsVirtualRpmValue;
                    }
                } else if (mTcuRpmEnableStatus == TcuRpmEnableStatus::ON) {
                    if (mConfigVehicleType == ConfigVehicleType::ICV) {
                        newRpm = static_cast<HDouble>(mTcuRpmFromTCU) * kFactorRpm16Bit;
                    } else if (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV) {
                        newRpm = static_cast<HDouble>(mTcuRpmFromHDCT) * kFactorRpm13bitForTcuRpmValueFromHDCT;
                    } else {
                        // no operation
                    }
                } else if (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                           mConfigVehicleType == ConfigVehicleType::PHEV) {
                    if (mEngineRpmErrorStatus == EngineRpmErrorStatus::NORMAL) {
                        newRpm = static_cast<HDouble>(mEngineRpm) * kFactorRpm16Bit;
                    }
                } else {
                    // no operation
                }
            } else if (mEngineRpmNewErrorStatus == EngineRpmNewErrorStatus::NORMAL) {
                newRpm = engineRpmDisplayReqValueDouble * kFactorRpm16Bit;
            } else {
                // no operation
            }
        }
        mPrevRpm = mRpm;
        mRpm = newRpm;
    }

    void updateRedZoneDisplay() {
        // 4.4.1 레드존 표시 여부 - Private_RedZoneDisplay
        if (mRpm >= mIncreaseRedZone) {
            mPrivate_RedZoneDisplay = SFCTachometerConstantRedZoneExceptNbrandStat::ON;
        }
        if (mRpm <= mDecreaseRedZone) {
            mPrivate_RedZoneDisplay = SFCTachometerConstantRedZoneExceptNbrandStat::OFF;
        }
    }

    void setRpmMax(HDouble RpmGaugeMaxValue) {
        mRpmMax = RpmGaugeMaxValue;
        if (almostEqual(mRpmMax, kDieselRpmMax) == true) {
            mIncreaseRedZone = kDieselIncreaseRedZone;
            mDecreaseRedZone = kDieselDecreaseRedZone;
        } else {
            mIncreaseRedZone = kDefaultIncreaseRedZone;
            mDecreaseRedZone = kDefaultDecreaseRedZone;
        }
    }

    void calculateMaxRPMStat() {
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            return;
        }

        if (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
            mConfigVehicleType == ConfigVehicleType::PHEV) {
            if (mPTLogicControlType == PTLogicControlType::OFF) {
                if (mConfigFuelType == ConfigFuelType::DSL) {
                    setRpmMax(kDieselRpmMax);
                } else {
                    setRpmMax(kDefaultRpmMax);
                }
            } else if (mPTLogicControlType == PTLogicControlType::ON) {
                if (mRpmGaugeMaxReqStatus == RpmGaugeMaxReqStatus::RPM6000) {
                    setRpmMax(kDieselRpmMax);
                } else {
                    setRpmMax(kDefaultRpmMax);
                }
            } else {
                // nothing
            }
        } else if (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mRpmGaugeMaxReqStatus == RpmGaugeMaxReqStatus::RPM6000) {
                setRpmMax(kDieselRpmMax);
            } else {
                setRpmMax(kDefaultRpmMax);
            }
        } else {
            // nothing
        }
        // 4.4.1 레드존 표시 여부
        if (almostEqual(mRpmMax, kDefaultRpmMax) == true) {
            setSFCTachometerConstantMaxRpmStat(SFCTachometerConstantMaxRpmStat::RPM8000);
        } else if (almostEqual(mRpmMax, kDieselRpmMax) == true) {
            setSFCTachometerConstantMaxRpmStat(SFCTachometerConstantMaxRpmStat::RPM6000);
        } else {
            // nothing
        }
    }

    void calculateRPMDampingStat() {
        SFCTachometerConstantRpmDampStat dampStat;
        HDouble delta = fabs(mRpm - mPrevRpm);

        if (mRpm == mPrevRpm) {
            return;
        }

        if (300 < mRpm && mRpm <= 1000) {
            if (delta <= 50) {
                dampStat = SFCTachometerConstantRpmDampStat::HOLD;
            } else if (delta <= 100) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV0;
            } else {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV1;
            }
        } else {
            if (delta <= 10) {
                dampStat = SFCTachometerConstantRpmDampStat::HOLD;
            } else if (delta <= 50) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV1;
            } else if (delta <= 75) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV2;
            } else if (delta <= 100) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV3;
            } else if (delta <= 125) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV4;
            } else if (delta <= 150) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV5;
            } else if (delta <= 175) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV6;
            } else if (delta <= 200) {
                dampStat = SFCTachometerConstantRpmDampStat::DAMPING_LV7;
            } else {
                dampStat = SFCTachometerConstantRpmDampStat::NO_DAMPING;
            }
        }
        setSFCTachometerConstantRpmDampStat(dampStat);
    }

    void calculateEngineOilTempValue() {
        // 4.2 엔진 오일 온도(도씨 환산) 계산
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            return;
        }

        mPrivateEngineOilCelsiusCurrentTempvalue = mPrivateEngineOilCelsiusNewTempvalue;

        if (mEngineOilTempValue == 255 || mEngineOilTempValueTimeout) {
            mPrivateEngineOilCelsiusNewTempvalue = 0;
        } else {
            mPrivateEngineOilCelsiusNewTempvalue =
                static_cast<HInt64>(round((static_cast<HDouble>(mEngineOilTempValue) * 0.75) - 40.0));
        }
    }

    using EngineRpmErrorStatus = ArgumentsEngineRpmChanged::Input_EngineRpmErrorStatus;
    using EmsVirtualRpmEnableStatus = ArgumentsEmsVirtualRpmChanged::Input_EmsVirtualRpmEnableStatus;
    using TcuRpmEnableStatus = ArgumentsTcuRpmChanged::Input_TcuRpmEnableStatus;
    using ConfigVehicleType = ArgumentsVehicleInfoChanged::Inter_ConfigVehicleType;
    using ConfigHighPerformance = ArgumentsVehicleInfoChanged::Inter_ConfigHighPerformance;
    using ConfigFuelType = ArgumentsVehicleInfoChanged::Inter_ConfigFuelType;
    using TelltaleDmsStat = ArgumentsDmsTypeChanged::DMSStat;
    using TelltaleDms = ArgumentsDmsTypeChanged::DMSStatOptional;
    using PTLogicControlType = ArgumentsPtLogicControlTypeChanged::Inter_PTLogicControlType;
    using EngineRpmNewErrorStatus = ArgumentsEngineRpmNewErrorStatusChanged::Input_EngineRpmNewErrorStatus;
    using RpmGaugeMaxReqStatus = ArgumentsRpmGaugeMaxReqStatusChanged::Input_RpmGaugeMaxReqStatus;
    using RpmGaugeRedStartPointReqStatus = ArgumentsRpmGaugeRedStartPointReqStatusChanged::Input_RpmGaugeRedStartPointReqStatus;

    // TODO(CWS) : factor Condition 16 bit, 13 bit 판단 방법 문의 함
    static constexpr HDouble kFactorRpm16Bit = 0.25;
    static constexpr HDouble kFactorRpm13bitForTcuRpmValueFromHDCT = 0.9766;
    static constexpr HDouble kFactorRpm13bitForEmsVirtualRpmValue = 0.9466;
    static constexpr HDouble kDefaultRpmMax = 8000.0;
    static constexpr HDouble kDefaultIncreaseRedZone = 5000.0;
    static constexpr HDouble kDefaultDecreaseRedZone = 4500.0;
    static constexpr HDouble kDieselRpmMax = 6000.0;
    static constexpr HDouble kDieselIncreaseRedZone = 4000.0;
    static constexpr HDouble kDieselDecreaseRedZone = 3500.0;
    static constexpr HDouble kVgsEvRpmLimit = 32767.0;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;
    TelltaleDms mTelltaleDmsType = TelltaleDms::OFF;
    TelltaleDmsStat mTelltaleDmsStat = TelltaleDmsStat::OFF;
    HUInt64 mEngineRpm = 0;
    EngineRpmErrorStatus mEngineRpmErrorStatus = EngineRpmErrorStatus::NORMAL;
    HDouble mEmsVirtualRpm = 0.0;
    EmsVirtualRpmEnableStatus mEmsVirtualRpmEnableStatus = EmsVirtualRpmEnableStatus::OFF;
    HUInt64 mTcuRpmFromTCU = 0;
    HUInt64 mTcuRpmFromHDCT = 0;
    TcuRpmEnableStatus mTcuRpmEnableStatus = TcuRpmEnableStatus::OFF;
    HDouble mPrevRpm = 0.0;
    HDouble mRpm = 0.0;
    HDouble mRpmMax = kDefaultRpmMax;
    HDouble mIncreaseRedZone = kDefaultIncreaseRedZone;
    HDouble mDecreaseRedZone = kDefaultDecreaseRedZone;
    HUInt64 mEngineOilCurrentTemp = 0;
    HUInt64 mEngineOilTempValue = 0;
    HBool mEngineOilTempValueTimeout = false;
    HBool mIncreasesTemp = false;
    SFCTachometerConstantRedZoneExceptNbrandStat mPrivate_RedZoneDisplay = SFCTachometerConstantRedZoneExceptNbrandStat::NONE;
    PTLogicControlType mPTLogicControlType = PTLogicControlType::OFF;
    EngineRpmNewErrorStatus mEngineRpmNewErrorStatus = EngineRpmNewErrorStatus::NORMAL;
    RpmGaugeMaxReqStatus mRpmGaugeMaxReqStatus = RpmGaugeMaxReqStatus::INVALID;
    RpmGaugeRedStartPointReqStatus mRpmGaugeRedStartPointReqStatus = RpmGaugeRedStartPointReqStatus::NO_DATA;
    HUInt64 mEngineRpmDisplayReqValue = 0;
    ConfigFuelType mConfigFuelType = ConfigFuelType::DSL;
    HInt64 mPrivateEngineOilCelsiusCurrentTempvalue = 0;
    HInt64 mPrivateEngineOilCelsiusNewTempvalue = 0;
};
}  // namespace ccos

#endif  // SFSS_Tachometer_H
