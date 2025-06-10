/**
 * @file xEV_System_Ready.hpp
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
#ifndef SFSS_xEV_System_Ready_H
#define SFSS_xEV_System_Ready_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_System_ReadyBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Reference : [PT210] xEV_System_Ready. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class xEV_System_Ready : public xEV_System_ReadyBase {
public:
    xEV_System_Ready() = default;
    ~xEV_System_Ready() override = default;
    xEV_System_Ready(const xEV_System_Ready& other) = delete;
    xEV_System_Ready(xEV_System_Ready&& other) noexcept = delete;
    xEV_System_Ready& operator=(const xEV_System_Ready& other) = delete;
    xEV_System_Ready& operator=(xEV_System_Ready&& other) noexcept = delete;

    void onInitialize() override {
        setSFCXEV_System_ReadyEventEvReadyIndReqID("E21601");
        setSFCXEV_System_ReadySoundEvReadyIndReqID("SND_Ready");
        setSFCXEV_System_ReadySoundEvReadyIndReqType(SFCXEV_System_ReadySoundEvReadyIndReqType::REPEAT_COUNT);
        setSFCXEV_System_ReadySoundEvReadyIndReqRepeatCount(1);
        setSFCXEV_System_ReadyTelltaleXevRdyBlinkValueA(SFCXEV_System_ReadyTelltaleXevRdyBlinkValueA::ON);
        setSFCXEV_System_ReadyTelltaleXevRdyBlinkValueB(SFCXEV_System_ReadyTelltaleXevRdyBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;

        updateTelltale();
        updateEvent();
        updateSound();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;

        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;

        updateTelltale();
        updateEvent();
        updateSound();
    }

    void onEvReadyIndReqChanged(const ArgumentsEvReadyIndReqChanged& args) {
        if (ISTIMEOUT(args.mInput_EvReadyIndReqStatus) == true) {
            mEvReadyIndReqStatus = EvReadyIndReqStatus::UNHANDLED_TIMEOUT;
        } else {
            mEvReadyIndReqStatus = args.mInput_EvReadyIndReqStatus;
        }
        if (ISTIMEOUT(args.mInput_EvReadyIndReqStatusForEv) == true) {
            mEvReadyIndReqStatusForEv = EvReadyIndReqStatusForEv::UNHANDLED_TIMEOUT;
        } else {
            mEvReadyIndReqStatusForEv = args.mInput_EvReadyIndReqStatusForEv;
        }

        updateTelltale();
        updateEvent();
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        DDebug() << "Inter_ConfigVehicleType: " << static_cast<HUInt64>(mInter_ConfigVehicleType);

        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCXEV_System_ReadyTelltaleXevRdyStat telltaleStat = SFCXEV_System_ReadyTelltaleXevRdyStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEvReadyIndReqStatus == EvReadyIndReqStatus::BLINK ||
                mEvReadyIndReqStatusForEv == EvReadyIndReqStatusForEv::BLINK) {
                telltaleStat = SFCXEV_System_ReadyTelltaleXevRdyStat::BLINK2;
            } else if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) &&
                       mEvReadyIndReqStatus == EvReadyIndReqStatus::UNHANDLED_TIMEOUT) {
                telltaleStat = SFCXEV_System_ReadyTelltaleXevRdyStat::BLINK2;
            } else if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) &&
                       mEvReadyIndReqStatusForEv == EvReadyIndReqStatusForEv::UNHANDLED_TIMEOUT) {
                telltaleStat = SFCXEV_System_ReadyTelltaleXevRdyStat::BLINK2;
            } else if (mEvReadyIndReqStatus == EvReadyIndReqStatus::ON ||
                       mEvReadyIndReqStatusForEv == EvReadyIndReqStatusForEv::ON) {
                telltaleStat = SFCXEV_System_ReadyTelltaleXevRdyStat::ON;
            } else {
                // no operation
            }
        }
        setSFCXEV_System_ReadyTelltaleXevRdyStat(telltaleStat);
    }

    void updateEvent() {
        SFCXEV_System_ReadyEventEvReadyIndReqStat eventStat = SFCXEV_System_ReadyEventEvReadyIndReqStat::OFF;

        if (mIsIgnOn == true &&
            (mEvReadyIndReqStatus == EvReadyIndReqStatus::ON || mEvReadyIndReqStatusForEv == EvReadyIndReqStatusForEv::ON)) {
            eventStat = SFCXEV_System_ReadyEventEvReadyIndReqStat::ON;
        }
        setSFCXEV_System_ReadyEventEvReadyIndReqStat(eventStat);
    }

    void updateSound() {
        SFCXEV_System_ReadySoundEvReadyIndReqStat soundStat = SFCXEV_System_ReadySoundEvReadyIndReqStat::OFF;

        if (mIsIgnOn == true &&
            (mEvReadyIndReqStatus == EvReadyIndReqStatus::ON || mEvReadyIndReqStatusForEv == EvReadyIndReqStatusForEv::ON)) {
            soundStat = SFCXEV_System_ReadySoundEvReadyIndReqStat::ON;
        }
        setSFCXEV_System_ReadySoundEvReadyIndReqStat(soundStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using EvReadyIndReqStatus = ArgumentsEvReadyIndReqChanged::Input_EvReadyIndReqStatus;
    using EvReadyIndReqStatusForEv = ArgumentsEvReadyIndReqChanged::Input_EvReadyIndReqStatusForEv;
    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    bool mIsIgnOn = false;
    EvReadyIndReqStatus mEvReadyIndReqStatus = EvReadyIndReqStatus::OFF;
    EvReadyIndReqStatusForEv mEvReadyIndReqStatusForEv = EvReadyIndReqStatusForEv::OFF;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
};

}  // namespace ccos

#endif  // SFSS_xEV_System_Ready_H
