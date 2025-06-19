/**
 * @file xEV_Power_Limit.hpp
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
#ifndef SFSS_xEV_Power_Limit_H
#define SFSS_xEV_Power_Limit_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_Power_LimitBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.15
// Reference : [CV440] xEV_Power_Limit. Contains : Telltale. Recommend: VALUE_CHANGED
class xEV_Power_Limit : public xEV_Power_LimitBase {
public:
    xEV_Power_Limit() = default;
    ~xEV_Power_Limit() override = default;
    xEV_Power_Limit(const xEV_Power_Limit& other) = delete;
    xEV_Power_Limit(xEV_Power_Limit&& other) noexcept = delete;
    xEV_Power_Limit& operator=(const xEV_Power_Limit& other) = delete;
    xEV_Power_Limit& operator=(xEV_Power_Limit&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void onPowerLimitStatusChanged(const ArgumentsPowerLimitStatusChanged& args) {
        mPowerLimitStatus = args.mInput_PowerLimitStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCXEV_Power_LimitTelltaleEvPwrDnStat stat = SFCXEV_Power_LimitTelltaleEvPwrDnStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mPowerLimitStatus == PowerLimitStatus::ON) {
            stat = SFCXEV_Power_LimitTelltaleEvPwrDnStat::ON;
        }
        setSFCXEV_Power_LimitTelltaleEvPwrDnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using PowerLimitStatus = ArgumentsPowerLimitStatusChanged::Input_PowerLimitStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    PowerLimitStatus mPowerLimitStatus = PowerLimitStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_xEV_Power_Limit_H
