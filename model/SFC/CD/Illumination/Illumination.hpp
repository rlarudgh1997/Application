/**
 * @file Illumination.hpp
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
#ifndef SFSS_Illumination_H
#define SFSS_Illumination_H

#define DLOG_ENABLED gEnableSFCLog

#include "IlluminationBase.hpp"
#include <array>
#include <cstring>
#include <SFCTimer.h>
#include <Vehicle.h>

namespace ccos {

// SFC Version : 27.0.1
// Reference : [CD050] Illumination. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Illumination : public IlluminationBase {
public:
    Illumination() = default;
    ~Illumination() override = default;
    Illumination(const Illumination& other) = delete;
    Illumination(Illumination&& other) noexcept = delete;
    Illumination& operator=(const Illumination& other) = delete;
    Illumination& operator=(Illumination&& other) noexcept = delete;

    void onInitialize() override {
        if (mRepeatTimerTransitionLogStyleDialTimerEvery100ms.create(
                kTimerTransitionInterval100ms, this, &Illumination::onRepeatTimerTransitionLogStyleDialTimerEvery100msChanged,
                true) == false) {
            DWarning() << "[Illumination] mRepeatTimerTransitionLogStyleDialTimerEvery100ms Create Failed";
        }

        if (mRepeatTimerTransitionExpStyleDialTimerEvery100ms.create(
                kTimerTransitionInterval100ms, this, &Illumination::onRepeatTimerTransitionExpStyleDialTimerEvery100msChanged,
                true) == false) {
            DWarning() << "[Illumination] mRepeatTimerTransitionExpStyleDialTimerEvery100ms Create Failed";
        }

        if (mRepeatTimerTransitionLogStylePointerTimerEvery100ms.create(
                kTimerTransitionInterval100ms, this, &Illumination::onRepeatTimerTransitionLogStylePointerTimerEvery100msChanged,
                true) == false) {
            DWarning() << "[Illumination] mRepeatTimerTransitionLogStylePointerTimerEvery100ms Create Failed";
        }

        if (mRepeatTimerTransitionExpStylePointerTimerEvery100ms.create(
                kTimerTransitionInterval100ms, this, &Illumination::onRepeatTimerTransitionExpStylePointerTimerEvery100msChanged,
                true) == false) {
            DWarning() << "[Illumination] mRepeatTimerTransitionExpStylePointerTimerEvery100ms Create Failed";
        }

        if (mOneShotTimerIgnElapsedOn400ms.create(kTimerIgnElapsedOnInterval400ms, this,
                                                  &Illumination::onOneShotTimerIgnElapsedOn400msChanged, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerIgnElapsedOn400ms Create Failed";
        }

        if (mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms.create(
                kTimerPrivateDialLightingTurnOnInterval2s, this,
                &Illumination::onOneShotTimerPrivate_DialLightingStatusTurningOn2000msChanged, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms Create Failed";
        }

        if (mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms.create(
                kTimerPrivateDialLightingTurnOffInterval2s, this,
                &Illumination::onOneShotTimerPrivate_DialLightingStatusTurningOff2000msChanged, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms Create Failed";
        }

        if (mOneShotTimerPrivate_RheostatSwitchOperation500ms.create(
                kTimerPrivateRheostatSwitchOperationInterval500ms, this,
                &Illumination::onOneShotTimerPrivate_RheostatSwitchOperation500msChanged, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_RheostatSwitchOperation500ms Create Failed";
        }

        if (mOneShotTimerBlinkPrivate_RheostatSwitchOperation.create(
                kTimerPrivateRheostatSwitchOperationInterval500ms, this,
                &Illumination::onOneShotTimerPrivate_RheostatSwitchOperationBlink, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerBlinkPrivate_RheostatSwitchOperation Create Failed";
        }

        if (mOneShotTimerInter_EventInfoDisplayStatusOff5000ms.create(
                kTimerEventInfoDisplayStatusOffInterval5s, this,
                &Illumination::onOneShotTimerInter_EventInfoDisplayStatusOff5000msTimeout, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerInter_EventInfoDisplayStatusOff5000ms Create Failed";
        }

        if (mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms.create(
                kTimerPrivateIgOffDisplayOnStatusOffInterval5s, this,
                &Illumination::onOneShotTimerPrivate_IgOffDisplayOnStatusOff5000msTimeout, false) == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms Create Failed";
        }

        setSFCIlluminationEventRheostatFeedbackNoAVNID("E00501");
        setSFCIlluminationEventRheostatFeedbackNoAVNStat(SFCIlluminationEventRheostatFeedbackNoAVNStat::OFF);

        setSFCIlluminationEventRheostatFeedbackAvnLinkID("E00502");
        setSFCIlluminationEventRheostatFeedbackAvnLinkStat(SFCIlluminationEventRheostatFeedbackAvnLinkStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        DDebug() << "Illu_SFC : IGN ON 0ms";
        mIgnElapsed = IgnElapsed::ON_0ms;

        if (mOneShotTimerIgnElapsedOn400ms.restart() == false) {
            DWarning() << "[Illumination] mOneShotTimerIgnElapsedOn400ms Restart Failed";
        }
        updateInterMinLightMode();
        updatePrivate_LcdLightingStatus();   // updateLcdDuty 포함
        updatePrivate_DialLightingStatus();  // updatePointerAndDialDuty 포함
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        DDebug() << "Illu_SFC : IGN OFF 0ms";
        mIgnElapsed = IgnElapsed::OFF_0ms;

        mOneShotTimerIgnElapsedOn400ms.stop();

        updateInterMinLightMode();
        updatePrivate_LcdLightingStatus();   //  updateLcdDuty 포함
        updatePrivate_DialLightingStatus();  // updatePointerAndDialDuty 포함
    }

    void onConfigAutolightChanged(const ArgumentsConfigAutolightChanged& args) {
        // Autolight OFF = MAX 21
        // Autolight D' = MAX 20

        mConfigAutolight = args.mInter_ConfigAutolight;
        DDebug() << "Illu_SFC : onConfigAutolightChanged - mConfigAutolight: " << static_cast<uint64_t>(mConfigAutolight);
        updateInterIllCluster();
        updateOutRheostatMaxInfo();
    }

    void onRheostatSwitchChanged(const ArgumentsRheostatSwitchChanged& args) {
        mRheostatSwitchPrevious = mRheostatSwitch;
        mRheostatSwitch = args.mInter_RheostatSwitchStatus;
        DDebug() << "Illu_SFC : onRheostatSwitchChanged - mRheostatSwitchPrevious: "
                 << static_cast<uint64_t>(mRheostatSwitchPrevious);
        DDebug() << "Illu_SFC : onRheostatSwitchChanged - mRheostatSwitch: " << static_cast<uint64_t>(mRheostatSwitch);
        updatePrivate_RheostatSwitchOperation();

        DDebug() << "Illu_SFC : onRheostatSwitchChanged - mEPrivate_RheostatSwitchOperationPrevious: "
                 << static_cast<uint64_t>(mEPrivate_RheostatSwitchOperationPrevious);
        DDebug() << "Illu_SFC : onRheostatSwitchChanged - mEPrivate_RheostatSwitchOperation: "
                 << static_cast<uint64_t>(mEPrivate_RheostatSwitchOperation);
        if (mEPrivate_RheostatSwitchOperationPrevious == EPrivate_RheostatSwitchOperation::DEFAULT &&
            mEPrivate_RheostatSwitchOperationPrevious != mEPrivate_RheostatSwitchOperation) {
            // NOTE: UP/DOWN 스위치를 누른 상태에서 IMG_ClusterLightingLevel_Stat 값 변경시에만, E00502 이벤트 갱신.
            // NOTE: related to: SFCDEV-69, EXNCP-45762
            mIsE00502PopupEventOn = true;
            updateInterRheostatLevelStatus(EPrivate_InterRheostatLevelUpdateReason::INTER_RHEO_STAT_SWITCH_STATUS_CHANGED);
            updateRheostatFeedbackEventAvnLink();
            doFlush();
        } else {
            mIsE00502PopupEventOn = false;
            DDebug() << "Illu_SFC : onRheostatSwitchChanged - switch condition not matched";
        }
    }

    void onRheostatLevelSetReqStatChanged(const ArgumentsRheostatLevelSetReqStatChanged& args) {
        mRheostLevelSetReqStatPrevious = mRheostLevelSetReqStat;
        mRheostLevelSetReqStat = args.mInput_RheostatLevelSetReqStatus;

        DDebug() << "Illu_SFC : onRheostatLevelSetReqStatChanged - mRheostLevelSetReqStatPrevious: "
                 << static_cast<uint64_t>(mRheostLevelSetReqStatPrevious);
        DDebug() << "Illu_SFC : onRheostatLevelSetReqStatChanged - mRheostLevelSetReqStat: "
                 << static_cast<uint64_t>(mRheostLevelSetReqStat);
        if (mRheostLevelSetReqStatPrevious == RheostatLevelSetReqStat::DEFAULT &&
            mRheostLevelSetReqStatPrevious != mRheostLevelSetReqStat) {
            updateInterRheostatLevelStatus(EPrivate_InterRheostatLevelUpdateReason::INPUT_RHEO_STAT_LEVEL_SET_REQ_STATUS_CHANGED);
            doFlush();
        } else {
            DDebug() << "Illu_SFC : onRheostatLevelSetReqStatChanged - RheostatLevelSetReqStat condition not matched";
        }
    }

    void onDayAndNightModeChanged(const ArgumentsDayAndNightModeChanged& args) {
        mExtLowBeamOnReqStatus = args.mInput_ExtLowBeamOnReqStatus;
        mAvTailLampStatus = args.mInput_AvTailLampStatus;
        mAutolightSensorNightStatus = args.mInput_AutolightSensorNightStatus;

        DDebug() << "Illu_SFC : onDayAndNightModeChanged - mExtLowBeamOnReqStatus: "
                 << static_cast<uint64_t>(mExtLowBeamOnReqStatus);
        DDebug() << "Illu_SFC : onDayAndNightModeChanged - mAvTailLampStatus: " << static_cast<uint64_t>(mAvTailLampStatus);
        DDebug() << "Illu_SFC : onDayAndNightModeChanged - mAutolightSensorNightStatus: "
                 << static_cast<uint64_t>(mAutolightSensorNightStatus);
        updateInterIllCluster();
        doFlush();
    }

    void onEventInfoDisplayStatusChanged(const ArgumentsEventInfoDisplayStatusChanged& args) {
        DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged - previous mEventInfoDisplayStatus: "
                 << static_cast<uint64_t>(mEventInfoDisplayStatus);
        mEventInfoDisplayStatus = args.mInter_EventInfoDisplayStatus;

        DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged - input mEventInfoDisplayStatus: "
                 << static_cast<uint64_t>(mEventInfoDisplayStatus);
        DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged - mInter_EventInfoDisplayStatusOff5000ms: "
                 << mInter_EventInfoDisplayStatusOff5000ms;
        if (mEventInfoDisplayStatus == EventInfoDisplayStatus::OFF) {
            if (mOneShotTimerInter_EventInfoDisplayStatusOff5000ms.start() == false) {
                DWarning() << "[Illumination] mOneShotTimerInter_EventInfoDisplayStatusOff5000ms Start Failed";
            }
        } else {
            DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged - mEventInfoDisplayStatus: NOT OFF";
            mOneShotTimerInter_EventInfoDisplayStatusOff5000ms.stop();
            mInter_EventInfoDisplayStatusOff5000ms = false;
        }
        DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged last stage - mInter_EventInfoDisplayStatusOff5000ms : "
                 << mInter_EventInfoDisplayStatusOff5000ms;
        updatePrivate_LcdLightingStatus();  // updateLcdDuty() 포함
        doFlush();
    }

    // EXNCP-22760 : IGN OFF조건 조명 켜짐 판단, Private_IgOffDisplayOnStatus는 SFC 외부에서 입력받아 처리함.
    void onIgOffDisplayOnStatusChanged(const ArgumentsIgOffDisplayOnStatusChanged& args) {
        DDebug() << "Illu_SFC : onIgOffDisplayOnStatusChanged - previous mIgOffDisplayOnStatus: "
                 << static_cast<uint64_t>(mIgOffDisplayOnStatus);
        mIgOffDisplayOnStatus = args.mInter_IgOffDisplayOnStatus;

        DDebug() << "Illu_SFC : onIgOffDisplayOnStatusChanged - input mIgOffDisplayOnStatus: "
                 << static_cast<uint64_t>(mIgOffDisplayOnStatus);

        if (mIgOffDisplayOnStatus == IgOffDisplayOnStatus::OFF) {
            if (mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms.start() == false) {
                DWarning() << "[Illumination] mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms Start Failed";
            }
        } else {
            mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms.stop();
            mPrivate_IgOffDisplayOnStatusOff5000ms = false;
        }
        DDebug() << "Illu_SFC : onEventInfoDisplayStatusChanged last stage - mPrivate_IgOffDisplayOnStatusOff5000ms : "
                 << mPrivate_IgOffDisplayOnStatusOff5000ms;
        updatePrivate_LcdLightingStatus();
        doFlush();
    }

    void onRheostatLevelStatusChanged(const ArgumentsRheostatLevelStatusChanged& args) {
        DDebug() << "Illu_SFC : onRheostatLevelStatusChanged entered";
        DDebug() << "Illu_SFC : onRheostatLevelStatusChanged - mAEMInitializeComplete : " << mAEMInitializeComplete;
        DDebug() << "Illu_SFC : onRheostatLevelStatusChanged - memValue(not sure valid) : " << args.mInter_RheostatLevelStatus;

        if (updateValueIfValid(mMemory_RheostatLevelStatus, args.mInter_RheostatLevelStatus) == true) {
            if (mAEMInitializeComplete == true) {
                updateIllumLoadCalLogic();
                setSFCIlluminationConstantRheostatLevelStat(mInter_RheostatLevelStatus);
                DDebug() << "Illu_SFC : onRheostatLevelStatusChanged - set mInter_RheostatLevelStatus : "
                         << mInter_RheostatLevelStatus;
                // updatePointerAndDialDuty updateGreenWhiteDuty updateLcdDuty updateOutIllumDutyValue 포함
                updateInterClusterLightingLevel();
                updateOutRheostatLevelStat();
                updateRheostatFeedbackEventNoAVN();
                doFlush();
            } else {
                DDebug() << "Illu_SFC : onRheostatLevelStatusChanged - mAEMInitializeComplete not initialized";
            }
        } else {
            DDebug() << "Illu_SFC : onRheostatLevelStatusChanged - updateValue is invalid";
        }
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        DDebug() << "Illu_SFC : onAEMInitializeCompleteChanged entered";
        DDebug() << "args.aeminit : " << args.mInter_AEMInitializeComplete;

        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            if (mAEMInitializeComplete == true) {
                updateIllumLoadCalLogic();
                setSFCIlluminationConstantRheostatLevelStat(mInter_RheostatLevelStatus);

                DDebug() << "Illu_SFC : onAEMInitializeCompleteChanged - set mInter_RheostatLevelStatus : "
                         << mInter_RheostatLevelStatus;

                if (HUInt64 private_NaviMapStatus = 0;
                    getCachedValue("SFC.Extension.Memory_IGN.Inter_NaviMapStatus", private_NaviMapStatus) == HResult::OK) {
                    mEPrivate_NaviMapStatus = static_cast<EPrivate_NaviMapStatus>(private_NaviMapStatus);
                    mEPrivate_NaviMapStatusPrevious = mEPrivate_NaviMapStatus;
                }

                // updatePointerAndDialDuty updateGreenWhiteDuty updateLcdDuty updateOutIllumDutyValue 포함
                updateInterClusterLightingLevel();
                updateOutRheostatLevelStat();
                updateRheostatFeedbackEventNoAVN();
                doFlush();
                DDebug() << "Private_NaviMapStatus Restored to : " << static_cast<HUInt64>(mEPrivate_NaviMapStatus);
            } else {
                DDebug() << "Illu_SFC : mAEMInitializeComplete not initialized";
            }
        } else {
            DDebug() << "AEM updateValue is invalid";
        }
    }

    void onNaviMapDayNightStatusChanged(const ArgumentsNaviMapDayNightStatusChanged& args) {
        mNaviMapDayNightStatus = args.mInput_NaviMapDayNightStatus;

        DDebug() << "Illu_SFC : onNaviMapDayNightStatusChanged - input mNaviMapDayNightStatus: "
                 << static_cast<uint64_t>(mNaviMapDayNightStatus);
        updatePrivate_NaviMapStatus();
    }

    void onAvnAutoBrightValueChanged(const ArgumentsAvnAutoBrightValueChanged& args) {
        mAvnAutoBrightValue = args.mInput_AvnAutoBrightValue;

        DDebug() << "Illu_SFC : onAvnAutoBrightValueChanged - input mAvnAutoBrightValue: "
                 << static_cast<uint64_t>(mAvnAutoBrightValue);
        updateInterClusterLightingLevel();
        doFlush();
    }

    void onConfigAvnBrightLinkChanged(const ArgumentsConfigAvnBrightLinkChanged& args) {
        mConfigAvnBrightLink = args.mInter_ConfigAvnBrightLink;
        DDebug() << "Illu_SFC : onConfigAvnBrightLinkChanged - input mConfigAvnBrightLink: "
                 << static_cast<uint64_t>(mConfigAvnBrightLink);
        updateOutRheostatMaxInfo();

        // updateRheostatFeedbackEventAvnLink updatePointerAndDialDuty updateGreenWhiteDuty updateLcdDuty 포함
        updateInterClusterLightingLevel();
        updateRheostatFeedbackEventNoAVN();
        updateConstantDayNightStat();
        doFlush();
    }

    void onMinLightModeChanged(const ArgumentsMinLightModeChanged& args) {
        mCampModeOnOffStatus = args.mInput_CampModeOnOffStatus;
        mUtilityModeOnOffStatus = args.mInput_UtilityModeOnOffStatus;
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateInterMinLightMode();
    }

    void onParamIllumDialPointerChanged(const ArgumentsParamIllumDialPointerChanged& args) {
        std::string tmp;
        if (updateValueIfValid(tmp, args.mPar_IllumDayDial) == true) {
            DDebug() << "onParamIllumDialPointerChanged called";
            DDebug() << "mPar_IllumDayDial set";
            copyParamStringToUInt16TargetArray(tmp, mIllumDayDialParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumNightDial) == true) {
            DDebug() << "mPar_IllumNightDial set";
            copyParamStringToUInt16TargetArray(tmp, mIllumNightDialParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumDayPointer) == true) {
            DDebug() << "mPar_IllumDayPointer set";
            copyParamStringToUInt16TargetArray(tmp, mIllumDayPointerParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumNightPointer) == true) {
            DDebug() << "mPar_IllumNightPointer set";
            copyParamStringToUInt16TargetArray(tmp, mIllumNightPointerParamDataArray);
        }
        updatePointerAndDialDuty();
    }

    void onParamIllumLCDChanged(const ArgumentsParamIllumLCDChanged& args) {
        std::string tmp;
        if (updateValueIfValid(tmp, args.mPar_IllumDayLCD) == true) {
            DDebug() << "onParamIllumLCDChanged called";
            DDebug() << "mPar_IllumDayLCD set";
            copyParamStringToUInt16TargetArray(tmp, mIllumDayLcdParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumNightLCD) == true) {
            DDebug() << "mPar_IllumNightLCD set";
            copyParamStringToUInt16TargetArray(tmp, mIllumNightLcdParamDataArray);
        }
        updateLcdDuty();
    }

    void onParamIllumSymbolGreenWhiteChanged(const ArgumentsParamIllumSymbolGreenWhiteChanged& args) {
        std::string tmp;
        if (updateValueIfValid(tmp, args.mPar_IllumDaySymbolGreen) == true) {
            DDebug() << "onParamIllumSymbolGreenWhiteChanged called";
            DDebug() << "mPar_IllumDaySymbolGreen set";
            copyParamStringToUInt16TargetArray(tmp, mIllumDaySymbolGreenParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumNightSymbolGreen) == true) {
            DDebug() << "mPar_IllumNightSymbolGreen set";
            copyParamStringToUInt16TargetArray(tmp, mIllumNightSymbolGreenParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumDaySymbolWhite) == true) {
            DDebug() << "mPar_IllumDaySymbolWhite set";
            copyParamStringToUInt16TargetArray(tmp, mIllumDaySymbolWhiteParamDataArray);
        }

        if (updateValueIfValid(tmp, args.mPar_IllumNightSymbolWhite) == true) {
            DDebug() << "mPar_IllumNightSymbolWhite set";
            copyParamStringToUInt16TargetArray(tmp, mIllumNightSymbolWhiteParamDataArray);
        }
        updateGreenWhiteDuty();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_400ms,
        OFF_0ms,
        MAX
    };

    enum class EPrivate_LcdLightingStatus : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    enum class EPrivate_DialLightingStatus : HUInt64 {
        NONE,
        TURNING_ON,
        ON,
        TURNING_OFF,
        OFF,
        MAX
    };

    enum class EPrivate_NaviMapStatus : HUInt64 {
        NONE,
        DAY,
        NIGHT,
        MAX
    };

    // TODO(SHJ) : USM 안의 Inter_RheostatLevelSetReq 노드 만들어지면 INTER_RHEO_STAT_LEVEL_SET_REQ_CHANGED 추가 필요.
    enum class EPrivate_InterRheostatLevelUpdateReason : HUInt64 {
        NONE,
        INTER_CONFIG_AUTO_LIGHT_CHANGED,
        INTER_RHEO_STAT_SWITCH_STATUS_CHANGED,
        INPUT_RHEO_STAT_LEVEL_SET_REQ_STATUS_CHANGED,
        MAX
    };

    enum class EPrivate_LightingTransitionType : HUInt64 {
        NONE,
        LOG_STYLE_TRANSITION,
        EXPONENTIAL_STYLE_TRANSITION,
        MAX
    };

    enum class EPrivate_RheostatSwitchOperation : HUInt64 {
        NONE,
        UP,
        DOWN,
        DEFAULT,
        MAX
    };

    enum class EBlinkStatePrivate_RheostatSwitchOperation : HUInt64 {
        NONE,
        STATE_A,
        STATE_B,
        MAX
    };

    void updateInterIllCluster() {
        DDebug() << "Illu_SFC : updateInterIllCluster - previous mInter_IllCluster : "
                 << static_cast<uint64_t>(mInter_IllCluster);
        if ((mConfigAutolight == ConfigAutolight::OFF && mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::OFF) ||
            (mConfigAutolight == ConfigAutolight::ON && mAvTailLampStatus == AvTailLampStatus::OFF &&
             mAutolightSensorNightStatus == AutolightSensorNightStatus::OFF)) {
            mInter_IllCluster = SFCIlluminationInter_IllCluster::DAY;
        } else if (mConfigAutolight == ConfigAutolight::ON && mAvTailLampStatus == AvTailLampStatus::OFF &&
                   mAutolightSensorNightStatus == AutolightSensorNightStatus::NIGHT) {
            mInter_IllCluster = SFCIlluminationInter_IllCluster::DUSK;
        } else {
            mInter_IllCluster = SFCIlluminationInter_IllCluster::NIGHT;
        }

        DDebug() << "Illu_SFC : updateInterIllCluster - mConfigAutolight : " << static_cast<uint64_t>(mConfigAutolight);
        DDebug() << "Illu_SFC : updateInterIllCluster - mExtLowBeamOnReqStatus : "
                 << static_cast<uint64_t>(mExtLowBeamOnReqStatus);
        DDebug() << "Illu_SFC : updateInterIllCluster - mAvTailLampStatus : " << static_cast<uint64_t>(mAvTailLampStatus);
        DDebug() << "Illu_SFC : updateInterIllCluster - mAutolightSensorNightStatus : "
                 << static_cast<uint64_t>(mAutolightSensorNightStatus);
        DDebug() << "Illu_SFC : updateInterIllCluster - new mInter_IllCluster : " << static_cast<uint64_t>(mInter_IllCluster);

        setSFCIlluminationInter_IllCluster(mInter_IllCluster);

        // updateInterClusterLightingLevel 함수 내에 아래 항목 포함됨
        // updateOutDetentStatus();
        // updateOutIlluminationLevelValue();
        // updateOutIllumDutyValue();
        // updatePointerAndDialDuty();
        // updateGreenWhiteDuty();
        // updateRheostatFeedbackEventAvnLink();
        // updateLcdDuty();
        updateInterClusterLightingLevel();
        updateConstantDayNightStat();
    }

    void updateInterMinLightMode() {
        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_400ms) &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
            mUtilityModeOnOffStatus == UtilityModeOnOffStatus::MODE_ON && mCampModeOnOffStatus == CampModeOnOffStatus::ON) {
            mInter_MinLightMode = SFCIlluminationInter_MinLightMode::ON;
        } else {
            mInter_MinLightMode = SFCIlluminationInter_MinLightMode::OFF;
        }
        setSFCIlluminationInter_MinLightMode(mInter_MinLightMode);

        updateLcdDuty();
        updateOutIlluminationLevelValue();
        updateOutIllumDutyValue();
        updateOutDetentStatus();
        updateRheostatFeedbackEventNoAVN();
        updateRheostatFeedbackEventAvnLink();
        doFlush();
    }

    void updateIllumSaveCalLogic() {
        // 조명 단계 저장 및 불러오기 -> 저장
        // B+ on after 500ms
        DDebug() << "Illu_SFC : updateIllumSaveCalLogic - mAEMInitializeComplete : " << mAEMInitializeComplete;
        DDebug() << "Illu_SFC : updateIllumSaveCalLogic - mInter_RheostatLevelStatus : " << mInter_RheostatLevelStatus;
        if (mAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory.Inter_RheostatLevelStatus", mInter_RheostatLevelStatus);

            DDebug() << "Illu_SFC : updateIllumSaveCalLogic - updateValue : Memory.Inter_RheostatLevelStatus : "
                     << mInter_RheostatLevelStatus;
        } else {
            DDebug() << "Illu_SFC : updateIllumSaveCalLogic - updateMemory cannot be run due to AEM Not init";
        }
    }

    void updateIllumLoadCalLogic() {
        // 조명 단계 저장 및 불러오기 -> 불러오기
        // B+ on after 500ms
        DDebug() << "Illu_SFC : updateIllumLoadCalLogic - previous InterRheostatLevelStatus : " << mInter_RheostatLevelStatus;
        DDebug() << "Illu_SFC : updateIllumLoadCalLogic - mMemory_RheostatLevelStatus : " << mMemory_RheostatLevelStatus;
        mInter_RheostatLevelStatus = mMemory_RheostatLevelStatus;

        DDebug() << "Illu_SFC : updateIllumLoadCalLogic - current InterRheostatLevelStatus : " << mInter_RheostatLevelStatus;
    }

    void calInterRheostatLevelStatusByRheostatSwitchStatus() {
        // 4.3.2.1.3 현재 상태값 변경
        if (mEPrivate_RheostatSwitchOperationPrevious == EPrivate_RheostatSwitchOperation::DEFAULT &&
            mEPrivate_RheostatSwitchOperation == EPrivate_RheostatSwitchOperation::UP) {
            HUInt64 rheostatLevelMax = 0;
            if (mConfigAutolight == ConfigAutolight::ON) {
                rheostatLevelMax = kRheostatLevelAutoOnMax;
            } else {
                rheostatLevelMax = kRheostatLevelAutoOffMax;
            }

            if (mInter_RheostatLevelStatus < rheostatLevelMax) {
                mInter_RheostatLevelStatus++;
            }
        } else if (mEPrivate_RheostatSwitchOperationPrevious == EPrivate_RheostatSwitchOperation::DEFAULT &&
                   mEPrivate_RheostatSwitchOperation == EPrivate_RheostatSwitchOperation::DOWN) {
            if (mInter_RheostatLevelStatus > kRheostatLevelMin) {
                mInter_RheostatLevelStatus--;
            }
        } else {
            // maintain the status
        }
    }

    void calInterRheostatLevelStatusByRheostatLevelSetReqStatus() {
        if (mRheostLevelSetReqStatPrevious == RheostatLevelSetReqStat::DEFAULT &&
            (mRheostLevelSetReqStat < RheostatLevelSetReqStat::STEP21 ||
             (mConfigAutolight == ConfigAutolight::OFF && mRheostLevelSetReqStat == RheostatLevelSetReqStat::STEP21))) {
            mInter_RheostatLevelStatus = static_cast<HUInt64>(mRheostLevelSetReqStat);
        }
    }

    // TODO(SHJ) : USM Inter_RheostatLevelSetReq 노드 만들어지면 함수 구현해야함.

    // void calInterRheostatLevelStatusByRheostatLevelSetReq() {
    // }

    void updateInterRheostatLevelStatus(const EPrivate_InterRheostatLevelUpdateReason& updateReason) {
        DDebug() << "Illu_SFC : updateInterRheostatLevelStatus - updateReason : " << static_cast<uint64_t>(updateReason);
        DDebug() << "Illu_SFC : updateInterRheostatLevelStatus - previous mInter_RheostatLevelStatus : "
                 << mInter_RheostatLevelStatus;

        // AVN 적용 사양의 경우 업데이트 미실시
        if (mConfigAvnBrightLink == ConfigAvnBrightLink::ON) {
            DDebug() << "Illu_SFC : updateInterRheostatLevelStatus - AvnBrightLink option case. Ignore update.";
            return;
        }

        if (updateReason == EPrivate_InterRheostatLevelUpdateReason::INTER_RHEO_STAT_SWITCH_STATUS_CHANGED) {
            calInterRheostatLevelStatusByRheostatSwitchStatus();
        } else if (updateReason == EPrivate_InterRheostatLevelUpdateReason::INPUT_RHEO_STAT_LEVEL_SET_REQ_STATUS_CHANGED) {
            calInterRheostatLevelStatusByRheostatLevelSetReqStatus();
        } else {
            // no operation
        }
        // TODO(SHJ) : USM 안의 Inter_RheostatLevelSetReq 노드 만들어지면 구현해야함.
        // else if (updateReason == EPrivate_InterRheostatLevelUpdateReason::INTER_RHEO_STAT_LEVEL_SET_REQ_CHANGED) {
        //     calInterRheostatLevelStatusByRheostatLevelSetReq();
        // }

        DDebug() << "Illu_SFC : updateInterRheostatLevelStatus - new mInter_RheostatLevelStatus : " << mInter_RheostatLevelStatus;
        // Inter_RheostatLevelStatus 업데이트시에는 onRheostatLevelStatusChanged에 의존하게끔 처리.
        if (mInter_RheostatLevelStatus != mMemory_RheostatLevelStatus) {
            updateIllumSaveCalLogic();
        } else {  // 값의 업데이트 없을시(최고/최저)시는 팝업이벤트만 표출
            updateRheostatFeedbackEventNoAVN();
        }
    }

    void updateLcdDuty() {
        // 4.4.3.2 조명 동작 상태별 조명 제어 Inter_IllumLcdDuty
        if (mAEMInitializeComplete == true) {
            HDouble lcdDuty = 0.;
            updatePrivateDuty(mPrivate_DayLcdDutySetValue, mIllumDayLcdParamDataArray, mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_NightLcdDutySetValue, mIllumNightLcdParamDataArray, mInter_ClusterLightingLevel);
            if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON &&
                mConfigAvnBrightLink == ConfigAvnBrightLink::OFF &&
                mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF) {
                lcdDuty = mPrivate_DayLcdDutySetValue;
                DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::OFF, EPrivate_LcdLightingStatus::ON, "
                         << "Inter_MinLightMode::OFF";
            } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON &&
                       mConfigAvnBrightLink == ConfigAvnBrightLink::OFF &&
                       mInter_MinLightMode == SFCIlluminationInter_MinLightMode::ON) {
                // EXNCP-34054 사양변경 추가
                // Par_IllumDayLCD[1]
                lcdDuty = getUInt16ValueFromArrayAsHDoubleWithDiv100(mIllumDayLcdParamDataArray, kParamArrayIndexFirst);
                DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::OFF, EPrivate_LcdLightingStatus::ON, "
                         << "Inter_MinLightMode::ON";
            } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON &&
                       mConfigAvnBrightLink == ConfigAvnBrightLink::ON &&
                       mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::DAY) {
                if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF) {
                    lcdDuty = mPrivate_DayLcdDutySetValue;
                    DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::ON, EPrivate_NaviMapStatus::DAY, "
                             << "EPrivate_LcdLightingStatus::ON, Inter_MinLightMode::SFCIlluminationInter_MinLightMode::OFF";
                } else if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::ON) {
                    // Par_IllumDayLCD[1]
                    lcdDuty = getUInt16ValueFromArrayAsHDoubleWithDiv100(mIllumDayLcdParamDataArray, kParamArrayIndexFirst);
                    DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::ON, EPrivate_NaviMapStatus::DAY, "
                             << "EPrivate_LcdLightingStatus::ON, Inter_MinLightMode::SFCIlluminationInter_MinLightMode::ON";
                } else {
                    // no operation
                }
            } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON &&
                       mConfigAvnBrightLink == ConfigAvnBrightLink::ON &&
                       mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::NIGHT) {
                if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF) {
                    lcdDuty = mPrivate_NightLcdDutySetValue;
                    DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::ON, EPrivate_NaviMapStatus::NIGHT, "
                             << "EPrivate_LcdLightingStatus::ON, Inter_MinLightMode::SFCIlluminationInter_MinLightMode::OFF";
                } else if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::ON) {
                    // Par_IllumNightLCD[1]
                    lcdDuty = getUInt16ValueFromArrayAsHDoubleWithDiv100(mIllumNightLcdParamDataArray, kParamArrayIndexFirst);
                    DDebug() << "Illu_SFC : updateLcdDuty - ConfigAvnBrightLink::ON, EPrivate_NaviMapStatus::NIGHT, "
                             << "EPrivate_LcdLightingStatus::ON, mInter_MinLightMode::SFCIlluminationInter_MinLightMode::ON";
                } else {
                    // no operation
                }
            } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::OFF) {
                lcdDuty = 0.;
                DDebug() << "Illu_SFC : updateLcdDuty - EPrivate_LcdLightingStatus::OFF OOPS!";
            } else {
                // no operation
            }

            DDebug() << "Illu_SFC : updateLcdDuty - mIgnElapsed: " << static_cast<uint64_t>(mIgnElapsed);
            DDebug() << "Illu_SFC : updateLcdDuty - mInter_IllCluster: " << static_cast<uint64_t>(mInter_IllCluster);
            DDebug() << "Illu_SFC : updateLcdDuty - mInter_ClusterLightingLevel: "
                     << static_cast<uint64_t>(mInter_ClusterLightingLevel);
            DDebug() << "Illu_SFC : updateLcdDuty - mConfigAvnBrightLink: " << static_cast<uint64_t>(mConfigAvnBrightLink);
            DDebug() << "Illu_SFC : updateLcdDuty - mEPrivate_NaviMapStatus: " << static_cast<uint64_t>(mEPrivate_NaviMapStatus);
            DDebug() << "Illu_SFC : updateLcdDuty - mEPrivate_LcdLightingStatus: "
                     << static_cast<uint64_t>(mEPrivate_LcdLightingStatus);
            DDebug() << "Illu_SFC : updateLcdDuty - mPrivate_DayLcdDutySetValue: "
                     << static_cast<uint64_t>(mPrivate_DayLcdDutySetValue);
            DDebug() << "Illu_SFC : updateLcdDuty - mPrivate_NightLcdDutySetValue: "
                     << static_cast<uint64_t>(mPrivate_NightLcdDutySetValue);
            DDebug() << "Illu_SFC : updateLcdDuty - mEventInfoDisplayStatus: " << static_cast<uint64_t>(mEventInfoDisplayStatus);
            DDebug() << "Illu_SFC : updateLcdDuty - mIgOffDisplayOnStatus: " << static_cast<uint64_t>(mIgOffDisplayOnStatus);

            DDebug() << "Illu_SFC : updateLcdDuty - lcdDuty: " << lcdDuty;

            if (almostEqual(lcdDuty, static_cast<HDouble>(UINT16_MAX)) == false) {
                DDebug() << "Illu_SFC : updateLcdDuty - set lcdDuty : " << lcdDuty;
                // AEM과의 순환 참조 이슈로 아래 set함수의 제어를 추가, 사양서에 표현되지 않음 (JIRA: EXNCP-22760, islim)
                if (mIgnElapsed == IgnElapsed::OFF_0ms && mIgOffDisplayOnStatus != IgOffDisplayOnStatus::NONE) {
                    setSFCIlluminationInter_IllumLcdDuty(lcdDuty);
                } else if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_400ms) {
                    setSFCIlluminationInter_IllumLcdDuty(lcdDuty);
                } else {
                    // No Operation
                }
            } else {
                DDebug() << "Illu_SFC : updateLcdDuty - lcdDuty value is UINT16_MAX, cannot be set";
            }
        } else {
            DDebug() << "Illu_SFC : updateLcdDuty - NOT AEM INITIALIZED";
        }
    }

    void updatePointerAndDialDuty() {
        // 4.4.4 Pointer 및 Dial 조명 제어
        if (mAEMInitializeComplete == true) {
            HDouble dialDuty = 0.;
            HDouble pointerDuty = 0.;
            EPrivate_LightingTransitionType dialTransitionType = EPrivate_LightingTransitionType::NONE;
            EPrivate_LightingTransitionType pointerTransitionType = EPrivate_LightingTransitionType::NONE;
            updatePrivateDuty(mPrivate_DayDialDutySetValue, mIllumDayDialParamDataArray, mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_NightDialDutySetValue, mIllumNightDialParamDataArray, mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_DayPointerDutySetValue, mIllumDayPointerParamDataArray, mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_NightPointerDutySetValue, mIllumNightPointerParamDataArray, mInter_ClusterLightingLevel);

            if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_400ms) {
                // IGN ON일때 mEPrivate_DialLightingStatus는 ON / TURNING ON status만 있으므로 코드 공통적용
                if (mConfigAvnBrightLink == ConfigAvnBrightLink::OFF || mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::DAY) {
                    dialDuty = mPrivate_DayDialDutySetValue;        // DayDial
                    pointerDuty = mPrivate_DayPointerDutySetValue;  // DayPointer
                } else if (mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::NIGHT) {
                    dialDuty = mPrivate_NightDialDutySetValue;        // NightDial
                    pointerDuty = mPrivate_NightPointerDutySetValue;  // NightPointer
                } else {
                    // no operation
                }
                if (mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_ON) {
                    dialTransitionType = EPrivate_LightingTransitionType::EXPONENTIAL_STYLE_TRANSITION;
                    pointerTransitionType = EPrivate_LightingTransitionType::EXPONENTIAL_STYLE_TRANSITION;
                }
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms &&
                       mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_OFF) {
                // EPrivate_DialLightingStatus::OFF / TURNING_OFF 상태는 각 duty들 모두 0이므로
                // 변수 초기값 그대로임. duty = 0; 설정 코드 유지 불필요.
                dialTransitionType = EPrivate_LightingTransitionType::LOG_STYLE_TRANSITION;
                pointerTransitionType = EPrivate_LightingTransitionType::LOG_STYLE_TRANSITION;
            } else {
                // default value
            }

            DDebug() << "mInterClusterLightingLevel : " << mInter_ClusterLightingLevel;
            DDebug() << "dialDuty : " << dialDuty;
            DDebug() << "pointerDuty : " << pointerDuty;
            DDebug() << "dialTransitionType : " << static_cast<HUInt64>(dialTransitionType);
            DDebug() << "pointerTransitionType : " << static_cast<HUInt64>(pointerTransitionType);
            DDebug() << "mIgnElapsed : " << static_cast<HUInt64>(mIgnElapsed);
            DDebug() << "mEPrivate_DialLightingStatus : " << static_cast<HUInt64>(mEPrivate_DialLightingStatus);
            DDebug() << "mEPrivate_NaviMapStatus : " << static_cast<HUInt64>(mEPrivate_NaviMapStatus);
            DDebug() << "mConfigAvnBrightLink : " << static_cast<HUInt64>(mConfigAvnBrightLink);

            if (almostEqual(dialDuty, static_cast<HDouble>(UINT16_MAX)) == false) {
                DDebug() << "set dialDuty : " << dialDuty;
                requestSetDialDuty(dialDuty, dialTransitionType);
            }
            if (almostEqual(pointerDuty, static_cast<HDouble>(UINT16_MAX)) == false) {
                DDebug() << "set pointerDuty : " << pointerDuty;
                requestSetPointerDuty(pointerDuty, pointerTransitionType);
            }
        }
    }

    // Dial과 Pointer의 Transition Control은
    // 현재 값과 설정해야 할 값의 차이값에 대하여 Log / Exponential 패턴의 값만큼 Magnification 하여 현재값에 더하고,
    // 이를 Duty값으로 100ms마다 2초에 걸쳐 설정함.
    void requestSetDialDuty(HDouble targetDialValue,
                            EPrivate_LightingTransitionType controlTransitionType = EPrivate_LightingTransitionType::NONE) {
        if (mTransitionDialStatus == true) {
            return;
        } else {
            if (controlTransitionType == EPrivate_LightingTransitionType::NONE ||
                controlTransitionType == EPrivate_LightingTransitionType::MAX) {
                DDebug() << "dialValue without transition: " << targetDialValue;
                setSFCIlluminationInter_IllumDialDuty(targetDialValue);
                return;
            }

            GETCACHEDVALUE(SFC.Illumination.Inter_IllumDialDuty, mTransitionDialStartValue);
            if (almostEqual(mTransitionDialStartValue, targetDialValue) == true) {
                DDebug() << "requestSetDialDuty : Start value same with target value. do nothing.";

                return;
            }

            mTransitionDialDiffValue = targetDialValue - mTransitionDialStartValue;
            mTransitionDialTargetValue = targetDialValue;

            mRepeatTimerTransitionLogStyleDialTimerEvery100ms.stop();
            mRepeatTimerTransitionExpStyleDialTimerEvery100ms.stop();

            if (controlTransitionType == EPrivate_LightingTransitionType::LOG_STYLE_TRANSITION) {
                mTransitionDialStatus = true;
                if (mRepeatTimerTransitionLogStyleDialTimerEvery100ms.start() == false) {
                    DWarning() << "[Illumination] mRepeatTimerTransitionLogStyleDialTimerEvery100ms Start Failed";
                }
            } else if (controlTransitionType == EPrivate_LightingTransitionType::EXPONENTIAL_STYLE_TRANSITION) {
                mTransitionDialStatus = true;
                if (mRepeatTimerTransitionExpStyleDialTimerEvery100ms.start() == false) {
                    DWarning() << "[Illumination] mRepeatTimerTransitionExpStyleDialTimerEvery100ms Start Failed";
                }
            } else {
                // maintain the status
            }
        }
    }

    void onRepeatTimerTransitionLogStyleDialTimerEvery100msChanged() {
        if (mTransitionDialTimerRunningCount < kTransitionTimeSlotCount - 1) {
            transitionLogDialFunction();
        } else if (mTransitionDialTimerRunningCount == kTransitionTimeSlotCount - 1) {
            transitionLogDialFunction();
            mTransitionDialTimerRunningCount = 0;
            mTransitionDialStatus = false;
            mRepeatTimerTransitionLogStyleDialTimerEvery100ms.stop();
        } else {
            mTransitionDialTimerRunningCount = 0;
            mTransitionDialStatus = false;
            mRepeatTimerTransitionLogStyleDialTimerEvery100ms.stop();
        }
    }

    void onRepeatTimerTransitionExpStyleDialTimerEvery100msChanged() {
        if (mTransitionDialTimerRunningCount < kTransitionTimeSlotCount - 1) {
            transitionExpDialFunction();
        } else if (mTransitionDialTimerRunningCount == kTransitionTimeSlotCount - 1) {
            transitionExpDialFunction();
            mTransitionDialTimerRunningCount = 0;
            mTransitionDialStatus = false;
            mRepeatTimerTransitionExpStyleDialTimerEvery100ms.stop();
        } else {
            mTransitionDialTimerRunningCount = 0;
            mTransitionDialStatus = false;
            mRepeatTimerTransitionExpStyleDialTimerEvery100ms.stop();
        }
    }

    void transitionLogDialFunction() {
        mTransitionDialStatus = true;
        DDebug() << "mTransitionDialStartValue: " << mTransitionDialStartValue;
        DDebug() << "mTransitionDialDiffValue: " << mTransitionDialDiffValue;
        DDebug() << "mTransitionDialTimerRunningCount: " << mTransitionDialTimerRunningCount;
        DDebug() << "mTransitionTableLogArray[i]: " << mTransitionTableLogArray[mTransitionDialTimerRunningCount];
        HDouble dialValue =
            mTransitionDialStartValue + mTransitionDialDiffValue * mTransitionTableLogArray[mTransitionDialTimerRunningCount];
        mTransitionDialTimerRunningCount++;
        DDebug() << "dialValue: " << dialValue;
        DDebug() << "NEXT mTransitionDialTimerRunningCount: " << mTransitionDialTimerRunningCount;
        DDebug() << "mTransitionDialTargetValue : " << mTransitionDialTargetValue;
        setSFCIlluminationInter_IllumDialDuty(dialValue);
        doFlush();
    }

    void transitionExpDialFunction() {
        mTransitionDialStatus = true;
        DDebug() << "mTransitionDialStartValue: " << mTransitionDialStartValue;
        DDebug() << "mTransitionDialDiffValue: " << mTransitionDialDiffValue;
        DDebug() << "mTransitionDialTimerRunningCount: " << mTransitionDialTimerRunningCount;
        DDebug() << "mTransitionTableExpArray[i]: " << mTransitionTableExpArray[mTransitionDialTimerRunningCount];
        HDouble dialValue =
            mTransitionDialStartValue + mTransitionDialDiffValue * mTransitionTableExpArray[mTransitionDialTimerRunningCount];
        mTransitionDialTimerRunningCount++;
        DDebug() << "dialValue: " << dialValue;
        DDebug() << "NEXT mTransitionDialTimerRunningCount: " << mTransitionDialTimerRunningCount;
        DDebug() << "mTransitionDialTargetValue : " << mTransitionDialTargetValue;
        setSFCIlluminationInter_IllumDialDuty(dialValue);
        doFlush();
    }

    void requestSetPointerDuty(HDouble targetPointerValue,
                               EPrivate_LightingTransitionType controlTransitionType = EPrivate_LightingTransitionType::NONE) {
        if (mTransitionPointerStatus == true) {
            return;
        } else {
            if (controlTransitionType == EPrivate_LightingTransitionType::NONE ||
                controlTransitionType == EPrivate_LightingTransitionType::MAX) {
                DDebug() << "pointerValue without transition: " << targetPointerValue;
                setSFCIlluminationInter_IllumPointerDuty(targetPointerValue);
                return;
            }

            GETCACHEDVALUE(SFC.Illumination.Inter_IllumPointerDuty, mTransitionPointerStartValue);
            if (almostEqual(mTransitionPointerStartValue, targetPointerValue) == true) {
                DDebug() << "requestSetPointerDuty : Start value same with target value. do nothing.";
                return;
            }

            mTransitionPointerDiffValue = targetPointerValue - mTransitionPointerStartValue;
            mTransitionPointerTargetValue = targetPointerValue;
            mRepeatTimerTransitionLogStylePointerTimerEvery100ms.stop();
            mRepeatTimerTransitionExpStylePointerTimerEvery100ms.stop();

            if (controlTransitionType == EPrivate_LightingTransitionType::LOG_STYLE_TRANSITION) {
                mTransitionPointerStatus = true;
                if (mRepeatTimerTransitionLogStylePointerTimerEvery100ms.start() == false) {
                    DWarning() << "[Illumination] mRepeatTimerTransitionLogStylePointerTimerEvery100ms Start Failed";
                }
            } else if (controlTransitionType == EPrivate_LightingTransitionType::EXPONENTIAL_STYLE_TRANSITION) {
                mTransitionPointerStatus = true;
                if (mRepeatTimerTransitionExpStylePointerTimerEvery100ms.start() == false) {
                    DWarning() << "[Illumination] mRepeatTimerTransitionExpStylePointerTimerEvery100ms Start Failed";
                }
            } else {
                // maintain the status
            }
        }
    }

    void onRepeatTimerTransitionLogStylePointerTimerEvery100msChanged() {
        if (mTransitionPointerTimerRunningCount < kTransitionTimeSlotCount - 1) {
            transitionLogPointerFunction();
        } else if (mTransitionPointerTimerRunningCount == kTransitionTimeSlotCount - 1) {
            transitionLogPointerFunction();
            mTransitionPointerTimerRunningCount = 0;
            mTransitionPointerStatus = false;
            mRepeatTimerTransitionLogStylePointerTimerEvery100ms.stop();
        } else {
            mTransitionPointerTimerRunningCount = 0;
            mTransitionPointerStatus = false;
            mRepeatTimerTransitionLogStylePointerTimerEvery100ms.stop();
        }
    }

    void onRepeatTimerTransitionExpStylePointerTimerEvery100msChanged() {
        if (mTransitionPointerTimerRunningCount < kTransitionTimeSlotCount - 1) {
            transitionExpPointerFunction();
        } else if (mTransitionPointerTimerRunningCount == kTransitionTimeSlotCount - 1) {
            transitionExpPointerFunction();
            mTransitionPointerTimerRunningCount = 0;
            mTransitionPointerStatus = false;
            mRepeatTimerTransitionExpStylePointerTimerEvery100ms.stop();
        } else {
            mTransitionPointerTimerRunningCount = 0;
            mTransitionPointerStatus = false;
            mRepeatTimerTransitionExpStylePointerTimerEvery100ms.stop();
        }
    }

    void transitionLogPointerFunction() {
        mTransitionPointerStatus = true;
        DDebug() << "mTransitionPointerStartValue: " << mTransitionPointerStartValue;
        DDebug() << "mTransitionPointerDiffValue: " << mTransitionPointerDiffValue;
        DDebug() << "mTransitionPointerTimerRunningCount: " << mTransitionPointerTimerRunningCount;
        DDebug() << "mTransitionTableLogArray[i]: " << mTransitionTableLogArray[mTransitionPointerTimerRunningCount];
        HDouble pointerValue = mTransitionPointerStartValue +
                               mTransitionPointerDiffValue * mTransitionTableLogArray[mTransitionPointerTimerRunningCount];
        mTransitionPointerTimerRunningCount++;
        DDebug() << "pointerValue: " << pointerValue;
        DDebug() << "NEXT mTransitionPointerTimerRunningCount: " << mTransitionPointerTimerRunningCount;
        DDebug() << "mTransitionPointerTargetValue : " << mTransitionPointerTargetValue;
        setSFCIlluminationInter_IllumPointerDuty(pointerValue);
        doFlush();
    }

    void transitionExpPointerFunction() {
        mTransitionPointerStatus = true;
        DDebug() << "mTransitionPointerStartValue: " << mTransitionPointerStartValue;
        DDebug() << "mTransitionPointerDiffValue: " << mTransitionPointerDiffValue;
        DDebug() << "mTransitionPointerTimerRunningCount: " << mTransitionPointerTimerRunningCount;
        DDebug() << "mTransitionTableExpArray[i]: " << mTransitionTableExpArray[mTransitionPointerTimerRunningCount];
        HDouble pointerValue = mTransitionPointerStartValue +
                               mTransitionPointerDiffValue * mTransitionTableExpArray[mTransitionPointerTimerRunningCount];
        mTransitionPointerTimerRunningCount++;
        DDebug() << "pointerValue: " << pointerValue;
        DDebug() << "NEXT mTransitionPointerTimerRunningCount: " << mTransitionPointerTimerRunningCount;
        DDebug() << "mTransitionPointerTargetValue : " << mTransitionPointerTargetValue;
        setSFCIlluminationInter_IllumPointerDuty(pointerValue);
        doFlush();
    }

    void updateGreenWhiteDuty() {
        // 4.4.5 Telltale 조명 제어
        if (mAEMInitializeComplete == true) {
            HDouble greenDuty = 0.;
            HDouble whiteDuty = 0.;
            updatePrivateDuty(mPrivate_DaySymbolGreenDutySetValue, mIllumDaySymbolGreenParamDataArray,
                              mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_NightSymbolGreenDutySetValue, mIllumNightSymbolGreenParamDataArray,
                              mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_DaySymbolWhiteDutySetValue, mIllumDaySymbolWhiteParamDataArray,
                              mInter_ClusterLightingLevel);
            updatePrivateDuty(mPrivate_NightSymbolWhiteDutySetValue, mIllumNightSymbolWhiteParamDataArray,
                              mInter_ClusterLightingLevel);
            if (mConfigAvnBrightLink == ConfigAvnBrightLink::OFF || mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::DAY) {
                greenDuty = mPrivate_DaySymbolGreenDutySetValue;
                whiteDuty = mPrivate_DaySymbolWhiteDutySetValue;
            } else if (mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::NIGHT) {
                greenDuty = mPrivate_NightSymbolGreenDutySetValue;
                whiteDuty = mPrivate_NightSymbolWhiteDutySetValue;
            } else {
                // default value(0.)
            }

            DDebug() << "greenDuty : " << greenDuty;
            DDebug() << "whiteDuty : " << whiteDuty;

            if (almostEqual(greenDuty, static_cast<HDouble>(UINT16_MAX)) == false) {
                DDebug() << "set greenDuty : " << greenDuty;
                setSFCIlluminationInter_IllumSymbolGreenDuty(greenDuty);
            }
            if (almostEqual(whiteDuty, static_cast<HDouble>(UINT16_MAX)) == false) {
                DDebug() << "set whiteDuty : " << whiteDuty;
                setSFCIlluminationInter_IllumSymbolWhiteDuty(whiteDuty);
            }
        }
    }

    void updateOutRheostatLevelStat() {
        // 4.5 조명 조절 USM 생성 제어
        // Output_RheostatLevelStatus의 STEP1 == Index 0, mInter_RheostatLevelStatus의 STEP1 == Index 1.

        AddressValueList outputAddressList;
        outputAddressList.emplace_back(Vehicle.CD.Illumination.Output_RheostatLevelStatus,
                                       mInter_RheostatLevelStatus - kRheostatLevelConvertIndex);
        setValue(outputAddressList);
        outputAddressList.clear();
        DDebug() << "set Output_RheostatLevelStatus : " << (mInter_RheostatLevelStatus - kRheostatLevelConvertIndex);
    }

    void updateOutIllumDutyValue() {
        // 4.5.2 조명 제어 출력
        if (mAEMInitializeComplete == true) {
            HDouble dutyValue = 50.0;
            if (mPrivate_LightOutLevel <= 2 || mInter_MinLightMode == SFCIlluminationInter_MinLightMode::ON) {
                dutyValue = 3.00;
            } else if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF) {
                if (mPrivate_LightOutLevel > 2 && mPrivate_LightOutLevel < 24) {
                    dutyValue =
                        roundOffTo2DecimalPlaces(3.00 + (static_cast<HDouble>(mPrivate_LightOutLevel) - 2.0) * 47.0 / 22.0);
                } else if (mPrivate_LightOutLevel == 24) {
                    dutyValue = 50.00;
                } else if (mPrivate_LightOutLevel > 24 && mPrivate_LightOutLevel < 90) {
                    dutyValue =
                        roundOffTo2DecimalPlaces(50.00 + (static_cast<HDouble>(mPrivate_LightOutLevel) - 24.0) * 50.0 / 66.0);
                } else if (mPrivate_LightOutLevel >= 90) {
                    dutyValue = 100.0;
                } else {
                    // Never get to here.
                }
            } else {
                // no operation
            }

            AddressValueList outputAddressList;
            HDouble dutyValue_x100 = dutyValue * 100.0;
            outputAddressList.emplace_back(Vehicle.System.HardWire.Output_IlluminationDutyValue,
                                           static_cast<ccos::HUInt64>(dutyValue_x100));
            setValue(outputAddressList);
            outputAddressList.clear();
            DDebug() << "set Output_IlluminationDutyValue : " << dutyValue;
        }
    }

    void updateOutRheostatMaxInfo() {
        // 4.6 조명 조절 USM 생성 제어
        AddressValueList outputAddressList;
        decltype(Vehicle.CD.Illumination.Output_RheostatMaxInfo)::TYPE output_RheostatMaxInfoType =
            decltype(Vehicle.CD.Illumination.Output_RheostatMaxInfo)::TYPE::INVALID;
        if (mConfigAvnBrightLink == ConfigAvnBrightLink::OFF) {
            if (mConfigAutolight == ConfigAutolight::ON) {
                output_RheostatMaxInfoType = decltype(Vehicle.CD.Illumination.Output_RheostatMaxInfo)::TYPE::STEP20;
            } else if (mConfigAutolight == ConfigAutolight::OFF) {
                output_RheostatMaxInfoType = decltype(Vehicle.CD.Illumination.Output_RheostatMaxInfo)::TYPE::STEP21;
            } else {
                // no operation
            }
        } else if (mConfigAvnBrightLink == ConfigAvnBrightLink::ON) {
            output_RheostatMaxInfoType = decltype(Vehicle.CD.Illumination.Output_RheostatMaxInfo)::TYPE::INVALID;
        } else {
            // no operation
        }

        outputAddressList.emplace_back(Vehicle.CD.Illumination.Output_RheostatMaxInfo,
                                       static_cast<HUInt64>(output_RheostatMaxInfoType));

        setValue(outputAddressList);
        outputAddressList.clear();
    }

    void updateOutDetentStatus() {
        // 4.4.2 조명 제어 출력
        AddressValueList outputAddressList;
        decltype(Vehicle.System.HardWire.Output_DetentStatus)::TYPE output_HardWireDetentStatusType =
            decltype(Vehicle.System.HardWire.Output_DetentStatus)::TYPE::OFF;
        decltype(Vehicle.CD.Illumination.Output_DetentStatus)::TYPE output_IllumDetentStatusType =
            decltype(Vehicle.CD.Illumination.Output_DetentStatus)::TYPE::OFF;
        if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF &&
            (mInter_IllCluster == SFCIlluminationInter_IllCluster::DAY || mInter_ClusterLightingLevel >= 455)) {
            output_HardWireDetentStatusType = decltype(Vehicle.System.HardWire.Output_DetentStatus)::TYPE::ON;
            output_IllumDetentStatusType = decltype(Vehicle.CD.Illumination.Output_DetentStatus)::TYPE::ON;
        } else if (mInter_MinLightMode != SFCIlluminationInter_MinLightMode::OFF ||
                   (mInter_IllCluster != SFCIlluminationInter_IllCluster::DAY && mInter_ClusterLightingLevel < 455)) {
            output_HardWireDetentStatusType = decltype(Vehicle.System.HardWire.Output_DetentStatus)::TYPE::OFF;
            output_IllumDetentStatusType = decltype(Vehicle.CD.Illumination.Output_DetentStatus)::TYPE::OFF;
        } else {
            // no operation
        }

        outputAddressList.emplace_back(Vehicle.System.HardWire.Output_DetentStatus,
                                       static_cast<HUInt64>(output_HardWireDetentStatusType));

        outputAddressList.emplace_back(Vehicle.CD.Illumination.Output_DetentStatus,
                                       static_cast<HUInt64>(output_IllumDetentStatusType));
        setValue(outputAddressList);
        outputAddressList.clear();
    }

    void updateOutIlluminationLevelValue() {
        AddressValueList outputAddressList;
        HUInt64 lvlVal = 0;
        if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF) {
            lvlVal = static_cast<HUInt64>(mPrivate_LightOutLevel);
        } else if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::ON) {
            lvlVal = 2;
        } else {
            // no operation
        }
        outputAddressList.emplace_back(Vehicle.CD.Illumination.Output_IlluminationLevelValue, static_cast<HUInt64>(lvlVal));
        setValue(outputAddressList);
        outputAddressList.clear();
    }

    void updateOutNotMinIlluminationLevelValue() {
        AddressValueList outputAddressList;
        outputAddressList.emplace_back(Vehicle.CD.Illumination.Output_NotMinIlluminationLevelValue,
                                       static_cast<HUInt64>(mPrivate_LightOutLevel));
        setValue(outputAddressList);
        outputAddressList.clear();
    }

    void updateInterClusterLightingLevel() {
        // 4.3.2.2 종합 판단, 4.3.3 AVN 연계 옵션 적용시 동작사양
        if (mConfigAvnBrightLink == ConfigAvnBrightLink::OFF) {
            if (mInter_RheostatLevelStatus == 21) {
                mInter_ClusterLightingLevel = 1000;
            } else if (mInter_IllCluster == SFCIlluminationInter_IllCluster::NIGHT) {
                mInter_ClusterLightingLevel =
                    mClusterLightingLevelNightTableArray[mInter_RheostatLevelStatus - kRheostatLevelConvertIndex];
            } else if (mInter_IllCluster == SFCIlluminationInter_IllCluster::DUSK) {
                mInter_ClusterLightingLevel =
                    mClusterLightingLevelDuskTableArray[mInter_RheostatLevelStatus - kRheostatLevelConvertIndex];
            } else if (mInter_IllCluster == SFCIlluminationInter_IllCluster::DAY) {
                mInter_ClusterLightingLevel =
                    mClusterLightingLevelDayTableArray[mInter_RheostatLevelStatus - kRheostatLevelConvertIndex];
            } else {
                // default value
            }
        } else if (mConfigAvnBrightLink == ConfigAvnBrightLink::ON) {
            if (mAvnAutoBrightValue >= 1 && mAvnAutoBrightValue <= 1000) {
                mInter_ClusterLightingLevel = mAvnAutoBrightValue;
            }
        } else {
            // no operation
        }

        DDebug() << "Illu_SFC : updateInterClusterLightingLevel - mConfigAvnBrightLink : "
                 << static_cast<uint64_t>(mConfigAvnBrightLink);
        DDebug() << "Illu_SFC : updateInterClusterLightingLevel - mInter_RheostatLevelStatus : " << mInter_RheostatLevelStatus;
        DDebug() << "Illu_SFC : updateInterClusterLightingLevel - mInter_IllCluster : "
                 << static_cast<uint64_t>(mInter_IllCluster);
        DDebug() << "Illu_SFC : updateInterClusterLightingLevel - mAvnAutoBrightValue : " << mAvnAutoBrightValue;
        setSFCIlluminationInter_ClusterLightingLevel(mInter_ClusterLightingLevel);

        DDebug() << "Illu_SFC : updateInterClusterLightingLevel - mInter_ClusterLightingLevel : " << mInter_ClusterLightingLevel;
        // 5.2.2 클러스터 조명 밝기 수준
        mIMG_ClusterLightingLevel_Stat = static_cast<HUInt64>(round(static_cast<HDouble>(mInter_ClusterLightingLevel) / 10.0));
        setSFCIlluminationConstantClusterLightingLevelStatValue(mIMG_ClusterLightingLevel_Stat);

        // 4.5.1 출력단계 결정
        if (mInter_ClusterLightingLevel < 5) {
            mPrivate_LightOutLevel = 1;
        } else {
            mPrivate_LightOutLevel = mInter_ClusterLightingLevel / 5;
        }

        // 5.3.2 Rheostat 스위치 조작 피드백 표시 (AVN 연동 사양)
        updateRheostatFeedbackEventAvnLink();

        updateOutDetentStatus();
        updateOutIlluminationLevelValue();
        updateOutNotMinIlluminationLevelValue();
        updateOutIllumDutyValue();
        updatePointerAndDialDuty();
        updateGreenWhiteDuty();
        updateLcdDuty();
    }

    void updatePrivate_LcdLightingStatus() {
        // 4.4.3.1 조명 동작 상태 - LCD 조명 제어
        DDebug() << "Illu_SFC : Enter updatePrivate_LcdLightingStatus()";
        DDebug() << "Illu_SFC : Before mEPrivate_LcdLightingStatus : " << static_cast<uint64_t>(mEPrivate_LcdLightingStatus);
        if (mIgnElapsed == IgnElapsed::ON_400ms) {
            mEPrivate_LcdLightingStatus = EPrivate_LcdLightingStatus::ON;
            DDebug() << "Illu_SFC : mEPrivate_LcdLightingStatus == ON, IGN ON 400ms";
        } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
            // 5.4.4.4.3.1 테이블에서 Private_IgOffDisplay 값 계산은 App에서 수행하고
            // 그 값을 SFC.Extension.Illumination.Inter_IgOffDisplayOnStatus로 받는다.
            DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus IGN OFF";
            if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::OFF &&
                (mEventInfoDisplayStatus == EventInfoDisplayStatus::ON || mIgOffDisplayOnStatus == IgOffDisplayOnStatus::ON)) {
                mEPrivate_LcdLightingStatus = EPrivate_LcdLightingStatus::ON;
            } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON &&
                       mInter_EventInfoDisplayStatusOff5000ms == true && mPrivate_IgOffDisplayOnStatusOff5000ms == true) {
                mEPrivate_LcdLightingStatus = EPrivate_LcdLightingStatus::OFF;
            } else {
                DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus IGN OFF, No condition matched";
            }
        } else {
            DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus IGN Condition Not matched!";
        }
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - IGN Status : " << static_cast<uint64_t>(mIgnElapsed);
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - mEventInfoDisplayStatus : "
                 << static_cast<uint64_t>(mEventInfoDisplayStatus);
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - mIgOffDisplayOnStatus : "
                 << static_cast<uint64_t>(mIgOffDisplayOnStatus);
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - mInter_EventInfoDisplayStatusOff5000ms : "
                 << mInter_EventInfoDisplayStatusOff5000ms;
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - mPrivate_IgOffDisplayOnStatusOff5000ms : "
                 << mPrivate_IgOffDisplayOnStatusOff5000ms;
        DDebug() << "Illu_SFC : updatePrivate_LcdLightingStatus last stage - mEPrivate_LcdLightingStatus : "
                 << static_cast<uint64_t>(mEPrivate_LcdLightingStatus);
        updateLcdDuty();
        updateConstantLcdLightingStat();
    }

    void updateConstantLcdLightingStat() {
        // update LcdLightingStatus
        if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::ON) {
            setSFCIlluminationConstantLcdLightingStat(SFCIlluminationConstantLcdLightingStat::ON);
        } else if (mEPrivate_LcdLightingStatus == EPrivate_LcdLightingStatus::OFF) {
            setSFCIlluminationConstantLcdLightingStat(SFCIlluminationConstantLcdLightingStat::OFF);
        } else {
            // N.A
        }
    }

    void updatePrivate_NaviMapStatus() {
        mEPrivate_NaviMapStatusPrevious = mEPrivate_NaviMapStatus;
        DDebug() << "Illu_SFC : updatePrivate_NaviMapStatus - mEPrivate_NaviMapStatusPrevious : "
                 << static_cast<uint64_t>(mEPrivate_NaviMapStatusPrevious);

        if (mEPrivate_NaviMapStatusPrevious == EPrivate_NaviMapStatus::DAY &&
            mNaviMapDayNightStatus == NaviMapDayNightStatus::INVALID) {
            mEPrivate_NaviMapStatus = EPrivate_NaviMapStatus::DAY;
        } else if (mEPrivate_NaviMapStatusPrevious == EPrivate_NaviMapStatus::NIGHT &&
                   mNaviMapDayNightStatus == NaviMapDayNightStatus::INVALID) {
            mEPrivate_NaviMapStatus = EPrivate_NaviMapStatus::NIGHT;
        } else if (mNaviMapDayNightStatus == NaviMapDayNightStatus::DAY) {
            mEPrivate_NaviMapStatus = EPrivate_NaviMapStatus::DAY;
        } else if (mNaviMapDayNightStatus == NaviMapDayNightStatus::NIGHT) {
            mEPrivate_NaviMapStatus = EPrivate_NaviMapStatus::NIGHT;
        } else {
            // no operation
        }

        if (mAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory_IGN.Inter_NaviMapStatus", static_cast<HUInt64>(mEPrivate_NaviMapStatus));
        }

        DDebug() << "Illu_SFC : updatePrivate_NaviMapStatus - mNaviMapDayNightStatus : "
                 << static_cast<uint64_t>(mNaviMapDayNightStatus);

        DDebug() << "Illu_SFC : updatePrivate_NaviMapStatus - mEPrivate_NaviMapStatus : "
                 << static_cast<uint64_t>(mEPrivate_NaviMapStatus);
        updateLcdDuty();
        updatePointerAndDialDuty();
        updateGreenWhiteDuty();
        updateConstantDayNightStat();
    }

    void updatePrivate_DialLightingStatus() {
        // 4.4.4 Pointer 및 Dial 조명 제어
        if (mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::OFF && mIgnElapsed == IgnElapsed::ON_400ms) {
            mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::TURNING_ON;
            requestSetTurnOn2000msPrivate_DialLightingStatusTimer();
        } else if (mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_OFF &&
                   mIgnElapsed == IgnElapsed::ON_0ms) {
            mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::TURNING_ON;
            requestSetTurnOn2000msPrivate_DialLightingStatusTimer();
        } else if ((mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::ON ||
                    mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_ON) &&
                   mIgnElapsed == IgnElapsed::OFF_0ms) {
            mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::TURNING_OFF;
            requestSetTurnOff2000msPrivate_DialLightingStatusTimer();
        } else {
            // no operation
        }

        DDebug() << "mEPrivate_DialLightingStatus : " << static_cast<HUInt64>(mEPrivate_DialLightingStatus);
        updatePointerAndDialDuty();
    }

    void updatePrivate_RheostatSwitchOperation() {
        // 4.3.2.1.2 스위치 동작 이벤트 정의 - oneshot
        mEPrivate_RheostatSwitchOperationPrevious = mEPrivate_RheostatSwitchOperation;

        if (mRheostatSwitch == RheostatSwitch::UP) {
            if (mRheostatSwitchPrevious == RheostatSwitch::DEFAULT) {
                mEPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::UP;
                requestSet500msPrivate_RheostatSwitchOperationTimer();
            } else if (mRheostatSwitchPrevious == RheostatSwitch::UP) {
                DDebug() << "Update again: Inter_RheostatSwitchStatus (" << static_cast<HUInt64>(mRheostatSwitchPrevious)
                         << ") => (" << static_cast<HUInt64>(mRheostatSwitch) << ")";
            } else {
                DDebug() << "Undefined case: Inter_RheostatSwitchStatus (" << static_cast<HUInt64>(mRheostatSwitchPrevious)
                         << ") => (" << static_cast<HUInt64>(mRheostatSwitch) << ")";
            }
        } else if (mRheostatSwitch == RheostatSwitch::DOWN) {
            if (mRheostatSwitchPrevious == RheostatSwitch::DEFAULT) {
                mEPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::DOWN;
                requestSet500msPrivate_RheostatSwitchOperationTimer();
            } else if (mRheostatSwitchPrevious == RheostatSwitch::DOWN) {
                DDebug() << "Update again: Inter_RheostatSwitchStatus (" << static_cast<HUInt64>(mRheostatSwitchPrevious)
                         << ") => (" << static_cast<HUInt64>(mRheostatSwitch) << ")";
            } else {
                DDebug() << "Undefined case: Inter_RheostatSwitchStatus (" << static_cast<HUInt64>(mRheostatSwitchPrevious)
                         << ") => (" << static_cast<HUInt64>(mRheostatSwitch) << ")";
            }
        } else {
            mEPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::DEFAULT;
            cancelSet500msPrivate_RheostatSwitchOperationTimer();
        }
    }

    uint32_t updatePrivate_RheostatSwitchOperationBlink() {
        // 4.3.2.1.2 스위치 동작 이벤트 정의 - periodic
        mEPrivate_RheostatSwitchOperationPrevious = mEPrivate_RheostatSwitchOperation;

        if (mBlinkStatePrivate_RheostatSwitchOperation == EBlinkStatePrivate_RheostatSwitchOperation::STATE_A) {
            mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::STATE_B;
            mEPrivate_RheostatSwitchOperation = mBlinkValueBPrivate_RheostatSwitchOperation;
            return kBlinkTimeBPrivate_RheostatSwitchOperation50ms;
        } else if (mBlinkStatePrivate_RheostatSwitchOperation == EBlinkStatePrivate_RheostatSwitchOperation::STATE_B) {
            mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::STATE_A;
            mEPrivate_RheostatSwitchOperation = mBlinkValueAPrivate_RheostatSwitchOperation;
            return kBlinkTimeAPrivate_RheostatSwitchOperation50ms;
        } else {
            // no operation
        }
        DDebug() << "undefined state for mBlinkStatePrivate_RheostatSwitchOperation: "
                 << static_cast<HUInt64>(mBlinkStatePrivate_RheostatSwitchOperation);
        mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::NONE;
        return 0;
    }

    // 5.2.3 주야간 판단
    void updateConstantDayNightStat() {
        SFCIlluminationConstantDayNightStat stat = SFCIlluminationConstantDayNightStat::NONE;

        if (mConfigAvnBrightLink == ConfigAvnBrightLink::ON) {
            if (mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::DAY) {
                stat = SFCIlluminationConstantDayNightStat::DAY;
            } else if (mEPrivate_NaviMapStatus == EPrivate_NaviMapStatus::NIGHT) {
                stat = SFCIlluminationConstantDayNightStat::NIGHT;
            } else {
                // nothing
            }
        } else if (mConfigAvnBrightLink == ConfigAvnBrightLink::OFF) {
            if (mInter_IllCluster == SFCIlluminationInter_IllCluster::DAY ||
                mInter_IllCluster == SFCIlluminationInter_IllCluster::DUSK) {
                stat = SFCIlluminationConstantDayNightStat::DAY;
            } else if (mInter_IllCluster == SFCIlluminationInter_IllCluster::NIGHT) {
                stat = SFCIlluminationConstantDayNightStat::NIGHT;
            } else {
                // nothing
            }
        } else {
            // nothing
        }
        setSFCIlluminationConstantDayNightStat(stat);
    }

    void requestSetTurnOn2000msPrivate_DialLightingStatusTimer() {
        mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms.stop();
        mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms.stop();
        if (mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms.start() == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms Start Failed";
        }
    }

    void requestSetTurnOff2000msPrivate_DialLightingStatusTimer() {
        mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms.stop();
        mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms.stop();
        if (mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms.start() == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms Start Failed";
        }
    }

    void requestSet500msPrivate_RheostatSwitchOperationTimer() {
        cancelSet500msPrivate_RheostatSwitchOperationTimer();
        if (mOneShotTimerPrivate_RheostatSwitchOperation500ms.start() == false) {
            DWarning() << "[Illumination] mOneShotTimerPrivate_RheostatSwitchOperation500ms Start Failed";
        }
    }

    void requestBlinkPrivate_RheostatSwitchOperationTimer(const uint32_t& myTimeout) {
        mOneShotTimerBlinkPrivate_RheostatSwitchOperation.stop();
        mOneShotTimerBlinkPrivate_RheostatSwitchOperation.setInterval(myTimeout);
        if (mOneShotTimerBlinkPrivate_RheostatSwitchOperation.start() == false) {
            DWarning() << "[Illumination] mOneShotTimerBlinkPrivate_RheostatSwitchOperation Start Failed";
        }
    }

    void cancelSet500msPrivate_RheostatSwitchOperationTimer() {
        mOneShotTimerPrivate_RheostatSwitchOperation500ms.stop();
        cancelSetPrivate_RheostatSwitchOperationTimer();
    }

    void cancelSetPrivate_RheostatSwitchOperationTimer() {
        mOneShotTimerBlinkPrivate_RheostatSwitchOperation.stop();
    }

    void onOneShotTimerPrivate_DialLightingStatusTurningOn2000msChanged() {
        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_400ms) &&
            mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_ON) {
            mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::ON;
            DDebug() << "mEPrivate_DialLightingStatus : " << static_cast<HUInt64>(mEPrivate_DialLightingStatus);
            updatePointerAndDialDuty();
            doFlush();
        }
    }

    void onOneShotTimerPrivate_DialLightingStatusTurningOff2000msChanged() {
        if (mIgnElapsed == IgnElapsed::OFF_0ms && mEPrivate_DialLightingStatus == EPrivate_DialLightingStatus::TURNING_OFF) {
            mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::OFF;
            DDebug() << "mEPrivate_DialLightingStatus : " << static_cast<HUInt64>(mEPrivate_DialLightingStatus);
            updatePointerAndDialDuty();
            doFlush();
        }
    }

    void onOneShotTimerPrivate_RheostatSwitchOperation500msChanged() {
        if (mEPrivate_RheostatSwitchOperation == EPrivate_RheostatSwitchOperation::UP) {
            mBlinkValueAPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::UP;
            mBlinkValueBPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::DEFAULT;
            mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::STATE_B;
            onOneShotTimerPrivate_RheostatSwitchOperationBlink();
        } else if (mEPrivate_RheostatSwitchOperation == EPrivate_RheostatSwitchOperation::DOWN) {
            mBlinkValueAPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::DOWN;
            mBlinkValueBPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::DEFAULT;
            mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::STATE_B;
            onOneShotTimerPrivate_RheostatSwitchOperationBlink();
        } else {
            mBlinkValueAPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::NONE;
            mBlinkValueBPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::NONE;
            mBlinkStatePrivate_RheostatSwitchOperation = EBlinkStatePrivate_RheostatSwitchOperation::NONE;
            cancelSetPrivate_RheostatSwitchOperationTimer();
        }
    }

    void onOneShotTimerPrivate_RheostatSwitchOperationBlink() {
        uint32_t timeInterval = updatePrivate_RheostatSwitchOperationBlink();
        if (timeInterval > 0) {
            requestBlinkPrivate_RheostatSwitchOperationTimer(timeInterval);
        }

        if (mEPrivate_RheostatSwitchOperationPrevious == EPrivate_RheostatSwitchOperation::DEFAULT &&
            mEPrivate_RheostatSwitchOperationPrevious != mEPrivate_RheostatSwitchOperation) {
            updateInterRheostatLevelStatus(EPrivate_InterRheostatLevelUpdateReason::INTER_RHEO_STAT_SWITCH_STATUS_CHANGED);
            doFlush();
        }
    }

    void onOneShotTimerIgnElapsedOn400msChanged() {
        mIgnElapsed = IgnElapsed::ON_400ms;
        DDebug() << "Illu_SFC : IGN ON 400ms";
        updateInterMinLightMode();
        updatePrivate_LcdLightingStatus();   //  updateLcdDuty() 포함
        updatePrivate_DialLightingStatus();  //  updatePointerAndDialDuty() 포함
        doFlush();
    }

    void onOneShotTimerInter_EventInfoDisplayStatusOff5000msTimeout() {
        mInter_EventInfoDisplayStatusOff5000ms = true;

        DDebug() << "Illu_SFC : onOneShotTimerInter_EventInfoDisplayStatusOff5000msTimeout called";
        updatePrivate_LcdLightingStatus();
        doFlush();
    }

    void onOneShotTimerPrivate_IgOffDisplayOnStatusOff5000msTimeout() {
        mPrivate_IgOffDisplayOnStatusOff5000ms = true;

        DDebug() << "Illu_SFC : onOneShotTimerPrivate_IgOffDisplayOnStatusOff5000msTimeout called";
        updatePrivate_LcdLightingStatus();
        doFlush();
    }

    // NOTE: 오직 스위치 입력에 의해서만 IMG_RheostatLevel_stat값 변경되어 E00501 팝업 이벤트 갱신됨.(SFCDEV-69)
    void updateRheostatFeedbackEventNoAVN() {
        if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF && mConfigAvnBrightLink == ConfigAvnBrightLink::OFF &&
            mRheostatSwitchPrevious == RheostatSwitch::DEFAULT &&
            (mRheostatSwitch == RheostatSwitch::UP || mRheostatSwitch == RheostatSwitch::DOWN)) {
            setSFCIlluminationEventRheostatFeedbackNoAVNStat(SFCIlluminationEventRheostatFeedbackNoAVNStat::ON);
            mForceUpdateGroup = true;
        } else {
            setSFCIlluminationEventRheostatFeedbackNoAVNStat(SFCIlluminationEventRheostatFeedbackNoAVNStat::OFF);
        }
    }

    void updateRheostatFeedbackEventAvnLink() {
        if (mInter_MinLightMode == SFCIlluminationInter_MinLightMode::OFF && mConfigAvnBrightLink == ConfigAvnBrightLink::ON &&
            ((mRheostatSwitchPrevious == RheostatSwitch::DEFAULT &&
              (mRheostatSwitch == RheostatSwitch::UP || mRheostatSwitch == RheostatSwitch::DOWN)) ||
             mIsE00502PopupEventOn == true)) {
            setSFCIlluminationEventRheostatFeedbackAvnLinkStat(SFCIlluminationEventRheostatFeedbackAvnLinkStat::ON);
            mForceUpdateGroup = true;
        } else {
            setSFCIlluminationEventRheostatFeedbackAvnLinkStat(SFCIlluminationEventRheostatFeedbackAvnLinkStat::OFF);
        }
    }

    void doFlush() {
        flush(mForceUpdateGroup);
        mForceUpdateGroup = false;
    }

    template <size_t N>
    void copyParamStringToUInt16TargetArray(const std::string& paramDataUInt16Array, std::array<uint16_t, N>& targetArray) {
        // 바이트수로 비교함
        if (targetArray.size() * sizeof(uint16_t) == paramDataUInt16Array.size()) {
            targetArray = {
                0,
            };
            std::memcpy(targetArray.data(), paramDataUInt16Array.data(), paramDataUInt16Array.size());
            // 보통 200, 1000개 데이터가 들어오므로 실제 로그 출력시에는 too long 처리되어 끊길 것으로 보임.
            std::string logString;
            for (size_t i = 0; i < targetArray.size(); i++) {
                logString.append("[");
                logString.append(std::to_string(i));
                logString.append("] : ");
                logString.append(std::to_string(targetArray[i]));
                logString.append("\t");
            }
            DDebug() << logString;
        } else {
            DError() << "copyParamStringToUInt16TargetArray : param data array size is not same with targetArray";
            DError() << "paramDataUInt16Array Size : " << paramDataUInt16Array.size();
            DError() << "sizeof(uint16_t) : " << sizeof(uint16_t);
            DError() << "targetArray size : " << targetArray.size();
            DError() << "copyParamStringToUInt16TargetArray didn't any copying process.";
        }
        return;
    }

    template <size_t N>
    HDouble getUInt16ValueFromArrayAsHDoubleWithDiv100(const std::array<uint16_t, N>& targetArray, const size_t index) {
        if (targetArray.size() > index) {  // array 범위 내의 index로 데이터 접근가능하도록 조건 확인
            return static_cast<HDouble>(targetArray[index]) / 100.0;
        }
        DError() << "TARGET ARRAY SIZE : " << targetArray.size();
        DError() << "INDEX ERROR : " << index;
        return static_cast<HDouble>(UINT16_MAX);
    }

    template <size_t N>
    void updatePrivateDuty(HDouble& privateTargetDutyValue, const std::array<uint16_t, N>& paramDataArray,
                           const HUInt64& interCluLightLv) {
        // 4.4.2 Duty비 값 설정
        if (interCluLightLv <= 10) {
            privateTargetDutyValue = getUInt16ValueFromArrayAsHDoubleWithDiv100(paramDataArray, kParamArrayIndexFirst);
        } else if (interCluLightLv >= 1000) {
            privateTargetDutyValue = getUInt16ValueFromArrayAsHDoubleWithDiv100(paramDataArray, kParamArrayIndexFourth);
        } else {
            HUInt64 idx_min = 0;
            HUInt64 idx_max = 0;
            HUInt64 offset = 0;
            HUInt64 interCluLightLvDiff = 0;
            HDouble denom = 0.;
            if (interCluLightLv > 10 && interCluLightLv < 120) {
                idx_min = kParamArrayIndexFirst;
                idx_max = kParamArrayIndexSecond;
                offset = 10;
                denom = 110.0;
            } else if (interCluLightLv >= 120 && interCluLightLv < 450) {
                idx_min = kParamArrayIndexSecond;
                idx_max = kParamArrayIndexThird;
                offset = 120;
                denom = 330.0;
            } else if (interCluLightLv >= 450 && interCluLightLv < 1000) {
                idx_min = kParamArrayIndexThird;
                idx_max = kParamArrayIndexFourth;
                offset = 450;
                denom = 550.0;
            } else {
                // default value
            }
            interCluLightLvDiff = interCluLightLv - offset;
            privateTargetDutyValue =
                roundOffTo2DecimalPlaces(getUInt16ValueFromArrayAsHDoubleWithDiv100(paramDataArray, idx_min) +
                                         static_cast<HDouble>(interCluLightLvDiff) *
                                             (getUInt16ValueFromArrayAsHDoubleWithDiv100(paramDataArray, idx_max) -
                                              getUInt16ValueFromArrayAsHDoubleWithDiv100(paramDataArray, idx_min)) /
                                             denom);
        }
    }

    inline HDouble roundOffTo2DecimalPlaces(HDouble value) {
        return round(value * 100.0) / 100.0;
    }

    using ConfigAutolight = ArgumentsConfigAutolightChanged::Inter_ConfigAutolight;
    using ConfigVehicleType = ArgumentsMinLightModeChanged::Inter_ConfigVehicleType;
    using UtilityModeOnOffStatus = ArgumentsMinLightModeChanged::Input_UtilityModeOnOffStatus;
    using CampModeOnOffStatus = ArgumentsMinLightModeChanged::Input_CampModeOnOffStatus;
    using RheostatSwitch = ArgumentsRheostatSwitchChanged::Inter_RheostatSwitchStatus;
    using RheostatLevelSetReqStat = ArgumentsRheostatLevelSetReqStatChanged::Input_RheostatLevelSetReqStatus;
    using ExtLowBeamOnReqStatus = ArgumentsDayAndNightModeChanged::Input_ExtLowBeamOnReqStatus;
    using AvTailLampStatus = ArgumentsDayAndNightModeChanged::Input_AvTailLampStatus;
    using AutolightSensorNightStatus = ArgumentsDayAndNightModeChanged::Input_AutolightSensorNightStatus;
    using EventInfoDisplayStatus = ArgumentsEventInfoDisplayStatusChanged::Inter_EventInfoDisplayStatus;
    using IgOffDisplayOnStatus = ArgumentsIgOffDisplayOnStatusChanged::Inter_IgOffDisplayOnStatus;
    using NaviMapDayNightStatus = ArgumentsNaviMapDayNightStatusChanged::Input_NaviMapDayNightStatus;
    using ConfigAvnBrightLink = ArgumentsConfigAvnBrightLinkChanged::Inter_ConfigAvnBrightLink;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    SFCIlluminationInter_IllCluster mInter_IllCluster = SFCIlluminationInter_IllCluster::NIGHT;
    ConfigAutolight mConfigAutolight = ConfigAutolight::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    UtilityModeOnOffStatus mUtilityModeOnOffStatus = UtilityModeOnOffStatus::NON_OPTION;
    CampModeOnOffStatus mCampModeOnOffStatus = CampModeOnOffStatus::OFF;
    SFCIlluminationInter_MinLightMode mInter_MinLightMode = SFCIlluminationInter_MinLightMode::OFF;
    RheostatSwitch mRheostatSwitch = RheostatSwitch::DEFAULT;
    RheostatSwitch mRheostatSwitchPrevious = RheostatSwitch::DEFAULT;
    RheostatLevelSetReqStat mRheostLevelSetReqStat = RheostatLevelSetReqStat::DEFAULT;
    RheostatLevelSetReqStat mRheostLevelSetReqStatPrevious = RheostatLevelSetReqStat::DEFAULT;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    AvTailLampStatus mAvTailLampStatus = AvTailLampStatus::OFF;
    AutolightSensorNightStatus mAutolightSensorNightStatus = AutolightSensorNightStatus::OFF;
    EventInfoDisplayStatus mEventInfoDisplayStatus = EventInfoDisplayStatus::OFF;
    IgOffDisplayOnStatus mIgOffDisplayOnStatus = IgOffDisplayOnStatus::NONE;
    NaviMapDayNightStatus mNaviMapDayNightStatus = NaviMapDayNightStatus::INVALID;
    ConfigAvnBrightLink mConfigAvnBrightLink = ConfigAvnBrightLink::OFF;
    EPrivate_LcdLightingStatus mEPrivate_LcdLightingStatus = EPrivate_LcdLightingStatus::OFF;
    EPrivate_NaviMapStatus mEPrivate_NaviMapStatus = EPrivate_NaviMapStatus::NIGHT;
    EPrivate_NaviMapStatus mEPrivate_NaviMapStatusPrevious = EPrivate_NaviMapStatus::NONE;
    EPrivate_DialLightingStatus mEPrivate_DialLightingStatus = EPrivate_DialLightingStatus::OFF;
    EPrivate_RheostatSwitchOperation mEPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::NONE;
    EPrivate_RheostatSwitchOperation mEPrivate_RheostatSwitchOperationPrevious = EPrivate_RheostatSwitchOperation::NONE;

    HUInt64 mInter_RheostatLevelStatus = 12;
    HUInt64 mMemory_RheostatLevelStatus = 12;

    HUInt64 mInter_ClusterLightingLevel = 455;
    HUInt64 mAvnAutoBrightValue = 450;

    HUInt64 mPrivate_LightOutLevel = 0;
    HDouble mPrivate_DayDialDutySetValue = 0.;
    HDouble mPrivate_NightDialDutySetValue = 0.;
    HDouble mPrivate_DayLcdDutySetValue = 0.;
    HDouble mPrivate_NightLcdDutySetValue = 0.;
    HDouble mPrivate_DayPointerDutySetValue = 0.;
    HDouble mPrivate_NightPointerDutySetValue = 0.;
    HDouble mPrivate_DaySymbolGreenDutySetValue = 0.;
    HDouble mPrivate_NightSymbolGreenDutySetValue = 0.;
    HDouble mPrivate_DaySymbolWhiteDutySetValue = 0.;
    HDouble mPrivate_NightSymbolWhiteDutySetValue = 0.;

    static constexpr HUInt64 kMaxParamArraySize4 = 4;
    static constexpr HUInt64 kTransitionTimeSlotCount = 20;
    static constexpr HUInt64 kClusterLightingLevelCount = 20;
    static constexpr HUInt64 kParamArrayIndexFirst = 0;
    static constexpr HUInt64 kParamArrayIndexSecond = 1;
    static constexpr HUInt64 kParamArrayIndexThird = 2;
    static constexpr HUInt64 kParamArrayIndexFourth = 3;

    std::array<uint16_t, kMaxParamArraySize4> mIllumDayLcdParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumDayDialParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumDayPointerParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumDaySymbolGreenParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumDaySymbolWhiteParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumNightLcdParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumNightDialParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumNightPointerParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumNightSymbolGreenParamDataArray{};
    std::array<uint16_t, kMaxParamArraySize4> mIllumNightSymbolWhiteParamDataArray{};

    // EXNCP-711 : illumination_transition_coefficient.png 기반으로 작성됨
    std::array<HDouble, kTransitionTimeSlotCount> mTransitionTableLogArray = {0,     0.091, 0.174, 0.25,  0.32,  0.385, 0.446,
                                                                              0.503, 0.556, 0.607, 0.655, 0.7,   0.743, 0.785,
                                                                              0.824, 0.862, 0.899, 0.934, 0.967, 1};

    std::array<HDouble, kTransitionTimeSlotCount> mTransitionTableExpArray = {0,     0.014, 0.037, 0.066, 0.101, 0.141, 0.186,
                                                                              0.234, 0.286, 0.341, 0.398, 0.458, 0.52,  0.584,
                                                                              0.649, 0.717, 0.785, 0.856, 0.927, 1};

    std::array<uint16_t, kClusterLightingLevelCount> mClusterLightingLevelNightTableArray = {
        10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 100, 110, 120};

    std::array<uint16_t, kClusterLightingLevelCount> mClusterLightingLevelDuskTableArray = {
        120, 125, 130, 135, 145, 155, 165, 175, 190, 205, 220, 235, 255, 275, 295, 320, 345, 375, 410, 450};

    std::array<uint16_t, kClusterLightingLevelCount> mClusterLightingLevelDayTableArray = {
        455, 465, 480, 495, 510, 530, 550, 570, 590, 615, 640, 665, 695, 725, 760, 795, 835, 880, 935, 1000};

    HBool mTransitionDialStatus = false;
    HBool mTransitionPointerStatus = false;
    HDouble mTransitionDialStartValue = 0.;
    HDouble mTransitionDialDiffValue = 0.;
    HDouble mTransitionDialTargetValue = 0.;
    uint32_t mTransitionDialTimerRunningCount = 0;
    ssfs::SFCTimer<Illumination> mRepeatTimerTransitionLogStyleDialTimerEvery100ms;
    ssfs::SFCTimer<Illumination> mRepeatTimerTransitionExpStyleDialTimerEvery100ms;

    HDouble mTransitionPointerStartValue = 0.;
    HDouble mTransitionPointerDiffValue = 0.;
    HDouble mTransitionPointerTargetValue = 0.;
    uint32_t mTransitionPointerTimerRunningCount = 0;
    ssfs::SFCTimer<Illumination> mRepeatTimerTransitionLogStylePointerTimerEvery100ms;
    ssfs::SFCTimer<Illumination> mRepeatTimerTransitionExpStylePointerTimerEvery100ms;

    ssfs::SFCTimer<Illumination> mOneShotTimerIgnElapsedOn400ms;
    ssfs::SFCTimer<Illumination> mOneShotTimerPrivate_DialLightingStatusTurningOn2000ms;
    ssfs::SFCTimer<Illumination> mOneShotTimerPrivate_DialLightingStatusTurningOff2000ms;
    ssfs::SFCTimer<Illumination> mOneShotTimerPrivate_RheostatSwitchOperation500ms;
    ssfs::SFCTimer<Illumination> mOneShotTimerBlinkPrivate_RheostatSwitchOperation;

    ssfs::SFCTimer<Illumination> mOneShotTimerInter_EventInfoDisplayStatusOff5000ms;
    ssfs::SFCTimer<Illumination> mOneShotTimerPrivate_IgOffDisplayOnStatusOff5000ms;
    HBool mInter_EventInfoDisplayStatusOff5000ms = false;
    HBool mPrivate_IgOffDisplayOnStatusOff5000ms = false;

    static constexpr HUInt64 kRheostatLevelAutoOnMax = 20;
    static constexpr HUInt64 kRheostatLevelAutoOffMax = 21;
    static constexpr HUInt64 kRheostatLevelMin = 1;
    static constexpr HUInt64 kRheostatLevelConvertIndex = 1;
    static constexpr HUInt64 kCluLightLevelConvertIndex = 1;
    static constexpr HUInt64 kTimerPrivateDialLightingTurnOnInterval2s = 2000;
    static constexpr HUInt64 kTimerPrivateDialLightingTurnOffInterval2s = 2000;
    static constexpr HUInt64 kTimerPrivateRheostatSwitchOperationInterval500ms = 500;
    static constexpr HUInt64 kTimerTransitionInterval100ms = 100;
    static constexpr HUInt64 kTimerIgnElapsedOnInterval400ms = 400;
    static constexpr HUInt64 kTimerEventInfoDisplayStatusOffInterval5s = 5000;
    static constexpr HUInt64 kTimerPrivateIgOffDisplayOnStatusOffInterval5s = 5000;

    HBool mAEMInitializeComplete = false;

    // 4.3.2.1.2 스위치 동작 이벤트 정의 - Blink 처리 변수
    EBlinkStatePrivate_RheostatSwitchOperation mBlinkStatePrivate_RheostatSwitchOperation =
        EBlinkStatePrivate_RheostatSwitchOperation::NONE;
    static constexpr uint32_t kBlinkTimeAPrivate_RheostatSwitchOperation50ms = 50;
    static constexpr uint32_t kBlinkTimeBPrivate_RheostatSwitchOperation50ms = 50;
    EPrivate_RheostatSwitchOperation mBlinkValueAPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::NONE;
    EPrivate_RheostatSwitchOperation mBlinkValueBPrivate_RheostatSwitchOperation = EPrivate_RheostatSwitchOperation::NONE;
    HBool mForceUpdateGroup = false;

    HUInt64 mIMG_ClusterLightingLevel_Stat = 0;
    HBool mIsE00502PopupEventOn = false;
};

}  // namespace ccos

#endif  // SFSS_Illumination_H
