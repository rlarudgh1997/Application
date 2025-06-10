/**
 * @file TCF.hpp
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
#ifndef SFSS_TCF_H
#define SFSS_TCF_H

#define DLOG_ENABLED gEnableSFCLog

#include "TCFBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [CH170] TCF. Contains : Event. Recommend: VALUE_CHANGED
class TCF : public TCFBase {
public:
    TCF() = default;
    ~TCF() override = default;
    TCF(const TCF& other) = delete;
    TCF(TCF&& other) noexcept = delete;
    TCF& operator=(const TCF& other) = delete;
    TCF& operator=(TCF&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTCFEventTCFID("E31201");
        setSFCTCFEventTCFLinkedSoundID("SND_PopUpInform2");
        setSFCTCFEventTCFLinkedSoundType(SFCTCFEventTCFLinkedSoundType::REPEAT_COUNT);
        setSFCTCFEventTCFLinkedSoundRepeatCount(1);
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

    void onTrafficChangeFunctionChanged(const ArgumentsTrafficChangeFunctionChanged& args) {
        mTrafficChangeFunctionStatusFromAFS = args.mInput_TrafficChangeFunctionStatusFromAFS;
        mTrafficChangeFunctionStatusFromIFS = args.mInput_TrafficChangeFunctionStatusFromIFS;
        mTrafficChangeFunctionStatusFromILCULH = args.mInput_TrafficChangeFunctionStatusFromILCULH;
        mTrafficChangeFunctionStatusFromILCURH = args.mInput_TrafficChangeFunctionStatusFromILCURH;
        updateEvent();
    }

    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mTrafficChangeFunctionStatusFromAFS == TrafficChangeFunctionStatusFromAFS::ON ||
             mTrafficChangeFunctionStatusFromIFS == TrafficChangeFunctionStatusFromIFS::ON ||
             (mTrafficChangeFunctionStatusFromILCULH == TrafficChangeFunctionStatusFromILCULH::ON &&
              mTrafficChangeFunctionStatusFromILCURH == TrafficChangeFunctionStatusFromILCURH::ON))) {
            setSFCTCFEventTCFStat(SFCTCFEventTCFStat::ON);
        } else {
            setSFCTCFEventTCFStat(SFCTCFEventTCFStat::OFF);
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using TrafficChangeFunctionStatusFromAFS = ArgumentsTrafficChangeFunctionChanged::Input_TrafficChangeFunctionStatusFromAFS;
    using TrafficChangeFunctionStatusFromIFS = ArgumentsTrafficChangeFunctionChanged::Input_TrafficChangeFunctionStatusFromIFS;
    using TrafficChangeFunctionStatusFromILCULH =
        ArgumentsTrafficChangeFunctionChanged::Input_TrafficChangeFunctionStatusFromILCULH;
    using TrafficChangeFunctionStatusFromILCURH =
        ArgumentsTrafficChangeFunctionChanged::Input_TrafficChangeFunctionStatusFromILCURH;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    TrafficChangeFunctionStatusFromAFS mTrafficChangeFunctionStatusFromAFS = TrafficChangeFunctionStatusFromAFS::OFF;
    TrafficChangeFunctionStatusFromIFS mTrafficChangeFunctionStatusFromIFS = TrafficChangeFunctionStatusFromIFS::OFF;
    TrafficChangeFunctionStatusFromILCULH mTrafficChangeFunctionStatusFromILCULH = TrafficChangeFunctionStatusFromILCULH::OFF;
    TrafficChangeFunctionStatusFromILCURH mTrafficChangeFunctionStatusFromILCURH = TrafficChangeFunctionStatusFromILCURH::OFF;
};

}  // namespace ccos

#endif  // SFSS_TCF_H
