/**
 * @file Transmission_Warning.hpp
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
#ifndef SFSS_Transmission_Warning_H
#define SFSS_Transmission_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Transmission_WarningBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [PT430] Transmission_Warning. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class Transmission_Warning : public Transmission_WarningBase {
public:
    Transmission_Warning() = default;
    ~Transmission_Warning() override = default;
    Transmission_Warning(const Transmission_Warning& other) = delete;
    Transmission_Warning(Transmission_Warning&& other) noexcept = delete;
    Transmission_Warning& operator=(const Transmission_Warning& other) = delete;
    Transmission_Warning& operator=(Transmission_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTransmission_WarningEventHighTempWarningLimpHomeID("E24101");
        setSFCTransmission_WarningEventHighTempWarningOverHeatedID("E24102");
        setSFCTransmission_WarningEventHighTempWarningCoolingCompletedID("E24103");
        setSFCTransmission_WarningEventHighTempWarningHighTemperatureID("E24104");
        setSFCTransmission_WarningEventHighTempWarningCoolingID("E24105");
        setSFCTransmission_WarningEventHighTempWarningHillHoldID("E24106");
        setSFCTransmission_WarningEventHighTempWarningImpossibleToDriveID("E24107");
        setSFCTransmission_WarningEventTransmissionPowerLimitedID("E24108");
        setSFCTransmission_WarningEventTransmissionPowerLimitedLinkedSoundID("SND_PopUpInform2");
        setSFCTransmission_WarningEventTransmissionPowerLimitedLinkedSoundType(
            SFCTransmission_WarningEventTransmissionPowerLimitedLinkedSoundType::REPEAT_COUNT);
        setSFCTransmission_WarningEventTransmissionPowerLimitedLinkedSoundRepeatCount(1);
        setSFCTransmission_WarningEventTransmissionSystemWarningID("E24109");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateConstantCoolingTime();
        updateEvent();
        updateSoundTransmissionHighTemp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateConstantCoolingTime();
        updateEvent();
        updateSoundTransmissionHighTemp();
    }

    void onVehicleStopTimeValueChanged(const ArgumentsVehicleStopTimeValueChanged& args) {
        DDebug() << "InputTransmissionVehicleStopTimeValue " << args.mInput_TransmissionVehicleStopTimeValue;
        mInput_TransmissionVehicleStopTimeValue = args.mInput_TransmissionVehicleStopTimeValue;
        mInput_TransmissionVehicleStopTimeValue_timeout = ISTIMEOUT(args.mInput_TransmissionVehicleStopTimeValue);
        updateConstantCoolingTime();
        updateEventTransmissionHighTempWarningCooling();
    }

    void onLimphomeWarnStatusInDCTChanged(const ArgumentsLimphomeWarnStatusInDCTChanged& args) {
        DDebug() << "InputTransmissionVehicleStopTimeValue "
                 << static_cast<HUInt64>(args.mInput_TransmissionLimphomeWarnStatusInDCT);
        mInput_TransmissionLimphomeWarnStatusInDCT = args.mInput_TransmissionLimphomeWarnStatusInDCT;
        updateEventTransmissionHighTempWarningLimpHome();
    }

    void onHighTempWarnStatusChanged(const ArgumentsHighTempWarnStatusChanged& args) {
        DDebug() << "Input_TransmissionHighTempWarnStatusInDCT1 "
                 << static_cast<HUInt64>(args.mInput_TransmissionHighTempWarnStatusInDCT1);
        DDebug() << "Input_TransmissionHighTempWarnStatusInDCT2 "
                 << static_cast<HUInt64>(args.mInput_TransmissionHighTempWarnStatusInDCT2);
        DDebug() << "Input_TransmissionHighTempWarnStatusInAT "
                 << static_cast<HUInt64>(args.mInput_TransmissionHighTempWarnStatusInAT);
        mInput_TransmissionHighTempWarnStatusInDCT1 = args.mInput_TransmissionHighTempWarnStatusInDCT1;
        mInput_TransmissionHighTempWarnStatusInDCT2 = args.mInput_TransmissionHighTempWarnStatusInDCT2;
        mInput_TransmissionHighTempWarnStatusInAT = args.mInput_TransmissionHighTempWarnStatusInAT;
        updateEventTransmissionHighTempWarningOverHeated();
        updateEventTransmissionHighTempWarningCoolingCompleted();
        updateEventTransmissionHighTempWarningHighTemperature();
        updateEventTransmissionHighTempWarningCooling();
    }

    void onHillHoldWarnStatusChanged(const ArgumentsHillHoldWarnStatusChanged& args) {
        DDebug() << "Input_TransmissionHillHoldWarnStatusInAT "
                 << static_cast<HUInt64>(args.mInput_TransmissionHillHoldWarnStatusInAT);
        DDebug() << "Input_TransmissionHillHoldWarnStatusInDCT "
                 << static_cast<HUInt64>(args.mInput_TransmissionHillHoldWarnStatusInDCT);
        mInput_TransmissionHillHoldWarnStatusInAT = args.mInput_TransmissionHillHoldWarnStatusInAT;
        mInput_TransmissionHillHoldWarnStatusInDCT = args.mInput_TransmissionHillHoldWarnStatusInDCT;
        updateEventTransmissionHighTempWarningHillHold();
    }

    void onEolErrorWarnStatusChanged(const ArgumentsEolErrorWarnStatusChanged& args) {
        DDebug() << "Input_TransmissionEolErrorWarnStatusFromTCU "
                 << static_cast<HUInt64>(args.mInput_TransmissionEolErrorWarnStatusFromTCU);
        mInput_TransmissionEolErrorWarnStatusFromTCU = args.mInput_TransmissionEolErrorWarnStatusFromTCU;
        updateEventTransmissionHighTempWarningImpossibleToDrive();
    }

    void onTransmissionPowerLimitedChanged(const ArgumentsTransmissionPowerLimitedChanged& args) {
        DDebug() << "Input_TransmissionPowerLimitStatus " << static_cast<HUInt64>(args.mInput_TransmissionPowerLimitStatus);
        mInput_TransmissionPowerLimitStatus = args.mInput_TransmissionPowerLimitStatus;
        updateEventTransmissionPowerLimited();
    }

    void onTransmissionWarnSoundStatusChanged(const ArgumentsTransmissionWarnSoundStatusChanged& args) {
        DDebug() << "Input_TransmissionWarnSoundStatusFromTCU "
                 << static_cast<HUInt64>(args.mInput_TransmissionWarnSoundStatusFromTCU);
        DDebug() << "Input_TransmissionWarnSoundStatusFromDCT "
                 << static_cast<HUInt64>(args.mInput_TransmissionWarnSoundStatusFromDCT);
        mInput_TransmissionWarnSoundStatusFromTCU = args.mInput_TransmissionWarnSoundStatusFromTCU;
        mInput_TransmissionWarnSoundStatusFromDCT = args.mInput_TransmissionWarnSoundStatusFromDCT;
        updateSoundTransmissionHighTemp();
    }

    void onTransmissionSysWarnStatusChanged(const ArgumentsTransmissionSysWarnStatusChanged& args) {
        DDebug() << "Input_TransmissionSysWarnStatus : " << static_cast<HUInt64>(args.mInput_TransmissionSysWarnStatus);
        mInput_TransmissionSysWarnStatus = args.mInput_TransmissionSysWarnStatus;
        updateEventTransmissionSystemWarning();
    }

private:
    void updateConstantCoolingTime() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms) {
            if (mInput_TransmissionVehicleStopTimeValue_timeout) {
                DDebug() << "timeout UNMEASURABLE";
                setSFCTransmission_WarningConstantCoolingTimeValue(mInput_TransmissionVehicleStopTimeValue);
                setSFCTransmission_WarningConstantCoolingTimeStat(SFCTransmission_WarningConstantCoolingTimeStat::UNMEASURABLE);
            } else {
                if (mInput_TransmissionVehicleStopTimeValue <= kTransmissionVehicleStopTimeValue) {
                    DDebug() << "updateCoolingTime VALID_DATA";
                    setSFCTransmission_WarningConstantCoolingTimeValue(mInput_TransmissionVehicleStopTimeValue);
                    setSFCTransmission_WarningConstantCoolingTimeStat(SFCTransmission_WarningConstantCoolingTimeStat::VALID_DATA);
                }
            }
        }
    }

    void updateEvent() {
        updateEventTransmissionHighTempWarningLimpHome();
        updateEventTransmissionHighTempWarningOverHeated();
        updateEventTransmissionHighTempWarningCoolingCompleted();
        updateEventTransmissionHighTempWarningHighTemperature();
        updateEventTransmissionHighTempWarningCooling();
        updateEventTransmissionHighTempWarningHillHold();
        updateEventTransmissionHighTempWarningImpossibleToDrive();
        updateEventTransmissionPowerLimited();
        updateEventTransmissionSystemWarning();
    }

    void updateEventTransmissionHighTempWarningLimpHome() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            mInput_TransmissionLimphomeWarnStatusInDCT == LimphomeWarnStatusInDCT::WARN) {
            setSFCTransmission_WarningEventHighTempWarningLimpHomeStat(
                SFCTransmission_WarningEventHighTempWarningLimpHomeStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningLimpHomeStat(
                SFCTransmission_WarningEventHighTempWarningLimpHomeStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningOverHeated() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            (mInput_TransmissionHighTempWarnStatusInAT == HighTempWarnStatusInAT::OVERHEAT_PARK ||
             mInput_TransmissionHighTempWarnStatusInDCT1 == HighTempWarnStatusInDCT1::OVERHEAT_PARK)) {
            setSFCTransmission_WarningEventHighTempWarningOverHeatedStat(
                SFCTransmission_WarningEventHighTempWarningOverHeatedStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningOverHeatedStat(
                SFCTransmission_WarningEventHighTempWarningOverHeatedStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningCoolingCompleted() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            (mInput_TransmissionHighTempWarnStatusInAT == HighTempWarnStatusInAT::OVERHEAT_RESUME ||
             mInput_TransmissionHighTempWarnStatusInDCT1 == HighTempWarnStatusInDCT1::OVERHEAT_RESUME)) {
            setSFCTransmission_WarningEventHighTempWarningCoolingCompletedStat(
                SFCTransmission_WarningEventHighTempWarningCoolingCompletedStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningCoolingCompletedStat(
                SFCTransmission_WarningEventHighTempWarningCoolingCompletedStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningHighTemperature() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            (mInput_TransmissionHighTempWarnStatusInAT == HighTempWarnStatusInAT::OVERHEAT ||
             mInput_TransmissionHighTempWarnStatusInDCT2 == HighTempWarnStatusInDCT2::OVERHEAT)) {
            setSFCTransmission_WarningEventHighTempWarningHighTemperatureStat(
                SFCTransmission_WarningEventHighTempWarningHighTemperatureStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningHighTemperatureStat(
                SFCTransmission_WarningEventHighTempWarningHighTemperatureStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningCooling() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            mInput_TransmissionVehicleStopTimeValue <= kTransmissionVehicleStopTimeValue &&
            (mInput_TransmissionHighTempWarnStatusInAT == HighTempWarnStatusInAT::COOLING ||
             mInput_TransmissionHighTempWarnStatusInDCT2 == HighTempWarnStatusInDCT2::COOLING)) {
            setSFCTransmission_WarningEventHighTempWarningCoolingStat(SFCTransmission_WarningEventHighTempWarningCoolingStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningCoolingStat(
                SFCTransmission_WarningEventHighTempWarningCoolingStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningHillHold() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            (mInput_TransmissionHillHoldWarnStatusInAT == HillHoldWarnStatusInAT::WARN ||
             mInput_TransmissionHillHoldWarnStatusInDCT == HillHoldWarnStatusInDCT::WARN)) {
            setSFCTransmission_WarningEventHighTempWarningHillHoldStat(
                SFCTransmission_WarningEventHighTempWarningHillHoldStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningHillHoldStat(
                SFCTransmission_WarningEventHighTempWarningHillHoldStat::OFF);
        }
    }

    void updateEventTransmissionHighTempWarningImpossibleToDrive() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms &&
            mInput_TransmissionEolErrorWarnStatusFromTCU == EolErrorWarnStatusFromTCU::EOL_ERROR) {
            setSFCTransmission_WarningEventHighTempWarningImpossibleToDriveStat(
                SFCTransmission_WarningEventHighTempWarningImpossibleToDriveStat::ON);
        } else {
            setSFCTransmission_WarningEventHighTempWarningImpossibleToDriveStat(
                SFCTransmission_WarningEventHighTempWarningImpossibleToDriveStat::OFF);
        }
    }

    void updateEventTransmissionPowerLimited() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms && mInput_TransmissionPowerLimitStatus == PowerLimitStatus::ON) {
            DDebug() << "updateTransmissionPowerLimited EventTransmissionPowerLimitedStat::ON";
            setSFCTransmission_WarningEventTransmissionPowerLimitedStat(
                SFCTransmission_WarningEventTransmissionPowerLimitedStat::ON);
        } else {
            DDebug() << "updateTransmissionPowerLimited EventTransmissionPowerLimitedStat::OFF";
            setSFCTransmission_WarningEventTransmissionPowerLimitedStat(
                SFCTransmission_WarningEventTransmissionPowerLimitedStat::OFF);
        }
    }

    void updateEventTransmissionSystemWarning() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms && mInput_TransmissionSysWarnStatus == SysWarnStatus::ON) {
            setSFCTransmission_WarningEventTransmissionSystemWarningStat(
                SFCTransmission_WarningEventTransmissionSystemWarningStat::ON);
        } else {
            setSFCTransmission_WarningEventTransmissionSystemWarningStat(
                SFCTransmission_WarningEventTransmissionSystemWarningStat::OFF);
        }
    }

    void updateSoundTransmissionHighTemp() {
        std::string soundID;
        HUInt64 repeatCount = 0;
        SFCTransmission_WarningSoundTransmissionHighTempType type = SFCTransmission_WarningSoundTransmissionHighTempType::NONE;
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms) {
            if (mInput_TransmissionWarnSoundStatusFromTCU == WarnSoundStatusFromTCU::WARN1 ||
                mInput_TransmissionWarnSoundStatusFromDCT == WarnSoundStatusFromDCT::WARN1) {
                DDebug() << "onTransmissionWarnSoundStatusChanged WARN1";
                soundID = "SND_TransmissionHighTemp";
                repeatCount = 3;
                type = SFCTransmission_WarningSoundTransmissionHighTempType::REPEAT_COUNT;
            } else if (mInput_TransmissionWarnSoundStatusFromTCU == WarnSoundStatusFromTCU::WARN2 ||
                       mInput_TransmissionWarnSoundStatusFromDCT == WarnSoundStatusFromDCT::WARN2) {
                DDebug() << "onTransmissionWarnSoundStatusChanged WARN2";
                soundID = "SND_TransmissionHighTemp";
                repeatCount = 0;
                type = SFCTransmission_WarningSoundTransmissionHighTempType::INFINITE;
            } else {
                // NONE
            }
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.Transmission_Warning.Sound.TransmissionHighTemp.ID, prevSoundID);
        if (!prevSoundID.empty() && prevSoundID != soundID) {
            setSFCTransmission_WarningSoundTransmissionHighTempStat(SFCTransmission_WarningSoundTransmissionHighTempStat::OFF);
            flushLastGroup();
        }
        if (!soundID.empty()) {
            setSFCTransmission_WarningSoundTransmissionHighTempID(soundID);
            setSFCTransmission_WarningSoundTransmissionHighTempStat(SFCTransmission_WarningSoundTransmissionHighTempStat::ON);
            setSFCTransmission_WarningSoundTransmissionHighTempRepeatCount(repeatCount);
            setSFCTransmission_WarningSoundTransmissionHighTempType(type);
        }
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using LimphomeWarnStatusInDCT = ArgumentsLimphomeWarnStatusInDCTChanged::Input_TransmissionLimphomeWarnStatusInDCT;
    using HighTempWarnStatusInDCT1 = ArgumentsHighTempWarnStatusChanged::Input_TransmissionHighTempWarnStatusInDCT1;
    using HighTempWarnStatusInDCT2 = ArgumentsHighTempWarnStatusChanged::Input_TransmissionHighTempWarnStatusInDCT2;
    using HighTempWarnStatusInAT = ArgumentsHighTempWarnStatusChanged::Input_TransmissionHighTempWarnStatusInAT;
    using HillHoldWarnStatusInAT = ArgumentsHillHoldWarnStatusChanged::Input_TransmissionHillHoldWarnStatusInAT;
    using HillHoldWarnStatusInDCT = ArgumentsHillHoldWarnStatusChanged::Input_TransmissionHillHoldWarnStatusInDCT;
    using EolErrorWarnStatusFromTCU = ArgumentsEolErrorWarnStatusChanged::Input_TransmissionEolErrorWarnStatusFromTCU;
    using PowerLimitStatus = ArgumentsTransmissionPowerLimitedChanged::Input_TransmissionPowerLimitStatus;
    using WarnSoundStatusFromTCU = ArgumentsTransmissionWarnSoundStatusChanged::Input_TransmissionWarnSoundStatusFromTCU;
    using WarnSoundStatusFromDCT = ArgumentsTransmissionWarnSoundStatusChanged::Input_TransmissionWarnSoundStatusFromDCT;
    using SysWarnStatus = ArgumentsTransmissionSysWarnStatusChanged::Input_TransmissionSysWarnStatus;

    static constexpr HUInt64 kTransmissionVehicleStopTimeValue = 31;
    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    HUInt64 mInput_TransmissionVehicleStopTimeValue = false;
    HBool mInput_TransmissionVehicleStopTimeValue_timeout = 0;
    PowerLimitStatus mInput_TransmissionPowerLimitStatus = PowerLimitStatus::OFF;
    LimphomeWarnStatusInDCT mInput_TransmissionLimphomeWarnStatusInDCT = LimphomeWarnStatusInDCT::OFF;
    HighTempWarnStatusInDCT1 mInput_TransmissionHighTempWarnStatusInDCT1 = HighTempWarnStatusInDCT1::OFF;
    HighTempWarnStatusInDCT2 mInput_TransmissionHighTempWarnStatusInDCT2 = HighTempWarnStatusInDCT2::OFF;
    HighTempWarnStatusInAT mInput_TransmissionHighTempWarnStatusInAT = HighTempWarnStatusInAT::OFF;
    HillHoldWarnStatusInAT mInput_TransmissionHillHoldWarnStatusInAT = HillHoldWarnStatusInAT::OFF;
    HillHoldWarnStatusInDCT mInput_TransmissionHillHoldWarnStatusInDCT = HillHoldWarnStatusInDCT::OFF;
    EolErrorWarnStatusFromTCU mInput_TransmissionEolErrorWarnStatusFromTCU = EolErrorWarnStatusFromTCU::NO_WARN;
    WarnSoundStatusFromTCU mInput_TransmissionWarnSoundStatusFromTCU = WarnSoundStatusFromTCU::OFF;
    WarnSoundStatusFromDCT mInput_TransmissionWarnSoundStatusFromDCT = WarnSoundStatusFromDCT::OFF;
    SysWarnStatus mInput_TransmissionSysWarnStatus = SysWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Transmission_Warning_H
