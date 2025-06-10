/**
 * @file RWS.hpp
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
#ifndef SFSS_RWS_H
#define SFSS_RWS_H

#define DLOG_ENABLED gEnableSFCLog

#include "RWSBase.hpp"

namespace ccos {

// SFC Version : 0.1.0
// Reference : [CH160] RWS. Contains : Event. Recommend: VALUE_CHANGED
class RWS : public RWSBase {
public:
    RWS() = default;
    ~RWS() override = default;
    RWS(const RWS& other) = delete;
    RWS(RWS&& other) noexcept = delete;
    RWS& operator=(const RWS& other) = delete;
    RWS& operator=(RWS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCRWSEventRwsSysFailWarnID("E31101");
        setSFCRWSEventRwsSysFailWarnStat(SFCRWSEventRwsSysFailWarnStat::OFF);
        setSFCRWSEventRwsSysFailWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCRWSEventRwsSysFailWarnLinkedSoundType(SFCRWSEventRwsSysFailWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRWSEventRwsSysFailWarnLinkedSoundRepeatCount(1);
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

    void onRwsSysFailWarnChanged(const ArgumentsRwsSysFailWarnChanged& args) {
        mRwsSysFailWarnStatus = args.mInput_RwsSysFailWarnStatus;
        updateEvent();
    }

private:
    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mRwsSysFailWarnStatus == RwsSysFailWarnStatus::ON) {
            setSFCRWSEventRwsSysFailWarnStat(SFCRWSEventRwsSysFailWarnStat::ON);
        } else {
            setSFCRWSEventRwsSysFailWarnStat(SFCRWSEventRwsSysFailWarnStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using RwsSysFailWarnStatus = ArgumentsRwsSysFailWarnChanged::Input_RwsSysFailWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    RwsSysFailWarnStatus mRwsSysFailWarnStatus = RwsSysFailWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_RWS_H
