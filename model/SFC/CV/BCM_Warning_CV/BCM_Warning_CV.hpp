/**
 * @file BCM_Warning_CV.hpp
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
#ifndef SFSS_BCM_Warning_CV_H
#define SFSS_BCM_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "BCM_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.09
// Reference : [CV850] BCM_Warning_CV. Contains : Event, Sound. Recommend: VALUE_CHANGED
class BCM_Warning_CV : public BCM_Warning_CVBase {
public:
    BCM_Warning_CV() = default;
    ~BCM_Warning_CV() override = default;
    BCM_Warning_CV(const BCM_Warning_CV& other) = delete;
    BCM_Warning_CV(BCM_Warning_CV&& other) noexcept = delete;
    BCM_Warning_CV& operator=(const BCM_Warning_CV& other) = delete;
    BCM_Warning_CV& operator=(BCM_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCBCM_Warning_CVEventBcmWarningID("E78501");
        setSFCBCM_Warning_CVEventNotParkingWarnID("E78502");
        setSFCBCM_Warning_CVSoundNotParkingWarnID("SND_VehicleNotPWarn");
        setSFCBCM_Warning_CVSoundNotParkingWarnType(SFCBCM_Warning_CVSoundNotParkingWarnType::INFINITE);
        setSFCBCM_Warning_CVSoundBcmWarningID("SND_PowerOffWarn");
        setSFCBCM_Warning_CVSoundBcmWarningType(SFCBCM_Warning_CVSoundBcmWarningType::DURATION_MINMAX);
        setSFCBCM_Warning_CVSoundBcmWarningDurationMin(0);
        setSFCBCM_Warning_CVSoundBcmWarningDurationMax(10000);
        setSFCBCM_Warning_CVSoundBcmSndWarningType(SFCBCM_Warning_CVSoundBcmSndWarningType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEventBcmWarningStat();
        updateSoundBcmWarningStat();
        updateSoundBcmSndWarningStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEventBcmWarningStat();
        updateSoundBcmWarningStat();
        updateSoundBcmSndWarningStat();
    }

    void onBcmWarningStatusChanged(const ArgumentsBcmWarningStatusChanged& args) {
        mBcmWarningStatus = args.mInput_BcmWarningStatus;
        updateEventBcmWarningStat();
        updateSoundBcmWarningStat();
    }

    void onNotParkingWarnStatusChanged(const ArgumentsNotParkingWarnStatusChanged& args) {
        mNotParkingWarnStatus = args.mInput_NotParkingWarnStatus;
        updateEventNotParkingWarnStat();
        updateSoundNotParkingWarnStat();
    }

    void onBcmSndWarningStatus(const ArgumentsBcmSndWarningStatus& args) {
        mBcmSndWarningStatus = args.mInput_BcmSndWarningStatus;
        updateSoundBcmSndWarningStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventBcmWarningStat() {
        SFCBCM_Warning_CVEventBcmWarningStat stat = SFCBCM_Warning_CVEventBcmWarningStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mBcmWarningStatus == BcmWarningStatus::POWER_OFF) {
            stat = SFCBCM_Warning_CVEventBcmWarningStat::ON;
        }
        setSFCBCM_Warning_CVEventBcmWarningStat(stat);
    }

    void updateEventNotParkingWarnStat() {
        SFCBCM_Warning_CVEventNotParkingWarnStat stat = SFCBCM_Warning_CVEventNotParkingWarnStat::OFF;

        if (mNotParkingWarnStatus == NotParkingWarnStatus::ON) {
            stat = SFCBCM_Warning_CVEventNotParkingWarnStat::ON;
        }
        setSFCBCM_Warning_CVEventNotParkingWarnStat(stat);
    }

    void updateSoundNotParkingWarnStat() {
        SFCBCM_Warning_CVSoundNotParkingWarnStat stat = SFCBCM_Warning_CVSoundNotParkingWarnStat::OFF;

        if (mNotParkingWarnStatus == NotParkingWarnStatus::ON) {
            stat = SFCBCM_Warning_CVSoundNotParkingWarnStat::ON;
        }
        setSFCBCM_Warning_CVSoundNotParkingWarnStat(stat);
    }

    void updateSoundBcmWarningStat() {
        SFCBCM_Warning_CVSoundBcmWarningStat stat = SFCBCM_Warning_CVSoundBcmWarningStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mBcmWarningStatus == BcmWarningStatus::POWER_OFF) {
            stat = SFCBCM_Warning_CVSoundBcmWarningStat::ON;
        }
        setSFCBCM_Warning_CVSoundBcmWarningStat(stat);
    }

    void updateSoundBcmSndWarningStat() {
        std::string soundID;
        if (mBcmSndWarningStatus == BcmSndWarningStatus::KEY_REMIND) {
            soundID = "SND_KeyRemindWarn";
        } else if (mIgnElapsed == IgnElapsed::ON_0ms && mBcmSndWarningStatus == BcmSndWarningStatus::PARKING_WARN) {
            soundID = "SND_ParkingBrakeWarn";
        } else {
            // no operation
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.BCM_Warning_CV.Sound.BcmSndWarning.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCBCM_Warning_CVSoundBcmSndWarningStat(SFCBCM_Warning_CVSoundBcmSndWarningStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCBCM_Warning_CVSoundBcmSndWarningID(soundID);
            setSFCBCM_Warning_CVSoundBcmSndWarningStat(SFCBCM_Warning_CVSoundBcmSndWarningStat::ON);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using BcmWarningStatus = ArgumentsBcmWarningStatusChanged::Input_BcmWarningStatus;
    using NotParkingWarnStatus = ArgumentsNotParkingWarnStatusChanged::Input_NotParkingWarnStatus;
    using BcmSndWarningStatus = ArgumentsBcmSndWarningStatus::Input_BcmSndWarningStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    BcmWarningStatus mBcmWarningStatus = BcmWarningStatus::OFF;
    NotParkingWarnStatus mNotParkingWarnStatus = NotParkingWarnStatus::OFF;
    BcmSndWarningStatus mBcmSndWarningStatus = BcmSndWarningStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_BCM_Warning_CV_H
