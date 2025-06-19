/**
 * @file Inter_Diff_Lock.hpp
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
#ifndef SFSS_Inter_Diff_Lock_H
#define SFSS_Inter_Diff_Lock_H

#define DLOG_ENABLED gEnableSFCLog

#include "Inter_Diff_LockBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.08
// Reference : [CV220] Inter_Diff_Lock. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Inter_Diff_Lock : public Inter_Diff_LockBase {
public:
    Inter_Diff_Lock() = default;
    ~Inter_Diff_Lock() override = default;
    Inter_Diff_Lock(const Inter_Diff_Lock& other) = delete;
    Inter_Diff_Lock(Inter_Diff_Lock&& other) noexcept = delete;
    Inter_Diff_Lock& operator=(const Inter_Diff_Lock& other) = delete;
    Inter_Diff_Lock& operator=(Inter_Diff_Lock&& other) noexcept = delete;

    void onInitialize() override {
        setSFCInter_Diff_LockEventLockInhibitLinkedSoundID("SND_PopUpWarn1");
        setSFCInter_Diff_LockEventLockInhibitLinkedSoundType(SFCInter_Diff_LockEventLockInhibitLinkedSoundType::REPEAT_COUNT);
        setSFCInter_Diff_LockEventLockInhibitLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnStatus = true;
        updateTelltale();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnStatus = false;
        updateTelltale();
        updateEvent();
    }

    void onInterDiffLockStatusChanged(const ArgumentsInterDiffLockStatusChanged& args) {
        mInterDiffLockStatus = args.mInput_InterDiffLockStatus;
        updateTelltale();
    }

    void onInterDiffLockInhibitStatusChanged(const ArgumentsInterDiffLockInhibitStatusChanged& args) {
        mInterDiffLockInhibitStatus = args.mInput_InterDiffLockInhibitStatus;
        updateEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltale();
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        SFCInter_Diff_LockTelltaleInter_Diff_LockStat telltaleStatus{SFCInter_Diff_LockTelltaleInter_Diff_LockStat::OFF};

        if (mInterDiffLockStatus == InterDiffLockStatus::ON && mIgnStatus == true) {
            telltaleStatus = SFCInter_Diff_LockTelltaleInter_Diff_LockStat::ON;
        }
        setSFCInter_Diff_LockTelltaleInter_Diff_LockStat(telltaleStatus);
    }

    void updateEvent() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        std::string eventID;
        std::string prevEventID;

        if (mIgnStatus == true) {
            if (mInterDiffLockInhibitStatus == InterDiffLockInhibitStatus::INHIBIT_1) {
                eventID = "E72201";
            } else if (mInterDiffLockInhibitStatus == InterDiffLockInhibitStatus::INHIBIT_2) {
                eventID = "E72202";
            } else {
                // No Operation
            }
        }

        GETCACHEDVALUE(SFC.Inter_Diff_Lock.Event.LockInhibit.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCInter_Diff_LockEventLockInhibitStat(SFCInter_Diff_LockEventLockInhibitStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCInter_Diff_LockEventLockInhibitID(eventID);
            setSFCInter_Diff_LockEventLockInhibitStat(SFCInter_Diff_LockEventLockInhibitStat::ON);
        }
    }

    using InterDiffLockStatus = ArgumentsInterDiffLockStatusChanged::Input_InterDiffLockStatus;
    using InterDiffLockInhibitStatus = ArgumentsInterDiffLockInhibitStatusChanged::Input_InterDiffLockInhibitStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    HBool mIgnStatus = false;
    InterDiffLockStatus mInterDiffLockStatus = InterDiffLockStatus::OFF;
    InterDiffLockInhibitStatus mInterDiffLockInhibitStatus = InterDiffLockInhibitStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
};

}  // namespace ccos

#endif  // SFSS_Inter_Diff_Lock_H
