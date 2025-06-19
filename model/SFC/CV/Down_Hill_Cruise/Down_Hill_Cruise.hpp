/**
 * @file Down_Hill_Cruise.hpp
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
#ifndef SFSS_Down_Hill_Cruise_H
#define SFSS_Down_Hill_Cruise_H

#define DLOG_ENABLED gEnableSFCLog

#include "Down_Hill_CruiseBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.33
// Reference : [CV940] Down_Hill_Cruise. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Down_Hill_Cruise : public Down_Hill_CruiseBase {
public:
    Down_Hill_Cruise() = default;
    ~Down_Hill_Cruise() override = default;
    Down_Hill_Cruise(const Down_Hill_Cruise& other) = delete;
    Down_Hill_Cruise(Down_Hill_Cruise&& other) noexcept = delete;
    Down_Hill_Cruise& operator=(const Down_Hill_Cruise& other) = delete;
    Down_Hill_Cruise& operator=(Down_Hill_Cruise&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateConstantDHCUnitStat();
        updateConstantDHCAbleToUpDownStat();
        updateConstantDHCEventToleranceValue();
        updateConstantDHCSummaryColorStat();
        updateEventDownHillCruise();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateConstantDHCUnitStat();
        updateConstantDHCAbleToUpDownStat();
        updateConstantDHCEventToleranceValue();
        updateConstantDHCSummaryColorStat();
        updateEventDownHillCruise();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;

        updateConstantDHCUnitStat();
        updateConstantDHCAbleToUpDownStat();
        updateEventDownHillCruise();
    }

    void onDhcToleranceValueChanged(const ArgumentsDhcToleranceValueChanged& args) {
        mDHCToleranceValue = args.mInput_DHCToleranceValue;

        updateConstantDHCAbleToUpDownStat();
        updateConstantDHCEventToleranceValue();
        updateEventDownHillCruise();
    }

    void onDhcToleranceSoundStatusChanged(const ArgumentsDhcToleranceSoundStatusChanged& args) {
        mDHCToleranceSoundStatus = args.mInput_DHCToleranceSoundStatus;

        updateEventDownHillCruise();
    }

    void onDhcStateChanged(const ArgumentsDhcStateChanged& args) {
        mDHCState = args.mInput_DHCState;

        updateConstantDHCSummaryColorStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantDHCUnitStat() {
        SFCDown_Hill_CruiseConstantDHCUnitStat stat = SFCDown_Hill_CruiseConstantDHCUnitStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCDown_Hill_CruiseConstantDHCUnitStat::KPH;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCDown_Hill_CruiseConstantDHCUnitStat::MPH;
            } else {
                // no operation
            }
        }
        setSFCDown_Hill_CruiseConstantDHCUnitStat(stat);
    }

    void updateConstantDHCAbleToUpDownStat() {
        SFCDown_Hill_CruiseConstantDHCAbleToUpDownStat stat = SFCDown_Hill_CruiseConstantDHCAbleToUpDownStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mTripDistanceUnit == TripDistanceUnit::KM && mDHCToleranceValue == 0x04) ||
                (mTripDistanceUnit == TripDistanceUnit::MILE && mDHCToleranceValue == 0x02)) {
                stat = SFCDown_Hill_CruiseConstantDHCAbleToUpDownStat::ABLETO_UP;
            } else if ((mTripDistanceUnit == TripDistanceUnit::KM &&
                        (0x05 <= mDHCToleranceValue && mDHCToleranceValue <= 0x0F)) ||
                       (mTripDistanceUnit == TripDistanceUnit::MILE &&
                        (0x03 <= mDHCToleranceValue && mDHCToleranceValue <= 0x0A))) {
                stat = SFCDown_Hill_CruiseConstantDHCAbleToUpDownStat::ABLETO_UPDOWN;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCDown_Hill_CruiseConstantDHCAbleToUpDownStat(stat);
    }

    void updateConstantDHCEventToleranceValue() {
        HUInt64 value = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms && (0x02 <= mDHCToleranceValue && mDHCToleranceValue <= 0x0F)) {
            value = mDHCToleranceValue;
        }
        setSFCDown_Hill_CruiseConstantDHCEventToleranceValue(value);
    }

    void updateConstantDHCSummaryColorStat() {
        SFCDown_Hill_CruiseConstantDHCSummaryColorStat stat = SFCDown_Hill_CruiseConstantDHCSummaryColorStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && (mDHCState == DHCState::READY || mDHCState == DHCState::ACTIVE)) {
            stat = SFCDown_Hill_CruiseConstantDHCSummaryColorStat::WHITE;
        }
        setSFCDown_Hill_CruiseConstantDHCSummaryColorStat(stat);
    }

    void updateEventDownHillCruise() {
        std::string eventID;
        std::string linkedSoundID;
        SFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType linkedSoundType =
            SFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        static HUInt64 mPrevDHCToleranceValue = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mTripDistanceUnit == TripDistanceUnit::KM && mDHCToleranceSoundStatus == DHCToleranceSoundStatus::ON &&
                mDHCToleranceValue == 0x04) {
                eventID = "E79401";
                linkedSoundID = "SND_PopUpInform1";
                linkedSoundType = SFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mTripDistanceUnit == TripDistanceUnit::KM && mDHCToleranceSoundStatus == DHCToleranceSoundStatus::OFF &&
                       ((mPrevDHCToleranceValue == 0x04 && mDHCToleranceValue == 0x05) ||
                        (mPrevDHCToleranceValue == 0x05 && mDHCToleranceValue == 0x04) ||
                        (mPrevDHCToleranceValue == 0x05 && mDHCToleranceValue == 0x06) ||
                        (mPrevDHCToleranceValue == 0x06 && mDHCToleranceValue == 0x05) ||
                        (mPrevDHCToleranceValue == 0x06 && mDHCToleranceValue == 0x07) ||
                        (mPrevDHCToleranceValue == 0x07 && mDHCToleranceValue == 0x06) ||
                        (mPrevDHCToleranceValue == 0x07 && mDHCToleranceValue == 0x08) ||
                        (mPrevDHCToleranceValue == 0x08 && mDHCToleranceValue == 0x07) ||
                        (mPrevDHCToleranceValue == 0x08 && mDHCToleranceValue == 0x09) ||
                        (mPrevDHCToleranceValue == 0x09 && mDHCToleranceValue == 0x08) ||
                        (mPrevDHCToleranceValue == 0x09 && mDHCToleranceValue == 0x0A) ||
                        (mPrevDHCToleranceValue == 0x0A && mDHCToleranceValue == 0x09) ||
                        (mPrevDHCToleranceValue == 0x0A && mDHCToleranceValue == 0x0B) ||
                        (mPrevDHCToleranceValue == 0x0B && mDHCToleranceValue == 0x0A) ||
                        (mPrevDHCToleranceValue == 0x0B && mDHCToleranceValue == 0x0C) ||
                        (mPrevDHCToleranceValue == 0x0C && mDHCToleranceValue == 0x0B) ||
                        (mPrevDHCToleranceValue == 0x0C && mDHCToleranceValue == 0x0D) ||
                        (mPrevDHCToleranceValue == 0x0D && mDHCToleranceValue == 0x0C) ||
                        (mPrevDHCToleranceValue == 0x0D && mDHCToleranceValue == 0x0E) ||
                        (mPrevDHCToleranceValue == 0x0E && mDHCToleranceValue == 0x0D) ||
                        (mPrevDHCToleranceValue == 0x0E && mDHCToleranceValue == 0x0F) ||
                        (mPrevDHCToleranceValue == 0x0F && mDHCToleranceValue == 0x0E) ||
                        (mPrevDHCToleranceValue == 0x1D && mDHCToleranceValue == 0x0F))) {
                eventID = "E79401";
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE && mDHCToleranceSoundStatus == DHCToleranceSoundStatus::ON &&
                       mDHCToleranceValue == 0x02) {
                eventID = "E79402";
                linkedSoundID = "SND_PopUpInform1";
                linkedSoundType = SFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE && mDHCToleranceSoundStatus == DHCToleranceSoundStatus::OFF &&
                       ((mPrevDHCToleranceValue == 0x02 && mDHCToleranceValue == 0x03) ||
                        (mPrevDHCToleranceValue == 0x03 && mDHCToleranceValue == 0x02) ||
                        (mPrevDHCToleranceValue == 0x03 && mDHCToleranceValue == 0x04) ||
                        (mPrevDHCToleranceValue == 0x04 && mDHCToleranceValue == 0x03) ||
                        (mPrevDHCToleranceValue == 0x04 && mDHCToleranceValue == 0x05) ||
                        (mPrevDHCToleranceValue == 0x05 && mDHCToleranceValue == 0x04) ||
                        (mPrevDHCToleranceValue == 0x05 && mDHCToleranceValue == 0x06) ||
                        (mPrevDHCToleranceValue == 0x06 && mDHCToleranceValue == 0x05) ||
                        (mPrevDHCToleranceValue == 0x06 && mDHCToleranceValue == 0x07) ||
                        (mPrevDHCToleranceValue == 0x07 && mDHCToleranceValue == 0x06) ||
                        (mPrevDHCToleranceValue == 0x07 && mDHCToleranceValue == 0x08) ||
                        (mPrevDHCToleranceValue == 0x08 && mDHCToleranceValue == 0x07) ||
                        (mPrevDHCToleranceValue == 0x08 && mDHCToleranceValue == 0x09) ||
                        (mPrevDHCToleranceValue == 0x09 && mDHCToleranceValue == 0x08) ||
                        (mPrevDHCToleranceValue == 0x09 && mDHCToleranceValue == 0x0A) ||
                        (mPrevDHCToleranceValue == 0x0A && mDHCToleranceValue == 0x09) ||
                        (mPrevDHCToleranceValue == 0x1D && mDHCToleranceValue == 0x0A))) {
                eventID = "E79402";
            } else if (mDHCToleranceSoundStatus == DHCToleranceSoundStatus::ON && mDHCToleranceValue == 0x1D) {
                eventID = "E79403";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Down_Hill_Cruise.Event.DownHillCruise.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDown_Hill_CruiseEventDownHillCruiseStat(SFCDown_Hill_CruiseEventDownHillCruiseStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCDown_Hill_CruiseEventDownHillCruiseID(eventID);
            setSFCDown_Hill_CruiseEventDownHillCruiseStat(SFCDown_Hill_CruiseEventDownHillCruiseStat::ON);
            setSFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundID(linkedSoundID);
            setSFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundType(linkedSoundType);
            setSFCDown_Hill_CruiseEventDownHillCruiseLinkedSoundRepeatCount(repeatCount);
        }

        mPrevDHCToleranceValue = mDHCToleranceValue;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using DHCToleranceSoundStatus = ArgumentsDhcToleranceSoundStatusChanged::Input_DHCToleranceSoundStatus;
    using DHCState = ArgumentsDhcStateChanged::Input_DHCState;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;
    DHCToleranceSoundStatus mDHCToleranceSoundStatus = DHCToleranceSoundStatus::OFF;
    DHCState mDHCState = DHCState::OFF;
    HUInt64 mDHCToleranceValue = 0;
};

}  // namespace ccos

#endif  // SFSS_Down_Hill_Cruise_H
