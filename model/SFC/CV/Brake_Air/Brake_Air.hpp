/**
 * @file Brake_Air.hpp
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
#ifndef SFSS_Brake_Air_H
#define SFSS_Brake_Air_H

#define DLOG_ENABLED gEnableSFCLog

#include "Brake_AirBase.hpp"

namespace ccos {

// SFC Version : 13.0.0
// Spec Version : v0.31
// Reference : [CV160] Brake Air. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class Brake_Air : public Brake_AirBase {
public:
    Brake_Air() = default;
    ~Brake_Air() override = default;
    Brake_Air(const Brake_Air& other) = delete;
    Brake_Air(Brake_Air&& other) noexcept = delete;
    Brake_Air& operator=(const Brake_Air& other) = delete;
    Brake_Air& operator=(Brake_Air&& other) noexcept = delete;

    void onInitialize() override {
        setSFCBrake_AirEventLowAirPressureWarningID("E71601");

        setSFCBrake_AirEventOverAirPressureWarningID("E71602");
        setSFCBrake_AirEventOverAirPressureWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCBrake_AirEventOverAirPressureWarningLinkedSoundType(
            SFCBrake_AirEventOverAirPressureWarningLinkedSoundType::REPEAT_COUNT);
        setSFCBrake_AirEventOverAirPressureWarningLinkedSoundRepeatCount(1);

        setSFCBrake_AirSoundLowAirPressureWarningID("SND_LowBrakeAirWarn");
        setSFCBrake_AirSoundLowAirPressureWarningType(SFCBrake_AirSoundLowAirPressureWarningType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updatePrivateLowAirPressureWarningStatus();
        updatePrivateOverAirPressureWarningStatus();
        updateConstantFrontAxleBrakeAirValue();
        updateConstantRearAxleBrakeAirValue();
        updateConstantFrontAxleBrakeAirColor();
        updateConstantRearAxleBrakeAirColor();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updatePrivateLowAirPressureWarningStatus();
        updatePrivateOverAirPressureWarningStatus();
        updateConstantFrontAxleBrakeAirValue();
        updateConstantRearAxleBrakeAirValue();
        updateConstantFrontAxleBrakeAirColor();
        updateConstantRearAxleBrakeAirColor();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;

        updatePrivateAirPressureFactor();
        updateConstantBrakeAirUnit();
        updateConstantBrakeAirMax();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;

        updatePrivateAirPressureFactor();
        updateConstantFrontAxleBrakeAirValue();
        updateConstantRearAxleBrakeAirValue();
        updatePrivateLowAirPressureWarningStatus();
        updatePrivateOverAirPressureWarningStatus();
        updateConstantFrontAxleBrakeAirColor();
        updateConstantRearAxleBrakeAirColor();
        updateConstantBrakeAirUnit();
        updateConstantBrakeAirMax();
    }

    void onFrontAxleBrakeAirValueChanged(const ArgumentsFrontAxleBrakeAirValueChanged& args) {
        mFrontAxleBrakeAirValue = args.mInput_FrontAxleBrakeAirValue;

        updatePrivateLowAirPressureWarningStatus();
        updatePrivateOverAirPressureWarningStatus();
        updateConstantFrontAxleBrakeAirValue();
        updateConstantFrontAxleBrakeAirColor();
    }

    void onRearAxleBrakeAirValueChanged(const ArgumentsRearAxleBrakeAirValueChanged& args) {
        mRearAxleBrakeAirValue = args.mInput_RearAxleBrakeAirValue;

        updatePrivateLowAirPressureWarningStatus();
        updatePrivateOverAirPressureWarningStatus();
        updateConstantRearAxleBrakeAirValue();
        updateConstantRearAxleBrakeAirColor();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1 지역별 Factor 정의
    void updatePrivateAirPressureFactor() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        mPrivateAirPressureFactor = kPrivateAirPressureFactorNotUSA;

        if (mConfigArea == ConfigArea::USA) {
            mPrivateAirPressureFactor = kPrivateAirPressureFactorUSA;
        }
    }

    // 4.2 Brake Air 부족 경고 히스테리시스
    void updatePrivateLowAirPressureWarningStatus() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        Private_AirPressureStatus status = Private_AirPressureStatus::OFF;
        if (mIsIgnOn == true) {
            if ((mFrontAxleBrakeAirValue >= 0x0 && mFrontAxleBrakeAirValue <= 0x4B) ||
                (mRearAxleBrakeAirValue >= 0x0 && mRearAxleBrakeAirValue <= 0x4B)) {
                status = Private_AirPressureStatus::ON;
            } else if (((mFrontAxleBrakeAirValue >= 0x4C && mFrontAxleBrakeAirValue <= 0x50) &&
                        (mRearAxleBrakeAirValue >= 0x4C && mRearAxleBrakeAirValue <= 0xFF)) ||
                       ((mFrontAxleBrakeAirValue >= 0x4C && mFrontAxleBrakeAirValue <= 0xFF) &&
                        (mRearAxleBrakeAirValue >= 0x4C && mRearAxleBrakeAirValue <= 0x50))) {
                status = Private_AirPressureStatus::KEEP_CUR_STAT;
            } else {
                // no operation
            }
        }
        mPrivateLowAirPressureWarningStatus = status;
        updateEventLowAirPressureWarningStat();
        updateSoundLowAirPressureWarningStat();
    }

    void updateEventLowAirPressureWarningStat() {
        HUInt64 tmpPrevLowEventStat;
        GETCACHEDVALUE(SFC.Brake_Air.Event.LowAirPressureWarning.Stat, tmpPrevLowEventStat);
        mPrevLowEventStat = static_cast<SFCBrake_AirEventLowAirPressureWarningStat>(tmpPrevLowEventStat);

        SFCBrake_AirEventLowAirPressureWarningStat eventStat = SFCBrake_AirEventLowAirPressureWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mPrivateLowAirPressureWarningStatus == Private_AirPressureStatus::ON) {
                eventStat = SFCBrake_AirEventLowAirPressureWarningStat::ON;
            } else if (mPrivateLowAirPressureWarningStatus == Private_AirPressureStatus::KEEP_CUR_STAT) {
                if (mPrevLowEventStat == SFCBrake_AirEventLowAirPressureWarningStat::ON) {
                    eventStat = SFCBrake_AirEventLowAirPressureWarningStat::ON;
                }
            }
        }

        setSFCBrake_AirEventLowAirPressureWarningStat(eventStat);
    }

    void updateSoundLowAirPressureWarningStat() {
        HUInt64 tmpPrevLowSoundStat;
        GETCACHEDVALUE(SFC.Brake_Air.Sound.LowAirPressureWarning.Stat, tmpPrevLowSoundStat);
        mPrevLowSoundStat = static_cast<SFCBrake_AirSoundLowAirPressureWarningStat>(tmpPrevLowSoundStat);

        SFCBrake_AirSoundLowAirPressureWarningStat soundStat = SFCBrake_AirSoundLowAirPressureWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mPrivateLowAirPressureWarningStatus == Private_AirPressureStatus::ON) {
                soundStat = SFCBrake_AirSoundLowAirPressureWarningStat::ON;
            } else if (mPrivateLowAirPressureWarningStatus == Private_AirPressureStatus::KEEP_CUR_STAT) {
                if (mPrevLowSoundStat == SFCBrake_AirSoundLowAirPressureWarningStat::ON) {
                    soundStat = SFCBrake_AirSoundLowAirPressureWarningStat::ON;
                }
            }
        }

        setSFCBrake_AirSoundLowAirPressureWarningStat(soundStat);
    }

    // 4.3 Brake Air 과다 경고 히스테리시스
    void updatePrivateOverAirPressureWarningStatus() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        Private_AirPressureStatus status = Private_AirPressureStatus::OFF;
        if (mIsIgnOn == true) {
            if (mConfigBrakeAirType == ConfigBrakeAirType::TYPE_2) {
                if ((mFrontAxleBrakeAirValue >= 0xA3 && mFrontAxleBrakeAirValue <= 0xFF) ||
                    (mRearAxleBrakeAirValue >= 0xA3 && mRearAxleBrakeAirValue <= 0xFF)) {
                    status = Private_AirPressureStatus::ON;
                } else if (((mFrontAxleBrakeAirValue >= 0x9B && mFrontAxleBrakeAirValue <= 0xA2) &&
                            (mRearAxleBrakeAirValue >= 0x0 && mRearAxleBrakeAirValue <= 0xA2)) ||
                           ((mFrontAxleBrakeAirValue >= 0x0 && mFrontAxleBrakeAirValue <= 0xA2) &&
                            (mRearAxleBrakeAirValue >= 0x9B && mRearAxleBrakeAirValue <= 0xA2))) {
                    status = Private_AirPressureStatus::KEEP_CUR_STAT;
                } else {
                    // no operation
                }
            } else if (mConfigBrakeAirType == ConfigBrakeAirType::TYPE_1) {
                if ((mFrontAxleBrakeAirValue >= 0x7D && mFrontAxleBrakeAirValue <= 0xFF) ||
                    (mRearAxleBrakeAirValue >= 0x7D && mRearAxleBrakeAirValue <= 0xFF)) {
                    status = Private_AirPressureStatus::ON;
                } else if (((mFrontAxleBrakeAirValue >= 0x77 && mFrontAxleBrakeAirValue <= 0x7C) &&
                            (mRearAxleBrakeAirValue >= 0x0 && mRearAxleBrakeAirValue <= 0x7C)) ||
                           ((mFrontAxleBrakeAirValue >= 0x0 && mFrontAxleBrakeAirValue <= 0x7C) &&
                            (mRearAxleBrakeAirValue >= 0x77 && mRearAxleBrakeAirValue <= 0x7C))) {
                    status = Private_AirPressureStatus::KEEP_CUR_STAT;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
        mPrivateOverAirPressureWarningStatus = status;
        updateEventOverAirPressureWarningStat();
    }

    void updateEventOverAirPressureWarningStat() {
        HUInt64 tmpPrevOverEventStat;
        GETCACHEDVALUE(SFC.Brake_Air.Event.OverAirPressureWarning.Stat, tmpPrevOverEventStat);
        mPrevOverEventStat = static_cast<SFCBrake_AirEventOverAirPressureWarningStat>(tmpPrevOverEventStat);

        SFCBrake_AirEventOverAirPressureWarningStat stat = SFCBrake_AirEventOverAirPressureWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mPrivateOverAirPressureWarningStatus == Private_AirPressureStatus::ON) {
                stat = SFCBrake_AirEventOverAirPressureWarningStat::ON;
            } else if (mPrivateOverAirPressureWarningStatus == Private_AirPressureStatus::KEEP_CUR_STAT) {
                if (mPrevOverEventStat == SFCBrake_AirEventOverAirPressureWarningStat::ON) {
                    stat = SFCBrake_AirEventOverAirPressureWarningStat::ON;
                }
            }
        }

        setSFCBrake_AirEventOverAirPressureWarningStat(stat);
    }

    // 5.2.1 Brake Air Gauge_Front
    void updateConstantFrontAxleBrakeAirValue() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        HUInt64 value = 0xFFF;

        if (mIsIgnOn == true) {
            if (mFrontAxleBrakeAirValue >= 0x00 && mFrontAxleBrakeAirValue <= 0xA2) {
                value = floor(static_cast<HDouble>(mFrontAxleBrakeAirValue) * mPrivateAirPressureFactor);
            } else if (mFrontAxleBrakeAirValue >= 0xA3 && mFrontAxleBrakeAirValue <= 0xFE) {
                value = 0xFFE;
            } else {
                // no operation
            }
        }

        setSFCBrake_AirConstantFrontAxleBrakeAirValue(value);
    }

    // 5.2.2 Brake Air Gauge_Rear
    void updateConstantRearAxleBrakeAirValue() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        HUInt64 value = 0xFFF;

        if (mIsIgnOn == true) {
            if (mRearAxleBrakeAirValue >= 0x00 && mRearAxleBrakeAirValue <= 0xA2) {
                value = floor(static_cast<HDouble>(mRearAxleBrakeAirValue) * mPrivateAirPressureFactor);
            } else if (mRearAxleBrakeAirValue >= 0xA3 && mRearAxleBrakeAirValue <= 0xFE) {
                value = 0xFFE;
            } else {
                // no operation
            }
        }

        setSFCBrake_AirConstantRearAxleBrakeAirValue(value);
    }

    // 5.2.3 Brake Air Gauge Color_Front
    void updateConstantFrontAxleBrakeAirColor() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        SFCBrake_AirConstantFrontAxleBrakeAirColorStat stat = SFCBrake_AirConstantFrontAxleBrakeAirColorStat::NORMAL;

        if (mIsIgnOn == true) {
            if (0x00 <= mFrontAxleBrakeAirValue && mFrontAxleBrakeAirValue <= 0x4B) {
                stat = SFCBrake_AirConstantFrontAxleBrakeAirColorStat::WARNING;
            }
        }

        setSFCBrake_AirConstantFrontAxleBrakeAirColorStat(stat);
    }

    // 5.2.4 Brake Air Gauge Color_Rear
    void updateConstantRearAxleBrakeAirColor() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        SFCBrake_AirConstantRearAxleBrakeAirColorStat stat = SFCBrake_AirConstantRearAxleBrakeAirColorStat::NORMAL;

        if (mIsIgnOn == true) {
            if (0x00 <= mRearAxleBrakeAirValue && mRearAxleBrakeAirValue <= 0x4B) {
                stat = SFCBrake_AirConstantRearAxleBrakeAirColorStat::WARNING;
            }
        }

        setSFCBrake_AirConstantRearAxleBrakeAirColorStat(stat);
    }

    // 5.2.5 Brake Air Gauge Unit
    void updateConstantBrakeAirUnit() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        SFCBrake_AirConstantBrakeAirUnitStat stat = SFCBrake_AirConstantBrakeAirUnitStat::X100KPA;

        if (mConfigArea == ConfigArea::USA) {
            stat = SFCBrake_AirConstantBrakeAirUnitStat::PSI;
        }

        setSFCBrake_AirConstantBrakeAirUnitStat(stat);
    }

    // 5.2.6 Brake Air Gauge Max
    void updateConstantBrakeAirMax() {
        if (mConfigBrakeAirType != ConfigBrakeAirType::TYPE_1 && mConfigBrakeAirType != ConfigBrakeAirType::TYPE_2) {
            return;
        }

        HUInt64 value = kNotUSABrakeAirMaxValue;

        if (mConfigArea == ConfigArea::USA) {
            value = kUSABrakeAirMaxValue;
        }

        setSFCBrake_AirConstantBrakeAirMaxValue(value);
    }

    enum class Private_AirPressureStatus : HUInt64 {
        NONE,
        OFF,
        ON,
        KEEP_CUR_STAT,
        MAX
    };

    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;

    HBool mIsIgnOn = false;

    HUInt64 mFrontAxleBrakeAirValue = 0;
    HUInt64 mRearAxleBrakeAirValue = 0;
    HDouble mPrivateAirPressureFactor = 0.0;
    Private_AirPressureStatus mPrivateLowAirPressureWarningStatus = Private_AirPressureStatus::OFF;
    Private_AirPressureStatus mPrivateOverAirPressureWarningStatus = Private_AirPressureStatus::OFF;
    SFCBrake_AirEventLowAirPressureWarningStat mPrevLowEventStat = SFCBrake_AirEventLowAirPressureWarningStat::OFF;
    SFCBrake_AirSoundLowAirPressureWarningStat mPrevLowSoundStat = SFCBrake_AirSoundLowAirPressureWarningStat::OFF;
    SFCBrake_AirEventOverAirPressureWarningStat mPrevOverEventStat = SFCBrake_AirEventOverAirPressureWarningStat::OFF;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::TYPE_2;
    ConfigArea mConfigArea = ConfigArea::DOM;

    static constexpr HDouble kPrivateAirPressureFactorUSA = 1.16;
    static constexpr HDouble kPrivateAirPressureFactorNotUSA = 0.08;
    static constexpr HUInt64 kNotUSABrakeAirMaxValue = 13;
    static constexpr HUInt64 kUSABrakeAirMaxValue = 190;
};

}  // namespace ccos

#endif  // SFSS_Brake_Air_H
