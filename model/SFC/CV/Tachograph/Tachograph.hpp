/**
 * @file Tachograph.hpp
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
#ifndef SFSS_Tachograph_H
#define SFSS_Tachograph_H

#define DLOG_ENABLED gEnableSFCLog

#include "TachographBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.07
// Reference : [CV090] Tachograph. Contains : Telltale. Recommend: VALUE_CHANGED
class Tachograph : public TachographBase {
public:
    Tachograph() = default;
    ~Tachograph() override = default;
    Tachograph(const Tachograph& other) = delete;
    Tachograph(Tachograph&& other) noexcept = delete;
    Tachograph& operator=(const Tachograph& other) = delete;
    Tachograph& operator=(Tachograph&& other) noexcept = delete;

    void onInitialize() override {
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigDTG = args.mInter_ConfigDTG;
        updateTelltale();
    }

    void onTachographSysFailStatusChanged(const ArgumentsTachographSysFailStatusChanged& args) {
        mTachographSysFailStatus = args.mInput_TachographSysFailStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTelltale() {
        if (mConfigDTG == ConfigDTG::OFF) {
            return;
        }

        SFCTachographTelltaleTachographStat status = SFCTachographTelltaleTachographStat::OFF;

        if (mIsIgnOn == true && mTachographSysFailStatus == TachographSysFailStatus::ON) {
            status = SFCTachographTelltaleTachographStat::ON;
        } else {
            // no operation
        }

        setSFCTachographTelltaleTachographStat(status);
    }

    using ConfigDTG = ArgumentsConfigChanged::Inter_ConfigDTG;
    using TachographSysFailStatus = ArgumentsTachographSysFailStatusChanged::Input_TachographSysFailStatus;

    HBool mIsIgnOn = false;
    ConfigDTG mConfigDTG = ConfigDTG::ON;
    TachographSysFailStatus mTachographSysFailStatus = TachographSysFailStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Tachograph_H
