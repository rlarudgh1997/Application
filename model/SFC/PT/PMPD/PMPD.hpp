/**
 * @file PMPD.hpp
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
#ifndef SFSS_PMPD_H
#define SFSS_PMPD_H

#define DLOG_ENABLED gEnableSFCLog

#include "PMPDBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [PT520] PMPD. Contains : Constant, Event. Recommend: VALUE_CHANGED
class PMPD : public PMPDBase {
public:
    PMPD() = default;
    ~PMPD() override = default;
    PMPD(const PMPD& other) = delete;
    PMPD(PMPD&& other) noexcept = delete;
    PMPD& operator=(const PMPD& other) = delete;
    PMPD& operator=(PMPD&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstant();
        updatePMPDEvent();
        updatePMSAActiveEvent();
        updatePMSAEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstant();
        updatePMPDEvent();
        updatePMSAActiveEvent();
        updatePMSAEvent();
    }

    void onPmpdActChanged(const ArgumentsPmpdActChanged& args) {
        mPmpdActStatus = args.mInput_PmpdActStatus;
        updatePMPDEvent();
    }

    void onPmsaActivateWaitTimeValChanged(const ArgumentsPmsaActivateWaitTimeValChanged& args) {
        mPmsaActivateWaitTimeVal = args.mInput_PmsaActivateWaitTimeVal;
        updateConstant();
        updatePMSAEvent();
    }

    void onPmsaWarnStatusChanged(const ArgumentsPmsaWarnStatusChanged& args) {
        mPmsaWarnStatus = args.mInput_PmsaWarnStatus;
        updatePMSAEvent();
    }

    void onPmpdActiveStatusChanged(const ArgumentsPmpdActiveStatusChanged& args) {
        mPmpdActiveStatus = args.mInput_PmpdActiveStatus;
        updatePMSAActiveEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        HUInt64 value = 0;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mPmsaActivateWaitTimeVal >= 1 && mPmsaActivateWaitTimeVal <= 15) {
            value = mPmsaActivateWaitTimeVal;
        }
        setSFCPMPDConstantPmsaWaitTimerValue(value);
    }

    void updatePMPDEvent() {
        std::string prevEventID;
        std::string eventID;
        SFCPMPDEventPMPDStat stat = SFCPMPDEventPMPDStat::OFF;
        std::string linkedSoundID;
        SFCPMPDEventPMPDLinkedSoundType linkedSoundType = SFCPMPDEventPMPDLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPmpdActStatus == PmpdActStatus::TORQUE_LIMIT || mPmpdActStatus == PmpdActStatus::BRAKE_ACT) {
                eventID = "E25401";
                stat = SFCPMPDEventPMPDStat::ON;
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCPMPDEventPMPDLinkedSoundType::INFINITE;
            } else if ((mPrevPmpdActStatus == PmpdActStatus::TORQUE_LIMIT || mPrevPmpdActStatus == PmpdActStatus::BRAKE_ACT) &&
                       mPmpdActStatus == PmpdActStatus::OFF) {
                eventID = "E25402";
                stat = SFCPMPDEventPMPDStat::ON;
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCPMPDEventPMPDLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.PMPD.Event.PMPD.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCPMPDEventPMPDStat(SFCPMPDEventPMPDStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCPMPDEventPMPDID(eventID);
            setSFCPMPDEventPMPDStat(stat);
            setSFCPMPDEventPMPDLinkedSoundID(linkedSoundID);
            setSFCPMPDEventPMPDLinkedSoundType(linkedSoundType);
            setSFCPMPDEventPMPDLinkedSoundRepeatCount(repeatCount);
        }
        mPrevPmpdActStatus = mPmpdActStatus;
    }

    void updatePMSAActiveEvent() {
        std::string prevEventID;
        std::string eventID;
        SFCPMPDEventPMSAActiveStat stat = SFCPMPDEventPMSAActiveStat::OFF;
        std::string linkedSoundID;
        SFCPMPDEventPMSAActiveLinkedSoundType linkedSoundType = SFCPMPDEventPMSAActiveLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPmpdActiveStatus == PmpdActiveStatus::DEACTIVATE) {
                eventID = "E25406";
                stat = SFCPMPDEventPMSAActiveStat::ON;
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCPMPDEventPMSAActiveLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mPmpdActiveStatus == PmpdActiveStatus::FAIL) {
                eventID = "E25407";
                stat = SFCPMPDEventPMSAActiveStat::ON;
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCPMPDEventPMSAActiveLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mPmpdActiveStatus == PmpdActiveStatus::ACTIVATE) {
                eventID = "E25408";
                stat = SFCPMPDEventPMSAActiveStat::ON;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.PMPD.Event.PMSAActive.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCPMPDEventPMSAActiveStat(SFCPMPDEventPMSAActiveStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCPMPDEventPMSAActiveID(eventID);
            setSFCPMPDEventPMSAActiveStat(stat);
            setSFCPMPDEventPMSAActiveLinkedSoundID(linkedSoundID);
            setSFCPMPDEventPMSAActiveLinkedSoundType(linkedSoundType);
            setSFCPMPDEventPMSAActiveLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updatePMSAEvent() {
        std::string prevEventID;
        std::string eventID;
        SFCPMPDEventPMSAStat stat = SFCPMPDEventPMSAStat::OFF;
        std::string linkedSoundID;
        SFCPMPDEventPMSALinkedSoundType linkedSoundType = SFCPMPDEventPMSALinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPmsaWarnStatus == PmsaWarnStatus::READY && mPmsaActivateWaitTimeVal >= 1 && mPmsaActivateWaitTimeVal <= 15) {
                eventID = "E25403";
                stat = SFCPMPDEventPMSAStat::ON;
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCPMPDEventPMSALinkedSoundType::INFINITE;
            } else if (mPmsaWarnStatus == PmsaWarnStatus::ACTIVATE) {
                eventID = "E25404";
                stat = SFCPMPDEventPMSAStat::ON;
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCPMPDEventPMSALinkedSoundType::INFINITE;
            } else if (mPmsaWarnStatus == PmsaWarnStatus::RELEASE) {
                eventID = "E25405";
                stat = SFCPMPDEventPMSAStat::ON;
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCPMPDEventPMSALinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.PMPD.Event.PMSA.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCPMPDEventPMSAStat(SFCPMPDEventPMSAStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCPMPDEventPMSAID(eventID);
            setSFCPMPDEventPMSAStat(stat);
            setSFCPMPDEventPMSALinkedSoundID(linkedSoundID);
            setSFCPMPDEventPMSALinkedSoundType(linkedSoundType);
            setSFCPMPDEventPMSALinkedSoundRepeatCount(repeatCount);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using PmpdActStatus = ArgumentsPmpdActChanged::Input_PmpdActStatus;
    using PmsaWarnStatus = ArgumentsPmsaWarnStatusChanged::Input_PmsaWarnStatus;
    using PmpdActiveStatus = ArgumentsPmpdActiveStatusChanged::Input_PmpdActiveStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    PmpdActStatus mPmpdActStatus = PmpdActStatus::OFF;
    PmpdActStatus mPrevPmpdActStatus = PmpdActStatus::OFF;
    HUInt64 mPmsaActivateWaitTimeVal = 0;
    PmsaWarnStatus mPmsaWarnStatus = PmsaWarnStatus::OFF;
    PmpdActiveStatus mPmpdActiveStatus = PmpdActiveStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_PMPD_H
