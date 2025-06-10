/**
 * @file Drive_Mode_2_0TelltaleLogics.cpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024 Hyundai Motor Company,
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

#include "Drive_Mode_2_0Types.hpp"

namespace ccos {
// 5.3.5.1.1 Normal display behavior
ETelltaleDriveModeStat TelltaleDriveModeStat::process() {
    ETelltaleDriveModeStat ret = ETelltaleDriveModeStat::OFF;
    if (mIgnStatus == true) {
        if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF ||
            mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::READY) {
            if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::ECO) {
                ret = ETelltaleDriveModeStat::ECO;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::NORMAL) {
                ret = ETelltaleDriveModeStat::NORMAL;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SPORT) {
                ret = ETelltaleDriveModeStat::SPORT;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SPORTPLUS) {
                ret = ETelltaleDriveModeStat::SPORTPLUS;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SMART) {
                if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::ECO ||
                    mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::ECO_PLUS) {
                    ret = ETelltaleDriveModeStat::SMART_ECO;
                } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::NORMAL) {
                    ret = ETelltaleDriveModeStat::SMART_COMFORT;
                } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::SPORT ||
                           mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::SPORT_PLUS) {
                    ret = ETelltaleDriveModeStat::SMART_SPORT;
                } else if (mInput_SmartDriveModeStatus == EInput_SmartDriveModeStatus::INACTIVE) {
                    ret = ETelltaleDriveModeStat::SMART;
                } else {
                    // Default: OFF
                }
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::CHAUFFEUR) {
                ret = ETelltaleDriveModeStat::CHAUFFEUR;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::MY) {
                ret = ETelltaleDriveModeStat::MY_DRIVE;
            } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SNOW) {
                ret = ETelltaleDriveModeStat::CUSTOM_SNOW;
            } else {
                // Default: OFF
            }
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SNOW) {
            ret = ETelltaleDriveModeStat::TERRAIN_SNOW;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::MUD) {
            ret = ETelltaleDriveModeStat::TERRAIN_MUD;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SAND) {
            ret = ETelltaleDriveModeStat::TERRAIN_SAND;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::AUTO) {
            ret = ETelltaleDriveModeStat::TERRAIN_AUTO;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DEEP_SNOW) {
            ret = ETelltaleDriveModeStat::TERRAIN_DEEP_SNOW;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::ROCK) {
            ret = ETelltaleDriveModeStat::TERRAIN_ROCK;
        } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DESERT) {
            ret = ETelltaleDriveModeStat::TERRAIN_DESERT;
        } else {
            // Default: OFF
        }
    }

    DDebug() << "[TelltaleDriveModeStat] SPEC 5.3.5.1.1";
    DDebug() << "[TelltaleDriveModeStat] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDriveModeStat] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[TelltaleDriveModeStat] Input_SmartDriveModeStatus: "
             << static_cast<uint64_t>(mInput_SmartDriveModeStatus.selfValue());
    DDebug() << "[TelltaleDriveModeStat] IgnStatus: " << static_cast<uint64_t>(mIgnStatus.selfValue());
    DDebug() << "[TelltaleDriveModeStat] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

}  // namespace ccos
