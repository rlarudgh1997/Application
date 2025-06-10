/**
 * @file HEV_Engine_Warning.hpp
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
#ifndef SFSS_HEV_Engine_Warning_H
#define SFSS_HEV_Engine_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "HEV_Engine_WarningBase.hpp"

namespace ccos {

// SFC Version : 0.0.2
// Reference : [PT250] HEV Engine Warning. Contains : Event. Recommend: VALUE_CHANGED
class HEV_Engine_Warning : public HEV_Engine_WarningBase {
public:
    HEV_Engine_Warning() = default;
    ~HEV_Engine_Warning() override = default;
    HEV_Engine_Warning(const HEV_Engine_Warning& other) = delete;
    HEV_Engine_Warning(HEV_Engine_Warning&& other) noexcept = delete;
    HEV_Engine_Warning& operator=(const HEV_Engine_Warning& other) = delete;
    HEV_Engine_Warning& operator=(HEV_Engine_Warning&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateEventHevEngineOffReq();
        updateEventHevSysFailNoStartReq();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateEventHevEngineOffReq();
        updateEventHevSysFailNoStartReq();
    }

    void onInputHevEngineOffReqStatusChanged(const ArgumentsInputHevEngineOffReqStatusChanged& args) {
        mHevEngineOffReqStatus = args.mInput_HevEngineOffReqStatus;
        updateEventHevEngineOffReq();
    }

    void onInputHevSysFailNoStartReqStatusChanged(const ArgumentsInputHevSysFailNoStartReqStatusChanged& args) {
        mHevSysFailNoStartReqStatus = args.mInput_HevSysFailNoStartReqStatus;
        updateEventHevSysFailNoStartReq();
    }

private:
    void updateEventHevEngineOffReq() {
        HBool eventStat = false;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mHevEngineOffReqStatus == HevEngineOffReqStatus::OFF_REQ) {
                eventStat = true;
            }
        }

        setSFCHEV_Engine_WarningEventCheckHEVEngineOffID("E21701");
        setSFCHEV_Engine_WarningEventCheckHEVEngineOffLinkedSoundID("SND_PopUpWarn2");
        setSFCHEV_Engine_WarningEventCheckHEVEngineOffLinkedSoundType(
            SFCHEV_Engine_WarningEventCheckHEVEngineOffLinkedSoundType::DURATION);
        setSFCHEV_Engine_WarningEventCheckHEVEngineOffLinkedSoundDuration(10000);
        if (eventStat) {
            setSFCHEV_Engine_WarningEventCheckHEVEngineOffStat(SFCHEV_Engine_WarningEventCheckHEVEngineOffStat::ON);
        } else {
            setSFCHEV_Engine_WarningEventCheckHEVEngineOffStat(SFCHEV_Engine_WarningEventCheckHEVEngineOffStat::OFF);
        }
    }

    void updateEventHevSysFailNoStartReq() {
        HBool eventStat = false;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mHevSysFailNoStartReqStatus == HevSysFailNoStartReqStatus::NO_START_REQ) {
                eventStat = true;
            }
        }

        setSFCHEV_Engine_WarningEventCheckHEVNoStartID("E21702");
        setSFCHEV_Engine_WarningEventCheckHEVNoStartLinkedSoundID("SND_PopUpWarn1");
        setSFCHEV_Engine_WarningEventCheckHEVNoStartLinkedSoundType(
            SFCHEV_Engine_WarningEventCheckHEVNoStartLinkedSoundType::REPEAT_COUNT);
        setSFCHEV_Engine_WarningEventCheckHEVNoStartLinkedSoundRepeatCount(1);
        if (eventStat) {
            setSFCHEV_Engine_WarningEventCheckHEVNoStartStat(SFCHEV_Engine_WarningEventCheckHEVNoStartStat::ON);
        } else {
            setSFCHEV_Engine_WarningEventCheckHEVNoStartStat(SFCHEV_Engine_WarningEventCheckHEVNoStartStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using HevEngineOffReqStatus = ArgumentsInputHevEngineOffReqStatusChanged::Input_HevEngineOffReqStatus;
    using HevSysFailNoStartReqStatus = ArgumentsInputHevSysFailNoStartReqStatusChanged::Input_HevSysFailNoStartReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HevEngineOffReqStatus mHevEngineOffReqStatus = HevEngineOffReqStatus::DEFAULT;
    HevSysFailNoStartReqStatus mHevSysFailNoStartReqStatus = HevSysFailNoStartReqStatus::DEFAULT;
};

}  // namespace ccos

#endif  // SFSS_HEV_Engine_Warning_H
