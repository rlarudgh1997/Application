/**
 * @file Green_Zone_Lamp.hpp
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
#ifndef SFSS_Green_Zone_Lamp_H
#define SFSS_Green_Zone_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "Green_Zone_LampBase.hpp"

namespace ccos {

// SFC Version : 1.1.1
// Reference : [PT230] Green_Zone_Lamp. Contains : Telltale. Recommend: VALUE_CHANGED
class Green_Zone_Lamp : public Green_Zone_LampBase {
public:
    Green_Zone_Lamp() = default;
    ~Green_Zone_Lamp() override = default;
    Green_Zone_Lamp(const Green_Zone_Lamp& other) = delete;
    Green_Zone_Lamp(Green_Zone_Lamp&& other) noexcept = delete;
    Green_Zone_Lamp& operator=(const Green_Zone_Lamp& other) = delete;
    Green_Zone_Lamp& operator=(Green_Zone_Lamp&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onGreenZondIndReqChanged(const ArgumentsGreenZondIndReqChanged& args) {
        mGreenZoneIndReqStatus = args.mInput_GreenZoneIndReqStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCGreen_Zone_LampTelltaleGreenZoneLmpStat stat = SFCGreen_Zone_LampTelltaleGreenZoneLmpStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms && mGreenZoneIndReqStatus == GreenZoneIndReqStatus::ON) {
            stat = SFCGreen_Zone_LampTelltaleGreenZoneLmpStat::ON;
        }
        setSFCGreen_Zone_LampTelltaleGreenZoneLmpStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using GreenZoneIndReqStatus = ArgumentsGreenZondIndReqChanged::Input_GreenZoneIndReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    GreenZoneIndReqStatus mGreenZoneIndReqStatus = GreenZoneIndReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Green_Zone_Lamp_H
