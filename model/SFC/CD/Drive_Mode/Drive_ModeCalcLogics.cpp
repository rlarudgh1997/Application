/**
 * @file Drive_ModeCalcLogics.cpp
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
#include <Timer.h>

namespace ccos {
// 5.2.4.2.1 콘솔스위치와 N스위치의 통합
EPrivate_DriveModeSwStatus Private_DriveModeSwStatus::process() {
    EPrivate_DriveModeSwStatus ret = EPrivate_DriveModeSwStatus::DEFAULT;

    if (mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::PUSH_TERRAIN) {
        ret = EPrivate_DriveModeSwStatus::PUSH_TERRAIN;
    } else if (mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::CCW) {
        ret = EPrivate_DriveModeSwStatus::CCW;
    } else if (mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::CW) {
        ret = EPrivate_DriveModeSwStatus::CW;
    } else if (mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::CCW_LONG) {
        ret = EPrivate_DriveModeSwStatus::CCW_LONG;
    } else if (mInter_DrvmodeSwitchStatus == EInter_DrvmodeSwitchStatus::CW_LONG) {
        ret = EPrivate_DriveModeSwStatus::CW_LONG;
    } else {
        // Default
    }

    DDebug() << "[Private_DriveModeSwStatus] SPEC 5.2.4.2.1";
    DDebug() << "[Private_DriveModeSwStatus] Inter_DrvmodeSwitchStatus: "
             << static_cast<uint64_t>(mInter_DrvmodeSwitchStatus.selfValue());
    DDebug() << "[Private_DriveModeSwStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.4.3 기본 모드 상태 판단
ESFCDrive_ModeInter_DriveModeSelectStatus Inter_DriveModeSelectStatus::process() {
    ESFCDrive_ModeInter_DriveModeSelectStatus ret = ESFCDrive_ModeInter_DriveModeSelectStatus::MAX;

    // 5.2.4.3.1 Long Press mode 적용 여부
    if (mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::N ||
        mInter_ConfigTowModeLongKey == EInter_ConfigTowModeLongKey::ON) {
        mPrivate_LongPressAppliedStatus = Private_LongPressAppliedStatus::APPLY;
    } else {
        mPrivate_LongPressAppliedStatus = Private_LongPressAppliedStatus::OFF;
    }

    // 5.2.4.3.2 기본 모드 적용 리스트
    if (mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::ECO);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE &&
               mIMG_DriveModePopupListEco_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::ECO, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::ECO, false);
    }
    if (mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::NORMAL);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE &&
               mIMG_DriveModePopupListNormal_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::NORMAL, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::NORMAL, false);
    }
    if (mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORT);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE &&
               mIMG_DriveModePopupListSport_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORT, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORT, false);
    }
    if (mIMG_DriveModePopupListChauffeur_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::CHAUFFEUR);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListChauffeur_stat ==
                   ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::AVAILABLE &&
               mIMG_DriveModePopupListChauffeur_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListChauffeur_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::CHAUFFEUR, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::CHAUFFEUR, false);
    }
    if (mIMG_DriveModePopupListMyDrive_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::MY_DRIVE);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListMyDrive_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::AVAILABLE &&
               mIMG_DriveModePopupListMyDrive_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListMyDrive_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::MY_DRIVE, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::MY_DRIVE, false);
    }
    if (mIMG_DriveModePopupListSmart_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SMART);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListSmart_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::AVAILABLE &&
               mIMG_DriveModePopupListSmart_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListSmart_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SMART, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SMART, false);
    }
    if (mIMG_DriveModePopupListSnow_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SNOW);
    } else if (mIgnStatus == true &&
               mPrevIMG_DriveModePopupListSnow_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::AVAILABLE &&
               mIMG_DriveModePopupListSnow_stat != ESFCDrive_ModeConstantIMG_DriveModePopupListSnow_stat::AVAILABLE) {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SNOW, false);
    } else {
        mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SNOW, false);
    }

    // 5.2.4.3.4 기본 모드 제어
    if (mIgnStatus == true) {
        if (mInter_ConfigEndlessDialSwitch == EInter_ConfigEndlessDialSwitch::ON) {
            // 5.2.4.3.4.1 RCS 에 의한 제어
            HBool isUpdate = true;
            if (mInput_RcsDriveModeOnOffStatus == EInput_RcsDriveModeOnOffStatus::NORMAL_MODE) {
                if (mInput_RcsDriveModeStatus == EInput_RcsDriveModeStatus::ECO) {
                    ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
                } else if (mInput_RcsDriveModeStatus == EInput_RcsDriveModeStatus::NORMAL) {
                    ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
                } else if (mInput_RcsDriveModeStatus == EInput_RcsDriveModeStatus::SPORT) {
                    ret = ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT;
                } else if (mInput_RcsDriveModeStatus == EInput_RcsDriveModeStatus::SMART) {
                    ret = ESFCDrive_ModeInter_DriveModeSelectStatus::SMART;
                } else {
                    isUpdate = false;
                }
            } else if (mInput_RcsDriveModeOnOffStatus == EInput_RcsDriveModeOnOffStatus::TERRAIN_MODE &&
                       mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::SNOW &&
                       mInter_ConfigOffRoad == EInter_ConfigOffRoad::OFF) {
                ret = ESFCDrive_ModeInter_DriveModeSelectStatus::SNOW;
            } else {
                isUpdate = false;
            }
            // NOTE: RCS에 의한 제어 로직을 통해 Inter_DriveModeSelectStatus값 갱신되면, 해당 값으로 기본 모드 리스트 갱신
            if (isUpdate == true) {
                mPrivate_BasicModeList.setCurrentMode(ret);
            }
        } else {
            // 5.2.4.3.3 기본 모드 이벤트 판단(스위치 입력)
            if (mPrivate_LongPressAppliedStatus == Private_LongPressAppliedStatus::APPLY) {
                if ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                     mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CW) ||
                    (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                     mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED)) {
                    mPrivate_BasicEvent = Private_BasicEvent::CW;
                } else if (mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                           mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CCW) {
                    mPrivate_BasicEvent = Private_BasicEvent::CCW;
                } else {
                    mPrivate_BasicEvent = Private_BasicEvent::OFF;
                    mIsSingleDriveModeEnabled = false;
                }
            } else {
                if ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                     (mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CW ||
                      mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CW_LONG)) ||
                    (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                     (mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED ||
                      mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::LONG_PRESSED))) {
                    mPrivate_BasicEvent = Private_BasicEvent::CW;
                } else if (mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                           (mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CCW ||
                            mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CCW_LONG)) {
                    mPrivate_BasicEvent = Private_BasicEvent::CCW;
                } else {
                    mPrivate_BasicEvent = Private_BasicEvent::OFF;
                    mIsSingleDriveModeEnabled = false;
                }
            }

            // 5.2.4.3.4.2 기본 모드 이벤트에 의한 제어
            if (mPrivate_BasicModeSwitchableStatus == EPrivate_BasicModeSwitchableStatus::ENABLE &&
                mPrivate_BasicEvent == Private_BasicEvent::CW) {
                if (const auto& result = mPrivate_BasicModeList.getNextEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                    if (selfValue() == static_cast<EInter_DriveModeSelectStatus>(ret) &&
                        mPrivate_BasicModeList.getModeActivationCount() == 1U) {
                        mIsSingleDriveModeEnabled = true;
                    } else {
                        mIsSingleDriveModeEnabled = false;
                    }
                }
            } else if (mPrivate_BasicModeSwitchableStatus == EPrivate_BasicModeSwitchableStatus::ENABLE &&
                       mPrivate_BasicEvent == Private_BasicEvent::CCW) {
                if (const auto& result = mPrivate_BasicModeList.getPrevEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                    if (selfValue() == static_cast<EInter_DriveModeSelectStatus>(ret) &&
                        mPrivate_BasicModeList.getModeActivationCount() == 1U) {
                        mIsSingleDriveModeEnabled = true;
                    } else {
                        mIsSingleDriveModeEnabled = false;
                    }
                }
            } else if (mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) {
                mPrivate_BasicModeList.setCurrentMode(ESFCDrive_ModeInter_DriveModeSelectStatus::ECO);
                ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
            } else if (mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) {
                mPrivate_BasicModeList.setCurrentMode(ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL);
                ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
            } else if (mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT) {
                mPrivate_BasicModeList.setCurrentMode(ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT);
                ret = ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT;
            } else if (mIMG_TelltaleTowMode_stat != EIMG_TelltaleTowMode_stat::ON &&
                       mPrivate_BasicModeList.verifyExistCurrentMode(selfValue()) == false) {
                if (const auto& result = mPrivate_BasicModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (mIMG_TelltaleTowMode_stat == EIMG_TelltaleTowMode_stat::ON) {
                if (mInter_ConfigNormalMode != EInter_ConfigNormalMode::ON) {
                    if (const auto& result = mPrivate_BasicModeList.getFirstEnabledMode(); result.has_value() == true) {
                        ret = result.value();
                    }
                } else {
                    mPrivate_BasicModeList.setCurrentMode(ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL);
                    ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
                }
            } else {
                // no operation
            }
        }
    } else if (mPrevIgnStatus == true && mIgnStatus == false) {
        mPrivate_BasicEvent = Private_BasicEvent::OFF;
        // 5.2.4.3.4.3 IGN on => Off 이벤트 발생시 드라이브 모드 제어
        HBool isUpdate = true;
        if (selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON &&
            mInter_ConfigDriveModeKeepEco == EInter_ConfigDriveModeKeepEco::ON) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
        } else if (selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON &&
                   mInter_ConfigDriveModeKeepEco != EInter_ConfigDriveModeKeepEco::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::ECO) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
        } else if (selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON &&
                   mInter_ConfigDriveModeKeepEco != EInter_ConfigDriveModeKeepEco::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::NORMAL) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else if (selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode != EInter_ConfigEcoMode::ON) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else if (selfValue() == EInter_DriveModeSelectStatus::NORMAL &&
                   mInter_ConfigDriveModeKeepNormal != EInter_ConfigDriveModeKeepNormal::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::ECO) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
        } else if (selfValue() == EInter_DriveModeSelectStatus::NORMAL &&
                   mInter_ConfigDriveModeKeepNormal != EInter_ConfigDriveModeKeepNormal::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::NORMAL) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else if (selfValue() == EInter_DriveModeSelectStatus::NORMAL &&
                   mInter_ConfigDriveModeKeepNormal == EInter_ConfigDriveModeKeepNormal::ON) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else if (selfValue() == EInter_DriveModeSelectStatus::SPORT &&
                   mInter_ConfigDriveModeKeepSport != EInter_ConfigDriveModeKeepSport::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::ECO) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
        } else if (selfValue() == EInter_DriveModeSelectStatus::SPORT &&
                   mInter_ConfigDriveModeKeepSport != EInter_ConfigDriveModeKeepSport::ON &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::NORMAL) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else if (selfValue() == EInter_DriveModeSelectStatus::SPORT &&
                   mInter_ConfigDriveModeKeepSport == EInter_ConfigDriveModeKeepSport::ON) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT;
        } else if (!(selfValue() == EInter_DriveModeSelectStatus::ECO || selfValue() == EInter_DriveModeSelectStatus::NORMAL ||
                     selfValue() == EInter_DriveModeSelectStatus::SPORT) &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::ECO) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::ECO;
        } else if (!(selfValue() == EInter_DriveModeSelectStatus::ECO || selfValue() == EInter_DriveModeSelectStatus::NORMAL ||
                     selfValue() == EInter_DriveModeSelectStatus::SPORT) &&
                   mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::NORMAL) {
            ret = ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL;
        } else {
            isUpdate = false;
        }
        // NOTE: IGN on => off 동작을 통해 Inter_DriveModeSelectStatus 값 갱신되면, 해당 값으로 기본 모드 리스트 갱신
        if (isUpdate == true) {
            mPrivate_BasicModeList.setCurrentMode(ret);
        }
    } else {
        mPrivate_BasicEvent = Private_BasicEvent::OFF;
    }

    if (ret == ESFCDrive_ModeInter_DriveModeSelectStatus::MAX) {
        DDebug() << "[Inter_DriveModeSelectStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
        ret = selfValue();
    }

    DDebug() << "[Inter_DriveModeSelectStatus] SPEC 5.2.4.3";
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigHighPerformance: "
             << static_cast<uint64_t>(mInter_ConfigHighPerformance.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigTowModeLongKey: "
             << static_cast<uint64_t>(mInter_ConfigTowModeLongKey.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mPrivate_LongPressAppliedStatus: "
             << static_cast<uint64_t>(mPrivate_LongPressAppliedStatus);
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListEco_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListEco_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListNormal_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListNormal_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListSport_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListSport_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListMyDrive_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListMyDrive_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListSmart_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListSmart_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mIMG_DriveModePopupListSnow_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListSnow_stat.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mPrivate_BasicEvent: " << static_cast<uint64_t>(mPrivate_BasicEvent);
    DDebug() << "[Inter_DriveModeSelectStatus] mPrivate_DriveModeSwStatus: "
             << static_cast<uint64_t>(mPrivate_DriveModeSwStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_DriveModeOnRoadSwitchStatus: "
             << static_cast<uint64_t>(mInter_DriveModeOnRoadSwitchStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInput_RcsDriveModeOnOffStatus: "
             << static_cast<uint64_t>(mInput_RcsDriveModeOnOffStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInput_RcsDriveModeStatus: "
             << static_cast<uint64_t>(mInput_RcsDriveModeStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_ConfigEndlessDialSwitch: "
             << static_cast<uint64_t>(mInter_ConfigEndlessDialSwitch.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mPrivate_BasicModeSwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeSwitchableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_ConfigDriveModeDefault: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeDefault.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_ConfigDriveModeKeepEco: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepEco.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_ConfigDriveModeKeepNormal: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepNormal.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] mInter_ConfigDriveModeKeepSport: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepSport.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeForSwKeySwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeForSwKeySwitchableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mInter_DriveModeChangeReqFromAVNT.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] PrevInter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mPrevInter_DriveModeChangeReqFromAVNT);
    DDebug() << "[Inter_DriveModeSelectStatus] Ignition: " << mPrevIgnStatus << "->" << mIgnStatus.selfValue();
    DDebug() << "[Inter_DriveModeSelectStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    mPrevIMG_DriveModePopupListEco_stat = mIMG_DriveModePopupListEco_stat;
    mPrevIMG_DriveModePopupListNormal_stat = mIMG_DriveModePopupListNormal_stat;
    mPrevIMG_DriveModePopupListSport_stat = mIMG_DriveModePopupListSport_stat;
    mPrevIMG_DriveModePopupListChauffeur_stat = mIMG_DriveModePopupListChauffeur_stat;
    mPrevIMG_DriveModePopupListMyDrive_stat = mIMG_DriveModePopupListMyDrive_stat;
    mPrevIMG_DriveModePopupListSmart_stat = mIMG_DriveModePopupListSmart_stat;
    mPrevIMG_DriveModePopupListSnow_stat = mIMG_DriveModePopupListSnow_stat;
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    mPrevIgnStatus = mIgnStatus;
    return ret;
}

// 5.2.4.2.2 기본 모드 전환 가능여부 판단
EPrivate_BasicModeSwitchableStatus Private_BasicModeSwitchableStatus::process() {
    EPrivate_BasicModeSwitchableStatus ret = EPrivate_BasicModeSwitchableStatus::DISABLE;
    if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF &&
        mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
        mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
        mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE &&
        mIMG_TelltaleTowMode_stat != EIMG_TelltaleTowMode_stat::ON) {
        ret = EPrivate_BasicModeSwitchableStatus::ENABLE;
    }
    DDebug() << "[Private_BasicModeSwitchableStatus] SPEC 5.2.4.2.2";
    DDebug() << "[Private_BasicModeSwitchableStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Inter_NModeSelectStatus: "
             << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] IMG_TelltaleTowMode_stat: "
             << static_cast<uint64_t>(mIMG_TelltaleTowMode_stat.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.4.2.3 기본 모드 전환 가능여부 판단(음성인식용)
EPrivate_BasicModeForSwKeySwitchableStatus Private_BasicModeForSwKeySwitchableStatus::process() {
    EPrivate_BasicModeForSwKeySwitchableStatus ret = EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE;
    if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
        mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
        mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE &&
        mIMG_TelltaleTowMode_stat != EIMG_TelltaleTowMode_stat::ON) {
        ret = EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE;
    }
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] SPEC 5.2.4.2.3";
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Inter_NModeSelectStatus: "
             << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] IMG_TelltaleTowMode_stat: "
             << static_cast<uint64_t>(mIMG_TelltaleTowMode_stat.selfValue());
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.4.4 터레인모드 상태 판단
EInter_TerrainModeSelectStatus Inter_TerrainModeSelectStatus::process() {
    EInter_TerrainModeSelectStatus ret = EInter_TerrainModeSelectStatus::MAX;

    // 5.2.4.4.1 오프로드 모드 전환 가능여부 판단.
    if (mInter_ConfigEndlessDialSwitch == EInter_ConfigEndlessDialSwitch::OFF &&
        mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
        mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE &&
        mIMG_TelltaleTowMode_stat != EIMG_TelltaleTowMode_stat::ON && mInput_AwdMode2HStatus != EInput_AwdMode2HStatus::ON) {
        mPrivate_OffroadModeSwitchableStatus = Private_OffroadModeSwitchableStatus::ENABLE;
    } else {
        mPrivate_OffroadModeSwitchableStatus = Private_OffroadModeSwitchableStatus::DISABLE;
    }

    // 4.4.2 오프로드 모드 적용 리스트.
    if (mIMG_TerrainModePopupListAuto_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::AUTO);
    } else if (mPrevIMG_TerrainModePopupListAuto_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::AVAILABLE &&
               mIMG_TerrainModePopupListAuto_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListAuto_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::AUTO, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::AUTO, false);
    }
    if (mIMG_TerrainModePopupListSnow_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SNOW);
    } else if (mPrevIMG_TerrainModePopupListSnow_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::AVAILABLE &&
               mIMG_TerrainModePopupListSnow_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListSnow_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SNOW, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SNOW, false);
    }
    if (mIMG_TerrainModePopupListDeepSnow_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DEEP_SNOW);
    } else if (mPrevIMG_TerrainModePopupListDeepSnow_stat ==
                   ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::AVAILABLE &&
               mIMG_TerrainModePopupListDeepSnow_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListDeepSnow_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DEEP_SNOW, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DEEP_SNOW, false);
    }
    if (mIMG_TerrainModePopupListMud_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::MUD);
    } else if (mPrevIMG_TerrainModePopupListMud_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::AVAILABLE &&
               mIMG_TerrainModePopupListMud_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListMud_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::MUD, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::MUD, false);
    }
    if (mIMG_TerrainModePopupListSand_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SAND);
    } else if (mPrevIMG_TerrainModePopupListSand_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::AVAILABLE &&
               mIMG_TerrainModePopupListSand_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListSand_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SAND, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SAND, false);
    }
    if (mIMG_TerrainModePopupListDesert_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DESERT);
    } else if (mPrevIMG_TerrainModePopupListDesert_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::AVAILABLE &&
               mIMG_TerrainModePopupListDesert_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListDesert_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DESERT, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DESERT, false);
    }
    if (mIMG_TerrainModePopupListRock_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::ROCK);
    } else if (mPrevIMG_TerrainModePopupListRock_stat == ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::AVAILABLE &&
               mIMG_TerrainModePopupListRock_stat != ESFCDrive_ModeConstantIMG_TerrainModePopupListRock_stat::AVAILABLE) {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::ROCK, false);
    } else {
        mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::ROCK, false);
    }

    if (mIgnStatus == true) {
        if (mInter_ConfigEndlessDialSwitch == EInter_ConfigEndlessDialSwitch::ON) {
            // removeTimer when RCS status changed [DISABLE -> ENABLE]
            removeTimer(mTimerIdNormalToErrorFor500ms);
            removeTimer(mTimerIdOffToReadyFor4000ms);
            if (mInput_RcsDriveModeOnOffStatus == EInput_RcsDriveModeOnOffStatus::TERRAIN_MODE) {
                if (mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::OFF) {
                    ret = EInter_TerrainModeSelectStatus::READY;
                } else if (mInter_ConfigOffRoad != EInter_ConfigOffRoad::OFF &&
                           mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::SNOW) {
                    ret = EInter_TerrainModeSelectStatus::SNOW;
                } else if (mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::MUD) {
                    ret = EInter_TerrainModeSelectStatus::MUD;
                } else if (mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::SAND) {
                    ret = EInter_TerrainModeSelectStatus::SAND;
                } else if (mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::AUTO) {
                    ret = EInter_TerrainModeSelectStatus::AUTO;
                } else if (mInput_RcsTerrainModeStatus == EInput_RcsTerrainModeStatus::INVALID) {
                    ret = EInter_TerrainModeSelectStatus::OFF;
                } else {
                    // no operation
                }
            } else {  // not TERRAIN_MODE
                ret = EInter_TerrainModeSelectStatus::OFF;
            }
        } else {  // RCS Controller [DISABLE] - [기존 터레인모드 동작은 차량기반-신호입력 동작은 "스위치 입력"]
            // 4.4.3 오프로드 모드 이벤트 판단(스위치 입력)
            if ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                 (mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CW ||
                  mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CW_LONG)) ||
                (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
                 (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
                  mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED))) {
                mPrivate_OffroadEvent = Private_OffroadEvent::CW;
            } else if (mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                       (mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CCW ||
                        mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::CCW_LONG)) {
                mPrivate_OffroadEvent = Private_OffroadEvent::CCW;
            } else {
                mPrivate_OffroadEvent = Private_OffroadEvent::OFF;
            }

            // 5.2.4.4.4.2 오프로드 모드 이벤트에 의한 제어
            if (selfValue() == EInter_TerrainModeSelectStatus::OFF &&
                (mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                 ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                   mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::PUSH_TERRAIN) ||
                  (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
                   (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
                    mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED))))) {
                ret = EInter_TerrainModeSelectStatus::READY;
            } else if (selfValue() == EInter_TerrainModeSelectStatus::READY &&
                       mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                       (mPrivate_OffroadEvent == Private_OffroadEvent::CW ||
                        mPrivate_OffroadEvent == Private_OffroadEvent::CCW)) {
                if (const auto& result = mPrivate_OffroadModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() != EInter_TerrainModeSelectStatus::READY &&
                       selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(selfValue()) == false &&
                       mPrivate_OffroadModeList.checkModeActivationStatus() == true) {
                if (const auto& result = mPrivate_OffroadModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() != EInter_TerrainModeSelectStatus::READY &&
                       selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                       mPrivate_OffroadEvent == Private_OffroadEvent::CW) {
                if (const auto& result = mPrivate_OffroadModeList.getNextEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() != EInter_TerrainModeSelectStatus::READY &&
                       selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                       mPrivate_OffroadEvent == Private_OffroadEvent::CCW) {
                if (const auto& result = mPrivate_OffroadModeList.getPrevEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrevInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
                       mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR) {
                mTimerIdNormalToErrorFor500ms =
                    Timer::addTimer(kTimerIntervalNormalToErrorForMs, this, &Inter_TerrainModeSelectStatus::onTimerNormalToError);
                mIsNeedFlushed = false;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       (mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                        ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                          mPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::PUSH_TERRAIN) ||
                         (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                          (mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED ||
                           mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::LONG_PRESSED))))) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE &&
                       mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeList.checkModeActivationStatus() == false) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       (mPrivate_OffroadModeSwitchableStatus == Private_OffroadModeSwitchableStatus::ENABLE &&
                        ((mPrevInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
                          mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::OFF) ||
                         (mPrevInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
                          mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::OFF)))) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mInput_AwdMode2HStatus == EInput_AwdMode2HStatus::ON) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mIMG_TelltaleTowMode_stat == EIMG_TelltaleTowMode_stat::ON) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else {
                // no operation
            }

            if (mInput_TerrainModeErrorStatus != EInput_TerrainModeErrorStatus::ERROR) {
                removeTimer(mTimerIdNormalToErrorFor500ms);
            }

            if (ret != EInter_TerrainModeSelectStatus::READY && ret != EInter_TerrainModeSelectStatus::MAX) {
                removeTimer(mTimerIdOffToReadyFor4000ms);
            } else if (selfValue() == EInter_TerrainModeSelectStatus::OFF && ret == EInter_TerrainModeSelectStatus::READY) {
                mTimerIdOffToReadyFor4000ms =
                    Timer::addTimer(kTimerIntervalOffToReadyForMs, this, &Inter_TerrainModeSelectStatus::onTimerOffToReady);
                mIsNeedFlushed = false;
            } else {
                // no operation
            }
        }
    } else if (mIgnStatus == false && mPrevIgnStatus == true) {
        removeTimer(mTimerIdNormalToErrorFor500ms);
        removeTimer(mTimerIdOffToReadyFor4000ms);
        mPrivate_OffroadEvent = Private_OffroadEvent::OFF;  // 5.2.4.4.3 오프로드 모드 이벤트 판단
        ret = EInter_TerrainModeSelectStatus::OFF;
    } else {
        // no operation
    }

    if (ret == EInter_TerrainModeSelectStatus::MAX) {
        DDebug() << "[Inter_TerrainModeSelectStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
        ret = selfValue();
    }
    DDebug() << "[Inter_TerrainModeSelectStatus] SPEC 5.2.4.4";
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_TerrainModeErrorStatus: "
             << static_cast<uint64_t>(mPrevInput_TerrainModeErrorStatus) << "->"
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_DriveModeSwStatus: "
             << static_cast<uint64_t>(mPrevPrivate_DriveModeSwStatus) << "->"
             << static_cast<uint64_t>(mPrivate_DriveModeSwStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListAuto_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListAuto_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListSnow_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListSnow_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListDeepSnow_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListDeepSnow_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListMud_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListMud_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListSand_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListSand_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListDesert_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListDesert_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] : mIMG_TerrainModePopupListRock_stat"
             << static_cast<uint64_t>(mIMG_TerrainModePopupListRock_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_NModeSelectStatus: " << static_cast<uint64_t>(mPrevInter_NModeSelectStatus)
             << "->" << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_DriveModeOnRoadSwitchStatus: "
             << static_cast<uint64_t>(mPrevInter_DriveModeOnRoadSwitchStatus) << "->"
             << static_cast<uint64_t>(mInter_DriveModeOnRoadSwitchStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mPrevInter_GtModeSelectStatus) << "->"
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_ConfigEndlessDialSwitch: "
             << static_cast<uint64_t>(mInter_ConfigEndlessDialSwitch.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_RcsDriveModeOnOffStatus: "
             << static_cast<uint64_t>(mInput_RcsDriveModeOnOffStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_RcsTerrainModeStatus: "
             << static_cast<uint64_t>(mInput_RcsTerrainModeStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] IMG_DriveModePopupListEco_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListEco_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] IMG_DriveModePopupListNormal_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListNormal_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] IMG_DriveModePopupListSport_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListSport_stat.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_BasicModeForSwKeySwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeForSwKeySwitchableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mInter_DriveModeChangeReqFromAVNT.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] PrevInter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mPrevInter_DriveModeChangeReqFromAVNT);
    DDebug() << "[Inter_TerrainModeSelectStatus] Ignition: " << mPrevIgnStatus << "->" << mIgnStatus.selfValue();
    DDebug() << "[Inter_TerrainModeSelectStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
    mPrevInter_NModeSelectStatus = mInter_NModeSelectStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    mPrevInter_GtModeSelectStatus = mInter_GtModeSelectStatus;
    mPrevIMG_TerrainModePopupListAuto_stat = mIMG_TerrainModePopupListAuto_stat;
    mPrevIMG_TerrainModePopupListSnow_stat = mIMG_TerrainModePopupListSnow_stat;
    mPrevIMG_TerrainModePopupListDeepSnow_stat = mIMG_TerrainModePopupListDeepSnow_stat;
    mPrevIMG_TerrainModePopupListMud_stat = mIMG_TerrainModePopupListMud_stat;
    mPrevIMG_TerrainModePopupListSand_stat = mIMG_TerrainModePopupListSand_stat;
    mPrevIMG_TerrainModePopupListDesert_stat = mIMG_TerrainModePopupListDesert_stat;
    mPrevIMG_TerrainModePopupListRock_stat = mIMG_TerrainModePopupListRock_stat;
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    mPrevIgnStatus = mIgnStatus;
    return ret;
}

// 5.2.4.5 N모드 상태 판단
ESFCDrive_ModeInter_NModeSelectStatus Inter_NModeSelectStatus::process() {
    ESFCDrive_ModeInter_NModeSelectStatus ret = ESFCDrive_ModeInter_NModeSelectStatus::MAX;

    if (mPrevIgnStatus == true && mIgnStatus == false) {  // ign on -> off
        ret = ESFCDrive_ModeInter_NModeSelectStatus::OFF;
    } else if (mIgnStatus == true && mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE) {
        if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
             selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
            (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
            (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::ON) &&
            (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
             mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::N;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
                    selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::ON) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::N;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::ON) &&
                   (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::OFF) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::N;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::ON) &&
                   (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::OFF) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::N;
        } else if ((selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::N &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::MAX) &&
                   (mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::N) &&
                   ((mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                     mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::LONG_PRESSED) ||
                    (mInter_NModeOnReq == EInter_NModeOnReq::ON_REQUEST))) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::N;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
                    selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::OFF) &&
                   (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::ON) &&
                   (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::NORMAL) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF ||
                    selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::OFF) &&
                   (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::ON) &&
                   (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::NORMAL) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::ON) &&
                   (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::NORMAL) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::ON) &&
                   (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::NORMAL) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1;
        } else if ((selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1 &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::MAX) &&
                   (mPrevInter_NCustom1AdminStatus == EInter_NCustom1AdminStatus::OFF &&
                    mInter_NCustom1AdminStatus == EInter_NCustom1AdminStatus::ADMIN)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchNModeSetStatus == EInput_N1SwitchNModeSetStatus::OFF) &&
                   (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::OFF) &&
                   (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::OFF) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchNModeSetStatus == EInput_N2SwitchNModeSetStatus::OFF) &&
                   (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::OFF) &&
                   (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchCustom1ModeSetStatus == EInput_N1SwitchCustom1ModeSetStatus::OFF) &&
                   (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::N) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchCustom1ModeSetStatus == EInput_N2SwitchCustom1ModeSetStatus::OFF) &&
                   (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
                   (mInput_N1SwitchMainSetStatus == EInput_N1SwitchMainSetStatus::N_MODE) &&
                   (mInput_N1SwitchCustom2ModeSetStatus == EInput_N1SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N1SwitchStatus == EInput_N1SwitchStatus::DEFAULT &&
                    mInput_N1SwitchStatus == EInput_N1SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1) &&
                   (mInput_N2SwitchMainSetStatus == EInput_N2SwitchMainSetStatus::N_MODE) &&
                   (mInput_N2SwitchCustom2ModeSetStatus == EInput_N2SwitchCustom2ModeSetStatus::ON) &&
                   (mInput_EscCustom2EnableStatus == EInput_EscCustom2EnableStatus::NORMAL) &&
                   (mPrevInput_N2SwitchStatus == EInput_N2SwitchStatus::DEFAULT &&
                    mInput_N2SwitchStatus == EInput_N2SwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2 &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::MAX) &&
                   (mPrevInter_NCustom2AdminStatus == EInter_NCustom2AdminStatus::OFF &&
                    mInter_NCustom2AdminStatus == EInter_NCustom2AdminStatus::ADMIN)) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2;
        } else if ((selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_NModeSelectStatus::MAX) &&
                   ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                     mPrivate_DriveModeSwStatus != EPrivate_DriveModeSwStatus::DEFAULT) ||
                    (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                     mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED) ||
                    (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
                     (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
                      mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED)))) {
            ret = ESFCDrive_ModeInter_NModeSelectStatus::OFF;
        } else {
            // no operation
        }
    }

    if (ret == ESFCDrive_ModeInter_NModeSelectStatus::MAX) {
        DDebug() << "[Inter_NModeSelectStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
        ret = selfValue();
    }

    DDebug() << "[Inter_NModeSelectStatus] SPEC 5.2.4.5";
    DDebug() << "[Inter_NModeSelectStatus] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N1SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchMainSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N1SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchNModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N1SwitchStatus: " << static_cast<uint64_t>(mPrevInput_N1SwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_N1SwitchStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N2SwitchMainSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchMainSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N2SwitchNModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchNModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N2SwitchStatus: " << static_cast<uint64_t>(mPrevInput_N2SwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_N2SwitchStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N1SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom2ModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N2SwitchCustom2ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom2ModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Inter_ConfigHighPerformance: "
             << static_cast<uint64_t>(mInter_ConfigHighPerformance.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Inter_DriveModeOnRoadSwitchStatus: "
             << static_cast<uint64_t>(mPrevInter_DriveModeOnRoadSwitchStatus) << "->"
             << static_cast<uint64_t>(mInter_DriveModeOnRoadSwitchStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N1SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N1SwitchCustom1ModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_EscCustom1EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom1EnableStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_N2SwitchCustom1ModeSetStatus: "
             << static_cast<uint64_t>(mInput_N2SwitchCustom1ModeSetStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Inter_NCustom1AdminStatus: " << static_cast<uint64_t>(mPrevInter_NCustom1AdminStatus)
             << "->" << static_cast<uint64_t>(mInter_NCustom1AdminStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Input_EscCustom2EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom2EnableStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Inter_NCustom2AdminStatus: " << static_cast<uint64_t>(mPrevInter_NCustom2AdminStatus)
             << "->" << static_cast<uint64_t>(mInter_NCustom2AdminStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] Private_DriveModeSwStatus: " << static_cast<uint64_t>(mPrevPrivate_DriveModeSwStatus)
             << "->" << static_cast<uint64_t>(mPrivate_DriveModeSwStatus.selfValue());
    DDebug() << "[Inter_NModeSelectStatus] IgnStatus: " << mPrevIgnStatus << "->" << mIgnStatus.selfValue();
    DDebug() << "[Inter_NModeSelectStatus] Result: " << static_cast<uint64_t>(ret);

    mPrevInput_N1SwitchStatus = mInput_N1SwitchStatus;
    mPrevInput_N2SwitchStatus = mInput_N2SwitchStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    mPrevInter_NCustom1AdminStatus = mInter_NCustom1AdminStatus;
    mPrevInter_NCustom2AdminStatus = mInter_NCustom2AdminStatus;
    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevIgnStatus = mIgnStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    return ret;
}
// 5.2.4.6 GT 모드 상태 판단
ESFCDrive_ModeInter_GtModeSelectStatus Inter_GtModeSelectStatus::process() {
    ESFCDrive_ModeInter_GtModeSelectStatus ret = ESFCDrive_ModeInter_GtModeSelectStatus::MAX;

    if (mPrevIgnStatus == true && mIgnStatus == false) {  // ign on -> off
        ret = ESFCDrive_ModeInter_GtModeSelectStatus::OFF;
    } else if (mIgnStatus == true && mInter_ConfigHighPerformance == EInter_ConfigHighPerformance::GT &&
               mInput_GearSelectStatusFromTCU != EInput_GearSelectStatusFromTCU::DS_MODE) {
        if ((selfValue() == ESFCDrive_ModeInter_GtModeSelectStatus::OFF ||
             selfValue() == ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) &&
            (mPrevInput_GtSwitchStatus == EInput_GtSwitchStatus::DEFAULT &&
             mInput_GtSwitchStatus == EInput_GtSwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_GtModeSelectStatus::GT;
        } else if ((selfValue() == ESFCDrive_ModeInter_GtModeSelectStatus::GT) &&
                   (mInput_EscCustom1EnableStatus == EInput_EscCustom1EnableStatus::NORMAL) &&
                   (mPrevInput_GtSwitchStatus == EInput_GtSwitchStatus::DEFAULT &&
                    mInput_GtSwitchStatus == EInput_GtSwitchStatus::PRESSED)) {
            ret = ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY;
        } else if ((selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY &&
                    selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::MAX) &&
                   (mPrevInter_GtMyAdminStatus == EInter_GtMyAdminStatus::OFF &&
                    mInter_GtMyAdminStatus == EInter_GtMyAdminStatus::ADMIN)) {
            ret = ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY;
        } else if ((selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::OFF &&
                    selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::NONE &&
                    selfValue() != ESFCDrive_ModeInter_GtModeSelectStatus::MAX) &&
                   ((mPrevPrivate_DriveModeSwStatus == EPrivate_DriveModeSwStatus::DEFAULT &&
                     mPrivate_DriveModeSwStatus != EPrivate_DriveModeSwStatus::DEFAULT) ||
                    (mPrevInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::UN_PRESSED &&
                     mInter_DriveModeOnRoadSwitchStatus == EInter_DriveModeOnRoadSwitchStatus::SHORT_PRESSED) ||
                    (mPrevInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::UN_PRESSED &&
                     (mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::SHORT_PRESSED ||
                      mInter_DriveModeTerrainSwitchStatus == EInter_DriveModeTerrainSwitchStatus::LONG_PRESSED)))) {
            ret = ESFCDrive_ModeInter_GtModeSelectStatus::OFF;
        } else {
            // no operation
        }
    } else {
        // no operation
    }

    if (ret == ESFCDrive_ModeInter_GtModeSelectStatus::MAX) {
        DDebug() << "[Inter_GtModeSelectStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
        ret = selfValue();
    }

    DDebug() << "[Inter_GtModeSelectStatus] SPEC 5.2.4.6";
    DDebug() << "[Inter_GtModeSelectStatus] Input_GearSelectStatusFromTCU: "
             << static_cast<uint64_t>(mInput_GearSelectStatusFromTCU.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Inter_ConfigHighPerformance: "
             << static_cast<uint64_t>(mInter_ConfigHighPerformance.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Input_EscCustom1EnableStatus: "
             << static_cast<uint64_t>(mInput_EscCustom1EnableStatus.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Input_GtSwitchStatus: " << static_cast<uint64_t>(mPrevInput_GtSwitchStatus) << "->"
             << static_cast<uint64_t>(mInput_GtSwitchStatus.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Inter_GtMyAdminStatus: " << static_cast<uint64_t>(mPrevInter_GtMyAdminStatus) << "->"
             << static_cast<uint64_t>(mInter_GtMyAdminStatus.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Private_DriveModeSwStatus: " << static_cast<uint64_t>(mPrevPrivate_DriveModeSwStatus)
             << "->" << static_cast<uint64_t>(mPrivate_DriveModeSwStatus.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] Inter_DriveModeOnRoadSwitchStatus: "
             << static_cast<uint64_t>(mPrevInter_DriveModeOnRoadSwitchStatus) << "->"
             << static_cast<uint64_t>(mInter_DriveModeOnRoadSwitchStatus.selfValue());
    DDebug() << "[Inter_GtModeSelectStatus] IgnStatus: " << mPrevIgnStatus << "->" << mIgnStatus.selfValue();
    DDebug() << "[Inter_GtModeSelectStatus] Result: " << static_cast<uint64_t>(ret);

    mPrevInput_GtSwitchStatus = mInput_GtSwitchStatus;
    mPrevInter_GtMyAdminStatus = mInter_GtMyAdminStatus;
    mPrevPrivate_DriveModeSwStatus = mPrivate_DriveModeSwStatus;
    mPrevInter_DriveModeOnRoadSwitchStatus = mInter_DriveModeOnRoadSwitchStatus;
    mPrevInter_DriveModeTerrainSwitchStatus = mInter_DriveModeTerrainSwitchStatus;
    mPrevIgnStatus = mIgnStatus;

    return ret;
}

// 5.2.4.7 드라이브 모드 상태 송출 [Output_DriveModeSwitchStatus]
EOutputDriveModeSwitchStatus Output_DriveModeSwitchStatus::process() {
    EOutputDriveModeSwitchStatus ret = selfValue();
    if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL &&
        mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
        mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
        mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
        ret = EOutputDriveModeSwitchStatus::NORMAL;
    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::ECO &&
               mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
               mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
               mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
        ret = EOutputDriveModeSwitchStatus::ECO;
    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT &&
               mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::OFF &&
               mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
               mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
        ret = EOutputDriveModeSwitchStatus::SPORT;
    } else if ((mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::NORMAL &&
                mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::ECO &&
                mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::SPORT &&
                mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::N &&
                mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1 &&
                mInter_NModeSelectStatus != ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2 &&
                mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT &&
                mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) ||
               mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM1 ||
               mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
        ret = EOutputDriveModeSwitchStatus::CUSTOM;
    } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::N ||
               mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT) {
        ret = EOutputDriveModeSwitchStatus::N_MODE;
    } else if (mInter_NModeSelectStatus == ESFCDrive_ModeInter_NModeSelectStatus::CUSTOM2) {
        ret = EOutputDriveModeSwitchStatus::CUSTOM2;
    } else {
        DDebug() << "[OutputDriveModeSwitchStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
    }
    DDebug() << "[OutputDriveModeSwitchStatus] SPEC 5.2.4.7";
    DDebug() << "[OutputDriveModeSwitchStatus] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[OutputDriveModeSwitchStatus] Inter_NModeSelectStatus: "
             << static_cast<uint64_t>(mInter_NModeSelectStatus.selfValue());
    DDebug() << "[OutputDriveModeSwitchStatus] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[OutputDriveModeSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.4.7 드라이브 모드 상태 송출 [Output_TerrainModeSwitchStatus]
EOutputTerrainModeSwitchStatus Output_TerrainModeSwitchStatus::process() {
    EOutputTerrainModeSwitchStatus ret = selfValue();
    if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF ||
        mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::READY) {
        ret = EOutputTerrainModeSwitchStatus::OFF;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SNOW) {
        ret = EOutputTerrainModeSwitchStatus::SNOW;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SAND) {
        ret = EOutputTerrainModeSwitchStatus::SAND;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DESERT) {
        ret = EOutputTerrainModeSwitchStatus::DESERT;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::MUD) {
        ret = EOutputTerrainModeSwitchStatus::MUD;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::AUTO) {
        ret = EOutputTerrainModeSwitchStatus::AUTO;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DEEP_SNOW) {
        ret = EOutputTerrainModeSwitchStatus::DEEP_SNOW;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::ROCK) {
        ret = EOutputTerrainModeSwitchStatus::ROCK;
    } else {
        DDebug() << "[OutputTerrainModeSwitchStatus] invalid request. keep current value (" << static_cast<uint64_t>(selfValue())
                 << ")";
    }
    DDebug() << "[OutputTerrainModeSwitchStatus] SPEC 5.2.4.7";
    DDebug() << "[OutputTerrainModeSwitchStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[OutputTerrainModeSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.4.7 드라이브 모드 상태 송출 [Output_TerrainModeMainSwitchStatus]
EOutputTerrainModeMainSwitchStatus Output_TerrainModeMainSwitchStatus::process() {
    EOutputTerrainModeMainSwitchStatus ret = selfValue();
    if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF) {
        ret = EOutputTerrainModeMainSwitchStatus::ON_ROAD;
    } else if (mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::MAX) {
        ret = EOutputTerrainModeMainSwitchStatus::OFF_ROAD;
    } else {
        DDebug() << "[OutputTerrainModeMainSwitchStatus] invalid request. keep current value ("
                 << static_cast<uint64_t>(selfValue()) << ")";
    }
    DDebug() << "[OutputTerrainModeMainSwitchStatus] SPEC 5.2.4.7";
    DDebug() << "[OutputTerrainModeMainSwitchStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[OutputTerrainModeMainSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.4.7 드라이브 모드 상태 송출 [Output_CustomDriveModeSetStatus]
// Apply [EXNCP-24845]
EOutputCustomDriveModeSetStatus Output_CustomDriveModeSetStatus::process() {
    EOutputCustomDriveModeSetStatus ret = EOutputCustomDriveModeSetStatus::INVALID;
    if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::MY_DRIVE ||
        mInter_GtModeSelectStatus == ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY) {
        ret = EOutputCustomDriveModeSetStatus::MY_DRIVE;
    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SMART) {
        ret = EOutputCustomDriveModeSetStatus::SMART;
    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::SNOW) {
        ret = EOutputCustomDriveModeSetStatus::SNOW_CUSTOM;
    } else if (mInter_DriveModeSelectStatus == ESFCDrive_ModeInter_DriveModeSelectStatus::CHAUFFEUR) {
        ret = EOutputCustomDriveModeSetStatus::CHAUFFEUR;
    } else if ((mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::MY_DRIVE &&
                mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::SMART &&
                mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::SNOW &&
                mInter_DriveModeSelectStatus != ESFCDrive_ModeInter_DriveModeSelectStatus::CHAUFFEUR) &&
               (mInter_GtModeSelectStatus != ESFCDrive_ModeInter_GtModeSelectStatus::GT_MY)) {
        ret = EOutputCustomDriveModeSetStatus::INVALID;
        DDebug() << "[OutputCustomDriveModeSetStatus] Not MY_DRIVE/SMART/SNOW or invalid value.";
    } else {
        // no operation
    }
    DDebug() << "[OutputCustomDriveModeSetStatus] SPEC 5.2.4.7";
    DDebug() << "[OutputCustomDriveModeSetStatus] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[OutputCustomDriveModeSetStatus] Inter_GtModeSelectStatus: "
             << static_cast<uint64_t>(mInter_GtModeSelectStatus.selfValue());
    DDebug() << "[OutputCustomDriveModeSetStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}
// 5.2.4.7 드라이브 모드 상태 송출 [Output_CustomDriveModeApplyStatus]
EOutputCustomDriveModeApplyStatus Output_CustomDriveModeApplyStatus::process() {
    EOutputCustomDriveModeApplyStatus ret = EOutputCustomDriveModeApplyStatus::OFF;
    DDebug() << "[OutputCustomDriveModeApplyStatus] SPEC 5.2.4.7";
    DDebug() << "[OutputCustomDriveModeApplyStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.2.4.8 모드 조작 불가 반환 정보 제공
EInter_DriveModeChangeResultStatus Inter_DriveModeChangeResultStatus::process() {
    EInter_DriveModeChangeResultStatus ret = EInter_DriveModeChangeResultStatus::NONE;
    if (mIgnStatus == true) {
        if ((mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::AVAILABLE &&
             mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
             mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
             mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) ||
            (mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::AVAILABLE &&
             mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
             mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
             mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) ||
            (mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::AVAILABLE &&
             mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE &&
             mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
             mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT)) {
            ret = EInter_DriveModeChangeResultStatus::SUCCESS;
        } else if ((mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::NONE &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) ||
                   (mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::NONE &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) ||
                   (mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::NONE &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT)) {
            ret = EInter_DriveModeChangeResultStatus::NOT_SUPPORTED;
        } else if (((mIMG_DriveModePopupListEco_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListEco_stat::UNAVAILABLE ||
                     mPrivate_BasicModeForSwKeySwitchableStatus != EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE) &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) ||
                   ((mIMG_DriveModePopupListNormal_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListNormal_stat::UNAVAILABLE ||
                     mPrivate_BasicModeForSwKeySwitchableStatus != EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE) &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) ||
                   ((mIMG_DriveModePopupListSport_stat == ESFCDrive_ModeConstantIMG_DriveModePopupListSport_stat::UNAVAILABLE ||
                     mPrivate_BasicModeForSwKeySwitchableStatus != EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE) &&
                    mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT)) {
            ret = EInter_DriveModeChangeResultStatus::INVALID_OPERATION;
        } else {
            // no operation
        }
    } else {
        ret = EInter_DriveModeChangeResultStatus::INVALID_OPERATION;
    }

    // On Condition 의 세부조건이 발생할 때마다 동일한 VALUE 라도 재활성화한다.
    if (ret != EInter_DriveModeChangeResultStatus::NONE && ret == selfValue()) {
        mIsForcedFlush = true;
    } else {
        mIsForcedFlush = false;
    }

    DDebug() << "[Inter_DriveModeChangeResultStatus] SPEC 5.2.4.8";
    DDebug() << "[Inter_DriveModeChangeResultStatus] IMG_DriveModePopupListEco_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListEco_stat.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] IMG_DriveModePopupListNormal_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListNormal_stat.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] IMG_DriveModePopupListSport_stat: "
             << static_cast<uint64_t>(mIMG_DriveModePopupListSport_stat.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] Private_BasicModeForSwKeySwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeForSwKeySwitchableStatus.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] Inter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mInter_DriveModeChangeReqFromAVNT.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] PrevInter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mPrevInter_DriveModeChangeReqFromAVNT);
    DDebug() << "[Inter_DriveModeChangeResultStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    return ret;
}

}  // namespace ccos
