/**
 * @file GFD_Warning.hpp
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
#ifndef SFSS_GFD_Warning_H
#define SFSS_GFD_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "GFD_WarningBase.hpp"

namespace ccos {

// SFC Version : 8.0.0
// Spec Version : v0.27
// Reference : [CV460] GFD_Warning. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class GFD_Warning : public GFD_WarningBase {
public:
    GFD_Warning() = default;
    ~GFD_Warning() override = default;
    GFD_Warning(const GFD_Warning& other) = delete;
    GFD_Warning(GFD_Warning&& other) noexcept = delete;
    GFD_Warning& operator=(const GFD_Warning& other) = delete;
    GFD_Warning& operator=(GFD_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCGFD_WarningEventGFDFaultWarningBMSID("E74601");
        setSFCGFD_WarningEventGFDFaultWarningBMSLinkedSoundID("SND_PopUpWarn1");
        setSFCGFD_WarningEventGFDFaultWarningBMSLinkedSoundType(
            SFCGFD_WarningEventGFDFaultWarningBMSLinkedSoundType::REPEAT_COUNT);
        setSFCGFD_WarningEventGFDFaultWarningBMSLinkedSoundRepeatCount(1);
        setSFCGFD_WarningEventGFDFaultWarningFCUID("E74602");
        setSFCGFD_WarningSoundGFDWarningID("SND_GFDWarning");
        setSFCGFD_WarningSoundGFDWarningType(SFCGFD_WarningSoundGFDWarningType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleGFD2();
        updateEventFCU();
        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleGFD2();
        updateEventFCU();
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleGFD();
        updateTelltaleGFD2();
        updateEventBMS();
        updateEventFCU();
        updateSound();
    }

    void onGfdWarningStatusFromBMSChanged(const ArgumentsGfdWarningStatusFromBMSChanged& args) {
        mGFDFaultWarnLampFromBMS = args.mInput_GFDFaultWarningLampStatusFromBMS;
        updateTelltaleGFD();
        updateEventBMS();
    }

    void onGfdFaultWarningLampStatusFromFCUChanged(const ArgumentsGfdFaultWarningLampStatusFromFCUChanged& args) {
        mGFDFaultWarnLampFromFCU = args.mInput_GFDFaultWarningLampStatusFromFCU;
        mGFDFaultWarnLampFromFCU2 = args.mInput_GFDFaultWarningLampStatusFromFCU2;
        updateTelltaleGFD2();
        updateEventBMS();
    }

    void onGfdWarningStatusFromFCUChanged(const ArgumentsGfdWarningStatusFromFCUChanged& args) {
        mGFDWarnStatusFromFCU = args.mInput_GFDWarningStatusFromFCU;
        mGFDWarnStatusFromFCU2 = args.mInput_GFDWarningStatusFromFCU2;
        updateEventFCU();
    }

    void onGfdWarningSoundFromFCUChanged(const ArgumentsGfdWarningSoundFromFCUChanged& args) {
        mGFDWarnSoundFromFCU = args.mInput_GFDWarningSoundFromFCU;
        mGFDWarnSoundFromFCU2 = args.mInput_GFDWarningSoundFromFCU2;
        updateSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleGFD() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCGFD_WarningTelltaleGFDStat stat = SFCGFD_WarningTelltaleGFDStat::OFF;
        if (mGFDFaultWarnLampFromBMS == GFDFaultWarnLampFromBMS::ON) {
            stat = SFCGFD_WarningTelltaleGFDStat::ON;
        }
        setSFCGFD_WarningTelltaleGFDStat(stat);
    }

    void updateTelltaleGFD2() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCGFD_WarningTelltaleGFD2Stat stat = SFCGFD_WarningTelltaleGFD2Stat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && (mGFDFaultWarnLampFromFCU == GFDFaultWarnLampFromFCU::ON ||
                                                  mGFDFaultWarnLampFromFCU2 == GFDFaultWarnLampFromFCU2::ON)) {
            stat = SFCGFD_WarningTelltaleGFD2Stat::ON;
        }
        setSFCGFD_WarningTelltaleGFD2Stat(stat);
    }

    void updateEventBMS() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCGFD_WarningEventGFDFaultWarningBMSStat eventStat = SFCGFD_WarningEventGFDFaultWarningBMSStat::OFF;
        if (mGFDFaultWarnLampFromBMS == GFDFaultWarnLampFromBMS::ON) {
            eventStat = SFCGFD_WarningEventGFDFaultWarningBMSStat::ON;
        }
        setSFCGFD_WarningEventGFDFaultWarningBMSStat(eventStat);
    }

    void updateEventFCU() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCGFD_WarningEventGFDFaultWarningFCUStat eventStat = SFCGFD_WarningEventGFDFaultWarningFCUStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mGFDWarnStatusFromFCU == GFDWarnStatusFromFCU::ON || mGFDWarnStatusFromFCU2 == GFDWarnStatusFromFCU2::ON) {
                eventStat = SFCGFD_WarningEventGFDFaultWarningFCUStat::ON;
            }
        }
        setSFCGFD_WarningEventGFDFaultWarningFCUStat(eventStat);
    }

    void updateSound() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCGFD_WarningSoundGFDWarningStat soundStat = SFCGFD_WarningSoundGFDWarningStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mGFDWarnSoundFromFCU == GFDWarnSoundFromFCU::ON || mGFDWarnSoundFromFCU2 == GFDWarnSoundFromFCU2::ON) {
                soundStat = SFCGFD_WarningSoundGFDWarningStat::ON;
            }
        }
        setSFCGFD_WarningSoundGFDWarningStat(soundStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using GFDFaultWarnLampFromBMS = ArgumentsGfdWarningStatusFromBMSChanged::Input_GFDFaultWarningLampStatusFromBMS;
    using GFDFaultWarnLampFromFCU = ArgumentsGfdFaultWarningLampStatusFromFCUChanged::Input_GFDFaultWarningLampStatusFromFCU;
    using GFDFaultWarnLampFromFCU2 = ArgumentsGfdFaultWarningLampStatusFromFCUChanged::Input_GFDFaultWarningLampStatusFromFCU2;
    using GFDWarnStatusFromFCU = ArgumentsGfdWarningStatusFromFCUChanged::Input_GFDWarningStatusFromFCU;
    using GFDWarnStatusFromFCU2 = ArgumentsGfdWarningStatusFromFCUChanged::Input_GFDWarningStatusFromFCU2;
    using GFDWarnSoundFromFCU = ArgumentsGfdWarningSoundFromFCUChanged::Input_GFDWarningSoundFromFCU;
    using GFDWarnSoundFromFCU2 = ArgumentsGfdWarningSoundFromFCUChanged::Input_GFDWarningSoundFromFCU2;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    GFDFaultWarnLampFromBMS mGFDFaultWarnLampFromBMS = GFDFaultWarnLampFromBMS::OFF;
    GFDFaultWarnLampFromFCU mGFDFaultWarnLampFromFCU = GFDFaultWarnLampFromFCU::OFF;
    GFDFaultWarnLampFromFCU2 mGFDFaultWarnLampFromFCU2 = GFDFaultWarnLampFromFCU2::OFF;
    GFDWarnStatusFromFCU mGFDWarnStatusFromFCU = GFDWarnStatusFromFCU::OFF;
    GFDWarnStatusFromFCU2 mGFDWarnStatusFromFCU2 = GFDWarnStatusFromFCU2::OFF;
    GFDWarnSoundFromFCU mGFDWarnSoundFromFCU = GFDWarnSoundFromFCU::OFF;
    GFDWarnSoundFromFCU2 mGFDWarnSoundFromFCU2 = GFDWarnSoundFromFCU2::OFF;
};

}  // namespace ccos

#endif  // SFSS_GFD_Warning_H
