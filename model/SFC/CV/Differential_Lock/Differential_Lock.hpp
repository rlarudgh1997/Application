/**
 * @file Differential_Lock.hpp
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
#ifndef SFSS_Differential_Lock_H
#define SFSS_Differential_Lock_H

#define DLOG_ENABLED gEnableSFCLog

#include "Differential_LockBase.hpp"

namespace ccos {

// SFC Version : 6.0.0
// Spec Version : v0.21
// Reference : [CV220] Differential_Lock. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class Differential_Lock : public Differential_LockBase {
public:
    Differential_Lock() = default;
    ~Differential_Lock() override = default;
    Differential_Lock(const Differential_Lock& other) = delete;
    Differential_Lock(Differential_Lock&& other) noexcept = delete;
    Differential_Lock& operator=(const Differential_Lock& other) = delete;
    Differential_Lock& operator=(Differential_Lock&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDifferential_LockEventLockInhibitLinkedSoundType(SFCDifferential_LockEventLockInhibitLinkedSoundType::REPEAT_COUNT);

        setSFCDifferential_LockSoundDiffLockWarningStatusID("SND_DIffLockWarn");
        setSFCDifferential_LockSoundDiffLockWarningStatusType(SFCDifferential_LockSoundDiffLockWarningStatusType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateTelltale();
        updateEvent();
        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateTelltale();
        updateEvent();
        updateSound();
    }

    void onDiffLockStatusChanged(const ArgumentsDiffLockStatusChanged& args) {
        mDiffLockStatus = args.mInput_DiffLockStatus;
        updateTelltale();
    }

    void onDiffLockInhibitStatusChanged(const ArgumentsDiffLockInhibitStatusChanged& args) {
        mDiffLockInhibitStatus = args.mInput_DiffLockInhibitStatus;
        updateEvent();
    }

    void onDiffLockWarningStatusChanged(const ArgumentsDiffLockWarningStatusChanged& args) {
        mDiffLockWarningStatus = args.mInput_DiffLockWarningStatus;
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltale();
        updateEvent();
        updateSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        SFCDifferential_LockTelltaleDiff_LockStat telltaleStatus{SFCDifferential_LockTelltaleDiff_LockStat::OFF};

        if (mDiffLockStatus == DiffLockStatus::ON && mIsIgnOn == true) {
            telltaleStatus = SFCDifferential_LockTelltaleDiff_LockStat::ON;
        }
        setSFCDifferential_LockTelltaleDiff_LockStat(telltaleStatus);
    }

    void updateEvent() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string soundID;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            if (mDiffLockInhibitStatus == DiffLockInhibitStatus::INHIBIT_1) {
                eventID = "E72401";
                soundID = "SND_PopUpWarn1";
                repeatCount = 1;
            } else if (mDiffLockInhibitStatus == DiffLockInhibitStatus::INHIBIT_2) {
                eventID = "E72402";
                soundID = "SND_PopUpWarn2";
                repeatCount = 2;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.Differential_Lock.Event.LockInhibit.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDifferential_LockEventLockInhibitStat(SFCDifferential_LockEventLockInhibitStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCDifferential_LockEventLockInhibitID(eventID);
            setSFCDifferential_LockEventLockInhibitStat(SFCDifferential_LockEventLockInhibitStat::ON);
            setSFCDifferential_LockEventLockInhibitLinkedSoundID(soundID);
            setSFCDifferential_LockEventLockInhibitLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateSound() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }
        SFCDifferential_LockSoundDiffLockWarningStatusStat soundStatus{SFCDifferential_LockSoundDiffLockWarningStatusStat::OFF};

        if (mDiffLockWarningStatus == DiffLockWarningStatus::ON && mIsIgnOn == true) {
            soundStatus = SFCDifferential_LockSoundDiffLockWarningStatusStat::ON;
        }
        setSFCDifferential_LockSoundDiffLockWarningStatusStat(soundStatus);
    }

    HBool mIsIgnOn = false;

    using DiffLockStatus = ArgumentsDiffLockStatusChanged::Input_DiffLockStatus;
    using DiffLockInhibitStatus = ArgumentsDiffLockInhibitStatusChanged::Input_DiffLockInhibitStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;
    using DiffLockWarningStatus = ArgumentsDiffLockWarningStatusChanged::Input_DiffLockWarningStatus;

    DiffLockStatus mDiffLockStatus = DiffLockStatus::OFF;
    DiffLockInhibitStatus mDiffLockInhibitStatus = DiffLockInhibitStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
    DiffLockWarningStatus mDiffLockWarningStatus = DiffLockWarningStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Differential_Lock_H
