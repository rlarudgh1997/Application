/**
 * @file IMS.hpp
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
#ifndef SFSS_IMS_H
#define SFSS_IMS_H

#define DLOG_ENABLED gEnableSFCLog

#include "IMSBase.hpp"

namespace ccos {

// SFC Version : 2.2.1
// Reference : [EC070] IMS. Contains : Event, Sound. Recommend: VALUE_CHANGED
class IMS : public IMSBase {
public:
    IMS() = default;
    ~IMS() override = default;
    IMS(const IMS& other) = delete;
    IMS(IMS&& other) noexcept = delete;
    IMS& operator=(const IMS& other) = delete;
    IMS& operator=(IMS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCIMSEventMemory1ReqStatusID("E61601");
        setSFCIMSEventMemory2ReqStatusID("E61602");
        setSFCIMSEventMemoryEnableStatusID("E61604");
        setSFCIMSEventPlay1ReqStatusID("E61605");
        setSFCIMSEventPlay2ReqStatusID("E61606");
        setSFCIMSEventPlayStopReqStatusID("E61608");
        setSFCIMSEventNotWorkingID("E61610");

        setSFCIMSSoundIMSID("SND_IMSBuzzer");
        setSFCIMSSoundIMSType(SFCIMSSoundIMSType::REPEAT_COUNT);
        setSFCIMSSoundIMSRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onImsMemory1ReqStatusChanged(const ArgumentsImsMemory1ReqStatusChanged& args) {
        if (args.mInput_ImsMemory1ReqStatus == ImsMemory1ReqStatus::ON) {
            setSFCIMSEventMemory1ReqStatusStat(SFCIMSEventMemory1ReqStatusStat::ON);
        } else {
            setSFCIMSEventMemory1ReqStatusStat(SFCIMSEventMemory1ReqStatusStat::OFF);
        }
    }

    void onImsMemory2ReqStatusChanged(const ArgumentsImsMemory2ReqStatusChanged& args) {
        if (args.mInput_ImsMemory2ReqStatus == ImsMemory2ReqStatus::ON) {
            setSFCIMSEventMemory2ReqStatusStat(SFCIMSEventMemory2ReqStatusStat::ON);
        } else {
            setSFCIMSEventMemory2ReqStatusStat(SFCIMSEventMemory2ReqStatusStat::OFF);
        }
    }

    void onImsMemoryEnableStatusChanged(const ArgumentsImsMemoryEnableStatusChanged& args) {
        if (args.mInput_ImsMemoryEnableStatus == ImsMemoryEnableStatus::ON) {
            setSFCIMSEventMemoryEnableStatusStat(SFCIMSEventMemoryEnableStatusStat::ON);
        } else {
            setSFCIMSEventMemoryEnableStatusStat(SFCIMSEventMemoryEnableStatusStat::OFF);
        }
    }

    void onImsPlay1ReqStatusChanged(const ArgumentsImsPlay1ReqStatusChanged& args) {
        if (args.mInput_ImsPlay1ReqStatus == ImsPlay1ReqStatus::ON) {
            setSFCIMSEventPlay1ReqStatusStat(SFCIMSEventPlay1ReqStatusStat::ON);
        } else {
            setSFCIMSEventPlay1ReqStatusStat(SFCIMSEventPlay1ReqStatusStat::OFF);
        }
    }

    void onImsPlay2ReqStatusChanged(const ArgumentsImsPlay2ReqStatusChanged& args) {
        if (args.mInput_ImsPlay2ReqStatus == ImsPlay2ReqStatus::ON) {
            setSFCIMSEventPlay2ReqStatusStat(SFCIMSEventPlay2ReqStatusStat::ON);
        } else {
            setSFCIMSEventPlay2ReqStatusStat(SFCIMSEventPlay2ReqStatusStat::OFF);
        }
    }

    void onImsPlayStopReqStatusChanged(const ArgumentsImsPlayStopReqStatusChanged& args) {
        if (args.mInput_ImsPlayStopReqStatus == ImsPlayStopReqStatus::ON) {
            setSFCIMSEventPlayStopReqStatusStat(SFCIMSEventPlayStopReqStatusStat::ON);
        } else {
            setSFCIMSEventPlayStopReqStatusStat(SFCIMSEventPlayStopReqStatusStat::OFF);
        }
    }

    void onImsErrorWarnStatusChanged(const ArgumentsImsErrorWarnStatusChanged& args) {
        if (args.mInput_ImsErrorWarnStatus == ImsErrorWarnStatus::ON) {
            setSFCIMSEventNotWorkingStat(SFCIMSEventNotWorkingStat::ON);
        } else {
            setSFCIMSEventNotWorkingStat(SFCIMSEventNotWorkingStat::OFF);
        }
    }

    void onImsSoundStatusChanged(const ArgumentsImsSoundStatusChanged& args) {
        if (args.mInput_ImsSoundStatus == ImsSoundStatus::WARN) {
            setSFCIMSSoundIMSStat(SFCIMSSoundIMSStat::ON);
        } else {
            setSFCIMSSoundIMSStat(SFCIMSSoundIMSStat::OFF);
        }
    }

private:
    using ImsMemory1ReqStatus = ArgumentsImsMemory1ReqStatusChanged::Input_ImsMemory1ReqStatus;
    using ImsMemory2ReqStatus = ArgumentsImsMemory2ReqStatusChanged::Input_ImsMemory2ReqStatus;
    using ImsMemoryEnableStatus = ArgumentsImsMemoryEnableStatusChanged::Input_ImsMemoryEnableStatus;
    using ImsPlay1ReqStatus = ArgumentsImsPlay1ReqStatusChanged::Input_ImsPlay1ReqStatus;
    using ImsPlay2ReqStatus = ArgumentsImsPlay2ReqStatusChanged::Input_ImsPlay2ReqStatus;
    using ImsPlayStopReqStatus = ArgumentsImsPlayStopReqStatusChanged::Input_ImsPlayStopReqStatus;
    using ImsErrorWarnStatus = ArgumentsImsErrorWarnStatusChanged::Input_ImsErrorWarnStatus;
    using ImsSoundStatus = ArgumentsImsSoundStatusChanged::Input_ImsSoundStatus;
};

}  // namespace ccos

#endif  // SFSS_IMS_H
