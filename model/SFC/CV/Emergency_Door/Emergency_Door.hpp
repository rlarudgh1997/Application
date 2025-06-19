/**
 * @file Emergency_Door.hpp
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
#ifndef SFSS_Emergency_Door_H
#define SFSS_Emergency_Door_H

#define DLOG_ENABLED gEnableSFCLog

#include "Emergency_DoorBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.12
// Reference : [CV580] Emergency_Door. Contains : Event. Recommend: VALUE_CHANGED
class Emergency_Door : public Emergency_DoorBase {
public:
    Emergency_Door() = default;
    ~Emergency_Door() override = default;
    Emergency_Door(const Emergency_Door& other) = delete;
    Emergency_Door(Emergency_Door&& other) noexcept = delete;
    Emergency_Door& operator=(const Emergency_Door& other) = delete;
    Emergency_Door& operator=(Emergency_Door&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEmergency_DoorEventDoorOpenID("E76001");
        setSFCEmergency_DoorEventDoorOpenLinkedSoundID("SND_PopUpWarn1");
        setSFCEmergency_DoorEventDoorOpenLinkedSoundType(SFCEmergency_DoorEventDoorOpenLinkedSoundType::REPEAT_COUNT);
        setSFCEmergency_DoorEventDoorOpenLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEventDoorOpen();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEventDoorOpen();
    }

    void onDoorOpenStatusChanged(const ArgumentsDoorOpenStatusChanged& args) {
        mDoorOpenStatus = args.mInput_EmerDoorOpenStatus;
        updateEventDoorOpen();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateEventDoorOpen();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventDoorOpen() {
        SFCEmergency_DoorEventDoorOpenStat stat = SFCEmergency_DoorEventDoorOpenStat::OFF;

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::BUS) {
            return;
        }

        if (mIsIgnOn == true && mDoorOpenStatus == DoorOpenStatus::ON) {
            stat = SFCEmergency_DoorEventDoorOpenStat::ON;
        }
        setSFCEmergency_DoorEventDoorOpenStat(stat);
    }

    using DoorOpenStatus = ArgumentsDoorOpenStatusChanged::Input_EmerDoorOpenStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    HBool mIsIgnOn = false;
    DoorOpenStatus mDoorOpenStatus = DoorOpenStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::BUS;
};

}  // namespace ccos

#endif  // SFSS_Emergency_Door_H
