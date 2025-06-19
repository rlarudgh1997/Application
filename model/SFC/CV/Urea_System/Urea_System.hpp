/**
 * @file Urea_System.hpp
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
#ifndef SFSS_Urea_System_H
#define SFSS_Urea_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Urea_SystemBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.08
// Reference : [CV550] Urea_System. Contains : Telltale, Constant. Recommend: VALUE_CHANGED
class Urea_System : public Urea_SystemBase {
public:
    Urea_System() = default;
    ~Urea_System() override = default;
    Urea_System(const Urea_System& other) = delete;
    Urea_System(Urea_System&& other) noexcept = delete;
    Urea_System& operator=(const Urea_System& other) = delete;
    Urea_System& operator=(Urea_System&& other) noexcept = delete;

    void onInitialize() override {
        // Urea Low Level Telltale
        setSFCUrea_SystemTelltaleLowLevelBlinkValueA(SFCUrea_SystemTelltaleLowLevelBlinkValueA::ON);
        setSFCUrea_SystemTelltaleLowLevelBlinkValueB(SFCUrea_SystemTelltaleLowLevelBlinkValueB::OFF);

        // Urea System Warning Telltale
        setSFCUrea_SystemTelltaleSystemWarningBlinkValueA(SFCUrea_SystemTelltaleSystemWarningBlinkValueA::ON);
        setSFCUrea_SystemTelltaleSystemWarningBlinkValueB(SFCUrea_SystemTelltaleSystemWarningBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateConstantUreaLevelDisplayValue();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleUreaLowLevel();
        updateTelltaleUreaSystemWarning();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltaleUreaLowLevel();
        updateTelltaleUreaSystemWarning();
        updateConstantUreaLevelDisplayValue();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleUreaLowLevel();
        updateTelltaleUreaSystemWarning();
        updateConstantUreaLevelDisplayValue();
    }

    void onConfigFuelTypeChanged(const ArgumentsConfigFuelTypeChanged& args) {
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltaleUreaLowLevel();
        updateTelltaleUreaSystemWarning();
        updateConstantUreaLevelDisplayValue();
    }

    void onUreaLowLevelTelltaleChanged(const ArgumentsUreaLowLevelTelltaleChanged& args) {
        mUreaTankLevelStatus = args.mInput_UreaTankLevelStatus;
        updateTelltaleUreaLowLevel();
    }

    void onUreaSystemWarningTelltaleChanged(const ArgumentsUreaSystemWarningTelltaleChanged& args) {
        // Input_UreaTankLevelStatus 대신 Input_UreaSystemWarnStatus 를 사용 : EXNCP-27445
        mUreaSystemWarnStatus = args.mInput_UreaSystemWarnStatus;
        updateTelltaleUreaSystemWarning();
    }

    void onEngineUreaSystemWarnStatusChanged(const ArgumentsEngineUreaSystemWarnStatusChanged& args) {
        mEngineUreaSystemWarnStatus = args.mInput_EngineUreaSystemWarnStatus;
        updateTelltaleUreaSystemWarning();
    }

    void onUreaLevelDisplayValueChanged(const ArgumentsUreaLevelDisplayValueChanged& args) {
        mUreaLevelDisplayValue = args.mInput_UreaLevelDisplayValue;
        updateConstantUreaLevelDisplayValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // Urea Low Level Telltale
    void updateTelltaleUreaLowLevel() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        SFCUrea_SystemTelltaleLowLevelStat stat = SFCUrea_SystemTelltaleLowLevelStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mUreaTankLevelStatus == UreaTankLevelStatus::ON) {
                stat = SFCUrea_SystemTelltaleLowLevelStat::ON;
            } else if (mUreaTankLevelStatus == UreaTankLevelStatus::BLINK) {
                stat = SFCUrea_SystemTelltaleLowLevelStat::BLINK2;
            } else {
                // Do nothing
            }
        }

        setSFCUrea_SystemTelltaleLowLevelStat(stat);
    }

    // Urea System Warning Telltale
    void updateTelltaleUreaSystemWarning() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        SFCUrea_SystemTelltaleSystemWarningStat stat = SFCUrea_SystemTelltaleSystemWarningStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if ((mUreaSystemWarnStatus == UreaSystemWarnStatus::ON &&
                 mEngineUreaSystemWarnStatus != EngineUreaSystemWarnStatus::BLINK) ||
                (mUreaSystemWarnStatus != UreaSystemWarnStatus::BLINK &&
                 mEngineUreaSystemWarnStatus == EngineUreaSystemWarnStatus::ON)) {
                stat = SFCUrea_SystemTelltaleSystemWarningStat::ON;
            } else if (mUreaSystemWarnStatus == UreaSystemWarnStatus::BLINK ||
                       mEngineUreaSystemWarnStatus == EngineUreaSystemWarnStatus::BLINK) {
                stat = SFCUrea_SystemTelltaleSystemWarningStat::BLINK2;
            } else {
                // Do nothing
            }
        }

        setSFCUrea_SystemTelltaleSystemWarningStat(stat);
    }

    void updateConstantUreaLevelDisplayValue() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        HUInt64 level = 0;

        if (mUreaLevelDisplayValue >= 18 && mUreaLevelDisplayValue <= 250) {
            level = mUreaLevelDisplayValue;
        }

        setSFCUrea_SystemConstantUreaGaugeLevelValue(level);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using UreaTankLevelStatus = ArgumentsUreaLowLevelTelltaleChanged::Input_UreaTankLevelStatus;
    using UreaSystemWarnStatus = ArgumentsUreaSystemWarningTelltaleChanged::Input_UreaSystemWarnStatus;
    using EngineUreaSystemWarnStatus = ArgumentsEngineUreaSystemWarnStatusChanged::Input_EngineUreaSystemWarnStatus;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigFuelTypeChanged::Inter_ConfigFuelTypeCV;

    HBool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    UreaTankLevelStatus mUreaTankLevelStatus = UreaTankLevelStatus::OFF;
    UreaSystemWarnStatus mUreaSystemWarnStatus = UreaSystemWarnStatus::OFF;
    EngineUreaSystemWarnStatus mEngineUreaSystemWarnStatus = EngineUreaSystemWarnStatus::OFF;
    HUInt64 mUreaLevelDisplayValue = 0;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
};

}  // namespace ccos

#endif  // SFSS_Urea_System_H
