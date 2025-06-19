/**
 * @file FCEV_Fuel_System.hpp
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
#ifndef SFSS_FCEV_Fuel_System_H
#define SFSS_FCEV_Fuel_System_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "FCEV_Fuel_SystemBase.hpp"

namespace ccos {

// SFC Version : 8.0.0
// Spec Version : v0.30
// Reference : [CV690] FCEV_Fuel_System. Contains : Constant, Event. Recommend: VALUE_CHANGED
class FCEV_Fuel_System : public FCEV_Fuel_SystemBase {
public:
    FCEV_Fuel_System() = default;
    ~FCEV_Fuel_System() override = default;
    FCEV_Fuel_System(const FCEV_Fuel_System& other) = delete;
    FCEV_Fuel_System(FCEV_Fuel_System&& other) noexcept = delete;
    FCEV_Fuel_System& operator=(const FCEV_Fuel_System& other) = delete;
    FCEV_Fuel_System& operator=(FCEV_Fuel_System&& other) noexcept = delete;

    void onInitialize() override {
        setSFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventLowFuelID("E76901");
        setSFCFCEV_Fuel_SystemEventLowFuelLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventLowFuelLinkedSoundType(SFCFCEV_Fuel_SystemEventLowFuelLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventLowFuelLinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarnID("E76906");
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarnLinkedSoundType(
            SFCFCEV_Fuel_SystemEventTankFillCountOverWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarnLinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMUID("E76908");
        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMULinkedSoundID("SND_PopUpInform2");
        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMULinkedSoundType(
            SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMULinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMULinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnID("E76909");
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnLinkedSoundType(
            SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnLinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventLowFuel2ID("E76910");
        setSFCFCEV_Fuel_SystemEventLowFuel2LinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventLowFuel2LinkedSoundType(SFCFCEV_Fuel_SystemEventLowFuel2LinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventLowFuel2LinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarn2ID("E76915");
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarn2LinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarn2LinkedSoundType(
            SFCFCEV_Fuel_SystemEventTankFillCountOverWarn2LinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarn2LinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2LinkedSoundType(
            SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2LinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2LinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2ID("E76918");
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2LinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2LinkedSoundType(
            SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2LinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2LinkedSoundRepeatCount(1);

        setSFCFCEV_Fuel_SystemConstantFCGaugeBlinkValueA(SFCFCEV_Fuel_SystemConstantFCGaugeBlinkValueA::ON);
        setSFCFCEV_Fuel_SystemConstantFCGaugeBlinkValueB(SFCFCEV_Fuel_SystemConstantFCGaugeBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateConstantFCGaugeStat();
        updateConstantFCGaugeValue();
        updateEventLowFuel();
        updateEventH2TankSystemWarn();
        updateEventFuelDoorOpenFromHMU();
        updateEventDoorSensorFaultWarn();
        updateEventLowFuel2();
        updateEventH2TankSystemWarn2();
        updateEventFuelDoorOpenFromHMU2();
        updateEventDoorSensorFaultWarn2();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateConstantFCGaugeStat();
        updateConstantFCGaugeValue();
        updateEventLowFuel();
        updateEventH2TankSystemWarn();
        updateEventFuelDoorOpenFromHMU();
        updateEventDoorSensorFaultWarn();
        updateEventLowFuel2();
        updateEventH2TankSystemWarn2();
        updateEventFuelDoorOpenFromHMU2();
        updateEventDoorSensorFaultWarn2();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        updateConstantFCGaugeStat();
        updateConstantFCGaugeValue();
        updateEventLowFuel();
        updateEventH2TankSystemWarn();
        updateEventTankFillCountOverWarn();
        updateEventFuelDoorOpenFromHMU();
        updateEventDoorSensorFaultWarn();
        updateEventLowFuel2();
        updateEventH2TankSystemWarn2();
        updateEventTankFillCountOverWarn2();
        updateEventFuelDoorOpenFromHMU2();
        updateEventDoorSensorFaultWarn2();
    }

    void onLowFuelWarnStatusChanged(const ArgumentsLowFuelWarnStatusChanged& args) {
        mFcevLowFuelWarnStatus = args.mInput_FcevLowFuelWarnStatus;
        updateEventLowFuel();
    }

    void onSofDisplayValueChanged(const ArgumentsSofDisplayValueChanged& args) {
        mFcevSofDisplayValue = args.mInput_FcevSofDisplayValue;
        updateConstantFCGaugeValue();
    }

    void onEmptyStatusChanged(const ArgumentsEmptyStatusChanged& args) {
        mFcevFuelEmptyStatus = args.mInput_FcevFuelEmptyStatus;
        updateConstantFCGaugeStat();
        updateEventLowFuel();
    }

    void onTankSystemWarnStatusChanged(const ArgumentsTankSystemWarnStatusChanged& args) {
        mH2TankSystemWarnStatus = args.mInput_H2TankSystemWarnStatus;
        updateEventH2TankSystemWarn();
    }

    void onTankFillCountOverWarnStatusChanged(const ArgumentsTankFillCountOverWarnStatusChanged& args) {
        mH2TankFillCountOverWarnStatus = args.mInput_H2TankFillCountOverWarnStatus;
        updateEventTankFillCountOverWarn();
    }

    void onFuelDoorStatusFromHMUChanged(const ArgumentsFuelDoorStatusFromHMUChanged& args) {
        mFuelDoorStatusFromHMU = args.mInput_FuelDoorStatusFromHMU;
        updateEventFuelDoorOpenFromHMU();
    }

    void onDoorSensorFaultStatusChanged(const ArgumentsDoorSensorFaultStatusChanged& args) {
        mFcevFuelDoorSensorFaultWarnStatus = args.mInput_FcevFuelDoorSensorFaultWarnStatus;
        updateEventDoorSensorFaultWarn();
    }

    void onTankFillCountOverWarnStatus2Changed(const ArgumentsTankFillCountOverWarnStatus2Changed& args) {
        mH2TankFillCountOverWarnStatus2 = args.mInput_H2TankFillCountOverWarnStatus2;
        updateEventTankFillCountOverWarn2();
    }

    void onTankSystemWarnStatus2Changed(const ArgumentsTankSystemWarnStatus2Changed& args) {
        mH2TankSystemWarnStatus2 = args.mInput_H2TankSystemWarnStatus2;
        updateEventH2TankSystemWarn2();
    }

    void onFuelDoorStatusFromHMU2Changed(const ArgumentsFuelDoorStatusFromHMU2Changed& args) {
        mFuelDoorStatusFromHMU2 = args.mInput_FuelDoorStatusFromHMU2;
        updateEventFuelDoorOpenFromHMU2();
    }

    void onDoorSensorFaultStatus2Changed(const ArgumentsDoorSensorFaultStatus2Changed& args) {
        mFcevFuelDoorSensorFaultWarnStatus2 = args.mInput_FcevFuelDoorSensorFaultWarnStatus2;
        updateEventDoorSensorFaultWarn2();
    }

    void onEmptyStatus2Changed(const ArgumentsEmptyStatus2Changed& args) {
        mFcevFuelEmptyStatus2 = args.mInput_FcevFuelEmptyStatus2;
        mFcevLowFuelWarnStatus2 = args.mInput_FcevLowFuelWarnStatus2;
        updateEventLowFuel2();
    }

    void onConfigHMU2Changed(const ArgumentsConfigHMU2Changed& args) {
        mConfigHMU2 = args.mInter_ConfigHMU2;
        updateEventLowFuel2();
        updateEventH2TankSystemWarn2();
        updateEventTankFillCountOverWarn2();
        updateEventFuelDoorOpenFromHMU2();
        updateEventDoorSensorFaultWarn2();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantFCGaugeValue() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (kSOFValue_0h <= mFcevSofDisplayValue && mFcevSofDisplayValue <= kSOFValue_3E8h) {
                value = floor(static_cast<HDouble>(mFcevSofDisplayValue) * 0.1);  // rounddown
            } else {
                // no operation
            }
        }
        setSFCFCEV_Fuel_SystemConstantFCGaugeValue(value);
    }

    void updateConstantFCGaugeStat() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemConstantFCGaugeStat stat = SFCFCEV_Fuel_SystemConstantFCGaugeStat::NORMAL;

        if (mIsIgnOn == true && mFcevFuelEmptyStatus == FcevFuelEmptyStatus::EMPTY) {
            stat = SFCFCEV_Fuel_SystemConstantFCGaugeStat::BLINK2;
        }
        setSFCFCEV_Fuel_SystemConstantFCGaugeStat(stat);
    }

    // 5.3.1 LOW FUEL, REFUEL(FCEV)
    void updateEventLowFuel() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventLowFuelStat stat = SFCFCEV_Fuel_SystemEventLowFuelStat::OFF;

        if (mIsIgnOn == true &&
            (mFcevFuelEmptyStatus == FcevFuelEmptyStatus::EMPTY || mFcevLowFuelWarnStatus == FcevLowFuelWarnStatus::WARN)) {
            stat = SFCFCEV_Fuel_SystemEventLowFuelStat::ON;
        }
        setSFCFCEV_Fuel_SystemEventLowFuelStat(stat);
    }

    // 5.3.2 수소 탱크 경고(FCEV)
    void updateEventH2TankSystemWarn() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;
        SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType linkedSoundType =
            SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType::NONE;

        switch (mH2TankSystemWarnStatus) {
            case H2TankSystemWarnStatus::OVERFILL:
                eventID = "E76902";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType::REPEAT_COUNT;
                break;
            case H2TankSystemWarnStatus::H2_LEAK:
                if (mIsIgnOn == true) {
                    eventID = "E76903";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType::REPEAT_COUNT;
                }
                break;
            case H2TankSystemWarnStatus::OVERCOOL:
                if (mIsIgnOn == true) {
                    eventID = "E76904";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType::REPEAT_COUNT;
                }
                break;
            case H2TankSystemWarnStatus::TEMP_ERROR:
                if (mIsIgnOn == true) {
                    eventID = "E76905";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType::INFINITE;
                }
                break;
            default:
                // no opeation
                break;
        }

        GETCACHEDVALUE(SFC.FCEV_Fuel_System.Event.TankSystemWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFCEV_Fuel_SystemEventTankSystemWarnStat(SFCFCEV_Fuel_SystemEventTankSystemWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFCEV_Fuel_SystemEventTankSystemWarnID(eventID);
            setSFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundID(linkedSoundID);
            setSFCFCEV_Fuel_SystemEventTankSystemWarnLinkedSoundType(linkedSoundType);
            setSFCFCEV_Fuel_SystemEventTankSystemWarnStat(SFCFCEV_Fuel_SystemEventTankSystemWarnStat::ON);
        }
    }

    // 5.3.3 탱크 충전 횟수 제한
    void updateEventTankFillCountOverWarn() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventTankFillCountOverWarnStat stat = SFCFCEV_Fuel_SystemEventTankFillCountOverWarnStat::OFF;

        if (mH2TankFillCountOverWarnStatus == H2TankFillCountOverWarnStatus::WARN) {
            stat = SFCFCEV_Fuel_SystemEventTankFillCountOverWarnStat::ON;
        }
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarnStat(stat);
    }

    // 5.3.4 연료 도어 상태 정보
    void updateEventFuelDoorOpenFromHMU() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMUStat stat = SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMUStat::OFF;

        if (mIsIgnOn == true && mFuelDoorStatusFromHMU == FuelDoorStatusFromHMU::REFUEL) {
            stat = SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMUStat::ON;
        }

        setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMUStat(stat);
    }

    // 5.3.5 연료 도어 센서 고장
    void updateEventDoorSensorFaultWarn() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnStat stat = SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnStat::OFF;

        if (mIsIgnOn == true && mFcevFuelDoorSensorFaultWarnStatus == FcevFuelDoorSensorFaultWarnStatus::FAULT) {
            stat = SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnStat::ON;
        }
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarnStat(stat);
    }

    // 5.3.6 LOW FUEL, REFUEL(FCEV) – HMU2
    void updateEventLowFuel2() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventLowFuel2Stat stat = SFCFCEV_Fuel_SystemEventLowFuel2Stat::OFF;

        if (mIsIgnOn == true && mConfigHMU2 == ConfigHMU2::ON &&
            (mFcevFuelEmptyStatus2 == FcevFuelEmptyStatus2::EMPTY || mFcevLowFuelWarnStatus2 == FcevLowFuelWarnStatus2::WARN)) {
            stat = SFCFCEV_Fuel_SystemEventLowFuel2Stat::ON;
        }
        setSFCFCEV_Fuel_SystemEventLowFuel2Stat(stat);
    }

    // 5.3.7 수소 탱크 경고(FCEV) – HMU2
    void updateEventH2TankSystemWarn2() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;
        SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType linkedSoundType =
            SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType::NONE;

        if (mConfigHMU2 == ConfigHMU2::ON) {
            switch (mH2TankSystemWarnStatus2) {
                case H2TankSystemWarnStatus2::OVERFILL:
                    eventID = "E76911";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType::REPEAT_COUNT;
                    break;
                case H2TankSystemWarnStatus2::H2_LEAK:
                    if (mIsIgnOn == true) {
                        eventID = "E76912";
                        linkedSoundID = "SND_PopUpWarn1";
                        linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType::REPEAT_COUNT;
                    }
                    break;
                case H2TankSystemWarnStatus2::OVERCOOL:
                    if (mIsIgnOn == true) {
                        eventID = "E76913";
                        linkedSoundID = "SND_PopUpWarn1";
                        linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType::REPEAT_COUNT;
                    }
                    break;
                case H2TankSystemWarnStatus2::TEMP_ERROR:
                    if (mIsIgnOn == true) {
                        eventID = "E76914";
                        linkedSoundID = "SND_PopUpWarn2";
                        linkedSoundType = SFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType::INFINITE;
                    }
                    break;
                default:
                    // no opeation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.FCEV_Fuel_System.Event.TankSystemWarn2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFCEV_Fuel_SystemEventTankSystemWarn2Stat(SFCFCEV_Fuel_SystemEventTankSystemWarn2Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFCEV_Fuel_SystemEventTankSystemWarn2ID(eventID);
            setSFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundID(linkedSoundID);
            setSFCFCEV_Fuel_SystemEventTankSystemWarn2LinkedSoundType(linkedSoundType);
            setSFCFCEV_Fuel_SystemEventTankSystemWarn2Stat(SFCFCEV_Fuel_SystemEventTankSystemWarn2Stat::ON);
        }
    }

    // 5.3.8 탱크 충전 횟수 제한 – HMU2
    void updateEventTankFillCountOverWarn2() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventTankFillCountOverWarn2Stat stat = SFCFCEV_Fuel_SystemEventTankFillCountOverWarn2Stat::OFF;

        if (mH2TankFillCountOverWarnStatus2 == H2TankFillCountOverWarnStatus2::WARN && mConfigHMU2 == ConfigHMU2::ON) {
            stat = SFCFCEV_Fuel_SystemEventTankFillCountOverWarn2Stat::ON;
        }
        setSFCFCEV_Fuel_SystemEventTankFillCountOverWarn2Stat(stat);
    }

    // 5.3.9 연료 도어 상태 정보 – HMU2
    void updateEventFuelDoorOpenFromHMU2() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;

        if (mIsIgnOn == true) {
            if (mFuelDoorStatusFromHMU2 == FuelDoorStatusFromHMU2::OPEN && mConfigHMU2 == ConfigHMU2::ON) {
                eventID = "E76916";
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mFuelDoorStatusFromHMU2 == FuelDoorStatusFromHMU2::REFUEL && mConfigHMU2 == ConfigHMU2::ON) {
                eventID = "E76917";
                linkedSoundID = "SND_PopUpInform2";
            } else {
                // no opeation
            }
        } else {
            // no opeation
        }

        GETCACHEDVALUE(SFC.FCEV_Fuel_System.Event.FuelDoorStatusFromHMU2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2Stat(SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2ID(eventID);
            setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2LinkedSoundID(linkedSoundID);
            setSFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2Stat(SFCFCEV_Fuel_SystemEventFuelDoorStatusFromHMU2Stat::ON);
        }
    }

    // 5.3.10 연료 도어 센서 고장 – HMU2
    void updateEventDoorSensorFaultWarn2() {
        if (mInter_ConfigVehicleType != Inter_ConfigVehicleType::FCEV) {
            return;
        }

        SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2Stat stat = SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2Stat::OFF;

        if (mIsIgnOn == true && mFcevFuelDoorSensorFaultWarnStatus2 == FcevFuelDoorSensorFaultWarnStatus2::FAULT &&
            mConfigHMU2 == ConfigHMU2::ON) {
            stat = SFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2Stat::ON;
        }
        setSFCFCEV_Fuel_SystemEventFuelDoorSensorFaultWarn2Stat(stat);
    }

    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigHMU2 = ArgumentsConfigHMU2Changed::Inter_ConfigHMU2;
    using FcevLowFuelWarnStatus = ArgumentsLowFuelWarnStatusChanged::Input_FcevLowFuelWarnStatus;
    using FcevFuelEmptyStatus = ArgumentsEmptyStatusChanged::Input_FcevFuelEmptyStatus;
    using H2TankSystemWarnStatus = ArgumentsTankSystemWarnStatusChanged::Input_H2TankSystemWarnStatus;
    using H2TankFillCountOverWarnStatus = ArgumentsTankFillCountOverWarnStatusChanged::Input_H2TankFillCountOverWarnStatus;
    using FuelDoorStatusFromHMU = ArgumentsFuelDoorStatusFromHMUChanged::Input_FuelDoorStatusFromHMU;
    using FcevFuelDoorSensorFaultWarnStatus = ArgumentsDoorSensorFaultStatusChanged::Input_FcevFuelDoorSensorFaultWarnStatus;
    using FcevLowFuelWarnStatus2 = ArgumentsEmptyStatus2Changed::Input_FcevLowFuelWarnStatus2;
    using FcevFuelEmptyStatus2 = ArgumentsEmptyStatus2Changed::Input_FcevFuelEmptyStatus2;
    using H2TankSystemWarnStatus2 = ArgumentsTankSystemWarnStatus2Changed::Input_H2TankSystemWarnStatus2;
    using H2TankFillCountOverWarnStatus2 = ArgumentsTankFillCountOverWarnStatus2Changed::Input_H2TankFillCountOverWarnStatus2;
    using FuelDoorStatusFromHMU2 = ArgumentsFuelDoorStatusFromHMU2Changed::Input_FuelDoorStatusFromHMU2;
    using FcevFuelDoorSensorFaultWarnStatus2 = ArgumentsDoorSensorFaultStatus2Changed::Input_FcevFuelDoorSensorFaultWarnStatus2;

    static constexpr HUInt64 kSOFValue_0h = 0x0;      // 0
    static constexpr HUInt64 kSOFValue_3E8h = 0x3E8;  // 1000

    bool mIsIgnOn = false;
    HUInt64 mFcevSofDisplayValue = 0;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    ConfigHMU2 mConfigHMU2 = ConfigHMU2::OFF;
    FcevLowFuelWarnStatus mFcevLowFuelWarnStatus = FcevLowFuelWarnStatus::OFF;
    FcevFuelEmptyStatus mFcevFuelEmptyStatus = FcevFuelEmptyStatus::NORMAL;
    H2TankSystemWarnStatus mH2TankSystemWarnStatus = H2TankSystemWarnStatus::OFF;
    H2TankFillCountOverWarnStatus mH2TankFillCountOverWarnStatus = H2TankFillCountOverWarnStatus::OFF;
    FuelDoorStatusFromHMU mFuelDoorStatusFromHMU = FuelDoorStatusFromHMU::OFF;
    FcevFuelDoorSensorFaultWarnStatus mFcevFuelDoorSensorFaultWarnStatus = FcevFuelDoorSensorFaultWarnStatus::OFF;
    FcevLowFuelWarnStatus2 mFcevLowFuelWarnStatus2 = FcevLowFuelWarnStatus2::OFF;
    FcevFuelEmptyStatus2 mFcevFuelEmptyStatus2 = FcevFuelEmptyStatus2::NORMAL;
    H2TankSystemWarnStatus2 mH2TankSystemWarnStatus2 = H2TankSystemWarnStatus2::OFF;
    H2TankFillCountOverWarnStatus2 mH2TankFillCountOverWarnStatus2 = H2TankFillCountOverWarnStatus2::OFF;
    FuelDoorStatusFromHMU2 mFuelDoorStatusFromHMU2 = FuelDoorStatusFromHMU2::OFF;
    FcevFuelDoorSensorFaultWarnStatus2 mFcevFuelDoorSensorFaultWarnStatus2 = FcevFuelDoorSensorFaultWarnStatus2::OFF;
};

}  // namespace ccos

#endif  // SFSS_FCEV_Fuel_System_H
