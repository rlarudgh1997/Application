/**
 * @file EV_Accel_Boost_Mode.hpp
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
#ifndef SFSS_EV_Accel_Boost_Mode_H
#define SFSS_EV_Accel_Boost_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "EV_Accel_Boost_ModeBase.hpp"

namespace ccos {

// SFC Version : 1.0.2
// Reference : [PT500] EV_Accel_Boost_Mode. Contains : Constant. Recommend: VALUE_CHANGED
class EV_Accel_Boost_Mode : public EV_Accel_Boost_ModeBase {
public:
    EV_Accel_Boost_Mode() = default;
    ~EV_Accel_Boost_Mode() override = default;
    EV_Accel_Boost_Mode(const EV_Accel_Boost_Mode& other) = delete;
    EV_Accel_Boost_Mode(EV_Accel_Boost_Mode&& other) noexcept = delete;
    EV_Accel_Boost_Mode& operator=(const EV_Accel_Boost_Mode& other) = delete;
    EV_Accel_Boost_Mode& operator=(EV_Accel_Boost_Mode&& other) noexcept = delete;

    void onInitialize() override {
        updateOutputAccelBoostModeAppliedStatus();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateConstant();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateConstant();
    }

    void onAccelBoostOnOffStatusChanged(const ArgumentsAccelBoostOnOffStatusChanged& args) {
        mAccelBoostOnOffStatus = args.mInput_AccelBoostOnOffStatus;
        updateConstant();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        SFCEV_Accel_Boost_ModeConstantAccelBoostOnOffStat stat = SFCEV_Accel_Boost_ModeConstantAccelBoostOnOffStat::DISPLAY_OFF;

        if (mIsIgnOn == true) {
            if (mAccelBoostOnOffStatus == AccelBoostOnOffStatus::BOOST_ON) {
                stat = SFCEV_Accel_Boost_ModeConstantAccelBoostOnOffStat::BOOST_ON;
            } else if (mAccelBoostOnOffStatus == AccelBoostOnOffStatus::BOOST_OFF) {
                stat = SFCEV_Accel_Boost_ModeConstantAccelBoostOnOffStat::BOOST_OFF;
            } else {
                // DISPLAY_OFF
            }
        }

        setSFCEV_Accel_Boost_ModeConstantAccelBoostOnOffStat(stat);
    }

    void updateOutputAccelBoostModeAppliedStatus() {
        OutputAccelBoostModeAppliedStatus status = OutputAccelBoostModeAppliedStatus::APPLIED;

        AddressValueList outputList;
        outputList.emplace_back(Vehicle.PT.EV_Accel_Boost_Mode.Output_AccelBoostModeAppliedStatus, static_cast<HUInt64>(status));
        setValue(outputList);
        outputList.clear();
    }

    using AccelBoostOnOffStatus = ArgumentsAccelBoostOnOffStatusChanged::Input_AccelBoostOnOffStatus;
    using OutputAccelBoostModeAppliedStatus = decltype(Vehicle.PT.EV_Accel_Boost_Mode.Output_AccelBoostModeAppliedStatus)::TYPE;

    AccelBoostOnOffStatus mAccelBoostOnOffStatus = AccelBoostOnOffStatus::OFF;

    bool mIsIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_EV_Accel_Boost_Mode_H
