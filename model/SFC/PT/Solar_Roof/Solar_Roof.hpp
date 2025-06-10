/**
 * @file Solar_Roof.hpp
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
#ifndef SFSS_Solar_Roof_H
#define SFSS_Solar_Roof_H

#define DLOG_ENABLED gEnableSFCLog

#include "Solar_RoofBase.hpp"

namespace ccos {

// SFC Version : 0.2.0
// Reference : [PT390] Solar Roof. Contains : Event. Recommend: VALUE_CHANGED
class Solar_Roof : public Solar_RoofBase {
public:
    Solar_Roof() = default;
    ~Solar_Roof() override = default;
    Solar_Roof(const Solar_Roof& other) = delete;
    Solar_Roof(Solar_Roof&& other) noexcept = delete;
    Solar_Roof& operator=(const Solar_Roof& other) = delete;
    Solar_Roof& operator=(Solar_Roof&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSolar_RoofEventSolar_RoofID("E23001");
        setSFCSolar_RoofEventSolar_RoofLinkedSoundID("SND_PopUpWarn1");
        setSFCSolar_RoofEventSolar_RoofLinkedSoundType(SFCSolar_RoofEventSolar_RoofLinkedSoundType::REPEAT_COUNT);
        setSFCSolar_RoofEventSolar_RoofLinkedSoundRepeatCount(1);
        setSFCSolar_RoofEventSolar_RoofStat(SFCSolar_RoofEventSolar_RoofStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onSolarRoofFailWarnChanged(const ArgumentsSolarRoofFailWarnChanged& args) {
        mSolarRoofFailWarnStatus = args.mInput_SolarRoofFailWarnStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        HBool eventStat = (mIgnElapsed == IgnElapsed::ON_0ms && mSolarRoofFailWarnStatus == SolarRoofFailWarnStatus::ON);

        if (eventStat) {
            setSFCSolar_RoofEventSolar_RoofStat(SFCSolar_RoofEventSolar_RoofStat::ON);
        } else {
            setSFCSolar_RoofEventSolar_RoofStat(SFCSolar_RoofEventSolar_RoofStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using SolarRoofFailWarnStatus = ArgumentsSolarRoofFailWarnChanged::Input_SolarRoofFailWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    SolarRoofFailWarnStatus mSolarRoofFailWarnStatus = SolarRoofFailWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Solar_Roof_H
