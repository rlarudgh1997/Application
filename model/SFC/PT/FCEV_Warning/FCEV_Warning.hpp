/**
 * @file FCEV_Warning.hpp
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
#ifndef SFSS_FCEV_Warning_H
#define SFSS_FCEV_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "FCEV_WarningBase.hpp"

namespace ccos {

// SFC Version : 7.1.0
// Reference : [PT480] FCEV_Warning. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class FCEV_Warning : public FCEV_WarningBase {
public:
    FCEV_Warning() = default;
    ~FCEV_Warning() override = default;
    FCEV_Warning(const FCEV_Warning& other) = delete;
    FCEV_Warning(FCEV_Warning&& other) noexcept = delete;
    FCEV_Warning& operator=(const FCEV_Warning& other) = delete;
    FCEV_Warning& operator=(FCEV_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCFCEV_WarningEventH2SensorFaultID("E25201");
        setSFCFCEV_WarningEventH2SensorFaultLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_WarningEventH2SensorFaultLinkedSoundType(SFCFCEV_WarningEventH2SensorFaultLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_WarningEventH2SensorFaultLinkedSoundRepeatCount(1);

        setSFCFCEV_WarningEventFcevWaterReleaseWarnLinkedSoundType(
            SFCFCEV_WarningEventFcevWaterReleaseWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_WarningEventFcevWaterReleaseWarnLinkedSoundRepeatCount(1);

        setSFCFCEV_WarningEventFcevSystemWarnLinkedSoundRepeatCount(1);
        setSFCFCEV_WarningEventFcevSystemWarnLinkedSoundDurationMin(0);
        setSFCFCEV_WarningEventFcevSystemWarnLinkedSoundDurationMax(10000);

        setSFCFCEV_WarningEventFcevSystemOffID("E25215");
        setSFCFCEV_WarningEventFcevColdStartStatusID("E25216");
        setSFCFCEV_WarningEventFcevAirFilterWarnID("E25212");
        setSFCFCEV_WarningEventFcevAirFilterWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_WarningEventFcevAirFilterWarnLinkedSoundType(
            SFCFCEV_WarningEventFcevAirFilterWarnLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_WarningEventFcevAirFilterWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIgnStatus = true;
        updateH2FaultTelltale();
        updateH2LeakTelltale();
        updateH2SensorFaultEvent();
        updateH2LeakWarnEvent();
        updateFcevSystemWarnEvent();
        updateFcevWaterReleaseWarnEvent();
        updateFcevColdStartEvent();
        updateFcevSystemOffEvent();
        updateFcevAirFilterWarnEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIgnStatus = true;
        updateH2FaultTelltale();
        updateH2LeakTelltale();
        updateH2SensorFaultEvent();
        updateH2LeakWarnEvent();
        updateFcevSystemWarnEvent();
        updateFcevWaterReleaseWarnEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIgnStatus = false;
        updateH2FaultTelltale();
        updateH2LeakTelltale();
        updateH2SensorFaultEvent();
        updateH2LeakWarnEvent();
        updateFcevSystemWarnEvent();
        updateFcevWaterReleaseWarnEvent();
        updateFcevColdStartEvent();
        updateFcevSystemOffEvent();
        updateFcevAirFilterWarnEvent();
    }

    void onFcevColdStartStatusChanged(const ArgumentsFcevColdStartStatusChanged& args) {
        mFcevColdStartOnOffStatus = args.mInput_FcevColdStartOnOffStatus;
        mFcevColdStartBarStatus = args.mInput_FcevColdStartBarStatus;
        updateFcevColdStartEvent();
    }

    void onH2SensorFaultStatusChanged(const ArgumentsH2SensorFaultStatusChanged& args) {
        mH2SensorFaultStatusFromFCU = args.mInput_H2SensorFaultStatusFromFCU;
        mH2SensorFaultStatusFromHMU = args.mInput_H2SensorFaultStatusFromHMU;
        if (ISTIMEOUT(args.mInput_H2SensorFaultStatusFromFCU)) {
            mH2SensorFaultStatusFromFCU = H2SensorFaultStatusFromFCU::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_H2SensorFaultStatusFromHMU)) {
            mH2SensorFaultStatusFromHMU = H2SensorFaultStatusFromHMU::UNHANDLED_TIMEOUT;
        }
        updateH2FaultTelltale();
        updateH2SensorFaultEvent();
    }

    void onH2LeakIndiReqStatusChanged(const ArgumentsH2LeakIndiReqStatusChanged& args) {
        mH2LeakIndiReqStatusFromFCU = args.mInput_H2LeakIndiReqStatusFromFCU;
        mH2LeakIndiReqStatusFromHMU = args.mInput_H2LeakIndiReqStatusFromHMU;
        if (ISTIMEOUT(args.mInput_H2LeakIndiReqStatusFromFCU)) {
            mH2LeakIndiReqStatusFromFCU = H2LeakIndiReqStatusFromFCU::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_H2LeakIndiReqStatusFromHMU)) {
            mH2LeakIndiReqStatusFromHMU = H2LeakIndiReqStatusFromHMU::UNHANDLED_TIMEOUT;
        }
        updateH2LeakTelltale();
        updateH2LeakWarnEvent();
    }

    void onH2LeakWarnStatusChanged(const ArgumentsH2LeakWarnStatusChanged& args) {
        mH2LeakWarnStatusFromFCU = args.mInput_H2LeakWarnStatusFromFCU;
        mH2LeakWarnStatusFromHMU = args.mInput_H2LeakWarnStatusFromHMU;
        updateH2LeakWarnEvent();
    }

    void onFcevSystemWarnStatusChanged(const ArgumentsFcevSystemWarnStatusChanged& args) {
        mFcevSystemWarnStatus = args.mInput_FcevSystemWarnStatus;
        updateFcevSystemWarnEvent();
    }

    void onFcevWaterReleaseWarnStatusChanged(const ArgumentsFcevWaterReleaseWarnStatusChanged& args) {
        mFcevWaterReleaseWarnStatus = args.mInput_FcevWaterReleaseWarnStatus;
        updateFcevWaterReleaseWarnEvent();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateH2FaultTelltale();
        updateH2LeakTelltale();
        updateH2SensorFaultEvent();
        updateH2LeakWarnEvent();
        updateFcevSystemWarnEvent();
        updateFcevWaterReleaseWarnEvent();
        updateFcevAirFilterWarnEvent();
    }

    void onFcevSystemOffStatusChanged(const ArgumentsFcevSystemOffStatusChanged& args) {
        mFcevSystemOffStatus = args.mInput_FcevSystemOffStatus;
        updateFcevSystemOffEvent();
    }

    void onFcevAirFilterWarnStatusChanged(const ArgumentsFcevAirFilterWarnStatusChanged& args) {
        mFcevAirFilterWarnStatus = args.mInput_FcevAirFilterWarnStatus;
        updateFcevAirFilterWarnEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateH2FaultTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mH2SensorFaultStatusFromFCU == H2SensorFaultStatusFromFCU::FAULT ||
             mH2SensorFaultStatusFromFCU == H2SensorFaultStatusFromFCU::UNHANDLED_TIMEOUT ||
             mH2SensorFaultStatusFromHMU == H2SensorFaultStatusFromHMU::FAULT ||
             mH2SensorFaultStatusFromHMU == H2SensorFaultStatusFromHMU::UNHANDLED_TIMEOUT)) {
            setSFCFCEV_WarningTelltaleTelltaleH2FaultStat(SFCFCEV_WarningTelltaleTelltaleH2FaultStat::ON);
        } else {
            setSFCFCEV_WarningTelltaleTelltaleH2FaultStat(SFCFCEV_WarningTelltaleTelltaleH2FaultStat::OFF);
        }
    }

    void updateH2LeakTelltale() {
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mH2LeakIndiReqStatusFromFCU == H2LeakIndiReqStatusFromFCU::ON ||
             mH2LeakIndiReqStatusFromFCU == H2LeakIndiReqStatusFromFCU::UNHANDLED_TIMEOUT ||
             mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::LEAK ||
             mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::UNHANDLED_TIMEOUT)) {
            setSFCFCEV_WarningTelltaleTelltaleH2LeakStat(SFCFCEV_WarningTelltaleTelltaleH2LeakStat::ON);
        } else {
            setSFCFCEV_WarningTelltaleTelltaleH2LeakStat(SFCFCEV_WarningTelltaleTelltaleH2LeakStat::OFF);
        }
    }

    void updateFcevColdStartEvent() {
        HDouble value = 0.0;
        SFCFCEV_WarningEventFcevColdStartStatusStat stat = SFCFCEV_WarningEventFcevColdStartStatusStat::OFF;
        if (mConfigVehicleType == ConfigVehicleType::FCEV && (mFcevColdStartBarStatus >= 0 && mFcevColdStartBarStatus <= 100)) {
            if (mFcevColdStartOnOffStatus == FcevColdStartOnOffStatus::ON && mIgnStatus == true) {
                stat = SFCFCEV_WarningEventFcevColdStartStatusStat::ON;
            }
            value = static_cast<HDouble>(mFcevColdStartBarStatus);
        }
        setSFCFCEV_WarningConstantFcevColdStartingValue(round((value)*100.0) / 100.0);
        setSFCFCEV_WarningEventFcevColdStartStatusStat(stat);
    }

    void updateH2SensorFaultEvent() {
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mH2SensorFaultStatusFromFCU == H2SensorFaultStatusFromFCU::FAULT ||
             mH2SensorFaultStatusFromHMU == H2SensorFaultStatusFromHMU::FAULT)) {
            setSFCFCEV_WarningEventH2SensorFaultStat(SFCFCEV_WarningEventH2SensorFaultStat::ON);
        } else {
            setSFCFCEV_WarningEventH2SensorFaultStat(SFCFCEV_WarningEventH2SensorFaultStat::OFF);
        }
    }

    void updateH2LeakWarnEvent() {
        std::string eventID;
        std::string linkedSoundID;
        SFCFCEV_WarningEventH2LeakWarnLinkedSoundType linkedSoundType = SFCFCEV_WarningEventH2LeakWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mH2LeakWarnStatusFromFCU == H2LeakWarnStatusFromFCU::LEAK_DETECT ||
                (mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::LEAK &&
                 mH2LeakWarnStatusFromHMU == H2LeakWarnStatusFromHMU::OFF)) {
                eventID = "E25202";
                linkedSoundID = "SND_PopUpWarn1";
                repeatCount = 1;
                linkedSoundType = SFCFCEV_WarningEventH2LeakWarnLinkedSoundType::REPEAT_COUNT;
            } else if (mH2LeakWarnStatusFromFCU == H2LeakWarnStatusFromFCU::LEAK_EMERGENCY ||
                       (mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::LEAK &&
                        mH2LeakWarnStatusFromHMU == H2LeakWarnStatusFromHMU::SD_REQ)) {
                eventID = "E25203";
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCFCEV_WarningEventH2LeakWarnLinkedSoundType::INFINITE;
            } else {
                // nothing
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.FCEV_Warning.Event.H2LeakWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCFCEV_WarningEventH2LeakWarnStat(SFCFCEV_WarningEventH2LeakWarnStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCFCEV_WarningEventH2LeakWarnID(eventID);
            setSFCFCEV_WarningEventH2LeakWarnStat(SFCFCEV_WarningEventH2LeakWarnStat::ON);
            setSFCFCEV_WarningEventH2LeakWarnLinkedSoundID(linkedSoundID);
            setSFCFCEV_WarningEventH2LeakWarnLinkedSoundType(linkedSoundType);
            setSFCFCEV_WarningEventH2LeakWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateFcevWaterReleaseWarnEvent() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::ONGOING) {
                eventID = "E25204";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::COMPLETE) {
                eventID = "E25205";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::NO_CONDITION) {
                eventID = "E25206";
                linkedSoundID = "SND_PopUpInform2";
            } else {
                // nothing
            }
        } else if (mIgnStatus == false && mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::ONGOING) {
                eventID = "E25217";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::COMPLETE) {
                eventID = "E25218";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::NO_CONDITION) {
                eventID = "E25219";
                linkedSoundID = "SND_PopUpInform2";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::ANTIFREEZE_ONGOING) {
                eventID = "E25220";
            } else if (mFcevWaterReleaseWarnStatus == FcevWaterReleaseWarnStatus::ANTIFREEZE_COMPLETE) {
                eventID = "E25221";
            } else {
                // nothing
            }
        } else {
            // nothing
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.FCEV_Warning.Event.FcevWaterReleaseWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCFCEV_WarningEventFcevWaterReleaseWarnStat(SFCFCEV_WarningEventFcevWaterReleaseWarnStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCFCEV_WarningEventFcevWaterReleaseWarnID(eventID);
            setSFCFCEV_WarningEventFcevWaterReleaseWarnStat(SFCFCEV_WarningEventFcevWaterReleaseWarnStat::ON);
            setSFCFCEV_WarningEventFcevWaterReleaseWarnLinkedSoundID(linkedSoundID);
        }
    }

    void updateFcevSystemWarnEvent() {
        std::string eventID;
        std::string linkedSoundID;
        SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType linkedSoundType =
            SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::NONE;
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV) {
            switch (mFcevSystemWarnStatus) {
                case FcevSystemWarnStatus::NO_START_VOLTAGE:
                    eventID = "E25207";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::REPEAT_COUNT;
                    break;
                case FcevSystemWarnStatus::NO_START_REFUEL:
                    eventID = "E25208";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::REPEAT_COUNT;
                    break;
                case FcevSystemWarnStatus::CHK_POWER_OFF:
                    eventID = "E25209";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::DURATION_MINMAX;
                    break;
                case FcevSystemWarnStatus::CHK_AUX_LV1:
                    eventID = "E25210";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::DURATION_MINMAX;
                    break;
                case FcevSystemWarnStatus::CHK_AUX_LV2:
                    eventID = "E25211";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::DURATION_MINMAX;
                    break;
                case FcevSystemWarnStatus::LIMIT_REGEN:
                    eventID = "E25213";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::REPEAT_COUNT;
                    break;
                case FcevSystemWarnStatus::RESTART:
                    eventID = "E25214";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCFCEV_WarningEventFcevSystemWarnLinkedSoundType::REPEAT_COUNT;
                    break;
                default:
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.FCEV_Warning.Event.FcevSystemWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCFCEV_WarningEventFcevSystemWarnStat(SFCFCEV_WarningEventFcevSystemWarnStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCFCEV_WarningEventFcevSystemWarnID(eventID);
            setSFCFCEV_WarningEventFcevSystemWarnStat(SFCFCEV_WarningEventFcevSystemWarnStat::ON);
            setSFCFCEV_WarningEventFcevSystemWarnLinkedSoundID(linkedSoundID);
            setSFCFCEV_WarningEventFcevSystemWarnLinkedSoundType(linkedSoundType);
        }
    }

    void updateFcevSystemOffEvent() {
        SFCFCEV_WarningEventFcevSystemOffStat stat = SFCFCEV_WarningEventFcevSystemOffStat::OFF;
        if (mIgnStatus == false && mFcevSystemOffStatus == FcevSystemOffStatus::SYSTEM_OFF) {
            stat = SFCFCEV_WarningEventFcevSystemOffStat::ON;
        }
        setSFCFCEV_WarningEventFcevSystemOffStat(stat);
    }

    void updateFcevAirFilterWarnEvent() {
        SFCFCEV_WarningEventFcevAirFilterWarnStat stat = SFCFCEV_WarningEventFcevAirFilterWarnStat::OFF;
        if (mIgnStatus == true && mConfigVehicleType == ConfigVehicleType::FCEV) {
            if (mFcevAirFilterWarnStatus == FcevAirFilterWarnStatus::CHK_FILTER) {
                stat = SFCFCEV_WarningEventFcevAirFilterWarnStat::ON;
            }
        }
        setSFCFCEV_WarningEventFcevAirFilterWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HBool mIgnStatus = false;

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using H2SensorFaultStatusFromFCU = ArgumentsH2SensorFaultStatusChanged::Input_H2SensorFaultStatusFromFCU;
    using H2SensorFaultStatusFromHMU = ArgumentsH2SensorFaultStatusChanged::Input_H2SensorFaultStatusFromHMU;
    using H2LeakIndiReqStatusFromFCU = ArgumentsH2LeakIndiReqStatusChanged::Input_H2LeakIndiReqStatusFromFCU;
    using H2LeakIndiReqStatusFromHMU = ArgumentsH2LeakIndiReqStatusChanged::Input_H2LeakIndiReqStatusFromHMU;
    using H2LeakWarnStatusFromHMU = ArgumentsH2LeakWarnStatusChanged::Input_H2LeakWarnStatusFromHMU;
    using H2LeakWarnStatusFromFCU = ArgumentsH2LeakWarnStatusChanged::Input_H2LeakWarnStatusFromFCU;
    using FcevSystemWarnStatus = ArgumentsFcevSystemWarnStatusChanged::Input_FcevSystemWarnStatus;
    using FcevWaterReleaseWarnStatus = ArgumentsFcevWaterReleaseWarnStatusChanged::Input_FcevWaterReleaseWarnStatus;
    using FcevColdStartOnOffStatus = ArgumentsFcevColdStartStatusChanged::Input_FcevColdStartOnOffStatus;
    using FcevSystemOffStatus = ArgumentsFcevSystemOffStatusChanged::Input_FcevSystemOffStatus;
    using FcevAirFilterWarnStatus = ArgumentsFcevAirFilterWarnStatusChanged::Input_FcevAirFilterWarnStatus;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    H2SensorFaultStatusFromFCU mH2SensorFaultStatusFromFCU = H2SensorFaultStatusFromFCU::OFF;
    H2SensorFaultStatusFromHMU mH2SensorFaultStatusFromHMU = H2SensorFaultStatusFromHMU::OFF;
    H2LeakIndiReqStatusFromFCU mH2LeakIndiReqStatusFromFCU = H2LeakIndiReqStatusFromFCU::OFF;
    H2LeakIndiReqStatusFromHMU mH2LeakIndiReqStatusFromHMU = H2LeakIndiReqStatusFromHMU::OFF;
    H2LeakWarnStatusFromHMU mH2LeakWarnStatusFromHMU = H2LeakWarnStatusFromHMU::OFF;
    H2LeakWarnStatusFromFCU mH2LeakWarnStatusFromFCU = H2LeakWarnStatusFromFCU::OFF;
    FcevSystemWarnStatus mFcevSystemWarnStatus = FcevSystemWarnStatus::OFF;
    FcevWaterReleaseWarnStatus mFcevWaterReleaseWarnStatus = FcevWaterReleaseWarnStatus::OFF;
    FcevColdStartOnOffStatus mFcevColdStartOnOffStatus = FcevColdStartOnOffStatus::OFF;
    FcevSystemOffStatus mFcevSystemOffStatus = FcevSystemOffStatus::NONE;
    FcevAirFilterWarnStatus mFcevAirFilterWarnStatus = FcevAirFilterWarnStatus::OFF;
    HUInt64 mFcevColdStartBarStatus = 0;
};

}  // namespace ccos

#endif  // SFSS_FCEV_Warning_H
