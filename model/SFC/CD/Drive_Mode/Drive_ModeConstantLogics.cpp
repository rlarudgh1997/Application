/**
 * @file Drive_ModeConstantLogics.cpp
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

#include "Drive_ModeTypes.hpp"

namespace ccos {
// 5.2.1 드라이브 모드 팝업 타이틀
ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat IMG_DriveModePopupTitle_stat::process() {
    // Note(andre) : No need for ign
    ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::DRIVE_ONLY;
    if (mInter_ConfigOffRoad == EInter_ConfigOffRoad::TERRAIN) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TERRAIN;
    } else if (mInter_ConfigOffRoad == EInter_ConfigOffRoad::TRACTION) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TRACTION;
    } else {
        // DRIVE_ONLY
    }
    DDebug() << "[IMG_DriveModePopupTitle_stat] SPEC 5.2.5.2.1";
    DDebug() << "[IMG_DriveModePopupTitle_stat] Inter_ConfigOffRoad: " << static_cast<uint64_t>(mInter_ConfigOffRoad.selfValue());
    DDebug() << "[IMG_DriveModePopupTitle_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.2 드라이브 모드 리스트 항목별 표시 여부
// 1) IMG_DriveModePopupListNormal_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat IMG_DriveModePopupListNormal_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigNormalMode == EInter_ConfigNormalMode::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::UNAVAILABLE;
        }
    }
    return ret;
}
// 2) IMG_DriveModePopupListEco_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat IMG_DriveModePopupListEco_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::UNAVAILABLE;
        }
    }
    return ret;
}
// 3) IMG_DriveModePopupListSport_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat IMG_DriveModePopupListSport_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigSportMode == EInter_ConfigSportMode::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::UNAVAILABLE;
        }
    }
    return ret;
}
// 4) IMG_DriveModePopupListChauffeur_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat IMG_DriveModePopupListChauffeur_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat ret =
        ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigChauffeurMode == EInter_ConfigChauffeurMode::ON) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::AVAILABLE;
    }
    return ret;
}
// 5) IMG_DriveModePopupListMyDrive_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat IMG_DriveModePopupListMyDrive_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigCustomMyDrive == EInter_ConfigCustomMyDrive::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::UNAVAILABLE;
        }
    }
    return ret;
}
// 6) IMG_DriveModePopupListSmart_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat IMG_DriveModePopupListSmart_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigCustomSmart == EInter_ConfigCustomSmart::ON) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::AVAILABLE;
    }
    return ret;
}
// 7) IMG_DriveModePopupListSnow_stat
ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat IMG_DriveModePopupListSnow_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigCustomSnow == EInter_ConfigCustomSnow::ON) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::AVAILABLE;
    }
    return ret;
}

ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat IMG_DriveModeTowLongkeyQue_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat ret = ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTowModeLongKey == EInter_ConfigTowModeLongKey::ON) {
        ret = ESFCDrive_ModeConstantIMG_DriveModeTowLongkeyQueue_stat::AVAILABLE;
    }
    return ret;
}

// 5.2.3 드라이브 모드 리스트 순서 타입
ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat IMG_DriveModePopupListType_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat ret = ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat::TYPE1;
    if (mIgnStatus == true && mInter_ConfigEndlessDialSwitch == EInter_ConfigEndlessDialSwitch::ON) {
        ret = ESFCDrive_ModeConstantIMG_DriveModePopupListType_stat::TYPE2;
    }
    return ret;
}

// 5.2.4 터레인 모드 리스트 타입
// 1) IMG_TerrainModePopupListSnow_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat IMG_TerrainModePopupListSnow_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainSnow == EInter_ConfigTerrainSnow::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListSnow_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListSnow_stat] Inter_ConfigTerrainSnow: "
             << static_cast<uint64_t>(mInter_ConfigTerrainSnow.selfValue());
    DDebug() << "[IMG_TerrainModePopupListSnow_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListSnow_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 2) IMG_TerrainModePopupListMud_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat IMG_TerrainModePopupListMud_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainMud == EInter_ConfigTerrainMud::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListMud_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListMud_stat] Inter_ConfigTerrainMud: "
             << static_cast<uint64_t>(mInter_ConfigTerrainMud.selfValue());
    DDebug() << "[IMG_TerrainModePopupListMud_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListMud_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 3) IMG_TerrainModePopupListSand_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat IMG_TerrainModePopupListSand_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainSand == EInter_ConfigTerrainSand::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListSand_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListSand_stat] Inter_ConfigTerrainSand: "
             << static_cast<uint64_t>(mInter_ConfigTerrainSand.selfValue());
    DDebug() << "[IMG_TerrainModePopupListSand_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListSand_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 4) IMG_TerrainModePopupListAuto_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat IMG_TerrainModePopupListAuto_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainAuto == EInter_ConfigTerrainAuto::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListAuto_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListAuto_stat] Inter_ConfigTerrainAuto: "
             << static_cast<uint64_t>(mInter_ConfigTerrainAuto.selfValue());
    DDebug() << "[IMG_TerrainModePopupListAuto_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListAuto_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5) IMG_TerrainModePopupListDeepSnow_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat IMG_TerrainModePopupListDeepSnow_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat ret =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainDeepSnow == EInter_ConfigTerrainDeepSnow::ON) {
        ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::AVAILABLE;
    }
    DDebug() << "[IMG_TerrainModePopupListDeepSnow_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListDeepSnow_stat] Inter_ConfigTerrainDeepSnow: "
             << static_cast<uint64_t>(mInter_ConfigTerrainDeepSnow.selfValue());
    DDebug() << "[IMG_TerrainModePopupListDeepSnow_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListDeepSnow_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 6) IMG_TerrainModePopupListRock_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat IMG_TerrainModePopupListRock_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainRock == EInter_ConfigTerrainRock::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListRock_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListRock_stat] Inter_ConfigTerrainRock: "
             << static_cast<uint64_t>(mInter_ConfigTerrainRock.selfValue());
    DDebug() << "[IMG_TerrainModePopupListRock_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListRock_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 7) IMG_TerrainModePopupListDesert_stat
ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat IMG_TerrainModePopupListDesert_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat ret =
        ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainDesert == EInter_ConfigTerrainDesert::ON) {
        if (mInput_AwdModeDriveModeControlStatus == false) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::AVAILABLE;
        } else {
            ret = ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::UNAVAILABLE;
        }
    }
    DDebug() << "[IMG_TerrainModePopupListDesert_stat] SPEC 5.2.5.2.4";
    DDebug() << "[IMG_TerrainModePopupListDesert_stat] Inter_ConfigTerrainDesert: "
             << static_cast<uint64_t>(mInter_ConfigTerrainDesert.selfValue());
    DDebug() << "[IMG_TerrainModePopupListDesert_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModePopupListDesert_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.5 N1모드 리스트 타입
// 1) IMG_N1ModePopupListN_stat
ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat IMG_N1ModePopupListN_stat::process() {
    ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat ret = ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat::NONE;
    if (mIgnStatus == true && mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N1ModePopupListN_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N1ModePopupListN_stat] SPEC 5.2.5.2.5 N1 Mode List Type";
    DDebug() << "[IMG_N1ModePopupListN_stat] Input_N1SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchNModeSetStatus.selfValue());
    DDebug() << "[IMG_N1ModePopupListN_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N1ModePopupListN_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 2) IMG_N1ModePopupListCustom1_stat
ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat IMG_N1ModePopupListCustom1_stat::process() {
    ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat ret = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat::NONE;
    if (mIgnStatus == true && mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom1_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N1ModePopupListCustom1_stat] SPEC 5.2.5.2.5 N1 Mode List Type";
    DDebug() << "[IMG_N1ModePopupListCustom1_stat] Input_N1SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom1ModeSetStatus.selfValue());
    DDebug() << "[IMG_N1ModePopupListCustom1_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N1ModePopupListCustom1_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 3) IMG_N1ModePopupListCustom2_stat
ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat IMG_N1ModePopupListCustom2_stat::process() {
    ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat ret = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat::NONE;
    if (mIgnStatus == true && mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N1ModePopupListCustom2_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N1ModePopupListCustom2_stat] SPEC 5.2.5.2.5 N1 Mode List Type";
    DDebug() << "[IMG_N1ModePopupListCustom2_stat] Input_N1SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom2ModeSetStatus.selfValue());
    DDebug() << "[IMG_N1ModePopupListCustom2_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N1ModePopupListCustom2_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.6 N2모드 리스트 타입
// 1) IMG_N2ModePopupListN_stat
ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat IMG_N2ModePopupListN_stat::process() {
    ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat ret = ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat::NONE;
    if (mIgnStatus == true && mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N2ModePopupListN_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N2ModePopupListN_stat] SPEC 5.2.5.2.6 N2 Mode List Type";
    DDebug() << "[IMG_N2ModePopupListN_stat] Input_N2SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchNModeSetStatus.selfValue());
    DDebug() << "[IMG_N2ModePopupListN_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N2ModePopupListN_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 2) IMG_N2ModePopupListCustom1_stat
ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat IMG_N2ModePopupListCustom1_stat::process() {
    ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat ret = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat::NONE;
    if (mIgnStatus == true && mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom1_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N2ModePopupListCustom1_stat] SPEC 5.2.5.2.6 N2 Mode List Type";
    DDebug() << "[IMG_N2ModePopupListCustom1_stat] Input_N2SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom1ModeSetStatus.selfValue());
    DDebug() << "[IMG_N2ModePopupListCustom1_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N2ModePopupListCustom1_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 3) IMG_N2ModePopupListCustom2_stat
ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat IMG_N2ModePopupListCustom2_stat::process() {
    ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat ret = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat::NONE;
    if (mIgnStatus == true && mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) {
        ret = ESFCDrive_ModeConstantIMG_N2ModePopupListCustom2_stat::AVAILABLE;
    }
    DDebug() << "[IMG_N2ModePopupListCustom2_stat] SPEC 5.2.5.2.6 N2 Mode List Type";
    DDebug() << "[IMG_N2ModePopupListCustom2_stat] Input_N2SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom2ModeSetStatus.selfValue());
    DDebug() << "[IMG_N2ModePopupListCustom2_stat] IgnStatus: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_N2ModePopupListCustom2_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.7 GT모드 리스트 타입
// 1) IMG_GtModePopupListGt_stat
ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat IMG_GtModePopupListGt_stat::process() {
    ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat ret = ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat::NONE;
    if (mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::GT) {
        ret = ESFCDrive_ModeConstantIMG_GtModePopupListGt_stat::AVAILABLE;
    }
    DDebug() << "[IMG_GtModePopupListGt_stat] SPEC 5.2.5.2.7 GT Mode List Type";
    DDebug() << "[IMG_GtModePopupListGt_stat] Inter_ConfigHighPerformance: "
             << static_cast<uint64_t>(mInter_ConfigHighPerformance.selfValue());
    DDebug() << "[IMG_GtModePopupListGt_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 2) IMG_GtModePopupListMy_stat
ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat IMG_GtModePopupListMy_stat::process() {
    ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat ret = ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat::NONE;
    if (mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::GT) {
        ret = ESFCDrive_ModeConstantIMG_GtModePopupListMy_stat::AVAILABLE;
    }
    DDebug() << "[IMG_GtModePopupListMy_stat] SPEC 5.2.5.2.7 GT Mode List Type";
    DDebug() << "[IMG_GtModePopupListMy_stat] Inter_ConfigHighPerformance: "
             << static_cast<uint64_t>(mInter_ConfigHighPerformance.selfValue());
    DDebug() << "[IMG_GtModePopupListMy_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.8 드라이브 모드 선택 상태
ESFCDrive_ModeConstantIMG_DriveModeSelect_stat IMG_DriveModeSelect_stat::process() {
    ESFCDrive_ModeConstantIMG_DriveModeSelect_stat ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::OFF;
    if (mIgnStatus == true) {
        if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::ECO) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::ECO_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::NORMAL_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::SPORT_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::MY_DRIVE) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::MY_DRIVE_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SMART) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::SMART_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SNOW) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::SNOW_ON;
        } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::CHAUFFEUR) {
            ret = ESFCDrive_ModeConstantIMG_DriveModeSelect_stat::CHAUFFEUR_ON;
        } else {
            // OFF
        }
    }
    DDebug() << "[IMG_DriveModeSelect_stat] SPEC 5.2.5.2.8";
    DDebug() << "[IMG_DriveModeSelect_stat] Inter_N2SwitchCustomSetStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[IMG_DriveModeSelect_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.9 터레인 모드 선택 상태
ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat IMG_TerrainModeSelect_stat::process() {
    ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::OFF;
    if (mIgnStatus == true) {
        if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SNOW) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::SNOW_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::MUD) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::MUD_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SAND) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::SAND_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DESERT) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::DESERT_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::AUTO) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::AUTO_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DEEP_SNOW) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::DEEP_SNOW_ON;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::ROCK) {
            ret = ESFCDrive_ModeConstantIMG_TerrainModeSelect_stat::ROCK;
        } else {
            // OFF
        }
    }
    DDebug() << "[IMG_TerrainModeSelect_stat] SPEC 5.2.5.2.9";
    DDebug() << "[IMG_TerrainModeSelect_stat] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[IMG_TerrainModeSelect_stat] IGN1: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_TerrainModeSelect_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.10 N 모드 선택 상태
ESFCDrive_ModeConstantIMG_NModeSelect_stat IMG_NModeSelect_stat::process() {
    ESFCDrive_ModeConstantIMG_NModeSelect_stat ret = ESFCDrive_ModeConstantIMG_NModeSelect_stat::OFF;
    if (mIgnStatus == true) {
        if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) {
            ret = ESFCDrive_ModeConstantIMG_NModeSelect_stat::N;
        } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) {
            ret = ESFCDrive_ModeConstantIMG_NModeSelect_stat::CUSTOM1;
        } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) {
            ret = ESFCDrive_ModeConstantIMG_NModeSelect_stat::CUSTOM2;
        } else {
            // OFF
        }
    }
    DDebug() << "[IMG_NModeSelect_stat] SPEC 5.2.5.2.10";
    DDebug() << "[IMG_NModeSelect_stat] Inter_NModeSelectStatus: " << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[IMG_NModeSelect_stat] IGN1: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_NModeSelect_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.11 GT모드 선택 상태
ESFCDrive_ModeConstantIMG_GtModeSelect_stat IMG_GtModeSelect_stat::process() {
    ESFCDrive_ModeConstantIMG_GtModeSelect_stat ret = ESFCDrive_ModeConstantIMG_GtModeSelect_stat::OFF;
    if (mIgnStatus == true) {
        if (mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT) {
            ret = ESFCDrive_ModeConstantIMG_GtModeSelect_stat::GT;
        } else if (mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
            ret = ESFCDrive_ModeConstantIMG_GtModeSelect_stat::GT_MY;
        } else {
            // Default: OFF
        }
    }
    DDebug() << "[IMG_NModeSelect_stat] SPEC 5.2.5.2.11";
    DDebug() << "[IMG_NModeSelect_stat] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[IMG_NModeSelect_stat] IGN1: " << mIgnStatus.selfValue();
    DDebug() << "[IMG_NModeSelect_stat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
}  // namespace ccos
