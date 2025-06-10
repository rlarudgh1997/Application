/**
 * @file Predictive_Shift.hpp
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
#ifndef SFSS_Predictive_Shift_H
#define SFSS_Predictive_Shift_H

#define DLOG_ENABLED gEnableSFCLog

#include "Predictive_ShiftBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [PT470] Predictive_Shift. Contains : Constant. Recommend: VALUE_CHANGED
class Predictive_Shift : public Predictive_ShiftBase {
public:
    Predictive_Shift() = default;
    ~Predictive_Shift() override = default;
    Predictive_Shift(const Predictive_Shift& other) = delete;
    Predictive_Shift(Predictive_Shift&& other) noexcept = delete;
    Predictive_Shift& operator=(const Predictive_Shift& other) = delete;
    Predictive_Shift& operator=(Predictive_Shift&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onConfigPredictShiftChanged(const ArgumentsConfigPredictShiftChanged& args) {
        mConfigPredictShift = args.mInter_ConfigPredictShift;
        updatePreShiftPageStatus();
        updatePreShiftTextDisplayStatus();
        updateAllPreShiftRoadSign();
    }

    void onPreShiftSystemStatusChanged(const ArgumentsPreShiftSystemStatusChanged& args) {
        mPreShiftSystemStatus = args.mInput_PreShiftSystemStatus;
        updatePreShiftPageStatus();
        updatePreShiftTextDisplayStatus();
        updateAllPreShiftRoadSign();
    }

    void onPreShiftVariableDisplayStatusChanged(const ArgumentsPreShiftVariableDisplayStatusChanged& args) {
        mPreShiftVariableDisplayStatus = args.mInput_PreShiftVariableDisplayStatus;
        updatePreShiftTextDisplayStatus();
        updateAllPreShiftRoadSign();
    }

    void onPreShiftCurveDisplayStatusChanged(const ArgumentsPreShiftCurveDisplayStatusChanged& args) {
        mPreShiftCurveDisplayStatus = args.mInput_PreShiftCurveDisplayStatus;
        updatePreShiftRoadSign1();
    }

    void onPreShiftMergeDisplayStatusChanged(const ArgumentsPreShiftMergeDisplayStatusChanged& args) {
        mPreShiftMergeDisplayStatus = args.mInput_PreShiftMergeDisplayStatus;
        updatePreShiftRoadSign2();
    }

    void onPreShiftSpeedLimitDisplayStatusChanged(const ArgumentsPreShiftSpeedLimitDisplayStatusChanged& args) {
        mPreShiftSpeedLimitDisplayStatus = args.mInput_PreShiftSpeedLimitDisplayStatus;
        updatePreShiftRoadSign3();
    }

    void onPreShiftStopSignDisplayStatusChanged(const ArgumentsPreShiftStopSignDisplayStatusChanged& args) {
        mPreShiftStopSignDisplayStatus = args.mInput_PreShiftStopSignDisplayStatus;
        updatePreShiftRoadSign3();
    }

    void onPreShiftRoundaboutDisplayStatusChanged(const ArgumentsPreShiftRoundaboutDisplayStatusChanged& args) {
        mPreShiftRoundaboutDisplayStatus = args.mInput_PreShiftRoundaboutDisplayStatus;
        updatePreShiftRoadSign3();
    }

    void onPreShiftFrontCarDisplayStatusChanged(const ArgumentsPreShiftFrontCarDisplayStatusChanged& args) {
        mPreShiftFrontCarDisplayStatus = args.mInput_PreShiftFrontCarDisplayStatus;
        updatePreShiftRoadSign4();
    }

    void onPreShiftDownhillDisplayStatusChanged(const ArgumentsPreShiftDownhillDisplayStatusChanged& args) {
        mPreShiftDownhillDisplayStatus = args.mInput_PreShiftDownhillDisplayStatus;
        updatePreShiftRoadSign5();
    }

    void onPreShiftOtherDisplayStatusChanged(const ArgumentsPreShiftOtherDisplayStatusChanged& args) {
        mPreShiftOtherDisplayStatus = args.mInput_PreShiftOtherDisplayStatus;
        updatePreShiftRoadSign6();
    }

    void onPreShiftTextDisplayStatusChanged(const ArgumentsPreShiftTextDisplayStatusChanged& args) {
        mPreShiftTextDisplayStatus = args.mInput_PreShiftTextDisplayStatus;
        updatePreShiftTextDisplayStatus();
    }

private:
    void updatePreShiftPageStatus() {
        SFCPredictive_ShiftConstantPreShiftPageStat status = SFCPredictive_ShiftConstantPreShiftPageStat::OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON) {
            if (mPreShiftSystemStatus == PreShiftSystemStatus::NOT_READY_MODE) {
                status = SFCPredictive_ShiftConstantPreShiftPageStat::NOT_READY_MODE;
            } else if (mPreShiftSystemStatus == PreShiftSystemStatus::NOT_READY_ETC) {
                status = SFCPredictive_ShiftConstantPreShiftPageStat::NOT_READY_ETC;
            } else if (mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
                status = SFCPredictive_ShiftConstantPreShiftPageStat::READY;
            } else {
                // nothing
            }
        }
        setSFCPredictive_ShiftConstantPreShiftPageStat(status);
    }
    void updatePreShiftRoadSign1() {
        SFCPredictive_ShiftConstantPreShiftRoadSign1Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign1Stat::CURVE_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftCurveDisplayStatus == PreShiftCurveDisplayStatus::ON) {
                status = SFCPredictive_ShiftConstantPreShiftRoadSign1Stat::CURVE_ON;
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign1Stat(status);
    }

    void updatePreShiftRoadSign2() {
        SFCPredictive_ShiftConstantPreShiftRoadSign2Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign2Stat::MERGE_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftMergeDisplayStatus == PreShiftMergeDisplayStatus::ON) {
                status = SFCPredictive_ShiftConstantPreShiftRoadSign2Stat::MERGE_ON;
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign2Stat(status);
    }

    void updatePreShiftRoadSign3() {
        SFCPredictive_ShiftConstantPreShiftRoadSign3Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::DISPLAY_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON) {
            if (mPreShiftVariableDisplayStatus == PreShiftVariableDisplayStatus::SPEEDLIMIT) {
                if (mPreShiftSpeedLimitDisplayStatus == PreShiftSpeedLimitDisplayStatus::ON &&
                    mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::SPEEDLIMIT_ON;
                } else {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::SPEEDLIMIT_OFF;
                }
            } else if (mPreShiftVariableDisplayStatus == PreShiftVariableDisplayStatus::STOPSIGN) {
                if (mPreShiftStopSignDisplayStatus == PreShiftStopSignDisplayStatus::ON &&
                    mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::STOPSIGN_ON;
                } else {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::STOPSIGN_OFF;
                }
            } else if (mPreShiftVariableDisplayStatus == PreShiftVariableDisplayStatus::ROUNDABOUT) {
                if (mPreShiftRoundaboutDisplayStatus == PreShiftRoundaboutDisplayStatus::ON &&
                    mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::ROUNDABOUT_ON;
                } else {
                    status = SFCPredictive_ShiftConstantPreShiftRoadSign3Stat::ROUNDABOUT_OFF;
                }
            } else {
                // nothing
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign3Stat(status);
    }

    void updatePreShiftRoadSign4() {
        SFCPredictive_ShiftConstantPreShiftRoadSign4Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign4Stat::FRONTCAR_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftFrontCarDisplayStatus == PreShiftFrontCarDisplayStatus::ON) {
                status = SFCPredictive_ShiftConstantPreShiftRoadSign4Stat::FRONTCAR_ON;
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign4Stat(status);
    }

    void updatePreShiftRoadSign5() {
        SFCPredictive_ShiftConstantPreShiftRoadSign5Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign5Stat::DOWNHILL_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftDownhillDisplayStatus == PreShiftDownhillDisplayStatus::ON) {
                status = SFCPredictive_ShiftConstantPreShiftRoadSign5Stat::DOWNHILL_ON;
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign5Stat(status);
    }

    void updatePreShiftRoadSign6() {
        SFCPredictive_ShiftConstantPreShiftRoadSign6Stat status = SFCPredictive_ShiftConstantPreShiftRoadSign6Stat::ETC_OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftOtherDisplayStatus == PreShiftOtherDisplayStatus::ON) {
                status = SFCPredictive_ShiftConstantPreShiftRoadSign6Stat::ETC_ON;
            }
        }
        setSFCPredictive_ShiftConstantPreShiftRoadSign6Stat(status);
    }

    void updateAllPreShiftRoadSign() {
        updatePreShiftRoadSign1();
        updatePreShiftRoadSign2();
        updatePreShiftRoadSign3();
        updatePreShiftRoadSign4();
        updatePreShiftRoadSign5();
        updatePreShiftRoadSign6();
    }

    void updatePreShiftTextDisplayStatus() {
        SFCPredictive_ShiftConstantPreShiftTextSignStat status = SFCPredictive_ShiftConstantPreShiftTextSignStat::OFF;
        if (mConfigPredictShift == ConfigPredictShift::ON && mPreShiftSystemStatus == PreShiftSystemStatus::READY) {
            if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::CURVE) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::CURVE;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::MERGE) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::MERGE;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::SPEEDLIMIT) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::SPEEDLIMIT;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::FRONTCAR) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::FRONTCAR;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::DOWNHILL) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::DOWNHILL;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::STOPSIGN) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::STOPSIGN;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::ROUNDABOUT) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::ROUNDABOUT;
            } else if (mPreShiftTextDisplayStatus == PreShiftTextDisplayStatus::ETC) {
                status = SFCPredictive_ShiftConstantPreShiftTextSignStat::ETC;
            } else {
                // OFF
            }
        }
        setSFCPredictive_ShiftConstantPreShiftTextSignStat(status);
    }

    using ConfigPredictShift = ArgumentsConfigPredictShiftChanged::Inter_ConfigPredictShift;
    using PreShiftSystemStatus = ArgumentsPreShiftSystemStatusChanged::Input_PreShiftSystemStatus;
    using PreShiftVariableDisplayStatus = ArgumentsPreShiftVariableDisplayStatusChanged::Input_PreShiftVariableDisplayStatus;
    using PreShiftCurveDisplayStatus = ArgumentsPreShiftCurveDisplayStatusChanged::Input_PreShiftCurveDisplayStatus;
    using PreShiftMergeDisplayStatus = ArgumentsPreShiftMergeDisplayStatusChanged::Input_PreShiftMergeDisplayStatus;
    using PreShiftSpeedLimitDisplayStatus =
        ArgumentsPreShiftSpeedLimitDisplayStatusChanged::Input_PreShiftSpeedLimitDisplayStatus;
    using PreShiftStopSignDisplayStatus = ArgumentsPreShiftStopSignDisplayStatusChanged::Input_PreShiftStopSignDisplayStatus;
    using PreShiftRoundaboutDisplayStatus =
        ArgumentsPreShiftRoundaboutDisplayStatusChanged::Input_PreShiftRoundaboutDisplayStatus;
    using PreShiftFrontCarDisplayStatus = ArgumentsPreShiftFrontCarDisplayStatusChanged::Input_PreShiftFrontCarDisplayStatus;
    using PreShiftDownhillDisplayStatus = ArgumentsPreShiftDownhillDisplayStatusChanged::Input_PreShiftDownhillDisplayStatus;
    using PreShiftOtherDisplayStatus = ArgumentsPreShiftOtherDisplayStatusChanged::Input_PreShiftOtherDisplayStatus;
    using PreShiftTextDisplayStatus = ArgumentsPreShiftTextDisplayStatusChanged::Input_PreShiftTextDisplayStatus;

    ConfigPredictShift mConfigPredictShift = ConfigPredictShift::OFF;
    PreShiftSystemStatus mPreShiftSystemStatus = PreShiftSystemStatus::OFF;
    PreShiftVariableDisplayStatus mPreShiftVariableDisplayStatus = PreShiftVariableDisplayStatus::NONE;
    PreShiftCurveDisplayStatus mPreShiftCurveDisplayStatus = PreShiftCurveDisplayStatus::OFF;
    PreShiftMergeDisplayStatus mPreShiftMergeDisplayStatus = PreShiftMergeDisplayStatus::OFF;
    PreShiftSpeedLimitDisplayStatus mPreShiftSpeedLimitDisplayStatus = PreShiftSpeedLimitDisplayStatus::OFF;
    PreShiftStopSignDisplayStatus mPreShiftStopSignDisplayStatus = PreShiftStopSignDisplayStatus::OFF;
    PreShiftRoundaboutDisplayStatus mPreShiftRoundaboutDisplayStatus = PreShiftRoundaboutDisplayStatus::OFF;
    PreShiftFrontCarDisplayStatus mPreShiftFrontCarDisplayStatus = PreShiftFrontCarDisplayStatus::OFF;
    PreShiftDownhillDisplayStatus mPreShiftDownhillDisplayStatus = PreShiftDownhillDisplayStatus::OFF;
    PreShiftOtherDisplayStatus mPreShiftOtherDisplayStatus = PreShiftOtherDisplayStatus::OFF;
    PreShiftTextDisplayStatus mPreShiftTextDisplayStatus = PreShiftTextDisplayStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Predictive_Shift_H
