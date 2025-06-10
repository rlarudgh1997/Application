/**
 * @file Transmission_Indicator.hpp
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
#ifndef SFSS_Transmission_Indicator_H
#define SFSS_Transmission_Indicator_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "Transmission_IndicatorBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 32.0.0
// Reference : [PT420] Transmission_Indicator. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class Transmission_Indicator : public Transmission_IndicatorBase {
public:
    Transmission_Indicator() = default;
    ~Transmission_Indicator() override = default;
    Transmission_Indicator(const Transmission_Indicator& other) = delete;
    Transmission_Indicator(Transmission_Indicator&& other) noexcept = delete;
    Transmission_Indicator& operator=(const Transmission_Indicator& other) = delete;
    Transmission_Indicator& operator=(Transmission_Indicator&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTransmission_IndicatorEventTransmission_PaddleShiftID("E24603");

        if (mOneShotTimerHWOutput2sTimeout.create(kTimerInterval2s, this,
                                                  &Transmission_Indicator::onOneShotTimerHWOutput2sTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerHWOutput2sTimeout Create Failed";
        }
        if (mOneShotTimerConstant2sTimeout.create(kTimerInterval2s, this,
                                                  &Transmission_Indicator::onOneShotTimerConstant2sTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerConstant2sTimeout Create Failed";
        }
        if (mOneShotTimerIgnOn300msTimeout.create(kTimerInterval300ms, this,
                                                  &Transmission_Indicator::onOneShotTimerIgnOn300msTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerIgnOn300msTimeout Create Failed";
        }
        if (mOneShotTimerSound300msTimeout.create(kTimerInterval300ms, this,
                                                  &Transmission_Indicator::onOneShotTimerSound300msTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerSound300msTimeout Create Failed";
        }
        if (mOneShotTimerEvent300msTimeout.create(kTimerInterval300ms, this,
                                                  &Transmission_Indicator::onOneShotTimerEvent300msTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerEvent300msTimeout Create Failed";
        }
        if (mOneShotTimerEventPaddleShift.create(kTimerInterval10ms, this,
                                                 &Transmission_Indicator::onOneShotTimerEventPaddleShift, false) == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerEventPaddleShift Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        DDebug() << "IgnitionElapsed::ON_0ms";
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        mIgnitionState = IgnitionState::ON;
        if (mOneShotTimerIgnOn300msTimeout.isRunning() == true) {
            mOneShotTimerIgnOn300msTimeout.stop();
        }
        if (mOneShotTimerIgnOn300msTimeout.start() == false) {
            DWarning() << "[Transmission_Indicator] mOneShotTimerIgnOn300msTimeout Start Failed";
        }
        updatePrivateGearStatus();
        updatePrivateTransmissionGearSta();
        updateOutputGearIndicatorStatus();
        updatePrivateTransmissionDisplayOffReq();
        updatePrivateTarGearUp();
        updatePrivateTarGearDown();
        updatePrivateTarGearStep();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateTransmissionAgiUpDownValue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
        flushActuatorOutput();
        mPrevIgnitionState = mIgnitionState;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        DDebug() << "IgnitionElapsed::OFF_0ms";
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        mIgnitionState = IgnitionState::OFF;
        if (mOneShotTimerIgnOn300msTimeout.isRunning() == true) {
            mOneShotTimerIgnOn300msTimeout.stop();
        }
        if (mOneShotTimerSound300msTimeout.isRunning() == true) {
            mOneShotTimerSound300msTimeout.stop();
        }
        updatePrivateTransmissionGearSta();
        updateOutputGearIndicatorStatus();
        updatePrivateTransmissionDisplayOffReq();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateTransmissionAgiUpDownValue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
        flushActuatorOutput();
        mPrevIgnitionState = mIgnitionState;
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        DDebug() << "Inter_ConfigHighPerformance " << static_cast<HUInt64>(args.mInter_ConfigHighPerformance);
        DDebug() << "Inter_ConfigTransmissionType " << static_cast<HUInt64>(args.mInter_ConfigTransmissionType);
        DDebug() << "Inter_ConfigVehicleType " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        DDebug() << "Inter_ConfigTransmissionDetailType " << static_cast<HUInt64>(args.mInter_ConfigTransmissionDetailType);
        DDebug() << "Inter_ConfigArea " << static_cast<HUInt64>(args.mInter_ConfigArea);
        DDebug() << "Inter_ConfigSBW " << static_cast<HUInt64>(args.mInter_ConfigSBW);
        DDebug() << "Inter_PTLogicControlType " << static_cast<HUInt64>(args.mInter_PTLogicControlType);
        mInter_ConfigHighPerformance = args.mInter_ConfigHighPerformance;
        mInter_ConfigTransmissionType = args.mInter_ConfigTransmissionType;
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        mInter_ConfigTransmissionDetailType = args.mInter_ConfigTransmissionDetailType;
        mInter_ConfigArea = args.mInter_ConfigArea;
        mInter_ConfigSBW = args.mInter_ConfigSBW;
        mInter_PTLogicControlType = args.mInter_PTLogicControlType;
        updateMaxStepTransmissionStat();
        updatePrivateGearStatus();
        updatePrivateTransmissionGearSta();
        updateOutputGearIndicatorStatus();
        updatePrivateTarGearUp();
        updatePrivateTarGearDown();
        updatePrivateTarGearStep();
        updateConstantTransmissionLayoutStat();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateConstantTransmissionPRNDfocusStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
        flushActuatorOutput();
    }

    void onPrivateGearAndGearSelectStatusChanged(const ArgumentsPrivateGearAndGearSelectStatusChanged& args) {
        DDebug() << "Input_GearDisplayStatusFromVCU " << static_cast<HUInt64>(args.mInput_GearDisplayStatusFromVCU);
        DDebug() << "Input_GearStepType " << static_cast<HUInt64>(args.mInput_GearStepType);
        DDebug() << "Input_GearIndManualSignalConfig " << static_cast<HUInt64>(args.mInput_GearIndManualSignalConfig);
        DDebug() << "Input_GearIndManualReqStatus1FromTCU " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatus1FromTCU);
        DDebug() << "Input_GearIndManualReqStatus2FromTCU " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatus2FromTCU);
        DDebug() << "Input_GearIndManualReqStatusFromHDCT " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatusFromHDCT);
        DDebug() << "Input_GearIndManualReqStatusOnEV " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatusOnEV);
        DDebug() << "Input_GearSelectStatusFromTCU " << static_cast<HUInt64>(args.mInput_GearSelectStatusFromTCU);
        DDebug() << "Input_GearSelectStatusFromHDCT " << static_cast<HUInt64>(args.mInput_GearSelectStatusFromHDCT);
        DDebug() << "Input_GearSelectStatusFromSCU " << static_cast<HUInt64>(args.mInput_GearSelectStatusFromSCU);
        mGearStepType = args.mInput_GearStepType;
        mGearIndManualSignalConfig = args.mInput_GearIndManualSignalConfig;

        if (ISTIMEOUT(args.mInput_GearDisplayStatusFromVCU) == true) {
            mGearDisplayStatusFromVCU = GearDisplayStatusFromVCU::UNHANDLED_TIMEOUT;
        } else {
            mGearDisplayStatusFromVCU = args.mInput_GearDisplayStatusFromVCU;
        }
        if (ISTIMEOUT(args.mInput_GearIndManualReqStatus1FromTCU)) {
            mGearIndManualReqStatus1FromTCU = GearIndManualReqStatus1FromTCU::UNHANDLED_TIMEOUT;
        } else {
            mGearIndManualReqStatus1FromTCU = args.mInput_GearIndManualReqStatus1FromTCU;
        }
        if (ISTIMEOUT(args.mInput_GearIndManualReqStatus2FromTCU)) {
            mGearIndManualReqStatus2FromTCU = GearIndManualReqStatus2FromTCU::UNHANDLED_TIMEOUT;
        } else {
            mGearIndManualReqStatus2FromTCU = args.mInput_GearIndManualReqStatus2FromTCU;
        }
        if (ISTIMEOUT(args.mInput_GearIndManualReqStatusFromHDCT)) {
            mGearIndManualReqStatusFromHDCT = GearIndManualReqStatusFromHDCT::UNHANDLED_TIMEOUT;
        } else {
            mGearIndManualReqStatusFromHDCT = args.mInput_GearIndManualReqStatusFromHDCT;
        }
        if (ISTIMEOUT(args.mInput_GearIndManualReqStatusOnEV)) {
            mGearIndManualReqStatusOnEV = GearIndManualReqStatusOnEV::UNHANDLED_TIMEOUT;
        } else {
            mGearIndManualReqStatusOnEV = args.mInput_GearIndManualReqStatusOnEV;
        }
        if (ISTIMEOUT(args.mInput_GearSelectStatusFromTCU) == true) {
            mGearSelectStatusFromTCU = GearSelectStatusFromTCU::UNHANDLED_TIMEOUT;
        } else {
            mGearSelectStatusFromTCU = args.mInput_GearSelectStatusFromTCU;
        }
        if (ISTIMEOUT(args.mInput_GearSelectStatusFromHDCT) == true) {
            mGearSelectStatusFromHDCT = GearSelectStatusFromHDCT::UNHANDLED_TIMEOUT;
        } else {
            mGearSelectStatusFromHDCT = args.mInput_GearSelectStatusFromHDCT;
        }
        if (ISTIMEOUT(args.mInput_GearSelectStatusFromSCU) == true) {
            mGearSelectStatusFromSCU = GearSelectStatusFromSCU::UNHANDLED_TIMEOUT;
        } else {
            mGearSelectStatusFromSCU = args.mInput_GearSelectStatusFromSCU;
        }
        updatePrivateGearStatus();
        updatePrivateTransmissionGearSta();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateConstantTransmissionPRNDfocusStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
        updateOutputGearIndicatorStatus();
        flushActuatorOutput();
    }

    void onPrivateTarGearUpChanged(const ArgumentsPrivateTarGearUpChanged& args) {
        DDebug() << "Input_GearShiftIndiUpStatusFromEmsHcu " << static_cast<HUInt64>(args.mInput_GearShiftIndiUpStatusFromEmsHcu);
        DDebug() << "Input_GearShiftIndiUpStatusFromHTCU " << static_cast<HUInt64>(args.mInput_GearShiftIndiUpStatusFromHTCU);
        mGearShiftIndiUpStatusFromEmsHcu = args.mInput_GearShiftIndiUpStatusFromEmsHcu;
        mGearShiftIndiUpStatusFromHTCU = args.mInput_GearShiftIndiUpStatusFromHTCU;
        updatePrivateTarGearUp();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
    }

    void onPrivateTarGearDownChanged(const ArgumentsPrivateTarGearDownChanged& args) {
        DDebug() << "Input_GearShiftIndiDownStatusFromHTCU " << static_cast<HUInt64>(args.mInput_GearShiftIndiDownStatusFromHTCU);
        DDebug() << "Input_GearShiftIndiDownStatusFromEmsHcu "
                 << static_cast<HUInt64>(args.mInput_GearShiftIndiDownStatusFromEmsHcu);
        mGearShiftIndiDownStatusFromEmsHcu = args.mInput_GearShiftIndiDownStatusFromEmsHcu;
        mGearShiftIndiDownStatusFromHTCU = args.mInput_GearShiftIndiDownStatusFromHTCU;
        updatePrivateTarGearDown();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
    }

    void onPrivateTarGearStepChanged(const ArgumentsPrivateTarGearStepChanged& args) {
        DDebug() << "Input_GearShiftIndiStepStatusFromEmsHcu "
                 << static_cast<HUInt64>(args.mInput_GearShiftIndiStepStatusFromEmsHcu);
        DDebug() << "Input_GearShiftIndiStepStatusFromHTCU " << static_cast<HUInt64>(args.mInput_GearShiftIndiStepStatusFromHTCU);
        DDebug() << "Input_GearShiftIndiStepNewStatus " << static_cast<HUInt64>(args.mInput_GearShiftIndiStepNewStatus);
        mGearShiftIndiStepStatusFromEmsHcu = args.mInput_GearShiftIndiStepStatusFromEmsHcu;
        mGearShiftIndiStepStatusFromHTCU = args.mInput_GearShiftIndiStepStatusFromHTCU;
        mGearShiftIndiStepNewStatus = args.mInput_GearShiftIndiStepNewStatus;
        updatePrivateTarGearStep();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
    }

    void onGearShiftIndiUpDownNewStatusChanged(const ArgumentsGearShiftIndiUpDownNewStatusChanged& args) {
        mGearShiftIndiUpDownNewStatus = args.mInput_GearShiftIndiUpDownNewStatus;
        updatePrivateTarGearUp();
        updatePrivateTarGearDown();
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
    }

    void onTransmissionDisplayValueATChanged(const ArgumentsTransmissionDisplayValueATChanged& args) {
        DDebug() << "Input_SbwParkNeutralStatus " << static_cast<HUInt64>(args.mInput_SbwParkNeutralStatus);
        DDebug() << "Input_DoorPassiveLockReqStatus " << static_cast<HUInt64>(args.mInput_DoorPassiveLockReqStatus);
        DDebug() << "Input_DoorRKELockUnlockReqStatus " << static_cast<HUInt64>(args.mInput_DoorRKELockUnlockReqStatus);

        // `mSbwParkNeutralStatus` signal은 `EXNCP-42785` 이슈와 연관이 없기 때문에,
        // 초기화 여부와 상관없이 값을 설정 및 사용하여 로직 처리가 수행될 수 있도록 위치 수정
        mSbwParkNeutralStatus = args.mInput_SbwParkNeutralStatus;

        static bool isLockReqInitialized = false;
        if (ISINITIALIZEDSIGNAL(args.mInput_DoorPassiveLockReqStatus) == true &&
            ISINITIALIZEDSIGNAL(args.mInput_DoorRKELockUnlockReqStatus) == true) {
            isLockReqInitialized = true;
        }

        if (isLockReqInitialized == true) {
            mDoorPassiveLockReqStatus = args.mInput_DoorPassiveLockReqStatus;
            mDoorRKELockUnlockReqStatus = args.mInput_DoorRKELockUnlockReqStatus;
            updatePrivateTransmissionDisplayOffReq();
            updateConstantTransmissionDisplayAndGSIStatValue();
            updatePrivateTransmissionDisplayForAgivalue();
            updatePrivateAgiDisplayOnOffStat();
            updatePrivateAgiUpDownAvailable_Stat();
            updateTransmissionAgiStat();
            updatePrivateTransmissionBlinkingStat();
            updateConstantTransmissionBlinkingStat();
            updateEventTransmissionPaddleShift();
            updatePrevPrivateTransmissionGearSta();
            mPrev_DoorPassiveLockReqStatus = mDoorPassiveLockReqStatus;
            mPrev_DoorRKELockUnlockReqStatus = mDoorRKELockUnlockReqStatus;
        }
        updateEventTransmissionIndicator();
        updateSoundTransmissionIndicator();
        mPrev_SbwParkNeutralStatus = mSbwParkNeutralStatus;
    }

    void onTransmissionDisplayValueNotMTNChanged(const ArgumentsTransmissionDisplayValueNotMTNChanged& args) {
        DDebug() << "Input_TransmissionEolErrorWarnStatusFromIMT "
                 << static_cast<HUInt64>(args.mInput_TransmissionEolErrorWarnStatusFromIMT);
        DDebug() << "Input_GearIndManualReqStatusOnIMT " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatusOnIMT);
        mTransmissionEolErrorWarnStatusFromIMT = args.mInput_TransmissionEolErrorWarnStatusFromIMT;
        mGearIndManualReqStatusOnIMT = args.mInput_GearIndManualReqStatusOnIMT;
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
    }

    void onGearIndiDisplayReqStatusFromIMTChanged(const ArgumentsGearIndiDisplayReqStatusFromIMTChanged& args) {
        DDebug() << "Input_GearIndiDisplayReqStatusFromIMT " << static_cast<HUInt64>(args.mInput_GearIndiDisplayReqStatusFromIMT);
        mGearIndiDisplayReqStatusFromIMT = args.mInput_GearIndiDisplayReqStatusFromIMT;
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
    }

    void onGearPositionRStatusFromICUChanged(const ArgumentsGearPositionRStatusFromICUChanged& args) {
        DDebug() << "Input_GearPositionRStatusFromICU " << static_cast<HUInt64>(args.mInput_GearPositionRStatusFromICU);
        mGearPositionRStatusFromICU = args.mInput_GearPositionRStatusFromICU;
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
    }

    void onTransmissionDisplayValueMTNChanged(const ArgumentsTransmissionDisplayValueMTNChanged& args) {
        DDebug() << "Input_GearPositionRStatusFromEMS " << static_cast<HUInt64>(args.mInput_GearPositionRStatusFromEMS);
        DDebug() << "Input_GearIndManualReqStatusOnMT " << static_cast<HUInt64>(args.mInput_GearIndManualReqStatusOnMT);
        mGearPositionRStatusFromEMS = args.mInput_GearPositionRStatusFromEMS;
        if (ISTIMEOUT(args.mInput_GearIndManualReqStatusOnMT)) {
            mGearIndManualReqStatusOnMT = GearIndManualReqStatusOnMT::UNHANDLED_TIMEOUT;
        } else {
            mGearIndManualReqStatusOnMT = args.mInput_GearIndManualReqStatusOnMT;
        }
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateEventTransmissionIndicator();
        updateEventTransmissionPaddleShift();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
    }

    void onTransmissionBlinkingStatChanged(const ArgumentsTransmissionBlinkingStatChanged& args) {
        DDebug() << "Input_GearIndiBlinkReqStatusFromDCT " << static_cast<HUInt64>(args.mInput_GearIndiBlinkReqStatusFromDCT);
        DDebug() << "Input_GearIndiBlinkReqStatusFromTCU " << static_cast<HUInt64>(args.mInput_GearIndiBlinkReqStatusFromTCU);
        mGearIndiBlinkReqStatusFromDCT = args.mInput_GearIndiBlinkReqStatusFromDCT;
        mGearIndiBlinkReqStatusFromTCU = args.mInput_GearIndiBlinkReqStatusFromTCU;
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
    }

    void onTransmissionEolErrorWarnStatusFromTCUChanged(const ArgumentsTransmissionEolErrorWarnStatusFromTCUChanged& args) {
        DDebug() << "Input_TransmissionEolErrorWarnStatusFromTCU "
                 << static_cast<HUInt64>(args.mInput_TransmissionEolErrorWarnStatusFromTCU);
        mTransmissionEolErrorWarnStatusFromTCU = args.mInput_TransmissionEolErrorWarnStatusFromTCU;
        updateConstantTransmissionDisplayAndGSIStatValue();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updateSoundTransmissionIndicator();
        updatePrevPrivateTransmissionGearSta();
    }

    void onGearMaxStepInfoChanged(const ArgumentsGearMaxStepInfoChanged& args) {
        mGearMaxStepInfoFromEmsHcu = args.mInput_GearMaxStepInfoFromEmsHcu;
        mGearMaxStepInfoFromHDCT = args.mInput_GearMaxStepInfoFromHDCT;
        updateMaxStepTransmissionStat();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        updateEventTransmissionPaddleShift();
    }

    void onSbwTypeStatusChanged(const ArgumentsSbwTypeStatusChanged& args) {
        DDebug() << "Input_SbwTypeStatus " << static_cast<HUInt64>(args.mInput_SbwTypeStatus);
        mSbwTypeStatus = args.mInput_SbwTypeStatus;
        updateEventTransmissionSBW();
    }

    void onTransmissionAgiStatusChanged(const ArgumentsTransmissionAgiStatusChanged& args) {
        mTransmissionAgiUpStatus = args.mInput_TransmissionAgiUpStatus;
        mTransmissionAgiDownStatus = args.mInput_TransmissionAgiDownStatus;
        updatePrivateTransmissionAgiUpDownValue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1
    void updateOutputGearIndicatorStatus() {
        HBool isTimer = false;
        if (mIgnitionElapsed == IgnitionElapsed::ON_300ms) {
            switch (getPrivateTransmissionGearSta()) {
                case Private_TransmissionGearSta::STAGE_P:
                    mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::STEP_P;
                    break;
                case Private_TransmissionGearSta::STAGE_L:
                case Private_TransmissionGearSta::STAGE_2:
                case Private_TransmissionGearSta::STAGE_3:
                case Private_TransmissionGearSta::STAGE_D:
                case Private_TransmissionGearSta::PADDLE_SHIFT:
                case Private_TransmissionGearSta::STAGE_M:
                    mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::STEP_D;
                    break;
                case Private_TransmissionGearSta::DS_MODE:
                case Private_TransmissionGearSta::MANUAL:
                    mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::STEP_S;
                    break;
                case Private_TransmissionGearSta::STAGE_N:
                    mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::STEP_N;
                    break;
                case Private_TransmissionGearSta::STAGE_R:
                    mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::STEP_R;
                    break;
                case Private_TransmissionGearSta::INTERMEDIATE:
                case Private_TransmissionGearSta::MESSAGE_TIMEOUT:
                    break;
                default:  // NONE, INVALID
                    // OLD_VALUE 2s => OFF
                    if (mOneShotTimerHWOutput2sTimeout.isRunning() == false) {
                        if (mPrev_Output_GearIndicatorStatus != Output_GearIndicatorStatus::OFF) {
                            if (mOneShotTimerHWOutput2sTimeout.start() == false) {
                                DWarning() << "[Transmission_Indicator] mOneShotTimerHWOutput2sTimeout Start Failed";
                            }
                        }
                    }
                    isTimer = true;
                    break;
            }
        } else {
            mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::OFF;
        }
        if (isTimer == false && mOneShotTimerHWOutput2sTimeout.isRunning() == true) {
            mOneShotTimerHWOutput2sTimeout.stop();
        }
        if (mPrev_Output_GearIndicatorStatus != mOutput_GearIndicatorStatus) {
            mPrev_Output_GearIndicatorStatus = mOutput_GearIndicatorStatus;
            updateActuatorValue(Vehicle.System.HardWire.Output_GearIndicatorStatus,
                                static_cast<HUInt64>(mOutput_GearIndicatorStatus));
            flushActuatorOutput();
        }
    }

    // 4.3.1
    void updatePrivateGearStatus() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                if (mGearIndManualSignalConfig == GearIndManualSignalConfig::SIGNAL2 &&
                    (mGearStepType == GearStepType::GEAR8 || mGearStepType == GearStepType::NON_GEAR8)) {
                    mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_2_TCU;
                    mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatus2FromTCU);
                } else if (mGearIndManualSignalConfig == GearIndManualSignalConfig::SIGNAL1) {
                    if (mGearStepType == GearStepType::GEAR8) {
                        mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_2_TCU;
                        mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatus2FromTCU);
                    } else if (mGearStepType == GearStepType::NON_GEAR8) {
                        mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_1_TCU;
                        mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatus1FromTCU);
                    } else {
                        // nothing
                    }
                } else {
                    // nothing
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                if (mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT) {
                    mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_HDCT;
                    mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatusFromHDCT);
                } else {
                    mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_1_TCU;
                    mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatus1FromTCU);
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) {
                mPrivate_GearStatusType = Private_GearStatusType::INPUT_GIMRS_ON_EV;
                mPrivate_GearStatusValue = static_cast<HUInt64>(mGearIndManualReqStatusOnEV);
            } else {
                // nothing
            }
        }
    }

    // 4.3.2
    void updatePrivateTransmissionGearSta() {
        mGearCrcError = false;

        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                if (mInter_ConfigSBW == Inter_ConfigSBW::OFF) {
                    mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_TCU;
                    mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromTCU);
                } else {
                    if (mGearSelectStatusFromTCU != GearSelectStatusFromTCU::INVALID &&
                        mGearSelectStatusFromTCU != GearSelectStatusFromTCU::UNHANDLED_TIMEOUT) {
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_TCU;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromTCU);
                    } else {  // Others
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_SCU;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromSCU);
                    }
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                if (mInter_ConfigSBW == Inter_ConfigSBW::OFF) {
                    if (mInter_ConfigTransmissionDetailType != Inter_ConfigTransmissionDetailType::DCT) {
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_TCU;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromTCU);
                    } else {
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_HDCT;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromHDCT);
                    }
                } else if (mInter_ConfigSBW == Inter_ConfigSBW::ON) {
                    if (mInter_ConfigTransmissionDetailType != Inter_ConfigTransmissionDetailType::DCT &&
                        mGearSelectStatusFromTCU != GearSelectStatusFromTCU::INVALID &&
                        mGearSelectStatusFromTCU != GearSelectStatusFromTCU::UNHANDLED_TIMEOUT) {
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_TCU;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromTCU);
                    } else if (mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT &&
                               mGearSelectStatusFromHDCT != GearSelectStatusFromHDCT::INVALID &&
                               mGearSelectStatusFromHDCT != GearSelectStatusFromHDCT::UNHANDLED_TIMEOUT) {
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_HDCT;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromHDCT);
                    } else {  // Others
                        mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_SCU;
                        mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromSCU);
                    }
                } else {
                    // nothing
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) {
                mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_VCU;
                mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearDisplayStatusFromVCU);
            } else {
                // nothing
            }
        } else if (mIgnitionState == IgnitionState::OFF) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                if (mInter_ConfigSBW == Inter_ConfigSBW::ON) {
                    mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_SCU;
                    mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearSelectStatusFromSCU);
                }
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) {
                mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::INPUT_GSS_VCU;
                mPrivate_TransmissionGearStaValue = static_cast<HUInt64>(mGearDisplayStatusFromVCU);
            } else {
                // nothing
            }
        } else {
            // nothing
        }

        // IGN 상태와 VehicleType, SBW ON/OFF에 따라 결정되는 한 개의 신호타입에 대한 CRC_ERROR Check
        if ((mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_VCU &&
             mGearDisplayStatusFromVCU == GearDisplayStatusFromVCU::CRC_ERROR) ||
            (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_SCU &&
             mGearSelectStatusFromSCU == GearSelectStatusFromSCU::CRC_ERROR) ||
            (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_TCU &&
             mGearSelectStatusFromTCU == GearSelectStatusFromTCU::CRC_ERROR) ||
            (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_HDCT &&
             mGearSelectStatusFromHDCT == GearSelectStatusFromHDCT::CRC_ERROR)) {
            mGearCrcError = true;
        }
        _DDebug() << "Private_TransmissionGearStaType: " << static_cast<HUInt64>(mPrivate_TransmissionGearStaType);
        _DDebug() << "Private_TransmissionGearStaValue: " << static_cast<HUInt64>(mPrivate_TransmissionGearStaValue);
        _DDebug() << "Private_TransmissionGearSta CRC_Error Status : " << static_cast<HBool>(mGearCrcError);

        updateEventTransmissionSBW();
    }

    // 4.4
    void updatePrivateTarGearUp() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_PTLogicControlType == Inter_PTLogicControlType::OFF) {
                if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                    mPrivate_TarGearUp = static_cast<HUInt64>(mGearShiftIndiUpStatusFromEmsHcu);
                } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                           mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                    if (mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT) {
                        mPrivate_TarGearUp = static_cast<HUInt64>(mGearShiftIndiUpStatusFromEmsHcu);
                    } else {
                        mPrivate_TarGearUp = static_cast<HUInt64>(mGearShiftIndiUpStatusFromHTCU);
                    }
                } else {
                    // nothing
                }
            } else if (mInter_PTLogicControlType == Inter_PTLogicControlType::ON) {
                if (mGearShiftIndiUpDownNewStatus == GearShiftIndiUpDownNewStatus::UP) {
                    mPrivate_TarGearUp = static_cast<HUInt64>(Private_TarGearType::ON);
                } else {
                    mPrivate_TarGearUp = static_cast<HUInt64>(Private_TarGearType::OFF);
                }
            } else {
                // nothing
            }
        }
    }

    // 4.4
    void updatePrivateTarGearDown() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_PTLogicControlType == Inter_PTLogicControlType::OFF) {
                if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                    mPrivate_TarGearDown = static_cast<HUInt64>(mGearShiftIndiDownStatusFromEmsHcu);
                } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                           mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                    if (mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT) {
                        mPrivate_TarGearDown = static_cast<HUInt64>(mGearShiftIndiDownStatusFromEmsHcu);
                    } else {
                        mPrivate_TarGearDown = static_cast<HUInt64>(mGearShiftIndiDownStatusFromHTCU);
                    }
                } else {
                    // nothing
                }
            } else if (mInter_PTLogicControlType == Inter_PTLogicControlType::ON) {
                if (mGearShiftIndiUpDownNewStatus == GearShiftIndiUpDownNewStatus::DOWN) {
                    mPrivate_TarGearDown = static_cast<HUInt64>(Private_TarGearType::ON);
                } else {
                    mPrivate_TarGearDown = static_cast<HUInt64>(Private_TarGearType::OFF);
                }
            } else {
                // nothing
            }
        }
    }

    // 4.4
    void updatePrivateTarGearStep() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mInter_PTLogicControlType == Inter_PTLogicControlType::OFF) {
                if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) {
                    mPrivate_TarGearStep = static_cast<Private_TarGearStep>(mGearShiftIndiStepStatusFromEmsHcu);
                } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                           mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                    if (mInter_ConfigTransmissionDetailType != Inter_ConfigTransmissionDetailType::NONE) {
                        mPrivate_TarGearStep = static_cast<Private_TarGearStep>(mGearShiftIndiStepStatusFromEmsHcu);
                    } else {
                        mPrivate_TarGearStep = static_cast<Private_TarGearStep>(mGearShiftIndiStepStatusFromHTCU);
                    }
                } else {
                    // nothing
                }
            } else if (mInter_PTLogicControlType == Inter_PTLogicControlType::ON) {
                mPrivate_TarGearStep = static_cast<Private_TarGearStep>(mGearShiftIndiStepNewStatus);
            } else {
                // nothing
            }
        }
    }

    // 4.5
    void updatePrivateTransmissionBlinkingStat() {
        if (mIgnitionState == IgnitionState::ON) {
            if (mConstantTransmissionGSIStat == ConstantTransmissionGSIStat::GSI_UP ||
                mConstantTransmissionGSIStat == ConstantTransmissionGSIStat::GSI_DOWN ||
                mConstantTransmissionDisplayValue == ConstantTransmissionDisplayValue::DISP_E) {
                mPrivate_TransmissionBlinking_Stat = Private_TransmissionBlinking_Stat::OFF;
            } else if (mGearIndiBlinkReqStatusFromDCT == GearIndiBlinkReqStatusFromDCT::BLINK ||
                       mGearIndiBlinkReqStatusFromTCU == GearIndiBlinkReqStatusFromTCU::ON ||
                       mGearIndiDisplayReqStatusFromIMT == GearIndiDisplayReqStatusFromIMT::BLINK) {
                mPrivate_TransmissionBlinking_Stat = Private_TransmissionBlinking_Stat::ON;
            } else {
                mPrivate_TransmissionBlinking_Stat = Private_TransmissionBlinking_Stat::OFF;
            }
        }
    }

    // 4.6 & 5.2.5
    void updateMaxStepTransmissionStat() {
        switch (mInter_ConfigVehicleType) {
            case Inter_ConfigVehicleType::ICV:
            case Inter_ConfigVehicleType::EV:
            case Inter_ConfigVehicleType::FCEV:
            case Inter_ConfigVehicleType::EREV:
                mPrivate_MaxStepTransmission_Stat = getPrivate_MaxStepTranmissionHcu();
                break;

            case Inter_ConfigVehicleType::HEV:
            case Inter_ConfigVehicleType::PHEV:
                if (mInter_ConfigTransmissionDetailType != Inter_ConfigTransmissionDetailType::DCT) {
                    mPrivate_MaxStepTransmission_Stat = getPrivate_MaxStepTranmissionHcu();
                } else {
                    mPrivate_MaxStepTransmission_Stat = getPrivate_MaxStepTranmissionHDCT();
                }
                break;

            default:
                break;
        }

        switch (mPrivate_MaxStepTransmission_Stat) {
            case Private_MaxStepTransmission_Stat::MAX_THREE:
                mPrivate_MaxStepTransmissionValue = 3;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_THREE);
                break;
            case Private_MaxStepTransmission_Stat::MAX_FOUR:
                mPrivate_MaxStepTransmissionValue = 4;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_FOUR);
                break;
            case Private_MaxStepTransmission_Stat::MAX_FIVE:
                mPrivate_MaxStepTransmissionValue = 5;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_FIVE);
                break;
            case Private_MaxStepTransmission_Stat::MAX_SIX:
                mPrivate_MaxStepTransmissionValue = 6;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_SIX);
                break;
            case Private_MaxStepTransmission_Stat::MAX_SEVEN:
                mPrivate_MaxStepTransmissionValue = 7;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_SEVEN);
                break;
            case Private_MaxStepTransmission_Stat::MAX_EIGHT:
                mPrivate_MaxStepTransmissionValue = 8;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::MAX_EIGHT);
                break;
            default:
                mPrivate_MaxStepTransmissionValue = 0;
                setSFCTransmission_IndicatorConstantMaxStepTransmissionStat(
                    SFCTransmission_IndicatorConstantMaxStepTransmissionStat::DISPLAY_OFF);
                break;
        }
    }

    // 5.2
    void updateConstantTransmissionDisplayAndGSIStatValue() {
        if (mInter_ConfigTransmissionType == Inter_ConfigTransmissionType::AT ||
            mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::AMT) {
            updateConstantTransmissionGSIStatValueAT();
            updateConstantTransmissionDisplayValueAT();
        } else if (mInter_ConfigTransmissionType == Inter_ConfigTransmissionType::MT &&
                   mInter_ConfigTransmissionDetailType != Inter_ConfigTransmissionDetailType::AMT &&
                   mInter_ConfigHighPerformance != Inter_ConfigHighPerformance::N) {
            updateConstantTransmissionGSIStatValueMT();
            updateConstantTransmissionDisplayValueMT();
        } else if (mInter_ConfigTransmissionType == Inter_ConfigTransmissionType::MT &&
                   mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N) {
            // same MT-!N
            updateConstantTransmissionGSIStatValueMT();
            updateConstantTransmissionDisplayValueMTN();
        } else {
            // nothing
        }
    }

    // 5.2.1.1.1
    void updateConstantTransmissionGSIStatValueAT() {
        mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_DISPLAY_OFF;            // for Others
        mConstantTransmissionGSITargetValue = ConstantTransmissionGSITargetValue::DISPLAY_OFF;  // for Others

        if (mIgnitionState == IgnitionState::ON) {
            HBool privateTransmissionGearStaFlag =
                (getPrivateTransmissionGearSta() == Private_TransmissionGearSta::MANUAL ||
                 getPrivateTransmissionGearSta() == Private_TransmissionGearSta::PADDLE_SHIFT ||
                 getPrivateTransmissionGearSta() == Private_TransmissionGearSta::STAGE_M);
            HBool inter_ConfigAreaFlag =
                (mInter_ConfigArea == Inter_ConfigArea::EEC || mInter_ConfigArea == Inter_ConfigArea::RUSSIA);
            HUInt64 speedValue = static_cast<HUInt64>(mPrivate_TarGearStep);
            HInt64 diffSpeedValue = static_cast<HUInt64>(ConstantTransmissionGSITargetValue::GSI_TARGET_1) -
                                    static_cast<HUInt64>(Private_TarGearStep::SPEED1);
            HBool private_TarGearStepFlag =
                (static_cast<HUInt64>(mPrivate_TarGearStep) >= static_cast<HUInt64>(Private_TarGearStep::SPEED1) &&
                 static_cast<HUInt64>(mPrivate_TarGearStep) <= static_cast<HUInt64>(Private_TarGearStep::SPEED8));

            if (privateTransmissionGearStaFlag && inter_ConfigAreaFlag && private_TarGearStepFlag) {
                if (mPrivate_TarGearUp == static_cast<HUInt64>(Private_TarGearType::ON) &&
                    mPrivate_TarGearDown == static_cast<HUInt64>(Private_TarGearType::OFF)) {
                    mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_UP;
                } else if (mPrivate_TarGearDown == static_cast<HUInt64>(Private_TarGearType::ON) &&
                           mPrivate_TarGearUp == static_cast<HUInt64>(Private_TarGearType::OFF)) {
                    mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_DOWN;
                } else {
                    // nothing
                }
            }

            if (private_TarGearStepFlag && (mConstantTransmissionGSIStat == ConstantTransmissionGSIStat::GSI_UP ||
                                            mConstantTransmissionGSIStat == ConstantTransmissionGSIStat::GSI_DOWN)) {
                mConstantTransmissionGSITargetValue =
                    static_cast<ConstantTransmissionGSITargetValue>(speedValue + diffSpeedValue);
            }
        }

        setSFCTransmission_IndicatorConstantTransmissionGSIStat(mConstantTransmissionGSIStat);
        setSFCTransmission_IndicatorConstantTransmissionGSITargetValue(mConstantTransmissionGSITargetValue);
    }

    // 5.2.1.1.2
    void updateConstantTransmissionDisplayValueAT() {
        ConstantTransmissionDisplayValue newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISPLAY_OFF;
        HBool oldValue2secFlag = false;

        if (mIgnitionState == IgnitionState::ON) {
            if (mTransmissionEolErrorWarnStatusFromTCU == TransmissionEolErrorWarnStatusFromTCU::EOL_ERROR) {
                newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_E;
                mIsGearChangedFromIgnOn = true;
            } else {
                HUInt64 privateGearStatus = static_cast<HUInt64>(getPrivateGearStatus());
                switch (getPrivateTransmissionGearSta()) {
                    case Private_TransmissionGearSta::STAGE_P:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_P;
                        break;
                    case Private_TransmissionGearSta::STAGE_L:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED1;
                        break;
                    case Private_TransmissionGearSta::STAGE_2:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED2;
                        break;
                    case Private_TransmissionGearSta::STAGE_3:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED3;
                        break;
                    case Private_TransmissionGearSta::STAGE_N:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_N;
                        break;
                    case Private_TransmissionGearSta::STAGE_R:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_R;
                        break;
                    case Private_TransmissionGearSta::MANUAL:
                    case Private_TransmissionGearSta::PADDLE_SHIFT:
                        mIsGearChangedFromIgnOn = true;
                        if ((privateGearStatus >= static_cast<HUInt64>(Private_GearStatus::SPEED1)) &&
                            (privateGearStatus <= static_cast<HUInt64>(Private_GearStatus::SPEED8))) {
                            newConstantTransmissionDisplayValue = static_cast<ConstantTransmissionDisplayValue>(
                                static_cast<HUInt64>(ConstantTransmissionDisplayValue::SPEED1) + privateGearStatus -
                                static_cast<HUInt64>(Private_GearStatus::SPEED1));
                        } else {
                            if (getPrivateGearStatus() == Private_GearStatus::UNHANDLED_TIMEOUT) {
                                oldValue2secFlag = true;
                            } else if (mGearCrcError == false) {
                                newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_D;
                            } else {
                                oldValue2secFlag = true;
                            }
                        }
                        break;
                    case Private_TransmissionGearSta::DS_MODE:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_S;  // for Others
                        if ((privateGearStatus >= static_cast<HUInt64>(Private_GearStatus::SPEED1)) &&
                            (privateGearStatus <= static_cast<HUInt64>(Private_GearStatus::SPEED8))) {
                            if (mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N ||
                                ((mInter_ConfigHighPerformance != Inter_ConfigHighPerformance::N &&
                                  mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT &&
                                  mInter_ConfigArea != Inter_ConfigArea::CAN && mInter_ConfigArea != Inter_ConfigArea::USA) &&
                                 (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
                                  mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                                  mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV))) {
                                if (mGearCrcError == false) {
                                    newConstantTransmissionDisplayValue = static_cast<ConstantTransmissionDisplayValue>(
                                        static_cast<HUInt64>(ConstantTransmissionDisplayValue::DISP_S1) + privateGearStatus -
                                        static_cast<HUInt64>(Private_GearStatus::SPEED1));
                                } else {
                                    oldValue2secFlag = true;
                                }
                            }
                        } else {
                            if (getPrivateGearStatus() == Private_GearStatus::UNHANDLED_TIMEOUT || mGearCrcError == true) {
                                oldValue2secFlag = true;
                            }
                        }
                        break;
                    case Private_TransmissionGearSta::STAGE_D:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_D;  // for Others
                        if ((privateGearStatus >= static_cast<HUInt64>(Private_GearStatus::SPEED1)) &&
                            (privateGearStatus <= static_cast<HUInt64>(Private_GearStatus::SPEED8))) {
                            if ((mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N) ||
                                (mInter_ConfigHighPerformance != Inter_ConfigHighPerformance::N &&
                                 mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::DCT &&
                                 mInter_ConfigArea != Inter_ConfigArea::CAN && mInter_ConfigArea != Inter_ConfigArea::USA &&
                                 mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV) ||
                                (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                                 mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV)) {
                                if (mGearCrcError == false) {
                                    newConstantTransmissionDisplayValue = static_cast<ConstantTransmissionDisplayValue>(
                                        static_cast<HUInt64>(ConstantTransmissionDisplayValue::DISP_D1) + privateGearStatus -
                                        static_cast<HUInt64>(Private_GearStatus::SPEED1));
                                } else {
                                    oldValue2secFlag = true;
                                }
                            }
                        } else {
                            // TCU(or HDCT, HTCU)신호로 기어 표시 중 CAN Message단위로 timeout인가 시 SCU 신호로 기어표시를
                            // 하게되고, CAN Message 단위로 timeout처리되면서 Private_GearStatus가 timeout으로 인가되어
                            // D단(SCU)에서 기어가 미표시되는 현상 수정 (EXNCP-37483)
                            if ((getPrivateGearStatus() == Private_GearStatus::UNHANDLED_TIMEOUT &&
                                 mPrivate_TransmissionGearStaType != Private_TransmissionGearStaType::INPUT_GSS_SCU) ||
                                mGearCrcError == true) {
                                oldValue2secFlag = true;
                            }
                        }
                        break;
                    case Private_TransmissionGearSta::STAGE_M:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_M;  // for Others
                        if ((privateGearStatus >= static_cast<HUInt64>(Private_GearStatus::SPEED1)) &&
                            (privateGearStatus <= static_cast<HUInt64>(Private_GearStatus::SPEED8))) {
                            if (mGearCrcError == false) {
                                newConstantTransmissionDisplayValue = static_cast<ConstantTransmissionDisplayValue>(
                                    static_cast<HUInt64>(ConstantTransmissionDisplayValue::DISP_M1) + privateGearStatus -
                                    static_cast<HUInt64>(Private_GearStatus::SPEED1));
                            } else {
                                oldValue2secFlag = true;
                            }
                        } else {
                            if (getPrivateGearStatus() == Private_GearStatus::UNHANDLED_TIMEOUT || mGearCrcError == true) {
                                oldValue2secFlag = true;
                            }
                        }
                        break;
                    case Private_TransmissionGearSta::INTERMEDIATE:
                        mIsGearChangedFromIgnOn = true;
                        newConstantTransmissionDisplayValue = mConstantTransmissionDisplayValue;  // OLD_VALUE
                        break;
                    default:
                        if (mIsGearChangedFromIgnOn == true) {
                            oldValue2secFlag = true;
                        }
                        break;
                }
            }
        } else if (mIgnitionState == IgnitionState::OFF) {
            mIsGearChangedFromIgnOn = false;
            newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISPLAY_OFF;  // for Others
            if (mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                mPrivate_TransmissionDisplayOffReq == Private_TransmissionDisplayOffReq::DISPLAY_ON) {
                if (mSbwParkNeutralStatus == SbwParkNeutralStatus::PARK) {
                    newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_P;
                } else if (mSbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL) {
                    newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_N;
                } else {
                    Private_TransmissionGearSta privateTransmissionGearSta = getPrivateTransmissionGearSta();
                    switch (privateTransmissionGearSta) {
                        case Private_TransmissionGearSta::STAGE_P:
                            newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_P;
                            break;
                        case Private_TransmissionGearSta::STAGE_D:
                            newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_D;
                            break;
                        case Private_TransmissionGearSta::STAGE_N:
                            newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_N;
                            break;
                        case Private_TransmissionGearSta::STAGE_R:
                            newConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_R;
                            break;
                        default:
                            break;
                    }
                }
            }
        } else {
            // nothing
        }

        // This condition case for OLDVALUE_2S_OFF
        if (oldValue2secFlag == true) {
            if (mOneShotTimerConstant2sTimeout.isRunning() == false) {
                if (mOneShotTimerConstant2sTimeout.start() == false) {
                    DWarning() << "[Transmission_Indicator] mOneShotTimerConstant2sTimeout Start Failed";
                }
            }
        } else {
            if (mOneShotTimerConstant2sTimeout.isRunning() == true) {
                mOneShotTimerConstant2sTimeout.stop();
            }
            mConstantTransmissionDisplayValue = newConstantTransmissionDisplayValue;
            setSFCTransmission_IndicatorConstantTransmissionDisplayValue(newConstantTransmissionDisplayValue);
            flush();
        }
    }

    // 5.2.2.1
    void updateConstantTransmissionGSIStatValueMT() {
        mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_DISPLAY_OFF;            // for Others
        mConstantTransmissionGSITargetValue = ConstantTransmissionGSITargetValue::DISPLAY_OFF;  // for Others

        if (mIgnitionState == IgnitionState::ON) {
            HBool private_TarGearStepFlag =
                (static_cast<HUInt64>(mPrivate_TarGearStep) >= static_cast<HUInt64>(Private_TarGearStep::SPEED1) &&
                 static_cast<HUInt64>(mPrivate_TarGearStep) <= static_cast<HUInt64>(Private_TarGearStep::SPEED8));
            HInt64 diffSpeedValue = static_cast<HUInt64>(ConstantTransmissionGSITargetValue::GSI_TARGET_1) -
                                    static_cast<HUInt64>(Private_TarGearStep::SPEED1);
            HUInt64 speedValue = static_cast<HUInt64>(mPrivate_TarGearStep);
            if (private_TarGearStepFlag) {
                if (mPrivate_TarGearUp == static_cast<HUInt64>(Private_TarGearType::ON) &&
                    mPrivate_TarGearDown == static_cast<HUInt64>(Private_TarGearType::OFF)) {
                    mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_UP;
                    mConstantTransmissionGSITargetValue =
                        static_cast<ConstantTransmissionGSITargetValue>(speedValue + diffSpeedValue);
                } else if (mPrivate_TarGearUp == static_cast<HUInt64>(Private_TarGearType::OFF) &&
                           mPrivate_TarGearDown == static_cast<HUInt64>(Private_TarGearType::ON)) {
                    mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::GSI_DOWN;
                    mConstantTransmissionGSITargetValue =
                        static_cast<ConstantTransmissionGSITargetValue>(speedValue + diffSpeedValue);
                } else {
                    // nothing
                }
            }
        }

        setSFCTransmission_IndicatorConstantTransmissionGSIStat(mConstantTransmissionGSIStat);
        setSFCTransmission_IndicatorConstantTransmissionGSITargetValue(mConstantTransmissionGSITargetValue);
    }

    // 5.2.2.2
    void updateConstantTransmissionDisplayValueMT() {
        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISPLAY_OFF;  // for Others

        if (mIgnitionState == IgnitionState::ON) {
            HBool input_GearIndManualReqStatusOnIMTFlag =
                (static_cast<HUInt64>(mGearIndManualReqStatusOnIMT) >=
                     static_cast<HUInt64>(GearIndManualReqStatusOnIMT::SPEED1) &&
                 static_cast<HUInt64>(mGearIndManualReqStatusOnIMT) <= static_cast<HUInt64>(GearIndManualReqStatusOnIMT::SPEED6));
            if (mGearPositionRStatusFromICU == GearPositionRStatusFromICU::STAGE_R ||
                mGearSelectStatusFromTCU == GearSelectStatusFromTCU::STAGE_R) {
                mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_R;
            } else if (mTransmissionEolErrorWarnStatusFromIMT == TransmissionEolErrorWarnStatusFromIMT::NO_WARN &&
                       (mGearIndiDisplayReqStatusFromIMT == GearIndiDisplayReqStatusFromIMT::ON ||
                        mGearIndiDisplayReqStatusFromIMT == GearIndiDisplayReqStatusFromIMT::BLINK)) {
                if (mGearSelectStatusFromTCU == GearSelectStatusFromTCU::STAGE_D && input_GearIndManualReqStatusOnIMTFlag) {
                    mConstantTransmissionDisplayValue = static_cast<ConstantTransmissionDisplayValue>(
                        static_cast<HUInt64>(ConstantTransmissionDisplayValue::SPEED1) +
                        static_cast<HUInt64>(mGearIndManualReqStatusOnIMT) -
                        static_cast<HUInt64>(GearIndManualReqStatusOnIMT::SPEED1));
                } else if (mGearSelectStatusFromTCU == GearSelectStatusFromTCU::STAGE_N &&
                           mGearIndManualReqStatusOnIMT == GearIndManualReqStatusOnIMT::STAGE_N) {
                    mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_N;
                } else {
                    // nothing
                }
            } else if (mTransmissionEolErrorWarnStatusFromIMT == TransmissionEolErrorWarnStatusFromIMT::DISPLAY_E_REQ) {
                mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_E;
            } else {
                // nothing
            }
        }

        setSFCTransmission_IndicatorConstantTransmissionDisplayValue(mConstantTransmissionDisplayValue);
    }

    // 5.2.3.2
    void updateConstantTransmissionDisplayValueMTN() {
        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISPLAY_OFF;

        if (mIgnitionState == IgnitionState::ON) {
            if (mGearPositionRStatusFromICU == GearPositionRStatusFromICU::STAGE_R ||
                mGearPositionRStatusFromEMS == GearPositionRStatusFromEMS::STAGE_R) {
                mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_R;
            } else {
                switch (mGearIndManualReqStatusOnMT) {
                    case GearIndManualReqStatusOnMT::STAGE_N:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISP_N;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED1:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED1;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED2:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED2;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED3:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED3;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED4:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED4;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED5:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED5;
                        break;
                    case GearIndManualReqStatusOnMT::SPEED6:
                        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::SPEED6;
                        break;
                    default:
                        break;
                }
            }
        }

        setSFCTransmission_IndicatorConstantTransmissionDisplayValue(mConstantTransmissionDisplayValue);
    }

    // 5.2.4
    void updateConstantTransmissionBlinkingStat() {
        if (mIgnitionState == IgnitionState::ON) {
            setSFCTransmission_IndicatorConstantTransmissionBlinkingBlinkValueA(
                static_cast<ConstantTransmissionBlinkingBlinkValueA>(mConstantTransmissionDisplayValue));
            if (mPrivate_TransmissionBlinking_Stat == Private_TransmissionBlinking_Stat::ON) {
                setSFCTransmission_IndicatorConstantTransmissionBlinkingStat(ConstantTransmissionBlinkingStat::BLINK2);
                setSFCTransmission_IndicatorConstantTransmissionBlinkingBlinkValueB(
                    ConstantTransmissionBlinkingBlinkValueB::DISPLAY_OFF);
            } else {
                setSFCTransmission_IndicatorConstantTransmissionBlinkingStat(ConstantTransmissionBlinkingStat::VALUE);
            }
        } else {
            setSFCTransmission_IndicatorConstantTransmissionBlinkingBlinkValueA(
                static_cast<ConstantTransmissionBlinkingBlinkValueA>(mConstantTransmissionDisplayValue));
            setSFCTransmission_IndicatorConstantTransmissionBlinkingStat(ConstantTransmissionBlinkingStat::VALUE);
        }
    }

    // 5.2.6
    void updateConstantTransmissionLayoutStat() {
        if ((mInter_ConfigTransmissionType == Inter_ConfigTransmissionType::AT &&
             (mInter_ConfigHighPerformance != Inter_ConfigHighPerformance::N &&
              mInter_ConfigHighPerformance != Inter_ConfigHighPerformance::GT) &&
             (mInter_ConfigArea == Inter_ConfigArea::EEC || mInter_ConfigArea == Inter_ConfigArea::RUSSIA)) ||
            (mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::IMT_3PEDAL ||
             mInter_ConfigTransmissionDetailType == Inter_ConfigTransmissionDetailType::IMT_2PEDAL) ||
            ((mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N ||
              mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::GT) &&
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV)) {
            setSFCTransmission_IndicatorConstantTransmissionLayoutStat(
                SFCTransmission_IndicatorConstantTransmissionLayoutStat::TOGETHER);
        } else if ((mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N ||
                    mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::GT) &&
                   mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) {
            setSFCTransmission_IndicatorConstantTransmissionLayoutStat(
                SFCTransmission_IndicatorConstantTransmissionLayoutStat::WITHAGI);
        } else {
            setSFCTransmission_IndicatorConstantTransmissionLayoutStat(
                SFCTransmission_IndicatorConstantTransmissionLayoutStat::ONLYONE);
        }
    }

    // 5.2.7
    void updateConstantTransmissionPRNDfocusStat() {
        static SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat focusStat =
            SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::NONE;

        switch (getPrivateTransmissionGearSta()) {
            case Private_TransmissionGearSta::STAGE_P:
                focusStat = SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::FOCUS_P;
                break;
            case Private_TransmissionGearSta::STAGE_N:
                focusStat = SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::FOCUS_N;
                break;
            case Private_TransmissionGearSta::STAGE_R:
                focusStat = SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::FOCUS_R;
                break;
            case Private_TransmissionGearSta::STAGE_D:
                focusStat = SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::FOCUS_D;
                break;
            case Private_TransmissionGearSta::STAGE_M:
                focusStat = SFCTransmission_IndicatorConstantTransmissionPRNDfocusStat::FOCUS_M;
                break;
            default:
                break;
        }
        setSFCTransmission_IndicatorConstantTransmissionPRNDfocusStat(focusStat);
    }

    // 5.3.1
    void updateEventTransmissionIndicator() {
        HBool forcedEventOff = false;
        Private_TransmissionGearSta mCurrPrivate_TransmissionGearSta = getPrivateTransmissionGearSta();

        if (mInter_ConfigSBW == Inter_ConfigSBW::ON) {
            if ((mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
                 mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV &&
                 mInter_ConfigVehicleType != Inter_ConfigVehicleType::EREV &&
                 (isCheckmPrevPrivate_TransmissionGearSta() == true &&
                  mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_P) &&
                 mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P) ||
                (mIgnitionState == IgnitionState::OFF && mInter_ConfigVehicleType != Inter_ConfigVehicleType::EV &&
                 mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV &&
                 mInter_ConfigVehicleType != Inter_ConfigVehicleType::EREV &&
                 mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL &&
                 mSbwParkNeutralStatus == SbwParkNeutralStatus::PARK)) {
                mEventID = "E24601";
                if (mOneShotTimerEvent300msTimeout.isRunning() == true) {
                    mOneShotTimerEvent300msTimeout.stop();
                }
                if (mOneShotTimerEvent300msTimeout.isRunning() == false) {
                    if (mOneShotTimerEvent300msTimeout.start() == false) {
                        DWarning() << "[Transmission_Indicator] mOneShotTimerEvent300msTimeout Start Failed";
                    }
                }
            } else if (mIgnitionState == IgnitionState::OFF &&
                       (((isCheckmPrevPrivate_TransmissionGearSta() == true &&
                          mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_N) &&
                         mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N) ||
                        (mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::PARK &&
                         mSbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL))) {
                mEventID = "E24602";
                if (mOneShotTimerEvent300msTimeout.isRunning() == true) {
                    mOneShotTimerEvent300msTimeout.stop();
                }
                if (mOneShotTimerEvent300msTimeout.isRunning() == false) {
                    if (mOneShotTimerEvent300msTimeout.start() == false) {
                        DWarning() << "[Transmission_Indicator] mOneShotTimerEvent300msTimeout Start Failed";
                    }
                }
            } else if (((mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P &&
                         mCurrPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_P) ||
                        (mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::PARK &&
                         mSbwParkNeutralStatus != SbwParkNeutralStatus::PARK)) ||
                       ((mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N &&
                         mCurrPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_N) ||
                        (mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL &&
                         mSbwParkNeutralStatus != SbwParkNeutralStatus::NEUTRAL))) {
                forcedEventOff = true;
                mEventID = "";
            } else {
                // nothing
            }
        }
        if (forcedEventOff == true) {
            if (mOneShotTimerEvent300msTimeout.isRunning() == true) {
                mOneShotTimerEvent300msTimeout.stop();
            }
            updateEvent(mEventID);
        }
    }

    // 5.3.2
    void updateEventTransmissionPaddleShift() {
        HUInt64 privateGearStatus = static_cast<HUInt64>(getPrivateGearStatus());

        if (mIgnitionState == IgnitionState::ON &&
            (mPrivate_MaxStepTransmission_Stat != Private_MaxStepTransmission_Stat::OFF &&
             getPrivateTransmissionGearSta() == Private_TransmissionGearSta::PADDLE_SHIFT &&
             ((privateGearStatus >= static_cast<HUInt64>(Private_GearStatus::SPEED1)) &&
              (privateGearStatus <= static_cast<HUInt64>(Private_GearStatus::SPEED8))))) {
            if (mOneShotTimerEventPaddleShift.restart() == false) {
                DWarning() << "[Transmission_Indicator] mOneShotTimerEventPaddleShift Restart Failed";
            }
        } else {
            mPrevPrivate_GearStatus = Private_GearStatus::NONE;
            mOneShotTimerEventPaddleShift.stop();
            // Conditions are not satisfied, Event Closed
            setSFCTransmission_IndicatorEventTransmission_PaddleShiftStat(
                SFCTransmission_IndicatorEventTransmission_PaddleShiftStat::OFF);
        }
    }

    // 5.3.3
    void updateEventTransmissionSBW() {
        std::string prevEventID;
        std::string eventID;
        SFCTransmission_IndicatorEventTransmission_SBWStat stat{SFCTransmission_IndicatorEventTransmission_SBWStat::OFF};

        if (mIgnitionState == IgnitionState::ON &&
            (mSbwTypeStatus == SbwTypeStatus::COLUMN || mSbwTypeStatus == SbwTypeStatus::COLUMN_LEVER_R ||
             mSbwTypeStatus == SbwTypeStatus::COLUMN_LEVER_L)) {
            switch (getPrivateTransmissionGearSta()) {
                case Private_TransmissionGearSta::STAGE_P:
                    if (isCheckmPrevPrivate_TransmissionGearSta() == true &&
                        mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_P) {
                        stat = SFCTransmission_IndicatorEventTransmission_SBWStat::ON;
                    }
                    break;
                case Private_TransmissionGearSta::STAGE_R:
                    if (isCheckmPrevPrivate_TransmissionGearSta() == true &&
                        mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_R) {
                        stat = SFCTransmission_IndicatorEventTransmission_SBWStat::ON;
                    }
                    break;
                case Private_TransmissionGearSta::STAGE_N:
                    if (isCheckmPrevPrivate_TransmissionGearSta() == true &&
                        mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_N) {
                        stat = SFCTransmission_IndicatorEventTransmission_SBWStat::ON;
                    }
                    break;
                case Private_TransmissionGearSta::STAGE_D:
                    if ((isCheckmPrevPrivate_TransmissionGearSta() == true &&
                         (mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_M))) {
                        stat = SFCTransmission_IndicatorEventTransmission_SBWStat::ON;
                    }
                    break;
                case Private_TransmissionGearSta::STAGE_M:
                    if ((isCheckmPrevPrivate_TransmissionGearSta() == true &&
                         (mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_D ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::MANUAL ||
                          mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::PADDLE_SHIFT))) {
                        stat = SFCTransmission_IndicatorEventTransmission_SBWStat::ON;
                    }
                    break;
                default:
                    break;
            }
        }

        if (stat == SFCTransmission_IndicatorEventTransmission_SBWStat::ON) {
            if (mSbwTypeStatus == SbwTypeStatus::COLUMN) {
                eventID = "E24604";
            } else if (mSbwTypeStatus == SbwTypeStatus::COLUMN_LEVER_R) {
                eventID = "E24605";
            } else if (mSbwTypeStatus == SbwTypeStatus::COLUMN_LEVER_L) {
                eventID = "E24606";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.Transmission_Indicator.Event.Transmission_SBW.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTransmission_IndicatorEventTransmission_SBWStat(SFCTransmission_IndicatorEventTransmission_SBWStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTransmission_IndicatorEventTransmission_SBWID(eventID);
            setSFCTransmission_IndicatorEventTransmission_SBWStat(SFCTransmission_IndicatorEventTransmission_SBWStat::ON);
        }
    }

    // 5.4
    void updateSoundTransmissionIndicator() {
        Private_TransmissionGearSta mCurrPrivate_TransmissionGearSta = getPrivateTransmissionGearSta();

        if ((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
             (isCheckmPrevPrivate_TransmissionGearSta() == true &&
              mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_P) &&
             mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P) ||
            (mInter_ConfigSBW == Inter_ConfigSBW::ON && mIgnitionState == IgnitionState::OFF &&
             mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL &&
             mSbwParkNeutralStatus == SbwParkNeutralStatus::PARK)) {
            setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                SFCTransmission_IndicatorSoundTransmission_IndicatorStat::OFF);
            mSoundID = "SND_SBWPPosition";
            mOneShotTimerSound300msTimeout.restart();
        } else if ((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                    (isCheckmPrevPrivate_TransmissionGearSta() == true &&
                     mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_N) &&
                    mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N) ||
                   (mInter_ConfigSBW == Inter_ConfigSBW::ON && mIgnitionState == IgnitionState::OFF &&
                    mPrev_SbwParkNeutralStatus == SbwParkNeutralStatus::PARK &&
                    mSbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL)) {
            setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                SFCTransmission_IndicatorSoundTransmission_IndicatorStat::OFF);
            mSoundID = "SND_SBWBtnNPosition";
            mOneShotTimerSound300msTimeout.restart();
        } else if (mIgnitionState == IgnitionState::ON) {
            if (mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                ((isCheckmPrevPrivate_TransmissionGearSta() == true) &&
                 (mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P ||
                  mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R ||
                  mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N ||
                  mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_M)) &&
                mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_D) {
                setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                    SFCTransmission_IndicatorSoundTransmission_IndicatorStat::OFF);
                mSoundID = "SND_SBWBtnDPosition";
                mOneShotTimerSound300msTimeout.restart();
            } else if (mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                       ((isCheckmPrevPrivate_TransmissionGearSta() == true) &&
                        (mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P ||
                         mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R ||
                         mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N ||
                         mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_D ||
                         mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::MANUAL ||
                         mPrevPrivate_TransmissionGearSta == Private_TransmissionGearSta::PADDLE_SHIFT)) &&
                       mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_M) {
                setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                    SFCTransmission_IndicatorSoundTransmission_IndicatorStat::OFF);
                mSoundID = "SND_SBWBtnDPosition";
                mOneShotTimerSound300msTimeout.restart();
            } else if (((isCheckmPrevPrivate_TransmissionGearSta() == true &&
                         mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::STAGE_R) &&
                        mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R) ||
                       (mPrev_GearPositionRStatusFromICU == GearPositionRStatusFromICU::OFF &&
                        mGearPositionRStatusFromICU == GearPositionRStatusFromICU::STAGE_R) ||
                       (mPrev_GearPositionRStatusFromEMS == GearPositionRStatusFromEMS::OFF &&
                        mGearPositionRStatusFromEMS == GearPositionRStatusFromEMS::STAGE_R)) {
                setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                    SFCTransmission_IndicatorSoundTransmission_IndicatorStat::OFF);
                mSoundID = "SND_RPosition";
                mOneShotTimerSound300msTimeout.restart();
            } else {
                // nothing
            }
        } else {
            // nothing
        }
    }

    void updateEvent(const std::string& eventID) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Transmission_Indicator.Event.Transmission_Indicator.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTransmission_IndicatorEventTransmission_IndicatorStat(
                SFCTransmission_IndicatorEventTransmission_IndicatorStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTransmission_IndicatorEventTransmission_IndicatorID(eventID);
            setSFCTransmission_IndicatorEventTransmission_IndicatorStat(
                SFCTransmission_IndicatorEventTransmission_IndicatorStat::ON);
        }
    }

    void updateSound(const std::string& soundID) {
        if (soundID.empty() == false) {
            setSFCTransmission_IndicatorSoundTransmission_IndicatorID(soundID);
            setSFCTransmission_IndicatorSoundTransmission_IndicatorStat(
                SFCTransmission_IndicatorSoundTransmission_IndicatorStat::ON);
            setSFCTransmission_IndicatorSoundTransmission_IndicatorType(
                SFCTransmission_IndicatorSoundTransmission_IndicatorType::REPEAT_COUNT);
            setSFCTransmission_IndicatorSoundTransmission_IndicatorRepeatCount(1);
        }
    }

    void updatePrivateTransmissionDisplayOffReq() {
        if (mIgnitionState == IgnitionState::OFF) {
            if ((mPrev_DoorPassiveLockReqStatus != DoorPassiveLockReqStatus::LOCK_REQ &&
                 mDoorPassiveLockReqStatus == DoorPassiveLockReqStatus::LOCK_REQ) ||
                (mPrev_DoorRKELockUnlockReqStatus != DoorRKELockUnlockReqStatus::LOCK_REQ &&
                 mDoorRKELockUnlockReqStatus == DoorRKELockUnlockReqStatus::LOCK_REQ)) {
                mPrivate_TransmissionDisplayOffReq = Private_TransmissionDisplayOffReq::DISPLAY_OFF;
            }
        } else {
            mPrivate_TransmissionDisplayOffReq = Private_TransmissionDisplayOffReq::DISPLAY_ON;
        }
    }

    void updatePrivateTransmissionDisplayForAgivalue() {
        mPrivateTransmissionDisplayForAgivalue = 0;

        if (mIgnitionState == IgnitionState::ON) {
            switch (mConstantTransmissionDisplayValue) {
                case ConstantTransmissionDisplayValue::SPEED1:
                    mPrivateTransmissionDisplayForAgivalue = 1;
                    break;
                case ConstantTransmissionDisplayValue::SPEED2:
                    mPrivateTransmissionDisplayForAgivalue = 2;
                    break;
                case ConstantTransmissionDisplayValue::SPEED3:
                    mPrivateTransmissionDisplayForAgivalue = 3;
                    break;
                case ConstantTransmissionDisplayValue::SPEED4:
                    mPrivateTransmissionDisplayForAgivalue = 4;
                    break;
                case ConstantTransmissionDisplayValue::SPEED5:
                    mPrivateTransmissionDisplayForAgivalue = 5;
                    break;
                case ConstantTransmissionDisplayValue::SPEED6:
                    mPrivateTransmissionDisplayForAgivalue = 6;
                    break;
                case ConstantTransmissionDisplayValue::SPEED7:
                    mPrivateTransmissionDisplayForAgivalue = 7;
                    break;
                case ConstantTransmissionDisplayValue::SPEED8:
                    mPrivateTransmissionDisplayForAgivalue = 8;
                    break;
                default:
                    // no operation
                    break;
            }
        }
    }

    void updatePrivateTransmissionAgiUpDownValue() {
        mPrivateTransmissionAgiUpValue = 0;
        mPrivateTransmissionAgiDownValue = 0;

        if (mIgnitionState == IgnitionState::ON) {
            switch (mTransmissionAgiUpStatus) {
                case TransmissionAgiUpStatus::SPEED1:
                    mPrivateTransmissionAgiUpValue = 1;
                    break;
                case TransmissionAgiUpStatus::SPEED2:
                    mPrivateTransmissionAgiUpValue = 2;
                    break;
                case TransmissionAgiUpStatus::SPEED3:
                    mPrivateTransmissionAgiUpValue = 3;
                    break;
                case TransmissionAgiUpStatus::SPEED4:
                    mPrivateTransmissionAgiUpValue = 4;
                    break;
                case TransmissionAgiUpStatus::SPEED5:
                    mPrivateTransmissionAgiUpValue = 5;
                    break;
                case TransmissionAgiUpStatus::SPEED6:
                    mPrivateTransmissionAgiUpValue = 6;
                    break;
                case TransmissionAgiUpStatus::SPEED7:
                    mPrivateTransmissionAgiUpValue = 7;
                    break;
                case TransmissionAgiUpStatus::SPEED8:
                    mPrivateTransmissionAgiUpValue = 8;
                    break;
                default:
                    // no operation
                    break;
            }
            switch (mTransmissionAgiDownStatus) {
                case TransmissionAgiDownStatus::SPEED1:
                    mPrivateTransmissionAgiDownValue = 1;
                    break;
                case TransmissionAgiDownStatus::SPEED2:
                    mPrivateTransmissionAgiDownValue = 2;
                    break;
                case TransmissionAgiDownStatus::SPEED3:
                    mPrivateTransmissionAgiDownValue = 3;
                    break;
                case TransmissionAgiDownStatus::SPEED4:
                    mPrivateTransmissionAgiDownValue = 4;
                    break;
                case TransmissionAgiDownStatus::SPEED5:
                    mPrivateTransmissionAgiDownValue = 5;
                    break;
                case TransmissionAgiDownStatus::SPEED6:
                    mPrivateTransmissionAgiDownValue = 6;
                    break;
                case TransmissionAgiDownStatus::SPEED7:
                    mPrivateTransmissionAgiDownValue = 7;
                    break;
                case TransmissionAgiDownStatus::SPEED8:
                    mPrivateTransmissionAgiDownValue = 8;
                    break;
                default:
                    // no operation
                    break;
            }
        }
    }

    void updatePrivateAgiDisplayOnOffStat() {
        // Private_AgiDisplayOnOff_Stat
        mPrivate_AgiDisplayOnOff_Stat = Private_AgiDisplayOnOff_Stat::OFF;

        if (mIgnitionState == IgnitionState::ON &&
            ((mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::N ||
              mInter_ConfigHighPerformance == Inter_ConfigHighPerformance::GT) &&
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV) &&
            mPrivateTransmissionDisplayForAgivalue != 0 &&
            mPrivate_MaxStepTransmissionValue >= mPrivateTransmissionDisplayForAgivalue) {
            mPrivate_AgiDisplayOnOff_Stat = Private_AgiDisplayOnOff_Stat::ON;
        }
    }

    void updatePrivateAgiUpDownAvailable_Stat() {
        mPrivate_AgiUpAvailable_Stat = Private_AgiAvailable_Stat::DISPLAY_OFF;
        mPrivate_AgiDownAvailable_Stat = Private_AgiAvailable_Stat::DISPLAY_OFF;

        // Private_AgiUpAvailable_Stat
        if (mIgnitionState == IgnitionState::ON && mPrivate_AgiDisplayOnOff_Stat == Private_AgiDisplayOnOff_Stat::ON &&
            mPrivateTransmissionAgiUpValue != 0) {
            if (mPrivateTransmissionAgiUpValue > mPrivateTransmissionDisplayForAgivalue) {
                mPrivate_AgiUpAvailable_Stat = Private_AgiAvailable_Stat::AVAILABLE;
            } else if (mPrivateTransmissionAgiUpValue <= mPrivateTransmissionDisplayForAgivalue &&
                       mPrivate_MaxStepTransmissionValue > mPrivateTransmissionDisplayForAgivalue) {
                mPrivate_AgiUpAvailable_Stat = Private_AgiAvailable_Stat::UNAVAILABLE;
            } else {
                // no operation
            }
        }

        // Private_AgiDownAvailable_Stat
        if (mIgnitionState == IgnitionState::ON && mPrivate_AgiDisplayOnOff_Stat == Private_AgiDisplayOnOff_Stat::ON &&
            mPrivateTransmissionAgiDownValue != 0) {
            if (mPrivateTransmissionAgiDownValue < mPrivateTransmissionDisplayForAgivalue) {
                mPrivate_AgiDownAvailable_Stat = Private_AgiAvailable_Stat::AVAILABLE;
            } else if (mPrivateTransmissionAgiDownValue >= mPrivateTransmissionDisplayForAgivalue) {
                mPrivate_AgiDownAvailable_Stat = Private_AgiAvailable_Stat::UNAVAILABLE;
            } else {
                // no operation
            }
        }
    }

    void updateTransmissionAgiStat() {
        // 5.8.5.1.3 변속가능단 표시(Available Gear Indicator)
        // IMG_TransmissionAgiUp_stat
        SFCTransmission_IndicatorConstantTransmissionAgiUpStat upStat =
            SFCTransmission_IndicatorConstantTransmissionAgiUpStat::DISPLAY_OFF;
        if (mIgnitionState == IgnitionState::ON && mPrivate_AgiUpAvailable_Stat == Private_AgiAvailable_Stat::AVAILABLE) {
            if (mPrivateTransmissionDisplayForAgivalue >= 1 && mPrivateTransmissionDisplayForAgivalue <= 7) {
                upStat = static_cast<SFCTransmission_IndicatorConstantTransmissionAgiUpStat>(
                    (mPrivateTransmissionDisplayForAgivalue - 1) +
                    static_cast<HUInt64>(SFCTransmission_IndicatorConstantTransmissionAgiUpStat::AVAILABLE_2));
                // AVAILABLE_2 ~ AVAILABLE_8
            }
        } else if (mIgnitionState == IgnitionState::ON &&
                   mPrivate_AgiUpAvailable_Stat == Private_AgiAvailable_Stat::UNAVAILABLE) {
            if (mPrivateTransmissionDisplayForAgivalue >= 1 && mPrivateTransmissionDisplayForAgivalue <= 7) {
                upStat = static_cast<SFCTransmission_IndicatorConstantTransmissionAgiUpStat>(
                    (mPrivateTransmissionDisplayForAgivalue - 1) +
                    static_cast<HUInt64>(SFCTransmission_IndicatorConstantTransmissionAgiUpStat::UNAVAILABLE_2));
                // UNAVAILABLE_2 ~ UNAVAILABLE_8
            }
        }

        // IMG_TransmissionAgiDown_stat
        SFCTransmission_IndicatorConstantTransmissionAgiDownStat downStat =
            SFCTransmission_IndicatorConstantTransmissionAgiDownStat::DISPLAY_OFF;
        if (mIgnitionState == IgnitionState::ON && mPrivate_AgiDownAvailable_Stat == Private_AgiAvailable_Stat::AVAILABLE) {
            if (mPrivateTransmissionDisplayForAgivalue >= 2 && mPrivateTransmissionDisplayForAgivalue <= 8) {
                downStat = static_cast<SFCTransmission_IndicatorConstantTransmissionAgiDownStat>(
                    (mPrivateTransmissionDisplayForAgivalue - 2) +
                    static_cast<HUInt64>(SFCTransmission_IndicatorConstantTransmissionAgiUpStat::AVAILABLE_1));
                // AVAILABLE_1 ~ AVAILABLE_7
            }
        } else if (mIgnitionState == IgnitionState::ON &&
                   mPrivate_AgiDownAvailable_Stat == Private_AgiAvailable_Stat::UNAVAILABLE) {
            if (mPrivateTransmissionDisplayForAgivalue >= 2 && mPrivateTransmissionDisplayForAgivalue <= 8) {
                downStat = static_cast<SFCTransmission_IndicatorConstantTransmissionAgiDownStat>(
                    (mPrivateTransmissionDisplayForAgivalue - 2) +
                    static_cast<HUInt64>(SFCTransmission_IndicatorConstantTransmissionAgiUpStat::UNAVAILABLE_1));
                // UNAVAILABLE_1 ~ UNAVAILABLE_7
            }
        }
        setSFCTransmission_IndicatorConstantTransmissionAgiUpStat(upStat);
        setSFCTransmission_IndicatorConstantTransmissionAgiDownStat(downStat);
    }

    void onOneShotTimerIgnOn300msTimeout() {
        DDebug() << "onOneShotTimerIgnOn300msTimeout()";
        mIgnitionElapsed = IgnitionElapsed::ON_300ms;
        updateOutputGearIndicatorStatus();
        flushActuatorOutput();
    }

    void onOneShotTimerSound300msTimeout() {
        DDebug() << "onOneShotTimerSound300msTimeout()";
        Private_TransmissionGearSta mCurrPrivate_TransmissionGearSta = getPrivateTransmissionGearSta();
        HBool isUpdate = false;

        if (((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
              mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_P) ||
             (mInter_ConfigSBW == Inter_ConfigSBW::ON && mIgnitionState == IgnitionState::OFF &&
              mSbwParkNeutralStatus == SbwParkNeutralStatus::PARK)) &&
            mSoundID == "SND_SBWPPosition") {
            isUpdate = true;
        } else if (((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                     mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_N) ||
                    (mInter_ConfigSBW == Inter_ConfigSBW::ON && mIgnitionState == IgnitionState::OFF &&
                     mSbwParkNeutralStatus == SbwParkNeutralStatus::NEUTRAL)) &&
                   mSoundID == "SND_SBWBtnNPosition") {
            isUpdate = true;
        } else if (mIgnitionState == IgnitionState::ON) {
            if (((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                  mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_D) &&
                 mSoundID == "SND_SBWBtnDPosition") ||
                ((mInter_ConfigSBW == Inter_ConfigSBW::ON &&
                  mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_M) &&
                 mSoundID == "SND_SBWBtnDPosition")) {
                isUpdate = true;
            } else if ((mCurrPrivate_TransmissionGearSta == Private_TransmissionGearSta::STAGE_R ||
                        mGearPositionRStatusFromICU == GearPositionRStatusFromICU::STAGE_R ||
                        mGearPositionRStatusFromEMS == GearPositionRStatusFromEMS::STAGE_R) &&
                       mSoundID == "SND_RPosition") {
                isUpdate = true;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        if (isUpdate == true) {
            updateSound(mSoundID);
            flush();
        }
    }

    void onOneShotTimerEvent300msTimeout() {
        DDebug() << "onOneShotTimerEvent300msTimeout()";
        updateEvent(mEventID);
        flush();
    }

    void onOneShotTimerHWOutput2sTimeout() {
        DDebug() << "onOneShotTimerHWOutput2sTimeout()";
        mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::OFF;
        mPrev_Output_GearIndicatorStatus = mOutput_GearIndicatorStatus;
        updateActuatorValue(Vehicle.System.HardWire.Output_GearIndicatorStatus,
                            static_cast<HUInt64>(mOutput_GearIndicatorStatus));
        flushActuatorOutput();
    }

    void onOneShotTimerConstant2sTimeout() {
        DDebug() << "onOneShotTimerConstant2sTimeout()";
        mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::DISPLAY_OFF;
        setSFCTransmission_IndicatorConstantTransmissionDisplayValue(mConstantTransmissionDisplayValue);
        updatePrivateTransmissionBlinkingStat();
        updateConstantTransmissionBlinkingStat();
        updatePrivateTransmissionDisplayForAgivalue();
        updatePrivateAgiDisplayOnOffStat();
        updatePrivateAgiUpDownAvailable_Stat();
        updateTransmissionAgiStat();
        flush();
    }

    void onOneShotTimerEventPaddleShift() {
        DDebug() << "onOneShotTimerEventPaddleShift()";
        Private_TransmissionGearSta currPrivate_TransmissionGearSta = getPrivateTransmissionGearSta();
        if (currPrivate_TransmissionGearSta == Private_TransmissionGearSta::PADDLE_SHIFT) {
            if (mPrevPrivate_GearStatus != getPrivateGearStatus()) {
                setSFCTransmission_IndicatorEventTransmission_PaddleShiftStat(
                    SFCTransmission_IndicatorEventTransmission_PaddleShiftStat::ON);
                flushLastGroup(true);
                mPrevPrivate_GearStatus = getPrivateGearStatus();
            }
        } else {
            mPrevPrivate_GearStatus = Private_GearStatus::NONE;
        }
    }

    enum class Private_GearStatus : HUInt64;
    Private_GearStatus getPrivateGearStatus() const {
        Private_GearStatus retVal = Private_GearStatus::NONE;
        if (mPrivate_GearStatusType == Private_GearStatusType::INPUT_GIMRS_1_TCU) {
            retVal = getPrivateGearStatus1TCU();
        } else if (mPrivate_GearStatusType == Private_GearStatusType::INPUT_GIMRS_2_TCU) {
            retVal = getPrivateGearStatus2TCU();
        } else if (mPrivate_GearStatusType == Private_GearStatusType::INPUT_GIMRS_HDCT) {
            retVal = getPrivateGearStatusHDCT();
        } else if (mPrivate_GearStatusType == Private_GearStatusType::INPUT_GIMRS_ON_EV) {
            retVal = getPrivateGearStatusOnEV();
        } else {
            // NONE
        }
        return retVal;
    }

    Private_GearStatus getPrivateGearStatus1TCU() const {
        Private_GearStatus retVal = Private_GearStatus::NONE;
        switch (static_cast<GearIndManualReqStatus1FromTCU>(mPrivate_GearStatusValue)) {
            case GearIndManualReqStatus1FromTCU::OFF:
                retVal = Private_GearStatus::OFF;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED1:
                retVal = Private_GearStatus::SPEED1;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED2:
                retVal = Private_GearStatus::SPEED2;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED3:
                retVal = Private_GearStatus::SPEED3;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED4:
                retVal = Private_GearStatus::SPEED4;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED5:
                retVal = Private_GearStatus::SPEED5;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED6:
                retVal = Private_GearStatus::SPEED6;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED7:
                retVal = Private_GearStatus::SPEED7;
                break;
            case GearIndManualReqStatus1FromTCU::SPEED8:
                retVal = Private_GearStatus::SPEED8;
                break;
            case GearIndManualReqStatus1FromTCU::UNHANDLED_TIMEOUT:
                retVal = Private_GearStatus::UNHANDLED_TIMEOUT;
                break;
        }
        return retVal;
    }

    Private_GearStatus getPrivateGearStatus2TCU() const {
        Private_GearStatus retVal = Private_GearStatus::NONE;
        switch (static_cast<GearIndManualReqStatus2FromTCU>(mPrivate_GearStatusValue)) {
            case GearIndManualReqStatus2FromTCU::OFF:
                retVal = Private_GearStatus::OFF;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED1:
                retVal = Private_GearStatus::SPEED1;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED2:
                retVal = Private_GearStatus::SPEED2;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED3:
                retVal = Private_GearStatus::SPEED3;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED4:
                retVal = Private_GearStatus::SPEED4;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED5:
                retVal = Private_GearStatus::SPEED5;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED6:
                retVal = Private_GearStatus::SPEED6;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED7:
                retVal = Private_GearStatus::SPEED7;
                break;
            case GearIndManualReqStatus2FromTCU::SPEED8:
                retVal = Private_GearStatus::SPEED8;
                break;
            case GearIndManualReqStatus2FromTCU::UNHANDLED_TIMEOUT:
                retVal = Private_GearStatus::UNHANDLED_TIMEOUT;
                break;
        }
        return retVal;
    }

    Private_GearStatus getPrivateGearStatusHDCT() const {
        Private_GearStatus retVal = Private_GearStatus::NONE;
        switch (static_cast<GearIndManualReqStatusFromHDCT>(mPrivate_GearStatusValue)) {
            case GearIndManualReqStatusFromHDCT::OFF:
                retVal = Private_GearStatus::OFF;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED1:
                retVal = Private_GearStatus::SPEED1;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED2:
                retVal = Private_GearStatus::SPEED2;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED3:
                retVal = Private_GearStatus::SPEED3;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED4:
                retVal = Private_GearStatus::SPEED4;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED5:
                retVal = Private_GearStatus::SPEED5;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED6:
                retVal = Private_GearStatus::SPEED6;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED7:
                retVal = Private_GearStatus::SPEED7;
                break;
            case GearIndManualReqStatusFromHDCT::SPEED8:
                retVal = Private_GearStatus::SPEED8;
                break;
            case GearIndManualReqStatusFromHDCT::UNHANDLED_TIMEOUT:
                retVal = Private_GearStatus::UNHANDLED_TIMEOUT;
                break;
        }
        return retVal;
    }

    Private_GearStatus getPrivateGearStatusOnEV() const {
        Private_GearStatus retVal = Private_GearStatus::NONE;
        switch (static_cast<GearIndManualReqStatusOnEV>(mPrivate_GearStatusValue)) {
            case GearIndManualReqStatusOnEV::OFF:
                retVal = Private_GearStatus::OFF;
                break;
            case GearIndManualReqStatusOnEV::SPEED1:
                retVal = Private_GearStatus::SPEED1;
                break;
            case GearIndManualReqStatusOnEV::SPEED2:
                retVal = Private_GearStatus::SPEED2;
                break;
            case GearIndManualReqStatusOnEV::SPEED3:
                retVal = Private_GearStatus::SPEED3;
                break;
            case GearIndManualReqStatusOnEV::SPEED4:
                retVal = Private_GearStatus::SPEED4;
                break;
            case GearIndManualReqStatusOnEV::SPEED5:
                retVal = Private_GearStatus::SPEED5;
                break;
            case GearIndManualReqStatusOnEV::SPEED6:
                retVal = Private_GearStatus::SPEED6;
                break;
            case GearIndManualReqStatusOnEV::SPEED7:
                retVal = Private_GearStatus::SPEED7;
                break;
            case GearIndManualReqStatusOnEV::SPEED8:
                retVal = Private_GearStatus::SPEED8;
                break;
            case GearIndManualReqStatusOnEV::UNHANDLED_TIMEOUT:
                retVal = Private_GearStatus::UNHANDLED_TIMEOUT;
                break;
        }
        return retVal;
    }

    enum class Private_TransmissionGearSta : HUInt64;
    Private_TransmissionGearSta getPrivateTransmissionGearSta() const {
        Private_TransmissionGearSta retVal = Private_TransmissionGearSta::NONE;
        if (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_VCU) {
            retVal = getPrivateTransmissionGearStaVCUType();
        } else if (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_SCU) {
            retVal = getPrivateTransmissionGearStaSCUType();
        } else if (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_TCU) {
            retVal = getPrivateTransmissionGearStaTCUType();
        } else if (mPrivate_TransmissionGearStaType == Private_TransmissionGearStaType::INPUT_GSS_HDCT) {
            retVal = getPrivateTransmissionGearStaHDCTType();
        } else {
            // NONE
        }
        return retVal;
    }

    Private_TransmissionGearSta getPrivateTransmissionGearStaVCUType() const {
        Private_TransmissionGearSta retVal = Private_TransmissionGearSta::NONE;
        switch (static_cast<GearDisplayStatusFromVCU>(mPrivate_TransmissionGearStaValue)) {
            case GearDisplayStatusFromVCU::STAGE_P:
                retVal = Private_TransmissionGearSta::STAGE_P;
                break;
            case GearDisplayStatusFromVCU::STAGE_D:
                retVal = Private_TransmissionGearSta::STAGE_D;
                break;
            case GearDisplayStatusFromVCU::STAGE_N:
                retVal = Private_TransmissionGearSta::STAGE_N;
                break;
            case GearDisplayStatusFromVCU::STAGE_R:
                retVal = Private_TransmissionGearSta::STAGE_R;
                break;
            case GearDisplayStatusFromVCU::INVALID:
                retVal = Private_TransmissionGearSta::INVALID;
                break;
            case GearDisplayStatusFromVCU::PADDLE_SHIFT:
                retVal = Private_TransmissionGearSta::PADDLE_SHIFT;
                break;
            case GearDisplayStatusFromVCU::UNHANDLED_TIMEOUT:
                retVal = Private_TransmissionGearSta::MESSAGE_TIMEOUT;
                break;
            default:
                break;
        }
        return retVal;
    }

    Private_TransmissionGearSta getPrivateTransmissionGearStaSCUType() const {
        Private_TransmissionGearSta retVal = Private_TransmissionGearSta::NONE;
        switch (static_cast<GearSelectStatusFromSCU>(mPrivate_TransmissionGearStaValue)) {
            case GearSelectStatusFromSCU::STAGE_P:
                retVal = Private_TransmissionGearSta::STAGE_P;
                break;
            case GearSelectStatusFromSCU::STAGE_D:
                retVal = Private_TransmissionGearSta::STAGE_D;
                break;
            case GearSelectStatusFromSCU::STAGE_N:
                retVal = Private_TransmissionGearSta::STAGE_N;
                break;
            case GearSelectStatusFromSCU::STAGE_R:
                retVal = Private_TransmissionGearSta::STAGE_R;
                break;
            case GearSelectStatusFromSCU::INVALID:
                retVal = Private_TransmissionGearSta::INVALID;
                break;
            case GearSelectStatusFromSCU::STAGE_M:
                retVal = Private_TransmissionGearSta::STAGE_M;
                break;
            case GearSelectStatusFromSCU::UNHANDLED_TIMEOUT:
                retVal = Private_TransmissionGearSta::MESSAGE_TIMEOUT;
                break;
            default:
                break;
        }
        return retVal;
    }

    Private_TransmissionGearSta getPrivateTransmissionGearStaTCUType() const {
        Private_TransmissionGearSta retVal = Private_TransmissionGearSta::NONE;
        switch (static_cast<GearSelectStatusFromTCU>(mPrivate_TransmissionGearStaValue)) {
            case GearSelectStatusFromTCU::STAGE_P:
                retVal = Private_TransmissionGearSta::STAGE_P;
                break;
            case GearSelectStatusFromTCU::STAGE_L:
                retVal = Private_TransmissionGearSta::STAGE_L;
                break;
            case GearSelectStatusFromTCU::STAGE_2:
                retVal = Private_TransmissionGearSta::STAGE_2;
                break;
            case GearSelectStatusFromTCU::STAGE_3:
                retVal = Private_TransmissionGearSta::STAGE_3;
                break;
            case GearSelectStatusFromTCU::DS_MODE:
                retVal = Private_TransmissionGearSta::DS_MODE;
                break;
            case GearSelectStatusFromTCU::STAGE_D:
                retVal = Private_TransmissionGearSta::STAGE_D;
                break;
            case GearSelectStatusFromTCU::STAGE_N:
                retVal = Private_TransmissionGearSta::STAGE_N;
                break;
            case GearSelectStatusFromTCU::STAGE_R:
                retVal = Private_TransmissionGearSta::STAGE_R;
                break;
            case GearSelectStatusFromTCU::MANUAL:
                retVal = Private_TransmissionGearSta::MANUAL;
                break;
            case GearSelectStatusFromTCU::PADDLE_SHIFT:
                retVal = Private_TransmissionGearSta::PADDLE_SHIFT;
                break;
            case GearSelectStatusFromTCU::INTERMEDIATE:
                retVal = Private_TransmissionGearSta::INTERMEDIATE;
                break;
            case GearSelectStatusFromTCU::INVALID:
                retVal = Private_TransmissionGearSta::INVALID;
                break;
            case GearSelectStatusFromTCU::STAGE_M:
                retVal = Private_TransmissionGearSta::STAGE_M;
                break;
            case GearSelectStatusFromTCU::UNHANDLED_TIMEOUT:
                retVal = Private_TransmissionGearSta::MESSAGE_TIMEOUT;
                break;
            default:
                break;
        }
        return retVal;
    }

    Private_TransmissionGearSta getPrivateTransmissionGearStaHDCTType() const {
        Private_TransmissionGearSta retVal = Private_TransmissionGearSta::NONE;
        switch (static_cast<GearSelectStatusFromHDCT>(mPrivate_TransmissionGearStaValue)) {
            case GearSelectStatusFromHDCT::STAGE_P:
                retVal = Private_TransmissionGearSta::STAGE_P;
                break;
            case GearSelectStatusFromHDCT::STAGE_L:
                retVal = Private_TransmissionGearSta::STAGE_L;
                break;
            case GearSelectStatusFromHDCT::STAGE_2:
                retVal = Private_TransmissionGearSta::STAGE_2;
                break;
            case GearSelectStatusFromHDCT::STAGE_3:
                retVal = Private_TransmissionGearSta::STAGE_3;
                break;
            case GearSelectStatusFromHDCT::DS_MODE:
                retVal = Private_TransmissionGearSta::DS_MODE;
                break;
            case GearSelectStatusFromHDCT::STAGE_D:
                retVal = Private_TransmissionGearSta::STAGE_D;
                break;
            case GearSelectStatusFromHDCT::STAGE_N:
                retVal = Private_TransmissionGearSta::STAGE_N;
                break;
            case GearSelectStatusFromHDCT::STAGE_R:
                retVal = Private_TransmissionGearSta::STAGE_R;
                break;
            case GearSelectStatusFromHDCT::MANUAL:
                retVal = Private_TransmissionGearSta::MANUAL;
                break;
            case GearSelectStatusFromHDCT::PADDLE_SHIFT:
                retVal = Private_TransmissionGearSta::PADDLE_SHIFT;
                break;
            case GearSelectStatusFromHDCT::INTERMEDIATE:
                retVal = Private_TransmissionGearSta::INTERMEDIATE;
                break;
            case GearSelectStatusFromHDCT::INVALID:
                retVal = Private_TransmissionGearSta::INVALID;
                break;
            case GearSelectStatusFromHDCT::STAGE_M:
                retVal = Private_TransmissionGearSta::STAGE_M;
                break;
            case GearSelectStatusFromHDCT::UNHANDLED_TIMEOUT:
                retVal = Private_TransmissionGearSta::MESSAGE_TIMEOUT;
                break;
            default:
                break;
        }
        return retVal;
    }

    // 5.2.5 ICV
    enum class Private_MaxStepTransmission_Stat : HUInt64;
    Private_MaxStepTransmission_Stat getPrivate_MaxStepTranmissionHcu() const {
        Private_MaxStepTransmission_Stat retVal = Private_MaxStepTransmission_Stat::NONE;
        switch (mGearMaxStepInfoFromEmsHcu) {
            case GearMaxStepInfoFromEmsHcu::MAX_THREE:
                retVal = Private_MaxStepTransmission_Stat::MAX_THREE;
                break;
            case GearMaxStepInfoFromEmsHcu::MAX_FOUR:
                retVal = Private_MaxStepTransmission_Stat::MAX_FOUR;
                break;
            case GearMaxStepInfoFromEmsHcu::MAX_FIVE:
                retVal = Private_MaxStepTransmission_Stat::MAX_FIVE;
                break;
            case GearMaxStepInfoFromEmsHcu::MAX_SIX:
                retVal = Private_MaxStepTransmission_Stat::MAX_SIX;
                break;
            case GearMaxStepInfoFromEmsHcu::MAX_SEVEN:
                retVal = Private_MaxStepTransmission_Stat::MAX_SEVEN;
                break;
            case GearMaxStepInfoFromEmsHcu::MAX_EIGHT:
                retVal = Private_MaxStepTransmission_Stat::MAX_EIGHT;
                break;
            default:
                retVal = Private_MaxStepTransmission_Stat::OFF;
                break;
        }
        return retVal;
    }

    // 5.2.5 HEV/PHEV
    Private_MaxStepTransmission_Stat getPrivate_MaxStepTranmissionHDCT() const {
        Private_MaxStepTransmission_Stat retVal = Private_MaxStepTransmission_Stat::NONE;
        switch (mGearMaxStepInfoFromHDCT) {
            case GearMaxStepInfoFromHDCT::MAX_THREE:
                retVal = Private_MaxStepTransmission_Stat::MAX_THREE;
                break;
            case GearMaxStepInfoFromHDCT::MAX_FOUR:
                retVal = Private_MaxStepTransmission_Stat::MAX_FOUR;
                break;
            case GearMaxStepInfoFromHDCT::MAX_FIVE:
                retVal = Private_MaxStepTransmission_Stat::MAX_FIVE;
                break;
            case GearMaxStepInfoFromHDCT::MAX_SIX:
                retVal = Private_MaxStepTransmission_Stat::MAX_SIX;
                break;
            default:
                retVal = Private_MaxStepTransmission_Stat::OFF;
                break;
        }
        return retVal;
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

    void updatePrevPrivateTransmissionGearSta() {
        Private_TransmissionGearSta mCurrPrivate_TransmissionGearSta = getPrivateTransmissionGearSta();
        // INTERMEDIATE  is wire of SBW moving status signal. don't use it for prev private gear status
        if (mCurrPrivate_TransmissionGearSta != Private_TransmissionGearSta::INTERMEDIATE) {
            mPrevPrivate_TransmissionGearSta = mCurrPrivate_TransmissionGearSta;
        }

        mPrev_GearPositionRStatusFromICU = mGearPositionRStatusFromICU;
        mPrev_GearPositionRStatusFromEMS = mGearPositionRStatusFromEMS;
    }

    inline bool isCheckmPrevPrivate_TransmissionGearSta() {
        return mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::NONE &&
               mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::INVALID &&
               mPrevPrivate_TransmissionGearSta != Private_TransmissionGearSta::MESSAGE_TIMEOUT;
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_300ms,
        OFF_0ms
    };

    enum class IgnitionState : HUInt64 {
        NONE,
        ON,
        OFF
    };

    enum class Private_GearStatusType : HUInt64 {
        NONE,
        INPUT_GIMRS_1_TCU,
        INPUT_GIMRS_2_TCU,
        INPUT_GIMRS_HDCT,
        INPUT_GIMRS_ON_EV,
        MAX
    };

    enum class Private_GearStatus : HUInt64 {
        NONE,
        OFF,
        SPEED1,
        SPEED2,
        SPEED3,
        SPEED4,
        SPEED5,
        SPEED6,
        SPEED7,
        SPEED8,
        UNHANDLED_TIMEOUT,
        MAX
    };

    enum class Private_TransmissionGearStaType : HUInt64 {
        NONE,
        INPUT_GSS_VCU,
        INPUT_GSS_SCU,
        INPUT_GSS_TCU,
        INPUT_GSS_HDCT,
        MAX
    };

    enum class Private_TransmissionGearSta : HUInt64 {
        NONE,
        STAGE_P,
        STAGE_L,
        STAGE_2,
        STAGE_3,
        DS_MODE,
        STAGE_D,
        STAGE_N,
        STAGE_R,
        MANUAL,
        PADDLE_SHIFT,
        INTERMEDIATE,
        INVALID,
        STAGE_M,
        MESSAGE_TIMEOUT,
        MAX
    };

    enum class Private_TarGearType : HUInt64 {
        OFF,
        ON
    };

    enum class Private_TransmissionBlinking_Stat : HUInt64 {
        OFF,
        ON
    };

    enum class Private_TarGearStep : HUInt64 {
        OFF,
        SPEED1,
        SPEED2,
        SPEED3,
        SPEED4,
        SPEED5,
        SPEED6,
        SPEED7,
        SPEED8
    };

    enum class Private_MaxStepTransmission_Stat : HUInt64 {
        NONE,
        OFF,
        MAX_THREE,
        MAX_FOUR,
        MAX_FIVE,
        MAX_SIX,
        MAX_SEVEN,
        MAX_EIGHT,
        MAX
    };

    enum class Private_TransmissionDisplayOffReq : HUInt64 {
        DISPLAY_OFF,
        DISPLAY_ON
    };

    enum class Private_AgiDisplayOnOff_Stat : HUInt64 {
        OFF,
        ON
    };

    enum class Private_AgiAvailable_Stat : HUInt64 {
        DISPLAY_OFF,
        AVAILABLE,
        UNAVAILABLE
    };

    using Inter_ConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using Inter_ConfigTransmissionType = ArgumentsConfigChanged::Inter_ConfigTransmissionType;
    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using Inter_ConfigTransmissionDetailType = ArgumentsConfigChanged::Inter_ConfigTransmissionDetailType;
    using Inter_ConfigArea = ArgumentsConfigChanged::Inter_ConfigArea;
    using Inter_ConfigSBW = ArgumentsConfigChanged::Inter_ConfigSBW;
    using Inter_PTLogicControlType = ArgumentsConfigChanged::Inter_PTLogicControlType;
    using GearStepType = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearStepType;
    using GearIndManualSignalConfig = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearIndManualSignalConfig;
    using GearIndManualReqStatus1FromTCU = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearIndManualReqStatus1FromTCU;
    using GearIndManualReqStatus2FromTCU = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearIndManualReqStatus2FromTCU;
    using GearIndManualReqStatusFromHDCT = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearIndManualReqStatusFromHDCT;
    using GearIndManualReqStatusOnEV = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearIndManualReqStatusOnEV;
    using GearDisplayStatusFromVCU = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearDisplayStatusFromVCU;
    using GearSelectStatusFromTCU = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearSelectStatusFromTCU;
    using GearSelectStatusFromHDCT = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearSelectStatusFromHDCT;
    using GearSelectStatusFromSCU = ArgumentsPrivateGearAndGearSelectStatusChanged::Input_GearSelectStatusFromSCU;
    using GearShiftIndiUpStatusFromEmsHcu = ArgumentsPrivateTarGearUpChanged::Input_GearShiftIndiUpStatusFromEmsHcu;
    using GearShiftIndiUpStatusFromHTCU = ArgumentsPrivateTarGearUpChanged::Input_GearShiftIndiUpStatusFromHTCU;
    using GearShiftIndiDownStatusFromEmsHcu = ArgumentsPrivateTarGearDownChanged::Input_GearShiftIndiDownStatusFromEmsHcu;
    using GearShiftIndiDownStatusFromHTCU = ArgumentsPrivateTarGearDownChanged::Input_GearShiftIndiDownStatusFromHTCU;
    using GearShiftIndiStepStatusFromEmsHcu = ArgumentsPrivateTarGearStepChanged::Input_GearShiftIndiStepStatusFromEmsHcu;
    using GearShiftIndiStepStatusFromHTCU = ArgumentsPrivateTarGearStepChanged::Input_GearShiftIndiStepStatusFromHTCU;
    using GearShiftIndiStepNewStatus = ArgumentsPrivateTarGearStepChanged::Input_GearShiftIndiStepNewStatus;
    using GearShiftIndiUpDownNewStatus = ArgumentsGearShiftIndiUpDownNewStatusChanged::Input_GearShiftIndiUpDownNewStatus;
    using SbwParkNeutralStatus = ArgumentsTransmissionDisplayValueATChanged::Input_SbwParkNeutralStatus;
    using DoorPassiveLockReqStatus = ArgumentsTransmissionDisplayValueATChanged::Input_DoorPassiveLockReqStatus;
    using DoorRKELockUnlockReqStatus = ArgumentsTransmissionDisplayValueATChanged::Input_DoorRKELockUnlockReqStatus;
    using TransmissionEolErrorWarnStatusFromIMT =
        ArgumentsTransmissionDisplayValueNotMTNChanged::Input_TransmissionEolErrorWarnStatusFromIMT;
    using GearIndManualReqStatusOnIMT = ArgumentsTransmissionDisplayValueNotMTNChanged::Input_GearIndManualReqStatusOnIMT;
    using GearIndiDisplayReqStatusFromIMT =
        ArgumentsGearIndiDisplayReqStatusFromIMTChanged::Input_GearIndiDisplayReqStatusFromIMT;
    using GearPositionRStatusFromICU = ArgumentsGearPositionRStatusFromICUChanged::Input_GearPositionRStatusFromICU;
    using GearPositionRStatusFromEMS = ArgumentsTransmissionDisplayValueMTNChanged::Input_GearPositionRStatusFromEMS;
    using GearIndManualReqStatusOnMT = ArgumentsTransmissionDisplayValueMTNChanged::Input_GearIndManualReqStatusOnMT;
    using GearIndiBlinkReqStatusFromDCT = ArgumentsTransmissionBlinkingStatChanged::Input_GearIndiBlinkReqStatusFromDCT;
    using GearIndiBlinkReqStatusFromTCU = ArgumentsTransmissionBlinkingStatChanged::Input_GearIndiBlinkReqStatusFromTCU;
    using TransmissionEolErrorWarnStatusFromTCU =
        ArgumentsTransmissionEolErrorWarnStatusFromTCUChanged::Input_TransmissionEolErrorWarnStatusFromTCU;
    using GearMaxStepInfoFromEmsHcu = ArgumentsGearMaxStepInfoChanged::Input_GearMaxStepInfoFromEmsHcu;
    using GearMaxStepInfoFromHDCT = ArgumentsGearMaxStepInfoChanged::Input_GearMaxStepInfoFromHDCT;
    using SbwTypeStatus = ArgumentsSbwTypeStatusChanged::Input_SbwTypeStatus;
    using TransmissionAgiUpStatus = ArgumentsTransmissionAgiStatusChanged::Input_TransmissionAgiUpStatus;
    using TransmissionAgiDownStatus = ArgumentsTransmissionAgiStatusChanged::Input_TransmissionAgiDownStatus;
    using ConstantTransmissionGSIStat = SFCTransmission_IndicatorConstantTransmissionGSIStat;
    using ConstantTransmissionGSITargetValue = SFCTransmission_IndicatorConstantTransmissionGSITargetValue;
    using ConstantTransmissionDisplayValue = SFCTransmission_IndicatorConstantTransmissionDisplayValue;
    using ConstantTransmissionBlinkingStat = SFCTransmission_IndicatorConstantTransmissionBlinkingStat;
    using ConstantTransmissionBlinkingBlinkValueA = SFCTransmission_IndicatorConstantTransmissionBlinkingBlinkValueA;
    using ConstantTransmissionBlinkingBlinkValueB = SFCTransmission_IndicatorConstantTransmissionBlinkingBlinkValueB;
    using Output_GearIndicatorStatus = decltype(Vehicle.System.HardWire.Output_GearIndicatorStatus)::TYPE;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    IgnitionState mIgnitionState = IgnitionState::NONE;
    IgnitionState mPrevIgnitionState = IgnitionState::OFF;
    Inter_ConfigHighPerformance mInter_ConfigHighPerformance = Inter_ConfigHighPerformance::OFF;
    Inter_ConfigTransmissionType mInter_ConfigTransmissionType = Inter_ConfigTransmissionType::MT;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    Inter_ConfigTransmissionDetailType mInter_ConfigTransmissionDetailType = Inter_ConfigTransmissionDetailType::NONE;
    Inter_ConfigArea mInter_ConfigArea = Inter_ConfigArea::DOM;
    Inter_ConfigSBW mInter_ConfigSBW = Inter_ConfigSBW::OFF;
    Inter_PTLogicControlType mInter_PTLogicControlType = Inter_PTLogicControlType::OFF;
    GearStepType mGearStepType = GearStepType::GEAR8;
    GearIndManualSignalConfig mGearIndManualSignalConfig = GearIndManualSignalConfig::SIGNAL1;
    GearIndManualReqStatus1FromTCU mGearIndManualReqStatus1FromTCU = GearIndManualReqStatus1FromTCU::OFF;
    GearIndManualReqStatus2FromTCU mGearIndManualReqStatus2FromTCU = GearIndManualReqStatus2FromTCU::OFF;
    GearIndManualReqStatusFromHDCT mGearIndManualReqStatusFromHDCT = GearIndManualReqStatusFromHDCT::OFF;
    GearIndManualReqStatusOnEV mGearIndManualReqStatusOnEV = GearIndManualReqStatusOnEV::OFF;
    GearDisplayStatusFromVCU mGearDisplayStatusFromVCU = GearDisplayStatusFromVCU::INVALID;
    GearSelectStatusFromTCU mGearSelectStatusFromTCU = GearSelectStatusFromTCU::INVALID;
    GearSelectStatusFromHDCT mGearSelectStatusFromHDCT = GearSelectStatusFromHDCT::INVALID;
    GearSelectStatusFromSCU mGearSelectStatusFromSCU = GearSelectStatusFromSCU::INVALID;
    GearShiftIndiUpStatusFromEmsHcu mGearShiftIndiUpStatusFromEmsHcu = GearShiftIndiUpStatusFromEmsHcu::OFF;
    GearShiftIndiDownStatusFromHTCU mGearShiftIndiDownStatusFromHTCU = GearShiftIndiDownStatusFromHTCU::OFF;
    GearShiftIndiUpStatusFromHTCU mGearShiftIndiUpStatusFromHTCU = GearShiftIndiUpStatusFromHTCU::OFF;
    GearShiftIndiDownStatusFromEmsHcu mGearShiftIndiDownStatusFromEmsHcu = GearShiftIndiDownStatusFromEmsHcu::OFF;
    GearShiftIndiStepStatusFromEmsHcu mGearShiftIndiStepStatusFromEmsHcu = GearShiftIndiStepStatusFromEmsHcu::OFF;
    GearShiftIndiStepStatusFromHTCU mGearShiftIndiStepStatusFromHTCU = GearShiftIndiStepStatusFromHTCU::OFF;
    GearShiftIndiStepNewStatus mGearShiftIndiStepNewStatus = GearShiftIndiStepNewStatus::OFF;
    GearShiftIndiUpDownNewStatus mGearShiftIndiUpDownNewStatus = GearShiftIndiUpDownNewStatus::DISPLAY_OFF;
    SbwParkNeutralStatus mSbwParkNeutralStatus = SbwParkNeutralStatus::OFF;
    SbwParkNeutralStatus mPrev_SbwParkNeutralStatus = SbwParkNeutralStatus::OFF;
    DoorPassiveLockReqStatus mDoorPassiveLockReqStatus = DoorPassiveLockReqStatus::LOCK_REQ;
    DoorPassiveLockReqStatus mPrev_DoorPassiveLockReqStatus = DoorPassiveLockReqStatus::LOCK_REQ;
    DoorRKELockUnlockReqStatus mDoorRKELockUnlockReqStatus = DoorRKELockUnlockReqStatus::LOCK_REQ;
    DoorRKELockUnlockReqStatus mPrev_DoorRKELockUnlockReqStatus = DoorRKELockUnlockReqStatus::LOCK_REQ;
    TransmissionEolErrorWarnStatusFromIMT mTransmissionEolErrorWarnStatusFromIMT = TransmissionEolErrorWarnStatusFromIMT::NO_WARN;
    GearIndManualReqStatusOnIMT mGearIndManualReqStatusOnIMT = GearIndManualReqStatusOnIMT::OFF;
    GearIndiDisplayReqStatusFromIMT mGearIndiDisplayReqStatusFromIMT = GearIndiDisplayReqStatusFromIMT::OFF;
    GearPositionRStatusFromICU mPrev_GearPositionRStatusFromICU = GearPositionRStatusFromICU::OFF;
    GearPositionRStatusFromICU mGearPositionRStatusFromICU = GearPositionRStatusFromICU::OFF;
    GearPositionRStatusFromEMS mPrev_GearPositionRStatusFromEMS = GearPositionRStatusFromEMS::OFF;
    GearPositionRStatusFromEMS mGearPositionRStatusFromEMS = GearPositionRStatusFromEMS::OFF;
    GearIndManualReqStatusOnMT mGearIndManualReqStatusOnMT = GearIndManualReqStatusOnMT::OFF;
    GearIndiBlinkReqStatusFromDCT mGearIndiBlinkReqStatusFromDCT = GearIndiBlinkReqStatusFromDCT::OFF;
    GearIndiBlinkReqStatusFromTCU mGearIndiBlinkReqStatusFromTCU = GearIndiBlinkReqStatusFromTCU::OFF;
    TransmissionEolErrorWarnStatusFromTCU mTransmissionEolErrorWarnStatusFromTCU = TransmissionEolErrorWarnStatusFromTCU::NO_WARN;
    GearMaxStepInfoFromEmsHcu mGearMaxStepInfoFromEmsHcu = GearMaxStepInfoFromEmsHcu::OFF;
    GearMaxStepInfoFromHDCT mGearMaxStepInfoFromHDCT = GearMaxStepInfoFromHDCT::OFF;
    SbwTypeStatus mSbwTypeStatus = SbwTypeStatus::OFF;
    TransmissionAgiUpStatus mTransmissionAgiUpStatus = TransmissionAgiUpStatus::OFF;
    TransmissionAgiDownStatus mTransmissionAgiDownStatus = TransmissionAgiDownStatus::OFF;
    Output_GearIndicatorStatus mPrev_Output_GearIndicatorStatus = Output_GearIndicatorStatus::UNHANDLED_TIMEOUT;
    Output_GearIndicatorStatus mOutput_GearIndicatorStatus = Output_GearIndicatorStatus::UNHANDLED_TIMEOUT;

    AddressValueList mActuatorOutputAddrList;
    Private_GearStatusType mPrivate_GearStatusType = Private_GearStatusType::NONE;
    HUInt64 mPrivate_GearStatusValue = 0;  // 4.2.1 Since the input stage is different, set it to HUInt64 type.
    Private_TransmissionGearStaType mPrivate_TransmissionGearStaType = Private_TransmissionGearStaType::NONE;
    HUInt64 mPrivate_TransmissionGearStaValue = 0;  // 4.2.2 Since the input stage is different, set it to HUInt64 type.
    Private_TransmissionGearSta mPrevPrivate_TransmissionGearSta = Private_TransmissionGearSta::NONE;
    Private_TransmissionDisplayOffReq mPrivate_TransmissionDisplayOffReq = Private_TransmissionDisplayOffReq::DISPLAY_ON;

    // 4.2.3 Set to HUInt64 type because of UNHANDLED_TIMEOUT
    HUInt64 mPrivate_TarGearUp = static_cast<HUInt64>(Private_TarGearType::OFF);
    // 4.2.3 Set to HUInt64 type because of UNHANDLED_TIMEOUT
    HUInt64 mPrivate_TarGearDown = static_cast<HUInt64>(Private_TarGearType::OFF);
    HUInt64 mPrivate_MaxStepTransmissionValue = 0;
    HUInt64 mPrivateTransmissionDisplayForAgivalue = 0;
    HUInt64 mPrivateTransmissionAgiUpValue = 0;
    HUInt64 mPrivateTransmissionAgiDownValue = 0;

    HBool mGearCrcError = false;  // false is CLEAR

    Private_TarGearStep mPrivate_TarGearStep = Private_TarGearStep::OFF;
    Private_TransmissionBlinking_Stat mPrivate_TransmissionBlinking_Stat = Private_TransmissionBlinking_Stat::OFF;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerHWOutput2sTimeout;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerConstant2sTimeout;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerIgnOn300msTimeout;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerSound300msTimeout;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerEvent300msTimeout;
    ssfs::SFCTimer<Transmission_Indicator> mOneShotTimerEventPaddleShift;
    static constexpr uint32_t kTimerInterval300ms = 300u;
    static constexpr uint32_t kTimerInterval10ms = 10u;
    static constexpr uint32_t kTimerInterval2s = 2000u;
    Private_MaxStepTransmission_Stat mPrivate_MaxStepTransmission_Stat = Private_MaxStepTransmission_Stat::OFF;
    Private_AgiDisplayOnOff_Stat mPrivate_AgiDisplayOnOff_Stat = Private_AgiDisplayOnOff_Stat::OFF;
    Private_AgiAvailable_Stat mPrivate_AgiUpAvailable_Stat = Private_AgiAvailable_Stat::DISPLAY_OFF;
    Private_AgiAvailable_Stat mPrivate_AgiDownAvailable_Stat = Private_AgiAvailable_Stat::DISPLAY_OFF;
    Private_GearStatus mPrevPrivate_GearStatus = Private_GearStatus::NONE;
    std::string mEventID;
    std::string mSoundID;

    // IMG_TransmissionGSI_stat
    ConstantTransmissionGSIStat mConstantTransmissionGSIStat = ConstantTransmissionGSIStat::NONE;
    // IMG_TransmissionGSITarget_value
    ConstantTransmissionGSITargetValue mConstantTransmissionGSITargetValue = ConstantTransmissionGSITargetValue::NONE;
    // IMG_TransmissionDisplay_value
    ConstantTransmissionDisplayValue mConstantTransmissionDisplayValue = ConstantTransmissionDisplayValue::NONE;
    HBool mIsGearChangedFromIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_Transmission_Indicator_H
