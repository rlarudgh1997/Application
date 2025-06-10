/**
 * @file Detail_Contents_TBT.hpp
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
#ifndef SFSS_Detail_Contents_TBT_H
#define SFSS_Detail_Contents_TBT_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_TBTBase.hpp"

namespace ccos {

// SFC Version : 20.0.0
// Reference : [HD130] Detail_Contents_TBT. Contains : Constant. Recommend: VALUE_CHANGED
class Detail_Contents_TBT : public Detail_Contents_TBTBase {
public:
    Detail_Contents_TBT() = default;
    ~Detail_Contents_TBT() override = default;
    Detail_Contents_TBT(const Detail_Contents_TBT& other) = delete;
    Detail_Contents_TBT(Detail_Contents_TBT&& other) noexcept = delete;
    Detail_Contents_TBT& operator=(const Detail_Contents_TBT& other) = delete;
    Detail_Contents_TBT& operator=(Detail_Contents_TBT&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAllSignal();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAllSignal();
    }

    void onTbtDirectionChanged(const ArgumentsTbtDirectionChanged& args) {
        mTbtDirectionValue = args.mInput_TbtDirectionValue;
        updateDirectionValue();
    }

    void onNaviTbtChanged(const ArgumentsNaviTbtChanged& args) {
        mTbtDisplayTypeStatus = args.mInput_TbtDisplayTypeStatus;
        updateNaviTbtStat();
    }

    void onNaviDistToTurnFirstChanged(const ArgumentsNaviDistToTurnFirstChanged& args) {
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
        mNaviDistToTurnFirstUnitStatus = args.mInput_NaviDistToTurnFirstUnitStatus;

        updateNaviDistToTurn1Value();
        updateNaviDistToTurn1Stat();
    }

    void onNaviDistToTurnSecondChanged(const ArgumentsNaviDistToTurnSecondChanged& args) {
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
        mNaviDistToTurnSecondUnitStatus = args.mInput_NaviDistToTurnSecondUnitStatus;

        updateNaviDistToTurn2Value();
        updateNaviDistToTurn2Stat();
    }

    void onSignalPatternGuideLaneStatusChanged(const ArgumentsSignalPatternGuideLaneStatusChanged& args) {
        mPatternGuideLaneStatus = args.mInput_PatternGuideLaneStatus & 0xff;  // use only 8bits (bit0 ~ bit7)
        if (ISTIMEOUT(args.mInput_PatternGuideLaneStatus) == true) {
            mPatternGuideLaneStatus = 0x00;  // Default
        }
        updatePatternType();
        updatePatternTbtLanes();
        updatePatternTbt1();
        updatePatternTbt2();
        updateNaviPatternTbtLaneColor();
    }

    void onSignalNaviPatternTbtIndexValueChanged(const ArgumentsSignalNaviPatternTbtIndexValueChanged& args) {
        mNaviPatternTbtBgIndexValue = args.mInput_NaviPatternTbtBgIndexValue;
        mNaviPatternTbt1IndexValue = args.mInput_NaviPatternTbt1IndexValue;
        mNaviPatternTbt2IndexValue = args.mInput_NaviPatternTbt2IndexValue;
        updatePatternTbtBg();
        updatePatternTbt1();
        updatePatternTbt2();
    }

    void onSignalNaviPatternGuideColorChanged(const ArgumentsSignalNaviPatternGuideColorChanged& args) {
        mPatternGuideColorstatus = args.mInput_PatternGuideColorStatus;
        updateNaviPatternTbtLaneColor();
    }

    void onSignalNaviRoadEventStatusChanged(const ArgumentsSignalNaviRoadEventStatusChanged& args) {
        mNaviRoadEventInfoStatus = args.mInput_NaviRoadEventInfoStatus;
        mConfigNaviDisCountry = args.mInter_ConfigNaviDisCountry;
        updateRoadEventStat();
    }

    void onSignalNaviRoadEventDistChanged(const ArgumentsSignalNaviRoadEventDistChanged& args) {
        mNaviRoadEventDistUnitStatus = args.mInput_NaviRoadEventDistUnitStatus;
        mNaviRoadEventDistOverDecimalValue = args.mInput_NaviRoadEventDistOverDecimalValue;
        mNaviRoadEventDistUnderDecimalValue = args.mInput_NaviRoadEventDistUnderDecimalValue;
        updateRoadEventDistValue();
        updateNaviRoadEventDistUnitStat();
    }

    void onNaviPopupChanged(const ArgumentsNaviPopupChanged& args) {
        mNaviPopupReqStatus = args.mInput_NaviPopupReqStatus;
        updateEvent();
    }

    void onSignalHudTBTInfoOnOffStatusChanged(const ArgumentsSignalHudTBTInfoOnOffStatusChanged& args) {
        if (updateValueIfValid(mHudTBTInfoOnOffStatus, args.mInter_HudTBTInfoOnOffStatus)) {
            updateAllSignal();
        }
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigTBT = args.mInter_ConfigTBT;
        mConfigNewHUD = args.mInter_ConfigNewHUD;

        updateAllSignal();
    }

    void onNaviAliveStatusChanged(const ArgumentsNaviAliveStatusChanged& args) {
        mNaviAliveStatus = args.mInter_NaviAliveStatus;

        updateNaviTbtStat();
        updatePatternTbtBg();
        updatePatternTbt1();
        updatePatternTbt2();
        updatePatternTbtLanes();
        updateNaviPatternTbtLaneColor();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAllSignal() {
        updateNaviTbtStat();
        updateNaviDistToTurn1Value();
        updateNaviDistToTurn1Stat();
        updateNaviDistToTurn2Value();
        updateNaviDistToTurn2Stat();
        updateDirectionValue();
        updatePatternType();
        updatePatternTbtBg();
        updatePatternTbt1();
        updatePatternTbt2();
        updatePatternTbtLanes();
        updateNaviPatternTbtLaneColor();
        updateRoadEventStat();
        updateRoadEventDistValue();
        updateNaviRoadEventDistUnitStat();
        updateEvent();
    }

    inline HBool isTBTOn() const {
        return mConfigTBT == ConfigTBT::ON && (mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON) &&
               mHudTBTInfoOnOffStatus == HudTBTInfoOnOffStatus::ENABLE;
    }

    inline HBool isTBTIgnOn() const {
        return isTBTOn() == true && mIgnElapsed == IgnElapsed::ON_0ms;
    }

    void updateDirectionValue() {
        SFCDetail_Contents_TBTConstantDirectionTypeStat stat = SFCDetail_Contents_TBTConstantDirectionTypeStat::OFF;

        if (isTBTIgnOn() == true) {
            const auto& it = mDirectionMap.find(mTbtDirectionValue);

            if (it != mDirectionMap.end()) {
                stat = it->second;
            }
        }

        setSFCDetail_Contents_TBTConstantDirectionTypeStat(stat);
    }

    void updateNaviTbtStat() {
        SFCDetail_Contents_TBTConstantNaviTbtStat stat = SFCDetail_Contents_TBTConstantNaviTbtStat::OFF;

        if (isTBTIgnOn() == true) {
            if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                if (mTbtDisplayTypeStatus == TbtDisplayTypeStatus::DIRECTION_TO_DESTINATION) {
                    stat = SFCDetail_Contents_TBTConstantNaviTbtStat::DIRECTION_TO_DESTINATION;
                } else if (mTbtDisplayTypeStatus == TbtDisplayTypeStatus::ARRIVED) {
                    stat = SFCDetail_Contents_TBTConstantNaviTbtStat::ARRIVED;
                } else if (mTbtDisplayTypeStatus == TbtDisplayTypeStatus::CALC_ROUTE) {
                    stat = SFCDetail_Contents_TBTConstantNaviTbtStat::CALC_ROUTE;
                } else if (mTbtDisplayTypeStatus == TbtDisplayTypeStatus::RECALC_ROUTE) {
                    stat = SFCDetail_Contents_TBTConstantNaviTbtStat::RECALC_ROUTE;
                } else {
                    // OFF
                }
            }
        }

        setSFCDetail_Contents_TBTConstantNaviTbtStat(stat);
    }

    void updateNaviDistToTurn1Value() {
        HDouble turnFirstVal = 0.0;
        SFCDetail_Contents_TBTConstantNaviDistToTurn1Stat turnFirstStat = SFCDetail_Contents_TBTConstantNaviDistToTurn1Stat::OFF;
        calTBTDistanceValue(mNaviDistToTurnFirstValue, mNaviDistToTurnFirstUnderDecimalValue, mNaviDistToTurnFirstUnitStatus,
                            turnFirstVal, turnFirstStat);
        setSFCDetail_Contents_TBTConstantNaviDistToTurn1Value(turnFirstVal);
        setSFCDetail_Contents_TBTConstantNaviDistToTurn1Stat(turnFirstStat);
    }

    void updateNaviDistToTurn1Stat() {
        SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat unitStat =
            SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::OFF;
        calUnitStat(mNaviDistToTurnFirstValue, mNaviDistToTurnFirstUnitStatus, mNaviDistToTurnFirstUnitStatusMap, unitStat);
        setSFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat(unitStat);
    }

    void updateNaviDistToTurn2Value() {
        HDouble turnSecondVal = 0.0;
        SFCDetail_Contents_TBTConstantNaviDistToTurn2Stat turnSecondStat = SFCDetail_Contents_TBTConstantNaviDistToTurn2Stat::OFF;
        calTBTDistanceValue(mNaviDistToTurnSecondValue, mNaviDistToTurnSecondUnderDecimalValue, mNaviDistToTurnSecondUnitStatus,
                            turnSecondVal, turnSecondStat);
        setSFCDetail_Contents_TBTConstantNaviDistToTurn2Value(turnSecondVal);
        setSFCDetail_Contents_TBTConstantNaviDistToTurn2Stat(turnSecondStat);
    }

    void updateNaviDistToTurn2Stat() {
        SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat unitStat =
            SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::OFF;
        calUnitStat(mNaviDistToTurnSecondValue, mNaviDistToTurnSecondUnitStatus, mNaviDistToTurnSecondUnitStatusMap, unitStat);
        setSFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat(unitStat);
    }

    void updatePatternType() {
        SFCDetail_Contents_TBTConstantInter_PatternType type = SFCDetail_Contents_TBTConstantInter_PatternType::OFF;

        if (isTBTIgnOn() == true) {
            if (mPatternGuideLaneStatus == 0) {
                // Bit0==0 && Bit1==0 && Bit2==0 && Bit3 ==0 && Bit4==0 && Bit5==0 && Bit6 == 0 && Bit7 == 0
                type = SFCDetail_Contents_TBTConstantInter_PatternType::TYPE1;
            } else if (mPatternGuideLaneStatus != 0) {
                // Bit0==1 || Bit1==1 || Bit2==1 || Bit3 ==1 || Bit4==1 || Bit5==1 || Bit6 == 1 || Bit7 == 1
                type = SFCDetail_Contents_TBTConstantInter_PatternType::TYPE2;
            } else {
                // impossible
                type = SFCDetail_Contents_TBTConstantInter_PatternType::TYPE1;
            }
        }

        mPatternType = type;
        setSFCDetail_Contents_TBTConstantInter_PatternType(type);
    }

    // 5.1.4 패턴분기 모식도
    void updatePatternTbtBg() {
        HUInt64 value = 0;
        SFCDetail_Contents_TBTConstantNaviPatternTbtBgStat stat = SFCDetail_Contents_TBTConstantNaviPatternTbtBgStat::OFF;

        if (isTBTIgnOn() == true) {
            if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                if (patternTbtValid(mNaviPatternTbtBgIndexValue) == true) {
                    value = mNaviPatternTbtBgIndexValue;
                    stat = SFCDetail_Contents_TBTConstantNaviPatternTbtBgStat::VALUE;
                }
            }
        }

        setSFCDetail_Contents_TBTConstantNaviPatternTbtBgValue(value);
        setSFCDetail_Contents_TBTConstantNaviPatternTbtBgStat(stat);
    }

    void updatePatternTbt1() {
        HUInt64 value = 0;
        SFCDetail_Contents_TBTConstantNaviPatternTbt1Stat stat = SFCDetail_Contents_TBTConstantNaviPatternTbt1Stat::OFF;

        if (isTBTIgnOn() == true) {
            if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                if (mPatternType == SFCDetail_Contents_TBTConstantInter_PatternType::TYPE1 &&
                    patternTbtValid(mNaviPatternTbt1IndexValue) == true) {
                    value = mNaviPatternTbt1IndexValue;
                    stat = SFCDetail_Contents_TBTConstantNaviPatternTbt1Stat::VALUE;
                }
            }
        }

        setSFCDetail_Contents_TBTConstantNaviPatternTbt1Value(value);
        setSFCDetail_Contents_TBTConstantNaviPatternTbt1Stat(stat);
    }

    void updatePatternTbt2() {
        HUInt64 value = 0;
        SFCDetail_Contents_TBTConstantNaviPatternTbt2Stat stat = SFCDetail_Contents_TBTConstantNaviPatternTbt2Stat::OFF;

        if (isTBTIgnOn() == true) {
            if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                if (mPatternType == SFCDetail_Contents_TBTConstantInter_PatternType::TYPE1 &&
                    patternTbtValid(mNaviPatternTbt2IndexValue) == true) {
                    value = mNaviPatternTbt2IndexValue;
                    stat = SFCDetail_Contents_TBTConstantNaviPatternTbt2Stat::VALUE;
                }
            }
        }

        setSFCDetail_Contents_TBTConstantNaviPatternTbt2Value(value);
        setSFCDetail_Contents_TBTConstantNaviPatternTbt2Stat(stat);
    }

    void updatePatternTbtLanes() {
        HUInt64 bitFlag = 0;

        for (HUInt64 laneIndex = 1; laneIndex <= 8; laneIndex++) {
            bitFlag = (bitFlag << 1) | 0x01;

            if (isTBTIgnOn() == true && mNaviAliveStatus == NaviAliveStatus::NORMAL &&
                nthBitFlag(mPatternGuideLaneStatus, laneIndex - 1) == true) {
                HUInt64 checkingBit = mPatternGuideLaneStatus & bitFlag;
                HUInt64 count = (HUInt64)__builtin_popcount(checkingBit);
                setPatternTbtLane(laneIndex, (HUInt64)PatternTbtLaneStat::VALUE, count);
            } else {
                setPatternTbtLane(laneIndex, (HUInt64)PatternTbtLaneStat::OFF, (HUInt64)0);
            }
        }
    }

    void updateNaviPatternTbtLaneColor() {
        SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat stat =
            SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::OFF;

        if (isTBTIgnOn() == true) {
            if (mNaviAliveStatus == NaviAliveStatus::NORMAL) {
                if (mPatternType == SFCDetail_Contents_TBTConstantInter_PatternType::TYPE1) {
                    stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::BRAND;
                } else if (mPatternType == SFCDetail_Contents_TBTConstantInter_PatternType::TYPE2) {
                    switch (mPatternGuideColorstatus) {
                        case PatternGuideColorStatus::PINK:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::PINK;
                            break;
                        case PatternGuideColorStatus::LIGHT_GREEN:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::LIGHT_GREEN;
                            break;
                        case PatternGuideColorStatus::GREEN:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::GREEN;
                            break;
                        case PatternGuideColorStatus::YELLOW:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::YELLOW;
                            break;
                        case PatternGuideColorStatus::ORANGE:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::ORANGE;
                            break;
                        case PatternGuideColorStatus::BLUE:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::BLUE;
                            break;
                        case PatternGuideColorStatus::WHITE:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::WHITE;
                            break;
                        case PatternGuideColorStatus::NO_DATA:
                            stat = SFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat::BRAND;
                            break;
                        default:
                            // OFF
                            break;
                    }
                } else {
                    // OFF
                }
            }
        }

        setSFCDetail_Contents_TBTConstantNaviPatternTbtLaneColorStat(stat);
    }

    // 5.1.6 유고 정보
    void updateRoadEventStat() {
        SFCDetail_Contents_TBTConstantNaviRoadEventStat roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::OFF;

        if (isTBTIgnOn() == true) {
            switch (mNaviRoadEventInfoStatus) {
                case NaviRoadEventInfoStatus::ROAD_CLOSED:
                    if (mConfigNaviDisCountry == ConfigNaviDisCountry::EUROPE ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::MIDDLE_EAST ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::AUSTRALIA ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_AMERICA ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_E ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_N ||
                        mConfigNaviDisCountry == ConfigNaviDisCountry::JAPAN) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_CLOSED;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::CHINA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_CLOSED_CHINA;
                    } else {
                        // OFF
                    }
                    break;
                case NaviRoadEventInfoStatus::ACCIDENT:
                    if (mConfigNaviDisCountry == ConfigNaviDisCountry::KOREA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ACCIDENT_A;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::EUROPE ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MIDDLE_EAST) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ACCIDENT_B;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::USA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::CANADA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MEXICO ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::AUSTRALIA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_AMERICA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_E ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_N) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ACCIDENT_C;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::CHINA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ACCIDENT_C_CHINA;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::JAPAN) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ACCIDENT_C_JAPAN;
                    } else {
                        // OFF
                    }
                    break;
                case NaviRoadEventInfoStatus::ROAD_WORK:
                    if (mConfigNaviDisCountry == ConfigNaviDisCountry::KOREA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_WORK_A;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::EUROPE ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MIDDLE_EAST) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_WORK_B;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::USA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::CANADA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MEXICO ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::AUSTRALIA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_AMERICA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_E ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_EAST_ASIA_N) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_WORK_C;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::CHINA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_WORK_C_CHINA;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::JAPAN) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ROAD_WORK_C_JAPAN;
                    } else {
                        // OFF
                    }
                    break;
                case NaviRoadEventInfoStatus::ETC:
                    if (mConfigNaviDisCountry == ConfigNaviDisCountry::KOREA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ETC_A;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::EUROPE ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MIDDLE_EAST) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ETC_B;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::USA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::CANADA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::MEXICO ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::AUSTRALIA ||
                               mConfigNaviDisCountry == ConfigNaviDisCountry::SOUTH_AMERICA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::ETC_C;
                    } else {
                        // OFF
                    }
                    break;
                case NaviRoadEventInfoStatus::EVENT:
                    if (mConfigNaviDisCountry == ConfigNaviDisCountry::KOREA) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::EVENT;
                    } else if (mConfigNaviDisCountry == ConfigNaviDisCountry::JAPAN) {
                        roadEventStat = SFCDetail_Contents_TBTConstantNaviRoadEventStat::EVENT_JAPAN;
                    } else {
                        // OFF
                    }
                    break;
                default:
                    break;
            }
        }
        setSFCDetail_Contents_TBTConstantNaviRoadEventStat(roadEventStat);
    }

    // 5.1.6 잔여 거리 표시
    void updateRoadEventDistValue() {
        if (isTBTOn() == false) {
            return;
        }

        SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat roadEventDistToDestStat =
            SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat::OFF;
        HDouble roadEventDistToDestValue = 0.0;

        if (mNaviRoadEventDistOverDecimalValue != kLimitNaviDistToDestValue65534 &&
            mNaviRoadEventDistOverDecimalValue != kLimitNaviDistToDestValue65535) {
            switch (mNaviRoadEventDistUnitStatus) {
                case NaviRoadEventDistUnitStatus::METER:
                case NaviRoadEventDistUnitStatus::FEET:
                case NaviRoadEventDistUnitStatus::YARD:
                    roadEventDistToDestValue = static_cast<HDouble>(mNaviRoadEventDistOverDecimalValue);
                    roadEventDistToDestStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat::VALUE;
                    break;
                case NaviRoadEventDistUnitStatus::KILOMETER:
                case NaviRoadEventDistUnitStatus::MILE:
                    if (mNaviRoadEventDistOverDecimalValue < kRoadEventDistOverDecimalValue) {
                        if (mNaviRoadEventDistUnderDecimalValue < kRoadEventDistUnderDecimalValue) {
                            roadEventDistToDestValue = static_cast<HDouble>(mNaviRoadEventDistOverDecimalValue) +
                                                       static_cast<HDouble>(mNaviRoadEventDistUnderDecimalValue) * 0.1;
                            roadEventDistToDestStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat::VALUE;
                        } else {
                            roadEventDistToDestValue = static_cast<HDouble>(mNaviRoadEventDistOverDecimalValue);
                            roadEventDistToDestStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat::VALUE;
                        }
                    } else {
                        roadEventDistToDestValue = static_cast<HDouble>(mNaviRoadEventDistOverDecimalValue);
                        roadEventDistToDestStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat::VALUE;
                    }
                    break;
                case NaviRoadEventDistUnitStatus::INVALID:
                    break;
                default:
                    break;
            }
        }
        setSFCDetail_Contents_TBTConstantNaviRoadEventDistToDestStat(roadEventDistToDestStat);
        setSFCDetail_Contents_TBTConstantNaviRoadEventDistToDestValue(roadEventDistToDestValue);
    }

    // 5.1.6 단위 표시
    void updateNaviRoadEventDistUnitStat() {
        if (isTBTOn() == false) {
            return;
        }

        SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat roadEventDistUnitStat =
            SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::OFF;
        if ((mNaviRoadEventDistOverDecimalValue != kLimitNaviDistToDestValue65534) &&
            (mNaviRoadEventDistOverDecimalValue != kLimitNaviDistToDestValue65535)) {
            switch (mNaviRoadEventDistUnitStatus) {
                case NaviRoadEventDistUnitStatus::METER:
                    roadEventDistUnitStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::METER;
                    break;
                case NaviRoadEventDistUnitStatus::KILOMETER:
                    roadEventDistUnitStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::KILOMETER;
                    break;
                case NaviRoadEventDistUnitStatus::MILE:
                    roadEventDistUnitStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::MILE;
                    break;
                case NaviRoadEventDistUnitStatus::FEET:
                    roadEventDistUnitStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::FEET;
                    break;
                case NaviRoadEventDistUnitStatus::YARD:
                    roadEventDistUnitStat = SFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat::YARD;
                    break;
                case NaviRoadEventDistUnitStatus::INVALID:
                    break;
                default:
                    break;
            }
        }
        setSFCDetail_Contents_TBTConstantNaviRoadEventDistUnitStat(roadEventDistUnitStat);
    }

    void updateEvent() {
        std::string prevEventID;
        std::string eventID;

        if (isTBTIgnOn() == true) {
            if (mNaviPopupReqStatus == NaviPopupReqStatus::ROADSLIP_WRNG1) {
                eventID = "E71150";
            } else if (mNaviPopupReqStatus == NaviPopupReqStatus::ROADDMG_WRNG1) {
                eventID = "E71152";
            } else if (mNaviPopupReqStatus == NaviPopupReqStatus::DO_NOT_ENTER) {
                eventID = "E71154";
            } else if (mNaviPopupReqStatus == NaviPopupReqStatus::SECOND_COLLISION) {
                eventID = "E71155";
            } else if (mNaviPopupReqStatus == NaviPopupReqStatus::DANGEROUS_DRIVING) {
                eventID = "E71156";
            } else {
                // nothing
            }
        }

        GETCACHEDVALUE(SFC.Detail_Contents_TBT.Event.NaviPopupDist.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDetail_Contents_TBTEventNaviPopupDistStat(SFCDetail_Contents_TBTEventNaviPopupDistStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDetail_Contents_TBTEventNaviPopupDistID(eventID);
            setSFCDetail_Contents_TBTEventNaviPopupDistStat(SFCDetail_Contents_TBTEventNaviPopupDistStat::ON);
        }
    }

    void setPatternTbtLane(HUInt64 laneIndex, HUInt64 stat, HUInt64 value) {
        if (laneIndex == 1) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane1Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane1Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane1Value(value);
        } else if (laneIndex == 2) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane2Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane2Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane2Value(value);
        } else if (laneIndex == 3) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane3Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane3Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane3Value(value);
        } else if (laneIndex == 4) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane4Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane4Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane4Value(value);
        } else if (laneIndex == 5) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane5Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane5Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane5Value(value);
        } else if (laneIndex == 6) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane6Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane6Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane6Value(value);
        } else if (laneIndex == 7) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane7Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane7Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane7Value(value);
        } else if (laneIndex == 8) {
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane8Stat(
                static_cast<SFCDetail_Contents_TBTConstantNaviPatternTbtLane8Stat>(stat));
            setSFCDetail_Contents_TBTConstantNaviPatternTbtLane8Value(value);
        } else {
            // Error
        }
    }

    template <typename T1, typename T2>
    inline void calTBTDistanceValue(const HUInt64& ovrDecVal, const HDouble& undDecVal, const T1& unitStat, HDouble& resVal,
                                    T2& nStat) {
        if (isTBTIgnOn() == true && ovrDecVal != 0xFFFE && ovrDecVal != 0xFFFF) {
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
        if (isTBTIgnOn() == true && overDecVal != 0xFFFE && overDecVal != 0xFFFF) {
            const auto& it = statValueMap.find(unitStatus);
            if (it != statValueMap.end()) {
                statVal = it->second;
            }
        }
    }

    inline HBool nthBitFlag(HUInt64 target, HUInt64 nthBit) const {
        return (HBool)(target & ((HUInt64)0x01 << (nthBit)));
    }

    inline HBool patternTbtValid(HUInt64 value) {
        return (kNaviPatternTbtMin <= value && value < kNaviPatternTbtInvalid);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    enum class PatternTbtLaneStat : HUInt64 {
        NONE,
        OFF,
        VALUE,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using TbtDisplayTypeStatus = ArgumentsNaviTbtChanged::Input_TbtDisplayTypeStatus;
    using NaviDistToTurnFirstUnitStatus = ArgumentsNaviDistToTurnFirstChanged::Input_NaviDistToTurnFirstUnitStatus;
    using NaviDistToTurnSecondUnitStatus = ArgumentsNaviDistToTurnSecondChanged::Input_NaviDistToTurnSecondUnitStatus;
    using PatternGuideColorStatus = ArgumentsSignalNaviPatternGuideColorChanged::Input_PatternGuideColorStatus;
    using NaviRoadEventInfoStatus = ArgumentsSignalNaviRoadEventStatusChanged::Input_NaviRoadEventInfoStatus;
    using ConfigNaviDisCountry = ArgumentsSignalNaviRoadEventStatusChanged::Inter_ConfigNaviDisCountry;
    using NaviRoadEventDistUnitStatus = ArgumentsSignalNaviRoadEventDistChanged::Input_NaviRoadEventDistUnitStatus;
    using NaviPopupReqStatus = ArgumentsNaviPopupChanged::Input_NaviPopupReqStatus;
    using HudTBTInfoOnOffStatus = ArgumentsSignalHudTBTInfoOnOffStatusChanged::Inter_HudTBTInfoOnOffStatus;
    using ConfigHUD = ArgumentsConfigChanged::Inter_ConfigHUD;
    using ConfigTBT = ArgumentsConfigChanged::Inter_ConfigTBT;
    using ConfigNewHUD = ArgumentsConfigChanged::Inter_ConfigNewHUD;
    using NaviAliveStatus = ArgumentsNaviAliveStatusChanged::Inter_NaviAliveStatus;

    HUInt64 mNaviDistToTurnFirstValue = 0;
    HDouble mNaviDistToTurnFirstUnderDecimalValue = 0;
    HUInt64 mNaviDistToTurnSecondValue = 0;
    HDouble mNaviDistToTurnSecondUnderDecimalValue = 0;
    HUInt64 mTbtDirectionValue = UINT64_MAX;
    HUInt64 mPatternGuideLaneStatus = 0;
    HUInt64 mNaviPatternTbtBgIndexValue = 0;
    HUInt64 mNaviPatternTbt1IndexValue = 0;
    HUInt64 mNaviPatternTbt2IndexValue = 0;
    HUInt64 mNaviRoadEventDistOverDecimalValue = 0;
    HUInt64 mNaviRoadEventDistUnderDecimalValue = 0;
    TbtDisplayTypeStatus mTbtDisplayTypeStatus = TbtDisplayTypeStatus::OFF;
    NaviDistToTurnFirstUnitStatus mNaviDistToTurnFirstUnitStatus = NaviDistToTurnFirstUnitStatus::INVALID;
    NaviDistToTurnSecondUnitStatus mNaviDistToTurnSecondUnitStatus = NaviDistToTurnSecondUnitStatus::INVALID;
    NaviRoadEventInfoStatus mNaviRoadEventInfoStatus = NaviRoadEventInfoStatus::OFF;
    PatternGuideColorStatus mPatternGuideColorstatus = PatternGuideColorStatus::NO_DATA;
    ConfigNaviDisCountry mConfigNaviDisCountry = ConfigNaviDisCountry::NONE;
    NaviRoadEventDistUnitStatus mNaviRoadEventDistUnitStatus = NaviRoadEventDistUnitStatus::INVALID;
    NaviPopupReqStatus mNaviPopupReqStatus = NaviPopupReqStatus::OFF;
    HudTBTInfoOnOffStatus mHudTBTInfoOnOffStatus = HudTBTInfoOnOffStatus::INVALID;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigTBT mConfigTBT = ConfigTBT::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    NaviAliveStatus mNaviAliveStatus = NaviAliveStatus::NONE;
    SFCDetail_Contents_TBTConstantInter_PatternType mPatternType = SFCDetail_Contents_TBTConstantInter_PatternType::OFF;

    static constexpr HUInt64 kDisplayDecimalPoint = 100;
    static constexpr HUInt64 kLimitNaviDistToDestValue = 65534;
    static constexpr HUInt64 kNaviPatternTbtMin = 1;
    static constexpr HUInt64 kNaviPatternTbtMax =
        12204;  // 추후 AVN의 TBT 값 확장을 대비하여 MAX 값을 실제로 확인하지는 않음. 명시적으로 남겨만 둠.
    static constexpr HUInt64 kNaviPatternTbtInvalid = 0xFFFF;
    static constexpr HUInt64 kRoadEventDistOverDecimalValue = 100;
    static constexpr HUInt64 kRoadEventDistUnderDecimalValue = 10;
    static constexpr HUInt64 kLimitNaviDistToDestValue65534 = 65534;
    static constexpr HUInt64 kLimitNaviDistToDestValue65535 = 65535;

    const std::map<HUInt64, SFCDetail_Contents_TBTConstantDirectionTypeStat> mDirectionMap = {
        {0x0, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X0},
        {0x3, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X3},
        {0x6, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X6},
        {0x9, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X9},
        {0xC, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0XC},
        {0xF, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0XF},
        {0x12, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X12},
        {0x15, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X15},
        {0x18, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X18},
        {0x1B, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X1B},
        {0x1E, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X1E},
        {0x21, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X21},
        {0x24, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X24},
        {0x27, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X27},
        {0x2A, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X2A},
        {0x2D, SFCDetail_Contents_TBTConstantDirectionTypeStat::VALUE_0X2D}};

    const std::map<NaviDistToTurnFirstUnitStatus, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat>
        mNaviDistToTurnFirstUnitStatusMap = {
            {NaviDistToTurnFirstUnitStatus::METER, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::METER},
            {NaviDistToTurnFirstUnitStatus::KILOMETER, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::KILOMETER},
            {NaviDistToTurnFirstUnitStatus::MILE, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::MILE},
            {NaviDistToTurnFirstUnitStatus::FEET, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::FEET},
            {NaviDistToTurnFirstUnitStatus::YARD, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::YARD},
            {NaviDistToTurnFirstUnitStatus::INVALID, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit1Stat::OFF}};

    const std::map<NaviDistToTurnSecondUnitStatus, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat>
        mNaviDistToTurnSecondUnitStatusMap = {
            {NaviDistToTurnSecondUnitStatus::METER, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::METER},
            {NaviDistToTurnSecondUnitStatus::KILOMETER, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::KILOMETER},
            {NaviDistToTurnSecondUnitStatus::MILE, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::MILE},
            {NaviDistToTurnSecondUnitStatus::FEET, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::FEET},
            {NaviDistToTurnSecondUnitStatus::YARD, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::YARD},
            {NaviDistToTurnSecondUnitStatus::INVALID, SFCDetail_Contents_TBTConstantNaviDistToTurnUnit2Stat::OFF}};
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_TBT_H
