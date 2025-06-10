/**
 * @file Coasting.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2020-2021  Hyundai Motor Company,
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
#ifndef SFSS_Coasting_H
#define SFSS_Coasting_H

#define DLOG_ENABLED gEnableSFCLog

#include "CoastingBase.hpp"

namespace ccos {

// SFC Version : 4.0.1
// Reference : [PT060] Coasting. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Coasting : public CoastingBase {
public:
    Coasting() = default;
    ~Coasting() override = default;
    Coasting(const Coasting& other) = delete;
    Coasting(Coasting&& other) noexcept = delete;
    Coasting& operator=(const Coasting& other) = delete;
    Coasting& operator=(Coasting&& other) noexcept = delete;

    void onInitialize() override {
        setSFCCoastingConstantCoastinginertiaBlinkValueA(SFCCoastingConstantCoastinginertiaBlinkValueA::READY);
        setSFCCoastingConstantCoastinginertiaBlinkValueB(SFCCoastingConstantCoastinginertiaBlinkValueB::OFF);
        setSFCCoastingEventCoastingIndiReqID("E20802");
        setSFCCoastingEventCoastingIndiReqLinkedSoundRepeatCount(1);
        setSFCCoastingEventControlID("E20804");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onCoastingIndiReqStatusChanged(const ArgumentsCoastingIndiReqStatusChanged& args) {
        mCoastingIndiReqStatus = args.mInput_CoastingIndiReqStatus;
        updateCoastingEvent();
    }

    void onCoastingWarnOnOffStatusChanged(const ArgumentsCoastingWarnOnOffStatusChanged& args) {
        mCoastingWarnOnOffStatus = args.mInput_CoastingWarnOnOffStatus;
        updateCoastingEvent();
    }

    void onControlStatusChanged(const ArgumentsControlStatusChanged& args) {
        mSscControlStatus = args.mInput_SscControlStatus;
        mNccControlStatus = args.mInput_NccControlStatus;
        mPreShiftCoastingDisplayStatus = args.mInput_PreShiftCoastingDisplayStatus;
        updateControlEvent();
    }

private:
    void updateAll() {
        updateCoastingEvent();
        updateControlEvent();
    }

    void updateCoastingEvent() {
        SFCCoastingConstantCoastinginertiaStat coastinginertiaStat = SFCCoastingConstantCoastinginertiaStat::OFF;
        SFCCoastingEventCoastingIndiReqStat coastingIndiReqStat = SFCCoastingEventCoastingIndiReqStat::OFF;
        std::string coastingIndiReqLinkedSoundId = "";
        SFCCoastingEventCoastingIndiReqLinkedSoundType coastingIndiReqLinkedSoundType =
            SFCCoastingEventCoastingIndiReqLinkedSoundType::NONE;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mCoastingIndiReqStatus == CoastingIndiReqStatus::READY) {
                coastinginertiaStat = SFCCoastingConstantCoastinginertiaStat::READY;
                coastingIndiReqStat = SFCCoastingEventCoastingIndiReqStat::ON;
                if (mCoastingWarnOnOffStatus == CoastingWarnOnOffStatus::DISABLE) {
                    coastingIndiReqLinkedSoundId = "SND_PopUpInform2";
                    coastingIndiReqLinkedSoundType = SFCCoastingEventCoastingIndiReqLinkedSoundType::REPEAT_COUNT;
                }
            } else if (mCoastingIndiReqStatus == CoastingIndiReqStatus::BLINK) {
                coastinginertiaStat = SFCCoastingConstantCoastinginertiaStat::BLINK1;
                coastingIndiReqStat = SFCCoastingEventCoastingIndiReqStat::ON;
            } else if (mCoastingIndiReqStatus == CoastingIndiReqStatus::SET) {
                coastinginertiaStat = SFCCoastingConstantCoastinginertiaStat::SET;
                coastingIndiReqStat = SFCCoastingEventCoastingIndiReqStat::ON;
            } else {
                // no operation
            }
            setSFCCoastingConstantCoastinginertiaStat(coastinginertiaStat);
        }
        setSFCCoastingEventCoastingIndiReqStat(coastingIndiReqStat);
        setSFCCoastingEventCoastingIndiReqLinkedSoundID(coastingIndiReqLinkedSoundId);
        setSFCCoastingEventCoastingIndiReqLinkedSoundType(coastingIndiReqLinkedSoundType);
    }

    void updateControlEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mSscControlStatus == SscControlStatus::ON || mNccControlStatus == NccControlStatus::ON ||
             mPreShiftCoastingDisplayStatus == PreShiftCoastingDisplayStatus::ON)) {
            setSFCCoastingEventControlStat(SFCCoastingEventControlStat::ON);
        } else {
            setSFCCoastingEventControlStat(SFCCoastingEventControlStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using CoastingIndiReqStatus = ArgumentsCoastingIndiReqStatusChanged::Input_CoastingIndiReqStatus;
    using NccControlStatus = ArgumentsControlStatusChanged::Input_NccControlStatus;
    using SscControlStatus = ArgumentsControlStatusChanged::Input_SscControlStatus;
    using PreShiftCoastingDisplayStatus = ArgumentsControlStatusChanged::Input_PreShiftCoastingDisplayStatus;
    using CoastingWarnOnOffStatus = ArgumentsCoastingWarnOnOffStatusChanged::Input_CoastingWarnOnOffStatus;
    CoastingIndiReqStatus mCoastingIndiReqStatus = CoastingIndiReqStatus::OFF;
    CoastingWarnOnOffStatus mCoastingWarnOnOffStatus = CoastingWarnOnOffStatus::NON_OPTION;
    NccControlStatus mNccControlStatus = NccControlStatus::OFF;
    SscControlStatus mSscControlStatus = SscControlStatus::OFF;
    PreShiftCoastingDisplayStatus mPreShiftCoastingDisplayStatus = PreShiftCoastingDisplayStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Coasting_H
