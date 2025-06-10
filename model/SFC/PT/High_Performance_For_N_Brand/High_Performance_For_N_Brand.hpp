/**
 * @file High_Performance_For_N_Brand.hpp
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
#ifndef SFSS_High_Performance_For_N_Brand_H
#define SFSS_High_Performance_For_N_Brand_H

#define DLOG_ENABLED gEnableSFCLog

#include "High_Performance_For_N_BrandBase.hpp"
#include <SFCTimer.h>
#include <Vehicle.h>

#include <array>
#include <cmath>
#include <chrono>

namespace ccos {

// SFC Version : 32.0.0
// Reference : [PT270] High_Performance_For_N_Brand. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class High_Performance_For_N_Brand : public High_Performance_For_N_BrandBase {
public:
    High_Performance_For_N_Brand() = default;
    ~High_Performance_For_N_Brand() override = default;
    High_Performance_For_N_Brand(const High_Performance_For_N_Brand& other) = delete;
    High_Performance_For_N_Brand(High_Performance_For_N_Brand&& other) noexcept = delete;
    High_Performance_For_N_Brand& operator=(const High_Performance_For_N_Brand& other) = delete;
    High_Performance_For_N_Brand& operator=(High_Performance_For_N_Brand&& other) noexcept = delete;

    void onInitialize() override {
        mNgsTimerMap = {{NgsTimerStatus::SEC_1, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_1},
                        {NgsTimerStatus::SEC_2, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_2},
                        {NgsTimerStatus::SEC_3, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_3},
                        {NgsTimerStatus::SEC_4, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_4},
                        {NgsTimerStatus::SEC_5, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_5},
                        {NgsTimerStatus::SEC_6, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_6},
                        {NgsTimerStatus::SEC_7, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_7},
                        {NgsTimerStatus::SEC_8, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_8},
                        {NgsTimerStatus::SEC_9, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_9},
                        {NgsTimerStatus::SEC_10, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_10},
                        {NgsTimerStatus::SEC_11, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_11},
                        {NgsTimerStatus::SEC_12, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_12},
                        {NgsTimerStatus::SEC_13, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_13},
                        {NgsTimerStatus::SEC_14, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_14},
                        {NgsTimerStatus::SEC_15, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_15},
                        {NgsTimerStatus::SEC_16, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_16},
                        {NgsTimerStatus::SEC_17, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_17},
                        {NgsTimerStatus::SEC_18, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_18},
                        {NgsTimerStatus::SEC_19, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_19},
                        {NgsTimerStatus::SEC_20, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::SEC_20}};
        setSFCHigh_Performance_For_N_BrandEventBrakeOverrideID("E24801");
        setSFCHigh_Performance_For_N_BrandEventBrakeOverrideLinkedSoundID("SND_PopUpWarn1");
        setSFCHigh_Performance_For_N_BrandEventBrakeOverrideLinkedSoundType(
            SFCHigh_Performance_For_N_BrandEventBrakeOverrideLinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandEventBrakeOverrideLinkedSoundRepeatCount(1);
        setSFCHigh_Performance_For_N_BrandEventNgs1ID("E24811");
        setSFCHigh_Performance_For_N_BrandSoundShiftLightWarnID("SND_ShiftLightWarn");
        setSFCHigh_Performance_For_N_BrandSoundShiftLightWarnType(
            SFCHigh_Performance_For_N_BrandSoundShiftLightWarnType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandSoundShiftLightWarnRepeatCount(1);
        setSFCHigh_Performance_For_N_BrandEventNRoadSenseID("E24814");
        setSFCHigh_Performance_For_N_BrandEventNRoadSenseLinkedSoundID("SND_PopUpInform2");
        setSFCHigh_Performance_For_N_BrandEventNRoadSenseLinkedSoundType(
            SFCHigh_Performance_For_N_BrandEventNRoadSenseLinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandEventNRoadSenseLinkedSoundRepeatCount(1);
        setSFCHigh_Performance_For_N_BrandEventBrakepadCheckID("E24815");
        setSFCHigh_Performance_For_N_BrandEventBrakepadCheckLinkedSoundID("SND_PopUpWarn1");
        setSFCHigh_Performance_For_N_BrandEventBrakepadCheckLinkedSoundType(
            SFCHigh_Performance_For_N_BrandEventBrakepadCheckLinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandEventBrakepadCheckLinkedSoundRepeatCount(1);
        setSFCHigh_Performance_For_N_BrandEventNgs2LinkedSoundID("SND_PopUpWarn1");
        setSFCHigh_Performance_For_N_BrandEventNgs2LinkedSoundType(
            SFCHigh_Performance_For_N_BrandEventNgs2LinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandEventNgs2LinkedSoundRepeatCount(1);
        setSFCHigh_Performance_For_N_BrandEventElectricKickDriftID("E24816");
        setSFCHigh_Performance_For_N_BrandEventElectricKickDriftLinkedSoundID("SND_PopUpInform2");
        setSFCHigh_Performance_For_N_BrandEventElectricKickDriftLinkedSoundType(
            SFCHigh_Performance_For_N_BrandEventElectricKickDriftLinkedSoundType::REPEAT_COUNT);
        setSFCHigh_Performance_For_N_BrandEventElectricKickDriftLinkedSoundRepeatCount(1);
        if (mRepeatTimerPrivateGforce.create(kTimerPrivateGforceInterval200ms, this,
                                             &High_Performance_For_N_Brand::onRepeatTimerPrivateGforce, true) == false) {
            DWarning() << "[High_Performance_For_N_Brand] mRepeatTimerPrivateGforce Create Failed";
        }
        if (mOneShotTimerPrivateDisplayLapTimeDiff.create(
                kTimerPrivateDisplayLapTimeDiffInterval5s, this,
                &High_Performance_For_N_Brand::onOneShotTimerUpdateDisplayLapTimeDiffTimeout, false) == false) {
            DWarning() << "[High_Performance_For_N_Brand] mOneShotTimerPrivateDisplayLapTimeDiff Create Failed";
        }

        if (mRepeatTimerLapTimeReal.create(kTimerLapTimeRealInterval16ms, this,
                                           &High_Performance_For_N_Brand::onRepeatTimerUpdateLapTimeReal, true) == false) {
            DWarning() << "[High_Performance_For_N_Brand] mRepeatTimerLapTimeReal Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIgn1Status = true;
        mPrevLapTimerPlayReq = LapTimerPlayReq::DEFAULT;
        mLapTimerPlayReq = LapTimerPlayReq::DEFAULT;
        mLapTimerOperationReqFromAvn = LapTimerOperationReqFromAvn::DEFAULT;
        mPrevLapTimerOperationReqFromAvn = LapTimerOperationReqFromAvn::DEFAULT;
        updatePrivateGforce();
        updatePrivateShiftLightStatus();
        updateTelltaleCreepOff();
        updateTelltaleShiftLight();
        updateConstantNgs();
        updateConstantNTrackSense();
        updateConstantNCustomSettingCaseStatus();
        updateConstantNCustomEngineSettingStatus();
        updateConstantNCustomMdpsSettingStatus();
        updateConstantNCustomEcsSettingStatus();
        updateConstantNCustomMotorSettingStatus();
        updateEventNgs1();
        updateEventNgs2();
        updateEventNRoadSense();
        updateEventBrakepadCheck();
        updateEventElectricKickDrift();
        updateSoundLogic();
        updateLapTimerOperationStatus();
        updateEventBrakeOverride();
        updateTelltaleEnduranceRaceStatus();
        updateTelltaleNBatteryStatus();
        mPrevIgn1Status = true;
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIgn1Status = true;
        updateTelltaleShiftLight();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIgn1Status = false;
        updatePrivateGforce();
        updatePrivateShiftLightStatus();
        updateTelltaleCreepOff();
        updateTelltaleShiftLight();
        updateTelltaleEnduranceRaceStatus();
        updateTelltaleNBatteryStatus();
        updateConstantNgs();
        updateConstantNTrackSense();
        updateConstantNCustomSettingCaseStatus();
        updateConstantNCustomEngineSettingStatus();
        updateConstantNCustomMdpsSettingStatus();
        updateConstantNCustomEcsSettingStatus();
        updateConstantNCustomMotorSettingStatus();
        updateEventNgs1();
        updateEventNgs2();
        updateEventNRoadSense();
        updateEventBrakepadCheck();
        updateEventElectricKickDrift();
        updateSoundLogic();
        updateLapTimerOperationStatus();
        updateEventBrakeOverride();
        mPrevIgn1Status = false;
        if (mAEMInitCompleteFlag == true) {
            updateValue("SFC.Extension.Memory_IGN.Private_LaptimerOperationStatus",
                        static_cast<HUInt64>(mPrivateLapTimerOperationStatus));
        }
        DDebug() << "mAEMInitCompleteFlag: " << static_cast<HBool>(mAEMInitCompleteFlag);
        DDebug() << "Private_LaptimerOperationStatus: " << static_cast<HUInt64>(mPrivateLapTimerOperationStatus);
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updatePrivateShiftLightStatus();
        updateTelltaleShiftLight();
        updateConstantNCustomSettingCaseStatus();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;
        updatePrivateGforce();
        updatePrivateShiftLightStatus();
        updateTelltaleCreepOff();
        updateTelltaleShiftLight();
        updateConstantNgs();
        updateConstantNTrackSense();
        updateConstantNCustomSettingCaseStatus();
        updateConstantNCustomEngineSettingStatus();
        updateConstantNCustomMdpsSettingStatus();
        updateConstantNCustomEcsSettingStatus();
        updateConstantNCustomMotorSettingStatus();
        updateEventNgs1();
        updateEventNgs2();
        updateEventNRoadSense();
        updateEventBrakepadCheck();
        updateEventElectricKickDrift();
        updateSoundLogic();
        updateLapTimerOperationStatus();
        updateTelltaleEnduranceRaceStatus();
        updateTelltaleNBatteryStatus();
    }

    void onGForceChanged(const ArgumentsGForceChanged& args) {
        mAccelerationLatStatus = args.mInput_AccelerationLatStatus;
        mAccelerationLongStatus = args.mInput_AccelerationLongStatus;
        mAccelerationLatValue = args.mInput_AccelerationLatValue;
        mAccelerationLongValue = args.mInput_AccelerationLongValue;
        mDrivingOnStatus = args.mInter_DrivingOnStatus;

        updatePrivateGforce();
    }

    void onLapTimerSignalChanged(const ArgumentsLapTimerSignalChanged& args) {
        mLapTimerAutoLapNumberValue = args.mInput_LapTimerAutoLapNumberValue;
        mLapTimerAutoHourValue = args.mInput_LapTimerAutoHourValue;
        mLapTimerAutoMinuteValue = args.mInput_LapTimerAutoMinuteValue;
        mLapTimerAutoSecondValue = args.mInput_LapTimerAutoSecondValue;
        mLapTimerAutoMilliSecondValue = args.mInput_LapTimerAutoMilisecondValue;
        mLapTimerAutoRunningStatus = args.mInput_LapTimerAutoRunningStatus;
        updateValueIfValid(mLapTimerAutoMeasureSetStatus, args.mInter_LapTimerAutoMeasureSetStatus);
        updateValueIfValid(mLapTimerDifferTimeSetStatus, args.mInter_LapTimerDifferTimeSetStatus);
        mLapTimerPlayReq = args.mInter_LapTimerPlayReq;
        mLapTimerOperationReqFromAvn = args.mInput_LapTimerOperationReqFromAvn;
        updateLapTimerOperationStatus();

        mPrevLapTimerAutoMeasureSetStatus = mLapTimerAutoMeasureSetStatus;
        mPrevLapTimerPlayReq = mLapTimerPlayReq;
        mPrevLapTimerOperationReqFromAvn = mLapTimerOperationReqFromAvn;
        mPrivatePrevLapTimerOperationStatus = mPrivateLapTimerOperationStatus;
        mPrivatePrevLapNumber = mPrivateLapNumber;
    }

    void onCreepOffIndiChanged(const ArgumentsCreepOffIndiChanged& args) {
        mCreepOffIndiStatus = args.mInput_CreepOffIndiStatus;
        updateTelltaleCreepOff();
    }

    void onShiftLightChanged(const ArgumentsShiftLightChanged& args) {
        mDriveModeSelectStatus = args.mInter_DriveModeSelectStatus;
        updateValueIfValid(mShiftLightNModeSetStatus, args.mInter_ShiftLightNModeSetStatus);
        updateValueIfValid(mShiftLightEcoModeSetStatus, args.mInter_ShiftLightEcoModeSetStatus);
        updateValueIfValid(mShiftLightCustomModeSetStatus, args.mInter_ShiftLightCustomModeSetStatus);
        updateValueIfValid(mShiftLightNormalModeSetStatus, args.mInter_ShiftLightNormalModeSetStatus);
        updateValueIfValid(mShiftLightSportModeSetStatus, args.mInter_ShiftLightSportModeSetStatus);
        updatePrivateShiftLightStatus();
        updateTelltaleShiftLight();
    }

    void onNModeSelectStatusChanged(const ArgumentsNModeSelectStatusChanged& args) {
        mNModeSelectStatus = args.mInter_NModeSelectStatus;
        updatePrivateShiftLightStatus();
        updateTelltaleShiftLight();
        updateConstantNCustomEngineSettingStatus();
        updateConstantNCustomMdpsSettingStatus();
        updateConstantNCustomEcsSettingStatus();
        updateConstantNCustomMotorSettingStatus();
        updateEventNRoadSense();
        mPrevNModeSelectStatus = mNModeSelectStatus;
    }

    void onGtModeSelectStatusChanged(const ArgumentsGtModeSelectStatusChanged& args) {
        mGtModeSelectStatus = args.mInter_GtModeSelectStatus;
        updateConstantNCustomEngineSettingStatus();
        updateConstantNCustomMdpsSettingStatus();
        updateConstantNCustomEcsSettingStatus();
        updateConstantNCustomMotorSettingStatus();
    }

    void onNTrackSenseDisplayReqChanged(const ArgumentsNTrackSenseDisplayReqChanged& args) {
        mNTrackSenseDisplayReqStatus = args.mInput_NTrackSenseDisplayReqStatus;
        updateConstantNTrackSense();
    }

    void onNgsTimerChanged(const ArgumentsNgsTimerChanged& args) {
        mNgsTimerStatus = args.mInput_NgsTimerStatus;
        updateConstantNgs();
    }

    void onNCustomEngineSetStatusChanged(const ArgumentsNCustomEngineSetStatusChanged& args) {
        mNCustom1EngineSetStatus = args.mInput_NCustom1EngineSetStatus;
        mNCustom2EngineSetStatus = args.mInput_NCustom2EngineSetStatus;
        updateConstantNCustomEngineSettingStatus();
    }

    void onNCustomEcsSetStatusChanged(const ArgumentsNCustomEcsSetStatusChanged& args) {
        mNCustom1EcsSetStatus = args.mInput_NCustom1EcsSetStatus;
        mNCustom2EcsSetStatus = args.mInput_NCustom2EcsSetStatus;
        updateConstantNCustomEcsSettingStatus();
    }

    void onNCustomMdpsSetStatusChanged(const ArgumentsNCustomMdpsSetStatusChanged& args) {
        mNCustom1MdpsSetStatus = args.mInput_NCustom1MdpsSetStatus;
        mNCustom2MdpsSetStatus = args.mInput_NCustom2MdpsSetStatus;
        updateConstantNCustomMdpsSettingStatus();
    }

    void onNgsEventChanged(const ArgumentsNgsEventChanged& args) {
        mNgsActiveStatus = args.mInput_NgsActiveStatus;
        mNgsFaultStatus = args.mInput_NgsFaultStatus;
        updateEventNgs1();
        updateEventNgs2();
    }

    void onSoundChanged(const ArgumentsSoundChanged& args) {
        mShiftLightIndReqStatus = args.mInput_ShiftLightIndReqStatus;
        updateValueIfValid(mShiftLightBeepSoundSetStatus, args.mInter_ShiftLightBeepSoundSetStatus);
        updateTelltaleShiftLight();
        updateSoundLogic();
    }

    void onInputNroadsenseActiveStatusChanged(const ArgumentsInputNroadsenseActiveStatusChanged& args) {
        mNroadsenseActiveStatus = args.mInput_NroadsenseActiveStatus;
        updateEventNRoadSense();
    }

    void onBrakepadCheckStatusChanged(const ArgumentsBrakepadCheckStatusChanged& args) {
        mPwiWarnStatus = args.mInput_PwiWarnStatus;
        updateEventBrakepadCheck();
    }

    void onEkdReadyStatusChanged(const ArgumentsEkdReadyStatusChanged& args) {
        mEkdReadyStatus = args.mInput_EkdReadyStatus;
        updateEventElectricKickDrift();
    }

    void onNCustomMotorSetStatusChanged(const ArgumentsNCustomMotorSetStatusChanged& args) {
        mNCustom1MotorSetStatus = args.mInput_NCustom1MotorSetStatus;
        mNCustom2MotorSetStatus = args.mInput_NCustom2MotorSetStatus;
        updateConstantNCustomMotorSettingStatus();
    }

    void onOverBoostStatusChanged(const ArgumentsOverBoostStatusChanged& args) {
        mOverBoostStatus = args.mInput_OverBoostStatus;
        updateEventBrakeOverride();
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitCompleteFlag, args.mInter_AEMInitializeComplete) == true) {
            DDebug() << "Inter_AEMInitializeComplete: " << static_cast<HBool>(mAEMInitCompleteFlag);
            if (mAEMInitCompleteFlag == true) {
                HUInt64 lapTimerOperationStatus = 0;
                GETCACHEDVALUE(SFC.Extension.Memory_IGN.Private_LaptimerOperationStatus, lapTimerOperationStatus);
                mPrivateLapTimerOperationStatus = static_cast<LapTimerOperationStatus>(lapTimerOperationStatus);
                if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::NONE ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::MAX) {
                    mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
                }
                updateValue("SFC.Extension.Memory_IGN.Private_LaptimerOperationStatus",
                            static_cast<HUInt64>(mPrivateLapTimerOperationStatus));
                DDebug() << "PrivatePrevLapTimerOperationStatus: " << static_cast<HUInt64>(mPrivatePrevLapTimerOperationStatus);
                updateLapTimerOperationStatus();
                mPrivatePrevLapTimerOperationStatus = mPrivateLapTimerOperationStatus;
            }
        }
    }

    void onEnduranceRaceStatusChanged(const ArgumentsEnduranceRaceStatusChanged& args) {
        mEvEndurancePowerLimitStatus = args.mInput_EvEndurancePowerLimitStatus;
        mTrackOptimizerOnOffStatus = args.mInput_TrackOptimizerOnOffStatus;
        updateTelltaleEnduranceRaceStatus();
    }

    void onCustomSettingsDisplayTypesInfoChanged(const ArgumentsCustomSettingsDisplayTypesInfoChanged& args) {
        mNCustomModeOptionStatus = args.mInput_NCustomModeOptionStatus;
        mNCustomMode2OptionStatus = args.mInput_NCustomMode2OptionStatus;
        mDmicModeAppliedStatus = args.mInput_DmicModeAppliedStatus;
        mDmicEngAppliedStatus = args.mInput_DmicEngAppliedStatus;
        mDmicMdpsAppliedStatus = args.mInput_DmicMdpsAppliedStatus;
        mDmicVcuAppliedStatus = args.mInput_DmicVcuAppliedStatus;
        mDmicEcsAppliedStatus = args.mInput_DmicEcsAppliedStatus;
        updateConstantNCustomSettingCaseStatus();
    }

    void onNBatteryStatusChanged(const ArgumentsNBatteryStatusChanged& args) {
        mNBatteryStatus = args.mInput_NBatteryStatus;
        updateTelltaleNBatteryStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1 G-FORCE 계산
    void onRepeatTimerPrivateGforce() {
        mPrivateGforceXCoordValue =
            floor((static_cast<HDouble>(mAccelerationLatValue) * 0.000127465 - 4.17677312) * 1000) * 0.001;
        mPrivateGforceYCoordValue =
            floor((static_cast<HDouble>(mAccelerationLongValue) * 0.000127465 - 4.17677312) * 1000) * 0.001;

        updateConstantGForce();
        flush();
    }

    // 4.2 G-FORCE 계산
    void updatePrivateGforce() {  // G-Force 시그널들은 10ms 주기로 업데이트 됨.
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true) {
            if (mRepeatTimerPrivateGforce.start() == false) {
                DWarning() << "[High_Performance_For_N_Brand] mRepeatTimerPrivateGforce Start Failed";
            }
        } else {
            mRepeatTimerPrivateGforce.stop();
        }
    }

    // 4.3 ShiftLigt enable 판단
    void updatePrivateShiftLightStatus() {
        mPrivateShiftLightStatus = ShiftLightStatus::DISABLE;
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true) {
            if (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                mConfigVehicleType == ConfigVehicleType::PHEV) {
                if (mShiftLightNModeSetStatus == ShiftLightNModeSetStatus::ENABLE && mNModeSelectStatus == NModeSelectStatus::N) {
                    mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
                } else if (mShiftLightCustomModeSetStatus == ShiftLightCustomModeSetStatus::ENABLE &&
                           (mNModeSelectStatus == NModeSelectStatus::CUSTOM1 ||
                            mNModeSelectStatus == NModeSelectStatus::CUSTOM2)) {
                    mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
                } else if (mShiftLightSportModeSetStatus == ShiftLightSportModeSetStatus::ENABLE &&
                           mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) {
                    mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
                } else if (mShiftLightNormalModeSetStatus == ShiftLightNormalModeSetStatus::ENABLE &&
                           mDriveModeSelectStatus == DriveModeSelectStatus::NORMAL) {
                    mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
                } else if (mShiftLightEcoModeSetStatus == ShiftLightEcoModeSetStatus::ENABLE &&
                           mDriveModeSelectStatus == DriveModeSelectStatus::ECO) {
                    mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
                } else {
                    // nothing
                }
            } else if (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
                mPrivateShiftLightStatus = ShiftLightStatus::ENABLE;
            } else {
                // nothing
            }
        }
    }

    // 4.4.1.3 랩타이머 누적 사용시간(이전랩) 저장 함수
    void updateLapTimePreAccum() {
        mLapTimerAccumCurrPoint = std::chrono::steady_clock::now();
        auto lapTimerDuration = mLapTimerAccumCurrPoint - mLapTimerAccumStartPoint;
        std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(lapTimerDuration);
        lapTimerDuration -= h;
        std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(lapTimerDuration);
        lapTimerDuration -= m;
        std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(lapTimerDuration);
        lapTimerDuration -= s;
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(lapTimerDuration);
        mLapTimerAccumStartPoint = mLapTimerAccumCurrPoint;

        DDebug() << "Prev Lap Time = " << h.count() << ":" << m.count() << ":" << s.count() << ":" << ms.count();

        // xx:xx:xx:xx. Two most-significiat digits used.
        mPrivateLapTimePreAccum[0] = h.count();
        mPrivateLapTimePreAccum[1] = m.count();
        mPrivateLapTimePreAccum[2] = s.count();
        mPrivateLapTimePreAccum[3] = ms.count() / kRangeForCanTransmission;
    }

    // 4.4.2.1 랩타이머 동작 상태
    void updateLapTimerOperationStatus() {
        static SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat stat =
            SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::OFF;

        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mIgn1Status == true) {
                if ((mPrevLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL &&
                     mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO) ||
                    (mPrevLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO &&
                     mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL)) {
                    mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
                } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL) {
                    if ((mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                         mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS) &&
                        ((mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT && mLapTimerPlayReq == LapTimerPlayReq::RESET_REQ) ||
                         (mPrevLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::DEFAULT &&
                          mLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::RESET_REQ))) {
                        mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
                    } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF &&
                               ((mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT &&
                                 mLapTimerPlayReq == LapTimerPlayReq::START_REQ) ||
                                (mPrevLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::DEFAULT &&
                                 mLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::START_REQ))) {
                        mPrivateLapTimerOperationStatus = LapTimerOperationStatus::RUNNING;
                    } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING &&
                               ((mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT &&
                                 mLapTimerPlayReq == LapTimerPlayReq::RESET_REQ) ||
                                (mPrevLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::DEFAULT &&
                                 mLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::RESET_REQ) ||
                                (mPrivateLapNumber >= (kLimitLapNum + 1)))) {
                        mPrivateLapTimerOperationStatus = LapTimerOperationStatus::FINISH;
                    } else {
                        // nothing
                    }
                } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO) {
                    if ((mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT && mLapTimerPlayReq == LapTimerPlayReq::RESET_REQ) ||
                        (mPrevLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::DEFAULT &&
                         mLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::RESET_REQ)) {
                        if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                            mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS ||
                            mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY) {
                            mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
                        } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
                            mPrivateLapTimerOperationStatus = LapTimerOperationStatus::FINISH;
                        } else {
                            // nothing
                        }
                    } else if (((mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT &&
                                 mLapTimerPlayReq == LapTimerPlayReq::START_REQ) ||
                                (mPrevLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::DEFAULT &&
                                 mLapTimerOperationReqFromAvn == LapTimerOperationReqFromAvn::START_REQ)) &&
                               mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF) {
                        mPrivateLapTimerOperationStatus = LapTimerOperationStatus::READY;
                    } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY &&
                               mLapTimerAutoRunningStatus == LapTimerAutoRunningStatus::RUN) {
                        mPrivateLapTimerOperationStatus = LapTimerOperationStatus::RUNNING;
                    } else {
                        // nothing
                    }
                } else {
                    // nothing
                }

                // 4.3.1.1 랩타이머 타이머 시작
                if (mPrivateLapTimerOperationStatus != LapTimerOperationStatus::RUNNING) {
                    mPrivateLapTimePreAccum = {
                        0,
                    };
                    mOnceFlag = false;
                } else {
                    if (!mOnceFlag) {
                        mLapTimerAccumStartPoint = std::chrono::steady_clock::now();
                        mLapTimerAccumCurrPoint = mLapTimerAccumStartPoint;
                        mOnceFlag = true;
                    }
                }
            } else if (mPrevIgn1Status == true && mIgn1Status == false) {
                if ((mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING ||
                     mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH) &&
                    (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL ||
                     mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO)) {
                    mPrivateLapTimerOperationStatus = LapTimerOperationStatus::FINISH_PREVIOUS;
                } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY) {
                    mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }

            updateLapNumber();
            updateDisplayLapTimeDiff();
            updateLapTimerOutputProc();
            updateConstantAutoDisplay();
            updateConstantLapTimeDiff();
            updateLapTimeRankRecording();
            updateConstantLapTimeDisplay();
            updateConstantRealAndPreviousTime();
            // 5.2.4.2 랩타임 표시 IMG_LaptimerOperationStatus
            // Sync mPrivateLapTimerOperationStatus and stat
            switch (mPrivateLapTimerOperationStatus) {
                case LapTimerOperationStatus::OFF:
                    stat = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::OFF;
                    break;
                case LapTimerOperationStatus::READY:
                    stat = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::READY;
                    break;
                case LapTimerOperationStatus::RUNNING:
                    stat = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::RUNNING;
                    break;
                case LapTimerOperationStatus::FINISH:
                    stat = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::FINISH;
                    break;
                case LapTimerOperationStatus::FINISH_PREVIOUS:
                    stat = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat::FINISH_PREVIOUS;
                    break;
                default:
                    break;
            }
            setSFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat(stat);
        }
    }

    // 4.4.2.2 랩타이머 비교값 표시 여부
    void updateDisplayLapTimeDiff() {
        if (mIgn1Status == true) {
            if (mPrivateDisplayLapTimeDiff == DisplayLapTimeDiff::OFF && mPrivateLapNumber > 1 &&
                mPrivateLapNumber > mPrivatePrevLapNumber && mPrivatePrevLapNumber > 1 &&
                mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
                if (mOneShotTimerPrivateDisplayLapTimeDiff.start() == false) {
                    DWarning() << "[High_Performance_For_N_Brand] mOneShotTimerPrivateDisplayLapTimeDiff Start Failed";
                }
                mPrivateDisplayLapTimeDiff = DisplayLapTimeDiff::ON;
                setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat(
                    SFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat::ON);
            } else {
                if ((mPrivateDisplayLapTimeDiff == DisplayLapTimeDiff::ON &&
                     mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING &&
                     mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT && mLapTimerPlayReq == LapTimerPlayReq::START_REQ) ||
                    (mPrivateLapNumber == 0 || mPrivateLapNumber == 1)) {
                    mOneShotTimerPrivateDisplayLapTimeDiff.stop();
                    mPrivateDisplayLapTimeDiff = DisplayLapTimeDiff::OFF;
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat(
                        SFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat::OFF);
                }
            }
        } else {
            mOneShotTimerPrivateDisplayLapTimeDiff.stop();
            mPrivateDisplayLapTimeDiff = DisplayLapTimeDiff::OFF;
            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat(
                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat::OFF);
        }
    }

    // 4.4.2.2 랩타이머 비교값 표시 여부 Private_DisplayLapTimeDiff == ON -> more than 5sec을 위한 Timer
    void onOneShotTimerUpdateDisplayLapTimeDiffTimeout() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mPrivateDisplayLapTimeDiff == DisplayLapTimeDiff::ON &&
            mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
            mPrivateDisplayLapTimeDiff = DisplayLapTimeDiff::OFF;
            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat(
                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffStat::OFF);
        }
    }

    // 4.4.2.3 랩타이머 랩 넘버 표시
    void updateLapNumber() {
        if (mIgn1Status == true) {
            if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL) {
                if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS) {
                    mPrivateLapNumber = 0;
                } else if (mPrivatePrevLapTimerOperationStatus == LapTimerOperationStatus::OFF &&
                           mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
                    mPrivateLapNumber = 1;
                } else if (mPrivateLapNumber < (kLimitLapNum + 1) &&
                           mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING &&
                           mPrivateDisplayLapTimeDiff == DisplayLapTimeDiff::OFF &&
                           mPrevLapTimerPlayReq == LapTimerPlayReq::DEFAULT && mLapTimerPlayReq == LapTimerPlayReq::START_REQ) {
                    mPrivateLapNumber++;
                } else {
                    // nothing
                }
            } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO) {
                if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                    mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS) {
                    mPrivateLapNumber = 0;
                } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
                    mPrivateLapNumber = mLapTimerAutoLapNumberValue + 1;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }

            // 4.3.1.3 랩타이머 누적 사용시간(이전랩)
            if (mPrivateLapNumber > mPrivatePrevLapNumber) {
                updateLapTimePreAccum();
            } else if (mPrivateLapNumber == 0) {
                mPrivateLapTimePreAccum = {
                    0,
                };
            } else {
                // nothing
            }

            updateRecordingLapTime();
        }
    }

    // 4.4.3.1(4.4.3.1.1&4.4.3.1.2) 랩타임 기록
    void updateRecordingLapTime() {
        if (mPrivateLapNumber == 0 || mPrivateLapNumber == 1) {
            mPrivateRecentLapTime = {
                0,
            };
            mPrivatePreviousLapTime = {
                0,
            };
        } else if (mPrivateLapNumber > 1 && mPrivateLapNumber > mPrivatePrevLapNumber) {
            mPrivatePreviousLapTime = mPrivateRecentLapTime;
            if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL) {
                mPrivateRecentLapTime = mPrivateLapTimePreAccum;
                mPrivateRecentLapTime[4] = mPrivateLapNumber - 1;
            } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO) {
                mPrivateRecentLapTime[0] = mLapTimerAutoHourValue;
                mPrivateRecentLapTime[1] = mLapTimerAutoMinuteValue;
                mPrivateRecentLapTime[2] = mLapTimerAutoSecondValue;
                mPrivateRecentLapTime[3] = mLapTimerAutoMilliSecondValue;
                mPrivateRecentLapTime[4] = mPrivateLapNumber - 1;
            } else {
                // nothing
            }
        } else {
            // nothing
        }

        // IMG_LapTimePrevious
        setSFCHigh_Performance_For_N_BrandConstantLapTimePreviousHourValue(mPrivateRecentLapTime[0]);
        setSFCHigh_Performance_For_N_BrandConstantLapTimePreviousMinValue(mPrivateRecentLapTime[1]);
        setSFCHigh_Performance_For_N_BrandConstantLapTimePreviousSecValue(mPrivateRecentLapTime[2]);
        setSFCHigh_Performance_For_N_BrandConstantLapTimePreviousMilliSecValue(mPrivateRecentLapTime[3]);
    }

    // 4.4.3.2 랩타임 등수 기록
    void updateLapTimeRankRecording() {
        std::vector<HUInt64> privateLapNumRecord(4);

        if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF ||
            mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY) {
            mPrivateLapTimeRecord.clear();
            resetConstantBestLapRecord();
        } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING &&
                   mPrivateLapNumber > mPrivatePrevLapNumber && mPrivateLapNumber > 1) {
            privateLapNumRecord.assign(begin(mPrivateRecentLapTime), end(mPrivateRecentLapTime));
            if (mPrivateLapTimeRecord.size() < 4) {
                mPrivateLapTimeRecord.emplace_back(privateLapNumRecord);
                if (!mPrivateLapTimeRecord.empty()) {
                    sort(mPrivateLapTimeRecord.begin(), mPrivateLapTimeRecord.end());
                }
            } else {
                mPrivateLapTimeRecord.emplace_back(privateLapNumRecord);
                sort(mPrivateLapTimeRecord.begin(), mPrivateLapTimeRecord.end());
                mPrivateLapTimeRecord.pop_back();
            }
        } else {
            // nothing
        }
    }

    // 4.4.4 출력 신호 처리
    void updateLapTimerOutputProc() {
        AddressValueList list;
        if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::OFF) {
            list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMeasuringStatus,
                              static_cast<HUInt64>(OutputLapTimerMeasuringStatusType::RESET_LAP_TIMER));
        } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                   mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS) {
            list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMeasuringStatus,
                              static_cast<HUInt64>(OutputLapTimerMeasuringStatusType::STOP_LAP_TIMER));
        } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY ||
                   mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
            list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMeasuringStatus,
                              static_cast<HUInt64>(OutputLapTimerMeasuringStatusType::UNDER_LAP_TIMER_MEASUREMENT));
        } else {
            // not specified.
        }

        if (mPrivateLapNumber > 0) {
            list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerLapNumberValue, mPrivateLapNumber - 1);
        } else {
            list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerLapNumberValue, 0U);
        }
        list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerHourValue, mPrivateRecentLapTime[0]);
        list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMinuteValue, mPrivateRecentLapTime[1]);
        list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerSecondValue, mPrivateRecentLapTime[2]);
        list.emplace_back(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMilisecondValue, mPrivateRecentLapTime[3]);

        setValue(list);
    }

    // 5.1.1 CREEP OFF
    void updateTelltaleCreepOff() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mCreepOffIndiStatus == CreepOffIndiStatus::ON) {
            setSFCHigh_Performance_For_N_BrandTelltaleCreepOffStat(SFCHigh_Performance_For_N_BrandTelltaleCreepOffStat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandTelltaleCreepOffStat(SFCHigh_Performance_For_N_BrandTelltaleCreepOffStat::OFF);
        }
    }

    // 5.1.3 Endurance Race
    void updateTelltaleEnduranceRaceStatus() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mEvEndurancePowerLimitStatus == EvEndurancePowerLimitStatus::LIMIT_ACTIVATE &&
            mTrackOptimizerOnOffStatus == TrackOptimizerOnOffStatus::ON) {
            setSFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat(
                SFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat::ON);
        } else if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
                   mIgn1Status == true && mEvEndurancePowerLimitStatus == EvEndurancePowerLimitStatus::LIMIT_DEACTIVATE &&
                   mTrackOptimizerOnOffStatus == TrackOptimizerOnOffStatus::ON) {
            setSFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat(
                SFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat::OPTIONAL);
            setSFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStatOptional(
                SFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStatOptional::ON_MAXRANGE);
        } else {
            setSFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat(
                SFCHigh_Performance_For_N_BrandTelltaleEnduranceRaceStat::OFF);
        }
    }

    // 5.1.3 N Battery (N 브랜드 전용, 현대 ONLY)
    void updateTelltaleNBatteryStatus() {
        if (mConfigHighPerformance != ConfigHighPerformance::N && mConfigHighPerformance != ConfigHighPerformance::GT) {
            return;
        }
        SFCHigh_Performance_For_N_BrandTelltaleNBatteryStat stat = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStat::OFF;
        SFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional statOptional =
            SFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional::DISPLAY_OFF;
        if (mIgn1Status == true) {
            switch (mNBatteryStatus) {
                case NBatteryStatus::DRAG:
                    stat = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStat::OPTIONAL;
                    statOptional = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional::DRAG;
                    break;
                case NBatteryStatus::SPRINT:
                    stat = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStat::OPTIONAL;
                    statOptional = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional::SPRINT;
                    break;
                case NBatteryStatus::ENDURANCE:
                    stat = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStat::OPTIONAL;
                    statOptional = SFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional::ENDURANCE;
                    break;
                default:
                    break;
            }
        }
        setSFCHigh_Performance_For_N_BrandTelltaleNBatteryStat(stat);
        setSFCHigh_Performance_For_N_BrandTelltaleNBatteryStatOptional(statOptional);
    }

    // 5.1.2 SHIFT LIGHT & 4.5 Inter 신호 송출
    void updateTelltaleShiftLight() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgnElapsed == IgnElapsed::ON_500ms && mPrivateShiftLightStatus == ShiftLightStatus::ENABLE) {
            switch (mShiftLightIndReqStatus) {
                case ShiftLightIndReqStatus::STEP1:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional::STEP1);
                    break;
                case ShiftLightIndReqStatus::STEP2:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional::STEP2);
                    break;
                case ShiftLightIndReqStatus::STEP3:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional::STEP3);
                    break;
                case ShiftLightIndReqStatus::STEP4:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional::STEP4);
                    break;
                case ShiftLightIndReqStatus::STEP5:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStatOptional::STEP5);
                    break;
                case ShiftLightIndReqStatus::STEP6:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::BLINK3);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueA(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueA::OFF);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueB(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueB::OPTIONAL);
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueBOptional(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightBlinkValueBOptional::STEP6);
                    break;
                default:
                    setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(
                        SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OFF);
                    break;
            }
        } else {
            setSFCHigh_Performance_For_N_BrandTelltaleShiftLightStat(SFCHigh_Performance_For_N_BrandTelltaleShiftLightStat::OFF);
        }
    }

    // 5.2.1 NGS
    void updateConstantNgs() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true) {
            switch (static_cast<NgsTimerStatus>(mNgsTimerStatus)) {
                case NgsTimerStatus::SEC_20:
                case NgsTimerStatus::SEC_19:
                case NgsTimerStatus::SEC_18:
                case NgsTimerStatus::SEC_17:
                case NgsTimerStatus::SEC_16:
                case NgsTimerStatus::SEC_15:
                case NgsTimerStatus::SEC_14:
                case NgsTimerStatus::SEC_13:
                case NgsTimerStatus::SEC_12:
                case NgsTimerStatus::SEC_11:
                case NgsTimerStatus::SEC_10:
                case NgsTimerStatus::SEC_9:
                case NgsTimerStatus::SEC_8:
                case NgsTimerStatus::SEC_7:
                case NgsTimerStatus::SEC_6:
                case NgsTimerStatus::SEC_5:
                case NgsTimerStatus::SEC_4:
                case NgsTimerStatus::SEC_3:
                case NgsTimerStatus::SEC_2:
                case NgsTimerStatus::SEC_1:
                    setSFCHigh_Performance_For_N_BrandConstantNgsTimerStat(
                        mNgsTimerMap[static_cast<NgsTimerStatus>(mNgsTimerStatus)]);
                    break;
                default:
                    setSFCHigh_Performance_For_N_BrandConstantNgsTimerStat(
                        SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::DISPLAY_OFF);
                    break;
            }
            if (mNgsTimerStatus >= kNgsTimerMinValue && mNgsTimerStatus <= kNgsTimerMaxValue) {
                setSFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat(
                    SFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat::VALUE);
                setSFCHigh_Performance_For_N_BrandConstantNgsErrTimerValue(mNgsTimerStatus);
            } else {
                setSFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat(
                    SFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat::DISPLAY_OFF);
            }
        } else {
            setSFCHigh_Performance_For_N_BrandConstantNgsTimerStat(
                SFCHigh_Performance_For_N_BrandConstantNgsTimerStat::DISPLAY_OFF);
            setSFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat(
                SFCHigh_Performance_For_N_BrandConstantNgsErrTimerStat::DISPLAY_OFF);
        }
    }

    // 5.2.2 N TRACK SENSE
    void updateConstantNTrackSense() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mNTrackSenseDisplayReqStatus == NTrackSenseDisplayReqStatus::N_TRACK_ON && mIgn1Status == true) {
                setSFCHigh_Performance_For_N_BrandConstantNTrackSenseStat(
                    SFCHigh_Performance_For_N_BrandConstantNTrackSenseStat::ON);
            } else if (mNTrackSenseDisplayReqStatus == NTrackSenseDisplayReqStatus::OFF && mIgn1Status == true) {
                setSFCHigh_Performance_For_N_BrandConstantNTrackSenseStat(
                    SFCHigh_Performance_For_N_BrandConstantNTrackSenseStat::OFF);
            } else {
                setSFCHigh_Performance_For_N_BrandConstantNTrackSenseStat(
                    SFCHigh_Performance_For_N_BrandConstantNTrackSenseStat::OFF);
            }
        }
    }

    // 5.2.3 G-FORCE (시그널 주기와는 별개로 표시 주기로 SFC 업데이트 되어야 함.)
    void updateConstantGForce() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mAccelerationLatStatus == AccelerationLatStatus::NORMAL &&
            mAccelerationLongStatus == AccelerationLongStatus::NORMAL && mDrivingOnStatus == DrivingOnStatus::ON) {
            setSFCHigh_Performance_For_N_BrandConstantGForceXcordValue(mPrivateGforceXCoordValue);
            setSFCHigh_Performance_For_N_BrandConstantGForceYcordValue(mPrivateGforceYCoordValue);
        } else {
            setSFCHigh_Performance_For_N_BrandConstantGForceXcordValue(0);
            setSFCHigh_Performance_For_N_BrandConstantGForceYcordValue(0);
        }
    }

    // 5.2.4.1 AUTO 표시
    void updateConstantAutoDisplay() {
        if (mIgn1Status == true) {
            if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO) {
                if ((mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) ||
                    (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::READY)) {
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                        SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::START);
                } else if (mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH ||
                           mPrivateLapTimerOperationStatus == LapTimerOperationStatus::FINISH_PREVIOUS) {
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                        SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::DISPLAY_OFF);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                        SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::READY);
                }
            } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::MANUAL) {
                setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                    SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::DISPLAY_OFF);
            } else {
                setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                    SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::DISPLAY_OFF);
            }
        } else {
            setSFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat(
                SFCHigh_Performance_For_N_BrandConstantLapTimeAutoStat::DISPLAY_OFF);
        }
    }

    // 5.2.4.2 랩타임 표시 (16ms 마다 실시간 랩타임 GUI update) (이력 : https://jira.ccos.dev/browse/EXNCP-33966)
    void onRepeatTimerUpdateLapTimeReal() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mPrivateLapTimerOperationStatus == LapTimerOperationStatus::RUNNING) {
            if ((mLapTimerAutoMeasureSetStatus != LapTimerAutoMeasureSetStatus::AUTO) ||
                (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO &&
                 mLapTimerAutoRunningStatus == LapTimerAutoRunningStatus::RUN)) {
                if (mLapTimerAccumPauseState == true) {
                    if (mIsLapTimerReset == true) {
                        // NOTE: Lap Timer Reset 후 Lap Timer 시작 전에 현재 시간을 가져옴
                        mLapTimerAccumStartPoint = std::chrono::steady_clock::now();
                        mIsLapTimerReset = false;
                    } else {
                        // NOTE: Lap Timer pause 후 재 시작 시 Pause 직전의 Lap Time 시간을 가져옴
                        mLapTimerAccumStartPoint =
                            mLapTimerAccumStartPoint + (std::chrono::steady_clock::now() - mLapTimerAccumPauseTime);
                    }
                    mLapTimerAccumPauseState = false;
                }
                mLapTimerAccumCurrPoint = std::chrono::steady_clock::now();

                auto lapTimerDuration = mLapTimerAccumCurrPoint - mLapTimerAccumStartPoint;
                std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(lapTimerDuration);
                lapTimerDuration -= h;
                std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(lapTimerDuration);
                lapTimerDuration -= m;
                std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(lapTimerDuration);
                lapTimerDuration -= s;
                std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(lapTimerDuration);

                DDebug() << "Real Time Lap Time = " << h.count() << ":" << m.count() << ":" << s.count() << ":" << ms.count();

                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealHourValue(static_cast<HUInt64>(h.count()));
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMinValue(static_cast<HUInt64>(m.count()));
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealSecValue(static_cast<HUInt64>(s.count()));
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMilliSecValue(static_cast<HUInt64>(ms.count()) /
                                                                                   kRangeForCanTransmission);

                // IMG_LapNumPrevious
                if (h.count() == 0) {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousHourValue(0);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousHourValue(static_cast<HUInt64>(h.count() - 1));
                }
                if (m.count() == 0) {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMinValue(0);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMinValue(static_cast<HUInt64>(m.count() - 1));
                }
                if (s.count() == 0) {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousSecValue(0);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousSecValue(static_cast<HUInt64>(s.count() - 1));
                }
                if (ms.count() == 0) {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMilliSecValue(0);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMilliSecValue(static_cast<HUInt64>(ms.count() - 1) /
                                                                                          kRangeForCanTransmission);
                }
            } else if (mLapTimerAutoMeasureSetStatus == LapTimerAutoMeasureSetStatus::AUTO &&
                       mLapTimerAutoRunningStatus != LapTimerAutoRunningStatus::RUN) {
                mLapTimerAccumPauseState = true;
                mLapTimerAccumPauseTime = mLapTimerAccumCurrPoint;
            } else {
                mLapTimerAccumStartPoint = std::chrono::steady_clock::now();
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealHourValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMinValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealSecValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMilliSecValue(0);
            }
        } else {
            mLapTimerAccumStartPoint = std::chrono::steady_clock::now();
            setSFCHigh_Performance_For_N_BrandConstantLapTimeRealHourValue(0);
            setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMinValue(0);
            setSFCHigh_Performance_For_N_BrandConstantLapTimeRealSecValue(0);
            setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMilliSecValue(0);
        }
        flush();
    }

    // 5.2.4.2 랩타임 표시
    // IMG_LapTimeDiffSignStat & IMG_LapTimeDiffVal
    void updateConstantLapTimeDiff() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mPrivateLapNumber > 2 && mPrivateLapNumber > mPrivatePrevLapNumber) {
                HUInt64 numberOfRecords = mPrivateLapTimeRecord.size();
                HUInt64 lapTimeDiffValue = 0;
                HUInt64 recentLapTime = 0;
                recentLapTime = mPrivateRecentLapTime[0] * kHourToMillisecond + mPrivateRecentLapTime[1] * kMinToMillisecond +
                                mPrivateRecentLapTime[2] * kSecondToMillisecond +
                                mPrivateRecentLapTime[3] * kRangeForCanTransmission;

                if (mLapTimerDifferTimeSetStatus == LapTimerDifferTimeSetStatus::BEST_LAP) {
                    if (numberOfRecords >= 1) {
                        HUInt64 privateLapTimeRecord = 0;
                        privateLapTimeRecord = mPrivateLapTimeRecord[0][0] * kHourToMillisecond +
                                               mPrivateLapTimeRecord[0][1] * kMinToMillisecond +
                                               mPrivateLapTimeRecord[0][2] * kSecondToMillisecond +
                                               mPrivateLapTimeRecord[0][3] * kRangeForCanTransmission;
                        if (recentLapTime >= privateLapTimeRecord) {
                            lapTimeDiffValue = recentLapTime - privateLapTimeRecord;
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat(
                                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat::PLUS);
                        } else {
                            lapTimeDiffValue = privateLapTimeRecord - recentLapTime;
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat(
                                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat::MINUS);
                        }
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffHourValue(lapTimeDiffValue / kHourToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffMinValue((lapTimeDiffValue % kHourToMillisecond) /
                                                                                      kMinToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSecValue(
                            ((lapTimeDiffValue % kHourToMillisecond) % kMinToMillisecond) / kSecondToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffMilliSecValue(
                            (((lapTimeDiffValue % kHourToMillisecond) % kMinToMillisecond) % kSecondToMillisecond) /
                            kRangeForCanTransmission);
                    } else {
                        DDebug() << "[Best_Lap]Recorded laptime does not exist";
                    }
                } else if (mLapTimerDifferTimeSetStatus == LapTimerDifferTimeSetStatus::LAST_LAP) {
                    if (numberOfRecords >= 1) {
                        HUInt64 previousLapTime = 0;
                        previousLapTime = mPrivatePreviousLapTime[0] * kHourToMillisecond +
                                          mPrivatePreviousLapTime[1] * kMinToMillisecond +
                                          mPrivatePreviousLapTime[2] * kSecondToMillisecond +
                                          mPrivatePreviousLapTime[3] * kRangeForCanTransmission;
                        if (recentLapTime >= previousLapTime) {
                            lapTimeDiffValue = recentLapTime - previousLapTime;
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat(
                                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat::PLUS);
                        } else {
                            lapTimeDiffValue = previousLapTime - recentLapTime;
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat(
                                SFCHigh_Performance_For_N_BrandConstantLapTimeDiffSignBestVsRecentStat::MINUS);
                        }
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffHourValue(lapTimeDiffValue / kHourToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffMinValue((lapTimeDiffValue % kHourToMillisecond) /
                                                                                      kMinToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffSecValue(
                            ((lapTimeDiffValue % kHourToMillisecond) % kMinToMillisecond) / kSecondToMillisecond);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeDiffMilliSecValue(
                            (((lapTimeDiffValue % kHourToMillisecond) % kMinToMillisecond) % kSecondToMillisecond) /
                            kRangeForCanTransmission);
                    } else {
                        DDebug() << "[Last_Lap]Recorded laptime does not exist";
                    }
                } else {
                    // nothing
                }
            }
        }
    }

    // 5.2.4.2 랩타임 표시
    void updateConstantLapTimeDisplay() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mPrivateLapNumber != 0 && mPrivateLapNumber != mPrivatePrevLapNumber) {
                HUInt64 numberOfRecords = mPrivateLapTimeRecord.size();
                numberOfRecords = mPrivateLapTimeRecord.size();

                if (numberOfRecords >= 1) {
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeBestHourValue(mPrivateLapTimeRecord[0][0]);
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeBestMinValue(mPrivateLapTimeRecord[0][1]);
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeBestSecValue(mPrivateLapTimeRecord[0][2]);
                    setSFCHigh_Performance_For_N_BrandConstantLapTimeBestMilliSecValue(mPrivateLapTimeRecord[0][3]);
                    if (mPrivateLapTimeRecord[0][4] < kLimitLapNum) {
                        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordBestValue(mPrivateLapTimeRecord[0][4]);
                    } else {
                        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordBestValue(kLimitLapNum);
                    }

                    DDebug() << "Lap[" << mPrivateLapNumber - 1 << "] 1등 기록 " << mPrivateLapTimeRecord[0][0] << ":"
                             << mPrivateLapTimeRecord[0][1] << ":" << mPrivateLapTimeRecord[0][2] << ":"
                             << mPrivateLapTimeRecord[0][3];

                    if (numberOfRecords >= 2) {
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondHourValue(mPrivateLapTimeRecord[1][0]);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondMinValue(mPrivateLapTimeRecord[1][1]);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondSecValue(mPrivateLapTimeRecord[1][2]);
                        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondMilliSecValue(mPrivateLapTimeRecord[1][3]);
                        if (mPrivateLapTimeRecord[1][4] < kLimitLapNum) {
                            setSFCHigh_Performance_For_N_BrandConstantLapNumRecordSecondValue(mPrivateLapTimeRecord[1][4]);
                        } else {
                            setSFCHigh_Performance_For_N_BrandConstantLapNumRecordSecondValue(kLimitLapNum);
                        }

                        DDebug() << "Lap[" << mPrivateLapNumber - 1 << "] 2등 기록 " << mPrivateLapTimeRecord[1][0] << ":"
                                 << mPrivateLapTimeRecord[1][1] << ":" << mPrivateLapTimeRecord[1][2] << ":"
                                 << mPrivateLapTimeRecord[1][3];

                        if (numberOfRecords >= 3) {
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdHourValue(mPrivateLapTimeRecord[2][0]);
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdMinValue(mPrivateLapTimeRecord[2][1]);
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdSecValue(mPrivateLapTimeRecord[2][2]);
                            setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdMilliSecValue(mPrivateLapTimeRecord[2][3]);
                            if (mPrivateLapTimeRecord[2][4] < kLimitLapNum) {
                                setSFCHigh_Performance_For_N_BrandConstantLapNumRecordThirdValue(mPrivateLapTimeRecord[2][4]);
                            } else {
                                setSFCHigh_Performance_For_N_BrandConstantLapNumRecordThirdValue(kLimitLapNum);
                            }

                            DDebug() << "Lap[" << mPrivateLapNumber - 1 << "] 3등 기록 " << mPrivateLapTimeRecord[2][0] << ":"
                                     << mPrivateLapTimeRecord[2][1] << ":" << mPrivateLapTimeRecord[2][2] << ":"
                                     << mPrivateLapTimeRecord[2][3];

                            if (numberOfRecords == 4) {
                                setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthHourValue(mPrivateLapTimeRecord[3][0]);
                                setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthMinValue(mPrivateLapTimeRecord[3][1]);
                                setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthSecValue(mPrivateLapTimeRecord[3][2]);
                                setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthMilliSecValue(mPrivateLapTimeRecord[3][3]);
                                if (mPrivateLapTimeRecord[3][4] < kLimitLapNum) {
                                    setSFCHigh_Performance_For_N_BrandConstantLapNumRecordFourthValue(
                                        mPrivateLapTimeRecord[3][4]);
                                } else {
                                    setSFCHigh_Performance_For_N_BrandConstantLapNumRecordFourthValue(kLimitLapNum);
                                }

                                DDebug() << "Lap[" << mPrivateLapNumber - 1 << "] 4등 기록 " << mPrivateLapTimeRecord[3][0] << ":"
                                         << mPrivateLapTimeRecord[3][1] << ":" << mPrivateLapTimeRecord[3][2] << ":"
                                         << mPrivateLapTimeRecord[3][3];
                            }
                        }
                    }
                }
            }
        }
    }

    void resetConstantBestLapRecord() {
        setSFCHigh_Performance_For_N_BrandConstantLapTimeBestHourValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeBestMinValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeBestSecValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeBestMilliSecValue(0);

        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondHourValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondMinValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondSecValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeSecondMilliSecValue(0);

        setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdHourValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdMinValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdSecValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeThirdMilliSecValue(0);

        setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthHourValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthMinValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthSecValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapTimeFourthMilliSecValue(0);

        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordBestValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordSecondValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordThirdValue(0);
        setSFCHigh_Performance_For_N_BrandConstantLapNumRecordFourthValue(0);
    }

    void updateConstantRealAndPreviousTime() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mPrivateLapNumber == 0) {
                setSFCHigh_Performance_For_N_BrandConstantLapNumRealValue(1);
            } else if (mPrivateLapNumber > 0) {
                if (mPrivateLapNumber < kLimitLapNum) {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumRealValue(mPrivateLapNumber);
                } else {
                    setSFCHigh_Performance_For_N_BrandConstantLapNumRealValue(kLimitLapNum);
                }
            } else {
                // no specification
            }

            if (mPrivateLapNumber != 0) {
                if (mPrivateLapNumber > mPrivatePrevLapNumber) {
                    if (mRepeatTimerLapTimeReal.isRunning() == false) {
                        if (mRepeatTimerLapTimeReal.start() == false) {
                            DWarning() << "[High_Performance_For_N_Brand] mRepeatTimerLapTimeReal Create Failed";
                        }
                    }
                }
            } else {
                mRepeatTimerLapTimeReal.stop();
                // NOTE: Lap Timer Auto mode에서 Lap 시간 기록 중 리셋 동작이 발생하였을 때를 체크하는 변수
                mIsLapTimerReset = true;
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealHourValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMinValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealSecValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapTimeRealMilliSecValue(0);

                // IMG_LapNumPrevious
                setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousHourValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMinValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousSecValue(0);
                setSFCHigh_Performance_For_N_BrandConstantLapNumPreviousMilliSecValue(0);
            }
        } else {
            mRepeatTimerLapTimeReal.stop();
        }
    }

    // 5.2.5.1 커스텀 세팅 표시 종류
    void updateConstantNCustomSettingCaseStatus() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat stat =
                SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::DISPLAY_OFF;
            if (mIgn1Status == true) {
                if (mDmicModeAppliedStatus != DmicModeAppliedStatus::ON) {
                    if (mNCustomModeOptionStatus == NCustomModeOptionStatus::ENGINE_MDPS) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::ENGINE_MDPS;
                    } else if (mNCustomModeOptionStatus == NCustomModeOptionStatus::ENGINE_MDPS_ECS) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::ENGINE_MDPS_ECS;
                    } else if (mNCustomModeOptionStatus == NCustomModeOptionStatus::ZERO &&
                               mNCustomMode2OptionStatus == NCustomMode2OptionStatus::MOTOR_MDPS_ECS) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::MOTOR_MDPS_ECS;
                    } else {
                        // DISPLAY_OFF
                    }
                } else {
                    if ((mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                         mConfigVehicleType == ConfigVehicleType::PHEV) &&
                        mDmicEngAppliedStatus != DmicEngAppliedStatus::OFF &&
                        mDmicMdpsAppliedStatus != DmicMdpsAppliedStatus::OFF &&
                        mDmicEcsAppliedStatus == DmicEcsAppliedStatus::OFF) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::ENGINE_MDPS;
                    } else if ((mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                                mConfigVehicleType == ConfigVehicleType::PHEV) &&
                               mDmicEngAppliedStatus != DmicEngAppliedStatus::OFF &&
                               mDmicMdpsAppliedStatus != DmicMdpsAppliedStatus::OFF &&
                               mDmicEcsAppliedStatus != DmicEcsAppliedStatus::OFF) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::ENGINE_MDPS_ECS;
                    } else if (mConfigVehicleType != ConfigVehicleType::ICV && mConfigVehicleType != ConfigVehicleType::HEV &&
                               mConfigVehicleType != ConfigVehicleType::PHEV &&
                               mDmicVcuAppliedStatus != DmicVcuAppliedStatus::OFF &&
                               mDmicMdpsAppliedStatus != DmicMdpsAppliedStatus::OFF &&
                               mDmicEcsAppliedStatus == DmicEcsAppliedStatus::OFF) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::MOTOR_MDPS;
                    } else if (mConfigVehicleType != ConfigVehicleType::ICV && mConfigVehicleType != ConfigVehicleType::HEV &&
                               mConfigVehicleType != ConfigVehicleType::PHEV &&
                               mDmicVcuAppliedStatus != DmicVcuAppliedStatus::OFF &&
                               mDmicMdpsAppliedStatus != DmicMdpsAppliedStatus::OFF &&
                               mDmicEcsAppliedStatus != DmicEcsAppliedStatus::OFF) {
                        stat = SFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat::MOTOR_MDPS_ECS;
                    } else {
                        // DISPLAY_OFF
                    }
                }
            }
            setSFCHigh_Performance_For_N_BrandConstantNCustomSettingCaseStat(stat);
        }
    }

    // 5.2.5.2.1 엔진 표시 사양
    void updateConstantNCustomEngineSettingStatus() {
        if (mConfigHighPerformance != ConfigHighPerformance::N && mConfigHighPerformance != ConfigHighPerformance::GT) {
            return;
        }
        SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat stat =
            SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::DISPLAY_OFF;
        if (mIgn1Status == true) {
            if (mNModeSelectStatus == NModeSelectStatus::N || mGtModeSelectStatus == GtModeSelectStatus::GT) {
                stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_3;
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM1 || mGtModeSelectStatus == GtModeSelectStatus::GT_MY) {
                if (mNCustom1EngineSetStatus == NCustom1EngineSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_3;
                } else if (mNCustom1EngineSetStatus == NCustom1EngineSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_2;
                } else if (mNCustom1EngineSetStatus == NCustom1EngineSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_1;
                } else if (mNCustom1EngineSetStatus == NCustom1EngineSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM2) {
                if (mNCustom2EngineSetStatus == NCustom2EngineSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_3;
                } else if (mNCustom2EngineSetStatus == NCustom2EngineSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_2;
                } else if (mNCustom2EngineSetStatus == NCustom2EngineSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_1;
                } else if (mNCustom2EngineSetStatus == NCustom2EngineSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }
        }
        setSFCHigh_Performance_For_N_BrandConstantNCustomEngineSettingStat(stat);
    }

    // 5.2.5.2.1 스티어링 표시 사양
    void updateConstantNCustomMdpsSettingStatus() {
        if (mConfigHighPerformance != ConfigHighPerformance::N && mConfigHighPerformance != ConfigHighPerformance::GT) {
            return;
        }
        SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat stat =
            SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::DISPLAY_OFF;
        if (mIgn1Status == true) {
            if (mNModeSelectStatus == NModeSelectStatus::N || mGtModeSelectStatus == GtModeSelectStatus::GT) {
                stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_3;
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM1 || mGtModeSelectStatus == GtModeSelectStatus::GT_MY) {
                if (mNCustom1MdpsSetStatus == NCustom1MdpsSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_3;
                } else if (mNCustom1MdpsSetStatus == NCustom1MdpsSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_2;
                } else if (mNCustom1MdpsSetStatus == NCustom1MdpsSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_1;
                } else if (mNCustom1MdpsSetStatus == NCustom1MdpsSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM2) {
                if (mNCustom2MdpsSetStatus == NCustom2MdpsSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_3;
                } else if (mNCustom2MdpsSetStatus == NCustom2MdpsSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_2;
                } else if (mNCustom2MdpsSetStatus == NCustom2MdpsSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_1;
                } else if (mNCustom2MdpsSetStatus == NCustom2MdpsSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }
        }
        setSFCHigh_Performance_For_N_BrandConstantNCustomMdpsSettingStat(stat);
    }

    // 5.2.5.2.1 서스펜션 표시 사양
    void updateConstantNCustomEcsSettingStatus() {
        if (mConfigHighPerformance != ConfigHighPerformance::N && mConfigHighPerformance != ConfigHighPerformance::GT) {
            return;
        }
        SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat stat =
            SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::DISPLAY_OFF;
        if (mIgn1Status == true) {
            if (mNModeSelectStatus == NModeSelectStatus::N || mGtModeSelectStatus == GtModeSelectStatus::GT) {
                stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_3;
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM1 || mGtModeSelectStatus == GtModeSelectStatus::GT_MY) {
                if (mNCustom1EcsSetStatus == NCustom1EcsSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_3;
                } else if (mNCustom1EcsSetStatus == NCustom1EcsSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_2;
                } else if (mNCustom1EcsSetStatus == NCustom1EcsSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_1;
                } else if (mNCustom1EcsSetStatus == NCustom1EcsSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM2) {
                if (mNCustom2EcsSetStatus == NCustom2EcsSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_3;
                } else if (mNCustom2EcsSetStatus == NCustom2EcsSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_2;
                } else if (mNCustom2EcsSetStatus == NCustom2EcsSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_1;
                } else if (mNCustom2EcsSetStatus == NCustom2EcsSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }
        }
        setSFCHigh_Performance_For_N_BrandConstantNCustomEcsSettingStat(stat);
    }

    // 5.2.5.2.1 모터 표시 사양
    void updateConstantNCustomMotorSettingStatus() {
        if (mConfigHighPerformance != ConfigHighPerformance::N && mConfigHighPerformance != ConfigHighPerformance::GT) {
            return;
        }
        SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat stat =
            SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::DISPLAY_OFF;
        if (mIgn1Status == true) {
            if (mNModeSelectStatus == NModeSelectStatus::N || mGtModeSelectStatus == GtModeSelectStatus::GT) {
                stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_4;
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM1 || mGtModeSelectStatus == GtModeSelectStatus::GT_MY) {
                if (mNCustom1MotorSetStatus == NCustom1MotorSetStatus::STEP4) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_4;
                } else if (mNCustom1MotorSetStatus == NCustom1MotorSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_3;
                } else if (mNCustom1MotorSetStatus == NCustom1MotorSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_2;
                } else if (mNCustom1MotorSetStatus == NCustom1MotorSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_1;
                } else if (mNCustom1MotorSetStatus == NCustom1MotorSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else if (mNModeSelectStatus == NModeSelectStatus::CUSTOM2) {
                if (mNCustom2MotorSetStatus == NCustom2MotorSetStatus::STEP4) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_4;
                } else if (mNCustom2MotorSetStatus == NCustom2MotorSetStatus::STEP3) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_3;
                } else if (mNCustom2MotorSetStatus == NCustom2MotorSetStatus::STEP2) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_2;
                } else if (mNCustom2MotorSetStatus == NCustom2MotorSetStatus::STEP1) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_1;
                } else if (mNCustom2MotorSetStatus == NCustom2MotorSetStatus::STEP0) {
                    stat = SFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat::STEP_0;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }
        }
        setSFCHigh_Performance_For_N_BrandConstantNCustomMotorSettingStat(stat);
    }

    // 5.3.3 NGS 1
    void updateEventNgs1() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mNgsActiveStatus == NgsActiveStatus::ON) {
            setSFCHigh_Performance_For_N_BrandEventNgs1Stat(SFCHigh_Performance_For_N_BrandEventNgs1Stat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandEventNgs1Stat(SFCHigh_Performance_For_N_BrandEventNgs1Stat::OFF);
        }
    }

    // 5.3.3 NGS 2
    void updateEventNgs2() {
        std::string eventID;
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true) {
            if (mNgsFaultStatus == NgsFaultStatus::WARN_ON) {
                eventID = "E24812";
            } else if (mNgsFaultStatus == NgsFaultStatus::WARN_ON_TIMER) {
                eventID = "E24813";
            } else {
                // nothing
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.High_Performance_For_N_Brand.Event.Ngs2.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCHigh_Performance_For_N_BrandEventNgs2Stat(SFCHigh_Performance_For_N_BrandEventNgs2Stat::OFF);
            flush();
        }
        if (!eventID.empty()) {
            setSFCHigh_Performance_For_N_BrandEventNgs2ID(eventID);
            setSFCHigh_Performance_For_N_BrandEventNgs2Stat(SFCHigh_Performance_For_N_BrandEventNgs2Stat::ON);
        }
    }

    // 5.3.4 N ROAD SENSE
    void updateEventNRoadSense() {
        if (mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) {
            if (mIgn1Status == true &&
                (mNModeSelectStatus != NModeSelectStatus::N && mNModeSelectStatus != NModeSelectStatus::CUSTOM1 &&
                 mNModeSelectStatus != NModeSelectStatus::CUSTOM2) &&
                mNroadsenseActiveStatus == NroadsenseActiveStatus::ON) {
                setSFCHigh_Performance_For_N_BrandEventNRoadSenseStat(SFCHigh_Performance_For_N_BrandEventNRoadSenseStat::ON);
                flush(true);
            } else if (mIgn1Status == false ||
                       ((mPrevNModeSelectStatus != NModeSelectStatus::N && mPrevNModeSelectStatus != NModeSelectStatus::CUSTOM1 &&
                         mPrevNModeSelectStatus != NModeSelectStatus::CUSTOM2) &&
                        (mNModeSelectStatus == NModeSelectStatus::N || mNModeSelectStatus == NModeSelectStatus::CUSTOM1 ||
                         mNModeSelectStatus == NModeSelectStatus::CUSTOM2))) {
                setSFCHigh_Performance_For_N_BrandEventNRoadSenseStat(SFCHigh_Performance_For_N_BrandEventNRoadSenseStat::OFF);
            } else {
                // do nothing...
            }
        }
    }

    // 5.3.5 CHECK BRAKEPAD
    void updateEventBrakepadCheck() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mPwiWarnStatus == PwiWarnStatus::WARN) {
            setSFCHigh_Performance_For_N_BrandEventBrakepadCheckStat(SFCHigh_Performance_For_N_BrandEventBrakepadCheckStat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandEventBrakepadCheckStat(SFCHigh_Performance_For_N_BrandEventBrakepadCheckStat::OFF);
        }
    }

    // 5.3.6 EKD(Electric Kick Drift) 동작 팝업
    void updateEventElectricKickDrift() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mEkdReadyStatus == EkdReadyStatus::ON) {
            setSFCHigh_Performance_For_N_BrandEventElectricKickDriftStat(
                SFCHigh_Performance_For_N_BrandEventElectricKickDriftStat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandEventElectricKickDriftStat(
                SFCHigh_Performance_For_N_BrandEventElectricKickDriftStat::OFF);
        }
    }

    // 5.4 사운드 로직
    void updateSoundLogic() {
        if ((mConfigHighPerformance == ConfigHighPerformance::N || mConfigHighPerformance == ConfigHighPerformance::GT) &&
            mIgn1Status == true && mShiftLightBeepSoundSetStatus == ShiftLightBeepSoundSetStatus::ENABLE &&
            mShiftLightIndReqStatus == ShiftLightIndReqStatus::STEP6) {
            setSFCHigh_Performance_For_N_BrandSoundShiftLightWarnStat(SFCHigh_Performance_For_N_BrandSoundShiftLightWarnStat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandSoundShiftLightWarnStat(
                SFCHigh_Performance_For_N_BrandSoundShiftLightWarnStat::OFF);
        }
    }

    // 5.3.1 BRAKE OVERRIDE
    void updateEventBrakeOverride() {
        if (mIgn1Status == true && mOverBoostStatus == OverBoostStatus::ON) {
            setSFCHigh_Performance_For_N_BrandEventBrakeOverrideStat(SFCHigh_Performance_For_N_BrandEventBrakeOverrideStat::ON);
        } else {
            setSFCHigh_Performance_For_N_BrandEventBrakeOverrideStat(SFCHigh_Performance_For_N_BrandEventBrakeOverrideStat::OFF);
        }
    }

    static constexpr uint32_t kTimerLapTimeRealInterval16ms = 16u;
    static constexpr uint32_t kTimerPrivateGforceInterval200ms = 200u;
    static constexpr uint32_t kTimerPrivateDisplayLapTimeDiffInterval5s = 5000u;
    static constexpr HUInt64 kNgsTimerMinValue = 1;
    static constexpr HUInt64 kNgsTimerMaxValue = 180;
    static constexpr HUInt64 kOverBoostOnAfter2SecDelay = 2000;
    static constexpr HUInt64 kLimitLapNum = 99;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    enum class ShiftLightStatus : HUInt64 {
        ENABLE,
        DISABLE
    };

    enum class NgsTimerStatus : HUInt64 {
        SEC_1 = 1,
        SEC_2,
        SEC_3,
        SEC_4,
        SEC_5,
        SEC_6,
        SEC_7,
        SEC_8,
        SEC_9,
        SEC_10,
        SEC_11,
        SEC_12,
        SEC_13,
        SEC_14,
        SEC_15,
        SEC_16,
        SEC_17,
        SEC_18,
        SEC_19,
        SEC_20
    };

    enum class DisplayLapTimeDiff : HUInt64 {
        OFF,
        ON
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using AccelerationLatStatus = ArgumentsGForceChanged::Input_AccelerationLatStatus;
    using AccelerationLongStatus = ArgumentsGForceChanged::Input_AccelerationLongStatus;
    using CreepOffIndiStatus = ArgumentsCreepOffIndiChanged::Input_CreepOffIndiStatus;
    using EvEndurancePowerLimitStatus = ArgumentsEnduranceRaceStatusChanged::Input_EvEndurancePowerLimitStatus;
    using TrackOptimizerOnOffStatus = ArgumentsEnduranceRaceStatusChanged::Input_TrackOptimizerOnOffStatus;
    using DriveModeSelectStatus = ArgumentsShiftLightChanged::Inter_DriveModeSelectStatus;
    using NModeSelectStatus = ArgumentsNModeSelectStatusChanged::Inter_NModeSelectStatus;
    using GtModeSelectStatus = ArgumentsGtModeSelectStatusChanged::Inter_GtModeSelectStatus;
    using ShiftLightNModeSetStatus = ArgumentsShiftLightChanged::Inter_ShiftLightNModeSetStatus;
    using ShiftLightEcoModeSetStatus = ArgumentsShiftLightChanged::Inter_ShiftLightEcoModeSetStatus;
    using ShiftLightCustomModeSetStatus = ArgumentsShiftLightChanged::Inter_ShiftLightCustomModeSetStatus;
    using ShiftLightNormalModeSetStatus = ArgumentsShiftLightChanged::Inter_ShiftLightNormalModeSetStatus;
    using ShiftLightSportModeSetStatus = ArgumentsShiftLightChanged::Inter_ShiftLightSportModeSetStatus;
    using NTrackSenseDisplayReqStatus = ArgumentsNTrackSenseDisplayReqChanged::Input_NTrackSenseDisplayReqStatus;
    using NCustom1EcsSetStatus = ArgumentsNCustomEcsSetStatusChanged::Input_NCustom1EcsSetStatus;
    using NCustom1EngineSetStatus = ArgumentsNCustomEngineSetStatusChanged::Input_NCustom1EngineSetStatus;
    using NCustom1MdpsSetStatus = ArgumentsNCustomMdpsSetStatusChanged::Input_NCustom1MdpsSetStatus;
    using NCustom2EcsSetStatus = ArgumentsNCustomEcsSetStatusChanged::Input_NCustom2EcsSetStatus;
    using NCustom2EngineSetStatus = ArgumentsNCustomEngineSetStatusChanged::Input_NCustom2EngineSetStatus;
    using NCustom2MdpsSetStatus = ArgumentsNCustomMdpsSetStatusChanged::Input_NCustom2MdpsSetStatus;
    using NgsActiveStatus = ArgumentsNgsEventChanged::Input_NgsActiveStatus;
    using NgsFaultStatus = ArgumentsNgsEventChanged::Input_NgsFaultStatus;
    using NroadsenseActiveStatus = ArgumentsInputNroadsenseActiveStatusChanged::Input_NroadsenseActiveStatus;
    using PwiWarnStatus = ArgumentsBrakepadCheckStatusChanged::Input_PwiWarnStatus;
    using ShiftLightIndReqStatus = ArgumentsSoundChanged::Input_ShiftLightIndReqStatus;
    using ShiftLightBeepSoundSetStatus = ArgumentsSoundChanged::Inter_ShiftLightBeepSoundSetStatus;
    using LapTimerAutoRunningStatus = ArgumentsLapTimerSignalChanged::Input_LapTimerAutoRunningStatus;
    using LapTimerAutoMeasureSetStatus = ArgumentsLapTimerSignalChanged::Inter_LapTimerAutoMeasureSetStatus;
    using LapTimerDifferTimeSetStatus = ArgumentsLapTimerSignalChanged::Inter_LapTimerDifferTimeSetStatus;
    using LapTimerPlayReq = ArgumentsLapTimerSignalChanged::Inter_LapTimerPlayReq;
    using LapTimerOperationReqFromAvn = ArgumentsLapTimerSignalChanged::Input_LapTimerOperationReqFromAvn;
    using EkdReadyStatus = ArgumentsEkdReadyStatusChanged::Input_EkdReadyStatus;
    using NCustom1MotorSetStatus = ArgumentsNCustomMotorSetStatusChanged::Input_NCustom1MotorSetStatus;
    using NCustom2MotorSetStatus = ArgumentsNCustomMotorSetStatusChanged::Input_NCustom2MotorSetStatus;
    using OutputLapTimerMeasuringStatusType =
        decltype(Vehicle.PT.High_Performance_For_N_Brand.Output_LapTimerMeasuringStatus)::TYPE;
    using OverBoostStatus = ArgumentsOverBoostStatusChanged::Input_OverBoostStatus;
    using LapTimerOperationStatus = SFCHigh_Performance_For_N_BrandConstantLaptimerOperationStat;
    using DrivingOnStatus = ArgumentsGForceChanged::Inter_DrivingOnStatus;
    using NCustomModeOptionStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_NCustomModeOptionStatus;
    using NCustomMode2OptionStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_NCustomMode2OptionStatus;
    using DmicModeAppliedStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_DmicModeAppliedStatus;
    using DmicEngAppliedStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_DmicEngAppliedStatus;
    using DmicMdpsAppliedStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_DmicMdpsAppliedStatus;
    using DmicEcsAppliedStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_DmicEcsAppliedStatus;
    using DmicVcuAppliedStatus = ArgumentsCustomSettingsDisplayTypesInfoChanged::Input_DmicVcuAppliedStatus;
    using NBatteryStatus = ArgumentsNBatteryStatusChanged::Input_NBatteryStatus;

    std::map<NgsTimerStatus, SFCHigh_Performance_For_N_BrandConstantNgsTimerStat> mNgsTimerMap;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIgn1Status = false;
    HBool mPrevIgn1Status = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;
    AccelerationLatStatus mAccelerationLatStatus = AccelerationLatStatus::ERROR;
    AccelerationLongStatus mAccelerationLongStatus = AccelerationLongStatus::ERROR;
    HUInt64 mAccelerationLatValue = 0;
    HUInt64 mAccelerationLongValue = 0;
    HDouble mPrivateGforceXCoordValue = 0.0;
    HDouble mPrivateGforceYCoordValue = 0.0;
    DrivingOnStatus mDrivingOnStatus = DrivingOnStatus::NONE;
    ssfs::SFCTimer<High_Performance_For_N_Brand> mRepeatTimerPrivateGforce;
    ShiftLightStatus mPrivateShiftLightStatus = ShiftLightStatus::DISABLE;
    CreepOffIndiStatus mCreepOffIndiStatus = CreepOffIndiStatus::OFF;
    EvEndurancePowerLimitStatus mEvEndurancePowerLimitStatus = EvEndurancePowerLimitStatus::OFF;
    TrackOptimizerOnOffStatus mTrackOptimizerOnOffStatus = TrackOptimizerOnOffStatus::OFF;
    DriveModeSelectStatus mDriveModeSelectStatus = DriveModeSelectStatus::NONE;
    NModeSelectStatus mNModeSelectStatus = NModeSelectStatus::OFF;
    NModeSelectStatus mPrevNModeSelectStatus = NModeSelectStatus::OFF;
    GtModeSelectStatus mGtModeSelectStatus = GtModeSelectStatus::OFF;
    ShiftLightNModeSetStatus mShiftLightNModeSetStatus = ShiftLightNModeSetStatus::NONE;
    ShiftLightEcoModeSetStatus mShiftLightEcoModeSetStatus = ShiftLightEcoModeSetStatus::NONE;
    ShiftLightCustomModeSetStatus mShiftLightCustomModeSetStatus = ShiftLightCustomModeSetStatus::NONE;
    ShiftLightNormalModeSetStatus mShiftLightNormalModeSetStatus = ShiftLightNormalModeSetStatus::NONE;
    ShiftLightSportModeSetStatus mShiftLightSportModeSetStatus = ShiftLightSportModeSetStatus::NONE;
    NTrackSenseDisplayReqStatus mNTrackSenseDisplayReqStatus = NTrackSenseDisplayReqStatus::UNHANDLED_TIMEOUT;
    HUInt64 mNgsTimerStatus = 0;
    NCustom1EcsSetStatus mNCustom1EcsSetStatus = NCustom1EcsSetStatus::STEP0;
    NCustom1EngineSetStatus mNCustom1EngineSetStatus = NCustom1EngineSetStatus::STEP0;
    NCustom1MdpsSetStatus mNCustom1MdpsSetStatus = NCustom1MdpsSetStatus::STEP0;
    NCustom2EcsSetStatus mNCustom2EcsSetStatus = NCustom2EcsSetStatus::STEP0;
    NCustom2EngineSetStatus mNCustom2EngineSetStatus = NCustom2EngineSetStatus::STEP0;
    NCustom2MdpsSetStatus mNCustom2MdpsSetStatus = NCustom2MdpsSetStatus::STEP0;
    NgsActiveStatus mNgsActiveStatus = NgsActiveStatus::OFF;
    NgsFaultStatus mNgsFaultStatus = NgsFaultStatus::WARN_OFF;
    NroadsenseActiveStatus mNroadsenseActiveStatus = NroadsenseActiveStatus::OFF;
    PwiWarnStatus mPwiWarnStatus = PwiWarnStatus::OFF;
    ShiftLightIndReqStatus mShiftLightIndReqStatus = ShiftLightIndReqStatus::OFF;
    ShiftLightBeepSoundSetStatus mShiftLightBeepSoundSetStatus = ShiftLightBeepSoundSetStatus::NONE;

    LapTimerOperationStatus mPrivateLapTimerOperationStatus = LapTimerOperationStatus::OFF;
    LapTimerOperationStatus mPrivatePrevLapTimerOperationStatus = LapTimerOperationStatus::OFF;
    DisplayLapTimeDiff mPrivateDisplayLapTimeDiff = DisplayLapTimeDiff::OFF;
    HUInt64 mPrivateLapNumber = 0;
    HUInt64 mPrivatePrevLapNumber = 0;
    LapTimerAutoRunningStatus mLapTimerAutoRunningStatus = LapTimerAutoRunningStatus::OFF;
    LapTimerAutoMeasureSetStatus mLapTimerAutoMeasureSetStatus = LapTimerAutoMeasureSetStatus::NONE;
    LapTimerAutoMeasureSetStatus mPrevLapTimerAutoMeasureSetStatus = LapTimerAutoMeasureSetStatus::NONE;
    LapTimerDifferTimeSetStatus mLapTimerDifferTimeSetStatus = LapTimerDifferTimeSetStatus::NONE;
    LapTimerPlayReq mLapTimerPlayReq = LapTimerPlayReq::DEFAULT;
    LapTimerPlayReq mPrevLapTimerPlayReq = LapTimerPlayReq::DEFAULT;
    LapTimerOperationReqFromAvn mLapTimerOperationReqFromAvn = LapTimerOperationReqFromAvn::DEFAULT;
    LapTimerOperationReqFromAvn mPrevLapTimerOperationReqFromAvn = LapTimerOperationReqFromAvn::DEFAULT;
    HUInt64 mLapTimerAutoLapNumberValue = 0;
    HUInt64 mLapTimerAutoHourValue = 0;
    HUInt64 mLapTimerAutoMinuteValue = 0;
    HUInt64 mLapTimerAutoSecondValue = 0;
    HUInt64 mLapTimerAutoMilliSecondValue = 0;
    ssfs::SFCTimer<High_Performance_For_N_Brand> mRepeatTimerLapTimeReal;
    ssfs::SFCTimer<High_Performance_For_N_Brand> mOneShotTimerPrivateDisplayLapTimeDiff;
    //[0]~[3]: time array, [4]: Lap Number
    std::array<HUInt64, 5> mPrivateLapTimePreAccum{
        0,
    };
    std::array<HUInt64, 5> mPrivateRecentLapTime{
        0,
    };
    std::array<HUInt64, 5> mPrivatePreviousLapTime{
        0,
    };
    std::vector<std::vector<HUInt64>> mPrivateLapTimeRecord;

    // 첫 타이머 작동 시점
    std::chrono::time_point<std::chrono::steady_clock> mLapTimerAccumStartPoint;
    // 현재 시점
    std::chrono::time_point<std::chrono::steady_clock> mLapTimerAccumCurrPoint;
    // 랩타이머 일시정지 후 경과 시간
    std::chrono::time_point<std::chrono::steady_clock> mLapTimerAccumPauseTime;

    HBool mLapTimerAccumPauseState = false;

    HBool mOnceFlag = false;
    EkdReadyStatus mEkdReadyStatus = EkdReadyStatus::OFF;
    NCustom1MotorSetStatus mNCustom1MotorSetStatus = NCustom1MotorSetStatus::STEP0;
    NCustom2MotorSetStatus mNCustom2MotorSetStatus = NCustom2MotorSetStatus::STEP0;
    OverBoostStatus mOverBoostStatus = OverBoostStatus::OFF;

    HBool mAEMInitCompleteFlag = false;

    static constexpr HUInt64 kHourToMillisecond = 60 * 60 * 1000;
    static constexpr HUInt64 kMinToMillisecond = 60 * 1000;
    static constexpr HUInt64 kSecondToMillisecond = 1000;
    static constexpr HUInt64 kRangeForCanTransmission = 10;  // For CAN communication range (EXNCP-32410)

    HBool mIsLapTimerReset = false;

    NCustomModeOptionStatus mNCustomModeOptionStatus = NCustomModeOptionStatus::OFF;
    NCustomMode2OptionStatus mNCustomMode2OptionStatus = NCustomMode2OptionStatus::OFF;
    DmicModeAppliedStatus mDmicModeAppliedStatus = DmicModeAppliedStatus::OFF;
    DmicEngAppliedStatus mDmicEngAppliedStatus = DmicEngAppliedStatus::OFF;
    DmicMdpsAppliedStatus mDmicMdpsAppliedStatus = DmicMdpsAppliedStatus::OFF;
    DmicEcsAppliedStatus mDmicEcsAppliedStatus = DmicEcsAppliedStatus::OFF;
    DmicVcuAppliedStatus mDmicVcuAppliedStatus = DmicVcuAppliedStatus::OFF;
    NBatteryStatus mNBatteryStatus = NBatteryStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_High_Performance_For_N_Brand_H
