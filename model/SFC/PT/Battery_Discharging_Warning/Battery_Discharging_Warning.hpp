/**
 * @file Battery_Discharging_Warning.hpp
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
#ifndef SFSS_Battery_Discharging_Warning_H
#define SFSS_Battery_Discharging_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Battery_Discharging_WarningBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [PT040] Battery_Discharging_Warning. Contains : Event. Recommend: VALUE_CHANGED
class Battery_Discharging_Warning : public Battery_Discharging_WarningBase {
public:
    Battery_Discharging_Warning() = default;
    ~Battery_Discharging_Warning() override = default;
    Battery_Discharging_Warning(const Battery_Discharging_Warning& other) = delete;
    Battery_Discharging_Warning(Battery_Discharging_Warning&& other) noexcept = delete;
    Battery_Discharging_Warning& operator=(const Battery_Discharging_Warning& other) = delete;
    Battery_Discharging_Warning& operator=(Battery_Discharging_Warning&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEvent();
    }

    void onBatDischargeWarnChanged(const ArgumentsBatDischargeWarnChanged& args) {
        mBatteryDischargeWarnStatus = args.mInput_BatteryDischargeWarnStatus;
        mLithiumBatteryDischargeWarnStatus = args.mInput_LithiumBatteryDischargeWarnStatus;
        updateEvent();
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using BatteryDischargeWarnStatus = ArgumentsBatDischargeWarnChanged::Input_BatteryDischargeWarnStatus;
    using LithiumBatteryDischargeWarnStatus = ArgumentsBatDischargeWarnChanged::Input_LithiumBatteryDischargeWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    BatteryDischargeWarnStatus mBatteryDischargeWarnStatus = BatteryDischargeWarnStatus::OFF;
    LithiumBatteryDischargeWarnStatus mLithiumBatteryDischargeWarnStatus = LithiumBatteryDischargeWarnStatus::OFF;

    void updateEvent() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
             mConfigVehicleType == ConfigVehicleType::PHEV) &&
            (mBatteryDischargeWarnStatus == BatteryDischargeWarnStatus::ON ||
             mLithiumBatteryDischargeWarnStatus == LithiumBatteryDischargeWarnStatus::ON)) {
            eventID = "E20101";
        } else if (mIgnElapsed == IgnElapsed::ON_0ms &&
                   (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV ||
                    mConfigVehicleType == ConfigVehicleType::EREV) &&
                   (mBatteryDischargeWarnStatus == BatteryDischargeWarnStatus::ON ||
                    mLithiumBatteryDischargeWarnStatus == LithiumBatteryDischargeWarnStatus::ON)) {
            eventID = "E20102";
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Battery_Discharging_Warning.Event.BatteryDischargeWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            DDebug() << "SFCBattery_Discharging_WarningEventBatteryDischargeWarnStat::OFF ";
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnStat(
                SFCBattery_Discharging_WarningEventBatteryDischargeWarnStat::OFF);
            flush();
        }
        if (eventID.empty() == false) {
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnID(eventID);
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnStat(
                SFCBattery_Discharging_WarningEventBatteryDischargeWarnStat::ON);
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnLinkedSoundID("SND_PopUpWarn1");
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnLinkedSoundType(
                SFCBattery_Discharging_WarningEventBatteryDischargeWarnLinkedSoundType::REPEAT_COUNT);
            setSFCBattery_Discharging_WarningEventBatteryDischargeWarnLinkedSoundRepeatCount(1);
        }
    }
};

}  // namespace ccos

#endif  // SFSS_Battery_Discharging_Warning_H
