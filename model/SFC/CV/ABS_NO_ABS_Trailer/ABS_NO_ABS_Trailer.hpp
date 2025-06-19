/**
 * @file ABS_NO_ABS_Trailer.hpp
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
#ifndef SFSS_ABS_NO_ABS_Trailer_H
#define SFSS_ABS_NO_ABS_Trailer_H

#define DLOG_ENABLED gEnableSFCLog

#include "ABS_NO_ABS_TrailerBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.06
// Reference : [CV280] ABS_NO ABS Trailer. Contains : Telltale. Recommend: VALUE_CHANGED
class ABS_NO_ABS_Trailer : public ABS_NO_ABS_TrailerBase {
public:
    ABS_NO_ABS_Trailer() = default;
    ~ABS_NO_ABS_Trailer() override = default;
    ABS_NO_ABS_Trailer(const ABS_NO_ABS_Trailer& other) = delete;
    ABS_NO_ABS_Trailer(ABS_NO_ABS_Trailer&& other) noexcept = delete;
    ABS_NO_ABS_Trailer& operator=(const ABS_NO_ABS_Trailer& other) = delete;
    ABS_NO_ABS_Trailer& operator=(ABS_NO_ABS_Trailer&& other) noexcept = delete;

    void onInitialize() override {
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleABSTrailer();
        updateTelltaleNoABSTrailer();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleABSTrailer();
        updateTelltaleNoABSTrailer();
    }

    void onAbsTrailerLampStatusChanged(const ArgumentsAbsTrailerLampStatusChanged& args) {
        mABSTrailerLampStatus = args.mInput_ABSTrailerLampStatus;
        updateTelltaleABSTrailer();
    }

    void onNoABSTrailerLampStatusChanged(const ArgumentsNoABSTrailerLampStatusChanged& args) {
        mNoABSTrailerLampStatus = args.mInput_NoABSTrailerLampStatus;
        updateTelltaleNoABSTrailer();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltaleABSTrailer();
        updateTelltaleNoABSTrailer();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTelltaleABSTrailer() {
        SFCABS_NO_ABS_TrailerTelltaleABSTrailerLampStat telltaleABSStat = SFCABS_NO_ABS_TrailerTelltaleABSTrailerLampStat::OFF;

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        if (mABSTrailerLampStatus == ABSTrailerLampStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            telltaleABSStat = SFCABS_NO_ABS_TrailerTelltaleABSTrailerLampStat::ON;
        }
        setSFCABS_NO_ABS_TrailerTelltaleABSTrailerLampStat(telltaleABSStat);
    }

    void updateTelltaleNoABSTrailer() {
        SFCABS_NO_ABS_TrailerTelltaleNoABSTrailerLampStat telltaleNoABSStat =
            SFCABS_NO_ABS_TrailerTelltaleNoABSTrailerLampStat::OFF;

        if (mConfigVehicleTypeCV != ConfigVehicleTypeCV::TRUCK) {
            return;
        }

        if (mNoABSTrailerLampStatus == NoABSTrailerLampStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            telltaleNoABSStat = SFCABS_NO_ABS_TrailerTelltaleNoABSTrailerLampStat::ON;
        }
        setSFCABS_NO_ABS_TrailerTelltaleNoABSTrailerLampStat(telltaleNoABSStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ABSTrailerLampStatus = ArgumentsAbsTrailerLampStatusChanged::Input_ABSTrailerLampStatus;
    using NoABSTrailerLampStatus = ArgumentsNoABSTrailerLampStatusChanged::Input_NoABSTrailerLampStatus;
    using ConfigVehicleTypeCV = ArgumentsConfigChanged::Inter_ConfigVehicleTypeCV;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ABSTrailerLampStatus mABSTrailerLampStatus = ABSTrailerLampStatus::OFF;
    NoABSTrailerLampStatus mNoABSTrailerLampStatus = NoABSTrailerLampStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
};

}  // namespace ccos

#endif  // SFSS_ABS_NO_ABS_Trailer_H
