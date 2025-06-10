/**
 * @file Drive_ModeEventLogics.cpp
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
// 5.2.5.3.1 Driving mode change
std::string EventDrivingModeChangeHandler::process() {
    // NOTE(andre) : there's no condition for IGN1 toggle. thus ign was dealt with outside (processSinkEvent).
    std::string ret = "";

    if (mIgnStatus == false) {
        DDebug() << "[EventDrivingModeChange] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        mPrevInter_NModeSelectStatus = mInter_NModeSelectStatus;
        mPrevInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mPrevInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mPrevInter_NModeSwitchStatus = mInter_NModeSwitchStatus;
        mPrevInput_RcsDriveModeRotateEndStatus = mInput_RcsDriveModeRotateEndStatus;
        mPrevInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        mPrevIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
        return ret;
    }

    if (mIMG_TelltaleTowMode_stat == EIMG_TelltaleTowMode_stat::OFF) {
        if ((mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
             mPrevInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::NONE &&
             mPrevInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::MAX &&
             mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
             mPrevInter_TerrainModeSelectStatus != mInter_TerrainModeSelectStatus) ||
            (mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
             (mPrevInput_RcsDriveModeRotateEndStatus != EInput_RcsDriveModeRotateEndStatus::ON &&
              mInput_RcsDriveModeRotateEndStatus == EInput_RcsDriveModeRotateEndStatus::ON))) {
            ret = "E00702";
        } else if (mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED &&
                   mPrevInter_NModeSelectStatus != mInter_NModeSelectStatus &&
                   mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::OFF) {
            ret = "E00704";
        } else if (mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED &&
                   mPrevInter_NModeSelectStatus != mInter_NModeSelectStatus &&
                   mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::OFF) {
            ret = "E00705";
        } else if ((mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::N) &&
                   (mInter_NModeOnReq == EInter_NModeOnReq::ON_REQUEST ||
                    mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::LONG_PRESSED) &&
                   (mPrevInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::N &&
                    mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N)) {
            ret = "E00706";
        } else if ((mPrevInter_DriveModeSelectStatus != mInter_DriveModeSelectStatus) || (mIsSingleDriveModeEnabled == true) ||
                   (mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
                    mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
                    mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
                    mPrevInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
                    mPrevInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::NONE &&
                    mPrevInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::MAX &&
                    mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF) ||
                   (mPrevInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
                    mPrevInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::NONE &&
                    mPrevInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::MAX &&
                    mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF) ||
                   (mPrevInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
                    mPrevInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::NONE &&
                    mPrevInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::MAX &&
                    mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF) ||
                   (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF &&
                    mPrevInput_RcsDriveModeRotateEndStatus != EInput_RcsDriveModeRotateEndStatus::ON &&
                    mInput_RcsDriveModeRotateEndStatus == EInput_RcsDriveModeRotateEndStatus::ON)) {
            ret = "E00713";
        } else if (mInput_GtSwitchStatus == EInput_GtSwitchStatus::PRESSED &&
                   mPrevInter_GtModeSelectStatus != mInter_GtModeSelectStatus &&
                   mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::OFF) {
            ret = "E00714";
        } else {
            // no operation
        }
    }

    DDebug() << "[EventDrivingModeChange] SPEC 5.2.5.3.1";
    DDebug() << "[EventDrivingModeChange] Inter_NModeSelectStatus: " << static_cast<uint64_t>(mPrevInter_NModeSelectStatus)
             << "->" << static_cast<uint64_t>(mInter_NModeSelectStatus);
    DDebug() << "[EventDrivingModeChange] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mPrevInter_TerrainModeSelectStatus) << "->"
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus);
    DDebug() << "[EventDrivingModeChange] Inter_NModeSwitchStatus: " << static_cast<uint64_t>(mPrevInter_NModeSwitchStatus)
             << "->" << static_cast<uint64_t>(mInter_NModeSwitchStatus);
    DDebug() << "[EventDrivingModeChange] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mPrevInter_DriveModeSelectStatus) << "->"
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus);
    DDebug() << "[EventDrivingModeChange] Inter_GtModeSelectStatus: " << static_cast<uint64_t>(mPrevInter_GtModeSelectStatus)
             << "->" << static_cast<uint64_t>(mInter_GtModeSelectStatus);
    DDebug() << "[EventDrivingModeChange] IMG_TelltaleTowMode_stat: " << static_cast<uint64_t>(mPrevIMG_TelltaleTowMode_stat)
             << "->" << static_cast<uint64_t>(mIMG_TelltaleTowMode_stat);
    DDebug() << "[EventDrivingModeChange] Input_GtSwitchStatus: " << static_cast<uint64_t>(mInput_GtSwitchStatus);
    DDebug() << "[EventDrivingModeChange] Inter_NModeOnReq: " << static_cast<uint64_t>(mInter_NModeOnReq);
    DDebug() << "[EventDrivingModeChange] Inter_ConfigHighPerformance: " << static_cast<uint64_t>(mInter_ConfigHighPerformance);
    DDebug() << "[EventDrivingModeChange] Input_TerrainModeErrorStatus: " << static_cast<uint64_t>(mInput_TerrainModeErrorStatus);
    DDebug() << "[EventDrivingModeChange] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[EventDrivingModeChange] Result: " << ret;
    mPrevInter_NModeSelectStatus = mInter_NModeSelectStatus;
    mPrevInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
    mPrevInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
    mPrevInter_NModeSwitchStatus = mInter_NModeSwitchStatus;
    mPrevInput_RcsDriveModeRotateEndStatus = mInput_RcsDriveModeRotateEndStatus;
    mPrevInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
    mPrevIMG_TelltaleTowMode_stat = mIMG_TelltaleTowMode_stat;
    return ret;
}
// 5.2.5.3.2 Terrain mode exit notification
std::string EventTerrainModeExitNotiHandler::process() {
    // NOTE(andre) : there's no condition for IGN1 toggle. thus ign was dealt with outside (processSinkEvent).
    std::string ret = "";

    if (mIgnStatus == false) {
        DDebug() << "[EventTerrainModeExitNoti] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        mPrevInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        return ret;
    }

    if ((mIMG_DriveModePopupTitle_stat == ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TERRAIN &&
         mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
         (mPrevInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
          mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR)) ||
        (mIMG_DriveModePopupTitle_stat == ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TERRAIN &&
         mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR &&
         ((mPrevInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::DEFAULT &&
           mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::PUSH_TERRAIN) ||
          (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
           (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
            mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED))))) {
        ret = "E00707";
    } else if ((mIMG_DriveModePopupTitle_stat == ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TRACTION &&
                mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
                (mPrevInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
                 mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR)) ||
               (mIMG_DriveModePopupTitle_stat == ESFCDrive_ModeConstantIMG_DriveModePopupTitle_stat::TRACTION &&
                mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR &&
                ((mPrevInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::DEFAULT &&
                  mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::PUSH_TERRAIN) ||
                 (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
                  (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
                   mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED))))) {
        ret = "E00716";
    } else {
        // no operation
    }

    DDebug() << "[EventTerrainModeExitNoti] SPEC 5.2.5.3.2";
    DDebug() << "[EventTerrainModeExitNoti] IMG_DriveModePopupTitle_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupTitle_stat);
    DDebug() << "[EventTerrainModeExitNoti] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus);
    DDebug() << "[EventTerrainModeExitNoti] Input_TerrainModeErrorStatus: "
             << static_cast<uint64_t>(mPrevInput_TerrainModeErrorStatus) << "->"
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus);
    DDebug() << "[EventTerrainModeExitNoti] Inter_DrvmodeSwitchStatus: " << static_cast<uint64_t>(mInter_DrvmodeSwitchStatus);
    DDebug() << "[EventTerrainModeExitNoti] Inter_DrvmodeSwitchStatus: " << static_cast<uint64_t>(mPrevInter_DrvmodeSwitchStatus)
             << "->" << static_cast<uint64_t>(mInter_DrvmodeSwitchStatus);
    DDebug() << "[EventTerrainModeExitNoti] Result: " << ret;
    mPrevInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
    mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    return ret;
}
//  5.2.5.3.3 N 커스텀 모드 진입 재확인
std::string EventReconfirmEnterNCustomHandler::process() {
    // NOTE(andre) : there's no condition for IGN1 toggle. thus ign was dealt with outside (processSinkEvent).
    std::string ret = "";
    if (mIgnStatus == false || mInput_GearSelectStatusFromTCU == EInput_GearSelectStatusFromTCU::DS_MODE) {
        DDebug() << "[EventReconfirmEnterNCustom] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        DDebug() << "[EventReconfirmEnterNCustom] IGN OFF > Input_GearSelectStatusFromTCU: "
                 << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU);
        mPrevInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mPrevInput_N2SwitchStatus = mInput_N2SwitchStatus;
        return ret;
    }

    if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
         mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
        (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
        (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::OFF) &&
        (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::ON) &&
        (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::DISABLE) &&
        (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
         mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
        ret = "E00708";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
               (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
               (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::ON) &&
               (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::DISABLE) &&
               (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
        ret = "E00708";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF) &&
               (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
               (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::OFF) &&
               (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::OFF) &&
               (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
        ret = "E00709";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
               (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
               (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::OFF) &&
               (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
        ret = "E00709";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
               (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
               (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
        ret = "E00709";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
                mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
               (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
               (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::OFF) &&
               (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::ON) &&
               (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::DISABLE) &&
               (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
        ret = "E00710";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
               (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
               (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::ON) &&
               (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::DISABLE) &&
               (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
        ret = "E00710";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF) &&
               (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
               (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::OFF) &&
               (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::OFF) &&
               (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
        ret = "E00711";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
               (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
               (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::OFF) &&
               (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
        ret = "E00711";
    } else if ((mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
               (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
               (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
               (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::DISABLE) &&
               (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
        ret = "E00711";
    } else {
        // no operation
    }

    DDebug() << "[EventReconfirmEnterNCustom] SPEC 5.2.5.3.3";
    DDebug() << "[EventReconfirmEnterNCustom] Inter_NModeSelectStatus: " << static_cast<uint64_t>(mInter_NModeSelectStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N1SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchMainSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N1SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchNModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N1SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom1ModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_EscCustom1EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom1EnableStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N1SwitchStatus:" << static_cast<uint64_t>(mPrevInput_N1SwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_N1SwitchStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N1SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom2ModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_EscCustom2EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom2EnableStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N2SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchMainSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N2SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchNModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N2SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom1ModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N2SwitchStatus:" << static_cast<uint64_t>(mPrevInput_N2SwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_N2SwitchStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Input_N2SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom2ModeSetStatus);
    DDebug() << "[EventReconfirmEnterNCustom] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[EventReconfirmEnterNCustom] Result: " << ret;
    mPrevInput_N1SwitchStatus = mInput_N1SwitchStatus;
    mPrevInput_N2SwitchStatus = mInput_N2SwitchStatus;
    return ret;
}

// 5.2.5.3.4 GT_MY 모드 진입 재확인
std::string EventReconfirmEnterGT_MYHandler::process() {
    std::string ret = "";

    if (mIgnStatus == false) {
        DDebug() << "[EventReconfirmEnterGT_MY] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        mPrevInput_GtSwitchStatus = mInput_GtSwitchStatus;
        mPrevInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
        return ret;
    }

    // NOTE: E00715 팝업은 DMS가 GT 상태일 때 GT 버튼을 한번 더 누르면 발화되어야 한다. (jira: EXNCP-42826)
    if (mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE &&
        mPrevInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
        mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
        mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::DISABLE &&
        (mPrevInput_GtSwitchStatus == EInput_GtSwitchStatus::DEFAULT &&
         mInput_GtSwitchStatus == EInput_GtSwitchStatus::PRESSED)) {
        ret = "E00715";
    }
    DDebug() << "[EventReconfirmEnterGT_MY] SPEC 5.2.5.3.4";
    DDebug() << "[EventReconfirmEnterGT_MY] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU);
    DDebug() << "[EventReconfirmEnterGT_MY] Inter_GtModeSelectStatus: " << static_cast<uint64_t>(mInter_GtModeSelectStatus);
    DDebug() << "[EventReconfirmEnterGT_MY] Input_EscCustom1EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom1EnableStatus);
    DDebug() << "[EventReconfirmEnterGT_MY] Input_N1SwitchStatus: " << static_cast<uint64_t>(mPrevInput_GtSwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_GtSwitchStatus);
    DDebug() << "[EventReconfirmEnterNCustom] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[EventReconfirmEnterGT_MY] Result: " << ret;
    mPrevInput_GtSwitchStatus = mInput_GtSwitchStatus;
    mPrevInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
    return ret;
}

// 5.2.5.3.5 모드 전환 불가 안내
std::string EventModeSwitchingImpossibleIndiHandler::process() {
    // NOTE(andre) : there's no condition for IGN1 toggle. thus ign was dealt with outside (processSinkEvent).
    std::string ret = "";
    if (mIgnStatus == false || mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE) {
        DDebug() << "[EventModeSwitchingImpossibleIndi] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        DDebug() << "[EventModeSwitchingImpossibleIndi] IGN OFF > Input_GearSelectStatusFromTCU: "
                 << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU);
        mPrevInput_N1SwitchStatus = mInput_N1SwitchStatus;
        mPrevInput_N2SwitchStatus = mInput_N2SwitchStatus;
        mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mPrevInput_GtSwitchStatus = mInput_GtSwitchStatus;
        mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        return ret;
    }

    if ((mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE &&
         (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
          mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) ||
        (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE &&
         (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
          mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) ||
        (mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
         mPrivate_DriveModeSwStatus != EPrivate_DriveModeSwStatus::DEFAULT) ||
        (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
         mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED) ||
        (mInter_NModeOnReq == EInter_NModeOnReq::ON_REQUEST) ||
        (mPrevInput_GtSwitchStatus == EInput_GtSwitchStatus::DEFAULT &&
         mInput_GtSwitchStatus == EInput_GtSwitchStatus::PRESSED)) {
        ret = "E00712";
    }

    DDebug() << "[EventModeSwitchingImpossibleIndi] SPEC 5.2.5.3.5";
    DDebug() << "[EventModeSwitchingImpossibleIndi] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Input_N1SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchMainSetStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Input_N2SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchMainSetStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Input_N1SwitchStatus: " << static_cast<uint64_t>(mPrevInput_N1SwitchStatus)
             << "->" << static_cast<uint64_t>(mInput_N1SwitchStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Input_N2SwitchStatus: " << static_cast<uint64_t>(mPrevInput_N2SwitchStatus)
             << "->" << static_cast<uint64_t>(mInput_N2SwitchStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Private_DriveModeSwStatus: "
             << static_cast<uint64_t>(mPrevPrivate_DriveModeSwStatus) << "->"
             << static_cast<uint64_t>(mPrivate_DriveModeSwStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Inter_NModeOnReq: " << static_cast<uint64_t>(mInter_NModeOnReq);
    DDebug() << "[EventReconfirmEnterNCustom] IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
    DDebug() << "[EventModeSwitchingImpossibleIndi] Result: " << ret;
    mPrevInput_N1SwitchStatus = mInput_N1SwitchStatus;
    mPrevInput_N2SwitchStatus = mInput_N2SwitchStatus;
    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevInput_GtSwitchStatus = mInput_GtSwitchStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    return ret;
}

// 5.2.5.3.6 구동모드 2H에 의한 오프로드 모드 진입 불가 알림
std::string EventAwd2hModeNotificationHandler::process() {
    std::string ret = "";
    if (mIgnStatus == false) {
        DDebug() << "[EventAwd2hModeNotificationHandler] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        return ret;
    }

    if (mInput_TerrainModeErrorStatus != EInput_TerrainModeErrorStatus::ERROR &&
        mInput_AwdMode2HStatus == EInput_AwdMode2HStatus::ON &&
        ((mPrevInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::DEFAULT &&
          mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::PUSH_TERRAIN) ||
         (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
          (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
           mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED)))) {
        ret = "E00717";
    }

    DDebug() << "[EventAwd2hModeNotificationHandler] SPEC 5.2.5.3.6";
    DDebug() << "[EventAwd2hModeNotificationHandler] Input_TerrainModeErrorStatus: "
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus);
    DDebug() << "[EventAwd2hModeNotificationHandler] mInput_AwdMode2HStatus: " << static_cast<uint64_t>(mInput_AwdMode2HStatus);
    DDebug() << "[EventAwd2hModeNotificationHandler] PrevInter_DrvmodeSwitchStatus: "
             << static_cast<uint64_t>(mPrevInter_DrvmodeSwitchStatus);
    DDebug() << "[EventAwd2hModeNotificationHandler] Inter_DrvmodeSwitchStatus: "
             << static_cast<uint64_t>(mInter_DrvmodeSwitchStatus);

    mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    return ret;
}

// 5.2.5.3.7 TOW 모드 진입에 의한 모드 전환 불가 알림
std::string EventUnableToSwitchModeByTowModeHandler::process() {
    std::string ret = "";
    if (mIgnStatus == false) {
        DDebug() << "[EventUnableToSwitchModeByTowModeHandler] IGN OFF > IgnStatus: " << static_cast<uint64_t>(mIgnStatus);
        mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
        mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
        mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
        mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
        return ret;
    }

    if ((mIMG_TelltaleTowMode_stat == EIMG_TelltaleTowMode_stat::ON &&
         ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
           mPrivate_DriveModeSwStatus != EPrivate_DriveModeSwStatus::DEFAULT) ||
          (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
           mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED))) ||
        (mInput_AwdMode2HStatus != EInput_AwdMode2HStatus::ON &&
         mInput_TerrainModeErrorStatus != EInput_TerrainModeErrorStatus::ERROR &&
         mIMG_TelltaleTowMode_stat == EIMG_TelltaleTowMode_stat::ON &&
         ((mPrevInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::DEFAULT &&
           mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::PUSH_TERRAIN) ||
          (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
           (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
            mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED))))) {
        ret = "E00718";
    }

    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] SPEC 5.2.5.3.7";
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] IMG_TelltaleTowMode_stat : "
             << static_cast<uint64_t>(mIMG_TelltaleTowMode_stat);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] Private_DriveModeSwStatus : "
             << static_cast<uint64_t>(mPrivate_DriveModeSwStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] PrevPrivate_DriveModeSwStatus : "
             << static_cast<uint64_t>(mPrevPrivate_DriveModeSwStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] Inter_DriveModeOnRoadSwitchStatus : "
             << static_cast<uint64_t>(mInter_DriveModeOnRoadSwitchStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] PrevInter_DriveModeOnRoadSwitchStatus : "
             << static_cast<uint64_t>(mPrevInter_DriveModeOnRoadSwitchStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] mInput_AwdMode2HStatus: "
             << static_cast<uint64_t>(mInput_AwdMode2HStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] Input_TerrainModeErrorStatus : "
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] Inter_DrvmodeSwitchStatus : "
             << static_cast<uint64_t>(mInter_DrvmodeSwitchStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] PrevInter_DrvmodeSwitchStatus : "
             << static_cast<uint64_t>(mPrevInter_DrvmodeSwitchStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] Inter_DriveModeTerrainSwitchStatus : "
             << static_cast<uint64_t>(mInter_DriveModeTerrainSwitchStatus);
    DDebug() << "[EventUnableToSwitchModeByTowModeHandler] PrevInter_DriveModeTerrainSwitchStatus : "
             << static_cast<uint64_t>(mPrevInter_DriveModeTerrainSwitchStatus);

    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    mPrevInter_DrvmodeSwitchStatus = mInter_DrvmodeSwitchStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    return ret;
}

}  // namespace ccos
