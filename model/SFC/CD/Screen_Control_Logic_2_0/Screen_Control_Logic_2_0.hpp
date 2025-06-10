/**
 * @file Screen_Control_Logic_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Screen_Control_Logic_2_0_H
#define SFSS_Screen_Control_Logic_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Screen_Control_Logic_2_0Base.hpp"

namespace ccos {

// SFC Version : 1.1.0
// Reference : [CD121] Screen_Control_Logic_2_0. Contains : Constant. Recommend: VALUE_CHANGED
class Screen_Control_Logic_2_0 : public Screen_Control_Logic_2_0Base {
public:
    Screen_Control_Logic_2_0() = default;
    ~Screen_Control_Logic_2_0() override = default;
    Screen_Control_Logic_2_0(const Screen_Control_Logic_2_0& other) = delete;
    Screen_Control_Logic_2_0(Screen_Control_Logic_2_0&& other) noexcept = delete;
    Screen_Control_Logic_2_0& operator=(const Screen_Control_Logic_2_0& other) = delete;
    Screen_Control_Logic_2_0& operator=(Screen_Control_Logic_2_0&& other) noexcept = delete;

    void onInitialize() override {
        initDisplayLanguageMap();
        updateAccumulScreenEnable();
        updateDriveInfoScreenEnable();
        updateAfterRefuelScreenEnable();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::ON_0ms;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsedStatus = IgnElapsed::OFF_0ms;
    }

    void onConfigISGChanged(const ArgumentsConfigISGChanged& args) {
        mConfigISG = args.mInter_ConfigISG;
        updateISGDetailScreen();
    }

    void onConfigSCRChanged(const ArgumentsConfigSCRChanged& args) {
        mConfigSCR = args.mInter_ConfigSCR;
        updateUreaLevelScreen();
    }

    void onConfigTPMSMonitorPageChanged(const ArgumentsConfigTPMSMonitorPageChanged& args) {
        mConfigTPMSMonitorPage = args.mInter_ConfigTPMSMonitorPage;
        updateTPMSPressureMonitorScreen();
    }

    void onDisplayLanguageStatusChanged(const ArgumentsDisplayLanguageStatusChanged& args) {
        mDisplayLanguageStatus = args.mInter_DisplayLanguageStatus;
        updateDisplayLanguageStat();
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
            {DisplayLanguageStatus::NONE, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::NONE},
            {DisplayLanguageStatus::KOREA, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::KOREA},
            {DisplayLanguageStatus::ENGLISH_US, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ENGLISH_US},
            {DisplayLanguageStatus::ENGLISH_UK, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ENGLISH_UK},
            {DisplayLanguageStatus::SIMPLIFIED_CHINESE,
             SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SIMPLIFIED_CHINESE},
            {DisplayLanguageStatus::TRADITIONAL_CHINESE,
             SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::TRADITIONAL_CHINESE},
            {DisplayLanguageStatus::FRENCH_EU, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::FRENCH_EU},
            {DisplayLanguageStatus::SPANISH_EU, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SPANISH_EU},
            {DisplayLanguageStatus::ARABIC, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ARABIC},
            {DisplayLanguageStatus::CZECH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::CZECH},
            {DisplayLanguageStatus::DANISH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::DANISH},
            {DisplayLanguageStatus::GERMAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::GERMAN},
            {DisplayLanguageStatus::ITALIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ITALIAN},
            {DisplayLanguageStatus::DUTCH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::DUTCH},
            {DisplayLanguageStatus::POLISH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::POLISH},
            {DisplayLanguageStatus::PORTUGUESE, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::PORTUGUESE},
            {DisplayLanguageStatus::RUSSIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::RUSSIAN},
            {DisplayLanguageStatus::SLOVAK, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SLOVAK},
            {DisplayLanguageStatus::SWEDISH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SWEDISH},
            {DisplayLanguageStatus::TURKISH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::TURKISH},
            {DisplayLanguageStatus::FRENCH_US, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::FRENCH_US},
            {DisplayLanguageStatus::SPANISH_US, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SPANISH_US},
            {DisplayLanguageStatus::ENGLISH_INDIA, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ENGLISH_INDIA},
            {DisplayLanguageStatus::NORWEGIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::NORWEGIAN},
            {DisplayLanguageStatus::FINNISH, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::FINNISH},
            {DisplayLanguageStatus::INDONESIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::INDONESIAN},
            {DisplayLanguageStatus::BRAZILIAN_PORTUGUESE,
             SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::BRAZILIAN_PORTUGUESE},
            {DisplayLanguageStatus::MALAY, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::MALAY},
            {DisplayLanguageStatus::HUNGARIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::HUNGARIAN},
            {DisplayLanguageStatus::ENGLISH_AUSTRALIA, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ENGLISH_AUSTRALIA},
            {DisplayLanguageStatus::JAPANESE, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::JAPANESE},
            {DisplayLanguageStatus::HINDI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::HINDI},
            {DisplayLanguageStatus::HEBREW, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::HEBREW},
            {DisplayLanguageStatus::GREEK, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::GREEK},
            {DisplayLanguageStatus::UKRAINIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::UKRAINIAN},
            {DisplayLanguageStatus::SLOVENIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::SLOVENIAN},
            {DisplayLanguageStatus::BULGARIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::BULGARIAN},
            {DisplayLanguageStatus::CROATIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::CROATIAN},
            {DisplayLanguageStatus::ROMANIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ROMANIAN},
            {DisplayLanguageStatus::BENGALI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::BENGALI},
            {DisplayLanguageStatus::MARATHI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::MARATHI},
            {DisplayLanguageStatus::TELUGU, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::TELUGU},
            {DisplayLanguageStatus::TAMIL, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::TAMIL},
            {DisplayLanguageStatus::GUJARATI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::GUJARATI},
            {DisplayLanguageStatus::KANNADA, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::KANNADA},
            {DisplayLanguageStatus::ODIA, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ODIA},
            {DisplayLanguageStatus::MALAYALAM, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::MALAYALAM},
            {DisplayLanguageStatus::PUNJABI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::PUNJABI},
            {DisplayLanguageStatus::INVALID, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::INVALID},
            {DisplayLanguageStatus::THAI, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::THAI},
            {DisplayLanguageStatus::LITHUANIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::LITHUANIAN},
            {DisplayLanguageStatus::LATVIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::LATVIAN},
            {DisplayLanguageStatus::ESTONIAN, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::ESTONIAN},
            {DisplayLanguageStatus::MAX, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat::MAX}};
    }

    void updateAccumulScreenEnable() {
        // 5.1.1 누적 정보
        setSFCScreen_Control_Logic_2_0ConstantAccumulScreenEnableStat(
            SFCScreen_Control_Logic_2_0ConstantAccumulScreenEnableStat::ENABLE);
    }

    void updateDriveInfoScreenEnable() {
        // 5.1.2 주행 정보
        setSFCScreen_Control_Logic_2_0ConstantDriveInfoScreenEnableStat(
            SFCScreen_Control_Logic_2_0ConstantDriveInfoScreenEnableStat::ENABLE);
    }

    void updateAfterRefuelScreenEnable() {
        // 5.1.3 주유후 정보
        setSFCScreen_Control_Logic_2_0ConstantAfterRefuelScreenEnableStat(
            SFCScreen_Control_Logic_2_0ConstantAfterRefuelScreenEnableStat::ENABLE);
    }

    void updateISGDetailScreen() {
        // 5.1.4 ISG 상태 정보
        if (mConfigISG == ConfigISG::ON) {
            setSFCScreen_Control_Logic_2_0ConstantISGDetailScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantISGDetailScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_Logic_2_0ConstantISGDetailScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantISGDetailScreenEnableStat::DISABLE);
        }
    }

    void updateUreaLevelScreen() {
        // 5.1.5 요소수 게이지
        if (mConfigSCR == ConfigSCR::ON) {
            setSFCScreen_Control_Logic_2_0ConstantUreaLevelScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantUreaLevelScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_Logic_2_0ConstantUreaLevelScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantUreaLevelScreenEnableStat::DISABLE);
        }
    }

    void updateTPMSPressureMonitorScreen() {
        // 5.1.6 TPMS 공기압 표시
        if (mConfigTPMSMonitorPage == ConfigTPMSMonitorPage::ON) {
            setSFCScreen_Control_Logic_2_0ConstantTPMSPressureMonitorScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantTPMSPressureMonitorScreenEnableStat::ENABLE);
        } else {
            setSFCScreen_Control_Logic_2_0ConstantTPMSPressureMonitorScreenEnableStat(
                SFCScreen_Control_Logic_2_0ConstantTPMSPressureMonitorScreenEnableStat::DISABLE);
        }
    }

    void updateDisplayLanguageStat() {
        // 5.2 표시 언어
        if (mDisplayLanguageMap.find(mDisplayLanguageStatus) != mDisplayLanguageMap.end()) {
            setSFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat(mDisplayLanguageMap[mDisplayLanguageStatus]);
        }
    }

    // 4.3.1 클러스터 영역의 테마 동작 : SFC 구현 X (별도 처리 로직이 없어 application 내 조치로 결정함, ESTRACK20-1821)
    // 5.3 클러스터 테마              : SFC 구현 X (별도 처리 로직이 없어 application 내 조치로 결정함, ESTRACK20-1821)

    IgnElapsed mIgnElapsedStatus = IgnElapsed::OFF_0ms;

    using ConfigISG = ArgumentsConfigISGChanged::Inter_ConfigISG;
    ConfigISG mConfigISG = ConfigISG::OFF;

    using ConfigSCR = ArgumentsConfigSCRChanged::Inter_ConfigSCR;
    ConfigSCR mConfigSCR = ConfigSCR::OFF;

    using ConfigTPMSMonitorPage = ArgumentsConfigTPMSMonitorPageChanged::Inter_ConfigTPMSMonitorPage;
    ConfigTPMSMonitorPage mConfigTPMSMonitorPage = ConfigTPMSMonitorPage::OFF;

    using DisplayLanguageStatus = ArgumentsDisplayLanguageStatusChanged::Inter_DisplayLanguageStatus;
    DisplayLanguageStatus mDisplayLanguageStatus = DisplayLanguageStatus::NONE;

    std::map<DisplayLanguageStatus, SFCScreen_Control_Logic_2_0ConstantDisplayLanguageStat> mDisplayLanguageMap;
};

}  // namespace ccos

#endif  // SFSS_Screen_Control_Logic_2_0_H
