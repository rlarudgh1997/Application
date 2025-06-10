/**
 * @file Drift_Mode.hpp
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
#ifndef SFSS_Drift_Mode_H
#define SFSS_Drift_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Drift_ModeBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [CH190] Drift Mode. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Drift_Mode : public Drift_ModeBase {
public:
    Drift_Mode() = default;
    ~Drift_Mode() override = default;
    Drift_Mode(const Drift_Mode& other) = delete;
    Drift_Mode(Drift_Mode&& other) noexcept = delete;
    Drift_Mode& operator=(const Drift_Mode& other) = delete;
    Drift_Mode& operator=(Drift_Mode&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDrift_ModeEventDriftModeLinkedSoundType(SFCDrift_ModeEventDriftModeLinkedSoundType::REPEAT_COUNT);
        setSFCDrift_ModeEventDriftModeLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateEvent();
    }

    void onEventValueChanged(const ArgumentsEventValueChanged& args) {
        mDisplayReqStatus = args.mInput_DriftModeDisplayReqStatus;
        updateEvent();
    }

    void onTelltaleSignalChanged(const ArgumentsTelltaleSignalChanged& args) {
        mIndiReqStatus = args.mInput_DriftModeIndiReqStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        std::tuple<SFCDrift_ModeTelltaleDriftModeStat, SFCDrift_ModeTelltaleDriftModeStatOptional> telltaleStat{
            SFCDrift_ModeTelltaleDriftModeStat::OFF, SFCDrift_ModeTelltaleDriftModeStatOptional::OFF};

        if (mIgnElapsed >= IgnElapsed::ON_500ms && mIndiReqStatus == IndiReqStatus::ON) {
            telltaleStat = {SFCDrift_ModeTelltaleDriftModeStat::OPTIONAL, SFCDrift_ModeTelltaleDriftModeStatOptional::ON_DRIFT};
        }
        setSFCDrift_ModeTelltaleDriftModeStat(std::get<0>(telltaleStat));
        setSFCDrift_ModeTelltaleDriftModeStatOptional(std::get<1>(telltaleStat));
    }

    void updateEvent() {
        std::string eventID;
        std::string soundID{"SND_PopUpInform1"};

        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            switch (mDisplayReqStatus) {
                case DisplayReqStatus::ACTIVE:
                    eventID = "E31401";
                    break;
                case DisplayReqStatus::INACTIVE:
                    eventID = "E31402";
                    break;
                case DisplayReqStatus::CONDITION_NOT_MET:
                    eventID = "E31403";
                    soundID = "SND_PopUpWarn1";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Drift_Mode.Event.DriftMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            DDebug() << "SFCDrift_ModeEventDriftModeStat::OFF";
            setSFCDrift_ModeEventDriftModeStat(SFCDrift_ModeEventDriftModeStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCDrift_ModeEventDriftModeID(eventID);
            setSFCDrift_ModeEventDriftModeStat(SFCDrift_ModeEventDriftModeStat::ON);
            setSFCDrift_ModeEventDriftModeLinkedSoundID(soundID);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using IndiReqStatus = ArgumentsTelltaleSignalChanged::Input_DriftModeIndiReqStatus;
    using DisplayReqStatus = ArgumentsEventValueChanged::Input_DriftModeDisplayReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    DisplayReqStatus mDisplayReqStatus = DisplayReqStatus::OFF;
    IndiReqStatus mIndiReqStatus = IndiReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Drift_Mode_H
