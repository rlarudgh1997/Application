/**
 * @file Key_System_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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
#ifndef SFSS_Key_System_2_0_H
#define SFSS_Key_System_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Key_System_2_0Base.hpp"
#include <SFCTimer.h>
#include <Vehicle.h>

namespace ccos {

// SFC Version : 1.3.1
// [EC081] Key_System_2_0. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class Key_System_2_0 : public Key_System_2_0Base {
public:
    Key_System_2_0() = default;
    ~Key_System_2_0() override = default;
    Key_System_2_0(const Key_System_2_0& other) = delete;
    Key_System_2_0(Key_System_2_0&& other) noexcept = delete;
    Key_System_2_0& operator=(const Key_System_2_0& other) = delete;
    Key_System_2_0& operator=(Key_System_2_0&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerImmoIndiReqCANStatus.create(
                kTimerTelltaleInterval200ms, this, &Key_System_2_0::onOneShotTimerImmoIndiReqCANStatusChanged, false) == false) {
            DWarning() << "[Key_System_2_0] mOneShotTimerImmoIndiReqCANStatus Create Failed";
        }

        setSFCKey_System_2_0SoundSMKDeactivationID("SND_DeactIDWarnSMK");
        setSFCKey_System_2_0SoundSMKDeactivationType(SFCKey_System_2_0SoundSMKDeactivationType::INFINITE);

        setSFCKey_System_2_0SoundKeyBattLowID("SND_LowKeyBattery");
        setSFCKey_System_2_0SoundKeyBattLowType(SFCKey_System_2_0SoundKeyBattLowType::REPEAT_COUNT);
        setSFCKey_System_2_0SoundKeyBattLowRepeatCount(3);

        setSFCKey_System_2_0SoundESCLWarnID("SND_ESCLNotLockWarn");
        setSFCKey_System_2_0SoundESCLWarnType(SFCKey_System_2_0SoundESCLWarnType::INFINITE);

        setSFCKey_System_2_0EventFingerprintRecognitionWarningLinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventFingerprintRecognitionWarningLinkedSoundType(
            SFCKey_System_2_0EventFingerprintRecognitionWarningLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventFingerprintRecognitionWarningLinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedLinkedSoundType(
            SFCKey_System_2_0EventDigitalKeyAuthenticationFailedLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedLinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventFailToStartLinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventFailToStart_1_1LinkedSoundID("SND_PopUpInform2");

        setSFCKey_System_2_0EventFailToStart2LinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventFailToStart2LinkedSoundType(SFCKey_System_2_0EventFailToStart2LinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventFailToStart2LinkedSoundRepeatCount(1);
        setSFCKey_System_2_0EventFailToStart_1_2LinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventFailToStart_1_2LinkedSoundType(
            SFCKey_System_2_0EventFailToStart_1_2LinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventFailToStart_1_2LinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventFailToStart_2LinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventFailToStart_2LinkedSoundType(SFCKey_System_2_0EventFailToStart_2LinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventFailToStart_2LinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventSystemFailLinkedSoundID("SND_PopUpWarn1");
        setSFCKey_System_2_0EventSystemFailLinkedSoundType(SFCKey_System_2_0EventSystemFailLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventSystemFailLinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventDigitalKeyRecertificationRequestLinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventDigitalKeyRecertificationRequestLinkedSoundType(
            SFCKey_System_2_0EventDigitalKeyRecertificationRequestLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventDigitalKeyRecertificationRequestLinkedSoundRepeatCount(1);

        setSFCKey_System_2_0EventServiceModeConditionLinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_2_0EventServiceModeConditionLinkedSoundType(
            SFCKey_System_2_0EventServiceModeConditionLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_2_0EventServiceModeConditionLinkedSoundRepeatCount(1);
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

    void onImmoIndiReqCANStatusChanged(const ArgumentsImmoIndiReqCANStatusChanged& args) {
        DDebug() << "Input_ImmoIndiReqCANStatus: " << static_cast<HUInt64>(args.mInput_ImmoIndiReqCANStatus);
        mOneShotTimerImmoIndiReqCANStatus.stop();

        if (args.mInput_ImmoIndiReqCANStatus == ImmoIndiReqCANStatus::ON) {
            if (mOneShotTimerImmoIndiReqCANStatus.start() == false) {
                DWarning() << "[Key_System_2_0] mOneShotTimerImmoIndiReqCANStatus Start Failed";
            }
        } else {
            setSFCKey_System_2_0TelltaleKey_System_2_0Stat(SFCKey_System_2_0TelltaleKey_System_2_0Stat::OFF);
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

    void onSMKWarnDisplayReq3StatusChanged(const ArgumentsSMKWarnDisplayReq3StatusChanged& args) {
        mSMKWarnDisplayReq3Status = args.mInput_SmkWarnDisplayReq3Status;
        updateSMKWarnDisplayReq3FailToStart2();
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

    void onEvVehicleStartStatusChanged(const ArgumentsEvVehicleStartStatusChanged& args) {
        mEvVehicleStartStatus = args.mInput_EvVehicleStartStatus;
        updateEvStartTemporaryStandbyStatus();
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
        setSFCKey_System_2_0TelltaleKey_System_2_0Stat(SFCKey_System_2_0TelltaleKey_System_2_0Stat::ON);
        flush();
    }

    void updateKeySystemDispLogicVersionStatus() {
        AddressValueList outputAddressList;
        decltype(Vehicle.EC.Key_System_2_0.Output_KeySystemDispLogicVersionStatus)::TYPE output_KeySystemDispLogicVersionStatus =
            decltype(Vehicle.EC.Key_System_2_0.Output_KeySystemDispLogicVersionStatus)::TYPE::NEW1;

        outputAddressList.emplace_back(Vehicle.EC.Key_System_2_0.Output_KeySystemDispLogicVersionStatus,
                                       static_cast<HUInt64>(output_KeySystemDispLogicVersionStatus));
        setValue(outputAddressList);
        outputAddressList.clear();
        DDebug() << "set output_KeySystemDispLogicVersionStatus";
    }

    void updateACCIndicator() {
        if ((mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::OFF_700ms) &&
            mAccessoryIndiReqStatus == AccessoryIndiReqStatus::ON) {
            setSFCKey_System_2_0EventACCIndicatorStat(SFCKey_System_2_0EventACCIndicatorStat::ON);
        } else {
            setSFCKey_System_2_0EventACCIndicatorStat(SFCKey_System_2_0EventACCIndicatorStat::OFF);
        }

        setSFCKey_System_2_0EventACCIndicatorID("E60701");
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.DigitalKeyAuthenticationFailed.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedStat(
                SFCKey_System_2_0EventDigitalKeyAuthenticationFailedStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedID(eventID);
            setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedStat(
                SFCKey_System_2_0EventDigitalKeyAuthenticationFailedStat::ON);
            setSFCKey_System_2_0EventDigitalKeyAuthenticationFailedLinkedSoundID(linkedSoundID);
        }
    }

    void updateShiftToPRequest() {
        std::string eventID;
        std::string linkedSoundID;
        SFCKey_System_2_0EventShiftToPRequestLinkedSoundType linkedSoundType =
            SFCKey_System_2_0EventShiftToPRequestLinkedSoundType::NONE;
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
                linkedSoundType = SFCKey_System_2_0EventShiftToPRequestLinkedSoundType::INFINITE;
            } else if (mConfigSBW == ConfigSBW::ON) {
                if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                    eventID = "E60707";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCKey_System_2_0EventShiftToPRequestLinkedSoundType::INFINITE;
                } else {
                    eventID = "E60706";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCKey_System_2_0EventShiftToPRequestLinkedSoundType::INFINITE;
                }
            } else {
                DWarning() << "Invalid ConfigSBW";
            }
        } else {
            // not specified in Section 5.7.5.3.3 Shift to P Request
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.ShiftToPRequest.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventShiftToPRequestStat(SFCKey_System_2_0EventShiftToPRequestStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventShiftToPRequestID(eventID);
            setSFCKey_System_2_0EventShiftToPRequestStat(SFCKey_System_2_0EventShiftToPRequestStat::ON);
            setSFCKey_System_2_0EventShiftToPRequestLinkedSoundID(linkedSoundID);
            setSFCKey_System_2_0EventShiftToPRequestLinkedSoundType(linkedSoundType);
        }
    }

    void updateSMKWarnDisplayReqFailToStart() {
        std::string eventID;
        SFCKey_System_2_0EventFailToStartLinkedSoundType linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::NONE;
        SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType linkedSoundType_1_1 =
            SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::NONE;
        HUInt64 count = 0;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::STEER_AND_START) {
            eventID = "E60711";
            linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::REPEAT_COUNT;
            linkedSoundType_1_1 = SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::UNPLUG_AND_START) {
            eventID = "E60712";
            linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::REPEAT_COUNT;
            linkedSoundType_1_1 = SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::START_BUTTON_AGAIN) {
            eventID = "E60720";
            linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::INFINITE;
            linkedSoundType_1_1 = SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::INFINITE;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::KEY_AND_START_BUTTON) {
            eventID = "E60722";
            linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::REPEAT_COUNT;
            linkedSoundType_1_1 = SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::PLACE_KEY_AND_START_BUTTON) {
            eventID = "E60749";
            linkedSoundType = SFCKey_System_2_0EventFailToStartLinkedSoundType::REPEAT_COUNT;
            linkedSoundType_1_1 = SFCKey_System_2_0EventFailToStart_1_1LinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else {
            DInfo() << "Invalid Data";
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FailToStart.ID, prevEventID);
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FailToStart_1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventFailToStartStat(SFCKey_System_2_0EventFailToStartStat::OFF);
            setSFCKey_System_2_0EventFailToStart_1_1Stat(SFCKey_System_2_0EventFailToStart_1_1Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventFailToStartID(eventID);
            setSFCKey_System_2_0EventFailToStartStat(SFCKey_System_2_0EventFailToStartStat::ON);
            setSFCKey_System_2_0EventFailToStartLinkedSoundType(linkedSoundType);
            setSFCKey_System_2_0EventFailToStartLinkedSoundRepeatCount(count);
            setSFCKey_System_2_0EventFailToStart_1_1ID(eventID);
            setSFCKey_System_2_0EventFailToStart_1_1Stat(SFCKey_System_2_0EventFailToStart_1_1Stat::ON);
            setSFCKey_System_2_0EventFailToStart_1_1LinkedSoundType(linkedSoundType_1_1);
            setSFCKey_System_2_0EventFailToStart_1_1LinkedSoundRepeatCount(count);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FailToStart2.ID, prevEventID);
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FailToStart_1_2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventFailToStart2Stat(SFCKey_System_2_0EventFailToStart2Stat::OFF);
            setSFCKey_System_2_0EventFailToStart_1_2Stat(SFCKey_System_2_0EventFailToStart_1_2Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventFailToStart2ID(eventID);
            setSFCKey_System_2_0EventFailToStart2Stat(SFCKey_System_2_0EventFailToStart2Stat::ON);
            setSFCKey_System_2_0EventFailToStart_1_2ID(eventID);
            setSFCKey_System_2_0EventFailToStart_1_2Stat(SFCKey_System_2_0EventFailToStart_1_2Stat::ON);
        }

        DDebug() << "mSMKWarnDisplayReq2Status: " << static_cast<HUInt64>(mSMKWarnDisplayReq2Status);
    }

    void updateSMKWarnDisplayReq3FailToStart2() {
        std::string eventID;
        if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::STEER_AND_BRAKE) {
            eventID = "E60757";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::RETRY_AFTER_PLACE_SMARTKEY) {
            eventID = "E60758";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::PLACE_KEY_AND_BRAKE) {
            eventID = "E60759";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::RETRY_AFTER_PLACE_DKEY) {
            eventID = "E60760";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::PLACE_DKEY_AND_BRAKE) {
            eventID = "E60761";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::RETRY_AFTER_AUTH_FINGERPRINT) {
            eventID = "E60762";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::AUTH_FINGERPRINT_AND_BRAKE) {
            eventID = "E60763";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::RETRY_AFTER_PLACE_DKEY_OR_AUTH_FINGERPRINT) {
            eventID = "E60764";
        } else if (mSMKWarnDisplayReq3Status == SMKWarnDisplayReq3Status::PLACE_DKEY_OR_AUTH_FINGERPRINT_AND_BRAKE) {
            eventID = "E60765";
        } else {
            DInfo() << "Invalid Data";
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FailToStart_2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventFailToStart_2Stat(SFCKey_System_2_0EventFailToStart_2Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventFailToStart_2ID(eventID);
            setSFCKey_System_2_0EventFailToStart_2Stat(SFCKey_System_2_0EventFailToStart_2Stat::ON);
        }

        DDebug() << "mSMKWarnDisplayReq3Status: " << static_cast<HUInt64>(mSMKWarnDisplayReq3Status);
    }

    void updateSystemWarning() {
        std::string eventID;
        std::string linkedSoundID;
        SFCKey_System_2_0EventSystemWarningLinkedSoundType linkedSoundType =
            SFCKey_System_2_0EventSystemWarningLinkedSoundType::NONE;
        HUInt64 count = 0;
        if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::LOW_SMK_BATTERY) {
            eventID = "E60725";
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CHECK_STEER_LOCK_SYSTEM) {
            eventID = "E60726";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCKey_System_2_0EventSystemWarningLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else if (mSMKWarnDisplayReqStatus == SMKWarnDisplayReqStatus::CHECK_BRAKE_SWITCH) {
            eventID = "E60727";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCKey_System_2_0EventSystemWarningLinkedSoundType::REPEAT_COUNT;
            count = 1;
        } else {
            // not specified in Section 5.7.5.3.5 System Warning
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.SystemWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventSystemWarningStat(SFCKey_System_2_0EventSystemWarningStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventSystemWarningID(eventID);
            setSFCKey_System_2_0EventSystemWarningStat(SFCKey_System_2_0EventSystemWarningStat::ON);
            setSFCKey_System_2_0EventSystemWarningLinkedSoundID(linkedSoundID);
            setSFCKey_System_2_0EventSystemWarningLinkedSoundType(linkedSoundType);
            setSFCKey_System_2_0EventSystemWarningLinkedSoundRepeatCount(count);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.KeyUnrecognized.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventKeyUnrecognizedStat(SFCKey_System_2_0EventKeyUnrecognizedStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventKeyUnrecognizedID(eventID);
            setSFCKey_System_2_0EventKeyUnrecognizedStat(SFCKey_System_2_0EventKeyUnrecognizedStat::ON);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.VehicleIsOnByDigitalKey.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventVehicleIsOnByDigitalKeyStat(SFCKey_System_2_0EventVehicleIsOnByDigitalKeyStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventVehicleIsOnByDigitalKeyID(eventID);
            setSFCKey_System_2_0EventVehicleIsOnByDigitalKeyStat(SFCKey_System_2_0EventVehicleIsOnByDigitalKeyStat::ON);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.SystemFail.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventSystemFailStat(SFCKey_System_2_0EventSystemFailStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventSystemFailID(eventID);
            setSFCKey_System_2_0EventSystemFailStat(SFCKey_System_2_0EventSystemFailStat::ON);
        }
    }

    void updateSoundSMKDeactivationWarn() {
        if (mSMKDeactivationWarnStatus == SMKDeactivationWarnStatus::WARN) {
            setSFCKey_System_2_0SoundSMKDeactivationStat(SFCKey_System_2_0SoundSMKDeactivationStat::ON);
        } else {
            setSFCKey_System_2_0SoundSMKDeactivationStat(SFCKey_System_2_0SoundSMKDeactivationStat::OFF);
        }
    }

    void updateSoundKeyBattLow() {
        if (mKeyBattLowWarnStatus == KeyBattLowWarnStatus::WARN) {
            setSFCKey_System_2_0SoundKeyBattLowStat(SFCKey_System_2_0SoundKeyBattLowStat::ON);
        } else {
            setSFCKey_System_2_0SoundKeyBattLowStat(SFCKey_System_2_0SoundKeyBattLowStat::OFF);
        }
    }

    void updateSoundESCLWarn() {
        if ((mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::OFF_700ms) &&
            mESCLWarnSoundStatus == ESCLWarnSoundStatus::WARN) {
            setSFCKey_System_2_0SoundESCLWarnStat(SFCKey_System_2_0SoundESCLWarnStat::ON);
        } else {
            setSFCKey_System_2_0SoundESCLWarnStat(SFCKey_System_2_0SoundESCLWarnStat::OFF);
        }
    }

    void updateSoundBCMWarn() {
        std::string soundID;
        SFCKey_System_2_0SoundBCMWarnType type = SFCKey_System_2_0SoundBCMWarnType::NONE;
        HUInt64 rptCnt = 0;

        if (mBCMWarnSoundStatus == BCMWarnSoundStatus::KEY_OPER_WARN) {
            soundID = "SND_KeyOperatedWarn";
            type = SFCKey_System_2_0SoundBCMWarnType::INFINITE;
        } else if (mBCMWarnSoundStatus == BCMWarnSoundStatus::KEY_LEARN_WARN) {
            soundID = "SND_KeyLearning";
            type = SFCKey_System_2_0SoundBCMWarnType::REPEAT_COUNT;
            rptCnt = 1;
        } else if (mBCMWarnSoundStatus == BCMWarnSoundStatus::PARK_BRAKE_WARN && mIgnElapsed == IgnElapsed::ON_0ms) {
            soundID = "SND_ParkBrakeWarn";
            type = SFCKey_System_2_0SoundBCMWarnType::INFINITE;
        } else {
            // no operation
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.Key_System_2_0.Sound.BCMWarn.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCKey_System_2_0SoundBCMWarnStat(SFCKey_System_2_0SoundBCMWarnStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCKey_System_2_0SoundBCMWarnID(soundID);
            setSFCKey_System_2_0SoundBCMWarnType(type);
            setSFCKey_System_2_0SoundBCMWarnRepeatCount(rptCnt);
            setSFCKey_System_2_0SoundBCMWarnStat(SFCKey_System_2_0SoundBCMWarnStat::ON);
        }
    }

    void updateFpsTimeLimitValueConstant() {
        SFCKey_System_2_0ConstantFpsTimeLimitValueStat stat = SFCKey_System_2_0ConstantFpsTimeLimitValueStat::VALUE;
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
            stat = SFCKey_System_2_0ConstantFpsTimeLimitValueStat::DISPLAY_OFF;
        }

        setSFCKey_System_2_0ConstantFpsTimeLimitValueStat(stat);
        setSFCKey_System_2_0ConstantFpsTimeLimitValue(value);

        DDebug() << "mFingerPrintWarnReqStatus: " << static_cast<HUInt64>(mFingerPrintWarnReqStatus);
    }

    void updateEvStartTemporaryStandbyStatus() {
        SFCKey_System_2_0ConstantEvStartTemporaryStandbyStat stat = SFCKey_System_2_0ConstantEvStartTemporaryStandbyStat::OFF;

        if (mEvVehicleStartStatus == EvVehicleStartStatus::WAIT) {
            stat = SFCKey_System_2_0ConstantEvStartTemporaryStandbyStat::ON;
        }

        setSFCKey_System_2_0ConstantEvStartTemporaryStandbyStat(stat);

        DDebug() << "mEvVehicleStartStatus" << static_cast<HUInt64>(mEvVehicleStartStatus);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.DigitalKeyRecertificationRequest.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventDigitalKeyRecertificationRequestStat(
                SFCKey_System_2_0EventDigitalKeyRecertificationRequestStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventDigitalKeyRecertificationRequestID(eventID);
            setSFCKey_System_2_0EventDigitalKeyRecertificationRequestStat(
                SFCKey_System_2_0EventDigitalKeyRecertificationRequestStat::ON);
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
        GETCACHEDVALUE(SFC.Key_System_2_0.Event.FingerprintRecognitionWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_2_0EventFingerprintRecognitionWarningStat(
                SFCKey_System_2_0EventFingerprintRecognitionWarningStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_2_0EventFingerprintRecognitionWarningID(eventID);
            setSFCKey_System_2_0EventFingerprintRecognitionWarningStat(
                SFCKey_System_2_0EventFingerprintRecognitionWarningStat::ON);
        }
    }

    void updateServiceModeIndicatorEvent() {
        SFCKey_System_2_0EventServiceModeIndicatorStat stat = SFCKey_System_2_0EventServiceModeIndicatorStat::OFF;

        if (mServiceModeIndicator == ServiceModeIndicator::ON) {
            stat = SFCKey_System_2_0EventServiceModeIndicatorStat::ON;
        }

        setSFCKey_System_2_0EventServiceModeIndicatorID("E60754");
        setSFCKey_System_2_0EventServiceModeIndicatorStat(stat);
    }

    void updateServiceModeConditionEvent() {
        SFCKey_System_2_0EventServiceModeConditionStat stat = SFCKey_System_2_0EventServiceModeConditionStat::OFF;

        if (mSMKWarnDisplayReq2Status == SMKWarnDisplayReq2Status::SERVICEMODE_FAIL) {
            stat = SFCKey_System_2_0EventServiceModeConditionStat::ON;
        }

        setSFCKey_System_2_0EventServiceModeConditionID("E60755");
        setSFCKey_System_2_0EventServiceModeConditionStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        OFF_700ms,
        MAX
    };

    using ConfigSBW = ArgumentsConfigSBWChanged::Inter_ConfigSBW;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ImmoIndiReqCANStatus = ArgumentsImmoIndiReqCANStatusChanged::Input_ImmoIndiReqCANStatus;
    using AccessoryIndiReqStatus = ArgumentsACCIndicatorChanged::Input_AccessoryIndiReqStatus;
    using IAUWarnReqStatus = ArgumentsIAUWarnReqStatusChanged::Input_IauWarnReqStatus;
    using SMKWarnDisplayReqStatus = ArgumentsSMKWarnDisplayReqStatusChanged::Input_SmkWarnDisplayReqStatus;
    using SMKDeactivationWarnStatus = ArgumentsSMKDeactivationChanged::Input_SmkDeactivationWarnStatus;
    using KeyBattLowWarnStatus = ArgumentsKeyBattLowWarnStatusChanged::Input_KeyBattLowWarnStatus;
    using ESCLWarnSoundStatus = ArgumentsESCLWarnSoundStatusChanged::Input_EsclWarnSoundStatus;
    using BCMWarnSoundStatus = ArgumentsBCMWarnSoundStatusChanged::Input_BcmWarnSoundStatus;
    using FingerPrintWarnReqStatus = ArgumentsFingerPrintWarnReqStatusChanged::Input_FingerPrintWarnReqStatus;
    using ServiceModeIndicator = ArgumentsServiceModeIndicatorChanged::Input_ServiceModeIndiReqStatus;
    using SMKWarnDisplayReq2Status = ArgumentsSMKWarnDisplayReq2StatusChanged::Input_SmkWarnDisplayReq2Status;
    using SMKWarnDisplayReq3Status = ArgumentsSMKWarnDisplayReq3StatusChanged::Input_SmkWarnDisplayReq3Status;
    using EvVehicleStartStatus = ArgumentsEvVehicleStartStatusChanged::Input_EvVehicleStartStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigSBW mConfigSBW = ConfigSBW::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    AccessoryIndiReqStatus mAccessoryIndiReqStatus = AccessoryIndiReqStatus::OFF;
    IAUWarnReqStatus mIAUWarnReqStatus = IAUWarnReqStatus::OFF;
    SMKWarnDisplayReqStatus mSMKWarnDisplayReqStatus = SMKWarnDisplayReqStatus::OFF;
    SMKDeactivationWarnStatus mSMKDeactivationWarnStatus = SMKDeactivationWarnStatus::OFF;
    KeyBattLowWarnStatus mKeyBattLowWarnStatus = KeyBattLowWarnStatus::OFF;
    ESCLWarnSoundStatus mESCLWarnSoundStatus = ESCLWarnSoundStatus::OFF;
    BCMWarnSoundStatus mBCMWarnSoundStatus = BCMWarnSoundStatus::OFF;
    FingerPrintWarnReqStatus mFingerPrintWarnReqStatus = FingerPrintWarnReqStatus::OFF;
    ServiceModeIndicator mServiceModeIndicator = ServiceModeIndicator::OFF;
    SMKWarnDisplayReq2Status mSMKWarnDisplayReq2Status = SMKWarnDisplayReq2Status::OFF;
    SMKWarnDisplayReq3Status mSMKWarnDisplayReq3Status = SMKWarnDisplayReq3Status::OFF;
    EvVehicleStartStatus mEvVehicleStartStatus = EvVehicleStartStatus::OFF;
    HBool mIgnOffStatus = false;
    HBool mAEMInitializeComplete = false;

    ssfs::SFCTimer<Key_System_2_0> mOneShotTimerImmoIndiReqCANStatus;
    static constexpr uint32_t kTimerTelltaleInterval200ms = 200u;
};

}  // namespace ccos

#endif  // SFSS_Key_System_2_0_H
