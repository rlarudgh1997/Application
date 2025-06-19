/**
 * @file Luggage_Door.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Luggage_Door_H
#define SFSS_Luggage_Door_H

#define DLOG_ENABLED gEnableSFCLog

#include "Luggage_DoorBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.13
// Reference : [CV590] Luggage_Door. Contains : Telltale. Recommend: VALUE_CHANGED
class Luggage_Door : public Luggage_DoorBase {
public:
    Luggage_Door() = default;
    ~Luggage_Door() override = default;
    Luggage_Door(const Luggage_Door& other) = delete;
    Luggage_Door(Luggage_Door&& other) noexcept = delete;
    Luggage_Door& operator=(const Luggage_Door& other) = delete;
    Luggage_Door& operator=(Luggage_Door&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onLuggageFlapOpenStatusChanged(const ArgumentsLuggageFlapOpenStatusChanged& args) {
        mLuggageFlapOpenStatus = args.mInput_LuggageFlapOpenStatus;
        updateTelltale();
    }

    void onLuggageFlapLockStatusChanged(const ArgumentsLuggageFlapLockStatusChanged& args) {
        mLuggageFlapLockStatus = args.mInput_LuggageFlapLockStatus;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCLuggage_DoorTelltaleLuggageDoorOpenStat stat = SFCLuggage_DoorTelltaleLuggageDoorOpenStat::OFF;

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::BUS) {
            return;
        }

        if (mLuggageFlapOpenStatus == LuggageFlapOpenStatus::ON || mLuggageFlapLockStatus == LuggageFlapLockStatus::ON) {
            stat = SFCLuggage_DoorTelltaleLuggageDoorOpenStat::ON;
        }

        setSFCLuggage_DoorTelltaleLuggageDoorOpenStat(stat);
    }

    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;
    using LuggageFlapOpenStatus = ArgumentsLuggageFlapOpenStatusChanged::Input_LuggageFlapOpenStatus;
    using LuggageFlapLockStatus = ArgumentsLuggageFlapLockStatusChanged::Input_LuggageFlapLockStatus;

    LuggageFlapLockStatus mLuggageFlapLockStatus = LuggageFlapLockStatus::OFF;
    LuggageFlapOpenStatus mLuggageFlapOpenStatus = LuggageFlapOpenStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::BUS;
};

}  // namespace ccos

#endif  // SFSS_Luggage_Door_H
