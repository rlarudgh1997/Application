/**
 * @file PHEV_Fuel_Tank.hpp
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
#ifndef SFSS_PHEV_Fuel_Tank_H
#define SFSS_PHEV_Fuel_Tank_H

#define DLOG_ENABLED gEnableSFCLog

#include "PHEV_Fuel_TankBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [EC120] PHEV_Fuel_Tank. Contains : Event. Recommend: VALUE_CHANGED
class PHEV_Fuel_Tank : public PHEV_Fuel_TankBase {
public:
    PHEV_Fuel_Tank() = default;
    ~PHEV_Fuel_Tank() override = default;
    PHEV_Fuel_Tank(const PHEV_Fuel_Tank& other) = delete;
    PHEV_Fuel_Tank(PHEV_Fuel_Tank&& other) noexcept = delete;
    PHEV_Fuel_Tank& operator=(const PHEV_Fuel_Tank& other) = delete;
    PHEV_Fuel_Tank& operator=(PHEV_Fuel_Tank&& other) noexcept = delete;

    void onInitialize() override {
        setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankLinkedSoundType(SFCPHEV_Fuel_TankEventPHEV_Fuel_TankLinkedSoundType::REPEAT_COUNT);
        setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankLinkedSoundRepeatCount(1);
        setSFCPHEV_Fuel_TankEventFuelDrOpenStaID("E61015");
        setSFCPHEV_Fuel_TankEventFuelDrOpenStaLinkedSoundID("SND_PopUpWarn1");
        setSFCPHEV_Fuel_TankEventFuelDrOpenStaLinkedSoundType(SFCPHEV_Fuel_TankEventFuelDrOpenStaLinkedSoundType::REPEAT_COUNT);
        setSFCPHEV_Fuel_TankEventFuelDrOpenStaLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
        updateEventFuelDoorOpenStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
        updateEventFuelDoorOpenStat();
    }

    void onPhevFuelDoorDisplayStatus(const ArgumentsPhevFuelDoorDisplayStatus& args) {
        mPhevFuelDoorDisplayReqStatus = args.mInput_PhevFuelDoorDisplayReqStatus;
        updateEvent();
    }

    void onFuelDoorOpenStatus(const ArgumentsFuelDoorOpenStatus& args) {
        mFuelDrOpenSta = args.mInput_FuelDrOpenSta;
        updateEventFuelDoorOpenStat();
    }

private:
    void updateEvent() {
        std::string eventID;
        std::string soundID;
        if (mPhevFuelDoorDisplayReqStatus == PhevFuelDoorDisplayReqStatus::WAIT_FOR_FUEL_DOOR_OPEN) {
            eventID = "E61001";
            soundID = "SND_PopUpInform2";
        } else if (mPhevFuelDoorDisplayReqStatus == PhevFuelDoorDisplayReqStatus::REFUEL) {
            eventID = "E61004";
            soundID = "SND_PopUpInform2";
        } else if (mIgnElapsed == IgnElapsed::ON_0ms &&
                   mPhevFuelDoorDisplayReqStatus == PhevFuelDoorDisplayReqStatus::FUEL_DOOR_OPEN_WARN) {
            eventID = "E61005";
            soundID = "SND_PopUpWarn1";
        } else if (mPhevFuelDoorDisplayReqStatus == PhevFuelDoorDisplayReqStatus::CHARGING) {
            eventID = "E61006";
            soundID = "SND_PopUpWarn1";
        } else if (mPhevFuelDoorDisplayReqStatus == PhevFuelDoorDisplayReqStatus::CHECK_FUEL_DOOR) {
            eventID = "E61007";
            soundID = "SND_PopUpWarn1";
        } else {
            // not specified in Section 5.11.5.3
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.PHEV_Fuel_Tank.Event.PHEV_Fuel_Tank.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankStat(SFCPHEV_Fuel_TankEventPHEV_Fuel_TankStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankID(eventID);
            setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankStat(SFCPHEV_Fuel_TankEventPHEV_Fuel_TankStat::ON);
            setSFCPHEV_Fuel_TankEventPHEV_Fuel_TankLinkedSoundID(soundID);
        }
    }

    void updateEventFuelDoorOpenStat() {
        SFCPHEV_Fuel_TankEventFuelDrOpenStaStat stat = SFCPHEV_Fuel_TankEventFuelDrOpenStaStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mFuelDrOpenSta == FuelDrOpenSta::OPEN) {
            stat = SFCPHEV_Fuel_TankEventFuelDrOpenStaStat::ON;
        }
        setSFCPHEV_Fuel_TankEventFuelDrOpenStaStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using PhevFuelDoorDisplayReqStatus = ArgumentsPhevFuelDoorDisplayStatus::Input_PhevFuelDoorDisplayReqStatus;
    PhevFuelDoorDisplayReqStatus mPhevFuelDoorDisplayReqStatus = PhevFuelDoorDisplayReqStatus::OFF;

    using FuelDrOpenSta = ArgumentsFuelDoorOpenStatus::Input_FuelDrOpenSta;
    FuelDrOpenSta mFuelDrOpenSta = FuelDrOpenSta::CLOSE;
};

}  // namespace ccos

#endif  // SFSS_PHEV_Fuel_Tank_H
