/**
 * @file ADAS_Driving_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_ADAS_Driving_CV_H
#define SFSS_ADAS_Driving_CV_H
#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "ADAS_Driving_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 10.0.0
// Spec Version : v0.36
// Reference : [CV730] ADAS_Driving_CV. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_Driving_CV : public ADAS_Driving_CVBase {
public:
    ADAS_Driving_CV() = default;
    ~ADAS_Driving_CV() override = default;
    ADAS_Driving_CV(const ADAS_Driving_CV& other) = delete;
    ADAS_Driving_CV(ADAS_Driving_CV&& other) noexcept = delete;
    ADAS_Driving_CV& operator=(const ADAS_Driving_CV& other) = delete;
    ADAS_Driving_CV& operator=(ADAS_Driving_CV&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1 주행 보조 요약 정보
        setSFCADAS_Driving_NewEventDrivingAssistSummaryID("E20207");

        // 5.3.6 7그룹 풀 팝업 – 운전자 보조 시스템 고장 1
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundType(
            SFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundRepeatCount(1);

        // 5.3.8 7그룹 풀 팝업 - 사이드 미러 고장
        setSFCADAS_Driving_NewEventOutsideMirrorSymbolFailureID("E77307");
        setSFCADAS_Driving_NewEventOutsideMirrorSymbolFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventOutsideMirrorSymbolFailureLinkedSoundType(
            SFCADAS_Driving_NewEventOutsideMirrorSymbolFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventOutsideMirrorSymbolFailureLinkedSoundRepeatCount(1);

        // 6.1 경고음 1-1
        setSFCADAS_Driving_NewSoundAdasWarning1_1Type(SFCADAS_Driving_NewSoundAdasWarning1_1Type::INFINITE);

        // 6.4 경고음 1-4
        setSFCADAS_Driving_NewSoundAdasWarning1_4ID("SND_ADAS_PopUpInform2_Fixed");
        setSFCADAS_Driving_NewSoundAdasWarning1_4Type(SFCADAS_Driving_NewSoundAdasWarning1_4Type::REPEAT_COUNT);
        setSFCADAS_Driving_NewSoundAdasWarning1_4RepeatCount(1);

        // 6.5 경고음 1-5
        setSFCADAS_Driving_NewSoundAdasWarning1_5ID("SND_ADAS_LKAWarn_Auto");
        setSFCADAS_Driving_NewSoundAdasWarning1_5Type(SFCADAS_Driving_NewSoundAdasWarning1_5Type::INFINITE);

        // 6.6 경고음 2-1
        setSFCADAS_Driving_NewSoundAdasWarning2_1ID("SND_ADAS_BCW_Auto");
        setSFCADAS_Driving_NewSoundAdasWarning2_1Type(SFCADAS_Driving_NewSoundAdasWarning2_1Type::INFINITE);

        if (mOneShotTimerPriorityISLA.create(kTimerInterval300ms, this, &ADAS_Driving_CV::onOneShotTimerISLAChanged, false) ==
            false) {
            DWarning() << "[ADAS_Driving_CV] mOneShotTimerPriorityISLA Create Failed";
        }
        if (mOneShotTimerPriorityDIS.create(kTimerInterval300ms, this, &ADAS_Driving_CV::onOneShotTimerDISChanged, false) ==
            false) {
            DWarning() << "[ADAS_Driving_CV] mOneShotTimerPriorityDIS Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;

        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;

        updateAll();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;

        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;

        updateAll();
    }

    void onConfigFca1Changed(const ArgumentsConfigFca1Changed& args) {
        mConfigAdasFCA = args.mInter_ConfigAdasFCA;
        updateTelltaleFCA();
        updateConstantForwardSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigLDWChanged(const ArgumentsConfigLDWChanged& args) {
        mConfigAdasLDW = args.mInter_ConfigAdasLDW;
        updateTelltaleLKA();
        updateConstantLaneSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigDawChanged(const ArgumentsConfigDawChanged& args) {
        mConfigAdasDAW = args.mInter_ConfigAdasDAW;
        updateTelltaleDAW();
        updateConstantDawFailure();
    }

    void onConfigFCWNearChanged(const ArgumentsConfigFCWNearChanged& args) {
        mConfigAdasFCWNear = args.mInter_ConfigAdasFCWNear;
        updateTelltaleFCWNearStat();
        updateConstantForwardSideSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigBCWNearChanged(const ArgumentsConfigBCWNearChanged& args) {
        mConfigAdasBCWNear = args.mInter_ConfigAdasBCWNear;
        updateTelltaleBCWNearStat();
        updateConstantBlindSpotSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateConstantISLAMainSpeedStat();
    }

    void onConfigIslaChanged(const ArgumentsConfigIslaChanged& args) {
        mConfigAdasISLA = args.mInter_ConfigAdasISLA;
        updateConstantISLAMainSpeedStat();
        updateConstantIslaFailure();
    }

    void onConfigAdasBcwChanged(const ArgumentsConfigAdasBcwChanged& args) {
        mConfigAdasBCW = args.mInter_ConfigAdasBCW;
        updateConstantBlindSpotSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigIFSCVChanged(const ArgumentsConfigIFSCVChanged& args) {
        mConfigIFSCV = args.mInter_ConfigIFSCV;
        updateTelltaleHBA();
    }

    void onConfigSCC2Changed(const ArgumentsConfigSCC2Changed& args) {
        mConfigSCC2 = args.mInter_ConfigSCC2;
        updateConstantEmergencyStopFailure();
    }

    void onConfigHbaChanged(const ArgumentsConfigHbaChanged& args) {
        mConfigNewHBA = args.mInter_ConfigNewHBA;
        updateTelltaleHBA();
        updateConstantHbaFailure();
    }

    void onConfigSccChanged(const ArgumentsConfigSccChanged& args) {
        mConfigAdasSCC = args.mInter_ConfigAdasSCC;
        updateConstantSccFailure();
    }

    void onConfigLfaChanged(const ArgumentsConfigLfaChanged& args) {
        mConfigAdasLFA = args.mInter_ConfigAdasLFA;
        updateConstantLfaFailure();
    }

    void onConfigHdaChanged(const ArgumentsConfigHdaChanged& args) {
        mConfigAdasHDA = args.mInter_ConfigAdasHDA;
        updateConstantHdaFailure();
    }

    void onInter_DISAddtnlSign(const ArgumentsInter_DISAddtnlSign& args) {
        mDISAddtnlSign = args.mInter_DISAddtnlSign;
        updateConstantFinalDISISLAAddtnlSignStat();
    }

    void onForwardSafetySymbolStatusChanged(const ArgumentsForwardSafetySymbolStatusChanged& args) {
        mForwardSafetySymbolStatus = args.mInput_ForwardSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_ForwardSafetySymbolStatus) == true) {
            mForwardSafetySymbolStatus = ForwardSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleFCA();
    }

    void onLaneSafetySymbolStatusChanged(const ArgumentsLaneSafetySymbolStatusChanged& args) {
        mLaneSafetySymbolStatus = args.mInput_LaneSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_LaneSafetySymbolStatus) == true) {
            mLaneSafetySymbolStatus = LaneSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleLKA();
    }

    void onDriveAttentionWarningSymbolStatusChanged(const ArgumentsDriveAttentionWarningSymbolStatusChanged& args) {
        mDriveAttentionWarningSymbolStatus = args.mInput_DriveAttentionWarningSymbolStatus;
        if (ISTIMEOUT(args.mInput_DriveAttentionWarningSymbolStatus) == true) {
            mDriveAttentionWarningSymbolStatus = DriveAttentionWarningSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleDAW();
    }

    void onIfsIndLampStatusChanged(const ArgumentsIfsIndLampStatusChanged& args) {
        mIfsIndLampStatus = args.mInput_IFSIndLampStatus;
        updateTelltaleHBA();
    }

    void onHbaSymbolStatusChanged(const ArgumentsHbaSymbolStatusChanged& args) {
        mHbaSymbolStatus = args.mInput_HbaSymbolStatus;
        updateTelltaleHBA();
    }

    void onSmvFrontObjectStatusChanged(const ArgumentsSmvFrontObjectStatusChanged& args) {
        mSmvFrontObjectStatus = args.mInput_SmvFrontObjectStatus;
        updateConstantFrontVehicleColorStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvVehicleDistanceLevelValueChanged(const ArgumentsSmvVehicleDistanceLevelValueChanged& args) {
        mSmvVehicleDistanceLevelValue = args.mInput_SmvVehicleDistanceLevelValue;
        updateConstantFrontVehicleLevelValue();
        updateConstantHeadywayDistanceLevelValue();
        updateEventDrivingAssistSummary();
    }

    void onSmvVehicleDistanceLevelStatusChanged(const ArgumentsSmvVehicleDistanceLevelStatusChanged& args) {
        mSmvVehicleDistanceLevelStatus = args.mInput_SmvVehicleDistanceLevelStatus;
        updateConstantHeadywayDistanceColorStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvHostVehicleStatusChanged(const ArgumentsSmvHostVehicleStatusChanged& args) {
        mSmvHostVehicleStatus = args.mInput_SmvHostVehicleStatus;
        updateConstantSubVehicleColorStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvSetSpeedStatusChanged(const ArgumentsSmvSetSpeedStatusChanged& args) {
        mSmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        updateConstantSetSpeedColorStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvSetSpeedValueChanged(const ArgumentsSmvSetSpeedValueChanged& args) {
        mSmvSetSpeedValue = args.mInput_SmvSetSpeedValue;
        updateConstantSetSpeedStat();
    }

    void onSmvHdaSymbolStatusChanged(const ArgumentsSmvHdaSymbolStatusChanged& args) {
        mSmvHdaSymbolStatus = args.mInput_SmvHdaSymbolStatus;
        updateConstantHDAHDPStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvIslaSetSpeedSymbolStatusChanged(const ArgumentsSmvIslaSetSpeedSymbolStatusChanged& args) {
        mSmvIslaSetSpeedSymbolStatus = args.mInput_SmvIslaSetSpeedSymbolStatus;
        updateConstantISLAManualSetSpeedStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvNsccSymbolStatusChanged(const ArgumentsSmvNsccSymbolStatusChanged& args) {
        mSmvNsccSymbolStatus = args.mInput_SmvNsccSymbolStatus;
        updateConstantNSCCAutoSetStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvLfaSymbolStatusChanged(const ArgumentsSmvLfaSymbolStatusChanged& args) {
        mSmvLfaSymbolStatus = args.mInput_SmvLfaSymbolStatus;
        updateTelltaleLFAStat();
    }

    void onFcwNearSymbolStatusChanged(const ArgumentsFcwNearSymbolStatusChanged& args) {
        mFCWNearSymbolStatus = args.mInput_FCWNearSymbolStatus;
        if (ISTIMEOUT(args.mInput_FCWNearSymbolStatus) == true) {
            mFCWNearSymbolStatus = FCWNearSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleFCWNearStat();
    }

    void onBlindSpotSafetySymbolStatusChanged(const ArgumentsBlindSpotSafetySymbolStatusChanged& args) {
        mBlindSpotSafetySymbolStatus = args.mInput_BlindSpotSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_BlindSpotSafetySymbolStatus) == true) {
            mBlindSpotSafetySymbolStatus = BlindSpotSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleBCWNearStat();
    }

    void onMvDrivingLaneCenterLineStatusChanged(const ArgumentsMvDrivingLaneCenterLineStatusChanged& args) {
        mMvDrivingLaneCenterLineStatus = args.mInput_MvDrivingLaneCenterLineStatus;
        updateConstantLaneCentralLineStat();
    }

    void onMvLeftLineStatusChanged(const ArgumentsMvLeftLineStatusChanged& args) {
        mMvLeftLineStatus = args.mInput_MvLeftLineStatus;
        if (ISTIMEOUT(args.mInput_MvLeftLineStatus) == true) {
            mMvLeftLineStatus = MvLeftLineStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLeftLaneColorStat();
    }

    void onMvLeftLineOffsetValueChanged(const ArgumentsMvLeftLineOffsetValueChanged& args) {
        mMvLeftLineOffsetValue = args.mInput_MvLeftLineOffsetValue;
        if (ISTIMEOUT(args.mInput_MvLeftLineOffsetValue) == true) {
            mMvLeftLineOffsetValue = kTimeOut;
        }
        updateConstantLeftLineLatPosStat();
    }

    void onMvRightLineStatusChanged(const ArgumentsMvRightLineStatusChanged& args) {
        mMvRightLineStatus = args.mInput_MvRightLineStatus;
        if (ISTIMEOUT(args.mInput_MvRightLineStatus) == true) {
            mMvRightLineStatus = MvRightLineStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantRightLaneColorStat();
    }

    void onMvRightLineOffsetValueChanged(const ArgumentsMvRightLineOffsetValueChanged& args) {
        mMvRightLineOffsetValue = args.mInput_MvRightLineOffsetValue;
        if (ISTIMEOUT(args.mInput_MvRightLineOffsetValue) == true) {
            mMvRightLineOffsetValue = kTimeOut;
        }
        updateConstantRightLineLatPosStat();
    }

    void onMvVehicleDistanceStatusChanged(const ArgumentsMvVehicleDistanceStatusChanged& args) {
        mMvVehicleDistanceStatus = args.mInput_MvVehicleDistanceStatus;
        updateConstantHeadwayDistColorStat();
    }

    void onMvVehicleDistanceValueChanged(const ArgumentsMvVehicleDistanceValueChanged& args) {
        mMvVehicleDistanceValue = args.mInput_MvVehicleDistanceValue;
        updateConstantHeadwayDistLongPosStat();
    }

    void onMvFrontObjectStatusChanged(const ArgumentsMvFrontObjectStatusChanged& args) {
        mMvFrontObjectStatus = args.mInput_MvFrontObjectStatus;
        updateConstantViewFrontVehicleColorStat();
    }

    void onMvFrontObjectLateralPositionValueChanged(const ArgumentsMvFrontObjectLateralPositionValueChanged& args) {
        mMvFrontObjectLateralPositionValue = args.mInput_MvFrontObjectLateralPositionValue;
        updateConstantViewFrontVehicleLatPosStat();
    }

    void onMvFrontObjectLongitudinalPositionValueChanged(const ArgumentsMvFrontObjectLongitudinalPositionValueChanged& args) {
        mMvFrontObjectLongitudinalPositionValue = args.mInput_MvFrontObjectLongitudinalPositionValue;
        updateConstantViewFrontVehicleLongPosStat();
    }

    void onMvRearLeftRadarWaveStatusChanged(const ArgumentsMvRearLeftRadarWaveStatusChanged& args) {
        mMvRearLeftRadarWave2Status = args.mInput_MvRearLeftRadarWave2Status;
        updateConstantViewRearLeftWarnStat();
    }

    void onMvRearRightRadarWaveStatusChanged(const ArgumentsMvRearRightRadarWaveStatusChanged& args) {
        mMvRearRightRadarWave2Status = args.mInput_MvRearRightRadarWave2Status;
        updateConstantViewRearRightWarnStat();
    }

    void onIslaSpeedLimitTrafficSignStatusChanged(const ArgumentsIslaSpeedLimitTrafficSignStatusChanged& args) {
        mIslaSpeedLimitTrafficSignStatus = args.mInput_IslaSpeedLimitTrafficSignStatus;
        if (ISTIMEOUT(args.mInput_IslaSpeedLimitTrafficSignStatus) == true) {
            mIslaSpeedLimitTrafficSignStatus = IslaSpeedLimitTrafficSignStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantISLAMainSpeedStat();
    }

    void onIslaSpeedLimitTrafficSignValueChanged(const ArgumentsIslaSpeedLimitTrafficSignValueChanged& args) {
        mIslaSpeedLimitTrafficSignValue = args.mInput_IslaSpeedLimitTrafficSignValue;
        updateConstantISLAMainSpeedStat();
        updateConstantISLAMainSignStat();
    }

    void onIslaAdditionalTrafficSignStatusChanged(const ArgumentsIslaAdditionalTrafficSignStatusChanged& args) {
        mIslaAdditionalTrafficSignStatus = args.mInput_IslaAdditionalTrafficSignStatus;
        updatePrivateISLAAddtnlSign();
    }

    void onIslaSupplementaryTrafficSignStatusChanged(const ArgumentsIslaSupplementaryTrafficSignStatusChanged& args) {
        mIslaSupplementaryTrafficSignStatus = args.mInput_IslaSupplementaryTrafficSignStatus;
        updateConstantISLAAddSignStat();
    }

    void onPuFGroup1AdasWarning1_1StatusChanged(const ArgumentsPuFGroup1AdasWarning1_1StatusChanged& args) {
        mPuFGroup1AdasWarning1_1Status = args.mInput_PuFGroup1AdasWarning1_1Status;
        updateEventGroup1FullPopup1_1();
    }

    void onPuFGroup1AdasWarning1_2StatusChanged(const ArgumentsPuFGroup1AdasWarning1_2StatusChanged& args) {
        mPuFGroup1AdasWarning1_2Status = args.mInput_PuFGroup1AdasWarning1_2Status;
        updateEventGroup1FullPopup1_2();
    }

    void onPuFGroup1AdasWarningA_AStatusChanged(const ArgumentsPuFGroup1AdasWarningA_AStatusChanged& args) {
        mPuFGroup1AdasWarningA_AStatus = args.mInput_PuFGroup1AdasWarningA_AStatus;
        updateEventGroup1FullPopupA_A();
    }

    void onPuFGroup4AdasWarning1_1StatusChanged(const ArgumentsPuFGroup4AdasWarning1_1StatusChanged& args) {
        mPuFGroup4AdasWarning1_1Status = args.mInput_PuFGroup4AdasWarning1_1Status;
        updateEventGroup4FullPopup1_1();
    }

    void onPuFGroup7BlindSpotSafetyFailureStatusChanged(const ArgumentsPuFGroup7BlindSpotSafetyFailureStatusChanged& args) {
        mPuFGroup7BlindSpotSafetyFailureStatus = args.mInput_PuFGroup7BlindSpotSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7BlindSpotSafetyFailureStatus) == true) {
            mPuFGroup7BlindSpotSafetyFailureStatus = PuFGroup7BlindSpotSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantBlindSpotSafetyFailure();
    }

    void onPuMGroup2AdasWarning1_1StatusChanged(const ArgumentsPuMGroup2AdasWarning1_1StatusChanged& args) {
        mPuMGroup2AdasWarning1_1Status = args.mInput_PuMGroup2AdasWarning1_1Status;
        updateEventGroup2MiniPopup1_1();
    }

    void onIslaTrafficSignCountryInformationStatusChanged(const ArgumentsIslaTrafficSignCountryInformationStatusChanged& args) {
        mIslaTrafficSignCountryInformationStatus = args.mInput_IslaTrafficSignCountryInformationStatus;
        updateConstantISLAMainSpeedStat();
        updatePrivateISLAAddtnlSign();
    }

    void onPuFGroup7DriverAssistFailure1StatusChanged(const ArgumentsPuFGroup7DriverAssistFailure1StatusChanged& args) {
        mPuFGroup7DriverAssistFailure1Status = args.mInput_PuFGroup7DriverAssistFailure1Status;
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure1Status) == true) {
            mPuFGroup7DriverAssistFailure1Status = PuFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopup1();
    }

    void onPuFGroup7OutsideMirrorSymbolFailureStatusChanged(
        const ArgumentsPuFGroup7OutsideMirrorSymbolFailureStatusChanged& args) {
        mPuFGroup7OutsideMirrorSymbolFailureStatus = args.mInput_PuFGroup7OutsideMirrorSymbolFailureStatus;
        updateEventGroup7OutsideMirrorSymbolFailure();
    }

    void onIslaWarningSoundSymbolStatusChanged(const ArgumentsIslaWarningSoundSymbolStatusChanged& args) {
        mIslaWarningSoundSymbolStatus = args.mInput_IslaWarningSoundSymbolStatus;
        updateConstantISLASoundSymbolStat();
    }

    void onSoundAdasWarning1_1StatusChanged(const ArgumentsSoundAdasWarning1_1StatusChanged& args) {
        mSoundAdasWarning1_1Status = args.mInput_SoundAdasWarning1_1Status;
        updateSoundAdasWarning1_1Status();
    }

    void onSoundAdasWarning1_2StatusChanged(const ArgumentsSoundAdasWarning1_2StatusChanged& args) {
        mSoundAdasWarning1_2Status = args.mInput_SoundAdasWarning1_2Status;
        updateSoundAdasWarning1_2Status();
    }

    void onSoundAdasWarning1_3StatusChanged(const ArgumentsSoundAdasWarning1_3StatusChanged& args) {
        mSoundAdasWarning1_3Status = args.mInput_SoundAdasWarning1_3Status;
        updateSoundAdasWarning1_3Status();
    }

    void onSoundAdasWarning1_4StatusChanged(const ArgumentsSoundAdasWarning1_4StatusChanged& args) {
        mSoundAdasWarning1_4Status = args.mInput_SoundAdasWarning1_4Status;
        updateSoundAdasWarning1_4Status();
    }

    void onSoundAdasWarning1_5StatusChanged(const ArgumentsSoundAdasWarning1_5StatusChanged& args) {
        mSoundAdasWarning1_5Status = args.mInput_SoundAdasWarning1_5Status;
        updateSoundAdasWarning1_5Status();
    }

    void onSoundAdasWarning2_1StatusChanged(const ArgumentsSoundAdasWarning2_1StatusChanged& args) {
        mSoundAdasWarning2_1Status = args.mInput_SoundAdasWarning2_1Status;
        updateSoundAdasWarning2_1Status();
    }

    void onPuFGroup7ForwardSafetyFailureStatusChanged(const ArgumentsPuFGroup7ForwardSafetyFailureStatusChanged& args) {
        mPuFGroup7ForwardSafetyFailureStatus = args.mInput_PuFGroup7ForwardSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7ForwardSafetyFailureStatus) == true) {
            mPuFGroup7ForwardSafetyFailureStatus = PuFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantForwardSafetyFailure();
    }

    void onPuFGroup7FCWNearFailureStatusChanged(const ArgumentsPuFGroup7FCWNearFailureStatusChanged& args) {
        mPuFGroup7FCWNearFailureStatus = args.mInput_PuFGroup7FCWNearFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7FCWNearFailureStatus) == true) {
            mPuFGroup7FCWNearFailureStatus = PuFGroup7FCWNearFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantForwardSideSafetyFailure();
    }

    void onPuFGroup7LaneSafetyFailureStatusChanged(const ArgumentsPuFGroup7LaneSafetyFailureStatusChanged& args) {
        mPuFGroup7LaneSafetyFailureStatus = args.mInput_PuFGroup7LaneSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7LaneSafetyFailureStatus) == true) {
            mPuFGroup7LaneSafetyFailureStatus = PuFGroup7LaneSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLaneSafetyFailure();
    }

    void onPuFGroup7IslaFailureStatusChanged(const ArgumentsPuFGroup7IslaFailureStatusChanged& args) {
        mPuFGroup7IslaFailureStatus = args.mInput_PuFGroup7IslaFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7IslaFailureStatus) == true) {
            mPuFGroup7IslaFailureStatus = PuFGroup7IslaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantIslaFailure();
    }

    void onPuFGroup7DawFailureStatusChanged(const ArgumentsPuFGroup7DawFailureStatusChanged& args) {
        mPuFGroup7DawFailureStatus = args.mInput_PuFGroup7DawFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7DawFailureStatus) == true) {
            mPuFGroup7DawFailureStatus = PuFGroup7DawFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantDawFailure();
    }

    void onPuFGroup7HbaFailureStatusChanged(const ArgumentsPuFGroup7HbaFailureStatusChanged& args) {
        mPuFGroup7HbaFailureStatus = args.mInput_PuFGroup7HbaFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7HbaFailureStatus) == true) {
            mPuFGroup7HbaFailureStatus = PuFGroup7HbaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHbaFailure();
    }

    void onPuFGroup7SccFailureStatusChanged(const ArgumentsPuFGroup7SccFailureStatusChanged& args) {
        mPuFGroup7SccFailureStatus = args.mInput_PuFGroup7SccFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7SccFailureStatus) == true) {
            mPuFGroup7SccFailureStatus = PuFGroup7SccFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantSccFailure();
    }

    void onPuFGroup7LfaFailureStatusChanged(const ArgumentsPuFGroup7LfaFailureStatusChanged& args) {
        mPuFGroup7LfaFailureStatus = args.mInput_PuFGroup7LfaFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7LfaFailureStatus) == true) {
            mPuFGroup7LfaFailureStatus = PuFGroup7LfaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLfaFailure();
    }

    void onPuFGroup7HdaFailureStatusChanged(const ArgumentsPuFGroup7HdaFailureStatusChanged& args) {
        mPuFGroup7HdaFailureStatus = args.mInput_PuFGroup7HdaFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7HdaFailureStatus) == true) {
            mPuFGroup7HdaFailureStatus = PuFGroup7HdaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHdaFailure();
    }

    void onPuFGroup7MRMFailureStatusChanged(const ArgumentsPuFGroup7MRMFailureStatusChanged& args) {
        mPuFGroup7MRMFailureStatus = args.mInput_PuFGroup7MRMFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7MRMFailureStatus) == true) {
            mPuFGroup7MRMFailureStatus = PuFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantEmergencyStopFailure();
    }

    void onInterTelltaleIccDrowChanged(const ArgumentsInterTelltaleIccDrowChanged& args) {
        mInterTelltaleIccDrow = args.mInter_TelltaleIccDrow;
        updateTelltaleDAW();
    }

    void onMvFrontLeftRadarWaveStatusChanged(const ArgumentsMvFrontLeftRadarWaveStatusChanged& args) {
        mMvFrontLeftRadarWaveStatus = args.mInput_MvFrontLeftRadarWaveStatus;
        updateConstantViewFrontLeftWarnStat();
    }

    void onMvFrontRightRadarWaveStatusChanged(const ArgumentsMvFrontRightRadarWaveStatusChanged& args) {
        mMvFrontRightRadarWaveStatus = args.mInput_MvFrontRightRadarWaveStatus;
        updateConstantViewFrontRightWarnStat();
    }

    void onMvFrontRadarWaveStatusChanged(const ArgumentsMvFrontRadarWaveStatusChanged& args) {
        mMvFrontRadarWaveStatus = args.mInput_MvFrontRadarWaveStatus_CV;
        updateConstantViewFrontWarnStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateAllConstant();
        updateAllTelltale();
        updateAllEvent();
        updateAllSound();
    }

    void updateAllConstant() {
        updateConstantFrontVehicleColorStat();
        updateConstantFrontVehicleLevelValue();
        updateConstantHeadywayDistanceColorStat();
        updateConstantHeadywayDistanceLevelValue();
        updateConstantSubVehicleColorStat();
        updateConstantSetSpeedColorStat();
        updateConstantSetSpeedStat();
        updateConstantHDAHDPStat();
        updateConstantISLAManualSetSpeedStat();
        updateConstantNSCCAutoSetStat();
        updateConstantLaneCentralLineStat();
        updateConstantLeftLaneColorStat();
        updateConstantLeftLineLatPosStat();
        updateConstantRightLaneColorStat();
        updateConstantRightLineLatPosStat();
        updateConstantHeadwayDistColorStat();
        updateConstantHeadwayDistLongPosStat();
        updateConstantViewFrontVehicleColorStat();
        updateConstantViewFrontVehicleLatPosStat();
        updateConstantViewFrontVehicleLongPosStat();
        updateConstantViewRearLeftWarnStat();
        updateConstantViewRearRightWarnStat();
        updateConstantViewFrontLeftWarnStat();
        updateConstantViewFrontRightWarnStat();
        updateConstantViewFrontWarnStat();
        updateConstantISLAMainSpeedStat();
        updateConstantISLAMainSignStat();
        updatePrivateISLAAddtnlSign();
        updateConstantFinalDISISLAAddtnlSignStat();
        updateEventDrivingAssistSummary();
        updateConstantISLAAddSignStat();
        updateConstantISLASoundSymbolStat();
        updateConstantForwardSafetyFailure();
        updateConstantForwardSideSafetyFailure();
        updateConstantLaneSafetyFailure();
        updateConstantBlindSpotSafetyFailure();
        updateConstantIslaFailure();
        updateConstantDawFailure();
        updateConstantHbaFailure();
        updateConstantSccFailure();
        updateConstantLfaFailure();
        updateConstantHdaFailure();
        updateConstantEmergencyStopFailure();
    }

    void updateAllTelltale() {
        updateTelltaleFCA();
        updateTelltaleLKA();
        updateTelltaleDAW();
        updateTelltaleHBA();
        updateTelltaleLFAStat();
        updateTelltaleFCWNearStat();
        updateTelltaleBCWNearStat();
    }

    void updateAllEvent() {
        updateEventDrivingAssistSummary();
        updateEventGroup1FullPopup1_1();
        updateEventGroup1FullPopup1_2();
        updateEventGroup1FullPopupA_A();
        updateEventGroup4FullPopup1_1();
        updateEventGroup7FullPopup1();
        updateEventGroup2MiniPopup1_1();
        updateEventGroup7OutsideMirrorSymbolFailure();
    }

    void updateAllSound() {
        updateSoundAdasWarning1_1Status();
        updateSoundAdasWarning1_2Status();
        updateSoundAdasWarning1_3Status();
        updateSoundAdasWarning1_4Status();
        updateSoundAdasWarning1_5Status();
        updateSoundAdasWarning2_1Status();
    }

    // Telltale
    // 5.1.1 FCA
    void updateTelltaleFCA() {
        SFCADAS_Driving_NewTelltaleFCAStat fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::AMBER ||
                (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasFCA == ConfigAdasFCA::ON)) {
                fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::AMBER;
            } else if (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::RED) {
                fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::RED;
            } else {
                // no operation
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasFCA == ConfigAdasFCA::ON) {
            fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::AMBER;
        } else {
            // no operation
        }

        setSFCADAS_Driving_NewTelltaleFCAStat(fcaStat);
    }

    // 5.1.2 LDW
    void updateTelltaleLKA() {
        SFCADAS_Driving_NewTelltaleLKAStat lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::GRAY) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::GRAY;
            } else if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::GREEN) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::GREEN;
            } else if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::AMBER ||
                       (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                        mConfigAdasLDW == ConfigAdasLDW::ON)) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::AMBER;
            } else {
                // no operation
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasLDW == ConfigAdasLDW::ON) {
            lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::AMBER;
        } else {
            // no operation
        }

        setSFCADAS_Driving_NewTelltaleLKAStat(lkaStat);
    }

    // 5.1.3 DAW
    void updateTelltaleDAW() {
        SFCADAS_Driving_NewTelltaleDAWStat dawStat = SFCADAS_Driving_NewTelltaleDAWStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasDAW == ConfigAdasDAW::ON) {
            dawStat = SFCADAS_Driving_NewTelltaleDAWStat::AMBER;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mDriveAttentionWarningSymbolStatus == DriveAttentionWarningSymbolStatus::UNHANDLED_TIMEOUT &&
                mConfigAdasDAW == ConfigAdasDAW::ON) {
                dawStat = SFCADAS_Driving_NewTelltaleDAWStat::AMBER;
            } else if (mDriveAttentionWarningSymbolStatus == DriveAttentionWarningSymbolStatus::AMBER) {
                dawStat = SFCADAS_Driving_NewTelltaleDAWStat::AMBER;
            } else if (mInterTelltaleIccDrow == InterTelltaleIccDrow::ON) {
                dawStat = SFCADAS_Driving_NewTelltaleDAWStat::AMBER;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        setSFCADAS_Driving_NewTelltaleDAWStat(dawStat);
    }

    // 5.1.4 HBA
    void updateTelltaleHBA() {
        SFCADAS_Driving_NewTelltaleHBAStat hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigIFSCV == ConfigIFSCV::ON) {
                if (mIfsIndLampStatus == IfsIndLampStatus::ON) {
                    hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::GREEN;
                } else if (mIfsIndLampStatus == IfsIndLampStatus::STANDBY) {
                    hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::WHITE;
                } else {
                    // no operation
                }
            } else if (mConfigNewHBA == ConfigNewHBA::ON) {
                if (mHbaSymbolStatus == HbaSymbolStatus::GREEN) {
                    hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::GREEN;
                } else if (mHbaSymbolStatus == HbaSymbolStatus::GRAY_WHITE) {
                    hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::WHITE;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewTelltaleHBAStat(hbaStat);
    }

    // 5.1.5 LFA
    void updateTelltaleLFAStat() {
        SFCADAS_Driving_NewTelltaleLFAStat stat = SFCADAS_Driving_NewTelltaleLFAStat::OFF;
        if (mIsIgnOn == true) {
            switch (mSmvLfaSymbolStatus) {
                case SmvLfaSymbolStatus::GRAY:
                    stat = SFCADAS_Driving_NewTelltaleLFAStat::GRAY;
                    break;
                case SmvLfaSymbolStatus::GREEN:
                    stat = SFCADAS_Driving_NewTelltaleLFAStat::GREEN;
                    break;
                case SmvLfaSymbolStatus::WHITE:
                    stat = SFCADAS_Driving_NewTelltaleLFAStat::WHITE;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCADAS_Driving_NewTelltaleLFAStat(stat);
    }

    // 5.1.6 FCW-Near
    void updateTelltaleFCWNearStat() {
        SFCADAS_Driving_NewTelltaleFCWNearStat fcwNearStat = SFCADAS_Driving_NewTelltaleFCWNearStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mFCWNearSymbolStatus == FCWNearSymbolStatus::AMBER ||
                (mFCWNearSymbolStatus == FCWNearSymbolStatus::UNHANDLED_TIMEOUT && mConfigAdasFCWNear == ConfigAdasFCWNear::ON)) {
                fcwNearStat = SFCADAS_Driving_NewTelltaleFCWNearStat::AMBER;
            } else if (mFCWNearSymbolStatus == FCWNearSymbolStatus::RED) {
                fcwNearStat = SFCADAS_Driving_NewTelltaleFCWNearStat::RED;
            } else {
                // no operation
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasFCWNear == ConfigAdasFCWNear::ON) {
            fcwNearStat = SFCADAS_Driving_NewTelltaleFCWNearStat::AMBER;
        } else {
            // no operation
        }

        setSFCADAS_Driving_NewTelltaleFCWNearStat(fcwNearStat);
    }

    // 5.1.7 BCW-Near
    void updateTelltaleBCWNearStat() {
        SFCADAS_Driving_NewTelltaleBCWNearStat bcwNearStat = SFCADAS_Driving_NewTelltaleBCWNearStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mBlindSpotSafetySymbolStatus == BlindSpotSafetySymbolStatus::AMBER ||
                (mBlindSpotSafetySymbolStatus == BlindSpotSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasBCWNear == ConfigAdasBCWNear::ON)) {
                bcwNearStat = SFCADAS_Driving_NewTelltaleBCWNearStat::AMBER;
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasBCWNear == ConfigAdasBCWNear::ON) {
            bcwNearStat = SFCADAS_Driving_NewTelltaleBCWNearStat::AMBER;
        } else {
            // no operation
        }

        setSFCADAS_Driving_NewTelltaleBCWNearStat(bcwNearStat);
    }

    // Constant
    // 5.2.1 주행 보조 요약
    // 5.2.1.1 전방 물체 사이드 뷰 (1)
    // 5.2.1.1.1 칼라
    void updateConstantFrontVehicleColorStat() {
        SFCADAS_Driving_NewConstantFrontVehicleColorStat stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvFrontObjectStatus == SmvFrontObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::DARK;
            } else if (mSmvFrontObjectStatus == SmvFrontObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::BRIGHT;
            } else {
                // no operation
            }
        }

        mSummaryFrontVehicleStat = stat;
        setSFCADAS_Driving_NewConstantFrontVehicleColorStat(stat);
    }

    // 5.2.1.1.2 위치
    void updateConstantFrontVehicleLevelValue() {
        SFCADAS_Driving_NewConstantFrontVehicleLevelValue stat = SFCADAS_Driving_NewConstantFrontVehicleLevelValue::OFF;
        if (mIsIgnOn == true) {
            if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL1) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleLevelValue::LEVEL1;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL2) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleLevelValue::LEVEL2;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL3) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleLevelValue::LEVEL3;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL4 ||
                       mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL4_SOLID) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleLevelValue::LEVEL4;
            } else {
                // no operation
            }
        }

        mSummaryFrontVehicleValue = stat;
        setSFCADAS_Driving_NewConstantFrontVehicleLevelValue(stat);
    }

    // 5.2.1.2 차간 거리 단계 (2)
    // 5.2.1.2.1 칼라
    void updateConstantHeadywayDistanceColorStat() {
        SFCADAS_Driving_NewConstantHeadwayDistanceColorStat stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::OFF;
        if (mIsIgnOn == true && mSmvVehicleDistanceLevelStatus == SmvVehicleDistanceLevelStatus::BRAND) {
            stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::BRAND;
        }

        mSummaryHeadwayDistanceSetStat = stat;
        setSFCADAS_Driving_NewConstantHeadwayDistanceColorStat(stat);
    }

    // 5.2.1.2.2 단계
    void updateConstantHeadywayDistanceLevelValue() {
        SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::OFF;
        if (mIsIgnOn == true) {
            if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL1) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::LEVEL1;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL2) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::LEVEL2;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL3) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::LEVEL3;
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL4) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::LEVEL4;
            } else {
                // no operation
            }
        }

        mSummaryHeadwayDistanceSetVal = stat;
        setSFCADAS_Driving_NewConstantHeadwayDistanceLevelValue(stat);
    }

    // 5.2.1.3 자차 사이드 뷰 (3)
    void updateConstantSubVehicleColorStat() {
        SFCADAS_Driving_NewConstantSubVehicleColorStat stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvHostVehicleStatus == SmvHostVehicleStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::DARK;
            } else if (mSmvHostVehicleStatus == SmvHostVehicleStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::BRIGHT;
            } else {
                // no operation
            }
        }

        mSummarySubVehicleStat = stat;
        setSFCADAS_Driving_NewConstantSubVehicleColorStat(stat);
    }

    // 5.2.1.4 설정 속도 및 속도 단위 (4)
    // 5.2.1.4.1 칼라
    void updateConstantSetSpeedColorStat() {
        SFCADAS_Driving_NewConstantSetSpeedColorStat stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::GRAY;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::GREEN;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::WHITE;
            } else {
                // no operation
            }
        }

        mSummarySetSpeedStat = stat;
        setSFCADAS_Driving_NewConstantSetSpeedColorStat(stat);
    }

    // 5.2.1.4.2 속도값
    void updateConstantSetSpeedStat() {
        SFCADAS_Driving_NewConstantSetSpeedStat stat = SFCADAS_Driving_NewConstantSetSpeedStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) {
                stat = SFCADAS_Driving_NewConstantSetSpeedStat::VALUE;
                value = mSmvSetSpeedValue;
            }
        }

        setSFCADAS_Driving_NewConstantSetSpeedStat(stat);
        setSFCADAS_Driving_NewConstantSetSpeedValue(value);
    }

    // 5.2.1.5 고속도로 주행 보조 및 고속도로 자율주행 심볼 (5)
    void updateConstantHDAHDPStat() {
        SFCADAS_Driving_NewConstantHDAHDPStat stat = SFCADAS_Driving_NewConstantHDAHDPStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::GRAY;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::GREEN;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::WHITE;
            } else {
                // no operation
            }
        }

        mSummaryHDAHDPTelltaleStat = stat;
        setSFCADAS_Driving_NewConstantHDAHDPStat(stat);
    }

    // 5.2.1.6 설정속도 조절 심볼 (6)
    void updateConstantISLAManualSetSpeedStat() {
        SFCADAS_Driving_NewConstantISLAManualSetSpeedStat stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_GREEN) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::PLUS_GREEN;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_GREEN) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::MINUS_GREEN;
            } else {
                // no operation
            }
        }
        mSummaryISLAManualSetSpeedStat = stat;
        setSFCADAS_Driving_NewConstantISLAManualSetSpeedStat(stat);
    }

    // 5.2.1.7 자동 속도 조절 심볼 (7)
    void updateConstantNSCCAutoSetStat() {
        SFCADAS_Driving_NewConstantNSCCAutoSetStat stat = SFCADAS_Driving_NewConstantNSCCAutoSetStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantNSCCAutoSetStat::GRAY;
            } else if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantNSCCAutoSetStat::GREEN;
            } else if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantNSCCAutoSetStat::WHITE;
            } else {
                // no operation
            }
        }
        mSummaryNSCCAutoSetTelltaleStat = stat;
        setSFCADAS_Driving_NewConstantNSCCAutoSetStat(stat);
    }

    // 5.2.2 주행 보조 뷰
    // 5.2.2.1 차로 중앙 실선 (1)
    void updateConstantLaneCentralLineStat() {
        SFCADAS_Driving_NewConstantLaneCentralLineStat stat = SFCADAS_Driving_NewConstantLaneCentralLineStat::OFF;
        if (mIsIgnOn == true && mMvDrivingLaneCenterLineStatus == MvDrivingLaneCenterLineStatus::GREEN) {
            stat = SFCADAS_Driving_NewConstantLaneCentralLineStat::GREEN;
        }

        setSFCADAS_Driving_NewConstantLaneCentralLineStat(stat);
    }

    // 5.2.2.2 차선-왼쪽 (2)
    // 5.2.2.2.1 칼라
    void updateConstantLeftLaneColorStat() {
        SFCADAS_Driving_NewConstantLeftLaneColorStat stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvLeftLineStatus == MvLeftLineStatus::GRAY || mMvLeftLineStatus == MvLeftLineStatus::UNHANDLED_TIMEOUT) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::GRAY;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::WHITE;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::AMBER) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::AMBER;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantLeftLaneColorStat(stat);
    }

    // 5.2.2.2.2 횡위치
    void updateConstantLeftLineLatPosStat() {
        SFCADAS_Driving_NewConstantLeftLineLatPosStat stat = SFCADAS_Driving_NewConstantLeftLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvLeftLineOffsetValue && mMvLeftLineOffsetValue <= 45) {
                stat = SFCADAS_Driving_NewConstantLeftLineLatPosStat::VALUE;
                value = mMvLeftLineOffsetValue;
            } else if (mMvLeftLineOffsetValue == kTimeOut) {
                stat = SFCADAS_Driving_NewConstantLeftLineLatPosStat::VALUE;
                value = 15;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantLeftLineLatPosStat(stat);
        setSFCADAS_Driving_NewConstantLeftLineLatPosValue(value);
    }

    // 5.2.2.3 차선-오른쪽 (3)
    // 5.2.2.3.1 칼라
    void updateConstantRightLaneColorStat() {
        SFCADAS_Driving_NewConstantRightLaneColorStat stat = SFCADAS_Driving_NewConstantRightLaneColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRightLineStatus == MvRightLineStatus::GRAY || mMvRightLineStatus == MvRightLineStatus::UNHANDLED_TIMEOUT) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::GRAY;
            } else if (mMvRightLineStatus == MvRightLineStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::WHITE;
            } else if (mMvRightLineStatus == MvRightLineStatus::AMBER) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::AMBER;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantRightLaneColorStat(stat);
    }

    // 5.2.2.3.2 횡위치
    void updateConstantRightLineLatPosStat() {
        SFCADAS_Driving_NewConstantRightLineLatPosStat stat = SFCADAS_Driving_NewConstantRightLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvRightLineOffsetValue && mMvRightLineOffsetValue <= 45) {
                stat = SFCADAS_Driving_NewConstantRightLineLatPosStat::VALUE;
                value = mMvRightLineOffsetValue;
            } else if (mMvRightLineOffsetValue == kTimeOut) {
                stat = SFCADAS_Driving_NewConstantRightLineLatPosStat::VALUE;
                value = 15;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantRightLineLatPosStat(stat);
        setSFCADAS_Driving_NewConstantRightLineLatPosValue(value);
    }

    // 5.2.2.4 차간 거리 (4)
    // 5.2.2.4.1 칼라
    void updateConstantHeadwayDistColorStat() {
        SFCADAS_Driving_NewConstantHeadwayDistColorStat stat = SFCADAS_Driving_NewConstantHeadwayDistColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvVehicleDistanceStatus == MvVehicleDistanceStatus::MAIN) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistColorStat::BRAND;
            }
        }

        setSFCADAS_Driving_NewConstantHeadwayDistColorStat(stat);
    }

    // 5.2.2.4.2 종위치
    void updateConstantHeadwayDistLongPosStat() {
        SFCADAS_Driving_NewConstantHeadwayDistLongPosStat stat = SFCADAS_Driving_NewConstantHeadwayDistLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvVehicleDistanceValue && mMvVehicleDistanceValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistLongPosStat::VALUE;
                value = mMvVehicleDistanceValue;
            } else if (1001 <= mMvVehicleDistanceValue && mMvVehicleDistanceValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistLongPosStat::VALUE;
                value = 1000;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantHeadwayDistLongPosStat(stat);
        setSFCADAS_Driving_NewConstantHeadwayDistLongPosValue(value);
    }

    // 5.2.2.5 전방 물체 (8)
    // 5.2.2.5.1 칼라
    void updateConstantViewFrontVehicleColorStat() {
        SFCADAS_Driving_NewConstantViewFrontVehicleColorStat stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontVehicleColorStat(stat);
    }

    // 5.2.2.5.2 횡위치
    void updateConstantViewFrontVehicleLatPosStat() {
        SFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat stat = SFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectLateralPositionValue == 0) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat::CENTER;
            } else if (1 <= mMvFrontObjectLateralPositionValue && mMvFrontObjectLateralPositionValue <= 45) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat::RIGHT_VALUE;
                value = mMvFrontObjectLateralPositionValue;
            } else if (83 <= mMvFrontObjectLateralPositionValue &&  // -45 ~ -1
                       mMvFrontObjectLateralPositionValue <= 127) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat::LEFT_VALUE;
                value = labs(mMvFrontObjectLateralPositionValue - 128);
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontVehicleLatPosValue(value);
    }

    // 5.2.2.5.3 종위치
    void updateConstantViewFrontVehicleLongPosStat() {
        SFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat stat = SFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvFrontObjectLongitudinalPositionValue && mMvFrontObjectLongitudinalPositionValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat::VALUE;
                value = mMvFrontObjectLongitudinalPositionValue;
            } else if (1001 <= mMvFrontObjectLongitudinalPositionValue && mMvFrontObjectLongitudinalPositionValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat::VALUE;
                value = 1000;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontVehicleLongPosValue(value);
    }

    // 5.2.2.6 레이더 파형-후방 왼쪽 (14)
    void updateConstantViewRearLeftWarnStat() {
        SFCADAS_Driving_NewConstantViewRearLeftWarnStat stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::WAVE_WARNING;
            } else if (mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::WAVE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewRearLeftWarnStat(stat);
    }

    // 5.2.2.7 레이더 파형-후방 오른쪽 (15)
    void updateConstantViewRearRightWarnStat() {
        SFCADAS_Driving_NewConstantViewRearRightWarnStat stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::WAVE_WARNING;
            } else if (mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::WAVE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewRearRightWarnStat(stat);
    }

    // 5.2.2.8 레이더 파형-전방 왼쪽
    void updateConstantViewFrontLeftWarnStat() {
        SFCADAS_Driving_NewConstantViewFrontLeftWarnStat stat = SFCADAS_Driving_NewConstantViewFrontLeftWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontLeftRadarWaveStatus == MvFrontLeftRadarWaveStatus::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftWarnStat::WAVE_WARNING;
            } else if (mMvFrontLeftRadarWaveStatus == MvFrontLeftRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftWarnStat::WAVE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontLeftWarnStat(stat);
    }

    // 5.2.2.9 레이더 파형-전방 오른쪽
    void updateConstantViewFrontRightWarnStat() {
        SFCADAS_Driving_NewConstantViewFrontRightWarnStat stat = SFCADAS_Driving_NewConstantViewFrontRightWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontRightRadarWaveStatus == MvFrontRightRadarWaveStatus::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightWarnStat::WAVE_WARNING;
            } else if (mMvFrontRightRadarWaveStatus == MvFrontRightRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightWarnStat::WAVE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontRightWarnStat(stat);
    }

    // 5.2.2.10 레이더 파형-전방
    void updateConstantViewFrontWarnStat() {
        SFCADAS_Driving_NewConstantViewFrontWarnStat stat = SFCADAS_Driving_NewConstantViewFrontWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontRadarWaveStatus == MvFrontRadarWaveStatus::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewFrontWarnStat::WAVE_WARNING;
            } else if (mMvFrontRadarWaveStatus == MvFrontRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewFrontWarnStat::WAVE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantViewFrontWarnStat(stat);
    }

    // 5.2.3.1 메인 차속 표지판 (1)
    // 5.2.3.1.1 형상
    void updateConstantISLAMainSpeedStat() {
        SFCADAS_Driving_NewConstantISLAMainSpeedStat stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::OFF;
        if (mConfigAdasISLA == ConfigAdasISLA::ON) {
            if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::BLACK_TEXT) {
                    if (mIslaSpeedLimitTrafficSignValue == 253) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::UNLIMITED;
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                               mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                               mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                               mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                               mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                        if (mIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE3_INVALID;
                        } else if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE3_VALID;
                        } else {
                            // no operation
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA) {
                        if (mIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE4_INVALID;
                        } else if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE4_VALID;
                        } else {
                            // no operation
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                        if (mIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE5_INVALID;
                        } else if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE5_VALID;
                        } else {
                            // no operation
                        }
                    } else {
                        // no operation
                    }
                } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::RED_TEXT) {
                    if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                        if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE6_VALID;
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA) {
                        if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE7_VALID;
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                        if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE8_VALID;
                        }
                    } else {
                        // no operation
                    }
                } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::AMBER) {
                    if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                    } else if ((mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) == false) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                    } else {
                        // no operation
                    }
                } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::UNHANDLED_TIMEOUT) {
                    if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                    } else if ((mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) == false) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                    } else {
                        // no operation
                    }
                } else {
                    // no operation
                }
            } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
                if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                    stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                } else if ((mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) == false) {
                    stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantISLAMainSpeedStat(stat);
    }

    // 5.2.3.1.2 제한 속도
    void updateConstantISLAMainSignStat() {
        SFCADAS_Driving_NewConstantISLAMainSignStat stat = SFCADAS_Driving_NewConstantISLAMainSignStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                stat = SFCADAS_Driving_NewConstantISLAMainSignStat::VALUE;
                value = mIslaSpeedLimitTrafficSignValue;
            } else if (mIslaSpeedLimitTrafficSignValue == 0) {
                stat = SFCADAS_Driving_NewConstantISLAMainSignStat::INVALID;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantISLAMainSignStat(stat);
        setSFCADAS_Driving_NewConstantISLAMainSignValue(value);
    }

    // 5.2.3.2 부가 표지판 (2)
    // 5.2.3.2.1 형상
    void updatePrivateISLAAddtnlSign() {
        mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
        if (mIsIgnOn == true) {
            if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::SCHOOL_CROSSING) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_AUS;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::STOP) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::YIELD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_AUS;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_PASS_LHD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_PASS_RHD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_RHD_EU;
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_ENTER) {
                mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER;
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::ROUNDABOUT_LHD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::ROUNDABOUT_RHD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_AUS;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::RIGHT_CURVE) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::LEFT_CURVE) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::RIGHT_LEFT_CURVES) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::LEFT_RIGHT_CURVES) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::PEDESTRIAN_CROSSING) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_AUS;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA ||
                           mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_CN;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_PASS_CV) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_CV_EU;
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_ENTER_TRUCK) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER_TRUCK_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER_TRUCK_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER_TRUCK_NA;
                } else {
                    // no operation
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::DO_NOT_ENTER_BUS) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER_BUS_DOM;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER_BUS_EU;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }

        updateConstantFinalDISISLAAddtnlSignStat();
    }

    // 5.2.3.2.2 ISLA/DIS 우선 순위 판단 로직
    // IMG_ISLAMainSign_Val에 대한 전달 방식은 Inter_DISAddtnlSign과 Private_ISLAAddtnlSign의 Enum 범위가 다르고,
    // Inter_DISAddtnlSign의 전달된 Enum value량이 많은 이유로, SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat과
    // SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat으로 나누어 output을 선언하였고, 어떠한 신호인지 casting하기 위해
    // SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat으로 선택하여 output을 전달하는 구조로 구현
    // (ADAS_Driving_New 참조)
    void updateConstantFinalDISISLAAddtnlSignStat() {
        static Private_ISLAAddtnlSign prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
        static DISAddtnlSign prevDISAddtnlSign = DISAddtnlSign::OFF;
        if (mIsIgnOn == true) {
            if (mPrivate_ISLAAddtnlSign == Private_ISLAAddtnlSign::OFF) {
                if (mDISAddtnlSign == DISAddtnlSign::OFF) {
                    if (mPriorityDISFlag == false && mPriorityISLAFlag == false) {
                        setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(SFCADAS_Driving_NewConstantFinalAddtnlSignStat::OFF);
                        setSFCADAS_Driving_NewConstantDISAddtnlSignStat(SFCADAS_Driving_NewConstantDISAddtnlSignStat::OFF);
                        setSFCADAS_Driving_NewConstantISLAAddtnlSignStat(SFCADAS_Driving_NewConstantISLAAddtnlSignStat::OFF);
                        prevDISAddtnlSign = DISAddtnlSign::OFF;
                        prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
                    }
                } else {
                    // Inter_DISAddtnlSign → at least 300ms
                    if (mPriorityISLAFlag == false && mPriorityDISFlag == false) {
                        // SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat을 DISADDTNLSIGN_STAT Type으로 캐스팅하여
                        // SFC.ADAS_Driving_New.Constant.DISAddtnlSign.Stat을 전달
                        setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(
                            SFCADAS_Driving_NewConstantFinalAddtnlSignStat::DISADDTNLSIGN_STAT);
                        setSFCADAS_Driving_NewConstantDISAddtnlSignStat(
                            static_cast<SFCADAS_Driving_NewConstantDISAddtnlSignStat>(mDISAddtnlSign));

                        mOneShotTimerPriorityDIS.stop();

                        if (prevDISAddtnlSign != mDISAddtnlSign) {
                            mPriorityDISFlag = true;
                            if (mOneShotTimerPriorityDIS.start() == false) {
                                DWarning() << "[ADAS_Driving_CV] mOneShotTimerPriorityDIS Start Failed";
                            }
                            prevDISAddtnlSign = mDISAddtnlSign;
                        }
                    }
                }
            } else {
                // Private_ISLAAddtnlSign → at least 300ms
                if (mPriorityISLAFlag == false && mPriorityDISFlag == false) {
                    // SFC.ADAS_Driving_New.Constant.FinalAddtnlSign.Stat을 ISLAADDTNLSIGN_STAT Type으로 캐스팅하여
                    // SFC.ADAS_Driving_New.Constant.ISLAAddtnlSign.Stat을 전달
                    setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(
                        SFCADAS_Driving_NewConstantFinalAddtnlSignStat::ISLAADDTNLSIGN_STAT);
                    setSFCADAS_Driving_NewConstantISLAAddtnlSignStat(mPrivate_ISLAAddtnlSign);

                    mOneShotTimerPriorityISLA.stop();

                    if (prevPrivate_ISLAAddtnlSign != mPrivate_ISLAAddtnlSign) {
                        mPriorityISLAFlag = true;
                        if (mOneShotTimerPriorityISLA.start() == false) {
                            DWarning() << "[ADAS_Driving_CV] mOneShotTimerPriorityISLA Start Failed";
                        }
                        prevPrivate_ISLAAddtnlSign = mPrivate_ISLAAddtnlSign;
                    }
                }
            }
        } else {
            setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(SFCADAS_Driving_NewConstantFinalAddtnlSignStat::OFF);
            setSFCADAS_Driving_NewConstantDISAddtnlSignStat(SFCADAS_Driving_NewConstantDISAddtnlSignStat::OFF);
            setSFCADAS_Driving_NewConstantISLAAddtnlSignStat(SFCADAS_Driving_NewConstantISLAAddtnlSignStat::OFF);
        }
    }

    void onOneShotTimerISLAChanged() {
        mPriorityISLAFlag = false;
        updateConstantFinalDISISLAAddtnlSignStat();
        flush();
    }

    void onOneShotTimerDISChanged() {
        mPriorityDISFlag = false;
        updateConstantFinalDISISLAAddtnlSignStat();
        flush();
    }

    // 5.2.3.3 보조 표지판 (3)
    void updateConstantISLAAddSignStat() {
        SFCADAS_Driving_NewConstantISLAAddSignStat stat = SFCADAS_Driving_NewConstantISLAAddSignStat::OFF;
        if (mIsIgnOn == true) {
            if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::RAIN) {
                stat = SFCADAS_Driving_NewConstantISLAAddSignStat::RAIN;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::SNOW) {
                stat = SFCADAS_Driving_NewConstantISLAAddSignStat::SNOW;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::SNOW_RAIN) {
                stat = SFCADAS_Driving_NewConstantISLAAddSignStat::SNOW_RAIN;
            } else if (mIslaSupplementaryTrafficSignStatus == IslaSupplementaryTrafficSignStatus::TRAILER) {
                stat = SFCADAS_Driving_NewConstantISLAAddSignStat::TRAILER;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantISLAAddSignStat(stat);
    }

    // 5.2.3.4 과속 경고음 ON/OFF 심볼
    void updateConstantISLASoundSymbolStat() {
        SFCADAS_Driving_NewConstantISLASoundSymbolStat stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::OFF;
        if (mIsIgnOn == true) {
            if (mIslaWarningSoundSymbolStatus == IslaWarningSoundSymbolStatus::DISABLE) {
                stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::DISABLE;
            } else if (mIslaWarningSoundSymbolStatus == IslaWarningSoundSymbolStatus::ENABLE) {
                stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::ENABLE;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantISLASoundSymbolStat(stat);
    }

    // 5.2.4 자율 주행 점검 메시지
    // 5.2.4.1 전방 안전 고장
    void updateConstantForwardSafetyFailure() {
        SFCADAS_Driving_NewConstantForwardSafetyFailureStat stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::OFF;

        if (mIsIgnOn == true) {
            if ((mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasFCA == ConfigAdasFCA::ON) ||
                (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantForwardSafetyFailureStat(stat);
    }

    // 5.2.4.2 전방 안전-근거리 고장 (FCW-Near)
    void updateConstantForwardSideSafetyFailure() {
        SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat stat =
            SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::OFF;

        if (mIsIgnOn == true) {
            if ((mPuFGroup7FCWNearFailureStatus == PuFGroup7FCWNearFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasFCWNear == ConfigAdasFCWNear::ON) ||
                (mPuFGroup7FCWNearFailureStatus == PuFGroup7FCWNearFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7FCWNearFailureStatus == PuFGroup7FCWNearFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantForwardSideSafetyFailureStat(stat);
    }

    // 5.2.4.3 차로 안전 고장
    void updateConstantLaneSafetyFailure() {
        SFCADAS_Driving_NewConstantLaneSafetyFailureStat stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasLDW == ConfigAdasLDW::ON) ||
                (mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::CAMERA_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantLaneSafetyFailureStat(stat);
    }

    // 5.2.4.4 후측방 안전 고장(BCW, BCW-Near)
    void updateConstantBlindSpotSafetyFailure() {
        SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasBCW == ConfigAdasBCW::ON || mConfigAdasBCWNear == ConfigAdasBCWNear::ON)) ||
                (mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat(stat);
    }

    // 5.2.4.5 속도 제한 고장
    void updateConstantIslaFailure() {
        SFCADAS_Driving_NewConstantIslaFailureStat stat = SFCADAS_Driving_NewConstantIslaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7IslaFailureStatus == PuFGroup7IslaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasISLA == ConfigAdasISLA::ON) ||
                (mPuFGroup7IslaFailureStatus == PuFGroup7IslaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantIslaFailureStat::FAILURE;
            } else if (mPuFGroup7IslaFailureStatus == PuFGroup7IslaFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantIslaFailureStat::CAMERA_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantIslaFailureStat(stat);
    }

    // 5.2.4.6 부주의 운전 경고 고장
    void updateConstantDawFailure() {
        SFCADAS_Driving_NewConstantDawFailureStat stat = SFCADAS_Driving_NewConstantDawFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7DawFailureStatus == PuFGroup7DawFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasDAW == ConfigAdasDAW::ON) ||
                (mPuFGroup7DawFailureStatus == PuFGroup7DawFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantDawFailureStat::FAILURE;
            } else if (mPuFGroup7DawFailureStatus == PuFGroup7DawFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantDawFailureStat::CAMERA_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantDawFailureStat(stat);
    }

    // 5.2.4.7 하이빔 보조 고장
    void updateConstantHbaFailure() {
        SFCADAS_Driving_NewConstantHbaFailureStat stat = SFCADAS_Driving_NewConstantHbaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7HbaFailureStatus == PuFGroup7HbaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigNewHBA == ConfigNewHBA::ON) ||
                (mPuFGroup7HbaFailureStatus == PuFGroup7HbaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantHbaFailureStat::FAILURE;
            }
        }

        setSFCADAS_Driving_NewConstantHbaFailureStat(stat);
    }

    // 5.2.4.8 스마트 크루즈 컨트롤 고장
    void updateConstantSccFailure() {
        SFCADAS_Driving_NewConstantSccFailureStat stat = SFCADAS_Driving_NewConstantSccFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7SccFailureStatus == PuFGroup7SccFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasSCC == ConfigAdasSCC::ON) ||
                (mPuFGroup7SccFailureStatus == PuFGroup7SccFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantSccFailureStat::FAILURE;
            } else if (mPuFGroup7SccFailureStatus == PuFGroup7SccFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantSccFailureStat::RADAR_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantSccFailureStat(stat);
    }

    // 5.2.4.9 차로 유지 보조 고장
    void updateConstantLfaFailure() {
        SFCADAS_Driving_NewConstantLfaFailureStat stat = SFCADAS_Driving_NewConstantLfaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7LfaFailureStatus == PuFGroup7LfaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasLFA == ConfigAdasLFA::ON) ||
                (mPuFGroup7LfaFailureStatus == PuFGroup7LfaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantLfaFailureStat::FAILURE;
            }
        }

        setSFCADAS_Driving_NewConstantLfaFailureStat(stat);
    }

    // 5.2.4.10 고속도로 주행 보조 고장
    void updateConstantHdaFailure() {
        SFCADAS_Driving_NewConstantHdaFailureStat stat = SFCADAS_Driving_NewConstantHdaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7HdaFailureStatus == PuFGroup7HdaFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA == ConfigAdasHDA::ON)) ||
                (mPuFGroup7HdaFailureStatus == PuFGroup7HdaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantHdaFailureStat::FAILURE;
            }
        }

        setSFCADAS_Driving_NewConstantHdaFailureStat(stat);
    }

    // 5.2.4.11 비상 정지 기능 고장
    void updateConstantEmergencyStopFailure() {
        SFCADAS_Driving_NewConstantEmergencyStopFailureStat stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT && mConfigSCC2 == ConfigSCC2::ON) ||
                (mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::FAILURE;
            } else if (mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::RADAR_BLOCKAGE;
            } else {
                // no operation
            }
        }

        setSFCADAS_Driving_NewConstantEmergencyStopFailureStat(stat);
    }

    // Event
    // 5.3.1 주행 보조 요약 정보
    void updateEventDrivingAssistSummary() {
        SFCADAS_Driving_NewEventDrivingAssistSummaryStat eventStat = SFCADAS_Driving_NewEventDrivingAssistSummaryStat::OFF;
        if (mIsIgnOn == true) {
            if (mSummaryFrontVehicleStat != SFCADAS_Driving_NewConstantFrontVehicleColorStat::OFF ||  // IMG_ADASFrontVehicle_Stat
                mSummaryFrontVehicleValue !=
                    SFCADAS_Driving_NewConstantFrontVehicleLevelValue::OFF ||  // IMG_ADAS_FrontVehicle_Value
                mSummaryHeadwayDistanceSetStat !=
                    SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::OFF ||  // IMG_ADASHeadwayDistanceSet_Stat
                mSummaryHeadwayDistanceSetVal !=
                    SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::OFF ||  // IMG_ADASHeadwayDistanceSet_Val
                mSummarySubVehicleStat != SFCADAS_Driving_NewConstantSubVehicleColorStat::OFF ||  // IMG_ADASSubVehicle_Stat
                mSummarySetSpeedStat != SFCADAS_Driving_NewConstantSetSpeedColorStat::OFF ||      // IMG_ADASSetSpeed_Stat
                mSummaryHDAHDPTelltaleStat != SFCADAS_Driving_NewConstantHDAHDPStat::OFF ||       // IMG_HDAHDPTelltale_Stat
                mSummaryISLAManualSetSpeedStat !=
                    SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::OFF ||  // IMG_ISLAManualSetSpeed_Stat
                mSummaryNSCCAutoSetTelltaleStat !=
                    SFCADAS_Driving_NewConstantNSCCAutoSetStat::OFF) {  // IMG_NSCCAutoSetTelltale_Stat
                eventStat = SFCADAS_Driving_NewEventDrivingAssistSummaryStat::ON;
            }
        }
        setSFCADAS_Driving_NewEventDrivingAssistSummaryStat(eventStat);
    }

    // 5.3.2 1그룹 풀 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEventGroup1FullPopup1_1() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mPuFGroup1AdasWarning1_1Status) {
                case PuFGroup1AdasWarning1_1Status::WARNING_FRONT:
                    eventID = "E52002";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT:
                    eventID = "E52007";
                    break;
                case PuFGroup1AdasWarning1_1Status::END_OF_ASSIST:
                    eventID = "E52018";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group1FullPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup1FullPopup1_1Stat(SFCADAS_Driving_NewEventGroup1FullPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup1FullPopup1_1ID(eventID);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_1Stat(SFCADAS_Driving_NewEventGroup1FullPopup1_1Stat::ON);
        }
    }

    // 5.3.3 1그룹 풀 팝업 1-2 (FR_CMR, ADAS_DRV)
    void updateEventGroup1FullPopup1_2() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mPuFGroup1AdasWarning1_2Status) {
                case PuFGroup1AdasWarning1_2Status::HANDS_OFF_WARN_LV1:
                    eventID = "E52019";
                    break;
                case PuFGroup1AdasWarning1_2Status::HANDS_OFF_WARN_LV2:
                    eventID = "E52020";
                    break;
                case PuFGroup1AdasWarning1_2Status::LFA_AUTOMATIC_OFF:
                    eventID = "E52021";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDA_AUTOMATIC_OFF:
                    eventID = "E52022";
                    break;
                case PuFGroup1AdasWarning1_2Status::DAW_TAKE_REST:
                    eventID = "E52023";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_TD_LV1:
                    eventID = "E52072";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_TD_LV2:
                    eventID = "E52073";
                    break;
                case PuFGroup1AdasWarning1_2Status::EMERGENCY_STOP:
                    eventID = "E52071";
                    break;
                case PuFGroup1AdasWarning1_2Status::EMERGENCY_STOP_INACTIVE:
                    eventID = "E52110";
                    break;
                case PuFGroup1AdasWarning1_2Status::DAW_TAKE_REST_WITH_OK_BUTTON:
                    eventID = "E52119";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        // 의도적으로 Event A On, B On 발생시 flush를
        // Event B ON -> Event A OFF 순서로 전달하기 위해 다른 모듈과 출력 코드를 다르게 적용합니다.
        // A OFF 전달이 노드의 최종 상태가 되므로, Active 되어 있는 B Event의 추후 OFF 처리를 위하여
        // lastActiveEventID를 사용했습니다.
        // std::string prevEventID;
        // GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event4Group.ID, prevEventID);

        static std::string lastActiveEventID;
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2ID(eventID);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2Stat(SFCADAS_Driving_NewEventGroup1FullPopup1_2Stat::ON);
            flushLastGroup();

        } else {
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2ID(lastActiveEventID);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2Stat(SFCADAS_Driving_NewEventGroup1FullPopup1_2Stat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2ID(lastActiveEventID);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2Stat(SFCADAS_Driving_NewEventGroup1FullPopup1_2Stat::OFF);
            flushLastGroup();
        }

        lastActiveEventID = eventID;
    }

    // 5.3.4 1그룹 풀 팝업 A-A (FCW-Near)
    void updateEventGroup1FullPopupA_A() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mPuFGroup1AdasWarningA_AStatus) {
                case PuFGroup1AdasWarningA_AStatus::FRONT_WARNING1:
                    eventID = "E77301";
                    break;
                case PuFGroup1AdasWarningA_AStatus::FRONT_WARNING2:
                    eventID = "E77302";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group1FullPopupA_A.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup1FullPopupA_AStat(SFCADAS_Driving_NewEventGroup1FullPopupA_AStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup1FullPopupA_AID(eventID);
            setSFCADAS_Driving_NewEventGroup1FullPopupA_AStat(SFCADAS_Driving_NewEventGroup1FullPopupA_AStat::ON);
        }
    }

    // 5.3.5 4그룹 풀 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEventGroup4FullPopup1_1() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType linkedSoundType =
            SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            switch (mPuFGroup4AdasWarning1_1Status) {
                case PuFGroup4AdasWarning1_1Status::SCC_MRM:
                    eventID = "E52031";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::INFINITE;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_ATTENTION_WARNING:
                    eventID = "E52032";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_AUTOMATIC_OFF:
                    eventID = "E52033";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_CONDITION_NOT_MET:
                    eventID = "E52034";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_STOP_CONTROL:
                    eventID = "E52035";
                    break;
                case PuFGroup4AdasWarning1_1Status::DAW_LEAD_VEHICLE_DEPARTURE:
                    eventID = "E52037";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_REGULATIONS:
                    eventID = "E52038";
                    break;
                case PuFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET:
                    eventID = "E52040";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::OFF_DUE_TO_LIMITATION_ON_DRIVER_MONITORING:
                    eventID = "E52109";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_CONDITION_NOT_MET_BY_MRM:
                    eventID = "E52111";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET_BY_EMERGENCY_STOP:
                    eventID = "E52118";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group4FullPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1Stat(SFCADAS_Driving_NewEventGroup4FullPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1ID(eventID);
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1Stat(SFCADAS_Driving_NewEventGroup4FullPopup1_1Stat::ON);
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundID(linkedSoundID);
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType(linkedSoundType);
            setSFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.3.6 7그룹 풀 팝업 - 운전자 보조 시스템 고장 1
    void updateEventGroup7FullPopup1() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT &&
                 (mConfigAdasLDW == ConfigAdasLDW::ON || mConfigAdasFCA == ConfigAdasFCA::ON ||
                  mConfigAdasFCWNear == ConfigAdasFCWNear::ON || mConfigAdasBCWNear == ConfigAdasBCWNear::ON ||
                  mConfigAdasBCW == ConfigAdasBCW::ON)) ||
                (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::FAILURE)) {
                eventID = "E77303";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::CAMERA_BLOCKAGE) {
                eventID = "E77304";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::RADAR_BLOCKAGE) {
                eventID = "E77305";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::CAMERA_RADAR_BLOCKAGE) {
                eventID = "E77306";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.DriverAssistFailure1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventDriverAssistFailure1Stat(SFCADAS_Driving_NewEventDriverAssistFailure1Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventDriverAssistFailure1ID(eventID);
            setSFCADAS_Driving_NewEventDriverAssistFailure1Stat(SFCADAS_Driving_NewEventDriverAssistFailure1Stat::ON);
        }
    }

    void updateEventGroup7OutsideMirrorSymbolFailure() {
        SFCADAS_Driving_NewEventOutsideMirrorSymbolFailureStat stat = SFCADAS_Driving_NewEventOutsideMirrorSymbolFailureStat::OFF;
        if (mIsIgnOn == true) {
            if (mPuFGroup7OutsideMirrorSymbolFailureStatus == PuFGroup7OutsideMirrorSymbolFailureStatus::FAILURE) {
                stat = SFCADAS_Driving_NewEventOutsideMirrorSymbolFailureStat::ON;
            }
        }

        setSFCADAS_Driving_NewEventOutsideMirrorSymbolFailureStat(stat);
    }

    // 5.3.8 2그룹 미니 팝업 1-1
    void updateEventGroup2MiniPopup1_1() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mPuMGroup2AdasWarning1_1Status) {
                case PuMGroup2AdasWarning1_1Status::MSLA_AUTOSET_ON:
                    eventID = "E52501";
                    break;
                case PuMGroup2AdasWarning1_1Status::MSLA_AUTOSET_SPEED_CHANGED:
                    eventID = "E52502";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_AUTOSET_ON:
                    eventID = "E52503";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_AUTOSET_SPEED_CHANGED:
                    eventID = "E52504";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV1:
                    eventID = "E52505";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV2:
                    eventID = "E52506";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV3:
                    eventID = "E52507";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV4:
                    eventID = "E52508";
                    break;
                case PuMGroup2AdasWarning1_1Status::NSCC_WARNING:
                    eventID = "E52509";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group2MiniPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup2MiniPopup1_1Stat(SFCADAS_Driving_NewEventGroup2MiniPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup2MiniPopup1_1ID(eventID);
            setSFCADAS_Driving_NewEventGroup2MiniPopup1_1Stat(SFCADAS_Driving_NewEventGroup2MiniPopup1_1Stat::ON);
        }
    }

    // Sound
    // 6.1 경고음 1-1
    void updateSoundAdasWarning1_1Status() {
        std::string soundID;

        if (mSoundAdasWarning1_1Status == SoundAdasWarning1_1Status::WARNING_SOUND_2_AUTO) {
            soundID = "SND_ADAS_PopUpWarn2_Auto";
        } else if (mSoundAdasWarning1_1Status == SoundAdasWarning1_1Status::WARNING_SOUND_3_AUTO) {
            soundID = "SND_ADAS_PopUpWarn3_Auto";
        } else {
            // no operation
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Sound.AdasWarning1_1.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_Driving_NewSoundAdasWarning1_1Stat(SFCADAS_Driving_NewSoundAdasWarning1_1Stat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_Driving_NewSoundAdasWarning1_1ID(soundID);
            setSFCADAS_Driving_NewSoundAdasWarning1_1Stat(SFCADAS_Driving_NewSoundAdasWarning1_1Stat::ON);
        }
    }

    // 6.2 경고음 1-2
    void updateSoundAdasWarning1_2Status() {
        std::string soundID;
        SFCADAS_Driving_NewSoundAdasWarning1_2Type type = SFCADAS_Driving_NewSoundAdasWarning1_2Type::NONE;
        HUInt64 count = 0;
        if (mIsIgnOn == true) {
            if (mSoundAdasWarning1_2Status == SoundAdasWarning1_2Status::WARNING_SOUND_1_MEDIUM) {
                soundID = "SND_ADAS_PopUpWarn1_Fixed";
                type = SFCADAS_Driving_NewSoundAdasWarning1_2Type::REPEAT_COUNT;
                count = 1;
            } else if (mSoundAdasWarning1_2Status == SoundAdasWarning1_2Status::WARNING_SOUND_1_AUTO) {
                soundID = "SND_ADAS_PopUpWarn1_Auto";
                type = SFCADAS_Driving_NewSoundAdasWarning1_2Type::INFINITE;
            } else if (mSoundAdasWarning1_2Status == SoundAdasWarning1_2Status::WARNING_SOUND_2_AUTO) {
                soundID = "SND_ADAS_PopUpWarn2_Auto";
                type = SFCADAS_Driving_NewSoundAdasWarning1_2Type::INFINITE;
            } else {
                // no operation
            }
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Sound.AdasWarning1_2.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_Driving_NewSoundAdasWarning1_2Stat(SFCADAS_Driving_NewSoundAdasWarning1_2Stat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_Driving_NewSoundAdasWarning1_2ID(soundID);
            setSFCADAS_Driving_NewSoundAdasWarning1_2Stat(SFCADAS_Driving_NewSoundAdasWarning1_2Stat::ON);
            setSFCADAS_Driving_NewSoundAdasWarning1_2Type(type);
            setSFCADAS_Driving_NewSoundAdasWarning1_2RepeatCount(count);
        }
    }

    // 6.3 경고음 1-3
    void updateSoundAdasWarning1_3Status() {
        std::string soundID;
        SFCADAS_Driving_NewSoundAdasWarning1_3Type type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::NONE;
        HUInt64 count = 0;

        if (mIsIgnOn == true) {
            if (mSoundAdasWarning1_3Status == SoundAdasWarning1_3Status::SPECIAL_SOUND_1_AUTO) {
                soundID = "SND_ADAS_MSLA_Auto";
                type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::INFINITE;
            } else if (mSoundAdasWarning1_3Status == SoundAdasWarning1_3Status::ALARM_SOUND_1_MEDIUM) {
                soundID = "SND_ADAS_PopUpInform1_Fixed";
                type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::REPEAT_COUNT;
                count = 1;
            } else {
                // no operation
            }
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Sound.AdasWarning1_3.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_Driving_NewSoundAdasWarning1_3Stat(SFCADAS_Driving_NewSoundAdasWarning1_3Stat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_Driving_NewSoundAdasWarning1_3ID(soundID);
            setSFCADAS_Driving_NewSoundAdasWarning1_3Stat(SFCADAS_Driving_NewSoundAdasWarning1_3Stat::ON);
            setSFCADAS_Driving_NewSoundAdasWarning1_3Type(type);
            setSFCADAS_Driving_NewSoundAdasWarning1_3RepeatCount(count);
        }
    }

    // 6.4 경고음 1-4
    void updateSoundAdasWarning1_4Status() {
        SFCADAS_Driving_NewSoundAdasWarning1_4Stat stat = SFCADAS_Driving_NewSoundAdasWarning1_4Stat::OFF;
        if (mIsIgnOn == true && mSoundAdasWarning1_4Status == SoundAdasWarning1_4Status::ALARM_SOUND_2_MEDIUM) {
            stat = SFCADAS_Driving_NewSoundAdasWarning1_4Stat::ON;
        }

        setSFCADAS_Driving_NewSoundAdasWarning1_4Stat(stat);
    }

    // 6.5 경고음 1-5
    void updateSoundAdasWarning1_5Status() {
        SFCADAS_Driving_NewSoundAdasWarning1_5Stat stat = SFCADAS_Driving_NewSoundAdasWarning1_5Stat::OFF;
        if (mIsIgnOn == true && mSoundAdasWarning1_5Status == SoundAdasWarning1_5Status::SPECIAL_SOUND_4_AUTO) {
            stat = SFCADAS_Driving_NewSoundAdasWarning1_5Stat::ON;
        }

        setSFCADAS_Driving_NewSoundAdasWarning1_5Stat(stat);
    }

    // 6.6 경고음 2-1
    void updateSoundAdasWarning2_1Status() {
        SFCADAS_Driving_NewSoundAdasWarning2_1Stat stat = SFCADAS_Driving_NewSoundAdasWarning2_1Stat::OFF;
        if (mIsIgnOn == true && mSoundAdasWarning2_1Status == SoundAdasWarning2_1Status::SPECIAL_SOUND_5_AUTO) {
            stat = SFCADAS_Driving_NewSoundAdasWarning2_1Stat::ON;
        }

        setSFCADAS_Driving_NewSoundAdasWarning2_1Stat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using Private_ISLAAddtnlSign = SFCADAS_Driving_NewConstantISLAAddtnlSignStat;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigAdasISLA = ArgumentsConfigIslaChanged::Inter_ConfigAdasISLA;
    using ConfigAdasFCA = ArgumentsConfigFca1Changed::Inter_ConfigAdasFCA;
    using ConfigAdasFCWNear = ArgumentsConfigFCWNearChanged::Inter_ConfigAdasFCWNear;
    using ConfigAdasLDW = ArgumentsConfigLDWChanged::Inter_ConfigAdasLDW;
    using ConfigAdasBCW = ArgumentsConfigAdasBcwChanged::Inter_ConfigAdasBCW;
    using ConfigAdasBCWNear = ArgumentsConfigBCWNearChanged::Inter_ConfigAdasBCWNear;
    using ConfigAdasDAW = ArgumentsConfigDawChanged::Inter_ConfigAdasDAW;
    using ConfigIFSCV = ArgumentsConfigIFSCVChanged::Inter_ConfigIFSCV;
    using ConfigNewHBA = ArgumentsConfigHbaChanged::Inter_ConfigNewHBA;
    using ConfigAdasSCC = ArgumentsConfigSccChanged::Inter_ConfigAdasSCC;
    using ConfigAdasLFA = ArgumentsConfigLfaChanged::Inter_ConfigAdasLFA;
    using ConfigAdasHDA = ArgumentsConfigHdaChanged::Inter_ConfigAdasHDA;
    using ConfigSCC2 = ArgumentsConfigSCC2Changed::Inter_ConfigSCC2;

    using DISAddtnlSign = ArgumentsInter_DISAddtnlSign::Inter_DISAddtnlSign;
    using ForwardSafetySymbolStatus = ArgumentsForwardSafetySymbolStatusChanged::Input_ForwardSafetySymbolStatus;
    using LaneSafetySymbolStatus = ArgumentsLaneSafetySymbolStatusChanged::Input_LaneSafetySymbolStatus;
    using DriveAttentionWarningSymbolStatus =
        ArgumentsDriveAttentionWarningSymbolStatusChanged::Input_DriveAttentionWarningSymbolStatus;
    using IfsIndLampStatus = ArgumentsIfsIndLampStatusChanged::Input_IFSIndLampStatus;
    using HbaSymbolStatus = ArgumentsHbaSymbolStatusChanged::Input_HbaSymbolStatus;
    using SmvFrontObjectStatus = ArgumentsSmvFrontObjectStatusChanged::Input_SmvFrontObjectStatus;
    using SmvVehicleDistanceLevelValue = ArgumentsSmvVehicleDistanceLevelValueChanged::Input_SmvVehicleDistanceLevelValue;
    using SmvVehicleDistanceLevelStatus = ArgumentsSmvVehicleDistanceLevelStatusChanged::Input_SmvVehicleDistanceLevelStatus;
    using SmvHostVehicleStatus = ArgumentsSmvHostVehicleStatusChanged::Input_SmvHostVehicleStatus;
    using SmvSetSpeedStatus = ArgumentsSmvSetSpeedStatusChanged::Input_SmvSetSpeedStatus;
    using SmvHdaSymbolStatus = ArgumentsSmvHdaSymbolStatusChanged::Input_SmvHdaSymbolStatus;
    using SmvIslaSetSpeedSymbolStatus = ArgumentsSmvIslaSetSpeedSymbolStatusChanged::Input_SmvIslaSetSpeedSymbolStatus;
    using SmvNsccSymbolStatus = ArgumentsSmvNsccSymbolStatusChanged::Input_SmvNsccSymbolStatus;
    using SmvLfaSymbolStatus = ArgumentsSmvLfaSymbolStatusChanged::Input_SmvLfaSymbolStatus;
    using FCWNearSymbolStatus = ArgumentsFcwNearSymbolStatusChanged::Input_FCWNearSymbolStatus;
    using BlindSpotSafetySymbolStatus = ArgumentsBlindSpotSafetySymbolStatusChanged::Input_BlindSpotSafetySymbolStatus;

    using MvDrivingLaneCenterLineStatus = ArgumentsMvDrivingLaneCenterLineStatusChanged::Input_MvDrivingLaneCenterLineStatus;
    using MvLeftLineStatus = ArgumentsMvLeftLineStatusChanged::Input_MvLeftLineStatus;
    using MvRightLineStatus = ArgumentsMvRightLineStatusChanged::Input_MvRightLineStatus;
    using MvVehicleDistanceStatus = ArgumentsMvVehicleDistanceStatusChanged::Input_MvVehicleDistanceStatus;
    using MvFrontObjectStatus = ArgumentsMvFrontObjectStatusChanged::Input_MvFrontObjectStatus;
    using MvRearLeftRadarWave2Status = ArgumentsMvRearLeftRadarWaveStatusChanged::Input_MvRearLeftRadarWave2Status;
    using MvRearRightRadarWave2Status = ArgumentsMvRearRightRadarWaveStatusChanged::Input_MvRearRightRadarWave2Status;
    using IslaSpeedLimitTrafficSignStatus =
        ArgumentsIslaSpeedLimitTrafficSignStatusChanged::Input_IslaSpeedLimitTrafficSignStatus;
    using IslaTrafficSignCountryInformationStatus =
        ArgumentsIslaTrafficSignCountryInformationStatusChanged::Input_IslaTrafficSignCountryInformationStatus;
    using PuFGroup7DriverAssistFailure1Status =
        ArgumentsPuFGroup7DriverAssistFailure1StatusChanged::Input_PuFGroup7DriverAssistFailure1Status;
    using PuFGroup7OutsideMirrorSymbolFailureStatus =
        ArgumentsPuFGroup7OutsideMirrorSymbolFailureStatusChanged::Input_PuFGroup7OutsideMirrorSymbolFailureStatus;
    using IslaWarningSoundSymbolStatus = ArgumentsIslaWarningSoundSymbolStatusChanged::Input_IslaWarningSoundSymbolStatus;
    using IslaAdditionalTrafficSignStatus =
        ArgumentsIslaAdditionalTrafficSignStatusChanged::Input_IslaAdditionalTrafficSignStatus;
    using IslaSupplementaryTrafficSignStatus =
        ArgumentsIslaSupplementaryTrafficSignStatusChanged::Input_IslaSupplementaryTrafficSignStatus;
    using PuFGroup1AdasWarning1_1Status = ArgumentsPuFGroup1AdasWarning1_1StatusChanged::Input_PuFGroup1AdasWarning1_1Status;
    using PuFGroup1AdasWarning1_2Status = ArgumentsPuFGroup1AdasWarning1_2StatusChanged::Input_PuFGroup1AdasWarning1_2Status;
    using PuFGroup1AdasWarningA_AStatus = ArgumentsPuFGroup1AdasWarningA_AStatusChanged::Input_PuFGroup1AdasWarningA_AStatus;
    using PuFGroup4AdasWarning1_1Status = ArgumentsPuFGroup4AdasWarning1_1StatusChanged::Input_PuFGroup4AdasWarning1_1Status;
    using PuFGroup7ForwardSafetyFailureStatus =
        ArgumentsPuFGroup7ForwardSafetyFailureStatusChanged::Input_PuFGroup7ForwardSafetyFailureStatus;
    using PuFGroup7FCWNearFailureStatus = ArgumentsPuFGroup7FCWNearFailureStatusChanged::Input_PuFGroup7FCWNearFailureStatus;
    using PuFGroup7LaneSafetyFailureStatus =
        ArgumentsPuFGroup7LaneSafetyFailureStatusChanged::Input_PuFGroup7LaneSafetyFailureStatus;
    using PuFGroup7BlindSpotSafetyFailureStatus =
        ArgumentsPuFGroup7BlindSpotSafetyFailureStatusChanged::Input_PuFGroup7BlindSpotSafetyFailureStatus;
    using PuFGroup7IslaFailureStatus = ArgumentsPuFGroup7IslaFailureStatusChanged::Input_PuFGroup7IslaFailureStatus;
    using PuFGroup7DawFailureStatus = ArgumentsPuFGroup7DawFailureStatusChanged::Input_PuFGroup7DawFailureStatus;
    using PuFGroup7HbaFailureStatus = ArgumentsPuFGroup7HbaFailureStatusChanged::Input_PuFGroup7HbaFailureStatus;
    using PuFGroup7SccFailureStatus = ArgumentsPuFGroup7SccFailureStatusChanged::Input_PuFGroup7SccFailureStatus;
    using PuFGroup7LfaFailureStatus = ArgumentsPuFGroup7LfaFailureStatusChanged::Input_PuFGroup7LfaFailureStatus;
    using PuFGroup7HdaFailureStatus = ArgumentsPuFGroup7HdaFailureStatusChanged::Input_PuFGroup7HdaFailureStatus;
    using PuFGroup7MRMFailureStatus = ArgumentsPuFGroup7MRMFailureStatusChanged::Input_PuFGroup7MRMFailureStatus;
    using PuMGroup2AdasWarning1_1Status = ArgumentsPuMGroup2AdasWarning1_1StatusChanged::Input_PuMGroup2AdasWarning1_1Status;
    using InterTelltaleIccDrow = ArgumentsInterTelltaleIccDrowChanged::Inter_TelltaleIccDrow;
    using MvFrontLeftRadarWaveStatus = ArgumentsMvFrontLeftRadarWaveStatusChanged::Input_MvFrontLeftRadarWaveStatus;
    using MvFrontRightRadarWaveStatus = ArgumentsMvFrontRightRadarWaveStatusChanged::Input_MvFrontRightRadarWaveStatus;
    using MvFrontRadarWaveStatus = ArgumentsMvFrontRadarWaveStatusChanged::Input_MvFrontRadarWaveStatus_CV;

    using SoundAdasWarning1_1Status = ArgumentsSoundAdasWarning1_1StatusChanged::Input_SoundAdasWarning1_1Status;
    using SoundAdasWarning1_2Status = ArgumentsSoundAdasWarning1_2StatusChanged::Input_SoundAdasWarning1_2Status;
    using SoundAdasWarning1_3Status = ArgumentsSoundAdasWarning1_3StatusChanged::Input_SoundAdasWarning1_3Status;
    using SoundAdasWarning1_4Status = ArgumentsSoundAdasWarning1_4StatusChanged::Input_SoundAdasWarning1_4Status;
    using SoundAdasWarning1_5Status = ArgumentsSoundAdasWarning1_5StatusChanged::Input_SoundAdasWarning1_5Status;
    using SoundAdasWarning2_1Status = ArgumentsSoundAdasWarning2_1StatusChanged::Input_SoundAdasWarning2_1Status;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    HBool mPriorityISLAFlag = false;
    HBool mPriorityDISFlag = false;

    Private_ISLAAddtnlSign mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::NONE;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigAdasISLA mConfigAdasISLA = ConfigAdasISLA::OFF;
    ConfigAdasFCA mConfigAdasFCA = ConfigAdasFCA::OFF;
    ConfigAdasFCWNear mConfigAdasFCWNear = ConfigAdasFCWNear::OFF;
    ConfigAdasLDW mConfigAdasLDW = ConfigAdasLDW::OFF;
    ConfigAdasBCWNear mConfigAdasBCWNear = ConfigAdasBCWNear::OFF;
    ConfigAdasBCW mConfigAdasBCW = ConfigAdasBCW::OFF;
    ConfigAdasDAW mConfigAdasDAW = ConfigAdasDAW::OFF;
    ConfigIFSCV mConfigIFSCV = ConfigIFSCV::OFF;
    ConfigNewHBA mConfigNewHBA = ConfigNewHBA::OFF;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;
    ConfigAdasLFA mConfigAdasLFA = ConfigAdasLFA::OFF;
    ConfigAdasHDA mConfigAdasHDA = ConfigAdasHDA::OFF;
    ConfigSCC2 mConfigSCC2 = ConfigSCC2::OFF;

    DISAddtnlSign mDISAddtnlSign = DISAddtnlSign::OFF;
    ForwardSafetySymbolStatus mForwardSafetySymbolStatus = ForwardSafetySymbolStatus::OFF;
    LaneSafetySymbolStatus mLaneSafetySymbolStatus = LaneSafetySymbolStatus::OFF;
    DriveAttentionWarningSymbolStatus mDriveAttentionWarningSymbolStatus = DriveAttentionWarningSymbolStatus::OFF;
    IfsIndLampStatus mIfsIndLampStatus = IfsIndLampStatus::OFF;
    HbaSymbolStatus mHbaSymbolStatus = HbaSymbolStatus::OFF;
    SmvFrontObjectStatus mSmvFrontObjectStatus = SmvFrontObjectStatus::OFF;
    SmvVehicleDistanceLevelValue mSmvVehicleDistanceLevelValue = SmvVehicleDistanceLevelValue::OFF;
    SmvVehicleDistanceLevelStatus mSmvVehicleDistanceLevelStatus = SmvVehicleDistanceLevelStatus::OFF;
    SmvHostVehicleStatus mSmvHostVehicleStatus = SmvHostVehicleStatus::OFF;
    SmvSetSpeedStatus mSmvSetSpeedStatus = SmvSetSpeedStatus::OFF;
    SmvHdaSymbolStatus mSmvHdaSymbolStatus = SmvHdaSymbolStatus::OFF;
    SmvIslaSetSpeedSymbolStatus mSmvIslaSetSpeedSymbolStatus = SmvIslaSetSpeedSymbolStatus::OFF;
    SmvNsccSymbolStatus mSmvNsccSymbolStatus = SmvNsccSymbolStatus::OFF;
    SmvLfaSymbolStatus mSmvLfaSymbolStatus = SmvLfaSymbolStatus::OFF;
    FCWNearSymbolStatus mFCWNearSymbolStatus = FCWNearSymbolStatus::OFF;
    BlindSpotSafetySymbolStatus mBlindSpotSafetySymbolStatus = BlindSpotSafetySymbolStatus::OFF;
    MvDrivingLaneCenterLineStatus mMvDrivingLaneCenterLineStatus = MvDrivingLaneCenterLineStatus::OFF;
    MvLeftLineStatus mMvLeftLineStatus = MvLeftLineStatus::OFF;
    MvRightLineStatus mMvRightLineStatus = MvRightLineStatus::OFF;
    MvVehicleDistanceStatus mMvVehicleDistanceStatus = MvVehicleDistanceStatus::OFF;
    MvFrontObjectStatus mMvFrontObjectStatus = MvFrontObjectStatus::OFF;
    MvRearLeftRadarWave2Status mMvRearLeftRadarWave2Status = MvRearLeftRadarWave2Status::OFF;
    MvRearRightRadarWave2Status mMvRearRightRadarWave2Status = MvRearRightRadarWave2Status::OFF;
    IslaSpeedLimitTrafficSignStatus mIslaSpeedLimitTrafficSignStatus = IslaSpeedLimitTrafficSignStatus::OFF;
    IslaTrafficSignCountryInformationStatus mIslaTrafficSignCountryInformationStatus =
        IslaTrafficSignCountryInformationStatus::INVALID;
    PuFGroup7DriverAssistFailure1Status mPuFGroup7DriverAssistFailure1Status = PuFGroup7DriverAssistFailure1Status::OFF;
    PuFGroup7OutsideMirrorSymbolFailureStatus mPuFGroup7OutsideMirrorSymbolFailureStatus =
        PuFGroup7OutsideMirrorSymbolFailureStatus::OFF;
    IslaWarningSoundSymbolStatus mIslaWarningSoundSymbolStatus = IslaWarningSoundSymbolStatus::OFF;
    IslaAdditionalTrafficSignStatus mIslaAdditionalTrafficSignStatus = IslaAdditionalTrafficSignStatus::OFF;
    IslaSupplementaryTrafficSignStatus mIslaSupplementaryTrafficSignStatus = IslaSupplementaryTrafficSignStatus::OFF;
    PuFGroup1AdasWarning1_1Status mPuFGroup1AdasWarning1_1Status = PuFGroup1AdasWarning1_1Status::OFF;
    PuFGroup1AdasWarning1_2Status mPuFGroup1AdasWarning1_2Status = PuFGroup1AdasWarning1_2Status::OFF;
    PuFGroup1AdasWarningA_AStatus mPuFGroup1AdasWarningA_AStatus = PuFGroup1AdasWarningA_AStatus::OFF;
    PuFGroup4AdasWarning1_1Status mPuFGroup4AdasWarning1_1Status = PuFGroup4AdasWarning1_1Status::OFF;
    PuFGroup7ForwardSafetyFailureStatus mPuFGroup7ForwardSafetyFailureStatus = PuFGroup7ForwardSafetyFailureStatus::OFF;
    PuFGroup7FCWNearFailureStatus mPuFGroup7FCWNearFailureStatus = PuFGroup7FCWNearFailureStatus::OFF;
    PuFGroup7LaneSafetyFailureStatus mPuFGroup7LaneSafetyFailureStatus = PuFGroup7LaneSafetyFailureStatus::OFF;
    PuFGroup7BlindSpotSafetyFailureStatus mPuFGroup7BlindSpotSafetyFailureStatus = PuFGroup7BlindSpotSafetyFailureStatus::OFF;
    PuFGroup7IslaFailureStatus mPuFGroup7IslaFailureStatus = PuFGroup7IslaFailureStatus::OFF;
    PuFGroup7DawFailureStatus mPuFGroup7DawFailureStatus = PuFGroup7DawFailureStatus::OFF;
    PuFGroup7HbaFailureStatus mPuFGroup7HbaFailureStatus = PuFGroup7HbaFailureStatus::OFF;
    PuFGroup7SccFailureStatus mPuFGroup7SccFailureStatus = PuFGroup7SccFailureStatus::OFF;
    PuFGroup7LfaFailureStatus mPuFGroup7LfaFailureStatus = PuFGroup7LfaFailureStatus::OFF;
    PuFGroup7HdaFailureStatus mPuFGroup7HdaFailureStatus = PuFGroup7HdaFailureStatus::OFF;
    PuFGroup7MRMFailureStatus mPuFGroup7MRMFailureStatus = PuFGroup7MRMFailureStatus::OFF;
    PuMGroup2AdasWarning1_1Status mPuMGroup2AdasWarning1_1Status = PuMGroup2AdasWarning1_1Status::OFF;
    MvFrontLeftRadarWaveStatus mMvFrontLeftRadarWaveStatus = MvFrontLeftRadarWaveStatus::OFF;
    MvFrontRightRadarWaveStatus mMvFrontRightRadarWaveStatus = MvFrontRightRadarWaveStatus::OFF;
    MvFrontRadarWaveStatus mMvFrontRadarWaveStatus = MvFrontRadarWaveStatus::OFF;

    InterTelltaleIccDrow mInterTelltaleIccDrow = InterTelltaleIccDrow::OFF;

    SoundAdasWarning1_1Status mSoundAdasWarning1_1Status = SoundAdasWarning1_1Status::OFF;
    SoundAdasWarning1_2Status mSoundAdasWarning1_2Status = SoundAdasWarning1_2Status::OFF;
    SoundAdasWarning1_3Status mSoundAdasWarning1_3Status = SoundAdasWarning1_3Status::OFF;
    SoundAdasWarning1_4Status mSoundAdasWarning1_4Status = SoundAdasWarning1_4Status::OFF;
    SoundAdasWarning1_5Status mSoundAdasWarning1_5Status = SoundAdasWarning1_5Status::OFF;
    SoundAdasWarning2_1Status mSoundAdasWarning2_1Status = SoundAdasWarning2_1Status::OFF;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;

    HUInt64 mSmvSetSpeedValue = 0;
    HUInt64 mMvLeftLineOffsetValue = 0;
    HUInt64 mMvRightLineOffsetValue = 0;
    HUInt64 mMvVehicleDistanceValue = 0;
    HUInt64 mMvFrontObjectLateralPositionValue = 0;
    HUInt64 mMvFrontObjectLongitudinalPositionValue = 0;
    HUInt64 mIslaSpeedLimitTrafficSignValue = 0;

    SFCADAS_Driving_NewConstantFrontVehicleColorStat mSummaryFrontVehicleStat =
        SFCADAS_Driving_NewConstantFrontVehicleColorStat::OFF;
    SFCADAS_Driving_NewConstantFrontVehicleLevelValue mSummaryFrontVehicleValue =
        SFCADAS_Driving_NewConstantFrontVehicleLevelValue::OFF;
    SFCADAS_Driving_NewConstantHeadwayDistanceColorStat mSummaryHeadwayDistanceSetStat =
        SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::OFF;
    SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue mSummaryHeadwayDistanceSetVal =
        SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::OFF;
    SFCADAS_Driving_NewConstantSubVehicleColorStat mSummarySubVehicleStat = SFCADAS_Driving_NewConstantSubVehicleColorStat::OFF;
    SFCADAS_Driving_NewConstantSetSpeedColorStat mSummarySetSpeedStat = SFCADAS_Driving_NewConstantSetSpeedColorStat::OFF;
    SFCADAS_Driving_NewConstantHDAHDPStat mSummaryHDAHDPTelltaleStat = SFCADAS_Driving_NewConstantHDAHDPStat::OFF;
    SFCADAS_Driving_NewConstantISLAManualSetSpeedStat mSummaryISLAManualSetSpeedStat =
        SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::OFF;
    SFCADAS_Driving_NewConstantNSCCAutoSetStat mSummaryNSCCAutoSetTelltaleStat = SFCADAS_Driving_NewConstantNSCCAutoSetStat::OFF;

    ssfs::SFCTimer<ADAS_Driving_CV> mOneShotTimerPriorityISLA;
    ssfs::SFCTimer<ADAS_Driving_CV> mOneShotTimerPriorityDIS;
    static constexpr uint32_t kTimerInterval300ms = 300u;
};

}  // namespace ccos

#endif  // SFSS_ADAS_Driving_CV_H
