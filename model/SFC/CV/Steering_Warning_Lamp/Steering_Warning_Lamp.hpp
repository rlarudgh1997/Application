/**
 * @file Steering_Warning_Lamp.hpp
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
#ifndef SFSS_Steering_Warning_Lamp_H
#define SFSS_Steering_Warning_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "Steering_Warning_LampBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.36
// Reference : [CV350] Steering_Warning_Lamp. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Steering_Warning_Lamp : public Steering_Warning_LampBase {
public:
    Steering_Warning_Lamp() = default;
    ~Steering_Warning_Lamp() override = default;
    Steering_Warning_Lamp(const Steering_Warning_Lamp& other) = delete;
    Steering_Warning_Lamp(Steering_Warning_Lamp&& other) noexcept = delete;
    Steering_Warning_Lamp& operator=(const Steering_Warning_Lamp& other) = delete;
    Steering_Warning_Lamp& operator=(Steering_Warning_Lamp&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSteering_Warning_LampEventAmberID("E73501");
        setSFCSteering_Warning_LampEventAmberLinkedSoundID("SND_PopUpWarn1");
        setSFCSteering_Warning_LampEventAmberLinkedSoundRepeatCount(1);
        setSFCSteering_Warning_LampEventAmberLinkedSoundType(SFCSteering_Warning_LampEventAmberLinkedSoundType::REPEAT_COUNT);
        setSFCSteering_Warning_LampEventRedID("E73502");
        setSFCSteering_Warning_LampEventRedLinkedSoundID("SND_PopUpWarn2");
        setSFCSteering_Warning_LampEventRedLinkedSoundRepeatCount(1);
        setSFCSteering_Warning_LampEventRedLinkedSoundType(SFCSteering_Warning_LampEventRedLinkedSoundType::REPEAT_COUNT);

        setSFCSteering_Warning_LampEventSteeringModeChangeStatusLinkedSoundID("SND_PopUpInform1");
        setSFCSteering_Warning_LampEventSteeringModeChangeStatusLinkedSoundRepeatCount(1);
        setSFCSteering_Warning_LampEventSteeringModeChangeStatusLinkedSoundType(
            SFCSteering_Warning_LampEventSteeringModeChangeStatusLinkedSoundType::REPEAT_COUNT);

        setSFCSteering_Warning_LampEventPSTROilReserverSwitchStatusID("E73505");
        setSFCSteering_Warning_LampEventPSTROilReserverSwitchStatusLinkedSoundID("SND_PopUpWarn2");
        setSFCSteering_Warning_LampEventPSTROilReserverSwitchStatusLinkedSoundRepeatCount(1);
        setSFCSteering_Warning_LampEventPSTROilReserverSwitchStatusLinkedSoundType(
            SFCSteering_Warning_LampEventPSTROilReserverSwitchStatusLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updatePrivateTelltaleEHPS1();
        updatePrivateTelltaleEHPS2();
        updatePrivateTelltaleMAHS();
        updatePrivateTelltaleRAS();
        updateConstantEHPSRedEventStatus();
        updateConstantMAHSRedEventStatus();
        updateConstantRASRedEventStatus();
        updateEventRed();
        updateConstantEHPSAmberEventStatus();
        updateConstantMAHSAmberEventStatus();
        updateConstantRASAmberEventStatus();
        updateEventAmber();
        updateEventSteeringModeChange();
        updateEventPSTROilReserver();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updatePrivateTelltaleEHPS1();
        updatePrivateTelltaleEHPS2();
        updatePrivateTelltaleMAHS();
        updatePrivateTelltaleRAS();
        updateConstantEHPSRedEventStatus();
        updateConstantMAHSRedEventStatus();
        updateConstantRASRedEventStatus();
        updateEventRed();
        updateConstantEHPSAmberEventStatus();
        updateConstantMAHSAmberEventStatus();
        updateConstantRASAmberEventStatus();
        updateEventAmber();
        updateEventSteeringModeChange();
        updateEventPSTROilReserver();
    }

    void onConfigMAHSChanged(const ArgumentsConfigMAHSChanged& args) {
        mConfigMAHS = args.mInter_ConfigMAHS;
        updatePrivateTelltaleMAHS();
        updateConstantMAHSRedEventStatus();
        updateConstantMAHSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onConfigEHPS1Changed(const ArgumentsConfigEHPS1Changed& args) {
        mConfigEHPS1 = args.mInter_ConfigEHPS1;
        updatePrivateTelltaleEHPS1();
        updateConstantEHPSRedEventStatus();
        updateConstantEHPSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onConfigEHPS2Changed(const ArgumentsConfigEHPS2Changed& args) {
        mConfigEHPS2 = args.mInter_ConfigEHPS2;
        updatePrivateTelltaleEHPS2();
        updateConstantEHPSRedEventStatus();
        updateConstantEHPSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onConfigRASChanged(const ArgumentsConfigRASChanged& args) {
        mConfigRAS = args.mInter_ConfigRAS;
        updatePrivateTelltaleRAS();
        updateConstantRASRedEventStatus();
        updateConstantRASAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onEhps1WarningStatusChanged(const ArgumentsEhps1WarningStatusChanged& args) {
        mWarning1StatusEHPS1 = args.mInput_SteeringWarning1Status_EHPS1;
        mWarning2StatusEHPS1 = args.mInput_SteeringWarning2Status_EHPS1;
        updatePrivateTelltaleEHPS1();
        updateConstantEHPSRedEventStatus();
        updateConstantEHPSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onEhps2WarningStatusChanged(const ArgumentsEhps2WarningStatusChanged& args) {
        mWarning1StatusEHPS2 = args.mInput_SteeringWarning1Status_EHPS2;
        mWarning2StatusEHPS2 = args.mInput_SteeringWarning2Status_EHPS2;
        updatePrivateTelltaleEHPS2();
        updateConstantEHPSRedEventStatus();
        updateConstantEHPSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onMahsWarningStatusChanged(const ArgumentsMahsWarningStatusChanged& args) {
        mWarning1StatusMAHS = args.mInput_SteeringWarning1Status_MAHS;
        mWarning2StatusMAHS = args.mInput_SteeringWarning2Status_MAHS;
        updatePrivateTelltaleMAHS();
        updateConstantMAHSRedEventStatus();
        updateConstantMAHSAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onRasWarningStatusChanged(const ArgumentsRasWarningStatusChanged& args) {
        mWarning1StatusRAS = args.mInput_SteeringWarning1Status_RAS;
        mWarning2StatusRAS = args.mInput_SteeringWarning2Status_RAS;
        updatePrivateTelltaleRAS();
        updateConstantRASRedEventStatus();
        updateConstantRASAmberEventStatus();
        updateEventAmber();
        updateEventRed();
    }

    void onMahsCheckStatusChanged(const ArgumentsMahsCheckStatusChanged& args) {
        mCheck1StatusMAHS = args.mInput_SteeringCheck1Status_MAHS;
        updateConstantMAHSAmberEventStatus();
        updateEventAmber();
    }

    void onEhpsCheckStatusChanged(const ArgumentsEhpsCheckStatusChanged& args) {
        mCheck1StatusEHPS1 = args.mInput_SteeringCheck1Status_EHPS1;
        mCheck1StatusEHPS2 = args.mInput_SteeringCheck1Status_EHPS2;
        mCheck2StatusEHPS1 = args.mInput_SteeringCheck2Status_EHPS1;
        mCheck2StatusEHPS2 = args.mInput_SteeringCheck2Status_EHPS2;
        updateConstantEHPSAmberEventStatus();
        updateEventAmber();
    }

    void onRasCheckStatusChanged(const ArgumentsRasCheckStatusChanged& args) {
        mCheck1StatusRAS = args.mInput_SteeringCheck1Status_RAS;
        updateConstantRASAmberEventStatus();
        updateEventAmber();
    }

    void onSteeringModeChangeStatusChanged(const ArgumentsSteeringModeChangeStatusChanged& args) {
        mSteeringModeChangeStatus = args.mInput_SteeringModeChangeStatus;
        updateEventSteeringModeChange();
    }

    void onPstrOilReserverSwitchStatusChanged(const ArgumentsPstrOilReserverSwitchStatusChanged& args) {
        mPSTROilReserverSwitchStatus = args.mInput_PSTROilReserverSwitchStatus;
        updateEventPSTROilReserver();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateTelltaleEHPS1() {
        if (mIsIgnOn == true) {
            mPrivateTelltaleEHPS1 = PrivateTelltaleEHPS1::OFF;
            if (mConfigEHPS1 == ConfigEHPS1::ON) {
                if ((mWarning1StatusEHPS1 == Warning1StatusEHPS1::WARNING &&
                     mWarning2StatusEHPS1 == Warning2StatusEHPS1::NORMAL) ||
                    (mWarning1StatusEHPS1 == Warning1StatusEHPS1::MESSAGE_TIMEOUT &&
                     mWarning2StatusEHPS1 == Warning2StatusEHPS1::MESSAGE_TIMEOUT)) {
                    mPrivateTelltaleEHPS1 = PrivateTelltaleEHPS1::ON;
                } else if (mWarning1StatusEHPS1 == Warning1StatusEHPS1::WARNING &&
                           mWarning2StatusEHPS1 == Warning2StatusEHPS1::WARNING) {
                    mPrivateTelltaleEHPS1 = PrivateTelltaleEHPS1::BLINK2;
                } else {
                    // no operation
                }
            }
#ifdef ENABLE_GCOV_ON
            setSFCPrivateSteering_Warning_LampTelltaleEHPS1Stat(mPrivateTelltaleEHPS1);
#endif
        }
    }

    void updatePrivateTelltaleEHPS2() {
        if (mIsIgnOn == true) {
            mPrivateTelltaleEHPS2 = PrivateTelltaleEHPS2::OFF;
            if (mConfigEHPS2 == ConfigEHPS2::ON) {
                if ((mWarning1StatusEHPS2 == Warning1StatusEHPS2::WARNING &&
                     mWarning2StatusEHPS2 == Warning2StatusEHPS2::NORMAL) ||
                    (mWarning1StatusEHPS2 == Warning1StatusEHPS2::MESSAGE_TIMEOUT &&
                     mWarning2StatusEHPS2 == Warning2StatusEHPS2::MESSAGE_TIMEOUT)) {
                    mPrivateTelltaleEHPS2 = PrivateTelltaleEHPS2::ON;
                } else if (mWarning1StatusEHPS2 == Warning1StatusEHPS2::WARNING &&
                           mWarning2StatusEHPS2 == Warning2StatusEHPS2::WARNING) {
                    mPrivateTelltaleEHPS2 = PrivateTelltaleEHPS2::BLINK2;
                } else {
                    // no operation
                }
            }
#ifdef ENABLE_GCOV_ON
            setSFCPrivateSteering_Warning_LampTelltaleEHPS2Stat(mPrivateTelltaleEHPS2);
#endif
        }
    }

    void updatePrivateTelltaleMAHS() {
        if (mIsIgnOn == true) {
            mPrivateTelltaleMAHS = PrivateTelltaleMAHS::OFF;
            if (mConfigMAHS == ConfigMAHS::ON) {
                if ((mWarning1StatusMAHS == Warning1StatusMAHS::WARNING && mWarning2StatusMAHS == Warning2StatusMAHS::NORMAL) ||
                    (mWarning1StatusMAHS == Warning1StatusMAHS::MESSAGE_TIMEOUT &&
                     mWarning2StatusMAHS == Warning2StatusMAHS::MESSAGE_TIMEOUT)) {
                    mPrivateTelltaleMAHS = PrivateTelltaleMAHS::ON;
                } else if (mWarning1StatusMAHS == Warning1StatusMAHS::WARNING &&
                           mWarning2StatusMAHS == Warning2StatusMAHS::WARNING) {
                    mPrivateTelltaleMAHS = PrivateTelltaleMAHS::BLINK2;
                } else {
                    // no operation
                }
            }
#ifdef ENABLE_GCOV_ON
            setSFCPrivateSteering_Warning_LampTelltaleMAHSStat(mPrivateTelltaleMAHS);
#endif
        }
    }

    void updatePrivateTelltaleRAS() {
        if (mIsIgnOn == true) {
            mPrivateTelltaleRAS = PrivateTelltaleRAS::OFF;
            if (mConfigRAS == ConfigRAS::ON) {
                if ((mWarning1StatusRAS == Warning1StatusRAS::WARNING && mWarning2StatusRAS == Warning2StatusRAS::NORMAL) ||
                    (mWarning1StatusRAS == Warning1StatusRAS::MESSAGE_TIMEOUT &&
                     mWarning2StatusRAS == Warning2StatusRAS::MESSAGE_TIMEOUT)) {
                    mPrivateTelltaleRAS = PrivateTelltaleRAS::ON;
                } else if (mWarning1StatusRAS == Warning1StatusRAS::WARNING && mWarning2StatusRAS == Warning2StatusRAS::WARNING) {
                    mPrivateTelltaleRAS = PrivateTelltaleRAS::BLINK2;
                } else {
                    // no operation
                }
            }
#ifdef ENABLE_GCOV_ON
            setSFCPrivateSteering_Warning_LampTelltaleRASStat(mPrivateTelltaleRAS);
#endif
        }
    }

    // IMG_EHPSRedEvent_Status
    void updateConstantEHPSRedEventStatus() {
        SFCSteering_Warning_LampConstantEHPSRedEventStat stat = SFCSteering_Warning_LampConstantEHPSRedEventStat::OFF;
        if (mIsIgnOn == true &&
            (mPrivateTelltaleEHPS1 == PrivateTelltaleEHPS1::ON || mPrivateTelltaleEHPS1 == PrivateTelltaleEHPS1::BLINK2 ||
             mPrivateTelltaleEHPS2 == PrivateTelltaleEHPS2::ON || mPrivateTelltaleEHPS2 == PrivateTelltaleEHPS2::BLINK2)) {
            stat = SFCSteering_Warning_LampConstantEHPSRedEventStat::ON;
        }
        mEHPSRedEventStat = stat;
        setSFCSteering_Warning_LampConstantEHPSRedEventStat(stat);
    }

    // IMG_MAHSRedEvent_Status
    void updateConstantMAHSRedEventStatus() {
        SFCSteering_Warning_LampConstantMAHSRedEventStat stat = SFCSteering_Warning_LampConstantMAHSRedEventStat::OFF;
        if (mIsIgnOn == true &&
            (mPrivateTelltaleMAHS == PrivateTelltaleMAHS::ON || mPrivateTelltaleMAHS == PrivateTelltaleMAHS::BLINK2)) {
            stat = SFCSteering_Warning_LampConstantMAHSRedEventStat::ON;
        }
        mMAHSRedEventStat = stat;
        setSFCSteering_Warning_LampConstantMAHSRedEventStat(stat);
    }

    // IMG_RASRedEvent_Status
    void updateConstantRASRedEventStatus() {
        SFCSteering_Warning_LampConstantRASRedEventStat stat = SFCSteering_Warning_LampConstantRASRedEventStat::OFF;
        if (mIsIgnOn == true &&
            (mPrivateTelltaleRAS == PrivateTelltaleRAS::ON || mPrivateTelltaleRAS == PrivateTelltaleRAS::BLINK2)) {
            stat = SFCSteering_Warning_LampConstantRASRedEventStat::ON;
        }
        mRASRedEventStat = stat;
        setSFCSteering_Warning_LampConstantRASRedEventStat(stat);
    }

    // IMG_EHPSAmberEvent_Status
    void updateConstantEHPSAmberEventStatus() {
        SFCSteering_Warning_LampConstantEHPSAmberEventStat stat = SFCSteering_Warning_LampConstantEHPSAmberEventStat::OFF;
        if (mIsIgnOn == true &&
            (mPrivateTelltaleEHPS1 == PrivateTelltaleEHPS1::OFF && mPrivateTelltaleEHPS2 == PrivateTelltaleEHPS2::OFF) &&
            (mCheck1StatusEHPS1 == Check1StatusEHPS1::CHECK || mCheck1StatusEHPS2 == Check1StatusEHPS2::CHECK ||
             (mCheck1StatusEHPS1 == Check1StatusEHPS1::NORMAL && mCheck2StatusEHPS1 == Check2StatusEHPS1::CHECK) ||
             (mCheck1StatusEHPS2 == Check1StatusEHPS2::NORMAL && mCheck2StatusEHPS2 == Check2StatusEHPS2::CHECK))) {
            stat = SFCSteering_Warning_LampConstantEHPSAmberEventStat::ON;
        }
        mEHPSAmberEventStat = stat;
        setSFCSteering_Warning_LampConstantEHPSAmberEventStat(stat);
    }

    // IMG_MAHSAmberEvent_Status
    void updateConstantMAHSAmberEventStatus() {
        SFCSteering_Warning_LampConstantMAHSAmberEventStat stat = SFCSteering_Warning_LampConstantMAHSAmberEventStat::OFF;
        if (mIsIgnOn == true && mPrivateTelltaleMAHS == PrivateTelltaleMAHS::OFF &&
            mCheck1StatusMAHS == Check1StatusMAHS::CHECK) {
            stat = SFCSteering_Warning_LampConstantMAHSAmberEventStat::ON;
        }
        mMAHSAmberEventStat = stat;
        setSFCSteering_Warning_LampConstantMAHSAmberEventStat(stat);
    }

    // IMG_RASAmberEvent_Status
    void updateConstantRASAmberEventStatus() {
        SFCSteering_Warning_LampConstantRASAmberEventStat stat = SFCSteering_Warning_LampConstantRASAmberEventStat::OFF;
        if (mIsIgnOn == true && mPrivateTelltaleRAS == PrivateTelltaleRAS::OFF && mCheck1StatusRAS == Check1StatusRAS::CHECK) {
            stat = SFCSteering_Warning_LampConstantRASAmberEventStat::ON;
        }
        mRASAmberEventStat = stat;
        setSFCSteering_Warning_LampConstantRASAmberEventStat(stat);
    }

    // E73501
    void updateEventAmber() {
        SFCSteering_Warning_LampEventAmberStat eventStat = SFCSteering_Warning_LampEventAmberStat::OFF;
        // Constant 값 ON 조건이 Ign On 이므로 Ign 비교구문 삽입 안함
        if (mEHPSAmberEventStat == SFCSteering_Warning_LampConstantEHPSAmberEventStat::ON ||
            mMAHSAmberEventStat == SFCSteering_Warning_LampConstantMAHSAmberEventStat::ON ||
            mRASAmberEventStat == SFCSteering_Warning_LampConstantRASAmberEventStat::ON) {
            eventStat = SFCSteering_Warning_LampEventAmberStat::ON;
        }
        setSFCSteering_Warning_LampEventAmberStat(eventStat);
    }

    // E73502
    void updateEventRed() {
        SFCSteering_Warning_LampEventRedStat eventStat = SFCSteering_Warning_LampEventRedStat::OFF;
        // Constant 값 ON 조건이 Ign On 이므로 Ign 비교구문 삽입 안함
        if (mEHPSRedEventStat == SFCSteering_Warning_LampConstantEHPSRedEventStat::ON ||
            mMAHSRedEventStat == SFCSteering_Warning_LampConstantMAHSRedEventStat::ON ||
            mRASRedEventStat == SFCSteering_Warning_LampConstantRASRedEventStat::ON) {
            eventStat = SFCSteering_Warning_LampEventRedStat::ON;
        }
        setSFCSteering_Warning_LampEventRedStat(eventStat);
    }

    void updateEventSteeringModeChange() {
        static SteeringModeChangeStatus preStat = SteeringModeChangeStatus::COMFORT;

        std::string eventId;
        std::string prevEventId;

        if (mIsIgnOn == true) {
            if (preStat == SteeringModeChangeStatus::HARD && mSteeringModeChangeStatus == SteeringModeChangeStatus::COMFORT) {
                eventId = "E73503";
            } else if (preStat == SteeringModeChangeStatus::COMFORT &&
                       mSteeringModeChangeStatus == SteeringModeChangeStatus::HARD) {
                eventId = "E73504";
            } else {
                // no operation
            }
        }

        preStat = mSteeringModeChangeStatus;

        GETCACHEDVALUE(SFC.Steering_Warning_Lamp.Event.SteeringModeChangeStatus.ID, prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCSteering_Warning_LampEventSteeringModeChangeStatusStat(
                SFCSteering_Warning_LampEventSteeringModeChangeStatusStat::OFF);
            flushLastGroup();
        }

        if (eventId.empty() == false) {
            setSFCSteering_Warning_LampEventSteeringModeChangeStatusID(eventId);
            setSFCSteering_Warning_LampEventSteeringModeChangeStatusStat(
                SFCSteering_Warning_LampEventSteeringModeChangeStatusStat::ON);
        }
    }

    // E73505
    void updateEventPSTROilReserver() {
        SFCSteering_Warning_LampEventPSTROilReserverSwitchStatusStat eventStat =
            SFCSteering_Warning_LampEventPSTROilReserverSwitchStatusStat::OFF;

        if (mIsIgnOn == true) {
            if (mPSTROilReserverSwitchStatus == PSTROilReserverSwitchStatus::ON) {
                eventStat = SFCSteering_Warning_LampEventPSTROilReserverSwitchStatusStat::ON;
            }
        }

        setSFCSteering_Warning_LampEventPSTROilReserverSwitchStatusStat(eventStat);
    }

    using ConfigMAHS = ArgumentsConfigMAHSChanged::Inter_ConfigMAHS;
    using ConfigEHPS1 = ArgumentsConfigEHPS1Changed::Inter_ConfigEHPS1;
    using ConfigEHPS2 = ArgumentsConfigEHPS2Changed::Inter_ConfigEHPS2;
    using ConfigRAS = ArgumentsConfigRASChanged::Inter_ConfigRAS;
    using Warning1StatusEHPS1 = ArgumentsEhps1WarningStatusChanged::Input_SteeringWarning1Status_EHPS1;
    using Warning2StatusEHPS1 = ArgumentsEhps1WarningStatusChanged::Input_SteeringWarning2Status_EHPS1;
    using Warning1StatusEHPS2 = ArgumentsEhps2WarningStatusChanged::Input_SteeringWarning1Status_EHPS2;
    using Warning2StatusEHPS2 = ArgumentsEhps2WarningStatusChanged::Input_SteeringWarning2Status_EHPS2;
    using Warning1StatusMAHS = ArgumentsMahsWarningStatusChanged::Input_SteeringWarning1Status_MAHS;
    using Warning2StatusMAHS = ArgumentsMahsWarningStatusChanged::Input_SteeringWarning2Status_MAHS;
    using Warning1StatusRAS = ArgumentsRasWarningStatusChanged::Input_SteeringWarning1Status_RAS;
    using Warning2StatusRAS = ArgumentsRasWarningStatusChanged::Input_SteeringWarning2Status_RAS;
    using Check1StatusMAHS = ArgumentsMahsCheckStatusChanged::Input_SteeringCheck1Status_MAHS;
    using Check1StatusEHPS1 = ArgumentsEhpsCheckStatusChanged::Input_SteeringCheck1Status_EHPS1;
    using Check1StatusEHPS2 = ArgumentsEhpsCheckStatusChanged::Input_SteeringCheck1Status_EHPS2;
    using Check2StatusEHPS1 = ArgumentsEhpsCheckStatusChanged::Input_SteeringCheck2Status_EHPS1;
    using Check2StatusEHPS2 = ArgumentsEhpsCheckStatusChanged::Input_SteeringCheck2Status_EHPS2;
    using Check1StatusRAS = ArgumentsRasCheckStatusChanged::Input_SteeringCheck1Status_RAS;
    using PrivateTelltaleEHPS1 = SFCPrivateSteering_Warning_LampTelltaleEHPS1Stat;
    using PrivateTelltaleEHPS2 = SFCPrivateSteering_Warning_LampTelltaleEHPS2Stat;
    using PrivateTelltaleMAHS = SFCPrivateSteering_Warning_LampTelltaleMAHSStat;
    using PrivateTelltaleRAS = SFCPrivateSteering_Warning_LampTelltaleRASStat;
    using SteeringModeChangeStatus = ArgumentsSteeringModeChangeStatusChanged::Input_SteeringModeChangeStatus;
    using PSTROilReserverSwitchStatus = ArgumentsPstrOilReserverSwitchStatusChanged::Input_PSTROilReserverSwitchStatus;

    HBool mIsIgnOn = false;
    ConfigMAHS mConfigMAHS = ConfigMAHS::OFF;
    ConfigEHPS1 mConfigEHPS1 = ConfigEHPS1::OFF;
    ConfigEHPS2 mConfigEHPS2 = ConfigEHPS2::OFF;
    ConfigRAS mConfigRAS = ConfigRAS::OFF;
    Warning1StatusEHPS1 mWarning1StatusEHPS1 = Warning1StatusEHPS1::NORMAL;
    Warning2StatusEHPS1 mWarning2StatusEHPS1 = Warning2StatusEHPS1::NORMAL;
    Warning1StatusEHPS2 mWarning1StatusEHPS2 = Warning1StatusEHPS2::NORMAL;
    Warning2StatusEHPS2 mWarning2StatusEHPS2 = Warning2StatusEHPS2::NORMAL;
    Warning1StatusMAHS mWarning1StatusMAHS = Warning1StatusMAHS::NORMAL;
    Warning2StatusMAHS mWarning2StatusMAHS = Warning2StatusMAHS::NORMAL;
    Warning1StatusRAS mWarning1StatusRAS = Warning1StatusRAS::NORMAL;
    Warning2StatusRAS mWarning2StatusRAS = Warning2StatusRAS::NORMAL;
    Check1StatusMAHS mCheck1StatusMAHS = Check1StatusMAHS::NORMAL;
    Check1StatusEHPS1 mCheck1StatusEHPS1 = Check1StatusEHPS1::NORMAL;
    Check1StatusEHPS2 mCheck1StatusEHPS2 = Check1StatusEHPS2::NORMAL;
    Check2StatusEHPS1 mCheck2StatusEHPS1 = Check2StatusEHPS1::NORMAL;
    Check2StatusEHPS2 mCheck2StatusEHPS2 = Check2StatusEHPS2::NORMAL;
    Check1StatusRAS mCheck1StatusRAS = Check1StatusRAS::NORMAL;
    PrivateTelltaleEHPS1 mPrivateTelltaleEHPS1 = PrivateTelltaleEHPS1::OFF;
    PrivateTelltaleEHPS2 mPrivateTelltaleEHPS2 = PrivateTelltaleEHPS2::OFF;
    PrivateTelltaleMAHS mPrivateTelltaleMAHS = PrivateTelltaleMAHS::OFF;
    PrivateTelltaleRAS mPrivateTelltaleRAS = PrivateTelltaleRAS::OFF;
    SteeringModeChangeStatus mSteeringModeChangeStatus = SteeringModeChangeStatus::COMFORT;
    PSTROilReserverSwitchStatus mPSTROilReserverSwitchStatus = PSTROilReserverSwitchStatus::OFF;
    SFCSteering_Warning_LampConstantEHPSRedEventStat mEHPSRedEventStat = SFCSteering_Warning_LampConstantEHPSRedEventStat::OFF;
    SFCSteering_Warning_LampConstantMAHSRedEventStat mMAHSRedEventStat = SFCSteering_Warning_LampConstantMAHSRedEventStat::OFF;
    SFCSteering_Warning_LampConstantRASRedEventStat mRASRedEventStat = SFCSteering_Warning_LampConstantRASRedEventStat::OFF;
    SFCSteering_Warning_LampConstantEHPSAmberEventStat mEHPSAmberEventStat =
        SFCSteering_Warning_LampConstantEHPSAmberEventStat::OFF;
    SFCSteering_Warning_LampConstantMAHSAmberEventStat mMAHSAmberEventStat =
        SFCSteering_Warning_LampConstantMAHSAmberEventStat::OFF;
    SFCSteering_Warning_LampConstantRASAmberEventStat mRASAmberEventStat = SFCSteering_Warning_LampConstantRASAmberEventStat::OFF;
};

}  // namespace ccos

#endif  // SFSS_Steering_Warning_Lamp_H
