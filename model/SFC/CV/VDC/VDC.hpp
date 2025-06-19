/**
 * @file VDC.hpp
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
#ifndef SFSS_VDC_H
#define SFSS_VDC_H

#define DLOG_ENABLED gEnableSFCLog

#include "VDCBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.23
// Reference : [CV340] VDC. Contains : Event. Recommend: VALUE_CHANGED
class VDC : public VDCBase {
public:
    VDC() = default;
    ~VDC() override = default;
    VDC(const VDC& other) = delete;
    VDC(VDC&& other) noexcept = delete;
    VDC& operator=(const VDC& other) = delete;
    VDC& operator=(VDC&& other) noexcept = delete;

    void onInitialize() override {
        setSFCVDCEventVDCOffID("E73402");
        setSFCVDCEventVDCOffLinkedSoundID("SND_PopUpInform1");
        setSFCVDCEventVDCOffLinkedSoundType(SFCVDCEventVDCOffLinkedSoundType::REPEAT_COUNT);
        setSFCVDCEventVDCOffLinkedSoundRepeatCount(1);

        setSFCVDCEventTCSOffID("E73401");
        setSFCVDCEventTCSOffLinkedSoundID("SND_PopUpInform1");
        setSFCVDCEventTCSOffLinkedSoundType(SFCVDCEventTCSOffLinkedSoundType::REPEAT_COUNT);
        setSFCVDCEventTCSOffLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateEventTCSOff();
        updateEventVDCOff();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateEventTCSOff();
        updateEventVDCOff();
    }

    void onConfigEBSChanged(const ArgumentsConfigEBSChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;

        updateEventVDCOff();
        updateEventTCSOff();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;

        updateEventVDCOff();
        updateEventTCSOff();
    }

    void onTcsOffStatusChanged(const ArgumentsTcsOffStatusChanged& args) {
        mTCSOffLampStatus = args.mInput_TCSOffLampStatus;
        mTCSOffSoundStatus = args.mInput_TCSOffSoundStatus;

        updateEventTCSOff();
        updateEventVDCOff();
    }

    void onVdcOffStatusChanged(const ArgumentsVdcOffStatusChanged& args) {
        mASROffroadSwitchStatus = args.mInput_ASROffroadSwitchStatus;
        mESPModeStatus = args.mInput_ESPModeStatus;

        updateEventVDCOff();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventTCSOff() {
        if (mConfigEBS != ConfigEBS::ABSNVDC && mConfigEBS != ConfigEBS::EBSNVDC) {
            return;
        }

        SFCVDCEventTCSOffStat stat = SFCVDCEventTCSOffStat::OFF;

        if (mIsIgnOn == true) {
            if ((mConfigEBS == ConfigEBS::ABSNVDC && mConfigBrakeAirType == ConfigBrakeAirType::OFF &&
                 mTCSOffLampStatus == TCSOffLampStatus::ON && mTCSOffSoundStatus == TCSOffSoundStatus::OFF)) {
                stat = SFCVDCEventTCSOffStat::ON;
            }
        }

        setSFCVDCEventTCSOffStat(stat);
    }

    void updateEventVDCOff() {
        if (mConfigEBS != ConfigEBS::ABSNVDC && mConfigEBS != ConfigEBS::EBSNVDC) {
            return;
        }

        SFCVDCEventVDCOffStat stat = SFCVDCEventVDCOffStat::OFF;
        if (mIsIgnOn == true) {
            if ((mConfigEBS == ConfigEBS::ABSNVDC && mConfigBrakeAirType == ConfigBrakeAirType::OFF &&
                 mTCSOffLampStatus == TCSOffLampStatus::ON && mTCSOffSoundStatus == TCSOffSoundStatus::ON) ||
                ((mConfigEBS == ConfigEBS::ABSNVDC || mConfigEBS == ConfigEBS::EBSNVDC) &&
                 mConfigBrakeAirType != ConfigBrakeAirType::OFF && mASROffroadSwitchStatus == ASROffroadSwitchStatus::ON) ||
                (mConfigEBS == ConfigEBS::EBSNVDC && mConfigBrakeAirType != ConfigBrakeAirType::OFF &&
                 mESPModeStatus == ESPModeStatus::VDCOFF_ON)) {
                stat = SFCVDCEventVDCOffStat::ON;
            }
        }
        setSFCVDCEventVDCOffStat(stat);
    }

    HBool mIsIgnOn = false;

    using ConfigEBS = ArgumentsConfigEBSChanged::Inter_ConfigEBS;
    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;
    using TCSOffLampStatus = ArgumentsTcsOffStatusChanged::Input_TCSOffLampStatus;
    using TCSOffSoundStatus = ArgumentsTcsOffStatusChanged::Input_TCSOffSoundStatus;
    using ASROffroadSwitchStatus = ArgumentsVdcOffStatusChanged::Input_ASROffroadSwitchStatus;
    using ESPModeStatus = ArgumentsVdcOffStatusChanged::Input_ESPModeStatus;

    ConfigEBS mConfigEBS = ConfigEBS::EBSNVDC;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::TYPE_2;
    TCSOffLampStatus mTCSOffLampStatus = TCSOffLampStatus::OFF;
    TCSOffSoundStatus mTCSOffSoundStatus = TCSOffSoundStatus::OFF;
    ASROffroadSwitchStatus mASROffroadSwitchStatus = ASROffroadSwitchStatus::OFF;
    ESPModeStatus mESPModeStatus = ESPModeStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_VDC_H
