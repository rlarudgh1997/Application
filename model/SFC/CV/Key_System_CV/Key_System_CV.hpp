/**
 * @file Key_System_CV.hpp
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
#ifndef SFSS_Key_System_CV_H
#define SFSS_Key_System_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Key_System_CVBase.hpp"

namespace ccos {

// SFC Version : 6.0.0
// Spec Version : v0.21
// Reference : [CV660] Key_System_CV. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class Key_System_CV : public Key_System_CVBase {
public:
    Key_System_CV() = default;
    ~Key_System_CV() override = default;
    Key_System_CV(const Key_System_CV& other) = delete;
    Key_System_CV(Key_System_CV&& other) noexcept = delete;
    Key_System_CV& operator=(const Key_System_CV& other) = delete;
    Key_System_CV& operator=(Key_System_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCKey_System_CVTelltaleImmobilizerBlinkValueA(SFCKey_System_CVTelltaleImmobilizerBlinkValueA::ON);
        setSFCKey_System_CVTelltaleImmobilizerBlinkValueB(SFCKey_System_CVTelltaleImmobilizerBlinkValueB::OFF);

        setSFCKey_System_CVSoundIDOutID("SND_IDOut");
        setSFCKey_System_CVSoundIDOutType(SFCKey_System_CVSoundIDOutType::INFINITE);

        setSFCKey_System_CVSoundDeactIDWarnSMKID("SND_DeactIDWarnSMK");
        setSFCKey_System_CVSoundDeactIDWarnSMKType(SFCKey_System_CVSoundDeactIDWarnSMKType::INFINITE);

        setSFCKey_System_CVSoundLowKeyBatteryID("SND_LowKeyBattery");
        setSFCKey_System_CVSoundLowKeyBatteryType(SFCKey_System_CVSoundLowKeyBatteryType::INFINITE);

        setSFCKey_System_CVSoundESCLNotLockWarnID("SND_ESCLNotLockWarn");
        setSFCKey_System_CVSoundESCLNotLockWarnType(SFCKey_System_CVSoundESCLNotLockWarnType::INFINITE);

        setSFCKey_System_CVSoundEsclFailID("SND_ESCLFailWarn");
        setSFCKey_System_CVSoundEsclFailType(SFCKey_System_CVSoundEsclFailType::INFINITE);

        setSFCKey_System_CVSoundEsclNotUnlockedID("SND_ESCLNotUnlockWarn");
        setSFCKey_System_CVSoundEsclNotUnlockedType(SFCKey_System_CVSoundEsclNotUnlockedType::INFINITE);

        setSFCKey_System_CVSoundSsbFailID("SND_SSBFailWarn");
        setSFCKey_System_CVSoundSsbFailType(SFCKey_System_CVSoundSsbFailType::INFINITE);

        setSFCKey_System_CVSoundEngRestartWarnID("SND_EngRestartWarnSMK");
        setSFCKey_System_CVSoundEngRestartWarnType(SFCKey_System_CVSoundEngRestartWarnType::INFINITE);

        setSFCKey_System_CVEventAccIndID("E76618");

        setSFCKey_System_CVEventCVServiceModeIndiID("E76619");

        setSFCKey_System_CVEventCVServiceModeCondiID("E76620");
        setSFCKey_System_CVEventCVServiceModeCondiLinkedSoundID("SND_PopUpInform2");
        setSFCKey_System_CVEventCVServiceModeCondiLinkedSoundType(
            SFCKey_System_CVEventCVServiceModeCondiLinkedSoundType::REPEAT_COUNT);
        setSFCKey_System_CVEventCVServiceModeCondiLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltale();
        updateEvent();
        updateEventAccInd();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltale();
        updateEventAccInd();
    }

    void onConfigIMMOChanged(const ArgumentsConfigIMMOChanged& args) {
        mConfigIMMO = args.mInter_ConfigIMMO;
        updateTelltale();
    }

    void onConfigSMKCVChanged(const ArgumentsConfigSMKCVChanged& args) {
        mConfigSMKCV = args.mInter_ConfigSMKCV;
        updateTelltale();
        updateEvent();
        updateEsclFailSound();
        updateEsclNotLockWarnSound();
        updateEsclNotUnlockedSound();
        updateSsbFailSound();
        updateIDOutSound();
        updateLowKeyBatterySound();
        updateDeactIdWarnSMKSound();
        updateEngRestartWarnSound();
        updateEventAccInd();
        updateEventServiceModeIndi();
        updateEventServiceModeCondi();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEventServiceModeIndi();
        updateEventServiceModeCondi();
    }

    void onImmoIndStatusChanged(const ArgumentsImmoIndStatusChanged& args) {
        mImmoIndStatus = args.mInput_ImmoIndStatus;
        updateTelltale();
    }

    void onSmkPicWarnStatusChanged(const ArgumentsSmkPicWarnStatusChanged& args) {
        mSMKPicWarnStatus = args.mInput_SMKPicWarnStatus;
        updateEvent();
        updateEventServiceModeCondi();
    }

    void onEsclFailSoundStatusChanged(const ArgumentsEsclFailSoundStatusChanged& args) {
        mEsclFailSoundStatus = args.mInput_EsclFailSoundStatus;
        updateEsclFailSound();
    }

    void onEsclNotLockedSoundStatusChanged(const ArgumentsEsclNotLockedSoundStatusChanged& args) {
        mEsclNotLockedSoundStatus = args.mInput_EsclNotLockedSoundStatus;
        updateEsclNotLockWarnSound();
    }

    void onEsclNotUnlockedSoundStatusChanged(const ArgumentsEsclNotUnlockedSoundStatusChanged& args) {
        mEsclNotUnlockedSoundStatus = args.mInput_EsclNotUnlockedSoundStatus;
        updateEsclNotUnlockedSound();
    }

    void onSsbFailSoundStatusChanged(const ArgumentsSsbFailSoundStatusChanged& args) {
        mSsbFailSoundStatus = args.mInput_SsbFailSoundStatus;
        updateSsbFailSound();
    }

    void onIdOutWarnSoundStatusChanged(const ArgumentsIdOutWarnSoundStatusChanged& args) {
        mIdOutWarnSoundStatus = args.mInput_IdOutWarnSoundStatus;
        updateIDOutSound();
    }

    void onKeyBatDischargeSoundStatusChanged(const ArgumentsKeyBatDischargeSoundStatusChanged& args) {
        mKeyBatDischargeSoundStatus = args.mInput_KeyBatDischargeSoundStatus;
        updateLowKeyBatterySound();
    }

    void onDeactivationWarnSoundStatusChanged(const ArgumentsDeactivationWarnSoundStatusChanged& args) {
        mDeactivationWarnSoundStatus = args.mInput_DeactivationWarnSoundStatus;
        updateDeactIdWarnSMKSound();
    }

    void onEngRestartWarnSoundStatusChanged(const ArgumentsEngRestartWarnSoundStatusChanged& args) {
        mEngRestartWarnSoundStatus = args.mInput_EngRestartWarnSoundStatus;
        updateEngRestartWarnSound();
    }

    void onAccIndStatusChanged(const ArgumentsAccIndStatusChanged& args) {
        mAccIndStatus = args.mInput_AccIndStatus;
        updateEventAccInd();
    }

    void onCvServiceModeIndiReqStatusChanged(const ArgumentsCvServiceModeIndiReqStatusChanged& args) {
        mCVServiceModeIndiReqStatus = args.mInput_CVServiceModeIndiReqStatus;
        updateEventServiceModeIndi();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventAccInd() {
        SFCKey_System_CVEventAccIndStat stat = SFCKey_System_CVEventAccIndStat::OFF;

        if (mIsIgnOn == false) {
            if (isAvailableKeySystemCV() == true && mAccIndStatus == AccIndStatus::ON) {
                stat = SFCKey_System_CVEventAccIndStat::ON;
            }
        }
        setSFCKey_System_CVEventAccIndStat(stat);
    }

    void updateEventServiceModeIndi() {
        SFCKey_System_CVEventCVServiceModeIndiStat stat = SFCKey_System_CVEventCVServiceModeIndiStat::OFF;

        if (isAvailableKeySystemCV() == true &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mCVServiceModeIndiReqStatus == CVServiceModeIndiReqStatus::ON) {
            stat = SFCKey_System_CVEventCVServiceModeIndiStat::ON;
        }

        setSFCKey_System_CVEventCVServiceModeIndiStat(stat);
    }

    void updateEventServiceModeCondi() {
        SFCKey_System_CVEventCVServiceModeCondiStat stat = SFCKey_System_CVEventCVServiceModeCondiStat::OFF;

        if (isAvailableKeySystemCV() == true &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mSMKPicWarnStatus == SMKPicWarnStatus::SERVICEMODE_FAIL) {
            stat = SFCKey_System_CVEventCVServiceModeCondiStat::ON;
        }

        setSFCKey_System_CVEventCVServiceModeCondiStat(stat);
    }

    void updateTelltale() {
        SFCKey_System_CVTelltaleImmobilizerStat stat = SFCKey_System_CVTelltaleImmobilizerStat::OFF;

        if (isAvailableKeySystemCV() == true || mConfigIMMO == ConfigIMMO::ON) {
            if (mIgnElapsed == IgnElapsed::ON_500ms) {
                stat = SFCKey_System_CVTelltaleImmobilizerStat::ON;
            } else if (mImmoIndStatus == ImmoIndStatus::ON) {
                stat = SFCKey_System_CVTelltaleImmobilizerStat::ON;
            } else if (mImmoIndStatus == ImmoIndStatus::BLINK) {
                stat = SFCKey_System_CVTelltaleImmobilizerStat::BLINK2;
            } else {
                // no operation
            }
        }
        setSFCKey_System_CVTelltaleImmobilizerStat(stat);
    }

    void updateEvent() {
        std::string eventID;
        if (isAvailableKeySystemCV() == true) {
            switch (mSMKPicWarnStatus) {
                case SMKPicWarnStatus::KEY_BAT_DISCHARGE:
                    eventID = "E76601";
                    break;
                case SMKPicWarnStatus::ESCL_NOT_UNLOCKED:
                    eventID = "E76602";
                    break;
                case SMKPicWarnStatus::ESCL_NOT_LOCKED:
                    eventID = "E76603";
                    break;
                case SMKPicWarnStatus::ESCL_FAIL:
                    eventID = "E76604";
                    break;
                case SMKPicWarnStatus::ENGINE_START_OPERATION1:
                    eventID = "E76605";
                    break;
                case SMKPicWarnStatus::AUTH_FAIL1:
                    eventID = "E76606";
                    break;
                case SMKPicWarnStatus::KEY_OUT_LAMP:
                    eventID = "E76607";
                    break;
                case SMKPicWarnStatus::SSB_FAIL:
                    eventID = "E76608";
                    break;
                case SMKPicWarnStatus::AUTH_FAIL2:
                    eventID = "E76609";
                    break;
                case SMKPicWarnStatus::VEHICLE_NOT_N:
                    eventID = "E76610";
                    break;
                case SMKPicWarnStatus::ENGINE_START_OPERATION2:
                    eventID = "E76611";
                    break;
                case SMKPicWarnStatus::ENGINE_FAIL:
                    eventID = "E76612";
                    break;
                case SMKPicWarnStatus::ENGINE_RESTARTING:
                    eventID = "E76613";
                    break;
                case SMKPicWarnStatus::PREHEAT:
                    eventID = "E76614";
                    break;
                case SMKPicWarnStatus::MODE_CHANGE:
                    eventID = "E76615";
                    break;
                case SMKPicWarnStatus::EMERGENCY_STOP:
                    eventID = "E76616";
                    break;
                default:
                    break;
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Key_System_CV.Event.SMKPicWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCKey_System_CVEventSMKPicWarnStat(SFCKey_System_CVEventSMKPicWarnStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCKey_System_CVEventSMKPicWarnID(eventID);
            setSFCKey_System_CVEventSMKPicWarnStat(SFCKey_System_CVEventSMKPicWarnStat::ON);
        }
    }

    void updateIDOutSound() {
        SFCKey_System_CVSoundIDOutStat stat = SFCKey_System_CVSoundIDOutStat::OFF;

        if (mIdOutWarnSoundStatus == IdOutWarnSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundIDOutStat::ON;
        }
        setSFCKey_System_CVSoundIDOutStat(stat);
    }

    void updateEsclFailSound() {
        SFCKey_System_CVSoundEsclFailStat stat = SFCKey_System_CVSoundEsclFailStat::OFF;

        if (mEsclFailSoundStatus == EsclFailSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundEsclFailStat::ON;
        }
        setSFCKey_System_CVSoundEsclFailStat(stat);
    }

    void updateDeactIdWarnSMKSound() {
        SFCKey_System_CVSoundDeactIDWarnSMKStat stat = SFCKey_System_CVSoundDeactIDWarnSMKStat::OFF;
        if (mDeactivationWarnSoundStatus == DeactivationWarnSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundDeactIDWarnSMKStat::ON;
        }
        setSFCKey_System_CVSoundDeactIDWarnSMKStat(stat);
    }

    void updateEsclNotUnlockedSound() {
        SFCKey_System_CVSoundEsclNotUnlockedStat stat = SFCKey_System_CVSoundEsclNotUnlockedStat::OFF;
        if (mEsclNotUnlockedSoundStatus == EsclNotUnlockedSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundEsclNotUnlockedStat::ON;
        }
        setSFCKey_System_CVSoundEsclNotUnlockedStat(stat);
    }

    void updateLowKeyBatterySound() {
        SFCKey_System_CVSoundLowKeyBatteryStat stat = SFCKey_System_CVSoundLowKeyBatteryStat::OFF;
        if (mKeyBatDischargeSoundStatus == KeyBatDischargeSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundLowKeyBatteryStat::ON;
        }
        setSFCKey_System_CVSoundLowKeyBatteryStat(stat);
    }

    void updateEsclNotLockWarnSound() {
        SFCKey_System_CVSoundESCLNotLockWarnStat stat = SFCKey_System_CVSoundESCLNotLockWarnStat::OFF;
        if (mEsclNotLockedSoundStatus == EsclNotLockedSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundESCLNotLockWarnStat::ON;
        }
        setSFCKey_System_CVSoundESCLNotLockWarnStat(stat);
    }

    void updateSsbFailSound() {
        SFCKey_System_CVSoundSsbFailStat stat = SFCKey_System_CVSoundSsbFailStat::OFF;
        if (mSsbFailSoundStatus == SsbFailSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundSsbFailStat::ON;
        }
        setSFCKey_System_CVSoundSsbFailStat(stat);
    }

    void updateEngRestartWarnSound() {
        SFCKey_System_CVSoundEngRestartWarnStat stat = SFCKey_System_CVSoundEngRestartWarnStat::OFF;
        if (mEngRestartWarnSoundStatus == EngRestartWarnSoundStatus::ON && isAvailableKeySystemCV() == true) {
            stat = SFCKey_System_CVSoundEngRestartWarnStat::ON;
        }
        setSFCKey_System_CVSoundEngRestartWarnStat(stat);
    }

    inline bool isAvailableKeySystemCV() const {
        return (mConfigSMKCV == ConfigSMKCV::ON);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using ImmoIndStatus = ArgumentsImmoIndStatusChanged::Input_ImmoIndStatus;
    using SMKPicWarnStatus = ArgumentsSmkPicWarnStatusChanged::Input_SMKPicWarnStatus;
    using AccIndStatus = ArgumentsAccIndStatusChanged::Input_AccIndStatus;
    using EsclFailSoundStatus = ArgumentsEsclFailSoundStatusChanged::Input_EsclFailSoundStatus;
    using EsclNotLockedSoundStatus = ArgumentsEsclNotLockedSoundStatusChanged::Input_EsclNotLockedSoundStatus;
    using EsclNotUnlockedSoundStatus = ArgumentsEsclNotUnlockedSoundStatusChanged::Input_EsclNotUnlockedSoundStatus;
    using SsbFailSoundStatus = ArgumentsSsbFailSoundStatusChanged::Input_SsbFailSoundStatus;
    using IdOutWarnSoundStatus = ArgumentsIdOutWarnSoundStatusChanged::Input_IdOutWarnSoundStatus;
    using KeyBatDischargeSoundStatus = ArgumentsKeyBatDischargeSoundStatusChanged::Input_KeyBatDischargeSoundStatus;
    using DeactivationWarnSoundStatus = ArgumentsDeactivationWarnSoundStatusChanged::Input_DeactivationWarnSoundStatus;
    using EngRestartWarnSoundStatus = ArgumentsEngRestartWarnSoundStatusChanged::Input_EngRestartWarnSoundStatus;
    using ConfigIMMO = ArgumentsConfigIMMOChanged::Inter_ConfigIMMO;
    using ConfigSMKCV = ArgumentsConfigSMKCVChanged::Inter_ConfigSMKCV;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using CVServiceModeIndiReqStatus = ArgumentsCvServiceModeIndiReqStatusChanged::Input_CVServiceModeIndiReqStatus;

    HBool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ImmoIndStatus mImmoIndStatus = ImmoIndStatus::OFF;
    SMKPicWarnStatus mSMKPicWarnStatus = SMKPicWarnStatus::OFF;
    ConfigSMKCV mConfigSMKCV = ConfigSMKCV::OFF;
    AccIndStatus mAccIndStatus = AccIndStatus::OFF;
    EsclFailSoundStatus mEsclFailSoundStatus = EsclFailSoundStatus::OFF;
    EsclNotLockedSoundStatus mEsclNotLockedSoundStatus = EsclNotLockedSoundStatus::OFF;
    EsclNotUnlockedSoundStatus mEsclNotUnlockedSoundStatus = EsclNotUnlockedSoundStatus::OFF;
    SsbFailSoundStatus mSsbFailSoundStatus = SsbFailSoundStatus::OFF;
    IdOutWarnSoundStatus mIdOutWarnSoundStatus = IdOutWarnSoundStatus::OFF;
    KeyBatDischargeSoundStatus mKeyBatDischargeSoundStatus = KeyBatDischargeSoundStatus::OFF;
    DeactivationWarnSoundStatus mDeactivationWarnSoundStatus = DeactivationWarnSoundStatus::OFF;
    EngRestartWarnSoundStatus mEngRestartWarnSoundStatus = EngRestartWarnSoundStatus::OFF;
    ConfigIMMO mConfigIMMO = ConfigIMMO::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    CVServiceModeIndiReqStatus mCVServiceModeIndiReqStatus = CVServiceModeIndiReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Key_System_CV_H
