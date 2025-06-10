/**
 * @file ECS.hpp
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
#ifndef SFSS_ECS_H
#define SFSS_ECS_H

#define DLOG_ENABLED gEnableSFCLog

#include "ECSBase.hpp"

namespace ccos {

// SFC Version : 0.1.1
// Reference : [CH070] ECS. Contains : Event. Recommend: VALUE_CHANGED
class ECS : public ECSBase {
public:
    ECS() = default;
    ~ECS() override = default;
    ECS(const ECS& other) = delete;
    ECS(ECS&& other) noexcept = delete;
    ECS& operator=(const ECS& other) = delete;
    ECS& operator=(ECS&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        DDebug() << "IgnitionElapsed::ON_0ms";
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateEcsSysFailWarnStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        DDebug() << "IgnitionElapsed::OFF_0ms";
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateEcsSysFailWarnStatus();
    }

    void onValueChanged(const ArgumentsValueChanged& args) {
        DDebug() << "Inter_ConfigECS " << static_cast<HUInt64>(args.mInter_ConfigECS);
        DDebug() << "Input_EcsSysFailWarnStatus " << static_cast<HUInt64>(args.mInput_EcsSysFailWarnStatus);
        mInterConfigECS = args.mInter_ConfigECS;
        if (ISTIMEOUT(args.mInput_EcsSysFailWarnStatus) == true) {
            mInputEcsSysFailWarnStatus = InputEcsSysFailWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputEcsSysFailWarnStatus = args.mInput_EcsSysFailWarnStatus;
        }
        updateEcsSysFailWarnStatus();
    }

private:
    void updateEcsSysFailWarnStatus() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms && mInterConfigECS == InterConfigECS::ON) {
            if (mInputEcsSysFailWarnStatus == InputEcsSysFailWarnStatus::WARN ||
                mInputEcsSysFailWarnStatus == InputEcsSysFailWarnStatus::UNHANDLED_TIMEOUT) {
                DDebug() << "SFCECSEventECSStat::ON";
                updateEventSignal(SFCECSEventECSStat::ON);
            } else {
                DDebug() << "SFCECSEventECSStat::OFF";
                updateEventSignal(SFCECSEventECSStat::OFF);
            }
        } else {
            DDebug() << "SFCECSEventECSStat::OFF";
            updateEventSignal(SFCECSEventECSStat::OFF);
        }
    }

    void updateEventSignal(const SFCECSEventECSStat& stat) {
        setSFCECSEventECSID("E30401");
        setSFCECSEventECSStat(stat);
        setSFCECSEventECSLinkedSoundType(SFCECSEventECSLinkedSoundType::REPEAT_COUNT);
        setSFCECSEventECSLinkedSoundRepeatCount(1);
        setSFCECSEventECSLinkedSoundID("SND_PopUpWarn1");
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using InterConfigECS = ArgumentsValueChanged::Inter_ConfigECS;
    using InputEcsSysFailWarnStatus = ArgumentsValueChanged::Input_EcsSysFailWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    InterConfigECS mInterConfigECS = InterConfigECS::OFF;
    InputEcsSysFailWarnStatus mInputEcsSysFailWarnStatus = InputEcsSysFailWarnStatus::NO_WARN;
};

}  // namespace ccos

#endif  // SFSS_ECS_H
