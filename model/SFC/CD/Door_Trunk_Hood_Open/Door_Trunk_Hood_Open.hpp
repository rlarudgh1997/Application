/**
 * @file Door_Trunk_Hood_Open.hpp
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
#ifndef SFSS_Door_Trunk_Hood_Open_H
#define SFSS_Door_Trunk_Hood_Open_H

#define DLOG_ENABLED gEnableSFCLog

#include "Door_Trunk_Hood_OpenBase.hpp"
#include <SFCTimer.h>
namespace ccos {

// SFC Version : 11.0.0
// Reference : [CD010] Door_Trunk_Hood_Open Contains : Constant, Event, Telltale Recommend: VALUE_CHANGED
class Door_Trunk_Hood_Open : public Door_Trunk_Hood_OpenBase {
public:
    Door_Trunk_Hood_Open() = default;
    ~Door_Trunk_Hood_Open() override = default;
    Door_Trunk_Hood_Open(const Door_Trunk_Hood_Open& other) = delete;
    Door_Trunk_Hood_Open(Door_Trunk_Hood_Open&& other) noexcept = delete;
    Door_Trunk_Hood_Open& operator=(const Door_Trunk_Hood_Open& other) = delete;
    Door_Trunk_Hood_Open& operator=(Door_Trunk_Hood_Open&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDoor_Trunk_Hood_OpenEventOpenStatusDisplayID("E00101");
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningID("E00102");
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorLinkedSoundType(EventFailureWarningDoorLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorLinkedSoundRepeatCount(kEventFailureWarnDoorLinkedSoundRepeatCount1);
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkLinkedSoundType(EventFailureWarningTrunkLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkLinkedSoundRepeatCount(kEventFailureWarnTrunkLinkedSoundRepeatCount1);
        setSFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStatOptional(
            SFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStatOptional::WARN_ON);

        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodID("E00107");
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodLinkedSoundID("SND_PopUpWarn1");
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodLinkedSoundType(
            SFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodLinkedSoundRepeatCount(1);

        setSFCDoor_Trunk_Hood_OpenEventGuideReqID("E00108");
        setSFCDoor_Trunk_Hood_OpenEventGuideReqLinkedSoundID("SND_PopUpInform2");
        setSFCDoor_Trunk_Hood_OpenEventGuideReqLinkedSoundType(SFCDoor_Trunk_Hood_OpenEventGuideReqLinkedSoundType::REPEAT_COUNT);
        setSFCDoor_Trunk_Hood_OpenEventGuideReqLinkedSoundRepeatCount(1);

        if (mOneShotTimerOpenStatusDisplay.create(kTimerOpenStatusInterval100ms, this,
                                                  &Door_Trunk_Hood_Open::onOneShotTimerOpenStatusDisplay, false) == false) {
            DWarning() << "[Door_Trunk_Hood_Open] mOneShotTimerOpenStatusDisplay Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        DDebug() << "IgnElapsed::ON_0ms";
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateInfo();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        DDebug() << "IgnElapsed::OFF_0ms";
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateInfo();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigSBCM = args.mInter_ConfigSBCM;
        updateInfo();
    }

    void onDoorOpenChanged(const ArgumentsDoorOpenChanged& args) {
        DDebug() << "Input_DoorOpenDriverSideStatus " << static_cast<HUInt64>(args.mInput_DoorOpenDriverSideStatus);
        DDebug() << "Input_DoorOpenAssistSideStatus " << static_cast<HUInt64>(args.mInput_DoorOpenAssistSideStatus);
        DDebug() << "Input_DoorOpenRearLeftSideStatus " << static_cast<HUInt64>(args.mInput_DoorOpenRearLeftSideStatus);
        DDebug() << "Input_DoorOpenRearRightSideStatus " << static_cast<HUInt64>(args.mInput_DoorOpenRearRightSideStatus);
        DDebug() << "Input_TrunkOpenStatus " << static_cast<HUInt64>(args.mInput_TrunkOpenStatus);
        DDebug() << "Input_HoodOpenStatus " << static_cast<HUInt64>(args.mInput_HoodOpenStatus);
        DDebug() << "Input_DoorOpenAssistSideStatusFromSBCM "
                 << static_cast<HUInt64>(args.mInput_DoorOpenAssistSideStatusFromSBCM);
        DDebug() << "Input_DoorOpenDriverSideStatusFromSBCM "
                 << static_cast<HUInt64>(args.mInput_DoorOpenDriverSideStatusFromSBCM);
        DDebug() << "Input_DoorOpenRearLeftSideStatusFromSBCM "
                 << static_cast<HUInt64>(args.mInput_DoorOpenRearLeftSideStatusFromSBCM);
        DDebug() << "Input_DoorOpenRearRightSideStatusFromSBCM "
                 << static_cast<HUInt64>(args.mInput_DoorOpenRearRightSideStatusFromSBCM);
        DDebug() << "Input_ElecHoodOpenStatus " << static_cast<HUInt64>(args.mInput_ElecHoodOpenStatus);
        DDebug() << "Input_ElecHoodOptionStatus " << static_cast<HUInt64>(args.mInput_ElecHoodOptionStatus);
        if (ISTIMEOUT(args.mInput_DoorOpenDriverSideStatus) == true) {
            mInput_DoorOpenDriverSideStatus = Input_DoorOpenDriverSideStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenDriverSideStatus = args.mInput_DoorOpenDriverSideStatus;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenAssistSideStatus) == true) {
            mInput_DoorOpenAssistSideStatus = Input_DoorOpenAssistSideStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenAssistSideStatus = args.mInput_DoorOpenAssistSideStatus;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenRearLeftSideStatus) == true) {
            mInput_DoorOpenRearLeftSideStatus = Input_DoorOpenRearLeftSideStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenRearLeftSideStatus = args.mInput_DoorOpenRearLeftSideStatus;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenRearRightSideStatus) == true) {
            mInput_DoorOpenRearRightSideStatus = Input_DoorOpenRearRightSideStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenRearRightSideStatus = args.mInput_DoorOpenRearRightSideStatus;
        }

        if (ISTIMEOUT(args.mInput_TrunkOpenStatus) == true) {
            mInput_TrunkOpenStatus = Input_TrunkOpenStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_TrunkOpenStatus = args.mInput_TrunkOpenStatus;
        }

        if (ISTIMEOUT(args.mInput_HoodOpenStatus) == true) {
            mInput_HoodOpenStatus = Input_HoodOpenStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_HoodOpenStatus = args.mInput_HoodOpenStatus;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenAssistSideStatusFromSBCM) == true) {
            mInput_DoorOpenAssistSideStatusFromSBCM = Input_DoorOpenAssistSideStatusFromSBCM::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenAssistSideStatusFromSBCM = args.mInput_DoorOpenAssistSideStatusFromSBCM;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenDriverSideStatusFromSBCM) == true) {
            mInput_DoorOpenDriverSideStatusFromSBCM = Input_DoorOpenDriverSideStatusFromSBCM::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenDriverSideStatusFromSBCM = args.mInput_DoorOpenDriverSideStatusFromSBCM;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenRearLeftSideStatusFromSBCM) == true) {
            mInput_DoorOpenRearLeftSideStatusFromSBCM = Input_DoorOpenRearLeftSideStatusFromSBCM::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenRearLeftSideStatusFromSBCM = args.mInput_DoorOpenRearLeftSideStatusFromSBCM;
        }

        if (ISTIMEOUT(args.mInput_DoorOpenRearRightSideStatusFromSBCM) == true) {
            mInput_DoorOpenRearRightSideStatusFromSBCM = Input_DoorOpenRearRightSideStatusFromSBCM::UNHANDLED_TIMEOUT;
        } else {
            mInput_DoorOpenRearRightSideStatusFromSBCM = args.mInput_DoorOpenRearRightSideStatusFromSBCM;
        }
        mInput_ElecHoodOpenStatus = args.mInput_ElecHoodOpenStatus;
        mInput_ElecHoodOptionStatus = args.mInput_ElecHoodOptionStatus;
        updateInfo();
    }

    void onRgwDoorOpenChanged(const ArgumentsRgwDoorOpenChanged& args) {
        DDebug() << "Input_RGWDoorOpenStatus " << static_cast<HUInt64>(args.mInput_RGWDoorOpenStatus);
        mInput_RGWDoorOpenStatus = args.mInput_RGWDoorOpenStatus;
        updateEventOpenWarning();
    }

    void onRgwDoorOpenChangedFromSAC(const ArgumentsRgwDoorOpenChangedFromSAC& args) {
        DDebug() << "Input_RGWDoorOpenStatusFromSAC " << static_cast<HUInt64>(args.mInput_RGWDoorOpenStatusFromSAC);
        mInput_RGWDoorOpenStatusFromSAC = args.mInput_RGWDoorOpenStatusFromSAC;
        updateEventOpenWarning();
    }

    void onStorageBoxDoorOpenStatusChanged(ArgumentsStorageBoxDoorOpenStatusChanged args) {
        DDebug() << "mInput_StorageBoxDoorOpenStatus " << static_cast<HUInt64>(args.mInput_StorageBoxDoorOpenStatus);
        mInput_StorageBoxDoorOpenStatus = args.mInput_StorageBoxDoorOpenStatus;
        updateEventStorageBoxDoorOpen();
    }

    void onDriverPositionChanged(const ArgumentsDriverPositionChanged& args) {
        DDebug() << "Input_DriverPositionTypeStatus " << static_cast<HUInt64>(args.mInput_DriverPositionTypeStatus);
        mInput_DriverPositionTypeStatus = args.mInput_DriverPositionTypeStatus;
        updateInfo();
    }

    void onDisplaySpeedValueKPHRangeChanged(const ArgumentsDisplaySpeedValueKPHRangeChanged& args) {
        DDebug() << "DisplaySpeedValueKPHRange " << static_cast<HUInt64>(args.mDisplaySpeedValueKPHRangeStat);
        mDisplaySpeedValueKPHRange = args.mDisplaySpeedValueKPHRangeStat;
        updateEventOpenWarning();
        updateEventStorageBoxDoorOpen();
    }

    void onElecHoodLatchErrorStatus(const ArgumentsElecHoodLatchErrorStatus& args) {
        mInput_ElecHoodLatchErrorStatus = args.mInput_ElecHoodLatchErrorStatus;
        updateEventFailureWarningHood();
    }

    void onElecHoodOpenGuideReqStatus(const ArgumentsElecHoodOpenGuideReqStatus& args) {
        mInput_ElecHoodOpenGuideReqStatus = args.mInput_ElecHoodOpenGuideReqStatus;
        updateEventGuideReq();
    }

    void onOdoMeterChanged(const ArgumentsOdoMeterChanged& args) {
        mOdometerValue = args.mOdometerValue;
        updateEventOpenWarning();
    }

    void onFactoryDiagnosisModeReqChanged(const ArgumentsFactoryDiagnosisModeReqChanged& args) {
        mInput_FactoryDiagnosisModeReq = args.mInput_FactoryDiagnosisModeReq;
        updateEventOpenWarning();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateInfo() {
        updatePrivate();
        updateConstant();
        updateEvent();
        updateTelltale();
    }

    void updatePrivate() {
        updatePrivateDoorOpenFrontStatus();
        updatePrivateDoorOpenRearStatus();
        updatePrivateHoodOpenStatus();
    }

    void updateConstant() {
        updateConstantFrontLeft();
        updateConstantFrontRight();
        updateConstantRearLeft();
        updateConstantRearRight();
        updateConstantTrunk();
        updateConstantHood();
    }

    void updateEvent() {
        updateEventOpenStatusDisplay();
        updateEventOpenWarning();
        updateEventFailureWarning();
        updateEventGuideReq();
        updateEventStorageBoxDoorOpen();
    }

    void updateTelltale() {
        // 5.1.5.1 Telltale
        if (mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::OPEN ||
            mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::OPEN ||
            mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::OPEN ||
            mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::OPEN ||
            mInput_TrunkOpenStatus == Input_TrunkOpenStatus::OPEN) {
            setSFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStat(SFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStat::OPTIONAL);
        } else {
            setSFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStat(SFCDoor_Trunk_Hood_OpenTelltaleDoorTrunkOpenStat::OFF);
        }
    }

    void updatePrivateDoorOpenFrontStatus() {
        // 4.1 연산로직 - 전석 도어 열림 상태 신호 추상화
        if (mInter_ConfigSBCM == Inter_ConfigSBCM::OFF) {
            if (mInput_DriverPositionTypeStatus == Input_DriverPositionTypeStatus::LHD) {
                mPrivate_DoorOpenFrontLeftStatus = static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenDriverSideStatus);
                mPrivate_DoorOpenFrontRightStatus = static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenAssistSideStatus);
            } else if (mInput_DriverPositionTypeStatus == Input_DriverPositionTypeStatus::RHD) {
                mPrivate_DoorOpenFrontLeftStatus = static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenAssistSideStatus);
                mPrivate_DoorOpenFrontRightStatus = static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenDriverSideStatus);
            } else {
                // Do nothing
            }
        } else if (mInter_ConfigSBCM == Inter_ConfigSBCM::ON) {
            if (mInput_DriverPositionTypeStatus == Input_DriverPositionTypeStatus::LHD) {
                mPrivate_DoorOpenFrontLeftStatus =
                    static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenDriverSideStatusFromSBCM);
                mPrivate_DoorOpenFrontRightStatus =
                    static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenAssistSideStatusFromSBCM);
            } else if (mInput_DriverPositionTypeStatus == Input_DriverPositionTypeStatus::RHD) {
                mPrivate_DoorOpenFrontLeftStatus =
                    static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenAssistSideStatusFromSBCM);
                mPrivate_DoorOpenFrontRightStatus =
                    static_cast<Private_DoorOpenFrontStatus>(mInput_DoorOpenDriverSideStatusFromSBCM);
            } else {
                // Do nothing
            }
        } else {
            // Do nothing
        }
    }

    void updatePrivateDoorOpenRearStatus() {
        // 4.2 연산로직 - 후석 도어 열림 상태 신호 추상화
        if (mInter_ConfigSBCM == Inter_ConfigSBCM::OFF) {
            mPrivate_DoorOpenRearLeftStatus = static_cast<Private_DoorOpenRearStatus>(mInput_DoorOpenRearLeftSideStatus);
            mPrivate_DoorOpenRearRightStatus = static_cast<Private_DoorOpenRearStatus>(mInput_DoorOpenRearRightSideStatus);
        } else if (mInter_ConfigSBCM == Inter_ConfigSBCM::ON) {
            mPrivate_DoorOpenRearLeftStatus = static_cast<Private_DoorOpenRearStatus>(mInput_DoorOpenRearLeftSideStatusFromSBCM);
            mPrivate_DoorOpenRearRightStatus =
                static_cast<Private_DoorOpenRearStatus>(mInput_DoorOpenRearRightSideStatusFromSBCM);
        } else {
            // Do nothing
        }
    }

    void updatePrivateHoodOpenStatus() {
        // 4.3 후드 열림 상태 신호 추상화
        if ((mInput_ElecHoodOptionStatus == Input_ElecHoodOptionStatus::NOT_APPLIED &&
             mInput_HoodOpenStatus == Input_HoodOpenStatus::OPEN) ||
            (mInput_ElecHoodOptionStatus == Input_ElecHoodOptionStatus::APPLIED &&
             mInput_ElecHoodOpenStatus == Input_ElecHoodOpenStatus::OPEN)) {
            mPrivate_HoodOpenStatus = Private_HoodOpenStatus::OPEN;
        } else {
            mPrivate_HoodOpenStatus = Private_HoodOpenStatus::CLOSE;
        }
    }

    void updateConstantFrontLeft() {
        ConstantFrontLeftStat frontLeftStatus = ConstantFrontLeftStat::CLOSE;
        switch (mPrivate_DoorOpenFrontLeftStatus) {
            case Private_DoorOpenFrontStatus::UNHANDLED_TIMEOUT:
            case Private_DoorOpenFrontStatus::CRC_ERROR:
                if (mIgnElapsed == IgnElapsed::ON_0ms) {
                    frontLeftStatus = ConstantFrontLeftStat::ERROR;
                }
                break;
            case Private_DoorOpenFrontStatus::OPEN:
                frontLeftStatus = ConstantFrontLeftStat::OPEN;
                break;
            default:
                frontLeftStatus = ConstantFrontLeftStat::CLOSE;
                break;
        }
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenFrontLeftStat(frontLeftStatus);
    }

    void updateConstantFrontRight() {
        ConstantFrontRightStat frontRightStatus = ConstantFrontRightStat::CLOSE;
        switch (mPrivate_DoorOpenFrontRightStatus) {
            case Private_DoorOpenFrontStatus::UNHANDLED_TIMEOUT:
            case Private_DoorOpenFrontStatus::CRC_ERROR:
                if (mIgnElapsed == IgnElapsed::ON_0ms) {
                    frontRightStatus = ConstantFrontRightStat::ERROR;
                }
                break;
            case Private_DoorOpenFrontStatus::OPEN:
                frontRightStatus = ConstantFrontRightStat::OPEN;
                break;
            default:
                frontRightStatus = ConstantFrontRightStat::CLOSE;
                break;
        }
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenFrontRightStat(frontRightStatus);
    }

    void updateConstantRearLeft() {
        ConstantRearLeftStat rearLeftStatus = ConstantRearLeftStat::CLOSE;
        switch (mPrivate_DoorOpenRearLeftStatus) {
            case Private_DoorOpenRearStatus::UNHANDLED_TIMEOUT:
            case Private_DoorOpenRearStatus::CRC_ERROR:
                if (mIgnElapsed == IgnElapsed::ON_0ms) {
                    rearLeftStatus = ConstantRearLeftStat::ERROR;
                }
                break;
            case Private_DoorOpenRearStatus::OPEN:
                rearLeftStatus = ConstantRearLeftStat::OPEN;
                break;
            default:
                break;
        }
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenRearLeftStat(rearLeftStatus);
    }

    void updateConstantRearRight() {
        ConstantRearRightStat rearRightStatus = ConstantRearRightStat::CLOSE;
        switch (mPrivate_DoorOpenRearRightStatus) {
            case Private_DoorOpenRearStatus::UNHANDLED_TIMEOUT:
            case Private_DoorOpenRearStatus::CRC_ERROR:
                if (mIgnElapsed == IgnElapsed::ON_0ms) {
                    rearRightStatus = ConstantRearRightStat::ERROR;
                }
                break;
            case Private_DoorOpenRearStatus::OPEN:
                rearRightStatus = ConstantRearRightStat::OPEN;
                break;
            default:
                break;
        }
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenRearRightStat(rearRightStatus);
    }

    void updateConstantTrunk() {
        ConstantTrunkStat trunkStatus = ConstantTrunkStat::CLOSE;
        switch (mInput_TrunkOpenStatus) {
            case Input_TrunkOpenStatus::UNHANDLED_TIMEOUT:
            case Input_TrunkOpenStatus::CRC_ERROR:
                if (mIgnElapsed == IgnElapsed::ON_0ms) {
                    trunkStatus = ConstantTrunkStat::ERROR;
                }
                break;
            case Input_TrunkOpenStatus::OPEN:
                trunkStatus = ConstantTrunkStat::OPEN;
                break;
            default:
                break;
        }
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusTrunkOpenStat(trunkStatus);
    }

    void updateConstantHood() {
        setSFCDoor_Trunk_Hood_OpenConstantOpenStatusHoodOpenStat(mPrivate_HoodOpenStatus);
    }

    void updateEventOpenStatusDisplay() {
        EventOpenStatusDisplayStat eventStat = EventOpenStatusDisplayStat::OFF;
        if (mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::OPEN ||
            mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::OPEN ||
            mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::OPEN ||
            mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::OPEN ||
            mInput_TrunkOpenStatus == Input_TrunkOpenStatus::OPEN || mPrivate_HoodOpenStatus == Private_HoodOpenStatus::OPEN) {
            eventStat = EventOpenStatusDisplayStat::ON;
        }

        // Door Open 팝업, ROA 팝업 표출 timing 이슈로 Door Open 팝업 표출 타이밍 100ms 지연 처리 (HKCLUPLAT-32861)
        // 임시 대책으로, 추후 App으로 timing 지연 코드 이관 필요함
        if (mIgnElapsed == IgnElapsed::OFF_0ms && eventStat == EventOpenStatusDisplayStat::ON) {
            if (mOneShotTimerOpenStatusDisplay.start() == false) {
                DWarning() << "[Door_Trunk_Hood_Open] mOneShotTimerOpenStatusDisplay Start Failed";
            }

        } else {
            mOneShotTimerOpenStatusDisplay.stop();
            setSFCDoor_Trunk_Hood_OpenEventOpenStatusDisplayStat(eventStat);
        }
    }

    void updateEventOpenWarning() {
        EventOpenWarningStat eventStat = EventOpenWarningStat::OFF;
        std::string linkedSoundID;
        EventOpenWarningLinkedSoundType eventLinkedSoundType = EventOpenWarningLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        HUInt64 durantion = 0;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_7_10 ||
                 mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_OVER_10) &&
                (mPrivate_HoodOpenStatus == Private_HoodOpenStatus::OPEN)) {
                if (mInput_FactoryDiagnosisModeReq != Input_FactoryDiagnosisModeReq::ON ||
                    (mInput_FactoryDiagnosisModeReq == Input_FactoryDiagnosisModeReq::ON && mOdometerValue > kOdometerValue1)) {
                    eventStat = EventOpenWarningStat::ON;
                    linkedSoundID = "SND_PopUpWarn2";
                    eventLinkedSoundType = EventOpenWarningLinkedSoundType::INFINITE;
                } else if (mInput_FactoryDiagnosisModeReq == Input_FactoryDiagnosisModeReq::ON &&
                           mOdometerValue <= kOdometerValue1) {
                    eventStat = EventOpenWarningStat::ON;
                    linkedSoundID = "SND_PopUpWarn1";
                    eventLinkedSoundType = EventOpenWarningLinkedSoundType::REPEAT_COUNT;
                    repeatCount = kEventOpenWarnLinkedSoundRepeatCount1;
                } else {
                    // OFF
                }
            } else if ((mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_OVER_10 ||
                        mInput_RGWDoorOpenStatus == Input_RGWDoorOpenStatus::ON ||
                        mInput_RGWDoorOpenStatusFromSAC == Input_RGWDoorOpenStatusFromSAC::ON) &&
                       (mPrivate_HoodOpenStatus != Private_HoodOpenStatus::OPEN) &&
                       (mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::OPEN ||
                        mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::OPEN ||
                        mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::OPEN ||
                        mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::OPEN ||
                        mInput_TrunkOpenStatus == Input_TrunkOpenStatus::OPEN)) {
                eventStat = EventOpenWarningStat::ON;
                linkedSoundID = "SND_PopUpWarn2";
                eventLinkedSoundType = EventOpenWarningLinkedSoundType::DURATION;
                durantion = kEventOpenWarnLinkedSoundDuration10000;
            } else {
                // OFF
            }
        }
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningStat(eventStat);
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningLinkedSoundID(linkedSoundID);
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningLinkedSoundType(eventLinkedSoundType);
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningLinkedSoundRepeatCount(repeatCount);
        setSFCDoor_Trunk_Hood_OpenEventOpenWarningLinkedSoundDuration(durantion);
    }

    void updateEventStorageBoxDoorOpen() {
        std::string prevEventID;
        std::string eventID;
        std::string soundID;
        EventStorageBoxDoorOpenLinkedSoundType soundType = EventStorageBoxDoorOpenLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        HUInt64 duration = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mInput_StorageBoxDoorOpenStatus == Input_StorageBoxDoorOpenStatus::OPEN) {
            if (mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_0_7 ||
                mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_7_10) {
                eventID = "E00109";
                soundID = "SND_PopUpInform2";
                soundType = EventStorageBoxDoorOpenLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
                duration = 0;
            } else if (mDisplaySpeedValueKPHRange == DisplaySpeedValueKPHRange::SPEED_OVER_10) {
                eventID = "E00110";
                soundID = "SND_PopUpWarn1";
                soundType = EventStorageBoxDoorOpenLinkedSoundType::DURATION;
                repeatCount = 0;
                duration = 10000;
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.Door_Trunk_Hood_Open.Event.StorageBoxDoorOpen.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenStat(EventStorageBoxDoorOpenStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenID(eventID);
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenLinkedSoundID(soundID);
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenLinkedSoundType(soundType);
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenLinkedSoundRepeatCount(repeatCount);
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenLinkedSoundDuration(duration);
            setSFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenStat(EventStorageBoxDoorOpenStat::ON);
        }
    }

    void updateEventFailureWarning() {
        updateEventFailureWarningDoor();
        updateEventFailureWarningTrunk();
        updateEventFailureWarningHood();
    }

    void updateEventFailureWarningDoor() {
        std::string prevEventID;
        std::string eventID;
        if (((mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::UNHANDLED_TIMEOUT ||
              mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::CRC_ERROR) ||
             (mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::UNHANDLED_TIMEOUT ||
              mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::CRC_ERROR) ||
             (mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::UNHANDLED_TIMEOUT ||
              mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::CRC_ERROR) ||
             (mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::UNHANDLED_TIMEOUT ||
              mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::CRC_ERROR)) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            eventID = "E00103";
        } else if ((mPrivate_DoorOpenFrontLeftStatus == Private_DoorOpenFrontStatus::CRC_ERROR ||
                    mPrivate_DoorOpenFrontRightStatus == Private_DoorOpenFrontStatus::CRC_ERROR ||
                    mPrivate_DoorOpenRearLeftStatus == Private_DoorOpenRearStatus::CRC_ERROR ||
                    mPrivate_DoorOpenRearRightStatus == Private_DoorOpenRearStatus::CRC_ERROR) &&
                   mIgnElapsed == IgnElapsed::OFF_0ms) {
            eventID = "E00105";
        } else {
            // Do nothing
        }

        GETCACHEDVALUE(SFC.Door_Trunk_Hood_Open.Event.FailureWarning_Door.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorStat(EventFailureWarningDoorStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorID(eventID);
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorStat(EventFailureWarningDoorStat::ON);
        }
    }

    void updateEventFailureWarningTrunk() {
        std::string prevEventID;
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms && (mInput_TrunkOpenStatus == Input_TrunkOpenStatus::UNHANDLED_TIMEOUT ||
                                                  mInput_TrunkOpenStatus == Input_TrunkOpenStatus::CRC_ERROR)) {
            eventID = "E00104";
        } else if (mIgnElapsed == IgnElapsed::OFF_0ms && mInput_TrunkOpenStatus == Input_TrunkOpenStatus::CRC_ERROR) {
            eventID = "E00106";
        } else {
            // no operation
        }
        GETCACHEDVALUE(SFC.Door_Trunk_Hood_Open.Event.FailureWarning_Trunk.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkStat(EventFailureWarningTrunkStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkID(eventID);
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkStat(EventFailureWarningTrunkStat::ON);
        }
    }

    void updateEventFailureWarningHood() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mInput_ElecHoodLatchErrorStatus == Input_ElecHoodLatchErrorStatus::ERROR) {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodStat(SFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodStat::ON);
        } else {
            setSFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodStat(SFCDoor_Trunk_Hood_OpenEventFailureWarning_HoodStat::OFF);
        }
    }

    void updateEventGuideReq() {
        if (mInput_ElecHoodOpenGuideReqStatus == Input_ElecHoodOpenGuideReqStatus::ON) {
            setSFCDoor_Trunk_Hood_OpenEventGuideReqStat(SFCDoor_Trunk_Hood_OpenEventGuideReqStat::ON);
        } else {
            setSFCDoor_Trunk_Hood_OpenEventGuideReqStat(SFCDoor_Trunk_Hood_OpenEventGuideReqStat::OFF);
        }
    }

    void onOneShotTimerOpenStatusDisplay() {
        mOneShotTimerOpenStatusDisplay.stop();
        setSFCDoor_Trunk_Hood_OpenEventOpenStatusDisplayStat(SFCDoor_Trunk_Hood_OpenEventOpenStatusDisplayStat::ON);
        flush();
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class Private_DoorOpenFrontStatus : HUInt64 {
        CLOSE = 0x0,
        OPEN = 0x1,
        CRC_ERROR = 0x2,
        UNHANDLED_TIMEOUT = 0xFFFFFFFF,
        MAX = 0x4
    };

    enum class Private_DoorOpenRearStatus : HUInt64 {
        CLOSE = 0x0,
        OPEN = 0x1,
        CRC_ERROR = 0x2,
        UNHANDLED_TIMEOUT = 0xFFFFFFFF,
        MAX = 0x4
    };

    using Private_HoodOpenStatus = SFCDoor_Trunk_Hood_OpenConstantOpenStatusHoodOpenStat;
    using Input_DoorOpenDriverSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenDriverSideStatus;
    using Input_DoorOpenAssistSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenAssistSideStatus;
    using Input_DoorOpenRearLeftSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenRearLeftSideStatus;
    using Input_DoorOpenRearRightSideStatus = ArgumentsDoorOpenChanged::Input_DoorOpenRearRightSideStatus;
    using Input_TrunkOpenStatus = ArgumentsDoorOpenChanged::Input_TrunkOpenStatus;
    using Input_HoodOpenStatus = ArgumentsDoorOpenChanged::Input_HoodOpenStatus;
    using Input_DoorOpenAssistSideStatusFromSBCM = ArgumentsDoorOpenChanged::Input_DoorOpenAssistSideStatusFromSBCM;
    using Input_DoorOpenDriverSideStatusFromSBCM = ArgumentsDoorOpenChanged::Input_DoorOpenDriverSideStatusFromSBCM;
    using Input_DoorOpenRearLeftSideStatusFromSBCM = ArgumentsDoorOpenChanged::Input_DoorOpenRearLeftSideStatusFromSBCM;
    using Input_DoorOpenRearRightSideStatusFromSBCM = ArgumentsDoorOpenChanged::Input_DoorOpenRearRightSideStatusFromSBCM;
    using Input_RGWDoorOpenStatus = ArgumentsRgwDoorOpenChanged::Input_RGWDoorOpenStatus;
    using Input_RGWDoorOpenStatusFromSAC = ArgumentsRgwDoorOpenChangedFromSAC::Input_RGWDoorOpenStatusFromSAC;
    using Input_StorageBoxDoorOpenStatus = ArgumentsStorageBoxDoorOpenStatusChanged::Input_StorageBoxDoorOpenStatus;
    using Input_ElecHoodOpenStatus = ArgumentsDoorOpenChanged::Input_ElecHoodOpenStatus;
    using Input_ElecHoodOptionStatus = ArgumentsDoorOpenChanged::Input_ElecHoodOptionStatus;
    using Input_DriverPositionTypeStatus = ArgumentsDriverPositionChanged::Input_DriverPositionTypeStatus;
    using DisplaySpeedValueKPHRange = ArgumentsDisplaySpeedValueKPHRangeChanged::DisplaySpeedValueKPHRangeStat;
    using Inter_ConfigSBCM = ArgumentsConfigChanged::Inter_ConfigSBCM;
    using Input_ElecHoodLatchErrorStatus = ArgumentsElecHoodLatchErrorStatus::Input_ElecHoodLatchErrorStatus;
    using Input_ElecHoodOpenGuideReqStatus = ArgumentsElecHoodOpenGuideReqStatus::Input_ElecHoodOpenGuideReqStatus;
    using Input_FactoryDiagnosisModeReq = ArgumentsFactoryDiagnosisModeReqChanged::Input_FactoryDiagnosisModeReq;

    // SFC - Constant
    using ConstantFrontLeftStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenFrontLeftStat;
    using ConstantFrontRightStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenFrontRightStat;
    using ConstantRearLeftStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenRearLeftStat;
    using ConstantRearRightStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusDoorOpenRearRightStat;
    using ConstantTrunkStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusTrunkOpenStat;
    using ConstantHoodStat = SFCDoor_Trunk_Hood_OpenConstantOpenStatusHoodOpenStat;

    // SFC - Event
    using EventOpenStatusDisplayStat = SFCDoor_Trunk_Hood_OpenEventOpenStatusDisplayStat;
    using EventOpenWarningStat = SFCDoor_Trunk_Hood_OpenEventOpenWarningStat;
    using EventOpenWarningLinkedSoundType = SFCDoor_Trunk_Hood_OpenEventOpenWarningLinkedSoundType;
    using EventFailureWarningDoorStat = SFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorStat;
    using EventFailureWarningDoorLinkedSoundType = SFCDoor_Trunk_Hood_OpenEventFailureWarning_DoorLinkedSoundType;
    using EventFailureWarningTrunkStat = SFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkStat;
    using EventFailureWarningTrunkLinkedSoundType = SFCDoor_Trunk_Hood_OpenEventFailureWarning_TrunkLinkedSoundType;
    using EventStorageBoxDoorOpenStat = SFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenStat;
    using EventStorageBoxDoorOpenLinkedSoundType = SFCDoor_Trunk_Hood_OpenEventStorageBoxDoorOpenLinkedSoundType;

    static constexpr HUInt64 kEventOpenWarnLinkedSoundDuration10000 = 10000;
    static constexpr HUInt64 kEventOpenWarnLinkedSoundRepeatCount1 = 1;
    static constexpr HUInt64 kEventFailureWarnDoorLinkedSoundRepeatCount1 = 1;
    static constexpr HUInt64 kEventFailureWarnTrunkLinkedSoundRepeatCount1 = 1;
    static constexpr HUInt64 kOdometerValue1 = 1;

    // Member - Variable
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    Inter_ConfigSBCM mInter_ConfigSBCM = Inter_ConfigSBCM::OFF;
    Input_DoorOpenDriverSideStatus mInput_DoorOpenDriverSideStatus = Input_DoorOpenDriverSideStatus::CLOSE;
    Input_DoorOpenAssistSideStatus mInput_DoorOpenAssistSideStatus = Input_DoorOpenAssistSideStatus::CLOSE;
    Input_DoorOpenRearLeftSideStatus mInput_DoorOpenRearLeftSideStatus = Input_DoorOpenRearLeftSideStatus::CLOSE;
    Input_DoorOpenRearRightSideStatus mInput_DoorOpenRearRightSideStatus = Input_DoorOpenRearRightSideStatus::CLOSE;
    Input_TrunkOpenStatus mInput_TrunkOpenStatus = Input_TrunkOpenStatus::CLOSE;
    Input_HoodOpenStatus mInput_HoodOpenStatus = Input_HoodOpenStatus::CLOSE;
    Input_DoorOpenAssistSideStatusFromSBCM mInput_DoorOpenAssistSideStatusFromSBCM =
        Input_DoorOpenAssistSideStatusFromSBCM::CLOSE;
    Input_DoorOpenDriverSideStatusFromSBCM mInput_DoorOpenDriverSideStatusFromSBCM =
        Input_DoorOpenDriverSideStatusFromSBCM::CLOSE;
    Input_DoorOpenRearLeftSideStatusFromSBCM mInput_DoorOpenRearLeftSideStatusFromSBCM =
        Input_DoorOpenRearLeftSideStatusFromSBCM::CLOSE;
    Input_DoorOpenRearRightSideStatusFromSBCM mInput_DoorOpenRearRightSideStatusFromSBCM =
        Input_DoorOpenRearRightSideStatusFromSBCM::CLOSE;
    Input_RGWDoorOpenStatus mInput_RGWDoorOpenStatus = Input_RGWDoorOpenStatus::OFF;
    Input_RGWDoorOpenStatusFromSAC mInput_RGWDoorOpenStatusFromSAC = Input_RGWDoorOpenStatusFromSAC::OFF;
    Input_StorageBoxDoorOpenStatus mInput_StorageBoxDoorOpenStatus = Input_StorageBoxDoorOpenStatus::CLOSE;
    Input_ElecHoodOpenStatus mInput_ElecHoodOpenStatus = Input_ElecHoodOpenStatus::CLOSE;
    Input_ElecHoodOptionStatus mInput_ElecHoodOptionStatus = Input_ElecHoodOptionStatus::NOT_APPLIED;

    Private_DoorOpenFrontStatus mPrivate_DoorOpenFrontLeftStatus = Private_DoorOpenFrontStatus::CLOSE;
    Private_DoorOpenFrontStatus mPrivate_DoorOpenFrontRightStatus = Private_DoorOpenFrontStatus::CLOSE;
    Private_DoorOpenRearStatus mPrivate_DoorOpenRearLeftStatus = Private_DoorOpenRearStatus::CLOSE;
    Private_DoorOpenRearStatus mPrivate_DoorOpenRearRightStatus = Private_DoorOpenRearStatus::CLOSE;
    Private_HoodOpenStatus mPrivate_HoodOpenStatus = Private_HoodOpenStatus::CLOSE;

    Input_DriverPositionTypeStatus mInput_DriverPositionTypeStatus = Input_DriverPositionTypeStatus::LHD;
    DisplaySpeedValueKPHRange mDisplaySpeedValueKPHRange = DisplaySpeedValueKPHRange::NONE;

    Input_ElecHoodLatchErrorStatus mInput_ElecHoodLatchErrorStatus = Input_ElecHoodLatchErrorStatus::NORMAL;
    Input_ElecHoodOpenGuideReqStatus mInput_ElecHoodOpenGuideReqStatus = Input_ElecHoodOpenGuideReqStatus::OFF;
    HUInt64 mOdometerValue = 0;
    Input_FactoryDiagnosisModeReq mInput_FactoryDiagnosisModeReq = Input_FactoryDiagnosisModeReq::OFF;

    ssfs::SFCTimer<Door_Trunk_Hood_Open> mOneShotTimerOpenStatusDisplay;
    static constexpr uint32_t kTimerOpenStatusInterval100ms = 100u;
};

}  // namespace ccos

#endif  // SFSS_Door_Trunk_Hood_Open_H
