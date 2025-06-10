/**
 * @file HEV_Mode.hpp
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
#ifndef SFSS_HEV_Mode_H
#define SFSS_HEV_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "HEV_ModeBase.hpp"

namespace ccos {

// SFC Version : 2.0.1
// Reference : [PT260] HEV_Mode. Contains : Telltale. Recommend: VALUE_CHANGED
class HEV_Mode : public HEV_ModeBase {
public:
    HEV_Mode() = default;
    ~HEV_Mode() override = default;
    HEV_Mode(const HEV_Mode& other) = delete;
    HEV_Mode(HEV_Mode&& other) noexcept = delete;
    HEV_Mode& operator=(const HEV_Mode& other) = delete;
    HEV_Mode& operator=(HEV_Mode&& other) noexcept = delete;

    void onInitialize() override {
        setSFCHEV_ModeTelltaleHevModStat(SFCHEV_ModeTelltaleHevModStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        if (mIsConfigOn == false) {
            return;
        }
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        if (mIsConfigOn == false) {
            return;
        }
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mIsConfigOn = (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV);
        if (mIsConfigOn == false) {
            return;
        }
        updateTelltale();
    }

    void onHevModChanged(const ArgumentsHevModChanged& args) {
        mVehicleReadyStatus = args.mInput_VehicleReadyStatus;
        mEvEnergyFlowStatus = args.mInput_EvEnergyFlowStatus;
        if (mIsConfigOn == false) {
            return;
        }
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCHEV_ModeTelltaleHevModStat stat = SFCHEV_ModeTelltaleHevModStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mVehicleReadyStatus == VehicleReadyStatus::READY &&
            (mEvEnergyFlowStatus == EvEnergyFlowStatus::VEHICLE_STOP ||
             mEvEnergyFlowStatus == EvEnergyFlowStatus::EV_PROPULSION ||
             mEvEnergyFlowStatus == EvEnergyFlowStatus::REGENERATION)) {
            stat = SFCHEV_ModeTelltaleHevModStat::ON;
        }
        setSFCHEV_ModeTelltaleHevModStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_500ms,
        MAX
    };

    using VehicleReadyStatus = ArgumentsHevModChanged::Input_VehicleReadyStatus;
    using EvEnergyFlowStatus = ArgumentsHevModChanged::Input_EvEnergyFlowStatus;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    HBool mIsConfigOn = false;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    VehicleReadyStatus mVehicleReadyStatus = VehicleReadyStatus::OFF;
    EvEnergyFlowStatus mEvEnergyFlowStatus = EvEnergyFlowStatus::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
};

}  // namespace ccos

#endif  // SFSS_HEV_Mode_H
