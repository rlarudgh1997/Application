/**
 * @file Auto_Grease.hpp
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
#ifndef SFSS_Auto_Grease_H
#define SFSS_Auto_Grease_H

#define DLOG_ENABLED gEnableSFCLog

#include "Auto_GreaseBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.08
// Reference : [CV190] Auto_Grease. Contains : Telltale. Recommend: VALUE_CHANGED
class Auto_Grease : public Auto_GreaseBase {
public:
    Auto_Grease() = default;
    ~Auto_Grease() override = default;
    Auto_Grease(const Auto_Grease& other) = delete;
    Auto_Grease(Auto_Grease&& other) noexcept = delete;
    Auto_Grease& operator=(const Auto_Grease& other) = delete;
    Auto_Grease& operator=(Auto_Grease&& other) noexcept = delete;

    void onInitialize() override {
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onAutoGreaseWarnStatusChanged(const ArgumentsAutoGreaseWarnStatusChanged& args) {
        mAutoGreaseWarnStatus = args.mInput_AutoGreaseWarnStatus;
        updateTelltale();
    }

private:
    void updateTelltale() {
        SFCAuto_GreaseTelltaleAutoGreaseWarnLampStat telltaleStatus = SFCAuto_GreaseTelltaleAutoGreaseWarnLampStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mAutoGreaseWarnStatus == AutoGreaseWarnStatus::ON) {
            telltaleStatus = SFCAuto_GreaseTelltaleAutoGreaseWarnLampStat::ON;
        }

        setSFCAuto_GreaseTelltaleAutoGreaseWarnLampStat(telltaleStatus);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using AutoGreaseWarnStatus = ArgumentsAutoGreaseWarnStatusChanged::Input_AutoGreaseWarnStatus;

    AutoGreaseWarnStatus mAutoGreaseWarnStatus = AutoGreaseWarnStatus::OFF;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
};

}  // namespace ccos

#endif  // SFSS_Auto_Grease_H
