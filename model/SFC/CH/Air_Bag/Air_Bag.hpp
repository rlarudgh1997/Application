/**
 * @file Air_Bag.hpp
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
#ifndef SFSS_Air_Bag_H
#define SFSS_Air_Bag_H

#define DLOG_ENABLED gEnableSFCLog

#include "Air_BagBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 4.0.1
// Reference : [CH050] Air_Bag. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Air_Bag : public Air_BagBase {
public:
    Air_Bag() = default;
    ~Air_Bag() override = default;
    Air_Bag(const Air_Bag& other) = delete;
    Air_Bag(Air_Bag&& other) noexcept = delete;
    Air_Bag& operator=(const Air_Bag& other) = delete;
    Air_Bag& operator=(Air_Bag&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAir_BagTelltaleAirBagStat(SFCAir_BagTelltaleAirBagStat::OFF);
        setSFCAir_BagEventSwivelSeatMotionStat(SFCAir_BagEventSwivelSeatMotionStat::OFF);
        setSFCAir_BagTelltaleAirBagBlinkValueA(SFCAir_BagTelltaleAirBagBlinkValueA::ON);
        setSFCAir_BagTelltaleAirBagBlinkValueB(SFCAir_BagTelltaleAirBagBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateOutputSignal();
        updateTelltale();
        updateSwivelSeatMotionEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
        updateSwivelSeatMotionEvent();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
        updateSwivelSeatMotionEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateOutputSignal();
        updateTelltale();
        updateSwivelSeatMotionEvent();
    }

    void onConfigAirbagChanged(const ArgumentsConfigAirbagChanged& args) {
        mConfigAIRBAG = args.mInter_ConfigAIRBAG;
        updateOutputSignal();
        updateTelltale();
        updateSwivelSeatMotionEvent();
    }

    void onAcuWarnChanged(const ArgumentsAcuWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_AcuWarnStatus) == true) {
            mAcuWarnStatus = AcuWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mAcuWarnStatus = args.mInput_AcuWarnStatus;
        }
        updateOutputSignal();
        updateTelltale();
    }

    void onSwivelSeatWarnChanged(const ArgumentsSwivelSeatWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_SwivelSeatWarnStatus) == true) {
            mSwivelSeatWarnStatus = SwivelSeatWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mSwivelSeatWarnStatus = args.mInput_SwivelSeatWarnStatus;
        }
        updateSwivelSeatMotionEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateOutputSignal() {
        AddressValueList list;
        if (mConfigAIRBAG == ConfigAIRBAG::ON && mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mAcuWarnStatus == AcuWarnStatus::OFF) {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::INDI_OFF));
            } else if (mAcuWarnStatus == AcuWarnStatus::ON) {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::INDI_ON));
            } else if (mAcuWarnStatus == AcuWarnStatus::FLASH) {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::INDI_FLASHING));
            } else if (mAcuWarnStatus == AcuWarnStatus::INVALID) {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::MESSAGE_ERROR));
            } else if (mAcuWarnStatus == AcuWarnStatus::UNHANDLED_TIMEOUT) {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::MESSAGE_ERROR));
            } else {
                list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                                  static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::INDI_OFF));
            }
        } else {
            list.emplace_back(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus.getAddress(),
                              static_cast<ccos::HUInt64>(OutputAirBagIndictorStatusType::INDI_OFF));
        }
        setValue(list);
    }

    void updateTelltale() {
        SFCAir_BagTelltaleAirBagStat stat = SFCAir_BagTelltaleAirBagStat::OFF;

        if (mConfigAIRBAG == ConfigAIRBAG::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            // no operation
        } else if (mConfigAIRBAG == ConfigAIRBAG::ON && mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCAir_BagTelltaleAirBagStat::ON;
        } else if (mConfigAIRBAG == ConfigAIRBAG::ON && mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mAcuWarnStatus == AcuWarnStatus::ON || mAcuWarnStatus == AcuWarnStatus::UNHANDLED_TIMEOUT) {
                stat = SFCAir_BagTelltaleAirBagStat::ON;
            } else if (mAcuWarnStatus == AcuWarnStatus::FLASH) {
                stat = SFCAir_BagTelltaleAirBagStat::BLINK2;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        setSFCAir_BagTelltaleAirBagStat(stat);
    }

    void updateSwivelSeatMotionEvent() {
        std::string eventID;
        std::string soundID;
        if (mConfigAIRBAG == ConfigAIRBAG::ON && mIgnElapsed >= IgnElapsed::ON_0ms) {
            if (mSwivelSeatWarnStatus == SwivelSeatWarnStatus::SWIVEL_ON) {
                eventID = "E31601";
                soundID = "SND_PopUpWarn1";
            } else if (mSwivelSeatWarnStatus == SwivelSeatWarnStatus::L_ON_R_ON) {
                eventID = "E31602";
                soundID = "SND_PopUpInform2";
            } else if (mSwivelSeatWarnStatus == SwivelSeatWarnStatus::L_OFF_R_OFF) {
                eventID = "E31603";
                soundID = "SND_PopUpInform2";
            } else if (mSwivelSeatWarnStatus == SwivelSeatWarnStatus::L_ON_R_OFF) {
                eventID = "E31604";
                soundID = "SND_PopUpInform2";
            } else if (mSwivelSeatWarnStatus == SwivelSeatWarnStatus::L_OFF_R_ON) {
                eventID = "E31605";
                soundID = "SND_PopUpInform2";
            } else {
                /* Intentionally empty body for MISRA rule.  */
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Air_Bag.Event.SwivelSeatMotion.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            DDebug() << "Set previous Stat to SFCAir_BagEventSwivelSeatMotionStat::OFF ";
            setSFCAir_BagEventSwivelSeatMotionStat(SFCAir_BagEventSwivelSeatMotionStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() != true) {
            setSFCAir_BagEventSwivelSeatMotionID(eventID);
            setSFCAir_BagEventSwivelSeatMotionStat(SFCAir_BagEventSwivelSeatMotionStat::ON);
            setSFCAir_BagEventSwivelSeatMotionLinkedSoundID(soundID);
            setSFCAir_BagEventSwivelSeatMotionLinkedSoundType(SFCAir_BagEventSwivelSeatMotionLinkedSoundType::REPEAT_COUNT);
            setSFCAir_BagEventSwivelSeatMotionLinkedSoundRepeatCount(1);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using ConfigAIRBAG = ArgumentsConfigAirbagChanged::Inter_ConfigAIRBAG;
    using AcuWarnStatus = ArgumentsAcuWarnChanged::Input_AcuWarnStatus;
    using OutputAirBagIndictorStatusType = decltype(Vehicle.CH.Air_Bag.Output_AirBagIndictorStatus)::TYPE;
    using SwivelSeatWarnStatus = ArgumentsSwivelSeatWarnChanged::Input_SwivelSeatWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigAIRBAG mConfigAIRBAG = ConfigAIRBAG::OFF;
    AcuWarnStatus mAcuWarnStatus = AcuWarnStatus::OFF;
    SwivelSeatWarnStatus mSwivelSeatWarnStatus = SwivelSeatWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Air_Bag_H
