/**
 * @file Lamp_Fail_Warning_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_Lamp_Fail_Warning_CV_H
#define SFSS_Lamp_Fail_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Lamp_Fail_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.11
// Reference : [CV880] Lamp_Fail_Warning_CV. Contains : Telltale. Recommend: VALUE_CHANGED
class Lamp_Fail_Warning_CV : public Lamp_Fail_Warning_CVBase {
public:
    Lamp_Fail_Warning_CV() = default;
    ~Lamp_Fail_Warning_CV() override = default;
    Lamp_Fail_Warning_CV(const Lamp_Fail_Warning_CV& other) = delete;
    Lamp_Fail_Warning_CV(Lamp_Fail_Warning_CV&& other) noexcept = delete;
    Lamp_Fail_Warning_CV& operator=(const Lamp_Fail_Warning_CV& other) = delete;
    Lamp_Fail_Warning_CV& operator=(Lamp_Fail_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleLedLamp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleLedLamp();
    }

    void onLampWarnStatusChanged(const ArgumentsLampWarnStatusChanged& args) {
        mAFLSLedLampWarnStatus = args.mInput_AFLSLedLampWarnStatus;
        mPDCExtLampWarnStatus = args.mInput_PDCExtLampWarnStatus;
        updateTelltaleLedLamp();
    }

    void onConfigLEDMalChanged(const ArgumentsConfigLEDMalChanged& args) {
        mConfigLEDMal = args.mInter_ConfigLEDMal;
        updateTelltaleLedLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operatoin
    }

private:
    void updateTelltaleLedLamp() {
        SFCLamp_Fail_Warning_CVTelltaleLedLampStat stat = SFCLamp_Fail_Warning_CVTelltaleLedLampStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms &&
            ((mAFLSLedLampWarnStatus == AFLSLedLampWarnStatus::ON && mConfigLEDMal == ConfigLEDMal::ON) ||
             (mPDCExtLampWarnStatus == PDCExtLampWarnStatus::ON))) {
            stat = SFCLamp_Fail_Warning_CVTelltaleLedLampStat::ON;
        }
        setSFCLamp_Fail_Warning_CVTelltaleLedLampStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_500ms,
        MAX
    };

    using AFLSLedLampWarnStatus = ArgumentsLampWarnStatusChanged::Input_AFLSLedLampWarnStatus;
    using PDCExtLampWarnStatus = ArgumentsLampWarnStatusChanged::Input_PDCExtLampWarnStatus;
    using ConfigLEDMal = ArgumentsConfigLEDMalChanged::Inter_ConfigLEDMal;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    AFLSLedLampWarnStatus mAFLSLedLampWarnStatus = AFLSLedLampWarnStatus::OFF;
    PDCExtLampWarnStatus mPDCExtLampWarnStatus = PDCExtLampWarnStatus::OFF;
    ConfigLEDMal mConfigLEDMal = ConfigLEDMal::OFF;
};

}  // namespace ccos

#endif  // SFSS_Lamp_Fail_Warning_CV_H
