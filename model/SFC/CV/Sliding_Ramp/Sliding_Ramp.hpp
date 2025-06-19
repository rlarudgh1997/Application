/**
 * @file Sliding_Ramp.hpp
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
#ifndef SFSS_Sliding_Ramp_H
#define SFSS_Sliding_Ramp_H

#define DLOG_ENABLED gEnableSFCLog

#include "Sliding_RampBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.11
// Reference : [CV930] Sliding_Ramp. Contains : Telltale. Recommend: VALUE_CHANGED
class Sliding_Ramp : public Sliding_RampBase {
public:
    Sliding_Ramp() = default;
    ~Sliding_Ramp() override = default;
    Sliding_Ramp(const Sliding_Ramp& other) = delete;
    Sliding_Ramp(Sliding_Ramp&& other) noexcept = delete;
    Sliding_Ramp& operator=(const Sliding_Ramp& other) = delete;
    Sliding_Ramp& operator=(Sliding_Ramp&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltale();
    }

    void onSlidingRampOpenStatusChanged(const ArgumentsSlidingRampOpenStatusChanged& args) {
        mSlidingRampOpenStatus = args.mInput_SlidingRampOpenStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mInter_ConfigVehicleTypeCV != ConfigVehicleTypeCV::BUS) {
            return;
        }
        SFCSliding_RampTelltaleSlidingRampStat stat = SFCSliding_RampTelltaleSlidingRampStat::OFF;
        if (mSlidingRampOpenStatus == SlidingRampOpenStatus::ON) {
            stat = SFCSliding_RampTelltaleSlidingRampStat::ON;
        }
        setSFCSliding_RampTelltaleSlidingRampStat(stat);
    }

    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;
    using SlidingRampOpenStatus = ArgumentsSlidingRampOpenStatusChanged::Input_SlidingRampOpenStatus;

    ConfigVehicleTypeCV mInter_ConfigVehicleTypeCV = ConfigVehicleTypeCV::BUS;
    SlidingRampOpenStatus mSlidingRampOpenStatus = SlidingRampOpenStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Sliding_Ramp_H
