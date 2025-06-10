/**
 * @file HEV_Charge_Warning.hpp
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
#ifndef SFSS_HEV_Charge_Warning_H
#define SFSS_HEV_Charge_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "HEV_Charge_WarningBase.hpp"

namespace ccos {

// SFC Version : 0.0.2
// Reference : [PT240] HEV_Charge_Warning. Contains : Event. Recommend: VALUE_CHANGED
class HEV_Charge_Warning : public HEV_Charge_WarningBase {
public:
    HEV_Charge_Warning() = default;
    ~HEV_Charge_Warning() override = default;
    HEV_Charge_Warning(const HEV_Charge_Warning& other) = delete;
    HEV_Charge_Warning(HEV_Charge_Warning&& other) noexcept = delete;
    HEV_Charge_Warning& operator=(const HEV_Charge_Warning& other) = delete;
    HEV_Charge_Warning& operator=(HEV_Charge_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCHEV_Charge_WarningEventHevStopToChargeReqID("E21801");
        setSFCHEV_Charge_WarningEventHevStopToChargeReqLinkedSoundID("SND_PopUpWarn1");
        setSFCHEV_Charge_WarningEventHevStopToChargeReqLinkedSoundType(
            SFCHEV_Charge_WarningEventHevStopToChargeReqLinkedSoundType::REPEAT_COUNT);
        setSFCHEV_Charge_WarningEventHevStopToChargeReqLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onHevStopToChargeReqChanged(const ArgumentsHevStopToChargeReqChanged& args) {
        mHevStopToChargeReqStatus = args.mInput_HevStopToChargeReqStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mHevStopToChargeReqStatus == HevStopToChargeReqStatus::ON) {
            setSFCHEV_Charge_WarningEventHevStopToChargeReqStat(SFCHEV_Charge_WarningEventHevStopToChargeReqStat::ON);

        } else {
            setSFCHEV_Charge_WarningEventHevStopToChargeReqStat(SFCHEV_Charge_WarningEventHevStopToChargeReqStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using HevStopToChargeReqStatus = ArgumentsHevStopToChargeReqChanged::Input_HevStopToChargeReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HevStopToChargeReqStatus mHevStopToChargeReqStatus = HevStopToChargeReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_HEV_Charge_Warning_H
