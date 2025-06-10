/**
 * @file LPI.hpp
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
#ifndef SFSS_LPI_H
#define SFSS_LPI_H

#define DLOG_ENABLED gEnableSFCLog

#include "LPIBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT310] LPI. Contains : Telltale. Recommend: VALUE_CHANGED
class LPI : public LPIBase {
public:
    LPI() = default;
    ~LPI() override = default;
    LPI(const LPI& other) = delete;
    LPI(LPI&& other) noexcept = delete;
    LPI& operator=(const LPI& other) = delete;
    LPI& operator=(LPI&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLPITelltaleLPIStat(SFCLPITelltaleLPIStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onFuelTypeChanged(const ArgumentsFuelTypeChanged& args) {
        mConfigFuelType = args.mInter_ConfigFuelType;
        updateTelltale();
    }

    void onLpiStartChanged(const ArgumentsLpiStartChanged& args) {
        mLpiStartAvailableStatus = args.mInput_LpiStartAvailableStatus;
        updateTelltale();
    }

private:
    void updateTelltale() {
        SFCLPITelltaleLPIStat stat = SFCLPITelltaleLPIStat::OFF;
        if (mConfigFuelType == ConfigFuelType::LPI && mIgnElapsed == IgnElapsed::ON_0ms &&
            mLpiStartAvailableStatus == LpiStartAvailableStatus::UNAVAILABLE) {
            stat = SFCLPITelltaleLPIStat::ON;
        }
        setSFCLPITelltaleLPIStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using ConfigFuelType = ArgumentsFuelTypeChanged::Inter_ConfigFuelType;
    using LpiStartAvailableStatus = ArgumentsLpiStartChanged::Input_LpiStartAvailableStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigFuelType mConfigFuelType = ConfigFuelType::GSL;
    LpiStartAvailableStatus mLpiStartAvailableStatus = LpiStartAvailableStatus::UNAVAILABLE;
};

}  // namespace ccos

#endif  // SFSS_LPI_H
