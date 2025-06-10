/**
 * @file RBS.hpp
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
#ifndef SFSS_RBS_H
#define SFSS_RBS_H

#define DLOG_ENABLED gEnableSFCLog

#include "RBSBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [CH150] RBS. Contains : Telltale. Recommend: VALUE_CHANGED
class RBS : public RBSBase {
public:
    RBS() = default;
    ~RBS() override = default;
    RBS(const RBS& other) = delete;
    RBS(RBS&& other) noexcept = delete;
    RBS& operator=(const RBS& other) = delete;
    RBS& operator=(RBS&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onRbsWarnChanged(const ArgumentsRbsWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_RbsWarnStatus) == true) {
            mRbsWarnStatus = RbsWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mRbsWarnStatus = args.mInput_RbsWarnStatus;
        }
        if (ISTIMEOUT(args.mInput_RbsWarnStatusFromEbb) == true) {
            mRbsWarnStatusFromEbb = RbsWarnStatusFromEbb::UNHANDLED_TIMEOUT;
        } else {
            mRbsWarnStatusFromEbb = args.mInput_RbsWarnStatusFromEbb;
        }
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigIEB = args.mInter_ConfigIEB;
        mConfigEBB = args.mInter_ConfigEBB;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCRBSTelltaleRbsStat telltaleStat = SFCRBSTelltaleRbsStat::OFF;
        if (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV ||
            mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
            if ((mIgnElapsed == IgnElapsed::ON_500ms ||
                 (mIgnElapsed == IgnElapsed::ON_3500ms &&
                  (mRbsWarnStatus == RbsWarnStatus::ON ||
                   (mConfigIEB == ConfigIEB::ON && mRbsWarnStatus == RbsWarnStatus::UNHANDLED_TIMEOUT) ||
                   (mConfigEBB == ConfigEBB::ON && (mRbsWarnStatusFromEbb == RbsWarnStatusFromEbb::ON ||
                                                    mRbsWarnStatusFromEbb == RbsWarnStatusFromEbb::UNHANDLED_TIMEOUT)))))) {
                telltaleStat = SFCRBSTelltaleRbsStat::ON;
            }
        }
        setSFCRBSTelltaleRbsStat(telltaleStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using RbsWarnStatus = ArgumentsRbsWarnChanged::Input_RbsWarnStatus;
    using RbsWarnStatusFromEbb = ArgumentsRbsWarnChanged::Input_RbsWarnStatusFromEbb;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigIEB = ArgumentsConfigChanged::Inter_ConfigIEB;
    using ConfigEBB = ArgumentsConfigChanged::Inter_ConfigEBB;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    RbsWarnStatus mRbsWarnStatus = RbsWarnStatus::OFF;
    RbsWarnStatusFromEbb mRbsWarnStatusFromEbb = RbsWarnStatusFromEbb::OFF;
    ConfigIEB mConfigIEB = ConfigIEB::OFF;
    ConfigEBB mConfigEBB = ConfigEBB::OFF;
};

}  // namespace ccos

#endif  // SFSS_RBS_H
