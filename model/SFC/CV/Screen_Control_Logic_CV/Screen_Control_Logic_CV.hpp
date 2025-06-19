/**
 * @file Screen_Control_Logic_CV.hpp
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
#ifndef SFSS_Screen_Control_Logic_CV_H
#define SFSS_Screen_Control_Logic_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Screen_Control_Logic_CVBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.22
// Reference : [CV650] Screen_Control_Logic_CV. Contains : Constant. Recommend: VALUE_CHANGED
class Screen_Control_Logic_CV : public Screen_Control_Logic_CVBase {
public:
    Screen_Control_Logic_CV() = default;
    ~Screen_Control_Logic_CV() override = default;
    Screen_Control_Logic_CV(const Screen_Control_Logic_CV& other) = delete;
    Screen_Control_Logic_CV(Screen_Control_Logic_CV&& other) noexcept = delete;
    Screen_Control_Logic_CV& operator=(const Screen_Control_Logic_CV& other) = delete;
    Screen_Control_Logic_CV& operator=(Screen_Control_Logic_CV&& other) noexcept = delete;

    void onInitialize() override {
        updateAccumulScreenEnable();
        updateDriveInfoScreenEnable();
        updateAfterRefuelScreenEnable();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAVNDisplayOnOffStatus();
    }

    void onClusterThemeSetStatus(const ArgumentsClusterThemeSetStatus& args) {
        mClusterThemeSetStatus = args.mInter_ClusterThemeSetStatus;
        updateDisplayTheme();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEnergyFlowScreen();
        updateActiveRPMGaugeDisplay();
        updateSootGaugeInfo();
        updateOilPressGaugeInfo();
        updateGaugeInfoScreen1();
    }

    void onConfigRPMGaugeFixedChanged(const ArgumentsConfigRPMGaugeFixedChanged& args) {
        mConfigRPMGaugeFixed = args.mInter_ConfigRPMGaugeFixed;
        updateActiveRPMGaugeDisplay();
    }

    void onConfigTBTChanged(const ArgumentsConfigTBTChanged& args) {
        mConfigTBT = args.mInter_ConfigTBT;
        updateTBTGuideScreen();
    }

    void onTbtDisplayTypeStatusChanged(const ArgumentsTbtDisplayTypeStatusChanged& args) {
        mTbtDisplayTypeStatus = args.mInput_TbtDisplayTypeStatus;
        updateTBTDestinationScreen();
    }

    void onConfigSccLdwBcwChanged(const ArgumentsConfigSccLdwBcwChanged& args) {
        mConfigAdasSCC = args.mInter_ConfigAdasSCC;
        mConfigAdasLDW = args.mInter_ConfigAdasLDW;
        mConfigAdasBCW = args.mInter_ConfigAdasBCW;
        mConfigAdasBCWNear = args.mInter_ConfigAdasBCWNear;
        updateDriverAssistScreen();
    }

    void onConfigEdgChanged(const ArgumentsConfigEdgChanged& args) {
        mConfigEDG = args.mInter_ConfigEDG;
        updateEDGScreen();
    }

    void onConfigTpmsCvChanged(const ArgumentsConfigTpmsCvChanged& args) {
        mConfigTPMSCV = args.mInter_ConfigTPMSCV;
        updateTPMSPressScreen();
        updateTPMSTempScreen();
        updateTPMSBattScreen();
    }

    void onConfigAxleWeightChanged(const ArgumentsConfigAxleWeightChanged& args) {
        mConfigAxleWeight = args.mInter_ConfigAxleWeight;
        updateAxleWeightScreen();
    }

    void onEventCheckDisplayStatusChanged(const ArgumentsEventCheckDisplayStatusChanged& args) {
        mEventCheckDisplayStatus = args.mInter_EventCheckDisplayStatus;
        updateWarningScreen();
    }

    void onConstantPDC24VBatteryVoltageValueChanged(const ArgumentsConstantPDC24VBatteryVoltageValueChanged& args) {
        mPDC24VBatteryVoltageValue = args.mPDC24VBatteryVoltageValue;
        update24VBatteryGauge();
        updateGaugeInfoScreen2();
    }

    void onConstantPDC12VBatteryVoltageValueChanged(const ArgumentsConstantPDC12VBatteryVoltageValueChanged& args) {
        mPDC12VBatteryVoltageValue = args.mPDC12VBatteryVoltageValue;
        update12VBatteryGauge();
        updateGaugeInfoScreen2();
    }

    void onDisplayLanguageStatusChanged(const ArgumentsDisplayLanguageStatusChanged& args) {
        mDisplayLanguageStatus = args.mInter_DisplayLanguageStatus;
        updateDisplayLanguageStat();
    }

    void onHeadUnitDisplayOnOffStatusChanged(const ArgumentsHeadUnitDisplayOnOffStatusChanged& args) {
        mHeadUnitDisplayOnOffStatus = args.mInput_HeadUnitDisplayOnOffStatus;
        updateAVNDisplayOnOffStatus();
    }

    void onLwsPageStatusChanged(const ArgumentsLwsPageStatusChanged& args) {
        mLWSPageStatus = args.mInput_LWSPageStatus;
        updateLWSScreen();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.3 그래픽 테마 동작 로직 & 5.5 표시 테마
    void updateDisplayTheme() {
        switch (mClusterThemeSetStatus) {
            case ClusterThemeSetStatus::THEME_A:
                setSFCScreen_Control_Logic_CVInter_DisplayThemeStatus(
                    SFCScreen_Control_Logic_CVInter_DisplayThemeStatus::THEME_A_NORMAL);
                setSFCScreen_Control_Logic_CVConstantDisplayThemeStat(
                    SFCScreen_Control_Logic_CVConstantDisplayThemeStat::THEME_A_NORMAL);
                break;
            case ClusterThemeSetStatus::THEME_B:
                setSFCScreen_Control_Logic_CVInter_DisplayThemeStatus(
                    SFCScreen_Control_Logic_CVInter_DisplayThemeStatus::THEME_B_NORMAL);
                setSFCScreen_Control_Logic_CVConstantDisplayThemeStat(
                    SFCScreen_Control_Logic_CVConstantDisplayThemeStat::THEME_B_NORMAL);
                break;
            case ClusterThemeSetStatus::THEME_C:
                setSFCScreen_Control_Logic_CVInter_DisplayThemeStatus(
                    SFCScreen_Control_Logic_CVInter_DisplayThemeStatus::THEME_C_NORMAL);
                setSFCScreen_Control_Logic_CVConstantDisplayThemeStat(
                    SFCScreen_Control_Logic_CVConstantDisplayThemeStat::THEME_C_NORMAL);
                break;
            default:
                // no operation
                break;
        }
    }

    // 5.1.1 주행 보조 시스템
    void updateDriverAssistScreen() {
        SFCScreen_Control_Logic_CVConstantDriverAssistScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantDriverAssistScreenEnableStat::DISABLE;
        if (mConfigAdasSCC == ConfigAdasSCC::ON || mConfigAdasLDW == ConfigAdasLDW::ON || mConfigAdasBCW == ConfigAdasBCW::ON ||
            mConfigAdasBCWNear == ConfigAdasBCWNear::ON) {
            stat = SFCScreen_Control_Logic_CVConstantDriverAssistScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantDriverAssistScreenEnableStat(stat);
    }

    // 5.1.2 트립 정보
    // 5.1.2.1 주행 정보
    void updateDriveInfoScreenEnable() {
        setSFCScreen_Control_Logic_CVConstantDriveInfoScreenEnableStat(
            SFCScreen_Control_Logic_CVConstantDriveInfoScreenEnableStat::ENABLE);
    }

    // 5.1.2.2 주유후 정보
    void updateAfterRefuelScreenEnable() {
        setSFCScreen_Control_Logic_CVConstantAfterRefuelScreenEnableStat(
            SFCScreen_Control_Logic_CVConstantAfterRefuelScreenEnableStat::ENABLE);
    }

    // 5.1.2.3 누적 정보
    void updateAccumulScreenEnable() {
        setSFCScreen_Control_Logic_CVConstantAccumulScreenEnableStat(
            SFCScreen_Control_Logic_CVConstantAccumulScreenEnableStat::ENABLE);
    }

    // 5.1.2.4 에너지 흐름도
    void updateEnergyFlowScreen() {
        SFCScreen_Control_Logic_CVConstantEnergyFlowScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantEnergyFlowScreenEnableStat::DISABLE;
        if (mConfigVehicleType == ConfigVehicleType::FCEV) {
            stat = SFCScreen_Control_Logic_CVConstantEnergyFlowScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantEnergyFlowScreenEnableStat(stat);
    }

    // 5.1.2.5 연비 운전 가이드(EDG)
    void updateEDGScreen() {
        SFCScreen_Control_Logic_CVConstantEDGScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantEDGScreenEnableStat::DISABLE;
        if (mConfigEDG == ConfigEDG::ON) {
            stat = SFCScreen_Control_Logic_CVConstantEDGScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantEDGScreenEnableStat(stat);
    }

    // 5.1.3 Navigation
    // 5.1.3.1 TBT 길안내
    void updateTBTGuideScreen() {
        SFCScreen_Control_Logic_CVConstantTBTGuideScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantTBTGuideScreenEnableStat::DISABLE;
        if (mConfigTBT == ConfigTBT::ON) {
            stat = SFCScreen_Control_Logic_CVConstantTBTGuideScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantTBTGuideScreenEnableStat(stat);
    }

    // 5.1.3.2 TBT 목적지 정보
    void updateTBTDestinationScreen() {
        switch (mTbtDisplayTypeStatus) {
            case TbtDisplayTypeStatus::OFF:
            case TbtDisplayTypeStatus::ARRIVED:
            case TbtDisplayTypeStatus::NO_ROUTE_GUIDANCE:
            case TbtDisplayTypeStatus::CALC_ROUTE:
            case TbtDisplayTypeStatus::RECALC_ROUTE:
                setSFCScreen_Control_Logic_CVConstantTBTDestinationScreenEnableStat(
                    SFCScreen_Control_Logic_CVConstantTBTDestinationScreenEnableStat::DISABLE);
                break;
            default:
                setSFCScreen_Control_Logic_CVConstantTBTDestinationScreenEnableStat(
                    SFCScreen_Control_Logic_CVConstantTBTDestinationScreenEnableStat::ENABLE);
                break;
        }
    }

    // 5.1.4 점검 메시지
    // 5.1.4.1 고장 팝업
    void updateWarningScreen() {
        SFCScreen_Control_Logic_CVConstantWarningScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantWarningScreenEnableStat::DISABLE;
        if (mEventCheckDisplayStatus == EventCheckDisplayStatus::ON) {
            stat = SFCScreen_Control_Logic_CVConstantWarningScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantWarningScreenEnableStat(stat);
    }

    // 5.1.5 차량 정보(Utility)
    // 5.1.5.1 타이어 공기압
    void updateTPMSPressScreen() {
        SFCScreen_Control_Logic_CVConstantTPMSPressScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantTPMSPressScreenEnableStat::DISABLE;
        if (mConfigTPMSCV == ConfigTPMSCV::ON) {
            stat = SFCScreen_Control_Logic_CVConstantTPMSPressScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantTPMSPressScreenEnableStat(stat);
    }

    // 5.1.5.2 타이어 온도
    void updateTPMSTempScreen() {
        SFCScreen_Control_Logic_CVConstantTPMSTempScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantTPMSTempScreenEnableStat::DISABLE;
        if (mConfigTPMSCV == ConfigTPMSCV::ON) {
            stat = SFCScreen_Control_Logic_CVConstantTPMSTempScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantTPMSTempScreenEnableStat(stat);
    }

    // 5.1.5.3 TPMS 센서 배터리 잔량
    void updateTPMSBattScreen() {
        SFCScreen_Control_Logic_CVConstantTPMSBattScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantTPMSBattScreenEnableStat::DISABLE;
        if (mConfigTPMSCV == ConfigTPMSCV::ON) {
            stat = SFCScreen_Control_Logic_CVConstantTPMSBattScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantTPMSBattScreenEnableStat(stat);
    }

    // 5.1.5.4 브레이크 라이닝 잔량
    void updateLWSScreen() {
        SFCScreen_Control_Logic_CVConstantLWSScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantLWSScreenEnableStat::DISABLE;
        if (mLWSPageStatus == LWSPageStatus::ON) {
            stat = SFCScreen_Control_Logic_CVConstantLWSScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantLWSScreenEnableStat(stat);
    }

    // 5.1.5.5 축 하중
    void updateAxleWeightScreen() {
        SFCScreen_Control_Logic_CVConstantAxleWeightScreenEnableStat stat =
            SFCScreen_Control_Logic_CVConstantAxleWeightScreenEnableStat::DISABLE;
        if (mConfigAxleWeight == ConfigAxleWeight::ON) {
            stat = SFCScreen_Control_Logic_CVConstantAxleWeightScreenEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantAxleWeightScreenEnableStat(stat);
    }

    // 5.1.6 게이지 정보
    // 5.1.6.1 게이지 정보_1 페이지
    void updateGaugeInfoScreen1() {
        SFCScreen_Control_Logic_CVConstantGaugeInfoScreen1EnableStat stat =
            SFCScreen_Control_Logic_CVConstantGaugeInfoScreen1EnableStat::DISABLE;
        if (mIMG_ShootGaugeInfoEnable_stat == SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat::ENABLE ||
            mIMG_OilPressGaugeInfoEnable_stat == SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat::ENABLE) {
            stat = SFCScreen_Control_Logic_CVConstantGaugeInfoScreen1EnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantGaugeInfoScreen1EnableStat(stat);
    }

    // 5.1.6.2 Soot 게이지 표시
    void updateSootGaugeInfo() {
        SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat stat =
            SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat::DISABLE;
        if (mConfigVehicleType == ConfigVehicleType::ICV) {
            stat = SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat::ENABLE;
        }
        mIMG_ShootGaugeInfoEnable_stat = stat;
        setSFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat(stat);
    }

    // 5.1.6.3 엔진 오일 압력 게이지 표시
    void updateOilPressGaugeInfo() {
        SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat stat =
            SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat::DISABLE;
        if (mConfigVehicleType == ConfigVehicleType::ICV) {
            stat = SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat::ENABLE;
        }
        mIMG_OilPressGaugeInfoEnable_stat = stat;
        setSFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat(stat);
    }

    // 5.1.6.4 게이지 정보_2 페이지
    void updateGaugeInfoScreen2() {
        SFCScreen_Control_Logic_CVConstantGaugeInfoScreen2EnableStat stat =
            SFCScreen_Control_Logic_CVConstantGaugeInfoScreen2EnableStat::DISABLE;
        if (mIMG_24VBatteryGauge_stat == SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat::ENABLE ||
            mIMG_12VBatteryGauge_stat == SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat::ENABLE) {
            stat = SFCScreen_Control_Logic_CVConstantGaugeInfoScreen2EnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantGaugeInfoScreen2EnableStat(stat);
    }

    // 5.1.6.5 24V 배터리 전압 게이지
    void update24VBatteryGauge() {
        SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat stat =
            SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat::DISABLE;
        if (mPDC24VBatteryVoltageValue != 0x0) {
            stat = SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat::ENABLE;
        }
        mIMG_24VBatteryGauge_stat = stat;
        setSFCScreen_Control_Logic_CVConstantBatteryGauge24VStat(stat);
    }

    // 5.1.6.6 12V 배터리 전압 게이지
    void update12VBatteryGauge() {
        SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat stat =
            SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat::DISABLE;
        if (mPDC12VBatteryVoltageValue != 0x0) {
            stat = SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat::ENABLE;
        }
        mIMG_12VBatteryGauge_stat = stat;
        setSFCScreen_Control_Logic_CVConstantBatteryGauge12VStat(stat);
    }

    // 5.3 RPM 게이지 표시 활성화
    void updateActiveRPMGaugeDisplay() {
        SFCScreen_Control_Logic_CVConstantRPMGaugeDisplayEnableStat stat =
            SFCScreen_Control_Logic_CVConstantRPMGaugeDisplayEnableStat::DISABLE;
        if (mConfigVehicleType == ConfigVehicleType::ICV && mConfigRPMGaugeFixed == ConfigRPMGaugeFixed::OFF) {
            stat = SFCScreen_Control_Logic_CVConstantRPMGaugeDisplayEnableStat::ENABLE;
        }
        setSFCScreen_Control_Logic_CVConstantRPMGaugeDisplayEnableStat(stat);
    }

    // 5.4 표시 언어
    void updateDisplayLanguageStat() {
        setSFCScreen_Control_Logic_CVConstantDisplayLanguageStat(
            static_cast<SFCScreen_Control_Logic_CVConstantDisplayLanguageStat>(mDisplayLanguageStatus));
    }

    // 5.9 AVN 디스플레이 동작 여부
    void updateAVNDisplayOnOffStatus() {
        SFCScreen_Control_Logic_CVConstantAVNDisplayOnOffStat stat = SFCScreen_Control_Logic_CVConstantAVNDisplayOnOffStat::OFF;
        if (mIsIgnOn == false) {
            if (mHeadUnitDisplayOnOffStatus == HeadUnitDisplayOnOffStatus::ON) {
                stat = SFCScreen_Control_Logic_CVConstantAVNDisplayOnOffStat::ON;
            }
            setSFCScreen_Control_Logic_CVConstantAVNDisplayOnOffStat(stat);
        }
    }

    using ClusterThemeSetStatus = ArgumentsClusterThemeSetStatus::Inter_ClusterThemeSetStatus;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigRPMGaugeFixed = ArgumentsConfigRPMGaugeFixedChanged::Inter_ConfigRPMGaugeFixed;
    using ConfigTBT = ArgumentsConfigTBTChanged::Inter_ConfigTBT;
    using TbtDisplayTypeStatus = ArgumentsTbtDisplayTypeStatusChanged::Input_TbtDisplayTypeStatus;
    using ConfigAdasSCC = ArgumentsConfigSccLdwBcwChanged::Inter_ConfigAdasSCC;
    using ConfigAdasLDW = ArgumentsConfigSccLdwBcwChanged::Inter_ConfigAdasLDW;
    using ConfigAdasBCW = ArgumentsConfigSccLdwBcwChanged::Inter_ConfigAdasBCW;
    using ConfigAdasBCWNear = ArgumentsConfigSccLdwBcwChanged::Inter_ConfigAdasBCWNear;
    using ConfigEDG = ArgumentsConfigEdgChanged::Inter_ConfigEDG;
    using ConfigTPMSCV = ArgumentsConfigTpmsCvChanged::Inter_ConfigTPMSCV;
    using ConfigAxleWeight = ArgumentsConfigAxleWeightChanged::Inter_ConfigAxleWeight;
    using EventCheckDisplayStatus = ArgumentsEventCheckDisplayStatusChanged::Inter_EventCheckDisplayStatus;
    using DisplayLanguageStatus = ArgumentsDisplayLanguageStatusChanged::Inter_DisplayLanguageStatus;
    using HeadUnitDisplayOnOffStatus = ArgumentsHeadUnitDisplayOnOffStatusChanged::Input_HeadUnitDisplayOnOffStatus;
    using LWSPageStatus = ArgumentsLwsPageStatusChanged::Input_LWSPageStatus;

    HBool mIsIgnOn = false;
    ClusterThemeSetStatus mClusterThemeSetStatus = ClusterThemeSetStatus::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigRPMGaugeFixed mConfigRPMGaugeFixed = ConfigRPMGaugeFixed::OFF;
    ConfigTBT mConfigTBT = ConfigTBT::OFF;
    TbtDisplayTypeStatus mTbtDisplayTypeStatus = TbtDisplayTypeStatus::OFF;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;
    ConfigAdasLDW mConfigAdasLDW = ConfigAdasLDW::OFF;
    ConfigAdasBCW mConfigAdasBCW = ConfigAdasBCW::OFF;
    ConfigAdasBCWNear mConfigAdasBCWNear = ConfigAdasBCWNear::OFF;
    ConfigEDG mConfigEDG = ConfigEDG::OFF;
    ConfigTPMSCV mConfigTPMSCV = ConfigTPMSCV::OFF;
    ConfigAxleWeight mConfigAxleWeight = ConfigAxleWeight::OFF;
    EventCheckDisplayStatus mEventCheckDisplayStatus = EventCheckDisplayStatus::OFF;
    HUInt64 mPDC24VBatteryVoltageValue = 0;
    HUInt64 mPDC12VBatteryVoltageValue = 0;
    DisplayLanguageStatus mDisplayLanguageStatus = DisplayLanguageStatus::NONE;
    HeadUnitDisplayOnOffStatus mHeadUnitDisplayOnOffStatus = HeadUnitDisplayOnOffStatus::OFF;
    LWSPageStatus mLWSPageStatus = LWSPageStatus::OFF;
    SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat mIMG_ShootGaugeInfoEnable_stat =
        SFCScreen_Control_Logic_CVConstantSootGaugeInfoEnableStat::DISABLE;
    SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat mIMG_OilPressGaugeInfoEnable_stat =
        SFCScreen_Control_Logic_CVConstantOilPressGaugeInfoEnableStat::DISABLE;
    SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat mIMG_24VBatteryGauge_stat =
        SFCScreen_Control_Logic_CVConstantBatteryGauge24VStat::DISABLE;
    SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat mIMG_12VBatteryGauge_stat =
        SFCScreen_Control_Logic_CVConstantBatteryGauge12VStat::DISABLE;
};

}  // namespace ccos

#endif  // SFSS_Screen_Control_Logic_CV_H
