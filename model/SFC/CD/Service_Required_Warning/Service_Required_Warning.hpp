/**
 * @file Service_Required_Warning.hpp
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
#ifndef SFSS_Service_Required_Warning_H
#define SFSS_Service_Required_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Service_Required_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 5.0.10
// Reference : [CD130] Service_Required_Warning. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Service_Required_Warning : public Service_Required_WarningBase {
public:
    Service_Required_Warning() = default;
    ~Service_Required_Warning() override = default;
    Service_Required_Warning(const Service_Required_Warning& other) = delete;
    Service_Required_Warning(Service_Required_Warning&& other) noexcept = delete;
    Service_Required_Warning& operator=(const Service_Required_Warning& other) = delete;
    Service_Required_Warning& operator=(Service_Required_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCService_Required_WarningConstantServiceRemindDayStat(
            SFCService_Required_WarningConstantServiceRemindDayStat::DISPLAY_OFF);
        setSFCService_Required_WarningConstantServiceRemindDayValue(0);
        setSFCService_Required_WarningConstantServiceRemindDistKmStat(
            SFCService_Required_WarningConstantServiceRemindDistKmStat::DISPLAY_OFF);
        setSFCService_Required_WarningConstantServiceRemindDistKmValue(0);
        setSFCService_Required_WarningConstantServiceRemindDistMiStat(
            SFCService_Required_WarningConstantServiceRemindDistMiStat::DISPLAY_OFF);
        setSFCService_Required_WarningConstantServiceRemindDistMiValue(0);
        setSFCService_Required_WarningEventServiceRemindApproachID("E00601");
        setSFCService_Required_WarningEventServiceRemindApproachLinkedSoundID("SND_PopUpInform2");
        setSFCService_Required_WarningEventServiceRemindApproachLinkedSoundType(
            SFCService_Required_WarningEventServiceRemindApproachLinkedSoundType::REPEAT_COUNT);
        setSFCService_Required_WarningEventServiceRemindApproachLinkedSoundRepeatCount(1);
        setSFCService_Required_WarningEventServiceRemindReachID("E00602");
        setSFCService_Required_WarningEventServiceRemindReachLinkedSoundID("SND_PopUpInform2");
        setSFCService_Required_WarningEventServiceRemindReachLinkedSoundType(
            SFCService_Required_WarningEventServiceRemindReachLinkedSoundType::REPEAT_COUNT);
        setSFCService_Required_WarningEventServiceRemindReachLinkedSoundRepeatCount(1);

        if (mOneShotTimerApproachOffId.create(kTimerApproachOffInterval4s, this,
                                              &Service_Required_Warning::onOneShotTimerApproachOff, false) == false) {
            DWarning() << "[Service_Required_Warning] mOneShotTimerApproachOffId Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstant();
        updateEvent();
    }

    void onIgnElapseOn4000msChanged(const ArgumentsIgnElapseOn4000msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_4000ms;
        updateConstant();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstant();
        updateEvent();
    }

    void onServiceChanged(const ArgumentsServiceChanged& args) {
        updateValueIfValid(mSrvRemindPeriodSetValue, args.mInter_SrvRemindPeriodSetValue);
        updateValueIfValid(mSrvRemindDistKmSetValue, args.mInter_SrvRemindDistKmSetValue);
        updateValueIfValid(mSrvRemindDistMiSetValue, args.mInter_SrvRemindDistMiSetValue);
        updateValueIfValid(mSrvRemainPeriod, args.mInter_SrvRemainPeriod);
        updateValueIfValid(mSrvRemainDistanceKm, args.mInter_SrvRemainDistanceKm);
        updateValueIfValid(mSrvRemainDistanceMi, args.mInter_SrvRemainDistanceMi);
        updateConstant();
        updateEvent();
    }

    void onTripAndSrvStatChanged(const ArgumentsTripAndSrvStatChanged& args) {
        updateValueIfValid(mTripDistanceUnit, args.mInter_TripDistanceUnit);
        updateValueIfValid(mSrvRemindSetStatus, args.mInter_SrvRemindSetStatus);
        updateEvent();
    }

    void onSystemCheckDisplayStatusChanged(const ArgumentsSystemCheckDisplayStatusChanged& args) {
        updateValueIfValid(mSystemCheckDisplayStatus, args.mInter_SystemCheckDisplayStatus);
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_4000ms) {
            if (mSrvRemindPeriodSetValue != 0) {
                setSFCService_Required_WarningConstantServiceRemindDayStat(
                    SFCService_Required_WarningConstantServiceRemindDayStat::VALUE);
                setSFCService_Required_WarningConstantServiceRemindDayValue(mSrvRemainPeriod);
            } else {
                setSFCService_Required_WarningConstantServiceRemindDayStat(
                    SFCService_Required_WarningConstantServiceRemindDayStat::DISPLAY_OFF);
            }

            if (mSrvRemindDistKmSetValue != 0) {
                setSFCService_Required_WarningConstantServiceRemindDistKmStat(
                    SFCService_Required_WarningConstantServiceRemindDistKmStat::VALUE);
                setSFCService_Required_WarningConstantServiceRemindDistKmValue(mSrvRemainDistanceKm);
            } else {
                setSFCService_Required_WarningConstantServiceRemindDistKmStat(
                    SFCService_Required_WarningConstantServiceRemindDistKmStat::DISPLAY_OFF);
            }

            if (mSrvRemindDistMiSetValue != 0) {
                setSFCService_Required_WarningConstantServiceRemindDistMiStat(
                    SFCService_Required_WarningConstantServiceRemindDistMiStat::VALUE);
                setSFCService_Required_WarningConstantServiceRemindDistMiValue(mSrvRemainDistanceMi);
            } else {
                setSFCService_Required_WarningConstantServiceRemindDistMiStat(
                    SFCService_Required_WarningConstantServiceRemindDistMiStat::DISPLAY_OFF);
            }
        } else {
            setSFCService_Required_WarningConstantServiceRemindDayStat(
                SFCService_Required_WarningConstantServiceRemindDayStat::DISPLAY_OFF);
            setSFCService_Required_WarningConstantServiceRemindDistKmStat(
                SFCService_Required_WarningConstantServiceRemindDistKmStat::DISPLAY_OFF);
            setSFCService_Required_WarningConstantServiceRemindDistMiStat(
                SFCService_Required_WarningConstantServiceRemindDistMiStat::DISPLAY_OFF);
        }
    }

    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSrvRemindSetStatus == SrvRemindSetStatus::ENABLE) {
            if (mTripDistanceUnit == TripDistanceUnit::KM &&
                ((mSrvRemainPeriod <= kPeriod30 && mSrvRemainPeriod > 0) ||
                 (mSrvRemainDistanceKm <= kDistanceKm1500 && mSrvRemainDistanceKm > 0))) {
                mStatApproach = SFCService_Required_WarningEventServiceRemindApproachStat::ON;
            }
            if (mTripDistanceUnit == TripDistanceUnit::KM && ((mSrvRemindPeriodSetValue != 0 && mSrvRemainPeriod <= 0) ||
                                                              (mSrvRemindDistKmSetValue != 0 && mSrvRemainDistanceKm <= 0))) {
                mStatReach = SFCService_Required_WarningEventServiceRemindReachStat::ON;
            }
            if (mTripDistanceUnit == TripDistanceUnit::MILE &&
                ((mSrvRemainPeriod <= kPeriod30 && mSrvRemainPeriod > 0) ||
                 (mSrvRemainDistanceMi <= kDistanceMi900 && mSrvRemainDistanceMi > 0))) {
                mStatApproach = SFCService_Required_WarningEventServiceRemindApproachStat::ON;
            }
            if (mTripDistanceUnit == TripDistanceUnit::MILE && ((mSrvRemindPeriodSetValue != 0 && mSrvRemainPeriod <= 0) ||
                                                                (mSrvRemindDistMiSetValue != 0 && mSrvRemainDistanceMi <= 0))) {
                mStatReach = SFCService_Required_WarningEventServiceRemindReachStat::ON;
            }
            if (mStatReach != SFCService_Required_WarningEventServiceRemindReachStat::ON) {
                setSFCService_Required_WarningEventServiceRemindApproachStat(mStatApproach);
            }
            setSFCService_Required_WarningEventServiceRemindReachStat(mStatReach);
        } else if (mIgnElapsed == IgnElapsed::ON_4000ms && mSrvRemindSetStatus == SrvRemindSetStatus::ENABLE &&
                   mStatApproach == SFCService_Required_WarningEventServiceRemindApproachStat::ON &&
                   mStatReach == SFCService_Required_WarningEventServiceRemindReachStat::ON) {
            if (mSystemCheckDisplayStatus == SystemCheckDisplayStatus::OFF && mOneShotTimerApproachOffId.isRunning() == false) {
                if (mOneShotTimerApproachOffId.start() == false) {
                    DWarning() << "[Service_Required_Warning] mOneShotTimerApproachOffId Start Failed";
                }
            }
        } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
            if (mStatApproach == SFCService_Required_WarningEventServiceRemindApproachStat::ON) {
                mStatApproach = SFCService_Required_WarningEventServiceRemindApproachStat::OFF;
                setSFCService_Required_WarningEventServiceRemindApproachStat(mStatApproach);
            }
            if (mStatReach == SFCService_Required_WarningEventServiceRemindReachStat::ON) {
                mStatReach = SFCService_Required_WarningEventServiceRemindReachStat::OFF;
                setSFCService_Required_WarningEventServiceRemindReachStat(mStatReach);
            }
        } else {
            // no operation
        }
    }

    void onOneShotTimerApproachOff() {
        mStatApproach = SFCService_Required_WarningEventServiceRemindApproachStat::OFF;
        setSFCService_Required_WarningEventServiceRemindApproachStat(mStatApproach);
        flush();
        mOneShotTimerApproachOffId.stop();
    }

    static constexpr HInt64 kPeriod30 = 30;
    static constexpr HInt64 kDistanceKm1500 = 1500;
    static constexpr HInt64 kDistanceMi900 = 900;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_4000ms,
        OFF_0ms,
        MAX
    };

    using TripDistanceUnit = ArgumentsTripAndSrvStatChanged::Inter_TripDistanceUnit;
    using SrvRemindSetStatus = ArgumentsTripAndSrvStatChanged::Inter_SrvRemindSetStatus;
    using SystemCheckDisplayStatus = ArgumentsSystemCheckDisplayStatusChanged::Inter_SystemCheckDisplayStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::NONE;
    SrvRemindSetStatus mSrvRemindSetStatus = SrvRemindSetStatus::NONE;
    SystemCheckDisplayStatus mSystemCheckDisplayStatus = SystemCheckDisplayStatus::OFF;
    HUInt64 mSrvRemindPeriodSetValue = 0;
    HUInt64 mSrvRemindDistKmSetValue = 0;
    HUInt64 mSrvRemindDistMiSetValue = 0;
    HInt64 mSrvRemainPeriod = 0;
    HInt64 mSrvRemainDistanceKm = 0;
    HInt64 mSrvRemainDistanceMi = 0;
    static constexpr uint32_t kTimerApproachOffInterval4s = 4000u;
    SFCService_Required_WarningEventServiceRemindApproachStat mStatApproach =
        SFCService_Required_WarningEventServiceRemindApproachStat::OFF;
    SFCService_Required_WarningEventServiceRemindReachStat mStatReach =
        SFCService_Required_WarningEventServiceRemindReachStat::OFF;
    ssfs::SFCTimer<Service_Required_Warning> mOneShotTimerApproachOffId;
};

}  // namespace ccos

#endif  // SFSS_Service_Required_Warning_H
