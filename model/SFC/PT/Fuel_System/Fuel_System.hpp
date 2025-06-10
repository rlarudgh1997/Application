/**
 * @file Fuel_System.hpp
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
#ifndef SFSS_Fuel_System_H
#define SFSS_Fuel_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Fuel_SystemBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 19.0.0
// Reference : [PT190] Fuel System. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Fuel_System : public Fuel_SystemBase {
public:
    Fuel_System() = default;
    ~Fuel_System() override = default;
    Fuel_System(const Fuel_System& other) = delete;
    Fuel_System(Fuel_System&& other) noexcept = delete;
    Fuel_System& operator=(const Fuel_System& other) = delete;
    Fuel_System& operator=(Fuel_System&& other) noexcept = delete;

    void onInitialize() override {
        setSFCFuel_SystemEventLowFuelRefuelFCEVID("E24701");
        setSFCFuel_SystemEventLowFuelRefuelFCEVLinkedSoundID("SND_PopUpInform2");
        setSFCFuel_SystemEventLowFuelRefuelFCEVLinkedSoundType(SFCFuel_SystemEventLowFuelRefuelFCEVLinkedSoundType::REPEAT_COUNT);
        setSFCFuel_SystemEventLowFuelRefuelFCEVLinkedSoundRepeatCount(1);

        setSFCFuel_SystemEventH2TankFillCountOverWarnID("E24707");
        setSFCFuel_SystemEventH2TankFillCountOverWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCFuel_SystemEventH2TankFillCountOverWarnLinkedSoundType(
            SFCFuel_SystemEventH2TankFillCountOverWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFuel_SystemEventH2TankFillCountOverWarnLinkedSoundRepeatCount(1);

        setSFCFuel_SystemEventFcevFuelDrWarnLinkedSoundType(SFCFuel_SystemEventFcevFuelDrWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFuel_SystemEventFcevFuelDrWarnLinkedSoundRepeatCount(1);

        setSFCFuel_SystemEventFcevFuelDrSwLinkedSoundType(SFCFuel_SystemEventFcevFuelDrSwLinkedSoundType::REPEAT_COUNT);
        setSFCFuel_SystemEventFcevFuelDrSwLinkedSoundRepeatCount(1);

        setSFCFuel_SystemEventFcevFuelDoorSensorFaultWarnID("E24712");
        setSFCFuel_SystemEventFcevFuelDoorSensorFaultWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCFuel_SystemEventFcevFuelDoorSensorFaultWarnLinkedSoundType(
            SFCFuel_SystemEventFcevFuelDoorSensorFaultWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFuel_SystemEventFcevFuelDoorSensorFaultWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateAll();
    }

    void onInputFcevLowFuelWarnStatusChanged(const ArgumentsInputFcevLowFuelWarnStatusChanged& args) {
        mFcevLowFuelWarnStatus = args.mInput_FcevLowFuelWarnStatus;
        updateTelltaleLowFuelWarnFCEV();
        updateEventLowFuelRefuelFCEV();
        updateOutputRefuelWarnStatus();
        flushActuatorOutput();
    }

    void onInputH2TankSystemWarnStatusChanged(const ArgumentsInputH2TankSystemWarnStatusChanged& args) {
        mH2TankSystemWarnStatus = args.mInput_H2TankSystemWarnStatus;
        updateEventH2TankSystemWarn();
    }

    void onInputH2TankFillCountOverWarnStatusChanged(const ArgumentsInputH2TankFillCountOverWarnStatusChanged& args) {
        mH2TankFillCountOverWarnStatus = args.mInput_H2TankFillCountOverWarnStatus;
        updateEventH2TankFillCountOverWarn();
    }

    void onInputFcevFuelDrWarnStatusChanged(const ArgumentsInputFcevFuelDrWarnStatusChanged& args) {
        mFcevFuelDrWarnStatus = args.mInput_FcevFuelDrWarnStatus;
        updateEventFcevFuelDrWarn();
    }

    void onInputFcevFuelDrStatusChanged(const ArgumentsInputFcevFuelDrStatusChanged& args) {
        mFcevFuelDrSwStatus = args.mInput_FcevFuelDrSwStatus;
        mFcevFuelDrUnableStatus = args.mInput_FcevFuelDrUnableStatus;
        mFcevSystemOffStatus = args.mInput_FcevSystemOffStatus;
        updateEventFcevFuelDrSwStatus();
    }

    void onInputFcevFuelDoorSensorFaultWarnStatusChanged(const ArgumentsInputFcevFuelDoorSensorFaultWarnStatusChanged& args) {
        mFcevFuelDoorSensorFaultWarnStatus = args.mInput_FcevFuelDoorSensorFaultWarnStatus;
        updateEventFcevFuelDoorSensorFaultWarn();
    }

    void onFcevSofDisplayValueChanged(const ArgumentsFcevSofDisplayValueChanged& args) {
        mFcevSofDisplayValue = static_cast<HDouble>(args.mInput_FcevSofDisplayValue);
        updateConstantFCEVFuelGauge();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateTelltaleLowFuelWarnFCEV();
        updateConstantFCEVFuelGauge();
        updateEventLowFuelRefuelFCEV();
        updateOutputRefuelWarnStatus();
        updateEventH2TankSystemWarn();
        updateEventH2TankFillCountOverWarn();
        updateEventFcevFuelDrWarn();
        updateEventFcevFuelDrSwStatus();
        updateEventFcevFuelDoorSensorFaultWarn();
        flushActuatorOutput();
    }

    // 4.1.1 연료부족 상태 신호 송출(FCEV)
    void updateOutputRefuelWarnStatus() {
        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if (mTelltaleStat == SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::ON) {
                updateActuatorValue(Vehicle.PT.Fuel_System.Output_RefuelWarnStatus,
                                    static_cast<HUInt64>(Output_RefuelWarnStatus::ON));
            } else if (mTelltaleStat == SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::OFF) {
                updateActuatorValue(Vehicle.PT.Fuel_System.Output_RefuelWarnStatus,
                                    static_cast<HUInt64>(Output_RefuelWarnStatus::OFF));
            } else {
                // Do Nothing
            }
        }
    }

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (!mActuatorOutputAddrList.empty()) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    // 5.1.1 FUEL WARNING(FCEV)
    void updateTelltaleLowFuelWarnFCEV() {
        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnitionElapsed == IgnitionElapsed::ON_500ms &&
            mFcevLowFuelWarnStatus == FcevLowFuelWarnStatus::WARN) {
            mTelltaleStat = SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::ON;
        } else {
            mTelltaleStat = SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::OFF;
        }
        setSFCFuel_SystemTelltaleLowFuelWarnFCEVStat(mTelltaleStat);
    }

    void updateConstantFCEVFuelGauge() {
        HDouble fcevFuelGaugeValue = 0.0;  // mFcevSofDisplayValue == MESSAGE TIME OUT 경우도 동일

        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if (kFcevSofDisplayValueEmpty <= mFcevSofDisplayValue && mFcevSofDisplayValue <= kFcevSofDisplayValueFull) {
                fcevFuelGaugeValue = mFcevSofDisplayValue * kFcevSofDisplayValueFactor;
            }
        }

        setSFCFuel_SystemConstantFCEVFuelgaugeValue(fcevFuelGaugeValue);
    }

    // 5.3.2 LOW FUEL, REFUEL(FCEV)
    void updateEventLowFuelRefuelFCEV() {
        SFCFuel_SystemEventLowFuelRefuelFCEVStat stat = SFCFuel_SystemEventLowFuelRefuelFCEVStat::OFF;
        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnitionElapsed >= IgnitionElapsed::ON_0ms &&
            mTelltaleStat == SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::ON) {
            stat = SFCFuel_SystemEventLowFuelRefuelFCEVStat::ON;
        }
        setSFCFuel_SystemEventLowFuelRefuelFCEVStat(stat);
    }

    // 5.3.3 (1) Event 수소 탱크 경고(FCEV)
    void updateEventH2TankSystemWarn() {
        std::string eventID;
        std::string soundID;
        SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
                switch (mH2TankSystemWarnStatus) {
                    case H2TankSystemWarnStatus::OVERFILL:
                        DDebug() << "Event On E24703";
                        eventID = "E24703";
                        soundID = "SND_PopUpWarn1";
                        soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::REPEAT_COUNT;
                        repeatCount = 1;
                        break;
                    case H2TankSystemWarnStatus::H2_LEAK:
                        DDebug() << "Event On E24704";
                        eventID = "E24704";
                        soundID = "SND_PopUpWarn1";
                        soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::REPEAT_COUNT;
                        repeatCount = 1;
                        break;
                    case H2TankSystemWarnStatus::OVERCOOL:
                        DDebug() << "Event On E24705";
                        eventID = "E24705";
                        soundID = "SND_PopUpWarn1";
                        soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::REPEAT_COUNT;
                        repeatCount = 1;
                        break;
                    case H2TankSystemWarnStatus::TEMP_ERROR:
                        DDebug() << "Event On E24706";
                        eventID = "E24706";
                        soundID = "SND_PopUpWarn2";
                        soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::INFINITE;
                        break;
                    default:
                        // Event OFF
                        break;
                }
            } else {
                if (mH2TankSystemWarnStatus == H2TankSystemWarnStatus::TEMP_ERROR) {
                    DDebug() << "Event On E24706";
                    eventID = "E24706";
                    soundID = "SND_PopUpWarn2";
                    soundType = SFCFuel_SystemEventH2TankSystemWarnLinkedSoundType::DURATION_MINMAX;
                    setSFCFuel_SystemEventH2TankSystemWarnLinkedSoundDurationMin(kEventDurationMin);
                    setSFCFuel_SystemEventH2TankSystemWarnLinkedSoundDurationMax(kEventDurationMax);
                }
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Fuel_System.Event.H2TankSystemWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFuel_SystemEventH2TankSystemWarnStat(SFCFuel_SystemEventH2TankSystemWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFuel_SystemEventH2TankSystemWarnID(eventID);
            setSFCFuel_SystemEventH2TankSystemWarnStat(SFCFuel_SystemEventH2TankSystemWarnStat::ON);
            setSFCFuel_SystemEventH2TankSystemWarnLinkedSoundID(soundID);
            setSFCFuel_SystemEventH2TankSystemWarnLinkedSoundType(soundType);
            setSFCFuel_SystemEventH2TankSystemWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.3.3 (2) Event 수소 탱크 경고(FCEV)
    void updateEventH2TankFillCountOverWarn() {
        SFCFuel_SystemEventH2TankFillCountOverWarnStat stat = SFCFuel_SystemEventH2TankFillCountOverWarnStat::OFF;
        if (mConfigVehicleType == ConfigVehicleType::FCEV &&
            mH2TankFillCountOverWarnStatus == H2TankFillCountOverWarnStatus::WARN &&
            mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            stat = SFCFuel_SystemEventH2TankFillCountOverWarnStat::ON;
        }
        setSFCFuel_SystemEventH2TankFillCountOverWarnStat(stat);
    }

    // 5.3.4 (1) Event 연료 도어 경고(FCEV)
    void updateEventFcevFuelDrWarn() {
        std::string eventID;
        std::string soundID;
        if (mConfigVehicleType == ConfigVehicleType::FCEV && mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if (mFcevFuelDrWarnStatus == FcevFuelDrWarnStatus::OPEN) {
                DDebug() << "Event On E24708";
                eventID = "E24708";
                soundID = "SND_PopUpWarn1";
            } else if (mFcevFuelDrWarnStatus == FcevFuelDrWarnStatus::CHARGING) {
                DDebug() << "Event On E24709";
                eventID = "E24709";
                soundID = "SND_PopUpInform2";
            } else {
                // Event OFF
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Fuel_System.Event.FcevFuelDrWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFuel_SystemEventFcevFuelDrWarnStat(SFCFuel_SystemEventFcevFuelDrWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFuel_SystemEventFcevFuelDrWarnID(eventID);
            setSFCFuel_SystemEventFcevFuelDrWarnStat(SFCFuel_SystemEventFcevFuelDrWarnStat::ON);
            setSFCFuel_SystemEventFcevFuelDrWarnLinkedSoundID(soundID);
        }
    }

    // 5.3.4 (2) Event 연료 도어 경고(FCEV)
    void updateEventFcevFuelDrSwStatus() {
        std::string eventID;
        std::string soundID;
        if (mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mFcevFuelDrSwStatus == FcevFuelDrSwStatus::ON && mFcevFuelDrUnableStatus == FcevFuelDrUnableStatus::UNABLE &&
                mFcevSystemOffStatus == FcevSystemOffStatus::SYSTEM_OFF) {
                DDebug() << "Event On E24711";
                eventID = "E24711";
                soundID = "SND_PopUpInform2";
            } else if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms && mFcevFuelDrSwStatus == FcevFuelDrSwStatus::ON &&
                       mFcevFuelDrUnableStatus == FcevFuelDrUnableStatus::UNABLE &&
                       mFcevSystemOffStatus == FcevSystemOffStatus::NONE) {
                DDebug() << "Event On E24710";
                eventID = "E24710";
                soundID = "SND_PopUpWarn1";
            } else {
                // Event OFF
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Fuel_System.Event.FcevFuelDrSw.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFuel_SystemEventFcevFuelDrSwStat(SFCFuel_SystemEventFcevFuelDrSwStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFuel_SystemEventFcevFuelDrSwID(eventID);
            setSFCFuel_SystemEventFcevFuelDrSwStat(SFCFuel_SystemEventFcevFuelDrSwStat::ON);
            setSFCFuel_SystemEventFcevFuelDrSwLinkedSoundID(soundID);
        }
    }

    // 5.3.4 (3) Event 연료 도어 경고(FCEV)
    void updateEventFcevFuelDoorSensorFaultWarn() {
        SFCFuel_SystemEventFcevFuelDoorSensorFaultWarnStat stat = SFCFuel_SystemEventFcevFuelDoorSensorFaultWarnStat::OFF;
        if (mConfigVehicleType == ConfigVehicleType::FCEV &&
            mFcevFuelDoorSensorFaultWarnStatus == FcevFuelDoorSensorFaultWarnStatus::FAULT &&
            mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            stat = SFCFuel_SystemEventFcevFuelDoorSensorFaultWarnStat::ON;
        }
        setSFCFuel_SystemEventFcevFuelDoorSensorFaultWarnStat(stat);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms
    };
    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    using Output_RefuelWarnStatus = decltype(Vehicle.PT.Fuel_System.Output_RefuelWarnStatus)::TYPE;
    AddressValueList mActuatorOutputAddrList;
    using FcevLowFuelWarnStatus = ArgumentsInputFcevLowFuelWarnStatusChanged::Input_FcevLowFuelWarnStatus;
    FcevLowFuelWarnStatus mFcevLowFuelWarnStatus = FcevLowFuelWarnStatus::OFF;
    using H2TankSystemWarnStatus = ArgumentsInputH2TankSystemWarnStatusChanged::Input_H2TankSystemWarnStatus;
    H2TankSystemWarnStatus mH2TankSystemWarnStatus = H2TankSystemWarnStatus::OFF;
    using H2TankFillCountOverWarnStatus = ArgumentsInputH2TankFillCountOverWarnStatusChanged::Input_H2TankFillCountOverWarnStatus;
    H2TankFillCountOverWarnStatus mH2TankFillCountOverWarnStatus = H2TankFillCountOverWarnStatus::OFF;
    using FcevFuelDrWarnStatus = ArgumentsInputFcevFuelDrWarnStatusChanged::Input_FcevFuelDrWarnStatus;
    FcevFuelDrWarnStatus mFcevFuelDrWarnStatus = FcevFuelDrWarnStatus::OFF;
    using FcevFuelDrSwStatus = ArgumentsInputFcevFuelDrStatusChanged::Input_FcevFuelDrSwStatus;
    using FcevFuelDrUnableStatus = ArgumentsInputFcevFuelDrStatusChanged::Input_FcevFuelDrUnableStatus;
    using FcevSystemOffStatus = ArgumentsInputFcevFuelDrStatusChanged::Input_FcevSystemOffStatus;
    FcevFuelDrSwStatus mFcevFuelDrSwStatus = FcevFuelDrSwStatus::OFF;
    FcevFuelDrUnableStatus mFcevFuelDrUnableStatus = FcevFuelDrUnableStatus::NONE;
    FcevSystemOffStatus mFcevSystemOffStatus = FcevSystemOffStatus::NONE;
    using FcevFuelDoorSensorFaultWarnStatus =
        ArgumentsInputFcevFuelDoorSensorFaultWarnStatusChanged::Input_FcevFuelDoorSensorFaultWarnStatus;
    FcevFuelDoorSensorFaultWarnStatus mFcevFuelDoorSensorFaultWarnStatus = FcevFuelDoorSensorFaultWarnStatus::OFF;
    SFCFuel_SystemTelltaleLowFuelWarnFCEVStat mTelltaleStat = SFCFuel_SystemTelltaleLowFuelWarnFCEVStat::OFF;

    static constexpr HUInt64 kEventDurationMin = 0;
    static constexpr HUInt64 kEventDurationMax = 10000;

    HDouble mFcevSofDisplayValue = 0.0;
    static constexpr HDouble kFcevSofDisplayValueFactor = 0.1;
    static constexpr HDouble kFcevSofDisplayValueEmpty = 0.0;
    static constexpr HDouble kFcevSofDisplayValueFull = 1000.0;
};

}  // namespace ccos

#endif  // SFSS_Fuel_System_H
