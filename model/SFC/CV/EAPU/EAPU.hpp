/**
 * @file EAPU.hpp
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
#ifndef SFSS_EAPU_H
#define SFSS_EAPU_H

#define DLOG_ENABLED gEnableSFCLog

#include "EAPUBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.25
// Reference : [CV180] EAPU. Contains : Telltale. Recommend: VALUE_CHANGED
class EAPU : public EAPUBase {
public:
    EAPU() = default;
    ~EAPU() override = default;
    EAPU(const EAPU& other) = delete;
    EAPU(EAPU&& other) noexcept = delete;
    EAPU& operator=(const EAPU& other) = delete;
    EAPU& operator=(EAPU&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnStat = true;
        updateTelltaleEAPULamp();
        updateTelltaleCartridge();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnStat = false;
        updateTelltaleEAPULamp();
        updateTelltaleCartridge();
    }

    void onEapuSignalChanged(const ArgumentsEapuSignalChanged& args) {
        mEAPUAmberWarnStatus = args.mInput_EAPUAmberWarnStatus;
        mEAPURedWarnStatus = args.mInput_EAPURedWarnStatus;
        updateTelltaleEAPULamp();
    }

    void onCartridgeStatusChanged(const ArgumentsCartridgeStatusChanged& args) {
        mCartridgeStatus = args.mInput_CartridgeStatus;
        updateTelltaleCartridge();
    }

    void onConfigEAPUChanged(const ArgumentsConfigEAPUChanged& args) {
        mConfigEAPU = args.mInter_ConfigEAPU;
        updateTelltaleEAPULamp();
        updateTelltaleCartridge();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleEAPULamp() {
        if (mConfigEAPU != ConfigEAPU::ON) {
            return;
        }

        SFCEAPUTelltaleEAPULampStat stat = SFCEAPUTelltaleEAPULampStat::OFF;
        if (mIgnStat == true) {
            if (mEAPUAmberWarnStatus == EAPUAmberWarnStatus::ON &&
                (mEAPURedWarnStatus != EAPURedWarnStatus::ON && mEAPURedWarnStatus != EAPURedWarnStatus::MESSAGE_TIMEOUT)) {
                stat = SFCEAPUTelltaleEAPULampStat::AMBER;
            } else if (mEAPURedWarnStatus == EAPURedWarnStatus::ON || mEAPURedWarnStatus == EAPURedWarnStatus::MESSAGE_TIMEOUT) {
                stat = SFCEAPUTelltaleEAPULampStat::RED;
            } else {
                // no operation
            }
        }
        setSFCEAPUTelltaleEAPULampStat(stat);
    }

    void updateTelltaleCartridge() {
        if (mConfigEAPU != ConfigEAPU::ON) {
            return;
        }

        SFCEAPUTelltaleCartridgeStat stat = SFCEAPUTelltaleCartridgeStat::OFF;
        if (mIgnStat == true && mCartridgeStatus == CartridgeStatus::REPLACE) {
            stat = SFCEAPUTelltaleCartridgeStat::ON;
        }
        setSFCEAPUTelltaleCartridgeStat(stat);
    }

    using EAPUAmberWarnStatus = ArgumentsEapuSignalChanged::Input_EAPUAmberWarnStatus;
    using EAPURedWarnStatus = ArgumentsEapuSignalChanged::Input_EAPURedWarnStatus;
    using CartridgeStatus = ArgumentsCartridgeStatusChanged::Input_CartridgeStatus;
    using ConfigEAPU = ArgumentsConfigEAPUChanged::Inter_ConfigEAPU;

    EAPUAmberWarnStatus mEAPUAmberWarnStatus = EAPUAmberWarnStatus::OFF;
    EAPURedWarnStatus mEAPURedWarnStatus = EAPURedWarnStatus::OFF;
    CartridgeStatus mCartridgeStatus = CartridgeStatus::NORMAL;
    ConfigEAPU mConfigEAPU = ConfigEAPU::OFF;

    HBool mIgnStat = false;
};

}  // namespace ccos

#endif  // SFSS_EAPU_H
