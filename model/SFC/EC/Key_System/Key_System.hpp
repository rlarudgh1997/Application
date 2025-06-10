/**
 * @file Key_System.hpp
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
#ifndef SFSS_Key_System_H
#define SFSS_Key_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Key_SystemBase.hpp"
#include <SFCTimer.h>
#include <Vehicle.h>

namespace ccos {

// SFC Version : 13.0.1
// [EC080] Key System. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class Key_System : public Key_SystemBase {
public:
    Key_System() = default;
    ~Key_System() override = default;
    Key_System(const Key_System& other) = delete;
    Key_System(Key_System&& other) noexcept = delete;
    Key_System& operator=(const Key_System& other) = delete;
    Key_System& operator=(Key_System&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerImmoIndiReqCANStatus.create(kTimerTelltaleInterval200ms, this,
                                                     &Key_System::onOneShotTimerImmoIndiReqCANStatusChanged, false) == false) {
            DWarning() << "[Key_System] mOneShotTimerImmoIndiReqCANStatus Create Failed";
        }

        setSFCKey_SystemSoundIDOutType(SFCKey_SystemSoundIDOutType::INFINITE);

        setSFCKey_SystemSoundSMKDeactivationID("SND_DeactIDWarnSMK");
        setSFCKey_SystemSoundSMKDeactivationType(SFCKey_SystemSoundSMKDeactivationType::INFINITE);

        setSFCKey_SystemSoundKeyBattLowID("SND_LowKeyBattery");
        setSFCKey_SystemSoundKeyBattLowType(SFCKey_SystemSoundKeyBattLowType::REPEAT_COUNT);
        setSFCKey_SystemSoundKeyBattLowRepeatCount(3);

        setSFCKey_SystemSoundESCLWarnID("SND_ESCLNotLockWarn");
        setSFCKey_SystemSoundESCLWarnType(SFCKey_SystemSoundESCLWarnType::INFINITE);

        setSFCKey_SystemEventFingerprintRecognitionWarningLinkedSoundID("SND_PopUpInform2");
        setSFCKey_SystemEventFingerprintRecognitionWarningLinkedSoundType(
            SFCKey_SystemEventFingerprintRecognitionWarningLinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventFingerprintRecognitionWarningLinkedSoundRepeatCount(1);

        setSFCKey_SystemEventDigitalKeyAuthenticationFailedLinkedSoundType(
            SFCKey_SystemEventDigitalKeyAuthenticationFailedLinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventDigitalKeyAuthenticationFailedLinkedSoundRepeatCount(1);

        setSFCKey_SystemEventFailToStartLinkedSoundID("SND_PopUpInform2");

        setSFCKey_SystemEventFailToStart2LinkedSoundID("SND_PopUpInform2");
        setSFCKey_SystemEventFailToStart2LinkedSoundType(SFCKey_SystemEventFailToStart2LinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventFailToStart2LinkedSoundRepeatCount(1);

        setSFCKey_SystemEventSystemFailLinkedSoundID("SND_PopUpWarn1");
        setSFCKey_SystemEventSystemFailLinkedSoundType(SFCKey_SystemEventSystemFailLinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventSystemFailLinkedSoundRepeatCount(1);

        setSFCKey_SystemEventDigitalKeyRecertificationRequestLinkedSoundID("SND_PopUpInform2");
        setSFCKey_SystemEventDigitalKeyRecertificationRequestLinkedSoundType(
            SFCKey_SystemEventDigitalKeyRecertificationRequestLinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventDigitalKeyRecertificationRequestLinkedSoundRepeatCount(1);

        setSFCKey_SystemEventServiceModeConditionLinkedSoundID("SND_PopUpInform2");
        setSFCKey_SystemEventServiceModeConditionLinkedSoundType(
            SFCKey_SystemEventServiceModeConditionLinkedSoundType::REPEAT_COUNT);
        setSFCKey_SystemEventServiceModeConditionLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        HBool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == false) {
            return;
        }

        mIgnElapsed = IgnElapsed::ON_0ms;
        mIgnOffStatus = false;
        updateACCIndicator();
        updateDigitalKeyAuthenticationFail();
        updateShiftToPRequest();
        updateSystemFail();
        updateSoundESCLWarn();
        updateSoundBCMWarn();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        HBool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == false) {
            return;
        }

        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIgnOffStatus = true;
        updateACCIndicator();
        updateDigitalKeyAuthenticationFail();
        updateShiftToPRequest();
        updateSystemFail();
        updateSoundESCLWarn();
        updateSoundBCMWarn();
    }

    void onIgnElapseOff700msChanged(const ArgumentsIgnElapseOff700msChanged& args) {
        HBool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff700ms);
        if (initState == false) {
            return;
        }

        mIgnElapsed = IgnElapsed::OFF_700ms;
        updateShiftToPRequest();
    }

    void onConfigSBWChanged(const ArgumentsConfigSBWChanged& args) {
        DDebug() << "Inter_ConfigSBW: " << static_cast<HUInt64>(args.mInter_ConfigSBW);
        mConfigSBW = args.mInter_ConfigSBW;
        updateShiftToPRequest();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        DDebug() << "Inter_ConfigVehicleType: " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateShiftToPRequest();
    }

    void onConfigSMKChanged(const ArgumentsConfigSMKChanged& args) {
        DDebug() << "Inter_ConfigSMK: " << static_cast<HUInt64>(args.mInter_ConfigSMK);
        mConfigSMK = args.mInter_ConfigSMK;
        updateSystemFail();
    }

    void onImmoIndiReqCANStatusChanged(const ArgumentsImmoIndiReqCANStatusChanged& args) {
        DDebug() << "Input_ImmoIndiReqCANStatus: " << static_cast<HUInt64>(args.mInput_ImmoIndiReqCANStatus);
        mOneShotTimerImmoIndiReqCANStatus.stop();

        if (args.mInput_ImmoIndiReqCANStatus == ImmoIndiReqCANStatus::ON) {
            if (mOneShotTimerImmoIndiReqCANStatus.start() == false) {
                DWarning() << "[Key_System] mOneShotTimerImmoIndiReqCANStatus Start Failed";
            }
        } else {
            setSFCKey_SystemTelltaleKey_SystemStat(SFCKey_SystemTelltaleKey_SystemStat::OFF);
        }
    }

    void onACCIndicatorChanged(const ArgumentsACCIndicatorChanged& args) {
        DDebug() << "Input_AccessoryIndiReqStatus: " << static_cast<HUInt64>(args.mInput_AccessoryIndiReqStatus);
        mAccessoryIndiReqStatus = args.mInput_AccessoryIndiReqStatus;
        updateACCIndicator();
    }

    void onIAUWarnReqStatusChanged(const ArgumentsIAUWarnReqStatusChanged& args) {
        DDebug() << "Input_IauWarnReqStatus: " << static_cast<HUInt64>(args.mInput_IauWarnReqStatus);
        mIAUWarnReqStatus = args.mInput_IauWarnReqStatus;
        updateDigitalKeyAuthenticationFail();
    }

    void onSMKWarnDisplayReqStatusChanged(const ArgumentsSMKWarnDisplayReqStatusChanged& args) {
        mSMKWarnDisplayReqStatus = args.mInput_SmkWarnDisplayReqStatus;
        updateShiftToPRequest();
        updateSMKWarnDisplayReqFailToStart();
        updateSystemWarning();
        updateKeyUnrecognized();
        updateVehicleIsOnByDigitalKey();
        updateSystemFail();
        updateDigitalKeyRecertificationRequestEvent();
    }

    void onIDOutWarnStatusChanged(const ArgumentsIDOutWarnStatusChanged& args) {
        mIDOutWarnStatus = args.mInput_IdOutWarnStatus;
        updateSoundIDOut();
    }

    void onSMKDeactivationChanged(const ArgumentsSMKDeactivationChanged& args) {
        mSMKDeactivationWarnStatus = args.mInput_SmkDeactivationWarnStatus;
        updateSoundSMKDeactivationWarn();
    }

    void onKeyBattLowWarnStatusChanged(const ArgumentsKeyBattLowWarnStatusChanged& args) {
        mKeyBattLowWarnStatus = args.mInput_KeyBattLowWarnStatus;
        updateSoundKeyBattLow();
    }

    void onESCLWarnSoundStatusChanged(const ArgumentsESCLWarnSoundStatusChanged& args) {
        mESCLWarnSoundStatus = args.mInput_EsclWarnSoundStatus;
        updateSoundESCLWarn();
    }

    void onBCMWarnSoundStatusChanged(const ArgumentsBCMWarnSoundStatusChanged& args) {
        mBCMWarnSoundStatus = args.mInput_BcmWarnSoundStatus;
        updateSoundBCMWarn();
    }

    void onSMKWarnDisplayReq2StatusChanged(const ArgumentsSMKWarnDisplayReq2StatusChanged& args) {
        mSMKWarnDisplayReq2Status = args.mInput_SmkWarnDisplayReq2Status;
        updateSMKWarnDisplayReq2FailToStart();
        updateServiceModeConditionEvent();
    }

    void onFingerPrintWarnReqStatusChanged(const ArgumentsFingerPrintWarnReqStatusChanged& args) {
        mFingerPrintWarnReqStatus = args.mInput_FingerPrintWarnReqStatus;
        updateFpsTimeLimitValueConstant();
        updateFingerprintRecognitionWarningEvent();
    }

    void onServiceModeIndicatorChanged(const ArgumentsServiceModeIndicatorChanged& args) {
        mServiceModeIndicator = args.mInput_ServiceModeIndiReqStatus;
        updateServiceModeIndicatorEvent();
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        DDebug() << "Key_System : onAEMInitializeCompleteChanged entered";
        DDebug() << "args.aeminit : " << args.mInter_AEMInitializeComplete;

        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            if (mAEMInitializeComplete == true) {
                updateKeySystemDispLogicVersionStatus();
                DDebug() << "Key_System : onAEMInitializeCompleteChanged";
                flush();
            } else {
                DDebug() << "Key_System : mAEMInitializeComplete not initialized";
            }
        } else {
            DDebug() << "AEM updateValue is invalid";
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void onOneShotTimerImmoIndiReqCANStatusChanged() {
        setSFCKey_SystemTelltaleKey_SystemStat(SFCKey_SystemTelltaleKey_SystemStat::ON);
        flush();
    }

    void updateKeySystemDispLogicVersionStatus() {
        AddressValueList outputAddressList;
        decltype(Vehicle.EC.Key_System.Output_KeySystemDispLogicVersionStatus)::TYPE output_KeySystemDispLogicVersionStatus =
            decltype(Vehicle.EC.Key_System.Output_KeySystemDispLogicVersionStatus)::TYPE::NEW1;

        outputAddressList.emplace_back(Vehicle.EC.Key_System.Output_KeySystemDispLogicVersionStatus,
                                       static_cast<HUInt64>(output_KeySystemDispLogicVersionStatus));
        setValue(outputAddressList);
        outputAddressList.clear();
        DDebug() << "set output_KeySystemDispLogicVersionStatus";
    }

    void updateACCIndicator() {
        if ((mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::OFF_700ms) &&
            mAccessoryIndiReqStatus == AccessoryIndiReqStatus::ON) {
            setSFCKey_SystemEventACCIndicatorStat(SFCKey_SystemEventACCIndicatorStat::ON);
        } else {
            setSFCKey_SystemEventACCIndicatorStat(SFCKey_SystemEventACCIndicatorStat::OFF);
        }

        setSFCKey_SystemEventACCIndicatorID("E60701");
    }

    // NOTE (SHJ)
    // Input_FingerPrintWarnReqStatus에도 E60742가 있어 FingerprintRecognitionWarning 노드에만 이벤트 설정할 경우
    // Input_IAUWarnReqStatus 입력이 NFC_PRE_START_AUTH_SUCCESS 일 때,
    // Input_FingerPrintWarnReqStatus 입력이 RECOGNITION_SUCCESS_TELE이 되더라도 E60743이 출력되지 않고
    // E60742만 FingerprintRecognitionWarning 노드에 출력되어 문제 발생할 수 있음.
    // 이에 EventDigitalKeyAuthenticationFailed 노드와 FingerprintRecognitionWarning 노드
    // 양쪽에서 E60742 표출 가능하도록 함.
    void updateDigitalKeyAuthenticationFail() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mIAUWarnReqStatus == IAUWarnReqStatus::SYS_ERROR) {
            eventID = "E60704";
            linkedSoundID = "SND_PopUpWarn1";
        } else if (mIAUWarnReqStatus == IAUWarnReqStatus::RETRY_BY_KEY) {
            eventID = "E60702";
            linkedSoundID = "SND_PopUpInform2";
        } else if (mIAUWarnReqStatus == IAUWarnReqStatus::RE_AUTH_WARN) {
            eventID = "E60703";
            linkedSoundID = "SND_PopUpInform2";
        } else if (mIAUWarnReqStatus == IAUWarnReqStatus::RETRY_BY_FPS) {
            eventID = "E60739";
            linkedSoundID = "SND_PopUpInform2";
        } else if (mIAUWarnReqStatus == IAUWarnReqStatus::RETRY_BY_NFC_OR_FPS) {
            eventID = "E60740";
            linkedSoundID = "SND_PopUpInform2";
        } else if (mIAUWarnReqStatus == IAUWarnReqStatus::NFC_PRE_START_AUTH_SUCCESS) {
            eventID = "E60742";
            linkedSoundID = "SND_PopUpInform2";
        } else {
            // not specified in Section 5.7.5.3.2 Digital Key Authentication Failed
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.DigitalKeyAuthenticationFailed.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventDigitalKeyAuthenticationFailedStat(SFCKey_SystemEventDigitalKeyAuthenticationFailedStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventDigitalKeyAuthenticationFailedID(eventID);
            setSFCKey_SystemEventDigitalKeyAuthenticationFailedStat(SFCKey_SystemEventDigitalKeyAuthenticationFailedStat::ON);
            setSFCKey_SystemEventDigitalKeyAuthenticationFailedLinkedSoundID(linkedSoundID);
        }
    }

    void updateShiftToPRequest() {
        std::string eventID;
        std::string linkedSoundID;
        SFCKey_SystemEventShiftToPRequestLinkedSoundType linkedSoundType = SFCKey_SystemEventShiftToPRequestLinkedSoundType::NONE;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::SHIFT_P) {
            if (mConfigSBW == ConfigSBW::OFF) {
                eventID = "E60708";
            } else if (mConfigSBW == ConfigSBW::ON) {
                eventID = "E60709";
            } else {
                DWarning() << "Invalid ConfigSBW";
            }
        } else if (mIgnElapsed == IgnElapsed::OFF_700ms &&
                   mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::SHIFT_P_AND_START) {
            if (mConfigSBW == ConfigSBW::OFF) {
                eventID = "E60705";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCKey_SystemEventShiftToPRequestLinkedSoundType::INFINITE;
            } else if (mConfigSBW == ConfigSBW::ON) {
                if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                    eventID = "E60707";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCKey_SystemEventShiftToPRequestLinkedSoundType::INFINITE;
                } else {
                    eventID = "E60706";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCKey_SystemEventShiftToPRequestLinkedSoundType::INFINITE;
                }
            } else {
                DWarning() << "Invalid ConfigSBW";
            }
        } else {
            // not specified in Section 5.7.5.3.3 Shift to P Request
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.ShiftToPRequest.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventShiftToPRequestStat(SFCKey_SystemEventShiftToPRequestStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventShiftToPRequestID(eventID);
            setSFCKey_SystemEventShiftToPRequestStat(SFCKey_SystemEventShiftToPRequestStat::ON);
            setSFCKey_SystemEventShiftToPRequestLinkedSoundID(linkedSoundID);
            setSFCKey_SystemEventShiftToPRequestLinkedSoundType(linkedSoundType);
        }
    }

    void updateSMKWarnDisplayReqFailToStart() {
        std::string eventID;
        SFCKey_SystemEventFailToStartLinkedSoundType linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::NONE;
        HUInt64 count = 0;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::STEER_AND_START) {
            eventID = "E60711";
            linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::UNPLUG_AND_START) {
            eventID = "E60712";
            linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::START_BUTTON_AGAIN) {
            eventID = "E60720";
            linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::INFINITE;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::KEY_AND_START_BUTTON) {
            eventID = "E60722";
            linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::PLACE_KEY_AND_START_BUTTON) {
            eventID = "E60749";
            linkedSoundType = SFCKey_SystemEventFailToStartLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else {
            DInfo() << "Invalid Data";
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.FailToStart.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventFailToStartStat(SFCKey_SystemEventFailToStartStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventFailToStartID(eventID);
            setSFCKey_SystemEventFailToStartStat(SFCKey_SystemEventFailToStartStat::ON);
            setSFCKey_SystemEventFailToStartLinkedSoundType(linkedSoundType);
            setSFCKey_SystemEventFailToStartLinkedSoundRepeatCount(count);
        }

        DDebug() << "mSMKWarnDisplayReqStatus: " << static_cast<HUInt64>(mSMKWarnDisplayReqStatus);
    }

    void updateSMKWarnDisplayReq2FailToStart() {
        std::string eventID;
        if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::BRAKE_AND_START) {
            eventID = "E60715";
        } else if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::CLUTCH_AND_START) {
            eventID = "E60716";
        } else if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::BRAKE_CLUTCH_AND_START) {
            eventID = "E60718";
        } else if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::SHIFT_P_N_AND_START) {
            eventID = "E60723";
        } else if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::SHIFT_P_AND_START) {
            eventID = "E60724";
        } else if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::REAUTH_AFTER_REMOTE_START) {
            eventID = "E60744";
        } else {
            DInfo() << "Invalid Data";
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.FailToStart2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventFailToStart2Stat(SFCKey_SystemEventFailToStart2Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventFailToStart2ID(eventID);
            setSFCKey_SystemEventFailToStart2Stat(SFCKey_SystemEventFailToStart2Stat::ON);
        }

        DDebug() << "mSMKWarnDisplayReq2Status: " << static_cast<HUInt64>(mSMKWarnDisplayReq2Status);
    }

    void updateSystemWarning() {
        std::string eventID;
        std::string linkedSoundID;
        SFCKey_SystemEventSystemWarningLinkedSoundType linkedSoundType = SFCKey_SystemEventSystemWarningLinkedSoundType::NONE;
        HUInt64 count = 0;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::LOW_SMK_BATTERY) {
            eventID = "E60725";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CHECK_STEER_LOCK_SYSTEM) {
            eventID = "E60726";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCKey_SystemEventSystemWarningLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CHECK_BRAKE_SWITCH) {
            eventID = "E60727";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCKey_SystemEventSystemWarningLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else {
            // not specified in Section 5.7.5.3.5 System Warning
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.SystemWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventSystemWarningStat(SFCKey_SystemEventSystemWarningStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventSystemWarningID(eventID);
            setSFCKey_SystemEventSystemWarningStat(SFCKey_SystemEventSystemWarningStat::ON);
            setSFCKey_SystemEventSystemWarningLinkedSoundID(linkedSoundID);
            setSFCKey_SystemEventSystemWarningLinkedSoundType(linkedSoundType);
            setSFCKey_SystemEventSystemWarningLinkedSoundRepeatCount(count);
        }
    }

    void updateKeyUnrecognized() {
        std::string eventID;

        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::KEY_NOT_IN_VEHICLE) {
            eventID = "E60731";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::KEY_NOT_IN_VEHICLE_FOB_DK) {
            eventID = "E60745";
        } else {
            // OFF
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.KeyUnrecognized.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventKeyUnrecognizedStat(SFCKey_SystemEventKeyUnrecognizedStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventKeyUnrecognizedID(eventID);
            setSFCKey_SystemEventKeyUnrecognizedStat(SFCKey_SystemEventKeyUnrecognizedStat::ON);
        }
    }

    void updateVehicleIsOnByDigitalKey() {
        std::string eventID;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::VEHICLE_IS_ON_FPS) {
            eventID = "E60732";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::VEHICLE_IS_ON_NFC) {
            eventID = "E60733";
        } else {
            // not specified in Section 5.7.5.3.7 Vehicle is On by Digital Key
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.VehicleIsOnByDigitalKey.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventVehicleIsOnByDigitalKeyStat(SFCKey_SystemEventVehicleIsOnByDigitalKeyStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventVehicleIsOnByDigitalKeyID(eventID);
            setSFCKey_SystemEventVehicleIsOnByDigitalKeyStat(SFCKey_SystemEventVehicleIsOnByDigitalKeyStat::ON);
        }
    }

    void updateSystemFail() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::MESSAGE_TIMEOUT ||
                mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CRC_ERROR) {
                eventID = "E60734";
            }
        } else if (mIgnOffStatus == true) {
            if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CRC_ERROR) {
                eventID = "E60735";
            }
        } else {
            // not specified in Section 5.7.5.3.8 System Fail
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.SystemFail.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventSystemFailStat(SFCKey_SystemEventSystemFailStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventSystemFailID(eventID);
            setSFCKey_SystemEventSystemFailStat(SFCKey_SystemEventSystemFailStat::ON);
        }
    }

    void updateSoundIDOut() {
        std::string soundID;

        if (mIDOutWarnStatus == IDOutWarnStatus::WARN) {
            soundID = "SND_IDOut";
        } else if (mIDOutWarnStatus == IDOutWarnStatus::WARN2) {
            soundID = "SND_IDOut2";
        } else {
            // no operation
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.Key_System.Sound.IDOut.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCKey_SystemSoundIDOutStat(SFCKey_SystemSoundIDOutStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCKey_SystemSoundIDOutID(soundID);
            setSFCKey_SystemSoundIDOutStat(SFCKey_SystemSoundIDOutStat::ON);
        }
    }

    void updateSoundSMKDeactivationWarn() {
        if (mSMKDeactivationWarnStatus == SMKDeactivationWarnStatus::WARN) {
            setSFCKey_SystemSoundSMKDeactivationStat(SFCKey_SystemSoundSMKDeactivationStat::ON);
        } else {
            setSFCKey_SystemSoundSMKDeactivationStat(SFCKey_SystemSoundSMKDeactivationStat::OFF);
        }
    }

    void updateSoundKeyBattLow() {
        if (mKeyBattLowWarnStatus == KeyBattLowWarnStatus::WARN) {
            setSFCKey_SystemSoundKeyBattLowStat(SFCKey_SystemSoundKeyBattLowStat::ON);
        } else {
            setSFCKey_SystemSoundKeyBattLowStat(SFCKey_SystemSoundKeyBattLowStat::OFF);
        }
    }

    void updateSoundESCLWarn() {
        if ((mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::OFF_700ms) &&
            mESCLWarnSoundStatus == ESCLWarnSoundStatus::WARN) {
            setSFCKey_SystemSoundESCLWarnStat(SFCKey_SystemSoundESCLWarnStat::ON);
        } else {
            setSFCKey_SystemSoundESCLWarnStat(SFCKey_SystemSoundESCLWarnStat::OFF);
        }
    }

    void updateSoundBCMWarn() {
        std::string soundID;
        SFCKey_SystemSoundBCMWarnType type = SFCKey_SystemSoundBCMWarnType::NONE;
        HUInt64 rptCnt = 0;

        if (mBCMWarnSoundStatus == BCMWarnSoundStatus::KEY_OPER_WARN) {
            soundID = "SND_KeyOperatedWarn";
            type = SFCKey_SystemSoundBCMWarnType::INFINITE;
        } else if (mBCMWarnSoundStatus == BCMWarnSoundStatus::KEY_LEARN_WARN) {
            soundID = "SND_KeyLearning";
            type = SFCKey_SystemSoundBCMWarnType::REPEAT_COUNT;
            rptCnt = 1;
        } else if (mBCMWarnSoundStatus == BCMWarnSoundStatus::PARK_BRAKE_WARN && mIgnElapsed == IgnElapsed::ON_0ms) {
            soundID = "SND_ParkBrakeWarn";
            type = SFCKey_SystemSoundBCMWarnType::INFINITE;
        } else {
            // no operation
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.Key_System.Sound.BCMWarn.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCKey_SystemSoundBCMWarnStat(SFCKey_SystemSoundBCMWarnStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCKey_SystemSoundBCMWarnID(soundID);
            setSFCKey_SystemSoundBCMWarnType(type);
            setSFCKey_SystemSoundBCMWarnRepeatCount(rptCnt);
            setSFCKey_SystemSoundBCMWarnStat(SFCKey_SystemSoundBCMWarnStat::ON);
        }
    }

    void updateFpsTimeLimitValueConstant() {
        SFCKey_SystemConstantFpsTimeLimitValueStat stat = SFCKey_SystemConstantFpsTimeLimitValueStat::VALUE;
        HUInt64 value = 0;

        if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_1MIN) {
            value = 1;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_2MIN) {
            value = 2;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_3MIN) {
            value = 3;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_4MIN) {
            value = 4;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_5MIN) {
            value = 5;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_6MIN) {
            value = 6;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_7MIN) {
            value = 7;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_8MIN) {
            value = 8;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_9MIN) {
            value = 9;
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::LIMIT_10MIN) {
            value = 10;
        } else {
            stat = SFCKey_SystemConstantFpsTimeLimitValueStat::DISPLAY_OFF;
        }

        setSFCKey_SystemConstantFpsTimeLimitValueStat(stat);
        setSFCKey_SystemConstantFpsTimeLimitValue(value);

        DDebug() << "mFingerPrintWarnReqStatus: " << static_cast<HUInt64>(mFingerPrintWarnReqStatus);
    }

    void updateDigitalKeyRecertificationRequestEvent() {
        std::string eventID;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_NFC) {
            eventID = "E60736";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_FPS) {
            eventID = "E60737";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_NFC_FPS) {
            eventID = "E60738";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_FOB_DK) {
            eventID = "E60746";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_FOB_FP) {
            eventID = "E60747";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_FOB_DK_FP) {
            eventID = "E60748";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::REAUTH_WARN_RSPA_KEY) {
            eventID = "E60756";
        } else {
            // not specified in Section 5.7.5.3.9 Digital key recertification request
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.DigitalKeyRecertificationRequest.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventDigitalKeyRecertificationRequestStat(
                SFCKey_SystemEventDigitalKeyRecertificationRequestStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventDigitalKeyRecertificationRequestID(eventID);
            setSFCKey_SystemEventDigitalKeyRecertificationRequestStat(SFCKey_SystemEventDigitalKeyRecertificationRequestStat::ON);
        }
    }

    void updateFingerprintRecognitionWarningEvent() {
        std::string eventID;
        if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::RECOGNITION_SUCCESS) {
            eventID = "E60742";
        } else if (mFingerPrintWarnReqStatus == FingerPrintWarnReqStatus::RECOGNITION_SUCCESS_TELE) {
            eventID = "E60743";
        } else if ((mFingerPrintWarnReqStatus != FingerPrintWarnReqStatus::RECOGNITION_SUCCESS) &&
                   (mFingerPrintWarnReqStatus != FingerPrintWarnReqStatus::RECOGNITION_SUCCESS_TELE) &&
                   (mFingerPrintWarnReqStatus != FingerPrintWarnReqStatus::OFF)) {
            eventID = "E60741";
        } else {
            // not specified in Section 5.7.5.3.10 Fingerprint recognition warning
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System.Event.FingerprintRecognitionWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_SystemEventFingerprintRecognitionWarningStat(SFCKey_SystemEventFingerprintRecognitionWarningStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_SystemEventFingerprintRecognitionWarningID(eventID);
            setSFCKey_SystemEventFingerprintRecognitionWarningStat(SFCKey_SystemEventFingerprintRecognitionWarningStat::ON);
        }
    }

    void updateServiceModeIndicatorEvent() {
        SFCKey_SystemEventServiceModeIndicatorStat stat = SFCKey_SystemEventServiceModeIndicatorStat::OFF;

        if (mServiceModeIndicator == ServiceModeIndicator::ON) {
            stat = SFCKey_SystemEventServiceModeIndicatorStat::ON;
        }

        setSFCKey_SystemEventServiceModeIndicatorID("E60754");
        setSFCKey_SystemEventServiceModeIndicatorStat(stat);
    }

    void updateServiceModeConditionEvent() {
        SFCKey_SystemEventServiceModeConditionStat stat = SFCKey_SystemEventServiceModeConditionStat::OFF;

        if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::SERVICEMODE_FAIL) {
            stat = SFCKey_SystemEventServiceModeConditionStat::ON;
        }

        setSFCKey_SystemEventServiceModeConditionID("E60755");
        setSFCKey_SystemEventServiceModeConditionStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        OFF_700ms,
        MAX
    };

    using ConfigSBW = ArgumentsConfigSBWChanged::Inter_ConfigSBW;
    using ConfigSMK = ArgumentsConfigSMKChanged::Inter_ConfigSMK;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ImmoIndiReqCANStatus = ArgumentsImmoIndiReqCANStatusChanged::Input_ImmoIndiReqCANStatus;
    using AccessoryIndiReqStatus = ArgumentsACCIndicatorChanged::Input_AccessoryIndiReqStatus;
    using IAUWarnReqStatus = ArgumentsIAUWarnReqStatusChanged::Input_IauWarnReqStatus;
    using SMKWarnDisplayReqStatus = ArgumentsSMKWarnDisplayReqStatusChanged::Input_SmkWarnDisplayReqStatus;
    using IDOutWarnStatus = ArgumentsIDOutWarnStatusChanged::Input_IdOutWarnStatus;
    using SMKDeactivationWarnStatus = ArgumentsSMKDeactivationChanged::Input_SmkDeactivationWarnStatus;
    using KeyBattLowWarnStatus = ArgumentsKeyBattLowWarnStatusChanged::Input_KeyBattLowWarnStatus;
    using ESCLWarnSoundStatus = ArgumentsESCLWarnSoundStatusChanged::Input_EsclWarnSoundStatus;
    using BCMWarnSoundStatus = ArgumentsBCMWarnSoundStatusChanged::Input_BcmWarnSoundStatus;
    using FingerPrintWarnReqStatus = ArgumentsFingerPrintWarnReqStatusChanged::Input_FingerPrintWarnReqStatus;
    using ServiceModeIndicator = ArgumentsServiceModeIndicatorChanged::Input_ServiceModeIndiReqStatus;
    using SMKWarnDisplayReq2Status = ArgumentsSMKWarnDisplayReq2StatusChanged::Input_SmkWarnDisplayReq2Status;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigSBW mConfigSBW = ConfigSBW::OFF;
    ConfigSMK mConfigSMK = ConfigSMK::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    AccessoryIndiReqStatus mAccessoryIndiReqStatus = AccessoryIndiReqStatus::OFF;
    IAUWarnReqStatus mIAUWarnReqStatus = IAUWarnReqStatus::OFF;
    SMKWarnDisplayReqStatus mSMKWarnDisplayReqStatus = SMKWarnDisplayReqStatus::OFF;
    IDOutWarnStatus mIDOutWarnStatus = IDOutWarnStatus::OFF;
    SMKDeactivationWarnStatus mSMKDeactivationWarnStatus = SMKDeactivationWarnStatus::OFF;
    KeyBattLowWarnStatus mKeyBattLowWarnStatus = KeyBattLowWarnStatus::OFF;
    ESCLWarnSoundStatus mESCLWarnSoundStatus = ESCLWarnSoundStatus::OFF;
    BCMWarnSoundStatus mBCMWarnSoundStatus = BCMWarnSoundStatus::OFF;
    FingerPrintWarnReqStatus mFingerPrintWarnReqStatus = FingerPrintWarnReqStatus::OFF;
    ServiceModeIndicator mServiceModeIndicator = ServiceModeIndicator::OFF;
    SMKWarnDisplayReq2Status mSMKWarnDisplayReq2Status = SMKWarnDisplayReq2Status::OFF;
    HBool mIgnOffStatus = false;
    HBool mAEMInitializeComplete = false;

    ssfs::SFCTimer<Key_System> mOneShotTimerImmoIndiReqCANStatus;
    static constexpr uint32_t kTimerTelltaleInterval200ms = 200u;
};

}  // namespace ccos

#endif  // SFSS_Key_System_H
