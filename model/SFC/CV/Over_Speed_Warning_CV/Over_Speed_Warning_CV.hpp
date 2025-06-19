/**
 * @file Over_Speed_Warning_CV.hpp
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
#ifndef SFSS_Over_Speed_Warning_CV_H
#define SFSS_Over_Speed_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Over_Speed_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 2.0.1
// Spec Version : v0.05
// Reference : [CV020] Over Speed Warning CV. Contains : Event, Sound. Recommend: VALUE_CHANGED
class Over_Speed_Warning_CV : public Over_Speed_Warning_CVBase {
public:
    Over_Speed_Warning_CV() = default;
    ~Over_Speed_Warning_CV() override = default;
    Over_Speed_Warning_CV(const Over_Speed_Warning_CV& other) = delete;
    Over_Speed_Warning_CV(Over_Speed_Warning_CV&& other) noexcept = delete;
    Over_Speed_Warning_CV& operator=(const Over_Speed_Warning_CV& other) = delete;
    Over_Speed_Warning_CV& operator=(Over_Speed_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCOver_Speed_Warning_CVEventOverSpeedID("E70201");
        setSFCOver_Speed_Warning_CVSoundOverSpeedID("SND_OverSpeedWarnCV");
        setSFCOver_Speed_Warning_CVSoundOverSpeedType(SFCOver_Speed_Warning_CVSoundOverSpeedType::INTERVAL);
        setSFCOver_Speed_Warning_CVSoundOverSpeedInterval(100);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updatePrivateOverSpeedWarnStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updatePrivateOverSpeedWarnStatus();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        mConfigTruckType = args.mInter_ConfigTruckType;
        updatePrivateOverSpeedWarnStatus();
    }

    void onDisplaySpeedValueKPHChanged(const ArgumentsDisplaySpeedValueKPHChanged& args) {
        mSpeedValue = args.mInter_DisplaySpeedValueKPH;
        updatePrivateOverSpeedWarnStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updatePrivateOverSpeedWarnStatus() {
        if (mIsIgnOn) {
            if (mConfigArea == ConfigArea::MID) {
                if (mSpeedValue >= 120.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::MID_WARN_ON;
                } else if (mSpeedValue < 118.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::MID_WARN_OFF;
                } else {
                    // no operation
                }
            } else if (mConfigArea == ConfigArea::CHINA && mConfigTruckType != ConfigTruckType::MIXER) {
                if (mSpeedValue >= 100.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::CHN1_WARN_ON;
                } else if (mSpeedValue < 98.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::CHN1_WARN_OFF;
                } else {
                    // no operation
                }
            } else if (mConfigArea == ConfigArea::CHINA && mConfigTruckType == ConfigTruckType::MIXER) {
                if (mSpeedValue >= 60.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::CHN2_WARN_ON;
                } else if (mSpeedValue < 58.0) {
                    mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::CHN2_WARN_OFF;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        } else {
            mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::OFF;
        }

        updateEventSound();
    }

    void updateEventSound() {
        SFCOver_Speed_Warning_CVEventOverSpeedStat eventStat{SFCOver_Speed_Warning_CVEventOverSpeedStat::OFF};
        SFCOver_Speed_Warning_CVSoundOverSpeedStat soundStat{SFCOver_Speed_Warning_CVSoundOverSpeedStat::OFF};

        if (mIsIgnOn) {
            switch (mPrivateOverSpeedWarnStatus) {
                case PrivateOverSpeedWarnStatus::MID_WARN_ON:
                case PrivateOverSpeedWarnStatus::CHN1_WARN_ON:
                case PrivateOverSpeedWarnStatus::CHN2_WARN_ON:
                    eventStat = SFCOver_Speed_Warning_CVEventOverSpeedStat::ON;
                    soundStat = SFCOver_Speed_Warning_CVSoundOverSpeedStat::ON;
                    break;
                default:
                    break;
            }
        }

        setSFCOver_Speed_Warning_CVEventOverSpeedStat(eventStat);
        setSFCOver_Speed_Warning_CVSoundOverSpeedStat(soundStat);
    }

    enum class PrivateOverSpeedWarnStatus : HUInt64 {
        NONE,
        MID_WARN_ON,
        MID_WARN_OFF,
        CHN1_WARN_ON,
        CHN1_WARN_OFF,
        CHN2_WARN_ON,
        CHN2_WARN_OFF,
        OFF,
        MAX
    };
    PrivateOverSpeedWarnStatus mPrivateOverSpeedWarnStatus = PrivateOverSpeedWarnStatus::OFF;

    HBool mIsIgnOn = false;
    HDouble mSpeedValue = 0;

    using ConfigArea = ArgumentsConfigChanged::Inter_ConfigArea;
    using ConfigTruckType = ArgumentsConfigChanged::Inter_ConfigTruckType;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigTruckType mConfigTruckType = ConfigTruckType::CARGO;
};

}  // namespace ccos

#endif  // SFSS_Over_Speed_Warning_CV_H
