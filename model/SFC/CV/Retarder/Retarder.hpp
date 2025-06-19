/**
 * @file Retarder.hpp
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
#ifndef SFSS_Retarder_H
#define SFSS_Retarder_H

#define DLOG_ENABLED gEnableSFCLog

#include "RetarderBase.hpp"

namespace ccos {

// SFC Version : 7.0.0
// Spec Version : v0.20
// Reference : [CV250] Retarder. Contains : Telltale. Recommend: VALUE_CHANGED
class Retarder : public RetarderBase {
public:
    Retarder() = default;
    ~Retarder() override = default;
    Retarder(const Retarder& other) = delete;
    Retarder(Retarder&& other) noexcept = delete;
    Retarder& operator=(const Retarder& other) = delete;
    Retarder& operator=(Retarder&& other) noexcept = delete;

    void onInitialize() override {
        setSFCRetarderTelltaleRetarderMalLampBlinkValueA(SFCRetarderTelltaleRetarderMalLampBlinkValueA::ON);
        setSFCRetarderTelltaleRetarderMalLampBlinkValueB(SFCRetarderTelltaleRetarderMalLampBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateRetarderTelltaleRetarderLamp();
        updateRetarderTelltaleRetarderMalLamp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateRetarderTelltaleRetarderLamp();
        updateRetarderTelltaleRetarderMalLamp();
    }

    void onRetarderMalStatusChanged(const ArgumentsRetarderMalStatusChanged& args) {
        mRTDOverHeatWarningStatus = args.mInput_RTDOverHeatWarningStatus;
        updateRetarderTelltaleRetarderMalLamp();
    }

    void onRetarderLampStatusChanged(const ArgumentsRetarderLampStatusChanged& args) {
        mRetarderPercentTorque_DR = args.mInput_RetarderPercentTorque_DR;
        updateRetarderTelltaleRetarderLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateRetarderTelltaleRetarderLamp() {
        SFCRetarderTelltaleRetarderLampStat telltaleStatus = SFCRetarderTelltaleRetarderLampStat::OFF;
        if (mIsIgnOn == true) {
            if (mRetarderPercentTorque_DR < kPercentTorque) {
                telltaleStatus = SFCRetarderTelltaleRetarderLampStat::ON;
            }
        }
        setSFCRetarderTelltaleRetarderLampStat(telltaleStatus);
    }

    void updateRetarderTelltaleRetarderMalLamp() {
        SFCRetarderTelltaleRetarderMalLampStat telltaleStatus = SFCRetarderTelltaleRetarderMalLampStat::OFF;
        if (mIsIgnOn == true) {
            if (mRTDOverHeatWarningStatus == RTDOverHeatWarningStatus::BLINK2) {
                telltaleStatus = SFCRetarderTelltaleRetarderMalLampStat::BLINK2;
            } else if (mRTDOverHeatWarningStatus == RTDOverHeatWarningStatus::ON) {
                telltaleStatus = SFCRetarderTelltaleRetarderMalLampStat::ON;
            } else {
                // No operation
            }
        }
        setSFCRetarderTelltaleRetarderMalLampStat(telltaleStatus);
    }

    using RTDOverHeatWarningStatus = ArgumentsRetarderMalStatusChanged::Input_RTDOverHeatWarningStatus;

    static constexpr uint32_t kPercentTorque = 0x7D;

    HBool mIsIgnOn = false;
    HUInt64 mRetarderPercentTorque_DR = 0x00;
    RTDOverHeatWarningStatus mRTDOverHeatWarningStatus = RTDOverHeatWarningStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Retarder_H
