/**
 * @file AMS.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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
#ifndef SFSS_AMS_H
#define SFSS_AMS_H

#define DLOG_ENABLED gEnableSFCLog

#include "AMSBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : 0.21
// Reference : [CV110] AMS. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class AMS : public AMSBase {
public:
    AMS() = default;
    ~AMS() override = default;
    AMS(const AMS& other) = delete;
    AMS(AMS&& other) noexcept = delete;
    AMS& operator=(const AMS& other) = delete;
    AMS& operator=(AMS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAMSEventAMSEvent71101To71103LinkedSoundType(SFCAMSEventAMSEvent71101To71103LinkedSoundType::REPEAT_COUNT);
        setSFCAMSEventAMSEvent71101To71103LinkedSoundRepeatCount(1);

        setSFCAMSEventAMSEvent71104ID("E71104");
        setSFCAMSEventAMSEvent71104LinkedSoundID("SND_PopUpInform2");
        setSFCAMSEventAMSEvent71104LinkedSoundType(SFCAMSEventAMSEvent71104LinkedSoundType::REPEAT_COUNT);
        setSFCAMSEventAMSEvent71104LinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIsIgnOn = true;
        updateTelltaleAMSMILLamp();
        updateEvent71101To71103();
        updateEvent71104();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIsIgnOn = false;
        updateTelltaleAMSMILLamp();
        updateEvent71101To71103();
        updateEvent71104();
    }

    void onAmsMILLampStatusChanged(const ArgumentsAmsMILLampStatusChanged& args) {
        mAMSMILLampStatus = args.mInput_AMSMILLampStatus;
        updateTelltaleAMSMILLamp();
    }

    void onAmsInfoStatusChanged(const ArgumentsAmsInfoStatusChanged& args) {
        mAMSInfoStatus = args.mInput_AMSInfoStatus;
        updateEvent71101To71103();
    }

    void onAmsSystemStatusChanged(const ArgumentsAmsSystemStatusChanged& args) {
        mAMSSystemStatus = args.mInput_AMSSystemStatus;
        updateEvent71104();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleAMSMILLamp() {
        SFCAMSTelltaleAMSMILLampStat stat = SFCAMSTelltaleAMSMILLampStat::OFF;
        if (mIsIgnOn == true && mAMSMILLampStatus == AMSMILLampStatus::ON) {
            stat = SFCAMSTelltaleAMSMILLampStat::ON;
        }
        setSFCAMSTelltaleAMSMILLampStat(stat);
    }

    void updateEvent71101To71103() {
        std::string id;
        std::string linkedSoundID;
        std::string prevEventID;
        if (mIsIgnOn == true) {
            switch (mAMSInfoStatus) {
                case AMSInfoStatus::ERROR1:
                    id = "E71101";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case AMSInfoStatus::ERROR2:
                    id = "E71102";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case AMSInfoStatus::ERROR3:
                    id = "E71103";
                    linkedSoundID = "SND_PopUpWarn2";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.AMS.Event.AMSEvent71101To71103.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != id) {
            setSFCAMSEventAMSEvent71101To71103Stat(SFCAMSEventAMSEvent71101To71103Stat::OFF);
            flushLastGroup();
        }
        if (id.empty() == false) {
            setSFCAMSEventAMSEvent71101To71103ID(id);
            setSFCAMSEventAMSEvent71101To71103Stat(SFCAMSEventAMSEvent71101To71103Stat::ON);
            setSFCAMSEventAMSEvent71101To71103LinkedSoundID(linkedSoundID);
        }
    }

    void updateEvent71104() {
        SFCAMSEventAMSEvent71104Stat stat = SFCAMSEventAMSEvent71104Stat::OFF;
        if (mIsIgnOn == true && mAMSSystemStatus == AMSSystemStatus::ON) {
            stat = SFCAMSEventAMSEvent71104Stat::ON;
        }
        setSFCAMSEventAMSEvent71104Stat(stat);
    }

    HBool mIsIgnOn = false;

    using AMSMILLampStatus = ArgumentsAmsMILLampStatusChanged::Input_AMSMILLampStatus;
    using AMSInfoStatus = ArgumentsAmsInfoStatusChanged::Input_AMSInfoStatus;
    using AMSSystemStatus = ArgumentsAmsSystemStatusChanged::Input_AMSSystemStatus;

    AMSInfoStatus mAMSInfoStatus = AMSInfoStatus::OFF;
    AMSMILLampStatus mAMSMILLampStatus = AMSMILLampStatus::OFF;
    AMSSystemStatus mAMSSystemStatus = AMSSystemStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_AMS_H
