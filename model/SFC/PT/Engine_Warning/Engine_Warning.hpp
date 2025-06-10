/**
 * @file Engine_Warning.hpp
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
#ifndef SFSS_Engine_Warning_H
#define SFSS_Engine_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_WarningBase.hpp"

namespace ccos {

// SFC Version : 7.0.0
// Reference : [PT150] Engine_Warning. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class Engine_Warning : public Engine_WarningBase {
public:
    Engine_Warning() = default;
    ~Engine_Warning() override = default;
    Engine_Warning(const Engine_Warning& other) = delete;
    Engine_Warning(Engine_Warning&& other) noexcept = delete;
    Engine_Warning& operator=(const Engine_Warning& other) = delete;
    Engine_Warning& operator=(Engine_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngine_WarningEventEmsSafetyFunctionStatusID("E21201");
        setSFCEngine_WarningEventEmsSafetyFunctionStatusLinkedSoundID("SND_PopUpWarn2");
        setSFCEngine_WarningEventEmsSafetyFunctionStatusLinkedSoundType(
            SFCEngine_WarningEventEmsSafetyFunctionStatusLinkedSoundType::INFINITE);

        setSFCEngine_WarningEventEngineOffWarnReqStatusLinkedSoundID("SND_PopUpWarn2");
        setSFCEngine_WarningEventEngineOffWarnReqStatusLinkedSoundType(
            SFCEngine_WarningEventEngineOffWarnReqStatusLinkedSoundType::INFINITE);

        setSFCEngine_WarningSoundEngine_WarningID("SND_EngineKSDSWarn");
        setSFCEngine_WarningSoundEngine_WarningType(SFCEngine_WarningSoundEngine_WarningType::REPEAT_MINMAX);
        setSFCEngine_WarningSoundEngine_WarningRepeatMin(0);
        setSFCEngine_WarningSoundEngine_WarningRepeatMax(15);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltale();
        updateEventEmsSafetyFunctionStatus();
        updateEventEngineOffWarnReq();
        updateEventEu7WarnReqStatus();
        updateSound();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        // Pre Warning
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
        updateSound();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltale();
        updateEventEmsSafetyFunctionStatus();
        updateEventEngineOffWarnReq();
        updateEventEu7WarnReqStatus();
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfig48V = args.mInter_Config48V;
        updateTelltale();
        updateEventEmsSafetyFunctionStatus();
        updateEventEngineOffWarnReq();
        updateEventEu7WarnReqStatus();
        updateSound();
    }

    void onEngineWarnReqChanged(const ArgumentsEngineWarnReqChanged& args) {
        if (ISTIMEOUT(args.mInput_EngineWarnReqFromEMS) == true) {
            mEngineWarnReqFromEMS = EngineWarnReqFromEMS::UNHANDLED_TIMEOUT;
        } else {
            mEngineWarnReqFromEMS = args.mInput_EngineWarnReqFromEMS;
        }
        mEngineWarnReqFromBMS = args.mInput_EngineWarnReqFromBMS;
        if (ISTIMEOUT(args.mInput_EngineWarnReqFromOBC) == true) {
            mEngineWarnReqFromOBC = EngineWarnReqFromOBC::UNHANDLED_TIMEOUT;
        } else {
            mEngineWarnReqFromOBC = args.mInput_EngineWarnReqFromOBC;
        }
        updateTelltale();
    }

    void onEngineEMSStatChanged(const ArgumentsEngineEMSStatChanged& args) {
        mEmsSafetyFunctionStatus = args.mInput_EmsSafetyFunctionStatus;
        updateEventEmsSafetyFunctionStatus();
    }

    void onEngineOffWarnReqChanged(const ArgumentsEngineOffWarnReqChanged& args) {
        mEngineOffWarnReq = args.mInput_EngineOffWarnReqStatus;
        updateEventEngineOffWarnReq();
    }

    void onEngineKSDSChanged(const ArgumentsEngineKSDSChanged& args) {
        mEngineKNKWarnStatus = args.mInput_EngineKNKWarnStatus;
        mEngineOnStatus = args.mInter_EngineOnStatus;
        updateSound();
    }

    void onEu7WarnReqStatusChanged(const ArgumentsEu7WarnReqStatusChanged& args) {
        mEu7WarnReqStatus = args.mInput_EU7WarnReqStatus;
        updateEventEu7WarnReqStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (isEngineWarningVehicleTypeOn() == false) {
            return;
        }

        SFCEngine_WarningTelltaleEngine_WarningStat telltaleStat = SFCEngine_WarningTelltaleEngine_WarningStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            telltaleStat = SFCEngine_WarningTelltaleEngine_WarningStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if ((mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
                 mConfigVehicleType == ConfigVehicleType::PHEV || mConfigVehicleType == ConfigVehicleType::EREV) &&
                (mEngineWarnReqFromEMS == EngineWarnReqFromEMS::ON ||
                 mEngineWarnReqFromEMS == EngineWarnReqFromEMS::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCEngine_WarningTelltaleEngine_WarningStat::ON;
            } else if (mConfig48V == Config48V::ON && mEngineWarnReqFromBMS == EngineWarnReqFromBMS::ON) {
                telltaleStat = SFCEngine_WarningTelltaleEngine_WarningStat::ON;
            } else if (mConfigVehicleType == ConfigVehicleType::PHEV &&
                       (mEngineWarnReqFromOBC == EngineWarnReqFromOBC::ON ||
                        mEngineWarnReqFromOBC == EngineWarnReqFromOBC::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCEngine_WarningTelltaleEngine_WarningStat::ON;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCEngine_WarningTelltaleEngine_WarningStat(telltaleStat);
    }

    void updateEventEmsSafetyFunctionStatus() {
        if (isEngineWarningVehicleTypeOn() == false) {
            return;
        }

        SFCEngine_WarningEventEmsSafetyFunctionStatusStat stat = SFCEngine_WarningEventEmsSafetyFunctionStatusStat::OFF;
        if (mIsIgnOn == true && mEmsSafetyFunctionStatus == EmsSafetyFunctionStatus::ON) {
            stat = SFCEngine_WarningEventEmsSafetyFunctionStatusStat::ON;
        }
        setSFCEngine_WarningEventEmsSafetyFunctionStatusStat(stat);
    }

    void updateEventEngineOffWarnReq() {
        if (isEngineWarningVehicleTypeOn() == false) {
            return;
        }

        std::string prevEventID;
        std::string eventID;
        GETCACHEDVALUE(SFC.Engine_Warning.Event.EngineOffWarnReqStatus.ID, prevEventID);
        if (mIsIgnOn == true) {
            if (mEngineOffWarnReq == EngineOffWarnReq::WARN1) {
                eventID = "E21202";
            } else if (mEngineOffWarnReq == EngineOffWarnReq::WARN2) {
                eventID = "E21203";
            } else {
                // no operation
            }
        }

        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEngine_WarningEventEngineOffWarnReqStatusStat(SFCEngine_WarningEventEngineOffWarnReqStatusStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCEngine_WarningEventEngineOffWarnReqStatusID(eventID);
            setSFCEngine_WarningEventEngineOffWarnReqStatusStat(SFCEngine_WarningEventEngineOffWarnReqStatusStat::ON);
        }
    }

    void updateEventEu7WarnReqStatus() {
        if (isEngineWarningVehicleTypeOn() == false) {
            return;
        }

        std::string eventID;
        std::string linkedSoundID;
        SFCEngine_WarningEventEU7WarnReqStatusLinkedSoundType linkedSoundType =
            SFCEngine_WarningEventEU7WarnReqStatusLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == false) {
            if (mEu7WarnReqStatus == Eu7WarnReqStatus::HEATING_ONGOING) {
                eventID = "E21204";
            } else if (mEu7WarnReqStatus == Eu7WarnReqStatus::HEATING_COMPLETE) {
                eventID = "E21205";
            } else {
                // no operation
            }
        } else {
            if (mEu7WarnReqStatus == Eu7WarnReqStatus::WARN_LOWTEMP) {
                eventID = "E21206";
            } else if (mEu7WarnReqStatus == Eu7WarnReqStatus::WARN_HIGHTEMP) {
                eventID = "E21207";
                linkedSoundID = "SND_PopUpInform1";
                linkedSoundType = SFCEngine_WarningEventEU7WarnReqStatusLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Engine_Warning.Event.EU7WarnReqStatus.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEngine_WarningEventEU7WarnReqStatusStat(SFCEngine_WarningEventEU7WarnReqStatusStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCEngine_WarningEventEU7WarnReqStatusID(eventID);
            setSFCEngine_WarningEventEU7WarnReqStatusStat(SFCEngine_WarningEventEU7WarnReqStatusStat::ON);
            setSFCEngine_WarningEventEU7WarnReqStatusLinkedSoundID(linkedSoundID);
            setSFCEngine_WarningEventEU7WarnReqStatusLinkedSoundType(linkedSoundType);
            setSFCEngine_WarningEventEU7WarnReqStatusLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateSound() {
        if (isEngineWarningVehicleTypeOn() == false) {
            return;
        }

        SFCEngine_WarningSoundEngine_WarningStat stat = SFCEngine_WarningSoundEngine_WarningStat::OFF;

        // Pre-Warning Sound OFF (IMG_TelltaleChkEng_stat, in Pre-warning, IGN1 OFF > ON 3 sec after 500ms)
        if (mIgnElapsed == IgnElapsed::ON_3500ms && mEngineOnStatus == EngineOnStatus::ON &&
            mEngineKNKWarnStatus == EngineKNKWarnStatus::WARN) {
            stat = SFCEngine_WarningSoundEngine_WarningStat::ON;
        }
        setSFCEngine_WarningSoundEngine_WarningStat(stat);
    }

    inline HBool isEngineWarningVehicleTypeOn() {
        return mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::HEV ||
               mConfigVehicleType == ConfigVehicleType::PHEV || mConfigVehicleType == ConfigVehicleType::EREV;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using Config48V = ArgumentsConfigChanged::Inter_Config48V;
    using EngineWarnReqFromEMS = ArgumentsEngineWarnReqChanged::Input_EngineWarnReqFromEMS;
    using EngineWarnReqFromBMS = ArgumentsEngineWarnReqChanged::Input_EngineWarnReqFromBMS;
    using EngineWarnReqFromOBC = ArgumentsEngineWarnReqChanged::Input_EngineWarnReqFromOBC;
    using EmsSafetyFunctionStatus = ArgumentsEngineEMSStatChanged::Input_EmsSafetyFunctionStatus;
    using EngineOffWarnReq = ArgumentsEngineOffWarnReqChanged::Input_EngineOffWarnReqStatus;
    using EngineKNKWarnStatus = ArgumentsEngineKSDSChanged::Input_EngineKNKWarnStatus;
    using EngineOnStatus = ArgumentsEngineKSDSChanged::Inter_EngineOnStatus;
    using Eu7WarnReqStatus = ArgumentsEu7WarnReqStatusChanged::Input_EU7WarnReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    Config48V mConfig48V = Config48V::OFF;
    EngineWarnReqFromEMS mEngineWarnReqFromEMS = EngineWarnReqFromEMS::OFF;
    EngineWarnReqFromOBC mEngineWarnReqFromOBC = EngineWarnReqFromOBC::OFF;
    EngineWarnReqFromBMS mEngineWarnReqFromBMS = EngineWarnReqFromBMS::OFF;
    EmsSafetyFunctionStatus mEmsSafetyFunctionStatus = EmsSafetyFunctionStatus::OFF;
    EngineOffWarnReq mEngineOffWarnReq = EngineOffWarnReq::OFF;
    EngineKNKWarnStatus mEngineKNKWarnStatus = EngineKNKWarnStatus::OFF;
    EngineOnStatus mEngineOnStatus = EngineOnStatus::OFF;
    Eu7WarnReqStatus mEu7WarnReqStatus = Eu7WarnReqStatus::OFF;

    HBool mIsIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_Engine_Warning_H
