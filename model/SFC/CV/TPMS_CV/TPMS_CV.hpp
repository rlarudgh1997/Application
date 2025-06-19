/**
 * @file TPMS_CV.hpp
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
#ifndef SFSS_TPMS_CV_H
#define SFSS_TPMS_CV_H

#define DLOG_ENABLED gEnableSFCLog
#include <tuple>
#include "TPMS_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 23.0.0
// Spec Version : v0.35
// Reference : [CV270] TPMS_CV. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class TPMS_CV : public TPMS_CVBase {
public:
    TPMS_CV() = default;
    ~TPMS_CV() override = default;
    TPMS_CV(const TPMS_CV& other) = delete;
    TPMS_CV(TPMS_CV&& other) noexcept = delete;
    TPMS_CV& operator=(const TPMS_CV& other) = delete;
    TPMS_CV& operator=(TPMS_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTPMS_CVTelltaleTPMSBlinkValueA(SFCTPMS_CVTelltaleTPMSBlinkValueA::ON);
        setSFCTPMS_CVTelltaleTPMSBlinkValueB(SFCTPMS_CVTelltaleTPMSBlinkValueB::OFF);
        setSFCTPMS_CVEventTireLowPressureWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCTPMS_CVEventTireLowPressureWarnLinkedSoundType(SFCTPMS_CVEventTireLowPressureWarnLinkedSoundType::REPEAT_COUNT);
        setSFCTPMS_CVEventTireLowPressureWarnLinkedSoundRepeatCount(1);
        setSFCTPMS_CVEventTPMSMalfunctionWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCTPMS_CVEventTPMSMalfunctionWarnLinkedSoundType(SFCTPMS_CVEventTPMSMalfunctionWarnLinkedSoundType::REPEAT_COUNT);
        setSFCTPMS_CVEventTPMSMalfunctionWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAemInitComplete, args.mInter_AEMInitializeComplete) == true) {
            _DInfo() << __FUNCTION__ << " AEM Initialize Complete!!!";
            HUInt64 par_TPMSBlinkTimeValue = 0;
            HResult result = getCachedValue("SFC.Extension.Param.Par_TPMSBlinkTimeValue", par_TPMSBlinkTimeValue);
            if (result == HResult::OK) {
                mPar_TPMSBlinkTimeValue = par_TPMSBlinkTimeValue;
            }
            if (mPar_TPMSBlinkTimeValue != 0) {
                if (mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status.create(
                        (mPar_TPMSBlinkTimeValue * kTimerInterval1s), this,
                        &TPMS_CV::onOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status, false) == false) {
                    _DWarning() << __FUNCTION__ << " mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status Create Failed";
                }
                if (mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status.create(
                        (mPar_TPMSBlinkTimeValue * kTimerInterval1s), this,
                        &TPMS_CV::onOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status, false) == false) {
                    _DWarning() << __FUNCTION__ << " mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status Create Failed";
                }
            } else {
                Error() << "Failed to retrieve the Par_TPMSBlinkTimeValue, Therefore, the TPMS_CV timer could not be created.";
            }
        } else {
            _DInfo() << __FUNCTION__ << " AEM is Not Initialize";
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updatePrivateEventTrailerTPMSSignalMerge1Status();
        updatePrivateEventTrailerTPMSSignalMerge2Status();
        updateTelltale();
        updateConstantPressPage();
        updateConstantTempPage();
        updateConstantBatPage();
        updateConstantVehicleAxle();
        updateWholeTirePrivateAndConstant();  // 전체 IMG_TPMSPress/Temp/Bat~~_Status/Value 를 업데이트함
        updateEventTireLowPressureWarn();
        updateEventTPMSMalfunctionWarn();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updatePrivateEventTrailerTPMSSignalMerge1Status();
        updatePrivateEventTrailerTPMSSignalMerge2Status();
        updateTelltale();
        updateConstantPressPage();
        updateConstantTempPage();
        updateConstantBatPage();
        updateConstantVehicleAxle();
        updateWholeTirePrivateAndConstant();
        updateEventTireLowPressureWarn();
        updateEventTPMSMalfunctionWarn();
    }

    void onConfigTPMSCVChanged(const ArgumentsConfigTPMSCVChanged& args) {
        mConfigTPMSCV = args.mInter_ConfigTPMSCV;
        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updatePrivateEventTrailerTPMSSignalMerge1Status();
        updatePrivateEventTrailerTPMSSignalMerge2Status();
        updateTelltale();
        updateConstantPressPage();
        updateConstantTempPage();
        updateConstantBatPage();
        updateConstantVehicleAxle();
        updateWholeTirePrivateAndConstant();
        updateEventTireLowPressureWarn();
        updateEventTPMSMalfunctionWarn();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updatePrivateTempFactor();
        updateConstantVehicleAxle();
        updateWholeTirePrivateAndConstant();
        updateEventTireLowPressureWarn();
        updateEventTPMSMalfunctionWarn();
    }

    void onTpmsVehicleTypeChanged(const ArgumentsTpmsVehicleTypeChanged& args) {
        mTPMSVehicleType = args.mInput_TPMSVehicleType;
        updateConstantVehicleAxle();
    }

    void onTpmsWarningStatusChanged(const ArgumentsTpmsWarningStatusChanged& args) {
        mTPMSWarningStatus = args.mInput_TPMSWarningStatus;
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updateEventTPMSMalfunctionWarn();
    }

    void onTpmsLowPressWarnStatusChanged(const ArgumentsTpmsLowPressWarnStatusChanged& args) {
        mTPMSLowPressWarnStatus = args.mInput_TPMSLowPressWarnStatus;
        updatePrivateTelltaleTPMSSignalMergeStatus();
        updateEventTireLowPressureWarn();
    }

    void onTpmsHighTempWarnStatusChanged(const ArgumentsTpmsHighTempWarnStatusChanged& args) {
        mTPMSHighTempWarnStatus = args.mInput_TPMSHighTempWarnStatus;
        updatePrivateTelltaleTPMSSignalMergeStatus();
    }

    void onTireInfoChanged(const ArgumentsTireInfoChanged& args) {
        if (ISTIMEOUT(args.mInput_TPMSWheelLocation) == true) {
            mTPMSWheelLocation = TPMSWheelLocation::UNHANDLED_TIMEOUT;
        } else {
            mTPMSWheelLocation = args.mInput_TPMSWheelLocation;
        }

        // WheelLocation과 아래 6개 데이터가 페어로 1개의 콜백으로 들어온다는 전제 하에 만들어짐.
        mLowTirePressureStatus = args.mInput_LowTirePressureStatus;
        mHighTireTemperatureStatus = args.mInput_HighTireTemperatureStatus;
        mTirePressureValue = args.mInput_TirePressureValue;
        mTireTemperatureValue = args.mInput_TireTemperatureValue;
        mTPMSSensorBatterySOCValue = args.mInput_TPMSSensorBatterySOCValue;
        mTPMSLearnStatus = args.mInput_TPMSLearnStatus;

        updateSingleWheelInputPrivateAndConstantValue();
        updateConstantPressPage();
        updateConstantTempPage();
        updateConstantBatPage();
    }

    void onTrailerTPMSAndCANFailureStatusChanged(const ArgumentsTrailerTPMSAndCANFailureStatusChanged& args) {
        mTrailerTPMSStatus1 = args.mInput_TrailerTPMSStatus1;
        mTrailerTPMSStatus2 = args.mInput_TrailerTPMSStatus2;
        mTrailerTPMSStatus3 = args.mInput_TrailerTPMSStatus3;
        mTrailerTPMSStatus4 = args.mInput_TrailerTPMSStatus4;
        mTrailerTPMSStatus5 = args.mInput_TrailerTPMSStatus5;
        mTrailerCANFailure = args.mInput_TrailerCANFailure;

        updatePrivateTelltaleTrailerTPMSSignalMergeStatus();
        updatePrivateEventTrailerTPMSSignalMerge1Status();
        updatePrivateEventTrailerTPMSSignalMerge2Status();
    }

    void onInterPressureUnitStatusChanged(const ArgumentsInterPressureUnitStatusChanged& args) {
        mPressureUnitStatus = args.mInter_PressureUnit_stat;

        updatePrivatePressFactor();
        updateConstantPressPage();
    }

    void onOatUnitStatusFatcChanged(const ArgumentsOatUnitStatusFatcChanged& args) {
        mOATUnitStatusFATC = args.mInput_OATUnitStatus_FATC;

        updatePrivateTempFactor();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    enum class Private_Status : HUInt64 {
        NONE,
        OFF,
        VALID,
        MAX
    };

    enum class Private_TelltaleTrailerTPMSSignalMerge_Status : HUInt64 {
        NONE,
        OFF,
        ON,
        BLINK,
        MAX
    };

    enum class Private_TelltaleTrailerTPMSTimeControl_Status : HUInt64 {
        NONE,
        OFF,
        ON,
        BLINK,
        BLINK_TO_ON,
        MAX
    };

    enum class Private_TelltaleTPMSSignalMerge_Status : HUInt64 {
        NONE,
        OFF,
        ON,
        BLINK,
        MAX
    };

    enum class Private_TelltaleTPMSTimeControl_Status : HUInt64 {
        NONE,
        OFF,
        ON,
        BLINK,
        BLINK_TO_ON,
        MAX
    };

    enum class TrailerTPMSSignalMerge1Status : HUInt64 {
        NONE,
        OFF,
        LOW,
        MAX
    };

    enum class TrailerTPMSSignalMerge2Status : HUInt64 {
        NONE,
        OFF,
        FAULT,
        MAX
    };

    using ConfigTPMSCV = ArgumentsConfigTPMSCVChanged::Inter_ConfigTPMSCV;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using TPMSVehicleType = ArgumentsTpmsVehicleTypeChanged::Input_TPMSVehicleType;
    using TPMSWarningStatus = ArgumentsTpmsWarningStatusChanged::Input_TPMSWarningStatus;
    using TPMSLowPressWarnStatus = ArgumentsTpmsLowPressWarnStatusChanged::Input_TPMSLowPressWarnStatus;
    using TPMSHighTempWarnStatus = ArgumentsTpmsHighTempWarnStatusChanged::Input_TPMSHighTempWarnStatus;
    using TPMSWheelLocation = ArgumentsTireInfoChanged::Input_TPMSWheelLocation;
    using LowTirePressureStatus = ArgumentsTireInfoChanged::Input_LowTirePressureStatus;
    using HighTireTemperatureStatus = ArgumentsTireInfoChanged::Input_HighTireTemperatureStatus;
    using TPMSLearnStatus = ArgumentsTireInfoChanged::Input_TPMSLearnStatus;
    // Input - Pressure Value, Temperature Value, Battery Value, Pressure Status, Temperature Status, Learn Status
    using TireInputDataTuple =
        std::tuple<HUInt64, HUInt64, HUInt64, LowTirePressureStatus, HighTireTemperatureStatus, TPMSLearnStatus>;
    using TireInputDataTupleMap = std::map<TPMSWheelLocation, TireInputDataTuple&>;

    // uint64_t는 enum stat이고, 사용시 캐스팅하여 setSFC~Stat()에 사용
    // Output
    // Private Value : Pressure, Temperature, Battery
    // Private Valid Status for : Pressure Value, Temperature Value, Battery Value,
    // Pressure Status, Temperature Status, Battery Status
    using TirePrivateDataTuple =
        std::tuple<HDouble, HDouble, HUInt64, Private_Status, Private_Status, Private_Status, uint64_t, uint64_t, uint64_t>;
    using TirePrivateDataTupleMap = std::map<TPMSWheelLocation, TirePrivateDataTuple&>;

    using OutConstSetFuncMap = std::map<TPMSWheelLocation, std::function<void(TPMS_CV&, const TirePrivateDataTuple&)>>;

    using TirePresStat = SFCTPMS_CVConstantTirePressure1_ILStat;
    using TireTempStat = SFCTPMS_CVConstantTireTemperature1_ILStat;
    using TireBattStat = SFCTPMS_CVConstantTPMSBattery1_ILStat;
    using TrailerTPMSStatus1 = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerTPMSStatus1;
    using TrailerTPMSStatus2 = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerTPMSStatus2;
    using TrailerTPMSStatus3 = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerTPMSStatus3;
    using TrailerTPMSStatus4 = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerTPMSStatus4;
    using TrailerTPMSStatus5 = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerTPMSStatus5;
    using TrailerCANFailure = ArgumentsTrailerTPMSAndCANFailureStatusChanged::Input_TrailerCANFailure;
    using PressureUnitStatus = ArgumentsInterPressureUnitStatusChanged::Inter_PressureUnit_stat;
    using OATUnitStatusFATC = ArgumentsOatUnitStatusFatcChanged::Input_OATUnitStatus_FATC;

    static constexpr HDouble kPressFactorPSI = 0.29;
    static constexpr HDouble kPressFactorx100kPa = 0.02;
    static constexpr HDouble kTempFactorCelsius = 1.00;
    static constexpr HDouble kTempFactorFahrenheit = 1.80;
    static constexpr HUInt64 kPrivate_BatteryFactor = 10;

    static constexpr std::size_t kTupleColumnInputPressure = 0;
    static constexpr std::size_t kTupleColumnInputTemperature = 1;
    static constexpr std::size_t kTupleColumnInputBattery = 2;
    static constexpr std::size_t kTupleColumnInputPressureStatus = 3;
    static constexpr std::size_t kTupleColumnInputTemperatureStatus = 4;
    static constexpr std::size_t kTupleColumnInputLearnStatus = 5;

    static constexpr std::size_t kTupleColumnPrivatePressure = 0;
    static constexpr std::size_t kTupleColumnPrivateTemperature = 1;
    static constexpr std::size_t kTupleColumnPrivateBattery = 2;
    static constexpr std::size_t kTupleColumnPrivateValueStatusPressure = 3;
    static constexpr std::size_t kTupleColumnPrivateValueStatusTemperature = 4;
    static constexpr std::size_t kTupleColumnPrivateValueStatusBattery = 5;
    static constexpr std::size_t kTupleColumnPrivatePressureStatus = 6;
    static constexpr std::size_t kTupleColumnPrivateTemperatureStatus = 7;
    static constexpr std::size_t kTupleColumnPrivateBatteryStatus = 8;

    static constexpr uint32_t kTimerInterval1s = 1000;
    // 70초 타이머를 사용하기 위한 초기값 설정(Par_TPMSBlinkTimeValue * 1sec)
    HUInt64 mPar_TPMSBlinkTimeValue = 0;
    HBool mIsAemInitComplete = false;

    HBool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    ConfigTPMSCV mConfigTPMSCV = ConfigTPMSCV::ON;
    ConfigArea mConfigArea = ConfigArea::DOM;
    TPMSVehicleType mTPMSVehicleType = TPMSVehicleType::NOT_CONFIGURE;
    TPMSWarningStatus mTPMSWarningStatus = TPMSWarningStatus::OFF;
    TPMSLowPressWarnStatus mTPMSLowPressWarnStatus = TPMSLowPressWarnStatus::NORMAL;
    TPMSHighTempWarnStatus mTPMSHighTempWarnStatus = TPMSHighTempWarnStatus::NORMAL;
    TPMSWheelLocation mTPMSWheelLocation = TPMSWheelLocation::AXLE1_IL;
    LowTirePressureStatus mLowTirePressureStatus = LowTirePressureStatus::NORMAL;
    SFCTPMS_CVConstantVehicleAxleStat mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::OFF;
    HUInt64 mTirePressureValue = 0;
    HighTireTemperatureStatus mHighTireTemperatureStatus = HighTireTemperatureStatus::NORMAL;
    HUInt64 mTireTemperatureValue = 0;
    TPMSLearnStatus mTPMSLearnStatus = TPMSLearnStatus::UNLEARNED;
    HUInt64 mTPMSSensorBatterySOCValue = 0;

    HDouble mPrivate_PressFactor = kPressFactorPSI;    // 기본 지역이 DOM이므로 PSI로 설정
    HDouble mPrivate_TempFactor = kTempFactorCelsius;  // 기본 지역이 DOM이므로 섭씨로 설정

    TrailerTPMSStatus1 mTrailerTPMSStatus1 = TrailerTPMSStatus1::OFF;
    TrailerTPMSStatus2 mTrailerTPMSStatus2 = TrailerTPMSStatus2::OFF;
    TrailerTPMSStatus3 mTrailerTPMSStatus3 = TrailerTPMSStatus3::OFF;
    TrailerTPMSStatus4 mTrailerTPMSStatus4 = TrailerTPMSStatus4::OFF;
    TrailerTPMSStatus5 mTrailerTPMSStatus5 = TrailerTPMSStatus5::OFF;
    TrailerCANFailure mTrailerCANFailure = TrailerCANFailure::OFF;
    OATUnitStatusFATC mOATUnitStatusFATC = OATUnitStatusFATC::OFF;

    PressureUnitStatus mPressureUnitStatus = PressureUnitStatus::USM_OFF;

    Private_TelltaleTrailerTPMSSignalMerge_Status mPrivate_TelltaleTrailerTPMSSignalMerge_Status =
        Private_TelltaleTrailerTPMSSignalMerge_Status::OFF;
    Private_TelltaleTrailerTPMSTimeControl_Status mPrivate_TelltaleTrailerTPMSTimeControl_Status =
        Private_TelltaleTrailerTPMSTimeControl_Status::OFF;
    Private_TelltaleTPMSSignalMerge_Status mPrivate_TelltaleTPMSSignalMerge_Status = Private_TelltaleTPMSSignalMerge_Status::OFF;
    Private_TelltaleTPMSTimeControl_Status mPrivate_TelltaleTPMSTimeControl_Status = Private_TelltaleTPMSTimeControl_Status::OFF;
    ssfs::SFCTimer<TPMS_CV> mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status;
    ssfs::SFCTimer<TPMS_CV> mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status;

    TrailerTPMSSignalMerge1Status mPrivateEventTrailerTPMSSignalMerge1Status = TrailerTPMSSignalMerge1Status::OFF;
    TrailerTPMSSignalMerge2Status mPrivateEventTrailerTPMSSignalMerge2Status = TrailerTPMSSignalMerge2Status::OFF;

    // 콜백으로부터 들어온 Wheel별 TPMS의 압력,온도,배터리 상태/값을 저장하기 위한 개별 Tuple.
    // Map 에서 Wheel을 키로하여 매핑됨
    TireInputDataTuple mTireInputDataAxle_NULL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle1_IL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle1_IR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle2_OL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle2_IL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle2_IR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle2_OR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle3_OL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle3_IL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle3_IR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle3_OR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle4_OL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle4_IL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle4_IR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle4_OR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle5_OL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle5_IL =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle5_IR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);
    TireInputDataTuple mTireInputDataAxle5_OR =
        std::make_tuple(0, 0, 0, LowTirePressureStatus::NORMAL, HighTireTemperatureStatus::NORMAL, TPMSLearnStatus::UNLEARNED);

    // Pressure(HDouble)/Temperature(HDouble)/Battery(HUInt64) Factor 적용 전 Private 데이터
    // Private Pressure, Temperature, Battery 값의 유효상태 확인, 최종 Constant 노드 출력 전 0 출력여부 결정
    // Pressure/Temperature/Battery Stat(NORMAL(1))

    // 콜백으로부터 들어온 Wheel별 TPMS의 압력,온도,배터리 상태/값의 Private 값을 저장하기 위한 개별 Tuple.
    // Map 에서 Wheel을 키로하여 매핑됨
    TirePrivateDataTuple mTirePrivateDataAxle_NULL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 0, 0, 0);
    TirePrivateDataTuple mTirePrivateDataAxle1_IL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle1_IR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle2_OL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle2_IL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle2_IR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle2_OR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle3_OL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle3_IL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle3_IR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle3_OR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle4_OL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle4_IL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle4_IR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle4_OR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle5_OL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle5_IL =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle5_IR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);
    TirePrivateDataTuple mTirePrivateDataAxle5_OR =
        std::make_tuple(0.0, 0.0, 0, Private_Status::OFF, Private_Status::OFF, Private_Status::OFF, 1, 1, 1);

    // Wheel별 Constant update 함수를 매핑함.
    // Input이나 Private update가 되었을 경우 마지막에 이곳의 함수를 호출하여 wheel별 Constant 노드 업데이트를 진행하게 함.
    OutConstSetFuncMap mOutConstSetFuncMap = {{TPMSWheelLocation::AXLE1_IL, &TPMS_CV::updateConstantTireAXLE1_IL},
                                              {TPMSWheelLocation::AXLE1_IR, &TPMS_CV::updateConstantTireAXLE1_IR},
                                              {TPMSWheelLocation::AXLE2_OL, &TPMS_CV::updateConstantTireAXLE2_OL},
                                              {TPMSWheelLocation::AXLE2_IL, &TPMS_CV::updateConstantTireAXLE2_IL},
                                              {TPMSWheelLocation::AXLE2_IR, &TPMS_CV::updateConstantTireAXLE2_IR},
                                              {TPMSWheelLocation::AXLE2_OR, &TPMS_CV::updateConstantTireAXLE2_OR},
                                              {TPMSWheelLocation::AXLE3_OL, &TPMS_CV::updateConstantTireAXLE3_OL},
                                              {TPMSWheelLocation::AXLE3_IL, &TPMS_CV::updateConstantTireAXLE3_IL},
                                              {TPMSWheelLocation::AXLE3_IR, &TPMS_CV::updateConstantTireAXLE3_IR},
                                              {TPMSWheelLocation::AXLE3_OR, &TPMS_CV::updateConstantTireAXLE3_OR},
                                              {TPMSWheelLocation::AXLE4_OL, &TPMS_CV::updateConstantTireAXLE4_OL},
                                              {TPMSWheelLocation::AXLE4_IL, &TPMS_CV::updateConstantTireAXLE4_IL},
                                              {TPMSWheelLocation::AXLE4_IR, &TPMS_CV::updateConstantTireAXLE4_IR},
                                              {TPMSWheelLocation::AXLE4_OR, &TPMS_CV::updateConstantTireAXLE4_OR},
                                              {TPMSWheelLocation::AXLE5_OL, &TPMS_CV::updateConstantTireAXLE5_OL},
                                              {TPMSWheelLocation::AXLE5_IL, &TPMS_CV::updateConstantTireAXLE5_IL},
                                              {TPMSWheelLocation::AXLE5_IR, &TPMS_CV::updateConstantTireAXLE5_IR},
                                              {TPMSWheelLocation::AXLE5_OR, &TPMS_CV::updateConstantTireAXLE5_OR},
                                              {TPMSWheelLocation::NOT_AVAILABLE, &TPMS_CV::updateConstantTireAXLE_NULL},
                                              {TPMSWheelLocation::UNHANDLED_TIMEOUT, &TPMS_CV::updateConstantTireAXLE_NULL}};

    // // Wheel별 Private data의 Tuple을 매핑함
    TireInputDataTupleMap mTireInputDataTupleMap = {{TPMSWheelLocation::AXLE1_IL, mTireInputDataAxle1_IL},
                                                    {TPMSWheelLocation::AXLE1_IR, mTireInputDataAxle1_IR},
                                                    {TPMSWheelLocation::AXLE2_OL, mTireInputDataAxle2_OL},
                                                    {TPMSWheelLocation::AXLE2_IL, mTireInputDataAxle2_IL},
                                                    {TPMSWheelLocation::AXLE2_IR, mTireInputDataAxle2_IR},
                                                    {TPMSWheelLocation::AXLE2_OR, mTireInputDataAxle2_OR},
                                                    {TPMSWheelLocation::AXLE3_OL, mTireInputDataAxle3_OL},
                                                    {TPMSWheelLocation::AXLE3_IL, mTireInputDataAxle3_IL},
                                                    {TPMSWheelLocation::AXLE3_IR, mTireInputDataAxle3_IR},
                                                    {TPMSWheelLocation::AXLE3_OR, mTireInputDataAxle3_OR},
                                                    {TPMSWheelLocation::AXLE4_OL, mTireInputDataAxle4_OL},
                                                    {TPMSWheelLocation::AXLE4_IL, mTireInputDataAxle4_IL},
                                                    {TPMSWheelLocation::AXLE4_IR, mTireInputDataAxle4_IR},
                                                    {TPMSWheelLocation::AXLE4_OR, mTireInputDataAxle4_OR},
                                                    {TPMSWheelLocation::AXLE5_OL, mTireInputDataAxle5_OL},
                                                    {TPMSWheelLocation::AXLE5_IL, mTireInputDataAxle5_IL},
                                                    {TPMSWheelLocation::AXLE5_IR, mTireInputDataAxle5_IR},
                                                    {TPMSWheelLocation::AXLE5_OR, mTireInputDataAxle5_OR},
                                                    {TPMSWheelLocation::NOT_AVAILABLE, mTireInputDataAxle_NULL},
                                                    {TPMSWheelLocation::UNHANDLED_TIMEOUT, mTireInputDataAxle_NULL}};

    // Wheel별 Private data의 tuple을 매핑함
    TirePrivateDataTupleMap mTirePrivateDataTulpleMap = {{TPMSWheelLocation::AXLE1_IL, mTirePrivateDataAxle1_IL},
                                                         {TPMSWheelLocation::AXLE1_IR, mTirePrivateDataAxle1_IR},
                                                         {TPMSWheelLocation::AXLE2_OL, mTirePrivateDataAxle2_OL},
                                                         {TPMSWheelLocation::AXLE2_IL, mTirePrivateDataAxle2_IL},
                                                         {TPMSWheelLocation::AXLE2_IR, mTirePrivateDataAxle2_IR},
                                                         {TPMSWheelLocation::AXLE2_OR, mTirePrivateDataAxle2_OR},
                                                         {TPMSWheelLocation::AXLE3_OL, mTirePrivateDataAxle3_OL},
                                                         {TPMSWheelLocation::AXLE3_IL, mTirePrivateDataAxle3_IL},
                                                         {TPMSWheelLocation::AXLE3_IR, mTirePrivateDataAxle3_IR},
                                                         {TPMSWheelLocation::AXLE3_OR, mTirePrivateDataAxle3_OR},
                                                         {TPMSWheelLocation::AXLE4_OL, mTirePrivateDataAxle4_OL},
                                                         {TPMSWheelLocation::AXLE4_IL, mTirePrivateDataAxle4_IL},
                                                         {TPMSWheelLocation::AXLE4_IR, mTirePrivateDataAxle4_IR},
                                                         {TPMSWheelLocation::AXLE4_OR, mTirePrivateDataAxle4_OR},
                                                         {TPMSWheelLocation::AXLE5_OL, mTirePrivateDataAxle5_OL},
                                                         {TPMSWheelLocation::AXLE5_IL, mTirePrivateDataAxle5_IL},
                                                         {TPMSWheelLocation::AXLE5_IR, mTirePrivateDataAxle5_IR},
                                                         {TPMSWheelLocation::AXLE5_OR, mTirePrivateDataAxle5_OR},
                                                         {TPMSWheelLocation::NOT_AVAILABLE, mTirePrivateDataAxle_NULL},
                                                         {TPMSWheelLocation::UNHANDLED_TIMEOUT, mTirePrivateDataAxle_NULL}};

    // 4.1.1 압력 단위 Factor 정의
    void updatePrivatePressFactor() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        mPrivate_PressFactor = kPressFactorx100kPa;

        if (mPressureUnitStatus == PressureUnitStatus::PSI) {
            mPrivate_PressFactor = kPressFactorPSI;
        }
        updateSingleWheelInputPrivateAndConstantValue();
    }

    // 4.1.2 온도 단위 Factor 정의
    void updatePrivateTempFactor() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        mPrivate_TempFactor = kTempFactorCelsius;
        if (mOATUnitStatusFATC == OATUnitStatusFATC::FAHRENHEIT ||
            (mOATUnitStatusFATC == OATUnitStatusFATC::MESSAGE_TIMEOUT && mConfigArea == ConfigArea::USA)) {
            mPrivate_TempFactor = kTempFactorFahrenheit;
        }
        updateConstantTempPage();
        updateSingleWheelInputPrivateAndConstantValue();
    }

    void updateTelltale() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        SFCTPMS_CVTelltaleTPMSStat stat = SFCTPMS_CVTelltaleTPMSStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCTPMS_CVTelltaleTPMSStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mPrivate_TelltaleTPMSTimeControl_Status == Private_TelltaleTPMSTimeControl_Status::BLINK ||
                mPrivate_TelltaleTrailerTPMSTimeControl_Status == Private_TelltaleTrailerTPMSTimeControl_Status::BLINK) {
                stat = SFCTPMS_CVTelltaleTPMSStat::BLINK2;
            } else if (((mPrivate_TelltaleTrailerTPMSTimeControl_Status ==
                             Private_TelltaleTrailerTPMSTimeControl_Status::BLINK_TO_ON ||
                         mPrivate_TelltaleTrailerTPMSTimeControl_Status == Private_TelltaleTrailerTPMSTimeControl_Status::ON) &&
                        mPrivate_TelltaleTPMSTimeControl_Status != Private_TelltaleTPMSTimeControl_Status::BLINK) ||
                       (mPrivate_TelltaleTrailerTPMSTimeControl_Status != Private_TelltaleTrailerTPMSTimeControl_Status::BLINK &&
                        (mPrivate_TelltaleTPMSTimeControl_Status == Private_TelltaleTPMSTimeControl_Status::BLINK_TO_ON ||
                         mPrivate_TelltaleTPMSTimeControl_Status == Private_TelltaleTPMSTimeControl_Status::ON))) {
                stat = SFCTPMS_CVTelltaleTPMSStat::ON;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCTPMS_CVTelltaleTPMSStat(stat);
    }

    void updateConstantPressPage() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        SFCTPMS_CVConstantPressPageStat stat = SFCTPMS_CVConstantPressPageStat::OFF;
        if (mIsIgnOn == true && mTPMSWheelLocation != TPMSWheelLocation::UNHANDLED_TIMEOUT) {
            if (mPressureUnitStatus == PressureUnitStatus::PSI) {
                stat = SFCTPMS_CVConstantPressPageStat::PSI_ON;
            } else if (mPressureUnitStatus == PressureUnitStatus::KPA) {
                stat = SFCTPMS_CVConstantPressPageStat::KPA_ON;
            } else if (mPressureUnitStatus == PressureUnitStatus::BAR) {
                stat = SFCTPMS_CVConstantPressPageStat::BAR_ON;
            } else {
                // no operation
            }
        }
        setSFCTPMS_CVConstantPressPageStat(stat);
    }

    void updateConstantTempPage() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        SFCTPMS_CVConstantTempPageStat stat = SFCTPMS_CVConstantTempPageStat::OFF;
        if (mIsIgnOn == true && mTPMSWheelLocation != TPMSWheelLocation::UNHANDLED_TIMEOUT) {
            if (mPrivate_TempFactor == kTempFactorCelsius) {
                stat = SFCTPMS_CVConstantTempPageStat::CELSIUS_ON;
            } else {
                stat = SFCTPMS_CVConstantTempPageStat::FAHRENHEIT_ON;
            }
        }
        setSFCTPMS_CVConstantTempPageStat(stat);
    }

    void updateConstantBatPage() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        SFCTPMS_CVConstantBatPageStat stat = SFCTPMS_CVConstantBatPageStat::OFF;
        if (mIsIgnOn == true && mTPMSWheelLocation != TPMSWheelLocation::UNHANDLED_TIMEOUT) {
            stat = SFCTPMS_CVConstantBatPageStat::PERCENT_ON;
        }
        setSFCTPMS_CVConstantBatPageStat(stat);
    }

    void updateConstantVehicleAxle() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        if (mIsIgnOn == true) {
            // 시스템 기본값이 T4X2 이므로 설정

            if (mTPMSVehicleType == TPMSVehicleType::T4X2) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T4X2;
            } else if (mTPMSVehicleType == TPMSVehicleType::T6X2) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T6X2;
            } else if (mTPMSVehicleType == TPMSVehicleType::T6X2_TAG) {
                if (mConfigArea == ConfigArea::EEC) {
                    mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T6X2_TAG;
                } else {
                    mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T6X2_TAG_DOUBLE;
                }
            } else if (mTPMSVehicleType == TPMSVehicleType::T6X4) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T6X4;
            } else if (mTPMSVehicleType == TPMSVehicleType::T8X4) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T8X4;
            } else if (mTPMSVehicleType == TPMSVehicleType::T8X4_PUSHER) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T8X4_PUSHER;
            } else if (mTPMSVehicleType == TPMSVehicleType::T10X4_PUSHER) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T10X4_PUSHER;
            } else if (mTPMSVehicleType == TPMSVehicleType::T10X4_TAG) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::T10X4_TAG;
            } else if (mTPMSVehicleType == TPMSVehicleType::NOT_CONFIGURE || mTPMSVehicleType == TPMSVehicleType::NOT_AVAILABLE) {
                mVehicleAxleStatus = SFCTPMS_CVConstantVehicleAxleStat::OFF;
            } else {
                // no operation
            }
            setSFCTPMS_CVConstantVehicleAxleStat(mVehicleAxleStatus);
        }
    }

    // IMG_TPMSPress~~_Status/Value
    // IMG_TPMSTemp~~_Status/Value
    // IMG_TPMSBat~~_Status/Value
    // 는 Wheel별로 묶어서 처리함
    // 입력된 데이터를 Wheel별 Tuple로 받아서 저장 후 연산 진행
    void updateSingleWheelInputPrivateAndConstantValue() {
        HBool isTupleGet = false;
        TireInputDataTuple& inputDataTuple = getTupleByWheelLocation(mTireInputDataTupleMap, mTPMSWheelLocation, isTupleGet);
        if (isTupleGet == true) {
            setTireInputDataTuple(mTirePressureValue, mTireTemperatureValue, mTPMSSensorBatterySOCValue, mLowTirePressureStatus,
                                  mHighTireTemperatureStatus, mTPMSLearnStatus, inputDataTuple);
            TirePrivateDataTuple& privateDataTuple =  // wheel location 에 맞는 private value tuple을 가지고 오고,
                getTupleByWheelLocation(mTirePrivateDataTulpleMap, mTPMSWheelLocation, isTupleGet);
            if (isTupleGet == true) {  // 변수 재활용
                                       // 콜백에서의 input값을 이용하여 Private 값과 Constant 출력노드 값을 업데이트함.
                calTirePrivateTuple(mIsIgnOn, inputDataTuple, privateDataTuple);
                updateConstantSpecificTireValueSet(mOutConstSetFuncMap, mTPMSWheelLocation, privateDataTuple);
            }
        }
    }

    // 여기서부터 끝까지 Wheel별 압력,온도,배터리 상태/값 연산관련
    // 전체 Wheel에 대한 Private과 Constant 값 업데이트
    void updateWholeTirePrivateAndConstant() {
        for (auto it = mTireInputDataTupleMap.begin(); it != mTireInputDataTupleMap.end(); it++) {
            TPMSWheelLocation wheel = it->first;
            auto inputDataTuple = it->second;
            auto privateDataTuple = mTirePrivateDataTulpleMap.find(wheel)->second;
            calTirePrivateTuple(mIsIgnOn, inputDataTuple, privateDataTuple);  // private 값 업데이트
            if (mConfigTPMSCV != ConfigTPMSCV::ON) {  // OFF일 경우 constant 노드 업데이트 함수 호출 제한
                return;
            }
            auto func = mOutConstSetFuncMap.find(wheel)->second;
            func(*this, privateDataTuple);
        }
    }

    // Wheel별 Data Tuple을 Map에서 가져옴.
    // 없으면 UNHANDLED_TIMEOUT용으로 정의된 dummy data 리턴
    template <typename T1, typename T2>
    const T1& getTupleByWheelLocation(const std::map<T2, T1>& tupleMap, const T2& wheelLoc, HBool& result) {
        auto iter = tupleMap.find(wheelLoc);
        if (iter != tupleMap.end()) {
            if (wheelLoc != T2::UNHANDLED_TIMEOUT && wheelLoc != T2::NOT_AVAILABLE) {
                result = true;
            } else {
                result = false;
            }
            return iter->second;
        } else {  // LCOV_EXCL_START
            result = false;
            return tupleMap.find(T2::UNHANDLED_TIMEOUT)->second;
        }  // LCOV_EXCL_STOP
    }

    // 특정 wheel의 constant 노드 업데이트를 진행
    template <typename T1, typename T2, typename T3>
    void updateConstantSpecificTireValueSet(const T1& functionMap, const T2& wheelLoc, const T3& privateDataTuple) {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {  // constant 노드의 업데이트는 동작사양과 맞춤.
            return;
        }
        HBool res = false;
        auto funcUpdateConstant = getTupleByWheelLocation(functionMap, wheelLoc, res);
        if (res == true) {
            funcUpdateConstant(*this, privateDataTuple);
        }
    }

    // factor 적용 전 private값 업데이트
    // Private_TpmsPress~_value 는 IMG_TPMSPress~_value 사양에서
    // 정상적으로 TPMS가 동작하는 입력조건일 경우 유효한 값으로 판단, 각각의 Private_Status을 추가함
    // Press, Temp, Bat 상동
    // Private_Status는 마지막 set~Constant~Value() 하기 전에 0을 내보낼지, Factor를 적용하고 내보낼지를 판단하는데에 사용됨
    template <typename T1, typename T2>
    void calTirePrivateTuple(const HBool& isIgnOn, const T1& inputTuple, T2& privateDataTuple) {
        HDouble pressVal = 0.0;
        HDouble tempVal = 0.0;
        HUInt64 battVal = 0;
        Private_Status pressValValidStatus = Private_Status::OFF;
        Private_Status tempValValidStatus = Private_Status::OFF;
        Private_Status battValValidStatus = Private_Status::OFF;

        TirePresStat presStat = TirePresStat::NORMAL;
        TireTempStat tempStat = TireTempStat::NORMAL;
        TireBattStat battStat = TireBattStat::NORMAL;

        // private값 계산
        calPrivateTPMSPressValue(isIgnOn, std::get<kTupleColumnInputLearnStatus>(inputTuple),
                                 std::get<kTupleColumnInputPressure>(inputTuple), pressVal, pressValValidStatus);
        calPrivateTPMSTempValue(isIgnOn, std::get<kTupleColumnInputLearnStatus>(inputTuple),
                                std::get<kTupleColumnInputTemperature>(inputTuple), tempVal, tempValValidStatus);
        calPrivateTPMSBattValue(isIgnOn, std::get<kTupleColumnInputLearnStatus>(inputTuple),
                                std::get<kTupleColumnInputBattery>(inputTuple), battVal, battValValidStatus);

        calTPMSPressStatus(isIgnOn, std::get<kTupleColumnInputPressureStatus>(inputTuple), presStat);
        calTPMSTempStatus(isIgnOn, std::get<kTupleColumnInputTemperatureStatus>(inputTuple), tempStat);
        calTPMSBattStatus(isIgnOn, std::get<kTupleColumnInputBattery>(inputTuple), battStat);

        // private값 tuple 저장
        std::get<kTupleColumnPrivatePressure>(privateDataTuple) = pressVal;
        std::get<kTupleColumnPrivateTemperature>(privateDataTuple) = tempVal;
        std::get<kTupleColumnPrivateBattery>(privateDataTuple) = battVal;
        std::get<kTupleColumnPrivateValueStatusPressure>(privateDataTuple) = pressValValidStatus;
        std::get<kTupleColumnPrivateValueStatusTemperature>(privateDataTuple) = tempValValidStatus;
        std::get<kTupleColumnPrivateValueStatusBattery>(privateDataTuple) = battValValidStatus;
        std::get<kTupleColumnPrivatePressureStatus>(privateDataTuple) = static_cast<uint64_t>(presStat);
        std::get<kTupleColumnPrivateTemperatureStatus>(privateDataTuple) = static_cast<uint64_t>(tempStat);
        std::get<kTupleColumnPrivateBatteryStatus>(privateDataTuple) = static_cast<uint64_t>(battStat);
    }

    // 정상적으로 TPMS가 동작하는 입력조건일 경우 유효한 값으로 판단, 각각의 Private_Status을 추가함
    // Press, Temp, Bat 상동
    // 지역변경시 factor를 변경할 수 있도록 factor 곱하는 부분 분리
    template <typename T1, typename T2>
    void calPrivateTPMSPressValue(const HBool& isIgnOn, const T1& learnStat, const HUInt64& pressVal, HDouble& targetValue,
                                  T2& valueStatus) {
        if (isIgnOn == true) {
            if (learnStat == T1::LEARNED && 0x000 <= pressVal && pressVal <= 0x3FA) {
                targetValue = static_cast<HDouble>(pressVal);
                valueStatus = T2::VALID;
            }
        }
    }

    // 지역변경시 factor를 변경할 수 있도록 분리
    template <typename T1, typename T2>
    void calPrivateTPMSTempValue(const HBool& isIgnOn, const T1& learnStat, const HUInt64& tempVal, HDouble& targetValue,
                                 T2& valueStatus) {
        if (isIgnOn == true) {
            if (learnStat == T1::LEARNED && 0x00 <= tempVal && tempVal <= 0xFA) {
                targetValue = static_cast<HDouble>(tempVal);
                valueStatus = T2::VALID;
            }
        }
    }

    template <typename T1, typename T2>
    void calPrivateTPMSBattValue(const HBool& isIgnOn, const T1& learnStat, const HUInt64& battVal, HUInt64& targetValue,
                                 T2& valueStatus) {
        if (isIgnOn == true) {
            if (learnStat == T1::LEARNED) {
                if (0x0 <= battVal && battVal <= 0xA) {  // 0~10
                    targetValue = static_cast<HDouble>(battVal);
                    valueStatus = T2::VALID;
                } else if (0xB <= battVal && battVal <= 0xD) {
                    targetValue = 10;  // 10 곱해서 100으로 나감
                    valueStatus = T2::VALID;
                } else {
                    // do nothing
                }
            }
        }
    }

    // IMG_TPMSPress~_Status
    template <typename T1, typename T2>
    void calTPMSPressStatus(const bool isIgnOn, const T1& pressStat, T2& targetStat) {
        if (isIgnOn == false || pressStat == LowTirePressureStatus::NORMAL) {
            targetStat = T2::NORMAL;
        } else if (pressStat == LowTirePressureStatus::LOW) {
            targetStat = T2::LOW_PRESS_WARN;
        } else {
            // do nothing
        }
    }

    // IMG_TPMSTemp~_Status
    template <typename T1, typename T2>
    void calTPMSTempStatus(const bool isIgnOn, const T1& tempStat, T2& targetStat) {
        if (isIgnOn == false || tempStat == T1::NORMAL) {
            targetStat = T2::NORMAL;
        } else if (tempStat == T1::HIGH) {
            targetStat = T2::HIGH_TEMP_WARN;
        } else {
            // do nothing
        }
    }

    // IMG_TPMSBat~_Status
    template <typename T1>
    void calTPMSBattStatus(const bool isIgnOn, const HUInt64& battVal, T1& targetStat) {
        if (isIgnOn == false || battVal >= 0x3) {
            targetStat = T1::NORMAL;
        } else if (battVal >= 0x0 && battVal <= 0x2) {
            targetStat = T1::LOW_BAT_WARN;
        } else {
            // do nothing
        }
    }

    inline HDouble ceilAt3DecimalPlace(const HDouble& value) const {
        HUInt64 truncatedVal = static_cast<HUInt64>(floor(value * std::pow(10, 3)));
        return ceil(static_cast<HDouble>(truncatedVal) / 10.0) / std::pow(10, 2);
    }

    // IMG_TPMSPress_value, Temp, Bat 에서 유효한 값일 때에만 Factor를 포함한 추가연산 수행
    // 아닐 경우 0을 출력
    // Private 변경, 출력조건(vaild 등) 변경, Factor 변경시마다 업데이트가 가능하게 됨
    template <typename T>
    void calConstantOutputValue(const T& tuple, HDouble& targetPressureValue, HDouble& targetTemperatureValue,
                                HUInt64& targetBatteryValue) {
        if (std::get<kTupleColumnPrivateValueStatusPressure>(tuple) == Private_Status::VALID) {
            targetPressureValue = ceilAt3DecimalPlace(std::get<kTupleColumnPrivatePressure>(tuple) * mPrivate_PressFactor);
        } else {
            targetPressureValue = 0xFFF;
        }
        if (std::get<kTupleColumnPrivateValueStatusTemperature>(tuple) == Private_Status::VALID) {
            targetTemperatureValue = floor(std::get<kTupleColumnPrivateTemperature>(tuple) * mPrivate_TempFactor - 40.0);
        } else {
            targetTemperatureValue = 0xFFF;
        }
        if (std::get<kTupleColumnPrivateValueStatusBattery>(tuple) == Private_Status::VALID) {
            targetBatteryValue = std::get<kTupleColumnPrivateBattery>(tuple) * kPrivate_BatteryFactor;  // 10
        } else {
            targetBatteryValue = 0xFF;
        }
    }

    // wheel 정보와 함께 들어온 TPMS데이터들을 미리 만들어둔 1개 Tuple 에 저장해둠
    template <typename T1, typename T2, typename T3, typename T4>
    void setTireInputDataTuple(const HUInt64& pressVal, const HUInt64& tempVal, const HUInt64& battVal, const T1& pressStat,
                               const T2& tempStat, const T3& learnStat, T4& retTuple) {
        std::get<kTupleColumnInputPressure>(retTuple) = pressVal;
        std::get<kTupleColumnInputTemperature>(retTuple) = tempVal;
        std::get<kTupleColumnInputBattery>(retTuple) = battVal;
        std::get<kTupleColumnInputPressureStatus>(retTuple) = pressStat;
        std::get<kTupleColumnInputTemperatureStatus>(retTuple) = tempStat;
        std::get<kTupleColumnInputLearnStatus>(retTuple) = learnStat;
    }

    // 아래 함수들은 Wheel 별로 데이터를 최종 출력하기 위한 Constant 값 연산과 노드 set 함수를 포함함.
    void updateConstantTireAXLE1_IL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        // privateData Tuple을 전달받으면 이 안에서 유효할 때 연산, 아닐경우 0 을 출력하도록 값을 설정함.
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure1_ILValue(pressVal);
        setSFCTPMS_CVConstantTirePressure1_ILStat(static_cast<SFCTPMS_CVConstantTirePressure1_ILStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature1_ILValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature1_ILStat(static_cast<SFCTPMS_CVConstantTireTemperature1_ILStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery1_ILValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery1_ILStat(static_cast<SFCTPMS_CVConstantTPMSBattery1_ILStat>(battStat));
    };

    void updateConstantTireAXLE1_IR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure1_IRValue(pressVal);
        setSFCTPMS_CVConstantTirePressure1_IRStat(static_cast<SFCTPMS_CVConstantTirePressure1_IRStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature1_IRValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature1_IRStat(static_cast<SFCTPMS_CVConstantTireTemperature1_IRStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery1_IRValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery1_IRStat(static_cast<SFCTPMS_CVConstantTPMSBattery1_IRStat>(battStat));
    };

    void updateConstantTireAXLE2_OL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure2_OLValue(pressVal);
        setSFCTPMS_CVConstantTirePressure2_OLStat(static_cast<SFCTPMS_CVConstantTirePressure2_OLStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature2_OLValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature2_OLStat(static_cast<SFCTPMS_CVConstantTireTemperature2_OLStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery2_OLValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery2_OLStat(static_cast<SFCTPMS_CVConstantTPMSBattery2_OLStat>(battStat));
    };

    void updateConstantTireAXLE2_IL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure2_ILValue(pressVal);
        setSFCTPMS_CVConstantTirePressure2_ILStat(static_cast<SFCTPMS_CVConstantTirePressure2_ILStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature2_ILValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature2_ILStat(static_cast<SFCTPMS_CVConstantTireTemperature2_ILStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery2_ILValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery2_ILStat(static_cast<SFCTPMS_CVConstantTPMSBattery2_ILStat>(battStat));
    };

    void updateConstantTireAXLE2_IR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure2_IRValue(pressVal);
        setSFCTPMS_CVConstantTirePressure2_IRStat(static_cast<SFCTPMS_CVConstantTirePressure2_IRStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature2_IRValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature2_IRStat(static_cast<SFCTPMS_CVConstantTireTemperature2_IRStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery2_IRValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery2_IRStat(static_cast<SFCTPMS_CVConstantTPMSBattery2_IRStat>(battStat));
    };

    void updateConstantTireAXLE2_OR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure2_ORValue(pressVal);
        setSFCTPMS_CVConstantTirePressure2_ORStat(static_cast<SFCTPMS_CVConstantTirePressure2_ORStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature2_ORValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature2_ORStat(static_cast<SFCTPMS_CVConstantTireTemperature2_ORStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery2_ORValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery2_ORStat(static_cast<SFCTPMS_CVConstantTPMSBattery2_ORStat>(battStat));
    };

    void updateConstantTireAXLE3_OL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure3_OLValue(pressVal);
        setSFCTPMS_CVConstantTirePressure3_OLStat(static_cast<SFCTPMS_CVConstantTirePressure3_OLStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature3_OLValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature3_OLStat(static_cast<SFCTPMS_CVConstantTireTemperature3_OLStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery3_OLValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery3_OLStat(static_cast<SFCTPMS_CVConstantTPMSBattery3_OLStat>(battStat));
    };

    void updateConstantTireAXLE3_IL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure3_ILValue(pressVal);
        setSFCTPMS_CVConstantTirePressure3_ILStat(static_cast<SFCTPMS_CVConstantTirePressure3_ILStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature3_ILValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature3_ILStat(static_cast<SFCTPMS_CVConstantTireTemperature3_ILStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery3_ILValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery3_ILStat(static_cast<SFCTPMS_CVConstantTPMSBattery3_ILStat>(battStat));
    };

    void updateConstantTireAXLE3_IR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure3_IRValue(pressVal);
        setSFCTPMS_CVConstantTirePressure3_IRStat(static_cast<SFCTPMS_CVConstantTirePressure3_IRStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature3_IRValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature3_IRStat(static_cast<SFCTPMS_CVConstantTireTemperature3_IRStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery3_IRValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery3_IRStat(static_cast<SFCTPMS_CVConstantTPMSBattery3_IRStat>(battStat));
    };

    void updateConstantTireAXLE3_OR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure3_ORValue(pressVal);
        setSFCTPMS_CVConstantTirePressure3_ORStat(static_cast<SFCTPMS_CVConstantTirePressure3_ORStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature3_ORValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature3_ORStat(static_cast<SFCTPMS_CVConstantTireTemperature3_ORStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery3_ORValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery3_ORStat(static_cast<SFCTPMS_CVConstantTPMSBattery3_ORStat>(battStat));
    };

    void updateConstantTireAXLE4_OL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure4_OLValue(pressVal);
        setSFCTPMS_CVConstantTirePressure4_OLStat(static_cast<SFCTPMS_CVConstantTirePressure4_OLStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature4_OLValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature4_OLStat(static_cast<SFCTPMS_CVConstantTireTemperature4_OLStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery4_OLValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery4_OLStat(static_cast<SFCTPMS_CVConstantTPMSBattery4_OLStat>(battStat));
    };

    void updateConstantTireAXLE4_IL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure4_ILValue(pressVal);
        setSFCTPMS_CVConstantTirePressure4_ILStat(static_cast<SFCTPMS_CVConstantTirePressure4_ILStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature4_ILValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature4_ILStat(static_cast<SFCTPMS_CVConstantTireTemperature4_ILStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery4_ILValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery4_ILStat(static_cast<SFCTPMS_CVConstantTPMSBattery4_ILStat>(battStat));
    };

    void updateConstantTireAXLE4_IR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure4_IRValue(pressVal);
        setSFCTPMS_CVConstantTirePressure4_IRStat(static_cast<SFCTPMS_CVConstantTirePressure4_IRStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature4_IRValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature4_IRStat(static_cast<SFCTPMS_CVConstantTireTemperature4_IRStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery4_IRValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery4_IRStat(static_cast<SFCTPMS_CVConstantTPMSBattery4_IRStat>(battStat));
    };

    void updateConstantTireAXLE4_OR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure4_ORValue(pressVal);
        setSFCTPMS_CVConstantTirePressure4_ORStat(static_cast<SFCTPMS_CVConstantTirePressure4_ORStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature4_ORValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature4_ORStat(static_cast<SFCTPMS_CVConstantTireTemperature4_ORStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery4_ORValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery4_ORStat(static_cast<SFCTPMS_CVConstantTPMSBattery4_ORStat>(battStat));
    };

    void updateConstantTireAXLE5_OL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure5_OLValue(pressVal);
        setSFCTPMS_CVConstantTirePressure5_OLStat(static_cast<SFCTPMS_CVConstantTirePressure5_OLStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature5_OLValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature5_OLStat(static_cast<SFCTPMS_CVConstantTireTemperature5_OLStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery5_OLValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery5_OLStat(static_cast<SFCTPMS_CVConstantTPMSBattery5_OLStat>(battStat));
    };

    void updateConstantTireAXLE5_IL(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure5_ILValue(pressVal);
        setSFCTPMS_CVConstantTirePressure5_ILStat(static_cast<SFCTPMS_CVConstantTirePressure5_ILStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature5_ILValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature5_ILStat(static_cast<SFCTPMS_CVConstantTireTemperature5_ILStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery5_ILValue(static_cast<uint64_t>(battVal));
        setSFCTPMS_CVConstantTPMSBattery5_ILStat(static_cast<SFCTPMS_CVConstantTPMSBattery5_ILStat>(battStat));
    };

    void updateConstantTireAXLE5_IR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure5_IRValue(pressVal);
        setSFCTPMS_CVConstantTirePressure5_IRStat(static_cast<SFCTPMS_CVConstantTirePressure5_IRStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature5_IRValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature5_IRStat(static_cast<SFCTPMS_CVConstantTireTemperature5_IRStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery5_IRValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery5_IRStat(static_cast<SFCTPMS_CVConstantTPMSBattery5_IRStat>(battStat));
    };

    void updateConstantTireAXLE5_OR(const TirePrivateDataTuple& tuple) {
        HDouble pressVal = std::get<kTupleColumnPrivatePressure>(tuple);
        HDouble tempVal = std::get<kTupleColumnPrivateTemperature>(tuple);
        HUInt64 battVal = std::get<kTupleColumnPrivateBattery>(tuple);
        uint64_t pressStat = std::get<kTupleColumnPrivatePressureStatus>(tuple);
        uint64_t tempStat = std::get<kTupleColumnPrivateTemperatureStatus>(tuple);
        uint64_t battStat = std::get<kTupleColumnPrivateBatteryStatus>(tuple);
        calConstantOutputValue(tuple, pressVal, tempVal, battVal);
        setSFCTPMS_CVConstantTirePressure5_ORValue(pressVal);
        setSFCTPMS_CVConstantTirePressure5_ORStat(static_cast<SFCTPMS_CVConstantTirePressure5_ORStat>(pressStat));
        setSFCTPMS_CVConstantTireTemperature5_ORValue(static_cast<HInt64>(tempVal));
        setSFCTPMS_CVConstantTireTemperature5_ORStat(static_cast<SFCTPMS_CVConstantTireTemperature5_ORStat>(tempStat));
        setSFCTPMS_CVConstantTPMSBattery5_ORValue(battVal);
        setSFCTPMS_CVConstantTPMSBattery5_ORStat(static_cast<SFCTPMS_CVConstantTPMSBattery5_ORStat>(battStat));
    };

    // 4.3.1 Trailer TPMS 경고등 신호 통합
    void updatePrivateTelltaleTrailerTPMSSignalMergeStatus() {
        mPrivate_TelltaleTrailerTPMSSignalMerge_Status = Private_TelltaleTrailerTPMSSignalMerge_Status::OFF;
        if (mConfigTPMSCV == ConfigTPMSCV::ON && mIsIgnOn == true) {
            if (mTrailerCANFailure != TrailerCANFailure::ON &&
                ((mTrailerTPMSStatus1 == TrailerTPMSStatus1::ON && mTrailerTPMSStatus2 != TrailerTPMSStatus2::BLINK &&
                  mTrailerTPMSStatus3 != TrailerTPMSStatus3::BLINK && mTrailerTPMSStatus4 != TrailerTPMSStatus4::BLINK &&
                  mTrailerTPMSStatus5 != TrailerTPMSStatus5::BLINK) ||
                 (mTrailerTPMSStatus1 != TrailerTPMSStatus1::BLINK && mTrailerTPMSStatus2 == TrailerTPMSStatus2::ON &&
                  mTrailerTPMSStatus3 != TrailerTPMSStatus3::BLINK && mTrailerTPMSStatus4 != TrailerTPMSStatus4::BLINK &&
                  mTrailerTPMSStatus5 != TrailerTPMSStatus5::BLINK) ||
                 (mTrailerTPMSStatus1 != TrailerTPMSStatus1::BLINK && mTrailerTPMSStatus2 != TrailerTPMSStatus2::BLINK &&
                  mTrailerTPMSStatus3 == TrailerTPMSStatus3::ON && mTrailerTPMSStatus4 != TrailerTPMSStatus4::BLINK &&
                  mTrailerTPMSStatus5 != TrailerTPMSStatus5::BLINK) ||
                 (mTrailerTPMSStatus1 != TrailerTPMSStatus1::BLINK && mTrailerTPMSStatus2 != TrailerTPMSStatus2::BLINK &&
                  mTrailerTPMSStatus3 != TrailerTPMSStatus3::BLINK && mTrailerTPMSStatus4 == TrailerTPMSStatus4::ON &&
                  mTrailerTPMSStatus5 != TrailerTPMSStatus5::BLINK) ||
                 (mTrailerTPMSStatus1 != TrailerTPMSStatus1::BLINK && mTrailerTPMSStatus2 != TrailerTPMSStatus2::BLINK &&
                  mTrailerTPMSStatus3 != TrailerTPMSStatus3::BLINK && mTrailerTPMSStatus4 != TrailerTPMSStatus4::BLINK &&
                  mTrailerTPMSStatus5 == TrailerTPMSStatus5::ON))) {
                mPrivate_TelltaleTrailerTPMSSignalMerge_Status = Private_TelltaleTrailerTPMSSignalMerge_Status::ON;
            } else if (mTrailerTPMSStatus1 == TrailerTPMSStatus1::BLINK || mTrailerTPMSStatus2 == TrailerTPMSStatus2::BLINK ||
                       mTrailerTPMSStatus3 == TrailerTPMSStatus3::BLINK || mTrailerTPMSStatus4 == TrailerTPMSStatus4::BLINK ||
                       mTrailerTPMSStatus5 == TrailerTPMSStatus5::BLINK || mTrailerCANFailure == TrailerCANFailure::ON) {
                mPrivate_TelltaleTrailerTPMSSignalMerge_Status = Private_TelltaleTrailerTPMSSignalMerge_Status::BLINK;
            } else {
                // no operation
            }
        }
        updatePrivateTelltaleTrailerTPMSTimeControlStatus();
    }

    // 4.3.2 차량 TPMS 경고등 신호 통합
    void updatePrivateTelltaleTPMSSignalMergeStatus() {
        mPrivate_TelltaleTPMSSignalMerge_Status = Private_TelltaleTPMSSignalMerge_Status::OFF;

        if (mConfigTPMSCV == ConfigTPMSCV::ON && mIsIgnOn == true) {
            if (mTPMSWarningStatus == TPMSWarningStatus::OFF &&
                ((mTPMSLowPressWarnStatus == TPMSLowPressWarnStatus::LOW &&
                  mTPMSHighTempWarnStatus != TPMSHighTempWarnStatus::MESSAGE_TIMEOUT) ||
                 (mTPMSHighTempWarnStatus == TPMSHighTempWarnStatus::HIGH &&
                  mTPMSLowPressWarnStatus != TPMSLowPressWarnStatus::MESSAGE_TIMEOUT))) {
                mPrivate_TelltaleTPMSSignalMerge_Status = Private_TelltaleTPMSSignalMerge_Status::ON;
            } else if (mTPMSWarningStatus == TPMSWarningStatus::BLINK2 ||
                       mTPMSWarningStatus == TPMSWarningStatus::MESSAGE_TIMEOUT ||
                       mTPMSLowPressWarnStatus == TPMSLowPressWarnStatus::MESSAGE_TIMEOUT ||
                       mTPMSHighTempWarnStatus == TPMSHighTempWarnStatus::MESSAGE_TIMEOUT) {
                mPrivate_TelltaleTPMSSignalMerge_Status = Private_TelltaleTPMSSignalMerge_Status::BLINK;
            } else {
                // no operation
            }
        }
        updatePrivateTelltaleTPMSTimeControlStatus();
    }

    // 4.3.3 Trailer TPMS 경고등 시간 제어 Timer callback
    void onOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status() {
        if (mPrivate_TelltaleTrailerTPMSSignalMerge_Status == Private_TelltaleTrailerTPMSSignalMerge_Status::BLINK) {
            mPrivate_TelltaleTrailerTPMSTimeControl_Status = Private_TelltaleTrailerTPMSTimeControl_Status::BLINK_TO_ON;
            updateTelltale();
            flush();
        }
    }

    // 4.3.3 Trailer TPMS 경고등 시간 제어
    void updatePrivateTelltaleTrailerTPMSTimeControlStatus() {
        if (mPrivate_TelltaleTrailerTPMSSignalMerge_Status == Private_TelltaleTrailerTPMSSignalMerge_Status::ON) {
            mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status.stop();
            mPrivate_TelltaleTrailerTPMSTimeControl_Status = Private_TelltaleTrailerTPMSTimeControl_Status::ON;
        } else if (mPrivate_TelltaleTrailerTPMSSignalMerge_Status == Private_TelltaleTrailerTPMSSignalMerge_Status::BLINK) {
            mPrivate_TelltaleTrailerTPMSTimeControl_Status = Private_TelltaleTrailerTPMSTimeControl_Status::BLINK;
            if (mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status.restart() == false) {
                _DWarning() << __FUNCTION__ << " mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status Start Failed";
            }
        } else {
            mOneShotTimer_mPrivate_TelltaleTrailerTPMSSignalMerge_Status.stop();
            mPrivate_TelltaleTrailerTPMSTimeControl_Status = Private_TelltaleTrailerTPMSTimeControl_Status::OFF;
        }
        updateTelltale();
    }

    // 4.3.4 차량 TPMS 경고등 시간 제어 Timer callback
    void onOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status() {
        if (mPrivate_TelltaleTPMSSignalMerge_Status == Private_TelltaleTPMSSignalMerge_Status::BLINK) {
            mPrivate_TelltaleTPMSTimeControl_Status = Private_TelltaleTPMSTimeControl_Status::BLINK_TO_ON;
            updateTelltale();
            flush();
        }
    }

    // 4.3.4 차량 TPMS 경고등 시간 제어
    void updatePrivateTelltaleTPMSTimeControlStatus() {
        if (mPrivate_TelltaleTPMSSignalMerge_Status == Private_TelltaleTPMSSignalMerge_Status::ON) {
            mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status.stop();
            mPrivate_TelltaleTPMSTimeControl_Status = Private_TelltaleTPMSTimeControl_Status::ON;
        } else if (mPrivate_TelltaleTPMSSignalMerge_Status == Private_TelltaleTPMSSignalMerge_Status::BLINK) {
            mPrivate_TelltaleTPMSTimeControl_Status = Private_TelltaleTPMSTimeControl_Status::BLINK;
            if (mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status.restart() == false) {
                _DWarning() << __FUNCTION__ << " mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status Start Failed";
            }
        } else {
            mOneShotTimer_mPrivate_TelltaleTPMSSignalMerge_Status.stop();
            mPrivate_TelltaleTPMSTimeControl_Status = Private_TelltaleTPMSTimeControl_Status::OFF;
        }
        updateTelltale();
    }

    // 4.3.5.1 저압 신호 통합
    void updatePrivateEventTrailerTPMSSignalMerge1Status() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        mPrivateEventTrailerTPMSSignalMerge1Status = TrailerTPMSSignalMerge1Status::OFF;
        if (mIsIgnOn == true && (mTrailerTPMSStatus1 == TrailerTPMSStatus1::ON || mTrailerTPMSStatus2 == TrailerTPMSStatus2::ON ||
                                 mTrailerTPMSStatus3 == TrailerTPMSStatus3::ON || mTrailerTPMSStatus4 == TrailerTPMSStatus4::ON ||
                                 mTrailerTPMSStatus5 == TrailerTPMSStatus5::ON)) {
            mPrivateEventTrailerTPMSSignalMerge1Status = TrailerTPMSSignalMerge1Status::LOW;
        }
        updateEventTireLowPressureWarn();
    }

    // 4.3.5.2 고장 신호 통합
    void updatePrivateEventTrailerTPMSSignalMerge2Status() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        mPrivateEventTrailerTPMSSignalMerge2Status = TrailerTPMSSignalMerge2Status::OFF;
        if (mIsIgnOn == true &&
            (mTrailerTPMSStatus1 == TrailerTPMSStatus1::BLINK || mTrailerTPMSStatus2 == TrailerTPMSStatus2::BLINK ||
             mTrailerTPMSStatus3 == TrailerTPMSStatus3::BLINK || mTrailerTPMSStatus4 == TrailerTPMSStatus4::BLINK ||
             mTrailerTPMSStatus5 == TrailerTPMSStatus5::BLINK || mTrailerCANFailure == TrailerCANFailure::ON)) {
            mPrivateEventTrailerTPMSSignalMerge2Status = TrailerTPMSSignalMerge2Status::FAULT;
        }
        updateEventTPMSMalfunctionWarn();
    }

    // 5.3.1 타이어 저압 경고
    void updateEventTireLowPressureWarn() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        std::string eventID;
        if (mIsIgnOn == true && mConfigArea == ConfigArea::EEC) {
            if (mTPMSLowPressWarnStatus != TPMSLowPressWarnStatus::LOW &&
                mPrivateEventTrailerTPMSSignalMerge1Status == TrailerTPMSSignalMerge1Status::LOW) {
                eventID = "E72701";
            } else if (mTPMSLowPressWarnStatus == TPMSLowPressWarnStatus::LOW &&
                       mPrivateEventTrailerTPMSSignalMerge1Status != TrailerTPMSSignalMerge1Status::LOW) {
                eventID = "E72702";
            } else if (mTPMSLowPressWarnStatus == TPMSLowPressWarnStatus::LOW &&
                       mPrivateEventTrailerTPMSSignalMerge1Status == TrailerTPMSSignalMerge1Status::LOW) {
                eventID = "E72703";
            } else {
                // no operation
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.TPMS_CV.Event.TireLowPressureWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTPMS_CVEventTireLowPressureWarnStat(SFCTPMS_CVEventTireLowPressureWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTPMS_CVEventTireLowPressureWarnID(eventID);
            setSFCTPMS_CVEventTireLowPressureWarnStat(SFCTPMS_CVEventTireLowPressureWarnStat::ON);
        }
    }

    // 5.3.2 TPMS 오작동 경고
    void updateEventTPMSMalfunctionWarn() {
        if (mConfigTPMSCV != ConfigTPMSCV::ON) {
            return;
        }
        std::string eventID;
        if (mIsIgnOn == true && mConfigArea == ConfigArea::EEC) {
            if (mTPMSWarningStatus != TPMSWarningStatus::BLINK2 &&
                mPrivateEventTrailerTPMSSignalMerge2Status == TrailerTPMSSignalMerge2Status::FAULT) {
                eventID = "E72704";
            } else if (mTPMSWarningStatus == TPMSWarningStatus::BLINK2 &&
                       mPrivateEventTrailerTPMSSignalMerge2Status != TrailerTPMSSignalMerge2Status::FAULT) {
                eventID = "E72705";
            } else if (mTPMSWarningStatus == TPMSWarningStatus::BLINK2 &&
                       mPrivateEventTrailerTPMSSignalMerge2Status == TrailerTPMSSignalMerge2Status::FAULT) {
                eventID = "E72706";
            } else {
                // no operation
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.TPMS_CV.Event.TPMSMalfunctionWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTPMS_CVEventTPMSMalfunctionWarnStat(SFCTPMS_CVEventTPMSMalfunctionWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTPMS_CVEventTPMSMalfunctionWarnID(eventID);
            setSFCTPMS_CVEventTPMSMalfunctionWarnStat(SFCTPMS_CVEventTPMSMalfunctionWarnStat::ON);
        }
    }

    // Dummy function for WHEELLOCATION==TIMEOUT
    void updateConstantTireAXLE_NULL(const TirePrivateDataTuple& tuple) {
        return;
    };
};

}  // namespace ccos

#endif  // SFSS_TPMS_CV_H
