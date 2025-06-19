/**
 * @file xEV_Battery_Warning.hpp
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
#ifndef SFSS_xEV_Battery_Warning_H
#define SFSS_xEV_Battery_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_Battery_WarningBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.22
// Reference : [CV830] xEV_Battery_Warning. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class xEV_Battery_Warning : public xEV_Battery_WarningBase {
public:
    xEV_Battery_Warning() = default;
    ~xEV_Battery_Warning() override = default;
    xEV_Battery_Warning(const xEV_Battery_Warning& other) = delete;
    xEV_Battery_Warning(xEV_Battery_Warning&& other) noexcept = delete;
    xEV_Battery_Warning& operator=(const xEV_Battery_Warning& other) = delete;
    xEV_Battery_Warning& operator=(xEV_Battery_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusID("E78301");
        setSFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusLinkedSoundType(
            SFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusLinkedSoundRepeatCount(1);
        setSFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusID("E78302");
        setSFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusLinkedSoundType(
            SFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusLinkedSoundRepeatCount(1);
        setSFCXEV_Battery_WarningEventBmsLowTempStatusID("E78303");
        setSFCXEV_Battery_WarningEventBmsLowTempStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Battery_WarningEventBmsLowTempStatusLinkedSoundType(
            SFCXEV_Battery_WarningEventBmsLowTempStatusLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Battery_WarningEventBmsLowTempStatusLinkedSoundRepeatCount(1);
        setSFCXEV_Battery_WarningEventBmsHighTempStatusID("E78304");
        setSFCXEV_Battery_WarningEventBmsHighTempStatusLinkedSoundID("SND_PopUpWarn2");
        setSFCXEV_Battery_WarningEventBmsHighTempStatusLinkedSoundType(
            SFCXEV_Battery_WarningEventBmsHighTempStatusLinkedSoundType::DURATION_MINMAX);
        setSFCXEV_Battery_WarningEventBmsHighTempStatusLinkedSoundDurationMin(kEventDurationMin);
        setSFCXEV_Battery_WarningEventBmsHighTempStatusLinkedSoundDurationMax(kEventDurationMax);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleEvBattFault();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleEvBattFault();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltaleEvBattFault();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleEvBattFault();
    }

    void onBmsServiceLampStatusChanged(const ArgumentsBmsServiceLampStatusChanged& args) {
        mBmsServiceLampStatus = args.mInput_BmsServiceLampStatus;
        updateTelltaleEvBattFault();
        updateEventBmsCluAlarm2ndStatus();
    }

    void onBmsHighTempStatusChanged(const ArgumentsBmsHighTempStatusChanged& args) {
        mBmsHighTempStatus = args.mInput_BmsHighTempStatus;
        updateTelltaleEvBattHighTemp();
        updateEventBmsHighTempStatus();
    }

    void onBmsLowTempStatusChanged(const ArgumentsBmsLowTempStatusChanged& args) {
        mBmsLowTempStatus = args.mInput_BmsLowTempStatus;
        updateTelltaleEvBattLowTemp();
        updateEventBmsLowTempStatus();
    }

    void onBmsCluAlarm2ndStatusChanged(const ArgumentsBmsCluAlarm2ndStatusChanged& args) {
        mBmsCluAlarm2ndStatus = args.mInput_BmsCluAlarm2ndStatus;
        updateEventBmsCluAlarm2ndStatus();
    }

    void onBmsCluAlarm3rdStatusChanged(const ArgumentsBmsCluAlarm3rdStatusChanged& args) {
        mBmsCluAlarm3rdStatus = args.mInput_BmsCluAlarm3rdStatus;
        updateEventBmsCluAlarm3rdStatus();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleEvBattFault();
        updateTelltaleEvBattHighTemp();
        updateTelltaleEvBattLowTemp();
        updateEventBmsHighTempStatus();
        updateEventBmsLowTempStatus();
        updateEventBmsCluAlarm2ndStatus();
        updateEventBmsCluAlarm3rdStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleEvBattFault() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningTelltaleEvBattFaultStat stat = SFCXEV_Battery_WarningTelltaleEvBattFaultStat::OFF;

        if ((mIgnElapsed == IgnElapsed::ON_3500ms && mBmsServiceLampStatus == BmsServiceLampStatus::ON) ||
            mIgnElapsed == IgnElapsed::ON_500ms || mBmsServiceLampStatus == BmsServiceLampStatus::ON) {
            stat = SFCXEV_Battery_WarningTelltaleEvBattFaultStat::ON;
        }

        setSFCXEV_Battery_WarningTelltaleEvBattFaultStat(stat);
    }

    void updateTelltaleEvBattHighTemp() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningTelltaleEvBattHighTempStat stat = SFCXEV_Battery_WarningTelltaleEvBattHighTempStat::OFF;

        if (mBmsHighTempStatus == BmsHighTempStatus::ON) {
            stat = SFCXEV_Battery_WarningTelltaleEvBattHighTempStat::ON;
        }

        setSFCXEV_Battery_WarningTelltaleEvBattHighTempStat(stat);
    }

    void updateTelltaleEvBattLowTemp() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningTelltaleEvBattLowTempStat stat = SFCXEV_Battery_WarningTelltaleEvBattLowTempStat::OFF;

        if (mBmsLowTempStatus == BmsLowTempStatus::ON) {
            stat = SFCXEV_Battery_WarningTelltaleEvBattLowTempStat::ON;
        }

        setSFCXEV_Battery_WarningTelltaleEvBattLowTempStat(stat);
    }

    void updateEventBmsCluAlarm2ndStatus() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusStat stat = SFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusStat::OFF;

        if (mBmsServiceLampStatus == BmsServiceLampStatus::ON || mBmsCluAlarm2ndStatus == BmsCluAlarm2ndStatus::ON) {
            stat = SFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusStat::ON;
        }

        setSFCXEV_Battery_WarningEventBmsCluAlarm2ndStatusStat(stat);
    }

    void updateEventBmsCluAlarm3rdStatus() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusStat stat = SFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusStat::OFF;

        if (mBmsCluAlarm3rdStatus == BmsCluAlarm3rdStatus::ON) {
            stat = SFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusStat::ON;
        }

        setSFCXEV_Battery_WarningEventBmsCluAlarm3rdStatusStat(stat);
    }

    void updateEventBmsLowTempStatus() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningEventBmsLowTempStatusStat stat = SFCXEV_Battery_WarningEventBmsLowTempStatusStat::OFF;

        if (mBmsLowTempStatus == BmsLowTempStatus::ON) {
            stat = SFCXEV_Battery_WarningEventBmsLowTempStatusStat::ON;
        }

        setSFCXEV_Battery_WarningEventBmsLowTempStatusStat(stat);
    }

    void updateEventBmsHighTempStatus() {
        if (isXEVBatteryWarningVehicleTypeOn() == false) {
            return;
        }

        SFCXEV_Battery_WarningEventBmsHighTempStatusStat stat = SFCXEV_Battery_WarningEventBmsHighTempStatusStat::OFF;

        if (mBmsHighTempStatus == BmsHighTempStatus::ON) {
            stat = SFCXEV_Battery_WarningEventBmsHighTempStatusStat::ON;
        }

        setSFCXEV_Battery_WarningEventBmsHighTempStatusStat(stat);
    }

    inline bool isXEVBatteryWarningVehicleTypeOn() const {
        return mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using BmsServiceLampStatus = ArgumentsBmsServiceLampStatusChanged::Input_BmsServiceLampStatus;
    using BmsHighTempStatus = ArgumentsBmsHighTempStatusChanged::Input_BmsHighTempStatus;
    using BmsLowTempStatus = ArgumentsBmsLowTempStatusChanged::Input_BmsLowTempStatus;
    using BmsCluAlarm2ndStatus = ArgumentsBmsCluAlarm2ndStatusChanged::Input_BmsCluAlarm2ndStatus;
    using BmsCluAlarm3rdStatus = ArgumentsBmsCluAlarm3rdStatusChanged::Input_BmsCluAlarm3rdStatus;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    BmsServiceLampStatus mBmsServiceLampStatus = BmsServiceLampStatus::OFF;
    BmsHighTempStatus mBmsHighTempStatus = BmsHighTempStatus::OFF;
    BmsLowTempStatus mBmsLowTempStatus = BmsLowTempStatus::OFF;
    BmsCluAlarm2ndStatus mBmsCluAlarm2ndStatus = BmsCluAlarm2ndStatus::OFF;
    BmsCluAlarm3rdStatus mBmsCluAlarm3rdStatus = BmsCluAlarm3rdStatus::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;

    static constexpr HUInt64 kEventDurationMin = 0u;
    static constexpr HUInt64 kEventDurationMax = 10000u;
};

}  // namespace ccos

#endif  // SFSS_xEV_Battery_Warning_H
