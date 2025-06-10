/**
 * @file Illumination_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Illumination_2_0_H
#define SFSS_Illumination_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Illumination_2_0Base.hpp"
#include <array>
#include <cstring>
#include <SFCTimer.h>
#include <Vehicle.h>

namespace ccos {

// SFC Version : 2.0.0
// Reference : [CD051] Illumination_2_0. Recommend: VALUE_CHANGED
class Illumination_2_0 : public Illumination_2_0Base {
public:
    Illumination_2_0() = default;
    ~Illumination_2_0() override = default;
    Illumination_2_0(const Illumination_2_0& other) = delete;
    Illumination_2_0(Illumination_2_0&& other) noexcept = delete;
    Illumination_2_0& operator=(const Illumination_2_0& other) = delete;
    Illumination_2_0& operator=(Illumination_2_0&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerPrivateiRheostatSwitchOperation500ms.create(
                kTimerInterval500ms, this, &Illumination_2_0::onOneShotTimerPrvateiRheostatSwitchOperation500ms, false) ==
            false) {
            DWarning() << "[Illumination_2_0] mOneShotTimerPrivateiRheostatSwitchOperation500ms Create Failed";
        }

        if (mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.create(
                kTimerInterval100ms, this, &Illumination_2_0::onRepeatTimerPrivateRheostatSwitchOperation100ms, true) == false) {
            DWarning() << "[Illumination_2_0] mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms Create Failed";
        }

        setSFCIllumination_2_0Inter_RheostatLevelStatus(mInter_RheostatLevelStatus);
        setSFCIllumination_2_0Inter_ClusterLightingLevel(mInter_ClusterLightingLevel);
        updateOutputIlluminationLevelValue();
        updateOutputIlluminationDutyValue();
        updateOutputDetentStatus();
        updateOutputNotMinIlluminationLevelValue();
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            loadMemoryPrivateNaviMapStatus();
            loadMemoryRheostatLevelStatus();
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn0ms) == true) {
            mIsIgnOn = true;
            updateInterMinLightMode(true);
            updatePrivateLcdLightingStatus();
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff0ms) == true) {
            mIsIgnOn = false;
            updateInterMinLightMode(true);
            updatePrivateLcdLightingStatus();
        }
    }

    void onConfigAutolightChanged(const ArgumentsConfigAutolightChanged& args) {
        if (updateValueIfValid(mInter_ConfigAutolight, args.mInter_ConfigAutolight) == true) {
            updateInterIllCluster(false);
            if (isHuOptionApplied() == false) {
                updateInterRheostatLevelStatus(true);
            }
            updateInterRheostatMaxInfo();
        }
    }

    void onDayAndNightModeChanged(const ArgumentsDayAndNightModeChanged& args) {
        HBool isInput_ExtLowBeamOnReqStatusChanged =
            updateValueIfValid(mInput_ExtLowBeamOnReqStatus, args.mInput_ExtLowBeamOnReqStatus);
        HBool isInput_AvTailLampStatusChanged = updateValueIfValid(mInput_AvTailLampStatus, args.mInput_AvTailLampStatus);
        HBool isInput_AutolightSensorNightStatusChanged =
            updateValueIfValid(mInput_AutolightSensorNightStatus, args.mInput_AutolightSensorNightStatus);

        if (isInput_ExtLowBeamOnReqStatusChanged == true || isInput_AvTailLampStatusChanged == true ||
            isInput_AutolightSensorNightStatusChanged == true) {
            updateInterIllCluster(true);
        }
    }

    void onDayNightStateChanged(const ArgumentsDayNightStateChanged& args) {
        if (updateValueIfValid(mInter_DayNightState, args.mInter_DayNightState) == true) {
            updatePrivateNaviMapStatus(true);
        }
    }

    void onNaviMapDayNightStatusChanged(const ArgumentsNaviMapDayNightStatusChanged& args) {
        if (updateValueIfValid(mInput_NaviMapDayNightStatus, args.mInput_NaviMapDayNightStatus) == true) {
            updatePrivateNaviMapStatus(false);
        }
    }

    void onMinLightModeChanged(const ArgumentsMinLightModeChanged& args) {
        HBool isInter_ConfigVehicleTypeChanged = updateValueIfValid(mInter_ConfigVehicleType, args.mInter_ConfigVehicleType);
        HBool isInput_UtilityModeOnOffStatusChanged =
            updateValueIfValid(mInput_UtilityModeOnOffStatus, args.mInput_UtilityModeOnOffStatus);
        HBool isInter_MinimizeInteriorLightsStatusChanged =
            updateValueIfValid(mInter_MinimizeInteriorLightsStatus, args.mInter_MinimizeInteriorLightsStatus);

        if (isInter_ConfigVehicleTypeChanged == true || isInput_UtilityModeOnOffStatusChanged == true ||
            isInter_MinimizeInteriorLightsStatusChanged == true) {
            updateInterMinLightMode();
        }
    }

    void onConfigAvnBrightLinkChanged(const ArgumentsConfigAvnBrightLinkChanged& args) {
        if (updateValueIfValid(mInter_ConfigAvnBrightLink, args.mInter_ConfigAvnBrightLink) == true) {
            if (isHuOptionApplied() == false) {
                updatePrivateRheostatSwitchOperation();
                updateInterRheostatLevelStatus(true);
            } else {
                updateClusterLightingLevel();
            }
            updateInterIllumLcdDuty();
            updateInterRheostatMaxInfo();
        }
    }

    void onRheostatSwitchStatusChanged(const ArgumentsRheostatSwitchStatusChanged& args) {
        if (updateValueIfValid(mInter_RheostatSwitchStatus, args.mInter_RheostatSwitchStatus) == true) {
            updatePrivateRheostatSwitchOperation();
            mPrevInter_RheostatSwitchStatus = mInter_RheostatSwitchStatus;
        }
    }

    void onSetRheoStatLvlChanged(const ArgumentsSetRheoStatLvlChanged& args) {
        if (updateValueIfValid(mInter_SetRheoStatLvl, args.mInter_SetRheoStatLvl) == true) {
            updateInterRheostatLevelStatus(true);
        }
    }

    void onAutoBrightLevelChanged(const ArgumentsAutoBrightLevelChanged& args) {
        if (updateValueIfValid(mInter_AutoBrightLevel, args.mInter_AutoBrightLevel) == true) {
            updateClusterLightingLevel();
        }
    }

    void onLcdLightingStatusChanged(const ArgumentsLcdLightingStatusChanged& args) {
        if (updateValueIfValid(mInter_LcdLightingStatus, args.mInter_LcdLightingStatus) == true) {
            updatePrivateLcdLightingStatus();
        }
    }

    void onParamIllumLCDChanged(const ArgumentsParamIllumLCDChanged& args) {
        std::string paramString;
        if (updateValueIfValid(paramString, args.mPar_IllumDayLCD) == true) {
            copyParamStringToUInt16Array(paramString, mIllumDayLcdParamDataArray);
            updatePrivateDayLcdDutySetValue();
        }

        if (updateValueIfValid(paramString, args.mPar_IllumNightLCD) == true) {
            copyParamStringToUInt16Array(paramString, mIllumNightLcdParamDataArray);
            updatePrivateNightLcdDutySetValue();
        }
    }

    void onMemoryNaviMapStatusChanged(const ArgumentsMemoryNaviMapStatusChanged& args) {
        Memory_NaviMapStatus tmpMemory = Memory_NaviMapStatus::NONE;
        if (updateValueIfValid(tmpMemory, args.mInter_NaviMapStatus) == true && tmpMemory != Memory_NaviMapStatus::NONE) {
            mMemoryPrivate_NaviMapStatus = tmpMemory;
            loadMemoryPrivateNaviMapStatus();
        } else {
            DWarning() << "[Illumination_2_0] Failed to load memory value: SFC.Extension.Memory_IGN.Inter_NaviMapStatus";
        }
    }

    void onMemoryRheostatLevelStatusChanged(const ArgumentsMemoryRheostatLevelStatusChanged& args) {
        HUInt64 tmpMemory = 0;
        if (updateValueIfValid(tmpMemory, args.mInter_RheostatLevelStatus) == true && tmpMemory != 0) {
            mMemoryInter_RheostatLevelStatus = static_cast<Inter_RheostatLevelStatus>(tmpMemory);
            loadMemoryRheostatLevelStatus();
        } else {
            DWarning() << "[Illumination_2_0] Failed to load memory value: SFC.Extension.Memory.Inter_RheostatLevelStatus";
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4 연산 로직
    // 4.1 주야간 모드 판단
    void updateInterIllCluster(const HBool& isChainCallRequired) {
        Inter_IllCluster stat = Inter_IllCluster::NIGHT;
        if ((mInter_ConfigAutolight == Inter_ConfigAutolight::OFF &&
             mInput_ExtLowBeamOnReqStatus == Input_ExtLowBeamOnReqStatus::OFF) ||
            (mInter_ConfigAutolight == Inter_ConfigAutolight::ON && mInput_AvTailLampStatus == Input_AvTailLampStatus::OFF &&
             mInput_AutolightSensorNightStatus == Input_AutolightSensorNightStatus::OFF)) {
            stat = Inter_IllCluster::DAY;
        } else if (mInter_ConfigAutolight == Inter_ConfigAutolight::ON &&
                   mInput_AvTailLampStatus == Input_AvTailLampStatus::OFF &&
                   mInput_AutolightSensorNightStatus == Input_AutolightSensorNightStatus::NIGHT) {
            stat = Inter_IllCluster::DUSK;
        } else {
            // no operation
        }
        setSFCIllumination_2_0Inter_IllCluster(stat);
        _DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_IllCluster == " << static_cast<HUInt64>(stat) << "("
                 << static_cast<HUInt64>(mInter_IllCluster) << ")";
        mInter_IllCluster = stat;
        if (isChainCallRequired == true) {
            updateClusterLightingLevel();
        }
    }

    // 4.2 내비게이션 맵 상태
    void updatePrivateNaviMapStatus(const HBool isInterChanged) {
        HBool isChanged = false;
        if (isInterChanged == true) {
            if (mInter_DayNightState == Inter_DayNightState::DAY) {
                mPrivate_NaviMapStatus = Private_NaviMapStatus::DAY;
                isChanged = true;
            } else if (mPrivate_NaviMapStatus == Private_NaviMapStatus::DAY &&
                       (mInter_DayNightState == Inter_DayNightState::NIGHT ||
                        mInter_DayNightState == Inter_DayNightState::EARLY_EVENING)) {
                mPrivate_NaviMapStatus = Private_NaviMapStatus::DAY;
                isChanged = true;
            } else {
                // no operation
            }
        } else {
            if (mInput_NaviMapDayNightStatus == Input_NaviMapDayNightStatus::NIGHT) {
                mPrivate_NaviMapStatus = Private_NaviMapStatus::NIGHT;
                isChanged = true;
            } else if (mPrivate_NaviMapStatus == Private_NaviMapStatus::NIGHT &&
                       mInput_NaviMapDayNightStatus == Input_NaviMapDayNightStatus::INVALID) {
                mPrivate_NaviMapStatus = Private_NaviMapStatus::NIGHT;
                isChanged = true;
            } else {
                // no operation
            }
        }

        if (isChanged == true) {
            saveMemoryPrivateNaviMapStatus();
        }
        updateInterIllumLcdDuty();
    }

    // 사양 및 이슈 이력 없음. 하지만 기존 Illumination 에서 Private_NaviMapStatus 를 저장하기 때문에 저장 및 불러오기 함.
    void loadMemoryPrivateNaviMapStatus() {
        if (mIsAEMInitializeComplete == true) {
            if (mMemoryPrivate_NaviMapStatus != Memory_NaviMapStatus::NONE &&
                mMemoryPrivate_NaviMapStatus != static_cast<Memory_NaviMapStatus>(mPrivate_NaviMapStatus)) {
                mPrivate_NaviMapStatus = static_cast<Private_NaviMapStatus>(mMemoryPrivate_NaviMapStatus);
                updateInterIllumLcdDuty();
            }
        }
    }

    void saveMemoryPrivateNaviMapStatus() {
        if (mIsAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory_IGN.Inter_NaviMapStatus", static_cast<HUInt64>(mPrivate_NaviMapStatus));
        }
    }

    // 4.3 조명 최소화 모드
    void updateInterMinLightMode(bool skipLcdDutyUpdate = false) {
        Inter_MinLightMode stat = Inter_MinLightMode::OFF;
        if ((mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV) &&
            mIsIgnOn == true && mInput_UtilityModeOnOffStatus == Input_UtilityModeOnOffStatus::MODE_ON &&
            mInter_MinimizeInteriorLightsStatus == Inter_MinimizeInteriorLightsStatus::ON) {
            stat = Inter_MinLightMode::ON;
        }
        setSFCIllumination_2_0Inter_MinLightMode(stat);
        _DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_MinLightMode == " << static_cast<HUInt64>(stat)
                 << "(" << static_cast<HUInt64>(mInter_MinLightMode) << ")";
        mInter_MinLightMode = stat;
        if (skipLcdDutyUpdate == false) {
            updateInterIllumLcdDuty();
        }
        updateOutputIlluminationLevelValue();
        updateOutputIlluminationDutyValue();
        updateOutputDetentStatus();
    }

    // 4.4 실내 조명 밝기 제어 수준 판단(Inter_ClusterLightingLevel)
    HBool isHuOptionApplied() {
        return (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::ON);
    }

    // 4.4.2 HU 제어 옵션 미적용시 동작사양
    // 4.4.2.1 조명 조작 단계 제어(Inter_RheostatLevelStatus)
    // 4.4.2.1.2 스위치 동작 이벤트 정의(Private_RheostatSwitchOperation)
    void updatePrivateRheostatSwitchOperation() {
        if (isHuOptionApplied() == true) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.stop();
            return;
        }

        Private_RheostatSwitchOperation stat = Private_RheostatSwitchOperation::DEFAULT;
        if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT &&
            mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP) {
            stat = Private_RheostatSwitchOperation::UP;
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN) {
            stat = Private_RheostatSwitchOperation::DOWN;
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP) {
            if (mOneShotTimerPrivateiRheostatSwitchOperation500ms.isRunning() == false) {
                if (mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.isRunning() == true) {
                    stat = Private_RheostatSwitchOperation::BLINK_UP_DEFAULT;
                } else {
                    mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
                }
            }
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN) {
            if (mOneShotTimerPrivateiRheostatSwitchOperation500ms.isRunning() == false) {
                if (mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.isRunning() == true) {
                    stat = Private_RheostatSwitchOperation::BLINK_DOWN_DEFAULT;
                } else {
                    mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
                }
            }
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            if (mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.isRunning() == true) {
                stat = Private_RheostatSwitchOperation::BLINK_DOWN_DEFAULT;
            } else {
                mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
            }
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            if (mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.isRunning() == true) {
                stat = Private_RheostatSwitchOperation::BLINK_UP_DEFAULT;
            } else {
                mOneShotTimerPrivateiRheostatSwitchOperation500ms.restart();
            }
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::UP &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.stop();
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DOWN &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.stop();
        } else if (mPrevInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT &&
                   mInter_RheostatSwitchStatus == Inter_RheostatSwitchStatus::DEFAULT) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.stop();
        } else {
            // no operation
        }

        mPrevPrivate_RheostatSwitchOperation = mPrivate_RheostatSwitchOperation;
        mPrivate_RheostatSwitchOperation = stat;
        updateInterRheostatLevelStatus();
    }

    // 4.4.2.1.2 스위치 동작 이벤트 정의
    // UP 또는 DOWN입력값이 지속되는 경우 500ms 이후 시점 timeout
    void onOneShotTimerPrvateiRheostatSwitchOperation500ms() {
        mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.restart();
        updatePrivateRheostatSwitchOperation();
        flush();
    }

    // 4.4.2.1.2 스위치 동작 이벤트 정의
    // 100ms 마다 UP 또는 DOWN 이벤트를 발생
    void onRepeatTimerPrivateRheostatSwitchOperation100ms() {
        updateInterRheostatLevelStatus();
        flush();
    }

    // 4.4.2.1.3 현재 상태값 변경
    void updateInterRheostatLevelStatus(const HBool& isSetRheoStatLvlChanged = false) {
        if (isHuOptionApplied() == true) {
            mOneShotTimerPrivateiRheostatSwitchOperation500ms.stop();
            mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms.stop();
            return;
        }

        Inter_RheostatLevelStatus prevInter_RheostatLevelStatus = mInter_RheostatLevelStatus;
        HBool isChanged = false;

        if (isSetRheoStatLvlChanged == true) {
            if (mInter_SetRheoStatLvl != Inter_SetRheoStatLvl::NONE && mInter_SetRheoStatLvl != Inter_SetRheoStatLvl::MAX) {
                if (mInter_SetRheoStatLvl == Inter_SetRheoStatLvl::STEP21) {
                    if (mInter_ConfigAutolight == Inter_ConfigAutolight::OFF) {
                        mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP21;
                        isChanged = true;
                    }
                } else {
                    mInter_RheostatLevelStatus = static_cast<Inter_RheostatLevelStatus>(mInter_SetRheoStatLvl);
                    isChanged = true;
                }
            }
        } else {
            switch (mInter_RheostatLevelStatus) {
                case Inter_RheostatLevelStatus::STEP1:
                    if (isSwitchUp() == true) {
                        mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP2;
                        isChanged = true;
                    }
                    break;
                case Inter_RheostatLevelStatus::STEP2... Inter_RheostatLevelStatus::STEP19:
                    if (isSwitchUp() == true) {
                        mInter_RheostatLevelStatus =
                            static_cast<Inter_RheostatLevelStatus>(static_cast<HUInt64>(mInter_RheostatLevelStatus) + 1);
                        isChanged = true;
                    } else if (isSwitchDown() == true) {
                        mInter_RheostatLevelStatus =
                            static_cast<Inter_RheostatLevelStatus>(static_cast<HUInt64>(mInter_RheostatLevelStatus) - 1);
                        isChanged = true;
                    } else {
                        // no operation
                    }
                    break;
                case Inter_RheostatLevelStatus::STEP20:
                    if (mInter_ConfigAutolight == Inter_ConfigAutolight::OFF && isSwitchUp() == true) {
                        mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP21;
                        isChanged = true;
                    } else if (isSwitchDown() == true) {
                        mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP19;
                        isChanged = true;
                    } else {
                        // no operation
                    }
                    break;
                case Inter_RheostatLevelStatus::STEP21:
                    if (isSwitchDown() == true) {
                        mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP20;
                        isChanged = true;
                    }
                    break;
                default:
                    break;
            }
        }

        if (isChanged == true) {
            setSFCIllumination_2_0Inter_RheostatLevelStatus(mInter_RheostatLevelStatus);
            DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_RheostatLevelStatus == "
                    << static_cast<HUInt64>(mInter_RheostatLevelStatus) << "("
                    << static_cast<HUInt64>(prevInter_RheostatLevelStatus) << ")";
            saveMemoryRheostatLevelStatus();
            updateClusterLightingLevel();
        }
    }

    // 4.4.2.1.3 현재 상태값 변경
    // Private_RheostatSwitchOperation 상태 UP 확인
    inline HBool isSwitchUp() const {
        return ((mPrevPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::DEFAULT &&
                 mPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::UP) ||
                mPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::BLINK_UP_DEFAULT);
    }

    // 4.4.2.1.3 현재 상태값 변경
    // Private_RheostatSwitchOperation 상태 DOWN 확인
    inline HBool isSwitchDown() const {
        return ((mPrevPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::DEFAULT &&
                 mPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::DOWN) ||
                mPrivate_RheostatSwitchOperation == Private_RheostatSwitchOperation::BLINK_DOWN_DEFAULT);
    }

    // 4.4.2.1.4 조명 조작 단계 저장 및 불러오기
    // 사양서에는 Inter_UserProfileStatus 를 보도록 되어있지만 이전 Illumination에서 해당 정보로 인한 처리를 SFC 에서 하지 않았음
    // 따라서 동일하게 SFC 에서 처리하지 않는 것으로 합의되어 아래와 같이 1개의 노드만 업데이트 함
    void loadMemoryRheostatLevelStatus() {
        if (mIsAEMInitializeComplete == true) {
            if (mMemoryInter_RheostatLevelStatus != Inter_RheostatLevelStatus::NONE &&
                mMemoryInter_RheostatLevelStatus != mInter_RheostatLevelStatus) {
                mInter_RheostatLevelStatus = mMemoryInter_RheostatLevelStatus;
                setSFCIllumination_2_0Inter_RheostatLevelStatus(mInter_RheostatLevelStatus);
                updateClusterLightingLevel();
            }
        }
    }

    void saveMemoryRheostatLevelStatus() {
        if (mIsAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory.Inter_RheostatLevelStatus", static_cast<HUInt64>(mInter_RheostatLevelStatus));
        }
    }

    // 4.4 실내 조명 밝기 제어 수준 판단(Inter_ClusterLightingLevel)
    // 4.4.2 HU 제어 옵션 미적용시 동작사양
    // 4.4.2.2 종합 판단
    // 4.4.3 HU 제어 옵션 적용시 동작사양
    void updateClusterLightingLevel() {
        HBool isChanged = false;
        HUInt64 prevInter_ClusterLightingLevel = mInter_ClusterLightingLevel;
        if (isHuOptionApplied() == false) {
            // 4.4.2 HU 제어 옵션 미적용시 동작사양
            // 4.4.2.2 종합 판단
            HUInt64 step = static_cast<HUInt64>(mInter_RheostatLevelStatus);

            if (mInter_RheostatLevelStatus == Inter_RheostatLevelStatus::STEP21) {
                mInter_ClusterLightingLevel = 1000;
                isChanged = true;
            } else if (mInter_IllCluster == Inter_IllCluster::NIGHT && step >= 1 && step <= 20) {
                mInter_ClusterLightingLevel = mInter_ClusterLightingLevelNightTableArray[step - 1];
                isChanged = true;
            } else if (mInter_IllCluster == Inter_IllCluster::DUSK && step >= 1 && step <= 20) {
                mInter_ClusterLightingLevel = mInter_ClusterLightingLevelDuskTableArray[step - 1];
                isChanged = true;
            } else if (mInter_IllCluster == Inter_IllCluster::DAY && step >= 1 && step <= 20) {
                mInter_ClusterLightingLevel = mInter_ClusterLightingLevelDayTableArray[step - 1];
                isChanged = true;
            } else {
                // no operation
            }
        } else {
            // 4.4.3 HU 제어 옵션 적용시 동작사양
            if ((almostEqual(mInter_AutoBrightLevel, 1.00) == true) ||
                (mInter_AutoBrightLevel > 1.00 && mInter_AutoBrightLevel < 100.00) ||
                (almostEqual(mInter_AutoBrightLevel, 100.00) == true)) {
                mInter_ClusterLightingLevel = static_cast<HUInt64>(mInter_AutoBrightLevel * 10.0);
                isChanged = true;
            }
        }

        if (isChanged == true) {
            setSFCIllumination_2_0Inter_ClusterLightingLevel(mInter_ClusterLightingLevel);
            DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_ClusterLightingLevel == "
                    << mInter_ClusterLightingLevel << "(" << prevInter_ClusterLightingLevel << ")";
            updatePrivateDayLcdDutySetValue();
            updatePrivateNightLcdDutySetValue();
            updatePrivateLightOutLevel();
            updateOutputDetentStatus();
        }
    }

    // 4.5 클러스터에서 직접 제어하는 디스플레이의 조명 제어
    // 4.5.1 출력 정의(Inter_IllumLcdDuty)
    // 4.5.2 Duty비 값 설정
    void updatePrivateDayLcdDutySetValue() {
        HBool isChanged = false;
        HDouble paramFirst = 0.0;
        HDouble paramSecond = 0.0;
        HDouble paramThird = 0.0;
        HDouble paramFourth = 0.0;
        if (mIllumDayLcdParamDataArray.size() > kParamArrayFourthIdx) {
            paramFirst = static_cast<HDouble>(mIllumDayLcdParamDataArray[kParamArrayFristIdx]);
            paramSecond = static_cast<HDouble>(mIllumDayLcdParamDataArray[kParamArraySecondIdx]);
            paramThird = static_cast<HDouble>(mIllumDayLcdParamDataArray[kParamArrayThirdIdx]);
            paramFourth = static_cast<HDouble>(mIllumDayLcdParamDataArray[kParamArrayFourthIdx]);
        } else {
            DError() << "[Illumination_2_0] updatePrivateDayLcdDutySetValue: Idx error";
            return;
        }

        if (mInter_ClusterLightingLevel <= 10) {
            mPrivate_DayLcdDutySetValue = roundsOffTo2DecimalPlaces(paramFirst);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel > 10 && mInter_ClusterLightingLevel < 120) {
            HDouble value =
                paramFirst + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 10.0) * (paramSecond - paramFirst) / 110.0;
            mPrivate_DayLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 120 && mInter_ClusterLightingLevel < 450) {
            HDouble value =
                paramSecond + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 120.0) * (paramThird - paramSecond) / 330.0;
            mPrivate_DayLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 450 && mInter_ClusterLightingLevel < 1000) {
            HDouble value =
                paramThird + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 450.0) * (paramFourth - paramThird) / 550.0;
            mPrivate_DayLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 1000) {
            mPrivate_DayLcdDutySetValue = roundsOffTo2DecimalPlaces(paramFourth);
            isChanged = true;
        } else {
            // no operation
        }

        if (isChanged == true) {
            updateInterIllumLcdDuty();
        }
    }

    void updatePrivateNightLcdDutySetValue() {
        HBool isChanged = false;
        HDouble paramFirst = 0.0;
        HDouble paramSecond = 0.0;
        HDouble paramThird = 0.0;
        HDouble paramFourth = 0.0;
        if (mIllumNightLcdParamDataArray.size() > kParamArrayFourthIdx) {
            paramFirst = static_cast<HDouble>(mIllumNightLcdParamDataArray[kParamArrayFristIdx]);
            paramSecond = static_cast<HDouble>(mIllumNightLcdParamDataArray[kParamArraySecondIdx]);
            paramThird = static_cast<HDouble>(mIllumNightLcdParamDataArray[kParamArrayThirdIdx]);
            paramFourth = static_cast<HDouble>(mIllumNightLcdParamDataArray[kParamArrayFourthIdx]);
        } else {
            DError() << "[Illumination_2_0] updatePrivateNightLcdDutySetValue: Idx error";
            return;
        }

        if (mInter_ClusterLightingLevel <= 10) {
            mPrivate_NightLcdDutySetValue = roundsOffTo2DecimalPlaces(paramFirst);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel > 10 && mInter_ClusterLightingLevel < 120) {
            HDouble value =
                paramFirst + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 10.0) * (paramSecond - paramFirst) / 110.0;
            mPrivate_NightLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 120 && mInter_ClusterLightingLevel < 450) {
            HDouble value =
                paramSecond + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 120.0) * (paramThird - paramSecond) / 330.0;
            mPrivate_NightLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 450 && mInter_ClusterLightingLevel < 1000) {
            HDouble value =
                paramThird + (static_cast<HDouble>(mInter_ClusterLightingLevel) - 450.0) * (paramFourth - paramThird) / 550.0;
            mPrivate_NightLcdDutySetValue = roundsOffTo2DecimalPlaces(value);
            isChanged = true;
        } else if (mInter_ClusterLightingLevel >= 1000) {
            mPrivate_NightLcdDutySetValue = roundsOffTo2DecimalPlaces(paramFourth);
            isChanged = true;
        } else {
            // no operation
        }

        if (isChanged == true) {
            updateInterIllumLcdDuty();
        }
    }

    inline HDouble roundsOffTo2DecimalPlaces(HDouble value) const {
        return round(value * 100.0) / 100.0;
    }

    // 4.5.3 조명 제어 로직
    // 4.5.3.1 조명 동작 상태
    void updatePrivateLcdLightingStatus() {
        Private_LcdLightingStatus stat = Private_LcdLightingStatus::OFF;
        if (mIsIgnOn == true || (mIsIgnOn == false && mInter_LcdLightingStatus == Inter_LcdLightingStatus::HMI_ON)) {
            stat = Private_LcdLightingStatus::ON;
        }
        mPrivate_LcdLightingStatus = stat;
        updateInterIllumLcdDuty();
    }

    // 4.5.3.2 조명 동작 상태별 조명 제어
    void updateInterIllumLcdDuty() {
        HDouble dutyValue = 0.0;
        if (mPrivate_LcdLightingStatus == Private_LcdLightingStatus::ON &&
            (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::OFF ||
             (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::ON &&
              mPrivate_NaviMapStatus == Private_NaviMapStatus::DAY)) &&
            mInter_MinLightMode == Inter_MinLightMode::OFF) {
            dutyValue = mPrivate_DayLcdDutySetValue;
        } else if (mPrivate_LcdLightingStatus == Private_LcdLightingStatus::ON &&
                   mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::ON &&
                   mPrivate_NaviMapStatus == Private_NaviMapStatus::NIGHT && mInter_MinLightMode == Inter_MinLightMode::OFF) {
            dutyValue = mPrivate_NightLcdDutySetValue;
        } else {
            // no operation
        }

        setSFCIllumination_2_0Inter_IllumLcdDuty(dutyValue);
        DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_IllumLcdDuty == " << dutyValue;
    }

    // 4.6 외부 전장품 조명 제어
    // 4.6.1 출력 단계 결정
    void updatePrivateLightOutLevel() {
        if (mInter_ClusterLightingLevel < 10) {
            mPrivate_LightOutLevel = 1;
        } else {
            mPrivate_LightOutLevel = floor((static_cast<HDouble>(mInter_ClusterLightingLevel) / 5.0));
        }
        updateOutputIlluminationLevelValue();
        updateOutputIlluminationDutyValue();
        updateOutputNotMinIlluminationLevelValue();
    }

    // 4.6.2 조명 제어 출력
    // Output_IlluminationLevelValue
    void updateOutputIlluminationLevelValue() {
        AddressValueList outputAddressList;
        HUInt64 value = 0;
        HBool isChanged = false;
        if (mInter_MinLightMode == Inter_MinLightMode::OFF) {
            value = mPrivate_LightOutLevel;
            isChanged = true;
        } else if (mInter_MinLightMode == Inter_MinLightMode::ON) {
            value = 2;
            isChanged = true;
        } else {
            // no operation
        }

        if (isChanged == true) {
            outputAddressList.emplace_back(Vehicle.CD.Illumination_2_0.Output_IlluminationLevelValue, value);
            setValue(outputAddressList);
            outputAddressList.clear();
        }
    }

    // Output_IlluminationDutyValue
    void updateOutputIlluminationDutyValue() {
        AddressValueList outputAddressList;
        HDouble value = 0.0;
        HBool isChanged = false;
        if (mInter_MinLightMode == Inter_MinLightMode::ON || mPrivate_LightOutLevel <= 2) {
            value = 3.00;
            isChanged = true;
        } else if (mInter_MinLightMode == Inter_MinLightMode::OFF && mPrivate_LightOutLevel > 2 && mPrivate_LightOutLevel < 24) {
            value = roundsOffTo2DecimalPlaces(3.00 + (static_cast<HDouble>(mPrivate_LightOutLevel) - 2.0) * 47.0 / 22.0);
            isChanged = true;
        } else if (mInter_MinLightMode == Inter_MinLightMode::OFF && mPrivate_LightOutLevel == 24) {
            value = 50.00;
            isChanged = true;
        } else if (mInter_MinLightMode == Inter_MinLightMode::OFF && mPrivate_LightOutLevel > 24 && mPrivate_LightOutLevel < 90) {
            value = roundsOffTo2DecimalPlaces(50.00 + (static_cast<HDouble>(mPrivate_LightOutLevel) - 24.0) * 50.0 / 66.0);
            isChanged = true;
        } else if (mInter_MinLightMode == Inter_MinLightMode::OFF && mPrivate_LightOutLevel >= 90) {
            value = 100.00;
            isChanged = true;
        } else {
            // no operation
        }

        if (isChanged == true) {
            HUInt64 value_x100 = static_cast<HUInt64>(value * 100.0);
            outputAddressList.emplace_back(Vehicle.System.HardWire.Output_IlluminationDutyValue, value_x100);
            setValue(outputAddressList);
            outputAddressList.clear();
        }
    }

    // Output_DetentStatus
    void updateOutputDetentStatus() {
        AddressValueList outputAddressList;
        if ((mInter_MinLightMode == Inter_MinLightMode::OFF && mInter_IllCluster == Inter_IllCluster::DAY) ||
            (mInter_MinLightMode == Inter_MinLightMode::OFF && mInter_ClusterLightingLevel >= 455)) {
            decltype(Vehicle.CD.Illumination_2_0.Output_DetentStatus)::TYPE stat =
                decltype(Vehicle.CD.Illumination_2_0.Output_DetentStatus)::TYPE::ON;
            outputAddressList.emplace_back(Vehicle.CD.Illumination_2_0.Output_DetentStatus, static_cast<HUInt64>(stat));
            setValue(outputAddressList);
        } else if (mInter_MinLightMode == Inter_MinLightMode::ON ||
                   (mInter_IllCluster != Inter_IllCluster::DAY && mInter_ClusterLightingLevel < 455)) {
            decltype(Vehicle.CD.Illumination_2_0.Output_DetentStatus)::TYPE stat =
                decltype(Vehicle.CD.Illumination_2_0.Output_DetentStatus)::TYPE::OFF;
            outputAddressList.emplace_back(Vehicle.CD.Illumination_2_0.Output_DetentStatus, static_cast<HUInt64>(stat));
            setValue(outputAddressList);
        } else {
            // no operation
        }
        outputAddressList.clear();
    }

    // Output_NotMinIlluminationLevelValue
    void updateOutputNotMinIlluminationLevelValue() {
        AddressValueList outputAddressList;
        outputAddressList.emplace_back(Vehicle.CD.Illumination_2_0.Output_NotMinIlluminationLevelValue, mPrivate_LightOutLevel);
        setValue(outputAddressList);
        outputAddressList.clear();
    }

    // 4.7 조명 조절 USM 생성 제어
    void updateInterRheostatMaxInfo() {
        SFCIllumination_2_0Inter_RheostatMaxInfo stat = SFCIllumination_2_0Inter_RheostatMaxInfo::NONE;
        if (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::OFF && mInter_ConfigAutolight == Inter_ConfigAutolight::ON) {
            stat = SFCIllumination_2_0Inter_RheostatMaxInfo::STEP20;
        } else if (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::OFF &&
                   mInter_ConfigAutolight == Inter_ConfigAutolight::OFF) {
            stat = SFCIllumination_2_0Inter_RheostatMaxInfo::STEP21;
        } else if (mInter_ConfigAvnBrightLink == Inter_ConfigAvnBrightLink::ON) {
            stat = SFCIllumination_2_0Inter_RheostatMaxInfo::INVALID;
        } else {
            // no operation
        }

        if (stat != SFCIllumination_2_0Inter_RheostatMaxInfo::NONE && stat != SFCIllumination_2_0Inter_RheostatMaxInfo::MAX) {
            setSFCIllumination_2_0Inter_RheostatMaxInfo(stat);
            DInfo() << "[llumination_2_0] setSFCNode: SFC.Illumination_2_0.Inter_RheostatMaxInfo == "
                    << static_cast<HUInt64>(stat);
        }
    }

    // 5 표시 로직 - N/A
    // 5.1 Telltale - N/A
    // 5.2 표시 구성요소 정보 처리 로직 - N/A
    // 5.3 사운드 로직 - N/A

    // Get parameter array
    // If the system is big-endian, this code might not work as expected (byte order could be different).
    template <size_t N>
    void copyParamStringToUInt16Array(const std::string& paramDataString, std::array<uint16_t, N>& paramDataUInt16Array) {
        if (N * sizeof(uint16_t) == paramDataString.size()) {
            paramDataUInt16Array.fill(0);
            std::memcpy(paramDataUInt16Array.data(), paramDataString.data(), paramDataString.size());
        } else {
            DError() << "[Illumination_2_0] Size error";
            DError() << "[Illumination_2_0] paramDataString : " << paramDataString.size();
            DError() << "[Illumination_2_0] Byte Size of paramDataUInt16Array: " << N * sizeof(uint16_t);
            DError() << "[Illumination_2_0] copyParamStringToUInt16TargetArray didn't any copying process.";
        }
        return;
    }

    // Constant Variables
    static constexpr HUInt64 kParamArraySize4 = 4;
    static constexpr HUInt64 kTimerInterval100ms = 100;
    static constexpr HUInt64 kTimerInterval500ms = 500;
    static constexpr HUInt64 kClusterLightingLevelCount = 20;
    static constexpr std::array<HUInt64, kClusterLightingLevelCount> mInter_ClusterLightingLevelNightTableArray = {
        10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 100, 110, 120};
    static constexpr std::array<HUInt64, kClusterLightingLevelCount> mInter_ClusterLightingLevelDuskTableArray = {
        120, 125, 130, 135, 145, 155, 165, 175, 190, 205, 220, 235, 255, 275, 295, 320, 345, 375, 410, 450};
    static constexpr std::array<HUInt64, kClusterLightingLevelCount> mInter_ClusterLightingLevelDayTableArray = {
        455, 465, 480, 495, 510, 530, 550, 570, 590, 615, 640, 665, 695, 725, 760, 795, 835, 880, 935, 1000};
    static constexpr HUInt64 kParamArrayFristIdx = 0;
    static constexpr HUInt64 kParamArraySecondIdx = 1;
    static constexpr HUInt64 kParamArrayThirdIdx = 2;
    static constexpr HUInt64 kParamArrayFourthIdx = 3;

    // VSM Inter Variables
    using Inter_ConfigAutolight = ArgumentsConfigAutolightChanged::Inter_ConfigAutolight;
    using Inter_ConfigVehicleType = ArgumentsMinLightModeChanged::Inter_ConfigVehicleType;
    using Inter_MinimizeInteriorLightsStatus = ArgumentsMinLightModeChanged::Inter_MinimizeInteriorLightsStatus;
    using Inter_ConfigAvnBrightLink = ArgumentsConfigAvnBrightLinkChanged::Inter_ConfigAvnBrightLink;

    Inter_ConfigAutolight mInter_ConfigAutolight = Inter_ConfigAutolight::OFF;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    Inter_MinimizeInteriorLightsStatus mInter_MinimizeInteriorLightsStatus = Inter_MinimizeInteriorLightsStatus::OFF;
    Inter_ConfigAvnBrightLink mInter_ConfigAvnBrightLink = Inter_ConfigAvnBrightLink::OFF;

    // VSM Input Variables
    using Input_ExtLowBeamOnReqStatus = ArgumentsDayAndNightModeChanged::Input_ExtLowBeamOnReqStatus;
    using Input_AvTailLampStatus = ArgumentsDayAndNightModeChanged::Input_AvTailLampStatus;
    using Input_AutolightSensorNightStatus = ArgumentsDayAndNightModeChanged::Input_AutolightSensorNightStatus;
    using Input_NaviMapDayNightStatus = ArgumentsNaviMapDayNightStatusChanged::Input_NaviMapDayNightStatus;
    using Input_UtilityModeOnOffStatus = ArgumentsMinLightModeChanged::Input_UtilityModeOnOffStatus;
    using Inter_SetRheoStatLvl = ArgumentsSetRheoStatLvlChanged::Inter_SetRheoStatLvl;

    Input_ExtLowBeamOnReqStatus mInput_ExtLowBeamOnReqStatus = Input_ExtLowBeamOnReqStatus::OFF;
    Input_AvTailLampStatus mInput_AvTailLampStatus = Input_AvTailLampStatus::OFF;
    Input_AutolightSensorNightStatus mInput_AutolightSensorNightStatus = Input_AutolightSensorNightStatus::OFF;
    Input_NaviMapDayNightStatus mInput_NaviMapDayNightStatus = Input_NaviMapDayNightStatus::INVALID;
    Input_UtilityModeOnOffStatus mInput_UtilityModeOnOffStatus = Input_UtilityModeOnOffStatus::NON_OPTION;
    Inter_SetRheoStatLvl mInter_SetRheoStatLvl = Inter_SetRheoStatLvl::NONE;

    // SFC Inter Variables
    using Inter_DayNightState = ArgumentsDayNightStateChanged::Inter_DayNightState;
    using Inter_RheostatSwitchStatus = ArgumentsRheostatSwitchStatusChanged::Inter_RheostatSwitchStatus;
    using Inter_LcdLightingStatus = ArgumentsLcdLightingStatusChanged::Inter_LcdLightingStatus;

    HBool mIsAEMInitializeComplete = false;
    HBool mIsIgnOn = false;
    Inter_DayNightState mInter_DayNightState = Inter_DayNightState::NIGHT;
    Inter_RheostatSwitchStatus mInter_RheostatSwitchStatus = Inter_RheostatSwitchStatus::DEFAULT;
    Inter_RheostatSwitchStatus mPrevInter_RheostatSwitchStatus = Inter_RheostatSwitchStatus::NONE;
    HDouble mInter_AutoBrightLevel = 0.0;
    Inter_LcdLightingStatus mInter_LcdLightingStatus = Inter_LcdLightingStatus::HMI_OFF;

    // SFC Param Variables
    std::array<uint16_t, kParamArraySize4> mIllumDayLcdParamDataArray{};
    std::array<uint16_t, kParamArraySize4> mIllumNightLcdParamDataArray{};

    // Illumination_2_0 Output Variables
    using Inter_IllCluster = SFCIllumination_2_0Inter_IllCluster;
    using Inter_MinLightMode = SFCIllumination_2_0Inter_MinLightMode;
    using Inter_RheostatLevelStatus = SFCIllumination_2_0Inter_RheostatLevelStatus;

    Inter_IllCluster mInter_IllCluster = Inter_IllCluster::NIGHT;
    Inter_MinLightMode mInter_MinLightMode = Inter_MinLightMode::OFF;
    Inter_RheostatLevelStatus mInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP12;
    HUInt64 mInter_ClusterLightingLevel = 455;

    // Private variables
    enum class Private_NaviMapStatus : HUInt64 {
        NONE,
        DAY,
        NIGHT,
        MAX
    };

    enum class Private_RheostatSwitchOperation : HUInt64 {
        NONE,
        UP,
        BLINK_UP_DEFAULT,
        DOWN,
        BLINK_DOWN_DEFAULT,
        DEFAULT,
        MAX
    };

    enum class Private_LcdLightingStatus : HUInt64 {
        NONE,
        OFF,
        ON,
        MAX
    };

    Private_NaviMapStatus mPrivate_NaviMapStatus = Private_NaviMapStatus::NIGHT;
    Private_RheostatSwitchOperation mPrivate_RheostatSwitchOperation = Private_RheostatSwitchOperation::DEFAULT;
    Private_RheostatSwitchOperation mPrevPrivate_RheostatSwitchOperation = Private_RheostatSwitchOperation::NONE;
    HDouble mPrivate_DayLcdDutySetValue = 0.0;
    HDouble mPrivate_NightLcdDutySetValue = 0.0;
    Private_LcdLightingStatus mPrivate_LcdLightingStatus = Private_LcdLightingStatus::OFF;
    HUInt64 mPrivate_LightOutLevel = 0.0;

    // SFC Memory Variables
    using Memory_NaviMapStatus = ArgumentsMemoryNaviMapStatusChanged::Inter_NaviMapStatus;

    Memory_NaviMapStatus mMemoryPrivate_NaviMapStatus = Memory_NaviMapStatus::NIGHT;
    Inter_RheostatLevelStatus mMemoryInter_RheostatLevelStatus = Inter_RheostatLevelStatus::STEP12;

    // SFC Timer
    ssfs::SFCTimer<Illumination_2_0> mOneShotTimerPrivateiRheostatSwitchOperation500ms;
    ssfs::SFCTimer<Illumination_2_0> mRepeatTimerPrivateiRheostatSwitchOperationEvery100ms;
};

}  // namespace ccos

#endif  // SFSS_Illumination_2_0_H
