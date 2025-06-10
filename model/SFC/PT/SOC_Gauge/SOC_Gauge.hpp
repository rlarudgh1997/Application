/**
 * @file SOC_Gauge.hpp
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
#ifndef SFSS_SOC_Gauge_H
#define SFSS_SOC_Gauge_H

#define DLOG_ENABLED gEnableSFCLog

#include "SOC_GaugeBase.hpp"

namespace ccos {

// SFC Version : 11.0.0
// Reference : [PT380] SOC_Gauge. Contains : Constant. Recommend: VALUE_CHANGED
class SOC_Gauge : public SOC_GaugeBase {
public:
    SOC_Gauge() = default;
    ~SOC_Gauge() override = default;
    SOC_Gauge(const SOC_Gauge& other) = delete;
    SOC_Gauge(SOC_Gauge&& other) noexcept = delete;
    SOC_Gauge& operator=(const SOC_Gauge& other) = delete;
    SOC_Gauge& operator=(SOC_Gauge&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        DDebug() << "Inter_ConfigVehicleType: " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        updateSOCGaugeLowAlertStat();
    }

    void onEvSocDisplayChanged(const ArgumentsEvSocDisplayChanged& args) {
        DDebug() << "Input_EvSocDisplayValue " << args.mInput_EvSocDisplayValue;
        mInput_EvSocDisplayValue = args.mInput_EvSocDisplayValue;
        mInput_LowSocDisplayReferenceValue = args.mInput_LowSocDisplayReferenceValue;
        mInput_LowSocDisplayStep1ReferenceValue = args.mInput_LowSocDisplayStep1ReferenceValue;
        updateSOCGaugeValue();
        updateSOCGaugeLowAlertStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateSOCGaugeValue() {
        if (0 <= mInput_EvSocDisplayValue && mInput_EvSocDisplayValue <= 200) {
            HDouble percentageValue = 0.0;
            percentageValue = floor(static_cast<HDouble>(mInput_EvSocDisplayValue) * 0.5);
            setSFCSOC_GaugeConstantSOCGaugeStat(SFCSOC_GaugeConstantSOCGaugeStat::VALUE);
            setSFCSOC_GaugeConstantSOCGaugeValue(static_cast<HUInt64>(percentageValue));
        } else {
            setSFCSOC_GaugeConstantSOCGaugeStat(SFCSOC_GaugeConstantSOCGaugeStat::UNMEASURABLE);
        }
    }

    void updateSOCGaugeLowAlertStat() {
        SFCSOC_GaugeConstantLowAlertStat stat = SFCSOC_GaugeConstantLowAlertStat::WARN_OFF;
        if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) &&
            (mInput_LowSocDisplayReferenceValue * 2 + 1 >= mInput_EvSocDisplayValue) &&
            validDisplayRange(mInput_LowSocDisplayReferenceValue) == true) {
            stat = SFCSOC_GaugeConstantLowAlertStat::WARN_ON;
        } else if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                    mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) &&
                   (mInput_LowSocDisplayStep1ReferenceValue * 2 + 1 >= mInput_EvSocDisplayValue) &&
                   validDisplayRange(mInput_LowSocDisplayStep1ReferenceValue) == true) {
            stat = SFCSOC_GaugeConstantLowAlertStat::WARN_ON_STEP1;
        }
        setSFCSOC_GaugeConstantLowAlertStat(stat);
    }

    inline bool validDisplayRange(const HUInt64 displayValue) {
        return 1 <= displayValue && displayValue <= 100;
    }

    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    HUInt64 mInput_EvSocDisplayValue = 0;
    HUInt64 mInput_LowSocDisplayReferenceValue = 0;
    HUInt64 mInput_LowSocDisplayStep1ReferenceValue = 0;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::EV;
};

}  // namespace ccos

#endif  // SFSS_SOC_Gauge_H
