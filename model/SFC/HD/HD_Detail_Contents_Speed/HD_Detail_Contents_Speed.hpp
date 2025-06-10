/**
 * @file HD_Detail_Contents_Speed.hpp
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
#ifndef SFSS_HD_Detail_Contents_Speed_H
#define SFSS_HD_Detail_Contents_Speed_H

#define DLOG_ENABLED gEnableSFCLog

#include "HD_Detail_Contents_SpeedBase.hpp"

namespace ccos {

// SFC Version : 5.3.1
// Reference : [HD100] Detail Contents_Speed. Contains : Constant. Recommend: VALUE_CHANGED
class HD_Detail_Contents_Speed : public HD_Detail_Contents_SpeedBase {
public:
    HD_Detail_Contents_Speed() = default;
    ~HD_Detail_Contents_Speed() override = default;
    HD_Detail_Contents_Speed(const HD_Detail_Contents_Speed& other) = delete;
    HD_Detail_Contents_Speed(HD_Detail_Contents_Speed&& other) noexcept = delete;
    HD_Detail_Contents_Speed& operator=(const HD_Detail_Contents_Speed& other) = delete;
    HD_Detail_Contents_Speed& operator=(HD_Detail_Contents_Speed&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        calculateSpdColor();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
    }

    void onNaviOverSpeedColorChanged(const ArgumentsNaviOverSpeedColorChanged& args) {
        mNaviOverSpeedColor1 = args.mInput_NaviOverSpeedColor1;
        mNaviOverSpeedColor2 = args.mInput_NaviOverSpeedColor2;
        calculateSpdColor();
    }

    void onUnitChanged(const ArgumentsUnitChanged& args) {
        mNaviOverSpdUnit = args.mInput_NaviOverSpdUnit;
        mDisplaySpeedUnit = args.mInter_DisplaySpeedUnit;
        updateHDVehicleSpeedMainDisplayUnitStat();
        calculateSpdColor();
    }

    void onNaviOverSpeedRangeChanged(const ArgumentsNaviOverSpeedRangeChanged& args) {
        mNaviOverSpeedRange1 = args.mInput_NaviOverSpeedRange1;
        mNaviOverSpeedRange2 = args.mInput_NaviOverSpeedRange2;
        calculateSpdColor();
    }

    void onDisplaySpeedChanged(const ArgumentsDisplaySpeedChanged& args) {
        updateValueIfValid(mSpeedDigitalValue, args.mSpeedDigitalValue);
        updateValueIfValid(mSpeedDigitalStat, args.mSpeedDigitalStat);

        calculateSpdColor();
        updateSpeedValue();
        updateSpeedStat();
    }

    void onConfigHUDChanged(const ArgumentsConfigHUDChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        calculateSpdColor();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void calculateSpdColor() {
        // NOTE(LDG): 사양서 변경 (EXNCP-1046)
        if (mIgnElapsed != IgnElapsed::ON_0ms || !isConfigHudOn()) {
            return;
        }

        SFCHD_Detail_Contents_SpeedConstantColorStat dspStat = SFCHD_Detail_Contents_SpeedConstantColorStat::DEFAULT_COLOR;
        HUInt64 dspValue = 0;
        if ((mDisplaySpeedUnit == DisplaySpeedUnit::KM_PER_HOUR) && (mNaviOverSpdUnit == NaviOverSpdUnit::KILOMETER)) {
            if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange1 && mNaviOverSpeedRange1 <= kNaviOverSpeedRangeMax) &&
                (kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                (kNaviOverSpeedColorMin <= mNaviOverSpeedColor1 && mNaviOverSpeedColor1 <= kNaviOverSpeedColorMax) &&
                (mNaviOverSpeedRange1 <= mSpeedDigitalValue) && (mSpeedDigitalValue < mNaviOverSpeedRange2)) {
                dspStat = SFCHD_Detail_Contents_SpeedConstantColorStat::VALUE;
                dspValue = mNaviOverSpeedColor1;
            } else if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                       (kNaviOverSpeedColorMin <= mNaviOverSpeedColor2 && mNaviOverSpeedColor2 <= kNaviOverSpeedColorMax) &&
                       (mNaviOverSpeedRange2 <= mSpeedDigitalValue)) {
                dspStat = SFCHD_Detail_Contents_SpeedConstantColorStat::VALUE;
                dspValue = mNaviOverSpeedColor2;
            } else {
                // DEFAULT_COLOR
            }
        } else if ((mDisplaySpeedUnit == DisplaySpeedUnit::MILE_PER_HOUR) && (mNaviOverSpdUnit == NaviOverSpdUnit::MILE)) {
            if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange1 && mNaviOverSpeedRange1 <= kNaviOverSpeedRangeMax) &&
                (kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                (kNaviOverSpeedColorMin <= mNaviOverSpeedColor1 && mNaviOverSpeedColor1 <= kNaviOverSpeedColorMax) &&
                (mNaviOverSpeedRange1 <= mSpeedDigitalValue) && (mSpeedDigitalValue < mNaviOverSpeedRange2)) {
                dspStat = SFCHD_Detail_Contents_SpeedConstantColorStat::VALUE;
                dspValue = mNaviOverSpeedColor1;
            } else if ((kNaviOverSpeedRangeMin <= mNaviOverSpeedRange2 && mNaviOverSpeedRange2 <= kNaviOverSpeedRangeMax) &&
                       (kNaviOverSpeedColorMin <= mNaviOverSpeedColor2 && mNaviOverSpeedColor2 <= kNaviOverSpeedColorMax) &&
                       (mNaviOverSpeedRange2 <= mSpeedDigitalValue)) {
                dspStat = SFCHD_Detail_Contents_SpeedConstantColorStat::VALUE;
                dspValue = mNaviOverSpeedColor2;
            } else {
                // DEFAULT_COLOR
            }
        } else {
            // DEFAULT_COLOR
        }
        setSFCHD_Detail_Contents_SpeedConstantColorStat(dspStat);
        setSFCHD_Detail_Contents_SpeedConstantColorValue(dspValue);
    }

    void updateSpeedValue() {
        setSFCHD_Detail_Contents_SpeedConstantSpeedValue(mSpeedDigitalValue);
    }

    void updateSpeedStat() {
        SFCHD_Detail_Contents_SpeedConstantSpeedStat stat = SFCHD_Detail_Contents_SpeedConstantSpeedStat::NONE;

        if (mSpeedDigitalStat == SpeedDigitalStat::DISPLAY_OFF) {
            stat = SFCHD_Detail_Contents_SpeedConstantSpeedStat::DISPLAY_OFF;
        } else if (mSpeedDigitalStat == SpeedDigitalStat::VALUE) {
            stat = SFCHD_Detail_Contents_SpeedConstantSpeedStat::VALUE;
        } else {
            // nothing
        }

        setSFCHD_Detail_Contents_SpeedConstantSpeedStat(stat);
    }

    void updateHDVehicleSpeedMainDisplayUnitStat() {
        if (mDisplaySpeedUnit == DisplaySpeedUnit::KM_PER_HOUR) {
            setSFCHD_Detail_Contents_SpeedConstantSpeedMainDisplayUnitStat(
                SFCHD_Detail_Contents_SpeedConstantSpeedMainDisplayUnitStat::KM_PER_HOUR);
        } else if (mDisplaySpeedUnit == DisplaySpeedUnit::MILE_PER_HOUR) {
            setSFCHD_Detail_Contents_SpeedConstantSpeedMainDisplayUnitStat(
                SFCHD_Detail_Contents_SpeedConstantSpeedMainDisplayUnitStat::MILE_PER_HOUR);
        } else {
            // no operation
        }
    }

    inline HBool isConfigHudOn() const {
        return mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    static constexpr HUInt64 kNaviOverSpeedColorMin = 0x000001;
    static constexpr HUInt64 kNaviOverSpeedColorMax = 0xFFFFFE;
    static constexpr HUInt64 kNaviOverSpeedRangeMin = 1;
    static constexpr HUInt64 kNaviOverSpeedRangeMax = 254;  // 0xFE

    HUInt64 mNaviOverSpeedColor1 = 0;
    HUInt64 mNaviOverSpeedColor2 = 0;
    HUInt64 mNaviOverSpeedRange1 = 0;
    HUInt64 mNaviOverSpeedRange2 = 0;
    HUInt64 mSpeedDigitalValue = 0;

    using NaviOverSpdUnit = ArgumentsUnitChanged::Input_NaviOverSpdUnit;
    using DisplaySpeedUnit = ArgumentsUnitChanged::Inter_DisplaySpeedUnit;
    using ConfigHUD = ArgumentsConfigHUDChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigHUDChanged::Inter_ConfigNewHUD;
    using SpeedDigitalStat = ArgumentsDisplaySpeedChanged::SpeedDigitalStat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    NaviOverSpdUnit mNaviOverSpdUnit = NaviOverSpdUnit::KILOMETER;
    DisplaySpeedUnit mDisplaySpeedUnit = DisplaySpeedUnit::KM_PER_HOUR;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    SpeedDigitalStat mSpeedDigitalStat = SpeedDigitalStat::NONE;
};

}  // namespace ccos

#endif  // SFSS_HD_Detail_Contents_Speed_H
