/**
 * @file Door_Open_CV.hpp
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
#ifndef SFSS_Door_Open_CV_H
#define SFSS_Door_Open_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Door_Open_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 9.0.0
// Spec Version : v0.20
// Reference : [CV290] Door_Open_CV. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class Door_Open_CV : public Door_Open_CVBase {
public:
    Door_Open_CV() = default;
    ~Door_Open_CV() override = default;
    Door_Open_CV(const Door_Open_CV& other) = delete;
    Door_Open_CV(Door_Open_CV&& other) noexcept = delete;
    Door_Open_CV& operator=(const Door_Open_CV& other) = delete;
    Door_Open_CV& operator=(Door_Open_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDoor_Open_CVEventDoorFailureWarningID("E72902");
        setSFCDoor_Open_CVEventDoorFailureWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Open_CVEventDoorFailureWarningLinkedSoundType(
            SFCDoor_Open_CVEventDoorFailureWarningLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventDoorFailureWarningLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventFrontPanelOpenWarningID("E72903");
        setSFCDoor_Open_CVEventFrontPanelOpenWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCDoor_Open_CVEventFrontPanelOpenWarningLinkedSoundType(
            SFCDoor_Open_CVEventFrontPanelOpenWarningLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventFrontPanelOpenWarningLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventSideLidOpenWarningID("E72904");
        setSFCDoor_Open_CVEventSideLidOpenWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCDoor_Open_CVEventSideLidOpenWarningLinkedSoundType(
            SFCDoor_Open_CVEventSideLidOpenWarningLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventSideLidOpenWarningLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventDoorEmergencyValveOperationID("E72905");
        setSFCDoor_Open_CVEventDoorEmergencyValveOperationLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Open_CVEventDoorEmergencyValveOperationLinkedSoundType(
            SFCDoor_Open_CVEventDoorEmergencyValveOperationLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventDoorEmergencyValveOperationLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventUnlockTheDoorID("E72906");
        setSFCDoor_Open_CVEventUnlockTheDoorLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Open_CVEventUnlockTheDoorLinkedSoundType(SFCDoor_Open_CVEventUnlockTheDoorLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventUnlockTheDoorLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventChargeLidOpenID("E72907");
        setSFCDoor_Open_CVEventChargeLidOpenLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Open_CVEventChargeLidOpenLinkedSoundType(SFCDoor_Open_CVEventChargeLidOpenLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventChargeLidOpenLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventIMUWarningAlarmLinkedSoundID("SND_PopUpInform1");
        setSFCDoor_Open_CVEventIMUWarningAlarmLinkedSoundType(SFCDoor_Open_CVEventIMUWarningAlarmLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Open_CVEventIMUWarningAlarmLinkedSoundRepeatCount(1);
        setSFCDoor_Open_CVEventIMUDoorOpenID("E72910");
        setSFCDoor_Open_CVEventDoorOpenID("E72911");
        setSFCDoor_Open_CVEventDoorOpenLinkedSoundID("SND_PopUpWarn2");
        setSFCDoor_Open_CVEventDoorOpenLinkedSoundType(SFCDoor_Open_CVEventDoorOpenLinkedSoundType::DURATION_MINMAX);
        setSFCDoor_Open_CVEventDoorOpenLinkedSoundDurationMin(kSoundDurationMin);
        setSFCDoor_Open_CVEventDoorOpenLinkedSoundDurationMax(kSoundDurationMax);
        setSFCDoor_Open_CVEventLuggageOpenID("E72912");
        setSFCDoor_Open_CVEventLuggageOpenLinkedSoundID("SND_PopUpWarn2");
        setSFCDoor_Open_CVEventLuggageOpenLinkedSoundType(SFCDoor_Open_CVEventLuggageOpenLinkedSoundType::DURATION_MINMAX);
        setSFCDoor_Open_CVEventLuggageOpenLinkedSoundDurationMin(kSoundDurationMin);
        setSFCDoor_Open_CVEventLuggageOpenLinkedSoundDurationMax(kSoundDurationMax);
        setSFCDoor_Open_CVSoundDoorOpenWarnID("SND_DoorOpenWarn");
        setSFCDoor_Open_CVSoundDoorOpenWarnType(SFCDoor_Open_CVSoundDoorOpenWarnType::DURATION_MINMAX);
        setSFCDoor_Open_CVSoundDoorOpenWarnDurationMin(kSoundDurationMin);
        setSFCDoor_Open_CVSoundDoorOpenWarnDurationMax(kSoundDurationMax);

        if (mOneShotTimerDisplaySpeedValueKPH1s.create(kTimerDisplaySpeedValueKPHInterval1s, this,
                                                       &Door_Open_CV::onOneShotTimerDisplaySpeedValueKPH1s, false) == false) {
            DWarning() << "[Door_Open_CV] mOneShotTimerDisplaySpeedValueKPH1s Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updatePrivateDispalySpeedOverStatus();
        updateEventDoorFailureWarningStatus();
        updateEventFrontPanelOpenWarningStatus();
        updateEventSideLidOpenWarningStatus();
        updateEventDoorEmergencyValveOperationStatus();
        updateEventUnlockTheDoorStatus();
        updateEventChargeLidOpenStatus();
        updateEventIMUWarningAlarmStatus();
        updateEventIMUDoorOpenStatus();
        updateEventDoorOpenStatus();
        updateEventLuggageOpenStatus();
        updateSoundDoorOpenWarnStatus();
        updateConstantBusFrontDoOpenStat();
        updateConstantBusMidDoorOpenStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updatePrivateDispalySpeedOverStatus();
        updateEventDoorFailureWarningStatus();
        updateEventFrontPanelOpenWarningStatus();
        updateEventSideLidOpenWarningStatus();
        updateEventDoorEmergencyValveOperationStatus();
        updateEventUnlockTheDoorStatus();
        updateEventChargeLidOpenStatus();
        updateEventIMUWarningAlarmStatus();
        updateEventIMUDoorOpenStatus();
        updateEventDoorOpenStatus();
        updateEventLuggageOpenStatus();
        updateSoundDoorOpenWarnStatus();
        updateConstantBusFrontDoOpenStat();
        updateConstantBusMidDoorOpenStat();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;

        updatePrivateDoorOpenBusStatus();
        updateEventFrontPanelOpenWarningStatus();
        updateEventSideLidOpenWarningStatus();
        updateEventDoorEmergencyValveOperationStatus();
        updateEventUnlockTheDoorStatus();
        updateEventDoorOpenStatus();
        updateEventLuggageOpenStatus();
        updateSoundDoorOpenWarnStatus();
    }

    void onConfigDoorOptTypeChanged(const ArgumentsConfigDoorOptTypeChanged& args) {
        mConfigDoorOptType = args.mInter_ConfigDoorOptType;

        updateEventIMUDoorOpenStatus();
    }

    void onDoorOpenFrontStatusChanged(const ArgumentsDoorOpenFrontStatusChanged& args) {
        mIMUDrvDoorOpenStatus = args.mInput_IMUDrvDoorOpenStatus;
        mIMUAstDoorOpenStatus = args.mInput_IMUAstDoorOpenStatus;

        updatePrivateDoorOpenFrontStatus();
        updateTelltaleDoorOpenStatus();
        updateEventDoorFailureWarningStatus();
        updateEventDoorOpenStatus();
        updateSoundDoorOpenWarnStatus();
    }

    void onDoorOpenBusStatusChanged(const ArgumentsDoorOpenBusStatusChanged& args) {
        mIMUFrDoorOpenStatus = args.mInput_IMUFrDoorOpenStatus;
        mIMUMidDoorOpenStatus = args.mInput_IMUMidDoorOpenStatus;
        mIMURrDoorOpenStatus = args.mInput_IMURrDoorOpenStatus;

        updatePrivateDoorOpenBusStatus();
        updateTelltaleDoorOpenStatus();
        updateEventDoorFailureWarningStatus();
        updateEventIMUDoorOpenStatus();
        updateEventDoorOpenStatus();
        updateConstantBusFrontDoOpenStat();
        updateConstantBusMidDoorOpenStat();
    }

    void onImuRrFlapDoorOpenStatusChanged(const ArgumentsImuRrFlapDoorOpenStatusChanged& args) {
        mIMURrFlapDoorOpenStatus = args.mInput_IMURrFlapDoorOpenStatus;

        updateTelltaleRearFlapDoorOpenStatus();
    }

    void onImuFrPnlOpenStatusChanged(const ArgumentsImuFrPnlOpenStatusChanged& args) {
        mIMUFrPnlOpenStatus = args.mInput_IMUFrPnlOpenStatus;

        updateEventFrontPanelOpenWarningStatus();
    }

    void onImuSideLidOpenStatusChanged(const ArgumentsImuSideLidOpenStatusChanged& args) {
        mIMUSideLidOpenStatus = args.mInput_IMUSideLidOpenStatus;

        updateEventSideLidOpenWarningStatus();
    }

    void onImuWarningAlarmStatusChanged(const ArgumentsImuWarningAlarmStatusChanged& args) {
        mIMUWarningAlarmStatus = args.mInput_IMUWarningAlarmStatus;

        updateEventIMUWarningAlarmStatus();
        updateEventDoorEmergencyValveOperationStatus();
    }

    void onImuChargeLidOpenStatusChanged(const ArgumentsImuChargeLidOpenStatusChanged& args) {
        mIMUChargeLidOpenStatus = args.mInput_IMUChargeLidOpenStatus;

        updateEventChargeLidOpenStatus();
    }

    void onEmerDoorOpenStatusChanged(const ArgumentsEmerDoorOpenStatusChanged& args) {
        mEmerDoorOpenStatus = args.mInput_EmerDoorOpenStatus;

        updateTelltaleDoorOpenStatus();
    }

    void onLuggageFlapOpenStatusChanged(const ArgumentsLuggageFlapOpenStatusChanged& args) {
        mLuggageFlapOpenStatus = args.mInput_LuggageFlapOpenStatus;

        updateEventLuggageOpenStatus();
    }

    void onImuDoorLockStatusChanged(const ArgumentsImuDoorLockStatusChanged& args) {
        mIMUFrDoorLockStatus = args.mInput_IMUFrDoorLockStatus;
        mIMUMidDoorLockStatus = args.mInput_IMUMidDoorLockStatus;

        updateEventUnlockTheDoorStatus();
    }

    void onSpeedValueChanged(const ArgumentsSpeedValueChanged& args) {
        mDisplaySpeedValueKPH = args.mInter_DisplaySpeedValueKPH;

        updatePrivateDispalySpeedOverStatus();
        updateEventDoorOpenStatus();
        updateEventLuggageOpenStatus();
        updateSoundDoorOpenWarnStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1 운전석/보조석 도어 열림 상태 신호 추상화
    void updatePrivateDoorOpenFrontStatus() {
        if (mIMUDrvDoorOpenStatus == IMUDrvDoorOpenStatus::MESSAGE_TIMEOUT ||
            mIMUAstDoorOpenStatus == IMUAstDoorOpenStatus::MESSAGE_TIMEOUT) {
            mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::MESSAGE_TIMEOUT;
        } else if (mIMUDrvDoorOpenStatus == IMUDrvDoorOpenStatus::OPEN || mIMUAstDoorOpenStatus == IMUAstDoorOpenStatus::OPEN) {
            mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::OPEN;
        } else {
            mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::CLOSE;
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateDoor_Open_CVDoorOpenFrontStat(
            static_cast<SFCPrivateDoor_Open_CVDoorOpenFrontStat>(mPrivate_DoorOpenFrontStatus));
#endif
    }

    // 4.2 버스 전/중/후 도어 열림 상태 신호 추상화
    void updatePrivateDoorOpenBusStatus() {
        mPrivate_DoorOpenBusStatus = Private_DoorOpenBusStatus::CLOSE;

        if (mIMUFrDoorOpenStatus == IMUFrDoorOpenStatus::MESSAGE_TIMEOUT ||
            mIMUMidDoorOpenStatus == IMUMidDoorOpenStatus::MESSAGE_TIMEOUT ||
            mIMURrDoorOpenStatus == IMURrDoorOpenStatus::MESSAGE_TIMEOUT) {
            mPrivate_DoorOpenBusStatus = Private_DoorOpenBusStatus::MESSAGE_TIMEOUT;
        } else if ((mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) &&
                   (mIMUFrDoorOpenStatus == IMUFrDoorOpenStatus::OPEN || mIMUMidDoorOpenStatus == IMUMidDoorOpenStatus::OPEN ||
                    mIMURrDoorOpenStatus == IMURrDoorOpenStatus::OPEN)) {
            mPrivate_DoorOpenBusStatus = Private_DoorOpenBusStatus::OPEN;
        } else {
            // no operation
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateDoor_Open_CVDoorOpenBusStat(static_cast<SFCPrivateDoor_Open_CVDoorOpenBusStat>(mPrivate_DoorOpenBusStatus));
#endif
    }

    // 4.3 도어 열림 사운드 차속 판단
    void updatePrivateDispalySpeedOverStatus() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::OFF &&
                (surelyGreaterThan(mDisplaySpeedValueKPH, kDisplaySpeedValueKPH5) == true ||
                 almostEqual(mDisplaySpeedValueKPH, kDisplaySpeedValueKPH5) == true)) {
                if (mOneShotTimerDisplaySpeedValueKPH1s.isRunning() == false) {
                    if (mOneShotTimerDisplaySpeedValueKPH1s.start() == false) {
                        DWarning() << "[Door_Open_CV] mOneShotTimerDisplaySpeedValueKPH1s Start Failed";
                    }
                }
            } else if (mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::ON &&
                       (surelyLessThan(mDisplaySpeedValueKPH, kDisplaySpeedValueKPH5) == true)) {
                mPrivate_DisplaySpeedOverStatus = Private_DisplaySpeedOverStatus::OFF;
                mOneShotTimerDisplaySpeedValueKPH1s.stop();
            } else {
                // maintain Private_DisplaySpeedOverStatus
                mOneShotTimerDisplaySpeedValueKPH1s.stop();
            }
        } else {
            mPrivate_DisplaySpeedOverStatus = Private_DisplaySpeedOverStatus::OFF;
            mOneShotTimerDisplaySpeedValueKPH1s.stop();
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateDoor_Open_CVDispalySpeedOverStatusStat(
            static_cast<SFCPrivateDoor_Open_CVDispalySpeedOverStatusStat>(mPrivate_DisplaySpeedOverStatus));
#endif
    }

    void onOneShotTimerDisplaySpeedValueKPH1s() {
        if (mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::OFF &&
            (surelyGreaterThan(mDisplaySpeedValueKPH, kDisplaySpeedValueKPH5) == true ||
             almostEqual(mDisplaySpeedValueKPH, kDisplaySpeedValueKPH5) == true)) {
            mPrivate_DisplaySpeedOverStatus = Private_DisplaySpeedOverStatus::ON;
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateDoor_Open_CVDispalySpeedOverStatusStat(
            static_cast<SFCPrivateDoor_Open_CVDispalySpeedOverStatusStat>(mPrivate_DisplaySpeedOverStatus));
#endif
        updateEventDoorOpenStatus();
        updateEventLuggageOpenStatus();
        updateSoundDoorOpenWarnStatus();
        flush();
    }

    // 5.1.1 Door Open Telltale
    void updateTelltaleDoorOpenStatus() {
        SFCDoor_Open_CVTelltaleDoorOpenStat stat = SFCDoor_Open_CVTelltaleDoorOpenStat::OFF;

        if (mPrivate_DoorOpenFrontStatus == Private_DoorOpenFrontStatus::OPEN ||
            mPrivate_DoorOpenBusStatus == Private_DoorOpenBusStatus::OPEN || mEmerDoorOpenStatus == EmerDoorOpenStatus::ON) {
            stat = SFCDoor_Open_CVTelltaleDoorOpenStat::ON;
        }
        setSFCDoor_Open_CVTelltaleDoorOpenStat(stat);
    }

    // 5.1.2 Rear Flap Door Open Telltale
    void updateTelltaleRearFlapDoorOpenStatus() {
        SFCDoor_Open_CVTelltaleRearFlapDoorOpenStat stat = SFCDoor_Open_CVTelltaleRearFlapDoorOpenStat::OFF;

        if (mIMURrFlapDoorOpenStatus == IMURrFlapDoorOpenStatus::OPEN) {
            stat = SFCDoor_Open_CVTelltaleRearFlapDoorOpenStat::ON;
        }
        setSFCDoor_Open_CVTelltaleRearFlapDoorOpenStat(stat);
    }

    // 5.2.1 시내버스 도어 열림 상태
    void updateConstantBusFrontDoOpenStat() {
        SFCDoor_Open_CVConstantBusFrontDoorOpenStat stat = SFCDoor_Open_CVConstantBusFrontDoorOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMUFrDoorOpenStatus == IMUFrDoorOpenStatus::OPEN) {
                stat = SFCDoor_Open_CVConstantBusFrontDoorOpenStat::ON;
            }
        }

        setSFCDoor_Open_CVConstantBusFrontDoorOpenStat(stat);
    }

    void updateConstantBusMidDoorOpenStat() {
        SFCDoor_Open_CVConstantBusMidDoorOpenStat stat = SFCDoor_Open_CVConstantBusMidDoorOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMUMidDoorOpenStatus == IMUMidDoorOpenStatus::OPEN) {
                stat = SFCDoor_Open_CVConstantBusMidDoorOpenStat::ON;
            }
        }

        setSFCDoor_Open_CVConstantBusMidDoorOpenStat(stat);
    }

    // 5.3.1 도어 고장 경고
    void updateEventDoorFailureWarningStatus() {
        SFCDoor_Open_CVEventDoorFailureWarningStat stat = SFCDoor_Open_CVEventDoorFailureWarningStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && (mPrivate_DoorOpenFrontStatus == Private_DoorOpenFrontStatus::MESSAGE_TIMEOUT ||
                                                  mPrivate_DoorOpenBusStatus == Private_DoorOpenBusStatus::MESSAGE_TIMEOUT)) {
            stat = SFCDoor_Open_CVEventDoorFailureWarningStat::ON;
        }
        setSFCDoor_Open_CVEventDoorFailureWarningStat(stat);
    }

    // 5.3.2 프론트 판넬 열림 경고
    void updateEventFrontPanelOpenWarningStatus() {
        SFCDoor_Open_CVEventFrontPanelOpenWarningStat stat = SFCDoor_Open_CVEventFrontPanelOpenWarningStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mIMUFrPnlOpenStatus == IMUFrPnlOpenStatus::OPEN) {
                stat = SFCDoor_Open_CVEventFrontPanelOpenWarningStat::ON;
            }
        }
        setSFCDoor_Open_CVEventFrontPanelOpenWarningStat(stat);
    }

    // 5.3.3 사이드 리드 열림 경고
    void updateEventSideLidOpenWarningStatus() {
        SFCDoor_Open_CVEventSideLidOpenWarningStat stat = SFCDoor_Open_CVEventSideLidOpenWarningStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mIMUSideLidOpenStatus == IMUSideLidOpenStatus::OPEN) {
                stat = SFCDoor_Open_CVEventSideLidOpenWarningStat::ON;
            }
        }
        setSFCDoor_Open_CVEventSideLidOpenWarningStat(stat);
    }

    // 5.3.4 도어 비상 밸브 작동
    void updateEventDoorEmergencyValveOperationStatus() {
        SFCDoor_Open_CVEventDoorEmergencyValveOperationStat stat = SFCDoor_Open_CVEventDoorEmergencyValveOperationStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS && mIMUWarningAlarmStatus == IMUWarningAlarmStatus::EMERGENCY) {
                stat = SFCDoor_Open_CVEventDoorEmergencyValveOperationStat::ON;
            }
        }
        setSFCDoor_Open_CVEventDoorEmergencyValveOperationStat(stat);
    }

    // 5.3.5 도어 잠금 장치 해제
    void updateEventUnlockTheDoorStatus() {
        SFCDoor_Open_CVEventUnlockTheDoorStat stat = SFCDoor_Open_CVEventUnlockTheDoorStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS &&
                (mIMUFrDoorLockStatus == IMUFrDoorLockStatus::ON || mIMUMidDoorLockStatus == IMUMidDoorLockStatus::ON)) {
                stat = SFCDoor_Open_CVEventUnlockTheDoorStat::ON;
            }
        }
        setSFCDoor_Open_CVEventUnlockTheDoorStat(stat);
    }

    // 5.3.6 충전구 상태
    void updateEventChargeLidOpenStatus() {
        SFCDoor_Open_CVEventChargeLidOpenStat stat = SFCDoor_Open_CVEventChargeLidOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMUChargeLidOpenStatus == IMUChargeLidOpenStatus::ON) {
                stat = SFCDoor_Open_CVEventChargeLidOpenStat::ON;
            }
        }
        setSFCDoor_Open_CVEventChargeLidOpenStat(stat);
    }

    // 5.3.7 사용자 모드 진입/해제
    void updateEventIMUWarningAlarmStatus() {
        std::string prevEventID;
        std::string eventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMUWarningAlarmStatus == IMUWarningAlarmStatus::USER_MODE_ON) {
                eventID = "E72908";
            } else if (mIMUWarningAlarmStatus == IMUWarningAlarmStatus::USER_MODE_OFF) {
                eventID = "E72909";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.Door_Open_CV.Event.IMUWarningAlarm.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDoor_Open_CVEventIMUWarningAlarmStat(SFCDoor_Open_CVEventIMUWarningAlarmStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDoor_Open_CVEventIMUWarningAlarmID(eventID);
            setSFCDoor_Open_CVEventIMUWarningAlarmStat(SFCDoor_Open_CVEventIMUWarningAlarmStat::ON);
        }
    }

    // 5.3.8 시내버스 도어 열림 상태 표시
    void updateEventIMUDoorOpenStatus() {
        SFCDoor_Open_CVEventIMUDoorOpenStat stat = SFCDoor_Open_CVEventIMUDoorOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mConfigDoorOptType == ConfigDoorOptType::GY_RHD || mConfigDoorOptType == ConfigDoorOptType::CY_GY ||
                 mConfigDoorOptType == ConfigDoorOptType::CY_DD) &&
                (mIMUFrDoorOpenStatus == IMUFrDoorOpenStatus::OPEN || mIMUMidDoorOpenStatus == IMUMidDoorOpenStatus::OPEN)) {
                stat = SFCDoor_Open_CVEventIMUDoorOpenStat::ON;
            }
        }

        setSFCDoor_Open_CVEventIMUDoorOpenStat(stat);
    }

    // 5.3.9 도어 열림 경고
    void updateEventDoorOpenStatus() {
        SFCDoor_Open_CVEventDoorOpenStat stat = SFCDoor_Open_CVEventDoorOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS &&
            mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::ON &&
            (mPrivate_DoorOpenFrontStatus == Private_DoorOpenFrontStatus::OPEN ||
             mPrivate_DoorOpenBusStatus == Private_DoorOpenBusStatus::OPEN)) {
            stat = SFCDoor_Open_CVEventDoorOpenStat::ON;
        }

        setSFCDoor_Open_CVEventDoorOpenStat(stat);
    }

    // 5.3.10 화물칸 열림 경고
    void updateEventLuggageOpenStatus() {
        SFCDoor_Open_CVEventLuggageOpenStat stat = SFCDoor_Open_CVEventLuggageOpenStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS &&
            mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::ON &&
            mLuggageFlapOpenStatus == LuggageFlapOpenStatus::ON) {
            stat = SFCDoor_Open_CVEventLuggageOpenStat::ON;
        }

        setSFCDoor_Open_CVEventLuggageOpenStat(stat);
    }

    // 5.4.1 도어 열림 사운드 경고
    void updateSoundDoorOpenWarnStatus() {
        SFCDoor_Open_CVSoundDoorOpenWarnStat stat = SFCDoor_Open_CVSoundDoorOpenWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK &&
                mPrivate_DisplaySpeedOverStatus == Private_DisplaySpeedOverStatus::ON &&
                mPrivate_DoorOpenFrontStatus == Private_DoorOpenFrontStatus::OPEN) {
                stat = SFCDoor_Open_CVSoundDoorOpenWarnStat::ON;
            }
        }
        setSFCDoor_Open_CVSoundDoorOpenWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class Private_DoorOpenFrontStatus : HUInt64 {
        NONE,
        OPEN,
        MESSAGE_TIMEOUT,
        CLOSE,
        MAX
    };

    enum class Private_DoorOpenBusStatus : HUInt64 {
        NONE,
        OPEN,
        MESSAGE_TIMEOUT,
        CLOSE,
        MAX
    };

    enum class Private_DisplaySpeedOverStatus : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    using ConfigVehicleTypeCV = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleTypeCV;
    using ConfigDoorOptType = ArgumentsConfigDoorOptTypeChanged::Inter_ConfigDoorOptType;
    using IMUFrDoorOpenStatus = ArgumentsDoorOpenBusStatusChanged::Input_IMUFrDoorOpenStatus;
    using IMUMidDoorOpenStatus = ArgumentsDoorOpenBusStatusChanged::Input_IMUMidDoorOpenStatus;
    using IMURrDoorOpenStatus = ArgumentsDoorOpenBusStatusChanged::Input_IMURrDoorOpenStatus;
    using IMUDrvDoorOpenStatus = ArgumentsDoorOpenFrontStatusChanged::Input_IMUDrvDoorOpenStatus;
    using IMUAstDoorOpenStatus = ArgumentsDoorOpenFrontStatusChanged::Input_IMUAstDoorOpenStatus;
    using IMURrFlapDoorOpenStatus = ArgumentsImuRrFlapDoorOpenStatusChanged::Input_IMURrFlapDoorOpenStatus;
    using IMUFrPnlOpenStatus = ArgumentsImuFrPnlOpenStatusChanged::Input_IMUFrPnlOpenStatus;
    using IMUSideLidOpenStatus = ArgumentsImuSideLidOpenStatusChanged::Input_IMUSideLidOpenStatus;
    using IMUWarningAlarmStatus = ArgumentsImuWarningAlarmStatusChanged::Input_IMUWarningAlarmStatus;
    using IMUChargeLidOpenStatus = ArgumentsImuChargeLidOpenStatusChanged::Input_IMUChargeLidOpenStatus;
    using EmerDoorOpenStatus = ArgumentsEmerDoorOpenStatusChanged::Input_EmerDoorOpenStatus;
    using LuggageFlapOpenStatus = ArgumentsLuggageFlapOpenStatusChanged::Input_LuggageFlapOpenStatus;
    using IMUFrDoorLockStatus = ArgumentsImuDoorLockStatusChanged::Input_IMUFrDoorLockStatus;
    using IMUMidDoorLockStatus = ArgumentsImuDoorLockStatusChanged::Input_IMUMidDoorLockStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    Private_DoorOpenFrontStatus mPrivate_DoorOpenFrontStatus = Private_DoorOpenFrontStatus::CLOSE;
    Private_DoorOpenBusStatus mPrivate_DoorOpenBusStatus = Private_DoorOpenBusStatus::CLOSE;
    Private_DisplaySpeedOverStatus mPrivate_DisplaySpeedOverStatus = Private_DisplaySpeedOverStatus::OFF;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
    ConfigDoorOptType mConfigDoorOptType = ConfigDoorOptType::OFF;
    IMUFrDoorOpenStatus mIMUFrDoorOpenStatus = IMUFrDoorOpenStatus::OFF;
    IMUMidDoorOpenStatus mIMUMidDoorOpenStatus = IMUMidDoorOpenStatus::OFF;
    IMURrDoorOpenStatus mIMURrDoorOpenStatus = IMURrDoorOpenStatus::OFF;
    IMUDrvDoorOpenStatus mIMUDrvDoorOpenStatus = IMUDrvDoorOpenStatus::OFF;
    IMUAstDoorOpenStatus mIMUAstDoorOpenStatus = IMUAstDoorOpenStatus::OFF;
    IMURrFlapDoorOpenStatus mIMURrFlapDoorOpenStatus = IMURrFlapDoorOpenStatus::OFF;
    IMUFrPnlOpenStatus mIMUFrPnlOpenStatus = IMUFrPnlOpenStatus::OFF;
    IMUSideLidOpenStatus mIMUSideLidOpenStatus = IMUSideLidOpenStatus::OFF;
    IMUWarningAlarmStatus mIMUWarningAlarmStatus = IMUWarningAlarmStatus::OFF;
    IMUChargeLidOpenStatus mIMUChargeLidOpenStatus = IMUChargeLidOpenStatus::OFF;
    EmerDoorOpenStatus mEmerDoorOpenStatus = EmerDoorOpenStatus::OFF;
    LuggageFlapOpenStatus mLuggageFlapOpenStatus = LuggageFlapOpenStatus::OFF;
    IMUFrDoorLockStatus mIMUFrDoorLockStatus = IMUFrDoorLockStatus::OFF;
    IMUMidDoorLockStatus mIMUMidDoorLockStatus = IMUMidDoorLockStatus::OFF;

    HDouble mDisplaySpeedValueKPH = 0.0;
    static constexpr HDouble kDisplaySpeedValueKPH5 = 5.0;
    static constexpr HDouble kSoundDisplaySpeedValueKPH7 = 7.0;
    static constexpr HUInt64 kSoundDurationMin = 0;
    static constexpr HUInt64 kSoundDurationMax = 10000;
    static constexpr uint32_t kTimerDisplaySpeedValueKPHInterval1s = 1000;

    ssfs::SFCTimer<Door_Open_CV> mOneShotTimerDisplaySpeedValueKPH1s;
};

}  // namespace ccos

#endif  // SFSS_Door_Open_CV_H
