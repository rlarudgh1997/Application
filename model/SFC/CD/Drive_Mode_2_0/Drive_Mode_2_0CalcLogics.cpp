/**
 * @file Drive_Mode_2_0CalcLogics.cpp
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
#include <Timer.h>

namespace ccos {
// 5.3.4.2.1 스위치 입력을 통한 기본 모드 전환 가능여부 판단
EPrivate_BasicModeSwitchableStatus Private_BasicModeSwitchableStatus::process() {
    EPrivate_BasicModeSwitchableStatus ret = EPrivate_BasicModeSwitchableStatus::DISABLE;
    if (mInter_ConfigDriveModeOptionWithVpc != EInter_ConfigDriveModeOptionWithVpc::ON &&
        mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF) {
        ret = EPrivate_BasicModeSwitchableStatus::ENABLE;
    }
    DDebug() << "[Private_BasicModeSwitchableStatus] SPEC 5.3.4.2.1";
    DDebug() << "[Private_BasicModeSwitchableStatus] Inter_ConfigDriveModeOptionWithVpc: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeOptionWithVpc.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[Private_BasicModeSwitchableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.2 스위치 외 입력을 통한 기본 모드 전환 가능여부 판단
EPrivate_BasicModeForSwKeySwitchableStatus Private_BasicModeForSwKeySwitchableStatus::process() {
    EPrivate_BasicModeForSwKeySwitchableStatus ret = EPrivate_BasicModeForSwKeySwitchableStatus::DISABLE;
    if (mInter_ConfigDriveModeOptionWithVpc != EInter_ConfigDriveModeOptionWithVpc::ON) {
        ret = EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE;
    }
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] SPEC 5.3.4.2.2";
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Inter_ConfigDriveModeOptionWithVpc: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeOptionWithVpc.selfValue());
    DDebug() << "[Private_BasicModeForSwKeySwitchableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeEcoAvailableStatus)
EPrivate_BasicModeEcoAvailableStatus Private_BasicModeEcoAvailableStatus::process() {
    EPrivate_BasicModeEcoAvailableStatus ret = EPrivate_BasicModeEcoAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_Eco == EInter_ConfigOipDriveMode_Eco::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_BasicModeEcoAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_BasicModeEcoAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_BasicModeEcoAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeEcoAvailableStatus";
    DDebug() << "[Private_BasicModeEcoAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeEcoAvailableStatus] Inter_ConfigOipDriveMode_Eco: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_Eco.selfValue());
    DDebug() << "[Private_BasicModeEcoAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_BasicModeEcoAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeNormalAvailableStatus)
EPrivate_BasicModeNormalAvailableStatus Private_BasicModeNormalAvailableStatus::process() {
    EPrivate_BasicModeNormalAvailableStatus ret = EPrivate_BasicModeNormalAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_Normal == EInter_ConfigOipDriveMode_Normal::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_BasicModeNormalAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_BasicModeNormalAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_BasicModeNormalAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeNormalAvailableStatus";
    DDebug() << "[Private_BasicModeNormalAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeNormalAvailableStatus] Inter_ConfigOipDriveMode_Normal: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_Normal.selfValue());
    DDebug() << "[Private_BasicModeNormalAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_BasicModeNormalAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeSportAvailableStatus)
EPrivate_BasicModeSportAvailableStatus Private_BasicModeSportAvailableStatus::process() {
    EPrivate_BasicModeSportAvailableStatus ret = EPrivate_BasicModeSportAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_Sport == EInter_ConfigOipDriveMode_Sport::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_BasicModeSportAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_BasicModeSportAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_BasicModeSportAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeSportAvailableStatus";
    DDebug() << "[Private_BasicModeSportAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeSportAvailableStatus] Inter_ConfigOipDriveMode_Sport: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_Sport.selfValue());
    DDebug() << "[Private_BasicModeSportAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_BasicModeSportAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeMyAvailableStatus)
EPrivate_BasicModeMyAvailableStatus Private_BasicModeMyAvailableStatus::process() {
    EPrivate_BasicModeMyAvailableStatus ret = EPrivate_BasicModeMyAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_My == EInter_ConfigOipDriveMode_My::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_BasicModeMyAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_BasicModeMyAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_BasicModeMyAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeMyAvailableStatus";
    DDebug() << "[Private_BasicModeMyAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeMyAvailableStatus] Inter_ConfigOipDriveMode_My: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_My.selfValue());
    DDebug() << "[Private_BasicModeMyAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_BasicModeMyAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeSmartAvailableStatus)
EPrivate_BasicModeSmartAvailableStatus Private_BasicModeSmartAvailableStatus::process() {
    EPrivate_BasicModeSmartAvailableStatus ret = EPrivate_BasicModeSmartAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigCustomSmart == EInter_ConfigCustomSmart::ON) {
        ret = EPrivate_BasicModeSmartAvailableStatus::AVAILABLE;
    }
    DDebug() << "[Private_BasicModeSmartAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeSmartAvailableStatus";
    DDebug() << "[Private_BasicModeSmartAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeSmartAvailableStatus] Inter_ConfigCustomSmart: "
             << static_cast<uint64_t>(mInter_ConfigCustomSmart.selfValue());
    DDebug() << "[Private_BasicModeSmartAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeSnowAvailableStatus)
EPrivate_BasicModeSnowAvailableStatus Private_BasicModeSnowAvailableStatus::process() {
    EPrivate_BasicModeSnowAvailableStatus ret = EPrivate_BasicModeSnowAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_Snow == EInter_ConfigOipDriveMode_Snow::ON) {
        ret = EPrivate_BasicModeSnowAvailableStatus::AVAILABLE;
    }
    DDebug() << "[Private_BasicModeSnowAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeSnowAvailableStatus";
    DDebug() << "[Private_BasicModeSnowAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeSnowAvailableStatus] Inter_ConfigOipDriveMode_Snow: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_Snow.selfValue());
    DDebug() << "[Private_BasicModeSnowAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeChauffeurAvailableStatus)
EPrivate_BasicModeChauffeurAvailableStatus Private_BasicModeChauffeurAvailableStatus::process() {
    EPrivate_BasicModeChauffeurAvailableStatus ret = EPrivate_BasicModeChauffeurAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_Chauffeur == EInter_ConfigOipDriveMode_Chauffeur::ON) {
        ret = EPrivate_BasicModeChauffeurAvailableStatus::AVAILABLE;
    }
    DDebug() << "[Private_BasicModeChauffeurAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeChauffeurAvailableStatus";
    DDebug() << "[Private_BasicModeChauffeurAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeChauffeurAvailableStatus] Inter_ConfigOipDriveMode_Chauffeur: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_Chauffeur.selfValue());
    DDebug() << "[Private_BasicModeChauffeurAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.3 기본 모드별 활성화 여부 판단 (Private_BasicModeSportPlusAvailableStatus)
EPrivate_BasicModeSportPlusAvailableStatus Private_BasicModeSportPlusAvailableStatus::process() {
    EPrivate_BasicModeSportPlusAvailableStatus ret = EPrivate_BasicModeSportPlusAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigOipDriveMode_SportPlus == EInter_ConfigOipDriveMode_SportPlus::ON) {
        ret = EPrivate_BasicModeSportPlusAvailableStatus::AVAILABLE;
    }
    DDebug() << "[Private_BasicModeSportPlusAvailableStatus] SPEC 5.3.4.2.3 Private_BasicModeSportPlusAvailableStatus";
    DDebug() << "[Private_BasicModeSportPlusAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_BasicModeSportPlusAvailableStatus] Inter_ConfigOipDriveMode_SportPlus: "
             << static_cast<uint64_t>(mInter_ConfigOipDriveMode_SportPlus.selfValue());
    DDebug() << "[Private_BasicModeSportPlusAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.2.6 기본 모드 전환 제어
EInter_DriveModeSelectStatus Inter_DriveModeSelectStatus::process() {
    EInter_DriveModeSelectStatus ret = EInter_DriveModeSelectStatus::NONE;

    // 5.3.4.2.4 기본 모드 리스트
    if (mPrevPrivate_BasicModeEcoAvailableStatus != mPrivate_BasicModeEcoAvailableStatus) {
        if (mPrivate_BasicModeEcoAvailableStatus == EPrivate_BasicModeEcoAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::ECO);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::ECO, false);
        }
    }
    if (mPrevPrivate_BasicModeNormalAvailableStatus != mPrivate_BasicModeNormalAvailableStatus) {
        if (mPrivate_BasicModeNormalAvailableStatus == EPrivate_BasicModeNormalAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::NORMAL);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::NORMAL, false);
        }
    }
    if (mPrevPrivate_BasicModeSportAvailableStatus != mPrivate_BasicModeSportAvailableStatus) {
        if (mPrivate_BasicModeSportAvailableStatus == EPrivate_BasicModeSportAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORT);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORT, false);
        }
    }
    if (mPrevPrivate_BasicModeSportPlusAvailableStatus != mPrivate_BasicModeSportPlusAvailableStatus) {
        if (mPrivate_BasicModeSportPlusAvailableStatus == EPrivate_BasicModeSportPlusAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORTPLUS);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SPORTPLUS, false);
        }
    }
    if (mPrevPrivate_BasicModeChauffeurAvailableStatus != mPrivate_BasicModeChauffeurAvailableStatus) {
        if (mPrivate_BasicModeChauffeurAvailableStatus == EPrivate_BasicModeChauffeurAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::CHAUFFEUR);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::CHAUFFEUR, false);
        }
    }
    if (mPrevPrivate_BasicModeMyAvailableStatus != mPrivate_BasicModeMyAvailableStatus) {
        if (mPrivate_BasicModeMyAvailableStatus == EPrivate_BasicModeMyAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::MY);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::MY, false);
        }
    }
    if (mPrevPrivate_BasicModeSmartAvailableStatus != mPrivate_BasicModeSmartAvailableStatus) {
        if (mPrivate_BasicModeSmartAvailableStatus == EPrivate_BasicModeSmartAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SMART);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SMART, false);
        }
    }
    if (mPrevPrivate_BasicModeSnowAvailableStatus != mPrivate_BasicModeSnowAvailableStatus) {
        if (mPrivate_BasicModeSnowAvailableStatus == EPrivate_BasicModeSnowAvailableStatus::AVAILABLE) {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SNOW);
        } else {
            mPrivate_BasicModeList.enableMode(EInter_DriveModeSelectStatus::SNOW, false);
        }
    }

    if (mInter_ConfigDriveModeOptionWithVpc == EInter_ConfigDriveModeOptionWithVpc::ON) {
        // 5.3.4.2.6.1 VPC 에 의한 제어
        if (mInput_DmicTerrainModeMainSwitchStatus != EInput_DmicTerrainModeMainSwitchStatus::OFF_ROAD) {
            switch (mInput_DmicDriveModeSwitchStatus) {
                case EInput_DmicDriveModeSwitchStatus::ECO:
                    ret = EInter_DriveModeSelectStatus::ECO;
                    break;
                case EInput_DmicDriveModeSwitchStatus::NORMAL:
                    ret = EInter_DriveModeSelectStatus::NORMAL;
                    break;
                case EInput_DmicDriveModeSwitchStatus::SPORT:
                    ret = EInter_DriveModeSelectStatus::SPORT;
                    break;
                case EInput_DmicDriveModeSwitchStatus::SPORTPLUS:
                    ret = EInter_DriveModeSelectStatus::SPORTPLUS;
                    break;
                case EInput_DmicDriveModeSwitchStatus::CUSTOM:
                    switch (mInput_DmicCustomDriveModeSetStatus) {
                        case EInput_DmicCustomDriveModeSetStatus::CHAUFFEUR:
                            ret = EInter_DriveModeSelectStatus::CHAUFFEUR;
                            break;
                        case EInput_DmicCustomDriveModeSetStatus::MY_DRIVE:
                            ret = EInter_DriveModeSelectStatus::MY;
                            break;
                        case EInput_DmicCustomDriveModeSetStatus::SMART:
                            ret = EInter_DriveModeSelectStatus::SMART;
                            break;
                        case EInput_DmicCustomDriveModeSetStatus::SNOW_CUSTOM:
                            ret = EInter_DriveModeSelectStatus::SNOW;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    } else {
        if (mIgnStatus == true) {
            // 5.3.4.2.5 스위치 입력을 통한 기본 모드 변경 이벤트 발생 판단
            if (mPrevInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::UN_PRESSED &&
                mInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::SHORT_PRESSED) {
                mPrivate_BasicEvent = EPrivate_BasicEvent::CW;
            } else {
                mPrivate_BasicEvent = EPrivate_BasicEvent::OFF;
            }

            // 5.3.4.2.6.2 기본 모드 스위치 입력 이벤트에 의한 제어
            if (mPrivate_BasicEvent == EPrivate_BasicEvent::CW &&
                mPrivate_BasicModeSwitchableStatus == EPrivate_BasicModeSwitchableStatus::ENABLE) {
                if (const auto& result = mPrivate_BasicModeList.getNextEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (mPrivate_BasicModeList.verifyExistCurrentMode(selfValue()) == false) {
                if (const auto& result = mPrivate_BasicModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else {
                // no operation
            }

            // 5.3.4.2.6.3 기본 모드 스위치 외 입력 이벤트에 의한 제어
            if (mInter_ConfigDriveModeOptionWithVpc == EInter_ConfigDriveModeOptionWithVpc::OFF &&
                mPrivate_BasicModeForSwKeySwitchableStatus == EPrivate_BasicModeForSwKeySwitchableStatus::ENABLE) {
                bool isUpdate = true;
                mIsDriveModeChangeRequestedFromAVNT = true;
                if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                    mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                    mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::ECO) == true) {
                    ret = EInter_DriveModeSelectStatus::ECO;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::NORMAL) == true) {
                    ret = EInter_DriveModeSelectStatus::NORMAL;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::SPORT) == true) {
                    ret = EInter_DriveModeSelectStatus::SPORT;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORTPLUS &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORTPLUS &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::SPORTPLUS) == true) {
                    ret = EInter_DriveModeSelectStatus::SPORTPLUS;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_CHAUFFEUR &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_CHAUFFEUR &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::CHAUFFEUR) == true) {
                    ret = EInter_DriveModeSelectStatus::CHAUFFEUR;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_MY &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_MY &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::MY) == true) {
                    ret = EInter_DriveModeSelectStatus::MY;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SMART &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SMART &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::SMART) == true) {
                    ret = EInter_DriveModeSelectStatus::SMART;
                } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SNOW &&
                           mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SNOW &&
                           mPrivate_BasicModeList.verifyExistCurrentMode(EInter_DriveModeSelectStatus::SNOW) == true) {
                    ret = EInter_DriveModeSelectStatus::SNOW;
                } else {
                    isUpdate = false;
                    mIsDriveModeChangeRequestedFromAVNT = false;
                }
                if (isUpdate == true) {
                    mPrivate_BasicModeList.setCurrentMode(ret);
                }
            }
        } else if (mPrevIgnStatus == true && mIgnStatus == false) {
            mPrivate_BasicEvent = EPrivate_BasicEvent::OFF;
            bool isUpdate = true;
            // 5.3.4.2.6.4 IGN1 On ⇒ Off 이벤트 발생시 드라이브 모드 제어
            if (mInter_ConfigDriveModeOptionWithVpc == EInter_ConfigDriveModeOptionWithVpc::OFF) {
                if ((selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON &&
                     mInter_ConfigDriveModeKeepEco != EInter_ConfigDriveModeKeepEco::ON) ||
                    (selfValue() == EInter_DriveModeSelectStatus::NORMAL &&
                     mInter_ConfigDriveModeKeepNormal != EInter_ConfigDriveModeKeepNormal::ON) ||
                    (selfValue() == EInter_DriveModeSelectStatus::SPORT &&
                     mInter_ConfigDriveModeKeepSport != EInter_ConfigDriveModeKeepSport::ON) ||
                    (selfValue() != EInter_DriveModeSelectStatus::ECO && selfValue() != EInter_DriveModeSelectStatus::NORMAL &&
                     selfValue() != EInter_DriveModeSelectStatus::SPORT)) {
                    if (mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::ECO) {
                        ret = EInter_DriveModeSelectStatus::ECO;
                    } else if (mInter_ConfigDriveModeDefault == EInter_ConfigDriveModeDefault::NORMAL) {
                        ret = EInter_DriveModeSelectStatus::NORMAL;
                    } else {
                        isUpdate = false;
                    }
                } else {
                    if (selfValue() == EInter_DriveModeSelectStatus::ECO && mInter_ConfigEcoMode == EInter_ConfigEcoMode::ON &&
                        mInter_ConfigDriveModeKeepEco == EInter_ConfigDriveModeKeepEco::ON) {
                        ret = EInter_DriveModeSelectStatus::ECO;
                    } else if (selfValue() == EInter_DriveModeSelectStatus::NORMAL &&
                               mInter_ConfigDriveModeKeepNormal == EInter_ConfigDriveModeKeepNormal::ON) {
                        ret = EInter_DriveModeSelectStatus::NORMAL;
                    } else if (selfValue() == EInter_DriveModeSelectStatus::SPORT &&
                               mInter_ConfigDriveModeKeepSport == EInter_ConfigDriveModeKeepSport::ON) {
                        ret = EInter_DriveModeSelectStatus::SPORT;
                    } else {
                        isUpdate = false;
                    }
                }
                if (isUpdate == true) {
                    setNeedFlushed(true);
                    mPrivate_BasicModeList.setCurrentMode(ret);
                }
            }
        } else {
            mPrivate_BasicEvent = EPrivate_BasicEvent::OFF;
        }
    }

    if (ret == EInter_DriveModeSelectStatus::NONE) {
        DDebug() << " invalid request. keep current value (" << static_cast<uint64_t>(selfValue()) << ")";
        ret = selfValue();
    }

    DDebug() << "[Inter_DriveModeSelectStatus] SPEC 5.3.4.2";
    DDebug() << "[Inter_DriveModeSelectStatus] Ignition: " << mPrevIgnStatus << " => " << mIgnStatus.selfValue();
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeEcoAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeEcoAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeEcoAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeNormalAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeNormalAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeNormalAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeSportAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeSportAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeSportAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeMyAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeMyAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeMyAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeSmartAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeSmartAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeSmartAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeSnowAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeSnowAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeSnowAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeChauffeurAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeChauffeurAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeChauffeurAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeSportPlusAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_BasicModeSportPlusAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_BasicModeSportPlusAvailableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigDriveModeOptionWithVpc: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeOptionWithVpc.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Input_DmicTerrainModeMainSwitchStatus: "
             << static_cast<uint64_t>(mInput_DmicTerrainModeMainSwitchStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Input_DmicDriveModeSwitchStatus: "
             << static_cast<uint64_t>(mInput_DmicDriveModeSwitchStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Input_DmicCustomDriveModeSetStatus: "
             << static_cast<uint64_t>(mInput_DmicCustomDriveModeSetStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicEvent: " << static_cast<uint64_t>(mPrivate_BasicEvent);
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeSwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeSwitchableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Private_BasicModeForSwKeySwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_BasicModeForSwKeySwitchableStatus.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mPrevInter_DriveModeChangeReqFromAVNT) << " => "
             << static_cast<uint64_t>(mInter_DriveModeChangeReqFromAVNT.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigEcoMode: " << static_cast<uint64_t>(mInter_ConfigEcoMode.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigDriveModeKeepEco: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepEco.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigDriveModeKeepNormal: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepNormal.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigDriveModeKeepSport: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeKeepSport.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Inter_ConfigDriveModeDefault: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeDefault.selfValue());
    DDebug() << "[Inter_DriveModeSelectStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevIgnStatus = mIgnStatus;
    mPrevPrivate_BasicModeEcoAvailableStatus = mPrivate_BasicModeEcoAvailableStatus;
    mPrevPrivate_BasicModeNormalAvailableStatus = mPrivate_BasicModeNormalAvailableStatus;
    mPrevPrivate_BasicModeSportAvailableStatus = mPrivate_BasicModeSportAvailableStatus;
    mPrevPrivate_BasicModeMyAvailableStatus = mPrivate_BasicModeMyAvailableStatus;
    mPrevPrivate_BasicModeSmartAvailableStatus = mPrivate_BasicModeSmartAvailableStatus;
    mPrevPrivate_BasicModeSnowAvailableStatus = mPrivate_BasicModeSnowAvailableStatus;
    mPrevPrivate_BasicModeChauffeurAvailableStatus = mPrivate_BasicModeChauffeurAvailableStatus;
    mPrevPrivate_BasicModeSportPlusAvailableStatus = mPrivate_BasicModeSportPlusAvailableStatus;
    mPrevInput_DynamicControlForOipSwitchStatus = mInput_DynamicControlForOipSwitchStatus;
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeSnowAvailableStatus)
EPrivate_TerrainModeSnowAvailableStatus Private_TerrainModeSnowAvailableStatus::process() {
    EPrivate_TerrainModeSnowAvailableStatus ret = EPrivate_TerrainModeSnowAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainSnow == EInter_ConfigTerrainSnow::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeSnowAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeSnowAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeSnowAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeSnowAvailableStatus";
    DDebug() << "[Private_TerrainModeSnowAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeSnowAvailableStatus] Inter_ConfigTerrainSnow: "
             << static_cast<uint64_t>(mInter_ConfigTerrainSnow.selfValue());
    DDebug() << "[Private_TerrainModeSnowAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeSnowAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeMudAvailableStatus)
EPrivate_TerrainModeMudAvailableStatus Private_TerrainModeMudAvailableStatus::process() {
    EPrivate_TerrainModeMudAvailableStatus ret = EPrivate_TerrainModeMudAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainMud == EInter_ConfigTerrainMud::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeMudAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeMudAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeMudAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeMudAvailableStatus";
    DDebug() << "[Private_TerrainModeMudAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeMudAvailableStatus] Inter_ConfigTerrainMud: "
             << static_cast<uint64_t>(mInter_ConfigTerrainMud.selfValue());
    DDebug() << "[Private_TerrainModeMudAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeMudAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeSandAvailableStatus)
EPrivate_TerrainModeSandAvailableStatus Private_TerrainModeSandAvailableStatus::process() {
    EPrivate_TerrainModeSandAvailableStatus ret = EPrivate_TerrainModeSandAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainSand == EInter_ConfigTerrainSand::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeSandAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeSandAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeSandAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeSandAvailableStatus";
    DDebug() << "[Private_TerrainModeSandAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeSandAvailableStatus] Inter_ConfigTerrainSand: "
             << static_cast<uint64_t>(mInter_ConfigTerrainSand.selfValue());
    DDebug() << "[Private_TerrainModeSandAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeSandAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeAutoAvailableStatus)
EPrivate_TerrainModeAutoAvailableStatus Private_TerrainModeAutoAvailableStatus::process() {
    EPrivate_TerrainModeAutoAvailableStatus ret = EPrivate_TerrainModeAutoAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainAuto == EInter_ConfigTerrainAuto::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeAutoAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeAutoAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeAutoAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeAutoAvailableStatus";
    DDebug() << "[Private_TerrainModeAutoAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeAutoAvailableStatus] Inter_ConfigTerrainAuto: "
             << static_cast<uint64_t>(mInter_ConfigTerrainAuto.selfValue());
    DDebug() << "[Private_TerrainModeAutoAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeAutoAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeDeepSnowAvailableStatus)
EPrivate_TerrainModeDeepSnowAvailableStatus Private_TerrainModeDeepSnowAvailableStatus::process() {
    EPrivate_TerrainModeDeepSnowAvailableStatus ret = EPrivate_TerrainModeDeepSnowAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainDeepSnow == EInter_ConfigTerrainDeepSnow::ON) {
        ret = EPrivate_TerrainModeDeepSnowAvailableStatus::AVAILABLE;
    }
    DDebug() << "[Private_TerrainModeDeepSnowAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeDeepSnowAvailableStatus";
    DDebug() << "[Private_TerrainModeDeepSnowAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeDeepSnowAvailableStatus] Inter_ConfigTerrainDeepSnow: "
             << static_cast<uint64_t>(mInter_ConfigTerrainDeepSnow.selfValue());
    DDebug() << "[Private_TerrainModeDeepSnowAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeRockAvailableStatus)
EPrivate_TerrainModeRockAvailableStatus Private_TerrainModeRockAvailableStatus::process() {
    EPrivate_TerrainModeRockAvailableStatus ret = EPrivate_TerrainModeRockAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainRock == EInter_ConfigTerrainRock::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeRockAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeRockAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeRockAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeRockAvailableStatus";
    DDebug() << "[Private_TerrainModeRockAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeRockAvailableStatus] Inter_ConfigTerrainRock: "
             << static_cast<uint64_t>(mInter_ConfigTerrainRock.selfValue());
    DDebug() << "[Private_TerrainModeRockAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeRockAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.1 오프로드 모드별 활성화 여부 판단 (Private_TerrainModeDesertAvailableStatus)
EPrivate_TerrainModeDesertAvailableStatus Private_TerrainModeDesertAvailableStatus::process() {
    EPrivate_TerrainModeDesertAvailableStatus ret = EPrivate_TerrainModeDesertAvailableStatus::NONE;
    if (mIgnStatus == true && mInter_ConfigTerrainDesert == EInter_ConfigTerrainDesert::ON) {
        if (mInput_AwdModeDriveModeControlStatus != true) {
            ret = EPrivate_TerrainModeDesertAvailableStatus::AVAILABLE;
        } else {
            ret = EPrivate_TerrainModeDesertAvailableStatus::UNAVAILABLE;
        }
    }
    DDebug() << "[Private_TerrainModeDesertAvailableStatus] SPEC 5.3.4.3.1 Private_TerrainModeDesertAvailableStatus";
    DDebug() << "[Private_TerrainModeDesertAvailableStatus] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_TerrainModeDesertAvailableStatus] Inter_ConfigTerrainDesert: "
             << static_cast<uint64_t>(mInter_ConfigTerrainDesert.selfValue());
    DDebug() << "[Private_TerrainModeDesertAvailableStatus] Input_AwdModeDriveModeControlStatus: "
             << static_cast<uint64_t>(mInput_AwdModeDriveModeControlStatus.selfValue());
    DDebug() << "[Private_TerrainModeDesertAvailableStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.3.3 스위치 입력을 통한 오프로드 모드 변경 이벤트 발생 판단
EPrivate_OffroadEvent Private_OffroadEvent::process() {
    EPrivate_OffroadEvent ret = EPrivate_OffroadEvent::OFF;
    if (mIgnStatus == true) {
        if (mPrevInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::UN_PRESSED &&
            mInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::SHORT_PRESSED) {
            ret = EPrivate_OffroadEvent::CW;
        } else if (mPrevInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::UN_PRESSED &&
                   mInput_DynamicControlForOipSwitchStatus == EInput_DynamicControlForOipSwitchStatus::LONG_PRESSED) {
            ret = EPrivate_OffroadEvent::TERRAIN_CHANGE;
        } else {
            // no operation
        }
    }
    DDebug() << "[Private_OffroadEvent] SPEC 5.3.4.3.3";
    DDebug() << "[Private_OffroadEvent] Ignition: " << mIgnStatus.selfValue();
    DDebug() << "[Private_OffroadEvent] Input_DynamicControlForOipSwitchStatus: "
             << static_cast<uint64_t>(mPrevInput_DynamicControlForOipSwitchStatus) << " => "
             << static_cast<uint64_t>(mInput_DynamicControlForOipSwitchStatus.selfValue());
    DDebug() << "[Private_OffroadEvent] Result: " << static_cast<uint64_t>(ret);
    mPrevInput_DynamicControlForOipSwitchStatus = mInput_DynamicControlForOipSwitchStatus;
    return ret;
}

// 5.3.4.3.4 오프로드 모드 전환 제어
EInter_TerrainModeSelectStatus Inter_TerrainModeSelectStatus::process() {
    EInter_TerrainModeSelectStatus ret = EInter_TerrainModeSelectStatus::NONE;

    // 5.3.4.3.1 오프로드 모드 전환 동작 가능여부 판단
    if (mInter_ConfigDriveModeOptionWithVpc != EInter_ConfigDriveModeOptionWithVpc::ON &&
        mInter_ConfigOffRoad != EInter_ConfigOffRoad::OFF &&
        mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL) {
        mPrivate_OffroadModeSwitchableStatus = EPrivate_OffroadModeSwitchableStatus::ENABLE;
    } else {
        mPrivate_OffroadModeSwitchableStatus = EPrivate_OffroadModeSwitchableStatus::DISABLE;
    }

    // 5.3.4.3.2 오프로드 모드 리스트
    if (mPrevPrivate_TerrainModeAutoAvailableStatus != mPrivate_TerrainModeAutoAvailableStatus) {
        if (mPrivate_TerrainModeAutoAvailableStatus == EPrivate_TerrainModeAutoAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::AUTO);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::AUTO, false);
        }
    }
    if (mPrevPrivate_TerrainModeSnowAvailableStatus != mPrivate_TerrainModeSnowAvailableStatus) {
        if (mPrivate_TerrainModeSnowAvailableStatus == EPrivate_TerrainModeSnowAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SNOW);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SNOW, false);
        }
    }
    if (mPrevPrivate_TerrainModeDeepSnowAvailableStatus != mPrivate_TerrainModeDeepSnowAvailableStatus) {
        if (mPrivate_TerrainModeDeepSnowAvailableStatus == EPrivate_TerrainModeDeepSnowAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DEEP_SNOW);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DEEP_SNOW, false);
        }
    }
    if (mPrevPrivate_TerrainModeMudAvailableStatus != mPrivate_TerrainModeMudAvailableStatus) {
        if (mPrivate_TerrainModeMudAvailableStatus == EPrivate_TerrainModeMudAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::MUD);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::MUD, false);
        }
    }
    if (mPrevPrivate_TerrainModeSandAvailableStatus != mPrivate_TerrainModeSandAvailableStatus) {
        if (mPrivate_TerrainModeSandAvailableStatus == EPrivate_TerrainModeSandAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SAND);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::SAND, false);
        }
    }
    if (mPrevPrivate_TerrainModeDesertAvailableStatus != mPrivate_TerrainModeDesertAvailableStatus) {
        if (mPrivate_TerrainModeDesertAvailableStatus == EPrivate_TerrainModeDesertAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DESERT);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::DESERT, false);
        }
    }
    if (mPrevPrivate_TerrainModeRockAvailableStatus != mPrivate_TerrainModeRockAvailableStatus) {
        if (mPrivate_TerrainModeRockAvailableStatus == EPrivate_TerrainModeRockAvailableStatus::AVAILABLE) {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::ROCK);
        } else {
            mPrivate_OffroadModeList.enableMode(EInter_TerrainModeSelectStatus::ROCK, false);
        }
    }

    if (mIgnStatus == true) {
        if (mInter_ConfigDriveModeOptionWithVpc == EInter_ConfigDriveModeOptionWithVpc::ON) {
            removeTimer(mTimerIdNormalToErrorFor500ms);
            removeTimer(mTimerIdOffToReadyFor4000ms);
            // 5.3.4.3.4.1 VPC 에 의한 제어
            if (mInput_DmicTerrainModeMainSwitchStatus == EInput_DmicTerrainModeMainSwitchStatus::OFF_ROAD) {
                switch (mInput_DmicTerrainModeSwitchStatus) {
                    case EInput_DmicTerrainModeSwitchStatus::OFF:
                        ret = EInter_TerrainModeSelectStatus::READY;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::AUTO:
                        ret = EInter_TerrainModeSelectStatus::AUTO;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::SNOW:
                        ret = EInter_TerrainModeSelectStatus::SNOW;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::DEEP_SNOW:
                        ret = EInter_TerrainModeSelectStatus::DEEP_SNOW;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::MUD:
                        ret = EInter_TerrainModeSelectStatus::MUD;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::SAND:
                        ret = EInter_TerrainModeSelectStatus::SAND;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::DESERT:
                        ret = EInter_TerrainModeSelectStatus::DESERT;
                        break;
                    case EInput_DmicTerrainModeSwitchStatus::ROCK:
                        ret = EInter_TerrainModeSelectStatus::ROCK;
                        break;
                    default:
                        ret = EInter_TerrainModeSelectStatus::OFF;
                        break;
                }
            } else {
                ret = EInter_TerrainModeSelectStatus::OFF;
            }
        } else {
            // 5.3.4.3.4.2 오프로드 모드 스위치 입력 이벤트에 의한 제어
            if (selfValue() == EInter_TerrainModeSelectStatus::OFF &&
                mPrivate_OffroadModeSwitchableStatus == EPrivate_OffroadModeSwitchableStatus::ENABLE &&
                mPrivate_OffroadEvent == EPrivate_OffroadEvent::TERRAIN_CHANGE) {
                ret = EInter_TerrainModeSelectStatus::READY;
            } else if (selfValue() == EInter_TerrainModeSelectStatus::READY &&
                       mPrivate_OffroadModeSwitchableStatus == EPrivate_OffroadModeSwitchableStatus::ENABLE &&
                       mPrivate_OffroadEvent == EPrivate_OffroadEvent::CW) {
                if (const auto& result = mPrivate_OffroadModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() == EInter_TerrainModeSelectStatus::READY &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_DEFAULT &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_DEFAULT) {
                if (const auto& result = mPrivate_OffroadModeList.getFirstEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                    mIsTerrainModeChangeRequestedFromAVNT = true;
                } else {
                    mIsTerrainModeChangeRequestedFromAVNT = false;
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
                       mPrivate_OffroadModeSwitchableStatus == EPrivate_OffroadModeSwitchableStatus::ENABLE &&
                       mPrivate_OffroadEvent == EPrivate_OffroadEvent::CW) {
                if (const auto& result = mPrivate_OffroadModeList.getNextEnabledMode(); result.has_value() == true) {
                    ret = result.value();
                }
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrevInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
                       mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR) {
                mTimerIdNormalToErrorFor500ms =
                    Timer::addTimer(kTimerIntervalNormalToErrorForMs, this, &Inter_TerrainModeSelectStatus::onTimerNormalToError);
                mIsNeedFlushed = false;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeSwitchableStatus == EPrivate_OffroadModeSwitchableStatus::ENABLE &&
                       mPrivate_OffroadEvent == EPrivate_OffroadEvent::TERRAIN_CHANGE) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() != EInter_TerrainModeSelectStatus::OFF &&
                       mPrivate_OffroadModeList.checkModeActivationStatus() == false) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else if (selfValue() == EInter_TerrainModeSelectStatus::READY &&
                       mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_CANCEL &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_CANCEL) {
                ret = EInter_TerrainModeSelectStatus::OFF;
            } else {
                // no operation
            }

            if (mInput_TerrainModeErrorStatus != EInput_TerrainModeErrorStatus::ERROR) {
                removeTimer(mTimerIdNormalToErrorFor500ms);
            }

            if (ret != EInter_TerrainModeSelectStatus::READY && ret != EInter_TerrainModeSelectStatus::MAX &&
                ret != EInter_TerrainModeSelectStatus::NONE) {
                removeTimer(mTimerIdOffToReadyFor4000ms);
            } else if (selfValue() == EInter_TerrainModeSelectStatus::OFF && ret == EInter_TerrainModeSelectStatus::READY) {
                mTimerIdOffToReadyFor4000ms =
                    Timer::addTimer(kTimerIntervalOffToReadyForMs, this, &Inter_TerrainModeSelectStatus::onTimerOffToReady);
                mIsNeedFlushed = false;
            } else {
                // no operation
            }
        }

        // 5.3.4.3.4.3 오프로드 모드 스위치 외 입력 이벤트에 의한 제어
        if (mPrivate_OffroadModeSwitchableStatus == EPrivate_OffroadModeSwitchableStatus::ENABLE) {
            bool isUpdate = true;
            mIsTerrainModeChangeRequestedFromAVNT = true;
            if ((mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_ECO &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_ECO) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_NORMAL) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORT &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORT) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SPORTPLUS &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SPORTPLUS) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_CHAUFFEUR &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_CHAUFFEUR) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_MY &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_MY) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SMART &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SMART) ||
                (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::DRV_SNOW &&
                 mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::DRV_SNOW)) {
                ret = EInter_TerrainModeSelectStatus::OFF;
                isUpdate = false;
                mIsTerrainModeChangeRequestedFromAVNT = false;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_READY &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_READY) {
                ret = EInter_TerrainModeSelectStatus::READY;
                isUpdate = false;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_AUTO &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_AUTO &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::AUTO) == true) {
                ret = EInter_TerrainModeSelectStatus::AUTO;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_SNOW &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_SNOW &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::SNOW) == true) {
                ret = EInter_TerrainModeSelectStatus::SNOW;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_DEEPSNOW &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_DEEPSNOW &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::DEEP_SNOW) == true) {
                ret = EInter_TerrainModeSelectStatus::DEEP_SNOW;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_MUD &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_MUD &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::MUD) == true) {
                ret = EInter_TerrainModeSelectStatus::MUD;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_SAND &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_SAND &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::SAND) == true) {
                ret = EInter_TerrainModeSelectStatus::SAND;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_DESERT &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_DESERT &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::DESERT) == true) {
                ret = EInter_TerrainModeSelectStatus::DESERT;
            } else if (mPrevInter_DriveModeChangeReqFromAVNT != EInter_DriveModeChangeReqFromAVNT::TER_ROCK &&
                       mInter_DriveModeChangeReqFromAVNT == EInter_DriveModeChangeReqFromAVNT::TER_ROCK &&
                       mPrivate_OffroadModeList.verifyExistCurrentMode(EInter_TerrainModeSelectStatus::ROCK) == true) {
                ret = EInter_TerrainModeSelectStatus::ROCK;
            } else {
                isUpdate = false;
                mIsTerrainModeChangeRequestedFromAVNT = false;
            }

            if (isUpdate == true) {
                mPrivate_OffroadModeList.setCurrentMode(ret);
            }
        }
    } else {
        // NOTE: Private_OffroadEvent Off case when IGN off (EXNCP-37388)
        removeTimer(mTimerIdNormalToErrorFor500ms);
        removeTimer(mTimerIdOffToReadyFor4000ms);
        mPrivate_OffroadEvent = EPrivate_OffroadEvent::OFF;
        ret = EInter_TerrainModeSelectStatus::OFF;
    }

    if (ret == EInter_TerrainModeSelectStatus::NONE) {
        DDebug() << __FUNCTION__ << " invalid request. keep current value (" << static_cast<uint64_t>(selfValue()) << ")";
        ret = selfValue();
    }
    DDebug() << "[Inter_TerrainModeSelectStatus] SPEC 5.2.4.4";
    DDebug() << "[Inter_TerrainModeSelectStatus] Ignition: " << mPrevIgnStatus << " => " << mIgnStatus.selfValue();
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeAutoAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeAutoAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeAutoAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeSnowAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeSnowAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeSnowAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeDeepSnowAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeDeepSnowAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeDeepSnowAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeMudAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeMudAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeMudAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeSandAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeSandAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeSandAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeDesertAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeDesertAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeDesertAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_TerrainModeRockAvailableStatus: "
             << static_cast<uint64_t>(mPrevPrivate_TerrainModeRockAvailableStatus) << " => "
             << static_cast<uint64_t>(mPrivate_TerrainModeRockAvailableStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_ConfigDriveModeOptionWithVpc: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeOptionWithVpc.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_DmicTerrainModeMainSwitchStatus: "
             << static_cast<uint64_t>(mInput_DmicTerrainModeMainSwitchStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_DmicTerrainModeSwitchStatus: "
             << static_cast<uint64_t>(mInput_DmicTerrainModeSwitchStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_OffroadModeSwitchableStatus: "
             << static_cast<uint64_t>(mPrivate_OffroadModeSwitchableStatus);
    DDebug() << "[Inter_TerrainModeSelectStatus] Private_OffroadEvent: "
             << static_cast<uint64_t>(mPrivate_OffroadEvent.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Inter_DriveModeChangeReqFromAVNT: "
             << static_cast<uint64_t>(mPrevInter_DriveModeChangeReqFromAVNT) << " => "
             << static_cast<uint64_t>(mInter_DriveModeChangeReqFromAVNT.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Input_TerrainModeErrorStatus: "
             << static_cast<uint64_t>(mPrevInput_TerrainModeErrorStatus) << " => "
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus.selfValue());
    DDebug() << "[Inter_TerrainModeSelectStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevIgnStatus = mIgnStatus;
    mPrevPrivate_TerrainModeAutoAvailableStatus = mPrivate_TerrainModeAutoAvailableStatus;
    mPrevPrivate_TerrainModeSnowAvailableStatus = mPrivate_TerrainModeSnowAvailableStatus;
    mPrevPrivate_TerrainModeDeepSnowAvailableStatus = mPrivate_TerrainModeDeepSnowAvailableStatus;
    mPrevPrivate_TerrainModeMudAvailableStatus = mPrivate_TerrainModeMudAvailableStatus;
    mPrevPrivate_TerrainModeSandAvailableStatus = mPrivate_TerrainModeSandAvailableStatus;
    mPrevPrivate_TerrainModeDesertAvailableStatus = mPrivate_TerrainModeDesertAvailableStatus;
    mPrevPrivate_TerrainModeRockAvailableStatus = mPrivate_TerrainModeRockAvailableStatus;
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    mPrevInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
    return ret;
}

// 5.3.4.4.1 드라이브 모드 상태 송출 [Output_DriveModeSwitchStatus]
EOutputDriveModeSwitchStatus Output_DriveModeSwitchStatus::process() {
    EOutputDriveModeSwitchStatus ret = EOutputDriveModeSwitchStatus::CUSTOM;
    if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::ECO) {
        ret = EOutputDriveModeSwitchStatus::ECO;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::NORMAL) {
        ret = EOutputDriveModeSwitchStatus::NORMAL;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SPORT) {
        ret = EOutputDriveModeSwitchStatus::SPORT;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SPORTPLUS) {
        ret = EOutputDriveModeSwitchStatus::SPORT_PLUS;
    } else {
        // no operation
    }
    DDebug() << "[Output_DriveModeSwitchStatus] SPEC 5.3.4.4.1";
    DDebug() << "[Output_DriveModeSwitchStatus] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[Output_DriveModeSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.4.1 드라이브 모드 상태 송출 [Output_CustomDriveModeSetStatus]
EOutputCustomDriveModeSetStatus Output_CustomDriveModeSetStatus::process() {
    EOutputCustomDriveModeSetStatus ret = EOutputCustomDriveModeSetStatus::INVALID;
    if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::MY) {
        ret = EOutputCustomDriveModeSetStatus::MY_DRIVE;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SMART) {
        ret = EOutputCustomDriveModeSetStatus::SMART;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::SNOW) {
        ret = EOutputCustomDriveModeSetStatus::SNOW_CUSTOM;
    } else if (mInter_DriveModeSelectStatus == EInter_DriveModeSelectStatus::CHAUFFEUR) {
        ret = EOutputCustomDriveModeSetStatus::CHAUFFEUR;
    } else {
        // no operation
    }
    DDebug() << "Output_CustomDriveModeSetStatus SPEC 5.3.4.4.1";
    DDebug() << "Output_CustomDriveModeSetStatus Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "Output_CustomDriveModeSetStatus Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.4.1 드라이브 모드 상태 송출 [Output_TerrainModeSwitchStatus]
EOutputTerrainModeSwitchStatus Output_TerrainModeSwitchStatus::process() {
    EOutputTerrainModeSwitchStatus ret = EOutputTerrainModeSwitchStatus::OFF;
    if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::AUTO) {
        ret = EOutputTerrainModeSwitchStatus::AUTO;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SNOW) {
        ret = EOutputTerrainModeSwitchStatus::SNOW;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DEEP_SNOW) {
        ret = EOutputTerrainModeSwitchStatus::DEEP_SNOW;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::MUD) {
        ret = EOutputTerrainModeSwitchStatus::MUD;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::SAND) {
        ret = EOutputTerrainModeSwitchStatus::SAND;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::DESERT) {
        ret = EOutputTerrainModeSwitchStatus::DESERT;
    } else if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::ROCK) {
        ret = EOutputTerrainModeSwitchStatus::ROCK;
    } else {
        // no operation
    }
    DDebug() << "[Output_TerrainModeSwitchStatus] SPEC 5.3.4.4.1";
    DDebug() << "[Output_TerrainModeSwitchStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[Output_TerrainModeSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.4.1 드라이브 모드 상태 송출 [Output_TerrainModeMainSwitchStatus]
EOutputTerrainModeMainSwitchStatus Output_TerrainModeMainSwitchStatus::process() {
    EOutputTerrainModeMainSwitchStatus ret = EOutputTerrainModeMainSwitchStatus::OFF_ROAD;
    if (mInter_TerrainModeSelectStatus == EInter_TerrainModeSelectStatus::OFF) {
        ret = EOutputTerrainModeMainSwitchStatus::ON_ROAD;
    }
    DDebug() << "[Output_TerrainModeMainSwitchStatus] SPEC 5.3.4.4.1";
    DDebug() << "[Output_TerrainModeMainSwitchStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[Output_TerrainModeMainSwitchStatus] Result: " << static_cast<uint64_t>(ret);
    return ret;
}

// 5.3.4.4.2.1 오프로드 모드 진입 불가 알림 정보 송출
EInter_DynamicControlPopupStatus Inter_DynamicControlPopupStatus::process() {
    EInter_DynamicControlPopupStatus ret = EInter_DynamicControlPopupStatus::NONE;
    if (mIgnStatus == true) {
        if (mInter_ConfigDriveModeOptionWithVpc != EInter_ConfigDriveModeOptionWithVpc::ON) {
            if (((mPrevInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::NORMAL &&
                  mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR) ||
                 (mInput_TerrainModeErrorStatus == EInput_TerrainModeErrorStatus::ERROR &&
                  mPrivate_OffroadEvent == EPrivate_OffroadEvent::TERRAIN_CHANGE))) {
                if (mInter_ConfigOffRoad == EInter_ConfigOffRoad::TERRAIN) {
                    ret = EInter_DynamicControlPopupStatus::TERRAIN_UNAVAILABLE;
                } else if (mInter_ConfigOffRoad == EInter_ConfigOffRoad::TRACTION) {
                    ret = EInter_DynamicControlPopupStatus::TRACTION_UNAVAILABLE;
                } else {
                    // no operation
                }
            }
        }
        if (selfValue() != ret) {
            mTimerId4SecAfterValueChanged = Timer::addTimer(kTimerInterval4SecAfterValueChanged, this,
                                                            &Inter_DynamicControlPopupStatus::onTimer4SecAfterValueChanged);
            mIsNeedFlushed = false;
        }
    } else {
        removeTimer(mTimerId4SecAfterValueChanged);
        ret = EInter_DynamicControlPopupStatus::SIGNATURE_NULL;
    }

    DDebug() << "[Inter_DynamicControlPopupStatus] SPEC 5.3.4.4.2.1";
    DDebug() << "[Inter_DynamicControlPopupStatus] IgnStatus: " << static_cast<uint64_t>(mIgnStatus.selfValue());
    DDebug() << "[Inter_DynamicControlPopupStatus] Inter_ConfigDriveModeOptionWithVpc: "
             << static_cast<uint64_t>(mInter_ConfigDriveModeOptionWithVpc.selfValue());
    DDebug() << "[Inter_DynamicControlPopupStatus] Inter_ConfigOffRoad: "
             << static_cast<uint64_t>(mInter_ConfigOffRoad.selfValue());
    DDebug() << "[Inter_DynamicControlPopupStatus] Input_TerrainModeErrorStatus: "
             << static_cast<uint64_t>(mPrevInput_TerrainModeErrorStatus) << " => "
             << static_cast<uint64_t>(mInput_TerrainModeErrorStatus.selfValue());
    DDebug() << "[Inter_DynamicControlPopupStatus] Private_OffroadEvent: "
             << static_cast<uint64_t>(mPrivate_OffroadEvent.selfValue());
    DDebug() << "[Inter_DynamicControlPopupStatus] Result: " << static_cast<uint64_t>(ret);
    mPrevInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
    return ret;
}

// 5.3.4.4.3 스위치 외 입력 이벤트에 대한 피드백 정보 제공
EInter_DriveModeChangeResultStatus Inter_DriveModeChangeResultStatus::process() {
    EInter_DriveModeChangeResultStatus ret = EInter_DriveModeChangeResultStatus::NONE;
    if (mIgnStatus == true) {
        if ((mInter_DriveModeSelectStatus != EInter_DriveModeSelectStatus::MAX &&
             mInter_DriveModeSelectStatus != EInter_DriveModeSelectStatus::NONE &&
             mPrevInter_DriveModeSelectStatus == mInter_DriveModeSelectStatus && mIsDriveModeChangeRequestedFromAVNT == true) ||
            (mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
             mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::NONE &&
             mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::MAX &&
             mPrevInter_TerrainModeSelectStatus == mInter_TerrainModeSelectStatus &&
             mIsTerrainModeChangeRequestedFromAVNT == true) ||
            (mInter_DriveModeSelectStatus != EInter_DriveModeSelectStatus::MAX &&
             mInter_DriveModeSelectStatus != EInter_DriveModeSelectStatus::NONE &&
             mPrevInter_DriveModeSelectStatus != mInter_DriveModeSelectStatus) ||
            (mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::OFF &&
             mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::NONE &&
             mInter_TerrainModeSelectStatus != EInter_TerrainModeSelectStatus::MAX &&
             mPrevInter_TerrainModeSelectStatus != mInter_TerrainModeSelectStatus)) {
            mIsDriveModeChangeRequestedFromAVNT = false;
            mIsTerrainModeChangeRequestedFromAVNT = false;
            ret = EInter_DriveModeChangeResultStatus::SUCCESS;
        } else {
            ret = EInter_DriveModeChangeResultStatus::NOT_SUPPORTED;
        }
    } else {
        ret = EInter_DriveModeChangeResultStatus::INVALID_OPERATION;
    }

    // NOTE : Inter_DriveModeSelectStatus 또는 Inter_TerrainModeSelectStatus 동일한 모드 값 반환 시
    if (ret != EInter_DriveModeChangeResultStatus::NONE && ret == selfValue()) {
        mIsForcedFlush = true;
    } else {
        mIsForcedFlush = false;
    }

    DDebug() << "[Inter_DriveModeChangeResultStatus] SPEC 5.3.4.4.3";
    DDebug() << "[Inter_DriveModeChangeResultStatus] IgnStatus: " << static_cast<uint64_t>(mIgnStatus.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] Inter_DriveModeSelectStatus: "
             << static_cast<uint64_t>(mPrevInter_DriveModeSelectStatus) << " => "
             << static_cast<uint64_t>(mInter_DriveModeSelectStatus.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] Inter_TerrainModeSelectStatus: "
             << static_cast<uint64_t>(mPrevInter_TerrainModeSelectStatus) << " => "
             << static_cast<uint64_t>(mInter_TerrainModeSelectStatus.selfValue());
    DDebug() << "[Inter_DriveModeChangeResultStatus] mIsDriveModeChangeRequestedFromAVNT: "
             << static_cast<uint64_t>(mIsDriveModeChangeRequestedFromAVNT);
    DDebug() << "[Inter_DriveModeChangeResultStatus] mIsTerrainModeChangeRequestedFromAVNT: "
             << static_cast<uint64_t>(mIsTerrainModeChangeRequestedFromAVNT);
    DDebug() << "[Inter_DriveModeChangeResultStatus] Inter_DriveModeChangeResultStatus Result: " << static_cast<uint64_t>(ret);
    mPrevInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
    mPrevInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
    mPrevInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
    return ret;
}

}  // namespace ccos
