/**
 * @file Trailer_Mode.hpp
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
#ifndef SFSS_Trailer_Mode_H
#define SFSS_Trailer_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Trailer_ModeBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 10.0.2
// Reference : [PT410] Trailer Mode. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Trailer_Mode : public Trailer_ModeBase {
public:
    Trailer_Mode() = default;
    ~Trailer_Mode() override = default;
    Trailer_Mode(const Trailer_Mode& other) = delete;
    Trailer_Mode(Trailer_Mode&& other) noexcept = delete;
    Trailer_Mode& operator=(const Trailer_Mode& other) = delete;
    Trailer_Mode& operator=(Trailer_Mode&& other) noexcept = delete;

    void onInitialize() override {
        if (mRepeatTimerE23605.create(kTimerInterval500ms, this, &Trailer_Mode::onRepeatTimerE23605Timeout, true) == false) {
            DWarning() << "[Trailer_Mode] mRepeatTimerE23605 Create Failed";
        }
        setSFCTrailer_ModeEventTrailer_ModeID("E23601");
        setSFCTrailer_ModeEventTrailer_ModeLinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTrailer_ModeLinkedSoundType(SFCTrailer_ModeEventTrailer_ModeLinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTrailer_ModeLinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventActiveWarnReqID("E23602");
        setSFCTrailer_ModeEventActiveWarnReqLinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventActiveWarnReqLinkedSoundType(SFCTrailer_ModeEventActiveWarnReqLinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventActiveWarnReqLinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcGainAndGaugeID("E23605");

        setSFCTrailer_ModeEventTowModeUnavailabeID("E23612");
        setSFCTrailer_ModeEventTowModeUnavailabeLinkedSoundID("SND_PopUpInform2");
        setSFCTrailer_ModeEventTowModeUnavailabeLinkedSoundType(
            SFCTrailer_ModeEventTowModeUnavailabeLinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTowModeUnavailabeLinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1ID("E23613");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2ID("E23614");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3ID("E23615");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1ID("E23616");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2ID("E23607");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3ID("E23606");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeDisconnectionID("E23617");
        setSFCTrailer_ModeEventTbcTrailerBrakeDisconnectionLinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeDisconnectionLinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeDisconnectionLinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeDisconnectionLinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckID("E23608");
        setSFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckLinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckLinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckLinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckLinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerLampInspection1LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerLampInspection1LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerLampInspection1LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerLampInspection1LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerLampInspection2ID("E23610");
        setSFCTrailer_ModeEventTbcTrailerLampInspection2LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerLampInspection2LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerLampInspection2LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerLampInspection2LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerLampInspection3ID("E23611");
        setSFCTrailer_ModeEventTbcTrailerLampInspection3LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerLampInspection3LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerLampInspection3LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerLampInspection3LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerLampInspection4ID("E23621");
        setSFCTrailer_ModeEventTbcTrailerLampInspection4LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerLampInspection4LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerLampInspection4LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerLampInspection4LinkedSoundRepeatCount(1);

        setSFCTrailer_ModeEventTbcTrailerLampInspection5ID("E23622");
        setSFCTrailer_ModeEventTbcTrailerLampInspection5LinkedSoundID("SND_PopUpWarn1");
        setSFCTrailer_ModeEventTbcTrailerLampInspection5LinkedSoundType(
            SFCTrailer_ModeEventTbcTrailerLampInspection5LinkedSoundType::REPEAT_COUNT);
        setSFCTrailer_ModeEventTbcTrailerLampInspection5LinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTowModeTelltale();
        updateTrailerModeWarnEvent();
        updateActiveWarnReqEvent();
        updateTowModeUnavailabeEvent();
        updateTrailerModeTelltale();
        updateFlatTowModeTelltale();
        updateTbcTrailerBrakeSystemInspection1Event();
        updateTbcTrailerBrakeSystemInspection2Event();
        updateTbcTrailerBrakeSystemInspection3Event();
        updateTbcTrailerBrakeSwitchInspection1Event();
        updateTbcTrailerBrakeSwitchInspection2Event();
        updateTbcTrailerBrakeSwitchInspection3Event();
        updateTbcTrailerBrakeDisconnectionEvent();
        updateTbcTrailerBrakeConnectionCheckEvent();
        updateTbcTrailerLampInspection1Event();
        updateTbcTrailerLampInspection2Event();
        updateTbcTrailerLampInspection3Event();
        updateTbcTrailerLampInspection4Event();
        updateTbcTrailerLampInspection5Event();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTbcGainAndGaugeEvent();
        // updateTowModeEvent()는 의도적으로 호출하지 않음.
        // 사양서에는 "시동 500ms 이후 판단 시작"으로 기술되어 있어,
        // ON_500ms 이후 Input_TowModeOnOffStatus 값이 변경되었을때 반응하도록 함.
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTowModeTelltale();
        updateTrailerModeWarnEvent();
        updateActiveWarnReqEvent();
        // Ign Off시 TowMode Event Off
        updateTowModeEvent();
        // Ign Off시 TbcGain Event Off
        updateTbcGainAndGaugeEvent();
        updateTowModeUnavailabeEvent();
        updateTrailerModeTelltale();
        updateFlatTowModeTelltale();
        updateTbcTrailerBrakeSystemInspection1Event();
        updateTbcTrailerBrakeSystemInspection2Event();
        updateTbcTrailerBrakeSystemInspection3Event();
        updateTbcTrailerBrakeSwitchInspection1Event();
        updateTbcTrailerBrakeSwitchInspection2Event();
        updateTbcTrailerBrakeSwitchInspection3Event();
        updateTbcTrailerBrakeDisconnectionEvent();
        updateTbcTrailerBrakeConnectionCheckEvent();
        updateTbcTrailerLampInspection1Event();
        updateTbcTrailerLampInspection2Event();
        updateTbcTrailerLampInspection3Event();
        updateTbcTrailerLampInspection4Event();
        updateTbcTrailerLampInspection5Event();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTrailerModeTelltale();
    }

    void onTrailerModeWarnChanged(const ArgumentsTrailerModeWarnChanged& args) {
        mTrailerModeWarnStatus = args.mInput_TrailerModeWarnStatus;
        updateTrailerModeWarnEvent();
    }

    void onActiveWarnReqStatusChanged(const ArgumentsActiveWarnReqStatusChanged& args) {
        mActiveWarnReqStatus = args.mInput_TrailerModeActiveWarnReqStatus;
        updateActiveWarnReqEvent();
    }

    void onTowModeOnOffStatusChanged(const ArgumentsTowModeOnOffStatusChanged& args) {
        mPrevTowModeOnOffStatus = mTowModeOnOffStatus;
        mPrevTowModeOnOffStatusFromTcu = mTowModeOnOffStatusFromTcu;
        mTowModeOnOffStatus = args.mInput_TowModeOnOffStatus;
        mTowModeOnOffStatusFromTcu = args.mInput_TowModeOnOffStatusFromTcu;
        updateTowModeEvent();
        updateTowModeTelltale();
    }

    void onTrailerModeOnOffStatusChanged(const ArgumentsTrailerModeOnOffStatusChanged& args) {
        mTrailerModeOnOffStatus = args.mInput_TrailerModeOnOffStatus;
        updateTrailerModeTelltale();
        updateTbcTrailerBrakeDisconnectionEvent();
    }

    void onFlatTowModeOnOffStatusChanged(const ArgumentsFlatTowModeOnOffStatusChanged& args) {
        mFlatTowModeOnOffStatus = args.mInput_FlatTowModeOnOffStatus;
        updateFlatTowModeTelltale();
    }

    void onItbcTrailerbrakeGainValueChanged(const ArgumentsItbcTrailerbrakeGainValueChanged& args) {
        mItbcTrailerbrakeGainValue = args.mInput_ItbcTrailerbrakeGainValue;
        updateTbcGainConstant();
        updateTbcGainAndGaugeEvent();
    }

    void onItbcTrailerbrakeGaugeValueChanged(const ArgumentsItbcTrailerbrakeGaugeValueChanged& args) {
        mItbcTrailerbrakeGaugeValue = args.mInput_ItbcTrailerbrakeGaugeValue;
        updateTbcGaugeConstant();
    }

    void onItbcTbcSwitchSqueezeValueChanged(const ArgumentsItbcTbcSwitchSqueezeValueChanged& args) {
        mItbcTbcSwitchSqueezeValue = args.mInput_ItbcTbcSwitchSqueezeValue;
        updateTbcGainAndGaugeEvent();
    }

    void onItbcTurnLampWarnStatusChanged(const ArgumentsItbcTurnLampWarnStatusChanged& args) {
        mItbcTurnLampWarnStatus = args.mInput_ItbcTurnLampWarnStatus;
        updateTbcTrailerLampInspection3Event();
        updateTbcTrailerLampInspection4Event();
        updateTbcTrailerLampInspection5Event();
    }

    void onItbcStopLampWarnStatusChanged(const ArgumentsItbcStopLampWarnStatusChanged& args) {
        mItbcStopLampWarnStatus = args.mInput_ItbcStopLampWarnStatus;
        updateTbcTrailerLampInspection2Event();
    }

    void onTowModeUnavailabeStatusChanged(const ArgumentsTowModeUnavailabeStatusChanged& args) {
        mTowModeUnavailabeStatus = args.mInput_TowModeUnavailabeStatus;
        updateTowModeUnavailabeEvent();
    }

    void onItbcTrailerbrakeFaultWarnStatusChanged(const ArgumentsItbcTrailerbrakeFaultWarnStatusChanged& args) {
        mItbcTrailerbrakeFaultWarnStatus = args.mInput_ItbcTrailerbrakeFaultWarnStatus;
        updateTbcTrailerBrakeSystemInspection1Event();
        updateTbcTrailerBrakeSystemInspection2Event();
        updateTbcTrailerBrakeSystemInspection3Event();
    }

    void onItbcTrailerbrakeOperationWarnStatusChanged(const ArgumentsItbcTrailerbrakeOperationWarnStatusChanged& args) {
        mItbcTrailerbrakeOperationWarnStatus = args.mInput_ItbcTrailerbrakeOperationWarnStatus;
        updateTbcTrailerBrakeSwitchInspection1Event();
        updateTbcTrailerBrakeSwitchInspection2Event();
        updateTbcTrailerBrakeSwitchInspection3Event();
    }

    void onItbcTrailerConnectStatusChanged(const ArgumentsItbcTrailerConnectStatusChanged& args) {
        mItbcTrailerConnectStatus = args.mInput_ItbcTrailerConnectStatus;
        updateTbcTrailerBrakeConnectionCheckEvent();
    }

    void onItbcTrailerLampWarnStatusChanged(const ArgumentsItbcTrailerLampWarnStatusChanged& args) {
        mItbcTrailerLampWarnStatus = args.mInput_ItbcTrailerLampWarnStatus;
        updateTbcTrailerLampInspection1Event();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTowModeTelltale() {
        SFCTrailer_ModeTelltaleTowModeStat stat = SFCTrailer_ModeTelltaleTowModeStat::OFF;

        if (mIsIgnOn == true) {
            if (mTowModeOnOffStatus == TowModeOnOffStatus::ON || mTowModeOnOffStatusFromTcu == TowModeOnOffStatusFromTcu::ON) {
                stat = SFCTrailer_ModeTelltaleTowModeStat::ON;
            }
        }
        setSFCTrailer_ModeTelltaleTowModeStat(stat);
    }

    void updateTrailerModeWarnEvent() {
        SFCTrailer_ModeEventTrailer_ModeStat stat = SFCTrailer_ModeEventTrailer_ModeStat::OFF;
        if (mIsIgnOn == true && mTrailerModeWarnStatus == TrailerModeWarnStatus::WARN_ON) {
            stat = SFCTrailer_ModeEventTrailer_ModeStat::ON;
        }
        setSFCTrailer_ModeEventTrailer_ModeStat(stat);
    }

    void updateActiveWarnReqEvent() {
        SFCTrailer_ModeEventActiveWarnReqStat stat = SFCTrailer_ModeEventActiveWarnReqStat::OFF;
        if (mIsIgnOn == true && mActiveWarnReqStatus == ActiveWarnReqStatus::ON) {
            stat = SFCTrailer_ModeEventActiveWarnReqStat::ON;
        }
        setSFCTrailer_ModeEventActiveWarnReqStat(stat);
    }

    void updateTowModeEvent() {
        std::string eventID;
        std::string prevEventID;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if ((mPrevTowModeOnOffStatus == TowModeOnOffStatus::OFF && mTowModeOnOffStatus == TowModeOnOffStatus::ON) ||
                (mPrevTowModeOnOffStatusFromTcu == TowModeOnOffStatusFromTcu::OFF &&
                 mTowModeOnOffStatusFromTcu == TowModeOnOffStatusFromTcu::ON)) {
                eventID = "E23603";
            } else if ((mPrevTowModeOnOffStatus == TowModeOnOffStatus::ON && mTowModeOnOffStatus == TowModeOnOffStatus::OFF) ||
                       (mPrevTowModeOnOffStatusFromTcu == TowModeOnOffStatusFromTcu::ON &&
                        mTowModeOnOffStatusFromTcu == TowModeOnOffStatusFromTcu::OFF)) {
                eventID = "E23604";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.Trailer_Mode.Event.TowMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTrailer_ModeEventTowModeStat(SFCTrailer_ModeEventTowModeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTrailer_ModeEventTowModeID(eventID);
            setSFCTrailer_ModeEventTowModeStat(SFCTrailer_ModeEventTowModeStat::ON);
        }
    }

    // EXNCP-45086
    // Input_ItbcTbcSwitchSqueezeValue == SWITCH_ON 상태에서는 Timer 를 동작시켜 E23605 ON 이 500ms 간격으로 App에 전달되도록 수정
    void onRepeatTimerE23605Timeout() {
        SFCTrailer_ModeEventTbcGainAndGaugeStat stat = SFCTrailer_ModeEventTbcGainAndGaugeStat::ON;
        if (mItbcTbcSwitchSqueezeValue == ItbcTbcSwitchSqueezeValue::SWITCH_ON) {
            setSFCTrailer_ModeEventTbcGainAndGaugeStat(stat);
            if (stat == SFCTrailer_ModeEventTbcGainAndGaugeStat::ON) {
                flush(true);
            }
        }
    }

    void updateTbcGainAndGaugeEvent() {
        SFCTrailer_ModeEventTbcGainAndGaugeStat stat = SFCTrailer_ModeEventTbcGainAndGaugeStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mItbcTbcSwitchSqueezeValue == ItbcTbcSwitchSqueezeValue::SWITCH_ON) {
                stat = SFCTrailer_ModeEventTbcGainAndGaugeStat::ON;
                if (mRepeatTimerE23605.start() == false) {
                    DWarning() << "[Trailer_Mode] mRepeatTimerE23605 Start Failed";
                }
            } else {
                mRepeatTimerE23605.stop();
                if ((mPrevItbcTrailerbrakeGainValue >= 0 && mPrevItbcTrailerbrakeGainValue <= 20) &&
                    (mItbcTrailerbrakeGainValue >= 0 && mItbcTrailerbrakeGainValue <= 20) &&
                    (mItbcTrailerbrakeGainValue != mPrevItbcTrailerbrakeGainValue)) {
                    stat = SFCTrailer_ModeEventTbcGainAndGaugeStat::ON;
                }
            }
        } else {
            mRepeatTimerE23605.stop();
        }

        setSFCTrailer_ModeEventTbcGainAndGaugeStat(stat);
        if (stat == SFCTrailer_ModeEventTbcGainAndGaugeStat::ON) {
            // EXNCP-42764: Event ON인 상태에서, 동일하게 ON 전달
            flush(true);
        }
        mPrevItbcTrailerbrakeGainValue = mItbcTrailerbrakeGainValue;
    }

    void updateTowModeUnavailabeEvent() {
        SFCTrailer_ModeEventTowModeUnavailabeStat stat = SFCTrailer_ModeEventTowModeUnavailabeStat::OFF;
        if (mIsIgnOn == true && mTowModeUnavailabeStatus == TowModeUnavailabeStatus::FAIL) {
            stat = SFCTrailer_ModeEventTowModeUnavailabeStat::ON;
        }
        setSFCTrailer_ModeEventTowModeUnavailabeStat(stat);
    }

    void updateTbcTrailerBrakeSystemInspection1Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1Stat::OFF;

        if (mIsIgnOn == true && (mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CHECK_WIRING ||
                                 mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection1Stat(stat);
    }

    void updateTbcTrailerBrakeSystemInspection2Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2Stat::OFF;
        if (mIsIgnOn == true && (mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CHECK_BRAKE_MODULE ||
                                 mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection2Stat(stat);
    }

    void updateTbcTrailerBrakeSystemInspection3Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3Stat::OFF;
        if (mIsIgnOn == true && (mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CHECK_BRAKE_SYS ||
                                 mItbcTrailerbrakeFaultWarnStatus == ItbcTrailerbrakeFaultWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSystemInspection3Stat(stat);
    }

    void updateTbcTrailerBrakeSwitchInspection1Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1Stat::OFF;
        if (mIsIgnOn == true && (mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::USE_BRAKE_SWITCH ||
                                 mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection1Stat(stat);
    }

    void updateTbcTrailerBrakeSwitchInspection2Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2Stat::OFF;
        if (mIsIgnOn == true &&
            (mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::CHECK_TRAILER_SWTICH ||
             mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection2Stat(stat);
    }

    void updateTbcTrailerBrakeSwitchInspection3Event() {
        SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3Stat stat =
            SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3Stat::OFF;
        if (mIsIgnOn == true &&
            (mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::NOT_OPERATION_BRAKE ||
             mItbcTrailerbrakeOperationWarnStatus == ItbcTrailerbrakeOperationWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeSwitchInspection3Stat(stat);
    }

    void updateTbcTrailerBrakeDisconnectionEvent() {
        SFCTrailer_ModeEventTbcTrailerBrakeDisconnectionStat stat = SFCTrailer_ModeEventTbcTrailerBrakeDisconnectionStat::OFF;
        if (mIsIgnOn == true && mTrailerModeOnOffStatus == TrailerModeOnOffStatus::DISCONNECT) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeDisconnectionStat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeDisconnectionStat(stat);
    }

    void updateTbcTrailerBrakeConnectionCheckEvent() {
        SFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckStat stat = SFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckStat::OFF;
        if (mIsIgnOn == true && mItbcTrailerConnectStatus == ItbcTrailerConnectStatus::TBC_CONNECT_CHECK) {
            stat = SFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckStat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerBrakeConnectionCheckStat(stat);
    }

    void updateTbcTrailerLampInspection1Event() {
        std::string eventID;
        std::string prevEventID;
        if (mIsIgnOn == true) {
            switch (mItbcTrailerLampWarnStatus) {
                case ItbcTrailerLampWarnStatus::CHECK_TAILLAMP:
                    eventID = "E23618";
                    break;
                case ItbcTrailerLampWarnStatus::CHECK_REARFOG:
                    eventID = "E23619";
                    break;
                case ItbcTrailerLampWarnStatus::CHECK_BACKUPLAMP:
                    eventID = "E23620";
                    break;
                default:
                    // no operation
                    break;
            }
        }
        GETCACHEDVALUE(SFC.Trailer_Mode.Event.TbcTrailerLampInspection1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCTrailer_ModeEventTbcTrailerLampInspection1Stat(SFCTrailer_ModeEventTbcTrailerLampInspection1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCTrailer_ModeEventTbcTrailerLampInspection1ID(eventID);
            setSFCTrailer_ModeEventTbcTrailerLampInspection1Stat(SFCTrailer_ModeEventTbcTrailerLampInspection1Stat::ON);
        }
    }

    void updateTbcTrailerLampInspection2Event() {
        SFCTrailer_ModeEventTbcTrailerLampInspection2Stat stat = SFCTrailer_ModeEventTbcTrailerLampInspection2Stat::OFF;
        if (mIsIgnOn == true && (mItbcStopLampWarnStatus == ItbcStopLampWarnStatus::ON ||
                                 mItbcStopLampWarnStatus == ItbcStopLampWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerLampInspection2Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerLampInspection2Stat(stat);
    }

    void updateTbcTrailerLampInspection3Event() {
        SFCTrailer_ModeEventTbcTrailerLampInspection3Stat stat = SFCTrailer_ModeEventTbcTrailerLampInspection3Stat::OFF;
        if (mIsIgnOn == true && (mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::WARN_LEFT ||
                                 mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerLampInspection3Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerLampInspection3Stat(stat);
    }

    void updateTbcTrailerLampInspection4Event() {
        SFCTrailer_ModeEventTbcTrailerLampInspection4Stat stat = SFCTrailer_ModeEventTbcTrailerLampInspection4Stat::OFF;
        if (mIsIgnOn == true && (mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::WARN_RIGHT ||
                                 mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerLampInspection4Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerLampInspection4Stat(stat);
    }

    void updateTbcTrailerLampInspection5Event() {
        SFCTrailer_ModeEventTbcTrailerLampInspection5Stat stat = SFCTrailer_ModeEventTbcTrailerLampInspection5Stat::OFF;
        if (mIsIgnOn == true && (mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::WARN_ALL ||
                                 mItbcTurnLampWarnStatus == ItbcTurnLampWarnStatus::CRC_ERROR)) {
            stat = SFCTrailer_ModeEventTbcTrailerLampInspection5Stat::ON;
        }
        setSFCTrailer_ModeEventTbcTrailerLampInspection5Stat(stat);
    }

    void updateTrailerModeTelltale() {
        if (mIsIgnOn == true && mConfigVehicleType == ConfigVehicleType::EV) {
            if (mTrailerModeOnOffStatus == TrailerModeOnOffStatus::ON) {
                setSFCTrailer_ModeTelltaleTrailerModeStat(SFCTrailer_ModeTelltaleTrailerModeStat::ON);
            } else if (mTrailerModeOnOffStatus == TrailerModeOnOffStatus::OFF) {
                setSFCTrailer_ModeTelltaleTrailerModeStat(SFCTrailer_ModeTelltaleTrailerModeStat::OFF);
            } else {
                setSFCTrailer_ModeTelltaleTrailerModeStat(SFCTrailer_ModeTelltaleTrailerModeStat::OFF);
            }
        } else {
            setSFCTrailer_ModeTelltaleTrailerModeStat(SFCTrailer_ModeTelltaleTrailerModeStat::OFF);
        }
    }

    void updateFlatTowModeTelltale() {
        SFCTrailer_ModeTelltaleFlatTowModeStat stat = SFCTrailer_ModeTelltaleFlatTowModeStat::OFF;
        SFCTrailer_ModeTelltaleFlatTowModeStatOptional statOptional = SFCTrailer_ModeTelltaleFlatTowModeStatOptional::OFF;

        if (mIsIgnOn == true) {
            if (mFlatTowModeOnOffStatus == FlatTowModeOnOffStatus::ON_NONCHARGE) {
                stat = SFCTrailer_ModeTelltaleFlatTowModeStat::OPTIONAL;
                statOptional = SFCTrailer_ModeTelltaleFlatTowModeStatOptional::ON_NONCHARGE;
            } else if (mFlatTowModeOnOffStatus == FlatTowModeOnOffStatus::ON_CHARGE) {
                stat = SFCTrailer_ModeTelltaleFlatTowModeStat::OPTIONAL;
                statOptional = SFCTrailer_ModeTelltaleFlatTowModeStatOptional::ON_CHARGE;
            } else {
                // no operation
            }
        } else {
            // no operaton
        }
        setSFCTrailer_ModeTelltaleFlatTowModeStat(stat);
        setSFCTrailer_ModeTelltaleFlatTowModeStatOptional(statOptional);
    }

    void updateTbcGainConstant() {
        HDouble value = 0.0;
        if (mItbcTrailerbrakeGainValue >= 0 && mItbcTrailerbrakeGainValue <= 20) {
            value = mItbcTrailerbrakeGainValue * 0.5;
        }
        setSFCTrailer_ModeConstantTbcGainValue(value);
    }

    void updateTbcGaugeConstant() {
        HUInt64 value = 0;
        if (mItbcTrailerbrakeGaugeValue >= 0 && mItbcTrailerbrakeGaugeValue <= 100) {
            value = mItbcTrailerbrakeGaugeValue;
        }
        setSFCTrailer_ModeConstantTbcGaugeValue(value);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using TrailerModeWarnStatus = ArgumentsTrailerModeWarnChanged::Input_TrailerModeWarnStatus;
    using ActiveWarnReqStatus = ArgumentsActiveWarnReqStatusChanged::Input_TrailerModeActiveWarnReqStatus;
    using TowModeOnOffStatus = ArgumentsTowModeOnOffStatusChanged::Input_TowModeOnOffStatus;
    using TowModeOnOffStatusFromTcu = ArgumentsTowModeOnOffStatusChanged::Input_TowModeOnOffStatusFromTcu;
    using TrailerModeOnOffStatus = ArgumentsTrailerModeOnOffStatusChanged::Input_TrailerModeOnOffStatus;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using FlatTowModeOnOffStatus = ArgumentsFlatTowModeOnOffStatusChanged::Input_FlatTowModeOnOffStatus;
    using ItbcTbcSwitchSqueezeValue = ArgumentsItbcTbcSwitchSqueezeValueChanged::Input_ItbcTbcSwitchSqueezeValue;
    using ItbcTurnLampWarnStatus = ArgumentsItbcTurnLampWarnStatusChanged::Input_ItbcTurnLampWarnStatus;
    using ItbcStopLampWarnStatus = ArgumentsItbcStopLampWarnStatusChanged::Input_ItbcStopLampWarnStatus;
    using TowModeUnavailabeStatus = ArgumentsTowModeUnavailabeStatusChanged::Input_TowModeUnavailabeStatus;
    using ItbcTrailerbrakeFaultWarnStatus =
        ArgumentsItbcTrailerbrakeFaultWarnStatusChanged::Input_ItbcTrailerbrakeFaultWarnStatus;
    using ItbcTrailerbrakeOperationWarnStatus =
        ArgumentsItbcTrailerbrakeOperationWarnStatusChanged::Input_ItbcTrailerbrakeOperationWarnStatus;
    using ItbcTrailerConnectStatus = ArgumentsItbcTrailerConnectStatusChanged::Input_ItbcTrailerConnectStatus;
    using ItbcTrailerLampWarnStatus = ArgumentsItbcTrailerLampWarnStatusChanged::Input_ItbcTrailerLampWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HBool mIsIgnOn = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    TrailerModeWarnStatus mTrailerModeWarnStatus = TrailerModeWarnStatus::WARN_OFF;
    ActiveWarnReqStatus mActiveWarnReqStatus = ActiveWarnReqStatus::OFF;
    TowModeOnOffStatus mTowModeOnOffStatus = TowModeOnOffStatus::UNHANDLED_TIMEOUT;
    TowModeOnOffStatus mPrevTowModeOnOffStatus = TowModeOnOffStatus::UNHANDLED_TIMEOUT;
    TowModeOnOffStatusFromTcu mTowModeOnOffStatusFromTcu = TowModeOnOffStatusFromTcu::UNHANDLED_TIMEOUT;
    TowModeOnOffStatusFromTcu mPrevTowModeOnOffStatusFromTcu = TowModeOnOffStatusFromTcu::UNHANDLED_TIMEOUT;
    TrailerModeOnOffStatus mTrailerModeOnOffStatus = TrailerModeOnOffStatus::UNHANDLED_TIMEOUT;
    FlatTowModeOnOffStatus mFlatTowModeOnOffStatus = FlatTowModeOnOffStatus::UNHANDLED_TIMEOUT;
    HUInt64 mItbcTrailerbrakeGainValue = 0;
    HUInt64 mPrevItbcTrailerbrakeGainValue = 0;
    HUInt64 mItbcTrailerbrakeGaugeValue = 0;
    ItbcTbcSwitchSqueezeValue mItbcTbcSwitchSqueezeValue = ItbcTbcSwitchSqueezeValue::OFF;
    ItbcTurnLampWarnStatus mItbcTurnLampWarnStatus = ItbcTurnLampWarnStatus::OFF;
    ItbcStopLampWarnStatus mItbcStopLampWarnStatus = ItbcStopLampWarnStatus::OFF;
    TowModeUnavailabeStatus mTowModeUnavailabeStatus = TowModeUnavailabeStatus::OFF;
    ItbcTrailerbrakeFaultWarnStatus mItbcTrailerbrakeFaultWarnStatus = ItbcTrailerbrakeFaultWarnStatus::OFF;
    ItbcTrailerbrakeOperationWarnStatus mItbcTrailerbrakeOperationWarnStatus = ItbcTrailerbrakeOperationWarnStatus::OFF;
    ItbcTrailerConnectStatus mItbcTrailerConnectStatus = ItbcTrailerConnectStatus::OFF;
    ItbcTrailerLampWarnStatus mItbcTrailerLampWarnStatus = ItbcTrailerLampWarnStatus::OFF;
    ssfs::SFCTimer<Trailer_Mode> mRepeatTimerE23605;

    static constexpr uint32_t kTimerInterval500ms = 500u;
};

}  // namespace ccos

#endif  // SFSS_Trailer_Mode_H
