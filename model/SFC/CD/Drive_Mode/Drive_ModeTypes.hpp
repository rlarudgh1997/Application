/**
 * @file Drive_ModeTypes.hpp
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

#ifndef SSFS_Drive_Mode_Drive_ModeTypes_H
#define SSFS_Drive_Mode_Drive_ModeTypes_H
#include <Vehicle.h>
#include <Timer.h>
#include "Drive_ModeBase.hpp"
#include "BaseNodeTemplate.hpp"

#include "circularQueue.hpp"

#define DLOG_ENABLED gEnableSFCLog

namespace ccos {

using EInter_ConfigDriveModeOption = ccos::Drive_ModeBase::ArgumentsDriveModeOptionConfigChanged::Inter_ConfigDriveModeOption;

using ESFCDrive_ModeTelltaleDMSStat = ccos::Drive_ModeBase::SFCDrive_ModeTelltaleDrive_ModeDMSStatOptional;
using ESFCDrive_ModeTelltaleDimmedStat = ccos::Drive_ModeBase::SFCDrive_ModeTelltaleDrive_ModeDimmedStatOptional;
using EInput_TerrainModeErrorStatus = ccos::Drive_ModeBase::ArgumentsDriveModeChanged::Input_TerrainModeErrorStatus;
using EInput_GearSelectStatusFromTCU = ccos::Drive_ModeBase::ArgumentsDriveModeChanged::Input_GearSelectStatusFromTCU;
using EInput_EscCustom1EnableStatus = ccos::Drive_ModeBase::ArgumentsDriveModeChanged::Input_EscCustom1EnableStatus;
using EInput_EscCustom2EnableStatus = ccos::Drive_ModeBase::ArgumentsDriveModeChanged::Input_EscCustom2EnableStatus;
using EInput_SmartDriveModeStatus = ccos::Drive_ModeBase::ArgumentsDriveModeChanged::Input_SmartDriveModeStatus;
using EInter_ConfigCustomSmart = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigCustomSmart;
using EInter_NCustom1AdminStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_NCustom1AdminStatus;
using EInter_NCustom2AdminStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_NCustom2AdminStatus;
using EInter_NModeOnReq = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_NModeOnReq;
using EInter_NModeSwitchStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_NModeSwitchStatus;
using EInter_DrvmodeSwitchStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_DrvmodeSwitchStatus;
using EInter_DriveModeOnRoadSwitchStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_DriveModeOnRoadSwitchStatus;
using EInput_N1SwitchCustom2ModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N1SwitchCustom2ModeSetStatus;
using EInter_DriveModeTerrainSwitchStatus = ccos::Drive_ModeBase::ArgumentsMcuInterChanged::Inter_DriveModeTerrainSwitchStatus;

using EInput_N1SwitchCustom1ModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N1SwitchCustom1ModeSetStatus;

using EInput_N1SwitchMainSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N1SwitchMainSetStatus;
using EInput_N1SwitchNModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N1SwitchNModeSetStatus;
using EInput_N1SwitchStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N1SwitchStatus;
using EInput_N2SwitchCustom1ModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N2SwitchCustom1ModeSetStatus;
using EInput_N2SwitchCustom2ModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N2SwitchCustom2ModeSetStatus;
using EInput_N2SwitchMainSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N2SwitchMainSetStatus;
using EInput_N2SwitchNModeSetStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N2SwitchNModeSetStatus;
using EInput_N2SwitchStatus = ccos::Drive_ModeBase::ArgumentsNModeSwitchChanged::Input_N2SwitchStatus;

using EInput_GtSwitchStatus = ccos::Drive_ModeBase::ArgumentsGtModeChanged::Input_GtSwitchStatus;
using EInter_GtMyAdminStatus = ccos::Drive_ModeBase::ArgumentsGtModeChanged::Inter_GtMyAdminStatus;

using EInput_AwdMode2HStatus = ccos::Drive_ModeBase::ArgumentsAwdModeStatChanged::Input_AwdMode2HStatus;

using EIMG_TelltaleTowMode_stat = ccos::Drive_ModeBase::ArgumentsTowModeStatusChanged::TowModeStat;
using EInter_DriveModeChangeReqFromAVNT =
    ccos::Drive_ModeBase::ArgumentsDriveModeChangeReqFromAVNTChanged::Inter_DriveModeChangeReqFromAVNT;

using EInter_ConfigOffRoad = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigOffRoad;
using EInter_ConfigHighPerformance = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigHighPerformance;
using EInter_ConfigTowModeLongKey = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTowModeLongKey;
using EInter_ConfigCustomMyDrive = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigCustomMyDrive;
using EInter_ConfigDriveModeKeepEco = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigDriveModeKeepEco;
using EInter_ConfigDriveModeKeepSport = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigDriveModeKeepSport;
using EInter_ConfigDriveModeKeepNormal = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigDriveModeKeepNormal;

using EInter_ConfigCustomSnow = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigCustomSnow;
using EInter_ConfigTerrainAuto = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainAuto;
using EInter_ConfigTerrainMud = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainMud;
using EInter_ConfigTerrainSand = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainSand;
using EInter_ConfigTerrainSnow = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainSnow;
using EInter_ConfigTerrainDeepSnow = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainDeepSnow;
using EInter_ConfigTerrainRock = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainRock;
using EInter_ConfigTerrainDesert = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigTerrainDesert;
using EInter_ConfigDriveModeDefault = ccos::Drive_ModeBase::ArgumentsSystemConfigChanged::Inter_ConfigDriveModeDefault;

using EInter_ConfigEcoMode = ccos::Drive_ModeBase::ArgumentsConfigChanged::Inter_ConfigEcoMode;
using EInter_ConfigNormalMode = ccos::Drive_ModeBase::ArgumentsConfigChanged::Inter_ConfigNormalMode;
using EInter_ConfigSportMode = ccos::Drive_ModeBase::ArgumentsConfigChanged::Inter_ConfigSportMode;
using EInter_ConfigChauffeurMode = ccos::Drive_ModeBase::ArgumentsConfigChanged::Inter_ConfigChauffeurMode;

using EInter_ConfigEndlessDialSwitch =
    ccos::Drive_ModeBase::ArgumentsEndlessDialSwitchConfigChanged::Inter_ConfigEndlessDialSwitch;

using EInput_RcsOptionStatus = ccos::Drive_ModeBase::ArgumentsRcsControllerChanged::Input_RcsOptionStatus;
using EInput_RcsDriveModeStatus = ccos::Drive_ModeBase::ArgumentsRcsControllerChanged::Input_RcsDriveModeStatus;
using EInput_RcsTerrainModeStatus = ccos::Drive_ModeBase::ArgumentsRcsControllerChanged::Input_RcsTerrainModeStatus;
using EInput_RcsDriveModeOnOffStatus = ccos::Drive_ModeBase::ArgumentsRcsControllerChanged::Input_RcsDriveModeOnOffStatus;
using EInput_RcsDriveModeRotateEndStatus = ccos::Drive_ModeBase::ArgumentsRcsControllerChanged::Input_RcsDriveModeRotateEndStatus;

using ESFCDrive_ModeInter_DriveModeSelectStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_DriveModeSelectStatus;
using ESFCDrive_ModeInter_GtModeSelectStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_GtModeSelectStatus;
using ESFCDrive_ModeInter_DriveModeChangeResultStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_DriveModeChangeResultStatus;

// Constant
using ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupTitle_stat;

using ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat;
using ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat;

using ESFCDrive_ModeConstantIMG_DriveModeSelect_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModeSelect_stat;
using ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_TerrainModeSelect_stat;
using ESFCDrive_ModeConstantIMG_NModeSelect_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_NModeSelect_stat;
using ESFCDrive_ModeConstantIMG_GtModeSelect_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_GtModeSelect_stat;

using ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N1ModePopupListN_stat;
using ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat;
using ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat;
using ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N2ModePopupListN_stat;
using ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat;
using ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat;

using ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_GtModePopupListGt_stat;

using ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat = ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_GtModePopupListMy_stat;

using ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListEco_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListSport_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat;
using ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModeTowLongkeyQue_stat;
using ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat =
    ccos::Drive_ModeBase::SFCDrive_ModeConstantIMG_DriveModePopupListType_stat;

using EInter_TerrainModeSelectStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_TerrainModeSelectStatus;
using EInter_DriveModeSelectStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_DriveModeSelectStatus;
using EInter_DriveModeChangeResultStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_DriveModeChangeResultStatus;
using ESFCDrive_ModeInter_NModeSelectStatus = ccos::Drive_ModeBase::SFCDrive_ModeInter_NModeSelectStatus;

using EOutputCustomDriveModeApplyStatus = VSM::VSMCD_SKEL::VSMDrive_Mode::VSMOutput_CustomDriveModeApplyStatus::Type;
using EOutputDriveModeSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode::VSMOutput_DriveModeSwitchStatus::Type;
using EOutputTerrainModeSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode::VSMOutput_TerrainModeSwitchStatus::Type;
using EOutputTerrainModeMainSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode::VSMOutput_TerrainModeMainSwitchStatus::Type;
using EOutputCustomDriveModeSetStatus = VSM::VSMCD_SKEL::VSMDrive_Mode::VSMOutput_CustomDriveModeSetStatus::Type;

enum class EPrivate_BasicModeSwitchableStatus {
    ENABLE,
    DISABLE,
    MAX
};

enum class EPrivate_BasicModeForSwKeySwitchableStatus {
    ENABLE,
    DISABLE
};

enum class EPrivate_DriveModeSwStatus {
    PUSH_TERRAIN,
    CCW,
    CW,
    CCW_LONG,
    CW_LONG,
    DEFAULT,
    MAX
};

class SFCDrive_ModeTelltaleDimmedStat : public BaseNode<ESFCDrive_ModeTelltaleDimmedStat> {
public:
    explicit SFCDrive_ModeTelltaleDimmedStat(ESFCDrive_ModeTelltaleDimmedStat _e = ESFCDrive_ModeTelltaleDimmedStat::MAX)
        : BaseNode(_e),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mInter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<ESFCDrive_ModeInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeTelltaleDimmedStat process() override;
};

class SFCDrive_ModeTelltaleDMSStat : public BaseNode<ESFCDrive_ModeTelltaleDMSStat> {
public:
    explicit SFCDrive_ModeTelltaleDMSStat(ESFCDrive_ModeTelltaleDMSStat _e = ESFCDrive_ModeTelltaleDMSStat::MAX)
        : BaseNode(_e),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInput_SmartDriveModeStatus(EInput_SmartDriveModeStatus::INACTIVE, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<EInput_SmartDriveModeStatus> mInput_SmartDriveModeStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeTelltaleDMSStat process() override;
};

class Inter_DriveModeSelectStatus : public BaseNode<ESFCDrive_ModeInter_DriveModeSelectStatus> {
public:
    explicit Inter_DriveModeSelectStatus(
        ESFCDrive_ModeInter_DriveModeSelectStatus _e = ESFCDrive_ModeInter_DriveModeSelectStatus::MAX)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigHighPerformance(EInter_ConfigHighPerformance::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigTowModeLongKey(EInter_ConfigTowModeLongKey::OFF, [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListEco_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE,
                                          [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListNormal_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListSport_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE,
                                            [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListChauffeur_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::NONE,
                                                [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListMyDrive_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::NONE,
                                              [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListSmart_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::NONE,
                                            [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListSnow_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::NONE,
                                           [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeList({EInter_DriveModeSelectStatus::ECO, EInter_DriveModeSelectStatus::NORMAL,
                                  EInter_DriveModeSelectStatus::SPORT, EInter_DriveModeSelectStatus::CHAUFFEUR,
                                  EInter_DriveModeSelectStatus::MY_DRIVE, EInter_DriveModeSelectStatus::SMART,
                                  EInter_DriveModeSelectStatus::SNOW}),
          mPrivate_DriveModeSwStatus(EPrivate_DriveModeSwStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_DriveModeOnRoadSwitchStatus(EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mInput_RcsDriveModeOnOffStatus(EInput_RcsDriveModeOnOffStatus::NORMAL_MODE, [this]() { setNeedUpdate(); }),
          mInput_RcsDriveModeStatus(EInput_RcsDriveModeStatus::NORMAL, [this]() { setNeedUpdate(); }),
          mInter_ConfigEndlessDialSwitch(EInter_ConfigEndlessDialSwitch::OFF, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSwitchableStatus(EPrivate_BasicModeSwitchableStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeDefault(EInter_ConfigDriveModeDefault::NORMAL, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepEco(EInter_ConfigDriveModeKeepEco::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepNormal(EInter_ConfigDriveModeKeepNormal::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepSport(EInter_ConfigDriveModeKeepSport::OFF, [this]() { setNeedUpdate(); }),
          mInput_RcsTerrainModeStatus(EInput_RcsTerrainModeStatus::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigOffRoad(EInter_ConfigOffRoad::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigEcoMode(EInter_ConfigEcoMode::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigNormalMode(EInter_ConfigNormalMode::OFF, [this]() { setNeedUpdate(); }),
          mIMG_TelltaleTowMode_stat(EIMG_TelltaleTowMode_stat::OFF, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeForSwKeySwitchableStatus(EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE,
                                                     [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::NONE, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigHighPerformance> mInter_ConfigHighPerformance;
    BaseNodeProp<EInter_ConfigTowModeLongKey> mInter_ConfigTowModeLongKey;
    enum class Private_LongPressAppliedStatus {
        APPLY,
        OFF
    };
    Private_LongPressAppliedStatus mPrivate_LongPressAppliedStatus = Private_LongPressAppliedStatus::OFF;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat> mIMG_DriveModePopupListEco_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat> mIMG_DriveModePopupListNormal_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat> mIMG_DriveModePopupListSport_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat> mIMG_DriveModePopupListChauffeur_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat> mIMG_DriveModePopupListMyDrive_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat> mIMG_DriveModePopupListSmart_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat> mIMG_DriveModePopupListSnow_stat;
    Private_DriveModeList<EInter_DriveModeSelectStatus> mPrivate_BasicModeList;
    enum class Private_BasicEvent {
        CW,
        CCW,
        OFF
    };
    Private_BasicEvent mPrivate_BasicEvent = Private_BasicEvent::OFF;
    BaseNodeProp<EPrivate_DriveModeSwStatus> mPrivate_DriveModeSwStatus;
    BaseNodeProp<EInter_DriveModeOnRoadSwitchStatus> mInter_DriveModeOnRoadSwitchStatus;
    BaseNodeProp<EInput_RcsDriveModeOnOffStatus> mInput_RcsDriveModeOnOffStatus;
    BaseNodeProp<EInput_RcsDriveModeStatus> mInput_RcsDriveModeStatus;
    BaseNodeProp<EInter_ConfigEndlessDialSwitch> mInter_ConfigEndlessDialSwitch;
    BaseNodeProp<EPrivate_BasicModeSwitchableStatus> mPrivate_BasicModeSwitchableStatus;
    BaseNodeProp<EInter_ConfigDriveModeDefault> mInter_ConfigDriveModeDefault;
    BaseNodeProp<EInter_ConfigDriveModeKeepEco> mInter_ConfigDriveModeKeepEco;
    BaseNodeProp<EInter_ConfigDriveModeKeepNormal> mInter_ConfigDriveModeKeepNormal;
    BaseNodeProp<EInter_ConfigDriveModeKeepSport> mInter_ConfigDriveModeKeepSport;
    BaseNodeProp<EInput_RcsTerrainModeStatus> mInput_RcsTerrainModeStatus;
    BaseNodeProp<EInter_ConfigOffRoad> mInter_ConfigOffRoad;
    BaseNodeProp<EInter_ConfigEcoMode> mInter_ConfigEcoMode;
    BaseNodeProp<EInter_ConfigNormalMode> mInter_ConfigNormalMode;
    BaseNodeProp<EIMG_TelltaleTowMode_stat> mIMG_TelltaleTowMode_stat;
    BaseNodeProp<EPrivate_BasicModeForSwKeySwitchableStatus> mPrivate_BasicModeForSwKeySwitchableStatus;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    bool mIsSingleDriveModeEnabled = false;

private:
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus;
    ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat mPrevIMG_DriveModePopupListEco_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat mPrevIMG_DriveModePopupListNormal_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat mPrevIMG_DriveModePopupListSport_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat mPrevIMG_DriveModePopupListChauffeur_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat mPrevIMG_DriveModePopupListMyDrive_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat mPrevIMG_DriveModePopupListSmart_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::NONE;
    ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat mPrevIMG_DriveModePopupListSnow_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::NONE;
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;
    bool mPrevIgnStatus = false;

    ESFCDrive_ModeInter_DriveModeSelectStatus process() override;
};

class Private_BasicModeSwitchableStatus : public BaseNode<EPrivate_BasicModeSwitchableStatus> {
public:
    explicit Private_BasicModeSwitchableStatus(EPrivate_BasicModeSwitchableStatus _e = EPrivate_BasicModeSwitchableStatus::MAX)
        : BaseNode(_e),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mIMG_TelltaleTowMode_stat(EIMG_TelltaleTowMode_stat::OFF, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;
    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<EIMG_TelltaleTowMode_stat> mIMG_TelltaleTowMode_stat;

private:
    EPrivate_BasicModeSwitchableStatus process() override;
};

class Private_BasicModeForSwKeySwitchableStatus : public BaseNode<EPrivate_BasicModeForSwKeySwitchableStatus> {
public:
    explicit Private_BasicModeForSwKeySwitchableStatus(
        EPrivate_BasicModeForSwKeySwitchableStatus _e = EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE)
        : BaseNode(_e),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mIMG_TelltaleTowMode_stat(EIMG_TelltaleTowMode_stat::OFF, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;
    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<EIMG_TelltaleTowMode_stat> mIMG_TelltaleTowMode_stat;

private:
    EPrivate_BasicModeForSwKeySwitchableStatus process() override;
};

class Inter_TerrainModeSelectStatus : public BaseNode<EInter_TerrainModeSelectStatus> {
public:
    explicit Inter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus _e = EInter_TerrainModeSelectStatus::MAX)
        : BaseNode(_e),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mInput_TerrainModeErrorStatus(EInput_TerrainModeErrorStatus::NORMAL, [this]() { setNeedUpdate(); }),
          mPrivate_DriveModeSwStatus(EPrivate_DriveModeSwStatus::MAX, [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListAuto_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListSnow_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListDeepSnow_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::NONE,
                                                 [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListMud_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::NONE,
                                            [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListSand_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListRock_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_TerrainModePopupListDesert_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::NONE,
                                               [this]() { setNeedUpdate(); }),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_DriveModeTerrainSwitchStatus(EInter_DriveModeTerrainSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mInter_DriveModeOnRoadSwitchStatus(EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mInput_RcsDriveModeOnOffStatus(EInput_RcsDriveModeOnOffStatus::NORMAL_MODE, [this]() { setNeedUpdate(); }),
          mInput_RcsTerrainModeStatus(EInput_RcsTerrainModeStatus::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigOffRoad(EInter_ConfigOffRoad::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigEndlessDialSwitch(EInter_ConfigEndlessDialSwitch::OFF, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdMode2HStatus(EInput_AwdMode2HStatus::OFF, [this]() { setNeedUpdate(); }),
          mIMG_TelltaleTowMode_stat(EIMG_TelltaleTowMode_stat::OFF, [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListEco_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE,
                                          [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListNormal_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListSport_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE,
                                            [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeForSwKeySwitchableStatus(EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE,
                                                     [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::NONE, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mPrivate_OffroadModeList({EInter_TerrainModeSelectStatus::AUTO, EInter_TerrainModeSelectStatus::SNOW,
                                    EInter_TerrainModeSelectStatus::DEEP_SNOW, EInter_TerrainModeSelectStatus::MUD,
                                    EInter_TerrainModeSelectStatus::SAND, EInter_TerrainModeSelectStatus::DESERT,
                                    EInter_TerrainModeSelectStatus::ROCK}) {
    }
    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<EInput_TerrainModeErrorStatus> mInput_TerrainModeErrorStatus;
    BaseNodeProp<EPrivate_DriveModeSwStatus> mPrivate_DriveModeSwStatus;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat> mIMG_TerrainModePopupListAuto_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat> mIMG_TerrainModePopupListSnow_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat> mIMG_TerrainModePopupListDeepSnow_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat> mIMG_TerrainModePopupListMud_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat> mIMG_TerrainModePopupListSand_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat> mIMG_TerrainModePopupListRock_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat> mIMG_TerrainModePopupListDesert_stat;
    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<EInter_DriveModeTerrainSwitchStatus> mInter_DriveModeTerrainSwitchStatus;
    BaseNodeProp<EInter_DriveModeOnRoadSwitchStatus> mInter_DriveModeOnRoadSwitchStatus;
    BaseNodeProp<EInput_RcsDriveModeOnOffStatus> mInput_RcsDriveModeOnOffStatus;
    BaseNodeProp<EInput_RcsTerrainModeStatus> mInput_RcsTerrainModeStatus;
    BaseNodeProp<EInter_ConfigOffRoad> mInter_ConfigOffRoad;
    BaseNodeProp<EInter_ConfigEndlessDialSwitch> mInter_ConfigEndlessDialSwitch;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;
    BaseNodeProp<EInput_AwdMode2HStatus> mInput_AwdMode2HStatus;
    BaseNodeProp<EIMG_TelltaleTowMode_stat> mIMG_TelltaleTowMode_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat> mIMG_DriveModePopupListEco_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat> mIMG_DriveModePopupListNormal_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat> mIMG_DriveModePopupListSport_stat;
    BaseNodeProp<EPrivate_BasicModeForSwKeySwitchableStatus> mPrivate_BasicModeForSwKeySwitchableStatus;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    BaseNodeProp<bool> mIgnStatus;

    enum class Private_OffroadModeSwitchableStatus {
        ENABLE,
        DISABLE
    };
    Private_OffroadModeSwitchableStatus mPrivate_OffroadModeSwitchableStatus = Private_OffroadModeSwitchableStatus::DISABLE;
    enum class Private_OffroadEvent {
        CW,
        CCW,
        OFF
    };
    Private_OffroadEvent mPrivate_OffroadEvent = Private_OffroadEvent::OFF;
    Private_DriveModeList<EInter_TerrainModeSelectStatus> mPrivate_OffroadModeList;

    static constexpr uint32_t kTimerIntervalOffToReadyForMs = 4000;
    static constexpr uint32_t kTimerIntervalNormalToErrorForMs = 500;
    uint32_t mTimerIdOffToReadyFor4000ms = 0;
    uint32_t mTimerIdNormalToErrorFor500ms = 0;

    bool onTimerOffToReady() {
        if (mCallback != nullptr) {
            BaseNode<EInter_TerrainModeSelectStatus>::operator=(EInter_TerrainModeSelectStatus::OFF);
            mCallback();
        }
        mTimerIdOffToReadyFor4000ms = 0;
        return false;
    }

    bool onTimerNormalToError() {
        if (mCallback != nullptr) {
            BaseNode<EInter_TerrainModeSelectStatus>::operator=(EInter_TerrainModeSelectStatus::OFF);
            mCallback();
        }
        mTimerIdNormalToErrorFor500ms = 0;
        return false;
    }

    void removeTimer(uint32_t& timer) {
        if (isEnableTimer(timer) == true) {
            Timer::remove(timer);
            timer = 0;
        }
    }

    inline HBool isEnableTimer(uint32_t timer) const {
        return timer != 0;
    }

private:
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus;
    EInput_TerrainModeErrorStatus mPrevInput_TerrainModeErrorStatus;
    ESFCDrive_ModeInter_NModeSelectStatus mPrevInter_NModeSelectStatus;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus;
    ESFCDrive_ModeInter_GtModeSelectStatus mPrevInter_GtModeSelectStatus;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat mPrevIMG_TerrainModePopupListAuto_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat mPrevIMG_TerrainModePopupListSnow_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat mPrevIMG_TerrainModePopupListDeepSnow_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat mPrevIMG_TerrainModePopupListMud_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat mPrevIMG_TerrainModePopupListSand_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat mPrevIMG_TerrainModePopupListRock_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::NONE;
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat mPrevIMG_TerrainModePopupListDesert_stat =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::NONE;
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;
    bool mPrevIgnStatus = false;

    EInter_TerrainModeSelectStatus process() override;
};

class Inter_NModeSelectStatus : public BaseNode<ESFCDrive_ModeInter_NModeSelectStatus> {
public:
    explicit Inter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus _e = ESFCDrive_ModeInter_NModeSelectStatus::MAX)
        : BaseNode(_e),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mInput_N1SwitchMainSetStatus(EInput_N1SwitchMainSetStatus::INVALID, [this]() { setNeedUpdate(); }),
          mInput_N1SwitchNModeSetStatus(EInput_N1SwitchNModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_N1SwitchStatus(EInput_N1SwitchStatus::DEFAULT, [this]() { setNeedUpdate(); }),
          mInput_N1SwitchCustom2ModeSetStatus(EInput_N1SwitchCustom2ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_N2SwitchMainSetStatus(EInput_N2SwitchMainSetStatus::INVALID, [this]() { setNeedUpdate(); }),
          mInput_N2SwitchNModeSetStatus(EInput_N2SwitchNModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_N2SwitchStatus(EInput_N2SwitchStatus::DEFAULT, [this]() { setNeedUpdate(); }),
          mInput_N2SwitchCustom2ModeSetStatus(EInput_N2SwitchCustom2ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigHighPerformance(EInter_ConfigHighPerformance::OFF, [this]() { setNeedUpdate(); }),
          mInter_DriveModeOnRoadSwitchStatus(EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mInput_N1SwitchCustom1ModeSetStatus(EInput_N1SwitchCustom1ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_EscCustom1EnableStatus(EInput_EscCustom1EnableStatus::DISABLE, [this]() { setNeedUpdate(); }),
          mInput_N2SwitchCustom1ModeSetStatus(EInput_N2SwitchCustom1ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInter_NCustom1AdminStatus(EInter_NCustom1AdminStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_EscCustom2EnableStatus(EInput_EscCustom2EnableStatus::DISABLE, [this]() { setNeedUpdate(); }),
          mInter_NCustom2AdminStatus(EInter_NCustom2AdminStatus::OFF, [this]() { setNeedUpdate(); }),
          mPrivate_DriveModeSwStatus(EPrivate_DriveModeSwStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_NModeOnReq(EInter_NModeOnReq::DEFAULT, [this]() { setNeedUpdate(); }),
          mInter_DriveModeTerrainSwitchStatus(EInter_DriveModeTerrainSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<EInput_N1SwitchMainSetStatus> mInput_N1SwitchMainSetStatus;
    BaseNodeProp<EInput_N1SwitchNModeSetStatus> mInput_N1SwitchNModeSetStatus;
    BaseNodeProp<EInput_N1SwitchStatus> mInput_N1SwitchStatus;
    BaseNodeProp<EInput_N1SwitchCustom2ModeSetStatus> mInput_N1SwitchCustom2ModeSetStatus;
    BaseNodeProp<EInput_N2SwitchMainSetStatus> mInput_N2SwitchMainSetStatus;
    BaseNodeProp<EInput_N2SwitchNModeSetStatus> mInput_N2SwitchNModeSetStatus;
    BaseNodeProp<EInput_N2SwitchStatus> mInput_N2SwitchStatus;

    BaseNodeProp<EInput_N2SwitchCustom2ModeSetStatus> mInput_N2SwitchCustom2ModeSetStatus;
    BaseNodeProp<EInter_ConfigHighPerformance> mInter_ConfigHighPerformance;
    BaseNodeProp<EInter_DriveModeOnRoadSwitchStatus> mInter_DriveModeOnRoadSwitchStatus;
    BaseNodeProp<EInput_N1SwitchCustom1ModeSetStatus> mInput_N1SwitchCustom1ModeSetStatus;
    BaseNodeProp<EInput_EscCustom1EnableStatus> mInput_EscCustom1EnableStatus;
    BaseNodeProp<EInput_N2SwitchCustom1ModeSetStatus> mInput_N2SwitchCustom1ModeSetStatus;
    BaseNodeProp<EInter_NCustom1AdminStatus> mInter_NCustom1AdminStatus;
    BaseNodeProp<EInput_EscCustom2EnableStatus> mInput_EscCustom2EnableStatus;
    BaseNodeProp<EInter_NCustom2AdminStatus> mInter_NCustom2AdminStatus;
    BaseNodeProp<EPrivate_DriveModeSwStatus> mPrivate_DriveModeSwStatus;
    BaseNodeProp<EInter_NModeOnReq> mInter_NModeOnReq;
    BaseNodeProp<EInter_DriveModeTerrainSwitchStatus> mInter_DriveModeTerrainSwitchStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    EInput_N1SwitchStatus mPrevInput_N1SwitchStatus;
    EInput_N2SwitchStatus mPrevInput_N2SwitchStatus;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus;
    EInter_NCustom1AdminStatus mPrevInter_NCustom1AdminStatus;
    EInter_NCustom2AdminStatus mPrevInter_NCustom2AdminStatus;
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus;
    bool mPrevIgnStatus = false;

    ESFCDrive_ModeInter_NModeSelectStatus process() override;
};

class Inter_GtModeSelectStatus : public BaseNode<ESFCDrive_ModeInter_GtModeSelectStatus> {
public:
    explicit Inter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus _e = ESFCDrive_ModeInter_GtModeSelectStatus::MAX)
        : BaseNode(_e),
          mInput_GearSelectStatusFromTCU(EInput_GearSelectStatusFromTCU::INVALID, [this]() { setNeedUpdate(); }),
          mInter_ConfigHighPerformance(EInter_ConfigHighPerformance::OFF, [this]() { setNeedUpdate(); }),
          mInput_GtSwitchStatus(EInput_GtSwitchStatus::DEFAULT, [this]() { setNeedUpdate(); }),
          mInput_EscCustom1EnableStatus(EInput_EscCustom1EnableStatus::DISABLE, [this]() { setNeedUpdate(); }),
          mInter_GtMyAdminStatus(EInter_GtMyAdminStatus::OFF, [this]() { setNeedUpdate(); }),
          mPrivate_DriveModeSwStatus(EPrivate_DriveModeSwStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_DriveModeOnRoadSwitchStatus(EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mInter_DriveModeTerrainSwitchStatus(EInter_DriveModeTerrainSwitchStatus::UN_PRESSED, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInput_GearSelectStatusFromTCU> mInput_GearSelectStatusFromTCU;
    BaseNodeProp<EInter_ConfigHighPerformance> mInter_ConfigHighPerformance;
    BaseNodeProp<EInput_GtSwitchStatus> mInput_GtSwitchStatus;
    BaseNodeProp<EInput_EscCustom1EnableStatus> mInput_EscCustom1EnableStatus;
    BaseNodeProp<EInter_GtMyAdminStatus> mInter_GtMyAdminStatus;
    BaseNodeProp<EPrivate_DriveModeSwStatus> mPrivate_DriveModeSwStatus;
    BaseNodeProp<EInter_DriveModeOnRoadSwitchStatus> mInter_DriveModeOnRoadSwitchStatus;
    BaseNodeProp<EInter_DriveModeTerrainSwitchStatus> mInter_DriveModeTerrainSwitchStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    EInput_GtSwitchStatus mPrevInput_GtSwitchStatus;
    EInter_GtMyAdminStatus mPrevInter_GtMyAdminStatus;
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus;
    bool mPrevIgnStatus = false;

    ESFCDrive_ModeInter_GtModeSelectStatus process() override;
};

class Inter_DriveModeChangeResultStatus : public BaseNode<ESFCDrive_ModeInter_DriveModeChangeResultStatus> {
public:
    explicit Inter_DriveModeChangeResultStatus(
        ESFCDrive_ModeInter_DriveModeChangeResultStatus _e = ESFCDrive_ModeInter_DriveModeChangeResultStatus::MAX)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListEco_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE,
                                          [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListNormal_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE,
                                             [this]() { setNeedUpdate(); }),
          mIMG_DriveModePopupListSport_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE,
                                            [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeForSwKeySwitchableStatus(EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE,
                                                     [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::NONE, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat> mIMG_DriveModePopupListEco_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat> mIMG_DriveModePopupListNormal_stat;
    BaseNodeProp<ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat> mIMG_DriveModePopupListSport_stat;
    BaseNodeProp<EPrivate_BasicModeForSwKeySwitchableStatus> mPrivate_BasicModeForSwKeySwitchableStatus;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    bool mIsForcedFlush = false;

private:
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;

    ESFCDrive_ModeInter_DriveModeChangeResultStatus process() override;
};

class IMG_DriveModePopupTitle_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat> {
public:
    explicit IMG_DriveModePopupTitle_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::MAX)
        : BaseNode(_e), mInter_ConfigOffRoad(EInter_ConfigOffRoad::OFF, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigOffRoad> mInter_ConfigOffRoad;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat process() override;
};

class IMG_TerrainModePopupListSnow_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat> {
public:
    explicit IMG_TerrainModePopupListSnow_stat(
        ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat _e = ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainSnow(EInter_ConfigTerrainSnow::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainSnow> mInter_ConfigTerrainSnow;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat process() override;
};

class IMG_TerrainModePopupListMud_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat> {
public:
    explicit IMG_TerrainModePopupListMud_stat(
        ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat _e = ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainMud(EInter_ConfigTerrainMud::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainMud> mInter_ConfigTerrainMud;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat process() override;
};

class IMG_TerrainModePopupListSand_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat> {
public:
    explicit IMG_TerrainModePopupListSand_stat(
        ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat _e = ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainSand(EInter_ConfigTerrainSand::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainSand> mInter_ConfigTerrainSand;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat process() override;
};

class IMG_TerrainModePopupListAuto_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat> {
public:
    explicit IMG_TerrainModePopupListAuto_stat(
        ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat _e = ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainAuto(EInter_ConfigTerrainAuto::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainAuto> mInter_ConfigTerrainAuto;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat process() override;
};

class IMG_TerrainModePopupListDeepSnow_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat> {
public:
    explicit IMG_TerrainModePopupListDeepSnow_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat _e =
                                                       ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainDeepSnow(EInter_ConfigTerrainDeepSnow::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainDeepSnow> mInter_ConfigTerrainDeepSnow;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat process() override;
};

class IMG_TerrainModePopupListRock_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat> {
public:
    explicit IMG_TerrainModePopupListRock_stat(
        ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat _e = ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainRock(EInter_ConfigTerrainRock::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainRock> mInter_ConfigTerrainRock;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat process() override;
};

class IMG_TerrainModePopupListDesert_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat> {
public:
    explicit IMG_TerrainModePopupListDesert_stat(ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat _e =
                                                     ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigTerrainDesert(EInter_ConfigTerrainDesert::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTerrainDesert> mInter_ConfigTerrainDesert;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat process() override;
};

class IMG_N1ModePopupListN_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat> {
public:
    explicit IMG_N1ModePopupListN_stat(
        ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat _e = ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat::MAX)
        : BaseNode(_e),
          mInput_N1SwitchNModeSetStatus(EInput_N1SwitchNModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInput_N1SwitchNModeSetStatus> mInput_N1SwitchNModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat process() override;
};

class IMG_N1ModePopupListCustom1_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat> {
public:
    explicit IMG_N1ModePopupListCustom1_stat(
        ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat _e = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat::MAX)
        : BaseNode(_e),
          mInput_N1SwitchCustom1ModeSetStatus(EInput_N1SwitchCustom1ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInput_N1SwitchCustom1ModeSetStatus> mInput_N1SwitchCustom1ModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat process() override;
};

class IMG_N1ModePopupListCustom2_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat> {
public:
    explicit IMG_N1ModePopupListCustom2_stat(
        ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat _e = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat::MAX)
        : BaseNode(_e),
          mInput_N1SwitchCustom2ModeSetStatus(EInput_N1SwitchCustom2ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInput_N1SwitchCustom2ModeSetStatus> mInput_N1SwitchCustom2ModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat process() override;
};

class IMG_N2ModePopupListN_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat> {
public:
    explicit IMG_N2ModePopupListN_stat(
        ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat _e = ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat::MAX)
        : BaseNode(_e),
          mInput_N2SwitchNModeSetStatus(EInput_N2SwitchNModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInput_N2SwitchNModeSetStatus> mInput_N2SwitchNModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat process() override;
};

class IMG_N2ModePopupListCustom1_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat> {
public:
    explicit IMG_N2ModePopupListCustom1_stat(
        ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat _e = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat::MAX)
        : BaseNode(_e),
          mInput_N2SwitchCustom1ModeSetStatus(EInput_N2SwitchCustom1ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInput_N2SwitchCustom1ModeSetStatus> mInput_N2SwitchCustom1ModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat process() override;
};

class IMG_N2ModePopupListCustom2_stat : public BaseNode<ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat> {
public:
    explicit IMG_N2ModePopupListCustom2_stat(
        ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat _e = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat::MAX)
        : BaseNode(_e),
          mInput_N2SwitchCustom2ModeSetStatus(EInput_N2SwitchCustom2ModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInput_N2SwitchCustom2ModeSetStatus> mInput_N2SwitchCustom2ModeSetStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat process() override;
};

class IMG_GtModePopupListGt_stat : public BaseNode<ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat> {
public:
    explicit IMG_GtModePopupListGt_stat(
        ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat _e = ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat::MAX)
        : BaseNode(_e), mInter_ConfigHighPerformance(EInter_ConfigHighPerformance::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_ConfigHighPerformance> mInter_ConfigHighPerformance;

private:
    ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat process() override;
};

class IMG_GtModePopupListMy_stat : public BaseNode<ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat> {
public:
    explicit IMG_GtModePopupListMy_stat(
        ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat _e = ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat::MAX)
        : BaseNode(_e), mInter_ConfigHighPerformance(EInter_ConfigHighPerformance::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_ConfigHighPerformance> mInter_ConfigHighPerformance;

private:
    ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat process() override;
};

class IMG_DriveModeSelect_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModeSelect_stat> {
public:
    explicit IMG_DriveModeSelect_stat(
        ESFCDrive_ModeConstantIMG_DriveModeSelect_stat _e = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::MAX)
        : BaseNode(_e),
          mInter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModeSelect_stat process() override;
};

class IMG_TerrainModeSelect_stat : public BaseNode<ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat> {
public:
    explicit IMG_TerrainModeSelect_stat(
        ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat _e = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::MAX)
        : BaseNode(_e),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat process() override;
};

class IMG_NModeSelect_stat : public BaseNode<ESFCDrive_ModeConstantIMG_NModeSelect_stat> {
public:
    explicit IMG_NModeSelect_stat(ESFCDrive_ModeConstantIMG_NModeSelect_stat _e = ESFCDrive_ModeConstantIMG_NModeSelect_stat::MAX)
        : BaseNode(_e),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_NModeSelect_stat process() override;
};

class IMG_GtModeSelect_stat : public BaseNode<ESFCDrive_ModeConstantIMG_GtModeSelect_stat> {
public:
    explicit IMG_GtModeSelect_stat(
        ESFCDrive_ModeConstantIMG_GtModeSelect_stat _e = ESFCDrive_ModeConstantIMG_GtModeSelect_stat::MAX)
        : BaseNode(_e),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_GtModeSelect_stat process() override;
};

class IMG_DriveModePopupListNormal_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat> {
public:
    explicit IMG_DriveModePopupListNormal_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigNormalMode(EInter_ConfigNormalMode::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigNormalMode> mInter_ConfigNormalMode;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat process() override;
};

class IMG_DriveModePopupListEco_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat> {
public:
    explicit IMG_DriveModePopupListEco_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigEcoMode(EInter_ConfigEcoMode::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigEcoMode> mInter_ConfigEcoMode;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat process() override;
};

class IMG_DriveModePopupListSport_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat> {
public:
    explicit IMG_DriveModePopupListSport_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigSportMode(EInter_ConfigSportMode::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigSportMode> mInter_ConfigSportMode;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat process() override;
};

class IMG_DriveModePopupListChauffeur_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat> {
public:
    explicit IMG_DriveModePopupListChauffeur_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat _e =
                                                      ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigChauffeurMode(EInter_ConfigChauffeurMode::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigChauffeurMode> mInter_ConfigChauffeurMode;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat process() override;
};

class IMG_DriveModePopupListMyDrive_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat> {
public:
    explicit IMG_DriveModePopupListMyDrive_stat(ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat _e =
                                                    ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigCustomMyDrive(EInter_ConfigCustomMyDrive::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigCustomMyDrive> mInter_ConfigCustomMyDrive;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat process() override;
};

class IMG_DriveModePopupListSmart_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat> {
public:
    explicit IMG_DriveModePopupListSmart_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigCustomSmart(EInter_ConfigCustomSmart::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigCustomSmart> mInter_ConfigCustomSmart;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat process() override;
};

class IMG_DriveModePopupListSnow_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat> {
public:
    explicit IMG_DriveModePopupListSnow_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigCustomSnow(EInter_ConfigCustomSnow::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigCustomSnow> mInter_ConfigCustomSnow;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat process() override;
};

class IMG_DriveModeTowLongkeyQue_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat> {
public:
    explicit IMG_DriveModeTowLongkeyQue_stat(ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat _e =
                                                 ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat::NONE)
        : BaseNode(_e),
          mInter_ConfigTowModeLongKey(EInter_ConfigTowModeLongKey::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigTowModeLongKey> mInter_ConfigTowModeLongKey;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat process() override;
};

class IMG_DriveModePopupListType_stat : public BaseNode<ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat> {
public:
    explicit IMG_DriveModePopupListType_stat(
        ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat _e = ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat::MAX)
        : BaseNode(_e),
          mInter_ConfigEndlessDialSwitch(EInter_ConfigEndlessDialSwitch::OFF, [this]() { setNeedUpdate(); }),
          mIgnStatus(false, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_ConfigEndlessDialSwitch> mInter_ConfigEndlessDialSwitch;
    BaseNodeProp<bool> mIgnStatus;

private:
    ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat process() override;
};

class Private_DriveModeSwStatus : public BaseNode<EPrivate_DriveModeSwStatus> {
public:
    explicit Private_DriveModeSwStatus(EPrivate_DriveModeSwStatus _e = EPrivate_DriveModeSwStatus::MAX)
        : BaseNode(_e), mInter_DrvmodeSwitchStatus(EInter_DrvmodeSwitchStatus::DEFAULT, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_DrvmodeSwitchStatus> mInter_DrvmodeSwitchStatus;

private:
    EPrivate_DriveModeSwStatus process() override;
};

class EventDrivingModeChangeHandler : public EventBase {
public:
    EventDrivingModeChangeHandler() = default;
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    ESFCDrive_ModeInter_NModeSelectStatus mInter_NModeSelectStatus = ESFCDrive_ModeInter_NModeSelectStatus::MAX;
    ESFCDrive_ModeInter_NModeSelectStatus mPrevInter_NModeSelectStatus = ESFCDrive_ModeInter_NModeSelectStatus::MAX;
    EInter_TerrainModeSelectStatus mInter_TerrainModeSelectStatus = EInter_TerrainModeSelectStatus::MAX;
    EInter_TerrainModeSelectStatus mPrevInter_TerrainModeSelectStatus = EInter_TerrainModeSelectStatus::MAX;
    ESFCDrive_ModeInter_DriveModeSelectStatus mInter_DriveModeSelectStatus = ESFCDrive_ModeInter_DriveModeSelectStatus::MAX;
    ESFCDrive_ModeInter_DriveModeSelectStatus mPrevInter_DriveModeSelectStatus = ESFCDrive_ModeInter_DriveModeSelectStatus::MAX;
    EInter_NModeSwitchStatus mInter_NModeSwitchStatus = EInter_NModeSwitchStatus::DEFAULT;
    EInter_NModeSwitchStatus mPrevInter_NModeSwitchStatus = EInter_NModeSwitchStatus::DEFAULT;
    EInter_ConfigHighPerformance mInter_ConfigHighPerformance = EInter_ConfigHighPerformance::OFF;
    EInter_NModeOnReq mInter_NModeOnReq = EInter_NModeOnReq::DEFAULT;
    EInput_N1SwitchStatus mInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N2SwitchStatus mInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;
    EInput_RcsDriveModeRotateEndStatus mInput_RcsDriveModeRotateEndStatus = EInput_RcsDriveModeRotateEndStatus::OFF;
    EInput_RcsDriveModeRotateEndStatus mPrevInput_RcsDriveModeRotateEndStatus = EInput_RcsDriveModeRotateEndStatus::OFF;
    EInput_GtSwitchStatus mInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;
    ESFCDrive_ModeInter_GtModeSelectStatus mInter_GtModeSelectStatus = ESFCDrive_ModeInter_GtModeSelectStatus::MAX;
    ESFCDrive_ModeInter_GtModeSelectStatus mPrevInter_GtModeSelectStatus = ESFCDrive_ModeInter_GtModeSelectStatus::MAX;
    EInter_DriveModeOnRoadSwitchStatus mInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;
    EIMG_TelltaleTowMode_stat mIMG_TelltaleTowMode_stat = EIMG_TelltaleTowMode_stat::OFF;
    EIMG_TelltaleTowMode_stat mPrevIMG_TelltaleTowMode_stat = EIMG_TelltaleTowMode_stat::OFF;
    bool mIsSingleDriveModeEnabled = false;
    bool mIgnStatus = false;

    std::string process() override;
};

class EventTerrainModeExitNotiHandler : public EventBase {
public:
    EventTerrainModeExitNotiHandler() = default;
    EInter_TerrainModeSelectStatus mInter_TerrainModeSelectStatus = EInter_TerrainModeSelectStatus::MAX;
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInput_TerrainModeErrorStatus mPrevInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInter_DrvmodeSwitchStatus mInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    EInter_DrvmodeSwitchStatus mPrevInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat mIMG_DriveModePopupTitle_stat =
        ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::DRIVE_ONLY;
    EInter_DriveModeTerrainSwitchStatus mInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;
    bool mIgnStatus = false;

    std::string process() override;
};

class EventReconfirmEnterNCustomHandler : public EventBase {
public:
    EventReconfirmEnterNCustomHandler() = default;
    EInput_GearSelectStatusFromTCU mInput_GearSelectStatusFromTCU = EInput_GearSelectStatusFromTCU::INVALID;
    ESFCDrive_ModeInter_NModeSelectStatus mInter_NModeSelectStatus = ESFCDrive_ModeInter_NModeSelectStatus::MAX;
    EInput_N1SwitchMainSetStatus mInput_N1SwitchMainSetStatus = EInput_N1SwitchMainSetStatus::INVALID;
    EInput_N1SwitchNModeSetStatus mInput_N1SwitchNModeSetStatus = EInput_N1SwitchNModeSetStatus::OFF;
    EInput_N1SwitchCustom1ModeSetStatus mInput_N1SwitchCustom1ModeSetStatus = EInput_N1SwitchCustom1ModeSetStatus::OFF;
    EInput_N1SwitchCustom2ModeSetStatus mInput_N1SwitchCustom2ModeSetStatus = EInput_N1SwitchCustom2ModeSetStatus::OFF;
    EInput_EscCustom1EnableStatus mInput_EscCustom1EnableStatus = EInput_EscCustom1EnableStatus::NORMAL;
    EInput_N2SwitchMainSetStatus mInput_N2SwitchMainSetStatus = EInput_N2SwitchMainSetStatus::INVALID;
    EInput_N2SwitchNModeSetStatus mInput_N2SwitchNModeSetStatus = EInput_N2SwitchNModeSetStatus::OFF;
    EInput_N2SwitchCustom1ModeSetStatus mInput_N2SwitchCustom1ModeSetStatus = EInput_N2SwitchCustom1ModeSetStatus::OFF;
    EInput_N2SwitchCustom2ModeSetStatus mInput_N2SwitchCustom2ModeSetStatus = EInput_N2SwitchCustom2ModeSetStatus::OFF;
    EInput_EscCustom2EnableStatus mInput_EscCustom2EnableStatus = EInput_EscCustom2EnableStatus::NORMAL;
    EInput_N1SwitchStatus mInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N1SwitchStatus mPrevInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N2SwitchStatus mInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;
    EInput_N2SwitchStatus mPrevInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;

    bool mIgnStatus = false;

    std::string process() override;
};

class EventReconfirmEnterGT_MYHandler : public EventBase {
public:
    EventReconfirmEnterGT_MYHandler() = default;
    EInput_GearSelectStatusFromTCU mInput_GearSelectStatusFromTCU = EInput_GearSelectStatusFromTCU::INVALID;
    ESFCDrive_ModeInter_GtModeSelectStatus mInter_GtModeSelectStatus = ESFCDrive_ModeInter_GtModeSelectStatus::MAX;
    ESFCDrive_ModeInter_GtModeSelectStatus mPrevInter_GtModeSelectStatus = ESFCDrive_ModeInter_GtModeSelectStatus::MAX;
    EInput_EscCustom1EnableStatus mInput_EscCustom1EnableStatus = EInput_EscCustom1EnableStatus::NORMAL;
    EInput_GtSwitchStatus mInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;
    EInput_GtSwitchStatus mPrevInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;

    bool mIgnStatus = false;

    std::string process() override;
};

class EventModeSwitchingImpossibleIndiHandler : public EventBase {
public:
    EventModeSwitchingImpossibleIndiHandler() = default;
    EInput_GearSelectStatusFromTCU mInput_GearSelectStatusFromTCU = EInput_GearSelectStatusFromTCU::INVALID;
    EInput_N1SwitchMainSetStatus mInput_N1SwitchMainSetStatus = EInput_N1SwitchMainSetStatus::INVALID;
    EInput_N2SwitchMainSetStatus mInput_N2SwitchMainSetStatus = EInput_N2SwitchMainSetStatus::INVALID;
    EInput_N1SwitchStatus mInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N1SwitchStatus mPrevInput_N1SwitchStatus = EInput_N1SwitchStatus::DEFAULT;
    EInput_N2SwitchStatus mInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;
    EInput_N2SwitchStatus mPrevInput_N2SwitchStatus = EInput_N2SwitchStatus::DEFAULT;
    EPrivate_DriveModeSwStatus mPrivate_DriveModeSwStatus = EPrivate_DriveModeSwStatus::MAX;
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus = EPrivate_DriveModeSwStatus::MAX;
    EInter_NModeOnReq mInter_NModeOnReq = EInter_NModeOnReq::DEFAULT;
    EInput_GtSwitchStatus mInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;
    EInput_GtSwitchStatus mPrevInput_GtSwitchStatus = EInput_GtSwitchStatus::DEFAULT;
    EInter_DriveModeOnRoadSwitchStatus mInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;

    bool mIgnStatus = false;

    std::string process() override;
};

class EventAwd2hModeNotificationHandler : public EventBase {
public:
    EventAwd2hModeNotificationHandler() = default;
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInput_AwdMode2HStatus mInput_AwdMode2HStatus = EInput_AwdMode2HStatus::OFF;
    EInter_DrvmodeSwitchStatus mInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    EInter_DrvmodeSwitchStatus mPrevInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    EInter_DriveModeTerrainSwitchStatus mInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;

    bool mIgnStatus = false;

    std::string process() override;
};

class EventUnableToSwitchModeByTowModeHandler : public EventBase {
public:
    EventUnableToSwitchModeByTowModeHandler() = default;
    EIMG_TelltaleTowMode_stat mIMG_TelltaleTowMode_stat = EIMG_TelltaleTowMode_stat::OFF;
    EInput_AwdMode2HStatus mInput_AwdMode2HStatus = EInput_AwdMode2HStatus::OFF;
    EPrivate_DriveModeSwStatus mPrivate_DriveModeSwStatus = EPrivate_DriveModeSwStatus::DEFAULT;
    EPrivate_DriveModeSwStatus mPrevPrivate_DriveModeSwStatus = EPrivate_DriveModeSwStatus::DEFAULT;
    EInter_DriveModeOnRoadSwitchStatus mInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;
    EInter_DriveModeOnRoadSwitchStatus mPrevInter_DriveModeOnRoadSwitchStatus = EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED;
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInter_DrvmodeSwitchStatus mInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    EInter_DrvmodeSwitchStatus mPrevInter_DrvmodeSwitchStatus = EInter_DrvmodeSwitchStatus::DEFAULT;
    EInter_DriveModeTerrainSwitchStatus mInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;
    EInter_DriveModeTerrainSwitchStatus mPrevInter_DriveModeTerrainSwitchStatus = EInter_DriveModeTerrainSwitchStatus::UN_PRESSED;

    bool mIgnStatus = false;

    std::string process() override;
};

class Output_DriveModeSwitchStatus : public BaseNode<EOutputDriveModeSwitchStatus> {
public:
    explicit Output_DriveModeSwitchStatus(EOutputDriveModeSwitchStatus _e = EOutputDriveModeSwitchStatus::NORMAL)
        : BaseNode(_e, true),
          mInter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_NModeSelectStatus(ESFCDrive_ModeInter_NModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_NModeSelectStatus> mInter_NModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;

private:
    EOutputDriveModeSwitchStatus process() override;
};

class Output_TerrainModeSwitchStatus : public BaseNode<EOutputTerrainModeSwitchStatus> {
public:
    explicit Output_TerrainModeSwitchStatus(EOutputTerrainModeSwitchStatus _e = EOutputTerrainModeSwitchStatus::OFF)
        : BaseNode(_e, true), mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::MAX, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;

private:
    EOutputTerrainModeSwitchStatus process() override;
};

class Output_TerrainModeMainSwitchStatus : public BaseNode<EOutputTerrainModeMainSwitchStatus> {
public:
    explicit Output_TerrainModeMainSwitchStatus(
        EOutputTerrainModeMainSwitchStatus _e = EOutputTerrainModeMainSwitchStatus::ON_ROAD)
        : BaseNode(_e, true), mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::MAX, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;

private:
    EOutputTerrainModeMainSwitchStatus process() override;
};

class Output_CustomDriveModeSetStatus : public BaseNode<EOutputCustomDriveModeSetStatus> {
public:
    explicit Output_CustomDriveModeSetStatus(EOutputCustomDriveModeSetStatus _e = EOutputCustomDriveModeSetStatus::MY_DRIVE)
        : BaseNode(_e, true),
          mInter_DriveModeSelectStatus(ESFCDrive_ModeInter_DriveModeSelectStatus::MAX, [this]() { setNeedUpdate(); }),
          mInter_GtModeSelectStatus(ESFCDrive_ModeInter_GtModeSelectStatus::MAX, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<ESFCDrive_ModeInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<ESFCDrive_ModeInter_GtModeSelectStatus> mInter_GtModeSelectStatus;

private:
    EOutputCustomDriveModeSetStatus process() override;
};

class Output_CustomDriveModeApplyStatus : public BaseNode<EOutputCustomDriveModeApplyStatus> {
public:
    explicit Output_CustomDriveModeApplyStatus(EOutputCustomDriveModeApplyStatus _e = EOutputCustomDriveModeApplyStatus::OFF)
        : BaseNode(_e) {
    }

private:
    EOutputCustomDriveModeApplyStatus process() override;
};

}  // namespace ccos

#endif  // DRIVE_MODE_R000_DRIVE_MODETYPES_HPP
