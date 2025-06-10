/**
 * @file Speed_Gauge.hpp
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
#ifndef SFSS_Speed_Gauge_H
#define SFSS_Speed_Gauge_H

#define DLOG_ENABLED gEnableSFCLog

#include <array>
#include <cmath>
#include <Vehicle.h>

#include "Speed_GaugeBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 12.0.0
// Reference : [CD160] Speed Gauge. Contains : Constant. Recommend: VALUE_CHANGED
class Speed_Gauge : public Speed_GaugeBase {
public:
    Speed_Gauge() = default;
    ~Speed_Gauge() override = default;
    Speed_Gauge(const Speed_Gauge& other) = delete;
    Speed_Gauge(Speed_Gauge&& other) noexcept = delete;
    Speed_Gauge& operator=(const Speed_Gauge& other) = delete;
    Speed_Gauge& operator=(Speed_Gauge&& other) noexcept = delete;

    void onInitialize() override {
        if (mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue.create(
                kTimerDigitalDisplayValueInterval200ms, this,
                &Speed_Gauge::onRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue, true) == false) {
            DWarning() << "[Speed_Gauge] mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue Create Failed";
        }
        if (mRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue.create(
                kTimerSubDigitalDisplayInterval200ms, this,
                &Speed_Gauge::onRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue, true) == false) {
            DWarning() << "[Speed_Gauge] mRepeatTimerTimeoutToSetVehicleSubSpeedDigitalDisplayValue Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::ON_0ms;

        updatePrivateCurrentSpeedSectionKph();
        updatePrivateCurrentSpeedSectionMph();

        updatePrivateCurrToleranceKph();
        updatePrivateCurrToleranceMph();

        updateVehicleSpeedSubDisplayStat();

        updateOutputDisplaySpeedUnit();
        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedDecimalValue();
        updateOutputDisplaySpeedKphValue();
        updateOutputActualVehicleSpeedValue();

        updateVehicleSpeedAnalogDisplayValue();
        updateVehicleSpeedDigitalDisplayValue();
        updateVehicleSpeedSubDigitalDisplayValue();
        updatePrivateNaviOverSpeedInputValid();
        updateVehicleSpeedScaleMaximumStat();

        updateNaviSpeedLimitStat();
        updateNaviSpeedLimitOver1ColorValue();
        updateNaviSpeedLimitOver2ColorValue();

        updateTimerToSetVehicleSpeedDigitalDisplayValue();
        updateTimerToSetVehicleSpeedSubDigitalDisplayValue();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::OFF_0ms;
        updatePrivateCurrentSpeedSectionKph();
        updatePrivateCurrentSpeedSectionMph();

        updatePrivateCurrToleranceKph();
        updatePrivateCurrToleranceMph();

        updateVehicleSpeedSubDisplayStat();

        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedDecimalValue();
        updateOutputDisplaySpeedKphValue();

        updateVehicleSpeedAnalogDisplayValue();
        updateVehicleSpeedDigitalDisplayValue();
        updateVehicleSpeedSubDigitalDisplayValue();
        updatePrivateNaviOverSpeedInputValid();

        updateNaviSpeedLimitStat();
        updateNaviSpeedLimitOver1ColorValue();
        updateNaviSpeedLimitOver2ColorValue();

        updateTimerToSetVehicleSpeedDigitalDisplayValue();
        updateTimerToSetVehicleSpeedSubDigitalDisplayValue();
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void onActualVehicleSpeedChanged(const ArgumentsActualVehicleSpeedChanged& args) {
        mActualVehicleSpeed = args.mInter_ActualVehicleSpeed * kFactorActualVehicleSpeed;
        updatePrivateCurrentSpeedSectionKph();
        updatePrivateCurrentSpeedSectionMph();
        updatePrivateCurrToleranceKph();
        updatePrivateCurrToleranceMph();
        updateVehicleSpeedAnalogDisplayValue();
        updateOutputActualVehicleSpeedValue();
    }

    void onActualVehicleSpeedValidityChanged(const ArgumentsActualVehicleSpeedValidityChanged& args) {
        mActualVehicleSpeedValidity = args.mInter_ActualVehicleSpeedValidity;
        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedDecimalValue();
        updateOutputDisplaySpeedKphValue();
        updateOutputActualVehicleSpeedValue();
    }

    void onAreaChanged(const ArgumentsAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateVehicleSpeedSubDisplayStat();
    }

    void onDisplaySpeedUnitChanged(const ArgumentsDisplaySpeedUnitChanged& args) {
        updateValueIfValid(mConfigMainSpeedUnitChangeable, args.mInter_ConfigMainSpeedUnitChangeable);
        updateValueIfValid(mConfigSpeedometerType, args.mInter_ConfigSpeedometerType);
        updateValueIfValid(mTripDistanceUnit, args.mInter_TripDistanceUnit);

        updatePrivateDisplaySpeedUnit();
        updateVehicleSpeedAnalogDisplayValue();
        updateVehicleSpeedDigitalDisplayValue();
        updateVehicleSpeedSubDigitalDisplayValue();

        updateVehicleSpeedAuxDisplayUnitStat();
        updateVehicleSpeedSubDisplayStat();

        updateOutputDisplaySpeedDefaultUnit();
    }

    void onNaviOverSpdUnitChanged(const ArgumentsNaviOverSpdUnitChanged& args) {
        updateValueIfValid(mNaviOverSpdUnit, args.mInput_NaviOverSpdUnit);

        updateNaviSpeedLimitStat();
    }

    void onNaviOverSpeedInputValidChanged(const ArgumentsNaviOverSpeedInputValidChanged& args) {
        updateValueIfValid(mNaviOverSpeedColor1, args.mInput_NaviOverSpeedColor1);
        updateValueIfValid(mNaviOverSpeedColor2, args.mInput_NaviOverSpeedColor2);
        updateValueIfValid(mNaviOverSpeedRange1, args.mInput_NaviOverSpeedRange1);
        updateValueIfValid(mNaviOverSpeedRange2, args.mInput_NaviOverSpeedRange2);

        updatePrivateNaviOverSpeedInputValid();
        updateNaviSpeedLimitStat();
        updateNaviSpeedLimitOver1ColorValue();
        updateNaviSpeedLimitOver2ColorValue();
    }

    void onAnalogSpeedMaxChanged(const ArgumentsAnalogSpeedMaxChanged& args) {
        updateValueIfValid(mConfigAnalogSpeedMax, args.mInter_ConfigAnalogSpeedMax);
        updateVehicleSpeedScaleMaximumStat();
    }

    void onMaxDisplaySpeedKphChanged(const ArgumentsMaxDisplaySpeedKphChanged& args) {
        mParMaxDisplaySpeedKph = args.mPar_MaxDisplaySpeedKph;
        updatePrivateCurrentSpeedSectionKph();
        updatePrivateCurrToleranceKph();
        updateVehicleSpeedAnalogDisplayValue();
        updateVehicleSpeedDigitalDisplayValue();
        updateVehicleSpeedSubDigitalDisplayValue();

        updateNaviSpeedLimitStat();
    }

    void onMaxDisplaySpeedMphChanged(const ArgumentsMaxDisplaySpeedMphChanged& args) {
        mParMaxDisplaySpeedMph = args.mPar_MaxDisplaySpeedMph;
        updatePrivateCurrentSpeedSectionMph();
        updatePrivateCurrToleranceMph();
        updateVehicleSpeedAnalogDisplayValue();
        updateVehicleSpeedDigitalDisplayValue();
        updateVehicleSpeedSubDigitalDisplayValue();
    }

    void onSpeedGaugeParamChanged(const ArgumentsSpeedGaugeParamChanged& args) {
        mParSpeedToleranceKphArray = {0,
                                      args.mPar_SpeedToleranceKph1,
                                      args.mPar_SpeedToleranceKph2,
                                      args.mPar_SpeedToleranceKph3,
                                      args.mPar_SpeedToleranceKph4,
                                      args.mPar_SpeedToleranceKph5,
                                      args.mPar_SpeedToleranceKph6,
                                      args.mPar_SpeedToleranceKph7,
                                      args.mPar_SpeedToleranceKph8,
                                      args.mPar_SpeedToleranceKph9,
                                      args.mPar_SpeedToleranceKph10,
                                      args.mPar_SpeedToleranceKph11,
                                      args.mPar_SpeedToleranceKph12,
                                      args.mPar_SpeedToleranceKph13,
                                      args.mPar_SpeedToleranceKph14,
                                      args.mPar_SpeedToleranceKph15};
        mParSpeedToleranceMphArray = {0,
                                      args.mPar_SpeedToleranceMph1,
                                      args.mPar_SpeedToleranceMph2,
                                      args.mPar_SpeedToleranceMph3,
                                      args.mPar_SpeedToleranceMph4,
                                      args.mPar_SpeedToleranceMph5,
                                      args.mPar_SpeedToleranceMph6,
                                      args.mPar_SpeedToleranceMph7,
                                      args.mPar_SpeedToleranceMph8,
                                      args.mPar_SpeedToleranceMph9,
                                      args.mPar_SpeedToleranceMph10};
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            if (mAEMInitializeComplete == true) {
                updatePrivateCurrentSpeedSectionKph();
                updatePrivateCurrToleranceKph();

                updatePrivateCurrentSpeedSectionMph();
                updatePrivateCurrToleranceMph();

                updateVehicleSpeedAnalogDisplayValue();
                updateVehicleSpeedDigitalDisplayValue();
                updateVehicleSpeedSubDigitalDisplayValue();

                updateNaviSpeedLimitStat();
            }
        }
    }

    void dump(std::ostringstream& out, const std::string& prefix, const std::string& command) override {
        DDebug() << "mActualVehicleSpeed : " << mActualVehicleSpeed;
        DDebug() << "mPrivateCurrentSpeedSectionKph : " << mPrivateCurrentSpeedSectionKph;
        DDebug() << "mPrivateToleranceKph1 : " << mPrivateToleranceKph1;
        DDebug() << "mPrivateToleranceKph2 : " << mPrivateToleranceKph2;
        DDebug() << "mPrivate_CurrToleranceKph : " << mPrivate_CurrToleranceKph;
        DDebug() << "mDisplaySpeedValueKPH : " << mDisplaySpeedValueKPH;
        DDebug() << "mParMaxDisplaySpeedKph : " << mParMaxDisplaySpeedKph;
        DDebug() << "mParMaxDisplaySpeedMph : " << mParMaxDisplaySpeedMph;
        for (size_t i = 0; i < mParSpeedToleranceKphArray.size(); i++) {
            DDebug() << "mParSpeedToleranceKphArray[" << i << "] : " << mParSpeedToleranceKphArray[i];
        }
        for (size_t i = 0; i < mParSpeedToleranceMphArray.size(); i++) {
            DDebug() << "mParSpeedToleranceMphArray[" << i << "] : " << mParSpeedToleranceMphArray[i];
        }
    }

private:
    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateCurrentSpeedSectionKph() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mActualVehicleSpeed < mParMaxDisplaySpeedKph) {
                mPrivateCurrentSpeedSectionKph = static_cast<HUInt64>(mActualVehicleSpeed) / 20;
            } else {
                mPrivateCurrentSpeedSectionKph = static_cast<HUInt64>(mParMaxDisplaySpeedKph) / 20;
            }
        }
        updatePrivateToleranceKph1();
        updatePrivateToleranceKph2();
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateCurrentSpeedSectionMph() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mActualVehicleSpeed * mFactor_ConvKmToMi < mParMaxDisplaySpeedMph) {
                HDouble ActualVehicleSpeed_Mph = mActualVehicleSpeed * mFactor_ConvKmToMi;
                mPrivateCurrentSpeedSectionMph = static_cast<HUInt64>(ActualVehicleSpeed_Mph) / 20;
            } else {
                mPrivateCurrentSpeedSectionMph = static_cast<HUInt64>(mParMaxDisplaySpeedMph) / 20;
            }
        }
        updatePrivateToleranceMph1();
        updatePrivateToleranceMph2();
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateToleranceKph1() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        if (mPrivateCurrentSpeedSectionKph > 0 && mPrivateCurrentSpeedSectionKph < mParSpeedToleranceKphArray.size()) {
            mPrivateToleranceKph1 = mParSpeedToleranceKphArray[mPrivateCurrentSpeedSectionKph];
        } else {
            if (mPrivateCurrentSpeedSectionKph < 1) {
                mPrivateToleranceKph1 = 0;
            } else {
                DError() << "Private_CurrentSpeedSectionKph index [ERROR] - Value is " << mPrivateCurrentSpeedSectionKph;
            }
        }
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateToleranceKph2() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        HUInt64 speedSectionValue = 0;
        if (mPrivateCurrentSpeedSectionKph < (static_cast<HUInt64>(mParMaxDisplaySpeedKph) / 20)) {
            speedSectionValue = mPrivateCurrentSpeedSectionKph + 1;
        } else {
            speedSectionValue = mPrivateCurrentSpeedSectionKph;
        }
        if (speedSectionValue > 0 && speedSectionValue < mParSpeedToleranceKphArray.size()) {
            mPrivateToleranceKph2 = mParSpeedToleranceKphArray[speedSectionValue];
        } else {
            mPrivateToleranceKph2 = 0;
        }
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateToleranceMph1() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        if (mPrivateCurrentSpeedSectionMph > 0 && mPrivateCurrentSpeedSectionMph < mParSpeedToleranceMphArray.size()) {
            mPrivateToleranceMph1 = mParSpeedToleranceMphArray[mPrivateCurrentSpeedSectionMph];
        } else {
            if (mPrivateCurrentSpeedSectionMph == 0) {
                mPrivateToleranceMph1 = 0;
            } else {
                DError() << "Private_ToleranceMph1 index [ERROR] - Value is " << mPrivateCurrentSpeedSectionMph;
            }
        }
    }

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기
    void updatePrivateToleranceMph2() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        HUInt64 speedSectionValue = 0;
        if (mPrivateCurrentSpeedSectionMph < (static_cast<HUInt64>(mParMaxDisplaySpeedMph) / 20)) {
            speedSectionValue = mPrivateCurrentSpeedSectionMph + 1;
        } else {
            speedSectionValue = mPrivateCurrentSpeedSectionMph;
        }
        if (speedSectionValue > 0 && speedSectionValue < mParSpeedToleranceMphArray.size()) {
            mPrivateToleranceMph2 = mParSpeedToleranceMphArray[speedSectionValue];
        } else {
            mPrivateToleranceMph2 = 0;
        }
    }

    // 5.15.4.1.1.2 현재 속도 공차 계산
    void updatePrivateCurrToleranceKph() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mActualVehicleSpeed <= mParMaxDisplaySpeedKph) {
                mPrivate_CurrToleranceKph =
                    (mPrivateToleranceKph2 - mPrivateToleranceKph1) / 20.0 *
                        (mActualVehicleSpeed - (static_cast<HDouble>(mPrivateCurrentSpeedSectionKph) * 20.0)) +
                    mPrivateToleranceKph1;
            } else {
                mPrivate_CurrToleranceKph =
                    (mPrivateToleranceKph2 - mPrivateToleranceKph1) / 20.0 *
                        (mParMaxDisplaySpeedKph - (static_cast<HDouble>(mPrivateCurrentSpeedSectionKph) * 20.0)) +
                    mPrivateToleranceKph1;
            }
        }
        updateDisplaySpeedValueKPH();
    }

    // 5.15.4.1.1.2 현재 속도 공차 계산
    void updatePrivateCurrToleranceMph() {
        if (mAEMInitializeComplete == false) {
            return;
        }
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mActualVehicleSpeed * mFactor_ConvKmToMi <= mParMaxDisplaySpeedMph) {
                mPrivate_CurrToleranceMph = (mPrivateToleranceMph2 - mPrivateToleranceMph1) / 20.0 *
                                                (mActualVehicleSpeed * mFactor_ConvKmToMi -
                                                 (static_cast<HDouble>(mPrivateCurrentSpeedSectionMph) * 20.0)) +
                                            mPrivateToleranceMph1;
            } else {
                mPrivate_CurrToleranceMph = (mPrivateToleranceMph2 - mPrivateToleranceMph1) / 20.0 *
                                                (mParMaxDisplaySpeedMph * mFactor_ConvKmToMi -
                                                 (static_cast<HDouble>(mPrivateCurrentSpeedSectionMph) * 20.0)) +
                                            mPrivateToleranceMph1;
            }
        }
        updateDisplaySpeedValueMPH();
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updateDisplaySpeedValueKPH() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            mDisplaySpeedValueKPH = mActualVehicleSpeed + mPrivate_CurrToleranceKph;
        } else {
            mDisplaySpeedValueKPH = 0.0l;
        }
        setSFCSpeed_GaugeInter_DisplaySpeedValueKPH(mDisplaySpeedValueKPH);
        updateSFCPrivateDisplaySpeedValueKPHRange();

        updatePrivateDisplaySpeedIntegerKph();
        updatePrivateDisplaySpeedDecimalKph();
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updatePrivateDisplaySpeedIntegerKph() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        mPrivateDisplaySpeedIntegerKph = static_cast<HUInt64>(mDisplaySpeedValueKPH);
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updatePrivateDisplaySpeedDecimalKph() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        HDouble kphGap = mDisplaySpeedValueKPH - static_cast<HDouble>(mPrivateDisplaySpeedIntegerKph);
        HDouble calcValue = floor(kphGap * 1000) / 1000.0;
        HDouble outDisplaySpeed = 0.0l;
        if (updateRoundDown(calcValue, kFactorDisSpdDcmVal, &outDisplaySpeed) == true) {
            mPrivateDisplaySpeedDecimalKph = outDisplaySpeed;
        }

        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedKphValue();
        updateOutputDisplaySpeedDecimalValue();
    }

    bool updateRoundDown(const HDouble& myValue, const HDouble& delim, HDouble* outValue) {
        bool res = true;
        if (almostEqual(delim, 0.0) == true) {
            res = false;
            DError() << "delim is zero [ERROR]";
            return res;
        }

        HDouble factor_double = 1.0 / delim;
        HUInt64 factor = static_cast<HUInt64>(factor_double);
        HUInt64 exponent = static_cast<HUInt64>(myValue);
        HDouble mantissa = myValue - static_cast<HDouble>(exponent);

        HDouble myIndex_double = mantissa * static_cast<HDouble>(factor);
        HUInt64 myIndex = static_cast<HUInt64>(myIndex_double);

        *outValue = static_cast<HDouble>(myIndex) * delim + static_cast<HDouble>(exponent);

        return res;
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updateDisplaySpeedValueMPH() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            mDisplaySpeedValueMPH = mActualVehicleSpeed * mFactor_ConvKmToMi + mPrivate_CurrToleranceMph;
        } else {
            mDisplaySpeedValueMPH = 0.0l;
        }
        setSFCSpeed_GaugeInter_DisplaySpeedValueMPH(mDisplaySpeedValueMPH);

        updatePrivateDisplaySpeedIntegerMph();
        updatePrivateDisplaySpeedDecimalMph();
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updatePrivateDisplaySpeedIntegerMph() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        mPrivateDisplaySpeedIntegerMph = static_cast<HUInt64>(mDisplaySpeedValueMPH);
    }

    // 5.15.4.1.2 최종 표시 속도 연산
    void updatePrivateDisplaySpeedDecimalMph() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        HDouble mphGap = mDisplaySpeedValueMPH - static_cast<HDouble>(mPrivateDisplaySpeedIntegerMph);
        HDouble calcValue = floor(mphGap * 1000.0) / 1000.0;
        HDouble outDisplaySpeed = 0.0l;
        if (updateRoundDown(calcValue, kFactorDisSpdDcmVal, &outDisplaySpeed) == true) {
            mPrivateDisplaySpeedDecimalMph = outDisplaySpeed;
        }
        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedKphValue();
        updateOutputDisplaySpeedDecimalValue();
    }

    // 5.15.4.2 표시 단위 판단
    void updatePrivateDisplaySpeedUnit() {
        if (mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::FIXED_UNIT &&
            (mConfigSpeedometerType == ConfigSpeedometerType::KPH || mConfigSpeedometerType == ConfigSpeedometerType::KMPH)) {
            mPrivateDisplaySpeedUnit = PrivateDisplaySpeedUnit::KM_PER_HOUR;
            setSFCSpeed_GaugeInter_DisplaySpeedUnit(SFCSpeed_GaugeInter_DisplaySpeedUnit::KM_PER_HOUR);
        } else if (mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::CHANGEABLE &&
                   mTripDistanceUnit == TripDistanceUnit::KM) {
            mPrivateDisplaySpeedUnit = PrivateDisplaySpeedUnit::KM_PER_HOUR;
            setSFCSpeed_GaugeInter_DisplaySpeedUnit(SFCSpeed_GaugeInter_DisplaySpeedUnit::KM_PER_HOUR);
        } else if (mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::FIXED_UNIT &&
                   mConfigSpeedometerType == ConfigSpeedometerType::MKPH) {
            mPrivateDisplaySpeedUnit = PrivateDisplaySpeedUnit::MILE_PER_HOUR;
            setSFCSpeed_GaugeInter_DisplaySpeedUnit(SFCSpeed_GaugeInter_DisplaySpeedUnit::MILE_PER_HOUR);
        } else if (mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::CHANGEABLE &&
                   mTripDistanceUnit == TripDistanceUnit::MILE) {
            mPrivateDisplaySpeedUnit = PrivateDisplaySpeedUnit::MILE_PER_HOUR;
            setSFCSpeed_GaugeInter_DisplaySpeedUnit(SFCSpeed_GaugeInter_DisplaySpeedUnit::MILE_PER_HOUR);
        } else {
            // No output
        }

        updateOutputDisplaySpeedUnit();
        updateOutputDisplaySpeedValue();
        updateOutputDisplaySpeedKphValue();
        updateOutputDisplaySpeedDecimalValue();

        updateVehicleSpeedMainDisplayUnitStat();
    }

    // 5.15.4.3 현재값 외부 출력
    void updateOutputDisplaySpeedValue() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue, static_cast<HUInt64>(0));
            flushActuatorOutput();
            return;
        }
        if (mActualVehicleSpeedValidity == ActualVehicleSpeedValidity::INVALID) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue,
                                static_cast<HUInt64>(kOutput_DisplaySpeedValueErr));
            flushActuatorOutput();
        } else if (mActualVehicleSpeedValidity == ActualVehicleSpeedValidity::VALID) {
            if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::KM_PER_HOUR) {
                if (mPrivateDisplaySpeedDecimalKph >= 0.5) {
                    // TODO(LDG): Output_DisplaySpeedValue
                    // 현재 Factor 와 Offset 처리를 해당 파일에서 하는데 추후 변경이 있을 수 있음
                    HDouble kph = static_cast<HDouble>(mPrivateDisplaySpeedIntegerKph) + 0.5;
                    kph = kph / kFactorCluDisSpdVal;
                    updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue, static_cast<HUInt64>(kph));
                    flushActuatorOutput();
                } else {
                    HDouble kph = static_cast<HDouble>(mPrivateDisplaySpeedIntegerKph);
                    kph = kph / kFactorCluDisSpdVal;
                    updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue, static_cast<HUInt64>(kph));
                    flushActuatorOutput();
                }
            } else if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR) {
                if (mPrivateDisplaySpeedDecimalMph >= 0.5) {
                    // TODO(LDG): Output_DisplaySpeedValue
                    // 현재 Factor 와 Offset 처리를 해당 파일에서 하는데 추후 변경이 있을 수 있음
                    HDouble mph = static_cast<HDouble>(mPrivateDisplaySpeedIntegerMph) + 0.5;
                    mph = mph / kFactorCluDisSpdVal;
                    updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue, static_cast<HUInt64>(mph));
                    flushActuatorOutput();
                } else {
                    HDouble mph = static_cast<HDouble>(mPrivateDisplaySpeedIntegerMph);
                    mph = mph / kFactorCluDisSpdVal;
                    updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedValue, static_cast<HUInt64>(mph));
                    flushActuatorOutput();
                }
            } else {
                // if (mPrivateDisplaySpeedUnit != PrivateDisplaySpeedUnit::{KM_PER_HOUR|MILE_PER_HOUR}
            }
        } else {
            // if (mActualVehicleSpeedValidity != ActualVehicleSpeedValidity::{INVALID|VALID}
        }
    }

    // 5.15.4.3 현재값 외부 출력
    void updateOutputDisplaySpeedDecimalValue() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue, static_cast<HUInt64>(0));
            flushActuatorOutput();
            return;
        }

        if (mActualVehicleSpeedValidity != ActualVehicleSpeedValidity::VALID) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue, static_cast<HUInt64>(0));
            flushActuatorOutput();
            return;
        }

        if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::KM_PER_HOUR) {
            if (mPrivateDisplaySpeedDecimalKph >= 0.5) {
                // 현재 Factor 와 Offset 처리를 해당 파일에서 하는데 추후 변경이 있을 수 있음
                HDouble DisplaySpeedUnderDecimalValue_temp = (mPrivateDisplaySpeedDecimalKph - 0.5) / kFactorDisSpdDcmVal;
                updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue,
                                    static_cast<HUInt64>(DisplaySpeedUnderDecimalValue_temp));
                flushActuatorOutput();
            } else {
                HDouble DisplaySpeedUnderDecimalValue_temp = mPrivateDisplaySpeedDecimalKph / kFactorDisSpdDcmVal;
                updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue,
                                    static_cast<HUInt64>(DisplaySpeedUnderDecimalValue_temp));
                flushActuatorOutput();
            }
        } else if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR) {
            if (mPrivateDisplaySpeedDecimalMph >= 0.5) {
                // 현재 Factor 와 Offset 처리를 해당 파일에서 하는데 추후 변경이 있을 수 있음
                HDouble DisplaySpeedUnderDecimalValue_temp = (mPrivateDisplaySpeedDecimalMph - 0.5) / kFactorDisSpdDcmVal;
                updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue,
                                    static_cast<HUInt64>(DisplaySpeedUnderDecimalValue_temp));
                flushActuatorOutput();
            } else {
                HDouble DisplaySpeedUnderDecimalValue_temp = mPrivateDisplaySpeedDecimalMph / kFactorDisSpdDcmVal;
                updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnderDecimalValue,
                                    static_cast<HUInt64>(DisplaySpeedUnderDecimalValue_temp));
                flushActuatorOutput();
            }
        } else {
            // if (mPrivateDisplaySpeedUnit != PrivateDisplaySpeedUnit::{KM_PER_HOUR|MILE_PER_HOUR}
        }
    }

    // 5.15.4.3 현재값 외부 출력
    void updateOutputDisplaySpeedKphValue() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedKphValue, static_cast<HUInt64>(0));
            flushActuatorOutput();
            return;
        }
        if (mActualVehicleSpeedValidity == ActualVehicleSpeedValidity::INVALID) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedKphValue, kOutputDisplaySpeedKphValueErr);
            flushActuatorOutput();
        } else if (mActualVehicleSpeedValidity == ActualVehicleSpeedValidity::VALID) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedKphValue,
                                static_cast<HUInt64>(llround(mDisplaySpeedValueKPH)));
            flushActuatorOutput();
        } else {
            // if (mActualVehicleSpeedValidity != ActualVehicleSpeedValidity::{INVALID|VALID}
        }
    }

    // 5.15.4.3 현재값 외부 출력
    void updateOutputActualVehicleSpeedValue() {
        HDouble outDisplaySpeed = 0.0l;
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (updateRoundDown(mActualVehicleSpeed, kFactorDisSpdDcmVal, &outDisplaySpeed) == true) {
                updateActuatorValue(Vehicle.System.HardWire.Output_ActualVehicleSpeedValue,
                                    static_cast<HUInt64>(outDisplaySpeed));
                flushActuatorOutput();
            }
        } else {
            // outDisplaySpeed == 0.0 when IGN1 is not on
            updateActuatorValue(Vehicle.System.HardWire.Output_ActualVehicleSpeedValue, static_cast<HUInt64>(outDisplaySpeed));
            flushActuatorOutput();
        }
    }

    void updateOutputDisplaySpeedUnit() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnit, kOutput_DisplaySpeedUnitMPH);
        } else {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedUnit, kOutput_DisplaySpeedUnitKPH);
        }
        flushActuatorOutput();
    }

    void updateOutputDisplaySpeedDefaultUnit() {
        if (mConfigSpeedometerType == ConfigSpeedometerType::KPH) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedDefaultUnit, kOutput_DisplaySpeedDefaultUnitKPH);
        } else if (mConfigSpeedometerType == ConfigSpeedometerType::KMPH) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedDefaultUnit, kOutput_DisplaySpeedDefaultUnitKMPH);
        } else if (mConfigSpeedometerType == ConfigSpeedometerType::MKPH) {
            updateActuatorValue(Vehicle.CD.Speed_Gauge.Output_DisplaySpeedDefaultUnit, kOutput_DisplaySpeedDefaultUnitMKPH);
        } else {
            // if (mConfigSpeedometerType != ConfigSpeedometerType::{KPH|KMPH|MKPH})
        }
        flushActuatorOutput();
    }

    // 5.15.4.4 내비게이션 과속 표시 신호 정보 유효성 판단
    void updatePrivateNaviOverSpeedInputValid() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms && mNaviOverSpeedRange1 > kNaviOverSpeedRangeMin &&
            mNaviOverSpeedRange1 < kNaviOverSpeedRangeMax && mNaviOverSpeedRange2 > kNaviOverSpeedRangeMin &&
            mNaviOverSpeedRange2 < kNaviOverSpeedRangeMax && mNaviOverSpeedColor1 > kNaviOverSpeedColorMin &&
            mNaviOverSpeedColor1 < kNaviOverSpeedColorMax && mNaviOverSpeedColor2 > kNaviOverSpeedColorMin &&
            mNaviOverSpeedColor2 < kNaviOverSpeedColorMax) {
            mPrivateNaviOverSpeedInputValid = PrivateNaviOverSpeedInputValid::VALID;
        } else {
            mPrivateNaviOverSpeedInputValid = PrivateNaviOverSpeedInputValid::INVALID;
        }
    }

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    // 5.15.5.2.1.1 아날로그 타입
    void updateVehicleSpeedAnalogDisplayValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            setSFCSpeed_GaugeConstantSpeedAnalogStat(SFCSpeed_GaugeConstantSpeedAnalogStat::VALUE);
            if (mTripDistanceUnit == TripDistanceUnit::KM && mDisplaySpeedValueKPH >= 2.0) {
                setSFCSpeed_GaugeConstantSpeedAnalogValue(mDisplaySpeedValueKPH);
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE && mDisplaySpeedValueKPH >= 2.0) {
                setSFCSpeed_GaugeConstantSpeedAnalogValue(mDisplaySpeedValueMPH);
            } else {
                setSFCSpeed_GaugeConstantSpeedAnalogValue(0.0l);
            }
        } else {
            setSFCSpeed_GaugeConstantSpeedAnalogStat(SFCSpeed_GaugeConstantSpeedAnalogStat::DISPLAY_OFF);
        }
    }

    // 5.15.5.2.1.2 디지털 타입
    void updateVehicleSpeedDigitalDisplayValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            setSFCSpeed_GaugeConstantSpeedDigitalStat(SFCSpeed_GaugeConstantSpeedDigitalStat::VALUE);
            if (mTripDistanceUnit == TripDistanceUnit::KM && mDisplaySpeedValueKPH >= 2.0) {
                mDigitalDisplaySpeedValueKPH = static_cast<HUInt64>(llround(mDisplaySpeedValueKPH));
                setSFCSpeed_GaugeConstantSpeedDigitalValue(mDigitalDisplaySpeedValueKPH);
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE && mDisplaySpeedValueKPH >= 2.0) {
                mDigitalDisplaySpeedValueMPH = static_cast<HUInt64>(llround(mDisplaySpeedValueMPH));
                setSFCSpeed_GaugeConstantSpeedDigitalValue(mDigitalDisplaySpeedValueMPH);
            } else {
                mDigitalDisplaySpeedValueKPH = 0;
                mDigitalDisplaySpeedValueMPH = 0;
                setSFCSpeed_GaugeConstantSpeedDigitalValue(0LL);
            }
        } else {
            setSFCSpeed_GaugeConstantSpeedDigitalStat(SFCSpeed_GaugeConstantSpeedDigitalStat::DISPLAY_OFF);
        }
    }

    // 5.15.5.2.1.3 디지털 타입 - 보조 속도계
    void updateVehicleSpeedSubDigitalDisplayValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            setSFCSpeed_GaugeConstantSpeedSubDigitalStat(SFCSpeed_GaugeConstantSpeedSubDigitalStat::VALUE);
            if (mTripDistanceUnit == TripDistanceUnit::KM && mDisplaySpeedValueKPH >= 2.0) {
                setSFCSpeed_GaugeConstantSpeedSubDigitalValue(lround(mDisplaySpeedValueMPH));
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE && mDisplaySpeedValueKPH >= 2.0) {
                setSFCSpeed_GaugeConstantSpeedSubDigitalValue(lround(mDisplaySpeedValueKPH));
            } else {
                setSFCSpeed_GaugeConstantSpeedSubDigitalValue(0.0l);
            }
        } else {
            setSFCSpeed_GaugeConstantSpeedSubDigitalStat(SFCSpeed_GaugeConstantSpeedSubDigitalStat::DISPLAY_OFF);
        }
    }

    void onRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue() {
        updateVehicleSpeedDigitalDisplayValue();
        updateNaviSpeedLimitStat();
        flush();
    }

    void onRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue() {
        updateVehicleSpeedSubDigitalDisplayValue();
        flush();
    }

    void updateTimerToSetVehicleSpeedDigitalDisplayValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue.start() == false) {
                DWarning() << "[Speed_Gauge] mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue add Timer Failed";
            }
        } else {
            mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue.stop();
        }
    }

    void updateTimerToSetVehicleSpeedSubDigitalDisplayValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue.start() == false) {
                DWarning() << "[Speed_Gauge] mRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue add Timer Failed";
            }
        } else {
            mRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue.stop();
        }
    }

    // 5.15.5.2.2.1 메인 속도계 단위
    void updateVehicleSpeedMainDisplayUnitStat() {
        if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::KM_PER_HOUR) {
            setSFCSpeed_GaugeConstantSpeedMainDisplayUnitStat(SFCSpeed_GaugeConstantSpeedMainDisplayUnitStat::KM_PER_HOUR);
        } else if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR) {
            setSFCSpeed_GaugeConstantSpeedMainDisplayUnitStat(SFCSpeed_GaugeConstantSpeedMainDisplayUnitStat::MILE_PER_HOUR);
        } else {
            // if (mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::{KM_PER_HOUR|MILE_PER_HOUR})
        }
    }

    // 5.15.5.2.2.2 참조 속도계 단위
    void updateVehicleSpeedAuxDisplayUnitStat() {
        if (mTripDistanceUnit == TripDistanceUnit::KM) {
            setSFCSpeed_GaugeConstantSpeedAuxDisplayUnitStat(SFCSpeed_GaugeConstantSpeedAuxDisplayUnitStat::KM_PER_HOUR);
        } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
            setSFCSpeed_GaugeConstantSpeedAuxDisplayUnitStat(SFCSpeed_GaugeConstantSpeedAuxDisplayUnitStat::MILE_PER_HOUR);
        } else {
            // if (mTripDistanceUnit == TripDistanceUnit::{KM|MILE})
        }
    }

    // 5.15.5.2.3 보조 속도계 표시 판단
    void updateVehicleSpeedSubDisplayStat() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms &&
            mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::CHANGEABLE && mConfigArea == ConfigArea::USA &&
            mTripDistanceUnit == TripDistanceUnit::KM) {
            setSFCSpeed_GaugeConstantSpeedSubDisplayStat(SFCSpeed_GaugeConstantSpeedSubDisplayStat::ON);
        } else if (mIgnElapsedStatus == IgnElapsed::ON_0ms &&
                   mConfigMainSpeedUnitChangeable == ConfigMainSpeedUnitChangeable::CHANGEABLE &&
                   mConfigArea == ConfigArea::CAN && mTripDistanceUnit == TripDistanceUnit::MILE) {
            setSFCSpeed_GaugeConstantSpeedSubDisplayStat(SFCSpeed_GaugeConstantSpeedSubDisplayStat::ON);
        } else {
            setSFCSpeed_GaugeConstantSpeedSubDisplayStat(SFCSpeed_GaugeConstantSpeedSubDisplayStat::OFF);
        }
    }

    // 5.15.5.2.4 아날로그 속도계 눈금 최고속도
    void updateVehicleSpeedScaleMaximumStat() {
        if (mIgnElapsedStatus != IgnElapsed::ON_0ms) {
            return;
        }
        if (mConfigAnalogSpeedMax == ConfigAnalogSpeedMax::KPH160) {
            setSFCSpeed_GaugeConstantSpeedScaleMaximumStat(SFCSpeed_GaugeConstantSpeedScaleMaximumStat::KPH160);
        } else if (mConfigAnalogSpeedMax == ConfigAnalogSpeedMax::KPH220) {
            setSFCSpeed_GaugeConstantSpeedScaleMaximumStat(SFCSpeed_GaugeConstantSpeedScaleMaximumStat::KPH220);
        } else if (mConfigAnalogSpeedMax == ConfigAnalogSpeedMax::KPH260) {
            setSFCSpeed_GaugeConstantSpeedScaleMaximumStat(SFCSpeed_GaugeConstantSpeedScaleMaximumStat::KPH260);
        } else if (mConfigAnalogSpeedMax == ConfigAnalogSpeedMax::KPH300) {
            setSFCSpeed_GaugeConstantSpeedScaleMaximumStat(SFCSpeed_GaugeConstantSpeedScaleMaximumStat::KPH300);
        } else {
            // if (mConfigAnalogSpeedMax == ConfigAnalogSpeedMax::{KPH160|KPH220|KPH260|KPH300})
        }
    }

    // 5.15.5.2.5 디지털 속도계 속도별 색상 차별화
    void updateNaviSpeedLimitStat() {
        SFCSpeed_GaugeConstantNaviSpeedLimitStat stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::DEFAULT;
        HUInt64 sfcValue = 0;

        getInternalCachedValue("SFC.Speed_Gauge.Constant.SpeedDigital.Value", sfcValue);

        if (mIgnElapsedStatus == IgnElapsed::ON_0ms && mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::KM_PER_HOUR &&
            mNaviOverSpdUnit == NaviOverSpdUnit::KILOMETER && sfcValue >= mNaviOverSpeedRange1 &&
            sfcValue < mNaviOverSpeedRange2 && mNaviOverSpeedRange1 >= 1 &&
            mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::OVERSPEED1;
        } else if (mIgnElapsedStatus == IgnElapsed::ON_0ms &&
                   mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR &&
                   mNaviOverSpdUnit == NaviOverSpdUnit::MILE && sfcValue >= mNaviOverSpeedRange1 &&
                   sfcValue < mNaviOverSpeedRange2 && mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::OVERSPEED1;
        } else if (mIgnElapsedStatus == IgnElapsed::ON_0ms && mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::KM_PER_HOUR &&
                   mNaviOverSpdUnit == NaviOverSpdUnit::KILOMETER && sfcValue >= mNaviOverSpeedRange2 &&
                   mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::OVERSPEED2;
        } else if (mIgnElapsedStatus == IgnElapsed::ON_0ms &&
                   mPrivateDisplaySpeedUnit == PrivateDisplaySpeedUnit::MILE_PER_HOUR &&
                   mNaviOverSpdUnit == NaviOverSpdUnit::MILE && sfcValue >= mNaviOverSpeedRange2 &&
                   mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::OVERSPEED2;
        } else {
            stat = SFCSpeed_GaugeConstantNaviSpeedLimitStat::DEFAULT;
        }
        setSFCSpeed_GaugeConstantNaviSpeedLimitStat(stat);
    }

    // 5.15.5.2.5 디지털 속도계 속도별 색상 차별화
    void updateNaviSpeedLimitOver1ColorValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms && mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            setSFCSpeed_GaugeConstantNaviSpeedLimitOver1ColorValue(mNaviOverSpeedColor1);
        } else {
            setSFCSpeed_GaugeConstantNaviSpeedLimitOver1ColorValue(0);
        }
    }

    // 5.15.5.2.5 디지털 속도계 속도별 색상 차별화
    void updateNaviSpeedLimitOver2ColorValue() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms && mPrivateNaviOverSpeedInputValid == PrivateNaviOverSpeedInputValid::VALID) {
            setSFCSpeed_GaugeConstantNaviSpeedLimitOver2ColorValue(mNaviOverSpeedColor2);
        } else {
            setSFCSpeed_GaugeConstantNaviSpeedLimitOver2ColorValue(0);
        }
    }

    // TODO: 사양서 상 위치 주석으로 표기 필요
    void updateSFCPrivateDisplaySpeedValueKPHRange() {
        static SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat prev = SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat::NONE;
        SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat current;
        if (mDisplaySpeedValueKPH >= 10) {
            current = SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat::SPEED_OVER_10;
        } else if (mDisplaySpeedValueKPH >= 7 && mDisplaySpeedValueKPH < 10) {
            current = SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat::SPEED_7_10;
        } else {
            current = SFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat::SPEED_0_7;
        }
        if (prev != current) {
            prev = current;
            setSFCPrivateSpeed_GaugeDisplaySpeedValueKPHRangeStat(current);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class PrivateDisplaySpeedUnit : HUInt64 {
        NONE,
        KM_PER_HOUR,
        MILE_PER_HOUR,
        MAX
    };

    enum class PrivateNaviOverSpeedInputValid : HUInt64 {
        VALID,
        INVALID
    };

    using ActualVehicleSpeedValidity = ArgumentsActualVehicleSpeedValidityChanged::Inter_ActualVehicleSpeedValidity;
    ActualVehicleSpeedValidity mActualVehicleSpeedValidity = ActualVehicleSpeedValidity::NONE;

    using ConfigArea = ArgumentsAreaChanged::Inter_ConfigArea;
    ConfigArea mConfigArea = ConfigArea::DOM;

    using ConfigMainSpeedUnitChangeable = ArgumentsDisplaySpeedUnitChanged::Inter_ConfigMainSpeedUnitChangeable;
    ConfigMainSpeedUnitChangeable mConfigMainSpeedUnitChangeable = ConfigMainSpeedUnitChangeable::FIXED_UNIT;

    using ConfigSpeedometerType = ArgumentsDisplaySpeedUnitChanged::Inter_ConfigSpeedometerType;
    ConfigSpeedometerType mConfigSpeedometerType = ConfigSpeedometerType::KPH;

    using TripDistanceUnit = ArgumentsDisplaySpeedUnitChanged::Inter_TripDistanceUnit;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;

    using NaviOverSpdUnit = ArgumentsNaviOverSpdUnitChanged::Input_NaviOverSpdUnit;
    NaviOverSpdUnit mNaviOverSpdUnit = NaviOverSpdUnit::KILOMETER;

    using ConfigAnalogSpeedMax = ArgumentsAnalogSpeedMaxChanged::Inter_ConfigAnalogSpeedMax;
    ConfigAnalogSpeedMax mConfigAnalogSpeedMax = ConfigAnalogSpeedMax::KPH160;

    std::array<HDouble, 16> mParSpeedToleranceKphArray{};
    std::array<HDouble, 11> mParSpeedToleranceMphArray{};
    HDouble mParMaxDisplaySpeedKph = 0.;
    HDouble mParMaxDisplaySpeedMph = 0.;

    IgnElapsed mIgnElapsedStatus = IgnElapsed::OFF_0ms;
    HDouble mActualVehicleSpeed = 0.;

    // 5.15.4.1.1.1 속도 구간별 Parameter값 불러오기 (private member variable)
    HUInt64 mPrivateCurrentSpeedSectionKph = 0;
    HUInt64 mPrivateCurrentSpeedSectionMph = 0;
    HDouble mPrivateToleranceKph1 = 0.;
    HDouble mPrivateToleranceKph2 = 0.;
    HDouble mPrivateToleranceMph1 = 0.;
    HDouble mPrivateToleranceMph2 = 0.;

    // 5.15.4.1.1.2 현재 속도 공차 계산 (private member variable)
    HDouble mPrivate_CurrToleranceKph = 0.;
    HDouble mPrivate_CurrToleranceMph = 0.;

    // 5.15.4.1.2 최종 표시 속도 연산 (private member variable)
    HUInt64 mPrivateDisplaySpeedIntegerKph = 0;
    HDouble mPrivateDisplaySpeedDecimalKph = 0.;
    HUInt64 mPrivateDisplaySpeedIntegerMph = 0;
    HDouble mPrivateDisplaySpeedDecimalMph = 0.;
    HDouble mDisplaySpeedValueKPH = 0.;
    HDouble mDisplaySpeedValueMPH = 0.;
    HUInt64 mDigitalDisplaySpeedValueKPH = 0;
    HUInt64 mDigitalDisplaySpeedValueMPH = 0;

    // 5.15.4.2 표시 단위 판단 (private member variable)
    HUInt64 mNaviOverSpeedColor1 = 0;
    HUInt64 mNaviOverSpeedColor2 = 0;
    HUInt64 mNaviOverSpeedRange1 = 0;
    HUInt64 mNaviOverSpeedRange2 = 0;
    PrivateDisplaySpeedUnit mPrivateDisplaySpeedUnit = PrivateDisplaySpeedUnit::NONE;

    // 5.15.4.4 내비게이션 과속 표시 신호 정보 유효성 판단 (private member variable)
    PrivateNaviOverSpeedInputValid mPrivateNaviOverSpeedInputValid = PrivateNaviOverSpeedInputValid::INVALID;

    // 5.15.5.2.1.2 디지털 타입
    ssfs::SFCTimer<Speed_Gauge> mRepeatTimerTimeoutToSetVehicleSpeedDigitalDisplayValue;
    static constexpr uint32_t kTimerDigitalDisplayValueInterval200ms = 200u;

    // 5.15.5.2.1.3 디지털 타입 - 보조 속도계
    ssfs::SFCTimer<Speed_Gauge> mRepeatTimerTimeoutToSetVehicleSpeedSubDigitalDisplayValue;
    static constexpr uint32_t kTimerSubDigitalDisplayInterval200ms = 200u;

    // Factor_ConvKmToMi
    HDouble mFactor_ConvKmToMi = 256.0 / 412.0;

    // Actuator
    AddressValueList mActuatorOutputAddrList;

    // Factor
    static constexpr HDouble kFactorActualVehicleSpeed = 1.;
    static constexpr HDouble kFactorCluDisSpdVal = 0.5;
    static constexpr HDouble kFactorDisSpdDcmVal = 0.125;
    static constexpr HUInt64 kOutputDisplaySpeedKphValueErr = 511;
    static constexpr HUInt64 kOutput_DisplaySpeedValueErr = 1023;

    static constexpr HUInt64 kOutput_DisplaySpeedUnitKPH = 0;
    static constexpr HUInt64 kOutput_DisplaySpeedUnitMPH = 1;

    static constexpr HUInt64 kOutput_DisplaySpeedDefaultUnitKPH = 0;
    static constexpr HUInt64 kOutput_DisplaySpeedDefaultUnitKMPH = 1;
    static constexpr HUInt64 kOutput_DisplaySpeedDefaultUnitMKPH = 2;

    static constexpr HUInt64 kNaviOverSpeedRangeMin = 0x0;
    static constexpr HUInt64 kNaviOverSpeedRangeMax = 0xff;
    static constexpr HUInt64 kNaviOverSpeedColorMin = 0x0;
    static constexpr HUInt64 kNaviOverSpeedColorMax = 0xffffff;

    HBool mAEMInitializeComplete = false;
};

}  // namespace ccos

#endif  // SFSS_Speed_Gauge_H
