/**
 * @file DVRS.hpp
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
#ifndef SFSS_DVRS_H
#define SFSS_DVRS_H

#define DLOG_ENABLED gEnableSFCLog

#include "DVRSBase.hpp"

namespace ccos {

// SFC Version : 0.1.0
// Reference : [EC030] DVRS. Contains : Event. Recommend: VALUE_CHANGED
class DVRS : public DVRSBase {
public:
    DVRS() = default;
    ~DVRS() override = default;
    DVRS(const DVRS& other) = delete;
    DVRS(DVRS&& other) noexcept = delete;
    DVRS& operator=(const DVRS& other) = delete;
    DVRS& operator=(DVRS&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateWarningEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateWarningEvent();
    }

    void onDvrsSysFailWarnStatusChanged(const ArgumentsDvrsSysFailWarnStatusChanged& args) {
        mDvrsSysFailWarnStatus = args.mInput_DvrsSysFailWarnStatus;
        updateWarningEvent();
    }

    void updateWarningEvent() {
        setSFCDVRSEventDVRSID("E60201");
        setSFCDVRSEventDVRSLinkedSoundID("SND_PopUpWarn1");
        setSFCDVRSEventDVRSLinkedSoundType(SFCDVRSEventDVRSLinkedSoundType::REPEAT_COUNT);
        setSFCDVRSEventDVRSLinkedSoundRepeatCount(1);
        if (mIgnElapsed == IgnElapsed::ON_0ms && mDvrsSysFailWarnStatus == DvrsSysFailWarnStatus::WARN) {
            setSFCDVRSEventDVRSStat(SFCDVRSEventDVRSStat::ON);
        } else {
            setSFCDVRSEventDVRSStat(SFCDVRSEventDVRSStat::OFF);
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

    using DvrsSysFailWarnStatus = ArgumentsDvrsSysFailWarnStatusChanged::Input_DvrsSysFailWarnStatus;
    DvrsSysFailWarnStatus mDvrsSysFailWarnStatus = DvrsSysFailWarnStatus::NO_WARN;
};

}  // namespace ccos

#endif  // SFSS_DVRS_H
