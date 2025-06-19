/**
 * @file Cooling_System.hpp
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
#ifndef SFSS_Cooling_System_H
#define SFSS_Cooling_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Cooling_SystemBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.19
// Reference : [CV420] Cooling_System. Contains : Telltale, Event, Constant. Recommend: VALUE_CHANGED
class Cooling_System : public Cooling_SystemBase {
public:
    Cooling_System() = default;
    ~Cooling_System() override = default;
    Cooling_System(const Cooling_System& other) = delete;
    Cooling_System(Cooling_System&& other) noexcept = delete;
    Cooling_System& operator=(const Cooling_System& other) = delete;
    Cooling_System& operator=(Cooling_System&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1.1 고전압 냉각수 부족 경고
        setSFCCooling_SystemEventLowPECoolantWarningLampID("E74201");
        setSFCCooling_SystemEventLowPECoolantWarningLampLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventLowPECoolantWarningLampLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventLowPECoolantWarningLampLinkedSoundType(
            SFCCooling_SystemEventLowPECoolantWarningLampLinkedSoundType::REPEAT_COUNT);
        // 5.3.1.2 고전압 배터리 냉각수 부족 경고
        setSFCCooling_SystemEventLowBATCoolantWarningID("E74202");
        setSFCCooling_SystemEventLowBATCoolantWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventLowBATCoolantWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventLowBATCoolantWarningLinkedSoundType(
            SFCCooling_SystemEventLowBATCoolantWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.1.3 ATM 냉각수 부족 경고
        setSFCCooling_SystemEventLowATMCoolantWarningID("E74203");
        setSFCCooling_SystemEventLowATMCoolantWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventLowATMCoolantWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventLowATMCoolantWarningLinkedSoundType(
            SFCCooling_SystemEventLowATMCoolantWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.1.4 연료 전지 냉각수 부족 경고
        setSFCCooling_SystemEventFCCoolantWarningID("E74211");
        setSFCCooling_SystemEventFCCoolantWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCCooling_SystemEventFCCoolantWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventFCCoolantWarningLinkedSoundType(
            SFCCooling_SystemEventFCCoolantWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.1.5 보조 제동 냉각수 부족
        setSFCCooling_SystemEventLowBRCoolantWarningID("E74213");
        setSFCCooling_SystemEventLowBRCoolantWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventLowBRCoolantWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventLowBRCoolantWarningLinkedSoundType(
            SFCCooling_SystemEventLowBRCoolantWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.2 팬 오일 부족 경고
        setSFCCooling_SystemEventLowFanOilWarningID("E74204");
        setSFCCooling_SystemEventLowFanOilWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCCooling_SystemEventLowFanOilWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventLowFanOilWarningLinkedSoundType(
            SFCCooling_SystemEventLowFanOilWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.3.1 고전압 냉각수 과열
        setSFCCooling_SystemEventPECoolantOverHeatWarningLampID("E74205");
        setSFCCooling_SystemEventPECoolantOverHeatWarningLampLinkedSoundID("SND_PopUpWarn1");
        setSFCCooling_SystemEventPECoolantOverHeatWarningLampLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventPECoolantOverHeatWarningLampLinkedSoundType(
            SFCCooling_SystemEventPECoolantOverHeatWarningLampLinkedSoundType::REPEAT_COUNT);
        // 5.3.3.2 보조 제동 냉각수 과열
        setSFCCooling_SystemEventBRCoolantOverHeatID("E74214");
        setSFCCooling_SystemEventBRCoolantOverHeatLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventBRCoolantOverHeatLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventBRCoolantOverHeatLinkedSoundType(
            SFCCooling_SystemEventBRCoolantOverHeatLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.1 고전압 냉각 시스템 점검
        setSFCCooling_SystemEventPECoolantSystemCheckID("E74206");
        setSFCCooling_SystemEventPECoolantSystemCheckLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventPECoolantSystemCheckLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventPECoolantSystemCheckLinkedSoundType(
            SFCCooling_SystemEventPECoolantSystemCheckLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.2 고전압 배터리 냉각 시스템 점검
        setSFCCooling_SystemEventBmsCoolingFaultID("E74212");
        setSFCCooling_SystemEventBmsCoolingFaultLinkedSoundID("SND_PopUpWarn1");
        setSFCCooling_SystemEventBmsCoolingFaultLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventBmsCoolingFaultLinkedSoundType(
            SFCCooling_SystemEventBmsCoolingFaultLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.3 ATM 냉각 시스템 점검
        setSFCCooling_SystemEventATMCoolantSystemCheckID("E74208");
        setSFCCooling_SystemEventATMCoolantSystemCheckLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventATMCoolantSystemCheckLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventATMCoolantSystemCheckLinkedSoundType(
            SFCCooling_SystemEventATMCoolantSystemCheckLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.4 연료 전지 냉각 시스템 점검
        setSFCCooling_SystemEventFCCoolantSystemCheckID("E74209");
        setSFCCooling_SystemEventFCCoolantSystemCheckLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventFCCoolantSystemCheckLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventFCCoolantSystemCheckLinkedSoundType(
            SFCCooling_SystemEventFCCoolantSystemCheckLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.5 보조 제동 냉각 시스템 점검
        setSFCCooling_SystemEventBRCoolantSystemCheckID("E74215");
        setSFCCooling_SystemEventBRCoolantSystemCheckLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventBRCoolantSystemCheckLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventBRCoolantSystemCheckLinkedSoundType(
            SFCCooling_SystemEventBRCoolantSystemCheckLinkedSoundType::REPEAT_COUNT);
        // 5.3.4.6 스택 냉각팬 시스템 점검
        // EXNCP-33851 : 스택 냉각팬 시스템 점검의 Event ID는 E74216
        setSFCCooling_SystemEventFCCoolingFanSystemCheckID("E74216");
        setSFCCooling_SystemEventFCCoolingFanSystemCheckLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventFCCoolingFanSystemCheckLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventFCCoolingFanSystemCheckLinkedSoundType(
            SFCCooling_SystemEventFCCoolingFanSystemCheckLinkedSoundType::REPEAT_COUNT);
        // 5.3.5 연료전지 냉각수 및 필터 교체 경고
        setSFCCooling_SystemEventFCCoolantReplaceWarningID("E74210");
        setSFCCooling_SystemEventFCCoolantReplaceWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCCooling_SystemEventFCCoolantReplaceWarningLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventFCCoolantReplaceWarningLinkedSoundType(
            SFCCooling_SystemEventFCCoolantReplaceWarningLinkedSoundType::REPEAT_COUNT);
        // 5.3.6 연료전지 냉각수 OverHeating 경고
        setSFCCooling_SystemEventFCCoolantOverheatingID("E74217");
        setSFCCooling_SystemEventFCCoolantOverheatingLinkedSoundID("SND_PopUpWarn2");
        setSFCCooling_SystemEventFCCoolantOverheatingLinkedSoundRepeatCount(1);
        setSFCCooling_SystemEventFCCoolantOverheatingLinkedSoundType(
            SFCCooling_SystemEventFCCoolantOverheatingLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltaleLowCoolantLamp();
        updateTelltaleOverHeatWarningLamp();
        updateConstantCoolantTemperature();
        updateEventLowPECoolantWarningLamp();
        updateEventLowATMCoolantWarning();
        updateEventFCCoolantWarning();
        updateEventLowBRCoolantWarning();
        updateEventLowFanOilWarning();
        updateEventPECoolantOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
        updateEventPECoolantSystemCheck();
        updateEventATMCoolantSystemCheck();
        updateEventFCCoolantSystemCheck();
        updateEventBRCoolantSystemCheck();
        updateEventFCCoolingFanSystemCheck();
        updateEventFCCoolantReplaceWarning();
        updateEventFCCoolantOverheating();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltaleLowCoolantLamp();
        updateTelltaleOverHeatWarningLamp();
        updateConstantCoolantTemperature();
        updateEventLowPECoolantWarningLamp();
        updateEventLowATMCoolantWarning();
        updateEventFCCoolantWarning();
        updateEventLowBRCoolantWarning();
        updateEventLowFanOilWarning();
        updateEventPECoolantOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
        updateEventPECoolantSystemCheck();
        updateEventATMCoolantSystemCheck();
        updateEventFCCoolantSystemCheck();
        updateEventBRCoolantSystemCheck();
        updateEventFCCoolingFanSystemCheck();
        updateEventFCCoolantReplaceWarning();
        updateEventFCCoolantOverheating();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updateTelltaleLowCoolantLamp();
        updateTelltaleOverHeatWarningLamp();
        updateConstantCoolantTemperature();
        updateEventLowPECoolantWarningLamp();
        updateEventLowATMCoolantWarning();
        updateEventFCCoolantWarning();
        updateEventLowBRCoolantWarning();
        updateEventLowFanOilWarning();
        updateEventPECoolantOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
        updateEventPECoolantSystemCheck();
        updateEventATMCoolantSystemCheck();
        updateEventFCCoolantSystemCheck();
        updateEventBRCoolantSystemCheck();
        updateEventFCCoolingFanSystemCheck();
        updateEventFCCoolantReplaceWarning();
        updateEventFCCoolantOverheating();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltaleLowCoolantLamp();
        updateTelltaleOverHeatWarningLamp();
        updateConstantCoolantTemperature();
        updateEventLowPECoolantWarningLamp();
        updateEventLowATMCoolantWarning();
        updateEventFCCoolantWarning();
        updateEventLowBRCoolantWarning();
        updateEventLowFanOilWarning();
        updateEventPECoolantOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
        updateEventPECoolantSystemCheck();
        updateEventATMCoolantSystemCheck();
        updateEventFCCoolantSystemCheck();
        updateEventBRCoolantSystemCheck();
        updateEventFCCoolingFanSystemCheck();
        updateEventFCCoolantReplaceWarning();
        updateEventFCCoolantOverheating();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleLowCoolantLamp();
        updateTelltaleOverHeatWarningLamp();
        updateTelltaleLowBATCoolantWarningLamp();
        updateConstantCoolantTemperature();
        updateEventLowPECoolantWarningLamp();
        updateEventLowBATCoolantWarningLamp();
        updateEventLowATMCoolantWarning();
        updateEventFCCoolantWarning();
        updateEventLowBRCoolantWarning();
        updateEventLowFanOilWarning();
        updateEventPECoolantOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
        updateEventPECoolantSystemCheck();
        updateEventATMCoolantSystemCheck();
        updateEventFCCoolantSystemCheck();
        updateEventBmsCoolingFault();
        updateEventBRCoolantSystemCheck();
        updateEventFCCoolingFanSystemCheck();
        updateEventFCCoolantReplaceWarning();
        updateEventFCCoolantOverheating();
    }

    void onLowPECoolantWarningLampStatusChanged(const ArgumentsLowPECoolantWarningLampStatusChanged& args) {
        mLowPECoolantWarningLamp = args.mInput_LowPECoolantWarningLampStatus;
        updateTelltaleLowCoolantLamp();
        updateEventLowPECoolantWarningLamp();
    }

    void onLowBATCoolantWarningLampStatusChanged(const ArgumentsLowBATCoolantWarningLampStatusChanged& args) {
        mLowBATCoolantWarningLamp = args.mInput_LowBATCoolantWarningLampStatus;
        updateTelltaleLowBATCoolantWarningLamp();
        updateEventLowBATCoolantWarningLamp();
    }

    void onPeCoolantOverHeatWarningLampStatusChanged(const ArgumentsPeCoolantOverHeatWarningLampStatusChanged& args) {
        mPECoolantOverHeatWarningLamp = args.mInput_PECoolantOverHeatWarningLampStatus;
        updateTelltaleOverHeatWarningLamp();
        updateEventPECoolantOverHeatWarningLamp();
    }

    void onLowATMCoolantWarningStatusChanged(const ArgumentsLowATMCoolantWarningStatusChanged& args) {
        mLowATMCoolantWarning = args.mInput_LowATMCoolantWarningStatus;
        updateTelltaleLowCoolantLamp();
        updateEventLowATMCoolantWarning();
    }

    void onLowFanOilWarningStatusChanged(const ArgumentsLowFanOilWarningStatusChanged& args) {
        mLowFanOilWarning = args.mInput_LowFanOilWarningStatus;
        updateEventLowFanOilWarning();
    }

    void onPeCoolantSystemCheckStatusChanged(const ArgumentsPeCoolantSystemCheckStatusChanged& args) {
        mPeCoolantSystemCheck = args.mInput_PECoolantSystemCheckStatus;
        updateEventPECoolantSystemCheck();
    }

    void onAtmCoolantSystemCheckStatusChanged(const ArgumentsAtmCoolantSystemCheckStatusChanged& args) {
        mATMCoolantSystemCheck = args.mInput_ATMCoolantSystemCheckStatus;
        updateEventATMCoolantSystemCheck();
    }

    void onFcCoolantSystemCheckStatusChanged(const ArgumentsFcCoolantSystemCheckStatusChanged& args) {
        mFCCoolantSystemCheck = args.mInput_FCCoolantSystemCheckStatus;
        updateEventFCCoolantSystemCheck();
    }

    void onBmsCoolingFaultStatusChanged(const ArgumentsBmsCoolingFaultStatusChanged& args) {
        mBmsCoolingFault = args.mInput_BmsCoolingFaultStatus;
        updateEventBmsCoolingFault();
    }

    void onFcCoolantReplaceWarningStatusChanged(const ArgumentsFcCoolantReplaceWarningStatusChanged& args) {
        mFCCoolantReplaceWarning1 = args.mInput_FCCoolantReplaceWarningStatus1;
        mFCCoolantReplaceWarning2 = args.mInput_FCCoolantReplaceWarningStatus2;
        updateEventFCCoolantReplaceWarning();
    }

    void onFcCoolantWarningStatusChanged(const ArgumentsFcCoolantWarningStatusChanged& args) {
        mFCCoolantWarning = args.mInput_FCCoolantWarningStatus;
        mFCCoolantWarning2 = args.mInput_FCCoolantWarningStatus2;
        updateTelltaleLowCoolantLamp();
        updateEventFCCoolantWarning();
    }

    void onFcCoolantOverheatingStatusChanged(const ArgumentsFcCoolantOverheatingStatusChanged& args) {
        mFCCoolantOverheating = args.mInput_FCCoolantOverheatingStatus;
        mFCCoolantOverheating2 = args.mInput_FCCoolantOverheatingStatus2;
        updateTelltaleOverHeatWarningLamp();
        updateEventFCCoolantOverheating();
    }

    void onEngineCoolantWarningLampChanged(const ArgumentsEngineCoolantWarningLampChanged& args) {
        mEngineCoolantWarning = args.mInput_EngineCoolantWarningStatus;
        updateTelltaleOverHeatWarningLamp();
    }

    void onLowEngineCoolantWarningLampStatusChanged(const ArgumentsLowEngineCoolantWarningLampStatusChanged& args) {
        mLowEngineCoolantWarningLamp = args.mInput_LowEngineCoolantWarningLampStatus;
        updateTelltaleLowCoolantLamp();
    }

    void onWaterTempChanged(const ArgumentsWaterTempChanged& args) {
        mEngineCoolingWaterTemperature = args.mInput_EngineCoolingWaterTemperature;
        updateConstantCoolantTemperature();
    }

    void onBrCoolantOverHeatWarningLampStatusChanged(const ArgumentsBrCoolantOverHeatWarningLampStatusChanged& args) {
        mBRCoolantOverHeatWarningLamp = args.mInput_BRCoolantOverHeatWarningLampStatus;
        updateTelltaleOverHeatWarningLamp();
        updateEventBRCoolantOverHeatWarning();
    }

    void onLowBRCoolantWarningStatusChanged(const ArgumentsLowBRCoolantWarningStatusChanged& args) {
        mLowBRCoolantWarning = args.mInput_LowBRCoolantWarningStatus;
        updateTelltaleLowCoolantLamp();
        updateEventLowBRCoolantWarning();
    }

    void onBrCoolantSystemCheckStatusChanged(const ArgumentsBrCoolantSystemCheckStatusChanged& args) {
        mBRCoolantSystemCheck = args.mInput_BRCoolantSystemCheckStatus;
        updateEventBRCoolantSystemCheck();
    }

    void onFcCoolingFanSystemCheckStatusChanged(const ArgumentsFcCoolingFanSystemCheckStatusChanged& args) {
        mFCCoolingFanSystemCheck = args.mInput_FCCoolingFanSystemCheckStatus;
        updateEventFCCoolingFanSystemCheck();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // EXNCP-34542 : C12, v0.11 사양서의 추가 개선 항목으로, 모든 출력에 Inter_ConfigVehicleType 비교 사양 추가됨
    // 5.1.1 냉각수 부족 경고, IMG_TelltaleLowCoolantLamp_stat
    void updateTelltaleLowCoolantLamp() {
        SFCCooling_SystemTelltaleLowCoolantLampStat telltaleStat = SFCCooling_SystemTelltaleLowCoolantLampStat::OFF;
        if (mIsIgnOn == true) {
            if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
                (mLowPECoolantWarningLamp == LowPECoolantWarningLamp::ON || mLowATMCoolantWarning == LowATMCoolantWarning::ON ||
                 mLowBRCoolantWarning == LowBRCoolantWarning::ON)) {
                telltaleStat = SFCCooling_SystemTelltaleLowCoolantLampStat::ON;
            } else if (mConfigVehicleType == ConfigVehicleType::FCEV &&
                       (mFCCoolantWarning == FCCoolantWarning::ON || mFCCoolantWarning2 == FCCoolantWarning2::ON)) {
                telltaleStat = SFCCooling_SystemTelltaleLowCoolantLampStat::ON;
            } else if ((mConfigVehicleType == ConfigVehicleType::ICV) &&
                       (mLowEngineCoolantWarningLamp == LowEngineCoolantWarningLamp::ON)) {
                telltaleStat = SFCCooling_SystemTelltaleLowCoolantLampStat::ON;
            } else {
                // no operation
            }
        }
        setSFCCooling_SystemTelltaleLowCoolantLampStat(telltaleStat);
    }

    // 5.1.2 메인 배터리 냉각수 부족 경고, IMG_TelltaleLowBATCoolantLamp_stat
    void updateTelltaleLowBATCoolantWarningLamp() {
        SFCCooling_SystemTelltaleLowBATCoolantLampStat telltaleStat = SFCCooling_SystemTelltaleLowBATCoolantLampStat::OFF;
        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowBATCoolantWarningLamp == LowBATCoolantWarningLamp::ON) {
            telltaleStat = SFCCooling_SystemTelltaleLowBATCoolantLampStat::ON;
        }
        setSFCCooling_SystemTelltaleLowBATCoolantLampStat(telltaleStat);
    }

    // 5.1.3 냉각수 과열 경고(VCU), IMG_TelltaleOverHeatWarningLamp_stat
    void updateTelltaleOverHeatWarningLamp() {
        SFCCooling_SystemTelltaleOverHeatWarningLampStat telltaleStat = SFCCooling_SystemTelltaleOverHeatWarningLampStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            telltaleStat = SFCCooling_SystemTelltaleOverHeatWarningLampStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
                (mPECoolantOverHeatWarningLamp == PECoolantOverHeatWarningLamp::ON ||
                 mBRCoolantOverHeatWarningLamp == BRCoolantOverHeatWarningLamp::ON)) {
                telltaleStat = SFCCooling_SystemTelltaleOverHeatWarningLampStat::ON;
            } else if (mConfigVehicleType == ConfigVehicleType::FCEV && (mFCCoolantOverheating == FCCoolantOverheating::ON ||
                                                                         mFCCoolantOverheating2 == FCCoolantOverheating2::ON)) {
                telltaleStat = SFCCooling_SystemTelltaleOverHeatWarningLampStat::ON;
            } else if (mConfigVehicleType == ConfigVehicleType::ICV && mEngineCoolantWarning == EngineCoolantWarning::ON) {
                telltaleStat = SFCCooling_SystemTelltaleOverHeatWarningLampStat::ON;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        setSFCCooling_SystemTelltaleOverHeatWarningLampStat(telltaleStat);
    }

    // 5.2.1 수온게이지 표시, IMG_CoolantTemp_Value
    void updateConstantCoolantTemperature() {
        HUInt64 tempValue = 0;
        if (mIsIgnOn == true && 0 <= mEngineCoolingWaterTemperature && mEngineCoolingWaterTemperature <= 250 &&
            mConfigVehicleType == ConfigVehicleType::ICV) {
            tempValue = mEngineCoolingWaterTemperature;
        }
        setSFCCooling_SystemConstantCoolantTempValue(tempValue);
    }

    // 5.3.1.1 고전압 냉각수 부족 경고
    void updateEventLowPECoolantWarningLamp() {
        SFCCooling_SystemEventLowPECoolantWarningLampStat eventStat = SFCCooling_SystemEventLowPECoolantWarningLampStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowPECoolantWarningLamp == LowPECoolantWarningLamp::ON) {
            eventStat = SFCCooling_SystemEventLowPECoolantWarningLampStat::ON;
        }
        setSFCCooling_SystemEventLowPECoolantWarningLampStat(eventStat);
    }

    // 5.3.1.2 고전압 배터리 냉각수 부족 경고
    void updateEventLowBATCoolantWarningLamp() {
        SFCCooling_SystemEventLowBATCoolantWarningStat eventStat = SFCCooling_SystemEventLowBATCoolantWarningStat::OFF;
        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowBATCoolantWarningLamp == LowBATCoolantWarningLamp::ON) {
            eventStat = SFCCooling_SystemEventLowBATCoolantWarningStat::ON;
        }
        setSFCCooling_SystemEventLowBATCoolantWarningStat(eventStat);
    }

    // 5.3.1.3 ATM 냉각수 부족 경고
    void updateEventLowATMCoolantWarning() {
        SFCCooling_SystemEventLowATMCoolantWarningStat eventStat = SFCCooling_SystemEventLowATMCoolantWarningStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowATMCoolantWarning == LowATMCoolantWarning::ON) {
            eventStat = SFCCooling_SystemEventLowATMCoolantWarningStat::ON;
        }
        setSFCCooling_SystemEventLowATMCoolantWarningStat(eventStat);
    }

    // 5.3.1.4 연료 전지 냉각수 부족 경고
    void updateEventFCCoolantWarning() {
        SFCCooling_SystemEventFCCoolantWarningStat eventStat = SFCCooling_SystemEventFCCoolantWarningStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mFCCoolantWarning == FCCoolantWarning::ON || mFCCoolantWarning2 == FCCoolantWarning2::ON)) {
            eventStat = SFCCooling_SystemEventFCCoolantWarningStat::ON;
        }
        setSFCCooling_SystemEventFCCoolantWarningStat(eventStat);
    }

    // 5.3.1.5 보조 제동 냉각수 부족
    // EXNCP-33891 : Input_LowBRCoolantWarningStatus == ON
    void updateEventLowBRCoolantWarning() {
        SFCCooling_SystemEventLowBRCoolantWarningStat eventStat = SFCCooling_SystemEventLowBRCoolantWarningStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowBRCoolantWarning == LowBRCoolantWarning::ON) {
            eventStat = SFCCooling_SystemEventLowBRCoolantWarningStat::ON;
        }
        setSFCCooling_SystemEventLowBRCoolantWarningStat(eventStat);
    }

    // 5.3.2 팬 오일 부족 경고
    void updateEventLowFanOilWarning() {
        SFCCooling_SystemEventLowFanOilWarningStat eventStat = SFCCooling_SystemEventLowFanOilWarningStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mLowFanOilWarning == LowFanOilWarning::ON) {
            eventStat = SFCCooling_SystemEventLowFanOilWarningStat::ON;
        }
        setSFCCooling_SystemEventLowFanOilWarningStat(eventStat);
    }

    // 5.3.3.1 고전압 냉각수 과열
    void updateEventPECoolantOverHeatWarningLamp() {
        SFCCooling_SystemEventPECoolantOverHeatWarningLampStat eventStat =
            SFCCooling_SystemEventPECoolantOverHeatWarningLampStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mPECoolantOverHeatWarningLamp == PECoolantOverHeatWarningLamp::ON) {
            eventStat = SFCCooling_SystemEventPECoolantOverHeatWarningLampStat::ON;
        }
        setSFCCooling_SystemEventPECoolantOverHeatWarningLampStat(eventStat);
    }

    // 5.3.3.2 보조 제동 냉각수 과열
    void updateEventBRCoolantOverHeatWarning() {
        SFCCooling_SystemEventBRCoolantOverHeatStat eventStat = SFCCooling_SystemEventBRCoolantOverHeatStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mBRCoolantOverHeatWarningLamp == BRCoolantOverHeatWarningLamp::ON) {
            eventStat = SFCCooling_SystemEventBRCoolantOverHeatStat::ON;
        }
        setSFCCooling_SystemEventBRCoolantOverHeatStat(eventStat);
    }

    // 5.3.4.1 고전압 냉각 시스템 점검
    void updateEventPECoolantSystemCheck() {
        SFCCooling_SystemEventPECoolantSystemCheckStat eventStat = SFCCooling_SystemEventPECoolantSystemCheckStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mPeCoolantSystemCheck == PeCoolantSystemCheck::ON) {
            eventStat = SFCCooling_SystemEventPECoolantSystemCheckStat::ON;
        }
        setSFCCooling_SystemEventPECoolantSystemCheckStat(eventStat);
    }

    // 5.3.4.2 고전압 배터리 냉각 시스템 점검
    void updateEventBmsCoolingFault() {
        SFCCooling_SystemEventBmsCoolingFaultStat eventStat = SFCCooling_SystemEventBmsCoolingFaultStat::OFF;
        if (mBmsCoolingFault == BmsCoolingFault::ON &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV)) {
            eventStat = SFCCooling_SystemEventBmsCoolingFaultStat::ON;
        }
        setSFCCooling_SystemEventBmsCoolingFaultStat(eventStat);
    }

    // 5.3.4.3 ATM 냉각 시스템 점검
    void updateEventATMCoolantSystemCheck() {
        SFCCooling_SystemEventATMCoolantSystemCheckStat eventStat = SFCCooling_SystemEventATMCoolantSystemCheckStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mATMCoolantSystemCheck == ATMCoolantSystemCheck::ON) {
            eventStat = SFCCooling_SystemEventATMCoolantSystemCheckStat::ON;
        }
        setSFCCooling_SystemEventATMCoolantSystemCheckStat(eventStat);
    }

    // 5.3.4.4 연료 전지 냉각 시스템 점검
    void updateEventFCCoolantSystemCheck() {
        SFCCooling_SystemEventFCCoolantSystemCheckStat eventStat = SFCCooling_SystemEventFCCoolantSystemCheckStat::OFF;
        if (mIsIgnOn == true && mFCCoolantSystemCheck == FCCoolantSystemCheck::ON &&
            mConfigVehicleType == ConfigVehicleType::FCEV) {
            eventStat = SFCCooling_SystemEventFCCoolantSystemCheckStat::ON;
        }
        setSFCCooling_SystemEventFCCoolantSystemCheckStat(eventStat);
    }

    // 5.3.4.5 보조 제동 냉각 시스템 점검
    void updateEventBRCoolantSystemCheck() {
        SFCCooling_SystemEventBRCoolantSystemCheckStat eventStat = SFCCooling_SystemEventBRCoolantSystemCheckStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mBRCoolantSystemCheck == BRCoolantSystemCheck::ON) {
            eventStat = SFCCooling_SystemEventBRCoolantSystemCheckStat::ON;
        }
        setSFCCooling_SystemEventBRCoolantSystemCheckStat(eventStat);
    }

    // 5.3.4.6 스택 냉각팬 시스템 점검
    void updateEventFCCoolingFanSystemCheck() {
        SFCCooling_SystemEventFCCoolingFanSystemCheckStat eventStat = SFCCooling_SystemEventFCCoolingFanSystemCheckStat::OFF;
        if (mIsIgnOn == true && (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mFCCoolingFanSystemCheck == FCCoolingFanSystemCheck::ON) {
            eventStat = SFCCooling_SystemEventFCCoolingFanSystemCheckStat::ON;
        }
        setSFCCooling_SystemEventFCCoolingFanSystemCheckStat(eventStat);
    }

    // 5.3.5 연료전지 냉각수 및 필터 교체 경고
    void updateEventFCCoolantReplaceWarning() {
        SFCCooling_SystemEventFCCoolantReplaceWarningStat eventStat = SFCCooling_SystemEventFCCoolantReplaceWarningStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mFCCoolantReplaceWarning1 == FCCoolantReplaceWarning1::ON ||
             mFCCoolantReplaceWarning2 == FCCoolantReplaceWarning2::ON)) {
            eventStat = SFCCooling_SystemEventFCCoolantReplaceWarningStat::ON;
        }
        setSFCCooling_SystemEventFCCoolantReplaceWarningStat(eventStat);
    }

    // 5.3.6연료 전지 냉각수 부족 경고
    void updateEventFCCoolantOverheating() {
        SFCCooling_SystemEventFCCoolantOverheatingStat eventStat = SFCCooling_SystemEventFCCoolantOverheatingStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mFCCoolantOverheating == FCCoolantOverheating::ON || mFCCoolantOverheating2 == FCCoolantOverheating2::ON)) {
            eventStat = SFCCooling_SystemEventFCCoolantOverheatingStat::ON;
        }
        setSFCCooling_SystemEventFCCoolantOverheatingStat(eventStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using LowPECoolantWarningLamp = ArgumentsLowPECoolantWarningLampStatusChanged::Input_LowPECoolantWarningLampStatus;
    using LowBATCoolantWarningLamp = ArgumentsLowBATCoolantWarningLampStatusChanged::Input_LowBATCoolantWarningLampStatus;
    using PECoolantOverHeatWarningLamp =
        ArgumentsPeCoolantOverHeatWarningLampStatusChanged::Input_PECoolantOverHeatWarningLampStatus;
    using LowATMCoolantWarning = ArgumentsLowATMCoolantWarningStatusChanged::Input_LowATMCoolantWarningStatus;
    using LowFanOilWarning = ArgumentsLowFanOilWarningStatusChanged::Input_LowFanOilWarningStatus;
    using PeCoolantSystemCheck = ArgumentsPeCoolantSystemCheckStatusChanged::Input_PECoolantSystemCheckStatus;
    using ATMCoolantSystemCheck = ArgumentsAtmCoolantSystemCheckStatusChanged::Input_ATMCoolantSystemCheckStatus;
    using FCCoolantSystemCheck = ArgumentsFcCoolantSystemCheckStatusChanged::Input_FCCoolantSystemCheckStatus;
    using FCCoolantReplaceWarning1 = ArgumentsFcCoolantReplaceWarningStatusChanged::Input_FCCoolantReplaceWarningStatus1;
    using FCCoolantReplaceWarning2 = ArgumentsFcCoolantReplaceWarningStatusChanged::Input_FCCoolantReplaceWarningStatus2;
    using FCCoolantWarning = ArgumentsFcCoolantWarningStatusChanged::Input_FCCoolantWarningStatus;
    using FCCoolantWarning2 = ArgumentsFcCoolantWarningStatusChanged::Input_FCCoolantWarningStatus2;
    using FCCoolantOverheating = ArgumentsFcCoolantOverheatingStatusChanged::Input_FCCoolantOverheatingStatus;
    using FCCoolantOverheating2 = ArgumentsFcCoolantOverheatingStatusChanged::Input_FCCoolantOverheatingStatus2;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using EngineCoolantWarning = ArgumentsEngineCoolantWarningLampChanged::Input_EngineCoolantWarningStatus;
    using LowEngineCoolantWarningLamp =
        ArgumentsLowEngineCoolantWarningLampStatusChanged::Input_LowEngineCoolantWarningLampStatus;
    using BmsCoolingFault = ArgumentsBmsCoolingFaultStatusChanged::Input_BmsCoolingFaultStatus;
    using BRCoolantOverHeatWarningLamp =
        ArgumentsBrCoolantOverHeatWarningLampStatusChanged::Input_BRCoolantOverHeatWarningLampStatus;
    using LowBRCoolantWarning = ArgumentsLowBRCoolantWarningStatusChanged::Input_LowBRCoolantWarningStatus;
    using BRCoolantSystemCheck = ArgumentsBrCoolantSystemCheckStatusChanged::Input_BRCoolantSystemCheckStatus;
    using FCCoolingFanSystemCheck = ArgumentsFcCoolingFanSystemCheckStatusChanged::Input_FCCoolingFanSystemCheckStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    bool mIsIgnOn = false;
    LowPECoolantWarningLamp mLowPECoolantWarningLamp = LowPECoolantWarningLamp::OFF;
    LowBATCoolantWarningLamp mLowBATCoolantWarningLamp = LowBATCoolantWarningLamp::OFF;
    PECoolantOverHeatWarningLamp mPECoolantOverHeatWarningLamp = PECoolantOverHeatWarningLamp::OFF;
    LowATMCoolantWarning mLowATMCoolantWarning = LowATMCoolantWarning::OFF;
    LowFanOilWarning mLowFanOilWarning = LowFanOilWarning::OFF;
    PeCoolantSystemCheck mPeCoolantSystemCheck = PeCoolantSystemCheck::OFF;
    ATMCoolantSystemCheck mATMCoolantSystemCheck = ATMCoolantSystemCheck::OFF;
    FCCoolantSystemCheck mFCCoolantSystemCheck = FCCoolantSystemCheck::OFF;
    FCCoolantReplaceWarning1 mFCCoolantReplaceWarning1 = FCCoolantReplaceWarning1::OFF;
    FCCoolantReplaceWarning2 mFCCoolantReplaceWarning2 = FCCoolantReplaceWarning2::OFF;
    FCCoolantWarning mFCCoolantWarning = FCCoolantWarning::OFF;
    FCCoolantWarning2 mFCCoolantWarning2 = FCCoolantWarning2::OFF;
    FCCoolantOverheating mFCCoolantOverheating = FCCoolantOverheating::OFF;
    FCCoolantOverheating2 mFCCoolantOverheating2 = FCCoolantOverheating2::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EngineCoolantWarning mEngineCoolantWarning = EngineCoolantWarning::OFF;
    HUInt64 mEngineCoolingWaterTemperature = 0;
    LowEngineCoolantWarningLamp mLowEngineCoolantWarningLamp = LowEngineCoolantWarningLamp::OFF;
    BmsCoolingFault mBmsCoolingFault = BmsCoolingFault::OFF;
    BRCoolantOverHeatWarningLamp mBRCoolantOverHeatWarningLamp = BRCoolantOverHeatWarningLamp::OFF;
    LowBRCoolantWarning mLowBRCoolantWarning = LowBRCoolantWarning::OFF;
    BRCoolantSystemCheck mBRCoolantSystemCheck = BRCoolantSystemCheck::OFF;
    FCCoolingFanSystemCheck mFCCoolingFanSystemCheck = FCCoolingFanSystemCheck::OFF;
};

}  // namespace ccos

#endif  // SFSS_Cooling_System_H
