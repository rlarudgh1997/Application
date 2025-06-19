/**
 * @file Engine_Oil_Warning_CV.hpp
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
#ifndef SFSS_Engine_Oil_Warning_CV_H
#define SFSS_Engine_Oil_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_Oil_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.17
// Reference : [CV780] Engine_Oil_Warning_CV. Contains : Telltale, Constant, Sound. Recommend: VALUE_CHANGED
class Engine_Oil_Warning_CV : public Engine_Oil_Warning_CVBase {
public:
    Engine_Oil_Warning_CV() = default;
    ~Engine_Oil_Warning_CV() override = default;
    Engine_Oil_Warning_CV(const Engine_Oil_Warning_CV& other) = delete;
    Engine_Oil_Warning_CV(Engine_Oil_Warning_CV&& other) noexcept = delete;
    Engine_Oil_Warning_CV& operator=(const Engine_Oil_Warning_CV& other) = delete;
    Engine_Oil_Warning_CV& operator=(Engine_Oil_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundID("SND_EngineOilPressureLow");
        setSFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat(
            SFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat::OFF);
        setSFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundType(
            SFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltale();
        updateConstant();
        updateSound();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltale();
        updateConstant();
        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltale();
        updateConstant();
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
        updateConstant();
        updateSound();
    }

    void onEngineOilWarnStatusChanged(const ArgumentsEngineOilWarnStatusChanged& args) {
        mEngineOilPressureWarningStatus = args.mInput_EngineOilPressureWarningStatus;
        mEngineOilFilterRestrictionSwitchStatus = args.mInput_EngineOilFilterRestrictionSwitchStatus;
        updateTelltale();
    }

    void onEngineOilPressureValueChanged(const ArgumentsEngineOilPressureValueChanged& args) {
        mEngineOilPressure = args.mInput_EngineOilPressure;
        updateConstant();
    }

    void onEngineOilPressureLowSoundChanged(const ArgumentsEngineOilPressureLowSoundChanged& args) {
        mEngineOilPressureLowSound = args.mInput_EngineOilPressureLowSound;
        updateSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }
        SFCEngine_Oil_Warning_CVTelltaleEngineOilWarnStat telltaleStat = SFCEngine_Oil_Warning_CVTelltaleEngineOilWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            telltaleStat = SFCEngine_Oil_Warning_CVTelltaleEngineOilWarnStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms &&
                   (mEngineOilPressureWarningStatus == EngineOilPressureWarningStatus::ON ||
                    mEngineOilFilterRestrictionSwitchStatus == EngineOilFilterRestrictionSwitchStatus::ON)) {
            telltaleStat = SFCEngine_Oil_Warning_CVTelltaleEngineOilWarnStat::ON;
        } else {
            // defualt OFF
        }
        setSFCEngine_Oil_Warning_CVTelltaleEngineOilWarnStat(telltaleStat);
    }

    void updateConstant() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mEngineOilPressure >= 0 && mEngineOilPressure <= 250) {
                value = mEngineOilPressure;
            }
        }
        setSFCEngine_Oil_Warning_CVConstantEngineOilPressureValue(value);
    }

    void updateSound() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        SFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat state =
            SFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat::OFF;
        if (mIsIgnOn == true) {
            if (mEngineOilPressureLowSound == EngineOilPressureLowSound::ON) {
                state = SFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat::ON;
            }
        }
        setSFCEngine_Oil_Warning_CVSoundEngineOilPressureLowSoundStat(state);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using EngineOilPressureWarningStatus = ArgumentsEngineOilWarnStatusChanged::Input_EngineOilPressureWarningStatus;
    using EngineOilFilterRestrictionSwitchStatus =
        ArgumentsEngineOilWarnStatusChanged::Input_EngineOilFilterRestrictionSwitchStatus;
    using EngineOilPressureLowSound = ArgumentsEngineOilPressureLowSoundChanged::Input_EngineOilPressureLowSound;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EngineOilPressureWarningStatus mEngineOilPressureWarningStatus = EngineOilPressureWarningStatus::OFF;
    EngineOilFilterRestrictionSwitchStatus mEngineOilFilterRestrictionSwitchStatus = EngineOilFilterRestrictionSwitchStatus::OFF;
    EngineOilPressureLowSound mEngineOilPressureLowSound = EngineOilPressureLowSound::OFF;

    HBool mIsIgnOn = false;
    HUInt64 mEngineOilPressure = 0;
};

}  // namespace ccos

#endif  // SFSS_Engine_Oil_Warning_CV_H
