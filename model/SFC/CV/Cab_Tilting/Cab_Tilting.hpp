/**
 * @file Cab_Tilting.hpp
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
#ifndef SFSS_Cab_Tilting_H
#define SFSS_Cab_Tilting_H

#define DLOG_ENABLED gEnableSFCLog

#include "Cab_TiltingBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.11
// Reference : [CV200] Cab_Tilting. Contains : Telltale. Recommend: VALUE_CHANGED
class Cab_Tilting : public Cab_TiltingBase {
public:
    Cab_Tilting() = default;
    ~Cab_Tilting() override = default;
    Cab_Tilting(const Cab_Tilting& other) = delete;
    Cab_Tilting(Cab_Tilting&& other) noexcept = delete;
    Cab_Tilting& operator=(const Cab_Tilting& other) = delete;
    Cab_Tilting& operator=(Cab_Tilting&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltale();
    }

    void onSignalCabTiltWarnStatusChanged(const ArgumentsSignalCabTiltWarnStatusChanged& args) {
        mCabTiltWarnStatus = args.mInput_CabTiltWarnStatus;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        SFCCab_TiltingTelltaleCab_TiltingStat stat = SFCCab_TiltingTelltaleCab_TiltingStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms && mCabTiltWarnStatus == CabTiltWarnStatus::ON) {
            stat = SFCCab_TiltingTelltaleCab_TiltingStat::ON;
        }
        setSFCCab_TiltingTelltaleCab_TiltingStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;

    using CabTiltWarnStatus = ArgumentsSignalCabTiltWarnStatusChanged::Input_CabTiltWarnStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    CabTiltWarnStatus mCabTiltWarnStatus = CabTiltWarnStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
};

}  // namespace ccos

#endif  // SFSS_Cab_Tilting_H
