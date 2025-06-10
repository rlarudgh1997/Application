/**
 * @file Navigation_DIS.hpp
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
#ifndef SFSS_Navigation_DIS_H
#define SFSS_Navigation_DIS_H

#define DLOG_ENABLED gEnableSFCLog

#include "Navigation_DISBase.hpp"

namespace ccos {

// SFC Version : 22.0.0
// Reference : [AV040] Navigation_DIS. Contains : Constant. Recommend: VALUE_CHANGED
class Navigation_DIS : public Navigation_DISBase {
public:
    Navigation_DIS() = default;
    ~Navigation_DIS() override = default;
    Navigation_DIS(const Navigation_DIS& other) = delete;
    Navigation_DIS(Navigation_DIS&& other) noexcept = delete;
    Navigation_DIS& operator=(const Navigation_DIS& other) = delete;
    Navigation_DIS& operator=(Navigation_DIS&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::ON_0ms;
        updateInterConfigNaviDisCountry();
        updateDISMainSpeedStat();
        updateDISMainSpeedValue();
        updateInterDISAddtnlSign();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::OFF_0ms;
        updateDISMainSpeedStat();
        updateDISMainSpeedValue();
        updateInterDISAddtnlSign();
    }

    void onHeadUnitNaviStatus(const ArgumentsHeadUnitNaviStatus& args) {
        mHeadUnitNaviStatus = args.mInput_HeadUnitNaviStatus;
        updateDISMainSpeedStat();
        updateDISMainSpeedValue();
    }

    void onDisSpeedLimitValue(const ArgumentsDisSpeedLimitValue& args) {
        mDisSpeedLimitValue = args.mInput_DisSpeedLimitValue;
        updateDISMainSpeedValue();
    }

    void onDisCameraSafetyAlertStatus(const ArgumentsDisCameraSafetyAlertStatus& args) {
        if (ISTIMEOUT(args.mInput_DisCameraSafetyAlertStatus) == true) {
            mDisCameraSafetyAlertStatus = DisCameraSafetyAlertStatus::UNHANDLED_TIMEOUT;
        } else {
            mDisCameraSafetyAlertStatus = args.mInput_DisCameraSafetyAlertStatus;
        }
        updateInterDISAddtnlSign();
    }

    void onIslaOptionstatus(const ArgumentsIslaOptionstatus& args) {
        mIslaOptionStatus = args.mInput_IslaOptionStatus;
        updateInterDISAddtnlSign();
    }

    void onNaviDisplayCountryStatus(const ArgumentsNaviDisplayCountryStatus& args) {
        if (ISTIMEOUT(args.mInput_NaviDisplayCountryStatus) == true) {
            mNaviDisplayCountryStatus = NaviDisplayCountryStatus::UNHANDLED_TIMEOUT;
        } else {
            mNaviDisplayCountryStatus = args.mInput_NaviDisplayCountryStatus;
        }
        updateInterConfigNaviDisCountry();
        updateDISMainSpeedStat();
        updateInterDISAddtnlSign();
    }

    void onDisWarnSetStatusChanged(const ArgumentsDisWarnSetStatusChanged& args) {
        mDisWarnSetStatus = args.mInter_DisWarnSetStatus;
        updateInterDISAddtnlSign();
        updateDISMainSpeedStat();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigAdasISLA = args.mInter_ConfigAdasISLA;
        updateInterDISAddtnlSign();
        updateDISMainSpeedStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateInterConfigNaviDisCountry() {
        // 5.3.1 연산 로직
        if (mIgnElapsedStatus <= IgnElapsed::OFF_0ms) {
            DDebug() << "[Ign is OFF]";
            return;
        }
        const auto iter = mNaviDisCountryMap.find(mNaviDisplayCountryStatus);
        if (iter != mNaviDisCountryMap.end()) {
            mInter_ConfigNaviDisCountry = iter->second;
        } else {
            DDebug() << "is TimeOut";
        }
        setSFCNavigation_DISInter_ConfigNaviDisCountry(mInter_ConfigNaviDisCountry);
    }

    // 5.3.2.2.1.1 형상
    void updateDISMainSpeedStat() {
        SFCNavigation_DISConstantDISMainSpeedStat state = SFCNavigation_DISConstantDISMainSpeedStat::OFF;
        if (mIgnElapsedStatus >= IgnElapsed::ON_0ms && mConfigAdasISLA == ConfigAdasISLA::OFF) {
            if (mDisWarnSetStatus == DisWarnSetStatus::ENABLE) {
                if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NORMAL) {
                    const auto iter = mDisMainSpeedStatMap.find(mInter_ConfigNaviDisCountry);
                    if (iter != mDisMainSpeedStatMap.end()) {
                        state = iter->second;
                    }
                }
            } else if (mDisWarnSetStatus == DisWarnSetStatus::DISABLE) {
                // nothing to do
            } else {
                // nothing to do
            }
        } else if (mConfigAdasISLA == ConfigAdasISLA::ON) {
            // nothing to do
        } else {
            // nothing to do
        }
        setSFCNavigation_DISConstantDISMainSpeedStat(state);
    }

    void updateDISMainSpeedValue() {
        // 5.3.2.2.1.2 속도값
        if (mIgnElapsedStatus < IgnElapsed::ON_0ms) {
            DDebug() << "[Ign is OFF]";
            return;
        }

        if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NORMAL) {
            if (mDisSpeedLimitValue >= 0x1 && mDisSpeedLimitValue <= 0xFC) {
                setSFCNavigation_DISConstantDISMainSpeedValue(mDisSpeedLimitValue);
            } else {
                // OFF == 0
                setSFCNavigation_DISConstantDISMainSpeedValue(kDisSpeedLimitValueOFF);
            }
        } else {
            // OFF == 0
            setSFCNavigation_DISConstantDISMainSpeedValue(kDisSpeedLimitValueOFF);
        }
    }

    // 5.3.2.2.2.1 형상
    void updateInterDISAddtnlSign() {
        SFCNavigation_DISInter_DISAddtnlSign stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
        // high - 4bit country
        // low - 12bit safety

        if (mIgnElapsedStatus >= IgnElapsed::ON_0ms && mConfigAdasISLA == ConfigAdasISLA::OFF) {
            if (mDisWarnSetStatus == DisWarnSetStatus::ENABLE) {
                stat = calculateInter_DisAddtnlSign();
            } else if (mDisWarnSetStatus == DisWarnSetStatus::DISABLE) {
                // nothing to do
            } else {
                // nothing to do
            }
        } else if (mIgnElapsedStatus >= IgnElapsed::ON_0ms && mConfigAdasISLA == ConfigAdasISLA::ON) {
            stat = calculateInter_DisAddtnlSign();
        } else {
            // nothing to do
        }
        setSFCNavigation_DISInter_DISAddtnlSign(stat);
    }

    inline SFCNavigation_DISInter_DISAddtnlSign findDisAddtnlSignOnMap(
        const ArgumentsDisCameraSafetyAlertStatus::Input_DisCameraSafetyAlertStatus& disCamSafetyAlert,
        const SFCNavigation_DISInter_ConfigNaviDisCountry& naviDisCountry) const {
        SFCNavigation_DISInter_DISAddtnlSign stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
        const size_t isElement = 1;
        if ((disCamSafetyAlert != DisCameraSafetyAlertStatus::OFF &&
             disCamSafetyAlert != DisCameraSafetyAlertStatus::UNHANDLED_TIMEOUT) &&
            (naviDisCountry != SFCNavigation_DISInter_ConfigNaviDisCountry::NONE &&
             naviDisCountry != SFCNavigation_DISInter_ConfigNaviDisCountry::INVALID &&
             naviDisCountry != SFCNavigation_DISInter_ConfigNaviDisCountry::MAX)) {
            DDebug() << "naviDisCountry : " << static_cast<HUInt64>(naviDisCountry);
            HUInt64 countryMask = 0xF000;
            HUInt64 countryStatus = ((static_cast<HUInt64>(naviDisCountry) << 12u) & countryMask);
            DDebug() << "countryStatus << 12 : " << countryStatus;

            HUInt64 safetyMask = 0x0FFF;
            HUInt64 safetyStatus = (static_cast<HUInt64>(disCamSafetyAlert) & safetyMask);
            HUInt64 resValue = countryStatus | safetyStatus;
            DDebug() << "result key : " << resValue;
            if (mCameraSafetyAlertStatSet.count(static_cast<SFCNavigation_DISInter_DISAddtnlSign>(resValue)) == isElement) {
                stat = static_cast<SFCNavigation_DISInter_DISAddtnlSign>(resValue);
            }
        }
        return stat;
    }

    SFCNavigation_DISInter_DISAddtnlSign calculateInter_DisAddtnlSign() {
        SFCNavigation_DISInter_DISAddtnlSign stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
        if (mConfigAdasISLA == ConfigAdasISLA::OFF) {
            if (mDisWarnSetStatus == DisWarnSetStatus::ENABLE) {
                stat = findDisAddtnlSignOnMap(mDisCameraSafetyAlertStatus, mInter_ConfigNaviDisCountry);
                if (stat != SFCNavigation_DISInter_DISAddtnlSign::OFF) {
                    // Noting to do
                } else if (mDisCameraSafetyAlertStatus == DisCameraSafetyAlertStatus::OFF) {
                    stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
                } else if (mInter_ConfigNaviDisCountry == SFCNavigation_DISInter_ConfigNaviDisCountry::INVALID) {
                    stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
                }
            } else if (mDisWarnSetStatus == DisWarnSetStatus::DISABLE) {
                stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
            }
        } else if (mConfigAdasISLA == ConfigAdasISLA::ON) {
            if (mIslaOptionStatus != IslaOptiontStatus::OFF) {
                stat = findDisAddtnlSignOnMap(mDisCameraSafetyAlertStatus, mInter_ConfigNaviDisCountry);
                if (stat != SFCNavigation_DISInter_DISAddtnlSign::OFF) {
                    // Noting to do
                }
            } else if (mIslaOptionStatus == IslaOptiontStatus::OFF) {
                stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
            } else if (mDisCameraSafetyAlertStatus == DisCameraSafetyAlertStatus::OFF) {
                stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
            } else if (mInter_ConfigNaviDisCountry == SFCNavigation_DISInter_ConfigNaviDisCountry::INVALID) {
                stat = SFCNavigation_DISInter_DISAddtnlSign::OFF;
            } else {
                // Noting to do
            }
        }
        return stat;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using ConfigAdasISLA = ArgumentsConfigChanged::Inter_ConfigAdasISLA;
    using HeadUnitNaviStatus = ArgumentsHeadUnitNaviStatus::Input_HeadUnitNaviStatus;
    using DisCameraSafetyAlertStatus = ArgumentsDisCameraSafetyAlertStatus::Input_DisCameraSafetyAlertStatus;
    using IslaOptiontStatus = ArgumentsIslaOptionstatus::Input_IslaOptionStatus;
    using NaviDisplayCountryStatus = ArgumentsNaviDisplayCountryStatus::Input_NaviDisplayCountryStatus;
    using DisWarnSetStatus = ArgumentsDisWarnSetStatusChanged::Inter_DisWarnSetStatus;

    static constexpr HUInt64 kDisSpeedLimitValueOFF = 0;

    IgnElapsed mIgnElapsedStatus = IgnElapsed::OFF_0ms;
    SFCNavigation_DISInter_ConfigNaviDisCountry mInter_ConfigNaviDisCountry = SFCNavigation_DISInter_ConfigNaviDisCountry::NONE;
    ConfigAdasISLA mConfigAdasISLA = ConfigAdasISLA::OFF;
    HeadUnitNaviStatus mHeadUnitNaviStatus = HeadUnitNaviStatus::INVALID;
    DisCameraSafetyAlertStatus mDisCameraSafetyAlertStatus = DisCameraSafetyAlertStatus::OFF;
    IslaOptiontStatus mIslaOptionStatus = IslaOptiontStatus::OFF;
    NaviDisplayCountryStatus mNaviDisplayCountryStatus = NaviDisplayCountryStatus::KOREA;
    HUInt64 mDisSpeedLimitValue = 0;
    DisWarnSetStatus mDisWarnSetStatus = DisWarnSetStatus::NONE;

    std::map<NaviDisplayCountryStatus, SFCNavigation_DISInter_ConfigNaviDisCountry> mNaviDisCountryMap = {
        {NaviDisplayCountryStatus::KOREA, SFCNavigation_DISInter_ConfigNaviDisCountry::KOREA},
        {NaviDisplayCountryStatus::USA, SFCNavigation_DISInter_ConfigNaviDisCountry::USA},
        {NaviDisplayCountryStatus::CANADA, SFCNavigation_DISInter_ConfigNaviDisCountry::CANADA},
        {NaviDisplayCountryStatus::MEXICO, SFCNavigation_DISInter_ConfigNaviDisCountry::MEXICO},
        {NaviDisplayCountryStatus::EUROPE, SFCNavigation_DISInter_ConfigNaviDisCountry::EUROPE},
        {NaviDisplayCountryStatus::MIDDLE_EAST, SFCNavigation_DISInter_ConfigNaviDisCountry::MIDDLE_EAST},
        {NaviDisplayCountryStatus::AUSTRALIA, SFCNavigation_DISInter_ConfigNaviDisCountry::AUSTRALIA},
        {NaviDisplayCountryStatus::SOUTH_AMERICA, SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_AMERICA},
        {NaviDisplayCountryStatus::CHINA, SFCNavigation_DISInter_ConfigNaviDisCountry::CHINA},
        {NaviDisplayCountryStatus::INDIA, SFCNavigation_DISInter_ConfigNaviDisCountry::INDIA},
        {NaviDisplayCountryStatus::JAPAN, SFCNavigation_DISInter_ConfigNaviDisCountry::JAPAN},
        {NaviDisplayCountryStatus::SOUTH_EAST_ASIA_E, SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_EAST_ASIA_E},
        {NaviDisplayCountryStatus::SOUTH_EAST_ASIA_N, SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_EAST_ASIA_N},
        {NaviDisplayCountryStatus::INVALID, SFCNavigation_DISInter_ConfigNaviDisCountry::INVALID}};

    std::map<SFCNavigation_DISInter_ConfigNaviDisCountry, SFCNavigation_DISConstantDISMainSpeedStat> mDisMainSpeedStatMap = {
        {SFCNavigation_DISInter_ConfigNaviDisCountry::KOREA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::MEXICO, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::EUROPE, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::MIDDLE_EAST, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::AUSTRALIA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_AMERICA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::CHINA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::INDIA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::JAPAN, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_EAST_ASIA_E, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::SOUTH_EAST_ASIA_N, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE1},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::USA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE2},
        {SFCNavigation_DISInter_ConfigNaviDisCountry::CANADA, SFCNavigation_DISConstantDISMainSpeedStat::MAIN_TYPE3}};

    std::set<SFCNavigation_DISInter_DISAddtnlSign> mCameraSafetyAlertStatSet = {
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_PORTABLECAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_BUSCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_SCHOOLZONEFIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_SCHOOLZONESPEEL_SIGNALCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_CUTINLINE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ROADSHOULDERVIOLATION,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_TRAFFICINFORMATION,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_INTERSECTIONJAMMING,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_RAMPMETERING,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_LANECHANGE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_BUSLANENOTAVAILABLE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_BUSLANEAVAILABLE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ILLEGALPARKING,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_FALLINGROCKS,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_FOGGYAREA,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_STEEPSLOPE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ROADKILL,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_SPEEDBUMP,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_NARROWROAD,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_HIGHWAYCONSTRUCTIONZONE,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ROADSLIPDETECTION,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_ROADDAMAGEDETECTION,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_FALLING_HAZARD,
        SFCNavigation_DISInter_DISAddtnlSign::KOREA_DROWSY,
        SFCNavigation_DISInter_DISAddtnlSign::USA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::USA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::USA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::USA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::CANADA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::CANADA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::CANADA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::CANADA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::MEXICO_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_PORTABLECAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_AVERAGESPEEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_AVERAGESPEEDCAMERAZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_SPEEDENFORCEMET,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_SPEEDENFORCEMETZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_MOBILEHOTSPOT,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_MOBILEHOTSPOTZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_ACCIDENTBLACKSPOT1,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_ACCIDENTBLACKSPOTZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_DANGERZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_RISKZONE,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_RESTRICTED,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_OTHERS,
        SFCNavigation_DISInter_DISAddtnlSign::EUROPE_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::MIDDLE_EAST_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::MIDDLE_EAST_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::MIDDLE_EAST_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_BUSCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::AUSTRALIA_ROADKILL,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_AMERICA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_AMERICA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_AMERICA_BUSCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_AMERICA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_BUSCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_OTHERS,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_NARROWROAD,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_BLINDSLOPE_CONTINUOUS,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_CROSSWIND,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_HILLSIDEDANGEROUS,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_HUMP_BRIDG,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_JOINT_LEFT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_JOINT_RIGHT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_LOWSPEED,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_NARROW_BOTH,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_NARROWBRIDGE,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_NARROW_LEFT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_NARROW_RIGHT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_ROCKFALL_LEFT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_ROCKFALL_RIGHT,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SLIP,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_BLINDSLOPE_UP,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_BLINDSLOPE_DOWN,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_TUNNEL,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_UNDERWATER,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_UNEVEN,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_UNMANAGED_RAILWAY_CROSS,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_ZIPPASS,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SLOPE_CONTINUOUSDOWN,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SLOPE_UP,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_SLOPE_DOWN,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_RAILWAY_MANAGED,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_VILLAGE,
        SFCNavigation_DISInter_DISAddtnlSign::CHINA_AUDIBLEWARNING,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_NARROWROAD,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_ZONE_30,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_STOP,
        SFCNavigation_DISInter_DISAddtnlSign::JAPAN_LANE_MOVE_REGULATION,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_DONOTENTER,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_FALLINGROCKS,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_SPEEDBUMP,
        SFCNavigation_DISInter_DISAddtnlSign::INDIA_OVERTAKING_LANE,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_FALLINGROCKS,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_STOP,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_E_OVERTAKING_LANE,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_FIXEDCAMERA,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_SPEED_SIGNAL,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_ACCIDENTBLACKSPOT2,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_SCHOOLZONE,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_FALLINGROCKS,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_RAILROADCROSSING,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_STOP,
        SFCNavigation_DISInter_DISAddtnlSign::SOUTH_EAST_ASIA_N_OVERTAKING_LANE};
};

}  // namespace ccos

#endif  // SFSS_Navigation_DIS_H
