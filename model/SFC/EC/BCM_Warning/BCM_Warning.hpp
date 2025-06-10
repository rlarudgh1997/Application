/**
 * @file BCM_Warning.hpp
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
#ifndef SFSS_BCM_Warning_H
#define SFSS_BCM_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "BCM_WarningBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [EC010] BCM_Warning. Contains : Event, Sound. Recommend: VALUE_CHANGED
class BCM_Warning : public BCM_WarningBase {
public:
    BCM_Warning() = default;
    ~BCM_Warning() override = default;
    BCM_Warning(const BCM_Warning& other) = delete;
    BCM_Warning(BCM_Warning&& other) noexcept = delete;
    BCM_Warning& operator=(const BCM_Warning& other) = delete;
    BCM_Warning& operator=(BCM_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCBCM_WarningSoundBCMID("SND_VehicleNotPWarn");
        setSFCBCM_WarningSoundBCMType(SFCBCM_WarningSoundBCMType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateWarningEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateWarningEvent();
    }

    void onIgnElapseOff700msChanged(const ArgumentsIgnElapseOff700msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_700ms;
        updateWarningEvent();
    }

    void onBCMWARNINGStatusChanged(const ArgumentsBCMWARNINGStatusChanged& args) {
        mBCMWARNINGState = args.mInput_BcmWarnStatus;
        updateWarningEvent();
        updateSound();
    }

    void onVehNotPWARNINGStatusChanged(const ArgumentsVehNotPWARNINGStatusChanged& args) {
        mVehNotPWARNINGState = args.mInput_VehNotPWrngSta;
        updateWarningEvent();
        updateSound();
    }

private:
    void updateWarningEvent() {
        std::string eventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mBCMWARNINGState == BcmWarnStatus::VEHICLE_IS_ON_WARN) {
                eventID = "E60001";
            } else if (mBCMWARNINGState == BcmWarnStatus::SHIFT_P_WARN) {
                eventID = "E60002";
            } else if (mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWRON_E_SHIFT) {
                eventID = "E60005";
            } else {
                // nothing
            }
        } else if (mIgnElapsed == IgnElapsed::OFF_700ms) {
            if (mBCMWARNINGState == BcmWarnStatus::SHIFT_P_WARN ||
                mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWROFF_E_SHIFT) {
                eventID = "E60003";
            } else if (mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWROFF_D_SHIFT) {
                eventID = "E60004";
            } else if (mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWRON_E_SHIFT) {
                eventID = "E60005";
            } else if (mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWRON_D_SHIFT) {
                eventID = "E60706";
            } else {
                // nothing
            }
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.BCM_Warning.Event.BCM.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCBCM_WarningEventBCMStat(SFCBCM_WarningEventBCMStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCBCM_WarningEventBCMID(eventID);
            setSFCBCM_WarningEventBCMStat(SFCBCM_WarningEventBCMStat::ON);
        }
    }

    void updateSound() {
        if (mBCMWARNINGState == BcmWarnStatus::SHIFT_P_WARN || mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWROFF_E_SHIFT ||
            mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWROFF_D_SHIFT ||
            mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWRON_E_SHIFT ||
            mVehNotPWARNINGState == VehNotPWarnStatus::NOT_P_PWRON_D_SHIFT) {
            setSFCBCM_WarningSoundBCMStat(SFCBCM_WarningSoundBCMStat::ON);
        } else {
            setSFCBCM_WarningSoundBCMStat(SFCBCM_WarningSoundBCMStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        OFF_700ms,
        MAX
    };

    using BcmWarnStatus = ArgumentsBCMWARNINGStatusChanged::Input_BcmWarnStatus;
    using VehNotPWarnStatus = ArgumentsVehNotPWARNINGStatusChanged::Input_VehNotPWrngSta;

    BcmWarnStatus mBCMWARNINGState = BcmWarnStatus::OFF;
    VehNotPWarnStatus mVehNotPWARNINGState = VehNotPWarnStatus::OFF;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
};

}  // namespace ccos

#endif  // SFSS_BCM_Warning_H
