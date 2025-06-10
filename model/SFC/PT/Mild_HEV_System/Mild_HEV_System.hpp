/**
 * @file Mild_HEV_System.hpp
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
#ifndef SFSS_Mild_HEV_System_H
#define SFSS_Mild_HEV_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Mild_HEV_SystemBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT320] Mild_HEV_System. Contains : Event. Recommend: VALUE_CHANGED
class Mild_HEV_System : public Mild_HEV_SystemBase {
public:
    Mild_HEV_System() = default;
    ~Mild_HEV_System() override = default;
    Mild_HEV_System(const Mild_HEV_System& other) = delete;
    Mild_HEV_System(Mild_HEV_System&& other) noexcept = delete;
    Mild_HEV_System& operator=(const Mild_HEV_System& other) = delete;
    Mild_HEV_System& operator=(Mild_HEV_System&& other) noexcept = delete;

    void onInitialize() override {
        setSFCMild_HEV_SystemEventMildHevSysWarnID("E22201");
        setSFCMild_HEV_SystemEventMildHevSysWarnStat(SFCMild_HEV_SystemEventMildHevSysWarnStat::OFF);
        setSFCMild_HEV_SystemEventMildHevSysWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCMild_HEV_SystemEventMildHevSysWarnLinkedSoundType(
            SFCMild_HEV_SystemEventMildHevSysWarnLinkedSoundType::REPEAT_COUNT);
        setSFCMild_HEV_SystemEventMildHevSysWarnLinkedSoundRepeatCount(1);

        setSFCMild_HEV_SystemEventMildHevLdcWarnID("E22202");
        setSFCMild_HEV_SystemEventMildHevLdcWarnStat(SFCMild_HEV_SystemEventMildHevLdcWarnStat::OFF);
        setSFCMild_HEV_SystemEventMildHevLdcWarnLinkedSoundID("SND_PopUpWarn2");
        setSFCMild_HEV_SystemEventMildHevLdcWarnLinkedSoundType(
            SFCMild_HEV_SystemEventMildHevLdcWarnLinkedSoundType::DURATION_MINMAX);
        setSFCMild_HEV_SystemEventMildHevLdcWarnLinkedSoundDurationMin(kMildHevLdcWarnDurationMin);
        setSFCMild_HEV_SystemEventMildHevLdcWarnLinkedSoundDurationMax(kMildHevLdcWarnDurationMax);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateMildHevSysWarnEvent();
        updateMildHevLdcWarnEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateMildHevSysWarnEvent();
        updateMildHevLdcWarnEvent();
    }

    void onMildHevSysWarnChanged(const ArgumentsMildHevSysWarnChanged& args) {
        mMildHevSysWarnStatusFromBMS = args.mInput_MildHevSysWarnStatusFromBMS;
        mMildHevSysWarnStatusFromEMS = args.mInput_MildHevSysWarnStatusFromEMS;
        mMildHevSysWarnStatusFromLDC = args.mInput_MildHevSysWarnStatusFromLDC;
        updateMildHevSysWarnEvent();
    }

    void onMildHevLdcWarnChanged(const ArgumentsMildHevLdcWarnChanged& args) {
        mMildHevLdcWarnStatus = args.mInput_MildHevLdcWarnStatus;
        updateMildHevLdcWarnEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateMildHevSysWarnEvent() {
        SFCMild_HEV_SystemEventMildHevSysWarnStat stat = SFCMild_HEV_SystemEventMildHevSysWarnStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && (mMildHevSysWarnStatusFromBMS == MildHevSysWarnStatusFromBMS::ON ||
                                                  mMildHevSysWarnStatusFromEMS == MildHevSysWarnStatusFromEMS::ON ||
                                                  mMildHevSysWarnStatusFromLDC == MildHevSysWarnStatusFromLDC::ON)) {
            stat = SFCMild_HEV_SystemEventMildHevSysWarnStat::ON;
        }
        setSFCMild_HEV_SystemEventMildHevSysWarnStat(stat);
    }

    void updateMildHevLdcWarnEvent() {
        SFCMild_HEV_SystemEventMildHevLdcWarnStat stat = SFCMild_HEV_SystemEventMildHevLdcWarnStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms && mMildHevLdcWarnStatus == MildHevLdcWarnStatus::ON) {
            stat = SFCMild_HEV_SystemEventMildHevLdcWarnStat::ON;
        }
        setSFCMild_HEV_SystemEventMildHevLdcWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    static constexpr HUInt64 kMildHevLdcWarnDurationMin = 0;
    static constexpr HUInt64 kMildHevLdcWarnDurationMax = 10000;

    using MildHevSysWarnStatusFromBMS = ArgumentsMildHevSysWarnChanged::Input_MildHevSysWarnStatusFromBMS;
    using MildHevSysWarnStatusFromEMS = ArgumentsMildHevSysWarnChanged::Input_MildHevSysWarnStatusFromEMS;
    using MildHevSysWarnStatusFromLDC = ArgumentsMildHevSysWarnChanged::Input_MildHevSysWarnStatusFromLDC;
    using MildHevLdcWarnStatus = ArgumentsMildHevLdcWarnChanged::Input_MildHevLdcWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    MildHevSysWarnStatusFromBMS mMildHevSysWarnStatusFromBMS = MildHevSysWarnStatusFromBMS::OFF;
    MildHevSysWarnStatusFromEMS mMildHevSysWarnStatusFromEMS = MildHevSysWarnStatusFromEMS::OFF;
    MildHevSysWarnStatusFromLDC mMildHevSysWarnStatusFromLDC = MildHevSysWarnStatusFromLDC::OFF;
    MildHevLdcWarnStatus mMildHevLdcWarnStatus = MildHevLdcWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Mild_HEV_System_H
