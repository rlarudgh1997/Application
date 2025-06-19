/**
 * @file AVH.hpp
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
#ifndef SFSS_AVH_H
#define SFSS_AVH_H

#define DLOG_ENABLED gEnableSFCLog

#include "AVHBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.34
// Reference : [CV030] AVH. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class AVH : public AVHBase {
public:
    AVH() = default;
    ~AVH() override = default;
    AVH(const AVH& other) = delete;
    AVH(AVH&& other) noexcept = delete;
    AVH& operator=(const AVH& other) = delete;
    AVH& operator=(AVH&& other) noexcept = delete;

    void onInitialize() override {
        // AVH 작동 정보 알림.
        setSFCAVHEventAutoHoldInfoLinkedSoundID("SND_PopUpInform2");
        setSFCAVHEventAutoHoldInfoLinkedSoundType(SFCAVHEventAutoHoldInfoLinkedSoundType::REPEAT_COUNT);
        setSFCAVHEventAutoHoldInfoLinkedSoundRepeatCount(1);

        // 고구배 sEHS 해제 불가
        setSFCAVHEventEHSPopUpID("E70304");
        setSFCAVHEventEHSPopUpLinkedSoundID("SND_PopUpInform2");
        setSFCAVHEventEHSPopUpLinkedSoundType(SFCAVHEventEHSPopUpLinkedSoundType::REPEAT_COUNT);
        setSFCAVHEventEHSPopUpLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleAutoHoldStat();
        updateEventAutoHoldInfo();
        updateEventEHSPopUp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleAutoHoldStat();
        updateEventAutoHoldInfo();
        updateEventEHSPopUp();
    }

    void onAvhStatusChanged(const ArgumentsAvhStatusChanged& args) {
        mAVHStatus = args.mInput_AVHStatus;
        updateTelltaleAutoHoldStat();
    }

    void onAvhInfoChanged(const ArgumentsAvhInfoChanged& args) {
        mAVHInfo = args.mInput_AVHInfo;
        updateEventAutoHoldInfo();
    }

    void onEhsPopUpChanged(const ArgumentsEhsPopUpChanged& args) {
        mEHSPopUp = args.mInput_EHSPopUp;
        updateEventEHSPopUp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleAutoHoldStat() {
        SFCAVHTelltaleAUTOHOLDStat stat = SFCAVHTelltaleAUTOHOLDStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mAVHStatus == AVHStatus::READY) {
                stat = SFCAVHTelltaleAUTOHOLDStat::WHITE;
            } else if (mAVHStatus == AVHStatus::ACTIVE) {
                stat = SFCAVHTelltaleAUTOHOLDStat::GREEN;
            } else if (mAVHStatus == AVHStatus::WARN) {
                stat = SFCAVHTelltaleAUTOHOLDStat::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCAVHTelltaleAUTOHOLDStat(stat);
    }

    void updateEventAutoHoldInfo() {
        std::string eventID;
        std::string prevEventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mAVHInfo == AVHInfo::RELEASE) {
                eventID = "E70301";
            } else if (mAVHInfo == AVHInfo::INHIBIT_AVH) {
                eventID = "E70302";
            } else if (mAVHInfo == AVHInfo::AUTO_PARKING) {
                eventID = "E70303";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.AVH.Event.AutoHoldInfo.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCAVHEventAutoHoldInfoStat(SFCAVHEventAutoHoldInfoStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCAVHEventAutoHoldInfoID(eventID);
            setSFCAVHEventAutoHoldInfoStat(SFCAVHEventAutoHoldInfoStat::ON);
        }
    }

    void updateEventEHSPopUp() {
        SFCAVHEventEHSPopUpStat stat = SFCAVHEventEHSPopUpStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mEHSPopUp == EHSPopUp::INHIBIT_RELEASE) {
            stat = SFCAVHEventEHSPopUpStat::ON;
        }
        setSFCAVHEventEHSPopUpStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using AVHStatus = ArgumentsAvhStatusChanged::Input_AVHStatus;
    using AVHInfo = ArgumentsAvhInfoChanged::Input_AVHInfo;
    using EHSPopUp = ArgumentsEhsPopUpChanged::Input_EHSPopUp;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    AVHStatus mAVHStatus = AVHStatus::OFF;
    AVHInfo mAVHInfo = AVHInfo::OFF;
    EHSPopUp mEHSPopUp = EHSPopUp::OFF;
};

}  // namespace ccos

#endif  // SFSS_AVH_H
