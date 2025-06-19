/**
 * @file CruiseControl_CV.hpp
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
#ifndef SFSS_CruiseControl_CV_H
#define SFSS_CruiseControl_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "CruiseControl_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.36
// Reference : [CV820] CruiseControl_CV. Contains : Constant, Event. Recommend: VALUE_CHANGED
class CruiseControl_CV : public CruiseControl_CVBase {
public:
    CruiseControl_CV() = default;
    ~CruiseControl_CV() override = default;
    CruiseControl_CV(const CruiseControl_CV& other) = delete;
    CruiseControl_CV(CruiseControl_CV&& other) noexcept = delete;
    CruiseControl_CV& operator=(const CruiseControl_CV& other) = delete;
    CruiseControl_CV& operator=(CruiseControl_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCCruiseControl_CVEventCcSummaryID("E78201");
        if (mOneShotTimerCcSummaryOff.create(kTimerInterval150ms, this, &CruiseControl_CV::onOneShotTimerCcSummaryOff, false,
                                             "OneShotTimerCcSummaryOff") == false) {
            DWarning() << "[CruiseControl_CV] mOneShotTimerCcSummaryOff Create Failed";
        }
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
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        mConfigAdasSCC = args.mInter_ConfigAdasSCC;
        updateAll();
    }

    void onEngineCcActiveChanged(const ArgumentsEngineCcActiveChanged& args) {
        mEngineCcActive = args.mInput_EngineCcActive;
        updateAll();
    }

    void onEngineCcSetSpeedChanged(const ArgumentsEngineCcSetSpeedChanged& args) {
        mEngineCcSetSpeed = args.mInput_EngineCcSetSpeed;
        updateAll();
    }

    void onEngineCcStateChanged(const ArgumentsEngineCcStateChanged& args) {
        mEngineCcState = args.mInput_EngineCcState;
        updateAll();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;
        updateConstantCcSpeedUnitStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateConstantCcColorStat();
        updateConstantCcSpeedStat();
        updateConstantCcSpeedValue();
        updateConstantCcSpeedUnitStat();
    }

    void updateConstantCcColorStat() {
        if (isCruiseControlCVOn() == false) {
            return;
        }

        SFCCruiseControl_CVConstantCcColorStat stat = SFCCruiseControl_CVConstantCcColorStat::DISPLAY_OFF;
        SFCCruiseControl_CVConstantCcColorBlinkValueA blinkValueA = SFCCruiseControl_CVConstantCcColorBlinkValueA::NONE;
        SFCCruiseControl_CVConstantCcColorBlinkValueB blinkValueB = SFCCruiseControl_CVConstantCcColorBlinkValueB::NONE;

        if (mIsIgnOn == true && mEngineCcSetSpeed != 0) {
            if (mEngineCcActive == EngineCcActive::OFF) {
                stat = SFCCruiseControl_CVConstantCcColorStat::CRUISE_PAUSE;
            } else if (mEngineCcActive == EngineCcActive::ON && mEngineCcState == EngineCcState::ACTIVE) {
                stat = SFCCruiseControl_CVConstantCcColorStat::CRUISE_ACTIVE;
            } else if (mEngineCcActive == EngineCcActive::ON && mEngineCcState == EngineCcState::OVERRIDE) {
                stat = SFCCruiseControl_CVConstantCcColorStat::BLINK2;
                blinkValueA = SFCCruiseControl_CVConstantCcColorBlinkValueA::ON;
                blinkValueB = SFCCruiseControl_CVConstantCcColorBlinkValueB::DISPLAY_OFF;
            } else {
                // no operation
            }
        }

        // !DISPLAY_OFF -> DISPLAY_OFF 인 경우 Update 진행하지 않음(Timer Callback에서 OFF처리 진행)
        // DISPLAY_OFF이고 Timer가 동작 중인 경우는 업데이트 진행하지 않음(Timer Callback에서 OFF처리 진행 및 Constant 값 유지)
        // Ign_Off인 경우 Event 조건 상 ColorStat 변경이 아닌 경우로 즉시 OFF 처리 진행
        bool isUpdate = ((isCcColorStatDisplayOn(mPrevCcColorStat) == true && isCcColorStatDisplayOn(stat) == false) == false &&
                         (isCcColorStatDisplayOn(stat) == false && mOneShotTimerCcSummaryOff.isRunning() == true) == false) ||
                        mIsIgnOn == false;

        if (isUpdate == true) {
            setSFCCruiseControl_CVConstantCcColorStat(stat);
            setSFCCruiseControl_CVConstantCcColorBlinkValueA(blinkValueA);
            setSFCCruiseControl_CVConstantCcColorBlinkValueB(blinkValueB);
        }

        mCcColorStat = stat;
        updateEvent();
    }

    void updateConstantCcSpeedStat() {
        if (isCruiseControlCVOn() == false || mOneShotTimerCcSummaryOff.isRunning() == true) {
            return;
        }

        SFCCruiseControl_CVConstantCcSpeedStat stat = SFCCruiseControl_CVConstantCcSpeedStat::SPEED_OFF;
        SFCCruiseControl_CVConstantCcSpeedBlinkValueA blinkValueA = SFCCruiseControl_CVConstantCcSpeedBlinkValueA::NONE;
        SFCCruiseControl_CVConstantCcSpeedBlinkValueB blinkValueB = SFCCruiseControl_CVConstantCcSpeedBlinkValueB::NONE;

        if (mIsIgnOn == true && mEngineCcSetSpeed != 0) {
            if (mEngineCcActive == EngineCcActive::OFF) {
                stat = SFCCruiseControl_CVConstantCcSpeedStat::SPEED_PAUSE;
            } else if (mEngineCcActive == EngineCcActive::ON && mEngineCcState == EngineCcState::ACTIVE) {
                stat = SFCCruiseControl_CVConstantCcSpeedStat::SPEED_ACTIVE;
            } else if (mEngineCcActive == EngineCcActive::ON && mEngineCcState == EngineCcState::OVERRIDE) {
                stat = SFCCruiseControl_CVConstantCcSpeedStat::BLINK2;
                blinkValueA = SFCCruiseControl_CVConstantCcSpeedBlinkValueA::ON;
                blinkValueB = SFCCruiseControl_CVConstantCcSpeedBlinkValueB::SPEED_OFF;
            } else {
                // no operation
            }
        }

        setSFCCruiseControl_CVConstantCcSpeedStat(stat);
        setSFCCruiseControl_CVConstantCcSpeedBlinkValueA(blinkValueA);
        setSFCCruiseControl_CVConstantCcSpeedBlinkValueB(blinkValueB);
        mCcSpeedStat = stat;
    }

    void updateConstantCcSpeedValue() {
        if (isCruiseControlCVOn() == false || mOneShotTimerCcSummaryOff.isRunning() == true) {
            return;
        }

        HUInt64 value = 0;

        if (mIsIgnOn == true && isCcSpeedStatOn() == true) {
            value = mEngineCcSetSpeed + kCcSpeedOffset;
        }

        setSFCCruiseControl_CVConstantCcSpeedValue(value);
    }

    void updateConstantCcSpeedUnitStat() {
        if (isCruiseControlCVOn() == false || mOneShotTimerCcSummaryOff.isRunning() == true) {
            return;
        }

        SFCCruiseControl_CVConstantCcSpeedUnitStat unit = SFCCruiseControl_CVConstantCcSpeedUnitStat::DISPLAY_OFF;

        if (mIsIgnOn == true && isCcSpeedStatOn() == true) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                unit = SFCCruiseControl_CVConstantCcSpeedUnitStat::KMH;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                unit = SFCCruiseControl_CVConstantCcSpeedUnitStat::MPH;
            } else {
                // no operation
            }
        }

        setSFCCruiseControl_CVConstantCcSpeedUnitStat(unit);
    }

    void updateEvent() {
        bool isUpdate = true;
        SFCCruiseControl_CVEventCcSummaryStat stat = SFCCruiseControl_CVEventCcSummaryStat::OFF;

        if (mIsIgnOn == true) {
            if (isCcColorStatDisplayOn(mCcColorStat) == true) {
                stat = SFCCruiseControl_CVEventCcSummaryStat::ON;
                if (mOneShotTimerCcSummaryOff.isRunning() == true) {
                    mOneShotTimerCcSummaryOff.stop();
                }
            } else {
                // !DISPLAY OFF -> DISPLAY_OFF
                if (isCcColorStatDisplayOn(mPrevCcColorStat) == true && isCcColorStatDisplayOn(mCcColorStat) == false) {
                    isUpdate = false;
                    if (mOneShotTimerCcSummaryOff.start() == false) {
                        DWarning() << "[CruiseControl_CV] mOneShotTimerCcSummaryOff Start Failed";
                    }
                }
            }
        } else {
            if (mOneShotTimerCcSummaryOff.isRunning() == true) {
                mOneShotTimerCcSummaryOff.stop();
            }
        }

        if (isUpdate == true) {
            setSFCCruiseControl_CVEventCcSummaryStat(stat);
        }

        mPrevCcColorStat = mCcColorStat;
    }

    void onOneShotTimerCcSummaryOff() {
        setSFCCruiseControl_CVEventCcSummaryStat(SFCCruiseControl_CVEventCcSummaryStat::OFF);
        setSFCCruiseControl_CVConstantCcColorStat(SFCCruiseControl_CVConstantCcColorStat::DISPLAY_OFF);
        mCcSpeedStat = SFCCruiseControl_CVConstantCcSpeedStat::SPEED_OFF;
        setSFCCruiseControl_CVConstantCcSpeedStat(mCcSpeedStat);
        setSFCCruiseControl_CVConstantCcSpeedValue(0);
        setSFCCruiseControl_CVConstantCcSpeedUnitStat(SFCCruiseControl_CVConstantCcSpeedUnitStat::DISPLAY_OFF);
        flush();
    }

    inline bool isCruiseControlCVOn() const {
        return mConfigVehicleType == ConfigVehicleType::ICV && mConfigAdasSCC == ConfigAdasSCC::OFF &&
               mConfigFuelTypeCV == ConfigFuelTypeCV::DSL;
    }

    inline bool isCcSpeedStatOn() const {
        return mCcSpeedStat == SFCCruiseControl_CVConstantCcSpeedStat::SPEED_PAUSE ||
               mCcSpeedStat == SFCCruiseControl_CVConstantCcSpeedStat::SPEED_ACTIVE ||
               mCcSpeedStat == SFCCruiseControl_CVConstantCcSpeedStat::BLINK2;
    }

    inline bool isCcColorStatDisplayOn(SFCCruiseControl_CVConstantCcColorStat& stat) const {
        return stat == SFCCruiseControl_CVConstantCcColorStat::CRUISE_PAUSE ||
               stat == SFCCruiseControl_CVConstantCcColorStat::CRUISE_ACTIVE ||
               stat == SFCCruiseControl_CVConstantCcColorStat::BLINK2;
    }

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using ConfigAdasSCC = ArgumentsConfigChanged::Inter_ConfigAdasSCC;
    using EngineCcActive = ArgumentsEngineCcActiveChanged::Input_EngineCcActive;
    using EngineCcState = ArgumentsEngineCcStateChanged::Input_EngineCcState;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;
    EngineCcActive mEngineCcActive = EngineCcActive::OFF;
    EngineCcState mEngineCcState = EngineCcState::OFF;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;

    SFCCruiseControl_CVConstantCcColorStat mPrevCcColorStat = SFCCruiseControl_CVConstantCcColorStat::DISPLAY_OFF;
    SFCCruiseControl_CVConstantCcColorStat mCcColorStat = SFCCruiseControl_CVConstantCcColorStat::DISPLAY_OFF;
    SFCCruiseControl_CVConstantCcSpeedStat mCcSpeedStat = SFCCruiseControl_CVConstantCcSpeedStat::SPEED_OFF;

    HUInt64 mEngineCcSetSpeed = 0;
    bool mIsIgnOn = false;

    static constexpr HUInt64 kTimerInterval150ms = 150;
    static constexpr HUInt64 kCcSpeedOffset = 2;

    ssfs::SFCTimer<CruiseControl_CV> mOneShotTimerCcSummaryOff;
};

}  // namespace ccos

#endif  // SFSS_CruiseControl_CV_H
