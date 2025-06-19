/**
 * @file EDG.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_EDG_H
#define SFSS_EDG_H

#define DLOG_ENABLED gEnableSFCLog

#include "EDGBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.13
// Reference : [CV470] EDG. Contains : Constant. Recommend: VALUE_CHANGED
class EDG : public EDGBase {
public:
    EDG() = default;
    ~EDG() override = default;
    EDG(const EDG& other) = delete;
    EDG(EDG&& other) noexcept = delete;
    EDG& operator=(const EDG& other) = delete;
    EDG& operator=(EDG&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigEDG = args.mInter_ConfigEDG;
        updateAll();
    }

    void onEdgTotalAvgScoreChanged(const ArgumentsEdgTotalAvgScoreChanged& args) {
        mEDGTotalAvgScore = args.mInput_EDGTotalAvgScore;
        updateConstantEDGTotalScoreValue();
    }

    void onEdgApsAvgScoreChanged(const ArgumentsEdgApsAvgScoreChanged& args) {
        mEDGApsAvgScore = args.mInput_EDGApsAvgScore;
        updateConstantEDGApsAvgScoreValue();
    }

    void onEdgApsCurScoreChanged(const ArgumentsEdgApsCurScoreChanged& args) {
        mEDGApsCurScore = args.mInput_EDGApsCurScore;
        updateConstantEDGApsCurScoreValue();
    }

    void onEdgApsActiveChanged(const ArgumentsEdgApsActiveChanged& args) {
        mEDGApsActive = args.mInput_EDGApsActive;
        updateConstantEDGApsScoreActiveStat();
    }

    void onEdgBrkAvgScoreChanged(const ArgumentsEdgBrkAvgScoreChanged& args) {
        mEDGBrkAvgScore = args.mInput_EDGBrkAvgScore;
        updateConstantEDGBrkAvgScoreValue();
    }

    void onEdgBrkCurScoreChanged(const ArgumentsEdgBrkCurScoreChanged& args) {
        mEDGBrkCurScore = args.mInput_EDGBrkCurScore;
        updateConstantEDGBrkCurScoreValue();
    }

    void onEdgBrkActiveChanged(const ArgumentsEdgBrkActiveChanged& args) {
        mEDGBrkActive = args.mInput_EDGBrkActive;
        updateConstantEDGBrkScoreActiveStat();
    }

    void onEdgIdleAvgScoreChanged(const ArgumentsEdgIdleAvgScoreChanged& args) {
        mEDGIdleAvgScore = args.mInput_EDGIdleAvgScore;
        updateConstantEDGIdleAvgScoreValue();
    }

    void onEdgIdleCurScoreChanged(const ArgumentsEdgIdleCurScoreChanged& args) {
        mEDGIdleCurScore = args.mInput_EDGIdleCurScore;
        updateConstantEDGIdleCurScoreValue();
    }

    void onEdgIdleActiveChanged(const ArgumentsEdgIdleActiveChanged& args) {
        mEDGIdleActive = args.mInput_EDGIdleActive;
        updateConstantEDGIdleScoreActiveStat();
    }

    void onEdgHillAvgScoreChanged(const ArgumentsEdgHillAvgScoreChanged& args) {
        mEDGHillAvgScore = args.mInput_EDGHillAvgScore;
        updateConstantEDGHillAvgScoreValue();
    }

    void onEdgHillCurScoreChanged(const ArgumentsEdgHillCurScoreChanged& args) {
        mEDGHillCurScore = args.mInput_EDGHillCurScore;
        updateConstantEDGHillCurScoreValue();
    }

    void onEdgHillActiveChanged(const ArgumentsEdgHillActiveChanged& args) {
        mEDGHillActive = args.mInput_EDGHillActive;
        updateConstantEDGHillScoreActiveStat();
    }

    void onEdgInfoStatusChanged(const ArgumentsEdgInfoStatusChanged& args) {
        mEDGInfoStatus = args.mInput_EDGInfoStatus;
        updateConstantEDGInfoStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    inline HBool isConfigOn() const {
        return mConfigEDG == ConfigEDG::ON;
    }

    void updateAll() {
        updateConstantEDGTotalScoreValue();
        updateConstantEDGApsAvgScoreValue();
        updateConstantEDGApsCurScoreValue();
        updateConstantEDGApsScoreActiveStat();
        updateConstantEDGBrkAvgScoreValue();
        updateConstantEDGBrkCurScoreValue();
        updateConstantEDGBrkScoreActiveStat();
        updateConstantEDGIdleAvgScoreValue();
        updateConstantEDGIdleCurScoreValue();
        updateConstantEDGIdleScoreActiveStat();
        updateConstantEDGHillAvgScoreValue();
        updateConstantEDGHillCurScoreValue();
        updateConstantEDGHillScoreActiveStat();
        updateConstantEDGInfoStat();
    }

    // 5.2.1 종합 점수
    void updateConstantEDGTotalScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGTotalAvgScore && mEDGTotalAvgScore <= 100) {
                value = mEDGTotalAvgScore;
            } else if (mEDGTotalAvgScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGTotalAvgScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGTotalScoreValue(value);
    }

    // 5.2.2 정속 점수
    // 5.2.2.1 정속 평균 점수_Text Type
    void updateConstantEDGApsAvgScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGApsAvgScore && mEDGApsAvgScore <= 100) {
                value = mEDGApsAvgScore;
            } else if (mEDGApsAvgScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGApsAvgScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGApsAvgScoreValue(value);
    }

    // 5.2.2.2 정속 순간 점수_Bar-graph Type
    void updateConstantEDGApsCurScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGApsCurScore && mEDGApsCurScore <= 100) {
                value = mEDGApsCurScore;
            } else if (mEDGApsCurScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGApsCurScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGApsCurScoreValue(value);
    }

    // 5.2.2.3 정속 점수 색상 표시
    void updateConstantEDGApsScoreActiveStat() {
        if (isConfigOn() == false) {
            return;
        }

        SFCEDGConstantEDGApsScoreActiveStat stat = SFCEDGConstantEDGApsScoreActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mEDGApsActive == EDGApsActive::INACTIVE) {
                stat = SFCEDGConstantEDGApsScoreActiveStat::INACTIVE;
            } else if (mEDGApsActive == EDGApsActive::ACTIVE) {
                stat = SFCEDGConstantEDGApsScoreActiveStat::ACTIVE;
            } else {
                // Default: OFF
            }
        }
        setSFCEDGConstantEDGApsScoreActiveStat(stat);
    }

    // 5.2.3 제동 점수
    // 5.2.3.1 제동 평균 점수_Text Type
    void updateConstantEDGBrkAvgScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGBrkAvgScore && mEDGBrkAvgScore <= 100) {
                value = mEDGBrkAvgScore;
            } else if (mEDGBrkAvgScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGBrkAvgScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGBrkAvgScoreValue(value);
    }

    // 5.2.3.2 제동 순간 점수_Bar-graph Type
    void updateConstantEDGBrkCurScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGBrkCurScore && mEDGBrkCurScore <= 100) {
                value = mEDGBrkCurScore;
            } else if (mEDGBrkCurScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGBrkCurScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGBrkCurScoreValue(value);
    }

    // 5.2.3.3 제동 점수 색상 표시
    void updateConstantEDGBrkScoreActiveStat() {
        if (isConfigOn() == false) {
            return;
        }

        SFCEDGConstantEDGBrkScoreActiveStat stat = SFCEDGConstantEDGBrkScoreActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mEDGBrkActive == EDGBrkActive::INACTIVE) {
                stat = SFCEDGConstantEDGBrkScoreActiveStat::INACTIVE;
            } else if (mEDGBrkActive == EDGBrkActive::ACTIVE) {
                stat = SFCEDGConstantEDGBrkScoreActiveStat::ACTIVE;
            } else {
                // Default: OFF
            }
        }
        setSFCEDGConstantEDGBrkScoreActiveStat(stat);
    }

    // 5.2.4 공회전 점수
    // 5.2.4.1 공회전 평균 점수_Text Type
    void updateConstantEDGIdleAvgScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGIdleAvgScore && mEDGIdleAvgScore <= 100) {
                value = mEDGIdleAvgScore;
            } else if (mEDGIdleAvgScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGIdleAvgScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGIdleAvgScoreValue(value);
    }

    // 5.2.4.2 공회전 순간 점수_Bar-graph Type
    void updateConstantEDGIdleCurScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGIdleCurScore && mEDGIdleCurScore <= 100) {
                value = mEDGIdleCurScore;
            } else if (mEDGIdleCurScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGIdleCurScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGIdleCurScoreValue(value);
    }

    // 5.2.4.3 공회전 점수 색상 표시
    void updateConstantEDGIdleScoreActiveStat() {
        if (isConfigOn() == false) {
            return;
        }

        SFCEDGConstantEDGIdleScoreActiveStat stat = SFCEDGConstantEDGIdleScoreActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mEDGIdleActive == EDGIdleActive::INACTIVE) {
                stat = SFCEDGConstantEDGIdleScoreActiveStat::INACTIVE;
            } else if (mEDGIdleActive == EDGIdleActive::ACTIVE) {
                stat = SFCEDGConstantEDGIdleScoreActiveStat::ACTIVE;
            } else {
                // Default: OFF
            }
        }
        setSFCEDGConstantEDGIdleScoreActiveStat(stat);
    }

    // 5.2.5 언덕 점수
    // 5.2.5.1 언덕 평균 점수_Text Type
    void updateConstantEDGHillAvgScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGHillAvgScore && mEDGHillAvgScore <= 100) {
                value = mEDGHillAvgScore;
            } else if (mEDGHillAvgScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGHillAvgScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGHillAvgScoreValue(value);
    }

    // 5.2.5.2 언덕 순간 점수_Bar-graph Type
    void updateConstantEDGHillCurScoreValue() {
        if (isConfigOn() == false) {
            return;
        }

        HUInt64 value = kEDGNoDisplayValue;

        if (mIsIgnOn == true) {
            if (0 <= mEDGHillCurScore && mEDGHillCurScore <= 100) {
                value = mEDGHillCurScore;
            } else if (mEDGHillCurScore == 101) {
                value = kEDGDashDisplayValue;
            } else {
                // mEDGHillCurScore > 101 (Default: 0xFF)
            }
        } else {
            value = 0;
        }
        setSFCEDGConstantEDGHillCurScoreValue(value);
    }

    // 5.2.5.3 언덕 점수 색상 표시
    void updateConstantEDGHillScoreActiveStat() {
        if (isConfigOn() == false) {
            return;
        }

        SFCEDGConstantEDGHillScoreActiveStat stat = SFCEDGConstantEDGHillScoreActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mEDGHillActive == EDGHillActive::INACTIVE) {
                stat = SFCEDGConstantEDGHillScoreActiveStat::INACTIVE;
            } else if (mEDGHillActive == EDGHillActive::ACTIVE) {
                stat = SFCEDGConstantEDGHillScoreActiveStat::ACTIVE;
            } else {
                // Default: OFF
            }
        }
        setSFCEDGConstantEDGHillScoreActiveStat(stat);
    }

    // 5.2.6 연비 운전 가이드
    void updateConstantEDGInfoStat() {
        if (isConfigOn() == false) {
            return;
        }

        SFCEDGConstantEDGInfoStat stat = SFCEDGConstantEDGInfoStat::OFF;

        if (mIsIgnOn == true) {
            switch (mEDGInfoStatus) {
                case EDGInfoStatus::DONT_ACCEL:
                    stat = SFCEDGConstantEDGInfoStat::DONT_ACCEL;
                    break;
                case EDGInfoStatus::DOWN_RPM:
                    stat = SFCEDGConstantEDGInfoStat::DOWN_RPM;
                    break;
                case EDGInfoStatus::DOWN_SPD:
                    stat = SFCEDGConstantEDGInfoStat::DOWN_SPD;
                    break;
                case EDGInfoStatus::SHIFT_HIGH_GEAR:
                    stat = SFCEDGConstantEDGInfoStat::SHIFT_HIGH_GEAR;
                    break;
                case EDGInfoStatus::USE_CC:
                    stat = SFCEDGConstantEDGInfoStat::USE_CC;
                    break;
                case EDGInfoStatus::USE_COASTING:
                    stat = SFCEDGConstantEDGInfoStat::USE_COASTING;
                    break;
                case EDGInfoStatus::USE_EROLL:
                    stat = SFCEDGConstantEDGInfoStat::USE_EROLL;
                    break;
                case EDGInfoStatus::DONT_SUDDEN_BRK:
                    stat = SFCEDGConstantEDGInfoStat::DONT_SUDDEN_BRK;
                    break;
                case EDGInfoStatus::KEY_OFF:
                    stat = SFCEDGConstantEDGInfoStat::KEY_OFF;
                    break;
                default:
                    // Default: OFF
                    break;
            }
        }
        setSFCEDGConstantEDGInfoStat(stat);
    }

    using ConfigEDG = ArgumentsConfigChanged::Inter_ConfigEDG;
    using EDGApsActive = ArgumentsEdgApsActiveChanged::Input_EDGApsActive;
    using EDGBrkActive = ArgumentsEdgBrkActiveChanged::Input_EDGBrkActive;
    using EDGIdleActive = ArgumentsEdgIdleActiveChanged::Input_EDGIdleActive;
    using EDGHillActive = ArgumentsEdgHillActiveChanged::Input_EDGHillActive;
    using EDGInfoStatus = ArgumentsEdgInfoStatusChanged::Input_EDGInfoStatus;

    static constexpr HUInt64 kEDGNoDisplayValue = 0xFF;
    static constexpr HUInt64 kEDGDashDisplayValue = 0xFE;

    HBool mIsIgnOn = false;
    ConfigEDG mConfigEDG = ConfigEDG::OFF;

    HUInt64 mEDGTotalAvgScore = 0;
    HUInt64 mEDGApsAvgScore = 0;
    HUInt64 mEDGApsCurScore = 0;
    EDGApsActive mEDGApsActive = EDGApsActive::INACTIVE;
    HUInt64 mEDGBrkAvgScore = 0;
    HUInt64 mEDGBrkCurScore = 0;
    EDGBrkActive mEDGBrkActive = EDGBrkActive::INACTIVE;
    HUInt64 mEDGIdleAvgScore = 0;
    HUInt64 mEDGIdleCurScore = 0;
    EDGIdleActive mEDGIdleActive = EDGIdleActive::INACTIVE;
    HUInt64 mEDGHillAvgScore = 0;
    HUInt64 mEDGHillCurScore = 0;
    EDGHillActive mEDGHillActive = EDGHillActive::INACTIVE;
    EDGInfoStatus mEDGInfoStatus = EDGInfoStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_EDG_H
