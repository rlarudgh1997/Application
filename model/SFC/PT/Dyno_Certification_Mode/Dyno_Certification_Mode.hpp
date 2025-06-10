/**
 * @file Dyno_Certification_Mode.hpp
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
#ifndef SFSS_Dyno_Certification_Mode_H
#define SFSS_Dyno_Certification_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Dyno_Certification_ModeBase.hpp"

namespace ccos {

// SFC Version : 2.0.3
// Reference : [PT090] Dyno_Certification_Mode. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Dyno_Certification_Mode : public Dyno_Certification_ModeBase {
public:
    Dyno_Certification_Mode() = default;
    ~Dyno_Certification_Mode() override = default;
    Dyno_Certification_Mode(const Dyno_Certification_Mode& other) = delete;
    Dyno_Certification_Mode(Dyno_Certification_Mode&& other) noexcept = delete;
    Dyno_Certification_Mode& operator=(const Dyno_Certification_Mode& other) = delete;
    Dyno_Certification_Mode& operator=(Dyno_Certification_Mode&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDyno_Certification_ModeEventDynoSocID("E20501");
        setSFCDyno_Certification_ModeEventDynoSocLinkedSoundID("SND_PopUpInform2");
        setSFCDyno_Certification_ModeEventDynoSocLinkedSoundType(
            SFCDyno_Certification_ModeEventDynoSocLinkedSoundType::REPEAT_COUNT);
        setSFCDyno_Certification_ModeEventDynoSocLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstant();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstant();
        updateEvent();
    }

    void onEvServiceModeChanged(const ArgumentsEvServiceModeChanged& args) {
        mEvServiceModeStatus = args.mInput_EvServiceModeStatus;
        updateEvent();
    }

    void onEvSocDisplayChanged(const ArgumentsEvSocDisplayChanged& args) {
        mEvSocDisplayValue = args.mInput_EvSocDisplayValue;
        updateConstant();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstant() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && 0 <= mEvSocDisplayValue && mEvSocDisplayValue <= 200) {
            HUInt64 value = mEvSocDisplayValue / 2;  // floor
            setSFCDyno_Certification_ModeConstantDynoSocValue(value);
            setSFCDyno_Certification_ModeConstantDynoSocStat(SFCDyno_Certification_ModeConstantDynoSocStat::VALUE);
        } else {
            setSFCDyno_Certification_ModeConstantDynoSocStat(SFCDyno_Certification_ModeConstantDynoSocStat::UNMEASURABLE);
        }
    }

    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && (mEvServiceModeStatus == EvServiceModeStatus::TEST_MODE_F ||
                                                  mEvServiceModeStatus == EvServiceModeStatus::TEST_MODE_R)) {
            setSFCDyno_Certification_ModeEventDynoSocStat(SFCDyno_Certification_ModeEventDynoSocStat::ON);
        } else {
            setSFCDyno_Certification_ModeEventDynoSocStat(SFCDyno_Certification_ModeEventDynoSocStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    using EvServiceModeStatus = ArgumentsEvServiceModeChanged::Input_EvServiceModeStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HUInt64 mEvSocDisplayValue = 0;
    EvServiceModeStatus mEvServiceModeStatus = EvServiceModeStatus::NORMAL;
};

}  // namespace ccos

#endif  // SFSS_Dyno_Certification_Mode_H
