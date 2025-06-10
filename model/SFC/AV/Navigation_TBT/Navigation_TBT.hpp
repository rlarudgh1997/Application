/**
 * @file Navigation_TBT.hpp
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
#ifndef SFSS_Navigation_TBT_H
#define SFSS_Navigation_TBT_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "Navigation_TBTBase.hpp"

namespace ccos {

// SFC Version : 30.0.0
// Reference : [AV030] Navigation_TBT. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Navigation_TBT : public Navigation_TBTBase {
public:
    Navigation_TBT() = default;
    ~Navigation_TBT() override = default;
    Navigation_TBT(const Navigation_TBT& other) = delete;
    Navigation_TBT(Navigation_TBT&& other) noexcept = delete;
    Navigation_TBT& operator=(const Navigation_TBT& other) = delete;
    Navigation_TBT& operator=(Navigation_TBT&& other) noexcept = delete;
    void onInitialize() override {
        setSFCNavigation_TBTEventNaviPopupDistLinkedSoundID("SND_PopUpWarn1");
        setSFCNavigation_TBTEventNaviPopupDistLinkedSoundType(SFCNavigation_TBTEventNaviPopupDistLinkedSoundType::REPEAT_COUNT);
        setSFCNavigation_TBTEventNaviPopupDistLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        refreshAllSignal();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        refreshAllSignal();
    }

    void onConfigTBTChanged(const ArgumentsConfigTBTChanged& args) {
        mConfigTBT = args.mInter_ConfigTBT;
        if (mConfigTBT != ConfigTBT::ON) {
            setAllSignalOff();
        } else {
            refreshAllSignal();
        }
    }

    // EXNCP-20541 : IMG_NaviTbt1_stat, IMG_NaviTbt2_stat은 SFC 개발 범위가 아님.
    void onNaviPointFlagChanged(const ArgumentsNaviPointFlagChanged& args) {
        mNaviPointFlagTypeStatus = args.mInput_NaviPointFlagTypeStatus;
        mNaviDisCountry = args.mInter_ConfigNaviDisCountry;
        updateFlagStat();
    }

    void onNaviPointPinChanged(const ArgumentsNaviPointPinChanged& args) {
        mNaviPointPinTypeStatus = args.mInput_NaviPointPinTypeStatus;
        updatePinStat();
    }

    void onNaviTbtChanged(const ArgumentsNaviTbtChanged& args) {
        mHeadUnitAliveStatus = args.mInput_HeadUnitAliveStatus;
        mHeadUnitNaviStatus = args.mInput_HeadUnitNaviStatus;
        mTbtDisplayTypeStatus = args.mInput_TbtDisplayTypeStatus;
        updateNaviAliveStatus();
        updateNaviTBTStat();
    }

    void onTbtDirectionChanged(const ArgumentsTbtDirectionChanged& args) {
        mTbtDirectionValue = args.mInput_TbtDirectionValue;
        updateDirectionValue();
    }

    void onTbtSideStreetChanged(const ArgumentsTbtSideStreetChanged& args) {
        mTbtSideStreetStatus = args.mInput_TbtSideStreetStatus;
        updateSideStreetStat();
    }

    void onTbtCombinedSideStreetChanged(const ArgumentsTbtCombinedSideStreetChanged& args) {
        mTbtCombinedSideStreetStatus = args.mInput_TbtCombinedSideStreetStatus;
        updateCombinedSideStreetStat();
    }

    void onNaviDistToDestChanged(const ArgumentsNaviDistToDestChanged& args) {
        mNaviDistToDestUnit = args.mInput_NaviDistToDestUnit;
        if (ISTIMEOUT(args.mInput_NaviDistToDestValue) == true) {
            mNaviDistToDestValue = 0xFFFF;
        } else {
            mNaviDistToDestValue = args.mInput_NaviDistToDestValue;
        }
        if (ISTIMEOUT(args.mInput_NaviDistToDestUnderDecimalValue) == true) {
            mNaviDistToDestUnderDecimalValue = static_cast<HDouble>(0xF);
        } else {
            mNaviDistToDestUnderDecimalValue = static_cast<HDouble>(args.mInput_NaviDistToDestUnderDecimalValue);
        }
        updateNaviDistToDestValue();
        updateNaviDistUnitStat();
    }

    void onTbtDistToTurnChanged(const ArgumentsTbtDistToTurnChanged& args) {
        mTbtDistanceUnit = args.mInput_TbtDstanceUnit;
        if (ISTIMEOUT(args.mInput_TbtDistToTurnPointValue) == true) {
            mTbtDistToTurnPointValue = 0xFFFF;
        } else {
            mTbtDistToTurnPointValue = args.mInput_TbtDistToTurnPointValue;
        }
        if (ISTIMEOUT(args.mInput_TbtDistToTurnPointUnderDecimalValue) == true) {
            mTbtDistToTurnPointUnderDecimalValue = static_cast<HDouble>(0xFF);
        } else {
            mTbtDistToTurnPointUnderDecimalValue = static_cast<HDouble>(args.mInput_TbtDistToTurnPointUnderDecimalValue);
        }
        updateTbtDistToTurnValue();
        updateTbtDistanceUnitStat();
    }

    void onTbtBarGraphChanged(const ArgumentsTbtBarGraphChanged& args) {
        if (ISTIMEOUT(args.mInput_TbtBarGraphLevelValue) == true) {
            mTbtBarGraphLevelValue = kTimeOut;
        } else {
            mTbtBarGraphLevelValue = args.mInput_TbtBarGraphLevelValue;
        }
        updateBarGraphLevelStat();
    }

    void onNaviDistToTurnFirstChanged(const ArgumentsNaviDistToTurnFirstChanged& args) {
        mNaviDistToTurnFirstUnitStatus = args.mInput_NaviDistToTurnFirstUnitStatus;
        if (ISTIMEOUT(args.mInput_NaviDistToTurnFirstValue) == true) {
            mNaviDistToTurnFirstValue = 0xFFFF;
        } else {
            mNaviDistToTurnFirstValue = args.mInput_NaviDistToTurnFirstValue;
        }
        if (ISTIMEOUT(args.mInput_NaviDistToTurnFirstUnderDecimalValue) == true) {
            mNaviDistToTurnFirstUnderDecimalValue = static_cast<HDouble>(0xFF);
        } else {
            mNaviDistToTurnFirstUnderDecimalValue = static_cast<HDouble>(args.mInput_NaviDistToTurnFirstUnderDecimalValue);
        }
        updateNaviDistToTurn1Value();
        updateNaviDistToTurn1Stat();
    }

    void onNaviDistToTurnSecondChanged(const ArgumentsNaviDistToTurnSecondChanged& args) {
        mNaviDistToTurnSecondUnitStatus = args.mInput_NaviDistToTurnSecondUnitStatus;
        if (ISTIMEOUT(args.mInput_NaviDistToTurnSecondValue) == true) {
            mNaviDistToTurnSecondValue = 0xFFFF;
        } else {
            mNaviDistToTurnSecondValue = args.mInput_NaviDistToTurnSecondValue;
        }
        if (ISTIMEOUT(args.mInput_NaviDistToTurnSecondUnderDecimalValue) == true) {
            mNaviDistToTurnSecondUnderDecimalValue = static_cast<HDouble>(0xFF);
        } else {
            mNaviDistToTurnSecondUnderDecimalValue = static_cast<HDouble>(args.mInput_NaviDistToTurnSecondUnderDecimalValue);
        }
        updateNaviDistToTurn2Value();
        updateNaviDistToTurn2Stat();
    }

    void onNaviLaneRemainDistChanged(const ArgumentsNaviLaneRemainDistChanged& args) {
        mNaviLaneRemainDistUnitStatus = args.mInput_NaviLaneRemainDistUnitStatus;
        if (ISTIMEOUT(args.mInput_NaviLaneRemainDistValue) == true) {
            mNaviLaneRemainDistValue = 0xFFFF;
        } else {
            mNaviLaneRemainDistValue = args.mInput_NaviLaneRemainDistValue;
        }
        if (ISTIMEOUT(args.mInput_NaviLaneRemainDistUnderDecimalValue) == true) {
            mNaviLaneRemainDistUnderDecimalValue = static_cast<HDouble>(0xFF);
        } else {
            mNaviLaneRemainDistUnderDecimalValue = static_cast<HDouble>(args.mInput_NaviLaneRemainDistUnderDecimalValue);
        }
        updateNaviLaneRemainDistValue();
        updateNaviLaneRemainDistStat();
    }

    void onNaviPopupChanged(const ArgumentsNaviPopupChanged& args) {
        mNaviPopupReqStatus = args.mInput_NaviPopupReqStatus;
        updateEvent();
    }

    void onNaviPopupDistChanged(const ArgumentsNaviPopupDistChanged& args) {
        mNaviPopupDistUnitStatus = args.mInput_NaviPopupDistUnitStatus;
        if (ISTIMEOUT(args.mInput_NaviPopupDistOverDecimalValue) == true) {
            mNaviPopupDistOverDecimalValue = 0xFFFF;
        } else {
            mNaviPopupDistOverDecimalValue = args.mInput_NaviPopupDistOverDecimalValue;
        }
        if (ISTIMEOUT(args.mInput_NaviPopupDistUnderDecimalValue) == true) {
            mNaviPopupDistUnderDecimalValue = static_cast<HDouble>(0xFFFF);
        } else {
            mNaviPopupDistUnderDecimalValue = static_cast<HDouble>(args.mInput_NaviPopupDistUnderDecimalValue);
        }
        updateNaviPopupDistValue();
        updateNaviPopupDistStat();
        updateNaviPopupDistBarStat();
    }

    void onGuideColorStatusChanged(const ArgumentsGuideColorStatusChanged& args) {
        mTbtGuideColorStatus = args.mInput_TBTGuideColorStatus;
        updateInterNaviTbtColor();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateNaviAliveStatus() {
        if (isAvailableTBT() == true) {
            if (mHeadUnitAliveStatus == HeadUnitAliveStatus::NOT_READY ||
                mHeadUnitAliveStatus == HeadUnitAliveStatus::UPDATE_MODE ||
                mHeadUnitAliveStatus == HeadUnitAliveStatus::INVALID) {
                mNaviAliveStatus = NaviAliveStatus::LOADING;
            } else if (mHeadUnitAliveStatus == HeadUnitAliveStatus::ON_MODE) {
                if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NORMAL) {
                    mNaviAliveStatus = NaviAliveStatus::NORMAL;
                } else if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NO_SDCARD) {
                    mNaviAliveStatus = NaviAliveStatus::NOSDCARD;
                } else if (mHeadUnitNaviStatus == HeadUnitNaviStatus::BOOTING ||
                           mHeadUnitNaviStatus == HeadUnitNaviStatus::INVALID) {
                    mNaviAliveStatus = NaviAliveStatus::LOADING;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
        setSFCNavigation_TBTInter_NaviAliveStatus(mNaviAliveStatus);
    }

    void updateInterNaviTbtColor() {
        NaviTbtColor naviColor = NaviTbtColor::BRAND;
        if (isAvailableTBT() == true) {
            const auto& it = mNaviTbtColorMap.find(mTbtGuideColorStatus);
            if (it != mNaviTbtColorMap.end()) {
                naviColor = it->second;
            }
        }
        setSFCNavigation_TBTInter_NaviTbtColor(naviColor);
        setSFCNavigation_TBTConstantTbtBarGraphLevelColor(static_cast<SFCNavigation_TBTConstantTbtBarGraphLevelColor>(naviColor));
    }

    void updateNaviTBTStat() {
        SFCNavigation_TBTConstantNaviTbtStat tbtStat = SFCNavigation_TBTConstantNaviTbtStat::OFF;
        if (isAvailableTBT() == true) {
            // 우선순위 중요.
            if (mNaviAliveStatus == NaviAliveStatus::NOSDCARD) {
                tbtStat = SFCNavigation_TBTConstantNaviTbtStat::NOSDCARD;
            } else if (mNaviAliveStatus == NaviAliveStatus::LOADING) {
                tbtStat = SFCNavigation_TBTConstantNaviTbtStat::LOADING;
            } else if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                const auto& it = mInputOutputMap.find(mTbtDisplayTypeStatus);
                if (it != mInputOutputMap.end()) {
                    tbtStat = it->second;
                }
            } else {
                // no operation
            }
        }
        setSFCNavigation_TBTConstantNaviTbtStat(tbtStat);
    }

    void updateDirectionValue() {
        SFCNavigation_TBTConstantDirectionType1Stat dirType1Stat = SFCNavigation_TBTConstantDirectionType1Stat::OFF;
        HUInt64 dirType1Value = 0;
        SFCNavigation_TBTConstantDirectionType2Stat dirType2Stat = SFCNavigation_TBTConstantDirectionType2Stat::OFF;
        HUInt64 dirType2Value = 0;
        if (isAvailableTBT() == true) {
            if (mTbtDirectionValue % 3 == 0 && mTbtDirectionValue <= 0x2D) {
                dirType1Stat = SFCNavigation_TBTConstantDirectionType1Stat::VALUE;
                dirType1Value = mTbtDirectionValue;
                if (mTbtDirectionValue == 0x0 || mTbtDirectionValue == 0xC || mTbtDirectionValue == 0x24) {
                    dirType2Stat = SFCNavigation_TBTConstantDirectionType2Stat::VALUE;
                    dirType2Value = mTbtDirectionValue;
                }
            }
        }
        setSFCNavigation_TBTConstantDirectionType1Stat(dirType1Stat);
        setSFCNavigation_TBTConstantDirectionType1Value(dirType1Value);
        setSFCNavigation_TBTConstantDirectionType2Stat(dirType2Stat);
        setSFCNavigation_TBTConstantDirectionType2Value(dirType2Value);
    }

    void updateSideStreetStat() {
        HUInt64 maskVal = 0;
        if (mTbtSideStreetStatus >= kMinimumSideStreetValue && mTbtSideStreetStatus <= kMaximumSideStreetValue &&
            isAvailableTBT() == true) {
            maskVal = mTbtSideStreetStatus;
        } else {
            maskVal = 0;
        }
        setSFCNavigation_TBTConstantSideStreetBitmask(maskVal);
    }

    void updateCombinedSideStreetStat() {
        SFCNavigation_TBTConstantCombinedSideStreetStat strStat = SFCNavigation_TBTConstantCombinedSideStreetStat::OFF;
        if (isAvailableTBT() == true) {
            const auto& it = mTbtCombinedSideStreetMap.find(mTbtCombinedSideStreetStatus);
            if (it != mTbtCombinedSideStreetMap.end()) {
                strStat = it->second;
            }
        }
        setSFCNavigation_TBTConstantCombinedSideStreetStat(strStat);
    }

    void updateNaviDistToDestValue() {
        HDouble destVal = 0.0;
        SFCNavigation_TBTConstantNaviDistToDestStat destStat = SFCNavigation_TBTConstantNaviDistToDestStat::OFF;
        calTBTDistanceValue(mNaviDistToDestValue, mNaviDistToDestUnderDecimalValue, mNaviDistToDestUnit, destVal, destStat);
        setSFCNavigation_TBTConstantNaviDistToDestValue(destVal);
        setSFCNavigation_TBTConstantNaviDistToDestStat(destStat);
    }

    void updateNaviDistUnitStat() {
        SFCNavigation_TBTConstantNaviDistUnitStat unitStat = SFCNavigation_TBTConstantNaviDistUnitStat::OFF;
        calUnitStat(mNaviDistToDestValue, mNaviDistToDestUnit, mNaviDistToDestUnitMap, unitStat);
        setSFCNavigation_TBTConstantNaviDistUnitStat(unitStat);
    }

    void updateTbtDistToTurnValue() {
        HDouble turnVal = 0.0;
        SFCNavigation_TBTConstantTbtDistToTurnPointStat turnStat = SFCNavigation_TBTConstantTbtDistToTurnPointStat::OFF;
        calTBTDistanceValue(mTbtDistToTurnPointValue, mTbtDistToTurnPointUnderDecimalValue, mTbtDistanceUnit, turnVal, turnStat);
        setSFCNavigation_TBTConstantTbtDistToTurnPointValue(turnVal);
        setSFCNavigation_TBTConstantTbtDistToTurnPointStat(turnStat);
    }

    void updateTbtDistanceUnitStat() {
        SFCNavigation_TBTConstantTbtDistanceUnitStat unitStat = SFCNavigation_TBTConstantTbtDistanceUnitStat::OFF;
        calUnitStat(mTbtDistToTurnPointValue, mTbtDistanceUnit, mTbtDistanceUnitMap, unitStat);
        setSFCNavigation_TBTConstantTbtDistanceUnitStat(unitStat);
    }

    void updateBarGraphLevelStat() {
        HUInt64 barValue = 0;
        SFCNavigation_TBTConstantTbtBarGraphLevelStat barStat = SFCNavigation_TBTConstantTbtBarGraphLevelStat::OFF;
        if (isAvailableTBT() == true && mTbtBarGraphLevelValue <= kLimitTbtBarGraphLevelValue) {
            barValue = mTbtBarGraphLevelValue;
            barStat = SFCNavigation_TBTConstantTbtBarGraphLevelStat::VALUE;
        }
        setSFCNavigation_TBTConstantTbtBarGraphLevelValue(barValue);
        setSFCNavigation_TBTConstantTbtBarGraphLevelStat(barStat);
    }

    void updateFlagStat() {
        SFCNavigation_TBTConstantNaviPointFlagTypeStat flagStat = SFCNavigation_TBTConstantNaviPointFlagTypeStat::OFF;
        if (isAvailableTBT() == true) {
            const auto flagMap = (mNaviDisCountry == NaviDisCountry::CHINA ? mNaviChinaNaviFlagMap : mNaviGeneralNaviFlagMap);
            const auto& it = flagMap.find(mNaviPointFlagTypeStatus);
            if (it != flagMap.end()) {
                flagStat = it->second;
            }
        }
        setSFCNavigation_TBTConstantNaviPointFlagTypeStat(flagStat);
    }

    void updatePinStat() {
        SFCNavigation_TBTConstantNaviPointPinTypeStat flagStat = SFCNavigation_TBTConstantNaviPointPinTypeStat::DEFAULT;
        if (isAvailableTBT() == true) {
            const auto& it = mNaviPointPinMap.find(mNaviPointPinTypeStatus);
            if (it != mNaviPointPinMap.end()) {
                flagStat = it->second;
            }
        }
        setSFCNavigation_TBTConstantNaviPointPinTypeStat(flagStat);
    }

    void updateNaviDistToTurn1Value() {
        HDouble turnFirstVal = 0.0;
        SFCNavigation_TBTConstantNaviDistToTurn1Stat turnFirstStat = SFCNavigation_TBTConstantNaviDistToTurn1Stat::OFF;
        calTBTDistanceValue(mNaviDistToTurnFirstValue, mNaviDistToTurnFirstUnderDecimalValue, mNaviDistToTurnFirstUnitStatus,
                            turnFirstVal, turnFirstStat);
        setSFCNavigation_TBTConstantNaviDistToTurn1Value(turnFirstVal);
        setSFCNavigation_TBTConstantNaviDistToTurn1Stat(turnFirstStat);
    }

    void updateNaviDistToTurn1Stat() {
        SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat unitStat = SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::OFF;
        calUnitStat(mNaviDistToTurnFirstValue, mNaviDistToTurnFirstUnitStatus, mNaviDistToTurnFirstUnitStatusMap, unitStat);
        setSFCNavigation_TBTConstantNaviDistToTurnUnit1Stat(unitStat);
    }

    void updateNaviDistToTurn2Value() {
        HDouble turnSecondVal = 0.0;
        SFCNavigation_TBTConstantNaviDistToTurn2Stat turnSecondStat = SFCNavigation_TBTConstantNaviDistToTurn2Stat::OFF;
        calTBTDistanceValue(mNaviDistToTurnSecondValue, mNaviDistToTurnSecondUnderDecimalValue, mNaviDistToTurnSecondUnitStatus,
                            turnSecondVal, turnSecondStat);
        setSFCNavigation_TBTConstantNaviDistToTurn2Value(turnSecondVal);
        setSFCNavigation_TBTConstantNaviDistToTurn2Stat(turnSecondStat);
    }

    void updateNaviDistToTurn2Stat() {
        SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat unitStat = SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::OFF;
        calUnitStat(mNaviDistToTurnSecondValue, mNaviDistToTurnSecondUnitStatus, mNaviDistToTurnSecondUnitStatusMap, unitStat);
        setSFCNavigation_TBTConstantNaviDistToTurnUnit2Stat(unitStat);
    }

    void updateNaviLaneRemainDistValue() {
        HDouble distVal = 0.0;
        SFCNavigation_TBTConstantNaviLaneRemainDistStat valStat = SFCNavigation_TBTConstantNaviLaneRemainDistStat::OFF;
        calTBTDistanceValue(mNaviLaneRemainDistValue, mNaviLaneRemainDistUnderDecimalValue, mNaviLaneRemainDistUnitStatus,
                            distVal, valStat);
        setSFCNavigation_TBTConstantNaviLaneRemainDistValue(distVal);
        setSFCNavigation_TBTConstantNaviLaneRemainDistStat(valStat);
    }

    void updateNaviLaneRemainDistStat() {
        SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat unitStat = SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::OFF;
        calUnitStat(mNaviLaneRemainDistValue, mNaviLaneRemainDistUnitStatus, mNaviLaneRemainDistUnitStatusMap, unitStat);
        setSFCNavigation_TBTConstantNaviLaneRemainDistUnitStat(unitStat);
    }

    void updateEvent() {
        std::string prevEventID;
        std::string eventID;
        if (isAvailableTBT() == true) {
            const auto& it = mEventIdMap.find(mNaviPopupReqStatus);
            if (it != mEventIdMap.end()) {
                eventID = it->second;
            }
        }

        GETCACHEDVALUE(SFC.Navigation_TBT.Event.NaviPopupDist.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCNavigation_TBTEventNaviPopupDistStat(SFCNavigation_TBTEventNaviPopupDistStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCNavigation_TBTEventNaviPopupDistID(eventID);
            setSFCNavigation_TBTEventNaviPopupDistStat(SFCNavigation_TBTEventNaviPopupDistStat::ON);
        }
    }

    void updateNaviPopupDistValue() {
        HDouble destVal = 0.0;
        SFCNavigation_TBTConstantNaviPopupDistToDestStat destStat = SFCNavigation_TBTConstantNaviPopupDistToDestStat::OFF;
        calTBTDistanceValue(mNaviPopupDistOverDecimalValue, mNaviPopupDistUnderDecimalValue, mNaviPopupDistUnitStatus, destVal,
                            destStat);
        setSFCNavigation_TBTConstantNaviPopupDistToDestValue(destVal);
        setSFCNavigation_TBTConstantNaviPopupDistToDestStat(destStat);
    }

    void updateNaviPopupDistStat() {
        SFCNavigation_TBTConstantNaviPopupDistUnitStat unitStat = SFCNavigation_TBTConstantNaviPopupDistUnitStat::OFF;
        calUnitStat(mNaviPopupDistOverDecimalValue, mNaviPopupDistUnitStatus, mNaviPopupDistUnitStatusMap, unitStat);
        setSFCNavigation_TBTConstantNaviPopupDistUnitStat(unitStat);
    }

    void updateNaviPopupDistBarStat() {
        SFCNavigation_TBTConstantNaviPopupDistBarStat barStat = SFCNavigation_TBTConstantNaviPopupDistBarStat::OFF;
        HDouble barValue = 0.0;

        if (isAvailableTBT() == true && mNaviPopupDistOverDecimalValue != 0xFFFE && mNaviPopupDistOverDecimalValue != 0xFFFF &&
            mNaviPopupDistUnderDecimalValue < 10.0) {
            mNaviPopupDistDecimalValue =
                static_cast<HDouble>(mNaviPopupDistOverDecimalValue) + (mNaviPopupDistUnderDecimalValue * 0.1);
            HDouble divisor = 0.0;

            const auto& it = mPopupAreaMap.find(mNaviPopupDistUnitStatus);
            if (it != mPopupAreaMap.end()) {
                divisor = it->second;
                // epsilon 비교 먼저 하면 0.0~divisor 사이 값 비교할때 epsilon비교 또 하지 않아도 됨.
                if (almostEqual(mNaviPopupDistDecimalValue, 0.0) == true) {
                    barStat = SFCNavigation_TBTConstantNaviPopupDistBarStat::EMPTY;
                } else if (almostEqual(mNaviPopupDistDecimalValue, divisor) == true) {
                    barStat = SFCNavigation_TBTConstantNaviPopupDistBarStat::FULL;
                } else if (0.0 < mNaviPopupDistDecimalValue && mNaviPopupDistDecimalValue < divisor) {
                    barStat = SFCNavigation_TBTConstantNaviPopupDistBarStat::VALUE;
                    barValue = mNaviPopupDistDecimalValue / divisor;
                } else {
                    // no operation
                }
            }
        }
        setSFCNavigation_TBTConstantNaviPopupDistBarStat(barStat);
        setSFCNavigation_TBTConstantNaviPopupDistBarValue(barValue);
    }

    void setAllSignalOff() {
        setSFCNavigation_TBTInter_NaviAliveStatus(SFCNavigation_TBTInter_NaviAliveStatus::LOADING);
        setSFCNavigation_TBTInter_NaviTbtColor(SFCNavigation_TBTInter_NaviTbtColor::BRAND);
        setSFCNavigation_TBTConstantTbtBarGraphLevelColor(SFCNavigation_TBTConstantTbtBarGraphLevelColor::BRAND);
        setSFCNavigation_TBTConstantNaviTbtStat(SFCNavigation_TBTConstantNaviTbtStat::OFF);
        setSFCNavigation_TBTConstantSideStreetBitmask(0);
        setSFCNavigation_TBTConstantCombinedSideStreetStat(SFCNavigation_TBTConstantCombinedSideStreetStat::OFF);
        setSFCNavigation_TBTConstantNaviDistToDestValue(0.0);
        setSFCNavigation_TBTConstantNaviDistToDestStat(SFCNavigation_TBTConstantNaviDistToDestStat::OFF);
        setSFCNavigation_TBTConstantNaviDistUnitStat(SFCNavigation_TBTConstantNaviDistUnitStat::OFF);
        setSFCNavigation_TBTConstantTbtDistToTurnPointValue(0.0);
        setSFCNavigation_TBTConstantTbtDistToTurnPointStat(SFCNavigation_TBTConstantTbtDistToTurnPointStat::OFF);
        setSFCNavigation_TBTConstantTbtDistanceUnitStat(SFCNavigation_TBTConstantTbtDistanceUnitStat::OFF);
        setSFCNavigation_TBTConstantTbtBarGraphLevelValue(0);
        setSFCNavigation_TBTConstantTbtBarGraphLevelStat(SFCNavigation_TBTConstantTbtBarGraphLevelStat::OFF);
        setSFCNavigation_TBTConstantNaviPointFlagTypeStat(SFCNavigation_TBTConstantNaviPointFlagTypeStat::OFF);
        setSFCNavigation_TBTConstantNaviPointPinTypeStat(SFCNavigation_TBTConstantNaviPointPinTypeStat::DEFAULT);
        setSFCNavigation_TBTConstantNaviDistToTurn1Value(0.0);
        setSFCNavigation_TBTConstantNaviDistToTurn1Stat(SFCNavigation_TBTConstantNaviDistToTurn1Stat::OFF);
        setSFCNavigation_TBTConstantNaviDistToTurnUnit1Stat(SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::OFF);
        setSFCNavigation_TBTConstantNaviDistToTurn2Value(0.0);
        setSFCNavigation_TBTConstantNaviDistToTurn2Stat(SFCNavigation_TBTConstantNaviDistToTurn2Stat::OFF);
        setSFCNavigation_TBTConstantNaviDistToTurnUnit2Stat(SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::OFF);
        setSFCNavigation_TBTConstantNaviLaneRemainDistValue(0.0);
        setSFCNavigation_TBTConstantNaviLaneRemainDistStat(SFCNavigation_TBTConstantNaviLaneRemainDistStat::OFF);
        setSFCNavigation_TBTConstantNaviLaneRemainDistUnitStat(SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::OFF);
        setSFCNavigation_TBTConstantNaviPopupDistToDestValue(0.0);
        setSFCNavigation_TBTConstantNaviPopupDistToDestStat(SFCNavigation_TBTConstantNaviPopupDistToDestStat::OFF);
        setSFCNavigation_TBTConstantNaviPopupDistUnitStat(SFCNavigation_TBTConstantNaviPopupDistUnitStat::OFF);
        setSFCNavigation_TBTConstantNaviPopupDistBarValue(0.0);
        setSFCNavigation_TBTConstantNaviPopupDistBarStat(SFCNavigation_TBTConstantNaviPopupDistBarStat::OFF);
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Navigation_TBT.Event.NaviPopupDist.ID, prevEventID);
        if (prevEventID.empty() == false) {
            setSFCNavigation_TBTEventNaviPopupDistStat(SFCNavigation_TBTEventNaviPopupDistStat::OFF);
        }
    }

    void refreshAllSignal() {
        updateNaviAliveStatus();
        updateInterNaviTbtColor();
        updateNaviTBTStat();
        updateDirectionValue();
        updateSideStreetStat();
        updateCombinedSideStreetStat();
        updateNaviDistToDestValue();
        updateNaviDistUnitStat();
        updateTbtDistToTurnValue();
        updateTbtDistanceUnitStat();
        updateBarGraphLevelStat();
        updateFlagStat();
        updatePinStat();
        updateNaviDistToTurn1Value();
        updateNaviDistToTurn1Stat();
        updateNaviDistToTurn2Value();
        updateNaviDistToTurn2Stat();
        updateNaviLaneRemainDistValue();
        updateNaviLaneRemainDistStat();
        updateNaviPopupDistValue();
        updateNaviPopupDistStat();
        updateNaviPopupDistBarStat();
        updateEvent();
    }

    inline bool isAvailableTBT() {
        return mIgnElapsed == IgnElapsed::ON_0ms && mConfigTBT == ConfigTBT::ON;
    }

    template <typename T1, typename T2>
    inline void calTBTDistanceValue(const HUInt64& ovrDecVal, const HDouble& undDecVal, const T1& unitStat, HDouble& resVal,
                                    T2& nStat) {
        if (isAvailableTBT() == true && ovrDecVal != 0xFFFE && ovrDecVal != 0xFFFF) {
            if (unitStat == T1::METER || unitStat == T1::FEET || unitStat == T1::YARD) {
                resVal = static_cast<HDouble>(ovrDecVal);
                nStat = T2::VALUE;
            } else if (unitStat == T1::KILOMETER || unitStat == T1::MILE) {
                if (ovrDecVal < kDisplayDecimalPoint) {
                    HDouble calculatedValue = static_cast<HDouble>(ovrDecVal);
                    if (undDecVal < 10.0) {
                        calculatedValue += undDecVal * 0.1;
                    }
                    resVal = calculatedValue;
                    nStat = T2::VALUE;
                } else if (ovrDecVal >= kDisplayDecimalPoint && ovrDecVal <= kLimitNaviDistToDestValue) {
                    resVal = static_cast<HDouble>(ovrDecVal);
                    nStat = T2::VALUE;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
    }

    template <typename T1, typename T2>
    inline void calUnitStat(const HUInt64& overDecVal, const T1& unitStatus, const std::map<T1, T2>& statValueMap, T2& statVal) {
        if (isAvailableTBT() == true && overDecVal != 0xFFFE && overDecVal != 0xFFFF) {
            const auto& it = statValueMap.find(unitStatus);
            if (it != statValueMap.end()) {
                statVal = it->second;
            }
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using NaviAliveStatus = SFCNavigation_TBTInter_NaviAliveStatus;
    using NaviTbtColor = SFCNavigation_TBTInter_NaviTbtColor;
    using ConfigTBT = ArgumentsConfigTBTChanged::Inter_ConfigTBT;
    using HeadUnitNaviStatus = ArgumentsNaviTbtChanged::Input_HeadUnitNaviStatus;
    using HeadUnitAliveStatus = ArgumentsNaviTbtChanged::Input_HeadUnitAliveStatus;
    using TbtDisplayTypeStatus = ArgumentsNaviTbtChanged::Input_TbtDisplayTypeStatus;
    using TbtGuideColorStatus = ArgumentsGuideColorStatusChanged::Input_TBTGuideColorStatus;
    using NaviDisCountry = ArgumentsNaviPointFlagChanged::Inter_ConfigNaviDisCountry;
    using NaviPointFlagTypeStatus = ArgumentsNaviPointFlagChanged::Input_NaviPointFlagTypeStatus;
    using NaviPointPinTypeStatus = ArgumentsNaviPointPinChanged::Input_NaviPointPinTypeStatus;
    using TbtCombinedSideStreetStatus = ArgumentsTbtCombinedSideStreetChanged ::Input_TbtCombinedSideStreetStatus;
    using NaviDistToDestUnit = ArgumentsNaviDistToDestChanged::Input_NaviDistToDestUnit;
    using TbtDistanceUnit = ArgumentsTbtDistToTurnChanged::Input_TbtDstanceUnit;
    using NaviDistToTurnFirstUnitStatus = ArgumentsNaviDistToTurnFirstChanged::Input_NaviDistToTurnFirstUnitStatus;
    using NaviDistToTurnSecondUnitStatus = ArgumentsNaviDistToTurnSecondChanged::Input_NaviDistToTurnSecondUnitStatus;
    using NaviLaneRemainDistUnitStatus = ArgumentsNaviLaneRemainDistChanged::Input_NaviLaneRemainDistUnitStatus;
    using NaviPopupReqStatus = ArgumentsNaviPopupChanged::Input_NaviPopupReqStatus;
    using NaviPopupDistUnitStatus = ArgumentsNaviPopupDistChanged::Input_NaviPopupDistUnitStatus;

    static constexpr HUInt64 kMinimumSideStreetValue = 0x0000;
    static constexpr HUInt64 kMaximumSideStreetValue = 0x7FFF;
    static constexpr HUInt64 kDisplayDecimalPoint = 100;
    static constexpr HUInt64 kLimitNaviDistToDestValue = 65534;
    static constexpr HUInt64 kLimitTbtBarGraphLevelValue = 100;
    static constexpr size_t kULP = 4UL;
    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    NaviAliveStatus mNaviAliveStatus = NaviAliveStatus::LOADING;
    ConfigTBT mConfigTBT = ConfigTBT::OFF;
    HeadUnitNaviStatus mHeadUnitNaviStatus = HeadUnitNaviStatus::INVALID;
    HeadUnitAliveStatus mHeadUnitAliveStatus = HeadUnitAliveStatus::INVALID;
    TbtDisplayTypeStatus mTbtDisplayTypeStatus = TbtDisplayTypeStatus::OFF;
    TbtGuideColorStatus mTbtGuideColorStatus = TbtGuideColorStatus::NO_DATA;
    NaviDisCountry mNaviDisCountry = NaviDisCountry::INVALID;
    NaviPointFlagTypeStatus mNaviPointFlagTypeStatus = NaviPointFlagTypeStatus::INVALID;
    NaviPointPinTypeStatus mNaviPointPinTypeStatus = NaviPointPinTypeStatus::NONE;
    HUInt64 mTbtDirectionValue = 0x0;
    HUInt64 mTbtSideStreetStatus = kMinimumSideStreetValue;
    TbtCombinedSideStreetStatus mTbtCombinedSideStreetStatus = TbtCombinedSideStreetStatus::NO_COMBINED;
    NaviDistToDestUnit mNaviDistToDestUnit = NaviDistToDestUnit::INVALID;
    HUInt64 mNaviDistToDestValue = 0;
    HDouble mNaviDistToDestUnderDecimalValue = 0.0;
    TbtDistanceUnit mTbtDistanceUnit = TbtDistanceUnit::INVALID;
    HUInt64 mTbtDistToTurnPointValue = 0;
    HDouble mTbtDistToTurnPointUnderDecimalValue = 0.0;
    HUInt64 mTbtBarGraphLevelValue = 0;
    NaviDistToTurnFirstUnitStatus mNaviDistToTurnFirstUnitStatus = NaviDistToTurnFirstUnitStatus::INVALID;
    HUInt64 mNaviDistToTurnFirstValue = 0;
    HDouble mNaviDistToTurnFirstUnderDecimalValue = 0.0;
    NaviDistToTurnSecondUnitStatus mNaviDistToTurnSecondUnitStatus = NaviDistToTurnSecondUnitStatus::INVALID;
    HUInt64 mNaviDistToTurnSecondValue = 0;
    HDouble mNaviDistToTurnSecondUnderDecimalValue = 0.0;
    NaviLaneRemainDistUnitStatus mNaviLaneRemainDistUnitStatus = NaviLaneRemainDistUnitStatus::INVALID;
    HUInt64 mNaviLaneRemainDistValue = 0;
    HDouble mNaviLaneRemainDistUnderDecimalValue = 0.0;
    NaviPopupReqStatus mNaviPopupReqStatus = NaviPopupReqStatus::OFF;
    NaviPopupDistUnitStatus mNaviPopupDistUnitStatus = NaviPopupDistUnitStatus::INVALID;
    HUInt64 mNaviPopupDistOverDecimalValue = 0;
    HDouble mNaviPopupDistUnderDecimalValue = 0.0;
    HDouble mNaviPopupDistDecimalValue = 0.0;

    const std::map<TbtDisplayTypeStatus, SFCNavigation_TBTConstantNaviTbtStat> mInputOutputMap = {
        {TbtDisplayTypeStatus::DIRECTION_TO_DESTINATION, SFCNavigation_TBTConstantNaviTbtStat::DIRECTION_TO_DESTINATION},
        {TbtDisplayTypeStatus::ARRIVED, SFCNavigation_TBTConstantNaviTbtStat::ARRIVED},
        {TbtDisplayTypeStatus::NO_ROUTE_GUIDANCE, SFCNavigation_TBTConstantNaviTbtStat::NO_ROUTE_GUIDANCE},
        {TbtDisplayTypeStatus::CALC_ROUTE, SFCNavigation_TBTConstantNaviTbtStat::CALC_ROUTE},
        {TbtDisplayTypeStatus::RECALC_ROUTE, SFCNavigation_TBTConstantNaviTbtStat::RECALC_ROUTE},
        {TbtDisplayTypeStatus::TURN, SFCNavigation_TBTConstantNaviTbtStat::TURN},
        {TbtDisplayTypeStatus::EXIT_R, SFCNavigation_TBTConstantNaviTbtStat::EXIT_R},
        {TbtDisplayTypeStatus::EXIT_L, SFCNavigation_TBTConstantNaviTbtStat::EXIT_L},
        {TbtDisplayTypeStatus::EXIT_COMBINATION_R, SFCNavigation_TBTConstantNaviTbtStat::EXIT_COMBINATION_R},
        {TbtDisplayTypeStatus::EXIT_COMBINATION_L, SFCNavigation_TBTConstantNaviTbtStat::EXIT_COMBINATION_L},
        {TbtDisplayTypeStatus::SERVICE_ROAD_R, SFCNavigation_TBTConstantNaviTbtStat::SERVICE_ROAD_R},
        {TbtDisplayTypeStatus::SERVICE_ROAD_L, SFCNavigation_TBTConstantNaviTbtStat::SERVICE_ROAD_L},
        {TbtDisplayTypeStatus::FORK2, SFCNavigation_TBTConstantNaviTbtStat::FORK2},
        {TbtDisplayTypeStatus::U_TURN, SFCNavigation_TBTConstantNaviTbtStat::U_TURN},
        {TbtDisplayTypeStatus::EXIT_ROUNDABOUT_TRS_R, SFCNavigation_TBTConstantNaviTbtStat::EXIT_ROUNDABOUT_TRS_R},
        {TbtDisplayTypeStatus::EXIT_ROUNDABOUT_TRS_L, SFCNavigation_TBTConstantNaviTbtStat::EXIT_ROUNDABOUT_TRS_L},
        {TbtDisplayTypeStatus::OVERPASS_R, SFCNavigation_TBTConstantNaviTbtStat::OVERPASS_R},
        {TbtDisplayTypeStatus::OVERPASS_L, SFCNavigation_TBTConstantNaviTbtStat::OVERPASS_L},
        {TbtDisplayTypeStatus::UNDERPASS_R, SFCNavigation_TBTConstantNaviTbtStat::UNDERPASS_R},
        {TbtDisplayTypeStatus::UNDERPASS_L, SFCNavigation_TBTConstantNaviTbtStat::UNDERPASS_L},
        {TbtDisplayTypeStatus::TOLL, SFCNavigation_TBTConstantNaviTbtStat::TOLL}};

    const std::map<TbtCombinedSideStreetStatus, SFCNavigation_TBTConstantCombinedSideStreetStat> mTbtCombinedSideStreetMap = {
        {TbtCombinedSideStreetStatus::RIGHT_COMBINED, SFCNavigation_TBTConstantCombinedSideStreetStat::RIGHT_COMBINED},
        {TbtCombinedSideStreetStatus::LEFT_COMBINED, SFCNavigation_TBTConstantCombinedSideStreetStat::LEFT_COMBINED},
        {TbtCombinedSideStreetStatus::BOTH_COMBINED, SFCNavigation_TBTConstantCombinedSideStreetStat::BOTH_COMBINED}};

    const std::map<NaviDistToDestUnit, SFCNavigation_TBTConstantNaviDistUnitStat> mNaviDistToDestUnitMap = {
        {NaviDistToDestUnit::METER, SFCNavigation_TBTConstantNaviDistUnitStat::METER},
        {NaviDistToDestUnit::KILOMETER, SFCNavigation_TBTConstantNaviDistUnitStat::KILOMETER},
        {NaviDistToDestUnit::MILE, SFCNavigation_TBTConstantNaviDistUnitStat::MILE},
        {NaviDistToDestUnit::FEET, SFCNavigation_TBTConstantNaviDistUnitStat::FEET},
        {NaviDistToDestUnit::YARD, SFCNavigation_TBTConstantNaviDistUnitStat::YARD},
        {NaviDistToDestUnit::INVALID, SFCNavigation_TBTConstantNaviDistUnitStat::OFF}};

    const std::map<TbtDistanceUnit, SFCNavigation_TBTConstantTbtDistanceUnitStat> mTbtDistanceUnitMap = {
        {TbtDistanceUnit::METER, SFCNavigation_TBTConstantTbtDistanceUnitStat::METER},
        {TbtDistanceUnit::KILOMETER, SFCNavigation_TBTConstantTbtDistanceUnitStat::KILOMETER},
        {TbtDistanceUnit::MILE, SFCNavigation_TBTConstantTbtDistanceUnitStat::MILE},
        {TbtDistanceUnit::FEET, SFCNavigation_TBTConstantTbtDistanceUnitStat::FEET},
        {TbtDistanceUnit::YARD, SFCNavigation_TBTConstantTbtDistanceUnitStat::YARD},
        {TbtDistanceUnit::INVALID, SFCNavigation_TBTConstantTbtDistanceUnitStat::OFF}};

    const std::map<NaviDistToTurnFirstUnitStatus, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat>
        mNaviDistToTurnFirstUnitStatusMap = {
            {NaviDistToTurnFirstUnitStatus::METER, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::METER},
            {NaviDistToTurnFirstUnitStatus::KILOMETER, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::KILOMETER},
            {NaviDistToTurnFirstUnitStatus::MILE, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::MILE},
            {NaviDistToTurnFirstUnitStatus::FEET, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::FEET},
            {NaviDistToTurnFirstUnitStatus::YARD, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::YARD},
            {NaviDistToTurnFirstUnitStatus::INVALID, SFCNavigation_TBTConstantNaviDistToTurnUnit1Stat::OFF}};

    const std::map<NaviDistToTurnSecondUnitStatus, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat>
        mNaviDistToTurnSecondUnitStatusMap = {
            {NaviDistToTurnSecondUnitStatus::METER, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::METER},
            {NaviDistToTurnSecondUnitStatus::KILOMETER, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::KILOMETER},
            {NaviDistToTurnSecondUnitStatus::MILE, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::MILE},
            {NaviDistToTurnSecondUnitStatus::FEET, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::FEET},
            {NaviDistToTurnSecondUnitStatus::YARD, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::YARD},
            {NaviDistToTurnSecondUnitStatus::INVALID, SFCNavigation_TBTConstantNaviDistToTurnUnit2Stat::OFF}};

    const std::map<NaviLaneRemainDistUnitStatus, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat>
        mNaviLaneRemainDistUnitStatusMap = {
            {NaviLaneRemainDistUnitStatus::METER, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::METER},
            {NaviLaneRemainDistUnitStatus::KILOMETER, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::KILOMETER},
            {NaviLaneRemainDistUnitStatus::MILE, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::MILE},
            {NaviLaneRemainDistUnitStatus::FEET, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::FEET},
            {NaviLaneRemainDistUnitStatus::YARD, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::YARD},
            {NaviLaneRemainDistUnitStatus::INVALID, SFCNavigation_TBTConstantNaviLaneRemainDistUnitStat::OFF}};

    const std::map<NaviPopupDistUnitStatus, SFCNavigation_TBTConstantNaviPopupDistUnitStat> mNaviPopupDistUnitStatusMap = {
        {NaviPopupDistUnitStatus::METER, SFCNavigation_TBTConstantNaviPopupDistUnitStat::METER},
        {NaviPopupDistUnitStatus::KILOMETER, SFCNavigation_TBTConstantNaviPopupDistUnitStat::KILOMETER},
        {NaviPopupDistUnitStatus::MILE, SFCNavigation_TBTConstantNaviPopupDistUnitStat::MILE},
        {NaviPopupDistUnitStatus::FEET, SFCNavigation_TBTConstantNaviPopupDistUnitStat::FEET},
        {NaviPopupDistUnitStatus::YARD, SFCNavigation_TBTConstantNaviPopupDistUnitStat::YARD},
        {NaviPopupDistUnitStatus::INVALID, SFCNavigation_TBTConstantNaviPopupDistUnitStat::OFF}};

    const std::map<TbtGuideColorStatus, SFCNavigation_TBTInter_NaviTbtColor> mNaviTbtColorMap = {
        {TbtGuideColorStatus::NO_DATA, SFCNavigation_TBTInter_NaviTbtColor::BRAND},
        {TbtGuideColorStatus::PINK, SFCNavigation_TBTInter_NaviTbtColor::PINK},
        {TbtGuideColorStatus::LIGHT_GREEN, SFCNavigation_TBTInter_NaviTbtColor::LIGHT_GREEN},
        {TbtGuideColorStatus::GREEN, SFCNavigation_TBTInter_NaviTbtColor::GREEN},
        {TbtGuideColorStatus::YELLOW, SFCNavigation_TBTInter_NaviTbtColor::YELLOW},
        {TbtGuideColorStatus::ORANGE, SFCNavigation_TBTInter_NaviTbtColor::ORANGE},
        {TbtGuideColorStatus::BLUE, SFCNavigation_TBTInter_NaviTbtColor::BLUE},
        {TbtGuideColorStatus::WHITE, SFCNavigation_TBTInter_NaviTbtColor::WHITE}};

    const std::map<NaviPopupDistUnitStatus, HDouble> mPopupAreaMap = {{NaviPopupDistUnitStatus::METER, 300.0},
                                                                      {NaviPopupDistUnitStatus::FEET, 984.0},
                                                                      {NaviPopupDistUnitStatus::YARD, 328.0},
                                                                      {NaviPopupDistUnitStatus::KILOMETER, 0.3},
                                                                      {NaviPopupDistUnitStatus::MILE, 0.19}};

    const std::map<NaviPopupReqStatus, std::string> mEventIdMap = {
        {NaviPopupReqStatus::ROADSLIP_WRNG1, "E40301"},   {NaviPopupReqStatus::ROADSLIP_WRNG2, "E40302"},
        {NaviPopupReqStatus::ROADDMG_WRNG1, "E40303"},    {NaviPopupReqStatus::ROADDMG_WRNG2, "E40304"},
        {NaviPopupReqStatus::DO_NOT_ENTER, "E40305"},     {NaviPopupReqStatus::SECOND_COLLISION, "E40306"},
        {NaviPopupReqStatus::DANGEROUS_DRIVING, "E40307"}};

    const std::map<NaviPointFlagTypeStatus, SFCNavigation_TBTConstantNaviPointFlagTypeStat> mNaviChinaNaviFlagMap = {
        {NaviPointFlagTypeStatus::DESTINATION, SFCNavigation_TBTConstantNaviPointFlagTypeStat::DESTINATION_CHINA},
        {NaviPointFlagTypeStatus::WAYPOINT1, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT1_CHINA},
        {NaviPointFlagTypeStatus::WAYPOINT2, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT2_CHINA},
        {NaviPointFlagTypeStatus::WAYPOINT3, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT3_CHINA},
        {NaviPointFlagTypeStatus::WAYPOINT4, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT4_CHINA},
        {NaviPointFlagTypeStatus::CHARGING_STATION, SFCNavigation_TBTConstantNaviPointFlagTypeStat::CHARGING_STATION}};

    const std::map<NaviPointFlagTypeStatus, SFCNavigation_TBTConstantNaviPointFlagTypeStat> mNaviGeneralNaviFlagMap = {
        {NaviPointFlagTypeStatus::DESTINATION, SFCNavigation_TBTConstantNaviPointFlagTypeStat::DESTINATION},
        {NaviPointFlagTypeStatus::WAYPOINT1, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT1},
        {NaviPointFlagTypeStatus::WAYPOINT2, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT2},
        {NaviPointFlagTypeStatus::WAYPOINT3, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT3},
        {NaviPointFlagTypeStatus::WAYPOINT4, SFCNavigation_TBTConstantNaviPointFlagTypeStat::WAYPOINT4},
        {NaviPointFlagTypeStatus::CHARGING_STATION, SFCNavigation_TBTConstantNaviPointFlagTypeStat::CHARGING_STATION}};

    const std::map<NaviPointPinTypeStatus, SFCNavigation_TBTConstantNaviPointPinTypeStat> mNaviPointPinMap = {
        {NaviPointPinTypeStatus::NONE, SFCNavigation_TBTConstantNaviPointPinTypeStat::DEFAULT},
        {NaviPointPinTypeStatus::WAYPOINT1, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT1},
        {NaviPointPinTypeStatus::WAYPOINT2, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT2},
        {NaviPointPinTypeStatus::WAYPOINT3, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT3},
        {NaviPointPinTypeStatus::WAYPOINT4, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT4},
        {NaviPointPinTypeStatus::WAYPOINT5, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT5},
        {NaviPointPinTypeStatus::WAYPOINT6, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT6},
        {NaviPointPinTypeStatus::WAYPOINT7, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT7},
        {NaviPointPinTypeStatus::WAYPOINT8, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT8},
        {NaviPointPinTypeStatus::WAYPOINT9, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT9},
        {NaviPointPinTypeStatus::WAYPOINT10, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT10},
        {NaviPointPinTypeStatus::WAYPOINT11, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT11},
        {NaviPointPinTypeStatus::WAYPOINT12, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT12},
        {NaviPointPinTypeStatus::WAYPOINT13, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT13},
        {NaviPointPinTypeStatus::WAYPOINT14, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT14},
        {NaviPointPinTypeStatus::WAYPOINT15, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT15},
        {NaviPointPinTypeStatus::WAYPOINT16, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT16},
        {NaviPointPinTypeStatus::WAYPOINT17, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT17},
        {NaviPointPinTypeStatus::WAYPOINT18, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT18},
        {NaviPointPinTypeStatus::WAYPOINT19, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT19},
        {NaviPointPinTypeStatus::WAYPOINT20, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT20},
        {NaviPointPinTypeStatus::WAYPOINT21, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT21},
        {NaviPointPinTypeStatus::WAYPOINT22, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT22},
        {NaviPointPinTypeStatus::WAYPOINT23, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT23},
        {NaviPointPinTypeStatus::WAYPOINT24, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT24},
        {NaviPointPinTypeStatus::WAYPOINT25, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT25},
        {NaviPointPinTypeStatus::WAYPOINT26, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT26},
        {NaviPointPinTypeStatus::WAYPOINT27, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT27},
        {NaviPointPinTypeStatus::WAYPOINT28, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT28},
        {NaviPointPinTypeStatus::WAYPOINT29, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT29},
        {NaviPointPinTypeStatus::WAYPOINT30, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT30},
        {NaviPointPinTypeStatus::WAYPOINT_COMMON, SFCNavigation_TBTConstantNaviPointPinTypeStat::WAYPOINT_COMMON},
        {NaviPointPinTypeStatus::CHARGING_STATION1, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION1},
        {NaviPointPinTypeStatus::CHARGING_STATION2, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION2},
        {NaviPointPinTypeStatus::CHARGING_STATION3, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION3},
        {NaviPointPinTypeStatus::CHARGING_STATION4, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION4},
        {NaviPointPinTypeStatus::CHARGING_STATION5, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION5},
        {NaviPointPinTypeStatus::CHARGING_STATION6, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION6},
        {NaviPointPinTypeStatus::CHARGING_STATION7, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION7},
        {NaviPointPinTypeStatus::CHARGING_STATION8, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION8},
        {NaviPointPinTypeStatus::CHARGING_STATION9, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION9},
        {NaviPointPinTypeStatus::CHARGING_STATION10, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION10},
        {NaviPointPinTypeStatus::CHARGING_STATION11, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION11},
        {NaviPointPinTypeStatus::CHARGING_STATION12, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION12},
        {NaviPointPinTypeStatus::CHARGING_STATION13, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION13},
        {NaviPointPinTypeStatus::CHARGING_STATION14, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION14},
        {NaviPointPinTypeStatus::CHARGING_STATION15, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION15},
        {NaviPointPinTypeStatus::CHARGING_STATION16, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION16},
        {NaviPointPinTypeStatus::CHARGING_STATION17, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION17},
        {NaviPointPinTypeStatus::CHARGING_STATION18, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION18},
        {NaviPointPinTypeStatus::CHARGING_STATION19, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION19},
        {NaviPointPinTypeStatus::CHARGING_STATION20, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION20},
        {NaviPointPinTypeStatus::CHARGING_STATION21, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION21},
        {NaviPointPinTypeStatus::CHARGING_STATION22, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION22},
        {NaviPointPinTypeStatus::CHARGING_STATION23, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION23},
        {NaviPointPinTypeStatus::CHARGING_STATION24, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION24},
        {NaviPointPinTypeStatus::CHARGING_STATION25, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION25},
        {NaviPointPinTypeStatus::CHARGING_STATION26, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION26},
        {NaviPointPinTypeStatus::CHARGING_STATION27, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION27},
        {NaviPointPinTypeStatus::CHARGING_STATION28, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION28},
        {NaviPointPinTypeStatus::CHARGING_STATION29, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION29},
        {NaviPointPinTypeStatus::CHARGING_STATION30, SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION30},
        {NaviPointPinTypeStatus::CHARGING_STATION_COMMON,
         SFCNavigation_TBTConstantNaviPointPinTypeStat::CHARGING_STATION_COMMON}};
};
}  // namespace ccos

#endif  // SFSS_Navigation_TBT_H
