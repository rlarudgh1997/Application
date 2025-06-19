/**
 * @file SOC_Gauge_CV.hpp
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
#ifndef SFSS_SOC_Gauge_CV_H
#define SFSS_SOC_Gauge_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "SOC_Gauge_CVBase.hpp"
#include <algorithm>
#include <array>
#include <cmath>

namespace ccos {

// SFC Version : 7.0.0
// Spec Version : v0.37
// Reference : [CV630] SOC_Gauge_CV. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class SOC_Gauge_CV : public SOC_Gauge_CVBase {
public:
    SOC_Gauge_CV() = default;
    ~SOC_Gauge_CV() override = default;
    SOC_Gauge_CV(const SOC_Gauge_CV& other) = delete;
    SOC_Gauge_CV(SOC_Gauge_CV&& other) noexcept = delete;
    SOC_Gauge_CV& operator=(const SOC_Gauge_CV& other) = delete;
    SOC_Gauge_CV& operator=(SOC_Gauge_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSOC_Gauge_CVTelltaleLowSOCBlinkValueA(SFCSOC_Gauge_CVTelltaleLowSOCBlinkValueA::ON);
        setSFCSOC_Gauge_CVTelltaleLowSOCBlinkValueB(SFCSOC_Gauge_CVTelltaleLowSOCBlinkValueB::OFF);
        setSFCSOC_Gauge_CVConstantLowAlertBlinkValueA(SFCSOC_Gauge_CVConstantLowAlertBlinkValueA::RED);
        setSFCSOC_Gauge_CVConstantLowAlertBlinkValueB(SFCSOC_Gauge_CVConstantLowAlertBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updatePrivateBMSMainDisplaySocValue();
        updatePrivateBMSSlaveMinSocValue();
        updatePrivateSOCgaugeTotalValue();
        updateTelltale();
        updateConstantLowAlert();
        updateEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updatePrivateBMSMainDisplaySocValue();
        updatePrivateBMSSlaveMinSocValue();
        updatePrivateSOCgaugeTotalValue();
        updateTelltale();
        updateConstantLowAlert();
        updateEvent();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updatePrivateBMSMainDisplaySocValue();
        updatePrivateBMSSlaveMinSocValue();
        updatePrivateSOCgaugeTotalValue();
        updateTelltale();
        updateConstantLowAlert();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updatePrivateBMSMainDisplaySocValue();
        updateTelltale();
        updateConstantSOCgaugeValue();
        updateConstantLowAlert();
        updateEvent();
    }

    void onBmsDisplaySocValueChanged(const ArgumentsBmsDisplaySocValueChanged& args) {
        mBmsDisplaySocValue = args.mInput_BmsDisplaySocValue;
        updatePrivateBMSMainDisplaySocValue();
        updatePrivateSOCgaugeTotalValue();
    }

    void onEvLowSocWarnCVStatusChanged(const ArgumentsEvLowSocWarnCVStatusChanged& args) {
        mEvLowSocWarnCVStatus = args.mInput_EvLowSocWarnCVStatus;
        updateTelltale();
        updateConstantLowAlert();
        updateEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updatePrivateBMSMainDisplaySocValue();
        updatePrivateBMSSlaveMinSocValue();
        updatePrivateSOCgaugeTotalValue();
        updateTelltale();
        updateConstantLowAlert();
        updateEvent();
    }

    void onBmsSlaveDisplaySocValueChanged(const ArgumentsBmsSlaveDisplaySocValueChanged& args) {
        mBMSSlaveDisplaySocValue1 = args.mInput_BMSSlaveDisplaySocValue1;
        mBMSSlaveDisplaySocValue2 = args.mInput_BMSSlaveDisplaySocValue2;
        mBMSSlaveDisplaySocValue3 = args.mInput_BMSSlaveDisplaySocValue3;
        mBMSSlaveDisplaySocValue4 = args.mInput_BMSSlaveDisplaySocValue4;
        mBMSSlaveDisplaySocValue5 = args.mInput_BMSSlaveDisplaySocValue5;
        mBMSSlaveDisplaySocValue6 = args.mInput_BMSSlaveDisplaySocValue6;
        updatePrivateBMSSlaveMinSocValue();
        updatePrivateSOCgaugeTotalValue();
    }

    void onVcuEmergencyMainRelayOnReqChanged(const ArgumentsVcuEmergencyMainRelayOnReqChanged& args) {
        mVCUEmergencyMainRelayOnReq = args.mInput_VCUEmergencyMainRelayOnReq;
        updatePrivateSOCgaugeTotalValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1 Main BMS SOC 값 계산
    void updatePrivateBMSMainDisplaySocValue() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }

        mPrivate_BMSMainDisplaySocValue = 255;
        if (mIsIgnOn == true && 0 <= mBmsDisplaySocValue && mBmsDisplaySocValue <= 200) {
            mPrivate_BMSMainDisplaySocValue = round(static_cast<HDouble>(mBmsDisplaySocValue) * 0.5);
        }
    }

    // 4.2 Slave BMS SOC 최소값 계산
    void updatePrivateBMSSlaveMinSocValue() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }

        std::array<HUInt64, 6> values = {mBMSSlaveDisplaySocValue1, mBMSSlaveDisplaySocValue2, mBMSSlaveDisplaySocValue3,
                                         mBMSSlaveDisplaySocValue4, mBMSSlaveDisplaySocValue5, mBMSSlaveDisplaySocValue6};
        HUInt64 minVal = *std::min_element(values.begin(), values.end());

        if (mIsIgnOn == true) {
            mPrivate_BMSSlaveMinSocValue = round(static_cast<HDouble>(minVal) * 0.5);
        }
    }

    // 4.3 SOC 최종값 계산
    void updatePrivateSOCgaugeTotalValue() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }

        if (mIsIgnOn == true) {
            if (mVCUEmergencyMainRelayOnReq == VCUEmergencyMainRelayOnReq::OFF) {
                mPrivate_SOCgaugeTotalValue = mPrivate_BMSMainDisplaySocValue;
            } else if (mVCUEmergencyMainRelayOnReq == VCUEmergencyMainRelayOnReq::ON) {
                mPrivate_SOCgaugeTotalValue = mPrivate_BMSSlaveMinSocValue;
            } else {
                // no operation
            }
        }
        updateConstantSOCgaugeValue();
    }

    void updateTelltale() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }
        SFCSOC_Gauge_CVTelltaleLowSOCStat stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms && mConfigVehicleType == ConfigVehicleType::FCEV) {
            switch (mEvLowSocWarnCVStatus) {
                case EvLowSocWarnCVStatus::WARN1:
                    stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::ON;
                    break;
                case EvLowSocWarnCVStatus::WARN2:
                case EvLowSocWarnCVStatus::WARN3:
                    stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::BLINK1;
                    break;
                default:
                    // no operation
                    break;
            }
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms && mConfigVehicleType == ConfigVehicleType::EV) {
            switch (mEvLowSocWarnCVStatus) {
                case EvLowSocWarnCVStatus::WARN2:
                    stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::ON;
                    break;
                case EvLowSocWarnCVStatus::WARN3:
                    stat = SFCSOC_Gauge_CVTelltaleLowSOCStat::BLINK1;
                    break;
                default:
                    // no operation
                    break;
            }
        } else {
            // no operation
        }
        setSFCSOC_Gauge_CVTelltaleLowSOCStat(stat);
    }

    void updateConstantSOCgaugeValue() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }
        HUInt64 percentageValue = 0;
        if (mIsIgnOn == true && 0 <= mPrivate_SOCgaugeTotalValue && mPrivate_SOCgaugeTotalValue <= 100) {
            percentageValue = mPrivate_SOCgaugeTotalValue;
            setSFCSOC_Gauge_CVConstantSOCgaugeStat(SFCSOC_Gauge_CVConstantSOCgaugeStat::VALUE);
            setSFCSOC_Gauge_CVConstantSOCgaugeValue(percentageValue);
        } else {
            setSFCSOC_Gauge_CVConstantSOCgaugeStat(SFCSOC_Gauge_CVConstantSOCgaugeStat::UNMEASURABLE);
        }
    }

    void updateConstantLowAlert() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }
        SFCSOC_Gauge_CVConstantLowAlertStat stat = SFCSOC_Gauge_CVConstantLowAlertStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::FCEV) {
            switch (mEvLowSocWarnCVStatus) {
                case EvLowSocWarnCVStatus::WARN1:
                    stat = SFCSOC_Gauge_CVConstantLowAlertStat::RED;
                    break;
                case EvLowSocWarnCVStatus::WARN2:
                case EvLowSocWarnCVStatus::WARN3:
                    stat = SFCSOC_Gauge_CVConstantLowAlertStat::BLINK2;
                    break;
                default:
                    // no operation
                    break;
            }
        } else if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::EV) {
            switch (mEvLowSocWarnCVStatus) {
                case EvLowSocWarnCVStatus::WARN1:
                    stat = SFCSOC_Gauge_CVConstantLowAlertStat::YELLOW;
                    break;
                case EvLowSocWarnCVStatus::WARN2:
                    stat = SFCSOC_Gauge_CVConstantLowAlertStat::RED;
                    break;
                case EvLowSocWarnCVStatus::WARN3:
                    stat = SFCSOC_Gauge_CVConstantLowAlertStat::BLINK2;
                    break;
                default:
                    // no operation
                    break;
            }
        } else {
            // no operation
        }
        setSFCSOC_Gauge_CVConstantLowAlertStat(stat);
    }

    void updateEvent() {
        if ((mConfigVehicleType != ConfigVehicleType::FCEV) && (mConfigVehicleType != ConfigVehicleType::EV)) {
            return;
        }
        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;
        SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType linkedSoundType = SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType::NONE;
        HUInt64 linkedSoundRepeatCount = 0;
        if (mIsIgnOn == true) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                switch (mEvLowSocWarnCVStatus) {
                    case EvLowSocWarnCVStatus::WARN1:
                        eventID = "E76301";
                        linkedSoundID = "SND_PopUpWarn1";
                        linkedSoundType = SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType::REPEAT_COUNT;
                        linkedSoundRepeatCount = 1;
                        break;
                    case EvLowSocWarnCVStatus::WARN2:
                        eventID = "E76302";
                        linkedSoundID = "SND_PopUpWarn2";
                        linkedSoundType = SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType::REPEAT_COUNT;
                        linkedSoundRepeatCount = 1;
                        break;
                    case EvLowSocWarnCVStatus::WARN3:
                        eventID = "E76303";
                        linkedSoundID = "SND_PopUpWarn2";
                        linkedSoundType = SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType::INFINITE;
                        break;
                    default:
                        break;
                }
            } else if (mConfigVehicleType == ConfigVehicleType::EV && mEvLowSocWarnCVStatus == EvLowSocWarnCVStatus::WARN3) {
                eventID = "E76304";
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType::REPEAT_COUNT;
                linkedSoundRepeatCount = 1;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.SOC_Gauge_CV.Event.EvLowSocWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCSOC_Gauge_CVEventEvLowSocWarnStat(SFCSOC_Gauge_CVEventEvLowSocWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCSOC_Gauge_CVEventEvLowSocWarnID(eventID);
            setSFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundID(linkedSoundID);
            setSFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundType(linkedSoundType);
            setSFCSOC_Gauge_CVEventEvLowSocWarnLinkedSoundRepeatCount(linkedSoundRepeatCount);
            setSFCSOC_Gauge_CVEventEvLowSocWarnStat(SFCSOC_Gauge_CVEventEvLowSocWarnStat::ON);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using EvLowSocWarnCVStatus = ArgumentsEvLowSocWarnCVStatusChanged::Input_EvLowSocWarnCVStatus;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using VCUEmergencyMainRelayOnReq = ArgumentsVcuEmergencyMainRelayOnReqChanged::Input_VCUEmergencyMainRelayOnReq;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    HUInt64 mBmsDisplaySocValue = 0;
    EvLowSocWarnCVStatus mEvLowSocWarnCVStatus = EvLowSocWarnCVStatus::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::FCEV;
    HDouble mPrivate_BMSMainDisplaySocValue = 0;
    HDouble mPrivate_BMSSlaveMinSocValue = 0;
    HUInt64 mBMSSlaveDisplaySocValue1 = 0;
    HUInt64 mBMSSlaveDisplaySocValue2 = 0;
    HUInt64 mBMSSlaveDisplaySocValue3 = 0;
    HUInt64 mBMSSlaveDisplaySocValue4 = 0;
    HUInt64 mBMSSlaveDisplaySocValue5 = 0;
    HUInt64 mBMSSlaveDisplaySocValue6 = 0;
    VCUEmergencyMainRelayOnReq mVCUEmergencyMainRelayOnReq = VCUEmergencyMainRelayOnReq::OFF;
    HDouble mPrivate_SOCgaugeTotalValue = 0;
};

}  // namespace ccos

#endif  // SFSS_SOC_Gauge_CV_H
