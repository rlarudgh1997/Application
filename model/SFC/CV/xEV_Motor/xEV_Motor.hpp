/**
 * @file xEV_Motor.hpp
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
#ifndef SFSS_xEV_Motor_H
#define SFSS_xEV_Motor_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_MotorBase.hpp"

namespace ccos {

// SFC Version : 3.0.1
// Spec Version : v0.34
// Reference : [CV870] xEV_Motor. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class xEV_Motor : public xEV_MotorBase {
public:
    xEV_Motor() = default;
    ~xEV_Motor() override = default;
    xEV_Motor(const xEV_Motor& other) = delete;
    xEV_Motor(xEV_Motor&& other) noexcept = delete;
    xEV_Motor& operator=(const xEV_Motor& other) = delete;
    xEV_Motor& operator=(xEV_Motor&& other) noexcept = delete;

    void onInitialize() override {
        setSFCXEV_MotorEventMotorPowerLimitFromMCUID("E78701");
        setSFCXEV_MotorEventMotorPowerLimitFromMCULinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_MotorEventMotorPowerLimitFromMCULinkedSoundType(
            SFCXEV_MotorEventMotorPowerLimitFromMCULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_MotorEventMotorPowerLimitFromMCULinkedSoundRepeatCount(1);

        setSFCXEV_MotorEventSlowDownFromMCUID("E78702");
        setSFCXEV_MotorEventSlowDownFromMCULinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_MotorEventSlowDownFromMCULinkedSoundType(SFCXEV_MotorEventSlowDownFromMCULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_MotorEventSlowDownFromMCULinkedSoundRepeatCount(1);

        // E78703 ~ E78704
        setSFCXEV_MotorEventIDCULinkedSoundType(SFCXEV_MotorEventIDCULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_MotorEventIDCULinkedSoundRepeatCount(1);

        setSFCXEV_MotorEventMCUResolverFailPopupStatusID("E78705");
        setSFCXEV_MotorEventMCUResolverFailPopupStatusLinkedSoundID("SND_PopUpInform2");
        setSFCXEV_MotorEventMCUResolverFailPopupStatusLinkedSoundType(
            SFCXEV_MotorEventMCUResolverFailPopupStatusLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_MotorEventMCUResolverFailPopupStatusLinkedSoundRepeatCount(1);

        // E78706, E78707
        setSFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundID("SND_PopUpInform2");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateMotorPowerLimitFromMCUEvent();
        updateSlowDownFromMCUEvent();
        updateEventIDCU();
        updateEventMCUResolverFailPopup();
        updateEventShiftLeverPositionAcceptance();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleIDCU();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateMotorPowerLimitFromMCUEvent();
        updateSlowDownFromMCUEvent();
        updateEventIDCU();
        updateTelltaleIDCU();
        updateEventMCUResolverFailPopup();
        updateEventShiftLeverPositionAcceptance();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateMotorPowerLimitFromMCUEvent();
        updateSlowDownFromMCUEvent();
        updateEventIDCU();
        updateTelltaleIDCU();
        updateEventMCUResolverFailPopup();
        updateEventShiftLeverPositionAcceptance();
    }

    void onMotorPowerLimitFromMCUChanged(const ArgumentsMotorPowerLimitFromMCUChanged& args) {
        mMotorPowerLimitFromMCU = args.mInput_MotorPowerLimitFromMCU;
        updateMotorPowerLimitFromMCUEvent();
    }

    void onSlowDownFromMCUChanged(const ArgumentsSlowDownFromMCUChanged& args) {
        mSlowDownFromMCU = args.mInput_SlowDownFromMCU;
        updateSlowDownFromMCUEvent();
    }

    void onMcuAmberLampStatusChanged(const ArgumentsMcuAmberLampStatusChanged& args) {
        mMCUAmberLampStatus = args.mInput_MCUAmberLampStatus;
        updateEventIDCU();
    }

    void onMcuRedLampStatusChanged(const ArgumentsMcuRedLampStatusChanged& args) {
        mMCURedLampStatus = args.mInput_MCURedLampStatus;
        updateEventIDCU();
        updateTelltaleIDCU();
    }

    void onMcuResolverFailPopupStatusChanged(const ArgumentsMcuResolverFailPopupStatusChanged& args) {
        mMCUResolverFailPopupStatus = args.mInput_MCUResolverFailPopupStatus;
        updateEventMCUResolverFailPopup();
    }

    void onShiftLeverPositionAcceptanceChanged(const ArgumentsShiftLeverPositionAcceptanceChanged& args) {
        mShiftLeverPositionAcceptance = args.mInput_ShiftLeverPositionAcceptance;
        updateEventShiftLeverPositionAcceptance();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleIDCU() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_MotorTelltaleIDCUFaultStat stat = SFCXEV_MotorTelltaleIDCUFaultStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms && mMCURedLampStatus == MCURedLampStatus::ON) {
            stat = SFCXEV_MotorTelltaleIDCUFaultStat::ON;
        }
        setSFCXEV_MotorTelltaleIDCUFaultStat(stat);
    }

    void updateMotorPowerLimitFromMCUEvent() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCXEV_MotorEventMotorPowerLimitFromMCUStat stat = SFCXEV_MotorEventMotorPowerLimitFromMCUStat::OFF;

        if (mIsIgnOn == true && mMotorPowerLimitFromMCU == MotorPowerLimitFromMCU::ON) {
            stat = SFCXEV_MotorEventMotorPowerLimitFromMCUStat::ON;
        }

        setSFCXEV_MotorEventMotorPowerLimitFromMCUStat(stat);
    }

    void updateSlowDownFromMCUEvent() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCXEV_MotorEventSlowDownFromMCUStat stat = SFCXEV_MotorEventSlowDownFromMCUStat::OFF;

        if (mIsIgnOn == true && mSlowDownFromMCU == SlowDownFromMCU::ON) {
            stat = SFCXEV_MotorEventSlowDownFromMCUStat::ON;
        }

        setSFCXEV_MotorEventSlowDownFromMCUStat(stat);
    }

    void updateEventIDCU() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;

        if (mIsIgnOn == true) {
            if (mMCURedLampStatus == MCURedLampStatus::ON) {
                eventID = "E78704";
                linkedSoundID = "SND_PopUpWarn2";
            } else if (mMCUAmberLampStatus == MCUAmberLampStatus::ON && mMCURedLampStatus != MCURedLampStatus::ON) {
                eventID = "E78703";
                linkedSoundID = "SND_PopUpWarn1";
            } else {
                // no opeation
            }
        } else {
            // no opeation
        }

        GETCACHEDVALUE(SFC.xEV_Motor.Event.IDCU.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCXEV_MotorEventIDCUStat(SFCXEV_MotorEventIDCUStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCXEV_MotorEventIDCUID(eventID);
            setSFCXEV_MotorEventIDCULinkedSoundID(linkedSoundID);
            setSFCXEV_MotorEventIDCUStat(SFCXEV_MotorEventIDCUStat::ON);
        }
    }

    void updateEventMCUResolverFailPopup() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCXEV_MotorEventMCUResolverFailPopupStatusStat stat = SFCXEV_MotorEventMCUResolverFailPopupStatusStat::OFF;

        if (mIsIgnOn == true && mMCUResolverFailPopupStatus == MCUResolverFailPopupStatus::ON) {
            stat = SFCXEV_MotorEventMCUResolverFailPopupStatusStat::ON;
        }

        setSFCXEV_MotorEventMCUResolverFailPopupStatusStat(stat);
    }

    void updateEventShiftLeverPositionAcceptance() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        std::string eventID;
        std::string prevEventID;
        SFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundType eventLinkedSoundType =
            SFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            if (mShiftLeverPositionAcceptance == ShiftLeverPositionAcceptance::COND_1) {
                eventID = "E78706";
                eventLinkedSoundType = SFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundType::INFINITE;
            } else if (mShiftLeverPositionAcceptance == ShiftLeverPositionAcceptance::COND_2 ||
                       mShiftLeverPositionAcceptance == ShiftLeverPositionAcceptance::COND_3) {
                eventID = "E78707";
                eventLinkedSoundType = SFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.xEV_Motor.Event.ShiftLeverPositionAcceptance.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCXEV_MotorEventShiftLeverPositionAcceptanceStat(SFCXEV_MotorEventShiftLeverPositionAcceptanceStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCXEV_MotorEventShiftLeverPositionAcceptanceID(eventID);
            setSFCXEV_MotorEventShiftLeverPositionAcceptanceStat(SFCXEV_MotorEventShiftLeverPositionAcceptanceStat::ON);
            setSFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundType(eventLinkedSoundType);
            setSFCXEV_MotorEventShiftLeverPositionAcceptanceLinkedSoundRepeatCount(repeatCount);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using MotorPowerLimitFromMCU = ArgumentsMotorPowerLimitFromMCUChanged::Input_MotorPowerLimitFromMCU;
    using SlowDownFromMCU = ArgumentsSlowDownFromMCUChanged::Input_SlowDownFromMCU;
    using MCURedLampStatus = ArgumentsMcuRedLampStatusChanged::Input_MCURedLampStatus;
    using MCUAmberLampStatus = ArgumentsMcuAmberLampStatusChanged::Input_MCUAmberLampStatus;
    using MCUResolverFailPopupStatus = ArgumentsMcuResolverFailPopupStatusChanged::Input_MCUResolverFailPopupStatus;
    using ShiftLeverPositionAcceptance = ArgumentsShiftLeverPositionAcceptanceChanged::Input_ShiftLeverPositionAcceptance;

    HBool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    MotorPowerLimitFromMCU mMotorPowerLimitFromMCU = MotorPowerLimitFromMCU::OFF;
    SlowDownFromMCU mSlowDownFromMCU = SlowDownFromMCU::OFF;
    MCURedLampStatus mMCURedLampStatus = MCURedLampStatus::OFF;
    MCUAmberLampStatus mMCUAmberLampStatus = MCUAmberLampStatus::OFF;
    MCUResolverFailPopupStatus mMCUResolverFailPopupStatus = MCUResolverFailPopupStatus::OFF;
    ShiftLeverPositionAcceptance mShiftLeverPositionAcceptance = ShiftLeverPositionAcceptance::OFF;
};

}  // namespace ccos

#endif  // SFSS_xEV_Motor_H
