/**
 * @file Screen_Control_Logic.hpp
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
#ifndef SFSS_Screen_Control_Logic_H
#define SFSS_Screen_Control_Logic_H

#define DLOG_ENABLED gEnableSFCLog

#include <map>

#include <Vehicle.h>
#include "Screen_Control_LogicBase.hpp"

namespace ccos {

// SFC Version : 41.0.0
// Reference : [CD120] Screen_Control_Logic. Contains : Constant. Recommend: VALUE_CHANGED
class Screen_Control_Logic : public Screen_Control_LogicBase {
public:
    Screen_Control_Logic() = default;
    ~Screen_Control_Logic() override = default;
    Screen_Control_Logic(const Screen_Control_Logic& other) = delete;
    Screen_Control_Logic(Screen_Control_Logic&& other) noexcept = delete;
    Screen_Control_Logic& operator=(const Screen_Control_Logic& other) = delete;
    Screen_Control_Logic& operator=(Screen_Control_Logic&& other) noexcept = delete;

    void onInitialize() override {
        initDisplayLanguageMap();
        updateAccumulScreenEnable();
        updateDriveInfoScreenEnable();
        updateAfterRefuelScreenEnable();
        updateRefSpeedometerScreenEnable();
        updateDisplayTheme();
        updateCurrentDisplayTheme();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::ON_0ms;
        updateDrivingAssistSystemOnOff();
        updateLapTimerActiveEventOnStatus();
        updateHdpSystemOnOffStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::OFF_0ms;
        updateHeadUnitDisplayOnOffStatus();
    }

    void onConfigEawdHevChanged(const ArgumentsConfigEawdHevChanged& args) {
        mConfigEawdHev = args.mInter_ConfigEawdHev;
        updateEnergyFlowScreen();
    }

    void onConfigBrandChanged(const ArgumentsConfigBrandChanged& args) {
        mConfigBrand = args.mInter_ConfigBrand;
        updateHighPerformanceScreenEnable();
    }

    void onConfigTBTChanged(const ArgumentsConfigTBTChanged& args) {
        mConfigTBT = args.mInter_ConfigTBT;
        updateTBTGuideScreen();
    }

    void onTbtDisplayTypeStatusChanged(const ArgumentsTbtDisplayTypeStatusChanged& args) {
        mTbtDisplayTypeStatus = args.mInput_TbtDisplayTypeStatus;
        updateTBTDestinationScreen();
    }

    void onPredictShiftChanged(const ArgumentsPredictShiftChanged& args) {
        mConfigPredictShift = args.mInter_ConfigPredictShift;
        updatePredictShiftScreenEnable();
    }

    void onEventCheckDisplayStatusChanged(const ArgumentsEventCheckDisplayStatusChanged& args) {
        mEventCheckDisplayStatus = args.mInter_EventCheckDisplayStatus;
        updateWarningScreen();
    }

    void onConfigCoolantTempGaugeFixedChanged(const ArgumentsConfigCoolantTempGaugeFixedChanged& args) {
        mConfigCoolantTempGaugeFixed = args.mInter_ConfigCoolantTempGaugeFixed;
        updateCoolantTempScreen();
    }

    void onConfigSCRChanged(const ArgumentsConfigSCRChanged& args) {
        mConfigSCR = args.mInter_ConfigSCR;
        updateUreaLevelScreen();
    }

    void onConfigTPMSMonitorPageChanged(const ArgumentsConfigTPMSMonitorPageChanged& args) {
        mConfigTPMSMonitorPage = args.mInter_ConfigTPMSMonitorPage;
        updateTPMSPressureMonitorScreen();
    }

    void onConfigTPMSSavePageChanged(const ArgumentsConfigTPMSSavePageChanged& args) {
        mConfigTPMSSavePage = args.mInter_ConfigTPMSSavePage;
        updateTPMSPressureSaveScreen();
    }

    void onConfigISGChanged(const ArgumentsConfigISGChanged& args) {
        mConfigISG = args.mInter_ConfigISG;
        updateISGDetailScreen();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEnergyFlowScreen();
        updateForceDistributeScreen();
        updateEVForceDistributeScreen();
        updateDteTotalScreenEnable();
        updateRPMGaugeDisplay();
        updateHighPerformanceScreenEnable();
        updateNBrandScreenEnable();
        updateOffRoadContentsForEngScreenEnalbe();
    }

    void onConfig4WDChanged(const ArgumentsConfig4WDChanged& args) {
        mConfig4WD = args.mInter_Config4WD;
        updateForceDistributeScreen();
        updateEVForceDistributeScreen();
        updateNBrandScreenEnable();
    }

    void onConfigOffRoadContentsChanged(const ArgumentsConfigOffRoadContentsChanged& args) {
        mConfigOffRoadContents = args.mInter_ConfigOffRoadContents;
        updateOffRoadContentsForEngScreenEnalbe();
    }

    void onDisplayThemeStatusChanged(const ArgumentsDisplayThemeStatusChanged& args) {
        mClusterThemeSetStatus = args.mInter_ClusterThemeSetStatus;
        mClusterThemeDetailSetStatus = args.mInter_ClusterThemeDetailSetStatus;
        mDriveModeSelectStatus = args.mInter_DriveModeSelectStatus;
        mTerrainModeSelectStatus = args.mInter_TerrainModeSelectStatus;
        updateRPMGaugeDisplay();
        updateDisplayTheme();
        updateCurrentDisplayTheme();
    }

    void onNModeSelectStatusChanged(const ArgumentsNModeSelectStatusChanged& args) {
        mNModeSelectStatus = args.mInter_NModeSelectStatus;
        updateCurrentDisplayTheme();
    }

    void onDriveAssistSystemChanged(const ArgumentsDriveAssistSystemChanged& args) {
        mConfigAdasSCC = args.mInter_ConfigAdasSCC;
        mConfigNewBCW = args.mInter_ConfigNewBCW;
        mConfigAdasLKA = args.mInter_ConfigAdasLKA;
        updateDriverAssistScreen();
    }

    void onRpmGaugeChanged(const ArgumentsRpmGaugeChanged& args) {
        mConfigRPMGaugeFixed = args.mInter_ConfigRPMGaugeFixed;
        mRpmGaugeNormalModeSetStatus = args.mInter_RpmGaugeNormalModeSetStatus;
        mRpmGaugeMyDriveModeSetStatus = args.mInter_RpmGaugeMyDriveModeSetStatus;
        mRpmGaugeSmartModeSetStatus = args.mInter_RpmGaugeSmartModeSetStatus;
        mRpmGaugeEcoModeSetStatus = args.mInter_RpmGaugeEcoModeSetStatus;
        mRpmGaugeSportModeSetStatus = args.mInter_RpmGaugeSportModeSetStatus;
        mRpmGaugeTerrainAutoModeSetStatus = args.mInter_RpmGaugeTerrainAutoModeSetStatus;
        mRpmGaugeMudModeSetStatus = args.mInter_RpmGaugeMudModeSetStatus;
        mRpmGaugeSnowModeSetStatus = args.mInter_RpmGaugeSnowModeSetStatus;
        mRpmGaugeSandModeSetStatus = args.mInter_RpmGaugeSandModeSetStatus;
        updateRPMGaugeDisplay();
    }

    void onDisplayLanguageStatusChanged(const ArgumentsDisplayLanguageStatusChanged& args) {
        mDisplayLanguageStatus = args.mInter_DisplayLanguageStatus;
        updateDisplayLanguageStat();
    }

    void onConfigHighPerformanceChanged(const ArgumentsConfigHighPerformanceChanged& args) {
        mConfigHighPerformance = args.mInter_ConfigHighPerformance;
        updateHighPerformanceScreenEnable();
        updateDisplayTheme();
        updateCurrentDisplayTheme();
        updateDragTimerScreenEnable();
        updateNBrandScreenEnable();
        updateGforceScreenEnable();
        updateLapTimerScreenEnable();
    }

    void onConfigTrailerBrakeControllerChanged(const ArgumentsConfigTrailerBrakeControllerChanged& args) {
        mConfigTrailerBrakeController = args.mInter_ConfigTrailerBrakeController;
        updateTrailerBrakeControllerEnableStat();
    }

    void onAdasOnStatusChanged(const ArgumentsAdasOnStatusChanged& args) {
        mAdasOnStatus = args.mInter_AdasOnStatus;
        updateDrivingAssistSystemOnOff();
    }

    void onLapTimerActiveEventOnStatusChanged(const ArgumentsLapTimerActiveEventOnStatusChanged& args) {
        mN1SwitchMainSetStatus = args.mInput_N1SwitchMainSetStatus;
        mN1SwitchStatus = args.mInput_N1SwitchStatus;
        mN2SwitchMainSetStatus = args.mInput_N2SwitchMainSetStatus;
        mN2SwitchStatus = args.mInput_N2SwitchStatus;
        updateLapTimerActiveEventOnStatus();
    }

    void onHdpSystemOnOffStatusChanged(const ArgumentsHdpSystemOnOffStatusChanged& args) {
        mAdasViewFixStatus = args.mInter_AdasViewFixStatus;
        updateHdpSystemOnOffStatus();
    }

    void onHeadUnitDisplayOnOffStatusChanged(const ArgumentsHeadUnitDisplayOnOffStatusChanged& args) {
        mHeadUnitDisplayOnOffStatus = args.mInput_HeadUnitDisplayOnOffStatus;
        updateHeadUnitDisplayOnOffStatus();
    }

    void onConfigEvDteGuideChanged(const ArgumentsConfigEvDteGuideChanged& args) {
        mConfigEvDteGuide = args.mInter_ConfigEvDteGuide;
        updateEvDteGuideScreenEnable();
    }

    void onConfigThemeUiChanged(const ArgumentsConfigThemeUiChanged& args) {
        mConfigThemeUI = args.mInter_ConfigThemeUI;
        updateDisplayTheme();
    }

    void onTelematicsWeatherTypeInfoChanged(const ArgumentsTelematicsWeatherTypeInfoChanged& args) {
        mTelematicsWeatherTypeInfo = args.mInput_TelematicsWeatherTypeInfo;
        DDebug() << "mTelematicsWeatherTypeInfo : " << static_cast<HUInt64>(mTelematicsWeatherTypeInfo);

        updateTelematicsWeatherTypeInfo();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    void initDisplayLanguageMap() {
        mDisplayLanguageMap = {
            {DisplayLanguageStatus::NONE, SFCScreen_Control_LogicConstantDisplayLanguageStat::NONE},
            {DisplayLanguageStatus::KOREA, SFCScreen_Control_LogicConstantDisplayLanguageStat::KOREA},
            {DisplayLanguageStatus::ENGLISH_US, SFCScreen_Control_LogicConstantDisplayLanguageStat::ENGLISH_US},
            {DisplayLanguageStatus::ENGLISH_UK, SFCScreen_Control_LogicConstantDisplayLanguageStat::ENGLISH_UK},
            {DisplayLanguageStatus::SIMPLIFIED_CHINESE, SFCScreen_Control_LogicConstantDisplayLanguageStat::SIMPLIFIED_CHINESE},
            {DisplayLanguageStatus::TRADITIONAL_CHINESE, SFCScreen_Control_LogicConstantDisplayLanguageStat::TRADITIONAL_CHINESE},
            {DisplayLanguageStatus::FRENCH_EU, SFCScreen_Control_LogicConstantDisplayLanguageStat::FRENCH_EU},
            {DisplayLanguageStatus::SPANISH_EU, SFCScreen_Control_LogicConstantDisplayLanguageStat::SPANISH_EU},
            {DisplayLanguageStatus::ARABIC, SFCScreen_Control_LogicConstantDisplayLanguageStat::ARABIC},
            {DisplayLanguageStatus::CZECH, SFCScreen_Control_LogicConstantDisplayLanguageStat::CZECH},
            {DisplayLanguageStatus::DANISH, SFCScreen_Control_LogicConstantDisplayLanguageStat::DANISH},
            {DisplayLanguageStatus::GERMAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::GERMAN},
            {DisplayLanguageStatus::ITALIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::ITALIAN},
            {DisplayLanguageStatus::DUTCH, SFCScreen_Control_LogicConstantDisplayLanguageStat::DUTCH},
            {DisplayLanguageStatus::POLISH, SFCScreen_Control_LogicConstantDisplayLanguageStat::POLISH},
            {DisplayLanguageStatus::PORTUGUESE, SFCScreen_Control_LogicConstantDisplayLanguageStat::PORTUGUESE},
            {DisplayLanguageStatus::RUSSIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::RUSSIAN},
            {DisplayLanguageStatus::SLOVAK, SFCScreen_Control_LogicConstantDisplayLanguageStat::SLOVAK},
            {DisplayLanguageStatus::SWEDISH, SFCScreen_Control_LogicConstantDisplayLanguageStat::SWEDISH},
            {DisplayLanguageStatus::TURKISH, SFCScreen_Control_LogicConstantDisplayLanguageStat::TURKISH},
            {DisplayLanguageStatus::FRENCH_US, SFCScreen_Control_LogicConstantDisplayLanguageStat::FRENCH_US},
            {DisplayLanguageStatus::SPANISH_US, SFCScreen_Control_LogicConstantDisplayLanguageStat::SPANISH_US},
            {DisplayLanguageStatus::ENGLISH_INDIA, SFCScreen_Control_LogicConstantDisplayLanguageStat::ENGLISH_INDIA},
            {DisplayLanguageStatus::NORWEGIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::NORWEGIAN},
            {DisplayLanguageStatus::FINNISH, SFCScreen_Control_LogicConstantDisplayLanguageStat::FINNISH},
            {DisplayLanguageStatus::INDONESIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::INDONESIAN},
            {DisplayLanguageStatus::BRAZILIAN_PORTUGUESE,
             SFCScreen_Control_LogicConstantDisplayLanguageStat::BRAZILIAN_PORTUGUESE},
            {DisplayLanguageStatus::MALAY, SFCScreen_Control_LogicConstantDisplayLanguageStat::MALAY},
            {DisplayLanguageStatus::HUNGARIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::HUNGARIAN},
            {DisplayLanguageStatus::ENGLISH_AUSTRALIA, SFCScreen_Control_LogicConstantDisplayLanguageStat::ENGLISH_AUSTRALIA},
            {DisplayLanguageStatus::JAPANESE, SFCScreen_Control_LogicConstantDisplayLanguageStat::JAPANESE},
            {DisplayLanguageStatus::HINDI, SFCScreen_Control_LogicConstantDisplayLanguageStat::HINDI},
            {DisplayLanguageStatus::HEBREW, SFCScreen_Control_LogicConstantDisplayLanguageStat::HEBREW},
            {DisplayLanguageStatus::GREEK, SFCScreen_Control_LogicConstantDisplayLanguageStat::GREEK},
            {DisplayLanguageStatus::UKRAINIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::UKRAINIAN},
            {DisplayLanguageStatus::SLOVENIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::SLOVENIAN},
            {DisplayLanguageStatus::BULGARIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::BULGARIAN},
            {DisplayLanguageStatus::CROATIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::CROATIAN},
            {DisplayLanguageStatus::ROMANIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::ROMANIAN},
            {DisplayLanguageStatus::BENGALI, SFCScreen_Control_LogicConstantDisplayLanguageStat::BENGALI},
            {DisplayLanguageStatus::MARATHI, SFCScreen_Control_LogicConstantDisplayLanguageStat::MARATHI},
            {DisplayLanguageStatus::TELUGU, SFCScreen_Control_LogicConstantDisplayLanguageStat::TELUGU},
            {DisplayLanguageStatus::TAMIL, SFCScreen_Control_LogicConstantDisplayLanguageStat::TAMIL},
            {DisplayLanguageStatus::GUJARATI, SFCScreen_Control_LogicConstantDisplayLanguageStat::GUJARATI},
            {DisplayLanguageStatus::KANNADA, SFCScreen_Control_LogicConstantDisplayLanguageStat::KANNADA},
            {DisplayLanguageStatus::ODIA, SFCScreen_Control_LogicConstantDisplayLanguageStat::ODIA},
            {DisplayLanguageStatus::MALAYALAM, SFCScreen_Control_LogicConstantDisplayLanguageStat::MALAYALAM},
            {DisplayLanguageStatus::PUNJABI, SFCScreen_Control_LogicConstantDisplayLanguageStat::PUNJABI},
            {DisplayLanguageStatus::INVALID, SFCScreen_Control_LogicConstantDisplayLanguageStat::INVALID},
            {DisplayLanguageStatus::THAI, SFCScreen_Control_LogicConstantDisplayLanguageStat::THAI},
            {DisplayLanguageStatus::LITHUANIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::LITHUANIAN},
            {DisplayLanguageStatus::LATVIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::LATVIAN},
            {DisplayLanguageStatus::ESTONIAN, SFCScreen_Control_LogicConstantDisplayLanguageStat::ESTONIAN},
            {DisplayLanguageStatus::MAX, SFCScreen_Control_LogicConstantDisplayLanguageStat::MAX}};
    }

    void updateAccumulScreenEnable() {
        // 5.1.1 누적 정보
        setSFCScreen_Control_LogicConstantAccumulScreenEnableStat(SFCScreen_Control_LogicConstantAccumulScreenEnableStat::ENABLE);
    }

    void updateDriveInfoScreenEnable() {
        // 5.1.2 주행 정보
        setSFCScreen_Control_LogicConstantDriveInfoScreenEnableStat(
            SFCScreen_Control_LogicConstantDriveInfoScreenEnableStat::ENABLE);
    }

    void updateAfterRefuelScreenEnable() {
        // 5.1.3 주유후 정보
        setSFCScreen_Control_LogicConstantAfterRefuelScreenEnableStat(
            SFCScreen_Control_LogicConstantAfterRefuelScreenEnableStat::ENABLE);
    }

    void updateISGDetailScreen() {
        // 5.1.4 ISG 상태 정보
        if (mConfigISG == ConfigISG::ON) {
            setSFCScreen_Control_LogicConstantISGDetailScreenEnableStat(
                SFCScreen_Control_LogicConstantISGDetailScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantISGDetailScreenEnableStat(
                SFCScreen_Control_LogicConstantISGDetailScreenEnableStat::DISABLE);
        }
    }

    void updateEnergyFlowScreen() {
        // 5.1.5 에너지 흐름도
        if ((mConfigVehicleType == ConfigVehicleType::FCEV) ||
            ((mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV) &&
             mConfigEawdHev == ConfigEawdHev::OFF)) {
            setSFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat(
                SFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat::ENABLE);
        } else if ((mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV) &&
                   mConfigEawdHev == ConfigEawdHev::ON) {
            setSFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat(
                SFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat::ENABLE_EAWD);
        } else {
            setSFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat(
                SFCScreen_Control_LogicConstantEnergyFlowScreenEnableStat::DISABLE);
        }
    }

    void updateTBTGuideScreen() {
        // 5.1.6 TBT 길안내
        if (mConfigTBT == ConfigTBT::ON) {
            setSFCScreen_Control_LogicConstantTBTGuideScreenEnableStat(
                SFCScreen_Control_LogicConstantTBTGuideScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantTBTGuideScreenEnableStat(
                SFCScreen_Control_LogicConstantTBTGuideScreenEnableStat::DISABLE);
        }
    }

    void updateTBTDestinationScreen() {
        // 5.1.7 TBT 목적지 정보
        switch (mTbtDisplayTypeStatus) {
            case TbtDisplayTypeStatus::OFF:
            case TbtDisplayTypeStatus::ARRIVED:
            case TbtDisplayTypeStatus::NO_ROUTE_GUIDANCE:
            case TbtDisplayTypeStatus::CALC_ROUTE:
            case TbtDisplayTypeStatus::RECALC_ROUTE:
                setSFCScreen_Control_LogicConstantTBTDestinationScreenEnableStat(
                    SFCScreen_Control_LogicConstantTBTDestinationScreenEnableStat::DISABLE);
                break;
            default:
                setSFCScreen_Control_LogicConstantTBTDestinationScreenEnableStat(
                    SFCScreen_Control_LogicConstantTBTDestinationScreenEnableStat::ENABLE);
                break;
        }
    }

    void updateDriverAssistScreen() {
        // 5.1.8 주행 보조 시스템
        if (mConfigAdasSCC == ConfigAdasSCC::ON || mConfigAdasLKA == ConfigAdasLKA::ON || mConfigNewBCW == ConfigNewBCW::ON) {
            setSFCScreen_Control_LogicConstantDriverAssistScreenEnableStat(
                SFCScreen_Control_LogicConstantDriverAssistScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantDriverAssistScreenEnableStat(
                SFCScreen_Control_LogicConstantDriverAssistScreenEnableStat::DISABLE);
        }
    }

    void updateWarningScreen() {
        // 5.1.9 고장 정보
        if (mEventCheckDisplayStatus == EventCheckDisplayStatus::ON) {
            setSFCScreen_Control_LogicConstantWarningScreenEnableStat(
                SFCScreen_Control_LogicConstantWarningScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantWarningScreenEnableStat(
                SFCScreen_Control_LogicConstantWarningScreenEnableStat::DISABLE);
        }
    }

    void updateRefSpeedometerScreenEnable() {
        // 5.1.10 참조 속도계
        setSFCScreen_Control_LogicConstantRefSpeedometerScreenEnableStat(
            SFCScreen_Control_LogicConstantRefSpeedometerScreenEnableStat::ENABLE);
    }

    void updateCoolantTempScreen() {
        // 5.1.11 냉각수 온도게이지
        if (mConfigCoolantTempGaugeFixed == ConfigCoolantTempGaugeFixed::ON) {
            setSFCScreen_Control_LogicConstantCoolantTempScreenEnableStat(
                SFCScreen_Control_LogicConstantCoolantTempScreenEnableStat::DISABLE);
        } else {
            setSFCScreen_Control_LogicConstantCoolantTempScreenEnableStat(
                SFCScreen_Control_LogicConstantCoolantTempScreenEnableStat::ENABLE);
        }
    }

    void updateUreaLevelScreen() {
        // 5.1.12 요소수 게이지
        if (mConfigSCR == ConfigSCR::ON) {
            setSFCScreen_Control_LogicConstantUreaLevelScreenEnableStat(
                SFCScreen_Control_LogicConstantUreaLevelScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantUreaLevelScreenEnableStat(
                SFCScreen_Control_LogicConstantUreaLevelScreenEnableStat::DISABLE);
        }
    }
    void updateForceDistributeScreen() {
        // 5.1.13 구동력 배분 정보
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfig4WD == Config4WD::ON) {
            setSFCScreen_Control_LogicConstantForceDistributeScreenEnableStat(
                SFCScreen_Control_LogicConstantForceDistributeScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantForceDistributeScreenEnableStat(
                SFCScreen_Control_LogicConstantForceDistributeScreenEnableStat::DISABLE);
        }
    }
    void updateEVForceDistributeScreen() {
        // 5.1.14 전기차 동력 배분 정보
        if (mConfigVehicleType == ConfigVehicleType::EV && mConfig4WD == Config4WD::ON) {
            setSFCScreen_Control_LogicConstantEVForceDistributeScreenEnableStat(
                SFCScreen_Control_LogicConstantEVForceDistributeScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantEVForceDistributeScreenEnableStat(
                SFCScreen_Control_LogicConstantEVForceDistributeScreenEnableStat::DISABLE);
        }
    }
    void updateTPMSPressureMonitorScreen() {
        // 5.1.15 TPMS 공기압 표시
        if (mConfigTPMSMonitorPage == ConfigTPMSMonitorPage::ON) {
            setSFCScreen_Control_LogicConstantTPMSPressureMonitorScreenEnableStat(
                SFCScreen_Control_LogicConstantTPMSPressureMonitorScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantTPMSPressureMonitorScreenEnableStat(
                SFCScreen_Control_LogicConstantTPMSPressureMonitorScreenEnableStat::DISABLE);
        }
    }
    void updateTPMSPressureSaveScreen() {
        // 5.1.16 TPMS 공기압 저장
        if (mConfigTPMSSavePage == ConfigTPMSSavePage::ON) {
            setSFCScreen_Control_LogicConstantTPMSPressureSaveScreenEnableStat(
                SFCScreen_Control_LogicConstantTPMSPressureSaveScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantTPMSPressureSaveScreenEnableStat(
                SFCScreen_Control_LogicConstantTPMSPressureSaveScreenEnableStat::DISABLE);
        }
    }

    void updateHighPerformanceScreenEnable() {
        // 5.1.17 고성능 컨텐츠
        if (mConfigBrand == ConfigBrand::HYUNDAI &&
            (mConfigHighPerformance == ConfigHighPerformance::HIGH_PERFORMANCE && mConfigVehicleType == ConfigVehicleType::ICV)) {
            setSFCScreen_Control_LogicConstantHighPerformanceScreenEnableStat(
                SFCScreen_Control_LogicConstantHighPerformanceScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantHighPerformanceScreenEnableStat(
                SFCScreen_Control_LogicConstantHighPerformanceScreenEnableStat::DISABLE);
        }
    }

    void updateNBrandScreenEnable() {
        // 5.1.18 N브랜드 전용 고성능 컨텐츠
        if (mConfigHighPerformance == ConfigHighPerformance::N && mConfigVehicleType == ConfigVehicleType::ICV) {
            setSFCScreen_Control_LogicConstantNBrandScreenEnableStat(
                SFCScreen_Control_LogicConstantNBrandScreenEnableStat::ENABLE_N_ICE);
        } else if (mConfigHighPerformance == ConfigHighPerformance::N && mConfigVehicleType == ConfigVehicleType::EV &&
                   mConfig4WD != Config4WD::ON) {
            setSFCScreen_Control_LogicConstantNBrandScreenEnableStat(
                SFCScreen_Control_LogicConstantNBrandScreenEnableStat::ENABLE_N_EV2WD);
        } else if (mConfigHighPerformance == ConfigHighPerformance::N && mConfigVehicleType == ConfigVehicleType::EV &&
                   mConfig4WD == Config4WD::ON) {
            setSFCScreen_Control_LogicConstantNBrandScreenEnableStat(
                SFCScreen_Control_LogicConstantNBrandScreenEnableStat::ENABLE_N_EV4WD);
        } else {
            setSFCScreen_Control_LogicConstantNBrandScreenEnableStat(
                SFCScreen_Control_LogicConstantNBrandScreenEnableStat::DISABLE);
        }
    }

    void updateDteTotalScreenEnable() {
        // 5.1.18 연료 및 전기 주행가능거리 총합
        if (mConfigVehicleType == ConfigVehicleType::PHEV) {
            setSFCScreen_Control_LogicConstantDteTotalScreenEnableStat(
                SFCScreen_Control_LogicConstantDteTotalScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantDteTotalScreenEnableStat(
                SFCScreen_Control_LogicConstantDteTotalScreenEnableStat::DISABLE);
        }
    }

    void updatePredictShiftScreenEnable() {
        // 5.1.19 Predictive Shift
        if (mConfigPredictShift == ConfigPredictShift::ON) {
            setSFCScreen_Control_LogicConstantPredictShiftScreenEnableStat(
                SFCScreen_Control_LogicConstantPredictShiftScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantPredictShiftScreenEnableStat(
                SFCScreen_Control_LogicConstantPredictShiftScreenEnableStat::DISABLE);
        }
    }

    void updateEvDteGuideScreenEnable() {
        // 5.1.20 DTE GUIDE
        if (mConfigEvDteGuide == ConfigEvDteGuide::ON) {
            setSFCScreen_Control_LogicConstantDteGuideScreenEnableStat(
                SFCScreen_Control_LogicConstantDteGuideScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantDteGuideScreenEnableStat(
                SFCScreen_Control_LogicConstantDteGuideScreenEnableStat::DISABLE);
        }
    }

    void updateDragTimerScreenEnable() {
        // 5.1.21 드래그 타이머
        if (mConfigHighPerformance == ConfigHighPerformance::GT) {
            setSFCScreen_Control_LogicConstantDragTimerScreenEnableStat(
                SFCScreen_Control_LogicConstantDragTimerScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantDragTimerScreenEnableStat(
                SFCScreen_Control_LogicConstantDragTimerScreenEnableStat::DISABLE);
        }
    }

    void updateOffRoadContentsForEngScreenEnalbe() {
        // 5.1.23 오프로드 전용 컨텐츠
        if ((mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
             mConfigVehicleType == ConfigVehicleType::PHEV) &&
            mConfigOffRoadContents == ConfigOffRoadContents::ON) {
            setSFCScreen_Control_LogicConstantOffRoadContentsForEngScreenEnableStat(
                SFCScreen_Control_LogicConstantOffRoadContentsForEngScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantOffRoadContentsForEngScreenEnableStat(
                SFCScreen_Control_LogicConstantOffRoadContentsForEngScreenEnableStat::DISABLE);
        }
    }

    void updateGforceScreenEnable() {
        // 5.1.24 G-Force
        if (mConfigHighPerformance == ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantGforceScreenEnableStat(
                SFCScreen_Control_LogicConstantGforceScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantGforceScreenEnableStat(
                SFCScreen_Control_LogicConstantGforceScreenEnableStat::DISABLE);
        }
    }

    void updateLapTimerScreenEnable() {
        // 5.1.25 랩타이머
        if (mConfigHighPerformance == ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantLapTimerScreenEnableStat(
                SFCScreen_Control_LogicConstantLapTimerScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantLapTimerScreenEnableStat(
                SFCScreen_Control_LogicConstantLapTimerScreenEnableStat::DISABLE);
        }
    }

    void updateTrailerBrakeControllerEnableStat() {
        // 5.1.26 Trailer Brake Controller(TBC)
        if (mConfigTrailerBrakeController == ConfigTrailerBrakeController::ON) {
            setSFCScreen_Control_LogicConstantTrailerBrakeControllerEnableStat(
                SFCScreen_Control_LogicConstantTrailerBrakeControllerEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantTrailerBrakeControllerEnableStat(
                SFCScreen_Control_LogicConstantTrailerBrakeControllerEnableStat::DISABLE);
        }
    }

    void updateRPMGaugeDisplay() {
        // 5.3 RPM게이지 표시 활성화
        HBool isEnable = false;
        if (mConfigRPMGaugeFixed == ConfigRPMGaugeFixed::OFF &&
            (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV)) {
            if (mRpmGaugeNormalModeSetStatus == RpmGaugeNormalModeSetStatus::ENABLE &&
                mDriveModeSelectStatus == DriveModeSelectStatus::NORMAL &&
                (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                 mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)) {
                isEnable = true;
            } else if (mRpmGaugeEcoModeSetStatus == RpmGaugeEcoModeSetStatus::ENABLE &&
                       mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                       (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                        mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)) {
                isEnable = true;
            } else if (mRpmGaugeSportModeSetStatus == RpmGaugeSportModeSetStatus::ENABLE &&
                       mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                       (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                        mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)) {
                isEnable = true;
            } else if (mRpmGaugeTerrainAutoModeSetStatus == RpmGaugeTerrainAutoModeSetStatus::ENABLE &&
                       mTerrainModeSelectStatus == TerrainModeSelectStatus::AUTO) {
                isEnable = true;
            } else if (mRpmGaugeSnowModeSetStatus == RpmGaugeSnowModeSetStatus::ENABLE &&
                       (mTerrainModeSelectStatus == TerrainModeSelectStatus::SNOW ||
                        (mDriveModeSelectStatus == DriveModeSelectStatus::SNOW &&
                         (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                          mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)))) {
                isEnable = true;
            } else if (mRpmGaugeMudModeSetStatus == RpmGaugeMudModeSetStatus::ENABLE &&
                       mTerrainModeSelectStatus == TerrainModeSelectStatus::MUD) {
                isEnable = true;
            } else if (mRpmGaugeSandModeSetStatus == RpmGaugeSandModeSetStatus::ENABLE &&
                       mTerrainModeSelectStatus == TerrainModeSelectStatus::SAND) {
                isEnable = true;
            } else if (mRpmGaugeSmartModeSetStatus == RpmGaugeSmartModeSetStatus::ENABLE &&
                       mDriveModeSelectStatus == DriveModeSelectStatus::SMART &&
                       (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                        mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)) {
                isEnable = true;
            } else if (mRpmGaugeMyDriveModeSetStatus == RpmGaugeMyDriveModeSetStatus::ENABLE &&
                       mDriveModeSelectStatus == DriveModeSelectStatus::MY_DRIVE &&
                       (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                        mTerrainModeSelectStatus == TerrainModeSelectStatus::READY)) {
                isEnable = true;
            } else {
                // do nothing
            }
        } else if (mConfigRPMGaugeFixed == ConfigRPMGaugeFixed::OFF && mConfigVehicleType == ConfigVehicleType::ICV) {
            isEnable = true;
        } else {
            // do nothing
        }
        if (isEnable == true) {
            setSFCScreen_Control_LogicConstantRPMGaugeDisplayEnableStat(
                SFCScreen_Control_LogicConstantRPMGaugeDisplayEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_LogicConstantRPMGaugeDisplayEnableStat(
                SFCScreen_Control_LogicConstantRPMGaugeDisplayEnableStat::DISABLE);
        }
    }

    void updateDisplayLanguageStat() {
        // 5.4 표시 언어
        if (mDisplayLanguageMap.find(mDisplayLanguageStatus) != mDisplayLanguageMap.end()) {
            setSFCScreen_Control_LogicConstantDisplayLanguageStat(mDisplayLanguageMap[mDisplayLanguageStatus]);
        }
    }

    // 4.3 그래픽 테마 동작 로직 & 5.5 표시 테마
    void updateDisplayTheme() {
        if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT1 &&
            mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   !(mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                     mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   (mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                    mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   !(mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                     mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT2 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT3 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_A) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_A_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT1 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   !(mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                     mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   (mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                    mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   !(mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                     mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT2 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT3 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_B) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_B_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT1 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   !(mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                     mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   (mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                    mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   !(mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                     mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT2 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT3 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT1 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   !(mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                     mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   (mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                    mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   !(mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                     mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT2 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT3 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance != ConfigHighPerformance::N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_D_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_SPORT;
        } else if (mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI != ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(
                SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_HYUNDAI_NTHEME);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(
                SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_HYUNDAI_NTHEME);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_HYUNDAI_NTHEME;
        } else if (mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_D &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(
                SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_HYUNDAI_NTHEME);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(
                SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_HYUNDAI_NTHEME);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_HYUNDAI_NTHEME;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT1 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus != DriveModeSelectStatus::ECO &&
                   mDriveModeSelectStatus != DriveModeSelectStatus::SPORT &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   (mDriveModeSelectStatus == DriveModeSelectStatus::ECO ||
                    mDriveModeSelectStatus == DriveModeSelectStatus::SPORT) &&
                   mTerrainModeSelectStatus != TerrainModeSelectStatus::OFF &&
                   mTerrainModeSelectStatus != TerrainModeSelectStatus::READY &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_NORMAL);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT2 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::ECO &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_ECO);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::SELECT3 &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT;
        } else if (mClusterThemeDetailSetStatus == ClusterThemeDetailSetStatus::AUTO &&
                   mDriveModeSelectStatus == DriveModeSelectStatus::SPORT &&
                   (mTerrainModeSelectStatus == TerrainModeSelectStatus::OFF ||
                    mTerrainModeSelectStatus == TerrainModeSelectStatus::READY) &&
                   mClusterThemeSetStatus == ClusterThemeSetStatus::THEME_C &&
                   mConfigHighPerformance == ConfigHighPerformance::N &&
                   mConfigThemeUI == ConfigThemeUI::SIMPLE_MODERN_NEOCLASSIC_N) {
            setSFCScreen_Control_LogicConstantDisplayThemeStat(SFCScreen_Control_LogicConstantDisplayThemeStat::THEME_C_SPORT);
            setSFCScreen_Control_LogicInter_DisplayThemeStatus(SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT);
            mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT;
        } else {
            // do nothing
        }
    }

    // 5.5.2 클러스터 현재 표시 테마(화면 표시 기준)
    void updateCurrentDisplayTheme() {
        if (mNModeSelectStatus == NModeSelectStatus::N || mNModeSelectStatus == NModeSelectStatus::CUSTOM1 ||
            mNModeSelectStatus == NModeSelectStatus::CUSTOM2) {
            setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_HYUNDAI_NTHEME);
        } else {
            switch (mDisplayThemeStatus) {
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_HYUNDAI_NTHEME:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_HYUNDAI_NTHEME);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_NORMAL:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_A_NORMAL);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_ECO:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_A_ECO);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_A_SPORT:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_A_SPORT);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_NORMAL:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_B_NORMAL);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_ECO:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_B_ECO);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_B_SPORT:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_B_SPORT);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_NORMAL:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_C_NORMAL);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_ECO:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_C_ECO);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_C_SPORT:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_C_SPORT);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_NORMAL:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_D_NORMAL);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_ECO:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_D_ECO);
                    break;
                case SFCScreen_Control_LogicInter_DisplayThemeStatus::THEME_D_SPORT:
                    setSFCScreen_Control_LogicConstantCurrentDisplayThemeStat(
                        SFCScreen_Control_LogicConstantCurrentDisplayThemeStat::THEME_D_SPORT);
                    break;
                default:
                    // do nothing
                    break;
            }
        }
    }

    void updateDrivingAssistSystemOnOff() {
        // 5.6 ADAS 시스템 동작 여부
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mAdasOnStatus == AdasOnStatus::ON) {
                setSFCScreen_Control_LogicConstantDrivingAssistSystemOnOffStat(
                    SFCScreen_Control_LogicConstantDrivingAssistSystemOnOffStat::ON);
            } else {
                setSFCScreen_Control_LogicConstantDrivingAssistSystemOnOffStat(
                    SFCScreen_Control_LogicConstantDrivingAssistSystemOnOffStat::OFF);
            }
        }
    }

    void updateLapTimerActiveEventOnStatus() {
        static N1SwitchStatus prevN1SwitchStatus = N1SwitchStatus::DEFAULT;
        static N2SwitchStatus prevN2SwitchStatus = N2SwitchStatus::DEFAULT;
        SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat stat =
            SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat::DEFAULT;

        // Apply Power Condition(IGN ON) - [EXNCP-25739]
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mN1SwitchMainSetStatus == N1SwitchMainSetStatus::LAP_TIMER_START &&
                prevN1SwitchStatus == N1SwitchStatus::DEFAULT && mN1SwitchStatus == N1SwitchStatus::PRESSED) {
                stat = SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat::ON_EVENT;
            } else if (mN2SwitchMainSetStatus == N2SwitchMainSetStatus::LAP_TIMER_START &&
                       prevN2SwitchStatus == N2SwitchStatus::DEFAULT && mN2SwitchStatus == N2SwitchStatus::PRESSED) {
                stat = SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat::ON_EVENT;
            } else if (mN1SwitchMainSetStatus == N1SwitchMainSetStatus::LAP_TIMER_STOP &&
                       prevN1SwitchStatus == N1SwitchStatus::DEFAULT && mN1SwitchStatus == N1SwitchStatus::PRESSED) {
                stat = SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat::ON_EVENT;
            } else if (mN2SwitchMainSetStatus == N2SwitchMainSetStatus::LAP_TIMER_STOP &&
                       prevN2SwitchStatus == N2SwitchStatus::DEFAULT && mN2SwitchStatus == N2SwitchStatus::PRESSED) {
                stat = SFCScreen_Control_LogicConstantLapTimerActiveEventOnStat::ON_EVENT;
            } else {
                // fallback condition: stat ==> DEFAULT
            }
            setSFCScreen_Control_LogicConstantLapTimerActiveEventOnStat(stat);
        }
        prevN1SwitchStatus = mN1SwitchStatus;
        prevN2SwitchStatus = mN2SwitchStatus;
    }

    void updateHdpSystemOnOffStatus() {
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mAdasViewFixStatus == AdasViewFixStatus::FIX) {
                setSFCScreen_Control_LogicConstantHDPSystemOnOffStat(SFCScreen_Control_LogicConstantHDPSystemOnOffStat::ON);
            } else {
                setSFCScreen_Control_LogicConstantHDPSystemOnOffStat(SFCScreen_Control_LogicConstantHDPSystemOnOffStat::OFF);
            }
        }
    }

    void updateHeadUnitDisplayOnOffStatus() {
        if (mIgnElapsedStatus == IgnElapsed::OFF_0ms) {
            if (mHeadUnitDisplayOnOffStatus == HeadUnitDisplayOnOffStatus::ON) {
                setSFCScreen_Control_LogicConstantAVNDisplayOnOffStat(SFCScreen_Control_LogicConstantAVNDisplayOnOffStat::ON);
            } else {
                setSFCScreen_Control_LogicConstantAVNDisplayOnOffStat(SFCScreen_Control_LogicConstantAVNDisplayOnOffStat::OFF);
            }
        }
    }

    void updateTelematicsWeatherTypeInfo() {
        // 5.9 날씨 효과
        SFCScreen_Control_LogicConstantWeatherTypeStat stat = SFCScreen_Control_LogicConstantWeatherTypeStat::DEFAULT;
        if (mIgnElapsedStatus == IgnElapsed::ON_0ms) {
            if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::CLOUDY) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::CLOUDY;
            } else if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::RAIN) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::RAIN;
            } else if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::SNOW) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::SNOW;
            } else if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::FOG) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::FOG;
            } else if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::THUNDER) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::THUNDER;
            } else if (mTelematicsWeatherTypeInfo == TelematicsWeatherTypeInfo::CLEAR_UP) {
                stat = SFCScreen_Control_LogicConstantWeatherTypeStat::CLEAR_UP;
            } else {
                // no operation
            }
        }
        setSFCScreen_Control_LogicConstantWeatherTypeStat(stat);
    }

    IgnElapsed mIgnElapsedStatus = IgnElapsed::OFF_0ms;

    using ConfigEawdHev = ArgumentsConfigEawdHevChanged::Inter_ConfigEawdHev;
    ConfigEawdHev mConfigEawdHev = ConfigEawdHev::OFF;

    using ConfigBrand = ArgumentsConfigBrandChanged::Inter_ConfigBrand;
    ConfigBrand mConfigBrand = ConfigBrand::NONE;

    using ConfigISG = ArgumentsConfigISGChanged::Inter_ConfigISG;
    ConfigISG mConfigISG = ConfigISG::OFF;

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;

    using Config4WD = ArgumentsConfig4WDChanged::Inter_Config4WD;
    Config4WD mConfig4WD = Config4WD::OFF;

    using ConfigOffRoadContents = ArgumentsConfigOffRoadContentsChanged::Inter_ConfigOffRoadContents;
    ConfigOffRoadContents mConfigOffRoadContents = ConfigOffRoadContents::OFF;

    using ConfigTBT = ArgumentsConfigTBTChanged::Inter_ConfigTBT;
    ConfigTBT mConfigTBT = ConfigTBT::OFF;

    using TbtDisplayTypeStatus = ArgumentsTbtDisplayTypeStatusChanged::Input_TbtDisplayTypeStatus;
    TbtDisplayTypeStatus mTbtDisplayTypeStatus = TbtDisplayTypeStatus::OFF;

    using ConfigNewBCW = ArgumentsDriveAssistSystemChanged::Inter_ConfigNewBCW;
    ConfigNewBCW mConfigNewBCW = ConfigNewBCW::OFF;

    using ConfigAdasSCC = ArgumentsDriveAssistSystemChanged::Inter_ConfigAdasSCC;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;

    using ConfigAdasLKA = ArgumentsDriveAssistSystemChanged::Inter_ConfigAdasLKA;
    ConfigAdasLKA mConfigAdasLKA = ConfigAdasLKA::OFF;

    using ConfigPredictShift = ArgumentsPredictShiftChanged::Inter_ConfigPredictShift;
    ConfigPredictShift mConfigPredictShift = ConfigPredictShift::OFF;

    using EventCheckDisplayStatus = ArgumentsEventCheckDisplayStatusChanged::Inter_EventCheckDisplayStatus;
    EventCheckDisplayStatus mEventCheckDisplayStatus = EventCheckDisplayStatus::NONE;

    using ConfigCoolantTempGaugeFixed = ArgumentsConfigCoolantTempGaugeFixedChanged::Inter_ConfigCoolantTempGaugeFixed;
    ConfigCoolantTempGaugeFixed mConfigCoolantTempGaugeFixed = ConfigCoolantTempGaugeFixed::OFF;

    using ConfigSCR = ArgumentsConfigSCRChanged::Inter_ConfigSCR;
    ConfigSCR mConfigSCR = ConfigSCR::OFF;

    using ConfigTPMSMonitorPage = ArgumentsConfigTPMSMonitorPageChanged::Inter_ConfigTPMSMonitorPage;
    ConfigTPMSMonitorPage mConfigTPMSMonitorPage = ConfigTPMSMonitorPage::OFF;

    using ConfigTPMSSavePage = ArgumentsConfigTPMSSavePageChanged::Inter_ConfigTPMSSavePage;
    ConfigTPMSSavePage mConfigTPMSSavePage = ConfigTPMSSavePage::OFF;

    using ConfigRPMGaugeFixed = ArgumentsRpmGaugeChanged::Inter_ConfigRPMGaugeFixed;
    ConfigRPMGaugeFixed mConfigRPMGaugeFixed = ConfigRPMGaugeFixed::OFF;

    using RpmGaugeNormalModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeNormalModeSetStatus;
    RpmGaugeNormalModeSetStatus mRpmGaugeNormalModeSetStatus = RpmGaugeNormalModeSetStatus::NONE;

    using RpmGaugeMyDriveModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeMyDriveModeSetStatus;
    RpmGaugeMyDriveModeSetStatus mRpmGaugeMyDriveModeSetStatus = RpmGaugeMyDriveModeSetStatus::NONE;

    using RpmGaugeSmartModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeSmartModeSetStatus;
    RpmGaugeSmartModeSetStatus mRpmGaugeSmartModeSetStatus = RpmGaugeSmartModeSetStatus::NONE;

    using RpmGaugeEcoModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeEcoModeSetStatus;
    RpmGaugeEcoModeSetStatus mRpmGaugeEcoModeSetStatus = RpmGaugeEcoModeSetStatus::NONE;

    using RpmGaugeSportModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeSportModeSetStatus;
    RpmGaugeSportModeSetStatus mRpmGaugeSportModeSetStatus = RpmGaugeSportModeSetStatus::NONE;

    using RpmGaugeTerrainAutoModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeTerrainAutoModeSetStatus;
    RpmGaugeTerrainAutoModeSetStatus mRpmGaugeTerrainAutoModeSetStatus = RpmGaugeTerrainAutoModeSetStatus::NONE;

    using RpmGaugeSnowModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeSnowModeSetStatus;
    RpmGaugeSnowModeSetStatus mRpmGaugeSnowModeSetStatus = RpmGaugeSnowModeSetStatus::NONE;

    using RpmGaugeMudModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeMudModeSetStatus;
    RpmGaugeMudModeSetStatus mRpmGaugeMudModeSetStatus = RpmGaugeMudModeSetStatus::NONE;

    using RpmGaugeSandModeSetStatus = ArgumentsRpmGaugeChanged::Inter_RpmGaugeSandModeSetStatus;
    RpmGaugeSandModeSetStatus mRpmGaugeSandModeSetStatus = RpmGaugeSandModeSetStatus::NONE;

    using ClusterThemeSetStatus = ArgumentsDisplayThemeStatusChanged::Inter_ClusterThemeSetStatus;
    ClusterThemeSetStatus mClusterThemeSetStatus = ClusterThemeSetStatus::NONE;

    using ClusterThemeDetailSetStatus = ArgumentsDisplayThemeStatusChanged::Inter_ClusterThemeDetailSetStatus;
    ClusterThemeDetailSetStatus mClusterThemeDetailSetStatus = ClusterThemeDetailSetStatus::NONE;

    using DriveModeSelectStatus = ArgumentsDisplayThemeStatusChanged::Inter_DriveModeSelectStatus;
    DriveModeSelectStatus mDriveModeSelectStatus = DriveModeSelectStatus::NONE;

    using TerrainModeSelectStatus = ArgumentsDisplayThemeStatusChanged::Inter_TerrainModeSelectStatus;
    TerrainModeSelectStatus mTerrainModeSelectStatus = TerrainModeSelectStatus::NONE;

    using DisplayLanguageStatus = ArgumentsDisplayLanguageStatusChanged::Inter_DisplayLanguageStatus;
    DisplayLanguageStatus mDisplayLanguageStatus = DisplayLanguageStatus::NONE;

    std::map<DisplayLanguageStatus, SFCScreen_Control_LogicConstantDisplayLanguageStat> mDisplayLanguageMap;

    using AdasOnStatus = ArgumentsAdasOnStatusChanged::Inter_AdasOnStatus;
    AdasOnStatus mAdasOnStatus = AdasOnStatus::NONE;

    using ConfigHighPerformance = ArgumentsConfigHighPerformanceChanged::Inter_ConfigHighPerformance;
    ConfigHighPerformance mConfigHighPerformance = ConfigHighPerformance::OFF;

    using ConfigTrailerBrakeController = ArgumentsConfigTrailerBrakeControllerChanged::Inter_ConfigTrailerBrakeController;
    ConfigTrailerBrakeController mConfigTrailerBrakeController = ConfigTrailerBrakeController::OFF;

    using N1SwitchMainSetStatus = ArgumentsLapTimerActiveEventOnStatusChanged::Input_N1SwitchMainSetStatus;
    N1SwitchMainSetStatus mN1SwitchMainSetStatus = N1SwitchMainSetStatus::INVALID;

    using N1SwitchStatus = ArgumentsLapTimerActiveEventOnStatusChanged::Input_N1SwitchStatus;
    N1SwitchStatus mN1SwitchStatus = N1SwitchStatus::DEFAULT;

    using N2SwitchMainSetStatus = ArgumentsLapTimerActiveEventOnStatusChanged::Input_N2SwitchMainSetStatus;
    N2SwitchMainSetStatus mN2SwitchMainSetStatus = N2SwitchMainSetStatus::INVALID;

    using N2SwitchStatus = ArgumentsLapTimerActiveEventOnStatusChanged::Input_N2SwitchStatus;
    N2SwitchStatus mN2SwitchStatus = N2SwitchStatus::DEFAULT;

    using AdasViewFixStatus = ArgumentsHdpSystemOnOffStatusChanged::Inter_AdasViewFixStatus;
    AdasViewFixStatus mAdasViewFixStatus = AdasViewFixStatus::NONE;

    using HeadUnitDisplayOnOffStatus = ArgumentsHeadUnitDisplayOnOffStatusChanged::Input_HeadUnitDisplayOnOffStatus;
    HeadUnitDisplayOnOffStatus mHeadUnitDisplayOnOffStatus = HeadUnitDisplayOnOffStatus::OFF;

    using ConfigEvDteGuide = ArgumentsConfigEvDteGuideChanged::Inter_ConfigEvDteGuide;
    ConfigEvDteGuide mConfigEvDteGuide = ConfigEvDteGuide::OFF;

    using NModeSelectStatus = ArgumentsNModeSelectStatusChanged::Inter_NModeSelectStatus;
    NModeSelectStatus mNModeSelectStatus = NModeSelectStatus::NONE;

    using ConfigThemeUI = ArgumentsConfigThemeUiChanged::Inter_ConfigThemeUI;
    ConfigThemeUI mConfigThemeUI = ConfigThemeUI::CLASSIC_SIMPLE;

    using TelematicsWeatherTypeInfo = ArgumentsTelematicsWeatherTypeInfoChanged::Input_TelematicsWeatherTypeInfo;
    TelematicsWeatherTypeInfo mTelematicsWeatherTypeInfo = TelematicsWeatherTypeInfo::INVALID;

    SFCScreen_Control_LogicInter_DisplayThemeStatus mDisplayThemeStatus = SFCScreen_Control_LogicInter_DisplayThemeStatus::NONE;
};

}  // namespace ccos

#endif  // SFSS_Screen_Control_Logic_H
