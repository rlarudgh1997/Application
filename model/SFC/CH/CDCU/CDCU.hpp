/**
 * @file CDCU.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_CDCU_H
#define SFSS_CDCU_H

#define DLOG_ENABLED gEnableSFCLog

#include "CDCUBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [CH200] CDCU. Contains : Event. Recommend: VALUE_CHANGED
class CDCU : public CDCUBase {
public:
    CDCU() = default;
    ~CDCU() override = default;
    CDCU(const CDCU& other) = delete;
    CDCU(CDCU&& other) noexcept = delete;
    CDCU& operator=(const CDCU& other) = delete;
    CDCU& operator=(CDCU&& other) noexcept = delete;

    void onInitialize() override {
        setSFCCDCUEventCDCUID("E31701");
        setSFCCDCUEventCDCUStat(SFCCDCUEventCDCUStat::OFF);
        setSFCCDCUEventCDCULinkedSoundID("SND_PopUpWarn1");
        setSFCCDCUEventCDCULinkedSoundType(SFCCDCUEventCDCULinkedSoundType::REPEAT_COUNT);
        setSFCCDCUEventCDCULinkedSoundRepeatCount(1);
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

    void onCdcuWarnChanged(const ArgumentsCdcuWarnChanged& args) {
        mCdcuWarnStatus = args.mInput_CDCUWarnStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mCdcuWarnStatus == CDCUWarnStatus::FAILURE) {
            setSFCCDCUEventCDCUStat(SFCCDCUEventCDCUStat::ON);
        } else {
            setSFCCDCUEventCDCUStat(SFCCDCUEventCDCUStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using CDCUWarnStatus = ArgumentsCdcuWarnChanged::Input_CDCUWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    CDCUWarnStatus mCdcuWarnStatus = CDCUWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_CDCU_H
