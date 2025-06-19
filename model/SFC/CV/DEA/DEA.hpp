/**
 * @file DEA.hpp
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
#ifndef SFSS_DEA_H
#define SFSS_DEA_H

#define DLOG_ENABLED gEnableSFCLog

#include "DEABase.hpp"

namespace ccos {

// SFC Version : 1.0.1
// Spec Version : v0.16
// Reference : [CV040] DEA. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class DEA : public DEABase {
public:
    DEA() = default;
    ~DEA() override = default;
    DEA(const DEA& other) = delete;
    DEA(DEA&& other) noexcept = delete;
    DEA& operator=(const DEA& other) = delete;
    DEA& operator=(DEA&& other) noexcept = delete;

    void onInitialize() override {
        // DEA 경고
        setSFCDEAEventDEAWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCDEAEventDEAWarnLinkedSoundType(SFCDEAEventDEAWarnLinkedSoundType::REPEAT_COUNT);
        setSFCDEAEventDEAWarnLinkedSoundRepeatCount(1);

        // DEA 고장 알림
        setSFCDEAEventDEAFailureID("E70403");
        setSFCDEAEventDEAFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCDEAEventDEAFailureLinkedSoundType(SFCDEAEventDEAFailureLinkedSoundType::REPEAT_COUNT);
        setSFCDEAEventDEAFailureLinkedSoundRepeatCount(1);

        // 사운드 로직
        setSFCDEASoundDEASoundWarnID("SND_ADAS_PopUpWarn1_Auto");
        setSFCDEASoundDEASoundWarnType(SFCDEASoundDEASoundWarnType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleDEAStat();
        updateEventDEAWarnStat();
        updateEventDEAFailureStat();
        updateSoundDEAStat();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleDEAStat();
        updateEventDEAWarnStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleDEAStat();
        updateEventDEAWarnStat();
        updateEventDEAFailureStat();
        updateSoundDEAStat();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigDEA = args.mInter_ConfigDEA;
        updateTelltaleDEAStat();
        updateEventDEAWarnStat();
        updateEventDEAFailureStat();
        updateSoundDEAStat();
    }

    void onDeaSymbStaChanged(const ArgumentsDeaSymbStaChanged& args) {
        mDEASymbSta = args.mInput_DEASymbSta;
        updateTelltaleDEAStat();
    }

    void onDeaWarnStaChanged(const ArgumentsDeaWarnStaChanged& args) {
        mDEAWarnSta = args.mInput_DEAWarnSta;
        updateEventDEAWarnStat();
    }

    void onDeaFailureStaChanged(const ArgumentsDeaFailureStaChanged& args) {
        mDEAFailureSta = args.mInput_DEAFailureSta;
        updateEventDEAFailureStat();
    }

    void onDeaSoundWarnChanged(const ArgumentsDeaSoundWarnChanged& args) {
        mDEASoundWarn = args.mInput_DEASoundWarn;
        updateSoundDEAStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleDEAStat() {
        if (mConfigDEA != ConfigDEA::ON) {
            return;
        }

        SFCDEATelltaleDEAStat stat = SFCDEATelltaleDEAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            stat = SFCDEATelltaleDEAStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDEASymbSta == DEASymbSta::ON || mDEASymbSta == DEASymbSta::MESSAGE_TIMEOUT) {
                stat = SFCDEATelltaleDEAStat::ON;
            }
        } else {
            // no operation
        }

        setSFCDEATelltaleDEAStat(stat);
    }

    void updateEventDEAWarnStat() {
        if (mConfigDEA != ConfigDEA::ON) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDEAWarnSta == DEAWarnSta::WARNING) {
                eventID = "E70401";
            } else if (mDEAWarnSta == DEAWarnSta::STOP) {
                eventID = "E70402";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.DEA.Event.DEAWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDEAEventDEAWarnStat(SFCDEAEventDEAWarnStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCDEAEventDEAWarnID(eventID);
            setSFCDEAEventDEAWarnStat(SFCDEAEventDEAWarnStat::ON);
        }
    }

    void updateEventDEAFailureStat() {
        if (mConfigDEA != ConfigDEA::ON) {
            return;
        }

        SFCDEAEventDEAFailureStat stat = SFCDEAEventDEAFailureStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDEAFailureSta == DEAFailureSta::FAILURE) {
                stat = SFCDEAEventDEAFailureStat::ON;
            }
        }

        setSFCDEAEventDEAFailureStat(stat);
    }

    void updateSoundDEAStat() {
        if (mConfigDEA != ConfigDEA::ON) {
            return;
        }

        SFCDEASoundDEASoundWarnStat stat = SFCDEASoundDEASoundWarnStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDEASoundWarn == DEASoundWarn::WARN) {
                stat = SFCDEASoundDEASoundWarnStat::ON;
            }
        }

        setSFCDEASoundDEASoundWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigDEA = ArgumentsConfigChanged::Inter_ConfigDEA;
    using DEASymbSta = ArgumentsDeaSymbStaChanged::Input_DEASymbSta;
    using DEAWarnSta = ArgumentsDeaWarnStaChanged::Input_DEAWarnSta;
    using DEAFailureSta = ArgumentsDeaFailureStaChanged::Input_DEAFailureSta;
    using DEASoundWarn = ArgumentsDeaSoundWarnChanged::Input_DEASoundWarn;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigDEA mConfigDEA = ConfigDEA::OFF;
    DEASymbSta mDEASymbSta = DEASymbSta::OFF;
    DEAWarnSta mDEAWarnSta = DEAWarnSta::OFF;
    DEAFailureSta mDEAFailureSta = DEAFailureSta::OFF;
    DEASoundWarn mDEASoundWarn = DEASoundWarn::OFF;
};

}  // namespace ccos

#endif  // SFSS_DEA_H
