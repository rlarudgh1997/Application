/**
 * @file Detail_Contents_N.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Detail_Contents_N_H
#define SFSS_Detail_Contents_N_H

#define DLOG_ENABLED gEnableSFCLog

#include <map>
#include <cassert>
#include "Detail_Contents_NBase.hpp"

namespace ccos {

// SFC Version : 6.1.0
// Reference : [HD190] Detail Contents_N. Contains : Constant. Recommend: VALUE_CHANGED
class Detail_Contents_N : public Detail_Contents_NBase {
public:
    Detail_Contents_N() = default;
    ~Detail_Contents_N() override = default;
    Detail_Contents_N(const Detail_Contents_N& other) = delete;
    Detail_Contents_N(Detail_Contents_N&& other) noexcept = delete;
    Detail_Contents_N& operator=(const Detail_Contents_N& other) = delete;
    Detail_Contents_N& operator=(Detail_Contents_N&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;

        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;

        updateRpmValue();
        updateAgiUpStatus();
        updateAgiDownStatus();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;

        updateAll();
    }

    void onMaxRpmStatusChanged(const ArgumentsMaxRpmStatusChanged& args) {
        mMaxRpmStat = args.mMaxRpmStat;

        updateMaxRpmStat();
        updateRpmValue();
    }

    void onRegenNpedalLvlStatChanged(const ArgumentsRegenNpedalLvlStatChanged& args) {
        mNpedalLvlStat = args.mNpedalLvlStat;

        updateRegenNpedalLvlStat();
    }

    void onRpmValueChanged(const ArgumentsRpmValueChanged& args) {
        mRpmValue = args.mRpmValue;

        updateRpmValue();
    }

    void onRpmDampStatChanged(const ArgumentsRpmDampStatChanged& args) {
        mRpmDampStat = args.mRpmDampStat;

        updateRpmDampStat();
    }

    void onRedZoneRangeStatChanged(const ArgumentsRedZoneRangeStatChanged& args) {
        mRedZoneRangeStat = args.mRedZoneNbrandStat;

        updateRpmRedZoneRangeStat();
    }

    void onTelltaleShiftLightChanged(const ArgumentsTelltaleShiftLightChanged& args) {
        mTelltaleShiftLightStat = args.mShiftLightStat;
        mTelltaleShiftLightBlinkValueA = args.mShiftLightBlinkValueA;
        mTelltaleShiftLightBlinkValueB = args.mShiftLightBlinkValueB;
        mTelltaleShiftLightBlinkValueBOptional = args.mShiftLightBlinkValueBOptional;
        mTelltaleShiftLightStatOptional = args.mShiftLightStatOptional;

        updateShiftLightIndicatorStat();
    }

    void onTransmissionChanged(const ArgumentsTransmissionChanged& args) {
        mTransmissionDisplay = args.mTransmissionDisplayValue;
        mTransmissionBlinkingStat = args.mTransmissionBlinkingStat;
        mTransmissionBlinkingA = args.mTransmissionBlinkingBlinkValueA;
        mTransmissionBlinkingB = args.mTransmissionBlinkingBlinkValueB;

        updateTransmissionDisplayValue();
    }

    void onTransmissionAgiStatusChanged(const ArgumentsTransmissionAgiStatusChanged& args) {
        mTransmissionAgiUpStatus = args.mTransmissionAgiUpStat;
        mTransmissionAgiDownStatus = args.mTransmissionAgiDownStat;

        updateAgiUpStatus();
        updateAgiDownStatus();
    }

    void onBoostTimerChanged(const ArgumentsBoostTimerChanged& args) {
        mBoostTimerValue = args.mBoostTimerValue;
        mBoostTimerStatus = args.mBoostTimerStat;

        updateBoostTimerValue();
        updateBoostTimerStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    inline HBool isNPerformanceOn() const {
        return (mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON) &&
               mConfigHighPerformance == ConfigHighPerformance::N;
    }

    inline HBool isIgnNPerformanceOn() const {
        return mIsIgnOn == true && isNPerformanceOn() == true;
    }

    template <typename KEY, typename VALUE>
    const VALUE getMappingData(const std::map<KEY, VALUE>& mapTable, const KEY& key) {
        VALUE ret = static_cast<VALUE>(0);
        const auto& it = mapTable.find(key);
        if (it != mapTable.end()) {
            ret = it->second;
        } else {
            assert(true);
        }

        return ret;
    }

    void updateAll() {
        updateMaxRpmStat();
        updateRpmValue();
        updateRpmDampStat();
        updateRpmRedZoneRangeStat();
        updateTransmissionDisplayValue();
        updateShiftLightIndicatorStat();
        updateRegenNpedalLvlStat();
        updateAgiUpStatus();
        updateAgiDownStatus();
        updateBoostTimerValue();
        updateBoostTimerStatus();
    }

    void updateMaxRpmStat() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantMaxRpmStat stat = SFCDetail_Contents_NConstantMaxRpmStat::NONE;
        stat = getMappingData<MaxRpmStat, SFCDetail_Contents_NConstantMaxRpmStat>(mMaxRpmStatMap, mMaxRpmStat);
        setSFCDetail_Contents_NConstantMaxRpmStat(stat);
    }

    void updateRpmValue() {
        if (isNPerformanceOn() == false) {
            return;
        }

        HDouble rpmValue = mRpmValue;
        HDouble maxRpmValue = 0.0;

        if (mIsIgnOn == true) {
            if (mMaxRpmStat == MaxRpmStat::RPM6000) {
                maxRpmValue = 6000.0;
            } else if (mMaxRpmStat == MaxRpmStat::RPM8000) {
                maxRpmValue = 8000.0;
            } else {
                // Error
            }
        }

        rpmValue = std::max(0.0, rpmValue);
        rpmValue = std::min(maxRpmValue, rpmValue);

        setSFCDetail_Contents_NConstantRpmValue(rpmValue);
    }

    void updateRpmDampStat() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantRpmDampStat stat = SFCDetail_Contents_NConstantRpmDampStat::NO_DAMPING;
        stat = getMappingData<RpmDampStat, SFCDetail_Contents_NConstantRpmDampStat>(mRpmDampStatMap, mRpmDampStat);
        setSFCDetail_Contents_NConstantRpmDampStat(stat);
    }

    void updateRpmRedZoneRangeStat() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantRedZoneRangeStat stat = SFCDetail_Contents_NConstantRedZoneRangeStat::NONE;
        stat = getMappingData<RedZoneRangeStat, SFCDetail_Contents_NConstantRedZoneRangeStat>(mRpmRedZoneRangeStatMap,
                                                                                              mRedZoneRangeStat);
        setSFCDetail_Contents_NConstantRedZoneRangeStat(stat);
    }

    void updateTransmissionDisplayValue() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantTransmissionDisplayValue value =
            SFCDetail_Contents_NConstantTransmissionDisplayValue::DISPLAY_OFF;
        SFCDetail_Contents_NConstantTransmissionBlinkingStat stat = SFCDetail_Contents_NConstantTransmissionBlinkingStat::NONE;
        SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA blinkA =
            SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISPLAY_OFF;
        SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB blinkB =
            SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISPLAY_OFF;

        value = getMappingData<TransmissionDisplayValue, SFCDetail_Contents_NConstantTransmissionDisplayValue>(
            mTransmissionDisplayValueMap, mTransmissionDisplay);
        stat = getMappingData<TransmissionBlinkingStat, SFCDetail_Contents_NConstantTransmissionBlinkingStat>(
            mTransmissionBlinkingStatMap, mTransmissionBlinkingStat);
        blinkA = getMappingData<TransmissionBlinkingA, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA>(
            mTransmissionBlinkValueAMap, mTransmissionBlinkingA);
        blinkB = getMappingData<TransmissionBlinkingB, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB>(
            mTransmissionBlinkValueBMap, mTransmissionBlinkingB);

        setSFCDetail_Contents_NConstantTransmissionDisplayValue(value);
        setSFCDetail_Contents_NConstantTransmissionBlinkingStat(stat);
        setSFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA(blinkA);
        setSFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB(blinkB);
    }

    void updateShiftLightIndicatorStat() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantShiftLightIndicatorStat stat = SFCDetail_Contents_NConstantShiftLightIndicatorStat::NONE;
        SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA blinkA =
            SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::NONE;
        SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB blinkB =
            SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::NONE;
        SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional blinkBOptional =
            SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::NONE;
        SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional statOptional =
            SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::NONE;

        stat = getMappingData<TelltaleShiftLightStat, SFCDetail_Contents_NConstantShiftLightIndicatorStat>(
            mShiftLightIndicatorStatMap, mTelltaleShiftLightStat);
        blinkA = getMappingData<TelltaleShiftLightBlinkValueA, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA>(
            mShiftLightIndicatorBlinkValueAMap, mTelltaleShiftLightBlinkValueA);
        blinkB = getMappingData<TelltaleShiftLightBlinkValueB, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB>(
            mShiftLightIndicatorBlinkValueBMap, mTelltaleShiftLightBlinkValueB);
        blinkBOptional = getMappingData<TelltaleShiftLightBlinkValueBOptional,
                                        SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional>(
            mShiftLightIndicatorBlinkValueBOptionalMap, mTelltaleShiftLightBlinkValueBOptional);
        statOptional =
            getMappingData<TelltaleShiftLightStatOptional, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional>(
                mShiftLightIndicatorStatOptionalMap, mTelltaleShiftLightStatOptional);

        setSFCDetail_Contents_NConstantShiftLightIndicatorStat(stat);
        setSFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA(blinkA);
        setSFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB(blinkB);
        setSFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional(blinkBOptional);
        setSFCDetail_Contents_NConstantShiftLightIndicatorStatOptional(statOptional);
    }

    void updateRegenNpedalLvlStat() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantNpedalLvlStat stat = SFCDetail_Contents_NConstantNpedalLvlStat::DISPLAY_OFF;

        stat = getMappingData<NpedalLvlStat, SFCDetail_Contents_NConstantNpedalLvlStat>(mRegenNpedalMap, mNpedalLvlStat);
        setSFCDetail_Contents_NConstantNpedalLvlStat(stat);
    }

    void updateAgiUpStatus() {
        if (isNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantTransmissionAgiUpStat stat = SFCDetail_Contents_NConstantTransmissionAgiUpStat::DISPLAY_OFF;
        stat = getMappingData<TransmissionAgiUpStatus, SFCDetail_Contents_NConstantTransmissionAgiUpStat>(
            mTransmissionAgiUpMap, mTransmissionAgiUpStatus);
        setSFCDetail_Contents_NConstantTransmissionAgiUpStat(stat);
    }

    void updateAgiDownStatus() {
        if (isNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantTransmissionAgiDownStat stat =
            SFCDetail_Contents_NConstantTransmissionAgiDownStat::DISPLAY_OFF;
        stat = getMappingData<TransmissionAgiDownStatus, SFCDetail_Contents_NConstantTransmissionAgiDownStat>(
            mTransmissionAgiDownMap, mTransmissionAgiDownStatus);
        setSFCDetail_Contents_NConstantTransmissionAgiDownStat(stat);
    }

    void updateBoostTimerValue() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        HUInt64 value = 0;
        if (0 <= mBoostTimerValue && mBoostTimerValue <= 20) {
            value = mBoostTimerValue;
        }

        setSFCDetail_Contents_NConstantBoostTimerValue(value);
    }

    void updateBoostTimerStatus() {
        if (isIgnNPerformanceOn() == false) {
            return;
        }

        SFCDetail_Contents_NConstantBoostTimerStat stat = SFCDetail_Contents_NConstantBoostTimerStat::NONE;

        stat = getMappingData<BoostTimerStatus, SFCDetail_Contents_NConstantBoostTimerStat>(mBoostTimerMap, mBoostTimerStatus);
        setSFCDetail_Contents_NConstantBoostTimerStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigHUD = ArgumentsConfigChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigChanged::Inter_ConfigNewHUD;
    using ConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using MaxRpmStat = ArgumentsMaxRpmStatusChanged::MaxRpmStat;
    using NpedalLvlStat = ArgumentsRegenNpedalLvlStatChanged::NpedalLvlStat;
    using RpmDampStat = ArgumentsRpmDampStatChanged::RpmDampStat;
    using RedZoneRangeStat = ArgumentsRedZoneRangeStatChanged::RedZoneNbrandStat;
    using TelltaleShiftLightStat = ArgumentsTelltaleShiftLightChanged::ShiftLightStat;
    using TelltaleShiftLightBlinkValueA = ArgumentsTelltaleShiftLightChanged::ShiftLightBlinkValueA;
    using TelltaleShiftLightBlinkValueB = ArgumentsTelltaleShiftLightChanged::ShiftLightBlinkValueB;
    using TelltaleShiftLightBlinkValueBOptional = ArgumentsTelltaleShiftLightChanged::ShiftLightBlinkValueBOptional;
    using TelltaleShiftLightStatOptional = ArgumentsTelltaleShiftLightChanged::ShiftLightStatOptional;
    using TransmissionDisplayValue = ArgumentsTransmissionChanged::TransmissionDisplayValue;
    using TransmissionBlinkingStat = ArgumentsTransmissionChanged::TransmissionBlinkingStat;
    using TransmissionBlinkingA = ArgumentsTransmissionChanged::TransmissionBlinkingBlinkValueA;
    using TransmissionBlinkingB = ArgumentsTransmissionChanged::TransmissionBlinkingBlinkValueB;
    using TransmissionAgiUpStatus = ArgumentsTransmissionAgiStatusChanged::TransmissionAgiUpStat;
    using TransmissionAgiDownStatus = ArgumentsTransmissionAgiStatusChanged::TransmissionAgiDownStat;
    using BoostTimerStatus = ArgumentsBoostTimerChanged::BoostTimerStat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    HDouble mRpmValue = 0.0;
    HUInt64 mBoostTimerValue = 0;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;
    MaxRpmStat mMaxRpmStat = MaxRpmStat::NONE;
    NpedalLvlStat mNpedalLvlStat = NpedalLvlStat::NONE;
    RpmDampStat mRpmDampStat = RpmDampStat::NONE;
    RedZoneRangeStat mRedZoneRangeStat = RedZoneRangeStat::NONE;
    TelltaleShiftLightStat mTelltaleShiftLightStat = TelltaleShiftLightStat::NONE;
    TelltaleShiftLightBlinkValueA mTelltaleShiftLightBlinkValueA = TelltaleShiftLightBlinkValueA::NONE;
    TelltaleShiftLightBlinkValueB mTelltaleShiftLightBlinkValueB = TelltaleShiftLightBlinkValueB::NONE;
    TelltaleShiftLightBlinkValueBOptional mTelltaleShiftLightBlinkValueBOptional = TelltaleShiftLightBlinkValueBOptional::NONE;
    TelltaleShiftLightStatOptional mTelltaleShiftLightStatOptional = TelltaleShiftLightStatOptional::NONE;
    TransmissionDisplayValue mTransmissionDisplay = TransmissionDisplayValue::NONE;
    TransmissionBlinkingStat mTransmissionBlinkingStat = TransmissionBlinkingStat::NONE;
    TransmissionBlinkingA mTransmissionBlinkingA = TransmissionBlinkingA::NONE;
    TransmissionBlinkingB mTransmissionBlinkingB = TransmissionBlinkingB::NONE;
    TransmissionAgiUpStatus mTransmissionAgiUpStatus = TransmissionAgiUpStatus::NONE;
    TransmissionAgiDownStatus mTransmissionAgiDownStatus = TransmissionAgiDownStatus::NONE;
    BoostTimerStatus mBoostTimerStatus = BoostTimerStatus::NONE;

    std::map<MaxRpmStat, SFCDetail_Contents_NConstantMaxRpmStat> mMaxRpmStatMap = {
        {MaxRpmStat::NONE, SFCDetail_Contents_NConstantMaxRpmStat::NONE},
        {MaxRpmStat::RPM6000, SFCDetail_Contents_NConstantMaxRpmStat::RPM6000},
        {MaxRpmStat::RPM8000, SFCDetail_Contents_NConstantMaxRpmStat::RPM8000}};

    std::map<RpmDampStat, SFCDetail_Contents_NConstantRpmDampStat> mRpmDampStatMap = {
        {RpmDampStat::NONE, SFCDetail_Contents_NConstantRpmDampStat::NONE},
        {RpmDampStat::HOLD, SFCDetail_Contents_NConstantRpmDampStat::HOLD},
        {RpmDampStat::DAMPING_LV0, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV0},
        {RpmDampStat::DAMPING_LV1, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV1},
        {RpmDampStat::DAMPING_LV2, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV2},
        {RpmDampStat::DAMPING_LV3, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV3},
        {RpmDampStat::DAMPING_LV4, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV4},
        {RpmDampStat::DAMPING_LV5, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV5},
        {RpmDampStat::DAMPING_LV6, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV6},
        {RpmDampStat::DAMPING_LV7, SFCDetail_Contents_NConstantRpmDampStat::DAMPING_LV7},
        {RpmDampStat::NO_DAMPING, SFCDetail_Contents_NConstantRpmDampStat::NO_DAMPING}};

    std::map<RedZoneRangeStat, SFCDetail_Contents_NConstantRedZoneRangeStat> mRpmRedZoneRangeStatMap = {
        {RedZoneRangeStat::NONE, SFCDetail_Contents_NConstantRedZoneRangeStat::NONE},
        {RedZoneRangeStat::STEP_FROM4000, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM4000},
        {RedZoneRangeStat::STEP_FROM4250, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM4250},
        {RedZoneRangeStat::STEP_FROM4500, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM4500},
        {RedZoneRangeStat::STEP_FROM4750, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM4750},
        {RedZoneRangeStat::STEP_FROM5000, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM5000},
        {RedZoneRangeStat::STEP_FROM5250, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM5250},
        {RedZoneRangeStat::STEP_FROM5500, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM5500},
        {RedZoneRangeStat::STEP_FROM5750, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM5750},
        {RedZoneRangeStat::STEP_FROM6000, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM6000},
        {RedZoneRangeStat::STEP_FROM6250, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM6250},
        {RedZoneRangeStat::STEP_FROM6500, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM6500},
        {RedZoneRangeStat::STEP_FROM6750, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM6750},
        {RedZoneRangeStat::STEP_FROM7000, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM7000},
        {RedZoneRangeStat::STEP_FROM7750, SFCDetail_Contents_NConstantRedZoneRangeStat::STEP_FROM7750}};

    std::map<TransmissionDisplayValue, SFCDetail_Contents_NConstantTransmissionDisplayValue> mTransmissionDisplayValueMap = {
        {TransmissionDisplayValue::NONE, SFCDetail_Contents_NConstantTransmissionDisplayValue::NONE},
        {TransmissionDisplayValue::DISPLAY_OFF, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISPLAY_OFF},
        {TransmissionDisplayValue::DISP_P, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_P},
        {TransmissionDisplayValue::DISP_N, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_N},
        {TransmissionDisplayValue::DISP_R, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_R},
        {TransmissionDisplayValue::DISP_D, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D},
        {TransmissionDisplayValue::DISP_S, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S},
        {TransmissionDisplayValue::DISP_E, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_E},
        {TransmissionDisplayValue::SPEED1, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_1},
        {TransmissionDisplayValue::SPEED2, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_2},
        {TransmissionDisplayValue::SPEED3, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_3},
        {TransmissionDisplayValue::SPEED4, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_4},
        {TransmissionDisplayValue::SPEED5, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_5},
        {TransmissionDisplayValue::SPEED6, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_6},
        {TransmissionDisplayValue::SPEED7, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_7},
        {TransmissionDisplayValue::SPEED8, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_8},
        {TransmissionDisplayValue::DISP_S1, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S1},
        {TransmissionDisplayValue::DISP_S2, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S2},
        {TransmissionDisplayValue::DISP_S3, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S3},
        {TransmissionDisplayValue::DISP_S4, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S4},
        {TransmissionDisplayValue::DISP_S5, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S5},
        {TransmissionDisplayValue::DISP_S6, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S6},
        {TransmissionDisplayValue::DISP_S7, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S7},
        {TransmissionDisplayValue::DISP_S8, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_S8},
        {TransmissionDisplayValue::DISP_D1, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D1},
        {TransmissionDisplayValue::DISP_D2, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D2},
        {TransmissionDisplayValue::DISP_D3, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D3},
        {TransmissionDisplayValue::DISP_D4, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D4},
        {TransmissionDisplayValue::DISP_D5, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D5},
        {TransmissionDisplayValue::DISP_D6, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D6},
        {TransmissionDisplayValue::DISP_D7, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D7},
        {TransmissionDisplayValue::DISP_D8, SFCDetail_Contents_NConstantTransmissionDisplayValue::DISP_D8}};

    std::map<TransmissionBlinkingStat, SFCDetail_Contents_NConstantTransmissionBlinkingStat> mTransmissionBlinkingStatMap = {
        {TransmissionBlinkingStat::NONE, SFCDetail_Contents_NConstantTransmissionBlinkingStat::NONE},
        {TransmissionBlinkingStat::BLINK2, SFCDetail_Contents_NConstantTransmissionBlinkingStat::BLINK2},
        {TransmissionBlinkingStat::VALUE, SFCDetail_Contents_NConstantTransmissionBlinkingStat::VALUE}};

    std::map<TransmissionBlinkingA, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA> mTransmissionBlinkValueAMap = {
        {TransmissionBlinkingA::NONE, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::NONE},
        {TransmissionBlinkingA::DISPLAY_OFF, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISPLAY_OFF},
        {TransmissionBlinkingA::DISP_P, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_P},
        {TransmissionBlinkingA::DISP_N, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_N},
        {TransmissionBlinkingA::DISP_R, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_R},
        {TransmissionBlinkingA::DISP_D, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D},
        {TransmissionBlinkingA::DISP_S, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S},
        {TransmissionBlinkingA::DISP_E, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_E},
        {TransmissionBlinkingA::SPEED1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_1},
        {TransmissionBlinkingA::SPEED2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_2},
        {TransmissionBlinkingA::SPEED3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_3},
        {TransmissionBlinkingA::SPEED4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_4},
        {TransmissionBlinkingA::SPEED5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_5},
        {TransmissionBlinkingA::SPEED6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_6},
        {TransmissionBlinkingA::SPEED7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_7},
        {TransmissionBlinkingA::SPEED8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_8},
        {TransmissionBlinkingA::DISP_S1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S1},
        {TransmissionBlinkingA::DISP_S2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S2},
        {TransmissionBlinkingA::DISP_S3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S3},
        {TransmissionBlinkingA::DISP_S4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S4},
        {TransmissionBlinkingA::DISP_S5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S5},
        {TransmissionBlinkingA::DISP_S6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S6},
        {TransmissionBlinkingA::DISP_S7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S7},
        {TransmissionBlinkingA::DISP_S8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_S8},
        {TransmissionBlinkingA::DISP_D1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D1},
        {TransmissionBlinkingA::DISP_D2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D2},
        {TransmissionBlinkingA::DISP_D3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D3},
        {TransmissionBlinkingA::DISP_D4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D4},
        {TransmissionBlinkingA::DISP_D5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D5},
        {TransmissionBlinkingA::DISP_D6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D6},
        {TransmissionBlinkingA::DISP_D7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D7},
        {TransmissionBlinkingA::DISP_D8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueA::DISP_D8}};

    std::map<TransmissionBlinkingB, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB> mTransmissionBlinkValueBMap = {
        {TransmissionBlinkingB::NONE, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::NONE},
        {TransmissionBlinkingB::DISPLAY_OFF, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISPLAY_OFF},
        {TransmissionBlinkingB::DISP_P, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_P},
        {TransmissionBlinkingB::DISP_N, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_N},
        {TransmissionBlinkingB::DISP_R, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_R},
        {TransmissionBlinkingB::DISP_D, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D},
        {TransmissionBlinkingB::DISP_S, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S},
        {TransmissionBlinkingB::DISP_E, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_E},
        {TransmissionBlinkingB::SPEED1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_1},
        {TransmissionBlinkingB::SPEED2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_2},
        {TransmissionBlinkingB::SPEED3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_3},
        {TransmissionBlinkingB::SPEED4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_4},
        {TransmissionBlinkingB::SPEED5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_5},
        {TransmissionBlinkingB::SPEED6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_6},
        {TransmissionBlinkingB::SPEED7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_7},
        {TransmissionBlinkingB::SPEED8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_8},
        {TransmissionBlinkingB::DISP_S1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S1},
        {TransmissionBlinkingB::DISP_S2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S2},
        {TransmissionBlinkingB::DISP_S3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S3},
        {TransmissionBlinkingB::DISP_S4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S4},
        {TransmissionBlinkingB::DISP_S5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S5},
        {TransmissionBlinkingB::DISP_S6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S6},
        {TransmissionBlinkingB::DISP_S7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S7},
        {TransmissionBlinkingB::DISP_S8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_S8},
        {TransmissionBlinkingB::DISP_D1, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D1},
        {TransmissionBlinkingB::DISP_D2, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D2},
        {TransmissionBlinkingB::DISP_D3, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D3},
        {TransmissionBlinkingB::DISP_D4, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D4},
        {TransmissionBlinkingB::DISP_D5, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D5},
        {TransmissionBlinkingB::DISP_D6, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D6},
        {TransmissionBlinkingB::DISP_D7, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D7},
        {TransmissionBlinkingB::DISP_D8, SFCDetail_Contents_NConstantTransmissionBlinkingBlinkValueB::DISP_D8}};

    std::map<TelltaleShiftLightStat, SFCDetail_Contents_NConstantShiftLightIndicatorStat> mShiftLightIndicatorStatMap = {
        {TelltaleShiftLightStat::NONE, SFCDetail_Contents_NConstantShiftLightIndicatorStat::NONE},
        {TelltaleShiftLightStat::OFF, SFCDetail_Contents_NConstantShiftLightIndicatorStat::DEFAULT},
        {TelltaleShiftLightStat::ON, SFCDetail_Contents_NConstantShiftLightIndicatorStat::ON},
        {TelltaleShiftLightStat::BLINK1, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLINK1},
        {TelltaleShiftLightStat::BLINK2, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLINK2},
        {TelltaleShiftLightStat::BLINK3, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLINK3},
        {TelltaleShiftLightStat::BLINK4, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLINK4},
        {TelltaleShiftLightStat::BLINK5, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLINK5},
        {TelltaleShiftLightStat::RED, SFCDetail_Contents_NConstantShiftLightIndicatorStat::RED},
        {TelltaleShiftLightStat::GREEN, SFCDetail_Contents_NConstantShiftLightIndicatorStat::GREEN},
        {TelltaleShiftLightStat::BLUE, SFCDetail_Contents_NConstantShiftLightIndicatorStat::BLUE},
        {TelltaleShiftLightStat::YELLOW, SFCDetail_Contents_NConstantShiftLightIndicatorStat::YELLOW},
        {TelltaleShiftLightStat::AMBER, SFCDetail_Contents_NConstantShiftLightIndicatorStat::AMBER},
        {TelltaleShiftLightStat::GRAY, SFCDetail_Contents_NConstantShiftLightIndicatorStat::GRAY},
        {TelltaleShiftLightStat::WHITE, SFCDetail_Contents_NConstantShiftLightIndicatorStat::WHITE},
        {TelltaleShiftLightStat::OPTIONAL, SFCDetail_Contents_NConstantShiftLightIndicatorStat::OPTIONAL},
        {TelltaleShiftLightStat::CYAN, SFCDetail_Contents_NConstantShiftLightIndicatorStat::CYAN}};

    std::map<TelltaleShiftLightBlinkValueA, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA>
        mShiftLightIndicatorBlinkValueAMap = {
            {TelltaleShiftLightBlinkValueA::NONE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::NONE},
            {TelltaleShiftLightBlinkValueA::OFF, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::OFF},
            {TelltaleShiftLightBlinkValueA::ON, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::ON},
            {TelltaleShiftLightBlinkValueA::ACTIVE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::ACTIVE},
            {TelltaleShiftLightBlinkValueA::INACTIVE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::INACTIVE},
            {TelltaleShiftLightBlinkValueA::TICK, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::TICK},
            {TelltaleShiftLightBlinkValueA::TOCK, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::TOCK},
            {TelltaleShiftLightBlinkValueA::ENABLE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::ENABLE},
            {TelltaleShiftLightBlinkValueA::DISABLE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::DISABLE},
            {TelltaleShiftLightBlinkValueA::BLINK1, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLINK1},
            {TelltaleShiftLightBlinkValueA::BLINK2, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLINK2},
            {TelltaleShiftLightBlinkValueA::BLINK3, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLINK3},
            {TelltaleShiftLightBlinkValueA::BLINK4, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLINK4},
            {TelltaleShiftLightBlinkValueA::BLINK5, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLINK5},
            {TelltaleShiftLightBlinkValueA::RED, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::RED},
            {TelltaleShiftLightBlinkValueA::GREEN, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::GREEN},
            {TelltaleShiftLightBlinkValueA::BLUE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::BLUE},
            {TelltaleShiftLightBlinkValueA::YELLOW, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::YELLOW},
            {TelltaleShiftLightBlinkValueA::AMBER, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::AMBER},
            {TelltaleShiftLightBlinkValueA::GRAY, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::GRAY},
            {TelltaleShiftLightBlinkValueA::WHITE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::WHITE},
            {TelltaleShiftLightBlinkValueA::OPTIONAL, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::OPTIONAL},
            {TelltaleShiftLightBlinkValueA::CYAN, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueA::CYAN}};

    std::map<TelltaleShiftLightBlinkValueB, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB>
        mShiftLightIndicatorBlinkValueBMap = {
            {TelltaleShiftLightBlinkValueB::NONE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::NONE},
            {TelltaleShiftLightBlinkValueB::OFF, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::OFF},
            {TelltaleShiftLightBlinkValueB::ON, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::ON},
            {TelltaleShiftLightBlinkValueB::ACTIVE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::ACTIVE},
            {TelltaleShiftLightBlinkValueB::INACTIVE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::INACTIVE},
            {TelltaleShiftLightBlinkValueB::TICK, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::TICK},
            {TelltaleShiftLightBlinkValueB::TOCK, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::TOCK},
            {TelltaleShiftLightBlinkValueB::ENABLE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::ENABLE},
            {TelltaleShiftLightBlinkValueB::DISABLE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::DISABLE},
            {TelltaleShiftLightBlinkValueB::BLINK1, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLINK1},
            {TelltaleShiftLightBlinkValueB::BLINK2, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLINK2},
            {TelltaleShiftLightBlinkValueB::BLINK3, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLINK3},
            {TelltaleShiftLightBlinkValueB::BLINK4, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLINK4},
            {TelltaleShiftLightBlinkValueB::BLINK5, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLINK5},
            {TelltaleShiftLightBlinkValueB::RED, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::RED},
            {TelltaleShiftLightBlinkValueB::GREEN, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::GREEN},
            {TelltaleShiftLightBlinkValueB::BLUE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::BLUE},
            {TelltaleShiftLightBlinkValueB::YELLOW, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::YELLOW},
            {TelltaleShiftLightBlinkValueB::AMBER, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::AMBER},
            {TelltaleShiftLightBlinkValueB::GRAY, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::GRAY},
            {TelltaleShiftLightBlinkValueB::WHITE, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::WHITE},
            {TelltaleShiftLightBlinkValueB::OPTIONAL, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::OPTIONAL},
            {TelltaleShiftLightBlinkValueB::CYAN, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueB::CYAN}};

    std::map<TelltaleShiftLightBlinkValueBOptional, SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional>
        mShiftLightIndicatorBlinkValueBOptionalMap = {
            {TelltaleShiftLightBlinkValueBOptional::STEP1,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP1},
            {TelltaleShiftLightBlinkValueBOptional::STEP2,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP2},
            {TelltaleShiftLightBlinkValueBOptional::STEP3,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP3},
            {TelltaleShiftLightBlinkValueBOptional::STEP4,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP4},
            {TelltaleShiftLightBlinkValueBOptional::STEP5,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP5},
            {TelltaleShiftLightBlinkValueBOptional::STEP6,
             SFCDetail_Contents_NConstantShiftLightIndicatorBlinkValueBOptional::STEP6}};

    std::map<TelltaleShiftLightStatOptional, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional>
        mShiftLightIndicatorStatOptionalMap = {
            {TelltaleShiftLightStatOptional::STEP1, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP1},
            {TelltaleShiftLightStatOptional::STEP2, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP2},
            {TelltaleShiftLightStatOptional::STEP3, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP3},
            {TelltaleShiftLightStatOptional::STEP4, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP4},
            {TelltaleShiftLightStatOptional::STEP5, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP5},
            {TelltaleShiftLightStatOptional::STEP6, SFCDetail_Contents_NConstantShiftLightIndicatorStatOptional::STEP6}};

    std::map<NpedalLvlStat, SFCDetail_Contents_NConstantNpedalLvlStat> mRegenNpedalMap = {
        {NpedalLvlStat::NONE, SFCDetail_Contents_NConstantNpedalLvlStat::NONE},
        {NpedalLvlStat::DISPLAY_OFF, SFCDetail_Contents_NConstantNpedalLvlStat::DISPLAY_OFF},
        {NpedalLvlStat::NPEDAL_LV1, SFCDetail_Contents_NConstantNpedalLvlStat::NPEDAL_LV1},
        {NpedalLvlStat::NPEDAL_LV2, SFCDetail_Contents_NConstantNpedalLvlStat::NPEDAL_LV2},
        {NpedalLvlStat::NPEDAL_LV3, SFCDetail_Contents_NConstantNpedalLvlStat::NPEDAL_LV3}};

    std::map<TransmissionAgiUpStatus, SFCDetail_Contents_NConstantTransmissionAgiUpStat> mTransmissionAgiUpMap = {
        {TransmissionAgiUpStatus::NONE, SFCDetail_Contents_NConstantTransmissionAgiUpStat::NONE},
        {TransmissionAgiUpStatus::DISPLAY_OFF, SFCDetail_Contents_NConstantTransmissionAgiUpStat::DISPLAY_OFF},
        {TransmissionAgiUpStatus::AVAILABLE_1, SFCDetail_Contents_NConstantTransmissionAgiUpStat::DISPLAY_OFF},
        {TransmissionAgiUpStatus::AVAILABLE_2, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_2},
        {TransmissionAgiUpStatus::AVAILABLE_3, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_3},
        {TransmissionAgiUpStatus::AVAILABLE_4, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_4},
        {TransmissionAgiUpStatus::AVAILABLE_5, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_5},
        {TransmissionAgiUpStatus::AVAILABLE_6, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_6},
        {TransmissionAgiUpStatus::AVAILABLE_7, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_7},
        {TransmissionAgiUpStatus::AVAILABLE_8, SFCDetail_Contents_NConstantTransmissionAgiUpStat::AVAILABLE_8},
        {TransmissionAgiUpStatus::UNAVAILABLE_1, SFCDetail_Contents_NConstantTransmissionAgiUpStat::DISPLAY_OFF},
        {TransmissionAgiUpStatus::UNAVAILABLE_2, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_2},
        {TransmissionAgiUpStatus::UNAVAILABLE_3, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_3},
        {TransmissionAgiUpStatus::UNAVAILABLE_4, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_4},
        {TransmissionAgiUpStatus::UNAVAILABLE_5, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_5},
        {TransmissionAgiUpStatus::UNAVAILABLE_6, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_6},
        {TransmissionAgiUpStatus::UNAVAILABLE_7, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_7},
        {TransmissionAgiUpStatus::UNAVAILABLE_8, SFCDetail_Contents_NConstantTransmissionAgiUpStat::UNAVAILABLE_8}};

    std::map<TransmissionAgiDownStatus, SFCDetail_Contents_NConstantTransmissionAgiDownStat> mTransmissionAgiDownMap = {
        {TransmissionAgiDownStatus::NONE, SFCDetail_Contents_NConstantTransmissionAgiDownStat::NONE},
        {TransmissionAgiDownStatus::DISPLAY_OFF, SFCDetail_Contents_NConstantTransmissionAgiDownStat::DISPLAY_OFF},
        {TransmissionAgiDownStatus::AVAILABLE_1, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_1},
        {TransmissionAgiDownStatus::AVAILABLE_2, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_2},
        {TransmissionAgiDownStatus::AVAILABLE_3, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_3},
        {TransmissionAgiDownStatus::AVAILABLE_4, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_4},
        {TransmissionAgiDownStatus::AVAILABLE_5, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_5},
        {TransmissionAgiDownStatus::AVAILABLE_6, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_6},
        {TransmissionAgiDownStatus::AVAILABLE_7, SFCDetail_Contents_NConstantTransmissionAgiDownStat::AVAILABLE_7},
        {TransmissionAgiDownStatus::AVAILABLE_8, SFCDetail_Contents_NConstantTransmissionAgiDownStat::DISPLAY_OFF},
        {TransmissionAgiDownStatus::UNAVAILABLE_1, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_1},
        {TransmissionAgiDownStatus::UNAVAILABLE_2, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_2},
        {TransmissionAgiDownStatus::UNAVAILABLE_3, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_3},
        {TransmissionAgiDownStatus::UNAVAILABLE_4, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_4},
        {TransmissionAgiDownStatus::UNAVAILABLE_5, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_5},
        {TransmissionAgiDownStatus::UNAVAILABLE_6, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_6},
        {TransmissionAgiDownStatus::UNAVAILABLE_7, SFCDetail_Contents_NConstantTransmissionAgiDownStat::UNAVAILABLE_7},
        {TransmissionAgiDownStatus::UNAVAILABLE_8, SFCDetail_Contents_NConstantTransmissionAgiDownStat::DISPLAY_OFF}};

    std::map<BoostTimerStatus, SFCDetail_Contents_NConstantBoostTimerStat> mBoostTimerMap = {
        {BoostTimerStatus::NONE, SFCDetail_Contents_NConstantBoostTimerStat::NONE},
        {BoostTimerStatus::BOOST_OFF, SFCDetail_Contents_NConstantBoostTimerStat::BOOST_OFF},
        {BoostTimerStatus::BOOST_READY, SFCDetail_Contents_NConstantBoostTimerStat::BOOST_READY},
        {BoostTimerStatus::BOOST_ON_NOTIMER, SFCDetail_Contents_NConstantBoostTimerStat::BOOST_ON_NOTIMER},
        {BoostTimerStatus::BOOST_ON, SFCDetail_Contents_NConstantBoostTimerStat::BOOST_ON}};
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_N_H
