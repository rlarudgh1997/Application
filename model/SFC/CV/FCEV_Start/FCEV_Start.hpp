/**
 * @file FCEV_Start.hpp
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
#ifndef SFSS_FCEV_Start_H
#define SFSS_FCEV_Start_H

#define DLOG_ENABLED gEnableSFCLog

#include "FCEV_StartBase.hpp"

namespace ccos {

// SFC Version : 5.0.0
// Spec Version : v0.16
// Reference : [CV490] FCEV_Start. Contains : Event. Recommend: VALUE_CHANGED
class FCEV_Start : public FCEV_StartBase {
public:
    FCEV_Start() = default;
    ~FCEV_Start() override = default;
    FCEV_Start(const FCEV_Start& other) = delete;
    FCEV_Start(FCEV_Start&& other) noexcept = delete;
    FCEV_Start& operator=(const FCEV_Start& other) = delete;
    FCEV_Start& operator=(FCEV_Start&& other) noexcept = delete;

    void onInitialize() override {
        setSFCFCEV_StartEventColdStartID("E74901");
        setSFCFCEV_StartEventColdStartLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_StartEventColdStartLinkedSoundType(SFCFCEV_StartEventColdStartLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_StartEventColdStartLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateEventColdStart();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateEventColdStart();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateEventColdStart();
    }

    void onColdStartStatusChanged(const ArgumentsColdStartStatusChanged& args) {
        mColdStartStatusFromFCU = args.mInput_ColdStartStatusFromFCU;
        mColdStartStatusFromFCU2 = args.mInput_ColdStartStatusFromFCU2;

        updateEventColdStart();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventColdStart() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCFCEV_StartEventColdStartStat stat{SFCFCEV_StartEventColdStartStat::OFF};

        if ((mColdStartStatusFromFCU == ColdStartStatusFromFCU::ON || mColdStartStatusFromFCU2 == ColdStartStatusFromFCU2::ON) &&
            mIsIgnOn == true) {
            stat = SFCFCEV_StartEventColdStartStat::ON;
        }
        setSFCFCEV_StartEventColdStartStat(stat);
    }

    HBool mIsIgnOn = false;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    using ColdStartStatusFromFCU = ArgumentsColdStartStatusChanged::Input_ColdStartStatusFromFCU;
    using ColdStartStatusFromFCU2 = ArgumentsColdStartStatusChanged::Input_ColdStartStatusFromFCU2;
    ColdStartStatusFromFCU mColdStartStatusFromFCU = ColdStartStatusFromFCU::OFF;
    ColdStartStatusFromFCU2 mColdStartStatusFromFCU2 = ColdStartStatusFromFCU2::OFF;
};

}  // namespace ccos

#endif  // SFSS_FCEV_Start_H
