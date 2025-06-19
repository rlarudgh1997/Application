/**
 * @file Energy_Flow_CV.hpp
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
#ifndef SFSS_Energy_Flow_CV_H
#define SFSS_Energy_Flow_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>

#include "Energy_Flow_CVBase.hpp"

namespace ccos {

// SFC Version : 5.0.0
// Spec Version : v0.37
// Reference : [CV390] Energy_Flow_CV. Contains : Constant. Recommend: VALUE_CHANGED
class Energy_Flow_CV : public Energy_Flow_CVBase {
public:
    Energy_Flow_CV() = default;
    ~Energy_Flow_CV() override = default;
    Energy_Flow_CV(const Energy_Flow_CV& other) = delete;
    Energy_Flow_CV(Energy_Flow_CV&& other) noexcept = delete;
    Energy_Flow_CV& operator=(const Energy_Flow_CV& other) = delete;
    Energy_Flow_CV& operator=(Energy_Flow_CV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateConstantBattery();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateConstantBattery();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateConstantBattery();
    }

    void onSocGaugeValueChanged(const ArgumentsSocGaugeValueChanged& args) {
        mSocGaugeValue = args.mSOCgaugeValue;
        mSoCGaugeStatus = args.mSOCgaugeStat;
        // mIsBmsDisplaySocValue_Timeout = ISTIMEOUT(args.mInput_BmsDisplaySocValue);

        updateConstantBattery();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantBattery() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            AddressValueList list;
            OutputSocDisplayLevelStatus levelStatus = OutputSocDisplayLevelStatus::INVALID;

            if (mIsBmsDisplaySocValue_Timeout == false) {
                if (mSoCGaugeStatus == SoCGaugeStatus::UNMEASURABLE) {
                    // no operation
                } else if (mSoCGaugeStatus == SoCGaugeStatus::VALUE) {
                    if ((mSocGaugeValue >= 0) && (mSocGaugeValue <= 11)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG0;
                    } else if ((mSocGaugeValue >= 12) && (mSocGaugeValue <= 24)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG1;
                    } else if ((mSocGaugeValue >= 25) && (mSocGaugeValue <= 39)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG2;
                    } else if ((mSocGaugeValue >= 40) && (mSocGaugeValue <= 49)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG3;
                    } else if ((mSocGaugeValue >= 50) && (mSocGaugeValue <= 59)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG4;
                    } else if ((mSocGaugeValue >= 60) && (mSocGaugeValue <= 69)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG5;
                    } else if ((mSocGaugeValue >= 70) && (mSocGaugeValue <= 79)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG6;
                    } else if ((mSocGaugeValue >= 80) && (mSocGaugeValue <= 91)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG7;
                    } else if ((mSocGaugeValue >= 92) && (mSocGaugeValue <= 100)) {
                        levelStatus = OutputSocDisplayLevelStatus::SEG8;
                    } else {
                        // no operation
                    }
                } else {
                    // no operation
                }
            }

            list.emplace_back(Vehicle.CV.Energy_Flow_CV.Output_SocDisplayLevelStatus, static_cast<ccos::HUInt64>(levelStatus));

            setValue(list);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using OutputSocDisplayLevelStatus = decltype(Vehicle.CV.Energy_Flow_CV.Output_SocDisplayLevelStatus)::TYPE;
    using SoCGaugeStatus = ArgumentsSocGaugeValueChanged::SOCgaugeStat;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    HBool mIsBmsDisplaySocValue_Timeout = false;
    HUInt64 mSocGaugeValue = 0;
    SoCGaugeStatus mSoCGaugeStatus = SoCGaugeStatus::NONE;
};

}  // namespace ccos

#endif  // SFSS_Energy_Flow_CV_H
