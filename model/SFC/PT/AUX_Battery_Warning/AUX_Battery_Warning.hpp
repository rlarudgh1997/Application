/**
 * @file AUX_Battery_Warning.hpp
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
#ifndef SFSS_AUX_Battery_Warning_H
#define SFSS_AUX_Battery_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "AUX_Battery_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 8.0.0
// Reference : [PT020] AUX_Battery_Warning. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class AUX_Battery_Warning : public AUX_Battery_WarningBase {
public:
    AUX_Battery_Warning() = default;
    ~AUX_Battery_Warning() override = default;
    AUX_Battery_Warning(const AUX_Battery_Warning& other) = delete;
    AUX_Battery_Warning(AUX_Battery_Warning&& other) noexcept = delete;
    AUX_Battery_Warning& operator=(const AUX_Battery_Warning& other) = delete;
    AUX_Battery_Warning& operator=(AUX_Battery_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAUX_Battery_WarningEventAuxBatteryResetOnReqID("E20001");
        setSFCAUX_Battery_WarningEventAuxBatteryResetOnReqLinkedSoundID("SND_PopUpInform1");
        setSFCAUX_Battery_WarningEventAuxBatteryResetOnReqLinkedSoundType(
            SFCAUX_Battery_WarningEventAuxBatteryResetOnReqLinkedSoundType::REPEAT_COUNT);
        setSFCAUX_Battery_WarningEventAuxBatteryResetOnReqLinkedSoundRepeatCount(1);
        setSFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnID("E20002");
        setSFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnLinkedSoundType(
            SFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnLinkedSoundType::REPEAT_COUNT);
        setSFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnLinkedSoundRepeatCount(1);
        setSFCAUX_Battery_WarningEventEvAuxBatteryWarnID("E20005");
        setSFCAUX_Battery_WarningEventEvAuxBatteryWarnLinkedSoundID("SND_PopUpWarn2");
        setSFCAUX_Battery_WarningEventEvAuxBatteryWarnLinkedSoundType(
            SFCAUX_Battery_WarningEventEvAuxBatteryWarnLinkedSoundType::DURATION);
        setSFCAUX_Battery_WarningEventEvAuxBatteryWarnLinkedSoundDuration(10000);
        setSFCAUX_Battery_WarningEventMainLBMRelayOnID("E20008");
        setSFCAUX_Battery_WarningEventMainLBMRelayOnLinkedSoundID("SND_PopUpInform1");
        setSFCAUX_Battery_WarningEventMainLBMRelayOnLinkedSoundType(
            SFCAUX_Battery_WarningEventMainLBMRelayOnLinkedSoundType::REPEAT_COUNT);
        setSFCAUX_Battery_WarningEventMainLBMRelayOnLinkedSoundRepeatCount(1);
        if (mOneShotTimerISGActiveStatusTime.create(kTimerISGActiveStatusTimeInterval3s, this,
                                                    &AUX_Battery_Warning::onOneShotTimerISGActiveStatusTime, false) == false) {
            DWarning() << "[AUX_Battery_Warning] mOneShotTimerISGActiveStatusTime Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateEventAuxBatteryResetOnReq();
        updateEventAuxBatteryRelayOnOffWarn();
        updateEventAuxBatteryWarnStatus();
        updateEventEvAuxBatteryWarn();
        updateEventMainLithumBatteryWarn();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateEventAuxBatteryResetOnReq();
        updateEventAuxBatteryRelayOnOffWarn();
        updateEventAuxBatteryWarnStatus();
        updateEventEvAuxBatteryWarn();
        updateEventMainLithumBatteryWarn();
    }

    void onIsgActiveStatusChanged(const ArgumentsIsgActiveStatusChanged& args) {
        mInter_ISGActiveStatus = args.mInter_ISGActiveStatus;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        mInter_Config48V = args.mInter_Config48V;
        mInter_ConfigISG = args.mInter_ConfigISG;
        updateTelltale();
    }

    void onAuxBatteryWarnChanged(const ArgumentsAuxBatteryWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_AuxBatteryWarnReqFromEMS) == true) {
            mInput_AuxBatteryWarnReqFromEMS = Input_AuxBatteryWarnReqFromEMS::UNHANDLED_TIMEOUT;
        } else {
            mInput_AuxBatteryWarnReqFromEMS = args.mInput_AuxBatteryWarnReqFromEMS;
        }
        if (ISTIMEOUT(args.mInput_AuxBatteryWarnReqFromLDC) == true) {
            mInput_AuxBatteryWarnReqFromLDC = Input_AuxBatteryWarnReqFromLDC::UNHANDLED_TIMEOUT;
        } else {
            mInput_AuxBatteryWarnReqFromLDC = args.mInput_AuxBatteryWarnReqFromLDC;
        }
        if (ISTIMEOUT(args.mInput_AuxBatteryWarnReqFromLDC48V) == true) {
            mInput_AuxBatteryWarnReqFromLDC48V = Input_AuxBatteryWarnReqFromLDC48V::UNHANDLED_TIMEOUT;
        } else {
            mInput_AuxBatteryWarnReqFromLDC48V = args.mInput_AuxBatteryWarnReqFromLDC48V;
        }
        mInput_AlternatorInput = args.mInput_AlternatorInput;
        updateTelltale();
    }

    void onAuxBatteryResetOnReqChanged(const ArgumentsAuxBatteryResetOnReqChanged& args) {
        mInput_AuxBatteryResetOnReqStatus = args.mInput_AuxBatteryResetOnReqStatus;
        updateEventAuxBatteryResetOnReq();
    }

    void onAuxBatteryRelayOnOffWarnChanged(const ArgumentsAuxBatteryRelayOnOffWarnChanged& args) {
        mInput_AuxBatteryRelayOffWarnStatus = args.mInput_AuxBatteryRelayOffWarnStatus;
        mInput_AuxBatteryRelayOnWarnStatus = args.mInput_AuxBatteryRelayOnWarnStatus;
        updateEventAuxBatteryRelayOnOffWarn();
    }

    void onAuxBatteryWarnStatusChanged(const ArgumentsAuxBatteryWarnStatusChanged& args) {
        mInput_AuxBatteryWarnStatus = args.mInput_AuxBatteryWarnStatus;
        updateEventAuxBatteryWarnStatus();
    }

    void onEvAuxBatteryWarnChanged(const ArgumentsEvAuxBatteryWarnChanged& args) {
        mInput_EvAuxBatteryWarnStatus = args.mInput_EvAuxBatteryWarnStatus;
        updateEventEvAuxBatteryWarn();
    }

    void onConfigALDCAuxBatteryWarnReqFromALDCChanged(const ArgumentsConfigALDCAuxBatteryWarnReqFromALDCChanged& args) {
        if (ISTIMEOUT(args.mInput_AuxBatteryWarnReqFromALDC) == true) {
            mInput_AuxBatteryWarnReqFromALDC = Input_AuxBatteryWarnReqFromALDC::UNHANDLED_TIMEOUT;
        } else {
            mInput_AuxBatteryWarnReqFromALDC = args.mInput_AuxBatteryWarnReqFromALDC;
        }
        mInter_ConfigALDC = args.mInter_ConfigALDC;
        updateTelltale();
    }

    void onEvAuxBatteryWarnStatusFromALDCChanged(const ArgumentsEvAuxBatteryWarnStatusFromALDCChanged& args) {
        mInput_EvAuxBatteryWarnStatusFromALDC = args.mInput_EvAuxBatteryWarnStatusFromALDC;
        updateEventEvAuxBatteryWarn();
    }

    void onMainLithumBatteryWarnStatusChanged(const ArgumentsMainLithumBatteryWarnStatusChanged& args) {
        mInput_MainLithumBatteryWarnStatus = args.mInput_MainLithumBatteryWarnStatus;
        updateEventMainLithumBatteryWarn();
    }

    void onMainLBMRelayOnStatusChanged(const ArgumentsMainLBMRelayOnStatusChanged& args) {
        mInput_MainLBMRelayOnStatus = args.mInput_MainLBMRelayOnStatus;
        updateEventMainLBMRelayOn();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat(SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::OFF);
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mInter_ConfigISG == Inter_ConfigISG::ON) {
                if (mInter_ISGActiveStatus == Inter_ISGActiveStatus::ON) {
                    mOneShotTimerISGActiveStatusTime.stop();
                    setSFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat(
                        SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::OFF);
                } else if (mInter_ISGActiveStatus == Inter_ISGActiveStatus::OFF &&
                           mPrev_Inter_ISGActiveStatus == Inter_ISGActiveStatus::ON) {
                    if (mOneShotTimerISGActiveStatusTime.start() == false) {
                        DWarning() << "[AUX_Battery_Warning] mOneShotTimerISGActiveStatusTime Start Failed";
                    }
                } else {
                    updateTelltaleFromCondition();
                }
                mPrev_Inter_ISGActiveStatus = mInter_ISGActiveStatus;
            } else {
                updateTelltaleFromCondition();
            }
        } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
            mOneShotTimerISGActiveStatusTime.stop();
            setSFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat(SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::OFF);
        } else {
            // nothing
        }
    }

    void updateTelltaleFromCondition(HBool enterFromTimerCallback = false) {
        if (mOneShotTimerISGActiveStatusTime.isRunning() == false || enterFromTimerCallback == true) {
            SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat telltaleStat =
                SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::OFF;
            if (mInput_AlternatorInput == Input_AlternatorInput::ON) {
                telltaleStat = SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::ON;
            } else if (mInter_Config48V == Inter_Config48V::ON &&
                       (mInput_AuxBatteryWarnReqFromLDC48V == Input_AuxBatteryWarnReqFromLDC48V::ON ||
                        mInput_AuxBatteryWarnReqFromLDC48V == Input_AuxBatteryWarnReqFromLDC48V::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::ON;
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV &&
                       (mInput_AuxBatteryWarnReqFromEMS == Input_AuxBatteryWarnReqFromEMS::ON ||
                        mInput_AuxBatteryWarnReqFromEMS == Input_AuxBatteryWarnReqFromEMS::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::ON;
            } else if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                        mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) &&
                       (mInput_AuxBatteryWarnReqFromLDC == Input_AuxBatteryWarnReqFromLDC::ON ||
                        mInput_AuxBatteryWarnReqFromLDC == Input_AuxBatteryWarnReqFromLDC::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::ON;
            } else if (mInter_ConfigALDC == Inter_ConfigALDC::ON &&
                       (mInput_AuxBatteryWarnReqFromALDC == Input_AuxBatteryWarnReqFromALDC::ON ||
                        mInput_AuxBatteryWarnReqFromALDC == Input_AuxBatteryWarnReqFromALDC::UNHANDLED_TIMEOUT)) {
                telltaleStat = SFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat::ON;
            } else {
                // no operation
            }
            setSFCAUX_Battery_WarningTelltaleAUX_Battery_WarningStat(telltaleStat);
        }
    }

    void updateEventAuxBatteryResetOnReq() {
        SFCAUX_Battery_WarningEventAuxBatteryResetOnReqStat eventStat = SFCAUX_Battery_WarningEventAuxBatteryResetOnReqStat::OFF;
        if (mIgnElapsed != IgnElapsed::OFF_0ms && mInput_AuxBatteryResetOnReqStatus == Input_AuxBatteryResetOnReqStatus::ON_REQ) {
            eventStat = SFCAUX_Battery_WarningEventAuxBatteryResetOnReqStat::ON;
        }
        setSFCAUX_Battery_WarningEventAuxBatteryResetOnReqStat(eventStat);
    }

    void updateEventAuxBatteryRelayOnOffWarn() {
        SFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnStat eventStat =
            SFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnStat::OFF;
        if (mIgnElapsed != IgnElapsed::OFF_0ms &&
            (mInput_AuxBatteryRelayOffWarnStatus == Input_AuxBatteryRelayOffWarnStatus::WARN_ON ||
             mInput_AuxBatteryRelayOnWarnStatus == Input_AuxBatteryRelayOnWarnStatus::WARN_ON)) {
            eventStat = SFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnStat::ON;
        }
        setSFCAUX_Battery_WarningEventAuxBatteryRelayOnOffWarnStat(eventStat);
    }

    void updateEventAuxBatteryWarnStatus() {
        std::string eventID;
        std::string soundID;
        SFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundType soundType =
            SFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundType::NONE;
        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mInput_AuxBatteryWarnStatus == Input_AuxBatteryWarnStatus::WARN1) {
                eventID = "E20003";
                soundID = "SND_PopUpWarn1";
                soundType = SFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundType::REPEAT_COUNT;
            } else if (mInput_AuxBatteryWarnStatus == Input_AuxBatteryWarnStatus::WARN2) {
                eventID = "E20004";
                soundID = "SND_PopUpWarn2";
                soundType = SFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundType::DURATION;
            } else {
                // NONE
            }
            std::string prevEventID;
            GETCACHEDVALUE(SFC.AUX_Battery_Warning.Event.AuxBatteryWarnStatus.ID, prevEventID);
            if (!prevEventID.empty() && prevEventID != eventID) {
                DDebug() << "SFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat::OFF ";
                setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat(SFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat::OFF);
                flushLastGroup();
            }
            if (!eventID.empty()) {
                setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusID(eventID);
                setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat(SFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat::ON);
                setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundID(soundID);
                setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundType(soundType);
                if (eventID == "E20003") {
                    setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundRepeatCount(1);
                } else if (eventID == "E20004") {
                    setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusLinkedSoundDuration(10000);
                } else {
                    // NONE
                }
            }
        } else {
            setSFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat(SFCAUX_Battery_WarningEventAuxBatteryWarnStatusStat::OFF);
        }
    }

    void updateEventEvAuxBatteryWarn() {
        SFCAUX_Battery_WarningEventEvAuxBatteryWarnStat eventStat = SFCAUX_Battery_WarningEventEvAuxBatteryWarnStat::OFF;
        if (mIgnElapsed != IgnElapsed::OFF_0ms &&
            (mInput_EvAuxBatteryWarnStatus == Input_EvAuxBatteryWarnStatus::ON ||
             mInput_EvAuxBatteryWarnStatusFromALDC == Input_EvAuxBatteryWarnStatusFromALDC::ON)) {
            eventStat = SFCAUX_Battery_WarningEventEvAuxBatteryWarnStat::ON;
        }
        setSFCAUX_Battery_WarningEventEvAuxBatteryWarnStat(eventStat);
    }

    void updateEventMainLithumBatteryWarn() {
        std::string eventID;
        std::string soundID;
        SFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundType soundType =
            SFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundType::NONE;
        if (mIgnElapsed != IgnElapsed::OFF_0ms) {
            if (mInput_MainLithumBatteryWarnStatus == Input_MainLithumBatteryWarnStatus::WARN1) {
                eventID = "E20006";
                soundID = "SND_PopUpWarn1";
                soundType = SFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundType::REPEAT_COUNT;
            } else if (mInput_MainLithumBatteryWarnStatus == Input_MainLithumBatteryWarnStatus::WARN2) {
                eventID = "E20007";
                soundID = "SND_PopUpWarn2";
                soundType = SFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundType::DURATION;
            } else {
                // NONE
            }
            std::string prevEventID;
            GETCACHEDVALUE(SFC.AUX_Battery_Warning.Event.MainLithumBatteryWarn.ID, prevEventID);
            if (!prevEventID.empty() && prevEventID != eventID) {
                DDebug() << "SFCAUX_Battery_WarningEventMainLithumBatteryWarnStat::OFF ";
                setSFCAUX_Battery_WarningEventMainLithumBatteryWarnStat(
                    SFCAUX_Battery_WarningEventMainLithumBatteryWarnStat::OFF);
                flushLastGroup();
            }
            if (!eventID.empty()) {
                setSFCAUX_Battery_WarningEventMainLithumBatteryWarnID(eventID);
                setSFCAUX_Battery_WarningEventMainLithumBatteryWarnStat(SFCAUX_Battery_WarningEventMainLithumBatteryWarnStat::ON);
                setSFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundID(soundID);
                setSFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundType(soundType);
                if (eventID == "E20006") {
                    setSFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundRepeatCount(1);
                } else if (eventID == "E20007") {
                    setSFCAUX_Battery_WarningEventMainLithumBatteryWarnLinkedSoundDuration(10000);
                } else {
                    // NONE
                }
            }
        } else {
            setSFCAUX_Battery_WarningEventMainLithumBatteryWarnStat(SFCAUX_Battery_WarningEventMainLithumBatteryWarnStat::OFF);
        }
    }

    void updateEventMainLBMRelayOn() {
        SFCAUX_Battery_WarningEventMainLBMRelayOnStat eventStat = SFCAUX_Battery_WarningEventMainLBMRelayOnStat::OFF;
        if (mInput_MainLBMRelayOnStatus == Input_MainLBMRelayOnStatus::PRESS_BTN) {
            eventStat = SFCAUX_Battery_WarningEventMainLBMRelayOnStat::ON;
        }
        setSFCAUX_Battery_WarningEventMainLBMRelayOnStat(eventStat);
    }

    void onOneShotTimerISGActiveStatusTime() {
        updateTelltaleFromCondition(true);
        flush();
    }

    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using Inter_ISGActiveStatus = ArgumentsIsgActiveStatusChanged::Inter_ISGActiveStatus;
    using Inter_Config48V = ArgumentsConfigChanged::Inter_Config48V;
    using Inter_ConfigISG = ArgumentsConfigChanged::Inter_ConfigISG;
    using Input_AuxBatteryWarnReqFromEMS = ArgumentsAuxBatteryWarnChanged::Input_AuxBatteryWarnReqFromEMS;
    using Input_AuxBatteryWarnReqFromLDC = ArgumentsAuxBatteryWarnChanged::Input_AuxBatteryWarnReqFromLDC;
    using Input_AuxBatteryWarnReqFromLDC48V = ArgumentsAuxBatteryWarnChanged::Input_AuxBatteryWarnReqFromLDC48V;
    using Input_AlternatorInput = ArgumentsAuxBatteryWarnChanged::Input_AlternatorInput;
    using Input_AuxBatteryResetOnReqStatus = ArgumentsAuxBatteryResetOnReqChanged::Input_AuxBatteryResetOnReqStatus;
    using Input_AuxBatteryRelayOffWarnStatus = ArgumentsAuxBatteryRelayOnOffWarnChanged::Input_AuxBatteryRelayOffWarnStatus;
    using Input_AuxBatteryRelayOnWarnStatus = ArgumentsAuxBatteryRelayOnOffWarnChanged::Input_AuxBatteryRelayOnWarnStatus;
    using Input_AuxBatteryWarnStatus = ArgumentsAuxBatteryWarnStatusChanged::Input_AuxBatteryWarnStatus;
    using Input_EvAuxBatteryWarnStatus = ArgumentsEvAuxBatteryWarnChanged::Input_EvAuxBatteryWarnStatus;
    using Input_AuxBatteryWarnReqFromALDC = ArgumentsConfigALDCAuxBatteryWarnReqFromALDCChanged::Input_AuxBatteryWarnReqFromALDC;
    using Inter_ConfigALDC = ArgumentsConfigALDCAuxBatteryWarnReqFromALDCChanged::Inter_ConfigALDC;
    using Input_EvAuxBatteryWarnStatusFromALDC =
        ArgumentsEvAuxBatteryWarnStatusFromALDCChanged::Input_EvAuxBatteryWarnStatusFromALDC;
    using Input_MainLithumBatteryWarnStatus = ArgumentsMainLithumBatteryWarnStatusChanged::Input_MainLithumBatteryWarnStatus;
    using Input_MainLBMRelayOnStatus = ArgumentsMainLBMRelayOnStatusChanged::Input_MainLBMRelayOnStatus;

    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    Inter_ISGActiveStatus mInter_ISGActiveStatus = Inter_ISGActiveStatus::OFF;
    Inter_ISGActiveStatus mPrev_Inter_ISGActiveStatus = Inter_ISGActiveStatus::OFF;
    Inter_Config48V mInter_Config48V = Inter_Config48V::OFF;
    Inter_ConfigISG mInter_ConfigISG = Inter_ConfigISG::OFF;
    Input_AuxBatteryWarnReqFromEMS mInput_AuxBatteryWarnReqFromEMS = Input_AuxBatteryWarnReqFromEMS::OFF;
    Input_AuxBatteryWarnReqFromLDC mInput_AuxBatteryWarnReqFromLDC = Input_AuxBatteryWarnReqFromLDC::OFF;
    Input_AuxBatteryWarnReqFromLDC48V mInput_AuxBatteryWarnReqFromLDC48V = Input_AuxBatteryWarnReqFromLDC48V::OFF;
    Input_AlternatorInput mInput_AlternatorInput = Input_AlternatorInput::OFF;
    Input_AuxBatteryResetOnReqStatus mInput_AuxBatteryResetOnReqStatus = Input_AuxBatteryResetOnReqStatus::DEFAULT;
    Input_AuxBatteryRelayOffWarnStatus mInput_AuxBatteryRelayOffWarnStatus = Input_AuxBatteryRelayOffWarnStatus::WARN_OFF;
    Input_AuxBatteryRelayOnWarnStatus mInput_AuxBatteryRelayOnWarnStatus = Input_AuxBatteryRelayOnWarnStatus::WARN_OFF;
    Input_AuxBatteryWarnStatus mInput_AuxBatteryWarnStatus = Input_AuxBatteryWarnStatus::OFF;
    Input_EvAuxBatteryWarnStatus mInput_EvAuxBatteryWarnStatus = Input_EvAuxBatteryWarnStatus::OFF;
    Input_AuxBatteryWarnReqFromALDC mInput_AuxBatteryWarnReqFromALDC = Input_AuxBatteryWarnReqFromALDC::OFF;
    Inter_ConfigALDC mInter_ConfigALDC = Inter_ConfigALDC::OFF;
    Input_EvAuxBatteryWarnStatusFromALDC mInput_EvAuxBatteryWarnStatusFromALDC = Input_EvAuxBatteryWarnStatusFromALDC::OFF;
    Input_MainLithumBatteryWarnStatus mInput_MainLithumBatteryWarnStatus = Input_MainLithumBatteryWarnStatus::OFF;
    Input_MainLBMRelayOnStatus mInput_MainLBMRelayOnStatus = Input_MainLBMRelayOnStatus::OFF;

    enum class IgnElapsed : HUInt64 {
        ON_0ms,
        ON_500ms,
        OFF_0ms
    };

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    static constexpr uint32_t kTimerISGActiveStatusTimeInterval3s = 3000u;
    ssfs::SFCTimer<AUX_Battery_Warning> mOneShotTimerISGActiveStatusTime;
};

}  // namespace ccos

#endif  // SFSS_AUX_Battery_Warning_H
