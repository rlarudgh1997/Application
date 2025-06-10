/**
 * @file Detail_Contents_ISLA_DIS_NEW.hpp
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
#ifndef SFSS_Detail_Contents_ISLA_DIS_NEW_H
#define SFSS_Detail_Contents_ISLA_DIS_NEW_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_ISLA_DIS_NEWBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 29.0.0
// Reference : [HD180] Detail_Contents_ISLA_DIS_NEW. Contains : Constant. Recommend: VALUE_CHANGED
class Detail_Contents_ISLA_DIS_NEW : public Detail_Contents_ISLA_DIS_NEWBase {
public:
    Detail_Contents_ISLA_DIS_NEW() = default;
    ~Detail_Contents_ISLA_DIS_NEW() override = default;
    Detail_Contents_ISLA_DIS_NEW(const Detail_Contents_ISLA_DIS_NEW& other) = delete;
    Detail_Contents_ISLA_DIS_NEW(Detail_Contents_ISLA_DIS_NEW&& other) noexcept = delete;
    Detail_Contents_ISLA_DIS_NEW& operator=(const Detail_Contents_ISLA_DIS_NEW& other) = delete;
    Detail_Contents_ISLA_DIS_NEW& operator=(Detail_Contents_ISLA_DIS_NEW&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerFinalAddtnlSign.create(kTimerAddtnSignInterval300ms, this,
                                                &Detail_Contents_ISLA_DIS_NEW::onOneShotTimerFinalAddtnlSign, false) == false) {
            DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalAddtnlSign Create Failed";
        }

        if (mOneShotTimerFinalMainSpeed.create(kTimerMainSpeedInterval300ms, this,
                                               &Detail_Contents_ISLA_DIS_NEW::onOneShotTimerFinalMainSpeed, false) == false) {
            DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalMainSpeed Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateAll();
    }

    void onHudCameraSafetyAlertChanged(const ArgumentsHudCameraSafetyAlertChanged& args) {
        mDisCameraSafetyAlertStatus = args.mInput_DisCameraSafetyAlertStatus;
        if (ISTIMEOUT(args.mInput_DisCameraSafetyAlertStatus) == true) {
            mDisCameraSafetyAlertStatus = DisCameraSafetyAlertStatus::UNHANDLED_TIMEOUT;
        }
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalAddtnlSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
    }

    void onHudNaviWarnDistChanged(const ArgumentsHudNaviWarnDistChanged& args) {
        mNaviWarnDistUnitStatus = args.mInput_NaviWarnDistUnitStatus;
        if (ISTIMEOUT(args.mInput_NaviWarnDistUnitStatus) == true) {
            mNaviWarnDistUnitStatus = NaviWarnDistUnitStatus::UNHANDLED_TIMEOUT;
        }
        mNaviWarnDistOverDecimalValue = args.mInput_NaviWarnDistOverDecimalValue;
        mNaviWarnDistUnderDecimalValue = args.mInput_NaviWarnDistUnderDecimalValue;
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
    }

    void onHeadUnitNaviStatus(const ArgumentsHeadUnitNaviStatus& args) {
        mHeadUnitNaviStatus = args.mInput_HeadUnitNaviStatus;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateDISMainSpeedStat();
        updatePrivateDISMainSpeedValue();
        updateFinalMainSpeed();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onDisSpeedLimitValue(const ArgumentsDisSpeedLimitValue& args) {
        mDisSpeedLimitValue = args.mInput_DisSpeedLimitValue;
        updatePrivateDISMainSpeedValue();
        updateFinalMainSpeed();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onInterHudDISInfoOnOffStatusChanged(const ArgumentsInterHudDISInfoOnOffStatusChanged& args) {
        if (updateValueIfValid(mHudDISInfoOnOffStatus, args.mInter_HudDISInfoOnOffStatus) == true) {
            updatePrivateISLAMainSpeedForHUD();
            updatePrivateDISMainSpeedStat();
            updatePrivateISLAAdditionalSign();
            updatePrivateDISAddtnlSignForHUD();
            updateFinalMainSpeed();
            updateFinalAddtnlSignStat();
            updateConstantISLAAddSignStat();
            updateConstantISLAAddSign2Stat();
            updateConstantISLANextSupportTrafficSignStat();
            updateHUDNaviWarnDistUnitStat();
            updateHUDNaviWarnDistValue();
            updateNaviRangeCamAvgSpeedValue();
            updateNaviRangeCamAvgSpeedStat();
        }
    }

    void onIslaOptionStatusChanged(const ArgumentsIslaOptionStatusChanged& args) {
        mIslaOptionStatus = args.mInput_IslaOptionStatus;
        updatePrivateDISAddtnlSignForHUD();
        updateFinalAddtnlSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
        updateNaviRangeCamAvgSpeedValue();
        updateNaviRangeCamAvgSpeedStat();
    }

    void onSgnISLAMainSpeedForHUDStatChanged(const ArgumentsSgnISLAMainSpeedForHUDStatChanged& args) {
        mIslaSpeedLimitTrafficSignStatus = args.mInput_IslaSpeedLimitTrafficSignStatus;
        mIslaSpeedLimitTrafficSignValue = args.mInput_IslaSpeedLimitTrafficSignValue;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateISLAMainSpeedValue();
        updateFinalMainSpeed();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onSgnIslaTrafficSignCountryInformationStatusChanged(
        const ArgumentsSgnIslaTrafficSignCountryInformationStatusChanged& args) {
        mIslaTrafficSignCountryInformationStatus = args.mInput_IslaTrafficSignCountryInformationStatus;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalMainSpeed();
        updateFinalAddtnlSignStat();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
    }

    void onConfigNewAdasISLAChanged(const ArgumentsConfigNewAdasISLAChanged& args) {
        mConfigAdasISLA = args.mInter_ConfigAdasISLA;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateDISMainSpeedStat();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalMainSpeed();
        updateFinalAddtnlSignStat();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateNaviRangeCamAvgSpeedValue();
        updateNaviRangeCamAvgSpeedStat();
    }

    void onSgnIslaAdditionalTrafficSignStatusChanged(const ArgumentsSgnIslaAdditionalTrafficSignStatusChanged& args) {
        mIslaAdditionalTrafficSignStatus = args.mInput_IslaAdditionalTrafficSignStatus;
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalAddtnlSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
    }

    void onConfigNaviDisCountryChanged(const ArgumentsConfigNaviDisCountryChanged& args) {
        mConfigNaviDisCountry = args.mInter_ConfigNaviDisCountry;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateDISMainSpeedStat();
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalMainSpeed();
        updateFinalAddtnlSignStat();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
    }

    void onSgnIslaSupplementaryTrafficSignStatusChanged(const ArgumentsSgnIslaSupplementaryTrafficSignStatusChanged& args) {
        mIslaSupplementaryTrafficSignStatus = args.mInput_IslaSupplementaryTrafficSignStatus;
        updateConstantISLAAddSignStat();
    }

    void onUnitChanged(const ArgumentsUnitChanged& args) {
        mDisplaySpeedUnit = args.mInter_DisplaySpeedUnit;
        mNaviOverSpeedUnit = args.mInput_NaviOverSpdUnit;
        updateNaviRangeCamAvgSpeedStat();
    }

    void onNaviOverSpeedColorChanged(const ArgumentsNaviOverSpeedColorChanged& args) {
        mNaviOverSpeedColor1 = static_cast<HUInt32>(args.mInput_NaviOverSpeedColor1);  // Intended Casting
        mNaviOverSpeedColor2 = static_cast<HUInt32>(args.mInput_NaviOverSpeedColor2);  // Intended Casting
        updateNaviRangeCamAvgSpeedStat();
    }

    void onNaviOverSpeedRangeChanged(const ArgumentsNaviOverSpeedRangeChanged& args) {
        mNaviOverSpeedRange1 = args.mInput_NaviOverSpeedRange1;
        mNaviOverSpeedRange2 = args.mInput_NaviOverSpeedRange2;
        mNaviRangeCamAvgSpeed = args.mInput_NaviRangeCamAvgSpdValue;
        updateNaviRangeCamAvgSpeedValue();
        updateNaviRangeCamAvgSpeedStat();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        updateAll();
    }

    void onIslaNextSpeedLimitTrafficSignStatusChanged(const ArgumentsIslaNextSpeedLimitTrafficSignStatusChanged& args) {
        mIslaNextSpeedLimitTrafficSignStatus = args.mInput_IslaNextSpeedLimitTrafficSignStatus;
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
    }

    void onIslaNextSpeedLimitTrafficSignValueChanged(const ArgumentsIslaNextSpeedLimitTrafficSignValueChanged& args) {
        mIslaNextSpeedLimitTrafficSignValue = args.mInput_IslaNextSpeedLimitTrafficSignValue;
        updateConstantISLANextSpeedLimitTrafficSignStat();
    }

    void onIslaNextSupportTrafficSignValueChanged(const ArgumentsIslaNextSupportTrafficSignValueChanged& args) {
        mIslaNextSupportTrafficSignValue = args.mInput_IslaNextSupportTrafficSignValue;
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onIslaSupplementaryTrafficSign2StatusChanged(const ArgumentsIslaSupplementaryTrafficSign2StatusChanged& args) {
        mIslaSupplementaryTrafficSign2Status = args.mInput_IslaSupplementaryTrafficSign2Status;
        updateConstantISLAAddSign2Stat();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onSgnINDIslaInfoChanged(const ArgumentsSgnINDIslaInfoChanged& args) {
        mINDIslaAdditionalTrafficSignStatus = args.mInput_INDIslaAdditionalTrafficSignStatus;
        mINDIslaNextSpeedLimitTrafficSignStatus = args.mInput_INDIslaNextSpeedLimitTrafficSignStatus;
        mINDIslaNextSpeedLimitTrafficSignValue = args.mInput_INDIslaNextSpeedLimitTrafficSignValue;
        mINDIslaNextSupportTrafficSignValue = args.mInput_INDIslaNextSupportTrafficSignValue;
        mINDIslaSpeedLimitTrafficSignStatus = args.mInput_INDIslaSpeedLimitTrafficSignStatus;
        mINDIslaSpeedLimitTrafficSignValue = args.mInput_INDIslaSpeedLimitTrafficSignValue;
        mINDIslaSupplementaryTrafficSign2Status = args.mInput_INDIslaSupplementaryTrafficSign2Status;
        mINDIslaSupplementaryTrafficSignStatus = args.mInput_INDIslaSupplementaryTrafficSignStatus;
        mINDIslaTrafficSignCountryInformationStatus = args.mInput_INDIslaTrafficSignCountryInformationStatus;
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateISLAMainSpeedValue();
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalMainSpeed();
        updateFinalAddtnlSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateConstantISLANextSpeedLimitTrafficSignStat();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    inline HBool isIslaDisNewOn() const {
        return mIsIgnOn && (mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON);
    }

    inline HBool isSignDisplay() const {
        HBool bIslaOffUsmHudDisEnable =
            (mConfigAdasISLA == ConfigAdasISLA::OFF) && (mHudDISInfoOnOffStatus == HudDISInfoOnOffStatus::ENABLE);
        HBool bIslaOnUsmIslaEnable = (mConfigAdasISLA == ConfigAdasISLA::ON) && (mIslaOptionStatus != IslaOptionStatus::OFF);

        return (bIslaOffUsmHudDisEnable == true || bIslaOnUsmIslaEnable == true);
    }

    void updateAll() {
        updatePrivateISLAMainSpeedForHUD();
        updatePrivateISLAMainSpeedValue();
        updatePrivateDISMainSpeedStat();
        updatePrivateDISMainSpeedValue();
        updatePrivateISLAAdditionalSign();
        updatePrivateDISAddtnlSignForHUD();
        updateFinalMainSpeed();
        updateFinalAddtnlSignStat();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateConstantISLANextSpeedLimitTrafficSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
        updateNaviRangeCamAvgSpeedValue();
        updateNaviRangeCamAvgSpeedStat();
    }

    void updateFinalMainSpeed() {
        // 5.1.1.1 ISLA/내비 속도제한 표지판 표시판단
        if (isIslaDisNewOn() == false) {
            return;
        }
        SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::OFF;
        SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat valueStat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat::OFF;

        if (mOneShotTimerFinalMainSpeed.isRunning() == false) {
            if (mConfigAdasISLA == ConfigAdasISLA::ON) {
                if (mPrivateISLAMainSpeedForHUDStat != Private_ISLAMainSpeedForHUDStat::OFF) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_ISLAMAINSPEEDFORHUD_STAT;
                    setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatIslaMainSpeedForHudStatValue(
                        mPrivateISLAMainSpeedForHUDStat);
                    if (mPrevPrivateISLAMainSpeedForHUDStat != mPrivateISLAMainSpeedForHUDStat) {
                        mPrevPrivateISLAMainSpeedForHUDStat = mPrivateISLAMainSpeedForHUDStat;
                        if (mOneShotTimerFinalMainSpeed.restart() == false) {
                            DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalMainSpeed Restart Failed";
                        }
                    }
                }

                if (mPrivateISLAMainSpeedValueStat == Private_ISLAMainSpeedValueStat::VALUE) {
                    valueStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat::VALUE;
                } else if (mPrivateISLAMainSpeedValueStat == Private_ISLAMainSpeedValueStat::INVALID) {
                    valueStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat::INVALID;
                } else {
                    // OFF
                }

                setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueIslaMainSpeedValue(mPrivateISLAMainSpeedValue);
            } else {
                if (mHudDISInfoOnOffStatus == HudDISInfoOnOffStatus::ENABLE) {
                    if (mPrivateDISMainSpeedStat != Private_DISMainSpeedStat::OFF) {
                        stat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_DISMAINSPEED_STAT;
                        setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatDisMainSpeedStatValue(
                            mPrivateDISMainSpeedStat);
                        if (mPrevPrivateDISMainSpeedStat != mPrivateDISMainSpeedStat) {
                            mPrevPrivateDISMainSpeedStat = mPrivateDISMainSpeedStat;
                            if (mOneShotTimerFinalMainSpeed.restart() == false) {
                                DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalMainSpeed Restart Failed";
                            }
                        }

                        if (mPrivateDISMainSpeedValueStat != Private_DISMainSpeedValueStat::OFF) {
                            valueStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat::VALUE;
                        }

                        setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueDisMainSpeedValue(mPrivateDISMainSpeedValue);
                    }
                }
            }
            mFinalMainSpeedStat = stat;
            setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat(stat);
            setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat(valueStat);
        }
    }

    void onOneShotTimerFinalMainSpeed() {
        mOneShotTimerFinalMainSpeed.stop();
        updateFinalMainSpeed();
        updateConstantISLAAddSignStat();
        updateConstantISLAAddSign2Stat();
        updateConstantISLANextSupportTrafficSignStat();
        flush();
    }

    // 5.1.1.2 ISLA 속도 제한 표지판
    void updatePrivateISLAMainSpeedForHUD() {
        // 5.1.1.2.1 형상
        if (isIslaDisNewOn() == false) {
            return;
        }
        Private_ISLAMainSpeedForHUDStat stat = Private_ISLAMainSpeedForHUDStat::OFF;

        if (mConfigAdasISLA == ConfigAdasISLA::ON) {
            if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::BLACK_TEXT ||
                mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::BLACK_TEXT) {
                if (mIslaSpeedLimitTrafficSignValue == 253 || mINDIslaSpeedLimitTrafficSignValue == 253) {
                    stat = Private_ISLAMainSpeedForHUDStat::UNLIMITED;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE3_VALID;
                    } else if (mIslaSpeedLimitTrafficSignValue == 0 && mINDIslaSpeedLimitTrafficSignValue == 0) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE3_INVALID;
                    } else {
                        // OFF
                    }
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE4_VALID;
                    } else if (mIslaSpeedLimitTrafficSignValue == 0 && mINDIslaSpeedLimitTrafficSignValue == 0) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE4_INVALID;
                    } else {
                        // OFF
                    }
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE5_VALID;
                    } else if (mIslaSpeedLimitTrafficSignValue == 0 && mINDIslaSpeedLimitTrafficSignValue == 0) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE5_INVALID;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::RED_TEXT ||
                       mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::RED_TEXT) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE6_VALID;
                    }
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE7_VALID;
                    }
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    if ((1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) ||
                        (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252)) {
                        stat = Private_ISLAMainSpeedForHUDStat::TYPE8_VALID;
                    }
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }
        mPrevPrivateISLAMainSpeedForHUDStat = mPrivateISLAMainSpeedForHUDStat;
        mPrivateISLAMainSpeedForHUDStat = stat;
    }

    void updatePrivateISLAMainSpeedValue() {
        // 5.1.1.2.2 ISLA 제한속도
        if (isIslaDisNewOn() == false) {
            return;
        }

        mPrevPrivateISLAMainSpeedValueStat = mPrivateISLAMainSpeedValueStat;
        if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
            mPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::VALUE;
            mPrivateISLAMainSpeedValue = mIslaSpeedLimitTrafficSignValue;
        } else if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
            mPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::VALUE;
            mPrivateISLAMainSpeedValue = mINDIslaSpeedLimitTrafficSignValue;
        } else if (mIslaSpeedLimitTrafficSignValue == 0 && mINDIslaSpeedLimitTrafficSignValue == 0) {
            mPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::INVALID;
            mPrivateISLAMainSpeedValue = 0;
        } else {
            mPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::OFF;
            mPrivateISLAMainSpeedValue = 0;
        }
    }

    // 5.1.1.3 ISLA 사전 속도 제한 표지판
    void updateConstantISLANextSpeedLimitTrafficSignTypeStat() {
        // 5.1.1.3.1 형상
        if (mConfigHUD != ConfigHUD::ON && mConfigNewHUD != ConfigNewHUD::ON) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasISLA == ConfigAdasISLA::ON) {
            if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY ||
                 mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE1;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY ||
                        mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE2;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY ||
                        mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE3;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY ||
                        mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE1;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY ||
                        mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE2;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY ||
                        mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE3;
            } else {
                // OFF
            }
        }
        mCurrentISLANextSpeedLimitTrafficSignStat = stat;
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat(stat);
        updateConstantISLANextSupportTrafficSignStat();
    }

    void updateConstantISLANextSpeedLimitTrafficSignStat() {
        // 5.1.1.3.2 ISLA 사전 제한 속도
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (1 <= mIslaNextSpeedLimitTrafficSignValue && mIslaNextSpeedLimitTrafficSignValue <= 252) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat::VALUE;
                value = mIslaNextSpeedLimitTrafficSignValue;
            } else if (1 <= mINDIslaNextSpeedLimitTrafficSignValue && mINDIslaNextSpeedLimitTrafficSignValue <= 252) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat::VALUE;
                value = mINDIslaNextSpeedLimitTrafficSignValue;
            } else if (mIslaNextSpeedLimitTrafficSignValue == 0 && mINDIslaNextSpeedLimitTrafficSignValue == 0) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat::INVALID;
            } else {
                // OFF
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignStat(stat);
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignValue(value);
    }

    // 5.1.1.4 내비 속도 제한 표지판
    void updatePrivateDISMainSpeedStat() {
        // 5.1.1.4.1 형상
        if (isIslaDisNewOn() == false) {
            return;
        }

        Private_DISMainSpeedStat stat = Private_DISMainSpeedStat::OFF;

        if (mConfigAdasISLA == ConfigAdasISLA::OFF) {
            if (mHudDISInfoOnOffStatus == HudDISInfoOnOffStatus::ENABLE) {
                if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NORMAL) {
                    switch (mConfigNaviDisCountry) {
                        case ConfigNaviDisCountry::KOREA:
                        case ConfigNaviDisCountry::MEXICO:
                        case ConfigNaviDisCountry::EUROPE:
                        case ConfigNaviDisCountry::MIDDLE_EAST:
                        case ConfigNaviDisCountry::AUSTRALIA:
                        case ConfigNaviDisCountry::SOUTH_AMERICA:
                        case ConfigNaviDisCountry::CHINA:
                        case ConfigNaviDisCountry::INDIA:
                        case ConfigNaviDisCountry::JAPAN:
                        case ConfigNaviDisCountry::SOUTH_EAST_ASIA_E:
                        case ConfigNaviDisCountry::SOUTH_EAST_ASIA_N:
                            stat = Private_DISMainSpeedStat::MAIN_TYPE1;
                            break;
                        case ConfigNaviDisCountry::USA:
                            stat = Private_DISMainSpeedStat::MAIN_TYPE2;
                            break;
                        case ConfigNaviDisCountry::CANADA:
                            stat = Private_DISMainSpeedStat::MAIN_TYPE3;
                            break;
                        default:
                            stat = Private_DISMainSpeedStat::OFF;
                            break;
                    }
                }
            }
        }

        mPrevPrivateDISMainSpeedStat = mPrivateDISMainSpeedStat;
        mPrivateDISMainSpeedStat = stat;
    }

    void updatePrivateDISMainSpeedValue() {
        // 5.1.1.4.2 내비 제한 속도
        if (isIslaDisNewOn() == false) {
            return;
        }

        mPrevPrivateDISMainSpeedValueStat = mPrivateDISMainSpeedValueStat;
        if (mHeadUnitNaviStatus == HeadUnitNaviStatus::NORMAL) {
            if (0x1 <= mDisSpeedLimitValue && mDisSpeedLimitValue <= 0xFC) {
                mPrivateDISMainSpeedValueStat = Private_DISMainSpeedValueStat::VALUE;
                mPrivateDISMainSpeedValue = mDisSpeedLimitValue;
            } else {
                mPrivateDISMainSpeedValueStat = Private_DISMainSpeedValueStat::OFF;
                mPrivateDISMainSpeedValue = 0;
            }
        } else {
            mPrivateDISMainSpeedValueStat = Private_DISMainSpeedValueStat::OFF;
            mPrivateDISMainSpeedValue = 0;
        }
    }

    void updateConstantISLAAddSignStat() {
        // 5.1.2 ISLA 보조 표지판1
        if (isIslaDisNewOn() == false) {
            return;
        }
        SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat::OFF;

        if (mFinalMainSpeedStat ==
            SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_ISLAMAINSPEEDFORHUD_STAT) {
            if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::RAIN ||
                mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::RAIN) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat::RAIN;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::SNOW ||
                       mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::SNOW) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat::SNOW;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::SNOW_RAIN ||
                       mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::SNOW_RAIN) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat::SNOW_RAIN;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::TRAILER ||
                       mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::TRAILER) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat::TRAILER;
            } else {
                // OFF
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSignStat(stat);
    }

    void updateConstantISLAAddSign2Stat() {
        // 5.1.3 ISLA 보조 표지판2
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat::OFF;
        if (mFinalMainSpeedStat ==
            SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_ISLAMAINSPEEDFORHUD_STAT) {
            if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::RAIN ||
                mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::RAIN) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat::RAIN;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::SNOW ||
                       mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::SNOW) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat::SNOW;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::SNOW_RAIN ||
                       mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::SNOW_RAIN) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat::SNOW_RAIN;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::TRAILER ||
                       mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::TRAILER) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat::TRAILER;
            } else {
                // OFF
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLAAddSign2Stat(stat);
    }

    // 5.1.4 ISLA 사전 제한 속도 보조 표지판
    void updateConstantISLANextSupportTrafficSignUnitStat() {
        // 5.1.4.1 형상
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignUnitStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignUnitStat::OFF;
        if (mIsIgnOn == true &&
            mFinalMainSpeedStat ==
                SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_ISLAMAINSPEEDFORHUD_STAT &&
            mCurrentIslaNextSupportTrafficSignValue !=
                SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::OFF) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignUnitStat::METER;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignUnitStat::FEET;
            } else {
                // OFF
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignUnitStat(stat);
    }

    void updateConstantISLANextSupportTrafficSignStat() {
        // 5.1.4.2 거리
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true &&
            mFinalMainSpeedStat ==
                SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::PRIVATE_ISLAMAINSPEEDFORHUD_STAT &&
            mCurrentISLANextSpeedLimitTrafficSignStat !=
                SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::OFF) {
            if (1 <= mIslaNextSupportTrafficSignValue && mIslaNextSupportTrafficSignValue <= 50) {
                if (mTripDistanceUnit == TripDistanceUnit::KM) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::VALUE;
                    value = mIslaNextSupportTrafficSignValue * 10;
                } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::VALUE;
                    value = mIslaNextSupportTrafficSignValue * 30;
                } else {
                    // OFF
                }
            } else if (1 <= mINDIslaNextSupportTrafficSignValue && mINDIslaNextSupportTrafficSignValue <= 50) {
                if (mTripDistanceUnit == TripDistanceUnit::KM) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::VALUE;
                    value = mINDIslaNextSupportTrafficSignValue * 10;
                } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::VALUE;
                    value = mINDIslaNextSupportTrafficSignValue * 30;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }
        mCurrentIslaNextSupportTrafficSignValue = stat;
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat(stat);
        setSFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignValue(value);
        updateConstantISLANextSupportTrafficSignUnitStat();
    }

    void updateFinalAddtnlSignStat() {
        // 5.1.5.1 ISLA/내비 부가표지판 표시판단
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::OFF;

        if (mOneShotTimerFinalAddtnlSign.isRunning() == false) {
            if (mPrivateISLAAddtnlSign != Private_ISLAAddtnlSign::OFF) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::PRIVATE_ISLAADDTNLSIGN;
                setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignISLAAddtnlSignValue(mPrivateISLAAddtnlSign);
                if (mPrevPrivateISLAAddtnlSign != mPrivateISLAAddtnlSign) {
                    mPrevPrivateISLAAddtnlSign = mPrivateISLAAddtnlSign;
                    restartTimerFinalAddtnlSign();
                }
            } else if (mPrivateISLAAddtnlSign == Private_ISLAAddtnlSign::OFF &&
                       mPrivateDISAddtnlSignForHUD != Private_DISAddtnlSignForHUD::OFF) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::PRIVATE_DISADDTNLSIGN;
                setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignDISAddtnlSignForHUDValue(mPrivateDISAddtnlSignForHUD);
                if (mPrevPrivateDISAddtnlSignForHUD != mPrivateDISAddtnlSignForHUD) {
                    mPrevPrivateDISAddtnlSignForHUD = mPrivateDISAddtnlSignForHUD;
                    restartTimerFinalAddtnlSign();
                }
            } else {
                // OFF
                setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignISLAAddtnlSignValue(mPrivateISLAAddtnlSign);
                setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignDISAddtnlSignForHUDValue(mPrivateDISAddtnlSignForHUD);
            }
            mFinalAddtnlStat = stat;
            setSFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat(stat);
        }
    }

    void restartTimerFinalAddtnlSign() {
        if (mOneShotTimerFinalAddtnlSign.isRunning() == true) {
            if (mOneShotTimerFinalAddtnlSign.restart() == false) {
                DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalAddtnlSign Start Failed";
            }
        } else {
            if (mOneShotTimerFinalAddtnlSign.start() == false) {
                DWarning() << "[Detail_Contents_ISLA_DIS_NEW] mOneShotTimerFinalAddtnlSign Start Failed";
            }
        }
    }

    void onOneShotTimerFinalAddtnlSign() {
        mOneShotTimerFinalAddtnlSign.stop();
        updateFinalAddtnlSignStat();
        updateHUDNaviWarnDistUnitStat();
        updateHUDNaviWarnDistValue();
        flush();
    }

    void updatePrivateISLAAdditionalSign() {
        // 5.1.5.2 ISLA 부가 표지판
        if (isIslaDisNewOn() == false) {
            return;
        }
        Private_ISLAAddtnlSign stat = Private_ISLAAddtnlSign::OFF;

        const TrafficSignStatusWithIslaAddtnlSignMap::const_iterator& signMapIter =
            mTrafficSignStatusWithIslaAddtnlSignMap.find(mIslaAdditionalTrafficSignStatus);
        const INDTrafficSignStatusWithIslaAddtnlSignMap::const_iterator& signMapIterIND =
            mINDTrafficSignStatusWithIslaAddtnlSignMap.find(mINDIslaAdditionalTrafficSignStatus);
        if (signMapIter != mTrafficSignStatusWithIslaAddtnlSignMap.end()) {
            const AdditionalSignMap& trafficMap = signMapIter->second;
            const AdditionalSignMap::const_iterator& islaaddtnlsignIter =
                trafficMap.find(mIslaTrafficSignCountryInformationStatus);
            if (islaaddtnlsignIter != trafficMap.end()) {
                stat = islaaddtnlsignIter->second;
            }
        } else if (signMapIterIND != mINDTrafficSignStatusWithIslaAddtnlSignMap.end()) {
            const AdditionalSignMapIND& trafficMapIND = signMapIterIND->second;
            const AdditionalSignMapIND::const_iterator& islaaddtnlsignIterIND =
                trafficMapIND.find(mINDIslaTrafficSignCountryInformationStatus);
            if (islaaddtnlsignIterIND != trafficMapIND.end()) {
                stat = islaaddtnlsignIterIND->second;
            }
        } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_ENTER ||
                   mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::DO_NOT_ENTER) {
            stat = Private_ISLAAddtnlSign::DO_NOT_ENTER;
        } else {
            // OFF
        }

        mPrevPrivateISLAAddtnlSign = mPrivateISLAAddtnlSign;
        mPrivateISLAAddtnlSign = stat;
    }

    void updatePrivateDISAddtnlSignForHUD() {
        const size_t isElement = 1;

        // 5.1.5.3.1 형상
        if (isIslaDisNewOn() == false) {
            return;
        }
        Private_DISAddtnlSignForHUD disAddtnlSign = Private_DISAddtnlSignForHUD::OFF;

        // high - 4bit country
        // low - 12bit safety
        if (isSignDisplay() == true) {
            if (mConfigNaviDisCountry != ConfigNaviDisCountry::NONE && mConfigNaviDisCountry != ConfigNaviDisCountry::INVALID &&
                mConfigNaviDisCountry != ConfigNaviDisCountry::MAX) {
                HUInt64 countryMask = 0xF000;
                HUInt64 countryStatus = ((static_cast<HUInt64>(mConfigNaviDisCountry) << 12u) & countryMask);

                if (mDisCameraSafetyAlertStatus != DisCameraSafetyAlertStatus::OFF &&
                    mDisCameraSafetyAlertStatus != DisCameraSafetyAlertStatus::UNHANDLED_TIMEOUT) {
                    HUInt64 safetyMask = 0x0FFF;
                    HUInt64 safetyStatus = (static_cast<HUInt64>(mDisCameraSafetyAlertStatus) & safetyMask);
                    HUInt64 resValue = countryStatus | safetyStatus;
                    if (mCameraSafetyAlertStatSet.count(static_cast<Private_DISAddtnlSignForHUD>(resValue)) == isElement) {
                        disAddtnlSign = static_cast<Private_DISAddtnlSignForHUD>(resValue);
                    }
                }
            }
        }

        mPrevPrivateDISAddtnlSignForHUD = mPrivateDISAddtnlSignForHUD;
        mPrivateDISAddtnlSignForHUD = disAddtnlSign;
    }

    void updateHUDNaviWarnDistValue() {
        // 5.1.5.3.2 카메라까지 단속 잔여 거리 및 단위 (잔여 거리 표시)
        if (isIslaDisNewOn() == false) {
            return;
        }
        SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat::OFF;

        if (mFinalAddtnlStat == SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::PRIVATE_DISADDTNLSIGN) {
            if (mNaviWarnDistOverDecimalValue != 0xFFFE && mNaviWarnDistOverDecimalValue != 0xFFFF) {
                if (mNaviWarnDistUnitStatus == NaviWarnDistUnitStatus::METER ||
                    mNaviWarnDistUnitStatus == NaviWarnDistUnitStatus::FEET ||
                    mNaviWarnDistUnitStatus == NaviWarnDistUnitStatus::YARD) {
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat::VALUE;
                    setSFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistValue(
                        static_cast<HDouble>(mNaviWarnDistOverDecimalValue));
                } else if (mNaviWarnDistUnitStatus == NaviWarnDistUnitStatus::KILOMETER ||
                           mNaviWarnDistUnitStatus == NaviWarnDistUnitStatus::MILE) {
                    if (mNaviWarnDistOverDecimalValue < kOverDecimalValueNaviWarnDist100) {
                        HDouble calculatedValue = static_cast<HDouble>(mNaviWarnDistOverDecimalValue);
                        if (mNaviWarnDistUnderDecimalValue < kUnderDecimalValueNaviWarnDist10) {
                            calculatedValue += static_cast<HDouble>(mNaviWarnDistUnderDecimalValue) * 0.1;
                        }
                        stat = SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat::VALUE;
                        setSFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistValue(calculatedValue);
                    } else if (mNaviWarnDistOverDecimalValue >= kOverDecimalValueNaviWarnDist100 &&
                               mNaviWarnDistOverDecimalValue < 0xFFFE) {
                        stat = SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat::VALUE;
                        setSFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistValue(
                            static_cast<HDouble>(mNaviWarnDistOverDecimalValue));
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistStat(stat);
    }

    void updateHUDNaviWarnDistUnitStat() {
        // 5.1.5.3.2 카메라까지 단속 잔여 거리 및 단위 (단위 표시)
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::OFF;

        if (mFinalAddtnlStat == SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::PRIVATE_DISADDTNLSIGN) {
            switch (mNaviWarnDistUnitStatus) {
                case NaviWarnDistUnitStatus::METER:
                case NaviWarnDistUnitStatus::KILOMETER:
                case NaviWarnDistUnitStatus::MILE:
                case NaviWarnDistUnitStatus::FEET:
                case NaviWarnDistUnitStatus::YARD:
                    stat = mNaviWarnDistUnitStatusMap[mNaviWarnDistUnitStatus];
                    break;
                default:
                    stat = SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::OFF;
                    break;
            }
        }
        setSFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat(stat);
    }

    // 5.1.6 구간 단속 표지판
    void updateNaviRangeCamAvgSpeedValue() {
        // 5.1.6.1 평균속도 값
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdStat stat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdStat::OFF;
        HUInt64 value = 0;

        if (isSignDisplay() == true) {
            if (1 <= mNaviRangeCamAvgSpeed && mNaviRangeCamAvgSpeed <= 254) {
                stat = SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdStat::VALUE;
                value = mNaviRangeCamAvgSpeed;
            }
        }

        setSFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdStat(stat);
        setSFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdValue(value);
    }

    void updateNaviRangeCamAvgSpeedStat() {
        // 5.1.6.2 평균속도 색상
        if (isIslaDisNewOn() == false) {
            return;
        }

        SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat colorStat =
            SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat::DEFAULT_COLOR;
        HUInt32 colorValue = 0;

        if ((mDisplaySpeedUnit == DisplaySpeedUnit::KM_PER_HOUR && mNaviOverSpeedUnit == NaviOverSpeedUnit::KILOMETER) ||
            (mDisplaySpeedUnit == DisplaySpeedUnit::MILE_PER_HOUR && mNaviOverSpeedUnit == NaviOverSpeedUnit::MILE)) {
            if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange1 && mNaviOverSpeedRange1 <= kNaviOverSpeedRangeMax) &&
                (mNaviOverSpeedRange1 > mNaviRangeCamAvgSpeed)) {
                colorStat = SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat::DEFAULT_COLOR;
            } else if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange1 && mNaviOverSpeedRange1 <= kNaviOverSpeedRangeMax) &&
                       (kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                       (kNaviOverSpeedColorMin <= mNaviOverSpeedColor1 && mNaviOverSpeedColor1 <= kNaviOverSpeedColorMax) &&
                       (mNaviOverSpeedRange1 <= mNaviRangeCamAvgSpeed) && (mNaviRangeCamAvgSpeed < mNaviOverSpeedRange2)) {
                colorStat = SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat::VALUE;
                colorValue = mNaviOverSpeedColor1;
            } else if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                       (kNaviOverSpeedColorMin <= mNaviOverSpeedColor2 && mNaviOverSpeedColor2 <= kNaviOverSpeedColorMax) &&
                       (mNaviOverSpeedRange2 <= mNaviRangeCamAvgSpeed)) {
                colorStat = SFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat::VALUE;
                colorValue = mNaviOverSpeedColor2;
            } else {
                // DEFAULT_COLOR
            }
        }

        setSFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorStat(colorStat);
        setSFCDetail_Contents_ISLA_DIS_NEWConstantNaviRangeCamAvgSpdColorValue(colorValue);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using Private_ISLAMainSpeedForHUDStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatIslaMainSpeedForHudStatValue;
    using Private_DISMainSpeedStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatDisMainSpeedStatValue;
    using Private_ISLAAddtnlSign = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignISLAAddtnlSignValue;
    using Private_DISAddtnlSignForHUD = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignDISAddtnlSignForHUDValue;
    using DisCameraSafetyAlertStatus = ArgumentsHudCameraSafetyAlertChanged::Input_DisCameraSafetyAlertStatus;
    using NaviWarnDistUnitStatus = ArgumentsHudNaviWarnDistChanged::Input_NaviWarnDistUnitStatus;
    using HeadUnitNaviStatus = ArgumentsHeadUnitNaviStatus::Input_HeadUnitNaviStatus;
    using HudDISInfoOnOffStatus = ArgumentsInterHudDISInfoOnOffStatusChanged::Inter_HudDISInfoOnOffStatus;
    using IslaOptionStatus = ArgumentsIslaOptionStatusChanged::Input_IslaOptionStatus;
    using IslaSpeedLimitTrafficSignStatus = ArgumentsSgnISLAMainSpeedForHUDStatChanged::Input_IslaSpeedLimitTrafficSignStatus;
    using IslaTrafficSignCountryInformationStatus =
        ArgumentsSgnIslaTrafficSignCountryInformationStatusChanged::Input_IslaTrafficSignCountryInformationStatus;
    using ConfigAdasISLA = ArgumentsConfigNewAdasISLAChanged::Inter_ConfigAdasISLA;
    using IslaAdditionalTrafficSignStatus =
        ArgumentsSgnIslaAdditionalTrafficSignStatusChanged::Input_IslaAdditionalTrafficSignStatus;
    using ConfigNaviDisCountry = ArgumentsConfigNaviDisCountryChanged::Inter_ConfigNaviDisCountry;
    using IslaSupplementaryTrafficSignStatus =
        ArgumentsSgnIslaSupplementaryTrafficSignStatusChanged::Input_IslaSupplementaryTrafficSignStatus;
    using CameraSafetyAlertStatSet = std::set<Private_DISAddtnlSignForHUD>;
    using AdditionalSignMap = std::map<IslaTrafficSignCountryInformationStatus, Private_ISLAAddtnlSign>;
    using NaviWarnDistUnitStatusMap =
        std::map<NaviWarnDistUnitStatus, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat>;
    using TrafficSignStatusWithIslaAddtnlSignMap = std::map<IslaAdditionalTrafficSignStatus, AdditionalSignMap>;
    using ConfigHUD = ArgumentsConfigChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigChanged::Inter_ConfigNewHUD;
    using Private_ISLAMainSpeedValueStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat;
    using Private_DISMainSpeedValueStat = SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedValueStat;
    using DisplaySpeedUnit = ArgumentsUnitChanged::Inter_DisplaySpeedUnit;
    using NaviOverSpeedUnit = ArgumentsUnitChanged::Input_NaviOverSpdUnit;
    using IslaNextSpeedLimitTrafficSignStatus =
        ArgumentsIslaNextSpeedLimitTrafficSignStatusChanged::Input_IslaNextSpeedLimitTrafficSignStatus;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using IslaSupplementaryTrafficSign2Status =
        ArgumentsIslaSupplementaryTrafficSign2StatusChanged::Input_IslaSupplementaryTrafficSign2Status;
    using INDIslaAdditionalTrafficSignStatus = ArgumentsSgnINDIslaInfoChanged::Input_INDIslaAdditionalTrafficSignStatus;
    using INDIslaNextSpeedLimitTrafficSignStatus = ArgumentsSgnINDIslaInfoChanged::Input_INDIslaNextSpeedLimitTrafficSignStatus;
    using INDIslaSpeedLimitTrafficSignStatus = ArgumentsSgnINDIslaInfoChanged::Input_INDIslaSpeedLimitTrafficSignStatus;
    using INDIslaSupplementaryTrafficSign2Status = ArgumentsSgnINDIslaInfoChanged::Input_INDIslaSupplementaryTrafficSign2Status;
    using INDIslaSupplementaryTrafficSignStatus = ArgumentsSgnINDIslaInfoChanged::Input_INDIslaSupplementaryTrafficSignStatus;
    using INDIslaTrafficSignCountryInformationStatus =
        ArgumentsSgnINDIslaInfoChanged::Input_INDIslaTrafficSignCountryInformationStatus;
    using AdditionalSignMapIND = std::map<INDIslaTrafficSignCountryInformationStatus, Private_ISLAAddtnlSign>;
    using INDTrafficSignStatusWithIslaAddtnlSignMap = std::map<INDIslaAdditionalTrafficSignStatus, AdditionalSignMapIND>;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    DisCameraSafetyAlertStatus mDisCameraSafetyAlertStatus = DisCameraSafetyAlertStatus::OFF;
    NaviWarnDistUnitStatus mNaviWarnDistUnitStatus = NaviWarnDistUnitStatus::INVALID;
    HeadUnitNaviStatus mHeadUnitNaviStatus = HeadUnitNaviStatus::INVALID;
    HudDISInfoOnOffStatus mHudDISInfoOnOffStatus = HudDISInfoOnOffStatus::INVALID;
    IslaOptionStatus mIslaOptionStatus = IslaOptionStatus::OFF;
    IslaSpeedLimitTrafficSignStatus mIslaSpeedLimitTrafficSignStatus = IslaSpeedLimitTrafficSignStatus::OFF;
    IslaTrafficSignCountryInformationStatus mIslaTrafficSignCountryInformationStatus =
        IslaTrafficSignCountryInformationStatus::INVALID;
    ConfigAdasISLA mConfigAdasISLA = ConfigAdasISLA::OFF;
    IslaAdditionalTrafficSignStatus mIslaAdditionalTrafficSignStatus = IslaAdditionalTrafficSignStatus::OFF;
    ConfigNaviDisCountry mConfigNaviDisCountry = ConfigNaviDisCountry::INVALID;
    IslaSupplementaryTrafficSignStatus mIslaSupplementaryTrafficSignStatus = IslaSupplementaryTrafficSignStatus::OFF;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    DisplaySpeedUnit mDisplaySpeedUnit = DisplaySpeedUnit::NONE;
    NaviOverSpeedUnit mNaviOverSpeedUnit = NaviOverSpeedUnit::OFF;
    IslaNextSpeedLimitTrafficSignStatus mIslaNextSpeedLimitTrafficSignStatus = IslaNextSpeedLimitTrafficSignStatus::OFF;
    HUInt64 mIslaNextSpeedLimitTrafficSignValue = 0;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;
    HUInt64 mIslaNextSupportTrafficSignValue = 0;
    IslaSupplementaryTrafficSign2Status mIslaSupplementaryTrafficSign2Status = IslaSupplementaryTrafficSign2Status::OFF;
    HUInt64 mNaviWarnDistOverDecimalValue = 0;
    HBool mNaviWarnDistOverDecimalValueTimeOut = false;
    HUInt64 mNaviWarnDistUnderDecimalValue = 0;
    HBool mNaviWarnDistUnderDecimalValueTimeOut = false;

    INDIslaAdditionalTrafficSignStatus mINDIslaAdditionalTrafficSignStatus = INDIslaAdditionalTrafficSignStatus::OFF;
    INDIslaNextSpeedLimitTrafficSignStatus mINDIslaNextSpeedLimitTrafficSignStatus = INDIslaNextSpeedLimitTrafficSignStatus::OFF;
    INDIslaSpeedLimitTrafficSignStatus mINDIslaSpeedLimitTrafficSignStatus = INDIslaSpeedLimitTrafficSignStatus::OFF;
    INDIslaSupplementaryTrafficSign2Status mINDIslaSupplementaryTrafficSign2Status = INDIslaSupplementaryTrafficSign2Status::OFF;
    INDIslaSupplementaryTrafficSignStatus mINDIslaSupplementaryTrafficSignStatus = INDIslaSupplementaryTrafficSignStatus::OFF;
    INDIslaTrafficSignCountryInformationStatus mINDIslaTrafficSignCountryInformationStatus =
        INDIslaTrafficSignCountryInformationStatus::INVALID;
    HUInt64 mINDIslaNextSpeedLimitTrafficSignValue = 0;
    HUInt64 mINDIslaNextSupportTrafficSignValue = 0;
    HUInt64 mINDIslaSpeedLimitTrafficSignValue = 0;

    HUInt64 mDisSpeedLimitValue = 0;
    static constexpr HUInt64 kUnderDecimalValueNaviWarnDist10 = 10;
    static constexpr HUInt64 kOverDecimalValueNaviWarnDist100 = 100;
    // static constexpr HUInt64 kTimerCountFinalMainSpeedValue300ms = 300;
    static constexpr HUInt64 kTimerMainSpeedInterval300ms = 300;
    static constexpr HUInt64 kTimerAddtnSignInterval300ms = 300;
    static constexpr HUInt64 kNaviOverSpeedRangeMin = 0x01;
    static constexpr HUInt64 kNaviOverSpeedRangeMax = 0xFE;
    static constexpr HUInt32 kNaviOverSpeedColorMin = 0x000001;
    static constexpr HUInt32 kNaviOverSpeedColorMax = 0xFFFFFE;

    ssfs::SFCTimer<Detail_Contents_ISLA_DIS_NEW> mOneShotTimerFinalMainSpeed;
    ssfs::SFCTimer<Detail_Contents_ISLA_DIS_NEW> mOneShotTimerFinalAddtnlSign;
    HUInt64 mIslwMainSignValue = 0;
    HUInt64 mPrivateDISMainSpeedValue = 0;
    HUInt64 mPrivateISLAMainSpeedValue = 0;
    HUInt64 mIslaSpeedLimitTrafficSignValue = 0;
    HUInt64 mNaviOverSpeedRange1 = 0;
    HUInt64 mNaviOverSpeedRange2 = 0;
    HUInt64 mNaviRangeCamAvgSpeed = 0;
    HUInt32 mNaviOverSpeedColor1 = 0;
    HUInt32 mNaviOverSpeedColor2 = 0;
    HBool mIsIgnOn = false;

    Private_ISLAMainSpeedForHUDStat mPrivateISLAMainSpeedForHUDStat = Private_ISLAMainSpeedForHUDStat::OFF;
    Private_ISLAMainSpeedForHUDStat mPrevPrivateISLAMainSpeedForHUDStat = Private_ISLAMainSpeedForHUDStat::OFF;
    Private_DISMainSpeedStat mPrivateDISMainSpeedStat = Private_DISMainSpeedStat::OFF;
    Private_DISMainSpeedStat mPrevPrivateDISMainSpeedStat = Private_DISMainSpeedStat::OFF;
    Private_DISMainSpeedValueStat mPrivateDISMainSpeedValueStat = Private_DISMainSpeedValueStat::OFF;
    Private_DISMainSpeedValueStat mPrevPrivateDISMainSpeedValueStat = Private_DISMainSpeedValueStat::OFF;
    Private_ISLAMainSpeedValueStat mPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::OFF;
    Private_ISLAMainSpeedValueStat mPrevPrivateISLAMainSpeedValueStat = Private_ISLAMainSpeedValueStat::OFF;
    Private_ISLAAddtnlSign mPrivateISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
    Private_ISLAAddtnlSign mPrevPrivateISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
    Private_DISAddtnlSignForHUD mPrivateDISAddtnlSignForHUD = Private_DISAddtnlSignForHUD::OFF;
    Private_DISAddtnlSignForHUD mPrevPrivateDISAddtnlSignForHUD = Private_DISAddtnlSignForHUD::OFF;
    SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat mFinalMainSpeedStat =
        SFCDetail_Contents_ISLA_DIS_NEWConstantFinalMainSpeedStatStat::OFF;
    SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat mFinalAddtnlStat =
        SFCDetail_Contents_ISLA_DIS_NEWConstantFinalAddtnlSignStat::OFF;
    SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat mCurrentISLANextSpeedLimitTrafficSignStat =
        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSpeedLimitTrafficSignTypeStat::OFF;
    SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat mCurrentIslaNextSupportTrafficSignValue =
        SFCDetail_Contents_ISLA_DIS_NEWConstantISLANextSupportTrafficSignStat::OFF;

    NaviWarnDistUnitStatusMap mNaviWarnDistUnitStatusMap = {
        {NaviWarnDistUnitStatus::METER, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::METER},
        {NaviWarnDistUnitStatus::KILOMETER, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::KILOMETER},
        {NaviWarnDistUnitStatus::MILE, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::MILE},
        {NaviWarnDistUnitStatus::FEET, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::FEET},
        {NaviWarnDistUnitStatus::YARD, SFCDetail_Contents_ISLA_DIS_NEWConstantHudNaviWarnDistUnitStat::YARD}};

    const AdditionalSignMap mAdditionalSignSchoolCross = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::SCHOOL_CROSS_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::SCHOOL_CROSS_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::SCHOOL_CROSS_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::SCHOOL_CROSS_AUS},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::SCHOOL_CROSS_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::SCHOOL_CROSS_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::SCHOOL_CROSS_CN},
    };

    const AdditionalSignMap mAdditionalSignStop = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::STOP_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::STOP_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::STOP_EU},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::STOP_EU},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::STOP_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::STOP_EU},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::STOP_CN},
    };

    const AdditionalSignMap mAdditionalSignYield = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::YIELD_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::YIELD_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::YIELD_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::YIELD_AUS},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::YIELD_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::YIELD_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::YIELD_CN},
    };

    const AdditionalSignMap mAdditionalSignDoNotPassLhd = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_CN},
    };

    const AdditionalSignMap mAdditionalSignDoNotPassRhd = {
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::DO_NOT_PASS_RHD_EU},
    };

    const AdditionalSignMap mAdditionalSignRoundAboutLhd = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_CN},
    };

    const AdditionalSignMap mAdditionalSignRoundAboutRhd = {
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_AUS},
    };

    const AdditionalSignMap mAdditionalSignRightCurves = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::RIGHT_CURVE_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::RIGHT_CURVE_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::RIGHT_CURVE_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::RIGHT_CURVE_CN},
    };

    const AdditionalSignMap mAdditionalSignLeftCurves = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::LEFT_CURVE_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::LEFT_CURVE_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::LEFT_CURVE_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::LEFT_CURVE_CN},
    };

    const AdditionalSignMap mAdditionalSignRightLeftCurves = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_CN},
    };

    const AdditionalSignMap mAdditionalSignLeftRightCurves = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_CN},
    };

    const AdditionalSignMap mAdditionalSignPedestrianCrossing = {
        {IslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_DOM},
        {IslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU},
        {IslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU},
        {IslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_AUS},
        {IslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA},
        {IslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA},
        {IslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_CN},
    };

    const TrafficSignStatusWithIslaAddtnlSignMap mTrafficSignStatusWithIslaAddtnlSignMap = {
        {IslaAdditionalTrafficSignStatus::SCHOOL_CROSSING, mAdditionalSignSchoolCross},
        {IslaAdditionalTrafficSignStatus::STOP, mAdditionalSignStop},
        {IslaAdditionalTrafficSignStatus::YIELD, mAdditionalSignYield},
        {IslaAdditionalTrafficSignStatus::DO_NOT_PASS_LHD, mAdditionalSignDoNotPassLhd},
        {IslaAdditionalTrafficSignStatus::DO_NOT_PASS_RHD, mAdditionalSignDoNotPassRhd},
        {IslaAdditionalTrafficSignStatus::ROUNDABOUT_LHD, mAdditionalSignRoundAboutLhd},
        {IslaAdditionalTrafficSignStatus::ROUNDABOUT_RHD, mAdditionalSignRoundAboutRhd},
        {IslaAdditionalTrafficSignStatus::RIGHT_CURVE, mAdditionalSignRightCurves},
        {IslaAdditionalTrafficSignStatus::LEFT_CURVE, mAdditionalSignLeftCurves},
        {IslaAdditionalTrafficSignStatus::RIGHT_LEFT_CURVES, mAdditionalSignRightLeftCurves},
        {IslaAdditionalTrafficSignStatus::LEFT_RIGHT_CURVES, mAdditionalSignLeftRightCurves},
        {IslaAdditionalTrafficSignStatus::PEDESTRIAN_CROSSING, mAdditionalSignPedestrianCrossing},
    };

    const AdditionalSignMapIND mAdditionalSignSchoolCrossIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::SCHOOL_CROSS_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::SCHOOL_CROSS_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::SCHOOL_CROSS_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::SCHOOL_CROSS_AUS},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::SCHOOL_CROSS_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::SCHOOL_CROSS_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::SCHOOL_CROSS_CN},
    };

    const AdditionalSignMapIND mAdditionalSignStopIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::STOP_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::STOP_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::STOP_EU},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::STOP_EU},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::STOP_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::STOP_EU},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::STOP_CN},
    };

    const AdditionalSignMapIND mAdditionalSignYieldIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::YIELD_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::YIELD_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::YIELD_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::YIELD_AUS},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::YIELD_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::YIELD_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::YIELD_CN},
    };

    const AdditionalSignMapIND mAdditionalSignDoNotPassLhdIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_CN},
    };

    const AdditionalSignMapIND mAdditionalSignDoNotPassRhdIND = {
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::DO_NOT_PASS_RHD_EU},
    };

    const AdditionalSignMapIND mAdditionalSignRoundAboutLhdIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::ROUNDABOUT_LHD_CN},
    };

    const AdditionalSignMapIND mAdditionalSignRoundAboutRhdIND = {
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::ROUNDABOUT_RHD_AUS},
    };

    const AdditionalSignMapIND mAdditionalSignRightCurvesIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::RIGHT_CURVE_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::RIGHT_CURVE_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::RIGHT_CURVE_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::RIGHT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::RIGHT_CURVE_CN},
    };

    const AdditionalSignMapIND mAdditionalSignLeftCurvesIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::LEFT_CURVE_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::LEFT_CURVE_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::LEFT_CURVE_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::LEFT_CURVE_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::LEFT_CURVE_CN},
    };

    const AdditionalSignMapIND mAdditionalSignRightLeftCurvesIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_CN},
    };

    const AdditionalSignMapIND mAdditionalSignLeftRightCurvesIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_CN},
    };

    const AdditionalSignMapIND mAdditionalSignPedestrianCrossingIND = {
        {INDIslaTrafficSignCountryInformationStatus::DOMESTIC, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_DOM},
        {INDIslaTrafficSignCountryInformationStatus::EUROPE, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU},
        {INDIslaTrafficSignCountryInformationStatus::RUSSIA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU},
        {INDIslaTrafficSignCountryInformationStatus::AUSTRALIA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_AUS},
        {INDIslaTrafficSignCountryInformationStatus::CANADA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA},
        {INDIslaTrafficSignCountryInformationStatus::USA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA},
        {INDIslaTrafficSignCountryInformationStatus::CHINA, Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_CN},
    };

    const INDTrafficSignStatusWithIslaAddtnlSignMap mINDTrafficSignStatusWithIslaAddtnlSignMap = {
        {INDIslaAdditionalTrafficSignStatus::SCHOOL_CROSSING, mAdditionalSignSchoolCrossIND},
        {INDIslaAdditionalTrafficSignStatus::STOP, mAdditionalSignStopIND},
        {INDIslaAdditionalTrafficSignStatus::YIELD, mAdditionalSignYieldIND},
        {INDIslaAdditionalTrafficSignStatus::DO_NOT_PASS_LHD, mAdditionalSignDoNotPassLhdIND},
        {INDIslaAdditionalTrafficSignStatus::DO_NOT_PASS_RHD, mAdditionalSignDoNotPassRhdIND},
        {INDIslaAdditionalTrafficSignStatus::ROUNDABOUT_LHD, mAdditionalSignRoundAboutLhdIND},
        {INDIslaAdditionalTrafficSignStatus::ROUNDABOUT_RHD, mAdditionalSignRoundAboutRhdIND},
        {INDIslaAdditionalTrafficSignStatus::RIGHT_CURVE, mAdditionalSignRightCurvesIND},
        {INDIslaAdditionalTrafficSignStatus::LEFT_CURVE, mAdditionalSignLeftCurvesIND},
        {INDIslaAdditionalTrafficSignStatus::RIGHT_LEFT_CURVES, mAdditionalSignRightLeftCurvesIND},
        {INDIslaAdditionalTrafficSignStatus::LEFT_RIGHT_CURVES, mAdditionalSignLeftRightCurvesIND},
        {INDIslaAdditionalTrafficSignStatus::PEDESTRIAN_CROSSING, mAdditionalSignPedestrianCrossingIND},
    };

    const CameraSafetyAlertStatSet mCameraSafetyAlertStatSet = {Private_DISAddtnlSignForHUD::KOREA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::KOREA_PORTABLECAMERA,
                                                                Private_DISAddtnlSignForHUD::KOREA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::KOREA_BUSCAMERA,
                                                                Private_DISAddtnlSignForHUD::KOREA_SCHOOLZONEFIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::KOREA_SCHOOLZONESPEEL_SIGNALCAMERA,
                                                                Private_DISAddtnlSignForHUD::KOREA_CUTINLINE,
                                                                Private_DISAddtnlSignForHUD::KOREA_ROADSHOULDERVIOLATION,
                                                                Private_DISAddtnlSignForHUD::KOREA_TRAFFICINFORMATION,
                                                                Private_DISAddtnlSignForHUD::KOREA_INTERSECTIONJAMMING,
                                                                Private_DISAddtnlSignForHUD::KOREA_RAMPMETERING,
                                                                Private_DISAddtnlSignForHUD::KOREA_LANECHANGE,
                                                                Private_DISAddtnlSignForHUD::KOREA_BUSLANENOTAVAILABLE,
                                                                Private_DISAddtnlSignForHUD::KOREA_BUSLANEAVAILABLE,
                                                                Private_DISAddtnlSignForHUD::KOREA_ILLEGALPARKING,
                                                                Private_DISAddtnlSignForHUD::KOREA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::KOREA_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::KOREA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::KOREA_FALLINGROCKS,
                                                                Private_DISAddtnlSignForHUD::KOREA_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::KOREA_FOGGYAREA,
                                                                Private_DISAddtnlSignForHUD::KOREA_STEEPSLOPE,
                                                                Private_DISAddtnlSignForHUD::KOREA_ROADKILL,
                                                                Private_DISAddtnlSignForHUD::KOREA_SPEEDBUMP,
                                                                Private_DISAddtnlSignForHUD::KOREA_NARROWROAD,
                                                                Private_DISAddtnlSignForHUD::KOREA_HIGHWAYCONSTRUCTIONZONE,
                                                                Private_DISAddtnlSignForHUD::KOREA_ROADSLIPDETECTION,
                                                                Private_DISAddtnlSignForHUD::KOREA_ROADDAMAGEDETECTION,
                                                                Private_DISAddtnlSignForHUD::KOREA_FALLING_HAZARD,
                                                                Private_DISAddtnlSignForHUD::KOREA_DROWSY,
                                                                Private_DISAddtnlSignForHUD::USA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::USA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::USA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::USA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::CANADA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::CANADA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::CANADA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::CANADA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::MEXICO_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::EUROPE_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::EUROPE_PORTABLECAMERA,
                                                                Private_DISAddtnlSignForHUD::EUROPE_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::EUROPE_AVERAGESPEEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::EUROPE_AVERAGESPEEDCAMERAZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_SPEEDENFORCEMET,
                                                                Private_DISAddtnlSignForHUD::EUROPE_SPEEDENFORCEMETZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_MOBILEHOTSPOT,
                                                                Private_DISAddtnlSignForHUD::EUROPE_MOBILEHOTSPOTZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_ACCIDENTBLACKSPOT1,
                                                                Private_DISAddtnlSignForHUD::EUROPE_ACCIDENTBLACKSPOTZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_DANGERZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_RISKZONE,
                                                                Private_DISAddtnlSignForHUD::EUROPE_RESTRICTED,
                                                                Private_DISAddtnlSignForHUD::EUROPE_OTHERS,
                                                                Private_DISAddtnlSignForHUD::EUROPE_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::MIDDLE_EAST_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::MIDDLE_EAST_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::MIDDLE_EAST_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_BUSCAMERA,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::AUSTRALIA_ROADKILL,
                                                                Private_DISAddtnlSignForHUD::SOUTH_AMERICA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::SOUTH_AMERICA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::SOUTH_AMERICA_BUSCAMERA,
                                                                Private_DISAddtnlSignForHUD::SOUTH_AMERICA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::CHINA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::CHINA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::CHINA_BUSCAMERA,
                                                                Private_DISAddtnlSignForHUD::CHINA_OTHERS,
                                                                Private_DISAddtnlSignForHUD::CHINA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::CHINA_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::CHINA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::CHINA_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::CHINA_NARROWROAD,
                                                                Private_DISAddtnlSignForHUD::CHINA_BLINDSLOPE_CONTINUOUS,
                                                                Private_DISAddtnlSignForHUD::CHINA_CROSSWIND,
                                                                Private_DISAddtnlSignForHUD::CHINA_HILLSIDEDANGEROUS,
                                                                Private_DISAddtnlSignForHUD::CHINA_HUMP_BRIDG,
                                                                Private_DISAddtnlSignForHUD::CHINA_JOINT_LEFT,
                                                                Private_DISAddtnlSignForHUD::CHINA_JOINT_RIGHT,
                                                                Private_DISAddtnlSignForHUD::CHINA_LOWSPEED,
                                                                Private_DISAddtnlSignForHUD::CHINA_NARROW_BOTH,
                                                                Private_DISAddtnlSignForHUD::CHINA_NARROWBRIDGE,
                                                                Private_DISAddtnlSignForHUD::CHINA_NARROW_LEFT,
                                                                Private_DISAddtnlSignForHUD::CHINA_NARROW_RIGHT,
                                                                Private_DISAddtnlSignForHUD::CHINA_ROCKFALL_LEFT,
                                                                Private_DISAddtnlSignForHUD::CHINA_ROCKFALL_RIGHT,
                                                                Private_DISAddtnlSignForHUD::CHINA_SLIP,
                                                                Private_DISAddtnlSignForHUD::CHINA_BLINDSLOPE_UP,
                                                                Private_DISAddtnlSignForHUD::CHINA_BLINDSLOPE_DOWN,
                                                                Private_DISAddtnlSignForHUD::CHINA_TUNNEL,
                                                                Private_DISAddtnlSignForHUD::CHINA_UNDERWATER,
                                                                Private_DISAddtnlSignForHUD::CHINA_UNEVEN,
                                                                Private_DISAddtnlSignForHUD::CHINA_UNMANAGED_RAILWAY_CROSS,
                                                                Private_DISAddtnlSignForHUD::CHINA_ZIPPASS,
                                                                Private_DISAddtnlSignForHUD::CHINA_SLOPE_CONTINUOUSDOWN,
                                                                Private_DISAddtnlSignForHUD::CHINA_SLOPE_UP,
                                                                Private_DISAddtnlSignForHUD::CHINA_SLOPE_DOWN,
                                                                Private_DISAddtnlSignForHUD::CHINA_RAILWAY_MANAGED,
                                                                Private_DISAddtnlSignForHUD::CHINA_VILLAGE,
                                                                Private_DISAddtnlSignForHUD::CHINA_AUDIBLEWARNING,
                                                                Private_DISAddtnlSignForHUD::JAPAN_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::JAPAN_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::JAPAN_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::JAPAN_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::JAPAN_NARROWROAD,
                                                                Private_DISAddtnlSignForHUD::JAPAN_ZONE_30,
                                                                Private_DISAddtnlSignForHUD::JAPAN_STOP,
                                                                Private_DISAddtnlSignForHUD::JAPAN_LANE_MOVE_REGULATION,
                                                                Private_DISAddtnlSignForHUD::INDIA_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::INDIA_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::INDIA_DONOTENTER,
                                                                Private_DISAddtnlSignForHUD::INDIA_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::INDIA_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::INDIA_FALLINGROCKS,
                                                                Private_DISAddtnlSignForHUD::INDIA_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::INDIA_SPEEDBUMP,
                                                                Private_DISAddtnlSignForHUD::INDIA_OVERTAKING_LANE,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_FALLINGROCKS,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_STOP,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_E_OVERTAKING_LANE,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_FIXEDCAMERA,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_SPEED_SIGNAL,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_ACCIDENTBLACKSPOT2,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_SCHOOLZONE,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_FALLINGROCKS,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_RAILROADCROSSING,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_STOP,
                                                                Private_DISAddtnlSignForHUD::SOUTH_EAST_ASIA_N_OVERTAKING_LANE};
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_ISLA_DIS_NEW_H
