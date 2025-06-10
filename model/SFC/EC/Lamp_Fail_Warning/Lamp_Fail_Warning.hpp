/**
 * @file Lamp_Fail_Warning.hpp
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
#ifndef SFSS_Lamp_Fail_Warning_H
#define SFSS_Lamp_Fail_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include <bitset>
#include "Lamp_Fail_WarningBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [EC090] Lamp_Fail_Warning. Contains : Telltale, Event, Constant. Recommend: VALUE_CHANGED
class Lamp_Fail_Warning : public Lamp_Fail_WarningBase {
public:
    Lamp_Fail_Warning() = default;
    ~Lamp_Fail_Warning() override = default;
    Lamp_Fail_Warning(const Lamp_Fail_Warning& other) = delete;
    Lamp_Fail_Warning(Lamp_Fail_Warning&& other) noexcept = delete;
    Lamp_Fail_Warning& operator=(const Lamp_Fail_Warning& other) = delete;
    Lamp_Fail_Warning& operator=(Lamp_Fail_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLamp_Fail_WarningEventLampFailBiID("E60801");
        setSFCLamp_Fail_WarningEventLampFailBiLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailBiLinkedSoundType(SFCLamp_Fail_WarningEventLampFailBiLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailBiLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailDrlID("E60802");
        setSFCLamp_Fail_WarningEventLampFailDrlLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailDrlLinkedSoundType(SFCLamp_Fail_WarningEventLampFailDrlLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailDrlLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailFogID("E60803");
        setSFCLamp_Fail_WarningEventLampFailFogLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailFogLinkedSoundType(SFCLamp_Fail_WarningEventLampFailFogLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailFogLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailHighID("E60805");
        setSFCLamp_Fail_WarningEventLampFailHighLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailHighLinkedSoundType(
            SFCLamp_Fail_WarningEventLampFailHighLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailHighLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailMountID("E60806");
        setSFCLamp_Fail_WarningEventLampFailMountLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailMountLinkedSoundType(
            SFCLamp_Fail_WarningEventLampFailMountLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailMountLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailLowID("E60807");
        setSFCLamp_Fail_WarningEventLampFailLowLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailLowLinkedSoundType(SFCLamp_Fail_WarningEventLampFailLowLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailLowLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailCornerID("E60809");
        setSFCLamp_Fail_WarningEventLampFailCornerLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailCornerLinkedSoundType(
            SFCLamp_Fail_WarningEventLampFailCornerLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailCornerLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailStopID("E60810");
        setSFCLamp_Fail_WarningEventLampFailStopLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailStopLinkedSoundType(
            SFCLamp_Fail_WarningEventLampFailStopLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailStopLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLampFailTurnID("E60811");
        setSFCLamp_Fail_WarningEventLampFailTurnLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLampFailTurnLinkedSoundType(
            SFCLamp_Fail_WarningEventLampFailTurnLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLampFailTurnLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventTrailerStopID("E60813");
        setSFCLamp_Fail_WarningEventTrailerStopLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventTrailerStopLinkedSoundType(SFCLamp_Fail_WarningEventTrailerStopLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventTrailerStopLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventTrailerTurnLeftID("E60814");
        setSFCLamp_Fail_WarningEventTrailerTurnLeftLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventTrailerTurnLeftLinkedSoundType(
            SFCLamp_Fail_WarningEventTrailerTurnLeftLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventTrailerTurnLeftLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventTrailerTurnRightID("E60815");
        setSFCLamp_Fail_WarningEventTrailerTurnRightLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventTrailerTurnRightLinkedSoundType(
            SFCLamp_Fail_WarningEventTrailerTurnRightLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventTrailerTurnRightLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventTrailerTailLeftID("E60816");
        setSFCLamp_Fail_WarningEventTrailerTailLeftLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventTrailerTailLeftLinkedSoundType(
            SFCLamp_Fail_WarningEventTrailerTailLeftLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventTrailerTailLeftLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventTrailerTailRightID("E60817");
        setSFCLamp_Fail_WarningEventTrailerTailRightLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventTrailerTailRightLinkedSoundType(
            SFCLamp_Fail_WarningEventTrailerTailRightLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventTrailerTailRightLinkedSoundRepeatCount(1);

        setSFCLamp_Fail_WarningEventLedLampFailID("E60818");
        setSFCLamp_Fail_WarningEventLedLampFailLinkedSoundID("SND_PopUpWarn1");
        setSFCLamp_Fail_WarningEventLedLampFailLinkedSoundType(SFCLamp_Fail_WarningEventLedLampFailLinkedSoundType::REPEAT_COUNT);
        setSFCLamp_Fail_WarningEventLedLampFailLinkedSoundRepeatCount(1);
        mBiFuncStat.reset();
        mDRLStat.reset();
        mFrontFogStat.reset();
        mHeadHighStat.reset();
        mHeadLowStat.reset();
        mStaticBendingStat.reset();
        mStopLampStat.reset();
        mTurnSignalStat.reset();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateLedLampFail();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onVariantChanged(const ArgumentsVariantChanged& args) {
        mInter_ConfigILCU = args.mInter_ConfigILCU;
        updateLedLampFail();
    }

    void onHeadBiFuncStatusChanged(const ArgumentsHeadBiFuncStatusChanged& args) {
        mLampFailHeadBiFuncLeftStatus = args.mInput_LampFailHeadBiFuncLeftStatus;
        mLampFailHeadBiFuncRightStatus = args.mInput_LampFailHeadBiFuncRightStatus;
        updateLampFailBi();
        setForced();
    }

    void onDrlStatusChanged(const ArgumentsDrlStatusChanged& args) {
        mLampFailDrlLeftStatus = args.mInput_LampFailDrlLeftStatus;
        mLampFailDrlRightStatus = args.mInput_LampFailDrlRightStatus;
        updateLampFailDrl();
        setForced();
    }

    void onFogStatusChanged(const ArgumentsFogStatusChanged& args) {
        mLampFailFrontFogLeftStatus = args.mInput_LampFailFrontFogLeftStatus;
        mLampFailFrontFogRightStatus = args.mInput_LampFailFrontFogRightStatus;
        updateLampFailFog();
        setForced();
    }

    void onHeadHighStatusChanged(const ArgumentsHeadHighStatusChanged& args) {
        mLampFailHeadHighLeftStatus = args.mInput_LampFailHeadHighLeftStatus;
        mLampFailHeadHighRightStatus = args.mInput_LampFailHeadHighRightStatus;
        updateLampFailHigh();
        setForced();
    }

    void onHeadMountStopStatusChanged(const ArgumentsHeadMountStopStatusChanged& args) {
        mLampFailHeadMountStopLampStatus = args.mInput_LampFailHeadMountStopLampStatus;
        updateMountStop();
    }

    void onHeadLowStatusChanged(const ArgumentsHeadLowStatusChanged& args) {
        mLampFailHeadLowLeftStatus = args.mInput_LampFailHeadLowLeftStatus;
        mLampFailHeadLowRightStatus = args.mInput_LampFailHeadLowRightStatus;
        updateLampFailLow();
        setForced();
    }

    void onStaticBendingStatusChanged(const ArgumentsStaticBendingStatusChanged& args) {
        mLampFailStaticBendingLeftStatus = args.mInput_LampFailStaticBendingLeftStatus;
        mLampFailStaticBendingRightStatus = args.mInput_LampFailStaticBendingRightStatus;
        updateLampFailCorner();
        setForced();
    }

    void onStopLampStatusChanged(const ArgumentsStopLampStatusChanged& args) {
        mLampFailStopLampLeftStatus = args.mInput_LampFailStopLampLeftStatus;
        mLampFailStopLampRightStatus = args.mInput_LampFailStopLampRightStatus;
        updateLampFailStop();
        setForced();
    }

    void onTurnSignalStatusChanged(const ArgumentsTurnSignalStatusChanged& args) {
        mLampFailFrontTurnLeftStatus = args.mInput_LampFailFrontTurnSignalLeftStatus;
        mLampFailFrontTurnRightStatus = args.mInput_LampFailFrontTurnSignalRightStatus;
        mLampFailRearTurnLeftStatus = args.mInput_LampFailRearTurnSignalLeftStatus;
        mLampFailRearTurnRightStatus = args.mInput_LampFailRearTurnSignalRightStatus;
        updateLampFailTurn();
        setForced();
    }

    void onTrailerStopStatusChanged(const ArgumentsTrailerStopStatusChanged& args) {
        mLampFailTrailerStopLampStatus = args.mInput_LampFailTrailerStopLampStatus;
        updateTrailerStop();
    }

    void onTrailerTurnLeftStatusChanged(const ArgumentsTrailerTurnLeftStatusChanged& args) {
        mLampFailTrailerTurnLeftStatus = args.mInput_LampFailTrailerTurnSignalLeftStatus;
        updateTrailerTurnLh();
    }

    void onTrailerTurnRightStatusChanged(const ArgumentsTrailerTurnRightStatusChanged& args) {
        mLampFailTrailerTurnRightStatus = args.mInput_LampFailTrailerTurnSignalRightStatus;
        updateTrailerTurnRh();
    }

    void onTrailerTailLeftStatusChanged(const ArgumentsTrailerTailLeftStatusChanged& args) {
        mLampFailTrailerTailLeftStatus = args.mInput_LampFailTrailerTailLeftStatus;
        updateTrailerTailLh();
    }

    void onTrailerTailRightStatusChanged(const ArgumentsTrailerTailRightStatusChanged& args) {
        mLampFailTrailerTailRightStatus = args.mInput_LampFailTrailerTailRightStatus;
        updateTrailerTailRh();
    }

    void onLedLampFailStatusChanged(const ArgumentsLedLampFailStatusChanged& args) {
        mLedLampFailStatusFromIcu = args.mInput_LedLampFailStatusFromIcu;
        if (ISTIMEOUT(args.mInput_LedLampLhFailStatusFromIlcu) == true) {
            mLedLampLhFailStatusFromIlcu = LedLampLhFailStatusFromIlcu::UNHANDLED_TIMEOUT;
        } else {
            mLedLampLhFailStatusFromIlcu = args.mInput_LedLampLhFailStatusFromIlcu;
        }
        if (ISTIMEOUT(args.mInput_LedLampRhFailStatusFromIlcu) == true) {
            mLedLampRhFailStatusFromIlcu = LedLampRhFailStatusFromIlcu::UNHANDLED_TIMEOUT;
        } else {
            mLedLampRhFailStatusFromIlcu = args.mInput_LedLampRhFailStatusFromIlcu;
        }
        updateLedLampFail();
    }

private:
    void updateAll() {
        updateLampFailBi();
        updateLampFailDrl();
        updateLampFailFog();
        updateLampFailHigh();
        updateMountStop();
        updateLampFailLow();
        updateLampFailCorner();
        updateLampFailStop();
        updateLampFailTurn();
        updateTrailerStop();
        updateTrailerTurnLh();
        updateTrailerTurnRh();
        updateTrailerTailLh();
        updateTrailerTailRh();
        updateLedLampFail();
    }

    void updateLampFailBi() {
        bool eventStat = false;
        std::bitset<2> prevBiFuncStat = mBiFuncStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailHeadBiFuncLeftStatus == LampFailHeadBiFuncLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailBiLhStat(SFCLamp_Fail_WarningConstantLampFailBiLhStat::ON);
                mBiFuncStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailBiLhStat(SFCLamp_Fail_WarningConstantLampFailBiLhStat::OFF);
                mBiFuncStat.set(kLHIndex, false);
            }
            if (mLampFailHeadBiFuncRightStatus == LampFailHeadBiFuncRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailBiRhStat(SFCLamp_Fail_WarningConstantLampFailBiRhStat::ON);
                mBiFuncStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailBiRhStat(SFCLamp_Fail_WarningConstantLampFailBiRhStat::OFF);
                mBiFuncStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailBiLhStat(SFCLamp_Fail_WarningConstantLampFailBiLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailBiRhStat(SFCLamp_Fail_WarningConstantLampFailBiRhStat::OFF);
            mBiFuncStat.reset();
        }
        if ((!prevBiFuncStat[kLHIndex] && mBiFuncStat[kLHIndex]) || (!prevBiFuncStat[kRHIndex] && mBiFuncStat[kRHIndex])) {
            eventStat = true;
        }

        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailBiStat(SFCLamp_Fail_WarningEventLampFailBiStat::ON);
        }
        if (mBiFuncStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailBiStat(SFCLamp_Fail_WarningEventLampFailBiStat::OFF);
        }
    }

    void updateLampFailDrl() {
        bool eventStat = false;
        std::bitset<2> prevDRLStat = mDRLStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailDrlLeftStatus == LampFailDrlLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailDrlLhStat(SFCLamp_Fail_WarningConstantLampFailDrlLhStat::ON);
                mDRLStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailDrlLhStat(SFCLamp_Fail_WarningConstantLampFailDrlLhStat::OFF);
                mDRLStat.set(kLHIndex, false);
            }
            if (mLampFailDrlRightStatus == LampFailDrlRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailDrlRhStat(SFCLamp_Fail_WarningConstantLampFailDrlRhStat::ON);
                mDRLStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailDrlRhStat(SFCLamp_Fail_WarningConstantLampFailDrlRhStat::OFF);
                mDRLStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailDrlLhStat(SFCLamp_Fail_WarningConstantLampFailDrlLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailDrlRhStat(SFCLamp_Fail_WarningConstantLampFailDrlRhStat::OFF);
            mDRLStat.reset();
        }

        if ((!prevDRLStat[kLHIndex] && mDRLStat[kLHIndex]) || (!prevDRLStat[kRHIndex] && mDRLStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailDrlStat(SFCLamp_Fail_WarningEventLampFailDrlStat::ON);
        }
        if (mDRLStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailDrlStat(SFCLamp_Fail_WarningEventLampFailDrlStat::OFF);
        }
    }

    void updateLampFailFog() {
        bool eventStat = false;
        std::bitset<2> prevFrontFogStat = mFrontFogStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailFrontFogLeftStatus == LampFailFrontFogLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailFrontFogLhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogLhStat::ON);
                mFrontFogStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailFrontFogLhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogLhStat::OFF);
                mFrontFogStat.set(kLHIndex, false);
            }
            if (mLampFailFrontFogRightStatus == LampFailFrontFogRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailFrontFogRhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogRhStat::ON);
                mFrontFogStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailFrontFogRhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogRhStat::OFF);
                mFrontFogStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailFrontFogLhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailFrontFogRhStat(SFCLamp_Fail_WarningConstantLampFailFrontFogRhStat::OFF);
            mFrontFogStat.reset();
        }

        if ((!prevFrontFogStat[kLHIndex] && mFrontFogStat[kLHIndex]) ||
            (!prevFrontFogStat[kRHIndex] && mFrontFogStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailFogStat(SFCLamp_Fail_WarningEventLampFailFogStat::ON);
        }
        if (mFrontFogStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailFogStat(SFCLamp_Fail_WarningEventLampFailFogStat::OFF);
        }
    }

    void updateLampFailHigh() {
        bool eventStat = false;
        std::bitset<2> prevHeadHighStat = mHeadHighStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailHeadHighLeftStatus == LampFailHeadHighLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailHighLhStat(SFCLamp_Fail_WarningConstantLampFailHighLhStat::ON);
                mHeadHighStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailHighLhStat(SFCLamp_Fail_WarningConstantLampFailHighLhStat::OFF);
                mHeadHighStat.set(kLHIndex, false);
            }
            if (mLampFailHeadHighRightStatus == LampFailHeadHighRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailHighRhStat(SFCLamp_Fail_WarningConstantLampFailHighRhStat::ON);
                mHeadHighStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailHighRhStat(SFCLamp_Fail_WarningConstantLampFailHighRhStat::OFF);
                mHeadHighStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailHighLhStat(SFCLamp_Fail_WarningConstantLampFailHighLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailHighRhStat(SFCLamp_Fail_WarningConstantLampFailHighRhStat::OFF);
            mHeadHighStat.reset();
        }

        if ((!prevHeadHighStat[kLHIndex] && mHeadHighStat[kLHIndex]) ||
            (!prevHeadHighStat[kRHIndex] && mHeadHighStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailHighStat(SFCLamp_Fail_WarningEventLampFailHighStat::ON);
        }
        if (mHeadHighStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailHighStat(SFCLamp_Fail_WarningEventLampFailHighStat::OFF);
        }
    }

    void updateMountStop() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailHeadMountStopLampStatus == LampFailHeadMountStopLampStatus::ON) {
            setSFCLamp_Fail_WarningEventLampFailMountStat(SFCLamp_Fail_WarningEventLampFailMountStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventLampFailMountStat(SFCLamp_Fail_WarningEventLampFailMountStat::OFF);
        }
    }

    void updateLampFailLow() {
        bool eventStat = false;
        std::bitset<2> prevHeadLowStat = mHeadLowStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailHeadLowLeftStatus == LampFailHeadLowLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailLowLhStat(SFCLamp_Fail_WarningConstantLampFailLowLhStat::ON);
                mHeadLowStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailLowLhStat(SFCLamp_Fail_WarningConstantLampFailLowLhStat::OFF);
                mHeadLowStat.set(kLHIndex, false);
            }
            if (mLampFailHeadLowRightStatus == LampFailHeadLowRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailLowRhStat(SFCLamp_Fail_WarningConstantLampFailLowRhStat::ON);
                mHeadLowStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailLowRhStat(SFCLamp_Fail_WarningConstantLampFailLowRhStat::OFF);
                mHeadLowStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailLowLhStat(SFCLamp_Fail_WarningConstantLampFailLowLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailLowRhStat(SFCLamp_Fail_WarningConstantLampFailLowRhStat::OFF);
            mHeadLowStat.reset();
        }

        if ((!prevHeadLowStat[kLHIndex] && mHeadLowStat[kLHIndex]) || (!prevHeadLowStat[kRHIndex] && mHeadLowStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailLowStat(SFCLamp_Fail_WarningEventLampFailLowStat::ON);
        }
        if (mHeadLowStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailLowStat(SFCLamp_Fail_WarningEventLampFailLowStat::OFF);
        }
    }

    void updateLampFailCorner() {
        bool eventStat = false;
        std::bitset<2> prevStaticBendingStat = mStaticBendingStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailStaticBendingLeftStatus == LampFailStaticBendingLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailCornerLhStat(SFCLamp_Fail_WarningConstantLampFailCornerLhStat::ON);
                mStaticBendingStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailCornerLhStat(SFCLamp_Fail_WarningConstantLampFailCornerLhStat::OFF);
                mStaticBendingStat.set(kLHIndex, false);
            }
            if (mLampFailStaticBendingRightStatus == LampFailStaticBendingRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailCornerRhStat(SFCLamp_Fail_WarningConstantLampFailCornerRhStat::ON);
                mStaticBendingStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailCornerRhStat(SFCLamp_Fail_WarningConstantLampFailCornerRhStat::OFF);
                mStaticBendingStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailCornerLhStat(SFCLamp_Fail_WarningConstantLampFailCornerLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailCornerRhStat(SFCLamp_Fail_WarningConstantLampFailCornerRhStat::OFF);
            mStaticBendingStat.reset();
        }

        if ((!prevStaticBendingStat[kLHIndex] && mStaticBendingStat[kLHIndex]) ||
            (!prevStaticBendingStat[kRHIndex] && mStaticBendingStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailCornerStat(SFCLamp_Fail_WarningEventLampFailCornerStat::ON);
        }
        if (mStaticBendingStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailCornerStat(SFCLamp_Fail_WarningEventLampFailCornerStat::OFF);
        }
    }

    void updateLampFailStop() {
        bool eventStat = false;
        std::bitset<2> prevStopLampStat = mStopLampStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailStopLampLeftStatus == LampFailStopLampLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailStopLhStat(SFCLamp_Fail_WarningConstantLampFailStopLhStat::ON);
                mStopLampStat.set(kLHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailStopLhStat(SFCLamp_Fail_WarningConstantLampFailStopLhStat::OFF);
                mStopLampStat.set(kLHIndex, false);
            }
            if (mLampFailStopLampRightStatus == LampFailStopLampRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailStopRhStat(SFCLamp_Fail_WarningConstantLampFailStopRhStat::ON);
                mStopLampStat.set(kRHIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailStopRhStat(SFCLamp_Fail_WarningConstantLampFailStopRhStat::OFF);
                mStopLampStat.set(kRHIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailStopLhStat(SFCLamp_Fail_WarningConstantLampFailStopLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailStopRhStat(SFCLamp_Fail_WarningConstantLampFailStopRhStat::OFF);
            mStopLampStat.reset();
        }

        if ((!prevStopLampStat[kLHIndex] && mStopLampStat[kLHIndex]) ||
            (!prevStopLampStat[kRHIndex] && mStopLampStat[kRHIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailStopStat(SFCLamp_Fail_WarningEventLampFailStopStat::ON);
        }
        if (mStopLampStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailStopStat(SFCLamp_Fail_WarningEventLampFailStopStat::OFF);
        }
    }

    void updateLampFailTurn() {
        bool eventStat = false;
        std::bitset<4> prevTurnSignalStat = mTurnSignalStat;

        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mLampFailFrontTurnLeftStatus == LampFailFrontTurnLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat::ON);
                mTurnSignalStat.set(kFLIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat::OFF);
                mTurnSignalStat.set(kFLIndex, false);
            }
            if (mLampFailFrontTurnRightStatus == LampFailFrontTurnRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat::ON);
                mTurnSignalStat.set(kFRIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat::OFF);
                mTurnSignalStat.set(kFRIndex, false);
            }
            if (mLampFailRearTurnLeftStatus == LampFailRearTurnLeftStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailRearTurnLhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnLhStat::ON);
                mTurnSignalStat.set(kRLIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailRearTurnLhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnLhStat::OFF);
                mTurnSignalStat.set(kRLIndex, false);
            }
            if (mLampFailRearTurnRightStatus == LampFailRearTurnRightStatus::ON) {
                setSFCLamp_Fail_WarningConstantLampFailRearTurnRhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnRhStat::ON);
                mTurnSignalStat.set(kRRIndex, true);
            } else {
                setSFCLamp_Fail_WarningConstantLampFailRearTurnRhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnRhStat::OFF);
                mTurnSignalStat.set(kRRIndex, false);
            }
        } else {
            setSFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat(SFCLamp_Fail_WarningConstantLampFailFrontTurnRhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailRearTurnLhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnLhStat::OFF);
            setSFCLamp_Fail_WarningConstantLampFailRearTurnRhStat(SFCLamp_Fail_WarningConstantLampFailRearTurnRhStat::OFF);
            mTurnSignalStat.reset();
        }

        if ((!prevTurnSignalStat[kFLIndex] && mTurnSignalStat[kFLIndex]) ||
            (!prevTurnSignalStat[kFRIndex] && mTurnSignalStat[kFRIndex]) ||
            (!prevTurnSignalStat[kRLIndex] && mTurnSignalStat[kRLIndex]) ||
            (!prevTurnSignalStat[kRRIndex] && mTurnSignalStat[kRRIndex])) {
            eventStat = true;
        }
        if (eventStat) {
            setSFCLamp_Fail_WarningEventLampFailTurnStat(SFCLamp_Fail_WarningEventLampFailTurnStat::ON);
        }
        if (mTurnSignalStat.none() || mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCLamp_Fail_WarningEventLampFailTurnStat(SFCLamp_Fail_WarningEventLampFailTurnStat::OFF);
        }
    }

    void updateTrailerStop() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailTrailerStopLampStatus == LampFailTrailerStopLampStatus::ON) {
            setSFCLamp_Fail_WarningEventTrailerStopStat(SFCLamp_Fail_WarningEventTrailerStopStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventTrailerStopStat(SFCLamp_Fail_WarningEventTrailerStopStat::OFF);
        }
    }

    void updateTrailerTurnLh() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailTrailerTurnLeftStatus == LampFailTrailerTurnLeftStatus::ON) {
            setSFCLamp_Fail_WarningEventTrailerTurnLeftStat(SFCLamp_Fail_WarningEventTrailerTurnLeftStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventTrailerTurnLeftStat(SFCLamp_Fail_WarningEventTrailerTurnLeftStat::OFF);
        }
    }

    void updateTrailerTurnRh() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailTrailerTurnRightStatus == LampFailTrailerTurnRightStatus::ON) {
            setSFCLamp_Fail_WarningEventTrailerTurnRightStat(SFCLamp_Fail_WarningEventTrailerTurnRightStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventTrailerTurnRightStat(SFCLamp_Fail_WarningEventTrailerTurnRightStat::OFF);
        }
    }
    void updateTrailerTailLh() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailTrailerTailLeftStatus == LampFailTrailerTailLeftStatus::ON) {
            setSFCLamp_Fail_WarningEventTrailerTailLeftStat(SFCLamp_Fail_WarningEventTrailerTailLeftStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventTrailerTailLeftStat(SFCLamp_Fail_WarningEventTrailerTailLeftStat::OFF);
        }
    }

    void updateTrailerTailRh() {
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mLampFailTrailerTailRightStatus == LampFailTrailerTailRightStatus::ON) {
            setSFCLamp_Fail_WarningEventTrailerTailRightStat(SFCLamp_Fail_WarningEventTrailerTailRightStat::ON);
        } else {
            setSFCLamp_Fail_WarningEventTrailerTailRightStat(SFCLamp_Fail_WarningEventTrailerTailRightStat::OFF);
        }
    }

    void updateLedLampFail() {
        if (mLedLampFailStatusFromIcu == LedLampFailStatusFromIcu::FAIL ||
            (mInter_ConfigILCU == Inter_ConfigILCU::ON &&
             ((mLedLampLhFailStatusFromIlcu == LedLampLhFailStatusFromIlcu::FAIL ||
               mLedLampLhFailStatusFromIlcu == LedLampLhFailStatusFromIlcu::UNHANDLED_TIMEOUT) ||
              (mLedLampRhFailStatusFromIlcu == LedLampRhFailStatusFromIlcu::FAIL ||
               mLedLampRhFailStatusFromIlcu == LedLampRhFailStatusFromIlcu::UNHANDLED_TIMEOUT)))) {
            // update event
            if (mIgnElapsed != IgnElapsed::OFF_0ms) {
                setSFCLamp_Fail_WarningEventLedLampFailStat(SFCLamp_Fail_WarningEventLedLampFailStat::ON);
            } else {
                setSFCLamp_Fail_WarningEventLedLampFailStat(SFCLamp_Fail_WarningEventLedLampFailStat::OFF);
            }
            // update telltale
            if (mIgnElapsed == IgnElapsed::ON_500ms) {
                setSFCLamp_Fail_WarningTelltaleLedLampFailStat(SFCLamp_Fail_WarningTelltaleLedLampFailStat::ON);
            } else {
                setSFCLamp_Fail_WarningTelltaleLedLampFailStat(SFCLamp_Fail_WarningTelltaleLedLampFailStat::OFF);
            }
        } else {
            // disable event & telltale
            setSFCLamp_Fail_WarningEventLedLampFailStat(SFCLamp_Fail_WarningEventLedLampFailStat::OFF);
            setSFCLamp_Fail_WarningTelltaleLedLampFailStat(SFCLamp_Fail_WarningTelltaleLedLampFailStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using Inter_ConfigILCU = ArgumentsVariantChanged::Inter_ConfigILCU;
    Inter_ConfigILCU mInter_ConfigILCU = Inter_ConfigILCU::OFF;
    using LampFailHeadBiFuncLeftStatus = ArgumentsHeadBiFuncStatusChanged::Input_LampFailHeadBiFuncLeftStatus;
    using LampFailHeadBiFuncRightStatus = ArgumentsHeadBiFuncStatusChanged::Input_LampFailHeadBiFuncRightStatus;
    LampFailHeadBiFuncLeftStatus mLampFailHeadBiFuncLeftStatus = LampFailHeadBiFuncLeftStatus::OFF;
    LampFailHeadBiFuncRightStatus mLampFailHeadBiFuncRightStatus = LampFailHeadBiFuncRightStatus::OFF;
    using LampFailDrlLeftStatus = ArgumentsDrlStatusChanged::Input_LampFailDrlLeftStatus;
    using LampFailDrlRightStatus = ArgumentsDrlStatusChanged::Input_LampFailDrlRightStatus;
    LampFailDrlLeftStatus mLampFailDrlLeftStatus = LampFailDrlLeftStatus::OFF;
    LampFailDrlRightStatus mLampFailDrlRightStatus = LampFailDrlRightStatus::OFF;
    using LampFailFrontFogLeftStatus = ArgumentsFogStatusChanged::Input_LampFailFrontFogLeftStatus;
    using LampFailFrontFogRightStatus = ArgumentsFogStatusChanged::Input_LampFailFrontFogRightStatus;
    LampFailFrontFogLeftStatus mLampFailFrontFogLeftStatus = LampFailFrontFogLeftStatus::OFF;
    LampFailFrontFogRightStatus mLampFailFrontFogRightStatus = LampFailFrontFogRightStatus::OFF;
    using LampFailHeadHighLeftStatus = ArgumentsHeadHighStatusChanged::Input_LampFailHeadHighLeftStatus;
    using LampFailHeadHighRightStatus = ArgumentsHeadHighStatusChanged::Input_LampFailHeadHighRightStatus;
    LampFailHeadHighLeftStatus mLampFailHeadHighLeftStatus = LampFailHeadHighLeftStatus::OFF;
    LampFailHeadHighRightStatus mLampFailHeadHighRightStatus = LampFailHeadHighRightStatus::OFF;
    using LampFailHeadMountStopLampStatus = ArgumentsHeadMountStopStatusChanged::Input_LampFailHeadMountStopLampStatus;
    LampFailHeadMountStopLampStatus mLampFailHeadMountStopLampStatus = LampFailHeadMountStopLampStatus::OFF;
    using LampFailHeadLowLeftStatus = ArgumentsHeadLowStatusChanged::Input_LampFailHeadLowLeftStatus;
    using LampFailHeadLowRightStatus = ArgumentsHeadLowStatusChanged::Input_LampFailHeadLowRightStatus;
    LampFailHeadLowLeftStatus mLampFailHeadLowLeftStatus = LampFailHeadLowLeftStatus::OFF;
    LampFailHeadLowRightStatus mLampFailHeadLowRightStatus = LampFailHeadLowRightStatus::OFF;
    using LampFailStaticBendingLeftStatus = ArgumentsStaticBendingStatusChanged::Input_LampFailStaticBendingLeftStatus;
    using LampFailStaticBendingRightStatus = ArgumentsStaticBendingStatusChanged::Input_LampFailStaticBendingRightStatus;
    LampFailStaticBendingLeftStatus mLampFailStaticBendingLeftStatus = LampFailStaticBendingLeftStatus::OFF;
    LampFailStaticBendingRightStatus mLampFailStaticBendingRightStatus = LampFailStaticBendingRightStatus::OFF;
    using LampFailStopLampLeftStatus = ArgumentsStopLampStatusChanged::Input_LampFailStopLampLeftStatus;
    using LampFailStopLampRightStatus = ArgumentsStopLampStatusChanged::Input_LampFailStopLampRightStatus;
    LampFailStopLampLeftStatus mLampFailStopLampLeftStatus = LampFailStopLampLeftStatus::OFF;
    LampFailStopLampRightStatus mLampFailStopLampRightStatus = LampFailStopLampRightStatus::OFF;
    using LampFailFrontTurnLeftStatus = ArgumentsTurnSignalStatusChanged::Input_LampFailFrontTurnSignalLeftStatus;
    using LampFailFrontTurnRightStatus = ArgumentsTurnSignalStatusChanged::Input_LampFailFrontTurnSignalRightStatus;
    using LampFailRearTurnLeftStatus = ArgumentsTurnSignalStatusChanged::Input_LampFailRearTurnSignalLeftStatus;
    using LampFailRearTurnRightStatus = ArgumentsTurnSignalStatusChanged::Input_LampFailRearTurnSignalRightStatus;
    LampFailFrontTurnLeftStatus mLampFailFrontTurnLeftStatus = LampFailFrontTurnLeftStatus::OFF;
    LampFailFrontTurnRightStatus mLampFailFrontTurnRightStatus = LampFailFrontTurnRightStatus::OFF;
    LampFailRearTurnLeftStatus mLampFailRearTurnLeftStatus = LampFailRearTurnLeftStatus::OFF;
    LampFailRearTurnRightStatus mLampFailRearTurnRightStatus = LampFailRearTurnRightStatus::OFF;
    using LampFailTrailerStopLampStatus = ArgumentsTrailerStopStatusChanged::Input_LampFailTrailerStopLampStatus;
    LampFailTrailerStopLampStatus mLampFailTrailerStopLampStatus = LampFailTrailerStopLampStatus::OFF;
    using LampFailTrailerTurnLeftStatus = ArgumentsTrailerTurnLeftStatusChanged::Input_LampFailTrailerTurnSignalLeftStatus;
    LampFailTrailerTurnLeftStatus mLampFailTrailerTurnLeftStatus = LampFailTrailerTurnLeftStatus::OFF;
    using LampFailTrailerTurnRightStatus = ArgumentsTrailerTurnRightStatusChanged::Input_LampFailTrailerTurnSignalRightStatus;
    LampFailTrailerTurnRightStatus mLampFailTrailerTurnRightStatus = LampFailTrailerTurnRightStatus::OFF;
    using LampFailTrailerTailLeftStatus = ArgumentsTrailerTailLeftStatusChanged::Input_LampFailTrailerTailLeftStatus;
    LampFailTrailerTailLeftStatus mLampFailTrailerTailLeftStatus = LampFailTrailerTailLeftStatus::OFF;
    using LampFailTrailerTailRightStatus = ArgumentsTrailerTailRightStatusChanged::Input_LampFailTrailerTailRightStatus;
    LampFailTrailerTailRightStatus mLampFailTrailerTailRightStatus = LampFailTrailerTailRightStatus::OFF;
    using LedLampFailStatusFromIcu = ArgumentsLedLampFailStatusChanged::Input_LedLampFailStatusFromIcu;
    using LedLampLhFailStatusFromIlcu = ArgumentsLedLampFailStatusChanged::Input_LedLampLhFailStatusFromIlcu;
    using LedLampRhFailStatusFromIlcu = ArgumentsLedLampFailStatusChanged::Input_LedLampRhFailStatusFromIlcu;
    LedLampFailStatusFromIcu mLedLampFailStatusFromIcu = LedLampFailStatusFromIcu::NORMAL;
    LedLampLhFailStatusFromIlcu mLedLampLhFailStatusFromIlcu = LedLampLhFailStatusFromIlcu::NORMAL;
    LedLampRhFailStatusFromIlcu mLedLampRhFailStatusFromIlcu = LedLampRhFailStatusFromIlcu::NORMAL;

    static constexpr HUInt64 kLHIndex = 0;
    static constexpr HUInt64 kRHIndex = 1;
    static constexpr HUInt64 kFLIndex = 0;
    static constexpr HUInt64 kFRIndex = 1;
    static constexpr HUInt64 kRLIndex = 2;
    static constexpr HUInt64 kRRIndex = 3;
    std::bitset<2> mBiFuncStat;         // {LH, RH}, BiFunc
    std::bitset<2> mDRLStat;            // {LH, RH}, DRL
    std::bitset<2> mFrontFogStat;       // {LH, RH}, FrontFog
    std::bitset<2> mHeadHighStat;       // {LH, RH}, HeadHigh
    std::bitset<2> mHeadLowStat;        // {LH, RH}, HeadLow
    std::bitset<2> mStaticBendingStat;  // {LH, RH}, StaticBending
    std::bitset<2> mStopLampStat;       // {LH, RH}, StopLamp
    std::bitset<4> mTurnSignalStat;     // {FL, FR, RL, RR} TurnSignal
};

}  // namespace ccos

#endif  // SFSS_Lamp_Fail_Warning_H
