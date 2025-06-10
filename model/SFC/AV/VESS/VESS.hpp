/**
 * @file VESS.hpp
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
#ifndef SFSS_VESS_H
#define SFSS_VESS_H

#define DLOG_ENABLED gEnableSFCLog

#include "VESSBase.hpp"

namespace ccos {

// SFC Version : 1.1.1
// Reference : [AV060] VESS. Contains : Event. Recommend: VALUE_CHANGED
class VESS : public VESSBase {
public:
    VESS() = default;
    ~VESS() override = default;
    VESS(const VESS& other) = delete;
    VESS(VESS&& other) noexcept = delete;
    VESS& operator=(const VESS& other) = delete;
    VESS& operator=(VESS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCVESSEventVessFailWarnID("E40601");
        setSFCVESSEventVessFailWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCVESSEventVessFailWarnLinkedSoundType(SFCVESSEventVessFailWarnLinkedSoundType::REPEAT_COUNT);
        setSFCVESSEventVessFailWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEvent();
    }

    void onVessFailWarnChanged(const ArgumentsVessFailWarnChanged& args) {
        mVessFailWarnStatus = args.mInput_VessFailWarnStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        SFCVESSEventVessFailWarnStat status = SFCVESSEventVessFailWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mVessFailWarnStatus == VessFailWarnStatus::ON) {
            if (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV ||
                mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
                status = SFCVESSEventVessFailWarnStat::ON;
            }
        }
        setSFCVESSEventVessFailWarnStat(status);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using VessFailWarnStatus = ArgumentsVessFailWarnChanged::Input_VessFailWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    VessFailWarnStatus mVessFailWarnStatus = VessFailWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_VESS_H
