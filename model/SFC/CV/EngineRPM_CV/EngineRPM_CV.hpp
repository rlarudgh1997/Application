/**
 * @file EngineRPM_CV.hpp
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
#ifndef SFSS_EngineRPM_CV_H
#define SFSS_EngineRPM_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "EngineRPM_CVBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.16
// Reference : [CV800] EngineRPM_CV. Contains : Constant, Event. Recommend: VALUE_CHANGED
class EngineRPM_CV : public EngineRPM_CVBase {
public:
    EngineRPM_CV() = default;
    ~EngineRPM_CV() override = default;
    EngineRPM_CV(const EngineRPM_CV& other) = delete;
    EngineRPM_CV(EngineRPM_CV&& other) noexcept = delete;
    EngineRPM_CV& operator=(const EngineRPM_CV& other) = delete;
    EngineRPM_CV& operator=(EngineRPM_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngineRPM_CVEventEngineLowBatteryIdleUpID("E78001");
        setSFCEngineRPM_CVEventEngineLowBatteryIdleUpLinkedSoundID("SND_PopUpInform2");
        setSFCEngineRPM_CVEventEngineLowBatteryIdleUpLinkedSoundRepeatCount(1);
        setSFCEngineRPM_CVEventEngineLowBatteryIdleUpLinkedSoundType(
            SFCEngineRPM_CVEventEngineLowBatteryIdleUpLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEvent();
        updateConstantRPMValue();
        updateConstantRPMMaxRedZone();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEvent();
        updateConstantRPMValue();
        updateConstantRPMMaxRedZone();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEvent();
        updateConstantRPMValue();
        updateConstantRPMMaxRedZone();
    }

    void onConfigRPMGaugeTypeCVChanged(const ArgumentsConfigRPMGaugeTypeCVChanged& args) {
        mConfigRPMGaugeType_CV = args.mInter_ConfigRPMGaugeType_CV;
        updateConstantRPMMaxRedZone();
    }

    void onEngineLowBatteryIdleUpChanged(const ArgumentsEngineLowBatteryIdleUpChanged& args) {
        mEngineLowBatteryIdleUp = args.mInput_EngineLowBatteryIdleUp;
        updateEvent();
    }

    void onEngineRPMDisplayValueCVChanged(const ArgumentsEngineRPMDisplayValueCVChanged& args) {
        mEngineRPMDisplayValue_CV = args.mInput_EngineRPMDisplayValue_CV;
        updateConstantRPMValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEvent() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        SFCEngineRPM_CVEventEngineLowBatteryIdleUpStat stat = SFCEngineRPM_CVEventEngineLowBatteryIdleUpStat::OFF;
        if (mIsIgnOn == true && mEngineLowBatteryIdleUp == EngineLowBatteryIdleUp::ON) {
            stat = SFCEngineRPM_CVEventEngineLowBatteryIdleUpStat::ON;
        }

        setSFCEngineRPM_CVEventEngineLowBatteryIdleUpStat(stat);
    }

    void updateConstantRPMValue() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mEngineRPMDisplayValue_CV >= 0 && mEngineRPMDisplayValue_CV <= 0xFAFF) {
                value = mEngineRPMDisplayValue_CV;
            }
        }

        setSFCEngineRPM_CVConstantRPMValueCVValue(value);
    }

    void updateConstantRPMMaxRedZone() {
        if (mConfigVehicleType != ConfigVehicleType::ICV) {
            return;
        }

        SFCEngineRPM_CVConstantRPMMaxRedZoneStat stat = SFCEngineRPM_CVConstantRPMMaxRedZoneStat::TYPE1;

        if (mIsIgnOn == true) {
            if (mConfigRPMGaugeType_CV == ConfigRPMGaugeType_CV::TYPE2) {
                stat = SFCEngineRPM_CVConstantRPMMaxRedZoneStat::TYPE2;
            }
        }
        setSFCEngineRPM_CVConstantRPMMaxRedZoneStat(stat);
    }

    using EngineLowBatteryIdleUp = ArgumentsEngineLowBatteryIdleUpChanged::Input_EngineLowBatteryIdleUp;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigRPMGaugeType_CV = ArgumentsConfigRPMGaugeTypeCVChanged::Inter_ConfigRPMGaugeType_CV;

    HBool mIsIgnOn = false;
    EngineLowBatteryIdleUp mEngineLowBatteryIdleUp = EngineLowBatteryIdleUp::OFF;
    HUInt64 mEngineRPMDisplayValue_CV = 0;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigRPMGaugeType_CV mConfigRPMGaugeType_CV = ConfigRPMGaugeType_CV::TYPE1;
};

}  // namespace ccos

#endif  // SFSS_EngineRPM_CV_H
