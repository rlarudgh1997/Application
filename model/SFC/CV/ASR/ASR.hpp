/**
 * @file ASR.hpp
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
#ifndef SFSS_ASR_H
#define SFSS_ASR_H

#define DLOG_ENABLED gEnableSFCLog

#include "ASRBase.hpp"

namespace ccos {

// SFC Version : 7.0.0
// Spec Version : v0.23
// Reference : [CV120] ASR. Contains : Telltale. Recommend: VALUE_CHANGED
class ASR : public ASRBase {
public:
    ASR() = default;
    ~ASR() override = default;
    ASR(const ASR& other) = delete;
    ASR(ASR&& other) noexcept = delete;
    ASR& operator=(const ASR& other) = delete;
    ASR& operator=(ASR&& other) noexcept = delete;

    void onInitialize() override {
        setSFCASRTelltaleASRBlinkValueA(SFCASRTelltaleASRBlinkValueA::ON);
        setSFCASRTelltaleASRBlinkValueB(SFCASRTelltaleASRBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onConfigEBSChanged(const ArgumentsConfigEBSChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;
        updateTelltale();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;
        updateTelltale();
    }

    void onAsrStatusChanged(const ArgumentsAsrStatusChanged& args) {
        mASRBrakeCtrlActiveStatus = args.mInput_ASRBrakeCtrlActiveStatus;
        mASREngCtrlActiveStatus = args.mInput_ASREngCtrlActiveStatus;
        mASROffroadSwitchStatus = args.mInput_ASROffroadSwitchStatus;
        mTCSOffLampStatus = args.mInput_TCSOffLampStatus;
        mTCSWarningAndInfoLampStatus = args.mInput_TCSWarningAndInfoLampStatus;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCASRTelltaleASRStat stat = SFCASRTelltaleASRStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC || mConfigEBS == ConfigEBS::EBSNASR ||
                mConfigEBS == ConfigEBS::EBSNVDC) {
                stat = SFCASRTelltaleASRStat::ON;
            }
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mConfigBrakeAirType == ConfigBrakeAirType::OFF &&
                (mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC)) {
                if (mTCSOffLampStatus == TCSOffLampStatus::OFF &&
                    mTCSWarningAndInfoLampStatus == TCSWarningAndInfoLampStatus::BLINK2 &&
                    ((mASREngCtrlActiveStatus == ASREngCtrlActiveStatus::ACTIVE &&
                      mASRBrakeCtrlActiveStatus != ASRBrakeCtrlActiveStatus::MESSAGE_TIMEOUT) ||
                     (mASREngCtrlActiveStatus != ASREngCtrlActiveStatus::MESSAGE_TIMEOUT &&
                      mASRBrakeCtrlActiveStatus == ASRBrakeCtrlActiveStatus::ACTIVE))) {
                    stat = SFCASRTelltaleASRStat::BLINK2;
                } else if ((mASREngCtrlActiveStatus == ASREngCtrlActiveStatus::MESSAGE_TIMEOUT &&
                            mASRBrakeCtrlActiveStatus == ASRBrakeCtrlActiveStatus::MESSAGE_TIMEOUT) ||
                           (mTCSWarningAndInfoLampStatus == TCSWarningAndInfoLampStatus::MESSAGE_TIMEOUT &&
                            (mTCSOffLampStatus == TCSOffLampStatus::ON ||
                             mTCSOffLampStatus == TCSOffLampStatus::MESSAGE_TIMEOUT))) {
                    stat = SFCASRTelltaleASRStat::ON;
                } else {
                    // no operation
                }
            } else if (mConfigBrakeAirType != ConfigBrakeAirType::OFF &&
                       (mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC ||
                        mConfigEBS == ConfigEBS::EBSNASR || mConfigEBS == ConfigEBS::EBSNVDC)) {
                if (mASROffroadSwitchStatus == ASROffroadSwitchStatus::OFF &&
                    ((mASREngCtrlActiveStatus == ASREngCtrlActiveStatus::ACTIVE &&
                      mASRBrakeCtrlActiveStatus != ASRBrakeCtrlActiveStatus::MESSAGE_TIMEOUT) ||
                     (mASREngCtrlActiveStatus != ASREngCtrlActiveStatus::MESSAGE_TIMEOUT &&
                      mASRBrakeCtrlActiveStatus == ASRBrakeCtrlActiveStatus::ACTIVE))) {
                    stat = SFCASRTelltaleASRStat::BLINK2;
                } else if ((mASREngCtrlActiveStatus == ASREngCtrlActiveStatus::MESSAGE_TIMEOUT &&
                            mASRBrakeCtrlActiveStatus == ASRBrakeCtrlActiveStatus::MESSAGE_TIMEOUT) ||
                           mASROffroadSwitchStatus == ASROffroadSwitchStatus::ON ||
                           mASROffroadSwitchStatus == ASROffroadSwitchStatus::MESSAGE_TIMEOUT) {
                    stat = SFCASRTelltaleASRStat::ON;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCASRTelltaleASRStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using ConfigEBS = ArgumentsConfigEBSChanged::Inter_ConfigEBS;
    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;
    using ASRBrakeCtrlActiveStatus = ArgumentsAsrStatusChanged::Input_ASRBrakeCtrlActiveStatus;
    using ASREngCtrlActiveStatus = ArgumentsAsrStatusChanged::Input_ASREngCtrlActiveStatus;
    using ASROffroadSwitchStatus = ArgumentsAsrStatusChanged::Input_ASROffroadSwitchStatus;
    using TCSOffLampStatus = ArgumentsAsrStatusChanged::Input_TCSOffLampStatus;
    using TCSWarningAndInfoLampStatus = ArgumentsAsrStatusChanged::Input_TCSWarningAndInfoLampStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigEBS mConfigEBS = ConfigEBS::EBSNVDC;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::OFF;
    ASRBrakeCtrlActiveStatus mASRBrakeCtrlActiveStatus = ASRBrakeCtrlActiveStatus::ACTIVE;
    ASREngCtrlActiveStatus mASREngCtrlActiveStatus = ASREngCtrlActiveStatus::ACTIVE;
    ASROffroadSwitchStatus mASROffroadSwitchStatus = ASROffroadSwitchStatus::OFF;
    TCSOffLampStatus mTCSOffLampStatus = TCSOffLampStatus::OFF;
    TCSWarningAndInfoLampStatus mTCSWarningAndInfoLampStatus = TCSWarningAndInfoLampStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_ASR_H
