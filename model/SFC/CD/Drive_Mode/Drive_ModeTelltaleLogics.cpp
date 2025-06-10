/**
 * @file Drive_ModeTelltaleLogics.cpp
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
// 5.2.5.1.2 Telltale 흐리게 표시 상태
ESFCDrive_ModeTelltaleDimmedStat SFCDrive_ModeTelltaleDimmedStat::process() {
    ESFCDrive_ModeTelltaleDimmedStat ret = ESFCDrive_ModeTelltaleDimmedStat::NORMAL;
    if (mIgnStatus == true && mInput_GearSelectStatusFromTCU == EInput_GearSelectStatusFromTCU::DS_MODE &&
        mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT) {
        ret = ESFCDrive_ModeTelltaleDimmedStat::DIMMED;
    }
    DDebug() << "[TelltaleDimmedStat] SPEC 5.2.5.1.2";
    DDebug() << "[TelltaleDimmedStat] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[TelltaleDimmedStat] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDimmedStat] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[TelltaleDimmedStat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.5.1.1 Normal display behavior
ESFCDrive_ModeTelltaleDMSStat SFCDrive_ModeTelltaleDMSStat::process() {
    ESFCDrive_ModeTelltaleDMSStat ret = ESFCDrive_ModeTelltaleDMSStat::OFF;
    if (mIgnStatus == true) {
        if (mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF) {
            if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF) {
                if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF ||
                    mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::READY) {
                    if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::ECO) {
                        ret = ESFCDrive_ModeTelltaleDMSStat::ECO;
                    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT) {
                        ret = ESFCDrive_ModeTelltaleDMSStat::SPORT;
                    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SMART) {
                        if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::ECO ||
                            mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::ECO_PLUS) {
                            ret = ESFCDrive_ModeTelltaleDMSStat::SMART_ECO;
                        } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::NORMAL) {
                            ret = ESFCDrive_ModeTelltaleDMSStat::SMART_COMFORT;
                        } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::SPORT ||
                                   mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::SPORT_PLUS) {
                            ret = ESFCDrive_ModeTelltaleDMSStat::SMART_SPORT;
                        } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::INACTIVE) {
                            ret = ESFCDrive_ModeTelltaleDMSStat::SMART;
                        } else {
                            // Default: OFF
                        }
                    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::CHAUFFEUR) {
                        ret = ESFCDrive_ModeTelltaleDMSStat::CHAUFFEUR;
                    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::MY_DRIVE) {
                        ret = ESFCDrive_ModeTelltaleDMSStat::MY_DRIVE;
                    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SNOW) {
                        ret = ESFCDrive_ModeTelltaleDMSStat::CUSTOM_SNOW;
                    } else {
                        // Default: OFF
                    }
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SNOW) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_SNOW;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::MUD) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_MUD;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SAND) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_SAND;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DESERT) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_DESERT;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::AUTO) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_AUTO;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DEEP_SNOW) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_DEEP_SNOW;
                } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::ROCK) {
                    ret = ESFCDrive_ModeTelltaleDMSStat::TERRAIN_ROCK;
                } else {
                    // Default: OFF
                }
            } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) {
                ret = ESFCDrive_ModeTelltaleDMSStat::N;
            } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) {
                ret = ESFCDrive_ModeTelltaleDMSStat::N_CUSTOM1;
            } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) {
                ret = ESFCDrive_ModeTelltaleDMSStat::N_CUSTOM2;
            } else {
                // Default: OFF
            }
        } else if (mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT) {
            ret = ESFCDrive_ModeTelltaleDMSStat::GT;
        } else if (mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
            ret = ESFCDrive_ModeTelltaleDMSStat::GT_MY;
        } else {
            // Default: OFF
        }
    }

    DDebug() << "[TelltaleDMSStat] SPEC 5.2.5.1.1";
    DDebug() << "[TelltaleDMSStat] Inter_GtModeSelectStatus: " << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDMSStat] Inter_NModeSelectStatus: " << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDMSStat] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDMSStat] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDMSStat] Input_SmartDriveModeStatus: "
             << static_cast<uint64_t>(mInput_SmartDriveModeStatus.selfValue());
    DDebug() << "[TelltaleDMSStat] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[TelltaleDMSStat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

}  // namespace ccos
