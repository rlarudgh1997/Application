/**
 * @file TM_High_Low_Range.hpp
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
#ifndef SFSS_TM_High_Low_Range_H
#define SFSS_TM_High_Low_Range_H

#define DLOG_ENABLED gEnableSFCLog

#include "TM_High_Low_RangeBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.06
// Reference : [CV230] TM_High_Low Range. Contains : Telltale. Recommend: VALUE_CHANGED
class TM_High_Low_Range : public TM_High_Low_RangeBase {
public:
    TM_High_Low_Range() = default;
    ~TM_High_Low_Range() override = default;
    TM_High_Low_Range(const TM_High_Low_Range& other) = delete;
    TM_High_Low_Range(TM_High_Low_Range&& other) noexcept = delete;
    TM_High_Low_Range& operator=(const TM_High_Low_Range& other) = delete;
    TM_High_Low_Range& operator=(TM_High_Low_Range&& other) noexcept = delete;

    void onInitialize() override {
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateTMHighTelltale();
        updateLowRangeTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateTMHighTelltale();
        updateLowRangeTelltale();
    }

    void onTmHighStatusChanged(const ArgumentsTmHighStatusChanged& args) {
        mTMHighStatus = args.mInput_TMHighStatus;
        updateTMHighTelltale();
    }

    void onLowRangeStatusChanged(const ArgumentsLowRangeStatusChanged& args) {
        mLowRangeStatus = args.mInput_LowRangeStatus;
        updateLowRangeTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTMHighTelltale();
        updateLowRangeTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTMHighTelltale() {
        SFCTM_High_Low_RangeTelltaleTM_HighStat telltaleTMHighStatus{SFCTM_High_Low_RangeTelltaleTM_HighStat::OFF};

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        if (mTMHighStatus == TMHighStatus::ON && mIsIgnOn) {
            telltaleTMHighStatus = SFCTM_High_Low_RangeTelltaleTM_HighStat::ON;
        }
        setSFCTM_High_Low_RangeTelltaleTM_HighStat(telltaleTMHighStatus);
    }

    void updateLowRangeTelltale() {
        SFCTM_High_Low_RangeTelltaleLow_RangeStat telltaleLowRangeStatus{SFCTM_High_Low_RangeTelltaleLow_RangeStat::OFF};

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        if (mLowRangeStatus == LowRangeStatus::ON && mIsIgnOn) {
            telltaleLowRangeStatus = SFCTM_High_Low_RangeTelltaleLow_RangeStat::ON;
        }
        setSFCTM_High_Low_RangeTelltaleLow_RangeStat(telltaleLowRangeStatus);
    }

    HBool mIsIgnOn = false;
    using TMHighStatus = ArgumentsTmHighStatusChanged::Input_TMHighStatus;
    using LowRangeStatus = ArgumentsLowRangeStatusChanged::Input_LowRangeStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    TMHighStatus mTMHighStatus = TMHighStatus::OFF;
    LowRangeStatus mLowRangeStatus = LowRangeStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
};

}  // namespace ccos

#endif  // SFSS_TM_High_Low_Range_H
