/**
 * @file Air_Bag_CV.hpp
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
#ifndef SFSS_Air_Bag_CV_H
#define SFSS_Air_Bag_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Air_Bag_CVBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.24
// Reference : [CV910] Air_Bag_CV. Contains : Event. Recommend: VALUE_CHANGED
class Air_Bag_CV : public Air_Bag_CVBase {
public:
    Air_Bag_CV() = default;
    ~Air_Bag_CV() override = default;
    Air_Bag_CV(const Air_Bag_CV& other) = delete;
    Air_Bag_CV(Air_Bag_CV&& other) noexcept = delete;
    Air_Bag_CV& operator=(const Air_Bag_CV& other) = delete;
    Air_Bag_CV& operator=(Air_Bag_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAir_Bag_CVEventAirBagCVEvent79101ID("E79101");
        setSFCAir_Bag_CVEventAirBagCVEvent79101LinkedSoundID("SND_PopUpWarn1");
        setSFCAir_Bag_CVEventAirBagCVEvent79101LinkedSoundType(SFCAir_Bag_CVEventAirBagCVEvent79101LinkedSoundType::REPEAT_COUNT);
        setSFCAir_Bag_CVEventAirBagCVEvent79101LinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateOutputSignal();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateOutputSignal();
        updateEvent();
    }

    void onConfigAIRBAGCVChanged(const ArgumentsConfigAIRBAGCVChanged& args) {
        mConfigAIRBAGCV = args.mInter_ConfigAIRBAGCV;

        updateOutputSignal();
        updateEvent();
    }

    void onConfigHighVolCutoffSystemChanged(const ArgumentsConfigHighVolCutoffSystemChanged& args) {
        mConfigHighVolCutoffSystem = args.mInter_ConfigHighVolCutoffSystem;

        updateOutputSignal();
        updateEvent();
    }

    void onAcuSysWarnLampStatusChanged(const ArgumentsAcuSysWarnLampStatusChanged& args) {
        mAcuSysWarnLampStatus = args.mInput_AcuSysWarnLampStatus;

        updateOutputSignal();
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1.1 에어백 경고등 표시(일반)
    void updateOutputSignal() {
        if (mConfigAIRBAGCV != ConfigAIRBAGCV::ON && mConfigHighVolCutoffSystem != ConfigHighVolCutoffSystem::ON) {
            return;
        }

        if (mIsIgnOn == true) {
            if (mAcuSysWarnLampStatus == AcuSysWarnLampStatus::OFF) {
                updateActuatorValue(Vehicle.CV.Air_Bag_CV.Output_AirBagIndictorStatus,
                                    static_cast<HUInt64>(Output_AirBagIndictorStatus::INDI_OFF));
            } else if (mAcuSysWarnLampStatus == AcuSysWarnLampStatus::ON) {
                updateActuatorValue(Vehicle.CV.Air_Bag_CV.Output_AirBagIndictorStatus,
                                    static_cast<HUInt64>(Output_AirBagIndictorStatus::INDI_ON));
            } else if (mAcuSysWarnLampStatus == AcuSysWarnLampStatus::FLASH) {
                updateActuatorValue(Vehicle.CV.Air_Bag_CV.Output_AirBagIndictorStatus,
                                    static_cast<HUInt64>(Output_AirBagIndictorStatus::INDI_FLASHING));
            } else if (mAcuSysWarnLampStatus == AcuSysWarnLampStatus::INVALID ||
                       mAcuSysWarnLampStatus == AcuSysWarnLampStatus::MESSAGE_TIMEOUT) {
                updateActuatorValue(Vehicle.CV.Air_Bag_CV.Output_AirBagIndictorStatus,
                                    static_cast<HUInt64>(Output_AirBagIndictorStatus::MESSAGE_ERROR));
            } else {
                // no operation
            }
            flushActuatorOutput();
        }
    }

    // 5.3 이벤트 정보 표시 로직
    void updateEvent() {
        if (mConfigAIRBAGCV != ConfigAIRBAGCV::ON && mConfigHighVolCutoffSystem != ConfigHighVolCutoffSystem::ON) {
            return;
        }
        SFCAir_Bag_CVEventAirBagCVEvent79101Stat stat = SFCAir_Bag_CVEventAirBagCVEvent79101Stat::OFF;
        if (mIsIgnOn == true && mConfigHighVolCutoffSystem == ConfigHighVolCutoffSystem::ON &&
            (mAcuSysWarnLampStatus == AcuSysWarnLampStatus::ON ||
             mAcuSysWarnLampStatus == AcuSysWarnLampStatus::MESSAGE_TIMEOUT)) {
            stat = SFCAir_Bag_CVEventAirBagCVEvent79101Stat::ON;
        }
        setSFCAir_Bag_CVEventAirBagCVEvent79101Stat(stat);
    }

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    using ConfigAIRBAGCV = ArgumentsConfigAIRBAGCVChanged::Inter_ConfigAIRBAGCV;
    using ConfigHighVolCutoffSystem = ArgumentsConfigHighVolCutoffSystemChanged::Inter_ConfigHighVolCutoffSystem;
    using AcuSysWarnLampStatus = ArgumentsAcuSysWarnLampStatusChanged::Input_AcuSysWarnLampStatus;
    using Output_AirBagIndictorStatus = decltype(Vehicle.CV.Air_Bag_CV.Output_AirBagIndictorStatus)::TYPE;

    bool mIsIgnOn = false;
    ConfigAIRBAGCV mConfigAIRBAGCV = ConfigAIRBAGCV::OFF;
    ConfigHighVolCutoffSystem mConfigHighVolCutoffSystem = ConfigHighVolCutoffSystem::OFF;
    AcuSysWarnLampStatus mAcuSysWarnLampStatus = AcuSysWarnLampStatus::OFF;
    AddressValueList mActuatorOutputAddrList;
};

}  // namespace ccos

#endif  // SFSS_Air_Bag_CV_H
