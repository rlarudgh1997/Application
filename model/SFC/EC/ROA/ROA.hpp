/**
 * @file ROA.hpp
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
#ifndef SFSS_ROA_H
#define SFSS_ROA_H

#define DLOG_ENABLED gEnableSFCLog

#include "ROABase.hpp"

namespace ccos {

// SFC Version : 7.0.0
// Reference : [EC140] ROA. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class ROA : public ROABase {
public:
    ROA() = default;
    ~ROA() override = default;
    ROA(const ROA& other) = delete;
    ROA(ROA&& other) noexcept = delete;
    ROA& operator=(const ROA& other) = delete;
    ROA& operator=(ROA&& other) noexcept = delete;

    void onInitialize() override {
        // E61201, E61202, E61205, E61211, E61212, E61215
        setSFCROAEventWarnStatusLinkedSoundType(SFCROAEventWarnStatusLinkedSoundType::REPEAT_COUNT);
        setSFCROAEventWarnStatusLinkedSoundRepeatCount(1);

        setSFCROAEventFunctionStatusID("E61203");
        setSFCROAEventFunctionStatusLinkedSoundID("SND_PopUpInform2");
        setSFCROAEventFunctionStatusLinkedSoundType(SFCROAEventFunctionStatusLinkedSoundType::REPEAT_COUNT);
        setSFCROAEventFunctionStatusLinkedSoundRepeatCount(1);

        setSFCROAEventSysStatusID("E61204");
        setSFCROAEventSysStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCROAEventSysStatusLinkedSoundType(SFCROAEventSysStatusLinkedSoundType::REPEAT_COUNT);
        setSFCROAEventSysStatusLinkedSoundRepeatCount(1);

        setSFCROAEventDelayWarnReqID("E61206");
        setSFCROAEventDelayWarnReqLinkedSoundID("SND_PopUpInform2");
        setSFCROAEventDelayWarnReqLinkedSoundType(SFCROAEventDelayWarnReqLinkedSoundType::REPEAT_COUNT);
        setSFCROAEventDelayWarnReqLinkedSoundRepeatCount(1);

        // E61207, E61208
        setSFCROAEventDelayOffWarnReqLinkedSoundType(SFCROAEventDelayOffWarnReqLinkedSoundType::REPEAT_COUNT);
        setSFCROAEventDelayOffWarnReqLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn0ms) == true) {
            mIgnElapsed = IgnElapsed::ON_0ms;
            updateAll();
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff0ms) == true) {
            mIgnElapsed = IgnElapsed::OFF_0ms;
            updateAll();
        }
    }

    void onIgnElapseOff700msChanged(const ArgumentsIgnElapseOff700msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff700ms) == true) {
            mIgnElapsed = IgnElapsed::OFF_700ms;
            updateAll();
        }
    }

    void onWarnStatusChanged(const ArgumentsWarnStatusChanged& args) {
        mRearSeatRemindWarnStatusFromBCM = args.mInput_RearSeatRemindWarnStatusFromBCM;
        mRearSeatRemindWarnStatusFromBCM_New = args.mInput_RearSeatRemindWarnStatusFromBCM_New;
        updateEventRearSeatWarning();
    }

    void onFunctionStatusChanged(const ArgumentsFunctionStatusChanged& args) {
        mRearSeatRemindFunctionNewStatusFromBCM = args.mInput_RearSeatRemindFunctionStatusFromBCM;
        updateAlarmOff();
    }

    void onSysFailStatusChanged(const ArgumentsSysFailStatusChanged& args) {
        mRearSeatRemindSysFailStatusFromBCM = args.mInput_RearSeatRemindSysFailStatusFromBCM;
        updateSystemfail();
    }

    void onDelayOffWarnReqStatusChanged(const ArgumentsDelayOffWarnReqStatusChanged& args) {
        mRearSeatRemindDelayOffWarnReqStatus = args.mInput_RearSeatRemindDelayOffWarnReqStatus;
        updateEventDelayOffWarnReq();
    }

    void onDelayWarnReqStatusChanged(const ArgumentsDelayWarnReqStatusChanged& args) {
        mRearSeatRemindDelayWarnReqStatus = args.mInput_RearSeatRemindDelayWarnReqStatus;
        updateEventDelayWarnReq();
    }

    void onOffIndiReqStatusChanged(const ArgumentsOffIndiReqStatusChanged& args) {
        mRearSeatRemindOffIndiReqStatus = args.mInput_RearSeatRemindOffIndiReqStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateEventRearSeatWarning();
        updateAlarmOff();
        updateSystemfail();
        updateEventDelayWarnReq();
        updateEventDelayOffWarnReq();
        updateTelltale();
    }

    void updateEventRearSeatWarning() {
        std::string eventID;
        std::string soundID;
        if (mIgnElapsed == IgnElapsed::OFF_700ms || mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mRearSeatRemindWarnStatusFromBCM_New == RearSeatRemindWarnStatusFromBCM_New::OLD) {
                if (mRearSeatRemindWarnStatusFromBCM == RearSeatRemindWarnStatusFromBCM::WARN1) {
                    eventID = "E61201";
                    soundID = "SND_PopUpWarn1";
                } else if (mRearSeatRemindWarnStatusFromBCM == RearSeatRemindWarnStatusFromBCM::WARN2) {
                    eventID = "E61202";
                    soundID = "SND_PopUpWarn1";
                } else if (mRearSeatRemindWarnStatusFromBCM == RearSeatRemindWarnStatusFromBCM::WARN3) {
                    eventID = "E61205";
                    soundID = "SND_PopUpWarn1";
                } else {
                    // no operation
                }
            } else if (mRearSeatRemindWarnStatusFromBCM_New == RearSeatRemindWarnStatusFromBCM_New::WARN1) {
                eventID = "E61211";
                soundID = "SND_PopUpInform2";
            } else if (mRearSeatRemindWarnStatusFromBCM_New == RearSeatRemindWarnStatusFromBCM_New::WARN2) {
                eventID = "E61212";
                soundID = "SND_PopUpInform2";
            } else if (mRearSeatRemindWarnStatusFromBCM_New == RearSeatRemindWarnStatusFromBCM_New::WARN3) {
                eventID = "E61215";
                soundID = "SND_PopUpInform2";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ROA.Event.WarnStatus.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCROAEventWarnStatusStat(SFCROAEventWarnStatusStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCROAEventWarnStatusID(eventID);
            setSFCROAEventWarnStatusStat(SFCROAEventWarnStatusStat::ON);
            setSFCROAEventWarnStatusLinkedSoundID(soundID);
        }
    }

    void updateEventDelayWarnReq() {
        if (mRearSeatRemindDelayWarnReqStatus == RearSeatRemindDelayWarnReqStatus::ON &&
            (mIgnElapsed == IgnElapsed::OFF_700ms || mIgnElapsed == IgnElapsed::ON_0ms)) {
            setSFCROAEventDelayWarnReqStat(SFCROAEventDelayWarnReqStat::ON);
        } else {
            setSFCROAEventDelayWarnReqStat(SFCROAEventDelayWarnReqStat::OFF);
        }
    }

    void updateEventDelayOffWarnReq() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            mRearSeatRemindDelayOffWarnReqStatus == RearSeatRemindDelayOffWarnReqStatus::WARN1) {
            eventID = "E61207";
            linkedSoundID = "SND_PopUpInform1";
        } else if ((mIgnElapsed == IgnElapsed::OFF_700ms || mIgnElapsed == IgnElapsed::ON_0ms) &&
                   mRearSeatRemindDelayOffWarnReqStatus == RearSeatRemindDelayOffWarnReqStatus::WARN2) {
            eventID = "E61208";
            linkedSoundID = "SND_PopUpInform2";
        } else {
            // do nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ROA.Event.DelayOffWarnReq.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCROAEventDelayOffWarnReqStat(SFCROAEventDelayOffWarnReqStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCROAEventDelayOffWarnReqID(eventID);
            setSFCROAEventDelayOffWarnReqStat(SFCROAEventDelayOffWarnReqStat::ON);
            setSFCROAEventDelayOffWarnReqLinkedSoundID(linkedSoundID);
        }
    }

    void updateAlarmOff() {
        if ((mRearSeatRemindFunctionCurrentStatusFromBCM == RearSeatRemindFunctionStatusFromBCM::ON &&
             mRearSeatRemindFunctionNewStatusFromBCM == RearSeatRemindFunctionStatusFromBCM::OFF) &&
            (mIgnElapsed == IgnElapsed::OFF_700ms || mIgnElapsed == IgnElapsed::ON_0ms)) {
            setSFCROAEventFunctionStatusStat(SFCROAEventFunctionStatusStat::ON);
        } else {
            setSFCROAEventFunctionStatusStat(SFCROAEventFunctionStatusStat::OFF);
        }

        mRearSeatRemindFunctionCurrentStatusFromBCM = mRearSeatRemindFunctionNewStatusFromBCM;
    }

    void updateSystemfail() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            mRearSeatRemindSysFailStatusFromBCM == RearSeatRemindSysFailStatusFromBCM::SYS_FAIL) {
            setSFCROAEventSysStatusStat(SFCROAEventSysStatusStat::ON);
        } else {
            setSFCROAEventSysStatusStat(SFCROAEventSysStatusStat::OFF);
        }
    }

    void updateTelltale() {
        if (mRearSeatRemindOffIndiReqStatus == RearSeatRemindOffIndiReqStatus::ON) {
            setSFCROATelltaleRoaOffStat(SFCROATelltaleRoaOffStat::ON);
        } else {
            setSFCROATelltaleRoaOffStat(SFCROATelltaleRoaOffStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        OFF_700ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using RearSeatRemindWarnStatusFromBCM = ArgumentsWarnStatusChanged::Input_RearSeatRemindWarnStatusFromBCM;
    RearSeatRemindWarnStatusFromBCM mRearSeatRemindWarnStatusFromBCM = RearSeatRemindWarnStatusFromBCM::OFF;
    using RearSeatRemindWarnStatusFromBCM_New = ArgumentsWarnStatusChanged::Input_RearSeatRemindWarnStatusFromBCM_New;
    RearSeatRemindWarnStatusFromBCM_New mRearSeatRemindWarnStatusFromBCM_New = RearSeatRemindWarnStatusFromBCM_New::OLD;
    using RearSeatRemindFunctionStatusFromBCM = ArgumentsFunctionStatusChanged::Input_RearSeatRemindFunctionStatusFromBCM;
    RearSeatRemindFunctionStatusFromBCM mRearSeatRemindFunctionCurrentStatusFromBCM =
        RearSeatRemindFunctionStatusFromBCM::INVALID;
    RearSeatRemindFunctionStatusFromBCM mRearSeatRemindFunctionNewStatusFromBCM = RearSeatRemindFunctionStatusFromBCM::INVALID;
    using RearSeatRemindSysFailStatusFromBCM = ArgumentsSysFailStatusChanged::Input_RearSeatRemindSysFailStatusFromBCM;
    RearSeatRemindSysFailStatusFromBCM mRearSeatRemindSysFailStatusFromBCM = RearSeatRemindSysFailStatusFromBCM::NORMAL;

    using RearSeatRemindDelayOffWarnReqStatus = ArgumentsDelayOffWarnReqStatusChanged::Input_RearSeatRemindDelayOffWarnReqStatus;
    RearSeatRemindDelayOffWarnReqStatus mRearSeatRemindDelayOffWarnReqStatus = RearSeatRemindDelayOffWarnReqStatus::OFF;

    using RearSeatRemindDelayWarnReqStatus = ArgumentsDelayWarnReqStatusChanged::Input_RearSeatRemindDelayWarnReqStatus;
    RearSeatRemindDelayWarnReqStatus mRearSeatRemindDelayWarnReqStatus = RearSeatRemindDelayWarnReqStatus::OFF;

    using RearSeatRemindOffIndiReqStatus = ArgumentsOffIndiReqStatusChanged::Input_RearSeatRemindOffIndiReqStatus;
    RearSeatRemindOffIndiReqStatus mRearSeatRemindOffIndiReqStatus = RearSeatRemindOffIndiReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_ROA_H
