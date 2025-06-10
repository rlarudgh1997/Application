/**
 * @file SCR.hpp
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
#ifndef SFSS_SCR_H
#define SFSS_SCR_H

#define DLOG_ENABLED gEnableSFCLog

#include "SCRBase.hpp"

namespace ccos {

// SFC Version : 6.0.1
// Reference : [PT360] SCR. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class SCR : public SCRBase {
public:
    SCR() = default;
    ~SCR() override = default;
    SCR(const SCR& other) = delete;
    SCR(SCR&& other) noexcept = delete;
    SCR& operator=(const SCR& other) = delete;
    SCR& operator=(SCR&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSCREventScrLvlWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCSCREventScrLvlWarnLinkedSoundRepeatCount(kLinkedSoundRepeatCount1);
        setSFCSCREventScrLvlWarnLinkedSoundDuration(kLinkedSoundDuration10000ms);  // for max 10s
        setSFCSCREventIncorrectUreaWarn1LinkedSoundID("SND_PopUpWarn1");
        setSFCSCREventIncorrectUreaWarn1LinkedSoundType(SFCSCREventIncorrectUreaWarn1LinkedSoundType::REPEAT_COUNT);
        setSFCSCREventIncorrectUreaWarn1LinkedSoundRepeatCount(kLinkedSoundRepeatCount1);
        setSFCSCREventIncorrectUreaWarn2LinkedSoundID("SND_PopUpWarn1");
        setSFCSCREventIncorrectUreaWarn2LinkedSoundDuration(kLinkedSoundDuration10000ms);  // for max 10s
        setSFCSCREventIncorrectUreaWarn2LinkedSoundDurationMin(kLinkedSoundMinimumDuration0ms);
        setSFCSCREventIncorrectUreaWarn2LinkedSoundDurationMax(kLinkedSoundMaximumDuration10000ms);  // for up to 10s
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIgnStat = true;

        updateTelltale();
        updateConstantScrRegion();
        updateConstantScrUrealLvl();
        updateConstantTripDistanceUnit();
        updateConstantScrRemainDistanceValue();
        updateEventScrLevelWarnDisplayReq();
        updateEventScrIncorrectUreaWarn1();
        updateEventScrIncorrectUreaWarn2();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIgnStat = true;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIgnStat = false;

        updateTelltale();
        updateConstantScrRegion();
        updateConstantScrUrealLvl();
        updateConstantTripDistanceUnit();
        updateConstantScrRemainDistanceValue();
        updateEventScrLevelWarnDisplayReq();
        updateEventScrIncorrectUreaWarn1();
        updateEventScrIncorrectUreaWarn2();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateConstantScrRegion();
    }

    void onConfigSCRChanged(const ArgumentsConfigSCRChanged& args) {
        mConfigSCR = args.mInter_ConfigSCR;
        updateTelltale();
        updateConstantScrRegion();
        updateConstantScrUrealLvl();
        updateConstantTripDistanceUnit();
        updateConstantScrRemainDistanceValue();
        updateEventScrLevelWarnDisplayReq();
        updateEventScrIncorrectUreaWarn1();
        updateEventScrIncorrectUreaWarn2();

        // Code below is to prevent PnP Config signal issues.
        if (mConfigSCR != ConfigSCR::ON) {
            setAllSignalOff();
            enqueue();
        }
        setBlocked(mConfigSCR != ConfigSCR::ON);
    }

    void onScrLevelWarnIndiReqChanged(const ArgumentsScrLevelWarnIndiReqChanged& args) {
        mScrLevelWarnIndiReqStatus = args.mInput_ScrLevelWarnIndiReqStatus;
        updateTelltale();
    }

    void onScrUreaLevelValueChanged(const ArgumentsScrUreaLevelValueChanged& args) {
        mScrUreaLevelValue = args.mInput_ScrUreaLevelValue;
        updateConstantScrUrealLvl();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;

        updateConstantTripDistanceUnit();
        updateConstantScrSpdValue();
    }

    void onScrRemainDistanceValueChanged(const ArgumentsScrRemainDistanceValueChanged& args) {
        mScrRemainDistanceValue = args.mInput_ScrRemainDistanceValue;
        updateConstantScrRemainDistanceValue();
    }

    void onScrLevelWarnDisplyReqStatChanged(const ArgumentsScrLevelWarnDisplyReqStatChanged& args) {
        mScrLevelWarnDisplayReqStatus = args.mInput_ScrLevelWarnDisplayReqStatus;
        updateEventScrLevelWarnDisplayReq();
    }

    void onScrIncorrectUreaWarnStat1Changed(const ArgumentsScrIncorrectUreaWarnStat1Changed& args) {
        mScrIncorrectUreaWarnStatus1 = args.mInput_ScrIncorrectUreaWarnStatus1;
        updateEventScrIncorrectUreaWarn1();
    }

    void onScrIncorrectUreaWarnStat2Changed(const ArgumentsScrIncorrectUreaWarnStat2Changed& args) {
        mScrIncorrectUreaWarnStatus2 = args.mInput_ScrIncorrectUreaWarnStatus2;
        updateEventScrIncorrectUreaWarn2();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCSCRTelltaleSCRStat stat = SFCSCRTelltaleSCRStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mScrLevelWarnIndiReqStatus == ScrLevelWarnIndiReqStatus::ON) {
            stat = SFCSCRTelltaleSCRStat::ON;
        } else {
            stat = SFCSCRTelltaleSCRStat::OFF;
        }

        setSFCSCRTelltaleSCRStat(stat);
    }

    // [4.36.5.2.1] UREA 표시
    void updateConstantScrRegion() {
        if (mIgnStat) {
            if (mConfigArea == ConfigArea::EEC || mConfigArea == ConfigArea::RUSSIA) {
                setSFCSCRConstantScrRegionStat(SFCSCRConstantScrRegionStat::REGION_EU);
            } else if (mConfigArea == ConfigArea::USA || mConfigArea == ConfigArea::CAN) {
                setSFCSCRConstantScrRegionStat(SFCSCRConstantScrRegionStat::REGION_NA);
            } else {
                setSFCSCRConstantScrRegionStat(SFCSCRConstantScrRegionStat::REGION_OTHERS);
            }
        }
    }

    // [4.36.5.2.2] UREA 단계 표시 (LEVEL 0 ~ LEVEL 20)
    void updateConstantScrUrealLvl() {
        if (mIgnStat) {
            if (1 <= mScrUreaLevelValue && mScrUreaLevelValue <= 200) {
                setSFCSCRConstantScrUreaLvlValue(((mScrUreaLevelValue - 1) / 10) + 1);
            } else {
                setSFCSCRConstantScrUreaLvlValue(0);
            }
        }
    }

    // [4.36.5.2.3.1] 거리/속도 단위 표시
    void updateConstantTripDistanceUnit() {
        if (mIgnStat) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                setSFCSCRConstantScrDistanceStat(SFCSCRConstantScrDistanceStat::KM);
                setSFCSCRConstantScrSpduntStat(SFCSCRConstantScrSpduntStat::KPH);
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                setSFCSCRConstantScrDistanceStat(SFCSCRConstantScrDistanceStat::MI);
                setSFCSCRConstantScrSpduntStat(SFCSCRConstantScrSpduntStat::MPH);
            } else {
                // Not condition
            }
        }
    }

    // [4.36.5.2.3.2] 속도 값 표시
    void updateConstantScrSpdValue() {
        if (mIgnStat) {
            if (mTripDistanceUnit == TripDistanceUnit::KM &&
                mScrSpdValueForKMMap.find(mEventGroup) != mScrSpdValueForKMMap.end()) {
                setSFCSCRConstantScrSpdValue(mScrSpdValueForKMMap[mEventGroup]);
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE &&
                       mScrSpdValueForMILEMap.find(mEventGroup) != mScrSpdValueForMILEMap.end()) {
                setSFCSCRConstantScrSpdValue(mScrSpdValueForMILEMap[mEventGroup]);
            } else {
                // Not condition
            }
        }
    }

    // [4.36.5.2.3.3] 거리 값 표시
    void updateConstantScrRemainDistanceValue() {
        if (mIgnStat) {
            if (0 <= mScrRemainDistanceValue && mScrRemainDistanceValue <= 2400) {
                setSFCSCRConstantScrRemainDistanceValueValue(mScrRemainDistanceValue);
                setSFCSCRConstantScrRemainDistanceValueStat(SFCSCRConstantScrRemainDistanceValueStat::VALUE);
            } else if (2401 <= mScrRemainDistanceValue && mScrRemainDistanceValue <= 65534) {
                setSFCSCRConstantScrRemainDistanceValueValue(2400);
                setSFCSCRConstantScrRemainDistanceValueStat(SFCSCRConstantScrRemainDistanceValueStat::VALUE);
            } else if (mScrRemainDistanceValue == 65535) {
                setSFCSCRConstantScrRemainDistanceValueValue(0);
                setSFCSCRConstantScrRemainDistanceValueStat(SFCSCRConstantScrRemainDistanceValueStat::DISPLAY_OFF);
            } else {
                // Not condition
            }
        }
    }

    // [4.36.5.3.1] 부족 1 ~ 4차
    void updateEventScrLevelWarnDisplayReq() {
        std::string prevEventID;
        std::string eventID;
        SFCSCREventScrLvlWarnLinkedSoundType linkedSoundType = SFCSCREventScrLvlWarnLinkedSoundType::REPEAT_COUNT;
        mEventGroup = EventGroup::NONE;

        if (mIgnStat) {
            if (mConfigArea == ConfigArea::USA || mConfigArea == ConfigArea::CAN) {
                switch (mScrLevelWarnDisplayReqStatus) {
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL1:
                        eventID = "E22902";
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL2:
                        eventID = "E22904";
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_2;
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL3:
                        eventID = "E22906";
                        // for MAX n[s]
                        linkedSoundType = SFCSCREventScrLvlWarnLinkedSoundType::DURATION;
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_3;
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL4:
                        eventID = "E22908";
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_4;
                        break;
                    default:
                        break;
                }
            } else {
                switch (mScrLevelWarnDisplayReqStatus) {
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL1:
                        eventID = "E22901";
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL2:
                        eventID = "E22903";
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_2;
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL3:
                        eventID = "E22905";
                        // for MAX n[s]
                        linkedSoundType = SFCSCREventScrLvlWarnLinkedSoundType::DURATION;
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_3;
                        break;
                    case ScrLevelWarnDisplayReqStatus::WARN_LEVEL4:
                        eventID = "E22907";
                        mEventGroup = EventGroup::SCR_LEVEL_WARN_4;
                        break;
                    default:
                        break;
                }
            }
        }

        GETCACHEDVALUE(SFC.SCR.Event.ScrLvlWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSCREventScrLvlWarnStat(SFCSCREventScrLvlWarnStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSCREventScrLvlWarnID(eventID);
            setSFCSCREventScrLvlWarnStat(SFCSCREventScrLvlWarnStat::ON);
            setSFCSCREventScrLvlWarnLinkedSoundType(linkedSoundType);
        }

        updateConstantScrSpdValue();
    }

    // [4.36.5.3.2 ~ 4] 비정상 1 ~ 3차, 시스템 점검 1 ~ 3차, 시스템 중지 Input_ScrIncorrectUreaWarnStatus1 신호 처리
    void updateEventScrIncorrectUreaWarn1() {
        std::string prevEventID;
        std::string eventID;
        mEventGroup = EventGroup::NONE;

        if (mIgnStat) {
            if (mConfigArea == ConfigArea::USA || mConfigArea == ConfigArea::CAN) {
                switch (mScrIncorrectUreaWarnStatus1) {
                    case ScrIncorrectUreaWarnStatus1::INCORRECT_UREA:
                        eventID = "E22910";
                        mEventGroup = EventGroup::SCR_INCORRECT_1;
                        break;
                    case ScrIncorrectUreaWarnStatus1::CHECK_UREA:
                        eventID = "E22914";
                        mEventGroup = EventGroup::SCR_CHECK_1;
                        break;
                    case ScrIncorrectUreaWarnStatus1::DEACTIVATED:
                        eventID = "E22918";
                        break;
                    default:
                        break;
                }
            } else {
                switch (mScrIncorrectUreaWarnStatus1) {
                    case ScrIncorrectUreaWarnStatus1::INCORRECT_UREA:
                        eventID = "E22909";
                        mEventGroup = EventGroup::SCR_INCORRECT_1;
                        break;
                    case ScrIncorrectUreaWarnStatus1::CHECK_UREA:
                        eventID = "E22913";
                        mEventGroup = EventGroup::SCR_CHECK_1;
                        break;
                    case ScrIncorrectUreaWarnStatus1::DEACTIVATED:
                        eventID = "E22917";
                        break;
                    default:
                        break;
                }
            }
        }

        GETCACHEDVALUE(SFC.SCR.Event.IncorrectUreaWarn1.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSCREventIncorrectUreaWarn1Stat(SFCSCREventIncorrectUreaWarn1Stat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSCREventIncorrectUreaWarn1ID(eventID);
            setSFCSCREventIncorrectUreaWarn1Stat(SFCSCREventIncorrectUreaWarn1Stat::ON);
        }

        updateConstantScrSpdValue();
    }

    // [4.36.5.3.2 ~ 3] 비정상 1 ~ 3차, 시스템 점검 1 ~ 3차 Input_ScrIncorrectUreaWarnStatus2 신호 처리
    void updateEventScrIncorrectUreaWarn2() {
        std::string prevEventID;
        std::string eventID;
        // for max n[s]
        SFCSCREventIncorrectUreaWarn2LinkedSoundType linkedSoundType = SFCSCREventIncorrectUreaWarn2LinkedSoundType::DURATION;
        mEventGroup = EventGroup::NONE;

        if (mIgnStat) {
            if (mConfigArea == ConfigArea::USA || mConfigArea == ConfigArea::CAN) {
                switch (mScrIncorrectUreaWarnStatus2) {
                    case ScrIncorrectUreaWarnStatus2::INCORRECT_UREA:
                        eventID = "E22912";
                        mEventGroup = EventGroup::SCR_INCORRECT_2;
                        // for up to n[s]
                        linkedSoundType = SFCSCREventIncorrectUreaWarn2LinkedSoundType::DURATION_MINMAX;
                        break;
                    case ScrIncorrectUreaWarnStatus2::CHECK_SYS:
                        eventID = "E22916";
                        mEventGroup = EventGroup::SCR_CHECK_2;
                        break;
                    default:
                        break;
                }
            } else {
                switch (mScrIncorrectUreaWarnStatus2) {
                    case ScrIncorrectUreaWarnStatus2::INCORRECT_UREA:
                        eventID = "E22911";
                        mEventGroup = EventGroup::SCR_INCORRECT_2;
                        // for up to n[s]
                        linkedSoundType = SFCSCREventIncorrectUreaWarn2LinkedSoundType::DURATION_MINMAX;
                        break;
                    case ScrIncorrectUreaWarnStatus2::CHECK_SYS:
                        eventID = "E22915";
                        mEventGroup = EventGroup::SCR_CHECK_2;
                        break;
                    default:
                        break;
                }
            }
        } else {
            // Do Nothing
        }

        GETCACHEDVALUE(SFC.SCR.Event.IncorrectUreaWarn2.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSCREventIncorrectUreaWarn2Stat(SFCSCREventIncorrectUreaWarn2Stat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSCREventIncorrectUreaWarn2ID(eventID);
            setSFCSCREventIncorrectUreaWarn2Stat(SFCSCREventIncorrectUreaWarn2Stat::ON);
            setSFCSCREventIncorrectUreaWarn2LinkedSoundType(linkedSoundType);
        }

        updateConstantScrSpdValue();
    }

    void setAllSignalOff() {
        std::string prevEventID;
        setSFCSCRTelltaleSCRStat(SFCSCRTelltaleSCRStat::OFF);
        setSFCSCRConstantScrRemainDistanceValueStat(SFCSCRConstantScrRemainDistanceValueStat::DISPLAY_OFF);

        GETCACHEDVALUE(SFC.SCR.Event.ScrLvlWarn.ID, prevEventID);
        if (!prevEventID.empty()) {
            setSFCSCREventScrLvlWarnStat(SFCSCREventScrLvlWarnStat::OFF);
        }

        GETCACHEDVALUE(SFC.SCR.Event.IncorrectUreaWarn1.ID, prevEventID);
        if (!prevEventID.empty()) {
            setSFCSCREventIncorrectUreaWarn1Stat(SFCSCREventIncorrectUreaWarn1Stat::OFF);
        }

        GETCACHEDVALUE(SFC.SCR.Event.IncorrectUreaWarn2.ID, prevEventID);
        if (!prevEventID.empty()) {
            setSFCSCREventIncorrectUreaWarn2Stat(SFCSCREventIncorrectUreaWarn2Stat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    enum class EventGroup : HUInt64 {
        NONE,
        SCR_LEVEL_WARN_2,  // 부족 2차
        SCR_LEVEL_WARN_3,  // 부족 3차
        SCR_LEVEL_WARN_4,  // 부족 4차
        SCR_INCORRECT_1,   // 비정상 1차
        SCR_INCORRECT_2,   // 비정상 2차
        SCR_CHECK_1,       // 시스템 점검 1차
        SCR_CHECK_2,       // 시스템 점검 2차
        MAX
    };

    using ConfigSCR = ArgumentsConfigSCRChanged::Inter_ConfigSCR;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ScrLevelWarnIndiReqStatus = ArgumentsScrLevelWarnIndiReqChanged::Input_ScrLevelWarnIndiReqStatus;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using ScrLevelWarnDisplayReqStatus = ArgumentsScrLevelWarnDisplyReqStatChanged::Input_ScrLevelWarnDisplayReqStatus;
    using ScrIncorrectUreaWarnStatus1 = ArgumentsScrIncorrectUreaWarnStat1Changed::Input_ScrIncorrectUreaWarnStatus1;
    using ScrIncorrectUreaWarnStatus2 = ArgumentsScrIncorrectUreaWarnStat2Changed::Input_ScrIncorrectUreaWarnStatus2;

    static constexpr HUInt64 kLinkedSoundDuration10000ms = 10000;
    static constexpr HUInt64 kLinkedSoundMinimumDuration0ms = 0;
    static constexpr HUInt64 kLinkedSoundMaximumDuration10000ms = 10000;
    static constexpr HUInt64 kLinkedSoundRepeatCount1 = 1;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigSCR mConfigSCR = ConfigSCR::OFF;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ScrLevelWarnIndiReqStatus mScrLevelWarnIndiReqStatus = ScrLevelWarnIndiReqStatus::OFF;
    HUInt64 mScrUreaLevelValue = 0;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::NONE;
    HUInt64 mScrRemainDistanceValue = 0;
    ScrLevelWarnDisplayReqStatus mScrLevelWarnDisplayReqStatus = ScrLevelWarnDisplayReqStatus::OFF;
    ScrIncorrectUreaWarnStatus1 mScrIncorrectUreaWarnStatus1 = ScrIncorrectUreaWarnStatus1::OFF;
    ScrIncorrectUreaWarnStatus2 mScrIncorrectUreaWarnStatus2 = ScrIncorrectUreaWarnStatus2::OFF;

    HBool mIgnStat = false;
    EventGroup mEventGroup = EventGroup::NONE;
    std::map<EventGroup, HUInt64> mScrSpdValueForKMMap = {
        {EventGroup::SCR_LEVEL_WARN_2, 72}, {EventGroup::SCR_LEVEL_WARN_3, 8}, {EventGroup::SCR_LEVEL_WARN_4, 8},
        {EventGroup::SCR_INCORRECT_1, 72},  {EventGroup::SCR_INCORRECT_2, 8},  {EventGroup::SCR_CHECK_1, 72},
        {EventGroup::SCR_CHECK_2, 8},
    };
    std::map<EventGroup, HUInt64> mScrSpdValueForMILEMap = {
        {EventGroup::SCR_LEVEL_WARN_2, 45}, {EventGroup::SCR_LEVEL_WARN_3, 5}, {EventGroup::SCR_LEVEL_WARN_4, 5},
        {EventGroup::SCR_INCORRECT_1, 45},  {EventGroup::SCR_INCORRECT_2, 5},  {EventGroup::SCR_CHECK_1, 45},
        {EventGroup::SCR_CHECK_2, 5},
    };
};

}  // namespace ccos

#endif  // SFSS_SCR_H
