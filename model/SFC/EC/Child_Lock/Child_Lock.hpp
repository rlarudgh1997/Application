/**
 * @file Child_Lock.hpp
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
#ifndef SFSS_Child_Lock_H
#define SFSS_Child_Lock_H

#define DLOG_ENABLED gEnableSFCLog

#include <bitset>
#include "Child_LockBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [EC180] Child_Lock Contains : Event. Recommend: VALUE_CHANGED
class Child_Lock : public Child_LockBase {
public:
    Child_Lock() = default;
    ~Child_Lock() override = default;
    Child_Lock(const Child_Lock& other) = delete;
    Child_Lock(Child_Lock&& other) noexcept = delete;
    Child_Lock& operator=(const Child_Lock& other) = delete;
    Child_Lock& operator=(Child_Lock&& other) noexcept = delete;

    void onInitialize() override {
        setSFCChild_LockEventE61701ID("E61701");
        setSFCChild_LockEventE61702ID("E61702");
        setSFCChild_LockEventSeaLockFailID("E61703");
        setSFCChild_LockEventSeaLockFailLinkedSoundID("SND_PopUpWarn1");
        setSFCChild_LockEventSeaLockFailLinkedSoundType(SFCChild_LockEventSeaLockFailLinkedSoundType::REPEAT_COUNT);
        setSFCChild_LockEventSeaLockFailLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onChildLockRearLatchRLPDCStatusChanged(const ArgumentsChildLockRearLatchRLPDCStatusChanged& args) {
        mChildLockRearLeftLatchStatusFromPDC = args.mInput_ChildLockRearLeftLatchStatusFromPDC;

        updateChildLockRearLatchRLPDCStatus();
        updateEvent();
        mPrevChildLockRearLeftLatchStatusFromPDC = mChildLockRearLeftLatchStatusFromPDC;
    }

    void onChildLockRearLatchRRPDCStatusChanged(const ArgumentsChildLockRearLatchRRPDCStatusChanged& args) {
        mChildLockRearRightLatchStatusFromPDC = args.mInput_ChildLockRearRightLatchStatusFromPDC;

        updateChildLockRearLatchRRPDCStatus();
        updateEvent();
        mPrevChildLockRearRightLatchStatusFromPDC = mChildLockRearRightLatchStatusFromPDC;
    }

    void onChildLockRearLatchRLSBCMStatusChanged(const ArgumentsChildLockRearLatchRLSBCMStatusChanged& args) {
        mChildLockRearLeftLatchStatusFromSBCM = args.mInput_ChildLockRearLeftLatchStatusFromSBCM;

        updateChildLockRearLatchRLSBCMStatus();
        updateEvent();
        mPrevChildLockRearLeftLatchStatusFromSBCM = mChildLockRearLeftLatchStatusFromSBCM;
    }

    void onChildLockRearLatchRRSBCMStatusChanged(const ArgumentsChildLockRearLatchRRSBCMStatusChanged& args) {
        mChildLockRearRightLatchStatusFromSBCM = args.mInput_ChildLockRearRightLatchStatusFromSBCM;

        updateChildLockRearLatchRRSBCMStatus();
        updateEvent();
        mPrevChildLockRearRightLatchStatusFromSBCM = mChildLockRearRightLatchStatusFromSBCM;
    }

    void onSeaLockFailWarnStatusChanged(const ArgumentsSeaLockFailWarnStatusChanged& args) {
        mChildLockSeaLockFailStatus = args.mInput_SeaLockFailWarnStatus;
        updateEventSeaLock();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateChildLockRearLatchRLPDCStatus() {
        if (mChildLockRearLeftLatchStatusFromPDC == ChildLockRearLeftLatchStatusFromPDC::LOCK &&
            mPrevChildLockRearLeftLatchStatusFromPDC == ChildLockRearLeftLatchStatusFromPDC::UNLOCK) {
            mChildLockRearLatchRLPDCStatus = LockStatus::UNLOCK_TO_LOCK;
        } else if (mChildLockRearLeftLatchStatusFromPDC == ChildLockRearLeftLatchStatusFromPDC::UNLOCK &&
                   mPrevChildLockRearLeftLatchStatusFromPDC == ChildLockRearLeftLatchStatusFromPDC::LOCK) {
            mChildLockRearLatchRLPDCStatus = LockStatus::LOCK_TO_UNLOCK;
        } else {
            mChildLockRearLatchRLPDCStatus = LockStatus::NONE;
        }
    }

    void updateChildLockRearLatchRRPDCStatus() {
        if (mChildLockRearRightLatchStatusFromPDC == ChildLockRearRightLatchStatusFromPDC::LOCK &&
            mPrevChildLockRearRightLatchStatusFromPDC == ChildLockRearRightLatchStatusFromPDC::UNLOCK) {
            mChildLockRearLatchRRPDCStatus = LockStatus::UNLOCK_TO_LOCK;
        } else if (mChildLockRearRightLatchStatusFromPDC == ChildLockRearRightLatchStatusFromPDC::UNLOCK &&
                   mPrevChildLockRearRightLatchStatusFromPDC == ChildLockRearRightLatchStatusFromPDC::LOCK) {
            mChildLockRearLatchRRPDCStatus = LockStatus::LOCK_TO_UNLOCK;
        } else {
            mChildLockRearLatchRRPDCStatus = LockStatus::NONE;
        }
    }

    void updateChildLockRearLatchRLSBCMStatus() {
        if (mChildLockRearLeftLatchStatusFromSBCM == ChildLockRearLeftLatchStatusFromSBCM::LOCK &&
            mPrevChildLockRearLeftLatchStatusFromSBCM == ChildLockRearLeftLatchStatusFromSBCM::UNLOCK) {
            mChildLockRearLatchRLSBCMStatus = LockStatus::UNLOCK_TO_LOCK;
        } else if (mChildLockRearLeftLatchStatusFromSBCM == ChildLockRearLeftLatchStatusFromSBCM::UNLOCK &&
                   mPrevChildLockRearLeftLatchStatusFromSBCM == ChildLockRearLeftLatchStatusFromSBCM::LOCK) {
            mChildLockRearLatchRLSBCMStatus = LockStatus::LOCK_TO_UNLOCK;
        } else {
            mChildLockRearLatchRLSBCMStatus = LockStatus::NONE;
        }
    }

    void updateChildLockRearLatchRRSBCMStatus() {
        if (mChildLockRearRightLatchStatusFromSBCM == ChildLockRearRightLatchStatusFromSBCM::LOCK &&
            mPrevChildLockRearRightLatchStatusFromSBCM == ChildLockRearRightLatchStatusFromSBCM::UNLOCK) {
            mChildLockRearLatchRRSBCMStatus = LockStatus::UNLOCK_TO_LOCK;
        } else if (mChildLockRearRightLatchStatusFromSBCM == ChildLockRearRightLatchStatusFromSBCM::UNLOCK &&
                   mPrevChildLockRearRightLatchStatusFromSBCM == ChildLockRearRightLatchStatusFromSBCM::LOCK) {
            mChildLockRearLatchRRSBCMStatus = LockStatus::LOCK_TO_UNLOCK;
        } else {
            mChildLockRearLatchRRSBCMStatus = LockStatus::NONE;
        }
    }

    void updateEvent() {
        static std::bitset<2> e61701ECU{};  // PDC, SBCM order
        static std::bitset<2> e61702ECU{};  // PDC, SBCM order
        HBool flushTrue = false;
        SFCChild_LockEventE61701Stat e61701Stat = SFCChild_LockEventE61701Stat::OFF;
        SFCChild_LockEventE61702Stat e61702Stat = SFCChild_LockEventE61702Stat::OFF;

        // E61701
        if (mChildLockRearLatchRLPDCStatus == LockStatus::UNLOCK_TO_LOCK &&
            mChildLockRearLatchRRPDCStatus == LockStatus::UNLOCK_TO_LOCK) {
            e61701Stat = SFCChild_LockEventE61701Stat::ON;
            if (e61701ECU[0] == false) {
                flushTrue = true;
            }
            e61701ECU.set(0, true);
        } else {
            e61701ECU.set(0, false);
        }
        if (mChildLockRearLatchRLSBCMStatus == LockStatus::UNLOCK_TO_LOCK &&
            mChildLockRearLatchRRSBCMStatus == LockStatus::UNLOCK_TO_LOCK) {
            e61701Stat = SFCChild_LockEventE61701Stat::ON;
            if (e61701ECU[1] == false) {
                flushTrue = true;
            }
            e61701ECU.set(1, true);
        } else {
            e61701ECU.set(1, false);
        }
        setSFCChild_LockEventE61701Stat(e61701Stat);
        if (flushTrue == true) {
            flushLastGroup(true);
        }
        if (e61701Stat == SFCChild_LockEventE61701Stat::OFF) {
            e61701ECU.reset();
        }

        flushTrue = false;

        // E61702
        if (mChildLockRearLatchRLPDCStatus == LockStatus::LOCK_TO_UNLOCK &&
            mChildLockRearLatchRRPDCStatus == LockStatus::LOCK_TO_UNLOCK) {
            e61702Stat = SFCChild_LockEventE61702Stat::ON;
            if (e61702ECU[0] == false) {
                flushTrue = true;
            }
            e61702ECU.set(0, true);
        } else {
            e61702ECU.set(0, false);
        }
        if (mChildLockRearLatchRLSBCMStatus == LockStatus::LOCK_TO_UNLOCK &&
            mChildLockRearLatchRRSBCMStatus == LockStatus::LOCK_TO_UNLOCK) {
            e61702Stat = SFCChild_LockEventE61702Stat::ON;
            if (e61702ECU[1] == false) {
                flushTrue = true;
            }
            e61702ECU.set(1, true);
        } else {
            e61702ECU.set(1, false);
        }
        setSFCChild_LockEventE61702Stat(e61702Stat);
        if (flushTrue == true) {
            flushLastGroup(true);
        }
        if (e61702Stat == SFCChild_LockEventE61702Stat::OFF) {
            e61702ECU.reset();
        }
    }

    void updateEventSeaLock() {
        SFCChild_LockEventSeaLockFailStat stat = SFCChild_LockEventSeaLockFailStat::OFF;

        if (mChildLockSeaLockFailStatus == ChildLockSeaLockFailStatus::ABNORMAL) {
            stat = SFCChild_LockEventSeaLockFailStat::ON;
        }
        setSFCChild_LockEventSeaLockFailStat(stat);
    }

    enum class LockStatus : HUInt64 {
        NONE,
        UNLOCK_TO_LOCK,
        LOCK_TO_UNLOCK,
        MAX
    };

    using ChildLockRearLeftLatchStatusFromPDC =
        ArgumentsChildLockRearLatchRLPDCStatusChanged::Input_ChildLockRearLeftLatchStatusFromPDC;
    using ChildLockRearRightLatchStatusFromPDC =
        ArgumentsChildLockRearLatchRRPDCStatusChanged::Input_ChildLockRearRightLatchStatusFromPDC;
    using ChildLockRearLeftLatchStatusFromSBCM =
        ArgumentsChildLockRearLatchRLSBCMStatusChanged::Input_ChildLockRearLeftLatchStatusFromSBCM;
    using ChildLockRearRightLatchStatusFromSBCM =
        ArgumentsChildLockRearLatchRRSBCMStatusChanged::Input_ChildLockRearRightLatchStatusFromSBCM;
    using ChildLockSeaLockFailStatus = ArgumentsSeaLockFailWarnStatusChanged::Input_SeaLockFailWarnStatus;

    ChildLockRearLeftLatchStatusFromPDC mChildLockRearLeftLatchStatusFromPDC = ChildLockRearLeftLatchStatusFromPDC::UNLOCK;
    ChildLockRearLeftLatchStatusFromPDC mPrevChildLockRearLeftLatchStatusFromPDC = ChildLockRearLeftLatchStatusFromPDC::UNLOCK;
    ChildLockRearRightLatchStatusFromPDC mChildLockRearRightLatchStatusFromPDC = ChildLockRearRightLatchStatusFromPDC::UNLOCK;
    ChildLockRearRightLatchStatusFromPDC mPrevChildLockRearRightLatchStatusFromPDC = ChildLockRearRightLatchStatusFromPDC::UNLOCK;
    ChildLockRearLeftLatchStatusFromSBCM mChildLockRearLeftLatchStatusFromSBCM = ChildLockRearLeftLatchStatusFromSBCM::UNLOCK;
    ChildLockRearLeftLatchStatusFromSBCM mPrevChildLockRearLeftLatchStatusFromSBCM = ChildLockRearLeftLatchStatusFromSBCM::UNLOCK;
    ChildLockRearRightLatchStatusFromSBCM mChildLockRearRightLatchStatusFromSBCM = ChildLockRearRightLatchStatusFromSBCM::UNLOCK;
    ChildLockRearRightLatchStatusFromSBCM mPrevChildLockRearRightLatchStatusFromSBCM =
        ChildLockRearRightLatchStatusFromSBCM::UNLOCK;
    ChildLockSeaLockFailStatus mChildLockSeaLockFailStatus = ChildLockSeaLockFailStatus::NORMAL;

    LockStatus mChildLockRearLatchRLPDCStatus = LockStatus::NONE;
    LockStatus mChildLockRearLatchRRPDCStatus = LockStatus::NONE;
    LockStatus mChildLockRearLatchRLSBCMStatus = LockStatus::NONE;
    LockStatus mChildLockRearLatchRRSBCMStatus = LockStatus::NONE;
};

}  // namespace ccos

#endif  // SFSS_Child_Lock_H
