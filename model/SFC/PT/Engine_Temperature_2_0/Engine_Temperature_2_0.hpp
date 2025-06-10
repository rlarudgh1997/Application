/**
 * @file Engine_Temperature_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Engine_Temperature_2_0_H
#define SFSS_Engine_Temperature_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_Temperature_2_0Base.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [PT141] Engine Temperature 2.0. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Engine_Temperature_2_0 : public Engine_Temperature_2_0Base {
public:
    Engine_Temperature_2_0() = default;
    ~Engine_Temperature_2_0() override = default;
    Engine_Temperature_2_0(const Engine_Temperature_2_0& other) = delete;
    Engine_Temperature_2_0(Engine_Temperature_2_0&& other) noexcept = delete;
    Engine_Temperature_2_0& operator=(const Engine_Temperature_2_0& other) = delete;
    Engine_Temperature_2_0& operator=(Engine_Temperature_2_0&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void onIgnElapseOn10sChanged(const ArgumentsIgnElapseOn10sChanged& /*args*/) {
        mIgnitionState = IgnitionState::ON;
        mIgnitionElapsed = IgnitionElapsed::ON_10s;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionState = IgnitionState::OFF;
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void onInputEhrsTempWarnStatusChanged(const ArgumentsInputEhrsTempWarnStatusChanged& args) {
        mInput_EhrsTempWarnStatus = args.mInput_EhrsTempWarnStatus;
        DDebug() << "Input_EhrsTempWarnStatus: " << static_cast<HUInt64>(mInput_EhrsTempWarnStatus);
        updateTelltale();
    }

    void onEngineCoolantTempGaugeValueChanged(const ArgumentsEngineCoolantTempGaugeValueChanged& args) {
        mInput_EngineCoolantTempGaugeValue = args.mInput_EngineCoolantTempGaugeValue;
        updateConstant();
    }

    void onEngineCoolantTempWarnStatus(const ArgumentsEngineCoolantTempWarnStatus& args) {
        mInput_EngineCoolanttTempWarnStatus = args.mInput_EngineCoolanttTempWarnStatus;
        updateTelltale();
        updateEvents();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCEngine_Temperature_2_0TelltaleCoolantTempStat stat = SFCEngine_Temperature_2_0TelltaleCoolantTempStat::OFF;

        if (mIgnitionElapsed == IgnitionElapsed::ON_10s &&
            (mInput_EhrsTempWarnStatus == Input_EhrsTempWarnStatus::ON ||
             mInput_EngineCoolanttTempWarnStatus == Input_EngineCoolanttTempWarnStatus::WARN_OVERHEAT)) {
            stat = SFCEngine_Temperature_2_0TelltaleCoolantTempStat::ON;
        }
        setSFCEngine_Temperature_2_0TelltaleCoolantTempStat(stat);
    }

    void updateConstant() {
        HDouble value = 0.0;

        if (mIgnitionState == IgnitionState::ON && mInput_EngineCoolantTempGaugeValue >= 0 &&
            mInput_EngineCoolantTempGaugeValue <= 100) {
            value = static_cast<HDouble>(mInput_EngineCoolantTempGaugeValue);
        }
        setSFCEngine_Temperature_2_0ConstantCoolantTempValue(value);
    }

    void updateEvents() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        HUInt64 linkedSoundBlinkOnDuration = 0;
        HUInt64 linkedSoundBlinkOffDuration = 0;
        HUInt64 linkedSoundRepeatCount = 0;
        SFCEngine_Temperature_2_0EventCoolantTempLinkedSoundType linkedSoundType =
            SFCEngine_Temperature_2_0EventCoolantTempLinkedSoundType::NONE;

        if (mIgnitionState == IgnitionState::ON &&
            mInput_EngineCoolanttTempWarnStatus == Input_EngineCoolanttTempWarnStatus::WARN_OVERHEAT) {
            eventID = "E21101";
            linkedSoundID = "SND_PopUpWarn2";
            linkedSoundBlinkOnDuration = 10000;
            linkedSoundBlinkOffDuration = 30000;
            linkedSoundType = SFCEngine_Temperature_2_0EventCoolantTempLinkedSoundType::BLINK;
        } else if (mIgnitionState == IgnitionState::ON &&
                   mInput_EngineCoolanttTempWarnStatus == Input_EngineCoolanttTempWarnStatus::WARN_TEMPHIGH) {
            eventID = "E21102";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCEngine_Temperature_2_0EventCoolantTempLinkedSoundType::REPEAT_COUNT;
            linkedSoundRepeatCount = 1;
        } else {
            // nothing
        }

        GETCACHEDVALUE(SFC.Engine_Temperature_2_0.Event.CoolantTemp.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEngine_Temperature_2_0EventCoolantTempStat(SFCEngine_Temperature_2_0EventCoolantTempStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCEngine_Temperature_2_0EventCoolantTempID(eventID);
            setSFCEngine_Temperature_2_0EventCoolantTempStat(SFCEngine_Temperature_2_0EventCoolantTempStat::ON);
            setSFCEngine_Temperature_2_0EventCoolantTempLinkedSoundID(linkedSoundID);
            setSFCEngine_Temperature_2_0EventCoolantTempLinkedSoundType(linkedSoundType);
            setSFCEngine_Temperature_2_0EventCoolantTempLinkedSoundBlinkOnDuration(linkedSoundBlinkOnDuration);
            setSFCEngine_Temperature_2_0EventCoolantTempLinkedSoundBlinkOffDuration(linkedSoundBlinkOffDuration);
            setSFCEngine_Temperature_2_0EventCoolantTempLinkedSoundRepeatCount(linkedSoundRepeatCount);
        }
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_10s,
        OFF_0ms
    };

    enum class IgnitionState : HUInt64 {
        NONE,
        ON,
        OFF
    };

    using Input_EhrsTempWarnStatus = ArgumentsInputEhrsTempWarnStatusChanged::Input_EhrsTempWarnStatus;
    using Input_EngineCoolanttTempWarnStatus = ArgumentsEngineCoolantTempWarnStatus::Input_EngineCoolanttTempWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    IgnitionState mIgnitionState = IgnitionState::NONE;
    HUInt64 mInput_EngineCoolantTempGaugeValue = 0;
    Input_EngineCoolanttTempWarnStatus mInput_EngineCoolanttTempWarnStatus = Input_EngineCoolanttTempWarnStatus::OFF;
    Input_EhrsTempWarnStatus mInput_EhrsTempWarnStatus = Input_EhrsTempWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Engine_Temperature_2_0_H
