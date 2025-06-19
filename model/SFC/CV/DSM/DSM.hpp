/**
 * @file DSM.hpp
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
#ifndef SFSS_DSM_H
#define SFSS_DSM_H

#define DLOG_ENABLED gEnableSFCLog

#include "DSMBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.22
// Reference : [CV890] DSM. Contains : Event. Recommend: VALUE_CHANGED
class DSM : public DSMBase {
public:
    DSM() = default;
    ~DSM() override = default;
    DSM(const DSM& other) = delete;
    DSM(DSM&& other) noexcept = delete;
    DSM& operator=(const DSM& other) = delete;
    DSM& operator=(DSM&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDSMEventCVDSMStatusID("E78901");
        setSFCDSMEventCVDSMStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCDSMEventCVDSMStatusLinkedSoundType(SFCDSMEventCVDSMStatusLinkedSoundType::REPEAT_COUNT);
        setSFCDSMEventCVDSMStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEventCVDSMStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEventCVDSMStatus();
    }

    void onCVDSMStatusChanged(const ArgumentsCVDSMStatusChanged& args) {
        mCVDSMStatus = args.mInput_CVDSMStatus;
        updateEventCVDSMStatus();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigDSM = args.mInter_ConfigDSM;
        updateEventCVDSMStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventCVDSMStatus() {
        if (mConfigDSM != ConfigDSM::ON) {
            return;
        }

        SFCDSMEventCVDSMStatusStat stat = SFCDSMEventCVDSMStatusStat::OFF;

        if (mIsIgnOn == true && (mCVDSMStatus == CVDSMStatus::ON || mCVDSMStatus == CVDSMStatus::MESSAGE_TIMEOUT)) {
            stat = SFCDSMEventCVDSMStatusStat::ON;
        }
        setSFCDSMEventCVDSMStatusStat(stat);
    }

    using CVDSMStatus = ArgumentsCVDSMStatusChanged::Input_CVDSMStatus;
    using ConfigDSM = ArgumentsConfigChanged::Inter_ConfigDSM;

    HBool mIsIgnOn = false;
    CVDSMStatus mCVDSMStatus = CVDSMStatus::OFF;
    ConfigDSM mConfigDSM = ConfigDSM::OFF;
};

}  // namespace ccos

#endif  // SFSS_DSM_H
