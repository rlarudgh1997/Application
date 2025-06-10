/**
 * @file Glow.hpp
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
#ifndef SFSS_Glow_H
#define SFSS_Glow_H

#define DLOG_ENABLED gEnableSFCLog

#include "GlowBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT220] Glow. Contains : Telltale. Recommend: VALUE_CHANGED
class Glow : public GlowBase {
public:
    Glow() = default;
    ~Glow() override = default;
    Glow(const Glow& other) = delete;
    Glow(Glow&& other) noexcept = delete;
    Glow& operator=(const Glow& other) = delete;
    Glow& operator=(Glow&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onVariantChanged(const ArgumentsVariantChanged& args) {
        mInter_ConfigFuelType = args.mInter_ConfigFuelType;
        // NOTE(LDG): MCU Variant Enum 적용으로 FFV  값이 사라지고 OTHERS 이 추가됨, 그에 따라 FFV 값을 OTHERS 값으로 변경
        if (mInter_ConfigFuelType == Inter_ConfigFuelType::DSL || mInter_ConfigFuelType == Inter_ConfigFuelType::OTHERS) {
            mFuelTypeVerified = true;
        } else {
            mFuelTypeVerified = false;
        }
        updateTelltale();
    }

    void onGlowIndiReqStatusChanged(const ArgumentsGlowIndiReqStatusChanged& args) {
        mGlowIndiReqStatus = args.mInput_GlowIndiReqStatus;
        updateTelltale();
    }

private:
    void updateTelltale() {
        SFCGlowTelltaleGlowStat stat = SFCGlowTelltaleGlowStat::OFF;
        if (mFuelTypeVerified && mGlowIndiReqStatus == GlowIndiReqStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            stat = SFCGlowTelltaleGlowStat::ON;
        }
        setSFCGlowTelltaleGlowStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using Inter_ConfigFuelType = ArgumentsVariantChanged::Inter_ConfigFuelType;
    Inter_ConfigFuelType mInter_ConfigFuelType = Inter_ConfigFuelType::GSL;
    using GlowIndiReqStatus = ArgumentsGlowIndiReqStatusChanged::Input_GlowIndiReqStatus;
    GlowIndiReqStatus mGlowIndiReqStatus = GlowIndiReqStatus::OFF;
    HBool mFuelTypeVerified = false;
};

}  // namespace ccos

#endif  // SFSS_Glow_H
