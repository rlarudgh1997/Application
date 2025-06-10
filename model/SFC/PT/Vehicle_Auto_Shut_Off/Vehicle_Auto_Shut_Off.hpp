/**
 * @file Vehicle_Auto_Shut_Off.hpp
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
#ifndef SFSS_Vehicle_Auto_Shut_Off_H
#define SFSS_Vehicle_Auto_Shut_Off_H

#define DLOG_ENABLED gEnableSFCLog

#include "Vehicle_Auto_Shut_OffBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Reference : [PT440] Vehicle_Auto_Shut_Off. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Vehicle_Auto_Shut_Off : public Vehicle_Auto_Shut_OffBase {
public:
    Vehicle_Auto_Shut_Off() = default;
    ~Vehicle_Auto_Shut_Off() override = default;
    Vehicle_Auto_Shut_Off(const Vehicle_Auto_Shut_Off& other) = delete;
    Vehicle_Auto_Shut_Off(Vehicle_Auto_Shut_Off&& other) noexcept = delete;
    Vehicle_Auto_Shut_Off& operator=(const Vehicle_Auto_Shut_Off& other) = delete;
    Vehicle_Auto_Shut_Off& operator=(Vehicle_Auto_Shut_Off&& other) noexcept = delete;

    void onInitialize() override {
        setSFCVehicle_Auto_Shut_OffEventDispWarningID("E24001");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateEvent();
    }

    void onTimerValueChanged(const ArgumentsTimerValueChanged& args) {
        mTimer1MinValue = args.mInput_VehicleAutoShutOffType1TimerMinValue;
        mTimer1SecValue = args.mInput_VehicleAutoShutOffType1TimerSecValue;
        mTimer2MinValue = args.mInput_VehicleAutoShutOffType2TimerMinValue;
        mTimer2SecValue = args.mInput_VehicleAutoShutOffType2TimerSecValue;
        updatePrivate_VehicleAutoShutOffTimerValue();
        updateTime();
        updateEvent();
    }

    void onWarnStatusChanged(const ArgumentsWarnStatusChanged& args) {
        mVehicleAutoShutOffType1WarnStatus = args.mInput_VehicleAutoShutOffType1WarnStatus;
        mVehicleAutoShutOffType2WarnStatus = args.mInput_VehicleAutoShutOffType2WarnStatus;
        updatePrivate_VehicleAutoShutOffTimerValue();
        updateTime();
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivate_VehicleAutoShutOffTimerValue() {
        if ((mVehicleAutoShutOffType2WarnStatus == VehicleAutoShutOffType2WarnStatus::WARN) ||
            (mVehicleAutoShutOffType2WarnStatus != VehicleAutoShutOffType2WarnStatus::WARN &&
             mVehicleAutoShutOffType1WarnStatus != VehicleAutoShutOffType1WarnStatus::WARN)) {
            mPrivateVehicleAutoShutOffTimerMinValue = mTimer2MinValue;
            mPrivateVehicleAutoShutOffTimerSecValue = mTimer2SecValue;
        } else if (mVehicleAutoShutOffType2WarnStatus != VehicleAutoShutOffType2WarnStatus::WARN &&
                   mVehicleAutoShutOffType1WarnStatus == VehicleAutoShutOffType1WarnStatus::WARN) {
            mPrivateVehicleAutoShutOffTimerMinValue = mTimer1MinValue;
            mPrivateVehicleAutoShutOffTimerSecValue = mTimer1SecValue;
        } else {
            // no operation
        }
    }

    void updateTime() {
        SFCVehicle_Auto_Shut_OffConstantTimeStat stat = SFCVehicle_Auto_Shut_OffConstantTimeStat::INVALID;
        HUInt64 mResultTimerMinValue = 0;
        HUInt64 mResultTimerSecValue = 0;

        mResultTimerMinValue = (mPrivateVehicleAutoShutOffTimerMinValue > 0 && mPrivateVehicleAutoShutOffTimerMinValue <= 250)
                                   ? 251 - mPrivateVehicleAutoShutOffTimerMinValue
                                   : 0;
        mResultTimerSecValue = (mPrivateVehicleAutoShutOffTimerSecValue >= 2 && mPrivateVehicleAutoShutOffTimerSecValue <= 60)
                                   ? 61 - mPrivateVehicleAutoShutOffTimerSecValue
                                   : 0;
        if ((mPrivateVehicleAutoShutOffTimerMinValue >= 0 && mPrivateVehicleAutoShutOffTimerMinValue <= 250) &&
            ((mPrivateVehicleAutoShutOffTimerSecValue == 0) ||
             (mPrivateVehicleAutoShutOffTimerSecValue >= 2 && mPrivateVehicleAutoShutOffTimerSecValue <= 60))) {
            stat = SFCVehicle_Auto_Shut_OffConstantTimeStat::VALID;
        }

        setSFCVehicle_Auto_Shut_OffConstantMinValue(mResultTimerMinValue);
        setSFCVehicle_Auto_Shut_OffConstantSecValue(mResultTimerSecValue);
        setSFCVehicle_Auto_Shut_OffConstantTimeStat(stat);
    }

    void eventDispWarningSoundOff() {
        setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType(SFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType::NONE);
        setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundRepeatCount(0);
        setSFCVehicle_Auto_Shut_OffEventDispWarningStat(SFCVehicle_Auto_Shut_OffEventDispWarningStat::OFF);
        flush();
    }

    void updateEvent() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            mVehicleAutoShutOffType1WarnStatus == VehicleAutoShutOffType1WarnStatus::WARN) {
            if (mTimer1MinValue >= 1) {
                if (mIsDispWarningInfiniteFlag == true) {
                    mIsDispWarningInfiniteFlag = false;
                    eventDispWarningSoundOff();
                }
                setSFCVehicle_Auto_Shut_OffEventDispWarningStat(SFCVehicle_Auto_Shut_OffEventDispWarningStat::ON);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundID("SND_PopUpInform1");
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType(
                    SFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType::REPEAT_COUNT);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundRepeatCount(1);
            } else if (mTimer1MinValue == 0 && mTimer1SecValue >= 2 && mTimer1SecValue <= 60) {
                setSFCVehicle_Auto_Shut_OffEventDispWarningStat(SFCVehicle_Auto_Shut_OffEventDispWarningStat::ON);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundID("SND_PopUpInform2");
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType(
                    SFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType::INFINITE);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundRepeatCount(0);
                mIsDispWarningInfiniteFlag = true;
            } else {
                eventDispWarningSoundOff();
            }
        } else if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
                   mVehicleAutoShutOffType2WarnStatus == VehicleAutoShutOffType2WarnStatus::WARN) {
            if (mTimer2MinValue >= 1) {
                if (mIsDispWarningInfiniteFlag == true) {
                    mIsDispWarningInfiniteFlag = false;
                    eventDispWarningSoundOff();
                }
                setSFCVehicle_Auto_Shut_OffEventDispWarningStat(SFCVehicle_Auto_Shut_OffEventDispWarningStat::ON);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundID("SND_PopUpInform1");
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType(
                    SFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType::REPEAT_COUNT);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundRepeatCount(1);
            } else if (mTimer2MinValue == 0 && mTimer2SecValue >= 2 && mTimer2SecValue <= 60) {
                setSFCVehicle_Auto_Shut_OffEventDispWarningStat(SFCVehicle_Auto_Shut_OffEventDispWarningStat::ON);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundID("SND_PopUpInform2");
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType(
                    SFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundType::INFINITE);
                setSFCVehicle_Auto_Shut_OffEventDispWarningLinkedSoundRepeatCount(0);
                mIsDispWarningInfiniteFlag = true;
            } else {
                eventDispWarningSoundOff();
            }
        } else {
            eventDispWarningSoundOff();
        }
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;

    HUInt64 mTimer1MinValue = 0;
    HUInt64 mTimer1SecValue = 0;
    HUInt64 mTimer2MinValue = 0;
    HUInt64 mTimer2SecValue = 0;
    HUInt64 mPrivateVehicleAutoShutOffTimerMinValue = 0;
    HUInt64 mPrivateVehicleAutoShutOffTimerSecValue = 0;
    HBool mIsDispWarningInfiniteFlag = false;
    using VehicleAutoShutOffType1WarnStatus = ArgumentsWarnStatusChanged::Input_VehicleAutoShutOffType1WarnStatus;
    using VehicleAutoShutOffType2WarnStatus = ArgumentsWarnStatusChanged::Input_VehicleAutoShutOffType2WarnStatus;
    VehicleAutoShutOffType1WarnStatus mVehicleAutoShutOffType1WarnStatus = VehicleAutoShutOffType1WarnStatus::NO_WARN;
    VehicleAutoShutOffType2WarnStatus mVehicleAutoShutOffType2WarnStatus = VehicleAutoShutOffType2WarnStatus::NO_WARN;
};

}  // namespace ccos

#endif  // SFSS_Vehicle_Auto_Shut_Off_H
