/**
 * @file MDPS.hpp
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
#ifndef SFSS_MDPS_H
#define SFSS_MDPS_H

#define DLOG_ENABLED gEnableSFCLog

#include "MDPSBase.hpp"
#include <Logger.h>

namespace ccos {

// SFC Version : 3.0.0
// Reference : [CH120] MDPS. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED"
class MDPS : public MDPSBase {
public:
    MDPS() = default;
    ~MDPS() override = default;
    MDPS(const MDPS& other) = delete;
    MDPS(MDPS&& other) noexcept = delete;
    MDPS& operator=(const MDPS& other) = delete;
    MDPS& operator=(MDPS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCMDPSTelltaleMdpsWarnStat(SFCMDPSTelltaleMdpsWarnStat::OFF);
        setSFCMDPSTelltaleMdpsWarnBlinkValueA(SFCMDPSTelltaleMdpsWarnBlinkValueA::ON);
        setSFCMDPSTelltaleMdpsWarnBlinkValueB(SFCMDPSTelltaleMdpsWarnBlinkValueB::OFF);
        setSFCMDPSTelltaleMdpsWarnStatOptional(SFCMDPSTelltaleMdpsWarnStatOptional::WARN_AMBER);

        setSFCMDPSEventMDPSSysWarningID("E30901");
        setSFCMDPSEventMDPSSysWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCMDPSEventMDPSSysWarningLinkedSoundType(SFCMDPSEventMDPSSysWarningLinkedSoundType::REPEAT_COUNT);
        setSFCMDPSEventMDPSSysWarningLinkedSoundRepeatCount(1);

        setSFCMDPSEventSfaSteerLongTimerID("E30913");
        setSFCMDPSEventSfaSteerLongTimerLinkedSoundID("SND_PopUpWarn1");
        setSFCMDPSEventSfaSteerLongTimerLinkedSoundType(SFCMDPSEventSfaSteerLongTimerLinkedSoundType::REPEAT_COUNT);
        setSFCMDPSEventSfaSteerLongTimerLinkedSoundRepeatCount(1);

        setSFCMDPSEventSfaSteerShortTimerID("E30914");
        setSFCMDPSEventSfaSteerShortTimerLinkedSoundID("SND_PopUpWarn1");
        setSFCMDPSEventSfaSteerShortTimerLinkedSoundType(SFCMDPSEventSfaSteerShortTimerLinkedSoundType::REPEAT_COUNT);
        setSFCMDPSEventSfaSteerShortTimerLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateEventMdpsSysWarn();
        updateEventSfaAlignWarn();
        updateEventSfaWarn();

        updateEventSfaLongTimer();
        updateEventSfaShortTimer();
        updateEventSfaWithoutTimer();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateEventMdpsSysWarn();
        updateEventSfaAlignWarn();
        updateEventSfaWarn();

        updateEventSfaLongTimer();
        updateEventSfaShortTimer();
        updateEventSfaWithoutTimer();
    }

    void onConfigMDPSChanged(const ArgumentsConfigMDPSChanged& args) {
        mConfigMDPS = args.mInter_ConfigMDPS;
        updateTelltale();
        updateEventMdpsSysWarn();
        updateEventSfaAlignWarn();
        updateEventSfaWarn();

        updateEventSfaLongTimer();
        updateEventSfaShortTimer();
        updateEventSfaWithoutTimer();

        updateConstant();
    }

    void onMdpsStatChanged(const ArgumentsMdpsStatChanged& args) {
        if (ISTIMEOUT(args.mInput_SfaLampStatus) == true) {
            mSfaLampStatus = SfaLampStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfaLampStatus = args.mInput_SfaLampStatus;
        }

        if (ISTIMEOUT(args.mInput_Sfa2LampStatus) == true) {
            mSfa2LampStatus = Sfa2LampStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfa2LampStatus = args.mInput_Sfa2LampStatus;
        }

        if (ISTIMEOUT(args.mInput_MdpsWarnStatus) == true) {
            mMdpsWarnStatus = MdpsWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mMdpsWarnStatus = args.mInput_MdpsWarnStatus;
        }

        mConfigSteerByWire = args.mInter_ConfigSteerByWire;

        updateTelltale();
    }

    void onMdpsSysWarnChanged(const ArgumentsMdpsSysWarnChanged& args) {
        mMdpsSysWarnStatus = args.mInput_MdpsSysWarnStatus;
        updateEventMdpsSysWarn();
    }

    void onSfaAlignWarnStatusChanged(const ArgumentsSfaAlignWarnStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_SfaAlignWarnStatus) == true) {
            mSfaAlignWarnStatus = SfaAlignWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfaAlignWarnStatus = args.mInput_SfaAlignWarnStatus;
        }

        if (ISTIMEOUT(args.mInput_Sfa2AlignWarnStatus) == true) {
            mSfa2AlignWarnStatus = Sfa2AlignWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfa2AlignWarnStatus = args.mInput_Sfa2AlignWarnStatus;
        }

        updateEventSfaAlignWarn();
    }

    void onSfaWarnStatusChanged(const ArgumentsSfaWarnStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_SfaWarnStatus) == true) {
            mSfaWarnStatus = SfaWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfaWarnStatus = args.mInput_SfaWarnStatus;
        }

        if (ISTIMEOUT(args.mInput_Sfa2WarnStatus) == true) {
            mSfa2WarnStatus = Sfa2WarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfa2WarnStatus = args.mInput_Sfa2WarnStatus;
        }

        updateEventSfaWarn();
    }

    void onSfaTimerChanged(const ArgumentsSfaTimerChanged& args) {
        if (ISTIMEOUT(args.mInput_SfaSpeedLimitWarnStatus) == true) {
            mSfaTimerSpeedLimitWarnStatus = SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfaTimerSpeedLimitWarnStatus = args.mInput_SfaSpeedLimitWarnStatus;
        }

        if (ISTIMEOUT(args.mInput_Sfa2SpeedLimitWarnStatus) == true) {
            mSfa2TimerSpeedLimitWarnStatus = Sfa2TimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSfa2TimerSpeedLimitWarnStatus = args.mInput_Sfa2SpeedLimitWarnStatus;
        }

        mSfaLongTimerSecValue = args.mInput_SfaLongTimerSecValue;
        mSfa2LongTimerSecValue = args.mInput_Sfa2LongTimerSecValue;
        mSfaShortTimerSecValue = args.mInput_SfaShortTimerSecValue;
        mSfa2ShortTimerSecValue = args.mInput_Sfa2ShortTimerSecValue;

        updateConstant();

        updateEventSfaLongTimer();
        updateEventSfaShortTimer();
        updateEventSfaWithoutTimer();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        updateLongTimerConstant();
        updateShortTimerConstant();
    }

    void updateLongTimerConstant() {
        HUInt64 value = 0;

        if (mConfigMDPS == ConfigMDPS::ON) {
            if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SPD_MID_SLOW) &&
                (mSfaLongTimerSecValue > 0 && mSfaLongTimerSecValue < 3600)) {
                value = mSfaLongTimerSecValue;
            } else if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT) &&
                       ((mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SPD_MID_SLOW) &&
                        (mSfa2LongTimerSecValue > 0 && mSfa2LongTimerSecValue < 3600))) {
                value = mSfa2LongTimerSecValue;
            }
        }

        setSFCMDPSConstantSteerLongTimerSecValue(value);
    }

    void updateShortTimerConstant() {
        HUInt64 value = 0;

        if (mConfigMDPS == ConfigMDPS::ON) {
            if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SPD_MID) &&
                (mSfaShortTimerSecValue > 0 && mSfaShortTimerSecValue < 255)) {
                value = mSfaShortTimerSecValue;
            } else if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT) &&
                       ((mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SPD_MID) &&
                        (mSfa2ShortTimerSecValue > 0 && mSfa2ShortTimerSecValue < 255))) {
                value = mSfa2ShortTimerSecValue;
            }
        }

        setSFCMDPSConstantSteerShortTimerSecValue(value);
    }

    void updateTelltale() {
        if (mConfigMDPS == ConfigMDPS::ON) {
            SFCMDPSTelltaleMdpsWarnStat stat = SFCMDPSTelltaleMdpsWarnStat::OFF;
            if (mIgnElapsed == IgnElapsed::ON_500ms) {
                stat = SFCMDPSTelltaleMdpsWarnStat::ON;
            } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                if (mConfigSteerByWire == ConfigSteerByWire::OFF) {
                    if (mMdpsWarnStatus == MdpsWarnStatus::ON || mMdpsWarnStatus == MdpsWarnStatus::UNHANDLED_TIMEOUT ||
                        mMdpsWarnStatus == MdpsWarnStatus::CRC_ERROR) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::ON;
                    } else if (mMdpsWarnStatus == MdpsWarnStatus::BLINK1) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::BLINK1;
                    } else if (mMdpsWarnStatus == MdpsWarnStatus::BLINK2) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::BLINK2;
                    } else {
                        // OFF
                    }
                } else if (mConfigSteerByWire == ConfigSteerByWire::ON) {
                    if (mSfaLampStatus == SfaLampStatus::WARN_AMBER ||
                        ((mSfaLampStatus == SfaLampStatus::UNHANDLED_TIMEOUT || mSfaLampStatus == SfaLampStatus::CRC_ERROR) &&
                         mSfa2LampStatus == Sfa2LampStatus::WARN_AMBER)) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::OPTIONAL;
                    } else if (mSfaLampStatus == SfaLampStatus::BLINK_RED ||
                               ((mSfaLampStatus == SfaLampStatus::UNHANDLED_TIMEOUT ||
                                 mSfaLampStatus == SfaLampStatus::CRC_ERROR) &&
                                mSfa2LampStatus == Sfa2LampStatus::BLINK_RED)) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::BLINK2;
                    } else if (mSfaLampStatus == SfaLampStatus::WARN_RED ||
                               ((mSfaLampStatus == SfaLampStatus::UNHANDLED_TIMEOUT ||
                                 mSfaLampStatus == SfaLampStatus::CRC_ERROR) &&
                                (mSfa2LampStatus == Sfa2LampStatus::WARN_RED ||
                                 mSfa2LampStatus == Sfa2LampStatus::UNHANDLED_TIMEOUT ||
                                 mSfa2LampStatus == Sfa2LampStatus::CRC_ERROR))) {
                        stat = SFCMDPSTelltaleMdpsWarnStat::ON;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
            setSFCMDPSTelltaleMdpsWarnStat(stat);
        }
    }

    void updateEventMdpsSysWarn() {
        if (mConfigMDPS == ConfigMDPS::ON && mMdpsSysWarnStatus == MdpsSysWarnStatus::ON &&
            (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms)) {
            setSFCMDPSEventMDPSSysWarningStat(SFCMDPSEventMDPSSysWarningStat::ON);
        } else {
            setSFCMDPSEventMDPSSysWarningStat(SFCMDPSEventMDPSSysWarningStat::OFF);
        }
    }

    void updateEventSfaAlignWarn() {
        std::string eventID;
        std::string linkedSoundID;
        SFCMDPSEventSfaAlignWarnLinkedSoundType linkedSoundType = SFCMDPSEventSfaAlignWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mConfigMDPS == ConfigMDPS::ON) {
            if (mSfaAlignWarnStatus == SfaAlignWarnStatus::ALIGNING_HANDSOFF ||
                (mSfaAlignWarnStatus == SfaAlignWarnStatus::UNHANDLED_TIMEOUT &&
                 mSfa2AlignWarnStatus == Sfa2AlignWarnStatus::ALIGNING_HANDSOFF)) {
                eventID = "E30902";
            } else if (mSfaAlignWarnStatus == SfaAlignWarnStatus::ALIGN_COMPLETE ||
                       (mSfaAlignWarnStatus == SfaAlignWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2AlignWarnStatus == Sfa2AlignWarnStatus::ALIGN_COMPLETE)) {
                eventID = "E30903";
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCMDPSEventSfaAlignWarnLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mSfaAlignWarnStatus == SfaAlignWarnStatus::ALIGNING_NOTOUCH ||
                       (mSfaAlignWarnStatus == SfaAlignWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2AlignWarnStatus == Sfa2AlignWarnStatus::ALIGNING_NOTOUCH)) {
                eventID = "E30904";
            } else if (mSfaAlignWarnStatus == SfaAlignWarnStatus::ALIGN_RESTART ||
                       (mSfaAlignWarnStatus == SfaAlignWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2AlignWarnStatus == Sfa2AlignWarnStatus::ALIGN_RESTART)) {
                eventID = "E30905";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCMDPSEventSfaAlignWarnLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mSfaAlignWarnStatus == SfaAlignWarnStatus::ALIGN_STOP ||
                       (mSfaAlignWarnStatus == SfaAlignWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2AlignWarnStatus == Sfa2AlignWarnStatus::ALIGN_STOP)) {
                eventID = "E30906";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCMDPSEventSfaAlignWarnLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.MDPS.Event.SfaAlignWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCMDPSEventSfaAlignWarnStat(SFCMDPSEventSfaAlignWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCMDPSEventSfaAlignWarnID(eventID);
            setSFCMDPSEventSfaAlignWarnStat(SFCMDPSEventSfaAlignWarnStat::ON);
            setSFCMDPSEventSfaAlignWarnLinkedSoundID(linkedSoundID);
            setSFCMDPSEventSfaAlignWarnLinkedSoundType(linkedSoundType);
            setSFCMDPSEventSfaAlignWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateEventSfaWarn() {
        std::string eventID;
        std::string linkedSoundID;
        SFCMDPSEventSfaWarnLinkedSoundType linkedSoundType = SFCMDPSEventSfaWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mConfigMDPS == ConfigMDPS::ON) {
            if (mSfaWarnStatus == SfaWarnStatus::OVERHEAT ||
                (mSfaWarnStatus == SfaWarnStatus::UNHANDLED_TIMEOUT && mSfa2WarnStatus == Sfa2WarnStatus::OVERHEAT)) {
                eventID = "E30907";
            } else if (mSfaWarnStatus == SfaWarnStatus::CHECK ||
                       (mSfaWarnStatus == SfaWarnStatus::UNHANDLED_TIMEOUT && mSfa2WarnStatus == Sfa2WarnStatus::CHECK)) {
                eventID = "E30908";
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCMDPSEventSfaWarnLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mSfaWarnStatus == SfaWarnStatus::ERROR_ALIGN ||
                       (mSfaWarnStatus == SfaWarnStatus::UNHANDLED_TIMEOUT && mSfa2WarnStatus == Sfa2WarnStatus::ERROR_ALIGN)) {
                eventID = "E30909";
            } else {
                // nothing
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.MDPS.Event.SfaWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCMDPSEventSfaWarnStat(SFCMDPSEventSfaWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCMDPSEventSfaWarnID(eventID);
            setSFCMDPSEventSfaWarnStat(SFCMDPSEventSfaWarnStat::ON);
            setSFCMDPSEventSfaWarnLinkedSoundID(linkedSoundID);
            setSFCMDPSEventSfaWarnLinkedSoundType(linkedSoundType);
            setSFCMDPSEventSfaWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateEventSfaLongTimer() {
        SFCMDPSEventSfaSteerLongTimerStat stat = SFCMDPSEventSfaSteerLongTimerStat::OFF;

        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mConfigMDPS == ConfigMDPS::ON) {
            if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SPD_MID_SLOW && mSfaLongTimerSecValue > 0 &&
                 mSfaLongTimerSecValue < 3600) ||
                (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                 mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SPD_MID_SLOW && mSfa2LongTimerSecValue > 0 &&
                 mSfa2LongTimerSecValue < 3600)) {
                stat = SFCMDPSEventSfaSteerLongTimerStat::ON;
            }
        }

        setSFCMDPSEventSfaSteerLongTimerStat(stat);
    }

    void updateEventSfaShortTimer() {
        SFCMDPSEventSfaSteerShortTimerStat stat = SFCMDPSEventSfaSteerShortTimerStat::OFF;

        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mConfigMDPS == ConfigMDPS::ON) {
            if ((mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SPD_MID && mSfaShortTimerSecValue > 0 &&
                 mSfaShortTimerSecValue < 255) ||
                (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                 mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SPD_MID && mSfa2ShortTimerSecValue > 0 &&
                 mSfa2ShortTimerSecValue < 255)) {
                stat = SFCMDPSEventSfaSteerShortTimerStat::ON;
            }
        }

        setSFCMDPSEventSfaSteerShortTimerStat(stat);
    }

    void updateEventSfaWithoutTimer() {
        std::string eventID;
        std::string linkedSoundID = "SND_PopUpWarn1";
        SFCMDPSEventSfaSteerWithoutTimerLinkedSoundType linkedSoundType =
            SFCMDPSEventSfaSteerWithoutTimerLinkedSoundType::REPEAT_COUNT;
        HUInt64 repeatCount = 1;
        HUInt64 duration = 0;

        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mConfigMDPS == ConfigMDPS::ON) {
            if (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SPD_SLOW ||
                (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                 mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SPD_SLOW)) {
                eventID = "E30915";
            } else if (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::STOP ||
                       (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::STOP)) {
                eventID = "E30916";
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCMDPSEventSfaSteerWithoutTimerLinkedSoundType::DURATION;
                repeatCount = 0;
                duration = 1000;
            } else if (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::MOVE ||
                       (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::MOVE)) {
                eventID = "E30917";
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCMDPSEventSfaSteerWithoutTimerLinkedSoundType::DURATION;
                repeatCount = 0;
                duration = 1000;
            } else if (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::SYS_CHECK ||
                       (mSfaTimerSpeedLimitWarnStatus == SfaTimerSpeedLimitWarnStatus::UNHANDLED_TIMEOUT &&
                        mSfa2TimerSpeedLimitWarnStatus == Sfa2TimerSpeedLimitWarnStatus::SYS_CHECK)) {
                eventID = "E30918";
            } else {
                // Nothing to do
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.MDPS.Event.SfaSteerWithoutTimer.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCMDPSEventSfaSteerWithoutTimerStat(SFCMDPSEventSfaSteerWithoutTimerStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCMDPSEventSfaSteerWithoutTimerID(eventID);
            setSFCMDPSEventSfaSteerWithoutTimerStat(SFCMDPSEventSfaSteerWithoutTimerStat::ON);
            setSFCMDPSEventSfaSteerWithoutTimerLinkedSoundID(linkedSoundID);
            setSFCMDPSEventSfaSteerWithoutTimerLinkedSoundType(linkedSoundType);
            setSFCMDPSEventSfaSteerWithoutTimerLinkedSoundRepeatCount(repeatCount);
            setSFCMDPSEventSfaSteerWithoutTimerLinkedSoundDuration(duration);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using ConfigMDPS = ArgumentsConfigMDPSChanged::Inter_ConfigMDPS;
    using ConfigSteerByWire = ArgumentsMdpsStatChanged::Inter_ConfigSteerByWire;
    using MdpsWarnStatus = ArgumentsMdpsStatChanged::Input_MdpsWarnStatus;
    using SfaLampStatus = ArgumentsMdpsStatChanged::Input_SfaLampStatus;
    using Sfa2LampStatus = ArgumentsMdpsStatChanged::Input_Sfa2LampStatus;
    using MdpsSysWarnStatus = ArgumentsMdpsSysWarnChanged::Input_MdpsSysWarnStatus;
    using SfaAlignWarnStatus = ArgumentsSfaAlignWarnStatusChanged::Input_SfaAlignWarnStatus;
    using Sfa2AlignWarnStatus = ArgumentsSfaAlignWarnStatusChanged::Input_Sfa2AlignWarnStatus;
    using SfaWarnStatus = ArgumentsSfaWarnStatusChanged::Input_SfaWarnStatus;
    using Sfa2WarnStatus = ArgumentsSfaWarnStatusChanged::Input_Sfa2WarnStatus;
    using SfaTimerSpeedLimitWarnStatus = ArgumentsSfaTimerChanged::Input_SfaSpeedLimitWarnStatus;
    using Sfa2TimerSpeedLimitWarnStatus = ArgumentsSfaTimerChanged::Input_Sfa2SpeedLimitWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigMDPS mConfigMDPS = ConfigMDPS::OFF;
    ConfigSteerByWire mConfigSteerByWire = ConfigSteerByWire::OFF;
    SfaLampStatus mSfaLampStatus = SfaLampStatus::OFF;
    Sfa2LampStatus mSfa2LampStatus = Sfa2LampStatus::OFF;
    MdpsWarnStatus mMdpsWarnStatus = MdpsWarnStatus::OFF;
    MdpsSysWarnStatus mMdpsSysWarnStatus = MdpsSysWarnStatus::OFF;
    SfaAlignWarnStatus mSfaAlignWarnStatus = SfaAlignWarnStatus::OFF;
    Sfa2AlignWarnStatus mSfa2AlignWarnStatus = Sfa2AlignWarnStatus::OFF;
    SfaWarnStatus mSfaWarnStatus = SfaWarnStatus::OFF;
    Sfa2WarnStatus mSfa2WarnStatus = Sfa2WarnStatus::OFF;
    SfaTimerSpeedLimitWarnStatus mSfaTimerSpeedLimitWarnStatus = SfaTimerSpeedLimitWarnStatus::OFF;
    Sfa2TimerSpeedLimitWarnStatus mSfa2TimerSpeedLimitWarnStatus = Sfa2TimerSpeedLimitWarnStatus::OFF;

    HUInt64 mSfaLongTimerSecValue = 0;
    HUInt64 mSfa2LongTimerSecValue = 0;
    HUInt64 mSfaShortTimerSecValue = 0;
    HUInt64 mSfa2ShortTimerSecValue = 0;
};

}  // namespace ccos

#endif  // SFSS_MDPS_H
