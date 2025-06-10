/**
 * @file ABS_ESC.hpp
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
#ifndef SFSS_ABS_ESC_H
#define SFSS_ABS_ESC_H

#define DLOG_ENABLED gEnableSFCLog

#include "ABS_ESCBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 11.0.1
// Reference : [CH010] ABS_ESC. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class ABS_ESC : public ABS_ESCBase {
public:
    ABS_ESC() = default;
    ~ABS_ESC() override = default;
    ABS_ESC(const ABS_ESC& other) = delete;
    ABS_ESC(ABS_ESC&& other) noexcept = delete;
    ABS_ESC& operator=(const ABS_ESC& other) = delete;
    ABS_ESC& operator=(ABS_ESC&& other) noexcept = delete;

    void onInitialize() override {
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSID("E30003");
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat(SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat::OFF);
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSLinkedSoundID("SND_PopUpWarn1");
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSLinkedSoundType(
            SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSLinkedSoundType::REPEAT_COUNT);
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSLinkedSoundRepeatCount(1);

        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCID("E30004");
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat(SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat::OFF);
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCLinkedSoundID("SND_PopUpWarn1");
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCLinkedSoundType(
            SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCLinkedSoundType::REPEAT_COUNT);
        setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCLinkedSoundRepeatCount(1);

        setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnID("E30005");
        setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat(
            SFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat::OFF);
        setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnLinkedSoundType(
            SFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnLinkedSoundType::REPEAT_COUNT);
        setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnLinkedSoundRepeatCount(1);

        setSFCABS_ESCEventCrosswindSafetyControlID("E30007");
        setSFCABS_ESCEventCrosswindSafetyControlStat(SFCABS_ESCEventCrosswindSafetyControlStat::OFF);

        setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::OFF);
        setSFCABS_ESCTelltaleESCBlinkValueA(SFCABS_ESCTelltaleESCBlinkValueA::ON);
        setSFCABS_ESCTelltaleESCBlinkValueB(SFCABS_ESCTelltaleESCBlinkValueB::OFF);

        setSFCABS_ESCTelltaleESCSportStat(SFCABS_ESCTelltaleESCSportStat::OFF);

        setSFCABS_ESCSoundEscOffInform1ID("SND_EscOffInform1");
        setSFCABS_ESCSoundEscOffInform1Stat(SFCABS_ESCSoundEscOffInform1Stat::OFF);
        setSFCABS_ESCSoundEscOffInform1Type(SFCABS_ESCSoundEscOffInform1Type::REPEAT_COUNT);
        setSFCABS_ESCSoundEscOffInform1RepeatCount(1);

        setSFCABS_ESCSoundEscOffInform2ID("SND_EscOffInform2");
        setSFCABS_ESCSoundEscOffInform2Stat(SFCABS_ESCSoundEscOffInform2Stat::OFF);
        setSFCABS_ESCSoundEscOffInform2Type(SFCABS_ESCSoundEscOffInform2Type::REPEAT_COUNT);
        setSFCABS_ESCSoundEscOffInform2RepeatCount(1);

        if (mOneShotTimerHold2sEscOff.create(kTimerHold2s, this, &ABS_ESC::onOneShotTimerHold2SecTimeout, false) == false) {
            DWarning() << "[ABS_ESC]  mOneShotTimerHold2sEscOff Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /* nodeAddress */) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updatePrivateEscOffHold();
        updatePrivateEscOffStepStatus();
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        updateAll();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_3500ms;
        updateAll();
    }

    void onIgnElapseOn4000msChanged(const ArgumentsIgnElapseOn4000msChanged& /* args */) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_4000ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnitionState = IgnitionState::OFF;
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInterConfigABS = args.mInter_ConfigABS;
        mInterConfigESC = args.mInter_ConfigESC;
        mInterConfigHighPerformance = args.mInter_ConfigHighPerformance;
        updateAll();
    }

    void onAbsTelltaleChanged(const ArgumentsAbsTelltaleChanged& args) {
        if (ISTIMEOUT(args.mInput_AbsWarnStatus) == true) {
            mInputAbsWarnStatus = InputAbsWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputAbsWarnStatus = args.mInput_AbsWarnStatus;
        }
        updateAbsTelltaleChanged();
    }

    void onEscTelltaleChanged(const ArgumentsEscTelltaleChanged& args) {
        if (ISTIMEOUT(args.mInput_EscIndReqStatus) == true) {
            mInputEscIndReqStatus = InputEscIndReqStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputEscIndReqStatus = args.mInput_EscIndReqStatus;
        }

        mEscIndReqStatusFromITC = args.mInput_EscIndReqStatusFromITC;

        updateEscTelltaleChanged();
    }

    void onEscOffTelltaleChanged(const ArgumentsEscOffTelltaleChanged& args) {
        mInputEscOffIndReqStatus = args.mInput_EscOffIndReqStatus;
        updateEscOffTelltaleChanged();
    }

    void onEscOffEventChanged(const ArgumentsEscOffEventChanged& args) {
        mInputEscOffStepStatus = args.mInput_EscOffStepStatus;
        updatePrivateEscOffStepStatus();
        updateSoundLogic();
    }

    void onBrakeMalfunctionWarningEbdWarnChanged(const ArgumentsBrakeMalfunctionWarningEbdWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_EbdWarnStatus) == true) {
            mInputEbdWarnStatus = InputEbdWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputEbdWarnStatus = args.mInput_EbdWarnStatus;
        }
        updateBrakeMalfunctionWarningEbdWarnChanged();
    }

    void onBrakeMalfunctionWarningBrakeFluidWarnChanged(const ArgumentsBrakeMalfunctionWarningBrakeFluidWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_BrakeFluidWarnStatus) == true) {
            mBrakeFluidWarnStatus = BrakeFluidWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mBrakeFluidWarnStatus = args.mInput_BrakeFluidWarnStatus;
        }
        updateBrakeMalfunctionWarningBrakeFluidWarnChanged();
    }

    void onEscSportReqChanged(const ArgumentsEscSportReqChanged& args) {
        mInputEscSportIndReqStatus = args.mInput_EscSportIndReqStatus;
        updateEscSportTelltaleChanged();
    }

    void onCrosswindWarnChanged(const ArgumentsCrosswindWarnChanged& args) {
        mInputCrosswindWarnStatus = args.mInput_CrosswindWarnStatus;
        updateCrosswindWarnChanged();
    }

    void onNModeSelectStatusChanged(const ArgumentsNModeSelectStatusChanged& args) {
        mPrevInterNModeSelectStatus = mInterNModeSelectStatus;
        mInterNModeSelectStatus = args.mInter_NModeSelectStatus;
        updatePrivateEscOffHold();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateEscTelltaleChanged();
        updateEscOffTelltaleChanged();
        updateAbsTelltaleChanged();
        updateEscOffEventChanged();
        updateBrakeMalfunctionWarningEbdWarnChanged();
        updateBrakeMalfunctionWarningBrakeFluidWarnChanged();
        updateEscSportTelltaleChanged();
        updateCrosswindWarnChanged();
        updateSoundLogic();
    }

    void updateAbsTelltaleChanged() {
        if (mInterConfigABS == InterConfigABS::ON || mInterConfigESC == InterConfigESC::ON) {
            if (mIgnitionElapsed >= IgnitionElapsed::ON_3500ms) {
                if (mInputAbsWarnStatus == ArgumentsAbsTelltaleChanged::Input_AbsWarnStatus::OFF) {
                    setSFCABS_ESCTelltaleABSStat(SFCABS_ESCTelltaleABSStat::OFF);
                } else {  // ON or MESSAGE_TIMEOUT
                    setSFCABS_ESCTelltaleABSStat(SFCABS_ESCTelltaleABSStat::ON);
                }
            } else if (mIgnitionElapsed == IgnitionElapsed::ON_500ms) {  // Prewarning
                setSFCABS_ESCTelltaleABSStat(SFCABS_ESCTelltaleABSStat::ON);
            } else {
                setSFCABS_ESCTelltaleABSStat(SFCABS_ESCTelltaleABSStat::OFF);
            }
        } else {
            setSFCABS_ESCTelltaleABSStat(SFCABS_ESCTelltaleABSStat::OFF);
        }
    }

    void updateEscTelltaleChanged() {
        if (mInterConfigESC == InterConfigESC::ON) {
            if (mIgnitionElapsed >= IgnitionElapsed::ON_3500ms) {
                if (mInputEscIndReqStatus == InputEscIndReqStatus::ON ||
                    mInputEscIndReqStatus == InputEscIndReqStatus::UNHANDLED_TIMEOUT) {
                    setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::ON);
                } else if (mInputEscIndReqStatus == InputEscIndReqStatus::BLINKING ||
                           ((mInputEscIndReqStatus != InputEscIndReqStatus::ON ||
                             mInputEscIndReqStatus != InputEscIndReqStatus::UNHANDLED_TIMEOUT) &&
                            mEscIndReqStatusFromITC == EscIndReqStatusFromITC::BLINKING)) {
                    setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::BLINK1);
                } else if (mInputEscIndReqStatus == InputEscIndReqStatus::OFF &&
                           mEscIndReqStatusFromITC != EscIndReqStatusFromITC::BLINKING) {
                    setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::OFF);
                } else {
                    setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::OFF);
                }
            } else if (mIgnitionElapsed == IgnitionElapsed::ON_500ms) {  // Prewarning
                setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::ON);
            } else {
                setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::OFF);
            }
        } else {
            setSFCABS_ESCTelltaleESCStat(SFCABS_ESCTelltaleESCStat::OFF);
        }
    }

    void updateEscOffTelltaleChanged() {
        if (mInterConfigESC == InterConfigESC::ON) {
            if (mIgnitionElapsed >= IgnitionElapsed::ON_3500ms) {
                if (mInputEscOffIndReqStatus == InputEscOffIndReqStatus::ON) {
                    setSFCABS_ESCTelltaleESCOffStat(SFCABS_ESCTelltaleESCOffStat::ON);
                } else {  // mInputEscOffIndReqStatus == InputEscOffIndReqStatus::OFF
                    setSFCABS_ESCTelltaleESCOffStat(SFCABS_ESCTelltaleESCOffStat::OFF);
                }
            } else if (mIgnitionElapsed == IgnitionElapsed::ON_500ms) {  // Prewarning
                setSFCABS_ESCTelltaleESCOffStat(SFCABS_ESCTelltaleESCOffStat::ON);
            } else {
                setSFCABS_ESCTelltaleESCOffStat(SFCABS_ESCTelltaleESCOffStat::OFF);
            }
        } else {
            setSFCABS_ESCTelltaleESCOffStat(SFCABS_ESCTelltaleESCOffStat::OFF);
        }
    }

    void updateEscOffEventChanged() {
        if (mIgnitionState != IgnitionState::ON || mPrivateEscOffStepStatus == InputEscOffStepStatus::NORMAL) {
            updateESCOffEvent("", "");
        } else {
            if (mPreviousPrivateEscOffStepStatus != mPrivateEscOffStepStatus) {
                if ((mPreviousPrivateEscOffStepStatus == InputEscOffStepStatus::NORMAL &&
                     mPrivateEscOffStepStatus == InputEscOffStepStatus::TCS_OFF) ||
                    (mPreviousPrivateEscOffStepStatus == InputEscOffStepStatus::ESC_OFF &&
                     mPrivateEscOffStepStatus == InputEscOffStepStatus::TCS_OFF)) {
                    if (mInterConfigHighPerformance == InterConfigHighPerformance::N) {
                        updateESCOffEvent("E30001", "");
                    } else {
                        updateESCOffEvent("E30001", "SND_PopUpInform1");
                    }
                } else if ((mPreviousPrivateEscOffStepStatus == InputEscOffStepStatus::TCS_OFF &&
                            mPrivateEscOffStepStatus == InputEscOffStepStatus::ESC_OFF) ||
                           (mPreviousPrivateEscOffStepStatus == InputEscOffStepStatus::NORMAL &&
                            mPrivateEscOffStepStatus == InputEscOffStepStatus::ESC_OFF)) {
                    if (mInterConfigHighPerformance == InterConfigHighPerformance::N) {
                        updateESCOffEvent("E30002", "");
                    } else {
                        updateESCOffEvent("E30002", "SND_PopUpInform2");
                    }
                } else {
                    // no operation
                }
            }
        }
    }

    void updateESCOffEvent(const std::string& eventId, const std::string& soundId) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.ABS_ESC.Event.Escoff.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventId) {
            setSFCABS_ESCEventEscoffStat(SFCABS_ESCEventEscoffStat::OFF);
            flushLastGroup();
        }

        if (!eventId.empty()) {
            setSFCABS_ESCEventEscoffID(eventId);
            setSFCABS_ESCEventEscoffStat(SFCABS_ESCEventEscoffStat::ON);
            setSFCABS_ESCEventEscoffLinkedSoundID(soundId);
            setSFCABS_ESCEventEscoffLinkedSoundType(SFCABS_ESCEventEscoffLinkedSoundType::REPEAT_COUNT);
            setSFCABS_ESCEventEscoffLinkedSoundRepeatCount(1);
        }
    }

    void updateBrakeMalfunctionWarningEbdWarnChanged() {
        if (mIgnitionElapsed >= IgnitionElapsed::ON_4000ms && mInputEbdWarnStatus == InputEbdWarnStatus::ON &&
            mInterConfigABS == InterConfigABS::ON) {
            setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat(SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat::ON);
        } else {
            setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat(
                SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ABSStat::OFF);
        }

        if (mIgnitionElapsed >= IgnitionElapsed::ON_4000ms && mInputEbdWarnStatus == InputEbdWarnStatus::ON &&
            mInterConfigESC == InterConfigESC::ON) {
            setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat(SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat::ON);
        } else {
            setSFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat(
                SFCABS_ESCEventBrakeMalfunctionWarning_EBDWarn_ESCStat::OFF);
        }
    }

    void updateBrakeMalfunctionWarningBrakeFluidWarnChanged() {
        if (mIgnitionState == IgnitionState::ON && mBrakeFluidWarnStatus == BrakeFluidWarnStatus::ON) {
            setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat(
                SFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat::ON);
        } else {
            setSFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat(
                SFCABS_ESCEventBrakeMalfunctionWarning_BrakeFluidWarnStat::OFF);
        }
    }

    void updateEscSportTelltaleChanged() {
        if (mIgnitionState == IgnitionState::ON && mInputEscSportIndReqStatus == InputEscSportIndReqStatus::ON) {
            setSFCABS_ESCTelltaleESCSportStat(SFCABS_ESCTelltaleESCSportStat::ON);
        } else {
            setSFCABS_ESCTelltaleESCSportStat(SFCABS_ESCTelltaleESCSportStat::OFF);
        }
    }

    void updateCrosswindWarnChanged() {
        SFCABS_ESCEventCrosswindSafetyControlStat stat = SFCABS_ESCEventCrosswindSafetyControlStat::OFF;
        std::string linkedSoundID;
        SFCABS_ESCEventCrosswindSafetyControlLinkedSoundType linkedSoundType =
            SFCABS_ESCEventCrosswindSafetyControlLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIgnitionState == IgnitionState::ON) {
            if (mInputCrosswindWarnStatus == InputCrosswindWarnStatus::ON) {
                stat = SFCABS_ESCEventCrosswindSafetyControlStat::ON;
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCABS_ESCEventCrosswindSafetyControlLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mInputCrosswindWarnStatus == InputCrosswindWarnStatus::NO_SOUND) {
                stat = SFCABS_ESCEventCrosswindSafetyControlStat::ON;
            } else {
                // OFF
            }
        }
        setSFCABS_ESCEventCrosswindSafetyControlStat(stat);
        setSFCABS_ESCEventCrosswindSafetyControlLinkedSoundID(linkedSoundID);
        setSFCABS_ESCEventCrosswindSafetyControlLinkedSoundType(linkedSoundType);
        setSFCABS_ESCEventCrosswindSafetyControlLinkedSoundRepeatCount(repeatCount);
    }

    void updateSoundLogic() {
        // We need some framework to manage previous value to update it after flush().
        static InputEscOffStepStatus previousInputEscOffStepStatus = InputEscOffStepStatus::INVALID;

        SFCABS_ESCSoundEscOffInform1Stat inform1Stat = SFCABS_ESCSoundEscOffInform1Stat::OFF;
        SFCABS_ESCSoundEscOffInform2Stat inform2Stat = SFCABS_ESCSoundEscOffInform2Stat::OFF;
        if (mInterConfigHighPerformance == InterConfigHighPerformance::N && mIgnitionState == IgnitionState::ON) {
            if (previousInputEscOffStepStatus == InputEscOffStepStatus::NORMAL &&
                mInputEscOffStepStatus == InputEscOffStepStatus::TCS_OFF) {
                inform1Stat = SFCABS_ESCSoundEscOffInform1Stat::ON;
            }
            if ((previousInputEscOffStepStatus == InputEscOffStepStatus::TCS_OFF &&
                 mInputEscOffStepStatus == InputEscOffStepStatus::ESC_OFF) ||
                (previousInputEscOffStepStatus == InputEscOffStepStatus::NORMAL &&
                 mInputEscOffStepStatus == InputEscOffStepStatus::ESC_OFF)) {
                inform2Stat = SFCABS_ESCSoundEscOffInform2Stat::ON;
            }
        }
        setSFCABS_ESCSoundEscOffInform1Stat(inform1Stat);
        setSFCABS_ESCSoundEscOffInform2Stat(inform2Stat);
        previousInputEscOffStepStatus = mInputEscOffStepStatus;
    }

    void updatePrivateEscOffHold() {
        if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if ((mPrevInterNModeSelectStatus != InputInterNModeSelectStatus::N &&
                 mInterNModeSelectStatus == InputInterNModeSelectStatus::N) ||
                (mPrevInterNModeSelectStatus != InputInterNModeSelectStatus::CUSTOM1 &&
                 mInterNModeSelectStatus == InputInterNModeSelectStatus::CUSTOM1) ||
                (mPrevInterNModeSelectStatus != InputInterNModeSelectStatus::CUSTOM2 &&
                 mInterNModeSelectStatus == InputInterNModeSelectStatus::CUSTOM2)) {
                mPrivateEscOffHoldStat = EscOffHoldStat::HOLD;
                mOneShotTimerHold2sEscOff.restart();
                return;
            }
        }

        mPrivateEscOffHoldStat = EscOffHoldStat::OFF;
        mOneShotTimerHold2sEscOff.stop();
        updatePrivateEscOffStepStatus();
    }

    void updatePrivateEscOffStepStatus() {
        if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if (mPrivateEscOffHoldStat == EscOffHoldStat::OFF ||
                mInputEscOffStepStatus == InputEscOffStepStatus::NORMAL) {  // 팝업 즉시 해제 조건
                if (mPrivateEscOffStepStatus != mInputEscOffStepStatus) {
                    mPreviousPrivateEscOffStepStatus = mPrivateEscOffStepStatus;
                    mPrivateEscOffStepStatus = mInputEscOffStepStatus;
                    updateEscOffEventChanged();
                }
            }
        }
    }

    void onOneShotTimerHold2SecTimeout() {
        _DDebug() << "ESCOffHoldTimer: hold stat off. " << (uint64_t)mPreviousPrivateEscOffStepStatus << ":"
                  << (uint64_t)mPrivateEscOffHoldStat;
        mPrivateEscOffHoldStat = EscOffHoldStat::OFF;
        updatePrivateEscOffStepStatus();
        flush();
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        ON_4000ms,
        MAX
    };

    enum class IgnitionState : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    enum class EscOffHoldStat : HUInt64 {
        NONE,
        OFF,
        HOLD
    };

    using InterConfigABS = ArgumentsConfigChanged::Inter_ConfigABS;
    using InterConfigESC = ArgumentsConfigChanged::Inter_ConfigESC;
    using InterConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using InputAbsWarnStatus = ArgumentsAbsTelltaleChanged::Input_AbsWarnStatus;
    using InputEscIndReqStatus = ArgumentsEscTelltaleChanged::Input_EscIndReqStatus;
    using EscIndReqStatusFromITC = ArgumentsEscTelltaleChanged::Input_EscIndReqStatusFromITC;
    using InputEscOffIndReqStatus = ArgumentsEscOffTelltaleChanged::Input_EscOffIndReqStatus;
    using InputEscOffStepStatus = ArgumentsEscOffEventChanged::Input_EscOffStepStatus;
    using InputEbdWarnStatus = ArgumentsBrakeMalfunctionWarningEbdWarnChanged::Input_EbdWarnStatus;
    using BrakeFluidWarnStatus = ArgumentsBrakeMalfunctionWarningBrakeFluidWarnChanged::Input_BrakeFluidWarnStatus;
    using InputEscSportIndReqStatus = ArgumentsEscSportReqChanged::Input_EscSportIndReqStatus;
    using InputCrosswindWarnStatus = ArgumentsCrosswindWarnChanged::Input_CrosswindWarnStatus;
    using InputInterNModeSelectStatus = ArgumentsNModeSelectStatusChanged::Inter_NModeSelectStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    IgnitionState mIgnitionState = IgnitionState::NONE;
    InterConfigABS mInterConfigABS = InterConfigABS::OFF;
    InterConfigESC mInterConfigESC = InterConfigESC::OFF;
    InterConfigHighPerformance mInterConfigHighPerformance = InterConfigHighPerformance::OFF;
    InputEscOffStepStatus mInputEscOffStepStatus = InputEscOffStepStatus::INVALID;
    InputEscOffStepStatus mPrivateEscOffStepStatus = InputEscOffStepStatus::INVALID;
    InputEscOffStepStatus mPreviousPrivateEscOffStepStatus = InputEscOffStepStatus::INVALID;
    InputAbsWarnStatus mInputAbsWarnStatus = InputAbsWarnStatus::OFF;
    InputEscIndReqStatus mInputEscIndReqStatus = InputEscIndReqStatus::OFF;
    EscIndReqStatusFromITC mEscIndReqStatusFromITC = EscIndReqStatusFromITC::OFF;
    InputEscOffIndReqStatus mInputEscOffIndReqStatus = InputEscOffIndReqStatus::OFF;
    InputEbdWarnStatus mInputEbdWarnStatus = InputEbdWarnStatus::OFF;
    BrakeFluidWarnStatus mBrakeFluidWarnStatus = BrakeFluidWarnStatus::OFF;
    InputEscSportIndReqStatus mInputEscSportIndReqStatus = InputEscSportIndReqStatus::OFF;
    InputCrosswindWarnStatus mInputCrosswindWarnStatus = InputCrosswindWarnStatus::OFF;
    EscOffHoldStat mPrivateEscOffHoldStat = EscOffHoldStat::OFF;
    InputInterNModeSelectStatus mInterNModeSelectStatus = InputInterNModeSelectStatus::NONE;
    InputInterNModeSelectStatus mPrevInterNModeSelectStatus = InputInterNModeSelectStatus::NONE;

    static constexpr uint32_t kTimerHold2s = 2000;
    ssfs::SFCTimer<ABS_ESC> mOneShotTimerHold2sEscOff;
};

}  // namespace ccos

#endif  // SFSS_ABS_ESC_H
