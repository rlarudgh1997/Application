/**
 * @file Drive_Mode_2_0Types.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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

#ifndef SSFS_Drive_Mode_Drive_Mode_2_0Types_H
#define SSFS_Drive_Mode_Drive_Mode_2_0Types_H
#include <Vehicle.h>
#include <Timer.h>
#include "Drive_Mode_2_0Base.hpp"
#include "BaseNodeTemplate_2_0.hpp"

#include "CircularQueue_2_0.hpp"

#define DLOG_ENABLED gEnableSFCLog

namespace ccos {

using EInter_ConfigDriveModeOption = ccos::Drive_Mode_2_0Base::ArgumentsConfigDriveModeOptionChanged::Inter_ConfigDriveModeOption;
using EInter_ConfigDriveModeOptionWithVpc =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigDriveModeOptionWithVpcChanged::Inter_ConfigDriveModeOptionWithVpc;
using EInter_ConfigOipDriveMode_Eco = ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_Eco;
using EInter_ConfigOipDriveMode_Normal =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_Normal;
using EInter_ConfigOipDriveMode_Sport =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_Sport;
using EInter_ConfigOipDriveMode_My = ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_My;
using EInter_ConfigCustomSmart = ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigCustomSmart;
using EInter_ConfigOipDriveMode_Snow =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_Snow;
using EInter_ConfigOipDriveMode_Chauffeur =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_Chauffeur;
using EInter_ConfigOipDriveMode_SportPlus =
    ccos::Drive_Mode_2_0Base::ArgumentsConfigOipDriveModeChanged::Inter_ConfigOipDriveMode_SportPlus;
using EInput_DynamicControlForOipSwitchStatus =
    ccos::Drive_Mode_2_0Base::ArgumentsDynamicControlForOipSwitchStatusChanged::Input_DynamicControlForOipSwitchStatus;
using EInput_DmicTerrainModeMainSwitchStatus =
    ccos::Drive_Mode_2_0Base::ArgumentsDmicInfoChanged::Input_DmicTerrainModeMainSwitchStatus;
using EInput_DmicDriveModeSwitchStatus = ccos::Drive_Mode_2_0Base::ArgumentsDmicInfoChanged::Input_DmicDriveModeSwitchStatus;
using EInput_DmicCustomDriveModeSetStatus =
    ccos::Drive_Mode_2_0Base::ArgumentsDmicInfoChanged::Input_DmicCustomDriveModeSetStatus;
using EInput_DmicTerrainModeSwitchStatus = ccos::Drive_Mode_2_0Base::ArgumentsDmicInfoChanged::Input_DmicTerrainModeSwitchStatus;
using EInter_DriveModeChangeReqFromAVNT =
    ccos::Drive_Mode_2_0Base::ArgumentsDriveModeChangeReqFromAVNTChanged::Inter_DriveModeChangeReqFromAVNT;
using EInter_ConfigEcoMode = ccos::Drive_Mode_2_0Base::ArgumentsIgnOnOffKeepDriveModeStatusChanged::Inter_ConfigEcoMode;
using EInter_ConfigDriveModeKeepEco =
    ccos::Drive_Mode_2_0Base::ArgumentsIgnOnOffKeepDriveModeStatusChanged::Inter_ConfigDriveModeKeepEco;
using EInter_ConfigDriveModeKeepNormal =
    ccos::Drive_Mode_2_0Base::ArgumentsIgnOnOffKeepDriveModeStatusChanged::Inter_ConfigDriveModeKeepNormal;
using EInter_ConfigDriveModeKeepSport =
    ccos::Drive_Mode_2_0Base::ArgumentsIgnOnOffKeepDriveModeStatusChanged::Inter_ConfigDriveModeKeepSport;
using EInter_ConfigDriveModeDefault =
    ccos::Drive_Mode_2_0Base::ArgumentsIgnOnOffKeepDriveModeStatusChanged::Inter_ConfigDriveModeDefault;
using EInter_DriveModeSelectStatus = ccos::Drive_Mode_2_0Base::SFCDrive_Mode_2_0Inter_DriveModeSelectStatus;
using EInter_ConfigOffRoad = ccos::Drive_Mode_2_0Base::ArgumentsOffRoadModeEnableStatusChanged::Inter_ConfigOffRoad;
using EInput_TerrainModeErrorStatus =
    ccos::Drive_Mode_2_0Base::ArgumentsOffRoadModeEnableStatusChanged::Input_TerrainModeErrorStatus;
using EInter_ConfigTerrainSnow = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainSnow;
using EInter_ConfigTerrainMud = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainMud;
using EInter_ConfigTerrainSand = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainSand;
using EInter_ConfigTerrainAuto = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainAuto;
using EInter_ConfigTerrainDeepSnow = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainDeepSnow;
using EInter_ConfigTerrainRock = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainRock;
using EInter_ConfigTerrainDesert = ccos::Drive_Mode_2_0Base::ArgumentsConfigTerrainModeChanged::Inter_ConfigTerrainDesert;
using EInter_TerrainModeSelectStatus = ccos::Drive_Mode_2_0Base::SFCDrive_Mode_2_0Inter_TerrainModeSelectStatus;
using EOutputDriveModeSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode_2_0::VSMOutput_DriveModeSwitchStatus::Type;
using EOutputCustomDriveModeSetStatus = VSM::VSMCD_SKEL::VSMDrive_Mode_2_0::VSMOutput_CustomDriveModeSetStatus::Type;
using EOutputTerrainModeSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode_2_0::VSMOutput_TerrainModeSwitchStatus::Type;
using EOutputTerrainModeMainSwitchStatus = VSM::VSMCD_SKEL::VSMDrive_Mode_2_0::VSMOutput_TerrainModeMainSwitchStatus::Type;
using EInter_DynamicControlPopupStatus = ccos::Drive_Mode_2_0Base::SFCDrive_Mode_2_0Inter_DynamicControlPopupStatus;
using EInter_DriveModeChangeResultStatus = ccos::Drive_Mode_2_0Base::SFCDrive_Mode_2_0Inter_DriveModeChangeResultStatus;
using EInput_SmartDriveModeStatus = ccos::Drive_Mode_2_0Base::ArgumentsSmartDriveModeStatusChanged::Input_SmartDriveModeStatus;
using ETelltaleDriveModeStat = ccos::Drive_Mode_2_0Base::SFCDrive_Mode_2_0TelltaleDriveModeStatOptional;

enum class EPrivate_BasicModeSwitchableStatus {
    ENABLE,
    DISABLE
};

enum class EPrivate_BasicModeForSwKeySwitchableStatus {
    ENABLE,
    DISABLE
};

enum class EPrivate_BasicModeEcoAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_BasicModeNormalAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_BasicModeSportAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_BasicModeMyAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_BasicModeSmartAvailableStatus {
    AVAILABLE,
    NONE
};

enum class EPrivate_BasicModeSnowAvailableStatus {
    AVAILABLE,
    NONE
};

enum class EPrivate_BasicModeChauffeurAvailableStatus {
    AVAILABLE,
    NONE
};

enum class EPrivate_BasicModeSportPlusAvailableStatus {
    AVAILABLE,
    NONE
};

enum class EPrivate_BasicEvent {
    CW,
    OFF
};

enum class EPrivate_OffroadModeSwitchableStatus {
    ENABLE,
    DISABLE
};

enum class EPrivate_TerrainModeSnowAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeMudAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeSandAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeAutoAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeDeepSnowAvailableStatus {
    AVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeRockAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_TerrainModeDesertAvailableStatus {
    AVAILABLE,
    UNAVAILABLE,
    NONE
};

enum class EPrivate_OffroadEvent {
    CW,
    TERRAIN_CHANGE,
    OFF
};

class Private_BasicModeSwitchableStatus : public BaseNode<EPrivate_BasicModeSwitchableStatus> {
public:
    explicit Private_BasicModeSwitchableStatus(
        EPrivate_BasicModeSwitchableStatus _e = EPrivate_BasicModeSwitchableStatus::DISABLE)
        : BaseNode(_e),
          mInter_ConfigDriveModeOptionWithVpc(EInter_ConfigDriveModeOptionWithVpc::UNHANDLED_TIMEOUT,
                                              [this]() { setNeedUpdate(); }),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::NONE, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_ConfigDriveModeOptionWithVpc> mInter_ConfigDriveModeOptionWithVpc;
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;

private:
    EPrivate_BasicModeSwitchableStatus process() override;
};

class Private_BasicModeForSwKeySwitchableStatus : public BaseNode<EPrivate_BasicModeForSwKeySwitchableStatus> {
public:
    explicit Private_BasicModeForSwKeySwitchableStatus(
        EPrivate_BasicModeForSwKeySwitchableStatus _e = EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE)
        : BaseNode(_e), mInter_ConfigDriveModeOptionWithVpc(EInter_ConfigDriveModeOptionWithVpc::UNHANDLED_TIMEOUT, [this]() {
              setNeedUpdate();
          }) {
    }
    BaseNodeProp<EInter_ConfigDriveModeOptionWithVpc> mInter_ConfigDriveModeOptionWithVpc;

private:
    EPrivate_BasicModeForSwKeySwitchableStatus process() override;
};

class Private_BasicModeEcoAvailableStatus : public BaseNode<EPrivate_BasicModeEcoAvailableStatus> {
public:
    explicit Private_BasicModeEcoAvailableStatus(
        EPrivate_BasicModeEcoAvailableStatus _e = EPrivate_BasicModeEcoAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_Eco(EInter_ConfigOipDriveMode_Eco::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_Eco> mInter_ConfigOipDriveMode_Eco;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_BasicModeEcoAvailableStatus process() override;
};

class Private_BasicModeNormalAvailableStatus : public BaseNode<EPrivate_BasicModeNormalAvailableStatus> {
public:
    explicit Private_BasicModeNormalAvailableStatus(
        EPrivate_BasicModeNormalAvailableStatus _e = EPrivate_BasicModeNormalAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_Normal(EInter_ConfigOipDriveMode_Normal::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_Normal> mInter_ConfigOipDriveMode_Normal;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_BasicModeNormalAvailableStatus process() override;
};

class Private_BasicModeSportAvailableStatus : public BaseNode<EPrivate_BasicModeSportAvailableStatus> {
public:
    explicit Private_BasicModeSportAvailableStatus(
        EPrivate_BasicModeSportAvailableStatus _e = EPrivate_BasicModeSportAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_Sport(EInter_ConfigOipDriveMode_Sport::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_Sport> mInter_ConfigOipDriveMode_Sport;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_BasicModeSportAvailableStatus process() override;
};

class Private_BasicModeMyAvailableStatus : public BaseNode<EPrivate_BasicModeMyAvailableStatus> {
public:
    explicit Private_BasicModeMyAvailableStatus(
        EPrivate_BasicModeMyAvailableStatus _e = EPrivate_BasicModeMyAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_My(EInter_ConfigOipDriveMode_My::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_My> mInter_ConfigOipDriveMode_My;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_BasicModeMyAvailableStatus process() override;
};

class Private_BasicModeSmartAvailableStatus : public BaseNode<EPrivate_BasicModeSmartAvailableStatus> {
public:
    explicit Private_BasicModeSmartAvailableStatus(
        EPrivate_BasicModeSmartAvailableStatus _e = EPrivate_BasicModeSmartAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigCustomSmart(EInter_ConfigCustomSmart::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigCustomSmart> mInter_ConfigCustomSmart;

private:
    EPrivate_BasicModeSmartAvailableStatus process() override;
};

class Private_BasicModeSnowAvailableStatus : public BaseNode<EPrivate_BasicModeSnowAvailableStatus> {
public:
    explicit Private_BasicModeSnowAvailableStatus(
        EPrivate_BasicModeSnowAvailableStatus _e = EPrivate_BasicModeSnowAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_Snow(EInter_ConfigOipDriveMode_Snow::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_Snow> mInter_ConfigOipDriveMode_Snow;

private:
    EPrivate_BasicModeSnowAvailableStatus process() override;
};

class Private_BasicModeChauffeurAvailableStatus : public BaseNode<EPrivate_BasicModeChauffeurAvailableStatus> {
public:
    explicit Private_BasicModeChauffeurAvailableStatus(
        EPrivate_BasicModeChauffeurAvailableStatus _e = EPrivate_BasicModeChauffeurAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_Chauffeur(EInter_ConfigOipDriveMode_Chauffeur::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_Chauffeur> mInter_ConfigOipDriveMode_Chauffeur;

private:
    EPrivate_BasicModeChauffeurAvailableStatus process() override;
};

class Private_BasicModeSportPlusAvailableStatus : public BaseNode<EPrivate_BasicModeSportPlusAvailableStatus> {
public:
    explicit Private_BasicModeSportPlusAvailableStatus(
        EPrivate_BasicModeSportPlusAvailableStatus _e = EPrivate_BasicModeSportPlusAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigOipDriveMode_SportPlus(EInter_ConfigOipDriveMode_SportPlus::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigOipDriveMode_SportPlus> mInter_ConfigOipDriveMode_SportPlus;

private:
    EPrivate_BasicModeSportPlusAvailableStatus process() override;
};

class Inter_DriveModeSelectStatus : public BaseNode<EInter_DriveModeSelectStatus> {
public:
    explicit Inter_DriveModeSelectStatus(EInter_DriveModeSelectStatus _e = EInter_DriveModeSelectStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeList({EInter_DriveModeSelectStatus::ECO, EInter_DriveModeSelectStatus::NORMAL,
                                  EInter_DriveModeSelectStatus::SPORT, EInter_DriveModeSelectStatus::SPORTPLUS,
                                  EInter_DriveModeSelectStatus::CHAUFFEUR, EInter_DriveModeSelectStatus::MY,
                                  EInter_DriveModeSelectStatus::SMART, EInter_DriveModeSelectStatus::SNOW}),
          mPrivate_BasicModeEcoAvailableStatus(EPrivate_BasicModeEcoAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeNormalAvailableStatus(EPrivate_BasicModeNormalAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSportAvailableStatus(EPrivate_BasicModeSportAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSportPlusAvailableStatus(EPrivate_BasicModeSportPlusAvailableStatus::NONE,
                                                     [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeChauffeurAvailableStatus(EPrivate_BasicModeChauffeurAvailableStatus::NONE,
                                                     [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeMyAvailableStatus(EPrivate_BasicModeMyAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSmartAvailableStatus(EPrivate_BasicModeSmartAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSnowAvailableStatus(EPrivate_BasicModeSnowAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeOptionWithVpc(EInter_ConfigDriveModeOptionWithVpc::UNHANDLED_TIMEOUT,
                                              [this]() { setNeedUpdate(); }),
          mInput_DmicTerrainModeMainSwitchStatus(EInput_DmicTerrainModeMainSwitchStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_DmicDriveModeSwitchStatus(EInput_DmicDriveModeSwitchStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_DmicCustomDriveModeSetStatus(EInput_DmicCustomDriveModeSetStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_DynamicControlForOipSwitchStatus(EInput_DynamicControlForOipSwitchStatus::UN_PRESSED,
                                                  [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeSwitchableStatus(EPrivate_BasicModeSwitchableStatus::DISABLE, [this]() { setNeedUpdate(); }),
          mPrivate_BasicModeForSwKeySwitchableStatus(EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE,
                                                     [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::NONE, [this]() { setNeedUpdate(); }),
          mInter_ConfigEcoMode(EInter_ConfigEcoMode::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepEco(EInter_ConfigDriveModeKeepEco::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepNormal(EInter_ConfigDriveModeKeepNormal::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeKeepSport(EInter_ConfigDriveModeKeepSport::OFF, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeDefault(EInter_ConfigDriveModeDefault::NORMAL, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    ModeCircularQueue<EInter_DriveModeSelectStatus> mPrivate_BasicModeList;
    BaseNodeProp<EPrivate_BasicModeEcoAvailableStatus> mPrivate_BasicModeEcoAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeNormalAvailableStatus> mPrivate_BasicModeNormalAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeSportAvailableStatus> mPrivate_BasicModeSportAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeSportPlusAvailableStatus> mPrivate_BasicModeSportPlusAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeChauffeurAvailableStatus> mPrivate_BasicModeChauffeurAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeMyAvailableStatus> mPrivate_BasicModeMyAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeSmartAvailableStatus> mPrivate_BasicModeSmartAvailableStatus;
    BaseNodeProp<EPrivate_BasicModeSnowAvailableStatus> mPrivate_BasicModeSnowAvailableStatus;
    BaseNodeProp<EInter_ConfigDriveModeOptionWithVpc> mInter_ConfigDriveModeOptionWithVpc;
    BaseNodeProp<EInput_DmicTerrainModeMainSwitchStatus> mInput_DmicTerrainModeMainSwitchStatus;
    BaseNodeProp<EInput_DmicDriveModeSwitchStatus> mInput_DmicDriveModeSwitchStatus;
    BaseNodeProp<EInput_DmicCustomDriveModeSetStatus> mInput_DmicCustomDriveModeSetStatus;
    BaseNodeProp<EInput_DynamicControlForOipSwitchStatus> mInput_DynamicControlForOipSwitchStatus;
    BaseNodeProp<EPrivate_BasicModeSwitchableStatus> mPrivate_BasicModeSwitchableStatus;
    BaseNodeProp<EPrivate_BasicModeForSwKeySwitchableStatus> mPrivate_BasicModeForSwKeySwitchableStatus;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    BaseNodeProp<EInter_ConfigEcoMode> mInter_ConfigEcoMode;
    BaseNodeProp<EInter_ConfigDriveModeKeepEco> mInter_ConfigDriveModeKeepEco;
    BaseNodeProp<EInter_ConfigDriveModeKeepNormal> mInter_ConfigDriveModeKeepNormal;
    BaseNodeProp<EInter_ConfigDriveModeKeepSport> mInter_ConfigDriveModeKeepSport;
    BaseNodeProp<EInter_ConfigDriveModeDefault> mInter_ConfigDriveModeDefault;
    EPrivate_BasicEvent mPrivate_BasicEvent = EPrivate_BasicEvent::OFF;
    bool mIsDriveModeChangeRequestedFromAVNT = false;

private:
    bool mPrevIgnStatus = false;
    EPrivate_BasicModeEcoAvailableStatus mPrevPrivate_BasicModeEcoAvailableStatus = EPrivate_BasicModeEcoAvailableStatus::NONE;
    EPrivate_BasicModeNormalAvailableStatus mPrevPrivate_BasicModeNormalAvailableStatus =
        EPrivate_BasicModeNormalAvailableStatus::NONE;
    EPrivate_BasicModeSportAvailableStatus mPrevPrivate_BasicModeSportAvailableStatus =
        EPrivate_BasicModeSportAvailableStatus::NONE;
    EPrivate_BasicModeMyAvailableStatus mPrevPrivate_BasicModeMyAvailableStatus = EPrivate_BasicModeMyAvailableStatus::NONE;
    EPrivate_BasicModeSmartAvailableStatus mPrevPrivate_BasicModeSmartAvailableStatus =
        EPrivate_BasicModeSmartAvailableStatus::NONE;
    EPrivate_BasicModeSnowAvailableStatus mPrevPrivate_BasicModeSnowAvailableStatus = EPrivate_BasicModeSnowAvailableStatus::NONE;
    EPrivate_BasicModeChauffeurAvailableStatus mPrevPrivate_BasicModeChauffeurAvailableStatus =
        EPrivate_BasicModeChauffeurAvailableStatus::NONE;
    EPrivate_BasicModeSportPlusAvailableStatus mPrevPrivate_BasicModeSportPlusAvailableStatus =
        EPrivate_BasicModeSportPlusAvailableStatus::NONE;
    EInput_DynamicControlForOipSwitchStatus mPrevInput_DynamicControlForOipSwitchStatus =
        EInput_DynamicControlForOipSwitchStatus::UN_PRESSED;
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;
    EInter_DriveModeSelectStatus process() override;
};

class Private_TerrainModeSnowAvailableStatus : public BaseNode<EPrivate_TerrainModeSnowAvailableStatus> {
public:
    explicit Private_TerrainModeSnowAvailableStatus(
        EPrivate_TerrainModeSnowAvailableStatus _e = EPrivate_TerrainModeSnowAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainSnow(EInter_ConfigTerrainSnow::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainSnow> mInter_ConfigTerrainSnow;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeSnowAvailableStatus process() override;
};

class Private_TerrainModeMudAvailableStatus : public BaseNode<EPrivate_TerrainModeMudAvailableStatus> {
public:
    explicit Private_TerrainModeMudAvailableStatus(
        EPrivate_TerrainModeMudAvailableStatus _e = EPrivate_TerrainModeMudAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainMud(EInter_ConfigTerrainMud::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainMud> mInter_ConfigTerrainMud;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeMudAvailableStatus process() override;
};

class Private_TerrainModeSandAvailableStatus : public BaseNode<EPrivate_TerrainModeSandAvailableStatus> {
public:
    explicit Private_TerrainModeSandAvailableStatus(
        EPrivate_TerrainModeSandAvailableStatus _e = EPrivate_TerrainModeSandAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainSand(EInter_ConfigTerrainSand::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainSand> mInter_ConfigTerrainSand;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeSandAvailableStatus process() override;
};

class Private_TerrainModeAutoAvailableStatus : public BaseNode<EPrivate_TerrainModeAutoAvailableStatus> {
public:
    explicit Private_TerrainModeAutoAvailableStatus(
        EPrivate_TerrainModeAutoAvailableStatus _e = EPrivate_TerrainModeAutoAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainAuto(EInter_ConfigTerrainAuto::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainAuto> mInter_ConfigTerrainAuto;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeAutoAvailableStatus process() override;
};

class Private_TerrainModeDeepSnowAvailableStatus : public BaseNode<EPrivate_TerrainModeDeepSnowAvailableStatus> {
public:
    explicit Private_TerrainModeDeepSnowAvailableStatus(
        EPrivate_TerrainModeDeepSnowAvailableStatus _e = EPrivate_TerrainModeDeepSnowAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainDeepSnow(EInter_ConfigTerrainDeepSnow::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainDeepSnow> mInter_ConfigTerrainDeepSnow;

private:
    EPrivate_TerrainModeDeepSnowAvailableStatus process() override;
};

class Private_TerrainModeRockAvailableStatus : public BaseNode<EPrivate_TerrainModeRockAvailableStatus> {
public:
    explicit Private_TerrainModeRockAvailableStatus(
        EPrivate_TerrainModeRockAvailableStatus _e = EPrivate_TerrainModeRockAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainRock(EInter_ConfigTerrainRock::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainRock> mInter_ConfigTerrainRock;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeRockAvailableStatus process() override;
};

class Private_TerrainModeDesertAvailableStatus : public BaseNode<EPrivate_TerrainModeDesertAvailableStatus> {
public:
    explicit Private_TerrainModeDesertAvailableStatus(
        EPrivate_TerrainModeDesertAvailableStatus _e = EPrivate_TerrainModeDesertAvailableStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigTerrainDesert(EInter_ConfigTerrainDesert::OFF, [this]() { setNeedUpdate(); }),
          mInput_AwdModeDriveModeControlStatus(true, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigTerrainDesert> mInter_ConfigTerrainDesert;
    BaseNodeProp<bool> mInput_AwdModeDriveModeControlStatus;

private:
    EPrivate_TerrainModeDesertAvailableStatus process() override;
};

class Private_OffroadEvent : public BaseNode<EPrivate_OffroadEvent> {
public:
    explicit Private_OffroadEvent(EPrivate_OffroadEvent _e = EPrivate_OffroadEvent::OFF)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInput_DynamicControlForOipSwitchStatus(EInput_DynamicControlForOipSwitchStatus::UN_PRESSED,
                                                  [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInput_DynamicControlForOipSwitchStatus> mInput_DynamicControlForOipSwitchStatus;

private:
    EInput_DynamicControlForOipSwitchStatus mPrevInput_DynamicControlForOipSwitchStatus =
        EInput_DynamicControlForOipSwitchStatus::UN_PRESSED;
    EPrivate_OffroadEvent process() override;
};

class Inter_TerrainModeSelectStatus : public BaseNode<EInter_TerrainModeSelectStatus> {
public:
    explicit Inter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus _e = EInter_TerrainModeSelectStatus::NONE)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeOptionWithVpc(EInter_ConfigDriveModeOptionWithVpc::UNHANDLED_TIMEOUT,
                                              [this]() { setNeedUpdate(); }),
          mInter_ConfigOffRoad(EInter_ConfigOffRoad::OFF, [this]() { setNeedUpdate(); }),
          mInput_TerrainModeErrorStatus(EInput_TerrainModeErrorStatus::NORMAL, [this]() { setNeedUpdate(); }),
          mPrivate_OffroadModeList({EInter_TerrainModeSelectStatus::AUTO, EInter_TerrainModeSelectStatus::SNOW,
                                    EInter_TerrainModeSelectStatus::DEEP_SNOW, EInter_TerrainModeSelectStatus::MUD,
                                    EInter_TerrainModeSelectStatus::SAND, EInter_TerrainModeSelectStatus::DESERT,
                                    EInter_TerrainModeSelectStatus::ROCK}),
          mPrivate_TerrainModeSnowAvailableStatus(EPrivate_TerrainModeSnowAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeMudAvailableStatus(EPrivate_TerrainModeMudAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeSandAvailableStatus(EPrivate_TerrainModeSandAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeAutoAvailableStatus(EPrivate_TerrainModeAutoAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeDeepSnowAvailableStatus(EPrivate_TerrainModeDeepSnowAvailableStatus::NONE,
                                                      [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeDesertAvailableStatus(EPrivate_TerrainModeDesertAvailableStatus::NONE,
                                                    [this]() { setNeedUpdate(); }),
          mPrivate_TerrainModeRockAvailableStatus(EPrivate_TerrainModeRockAvailableStatus::NONE, [this]() { setNeedUpdate(); }),
          mInput_DmicTerrainModeMainSwitchStatus(EInput_DmicTerrainModeMainSwitchStatus::OFF, [this]() { setNeedUpdate(); }),
          mInput_DmicTerrainModeSwitchStatus(EInput_DmicTerrainModeSwitchStatus::OFF, [this]() { setNeedUpdate(); }),
          mPrivate_OffroadEvent(EPrivate_OffroadEvent::OFF, [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::NONE, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigDriveModeOptionWithVpc> mInter_ConfigDriveModeOptionWithVpc;
    BaseNodeProp<EInter_ConfigOffRoad> mInter_ConfigOffRoad;
    BaseNodeProp<EInput_TerrainModeErrorStatus> mInput_TerrainModeErrorStatus;
    ModeCircularQueue<EInter_TerrainModeSelectStatus> mPrivate_OffroadModeList;
    BaseNodeProp<EPrivate_TerrainModeSnowAvailableStatus> mPrivate_TerrainModeSnowAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeMudAvailableStatus> mPrivate_TerrainModeMudAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeSandAvailableStatus> mPrivate_TerrainModeSandAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeAutoAvailableStatus> mPrivate_TerrainModeAutoAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeDeepSnowAvailableStatus> mPrivate_TerrainModeDeepSnowAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeDesertAvailableStatus> mPrivate_TerrainModeDesertAvailableStatus;
    BaseNodeProp<EPrivate_TerrainModeRockAvailableStatus> mPrivate_TerrainModeRockAvailableStatus;
    BaseNodeProp<EInput_DmicTerrainModeMainSwitchStatus> mInput_DmicTerrainModeMainSwitchStatus;
    BaseNodeProp<EInput_DmicTerrainModeSwitchStatus> mInput_DmicTerrainModeSwitchStatus;
    BaseNodeProp<EPrivate_OffroadEvent> mPrivate_OffroadEvent;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    EPrivate_OffroadModeSwitchableStatus mPrivate_OffroadModeSwitchableStatus = EPrivate_OffroadModeSwitchableStatus::DISABLE;

    static constexpr uint32_t kTimerIntervalNormalToErrorForMs = 500;
    uint32_t mTimerIdNormalToErrorFor500ms = 0;

    bool onTimerNormalToError() {
        if (mCallback != nullptr) {
            BaseNode<EInter_TerrainModeSelectStatus>::operator=(EInter_TerrainModeSelectStatus::OFF);
            mCallback();
        }
        mTimerIdNormalToErrorFor500ms = 0;
        return false;
    }

    static constexpr uint32_t kTimerIntervalOffToReadyForMs = 4000;
    uint32_t mTimerIdOffToReadyFor4000ms = 0;

    bool onTimerOffToReady() {
        if (mCallback != nullptr) {
            BaseNode<EInter_TerrainModeSelectStatus>::operator=(EInter_TerrainModeSelectStatus::OFF);
            mCallback();
        }
        mTimerIdOffToReadyFor4000ms = 0;
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

    bool mIsTerrainModeChangeRequestedFromAVNT = false;

private:
    bool mPrevIgnStatus = false;
    EPrivate_TerrainModeSnowAvailableStatus mPrevPrivate_TerrainModeSnowAvailableStatus =
        EPrivate_TerrainModeSnowAvailableStatus::NONE;
    EPrivate_TerrainModeMudAvailableStatus mPrevPrivate_TerrainModeMudAvailableStatus =
        EPrivate_TerrainModeMudAvailableStatus::NONE;
    EPrivate_TerrainModeSandAvailableStatus mPrevPrivate_TerrainModeSandAvailableStatus =
        EPrivate_TerrainModeSandAvailableStatus::NONE;
    EPrivate_TerrainModeAutoAvailableStatus mPrevPrivate_TerrainModeAutoAvailableStatus =
        EPrivate_TerrainModeAutoAvailableStatus::NONE;
    EPrivate_TerrainModeDeepSnowAvailableStatus mPrevPrivate_TerrainModeDeepSnowAvailableStatus =
        EPrivate_TerrainModeDeepSnowAvailableStatus::NONE;
    EPrivate_TerrainModeRockAvailableStatus mPrevPrivate_TerrainModeRockAvailableStatus =
        EPrivate_TerrainModeRockAvailableStatus::NONE;
    EPrivate_TerrainModeDesertAvailableStatus mPrevPrivate_TerrainModeDesertAvailableStatus =
        EPrivate_TerrainModeDesertAvailableStatus::NONE;
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;
    EInput_TerrainModeErrorStatus mPrevInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInter_TerrainModeSelectStatus process() override;
};

class Output_DriveModeSwitchStatus : public BaseNode<EOutputDriveModeSwitchStatus> {
public:
    explicit Output_DriveModeSwitchStatus(EOutputDriveModeSwitchStatus _e = EOutputDriveModeSwitchStatus::NORMAL)
        : BaseNode(_e, true), mInter_DriveModeSelectStatus(EInter_DriveModeSelectStatus::NORMAL, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;

private:
    EOutputDriveModeSwitchStatus process() override;
};

class Output_CustomDriveModeSetStatus : public BaseNode<EOutputCustomDriveModeSetStatus> {
public:
    explicit Output_CustomDriveModeSetStatus(EOutputCustomDriveModeSetStatus _e = EOutputCustomDriveModeSetStatus::INVALID)
        : BaseNode(_e, true), mInter_DriveModeSelectStatus(EInter_DriveModeSelectStatus::NORMAL, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;

private:
    EOutputCustomDriveModeSetStatus process() override;
};

class Output_TerrainModeSwitchStatus : public BaseNode<EOutputTerrainModeSwitchStatus> {
public:
    explicit Output_TerrainModeSwitchStatus(EOutputTerrainModeSwitchStatus _e = EOutputTerrainModeSwitchStatus::OFF)
        : BaseNode(_e, true), mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;

private:
    EOutputTerrainModeSwitchStatus process() override;
};

class Output_TerrainModeMainSwitchStatus : public BaseNode<EOutputTerrainModeMainSwitchStatus> {
public:
    explicit Output_TerrainModeMainSwitchStatus(
        EOutputTerrainModeMainSwitchStatus _e = EOutputTerrainModeMainSwitchStatus::ON_ROAD)
        : BaseNode(_e, true), mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::OFF, [this]() { setNeedUpdate(); }) {
    }
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;

private:
    EOutputTerrainModeMainSwitchStatus process() override;
};

class Inter_DynamicControlPopupStatus : public BaseNode<EInter_DynamicControlPopupStatus> {
public:
    explicit Inter_DynamicControlPopupStatus(
        EInter_DynamicControlPopupStatus _e = EInter_DynamicControlPopupStatus::SIGNATURE_NULL)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_ConfigDriveModeOptionWithVpc(EInter_ConfigDriveModeOptionWithVpc::UNHANDLED_TIMEOUT,
                                              [this]() { setNeedUpdate(); }),
          mInter_ConfigOffRoad(EInter_ConfigOffRoad::OFF, [this]() { setNeedUpdate(); }),
          mInput_TerrainModeErrorStatus(EInput_TerrainModeErrorStatus::NORMAL, [this]() { setNeedUpdate(); }),
          mPrivate_OffroadEvent(EPrivate_OffroadEvent::OFF, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_ConfigDriveModeOptionWithVpc> mInter_ConfigDriveModeOptionWithVpc;
    BaseNodeProp<EInter_ConfigOffRoad> mInter_ConfigOffRoad;
    BaseNodeProp<EInput_TerrainModeErrorStatus> mInput_TerrainModeErrorStatus;
    BaseNodeProp<EPrivate_OffroadEvent> mPrivate_OffroadEvent;
    static constexpr uint32_t kTimerInterval4SecAfterValueChanged = 4000;
    uint32_t mTimerId4SecAfterValueChanged = 0;

    bool onTimer4SecAfterValueChanged() {
        if (mCallback != nullptr) {
            BaseNode<EInter_DynamicControlPopupStatus>::operator=(EInter_DynamicControlPopupStatus::SIGNATURE_NULL);
            mCallback();
        }
        mTimerId4SecAfterValueChanged = 0;
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
    EInput_TerrainModeErrorStatus mPrevInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInter_DynamicControlPopupStatus process() override;
};

class Inter_DriveModeChangeResultStatus : public BaseNode<EInter_DriveModeChangeResultStatus> {
public:
    explicit Inter_DriveModeChangeResultStatus(
        EInter_DriveModeChangeResultStatus _e = EInter_DriveModeChangeResultStatus::INVALID_OPERATION)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_DriveModeSelectStatus(EInter_DriveModeSelectStatus::NONE, [this]() { setNeedUpdate(); }),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::NONE, [this]() { setNeedUpdate(); }),
          mInter_DriveModeChangeReqFromAVNT(EInter_DriveModeChangeReqFromAVNT::SIGNATURE_NULL, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;
    BaseNodeProp<EInter_DriveModeChangeReqFromAVNT> mInter_DriveModeChangeReqFromAVNT;
    bool mIsDriveModeChangeRequestedFromAVNT = false;
    bool mIsTerrainModeChangeRequestedFromAVNT = false;
    bool mIsForcedFlush = false;

private:
    EInter_DriveModeSelectStatus mPrevInter_DriveModeSelectStatus = EInter_DriveModeSelectStatus::NONE;
    EInter_TerrainModeSelectStatus mPrevInter_TerrainModeSelectStatus = EInter_TerrainModeSelectStatus::NONE;
    EInter_DriveModeChangeReqFromAVNT mPrevInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::SIGNATURE_NULL;
    EInter_DriveModeChangeResultStatus process() override;
};

class TelltaleDriveModeStat : public BaseNode<ETelltaleDriveModeStat> {
public:
    explicit TelltaleDriveModeStat(ETelltaleDriveModeStat _e = ETelltaleDriveModeStat::MAX)
        : BaseNode(_e),
          mIgnStatus(false, [this]() { setNeedUpdate(); }),
          mInter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::NONE, [this]() { setNeedUpdate(); }),
          mInter_DriveModeSelectStatus(EInter_DriveModeSelectStatus::NONE, [this]() { setNeedUpdate(); }),
          mInput_SmartDriveModeStatus(EInput_SmartDriveModeStatus::INACTIVE, [this]() { setNeedUpdate(); }) {
    }

    BaseNodeProp<bool> mIgnStatus;
    BaseNodeProp<EInter_TerrainModeSelectStatus> mInter_TerrainModeSelectStatus;
    BaseNodeProp<EInter_DriveModeSelectStatus> mInter_DriveModeSelectStatus;
    BaseNodeProp<EInput_SmartDriveModeStatus> mInput_SmartDriveModeStatus;

private:
    ETelltaleDriveModeStat process() override;
};

}  // namespace ccos

#endif  // DRIVE_MODE_R000_DRIVE_MODE_2_0TYPES_HPP
