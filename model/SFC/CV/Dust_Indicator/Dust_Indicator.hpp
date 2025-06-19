/**
 * @file Dust_Indicator.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Dust_Indicator_H
#define SFSS_Dust_Indicator_H

#define DLOG_ENABLED gEnableSFCLog

#include "Dust_IndicatorBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.06
// Reference : [CV510] Dust_Indicator. Contains : Telltale. Recommend: VALUE_CHANGED
class Dust_Indicator : public Dust_IndicatorBase {
public:
    Dust_Indicator() = default;
    ~Dust_Indicator() override = default;
    Dust_Indicator(const Dust_Indicator& other) = delete;
    Dust_Indicator(Dust_Indicator&& other) noexcept = delete;
    Dust_Indicator& operator=(const Dust_Indicator& other) = delete;
    Dust_Indicator& operator=(Dust_Indicator&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onDustWarnStatusChanged(const ArgumentsDustWarnStatusChanged& args) {
        mDustWarnStatus = args.mInput_DustWarnStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCDust_IndicatorTelltaleDust_IndicatorStat stat = SFCDust_IndicatorTelltaleDust_IndicatorStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mDustWarnStatus == DustWarnStatus::ON) {
            stat = SFCDust_IndicatorTelltaleDust_IndicatorStat::ON;
        }
        setSFCDust_IndicatorTelltaleDust_IndicatorStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_500ms,
        MAX
    };

    using DustWarnStatus = ArgumentsDustWarnStatusChanged::Input_DustWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    DustWarnStatus mDustWarnStatus = DustWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Dust_Indicator_H
