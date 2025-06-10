/**
 * @file Engine_Temperature.hpp
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
#ifndef SFSS_Engine_Temperature_H
#define SFSS_Engine_Temperature_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>

#include "Engine_TemperatureBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 7.0.0
// Reference : [PT140] Engine Temperature. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Engine_Temperature : public Engine_TemperatureBase {
public:
    Engine_Temperature() = default;
    ~Engine_Temperature() override = default;
    Engine_Temperature(const Engine_Temperature& other) = delete;
    Engine_Temperature(Engine_Temperature&& other) noexcept = delete;
    Engine_Temperature& operator=(const Engine_Temperature& other) = delete;
    Engine_Temperature& operator=(Engine_Temperature&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngine_TemperatureTelltaleCoolantTempBlinkValueA(SFCEngine_TemperatureTelltaleCoolantTempBlinkValueA::ON);
        setSFCEngine_TemperatureTelltaleCoolantTempBlinkValueB(SFCEngine_TemperatureTelltaleCoolantTempBlinkValueB::OFF);
        if (mOneShotTimer500msStatus.create(kTimerInterval500ms, this, &Engine_Temperature::onOneShotTimer500msTimeout, false) ==
            false) {
            DWarning() << "[Engine_Temperature] mOneShotTimer500msStatus Create Failed";
        }
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
        updateEvents();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionState = IgnitionState::OFF;
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void onInputEngineCoolantTempValueChanged(const ArgumentsInputEngineCoolantTempValueChanged& args) {
        mInput_EngineCoolantTempValue = args.mInput_EngineCoolantTempValue;
        mInput_EngineCoolantTempValue_timeout = ISTIMEOUT(args.mInput_EngineCoolantTempValue);
        DDebug() << "Input_EngineCoolantTempValue: " << mInput_EngineCoolantTempValue;
        DDebug() << "Input_EngineCoolantTempValue_timeout: " << mInput_EngineCoolantTempValue_timeout;
        updatePrivateCoolantWarn();
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void onInputEhrsTempWarnStatusChanged(const ArgumentsInputEhrsTempWarnStatusChanged& args) {
        mInput_EhrsTempWarnStatus = args.mInput_EhrsTempWarnStatus;
        DDebug() << "Input_EhrsTempWarnStatus: " << static_cast<HUInt64>(mInput_EhrsTempWarnStatus);
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigFuelType = args.mInter_ConfigFuelType;
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        DDebug() << "Inter_ConfigFuelType: " << static_cast<HUInt64>(mInter_ConfigFuelType);
        DDebug() << "Inter_ConfigVehicleType: " << static_cast<HUInt64>(mInter_ConfigVehicleType);
        updateTelltale();
        updateConstant();
    }

    void onEngineCoolantTempGaugeValueChanged(const ArgumentsEngineCoolantTempGaugeValueChanged& args) {
        mInput_EngineCoolantTempGaugeValue = args.mInput_EngineCoolantTempGaugeValue;
        updateConstant();
    }

    void onEngineCoolantTempWarnStatus(const ArgumentsEngineCoolantTempWarnStatus& args) {
        mInput_EngineCoolantTempWarnStatus = args.mInput_EngineCoolanttTempWarnStatus;
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void onPtLogicControlType(const ArgumentsPtLogicControlType& args) {
        mInter_ConfigPTLogicControlType = args.mInter_PTLogicControlType;
        updateTelltale();
        updateConstant();
        updateEvents();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateCoolantWarn() {
        if (mPrivate_CoolantWarn == PrivateCoolantWarn::ON) {  // Section B
            if (mInput_EngineCoolantTempValue <= 221 && mInput_EngineCoolantTempValue >= 0) {
                // 0x00 <= mInput_EngineCoolantTempValue <= 0xDD
                DDebug() << "Private_CoolantWarn::OFF";
                mPrivate_CoolantWarn = PrivateCoolantWarn::OFF;
            }
        } else {  // Section A
            if (mInput_EngineCoolantTempValue >= 225 && mInput_EngineCoolantTempValue <= 254) {
                // 0xE1 <= mInput_EngineCoolantTempValue <= 0xFE
                DDebug() << "Private_CoolantWarn::ON";
                mPrivate_CoolantWarn = PrivateCoolantWarn::ON;
            }
        }
    }

    void updateTelltale() {
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms) {
            setSFCEngine_TemperatureTelltaleCoolantTempStat(SFCEngine_TemperatureTelltaleCoolantTempStat::OFF);
            return;
        }

        if (mIgnitionElapsed == IgnitionElapsed::ON_10s && (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                                                            mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
                                                            mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV)) {
            if (mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::OFF && mInput_EngineCoolantTempValue == 255) {
                setSFCEngine_TemperatureTelltaleCoolantTempStat(SFCEngine_TemperatureTelltaleCoolantTempStat::BLINK2);
            } else if ((mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::OFF &&
                        mPrivate_CoolantWarn == PrivateCoolantWarn::ON && mInput_EngineCoolantTempValue != 255) ||
                       (mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::ON &&
                        mInput_EngineCoolantTempWarnStatus == Input_EngineCoolantTempWarnStatus::WARN_OVERHEAT) ||
                       mInput_EhrsTempWarnStatus == Input_EhrsTempWarnStatus::ON) {
                setSFCEngine_TemperatureTelltaleCoolantTempStat(SFCEngine_TemperatureTelltaleCoolantTempStat::ON);
            } else {
                setSFCEngine_TemperatureTelltaleCoolantTempStat(SFCEngine_TemperatureTelltaleCoolantTempStat::OFF);
            }
        } else {
            setSFCEngine_TemperatureTelltaleCoolantTempStat(SFCEngine_TemperatureTelltaleCoolantTempStat::OFF);
        }
    }

    void updateConstant() {
        HBool isTimerValid = false;

        if (mIgnitionState == IgnitionState::ON && mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::ON &&
            (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV)) {
            if (mInput_EngineCoolantTempGaugeValue >= 0 && mInput_EngineCoolantTempGaugeValue <= 100) {
                updateCoolantTemperature(static_cast<HDouble>(mInput_EngineCoolantTempGaugeValue));
            } else {
                updateCoolantTemperature(static_cast<HDouble>(0));
            }
            return;
        }

        if (mIgnitionState == IgnitionState::ON && mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::OFF &&
            (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV)) {
            HDouble coolantTempValue = 0.0;
            const TempStatMap tempStatMap = mInter_ConfigFuelType == Inter_ConfigFuelType::DSL ? mDieselTempMap : mTempMap;

            if (mInput_EngineCoolantTempValue_timeout) {  // timeout -> 0x00
                coolantTempValue = -5.0;
            } else {
                if (mInput_EngineCoolantTempValue < tempStatMap.at(TempStat::MIN_UNDER)) {
                    coolantTempValue =
                        getTempValuePercentage(tempStatMap, TempStat::NONE, TempStat::MIN_UNDER, mInput_EngineCoolantTempValue);
                } else if (mInput_EngineCoolantTempValue >= tempStatMap.at(TempStat::MIN_UNDER) &&
                           mInput_EngineCoolantTempValue < tempStatMap.at(TempStat::MIN)) {
                    coolantTempValue =
                        getTempValuePercentage(tempStatMap, TempStat::MIN_UNDER, TempStat::MIN, mInput_EngineCoolantTempValue);
                } else if (mInput_EngineCoolantTempValue >= tempStatMap.at(TempStat::MIN) &&
                           mInput_EngineCoolantTempValue <= tempStatMap.at(TempStat::MIDDLE_STABLE)) {
                    coolantTempValue = 45.0;
                } else if (mInput_EngineCoolantTempValue > tempStatMap.at(TempStat::MIDDLE_STABLE) &&
                           mInput_EngineCoolantTempValue <= tempStatMap.at(TempStat::MAX)) {
                    coolantTempValue = getTempValuePercentage(tempStatMap, TempStat::MIDDLE_STABLE, TempStat::MAX,
                                                              mInput_EngineCoolantTempValue);
                } else if (mInput_EngineCoolantTempValue > tempStatMap.at(TempStat::MAX) && mInput_EngineCoolantTempValue < 255) {
                    coolantTempValue =
                        getTempValuePercentage(tempStatMap, TempStat::MAX, TempStat::MAX_OVER, mInput_EngineCoolantTempValue);
                } else {  // 0xFF
                    isTimerValid = true;
                    if (mOneShotTimer500msStatus.start() == false) {
                        DWarning() << "[Engine_Temperature] mOneShotTimer500msStatus Start Failed";
                    }
                }
            }
            DDebug() << "calculated value: " << coolantTempValue;
            if (isTimerValid == false) {
                updateCoolantTemperature(coolantTempValue);
            }
        } else {
            updateCoolantTemperature(static_cast<HDouble>(0));
        }

        if (isTimerValid == false) {
            mOneShotTimer500msStatus.stop();
        }
    }

    void updateEvents() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        HUInt64 linkedSoundBlinkOnDuration = 0;
        HUInt64 linkedSoundBlinkOffDuration = 0;
        HUInt64 linkedSoundRepeatCount = 0;
        SFCEngine_TemperatureEventCoolantTempLinkedSoundType linkedSoundType =
            SFCEngine_TemperatureEventCoolantTempLinkedSoundType::NONE;

        GETCACHEDVALUE(SFC.Engine_Temperature.Event.CoolantTemp.ID, prevEventID);

        if ((mIgnitionElapsed == IgnitionElapsed::ON_10s &&
             mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::OFF &&
             mPrivate_CoolantWarn == PrivateCoolantWarn::ON) ||
            (mIgnitionState == IgnitionState::ON && mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::ON &&
             mInput_EngineCoolantTempWarnStatus == Input_EngineCoolantTempWarnStatus::WARN_OVERHEAT)) {
            eventID = "E21101";
            linkedSoundID = "SND_PopUpWarn2";
            linkedSoundBlinkOnDuration = 10000;
            linkedSoundBlinkOffDuration = 30000;
            linkedSoundType = SFCEngine_TemperatureEventCoolantTempLinkedSoundType::BLINK;
        } else if (mIgnitionState == IgnitionState::ON && mInter_ConfigPTLogicControlType == Inter_ConfigPTLogicControlType::ON &&
                   mInput_EngineCoolantTempWarnStatus == Input_EngineCoolantTempWarnStatus::WARN_TEMPHIGH) {
            eventID = "E21102";
            linkedSoundID = "SND_PopUpWarn1";
            linkedSoundType = SFCEngine_TemperatureEventCoolantTempLinkedSoundType::REPEAT_COUNT;
            linkedSoundRepeatCount = 1;
        } else {
            // nothing
        }

        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCEngine_TemperatureEventCoolantTempStat(SFCEngine_TemperatureEventCoolantTempStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCEngine_TemperatureEventCoolantTempID(eventID);
            setSFCEngine_TemperatureEventCoolantTempStat(SFCEngine_TemperatureEventCoolantTempStat::ON);
            setSFCEngine_TemperatureEventCoolantTempLinkedSoundID(linkedSoundID);
            setSFCEngine_TemperatureEventCoolantTempLinkedSoundType(linkedSoundType);
            setSFCEngine_TemperatureEventCoolantTempLinkedSoundBlinkOnDuration(linkedSoundBlinkOnDuration);
            setSFCEngine_TemperatureEventCoolantTempLinkedSoundBlinkOffDuration(linkedSoundBlinkOffDuration);
            setSFCEngine_TemperatureEventCoolantTempLinkedSoundRepeatCount(linkedSoundRepeatCount);
        }
    }

    void updateCoolantTemperature(const HDouble& value) {
        mEngineCoolantTempPercentageValue = value;
        DDebug() << "Engine_Temperature: " << mEngineCoolantTempPercentageValue;
        setSFCEngine_TemperatureConstantCoolantTempGaugeValue(round((mEngineCoolantTempPercentageValue)*10000) / 10000);
    }

    enum class TempStat : HUInt64;
    HDouble getTempValuePercentage(const std::map<TempStat, HUInt64>& tempMap, const TempStat& range_start,
                                   const TempStat& range_end, const HUInt64& value) {
        HDouble percentage = 0;
        if (value >= tempMap.at(range_start) && value <= tempMap.at(range_end)) {
            percentage =
                (static_cast<HDouble>(value) - static_cast<HDouble>(tempMap.at(range_start))) *
                    (static_cast<HDouble>(mTempValueMap.at(range_end)) - static_cast<HDouble>(mTempValueMap.at(range_start))) /
                    (static_cast<HDouble>(tempMap.at(range_end)) - static_cast<HDouble>(tempMap.at(range_start))) +
                static_cast<HDouble>(mTempValueMap.at(range_start));
        }
        return percentage;
    }

    void onOneShotTimer500msTimeout() {
        updateCoolantTemperature(105.0);
        flush();
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

    enum class PrivateCoolantWarn : HUInt64 {
        OFF,
        ON
    };

    enum class TempStat : HUInt64 {
        NONE,
        MIN_UNDER,
        MIN,
        MIDDLE_STABLE,
        MAX,
        MAX_OVER
    };

    using TempStatMap = std::map<TempStat, HUInt64>;
    const TempStatMap mTempMap = {{TempStat::NONE, 0},
                                  {TempStat::MIN_UNDER, 118},      // 0x76 MIN_UNDER
                                  {TempStat::MIN, 145},            // 0x91 MIN
                                  {TempStat::MIDDLE_STABLE, 216},  // 0xD8 MIDDLE_STABLE
                                  {TempStat::MAX, 225},            // 0xE1 MAX
                                  {TempStat::MAX_OVER, 255}};      // 0xFF MAX_OVER
    const TempStatMap mDieselTempMap = {{TempStat::NONE, 0},
                                        {TempStat::MIN_UNDER, 104},      // 0x68 MIN_UNDER
                                        {TempStat::MIN, 145},            // 0x91 MIN
                                        {TempStat::MIDDLE_STABLE, 216},  // 0xD8 MIDDLE_STABLE
                                        {TempStat::MAX, 225},            // 0xE1 MAX
                                        {TempStat::MAX_OVER, 255}};      // 0xFF MAX_OVER

    using TempValueMap = std::map<TempStat, HDouble>;
    const TempValueMap mTempValueMap = {{TempStat::NONE, -5.0},          {TempStat::MIN_UNDER, 0.0}, {TempStat::MIN, 45.0},
                                        {TempStat::MIDDLE_STABLE, 45.0}, {TempStat::MAX, 100.0},     {TempStat::MAX_OVER, 105.0}};

    using Input_EhrsTempWarnStatus = ArgumentsInputEhrsTempWarnStatusChanged::Input_EhrsTempWarnStatus;
    using Inter_ConfigFuelType = ArgumentsConfigChanged::Inter_ConfigFuelType;
    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using Inter_ConfigPTLogicControlType = ArgumentsPtLogicControlType::Inter_PTLogicControlType;
    using Input_EngineCoolantTempWarnStatus = ArgumentsEngineCoolantTempWarnStatus::Input_EngineCoolanttTempWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    IgnitionState mIgnitionState = IgnitionState::NONE;
    HUInt64 mInput_EngineCoolantTempValue = 0;
    HBool mInput_EngineCoolantTempValue_timeout = false;
    HDouble mEngineCoolantTempPercentageValue = 0.0;
    Inter_ConfigPTLogicControlType mInter_ConfigPTLogicControlType = Inter_ConfigPTLogicControlType::OFF;
    HUInt64 mInput_EngineCoolantTempGaugeValue = 0;
    Input_EngineCoolantTempWarnStatus mInput_EngineCoolantTempWarnStatus = Input_EngineCoolantTempWarnStatus::OFF;
    Input_EhrsTempWarnStatus mInput_EhrsTempWarnStatus = Input_EhrsTempWarnStatus::OFF;
    Inter_ConfigFuelType mInter_ConfigFuelType = Inter_ConfigFuelType::GSL;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;

    PrivateCoolantWarn mPrivate_CoolantWarn = PrivateCoolantWarn::OFF;
    ssfs::SFCTimer<Engine_Temperature> mOneShotTimer500msStatus;
    static constexpr uint32_t kTimerInterval500ms = 500u;
};

}  // namespace ccos

#endif  // SFSS_Engine_Temperature_H
