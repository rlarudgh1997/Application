/**
 * @file EV_Charging_System_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_EV_Charging_System_2_0_H
#define SFSS_EV_Charging_System_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "EV_Charging_System_2_0Base.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 3.1.0
// Reference : [PT171] EV_Charging_System_2_0. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class EV_Charging_System_2_0 : public EV_Charging_System_2_0Base {
public:
    EV_Charging_System_2_0() = default;
    ~EV_Charging_System_2_0() override = default;
    EV_Charging_System_2_0(const EV_Charging_System_2_0& other) = delete;
    EV_Charging_System_2_0(EV_Charging_System_2_0&& other) noexcept = delete;
    EV_Charging_System_2_0& operator=(const EV_Charging_System_2_0& other) = delete;
    EV_Charging_System_2_0& operator=(EV_Charging_System_2_0&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerPrivateChargingStatus.create(kTimerPrivateChargingStatusInterval60s, this,
                                                      &EV_Charging_System_2_0::onOneShotTimerPrivateChargingStatus) == false) {
            DWarning() << "[EV_Charging_System_2_0] mOneShotTimerPrivateChargingStatus Create Failed";
        }
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChargeFailWarnObcLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChargeFailWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChargeFailWarnLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0TelltaleEvChgCableStat(SFCEV_Charging_System_2_0TelltaleEvChgCableStat::OFF);
        setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqID("E22101");
        setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqLinkedSoundID("SND_PopUpInform2");
        setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvShiftPToChargeReqLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvChargeSchedWaitID("E22110");
        setSFCEV_Charging_System_2_0EventEvChargeSchedWaitLinkedSoundID("SND_PopUpInform2");
        setSFCEV_Charging_System_2_0EventEvChargeSchedWaitLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChargeSchedWaitLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChargeSchedWaitLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvChargeLinkedSoundID("SND_PopUpInform2");
        setSFCEV_Charging_System_2_0EventEvChargeLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChargeLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChargeLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnLinkedSoundID("SND_PopUpInform2");
        setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvFastChargeFailWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvChgCalculateErrorLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventEvChgCalculateErrorLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvChgCalculateErrorLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvChgCalculateErrorLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnLinkedSoundType(
            SFCEV_Charging_System_2_0EventEvUnreliableSocWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningLinkedSoundType(
            SFCEV_Charging_System_2_0EventV2LCInoperableAndRunningLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningLinkedSoundRepeatCount(1);
        setSFCEV_Charging_System_2_0EventV2LCTailgateID("E22131");
        setSFCEV_Charging_System_2_0EventV2LCTailgateLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Charging_System_2_0EventV2LCTailgateLinkedSoundType(
            SFCEV_Charging_System_2_0EventV2LCTailgateLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Charging_System_2_0EventV2LCTailgateLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        mOneShotTimerPrivateChargingStatus.stop();

        updatePrivateChargingStatus();
        updateTelltaleEvChgCable();
        updateEventUnrechargeableShiftP();
        updateEventChargeDoorOpen();
        updateEventChargeDoorOpenMalfunction();
        updateEventCharging();
        updateEventEvUnreliableSocWarn();
        updateEventV2LCInoperableAndRunning();
        updateEventV2LOnlyDoorOpen();
        updateEventV2LCTailgate();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;

        updateTelltaleEvChgCable();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;

        updatePrivateChargingStatus();
        updateTelltaleEvChgCable();
        updateEventUnrechargeableShiftP();
        updateEventChargeDoorOpen();
        updateEventChargeDoorOpenMalfunction();
        updateEventCharging();
        updateEventEvUnreliableSocWarn();
        updateEventV2LCInoperableAndRunning();
        updateEventV2LOnlyDoorOpen();
        updateEventV2LCTailgate();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateConstantChargingPwr();
        updateConstantChargingInfoForConnect();
        updateEventCharging();
    }

    void onTelltaleEvChargeCableChanged(const ArgumentsTelltaleEvChargeCableChanged& args) {
        mVcmsReadyStatus = args.mInput_VcmsReadyStatus;
        mV2LDisplayOnFromVCMS = args.mInput_V2LDisplayOnFromVCMS;
        mPhevPlugConnectStatusFromOBC = args.mInput_PhevPlugConnectStatusFromOBC;

        updateConstantChargingInfoForConnect();
        updateTelltaleEvChgCable();
        updateEventCharging();
        updateEventChargingV2L();
    }

    void onEvFastChargeChanged(const ArgumentsEvFastChargeChanged& args) {
        mEvFastChargeStatus = args.mInput_EvFastChargeStatus;
        mEvFastChargeTargetSocValue = args.mInput_EvFastChargeTargetSocValue;

        updatePrivateDisplay80SocStatus();
        updateConstantCharging80SocTime();
        updateConstantChargingMaxSoc();
        updateConstantChargingMaxDte();
    }

    void onEvSocDisplayValueChanged(const ArgumentsEvSocDisplayValueChanged& args) {
        mEvSocDisplayValue = args.mInput_EvSocDisplayValue;

        updatePrivateDisplay80SocStatus();
        updateConstantCharging80SocTime();
        updateConstantChargingCurrentSoc();
    }

    void onDoorOpenChanged(const ArgumentsDoorOpenChanged& args) {
        mDoorOpenAssistSideStatus = args.mInput_DoorOpenAssistSideStatus;
        mDoorOpenDriverSideStatus = args.mInput_DoorOpenDriverSideStatus;
        mDoorOpenRearLeftSideStatus = args.mInput_DoorOpenRearLeftSideStatus;
        mDoorOpenRearRightSideStatus = args.mInput_DoorOpenRearRightSideStatus;

        updatePrivateDoorOpenStatus();
        updatePrivateChargingStatus();
        updateEventCharging();
    }

    void onEvChargeStatusChanged(const ArgumentsEvChargeStatusChanged& args) {
        mEvChargeStatus = args.mInput_EvChargeStatus;
        mEvChargeSchedWaitIndiStatus = args.mInput_EvChargeSchedWaitIndiStatus;

        updatePrivateChargingStatus();
        updateConstantChargingOnOffStat();
        updateConstantChargingWaitInfo();
        updateEventChargingWaiting();
        updateEventCharging();
    }

    void onEvStdChargeChanged(const ArgumentsEvStdChargeChanged& args) {
        mEvStdChargeStatus = args.mInput_EvStdChargeStatus;
        mEvStdChargeTargetSocValue = args.mInput_EvStdChargeTargetSocValue;

        updateConstantChargingMaxSoc();
        updateConstantChargingMaxDte();
    }

    void onEvFastChargeFromVCMSChanged(const ArgumentsEvFastChargeFromVCMSChanged& args) {
        mEvFastChargeFailWarnStatusFromVCMS = args.mInput_EvFastChargeFailWarnStatusFromVCMS;
        mEvChargeAlarmSubInfoFromVCMS = args.mInput_EvChargeAlarmSubInfoFromVCMS;

        updateConstantChargingInfoForConnect();
        updateEventUnrechargeable();
        updateEventCharging();
        updateEventChargingV2L();
    }

    void onChargingMaxDteChanged(const ArgumentsChargingMaxDteChanged& args) {
        mEvFastChargeTargetDteValue = args.mInput_EvFastChargeTargetDteValue;
        mEvStdChargeTargetDteValue = args.mInput_EvStdChargeTargetDteValue;
        mTripDistanceUnit = args.mInter_TripDistanceUnit;

        updateConstantChargingMaxDte();
        updateConstantChargingEightyPercentDte();
        updateConstantV2LRemainDteAfterUse();
    }

    void onV2lMsgAppliedStatusChanged(const ArgumentsV2lMsgAppliedStatusChanged& args) {
        mV2LMsgAppliedStatus = args.mInput_V2LMsgAppliedStatus;

        updatePrivateV2LMinimumSocFromVcmsValue();
    }

    void onV2lMinimumSocValueChanged(const ArgumentsV2lMinimumSocValueChanged& args) {
        mV2LMinimumSocValue = args.mInput_V2LMinimumSocValue;
        mV2LMinimumSocFromVcmsValue = args.mInput_V2LMinimumSocFromVcmsValue;

        updatePrivateV2LMinimumSocFromVcmsValue();
    }

    void onEvShiftPToChargeReqStatusChanged(const ArgumentsEvShiftPToChargeReqStatusChanged& args) {
        mEvShiftPToChargeReqStatus = args.mInput_EvShiftPToChargeReqStatus;

        updateEventUnrechargeableShiftP();
    }

    void onEvChargeFinishStatusChanged(const ArgumentsEvChargeFinishStatusChanged& args) {
        mEvChargeFinishStatus = args.mInput_EvChargeFinishStatus;
        mEvBulkChargeRemainTimeValue = args.mInput_EvBulkChargeRemainTimeValue;
        mEvStdChargeRemainTimeValue = args.mInput_EvStdChargeRemainTimeValue;

        updateConstantChargingRemainTime();
        updateConstantCharging80SocTime();
        updateEventUnrechargeableShiftP();
    }

    void onEvChargeFailWarnStatusFromOBCChanged(const ArgumentsEvChargeFailWarnStatusFromOBCChanged& args) {
        mEvChargeFailWarnStatusFromOBC = args.mInput_EvChargeFailWarnStatusFromOBC;

        updateEventUnrechargeableObc();
    }

    void onEvChgCalculateErrorStatusChanged(const ArgumentsEvChgCalculateErrorStatusChanged& args) {
        mEvChgCalcErrorStatus = args.mInput_EvChgCalculateErrorStatus;

        updateEventChgCalcErrorStatus();
    }

    void onEventDoorVsmChanged(const ArgumentsEventDoorVsmChanged& args) {
        mEvChargeDoorFrontOpenStatus = args.mInput_EvChargeDoorFrontOpenStatus;
        mEvChargeDoorMalfunctionStatus = args.mInput_EvChargeDoorMalfunctionStatus;
        mEvChargeDoorOpenStatus = args.mInput_EvChargeDoorOpenStatus;
        mEvChargeDoorRearOpenStatus = args.mInput_EvChargeDoorRearOpenStatus;
        mVehicleReadyStatus = args.mInput_VehicleReadyStatus;
        mDisplaySpeedValueKPH = args.mInter_DisplaySpeedValueKPH;

        updateEventChargeDoorOpen();
        updateEventChargeDoorOpenMalfunction();
        updateEventV2LOnlyDoorOpen();
    }

    void onChargingPwrChanged(const ArgumentsChargingPwrChanged& args) {
        mEvStdChargeInputPowerValue = args.mInput_EvStdChargeInputPowerValue;
        if (ISTIMEOUT(args.mInput_EvMergeChargeInputPowerValue) == true) {
            mEvMergeChargeInputPowerValue = 0;
        } else {
            mEvMergeChargeInputPowerValue = args.mInput_EvMergeChargeInputPowerValue;
        }

        updateConstantChargingPwr();
    }

    void onEvChargeSchedDayChanged(const ArgumentsEvChargeSchedDayChanged& args) {
        mEvChargeSchedDayStatus = args.mInput_EvChargeSchedDayStatus;

        updateConstantEvChargeSchedDay();
    }

    void onEvChargeSchedMinChanged(const ArgumentsEvChargeSchedMinChanged& args) {
        mEvChargeSchedMinValue = args.mInput_EvChargeSchedMinValue;

        updateConstantEvChargeSchedMin();
    }

    void onV2lUsingAvailableTimeMinValueChanged(const ArgumentsV2lUsingAvailableTimeMinValueChanged& args) {
        mV2LUsingAvailableTimeMinValue = args.mInput_V2LUsingAvailableTimeMinValue;

        updateConstantV2LUsingAvailableTimeMin();
    }

    void onV2lRemainDteAfterUseValueChanged(const ArgumentsV2lRemainDteAfterUseValueChanged& args) {
        mV2LRemainDteAfterUseValue = args.mInput_V2LRemainDteAfterUseValue;
        if (ISTIMEOUT(args.mInput_V2LRemainDteAfterUseValue) == true) {
            mIsV2LRemainDteAfterUseValueTimeout = true;
        } else {
            mIsV2LRemainDteAfterUseValueTimeout = false;
        }

        updateConstantV2LRemainDteAfterUse();
    }

    void onEvUnreliableSocWarnChanged(const ArgumentsEvUnreliableSocWarnChanged& args) {
        mEvUnreliableSocWarn = args.mInput_EvUnreliableSocWarn;

        updateEventEvUnreliableSocWarn();
    }

    void onV2LCUsingAvailableTimeMinValueChanged(const ArgumentsV2LCUsingAvailableTimeMinValueChanged& args) {
        if (ISTIMEOUT(args.mInput_V2LCUsingAvailableTimeMinValue) == true) {
            mV2LCUsingAvailableTimeMinStatus = V2LCUsingAvailableTimeMinStatus::MESSAGE_TIMEOUT;
        } else {
            mV2LCUsingAvailableTimeMinStatus = V2LCUsingAvailableTimeMinStatus::VALUE;
            mV2LCUsingAvailableTimeMinValue = args.mInput_V2LCUsingAvailableTimeMinValue;
        }
        updateConstantV2LCUsingAvailableTimeMin();
    }

    void onV2LCChargeInputPowerValueChanged(const ArgumentsV2LCChargeInputPowerValueChanged& args) {
        if (ISTIMEOUT(args.mInput_V2LCChargeInputPowerValue) == true) {
            mV2LCChargeInputPowerStatus = V2LCChargeInputPowerStatus::MESSAGE_TIMEOUT;
        } else {
            mV2LCChargeInputPowerStatus = V2LCChargeInputPowerStatus::VALUE;
            mV2LCChargeInputPowerValue = args.mInput_V2LCChargeInputPowerValue;
        }
        updateConstantV2LCChargingPwr();
    }

    void onV2LCWarnAndAlarmSubInfoChanged(const ArgumentsV2LCWarnAndAlarmSubInfoChanged& args) {
        mV2LCDisplayOnOffStatus = args.mInput_V2LCDisplayOnOffStatus;
        mV2LCWarnStatus = args.mInput_V2LCWarnStatus;
        mV2LCAlarmSubInfo = args.mInput_V2LCAlarmSubInfo;
        updateConstantChargingInfoForConnect();
        updateEventV2LCInoperableAndRunning();
    }

    void onEvFastChargeEightyPercentDteValueChanged(const ArgumentsEvFastChargeEightyPercentDteValueChanged& args) {
        mEvFastChargeEightyPercentDteValue = args.mInput_EvFastChargeEightyPercentDteValue;
        updateConstantChargingEightyPercentDte();
    }

    void onV2LOnlyDoorOpenStatusChanged(const ArgumentsV2LOnlyDoorOpenStatusChanged& args) {
        mV2LOnlyDoorOpenStatus = args.mInput_V2LOnlyDoorOpenStatus;
        updateEventV2LOnlyDoorOpen();
    }

    void onSbwWarnStatusFromScuChanged(const ArgumentsSbwWarnStatusFromScuChanged& args) {
        mSbwWarnStatusFromSCU = args.mInput_SbwWarnStatusFromSCU;
        updateEventV2LCTailgate();
    }

    void onEvChargeDoorErrorStatusFromCDMChanged(const ArgumentsEvChargeDoorErrorStatusFromCDMChanged& args) {
        mEvChargeDoorErrorStatusFromCDM = args.mInput_EvChargeDoorErrorStatusFromCDM;
        updateEventChargeDoorOpenMalfunction();
    }

    void onEvChargeFrontDoorErrorStatusFromCDMChanged(const ArgumentsEvChargeFrontDoorErrorStatusFromCDMChanged& args) {
        mEvChargeFrontDoorErrorStatusFromCDM = args.mInput_EvChargeFrontDoorErrorStatusFromCDM;
        updateEventChargeDoorOpenMalfunction();
    }

    void onEvChargeRearDoorErrorStatusFromCDMChanged(const ArgumentsEvChargeRearDoorErrorStatusFromCDMChanged& args) {
        mEvChargeRearDoorErrorStatusFromCDM = args.mInput_EvChargeRearDoorErrorStatusFromCDM;
        updateEventChargeDoorOpenMalfunction();
    }

    void onEvChargerRear2ndDoorStatusChanged(const ArgumentsEvChargerRear2ndDoorStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_EvChargerRear2ndDoorStatus) == true) {
            mEvChargerRear2ndDoorStatus = EvChargerRear2ndDoorStatus::UNHANDLED_TIMEOUT;
        } else {
            mEvChargerRear2ndDoorStatus = args.mInput_EvChargerRear2ndDoorStatus;
        }
        updateEventChargeDoorOpen();
    }

    void onGeneralSettingsChanged(const ArgumentsGeneralSettingsChanged& args) {
        mGeneralSettingsTimeFormat = args.mInter_GeneralSettingsTimeFormat;
        mEvChargeSchedHourValue = args.mInput_EvChargeSchedHourValue;

        updateInterConstantEvChargeSchedHour();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.2 잔여 충전 시간(80%) 표시 조건 / Display remaining Charging time(80%)
    void updatePrivateDisplay80SocStatus() {
        if ((mEvFastChargeStatus == EvFastChargeStatus::CHARGING) &&
            (mEvFastChargeTargetSocValue > kEvFastChargeTargetSocValue) && (mEvSocDisplayValue < kEvSocDisplayValue)) {
            mPrivateDisplay80SocStatus = Display80SocStatus::ON;
        } else {
            mPrivateDisplay80SocStatus = Display80SocStatus::OFF;
        }
    }

    // 4.3 도어 열림 판단
    void updatePrivateDoorOpenStatus() {
        if (mDoorOpenAssistSideStatus == DoorOpenAssistSideStatus::OPEN ||
            mDoorOpenDriverSideStatus == DoorOpenDriverSideStatus::OPEN ||
            mDoorOpenRearLeftSideStatus == DoorOpenRearLeftSideStatus::OPEN ||
            mDoorOpenRearRightSideStatus == DoorOpenRearRightSideStatus::OPEN) {
            mPrivateDoorOpenStatus = DoorOpenStatus::OPEN;
        } else {
            mPrivateDoorOpenStatus = DoorOpenStatus::CLOSE;
        }
    }

    void onOneShotTimerPrivateChargingStatus() {
        mPrivateChargingStatus = ChargingStatus::OFF;

        updateConstantChargingInfoForConnect();
        updateEventCharging();
        flush();
    }

    // 4.4 충전 중 상태 판단
    void updatePrivateChargingStatus() {
        if (mEvChargeSchedWaitIndiStatus == EvChargeSchedWaitIndiStatus::OFF && mEvChargeStatus == EvChargeStatus::CHARGING) {
            if (mIsIgnOn == true || (mIsIgnOn == false && mPrivateDoorOpenStatus == DoorOpenStatus::OPEN)) {
                mPrivateChargingDisplayStatus = ChargingDisplayStatus::ON_INFINITE;
            } else if (mIsIgnOn == false && mPrivateDoorOpenStatus == DoorOpenStatus::CLOSE) {
                mPrivateChargingDisplayStatus = ChargingDisplayStatus::ON_TEMPORARY;
            } else {
                // impossible
            }
        } else {
            mPrivateChargingDisplayStatus = ChargingDisplayStatus::OFF;
        }

        if (mPrivateChargingDisplayStatus == ChargingDisplayStatus::ON_INFINITE) {
            mPrivateChargingStatus = ChargingStatus::CHARGING;
            mOneShotTimerPrivateChargingStatus.stop();
        } else if (mPrivateChargingDisplayStatus == ChargingDisplayStatus::ON_TEMPORARY) {
            mPrivateChargingStatus = ChargingStatus::CHARGING;
            if (mOneShotTimerPrivateChargingStatus.start() == false) {
                DWarning() << "[EV_Charging_System_2_0] mOneShotTimerPrivateChargingStatus Start Failed";
            }
        } else {
            mPrivateChargingStatus = ChargingStatus::OFF;
            mOneShotTimerPrivateChargingStatus.stop();
        }
        updateConstantChargingInfoForConnect();
    }

    // 4.5 V2X,V2L 최소 동작가능 충전량 신호 판단
    void updatePrivateV2LMinimumSocFromVcmsValue() {
        if (mV2LMsgAppliedStatus == V2LMsgAppliedStatus::ON) {
            mPrivateV2LMinimumSocFromVcmsValue = mV2LMinimumSocFromVcmsValue;
        } else {
            mPrivateV2LMinimumSocFromVcmsValue = mV2LMinimumSocValue;
        }
        updateConstantV2LMinimumSoc();
    }

    // 5.1 Telltale
    void updateTelltaleEvChgCable() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCEV_Charging_System_2_0TelltaleEvChgCableStat(SFCEV_Charging_System_2_0TelltaleEvChgCableStat::OFF);
        } else if (mVcmsReadyStatus == VcmsReadyStatus::READY && mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON) {
            setSFCEV_Charging_System_2_0TelltaleEvChgCableStat(SFCEV_Charging_System_2_0TelltaleEvChgCableStat::ON);
        } else if (mVcmsReadyStatus != VcmsReadyStatus::READY &&
                   mPhevPlugConnectStatusFromOBC == PhevPlugConnectStatusFromOBC::CONNECTED) {
            setSFCEV_Charging_System_2_0TelltaleEvChgCableStat(SFCEV_Charging_System_2_0TelltaleEvChgCableStat::ON);
        } else {
            setSFCEV_Charging_System_2_0TelltaleEvChgCableStat(SFCEV_Charging_System_2_0TelltaleEvChgCableStat::OFF);
        }
    }

    // 5.2.1.1 잔여 충전 시간 / Remaining charging time
    void updateConstantChargingRemainTime() {
        SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat timeStat =
            SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::DISPLAY_OFF;

        if (mEvChargeFinishStatus == EvChargeFinishStatus::NORMAL) {
            if (mEvStdChargeRemainTimeValue == kEvStdChargeRemainTimeValue0 ||
                mEvStdChargeRemainTimeValue == kEvStdChargeRemainTimeValue65535) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::UNMEASURABLE;
            } else if (mEvStdChargeRemainTimeValue >= kEvStdChargeRemainTimeValue1 &&
                       mEvStdChargeRemainTimeValue <= kEvStdChargeRemainTimeValue59999) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::VALUE;
                setSFCEV_Charging_System_2_0ConstantChargingRemainTimeValue(mEvStdChargeRemainTimeValue);
            } else if (mEvStdChargeRemainTimeValue >= kEvStdChargeRemainTimeValue60000 &&
                       mEvStdChargeRemainTimeValue <= kEvStdChargeRemainTimeValue65534) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::VALUE;
                setSFCEV_Charging_System_2_0ConstantChargingRemainTimeValue(kEvStdChargeRemainTimeValue59999);
            } else {
                // DISPLAY_OFF
            }
        } else if (mEvChargeFinishStatus == EvChargeFinishStatus::FINISHED) {
            if (mEvStdChargeRemainTimeValue == kEvStdChargeRemainTimeValue0) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::MIN;
            } else if (mEvStdChargeRemainTimeValue >= kEvStdChargeRemainTimeValue1 &&
                       mEvStdChargeRemainTimeValue <= kEvStdChargeRemainTimeValue59999) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::VALUE;
                setSFCEV_Charging_System_2_0ConstantChargingRemainTimeValue(mEvStdChargeRemainTimeValue);
            } else if (mEvStdChargeRemainTimeValue >= kEvStdChargeRemainTimeValue60000 &&
                       mEvStdChargeRemainTimeValue <= kEvStdChargeRemainTimeValue65534) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::VALUE;
                setSFCEV_Charging_System_2_0ConstantChargingRemainTimeValue(kEvStdChargeRemainTimeValue59999);
            } else if (mEvStdChargeRemainTimeValue == kEvStdChargeRemainTimeValue65535) {
                timeStat = SFCEV_Charging_System_2_0ConstantChargingRemainTimeStat::UNMEASURABLE;
            } else {
                // DISPLAY_OFF
            }
        } else {
            // DISPLAY_OFF
        }
        setSFCEV_Charging_System_2_0ConstantChargingRemainTimeStat(timeStat);
    }

    // 5.2.1.2 잔여 충전 시간(80%)
    void updateConstantCharging80SocTime() {
        mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
            SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::DISPLAY_OFF;

        if (mPrivateDisplay80SocStatus == Display80SocStatus::ON) {
            if (mEvChargeFinishStatus == EvChargeFinishStatus::NORMAL) {
                if (mEvBulkChargeRemainTimeValue == kEvBulkChargeRemainTimeValue0 ||
                    mEvBulkChargeRemainTimeValue == kEvBulkChargeRemainTimeValue65535) {
                    mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
                        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::UNMEASURABLE;
                } else if (mEvBulkChargeRemainTimeValue >= kEvBulkChargeRemainTimeValue1 &&
                           mEvBulkChargeRemainTimeValue <= kEvBulkChargeRemainTimeValue65534) {
                    mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
                        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::VALUE;
                    setSFCEV_Charging_System_2_0ConstantCharging80SocTimeValue(mEvBulkChargeRemainTimeValue);
                } else {
                    // nothing
                }
            } else if (mEvChargeFinishStatus == EvChargeFinishStatus::FINISHED) {
                if (mEvBulkChargeRemainTimeValue == kEvBulkChargeRemainTimeValue0) {
                    mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
                        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::MIN;
                } else if (mEvBulkChargeRemainTimeValue >= kEvBulkChargeRemainTimeValue1 &&
                           mEvBulkChargeRemainTimeValue <= kEvBulkChargeRemainTimeValue65534) {
                    mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
                        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::VALUE;
                    setSFCEV_Charging_System_2_0ConstantCharging80SocTimeValue(mEvBulkChargeRemainTimeValue);
                } else if (mEvBulkChargeRemainTimeValue == kEvBulkChargeRemainTimeValue65535) {
                    mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
                        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::UNMEASURABLE;
                } else {
                    // nothing
                }
            } else {
                // nothing
            }
        }
        setSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat(mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat);
        updateConstantChargingEightyPercentDte();
    }

    // 5.2.1.3 목표(최대) 충전량 / Target (max) charge
    void updateConstantChargingMaxSoc() {
        SFCEV_Charging_System_2_0ConstantChargingMaxSocStat stat =
            SFCEV_Charging_System_2_0ConstantChargingMaxSocStat::UNMEASURABLE;
        HUInt64 value = 0;

        if (mEvFastChargeStatus == EvFastChargeStatus::CHARGING && mEvStdChargeStatus == EvStdChargeStatus::OFF &&
            mEvFastChargeTargetSocValue >= kEvFastChargeTargetSocValue0 &&
            mEvFastChargeTargetSocValue <= kEvFastChargeTargetSocValue200) {
            stat = SFCEV_Charging_System_2_0ConstantChargingMaxSocStat::VALUE;
            value = mEvFastChargeTargetSocValue / 2;  // floor
        } else if (mEvFastChargeStatus == EvFastChargeStatus::OFF && mEvStdChargeStatus == EvStdChargeStatus::CHARGING &&
                   mEvStdChargeTargetSocValue >= kEvStdChargeTargetSocValue0 &&
                   mEvStdChargeTargetSocValue <= kEvStdChargeTargetSocValue200) {
            stat = SFCEV_Charging_System_2_0ConstantChargingMaxSocStat::VALUE;
            value = mEvStdChargeTargetSocValue / 2;  // floor
        } else {
            // nothing
        }
        setSFCEV_Charging_System_2_0ConstantChargingMaxSocStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingMaxSocValue(value);
    }

    // 5.2.1.4 현재 충전량
    void updateConstantChargingCurrentSoc() {
        SFCEV_Charging_System_2_0ConstantChargingCurrentSocStat stat =
            SFCEV_Charging_System_2_0ConstantChargingCurrentSocStat::UNMEASURABLE;
        HUInt64 value = 0;

        if (mEvSocDisplayValue >= kEvSocDisplayValue0 && mEvSocDisplayValue <= kEvSocDisplayValueLimit) {
            stat = SFCEV_Charging_System_2_0ConstantChargingCurrentSocStat::VALUE;
            value = mEvSocDisplayValue / 2;  // floor
        }
        setSFCEV_Charging_System_2_0ConstantChargingCurrentSocStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingCurrentSocValue(value);
    }

    // 5.2.1.5 충전 전력량
    void updateConstantChargingPwr() {
        SFCEV_Charging_System_2_0ConstantChargingPwrStat stat = SFCEV_Charging_System_2_0ConstantChargingPwrStat::DISPLAY_OFF;
        HDouble value = 0.0;

        if (mConfigVehicleType == ConfigVehicleType::PHEV) {
            if (mEvStdChargeInputPowerValue >= kEvChargingValue1 && mEvStdChargeInputPowerValue <= kEvChargingValue254) {
                stat = SFCEV_Charging_System_2_0ConstantChargingPwrStat::VALUE;
                value = static_cast<HDouble>(mEvStdChargeInputPowerValue) * kEvStdChargeInputPowerCalValue;
            } else if (mEvStdChargeInputPowerValue == kEvChargingValue255) {
                stat = SFCEV_Charging_System_2_0ConstantChargingPwrStat::UNMEASURABLE;
            } else {
                // nothing
            }
        } else if (mConfigVehicleType == ConfigVehicleType::EV) {
            if (mEvMergeChargeInputPowerValue >= kEvMergeChargeInputPowerValue1 &&
                mEvMergeChargeInputPowerValue <= kEvMergeChargeInputPowerValue32767) {
                stat = SFCEV_Charging_System_2_0ConstantChargingPwrStat::VALUE;
                value = static_cast<HDouble>(mEvMergeChargeInputPowerValue) * kEvMergeChargeInputPowerCalValue;
            } else if (mEvMergeChargeInputPowerValue >= kEvMergeChargeInputPowerValue32768 &&
                       mEvMergeChargeInputPowerValue <= kEvMergeChargeInputPowerValue65535) {
                stat = SFCEV_Charging_System_2_0ConstantChargingPwrStat::VALUE;
                value = (static_cast<HDouble>(mEvMergeChargeInputPowerValue) - kEvMergeChargeInputPowerCalValue65536) *
                        kEvMergeChargeInputPowerCalValue;
            } else {
                // nothing
            }
        } else {
            // nothing
        }
        setSFCEV_Charging_System_2_0ConstantChargingPwrStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingPwrValue(value);
    }

    // 5.2.1.6 목표(최대) 충전량 예상 DTE
    void updateConstantChargingMaxDte() {
        SFCEV_Charging_System_2_0ConstantChargingMaxDteStat stat =
            SFCEV_Charging_System_2_0ConstantChargingMaxDteStat::DISPLAY_OFF;
        HUInt64 value = 0;

        if (mEvFastChargeStatus == EvFastChargeStatus::CHARGING && mEvStdChargeStatus == EvStdChargeStatus::OFF &&
            mEvFastChargeTargetSocValue >= kEvFastChargeTargetSocValue0 &&
            mEvFastChargeTargetSocValue <= kEvFastChargeTargetSocValue200) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCEV_Charging_System_2_0ConstantChargingMaxDteStat::VALUE;
                value = mEvFastChargeTargetDteValue;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCEV_Charging_System_2_0ConstantChargingMaxDteStat::VALUE;
                value = lround(mEvFastChargeTargetDteValue * kFactor_ConvKmToMi);  // Round off
            } else {
                // no operation
            }
        } else if (mEvFastChargeStatus == EvFastChargeStatus::OFF && mEvStdChargeStatus == EvStdChargeStatus::CHARGING &&
                   mEvStdChargeTargetSocValue >= kEvStdChargeTargetSocValue0 &&
                   mEvStdChargeTargetSocValue <= kEvStdChargeTargetSocValue200) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCEV_Charging_System_2_0ConstantChargingMaxDteStat::VALUE;
                value = mEvStdChargeTargetDteValue;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCEV_Charging_System_2_0ConstantChargingMaxDteStat::VALUE;
                value = lround(mEvStdChargeTargetDteValue * kFactor_ConvKmToMi);  // Round off
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCEV_Charging_System_2_0ConstantChargingMaxDteStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingMaxDteValue(value);
    }

    // 5.2.1.7 80% 충전량 예상 DTE
    void updateConstantChargingEightyPercentDte() {
        SFCEV_Charging_System_2_0ConstantChargingEightyPercentDteStat stat =
            SFCEV_Charging_System_2_0ConstantChargingEightyPercentDteStat::DISPLAY_OFF;
        HUInt64 value = 0;
        if (mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat !=
            SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::DISPLAY_OFF) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCEV_Charging_System_2_0ConstantChargingEightyPercentDteStat::ON;
                value = mEvFastChargeEightyPercentDteValue;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCEV_Charging_System_2_0ConstantChargingEightyPercentDteStat::ON;
                value = lround(mEvFastChargeEightyPercentDteValue * kFactor_ConvKmToMi);
            } else {
                // no operation
            }
        }
        setSFCEV_Charging_System_2_0ConstantChargingEightyPercentDteStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingEightyPercentDteValue(value);
    }

    // 5.2.1.8 V2X,V2L 최소 동작 가능 충전량(사용 한도)
    void updateConstantV2LMinimumSoc() {
        SFCEV_Charging_System_2_0ConstantV2LMinimumSocStat stat = SFCEV_Charging_System_2_0ConstantV2LMinimumSocStat::VALUE;
        HUInt64 value = 0;

        if (mPrivateV2LMinimumSocFromVcmsValue >= kV2LMinimumSocValue1 &&
            mPrivateV2LMinimumSocFromVcmsValue <= kV2LMinimumSocValue200) {
            value = mPrivateV2LMinimumSocFromVcmsValue * 0.5;  // Round Down
        } else if (mPrivateV2LMinimumSocFromVcmsValue >= kV2LMinimumSocValue201 &&
                   mPrivateV2LMinimumSocFromVcmsValue <= kV2LMinimumSocValue254) {
            value = 100;
        } else {
            stat = SFCEV_Charging_System_2_0ConstantV2LMinimumSocStat::DISPLAY_OFF;
        }

        setSFCEV_Charging_System_2_0ConstantV2LMinimumSocStat(stat);
        setSFCEV_Charging_System_2_0ConstantV2LMinimumSocValue(value);
    }

    // 5.2.1.9 V2L 사용 가능 시간(사용한도 도달 시간)
    void updateConstantV2LUsingAvailableTimeMin() {
        SFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat stat =
            SFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat::DISPLAY_OFF;
        HUInt64 value = 0;

        if (mV2LUsingAvailableTimeMinValue == kV2LUsingAvailableTimeMinValue0 ||
            mV2LUsingAvailableTimeMinValue == kV2LUsingAvailableTimeMinValue65535) {
            stat = SFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat::UNMEASURABLE;
        } else if (mV2LUsingAvailableTimeMinValue >= kV2LUsingAvailableTimeMinValue1 &&
                   mV2LUsingAvailableTimeMinValue <= kV2LUsingAvailableTimeMinValue59999) {
            stat = SFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat::VALUE;
            value = mV2LUsingAvailableTimeMinValue;
        } else if (mV2LUsingAvailableTimeMinValue >= kV2LUsingAvailableTimeMinValue60000 &&
                   mV2LUsingAvailableTimeMinValue <= kV2LUsingAvailableTimeMinValue65534) {
            stat = SFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat::VALUE;
            value = kV2LUsingAvailableTimeMinValue59999;
        } else {
            // no operation
        }
        setSFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinStat(stat);
        setSFCEV_Charging_System_2_0ConstantV2LUsingAvailableTimeMinValue(value);
    }

    // 5.2.1.10 V2L 사용한도 도달 후 잔여 DTE
    void updateConstantV2LRemainDteAfterUse() {
        SFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseStat stat =
            SFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseStat::DISPLAY_OFF;
        HUInt64 value = 0;

        if (mIsV2LRemainDteAfterUseValueTimeout == false) {
            if (mV2LRemainDteAfterUseValue >= kV2LRemainDteAfterUseValue0 &&
                mV2LRemainDteAfterUseValue <= kV2LRemainDteAfterUseValue2047) {
                if (mTripDistanceUnit == TripDistanceUnit::KM) {
                    stat = SFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseStat::VALUE;
                    value = mV2LRemainDteAfterUseValue;
                } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                    stat = SFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseStat::VALUE;
                    value = lround(mV2LRemainDteAfterUseValue * kFactor_ConvKmToMi);  // Rounds off
                } else {
                    // no operation
                }
            }
        }
        setSFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseStat(stat);
        setSFCEV_Charging_System_2_0ConstantV2LRemainDteAfterUseValue(value);
    }

    // 5.2.1.11.1 V2LC 사용 가능 시간(사용 한도 도달 시간)
    void updateConstantV2LCUsingAvailableTimeMin() {
        SFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat stat =
            SFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat::DISPLAY_OFF;
        HUInt64 value = 0;

        if (mV2LCUsingAvailableTimeMinStatus == V2LCUsingAvailableTimeMinStatus::VALUE) {
            if (mV2LCUsingAvailableTimeMinValue == 0) {
                stat = SFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat::UNMEASURABLE;
            } else if (mV2LCUsingAvailableTimeMinValue >= 1 && mV2LCUsingAvailableTimeMinValue <= 59999) {
                stat = SFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat::ON;
                value = mV2LCUsingAvailableTimeMinValue;
            } else if (mV2LCUsingAvailableTimeMinValue >= 60000 && mV2LCUsingAvailableTimeMinValue <= 65534) {
                stat = SFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat::ON;
                value = 59999;
            } else {
                // no operation
            }
        }
        setSFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinStat(stat);
        setSFCEV_Charging_System_2_0ConstantV2LCUsingAvailableTimeMinValue(value);
    }

    // 5.2.1.11.2 V2LC 방전 전력량
    void updateConstantV2LCChargingPwr() {
        SFCEV_Charging_System_2_0ConstantV2LCChargingPwrStat stat =
            SFCEV_Charging_System_2_0ConstantV2LCChargingPwrStat::DISPLAY_OFF;
        HDouble value = 0.0;

        if (mV2LCChargeInputPowerStatus == V2LCChargeInputPowerStatus::VALUE) {
            if (mV2LCChargeInputPowerValue == 0) {
                stat = SFCEV_Charging_System_2_0ConstantV2LCChargingPwrStat::ON;
            } else if (mV2LCChargeInputPowerValue >= 1 && mV2LCChargeInputPowerValue <= 250) {
                stat = SFCEV_Charging_System_2_0ConstantV2LCChargingPwrStat::ON;
                value = static_cast<HDouble>(mV2LCChargeInputPowerValue) * 0.1;
                value *= kFactor_PosiToNeg;
            } else {
                // no operation
            }
        }
        setSFCEV_Charging_System_2_0ConstantV2LCChargingPwrStat(stat);
        setSFCEV_Charging_System_2_0ConstantV2LCChargingPwrValue(value);
    }

    // 5.2.2 충전대기중 상세 표시
    // 5.2.2.1 충전 대기중 위젯 표시(Connect 플랫폼 Only)
    void updateConstantChargingWaitInfo() {
        SFCEV_Charging_System_2_0ConstantChargingWaitInfoStat stat =
            SFCEV_Charging_System_2_0ConstantChargingWaitInfoStat::DISPLAY_OFF;

        if (mEvChargeSchedWaitIndiStatus == EvChargeSchedWaitIndiStatus::ON) {
            stat = SFCEV_Charging_System_2_0ConstantChargingWaitInfoStat::ON;
        }
        setSFCEV_Charging_System_2_0ConstantChargingWaitInfoStat(stat);
    }

    // 5.2.2.1 요일 표시
    void updateConstantEvChargeSchedDay() {
        SFCEV_Charging_System_2_0ConstantChargingSchedDayStat stat =
            SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::DISPLAY_OFF;

        switch (mEvChargeSchedDayStatus) {
            case EvChargeSchedDayStatus::SUN:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::SUN;
                break;
            case EvChargeSchedDayStatus::MON:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::MON;
                break;
            case EvChargeSchedDayStatus::TUE:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::TUE;
                break;
            case EvChargeSchedDayStatus::WED:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::WED;
                break;
            case EvChargeSchedDayStatus::THU:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::THU;
                break;
            case EvChargeSchedDayStatus::FRI:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::FRI;
                break;
            case EvChargeSchedDayStatus::SAT:
                stat = SFCEV_Charging_System_2_0ConstantChargingSchedDayStat::SAT;
                break;
            default:
                break;
        }
        setSFCEV_Charging_System_2_0ConstantChargingSchedDayStat(stat);
    }

    // 시간 표시(AM,PM,24 시간제)
    void updateInterConstantEvChargeSchedHour() {
        SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat ampmStat =
            SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::DISPLAY_OFF;
        SFCEV_Charging_System_2_0ConstantChargingSchedHourStat hourStat =
            SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::DISPLAY_OFF;
        HUInt64 hourValue = 0;

        if (mGeneralSettingsTimeFormat != GeneralSettingsTimeFormat::HOUR_24) {
            if (mEvChargeSchedHourValue == kHourValue0) {
                ampmStat = SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::AM;
                hourStat = SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::VALUE;
                hourValue = kHourValue12;
            } else if (mEvChargeSchedHourValue >= kHourValue1 && mEvChargeSchedHourValue <= kHourValue11) {
                ampmStat = SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::AM;
                hourStat = SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::VALUE;
                hourValue = mEvChargeSchedHourValue;
            } else if (mEvChargeSchedHourValue == kHourValue12) {
                ampmStat = SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::PM;
                hourStat = SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::VALUE;
                hourValue = kHourValue12;
            } else if (mEvChargeSchedHourValue >= kHourValue13 && mEvChargeSchedHourValue <= kHourMaxValue23) {
                ampmStat = SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::PM;
                hourStat = SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::VALUE;
                hourValue = mEvChargeSchedHourValue - kHourValue12;
            } else {
                // no operation
            }
        } else {
            if (mEvChargeSchedHourValue >= kHourValue0 && mEvChargeSchedHourValue <= kHourMaxValue23) {
                ampmStat = SFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat::DISPLAY_OFF;
                hourStat = SFCEV_Charging_System_2_0ConstantChargingSchedHourStat::VALUE;
                hourValue = mEvChargeSchedHourValue;
            }
        }
        setSFCEV_Charging_System_2_0ConstantChargingSchedAMPMStat(ampmStat);
        setSFCEV_Charging_System_2_0ConstantChargingSchedHourStat(hourStat);
        setSFCEV_Charging_System_2_0ConstantChargingSchedHourValue(hourValue);
    }

    // 5.2.2.1 분 표시
    void updateConstantEvChargeSchedMin() {
        SFCEV_Charging_System_2_0ConstantChargingSchedMinStat stat =
            SFCEV_Charging_System_2_0ConstantChargingSchedMinStat::DISPLAY_OFF;
        HUInt64 value = 0;

        if (mEvChargeSchedMinValue >= kMinMaxValue0 && mEvChargeSchedMinValue <= kMinMaxValue59) {
            stat = SFCEV_Charging_System_2_0ConstantChargingSchedMinStat::VALUE;
            value = mEvChargeSchedMinValue;
        }

        setSFCEV_Charging_System_2_0ConstantChargingSchedMinStat(stat);
        setSFCEV_Charging_System_2_0ConstantChargingSchedMinValue(value);
    }

    // 5.2.3 충전 여부 판단
    void updateConstantChargingOnOffStat() {
        if (mEvChargeSchedWaitIndiStatus == EvChargeSchedWaitIndiStatus::OFF && mEvChargeStatus == EvChargeStatus::CHARGING) {
            setSFCEV_Charging_System_2_0ConstantChargingOnOffStat(SFCEV_Charging_System_2_0ConstantChargingOnOffStat::ON);
        } else {
            setSFCEV_Charging_System_2_0ConstantChargingOnOffStat(SFCEV_Charging_System_2_0ConstantChargingOnOffStat::OFF);
        }
    }

    // 5.2.1 충전,V2L,V2X,V2Lc 동작 상태(Connect 플랫폼 Only)
    // 사양 분기 시점 COMPLETE 발생 조건 없음. 향후 조건 발생 시 추가 예정(SFCDEV-38)
    // COMPLETE 는 미구현으로 진행하라는 가이드를 받음
    void updateConstantChargingInfoForConnect() {
        SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat stat =
            SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::OFF;

        if (mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
            mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::V2X_ON) {
            stat = SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::V2X;
        } else if (mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
                   mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::V2L_START) {
            stat = SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::V2L;
        } else if (mV2LCDisplayOnOffStatus == V2LCDisplayOnOffStatus::ON && mV2LCWarnStatus == V2LCWarnStatus::V2LC_START &&
                   mV2LCAlarmSubInfo == 3) {
            stat = SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::V2LC;
        } else if (mConfigVehicleType == ConfigVehicleType::EV && mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
                   (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_SCHEDULE ||
                    mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_NORMAL ||
                    mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_WIRELESS)) {
            stat = SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::CHARGING_EV;
        } else if (mConfigVehicleType == ConfigVehicleType::PHEV && mPrivateChargingStatus == ChargingStatus::CHARGING) {
            stat = SFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat::CHARGING_PHEV;
        } else {
            // no operation
        }
        setSFCEV_Charging_System_2_0ConstantChargingInfoForConnectStat(stat);
    }

    // 5.3.1 충전 불가(P 단 이동)
    void updateEventUnrechargeableShiftP() {
        if (mIsIgnOn == true && mEvShiftPToChargeReqStatus == EvShiftPToChargeReqStatus::ON &&
            mEvChargeFinishStatus == EvChargeFinishStatus::NORMAL) {
            setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqStat(SFCEV_Charging_System_2_0EventEvShiftPToChargeReqStat::ON);
        } else {
            setSFCEV_Charging_System_2_0EventEvShiftPToChargeReqStat(SFCEV_Charging_System_2_0EventEvShiftPToChargeReqStat::OFF);
        }
    }

    // 5.3.2 충전 불가(OBC 신호)
    void updateEventUnrechargeableObc() {
        std::string eventID;

        if (mEvChargeFailWarnStatusFromOBC == EvChargeFailWarnStatusFromOBC::CHARGER_ERROR) {
            eventID = "E22102";
        } else if (mEvChargeFailWarnStatusFromOBC == EvChargeFailWarnStatusFromOBC::CONNECTOR_ERROR) {
            eventID = "E22103";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvChargeFailWarnObc.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcStat(SFCEV_Charging_System_2_0EventEvChargeFailWarnObcStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcID(eventID);
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnObcStat(SFCEV_Charging_System_2_0EventEvChargeFailWarnObcStat::ON);
        }
    }

    // 5.3.2 충전 불가(VCMS 신호)
    void updateEventUnrechargeable() {
        std::string eventID;

        if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_AC &&
            mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS2) {
            eventID = "E22102";
        } else if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_AC &&
                   mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS1) {
            eventID = "E22103";
        } else if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_DC &&
                   mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS2) {
            eventID = "E22104";
        } else if ((mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_AC ||
                    mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_DC) &&
                   mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS5) {
            eventID = "E22105";
        } else if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FINISH) {
            if (mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS2) {
                eventID = "E22106";
            } else if (mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS3) {
                eventID = "E22112";
            } else {
                // nothing
            }
        } else if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::V2L_FAIL) {
            if (mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS1) {
                eventID = "E22113";
            } else if (mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS2) {
                eventID = "E22114";
            } else if (mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS3) {
                eventID = "E22132";
            } else {
                // nothing
            }
        } else if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_FAIL_AC &&
                   mEvChargeAlarmSubInfoFromVCMS == kEvChargeAlarmSubInfoFromVCMS6) {
            eventID = "E22119";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvChargeFailWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnStat(SFCEV_Charging_System_2_0EventEvChargeFailWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnID(eventID);
            setSFCEV_Charging_System_2_0EventEvChargeFailWarnStat(SFCEV_Charging_System_2_0EventEvChargeFailWarnStat::ON);
        }
    }

    // 5.3.3 충전 도어 열림 1
    void updateEventChargeDoorOpen() {
        std::string eventID;
        if (mIsIgnOn == true && mVehicleReadyStatus == VehicleReadyStatus::READY) {
            if (mEvChargerRear2ndDoorStatus == EvChargerRear2ndDoorStatus::OFF ||
                mEvChargerRear2ndDoorStatus == EvChargerRear2ndDoorStatus::UNHANDLED_TIMEOUT) {
                if (mEvChargeDoorFrontOpenStatus == EvChargeDoorFrontOpenStatus::OPEN ||
                    mEvChargeDoorRearOpenStatus == EvChargeDoorRearOpenStatus::OPEN ||
                    mEvChargeDoorOpenStatus == EvChargeDoorOpenStatus::OPEN) {
                    if (mDisplaySpeedValueKPH < kDisplaySpeedValueKPH10) {
                        eventID = "E22107";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpInform2");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::REPEAT_COUNT);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundRepeatCount(1);
                    } else {
                        eventID = "E22108";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpWarn1");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::DURATION);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundDuration(kMaxEventDuration);
                    }
                }
            } else {
                if (mEvChargeDoorFrontOpenStatus == EvChargeDoorFrontOpenStatus::OPEN) {
                    if (mDisplaySpeedValueKPH < kDisplaySpeedValueKPH10) {
                        eventID = "E22133";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpInform2");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::REPEAT_COUNT);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundRepeatCount(1);
                    } else {
                        eventID = "E22134";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpWarn1");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::DURATION);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundDuration(kMaxEventDuration);
                    }
                } else if (mEvChargerRear2ndDoorStatus == EvChargerRear2ndDoorStatus::OPEN) {
                    if (mDisplaySpeedValueKPH < kDisplaySpeedValueKPH10) {
                        eventID = "E22135";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpInform2");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::REPEAT_COUNT);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundRepeatCount(1);
                    } else {
                        eventID = "E22136";
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundID("SND_PopUpWarn1");
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType(
                            SFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundType::DURATION);
                        setSFCEV_Charging_System_2_0EventEvChargeDoorOpenLinkedSoundDuration(kMaxEventDuration);
                    }
                } else {
                    // no operation
                }
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvChargeDoorOpen.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChargeDoorOpenStat(SFCEV_Charging_System_2_0EventEvChargeDoorOpenStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChargeDoorOpenID(eventID);
            setSFCEV_Charging_System_2_0EventEvChargeDoorOpenStat(SFCEV_Charging_System_2_0EventEvChargeDoorOpenStat::ON);
        }
    }

    // 5.3.3 충전 도어 열림 2
    void updateEventChargeDoorOpenMalfunction() {
        std::string eventID;
        std::string prevEventID;
        if (mIsIgnOn == true) {
            if (mEvChargeDoorMalfunctionStatus == EvChargeDoorMalfunctionStatus::FAILURE ||
                mEvChargeDoorErrorStatusFromCDM == EvChargeDoorErrorStatusFromCDM::FAILURE) {
                setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat(
                    SFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat::ON);
                eventID = "E22109";
            } else if (mEvChargeFrontDoorErrorStatusFromCDM == EvChargeFrontDoorErrorStatusFromCDM::FAILURE_FRONT) {
                eventID = "E22137";
            } else if (mEvChargeRearDoorErrorStatusFromCDM == EvChargeRearDoorErrorStatusFromCDM::FAILURE_REAR) {
                eventID = "E22138";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvChargeDoorMalfunction.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat(
                SFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionID(eventID);
            setSFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat(
                SFCEV_Charging_System_2_0EventEvChargeDoorMalfunctionStat::ON);
        }
    }

    // 5.3.4 충전 중(충전 대기 중)
    void updateEventChargingWaiting() {
        if (mEvChargeSchedWaitIndiStatus == EvChargeSchedWaitIndiStatus::ON) {
            setSFCEV_Charging_System_2_0EventEvChargeSchedWaitStat(SFCEV_Charging_System_2_0EventEvChargeSchedWaitStat::ON);
        } else {
            setSFCEV_Charging_System_2_0EventEvChargeSchedWaitStat(SFCEV_Charging_System_2_0EventEvChargeSchedWaitStat::OFF);
        }
    }

    // 5.3.4 충전 중(EV / PHEV 용)
    void updateEventCharging() {
        std::string eventID;
        if (mConfigVehicleType == ConfigVehicleType::EV && mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
            (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_SCHEDULE ||
             mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_NORMAL ||
             mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_WIRELESS)) {
            eventID = "E22111";
        } else if (mConfigVehicleType == ConfigVehicleType::PHEV && mPrivateChargingStatus == ChargingStatus::CHARGING) {
            eventID = "E22120";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvCharge.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChargeStat(SFCEV_Charging_System_2_0EventEvChargeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChargeID(eventID);
            setSFCEV_Charging_System_2_0EventEvChargeStat(SFCEV_Charging_System_2_0EventEvChargeStat::ON);
        }
    }

    // 5.3.4 충전 중(V2L,V2X 동작중)
    void updateEventChargingV2L() {
        std::string eventID;
        if (mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
            mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::V2L_START) {
            eventID = "E22115";
        } else if (mV2LDisplayOnFromVCMS == V2LDisplayOnFromVCMS::V2L_DISPLAY_ON &&
                   mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::V2X_ON) {
            eventID = "E22116";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvFastChargeFailWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnStat(
                SFCEV_Charging_System_2_0EventEvFastChargeFailWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnID(eventID);
            setSFCEV_Charging_System_2_0EventEvFastChargeFailWarnStat(SFCEV_Charging_System_2_0EventEvFastChargeFailWarnStat::ON);
        }
    }

    // 5.3.4 충전 중(충전시간 지연)
    void updateEventChgCalcErrorStatus() {
        std::string eventID;

        if (mEvChgCalcErrorStatus == EvChgCalculateErrorStatus::CANNOT_CALCULATE) {
            eventID = "E22117";
        } else if (mEvChgCalcErrorStatus == EvChgCalculateErrorStatus::CHARGER_OVERHEAT) {
            eventID = "E22118";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvChgCalculateError.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvChgCalculateErrorStat(SFCEV_Charging_System_2_0EventEvChgCalculateErrorStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvChgCalculateErrorID(eventID);
            setSFCEV_Charging_System_2_0EventEvChgCalculateErrorStat(SFCEV_Charging_System_2_0EventEvChgCalculateErrorStat::ON);
        }
    }

    // 5.3.5 배터리 관리(LFP 배터리)
    void updateEventEvUnreliableSocWarn() {
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mEvUnreliableSocWarn) {
                case EvUnreliableSocWarn::WARN30PER:
                    eventID = "E22121";
                    break;
                case EvUnreliableSocWarn::WARN70PER:
                    eventID = "E22122";
                    break;
                case EvUnreliableSocWarn::WARN100PER:
                    eventID = "E22123";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.EvUnreliableSocWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnStat(SFCEV_Charging_System_2_0EventEvUnreliableSocWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnID(eventID);
            setSFCEV_Charging_System_2_0EventEvUnreliableSocWarnStat(SFCEV_Charging_System_2_0EventEvUnreliableSocWarnStat::ON);
        }
    }

    // 5.3.6 V2LC 동작 불가, 실행 중, V2L 도어 열림
    void updateEventV2LCInoperableAndRunning() {
        std::string eventID;
        std::string soundID;

        if (mIsIgnOn == true) {
            if (mV2LCWarnStatus == V2LCWarnStatus::V2LC_STOP_CONNECTOR && mV2LCAlarmSubInfo == 1) {
                eventID = "E22124";
                soundID = "SND_PopUpWarn1";
            } else if (mV2LCWarnStatus == V2LCWarnStatus::V2LC_FINISH_SOC && mV2LCAlarmSubInfo == 3) {
                eventID = "E22125";
                soundID = "SND_PopUpWarn1";
            } else if (mV2LCWarnStatus == V2LCWarnStatus::V2LC_FAIL && mV2LCAlarmSubInfo == 1) {
                eventID = "E22126";
                soundID = "SND_PopUpWarn1";
            } else if (mV2LCWarnStatus == V2LCWarnStatus::V2LC_FAIL && mV2LCAlarmSubInfo == 2) {
                eventID = "E22127";
                soundID = "SND_PopUpWarn1";
            } else if (mV2LCDisplayOnOffStatus == V2LCDisplayOnOffStatus::ON && mV2LCWarnStatus == V2LCWarnStatus::V2LC_START &&
                       mV2LCAlarmSubInfo == 3) {
                eventID = "E22128";
                soundID = "SND_PopUpInform2";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.V2LCInoperableAndRunning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningStat(
                SFCEV_Charging_System_2_0EventV2LCInoperableAndRunningStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningID(eventID);
            setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningStat(
                SFCEV_Charging_System_2_0EventV2LCInoperableAndRunningStat::ON);
            setSFCEV_Charging_System_2_0EventV2LCInoperableAndRunningLinkedSoundID(soundID);
        }
    }

    // 5.3.7 V2LC 도어 열림
    void updateEventV2LOnlyDoorOpen() {
        std::string eventID;
        std::string soundID;
        SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundType soundType =
            SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundType::NONE;
        if (mIsIgnOn == true && mV2LOnlyDoorOpenStatus == V2LOnlyDoorOpenStatus::OPEN &&
            mVehicleReadyStatus == VehicleReadyStatus::READY) {
            if (mDisplaySpeedValueKPH < kDisplaySpeedValueKPH10) {
                eventID = "E22129";
                soundID = "SND_PopUpInform2";
                soundType = SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundType::REPEAT_COUNT;
                setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundRepeatCount(1);
            } else {
                eventID = "E22130";
                soundID = "SND_PopUpWarn1";
                soundType = SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundType::DURATION;
                setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundDuration(kMaxEventDuration);
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Charging_System_2_0.Event.V2LOnlyDoorOpen.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenStat(SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenID(eventID);
            setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenStat(SFCEV_Charging_System_2_0EventV2LOnlyDoorOpenStat::ON);
            setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundID(soundID);
            setSFCEV_Charging_System_2_0EventV2LOnlyDoorOpenLinkedSoundType(soundType);
        }
    }

    // 5.3.8 V2LC 실행 중 변속 불가
    void updateEventV2LCTailgate() {
        SFCEV_Charging_System_2_0EventV2LCTailgateStat stat = SFCEV_Charging_System_2_0EventV2LCTailgateStat::OFF;
        if (mIsIgnOn == true && mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::V2LC_TAILGATE) {
            stat = SFCEV_Charging_System_2_0EventV2LCTailgateStat::ON;
        }
        setSFCEV_Charging_System_2_0EventV2LCTailgateStat(stat);
    }

    static constexpr HUInt64 kEvFastChargeTargetSocValue = 160;
    static constexpr HUInt64 kEvSocDisplayValue = 160;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue0 = 0;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue65535 = 65535;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue65534 = 65534;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue60000 = 60000;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue59999 = 59999;
    static constexpr HUInt64 kEvStdChargeRemainTimeValue1 = 1;
    static constexpr HUInt64 kEvBulkChargeRemainTimeValue0 = 0;
    static constexpr HUInt64 kEvBulkChargeRemainTimeValue65535 = 65535;
    static constexpr HUInt64 kEvBulkChargeRemainTimeValue65534 = 65534;
    static constexpr HUInt64 kEvBulkChargeRemainTimeValue1 = 1;
    static constexpr HUInt64 kEvSocDisplayValue0 = 0;
    static constexpr HUInt64 kEvSocDisplayValueLimit = 200;
    static constexpr HUInt64 kEvFastChargeInputPowerValue1 = 1;
    static constexpr HUInt64 kEvFastChargeInputPowerValue510 = 510;
    static constexpr HUInt64 kEvFastChargeInputPowerValue511 = 511;
    static constexpr HUInt64 kEvChargingValue1 = 1;
    static constexpr HUInt64 kEvChargingValue254 = 254;
    static constexpr HUInt64 kEvChargingValue255 = 255;
    static constexpr HUInt64 kEvChargeAlarmSubInfoFromVCMS1 = 1;
    static constexpr HUInt64 kEvChargeAlarmSubInfoFromVCMS2 = 2;
    static constexpr HUInt64 kEvChargeAlarmSubInfoFromVCMS3 = 3;
    static constexpr HUInt64 kEvChargeAlarmSubInfoFromVCMS5 = 5;
    static constexpr HUInt64 kEvChargeAlarmSubInfoFromVCMS6 = 6;
    static constexpr HUInt64 kMaxEventDuration = 10000;
    static constexpr HUInt64 kTimerPrivateChargingStatusInterval60s = 60000u;
    static constexpr HUInt64 kHourValue0 = 0;
    static constexpr HUInt64 kHourValue1 = 1;
    static constexpr HUInt64 kHourValue11 = 11;
    static constexpr HUInt64 kHourValue12 = 12;
    static constexpr HUInt64 kHourValue13 = 13;
    static constexpr HUInt64 kHourMaxValue23 = 23;
    static constexpr HUInt64 kMinMaxValue0 = 0;
    static constexpr HUInt64 kMinMaxValue59 = 59;
    static constexpr HUInt64 kEvMergeChargeInputPowerValue1 = 1;
    static constexpr HUInt64 kEvMergeChargeInputPowerValue32767 = 32767;
    static constexpr HUInt64 kEvMergeChargeInputPowerValue32768 = 32768;
    static constexpr HUInt64 kEvMergeChargeInputPowerValue65535 = 65535;
    static constexpr HDouble kEvMergeChargeInputPowerCalValue65536 = 65536.0;
    static constexpr HDouble kEvMergeChargeInputPowerCalValue = 0.1;
    static constexpr HUInt64 kEvFastChargeTargetSocValue0 = 0;
    static constexpr HUInt64 kEvFastChargeTargetSocValue200 = 200;
    static constexpr HUInt64 kEvStdChargeTargetSocValue0 = 0;
    static constexpr HUInt64 kEvStdChargeTargetSocValue200 = 200;
    static constexpr HUInt64 kV2LMinimumSocValue1 = 1;
    static constexpr HUInt64 kV2LMinimumSocValue200 = 200;
    static constexpr HUInt64 kV2LMinimumSocValue201 = 201;
    static constexpr HUInt64 kV2LMinimumSocValue254 = 254;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue0 = 0;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue1 = 1;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue59999 = 59999;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue60000 = 60000;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue65534 = 65534;
    static constexpr HUInt64 kV2LUsingAvailableTimeMinValue65535 = 65535;
    static constexpr HUInt64 kV2LRemainDteAfterUseValue0 = 0;
    static constexpr HUInt64 kV2LRemainDteAfterUseValue2047 = 2047;

    static constexpr HDouble kDisplaySpeedValueKPH10 = 10.0;
    static constexpr HDouble kEvStdChargeInputPowerCalValue = 0.1;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    enum class Display80SocStatus : HUInt64 {
        ON,
        OFF
    };

    enum class DoorOpenStatus : HUInt64 {
        OPEN,
        CLOSE
    };

    enum class ChargingStatus : HUInt64 {
        CHARGING,
        OFF
    };

    enum class ChargingDisplayStatus {
        ON_INFINITE,
        ON_TEMPORARY,
        OFF
    };

    enum class V2LCUsingAvailableTimeMinStatus {
        NONE,
        VALUE,
        MESSAGE_TIMEOUT,
        MAX
    };

    enum class V2LCChargeInputPowerStatus {
        NONE,
        VALUE,
        MESSAGE_TIMEOUT,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using VcmsReadyStatus = ArgumentsTelltaleEvChargeCableChanged::Input_VcmsReadyStatus;
    using V2LDisplayOnFromVCMS = ArgumentsTelltaleEvChargeCableChanged::Input_V2LDisplayOnFromVCMS;
    using PhevPlugConnectStatusFromOBC = ArgumentsTelltaleEvChargeCableChanged::Input_PhevPlugConnectStatusFromOBC;
    using EvFastChargeStatus = ArgumentsEvFastChargeChanged::Input_EvFastChargeStatus;
    using DoorOpenAssistSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenAssistSideStatus;
    using DoorOpenDriverSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenDriverSideStatus;
    using DoorOpenRearLeftSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenRearLeftSideStatus;
    using DoorOpenRearRightSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenRearRightSideStatus;
    using EvChargeStatus = ArgumentsEvChargeStatusChanged::Input_EvChargeStatus;
    using EvChargeSchedWaitIndiStatus = ArgumentsEvChargeStatusChanged::Input_EvChargeSchedWaitIndiStatus;
    using EvChargeFinishStatus = ArgumentsEvChargeFinishStatusChanged::Input_EvChargeFinishStatus;
    using EvStdChargeStatus = ArgumentsEvStdChargeChanged::Input_EvStdChargeStatus;
    using EvFastChargeFailWarnStatusFromVCMS = ArgumentsEvFastChargeFromVCMSChanged::Input_EvFastChargeFailWarnStatusFromVCMS;
    using TripDistanceUnit = ArgumentsChargingMaxDteChanged::Inter_TripDistanceUnit;

    using EvChgCalculateErrorStatus = ArgumentsEvChgCalculateErrorStatusChanged::Input_EvChgCalculateErrorStatus;
    using EvChargeFailWarnStatusFromOBC = ArgumentsEvChargeFailWarnStatusFromOBCChanged::Input_EvChargeFailWarnStatusFromOBC;
    using EvShiftPToChargeReqStatus = ArgumentsEvShiftPToChargeReqStatusChanged::Input_EvShiftPToChargeReqStatus;
    using EvChargeDoorFrontOpenStatus = ArgumentsEventDoorVsmChanged::Input_EvChargeDoorFrontOpenStatus;
    using EvChargeDoorMalfunctionStatus = ArgumentsEventDoorVsmChanged::Input_EvChargeDoorMalfunctionStatus;
    using EvChargeDoorOpenStatus = ArgumentsEventDoorVsmChanged::Input_EvChargeDoorOpenStatus;
    using EvChargeDoorRearOpenStatus = ArgumentsEventDoorVsmChanged::Input_EvChargeDoorRearOpenStatus;
    using VehicleReadyStatus = ArgumentsEventDoorVsmChanged::Input_VehicleReadyStatus;
    using EvChargeSchedDayStatus = ArgumentsEvChargeSchedDayChanged::Input_EvChargeSchedDayStatus;
    using EvUnreliableSocWarn = ArgumentsEvUnreliableSocWarnChanged::Input_EvUnreliableSocWarn;
    using V2LMsgAppliedStatus = ArgumentsV2lMsgAppliedStatusChanged::Input_V2LMsgAppliedStatus;
    using V2LCDisplayOnOffStatus = ArgumentsV2LCWarnAndAlarmSubInfoChanged::Input_V2LCDisplayOnOffStatus;
    using V2LCWarnStatus = ArgumentsV2LCWarnAndAlarmSubInfoChanged::Input_V2LCWarnStatus;
    using V2LOnlyDoorOpenStatus = ArgumentsV2LOnlyDoorOpenStatusChanged::Input_V2LOnlyDoorOpenStatus;
    using SbwWarnStatusFromSCU = ArgumentsSbwWarnStatusFromScuChanged::Input_SbwWarnStatusFromSCU;
    using EvChargeDoorErrorStatusFromCDM = ArgumentsEvChargeDoorErrorStatusFromCDMChanged::Input_EvChargeDoorErrorStatusFromCDM;
    using EvChargeFrontDoorErrorStatusFromCDM =
        ArgumentsEvChargeFrontDoorErrorStatusFromCDMChanged::Input_EvChargeFrontDoorErrorStatusFromCDM;
    using EvChargeRearDoorErrorStatusFromCDM =
        ArgumentsEvChargeRearDoorErrorStatusFromCDMChanged::Input_EvChargeRearDoorErrorStatusFromCDM;
    using EvChargerRear2ndDoorStatus = ArgumentsEvChargerRear2ndDoorStatusChanged::Input_EvChargerRear2ndDoorStatus;
    using GeneralSettingsTimeFormat = ArgumentsGeneralSettingsChanged::Inter_GeneralSettingsTimeFormat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    PhevPlugConnectStatusFromOBC mPhevPlugConnectStatusFromOBC = PhevPlugConnectStatusFromOBC::OFF;
    VcmsReadyStatus mVcmsReadyStatus = VcmsReadyStatus::OFF;
    EvChargeFinishStatus mEvChargeFinishStatus = EvChargeFinishStatus::INVALID;
    EvFastChargeStatus mEvFastChargeStatus = EvFastChargeStatus::OFF;
    EvStdChargeStatus mEvStdChargeStatus = EvStdChargeStatus::OFF;
    HUInt64 mEvStdChargeRemainTimeValue = 0;
    HUInt64 mEvSocDisplayValue = 0;
    HUInt64 mEvFastChargeTargetSocValue = 0;
    HUInt64 mEvBulkChargeRemainTimeValue = 0;
    HUInt64 mEvStdChargeTargetSocValue = 0;
    HUInt64 mEvStdChargeInputPowerValue = 0;
    HUInt64 mEvMergeChargeInputPowerValue = 0;
    HUInt64 mEvFastChargeTargetDteValue = 0;
    HUInt64 mEvStdChargeTargetDteValue = 0;
    HUInt64 mV2LMinimumSocValue = 0;
    HUInt64 mV2LMinimumSocFromVcmsValue = 0;
    HUInt64 mPrivateV2LMinimumSocFromVcmsValue = 0;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;

    EvChargeFailWarnStatusFromOBC mEvChargeFailWarnStatusFromOBC = EvChargeFailWarnStatusFromOBC::NORMAL;
    EvChargeSchedWaitIndiStatus mEvChargeSchedWaitIndiStatus = EvChargeSchedWaitIndiStatus::OFF;
    EvChargeStatus mEvChargeStatus = EvChargeStatus::NONE;
    EvFastChargeFailWarnStatusFromVCMS mEvFastChargeFailWarnStatusFromVCMS = EvFastChargeFailWarnStatusFromVCMS::OFF;
    EvShiftPToChargeReqStatus mEvShiftPToChargeReqStatus = EvShiftPToChargeReqStatus::OFF;
    V2LDisplayOnFromVCMS mV2LDisplayOnFromVCMS = V2LDisplayOnFromVCMS::OFF;
    EvChgCalculateErrorStatus mEvChgCalcErrorStatus = EvChgCalculateErrorStatus::OFF;
    HUInt64 mEvChargeAlarmSubInfoFromVCMS = 0;
    EvChargeDoorFrontOpenStatus mEvChargeDoorFrontOpenStatus = EvChargeDoorFrontOpenStatus::CLOSE;
    EvChargeDoorMalfunctionStatus mEvChargeDoorMalfunctionStatus = EvChargeDoorMalfunctionStatus::NORMAL;
    EvChargeDoorOpenStatus mEvChargeDoorOpenStatus = EvChargeDoorOpenStatus::CLOSE;
    EvChargeDoorRearOpenStatus mEvChargeDoorRearOpenStatus = EvChargeDoorRearOpenStatus::CLOSE;
    DoorOpenAssistSideStatus mDoorOpenAssistSideStatus = DoorOpenAssistSideStatus::CLOSE;
    DoorOpenDriverSideStatus mDoorOpenDriverSideStatus = DoorOpenDriverSideStatus::CLOSE;
    DoorOpenRearLeftSideStatus mDoorOpenRearLeftSideStatus = DoorOpenRearLeftSideStatus::CLOSE;
    DoorOpenRearRightSideStatus mDoorOpenRearRightSideStatus = DoorOpenRearRightSideStatus::CLOSE;
    VehicleReadyStatus mVehicleReadyStatus = VehicleReadyStatus::OFF;
    HDouble mDisplaySpeedValueKPH = 0.0;
    Display80SocStatus mPrivateDisplay80SocStatus = Display80SocStatus::OFF;
    DoorOpenStatus mPrivateDoorOpenStatus = DoorOpenStatus::CLOSE;
    ChargingStatus mPrivateChargingStatus = ChargingStatus::OFF;
    ChargingDisplayStatus mPrivateChargingDisplayStatus = ChargingDisplayStatus::OFF;
    EvChargeSchedDayStatus mEvChargeSchedDayStatus = EvChargeSchedDayStatus::OFF;
    HUInt64 mEvChargeSchedHourValue = 0;
    HUInt64 mEvChargeSchedMinValue = 0;
    HUInt64 mV2LUsingAvailableTimeMinValue = 0;
    HUInt64 mV2LRemainDteAfterUseValue = 0;
    HBool mIsV2LRemainDteAfterUseValueTimeout = false;
    EvUnreliableSocWarn mEvUnreliableSocWarn = EvUnreliableSocWarn::OFF;
    V2LMsgAppliedStatus mV2LMsgAppliedStatus = V2LMsgAppliedStatus::OFF;
    HUInt64 mV2LCUsingAvailableTimeMinValue = 0;
    V2LCUsingAvailableTimeMinStatus mV2LCUsingAvailableTimeMinStatus = V2LCUsingAvailableTimeMinStatus::MESSAGE_TIMEOUT;
    HUInt64 mV2LCChargeInputPowerValue = 0;
    V2LCChargeInputPowerStatus mV2LCChargeInputPowerStatus = V2LCChargeInputPowerStatus::MESSAGE_TIMEOUT;
    V2LCDisplayOnOffStatus mV2LCDisplayOnOffStatus = V2LCDisplayOnOffStatus::OFF;
    V2LCWarnStatus mV2LCWarnStatus = V2LCWarnStatus::OFF;
    HUInt64 mV2LCAlarmSubInfo = 0;
    HUInt64 mEvFastChargeEightyPercentDteValue = 0;
    V2LOnlyDoorOpenStatus mV2LOnlyDoorOpenStatus = V2LOnlyDoorOpenStatus::CLOSE;
    SbwWarnStatusFromSCU mSbwWarnStatusFromSCU = SbwWarnStatusFromSCU::OFF;
    SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat mSFCEV_Charging_System_2_0ConstantCharging80SocTimeStat =
        SFCEV_Charging_System_2_0ConstantCharging80SocTimeStat::DISPLAY_OFF;
    EvChargeDoorErrorStatusFromCDM mEvChargeDoorErrorStatusFromCDM = EvChargeDoorErrorStatusFromCDM::NORMAL;
    EvChargeFrontDoorErrorStatusFromCDM mEvChargeFrontDoorErrorStatusFromCDM = EvChargeFrontDoorErrorStatusFromCDM::NORMAL;
    EvChargeRearDoorErrorStatusFromCDM mEvChargeRearDoorErrorStatusFromCDM = EvChargeRearDoorErrorStatusFromCDM::NORMAL;
    EvChargerRear2ndDoorStatus mEvChargerRear2ndDoorStatus = EvChargerRear2ndDoorStatus::OFF;
    GeneralSettingsTimeFormat mGeneralSettingsTimeFormat = GeneralSettingsTimeFormat::NONE;

    // Factor_ConvKmToMi
    static constexpr HDouble kFactor_ConvKmToMi = 256.0 / 412.0;
    // Factor_PositiveToNegative
    static constexpr HDouble kFactor_PosiToNeg = -1;

    ssfs::SFCTimer<EV_Charging_System_2_0> mOneShotTimerPrivateChargingStatus;
};

}  // namespace ccos

#endif  // SFSS_EV_Charging_System_2_0_H
