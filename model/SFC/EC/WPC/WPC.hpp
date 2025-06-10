/**
 * @file WPC.hpp
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
#ifndef SFSS_WPC_H
#define SFSS_WPC_H

#define DLOG_ENABLED gEnableSFCLog

#include "WPCBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 4.0.1
// Reference : [EC170] WPC. Contains : Event. Recommend: VALUE_CHANGED
class WPC : public WPCBase {
public:
    WPC() = default;
    ~WPC() override = default;

    void onInitialize() override {
        if (mOneShotTimerDoorOpenFront700msStatus.create(kTimerEventInterval700ms, this,
                                                         &WPC::onOneShotTimerDoorOpenFront700ms) == false) {
            DWarning() << "[WPC] mOneShotTimerDoorOpenFront700msStatus Create Failed";
        }
        setSFCWPCEventDispWarningID("E61501");
        setSFCWPCEventDispWarningLinkedSoundID("SND_PopUpInform2");
        setSFCWPCEventDispWarningLinkedSoundType(SFCWPCEventDispWarningLinkedSoundType::REPEAT_COUNT);
        setSFCWPCEventDispWarningLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onIgnElapseOff700msChanged(const ArgumentsIgnElapseOff700msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_700ms;
        updateEvent();
    }

    void onDoorStatusChanged(const ArgumentsDoorStatusChanged& args) {
        mDoorOpenDriverSideStatus = args.mInput_DoorOpenDriverSideStatus;
        mDoorOpenAssistSideStatus = args.mInput_DoorOpenAssistSideStatus;
        mDoorOpenDriverSideStatusFromSBCM = args.mInput_DoorOpenDriverSideStatusFromSBCM;
        mDoorOpenAssistSideStatusFromSBCM = args.mInput_DoorOpenAssistSideStatusFromSBCM;
        updatePrivateDoorOpenFrontStatus();
    }

    void onCellphoneReminderWarnStatus(const ArgumentsCellphoneReminderWarnStatus& args) {
        mCellphoneFrontReminderWarnStatus = args.mInput_CellphoneFrontReminderWarnStatus;
        mCellphoneRearReminderWarnStatus = args.mInput_CellphoneRearReminderWarnStatus;
        mCellphoneDuelFrontReminderWarnStatus = args.mInput_CellphoneDuelFrontReminderWarnStatus;
        updateEvent();
    }

private:
    void onOneShotTimerDoorOpenFront700ms() {
        if (mPrivate_DoorOpenFrontStatus == Private_DoorOpenFrontStatus::OPEN) {
            mDoorOpenFront700msDone = true;
        } else {
            mDoorOpenFront700msDone = false;
        }
        updateEvent();
        flush();
    }

    void updateEvent() {
        SFCWPCEventDispWarningStat stat = SFCWPCEventDispWarningStat::OFF;
        if (mCellphoneFrontReminderWarnStatus == CellphoneFrontReminderWarnStatus::WARN2 ||
            mCellphoneRearReminderWarnStatus == CellphoneRearReminderWarnStatus::WARN2 ||
            mCellphoneDuelFrontReminderWarnStatus == CellphoneDuelFrontReminderWarnStatus::WARN2) {
            stat = SFCWPCEventDispWarningStat::ON;
        } else if (mIgnElapsed == IgnElapsed::OFF_700ms) {
            if (mDoorOpenFront700msDone &&
                (mCellphoneFrontReminderWarnStatus == CellphoneFrontReminderWarnStatus::WARN1 ||
                 mCellphoneRearReminderWarnStatus == CellphoneRearReminderWarnStatus::WARN1 ||
                 mCellphoneDuelFrontReminderWarnStatus == CellphoneDuelFrontReminderWarnStatus::WARN1)) {
                stat = SFCWPCEventDispWarningStat::ON;
            }
        } else {
            // no operation
        }
        setSFCWPCEventDispWarningStat(stat);
    }

    void updatePrivateDoorOpenFrontStatus() {
        mDoorOpenFront700msDone = false;

        if (mDoorOpenDriverSideStatus == DoorOpenDriverSideStatus::OPEN ||
            mDoorOpenDriverSideStatusFromSBCM == DoorOpenDriverSideStatusFromSBCM::OPEN ||
            mDoorOpenAssistSideStatus == DoorOpenAssistSideStatus::OPEN ||
            mDoorOpenAssistSideStatusFromSBCM == DoorOpenAssistSideStatusFromSBCM::OPEN) {
            mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::OPEN;
            if (mOneShotTimerDoorOpenFront700msStatus.start() == false) {
                DWarning() << "[WPC] mOneShotTimerDoorOpenFront700msStatus Start Failed";
            }
        } else {
            mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::CLOSE;
            mOneShotTimerDoorOpenFront700msStatus.stop();
            updateEvent();
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        OFF_700ms,
        MAX
    };

    enum class Private_DoorOpenFrontStatus : HUInt64 {
        CLOSE = 0x0,
        OPEN = 0x1
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using DoorOpenDriverSideStatus = ArgumentsDoorStatusChanged::Input_DoorOpenDriverSideStatus;
    DoorOpenDriverSideStatus mDoorOpenDriverSideStatus = DoorOpenDriverSideStatus::CLOSE;
    using DoorOpenAssistSideStatus = ArgumentsDoorStatusChanged::Input_DoorOpenAssistSideStatus;
    DoorOpenAssistSideStatus mDoorOpenAssistSideStatus = DoorOpenAssistSideStatus::CLOSE;
    using DoorOpenDriverSideStatusFromSBCM = ArgumentsDoorStatusChanged::Input_DoorOpenDriverSideStatusFromSBCM;
    DoorOpenDriverSideStatusFromSBCM mDoorOpenDriverSideStatusFromSBCM = DoorOpenDriverSideStatusFromSBCM::CLOSE;
    using DoorOpenAssistSideStatusFromSBCM = ArgumentsDoorStatusChanged::Input_DoorOpenAssistSideStatusFromSBCM;
    DoorOpenAssistSideStatusFromSBCM mDoorOpenAssistSideStatusFromSBCM = DoorOpenAssistSideStatusFromSBCM::CLOSE;
    using CellphoneFrontReminderWarnStatus = ArgumentsCellphoneReminderWarnStatus::Input_CellphoneFrontReminderWarnStatus;
    CellphoneFrontReminderWarnStatus mCellphoneFrontReminderWarnStatus = CellphoneFrontReminderWarnStatus::OFF;
    using CellphoneRearReminderWarnStatus = ArgumentsCellphoneReminderWarnStatus::Input_CellphoneRearReminderWarnStatus;
    CellphoneRearReminderWarnStatus mCellphoneRearReminderWarnStatus = CellphoneRearReminderWarnStatus::OFF;
    using CellphoneDuelFrontReminderWarnStatus = ArgumentsCellphoneReminderWarnStatus::Input_CellphoneDuelFrontReminderWarnStatus;
    CellphoneDuelFrontReminderWarnStatus mCellphoneDuelFrontReminderWarnStatus = CellphoneDuelFrontReminderWarnStatus::OFF;
    Private_DoorOpenFrontStatus mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::CLOSE;
    static constexpr uint32_t kTimerEventInterval700ms = 700u;
    bool mDoorOpenFront700msDone = false;
    ssfs::SFCTimer<WPC> mOneShotTimerDoorOpenFront700msStatus;
};

}  // namespace ccos

#endif  // SFSS_WPC_H
