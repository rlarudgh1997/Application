/**
 * @file Water_Separator.hpp
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
#ifndef SFSS_Water_Separator_H
#define SFSS_Water_Separator_H

#define DLOG_ENABLED gEnableSFCLog

#include "Water_SeparatorBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT450] Water_Separator. Contains : Telltale. Recommend: VALUE_CHANGED
class Water_Separator : public Water_SeparatorBase {
public:
    Water_Separator() = default;
    ~Water_Separator() override = default;
    Water_Separator(const Water_Separator& other) = delete;
    Water_Separator(Water_Separator&& other) noexcept = delete;
    Water_Separator& operator=(const Water_Separator& other) = delete;
    Water_Separator& operator=(Water_Separator&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onWaterSeparatorStatusChanged(const ArgumentsWaterSeparatorStatusChanged& args) {
        mWaterSeparatorIndiReqStatus = args.mInput_WaterSeparatorIndiReqStatus;
        updateTelltale();
    }

private:
    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mWaterSeparatorIndiReqStatus == WaterSeparatorIndiReqStatus::ON) {
                setSFCWater_SeparatorTelltaleWater_SeparatorStat(SFCWater_SeparatorTelltaleWater_SeparatorStat::ON);
            } else {
                setSFCWater_SeparatorTelltaleWater_SeparatorStat(SFCWater_SeparatorTelltaleWater_SeparatorStat::OFF);
            }
        } else {
            setSFCWater_SeparatorTelltaleWater_SeparatorStat(SFCWater_SeparatorTelltaleWater_SeparatorStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using WaterSeparatorIndiReqStatus = ArgumentsWaterSeparatorStatusChanged::Input_WaterSeparatorIndiReqStatus;
    WaterSeparatorIndiReqStatus mWaterSeparatorIndiReqStatus = WaterSeparatorIndiReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Water_Separator_H
