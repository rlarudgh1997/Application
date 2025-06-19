/**
 * @file Trip_Info_CV.hpp
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
#ifndef SFSS_Trip_Info_CV_H
#define SFSS_Trip_Info_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include <chrono>
#include <Vehicle.h>
#include "Trip_Info_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 8.0.2
// Spec Version : v0.34
// Reference : [CV380] Trip_Info_CV. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Trip_Info_CV : public Trip_Info_CVBase {
public:
    Trip_Info_CV() = default;
    ~Trip_Info_CV() override = default;
    Trip_Info_CV(const Trip_Info_CV& other) = delete;
    Trip_Info_CV(Trip_Info_CV&& other) noexcept = delete;
    Trip_Info_CV& operator=(const Trip_Info_CV& other) = delete;
    Trip_Info_CV& operator=(Trip_Info_CV&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3 이벤트 표시 동작 로직
        // 5.3.1 주행가능거리 표시
        setSFCTrip_InfoEventDistanceToEmptyDisplayID("E00801");
        setSFCTrip_InfoEventDistanceToEmptyDisplayStat(SFCTrip_InfoEventDistanceToEmptyDisplayStat::ON);
        if (mRepeatTimerDrivingTimeTotal1sStatus.create(
                kTimerInterval1s, this, &Trip_Info_CV::onRepeatTimerAccumulateOperationOfDrivingTime, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerDrivingTimeTotal1sStatus Create Failed";
        }
        if (mOneShotTimerIgnitionOff180sStatus.create(kTimerInterval180s, this,
                                                      &Trip_Info_CV::onOneShotTimerIgnitionOff180sTimeout, false) == false) {
            _DWarning() << __FUNCTION__ << " mOneShotTimerIgnitionOff180sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAccumFuelEcoKmL10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAccumFuelEcoKmL10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumFuelEcoKmL10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAccumFuelEcoKmKwh10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAccumHydrogenEco10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAccumHydrogenEco10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumHydrogenEco10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAccumVehicleSpeedKph10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationDriveFuelEcoKmL10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationDriveFuelEcoKmL10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveFuelEcoKmL10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationDriveFuelEcoKmKwh10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationDriveHydrogenEco10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationDriveHydrogenEco10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveHydrogenEco10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationDriveInfoVehicleSpeedKph10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAfterRefuelFuelEcoKmL10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAfterRefuelHydrogenEco10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus.create(
                kTimerInterval10s, this, &Trip_Info_CV::onRepeatTimerCalculationAfterRefuelVehicleSpeedKph10s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus Create Failed";
        }
        if (mRepeatTimerCalculationDistanceAndTimeDisplay1sStatus.create(
                kTimerInterval1s, this, &Trip_Info_CV::onRepeatTimerCalculationDistanceAndTimeDisplay1s, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDistanceAndTimeDisplay1sStatus Create Failed";
        }
        if (mRepeatTimerCalculationInstantaneousFuelEco500msStatus.create(
                kTimerInterval500ms, this, &Trip_Info_CV::onRepeatTimerCalculationInstantaneousFuelEco500ms, true) == false) {
            _DWarning() << __FUNCTION__ << " mRepeatTimerCalculationInstantaneousFuelEco500msStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAemInitComplete, args.mInter_AEMInitializeComplete) == true) {
            _DInfo() << __FUNCTION__ << " AEM Initialize Complete!!!";
            loadParameterAndMemory(args);
        } else {
            _DInfo() << __FUNCTION__ << " AEM is Not Initialize";
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == true) {
            _DInfo() << __FUNCTION__ << " IgnitionState : ON";
            mIgnitionState = IgnitionState::ON;
            mRepeatTimerCalculationDistanceAndTimeDisplay1sStatus.restart();
            updatePrivateDTGInputStatus();
            determineOperationOfDrivingTimeTimerStatus();
            determineAdmStatus();
            update10sTimerOnIgnOnOffAndAemInitStatus();
            onRepeatTimerCalculationDistanceAndTimeDisplay1s();
            updateInstFuelEcoCalModeStatus();
            updateAllOfFuelEconomyValue();
            updateOutputEvDteValue();
            updateOutputTotalDteValue();
            updateOutputTripUnitStatus();
            updateOutputAvgFuelEconomyUnitStatus();
            determineDriveInfoResetStatus();
            updateOutputDrvInfoAvgFuelCons();
            updateOdometerStat();
            updateElecDte();
            updateDteDashBlinkDisplayForEv();
            updateDteGuideFunctionStatus();
            updateDteGuideLowBoundValue();
            updateDteGuideUpperBoundValue();
            updateDteGuideMainDtePositionValue();
            updateDteGuideCoachPositionValue();
            mPrevIgnitionState = mIgnitionState;
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == true) {
            _DInfo() << __FUNCTION__ << " IgnitionState : OFF";
            mIgnitionState = IgnitionState::OFF;
            mRepeatTimerCalculationDistanceAndTimeDisplay1sStatus.stop();
            determineOperationOfDrivingTimeTimerStatus();
            determineAdmStatus();
            saveDataToMemory();
            update10sTimerOnIgnOnOffAndAemInitStatus();
            updateInstFuelEcoCalModeStatus();
            updateAllOfFuelEconomyValue();
            saveAfterDriveFuelEcoValue();
            determineDriveInfoResetStatus();
            updateInputDisplayTypeInstFuelEconomy();
            updateAccumulatedInfoDistance();
            updateAccumulatedInfoTime();
            updateAccumulatedInfoSpeed();
            updateAfterRefuelDistance();
            updateAfterRefuelTime();
            updateAfterRefuelSpeed();
            updateElecDte();
            mPrevIgnitionState = mIgnitionState;
        }
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        if (updateValueIfValid(mInter_ConfigVehicleType, args.mInter_ConfigVehicleType) == true) {
            _DInfo() << __FUNCTION__ << " Inter_ConfigVehicleType : " << static_cast<HUInt64>(mInter_ConfigVehicleType);
            saveDriveStartPointToMemory();
            saveAfterRefuelStartPointToMemory();
            updateInstFuelEcoCalModeStatus();
            updateOutputEvDteValue();
            updateOutputTotalDteValue();
            saveAfterDriveFuelEcoValue();
            determineDriveInfoResetStatus();
            updateOutputDrvInfoAvgFuelCons();
            updateInputDisplayTypeInstFuelEconomy();
            updateFuelEcoUnit();
            updateDteSymbol();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_ConfigVehicleType : 0 (Initialization value is ICV)";
        }
        if (updateValueIfValid(mInter_ConfigFuelTypeCV, args.mInter_ConfigFuelTypeCV) == true) {
            _DInfo() << __FUNCTION__ << " Inter_ConfigFuelTypeCV : " << static_cast<HUInt64>(mInter_ConfigFuelTypeCV);
            updateFuelEcoUnit();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_ConfigFuelTypeCV : 0 (Initialization value is DSL)";
        }
        if (updateValueIfValid(mInter_ConfigArea, args.mInter_ConfigArea)) {
            _DInfo() << __FUNCTION__ << " Inter_ConfigArea : " << static_cast<HUInt64>(mInter_ConfigArea);
            updatePrivateDTGInputStatus();
            updateOdometerStat();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_ConfigArea : 0 (Initialization value is DOM)";
        }
    }

    void onActuatorVehicleSpeedChanged(const ArgumentsActuatorVehicleSpeedChanged& args) {
        if (updateValueIfValid(mInter_ActualVehicleSpeed, args.mInter_ActualVehicleSpeed) == true) {
            if (almostEqual(mInter_ActualVehicleSpeed, 0.0) == false) {
                updatePrivateOdometerTotal();
            } else {
                mIsValidPrevOdometerRefreshTime = false;
            }
            updateInstFuelEcoCalModeStatus();
            mPrevInter_ActualVehicleSpeed = mInter_ActualVehicleSpeed;
        }
    }

    void onFuelConsumptionValueChanged(const ArgumentsFuelConsumptionValueChanged& args) {
        if (updateValueIfValid(mInput_FuelConsumptionValue, args.mInput_FuelConsumptionValue) == true) {
            if (mInput_FuelConsumptionValue > 0) {
                calculateFuelConsumptionTotal();
            }
        }
    }

    void onFuelConsumptionFcu1Changed(const ArgumentsFuelConsumptionFcu1Changed& args) {
        if (updateValueIfValid(mInput_FuelConsumption_FCU1, args.mInput_FuelConsumption_FCU1) == true) {
            if (mInput_FuelConsumption_FCU1 > 0) {
                calculateHydrogenConsumptionTotal(mInput_FuelConsumption_FCU1);
            }
        }
    }

    void onFuelConsumptionFcu2Changed(const ArgumentsFuelConsumptionFcu2Changed& args) {
        if (updateValueIfValid(mInput_FuelConsumption_FCU2, args.mInput_FuelConsumption_FCU2) == true) {
            if (mInput_FuelConsumption_FCU2 > 0) {
                calculateHydrogenConsumptionTotal(mInput_FuelConsumption_FCU2);
            }
        }
    }

    void onElectricityConsumptionChanged(const ArgumentsElectricityConsumptionChanged& args) {
        if (updateValueIfValid(mInput_EvBatteryCurrentValue, args.mInput_EvBatteryCurrentValue) == true &&
            updateValueIfValid(mInput_EvBatteryVoltageValue, args.mInput_EvBatteryVoltageValue) == true) {
            updateElecFuelConsumptionTotal();
        }
    }

    void onEvInstFuelEcoValuePerKwhChanged(const ArgumentsEvInstFuelEcoValuePerKwhChanged& args) {
        if (updateValueIfValid(mInput_EvInstFuelEcoValuePerKWh, args.mInput_EvInstFuelEcoValuePerKWh) == true) {
            calculateInputDisplayTypeInstFuelEconomyForEv();
        }
    }

    void onEvDteLowBoundValueChanged(const ArgumentsEvDteLowBoundValueChanged& args) {
        if (updateValueIfValid(mInput_EvDteLowBoundValue, args.mInput_EvDteLowBoundValue) == true) {
            updateDteGuideLowBoundValue();
        }
    }

    void onEvDteUpperBoundValueChanged(const ArgumentsEvDteUpperBoundValueChanged& args) {
        if (updateValueIfValid(mInput_EvDteUpperBoundValue, args.mInput_EvDteUpperBoundValue) == true) {
            updateDteGuideUpperBoundValue();
        }
    }

    void onEvDteGuideDtePositionValueChanged(const ArgumentsEvDteGuideDtePositionValueChanged& args) {
        if (updateValueIfValid(mInput_EvDteGuideDtePositionValue, args.mInput_EvDteGuideDtePositionValue) == true) {
            updateDteGuideMainDtePositionValue();
        }
    }

    void onEvDteGuideCoachPositionValueChanged(const ArgumentsEvDteGuideCoachPositionValueChanged& args) {
        if (updateValueIfValid(mInput_EvDteGuideCoachPositionValue, args.mInput_EvDteGuideCoachPositionValue) == true) {
            updateDteGuideCoachPositionValue();
        }
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        if (updateValueIfValid(mInter_TripDistanceUnit, args.mInter_TripDistanceUnit) == true) {
            _DInfo() << __FUNCTION__ << " Inter_TripDistanceUnit : " << static_cast<HUInt64>(mInter_TripDistanceUnit);
            calculateAccumDistanceKmAndMi();
            calculateDriveInfoDistanceKmAndMi();
            calculateAfterRefuelDistanceKmAndMi();
            updateAllOfFuelEconomyValue();
            updateOutputEvDteValue();
            updateOutputTotalDteValue();
            updateOutputTripUnitStatus();
            updateOutputAvgFuelEconomyUnitStatus();
            updateInputDisplayTypeInstFuelEconomy();
            updateAccumulatedInfoSpeed();
            updateDrivingInfoSpeed();
            updateAfterRefuelSpeed();
            updateOdometerValue();
            updateElecDte();
            updateTripDistanceUnit();
            updateFuelEcoUnit();
            updateDteGuideLowBoundValue();
            updateDteGuideUpperBoundValue();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_TripDistanceUnit : 1 (Initialization value is KM)";
        }
    }

    void onFuelEcoUnitKmSetStatusChanged(const ArgumentsFuelEcoUnitKmSetStatusChanged& args) {
        if (updateValueIfValid(mInter_FuelEcoUnitKmSetStatus, args.mInter_FuelEcoUnitKmSetStatus) == true) {
            _DInfo() << __FUNCTION__ << " Inter_FuelEcoUnitKmSetStatus : " << static_cast<HUInt64>(mInter_FuelEcoUnitKmSetStatus);
            updateAllOfFuelEconomyValue();
            updateOutputAvgFuelEconomyUnitStatus();
            updateInputDisplayTypeInstFuelEconomy();
            updateFuelEcoUnit();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_FuelEcoUnitKmSetStatus : 1 (Initialization value is KM_PER_LITTER)";
        }
    }

    void onFuelEcoUnitMiSetStatusChanged(const ArgumentsFuelEcoUnitMiSetStatusChanged& args) {
        if (updateValueIfValid(mInter_FuelEcoUnitMiSetStatus, args.mInter_FuelEcoUnitMiSetStatus) == true) {
            _DInfo() << __FUNCTION__ << " Inter_FuelEcoUnitMiSetStatus : " << static_cast<HUInt64>(mInter_FuelEcoUnitMiSetStatus);
            updateAllOfFuelEconomyValue();
            updateOutputAvgFuelEconomyUnitStatus();
            updateInputDisplayTypeInstFuelEconomy();
            updateFuelEcoUnit();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_FuelEcoUnitMiSetStatus : 1 (Initialization value is US_GALLON)";
        }
    }

    void onDisplayLanguageStatusChanged(const ArgumentsDisplayLanguageStatusChanged& args) {
        if (updateValueIfValid(mInter_DisplayLanguageStatus, args.mInter_DisplayLanguageStatus) == true) {
            _DInfo() << __FUNCTION__ << " Inter_DisplayLanguageStatus : " << static_cast<HUInt64>(mInter_DisplayLanguageStatus);
            updateTripInfoSubTitleType();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_DisplayLanguageStatus : 0 (Initialization value is NONE)";
        }
    }

    void onEngineOnStatusChanged(const ArgumentsEngineOnStatusChanged& args) {
        if (updateValueIfValid(mInter_EngineOnStatus, args.mInter_EngineOnStatus) == true) {
            _DInfo() << __FUNCTION__ << " Inter_EngineOnStatus : " << static_cast<HUInt64>(mInter_EngineOnStatus);
            determineOperationOfDrivingTimeTimerStatus();
            mPrevInter_EngineOnStatus = mInter_EngineOnStatus;
        } else {
            _DInfo() << __FUNCTION__ << " Inter_EngineOnStatus : 2 (Initialization value is OFF)";
        }
    }

    void onIsgActiveStatusChanged(const ArgumentsIsgActiveStatusChanged& args) {
        if (updateValueIfValid(mInter_ISGActiveStatus, args.mInter_ISGActiveStatus) == true) {
            _DInfo() << __FUNCTION__ << " Inter_ISGActiveStatus : " << static_cast<HUInt64>(mInter_ISGActiveStatus);
            determineOperationOfDrivingTimeTimerStatus();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_ISGActiveStatus : 2 (Initialization value is OFF)";
        }
    }

    void onFuelDetectRealtimeModeStatusChanged(const ArgumentsFuelDetectRealtimeModeStatusChanged& args) {
        if (updateValueIfValid(mInter_FuelDetectRealtimeModeStatus, args.mInter_FuelDetectRealtimeModeStatus) == true) {
            _DInfo() << __FUNCTION__
                     << " Inter_FuelDetectRealtimeModeStatus : " << static_cast<HUInt64>(mInter_FuelDetectRealtimeModeStatus);
            saveAfterRefuelStartPointToMemory();
        } else {
            _DInfo() << __FUNCTION__ << " Inter_FuelDetectRealtimeModeStatus : 1 (Initialization value is OFF)";
        }
    }

    void onImuDrvDoorOpenStatusChanged(const ArgumentsImuDrvDoorOpenStatusChanged& args) {
        if (updateValueIfValid(mInput_IMUDrvDoorOpenStatus, args.mInput_IMUDrvDoorOpenStatus) == true) {
            _DInfo() << __FUNCTION__ << " Input_IMUDrvDoorOpenStatus : " << static_cast<HUInt64>(mInput_IMUDrvDoorOpenStatus);
            determineAdmStatus();
        } else {
            _DInfo() << __FUNCTION__ << " Input_IMUDrvDoorOpenStatus : 0 (Initialization value is OFF)";
        }
    }

    void onImuFrDoorOpenStatusChanged(const ArgumentsImuFrDoorOpenStatusChanged& args) {
        if (updateValueIfValid(mInput_IMUFrDoorOpenStatus, args.mInput_IMUFrDoorOpenStatus) == true) {
            _DInfo() << __FUNCTION__ << " Input_IMUFrDoorOpenStatus : " << static_cast<HUInt64>(mInput_IMUFrDoorOpenStatus);
            determineAdmStatus();
        } else {
            _DInfo() << __FUNCTION__ << " Input_IMUFrDoorOpenStatus : 0 (Initialization value is OFF)";
        }
    }

    void onVehicleReadyStatusChanged(const ArgumentsVehicleReadyStatusChanged& args) {
        if (updateValueIfValid(mInput_VehicleReadyStatus, args.mInput_VehicleReadyStatus) == true) {
            _DInfo() << __FUNCTION__ << " Input_VehicleReadyStatus : " << static_cast<HUInt64>(mInput_VehicleReadyStatus);
        } else {
            _DInfo() << __FUNCTION__ << " Input_VehicleReadyStatus : 1 (Initialization value is READY)";
        }
    }

    void onAccumInfoManualResetReqChanged(const ArgumentsAccumInfoManualResetReqChanged& args) {
        if (updateValueIfValid(mInter_AccumInfoManualResetReq, args.mInter_AccumInfoManualResetReq) == true) {
            _DInfo() << __FUNCTION__
                     << " Inter_AccumInfoManualResetReq : " << static_cast<HUInt64>(mInter_AccumInfoManualResetReq);
            saveAccumStartPointToMemory();
            mIsAccumDisplay = false;
            mPrevInter_AccumInfoManualResetReq = mInter_AccumInfoManualResetReq;
        } else {
            _DInfo() << __FUNCTION__ << " Inter_AccumInfoManualResetReq : 1 (Initialization value is DEFAULT)";
        }
    }

    void onDriveInfoManualResetReqChanged(const ArgumentsDriveInfoManualResetReqChanged& args) {
        if (updateValueIfValid(mInter_DriveInfoManualResetReq, args.mInter_DriveInfoManualResetReq) == true) {
            _DInfo() << __FUNCTION__
                     << " Inter_DriveInfoManualResetReq : " << static_cast<HUInt64>(mInter_DriveInfoManualResetReq);
            saveDriveStartPointToMemory();
            mIsDriveDisplay = false;
            mPrevInter_DriveInfoManualResetReq = mInter_DriveInfoManualResetReq;
        } else {
            _DInfo() << __FUNCTION__ << " Inter_DriveInfoManualResetReq : 1 (Initialization value is DEFAULT)";
        }
    }

    void onAfterRefuelInfoManualResetReqChanged(const ArgumentsAfterRefuelInfoManualResetReqChanged& args) {
        if (updateValueIfValid(mInter_AfterRefuelInfoManualResetReq, args.mInter_AfterRefuelInfoManualResetReq) == true) {
            _DInfo() << __FUNCTION__
                     << " Inter_AfterRefuelInfoManualResetReq : " << static_cast<HUInt64>(mInter_AfterRefuelInfoManualResetReq);
            saveAfterRefuelStartPointToMemory();
            mIsAfterRefuelDisplay = false;
            mPrevInter_AfterRefuelInfoManualResetReq = mInter_AfterRefuelInfoManualResetReq;
        } else {
            _DInfo() << __FUNCTION__ << " Inter_AfterRefuelInfoManualResetReq : 1 (Initialization value is DEFAULT)";
        }
    }

    void onEvDteValuePerKmChanged(const ArgumentsEvDteValuePerKmChanged& args) {
        if (ISTIMEOUT(args.mInput_EvDTEValuePerKm) == true) {
            _DInfo() << __FUNCTION__ << " Input_EvDTEValuePerKm : MESSAGE_TIMEOUT";
            mInput_ElecDteStatus = ElecDteStatus::MESSAGE_TIMEOUT;
        } else {
            mInput_ElecDteStatus = ElecDteStatus::VALUE;
        }
        if (updateValueIfValid(mInput_EvDTEValuePerKm, args.mInput_EvDTEValuePerKm) == true) {
            _DInfo() << __FUNCTION__ << " Input_EvDTEValuePerKm : " << mInput_EvDTEValuePerKm;
            updateOutputEvDteValue();
            updateOutputTotalDteValue();
            updateElecDte();
            updateDteDashBlinkDisplayForEv();
        }
    }

    void onBmsChargeStatusAndEvSocDisplayValueChanged(const ArgumentsBmsChargeStatusAndEvSocDisplayValueChanged& args) {
        if (updateValueIfValid(mInput_BmsChgStatus, args.mInput_BmsChgStatus) == true &&
            updateValueIfValid(mInput_BmsDisplaySocValue, args.mInput_BmsDisplaySocValue) == true) {
            _DInfo() << __FUNCTION__ << " Input_BmsChgStatus : " << static_cast<HUInt64>(mInput_BmsChgStatus);
            _DInfo() << __FUNCTION__ << " Input_BmsDisplaySocValue : " << mInput_BmsDisplaySocValue;
            determineBmsDisplaySocValueIncrease10OnBmsChgStatus();
            mPrevInput_BmsChgStatus = mInput_BmsChgStatus;
        } else {
            _DInfo() << __FUNCTION__ << " Input_BmsChgStatus : 0 (Initialization value is OFF)";
            _DInfo() << __FUNCTION__ << " Input_BmsDisplaySocValue : 0 (Initialization value is 0)";
        }
    }

    void onEvDteAvnCalcStatusChanged(const ArgumentsEvDteAvnCalcStatusChanged& args) {
        if (updateValueIfValid(mInput_EvDteAvnCalcStatus, args.mInput_EvDTEAvnCalcStatus) == true) {
            _DInfo() << __FUNCTION__ << " Input_EvDTEAvnCalcStatus : " << static_cast<HUInt64>(mInput_EvDteAvnCalcStatus);
            updateDteAvnCalStat();
        } else {
            _DInfo() << __FUNCTION__ << " Input_EvDTEAvnCalcStatus : 0 (Initialization value is OFF)";
        }
    }

    void onEvDteDisplayOffReqStatusChanged(const ArgumentsEvDteDisplayOffReqStatusChanged& args) {
        if (updateValueIfValid(mInput_EvDteDisplayOffReqStatus, args.mInput_EvDTEDisplayOffReqStatus) == true) {
            _DInfo() << __FUNCTION__
                     << " Input_EvDTEDisplayOffReqStatus : " << static_cast<HUInt64>(mInput_EvDteDisplayOffReqStatus);
            updateOutputTotalDteValue();
            updateDteDashDisplayForEvStat();
        } else {
            _DInfo() << __FUNCTION__ << " Input_EvDTEDisplayOffReqStatus : 0 (Initialization value is DEFAULT)";
        }
    }

    void onEvDteGuidePageInfoStatusChanged(const ArgumentsEvDteGuidePageInfoStatusChanged& args) {
        if (updateValueIfValid(mInput_EvDteGuidePageInfoStatus, args.mInput_EvDteGuidePageInfoStatus) == true) {
            _DInfo() << __FUNCTION__
                     << " Input_EvDteGuidePageInfoStatus : " << static_cast<HUInt64>(mInput_EvDteGuidePageInfoStatus);
            updateDteGuideFunctionStatus();
        } else {
            _DInfo() << __FUNCTION__ << " Input_EvDteGuidePageInfoStatus : 0 (Initialization value is NONE)";
        }
    }

    void onDtgOdometerChanged(const ArgumentsDtgOdometerChanged& args) {
        if (updateValueIfValid(mInput_DTGOdometer, args.mInput_DTGOdometer) == true) {
            _DInfo() << __FUNCTION__ << " Input_DTGOdometer : " << mInput_DTGOdometer;
            updatePrivateDTGInputStatus();
            updateOdometerStat();
        } else {
            _DInfo() << __FUNCTION__ << " Input_DTGOdometer : 0xFFFFFFFF (Initialization value is 0xFFFFFFFF(TIMEOUT))";
        }
    }

    void onFcevSofDisplayValueChanged(const ArgumentsFcevSofDisplayValueChanged& args) {
        if (updateValueIfValid(mInput_FcevSofDisplayValue, args.mInput_FcevSofDisplayValue) == true) {
            _DInfo() << __FUNCTION__ << " Input_FcevSofDisplayValue : " << static_cast<HUInt64>(mInput_FcevSofDisplayValue);
            mDiffInput_FcevSofDisplayValue =
                static_cast<HInt64>(mInput_FcevSofDisplayValue) - static_cast<HInt64>(mPrevInput_FcevSofDisplayValue);
            saveAfterRefuelStartPointToMemory();
            mPrevInput_FcevSofDisplayValue = mInput_FcevSofDisplayValue;
        } else {
            _DInfo() << __FUNCTION__ << " Input_FcevSofDisplayValue : 0 (Initialization value is 0)";
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.2.1 입력 기반 주행거리 상태 신호 정의(클러스터 표시용)
    void updatePrivateDTGInputStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigArea != ConfigArea::EEC ||
                (mInter_ConfigArea == ConfigArea::EEC && mInput_DTGOdometer > klimitDTGOdometer)) {
                mPrivate_DTGInputStatus = Private_DTGInputStatus::INVALID;
            } else if (mInter_ConfigArea == ConfigArea::EEC && mInput_DTGOdometer <= klimitDTGOdometer) {
                mPrivate_DTGInputStatus = Private_DTGInputStatus::VALID;
            } else {
                // no operation
            }
            DDebug() << __FUNCTION__ << " Private_DTGInputStatus : " << static_cast<HUInt64>(mPrivate_DTGInputStatus);
            updatePrivateOdometerDisplayValue();
        }
    }

    // 4.2.4 차속 기반 주행거리 연산 신호 정의(클러스터 내부 연산용)
    void updatePrivateOdometerTotal() {
        if (mIgnitionState == IgnitionState::ON) {
            // NOTE : 10ms 마다 nano 단위의 오차를 측정하여 총 주행 거리 적산 연산에서 오차 보정
            std::chrono::time_point<std::chrono::steady_clock> currentOdometerRefreshTime = std::chrono::steady_clock::now();
            const std::chrono::duration<double, std::nano> durationNanosecs =
                currentOdometerRefreshTime - mPrevOdometerRefreshTime;
            mPrevOdometerRefreshTime = currentOdometerRefreshTime;
            if (mIsValidPrevOdometerRefreshTime == false) {
                mIsValidPrevOdometerRefreshTime = true;
                mPrivate_OdometerTotal += mInter_ActualVehicleSpeed * k10MsecPerHourDouble * mPar_OdoCorrectionFactor;
            } else {
                // kOdometerCalculationValue와 단위를 동일하게 맞추기 위해 시간의 역수(durationMillisecs)에 ms->h로의
                // 변환 계수 3600 (1 hour/1 sec) * 1000 (1 sec/1 msec)를 곱한 값을 사용
                if (durationNanosecs.count() > 0U) {
                    HDouble odometerRefreshCycle = static_cast<HDouble>(durationNanosecs.count()) * kNsecPerHourDouble;
                    mPrivate_OdometerTotal += mInter_ActualVehicleSpeed * odometerRefreshCycle * mPar_OdoCorrectionFactor;
                }
            }
            updatePrivateOdometerDisplayValue();
        }
    }

    // 4.2.5 Odometer 표시 신호 연산
    void updatePrivateOdometerDisplayValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigArea == ConfigArea::EEC) {
                if (mPrivate_DTGInputStatus == Private_DTGInputStatus::VALID) {
                    mPrivate_OdometerDisplayValue = static_cast<HDouble>(mInput_DTGOdometer) * 0.005;
                } else {
                    if (mMemory_OdometerDisplayValue > mPrivate_OdometerTotal) {
                        mPrivate_OdometerDisplayValue = mMemory_OdometerDisplayValue;
                    } else {
                        mPrivate_OdometerDisplayValue = mPrivate_OdometerTotal;
                    }
                }
            } else {
                mPrivate_OdometerDisplayValue = mPrivate_OdometerTotal;
            }
            DDebug() << __FUNCTION__ << " Private_OdometerDisplayValue : " << mPrivate_OdometerDisplayValue;
            updateOutputOdometer();
            updateOdometerValue();
            updateMemoryOdometerDisplayValue();
            mPrevPrivate_OdometerDisplayValue = mPrivate_OdometerDisplayValue;
        }
    }

    // 4.2.6 Odometer 표시 신호 EEPROM 저장
    void updateMemoryOdometerDisplayValue() {
        if ((mIgnitionState == IgnitionState::ON && mPrevPrivate_OdometerDisplayValue != mPrivate_OdometerDisplayValue) ||
            (mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF)) {
            mMemory_OdometerDisplayValue = static_cast<HDouble>(floor(mPrivate_OdometerDisplayValue));
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_OdometerDisplayValue], mMemory_OdometerDisplayValue);
            DDebug() << __FUNCTION__ << " Memory_OdometerDisplayValue : " << mMemory_OdometerDisplayValue;
        }
    }

    // 4.3 연료소모량 누적 연산 (연료)
    // NOTE : Input_FuelConsumptionValue값이 유효할 때 DB Cycle 마다 적산
    void calculateFuelConsumptionTotal() {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
            mPrivate_FuelConsumptionTotal += static_cast<HDouble>(mInput_FuelConsumptionValue) * kFuelCalculationFactor;
            DDebug() << __FUNCTION__ << " Private_FuelConsumptionTotal : " << mPrivate_FuelConsumptionTotal;
            updateCalculationAccumFuelEcoKmL10sTimer();
            updateCalculationDriveFuelEcoKmL10sTimer();
            updateCalculationAfterRefuelFuelEcoKmL10sTimer();
        }
    }

    // 4.3 연료소모량 누적 연산 (수소)
    // NOTE : Input_FuelConsumption_FCU1, Input_FuelConsumption_FCU2 각각의 값이 유효할 떄 DB Cycle마다 각각의 값에 Factor를
    // NOTE : 곱하여 적산
    void calculateHydrogenConsumptionTotal(const HUInt64& fuelConsumptionFcu) {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
            mPrivate_HydrogenConsumptionTotal += fuelConsumptionFcu * kHydrogenCalculationFactor;
            DDebug() << __FUNCTION__ << " Private_HydrogenConsumptionTotal : " << mPrivate_HydrogenConsumptionTotal;
            updateCalculationAccumHydrogenEco10sTimer();
            updateCalculationDriveHydrogenEco10sTimer();
            updateCalculationAfterRefuelHydrogenEco10sTimer();
        }
    }

    // 4.4 전기소비전력량 누적 연산
    void updateElecFuelConsumptionTotal() {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == ConfigVehicleType::EV &&
            mInput_VehicleReadyStatus == VehicleReadyStatus::READY) {
            // NOTE : kElecCalculationFactor == 0.01 / 1000 / 3600 * {100} / 1000
            // NOTE : Input_EvBatteryCurrentValue, Input_EvBatteryVoltageValue DB Cycle은 100ms
            mPrivate_ElecFuelConsumptionTotal += static_cast<HDouble>(mInput_EvBatteryCurrentValue) *
                                                 static_cast<HDouble>(mInput_EvBatteryVoltageValue) * kElecCalculationFactor;
            DDebug() << __FUNCTION__ << " Private_ElecFuelConsumptionTotal : " << mPrivate_ElecFuelConsumptionTotal;
            updateCalculationAccumFuelEcoKmKwh10sTimer();
            updateCalculationDriveFuelEcoKmKwh10sTimer();
            updateCalculationAfterRefuelFuelEcoKmKwh10sTimer();
        }
    }

    // 4.5 주행시간 누적 연산
    // NOTE : 1초 마다 1씩 증가
    void onRepeatTimerAccumulateOperationOfDrivingTime() {
        ++mPrivate_DrivingTimeTotal;
        _DInfo() << __FUNCTION__ << " Private_DrivingTimeTotal : " << mPrivate_DrivingTimeTotal;
        updateCalculationAccumVehicleSpeedKph10sTimer();
        updateCalculationDriveInfoVehicleSpeedKph10sTimer();
        updateCalculationAfterRefuelVehicleSpeedKph10sTimer();
        flush();
    }

    // 4.5 주행시간 누적 연산 (Timer 실행/중지 판단)
    void determineOperationOfDrivingTimeTimerStatus() {
        if (mIgnitionState == IgnitionState::ON &&
            (mInter_EngineOnStatus == EngineOnStatus::ON || mInter_ISGActiveStatus == IsgActiveStatus::ON)) {
            if (mRepeatTimerDrivingTimeTotal1sStatus.isRunning() == false &&
                mRepeatTimerDrivingTimeTotal1sStatus.start() == false) {
                _DWarning() << __FUNCTION__ << " mRepeatTimerDrivingTimeTotal1sStatus Start Failed";
            }
        } else {
            mRepeatTimerDrivingTimeTotal1sStatus.stop();
        }
    }

    // 4.6 ADM 상태 판단 (IGN On -> Off After 180s timer callback)
    void onOneShotTimerIgnitionOff180sTimeout() {
        _DInfo() << __FUNCTION__ << " ADM 180s Timer Logic On. Private_ADMStatus is OFF";
        mPrivate_ADMStatus = AdmStatus::OFF;
        saveDriveStartPointToMemory();
        determineDriveInfoResetStatus();
        flush();
        mPrevPrivate_ADMStatus = mPrivate_ADMStatus;
    }

    // 4.6 ADM 상태 판단
    void determineAdmStatus() {
        HBool isUpdate = false;
        static HBool isIgnOnToOff = false;
        if (mIgnitionState == IgnitionState::ON) {
            mOneShotTimerIgnitionOff180sStatus.stop();
            isUpdate = true;
            isIgnOnToOff = false;
            mPrivate_ADMStatus = AdmStatus::ON;
        } else {
            if (mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
                isIgnOnToOff = true;
                if (mOneShotTimerIgnitionOff180sStatus.start() == false) {
                    _DWarning() << __FUNCTION__ << " mOneShotTimerIgnitionOff180sStatus Start Failed";
                }
                // NOTE : Door Open 상태에서 IGN ON -> OFF시 해당 조건 수행
                if (mInput_IMUDrvDoorOpenStatus == ImuDrvDoorOpenStatus::OPEN ||
                    mInput_IMUFrDoorOpenStatus == ImuFrDoorOpenStatus::OPEN) {
                    isUpdate = true;
                    mPrivate_ADMStatus = AdmStatus::OFF;
                }
                // NOTE : IGN ON -> OFF 변화 후 180초 이전에 Door Open 상태가 되는 경우 해당 조건을 수행하게 됨
            } else if (isIgnOnToOff == true && (mInput_IMUDrvDoorOpenStatus == ImuDrvDoorOpenStatus::OPEN ||
                                                mInput_IMUFrDoorOpenStatus == ImuFrDoorOpenStatus::OPEN)) {
                isUpdate = true;
                mPrivate_ADMStatus = AdmStatus::OFF;
            } else {
                // no operation
            }
        }
        if (mIsAemInitComplete == true && isUpdate == true) {
            _DInfo() << __FUNCTION__ << " Private_ADMStatus Changed From : " << static_cast<HUInt64>(mPrevPrivate_ADMStatus)
                     << " -> " << static_cast<HUInt64>(mPrivate_ADMStatus);
            saveDriveStartPointToMemory();
            determineDriveInfoResetStatus();
            mPrevPrivate_ADMStatus = mPrivate_ADMStatus;
        }
    }

    // 4.7 계산값 저장 및 불러오기 (IGN1 OFF시 누적 데이터 EEPROM에 즉시 저장)
    void saveDataToMemory() {
        // NOTE : IGN ON/OFF시 계산 값 모두 저장될 수 있도록 구현
        if (mIsAemInitComplete == true && mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_OdometerValue], roundOff2DecimalValue(mPrivate_OdometerTotal));
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_FuelConsumptValue], mPrivate_FuelConsumptionTotal);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_ElecFuelConsumptValue], mPrivate_ElecFuelConsumptionTotal);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_HydrogenConsumptionValue], mPrivate_HydrogenConsumptionTotal);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DrivingTimeValue], mPrivate_DrivingTimeTotal);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartDistanceValue],
                        roundOff2DecimalValue(mPrivate_AccumStartDistance));
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartTimeValue], mPrivate_AccumStartTime);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartFuelConsValue], mPrivate_AccumStartFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartElecFuelConsValue], mPrivate_AccumStartElecFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartHydrogenValue], mPrivate_AccumStartHydrogenCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartDistanceValue],
                        roundOff2DecimalValue(mPrivate_DriveStartDistance));
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartTimeValue], mPrivate_DriveStartTime);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartFuelConsValue], mPrivate_DriveStartFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartElecFuelConsValue], mPrivate_DriveStartElecFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartHydrogenValue], mPrivate_DriveStartHydrogenCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartDistanceValue],
                        roundOff2DecimalValue(mPrivate_AfterRefuelStartDistance));
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartTimeValue], mPrivate_AfterRefuelStartTime);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartFuelConsValue],
                        mPrivate_AfterRefuelStartFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartElecFuelConsValue],
                        mPrivate_AfterRefuelStartElecFuelCons);
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartHydrogenValue],
                        mPrivate_AfterRefuelStartHydrogenCons);
            updateMemoryOdometerDisplayValue();
            _DInfo() << __FUNCTION__ << " Private_OdometerTotal : " << roundOff2DecimalValue(mPrivate_OdometerTotal);
            _DInfo() << __FUNCTION__ << " Private_FuelConsumptionTotal : " << mPrivate_FuelConsumptionTotal;
            _DInfo() << __FUNCTION__ << " Private_ElecFuelConsumptionTotal : " << mPrivate_ElecFuelConsumptionTotal;
            _DInfo() << __FUNCTION__ << " Private_HydrogenConsumptionTotal : " << mPrivate_HydrogenConsumptionTotal;
            _DInfo() << __FUNCTION__ << " Private_DrivingTimeTotal : " << mPrivate_DrivingTimeTotal;
            _DInfo() << __FUNCTION__ << " Private_AccumStartDistance : " << roundOff2DecimalValue(mPrivate_AccumStartDistance);
            _DInfo() << __FUNCTION__ << " Private_AccumStartTime : " << mPrivate_AccumStartTime;
            _DInfo() << __FUNCTION__ << " Private_AccumStartFuelCons : " << mPrivate_AccumStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AccumStartElecFuelCons : " << mPrivate_AccumStartElecFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AccumStartHydrogenCons : " << mPrivate_AccumStartHydrogenCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartDistance : " << roundOff2DecimalValue(mPrivate_DriveStartDistance);
            _DInfo() << __FUNCTION__ << " Private_DriveStartTime : " << mPrivate_DriveStartTime;
            _DInfo() << __FUNCTION__ << " Private_DriveStartFuelCons : " << mPrivate_DriveStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartElecFuelCons : " << mPrivate_DriveStartElecFuelCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartHydrogenCons : " << mPrivate_DriveStartHydrogenCons;
            _DInfo() << __FUNCTION__
                     << " Private_AfterRefuelStartDistance : " << roundOff2DecimalValue(mPrivate_AfterRefuelStartDistance);
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartTime : " << mPrivate_AfterRefuelStartTime;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartFuelCons : " << mPrivate_AfterRefuelStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartElecFuelCons : " << mPrivate_AfterRefuelStartElecFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartHydrogenCons : " << mPrivate_AfterRefuelStartHydrogenCons;
        }
    }

    // 4.7 계산값 저장 및 불러오기
    // NOTE : 총 주행 거리 1증가 시 마다 총 주행 거리, 총 연료 소모량, 총 전력 소모량, 주행 시간, 총 수소 소모량 저장
    void saveDataToMemoryOnOdometerIntergerChanged() {
        saveOdometerTotal();
        saveFuelConsumptionTotal();
        saveElecConsumptionTotal();
        saveDrivingTimeTotal();
        saveHydrogenConsumptionTotal();
    }

    // 4.7 계산값 저장 Private_OdometerTotal (총 주행 거리 저장)
    void saveOdometerTotal() {
        if (mIsAemInitComplete == true && almostEqual(mPrivate_OdometerTotal, 0.0) == false) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_OdometerValue], roundOff2DecimalValue(mPrivate_OdometerTotal));
            _DInfo() << __FUNCTION__ << " Private_OdometerTotal" << roundOff2DecimalValue(mPrivate_OdometerTotal);
        }
    }

    // 4.7 계산값 저장 Private_DrivingTimeTotal (주행 시간 저장)
    void saveDrivingTimeTotal() {
        if (mIsAemInitComplete == true && mPrivate_DrivingTimeTotal > 0) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DrivingTimeValue], mPrivate_DrivingTimeTotal);
            _DInfo() << __FUNCTION__ << " Private_DrivingTimeTotal" << mPrivate_DrivingTimeTotal;
        }
    }

    // 4.7 계산값 저장 Private_FuelConsumptionTotal (총 연료 소모량 저장)
    void saveFuelConsumptionTotal() {
        if (mIsAemInitComplete == true && almostEqual(mPrivate_FuelConsumptionTotal, 0.0) == false) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_FuelConsumptValue], mPrivate_FuelConsumptionTotal);
            _DInfo() << __FUNCTION__ << " Private_FuelConsumptionTotal" << mPrivate_FuelConsumptionTotal;
        }
    }

    // 4.7 계산값 저장 Private_ElecFuelConsumptionTotal (총 전력 소모량 저장)
    void saveElecConsumptionTotal() {
        if (mIsAemInitComplete == true && almostEqual(mPrivate_ElecFuelConsumptionTotal, 0.0) == false) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_ElecFuelConsumptValue], mPrivate_ElecFuelConsumptionTotal);
            _DInfo() << __FUNCTION__ << " Private_ElecFuelConsumptionTotal" << mPrivate_ElecFuelConsumptionTotal;
        }
    }

    // 4.7 계산값 저장 Private_HydrogenConsumptionTotal (총 수소 소모량 저장)
    void saveHydrogenConsumptionTotal() {
        if (mIsAemInitComplete == true && almostEqual(mPrivate_HydrogenConsumptionTotal, 0.0) == false) {
            updateValue(mMemoryAddressMap[MemoryAddrName::Memory_HydrogenConsumptionValue], mPrivate_HydrogenConsumptionTotal);
            _DInfo() << __FUNCTION__ << " Private_HydrogenConsumptionTotal" << mPrivate_HydrogenConsumptionTotal;
        }
    }

    // 4.8 표시값 계산 시작점 저장 (누적 정보)
    void saveAccumStartPointToMemory() {
        if (mPrevInter_AccumInfoManualResetReq == AccumInfoManualResetReq::DEFAULT &&
            mInter_AccumInfoManualResetReq == AccumInfoManualResetReq::RESET_REQ) {
            _DInfo() << __FUNCTION__ << " Accum start reset invoked.";
            mPrivate_AccumStartDistance = mPrivate_OdometerTotal;
            mPrivate_AccumStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_AccumStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_AccumStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_AccumStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            calculateAccumDistanceKmAndMi();
            calculateAccumTime();
            updateCalculationAccumFuelEcoKmL10sTimer();
            updateCalculationAccumFuelEcoKmKwh10sTimer();
            updateCalculationAccumHydrogenEco10sTimer();
            updateCalculationAccumVehicleSpeedKph10sTimer();

            // NOTE : 4.7 계산값 저장 및 불러오기
            if (almostEqual(mPrivate_AccumStartDistance, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartDistanceValue],
                            roundOff2DecimalValue(mPrivate_AccumStartDistance));
                _DInfo() << __FUNCTION__
                         << " Private_AccumStartDistance : " << roundOff2DecimalValue(mPrivate_AccumStartDistance);
                saveOdometerTotal();
            }
            if (mPrivate_AccumStartTime > 0) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartTimeValue], mPrivate_AccumStartTime);
                _DInfo() << __FUNCTION__ << " Private_AccumStartTime : " << mPrivate_AccumStartTime;
                saveDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_AccumStartFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartFuelConsValue], mPrivate_AccumStartFuelCons);
                _DInfo() << __FUNCTION__ << " Private_AccumStartFuelCons : " << mPrivate_AccumStartFuelCons;
                saveFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AccumStartElecFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartElecFuelConsValue],
                            mPrivate_AccumStartElecFuelCons);
                _DInfo() << __FUNCTION__ << " Private_AccumStartElecFuelCons : " << mPrivate_AccumStartElecFuelCons;
                saveElecConsumptionTotal();
            }
            if (almostEqual(mPrivate_AccumStartHydrogenCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartHydrogenValue], mPrivate_AccumStartHydrogenCons);
                _DInfo() << __FUNCTION__ << " Private_AccumStartHydrogenCons : " << mPrivate_AccumStartHydrogenCons;
                saveHydrogenConsumptionTotal();
            }
            // NOTE : EXNCP-32584
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::DEFAULT);
        } else {
            setSFCTrip_InfoInter_AccumInfoResetReq(SFCTrip_InfoInter_AccumInfoResetReq::DEFAULT);
            // NOTE : ~EXNCP-32584
        }
    }

    // 4.8 표시값 계산 시작점 저장 (주행 정보)
    void saveDriveStartPointToMemory() {
        if ((mInter_ConfigVehicleType == ConfigVehicleType::ICV &&
             mPrevInter_DriveInfoManualResetReq == DriveInfoManualResetReq::DEFAULT &&
             mInter_DriveInfoManualResetReq == DriveInfoManualResetReq::RESET_REQ) ||
            (mPrevPrivate_ADMStatus == AdmStatus::OFF && mPrivate_ADMStatus == AdmStatus::ON)) {
            _DInfo() << __FUNCTION__ << " Drive info start reset invoked.";
            mPrivate_DriveStartDistance = mPrivate_OdometerTotal;
            mPrivate_DriveStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_DriveStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_DriveStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_DriveStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            calculateDriveInfoDistanceKmAndMi();
            calculateDriveInfoTime();
            updateCalculationDriveFuelEcoKmL10sTimer();
            updateCalculationDriveFuelEcoKmKwh10sTimer();
            updateCalculationDriveHydrogenEco10sTimer();
            updateCalculationDriveInfoVehicleSpeedKph10sTimer();

            // NOTE : 4.7 계산값 저장 및 불러오기
            if (almostEqual(mPrivate_DriveStartDistance, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartDistanceValue],
                            roundOff2DecimalValue(mPrivate_DriveStartDistance));
                _DInfo() << __FUNCTION__
                         << " Private_DriveStartDistance : " << roundOff2DecimalValue(mPrivate_DriveStartDistance);
                saveOdometerTotal();
            }
            if (mPrivate_DriveStartTime > 0) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartTimeValue], mPrivate_DriveStartTime);
                _DInfo() << __FUNCTION__ << " Private_DriveStartTime : " << mPrivate_DriveStartTime;
                saveDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_DriveStartFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartFuelConsValue], mPrivate_DriveStartFuelCons);
                _DInfo() << __FUNCTION__ << " Private_DriveStartFuelCons : " << mPrivate_DriveStartFuelCons;
                saveFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_DriveStartElecFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartElecFuelConsValue],
                            mPrivate_DriveStartElecFuelCons);
                _DInfo() << __FUNCTION__ << " Private_DriveStartElecFuelCons : " << mPrivate_DriveStartElecFuelCons;
                saveElecConsumptionTotal();
            }
            if (almostEqual(mPrivate_DriveStartHydrogenCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartHydrogenValue], mPrivate_DriveStartHydrogenCons);
                _DInfo() << __FUNCTION__ << " Private_DriveStartHydrogenCons : " << mPrivate_DriveStartHydrogenCons;
                saveHydrogenConsumptionTotal();
            }
            // NOTE : EXNCP-32584
            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::DEFAULT);
        } else {
            setSFCTrip_InfoInter_DriveInfoResetReq(SFCTrip_InfoInter_DriveInfoResetReq::DEFAULT);
            // NOTE : ~EXNCP-32584
        }
    }

    // 4.8 표시값 계산 시작점 저장 (주유 후 정보)
    void saveAfterRefuelStartPointToMemory() {
        if ((mPrevInter_AfterRefuelInfoManualResetReq == AfterRefuelInfoManualResetReq::DEFAULT &&
             mInter_AfterRefuelInfoManualResetReq == AfterRefuelInfoManualResetReq::RESET_REQ) ||
            mInter_FuelDetectRealtimeModeStatus == FuelDetectRealtimeModeStatus::ON ||
            (mInter_ConfigVehicleType == ConfigVehicleType::EV && mIsDisplaySocValue10IncreasedStatus == true) ||
            (mInter_ConfigVehicleType == ConfigVehicleType::FCEV && (mDiffInput_FcevSofDisplayValue >= 100))) {
            _DInfo() << __FUNCTION__ << " After refuel info start reset invoked.";
            mPrivate_AfterRefuelStartDistance = mPrivate_OdometerTotal;
            mPrivate_AfterRefuelStartTime = mPrivate_DrivingTimeTotal;
            mPrivate_AfterRefuelStartFuelCons = mPrivate_FuelConsumptionTotal;
            mPrivate_AfterRefuelStartHydrogenCons = mPrivate_HydrogenConsumptionTotal;
            mPrivate_AfterRefuelStartElecFuelCons = mPrivate_ElecFuelConsumptionTotal;
            calculateAfterRefuelDistanceKmAndMi();
            calculateAfterRefuelTime();
            updateCalculationAfterRefuelFuelEcoKmL10sTimer();
            updateCalculationAfterRefuelFuelEcoKmKwh10sTimer();
            updateCalculationAfterRefuelHydrogenEco10sTimer();
            updateCalculationAfterRefuelVehicleSpeedKph10sTimer();

            // NOTE : 4.7 계산값 저장 및 불러오기
            if (almostEqual(mPrivate_AfterRefuelStartDistance, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartDistanceValue],
                            roundOff2DecimalValue(mPrivate_AfterRefuelStartDistance));
                _DInfo() << __FUNCTION__
                         << " Private_AfterRefuelStartDistance : " << roundOff2DecimalValue(mPrivate_AfterRefuelStartDistance);
                saveOdometerTotal();
            }
            if (mPrivate_AfterRefuelStartTime > 0) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartTimeValue], mPrivate_AfterRefuelStartTime);
                _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartTime : " << mPrivate_AfterRefuelStartTime;
                saveDrivingTimeTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartFuelConsValue],
                            mPrivate_AfterRefuelStartFuelCons);
                _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartFuelCons : " << mPrivate_AfterRefuelStartFuelCons;
                saveFuelConsumptionTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartElecFuelCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartElecFuelConsValue],
                            mPrivate_AfterRefuelStartElecFuelCons);
                _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartElecFuelCons : " << mPrivate_AfterRefuelStartElecFuelCons;
                saveElecConsumptionTotal();
            }
            if (almostEqual(mPrivate_AfterRefuelStartHydrogenCons, 0.0) == false) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartHydrogenValue],
                            mPrivate_AfterRefuelStartHydrogenCons);
                _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartHydrogenCons : " << mPrivate_AfterRefuelStartHydrogenCons;
                saveHydrogenConsumptionTotal();
            }
            // NOTE : EXNCP-32584
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::RESET_REQ);
            flush();
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::DEFAULT);
            mPrevPrivate_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
        } else {
            setSFCTrip_InfoInter_AfterRefuelInfoResetReq(SFCTrip_InfoInter_AfterRefuelInfoResetReq::DEFAULT);
            // NOTE : ~EXNCP-32584
        }
    }

    // 4.9 평균 연비, 속도값 계산 (IGN ON/OFF시 10s Timer 전체 호출)
    void update10sTimerOnIgnOnOffAndAemInitStatus() {
        updateCalculationAccumFuelEcoKmL10sTimer();
        updateCalculationAccumFuelEcoKmKwh10sTimer();
        updateCalculationAccumHydrogenEco10sTimer();
        updateCalculationAccumVehicleSpeedKph10sTimer();
        updateCalculationDriveFuelEcoKmL10sTimer();
        updateCalculationDriveFuelEcoKmKwh10sTimer();
        updateCalculationDriveHydrogenEco10sTimer();
        updateCalculationDriveInfoVehicleSpeedKph10sTimer();
        updateCalculationAfterRefuelFuelEcoKmL10sTimer();
        updateCalculationAfterRefuelFuelEcoKmKwh10sTimer();
        updateCalculationAfterRefuelHydrogenEco10sTimer();
        updateCalculationAfterRefuelVehicleSpeedKph10sTimer();
    }

    // 4.9.1 누적 연비 계산(연료) Timer Callback
    void onRepeatTimerCalculationAccumFuelEcoKmL10s() {
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AccumStartFuelCons) == true) {
            mPrivate_AccumFuelEcoKmL = 0.0;
        } else {
            mPrivate_AccumFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                                       (mPrivate_FuelConsumptionTotal - mPrivate_AccumStartFuelCons);
        }
        if (isValidPositive(mPrivate_AccumFuelEcoKmL) == true) {
            mPrivate_AccumFuelEcoL100km = 1.0 / mPrivate_AccumFuelEcoKmL * 100.0;
        } else {
            mPrivate_AccumFuelEcoL100km = 0.0;
        }
        mPrivate_AccumFuelEcoUsMPG = mPrivate_AccumFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUsGal;
        mPrivate_AccumFuelEcoUkMPG = mPrivate_AccumFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUkGal;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoKmL : " << mPrivate_AccumFuelEcoKmL;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoL100km : " << mPrivate_AccumFuelEcoL100km;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoUsMPG : " << mPrivate_AccumFuelEcoUsMPG;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoUkMPG : " << mPrivate_AccumFuelEcoUkMPG;
        determineAccumFuelEconomyValue();
        flush();
    }

    // 4.9.1 누적 연비 계산(연료)
    void updateCalculationAccumFuelEcoKmL10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AccumStartFuelCons) == false) {
                if (mRepeatTimerCalculationAccumFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAccumFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAccumFuelEcoKmL10sStatus.stop();
                mPrivate_AccumFuelEcoKmL = 0.0;
                mPrivate_AccumFuelEcoL100km = 0.0;
                mPrivate_AccumFuelEcoUsMPG = 0.0;
                mPrivate_AccumFuelEcoUkMPG = 0.0;
                determineAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAccumFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.2 누적 연비 계산(전기) Timer Callback
    void onRepeatTimerCalculationAccumFuelEcoKmKwh10s() {
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AccumStartElecFuelCons) == true) {
            mPrivate_AccumFuelEcoKmKwh = 0.0;
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
        mPrivate_AccumFuelEcoMiKwh = mPrivate_AccumFuelEcoKmKwh * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoKmKwh : " << mPrivate_AccumFuelEcoKmKwh;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoKwh100km : " << mPrivate_AccumFuelEcoKwh100km;
        _DInfo() << __FUNCTION__ << " Private_AccumFuelEcoMiKwh : " << mPrivate_AccumFuelEcoMiKwh;
        determineAccumFuelEconomyValue();
        flush();
    }

    // 4.9.2 누적 연비 계산(전기)
    void updateCalculationAccumFuelEcoKmKwh10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AccumStartElecFuelCons) == false) {
                if (mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus.stop();
                mPrivate_AccumFuelEcoKmKwh = 0.0;
                mPrivate_AccumFuelEcoKwh100km = 0.0;
                mPrivate_AccumFuelEcoMiKwh = 0.0;
                determineAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.3 누적 연비 계산(수소) Timer Callback
    void onRepeatTimerCalculationAccumHydrogenEco10s() {
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AccumStartHydrogenCons) == true) {
            mPrivate_AccumHydrogenEcoKmKg = 0.0;
        } else {
            mPrivate_AccumHydrogenEcoKmKg = (mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                                            (mPrivate_HydrogenConsumptionTotal - mPrivate_AccumStartHydrogenCons);
        }
        if (isValidPositive(mPrivate_AccumHydrogenEcoKmKg) == true) {
            mPrivate_AccumHydrogenEcoKg100km = 1.0 / mPrivate_AccumHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_AccumHydrogenEcoKg100km = 0.0;
        }
        mPrivate_AccumHydrogenEcoMPG = mPrivate_AccumHydrogenEcoKmKg * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_AccumHydrogenEcoKmKg : " << mPrivate_AccumHydrogenEcoKmKg;
        _DInfo() << __FUNCTION__ << " Private_AccumHydrogenEcoKg100km : " << mPrivate_AccumHydrogenEcoKg100km;
        _DInfo() << __FUNCTION__ << " Private_AccumHydrogenEcoMPG : " << mPrivate_AccumHydrogenEcoMPG;
        determineAccumFuelEconomyValue();
        flush();
    }

    // 4.9.3 누적 연비 계산(수소)
    void updateCalculationAccumHydrogenEco10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AccumStartHydrogenCons) == false) {
                if (mRepeatTimerCalculationAccumHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAccumHydrogenEco10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAccumHydrogenEco10sStatus.stop();
                mPrivate_AccumHydrogenEcoKmKg = 0.0;
                mPrivate_AccumHydrogenEcoKg100km = 0.0;
                mPrivate_AccumHydrogenEcoMPG = 0.0;
                determineAccumFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAccumHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.4 누적 평균 속도 계산 Timer Callback
    void onRepeatTimerCalculationAccumVehicleSpeedKph10s() {
        if (mPrivate_DrivingTimeTotal == mPrivate_AccumStartTime) {
            mPrivate_AccumVehicleSpeedKph = 0;
        } else {
            mPrivate_AccumVehicleSpeedKph = lround(
                (mPrivate_OdometerTotal - mPrivate_AccumStartDistance) /
                ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_AccumStartTime)) / 3600.0));
        }
        mPrivate_AccumVehicleSpeedMph =
            static_cast<HUInt64>(floor(static_cast<HDouble>(mPrivate_AccumVehicleSpeedKph) * kFactor_ConvKmToMi));
        _DInfo() << __FUNCTION__ << " Private_AccumVehicleSpeedKph : " << mPrivate_AccumVehicleSpeedKph;
        _DInfo() << __FUNCTION__ << " Private_AccumVehicleSpeedMph : " << mPrivate_AccumVehicleSpeedMph;
        updateAccumulatedInfoSpeed();
        flush();
    }

    // 4.9.4 누적 평균 속도 계산
    void updateCalculationAccumVehicleSpeedKph10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_AccumStartTime) {
                if (mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus.stop();
                mPrivate_AccumVehicleSpeedKph = 0;
                mPrivate_AccumVehicleSpeedMph = 0;
                updateAccumulatedInfoSpeed();
            }
        } else {
            mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.9.5 주행 연비 계산(연료) Timer Callback
    void onRepeatTimerCalculationDriveFuelEcoKmL10s() {
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_DriveStartFuelCons) == true) {
            mPrivate_DriveFuelEcoKmL = 0.0;
        } else {
            mPrivate_DriveFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                                       (mPrivate_FuelConsumptionTotal - mPrivate_DriveStartFuelCons);
        }
        if (isValidPositive(mPrivate_DriveFuelEcoKmL) == true) {
            mPrivate_DriveFuelEcoL100km = 1.0 / mPrivate_DriveFuelEcoKmL * 100.0;
        } else {
            mPrivate_DriveFuelEcoL100km = 0.0;
        }
        mPrivate_DriveFuelEcoUsMPG = mPrivate_DriveFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUsGal;
        mPrivate_DriveFuelEcoUkMPG = mPrivate_DriveFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUkGal;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoKmL : " << mPrivate_DriveFuelEcoKmL;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoL100km : " << mPrivate_DriveFuelEcoL100km;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoUsMPG : " << mPrivate_DriveFuelEcoUsMPG;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoUkMPG : " << mPrivate_DriveFuelEcoUkMPG;
        determineDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.5 주행 연비 계산(연료)
    void updateCalculationDriveFuelEcoKmL10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_DriveStartFuelCons) == false) {
                if (mRepeatTimerCalculationDriveFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationDriveFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationDriveFuelEcoKmL10sStatus.stop();
                mPrivate_DriveFuelEcoKmL = 0.0;
                mPrivate_DriveFuelEcoL100km = 0.0;
                mPrivate_DriveFuelEcoUsMPG = 0.0;
                mPrivate_DriveFuelEcoUkMPG = 0.0;
                determineDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerCalculationDriveFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.6 주행 연비 계산(전기) Timer Callback
    void onRepeatTimerCalculationDriveFuelEcoKmKwh10s() {
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
        mPrivate_DriveFuelEcoMiKwh = mPrivate_DriveFuelEcoKmKwh * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoKmKwh : " << mPrivate_DriveFuelEcoKmKwh;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoKwh100km : " << mPrivate_DriveFuelEcoKwh100km;
        _DInfo() << __FUNCTION__ << " Private_DriveFuelEcoMiKwh : " << mPrivate_DriveFuelEcoMiKwh;
        determineDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.6 주행 연비 계산(전기)
    void updateCalculationDriveFuelEcoKmKwh10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_DriveStartElecFuelCons) == false) {
                if (mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus.stop();
                mPrivate_DriveFuelEcoKmKwh = 0.0;
                mPrivate_DriveFuelEcoKwh100km = 0.0;
                mPrivate_DriveFuelEcoMiKwh = 0.0;
                determineDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.7 주행 연비 계산(수소) Timer Callback
    void onRepeatTimerCalculationDriveHydrogenEco10s() {
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_DriveStartHydrogenCons) == true) {
            mPrivate_DriveHydrogenEcoKmKg = 0.0;
        } else {
            mPrivate_DriveHydrogenEcoKmKg = (mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                                            (mPrivate_HydrogenConsumptionTotal - mPrivate_DriveStartHydrogenCons);
        }
        if (isValidPositive(mPrivate_DriveHydrogenEcoKmKg) == true) {
            mPrivate_DriveHydrogenEcoKg100km = 1.0 / mPrivate_DriveHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_DriveHydrogenEcoKg100km = 0.0;
        }
        mPrivate_DriveHydrogenEcoMPG = mPrivate_DriveHydrogenEcoKmKg * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_DriveHydrogenEcoKmKg : " << mPrivate_DriveHydrogenEcoKmKg;
        _DInfo() << __FUNCTION__ << " Private_DriveHydrogenEcoKg100km : " << mPrivate_DriveHydrogenEcoKg100km;
        _DInfo() << __FUNCTION__ << " Private_DriveHydrogenEcoMPG : " << mPrivate_DriveHydrogenEcoMPG;
        determineDriveInfoFuelEcoValue();
        flush();
    }

    // 4.9.7 주행 연비 계산(수소)
    void updateCalculationDriveHydrogenEco10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_DriveStartHydrogenCons) == false) {
                if (mRepeatTimerCalculationDriveHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationDriveHydrogenEco10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationDriveHydrogenEco10sStatus.stop();
                mPrivate_DriveHydrogenEcoKmKg = 0.0;
                mPrivate_DriveHydrogenEcoKg100km = 0.0;
                mPrivate_DriveHydrogenEcoMPG = 0.0;
                determineDriveInfoFuelEcoValue();
            }
        } else {
            mRepeatTimerCalculationDriveHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.8 주행 평균 속도 계산 Timer Callback
    void onRepeatTimerCalculationDriveInfoVehicleSpeedKph10s() {
        if (mPrivate_DrivingTimeTotal == mPrivate_DriveStartTime) {
            mPrivate_DriveInfoVehicleSpeedKph = 0;
        } else {
            mPrivate_DriveInfoVehicleSpeedKph = lround(
                (mPrivate_OdometerTotal - mPrivate_DriveStartDistance) /
                ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_DriveStartTime)) / 3600.0));
        }
        mPrivate_DriveInfoVehicleSpeedMph =
            static_cast<HUInt64>(floor(static_cast<HDouble>(mPrivate_DriveInfoVehicleSpeedKph) * kFactor_ConvKmToMi));
        _DInfo() << __FUNCTION__ << " Inter_DriveInfoVehicleSpeedKph : " << mPrivate_DriveInfoVehicleSpeedKph;
        _DInfo() << __FUNCTION__ << " Inter_DriveInfoVehicleSpeedMph : " << mPrivate_DriveInfoVehicleSpeedMph;
        updateDrivingInfoSpeed();
        flush();
    }

    // 4.9.8 주행 평균 속도 계산
    void updateCalculationDriveInfoVehicleSpeedKph10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_DriveStartTime) {
                if (mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus.stop();
                mPrivate_DriveInfoVehicleSpeedKph = 0;
                mPrivate_DriveInfoVehicleSpeedMph = 0;
                updateDrivingInfoSpeed();
            }
        } else {
            mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.9.9 주유 후 연비 계산(연료) Timer Callback
    void onRepeatTimerCalculationAfterRefuelFuelEcoKmL10s() {
        if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AfterRefuelStartFuelCons) == true) {
            mPrivate_AfterRefuelFuelEcoKmL = 0.0;
        } else {
            mPrivate_AfterRefuelFuelEcoKmL = (mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                                             (mPrivate_FuelConsumptionTotal - mPrivate_AfterRefuelStartFuelCons);
        }
        if (isValidPositive(mPrivate_AfterRefuelFuelEcoKmL) == true) {
            mPrivate_AfterRefuelFuelEcoL100km = 1.0 / mPrivate_AfterRefuelFuelEcoKmL * 100.0;
        } else {
            mPrivate_AfterRefuelFuelEcoL100km = 0.0;
        }
        mPrivate_AfterRefuelFuelEcoUsMPG = mPrivate_AfterRefuelFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUsGal;
        mPrivate_AfterRefuelFuelEcoUkMPG = mPrivate_AfterRefuelFuelEcoKmL * kFactor_ConvKmToMi / kFactor_ConvLitterToUkGal;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoKmL : " << mPrivate_AfterRefuelFuelEcoKmL;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoL100km : " << mPrivate_AfterRefuelFuelEcoL100km;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoUsMPG : " << mPrivate_AfterRefuelFuelEcoUsMPG;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoUkMPG : " << mPrivate_AfterRefuelFuelEcoUkMPG;
        determineAfterRefuelFuelEconomyValue();
        flush();
    }

    // 4.9.9 주유 후 연비 계산(연료)
    void updateCalculationAfterRefuelFuelEcoKmL10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_FuelConsumptionTotal, mPrivate_AfterRefuelStartFuelCons) == false) {
                if (mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus.stop();
                mPrivate_AfterRefuelFuelEcoKmL = 0.0;
                mPrivate_AfterRefuelFuelEcoL100km = 0.0;
                mPrivate_AfterRefuelFuelEcoUsMPG = 0.0;
                mPrivate_AfterRefuelFuelEcoUkMPG = 0.0;
                determineAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus.stop();
        }
    }

    // 4.9.10 주유 후 연비 계산(전기) Timer Callback
    void onRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10s() {
        if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AfterRefuelStartElecFuelCons) == true) {
            mPrivate_AfterRefuelFuelEcoKmKwh = 0.0;
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
        mPrivate_AfterRefuelFuelEcoMiKwh = mPrivate_AfterRefuelFuelEcoKmKwh * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoKmKwh : " << mPrivate_AfterRefuelFuelEcoKmKwh;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoKwh100km : " << mPrivate_AfterRefuelFuelEcoKwh100km;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelFuelEcoMiKwh : " << mPrivate_AfterRefuelFuelEcoMiKwh;
        determineAfterRefuelFuelEconomyValue();
        flush();
    }

    // 4.9.10 주유 후 연비 계산(전기)
    void updateCalculationAfterRefuelFuelEcoKmKwh10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_ElecFuelConsumptionTotal, mPrivate_AfterRefuelStartElecFuelCons) == false) {
                if (mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus.stop();
                mPrivate_AfterRefuelFuelEcoKmKwh = 0.0;
                mPrivate_AfterRefuelFuelEcoKwh100km = 0.0;
                mPrivate_AfterRefuelFuelEcoMiKwh = 0.0;
                determineAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus.stop();
        }
    }

    // 4.9.11 주유 후 연비 계산(수소) Timer Callback
    void onRepeatTimerCalculationAfterRefuelHydrogenEco10s() {
        if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AfterRefuelStartHydrogenCons) == true) {
            mPrivate_AfterRefuelHydrogenEcoKmKg = 0.0;
        } else {
            mPrivate_AfterRefuelHydrogenEcoKmKg = (mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                                                  (mPrivate_HydrogenConsumptionTotal - mPrivate_AfterRefuelStartHydrogenCons);
        }
        if (isValidPositive(mPrivate_AfterRefuelHydrogenEcoKmKg) == true) {
            mPrivate_AfterRefuelHydrogenEcoKg100km = 1.0 / mPrivate_AfterRefuelHydrogenEcoKmKg * 100.0;
        } else {
            mPrivate_AfterRefuelHydrogenEcoKg100km = 0.0;
        }
        mPrivate_AfterRefuelHydrogenEcoMPG = mPrivate_AfterRefuelHydrogenEcoKmKg * kFactor_ConvKmToMi;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelHydrogenEcoKmKg : " << mPrivate_AfterRefuelHydrogenEcoKmKg;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelHydrogenEcoKg100km : " << mPrivate_AfterRefuelHydrogenEcoKg100km;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelHydrogenEcoMPG : " << mPrivate_AfterRefuelHydrogenEcoMPG;
        determineAfterRefuelFuelEconomyValue();
        flush();
    }

    // 4.9.11 주유 후 연비 계산(수소)
    void updateCalculationAfterRefuelHydrogenEco10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (almostEqual(mPrivate_HydrogenConsumptionTotal, mPrivate_AfterRefuelStartHydrogenCons) == false) {
                if (mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus.stop();
                mPrivate_AfterRefuelHydrogenEcoKmKg = 0.0;
                mPrivate_AfterRefuelHydrogenEcoKg100km = 0.0;
                mPrivate_AfterRefuelHydrogenEcoMPG = 0.0;
                determineAfterRefuelFuelEconomyValue();
            }
        } else {
            mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus.stop();
        }
    }

    // 4.9.12 주유 후 평균 속도 계산 Timer Callback
    void onRepeatTimerCalculationAfterRefuelVehicleSpeedKph10s() {
        if (mPrivate_DrivingTimeTotal == mPrivate_AfterRefuelStartTime) {
            mPrivate_AfterRefuelVehicleSpeedKph = 0;
        } else {
            mPrivate_AfterRefuelVehicleSpeedKph =
                lround((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) /
                       ((static_cast<HDouble>(mPrivate_DrivingTimeTotal) - static_cast<HDouble>(mPrivate_AfterRefuelStartTime)) /
                        3600.0));
        }
        mPrivate_AfterRefuelVehicleSpeedMph =
            static_cast<HUInt64>(floor(static_cast<HDouble>(mPrivate_AfterRefuelVehicleSpeedKph) * kFactor_ConvKmToMi));
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelVehicleSpeedKph : " << mPrivate_AfterRefuelVehicleSpeedKph;
        _DInfo() << __FUNCTION__ << " Private_AfterRefuelVehicleSpeedMph : " << mPrivate_AfterRefuelVehicleSpeedMph;
        updateAfterRefuelSpeed();
        flush();
    }

    // 4.9.12 주유 후 평균 속도 계산
    void updateCalculationAfterRefuelVehicleSpeedKph10sTimer() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DrivingTimeTotal != mPrivate_AfterRefuelStartTime) {
                if (mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus.isRunning() == false &&
                    mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus.start() == false) {
                    DWarning() << __FUNCTION__ << " mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus Start Failed";
                }
            } else {
                mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus.stop();
                mPrivate_AfterRefuelVehicleSpeedKph = 0;
                mPrivate_AfterRefuelVehicleSpeedMph = 0;
                updateAfterRefuelSpeed();
            }
        } else {
            mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus.stop();
        }
    }

    // 4.10 거리, 시간 표시값 계산 (1s Timer Callback)
    void onRepeatTimerCalculationDistanceAndTimeDisplay1s() {
        calculateAccumDistanceKmAndMi();
        calculateAccumTime();
        calculateDriveInfoDistanceKmAndMi();
        calculateDriveInfoTime();
        calculateAfterRefuelDistanceKmAndMi();
        calculateAfterRefuelTime();

        // Each Display Status Flag Prevents Not To Flush Fuel Eco On 1S Timer
        // Each  Display Status Exists To Make flush On 300m (One Time Event)
        // Remark: FuelEco Required 10S Timer Flush Period (EXNCP-28342)
        // NOTE : 누적, 주행, 주유 후 10초 연비 업데이트 전에 주행 거리 300m이상 될 시 업데이트 하기 위함
        if (mIsAccumDisplay == false) {
            if (mPrivate_AccumDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mIsAccumDisplay = true;
                determineAccumFuelEconomyValue();
            }
        }
        if (mIsDriveDisplay == false) {
            if (mInter_DriveInfoDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mIsDriveDisplay = true;
                determineDriveInfoFuelEcoValue();
            }
        }
        if (mIsAfterRefuelDisplay == false) {
            if (mPrivate_AfterRefuelDistanceKm >= kMinDistanceForUpdateFuelEconomyValue) {
                mIsAfterRefuelDisplay = true;
                determineAfterRefuelFuelEconomyValue();
            }
        }
        flush();
    }

    // 4.10.1 누적 거리 계산(km), 4.10.2 누적 거리 계산(mi)
    void calculateAccumDistanceKmAndMi() {
        mPrivate_AccumDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_AccumStartDistance);
        mPrivate_AccumDistanceMi =
            roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AccumStartDistance) * kFactor_ConvKmToMi);
        updateAccumulatedInfoDistance();
        updateAccumulatedFuelEconomy();
    }

    // 4.10.3 누적 시간 계산
    void calculateAccumTime() {
        mPrivate_AccumTime = static_cast<HUInt64>((mPrivate_DrivingTimeTotal - mPrivate_AccumStartTime) / 60.0);
        updateAccumulatedInfoTime();
    }

    // 4.10.4 주행 거리 계산(km), 4.10.5 주행 거리 계산(mi)
    void calculateDriveInfoDistanceKmAndMi() {
        mInter_DriveInfoDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_DriveStartDistance);
        mInter_DriveInfoDistanceMi =
            roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_DriveStartDistance) * kFactor_ConvKmToMi);
        setSFCTrip_InfoInter_DriveInfoDistanceKm(mInter_DriveInfoDistanceKm);
        setSFCTrip_InfoInter_DriveInfoDistanceMi(mInter_DriveInfoDistanceMi);
        updateDrivingInfoDistance();
        updateDrivingInfoFuelEconomy();
    }

    // 4.10.6 주행 시간 계산
    void calculateDriveInfoTime() {
        mInter_DriveInfoTime = static_cast<HUInt64>((mPrivate_DrivingTimeTotal - mPrivate_DriveStartTime) / 60.0);
        setSFCTrip_InfoInter_DriveInfoTime(mInter_DriveInfoTime);
        updateDrivingInfoTime();
    }

    // 4.10.7 주유 후 거리 계산(km), 4.10.8 주유 후 거리 계산(mi)
    void calculateAfterRefuelDistanceKmAndMi() {
        mPrivate_AfterRefuelDistanceKm = roundOff2DecimalValue(mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance);
        mPrivate_AfterRefuelDistanceMi =
            roundOff2DecimalValue((mPrivate_OdometerTotal - mPrivate_AfterRefuelStartDistance) * kFactor_ConvKmToMi);
        updateAfterRefuelDistance();
        updateAfterRefuelFuelEconomy();
    }

    // 4.10.9 주유 후 시간 계산
    void calculateAfterRefuelTime() {
        mPrivate_AfterRefuelTime = static_cast<HUInt64>((mPrivate_DrivingTimeTotal - mPrivate_AfterRefuelStartTime) / 60.0);
        updateAfterRefuelTime();
    }

    // 4.11 순간 연비 표시값 계산
    // 4.11.1 이동 평균 계산 방식
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
    void onRepeatTimerCalculationInstantaneousFuelEco500ms() {
        if (mPrevPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::OFF &&
            mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::INIT1) {
            // OFF => INIT1 after 500ms
            // INIT1 => INIT2 before 500ms
            DDebug() << __FUNCTION__ << " OFF => INIT1 after 500ms / INIT1 => INIT2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::INIT2;
            updateInstFunc2();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::INIT1 &&
                   mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::INIT2) {
            // INIT1 => INIT2 after 500ms
            // INIT2 => ON1 before 500ms
            DDebug() << __FUNCTION__ << " INIT1 => INIT2 after 500ms / INIT2 => ON1 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::ON1;
            updateInstFunc1();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::INIT2 &&
                   mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
            // INIT2 => ON1 after 500ms
            // ON1 => ON2 before 500ms
            DDebug() << __FUNCTION__ << " INIT2 => ON1 after 500ms / ON1 => ON2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::ON2;
            updateInstFunc2();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1 &&
                   mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
            // ON1 => ON2 after 500ms
            // ON2 => ON1 before 500ms
            DDebug() << __FUNCTION__ << " ON1 => ON2 after 500ms / ON2 => ON1 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::ON1;
            updateInstFunc1();
        } else if (mPrevPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2 &&
                   mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
            // ON2 => ON1 after 500ms
            // ON1 => ON2 before 500ms
            DDebug() << __FUNCTION__ << " ON2 => ON1 after 500ms / ON1 => ON2 before 500ms.";
            mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
            mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::ON2;
            updateInstFunc2();
        } else {
            // no operation
        }
        updateInstFuelEcoKmL();
        updateInstHydrogenEcoKmKg();
        updateInstFuelEcoL100km();
        updateInstHydrogenEcoKg100km();
        updateInstFuelEcoUsMPG();
        updateInstHydrogenEcoMPG();
        updateInstFuelEcoUkMPG();
        flush();
    }

    // 4.11.1.1 계산 모드 판단, 4.11.1.2 구간 거리 및 연료 소모량 계산 (ICV, FCEV 이동 평균 계산 방식)
    void updateInstFuelEcoCalModeStatus() {
        if (mInter_ConfigVehicleType == ConfigVehicleType::ICV || mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mPrevIgnitionState == IgnitionState::ON && mIgnitionState == IgnitionState::OFF) {
                mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::OFF;
            }
            if (mIgnitionState == IgnitionState::ON) {
                if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::OFF &&
                    surelyGreaterThan(mInter_ActualVehicleSpeed, 3.0) == true) {
                    // OFF => INIT1 before 500ms
                    DDebug() << __FUNCTION__ << "OFF => INIT1 before 500ms.";
                    mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                    mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::INIT1;
                    mPrivate_InstStart1Distance = mPrivate_OdometerTotal;
                    mPrivate_InstStart1FuelCons = mPrivate_FuelConsumptionTotal;
                    mPrivate_InstStart1HydrogenCons = mPrivate_HydrogenConsumptionTotal;
                    if (mRepeatTimerCalculationInstantaneousFuelEco500msStatus.restart() == false) {
                        DWarning() << __FUNCTION__ << " mRepeatTimerCalculationInstantaneousFuelEco500msStatus Start Failed";
                    }
                } else if (mPrivate_InstFuelEcoCalModeStatus != InstFuelEcoCalModeStatus::OFF &&
                           (surelyGreaterThan(mPrevInter_ActualVehicleSpeed, 0.0) == true &&
                            almostEqual(mInter_ActualVehicleSpeed, 0.0) == true)) {
                    // Excluding OFF
                    DDebug() << __FUNCTION__ << "Excluding OFF.";
                    mPrevPrivate_InstFuelEcoCalModeStatus = mPrivate_InstFuelEcoCalModeStatus;
                    mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::OFF;
                    mRepeatTimerCalculationInstantaneousFuelEco500msStatus.stop();
                } else {
                    DDebug() << __FUNCTION__ << "not operating condition.";
                    return;
                }
            } else {
                mRepeatTimerCalculationInstantaneousFuelEco500msStatus.stop();
            }
            updateInstFuelEcoKmL();
            updateInstHydrogenEcoKmKg();
            updateInstFuelEcoL100km();
            updateInstHydrogenEcoKg100km();
            updateInstFuelEcoUsMPG();
            updateInstHydrogenEcoMPG();
            updateInstFuelEcoUkMPG();
        } else {
            mRepeatTimerCalculationInstantaneousFuelEco500msStatus.stop();
        }
    }

    // 4.11.1.3 연비 표시 목표값 계산 (ICV, FCEV 이동 평균 계산 방식에서만 연산)
    // 4.11.1.3.1 km/L
    void updateInstFuelEcoKmL() {
        mPrivate_InstFuelEcoKmL = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1),
                                0.0) == false) {
                    mPrivate_InstFuelEcoKmL =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1);
                } else {
                    mPrivate_InstFuelEcoKmL = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2),
                                0.0) == false) {
                    mPrivate_InstFuelEcoKmL =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2);
                } else {
                    mPrivate_InstFuelEcoKmL = 999.0;
                }
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.2 km/kg
    void updateInstHydrogenEcoKmKg() {
        mPrivate_InstHydrogenEcoKmKg = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual(
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons + mPrivate_InstHydrogenConsHalfSum1),
                        0.0) == false) {
                    mPrivate_InstHydrogenEcoKmKg =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons + mPrivate_InstHydrogenConsHalfSum1);
                } else {
                    mPrivate_InstHydrogenEcoKmKg = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual(
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons + mPrivate_InstHydrogenConsHalfSum2),
                        0.0) == false) {
                    mPrivate_InstHydrogenEcoKmKg =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons + mPrivate_InstHydrogenConsHalfSum2);
                } else {
                    mPrivate_InstHydrogenEcoKmKg = 999.0;
                }
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.3 L/100km
    void updateInstFuelEcoL100km() {
        mPrivate_InstFuelEcoL100km = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if ((mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) &&
                (almostEqual((mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1), 0.0) == false)) {
                mPrivate_InstFuelEcoL100km =
                    (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) /
                    (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) * 100.0;
            } else if ((mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) &&
                       (almostEqual((mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2), 0.0) ==
                        false)) {
                mPrivate_InstFuelEcoL100km =
                    (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) /
                    (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) * 100.0;
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.4 kg/100km
    void updateInstHydrogenEcoKg100km() {
        mPrivate_InstHydrogenEcoKg100km = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if ((mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) &&
                (almostEqual(mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1, 0.0) == false)) {
                mPrivate_InstHydrogenEcoKg100km =
                    (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons + mPrivate_InstHydrogenConsHalfSum1) /
                    (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) * 100.0;
            } else if ((mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) &&
                       (almostEqual(mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2, 0.0) ==
                        false)) {
                mPrivate_InstHydrogenEcoKg100km =
                    (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons + mPrivate_InstHydrogenConsHalfSum2) /
                    (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) * 100.0;
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.5 US MPG
    void updateInstFuelEcoUsMPG() {
        mPrivate_InstFuelEcoUsMPG = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUsMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) *
                        kFactor_ConvKmToMi / kFactor_ConvLitterToUsGal;
                } else {
                    mPrivate_InstFuelEcoUsMPG = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual(mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2,
                                0.0) == false) {
                    mPrivate_InstFuelEcoUsMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) *
                        kFactor_ConvKmToMi / kFactor_ConvLitterToUsGal;
                } else {
                    mPrivate_InstFuelEcoUsMPG = 999.0;
                }
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.6 MPGe
    void updateInstHydrogenEcoMPG() {
        mPrivate_InstHydrogenEcoMPG = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual(
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons + mPrivate_InstHydrogenConsHalfSum1),
                        0.0) == false) {
                    mPrivate_InstHydrogenEcoMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart2HydrogenCons +
                         mPrivate_InstHydrogenConsHalfSum1) *
                        kFactor_ConvKmToMi;
                } else {
                    mPrivate_InstHydrogenEcoMPG = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual(
                        mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons + mPrivate_InstHydrogenConsHalfSum2,
                        0.0) == false) {
                    mPrivate_InstHydrogenEcoMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_HydrogenConsumptionTotal - mPrivate_InstStart1HydrogenCons +
                         mPrivate_InstHydrogenConsHalfSum2) *
                        kFactor_ConvKmToMi;
                } else {
                    mPrivate_InstHydrogenEcoMPG = 999.0;
                }
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.1.3.7 UK MPG
    void updateInstFuelEcoUkMPG() {
        mPrivate_InstFuelEcoUkMPG = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON1) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUkMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart2Distance + mPrivate_InstDistHalfSum1) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart2FuelCons + mPrivate_InstFuelConsHalfSum1) *
                        kFactor_ConvKmToMi / kFactor_ConvLitterToUkGal;
                } else {
                    mPrivate_InstFuelEcoUkMPG = 999.0;
                }
            } else if (mPrivate_InstFuelEcoCalModeStatus == InstFuelEcoCalModeStatus::ON2) {
                if (almostEqual((mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2),
                                0.0) == false) {
                    mPrivate_InstFuelEcoUkMPG =
                        (mPrivate_OdometerTotal - mPrivate_InstStart1Distance + mPrivate_InstDistHalfSum2) /
                        (mPrivate_FuelConsumptionTotal - mPrivate_InstStart1FuelCons + mPrivate_InstFuelConsHalfSum2) *
                        kFactor_ConvKmToMi / kFactor_ConvLitterToUkGal;
                } else {
                    mPrivate_InstFuelEcoUkMPG = 999.0;
                }
            } else {
                // no operation
            }
        }
        updateMoveAvgTypeInstFuelEconomy();
    }

    // 4.11.2 입력 기반 표시 방식 (EV)
    // NOTE : Input_EvInstFuelEcoValuePerKWh 값이 유효할 때 DB Cycle 마다 호출
    void calculateInputDisplayTypeInstFuelEconomyForEv() {
        if (mInter_ConfigVehicleType == ConfigVehicleType::EV && mIgnitionState == IgnitionState::ON) {
            mPrivate_InstFuelEcoInputKmKwh = roundOff1DecimalValue(static_cast<HDouble>(mInput_EvInstFuelEcoValuePerKWh) * 0.01);
            mPrivate_InstFuelEcoInputMiKwh = mPrivate_InstFuelEcoInputKmKwh * kFactor_ConvKmToMi;
            mPrivate_InstFuelEcoInputKwh100km =
                roundOff1DecimalValue(100.0 / (static_cast<HDouble>(mInput_EvInstFuelEcoValuePerKWh) * 0.01));
        } else {
            mPrivate_InstFuelEcoInputKmKwh = 0.0;
            mPrivate_InstFuelEcoInputMiKwh = 0.0;
            mPrivate_InstFuelEcoInputKwh100km = 0.0;
        }
        updateInputDisplayTypeInstFuelEconomy();
    }

    // 4.12 평균 연비 표시 종류 판단 (누적, 주행, 주유 후 연비값 Trip 또는 연비 unit 단위 변경 시 전체 업데이트)
    // NOTE : 클러스터의 메인 컨텐츠의 누적, 주행, 주유 후 연비 값은 Trip 단위 변경 시 동시에 변환 돼야 함
    void updateAllOfFuelEconomyValue() {
        determineAccumFuelEconomyValue();
        determineDriveInfoFuelEcoValue();
        determineAfterRefuelFuelEconomyValue();
    }

    // 4.12.1 누적 연비
    void determineAccumFuelEconomyValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoL100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoUkMPG;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoKwh100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mPrivate_AccumFuelEconomyValue = mPrivate_AccumFuelEcoMiKwh;
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoKg100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mPrivate_AccumFuelEconomyValue = mPrivate_AccumHydrogenEcoMPG;
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        updateAccumulatedFuelEconomy();
    }

    // 4.12.2 주행 연비
    void determineDriveInfoFuelEcoValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoL100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoUkMPG;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoKwh100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mInter_DriveInfoFuelEcoValue = mPrivate_DriveFuelEcoMiKwh;
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoKg100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mInter_DriveInfoFuelEcoValue = mPrivate_DriveHydrogenEcoMPG;
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        updateDrivingInfoFuelEconomy();
    }

    // 4.12.3 주유 후 연비
    void determineAfterRefuelFuelEconomyValue() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoL100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoUkMPG;
                    } else {
                        return;
                    }
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoKwh100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelFuelEcoMiKwh;
                } else {
                    return;
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoKg100km;
                    } else {
                        return;
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    mPrivate_AfterRefuelFuelEconomyValue = mPrivate_AfterRefuelHydrogenEcoMPG;
                } else {
                    return;
                }
            } else {
                return;
            }
        }
        updateAfterRefuelFuelEconomy();
        updateOutputAvgFuelEconomy();
        updateOutputAvgFuelEconomyDigitStatus();
    }

    // 4.13 현재값 외부 출력 (Output_EvDTEValue)
    void updateOutputEvDteValue() {
        if (mIgnitionState == IgnitionState::ON && mInter_ConfigVehicleType == ConfigVehicleType::EV) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_EvDTEValue.getAddress(), mInput_EvDTEValuePerKm);
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_EvDTEValue.getAddress(),
                                    static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * kFactor_ConvKmToMi)));
            } else {
                return;
            }
            flushActuatorOutput();
        }
    }

    // 4.13 현재값 외부 출력 (Output_TotalDTEValue)
    void updateOutputTotalDteValue() {
        if (mIgnitionState == IgnitionState::ON &&
            (mInter_ConfigVehicleType == ConfigVehicleType::EV || mInter_ConfigVehicleType == ConfigVehicleType::FCEV)) {
            if (mInput_EvDteDisplayOffReqStatus == EvDteDisplayOffReqStatus::OFF_REQUEST) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TotalDTEValue.getAddress(), 0U);
            } else if (mInput_EvDteDisplayOffReqStatus != EvDteDisplayOffReqStatus::OFF_REQUEST &&
                       mInter_TripDistanceUnit == TripDistanceUnit::KM &&
                       mInput_ElecDteStatus != ElecDteStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TotalDTEValue.getAddress(), mInput_EvDTEValuePerKm);
            } else if (mInput_EvDteDisplayOffReqStatus != EvDteDisplayOffReqStatus::OFF_REQUEST &&
                       mInter_TripDistanceUnit == TripDistanceUnit::MILE &&
                       mInput_ElecDteStatus != ElecDteStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TotalDTEValue.getAddress(),
                                    static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * kFactor_ConvKmToMi)));
            } else if (mInput_EvDteDisplayOffReqStatus != EvDteDisplayOffReqStatus::OFF_REQUEST &&
                       mInput_ElecDteStatus == ElecDteStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TotalDTEValue.getAddress(), 4095U);
            } else {
                return;
            }
            flushActuatorOutput();
        }
    }

    // 4.13 현재값 외부 출력 (Output_OdometerValue)
    // NOTE : Output_OdometerValue는 Private_OdometerTotal값을 소숫점 1자리 round off한 후 정수형으로 변환 후 송출
    // NOTE : CAN Output은 정수형으로만 전달 해야 함
    void updateOutputOdometer() {
        updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_OdometerValue.getAddress(),
                            static_cast<HUInt64>(roundOff1DecimalValue(mPrivate_OdometerDisplayValue) * kOutputOdometerFactor));
        flushActuatorOutput();
    }

    // 4.13 현재값 외부 출력 (Output_TripUnitStatus)
    void updateOutputTripUnitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TripUnitStatus.getAddress(),
                                    static_cast<HUInt64>(OutputTripUnitStatus::KM));
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_TripUnitStatus.getAddress(),
                                    static_cast<HUInt64>(OutputTripUnitStatus::MILE));
            } else {
                return;
            }
            flushActuatorOutput();
        }
    }

    // 4.13 현재 값 외부 출력 (Output_AvgFuelEconomyUnitStatus)
    void updateOutputAvgFuelEconomyUnitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            OutputAvgFuelEconomyUnitStatus outputAvgFuelEconomyUnitStatus;
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    outputAvgFuelEconomyUnitStatus = OutputAvgFuelEconomyUnitStatus::KM_PER_LITTER;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    outputAvgFuelEconomyUnitStatus = OutputAvgFuelEconomyUnitStatus::LITTER_PER_KM;
                } else {
                    return;
                }
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                    outputAvgFuelEconomyUnitStatus = OutputAvgFuelEconomyUnitStatus::MPG_US;
                } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                    outputAvgFuelEconomyUnitStatus = OutputAvgFuelEconomyUnitStatus::MPG_UK;
                } else {
                    return;
                }
            } else {
                return;
            }
            updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_AvgFuelEconomyUnitStatus.getAddress(),
                                static_cast<HUInt64>(outputAvgFuelEconomyUnitStatus));
            flushActuatorOutput();
        }
    }

    // 4.13 현재 값 외부 출력 (Output_AvgFuelEconomyValue)
    void updateOutputAvgFuelEconomy() {
        if (mIgnitionState == IgnitionState::ON) {
            HDouble outputAvgFuelEconomyValue = 0.0;
            if (surelyLessThan(mPrivate_AfterRefuelDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == false) {
                if (surelyGreaterThan(mPrivate_AfterRefuelFuelEconomyValue, 999.0) == true ||
                    almostEqual(mPrivate_AfterRefuelFuelEconomyValue, 999.0) == true) {
                    outputAvgFuelEconomyValue = 999.0;
                } else if (surelyLessThan(mPrivate_AfterRefuelFuelEconomyValue, 100.0) == true) {
                    outputAvgFuelEconomyValue =
                        roundOff1DecimalValue(mPrivate_AfterRefuelFuelEconomyValue) * kOutputAvgFuelEconomyValueFactor;
                } else {
                    outputAvgFuelEconomyValue = round(mPrivate_AfterRefuelFuelEconomyValue);
                }
            }
            updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_AvgFuelEconomyValue.getAddress(),
                                static_cast<HUInt64>(outputAvgFuelEconomyValue));
            flushActuatorOutput();
        }
    }

    // 4.13 현재 값 외부 출력 (Output_AvgFuelEconomyDigitStatus)
    void updateOutputAvgFuelEconomyDigitStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            OutputAvgFuelEconomyDigitStatus digitStat = OutputAvgFuelEconomyDigitStatus::DIGIT3;
            if (surelyLessThan(mPrivate_AfterRefuelFuelEconomyValue, 100.0) == true) {
                digitStat = OutputAvgFuelEconomyDigitStatus::DIGIT2;
            }
            updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_AvgFuelEconomyDigitStatus.getAddress(),
                                static_cast<HUInt64>(digitStat));
            flushActuatorOutput();
        }
    }

    // 4.15 주행 후 연비 측정 및 신호 송출
    // 4.15.1 주행 후 연비 측정
    void saveAfterDriveFuelEcoValue() {
        if (mInter_ConfigVehicleType == ConfigVehicleType::EV && mPrevIgnitionState == IgnitionState::ON &&
            mIgnitionState == IgnitionState::OFF) {
            mPrivate_AfterDriveFuelEcoValue = mInter_DriveInfoFuelEcoValue;
            if (mIsAemInitComplete == true) {
                updateValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterDriveFuelEcoValue], mPrivate_AfterDriveFuelEcoValue);
            }
        }
    }

    // 4.15.2 RESET 여부 판단
    void determineDriveInfoResetStatus() {
        if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
            if (mIgnitionState == IgnitionState::ON && mPrevPrivate_ADMStatus == AdmStatus::OFF &&
                mPrivate_ADMStatus == AdmStatus::ON) {
                mPrivate_DriveInfoResetStatus = DriveInfoResetStatus::RESET;
            } else if (mIgnitionState == IgnitionState::OFF) {
                mPrivate_DriveInfoResetStatus = DriveInfoResetStatus::OFF;
            } else {
                // no operation
            }
            updateOutputDrvInfoAvgFuelConsResetStatus();
        }
    }

    // 4.15.3 신호 송출 (Output_DrvInfoAvgFuelConsValue, Output_DrvInfoAvgFuelConsDigit)
    void updateOutputDrvInfoAvgFuelCons() {
        if (mInter_ConfigVehicleType == ConfigVehicleType::EV && mIgnitionState == IgnitionState::ON) {
            HDouble outputDrvInfoAvgFuelConsValue = 0.0;
            OutputDrvInfoAvgFuelConsDigit digitStat = OutputDrvInfoAvgFuelConsDigit::DIGIT3;
            if (surelyGreaterThan(mPrivate_AfterDriveFuelEcoValue, 999.0) == true ||
                almostEqual(mPrivate_AfterDriveFuelEcoValue, 999.0) == true) {
                outputDrvInfoAvgFuelConsValue = 999.0;
            } else if (surelyLessThan(mPrivate_AfterDriveFuelEcoValue, 100.0) == true) {
                outputDrvInfoAvgFuelConsValue =
                    roundOff1DecimalValue(mPrivate_AfterDriveFuelEcoValue) * kOutputDrvInfoAvgFuelConsValueFactor;
                digitStat = OutputDrvInfoAvgFuelConsDigit::DIGIT2;
            } else {
                outputDrvInfoAvgFuelConsValue = round(mPrivate_AfterDriveFuelEcoValue);
            }
            updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsValue.getAddress(),
                                static_cast<HUInt64>(outputDrvInfoAvgFuelConsValue));
            updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsDigit.getAddress(),
                                static_cast<HUInt64>(digitStat));
            flushActuatorOutput();
        }
    }

    // 4.15.3 신호 송출 (Output_DrvInfoAvgFuelConsResetStatus)
    void updateOutputDrvInfoAvgFuelConsResetStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mPrivate_DriveInfoResetStatus == DriveInfoResetStatus::RESET) {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsResetStatus.getAddress(),
                                    static_cast<HUInt64>(OutputDrvInfoAvgFuelConsResetStatus::RESET));
            } else {
                updateActuatorValue(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsResetStatus.getAddress(),
                                    static_cast<HUInt64>(OutputDrvInfoAvgFuelConsResetStatus::DEFAULT));
            }
            flushActuatorOutput();
        }
    }

    // 5.2 표시 구성요소 정보 처리 로직
    // 5.2.1 순간 연비 (이동 평균 계산 방식)
    void updateMoveAvgTypeInstFuelEconomy() {
        HDouble instFuelEconomyValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoKmL;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoL100km;
                    } else {
                        DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoUsMPG;
                    } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoUkMPG;
                    } else {
                        DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                    }
                } else {
                    DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                }
            } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstHydrogenEcoKmKg;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstHydrogenEcoKg100km;
                    } else {
                        DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    instFuelEconomyValue = mPrivate_InstHydrogenEcoMPG;
                } else {
                    DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCTrip_InfoConstantInstFuelEconomyValue(instFuelEconomyValue);
    }

    // 5.2.2 순간 연비 (입력 기반 표시 방식)
    void updateInputDisplayTypeInstFuelEconomy() {
        HDouble instFuelEconomyValue = 0.0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
                if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                    if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKmKwh;
                    } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                        instFuelEconomyValue = mPrivate_InstFuelEcoInputKwh100km;
                    } else {
                        DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                    }
                } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                    instFuelEconomyValue = mPrivate_InstFuelEcoInputMiKwh;
                } else {
                    DDebug() << __FUNCTION__ << " instFuelEconomyValue is zero, but is not update.";
                }
            } else {
                instFuelEconomyValue = 0.0;
            }
        } else {
            instFuelEconomyValue = 0.0;
        }
        setSFCTrip_InfoConstantInstFuelEconomyInputTypeValue(instFuelEconomyValue);
    }

    // 5.2.3 누적 정보
    // 5.2.3.1 거리 (누적)
    void updateAccumulatedInfoDistance() {
        SFCTrip_InfoConstantAccumulDistanceStat stat = SFCTrip_InfoConstantAccumulDistanceStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HDouble accumulDistanceValue = 0.0;
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                accumulDistanceValue = mPrivate_AccumDistanceKm;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                accumulDistanceValue = mPrivate_AccumDistanceMi;
            } else {
                stat = SFCTrip_InfoConstantAccumulDistanceStat::DISPLAY_OFF;
            }
            setSFCTrip_InfoConstantAccumulDistanceValue(accumulDistanceValue);
        } else {
            stat = SFCTrip_InfoConstantAccumulDistanceStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAccumulDistanceStat(stat);
    }

    // 5.2.3.1 시간 (누적)
    void updateAccumulatedInfoTime() {
        if (mIgnitionState == IgnitionState::ON) {
            setSFCTrip_InfoConstantAccumulTimeValue(mPrivate_AccumTime);
            setSFCTrip_InfoConstantAccumulTimeStat(SFCTrip_InfoConstantAccumulTimeStat::DISPLAY);
        } else {
            setSFCTrip_InfoConstantAccumulTimeStat(SFCTrip_InfoConstantAccumulTimeStat::DISPLAY_OFF);
        }
    }

    // 5.2.3.1 속도 (누적)
    void updateAccumulatedInfoSpeed() {
        SFCTrip_InfoConstantAccumulSpeedStat stat = SFCTrip_InfoConstantAccumulSpeedStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HUInt64 accumulSpeedValue = 0;
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                accumulSpeedValue = mPrivate_AccumVehicleSpeedKph;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                accumulSpeedValue = mPrivate_AccumVehicleSpeedMph;
            } else {
                stat = SFCTrip_InfoConstantAccumulSpeedStat::DISPLAY_OFF;
            }
            setSFCTrip_InfoConstantAccumulSpeedValue(accumulSpeedValue);
        } else {
            stat = SFCTrip_InfoConstantAccumulSpeedStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAccumulSpeedStat(stat);
    }

    // 5.2.3.2 연비 (누적)
    void updateAccumulatedFuelEconomy() {
        SFCTrip_InfoConstantAccumulFuelEconomyStat stat = SFCTrip_InfoConstantAccumulFuelEconomyStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HDouble accumulFuelEconomyValue = 0.0;
            if ((surelyGreaterThan(mPrivate_AccumDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                 almostEqual(mPrivate_AccumDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                surelyLessThan(mPrivate_AccumFuelEconomyValue, 100.0) == true) {
                accumulFuelEconomyValue = roundOff1DecimalValue(mPrivate_AccumFuelEconomyValue);
            } else if ((surelyGreaterThan(mPrivate_AccumDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                        almostEqual(mPrivate_AccumDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                       surelyLessThan(mPrivate_AccumFuelEconomyValue, 100.0) == false) {
                accumulFuelEconomyValue = round(mPrivate_AccumFuelEconomyValue);
            } else {
                accumulFuelEconomyValue = 0.0;
            }
            setSFCTrip_InfoConstantAccumulFuelEconomyValue(accumulFuelEconomyValue);
        } else {
            stat = SFCTrip_InfoConstantAccumulFuelEconomyStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAccumulFuelEconomyStat(stat);
    }

    // 5.2.4 주행정보
    // 5.2.4.1 거리 (주행)
    void updateDrivingInfoDistance() {
        HDouble driveDistanceValue = 0.0;
        SFCTrip_InfoConstantDriveDistanceStat stat = SFCTrip_InfoConstantDriveDistanceStat::DISPLAY;
        if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
            driveDistanceValue = mInter_DriveInfoDistanceKm;
        } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
            driveDistanceValue = mInter_DriveInfoDistanceMi;
        } else {
            stat = SFCTrip_InfoConstantDriveDistanceStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantDriveDistanceValue(driveDistanceValue);
        setSFCTrip_InfoConstantDriveDistanceStat(stat);
    }

    // 5.2.4.1 시간 (주행)
    void updateDrivingInfoTime() {
        setSFCTrip_InfoConstantDriveTimeValue(mInter_DriveInfoTime);
        setSFCTrip_InfoConstantDriveTimeStat(SFCTrip_InfoConstantDriveTimeStat::DISPLAY);
    }

    // 5.2.4.1 속도 (주행)
    void updateDrivingInfoSpeed() {
        HUInt64 driveSpeedValue = 0;
        SFCTrip_InfoConstantDriveSpeedStat stat = SFCTrip_InfoConstantDriveSpeedStat::DISPLAY;
        if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
            driveSpeedValue = mPrivate_DriveInfoVehicleSpeedKph;
            setSFCTrip_InfoInter_DriveInfoVehicleSpeedKph(driveSpeedValue);
        } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
            driveSpeedValue = mPrivate_DriveInfoVehicleSpeedMph;
            setSFCTrip_InfoInter_DriveInfoVehicleSpeedMph(driveSpeedValue);
        } else {
            stat = SFCTrip_InfoConstantDriveSpeedStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantDriveSpeedValue(driveSpeedValue);
        setSFCTrip_InfoConstantDriveSpeedStat(stat);
    }

    // 5.2.4.2 연비 (주행)
    void updateDrivingInfoFuelEconomy() {
        SFCTrip_InfoConstantDriveFuelEconomyStat stat = SFCTrip_InfoConstantDriveFuelEconomyStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HDouble driveFuelEconomyValue = 0.0;
            if ((surelyGreaterThan(mInter_DriveInfoDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                 almostEqual(mInter_DriveInfoDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                surelyLessThan(mInter_DriveInfoFuelEcoValue, 100.0) == true) {
                driveFuelEconomyValue = roundOff1DecimalValue(mInter_DriveInfoFuelEcoValue);
            } else if ((surelyGreaterThan(mInter_DriveInfoDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                        almostEqual(mInter_DriveInfoDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                       surelyLessThan(mInter_DriveInfoFuelEcoValue, 100.0) == false) {
                driveFuelEconomyValue = round(mInter_DriveInfoFuelEcoValue);
            } else {
                driveFuelEconomyValue = 0.0;
            }
            setSFCTrip_InfoConstantDriveFuelEconomyValue(driveFuelEconomyValue);
            setSFCTrip_InfoInter_DriveInfoFuelEcoValue(driveFuelEconomyValue);
        } else {
            stat = SFCTrip_InfoConstantDriveFuelEconomyStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantDriveFuelEconomyStat(stat);
    }

    // 5.2.5 주유 후 정보
    // 5.2.5.1 거리 (주유 후)
    void updateAfterRefuelDistance() {
        SFCTrip_InfoConstantAfterRefuelDistanceStat stat = SFCTrip_InfoConstantAfterRefuelDistanceStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HDouble afterRefuelDistanceValue = 0.0;
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                afterRefuelDistanceValue = mPrivate_AfterRefuelDistanceKm;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                afterRefuelDistanceValue = mPrivate_AfterRefuelDistanceMi;
            } else {
                stat = SFCTrip_InfoConstantAfterRefuelDistanceStat::DISPLAY_OFF;
            }
            setSFCTrip_InfoConstantAfterRefuelDistanceValue(afterRefuelDistanceValue);
        } else {
            stat = SFCTrip_InfoConstantAfterRefuelDistanceStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAfterRefuelDistanceStat(stat);
    }

    // 5.2.5.1 시간 (주유 후)
    void updateAfterRefuelTime() {
        if (mIgnitionState == IgnitionState::ON) {
            setSFCTrip_InfoConstantAfterRefuelTimeValue(mPrivate_AfterRefuelTime);
            setSFCTrip_InfoConstantAfterRefuelTimeStat(SFCTrip_InfoConstantAfterRefuelTimeStat::DISPLAY);
        } else {
            setSFCTrip_InfoConstantAfterRefuelTimeStat(SFCTrip_InfoConstantAfterRefuelTimeStat::DISPLAY_OFF);
        }
    }

    // 5.2.5.1 속도 (주유 후)
    void updateAfterRefuelSpeed() {
        SFCTrip_InfoConstantAfterRefuelSpeedStat stat = SFCTrip_InfoConstantAfterRefuelSpeedStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HUInt64 afterRefuelSpeedValue = 0;
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                afterRefuelSpeedValue = mPrivate_AfterRefuelVehicleSpeedKph;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                afterRefuelSpeedValue = mPrivate_AfterRefuelVehicleSpeedMph;
            } else {
                stat = SFCTrip_InfoConstantAfterRefuelSpeedStat::DISPLAY_OFF;
            }
            setSFCTrip_InfoConstantAfterRefuelSpeedValue(afterRefuelSpeedValue);
        } else {
            stat = SFCTrip_InfoConstantAfterRefuelSpeedStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAfterRefuelSpeedStat(stat);
    }

    // 5.2.5.2 연비 (주유 후)
    void updateAfterRefuelFuelEconomy() {
        SFCTrip_InfoConstantAfterRefuelFuelEconomyStat stat = SFCTrip_InfoConstantAfterRefuelFuelEconomyStat::DISPLAY;
        if (mIgnitionState == IgnitionState::ON) {
            HDouble afterRefuelFuelEconomyValue = 0.0;
            if ((surelyGreaterThan(mPrivate_AfterRefuelDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                 almostEqual(mPrivate_AfterRefuelDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                surelyLessThan(mPrivate_AfterRefuelFuelEconomyValue, 100.0) == true) {
                afterRefuelFuelEconomyValue = roundOff1DecimalValue(mPrivate_AfterRefuelFuelEconomyValue);
            } else if ((surelyGreaterThan(mPrivate_AfterRefuelDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true ||
                        almostEqual(mPrivate_AfterRefuelDistanceKm, kMinDistanceForUpdateFuelEconomyValue) == true) &&
                       surelyLessThan(mPrivate_AfterRefuelFuelEconomyValue, 100.0) == false) {
                afterRefuelFuelEconomyValue = round(mPrivate_AfterRefuelFuelEconomyValue);
            } else {
                afterRefuelFuelEconomyValue = 0.0;
            }
            setSFCTrip_InfoConstantAfterRefuelFuelEconomyValue(afterRefuelFuelEconomyValue);
        } else {
            stat = SFCTrip_InfoConstantAfterRefuelFuelEconomyStat::DISPLAY_OFF;
        }
        setSFCTrip_InfoConstantAfterRefuelFuelEconomyStat(stat);
    }

    // 5.2.6.1 누적 주행 거리 값 표시
    void updateOdometerValue() {
        HUInt64 private_OdometerIntegerValue;
        static HUInt64 prevPrivate_OdometerIntegerValue = 0;

        if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
            private_OdometerIntegerValue = static_cast<HUInt64>(floor(mPrivate_OdometerDisplayValue * kFactor_ConvKmToMi));
        } else {
            private_OdometerIntegerValue = static_cast<HUInt64>(floor(mPrivate_OdometerDisplayValue));
        }

        // NOTE : 최초 부팅 시 Odometer 값이 0인 상태에서 클러스터 화면에 Km만 표시되는 버그 개선을 위한
        // NOTE : private_OdometerIntegerValue == 0 조건 추가
        if (prevPrivate_OdometerIntegerValue != private_OdometerIntegerValue || private_OdometerIntegerValue == 0) {
            setSFCTrip_InfoConstantOdometerValue(private_OdometerIntegerValue);
            // NOTE : 4.7 계산값 저장 및 불러오기 총 주행 거리가 1 증가 하면 저장
            saveDataToMemoryOnOdometerIntergerChanged();
            _DInfo() << __FUNCTION__ << " Private_OdometerTotal : " << private_OdometerIntegerValue;
            prevPrivate_OdometerIntegerValue = private_OdometerIntegerValue;
        }
    }

    // 5.2.6.2 누적 주행거리 상태 표시
    void updateOdometerStat() {
        if (mIgnitionState == IgnitionState::ON) {
            SFCTrip_InfoConstantOdometerStat stat = SFCTrip_InfoConstantOdometerStat::ON;
            if (mInter_ConfigArea == ConfigArea::EEC && mInput_DTGOdometer > klimitDTGOdometer) {
                stat = SFCTrip_InfoConstantOdometerStat::BLINK4;
            }
            setSFCTrip_InfoConstantOdometerStat(stat);
        }
    }

    // 5.2.7 EV/FCEV 주행 가능 거리
    void updateElecDte() {
        HUInt64 input_EvDTEValuePerKm = 0;
        SFCTrip_InfoConstantElecDTEStat stat = SFCTrip_InfoConstantElecDTEStat::VALUE;
        if (mIgnitionState == IgnitionState::OFF && mInput_ElecDteStatus == ElecDteStatus::MESSAGE_TIMEOUT) {
            stat = SFCTrip_InfoConstantElecDTEStat::DISPLAY_OFF;
        } else {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                input_EvDTEValuePerKm = mInput_EvDTEValuePerKm;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                input_EvDTEValuePerKm = static_cast<HUInt64>(round(mInput_EvDTEValuePerKm * kFactor_ConvKmToMi));
            } else {
                return;
            }
        }
        setSFCTrip_InfoConstantElecDTEStat(stat);
        setSFCTrip_InfoConstantElecDTEValue(input_EvDTEValuePerKm);
    }

    // 5.2.8 단위 (IMG_FuelEcoUnit_stat)
    void updateFuelEcoUnit() {
        SFCTrip_InfoConstantFuelEcoUnitStat fuelEcoUnitStat;
        if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER &&
                    mInter_ConfigFuelTypeCV == ConfigFuelTypeCV::DSL) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_LITTER;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER &&
                           mInter_ConfigFuelTypeCV == ConfigFuelTypeCV::CNG) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_M3;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM &&
                           mInter_ConfigFuelTypeCV == ConfigFuelTypeCV::DSL) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::LITTER_PER_KM;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM &&
                           mInter_ConfigFuelTypeCV == ConfigFuelTypeCV::CNG) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::M3_PER_KM;
                } else {
                    return;
                }
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::US_GALLON) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::US_MPG;
                } else if (mInter_FuelEcoUnitMiSetStatus == FuelEcoUnitMiSetStatus::UK_GALLON) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::UK_MPG;
                } else {
                    return;
                }
            } else {
                return;
            }
        } else if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_KWH;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KWH_PER_KM;
                } else {
                    return;
                }
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::MILE_PER_KWH;
            } else {
                return;
            }
        } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::KM_PER_LITTER) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KM_PER_KG;
                } else if (mInter_FuelEcoUnitKmSetStatus == FuelEcoUnitKmSetStatus::LITTER_PER_KM) {
                    fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::KG_PER_KM;
                } else {
                    return;
                }
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                fuelEcoUnitStat = SFCTrip_InfoConstantFuelEcoUnitStat::MPGE;
            } else {
                return;
            }
        } else {
            return;
        }
        setSFCTrip_InfoConstantFuelEcoUnitStat(fuelEcoUnitStat);
    }

    // 5.2.8 단위 (IMG_TripDistanceUnit_stat, IMG_TripVehicleSpeedUnit_stat)
    void updateTripDistanceUnit() {
        SFCTrip_InfoConstantTripDistanceUnitStat tripDistanceUnit;
        SFCTrip_InfoConstantTripVehicleSpeedUnitStat tripVehicleSpeedUnit;
        if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
            tripDistanceUnit = SFCTrip_InfoConstantTripDistanceUnitStat::KM;
            tripVehicleSpeedUnit = SFCTrip_InfoConstantTripVehicleSpeedUnitStat::KM_PER_HOUR;
        } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
            tripDistanceUnit = SFCTrip_InfoConstantTripDistanceUnitStat::MILE;
            tripVehicleSpeedUnit = SFCTrip_InfoConstantTripVehicleSpeedUnitStat::MILE_PER_HOUR;
        } else {
            return;
        }
        setSFCTrip_InfoConstantTripDistanceUnitStat(tripDistanceUnit);
        setSFCTrip_InfoConstantTripVehicleSpeedUnitStat(tripVehicleSpeedUnit);
    }

    // 5.2.9 세부 항목 명칭 표시 형태
    void updateTripInfoSubTitleType() {
        if (mInter_DisplayLanguageStatus == DisplayLanguageStatus::KOREA ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::SIMPLIFIED_CHINESE ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::ENGLISH_US ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::FRENCH_US ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::SPANISH_US ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::TRADITIONAL_CHINESE ||
            mInter_DisplayLanguageStatus == DisplayLanguageStatus::JAPANESE) {
            setSFCTrip_InfoConstantTripInfoSubTitleTypeStat(SFCTrip_InfoConstantTripInfoSubTitleTypeStat::TEXT);
        } else {
            setSFCTrip_InfoConstantTripInfoSubTitleTypeStat(SFCTrip_InfoConstantTripInfoSubTitleTypeStat::SYMBOL);
        }
    }

    // 5.2.10 주행가능거리 심볼 형상
    void updateDteSymbol() {
        SFCTrip_InfoConstantDTESymbolStat dteSymbolStat;
        if (mInter_ConfigVehicleType == ConfigVehicleType::ICV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::GAS_STATION;
        } else if (mInter_ConfigVehicleType == ConfigVehicleType::EV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::PLUG;
        } else if (mInter_ConfigVehicleType == ConfigVehicleType::FCEV) {
            dteSymbolStat = SFCTrip_InfoConstantDTESymbolStat::H2;
        } else {
            return;
        }
        setSFCTrip_InfoConstantDTESymbolStat(dteSymbolStat);
    }

    // 5.2.11 EV 주행가능거리 경로 반영중 상태 표시
    void updateDteAvnCalStat() {
        if (mInput_EvDteAvnCalcStatus == EvDteAvnCalcStatus::ON) {
            setSFCTrip_InfoConstantDTEAvnCalStat(SFCTrip_InfoConstantDTEAvnCalStat::ON);
        } else if (mInput_EvDteAvnCalcStatus == EvDteAvnCalcStatus::OFF) {
            setSFCTrip_InfoConstantDTEAvnCalStat(SFCTrip_InfoConstantDTEAvnCalStat::OFF);
        } else {
            // no operation
        }
    }

    // 5.2.12 EV 주행가능거리 OFF 표시
    void updateDteDashDisplayForEvStat() {
        if (mInput_EvDteDisplayOffReqStatus == EvDteDisplayOffReqStatus::OFF_REQUEST) {
            setSFCTrip_InfoConstantDTEDashDisplayForEvStat(SFCTrip_InfoConstantDTEDashDisplayForEvStat::ON);
        } else if (mInput_EvDteDisplayOffReqStatus == EvDteDisplayOffReqStatus::DEFAULT) {
            setSFCTrip_InfoConstantDTEDashDisplayForEvStat(SFCTrip_InfoConstantDTEDashDisplayForEvStat::OFF);
        } else {
            // no operation
        }
    }

    // 5.2.13 EV 주행가능거리 오류 표시
    void updateDteDashBlinkDisplayForEv() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInput_ElecDteStatus == ElecDteStatus::MESSAGE_TIMEOUT) {
                setSFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat(SFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat::ON);
            } else {
                setSFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat(SFCTrip_InfoConstantDTEDashBlinkDisplayForEvStat::OFF);
            }
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시 (IMG_DTEGuideFunctionStatus_stat)
    void updateDteGuideFunctionStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            switch (mInput_EvDteGuidePageInfoStatus) {
                case EvDteGuidePageInfoStatus::NORMAL:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::NORMAL);
                    break;
                case EvDteGuidePageInfoStatus::RECALCULATE:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(
                        SFCTrip_InfoConstantDTEGuideFunctionStatusStat::RECALCULATE);
                    break;
                case EvDteGuidePageInfoStatus::LOW_DTE:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::LOW_DTE);
                    break;
                case EvDteGuidePageInfoStatus::ERROR:
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::ERROR);
                    break;
                default:
                    // NOTE: Input_EvDteGuidePageInfoStatus는 아래와 같이 5개의 enum 값을 가지고 있기 때문에 default값은 NONE이됨
                    // NOTE: 0x0: "NONE", 0x1: "NORMAL", 0x2: "RECALCULATE", 0x3: "LOW_DTE" 0x4: "ERROR"
                    setSFCTrip_InfoConstantDTEGuideFunctionStatusStat(SFCTrip_InfoConstantDTEGuideFunctionStatusStat::NONE);
                    break;
            }
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시 (IMG_DTEGuideLowBound_value)
    void updateDteGuideLowBoundValue() {
        HUInt64 value = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                value = mInput_EvDteLowBoundValue;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                value = static_cast<HUInt64>(round(mInput_EvDteLowBoundValue * kFactor_ConvKmToMi));
            } else {
                return;
            }
            setSFCTrip_InfoConstantDTEGuideLowBoundValue(value);
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시 (IMG_DTEGuideUpperBound_value)
    void updateDteGuideUpperBoundValue() {
        HUInt64 value = 0;
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_TripDistanceUnit == TripDistanceUnit::KM) {
                value = mInput_EvDteUpperBoundValue;
            } else if (mInter_TripDistanceUnit == TripDistanceUnit::MILE) {
                value = static_cast<HUInt64>(round(mInput_EvDteUpperBoundValue * kFactor_ConvKmToMi));
            } else {
                return;
            }
            setSFCTrip_InfoConstantDTEGuideUpperBoundValue(value);
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시 (IMG_DTEGuideMainDtePosition_value)
    void updateDteGuideMainDtePositionValue() {
        if (mIgnitionState == IgnitionState::ON && mInput_EvDteGuideDtePositionValue <= kEvDteGuidePostionMaxValue) {
            setSFCTrip_InfoConstantDTEGuideMainDtePositionValue(
                static_cast<HInt64>(mInput_EvDteGuideDtePositionValue - kEvDteGuidePostionOffset));
        }
    }

    // 5.2.14 EV 주행가능거리 GUIDE 표시 (IMG_DTEGuideCoachPosition_value)
    void updateDteGuideCoachPositionValue() {
        if (mIgnitionState == IgnitionState::ON && mInput_EvDteGuideCoachPositionValue <= kEvDteGuidePostionMaxValue) {
            setSFCTrip_InfoConstantDTEGuideCoachPositionValue(
                static_cast<HInt64>(mInput_EvDteGuideCoachPositionValue - kEvDteGuidePostionOffset));
        }
    }

    // parameter 및 memory data 로드
    void loadParameterAndMemory(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mPar_OdoCorrectionFactor, args.mPar_OdoCorrectionFactor) == true) {
            _DInfo() << __FUNCTION__ << " Par_OdoCorrectionFactor : " << mPar_OdoCorrectionFactor;
        } else {
            _DInfo() << __FUNCTION__ << " Par_OdoCorrectionFactor : 0.0 (default)";
        }
        // 4.7 계산 값 불러오기
        HDouble odometer = 0.0;
        HResult result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_OdometerValue], odometer);
        if (result == HResult::OK) {
            mPrivate_OdometerTotal = odometer;
            _DInfo() << __FUNCTION__ << "Private_OdometerTotal : " << mPrivate_OdometerTotal;
        } else {
            _DInfo() << __FUNCTION__ << "Private_OdometerTotal : 0.0 (default)";
        }
        HDouble afterDriveFuelEcoValue = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterDriveFuelEcoValue], afterDriveFuelEcoValue);
        if (result == HResult::OK) {
            mPrivate_AfterDriveFuelEcoValue = afterDriveFuelEcoValue;
            _DInfo() << __FUNCTION__ << " Private_AfterDriveFuelEcoValue : " << mPrivate_AfterDriveFuelEcoValue;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterDriveFuelEcoValue : 0.0 (default)";
        }
        HDouble fuelConsumpt = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_FuelConsumptValue], fuelConsumpt);
        if (result == HResult::OK) {
            mPrivate_FuelConsumptionTotal = fuelConsumpt;
            _DInfo() << __FUNCTION__ << " Private_FuelConsumptionTotal : " << mPrivate_FuelConsumptionTotal;
        } else {
            _DInfo() << __FUNCTION__ << " Private_FuelConsumptionTotal : 0.0 (default)";
        }
        HDouble elecFuelConsumpt = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_ElecFuelConsumptValue], elecFuelConsumpt);
        if (result == HResult::OK) {
            mPrivate_ElecFuelConsumptionTotal = elecFuelConsumpt;
            _DInfo() << __FUNCTION__ << " Private_ElecFuelConsumptionTotal : " << mPrivate_ElecFuelConsumptionTotal;
        } else {
            _DInfo() << __FUNCTION__ << " Private_ElecFuelConsumptionTotal : 0.0 (default)";
        }
        HDouble hydrogenFuelConsumpt = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_HydrogenConsumptionValue], hydrogenFuelConsumpt);
        if (result == HResult::OK) {
            mPrivate_HydrogenConsumptionTotal = hydrogenFuelConsumpt;
            _DInfo() << __FUNCTION__ << " Private_HydrogenConsumptionTotal : " << mPrivate_HydrogenConsumptionTotal;
        } else {
            _DInfo() << __FUNCTION__ << " Private_HydrogenConsumptionTotal : 0.0 (default)";
        }
        HUInt64 drivingTime = 0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DrivingTimeValue], drivingTime);
        if (result == HResult::OK) {
            mPrivate_DrivingTimeTotal = drivingTime;
            _DInfo() << __FUNCTION__ << " Private_DrivingTimeTotal : " << mPrivate_DrivingTimeTotal;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DrivingTimeTotal : 0 (default)";
        }
        HDouble accumStartDistance = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartDistanceValue], accumStartDistance);
        if (result == HResult::OK) {
            mPrivate_AccumStartDistance = accumStartDistance;
            _DInfo() << __FUNCTION__ << " Private_AccumStartDistance : " << mPrivate_AccumStartDistance;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AccumStartDistance : 0.0 (default)";
        }
        HUInt64 accumStartTime = 0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartTimeValue], accumStartTime);
        if (result == HResult::OK) {
            mPrivate_AccumStartTime = accumStartTime;
            _DInfo() << __FUNCTION__ << " Private_AccumStartTime : " << mPrivate_AccumStartTime;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AccumStartTime : 0 (default)";
        }
        HDouble accumStartFuelCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartFuelConsValue], accumStartFuelCons);
        if (result == HResult::OK) {
            mPrivate_AccumStartFuelCons = accumStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AccumStartFuelCons : " << mPrivate_AccumStartFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AccumStartFuelCons : 0.0 (default)";
        }
        HDouble accumStartElecCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartElecFuelConsValue], accumStartElecCons);
        if (result == HResult::OK) {
            mPrivate_AccumStartElecFuelCons = accumStartElecCons;
            _DInfo() << __FUNCTION__ << " Private_AccumStartElecFuelCons : " << mPrivate_AccumStartElecFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AccumStartElecFuelCons : 0.0 (default)";
        }
        HDouble accumStartHydrogenCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AccumStartHydrogenValue], accumStartHydrogenCons);
        if (result == HResult::OK) {
            mPrivate_AccumStartHydrogenCons = accumStartHydrogenCons;
            _DInfo() << __FUNCTION__ << " Private_AccumStartHydrogenCons : " << mPrivate_AccumStartHydrogenCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AccumStartHydrogenCons : 0.0 (default)";
        }
        HDouble driveStartDistance = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartDistanceValue], driveStartDistance);
        if (result == HResult::OK) {
            mPrivate_DriveStartDistance = driveStartDistance;
            _DInfo() << __FUNCTION__ << " Private_DriveStartDistance : " << mPrivate_DriveStartDistance;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DriveStartDistance : 0.0 (default)";
        }
        HUInt64 driveStartTime = 0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartTimeValue], driveStartTime);
        if (result == HResult::OK) {
            mPrivate_DriveStartTime = driveStartTime;
            _DInfo() << __FUNCTION__ << " Private_DriveStartTime : " << mPrivate_DriveStartTime;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DriveStartTime : 0 (default)";
        }
        HDouble driveStartFuelCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartFuelConsValue], driveStartFuelCons);
        if (result == HResult::OK) {
            mPrivate_DriveStartFuelCons = driveStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartFuelCons : " << mPrivate_DriveStartFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DriveStartFuelCons : 0.0 (default)";
        }
        HDouble driveStartElecFuelCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartElecFuelConsValue], driveStartElecFuelCons);
        if (result == HResult::OK) {
            mPrivate_DriveStartElecFuelCons = driveStartElecFuelCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartElecFuelCons : " << mPrivate_DriveStartElecFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DriveStartElecFuelCons : 0.0 (default)";
        }
        HDouble driveStartHydrogenCons = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_DriveStartHydrogenValue], driveStartHydrogenCons);
        if (result == HResult::OK) {
            mPrivate_DriveStartHydrogenCons = driveStartHydrogenCons;
            _DInfo() << __FUNCTION__ << " Private_DriveStartHydrogenCons : " << mPrivate_DriveStartHydrogenCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_DriveStartHydrogenCons : 0.0 (default)";
        }
        HDouble afterRefuelStartDistance = 0.0;
        result =
            getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartDistanceValue], afterRefuelStartDistance);
        if (result == HResult::OK) {
            mPrivate_AfterRefuelStartDistance = afterRefuelStartDistance;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartDistance : " << mPrivate_AfterRefuelStartDistance;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartDistance : 0.0 (default)";
        }
        HUInt64 afterRefuelStartTime = 0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartTimeValue], afterRefuelStartTime);
        if (result == HResult::OK) {
            mPrivate_AfterRefuelStartTime = afterRefuelStartTime;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartTime : " << mPrivate_AfterRefuelStartTime;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartTime : 0 (default)";
        }
        HDouble afterRefuelStartFuelCons = 0.0;
        result =
            getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartFuelConsValue], afterRefuelStartFuelCons);
        if (result == HResult::OK) {
            mPrivate_AfterRefuelStartFuelCons = afterRefuelStartFuelCons;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartFuelCons : " << mPrivate_AfterRefuelStartFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartFuelCons : 0.0 (default)";
        }
        HDouble afterRefuelStartElecCons = 0.0;
        result =
            getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartElecFuelConsValue], afterRefuelStartElecCons);
        if (result == HResult::OK) {
            mPrivate_AfterRefuelStartElecFuelCons = afterRefuelStartElecCons;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartElecFuelCons : " << mPrivate_AfterRefuelStartElecFuelCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartElecFuelCons : 0.0 (default)";
        }
        HDouble afterRefuelStartHydrogenCons = 0.0;
        result =
            getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_AfterRefuelStartHydrogenValue], afterRefuelStartHydrogenCons);
        if (result == HResult::OK) {
            mPrivate_AfterRefuelStartHydrogenCons = afterRefuelStartHydrogenCons;
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartHydrogenCons : " << mPrivate_AfterRefuelStartHydrogenCons;
        } else {
            _DInfo() << __FUNCTION__ << " Private_AfterRefuelStartHydrogenCons : 0.0 (default)";
        }
        // NOTE : 4.2.2 B+ 리셋시 EEPROM 에 저장된 DTGOdometerValue 값을 가져온다.
        HDouble odometerDisplayValue = 0.0;
        result = getCachedValue(mMemoryAddressMap[MemoryAddrName::Memory_OdometerDisplayValue], odometerDisplayValue);
        if (result == HResult::OK) {
            mPrivate_OdometerDisplayValue = odometerDisplayValue;
            mMemory_OdometerDisplayValue = odometerDisplayValue;
            _DInfo() << __FUNCTION__ << " Memory_OdometerDisplayValue : " << mPrivate_OdometerDisplayValue;
        } else {
            _DInfo() << __FUNCTION__ << " Memory_OdometerDisplayValue : 0.0 (default)";
        }
        update10sTimerOnIgnOnOffAndAemInitStatus();
        onRepeatTimerCalculationAccumFuelEcoKmL10s();
        onRepeatTimerCalculationAccumFuelEcoKmKwh10s();
        onRepeatTimerCalculationAccumHydrogenEco10s();
        onRepeatTimerCalculationAccumVehicleSpeedKph10s();
        onRepeatTimerCalculationDriveFuelEcoKmL10s();
        onRepeatTimerCalculationDriveFuelEcoKmKwh10s();
        onRepeatTimerCalculationDriveHydrogenEco10s();
        onRepeatTimerCalculationDriveInfoVehicleSpeedKph10s();
        onRepeatTimerCalculationAfterRefuelFuelEcoKmL10s();
        onRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10s();
        onRepeatTimerCalculationAfterRefuelHydrogenEco10s();
        onRepeatTimerCalculationAfterRefuelVehicleSpeedKph10s();
        updateOutputDrvInfoAvgFuelCons();
        updateOdometerValue();
        mPrevPrivate_AfterRefuelStartDistance = mPrivate_AfterRefuelStartDistance;
    }

    // NOTE : 4.8 표시값 계산 시작점 저장 Input_BmsDisplaySocValue값이 10.0 이상 증가시 조건
    void determineBmsDisplaySocValueIncrease10OnBmsChgStatus() {
        static HUInt64 input_BmsDisplaySocValue = 0;
        mIsDisplaySocValue10IncreasedStatus = false;
        if (mInput_BmsChgStatus == BmsChgStatus::CHARGING) {
            if (mPrevInput_BmsChgStatus == BmsChgStatus::OFF) {
                input_BmsDisplaySocValue = mInput_BmsDisplaySocValue;
                _DInfo() << __FUNCTION__ << " at start of charging Input_BmsDisplaySocValue : " << input_BmsDisplaySocValue;
            }
            if (mInput_BmsDisplaySocValue >= input_BmsDisplaySocValue + 10) {
                _DInfo() << __FUNCTION__ << " Input_BmsDisplaySocValue increased by more than 10 while charged";
                mIsDisplaySocValue10IncreasedStatus = true;
            }
        }
        saveAfterRefuelStartPointToMemory();
    }

    void flushActuatorOutput() {
        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    inline HBool isValidPositive(HDouble value) const {
        return surelyGreaterThan(value, 0.0) == true;
    }

    // 소수점 2자리로 Round Off 하는 함수
    inline HDouble roundOff2DecimalValue(const HDouble& value) {
        return round(value * 100.0) / 100.0;
    }

    // 소수점 1자리로 Round Off 하는 함수
    inline HDouble roundOff1DecimalValue(const HDouble& value) {
        return round(value * 10.0) / 10.0;
    }

    template <typename T>
    inline void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    enum class AdmStatus : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    enum class IgnitionState : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    enum class MemoryAddrName : HUInt64 {
        NONE,
        Memory_DrivingTimeValue,
        Memory_FuelConsumptValue,
        Memory_ElecFuelConsumptValue,
        Memory_HydrogenConsumptionValue,
        Memory_OdometerValue,
        Memory_AccumStartDistanceValue,
        Memory_AccumStartHydrogenValue,
        Memory_AccumStartElecFuelConsValue,
        Memory_AccumStartFuelConsValue,
        Memory_AccumStartTimeValue,
        Memory_AfterRefuelStartDistanceValue,
        Memory_AfterRefuelStartHydrogenValue,
        Memory_AfterRefuelStartElecFuelConsValue,
        Memory_AfterRefuelStartFuelConsValue,
        Memory_AfterRefuelStartTimeValue,
        Memory_DriveStartDistanceValue,
        Memory_DriveStartHydrogenValue,
        Memory_DriveStartElecFuelConsValue,
        Memory_DriveStartFuelConsValue,
        Memory_DriveStartTimeValue,
        Memory_AfterDriveFuelEcoValue,
        Memory_OdometerDisplayValue,
        MAX
    };

    enum class DisplayFuelEconomyType : HUInt64 {
        NONE,
        DRIVE,
        ACCUMULATION,
        AFTERREFUEL,
        ALL,
        MAX
    };

    enum class InstFuelEcoCalModeStatus : HUInt64 {
        NONE,
        INIT1,
        INIT2,
        ON1,
        ON2,
        OFF,
        MAX
    };

    enum class DriveInfoResetStatus : HUInt64 {
        NONE,
        RESET,
        OFF,
        MAX
    };

    enum class ElecDteStatus : HUInt64 {
        NONE,
        VALUE,
        MESSAGE_TIMEOUT,
        MAX
    };

    enum class Private_DTGInputStatus : HUInt64 {
        NONE,
        INVALID,
        VALID,
        MAX
    };

    AddressValueList mActuatorOutputAddrList;
    using MemoryAddressMap = std::map<MemoryAddrName, std::string>;
    MemoryAddressMap mMemoryAddressMap = {
        {MemoryAddrName::Memory_DrivingTimeValue, "SFC.Extension.Memory.Inter_DrivingTimeValue"},
        {MemoryAddrName::Memory_FuelConsumptValue, "SFC.Extension.Memory.Inter_FuelConsumptValue"},
        {MemoryAddrName::Memory_ElecFuelConsumptValue, "SFC.Extension.Memory.Inter_ElecFuelConsumptValue"},
        {MemoryAddrName::Memory_HydrogenConsumptionValue, "SFC.Extension.Memory.Inter_HydrogenConsumptionValue"},
        {MemoryAddrName::Memory_OdometerValue, "SFC.Extension.Memory.Inter_OdometerValue"},
        {MemoryAddrName::Memory_AccumStartDistanceValue, "SFC.Extension.Memory.Inter_AccumStartDistanceValue"},
        {MemoryAddrName::Memory_AccumStartHydrogenValue, "SFC.Extension.Memory.Inter_AccumStartHydrogenValue"},
        {MemoryAddrName::Memory_AccumStartElecFuelConsValue, "SFC.Extension.Memory.Inter_AccumStartElecFuelConsValue"},
        {MemoryAddrName::Memory_AccumStartFuelConsValue, "SFC.Extension.Memory.Inter_AccumStartFuelConsValue"},
        {MemoryAddrName::Memory_AccumStartTimeValue, "SFC.Extension.Memory.Inter_AccumStartTimeValue"},
        {MemoryAddrName::Memory_AfterRefuelStartDistanceValue, "SFC.Extension.Memory.Inter_AfterRefuelStartDistanceValue"},
        {MemoryAddrName::Memory_AfterRefuelStartHydrogenValue, "SFC.Extension.Memory.Inter_AfterRefuelStartHydrogenValue"},
        {MemoryAddrName::Memory_AfterRefuelStartElecFuelConsValue,
         "SFC.Extension.Memory.Inter_AfterRefuelStartElecFuelConsValue"},
        {MemoryAddrName::Memory_AfterRefuelStartFuelConsValue, "SFC.Extension.Memory.Inter_AfterRefuelStartFuelConsValue"},
        {MemoryAddrName::Memory_AfterRefuelStartTimeValue, "SFC.Extension.Memory.Inter_AfterRefuelStartTimeValue"},
        {MemoryAddrName::Memory_DriveStartDistanceValue, "SFC.Extension.Memory.Inter_DriveStartDistanceValue"},
        {MemoryAddrName::Memory_DriveStartHydrogenValue, "SFC.Extension.Memory.Inter_DriveStartHydrogenValue"},
        {MemoryAddrName::Memory_DriveStartElecFuelConsValue, "SFC.Extension.Memory.Inter_DriveStartElecFuelConsValue"},
        {MemoryAddrName::Memory_DriveStartFuelConsValue, "SFC.Extension.Memory.Inter_DriveStartFuelConsValue"},
        {MemoryAddrName::Memory_DriveStartTimeValue, "SFC.Extension.Memory.Inter_DriveStartTimeValue"},
        {MemoryAddrName::Memory_AfterDriveFuelEcoValue, "SFC.Extension.Memory_IGN.Inter_AfterDriveFuelEcoValue"},
        {MemoryAddrName::Memory_OdometerDisplayValue, "SFC.Extension.Memory.Inter_OdometerDisplayValue"}};

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using ConfigArea = ArgumentsConfigChanged::Inter_ConfigArea;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using VehicleReadyStatus = ArgumentsVehicleReadyStatusChanged::Input_VehicleReadyStatus;
    using EngineOnStatus = ArgumentsEngineOnStatusChanged::Inter_EngineOnStatus;
    using IsgActiveStatus = ArgumentsIsgActiveStatusChanged::Inter_ISGActiveStatus;
    using ImuDrvDoorOpenStatus = ArgumentsImuDrvDoorOpenStatusChanged::Input_IMUDrvDoorOpenStatus;
    using ImuFrDoorOpenStatus = ArgumentsImuFrDoorOpenStatusChanged::Input_IMUFrDoorOpenStatus;
    using AccumInfoManualResetReq = ArgumentsAccumInfoManualResetReqChanged::Inter_AccumInfoManualResetReq;
    using DriveInfoManualResetReq = ArgumentsDriveInfoManualResetReqChanged::Inter_DriveInfoManualResetReq;
    using BmsChgStatus = ArgumentsBmsChargeStatusAndEvSocDisplayValueChanged::Input_BmsChgStatus;
    using AfterRefuelInfoManualResetReq = ArgumentsAfterRefuelInfoManualResetReqChanged::Inter_AfterRefuelInfoManualResetReq;
    using FuelDetectRealtimeModeStatus = ArgumentsFuelDetectRealtimeModeStatusChanged::Inter_FuelDetectRealtimeModeStatus;
    using FuelEcoUnitKmSetStatus = ArgumentsFuelEcoUnitKmSetStatusChanged::Inter_FuelEcoUnitKmSetStatus;
    using FuelEcoUnitMiSetStatus = ArgumentsFuelEcoUnitMiSetStatusChanged::Inter_FuelEcoUnitMiSetStatus;
    using EvDteDisplayOffReqStatus = ArgumentsEvDteDisplayOffReqStatusChanged::Input_EvDTEDisplayOffReqStatus;
    using DisplayLanguageStatus = ArgumentsDisplayLanguageStatusChanged::Inter_DisplayLanguageStatus;
    using EvDteGuidePageInfoStatus = ArgumentsEvDteGuidePageInfoStatusChanged::Input_EvDteGuidePageInfoStatus;
    using EvDteAvnCalcStatus = ArgumentsEvDteAvnCalcStatusChanged::Input_EvDTEAvnCalcStatus;
    using OutputAvgFuelEconomyDigitStatus = decltype(Vehicle.CV.Trip_Info_CV.Output_AvgFuelEconomyDigitStatus)::TYPE;
    using OutputTripUnitStatus = decltype(Vehicle.CV.Trip_Info_CV.Output_TripUnitStatus)::TYPE;
    using OutputAvgFuelEconomyUnitStatus = decltype(Vehicle.CV.Trip_Info_CV.Output_AvgFuelEconomyUnitStatus)::TYPE;
    using OutputDrvInfoAvgFuelConsResetStatus = decltype(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsResetStatus)::TYPE;
    using OutputDrvInfoAvgFuelConsDigit = decltype(Vehicle.CV.Trip_Info_CV.Output_DrvInfoAvgFuelConsDigit)::TYPE;

    static constexpr HDouble kFuelCalculationFactor = 0.05 * 1.0 / 3600.0 * 1.0 / 10.0;
    static constexpr HDouble kHydrogenCalculationFactor = 0.02 * 0.000001;
    // NOTE : * 100.0 연산은 onElectricityConsumptionChanged() 호출 되는 주기 (CAN DB에 따라 달라질 수 있음 상용 DB 배포시 확인)
    static constexpr HDouble kElecCalculationFactor = 0.01 / 1000.0 / 3600.0 * 100.0 / 1000.0;
    static constexpr HDouble k10MsecPerHourDouble = 1.0 / 360000.0;
    static constexpr HDouble kNsecPerHourDouble = 1.0 / 3600000000000.0;
    static constexpr HDouble kFactor_ConvKmToMi = 0.6213592233009709;
    static constexpr HDouble kFactor_ConvLitterToUsGal = 0.26418;
    static constexpr HDouble kFactor_ConvLitterToUkGal = 0.21997;
    static constexpr HDouble kOutputOdometerFactor = 10.0;
    static constexpr HDouble kOutputDrvInfoAvgFuelConsValueFactor = 10.0;
    static constexpr HDouble kOutputAvgFuelEconomyValueFactor = 10.0;
    static constexpr HDouble kMinDistanceForUpdateFuelEconomyValue = 0.3;
    static constexpr HUInt64 kEvDteGuidePostionMaxValue = 120;
    static constexpr HUInt64 kEvDteGuidePostionOffset = 10;
    static constexpr uint32_t kTimerInterval500ms = 500;
    static constexpr uint32_t kTimerInterval1s = 1000;
    static constexpr uint32_t kTimerInterval10s = 10000;
    static constexpr uint32_t kTimerInterval180s = 180000;
    static constexpr uint32_t klimitDTGOdometer = 0xFAFFFFFF;

    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerDrivingTimeTotal1sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mOneShotTimerIgnitionOff180sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAccumFuelEcoKmL10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAccumFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAccumHydrogenEco10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAccumVehicleSpeedKph10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationDriveFuelEcoKmL10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationDriveFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationDriveHydrogenEco10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationDriveInfoVehicleSpeedKph10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAfterRefuelFuelEcoKmL10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAfterRefuelFuelEcoKmKwh10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAfterRefuelHydrogenEco10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationAfterRefuelVehicleSpeedKph10sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationDistanceAndTimeDisplay1sStatus;
    ssfs::SFCTimer<Trip_Info_CV> mRepeatTimerCalculationInstantaneousFuelEco500msStatus;

    IgnitionState mIgnitionState = IgnitionState::OFF;
    IgnitionState mPrevIgnitionState = IgnitionState::OFF;
    ConfigVehicleType mInter_ConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mInter_ConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    ConfigArea mInter_ConfigArea = ConfigArea::DOM;
    TripDistanceUnit mInter_TripDistanceUnit = TripDistanceUnit::KM;
    FuelEcoUnitKmSetStatus mInter_FuelEcoUnitKmSetStatus = FuelEcoUnitKmSetStatus::KM_PER_LITTER;
    FuelEcoUnitMiSetStatus mInter_FuelEcoUnitMiSetStatus = FuelEcoUnitMiSetStatus::US_GALLON;
    std::chrono::time_point<std::chrono::steady_clock> mPrevOdometerRefreshTime;
    HBool mIsAemInitComplete = false;
    HDouble mPar_OdoCorrectionFactor = 1.0;
    Private_DTGInputStatus mPrivate_DTGInputStatus = Private_DTGInputStatus::INVALID;
    HDouble mPrivate_OdometerDisplayValue = 0.0;
    HDouble mPrevPrivate_OdometerDisplayValue = 0.0;
    HDouble mMemory_OdometerDisplayValue = 0.0;
    HDouble mPrivate_OdometerTotal = 0.0;
    HDouble mPrivate_AfterDriveFuelEcoValue = 0.0;
    HDouble mPrivate_ElecFuelConsumptionTotal = 0.0;
    HDouble mPrivate_HydrogenConsumptionTotal = 0.0;
    HUInt64 mPrivate_DrivingTimeTotal = 0;
    HDouble mPrivate_AccumStartDistance = 0.0;
    HUInt64 mPrivate_AccumStartTime = 0;
    HDouble mPrivate_AccumStartFuelCons = 0.0;
    HDouble mPrivate_AccumStartElecFuelCons = 0.0;
    HDouble mPrivate_AccumStartHydrogenCons = 0.0;
    HDouble mPrivate_DriveStartDistance = 0.0;
    HUInt64 mPrivate_DriveStartTime = 0;
    HDouble mPrivate_DriveStartFuelCons = 0.0;
    HDouble mPrivate_DriveStartElecFuelCons = 0.0;
    HDouble mPrivate_DriveStartHydrogenCons = 0.0;
    HDouble mPrivate_AfterRefuelStartDistance = 0.0;
    HDouble mPrevPrivate_AfterRefuelStartDistance = 0.0;
    HUInt64 mPrivate_AfterRefuelStartTime = 0;
    HDouble mPrivate_AfterRefuelStartFuelCons = 0.0;
    HDouble mPrivate_AfterRefuelStartElecFuelCons = 0.0;
    HDouble mPrivate_AfterRefuelStartHydrogenCons = 0.0;
    HDouble mInter_ActualVehicleSpeed = 0.0;
    HUInt64 mInput_DTGOdometer = 0xFFFFFFFF;
    HDouble mPrevInter_ActualVehicleSpeed = 0.0;
    HBool mIsValidPrevOdometerRefreshTime = false;
    HUInt64 mInput_FuelConsumptionValue = 0;
    HUInt64 mInput_FuelConsumption_FCU1 = 0;
    HUInt64 mInput_FuelConsumption_FCU2 = 0;
    HDouble mPrivate_FuelConsumptionTotal = 0.0;
    VehicleReadyStatus mInput_VehicleReadyStatus = VehicleReadyStatus::READY;
    HInt64 mInput_EvBatteryCurrentValue = 0;
    HUInt64 mInput_EvBatteryVoltageValue = 0;
    EngineOnStatus mInter_EngineOnStatus = EngineOnStatus::OFF;
    EngineOnStatus mPrevInter_EngineOnStatus = EngineOnStatus::OFF;
    IsgActiveStatus mInter_ISGActiveStatus = IsgActiveStatus::OFF;
    AdmStatus mPrivate_ADMStatus = AdmStatus::OFF;
    AdmStatus mPrevPrivate_ADMStatus = AdmStatus::OFF;
    ImuDrvDoorOpenStatus mInput_IMUDrvDoorOpenStatus = ImuDrvDoorOpenStatus::OFF;
    ImuFrDoorOpenStatus mInput_IMUFrDoorOpenStatus = ImuFrDoorOpenStatus::OFF;
    AccumInfoManualResetReq mInter_AccumInfoManualResetReq = AccumInfoManualResetReq::DEFAULT;
    AccumInfoManualResetReq mPrevInter_AccumInfoManualResetReq = AccumInfoManualResetReq::DEFAULT;
    DriveInfoManualResetReq mInter_DriveInfoManualResetReq = DriveInfoManualResetReq::DEFAULT;
    DriveInfoManualResetReq mPrevInter_DriveInfoManualResetReq = DriveInfoManualResetReq::DEFAULT;
    AfterRefuelInfoManualResetReq mInter_AfterRefuelInfoManualResetReq = AfterRefuelInfoManualResetReq::DEFAULT;
    AfterRefuelInfoManualResetReq mPrevInter_AfterRefuelInfoManualResetReq = AfterRefuelInfoManualResetReq::DEFAULT;
    FuelDetectRealtimeModeStatus mInter_FuelDetectRealtimeModeStatus = FuelDetectRealtimeModeStatus::OFF;
    BmsChgStatus mInput_BmsChgStatus = BmsChgStatus::OFF;
    BmsChgStatus mPrevInput_BmsChgStatus = BmsChgStatus::OFF;
    HUInt64 mInput_BmsDisplaySocValue = 0;
    HUInt64 mPrevInput_FcevSofDisplayValue = 0;
    HUInt64 mInput_FcevSofDisplayValue = 0;
    HInt64 mDiffInput_FcevSofDisplayValue = 0;
    HBool mIsDisplaySocValue10IncreasedStatus = false;
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
    HUInt64 mPrivate_DriveInfoVehicleSpeedKph = 0;
    HUInt64 mPrivate_DriveInfoVehicleSpeedMph = 0;
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
    HBool mIsAccumDisplay = false;
    HBool mIsDriveDisplay = false;
    HBool mIsAfterRefuelDisplay = false;
    HUInt64 mInter_DriveInfoTime = 0;
    HDouble mPrivate_AfterRefuelDistanceKm = 0.0;
    HDouble mPrivate_AfterRefuelDistanceMi = 0.0;
    HUInt64 mPrivate_AfterRefuelTime = 0;
    InstFuelEcoCalModeStatus mPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::OFF;
    InstFuelEcoCalModeStatus mPrevPrivate_InstFuelEcoCalModeStatus = InstFuelEcoCalModeStatus::OFF;
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
    HDouble mPrivate_InstHydrogenEcoKmKg = 0.0;
    HDouble mPrivate_InstFuelEcoL100km = 0.0;
    HDouble mPrivate_InstHydrogenEcoKg100km = 0.0;
    HDouble mPrivate_InstFuelEcoUsMPG = 0.0;
    HDouble mPrivate_InstHydrogenEcoMPG = 0.0;
    HDouble mPrivate_InstFuelEcoUkMPG = 0.0;
    HUInt64 mInput_EvInstFuelEcoValuePerKWh = 0;
    HDouble mPrivate_InstFuelEcoInputKmKwh = 0.0;
    HDouble mPrivate_InstFuelEcoInputKwh100km = 0.0;
    HDouble mPrivate_InstFuelEcoInputMiKwh = 0.0;
    HDouble mPrivate_AccumFuelEconomyValue = 0.0;
    HDouble mInter_DriveInfoFuelEcoValue = 0.0;
    HDouble mPrivate_AfterRefuelFuelEconomyValue = 0.0;
    HUInt64 mInput_EvDTEValuePerKm = 0;
    ElecDteStatus mInput_ElecDteStatus = ElecDteStatus::NONE;
    EvDteDisplayOffReqStatus mInput_EvDteDisplayOffReqStatus = EvDteDisplayOffReqStatus::DEFAULT;
    DriveInfoResetStatus mPrivate_DriveInfoResetStatus = DriveInfoResetStatus::OFF;
    DisplayLanguageStatus mInter_DisplayLanguageStatus = DisplayLanguageStatus::NONE;
    EvDteAvnCalcStatus mInput_EvDteAvnCalcStatus = EvDteAvnCalcStatus::OFF;
    EvDteGuidePageInfoStatus mInput_EvDteGuidePageInfoStatus = EvDteGuidePageInfoStatus::NONE;
    HUInt64 mInput_EvDteLowBoundValue = 0;
    HUInt64 mInput_EvDteUpperBoundValue = 0;
    HUInt64 mInput_EvDteGuideDtePositionValue = 0;
    HUInt64 mInput_EvDteGuideCoachPositionValue = 0;
};

}  // namespace ccos

#endif  // SFSS_Trip_Info_CV_H
