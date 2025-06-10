/**
 * @file Drive_Mode.hpp
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
#ifndef SFSS_Drive_Mode_H
#define SFSS_Drive_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "Drive_ModeBase.hpp"
#include "Drive_ModeTypes.hpp"  // Note(andre) : using for enumeration & define chain class
#include <functional>
#include <bitset>

namespace ccos {

// SFC Version : 56.0.2
// Reference : [CD020] Drive Mode. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Drive_Mode : public Drive_ModeBase {
public:
    Drive_Mode() = default;
    ~Drive_Mode() override = default;
    Drive_Mode(const Drive_Mode& other) = delete;
    Drive_Mode(Drive_Mode&& other) noexcept = delete;
    Drive_Mode& operator=(const Drive_Mode& other) = delete;
    Drive_Mode& operator=(Drive_Mode&& other) noexcept = delete;

    void timerCallbackInter_TerrainModeSelectStatus() {
        mInter_TerrainModeSelectStatus.setNeedFlushed(true);
        processChain(false);
    }

    void onInitialize() override {
        mInter_TerrainModeSelectStatus.setCallback(std::bind(&Drive_Mode::timerCallbackInter_TerrainModeSelectStatus, this));
        setSFCDrive_ModeEventTerrainModeExitNotificationLinkedSoundID("SND_PopUpWarn1");
        setSFCDrive_ModeEventTerrainModeExitNotificationLinkedSoundType(
            Drive_Mode::SFCDrive_ModeEventTerrainModeExitNotificationLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventTerrainModeExitNotificationLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventTerrainModeExitNotificationStat(Drive_Mode::SFCDrive_ModeEventTerrainModeExitNotificationStat::OFF);

        setSFCDrive_ModeEventReconfirmNCustomModeLinkedSoundID("SND_PopUpInform1");
        setSFCDrive_ModeEventReconfirmNCustomModeLinkedSoundType(
            Drive_Mode::SFCDrive_ModeEventReconfirmNCustomModeLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventReconfirmNCustomModeLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventReconfirmNCustomModeStat(SFCDrive_ModeEventReconfirmNCustomModeStat::OFF);

        setSFCDrive_ModeEventModeSwitchingImpossibleLinkedSoundID("SND_PopUpWarn1");
        setSFCDrive_ModeEventModeSwitchingImpossibleLinkedSoundType(
            Drive_Mode::SFCDrive_ModeEventModeSwitchingImpossibleLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventModeSwitchingImpossibleLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventModeSwitchingImpossibleStat(SFCDrive_ModeEventModeSwitchingImpossibleStat::OFF);

        setSFCDrive_ModeEventReconfirmGT_MYModeLinkedSoundID("SND_PopUpInform1");
        setSFCDrive_ModeEventReconfirmGT_MYModeLinkedSoundType(
            Drive_Mode::SFCDrive_ModeEventReconfirmGT_MYModeLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventReconfirmGT_MYModeLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventReconfirmGT_MYModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmGT_MYModeStat::OFF);

        setSFCDrive_ModeEventAwd2hModeNotificationLinkedSoundID("SND_PopUpWarn1");
        setSFCDrive_ModeEventAwd2hModeNotificationLinkedSoundType(
            SFCDrive_ModeEventAwd2hModeNotificationLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventAwd2hModeNotificationLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventAwd2hModeNotificationStat(SFCDrive_ModeEventAwd2hModeNotificationStat::OFF);

        setSFCDrive_ModeEventUnableToSwitchModeByTowModeLinkedSoundID("SND_PopUpWarn1");
        setSFCDrive_ModeEventUnableToSwitchModeByTowModeLinkedSoundType(
            SFCDrive_ModeEventUnableToSwitchModeByTowModeLinkedSoundType::REPEAT_COUNT);
        setSFCDrive_ModeEventUnableToSwitchModeByTowModeLinkedSoundRepeatCount(1);
        setSFCDrive_ModeEventUnableToSwitchModeByTowModeStat(SFCDrive_ModeEventUnableToSwitchModeByTowModeStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == false) {
            return;
        }

        onIgnChanged(true);
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == false || mIgnStates != true) {
            return;
        }
        onIgnChanged(false);
        memoryDriveModeSelectStatus();

        std::string prevEventID;

        GETCACHEDVALUE(SFC.Drive_Mode.Event.DrivingModeChange.ID, prevEventID);
        if (prevEventID.empty() == false) {
            setSFCDrive_ModeEventDrivingModeChangeStat(Drive_Mode::SFCDrive_ModeEventDrivingModeChangeStat::OFF);
        }

        GETCACHEDVALUE(SFC.Drive_Mode.Event.TerrainModeExitNotification.ID, prevEventID);
        if (prevEventID.empty() == false) {
            setSFCDrive_ModeEventTerrainModeExitNotificationStat(
                Drive_Mode::SFCDrive_ModeEventTerrainModeExitNotificationStat::OFF);
        }

        GETCACHEDVALUE(SFC.Drive_Mode.Event.ReconfirmNCustomMode.ID, prevEventID);
        if (prevEventID.empty() == false) {
            setSFCDrive_ModeEventReconfirmNCustomModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmNCustomModeStat::OFF);
        }

        GETCACHEDVALUE(SFC.Drive_Mode.Event.ModeSwitchingImpossible.ID, prevEventID);
        if (prevEventID.empty() == false) {
            setSFCDrive_ModeEventModeSwitchingImpossibleStat(Drive_Mode::SFCDrive_ModeEventModeSwitchingImpossibleStat::OFF);
        }
    }

    void onDriveModeChanged(const ArgumentsDriveModeChanged& args) {
        mInput_EscCustom1EnableStatus = args.mInput_EscCustom1EnableStatus;
        mInput_EscCustom2EnableStatus = args.mInput_EscCustom2EnableStatus;
        mInput_GearSelectStatusFromTCU = args.mInput_GearSelectStatusFromTCU;
        mInput_SmartDriveModeStatus = args.mInput_SmartDriveModeStatus;
        mInput_TerrainModeErrorStatus = args.mInput_TerrainModeErrorStatus;

        processChain(false);
    }

    void onSystemConfigChanged(const ArgumentsSystemConfigChanged& args) {
        mInter_ConfigCustomMyDrive = args.mInter_ConfigCustomMyDrive;
        mInter_ConfigCustomSnow = args.mInter_ConfigCustomSnow;
        mInter_ConfigDriveModeDefault = args.mInter_ConfigDriveModeDefault;
        mInter_ConfigHighPerformance = args.mInter_ConfigHighPerformance;
        mInter_ConfigTowModeLongKey = args.mInter_ConfigTowModeLongKey;
        mInter_ConfigOffRoad = args.mInter_ConfigOffRoad;
        mInter_ConfigCustomSmart = args.mInter_ConfigCustomSmart;
        mInter_ConfigTerrainAuto = args.mInter_ConfigTerrainAuto;
        mInter_ConfigTerrainMud = args.mInter_ConfigTerrainMud;
        mInter_ConfigTerrainSand = args.mInter_ConfigTerrainSand;
        mInter_ConfigTerrainSnow = args.mInter_ConfigTerrainSnow;
        mInter_ConfigTerrainDeepSnow = args.mInter_ConfigTerrainDeepSnow;
        mInter_ConfigTerrainRock = args.mInter_ConfigTerrainRock;
        mInter_ConfigTerrainDesert = args.mInter_ConfigTerrainDesert;
        mInter_ConfigDriveModeKeepEco = args.mInter_ConfigDriveModeKeepEco;
        mInter_ConfigDriveModeKeepSport = args.mInter_ConfigDriveModeKeepSport;
        mInter_ConfigDriveModeKeepNormal = args.mInter_ConfigDriveModeKeepNormal;

        processChain(false);
    }

    void onMcuInterChanged(const ArgumentsMcuInterChanged& args) {
        mInter_DrvmodeSwitchStatus = args.mInter_DrvmodeSwitchStatus;
        {
            mInter_NModeOnReq = args.mInter_NModeOnReq;
            // TODO(andre) : Need to acquire new specifications [EXNCP-290]. refer: Event Control Logic
            mInter_NCustom1AdminStatus = args.mInter_NCustom1AdminStatus;
            mInter_NCustom2AdminStatus = args.mInter_NCustom2AdminStatus;
        }
        mInter_NModeSwitchStatus = args.mInter_NModeSwitchStatus;
        mInter_DriveModeOnRoadSwitchStatus = args.mInter_DriveModeOnRoadSwitchStatus;
        mInter_DriveModeTerrainSwitchStatus = args.mInter_DriveModeTerrainSwitchStatus;

        processChain(false);
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAEMInitComplete, args.mInter_AEMInitializeComplete)) {
            if (mIsAEMInitComplete == true) {
                HUInt64 driveModeSelectStatus = 0;
                HResult result = getCachedValue("SFC.Extension.Memory.Inter_DriveModeSelectStatus", driveModeSelectStatus);
                if (result == HResult::OK) {
                    // This will flushed on follwoing IGN_ON
                    mInter_DriveModeSelectStatus.setValue(
                        static_cast<ESFCDrive_ModeInter_DriveModeSelectStatus>(driveModeSelectStatus));
                    // Don't trigger change event
                    mEventDrivingModeChangeHandler.mPrevInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;

                    // Send initial values. (5.2.4.1) Sequence of AEM-->mignOn or IgnOn-->AEM is not determined :( ...
                    // EXNCP-33364: Drive_Mode(ECO) & Screen_Control_Logic Theme(Eco) 모드 부팅 시 Normal 모드 출력 이슈 수정
                    mInter_DriveModeSelectStatus.setNeedFlushed(true);
                    mInter_TerrainModeSelectStatus.setNeedFlushed(true);
                    mInter_NModeSelectStatus.setNeedFlushed(true);

                    // NOTE: 저장된 Inter_DriveModeSelectStatus 값이 있다면, 해당 값으로 기본 모드 리스트 초기화
                    mInter_DriveModeSelectStatus.mPrivate_BasicModeList.setCurrentMode(
                        static_cast<ESFCDrive_ModeInter_DriveModeSelectStatus>(driveModeSelectStatus));
                    _DInfo() << __FUNCTION__ << " Inter_DriveModeSelectStatus: " << driveModeSelectStatus;
                } else {
                    _DInfo() << __FUNCTION__ << " Inter_DriveModeSelectStatus: NORMAL (default)";
                }

                processChain(false);
            }
        }
    }

    void onNModeSwitchChanged(const ArgumentsNModeSwitchChanged& args) {
        mInput_N1SwitchCustom1ModeSetStatus = args.mInput_N1SwitchCustom1ModeSetStatus;
        mInput_N1SwitchCustom2ModeSetStatus = args.mInput_N1SwitchCustom2ModeSetStatus;
        mInput_N1SwitchMainSetStatus = args.mInput_N1SwitchMainSetStatus;
        mInput_N1SwitchNModeSetStatus = args.mInput_N1SwitchNModeSetStatus;
        mInput_N1SwitchStatus = args.mInput_N1SwitchStatus;

        mInput_N2SwitchCustom1ModeSetStatus = args.mInput_N2SwitchCustom1ModeSetStatus;
        mInput_N2SwitchCustom2ModeSetStatus = args.mInput_N2SwitchCustom2ModeSetStatus;
        mInput_N2SwitchMainSetStatus = args.mInput_N2SwitchMainSetStatus;
        mInput_N2SwitchNModeSetStatus = args.mInput_N2SwitchNModeSetStatus;
        mInput_N2SwitchStatus = args.mInput_N2SwitchStatus;

        processChain(false);
    }

    void onRcsControllerChanged(const ArgumentsRcsControllerChanged& args) {
        mInput_RcsOptionStatus = args.mInput_RcsOptionStatus;
        mInput_RcsDriveModeOnOffStatus = args.mInput_RcsDriveModeOnOffStatus;
        mInput_RcsDriveModeStatus = args.mInput_RcsDriveModeStatus;
        mInput_RcsTerrainModeStatus = args.mInput_RcsTerrainModeStatus;
        mInput_RcsDriveModeRotateEndStatus = args.mInput_RcsDriveModeRotateEndStatus;

        processChain(false);
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInter_ConfigEcoMode = args.mInter_ConfigEcoMode;
        mInter_ConfigNormalMode = args.mInter_ConfigNormalMode;
        mInter_ConfigSportMode = args.mInter_ConfigSportMode;
        mInter_ConfigChauffeurMode = args.mInter_ConfigChauffeurMode;

        processChain(false);
    }

    void onDriveModeOptionConfigChanged(const ArgumentsDriveModeOptionConfigChanged& args) {
        mInter_ConfigDriveModeOption = args.mInter_ConfigDriveModeOption;

        processChain(false);
    }

    void onEndlessDialSwitchConfigChanged(const ArgumentsEndlessDialSwitchConfigChanged& args) {
        mInter_ConfigEndlessDialSwitch = args.mInter_ConfigEndlessDialSwitch;

        processChain(false);
    }

    void onGtModeChanged(const ArgumentsGtModeChanged& args) {
        mInput_GtSwitchStatus = args.mInput_GtSwitchStatus;
        mInter_GtMyAdminStatus = args.mInter_GtMyAdminStatus;

        processChain(false);
    }

    void onAwdModeStatChanged(const ArgumentsAwdModeStatChanged& args) {
        mInput_AwdMode2HStatus = args.mInput_AwdMode2HStatus;

        processChain(false);
    }

    void onAwdModeDriveModeControlStatusChanged(const ArgumentsAwdModeDriveModeControlStatusChanged& args) {
        mInput_AwdModeDriveModeControlStatus = args.mInput_AwdModeDriveModeControlStatus;

        processChain(false);
    }

    void onTowModeStatusChanged(const ArgumentsTowModeStatusChanged& args) {
        mIMG_TelltaleTowMode_stat = args.mTowModeStat;

        processChain(false);
    }

    void onDriveModeChangeReqFromAVNTChanged(const ArgumentsDriveModeChangeReqFromAVNTChanged& args) {
        mInter_DriveModeChangeReqFromAVNT = args.mInter_DriveModeChangeReqFromAVNT;

        processChain(false);
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.2.5 오프로드 모드 리스트 타입
    // NOTE: Input_AwdModeDriveModeControlStatus 시그널에서 특정 bit의 true/false 상태를 return 해주는 함수.
    inline bool checkBitValue(const HUInt64& awdModeDriveModeControlStatusNumber, const HUInt64& bitIndex) const {
        return static_cast<bool>(std::bitset<9>(awdModeDriveModeControlStatusNumber)[bitIndex]);
    }

    void memoryDriveModeSelectStatus() {
        // 5.2.3 동작 조건 > 5.2.3.1 동작 옵션
        if (mIsAEMInitComplete == true && mInter_ConfigDriveModeOption == EInter_ConfigDriveModeOption::ON) {
            updateValue("SFC.Extension.Memory.Inter_DriveModeSelectStatus",
                        static_cast<HUInt64>(mInter_DriveModeSelectStatus.selfValue()));
        }
    }

    void onIgnChanged(const bool& ign) {
        if (mIgnStates != ign) {
            mIgnStates = ign;

            mIMG_DriveModePopupListNormal_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListEco_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListSport_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListChauffeur_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListMyDrive_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListSmart_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListSnow_stat.mIgnStatus = ign;
            mIMG_DriveModeTowLongkeyQue_stat.mIgnStatus = ign;
            mIMG_DriveModePopupListType_stat.mIgnStatus = ign;

            mDrive_ModeTelltaleDMSStat.mIgnStatus = ign;
            mDrive_ModeTelltaleDimmedStat.mIgnStatus = ign;
            mIMG_DriveModeSelect_stat.mIgnStatus = ign;
            mIMG_NModeSelect_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListSnow_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListMud_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListSand_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListAuto_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListDeepSnow_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListRock_stat.mIgnStatus = ign;
            mIMG_TerrainModePopupListDesert_stat.mIgnStatus = ign;

            mIMG_N1ModePopupListN_stat.mIgnStatus = ign;
            mIMG_N1ModePopupListCustom1_stat.mIgnStatus = ign;
            mIMG_N1ModePopupListCustom2_stat.mIgnStatus = ign;
            mIMG_N2ModePopupListN_stat.mIgnStatus = ign;
            mIMG_N2ModePopupListCustom1_stat.mIgnStatus = ign;
            mIMG_N2ModePopupListCustom2_stat.mIgnStatus = ign;

            mIMG_TerrainModeSelect_stat.mIgnStatus = ign;
            mIMG_GtModeSelect_stat.mIgnStatus = ign;

            mInter_DriveModeSelectStatus.mIgnStatus = ign;
            mInter_NModeSelectStatus.mIgnStatus = ign;
            mInter_TerrainModeSelectStatus.mIgnStatus = ign;
            mInter_GtModeSelectStatus.mIgnStatus = ign;
            mInter_DriveModeChangeResultStatus.mIgnStatus = ign;
            mEventDrivingModeChangeHandler.mIgnStatus = ign;
            mEventTerrainModeExitNotiHandler.mIgnStatus = ign;
            mEventReconfirmEnterNCustomHandler.mIgnStatus = ign;
            mEventReconfirmEnterGT_MYHandler.mIgnStatus = ign;
            mEventModeSwitchingImpossibleIndiHandler.mIgnStatus = ign;
            mEventAwd2hModeNotificationHandler.mIgnStatus = ign;
            mEventUnableToSwitchModeByTowModeHandler.mIgnStatus = ign;

            // Send initial values. (5.2.4.1) Sequence of AEM-->mignOn or IgnOn-->AEM is not determined :( ...
            if (mIsAEMInitComplete == true) {
                mInter_DriveModeSelectStatus.setNeedFlushed(true);
                mInter_TerrainModeSelectStatus.setNeedFlushed(true);
                mInter_NModeSelectStatus.setNeedFlushed(true);
            }

            processChain(ign);
        }
    }

    void processChain(const bool& forceUpdate) {
        if (mInter_ConfigDriveModeOption == EInter_ConfigDriveModeOption::OFF) {
            return;
        }

        // XXX(andre) : DO NOT RE-ORDER CODE BELOW
        mPrivate_DriveModeSwStatus.mInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mPrivate_DriveModeSwStatus.renew();

        // Private_BasicModeSwitchableStatus should use pre-updated values.
        mPrivate_BasicModeSwitchableStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mPrivate_BasicModeSwitchableStatus.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mPrivate_BasicModeSwitchableStatus.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mPrivate_BasicModeSwitchableStatus.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mPrivate_BasicModeSwitchableStatus.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mPrivate_BasicModeSwitchableStatus.renew();

        mPrivate_BasicModeForSwKeySwitchableStatus.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mPrivate_BasicModeForSwKeySwitchableStatus.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mPrivate_BasicModeForSwKeySwitchableStatus.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mPrivate_BasicModeForSwKeySwitchableStatus.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mPrivate_BasicModeForSwKeySwitchableStatus.renew();

        mInter_GtModeSelectStatus.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mInter_GtModeSelectStatus.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mInter_GtModeSelectStatus.mInput_GtSwitchStatus = mInput_GtSwitchStatus;
        mInter_GtModeSelectStatus.mInput_EscCustom1EnableStatus = mInput_EscCustom1EnableStatus;
        mInter_GtModeSelectStatus.mInter_GtMyAdminStatus = mInter_GtMyAdminStatus;
        mInter_GtModeSelectStatus.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mInter_GtModeSelectStatus.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mInter_GtModeSelectStatus.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        mInter_GtModeSelectStatus.renew();

        mInter_DriveModeChangeResultStatus.mIMG_DriveModePopupListEco_stat = mIMG_DriveModePopupListEco_stat;
        mInter_DriveModeChangeResultStatus.mIMG_DriveModePopupListNormal_stat = mIMG_DriveModePopupListNormal_stat;
        mInter_DriveModeChangeResultStatus.mIMG_DriveModePopupListSport_stat = mIMG_DriveModePopupListSport_stat;
        mInter_DriveModeChangeResultStatus.mPrivate_BasicModeForSwKeySwitchableStatus =
            mPrivate_BasicModeForSwKeySwitchableStatus;
        mInter_DriveModeChangeResultStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_DriveModeChangeResultStatus.renew();

        // Re-confir of enterance of N_CUSTOM* should use pre-updated Inter_NModeSelectStusts.
        mEventReconfirmEnterNCustomHandler.mInter_NModeSelectStatus = mInter_NModeSelectStatus;

        mInter_NModeSelectStatus.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mInter_NModeSelectStatus.mInput_N1SwitchMainSetStatus = mInput_N1SwitchMainSetStatus;
        mInter_NModeSelectStatus.mInput_N1SwitchNModeSetStatus = mInput_N1SwitchNModeSetStatus;
        mInter_NModeSelectStatus.mInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mInter_NModeSelectStatus.mInput_N2SwitchMainSetStatus = mInput_N2SwitchMainSetStatus;
        mInter_NModeSelectStatus.mInput_N2SwitchNModeSetStatus = mInput_N2SwitchNModeSetStatus;
        mInter_NModeSelectStatus.mInput_N2SwitchStatus = mInput_N2SwitchStatus;
        mInter_NModeSelectStatus.mInput_N1SwitchCustom2ModeSetStatus = mInput_N1SwitchCustom2ModeSetStatus;
        mInter_NModeSelectStatus.mInput_N2SwitchCustom2ModeSetStatus = mInput_N2SwitchCustom2ModeSetStatus;
        mInter_NModeSelectStatus.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mInter_NModeSelectStatus.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mInter_NModeSelectStatus.mInput_N1SwitchCustom1ModeSetStatus = mInput_N1SwitchCustom1ModeSetStatus;
        mInter_NModeSelectStatus.mInput_EscCustom1EnableStatus = mInput_EscCustom1EnableStatus;
        mInter_NModeSelectStatus.mInput_N2SwitchCustom1ModeSetStatus = mInput_N2SwitchCustom1ModeSetStatus;
        mInter_NModeSelectStatus.mInter_NCustom1AdminStatus = mInter_NCustom1AdminStatus;
        mInter_NModeSelectStatus.mInput_EscCustom2EnableStatus = mInput_EscCustom2EnableStatus;
        mInter_NModeSelectStatus.mInter_NCustom2AdminStatus = mInter_NCustom2AdminStatus;
        mInter_NModeSelectStatus.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mInter_NModeSelectStatus.mInter_NModeOnReq = mInter_NModeOnReq;
        mInter_NModeSelectStatus.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        mInter_NModeSelectStatus.renew();

        mInter_TerrainModeSelectStatus.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mInter_TerrainModeSelectStatus.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mInter_TerrainModeSelectStatus.mInter_ConfigEndlessDialSwitch = mInter_ConfigEndlessDialSwitch;
        mInter_TerrainModeSelectStatus.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mInter_TerrainModeSelectStatus.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mInter_TerrainModeSelectStatus.mInput_AwdMode2HStatus = mInput_AwdMode2HStatus;
        mInter_TerrainModeSelectStatus.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListAuto_stat = mIMG_TerrainModePopupListAuto_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListSnow_stat = mIMG_TerrainModePopupListSnow_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListDeepSnow_stat = mIMG_TerrainModePopupListDeepSnow_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListMud_stat = mIMG_TerrainModePopupListMud_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListSand_stat = mIMG_TerrainModePopupListSand_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListRock_stat = mIMG_TerrainModePopupListRock_stat;
        mInter_TerrainModeSelectStatus.mIMG_TerrainModePopupListDesert_stat = mIMG_TerrainModePopupListDesert_stat;
        mInter_TerrainModeSelectStatus.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        mInter_TerrainModeSelectStatus.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mInter_TerrainModeSelectStatus.mInput_RcsDriveModeOnOffStatus = mInput_RcsDriveModeOnOffStatus;
        mInter_TerrainModeSelectStatus.mInput_RcsTerrainModeStatus = mInput_RcsTerrainModeStatus;
        mInter_TerrainModeSelectStatus.mInter_ConfigOffRoad = mInter_ConfigOffRoad;
        mInter_TerrainModeSelectStatus.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mInter_TerrainModeSelectStatus.mIMG_DriveModePopupListEco_stat = mIMG_DriveModePopupListEco_stat;
        mInter_TerrainModeSelectStatus.mIMG_DriveModePopupListNormal_stat = mIMG_DriveModePopupListNormal_stat;
        mInter_TerrainModeSelectStatus.mIMG_DriveModePopupListSport_stat = mIMG_DriveModePopupListSport_stat;
        mInter_TerrainModeSelectStatus.mPrivate_BasicModeForSwKeySwitchableStatus = mPrivate_BasicModeForSwKeySwitchableStatus;
        mInter_TerrainModeSelectStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_TerrainModeSelectStatus.renew();

        mInter_DriveModeSelectStatus.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mInter_DriveModeSelectStatus.mInter_ConfigTowModeLongKey = mInter_ConfigTowModeLongKey;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListEco_stat = mIMG_DriveModePopupListEco_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListNormal_stat = mIMG_DriveModePopupListNormal_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListSport_stat = mIMG_DriveModePopupListSport_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListChauffeur_stat = mIMG_DriveModePopupListChauffeur_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListMyDrive_stat = mIMG_DriveModePopupListMyDrive_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListSmart_stat = mIMG_DriveModePopupListSmart_stat;
        mInter_DriveModeSelectStatus.mIMG_DriveModePopupListSnow_stat = mIMG_DriveModePopupListSnow_stat;
        mInter_DriveModeSelectStatus.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mInter_DriveModeSelectStatus.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mInter_DriveModeSelectStatus.mInput_RcsDriveModeOnOffStatus = mInput_RcsDriveModeOnOffStatus;
        mInter_DriveModeSelectStatus.mInput_RcsDriveModeStatus = mInput_RcsDriveModeStatus;
        mInter_DriveModeSelectStatus.mInter_ConfigEndlessDialSwitch = mInter_ConfigEndlessDialSwitch;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSwitchableStatus = mPrivate_BasicModeSwitchableStatus;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeDefault = mInter_ConfigDriveModeDefault;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepEco = mInter_ConfigDriveModeKeepEco;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepNormal = mInter_ConfigDriveModeKeepNormal;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepSport = mInter_ConfigDriveModeKeepSport;
        mInter_DriveModeSelectStatus.mInput_RcsTerrainModeStatus = mInput_RcsTerrainModeStatus;
        mInter_DriveModeSelectStatus.mInter_ConfigOffRoad = mInter_ConfigOffRoad;
        mInter_DriveModeSelectStatus.mInter_ConfigEcoMode = mInter_ConfigEcoMode;
        mInter_DriveModeSelectStatus.mInter_ConfigNormalMode = mInter_ConfigNormalMode;
        mInter_DriveModeSelectStatus.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeForSwKeySwitchableStatus = mPrivate_BasicModeForSwKeySwitchableStatus;
        mInter_DriveModeSelectStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_DriveModeSelectStatus.renew();

        processSink(forceUpdate);
    }

    void processSink(const bool& forceUpdate) {
        bool hasToFlush = processSinkCalculation();
        if (hasToFlush == true) {
            flush(true);
        }
        hasToFlush = processSinkConstant() || hasToFlush;
        hasToFlush = processSinkTelltale() || hasToFlush;
        if (hasToFlush == true) {
            DDebug() << "flushed";
            flush();
        }
        // Note(csh) : On Condition 조건 만족시마다 Event는 재활성화한다 [EXNCP-20819]
        bool eventHasToFlush = processSinkEvent();
        if (eventHasToFlush == true) {
            DDebug() << "event forced flushed";
            flush(true);
        }
        // Note(andre) : external sink doesn't have to flush
        processSinkExternal(forceUpdate);
    }

    void processSinkExternal(const bool& forceUpdate) {
        // 5.2.4.6 Output_CustomDriveModeApplyStatus B+ 확인
        mOutput_CustomDriveModeApplyStatus.renew();

        // 5.2.4.8 Output_TerrainModeSwitchStatus
        mOutput_TerrainModeSwitchStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mOutput_TerrainModeSwitchStatus.renew();

        // 5.2.4.8 Output_TerrainModeMainSwitchStatus
        mOutput_TerrainModeMainSwitchStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mOutput_TerrainModeMainSwitchStatus.renew();

        // 5.2.4.8 Output_CustomDriveModeSetStatus
        mOutput_CustomDriveModeSetStatus.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mOutput_CustomDriveModeSetStatus.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mOutput_CustomDriveModeSetStatus.renew();

        // 5.2.4.6 Output_DriveModeSwitchStatus
        // power condition: B+ ON
        mOutput_DriveModeSwitchStatus.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mOutput_DriveModeSwitchStatus.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mOutput_DriveModeSwitchStatus.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mOutput_DriveModeSwitchStatus.renew();

        if (mOutput_CustomDriveModeApplyStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_CustomDriveModeApplyStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode.Output_CustomDriveModeApplyStatus,
                                                 static_cast<HUInt64>(mOutput_CustomDriveModeApplyStatus.selfValue()));
        }
        if (mOutput_TerrainModeSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_TerrainModeSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode.Output_TerrainModeSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_TerrainModeSwitchStatus.selfValue()));
        }
        if (mOutput_TerrainModeMainSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_TerrainModeMainSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode.Output_TerrainModeMainSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_TerrainModeMainSwitchStatus.selfValue()));
        }
        if (mOutput_CustomDriveModeSetStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_CustomDriveModeSetStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode.Output_CustomDriveModeSetStatus,
                                                 static_cast<HUInt64>(mOutput_CustomDriveModeSetStatus.selfValue()));
        }
        if (mOutput_DriveModeSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_DriveModeSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode.Output_DriveModeSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_DriveModeSwitchStatus.selfValue()));
        }

        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    bool processSinkCalculation() {
        bool hasToFlush = false;
        // 5.2.4.3
        if (mInter_DriveModeSelectStatus.isNeedFlushed() == true) {
            mInter_DriveModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeInter_DriveModeSelectStatus(mInter_DriveModeSelectStatus);
        }
        // 5.2.4.5
        if (mInter_TerrainModeSelectStatus.isNeedFlushed() == true) {
            mInter_TerrainModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeInter_TerrainModeSelectStatus(mInter_TerrainModeSelectStatus);
        }
        // 5.2.4.5
        if (mInter_NModeSelectStatus.isNeedFlushed() == true) {
            mInter_NModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeInter_NModeSelectStatus(mInter_NModeSelectStatus);
        }

        // 5.2.4.1
        if (mInter_GtModeSelectStatus.isNeedFlushed() == true) {
            mInter_GtModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeInter_GtModeSelectStatus(mInter_GtModeSelectStatus);
        }

        // 5.2.4.8
        if (mInter_DriveModeChangeResultStatus.isNeedFlushed() == true ||
            mInter_DriveModeChangeResultStatus.mIsForcedFlush == true) {
            mInter_DriveModeChangeResultStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeInter_DriveModeChangeResultStatus(mInter_DriveModeChangeResultStatus);
        }

        if (hasToFlush == true) {
            DDebug() << "[HIT] Calculation Logic";
        }
        return hasToFlush;
    }

    bool processSinkTelltale() {
        bool hasToFlush = false;

        mDrive_ModeTelltaleDMSStat.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mDrive_ModeTelltaleDMSStat.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mDrive_ModeTelltaleDMSStat.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mDrive_ModeTelltaleDMSStat.mInput_SmartDriveModeStatus = mInput_SmartDriveModeStatus;
        mDrive_ModeTelltaleDMSStat.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mDrive_ModeTelltaleDMSStat.renew();

        mDrive_ModeTelltaleDimmedStat.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mDrive_ModeTelltaleDimmedStat.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mDrive_ModeTelltaleDimmedStat.renew();

        if (mDrive_ModeTelltaleDMSStat.isNeedFlushed() == true) {
            mDrive_ModeTelltaleDMSStat.setNeedFlushed(false);
            hasToFlush = true;
            if (mDrive_ModeTelltaleDMSStat == SFCDrive_ModeTelltaleDrive_ModeDMSStatOptional::OFF) {
                setSFCDrive_ModeTelltaleDrive_ModeDMSStat(SFCDrive_ModeTelltaleDrive_ModeDMSStat::OFF);
                setSFCDrive_ModeTelltaleDrive_ModeDimmedStat(SFCDrive_ModeTelltaleDrive_ModeDimmedStat::OFF);
            } else {
                setSFCDrive_ModeTelltaleDrive_ModeDMSStat(SFCDrive_ModeTelltaleDrive_ModeDMSStat::OPTIONAL);
                setSFCDrive_ModeTelltaleDrive_ModeDimmedStat(SFCDrive_ModeTelltaleDrive_ModeDimmedStat::OPTIONAL);
                setSFCDrive_ModeTelltaleDrive_ModeDMSStatOptional(mDrive_ModeTelltaleDMSStat);
            }
        }
        if (mDrive_ModeTelltaleDimmedStat.isNeedFlushed() == true) {
            mDrive_ModeTelltaleDimmedStat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeTelltaleDrive_ModeDimmedStatOptional(mDrive_ModeTelltaleDimmedStat);
        }
        if (hasToFlush == true) {
            DDebug() << "[HIT] Telltale Logic";
        }
        return hasToFlush;
    }

    bool processSinkConstant() {
        bool hasToFlush = false;
        bool isDriveModeStatUpdate = false;
        bool isTerrainModeStatUpdate = false;
        // 5.2.5.2.1 드라이브 모드 팝업 타이틀
        mIMG_DriveModePopupTitle_stat.mInter_ConfigOffRoad = mInter_ConfigOffRoad;
        mIMG_DriveModePopupTitle_stat.renew();

        // 5.2.5.2.4 터레인 모드 리스트 타입
        mIMG_TerrainModePopupListSnow_stat.mInter_ConfigTerrainSnow = mInter_ConfigTerrainSnow;
        mIMG_TerrainModePopupListSnow_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeSnowBitIndex);
        mIMG_TerrainModePopupListSnow_stat.renew();

        mIMG_TerrainModePopupListMud_stat.mInter_ConfigTerrainMud = mInter_ConfigTerrainMud;
        mIMG_TerrainModePopupListMud_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeMudBitIndex);
        mIMG_TerrainModePopupListMud_stat.renew();

        mIMG_TerrainModePopupListSand_stat.mInter_ConfigTerrainSand = mInter_ConfigTerrainSand;
        mIMG_TerrainModePopupListSand_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeSandBitIndex);
        mIMG_TerrainModePopupListSand_stat.renew();

        mIMG_TerrainModePopupListAuto_stat.mInter_ConfigTerrainAuto = mInter_ConfigTerrainAuto;
        mIMG_TerrainModePopupListAuto_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeAutoBitIndex);
        mIMG_TerrainModePopupListAuto_stat.renew();

        mIMG_TerrainModePopupListDeepSnow_stat.mInter_ConfigTerrainDeepSnow = mInter_ConfigTerrainDeepSnow;
        mIMG_TerrainModePopupListDeepSnow_stat.renew();

        mIMG_TerrainModePopupListRock_stat.mInter_ConfigTerrainRock = mInter_ConfigTerrainRock;
        mIMG_TerrainModePopupListRock_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeRockBitIndex);
        mIMG_TerrainModePopupListRock_stat.renew();

        mIMG_TerrainModePopupListDesert_stat.mInter_ConfigTerrainDesert = mInter_ConfigTerrainDesert;
        mIMG_TerrainModePopupListDesert_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeDesertBitIndex);
        mIMG_TerrainModePopupListDesert_stat.renew();

        // 5.2.5.2.5 N1모드 리스트 타입
        mIMG_N1ModePopupListN_stat.mInput_N1SwitchNModeSetStatus = mInput_N1SwitchNModeSetStatus;
        mIMG_N1ModePopupListN_stat.renew();

        mIMG_N1ModePopupListCustom1_stat.mInput_N1SwitchCustom1ModeSetStatus = mInput_N1SwitchCustom1ModeSetStatus;
        mIMG_N1ModePopupListCustom1_stat.renew();

        mIMG_N1ModePopupListCustom2_stat.mInput_N1SwitchCustom2ModeSetStatus = mInput_N1SwitchCustom2ModeSetStatus;
        mIMG_N1ModePopupListCustom2_stat.renew();

        // 5.2.5.2.6 N2모드 리스트 타입
        mIMG_N2ModePopupListN_stat.mInput_N2SwitchNModeSetStatus = mInput_N2SwitchNModeSetStatus;
        mIMG_N2ModePopupListN_stat.renew();

        mIMG_N2ModePopupListCustom1_stat.mInput_N2SwitchCustom1ModeSetStatus = mInput_N2SwitchCustom1ModeSetStatus;
        mIMG_N2ModePopupListCustom1_stat.renew();

        mIMG_N2ModePopupListCustom2_stat.mInput_N2SwitchCustom2ModeSetStatus = mInput_N2SwitchCustom2ModeSetStatus;
        mIMG_N2ModePopupListCustom2_stat.renew();

        // 5.2.5.2.7 GT모드 리스트 타입
        mIMG_GtModePopupListGt_stat.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mIMG_GtModePopupListGt_stat.renew();

        mIMG_GtModePopupListMy_stat.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mIMG_GtModePopupListMy_stat.renew();

        // 5.2.5.2.8 드라이브 모드 선택 상태
        mIMG_DriveModeSelect_stat.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mIMG_DriveModeSelect_stat.renew();

        // 5.2.5.2.9 터레인 모드 선택 상태
        mIMG_TerrainModeSelect_stat.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mIMG_TerrainModeSelect_stat.renew();

        // 5.2.5.2.10 N모드 선택 상태
        mIMG_NModeSelect_stat.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mIMG_NModeSelect_stat.renew();

        // 5.2.5.2.11 GT모드 선택 상태
        mIMG_GtModeSelect_stat.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mIMG_GtModeSelect_stat.renew();

        // 5.2.5.2.2 드라이브 모드 리스트 항목별 표시 여부
        mIMG_DriveModePopupListNormal_stat.mInter_ConfigNormalMode = mInter_ConfigNormalMode;
        mIMG_DriveModePopupListNormal_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeNormalBitIndex);
        mIMG_DriveModePopupListNormal_stat.renew();
        mIMG_DriveModePopupListEco_stat.mInter_ConfigEcoMode = mInter_ConfigEcoMode;
        mIMG_DriveModePopupListEco_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeEcoBitIndex);
        mIMG_DriveModePopupListEco_stat.renew();
        mIMG_DriveModePopupListSport_stat.mInter_ConfigSportMode = mInter_ConfigSportMode;
        mIMG_DriveModePopupListSport_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeSportBitIndex);
        mIMG_DriveModePopupListSport_stat.renew();

        mIMG_DriveModePopupListChauffeur_stat.mInter_ConfigChauffeurMode = mInter_ConfigChauffeurMode;
        mIMG_DriveModePopupListChauffeur_stat.renew();

        mIMG_DriveModePopupListMyDrive_stat.mInter_ConfigCustomMyDrive = mInter_ConfigCustomMyDrive;
        mIMG_DriveModePopupListMyDrive_stat.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeMyDriveBitIndex);
        mIMG_DriveModePopupListMyDrive_stat.renew();

        mIMG_DriveModePopupListSmart_stat.mInter_ConfigCustomSmart = mInter_ConfigCustomSmart;
        mIMG_DriveModePopupListSmart_stat.renew();

        mIMG_DriveModePopupListSnow_stat.mInter_ConfigCustomSnow = mInter_ConfigCustomSnow;
        mIMG_DriveModePopupListSnow_stat.renew();

        // 5.2.3 드라이브 모드 하단 QUE 표시 여부
        mIMG_DriveModeTowLongkeyQue_stat.mInter_ConfigTowModeLongKey = mInter_ConfigTowModeLongKey;
        mIMG_DriveModeTowLongkeyQue_stat.renew();

        // 5.2.4 드라이브 모드 리스트 순서 타입
        mIMG_DriveModePopupListType_stat.mInter_ConfigEndlessDialSwitch = mInter_ConfigEndlessDialSwitch;
        mIMG_DriveModePopupListType_stat.renew();

        // 5.2.5.2.1
        if (mIMG_DriveModePopupTitle_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupTitle_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupTitle_stat(mIMG_DriveModePopupTitle_stat);
        }
        // 5.2.5.2.2
        if (mIMG_DriveModePopupListNormal_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListNormal_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat(mIMG_DriveModePopupListNormal_stat);
        }
        if (mIMG_DriveModePopupListEco_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListEco_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListEco_stat(mIMG_DriveModePopupListEco_stat);
        }
        if (mIMG_DriveModePopupListSport_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListSport_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListSport_stat(mIMG_DriveModePopupListSport_stat);
        }
        if (mIMG_DriveModePopupListChauffeur_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListChauffeur_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat(mIMG_DriveModePopupListChauffeur_stat);
        }
        if (mIMG_DriveModePopupListMyDrive_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListMyDrive_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat(mIMG_DriveModePopupListMyDrive_stat);
        }
        if (mIMG_DriveModePopupListSmart_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListSmart_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat(mIMG_DriveModePopupListSmart_stat);
        }
        if (mIMG_DriveModePopupListSnow_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListSnow_stat.setNeedFlushed(false);
            hasToFlush = true;
            isDriveModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat(mIMG_DriveModePopupListSnow_stat);
        }

        // 5.2.3 드라이브 모드 하단 QUE 표시 여부
        if (mIMG_DriveModeTowLongkeyQue_stat.isNeedFlushed() == true) {
            mIMG_DriveModeTowLongkeyQue_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_DriveModeTowLongkeyQue_stat(mIMG_DriveModeTowLongkeyQue_stat);
        }

        // 5.2.4.3 기본 모드 상태 판단 update
        if (isDriveModeStatUpdate == true) {
            processChain(false);
        }

        // 5.2.5.2.3
        if (mIMG_DriveModePopupListType_stat.isNeedFlushed() == true) {
            mIMG_DriveModePopupListType_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_DriveModePopupListType_stat(mIMG_DriveModePopupListType_stat);
        }
        // 5.2.5.2.4

        if (mIMG_TerrainModePopupListAuto_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListAuto_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat(mIMG_TerrainModePopupListAuto_stat);
        }

        if (mIMG_TerrainModePopupListSnow_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListSnow_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat(mIMG_TerrainModePopupListSnow_stat);
        }

        if (mIMG_TerrainModePopupListDeepSnow_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListDeepSnow_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat(mIMG_TerrainModePopupListDeepSnow_stat);
        }

        if (mIMG_TerrainModePopupListMud_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListMud_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat(mIMG_TerrainModePopupListMud_stat);
        }

        if (mIMG_TerrainModePopupListSand_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListSand_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat(mIMG_TerrainModePopupListSand_stat);
        }

        if (mIMG_TerrainModePopupListRock_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListRock_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat(mIMG_TerrainModePopupListRock_stat);
        }

        if (mIMG_TerrainModePopupListDesert_stat.isNeedFlushed() == true) {
            mIMG_TerrainModePopupListDesert_stat.setNeedFlushed(false);
            hasToFlush = true;
            isTerrainModeStatUpdate = true;
            setSFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat(mIMG_TerrainModePopupListDesert_stat);
        }

        // 5.2.4.4.2 오프로드 모드 적용 리스트 update
        if (isTerrainModeStatUpdate == true) {
            processChain(false);
        }

        // 5.2.5.2.5
        if (mIMG_N1ModePopupListN_stat.isNeedFlushed() == true) {
            mIMG_N1ModePopupListN_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N1ModePopupListN_stat(mIMG_N1ModePopupListN_stat);
        }
        if (mIMG_N1ModePopupListCustom1_stat.isNeedFlushed() == true) {
            mIMG_N1ModePopupListCustom1_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat(mIMG_N1ModePopupListCustom1_stat);
        }
        if (mIMG_N1ModePopupListCustom2_stat.isNeedFlushed() == true) {
            mIMG_N1ModePopupListCustom2_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat(mIMG_N1ModePopupListCustom2_stat);
        }
        // 5.2.5.2.6
        if (mIMG_N2ModePopupListN_stat.isNeedFlushed() == true) {
            mIMG_N2ModePopupListN_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N2ModePopupListN_stat(mIMG_N2ModePopupListN_stat);
        }
        if (mIMG_N2ModePopupListCustom1_stat.isNeedFlushed() == true) {
            mIMG_N2ModePopupListCustom1_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat(mIMG_N2ModePopupListCustom1_stat);
        }
        if (mIMG_N2ModePopupListCustom2_stat.isNeedFlushed() == true) {
            mIMG_N2ModePopupListCustom2_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat(mIMG_N2ModePopupListCustom2_stat);
        }

        // 5.2.5.2.7
        if (mIMG_GtModePopupListGt_stat.isNeedFlushed() == true) {
            mIMG_GtModePopupListGt_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_GtModePopupListGt_stat(mIMG_GtModePopupListGt_stat);
        }
        if (mIMG_GtModePopupListMy_stat.isNeedFlushed() == true) {
            mIMG_GtModePopupListMy_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_GtModePopupListMy_stat(mIMG_GtModePopupListMy_stat);
        }

        // 5.2.5.2.8
        if (mIMG_DriveModeSelect_stat.isNeedFlushed() == true) {
            mIMG_DriveModeSelect_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_DriveModeSelect_stat(mIMG_DriveModeSelect_stat);
        }
        // 5.2.5.2.9
        if (mIMG_TerrainModeSelect_stat.isNeedFlushed() == true) {
            mIMG_TerrainModeSelect_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_TerrainModeSelect_stat(mIMG_TerrainModeSelect_stat);
        }
        // 5.2.5.2.10
        if (mIMG_NModeSelect_stat.isNeedFlushed() == true) {
            mIMG_NModeSelect_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_NModeSelect_stat(mIMG_NModeSelect_stat);
        }

        // 5.2.5.2.11
        if (mIMG_GtModeSelect_stat.isNeedFlushed() == true) {
            mIMG_GtModeSelect_stat.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_ModeConstantIMG_GtModeSelect_stat(mIMG_GtModeSelect_stat);
        }
        if (hasToFlush == true) {
            DDebug() << "[HIT] Constant Logic";
        }
        return hasToFlush;
    }

    bool processSinkEvent() {
        bool hasToFlush = false;

        mEventDrivingModeChangeHandler.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mEventDrivingModeChangeHandler.mInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mEventDrivingModeChangeHandler.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mEventDrivingModeChangeHandler.mInter_NModeSwitchStatus = mInter_NModeSwitchStatus;
        mEventDrivingModeChangeHandler.mInter_ConfigHighPerformance = mInter_ConfigHighPerformance;
        mEventDrivingModeChangeHandler.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mEventDrivingModeChangeHandler.mInter_NModeOnReq = mInter_NModeOnReq;
        mEventDrivingModeChangeHandler.mInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mEventDrivingModeChangeHandler.mInput_N2SwitchStatus = mInput_N2SwitchStatus;
        mEventDrivingModeChangeHandler.mInput_RcsDriveModeRotateEndStatus = mInput_RcsDriveModeRotateEndStatus;
        mEventDrivingModeChangeHandler.mInput_GtSwitchStatus = mInput_GtSwitchStatus;
        mEventDrivingModeChangeHandler.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mEventDrivingModeChangeHandler.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mEventDrivingModeChangeHandler.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mEventDrivingModeChangeHandler.mIsSingleDriveModeEnabled = mInter_DriveModeSelectStatus.mIsSingleDriveModeEnabled;

        mEventModeSwitchingImpossibleIndiHandler.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mEventModeSwitchingImpossibleIndiHandler.mInput_N1SwitchMainSetStatus = mInput_N1SwitchMainSetStatus;
        mEventModeSwitchingImpossibleIndiHandler.mInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mEventModeSwitchingImpossibleIndiHandler.mInput_N2SwitchMainSetStatus = mInput_N2SwitchMainSetStatus;
        mEventModeSwitchingImpossibleIndiHandler.mInput_N2SwitchStatus = mInput_N2SwitchStatus;
        mEventModeSwitchingImpossibleIndiHandler.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mEventModeSwitchingImpossibleIndiHandler.mInter_NModeOnReq = mInter_NModeOnReq;
        mEventModeSwitchingImpossibleIndiHandler.mInput_GtSwitchStatus = mInput_GtSwitchStatus;
        mEventModeSwitchingImpossibleIndiHandler.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;

        mEventReconfirmEnterNCustomHandler.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        // mEventReconfirmEnterNCustomHandler.mInter_NModeSelectStatus should use pre-updated value.
        mEventReconfirmEnterNCustomHandler.mInput_N1SwitchMainSetStatus = mInput_N1SwitchMainSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N1SwitchNModeSetStatus = mInput_N1SwitchNModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N1SwitchCustom1ModeSetStatus = mInput_N1SwitchCustom1ModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N1SwitchCustom2ModeSetStatus = mInput_N1SwitchCustom2ModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_EscCustom1EnableStatus = mInput_EscCustom1EnableStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N2SwitchMainSetStatus = mInput_N2SwitchMainSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N2SwitchNModeSetStatus = mInput_N2SwitchNModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N2SwitchCustom1ModeSetStatus = mInput_N2SwitchCustom1ModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N2SwitchCustom2ModeSetStatus = mInput_N2SwitchCustom2ModeSetStatus;
        mEventReconfirmEnterNCustomHandler.mInput_EscCustom2EnableStatus = mInput_EscCustom2EnableStatus;
        mEventReconfirmEnterNCustomHandler.mInput_N2SwitchStatus = mInput_N2SwitchStatus;

        mEventReconfirmEnterGT_MYHandler.mInput_GearSelectStatusFromTCU = mInput_GearSelectStatusFromTCU;
        mEventReconfirmEnterGT_MYHandler.mInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mEventReconfirmEnterGT_MYHandler.mInput_EscCustom1EnableStatus = mInput_EscCustom1EnableStatus;
        mEventReconfirmEnterGT_MYHandler.mInput_GtSwitchStatus = mInput_GtSwitchStatus;

        // Constant(IMG_DriveModePopupTitle_stat) should be pre-processed.
        mEventTerrainModeExitNotiHandler.mIMG_DriveModePopupTitle_stat = mIMG_DriveModePopupTitle_stat;
        mEventTerrainModeExitNotiHandler.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mEventTerrainModeExitNotiHandler.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mEventTerrainModeExitNotiHandler.mInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mEventTerrainModeExitNotiHandler.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;

        mEventAwd2hModeNotificationHandler.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mEventAwd2hModeNotificationHandler.mInput_AwdMode2HStatus = mInput_AwdMode2HStatus;
        mEventAwd2hModeNotificationHandler.mInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mEventAwd2hModeNotificationHandler.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;

        mEventUnableToSwitchModeByTowModeHandler.mIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        mEventUnableToSwitchModeByTowModeHandler.mInput_AwdMode2HStatus = mInput_AwdMode2HStatus;
        mEventUnableToSwitchModeByTowModeHandler.mPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mEventUnableToSwitchModeByTowModeHandler.mInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mEventUnableToSwitchModeByTowModeHandler.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mEventUnableToSwitchModeByTowModeHandler.mInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mEventUnableToSwitchModeByTowModeHandler.mInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;

        // 5.2.5.3.1 Driving mode change
        std::string eventID = mEventDrivingModeChangeHandler.process();
        std::string prevEventID;
        GETCACHEDVALUE(SFC.Drive_Mode.Event.DrivingModeChange.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventDrivingModeChangeStat(Drive_Mode::SFCDrive_ModeEventDrivingModeChangeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventDrivingModeChangeID(eventID);
            setSFCDrive_ModeEventDrivingModeChangeStat(Drive_Mode::SFCDrive_ModeEventDrivingModeChangeStat::ON);
        }

        // 5.2.5.3.2 Terrain mode exit notification
        // TODO(csh) : No Event Spec added, delete compare logic
        eventID = mEventTerrainModeExitNotiHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.TerrainModeExitNotification.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventTerrainModeExitNotificationStat(
                Drive_Mode::SFCDrive_ModeEventTerrainModeExitNotificationStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventTerrainModeExitNotificationID(eventID);
            setSFCDrive_ModeEventTerrainModeExitNotificationStat(
                Drive_Mode::SFCDrive_ModeEventTerrainModeExitNotificationStat::ON);
        }

        // 5.2.5.3.3 N 커스텀 모드 진입 재확인
        eventID = mEventReconfirmEnterNCustomHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.ReconfirmNCustomMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventReconfirmNCustomModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmNCustomModeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventReconfirmNCustomModeID(eventID);
            setSFCDrive_ModeEventReconfirmNCustomModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmNCustomModeStat::ON);
        }

        // 5.2.5.3.4 GT_MY 모드 진입 재확인
        eventID = mEventReconfirmEnterGT_MYHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.ReconfirmGT_MYMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventReconfirmGT_MYModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmGT_MYModeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventReconfirmGT_MYModeID(eventID);
            setSFCDrive_ModeEventReconfirmGT_MYModeStat(Drive_Mode::SFCDrive_ModeEventReconfirmGT_MYModeStat::ON);
        }

        // 5.2.5.3.5 모드 전환 불가 안내
        // TODO(csh) : No Event Spec added, delete compare logic
        eventID = mEventModeSwitchingImpossibleIndiHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.ModeSwitchingImpossible.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventModeSwitchingImpossibleStat(Drive_Mode::SFCDrive_ModeEventModeSwitchingImpossibleStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventModeSwitchingImpossibleID(eventID);
            setSFCDrive_ModeEventModeSwitchingImpossibleStat(Drive_Mode::SFCDrive_ModeEventModeSwitchingImpossibleStat::ON);
        }

        // 5.2.5.3.6 구동모드 2H에 의한 오프로드 모드 진입 불가 알림
        eventID = mEventAwd2hModeNotificationHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.Awd2hModeNotification.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventAwd2hModeNotificationStat(SFCDrive_ModeEventAwd2hModeNotificationStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventAwd2hModeNotificationID(eventID);
            setSFCDrive_ModeEventAwd2hModeNotificationStat(SFCDrive_ModeEventAwd2hModeNotificationStat::ON);
        }

        // 5.2.5.3.7 TOW 모드 진입에 의한 모드 전환 불가 알림
        eventID = mEventUnableToSwitchModeByTowModeHandler.process();
        GETCACHEDVALUE(SFC.Drive_Mode.Event.UnableToSwitchModeByTowMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDrive_ModeEventUnableToSwitchModeByTowModeStat(SFCDrive_ModeEventUnableToSwitchModeByTowModeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            hasToFlush = true;
            setSFCDrive_ModeEventUnableToSwitchModeByTowModeID(eventID);
            setSFCDrive_ModeEventUnableToSwitchModeByTowModeStat(SFCDrive_ModeEventUnableToSwitchModeByTowModeStat::ON);
        }

        if (hasToFlush == true) {
            DDebug() << "[HIT] Event Logic";
        }
        return hasToFlush;
    }

    static constexpr uint64_t kDriveModeEcoBitIndex = 0;
    static constexpr uint64_t kDriveModeNormalBitIndex = 1;
    static constexpr uint64_t kDriveModeSportBitIndex = 2;
    static constexpr uint64_t kDriveModeMyDriveBitIndex = 3;
    static constexpr uint64_t kTerrainModeSnowBitIndex = 6;
    static constexpr uint64_t kTerrainModeMudBitIndex = 4;
    static constexpr uint64_t kTerrainModeSandBitIndex = 5;
    static constexpr uint64_t kTerrainModeDesertBitIndex = 5;
    static constexpr uint64_t kTerrainModeAutoBitIndex = 8;
    static constexpr uint64_t kTerrainModeRockBitIndex = 7;

    // source external - VSM
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInput_GearSelectStatusFromTCU mInput_GearSelectStatusFromTCU = EInput_GearSelectStatusFromTCU::INVALID;
    EInput_EscCustom1EnableStatus mInput_EscCustom1EnableStatus = EInput_EscCustom1EnableStatus::NORMAL;
    EInput_EscCustom2EnableStatus mInput_EscCustom2EnableStatus = EInput_EscCustom2EnableStatus::NORMAL;
    EInput_SmartDriveModeStatus mInput_SmartDriveModeStatus = EInput_SmartDriveModeStatus::INACTIVE;
    // source internal - Not Found
    // TODO(andre) : Need to acquire new specifications [EXNCP-290]. refer: Event Control Logic
    EInter_NCustom1AdminStatus mInter_NCustom1AdminStatus = EInter_NCustom1AdminStatus::OFF;
    // TODO(andre) : Need to acquire new specifications [EXNCP-290]. refer: Event Control Logic
    EInter_NCustom2AdminStatus mInter_NCustom2AdminStatus = EInter_NCustom2AdminStatus::OFF;
    EInter_NModeOnReq mInter_NModeOnReq = EInter_NModeOnReq::DEFAULT;
    // TODO(andre) : from Input Processing
    EInter_NModeSwitchStatus mInter_NModeSwitchStatus = EInter_NModeSwitchStatus::DEFAULT;
    // TODO(andre) : from Input Processing
    EInter_DrvmodeSwitchStatus mInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    // TODO(csh) : from Input_Output_Processing -> will be changed to HardWire
    EInter_DriveModeOnRoadSwitchStatus mInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;
    EInter_DriveModeTerrainSwitchStatus mInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;
    // TODO(csh) : VSM (N Mode Switch)
    EInput_N1SwitchCustom1ModeSetStatus mInput_N1SwitchCustom1ModeSetStatus = EInput_N1SwitchCustom1ModeSetStatus::OFF;
    EInput_N1SwitchCustom2ModeSetStatus mInput_N1SwitchCustom2ModeSetStatus = EInput_N1SwitchCustom2ModeSetStatus::OFF;
    EInput_N1SwitchMainSetStatus mInput_N1SwitchMainSetStatus = EInput_N1SwitchMainSetStatus::INVALID;
    EInput_N1SwitchNModeSetStatus mInput_N1SwitchNModeSetStatus = EInput_N1SwitchNModeSetStatus::OFF;
    EInput_N1SwitchStatus mInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N2SwitchCustom1ModeSetStatus mInput_N2SwitchCustom1ModeSetStatus = EInput_N2SwitchCustom1ModeSetStatus::OFF;
    EInput_N2SwitchCustom2ModeSetStatus mInput_N2SwitchCustom2ModeSetStatus = EInput_N2SwitchCustom2ModeSetStatus::OFF;
    EInput_N2SwitchMainSetStatus mInput_N2SwitchMainSetStatus = EInput_N2SwitchMainSetStatus::INVALID;
    EInput_N2SwitchNModeSetStatus mInput_N2SwitchNModeSetStatus = EInput_N2SwitchNModeSetStatus::OFF;
    EInput_N2SwitchStatus mInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;
    // TODO(csh) : VSM (RCS Controller)
    EInput_RcsOptionStatus mInput_RcsOptionStatus = EInput_RcsOptionStatus::NONE;
    EInput_RcsDriveModeOnOffStatus mInput_RcsDriveModeOnOffStatus = EInput_RcsDriveModeOnOffStatus::NORMAL_MODE;
    EInput_RcsDriveModeStatus mInput_RcsDriveModeStatus = EInput_RcsDriveModeStatus::NORMAL;
    EInput_RcsTerrainModeStatus mInput_RcsTerrainModeStatus = EInput_RcsTerrainModeStatus::OFF;
    EInput_RcsDriveModeRotateEndStatus mInput_RcsDriveModeRotateEndStatus = EInput_RcsDriveModeRotateEndStatus::OFF;

    // TODO(csh) : VSM (GT Mode signal)
    EInput_GtSwitchStatus mInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;
    EInter_GtMyAdminStatus mInter_GtMyAdminStatus = EInter_GtMyAdminStatus::OFF;

    EInput_AwdMode2HStatus mInput_AwdMode2HStatus = EInput_AwdMode2HStatus::OFF;

    EIMG_TelltaleTowMode_stat mIMG_TelltaleTowMode_stat = EIMG_TelltaleTowMode_stat::OFF;
    EInter_DriveModeChangeReqFromAVNT mInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;

    HUInt64 mInput_AwdModeDriveModeControlStatus = 0;

    // source internal - OCL
    EInter_ConfigDriveModeOption mInter_ConfigDriveModeOption = EInter_ConfigDriveModeOption::OFF;

    EInter_ConfigOffRoad mInter_ConfigOffRoad = EInter_ConfigOffRoad::OFF;
    EInter_ConfigHighPerformance mInter_ConfigHighPerformance = EInter_ConfigHighPerformance::OFF;
    EInter_ConfigTowModeLongKey mInter_ConfigTowModeLongKey = EInter_ConfigTowModeLongKey::OFF;
    EInter_ConfigCustomMyDrive mInter_ConfigCustomMyDrive = EInter_ConfigCustomMyDrive::OFF;
    EInter_ConfigCustomSnow mInter_ConfigCustomSnow = EInter_ConfigCustomSnow::OFF;
    EInter_ConfigDriveModeDefault mInter_ConfigDriveModeDefault = EInter_ConfigDriveModeDefault::NORMAL;
    EInter_ConfigCustomSmart mInter_ConfigCustomSmart = EInter_ConfigCustomSmart::OFF;
    EInter_ConfigTerrainAuto mInter_ConfigTerrainAuto = EInter_ConfigTerrainAuto::OFF;
    EInter_ConfigTerrainMud mInter_ConfigTerrainMud = EInter_ConfigTerrainMud::OFF;
    EInter_ConfigTerrainSand mInter_ConfigTerrainSand = EInter_ConfigTerrainSand::OFF;
    EInter_ConfigTerrainSnow mInter_ConfigTerrainSnow = EInter_ConfigTerrainSnow::OFF;
    EInter_ConfigTerrainDeepSnow mInter_ConfigTerrainDeepSnow = EInter_ConfigTerrainDeepSnow::OFF;
    EInter_ConfigTerrainRock mInter_ConfigTerrainRock = EInter_ConfigTerrainRock::OFF;
    EInter_ConfigTerrainDesert mInter_ConfigTerrainDesert = EInter_ConfigTerrainDesert::OFF;
    EInter_ConfigDriveModeKeepEco mInter_ConfigDriveModeKeepEco = EInter_ConfigDriveModeKeepEco::OFF;
    EInter_ConfigDriveModeKeepSport mInter_ConfigDriveModeKeepSport = EInter_ConfigDriveModeKeepSport::OFF;
    EInter_ConfigDriveModeKeepNormal mInter_ConfigDriveModeKeepNormal = EInter_ConfigDriveModeKeepNormal::OFF;

    EInter_ConfigEcoMode mInter_ConfigEcoMode = EInter_ConfigEcoMode::OFF;
    EInter_ConfigNormalMode mInter_ConfigNormalMode = EInter_ConfigNormalMode::OFF;
    EInter_ConfigSportMode mInter_ConfigSportMode = EInter_ConfigSportMode::OFF;
    EInter_ConfigChauffeurMode mInter_ConfigChauffeurMode = EInter_ConfigChauffeurMode::OFF;

    EInter_ConfigEndlessDialSwitch mInter_ConfigEndlessDialSwitch = EInter_ConfigEndlessDialSwitch::OFF;
    // sink internal
    Inter_NModeSelectStatus mInter_NModeSelectStatus = Inter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::OFF);
    Inter_DriveModeSelectStatus mInter_DriveModeSelectStatus =
        Inter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL);
    Inter_TerrainModeSelectStatus mInter_TerrainModeSelectStatus =
        Inter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::OFF);
    Inter_GtModeSelectStatus mInter_GtModeSelectStatus = Inter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::OFF);
    Inter_DriveModeChangeResultStatus mInter_DriveModeChangeResultStatus =
        Inter_DriveModeChangeResultStatus(ESFCDrive_ModeInter_DriveModeChangeResultStatus::INVALID_OPERATION);
    // sink external
    AddressValueList mActuatorOutputAddrList;
    Output_CustomDriveModeApplyStatus mOutput_CustomDriveModeApplyStatus;
    Output_DriveModeSwitchStatus mOutput_DriveModeSwitchStatus;
    Output_TerrainModeSwitchStatus mOutput_TerrainModeSwitchStatus;
    Output_TerrainModeMainSwitchStatus mOutput_TerrainModeMainSwitchStatus;
    Output_CustomDriveModeSetStatus mOutput_CustomDriveModeSetStatus;
    // sink sfc - constant
    IMG_DriveModePopupTitle_stat mIMG_DriveModePopupTitle_stat;
    IMG_N1ModePopupListN_stat mIMG_N1ModePopupListN_stat;
    IMG_N1ModePopupListCustom1_stat mIMG_N1ModePopupListCustom1_stat;
    IMG_N1ModePopupListCustom2_stat mIMG_N1ModePopupListCustom2_stat;
    IMG_N2ModePopupListN_stat mIMG_N2ModePopupListN_stat;
    IMG_N2ModePopupListCustom1_stat mIMG_N2ModePopupListCustom1_stat;
    IMG_N2ModePopupListCustom2_stat mIMG_N2ModePopupListCustom2_stat;
    IMG_GtModePopupListGt_stat mIMG_GtModePopupListGt_stat;
    IMG_GtModePopupListMy_stat mIMG_GtModePopupListMy_stat;
    IMG_DriveModeSelect_stat mIMG_DriveModeSelect_stat;
    IMG_TerrainModeSelect_stat mIMG_TerrainModeSelect_stat;
    IMG_NModeSelect_stat mIMG_NModeSelect_stat;
    IMG_GtModeSelect_stat mIMG_GtModeSelect_stat;

    IMG_TerrainModePopupListSnow_stat mIMG_TerrainModePopupListSnow_stat;
    IMG_TerrainModePopupListMud_stat mIMG_TerrainModePopupListMud_stat;
    IMG_TerrainModePopupListSand_stat mIMG_TerrainModePopupListSand_stat;
    IMG_TerrainModePopupListAuto_stat mIMG_TerrainModePopupListAuto_stat;
    IMG_TerrainModePopupListDeepSnow_stat mIMG_TerrainModePopupListDeepSnow_stat;
    IMG_TerrainModePopupListRock_stat mIMG_TerrainModePopupListRock_stat;
    IMG_TerrainModePopupListDesert_stat mIMG_TerrainModePopupListDesert_stat;

    IMG_DriveModePopupListNormal_stat mIMG_DriveModePopupListNormal_stat;
    IMG_DriveModePopupListEco_stat mIMG_DriveModePopupListEco_stat;
    IMG_DriveModePopupListSport_stat mIMG_DriveModePopupListSport_stat;
    IMG_DriveModePopupListChauffeur_stat mIMG_DriveModePopupListChauffeur_stat;
    IMG_DriveModePopupListMyDrive_stat mIMG_DriveModePopupListMyDrive_stat;
    IMG_DriveModePopupListSmart_stat mIMG_DriveModePopupListSmart_stat;
    IMG_DriveModePopupListSnow_stat mIMG_DriveModePopupListSnow_stat;
    IMG_DriveModeTowLongkeyQue_stat mIMG_DriveModeTowLongkeyQue_stat;

    IMG_DriveModePopupListType_stat mIMG_DriveModePopupListType_stat;
    // sink sfc - telltale
    SFCDrive_ModeTelltaleDimmedStat mDrive_ModeTelltaleDimmedStat;
    SFCDrive_ModeTelltaleDMSStat mDrive_ModeTelltaleDMSStat;
    // sink sfc - event
    EventDrivingModeChangeHandler mEventDrivingModeChangeHandler;
    EventModeSwitchingImpossibleIndiHandler mEventModeSwitchingImpossibleIndiHandler;
    EventReconfirmEnterNCustomHandler mEventReconfirmEnterNCustomHandler;
    EventTerrainModeExitNotiHandler mEventTerrainModeExitNotiHandler;
    EventReconfirmEnterGT_MYHandler mEventReconfirmEnterGT_MYHandler;
    EventAwd2hModeNotificationHandler mEventAwd2hModeNotificationHandler;
    EventUnableToSwitchModeByTowModeHandler mEventUnableToSwitchModeByTowModeHandler;
    // private chain
    Private_DriveModeSwStatus mPrivate_DriveModeSwStatus = Private_DriveModeSwStatus(EPrivate_DriveModeSwStatus::DEFAULT);
    Private_BasicModeSwitchableStatus mPrivate_BasicModeSwitchableStatus;
    Private_BasicModeForSwKeySwitchableStatus mPrivate_BasicModeForSwKeySwitchableStatus;

    bool mIsAEMInitComplete = false;
    bool mIgnStates = false;
};  // Class Drive_Mode
}  // namespace ccos

#endif  // SFSS_Drive_Mode_H
