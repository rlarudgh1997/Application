/**
 * @file EV_Power_Down.hpp
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
#ifndef SFSS_EV_Power_Down_H
#define SFSS_EV_Power_Down_H

#define DLOG_ENABLED gEnableSFCLog

#include "EV_Power_DownBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Reference : [PT180] 5.18 EV Power Down. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class EV_Power_Down : public EV_Power_DownBase {
public:
    EV_Power_Down() = default;
    ~EV_Power_Down() override = default;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateEVPowerDownEvent();
        updateFCEVPowerDownEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
        updateEVPowerDownEvent();
        updateFCEVPowerDownEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateEVPowerDownEvent();
        updateFCEVPowerDownEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
        updateEVPowerDownEvent();
        updateFCEVPowerDownEvent();
    }

    void onInputEvPowerDownModeStatusChanged(const ArgumentsInputEvPowerDownModeStatusChanged& args) {
        mEvPowerDownModeStatus = args.mInput_EvPowerDownModeStatus;
        updateTelltale();
        updateEVPowerDownEvent();
    }

    void onInputFcevPowerLimitStatusChanged(const ArgumentsInputFcevPowerLimitStatusChanged& args) {
        mFcevPowerLimitStatus = args.mInput_FcevPowerLimitStatus;
        updateFCEVPowerDownEvent();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mInter_TripDistanceUnit = args.mInter_TripDistanceUnit;
        updateFCEVPowerDownEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mEvPowerDownModeStatus == EvPowerDownModeStatus::POWER_DOWN && mIgnElapsed == IgnElapsed::ON_500ms &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV ||
             mConfigVehicleType == ConfigVehicleType::EREV)) {
            setSFCEV_Power_DownTelltaleEV_Power_DownStat(SFCEV_Power_DownTelltaleEV_Power_DownStat::ON);
        } else {
            setSFCEV_Power_DownTelltaleEV_Power_DownStat(SFCEV_Power_DownTelltaleEV_Power_DownStat::OFF);
        }
    }

    void updateEVPowerDownEvent() {
        SFCEV_Power_DownEventEV_Power_DownStat stat1 = SFCEV_Power_DownEventEV_Power_DownStat::OFF;
        SFCEV_Power_DownEventEV_Power_DownLinkedSoundType type1 = SFCEV_Power_DownEventEV_Power_DownLinkedSoundType::NONE;
        HUInt64 count1 = 0;

        if (mEvPowerDownModeStatus == EvPowerDownModeStatus::POWER_DOWN &&
            (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::EREV)) {
            stat1 = SFCEV_Power_DownEventEV_Power_DownStat::ON;
            type1 = SFCEV_Power_DownEventEV_Power_DownLinkedSoundType::REPEAT_COUNT;
            count1 = 1;
        }

        setSFCEV_Power_DownEventEV_Power_DownID("E21401");
        setSFCEV_Power_DownEventEV_Power_DownStat(stat1);
        setSFCEV_Power_DownEventEV_Power_DownLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Power_DownEventEV_Power_DownLinkedSoundType(type1);
        setSFCEV_Power_DownEventEV_Power_DownLinkedSoundRepeatCount(count1);
    }

    void updateFCEVPowerDownEvent() {
        std::string eventID2;
        SFCEV_Power_DownEventFCEV_Power_LimitStat stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::OFF;
        std::string soundID2;
        SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::NONE;
        HUInt64 count2 = 0;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV ||
             mConfigVehicleType == ConfigVehicleType::EREV) &&
            (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms)) {
            if (mFcevPowerLimitStatus == FcevPowerLimitStatus::LIMIT_OVERHEAT) {
                eventID2 = "E21402";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn1";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::REPEAT_COUNT;
                count2 = 1;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::LIMIT_ERROR) {
                eventID2 = "E21403";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn1";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::REPEAT_COUNT;
                count2 = 1;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::EMERGENCY_WARN1) {
                eventID2 = "E21404";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn2";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::INFINITE;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::EMERGENCY_WARN2 &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                eventID2 = "E21405";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn2";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::INFINITE;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::EMERGENCY_WARN2 &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                eventID2 = "E21407";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn2";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::INFINITE;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::EMERGENCY_WARN3 &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM) {
                eventID2 = "E21406";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn2";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::INFINITE;
            } else if (mFcevPowerLimitStatus == FcevPowerLimitStatus::EMERGENCY_WARN3 &&
                       mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
                eventID2 = "E21408";
                stat2 = SFCEV_Power_DownEventFCEV_Power_LimitStat::ON;
                soundID2 = "SND_PopUpWarn2";
                type2 = SFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType::INFINITE;
            } else {
                // nothing
            }
        }

        std::string prevEventID2;
        GETCACHEDVALUE(SFC.EV_Power_Down.Event.FCEV_Power_Limit.ID, prevEventID2);
        if (!prevEventID2.empty() && prevEventID2 != eventID2) {
            setSFCEV_Power_DownEventFCEV_Power_LimitStat(SFCEV_Power_DownEventFCEV_Power_LimitStat::OFF);
            flushLastGroup();
        }
        if (!eventID2.empty()) {
            setSFCEV_Power_DownEventFCEV_Power_LimitID(eventID2);
            setSFCEV_Power_DownEventFCEV_Power_LimitStat(stat2);
            setSFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundID(soundID2);
            setSFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundType(type2);
            setSFCEV_Power_DownEventFCEV_Power_LimitLinkedSoundRepeatCount(count2);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using EvPowerDownModeStatus = ArgumentsInputEvPowerDownModeStatusChanged::Input_EvPowerDownModeStatus;
    using FcevPowerLimitStatus = ArgumentsInputFcevPowerLimitStatusChanged::Input_FcevPowerLimitStatus;
    using Inter_TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EvPowerDownModeStatus mEvPowerDownModeStatus = EvPowerDownModeStatus::NORMAL;
    FcevPowerLimitStatus mFcevPowerLimitStatus = FcevPowerLimitStatus::OFF;
    Inter_TripDistanceUnit mInter_TripDistanceUnit = Inter_TripDistanceUnit::KM;
};

}  // namespace ccos

#endif  // SFSS_EV_Power_Down_H
