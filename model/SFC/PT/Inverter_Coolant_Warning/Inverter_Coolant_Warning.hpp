/**
 * @file Inverter_Coolant_Warning.hpp
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
#ifndef SFSS_Inverter_Coolant_Warning_H
#define SFSS_Inverter_Coolant_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Inverter_Coolant_WarningBase.hpp"

namespace ccos {

// SFC Version : 5.1.0
// Reference : [PT290] Inverter_Coolant_Warning. Contains : Event. Recommend: VALUE_CHANGED
class Inverter_Coolant_Warning : public Inverter_Coolant_WarningBase {
public:
    Inverter_Coolant_Warning() = default;
    ~Inverter_Coolant_Warning() override = default;
    Inverter_Coolant_Warning(const Inverter_Coolant_Warning& other) = delete;
    Inverter_Coolant_Warning(Inverter_Coolant_Warning&& other) noexcept = delete;
    Inverter_Coolant_Warning& operator=(const Inverter_Coolant_Warning& other) = delete;
    Inverter_Coolant_Warning& operator=(Inverter_Coolant_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCInverter_Coolant_WarningEventCoolantWarn1ID("E22004");  // MCU, MCU+VCU, MCU전륜 냉각수 팝업
        setSFCInverter_Coolant_WarningEventCoolantWarn1LinkedSoundID("SND_PopUpWarn1");
        setSFCInverter_Coolant_WarningEventCoolantWarn1LinkedSoundType(
            SFCInverter_Coolant_WarningEventCoolantWarn1LinkedSoundType::REPEAT_COUNT);
        setSFCInverter_Coolant_WarningEventCoolantWarn1LinkedSoundRepeatCount(1);

        setSFCInverter_Coolant_WarningEventCoolantWarn2ID("E22005");  // BMS , BMS+VCU 냉각수 팝업
        setSFCInverter_Coolant_WarningEventCoolantWarn2LinkedSoundID("SND_PopUpWarn1");
        setSFCInverter_Coolant_WarningEventCoolantWarn2LinkedSoundType(
            SFCInverter_Coolant_WarningEventCoolantWarn2LinkedSoundType::REPEAT_COUNT);
        setSFCInverter_Coolant_WarningEventCoolantWarn2LinkedSoundRepeatCount(1);

        setSFCInverter_Coolant_WarningEventCoolantWarnFromFCUID("E22006");  // FCU 냉각수 팝업
        setSFCInverter_Coolant_WarningEventCoolantWarnFromFCULinkedSoundID("SND_PopUpWarn1");
        setSFCInverter_Coolant_WarningEventCoolantWarnFromFCULinkedSoundType(
            SFCInverter_Coolant_WarningEventCoolantWarnFromFCULinkedSoundType::REPEAT_COUNT);
        setSFCInverter_Coolant_WarningEventCoolantWarnFromFCULinkedSoundRepeatCount(1);

        setSFCInverter_Coolant_WarningEventCoolantErrorStatusID("E22007");  // FCEV 냉각수 고장
        setSFCInverter_Coolant_WarningEventCoolantErrorStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCInverter_Coolant_WarningEventCoolantErrorStatusLinkedSoundType(
            SFCInverter_Coolant_WarningEventCoolantErrorStatusLinkedSoundType::REPEAT_COUNT);
        setSFCInverter_Coolant_WarningEventCoolantErrorStatusLinkedSoundRepeatCount(1);

        setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSID("E22008");  // ICV 냉각수 팝업
        setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSLinkedSoundID("SND_PopUpWarn1");
        setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSLinkedSoundType(
            SFCInverter_Coolant_WarningEventCoolantWarnFromEMSLinkedSoundType::REPEAT_COUNT);
        setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateMCUEvent();
        updateBMSEvent();
        updateFCUEvent();
        updateEMSEvent();
        updateErrorStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateMCUEvent();
        updateBMSEvent();
        updateFCUEvent();
        updateEMSEvent();
        updateErrorStatus();
    }

    void onCoolantWarnStatus1Changed(const ArgumentsCoolantWarnStatus1Changed& args) {
        mEwpCoolantRefillWarnStatus1 = args.mInput_EwpCoolantRefillWarnStatus1;
        mEwpCoolantRefillWarnStatus1FromVCU = args.mInput_EwpCoolantRefillWarnStatus1FromVCU;
        mEwpCoolantRefillWarnStatus1FromMCU2 = args.mInput_EwpCoolantRefillWarnStatus1FromMCU2;
        updateMCUEvent();
    }

    void onCoolantWarnStatus2Changed(const ArgumentsCoolantWarnStatus2Changed& args) {
        mEwpCoolantRefillWarnStatus2 = args.mInput_EwpCoolantRefillWarnStatus2;
        mEwpCoolantRefillWarnStatus2FromVCU = args.mInput_EwpCoolantRefillWarnStatus2FromVCU;
        updateBMSEvent();
    }

    void onCoolantWarnStatusFromFCUChanged(const ArgumentsCoolantWarnStatusFromFCUChanged& args) {
        mEwpCoolantRefillWarnStatusFromFCU = args.mInput_EwpCoolantRefillWarnStatusFromFCU;
        updateFCUEvent();
    }

    void onCoolantErrorStatusChanged(const ArgumentsCoolantErrorStatusChanged& args) {
        mEwpCoolantErrorStatus = args.mInput_EwpCoolantErrorStatus;
        updateErrorStatus();
    }

    void onCoolantWarnStatusFromEMSChanged(const ArgumentsCoolantWarnStatusFromEMSChanged& args) {
        mEwpCoolantRefillWarnStatusFromEMS = args.mInput_EwpCoolantRefillWarnStatusFromEMS;
        updateEMSEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateMCUEvent() {
        if ((mEwpCoolantRefillWarnStatus1FromMCU2 == EwpCoolantRefillWarnStatus1FromMCU2::ON ||
             mEwpCoolantRefillWarnStatus1FromVCU == EwpCoolantRefillWarnStatus1FromVCU::ON ||
             mEwpCoolantRefillWarnStatus1 == EwpCoolantRefillWarnStatus1::ON) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCInverter_Coolant_WarningEventCoolantWarn1Stat(SFCInverter_Coolant_WarningEventCoolantWarn1Stat::ON);
        } else {
            setSFCInverter_Coolant_WarningEventCoolantWarn1Stat(SFCInverter_Coolant_WarningEventCoolantWarn1Stat::OFF);
        }
    }

    void updateBMSEvent() {
        if ((mEwpCoolantRefillWarnStatus2 == EwpCoolantRefillWarnStatus2::ON ||
             mEwpCoolantRefillWarnStatus2FromVCU == EwpCoolantRefillWarnStatus2FromVCU::ON) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCInverter_Coolant_WarningEventCoolantWarn2Stat(SFCInverter_Coolant_WarningEventCoolantWarn2Stat::ON);
        } else {
            setSFCInverter_Coolant_WarningEventCoolantWarn2Stat(SFCInverter_Coolant_WarningEventCoolantWarn2Stat::OFF);
        }
    }

    void updateFCUEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mEwpCoolantRefillWarnStatusFromFCU == EwpCoolantRefillWarnStatusFromFCU::ON) {
            setSFCInverter_Coolant_WarningEventCoolantWarnFromFCUStat(SFCInverter_Coolant_WarningEventCoolantWarnFromFCUStat::ON);
        } else {
            setSFCInverter_Coolant_WarningEventCoolantWarnFromFCUStat(
                SFCInverter_Coolant_WarningEventCoolantWarnFromFCUStat::OFF);
        }
    }

    void updateErrorStatus() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mEwpCoolantErrorStatus == EwpCoolantErrorStatus::ERROR) {
            setSFCInverter_Coolant_WarningEventCoolantErrorStatusStat(SFCInverter_Coolant_WarningEventCoolantErrorStatusStat::ON);
        } else {
            setSFCInverter_Coolant_WarningEventCoolantErrorStatusStat(
                SFCInverter_Coolant_WarningEventCoolantErrorStatusStat::OFF);
        }
    }

    void updateEMSEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mEwpCoolantRefillWarnStatusFromEMS == EwpCoolantRefillWarnStatusFromEMS::ON) {
            setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSStat(SFCInverter_Coolant_WarningEventCoolantWarnFromEMSStat::ON);
        } else {
            setSFCInverter_Coolant_WarningEventCoolantWarnFromEMSStat(
                SFCInverter_Coolant_WarningEventCoolantWarnFromEMSStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using EwpCoolantRefillWarnStatus1 = ArgumentsCoolantWarnStatus1Changed::Input_EwpCoolantRefillWarnStatus1;
    using EwpCoolantRefillWarnStatus1FromVCU = ArgumentsCoolantWarnStatus1Changed::Input_EwpCoolantRefillWarnStatus1FromVCU;
    using EwpCoolantRefillWarnStatus1FromMCU2 = ArgumentsCoolantWarnStatus1Changed::Input_EwpCoolantRefillWarnStatus1FromMCU2;
    using EwpCoolantRefillWarnStatus2 = ArgumentsCoolantWarnStatus2Changed::Input_EwpCoolantRefillWarnStatus2;
    using EwpCoolantRefillWarnStatus2FromVCU = ArgumentsCoolantWarnStatus2Changed::Input_EwpCoolantRefillWarnStatus2FromVCU;
    using EwpCoolantRefillWarnStatusFromFCU = ArgumentsCoolantWarnStatusFromFCUChanged::Input_EwpCoolantRefillWarnStatusFromFCU;
    using EwpCoolantErrorStatus = ArgumentsCoolantErrorStatusChanged::Input_EwpCoolantErrorStatus;
    using EwpCoolantRefillWarnStatusFromEMS = ArgumentsCoolantWarnStatusFromEMSChanged::Input_EwpCoolantRefillWarnStatusFromEMS;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    EwpCoolantRefillWarnStatus1 mEwpCoolantRefillWarnStatus1 = EwpCoolantRefillWarnStatus1::OFF;
    EwpCoolantRefillWarnStatus1FromVCU mEwpCoolantRefillWarnStatus1FromVCU = EwpCoolantRefillWarnStatus1FromVCU::OFF;
    EwpCoolantRefillWarnStatus1FromMCU2 mEwpCoolantRefillWarnStatus1FromMCU2 = EwpCoolantRefillWarnStatus1FromMCU2::OFF;
    EwpCoolantRefillWarnStatus2 mEwpCoolantRefillWarnStatus2 = EwpCoolantRefillWarnStatus2::OFF;
    EwpCoolantRefillWarnStatus2FromVCU mEwpCoolantRefillWarnStatus2FromVCU = EwpCoolantRefillWarnStatus2FromVCU::OFF;
    EwpCoolantRefillWarnStatusFromFCU mEwpCoolantRefillWarnStatusFromFCU = EwpCoolantRefillWarnStatusFromFCU::OFF;
    EwpCoolantErrorStatus mEwpCoolantErrorStatus = EwpCoolantErrorStatus::OFF;
    EwpCoolantRefillWarnStatusFromEMS mEwpCoolantRefillWarnStatusFromEMS = EwpCoolantRefillWarnStatusFromEMS::OFF;
};

}  // namespace ccos

#endif  // SFSS_Inverter_Coolant_Warning_H
