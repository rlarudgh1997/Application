/**
 * @file Glow_CV.hpp
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
#ifndef SFSS_Glow_CV_H
#define SFSS_Glow_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Glow_CVBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.11
// Reference : [CV980] Glow_CV. Contains : Telltale. Recommend: VALUE_CHANGED
class Glow_CV : public Glow_CVBase {
public:
    Glow_CV() = default;
    ~Glow_CV() override = default;
    Glow_CV(const Glow_CV& other) = delete;
    Glow_CV(Glow_CV&& other) noexcept = delete;
    Glow_CV& operator=(const Glow_CV& other) = delete;
    Glow_CV& operator=(Glow_CV&& other) noexcept = delete;

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

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltale();
    }

    void onEngineFlowIndiReqStatusChanged(const ArgumentsEngineFlowIndiReqStatusChanged& args) {
        mEngineGlowIndiReqStatus = args.mInput_EngineGlowIndiReqStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mInter_ConfigFuelTypeCV != Inter_ConfigFuelTypeCV::DSL || mInter_ConfigVehicleType != Inter_ConfigVehicleType::ICV) {
            return;
        }
        SFCGlow_CVTelltaleGlowCVStat stat = SFCGlow_CVTelltaleGlowCVStat::OFF;
        if (mEngineGlowIndiReqStatus == EngineGlowIndiReqStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            stat = SFCGlow_CVTelltaleGlowCVStat::ON;
        }
        setSFCGlow_CVTelltaleGlowCVStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using Inter_ConfigFuelTypeCV = ArgumentsConfigChanged::Inter_ConfigFuelTypeCV;
    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using EngineGlowIndiReqStatus = ArgumentsEngineFlowIndiReqStatusChanged::Input_EngineGlowIndiReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    Inter_ConfigFuelTypeCV mInter_ConfigFuelTypeCV = Inter_ConfigFuelTypeCV::DSL;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    EngineGlowIndiReqStatus mEngineGlowIndiReqStatus = EngineGlowIndiReqStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Glow_CV_H
