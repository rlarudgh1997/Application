/**
 * @file VCU.hpp
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
#ifndef SFSS_VCU_H
#define SFSS_VCU_H

#define DLOG_ENABLED gEnableSFCLog

#include "VCUBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.30
// Reference : [CV950] VCU. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class VCU : public VCUBase {
public:
    VCU() = default;
    ~VCU() override = default;
    VCU(const VCU& other) = delete;
    VCU(VCU&& other) noexcept = delete;
    VCU& operator=(const VCU& other) = delete;
    VCU& operator=(VCU&& other) noexcept = delete;

    void onInitialize() override {
        // 5.1.1 Ready 작동등
        setSFCVCUTelltaleReadyBlinkValueA(SFCVCUTelltaleReadyBlinkValueA::ON);
        setSFCVCUTelltaleReadyBlinkValueB(SFCVCUTelltaleReadyBlinkValueB::OFF);
        // 5.3.1 VCU 점검 팝업
        setSFCVCUEventVCUFaultID("E79501");
        setSFCVCUEventVCUFaultLinkedSoundID("SND_PopUpWarn1");
        setSFCVCUEventVCUFaultLinkedSoundType(SFCVCUEventVCUFaultLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventVCUFaultLinkedSoundRepeatCount(1);
        // 5.3.2 고전압 릴레이 비정상 동작
        setSFCVCUEventMainRelayOpenID("E79502");
        setSFCVCUEventMainRelayOpenLinkedSoundID("SND_PopUpWarn1");
        setSFCVCUEventMainRelayOpenLinkedSoundType(SFCVCUEventMainRelayOpenLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventMainRelayOpenLinkedSoundRepeatCount(1);
        // 5.3.3 고전압 커넥터 연결 확인 알림
        setSFCVCUEventStartInterLockID("E79503");
        setSFCVCUEventStartInterLockLinkedSoundID("SND_PopUpInform1");
        setSFCVCUEventStartInterLockLinkedSoundType(SFCVCUEventStartInterLockLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventStartInterLockLinkedSoundRepeatCount(1);
        // 5.3.4 도어 열림 상태 주행 불가 알림
        setSFCVCUEventDoorOpenAccelID("E79504");
        setSFCVCUEventDoorOpenAccelLinkedSoundID("SND_PopUpInform1");
        setSFCVCUEventDoorOpenAccelLinkedSoundType(SFCVCUEventDoorOpenAccelLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventDoorOpenAccelLinkedSoundRepeatCount(1);
        // 5.3.5 시동 불가 – 브레이크 신호 미입력 알림
        setSFCVCUEventStartBrakeOffID("E79505");
        setSFCVCUEventStartBrakeOffLinkedSoundID("SND_PopUpInform1");
        setSFCVCUEventStartBrakeOffLinkedSoundType(SFCVCUEventStartBrakeOffLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventStartBrakeOffLinkedSoundRepeatCount(1);
        // 5.3.6 브레이크 신호와 악셀 페달 신호 동시 입력 알림
        setSFCVCUEventBrakePedalOverrideID("E79506");
        setSFCVCUEventBrakePedalOverrideLinkedSoundID("SND_PopUpInform1");
        setSFCVCUEventBrakePedalOverrideLinkedSoundType(SFCVCUEventBrakePedalOverrideLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventBrakePedalOverrideLinkedSoundRepeatCount(1);
        // 5.3.7 시동 끄는 중 알림
        setSFCVCUEventPowerDownID("E79507");
        setSFCVCUEventPowerDownLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventPowerDownLinkedSoundType(SFCVCUEventPowerDownLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventPowerDownLinkedSoundRepeatCount(1);
        // 5.3.8 악셀 페달 점검
        setSFCVCUEventAccelPedalInfoID("E79508");
        setSFCVCUEventAccelPedalInfoLinkedSoundID("SND_PopUpWarn1");
        setSFCVCUEventAccelPedalInfoLinkedSoundType(SFCVCUEventAccelPedalInfoLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventAccelPedalInfoLinkedSoundRepeatCount(1);
        // 5.3.9 시동 불가 – 안전 스위치 잠김 알림
        setSFCVCUEventSafetySwitchID("E79509");
        setSFCVCUEventSafetySwitchLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventSafetySwitchLinkedSoundType(SFCVCUEventSafetySwitchLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventSafetySwitchLinkedSoundRepeatCount(1);
        // 5.3.10 충전 불가 – 안전 스위치 잠김 알림
        setSFCVCUEventChargeSafetySwitchID("E79510");
        setSFCVCUEventChargeSafetySwitchLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventChargeSafetySwitchLinkedSoundType(SFCVCUEventChargeSafetySwitchLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventChargeSafetySwitchLinkedSoundRepeatCount(1);
        // 5.3.11 시동 불가 – 연료 주입구 열림 알림
        setSFCVCUEventChargeFlapDoorSwitchID("E79511");
        setSFCVCUEventChargeFlapDoorSwitchLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventChargeFlapDoorSwitchLinkedSoundType(SFCVCUEventChargeFlapDoorSwitchLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventChargeFlapDoorSwitchLinkedSoundRepeatCount(1);
        // 5.3.12 저전압 배터리 점검
        setSFCVCUEventAuxBatteryChargeID("E79512");
        setSFCVCUEventAuxBatteryChargeLinkedSoundID("SND_PopUpWarn1");
        setSFCVCUEventAuxBatteryChargeLinkedSoundType(SFCVCUEventAuxBatteryChargeLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventAuxBatteryChargeLinkedSoundRepeatCount(1);
        // 5.3.13 프리차지 실패 알림
        setSFCVCUEventStartPreChargeInfoID("E79513");
        setSFCVCUEventStartPreChargeInfoLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventStartPreChargeInfoLinkedSoundType(SFCVCUEventStartPreChargeInfoLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventStartPreChargeInfoLinkedSoundRepeatCount(1);
        // 5.3.14 긴급 정차 경고
        setSFCVCUEventEmergencyEvWarnID("E79515");
        setSFCVCUEventEmergencyEvWarnLinkedSoundType(SFCVCUEventEmergencyEvWarnLinkedSoundType::INFINITE);
        // 5.3.15 시동 불가 – 충전 커넥터 연결 알림
        setSFCVCUEventPlugConnID("E79516");
        setSFCVCUEventPlugConnLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventPlugConnLinkedSoundType(SFCVCUEventPlugConnLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventPlugConnLinkedSoundRepeatCount(1);
        // 5.3.16 Utility Mode 작동 알림
        setSFCVCUEventUtilityModeLinkedSoundType(SFCVCUEventUtilityModeLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventUtilityModeLinkedSoundRepeatCount(1);
        // 5.3.17 변속 불가 – 브레이크 밟고 시동 요청/변속 조건 불일치
        setSFCVCUEventTransCurrentRangeLinkedSoundRepeatCount(1);
        // 5.3.18 충전 불가 - 파킹 브레이크 체결 요청
        setSFCVCUEventChangeParkingOffID("E79523");
        setSFCVCUEventChangeParkingOffLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventChangeParkingOffLinkedSoundType(SFCVCUEventChangeParkingOffLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventChangeParkingOffLinkedSoundRepeatCount(1);
        // 5.3.19 시동 불가 - 충전 도어 열림 알림
        setSFCVCUEventChargeFlapDoorSwitchEVID("E79524");
        setSFCVCUEventChargeFlapDoorSwitchEVLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventChargeFlapDoorSwitchEVLinkedSoundType(SFCVCUEventChargeFlapDoorSwitchEVLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventChargeFlapDoorSwitchEVLinkedSoundRepeatCount(1);
        // 5.3.20 주행 가능 상태 알림
        setSFCVCUEventReadyID("E79525");
        setSFCVCUEventReadyLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventReadyLinkedSoundType(SFCVCUEventReadyLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventReadyLinkedSoundRepeatCount(1);
        // 5.3.21 현재 기어단 선택 알림
        setSFCVCUEventSellectedGearID("E79526");
        setSFCVCUEventSellectedGearLinkedSoundID("SND_PopUpInform2");
        setSFCVCUEventSellectedGearLinkedSoundType(SFCVCUEventSellectedGearLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventSellectedGearLinkedSoundRepeatCount(1);
        // 5.3.22 최고속도 가변
        setSFCVCUEventMaxSpeedVariableLinkedSoundType(SFCVCUEventMaxSpeedVariableLinkedSoundType::REPEAT_COUNT);
        setSFCVCUEventMaxSpeedVariableLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateTelltaleReady();
        updateAllConstant();
        updateAllEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateTelltaleReady();
        updateAllConstant();
        updateAllEvent();
    }

    void onVehicleTypeChanged(const ArgumentsVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateTelltaleReady();
        updateConstantIncludeVehicleType();
        updateAllEvent();
    }

    void onReadyStatChanged(const ArgumentsReadyStatChanged& args) {
        mEvModeStatus = args.mInput_EvModeStatus;
        mFCReadyStatus = args.mInput_FCReadyStatus;

        updateTelltaleReady();
        updateEventReady();
    }

    void onVcuToAuxInvCommFaultStatusChanged(const ArgumentsVcuToAuxInvCommFaultStatusChanged& args) {
        mVCUToAuxInvCommFaultStatus = args.mInput_VCUToAuxInvCommFaultStatus;

        updateConstantAuxInvCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToBMSCommFaultStatusChanged(const ArgumentsVcuToBMSCommFaultStatusChanged& args) {
        mVCUToBMSCommFaultStatus = args.mInput_VCUToBMSCommFaultStatus;

        updateConstantBMSCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToFCU1CommFaultStatusChanged(const ArgumentsVcuToFCU1CommFaultStatusChanged& args) {
        mVCUToFCU1CommFaultStatus = args.mInput_VCUToFCU1CommFaultStatus;

        updateConstantFCU1CommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToFCU2CommFaultStatusChanged(const ArgumentsVcuToFCU2CommFaultStatusChanged& args) {
        mVCUToFCU2CommFaultStatus = args.mInput_VCUToFCU2CommFaultStatus;

        updateConstantFCU2CommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToHMU1CommFaultStatusChanged(const ArgumentsVcuToHMU1CommFaultStatusChanged& args) {
        mVCUToHMU1CommFaultStatus = args.mInput_VCUToHMU1CommFaultStatus;

        updateConstantHMU1CommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToLDCCommFaultStatusChanged(const ArgumentsVcuToLDCCommFaultStatusChanged& args) {
        mVCUToLDCCommFaultStatus = args.mInput_VCUToLDCCommFaultStatus;

        updateConstantLDCCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuToMCUCommFaultStatusChanged(const ArgumentsVcuToMCUCommFaultStatusChanged& args) {
        mVCUToMCUCommFaultStatus = args.mInput_VCUToMCUCommFaultStatus;

        updateConstantMCUCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onVcuFaultStatChanged(const ArgumentsVcuFaultStatChanged& args) {
        mServiceIndiReqStatusFromVCU = args.mInput_ServiceIndiReqStatusFromVCU;
        mVCUFaultECStatus = args.mInput_VCUFaultECStatus;

        updateConstantVCUFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onBusOffECStatusChanged(const ArgumentsBusOffECStatusChanged& args) {
        mBusOffECStatus = args.mInput_BusOffECStatus;

        updateConstantVCUCANBusOffStatus();
        updateEventVCUFaultStatus();
    }

    void onEbsCtoECStatusChanged(const ArgumentsEbsCtoECStatusChanged& args) {
        mEBSCtoECStatus = args.mInput_EBSCtoECStatus;

        updateConstantEBSCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onTcuCtoECStatusChanged(const ArgumentsTcuCtoECStatusChanged& args) {
        mTCUCtoECStatus = args.mInput_TCUCtoECStatus;

        updateConstantTCUCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onEmsCtoECStatusChanged(const ArgumentsEmsCtoECStatusChanged& args) {
        mEMSCtoECStatus = args.mInput_EMSCtoECStatus;

        updateConstantEMSCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onIbuCtoECStatusChanged(const ArgumentsIbuCtoECStatusChanged& args) {
        mIBUCtoECStatus = args.mInput_IBUCtoECStatus;

        updateConstantIBUCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onImuCtoECStatusChange(const ArgumentsImuCtoECStatusChange& args) {
        mIMUCtoECStatus = args.mInput_IMUCtoECStatus;

        updateConstantIMUCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onIobxCtoECStatusChanged(const ArgumentsIobxCtoECStatusChanged& args) {
        mIOBXCtoECStatus = args.mInput_IOBXCtoECStatus;

        updateConstantIOBXCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onIpsmCtoECStatusChanged(const ArgumentsIpsmCtoECStatusChanged& args) {
        mIPSMCtoECStatus = args.mInput_IPSMCtoECStatus;

        updateConstantIPSMCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onMfsmCtoECStatusChanged(const ArgumentsMfsmCtoECStatusChanged& args) {
        mMFSMCtoECStatus = args.mInput_MFSMCtoECStatus;

        updateConstantMFSMCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onPdcCtoECStatusChanged(const ArgumentsPdcCtoECStatusChanged& args) {
        mPDCCtoECStatus = args.mInput_PDCCtoECStatus;

        updateConstantPDCCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onSpaCtoECStatusChanged(const ArgumentsSpaCtoECStatusChanged& args) {
        mSPACtoECStatus = args.mInput_SPACtoECStatus;

        updateConstantSPACommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onAdasDRVCtoECStatusChanged(const ArgumentsAdasDRVCtoECStatusChanged& args) {
        mADASDRVCtoECStatus = args.mInput_ADAS_DRVCtoECStatus;

        updateConstantADASDRVCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onDtgCtoECStatusChanged(const ArgumentsDtgCtoECStatusChanged& args) {
        mDTGCtoECStatus = args.mInput_DTGCtoECStatus;

        updateConstantDTGCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onEapuCtoECStatusChanged(const ArgumentsEapuCtoECStatusChanged& args) {
        mEAPUCtoECStatus = args.mInput_EAPUCtoECStatus;

        updateConstantEAPUCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onEpbCtoECStatusChanged(const ArgumentsEpbCtoECStatusChanged& args) {
        mEPBCtoECStatus = args.mInput_EPBCtoECStatus;

        updateConstantEPBCommFaultStatus();
        updateEventVCUFaultStatus();
    }

    void onMainRelayOpenStatusChanged(const ArgumentsMainRelayOpenStatusChanged& args) {
        mMainRelayOpenStatus = args.mInput_MainRelayOpenStatus;

        updateEventMainRelayOpenStatus();
    }

    void onStartInterLockStatusChanged(const ArgumentsStartInterLockStatusChanged& args) {
        mStartInterLockStatus = args.mInput_StartInterLockStatus;

        updateEventStartInterLockStatus();
    }

    void onDoorOpenAccelStatusChanged(const ArgumentsDoorOpenAccelStatusChanged& args) {
        mDoorOpenAccelStatus = args.mInput_DoorOpenAccelStatus;

        updateEventDoorOpenAccelStatus();
    }

    void onStartBrakeOffStatusChanged(const ArgumentsStartBrakeOffStatusChanged& args) {
        mStartBrakeOffStatus = args.mInput_StartBrakeOffStatus;

        updateEventStartBrakeOffStatus();
    }

    void onBrakePedalOverrideStatusChanged(const ArgumentsBrakePedalOverrideStatusChanged& args) {
        mBrakePedalOverrideStatus = args.mInput_BrakePedalOverrideStatus;

        updateEventBrakePedalOverrideStatus();
    }

    void onPowerDownStatusChanged(const ArgumentsPowerDownStatusChanged& args) {
        mPowerDownStatus = args.mInput_PowerDownStatus;

        updateEventPowerDownStatus();
    }

    void onAccelPedalInfoStatusChanged(const ArgumentsAccelPedalInfoStatusChanged& args) {
        mAccelPedalInfoStatus = args.mInput_AccelPedalInfoStatus;

        updateEventAccelPedalInfoStatus();
    }

    void onSafetySwitchStatusChanged(const ArgumentsSafetySwitchStatusChanged& args) {
        mSafetySwitchStatus = args.mInput_SafetySwitchStatus;

        updateEventSafetySwitchStatus();
    }

    void onChargeSafetySwitchStatusChanged(const ArgumentsChargeSafetySwitchStatusChanged& args) {
        mChargeSafetySwitchStatus = args.mInput_ChargeSafetySwitchStatus;

        updateEventChargeSafetySwitchStatus();
    }

    void onChargeFlapDoorSwitchStatusChanged(const ArgumentsChargeFlapDoorSwitchStatusChanged& args) {
        mChargeFlapDoorSwitchStatus = args.mInput_ChargeFlapDoorSwitchStatus;

        updateEventChargeFlapDoorSwitchStatus();
        updateEventChargeFlapDoorSwitchStatusEV();
    }

    void onAuxBatteryChargeStatusChanged(const ArgumentsAuxBatteryChargeStatusChanged& args) {
        mAuxBatteryChargeStatus = args.mInput_AuxBatteryChargeStatus;

        updateEventAuxBatteryChargeStatus();
    }

    void onStartPreChargeInfoStatusChanged(const ArgumentsStartPreChargeInfoStatusChanged& args) {
        mStartPreChargeInfoStatus = args.mInput_StartPreChargeInfoStatus;

        updateEventStartPreChargeInfoStatus();
    }

    void onEmergencyEvWarnStatusChanged(const ArgumentsEmergencyEvWarnStatusChanged& args) {
        mEmergencyEvWarnStatus = args.mInput_EmergencyEvWarnStatus;

        updateEventEmergencyEvWarnStatus();
    }

    void onPlugConnStatusChanged(const ArgumentsPlugConnStatusChanged& args) {
        mPlugConnStatus = args.mInput_PlugConnStatus;

        updateEventPlugConnStatus();
    }

    void onUtilityModeStatusChanged(const ArgumentsUtilityModeStatusChanged& args) {
        mUtilityModeStatus = args.mInput_UtilityModeStatus;

        updateEventUtilityModeStatus();
    }

    void onTransCurrentRangeStatusChanged(const ArgumentsTransCurrentRangeStatusChanged& args) {
        mTransCurrentRangeStatus = args.mInput_TransCurrentRangeStatus;

        updateEventTransCurrentRangeStatus();
    }

    void onMaxSpeedStatusVCUChanged(const ArgumentsMaxSpeedStatusVCUChanged& args) {
        mMaxSpeedStatus_VCU = args.mInput_MaxSpeedStatus_VCU;

        updateEventMaxSpeedVariableStatus();
    }

    void onChangeParkingOffStatusChanged(const ArgumentsChangeParkingOffStatusChanged& args) {
        mChangeParkingOffStatus = args.mInput_ChangeParkingOffStatus;

        updateEventChangeParkingOffStatus();
    }

    void onSbwWarningStatusVCUChanged(const ArgumentsSbwWarningStatusVCUChanged& args) {
        mSbwWarningStatusVCU = args.mInput_SbwWarningStatus_VCU;

        updateEventSelletedGear();
    }

    void onMccDisplayStatusChanged(const ArgumentsMccDisplayStatusChanged& args) {
        mMCCDisplayStatus = args.mInput_MCCDisplayStatus;

        updateConstantMCCDisplayStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAllConstant() {
        updateConstantIncludeVehicleType();
        updateConstantExcludeVehicleType();
    }

    void updateConstantIncludeVehicleType() {
        updateConstantAuxInvCommFaultStatus();
        updateConstantBMSCommFaultStatus();
        updateConstantFCU1CommFaultStatus();
        updateConstantFCU2CommFaultStatus();
        updateConstantHMU1CommFaultStatus();
        updateConstantLDCCommFaultStatus();
        updateConstantMCUCommFaultStatus();
        updateConstantVCUFaultStatus();
    }

    void updateConstantExcludeVehicleType() {
        updateConstantVCUCANBusOffStatus();
        updateConstantEBSCommFaultStatus();
        updateConstantTCUCommFaultStatus();
        updateConstantEMSCommFaultStatus();
        updateConstantIBUCommFaultStatus();
        updateConstantIMUCommFaultStatus();
        updateConstantIOBXCommFaultStatus();
        updateConstantIPSMCommFaultStatus();
        updateConstantMFSMCommFaultStatus();
        updateConstantPDCCommFaultStatus();
        updateConstantSPACommFaultStatus();
        updateConstantADASDRVCommFaultStatus();
        updateConstantDTGCommFaultStatus();
        updateConstantEAPUCommFaultStatus();
        updateConstantEPBCommFaultStatus();
        updateConstantMCCDisplayStatus();
    }

    void updateAllEvent() {
        updateEventVCUFaultStatus();
        updateEventMainRelayOpenStatus();
        updateEventStartInterLockStatus();
        updateEventDoorOpenAccelStatus();
        updateEventStartBrakeOffStatus();
        updateEventBrakePedalOverrideStatus();
        updateEventPowerDownStatus();
        updateEventAccelPedalInfoStatus();
        updateEventSafetySwitchStatus();
        updateEventChargeSafetySwitchStatus();
        updateEventChargeFlapDoorSwitchStatus();
        updateEventAuxBatteryChargeStatus();
        updateEventStartPreChargeInfoStatus();
        updateEventEmergencyEvWarnStatus();
        updateEventPlugConnStatus();
        updateEventUtilityModeStatus();
        updateEventTransCurrentRangeStatus();
        updateEventChangeParkingOffStatus();
        updateEventChargeFlapDoorSwitchStatusEV();
        updateEventReady();
        updateEventSelletedGear();
        updateEventMaxSpeedVariableStatus();
    }

    // Telltale
    // 5.1.1 Ready 작동등
    void updateTelltaleReady() {
        SFCVCUTelltaleReadyStat stat = SFCVCUTelltaleReadyStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mEvModeStatus == EvModeStatus::EV_MODE && mFCReadyStatus != FCReadyStatus::FC_READY) {
                    stat = SFCVCUTelltaleReadyStat::BLINK1;
                } else if (mFCReadyStatus == FCReadyStatus::FC_READY) {
                    stat = SFCVCUTelltaleReadyStat::ON;
                } else {
                    // no operation
                }
            } else if (mConfigVehicleType == ConfigVehicleType::EV) {
                if (mEvModeStatus == EvModeStatus::EV_MODE) {
                    stat = SFCVCUTelltaleReadyStat::ON;
                }
            } else {
                // no operation
            }
        }
        setSFCVCUTelltaleReadyStat(stat);
    }

    // Constant
    // 5.2.1 VCU to AuxInverter 점검
    void updateConstantAuxInvCommFaultStatus() {
        SFCVCUConstantAuxInvCommFaultStat stat = SFCVCUConstantAuxInvCommFaultStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mVCUToAuxInvCommFaultStatus == VCUToAuxInvCommFaultStatus::ON) {
            stat = SFCVCUConstantAuxInvCommFaultStat::ON;
        }
        mSFCVCUConstantAuxInvCommFaultStat = stat;
        setSFCVCUConstantAuxInvCommFaultStat(stat);
    }

    // 5.2.2 VCU to BMS 점검
    void updateConstantBMSCommFaultStatus() {
        SFCVCUConstantBMSCommFaultStat stat = SFCVCUConstantBMSCommFaultStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mVCUToBMSCommFaultStatus == VCUToBMSCommFaultStatus::ON) {
            stat = SFCVCUConstantBMSCommFaultStat::ON;
        }
        mSFCVCUConstantBMSCommFaultStat = stat;
        setSFCVCUConstantBMSCommFaultStat(stat);
    }

    // 5.2.3 VCU to FCU1 점검
    void updateConstantFCU1CommFaultStatus() {
        SFCVCUConstantFCU1CommFaultStat stat = SFCVCUConstantFCU1CommFaultStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mVCUToFCU1CommFaultStatus == VCUToFCU1CommFaultStatus::ON) {
            stat = SFCVCUConstantFCU1CommFaultStat::ON;
        }
        mSFCVCUConstantFCU1CommFaultStat = stat;
        setSFCVCUConstantFCU1CommFaultStat(stat);
    }

    // 5.2.4 VCU to FCU2 점검
    void updateConstantFCU2CommFaultStatus() {
        SFCVCUConstantFCU2CommFaultStat stat = SFCVCUConstantFCU2CommFaultStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mVCUToFCU2CommFaultStatus == VCUToFCU2CommFaultStatus::ON) {
            stat = SFCVCUConstantFCU2CommFaultStat::ON;
        }
        mSFCVCUConstantFCU2CommFaultStat = stat;
        setSFCVCUConstantFCU2CommFaultStat(stat);
    }

    // 5.2.5 VCU to HMU 점검
    void updateConstantHMU1CommFaultStatus() {
        SFCVCUConstantHMU1CommFaultStat stat = SFCVCUConstantHMU1CommFaultStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mVCUToHMU1CommFaultStatus == VCUToHMU1CommFaultStatus::ON) {
            stat = SFCVCUConstantHMU1CommFaultStat::ON;
        }
        mSFCVCUConstantHMU1CommFaultStat = stat;
        setSFCVCUConstantHMU1CommFaultStat(stat);
    }

    // 5.2.6 VCU to LDC 점검
    void updateConstantLDCCommFaultStatus() {
        SFCVCUConstantLDCCommFaultStat stat = SFCVCUConstantLDCCommFaultStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mVCUToLDCCommFaultStatus == VCUToLDCCommFaultStatus::ON) {
            stat = SFCVCUConstantLDCCommFaultStat::ON;
        }
        mSFCVCUConstantLDCCommFaultStat = stat;
        setSFCVCUConstantLDCCommFaultStat(stat);
    }

    // 5.2.7 VCU to MCU 점검
    void updateConstantMCUCommFaultStatus() {
        SFCVCUConstantMCUCommFaultStat stat = SFCVCUConstantMCUCommFaultStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mVCUToMCUCommFaultStatus == VCUToMCUCommFaultStatus::ON) {
            stat = SFCVCUConstantMCUCommFaultStat::ON;
        }
        mSFCVCUConstantMCUCommFaultStat = stat;
        setSFCVCUConstantMCUCommFaultStat(stat);
    }

    // 5.2.8 VCU 자체 점검
    void updateConstantVCUFaultStatus() {
        SFCVCUConstantVCUFaultStat stat = SFCVCUConstantVCUFaultStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (((mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::EV ||
                  mConfigVehicleType == ConfigVehicleType::FCEV) &&
                 mVCUFaultECStatus == VCUFaultECStatus::ON) ||
                ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
                 (mServiceIndiReqStatusFromVCU == ServiceIndiReqStatusFromVCU::ON ||
                  mServiceIndiReqStatusFromVCU == ServiceIndiReqStatusFromVCU::MESSAGE_TIMEOUT))) {
                stat = SFCVCUConstantVCUFaultStat::ON;
            }
        }
        mSFCVCUConstantVCUFaultStat = stat;
        setSFCVCUConstantVCUFaultStat(stat);
    }

    // 5.2.9 CAN 라인 점검
    void updateConstantVCUCANBusOffStatus() {
        SFCVCUConstantVCUCANBusOffStat stat = SFCVCUConstantVCUCANBusOffStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mBusOffECStatus == BusOffECStatus::ON) {
            stat = SFCVCUConstantVCUCANBusOffStat::ON;
        }

        mSFCVCUConstantVCUCANBusOffStat = stat;
        setSFCVCUConstantVCUCANBusOffStat(stat);
    }

    // 5.2.10 VCU to EBS 점검
    void updateConstantEBSCommFaultStatus() {
        SFCVCUConstantEBSCommFaultStat stat = SFCVCUConstantEBSCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mEBSCtoECStatus == EBSCtoECStatus::ON) {
            stat = SFCVCUConstantEBSCommFaultStat::ON;
        }

        mSFCVCUConstantEBSCommFaultStat = stat;
        setSFCVCUConstantEBSCommFaultStat(stat);
    }

    // 5.2.11 VCU to TCU 점검
    void updateConstantTCUCommFaultStatus() {
        SFCVCUConstantTCUCommFaultStat stat = SFCVCUConstantTCUCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mTCUCtoECStatus == TCUCtoECStatus::ON) {
            stat = SFCVCUConstantTCUCommFaultStat::ON;
        }

        mSFCVCUConstantTCUCommFaultStat = stat;
        setSFCVCUConstantTCUCommFaultStat(stat);
    }

    // 5.2.12 VCU to EMS 점검
    void updateConstantEMSCommFaultStatus() {
        SFCVCUConstantEMSCommFaultStat stat = SFCVCUConstantEMSCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mEMSCtoECStatus == EMSCtoECStatus::ON) {
            stat = SFCVCUConstantEMSCommFaultStat::ON;
        }

        mSFCVCUConstantEMSCommFaultStat = stat;
        setSFCVCUConstantEMSCommFaultStat(stat);
    }

    // 5.2.13 VCU to IBU 점검
    void updateConstantIBUCommFaultStatus() {
        SFCVCUConstantIBUCommFaultStat stat = SFCVCUConstantIBUCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mIBUCtoECStatus == IBUCtoECStatus::ON) {
            stat = SFCVCUConstantIBUCommFaultStat::ON;
        }

        mSFCVCUConstantIBUCommFaultStat = stat;
        setSFCVCUConstantIBUCommFaultStat(stat);
    }

    // 5.2.14 VCU to IMU 점검
    void updateConstantIMUCommFaultStatus() {
        SFCVCUConstantIMUCommFaultStat stat = SFCVCUConstantIMUCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mIMUCtoECStatus == IMUCtoECStatus::ON) {
            stat = SFCVCUConstantIMUCommFaultStat::ON;
        }

        mSFCVCUConstantIMUCommFaultStat = stat;
        setSFCVCUConstantIMUCommFaultStat(stat);
    }

    // 5.2.15 VCU to IOBOX 점검
    void updateConstantIOBXCommFaultStatus() {
        SFCVCUConstantIOBXCommFaultStat stat = SFCVCUConstantIOBXCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mIOBXCtoECStatus == IOBXCtoECStatus::ON) {
            stat = SFCVCUConstantIOBXCommFaultStat::ON;
        }

        mSFCVCUConstantIOBXCommFaultStat = stat;
        setSFCVCUConstantIOBXCommFaultStat(stat);
    }

    // 5.2.16 VCU to IPSM 점검
    void updateConstantIPSMCommFaultStatus() {
        SFCVCUConstantIPSMCommFaultStat stat = SFCVCUConstantIPSMCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mIPSMCtoECStatus == IPSMCtoECStatus::ON) {
            stat = SFCVCUConstantIPSMCommFaultStat::ON;
        }

        mSFCVCUConstantIPSMCommFaultStat = stat;
        setSFCVCUConstantIPSMCommFaultStat(stat);
    }

    // 5.2.17 VCU to MFSM 점검
    void updateConstantMFSMCommFaultStatus() {
        SFCVCUConstantMFSMCommFaultStat stat = SFCVCUConstantMFSMCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mMFSMCtoECStatus == MFSMCtoECStatus::ON) {
            stat = SFCVCUConstantMFSMCommFaultStat::ON;
        }

        mSFCVCUConstantMFSMCommFaultStat = stat;
        setSFCVCUConstantMFSMCommFaultStat(stat);
    }

    // 5.2.18 VCU to PDC 점검
    void updateConstantPDCCommFaultStatus() {
        SFCVCUConstantPDCCommFaultStat stat = SFCVCUConstantPDCCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mPDCCtoECStatus == PDCCtoECStatus::ON) {
            stat = SFCVCUConstantPDCCommFaultStat::ON;
        }

        mSFCVCUConstantPDCCommFaultStat = stat;
        setSFCVCUConstantPDCCommFaultStat(stat);
    }

    // 5.2.19 VCU to SPA 점검
    void updateConstantSPACommFaultStatus() {
        SFCVCUConstantSPACommFaultStat stat = SFCVCUConstantSPACommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mSPACtoECStatus == SPACtoECStatus::ON) {
            stat = SFCVCUConstantSPACommFaultStat::ON;
        }

        mSFCVCUConstantSPACommFaultStat = stat;
        setSFCVCUConstantSPACommFaultStat(stat);
    }

    // 5.2.20 VCU to ADAS_DRV 점검
    void updateConstantADASDRVCommFaultStatus() {
        SFCVCUConstantADASDRVCommFaultStat stat = SFCVCUConstantADASDRVCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mADASDRVCtoECStatus == ADASDRVCtoECStatus::ON) {
            stat = SFCVCUConstantADASDRVCommFaultStat::ON;
        }

        mSFCVCUConstantADASDRVCommFaultStat = stat;
        setSFCVCUConstantADASDRVCommFaultStat(stat);
    }

    // 5.2.21 VCU to DTG 점검
    void updateConstantDTGCommFaultStatus() {
        SFCVCUConstantDTGCommFaultStat stat = SFCVCUConstantDTGCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mDTGCtoECStatus == DTGCtoECStatus::ON) {
            stat = SFCVCUConstantDTGCommFaultStat::ON;
        }

        mSFCVCUConstantDTGCommFaultStat = stat;
        setSFCVCUConstantDTGCommFaultStat(stat);
    }

    // 5.2.22 VCU to EAPU 점검
    void updateConstantEAPUCommFaultStatus() {
        SFCVCUConstantEAPUCommFaultStat stat = SFCVCUConstantEAPUCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mEAPUCtoECStatus == EAPUCtoECStatus::ON) {
            stat = SFCVCUConstantEAPUCommFaultStat::ON;
        }

        mSFCVCUConstantEAPUCommFaultStat = stat;
        setSFCVCUConstantEAPUCommFaultStat(stat);
    }

    // 5.2.23 VCU to EPB 점검
    void updateConstantEPBCommFaultStatus() {
        SFCVCUConstantEPBCommFaultStat stat = SFCVCUConstantEPBCommFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mEPBCtoECStatus == EPBCtoECStatus::ON) {
            stat = SFCVCUConstantEPBCommFaultStat::ON;
        }

        mSFCVCUConstantEPBCommFaultStat = stat;
        setSFCVCUConstantEPBCommFaultStat(stat);
    }

    // 5.2.24 MCC
    void updateConstantMCCDisplayStatus() {
        SFCVCUConstantMCCDisplayStat stat = SFCVCUConstantMCCDisplayStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mMCCDisplayStatus) {
                case MCCDisplayStatus::MCC1_ACTIVE:
                    stat = SFCVCUConstantMCCDisplayStat::MCC1_ACTIVE;
                    break;
                case MCCDisplayStatus::MCC2_ACTIVE:
                    stat = SFCVCUConstantMCCDisplayStat::MCC2_ACTIVE;
                    break;
                case MCCDisplayStatus::MCC1_OVERRIDE:
                    stat = SFCVCUConstantMCCDisplayStat::MCC1_OVERRIDE;
                    break;
                case MCCDisplayStatus::MCC2_OVERRIDE:
                    stat = SFCVCUConstantMCCDisplayStat::MCC2_OVERRIDE;
                    break;
                case MCCDisplayStatus::MCC1_STANDBY:
                    stat = SFCVCUConstantMCCDisplayStat::MCC1_STANDBY;
                    break;
                case MCCDisplayStatus::MCC2_STANDBY:
                    stat = SFCVCUConstantMCCDisplayStat::MCC2_STANDBY;
                    break;
                case MCCDisplayStatus::MCCCFAIL:
                    stat = SFCVCUConstantMCCDisplayStat::MCCCFAIL;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCVCUConstantMCCDisplayStat(stat);
    }

    // Event
    // 5.3.1 VCU 점검 팝업 (E79501)
    void updateEventVCUFaultStatus() {
        SFCVCUEventVCUFaultStat stat = SFCVCUEventVCUFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mSFCVCUConstantAuxInvCommFaultStat == SFCVCUConstantAuxInvCommFaultStat::ON ||
                mSFCVCUConstantBMSCommFaultStat == SFCVCUConstantBMSCommFaultStat::ON ||
                mSFCVCUConstantFCU1CommFaultStat == SFCVCUConstantFCU1CommFaultStat::ON ||
                mSFCVCUConstantFCU2CommFaultStat == SFCVCUConstantFCU2CommFaultStat::ON ||
                mSFCVCUConstantHMU1CommFaultStat == SFCVCUConstantHMU1CommFaultStat::ON ||
                mSFCVCUConstantLDCCommFaultStat == SFCVCUConstantLDCCommFaultStat::ON ||
                mSFCVCUConstantMCUCommFaultStat == SFCVCUConstantMCUCommFaultStat::ON ||
                mSFCVCUConstantVCUFaultStat == SFCVCUConstantVCUFaultStat::ON ||
                mSFCVCUConstantVCUCANBusOffStat == SFCVCUConstantVCUCANBusOffStat::ON ||
                mSFCVCUConstantEBSCommFaultStat == SFCVCUConstantEBSCommFaultStat::ON ||
                mSFCVCUConstantTCUCommFaultStat == SFCVCUConstantTCUCommFaultStat::ON ||
                mSFCVCUConstantEMSCommFaultStat == SFCVCUConstantEMSCommFaultStat::ON ||
                mSFCVCUConstantIBUCommFaultStat == SFCVCUConstantIBUCommFaultStat::ON ||
                mSFCVCUConstantIMUCommFaultStat == SFCVCUConstantIMUCommFaultStat::ON ||
                mSFCVCUConstantIOBXCommFaultStat == SFCVCUConstantIOBXCommFaultStat::ON ||
                mSFCVCUConstantIPSMCommFaultStat == SFCVCUConstantIPSMCommFaultStat::ON ||
                mSFCVCUConstantMFSMCommFaultStat == SFCVCUConstantMFSMCommFaultStat::ON ||
                mSFCVCUConstantPDCCommFaultStat == SFCVCUConstantPDCCommFaultStat::ON ||
                mSFCVCUConstantSPACommFaultStat == SFCVCUConstantSPACommFaultStat::ON ||
                mSFCVCUConstantADASDRVCommFaultStat == SFCVCUConstantADASDRVCommFaultStat::ON ||
                mSFCVCUConstantDTGCommFaultStat == SFCVCUConstantDTGCommFaultStat::ON ||
                mSFCVCUConstantEAPUCommFaultStat == SFCVCUConstantEAPUCommFaultStat::ON ||
                mSFCVCUConstantEPBCommFaultStat == SFCVCUConstantEPBCommFaultStat::ON) {
                stat = SFCVCUEventVCUFaultStat::ON;
            }
        }
        setSFCVCUEventVCUFaultStat(stat);
    }

    // 5.3.2 고전압 릴레이 비정상 동작 (E79502)
    void updateEventMainRelayOpenStatus() {
        SFCVCUEventMainRelayOpenStat stat = SFCVCUEventMainRelayOpenStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mMainRelayOpenStatus == MainRelayOpenStatus::ON) {
            stat = SFCVCUEventMainRelayOpenStat::ON;
        }
        setSFCVCUEventMainRelayOpenStat(stat);
    }

    // 5.3.3 고전압 커넥터 연결 확인 알림 (E79503)
    void updateEventStartInterLockStatus() {
        SFCVCUEventStartInterLockStat stat = SFCVCUEventStartInterLockStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mStartInterLockStatus == StartInterLockStatus::ON) {
            stat = SFCVCUEventStartInterLockStat::ON;
        }
        setSFCVCUEventStartInterLockStat(stat);
    }

    // 5.3.4 도어 열림 상태 주행 불가 알림 (E79504)
    void updateEventDoorOpenAccelStatus() {
        SFCVCUEventDoorOpenAccelStat stat = SFCVCUEventDoorOpenAccelStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mDoorOpenAccelStatus == DoorOpenAccelStatus::ON) {
            stat = SFCVCUEventDoorOpenAccelStat::ON;
        }
        setSFCVCUEventDoorOpenAccelStat(stat);
    }

    // 5.3.5 시동 불가 – 브레이크 신호 미입력 알림 (E79505)
    void updateEventStartBrakeOffStatus() {
        SFCVCUEventStartBrakeOffStat stat = SFCVCUEventStartBrakeOffStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mStartBrakeOffStatus == StartBrakeOffStatus::ON) {
            stat = SFCVCUEventStartBrakeOffStat::ON;
        }
        setSFCVCUEventStartBrakeOffStat(stat);
    }

    // 5.3.6 브레이크 신호와 악셀 페달 신호 동시 입력 알림 (E79506)
    void updateEventBrakePedalOverrideStatus() {
        SFCVCUEventBrakePedalOverrideStat stat = SFCVCUEventBrakePedalOverrideStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mBrakePedalOverrideStatus == BrakePedalOverrideStatus::ON) {
            stat = SFCVCUEventBrakePedalOverrideStat::ON;
        }
        setSFCVCUEventBrakePedalOverrideStat(stat);
    }

    // 5.3.7 시동 끄는 중 알림 (E79507)
    void updateEventPowerDownStatus() {
        SFCVCUEventPowerDownStat stat = SFCVCUEventPowerDownStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mPowerDownStatus == PowerDownStatus::ON) {
            stat = SFCVCUEventPowerDownStat::ON;
        }
        setSFCVCUEventPowerDownStat(stat);
    }

    // 5.3.8 악셀 페달 점검 (E79508)
    void updateEventAccelPedalInfoStatus() {
        SFCVCUEventAccelPedalInfoStat stat = SFCVCUEventAccelPedalInfoStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mAccelPedalInfoStatus == AccelPedalInfoStatus::ON) {
            stat = SFCVCUEventAccelPedalInfoStat::ON;
        }
        setSFCVCUEventAccelPedalInfoStat(stat);
    }

    // 5.3.9 시동 불가 – 안전 스위치 잠김 알림 (E79509)
    void updateEventSafetySwitchStatus() {
        SFCVCUEventSafetySwitchStat stat = SFCVCUEventSafetySwitchStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mSafetySwitchStatus == SafetySwitchStatus::ON) {
            stat = SFCVCUEventSafetySwitchStat::ON;
        }
        setSFCVCUEventSafetySwitchStat(stat);
    }

    // 5.3.10 충전 불가 – 안전 스위치 잠김 알림 (E79510)
    void updateEventChargeSafetySwitchStatus() {
        SFCVCUEventChargeSafetySwitchStat stat = SFCVCUEventChargeSafetySwitchStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mChargeSafetySwitchStatus == ChargeSafetySwitchStatus::ON) {
            stat = SFCVCUEventChargeSafetySwitchStat::ON;
        }
        setSFCVCUEventChargeSafetySwitchStat(stat);
    }

    // 5.3.11 시동 불가 – 연료 주입구 열림 알림 (E79511)
    void updateEventChargeFlapDoorSwitchStatus() {
        SFCVCUEventChargeFlapDoorSwitchStat stat = SFCVCUEventChargeFlapDoorSwitchStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mChargeFlapDoorSwitchStatus == ChargeFlapDoorSwitchStatus::ON) {
            stat = SFCVCUEventChargeFlapDoorSwitchStat::ON;
        }
        setSFCVCUEventChargeFlapDoorSwitchStat(stat);
    }

    // 5.3.12 저전압 배터리 점검 (E79512)
    void updateEventAuxBatteryChargeStatus() {
        SFCVCUEventAuxBatteryChargeStat stat = SFCVCUEventAuxBatteryChargeStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mAuxBatteryChargeStatus == AuxBatteryChargeStatus::ON) {
            stat = SFCVCUEventAuxBatteryChargeStat::ON;
        }
        setSFCVCUEventAuxBatteryChargeStat(stat);
    }

    // 5.3.13 프리차지 실패 알림 (E79513)
    void updateEventStartPreChargeInfoStatus() {
        SFCVCUEventStartPreChargeInfoStat stat = SFCVCUEventStartPreChargeInfoStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mStartPreChargeInfoStatus == StartPreChargeInfoStatus::ON) {
            stat = SFCVCUEventStartPreChargeInfoStat::ON;
        }
        setSFCVCUEventStartPreChargeInfoStat(stat);
    }

    // 5.3.14 긴급 정차 경고 (E79515)
    void updateEventEmergencyEvWarnStatus() {
        SFCVCUEventEmergencyEvWarnStat stat = SFCVCUEventEmergencyEvWarnStat::OFF;
        std::string linkedSoundID;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mEmergencyEvWarnStatus == EmergencyEvWarnStatus::WARN1) {
                stat = SFCVCUEventEmergencyEvWarnStat::ON;
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mEmergencyEvWarnStatus == EmergencyEvWarnStatus::WARN2) {
                stat = SFCVCUEventEmergencyEvWarnStat::ON;
                linkedSoundID = "SND_PopUpWarn2";
            } else {
                // no operation
            }
        }
        setSFCVCUEventEmergencyEvWarnStat(stat);
        setSFCVCUEventEmergencyEvWarnLinkedSoundID(linkedSoundID);
    }

    // 5.3.15 시동 불가 – 충전 커넥터 연결 알림 (E79516)
    void updateEventPlugConnStatus() {
        SFCVCUEventPlugConnStat stat = SFCVCUEventPlugConnStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mPlugConnStatus == PlugConnStatus::ON) {
            stat = SFCVCUEventPlugConnStat::ON;
        }
        setSFCVCUEventPlugConnStat(stat);
    }

    // 5.3.16 Utility Mode 작동 알림 (E79517 ~ E79520)
    void updateEventUtilityModeStatus() {
        std::string eventID;
        std::string linkedSoundID;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mUtilityModeStatus) {
                case UtilityModeStatus::ON:
                    eventID = "E79517";
                    linkedSoundID = "SND_PopUpInform1";
                    break;
                case UtilityModeStatus::INHIBIT:
                    eventID = "E79519";
                    linkedSoundID = "SND_PopUpInform1";
                    break;
                case UtilityModeStatus::EVON_MODE_AUTO_OFF:
                    eventID = "E79520";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case UtilityModeStatus::OFF:
                    if (mPrevUtilityModeStatus == UtilityModeStatus::ON) {
                        eventID = "E79518";
                        linkedSoundID = "SND_PopUpInform1";
                    }
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.VCU.Event.UtilityMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCVCUEventUtilityModeStat(SFCVCUEventUtilityModeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCVCUEventUtilityModeID(eventID);
            setSFCVCUEventUtilityModeStat(SFCVCUEventUtilityModeStat::ON);
            setSFCVCUEventUtilityModeLinkedSoundID(linkedSoundID);
        }
        mPrevUtilityModeStatus = mUtilityModeStatus;
    }

    // 5.3.17 변속 불가 – 브레이크 밟고 시동 요청/변속 조건 불일치 (E79522, E79527)
    void updateEventTransCurrentRangeStatus() {
        std::string eventID;
        std::string linkedSoundID;
        SFCVCUEventTransCurrentRangeLinkedSoundType linkedSoundType = SFCVCUEventTransCurrentRangeLinkedSoundType::NONE;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mTransCurrentRangeStatus == TransCurrentRangeStatus::NO_BRAKE_INPUT) {
                eventID = "E79522";
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCVCUEventTransCurrentRangeLinkedSoundType::INFINITE;
            } else if (mTransCurrentRangeStatus == TransCurrentRangeStatus::NOT_MATCH) {
                eventID = "E79527";
                linkedSoundID = "SND_PopUpInform1";
                linkedSoundType = SFCVCUEventTransCurrentRangeLinkedSoundType::REPEAT_COUNT;
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.VCU.Event.TransCurrentRange.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCVCUEventTransCurrentRangeStat(SFCVCUEventTransCurrentRangeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCVCUEventTransCurrentRangeID(eventID);
            setSFCVCUEventTransCurrentRangeStat(SFCVCUEventTransCurrentRangeStat::ON);
            setSFCVCUEventTransCurrentRangeLinkedSoundID(linkedSoundID);
            setSFCVCUEventTransCurrentRangeLinkedSoundType(linkedSoundType);
        }
    }

    // 5.3.18 충전 불가 - 파킹 브레이크 체결 요청
    void updateEventChangeParkingOffStatus() {
        SFCVCUEventChangeParkingOffStat stat = SFCVCUEventChangeParkingOffStat::OFF;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mIgnElapsed == IgnElapsed::ON_0ms && mChangeParkingOffStatus == ChangeParkingOffStatus::ON) {
            stat = SFCVCUEventChangeParkingOffStat::ON;
        }

        setSFCVCUEventChangeParkingOffStat(stat);
    }

    // 5.3.19 시동 불가 - 충전 도어 열림 알림 (E79524)
    void updateEventChargeFlapDoorSwitchStatusEV() {
        SFCVCUEventChargeFlapDoorSwitchEVStat stat = SFCVCUEventChargeFlapDoorSwitchEVStat::OFF;

        if (mConfigVehicleType == ConfigVehicleType::EV && mIgnElapsed == IgnElapsed::ON_0ms &&
            mChargeFlapDoorSwitchStatus == ChargeFlapDoorSwitchStatus::ON) {
            stat = SFCVCUEventChargeFlapDoorSwitchEVStat::ON;
        }

        setSFCVCUEventChargeFlapDoorSwitchEVStat(stat);
    }

    // 5.3.20 주행 가능 상태 알림 (E79525)
    void updateEventReady() {
        SFCVCUEventReadyStat stat = SFCVCUEventReadyStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV || mConfigVehicleType == ConfigVehicleType::EV) {
                if (mFCReadyStatus == FCReadyStatus::FC_READY || mEvModeStatus == EvModeStatus::EV_MODE) {
                    stat = SFCVCUEventReadyStat::ON;
                }
            }
        }
        setSFCVCUEventReadyStat(stat);
    }

    // 5.3.21 현재 기어단 선택 알림 (E79526)
    void updateEventSelletedGear() {
        SFCVCUEventSellectedGearStat stat = SFCVCUEventSellectedGearStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV || mConfigVehicleType == ConfigVehicleType::EV) {
                if (mSbwWarningStatusVCU == SbwWarningStatusVCU::ON) {
                    stat = SFCVCUEventSellectedGearStat::ON;
                }
            }
        }
        setSFCVCUEventSellectedGearStat(stat);
    }

    // 5.3.22 최고속도 가변 (E79528, E79529, E79530)
    void updateEventMaxSpeedVariableStatus() {
        std::string eventID;
        std::string linkedSoundID;

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mMaxSpeedStatus_VCU == MaxSpeedStatus_VCU::INHIBIT_INCREASE) {
                eventID = "E79528";
                linkedSoundID = "SND_PopUpInform1";
            } else if (mMaxSpeedStatus_VCU == MaxSpeedStatus_VCU::INCREASE) {
                eventID = "E79529";
                linkedSoundID = "SND_PopUpInform1";
            } else if (mMaxSpeedStatus_VCU == MaxSpeedStatus_VCU::DECREASE) {
                eventID = "E79530";
                linkedSoundID = "SND_PopUpInform2";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.VCU.Event.MaxSpeedVariable.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCVCUEventMaxSpeedVariableStat(SFCVCUEventMaxSpeedVariableStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCVCUEventMaxSpeedVariableID(eventID);
            setSFCVCUEventMaxSpeedVariableStat(SFCVCUEventMaxSpeedVariableStat::ON);
            setSFCVCUEventMaxSpeedVariableLinkedSoundID(linkedSoundID);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsVehicleTypeChanged::Inter_ConfigVehicleType;
    using EvModeStatus = ArgumentsReadyStatChanged::Input_EvModeStatus;
    using FCReadyStatus = ArgumentsReadyStatChanged::Input_FCReadyStatus;

    using VCUToAuxInvCommFaultStatus = ArgumentsVcuToAuxInvCommFaultStatusChanged::Input_VCUToAuxInvCommFaultStatus;
    using VCUToBMSCommFaultStatus = ArgumentsVcuToBMSCommFaultStatusChanged::Input_VCUToBMSCommFaultStatus;
    using VCUToFCU1CommFaultStatus = ArgumentsVcuToFCU1CommFaultStatusChanged::Input_VCUToFCU1CommFaultStatus;
    using VCUToFCU2CommFaultStatus = ArgumentsVcuToFCU2CommFaultStatusChanged::Input_VCUToFCU2CommFaultStatus;
    using VCUToHMU1CommFaultStatus = ArgumentsVcuToHMU1CommFaultStatusChanged::Input_VCUToHMU1CommFaultStatus;
    using VCUToLDCCommFaultStatus = ArgumentsVcuToLDCCommFaultStatusChanged::Input_VCUToLDCCommFaultStatus;
    using VCUToMCUCommFaultStatus = ArgumentsVcuToMCUCommFaultStatusChanged::Input_VCUToMCUCommFaultStatus;
    using ServiceIndiReqStatusFromVCU = ArgumentsVcuFaultStatChanged::Input_ServiceIndiReqStatusFromVCU;
    using VCUFaultECStatus = ArgumentsVcuFaultStatChanged::Input_VCUFaultECStatus;
    using BusOffECStatus = ArgumentsBusOffECStatusChanged::Input_BusOffECStatus;
    using EBSCtoECStatus = ArgumentsEbsCtoECStatusChanged::Input_EBSCtoECStatus;
    using TCUCtoECStatus = ArgumentsTcuCtoECStatusChanged::Input_TCUCtoECStatus;
    using EMSCtoECStatus = ArgumentsEmsCtoECStatusChanged::Input_EMSCtoECStatus;
    using IBUCtoECStatus = ArgumentsIbuCtoECStatusChanged::Input_IBUCtoECStatus;
    using IMUCtoECStatus = ArgumentsImuCtoECStatusChange::Input_IMUCtoECStatus;
    using IOBXCtoECStatus = ArgumentsIobxCtoECStatusChanged::Input_IOBXCtoECStatus;
    using IPSMCtoECStatus = ArgumentsIpsmCtoECStatusChanged::Input_IPSMCtoECStatus;
    using MFSMCtoECStatus = ArgumentsMfsmCtoECStatusChanged::Input_MFSMCtoECStatus;
    using PDCCtoECStatus = ArgumentsPdcCtoECStatusChanged::Input_PDCCtoECStatus;
    using SPACtoECStatus = ArgumentsSpaCtoECStatusChanged::Input_SPACtoECStatus;
    using ADASDRVCtoECStatus = ArgumentsAdasDRVCtoECStatusChanged::Input_ADAS_DRVCtoECStatus;
    using DTGCtoECStatus = ArgumentsDtgCtoECStatusChanged::Input_DTGCtoECStatus;
    using EAPUCtoECStatus = ArgumentsEapuCtoECStatusChanged::Input_EAPUCtoECStatus;
    using EPBCtoECStatus = ArgumentsEpbCtoECStatusChanged::Input_EPBCtoECStatus;

    using MainRelayOpenStatus = ArgumentsMainRelayOpenStatusChanged::Input_MainRelayOpenStatus;
    using StartInterLockStatus = ArgumentsStartInterLockStatusChanged::Input_StartInterLockStatus;
    using DoorOpenAccelStatus = ArgumentsDoorOpenAccelStatusChanged::Input_DoorOpenAccelStatus;
    using StartBrakeOffStatus = ArgumentsStartBrakeOffStatusChanged::Input_StartBrakeOffStatus;
    using BrakePedalOverrideStatus = ArgumentsBrakePedalOverrideStatusChanged::Input_BrakePedalOverrideStatus;
    using PowerDownStatus = ArgumentsPowerDownStatusChanged::Input_PowerDownStatus;
    using AccelPedalInfoStatus = ArgumentsAccelPedalInfoStatusChanged::Input_AccelPedalInfoStatus;
    using SafetySwitchStatus = ArgumentsSafetySwitchStatusChanged::Input_SafetySwitchStatus;
    using ChargeSafetySwitchStatus = ArgumentsChargeSafetySwitchStatusChanged::Input_ChargeSafetySwitchStatus;
    using ChargeFlapDoorSwitchStatus = ArgumentsChargeFlapDoorSwitchStatusChanged::Input_ChargeFlapDoorSwitchStatus;
    using AuxBatteryChargeStatus = ArgumentsAuxBatteryChargeStatusChanged::Input_AuxBatteryChargeStatus;
    using StartPreChargeInfoStatus = ArgumentsStartPreChargeInfoStatusChanged::Input_StartPreChargeInfoStatus;
    using EmergencyEvWarnStatus = ArgumentsEmergencyEvWarnStatusChanged::Input_EmergencyEvWarnStatus;
    using PlugConnStatus = ArgumentsPlugConnStatusChanged::Input_PlugConnStatus;
    using UtilityModeStatus = ArgumentsUtilityModeStatusChanged::Input_UtilityModeStatus;
    using MaxSpeedStatus_VCU = ArgumentsMaxSpeedStatusVCUChanged::Input_MaxSpeedStatus_VCU;
    using ChangeParkingOffStatus = ArgumentsChangeParkingOffStatusChanged::Input_ChangeParkingOffStatus;
    using SbwWarningStatusVCU = ArgumentsSbwWarningStatusVCUChanged::Input_SbwWarningStatus_VCU;
    using TransCurrentRangeStatus = ArgumentsTransCurrentRangeStatusChanged::Input_TransCurrentRangeStatus;
    using MCCDisplayStatus = ArgumentsMccDisplayStatusChanged::Input_MCCDisplayStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EvModeStatus mEvModeStatus = EvModeStatus::OFF;
    FCReadyStatus mFCReadyStatus = FCReadyStatus::OFF;

    SFCVCUConstantAuxInvCommFaultStat mSFCVCUConstantAuxInvCommFaultStat = SFCVCUConstantAuxInvCommFaultStat::OFF;
    SFCVCUConstantBMSCommFaultStat mSFCVCUConstantBMSCommFaultStat = SFCVCUConstantBMSCommFaultStat::OFF;
    SFCVCUConstantFCU1CommFaultStat mSFCVCUConstantFCU1CommFaultStat = SFCVCUConstantFCU1CommFaultStat::OFF;
    SFCVCUConstantFCU2CommFaultStat mSFCVCUConstantFCU2CommFaultStat = SFCVCUConstantFCU2CommFaultStat::OFF;
    SFCVCUConstantHMU1CommFaultStat mSFCVCUConstantHMU1CommFaultStat = SFCVCUConstantHMU1CommFaultStat::OFF;
    SFCVCUConstantLDCCommFaultStat mSFCVCUConstantLDCCommFaultStat = SFCVCUConstantLDCCommFaultStat::OFF;
    SFCVCUConstantMCUCommFaultStat mSFCVCUConstantMCUCommFaultStat = SFCVCUConstantMCUCommFaultStat::OFF;
    SFCVCUConstantVCUFaultStat mSFCVCUConstantVCUFaultStat = SFCVCUConstantVCUFaultStat::OFF;
    SFCVCUConstantVCUCANBusOffStat mSFCVCUConstantVCUCANBusOffStat = SFCVCUConstantVCUCANBusOffStat::OFF;
    SFCVCUConstantEBSCommFaultStat mSFCVCUConstantEBSCommFaultStat = SFCVCUConstantEBSCommFaultStat::OFF;
    SFCVCUConstantTCUCommFaultStat mSFCVCUConstantTCUCommFaultStat = SFCVCUConstantTCUCommFaultStat::OFF;
    SFCVCUConstantEMSCommFaultStat mSFCVCUConstantEMSCommFaultStat = SFCVCUConstantEMSCommFaultStat::OFF;
    SFCVCUConstantIBUCommFaultStat mSFCVCUConstantIBUCommFaultStat = SFCVCUConstantIBUCommFaultStat::OFF;
    SFCVCUConstantIMUCommFaultStat mSFCVCUConstantIMUCommFaultStat = SFCVCUConstantIMUCommFaultStat::OFF;
    SFCVCUConstantIOBXCommFaultStat mSFCVCUConstantIOBXCommFaultStat = SFCVCUConstantIOBXCommFaultStat::OFF;
    SFCVCUConstantIPSMCommFaultStat mSFCVCUConstantIPSMCommFaultStat = SFCVCUConstantIPSMCommFaultStat::OFF;
    SFCVCUConstantMFSMCommFaultStat mSFCVCUConstantMFSMCommFaultStat = SFCVCUConstantMFSMCommFaultStat::OFF;
    SFCVCUConstantPDCCommFaultStat mSFCVCUConstantPDCCommFaultStat = SFCVCUConstantPDCCommFaultStat::OFF;
    SFCVCUConstantSPACommFaultStat mSFCVCUConstantSPACommFaultStat = SFCVCUConstantSPACommFaultStat::OFF;
    SFCVCUConstantADASDRVCommFaultStat mSFCVCUConstantADASDRVCommFaultStat = SFCVCUConstantADASDRVCommFaultStat::OFF;
    SFCVCUConstantDTGCommFaultStat mSFCVCUConstantDTGCommFaultStat = SFCVCUConstantDTGCommFaultStat::OFF;
    SFCVCUConstantEAPUCommFaultStat mSFCVCUConstantEAPUCommFaultStat = SFCVCUConstantEAPUCommFaultStat::OFF;
    SFCVCUConstantEPBCommFaultStat mSFCVCUConstantEPBCommFaultStat = SFCVCUConstantEPBCommFaultStat::OFF;

    VCUToAuxInvCommFaultStatus mVCUToAuxInvCommFaultStatus = VCUToAuxInvCommFaultStatus::OFF;
    VCUToBMSCommFaultStatus mVCUToBMSCommFaultStatus = VCUToBMSCommFaultStatus::OFF;
    VCUToFCU1CommFaultStatus mVCUToFCU1CommFaultStatus = VCUToFCU1CommFaultStatus::OFF;
    VCUToFCU2CommFaultStatus mVCUToFCU2CommFaultStatus = VCUToFCU2CommFaultStatus::OFF;
    VCUToHMU1CommFaultStatus mVCUToHMU1CommFaultStatus = VCUToHMU1CommFaultStatus::OFF;
    VCUToLDCCommFaultStatus mVCUToLDCCommFaultStatus = VCUToLDCCommFaultStatus::OFF;
    VCUToMCUCommFaultStatus mVCUToMCUCommFaultStatus = VCUToMCUCommFaultStatus::OFF;
    ServiceIndiReqStatusFromVCU mServiceIndiReqStatusFromVCU = ServiceIndiReqStatusFromVCU::OFF;
    VCUFaultECStatus mVCUFaultECStatus = VCUFaultECStatus::OFF;
    BusOffECStatus mBusOffECStatus = BusOffECStatus::OFF;
    EBSCtoECStatus mEBSCtoECStatus = EBSCtoECStatus::OFF;
    TCUCtoECStatus mTCUCtoECStatus = TCUCtoECStatus::OFF;
    EMSCtoECStatus mEMSCtoECStatus = EMSCtoECStatus::OFF;
    IBUCtoECStatus mIBUCtoECStatus = IBUCtoECStatus::OFF;
    IMUCtoECStatus mIMUCtoECStatus = IMUCtoECStatus::OFF;
    IOBXCtoECStatus mIOBXCtoECStatus = IOBXCtoECStatus::OFF;
    IPSMCtoECStatus mIPSMCtoECStatus = IPSMCtoECStatus::OFF;
    MFSMCtoECStatus mMFSMCtoECStatus = MFSMCtoECStatus::OFF;
    PDCCtoECStatus mPDCCtoECStatus = PDCCtoECStatus::OFF;
    SPACtoECStatus mSPACtoECStatus = SPACtoECStatus::OFF;
    ADASDRVCtoECStatus mADASDRVCtoECStatus = ADASDRVCtoECStatus::OFF;
    DTGCtoECStatus mDTGCtoECStatus = DTGCtoECStatus::OFF;
    EAPUCtoECStatus mEAPUCtoECStatus = EAPUCtoECStatus::OFF;
    EPBCtoECStatus mEPBCtoECStatus = EPBCtoECStatus::OFF;
    MainRelayOpenStatus mMainRelayOpenStatus = MainRelayOpenStatus::OFF;
    StartInterLockStatus mStartInterLockStatus = StartInterLockStatus::OFF;
    DoorOpenAccelStatus mDoorOpenAccelStatus = DoorOpenAccelStatus::OFF;
    StartBrakeOffStatus mStartBrakeOffStatus = StartBrakeOffStatus::OFF;
    BrakePedalOverrideStatus mBrakePedalOverrideStatus = BrakePedalOverrideStatus::OFF;
    PowerDownStatus mPowerDownStatus = PowerDownStatus::OFF;
    AccelPedalInfoStatus mAccelPedalInfoStatus = AccelPedalInfoStatus::OFF;
    SafetySwitchStatus mSafetySwitchStatus = SafetySwitchStatus::OFF;
    ChargeSafetySwitchStatus mChargeSafetySwitchStatus = ChargeSafetySwitchStatus::OFF;
    ChargeFlapDoorSwitchStatus mChargeFlapDoorSwitchStatus = ChargeFlapDoorSwitchStatus::OFF;
    AuxBatteryChargeStatus mAuxBatteryChargeStatus = AuxBatteryChargeStatus::OFF;
    StartPreChargeInfoStatus mStartPreChargeInfoStatus = StartPreChargeInfoStatus::OFF;
    EmergencyEvWarnStatus mEmergencyEvWarnStatus = EmergencyEvWarnStatus::OFF;
    PlugConnStatus mPlugConnStatus = PlugConnStatus::OFF;
    UtilityModeStatus mUtilityModeStatus = UtilityModeStatus::OFF;
    UtilityModeStatus mPrevUtilityModeStatus = UtilityModeStatus::OFF;
    ChangeParkingOffStatus mChangeParkingOffStatus = ChangeParkingOffStatus::OFF;
    SbwWarningStatusVCU mSbwWarningStatusVCU = SbwWarningStatusVCU::OFF;
    TransCurrentRangeStatus mTransCurrentRangeStatus = TransCurrentRangeStatus::OFF;
    MaxSpeedStatus_VCU mMaxSpeedStatus_VCU = MaxSpeedStatus_VCU::OFF;
    MCCDisplayStatus mMCCDisplayStatus = MCCDisplayStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_VCU_H
