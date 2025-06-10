/**
 * @file Trip_Info.hpp
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
#ifndef SFSS_Trip_Info_H
#define SFSS_Trip_Info_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include <chrono>
#include <Vehicle.h>
#include "Trip_InfoBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 47.0.0
// Reference : [CD180] Trip Info. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Trip_Info : public Trip_InfoBase {
public:
    Trip_Info() = default;
    ~Trip_Info() override = default;
    Trip_Info(const Trip_Info& other) = delete;
    Trip_Info(Trip_Info&& other) noexcept = delete;
    Trip_Info& operator=(const Trip_Info& other) = delete;
    Trip_Info& operator=(Trip_Info&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1 주행가능거리 표시
        setSFCTrip_InfoEventDistanceToEmptyDisplayID("E00801");
        setSFCTrip_InfoEventDistanceToEmptyDisplayStat(SFCTrip_InfoEventDistanceToEmptyDisplayStat::ON);

        if (mOneShotTimerIgnitionOff180sStatus.create(kTimerIgnitionOffInterval180s, this,
                                                      &Trip_Info::onOneShotTimerIgnitionOff180sTimeout, false) == false) {
            DWarning() << "[Trip_Info] mOneShotTimerIgnitionOff180sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAccumFuelEcoKmL10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAccumFuelEcoKmL10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumFuelEcoKmL10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAccumFuelEcoKmKwh10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAccumHydrogenEco10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAccumHydrogenEco10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumHydrogenEco10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAccumVehicleSpeedKph10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateDriveFuelEcoKmL10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateDriveFuelEcoKmL10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveFuelEcoKmL10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateDriveFuelEcoKmKwh10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateDriveHydrogenEco10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateDriveHydrogenEco10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveHydrogenEco10sStatus Create Failed";
        }

        if (mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerInterDriveInfoVehicleSpeedKph10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAfterRefuelFuelEcoKmL10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAfterRefuelHydrogenEco10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus Create Failed";
        }

        if (mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus.create(
                kTimerTripInfoInterval10s, this, &Trip_Info::onRepeatTimerPrivateAfterRefuelVehicleSpeedKph10s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus Create Failed";
        }

        if (mRepeatTimer500msStatus.create(kTimerStatusInterval500ms, this, &Trip_Info::onRepeatTimer500msTimeout, true) ==
            false) {
            DWarning() << "[Trip_Info] mRepeatTimer500msStatus Create Failed";
        }

        if (mRepeatTimer1sStatus.create(kTimerStatusInterval1s, this, &Trip_Info::onRepeatTimer1sTimeout, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimer1sStatus Create Failed";
        }

        if (mRepeatTimerDrivingTimeTotal1sStatus.create(kTimerStatusInterval1s, this,
                                                        &Trip_Info::onRepeatTimerDrivingTimeTotal1sTimeout, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerDrivingTimeTotal1sStatus Create Failed";
        }

        if (mRepeatTimerPrivateTerm1sAvgStatus.create(kTimerStatusInterval1s, this,
                                                      &Trip_Info::onRepeatTimerPrivateTermAvgTimer1s, true) == false) {
            DWarning() << "[Trip_Info] mRepeatTimerPrivateTerm1sAvgStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    // Subscribe Logic
    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        _DDebug() << "Ign ON_0ms";
        mIgninitState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        mIgnitionState = IgnitionState::ON;
        mBeforeIgnitionOff180sStatus = false;
        if (mRepeatTimer1sStatus.restart() == false) {
            DWarning() << "[Trip_Info] mRepeatTimer1sStatus Restart Failed";
        }
        mPrevOdometerRefreshTime = std::chrono::steady_clock::now();
        updatePrivateDrivingTimeTotal();
        updatePrivateADMStatus();
        updatePrivateAccumFuelEcoKmL();
        updatePrivateAccumFuelEcoKmKwh();
        updatePrivateAccumHydrogenEco();
        updatePrivateAccumVehicleSpeedKph();
        updatePrivateDriveFuelEcoKmL();
        updatePrivateDriveFuelEcoKmKwh();
        updatePrivateDriveHydrogenEco();
        updateInterDriveInfoVehicleSpeedKph();
        updatePrivateAfterRefuelFuelEcoKmL();
        updatePrivateAfterRefuelFuelEcoKmKwh();
        updatePrivateAfterRefuelHydrogenEco();
        updatePrivateAfterRefuelVehicleSpeedKph();
        updatePrivateInstFuelEcoCalModeStatus();
        updateOutputAvgFuelEconomyUnitStatus();
        updatePrivateTermAvgTimer();
        updatePrivateAfterDriveFuelEcoValue();
        updatePrivateDriveInfoResetStatus();
        updateOutputDrvInfoAvgFuelConsValue();
        updateOutputDrvInfoAvgFuelConsDigit();
        updateElecDTE();
        updateDTEDashBlinkDisplayForEv();
        updateConstantDTEGuideFunctionStatus();
        updateConstantDTEGuideLowBoundValue();
        updateConstantDTEGuideUpperBoundValue();
        updateConstantDTEGuideMainDtePositionValue();
        updateConstantDTEGuideCoachPositionValue();
        updateConstantDTEGuideCoachValue();
        updateConstantDTEGuideCoachValueAvailableStat();
        updateOutputEvDteValue();
        updateOutputTotalDTEValue();
        updateOutputTripUnitStatus();
        flushActuatorOutput();
        mPrevIgnitionState = mIgnitionState;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        _DDebug() << "Ign OFF_0ms";
        mIgnitionState = IgnitionState::OFF;
        mBeforeIgnitionOff180sStatus = false;
        mRepeatTimer1sStatus.stop();
        updatePrivateDrivingTimeTotal();
        updatePrivateADMStatus();
        updatePrivateAccumFuelEcoKmL();
        updatePrivateAccumFuelEcoKmKwh();
        updatePrivateAccumHydrogenEco();
        updatePrivateAccumVehicleSpeedKph();
        updatePrivateDriveFuelEcoKmL();
        updatePrivateDriveFuelEcoKmKwh();
        updatePrivateDriveHydrogenEco();
        updateInterDriveInfoVehicleSpeedKph();
        updatePrivateAfterRefuelFuelEcoKmL();
        updatePrivateAfterRefuelFuelEcoKmKwh();
        updatePrivateAfterRefuelHydrogenEco();
        updatePrivateAfterRefuelVehicleSpeedKph();
        updatePrivateInstFuelEcoCalModeStatus();
        updatePrivateTermAvgTimer();
        updatePrivateAfterDriveFuelEcoValue();
        updatePrivateDriveInfoResetStatus();
        updateInstantaneousFuelEconomy();
        updateInstFuelEconomyInputType();
        updateAccumulatedInfoDistance();
        updateAccumulatedInfoTime();
        updateAccumulatedInfoSpeed();
        updateAccumulatedFuelEconomy();
        updateDrivingInfoDistance();
        updateDrivingInfoTime();
        updateDrivingInfoSpeed();
        updateAfterRefuelDistance();
        updateAfterRefuelTime();
        updateAfterRefuelSpeed();
        updateAfterRefuelFuelEconomy();
        updateElecDTE();
        flushActuatorOutput();
        mPrevIgnitionState = mIgnitionState;
        saveDataToMemory();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        DDebug() << "Inter_ConfigVehicleType : " << static_cast<HUInt64>(mInter_ConfigVehicleType);
        saveTheStartPointDriveStart();
        updatePrivateInstFuelEcoCalModeStatus();
        updatePrivateTermAvgTimer();
        updatePrivateAfterDriveFuelEcoValue();
        updatePrivateDriveInfoResetStatus();
        updateOutputDrvInfoAvgFuelConsValue();
        updateInstantaneousFuelEconomy();
        updateInstFuelEconomyInputType();
        updateFuelEcoUnit();
        updateDTESymbol();
        updateOutputEvDteValue();
        updateOutputTotalDTEValue();
        flushActuatorOutput();
    }

    void onFuelConsumptionValueChanged(const ArgumentsFuelConsumptionValueChanged& args) {
        // PERIODIC_DB_CYCLETIME 10ms
        mInput_FuelConsumptionValue = args.mInput_FuelConsumptionValue;
        updatePrivateFuelConsumptionTotal();
    }

    void onInputEvDTEValuePerKmChanged(const ArgumentsInputEvDTEValuePerKmChanged& args) {
        if (ISTIMEOUT(args.mInput_EvDTEValuePerKm) == true) {
            mInput_EvDTEValuePerKm = 0;
            mElecDTEStatus = ElecDTEStatus::MESSAGE_TIMEOUT;
        } else {
            mInput_EvDTEValuePerKm = args.mInput_EvDTEValuePerKm;
            mElecDTEStatus = ElecDTEStatus::VALUE;
        }
        DDebug() << "Input_EvDTEValuePerKm : " << static_cast<HUInt64>(mInput_EvDTEValuePerKm);

        updateElecDTE();
        updateDTEDashBlinkDisplayForEv();
        updateOutputEvDteValue();
        updateOutputTotalDTEValue();
        flushActuatorOutput();
    }

    void onElectricityConsumptionChanged(const ArgumentsElectricityConsumptionChanged& args) {
        // PERIODIC_DB_CYCLETIME 100ms
        mInput_EvBatteryCurrentValue = args.mInput_EvBatteryCurrentValue;
        mInput_EvBatteryVoltageValue = args.mInput_EvBatteryVoltageValue;
        updatePrivateElecFuelConsumptionTotal();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mInter_TripDistanceUnit = args.mInter_TripDistanceUnit;
        DDebug() << "Inter_TripDistanceUnit : " << static_cast<HUInt64>(mInter_TripDistanceUnit);
        updateOutputAvgFuelEconomyUnitStatus();
        updateFuelEcoUnit();
        updateTripDistanceUnit();
        updateTripVehicleSpeedUnit();
        updateOdometer();
        updatePrivateAccumFuelEconomyValue();
        updateInterDriveInfoFuelEcoValue();
        updatePrivateAfterRefuelFuelEconomyValue();
        updateAccumulatedInfoSpeed();
        updateDrivingInfoSpeed();
        updateAfterRefuelSpeed();
        updateInterDriveInfoDistanceKmAndMi();
        updatePrivateInstFuelEcoInput1();
        updatePrivateInstFuelEcoInput2();
        updatePrivateInstFuelEcoInput3();
        updatePrivateAfterRefuelDistanceKmAndMi();
        updatePrivateAccumDistanceKmAndMi();
        updatePrivateTermAvgFuelEconomyValue();
        updateOutputTermAvgFuelConsValue();
        updateInterTermAvgFuelConsValue();
        updateElecDTE();
        updateConstantDTEGuideLowBoundValue();
        updateConstantDTEGuideUpperBoundValue();
        updateConstantDTEGuideCoachValue();
        updateOutputEvDteValue();
        updateOutputTotalDTEValue();
        updateOutputTripUnitStatus();
        flushActuatorOutput();
    }

    void onInterFuelEcoUnitKmSetStatusChanged(const ArgumentsInterFuelEcoUnitKmSetStatusChanged& args) {
        mInter_FuelEcoUnitKmSetStatus = args.mInter_FuelEcoUnitKmSetStatus;
        DDebug() << "Inter_FuelEcoUnitKmSetStatus : " << static_cast<HUInt64>(mInter_FuelEcoUnitKmSetStatus);
        updateOutputAvgFuelEconomyUnitStatus();
        updateFuelEcoUnit();
        updatePrivateAfterRefuelFuelEconomyValue();
        updatePrivateAccumFuelEconomyValue();
        updateInterDriveInfoFuelEcoValue();
        updatePrivateTermAvgFuelEconomyValue();
        updatePrivateInstFuelEcoInput1();
        updatePrivateInstFuelEcoInput2();
        updatePrivateInstFuelEcoInput3();
        updateOutputTermAvgFuelConsValue();
        updateInterTermAvgFuelConsValue();
        flushActuatorOutput();
    }

    void onInterFuelEcoUnitMiSetStatusChanged(const ArgumentsInterFuelEcoUnitMiSetStatusChanged& args) {
        mInter_FuelEcoUnitMiSetStatus = args.mInter_FuelEcoUnitMiSetStatus;
        DDebug() << "Inter_FuelEcoUnitMiSetStatus : " << static_cast<HUInt64>(mInter_FuelEcoUnitMiSetStatus);
        updateOutputAvgFuelEconomyUnitStatus();
        updateFuelEcoUnit();
        updatePrivateAfterRefuelFuelEconomyValue();
        updatePrivateAccumFuelEconomyValue();
        updateInterDriveInfoFuelEcoValue();
        updatePrivateTermAvgFuelEconomyValue();
        updatePrivateInstFuelEcoInput1();
        updatePrivateInstFuelEcoInput2();
        updatePrivateInstFuelEcoInput3();
        updateOutputTermAvgFuelConsValue();
        updateInterTermAvgFuelConsValue();
        flushActuatorOutput();
    }

    void onInterDisplayLanguageStatusChanged(const ArgumentsInterDisplayLanguageStatusChanged& args) {
        mInter_DisplayLanguageStatus = args.mInter_DisplayLanguageStatus;
        DDebug() << "Inter_DisplayLanguageStatus : " << static_cast<HUInt64>(mInter_DisplayLanguageStatus);
        updateTripInfoSubTitleType();
    }

    void onInterActuatorVehicleSpeedChanged(const ArgumentsInterActuatorVehicleSpeedChanged& args) {
        // PERIODIC_DB_CYCLETIME 10ms
        if (updateValueIfValid(mInter_ActualVehicleSpeed, args.mInter_ActualVehicleSpeed)) {
            if (almostEqual(mInter_ActualVehicleSpeed, 0.0) == false) {
                updatePrivateOdometerTotal();
            } else {
                mIsValidPrevOdometerRefreshTime = false;
            }
            updatePrivateInstFuelEcoCalModeStatus();
            mPrevInter_ActualVehicleSpeed = mInter_ActualVehicleSpeed;
        }
    }

    void onInterEngineOnStatusChanged(const ArgumentsInterEngineOnStatusChanged& args) {
        mInter_EngineOnStatus = args.mInter_EngineOnStatus;
        if (mPrevInter_EngineOnStatus == Inter_EngineOnStatus::OFF && mInter_EngineOnStatus == Inter_EngineOnStatus::ON) {
            mInter_EngineOnTransStatus = true;
        } else {
            mInter_EngineOnTransStatus = false;
        }
        DDebug() << "Inter_EngineOnStatus : " << static_cast<HUInt64>(mInter_EngineOnStatus);
        updatePrivateDrivingTimeTotal();
        // updatePrivateTermAvgSignalTxModeStatus();
        // updatePrivateTermAvgCalModeStatus();
        updatePrivateTermAvgTimer();
        flushActuatorOutput();
        mPrevInter_EngineOnStatus = mInter_EngineOnStatus;
    }

    void onInterISGActiveStatusChanged(const ArgumentsInterISGActiveStatusChanged& args) {
        mInter_ISGActiveStatus = args.mInter_ISGActiveStatus;
        DDebug() << "Inter_ISGActiveStatus : " << static_cast<HUInt64>(mInter_ISGActiveStatus);
        updatePrivateDrivingTimeTotal();
    }

    void onInterFuelDetectRealtimeModeStatusChanged(const ArgumentsInterFuelDetectRealtimeModeStatusChanged& args) {
        mInter_FuelDetectRealtimeModeStatus = args.mInter_FuelDetectRealtimeModeStatus;
        DDebug() << "Inter_FuelDetectRealtimeModeStatus : " << static_cast<HUInt64>(mInter_FuelDetectRealtimeModeStatus);
        saveTheAfterRefuelStartPoint();
    }

    void onInputH2RefuelDetectionStatusChanged(const ArgumentsInputH2RefuelDetectionStatusChanged& args) {
        mInput_H2RefuelDetectionStatus = args.mInput_H2RefuelDetectionStatus;
        DDebug() << "Input_H2RefuelDetectionStatus : " << static_cast<HUInt64>(mInput_H2RefuelDetectionStatus);
        saveTheAfterRefuelStartPoint();
    }

    void onInterAfterRefuelInfoManualResetReqChanged(const ArgumentsInterAfterRefuelInfoManualResetReqChanged& args) {
        mInter_AfterRefuelInfoManualResetReq = args.mInter_AfterRefuelInfoManualResetReq;
        DDebug() << "Inter_AfterRefuelInfoManualResetReq : " << static_cast<HUInt64>(mInter_AfterRefuelInfoManualResetReq);
        saveTheAfterRefuelStartPoint();
        updatePrivateAfterRefuelDistanceKmAndMi();
        updatePrivateAfterRefuelTime();
        mAfterRefuelDisplayStatus = false;
        mPrevInter_AfterRefuelInfoManualResetReq = mInter_AfterRefuelInfoManualResetReq;
    }

    void onInputDoorOpenDriverSideStatusChanged(const ArgumentsInputDoorOpenDriverSideStatusChanged& args) {
        mInput_DoorOpenDriverSideStatus = args.mInput_DoorOpenDriverSideStatus;
        mInput_DoorOpenDriverSideStatusFromSBCM = args.mInput_DoorOpenDriverSideStatusFromSBCM;
        DDebug() << "Input_DoorOpenDriverSideStatus : " << static_cast<HUInt64>(mInput_DoorOpenDriverSideStatus);
        updatePrivateADMStatus();
        flushActuatorOutput();
    }

    void onInputVehicleReadyStatusChanged(const ArgumentsInputVehicleReadyStatusChanged& args) {
        mInput_VehicleReadyStatus = args.mInput_VehicleReadyStatus;
        DDebug() << "Input_VehicleReadyStatus : " << static_cast<HUInt64>(mInput_VehicleReadyStatus);
        updatePrivateElecFuelConsumptionTotal();
    }

    void onAccumInfoManualResetReqChanged(const ArgumentsAccumInfoManualResetReqChanged& args) {
        mInter_AccumInfoManualResetReq = args.mInter_AccumInfoManualResetReq;
        DDebug() << "Inter_AccumInfoManualResetReq : " << static_cast<HUInt64>(mInter_AccumInfoManualResetReq);
        saveTheAccumStartPoint();
        updatePrivateAccumDistanceKmAndMi();
        updatePrivateAccumTime();
        mAccumDisplayStatus = false;
        mPrevInter_AccumInfoManualResetReq = mInter_AccumInfoManualResetReq;
    }

    void onDriveInfoManualResetReqChanged(const ArgumentsDriveInfoManualResetReqChanged& args) {
        mInter_DriveInfoManualResetReq = args.mInter_DriveInfoManualResetReq;
        DDebug() << "Inter_DriveInfoManualResetReq : " << static_cast<HUInt64>(mInter_DriveInfoManualResetReq);
        saveTheStartPointDriveStart();
        updateInterDriveInfoDistanceKmAndMi();
        updateInterDriveInfoTime();
        mDriveDisplayStatus = false;
        mPrevInter_DriveInfoManualResetReq = mInter_DriveInfoManualResetReq;
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitCompleteFlag, args.mInter_AEMInitializeComplete) == true) {
            DDebug() << "Inter_AEMInitializeComplete : " << static_cast<HBool>(args.mInter_AEMInitializeComplete);
            if (mAEMInitCompleteFlag) {
                // 4.9 계산값 불러오기 (사양서상 B+ On시 메모리 로드 기능 구현)
                HDouble odometer = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_OdometerValue, odometer);
                mPrivate_OdometerTotal = odometer;
                updateOdometer();
                HDouble afterDriveFuelEcoValue = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory_IGN.Inter_AfterDriveFuelEcoValue, afterDriveFuelEcoValue);
                if (isValidPositive(afterDriveFuelEcoValue) == true) {
                    mPrivate_AfterDriveFuelEcoValue = afterDriveFuelEcoValue;
                    updateOutputDrvInfoAvgFuelConsValue();
                    updateOutputDrvInfoAvgFuelConsDigit();
                    updateInterDriveInfoAvgFuelConsValue();
                }
                HDouble fuelConsumpt = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_FuelConsumptValue, fuelConsumpt);
                if (isValidPositive(fuelConsumpt) == true) {
                    mPrivate_FuelConsumptionTotal = fuelConsumpt;
                    updatePrivateAccumFuelEcoKmL();
                    updatePrivateDriveFuelEcoKmL();
                    updatePrivateAfterRefuelFuelEcoKmL();
                }
                HDouble elecFuelConsumpt = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_ElecFuelConsumptValue, elecFuelConsumpt);
                if (isValidPositive(elecFuelConsumpt) == true) {
                    mPrivate_ElecFuelConsumptionTotal = elecFuelConsumpt;
                    updatePrivateAccumFuelEcoKmKwh();
                    updatePrivateDriveFuelEcoKmKwh();
                    updatePrivateAfterRefuelFuelEcoKmKwh();
                }
                HDouble hydrogenFuelConsumpt = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_HydrogenFuelConsumption, hydrogenFuelConsumpt);
                if (isValidPositive(hydrogenFuelConsumpt) == true) {
                    mPrivate_HydrogenConsumptionTotal = hydrogenFuelConsumpt;
                    updatePrivateAccumFuelEcoKmKwh();
                    updatePrivateDriveFuelEcoKmKwh();
                    updatePrivateAfterRefuelFuelEcoKmKwh();
                }
                HUInt64 drivingTime = 0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DrivingTimeValue, drivingTime);
                if (drivingTime > 0) {
                    mPrivate_DrivingTimeTotal = drivingTime;
                    updatePrivateAccumVehicleSpeedKph();
                    updateInterDriveInfoVehicleSpeedKph();
                    updatePrivateAfterRefuelVehicleSpeedKph();
                }
                HDouble accumStartDistance = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AccumStartDistanceValue, accumStartDistance);
                if (isValidPositive(accumStartDistance) == true) {
                    mPrivate_AccumStartDistance = accumStartDistance;
                }
                HUInt64 accumStartTime = 0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AccumStartTimeValue, accumStartTime);
                if (accumStartTime > 0) {
                    mPrivate_AccumStartTime = accumStartTime;
                    updatePrivateAccumVehicleSpeedKph();
                }
                HDouble accumStartFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AccumStartFuelConsValue, accumStartFuelCons);
                if (isValidPositive(accumStartFuelCons) == true) {
                    mPrivate_AccumStartFuelCons = accumStartFuelCons;
                    updatePrivateAccumFuelEcoKmL();
                }
                HDouble accumStartElecFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AccumStartElecFuelConsValue, accumStartElecFuelCons);
                if (isValidPositive(accumStartElecFuelCons) == true) {
                    mPrivate_AccumStartElecFuelCons = accumStartElecFuelCons;
                    updatePrivateAccumFuelEcoKmKwh();
                }
                HDouble accumStartHydrogenCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AccumStartHydrogenConsValue, accumStartHydrogenCons);
                if (isValidPositive(accumStartHydrogenCons) == true) {
                    mPrivate_AccumStartHydrogenCons = accumStartHydrogenCons;
                    updatePrivateAccumHydrogenEco();
                }
                HDouble driveStartDistance = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DriveStartDistanceValue, driveStartDistance);
                if (isValidPositive(driveStartDistance) == true) {
                    mPrivate_DriveStartDistance = driveStartDistance;
                }
                HUInt64 driveStartTime = 0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DriveStartTimeValue, driveStartTime);
                if (driveStartTime > 0) {
                    mPrivate_DriveStartTime = driveStartTime;
                    updateInterDriveInfoVehicleSpeedKph();
                }
                HDouble driveStartFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DriveStartFuelConsValue, driveStartFuelCons);
                if (isValidPositive(driveStartFuelCons) == true) {
                    mPrivate_DriveStartFuelCons = driveStartFuelCons;
                    updatePrivateDriveFuelEcoKmL();
                }
                HDouble driveStartElecFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DriveStartElecFuelConsValue, driveStartElecFuelCons);
                if (isValidPositive(driveStartElecFuelCons) == true) {
                    mPrivate_DriveStartElecFuelCons = driveStartElecFuelCons;
                    updatePrivateDriveFuelEcoKmKwh();
                }
                HDouble driveStartHydrogenCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_DriveStartHydrogenConsValue, driveStartHydrogenCons);
                if (isValidPositive(driveStartElecFuelCons) == true) {
                    mPrivate_DriveStartHydrogenCons = driveStartHydrogenCons;
                    updatePrivateDriveHydrogenEco();
                }
                HDouble afterRefuelStartDistance = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AfterRefuelStartDistanceValue, afterRefuelStartDistance);
                if (isValidPositive(afterRefuelStartDistance) == true) {
                    mPrivate_AfterRefuelStartDistance = afterRefuelStartDistance;
                    saveAfterRefuelStartDistanceAndFuelCons();
                    mPrevPrivate_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
                }
                HUInt64 afterRefuelStartTime = 0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AfterRefuelStartTimeValue, afterRefuelStartTime);
                if (afterRefuelStartTime > 0) {
                    mPrivate_AfterRefuelStartTime = afterRefuelStartTime;
                    updatePrivateAfterRefuelVehicleSpeedKph();
                }
                HDouble afterRefuelStartFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AfterRefuelStartFuelConsValue, afterRefuelStartFuelCons);
                if (isValidPositive(afterRefuelStartFuelCons) == true) {
                    mPrivate_AfterRefuelStartFuelCons = afterRefuelStartFuelCons;
                    updatePrivateAfterRefuelFuelEcoKmL();
                }
                HDouble afterRefuelStartElecFuelCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AfterRefuelStartElecFuelConsValue, afterRefuelStartElecFuelCons);
                if (isValidPositive(afterRefuelStartElecFuelCons) == true) {
                    mPrivate_AfterRefuelStartElecFuelCons = afterRefuelStartElecFuelCons;
                    updatePrivateAfterRefuelFuelEcoKmKwh();
                }
                HDouble afterRefuelStartHydrogenCons = 0.0;
                GETCACHEDVALUE(SFC.Extension.Memory.Inter_AfterRefuelStartHydrogenConsValue, afterRefuelStartHydrogenCons);
                if (isValidPositive(afterRefuelStartHydrogenCons) == true) {
                    mPrivate_AfterRefuelStartHydrogenCons = afterRefuelStartHydrogenCons;
                    updatePrivateAfterRefuelHydrogenEco();
                }
                HUInt64 admStatus = 0;
                if (static_cast<Private_ADMStatus>(admStatus) != Private_ADMStatus::NONE) {
                    updatePrivateADMStatus();
                }
                _DDebug() << "Memory -> mPrivate_OdometerTotal : " << static_cast<HDouble>(mPrivate_OdometerTotal);
                _DDebug() << "Memory -> mPrivate_FuelConsumptionTotal : " << static_cast<HDouble>(mPrivate_FuelConsumptionTotal);
                _DDebug() << "Memory -> mPrivate_ElecFuelConsumptionTotal : "
                          << static_cast<HDouble>(mPrivate_ElecFuelConsumptionTotal);
                _DDebug() << "Memory -> mPrivate_DrivingTimeTotal : " << static_cast<HUInt64>(mPrivate_DrivingTimeTotal);
                _DDebug() << "Memory -> mPrivate_AccumStartDistance : " << static_cast<HDouble>(mPrivate_AccumStartDistance);
                _DDebug() << "Memory -> mPrivate_AccumStartTime : " << static_cast<HUInt64>(mPrivate_AccumStartTime);
                _DDebug() << "Memory -> mPrivate_AccumStartFuelCons : " << static_cast<HDouble>(mPrivate_AccumStartFuelCons);
                _DDebug() << "Memory -> mPrivate_AccumStartElecFuelCons : "
                          << static_cast<HDouble>(mPrivate_AccumStartElecFuelCons);
                _DDebug() << "Memory -> mPrivate_AccumStartHydrogenCons : "
                          << static_cast<HDouble>(mPrivate_AccumStartHydrogenCons);
                _DDebug() << "Memory -> mPrivate_DriveStartDistance : " << static_cast<HDouble>(mPrivate_DriveStartDistance);
                _DDebug() << "Memory -> mPrivate_DriveStartTime : " << static_cast<HUInt64>(mPrivate_DriveStartTime);
                _DDebug() << "Memory -> mPrivate_DriveStartFuelCons : " << static_cast<HDouble>(mPrivate_DriveStartFuelCons);
                _DDebug() << "Memory -> mPrivate_DriveStartElecFuelCons : "
                          << static_cast<HDouble>(mPrivate_DriveStartElecFuelCons);
                _DDebug() << "Memory -> mPrivate_DriveStartHydrogenCons : "
                          << static_cast<HDouble>(mPrivate_DriveStartHydrogenCons);
                _DDebug() << "Memory -> mPrivate_AfterRefuelStartDistance : "
                          << static_cast<HDouble>(mPrivate_AfterRefuelStartDistance);
                _DDebug() << "Memory -> mPrivate_AfterRefuelStartTime : " << static_cast<HUInt64>(mPrivate_AfterRefuelStartTime);
                _DDebug() << "Memory -> mPrivate_AfterRefuelStartFuelCons : "
                          << static_cast<HDouble>(mPrivate_AfterRefuelStartFuelCons);
                _DDebug() << "Memory -> mPrivate_AfterRefuelStartElecFuelCons : "
                          << static_cast<HDouble>(mPrivate_AfterRefuelStartElecFuelCons);
                _DDebug() << "Memory -> mPrivate_AfterRefuelStartHydrogenCons : "
                          << static_cast<HDouble>(mPrivate_AfterRefuelStartHydrogenCons);
                _DDebug() << "Memory -> mPrevPrivate_ADMStatus : " << static_cast<HUInt64>(mPrevPrivate_ADMStatus);

                onRepeatTimerPrivateAccumFuelEcoKmL10s();
                onRepeatTimerPrivateAccumFuelEcoKmKwh10s();
                onRepeatTimerPrivateAccumHydrogenEco10s();
                onRepeatTimerPrivateAccumVehicleSpeedKph10s();

                onRepeatTimerPrivateDriveFuelEcoKmL10s();
                onRepeatTimerPrivateDriveFuelEcoKmKwh10s();
                onRepeatTimerPrivateDriveHydrogenEco10s();

                onRepeatTimerPrivateAfterRefuelFuelEcoKmL10s();
                onRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10s();
                onRepeatTimerPrivateAfterRefuelHydrogenEco10s();
                onRepeatTimerPrivateAfterRefuelVehicleSpeedKph10s();

                flushActuatorOutput();
            }
        }
    }

    void onParamChanged(const ArgumentsParamChanged& args) {
        if (updateValueIfValid(mPar_OdoCorrectionFactor, args.mPar_OdoCorrectionFactor) == true) {
            DDebug() << "Par_OdoCorrectionFactor : " << static_cast<HDouble>(args.mPar_OdoCorrectionFactor);
        }
    }

    void onInputEvEnergyFlowStatusChanged(const ArgumentsInputEvEnergyFlowStatusChanged& args) {
        DDebug() << "Input_EvEnergyFlowStatus : " << static_cast<HUInt64>(args.mInput_EvEnergyFlowStatus);
        mInput_EvEnergyFlowStatus = args.mInput_EvEnergyFlowStatus;
        updatePrivateInstFuelEcoInputL100km();
    }

    void onInputHeadUnitFuelEcoGraphResetStatusChanged(const ArgumentsInputHeadUnitFuelEcoGraphResetStatusChanged& args) {
        DDebug() << "Input_HeadUnitFuelEcoGraphResetStatus : "
                 << static_cast<HUInt64>(args.mInput_HeadUnitFuelEcoGraphResetStatus);
        mInput_HeadUnitFuelEcoGraphResetStatus = args.mInput_HeadUnitFuelEcoGraphResetStatus;
        updatePrivateTermAvgTimer();
        flushActuatorOutput();
    }

    void onEvChargeStatusAndEvSocDisplayValueChanged(const ArgumentsEvChargeStatusAndEvSocDisplayValueChanged& args) {
        static Input_EvChargeStatus prevInput_EvChargeStatus = Input_EvChargeStatus::NONE;
        static HUInt64 evSocDisplayValueBase = 0;

        mInput_EvChargeStatus = args.mInput_EvChargeStatus;
        mInput_EvSocDisplayValue = args.mInput_EvSocDisplayValue;

        if (prevInput_EvChargeStatus != Input_EvChargeStatus::CHARGING &&
            mInput_EvChargeStatus == Input_EvChargeStatus::CHARGING) {
            evSocDisplayValueBase = mInput_EvSocDisplayValue;
        }

        if (mInput_EvSocDisplayValue >= evSocDisplayValueBase + 10 && mInput_EvChargeStatus == Input_EvChargeStatus::CHARGING) {
            mEvSocAfterRefuelResetStatus = true;
        } else {
            mEvSocAfterRefuelResetStatus = false;
        }

        prevInput_EvChargeStatus = mInput_EvChargeStatus;
        saveTheAfterRefuelStartPoint();
    }

    void onInputEvDTEAvnCalcStatus(const ArgumentsInputEvDTEAvnCalcStatus& args) {
        mInput_EvDTEAvnCalcStatus = args.mInput_EvDTEAvnCalcStatus;
        updateDTEAvnCalStat();
    }

    void onInputEvDTEDisplayOffReqStatus(const ArgumentsInputEvDTEDisplayOffReqStatus& args) {
        mInput_EvDTEDisplayOffReqStatus = args.mInput_EvDTEDisplayOffReqStatus;
        updateDTEDashDisplayForEvStat();
        updateOutputTotalDTEValue();
        flushActuatorOutput();
    }

    void onInputEvInstFuelEcoValuePerKWh(const ArgumentsInputEvInstFuelEcoValuePerKWh& args) {
        mInput_EvInstFuelEcoValuePerKWh = args.mInput_EvInstFuelEcoValuePerKWh;
        updatePrivateInstFuelEcoInput2();
    }

    void onInputEvInstFuelEcoValuePer100km(const ArgumentsInputEvInstFuelEcoValuePer100km& args) {
        mInput_EvInstFuelEcoValuePer100km = args.mInput_EvInstFuelEcoValuePer100km;
        updatePrivateInstFuelEcoInput2();
    }

    void onInputEvInstFuelEcoValuePerMPG(const ArgumentsInputEvInstFuelEcoValuePerMPG& args) {
        mInput_EvInstFuelEcoValuePerMPG = args.mInput_EvInstFuelEcoValuePerMPG;
        updatePrivateInstFuelEcoInput1();
    }

    void onInputFcevInstFuelEcoValuePerKg(const ArgumentsInputFcevInstFuelEcoValuePerKg& args) {
        mInput_FcevInstFuelEcoValuePerKg = args.mInput_FcevInstFuelEcoValuePerKg;
        updatePrivateInstFuelEcoInput3();
    }

    void onEvDteGuidePageInfoStatusChanged(const ArgumentsEvDteGuidePageInfoStatusChanged& args) {
        mInput_EvDteGuidePageInfoStatus = args.mInput_EvDteGuidePageInfoStatus;
        updateConstantDTEGuideFunctionStatus();
    }

    void onEvDteLowBoundValueChanged(const ArgumentsEvDteLowBoundValueChanged& args) {
        mEvDteLowBoundValue = args.mInput_EvDteLowBoundValue;
        updateConstantDTEGuideLowBoundValue();
    }

    void onEvDteUpperBoundValueChanged(const ArgumentsEvDteUpperBoundValueChanged& args) {
        mEvDteUpperBoundValue = args.mInput_EvDteUpperBoundValue;
        updateConstantDTEGuideUpperBoundValue();
    }

    void onEvDteGuideDtePositionValueChanged(const ArgumentsEvDteGuideDtePositionValueChanged& args) {
        mEvDteGuideDtePositionValue = args.mInput_EvDteGuideDtePositionValue;
        updateConstantDTEGuideMainDtePositionValue();
    }

    void onEvDteGuideCoachPositionValueChanged(const ArgumentsEvDteGuideCoachPositionValueChanged& args) {
        mEvDteGuideCoachPositionValue = args.mInput_EvDteGuideCoachPositionValue;
        updateConstantDTEGuideCoachPositionValue();
    }

    void onEvDteGuideRealTimeValueChanged(const ArgumentsEvDteGuideRealTimeValueChanged& args) {
        mEvDteGuideRealTimeValue = args.mInput_EvDteGuideRealTimeValue;
        updateConstantDTEGuideCoachValue();
        updateConstantDTEGuideCoachValueAvailableStat();
    }

    void onInterHeadUnitFuelEcoGraphResetStatusChanged(const ArgumentsInterHeadUnitFuelEcoGraphResetStatusChanged& args) {
        mInter_HeadUnitFuelEcoGraphResetStatus = args.mInter_HeadUnitFuelEcoGraphResetStatus;
        updatePrivateTermAvgTimer();
        flushActuatorOutput();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // Private Logic
    // 소수점 2자리를 Round Off 하는 함수
    inline HDouble roundOff2DecimalValue(HDouble value) {
        return round((value)*100.0) / 100.0;
    }

    // 공통된 ConfigVehicleType 조건 및 Ign 상태 묶음
    inline bool isConfigAndIgnStatusCondition() const {
        return (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV);
    }

    inline HBool isValidPositive(HDouble value) const {
        return surelyGreaterThan(value, 0.0) == true;
    }

    // 4.2 주행거리 누적 연산
    void updatePrivateOdometerTotal() {
        if (mIgnitionState == IgnitionState::ON) {
            std::chrono::time_point<std::chrono::steady_clock> currentOdometerRefreshTime = std::chrono::steady_clock::now();
            const std::chrono::duration<double, std::nano> durationNanosecs =
                currentOdometerRefreshTime - mPrevOdometerRefreshTime;
            mPrevOdometerRefreshTime = currentOdometerRefreshTime;
            mPrevPrivate_OdometerTotal = mPrivate_OdometerTotal;
            HBool isOdometerUpdate = true;
            HDouble odometerRefreshCycle = 0.0;
            if (mIsValidPrevOdometerRefreshTime == false) {
                mIsValidPrevOdometerRefreshTime = true;
                isOdometerUpdate = false;
                mPrivate_OdometerTotal += mInter_ActualVehicleSpeed * k10MsecPerHourDouble * mPar_OdoCorrectionFactor;
            } else {
                // kOdometerCalculationValue와 단위를 동일하게 맞추기 위해 시간의 역수에 ms->h로의
                // 변환 계수 3600 (1 hour/1 sec) * 1000 (1 sec/1 msec)를 곱한 값을 사용
                if (durationNanosecs.count() > 0U) {
                    odometerRefreshCycle = static_cast<HDouble>(durationNanosecs.count()) * kNsecPerHourDouble;
                } else {
                    isOdometerUpdate = false;
                }
            }
            if (isOdometerUpdate == true) {
                mPrivate_OdometerTotal += mInter_ActualVehicleSpeed * odometerRefreshCycle * mPar_OdoCorrectionFactor;
            }

            DDebug() << "Private_OdometerTotal : " << static_cast<HDouble>(mPrivate_OdometerTotal);

#ifdef ENABLE_GCOV_ON
            setSFCTrip_InfoGcovPrivate_OdometerTotal(mPrivate_OdometerTotal);
#endif
            updateOdometer();
            if (surelyGreaterThan(mPrivate_OdometerTotal, mPrevPrivate_OdometerTotal) == true) {
                mDiffDistance += mPrivate_OdometerTotal - mPrevPrivate_OdometerTotal;
            }
            // mPrivate_Odometer는 timeout 발생이후 속도 입력시 크게 점프할 수 있음
            // 그럴 경우 mDiffDistancce가 차감되기까지는 오랜 시간이 소요되어, 학습 연비 계산이 안될 수 있음
            if (mDiffDistance >= 0.2 || mPrivate_OdometerTotal - mPrevPrivate_OdometerTotal > 1) {
                mDiffDistance -= 0.2;
            }
            flushActuatorOutput();
        }
    }

    // 4.3 연료소모량 누적 연산
    void updatePrivateFuelConsumptionTotal() {
        if (mIgnitionState == IgnitionState::ON) {
            if (isConfigAndIgnStatusCondition()) {
                // kFuelCalculationValue(0.000000128) == 0.128 * 0.000001
                // value Increases every 10 miliseconds-time. (DB Cycle)
                mPrivate_FuelConsumptionTotal += static_cast<HDouble>(mInput_FuelConsumptionValue) * kFuelCalculationValue;

                DDebug() << "Private_FuelConsumptionTotal : " << static_cast<HDouble>(mPrivate_FuelConsumptionTotal);

#ifdef ENABLE_GCOV_ON
                setSFCTrip_InfoGcovPrivate_FuelConsumptionTotal(mPrivate_FuelConsumptionTotal);
#endif

                updatePrivateAccumFuelEcoKmL();
                updatePrivateDriveFuelEcoKmL();
                updatePrivateAfterRefuelFuelEcoKmL();
                flushActuatorOutput();
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
                // value Increases every 200 miliseconds-time. (DB Cycle)
                mPrivate_HydrogenConsumptionTotal +=
                    static_cast<HDouble>(mInput_FuelConsumptionValue) * kHydrogenCalValue * kHydrogenCalValueKg;

                DDebug() << "Private_HydrogenConsumptionTotal : " << static_cast<HDouble>(mPrivate_HydrogenConsumptionTotal);

#ifdef ENABLE_GCOV_ON
                setSFCTrip_InfoGcovPrivate_HydrogenConsumptionTotal(mPrivate_HydrogenConsumptionTotal);
#endif

                updatePrivateAccumHydrogenEco();
                updatePrivateDriveHydrogenEco();
                updatePrivateAfterRefuelHydrogenEco();
            } else {
                // Do nothing
            }
        }
    }

    // 4.4 전기소비전력량 누적 연산
    void updatePrivateElecFuelConsumptionTotal() {
        double elecFuelConsumptionDiff = 0.0;
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV &&
            mInput_VehicleReadyStatus == Input_VehicleReadyStatus::READY) {
            // kElecCalculationValue == 0.01 / 1000 / 3600
            // Note that " * 0.5" isn't in the Trip_Info SRS
            // Due to electricityConsumptionChanged has two different signal with different CAN message
            // onElectricityConsumptionChanged callback is envoked twice during 100ms
            // on the other hand Trip_Info SRS expect 100ms callback period
            // to remove this unexpected effect * 0.5 is applied
            elecFuelConsumptionDiff = static_cast<HDouble>(mInput_EvBatteryCurrentValue) *
                                      static_cast<HDouble>(mInput_EvBatteryVoltageValue) * kElecCalculationValue * 0.5;
            mPrivate_ElecFuelConsumptionTotal += (elecFuelConsumptionDiff);

            _DDebug() << "Private_ElecFuelConsumptionTotal diff : " << elecFuelConsumptionDiff;

#ifdef ENABLE_GCOV_ON
            setSFCTrip_InfoGcovPrivate_ElecFuelConsumptionTotal(mPrivate_ElecFuelConsumptionTotal);
#endif

            updatePrivateAccumFuelEcoKmKwh();
            updatePrivateDriveFuelEcoKmKwh();
            updatePrivateAfterRefuelFuelEcoKmKwh();
            flushActuatorOutput();
        }
    }

    // 4.5 주행시간 누적 연산 Timer Logic
    void calPrivateDrivingTimeTotal() {
        mPrivate_DrivingTimeTotal += 1;
        DDebug() << "Private_DrivingTimeTotal : " << static_cast<HUInt64>(mPrivate_DrivingTimeTotal);
#ifdef ENABLE_GCOV_ON
        setSFCTrip_InfoGcovPrivate_DrivingTimeTotal(mPrivate_DrivingTimeTotal);
#endif
        updatePrivateAccumVehicleSpeedKph();
        updateInterDriveInfoVehicleSpeedKph();
        updatePrivateAfterRefuelVehicleSpeedKph();
        flush();
    }

    // 4.5 주행시간 누적 연산 Timer callback
    void onRepeatTimerDrivingTimeTotal1sTimeout() {
        calPrivateDrivingTimeTotal();
    }

    // 4.5 주행시간 누적 연산
    void updatePrivateDrivingTimeTotal() {
        if (mIgnitionState == IgnitionState::ON &&
            (mInter_EngineOnStatus == Inter_EngineOnStatus::ON || mInter_ISGActiveStatus == Inter_ISGActiveStatus::ON)) {
            if (mRepeatTimerDrivingTimeTotal1sStatus.isRunning() == false &&
                mRepeatTimerDrivingTimeTotal1sStatus.start() == false) {
                DWarning() << "[Trip_Info] mRepeatTimerDrivingTimeTotal1sStatus Start Failed";
            }
        } else {
            mRepeatTimerDrivingTimeTotal1sStatus.stop();
        }
    }

    // 4.6 ADM 상태 판단 Timer Logic
    void onOneShotTimerIgnitionOff180sTimeout() {
        DDebug() << "ADM 180s Timer Logic On. Private_ADMStatus is OFF";
        mBeforeIgnitionOff180sStatus = true;
        mPrivate_ADMStatus = Private_ADMStatus::OFF;
        DDebug() << "mPrivate_ADMStatus : " << static_cast<HUInt64>(mPrivate_ADMStatus);
        saveTheStartPointDriveStart();
        updatePrivateDriveInfoResetStatus();
        flushActuatorOutput();
        flush();
        mPrevPrivate_ADMStatus = mPrivate_ADMStatus;
    }

    // 4.6 ADM 상태 판단
    void updatePrivateADMStatus() {
        bool isUpdate = false;
        static bool admPowerStatus = false;
        if (mIgninitState == false) {
            return;
        }
        if (mIgnitionState == IgnitionState::ON) {
            mOneShotTimerIgnitionOff180sStatus.stop();
            admPowerStatus = false;
            isUpdate = true;
            mPrivate_ADMStatus = Private_ADMStatus::ON;
        } else {
            if (mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
                admPowerStatus = true;
                if (!mBeforeIgnitionOff180sStatus &&
                    (mInput_DoorOpenDriverSideStatus == Input_DoorOpenDriverSideStatus::OPEN ||
                     mInput_DoorOpenDriverSideStatusFromSBCM == Input_DoorOpenDriverSideStatusFromSBCM::OPEN)) {
                    isUpdate = true;
                    mPrivate_ADMStatus = Private_ADMStatus::OFF;
                }
                if (mOneShotTimerIgnitionOff180sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mOneShotTimerIgnitionOff180sStatus Start Failed";
                }
            } else if (admPowerStatus == true &&
                       (mInput_DoorOpenDriverSideStatus == Input_DoorOpenDriverSideStatus::OPEN ||
                        mInput_DoorOpenDriverSideStatusFromSBCM == Input_DoorOpenDriverSideStatusFromSBCM::OPEN)) {
                isUpdate = true;
                mPrivate_ADMStatus = Private_ADMStatus::OFF;
            } else {
                // no operation
            }
        }
        if (mAEMInitCompleteFlag == true) {
            if (isUpdate == true) {
                saveTheStartPointDriveStart();
                updatePrivateDriveInfoResetStatus();
                mPrevPrivate_ADMStatus = mPrivate_ADMStatus;
                DDebug() << "mPrevPrivate_ADMStatus Changed From : " << static_cast<HUInt64>(mPrevPrivate_ADMStatus)
                         << " /To : " << static_cast<HUInt64>(mPrivate_ADMStatus);
            }
        }
        DDebug() << "mPrivate_ADMStatus : " << static_cast<HUInt64>(mPrivate_ADMStatus);
    }

    // 4.7 계산값 저장
    void saveDataToMemory() {
        _DDebug() << "AEM Initialization State(saveDataToMemory) : " << static_cast<HBool>(mAEMInitCompleteFlag);

        if (mAEMInitCompleteFlag == false) {
            return;
        }

        mSavedMemory_OdometerTotal = mPrivate_OdometerTotal;
        updateValue(getMemNodeAddr(MemoryAddrName::ODOMETER_VALUE), roundOff2DecimalValue(mSavedMemory_OdometerTotal));
        _DDebug() << "mPrivate_OdometerTotal" << mSavedMemory_OdometerTotal;
        mSavedMemory_FuelConsumptionTotal = mPrivate_FuelConsumptionTotal;
        updateValue(getMemNodeAddr(MemoryAddrName::FUEL_CONSUMPT_VALUE), mSavedMemory_FuelConsumptionTotal);
        _DDebug() << "mPrivate_FuelConsumptionTotal" << mPrivate_FuelConsumptionTotal;
        mSavedMemory_ElecFuelConsumptionTotal = mPrivate_ElecFuelConsumptionTotal;
        updateValue(getMemNodeAddr(MemoryAddrName::ELEC_FUEL_CONSUMPT_VALUE), mSavedMemory_ElecFuelConsumptionTotal);
        _DDebug() << "mPrivate_ElecFuelConsumptionTotal" << mPrivate_ElecFuelConsumptionTotal;
        mSavedMemory_HydrogenFuelConsumptionTotal = mPrivate_HydrogenConsumptionTotal;
        updateValue(getMemNodeAddr(MemoryAddrName::HYDROGEN_FUEL_CONSUMPT_VALUE), mSavedMemory_HydrogenFuelConsumptionTotal);
        _DDebug() << "mPrivate_HydrogenConsumptionTotal" << mPrivate_HydrogenConsumptionTotal;
        mSavedMemory_DrivingTimeTotal = mPrivate_DrivingTimeTotal;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVING_TIME_VALUE), mSavedMemory_DrivingTimeTotal);
        _DDebug() << "mPrivate_DrivingTimeTotal" << mPrivate_DrivingTimeTotal;

        mSavedMemory_AccumStartDistance = mPrivate_AccumStartDistance;
        updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_DISTANCE_VALUE),
                    roundOff2DecimalValue(mSavedMemory_AccumStartDistance));
        mSavedMemory_AccumStartTime = mPrivate_AccumStartTime;
        updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_TIME_VALUE), mSavedMemory_AccumStartTime);
        mSavedMemory_AccumStartFuelCons = mPrivate_AccumStartFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_FUEL_CONS_VALUE), mSavedMemory_AccumStartFuelCons);
        mSavedMemory_AccumStartElecFuelCons = mPrivate_AccumStartElecFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_ELEC_FUEL_CONS_VALUE), mSavedMemory_AccumStartElecFuelCons);
        mSavedMemory_AccumStartHydrogenCons = mPrivate_AccumStartHydrogenCons;
        updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_HYDROGEN_CONS_VALUE), mSavedMemory_AccumStartHydrogenCons);
        mSavedMemory_DriveStartDistance = mPrivate_DriveStartDistance;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_DISTANCE_VALUE),
                    roundOff2DecimalValue(mSavedMemory_DriveStartDistance));
        mSavedMemory_DriveStartTime = mPrivate_DriveStartTime;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_TIME_VALUE), mSavedMemory_DriveStartTime);
        mSavedMemory_DriveStartFuelCons = mPrivate_DriveStartFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_FUEL_CONS_VALUE), mSavedMemory_DriveStartFuelCons);
        mSavedMemory_DriveStartElecFuelCons = mPrivate_DriveStartElecFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_ELEC_FUEL_CONS_VALUE), mSavedMemory_DriveStartElecFuelCons);
        mSavedMemory_DriveStartHydrogenCons = mPrivate_DriveStartHydrogenCons;
        updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_HYDROGEN_CONS_VALUE), mSavedMemory_DriveStartHydrogenCons);

        mSavedMemory_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
        updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_DISTANCE_VALUE),
                    roundOff2DecimalValue(mSavedMemory_AfterRefuelStartDistance));
        mSavedMemory_AfterRefuelStartTime = mPrivate_AfterRefuelStartTime;
        updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_TIME_VALUE), mSavedMemory_AfterRefuelStartTime);
        mSavedMemory_AfterRefuelStartFuelCons = mPrivate_AfterRefuelStartFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_FUEL_CONS_VALUE), mSavedMemory_AfterRefuelStartFuelCons);
        mSavedMemory_AfterRefuelStartElecFuelCons = mPrivate_AfterRefuelStartElecFuelCons;
        updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_ELEC_FUEL_CONS_VALUE),
                    mSavedMemory_AfterRefuelStartElecFuelCons);
        mSavedMemory_AfterRefuelStartHydrogenCons = mPrivate_AfterRefuelStartHydrogenCons;
        updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_HYDROGEN_CONS_VALUE),
                    mSavedMemory_AfterRefuelStartHydrogenCons);
    }

    // 계산값 주기 저장
    void saveDataToMemoryPeriodic() {
        _DDebug() << "AEM Initialization State(saveDataToMemoryPeriodic) : " << static_cast<HBool>(mAEMInitCompleteFlag);

        if (mAEMInitCompleteFlag == false) {
            return;
        }
        savePrivateOdometerTotal();
        savePrivateDrivingTimeTotal();
        savePrivateFuelConsumptionTotal();
        savePrivateElecFuelConsumptionTotal();
        savePrivateHydrogenConsumptionTotal();
    }

    void savePrivateOdometerTotal() {
        if (almostEqual(mPrivate_OdometerTotal, 0.0) == false) {
            mSavedMemory_OdometerTotal = mPrivate_OdometerTotal;
            updateValue(getMemNodeAddr(MemoryAddrName::ODOMETER_VALUE), roundOff2DecimalValue(mSavedMemory_OdometerTotal));
            _DDebug() << "mPrivate_OdometerTotal" << mSavedMemory_OdometerTotal;
        }
    }

    void savePrivateFuelConsumptionTotal() {
        if (almostEqual(mPrivate_FuelConsumptionTotal, 0.0) == false) {
            mSavedMemory_FuelConsumptionTotal = mPrivate_FuelConsumptionTotal;
            updateValue(getMemNodeAddr(MemoryAddrName::FUEL_CONSUMPT_VALUE), mSavedMemory_FuelConsumptionTotal);
            _DDebug() << "mPrivate_FuelConsumptionTotal" << mPrivate_FuelConsumptionTotal;
        }
    }

    void savePrivateElecFuelConsumptionTotal() {
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, 0.0) == false) {
            mSavedMemory_ElecFuelConsumptionTotal = mPrivate_ElecFuelConsumptionTotal;
            updateValue(getMemNodeAddr(MemoryAddrName::ELEC_FUEL_CONSUMPT_VALUE), mSavedMemory_ElecFuelConsumptionTotal);
            _DDebug() << "mPrivate_ElecFuelConsumptionTotal" << mPrivate_ElecFuelConsumptionTotal;
        }
    }

    void savePrivateDrivingTimeTotal() {
        if (mPrivate_DrivingTimeTotal > 0) {
            mSavedMemory_DrivingTimeTotal = mPrivate_DrivingTimeTotal;
            updateValue(getMemNodeAddr(MemoryAddrName::DRIVING_TIME_VALUE), mSavedMemory_DrivingTimeTotal);
            _DDebug() << "mPrivate_DrivingTimeTotal" << mPrivate_DrivingTimeTotal;
        }
    }

    void savePrivateHydrogenConsumptionTotal() {
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, 0.0) == false) {
            mSavedMemory_HydrogenFuelConsumptionTotal = mPrivate_HydrogenConsumptionTotal;
            updateValue(getMemNodeAddr(MemoryAddrName::HYDROGEN_FUEL_CONSUMPT_VALUE), mSavedMemory_HydrogenFuelConsumptionTotal);
            _DDebug() << "mPrivate_HydrogenConsumptionTotal" << mPrivate_HydrogenConsumptionTotal;
        }
    }

    // 4.8 (1) 표시값 계산 시작점 저장
    void saveTheAccumStartPoint() {
        if (mPrevInter_AccumInfoManualResetReq == Inter_AccumInfoManualResetReq::DEFAULT &&
            mInter_AccumInfoManualResetReq == Inter_AccumInfoManualResetReq::RESET_REQ) {
            mPrivate_AccumStartDistance = mPrivate_OdometerTotal;
            mPrivate_AccumStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_AccumStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_AccumStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_AccumStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            updatePrivateAccumDistanceKmAndMi();
            updatePrivateAccumTime();
            updatePrivateAccumFuelEcoKmL();
            updatePrivateAccumFuelEcoKmKwh();
            updatePrivateAccumHydrogenEco();
            updatePrivateAccumVehicleSpeedKph();

            if (almostEqual(mPrivate_AccumStartDistance, 0.0) == false) {
                mSavedMemory_AccumStartDistance = mPrivate_AccumStartDistance;
                updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_DISTANCE_VALUE),
                            roundOff2DecimalValue(mSavedMemory_AccumStartDistance));
                _DDebug() << "Accum start reset invoked. mPrivate_AccumStartDistance : " << mPrivate_AccumStartDistance;
                savePrivateOdometerTotal();
            }
            if (mPrivate_AccumStartTime > 0) {
                mSavedMemory_AccumStartTime = mPrivate_AccumStartTime;
                updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_TIME_VALUE), mSavedMemory_AccumStartTime);
                _DDebug() << "Accum start reset invoked. mPrivate_AccumStartTime : " << mPrivate_AccumStartTime;
                savePrivateDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_AccumStartFuelCons, 0.0) == false) {
                mSavedMemory_AccumStartFuelCons = mPrivate_AccumStartFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_FUEL_CONS_VALUE), mSavedMemory_AccumStartFuelCons);
                _DDebug() << "Accum start reset invoked. mPrivate_AccumStartFuelCons : " << mPrivate_AccumStartFuelCons;
                savePrivateFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AccumStartElecFuelCons, 0.0) == false) {
                mSavedMemory_AccumStartElecFuelCons = mPrivate_AccumStartElecFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_ELEC_FUEL_CONS_VALUE),
                            mSavedMemory_AccumStartElecFuelCons);
                _DDebug() << "Accum start reset invoked. mPrivate_AccumStartElecFuelCons : " << mPrivate_AccumStartElecFuelCons;
                savePrivateElecFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AccumStartHydrogenCons, 0.0) == false) {
                mSavedMemory_AccumStartHydrogenCons = mPrivate_AccumStartHydrogenCons;
                updateValue(getMemNodeAddr(MemoryAddrName::ACCUM_START_HYDROGEN_CONS_VALUE), mSavedMemory_AccumStartHydrogenCons);
                _DDebug() << "Accum start reset invoked. mPrivate_AccumStartHydrogenCons : " << mPrivate_AccumStartHydrogenCons;
                savePrivateHydrogenConsumptionTotal();
            }
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::DEFAULT);
        } else {
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::DEFAULT);
        }
    }

    // 4.8 (2) 표시값 계산 시작점 저장
    void saveTheStartPointDriveStart() {
        if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV &&
             mPrevInter_DriveInfoManualResetReq == Inter_DriveInfoManualResetReq::DEFAULT &&
             mInter_DriveInfoManualResetReq == Inter_DriveInfoManualResetReq::RESET_REQ) ||
            (mPrevPrivate_ADMStatus == Private_ADMStatus::OFF && mPrivate_ADMStatus == Private_ADMStatus::ON)) {
            _DDebug() << "DriveInformation has been reset on SaveTheStartPointDriveStart()";
            mPrivate_DriveStartDistance = mPrivate_OdometerTotal;
            mPrivate_DriveStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_DriveStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_DriveStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_DriveStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            updateInterDriveInfoDistanceKmAndMi();
            updateInterDriveInfoTime();
            updatePrivateDriveFuelEcoKmL();
            updatePrivateDriveFuelEcoKmKwh();
            updatePrivateDriveHydrogenEco();
            updateInterDriveInfoVehicleSpeedKph();

            if (almostEqual(mPrivate_DriveStartDistance, 0.0) == false) {
                mSavedMemory_DriveStartDistance = mPrivate_DriveStartDistance;
                updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_DISTANCE_VALUE),
                            roundOff2DecimalValue(mSavedMemory_DriveStartDistance));
                _DDebug() << "Drive Start reset invoked. mPrivate_DriveStartDistance : " << mPrivate_DriveStartDistance;
                savePrivateOdometerTotal();
            }
            if (mPrivate_DriveStartTime > 0) {
                mSavedMemory_DriveStartTime = mPrivate_DriveStartTime;
                updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_TIME_VALUE), mSavedMemory_DriveStartTime);
                _DDebug() << "Drive Start reset invoked. mPrivate_DriveStartTime : " << mPrivate_DriveStartTime;
                savePrivateDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_DriveStartFuelCons, 0.0) == false) {
                mSavedMemory_DriveStartFuelCons = mPrivate_DriveStartFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_FUEL_CONS_VALUE), mSavedMemory_DriveStartFuelCons);
                _DDebug() << "Drive Start reset invoked. mPrivate_DriveStartFuelCons : " << mPrivate_DriveStartFuelCons;
                savePrivateFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_DriveStartElecFuelCons, 0.0) == false) {
                mSavedMemory_DriveStartElecFuelCons = mPrivate_DriveStartElecFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_ELEC_FUEL_CONS_VALUE),
                            mSavedMemory_DriveStartElecFuelCons);
                _DDebug() << "Drive Start reset invoked. mPrivate_DriveStartElecFuelCons : " << mPrivate_DriveStartElecFuelCons;
                savePrivateElecFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_DriveStartHydrogenCons, 0.0) == false) {
                mSavedMemory_DriveStartHydrogenCons = mPrivate_DriveStartHydrogenCons;
                updateValue(getMemNodeAddr(MemoryAddrName::DRIVE_START_HYDROGEN_CONS_VALUE), mSavedMemory_DriveStartHydrogenCons);
                _DDebug() << "Drive Start reset invoked. mPrivate_DriveStartHydrogenCons: " << mPrivate_DriveStartHydrogenCons;
                savePrivateHydrogenConsumptionTotal();
            }

            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::DEFAULT);
        } else {
            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::DEFAULT);
        }
    }

    // 4.8 (3) 표시값 계산 시작점 저장
    void saveTheAfterRefuelStartPoint() {
        if ((mPrevInter_AfterRefuelInfoManualResetReq == Inter_AfterRefuelInfoManualResetReq::DEFAULT &&
             mInter_AfterRefuelInfoManualResetReq == Inter_AfterRefuelInfoManualResetReq::RESET_REQ) ||
            mInter_FuelDetectRealtimeModeStatus == Inter_FuelDetectRealtimeModeStatus::ON ||
            (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV && mInput_EvChargeStatus == Input_EvChargeStatus::CHARGING &&
             mEvSocAfterRefuelResetStatus == true) ||
            (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV &&
             mInput_H2RefuelDetectionStatus == Input_H2RefuelDetectionStatus::ON)) {
            mPrivate_AfterRefuelStartDistance = mPrivate_OdometerTotal;
            mPrivate_AfterRefuelStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_AfterRefuelStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_AfterRefuelStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_AfterRefuelStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            saveAfterRefuelStartDistanceAndFuelCons();
            updatePrivateAfterRefuelDistanceKmAndMi();
            updatePrivateAfterRefuelTime();
            updatePrivateAfterRefuelFuelEcoKmL();
            updatePrivateAfterRefuelFuelEcoKmKwh();
            updatePrivateAfterRefuelHydrogenEco();
            updatePrivateAfterRefuelVehicleSpeedKph();
            flushActuatorOutput();

            if (almostEqual(mPrivate_AfterRefuelStartDistance, 0.0) == false) {
                mSavedMemory_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
                updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_DISTANCE_VALUE),
                            roundOff2DecimalValue(mSavedMemory_AfterRefuelStartDistance));
                _DDebug() << "After refuel reset invoked. mPrivate_AfterRefuelStartDistance : "
                          << mPrivate_AfterRefuelStartDistance;
                savePrivateOdometerTotal();
            }
            if (mPrivate_AfterRefuelStartTime > 0) {
                mSavedMemory_AfterRefuelStartTime = mPrivate_AfterRefuelStartTime;
                updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_TIME_VALUE), mSavedMemory_AfterRefuelStartTime);
                _DDebug() << "After refuel reset invoked. mPrivate_AfterRefuelStartTime : " << mPrivate_AfterRefuelStartTime;
                savePrivateDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartFuelCons, 0.0) == false) {
                mSavedMemory_AfterRefuelStartFuelCons = mPrivate_AfterRefuelStartFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_FUEL_CONS_VALUE),
                            mSavedMemory_AfterRefuelStartFuelCons);
                _DDebug() << "After refuel reset invoked. mPrivate_AfterRefuelStartFuelCons : "
                          << mPrivate_AfterRefuelStartFuelCons;
                savePrivateFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartElecFuelCons, 0.0) == false) {
                mSavedMemory_AfterRefuelStartElecFuelCons = mPrivate_AfterRefuelStartElecFuelCons;
                updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_ELEC_FUEL_CONS_VALUE),
                            mSavedMemory_AfterRefuelStartElecFuelCons);
                _DDebug() << "After refuel reset invoked. mPrivate_AfterRefuelStartElecFuelCons : "
                          << mPrivate_AfterRefuelStartElecFuelCons;
                savePrivateElecFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartHydrogenCons, 0.0) == false) {
                mSavedMemory_AfterRefuelStartHydrogenCons = mPrivate_AfterRefuelStartHydrogenCons;
                updateValue(getMemNodeAddr(MemoryAddrName::AFTER_REFUEL_START_HYDROGEN_CONS_VALUE),
                            mSavedMemory_AfterRefuelStartHydrogenCons);
                _DDebug() << "After refuel reset invoked. mPrivate_AfterRefuelStartHydrogenCons : "
                          << mPrivate_AfterRefuelStartHydrogenCons;
                savePrivateHydrogenConsumptionTotal();
            }
            mPrevPrivate_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::DEFAULT);
        } else {
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::DEFAULT);
        }
    }

    // 4.8 (4) 표시값 계산 시작점 저장
    void saveAfterRefuelStartDistanceAndFuelCons() {
        if (mPrevPrivate_AfterRefuelStartDistance < kMinDistanceForUpdateFuelEconomyValue &&
            mPrivate_AfterRefuelStartDistance >= kMinDistanceForUpdateFuelEconomyValue) {
            updatePrivateAfterRefuelDistanceKmAndMi();  // EXNCP-22986
        }
    }

    // 4.9.1 누적 연비 계산(연료) 10s Timer Logic
    void onRepeatTimerPrivateAccumFuelEcoKmL10s() {
        DDebug() << "On Private_AccumFuelEcoKmL 10s Timer.";
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AccumStartFuelCons) == true) {
            mPrivate_AccumFuelEcoKmL = 0;
        } else {
            mPrivate_AccumFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                                       (mPrivate_FuelConsumptionTotal - mPrivate_AccumStartFuelCons);
        }

        if (isValidPositive(mPrivate_AccumFuelEcoKmL) == true) {
            mPrivate_AccumFuelEcoL100km = 1.0 / mPrivate_AccumFuelEcoKmL * 100.0;
        } else {
            mPrivate_AccumFuelEcoL100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AccumFuelEcoUsMPG = mPrivate_AccumFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
            mPrivate_AccumFuelEcoUkMPG = mPrivate_AccumFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAccumFuelEconomyValue();
        flush();
    }

    // 4.9.1 누적 연비 계산(연료)
    void updatePrivateAccumFuelEcoKmL() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AccumStartFuelCons) == false) {
                if (mRepeatTimerPrivateAccumFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAccumFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAccumFuelEcoKmL10sStatus.stop();
                mPrivate_AccumFuelEcoKmL = 0.0;
                mPrivate_AccumFuelEcoL100km = 0.0;
                mPrivate_AccumFuelEcoUsMPG = 0.0;
                mPrivate_AccumFuelEcoUkMPG = 0.0;
                updatePrivateAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAccumFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.2 누적 연비 계산(전기) 10s Timer Logic
    void onRepeatTimerPrivateAccumFuelEcoKmKwh10s() {
        DDebug() << "On Private_AccumFuelEcoKmKwh 10s Timer.";
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AccumStartElecFuelCons) == true) {
            mPrivate_AccumFuelEcoKmKwh = 0;
        } else {
            mPrivate_AccumFuelEcoKmKwh =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                                      (mPrivate_ElecFuelConsumptionTotal - mPrivate_AccumStartElecFuelCons));
        }

        if (isValidPositive(mPrivate_AccumFuelEcoKmKwh) == true) {
            mPrivate_AccumFuelEcoKwh100km = 1.0 / mPrivate_AccumFuelEcoKmKwh * 100.0;
        } else {
            mPrivate_AccumFuelEcoKwh100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AccumFuelEcoMiKwh = mPrivate_AccumFuelEcoKmKwh * mFactor_ConvKmToMi;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAccumFuelEconomyValue();
        flush();
    }

    // 4.9.2 누적 연비 계산(전기)
    void updatePrivateAccumFuelEcoKmKwh() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AccumStartElecFuelCons) == false) {
                if (mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus.stop();
                mPrivate_AccumFuelEcoKmKwh = 0.0;
                mPrivate_AccumFuelEcoKwh100km = 0.0;
                mPrivate_AccumFuelEcoMiKwh = 0.0;
                updatePrivateAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.3 누적 연비 계산(수소) 10s Timer Logic
    void onRepeatTimerPrivateAccumHydrogenEco10s() {
        DDebug() << "On PrivateAccumHydrogenEco 10s RepeatTimer.";
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AccumStartHydrogenCons) == true) {
            mPrivate_AccumHydrogenEcoKmKg = 0;
        } else {
            mPrivate_AccumHydrogenEcoKmKg =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                                      (mPrivate_HydrogenConsumptionTotal - mPrivate_AccumStartHydrogenCons));
        }

        if (isValidPositive(mPrivate_AccumHydrogenEcoKmKg) == true) {
            mPrivate_AccumHydrogenEcoKg100km = 1.0 / mPrivate_AccumHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_AccumHydrogenEcoKg100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AccumHydrogenEcoMPG = mPrivate_AccumHydrogenEcoKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAccumFuelEconomyValue();
        flush();
    }

    // 4.9.3 누적 연비 계산(수소)
    void updatePrivateAccumHydrogenEco() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AccumStartHydrogenCons) == false) {
                if (mRepeatTimerPrivateAccumHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAccumHydrogenEco10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAccumHydrogenEco10sStatus.stop();
                mPrivate_AccumHydrogenEcoKmKg = 0.0;
                mPrivate_AccumHydrogenEcoKg100km = 0.0;
                updatePrivateAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAccumHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.4 누적 평균 속도 계산 10s Timer Logic
    void onRepeatTimerPrivateAccumVehicleSpeedKph10s() {
        DDebug() << "On Private_AccumVehicleSpeedKph 10s RepeatTimer.";
        if (mPrivate_DrivingTimeTotal == mPrivate_AccumStartTime) {
            mPrivate_AccumVehicleSpeedKph = 0;
        } else {
            mPrivate_AccumVehicleSpeedKph = lround(
                (mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_AccumStartTime)) / 3600.0));
        }

        if (mAEMInitCompleteFlag) {
            HDouble mph = static_cast<HDouble>(mPrivate_AccumVehicleSpeedKph) * mFactor_ConvKmToMi;
            mPrivate_AccumVehicleSpeedMph = static_cast<HUInt64>(floor(mph));
        } else {
            DDebug() << "AEM is not initialized";
        }
        updateAccumulatedInfoSpeed();
        flush();
    }

    // 4.9.4 누적 평균 속도 계산
    void updatePrivateAccumVehicleSpeedKph() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_AccumStartTime) {
                if (mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus.stop();
                mPrivate_AccumVehicleSpeedKph = 0;
                mPrivate_AccumVehicleSpeedMph = 0;
                updateAccumulatedInfoSpeed();
            }
        } else {
            mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.9.5 주행 연비 계산(연료) 10s Timer Logic - Private_DriveFuelEcoKmL
    void onRepeatTimerPrivateDriveFuelEcoKmL10s() {
        DDebug() << "On Private_DriveFuelEcoKmL 10s Timer.";
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_DriveStartFuelCons) == true) {
            mPrivate_DriveFuelEcoKmL = 0;
        } else {
            mPrivate_DriveFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                                       (mPrivate_FuelConsumptionTotal - mPrivate_DriveStartFuelCons);
        }

        if (isValidPositive(mPrivate_DriveFuelEcoKmL) == true) {
            mPrivate_DriveFuelEcoL100km = 1.0 / mPrivate_DriveFuelEcoKmL * 100.0;
        } else {
            mPrivate_DriveFuelEcoL100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_DriveFuelEcoUsMPG = mPrivate_DriveFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
            mPrivate_DriveFuelEcoUkMPG = mPrivate_DriveFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updateInterDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.5 주행 연비 계산(연료) - Private_DriveFuelEcoKmL
    void updatePrivateDriveFuelEcoKmL() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_DriveStartFuelCons) == false) {
                if (mRepeatTimerPrivateDriveFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateDriveFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateDriveFuelEcoKmL10sStatus.stop();
                mPrivate_DriveFuelEcoKmL = 0.0;
                mPrivate_DriveFuelEcoL100km = 0.0;
                mPrivate_DriveFuelEcoUsMPG = 0.0;
                mPrivate_DriveFuelEcoUkMPG = 0.0;
                updateInterDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerPrivateDriveFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.6 주행 연비 계산(전기) 10s Timer Logic
    void onRepeatTimerPrivateDriveFuelEcoKmKwh10s() {
        DDebug() << "On Private_DriveFuelEcoKmKwh 10s Timer.";
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_DriveStartElecFuelCons) == true) {
            mPrivate_DriveFuelEcoKmKwh = 0;
        } else {
            mPrivate_DriveFuelEcoKmKwh =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                                      (mPrivate_ElecFuelConsumptionTotal - mPrivate_DriveStartElecFuelCons));
        }

        if (isValidPositive(mPrivate_DriveFuelEcoKmKwh) == true) {
            mPrivate_DriveFuelEcoKwh100km = 1.0 / mPrivate_DriveFuelEcoKmKwh * 100.0;
        } else {
            mPrivate_DriveFuelEcoKwh100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_DriveFuelEcoMiKwh = mPrivate_DriveFuelEcoKmKwh * mFactor_ConvKmToMi;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updateInterDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.6 주행 연비 계산(전기)
    void updatePrivateDriveFuelEcoKmKwh() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_DriveStartElecFuelCons) == false) {
                if (mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus.stop();
                mPrivate_DriveFuelEcoKmKwh = 0.0;
                mPrivate_DriveFuelEcoKwh100km = 0.0;
                mPrivate_DriveFuelEcoMiKwh = 0.0;
                updateInterDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.7 주행 연비 계산(수소) 10s Timer Logic
    void onRepeatTimerPrivateDriveHydrogenEco10s() {
        DDebug() << "On PrivateDriveHydrogenEco 10s Timer.";
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_DriveStartHydrogenCons) == true) {
            mPrivate_DriveHydrogenEcoKmKg = 0;
        } else {
            mPrivate_DriveHydrogenEcoKmKg =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                                      (mPrivate_HydrogenConsumptionTotal - mPrivate_DriveStartHydrogenCons));
        }

        if (isValidPositive(mPrivate_DriveHydrogenEcoKmKg) == true) {
            mPrivate_DriveHydrogenEcoKg100km = 1.0 / mPrivate_DriveHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_DriveHydrogenEcoKg100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_DriveHydrogenEcoMPG = mPrivate_DriveHydrogenEcoKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updateInterDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.7 주행 연비 계산(수소)
    void updatePrivateDriveHydrogenEco() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_DriveStartHydrogenCons) == false) {
                if (mRepeatTimerPrivateDriveHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateDriveHydrogenEco10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateDriveHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateDriveHydrogenEco10sStatus.stop();
                mPrivate_DriveHydrogenEcoKmKg = 0.0;
                mPrivate_DriveHydrogenEcoKg100km = 0.0;
                mPrivate_DriveHydrogenEcoMPG = 0.0;
                updateInterDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerPrivateDriveHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.8 주행 평균 속도 계산 10s Timer Logic
    void onRepeatTimerInterDriveInfoVehicleSpeedKph10s() {
        DDebug() << "On Inter_DriveInfoVehicleSpeedKph 10s Timer.";
        if (mPrivate_DrivingTimeTotal == mPrivate_DriveStartTime) {
            mInter_DriveInfoVehicleSpeedKph = 0;
        } else {
            mInter_DriveInfoVehicleSpeedKph = lround(
                (mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_DriveStartTime)) / 3600.0));
        }
        if (mAEMInitCompleteFlag) {
            HDouble mph = static_cast<HDouble>(mInter_DriveInfoVehicleSpeedKph) * mFactor_ConvKmToMi;
            mInter_DriveInfoVehicleSpeedMph = static_cast<HUInt64>(floor(mph));
            setSFCTrip_InfoInter_DriveInfoVehicleSpeedMph(mInter_DriveInfoVehicleSpeedMph);
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updateDrivingInfoSpeed();
        setSFCTrip_InfoInter_DriveInfoVehicleSpeedKph(mInter_DriveInfoVehicleSpeedKph);
        flush();
    }

    // 4.9.8 주행 평균 속도 계산
    void updateInterDriveInfoVehicleSpeedKph() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_DriveStartTime) {
                if (mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus.stop();
                mInter_DriveInfoVehicleSpeedKph = 0;
                mInter_DriveInfoVehicleSpeedMph = 0;
                updateDrivingInfoSpeed();
                setSFCTrip_InfoInter_DriveInfoVehicleSpeedKph(mInter_DriveInfoVehicleSpeedKph);
            }
        } else {
            mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.9.9 주유 후 연비 계산(연료) 10s Timer Logic
    void onRepeatTimerPrivateAfterRefuelFuelEcoKmL10s() {
        DDebug() << "On Private_AfterRefuelFuelEcoKmL 10s Timer.";
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AfterRefuelStartFuelCons) == true) {
            mPrivate_AfterRefuelFuelEcoKmL = 0;
        } else {
            mPrivate_AfterRefuelFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                                             (mPrivate_FuelConsumptionTotal - mPrivate_AfterRefuelStartFuelCons);
        }

        if (isValidPositive(mPrivate_AfterRefuelFuelEcoKmL) == true) {
            mPrivate_AfterRefuelFuelEcoL100km = 1.0 / mPrivate_AfterRefuelFuelEcoKmL * 100.0;
        } else {
            mPrivate_AfterRefuelFuelEcoL100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AfterRefuelFuelEcoUsMPG = mPrivate_AfterRefuelFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
            mPrivate_AfterRefuelFuelEcoUkMPG = mPrivate_AfterRefuelFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAfterRefuelFuelEconomyValue();
        flushActuatorOutput();
        flush();
    }

    // 4.9.9 주유 후 연비 계산(연료)
    void updatePrivateAfterRefuelFuelEcoKmL() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AfterRefuelStartFuelCons) == false) {
                if (mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus.stop();
                mPrivate_AfterRefuelFuelEcoKmL = 0.0;
                mPrivate_AfterRefuelFuelEcoL100km = 0.0;
                mPrivate_AfterRefuelFuelEcoUsMPG = 0.0;
                mPrivate_AfterRefuelFuelEcoUkMPG = 0.0;
                updatePrivateAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.10 주유 후 연비 계산(전기) 10s Timer Logic
    void onRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10s() {
        DDebug() << "On Private_AfterRefuelFuelEcoKmKwh 10s Timer.";
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AfterRefuelStartElecFuelCons) == true) {
            mPrivate_AfterRefuelFuelEcoKmKwh = 0;
        } else {
            mPrivate_AfterRefuelFuelEcoKmKwh =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                                      (mPrivate_ElecFuelConsumptionTotal - mPrivate_AfterRefuelStartElecFuelCons));
        }

        if (isValidPositive(mPrivate_AfterRefuelFuelEcoKmKwh) == true) {
            mPrivate_AfterRefuelFuelEcoKwh100km = 1.0 / mPrivate_AfterRefuelFuelEcoKmKwh * 100.0;
        } else {
            mPrivate_AfterRefuelFuelEcoKwh100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AfterRefuelFuelEcoMiKwh = mPrivate_AfterRefuelFuelEcoKmKwh * mFactor_ConvKmToMi;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAfterRefuelFuelEconomyValue();
        flushActuatorOutput();
        flush();
    }

    // 4.9.10 주유 후 연비 계산(전기)
    void updatePrivateAfterRefuelFuelEcoKmKwh() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AfterRefuelStartElecFuelCons) == false) {
                if (mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus.stop();
                mPrivate_AfterRefuelFuelEcoKmKwh = 0.0;
                mPrivate_AfterRefuelFuelEcoKwh100km = 0.0;
                mPrivate_AfterRefuelFuelEcoMiKwh = 0.0;
                updatePrivateAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.11 주유 후 연비 계산(수소) 10s Timer Logic
    void onRepeatTimerPrivateAfterRefuelHydrogenEco10s() {
        DDebug() << "On PrivateAfterRefuelHydrogenEco 10s Timer.";
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AfterRefuelStartHydrogenCons) == true) {
            mPrivate_AfterRefuelHydrogenEcoKmKg = 0;
        } else {
            mPrivate_AfterRefuelHydrogenEcoKmKg =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                                      (mPrivate_HydrogenConsumptionTotal - mPrivate_AfterRefuelStartHydrogenCons));
        }

        if (isValidPositive(mPrivate_AfterRefuelHydrogenEcoKmKg) == true) {
            mPrivate_AfterRefuelHydrogenEcoKg100km = 1.0 / mPrivate_AfterRefuelHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_AfterRefuelHydrogenEcoKg100km = 0.0;
        }
        if (mAEMInitCompleteFlag) {
            mPrivate_AfterRefuelHydrogenEcoMPG = mPrivate_AfterRefuelHydrogenEcoKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updatePrivateAfterRefuelFuelEconomyValue();
        flush();
    }

    // 4.9.11 주유 후 연비 계산(수소)
    void updatePrivateAfterRefuelHydrogenEco() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AfterRefuelStartHydrogenCons) == false) {
                if (mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus.stop();
                mPrivate_AfterRefuelHydrogenEcoKmKg = 0.0;
                mPrivate_AfterRefuelHydrogenEcoKg100km = 0.0;
                updatePrivateAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.12 주유 후 평균 속도 계산 10s Timer Logic
    void onRepeatTimerPrivateAfterRefuelVehicleSpeedKph10s() {
        DDebug() << "On Private_AfterRefuelVehicleSpeedKph 10s Timer.";
        if (mPrivate_DrivingTimeTotal == mPrivate_AfterRefuelStartTime) {
            mPrivate_AfterRefuelVehicleSpeedKph = 0;
        } else {
            mPrivate_AfterRefuelVehicleSpeedKph =
                lround((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                       ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_AfterRefuelStartTime)) /
                        3600.0));
        }

        if (mAEMInitCompleteFlag) {
            HDouble mph = static_cast<HDouble>(mPrivate_AfterRefuelVehicleSpeedKph) * mFactor_ConvKmToMi;
            mPrivate_AfterRefuelVehicleSpeedMph = static_cast<HUInt64>(floor(mph));
        } else {
            DDebug() << "AEM is not initialized.";
        }
        updateAfterRefuelSpeed();
        flush();
    }

    // 4.9.12 주유 후 평균 속도 계산
    void updatePrivateAfterRefuelVehicleSpeedKph() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_AfterRefuelStartTime) {
                if (mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << "[Trip_Info] mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus.stop();
                mPrivate_AfterRefuelVehicleSpeedKph = 0;
                mPrivate_AfterRefuelVehicleSpeedMph = 0;
                updateAfterRefuelSpeed();
            }
        } else {
            mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.10 거리, 시간 표시값 계산 1S Timer Logic
    void onRepeatTimer1sTimeout() {
        updatePrivateAccumDistanceKmAndMi();
        updatePrivateAccumTime();
        updateInterDriveInfoDistanceKmAndMi();
        updateInterDriveInfoTime();
        updatePrivateAfterRefuelDistanceKmAndMi();
        updatePrivateAfterRefuelTime();

        // Each Display Status Flag Prevents Not To Flush Fuel Eco On 1S Timer
        // Each  Display Status Exists To Make flush On 300m (One Time Event)
        // Remark: FuelEco Required 10S Timer Flush Period
        if (mAccumDisplayStatus == false) {
            if (mPrivate_AccumDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mAccumDisplayStatus = true;
                onRepeatTimerPrivateAccumFuelEcoKmL10s();
                onRepeatTimerPrivateAccumFuelEcoKmKwh10s();
                onRepeatTimerPrivateAccumHydrogenEco10s();
            }
        }
        if (mDriveDisplayStatus == false) {
            if (mInter_DriveInfoDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mDriveDisplayStatus = true;
                onRepeatTimerPrivateDriveFuelEcoKmL10s();
                onRepeatTimerPrivateDriveFuelEcoKmKwh10s();
                onRepeatTimerPrivateDriveHydrogenEco10s();
            }
        }
        if (mAfterRefuelDisplayStatus == false) {
            if (mPrivate_AfterRefuelDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mAfterRefuelDisplayStatus = true;
                onRepeatTimerPrivateAfterRefuelFuelEcoKmL10s();
                onRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10s();
                onRepeatTimerPrivateAfterRefuelHydrogenEco10s();
            }
        }
        flush();
    }

    // 4.10.1, 4.10.2 누적 거리 계산(km), 누적 거리 계산(mi)
    void updatePrivateAccumDistanceKmAndMi() {
        mPrivate_AccumDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_AccumStartDistance);
        DDebug() << "Private_AccumDistanceKm : " << static_cast<HDouble>(mPrivate_AccumDistanceKm);
        if (mAEMInitCompleteFlag) {
            mPrivate_AccumDistanceMi =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AccumStartDistance) * mFactor_ConvKmToMi);
            DDebug() << "Private_AccumDistanceMi : " << static_cast<HDouble>(mPrivate_AccumDistanceMi);
        }
        updateAccumulatedInfoDistance();
        updateAccumulatedFuelEconomy();
    }

    // 4.10.3 누적 시간 계산
    void updatePrivateAccumTime() {
        HUInt64 timeGap = mPrivate_DrivingTimeTotal - mPrivate_AccumStartTime;
        mPrivate_AccumTime = timeGap / 60.0;
        DDebug() << "Private_AccumTime : " << static_cast<HUInt64>(mPrivate_AccumTime);
        updateAccumulatedInfoTime();
    }

    // 4.10.4, 4.10.5 주행 거리 계산(km), 주행 거리 계산(mi)
    void updateInterDriveInfoDistanceKmAndMi() {
        mInter_DriveInfoDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_DriveStartDistance);
        DDebug() << "Inter_DriveInfoDistanceKm : " << static_cast<HDouble>(mInter_DriveInfoDistanceKm);
        if (mAEMInitCompleteFlag) {
            mInter_DriveInfoDistanceMi =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_DriveStartDistance) * mFactor_ConvKmToMi);
            DDebug() << "Inter_DriveInfoDistanceMi : " << static_cast<HDouble>(mInter_DriveInfoDistanceMi);
            setSFCTrip_InfoInter_DriveInfoDistanceMi(mInter_DriveInfoDistanceMi);
        }
        setSFCTrip_InfoInter_DriveInfoDistanceKm(mInter_DriveInfoDistanceKm);
        updateDrivingInfoDistance();
        updateDrivingInfoFuelEconomy();
    }

    // 4.10.6 주행 시간 계산
    void updateInterDriveInfoTime() {
        HUInt64 timeGap = mPrivate_DrivingTimeTotal - mPrivate_DriveStartTime;
        mInter_DriveInfoTime = timeGap / 60.0;
        DDebug() << "Inter_DriveInfoTime : " << static_cast<HUInt64>(mInter_DriveInfoTime);
        setSFCTrip_InfoInter_DriveInfoTime(mInter_DriveInfoTime);
        updateDrivingInfoTime();
    }

    // 4.10.7, 4.10.8 주유 후 거리 계산(km), 주유 후 거리 계산(mi)
    void updatePrivateAfterRefuelDistanceKmAndMi() {
        mPrivate_AfterRefuelDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance);
        DDebug() << "Private_AfterRefuelDistanceKm : " << static_cast<HDouble>(mPrivate_AfterRefuelDistanceKm);
        if (mAEMInitCompleteFlag) {
            mPrivate_AfterRefuelDistanceMi =
                roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) * mFactor_ConvKmToMi);
            DDebug() << "Private_AfterRefuelDistanceMi : " << static_cast<HDouble>(mPrivate_AfterRefuelDistanceMi);
        }
        updateAfterRefuelDistance();
        updateAfterRefuelFuelEconomy();
    }

    // 4.10.9 주유 후 시간 계산
    void updatePrivateAfterRefuelTime() {
        HUInt64 timeGap = mPrivate_DrivingTimeTotal - mPrivate_AfterRefuelStartTime;
        mPrivate_AfterRefuelTime = timeGap / 60.0;
        DDebug() << "Private_AfterRefuelTime : " << static_cast<HUInt64>(mPrivate_AfterRefuelTime);
        updateAfterRefuelTime();
    }

    // 4.11 순간 연비 표시값 계산 (이동 평균 계산 방식 조건)
    inline bool isMoveAvgCalCondition() const {
        return (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV);
    }

    // 4.11 순간 연비 표시값 계산 (입력 기반 표시 방식 조건)
    inline bool isInputBaseCalCondition() const {
        return (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV);
    }

    // 4.11.1.1 private value update function 1
    void updateInstFunc1() {
        mPrivate_InstStart2Distance = mPrivate_OdometerTotal;
        mPrivate_InstStart2FuelCons = mPrivate_FuelConsumptionTotal;
        mPrivate_InstStart2HydrogenCons = mPrivate_HydrogenConsumptionTotal;
        mPrivate_InstDistHalfSum1 = mPrivate_OdometerTotal - mPrivate_InstStart1Distance;
        mPrivate_InstFuelConsHalfSum1 = mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons;
        mPrivate_InstHydrogenConsHalfSum1 = mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons;
    }

    // 4.11.1.1 private value update function 2
    void updateInstFunc2() {
        mPrivate_InstStart1Distance = mPrivate_OdometerTotal;
        mPrivate_InstStart1FuelCons = mPrivate_FuelConsumptionTotal;
        mPrivate_InstStart1HydrogenCons = mPrivate_HydrogenConsumptionTotal;
        mPrivate_InstDistHalfSum2 = mPrivate_OdometerTotal - mPrivate_InstStart2Distance;
        mPrivate_InstFuelConsHalfSum2 = mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons;
        mPrivate_InstHydrogenConsHalfSum2 = mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons;
    }

    // 4.11.1.1 계산 모드 판단, 4.11.1.2 구간 거리 및 연료 소모량 계산 Timer Logic
    void onRepeatTimer500msTimeout() {
        HBool isUpdate = true;
        if (mPrevPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::OFF &&
            mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::INIT1) {
            // OFF => INIT1 after 500ms
            // INIT1 => INIT2 before 500ms
            DDebug() << "OFF => INIT1 after 500ms / INIT1 => INIT2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::INIT2;
            updateInstFunc2();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::INIT1 &&
                   mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::INIT2) {
            // INIT1 => INIT2 after 500ms
            // INIT2 => ON1 before 500ms
            DDebug() << "INIT1 => INIT2 after 500ms / INIT2 => ON1 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::ON1;
            updateInstFunc1();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::INIT2 &&
                   mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) {
            // INIT2 => ON1 after 500ms
            // ON1 => ON2 before 500ms
            DDebug() << "INIT2 => ON1 after 500ms / ON1 => ON2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::ON2;
            updateInstFunc2();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1 &&
                   mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2) {
            // ON1 => ON2 after 500ms
            // ON2 => ON1 before 500ms
            DDebug() << "ON1 => ON2 after 500ms / ON2 => ON1 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::ON1;
            updateInstFunc1();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2 &&
                   mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) {
            // ON2 => ON1 after 500ms
            // ON1 => ON2 before 500ms
            DDebug() << "ON2 => ON1 after 500ms / ON1 => ON2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::ON2;
            updateInstFunc2();
        } else {
            isUpdate = false;
            DDebug() << "not operating condition.";
        }
        if (isUpdate) {
            updatePrivateInstFuelEcoKmL();
            updatePrivateInstFuelEcoL100km();
            updatePrivateInstFuelEcoUsMPG();
            updatePrivateInstFuelEcoUkMPG();
            flush();
        }
    }

    // 4.11.1.1 계산 모드 판단, 4.11.1.2 구간 거리 및 연료 소모량 계산
    void updatePrivateInstFuelEcoCalModeStatus() {
        if (isMoveAvgCalCondition() == true) {
            if (mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
                mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::OFF;
            }
            if (mIgnitionState == IgnitionState::ON) {
                if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::OFF &&
                    isValidPositive(mInter_ActualVehicleSpeed - 3.0) == true) {
                    // OFF => INIT1 before 500ms
                    DDebug() << "OFF => INIT1 before 500ms.";
                    mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                    mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::INIT1;
                    mPrivate_InstStart1Distance = mPrivate_OdometerTotal;
                    mPrivate_InstStart1FuelCons = mPrivate_FuelConsumptionTotal;
                    if (mRepeatTimer500msStatus.restart() == false) {
                        DWarning() << "[Trip_Info] mRepeatTimer500msStatus Start Failed";
                    }
                } else if (mPrivate_InstFuelEcoCalModeStatus != Private_InstFuelEcoCalModeStatus::OFF &&
                           (surelyLessThan(mInter_ActualVehicleSpeed, 3.0) == true ||
                            almostEqual(mInter_ActualVehicleSpeed, 3.0) == true)) {
                    // Excluding OFF
                    DDebug() << "Excluding OFF.";
                    mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                    mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::OFF;
                    mRepeatTimer500msStatus.stop();
                } else {
                    DDebug() << "not operating condition.";
                    return;
                }
            } else {
                mRepeatTimer500msStatus.stop();
            }
            updatePrivateInstFuelEcoKmL();
            updatePrivateInstFuelEcoL100km();
            updatePrivateInstFuelEcoUsMPG();
            updatePrivateInstFuelEcoUkMPG();
        } else {
            mRepeatTimer500msStatus.stop();
        }
    }

    // 4.11.1.3 연비 표시 목표값 계산
    // 4.11.1.3.1 km/L
    void updatePrivateInstFuelEcoKmL() {
        mPrivate_InstFuelEcoKmL = 0.0;
        if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) {
            if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1), 0.0) ==
                false) {
                mPrivate_InstFuelEcoKmL =
                    (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                    (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1);
            } else {
                mPrivate_InstFuelEcoKmL = 999.0;
            }
        } else if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2) {
            if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2), 0.0) ==
                false) {
                mPrivate_InstFuelEcoKmL =
                    (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                    (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2);
            } else {
                mPrivate_InstFuelEcoKmL = 999.0;
            }
        } else {
            // 0.0
        }
        updateInstantaneousFuelEconomy();
    }

    // 4.11.1.3.3 L/100km
    void updatePrivateInstFuelEcoL100km() {
        if ((mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) &&
            (almostEqual((mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1), 0.0) == false)) {
            mPrivate_InstFuelEcoL100km =
                (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) /
                (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) * 100.0;
        } else if ((mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2) &&
                   (almostEqual((mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2), 0.0) ==
                    false)) {
            mPrivate_InstFuelEcoL100km =
                (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) /
                (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) * 100.0;
        } else {
            mPrivate_InstFuelEcoL100km = 0.0;
        }
        updateInstantaneousFuelEconomy();
    }

    // 4.11.1.3.5 US MPG
    void updatePrivateInstFuelEcoUsMPG() {
        mPrivate_InstFuelEcoUsMPG = 0.0;
        if (mAEMInitCompleteFlag) {
            if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUsMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) *
                        mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
                } else {
                    mPrivate_InstFuelEcoUsMPG = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual(mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2,
                                0.0) == false) {
                    mPrivate_InstFuelEcoUsMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) *
                        mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
                } else {
                    mPrivate_InstFuelEcoUsMPG = 999.0;
                }
            } else {
                // 0.0
            }
        }
        updateInstantaneousFuelEconomy();
    }

    // 4.11.1.3.7 UK MPG
    void updatePrivateInstFuelEcoUkMPG() {
        mPrivate_InstFuelEcoUkMPG = 0.0;
        if (mAEMInitCompleteFlag) {
            if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUkMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) *
                        mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
                } else {
                    mPrivate_InstFuelEcoUkMPG = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == Private_InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUkMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) *
                        mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
                } else {
                    mPrivate_InstFuelEcoUkMPG = 999.0;
                }
            } else {
                // 0.0
            }
        }
        updateInstantaneousFuelEconomy();
    }

    // 4.11.2 입력 기반 표시 방식 DB_CYCLE_200ms
    void updatePrivateInstFuelEcoInput1() {
        if (isInputBaseCalCondition() == true) {
            if (mIgnitionState == IgnitionState::ON) {
                mPrivate_InstFuelEcoInputKmL = static_cast<HDouble>(mInput_EvInstFuelEcoValuePerMPG) * (60.0 / 255.0);
                mPrivate_InstFuelEcoInputUsMPG = mPrivate_InstFuelEcoInputKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
                mPrivate_InstFuelEcoInputUkMPG = mPrivate_InstFuelEcoInputKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
            } else {
                mPrivate_InstFuelEcoInputKmL = 0.0;
            }
            updatePrivateInstFuelEcoInputL100km();
            updateInstFuelEconomyInputType();
        }
    }

    // 4.11.2 입력 기반 표시 방식 (Private_InstFuelEcoInputL100km)
    void updatePrivateInstFuelEcoInputL100km() {
        if (isInputBaseCalCondition() == true) {
            if (mIgnitionState == IgnitionState::ON) {
                if ((almostEqual(mPrivate_InstFuelEcoInputKmL, 0.0) == true) ||
                    (mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::VEHICLE_STOP ||
                     mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::EV_PROPULSION ||
                     mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::ENGINE_GENERATION ||
                     mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::REGENERATION ||
                     mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::ENGINE_BRAKE ||
                     mInput_EvEnergyFlowStatus == Input_EvEnergyFlowStatus::ENGINE_BRAKERE_GENERATION)) {
                    mPrivate_InstFuelEcoInputL100km = 0.0;
                } else {
                    mPrivate_InstFuelEcoInputL100km = 1.0 / mPrivate_InstFuelEcoInputKmL * 100.0;
                }
            } else {
                mPrivate_InstFuelEcoInputL100km = 0.0;
            }
        }
    }

    // 4.11.2 입력 기반 표시 방식 (Private_InstFuelEcoInputKmKwh) DB_CYCLE_200ms
    void updatePrivateInstFuelEcoInput2() {
        if (isInputBaseCalCondition() == true) {
            if (mIgnitionState == IgnitionState::ON) {
                mPrivate_InstFuelEcoInputKmKwh = static_cast<HDouble>(mInput_EvInstFuelEcoValuePerKWh) * 0.1;
                mPrivate_InstFuelEcoInputKwh100km = static_cast<HDouble>(mInput_EvInstFuelEcoValuePer100km) * 0.5;
                mPrivate_InstFuelEcoInputMiKwh = mPrivate_InstFuelEcoInputKmKwh * mFactor_ConvKmToMi;
            } else {
                mPrivate_InstFuelEcoInputKmKwh = 0.0;
                mPrivate_InstFuelEcoInputKwh100km = 0.0;
                mPrivate_InstFuelEcoInputMiKwh = 0.0;
            }
            updateInstFuelEconomyInputType();
        }
    }

    // 4.11.2 입력 기반 표시 방식 (Private_InstFuelEcoInputKmKg) DB_CYCLE_200ms
    void updatePrivateInstFuelEcoInput3() {
        if (isInputBaseCalCondition() == true) {
            if (mIgnitionState == IgnitionState::ON) {
                mPrivate_InstFuelEcoInputKmKg = static_cast<HDouble>(mInput_FcevInstFuelEcoValuePerKg);
                if (almostEqual(mPrivate_InstFuelEcoInputKmKg, 0.0) == true ||
                    almostEqual(mPrivate_InstFuelEcoInputKmKg, 255.0) == true) {
                    mPrivate_InstFuelEcoInputKg100Km = 0.0;
                } else {
                    mPrivate_InstFuelEcoInputKg100Km = 1.0 / mPrivate_InstFuelEcoInputKmKg * 100.0;
                }
                mPrivate_InstFuelEcoInputMileKg = mPrivate_InstFuelEcoInputKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
            } else {
                mPrivate_InstFuelEcoInputKmKg = 0.0;
                mPrivate_InstFuelEcoInputKg100Km = 0.0;
                mPrivate_InstFuelEcoInputMileKg = 0.0;
            }
            updateInstFuelEconomyInputType();
        }
    }

    // 4.12 평균 연비 표시 종류 판단
    // 4.12.1 누적 연비
    void updatePrivateAccumFuelEconomyValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
                mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoL100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoUkMPG;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                        return;
                    }
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKwh100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoMiKwh;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoKg100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoMPG;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else {
                DDebug() << "Inter_ConfigVehicleType is not operate condition.";
                return;
            }
            updateAccumulatedFuelEconomy();
        }
    }

    // 4.12.2 주행 연비
    void updateInterDriveInfoFuelEcoValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
                mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoL100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoUkMPG;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                        return;
                    }
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKwh100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoMiKwh;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoKg100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoMPG;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else {
                DDebug() << "Inter_ConfigVehicleType is not operate condition.";
                return;
            }
            updateDrivingInfoFuelEconomy();
            setSFCTrip_InfoInter_DriveInfoFuelEcoValue(mInter_DriveInfoFuelEcoValue);
        }
    }

    // 4.12.3 주유 후 연비
    void updatePrivateAfterRefuelFuelEconomyValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
                mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoL100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoUkMPG;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                        return;
                    }
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKwh100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoMiKwh;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoKg100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoMPG;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else {
                DDebug() << "Inter_ConfigVehicleType is not operate condition.";
                return;
            }
            updateAfterRefuelFuelEconomy();
            updateOutputAvgFuelEconomyValue();
            updateOutputAvgFuelEconomyDigitStatus();
        }
    }

    // 4.13 현재 값 외부 출력 Output_OdometerValue
    void updateOutputOdometerValue(const HUInt64& odometerValue) {
        updateActuatorValue(Vehicle.CD.Trip_Info.Output_OdometerValue.getAddress(), odometerValue);
    }

    // 4.13 현재 값 외부 출력 Output_AvgFuelEconomyUnitStatus
    void updateOutputAvgFuelEconomyUnitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            Output_AvgFuelEconomyUnitStatus outputAvgFuelEconomyUnitStatus;
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    outputAvgFuelEconomyUnitStatus = Output_AvgFuelEconomyUnitStatus::KM_PER_LITTER;
                } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    outputAvgFuelEconomyUnitStatus = Output_AvgFuelEconomyUnitStatus::LITTER_PER_KM;
                } else {
                    DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                    return;
                }
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                    outputAvgFuelEconomyUnitStatus = Output_AvgFuelEconomyUnitStatus::MPG_US;
                } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                    outputAvgFuelEconomyUnitStatus = Output_AvgFuelEconomyUnitStatus::MPG_UK;
                } else {
                    DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                    return;
                }
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
            updateActuatorValue(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyUnitStatus.getAddress(),
                                static_cast<HUInt64>(outputAvgFuelEconomyUnitStatus));
        }
    }

    // 4.13 현재 값 외부 출력 Output_AvgFuelEconomyValue
    void updateOutputAvgFuelEconomyValue() {
        if (mIgnitionState == IgnitionState::ON) {
            HDouble outputAvgFuelEconomyValue = 0.0;

            if (mPrivate_AfterRefuelDistanceKm >= 0.3 && mPrivate_AfterRefuelFuelEconomyValue < 100) {
                outputAvgFuelEconomyValue =
                    round(mPrivate_AfterRefuelFuelEconomyValue * 10.0) / 10.0 * kOutputAvgFuelEconomyValueFactor;
            } else if (mPrivate_AfterRefuelDistanceKm >= 0.3 && mPrivate_AfterRefuelFuelEconomyValue >= 100 &&
                       mPrivate_AfterRefuelFuelEconomyValue < 999) {
                outputAvgFuelEconomyValue = round(mPrivate_AfterRefuelFuelEconomyValue);
            } else if (mPrivate_AfterRefuelDistanceKm >= 0.3 && mPrivate_AfterRefuelFuelEconomyValue >= 999) {
                outputAvgFuelEconomyValue = 999;
            } else if (mPrivate_AfterRefuelDistanceKm < 0.3) {
                outputAvgFuelEconomyValue = 0;
            } else {
                // N.A
            }

            updateActuatorValue(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyValue.getAddress(),
                                static_cast<HUInt64>(outputAvgFuelEconomyValue));
        }
    }

    // 4.13 현재 값 외부 출력 Output_AvgFuelEconomyDigitStatus
    void updateOutputAvgFuelEconomyDigitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_AfterRefuelFuelEconomyValue < 100.0) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyDigitStatus.getAddress(),
                                    static_cast<HUInt64>(Output_AvgFuelEconomyDigitStatus::DIGIT2));
            } else {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyDigitStatus.getAddress(),
                                    static_cast<HUInt64>(Output_AvgFuelEconomyDigitStatus::DIGIT3));
            }
        }
    }

    // 4.14 2.5 분 평균연비 계산 및 신호 송출
    // 4.14 Inter_ConfigVehicleType Condition
    inline bool termAvgTimerCommonCondition() const {
        return (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV);
    }

    // 4.14.1 2.5분 타이머 동작 Private_TermAvgTimer value reset
    void resetPrivateTermAvgTimer() {
        mPrivate_TermAvgTimer = 0;
        DDebug() << "Private_TermAvgTimer : " << static_cast<HUInt64>(mPrivate_TermAvgTimer);
    }

    // 4.14.1 2.5분 타이머 동작 Timer Logic
    void onRepeatTimerPrivateTermAvgTimer1s() {
        mPrivate_TermAvgTimer++;
        if (mPrivate_TermAvgTimer >= kTermAvgCountLimit) {
            updatePrivateTermAvgCalModeStatus();
            updatePrivateTermAvgSignalTxModeStatus();
            flushActuatorOutput();
            mPrivate_TermAvgTimer = 0;
            _DDebug() << "mPrivate_TermAvgFuelEconomyValue : " << mPrivate_TermAvgFuelEconomyValue;
            _DDebug() << "mPrivate_TermAvgCalModeStatus : " << static_cast<HUInt64>(mPrivate_TermAvgCalModeStatus);
            _DDebug() << "mPrivate_TermAvgSignalTxModeStatus : " << static_cast<HUInt64>(mPrivate_TermAvgSignalTxModeStatus);
        }
        _DDebug() << "Private_TermAvgTimer : " << static_cast<HUInt64>(mPrivate_TermAvgTimer);
    }

    // 4.14.1 2.5분 타이머 동작
    void updatePrivateTermAvgTimer() {
        if (termAvgTimerCommonCondition()) {
            HBool isUpdate = false;
            if (mIgnitionState == IgnitionState::ON) {
                if (mInput_HeadUnitFuelEcoGraphResetStatus == Input_HeadUnitFuelEcoGraphResetStatus::RESET) {
                    isUpdate = true;
                    resetPrivateTermAvgTimer();
                } else {
                    // Input_HeadUnitFuelEcoGraphResetStatus != RESET
                    DDebug() << "Input_HeadUnitFuelEcoGraphResetStatus is not RESET.";
                }
                if (mInter_EngineOnStatus == Inter_EngineOnStatus::ON) {
                    isUpdate = true;
                    if (mRepeatTimerPrivateTerm1sAvgStatus.isRunning() == false &&
                        mRepeatTimerPrivateTerm1sAvgStatus.start() == false) {
                        DWarning() << "[Trip_Info] mRepeatTimerPrivateTerm1sAvgStatus Start Failed";
                    }
                } else {
                    // Inter_EngineOnStatus == OFF
                    isUpdate = true;
                    resetPrivateTermAvgTimer();
                    mRepeatTimerPrivateTerm1sAvgStatus.stop();
                }
            } else {
                // IGN OFF
                isUpdate = true;
                resetPrivateTermAvgTimer();
                mRepeatTimerPrivateTerm1sAvgStatus.stop();
            }
            if (isUpdate) {
                updatePrivateTermAvgCalModeStatus();
                updatePrivateTermAvgSignalTxModeStatus();
            }
        } else {
            mRepeatTimerPrivateTerm1sAvgStatus.stop();
        }
    }

    // 4.14.2 계산 모드 정의
    void updatePrivateTermAvgCalModeStatus() {
        static HBool modeInitFlag = false;
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        if (mIgnitionState == IgnitionState::ON) {
            DDebug() << "mPrevInter_EngineOnStatus:: " << static_cast<HUInt64>(mPrevInter_EngineOnStatus);
            DDebug() << "mInter_EngineOnStatus:: " << static_cast<HUInt64>(mInter_EngineOnStatus);
            if ((mInter_EngineOnTransStatus == true && modeInitFlag == false) ||
                (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                 (mPrivate_TermAvgTimer >= kTermAvgCountLimit)) ||
                (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                 mInput_HeadUnitFuelEcoGraphResetStatus == Input_HeadUnitFuelEcoGraphResetStatus::RESET) ||
                (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                 mInter_HeadUnitFuelEcoGraphResetStatus == true)) {
                mPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::ON1;
                if (mInter_EngineOnTransStatus == true) {
                    modeInitFlag = true;
                }
            } else if ((mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                        (mPrivate_TermAvgTimer >= kTermAvgCountLimit)) ||
                       (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                        mInput_HeadUnitFuelEcoGraphResetStatus == Input_HeadUnitFuelEcoGraphResetStatus::RESET) ||
                       (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                        mInter_HeadUnitFuelEcoGraphResetStatus == true)) {
                mPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::ON2;
            } else if (mInter_EngineOnStatus == Inter_EngineOnStatus::OFF) {
                mPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::OFF;
                modeInitFlag = false;
            } else {
                DDebug() << "is not Private_TermAvgCalModeStatus update condition.";
                return;
            }
        } else {
            mPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::OFF;
            modeInitFlag = false;
        }
        updatePrivateTermAvgStartDistAndFuelConsSave();
        updateOutputTermAvgFuelConsSyncCounter();
        DDebug() << "mPrivate_TermAvgCalModeStatus: " << static_cast<HUInt64>(mPrivate_TermAvgCalModeStatus);
        mPrevPrivate_TermAvgCalModeStatus = mPrivate_TermAvgCalModeStatus;
    }

    // 4.14.3 신호 송출 모드 정의
    void updatePrivateTermAvgSignalTxModeStatus() {
        static HBool modeInitFlag = false;
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_EngineOnStatus == Inter_EngineOnStatus::OFF) {
                mPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::READY;
                modeInitFlag = false;
            } else if ((mInter_EngineOnTransStatus == true && modeInitFlag == false) ||
                       (mPrevPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::RUNNING &&
                        mInput_HeadUnitFuelEcoGraphResetStatus == Input_HeadUnitFuelEcoGraphResetStatus::RESET) ||
                       (mPrevPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::RUNNING &&
                        mInter_HeadUnitFuelEcoGraphResetStatus == true)) {
                mPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::START;
                if (mInter_EngineOnTransStatus == true) {
                    modeInitFlag = true;
                }
            } else if (mPrevPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::START &&
                       (mPrivate_TermAvgTimer >= kTermAvgCountLimit)) {
                mPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::RUNNING;
            } else {
                // DDebug() << "is not Private_TermAvgSignalTxModeStatus update condition.";
            }
        } else {
            mPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::OFF;
            modeInitFlag = false;
        }
        updateOutputTermAvgFuelConsValue();
        updateInterTermAvgFuelConsValue();
        updateInterTermAvgFuelConsValid();
        DDebug() << "mPrivate_TermAvgSignalTxModeStatus: " << static_cast<HUInt64>(mPrivate_TermAvgSignalTxModeStatus);
        mPrevPrivate_TermAvgSignalTxModeStatus = mPrivate_TermAvgSignalTxModeStatus;
    }

    // 4.14.4 2.5 분 연비 연산 시작점 저장
    void updatePrivateTermAvgStartDistAndFuelConsSave() {
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrevPrivate_TermAvgCalModeStatus != Private_TermAvgCalModeStatus::ON1 &&
                mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1) {
                mPrivate_TermAvg1StartDistance = mPrivate_OdometerTotal;
                if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                    mPrivate_TermAvg1StartFuelCons = mPrivate_FuelConsumptionTotal;
                } else {
                    mPrivate_TermAvg1StartFuelCons = mPrivate_HydrogenConsumptionTotal;
                }
            } else if (mPrevPrivate_TermAvgCalModeStatus != Private_TermAvgCalModeStatus::ON2 &&
                       mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) {
                mPrivate_TermAvg2StartDistance = mPrivate_OdometerTotal;
                if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                    mPrivate_TermAvg2StartFuelCons = mPrivate_FuelConsumptionTotal;
                } else {
                    mPrivate_TermAvg2StartFuelCons = mPrivate_HydrogenConsumptionTotal;
                }
            } else if (mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::OFF) {
                mPrivate_TermAvg1StartDistance = 0.0;
                mPrivate_TermAvg1StartFuelCons = 0.0;
                mPrivate_TermAvg2StartDistance = 0.0;
                mPrivate_TermAvg2StartFuelCons = 0.0;
            } else {
                DDebug() << "is not updatePrivateTermAvgStartDistAndFuelConsSave() update condition.";
                return;
            }
            DDebug() << "mPrivate_TermAvg1StartDistance: " << mPrivate_TermAvg1StartDistance;
            DDebug() << "mPrivate_TermAvg1StartFuelCons: " << mPrivate_TermAvg1StartFuelCons;
            DDebug() << "mPrivate_TermAvg2StartDistance: " << mPrivate_TermAvg2StartDistance;
            DDebug() << "mPrivate_TermAvg2StartFuelCons: " << mPrivate_TermAvg2StartFuelCons;

            updatePrivateTermAvgFuelEcoCalc();
        }
    }

    // 4.14.5 2.5 분 연비 계산
    void updatePrivateTermAvgFuelEcoCalc() {
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        mPrivate_TermAvgFuelEcoMpge = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_TermAvg1StartFuelCons, mPrivate_TermAvg2StartFuelCons) == false) {
                if (mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1) {
                    mPrivate_TermAvgFuelEcoKmL = (mPrivate_TermAvg1StartDistance - mPrivate_TermAvg2StartDistance) /
                                                 (mPrivate_TermAvg1StartFuelCons - mPrivate_TermAvg2StartFuelCons);
                    mPrivate_TermAvgFuelEcoKmKg = (mPrivate_TermAvg1StartDistance - mPrivate_TermAvg2StartDistance) /
                                                  (mPrivate_TermAvg1StartFuelCons - mPrivate_TermAvg2StartFuelCons);
                } else if (mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) {
                    mPrivate_TermAvgFuelEcoKmL = (mPrivate_TermAvg2StartDistance - mPrivate_TermAvg1StartDistance) /
                                                 (mPrivate_TermAvg2StartFuelCons - mPrivate_TermAvg1StartFuelCons);
                    mPrivate_TermAvgFuelEcoKmKg = (mPrivate_TermAvg2StartDistance - mPrivate_TermAvg1StartDistance) /
                                                  (mPrivate_TermAvg2StartFuelCons - mPrivate_TermAvg1StartFuelCons);
                } else {
                    mPrivate_TermAvgFuelEcoKmL = 0.0;
                    mPrivate_TermAvgFuelEcoKmKg = 0.0;
                }
            } else if (surelyGreaterThan(std::abs(mPrivate_TermAvg1StartDistance - mPrivate_TermAvg2StartDistance), 0.05) ==
                           true &&
                       almostEqual(mPrivate_TermAvg1StartFuelCons, mPrivate_TermAvg2StartFuelCons) == true) {
                mPrivate_TermAvgFuelEcoKmL = 999.0;
                mPrivate_TermAvgFuelEcoKmKg = 999.0;
            } else {
                mPrivate_TermAvgFuelEcoKmL = 0.0;
                mPrivate_TermAvgFuelEcoKmKg = 0.0;
            }
            if ((mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 ||
                 mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) &&
                almostEqual(mPrivate_TermAvg1StartDistance, mPrivate_TermAvg2StartDistance) == false) {
                if (almostEqual(mPrivate_TermAvgFuelEcoKmL, 0.0) == false) {
                    mPrivate_TermAvgFuelEcoL100km = 1.0 / mPrivate_TermAvgFuelEcoKmL * 100.0;
                } else {
                    mPrivate_TermAvgFuelEcoL100km = 999.0;
                }
                if (almostEqual(mPrivate_TermAvgFuelEcoKmKg, 0.0) == false) {
                    mPrivate_TermAvgFuelEcoKg100Km = 1.0 / mPrivate_TermAvgFuelEcoKmKg * 100.0;
                } else {
                    mPrivate_TermAvgFuelEcoKg100Km = 999.0;
                }
            } else if (almostEqual(mPrivate_TermAvg1StartDistance, mPrivate_TermAvg2StartDistance) == true) {
                mPrivate_TermAvgFuelEcoL100km = 999.0;
                mPrivate_TermAvgFuelEcoKg100Km = 999.0;
            } else {
                mPrivate_TermAvgFuelEcoL100km = 0.0;
                mPrivate_TermAvgFuelEcoKg100Km = 0.0;
            }
            if (surelyGreaterThan(std::abs(mPrivate_TermAvg1StartDistance - mPrivate_TermAvg2StartDistance), 0.05) == true &&
                almostEqual(mPrivate_TermAvg1StartFuelCons, mPrivate_TermAvg2StartFuelCons) == true) {
                mPrivate_TermAvgFuelEcoUsMPG = 999.0;
                mPrivate_TermAvgFuelEcoUkMPG = 999.0;
                mPrivate_TermAvgFuelEcoMpge = 999.0;
            } else {
                mPrivate_TermAvgFuelEcoUsMPG = mPrivate_TermAvgFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
                mPrivate_TermAvgFuelEcoUkMPG = mPrivate_TermAvgFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
                mPrivate_TermAvgFuelEcoMpge = mPrivate_TermAvgFuelEcoKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
            }
        } else {
            mPrivate_TermAvgFuelEcoKmL = 0.0;
            mPrivate_TermAvgFuelEcoL100km = 0.0;
            mPrivate_TermAvgFuelEcoUsMPG = mPrivate_TermAvgFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUsGal;
            mPrivate_TermAvgFuelEcoUkMPG = mPrivate_TermAvgFuelEcoKmL * mFactor_ConvKmToMi / mFactor_ConvLitterToUkGal;
            mPrivate_TermAvgFuelEcoKmKg = 0.0;
            mPrivate_TermAvgFuelEcoKg100Km = 0.0;
            mPrivate_TermAvgFuelEcoMpge = mPrivate_TermAvgFuelEcoKmKg * mFactor_ConvKmToMi / mFactor_ConvKgToGal;
        }
        _DDebug() << "2.5m mPrivate_TermAvgFuelEcoKmL: " << mPrivate_TermAvgFuelEcoKmL << "= " << mPrivate_TermAvg1StartDistance
                  << ", " << mPrivate_TermAvg2StartDistance << " / " << mPrivate_TermAvg1StartFuelCons << ", "
                  << mPrivate_TermAvg2StartFuelCons;

        updatePrivateTermAvgFuelEconomyValue();
    }

    // 4.14.6 송출 연비 종류 판단
    void updatePrivateTermAvgFuelEconomyValue() {
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoL100km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoUkMPG;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                        return;
                    }
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            } else {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoKg100Km;
                    } else {
                        DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    mPrivate_TermAvgFuelEconomyValue = mPrivate_TermAvgFuelEcoMpge;
                } else {
                    DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                    return;
                }
            }
            updateOutputTermAvgFuelConsValueDigit();
            updateInterTermAvgFuelConsValue();
        }
    }

    // 4.14.7 연비값 신호 송출 Value
    void updateOutputTermAvgFuelConsValue() {
        // Inter_ConfigVehicleType == HEV, PHEV, FCEV
        HDouble outputTermAvgFuelConsValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::RUNNING) {
                if (mPrivate_TermAvgFuelEconomyValue < 100) {
                    outputTermAvgFuelConsValue =
                        round(mPrivate_TermAvgFuelEconomyValue * 10.0) / 10.0 * kOutputTermAvgFuelConsValueFactor;
                } else if (mPrivate_TermAvgFuelEconomyValue >= 100 && mPrivate_TermAvgFuelEconomyValue < 999) {
                    outputTermAvgFuelConsValue = round(mPrivate_TermAvgFuelEconomyValue);
                } else if (mPrivate_TermAvgFuelEconomyValue >= 999) {
                    outputTermAvgFuelConsValue = 999;
                } else {
                    // N.A
                }

                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValue,
                                    static_cast<HUInt64>(outputTermAvgFuelConsValue));
                DDebug() << "outputTermAvgFuelConsValue: " << outputTermAvgFuelConsValue;
            } else if (mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::READY ||
                       mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::START) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValue, 1023u);
                DDebug() << "outputTermAvgFuelConsValue: 1023u";
            } else {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValue, 0u);
                DDebug() << "outputTermAvgFuelConsValue: 0u";
            }
        } else {
            updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValue, 0u);
            DDebug() << "outputTermAvgFuelConsValue: 0u, IGN OFF";
        }
    }

    // 4.14.7 연비값 신호 송출 ValueDigit
    void updateOutputTermAvgFuelConsValueDigit() {
        if (mPrivate_TermAvgFuelEconomyValue < 100.0) {
            DDebug() << "Output_TermAvgFuelConsValueDigit: digit2";
            updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValueDigit,
                                static_cast<HUInt64>(Output_TermAvgFuelConsValueDigit::DIGIT2));
        } else {
            DDebug() << "Output_TermAvgFuelConsValueDigit: digit3";
            updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValueDigit,
                                static_cast<HUInt64>(Output_TermAvgFuelConsValueDigit::DIGIT3));
        }
    }

    // 4.14.7 연비값 신호 송출 Inter_TermAvgFuelConsValue
    void updateInterTermAvgFuelConsValue() {
        HDouble interTermAvgFuelConsValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::RUNNING) {
                if (mPrivate_TermAvgFuelEconomyValue < 999.0) {
                    interTermAvgFuelConsValue = mPrivate_TermAvgFuelEconomyValue;
                } else {
                    interTermAvgFuelConsValue = 999.0;
                }
            } else if (mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::READY ||
                       mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::START) {
                interTermAvgFuelConsValue = 1023.0;
            } else {
                // no operation
            }
        }
        setSFCTrip_InfoInter_TermAvgFuelConsValue(interTermAvgFuelConsValue);
    }

    // 4.14.7 연비값 신호 송출 Inter_TermAvgFuelConsValid
    void updateInterTermAvgFuelConsValid() {
        SFCTrip_InfoInter_TermAvgFuelConsValid interTermAvgFuelConsValid = SFCTrip_InfoInter_TermAvgFuelConsValid::INVALID;
        if (mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::READY ||
            mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::START ||
            mPrivate_TermAvgSignalTxModeStatus == Private_TermAvgSignalTxModeStatus::RUNNING) {
            interTermAvgFuelConsValid = SFCTrip_InfoInter_TermAvgFuelConsValid::VALID;
        }
        setSFCTrip_InfoInter_TermAvgFuelConsValid(interTermAvgFuelConsValid);
    }

    // 4.14.8 SYNC 신호 송출
    void updateOutputTermAvgFuelConsSyncCounter() {
        if (mIgnitionState == IgnitionState::ON &&
            mOutput_TermAvgFuelConsSyncCounter == Output_TermAvgFuelConsSyncCounter::TIME0 &&
            ((mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
              mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) ||
             (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
              mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1))) {
            mOutput_TermAvgFuelConsSyncCounter = Output_TermAvgFuelConsSyncCounter::TIME1;
            DDebug() << "Output_TermAvgFuelConsSyncCounter: TIME1";
        } else if (mIgnitionState == IgnitionState::ON &&
                   mOutput_TermAvgFuelConsSyncCounter == Output_TermAvgFuelConsSyncCounter::TIME1 &&
                   ((mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                     mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) ||
                    (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                     mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1))) {
            mOutput_TermAvgFuelConsSyncCounter = Output_TermAvgFuelConsSyncCounter::TIME2;
            DDebug() << "Output_TermAvgFuelConsSyncCounter: TIME2";
        } else if (mIgnitionState == IgnitionState::ON &&
                   mOutput_TermAvgFuelConsSyncCounter == Output_TermAvgFuelConsSyncCounter::TIME2 &&
                   ((mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                     mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) ||
                    (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                     mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1))) {
            mOutput_TermAvgFuelConsSyncCounter = Output_TermAvgFuelConsSyncCounter::TIME3;
            DDebug() << "Output_TermAvgFuelConsSyncCounter: TIME3";
        } else if ((mIgnitionState == IgnitionState::ON &&
                    ((mOutput_TermAvgFuelConsSyncCounter == Output_TermAvgFuelConsSyncCounter::TIME3 &&
                      ((mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1 &&
                        mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2) ||
                       (mPrevPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON2 &&
                        mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::ON1))) ||
                     mPrivate_TermAvgCalModeStatus == Private_TermAvgCalModeStatus::OFF)) ||
                   mIgnitionState == IgnitionState::OFF) {
            mOutput_TermAvgFuelConsSyncCounter = Output_TermAvgFuelConsSyncCounter::TIME0;
            DDebug() << "Output_TermAvgFuelConsSyncCounter: TIME0";
        } else {
            // default
        }
        updateActuatorValue(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsSyncCounter,
                            static_cast<HUInt64>(mOutput_TermAvgFuelConsSyncCounter));
    }

    // 4.15 주행 후 연비 측정 및 신호 송출
    // 4.15.1 주행 후 연비 측정
    void updatePrivateAfterDriveFuelEcoValue() {
        if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) &&
            mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
            mPrivate_AfterDriveFuelEcoValue = mInter_DriveInfoFuelEcoValue;
            mPrivate_AfterDriveDistanceKmValue = mInter_DriveInfoDistanceKm;
            mPrivate_AfterDriveDistanceMiValue = mInter_DriveInfoDistanceMi;
            updateValue("SFC.Extension.Memory_IGN.Inter_AfterDriveFuelEcoValue", mPrivate_AfterDriveFuelEcoValue);
            updateInterDriveInfoAvgFuelConsValue();
            updateInterDrvInfoDistanceValue();
        }
    }

    // 4.15.2 RESET 여부 판단
    void updatePrivateDriveInfoResetStatus() {
        if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
            mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            if (mIgnitionState == IgnitionState::ON && mPrevPrivate_ADMStatus == Private_ADMStatus::OFF &&
                mPrivate_ADMStatus == Private_ADMStatus::ON) {
                mPrivate_DriveInfoResetStatus = Private_DriveInfoResetStatus::RESET;
            } else if (mIgnitionState == IgnitionState::OFF) {
                mPrivate_DriveInfoResetStatus = Private_DriveInfoResetStatus::OFF;
            } else {
                // default
            }
            updateOutputDrvInfoAvgFuelConsResetStatus();
            updateInterDrvInfoAvgFuelConsResetStatus();
        }
    }

    // 4.15.3 신호 송출 Output_DrvInfoAvgFuelConsValue
    void updateOutputDrvInfoAvgFuelConsValue() {
        if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) &&
            mIgnitionState == IgnitionState::ON) {
            HDouble outputDrvInfoAvgFuelConsValue = 0.0;

            if (mPrivate_AfterDriveFuelEcoValue < 100) {
                outputDrvInfoAvgFuelConsValue =
                    round(mPrivate_AfterDriveFuelEcoValue * 10.0) / 10.0 * kOutputDrvInfoAvgFuelConsValueFactor;
            } else if (mPrivate_AfterDriveFuelEcoValue >= 100 && mPrivate_AfterDriveFuelEcoValue < 999) {
                outputDrvInfoAvgFuelConsValue = round(mPrivate_AfterDriveFuelEcoValue);
            } else if (mPrivate_AfterDriveFuelEcoValue >= 999) {
                outputDrvInfoAvgFuelConsValue = 999;
            } else {
                // N.A
            }

            updateActuatorValue(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsValue.getAddress(),
                                static_cast<HUInt64>(outputDrvInfoAvgFuelConsValue));
        }
    }

    // 4.15.3 신호 송출 Output_DrvInfoAvgFuelConsDigit
    void updateOutputDrvInfoAvgFuelConsDigit() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_AfterDriveFuelEcoValue < 100.0) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsDigit.getAddress(),
                                    static_cast<HUInt64>(Output_DrvInfoAvgFuelConsDigit::DIGIT2));
            } else {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsDigit.getAddress(),
                                    static_cast<HUInt64>(Output_DrvInfoAvgFuelConsDigit::DIGIT3));
            }
        }
    }

    // 4.15.3 신호 송출 Output_DrvInfoAvgFuelConsResetStatus
    void updateOutputDrvInfoAvgFuelConsResetStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DriveInfoResetStatus == Private_DriveInfoResetStatus::RESET) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsResetStatus.getAddress(),
                                    static_cast<HUInt64>(Output_DrvInfoAvgFuelConsResetStatus::RESET));
            } else {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsResetStatus.getAddress(),
                                    static_cast<HUInt64>(Output_DrvInfoAvgFuelConsResetStatus::DEFAULT));
            }
        }
    }

    // 4.15.3 신호 송출 Inter_DrvInfoAvgFuelConsValue
    void updateInterDriveInfoAvgFuelConsValue() {
        setSFCTrip_InfoInter_DrvInfoAvgFuelConsValue(mPrivate_AfterDriveFuelEcoValue);
    }

    // 4.15.3 신호 송출 Inter_DrvInfoAvgFuelConsResetStatus
    void updateInterDrvInfoAvgFuelConsResetStatus() {
        SFCTrip_InfoInter_DrvInfoAvgFuelConsResetStatus stat = SFCTrip_InfoInter_DrvInfoAvgFuelConsResetStatus::DEFAULT;
        if (mPrivate_DriveInfoResetStatus == Private_DriveInfoResetStatus::RESET) {
            stat = SFCTrip_InfoInter_DrvInfoAvgFuelConsResetStatus::RESET;
        }
        setSFCTrip_InfoInter_DrvInfoAvgFuelConsResetStatus(stat);
    }

    // 4.15.3 신호 송출 Inter_DrvInfoDistanceValue
    void updateInterDrvInfoDistanceValue() {
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
            setSFCTrip_InfoInter_DrvInfoDistanceValue(mPrivate_AfterDriveDistanceKmValue);
        } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            setSFCTrip_InfoInter_DrvInfoDistanceValue(mPrivate_AfterDriveDistanceMiValue);
        } else {
            // no operation
        }
    }

    // ~Private Logic

    // 5.2 표시 구성요소 정보 처리 로직
    // 5.2.1 순간 연비 (이동 평균 계산 방식)
    void updateInstantaneousFuelEconomy() {
        HDouble instFuelEconomyValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoL100km;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoUkMPG;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else {
                    DDebug() << "instFuelEconomyValue is zero, but is not update.";
                    return;
                }
            } else {
                instFuelEconomyValue = 0.0;
            }
        } else {
            instFuelEconomyValue = 0.0;
        }
        DDebug() << "InstFuelEconomyValue : " << static_cast<HDouble>(instFuelEconomyValue);
        setSFCTrip_InfoConstantInstFuelEconomyValue(instFuelEconomyValue);
    }

    // 5.2.2 순간 연비 (입력 기반 표시 방식)
    void updateInstFuelEconomyInputType() {
        HDouble instFuelEconomyValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputL100km;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputUkMPG;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else {
                    DDebug() << "instFuelEconomyValue is zero, but is not update.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKwh100km;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    instFuelEconomyValue = mPrivate_InstFuelEcoInputMiKwh;
                } else {
                    DDebug() << "instFuelEconomyValue is zero, but is not update.";
                    return;
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKg100Km;
                    } else {
                        DDebug() << "instFuelEconomyValue is zero, but is not update.";
                        return;
                    }
                } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                    instFuelEconomyValue = mPrivate_InstFuelEcoInputMileKg;
                } else {
                    DDebug() << "instFuelEconomyValue is zero, but is not update.";
                    return;
                }
            } else {
                instFuelEconomyValue = 0.0;
            }
        } else {
            instFuelEconomyValue = 0.0;
        }
        DDebug() << "InstFuelEconomyValue : " << static_cast<HDouble>(instFuelEconomyValue);
        setSFCTrip_InfoConstantInstFuelEconomyInputTypeValue(instFuelEconomyValue);
    }

    // 5.2.3 누적 정보
    // 5.2.3.1 누적 거리
    void updateAccumulatedInfoDistance() {
        bool isDisplayOn = false;
        HDouble accumulDistanceValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                isDisplayOn = true;
                accumulDistanceValue = mPrivate_AccumDistanceKm;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                isDisplayOn = true;
                accumulDistanceValue = mPrivate_AccumDistanceMi;
            } else {
                DDebug() << "[DISPLAY_OFF] conditions.";
            }
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }
        if (isDisplayOn) {
            DDebug() << "[DISPLAY] AccumulDistanceValue : " << static_cast<HDouble>(accumulDistanceValue);
            setSFCTrip_InfoConstantAccumulDistanceValue(accumulDistanceValue);
            setSFCTrip_InfoConstantAccumulDistanceStat(SFCTrip_InfoConstantAccumulDistanceStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAccumulDistanceStat(SFCTrip_InfoConstantAccumulDistanceStat::DISPLAY_OFF);
        }
    }

    // 5.2.3.1 누적 시간
    void updateAccumulatedInfoTime() {
        if (mIgnitionState == IgnitionState::ON) {
            DDebug() << "[DISPLAY] AccumulTimeValue : " << static_cast<HUInt64>(mPrivate_AccumTime);
            setSFCTrip_InfoConstantAccumulTimeValue(mPrivate_AccumTime);
            setSFCTrip_InfoConstantAccumulTimeStat(SFCTrip_InfoConstantAccumulTimeStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAccumulTimeStat(SFCTrip_InfoConstantAccumulTimeStat::DISPLAY_OFF);
        }
    }

    // 5.2.3.1 누적 속도
    void updateAccumulatedInfoSpeed() {
        bool isDisplayOn = false;
        HUInt64 accumulSpeedValue = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                isDisplayOn = true;
                accumulSpeedValue = mPrivate_AccumVehicleSpeedKph;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                isDisplayOn = true;
                accumulSpeedValue = mPrivate_AccumVehicleSpeedMph;
            } else {
                DDebug() << "[DISPLAY_OFF] conditions.";
            }
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }
        if (isDisplayOn) {
            DDebug() << "[DISPLAY] AccumulSpeedValue : " << static_cast<HUInt64>(accumulSpeedValue);
            setSFCTrip_InfoConstantAccumulSpeedValue(accumulSpeedValue);
            setSFCTrip_InfoConstantAccumulSpeedStat(SFCTrip_InfoConstantAccumulSpeedStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAccumulSpeedStat(SFCTrip_InfoConstantAccumulSpeedStat::DISPLAY_OFF);
        }
    }

    // 5.2.3.2 누적 연비
    void updateAccumulatedFuelEconomy() {
        HDouble accumulFuelEconomyValue = 0.0;
        SFCTrip_InfoConstantAccumulFuelEconomyStat stat = SFCTrip_InfoConstantAccumulFuelEconomyStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_AccumDistanceKm >= kMinDistanceForUpdateFuelEconomyValue && mPrivate_AccumFuelEconomyValue < 100) {
                accumulFuelEconomyValue = round((mPrivate_AccumFuelEconomyValue)*10.0) / 10.0;
            } else if (mPrivate_AccumDistanceKm >= kMinDistanceForUpdateFuelEconomyValue &&
                       mPrivate_AccumFuelEconomyValue >= 100) {
                accumulFuelEconomyValue = round(mPrivate_AccumFuelEconomyValue);
            } else {
                accumulFuelEconomyValue = 0.0;
                stat = SFCTrip_InfoConstantAccumulFuelEconomyStat::UNMEASURABLE;
            }
        } else {
            stat = SFCTrip_InfoConstantAccumulFuelEconomyStat::DISPLAY_OFF;
        }

        DDebug() << "AccumulFuelEconomyValue : " << static_cast<HDouble>(accumulFuelEconomyValue);
        DDebug() << "AccumulFuelEconomyStat : " << static_cast<uint64_t>(stat);
        if (mIgnitionState == IgnitionState::ON) {
            setSFCTrip_InfoConstantAccumulFuelEconomyValue(accumulFuelEconomyValue);
        }
        setSFCTrip_InfoConstantAccumulFuelEconomyStat(stat);
    }

    // 5.2.4 주행정보
    // 5.2.4.1 주행 거리
    void updateDrivingInfoDistance() {
        bool isDisplayOn = false;
        HDouble driveDistanceValue = 0.0;
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
            isDisplayOn = true;
            driveDistanceValue = mInter_DriveInfoDistanceKm;
        } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            isDisplayOn = true;
            driveDistanceValue = mInter_DriveInfoDistanceMi;
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }

        if (isDisplayOn == true) {
            DDebug() << "[DISPLAY] DriveDistanceValue : " << static_cast<HDouble>(driveDistanceValue);
            setSFCTrip_InfoConstantDriveDistanceValue(driveDistanceValue);
            setSFCTrip_InfoConstantDriveDistanceStat(SFCTrip_InfoConstantDriveDistanceStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantDriveDistanceStat(SFCTrip_InfoConstantDriveDistanceStat::DISPLAY_OFF);
        }
    }

    // 5.2.4.1 주행 시간
    void updateDrivingInfoTime() {
        DDebug() << "[DISPLAY] DriveTimeValue : " << static_cast<HUInt64>(mInter_DriveInfoTime);
        setSFCTrip_InfoConstantDriveTimeValue(mInter_DriveInfoTime);
        setSFCTrip_InfoConstantDriveTimeStat(SFCTrip_InfoConstantDriveTimeStat::DISPLAY);
    }

    // 5.2.3.1 주행 속도
    void updateDrivingInfoSpeed() {
        bool isDisplayOn = false;
        HUInt64 driveSpeedValue = 0;
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
            isDisplayOn = true;
            driveSpeedValue = mInter_DriveInfoVehicleSpeedKph;
        } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            isDisplayOn = true;
            driveSpeedValue = mInter_DriveInfoVehicleSpeedMph;
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }

        if (isDisplayOn == true) {
            DDebug() << "[DISPLAY] DriveSpeedValue : " << static_cast<HUInt64>(driveSpeedValue);
            setSFCTrip_InfoConstantDriveSpeedValue(driveSpeedValue);
            setSFCTrip_InfoConstantDriveSpeedStat(SFCTrip_InfoConstantDriveSpeedStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantDriveSpeedStat(SFCTrip_InfoConstantDriveSpeedStat::DISPLAY_OFF);
        }
    }

    // 5.2.4.2 주행 연비
    void updateDrivingInfoFuelEconomy() {
        HDouble driveFuelEconomyValue = 0.0;
        SFCTrip_InfoConstantDriveFuelEconomyStat stat = SFCTrip_InfoConstantDriveFuelEconomyStat::DISPLAY;
        if (mInter_DriveInfoDistanceKm >= kMinDistanceForUpdateFuelEconomyValue && mInter_DriveInfoFuelEcoValue < 100) {
            driveFuelEconomyValue = round((mInter_DriveInfoFuelEcoValue)*10.0) / 10.0;
        } else if (mInter_DriveInfoDistanceKm >= kMinDistanceForUpdateFuelEconomyValue && mInter_DriveInfoFuelEcoValue >= 100) {
            driveFuelEconomyValue = round(mInter_DriveInfoFuelEcoValue);
        } else {
            driveFuelEconomyValue = 0.0;
            stat = SFCTrip_InfoConstantDriveFuelEconomyStat::UNMEASURABLE;
        }

        DDebug() << "DriveFuelEconomyValue : " << static_cast<HDouble>(driveFuelEconomyValue);
        DDebug() << "DriveFuelEconomyStat : " << static_cast<uint64_t>(stat);
        setSFCTrip_InfoConstantDriveFuelEconomyValue(driveFuelEconomyValue);
        setSFCTrip_InfoConstantDriveFuelEconomyStat(stat);
    }

    // 5.2.5 주유 후 정보
    // 5.2.5.1 주유 후 거리
    void updateAfterRefuelDistance() {
        bool isDisplayOn = false;
        HDouble afterRefuelDistanceValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                isDisplayOn = true;
                afterRefuelDistanceValue = mPrivate_AfterRefuelDistanceKm;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                isDisplayOn = true;
                afterRefuelDistanceValue = mPrivate_AfterRefuelDistanceMi;
            } else {
                DDebug() << "[DISPLAY_OFF] conditions.";
            }
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }
        if (isDisplayOn == true) {
            DDebug() << "[DISPLAY] AfterRefuelDistanceValue : " << static_cast<HDouble>(afterRefuelDistanceValue);
            setSFCTrip_InfoConstantAfterRefuelDistanceValue(afterRefuelDistanceValue);
            setSFCTrip_InfoConstantAfterRefuelDistanceStat(SFCTrip_InfoConstantAfterRefuelDistanceStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAfterRefuelDistanceStat(SFCTrip_InfoConstantAfterRefuelDistanceStat::DISPLAY_OFF);
        }
    }

    // 5.2.5.1 주유 후 시간
    void updateAfterRefuelTime() {
        if (mIgnitionState == IgnitionState::ON) {
            DDebug() << "[DISPLAY] AfterRefuelTimeValue : " << static_cast<HUInt64>(mPrivate_AfterRefuelTime);
            setSFCTrip_InfoConstantAfterRefuelTimeValue(mPrivate_AfterRefuelTime);
            setSFCTrip_InfoConstantAfterRefuelTimeStat(SFCTrip_InfoConstantAfterRefuelTimeStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAfterRefuelTimeStat(SFCTrip_InfoConstantAfterRefuelTimeStat::DISPLAY_OFF);
        }
    }

    // 5.2.5.1 주유 후 속도
    void updateAfterRefuelSpeed() {
        bool isDisplayOn = false;
        HUInt64 afterRefuelSpeedValue = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                isDisplayOn = true;
                afterRefuelSpeedValue = mPrivate_AfterRefuelVehicleSpeedKph;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                isDisplayOn = true;
                afterRefuelSpeedValue = mPrivate_AfterRefuelVehicleSpeedMph;
            } else {
                DDebug() << "[DISPLAY_OFF] conditions.";
            }
        } else {
            DDebug() << "[DISPLAY_OFF] conditions.";
        }
        if (isDisplayOn) {
            DDebug() << "[DISPLAY] AfterRefuelSpeedValue : " << static_cast<HUInt64>(afterRefuelSpeedValue);
            setSFCTrip_InfoConstantAfterRefuelSpeedValue(afterRefuelSpeedValue);
            setSFCTrip_InfoConstantAfterRefuelSpeedStat(SFCTrip_InfoConstantAfterRefuelSpeedStat::DISPLAY);
        } else {
            DDebug() << "[DISPLAY_OFF]";
            setSFCTrip_InfoConstantAfterRefuelSpeedStat(SFCTrip_InfoConstantAfterRefuelSpeedStat::DISPLAY_OFF);
        }
    }

    // 5.2.5.2 주유 후 연비
    void updateAfterRefuelFuelEconomy() {
        HDouble afterRefuelFuelEconomyValue = 0.0;
        SFCTrip_InfoConstantAfterRefuelFuelEconomyStat stat = SFCTrip_InfoConstantAfterRefuelFuelEconomyStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_AfterRefuelDistanceKm >= kMinDistanceForUpdateFuelEconomyValue &&
                mPrivate_AfterRefuelFuelEconomyValue < 100) {
                afterRefuelFuelEconomyValue = round((mPrivate_AfterRefuelFuelEconomyValue)*10.0) / 10.0;
            } else if (mPrivate_AfterRefuelDistanceKm >= kMinDistanceForUpdateFuelEconomyValue &&
                       mPrivate_AfterRefuelFuelEconomyValue >= 100) {
                afterRefuelFuelEconomyValue = round(mPrivate_AfterRefuelFuelEconomyValue);
            } else {
                afterRefuelFuelEconomyValue = 0.0;
                stat = SFCTrip_InfoConstantAfterRefuelFuelEconomyStat::UNMEASURABLE;
            }
        } else {
            stat = SFCTrip_InfoConstantAfterRefuelFuelEconomyStat::DISPLAY_OFF;
        }

        DDebug() << "AfterRefuelFuelEconomyValue : " << static_cast<HDouble>(afterRefuelFuelEconomyValue);
        DDebug() << "AfterRefuelFuelEconomyStat : " << static_cast<uint64_t>(stat);
        if (mIgnitionState == IgnitionState::ON) {
            setSFCTrip_InfoConstantAfterRefuelFuelEconomyValue(afterRefuelFuelEconomyValue);
        }
        setSFCTrip_InfoConstantAfterRefuelFuelEconomyStat(stat);
    }

    // 5.2.6 누적 주행 거리
    void updateOdometer() {
        if (!mAEMInitCompleteFlag) {
            DDebug() << "AEM is not initialized.";
            return;
        }

        // Km는 Default로 계산하는 것으로 변경
        mOdometerValue = static_cast<HUInt64>(floor(mPrivate_OdometerTotal));
        HDouble odometerDouble = roundOff2DecimalValue(mPrivate_OdometerTotal) * kOutputOdometerFactor;
        mOutputOdometerValue = static_cast<HUInt64>(floor(odometerDouble));
        // mOutputOdometerValue 값의 resolution이 1/kOutputOdometerFactor 이므로 0.1 증가시마다 아래 조건문이 만족하게 됨
        // Mi 계산전에 OutputOdometerValue 호출하여 Km로만 송출
        if (mPrevOutputOdometerValue != mOutputOdometerValue || mPrevOutputOdometerValue == kOutputOdometerMaxValue) {
            if (mOutputOdometerValue > kOutputOdometerMaxValue) {
                mOutputOdometerValue = kOutputOdometerMaxValue;
            }
            updateOutputOdometerValue(mOutputOdometerValue);
            mPrevOutputOdometerValue = mOutputOdometerValue;
        }

        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            mOdometerValue = static_cast<HUInt64>(floor(mPrivate_OdometerTotal * mFactor_ConvKmToMi));
            HDouble odometerDouble = roundOff2DecimalValue(mPrivate_OdometerTotal * mFactor_ConvKmToMi) * kOutputOdometerFactor;
            mOutputOdometerValue = static_cast<HUInt64>(floor(odometerDouble));
        }

        if (mOdometerValue == UINT64_MAX) {
            DDebug() << "odometer value is UINT64_MAX";
            return;
        }

        // mOdometerValue의 resolution은 1이므로 1 증가시마다 아래 조건문이 만족하게 됨
        if (mPrevOdometerInteger != mOdometerValue || mOdometerValue == 0) {
            DDebug() << "Total Odometer Value : " << static_cast<HUInt64>(mOdometerValue);
            setSFCTrip_InfoConstantOdometerValue(mOdometerValue);
            mOdometerDiff += mOdometerValue - mPrevOdometerInteger;
            if (mOdometerDiff >= 1) {
                mOdometerDiff -= 1;
                saveDataToMemoryPeriodic();
            }
            mPrevOdometerInteger = mOdometerValue;
        }
    }

    // 5.2.7 EV 주행 가능 거리
    void updateElecDTE() {
        HUInt64 elecDTEValue = 0;
        SFCTrip_InfoConstantElecDTEStat stat = SFCTrip_InfoConstantElecDTEStat::VALUE;
        if (mIgnitionState == IgnitionState::OFF && mElecDTEStatus == ElecDTEStatus::MESSAGE_TIMEOUT) {
            stat = SFCTrip_InfoConstantElecDTEStat::DISPLAY_OFF;
        } else {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                elecDTEValue = mInput_EvDTEValuePerKm;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                elecDTEValue = static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * mFactor_ConvKmToMi));
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
        }

        DDebug() << "ElecDTEValue : " << elecDTEValue;
        setSFCTrip_InfoConstantElecDTEStat(stat);
        setSFCTrip_InfoConstantElecDTEValue(elecDTEValue);
    }

    // 5.2.8 단위
    void updateFuelEcoUnit() {
        SFCTrip_InfoConstantFuelEcoUnitStat fuelEcoUnitStat;
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
            mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_LITTER;
                } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::LITTER_PER_KM;
                } else {
                    DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                    return;
                }
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::US_GALLON) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::US_MPG;
                } else if (mInter_FuelEcoUnitMiSetStatus == Inter_FuelEcoUnitMiSetStatus::UK_GALLON) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::UK_MPG;
                } else {
                    DDebug() << "Inter_FuelEcoUnitMiSetStatus is not US_GALLON or UK_GALLON.";
                    return;
                }
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
        } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_KWH;
                } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KWH_PER_KM;
                } else {
                    DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                    return;
                }
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::MILE_PER_KWH;
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
        } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_KG;
                } else if (mInter_FuelEcoUnitKmSetStatus == Inter_FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KG_PER_KM;
                } else {
                    DDebug() << "Inter_FuelEcoUnitKmSetStatus is not KM_PER_LITTER or LITTER_PER_KM.";
                    return;
                }
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::MPGE;
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
        } else {
            DDebug() << "Inter_ConfigVehicleType is not operation condition.";
            return;
        }
        setSFCTrip_InfoConstantFuelEcoUnitStat(fuelEcoUnitStat);
    }

    // 5.2.8 단위
    void updateTripDistanceUnit() {
        SFCTrip_InfoConstantTripDistanceUnitStat tripDistanceUnit;
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
            tripDistanceUnit = SFCTrip_InfoConstantTripDistanceUnitStat::KM;
        } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            tripDistanceUnit = SFCTrip_InfoConstantTripDistanceUnitStat::MILE;
        } else {
            DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
            return;
        }
        setSFCTrip_InfoConstantTripDistanceUnitStat(tripDistanceUnit);
    }

    // 5.2.8 단위
    void updateTripVehicleSpeedUnit() {
        SFCTrip_InfoConstantTripVehicleSpeedUnitStat tripVehicleSpeedUnit;
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
            tripVehicleSpeedUnit = SFCTrip_InfoConstantTripVehicleSpeedUnitStat::KM_PER_HOUR;
        } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            tripVehicleSpeedUnit = SFCTrip_InfoConstantTripVehicleSpeedUnitStat::MILE_PER_HOUR;
        } else {
            DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
            return;
        }
        setSFCTrip_InfoConstantTripVehicleSpeedUnitStat(tripVehicleSpeedUnit);
    }

    // 5.2.9 세부 항목 명칭 표시 형태
    void updateTripInfoSubTitleType() {
        if (mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::KOREA ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::SIMPLIFIED_CHINESE ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::ENGLISH_US ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::FRENCH_US ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::SPANISH_US ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::TRADITIONAL_CHINESE ||
            mInter_DisplayLanguageStatus == Inter_DisplayLanguageStatus::JAPANESE) {
            setSFCTrip_InfoConstantTripInfoSubTitleTypeStat(SFCTrip_InfoConstantTripInfoSubTitleTypeStat::TEXT);
        } else {
            setSFCTrip_InfoConstantTripInfoSubTitleTypeStat(SFCTrip_InfoConstantTripInfoSubTitleTypeStat::SYMBOL);
        }
    }

    // 5.2.10 주행가능거리 심볼 형상
    void updateDTESymbol() {
        SFCTrip_InfoConstantDTESymbolStat dteSymbolStat;
        if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
            mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::GAS_STATION;
        } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::GAS_STATION_PLUG;
        } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::PLUG;
        } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::H2;
        } else {
            DDebug() << "Inter_ConfigVehicleType is not operation condition.";
            return;
        }
        setSFCTrip_InfoConstantDTESymbolStat(dteSymbolStat);
    }

    // 5.2.11 EV 주행가능거리 경로 반영중 상태 표시
    void updateDTEAvnCalStat() {
        if (mInput_EvDTEAvnCalcStatus == Input_EvDTEAvnCalcStatus::ON) {
            setSFCTrip_InfoConstantDTEAvnCalStat(SFCTrip_InfoConstantDTEAvnCalStat::ON);
        } else if (mInput_EvDTEAvnCalcStatus == Input_EvDTEAvnCalcStatus::OFF) {
            setSFCTrip_InfoConstantDTEAvnCalStat(SFCTrip_InfoConstantDTEAvnCalStat::OFF);
        } else {
            // Do nothing
        }
    }

    // 5.2.12 EV 주행가능거리 OFF 표시
    void updateDTEDashDisplayForEvStat() {
        if (mInput_EvDTEDisplayOffReqStatus == Input_EvDTEDisplayOffReqStatus::OFF_REQUEST) {
            setSFCTrip_InfoConstantDTEDashDisplayForEvStat(SFCTrip_InfoConstantDTEDashDisplayForEvStat::ON);
        } else if (mInput_EvDTEDisplayOffReqStatus == Input_EvDTEDisplayOffReqStatus::DEFAULT) {
            setSFCTrip_InfoConstantDTEDashDisplayForEvStat(SFCTrip_InfoConstantDTEDashDisplayForEvStat::OFF);
        } else {
            // Do nothing
        }
    }

    // 5.2.13 EV 주행가능거리 오류 표시
    void updateDTEDashBlinkDisplayForEv() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mElecDTEStatus == ElecDTEStatus::MESSAGE_TIMEOUT) {
                setSFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat(SFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat::ON);
            } else {
                setSFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat(SFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat::OFF);
            }
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시
    // IMG_DTEGuideFunctionStatus_stat
    void updateConstantDTEGuideFunctionStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            switch (mInput_EvDteGuidePageInfoStatus) {
                case Input_EvDteGuidePageInfoStatus::NORMAL:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::NORMAL);
                    break;
                case Input_EvDteGuidePageInfoStatus::RECALCULATE:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(
                        SFCTrip_InfoConstantDTEGuideFunctionStatusStat::RECALCULATE);
                    break;
                case Input_EvDteGuidePageInfoStatus::LOW_DTE:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::LOW_DTE);
                    break;
                case Input_EvDteGuidePageInfoStatus::START:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::START);
                    break;
                default:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::NONE);
                    break;
            }
        }
    }

    // IMG_DTEGuideLowBound_value
    void updateConstantDTEGuideLowBoundValue() {
        HUInt64 value = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                value = mEvDteLowBoundValue;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                value = static_cast<HUInt64>(round(mEvDteLowBoundValue * mFactor_ConvKmToMi));
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
            DDebug() << "EvDteLowBoundValue : " << value;
            setSFCTrip_InfoConstantDTEGuideLowBoundValue(value);
        }
    }

    // IMG_DTEGuideUpperBound_value
    void updateConstantDTEGuideUpperBoundValue() {
        HUInt64 value = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                value = mEvDteUpperBoundValue;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                value = static_cast<HUInt64>(round(mEvDteUpperBoundValue * mFactor_ConvKmToMi));
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
            DDebug() << "EvDteUpperBoundValue : " << value;
            setSFCTrip_InfoConstantDTEGuideUpperBoundValue(value);
        }
    }

    // IMG_DTEGuideMainDtePosition_value
    void updateConstantDTEGuideMainDtePositionValue() {
        if (mIgnitionState == IgnitionState::ON && mEvDteGuideDtePositionValue <= kEvDteGuidePostionMaxValue) {
            setSFCTrip_InfoConstantDTEGuideMainDtePositionValue(
                static_cast<HInt64>(mEvDteGuideDtePositionValue - kEvDteGuidePostionOffset));
        }
    }

    // IMG_DTEGuideCoachPosition_value
    void updateConstantDTEGuideCoachPositionValue() {
        if (mIgnitionState == IgnitionState::ON && mEvDteGuideCoachPositionValue <= kEvDteGuidePostionMaxValue) {
            setSFCTrip_InfoConstantDTEGuideCoachPositionValue(
                static_cast<HInt64>(mEvDteGuideCoachPositionValue - kEvDteGuidePostionOffset));
        }
    }

    // IMG_DTEGuideCoach_value
    void updateConstantDTEGuideCoachValue() {
        HUInt64 value = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                value = mEvDteGuideRealTimeValue;
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                value = static_cast<HUInt64>(round(mEvDteGuideRealTimeValue * mFactor_ConvKmToMi));
            } else {
                DDebug() << "Inter_TripDistanceUnit is not KM or MILE.";
                return;
            }
            DDebug() << "EvDteGuideCoachValue : " << value;
            setSFCTrip_InfoConstantDTEGuideCoachValue(value);
        }
    }

    // IMG_DTEGuideCoachValueAvailable_stat
    void updateConstantDTEGuideCoachValueAvailableStat() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mEvDteGuideRealTimeValue != 0) {
                setSFCTrip_InfoConstantDTEGuideCoachValueAvailableStat(SFCTrip_InfoConstantDTEGuideCoachValueAvailableStat::ON);
            } else {
                setSFCTrip_InfoConstantDTEGuideCoachValueAvailableStat(SFCTrip_InfoConstantDTEGuideCoachValueAvailableStat::OFF);
            }
        }
    }

    // 4.13 현재값 외부 출력
    void updateOutputEvDteValue() {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_EvDTEValue.getAddress(), mInput_EvDTEValuePerKm);
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_EvDTEValue.getAddress(),
                                    static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * mFactor_ConvKmToMi)));
            } else {
                // Do nothing
            }
        }
    }

    void updateOutputTotalDTEValue() {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            if (mInput_EvDTEDisplayOffReqStatus == Input_EvDTEDisplayOffReqStatus::OFF_REQUEST) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TotalDTEValue.getAddress(), 0U);
            } else if (mInput_EvDTEDisplayOffReqStatus != Input_EvDTEDisplayOffReqStatus::OFF_REQUEST &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM &&
                       mElecDTEStatus != ElecDTEStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TotalDTEValue.getAddress(), mInput_EvDTEValuePerKm);
            } else if (mInput_EvDTEDisplayOffReqStatus != Input_EvDTEDisplayOffReqStatus::OFF_REQUEST &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE &&
                       mElecDTEStatus != ElecDTEStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TotalDTEValue.getAddress(),
                                    static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * mFactor_ConvKmToMi)));
            } else if (mInput_EvDTEDisplayOffReqStatus != Input_EvDTEDisplayOffReqStatus::OFF_REQUEST &&
                       mElecDTEStatus == ElecDTEStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TotalDTEValue.getAddress(), 4095U);
            } else {
                // Do nothing
            }
        }
    }

    void updateOutputTripUnitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TripUnitStatus.getAddress(),
                                    static_cast<HUInt64>(Output_TripUnitStatus::KM));
            } else if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                updateActuatorValue(Vehicle.CD.Trip_Info.Output_TripUnitStatus.getAddress(),
                                    static_cast<HUInt64>(Output_TripUnitStatus::MILE));
            } else {
                // no operation
            }
        }
    }

    // ~Display Output Logic

    enum class MemoryAddrName : HUInt64;
    std::string getMemNodeAddr(const Trip_Info::MemoryAddrName& name) const {
        return mMemoryAddressMap.at(name);
    }

    template <typename T>
    void checkReceivedMemoryData(const T& memoryData, T& privateData, const T& savedData) {
        if (memoryData == 0) {
            privateData = 0;
        } else {
            if (savedData == 0) {
                privateData += memoryData;
            } else {
                if (memoryData != savedData) {
                    privateData = memoryData;
                }
            }
        }
    }

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (!mActuatorOutputAddrList.empty()) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    enum class IgnitionState : HUInt64 {
        ON,
        OFF
    };

    enum class Private_ADMStatus : HUInt64 {
        NONE,
        ON,
        OFF
    };

    enum class Private_InstFuelEcoCalModeStatus : HUInt64 {
        NONE,
        INIT1,
        INIT2,
        ON1,
        ON2,
        OFF
    };

    enum class Private_TermAvgCalModeStatus : HUInt64 {
        NONE,
        ON1,
        ON2,
        OFF
    };

    enum class Private_TermAvgSignalTxModeStatus : HUInt64 {
        NONE,
        READY,
        START,
        RUNNING,
        OFF
    };

    enum class Private_DriveInfoResetStatus : HUInt64 {
        RESET,
        OFF
    };

    enum class MemoryAddrName : HUInt64 {
        DRIVING_TIME_VALUE,
        FUEL_CONSUMPT_VALUE,
        ELEC_FUEL_CONSUMPT_VALUE,
        HYDROGEN_FUEL_CONSUMPT_VALUE,
        ODOMETER_VALUE,
        ACCUM_START_DISTANCE_VALUE,
        ACCUM_START_ELEC_FUEL_CONS_VALUE,
        ACCUM_START_FUEL_CONS_VALUE,
        ACCUM_START_HYDROGEN_CONS_VALUE,
        ACCUM_START_TIME_VALUE,
        AFTER_REFUEL_START_DISTANCE_VALUE,
        AFTER_REFUEL_START_ELEC_FUEL_CONS_VALUE,
        AFTER_REFUEL_START_HYDROGEN_CONS_VALUE,
        AFTER_REFUEL_START_FUEL_CONS_VALUE,
        AFTER_REFUEL_START_TIME_VALUE,
        DRIVE_START_DISTANCE_VALUE,
        DRIVE_START_ELEC_FUEL_CONS_VALUE,
        DRIVE_START_HYDROGEN_CONS_VALUE,
        DRIVE_START_FUEL_CONS_VALUE,
        DRIVE_START_TIME_VALUE,
        AFTER_REFUEL_DRIVE_START_DISTANCE,
        AFTER_REFUEL_DRIVE_START_FUEL_CONS,
    };

    enum class ElecDTEStatus : HUInt64 {
        NONE,
        VALUE,
        MESSAGE_TIMEOUT,
        MAX
    };

    using Inter_ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using Inter_TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using Inter_FuelEcoUnitKmSetStatus = ArgumentsInterFuelEcoUnitKmSetStatusChanged::Inter_FuelEcoUnitKmSetStatus;
    using Inter_FuelEcoUnitMiSetStatus = ArgumentsInterFuelEcoUnitMiSetStatusChanged::Inter_FuelEcoUnitMiSetStatus;
    using Inter_DisplayLanguageStatus = ArgumentsInterDisplayLanguageStatusChanged::Inter_DisplayLanguageStatus;
    using Inter_EngineOnStatus = ArgumentsInterEngineOnStatusChanged::Inter_EngineOnStatus;
    using Inter_ISGActiveStatus = ArgumentsInterISGActiveStatusChanged::Inter_ISGActiveStatus;
    using Inter_FuelDetectRealtimeModeStatus =
        ArgumentsInterFuelDetectRealtimeModeStatusChanged::Inter_FuelDetectRealtimeModeStatus;
    using Input_H2RefuelDetectionStatus = ArgumentsInputH2RefuelDetectionStatusChanged::Input_H2RefuelDetectionStatus;
    using Inter_AfterRefuelInfoManualResetReq =
        ArgumentsInterAfterRefuelInfoManualResetReqChanged::Inter_AfterRefuelInfoManualResetReq;
    using Input_DoorOpenDriverSideStatus = ArgumentsInputDoorOpenDriverSideStatusChanged::Input_DoorOpenDriverSideStatus;
    using Input_DoorOpenDriverSideStatusFromSBCM =
        ArgumentsInputDoorOpenDriverSideStatusChanged::Input_DoorOpenDriverSideStatusFromSBCM;
    using Input_VehicleReadyStatus = ArgumentsInputVehicleReadyStatusChanged::Input_VehicleReadyStatus;
    using Output_AvgFuelEconomyUnitStatus = decltype(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyUnitStatus)::TYPE;
    using Output_AvgFuelEconomyDigitStatus = decltype(Vehicle.CD.Trip_Info.Output_AvgFuelEconomyDigitStatus)::TYPE;
    using Output_DrvInfoAvgFuelConsDigit = decltype(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsDigit)::TYPE;
    using Output_DrvInfoAvgFuelConsResetStatus = decltype(Vehicle.CD.Trip_Info.Output_DrvInfoAvgFuelConsResetStatus)::TYPE;
    using Output_TermAvgFuelConsValueDigit = decltype(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsValueDigit)::TYPE;
    using Output_TermAvgFuelConsSyncCounter = decltype(Vehicle.CD.Trip_Info.Output_TermAvgFuelConsSyncCounter)::TYPE;
    using Inter_AccumInfoManualResetReq = ArgumentsAccumInfoManualResetReqChanged::Inter_AccumInfoManualResetReq;
    using Inter_DriveInfoManualResetReq = ArgumentsDriveInfoManualResetReqChanged::Inter_DriveInfoManualResetReq;
    using Input_EvEnergyFlowStatus = ArgumentsInputEvEnergyFlowStatusChanged::Input_EvEnergyFlowStatus;
    using Input_HeadUnitFuelEcoGraphResetStatus =
        ArgumentsInputHeadUnitFuelEcoGraphResetStatusChanged::Input_HeadUnitFuelEcoGraphResetStatus;
    using Input_EvChargeStatus = ArgumentsEvChargeStatusAndEvSocDisplayValueChanged::Input_EvChargeStatus;
    using Input_EvDTEAvnCalcStatus = ArgumentsInputEvDTEAvnCalcStatus::Input_EvDTEAvnCalcStatus;
    using Input_EvDTEDisplayOffReqStatus = ArgumentsInputEvDTEDisplayOffReqStatus::Input_EvDTEDisplayOffReqStatus;
    using Output_TripUnitStatus = decltype(Vehicle.CD.Trip_Info.Output_TripUnitStatus)::TYPE;
    using Input_EvDteGuidePageInfoStatus = ArgumentsEvDteGuidePageInfoStatusChanged::Input_EvDteGuidePageInfoStatus;

    IgnitionState mIgnitionState = IgnitionState::OFF;
    IgnitionState mPrevIgnitionState = IgnitionState::OFF;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    HUInt64 mInput_FuelConsumptionValue = 0;
    HUInt64 mInput_EvDTEValuePerKm = 0;
    HInt64 mInput_EvBatteryCurrentValue = 0;
    HUInt64 mInput_EvBatteryVoltageValue = 0;
    Inter_TripDistanceUnit mInter_TripDistanceUnit = Inter_TripDistanceUnit::KM;
    Inter_FuelEcoUnitKmSetStatus mInter_FuelEcoUnitKmSetStatus = Inter_FuelEcoUnitKmSetStatus::MAX;
    Inter_FuelEcoUnitMiSetStatus mInter_FuelEcoUnitMiSetStatus = Inter_FuelEcoUnitMiSetStatus::MAX;
    Inter_DisplayLanguageStatus mInter_DisplayLanguageStatus = Inter_DisplayLanguageStatus::MAX;
    HDouble mInter_ActualVehicleSpeed = 0.0;
    HDouble mPrevInter_ActualVehicleSpeed = 0.0;
    HDouble mDiffDistance = 0.0;
    HUInt64 mOdometerValue = 0;
    HUInt64 mOutputOdometerValue = 0;
    HUInt64 mPrevOdometerInteger = 0;
    HUInt64 mPrevOutputOdometerValue = kOutputOdometerMaxValue;
    HUInt64 mOdometerDiff = 0;
    Inter_EngineOnStatus mInter_EngineOnStatus = Inter_EngineOnStatus::OFF;
    Inter_EngineOnStatus mPrevInter_EngineOnStatus = Inter_EngineOnStatus::OFF;
    Inter_ISGActiveStatus mInter_ISGActiveStatus = Inter_ISGActiveStatus::OFF;
    Inter_FuelDetectRealtimeModeStatus mInter_FuelDetectRealtimeModeStatus = Inter_FuelDetectRealtimeModeStatus::OFF;
    Input_H2RefuelDetectionStatus mInput_H2RefuelDetectionStatus = Input_H2RefuelDetectionStatus::OFF;
    Inter_AfterRefuelInfoManualResetReq mInter_AfterRefuelInfoManualResetReq = Inter_AfterRefuelInfoManualResetReq::DEFAULT;
    Inter_AfterRefuelInfoManualResetReq mPrevInter_AfterRefuelInfoManualResetReq = Inter_AfterRefuelInfoManualResetReq::DEFAULT;
    Input_DoorOpenDriverSideStatus mInput_DoorOpenDriverSideStatus = Input_DoorOpenDriverSideStatus::CLOSE;
    Input_DoorOpenDriverSideStatusFromSBCM mInput_DoorOpenDriverSideStatusFromSBCM =
        Input_DoorOpenDriverSideStatusFromSBCM::CLOSE;
    Input_VehicleReadyStatus mInput_VehicleReadyStatus = Input_VehicleReadyStatus::READY;
    Inter_AccumInfoManualResetReq mInter_AccumInfoManualResetReq = Inter_AccumInfoManualResetReq::DEFAULT;
    Inter_AccumInfoManualResetReq mPrevInter_AccumInfoManualResetReq = Inter_AccumInfoManualResetReq::DEFAULT;
    Inter_DriveInfoManualResetReq mInter_DriveInfoManualResetReq = Inter_DriveInfoManualResetReq::DEFAULT;
    Inter_DriveInfoManualResetReq mPrevInter_DriveInfoManualResetReq = Inter_DriveInfoManualResetReq::DEFAULT;
    Input_EvEnergyFlowStatus mInput_EvEnergyFlowStatus = Input_EvEnergyFlowStatus::NONE;
    Input_HeadUnitFuelEcoGraphResetStatus mInput_HeadUnitFuelEcoGraphResetStatus = Input_HeadUnitFuelEcoGraphResetStatus::DEFAULT;
    Output_TermAvgFuelConsSyncCounter mOutput_TermAvgFuelConsSyncCounter = Output_TermAvgFuelConsSyncCounter::TIME0;
    Input_EvChargeStatus mInput_EvChargeStatus = Input_EvChargeStatus::NONE;
    HUInt64 mInput_EvSocDisplayValue = 0;
    HBool mEvSocAfterRefuelResetStatus = false;
    Input_EvDTEAvnCalcStatus mInput_EvDTEAvnCalcStatus = Input_EvDTEAvnCalcStatus::OFF;
    Input_EvDTEDisplayOffReqStatus mInput_EvDTEDisplayOffReqStatus = Input_EvDTEDisplayOffReqStatus::DEFAULT;
    HUInt64 mInput_EvInstFuelEcoValuePerKWh = 0;
    HUInt64 mInput_EvInstFuelEcoValuePer100km = 0;
    HUInt64 mInput_EvInstFuelEcoValuePerMPG = 0;
    HUInt64 mInput_FcevInstFuelEcoValuePerKg = 0;
    Input_EvDteGuidePageInfoStatus mInput_EvDteGuidePageInfoStatus = Input_EvDteGuidePageInfoStatus::NONE;
    HUInt64 mEvDteLowBoundValue = 0;
    HUInt64 mEvDteUpperBoundValue = 0;
    HUInt64 mEvDteGuideDtePositionValue = 0;
    HUInt64 mEvDteGuideCoachPositionValue = 0;
    HUInt64 mEvDteGuideRealTimeValue = 0;

    static constexpr uint32_t kTimerStatusInterval500ms = 500;
    static constexpr uint32_t kTimerStatusInterval1s = 1000;
    static constexpr uint32_t kTimerTripInfoInterval10s = 10000;
    static constexpr uint32_t kTimerIgnitionOffInterval180s = 180000;

    static constexpr HUInt64 kTermAvgCountLimit = 150;
    static constexpr HDouble kOutputOdometerFactor = 10.0;
    static constexpr HDouble kOutputDrvInfoAvgFuelConsValueFactor = 10.0;
    static constexpr HDouble kOutputTermAvgFuelConsValueFactor = 10.0;
    static constexpr HDouble kOutputAvgFuelEconomyValueFactor = 10.0;

    // kFuelCalculationValue(0.000000128) == 0.128 * 0.000001
    static constexpr HDouble kFuelCalculationValue = 0.000000128;
    // kElecCalculationValue == 0.01 / 1000 / 3600
    static constexpr HDouble kElecCalculationValue{0.01 / 3600000.0 * 100.0 / 1000.0};
    // kOdometerCalculationValue == 3600 * 100
    static constexpr HUInt64 kOdometerCalculationValue = 360000;

    static constexpr HDouble kHydrogenCalValue = 0.01;
    static constexpr HDouble kHydrogenCalValueKg = 0.000001;
    static constexpr HUInt64 kEvDteGuidePostionMaxValue = 120;
    static constexpr HUInt64 kEvDteGuidePostionOffset = 10;

    static constexpr HUInt64 kOutputOdometerMaxValue = 16777214;

    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateTerm1sAvgStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerDrivingTimeTotal1sStatus;
    ssfs::SFCTimer<Trip_Info> mOneShotTimerIgnitionOff180sStatus;

    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAccumFuelEcoKmL10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAccumFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAccumHydrogenEco10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAccumVehicleSpeedKph10sStatus;

    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateDriveFuelEcoKmL10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateDriveFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateDriveHydrogenEco10sStatus;

    ssfs::SFCTimer<Trip_Info> mRepeatTimerInterDriveInfoVehicleSpeedKph10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAfterRefuelFuelEcoKmL10sStatus;

    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAfterRefuelFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAfterRefuelHydrogenEco10sStatus;
    ssfs::SFCTimer<Trip_Info> mRepeatTimerPrivateAfterRefuelVehicleSpeedKph10sStatus;

    ssfs::SFCTimer<Trip_Info> mRepeatTimer500msStatus;  // repeater
    ssfs::SFCTimer<Trip_Info> mRepeatTimer1sStatus;     // repeater

    HBool mBeforeIgnitionOff180sStatus = false;

    bool mIsValidPrevOdometerRefreshTime = false;
    std::chrono::time_point<std::chrono::steady_clock> mPrevOdometerRefreshTime;

    HDouble mFactor_ConvKmToMi = 0.6213592233009709;
    HDouble mPar_OdoCorrectionFactor = 0.0;
    HDouble mFactor_ConvLitterToUsGal = 0.26418;
    HDouble mFactor_ConvLitterToUkGal = 0.21997;
    HDouble mFactor_ConvKgToGal = 0.6375;

    HDouble mPrivate_OdometerTotal = 0.0;
    HDouble mPrevPrivate_OdometerTotal = 0.0;
    HDouble mPrivate_FuelConsumptionTotal = 0.0;
    HDouble mPrivate_HydrogenConsumptionTotal = 0.0;
    HDouble mPrivate_ElecFuelConsumptionTotal = 0.0;
    HUInt64 mPrivate_DrivingTimeTotal = 0;
    Private_ADMStatus mPrivate_ADMStatus = Private_ADMStatus::NONE;
    Private_ADMStatus mPrevPrivate_ADMStatus = Private_ADMStatus::OFF;

    HDouble mPrivate_AccumStartDistance = 0.0;
    HUInt64 mPrivate_AccumStartTime = 0;
    HDouble mPrivate_AccumStartFuelCons = 0.0;
    HDouble mPrivate_AccumStartHydrogenCons = 0.0;
    HDouble mPrivate_AccumStartElecFuelCons = 0.0;
    HDouble mPrivate_DriveStartDistance = 0.0;
    HUInt64 mPrivate_DriveStartTime = 0;
    HDouble mPrivate_DriveStartFuelCons = 0.0;
    HDouble mPrivate_DriveStartHydrogenCons = 0.0;
    HDouble mPrivate_DriveStartElecFuelCons = 0.0;
    HDouble mPrivate_AfterRefuelStartDistance = 0.0;
    HDouble mPrevPrivate_AfterRefuelStartDistance = 0.0;
    HUInt64 mPrivate_AfterRefuelStartTime = 0;
    HDouble mPrivate_AfterRefuelStartFuelCons = 0.0;
    HDouble mPrivate_AfterRefuelStartHydrogenCons = 0.0;
    HDouble mPrivate_AfterRefuelStartElecFuelCons = 0.0;

    HDouble mPrivate_AccumFuelEcoKmL = 0.0;
    HDouble mPrivate_AccumFuelEcoL100km = 0.0;
    HDouble mPrivate_AccumFuelEcoUsMPG = 0.0;
    HDouble mPrivate_AccumFuelEcoUkMPG = 0.0;

    HDouble mPrivate_AccumFuelEcoKmKwh = 0.0;
    HDouble mPrivate_AccumFuelEcoKwh100km = 0.0;
    HDouble mPrivate_AccumFuelEcoMiKwh = 0.0;

    HDouble mPrivate_AccumHydrogenEcoKmKg = 0.0;
    HDouble mPrivate_AccumHydrogenEcoKg100km = 0.0;
    HDouble mPrivate_AccumHydrogenEcoMPG = 0.0;

    HUInt64 mPrivate_AccumVehicleSpeedKph = 0;
    HUInt64 mPrivate_AccumVehicleSpeedMph = 0;

    HDouble mPrivate_DriveFuelEcoKmL = 0.0;
    HDouble mPrivate_DriveFuelEcoL100km = 0.0;
    HDouble mPrivate_DriveFuelEcoUsMPG = 0.0;
    HDouble mPrivate_DriveFuelEcoUkMPG = 0.0;

    HDouble mPrivate_DriveFuelEcoKmKwh = 0.0;
    HDouble mPrivate_DriveFuelEcoKwh100km = 0.0;
    HDouble mPrivate_DriveFuelEcoMiKwh = 0.0;

    HDouble mPrivate_DriveHydrogenEcoKmKg = 0.0;
    HDouble mPrivate_DriveHydrogenEcoKg100km = 0.0;
    HDouble mPrivate_DriveHydrogenEcoMPG = 0.0;

    HUInt64 mInter_DriveInfoVehicleSpeedKph = 0;
    HUInt64 mInter_DriveInfoVehicleSpeedMph = 0;

    HDouble mPrivate_AfterRefuelFuelEcoKmL = 0.0;
    HDouble mPrivate_AfterRefuelFuelEcoL100km = 0.0;
    HDouble mPrivate_AfterRefuelFuelEcoUsMPG = 0.0;
    HDouble mPrivate_AfterRefuelFuelEcoUkMPG = 0.0;

    HDouble mPrivate_AfterRefuelFuelEcoKmKwh = 0.0;
    HDouble mPrivate_AfterRefuelFuelEcoKwh100km = 0.0;
    HDouble mPrivate_AfterRefuelFuelEcoMiKwh = 0.0;

    HDouble mPrivate_AfterRefuelHydrogenEcoKmKg = 0.0;
    HDouble mPrivate_AfterRefuelHydrogenEcoKg100km = 0.0;
    HDouble mPrivate_AfterRefuelHydrogenEcoMPG = 0.0;

    HUInt64 mPrivate_AfterRefuelVehicleSpeedKph = 0;
    HUInt64 mPrivate_AfterRefuelVehicleSpeedMph = 0;

    HDouble mPrivate_AccumDistanceKm = 0.0;
    HDouble mPrivate_AccumDistanceMi = 0.0;
    HUInt64 mPrivate_AccumTime = 0;

    HDouble mInter_DriveInfoDistanceKm = 0.0;
    HDouble mInter_DriveInfoDistanceMi = 0.0;
    HUInt64 mInter_DriveInfoTime = 0;

    HDouble mPrivate_AfterRefuelDistanceKm = 0.0;
    HDouble mPrivate_AfterRefuelDistanceMi = 0.0;
    HUInt64 mPrivate_AfterRefuelTime = 0;

    Private_InstFuelEcoCalModeStatus mPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::OFF;
    Private_InstFuelEcoCalModeStatus mPrevPrivate_InstFuelEcoCalModeStatus = Private_InstFuelEcoCalModeStatus::OFF;
    HDouble mPrivate_InstStart1Distance = 0.0;
    HDouble mPrivate_InstStart1FuelCons = 0.0;
    HDouble mPrivate_InstStart1HydrogenCons = 0.0;
    HDouble mPrivate_InstStart2Distance = 0.0;
    HDouble mPrivate_InstStart2FuelCons = 0.0;
    HDouble mPrivate_InstStart2HydrogenCons = 0.0;
    HDouble mPrivate_InstDistHalfSum1 = 0.0;
    HDouble mPrivate_InstDistHalfSum2 = 0.0;
    HDouble mPrivate_InstFuelConsHalfSum1 = 0.0;
    HDouble mPrivate_InstHydrogenConsHalfSum1 = 0.0;
    HDouble mPrivate_InstFuelConsHalfSum2 = 0.0;
    HDouble mPrivate_InstHydrogenConsHalfSum2 = 0.0;

    HDouble mPrivate_InstFuelEcoKmL = 0.0;
    HDouble mPrivate_InstFuelEcoL100km = 0.0;
    HDouble mPrivate_InstFuelEcoUsMPG = 0.0;
    HDouble mPrivate_InstFuelEcoUkMPG = 0.0;

    HDouble mPrivate_InstFuelEcoInputKmL = 0.0;
    HDouble mPrivate_InstFuelEcoInputL100km = 0.0;
    HDouble mPrivate_InstFuelEcoInputUsMPG = 0.0;
    HDouble mPrivate_InstFuelEcoInputUkMPG = 0.0;
    HDouble mPrivate_InstFuelEcoInputKmKwh = 0.0;
    HDouble mPrivate_InstFuelEcoInputKwh100km = 0.0;
    HDouble mPrivate_InstFuelEcoInputMiKwh = 0.0;
    HDouble mPrivate_InstFuelEcoInputKmKg = 0.0;
    HDouble mPrivate_InstFuelEcoInputKg100Km = 0.0;
    HDouble mPrivate_InstFuelEcoInputMileKg = 0.0;

    HDouble mPrivate_AccumFuelEconomyValue = 0.0;
    HDouble mInter_DriveInfoFuelEcoValue = 0.0;
    HDouble mPrivate_AfterDriveFuelEcoValue = 0.0;
    HDouble mPrivate_AfterDriveDistanceKmValue = 0.0;
    HDouble mPrivate_AfterDriveDistanceMiValue = 0.0;
    Private_DriveInfoResetStatus mPrivate_DriveInfoResetStatus = Private_DriveInfoResetStatus::OFF;
    HDouble mPrivate_AfterRefuelFuelEconomyValue = 0.0;

    HUInt64 mPrivate_TermAvgTimer = 0;
    Private_TermAvgCalModeStatus mPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::OFF;
    Private_TermAvgCalModeStatus mPrevPrivate_TermAvgCalModeStatus = Private_TermAvgCalModeStatus::OFF;
    Private_TermAvgSignalTxModeStatus mPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::OFF;
    Private_TermAvgSignalTxModeStatus mPrevPrivate_TermAvgSignalTxModeStatus = Private_TermAvgSignalTxModeStatus::OFF;
    HDouble mPrivate_TermAvg1StartDistance = 0.0;
    HDouble mPrivate_TermAvg1StartFuelCons = 0.0;
    HDouble mPrivate_TermAvg2StartDistance = 0.0;
    HDouble mPrivate_TermAvg2StartFuelCons = 0.0;
    HDouble mPrivate_TermAvgFuelEcoKmL = 0.0;
    HDouble mPrivate_TermAvgFuelEcoL100km = 0.0;
    HDouble mPrivate_TermAvgFuelEcoUsMPG = 0.0;
    HDouble mPrivate_TermAvgFuelEcoUkMPG = 0.0;
    HDouble mPrivate_TermAvgFuelEcoKmKg = 0.0;
    HDouble mPrivate_TermAvgFuelEcoKg100Km = 0.0;
    HDouble mPrivate_TermAvgFuelEcoMpge = 0.0;
    HDouble mPrivate_TermAvgFuelEconomyValue = 0.0;
    HBool mInter_EngineOnTransStatus = false;
    HBool mInter_HeadUnitFuelEcoGraphResetStatus = false;

    AddressValueList mActuatorOutputAddrList;

    using MemoryAddressMap = std::map<MemoryAddrName, std::string>;
    MemoryAddressMap mMemoryAddressMap = {
        {MemoryAddrName::DRIVING_TIME_VALUE, "SFC.Extension.Memory.Inter_DrivingTimeValue"},
        {MemoryAddrName::FUEL_CONSUMPT_VALUE, "SFC.Extension.Memory.Inter_FuelConsumptValue"},
        {MemoryAddrName::ELEC_FUEL_CONSUMPT_VALUE, "SFC.Extension.Memory.Inter_ElecFuelConsumptValue"},
        {MemoryAddrName::HYDROGEN_FUEL_CONSUMPT_VALUE, "SFC.Extension.Memory.Inter_HydrogenFuelConsumption"},
        {MemoryAddrName::ODOMETER_VALUE, "SFC.Extension.Memory.Inter_OdometerValue"},
        {MemoryAddrName::ACCUM_START_DISTANCE_VALUE, "SFC.Extension.Memory.Inter_AccumStartDistanceValue"},
        {MemoryAddrName::ACCUM_START_ELEC_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_AccumStartElecFuelConsValue"},
        {MemoryAddrName::ACCUM_START_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_AccumStartFuelConsValue"},
        {MemoryAddrName::ACCUM_START_HYDROGEN_CONS_VALUE, "SFC.Extension.Memory.Inter_AccumStartHydrogenConsValue"},
        {MemoryAddrName::ACCUM_START_TIME_VALUE, "SFC.Extension.Memory.Inter_AccumStartTimeValue"},
        {MemoryAddrName::AFTER_REFUEL_START_DISTANCE_VALUE, "SFC.Extension.Memory.Inter_AfterRefuelStartDistanceValue"},
        {MemoryAddrName::AFTER_REFUEL_START_ELEC_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_AfterRefuelStartElecFuelConsValue"},
        {MemoryAddrName::AFTER_REFUEL_START_HYDROGEN_CONS_VALUE, "SFC.Extension.Memory.Inter_AfterRefuelStartHydrogenConsValue"},
        {MemoryAddrName::AFTER_REFUEL_START_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_AfterRefuelStartFuelConsValue"},
        {MemoryAddrName::AFTER_REFUEL_START_TIME_VALUE, "SFC.Extension.Memory.Inter_AfterRefuelStartTimeValue"},
        {MemoryAddrName::DRIVE_START_DISTANCE_VALUE, "SFC.Extension.Memory.Inter_DriveStartDistanceValue"},
        {MemoryAddrName::DRIVE_START_ELEC_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_DriveStartElecFuelConsValue"},
        {MemoryAddrName::DRIVE_START_HYDROGEN_CONS_VALUE, "SFC.Extension.Memory.Inter_DriveStartHydrogenConsValue"},
        {MemoryAddrName::DRIVE_START_FUEL_CONS_VALUE, "SFC.Extension.Memory.Inter_DriveStartFuelConsValue"},
        {MemoryAddrName::DRIVE_START_TIME_VALUE, "SFC.Extension.Memory.Inter_DriveStartTimeValue"}};
    bool mAEMInitCompleteFlag = false;

    HDouble mSavedMemory_OdometerTotal = 0.0;
    HDouble mSavedMemory_FuelConsumptionTotal = 0.0;
    HDouble mSavedMemory_ElecFuelConsumptionTotal = 0.0;
    HDouble mSavedMemory_HydrogenFuelConsumptionTotal = 0.0;
    HUInt64 mSavedMemory_DrivingTimeTotal = 0;
    HDouble mSavedMemory_AccumStartDistance = 0.0;
    HUInt64 mSavedMemory_AccumStartTime = 0;
    HDouble mSavedMemory_AccumStartFuelCons = 0.0;
    HDouble mSavedMemory_AccumStartElecFuelCons = 0.0;
    HDouble mSavedMemory_AccumStartHydrogenCons = 0.0;
    HDouble mSavedMemory_DriveStartDistance = 0.0;
    HUInt64 mSavedMemory_DriveStartTime = 0;
    HDouble mSavedMemory_DriveStartFuelCons = 0.0;
    HDouble mSavedMemory_DriveStartElecFuelCons = 0.0;
    HDouble mSavedMemory_DriveStartHydrogenCons = 0.0;
    HDouble mSavedMemory_AfterRefuelStartDistance = 0.0;
    HUInt64 mSavedMemory_AfterRefuelStartTime = 0;
    HDouble mSavedMemory_AfterRefuelStartFuelCons = 0.0;
    HDouble mSavedMemory_AfterRefuelStartElecFuelCons = 0.0;
    HDouble mSavedMemory_AfterRefuelStartHydrogenCons = 0.0;
    HBool mIgninitState = false;
    HBool mAccumDisplayStatus = false;
    HBool mDriveDisplayStatus = false;
    HBool mAfterRefuelDisplayStatus = false;

    ElecDTEStatus mElecDTEStatus = ElecDTEStatus::NONE;

    static constexpr HDouble kNsecPerHourDouble{1.0 / 3600000000000.0};
    static constexpr HDouble k10MsecPerHourDouble{1.0 / 360000.0};
    static constexpr HDouble kMinDistanceForUpdateFuelEconomyValue = 0.3;  // in [Km]
};

}  // namespace ccos

#endif  // SFSS_Trip_Info_H
