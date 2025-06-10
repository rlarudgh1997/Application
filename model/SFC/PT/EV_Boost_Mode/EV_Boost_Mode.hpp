/**
 * @file EV_Boost_Mode.hpp
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
#ifndef SFSS_EV_Boost_Mode_H
#define SFSS_EV_Boost_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "EV_Boost_ModeBase.hpp"

namespace ccos {

// SFC Version : 9.1.0
// Reference : [PT460] EV Boost Mode. Contains : Constant, Event. Recommend: VALUE_CHANGED
class EV_Boost_Mode : public EV_Boost_ModeBase {
public:
    EV_Boost_Mode() = default;
    ~EV_Boost_Mode() override = default;
    EV_Boost_Mode(const EV_Boost_Mode& other) = delete;
    EV_Boost_Mode(EV_Boost_Mode&& other) noexcept = delete;
    EV_Boost_Mode& operator=(const EV_Boost_Mode& other) = delete;
    EV_Boost_Mode& operator=(EV_Boost_Mode&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEV_Boost_ModeEventBoostModeWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Boost_ModeEventBoostModeWarnLinkedSoundType(SFCEV_Boost_ModeEventBoostModeWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Boost_ModeEventBoostModeWarnLinkedSoundRepeatCount(kEventRepeatCount);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstant();
        updateEventBoostModeWarn();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstant();
        updateEventBoostModeWarn();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigBoostMode = args.mInter_ConfigBoostMode;
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;
        updateConstant();
        updateEventBoostModeWarn();
    }

    void onBoostTimerChanged(const ArgumentsBoostTimerChanged& args) {
        mEvBoostModeTimerValue = args.mInput_EvBoostModeTimerValue;
        mEvBoostModeOperationStatus = args.mInput_EvBoostModeOperationStatus;
        mEvBoostModeAvailableStatus = args.mInput_EvBoostModeAvailableStatus;
        updateConstant();
    }

    void onInputEvBoostModeWarnStatusChanged(const ArgumentsInputEvBoostModeWarnStatusChanged& args) {
        mEvBoostModeWarnStatus = args.mInput_EvBoostModeWarnStatus;
        updateEventBoostModeWarn();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        SFCEV_Boost_ModeConstantBoostTimerStat stat = SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_OFF;
        HUInt64 evBoostModeTimerValue = 0;
        if (mConfigBoostMode == ConfigBoostMode::ON) {
            if (mEvBoostModeTimerValue >= 1 && mEvBoostModeTimerValue <= 20) {
                evBoostModeTimerValue = mEvBoostModeTimerValue;
            } else {
                evBoostModeTimerValue = 0;
            }
            if (mIgnElapsed == IgnElapsed::ON_0ms && mEvBoostModeAvailableStatus == EvBoostModeAvailableStatus::ACTIVATE) {
                if (mEvBoostModeOperationStatus == EvBoostModeOperationStatus::ACTIVATE) {
                    if (mEvBoostModeTimerValue == 63) {
                        stat = SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_ON_NOTIMER;
                    } else {
                        stat = SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_ON;
                    }
                } else if (mEvBoostModeOperationStatus == EvBoostModeOperationStatus::READY) {
                    stat = SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_READY;
                } else {
                    // nothing
                }
            }
        }
        setSFCEV_Boost_ModeConstantBoostTimerValue(evBoostModeTimerValue);
        setSFCEV_Boost_ModeConstantBoostTimerStat(stat);

        if (mPrevBoostTimerStat == SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_OFF &&
            stat != SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_OFF) {
            updateBoostTimerEventOFF();
        }
        mPrevBoostTimerStat = stat;
    }

    void updateEventBoostModeWarn() {
        std::string eventID;
        if (mConfigBoostMode == ConfigBoostMode::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::WARN && mConfigHighPerformance != ConfigHighPerformance::N) {
                eventID = "E24901";
            } else if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::WARN &&
                       mConfigHighPerformance == ConfigHighPerformance::N) {
                eventID = "E24904";
            } else if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::BATTERY_WARN &&
                       mConfigHighPerformance != ConfigHighPerformance::N) {
                eventID = "E24902";
            } else if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::BATTERY_WARN &&
                       mConfigHighPerformance == ConfigHighPerformance::N) {
                eventID = "E24905";
            } else if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::WARN_TIMER_ON &&
                       mConfigHighPerformance != ConfigHighPerformance::N) {
                eventID = "E24903";
            } else if (mEvBoostModeWarnStatus == EvBoostModeWarnStatus::WARN_TIMER_ON &&
                       mConfigHighPerformance == ConfigHighPerformance::N) {
                eventID = "E24906";
            } else {
                // nothing
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Boost_Mode.Event.BoostModeWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCEV_Boost_ModeEventBoostModeWarnStat(SFCEV_Boost_ModeEventBoostModeWarnStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCEV_Boost_ModeEventBoostModeWarnID(eventID);
            setSFCEV_Boost_ModeEventBoostModeWarnStat(SFCEV_Boost_ModeEventBoostModeWarnStat::ON);
        }
    }

    void updateBoostTimerEventOFF() {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Boost_Mode.Event.BoostModeWarn.ID, prevEventID);
        HUInt64 stat;
        GETCACHEDVALUE(SFC.EV_Boost_Mode.Event.BoostModeWarn.Stat, stat);
        SFCEV_Boost_ModeEventBoostModeWarnStat prevEventStat = static_cast<SFCEV_Boost_ModeEventBoostModeWarnStat>(stat);
        if ((prevEventID == "E24903" || prevEventID == "E24906") && prevEventStat == SFCEV_Boost_ModeEventBoostModeWarnStat::ON) {
            setSFCEV_Boost_ModeEventBoostModeWarnStat(SFCEV_Boost_ModeEventBoostModeWarnStat::OFF);
            flushLastGroup();
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigBoostMode = ArgumentsConfigChanged::Inter_ConfigBoostMode;
    using ConfigHighPerformance = ArgumentsConfigChanged::Inter_ConfigHighPerformance;
    using EvBoostModeAvailableStatus = ArgumentsBoostTimerChanged::Input_EvBoostModeAvailableStatus;
    using EvBoostModeOperationStatus = ArgumentsBoostTimerChanged::Input_EvBoostModeOperationStatus;
    using EvBoostModeWarnStatus = ArgumentsInputEvBoostModeWarnStatusChanged::Input_EvBoostModeWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigBoostMode mConfigBoostMode = ConfigBoostMode::OFF;
    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;
    EvBoostModeAvailableStatus mEvBoostModeAvailableStatus = EvBoostModeAvailableStatus::OFF;
    EvBoostModeOperationStatus mEvBoostModeOperationStatus = EvBoostModeOperationStatus::OFF;
    SFCEV_Boost_ModeConstantBoostTimerStat mPrevBoostTimerStat = SFCEV_Boost_ModeConstantBoostTimerStat::BOOST_OFF;
    EvBoostModeWarnStatus mEvBoostModeWarnStatus = EvBoostModeWarnStatus::OFF;

    HUInt64 mEvBoostModeTimerValue = 0;
    static constexpr HUInt64 kEventRepeatCount = 1;
};

}  // namespace ccos

#endif  // SFSS_EV_Boost_Mode_H
