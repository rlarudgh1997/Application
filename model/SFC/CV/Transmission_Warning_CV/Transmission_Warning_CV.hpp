/**
 * @file Transmission_Warning_CV.hpp
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
#ifndef SFSS_Transmission_Warning_CV_H
#define SFSS_Transmission_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Transmission_Warning_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 15.0.0
// Spec Version : v0.36
// Reference : [CV410] Transmission_Warning_CV. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Transmission_Warning_CV : public Transmission_Warning_CVBase {
public:
    Transmission_Warning_CV() = default;
    ~Transmission_Warning_CV() override = default;
    Transmission_Warning_CV(const Transmission_Warning_CV& other) = delete;
    Transmission_Warning_CV(Transmission_Warning_CV&& other) noexcept = delete;
    Transmission_Warning_CV& operator=(const Transmission_Warning_CV& other) = delete;
    Transmission_Warning_CV& operator=(Transmission_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTransmission_Warning_CVTelltaleATMOverHeatBlinkValueA(SFCTransmission_Warning_CVTelltaleATMOverHeatBlinkValueA::ON);
        setSFCTransmission_Warning_CVTelltaleATMOverHeatBlinkValueB(
            SFCTransmission_Warning_CVTelltaleATMOverHeatBlinkValueB::OFF);
        setSFCTransmission_Warning_CVConstantTMFailureModeStopBlinkValueA(
            SFCTransmission_Warning_CVConstantTMFailureModeStopBlinkValueA::ON);
        setSFCTransmission_Warning_CVConstantTMFailureModeStopBlinkValueB(
            SFCTransmission_Warning_CVConstantTMFailureModeStopBlinkValueB::OFF);
        setSFCTransmission_Warning_CVEventShiftLeverPositionMismatchID("E74101");
        setSFCTransmission_Warning_CVEventShiftLeverPositionMismatchLinkedSoundID("SND_PopUpWarn1");
        setSFCTransmission_Warning_CVEventShiftLeverPositionMismatchLinkedSoundType(
            SFCTransmission_Warning_CVEventShiftLeverPositionMismatchLinkedSoundType::REPEAT_COUNT);
        setSFCTransmission_Warning_CVEventShiftLeverPositionMismatchLinkedSoundRepeatCount(1);
        setSFCTransmission_Warning_CVEventGearRatioStatusWarningID("E74102");
        setSFCTransmission_Warning_CVEventGearRatioStatusWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCTransmission_Warning_CVEventGearRatioStatusWarningLinkedSoundType(
            SFCTransmission_Warning_CVEventGearRatioStatusWarningLinkedSoundType::INFINITE);
        setSFCTransmission_Warning_CVEventNoBrakeShiftingID("E74103");
        setSFCTransmission_Warning_CVEventNoBrakeShiftingLinkedSoundID("SND_PopUpInform2");
        setSFCTransmission_Warning_CVEventNoBrakeShiftingLinkedSoundType(
            SFCTransmission_Warning_CVEventNoBrakeShiftingLinkedSoundType::REPEAT_COUNT);
        setSFCTransmission_Warning_CVEventNoBrakeShiftingLinkedSoundRepeatCount(1);

        if (mOneShotTimerShiftInhibitReasonStatusCV1sTimeout.create(
                kTimerInterval1s, this, &Transmission_Warning_CV::onOneShotTimerShiftInhibitReasonStatusCV1sTimeout, false) ==
            false) {
            DWarning() << __FUNCTION__ << "mOneShotTimerShiftInhibitReasonStatusCV1sTimeout Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltaleCheckATM();
        updateTelltaleATMOverHeat();
        updateConstant();
        updateEventShiftLeverPositionMismatch();
        updateEventGearRatioStatusWarning();
        updateEventNoBrakeShifting();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltaleCheckATM();
        updateTelltaleATMOverHeat();
        updateConstant();
        updateEventShiftLeverPositionMismatch();
        updateEventGearRatioStatusWarning();
        updateEventNoBrakeShifting();
    }

    void onConfigTMTypeChanged(const ArgumentsConfigTMTypeChanged& args) {
        mConfigTMType = args.mInter_ConfigTMType;
        updateTelltaleCheckATM();
        updateTelltaleATMOverHeat();
        updateConstant();
        updateEventShiftLeverPositionMismatch();
        updateEventGearRatioStatusWarning();
        updateEventNoBrakeShifting();
    }

    void onConfigVehicleTypeCVChanged(const ArgumentsConfigVehicleTypeCVChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateConstantTMFailureModeSpanner();
        updateConstantTMFailureModeStop();
        updateConstantTMHighTemperature();
        updateConstantTMRangeInhibit();
        updateEventGearRatioStatusWarning();
        updateEventNoBrakeShifting();
    }

    void onCheckATMChanged(const ArgumentsCheckATMChanged& args) {
        mATMWarningLampStatus = args.mInput_ATMWarningLampStatus;
        mTCUPGNumberStatusAllison = args.mInput_TCUPGNumberStatus_Allison;
        updateTelltaleCheckATM();
    }

    void onSignalATMOverHeatLampStatusChanged(const ArgumentsSignalATMOverHeatLampStatusChanged& args) {
        mATMOverHeatLampStatus = args.mInput_ATMOverHeatLampStatus;
        updateTelltaleATMOverHeat();
    }

    void onAtmRangeInhibitStatusChanged(const ArgumentsAtmRangeInhibitStatusChanged& args) {
        mAtmRangeInhibitStatus = args.mInput_ATMRangeInhibitStatus;
        updateConstantTMRangeInhibit();
    }

    void onTmAirlessStatusChanged(const ArgumentsTmAirlessStatusChanged& args) {
        mTMAirlessStatus = args.mInput_TMAirlessStatus;
        updateConstantTMAirless();
    }

    void onTmCheckPhaseStatusChanged(const ArgumentsTmCheckPhaseStatusChanged& args) {
        mTMCheckPhaseStatus = args.mInput_TMCheckPhaseStatus;
        updateConstantTMCheckPhase();
    }

    void onTmClutchLoadStatusChanged(const ArgumentsTmClutchLoadStatusChanged& args) {
        mTMClutchLoadStatus = args.mInput_TMClutchLoadStatus;
        updateConstantTMClutchLoad();
    }

    void onTmClutchWearStatusChanged(const ArgumentsTmClutchWearStatusChanged& args) {
        mTMClutchWearStatus = args.mInput_TMClutchWearStatus;
        updateConstantTMClutchWear();
    }

    void onTmFailureModeSpannerStatusChanged(const ArgumentsTmFailureModeSpannerStatusChanged& args) {
        mTMFailureModeSpannerStatus = args.mInput_TMFailureModeSpannerStatus;
        updateConstantTMFailureModeSpanner();
        updateConstantTMRangeInhibit();
    }

    void onTmFailureModeStopStatusChanged(const ArgumentsTmFailureModeStopStatusChanged& args) {
        mTMFailureModeStopStatus = args.mInput_TMFailureModeStopStatus;
        updateConstantTMFailureModeSpanner();
        updateConstantTMFailureModeStop();
        updateConstantTMRangeInhibit();
    }

    void onTmHighTemperatureStatusChanged(const ArgumentsTmHighTemperatureStatusChanged& args) {
        mTMHighTemperatureStatus = args.mInput_TMHighTemperatureStatus;
        updateConstantTMHighTemperature();
    }

    void onTmReleaseAccPedalStatusChanged(const ArgumentsTmReleaseAccPedalStatusChanged& args) {
        mTMReleaseAccPedalStatus = args.mInput_TMReleaseAccPedalStatus;
        updateConstantTMReleaseAccPedal();
    }

    void onTmWarningBuzzerStatusChanged(const ArgumentsTmWarningBuzzerStatusChanged& args) {
        mTMWarningBuzzerStatus = args.mInput_TMWarningBuzzerStatus;
        updateEventShiftLeverPositionMismatch();
    }

    void onLeverPosUnsuitStatusChanged(const ArgumentsLeverPosUnsuitStatusChanged& args) {
        mLeverPosUnsuitStatus = args.mInput_LeverPosUnsuitStatus;
        updateConstantTMRangeInhibit();
        updateEventGearRatioStatusWarning();
    }

    void onPressBrakePedalStatusChanged(const ArgumentsPressBrakePedalStatusChanged& args) {
        mPressBrakePedalStatus = args.mInput_PressBrakePedalStatus;
        updateEventNoBrakeShifting();
    }

    void onShiftInhibitReasonStatusChanged(const ArgumentsShiftInhibitReasonStatusChanged& args) {
        mShiftInhibitReasonStatus = args.mInput_ShiftInhibitReasonStatus;
        updateConstantTMRangeInhibit();
        updateEventNoBrakeShifting();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.1.1 ATM 고장 경고
    void updateTelltaleCheckATM() {
        SFCTransmission_Warning_CVTelltaleCheckATMStat stat = SFCTransmission_Warning_CVTelltaleCheckATMStat::OFF;
        if (mIsIgnOn == true && (mConfigTMType == ConfigTMType::ATM_ALLISON || mConfigTMType == ConfigTMType::ATM_ZF) &&
            (mATMWarningLampStatus == ATMWarningLampStatus::ON || mTCUPGNumberStatusAllison == TCUPGNumberStatusAllison::ON)) {
            stat = SFCTransmission_Warning_CVTelltaleCheckATMStat::ON;
        }
        setSFCTransmission_Warning_CVTelltaleCheckATMStat(stat);
    }

    // 5.1.2 ATM 과열 경고
    void updateTelltaleATMOverHeat() {
        SFCTransmission_Warning_CVTelltaleATMOverHeatStat stat = SFCTransmission_Warning_CVTelltaleATMOverHeatStat::OFF;
        if (mIsIgnOn == true && (mConfigTMType == ConfigTMType::ATM_ALLISON || mConfigTMType == ConfigTMType::ATM_ZF)) {
            if (mATMOverHeatLampStatus == ATMOverHeatLampStatus::ON) {
                stat = SFCTransmission_Warning_CVTelltaleATMOverHeatStat::ON;
            } else if (mATMOverHeatLampStatus == ATMOverHeatLampStatus::BLINK2) {
                stat = SFCTransmission_Warning_CVTelltaleATMOverHeatStat::BLINK2;
            } else {
                // no operation
            }
        }
        setSFCTransmission_Warning_CVTelltaleATMOverHeatStat(stat);
    }

    void updateConstant() {
        updateConstantTMAirless();
        updateConstantTMCheckPhase();
        updateConstantTMClutchLoad();
        updateConstantTMClutchWear();
        updateConstantTMFailureModeSpanner();
        updateConstantTMFailureModeStop();
        updateConstantTMHighTemperature();
        updateConstantTMReleaseAccPedal();
        updateConstantTMRangeInhibit();
    }

    // 5.2.1.1 Low Air
    void updateConstantTMAirless() {
        SFCTransmission_Warning_CVConstantTMAirlessStat stat = SFCTransmission_Warning_CVConstantTMAirlessStat::OFF;
        if (mIsIgnOn == true && mTMAirlessStatus == TMAirlessStatus::ON && mConfigTMType == ConfigTMType::AMT_ZF) {
            stat = SFCTransmission_Warning_CVConstantTMAirlessStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMAirlessStat(stat);
    }

    // 5.2.1.2 Check Phase
    void updateConstantTMCheckPhase() {
        SFCTransmission_Warning_CVConstantTMCheckPhaseStat stat = SFCTransmission_Warning_CVConstantTMCheckPhaseStat::OFF;
        if (mIsIgnOn == true && mTMCheckPhaseStatus == TMCheckPhaseStatus::ON && mConfigTMType == ConfigTMType::AMT_ZF) {
            stat = SFCTransmission_Warning_CVConstantTMCheckPhaseStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMCheckPhaseStat(stat);
    }

    // 5.2.1.3 Clutch Load
    void updateConstantTMClutchLoad() {
        SFCTransmission_Warning_CVConstantTMClutchLoadStat stat = SFCTransmission_Warning_CVConstantTMClutchLoadStat::OFF;
        if (mIsIgnOn == true && mTMClutchLoadStatus == TMClutchLoadStatus::ON && mConfigTMType == ConfigTMType::AMT_ZF) {
            stat = SFCTransmission_Warning_CVConstantTMClutchLoadStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMClutchLoadStat(stat);
    }

    // 5.2.1.4 Clutch Wear
    void updateConstantTMClutchWear() {
        SFCTransmission_Warning_CVConstantTMClutchWearStat stat = SFCTransmission_Warning_CVConstantTMClutchWearStat::OFF;
        if (mIsIgnOn == true && mTMClutchWearStatus == TMClutchWearStatus::ON && mConfigTMType == ConfigTMType::AMT_ZF) {
            stat = SFCTransmission_Warning_CVConstantTMClutchWearStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMClutchWearStat(stat);
    }

    // 5.2.1.5 Failure Mode Spanner
    void updateConstantTMFailureModeSpanner() {
        SFCTransmission_Warning_CVConstantTMFailureModeSpannerStat stat =
            SFCTransmission_Warning_CVConstantTMFailureModeSpannerStat::OFF;
        if (mIsIgnOn == true && mTMFailureModeSpannerStatus == TMFailureModeSpannerStatus::ON &&
            mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mTMFailureModeStopStatus == TMFailureModeStopStatus::OFF &&
            (mConfigTMType == ConfigTMType::AMT_ZF || mConfigTMType == ConfigTMType::ATM_ZF)) {
            stat = SFCTransmission_Warning_CVConstantTMFailureModeSpannerStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMFailureModeSpannerStat(stat);
    }

    // 5.2.1.6 Failure Mode Stop
    void updateConstantTMFailureModeStop() {
        SFCTransmission_Warning_CVConstantTMFailureModeStopStat stat =
            SFCTransmission_Warning_CVConstantTMFailureModeStopStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK &&
            mTMFailureModeStopStatus == TMFailureModeStopStatus::ON &&
            (mConfigTMType == ConfigTMType::AMT_ZF || mConfigTMType == ConfigTMType::ATM_ZF)) {
            stat = SFCTransmission_Warning_CVConstantTMFailureModeStopStat::BLINK2;
        }
        setSFCTransmission_Warning_CVConstantTMFailureModeStopStat(stat);
    }

    // 5.2.1.7 High Temperature
    void updateConstantTMHighTemperature() {
        SFCTransmission_Warning_CVConstantTMHighTemperatureStat stat =
            SFCTransmission_Warning_CVConstantTMHighTemperatureStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK &&
            mTMHighTemperatureStatus == TMHighTemperatureStatus::ON &&
            (mConfigTMType == ConfigTMType::AMT_ZF || mConfigTMType == ConfigTMType::ATM_ZF)) {
            stat = SFCTransmission_Warning_CVConstantTMHighTemperatureStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMHighTemperatureStat(stat);
    }

    // 5.2.1.9 Release AccPedal
    void updateConstantTMReleaseAccPedal() {
        SFCTransmission_Warning_CVConstantTMReleaseAccPedalStat stat =
            SFCTransmission_Warning_CVConstantTMReleaseAccPedalStat::OFF;
        if (mIsIgnOn == true && mTMReleaseAccPedalStatus == TMReleaseAccPedalStatus::ON &&
            mConfigTMType == ConfigTMType::AMT_ZF) {
            stat = SFCTransmission_Warning_CVConstantTMReleaseAccPedalStat::ON;
        }
        setSFCTransmission_Warning_CVConstantTMReleaseAccPedalStat(stat);
    }

    // 5.2.1.10 Range Inhibit
    void updateConstantTMRangeInhibit() {
        SFCTransmission_Warning_CVConstantTMRangeInhibitStat stat = SFCTransmission_Warning_CVConstantTMRangeInhibitStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigTMType == ConfigTMType::ATM_ALLISON) {
                if (mAtmRangeInhibitStatus == AtmRangeInhibitStatus::ON &&
                    mShiftInhibitReasonStatus == ShiftInhibitReasonStatus::OFF) {
                    stat = SFCTransmission_Warning_CVConstantTMRangeInhibitStat::ON;
                }
            } else if (mConfigTMType == ConfigTMType::ATM_ZF) {
                if (mLeverPosUnsuitStatus == LeverPosUnsuitStatus::ON &&
                    mTMFailureModeSpannerStatus == TMFailureModeSpannerStatus::OFF &&
                    mTMFailureModeStopStatus == TMFailureModeStopStatus::OFF &&
                    mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK) {
                    stat = SFCTransmission_Warning_CVConstantTMRangeInhibitStat::ON;
                }
            } else {
                // no operation
            }
        }
        setSFCTransmission_Warning_CVConstantTMRangeInhibitStat(stat);
    }

    // 5.3.1 변속 레버 포지션 불일치
    void updateEventShiftLeverPositionMismatch() {
        SFCTransmission_Warning_CVEventShiftLeverPositionMismatchStat stat =
            SFCTransmission_Warning_CVEventShiftLeverPositionMismatchStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::AMT_ZF && mTMWarningBuzzerStatus == TMWarningBuzzerStatus::ON) {
            stat = SFCTransmission_Warning_CVEventShiftLeverPositionMismatchStat::ON;
        }
        setSFCTransmission_Warning_CVEventShiftLeverPositionMismatchStat(stat);
    }

    // 5.3.2 운전자 의도와 다른 기어 치합 상태 경고
    void updateEventGearRatioStatusWarning() {
        SFCTransmission_Warning_CVEventGearRatioStatusWarningStat stat =
            SFCTransmission_Warning_CVEventGearRatioStatusWarningStat::OFF;
        if (mIsIgnOn == true && (mConfigTMType == ConfigTMType::AMT_ZF || mConfigTMType == ConfigTMType::ATM_ZF) &&
            mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mLeverPosUnsuitStatus == LeverPosUnsuitStatus::ON) {
            stat = SFCTransmission_Warning_CVEventGearRatioStatusWarningStat::ON;
        }
        setSFCTransmission_Warning_CVEventGearRatioStatusWarningStat(stat);
    }

    // 5.3.3 브레이크 밟지 않은 상태에서 변속시 알림
    void updateEventNoBrakeShifting() {
        SFCTransmission_Warning_CVEventNoBrakeShiftingStat stat = SFCTransmission_Warning_CVEventNoBrakeShiftingStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ALLISON) {
            if (mOneShotTimerShiftInhibitReasonStatusCV1sTimeout.isRunning() == true) {
                mOneShotTimerShiftInhibitReasonStatusCV1sTimeout.stop();
            }

            if (mShiftInhibitReasonStatus == ShiftInhibitReasonStatus::ON) {
                stat = SFCTransmission_Warning_CVEventNoBrakeShiftingStat::ON;
            } else if (mShiftInhibitReasonStatus == ShiftInhibitReasonStatus::ON_DELAY) {
                if (mOneShotTimerShiftInhibitReasonStatusCV1sTimeout.start() == false) {
                    DWarning() << "[Transmission_Warning_CV] mOneShotTimerShiftInhibitReasonStatusCV1sTimeout Start Failed";
                }
            } else {
                // no operation
            }
        } else if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ZF &&
                   mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mPressBrakePedalStatus == PressBrakePedalStatus::ON) {
            stat = SFCTransmission_Warning_CVEventNoBrakeShiftingStat::ON;
        } else {
            // no operation
        }
        setSFCTransmission_Warning_CVEventNoBrakeShiftingStat(stat);
    }

    void onOneShotTimerShiftInhibitReasonStatusCV1sTimeout() {
        if (mIsIgnOn == true && mShiftInhibitReasonStatus == ShiftInhibitReasonStatus::ON_DELAY) {
            SFCTransmission_Warning_CVEventNoBrakeShiftingStat stat = SFCTransmission_Warning_CVEventNoBrakeShiftingStat::ON;

            setSFCTransmission_Warning_CVEventNoBrakeShiftingStat(stat);
            flush();
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigTMType = ArgumentsConfigTMTypeChanged::Inter_ConfigTMType;
    using ConfigVehicleTypeCV = ArgumentsConfigVehicleTypeCVChanged::Inter_ConfigVehicleTypeCV;
    using ATMWarningLampStatus = ArgumentsCheckATMChanged::Input_ATMWarningLampStatus;
    using TCUPGNumberStatusAllison = ArgumentsCheckATMChanged::Input_TCUPGNumberStatus_Allison;
    using ATMOverHeatLampStatus = ArgumentsSignalATMOverHeatLampStatusChanged::Input_ATMOverHeatLampStatus;
    using AtmRangeInhibitStatus = ArgumentsAtmRangeInhibitStatusChanged::Input_ATMRangeInhibitStatus;
    using TMAirlessStatus = ArgumentsTmAirlessStatusChanged::Input_TMAirlessStatus;
    using TMCheckPhaseStatus = ArgumentsTmCheckPhaseStatusChanged::Input_TMCheckPhaseStatus;
    using TMClutchLoadStatus = ArgumentsTmClutchLoadStatusChanged::Input_TMClutchLoadStatus;
    using TMClutchWearStatus = ArgumentsTmClutchWearStatusChanged::Input_TMClutchWearStatus;
    using TMFailureModeSpannerStatus = ArgumentsTmFailureModeSpannerStatusChanged::Input_TMFailureModeSpannerStatus;
    using TMFailureModeStopStatus = ArgumentsTmFailureModeStopStatusChanged::Input_TMFailureModeStopStatus;
    using TMHighTemperatureStatus = ArgumentsTmHighTemperatureStatusChanged::Input_TMHighTemperatureStatus;
    using TMReleaseAccPedalStatus = ArgumentsTmReleaseAccPedalStatusChanged::Input_TMReleaseAccPedalStatus;
    using LeverPosUnsuitStatus = ArgumentsLeverPosUnsuitStatusChanged::Input_LeverPosUnsuitStatus;
    using PressBrakePedalStatus = ArgumentsPressBrakePedalStatusChanged::Input_PressBrakePedalStatus;
    using TMWarningBuzzerStatus = ArgumentsTmWarningBuzzerStatusChanged::Input_TMWarningBuzzerStatus;
    using ShiftInhibitReasonStatus = ArgumentsShiftInhibitReasonStatusChanged::Input_ShiftInhibitReasonStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    ConfigTMType mConfigTMType = ConfigTMType::MTM;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
    ATMWarningLampStatus mATMWarningLampStatus = ATMWarningLampStatus::OFF;
    TCUPGNumberStatusAllison mTCUPGNumberStatusAllison = TCUPGNumberStatusAllison::OFF;
    ATMOverHeatLampStatus mATMOverHeatLampStatus = ATMOverHeatLampStatus::OFF;
    AtmRangeInhibitStatus mAtmRangeInhibitStatus = AtmRangeInhibitStatus::OFF;
    LeverPosUnsuitStatus mLeverPosUnsuitStatus = LeverPosUnsuitStatus::OFF;
    PressBrakePedalStatus mPressBrakePedalStatus = PressBrakePedalStatus::OFF;
    TMAirlessStatus mTMAirlessStatus = TMAirlessStatus::OFF;
    TMCheckPhaseStatus mTMCheckPhaseStatus = TMCheckPhaseStatus::OFF;
    TMClutchLoadStatus mTMClutchLoadStatus = TMClutchLoadStatus::OFF;
    TMClutchWearStatus mTMClutchWearStatus = TMClutchWearStatus::OFF;
    TMFailureModeSpannerStatus mTMFailureModeSpannerStatus = TMFailureModeSpannerStatus::OFF;
    TMFailureModeStopStatus mTMFailureModeStopStatus = TMFailureModeStopStatus::OFF;
    TMHighTemperatureStatus mTMHighTemperatureStatus = TMHighTemperatureStatus::OFF;
    TMReleaseAccPedalStatus mTMReleaseAccPedalStatus = TMReleaseAccPedalStatus::OFF;
    TMWarningBuzzerStatus mTMWarningBuzzerStatus = TMWarningBuzzerStatus::OFF;
    ShiftInhibitReasonStatus mShiftInhibitReasonStatus = ShiftInhibitReasonStatus::OFF;

    HBool mIsIgnOn = false;

    ssfs::SFCTimer<Transmission_Warning_CV> mOneShotTimerShiftInhibitReasonStatusCV1sTimeout;

    static constexpr uint32_t kTimerInterval1s = 1000;
};

}  // namespace ccos

#endif  // SFSS_Transmission_Warning_CV_H
