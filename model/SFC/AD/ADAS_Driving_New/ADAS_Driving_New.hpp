/**
 * @file ADAS_Driving_New.hpp
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
#ifndef SFSS_ADAS_Driving_New_H
#define SFSS_ADAS_Driving_New_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include <Vehicle.h>
#include "ADAS_Driving_NewBase.hpp"
#include <SFCTimer.h>
#include <tuple>

namespace ccos {

// SFC Version : 67.0.0
// Reference : [AD080] ADAS_DRIVING_NEW. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_Driving_New : public ADAS_Driving_NewBase {
public:
    ADAS_Driving_New() = default;
    ~ADAS_Driving_New() override = default;
    ADAS_Driving_New(const ADAS_Driving_New& other) = delete;
    ADAS_Driving_New(ADAS_Driving_New&& other) noexcept = delete;
    ADAS_Driving_New& operator=(const ADAS_Driving_New& other) = delete;
    ADAS_Driving_New& operator=(ADAS_Driving_New&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1 주행 보조 요약 정보
        setSFCADAS_Driving_NewEventDrivingAssistSummaryID("E20207");

        // 5.3.4 1그룹 풀 팝업 2-1 (RR_C_RDR, ADAS_DRV)
        setSFCADAS_Driving_NewEventGroup1FullPopup2_1ID("E52030");
        setSFCADAS_Driving_NewEventGroup1FullPopup2_1LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventGroup1FullPopup2_1LinkedSoundType(
            SFCADAS_Driving_NewEventGroup1FullPopup2_1LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventGroup1FullPopup2_1LinkedSoundRepeatCount(1);

        // 5.3.5 1그룹 풀 팝업 3-1 (RDD)
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099ID("E52099");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099LinkedSoundID("SND_PopUpWarn3");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099LinkedSoundType(
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099LinkedSoundType::INFINITE);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52100ID("E52100");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101ID("E52101");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101LinkedSoundType(
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101LinkedSoundRepeatCount(1);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105ID("E52105");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105LinkedSoundID("SND_PopUpWarn3");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105LinkedSoundType(
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105LinkedSoundType::INFINITE);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52106ID("E52106");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107ID("E52107");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107LinkedSoundType(
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107LinkedSoundRepeatCount(1);

        // 5.3.8 7그룹 풀 팝업 – 사이드 미러 고장
        setSFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureID("E52054");
        setSFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundType(
            SFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundRepeatCount(1);

        // 5.3.9 7 그룹 풀 팝업 – 고속도로 자율주행 고장
        setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureLinkedSoundType(
            SFCADAS_Driving_NewEventGroup7FullPopupHdpFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureLinkedSoundRepeatCount(1);

        // 5.3.10 7그룹 풀 팝업 – 운전자 보조 시스템 고장 1
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundType(
            SFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventDriverAssistFailure1LinkedSoundRepeatCount(1);

        // 5.3.11 7그룹 풀 팝업 – 운전자 보조 시스템 고장 2
        setSFCADAS_Driving_NewEventDriverAssistFailure2LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_Driving_NewEventDriverAssistFailure2LinkedSoundType(
            SFCADAS_Driving_NewEventDriverAssistFailure2LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_Driving_NewEventDriverAssistFailure2LinkedSoundRepeatCount(1);

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

        if (mOneShotTimerPriorityISLA.create(kTimerInterval300ms, this, &ADAS_Driving_New::onOneShotTimerISLAChanged, false) ==
            false) {
            DWarning() << "[ADAS_Driving_New] mOneShotTimerPriorityISLA Create Failed";
        }

        if (mOneShotTimerPriorityDIS.create(kTimerInterval300ms, this, &ADAS_Driving_New::onOneShotTimerDISChanged, false) ==
            false) {
            DWarning() << "[ADAS_Driving_New] mOneShotTimerPriorityDIS Create Failed";
        }

        setSFCADAS_Driving_NewEventHDPOperProhibitID("E52643");

#ifdef ENABLE_GCOV_ON
        mOneShotTimerPriorityDIS.setTimerStatusCallback(&ADAS_Driving_New::updateOneShotTimerPriorityDISStatus);
        mOneShotTimerPriorityISLA.setTimerStatusCallback(&ADAS_Driving_New::updateOneShotTimerPriorityISLAStatus);
#endif
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

    void onConfigFca2Changed(const ArgumentsConfigFca2Changed& args) {
        mConfigAdasFCA2 = args.mInter_ConfigAdasFCA2;
        updateTelltaleFCA();
        updateTelltaleELK();
        updateConstantForwardSafetyFailure();
        updateConstantForwardSideSafetyFailure();
    }

    void onConfigLkaChanged(const ArgumentsConfigLkaChanged& args) {
        mConfigAdasLKA = args.mInter_ConfigAdasLKA;
        updateTelltaleLKA();
        updateConstantLaneSafetyFailure();
        updateEventGroup7FullPopup1();
    }

    void onConfigDawChanged(const ArgumentsConfigDawChanged& args) {
        mConfigAdasDAW = args.mInter_ConfigAdasDAW;
        updateTelltaleDAW();
        updateConstantDawFailure();
    }

    void onConfigIlcuIfsChanged(const ArgumentsConfigIlcuIfsChanged& args) {
        mConfigILCU = args.mInter_ConfigILCU;
        mConfigIFS = args.mInter_ConfigIFS;
        updateTelltaleHBA();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateConstantISLAMainSpeedStat();
    }

    void onConfigIslaChanged(const ArgumentsConfigIslaChanged& args) {
        mConfigAdasISLA = args.mInter_ConfigAdasISLA;
        updateConstantIslaFailure();
        updateConstantISLAMainSpeedStat();
    }

    void onConfigBcwChanged(const ArgumentsConfigBcwChanged& args) {
        mConfigNewBCW = args.mInter_ConfigNewBCW;
        updateConstantBlindSpotSafetyFailure();
        updateEventGroup7FullPopup2();
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

    void onConfigHda2Changed(const ArgumentsConfigHda2Changed& args) {
        mConfigAdasHDA2 = args.mInter_ConfigAdasHDA2;
        updateConstantHdaFailure();
        updateConstantLcaFailure();
    }

    void onConfigHda3Changed(const ArgumentsConfigHda3Changed& args) {
        mConfigAdasHDA3 = args.mInter_ConfigAdasHDA3;
        updateConstantHda3HandsFreeFailure();
    }

    void onConfigHdpChanged(const ArgumentsConfigHdpChanged& args) {
        mConfigAdasHDP = args.mInter_ConfigAdasHDP;
        updateConstantHdaFailure();
        updateConstantLcaFailure();
        updateEventGroup7FullPopupHdpFailure();
        updateConstantHda3HandsFreeFailure();
    }

    void onConfigEmergencyStopChanged(const ArgumentsConfigEmergencyStopChanged& args) {
        mConfigAdasEmergencyStop = args.mInter_ConfigAdasEmergencyStop;
        updateConstantEmergencyStopFailure();
    }

    void onConfigHDPNameChanged(const ArgumentsConfigHDPNameChanged& args) {
        mConfigHDPName = args.mInter_ConfigHDPName;
        updateConstantHDAHDPStat();
        updateConstantHDPProductDivisionAdditionalInformation();
        updateConstantSubVehicleColorStat();
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

    void onEmergencySteeringSymbolStatusChanged(const ArgumentsEmergencySteeringSymbolStatusChanged& args) {
        mEmergencySteeringSymbolStatus = args.mInput_EmergencySteeringSymbolStatus;
        if (ISTIMEOUT(args.mInput_EmergencySteeringSymbolStatus) == true) {
            mEmergencySteeringSymbolStatus = EmergencySteeringSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleELK();
    }

    void onLaneSafetySymbolStatusChanged(const ArgumentsLaneSafetySymbolStatusChanged& args) {
        mLaneSafetySymbolStatus = args.mInput_LaneSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_LaneSafetySymbolStatus) == true) {
            mLaneSafetySymbolStatus = LaneSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleLKA();
    }

    void onHbaSymbolStatusChanged(const ArgumentsHbaSymbolStatusChanged& args) {
        mHbaSymbolStatus = args.mInput_HbaSymbolStatus;
        updateTelltaleHBA();
    }

    void onSmvADASSummeryInfoStatusChanged(const ArgumentsSmvADASSummeryInfoStatusChanged& args) {
        mSmvHostVehicleStatus = args.mInput_SmvHostVehicleStatus;
        mSmvFrontObjectStatus = args.mInput_SmvFrontObjectStatus;
        mSmvNsccSymbol2Status = args.mInput_SmvNsccSymbol2Status;
        mSmvVehicleDistanceLevelValue = args.mInput_SmvVehicleDistanceLevelValue;
        mSmvVehicleDistanceLevelStatus = args.mInput_SmvVehicleDistanceLevelStatus;
        mSmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        mSmvSetSpeedValue = args.mInput_SmvSetSpeedValue;
        mSmvHdaSymbolStatus = args.mInput_SmvHdaSymbolStatus;
        mSmvIslaSetSpeedSymbolStatus = args.mInput_SmvIslaSetSpeedSymbolStatus;
        mSmvNsccSymbolStatus = args.mInput_SmvNsccSymbolStatus;
        updateConstantSubVehicleColorStat();
        updateAdasViewFixStatus();
        updateConstantFrontVehicleColorStat();
        updateConstantFrontVehicleLevelValue();
        updateConstantHeadywayDistanceLevelValue();
        updateConstantHeadywayDistanceColorStat();
        updateConstantSetSpeedColorStat();
        updateConstantSetSpeedStat();
        updateConstantHDAHDPStat();
        updateConstantISLAManualSetSpeedStat();
        updateConstantNSCCAutoSetStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvLfaSymbolStatusChanged(const ArgumentsSmvLfaSymbolStatusChanged& args) {
        mSmvLfaSymbolStatus = args.mInput_SmvLfaSymbolStatus;
        updateTelltaleLFAStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvLcaLeftSymbolStatusChanged(const ArgumentsSmvLcaLeftSymbolStatusChanged& args) {
        mSmvLcaLeftSymbolStatus = args.mInput_SmvLcaLeftSymbolStatus;
        updateTelltaleLCALeftStat();
        updateEventDrivingAssistSummary();
    }

    void onSmvLcaRightSymbolStatusChanged(const ArgumentsSmvLcaRightSymbolStatusChanged& args) {
        mSmvLcaRightSymbolStatus = args.mInput_SmvLcaRightSymbolStatus;
        updateTelltaleLCARightStat();
        updateEventDrivingAssistSummary();
    }

    void onDriverHandsOnOffStatusStatusChanged(const ArgumentsDriverHandsOnOffStatusStatusChanged& args) {
        mDriverHandsOnOffStatus = args.mInput_MvDriverHandsOnOffStatus;
        updateHandsOnOffTelltaleStatus();
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
        updateConstantCenterRoadLeftLatPosStat();
        updateConstantLeftRoadLatPosStat();
    }

    void onMvDrivingLaneRadiusValueChanged(const ArgumentsMvDrivingLaneRadiusValueChanged& args) {
        mMvDrivingLaneRadiusValue = args.mInput_MvDrivingLaneRadiusValue;
        if (ISTIMEOUT(args.mInput_MvDrivingLaneRadiusValue) == true) {
            mMvDrivingLaneRadiusValue = kTimeOut;
        }
        updateConstantLaneRoadCvrtLvlStat();
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
        updateConstantCenterRoadRightLatPosStat();
        updateConstantRightRoadLatPosStat();
    }

    void onMvVehicleDistanceStatusChanged(const ArgumentsMvVehicleDistanceStatusChanged& args) {
        mMvVehicleDistanceStatus = args.mInput_MvVehicleDistanceStatus;
        updateConstantHeadwayDistColorStat();
    }

    void onMvVehicleDistanceValueChanged(const ArgumentsMvVehicleDistanceValueChanged& args) {
        mMvVehicleDistanceValue = args.mInput_MvVehicleDistanceValue;
        updateConstantHeadwayDistLongPosStat();
    }

    void onMvCenterRoadSurfaceStatusChanged(const ArgumentsMvCenterRoadSurfaceStatusChanged& args) {
        mMvCenterRoadSurfaceStatus = args.mInput_MvCenterRoadSurfaceStatus;
        updateConstantCenterRoadColorStat();
    }

    void onMvCenterRoadSurfaceDistanceValueChanged(const ArgumentsMvCenterRoadSurfaceDistanceValueChanged& args) {
        mMvCenterRoadSurfaceDistanceValue = args.mInput_MvCenterRoadSurfaceDistanceValue;
        updateConstantCenterRoadLongPosStat();
    }

    void onMvLeftRoadSurfaceStatusChanged(const ArgumentsMvLeftRoadSurfaceStatusChanged& args) {
        mMvLeftRoadSurfaceStatus = args.mInput_MvLeftRoadSurfaceStatus;
        updateConstantLeftRoadColorStat();
    }

    void onMvRightRoadSurfaceStatusChanged(const ArgumentsMvRightRoadSurfaceStatusChanged& args) {
        mMvRightRoadSurfaceStatus = args.mInput_MvRightRoadSurfaceStatus;
        updateConstantRightRoadColorStat();
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

    void onMvFrontLeftObjectStatusChanged(const ArgumentsMvFrontLeftObjectStatusChanged& args) {
        mMvFrontLeftObjectStatus = args.mInput_MvFrontLeftObjectStatus;
        updateConstantViewFrontLeftVehicleColorStat();
    }

    void onMvFrontLeftObjectLateralPositionValueChanged(const ArgumentsMvFrontLeftObjectLateralPositionValueChanged& args) {
        mMvFrontLeftObjectLateralPositionValue = args.mInput_MvFrontLeftObjectLateralPositionValue;
        updateConstantViewFrontLeftVehicleLatPosStat();
    }

    void onMvFrontLeftObjectLongitudinalPositionValueChanged(
        const ArgumentsMvFrontLeftObjectLongitudinalPositionValueChanged& args) {
        mMvFrontLeftObjectLongitudinalPositionValue = args.mInput_MvFrontLeftObjectLongitudinalPositionValue;
        updateConstantViewFrontLeftVehicleLongPosStat();
    }

    void onMvFrontRightObjectStatusChanged(const ArgumentsMvFrontRightObjectStatusChanged& args) {
        mMvFrontRightObjectStatus = args.mInput_MvFrontRightObjectStatus;
        updateConstantViewFrontRightVehicleColorStat();
    }

    void onMvFrontRightObjectLateralPositionValueChanged(const ArgumentsMvFrontRightObjectLateralPositionValueChanged& args) {
        mMvFrontRightObjectLateralPositionValue = args.mInput_MvFrontRightObjectLateralPositionValue;
        updateConstantViewFrontRightVehicleLatPosStat();
    }

    void onMvFrontRightObjectLongitudinalPositionValueChanged(
        const ArgumentsMvFrontRightObjectLongitudinalPositionValueChanged& args) {
        mMvFrontRightObjectLongitudinalPositionValue = args.mInput_MvFrontRightObjectLongitudinalPositionValue;
        updateConstantViewFrontRightVehicleLongPosStat();
    }

    void onMvLeftObjectStatusChanged(const ArgumentsMvLeftObjectStatusChanged& args) {
        mMvLeftObjectStatus = args.mInput_MvLeftObjectStatus;
        updateConstantViewLeftVehicleColorStat();
    }

    void onMvLeftObjectLateralPositionValueChanged(const ArgumentsMvLeftObjectLateralPositionValueChanged& args) {
        mMvLeftObjectLateralPositionValue = args.mInput_MvLeftObjectLateralPositionValue;
        updateConstantViewLeftVehicleLatPosStat();
    }

    void onMvLeftObjectLongitudinalPositionValueChanged(const ArgumentsMvLeftObjectLongitudinalPositionValueChanged& args) {
        mMvLeftObjectLongitudinalPositionValue = args.mInput_MvLeftObjectLongitudinalPositionValue;
        updateConstantViewLeftVehicleLongPosStat();
    }

    void onMvRightObjectStatusChanged(const ArgumentsMvRightObjectStatusChanged& args) {
        mMvRightObjectStatus = args.mInput_MvRightObjectStatus;
        updateConstantViewRightVehicleColorStat();
    }

    void onMvRightObjectLateralPositionValueChanged(const ArgumentsMvRightObjectLateralPositionValueChanged& args) {
        mMvRightObjectLateralPositionValue = args.mInput_MvRightObjectLateralPositionValue;
        updateConstantViewRightVehicleLatPosStat();
    }

    void onMvRightObjectLongitudinalPositionValueChanged(const ArgumentsMvRightObjectLongitudinalPositionValueChanged& args) {
        mMvRightObjectLongitudinalPositionValue = args.mInput_MvRightObjectLongitudinalPositionValue;
        updateConstantViewRightVehicleLongPosStat();
    }

    void onMvRearLeftRadarWaveStatusChanged(const ArgumentsMvRearLeftRadarWaveStatusChanged& args) {
        mMvRearLeftRadarWave1Status = args.mInput_MvRearLeftRadarWave1Status;
        mMvRearLeftRadarWave2Status = args.mInput_MvRearLeftRadarWave2Status;
        updateConstantViewRearLeftWarnStat();
    }

    void onMvRearRightRadarWaveStatusChanged(const ArgumentsMvRearRightRadarWaveStatusChanged& args) {
        mMvRearRightRadarWave1Status = args.mInput_MvRearRightRadarWave1Status;
        mMvRearRightRadarWave2Status = args.mInput_MvRearRightRadarWave2Status;
        updateConstantViewRearRightWarnStat();
    }

    void onMvLeftLaneChangeDirectionStatusChanged(const ArgumentsMvLeftLaneChangeDirectionStatusChanged& args) {
        mMvLeftLaneChangeDirectionStatus = args.mInput_MvLeftLaneChangeDirectionStatus;
        updateConstantViewLeftArrowStat();
    }

    void onMvRightLaneChangeDirectionStatusChanged(const ArgumentsMvRightLaneChangeDirectionStatusChanged& args) {
        mMvRightLaneChangeDirectionStatus = args.mInput_MvRightLaneChangeDirectionStatus;
        updateConstantViewRightArrowStat();
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
        updateConstantISLAMainSignStat();
        updateConstantISLAMainSpeedStat();
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
        if (ISTIMEOUT(args.mInput_PuFGroup1AdasWarning1_2Status) == true) {
            mPuFGroup1AdasWarning1_2Status = PuFGroup1AdasWarning1_2Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup1FullPopup1_2();
    }

    void onPuFGroup1AdasWarning2_1StatusChanged(const ArgumentsPuFGroup1AdasWarning2_1StatusChanged& args) {
        mPuFGroup1AdasWarning2_1Status = args.mInput_PuFGroup1AdasWarning2_1Status;
        updateEventGroup1FullPopup2_1();
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
        updateEventGroup7FullPopupBlindSpotSafetyFailure();
        updateConstantBlindSpotSafetyFailure();
    }

    void onPuFGroup7OutsideMirrorSymbolFailureStatusChanged(
        const ArgumentsPuFGroup7OutsideMirrorSymbolFailureStatusChanged& args) {
        mPuFGroup7OutsideMirrorSymbolFailureStatus = args.mInput_PuFGroup7OutsideMirrorSymbolFailureStatus;
        updateEventGroup7FullPopupOutsideMirrorSymbolFailure();
    }

    void onPuMGroup2AdasWarning1_1StatusChanged(const ArgumentsPuMGroup2AdasWarning1_1StatusChanged& args) {
        mPuMGroup2AdasWarning1_1Status = args.mInput_PuMGroup2AdasWarning1_1Status;
        updateEventGroup2MiniPopup1_1();
    }

    void onMvHostVehicleStatusChanged(const ArgumentsMvHostVehicleStatusChanged& args) {
        mMvHostVehicle1Status = args.mInput_MvHostVehicle1Status;
        mMvHostVehicle2Status = args.mInput_MvHostVehicle2Status;
        updateAdasOnStatus();
    }

    void onIlcuActWarnStatusChanged(const ArgumentsIlcuActWarnStatusChanged& args) {
        mIlcuLhActWarnStatus = args.mInput_IlcuLhActWarnStatus;
        mIlcuRhActWarnStatus = args.mInput_IlcuRhActWarnStatus;
        updateTelltaleHBA();
    }

    void onIfsIndiReqStatusChanged(const ArgumentsIfsIndiReqStatusChanged& args) {
        mIfsIndiReqStatus = args.mInput_IfsIndiReqStatus;
        updateTelltaleHBA();
    }

    void onPuFGroup7HdpFailureStatusChanged(const ArgumentsPuFGroup7HdpFailureStatusChanged& args) {
        mPuFGroup7HdpFailureStatus = args.mInput_PuFGroup7HdpFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7HdpFailureStatus) == true) {
            mPuFGroup7HdpFailureStatus = PuFGroup7HdpFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopupHdpFailure();
    }

    void onMvFrontRadarWaveStatusChanged(const ArgumentsMvFrontRadarWaveStatusChanged& args) {
        mMvFrontRadarWaveStatus = args.mInput_MvFrontRadarWaveStatus;
        updateConstantViewFrontWarnStat();
    }

    void onIslaTrafficSignCountryInformationStatusChanged(const ArgumentsIslaTrafficSignCountryInformationStatusChanged& args) {
        mIslaTrafficSignCountryInformationStatus = args.mInput_IslaTrafficSignCountryInformationStatus;
        updateConstantISLAMainSpeedStat();
        updatePrivateISLAAddtnlSign();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
    }

    void onPuFGroup7DriverAssistFailure1StatusChanged(const ArgumentsPuFGroup7DriverAssistFailure1StatusChanged& args) {
        mPuFGroup7DriverAssistFailure1Status = args.mInput_PuFGroup7DriverAssistFailure1Status;
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure1Status) == true) {
            mPuFGroup7DriverAssistFailure1Status = PuFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopup1();
    }

    void onPuFGroup7DriverAssistFailure2StatusChanged(const ArgumentsPuFGroup7DriverAssistFailure2StatusChanged& args) {
        mPuFGroup7DriverAssistFailure2Status = args.mInput_PuFGroup7DriverAssistFailure2Status;
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure2Status) == true) {
            mPuFGroup7DriverAssistFailure2Status = PuFGroup7DriverAssistFailure2Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopup2();
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
        mIslaOption2Status = args.mInput_IslaOption2Status;
        mAdasOverSpeedWarnOnOffStatus = args.mInput_AdasOverSpeedWarnOnOffStatus;

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

    void onPuMGroup2AdasWarning1_2StatusChanged(const ArgumentsPuMGroup2AdasWarning1_2StatusChanged& args) {
        mPuMGroup2AdasWarning1_2Status = args.mInput_PuMGroup2AdasWarning1_2Status;
        updateEventGroup2MiniPopup1_2();
    }

    void onBgHDPStatusChanged(const ArgumentsBgHDPStatusChanged& args) {
        mBGHDPStatus = args.mInput_BGHDPStatus;
        updateHDPMasterWarningStatus();
        updateConstantHDPBackGroundOutlineStat();
        updateConstantHDPBackGroundStat();
    }

    void onDriveAttentionWarningSymbolStatusChanged(const ArgumentsDriveAttentionWarningSymbolStatusChanged& args) {
        mDriveAttentionWarningSymbolStatus = args.mInput_DriveAttentionWarningSymbolStatus;
        if (ISTIMEOUT(args.mInput_DriveAttentionWarningSymbolStatus) == true) {
            mDriveAttentionWarningSymbolStatus = DriveAttentionWarningSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleDAW();
    }

    void onPuFGroup7ForwardSafetyFailureStatusChanged(const ArgumentsPuFGroup7ForwardSafetyFailureStatusChanged& args) {
        mPuFGroup7ForwardSafetyFailureStatus = args.mInput_PuFGroup7ForwardSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7ForwardSafetyFailureStatus) == true) {
            mPuFGroup7ForwardSafetyFailureStatus = PuFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantForwardSafetyFailure();
    }

    void onPuFGroup7ForwardSideSafetyFailureStatusChanged(const ArgumentsPuFGroup7ForwardSideSafetyFailureStatusChanged& args) {
        mPuFGroup7ForwardSideSafetyFailureStatus = args.mInput_PuFGroup7ForwardSideSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7ForwardSideSafetyFailureStatus) == true) {
            mPuFGroup7ForwardSideSafetyFailureStatus = PuFGroup7ForwardSideSafetyFailureStatus::UNHANDLED_TIMEOUT;
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

    void onPuFGroup7LcaFailureStatusChanged(const ArgumentsPuFGroup7LcaFailureStatusChanged& args) {
        mPuFGroup7LcaFailureStatus = args.mInput_PuFGroup7LcaFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7LcaFailureStatus) == true) {
            mPuFGroup7LcaFailureStatus = PuFGroup7LcaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLcaFailure();
    }

    void onHdpRDDWarningStateChanged(const ArgumentsHdpRDDWarningStateChanged& args) {
        mHDP_RDDWarningState = args.mInput_HDP_RDDWarningState;
        updatePrivateHDPRedundancyStatus();
        updateAdasViewFixStatus();
        updateEventGroup1FullPopup3_1();
    }

    void onEscHDPEngagedStatusChanged(const ArgumentsEscHDPEngagedStatusChanged& args) {
        mESC_HDPEngagedStatus = args.mInput_ESC_HDPEngagedStatus;
        updatePrivateHDPRedundancyStatus();
        updateAdasViewFixStatus();
        updateEventGroup1FullPopup3_1();
        mPrevESC_HDPEngagedStatus = mESC_HDPEngagedStatus;
    }

    void onRcuHDPEngagedStatusChanged(const ArgumentsRcuHDPEngagedStatusChanged& args) {
        mRCU_HDPEngagedStatus = args.mInput_RCU_HDPEngagedStatus;
        updatePrivateHDPRedundancyStatus();
        updateAdasViewFixStatus();
        updateEventGroup1FullPopup3_1();
        mPrevRCU_HDPEngagedStatus = mRCU_HDPEngagedStatus;
    }

    void onHdpFaultStatusChanged(const ArgumentsHdpFaultStatusChanged& args) {
        mESC_HDPFaultStatus = args.mInput_ESC_HDPFaultStatus;
        mRCU_HDPFaultStatus = args.mInput_RCU_HDPFaultStatus;
        updateEventGroup7FullPopupHdpFailure();
    }

    void onInterTelltaleIccDrowChanged(const ArgumentsInterTelltaleIccDrowChanged& args) {
        mInterTelltaleIccDrow = args.mInter_TelltaleIccDrow;
        updateTelltaleDAW();
    }

    void onBrakeLampControlCommand_StatusChanged(const ArgumentsBrakeLampControlCommand_StatusChanged& args) {
        mBrakeLampControlCommand_Status = args.mInput_BrakeLampControlCommand_Status;
        updateConstantMainBrakeLight();
    }

    void onPuFGroup7MRMFailureStatusChanged(const ArgumentsPuFGroup7MRMFailureStatusChanged& args) {
        mPuFGroup7MRMFailureStatus = args.mInput_PuFGroup7MRMFailureStatus;
        if (ISTIMEOUT(args.mInput_PuFGroup7MRMFailureStatus) == true) {
            mPuFGroup7MRMFailureStatus = PuFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantEmergencyStopFailure();
    }

    void onMvCircleAroundHostVehicleStatusChanged(const ArgumentsMvCircleAroundHostVehicleStatusChanged& args) {
        mMvCircleAroundHostVehicleStatus = args.mInput_MVCircleAroundHostVehicleStatus;
        updateConstantMainVehicleStat();
    }

    void onMvFrontObstacleStatusChanged(const ArgumentsMvFrontObstacleStatusChanged& args) {
        mMvFrontObstacleStatus = args.mInput_MvFrontObstacleStatus;
        updateConstantViewFrontObstacleColorStat();
    }

    void onMvFrontObstacleLateralPositionValueChanged(const ArgumentsMvFrontObstacleLateralPositionValueChanged& args) {
        mMvFrontObstacleLateralPositionValue = args.mInput_MvFrontObstacleLateralPositionValue;
        updateConstantViewFrontObstacleLatPosStat();
    }

    void onMvFrontObstacleLongitudinalPositionValueChanged(const ArgumentsMvFrontObstacleLongitudinalPositionValueChanged& args) {
        mMvFrontObstacleLongitudinalPositionValue = args.mInput_MvFrontObstacleLongitudinalPositionValue;
        updateConstantViewFrontObstacleLongPosStat();
    }

    void onHmiMenuSwitchStatusValueChanged(const ArgumentsHmiMenuSwitchStatusValueChanged& args) {
        mHmiMenuSwitchStatus = args.mInter_HmiMenuSwitchStatus;
        updateEventHDPOperProhibit();
    }

    void onExtLampInputValueChanged(const ArgumentsExtLampInputValueChanged& args) {
        mExtTurnLeftLampStatus = args.mInput_ExtTurnLeftLampStatus;
        mExtTurnRightLampStatus = args.mInput_ExtTurnRightLampStatus;
        mExtLowBeamOnReqStatus = args.mInput_ExtLowBeamOnReqStatus;
        mExtHighBeamOnReqStatus = args.mInput_ExtHighBeamOnReqStatus;
        updateConstantExtLampStat();
    }

    void onIslaWarningSoundSymboleStatusChanged(const ArgumentsIslaWarningSoundSymboleStatusChanged& args) {
        mIslaWarningSoundSymboleStatus = args.mInput_IslaWarningSoundSymbolStatus;
        updateConsstantIslaSoundSymbolStat();
    }

    void onInfoDDTResidualDistanceValueChanged(const ArgumentsInfoDDTResidualDistanceValueChanged& args) {
        mInfoDDTResidualDistanceValue = args.mInput_InfoDDTResidualDistanceValue;
        updateConstantInfoDDTResidualDistanceValue();
    }

    void onMvLaneChangeRectangleStatusChanged(const ArgumentsMvLaneChangeRectangleStatusChanged& args) {
        mMvLaneChangeRectangleStatus = args.mInput_MvLaneChangeRectangleStatus;
        updateADASViewLaneChangeRectangleStat();
    }

    void onMvLaneChangeRectangleLateralPositionValueChanged(
        const ArgumentsMvLaneChangeRectangleLateralPositionValueChanged& args) {
        mMvLaneChangeRectangleLateralPositionValue = args.mInput_MvLaneChangeRectangleLateralPositionValue;
        updateADASViewLaneChangeRectangleLat();
        updateConstantViewLeftArrowLatValue();
        updateConstantViewRightArrowLatValue();
    }

    void onMvFrontObject01Changed(const ArgumentsMvFrontObject01Changed& args) {
        mMvFrontObject01Status = args.mInput_MvFrontObject01Status;
        mMvFrontObjectClassification01Status = args.mInput_MvFrontObjectClassification01Status;
        mMvFrontObjectLateralPosition01Value = args.mInput_MvFrontObjectLateralPosition01Value;
        mMvFrontObjectLongitudinalPosition01Value = args.mInput_MvFrontObjectLongitudinalPosition01Value;
        mMvFrontObjectHeadingAngle01Value = args.mInput_MvFrontObjectHeadingAngle01Value;
        updateConstantADASViewFrontObjectColor01Stat();
        updateConstantADASViewFrontObjectClass01Stat();
        updateConstantADASViewFrontObjectLat01();
        updateConstantADASViewFrontObjectLong01();
        updateConstantADASViewFrontObjectHeadingAngle01();
    }

    void onMvSurroundingObject01to05StatusChanged(const ArgumentsMvSurroundingObject01to05StatusChanged& args) {
        // SFCDEV-26: Precodition을 함께 확인하는 사양이기 때문에, 값이 변경되었을 경우에만 update를 수행함
        mMvSurroundingObject01Status = args.mInput_MvSurroundingObject01Status;
        mMvSurroundingObject02Status = args.mInput_MvSurroundingObject02Status;
        mMvSurroundingObject03Status = args.mInput_MvSurroundingObject03Status;
        mMvSurroundingObject04Status = args.mInput_MvSurroundingObject04Status;
        mMvSurroundingObject05Status = args.mInput_MvSurroundingObject05Status;
        if (mPrevMvSurroundingObject01Status != mMvSurroundingObject01Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject01Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor01Stat>();
            mPrevMvSurroundingObject01Status = mMvSurroundingObject01Status;
        }
        if (mPrevMvSurroundingObject02Status != mMvSurroundingObject02Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject02Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor02Stat>();
            mPrevMvSurroundingObject02Status = mMvSurroundingObject02Status;
        }
        if (mPrevMvSurroundingObject03Status != mMvSurroundingObject03Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject03Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor03Stat>();
            mPrevMvSurroundingObject03Status = mMvSurroundingObject03Status;
        }
        if (mPrevMvSurroundingObject04Status != mMvSurroundingObject04Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject04Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor04Stat>();
            mPrevMvSurroundingObject04Status = mMvSurroundingObject04Status;
        }
        if (mPrevMvSurroundingObject05Status != mMvSurroundingObject05Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject05Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor05Stat>();
            mPrevMvSurroundingObject05Status = mMvSurroundingObject05Status;
        }
    }
    void onMvSurroundingObject06to11StatusChanged(const ArgumentsMvSurroundingObject06to11StatusChanged& args) {
        // SFCDEV-26: Precodition을 함께 확인하는 사양이기 때문에, 값이 변경되었을 경우에만 update를 수행함
        mMvSurroundingObject06Status = args.mInput_MvSurroundingObject06Status;
        mMvSurroundingObject07Status = args.mInput_MvSurroundingObject07Status;
        mMvSurroundingObject08Status = args.mInput_MvSurroundingObject08Status;
        mMvSurroundingObject09Status = args.mInput_MvSurroundingObject09Status;
        mMvSurroundingObject10Status = args.mInput_MvSurroundingObject10Status;
        mMvSurroundingObject11Status = args.mInput_MvSurroundingObject11Status;
        if (mPrevMvSurroundingObject06Status != mMvSurroundingObject06Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject06Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor06Stat>();
            mPrevMvSurroundingObject06Status = mMvSurroundingObject06Status;
        }
        if (mPrevMvSurroundingObject07Status != mMvSurroundingObject07Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject07Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor07Stat>();
            mPrevMvSurroundingObject07Status = mMvSurroundingObject07Status;
        }
        if (mPrevMvSurroundingObject08Status != mMvSurroundingObject08Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject08Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor08Stat>();
            mPrevMvSurroundingObject08Status = mMvSurroundingObject08Status;
        }
        if (mPrevMvSurroundingObject09Status != mMvSurroundingObject09Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject09Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor09Stat>();
            mPrevMvSurroundingObject09Status = mMvSurroundingObject09Status;
        }
        if (mPrevMvSurroundingObject10Status != mMvSurroundingObject10Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject10Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor10Stat>();
            mPrevMvSurroundingObject10Status = mMvSurroundingObject10Status;
        }
        if (mPrevMvSurroundingObject11Status != mMvSurroundingObject11Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject11Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor11Stat>();
            mPrevMvSurroundingObject11Status = mMvSurroundingObject11Status;
        }
    }

    void onMvSurroundingObject12to14StatusChanged(const ArgumentsMvSurroundingObject12to14StatusChanged& args) {
        // SFCDEV-26: Precodition을 함께 확인하는 사양이기 때문에, 값이 변경되었을 경우에만 update를 수행함
        mMvSurroundingObject12Status = args.mInput_MvSurroundingObject12Status;
        mMvSurroundingObject13Status = args.mInput_MvSurroundingObject13Status;
        mMvSurroundingObject14Status = args.mInput_MvSurroundingObject14Status;
        if (mPrevMvSurroundingObject12Status != mMvSurroundingObject12Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject12Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor12Stat>();
            mPrevMvSurroundingObject12Status = mMvSurroundingObject12Status;
        }
        if (mPrevMvSurroundingObject13Status != mMvSurroundingObject13Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject13Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor13Stat>();
            mPrevMvSurroundingObject13Status = mMvSurroundingObject13Status;
        }
        if (mPrevMvSurroundingObject14Status != mMvSurroundingObject14Status) {
            updateADASViewSurroundingObjectColor<MvSurroundingObject14Status,
                                                 SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor14Stat>();
            mPrevMvSurroundingObject14Status = mMvSurroundingObject14Status;
        }
    }

    void onMvSurroundingObjectClassification01to05StatusChanged(
        const ArgumentsMvSurroundingObjectClassification01to05StatusChanged& args) {
        mMvSurroundingObjectClassification01Status = args.mInput_MvSurroundingObjectClassification01Status;
        mMvSurroundingObjectClassification02Status = args.mInput_MvSurroundingObjectClassification02Status;
        mMvSurroundingObjectClassification03Status = args.mInput_MvSurroundingObjectClassification03Status;
        mMvSurroundingObjectClassification04Status = args.mInput_MvSurroundingObjectClassification04Status;
        mMvSurroundingObjectClassification05Status = args.mInput_MvSurroundingObjectClassification05Status;
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification01Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass01Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification02Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass02Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification03Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass03Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification04Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass04Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification05Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass05Stat>();
    }

    void onMvSurroundingObjectClassification06to11StatusChanged(
        const ArgumentsMvSurroundingObjectClassification06to11StatusChanged& args) {
        mMvSurroundingObjectClassification06Status = args.mInput_MvSurroundingObjectClassification06Status;
        mMvSurroundingObjectClassification07Status = args.mInput_MvSurroundingObjectClassification07Status;
        mMvSurroundingObjectClassification08Status = args.mInput_MvSurroundingObjectClassification08Status;
        mMvSurroundingObjectClassification09Status = args.mInput_MvSurroundingObjectClassification09Status;
        mMvSurroundingObjectClassification10Status = args.mInput_MvSurroundingObjectClassification10Status;
        mMvSurroundingObjectClassification11Status = args.mInput_MvSurroundingObjectClassification11Status;
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification06Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass06Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification07Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass07Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification08Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass08Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification09Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass09Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification10Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass10Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification11Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass11Stat>();
    }

    void onMvSurroundingObjectClassification12to14StatusChanged(
        const ArgumentsMvSurroundingObjectClassification12to14StatusChanged& args) {
        mMvSurroundingObjectClassification12Status = args.mInput_MvSurroundingObjectClassification12Status;
        mMvSurroundingObjectClassification13Status = args.mInput_MvSurroundingObjectClassification13Status;
        mMvSurroundingObjectClassification14Status = args.mInput_MvSurroundingObjectClassification14Status;
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification12Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass12Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification13Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass13Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification14Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass14Stat>();
    }

    void onMvSurroundingObjectHeadingAngle01to05ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle01to05ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle01Value = args.mInput_MvSurroundingObjectHeadingAngle01Value;
        mMvSurroundingObjectHeadingAngle02Value = args.mInput_MvSurroundingObjectHeadingAngle02Value;
        mMvSurroundingObjectHeadingAngle03Value = args.mInput_MvSurroundingObjectHeadingAngle03Value;
        mMvSurroundingObjectHeadingAngle04Value = args.mInput_MvSurroundingObjectHeadingAngle04Value;
        mMvSurroundingObjectHeadingAngle05Value = args.mInput_MvSurroundingObjectHeadingAngle05Value;
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle02Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle03Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle04Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle05Stat>();
    }

    void onMvSurroundingObjectHeadingAngle06to11ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle06to11ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle06Value = args.mInput_MvSurroundingObjectHeadingAngle06Value;
        mMvSurroundingObjectHeadingAngle07Value = args.mInput_MvSurroundingObjectHeadingAngle07Value;
        mMvSurroundingObjectHeadingAngle08Value = args.mInput_MvSurroundingObjectHeadingAngle08Value;
        mMvSurroundingObjectHeadingAngle09Value = args.mInput_MvSurroundingObjectHeadingAngle09Value;
        mMvSurroundingObjectHeadingAngle10Value = args.mInput_MvSurroundingObjectHeadingAngle10Value;
        mMvSurroundingObjectHeadingAngle11Value = args.mInput_MvSurroundingObjectHeadingAngle11Value;
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle06Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle07Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle08Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle09Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle10Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle11Stat>();
    }

    void onMvSurroundingObjectHeadingAngle12to14ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle12to14ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle12Value = args.mInput_MvSurroundingObjectHeadingAngle12Value;
        mMvSurroundingObjectHeadingAngle13Value = args.mInput_MvSurroundingObjectHeadingAngle13Value;
        mMvSurroundingObjectHeadingAngle14Value = args.mInput_MvSurroundingObjectHeadingAngle14Value;
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle12Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle13Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Stat>();
    }

    void onMvSurroundingObjectLateralPosition01to05ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition01to05ValueChanged& args) {
        mMvSurroundingObjectLateralPosition01Value = args.mInput_MvSurroundingObjectLateralPosition01Value;
        mMvSurroundingObjectLateralPosition02Value = args.mInput_MvSurroundingObjectLateralPosition02Value;
        mMvSurroundingObjectLateralPosition03Value = args.mInput_MvSurroundingObjectLateralPosition03Value;
        mMvSurroundingObjectLateralPosition04Value = args.mInput_MvSurroundingObjectLateralPosition04Value;
        mMvSurroundingObjectLateralPosition05Value = args.mInput_MvSurroundingObjectLateralPosition05Value;
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat02Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat03Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat04Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat05Stat>();
    }

    void onMvSurroundingObjectLateralPosition06to11ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition06to11ValueChanged& args) {
        mMvSurroundingObjectLateralPosition06Value = args.mInput_MvSurroundingObjectLateralPosition06Value;
        mMvSurroundingObjectLateralPosition07Value = args.mInput_MvSurroundingObjectLateralPosition07Value;
        mMvSurroundingObjectLateralPosition08Value = args.mInput_MvSurroundingObjectLateralPosition08Value;
        mMvSurroundingObjectLateralPosition09Value = args.mInput_MvSurroundingObjectLateralPosition09Value;
        mMvSurroundingObjectLateralPosition10Value = args.mInput_MvSurroundingObjectLateralPosition10Value;
        mMvSurroundingObjectLateralPosition11Value = args.mInput_MvSurroundingObjectLateralPosition11Value;
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat06Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat07Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat08Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat09Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat10Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat11Stat>();
    }

    void onMvSurroundingObjectLateralPosition12to14ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition12to14ValueChanged& args) {
        mMvSurroundingObjectLateralPosition12Value = args.mInput_MvSurroundingObjectLateralPosition12Value;
        mMvSurroundingObjectLateralPosition13Value = args.mInput_MvSurroundingObjectLateralPosition13Value;
        mMvSurroundingObjectLateralPosition14Value = args.mInput_MvSurroundingObjectLateralPosition14Value;
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat12Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat13Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Stat>();
    }

    void onMvSurroundingObjectLongitudinalPosition01to05ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition01to05ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition01Value = args.mInput_MvSurroundingObjectLongitudinalPosition01Value;
        mMvSurroundingObjectLongitudinalPosition02Value = args.mInput_MvSurroundingObjectLongitudinalPosition02Value;
        mMvSurroundingObjectLongitudinalPosition03Value = args.mInput_MvSurroundingObjectLongitudinalPosition03Value;
        mMvSurroundingObjectLongitudinalPosition04Value = args.mInput_MvSurroundingObjectLongitudinalPosition04Value;
        mMvSurroundingObjectLongitudinalPosition05Value = args.mInput_MvSurroundingObjectLongitudinalPosition05Value;
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong02Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong03Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong04Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong05Stat>();
    }

    void onMvSurroundingObjectLongitudinalPosition06to11ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition06to11ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition06Value = args.mInput_MvSurroundingObjectLongitudinalPosition06Value;
        mMvSurroundingObjectLongitudinalPosition07Value = args.mInput_MvSurroundingObjectLongitudinalPosition07Value;
        mMvSurroundingObjectLongitudinalPosition08Value = args.mInput_MvSurroundingObjectLongitudinalPosition08Value;
        mMvSurroundingObjectLongitudinalPosition09Value = args.mInput_MvSurroundingObjectLongitudinalPosition09Value;
        mMvSurroundingObjectLongitudinalPosition10Value = args.mInput_MvSurroundingObjectLongitudinalPosition10Value;
        mMvSurroundingObjectLongitudinalPosition11Value = args.mInput_MvSurroundingObjectLongitudinalPosition11Value;
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong06Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong07Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong08Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong09Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong10Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong11Stat>();
    }

    void onMvSurroundingObjectLongitudinalPosition12to14ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition12to14ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition12Value = args.mInput_MvSurroundingObjectLongitudinalPosition12Value;
        mMvSurroundingObjectLongitudinalPosition13Value = args.mInput_MvSurroundingObjectLongitudinalPosition13Value;
        mMvSurroundingObjectLongitudinalPosition14Value = args.mInput_MvSurroundingObjectLongitudinalPosition14Value;
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong12Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong13Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Stat>();
    }

    void onPuMGroup2AdasWarning1_3StatusChanged(const ArgumentsPuMGroup2AdasWarning1_3StatusChanged& args) {
        mPuMGroup2AdasWarning1_3Status = args.mInput_PuMGroup2AdasWarning1_3Status;

        updateEventGroup2MiniPopup1_3();
    }

    void onPuFGroup7HandsFreeFailureStatusChanged(const ArgumentsPuFGroup7HandsFreeFailureStatusChanged& args) {
        mPuFGroup7HandsFreeFailureStatus = args.mInput_PuFGroup7HandsFreeFailureStatus;

        if (ISTIMEOUT(args.mInput_PuFGroup7HandsFreeFailureStatus) == true) {
            mPuFGroup7HandsFreeFailureStatus = PuFGroup7HandsFreeFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHdaFailure();
        updateConstantLcaFailure();
        updateConstantHda3HandsFreeFailure();
    }

    void onIslaNextSpeedLimitTrafficSignStatusChanged(const ArgumentsIslaNextSpeedLimitTrafficSignStatusChanged& args) {
        mIslaNextSpeedLimitTrafficSignStatus = args.mInput_IslaNextSpeedLimitTrafficSignStatus;
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
    }

    void onIslaNextSpeedLimitTrafficSignValueChanged(const ArgumentsIslaNextSpeedLimitTrafficSignValueChanged& args) {
        mIslaNextSpeedLimitTrafficSignValue = args.mInput_IslaNextSpeedLimitTrafficSignValue;
        updateConstantISLANextSpeedLimitTrafficSignStat();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;
        updateConstantISLANextSupportTrafficSignUnitStat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onIslaNextSupportTrafficSignValueChanged(const ArgumentsIslaNextSupportTrafficSignValueChanged& args) {
        mIslaNextSupportTrafficSignValue = args.mInput_IslaNextSupportTrafficSignValue;
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onIslaSupplementaryTrafficSign2StatusChanged(const ArgumentsIslaSupplementaryTrafficSign2StatusChanged& args) {
        mIslaSupplementaryTrafficSign2Status = args.mInput_IslaSupplementaryTrafficSign2Status;
        updateConstantISLAAddSign2Stat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
#ifdef ENABLE_GCOV_ON
    void updateOneShotTimerPriorityDISStatus(const ssfs::SFCTimerStatus& status) {
        setSFCADAS_Driving_NewTimerOneShotTimerPriorityDISStat(
            static_cast<SFCADAS_Driving_NewTimerOneShotTimerPriorityDISStat>(status));
    }

    void updateOneShotTimerPriorityISLAStatus(const ssfs::SFCTimerStatus& status) {
        setSFCADAS_Driving_NewTimerOneShotTimerPriorityISLAStat(
            static_cast<SFCADAS_Driving_NewTimerOneShotTimerPriorityISLAStat>(status));
    }
#endif
    void updateAll() {
        updateAllConstant();
        updateAllTelltale();
        updateAllEvent();
        updateAllSound();
        updateAdasInter();
        updateSWVersionInfo();
    }

    void updateAllConstant() {
        updateConstantHDAHDPStat();
        updateConstantNSCCAutoSetStat();
        updateConstantFrontVehicleColorStat();
        updateConstantFrontVehicleLevelValue();
        updateConstantHeadywayDistanceColorStat();
        updateConstantHeadywayDistanceLevelValue();
        updateConstantSubVehicleColorStat();
        updateConstantSetSpeedColorStat();
        updateConstantSetSpeedStat();
        updateConstantISLAManualSetSpeedStat();
        updateConstantLaneCentralLineStat();
        updateConstantLeftLaneColorStat();
        updateConstantLeftLineLatPosStat();
        updateConstantLaneRoadCvrtLvlStat();
        updateConstantRightLaneColorStat();
        updateConstantRightLineLatPosStat();
        updateConstantHeadwayDistColorStat();
        updateConstantHeadwayDistLongPosStat();
        updateConstantCenterRoadColorStat();
        updateConstantCenterRoadLongPosStat();
        updateConstantLeftRoadColorStat();
        updateConstantRightRoadColorStat();
        updateConstantViewFrontVehicleColorStat();
        updateConstantViewFrontVehicleLatPosStat();
        updateConstantViewFrontVehicleLongPosStat();
        updateConstantViewFrontObstacleColorStat();
        updateConstantViewFrontObstacleLatPosStat();
        updateConstantViewFrontObstacleLongPosStat();
        updateConstantViewFrontLeftVehicleColorStat();
        updateConstantViewFrontLeftVehicleLatPosStat();
        updateConstantViewFrontLeftVehicleLongPosStat();
        updateConstantViewFrontRightVehicleColorStat();
        updateConstantViewFrontRightVehicleLatPosStat();
        updateConstantViewFrontRightVehicleLongPosStat();
        updateConstantViewLeftVehicleColorStat();
        updateConstantViewLeftVehicleLatPosStat();
        updateConstantViewLeftVehicleLongPosStat();
        updateConstantViewRightVehicleColorStat();
        updateConstantViewRightVehicleLatPosStat();
        updateConstantViewRightVehicleLongPosStat();
        updateConstantViewRearLeftWarnStat();
        updateConstantViewRearRightWarnStat();
        updateConstantViewLeftArrowStat();
        updateConstantViewLeftArrowLatValue();
        updateConstantViewRightArrowStat();
        updateConstantViewRightArrowLatValue();
        updateConstantMainVehicleStat();
        updateConstantISLAMainSpeedStat();
        updateConstantISLAMainSignStat();
        updatePrivateISLAAddtnlSign();
        updateConstantFinalDISISLAAddtnlSignStat();
        updateEventDrivingAssistSummary();
        updateConstantISLAAddSignStat();
        updateConstantCenterRoadLeftLatPosStat();
        updateConstantCenterRoadRightLatPosStat();
        updateConstantLeftRoadLatPosStat();
        updateConstantRightRoadLatPosStat();
        updateConstantViewFrontWarnStat();
        updateConstantHDPBackGroundOutlineStat();
        updateConstantHDPBackGroundStat();
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
        updateConstantLcaFailure();
        updateConstantEmergencyStopFailure();
        updateConstantHda3HandsFreeFailure();
        updateConstantHDPProductDivisionAdditionalInformation();
        updateConstantExtLampStat();
        updateConsstantIslaSoundSymbolStat();
        updateConstantMainBrakeLight();
        updateConstantInfoDDTResidualDistanceValue();
        updateADASViewLaneChangeRectangleStat();
        updateADASViewLaneChangeRectangleLat();
        updateConstantADASViewFrontObjectColor01Stat();
        updateConstantADASViewFrontObjectClass01Stat();
        updateConstantADASViewFrontObjectLat01();
        updateConstantADASViewFrontObjectLong01();
        updateConstantADASViewFrontObjectHeadingAngle01();
        updateHandsOnOffTelltaleStatus();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateConstantISLANextSpeedLimitTrafficSignStat();
        updateConstantISLANextSupportTrafficSignUnitStat();
        updateConstantISLANextSupportTrafficSignStat();
        updateConstantISLAAddSign2Stat();
        updateADASViewSurroundingObjectColor<MvSurroundingObject01Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor01Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject02Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor02Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject03Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor03Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject04Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor04Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject05Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor05Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject06Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor06Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject07Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor07Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject08Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor08Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject09Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor09Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject10Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor10Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject11Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor11Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject12Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor12Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject13Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor13Stat>();
        updateADASViewSurroundingObjectColor<MvSurroundingObject14Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor14Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification01Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass01Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification02Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass02Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification03Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass03Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification04Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass04Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification05Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass05Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification06Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass06Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification07Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass07Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification08Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass08Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification09Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass09Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification10Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass10Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification11Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass11Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification12Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass12Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification13Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass13Stat>();
        updateADASViewSurroundingObjectClass<MvSurroundingObjectClassification14Status,
                                             SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass14Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle02Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle03Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle04Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle05Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle06Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle07Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle08Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle09Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle10Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle11Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle12Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle13Stat>();
        updateADASViewSurroundingObjectHeadingAngle<SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat02Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat03Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat04Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat05Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat06Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat07Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat08Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat09Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat10Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat11Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat12Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat13Stat>();
        updateADASViewSurroundingObjectLat<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong02Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong03Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong04Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong05Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong06Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong07Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong08Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong09Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong10Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong11Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong12Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong13Stat>();
        updateADASViewSurroundingObjectLong<SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Stat>();
    }

    void updateAllTelltale() {
        updateTelltaleFCA();
        updateTelltaleELK();
        updateTelltaleLKA();
        updateTelltaleHBA();
        updateTelltaleDAW();
        updateTelltaleLFAStat();
        updateTelltaleLCALeftStat();
        updateTelltaleLCARightStat();
    }

    void updateAllEvent() {
        updateEventDrivingAssistSummary();
        updateEventGroup1FullPopup1_1();
        updateEventGroup1FullPopup1_2();
        updateEventGroup1FullPopup2_1();
        updateEventGroup1FullPopup3_1();
        updateEventGroup4FullPopup1_1();
        updateEventGroup7FullPopupBlindSpotSafetyFailure();
        updateEventGroup7FullPopupOutsideMirrorSymbolFailure();
        updateEventGroup2MiniPopup1_1();
        updateEventGroup2MiniPopup1_2();
        updateEventGroup2MiniPopup1_3();
        updateEventGroup7FullPopupHdpFailure();
        updateEventGroup7FullPopup1();
        updateEventGroup7FullPopup2();
        updateEventHDPOperProhibit();
    }

    void updateAllSound() {
        updateSoundAdasWarning1_1Status();
        updateSoundAdasWarning1_2Status();
        updateSoundAdasWarning1_3Status();
        updateSoundAdasWarning1_4Status();
        updateSoundAdasWarning1_5Status();
        updateSoundAdasWarning2_1Status();
    }

    void updateAdasInter() {
        updateAdasOnStatus();
        updateAdasViewFixStatus();
        updateHDPMasterWarningStatus();
    }

    void updatePrivateHDPRedundancyStatus() {
        updatePrivate_HDPRedundancyStatus_1();  // used
        updatePrivate_HDPRedundancyStatus_2();  // TODO: not used
    }

    // 연산로직
    // 4.1 ADAS 뷰 전환 조건
    void updateAdasOnStatus() {
        SFCADAS_Driving_NewInter_AdasOnStatus stat = SFCADAS_Driving_NewInter_AdasOnStatus::OFF;
        if (mIsIgnOn == true && mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF &&
            (mMvHostVehicle1Status == MvHostVehicle1Status::ON || mMvHostVehicle2Status == MvHostVehicle2Status::ON)) {
            stat = SFCADAS_Driving_NewInter_AdasOnStatus::ON;
        }
        setSFCADAS_Driving_NewInter_AdasOnStatus(stat);
    }

    // 4.2 ADAS 뷰 고정 표시 조건
    void updateAdasViewFixStatus() {
        SFCADAS_Driving_NewInter_AdasViewFixStatus stat = SFCADAS_Driving_NewInter_AdasViewFixStatus::NOT_FIX;
        if (mIsIgnOn == true && (mSmvHostVehicleStatus == SmvHostVehicleStatus::CYAN ||
                                 mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON)) {
            stat = SFCADAS_Driving_NewInter_AdasViewFixStatus::FIX;
            mInter_AdasViewFixStatus = SFCADAS_Driving_NewInter_AdasViewFixStatus::FIX;
        } else {
            mInter_AdasViewFixStatus = SFCADAS_Driving_NewInter_AdasViewFixStatus::NOT_FIX;
        }
        setSFCADAS_Driving_NewInter_AdasViewFixStatus(stat);
        updateEventHDPOperProhibit();
    }

    // 4.3.1 HDP 고장
    void updatePrivate_HDPRedundancyStatus_1() {
        mPrivate_HDPRedundancyStatus_1 = Private_HDPRedundancyStatus_1::OFF;
        if ((mHDP_RDDWarningState == HDP_RDDWarningState::MRM_WARNING_REQUEST ||
             mHDP_RDDWarningState == HDP_RDDWarningState::EM_WARNING_REQUEST) ||
            (mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_RCU_ALONE) ||
            (mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_RCU_ALONE)) {
            mPrivate_HDPRedundancyStatus_1 = Private_HDPRedundancyStatus_1::ON;
        }
        updateConstantSubVehicleColorStat();
        updateConstantSetSpeedColorStat();
        updateConstantHDAHDPStat();
        updateConstantHDPBackGroundOutlineStat();
        updateConstantHDPBackGroundStat();
        updateHDPMasterWarningStatus();
        updateAdasOnStatus();
        updateAdasViewFixStatus();
        updateEventDrivingAssistSummary();
        updateConstantSetSpeedStat();
    }

    // 4.3.2 HDP 고장 (횡제어 가능)
    // TODO: LFA에서 삭제됨에 따라 Private변수 사용 안 함
    void updatePrivate_HDPRedundancyStatus_2() {
        mPrivate_HDPRedundancyStatus_2 = Private_HDPRedundancyStatus_2::OFF;
        if ((mHDP_RDDWarningState == HDP_RDDWarningState::MRM_WARNING_REQUEST ||
             mHDP_RDDWarningState == HDP_RDDWarningState::EM_WARNING_REQUEST) ||
            (mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU) ||
            (mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU)) {
            mPrivate_HDPRedundancyStatus_2 = Private_HDPRedundancyStatus_2::ON;
        }
        updateTelltaleLFAStat();
    }

    // 4.4 HDP S/W 버전 정보
    void updateSWVersionInfo() {
        AddressValueList outputAddressList;
        if (mIsIgnOn == true) {
            outputAddressList.emplace_back(Vehicle.AD.ADAS_DRIVING_NEW.Output_HDPSupportSWVersion, static_cast<HUInt64>(1));
            setValue(outputAddressList);
            outputAddressList.clear();
        }
    }

    // 4.5 HDP Master Warning 점등 조건
    void updateHDPMasterWarningStatus() {
        SFCADAS_Driving_NewInter_HDPMasterWarningStatus stat = SFCADAS_Driving_NewInter_HDPMasterWarningStatus::OFF;
        if (mIgnElapsed == IgnElapsed::OFF_0ms &&
            (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON || mBGHDPStatus == BGHDPStatus::RED_BLINK)) {
            stat = SFCADAS_Driving_NewInter_HDPMasterWarningStatus::ON;
        }
        setSFCADAS_Driving_NewInter_HDPMasterWarningStatus(stat);
    }

    // Telltale
    // 5.1.1 FCA
    void updateTelltaleFCA() {
        SFCADAS_Driving_NewTelltaleFCAStat fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::AMBER ||
                (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON))) {
                fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::AMBER;
            } else if (mForwardSafetySymbolStatus == ForwardSafetySymbolStatus::RED) {
                fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::RED;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms &&
                   (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON)) {
            fcaStat = SFCADAS_Driving_NewTelltaleFCAStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewTelltaleFCAStat(fcaStat);
    }

    // 5.1.2 ELK
    void updateTelltaleELK() {
        SFCADAS_Driving_NewTelltaleELKStat elkStat = SFCADAS_Driving_NewTelltaleELKStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mEmergencySteeringSymbolStatus == EmergencySteeringSymbolStatus::RED) {
                elkStat = SFCADAS_Driving_NewTelltaleELKStat::RED;
            } else if (mEmergencySteeringSymbolStatus == EmergencySteeringSymbolStatus::AMBER ||
                       (mEmergencySteeringSymbolStatus == EmergencySteeringSymbolStatus::UNHANDLED_TIMEOUT &&
                        mConfigAdasFCA2 == ConfigAdasFCA2::ON)) {
                elkStat = SFCADAS_Driving_NewTelltaleELKStat::AMBER;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasFCA2 == ConfigAdasFCA2::ON) {
            elkStat = SFCADAS_Driving_NewTelltaleELKStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewTelltaleELKStat(elkStat);
    }

    // 5.1.3 LKA
    void updateTelltaleLKA() {
        SFCADAS_Driving_NewTelltaleLKAStat lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::GRAY) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::GRAY;
            } else if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::GREEN) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::GREEN;
            } else if (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::AMBER ||
                       (mLaneSafetySymbolStatus == LaneSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                        mConfigAdasLKA == ConfigAdasLKA::ON)) {
                lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::AMBER;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasLKA == ConfigAdasLKA::ON) {
            lkaStat = SFCADAS_Driving_NewTelltaleLKAStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewTelltaleLKAStat(lkaStat);
    }

    // 5.1.4 DAW
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
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewTelltaleDAWStat(dawStat);
    }

    // 5.1.5 HBA
    void updateTelltaleHBA() {
        SFCADAS_Driving_NewTelltaleHBAStat hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigILCU == ConfigILCU::ON && (mIlcuLhActWarnStatus == IlcuLhActWarnStatus::HBA_INDI_ON ||
                                                  mIlcuRhActWarnStatus == IlcuRhActWarnStatus::HBA_INDI_ON)) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::GREEN;
            } else if (mConfigILCU == ConfigILCU::ON && (mIlcuLhActWarnStatus == IlcuLhActWarnStatus::STANDBY ||
                                                         mIlcuRhActWarnStatus == IlcuRhActWarnStatus::STANDBY)) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::WHITE;
            } else if (mConfigIFS == ConfigIFS::ON && mIfsIndiReqStatus == IfsIndiReqStatus::ON) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::GREEN;
            } else if (mConfigIFS == ConfigIFS::ON && mIfsIndiReqStatus == IfsIndiReqStatus::STANDBY) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::WHITE;
            } else if (mConfigNewHBA == ConfigNewHBA::ON && mHbaSymbolStatus == HbaSymbolStatus::GREEN) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::GREEN;
            } else if (mConfigNewHBA == ConfigNewHBA::ON && mHbaSymbolStatus == HbaSymbolStatus::GRAY_WHITE) {
                hbaStat = SFCADAS_Driving_NewTelltaleHBAStat::WHITE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewTelltaleHBAStat(hbaStat);
    }

    // 5.1.6 LFA
    void updateTelltaleLFAStat() {
        SFCADAS_Driving_NewTelltaleLFAStat stat = SFCADAS_Driving_NewTelltaleLFAStat::OFF;
        SFCADAS_Driving_NewTelltaleLFAStatOptional statOptional = SFCADAS_Driving_NewTelltaleLFAStatOptional::OFF;
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
            case SmvLfaSymbolStatus::CYAN:
                stat = SFCADAS_Driving_NewTelltaleLFAStat::CYAN;
                break;
            case SmvLfaSymbolStatus::GRAY_TYPE_B:
                stat = SFCADAS_Driving_NewTelltaleLFAStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLFAStatOptional::GRAY_TYPE_B;
                break;
            case SmvLfaSymbolStatus::GREEN_TYPE_B:
                stat = SFCADAS_Driving_NewTelltaleLFAStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLFAStatOptional::GREEN_TYPE_B;
                break;
            case SmvLfaSymbolStatus::WHITE_TYPE_B:
                stat = SFCADAS_Driving_NewTelltaleLFAStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLFAStatOptional::WHITE_TYPE_B;
                break;
            default:
                // OFF
                break;
        }
        setSFCADAS_Driving_NewTelltaleLFAStat(stat);
        setSFCADAS_Driving_NewTelltaleLFAStatOptional(statOptional);
    }

    // 5.1.7 LCA
    // 5.1.7.1 차로 변경 보조 심볼-왼쪽
    void updateTelltaleLCALeftStat() {
        SFCADAS_Driving_NewTelltaleLCALeftStat stat = SFCADAS_Driving_NewTelltaleLCALeftStat::OFF;
        SFCADAS_Driving_NewTelltaleLCALeftStatOptional statOptional = SFCADAS_Driving_NewTelltaleLCALeftStatOptional::OFF;
        if (mIsIgnOn == true) {
            if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::GRAY) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::GRAY;
            } else if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::GREEN) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::GREEN;
            } else if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::WHITE) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::WHITE;
            } else if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::GRAY_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCALeftStatOptional::GRAY_TYPE_B;
            } else if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::GREEN_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCALeftStatOptional::GREEN_TYPE_B;
            } else if (mSmvLcaLeftSymbolStatus == SmvLcaLeftSymbolStatus::WHITE_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCALeftStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCALeftStatOptional::WHITE_TYPE_B;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewTelltaleLCALeftStat(stat);
        setSFCADAS_Driving_NewTelltaleLCALeftStatOptional(statOptional);
    }

    // 5.1.7.1 차로 변경 보조 심볼-오른쪽
    void updateTelltaleLCARightStat() {
        SFCADAS_Driving_NewTelltaleLCARightStat stat = SFCADAS_Driving_NewTelltaleLCARightStat::OFF;
        SFCADAS_Driving_NewTelltaleLCARightStatOptional statOptional = SFCADAS_Driving_NewTelltaleLCARightStatOptional::OFF;
        if (mIsIgnOn == true) {
            if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::GRAY) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::GRAY;
            } else if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::GREEN) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::GREEN;
            } else if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::WHITE) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::WHITE;
            } else if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::GRAY_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCARightStatOptional::GRAY_TYPE_B;
            } else if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::GREEN_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCARightStatOptional::GREEN_TYPE_B;
            } else if (mSmvLcaRightSymbolStatus == SmvLcaRightSymbolStatus::WHITE_TYPE_B) {
                stat = SFCADAS_Driving_NewTelltaleLCARightStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleLCARightStatOptional::WHITE_TYPE_B;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewTelltaleLCARightStat(stat);
        setSFCADAS_Driving_NewTelltaleLCARightStatOptional(statOptional);
    }

    void updateHandsOnOffTelltaleStatus() {
        SFCADAS_Driving_NewTelltaleHandsOnOffStat stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OFF;
        SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::OFF;

        if (mIsIgnOn == true) {
            if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_FREE_CYAN) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::OFF_CYAN;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_FREE_GREEN) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::OFF_GREEN;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_ON_CYAN) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::ON_CYAN;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_ON_GRAY) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::ON_GRAY;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_ON_AMBER) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::ON_AMBER;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_ON_RED) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::ON_RED;
            } else if (mDriverHandsOnOffStatus == DriverHandsOnOffStatus::HANDS_ON_WHITE) {
                stat = SFCADAS_Driving_NewTelltaleHandsOnOffStat::OPTIONAL;
                statOptional = SFCADAS_Driving_NewTelltaleHandsOnOffStatOptional::ON_WHITE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewTelltaleHandsOnOffStat(stat);
        setSFCADAS_Driving_NewTelltaleHandsOnOffStatOptional(statOptional);
    }

    // Constant
    // 5.2. 표시 구성요소 정보 처리 로직
    // 5.2.1 주행 보조 요약
    // 5.2.1.1 전방 물체 사이드 뷰 / NSCC 전방 도로 심볼 (1)
    // 5.2.1.1.1 칼라
    void updateConstantFrontVehicleColorStat() {
        SFCADAS_Driving_NewConstantFrontVehicleColorStat stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::OFF) {
                if (mSmvFrontObjectStatus == SmvFrontObjectStatus::DARK) {
                    stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::DARK;
                } else if (mSmvFrontObjectStatus == SmvFrontObjectStatus::BRIGHT) {
                    stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::BRIGHT;
                } else {
                    // OFF
                }
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::SAFETY_ZONE) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::SAFETY_ZONE;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::CURVE) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::CURVE;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::IC_AND_JC_LEFT) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::IC_AND_JC_LEFT;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::IC_AND_JC_RIGHT) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::IC_AND_JC_RIGHT;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::TOLLGATE) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::TOLLGATE;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::ROUNDABOUT) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::ROUNDABOUT;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::INTERSECTION) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::INTERSECTION;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::T_INTERSECTION) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::T_INTERSECTION;
            } else if (mSmvNsccSymbol2Status == SmvNsccSymbol2Status::SPEED_BUMP) {
                stat = SFCADAS_Driving_NewConstantFrontVehicleColorStat::SPEED_BUMP;
            } else {
                // OFF
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
                // OFF
            }
        }
        mSummaryFrontVehicleValue = stat;
        setSFCADAS_Driving_NewConstantFrontVehicleLevelValue(stat);
    }

    // 5.2.1.2 차간 거리 단계 (2)
    // 5.2.1.2.1 칼라
    void updateConstantHeadywayDistanceColorStat() {
        SFCADAS_Driving_NewConstantHeadwayDistanceColorStat stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvVehicleDistanceLevelStatus == SmvVehicleDistanceLevelStatus::BRAND) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::BRAND;
            } else if (mSmvVehicleDistanceLevelStatus == SmvVehicleDistanceLevelStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::WHITE;
            } else if (mSmvVehicleDistanceLevelStatus == SmvVehicleDistanceLevelStatus::CYAN) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::CYAN;
            } else {
                // OFF
            }
        }
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
            } else if (mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL4 ||
                       mSmvVehicleDistanceLevelValue == SmvVehicleDistanceLevelValue::LEVEL4_SOLID) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::LEVEL4;
            } else {
                // OFF
            }
        }
        mSummaryHeadwayDistanceSetVal = stat;
        setSFCADAS_Driving_NewConstantHeadwayDistanceLevelValue(stat);
    }

    // 5.2.1.3 자차 사이드 뷰 (3)
    void updateConstantSubVehicleColorStat() {
        SFCADAS_Driving_NewConstantSubVehicleColorStat stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::OFF;
        if (mSmvHostVehicleStatus == SmvHostVehicleStatus::DARK &&
            mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::DARK;
        } else if (mSmvHostVehicleStatus == SmvHostVehicleStatus::BRIGHT &&
                   mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::BRIGHT;
        } else if (mConfigHDPName == ConfigHDPName::TYPE_A) {
            if (mSmvHostVehicleStatus == SmvHostVehicleStatus::CYAN &&
                mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::CYAN;
            } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::CYAN;
            } else {
                // OFF
            }
        } else if (mConfigHDPName == ConfigHDPName::TYPE_B) {
            if (mSmvHostVehicleStatus == SmvHostVehicleStatus::CYAN &&
                mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::CYAN_B;
            } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
                stat = SFCADAS_Driving_NewConstantSubVehicleColorStat::CYAN_B;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        mSummarySubVehicleStat = stat;
        setSFCADAS_Driving_NewConstantSubVehicleColorStat(stat);
    }

    // 5.2.1.4 설정 속도 및 속도 단위 (4)
    // 5.2.1.4.1 칼라
    void updateConstantSetSpeedColorStat() {
        SFCADAS_Driving_NewConstantSetSpeedColorStat stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::OFF;
        if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::GRAY;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::GREEN;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::WHITE;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::CYAN) {
                stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::CYAN;
            } else {
                // OFF
            }
        } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
            stat = SFCADAS_Driving_NewConstantSetSpeedColorStat::CYAN;
        } else {
            // OFF
        }
        mSummarySetSpeedStat = stat;
        setSFCADAS_Driving_NewConstantSetSpeedColorStat(stat);
    }

    // 5.2.1.4.2 속도값
    void updateConstantSetSpeedStat() {
        SFCADAS_Driving_NewConstantSetSpeedStat stat = SFCADAS_Driving_NewConstantSetSpeedStat::OFF;
        HUInt64 value = 0;
        if ((0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) &&
            (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF)) {
            stat = SFCADAS_Driving_NewConstantSetSpeedStat::VALUE;
            value = mSmvSetSpeedValue;
        } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
            stat = SFCADAS_Driving_NewConstantSetSpeedStat::VALUE;
            // value = 0;
        } else {
            // no operation
        }
        setSFCADAS_Driving_NewConstantSetSpeedStat(stat);
        setSFCADAS_Driving_NewConstantSetSpeedValue(value);
    }

    // 5.2.1.5 고속도로 주행 보조 및 고속도로 자율주행 심볼 (5)
    void updateConstantHDAHDPStat() {
        SFCADAS_Driving_NewConstantHDAHDPStat stat = SFCADAS_Driving_NewConstantHDAHDPStat::OFF;
        if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::GRAY;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::GREEN;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::WHITE;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::CYAN) {
                if (mConfigHDPName == ConfigHDPName::TYPE_A) {
                    stat = SFCADAS_Driving_NewConstantHDAHDPStat::CYAN;
                } else if (mConfigHDPName == ConfigHDPName::TYPE_B) {
                    stat = SFCADAS_Driving_NewConstantHDAHDPStat::CYAN_B;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
            if (mConfigHDPName == ConfigHDPName::TYPE_A) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::CYAN;
            } else if (mConfigHDPName == ConfigHDPName::TYPE_B) {
                stat = SFCADAS_Driving_NewConstantHDAHDPStat::CYAN_B;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        mSummaryHDAHDPTelltaleStat = stat;
        setSFCADAS_Driving_NewConstantHDAHDPStat(stat);
    }

    // 5.2.1.6 설정속도 조절 심볼 (6)
    void updateConstantISLAManualSetSpeedStat() {
        SFCADAS_Driving_NewConstantISLAManualSetSpeedStat stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_WHITE) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::PLUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_WHITE) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::MINUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_GREEN) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::PLUS_GREEN;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_GREEN) {
                stat = SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::MINUS_GREEN;
            } else {
                // OFF
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
                // OFF
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
            } else if (mMvLeftLineStatus == MvLeftLineStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::GREEN;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::WHITE_DASH) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::WHITE_DASH;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::GREEN_DASH) {
                stat = SFCADAS_Driving_NewConstantLeftLaneColorStat::GREEN_DASH;
            } else {
                // OFF
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
                // default
            }
        }
        setSFCADAS_Driving_NewConstantLeftLineLatPosStat(stat);
        setSFCADAS_Driving_NewConstantLeftLineLatPosValue(value);
    }

    // 5.2.2.2 전방 장애물 (20)
    // 5.2.2.2.1 칼라
    void updateConstantViewFrontObstacleColorStat() {
        SFCADAS_Driving_NewConstantViewFrontObstacleColorStat stat = SFCADAS_Driving_NewConstantViewFrontObstacleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObstacleStatus == MvFrontObstacleStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleColorStat::DARK;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleColorStat::BRIGHT;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleColorStat::DARK_RUBBER_CONE;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleColorStat::BRIGHT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontObstacleColorStat(stat);
    }

    // 5.2.2.2.2 횡위치
    void updateConstantViewFrontObstacleLatPosStat() {
        SFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat stat = SFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mMvFrontObstacleLateralPositionValue == 0) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat::CENTER;
            } else if (1 <= mMvFrontObstacleLateralPositionValue && mMvFrontObstacleLateralPositionValue <= 45) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat::RIGHT_VALUE;
                value = mMvFrontObstacleLateralPositionValue;
            } else if (83 <= mMvFrontObstacleLateralPositionValue &&  // -45 ~ -1
                       mMvFrontObstacleLateralPositionValue <= 127) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat::LEFT_VALUE;
                value = labs(mMvFrontObstacleLateralPositionValue - 128);
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontObstacleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontObstacleLatPosValue(value);
    }

    // 5.2.2.2.3 종위치
    void updateConstantViewFrontObstacleLongPosStat() {
        SFCADAS_Driving_NewConstantViewFrontObstacleLongPosStat stat =
            SFCADAS_Driving_NewConstantViewFrontObstacleLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvFrontObstacleLongitudinalPositionValue && mMvFrontObstacleLongitudinalPositionValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleLongPosStat::VALUE;
                value = mMvFrontObstacleLongitudinalPositionValue;
            } else if (1001 <= mMvFrontObstacleLongitudinalPositionValue && mMvFrontObstacleLongitudinalPositionValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantViewFrontObstacleLongPosStat::VALUE;
                value = 1000;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontObstacleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontObstacleLongPosValue(value);
    }

    // 5.2.2.3 자차 방향 지시등-왼쪽 (21)
    // 5.2.2.4 자차 방향 지시등-오른쪽 (22)
    // 5.2.2.5 자차 전조등 (23)
    // 5.2.2.6 자차 상향등 (24)

    void updateConstantExtLampStat() {
        SFCADAS_Driving_NewConstantExteriorTurnLeftLampActiveStat extTurnLeftStat =
            SFCADAS_Driving_NewConstantExteriorTurnLeftLampActiveStat::OFF;
        SFCADAS_Driving_NewConstantExteriorTurnRightLampActiveStat extTurnRightStat =
            SFCADAS_Driving_NewConstantExteriorTurnRightLampActiveStat::OFF;
        SFCADAS_Driving_NewConstantExteriorLowBeamActiveStat extLowBeamStat =
            SFCADAS_Driving_NewConstantExteriorLowBeamActiveStat::OFF;
        SFCADAS_Driving_NewConstantExteriorHighBeamActiveStat extHighBeamStat =
            SFCADAS_Driving_NewConstantExteriorHighBeamActiveStat::OFF;
        if (mIsIgnOn == true) {
            if (mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::ON) {
                extTurnLeftStat = SFCADAS_Driving_NewConstantExteriorTurnLeftLampActiveStat::ON;
            }
            if (mExtTurnRightLampStatus == ExtTurnRightLampStatus::ON) {
                extTurnRightStat = SFCADAS_Driving_NewConstantExteriorTurnRightLampActiveStat::ON;
            }
            if (mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::ON) {
                extLowBeamStat = SFCADAS_Driving_NewConstantExteriorLowBeamActiveStat::ON;
            }
            if (mExtHighBeamOnReqStatus == ExtHighBeamOnReqStatus::ON) {
                extHighBeamStat = SFCADAS_Driving_NewConstantExteriorHighBeamActiveStat::ON;
            }
        }
        setSFCADAS_Driving_NewConstantExteriorTurnLeftLampActiveStat(extTurnLeftStat);
        setSFCADAS_Driving_NewConstantExteriorTurnRightLampActiveStat(extTurnRightStat);
        setSFCADAS_Driving_NewConstantExteriorLowBeamActiveStat(extLowBeamStat);
        setSFCADAS_Driving_NewConstantExteriorHighBeamActiveStat(extHighBeamStat);
    }

    // 5.2.2.25 HDP 잔여 거리 (25)
    void updateConstantInfoDDTResidualDistanceValue() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantDDTResidualDistanceStat stat = SFCADAS_Driving_NewConstantDDTResidualDistanceStat::OFF;

        if (mIsIgnOn == true) {
            if ((mInfoDDTResidualDistanceValue >= 1) && (mInfoDDTResidualDistanceValue <= 50)) {
                value = mInfoDDTResidualDistanceValue;
                stat = SFCADAS_Driving_NewConstantDDTResidualDistanceStat::VALUE;
            }
        }
        setSFCADAS_Driving_NewConstantDDTResidualDistanceStat(stat);
        setSFCADAS_Driving_NewConstantDDTResidualDistanceValue(value);
    }

    // 5.2.2.26 차로 변경 방향 보조/제안 (26)
    // 5.2.2.26.1 보조/제안
    void updateADASViewLaneChangeRectangleStat() {
        SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleStat stat =
            SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvLaneChangeRectangleStatus == MvLaneChangeRectangleStatus::DOTTED_LINE) {
                stat = SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleStat::DOTTED_LINE;
            } else if (mMvLaneChangeRectangleStatus == MvLaneChangeRectangleStatus::SOLID_LINE) {
                stat = SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleStat::SOLID_LINE;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewLaneChangeRectangleStat(stat);
    }

    // 5.2.2.26.2 횡위치
    void updateADASViewLaneChangeRectangleLat() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat stat =
            SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat::CENTER;
                value = mMvLaneChangeRectangleLateralPositionValue;
            } else if (mMvLaneChangeRectangleLateralPositionValue >= 1 && mMvLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat::RIGHT;
                value = mMvLaneChangeRectangleLateralPositionValue;
            } else if (mMvLaneChangeRectangleLateralPositionValue >= 0x42 && mMvLaneChangeRectangleLateralPositionValue <= 0x7F) {
                // 0x7f -> -1, 0x42 -> -62
                stat = SFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat::LEFT;
                value = (mMvLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatStat(stat);
        setSFCADAS_Driving_NewConstantADASViewLaneChangeRectangleLatValue(value);
    }

    // 5.2.2.27 전방 물체_Extended (27)
    // 5.2.2.27.1 칼라
    void updateConstantADASViewFrontObjectColor01Stat() {
        SFCADAS_Driving_NewConstantADASViewFrontObjectColor01Stat stat =
            SFCADAS_Driving_NewConstantADASViewFrontObjectColor01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObject01Status == MvFrontObject01Status::DARK) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectColor01Stat::DARK;
            } else if (mMvFrontObject01Status == MvFrontObject01Status::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectColor01Stat::BRIGHT;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewFrontObjectColor01Stat(stat);
    }

    // 5.2.2.27.2 타입
    void updateConstantADASViewFrontObjectClass01Stat() {
        SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat stat =
            SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::UNKNOWN) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::UNKNOWN;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::PASSENGER_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::PEDESTRIAN;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::CYCLIST) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::CYCLIST;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::RUBBER_CONE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::EMERGENCY_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::EMERGENCY_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::ANIMAL) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::ANIMAL;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::TUBULAR_MARKER) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::TUBULAR_MARKER;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PE_BARREL) {
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewFrontObjectClass01Stat(stat);
    }

    // 5.2.2.27.3 횡위치
    void updateConstantADASViewFrontObjectLat01() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantADASViewFrontObjectLat01Stat stat =
            SFCADAS_Driving_NewConstantADASViewFrontObjectLat01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectLateralPosition01Value >= 0 && mMvFrontObjectLateralPosition01Value <= 300) {
                value = mMvFrontObjectLateralPosition01Value;
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectLat01Stat::LEFT;
            } else if (mMvFrontObjectLateralPosition01Value >= 1024 && mMvFrontObjectLateralPosition01Value <= 1323) {
                value = 1023 - mMvFrontObjectLateralPosition01Value;  // 1024~1323 : -1~-300
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectLat01Stat::RIGHT;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewFrontObjectLat01Value(value);
        setSFCADAS_Driving_NewConstantADASViewFrontObjectLat01Stat(stat);
    }

    // 5.2.2.27.4 종위치
    void updateConstantADASViewFrontObjectLong01() {
        HInt64 value = 0;
        SFCADAS_Driving_NewConstantADASViewFrontObjectLong01Stat stat =
            SFCADAS_Driving_NewConstantADASViewFrontObjectLong01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectLongitudinalPosition01Value >= 0 && mMvFrontObjectLongitudinalPosition01Value <= 1500) {
                value = mMvFrontObjectLongitudinalPosition01Value;
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectLong01Stat::VALUE;
            } else if (mMvFrontObjectLongitudinalPosition01Value >= 1846 && mMvFrontObjectLongitudinalPosition01Value <= 2045) {
                value = mMvFrontObjectLongitudinalPosition01Value - 2046;
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectLong01Stat::VALUE;
            } else {
                // no operation
            }
        }
        setSFCADAS_Driving_NewConstantADASViewFrontObjectLong01Value(value);
        setSFCADAS_Driving_NewConstantADASViewFrontObjectLong01Stat(stat);
    }

    // 5.2.2.27.5 방향
    void updateConstantADASViewFrontObjectHeadingAngle01() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantADASViewFrontObjectHeadingAngle01Stat stat =
            SFCADAS_Driving_NewConstantADASViewFrontObjectHeadingAngle01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectHeadingAngle01Value >= 0 && mMvFrontObjectHeadingAngle01Value <= 72) {
                value = mMvFrontObjectHeadingAngle01Value;
                stat = SFCADAS_Driving_NewConstantADASViewFrontObjectHeadingAngle01Stat::VALUE;
            }
        }
        setSFCADAS_Driving_NewConstantADASViewFrontObjectHeadingAngle01Value(value);
        setSFCADAS_Driving_NewConstantADASViewFrontObjectHeadingAngle01Stat(stat);
    }

    // 5.2.2.28 주변 물체 01~14_Extended (28)
    // 5.2.2.28.1 칼라
    template <typename InputSignalType, typename OutputSignalType>
    void updateADASViewSurroundingObjectColor() {
        if (std::tuple_size<decltype(mSurroundingObjectColorInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of mSurroundingObjectColorInfoTuple";
            return;
        }
        // mSurroundingObjectColorInfoTuple에서 std::tuple<Type0, Type1, Type2>으로 해당 type의 tuple를 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<InputSignalType>
        // Type1(On condition에 필요한 멤버변수의 레퍼런스, Type0의 Prev값): std::reference_wrapper<InputSignalType>
        // Type2(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        std::tuple surroundingObjectColorInfoElement =
            std::get<std::tuple<std::reference_wrapper<InputSignalType>, std::reference_wrapper<InputSignalType>,
                                std::function<std::string(const OutputSignalType&)>>>(mSurroundingObjectColorInfoTuple);
        if (std::tuple_size<decltype(surroundingObjectColorInfoElement)>::value != kSurroundingObjectInfoElementSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of surroundingObjectColorInfoElement";
            return;
        }
        InputSignalType mvSurroundingObjectStatus = std::get<kType0>(surroundingObjectColorInfoElement);
        InputSignalType prevMvSurroundingObjectStatus = std::get<kType1>(surroundingObjectColorInfoElement);
        OutputSignalType stat = OutputSignalType::OFF;
        if (mIsIgnOn == true) {
            if (prevMvSurroundingObjectStatus != InputSignalType::NO_DISPLAY &&
                mvSurroundingObjectStatus == InputSignalType::DARK) {
                stat = OutputSignalType::DARK;
            } else if (mvSurroundingObjectStatus == InputSignalType::BRIGHT) {
                stat = OutputSignalType::BRIGHT;
            } else if (prevMvSurroundingObjectStatus == InputSignalType::NO_DISPLAY &&
                       mvSurroundingObjectStatus == InputSignalType::DARK) {
                stat = OutputSignalType::FADE_IN;
            } else if (prevMvSurroundingObjectStatus == InputSignalType::DARK &&
                       mvSurroundingObjectStatus == InputSignalType::NO_DISPLAY) {
                stat = OutputSignalType::FADE_OUT;
            } else {
                // no operation
            }
        }
        std::get<kType2>(surroundingObjectColorInfoElement)(stat);
    }

    // 5.2.2.28.2 타입
    template <typename InputSignalType, typename OutputSignalType>
    void updateADASViewSurroundingObjectClass() {
        if (std::tuple_size<decltype(mSurroundingObjectClassInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of mSurroundingObjectClassInfoTuple";
            return;
        }
        // mSurroundingObjectClassInfoTuple에서 std::pair<Type0, Type1>으로 해당 type의 pair를 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<InputSignalType>
        // Type1(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        std::pair surroundingObjectClassInfoElement =
            std::get<std::pair<std::reference_wrapper<InputSignalType>, std::function<std::string(const OutputSignalType&)>>>(
                mSurroundingObjectClassInfoTuple);
        InputSignalType mvSurroundingObjectClassificationStatus = surroundingObjectClassInfoElement.first.get();
        OutputSignalType stat = OutputSignalType::OFF;
        if (mIsIgnOn == true) {
            if (mvSurroundingObjectClassificationStatus == InputSignalType::UNKNOWN) {
                stat = OutputSignalType::UNKNOWN;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::PASSENGER_VEHICLE) {
                stat = OutputSignalType::PASSENGER_VEHICLE;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::COMMERCIAL_VEHICLE) {
                stat = OutputSignalType::COMMERCIAL_VEHICLE;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::PEDESTRIAN) {
                stat = OutputSignalType::PEDESTRIAN;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::CYCLIST) {
                stat = OutputSignalType::CYCLIST;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::MOTOR_CYCLIST_OR_PTW) {
                stat = OutputSignalType::MOTOR_CYCLIST_OR_PTW;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::RUBBER_CONE) {
                stat = OutputSignalType::RUBBER_CONE;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::EMERGENCY_VEHICLE) {
                stat = OutputSignalType::EMERGENCY_VEHICLE;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::ANIMAL) {
                stat = OutputSignalType::ANIMAL;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::TUBULAR_MARKER) {
                stat = OutputSignalType::TUBULAR_MARKER;
            } else if (mvSurroundingObjectClassificationStatus == InputSignalType::PE_BARREL) {
                stat = OutputSignalType::PE_BARREL;
            } else {
                // no operation
            }
        }
        surroundingObjectClassInfoElement.second(stat);
    }

    // 5.2.2.28.3 횡위치
    template <typename OutputSignalType>
    void updateADASViewSurroundingObjectLat() {
        if (std::tuple_size<decltype(mSurroundingObjectLatInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of mSurroundingObjectLatInfoTuple";
            return;
        }
        // mSurroundingObjectLatInfoTuple에서 std::tuple<Type0, Type1, Type2>으로 해당 type의 tuple을 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<HUInt64>
        // Type1(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        // Type2(Set output value를 위한 함수 레퍼런스): std::function<std::string(const HInt64&)>
        std::tuple surroundingObjectLatInfoElement =
            std::get<std::tuple<std::reference_wrapper<HUInt64>, std::function<std::string(const OutputSignalType&)>,
                                std::function<std::string(const HInt64&)>>>(mSurroundingObjectLatInfoTuple);
        if (std::tuple_size<decltype(surroundingObjectLatInfoElement)>::value != kSurroundingObjectInfoElementSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of surroundingObjectLatInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 mvSurroundingObjectLateralPositionValue = std::get<kType0>(surroundingObjectLatInfoElement).get();
        if (mIsIgnOn == true) {
            if (mvSurroundingObjectLateralPositionValue >= 0 && mvSurroundingObjectLateralPositionValue <= 300) {
                stat = OutputSignalType::LEFT;
                value = static_cast<HInt64>(mvSurroundingObjectLateralPositionValue);
            } else if (mvSurroundingObjectLateralPositionValue >= 1024 && mvSurroundingObjectLateralPositionValue <= 1323) {
                stat = OutputSignalType::RIGHT;
                value = 1023 - static_cast<HInt64>(mvSurroundingObjectLateralPositionValue);  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        std::get<kType1>(surroundingObjectLatInfoElement)(stat);
        std::get<kType2>(surroundingObjectLatInfoElement)(value);
    }

    // 5.2.2.28.4 종위치
    template <typename OutputSignalType>
    void updateADASViewSurroundingObjectLong() {
        if (std::tuple_size<decltype(mSurroundingObjectLongInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of mSurroundingObjectLongInfoTuple";
            return;
        }
        // mSurroundingObjectLongInfoTuple에서 std::tuple<Type0, Type1, Type2>으로 해당 type의 tuple을 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<HUInt64>
        // Type1(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        // Type2(Set output value를 위한 함수 레퍼런스): std::function<std::string(const HInt64&)>
        std::tuple surroundingObjectLongInfoElement =
            std::get<std::tuple<std::reference_wrapper<HUInt64>, std::function<std::string(const OutputSignalType&)>,
                                std::function<std::string(const HInt64&)>>>(mSurroundingObjectLongInfoTuple);
        if (std::tuple_size<decltype(surroundingObjectLongInfoElement)>::value != kSurroundingObjectInfoElementSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of surroundingObjectLongInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 mvSurroundingObjectLongitudinalPositionValue = std::get<kType0>(surroundingObjectLongInfoElement).get();
        if (mIsIgnOn == true) {
            if (mvSurroundingObjectLongitudinalPositionValue >= 0 && mvSurroundingObjectLongitudinalPositionValue <= 1500) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(mvSurroundingObjectLongitudinalPositionValue);
            } else if (mvSurroundingObjectLongitudinalPositionValue >= 1846 &&
                       mvSurroundingObjectLongitudinalPositionValue <= 2045) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(mvSurroundingObjectLongitudinalPositionValue) - 2046;
            } else {
                // no operation
            }
        }
        std::get<kType1>(surroundingObjectLongInfoElement)(stat);
        std::get<kType2>(surroundingObjectLongInfoElement)(value);
    }

    // 5.2.2.28.5 방향
    template <typename OutputSignalType>
    void updateADASViewSurroundingObjectHeadingAngle() {
        if (std::tuple_size<decltype(mSurroundingObjectHeadingAngleInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of mSurroundingObjectHeadingAngleInfoTuple";
            return;
        }
        // mSurroundingObjectHeadingAngleInfoTuple에서 std::tuple<Type0, Type1, Type2>으로 해당 type의 tuple을 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<HUInt64>
        // Type1(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        // Type2(Set output value를 위한 함수 레퍼런스): std::function<std::string(const HUInt64&)>
        std::tuple surroundingObjectHeadingAngleInfoElement =
            std::get<std::tuple<std::reference_wrapper<HUInt64>, std::function<std::string(const OutputSignalType&)>,
                                std::function<std::string(const HUInt64&)>>>(mSurroundingObjectHeadingAngleInfoTuple);
        if (std::tuple_size<decltype(surroundingObjectHeadingAngleInfoElement)>::value != kSurroundingObjectInfoElementSize) {
            DWarning() << "[ADAS_Driving_New] Invalied tuple size of surroundingObjectHeadingAngleInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HUInt64 value = 0;
        HUInt64 mvSurroundingObjectHeadingAngleValue = std::get<kType0>(surroundingObjectHeadingAngleInfoElement).get();
        if (mIsIgnOn == true) {
            if (mvSurroundingObjectHeadingAngleValue >= 0 && mvSurroundingObjectHeadingAngleValue <= 72) {
                stat = OutputSignalType::VALUE;
                value = mvSurroundingObjectHeadingAngleValue;
            }
        }
        std::get<kType1>(surroundingObjectHeadingAngleInfoElement)(stat);
        std::get<kType2>(surroundingObjectHeadingAngleInfoElement)(value);
    }

    void updateConstantLaneRoadCvrtLvlStat() {
        // 5.2.2.2.3 차선-왼쪽 (2) 곡률
        // 5.2.2.3.2 차선-오른쪽 (3) 곡률
        // 5.7.5.2.2.5.2 노면-중앙 (5) 곡률
        // 5.7.5.2.2.6.3 노면-왼쪽 (6) 곡률
        // 5.7.5.2.2.7.3 노면-오른쪽 (7) 곡률
        SFCADAS_Driving_NewConstantLaneCvrtLvlStat laneStat = SFCADAS_Driving_NewConstantLaneCvrtLvlStat::OFF;
        SFCADAS_Driving_NewConstantRoadCvrtLvlStat roadStat = SFCADAS_Driving_NewConstantRoadCvrtLvlStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mMvDrivingLaneRadiusValue == 0 || mMvDrivingLaneRadiusValue == kTimeOut) {
                laneStat = SFCADAS_Driving_NewConstantLaneCvrtLvlStat::STRAIGHT;
                roadStat = SFCADAS_Driving_NewConstantRoadCvrtLvlStat::STRAIGHT;
            } else if (1 <= mMvDrivingLaneRadiusValue && mMvDrivingLaneRadiusValue <= 15) {
                laneStat = SFCADAS_Driving_NewConstantLaneCvrtLvlStat::LEFT_VALUE;
                roadStat = SFCADAS_Driving_NewConstantRoadCvrtLvlStat::LEFT_VALUE;
                value = mMvDrivingLaneRadiusValue;
            } else if (16 <= mMvDrivingLaneRadiusValue && mMvDrivingLaneRadiusValue <= 30) {
                laneStat = SFCADAS_Driving_NewConstantLaneCvrtLvlStat::RIGHT_VALUE;
                roadStat = SFCADAS_Driving_NewConstantRoadCvrtLvlStat::RIGHT_VALUE;
                value = mMvDrivingLaneRadiusValue - 15;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantLaneCvrtLvlStat(laneStat);
        setSFCADAS_Driving_NewConstantLaneCvrtLvlValue(value);
        setSFCADAS_Driving_NewConstantRoadCvrtLvlStat(roadStat);
        setSFCADAS_Driving_NewConstantRoadCvrtLvlValue(value);
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
            } else if (mMvRightLineStatus == MvRightLineStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::GREEN;
            } else if (mMvRightLineStatus == MvRightLineStatus::WHITE_DASH) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::WHITE_DASH;
            } else if (mMvRightLineStatus == MvRightLineStatus::GREEN_DASH) {
                stat = SFCADAS_Driving_NewConstantRightLaneColorStat::GREEN_DASH;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantRightLaneColorStat(stat);
    }

    // 5.2.2.3.1 횡위치
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
                // default
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
            } else if (mMvVehicleDistanceStatus == MvVehicleDistanceStatus::WHITE) {
                stat = SFCADAS_Driving_NewConstantHeadwayDistColorStat::WHITE;
            } else {
                // OFF
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
                // default
            }
        }
        setSFCADAS_Driving_NewConstantHeadwayDistLongPosStat(stat);
        setSFCADAS_Driving_NewConstantHeadwayDistLongPosValue(value);
    }

    // 5.2.2.5 노면-중앙 (5)
    // 5.2.2.5.1 칼라
    void updateConstantCenterRoadColorStat() {
        SFCADAS_Driving_NewConstantCenterRoadColorStat stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::GREEN;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::GRAY;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::CYAN) {
                stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::CYAN;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::AMBER) {
                stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::AMBER;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::RED) {
                stat = SFCADAS_Driving_NewConstantCenterRoadColorStat::RED;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantCenterRoadColorStat(stat);
    }

    // 5.2.2.5.2 종위치
    void updateConstantCenterRoadLongPosStat() {
        SFCADAS_Driving_NewConstantCenterRoadLongPosStat stat = SFCADAS_Driving_NewConstantCenterRoadLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvCenterRoadSurfaceDistanceValue && mMvCenterRoadSurfaceDistanceValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantCenterRoadLongPosStat::VALUE;
                value = mMvCenterRoadSurfaceDistanceValue;
            } else if (1001 <= mMvCenterRoadSurfaceDistanceValue && mMvCenterRoadSurfaceDistanceValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantCenterRoadLongPosStat::VALUE;
                value = 1000;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantCenterRoadLongPosStat(stat);
        setSFCADAS_Driving_NewConstantCenterRoadLongPosValue(value);
    }

    // 5.2.2.5.3 좌측 횡위치
    void updateConstantCenterRoadLeftLatPosStat() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantCenterRoadLeftLatPosStat stat = SFCADAS_Driving_NewConstantCenterRoadLeftLatPosStat::OFF;
        if (mIsIgnOn == true && (0 <= mMvLeftLineOffsetValue && mMvLeftLineOffsetValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantCenterRoadLeftLatPosStat::VALUE;
            value = mMvLeftLineOffsetValue;
        }
        setSFCADAS_Driving_NewConstantCenterRoadLeftLatPosStat(stat);
        setSFCADAS_Driving_NewConstantCenterRoadLeftLatPosValue(value);
    }

    // 5.2.2.5.1 우측 횡위치
    void updateConstantCenterRoadRightLatPosStat() {
        HUInt64 value = 0;
        SFCADAS_Driving_NewConstantCenterRoadRightLatPosStat stat = SFCADAS_Driving_NewConstantCenterRoadRightLatPosStat::OFF;
        if (mIsIgnOn == true && (0 <= mMvRightLineOffsetValue && mMvRightLineOffsetValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantCenterRoadRightLatPosStat::VALUE;
            value = mMvRightLineOffsetValue;
        }
        setSFCADAS_Driving_NewConstantCenterRoadRightLatPosStat(stat);
        setSFCADAS_Driving_NewConstantCenterRoadRightLatPosValue(value);
    }

    // 5.2.2.6 노면-왼쪽 (6)
    // 5.2.2.6.1 칼라
    void updateConstantLeftRoadColorStat() {
        SFCADAS_Driving_NewConstantLeftRoadColorStat stat = SFCADAS_Driving_NewConstantLeftRoadColorStat::OFF;
        if (mIsIgnOn == true && (mMvLeftRoadSurfaceStatus == MvLeftRoadSurfaceStatus::GREEN)) {
            stat = SFCADAS_Driving_NewConstantLeftRoadColorStat::GREEN;
        }
        setSFCADAS_Driving_NewConstantLeftRoadColorStat(stat);
    }

    // 5.2.2.6.2 횡위치
    void updateConstantLeftRoadLatPosStat() {
        SFCADAS_Driving_NewConstantLeftRoadLatPosStat stat = SFCADAS_Driving_NewConstantLeftRoadLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvLeftLineOffsetValue && mMvLeftLineOffsetValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantLeftRoadLatPosStat::VALUE;
            value = mMvLeftLineOffsetValue;
        }
        setSFCADAS_Driving_NewConstantLeftRoadLatPosStat(stat);
        setSFCADAS_Driving_NewConstantLeftRoadLatPosValue(value);
    }

    // 5.2.2.7.1 노면-오른쪽 (7)
    // 5.2.2.7.1 칼라
    void updateConstantRightRoadColorStat() {
        SFCADAS_Driving_NewConstantRightRoadColorStat stat = SFCADAS_Driving_NewConstantRightRoadColorStat::OFF;
        if (mIsIgnOn == true && (mMvRightRoadSurfaceStatus == MvRightRoadSurfaceStatus::GREEN)) {
            stat = SFCADAS_Driving_NewConstantRightRoadColorStat::GREEN;
        }
        setSFCADAS_Driving_NewConstantRightRoadColorStat(stat);
    }

    // 5.2.2.7.2 횡위치
    void updateConstantRightRoadLatPosStat() {
        SFCADAS_Driving_NewConstantRightRoadLatPosStat stat = SFCADAS_Driving_NewConstantRightRoadLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvRightLineOffsetValue && mMvRightLineOffsetValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantRightRoadLatPosStat::VALUE;
            value = mMvRightLineOffsetValue;
        }
        setSFCADAS_Driving_NewConstantRightRoadLatPosStat(stat);
        setSFCADAS_Driving_NewConstantRightRoadLatPosValue(value);
    }

    // 5.2.2.8 전방 물체 (8)
    // 5.2.2.8.1 칼라
    void updateConstantViewFrontVehicleColorStat() {
        SFCADAS_Driving_NewConstantViewFrontVehicleColorStat stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_PEDESTRIAN;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_PEDESTRIAN;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_CYCLIST;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_CYCLIST;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::DARK_RUBBER_CONE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontVehicleColorStat::BRIGHT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontVehicleColorStat(stat);
    }

    // 5.2.2.8.2 횡위치
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
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontVehicleLatPosValue(value);
    }

    // 5.2.2.8.3 종위치
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
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontVehicleLongPosValue(value);
    }

    // 5.2.2.9 주변 물체-전방 왼쪽 (9)
    // 5.2.2.9.1 칼라
    void updateConstantViewFrontLeftVehicleColorStat() {
        static MvFrontLeftObjectStatus prevMvFrontLeftObjectStatus = MvFrontLeftObjectStatus::OFF;
        SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat stat =
            SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_PEDESTRIAN;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_PEDESTRIAN;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_PEDESTRIAN;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_PEDESTRIAN;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_CYCLIST;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_CYCLIST;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_CYCLIST;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_CYCLIST;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (prevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::DARK_RUBBER_CONE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::BRIGHT_RUBBER_CONE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_IN_RUBBER_CONE;
            } else if (prevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        prevMvFrontLeftObjectStatus = mMvFrontLeftObjectStatus;
        setSFCADAS_Driving_NewConstantViewFrontLeftVehicleColorStat(stat);
    }

    // 5.2.2.9.2 횡위치
    void updateConstantViewFrontLeftVehicleLatPosStat() {
        SFCADAS_Driving_NewConstantViewFrontLeftVehicleLatPosStat stat =
            SFCADAS_Driving_NewConstantViewFrontLeftVehicleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvFrontLeftObjectLateralPositionValue && mMvFrontLeftObjectLateralPositionValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleLatPosStat::VALUE;
            value = mMvFrontLeftObjectLateralPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewFrontLeftVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontLeftVehicleLatPosValue(value);
    }

    // 5.2.2.9.3 종위치
    void updateConstantViewFrontLeftVehicleLongPosStat() {
        SFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat stat =
            SFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvFrontLeftObjectLongitudinalPositionValue && mMvFrontLeftObjectLongitudinalPositionValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat::VALUE;
                value = static_cast<HInt64>(mMvFrontLeftObjectLongitudinalPositionValue);
            } else if (1001 <= mMvFrontLeftObjectLongitudinalPositionValue &&
                       mMvFrontLeftObjectLongitudinalPositionValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat::VALUE;
                value = 1000;
            } else if (1996 <= mMvFrontLeftObjectLongitudinalPositionValue &&
                       mMvFrontLeftObjectLongitudinalPositionValue <= 2045) {
                stat = SFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat::VALUE;
                value = static_cast<HInt64>(mMvFrontLeftObjectLongitudinalPositionValue) - 2046;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontLeftVehicleLongPosValue(value);
    }

    // 5.2.2.1 주변 물체-전방 오른쪽 (10)
    // 5.2.2.1.1 칼라
    void updateConstantViewFrontRightVehicleColorStat() {
        static MvFrontRightObjectStatus prevMvFrontRightObjectStatus = MvFrontRightObjectStatus::OFF;
        SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat stat =
            SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_PEDESTRIAN;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_PEDESTRIAN;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_PEDESTRIAN;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_PEDESTRIAN;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_CYCLIST;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_CYCLIST;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_CYCLIST;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_CYCLIST;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (prevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::DARK_RUBBER_CONE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::BRIGHT_RUBBER_CONE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_IN_RUBBER_CONE;
            } else if (prevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        prevMvFrontRightObjectStatus = mMvFrontRightObjectStatus;
        setSFCADAS_Driving_NewConstantViewFrontRightVehicleColorStat(stat);
    }

    // 5.2.2.1.2 횡위치
    void updateConstantViewFrontRightVehicleLatPosStat() {
        SFCADAS_Driving_NewConstantViewFrontRightVehicleLatPosStat stat =
            SFCADAS_Driving_NewConstantViewFrontRightVehicleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvFrontRightObjectLateralPositionValue && mMvFrontRightObjectLateralPositionValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleLatPosStat::VALUE;
            value = mMvFrontRightObjectLateralPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewFrontRightVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontRightVehicleLatPosValue(value);
    }

    // 5.2.2.1.3 종위치
    void updateConstantViewFrontRightVehicleLongPosStat() {
        SFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat stat =
            SFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mMvFrontRightObjectLongitudinalPositionValue && mMvFrontRightObjectLongitudinalPositionValue <= 1000) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat::VALUE;
                value = static_cast<HInt64>(mMvFrontRightObjectLongitudinalPositionValue);
            } else if (1001 <= mMvFrontRightObjectLongitudinalPositionValue &&
                       mMvFrontRightObjectLongitudinalPositionValue <= 1500) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat::VALUE;
                value = 1000;
            } else if (1996 <= mMvFrontRightObjectLongitudinalPositionValue &&
                       mMvFrontRightObjectLongitudinalPositionValue <= 2045) {
                stat = SFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat::VALUE;
                value = static_cast<HInt64>(mMvFrontRightObjectLongitudinalPositionValue) - 2046;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewFrontRightVehicleLongPosValue(value);
    }

    // 5.2.2.2 주변 물체-왼쪽 (11)
    // 5.2.2.2.1 칼라
    void updateConstantViewLeftVehicleColorStat() {
        static MvLeftObjectStatus prevMvLeftObjectStatus = MvLeftObjectStatus::OFF;
        SFCADAS_Driving_NewConstantViewLeftVehicleColorStat stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY && mMvLeftObjectStatus == MvLeftObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_PEDESTRIAN;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_PEDESTRIAN;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_PEDESTRIAN;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_PEDESTRIAN;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_CYCLIST;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_CYCLIST;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_CYCLIST;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_CYCLIST;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (prevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::DARK_RUBBER_CONE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::BRIGHT_RUBBER_CONE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_IN_RUBBER_CONE;
            } else if (prevMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewLeftVehicleColorStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        prevMvLeftObjectStatus = mMvLeftObjectStatus;
        setSFCADAS_Driving_NewConstantViewLeftVehicleColorStat(stat);
    }

    // 5.2.2.2.2 횡위치
    void updateConstantViewLeftVehicleLatPosStat() {
        SFCADAS_Driving_NewConstantViewLeftVehicleLatPosStat stat = SFCADAS_Driving_NewConstantViewLeftVehicleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvLeftObjectLateralPositionValue && mMvLeftObjectLateralPositionValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantViewLeftVehicleLatPosStat::VALUE;
            value = mMvLeftObjectLateralPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewLeftVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewLeftVehicleLatPosValue(value);
    }

    // 5.2.2.2.3 종위치
    void updateConstantViewLeftVehicleLongPosStat() {
        SFCADAS_Driving_NewConstantViewLeftVehicleLongPosStat stat = SFCADAS_Driving_NewConstantViewLeftVehicleLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvLeftObjectLongitudinalPositionValue && mMvLeftObjectLongitudinalPositionValue <= 150)) {
            stat = SFCADAS_Driving_NewConstantViewLeftVehicleLongPosStat::VALUE;
            value = mMvLeftObjectLongitudinalPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewLeftVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewLeftVehicleLongPosValue(value);
    }

    // 5.2.2.1 주변 물체-오른쪽 (12)
    // 5.2.2.1.1 칼라
    void updateConstantViewRightVehicleColorStat() {
        static MvRightObjectStatus prevMvRightObjectStatus = MvRightObjectStatus::OFF;
        SFCADAS_Driving_NewConstantViewRightVehicleColorStat stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::OFF;
        if (mIsIgnOn == true) {
            if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY && mMvRightObjectStatus == MvRightObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_PEDESTRIAN;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_PEDESTRIAN;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_PEDESTRIAN;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_PEDESTRIAN;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_CYCLIST;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_CYCLIST;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_CYCLIST;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_CYCLIST;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (prevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::DARK_RUBBER_CONE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::BRIGHT_RUBBER_CONE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_IN_RUBBER_CONE;
            } else if (prevMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                stat = SFCADAS_Driving_NewConstantViewRightVehicleColorStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }
        prevMvRightObjectStatus = mMvRightObjectStatus;
        setSFCADAS_Driving_NewConstantViewRightVehicleColorStat(stat);
    }

    // 5.2.2.1.2 횡위치
    void updateConstantViewRightVehicleLatPosStat() {
        SFCADAS_Driving_NewConstantViewRightVehicleLatPosStat stat = SFCADAS_Driving_NewConstantViewRightVehicleLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && (0 <= mMvRightObjectLateralPositionValue && mMvRightObjectLateralPositionValue <= 45)) {
            stat = SFCADAS_Driving_NewConstantViewRightVehicleLatPosStat::VALUE;
            value = mMvRightObjectLateralPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewRightVehicleLatPosStat(stat);
        setSFCADAS_Driving_NewConstantViewRightVehicleLatPosValue(value);
    }

    // 5.2.2.1.3 종위치
    void updateConstantViewRightVehicleLongPosStat() {
        SFCADAS_Driving_NewConstantViewRightVehicleLongPosStat stat = SFCADAS_Driving_NewConstantViewRightVehicleLongPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true &&
            (0 <= mMvRightObjectLongitudinalPositionValue && mMvRightObjectLongitudinalPositionValue <= 150)) {
            stat = SFCADAS_Driving_NewConstantViewRightVehicleLongPosStat::VALUE;
            value = mMvRightObjectLongitudinalPositionValue;
        }
        setSFCADAS_Driving_NewConstantViewRightVehicleLongPosStat(stat);
        setSFCADAS_Driving_NewConstantViewRightVehicleLongPosValue(value);
    }

    // 5.2.2.2 레이더 파형-전방 (13)
    void updateConstantViewFrontWarnStat() {
        SFCADAS_Driving_NewConstantViewFrontWarnStat stat = SFCADAS_Driving_NewConstantViewFrontWarnStat::OFF;
        if (mIsIgnOn == true && mMvFrontRadarWaveStatus == MvFrontRadarWaveStatus::RED_WAVE_WARNING_SYMBOL) {
            stat = SFCADAS_Driving_NewConstantViewFrontWarnStat::WAVE_WARNING;
        }
        setSFCADAS_Driving_NewConstantViewFrontWarnStat(stat);
    }

    // 5.2.2.3 레이더 파형-후방 왼쪽 (14)
    void updateConstantViewRearLeftWarnStat() {
        SFCADAS_Driving_NewConstantViewRearLeftWarnStat stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::WAVE_WARNING;
            } else if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE ||
                       mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewRearLeftWarnStat::WAVE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewRearLeftWarnStat(stat);
    }

    // 5.2.2.1 레이더 파형-후방 오른쪽 (15)
    void updateConstantViewRearRightWarnStat() {
        SFCADAS_Driving_NewConstantViewRearRightWarnStat stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::WAVE_WARNING;
            } else if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE ||
                       mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE) {
                stat = SFCADAS_Driving_NewConstantViewRearRightWarnStat::WAVE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewRearRightWarnStat(stat);
    }

    // 5.2.2.16 차로 변경 방향-왼쪽
    // 5.2.2.16.1 타입
    void updateConstantViewLeftArrowStat() {
        SFCADAS_Driving_NewConstantViewLeftArrowStat stat = SFCADAS_Driving_NewConstantViewLeftArrowStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvLeftLaneChangeDirectionStatus == MvLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCADAS_Driving_NewConstantViewLeftArrowStat::GREEN_ANIMATION;
            } else if (mMvLeftLaneChangeDirectionStatus == MvLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION_TYPE_B) {
                stat = SFCADAS_Driving_NewConstantViewLeftArrowStat::GREEN_ANIMATION_TYPE_B;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewLeftArrowStat(stat);
    }

    // 5.2.2.16.2 횡위치
    void updateConstantViewLeftArrowLatValue() {
        SFCADAS_Driving_NewConstantViewLeftArrowLatStat stat = SFCADAS_Driving_NewConstantViewLeftArrowLatStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_Driving_NewConstantViewLeftArrowLatStat::CENTER;
            } else if (mMvLaneChangeRectangleLateralPositionValue >= 0x42 && mMvLaneChangeRectangleLateralPositionValue <= 0x7F) {
                // 0x7f -> -1, 0x42 -> -62
                stat = SFCADAS_Driving_NewConstantViewLeftArrowLatStat::LEFT;
                value = (mMvLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewLeftArrowLatStat(stat);
        setSFCADAS_Driving_NewConstantViewLeftArrowLatValue(value);
    }

    // 5.2.2.17 차로 변경 방향-오른쪽
    // 5.2.2.17.1 타입
    void updateConstantViewRightArrowStat() {
        SFCADAS_Driving_NewConstantViewRightArrowStat stat = SFCADAS_Driving_NewConstantViewRightArrowStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvRightLaneChangeDirectionStatus == MvRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCADAS_Driving_NewConstantViewRightArrowStat::GREEN_ANIMATION;
            } else if (mMvRightLaneChangeDirectionStatus == MvRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION_TYPE_B) {
                stat = SFCADAS_Driving_NewConstantViewRightArrowStat::GREEN_ANIMATION_TYPE_B;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewRightArrowStat(stat);
    }

    // 5.2.2.17.2 횡위치
    void updateConstantViewRightArrowLatValue() {
        SFCADAS_Driving_NewConstantViewRightArrowLatStat stat = SFCADAS_Driving_NewConstantViewRightArrowLatStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_Driving_NewConstantViewRightArrowLatStat::CENTER;
            } else if (mMvLaneChangeRectangleLateralPositionValue >= 1 && mMvLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCADAS_Driving_NewConstantViewRightArrowLatStat::RIGHT;
                value = mMvLaneChangeRectangleLateralPositionValue;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantViewRightArrowLatStat(stat);
        setSFCADAS_Driving_NewConstantViewRightArrowLatValue(value);
    }

    // 5.2.2.2 자차 테두리 (18)
    void updateConstantMainVehicleStat() {
        SFCADAS_Driving_NewConstantADASViewMainVehicleStat stat = SFCADAS_Driving_NewConstantADASViewMainVehicleStat::OFF;
        if (mIsIgnOn == true) {
            if (mMvCircleAroundHostVehicleStatus == MvCircleAroundHostVehicleStatus::CYAN) {
                stat = SFCADAS_Driving_NewConstantADASViewMainVehicleStat::CYAN;
            } else if (mMvCircleAroundHostVehicleStatus == MvCircleAroundHostVehicleStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantADASViewMainVehicleStat::GREEN;
            } else if (mMvCircleAroundHostVehicleStatus == MvCircleAroundHostVehicleStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantADASViewMainVehicleStat::GRAY;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantADASViewMainVehicleStat(stat);
    }

    // 5.2.3 교통 표지판
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
                            // OFF
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA) {
                        if (mIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE4_INVALID;
                        } else if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE4_VALID;
                        } else {
                            // OFF
                        }
                    } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA) {
                        if (mIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE5_INVALID;
                        } else if (1 <= mIslaSpeedLimitTrafficSignValue && mIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE5_VALID;
                        } else {
                            // OFF
                        }
                    } else {
                        // OFF
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
                        // OFF
                    }
                } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::AMBER) {
                    if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                    } else if (!(mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA)) {
                        stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                    } else {
                        // OFF
                    }
                } else if (mIslaSpeedLimitTrafficSignStatus == IslaSpeedLimitTrafficSignStatus::UNHANDLED_TIMEOUT) {
                    if (mConfigAdasISLA == ConfigAdasISLA::ON) {
                        if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                        } else if (!(mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA)) {
                            stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                        } else {
                            // OFF
                        }
                    }
                } else {
                    // OFF
                }
            } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
                if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                    stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE2_INVALID;
                } else if (!(mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA)) {
                    stat = SFCADAS_Driving_NewConstantISLAMainSpeedStat::TYPE1_INVALID;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else {
            // OFF
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
                // default
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
                    // OFF
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
                    // OFF
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
                    // OFF
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
                    // OFF
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
                    // OFF
                }
            } else if (mIslaAdditionalTrafficSignStatus == IslaAdditionalTrafficSignStatus::ROUNDABOUT_RHD) {
                if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                    mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU;
                } else if (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_AUS;
                } else {
                    // OFF
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
                    // OFF
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
                    // OFF
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
                    // OFF
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
                    // OFF
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
                    // OFF
                }
            } else {
                // OFF
            }
        }
        updateConstantFinalDISISLAAddtnlSignStat();
    }

    // 5.2.3.2.2 ISLA/DIS 우선 순위 판단 로직
    void updateConstantFinalDISISLAAddtnlSignStat() {
        static Private_ISLAAddtnlSign prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
        static DISAddtnlSign prevDISAddtnlSign = DISAddtnlSign::OFF;
        if (mIsIgnOn == true) {
            if (mPrivate_ISLAAddtnlSign == Private_ISLAAddtnlSign::OFF) {
                if (mDISAddtnlSign == DISAddtnlSign::OFF) {
                    if (!mPriorityDISFlag && !mPriorityISLAFlag) {
                        setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(SFCADAS_Driving_NewConstantFinalAddtnlSignStat::OFF);
                        setSFCADAS_Driving_NewConstantDISAddtnlSignStat(SFCADAS_Driving_NewConstantDISAddtnlSignStat::OFF);
                        setSFCADAS_Driving_NewConstantISLAAddtnlSignStat(SFCADAS_Driving_NewConstantISLAAddtnlSignStat::OFF);
                        prevDISAddtnlSign = DISAddtnlSign::OFF;
                        prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
                    }
                } else {
                    // Inter_DISAddtnlSign → at least 300ms
                    if (!mPriorityISLAFlag && !mPriorityDISFlag) {
                        setSFCADAS_Driving_NewConstantDISAddtnlSignStat(
                            static_cast<SFCADAS_Driving_NewConstantDISAddtnlSignStat>(mDISAddtnlSign));
                        setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(
                            SFCADAS_Driving_NewConstantFinalAddtnlSignStat::DISADDTNLSIGN_STAT);

                        mOneShotTimerPriorityDIS.stop();

                        if (prevDISAddtnlSign != mDISAddtnlSign) {
                            mPriorityDISFlag = true;
                            if (mOneShotTimerPriorityDIS.start() == false) {
                                DWarning() << "[ADAS_Driving_New] mOneShotTimerPriorityDIS Start Failed";
                            }
                            prevDISAddtnlSign = mDISAddtnlSign;
                        }
                    }
                }
            } else {
                // Private_ISLAAddtnlSign → at least 300ms
                if (!mPriorityISLAFlag && !mPriorityDISFlag) {
                    setSFCADAS_Driving_NewConstantFinalAddtnlSignStat(
                        SFCADAS_Driving_NewConstantFinalAddtnlSignStat::ISLAADDTNLSIGN_STAT);
                    setSFCADAS_Driving_NewConstantISLAAddtnlSignStat(mPrivate_ISLAAddtnlSign);

                    mOneShotTimerPriorityISLA.stop();

                    if (prevPrivate_ISLAAddtnlSign != mPrivate_ISLAAddtnlSign) {
                        mPriorityISLAFlag = true;
                        if (mOneShotTimerPriorityISLA.start() == false) {
                            DWarning() << "[ADAS_Driving_New] mOneShotTimerPriorityISLA Start Failed";
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
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantISLAAddSignStat(stat);
    }

    // 5.2.3.4 과속 경고음 ON/OFF 심볼 (4)
    void updateConsstantIslaSoundSymbolStat() {
        SFCADAS_Driving_NewConstantISLASoundSymbolStat stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::OFF;
        if (mIsIgnOn == true) {
            if (mIslaWarningSoundSymboleStatus == IslaWarningSoundSymboleStatus::DISABLE) {
                stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::DISABLE;
            } else if (mIslaWarningSoundSymboleStatus == IslaWarningSoundSymboleStatus::ENABLE) {
                stat = SFCADAS_Driving_NewConstantISLASoundSymbolStat::ENABLE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantISLASoundSymbolStat(stat);
    }

    // 5.2.3.5 사전 제한 속도 표지판 (5)
    // 5.2.3.5.1 형상
    void updateConstantISLANextSpeedLimitTrafficSignTypeStat() {
        SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat stat =
            SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::OFF;
        if (mIsIgnOn == true) {
            if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                 mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE1;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE2;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE3;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::EUROPE ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CHINA ||
                        mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE1;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE2;
            } else if ((mIslaNextSpeedLimitTrafficSignStatus == IslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mIslaTrafficSignCountryInformationStatus == IslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE3;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignTypeStat(stat);
    }

    // 5.2.3.5.2 제한속도
    void updateConstantISLANextSpeedLimitTrafficSignStat() {
        SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignStat stat =
            SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mIslaNextSpeedLimitTrafficSignValue == 0) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignStat::INVALID;
            } else if (1 <= mIslaNextSpeedLimitTrafficSignValue && mIslaNextSpeedLimitTrafficSignValue <= 252) {
                stat = SFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignStat::VALUE;
                value = mIslaNextSpeedLimitTrafficSignValue;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignStat(stat);
        setSFCADAS_Driving_NewConstantISLANextSpeedLimitTrafficSignValue(value);
    }

    // 5.2.3.6 사전 제한 속도 보조 표지판 (6)
    // 5.2.3.6.1 형상
    void updateConstantISLANextSupportTrafficSignUnitStat() {
        SFCADAS_Driving_NewConstantISLANextSupportTrafficSignUnitStat stat =
            SFCADAS_Driving_NewConstantISLANextSupportTrafficSignUnitStat::OFF;
        if (mIsIgnOn == true) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCADAS_Driving_NewConstantISLANextSupportTrafficSignUnitStat::METER;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCADAS_Driving_NewConstantISLANextSupportTrafficSignUnitStat::FEET;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantISLANextSupportTrafficSignUnitStat(stat);
    }

    // 5.2.3.6.2 거리
    void updateConstantISLANextSupportTrafficSignStat() {
        SFCADAS_Driving_NewConstantISLANextSupportTrafficSignStat stat =
            SFCADAS_Driving_NewConstantISLANextSupportTrafficSignStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if ((1 <= mIslaNextSupportTrafficSignValue && mIslaNextSupportTrafficSignValue <= 50) &&
                (mTripDistanceUnit == TripDistanceUnit::KM)) {
                stat = SFCADAS_Driving_NewConstantISLANextSupportTrafficSignStat::VALUE;
                value = mIslaNextSupportTrafficSignValue * 10;
            } else if ((1 <= mIslaNextSupportTrafficSignValue && mIslaNextSupportTrafficSignValue <= 50) &&
                       (mTripDistanceUnit == TripDistanceUnit::MILE)) {
                stat = SFCADAS_Driving_NewConstantISLANextSupportTrafficSignStat::VALUE;
                value = mIslaNextSupportTrafficSignValue * 30;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantISLANextSupportTrafficSignStat(stat);
        setSFCADAS_Driving_NewConstantISLANextSupportTrafficSignValue(value);
    }

    // 5.2.3.7 보조 표지판 2 (7)
    void updateConstantISLAAddSign2Stat() {
        SFCADAS_Driving_NewConstantISLAAddSign2Stat stat = SFCADAS_Driving_NewConstantISLAAddSign2Stat::OFF;
        if (mIsIgnOn == true) {
            if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::RAIN) {
                stat = SFCADAS_Driving_NewConstantISLAAddSign2Stat::RAIN;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::SNOW) {
                stat = SFCADAS_Driving_NewConstantISLAAddSign2Stat::SNOW;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::SNOW_RAIN) {
                stat = SFCADAS_Driving_NewConstantISLAAddSign2Stat::SNOW_RAIN;
            } else if (mIslaSupplementaryTrafficSign2Status == IslaSupplementaryTrafficSign2Status::TRAILER) {
                stat = SFCADAS_Driving_NewConstantISLAAddSign2Stat::TRAILER;
            } else {
                // default
            }
        }
        setSFCADAS_Driving_NewConstantISLAAddSign2Stat(stat);
    }

    // 5.2.4 HDP 배경 제어
    // 5.2.4.1 클러스터 테두리
    void updateConstantHDPBackGroundOutlineStat() {
        SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::OFF;

        if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            if (mBGHDPStatus == BGHDPStatus::CYAN) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::CYAN;
            } else if (mBGHDPStatus == BGHDPStatus::GREEN) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::GREEN;
            } else if (mBGHDPStatus == BGHDPStatus::GRAY) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::GRAY;
            } else if (mBGHDPStatus == BGHDPStatus::AMBER || mBGHDPStatus == BGHDPStatus::AMBER_BLINK) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::AMBER;
            } else if (mBGHDPStatus == BGHDPStatus::RED_BLINK) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::RED;
            } else if (mBGHDPStatus == BGHDPStatus::RED) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::RED;
            } else {
                // OFF
            }
        } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
            stat = SFCADAS_Driving_NewConstantHDPBackgroundOutlineStat::RED;
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewConstantHDPBackgroundOutlineStat(stat);
    }

    // 5.2.4.2 클러스터 배경
    void updateConstantHDPBackGroundStat() {
        SFCADAS_Driving_NewConstantHDPBackgroundStat stat = SFCADAS_Driving_NewConstantHDPBackgroundStat::OFF;

        if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::OFF) {
            if (mBGHDPStatus == BGHDPStatus::AMBER_BLINK) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundStat::AMBER_BLINK;
            } else if (mBGHDPStatus == BGHDPStatus::RED_BLINK) {
                stat = SFCADAS_Driving_NewConstantHDPBackgroundStat::RED_BLINK;
            } else {
                // OFF
            }
        } else if (mPrivate_HDPRedundancyStatus_1 == Private_HDPRedundancyStatus_1::ON) {
            stat = SFCADAS_Driving_NewConstantHDPBackgroundStat::RED_BLINK;
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewConstantHDPBackgroundStat(stat);
    }

    // 5.2.5.1 전방 안전 고장
    void updateConstantForwardSafetyFailure() {
        SFCADAS_Driving_NewConstantForwardSafetyFailureStat stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::OFF;

        if (mIsIgnOn == true) {
            if ((mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON)) ||
                (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7ForwardSafetyFailureStatus == PuFGroup7ForwardSafetyFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantForwardSafetyFailureStat(stat);
    }

    // 5.2.5.2 전방/측방 안전 고장
    void updateConstantForwardSideSafetyFailure() {
        SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat stat =
            SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::OFF;

        if (mIsIgnOn == true) {
            if ((mPuFGroup7ForwardSideSafetyFailureStatus == PuFGroup7ForwardSideSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasFCA2 == ConfigAdasFCA2::ON) ||
                (mPuFGroup7ForwardSideSafetyFailureStatus == PuFGroup7ForwardSideSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7ForwardSideSafetyFailureStatus == PuFGroup7ForwardSideSafetyFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7ForwardSideSafetyFailureStatus == PuFGroup7ForwardSideSafetyFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantForwardSideSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantForwardSideSafetyFailureStat(stat);
    }

    // 5.2.5.3 차로 안전 고장
    void updateConstantLaneSafetyFailure() {
        SFCADAS_Driving_NewConstantLaneSafetyFailureStat stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasLKA == ConfigAdasLKA::ON) ||
                (mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7LaneSafetyFailureStatus == PuFGroup7LaneSafetyFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantLaneSafetyFailureStat::CAMERA_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantLaneSafetyFailureStat(stat);
    }

    // 5.2.5.4 후측방 안전 고장
    void updateConstantBlindSpotSafetyFailure() {
        SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigNewBCW == ConfigNewBCW::ON) ||
                mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::FAILURE) {
                stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::FAILURE;
            } else if (mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantBlindSpotSafetyFailureStat(stat);
    }

    // 5.2.5.5 속도 제한 고장
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
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantIslaFailureStat(stat);
    }

    // 5.2.5.6 부주의 운전 경고 고장
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
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantDawFailureStat(stat);
    }

    // 5.2.5.7 하이빔 보조 고장
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

    // 5.2.5.8 스마트 크루즈 컨트롤 고장
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
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantSccFailureStat(stat);
    }

    // 5.2.5.9 차로 유지 보조 고장
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

    // 5.2.5.10 고속도로 주행 보조 고장
    void updateConstantHdaFailure() {
        SFCADAS_Driving_NewConstantHdaFailureStat stat = SFCADAS_Driving_NewConstantHdaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7HdaFailureStatus == PuFGroup7HdaFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA == ConfigAdasHDA::ON || mConfigAdasHDA2 == ConfigAdasHDA2::ON ||
                  mConfigAdasHDP == ConfigAdasHDP::ON || mConfigAdasHDA3 == ConfigAdasHDA3::ON)) ||
                (mPuFGroup7HdaFailureStatus == PuFGroup7HdaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantHdaFailureStat::FAILURE;
            }
        }
        setSFCADAS_Driving_NewConstantHdaFailureStat(stat);
    }

    // 5.2.5.11 차로 변경 보조 고장
    void updateConstantLcaFailure() {
        SFCADAS_Driving_NewConstantLcaFailureStat stat = SFCADAS_Driving_NewConstantLcaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7LcaFailureStatus == PuFGroup7LcaFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA2 == ConfigAdasHDA2::ON || mConfigAdasHDP == ConfigAdasHDP::ON ||
                  mConfigAdasHDA3 == ConfigAdasHDA3::ON)) ||
                (mPuFGroup7LcaFailureStatus == PuFGroup7LcaFailureStatus::FAILURE)) {
                stat = SFCADAS_Driving_NewConstantLcaFailureStat::FAILURE;
            } else if (mPuFGroup7LcaFailureStatus == PuFGroup7LcaFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantLcaFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7LcaFailureStatus == PuFGroup7LcaFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantLcaFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantLcaFailureStat(stat);
    }

    // 5.2.5.12 비상 정지 기능 고장
    void updateConstantEmergencyStopFailure() {
        SFCADAS_Driving_NewConstantEmergencyStopFailureStat stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasEmergencyStop == ConfigAdasEmergencyStop::ON) ||
                mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::FAILURE) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::FAILURE;
            } else if (mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7MRMFailureStatus == PuFGroup7MRMFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantEmergencyStopFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_Driving_NewConstantEmergencyStopFailureStat(stat);
    }

    // 5.2.5.13 고속도로 주행 보조 핸즈프리 고장
    void updateConstantHda3HandsFreeFailure() {
        SFCADAS_Driving_NewConstantHda3HandsFreeFailureStat stat = SFCADAS_Driving_NewConstantHda3HandsFreeFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7HandsFreeFailureStatus == PuFGroup7HandsFreeFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA3 == ConfigAdasHDA3::ON || mConfigAdasHDP == ConfigAdasHDP::ON)) ||
                mPuFGroup7HandsFreeFailureStatus == PuFGroup7HandsFreeFailureStatus::FAILURE) {
                stat = SFCADAS_Driving_NewConstantHda3HandsFreeFailureStat::FAILURE;
            } else if (mPuFGroup7HandsFreeFailureStatus == PuFGroup7HandsFreeFailureStatus::CAMERA_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantHda3HandsFreeFailureStat::CAMERA_BLOCKAGE;
            } else if (mPuFGroup7HandsFreeFailureStatus == PuFGroup7HandsFreeFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_Driving_NewConstantHda3HandsFreeFailureStat::RADAR_BLOCKAGE;
            } else {
                // OFF
            }
        }
        setSFCADAS_Driving_NewConstantHda3HandsFreeFailureStat(stat);
    }

    // 5.2.6 HDP 상품명 구분 사양
    void updateConstantHDPProductDivisionAdditionalInformation() {
        updateConstant1GroupFullPopup1_2AdditionalInformation();
        updateConstant1GroupFullPopup3_1AdditionalInformation();
        updateConstant4GroupFullPopup1_1AdditionalInformation();
        updateConstant7GroupFullPopupAdditionalInformation();
        updateConstant2GroupMiniPopup1_2AdditionalInformation();
        updateConstantHDP_ProhibitAdditionalInformation();
    }

    // 5.2.6.1 1그룹 풀팝업 1-2
    void updateConstant1GroupFullPopup1_2AdditionalInformation() {
        if (mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52070Value(SFCADAS_Driving_NewConstantHDP_E52070Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52074Value(SFCADAS_Driving_NewConstantHDP_E52074Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52076Value(SFCADAS_Driving_NewConstantHDP_E52076Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52096Value(SFCADAS_Driving_NewConstantHDP_E52096Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52070Value(SFCADAS_Driving_NewConstantHDP_E52070Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52074Value(SFCADAS_Driving_NewConstantHDP_E52074Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52076Value(SFCADAS_Driving_NewConstantHDP_E52076Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52096Value(SFCADAS_Driving_NewConstantHDP_E52096Value::TYPE_A);
        }
    }

    // 5.2.6.2 1그룹 풀팝업 3-1
    void updateConstant1GroupFullPopup3_1AdditionalInformation() {
        if (mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52100Value(SFCADAS_Driving_NewConstantHDP_E52100Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52101Value(SFCADAS_Driving_NewConstantHDP_E52101Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52100Value(SFCADAS_Driving_NewConstantHDP_E52100Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52101Value(SFCADAS_Driving_NewConstantHDP_E52101Value::TYPE_A);
        }
    }

    // 5.2.6.3 4그룹 풀팝업 1-1
    void updateConstant4GroupFullPopup1_1AdditionalInformation() {
        if (mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52082Value(SFCADAS_Driving_NewConstantHDP_E52082Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52083Value(SFCADAS_Driving_NewConstantHDP_E52083Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52084Value(SFCADAS_Driving_NewConstantHDP_E52084Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52095Value(SFCADAS_Driving_NewConstantHDP_E52095Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52108Value(SFCADAS_Driving_NewConstantHDP_E52108Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52082Value(SFCADAS_Driving_NewConstantHDP_E52082Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52083Value(SFCADAS_Driving_NewConstantHDP_E52083Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52084Value(SFCADAS_Driving_NewConstantHDP_E52084Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52095Value(SFCADAS_Driving_NewConstantHDP_E52095Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52108Value(SFCADAS_Driving_NewConstantHDP_E52108Value::TYPE_A);
        }
    }

    // 5.2.6.4 7그룹 풀팝업 - 고속도로 자율주행 고장
    void updateConstant7GroupFullPopupAdditionalInformation() {
        if (mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52077Value(SFCADAS_Driving_NewConstantHDP_E52077Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52085Value(SFCADAS_Driving_NewConstantHDP_E52085Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52086Value(SFCADAS_Driving_NewConstantHDP_E52086Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52087Value(SFCADAS_Driving_NewConstantHDP_E52087Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52077Value(SFCADAS_Driving_NewConstantHDP_E52077Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52085Value(SFCADAS_Driving_NewConstantHDP_E52085Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52086Value(SFCADAS_Driving_NewConstantHDP_E52086Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52087Value(SFCADAS_Driving_NewConstantHDP_E52087Value::TYPE_A);
        }
    }

    // 5.2.6.5 2그룹 미니 팝업 1-2
    void updateConstant2GroupMiniPopup1_2AdditionalInformation() {
        if (mIsIgnOn == true && mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52523Value(SFCADAS_Driving_NewConstantHDP_E52523Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52524Value(SFCADAS_Driving_NewConstantHDP_E52524Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52598Value(SFCADAS_Driving_NewConstantHDP_E52598Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52527Value(SFCADAS_Driving_NewConstantHDP_E52527Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52528Value(SFCADAS_Driving_NewConstantHDP_E52528Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52547Value(SFCADAS_Driving_NewConstantHDP_E52547Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52559Value(SFCADAS_Driving_NewConstantHDP_E52559Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52560Value(SFCADAS_Driving_NewConstantHDP_E52560Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52561Value(SFCADAS_Driving_NewConstantHDP_E52561Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52562Value(SFCADAS_Driving_NewConstantHDP_E52562Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52608Value(SFCADAS_Driving_NewConstantHDP_E52608Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52616Value(SFCADAS_Driving_NewConstantHDP_E52616Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52625Value(SFCADAS_Driving_NewConstantHDP_E52625Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52591Value(SFCADAS_Driving_NewConstantHDP_E52591Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52626Value(SFCADAS_Driving_NewConstantHDP_E52626Value::TYPE_B);
            setSFCADAS_Driving_NewConstantHDP_E52637Value(SFCADAS_Driving_NewConstantHDP_E52637Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52523Value(SFCADAS_Driving_NewConstantHDP_E52523Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52524Value(SFCADAS_Driving_NewConstantHDP_E52524Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52598Value(SFCADAS_Driving_NewConstantHDP_E52598Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52527Value(SFCADAS_Driving_NewConstantHDP_E52527Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52528Value(SFCADAS_Driving_NewConstantHDP_E52528Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52547Value(SFCADAS_Driving_NewConstantHDP_E52547Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52559Value(SFCADAS_Driving_NewConstantHDP_E52559Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52560Value(SFCADAS_Driving_NewConstantHDP_E52560Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52561Value(SFCADAS_Driving_NewConstantHDP_E52561Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52562Value(SFCADAS_Driving_NewConstantHDP_E52562Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52608Value(SFCADAS_Driving_NewConstantHDP_E52608Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52616Value(SFCADAS_Driving_NewConstantHDP_E52616Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52625Value(SFCADAS_Driving_NewConstantHDP_E52625Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52591Value(SFCADAS_Driving_NewConstantHDP_E52591Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52626Value(SFCADAS_Driving_NewConstantHDP_E52626Value::TYPE_A);
            setSFCADAS_Driving_NewConstantHDP_E52637Value(SFCADAS_Driving_NewConstantHDP_E52637Value::TYPE_A);
        }
    }

    // 5.2.6.6 HDP 동작 시 뷰 전환 불가 팝업
    void updateConstantHDP_ProhibitAdditionalInformation() {
        if (mIsIgnOn == true && mConfigHDPName == ConfigHDPName::TYPE_B) {
            setSFCADAS_Driving_NewConstantHDP_E52643Value(SFCADAS_Driving_NewConstantHDP_E52643Value::TYPE_B);
        } else {
            setSFCADAS_Driving_NewConstantHDP_E52643Value(SFCADAS_Driving_NewConstantHDP_E52643Value::TYPE_A);
        }
    }

    // 5.3 Event
    // 5.3.1 주행 보조 요약 정보
    void updateEventDrivingAssistSummary() {
        SFCADAS_Driving_NewEventDrivingAssistSummaryStat eventStat = SFCADAS_Driving_NewEventDrivingAssistSummaryStat::OFF;

        if (mSummaryFrontVehicleStat != SFCADAS_Driving_NewConstantFrontVehicleColorStat::OFF ||    // IMG_ADASFrontVehicle_Stat
            mSummaryFrontVehicleValue != SFCADAS_Driving_NewConstantFrontVehicleLevelValue::OFF ||  // IMG_ADAS_FrontVehicle_Value
            mSummaryHeadwayDistanceSetStat !=
                SFCADAS_Driving_NewConstantHeadwayDistanceColorStat::OFF ||  // IMG_ADASHeadwayDistanceSet_Stat
            mSummaryHeadwayDistanceSetVal !=
                SFCADAS_Driving_NewConstantHeadwayDistanceLevelValue::OFF ||                  // IMG_ADASHeadwayDistanceSet_Val
            mSummarySubVehicleStat != SFCADAS_Driving_NewConstantSubVehicleColorStat::OFF ||  // IMG_ADASSubVehicle_Stat
            mSummarySetSpeedStat != SFCADAS_Driving_NewConstantSetSpeedColorStat::OFF ||      // IMG_ADASSetSpeed_Stat
            mSummaryHDAHDPTelltaleStat != SFCADAS_Driving_NewConstantHDAHDPStat::OFF ||       // IMG_HDAHDPTelltale_Stat
            mSummaryISLAManualSetSpeedStat !=
                SFCADAS_Driving_NewConstantISLAManualSetSpeedStat::OFF ||                          // IMG_ISLAManualSetSpeed_Stat
            mSummaryNSCCAutoSetTelltaleStat != SFCADAS_Driving_NewConstantNSCCAutoSetStat::OFF) {  // IMG_NSCCAutoSetTelltale_Stat
            eventStat = SFCADAS_Driving_NewEventDrivingAssistSummaryStat::ON;
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
                case PuFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT:
                    eventID = "E52003";
                    break;
                case PuFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT:
                    eventID = "E52004";
                    break;
                case PuFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT_CROSSING:
                    eventID = "E52005";
                    break;
                case PuFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT_CROSSING:
                    eventID = "E52006";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT:
                    eventID = "E52007";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT:
                    eventID = "E52008";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT:
                    eventID = "E52009";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT_CROSSING:
                    eventID = "E52010";
                    break;
                case PuFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT_CROSSING:
                    eventID = "E52011";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT:
                    eventID = "E52012";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT:
                    eventID = "E52013";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT_PATH:
                    eventID = "E52014";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT_PATH:
                    eventID = "E52015";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_REAR_LEFT:
                    eventID = "E52016";
                    break;
                case PuFGroup1AdasWarning1_1Status::STEERING_REAR_RIGHT:
                    eventID = "E52017";
                    break;
                case PuFGroup1AdasWarning1_1Status::END_OF_ASSIST:
                    eventID = "E52018";
                    break;
                default:
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
        std::string linkedSoundID;
        SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType linkedSoundType =
            SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
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
                case PuFGroup1AdasWarning1_2Status::LCA_MANUAL_ON_REQUEST:
                    eventID = "E52069";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_TD_LV1:
                    eventID = "E52072";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_TD_LV2:
                    eventID = "E52073";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_WARNING:
                    eventID = "E52075";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_CONDITION_NO_MET:
                    eventID = "E52076";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_EM_IN_DDT:
                    eventID = "E52096";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_SET_THE_WIPER_LIGHT_AUTO:
                    eventID = "E52097";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_BE_PREPARED_TO_TAKEOVER:
                    eventID = "E52098";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_EM:
                    eventID = "E52070";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_MRM:
                    eventID = "E52071";
                    break;
                case PuFGroup1AdasWarning1_2Status::EMERGENCY_STOP:
                    eventID = "E52113";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_INACTIVE:
                    eventID = "E52074";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup1AdasWarning1_2Status::EMERGENCY_STOP_INACTIVE:
                    eventID = "E52110";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDA_HF_MANUAL_ON_REQUEST:
                    eventID = "E52114";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL1:
                    eventID = "E52116";
                    break;
                case PuFGroup1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL2:
                    eventID = "E52117";
                    break;
                case PuFGroup1AdasWarning1_2Status::DAW_TAKE_REST_WITH_OK_BUTTON:
                    eventID = "E52119";
                    break;
                case PuFGroup1AdasWarning1_2Status::TD_LV2_IN_BIGGER:
                    eventID = "E52120";
                    break;
                case PuFGroup1AdasWarning1_2Status::EMERGENCY_STOP_ACTIVE_IN_BIGGER:
                    eventID = "E52121";
                    break;
                default:
                    break;
            }
        } else {
            switch (mPuFGroup1AdasWarning1_2Status) {
                case PuFGroup1AdasWarning1_2Status::HDP_EM:
                    eventID = "E52102";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_MRM:
                    eventID = "E52103";
                    break;
                case PuFGroup1AdasWarning1_2Status::HDP_INACTIVE:
                    eventID = "E52104";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                default:
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
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundID(linkedSoundID);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundType(linkedSoundType);
            setSFCADAS_Driving_NewEventGroup1FullPopup1_2LinkedSoundRepeatCount(repeatCount);
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

    // 5.3.4 1그룹 풀 팝업 2-1 (RR_C_RDR, ADAS_DRV)
    void updateEventGroup1FullPopup2_1() {
        std::string prevEventID;
        std::string eventID;
        SFCADAS_Driving_NewEventGroup1FullPopup2_1Stat stat = SFCADAS_Driving_NewEventGroup1FullPopup2_1Stat::OFF;
        if (mPuFGroup1AdasWarning2_1Status == PuFGroup1AdasWarning2_1Status::WARNING_CHECK_BLIND_SPOT) {
            stat = SFCADAS_Driving_NewEventGroup1FullPopup2_1Stat::ON;
            if (mIsIgnOn == true) {
                eventID = "E52030";
            } else {
                eventID = "E52094";
            }
        }
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group1FullPopup2_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup1FullPopup2_1Stat(SFCADAS_Driving_NewEventGroup1FullPopup2_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup1FullPopup2_1ID(eventID);
            setSFCADAS_Driving_NewEventGroup1FullPopup2_1Stat(stat);
        }
    }

    // 5.3.5 1그룹 풀 팝업 3-1 (RDD)
    void updateEventGroup1FullPopup3_1() {
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099Stat statE52099 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099Stat::OFF;
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52100Stat statE52100 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52100Stat::OFF;
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101Stat statE52101 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101Stat::OFF;
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105Stat statE52105 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105Stat::OFF;
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52106Stat statE52106 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52106Stat::OFF;
        SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107Stat statE52107 =
            SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107Stat::OFF;

        // E52099 && E52105
        if (mHDP_RDDWarningState == HDP_RDDWarningState::MRM_WARNING_REQUEST ||
            (mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_RCU_ALONE) ||
            (mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_RCU_ALONE)) {
            if (mIsIgnOn == true) {
                statE52099 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099Stat::ON;
            } else {
                statE52105 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105Stat::ON;
            }
        }
        // E52100 && E52106
        if (mHDP_RDDWarningState == HDP_RDDWarningState::EM_WARNING_REQUEST) {
            if (mIsIgnOn == true) {
                statE52100 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52100Stat::ON;
            } else {
                statE52106 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52106Stat::ON;
            }
        }
        // E52101 && E52107
        if (mHDP_RDDWarningState == HDP_RDDWarningState::MRM_STOP ||
            ((mPrevESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
              mPrevESC_HDPEngagedStatus == ESC_HDPEngagedStatus::MRM_WITH_RCU_ALONE) &&
             mESC_HDPEngagedStatus == ESC_HDPEngagedStatus::NOT_ENGAGED) ||
            ((mPrevRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_IEB_ALONE ||
              mPrevRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::MRM_WITH_RCU_ALONE) &&
             mRCU_HDPEngagedStatus == RCU_HDPEngagedStatus::NOT_ENGAGED)) {
            if (mIsIgnOn == true) {
                statE52101 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101Stat::ON;
            } else {
                statE52107 = SFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107Stat::ON;
            }
        }
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52099Stat(statE52099);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52105Stat(statE52105);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52100Stat(statE52100);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52106Stat(statE52106);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52101Stat(statE52101);
        setSFCADAS_Driving_NewEventGroup1FullPopup3_1_E52107Stat(statE52107);
    }

    // 5.3.6 4그룹 풀 팝업 1-1 (FR_CMR, ADAS_DRV)
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
                case PuFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET:
                    eventID = "E52040";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::SCC_STOP_CONTROL:
                    eventID = "E52035";
                    break;
                case PuFGroup4AdasWarning1_1Status::CONNECTING_TRAILER_OFF:
                    eventID = "E52036";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
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
                case PuFGroup4AdasWarning1_1Status::HDP_INITIAL_ACTIVATION:
                    eventID = "E52082";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::HDP_CHECK_PATH:
                    eventID = "E52083";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::HDP_INACTIVE:
                    eventID = "E52084";
                    break;
                case PuFGroup4AdasWarning1_1Status::HDP_NOT_APPLIED:
                    eventID = "E52095";
                    break;
                case PuFGroup4AdasWarning1_1Status::HDP_LV2_CONDITION_NOT_MET:
                    eventID = "E52108";
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
                case PuFGroup4AdasWarning1_1Status::HDA_START_POP_UP:
                    eventID = "E52112";
                    break;
                case PuFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET_BY_EMERGENCY_STOP:
                    eventID = "E52118";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_Driving_NewEventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case PuFGroup4AdasWarning1_1Status::HANDS_FREE_INITIAL_ACTIVATION:
                    eventID = "E52115";
                    break;
                case PuFGroup4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_IIHS:
                    eventID = "E52122";
                    break;
                default:
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

    // 5.3.7 7그룹 풀 팝업 – 후측방 안전 고장
    void updateEventGroup7FullPopupBlindSpotSafetyFailure() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType linkedSoundType =
            SFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            if (mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::CONNECTING_TRAILER_OFF) {
                eventID = "E52050";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mPuFGroup7BlindSpotSafetyFailureStatus == PuFGroup7BlindSpotSafetyFailureStatus::DISPLAY_OFF) {
                eventID = "E52051";
            } else {
                // default
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group7FullPopupBlindSpotSafetyFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureStat(
                SFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureID(eventID);
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureStat(
                SFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureStat::ON);
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundID(linkedSoundID);
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType(linkedSoundType);
            setSFCADAS_Driving_NewEventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.3.8 7그룹 풀 팝업 – 사이드 미러 고장
    void updateEventGroup7FullPopupOutsideMirrorSymbolFailure() {
        SFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureStat eventStat =
            SFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureStat::OFF;
        if (mPuFGroup7OutsideMirrorSymbolFailureStatus == PuFGroup7OutsideMirrorSymbolFailureStatus::FAILURE &&
            mIsIgnOn == true) {
            eventStat = SFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureStat::ON;
        }
        setSFCADAS_Driving_NewEventGroup7FullPopupOutsideMirrorSymbolFailureStat(eventStat);
    }

    // 5.3.9 7 그룹 풀 팝업 – 고속도로 자율주행 고장
    void updateEventGroup7FullPopupHdpFailure() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            if ((mRCU_HDPFaultStatus == RCU_HDPFaultStatus::MAIN_CONTROLLER_FAULT_DETECTED ||
                 mRCU_HDPFaultStatus == RCU_HDPFaultStatus::SUB_CONTROLLER_FAULT_DETECTED ||
                 mRCU_HDPFaultStatus == RCU_HDPFaultStatus::MAIN_SUB_CONTROLLER_FAULT_DETECTED) ||
                (mESC_HDPFaultStatus == ESC_HDPFaultStatus::MAIN_CONTROLLER_FAULT_DETECTED ||
                 mESC_HDPFaultStatus == ESC_HDPFaultStatus::SUB_CONTROLLER_FAULT_DETECTED ||
                 mESC_HDPFaultStatus == ESC_HDPFaultStatus::MAIN_SUB_CONTROLLER_FAULT_DETECTED)) {
                eventID = "E52077";
            } else {
                if (mPuFGroup7HdpFailureStatus == PuFGroup7HdpFailureStatus::FAILURE ||
                    (mPuFGroup7HdpFailureStatus == PuFGroup7HdpFailureStatus::UNHANDLED_TIMEOUT &&
                     mConfigAdasHDP == ConfigAdasHDP::ON)) {
                    eventID = "E52077";
                } else if (mPuFGroup7HdpFailureStatus == PuFGroup7HdpFailureStatus::CAMERA_BLOCKAGE) {
                    eventID = "E52085";
                } else if (mPuFGroup7HdpFailureStatus == PuFGroup7HdpFailureStatus::RADAR_BLOCKAGE) {
                    eventID = "E52086";
                } else if (mPuFGroup7HdpFailureStatus == PuFGroup7HdpFailureStatus::LIDAR_BLOCKAGE) {
                    eventID = "E52087";
                } else {
                    // no operation
                }
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.Group7FullPopupHdpFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureStat(SFCADAS_Driving_NewEventGroup7FullPopupHdpFailureStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureID(eventID);
            setSFCADAS_Driving_NewEventGroup7FullPopupHdpFailureStat(SFCADAS_Driving_NewEventGroup7FullPopupHdpFailureStat::ON);
        }
    }

    // 5.3.10 7그룹 풀 팝업 – 운전자 보조 시스템 고장 1
    void updateEventGroup7FullPopup1() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT &&
                 (mConfigAdasLKA == ConfigAdasLKA::ON || mConfigAdasFCA == ConfigAdasFCA::ON)) ||
                (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::FAILURE)) {
                eventID = "E52088";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::CAMERA_BLOCKAGE) {
                eventID = "E52089";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::RADAR_BLOCKAGE) {
                eventID = "E52090";
            } else if (mPuFGroup7DriverAssistFailure1Status == PuFGroup7DriverAssistFailure1Status::CAMERA_RADAR_BLOCKAGE) {
                eventID = "E52091";
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

    // 5.3.11 7그룹 풀 팝업 – 운전자 보조 시스템 고장 2
    void updateEventGroup7FullPopup2() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7DriverAssistFailure2Status == PuFGroup7DriverAssistFailure2Status::UNHANDLED_TIMEOUT &&
                 mConfigNewBCW == ConfigNewBCW::ON) ||
                (mPuFGroup7DriverAssistFailure2Status == PuFGroup7DriverAssistFailure2Status::FAILURE)) {
                eventID = "E52092";
            } else if (mPuFGroup7DriverAssistFailure2Status == PuFGroup7DriverAssistFailure2Status::RADAR_BLOCKAGE) {
                eventID = "E52093";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.DriverAssistFailure2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventDriverAssistFailure2Stat(SFCADAS_Driving_NewEventDriverAssistFailure2Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventDriverAssistFailure2ID(eventID);
            setSFCADAS_Driving_NewEventDriverAssistFailure2Stat(SFCADAS_Driving_NewEventDriverAssistFailure2Stat::ON);
        }
    }

    // 5.3.14 HDP 동작 시 뷰 전환 불가 팝업
    void updateEventHDPOperProhibit() {
        if (mIsIgnOn == true) {
            if (mInter_AdasViewFixStatus == SFCADAS_Driving_NewInter_AdasViewFixStatus::FIX &&
                (mHmiMenuSwitchStatus == HmiMenuSwitchStatus::SHORT_PRESSED ||
                 mHmiMenuSwitchStatus == HmiMenuSwitchStatus::LONG_PRESSED)) {
                setSFCADAS_Driving_NewEventHDPOperProhibitStat(SFCADAS_Driving_NewEventHDPOperProhibitStat::ON);
            } else {
                setSFCADAS_Driving_NewEventHDPOperProhibitStat(SFCADAS_Driving_NewEventHDPOperProhibitStat::OFF);
            }
        } else {
            setSFCADAS_Driving_NewEventHDPOperProhibitStat(SFCADAS_Driving_NewEventHDPOperProhibitStat::OFF);
        }
    }

    // 5.3.2 2그룹 미니 팝업 1-1
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
                case PuMGroup2AdasWarning1_1Status::SCC_ML_DISTANCE_LV1:
                    eventID = "E52631";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_ML_DISTANCE_LV2:
                    eventID = "E52632";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_ML_DISTANCE_LV3:
                    eventID = "E52633";
                    break;
                case PuMGroup2AdasWarning1_1Status::SCC_ML_DISTANCE_LV4:
                    eventID = "E52634";
                    break;
                case PuMGroup2AdasWarning1_1Status::NSCC_WARNING:
                    eventID = "E52509";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_COLLISION:
                    eventID = "E52510";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_CONDITION_NOT_MET:
                    eventID = "E52511";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_DRIVING_LANE_ANALYZING:
                    eventID = "E52512";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_CURVED_ROAD:
                    eventID = "E52513";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_NARROW_LANE:
                    eventID = "E52514";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_NOT_OPERATIONAL_SECTION:
                    eventID = "E52515";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_HAZARD_LIGHT:
                    eventID = "E52516";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_LOW_SPEED:
                    eventID = "E52517";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_HANDS_OFF:
                    eventID = "E52518";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_UNAVAILABLE_LANE:
                    eventID = "E52519";
                    break;
                case PuMGroup2AdasWarning1_1Status::LCA_STEERING_CONTROL_DETECTED:
                    eventID = "E52520";
                    break;
                default:
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

    // 5.3.3 2그룹 미니 팝업 1-2
    void updateEventGroup2MiniPopup1_2() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            switch (mPuMGroup2AdasWarning1_2Status) {
                case PuMGroup2AdasWarning1_2Status::STOP_THE_ACCELERATOR_PEDAL_INPUT:
                    eventID = "E52521";
                    break;
                case PuMGroup2AdasWarning1_2Status::STOP_THE_BRAKE_PEDAL_INPUT:
                    eventID = "E52522";
                    break;
                case PuMGroup2AdasWarning1_2Status::INABILITY_TO_SET_ADAS_FUNCTION:
                    eventID = "E52523";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_SHORTKEY_INPUT:
                    eventID = "E52524";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_UNABLE_ACC_OVERRIDE:
                    eventID = "E52593";
                    break;
                case PuMGroup2AdasWarning1_2Status::TURN_OFF_HAZARD_LIGHTS_AND_TURN_SIGNAL:
                    eventID = "E52594";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_KEEP_REACHABLE_RANGE:
                    eventID = "E52610";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_THE_SET_SPEED_CHANGED:
                    eventID = "E52525";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_AUTO_ACTIVATE_DEFOG:
                    eventID = "E52526";
                    break;
                case PuMGroup2AdasWarning1_2Status::SET_THE_WIPER_LIGHT_AUTO:
                    eventID = "E52545";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_MERGING_LANES:
                    eventID = "E52595";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_CONSTRUCTION:
                    eventID = "E52596";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_SENSOR_RANGE:
                    eventID = "E52597";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_PLANNED_EVENT:
                    eventID = "E52598";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_IC_JC_AHEAD:
                    eventID = "E52611";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_TOLLGATE_AHEAD:
                    eventID = "E52612";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_RC_AREA_HEAD:
                    eventID = "E52613";
                    break;
                case PuMGroup2AdasWarning1_2Status::EMERGENCY_CORRIDOR:
                    eventID = "E52614";
                    break;
                case PuMGroup2AdasWarning1_2Status::LOW_SPEED_TO_TRAFFIC_FLOW:
                    eventID = "E52615";
                    break;
                case PuMGroup2AdasWarning1_2Status::SPEED_DOWN_FOR_NIGHT:
                    eventID = "E52616";
                    break;
                case PuMGroup2AdasWarning1_2Status::LIMIT_SPEED_FOR_MERGING_LANE:
                    eventID = "E52617";
                    break;
                case PuMGroup2AdasWarning1_2Status::LIMIT_SPEED_FOR_ROAD_CONSTRUCTION:
                    eventID = "E52618";
                    break;
                case PuMGroup2AdasWarning1_2Status::LIMIT_SPEED_FOR_SAFETY:
                    eventID = "E52619";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_HDP_LONGKEY_INPUT:
                    eventID = "E52527";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_TUNNEL_AHEAD:
                    eventID = "E52635";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_CHECKPOINTS_AHEAD:
                    eventID = "E52636";
                    break;
                case PuMGroup2AdasWarning1_2Status::UNAVAILABLE_DURING_SERVER_CHECK:
                    eventID = "E52637";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_NAVIGATION_UPDATE:
                    eventID = "E52638";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_UNSUPPORTED_LANE_AHEAD:
                    eventID = "E52639";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_HDP_CAN_BE_ACTIVATED:
                    eventID = "E52528";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_VEHICLE_RESTART:
                    eventID = "E52529";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_ACTIVE_CONNETED_SERVICE:
                    eventID = "E52530";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_SOFTWARE_UPDATE:
                    eventID = "E52531";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_IN_SUPPORTED_ROAD_TYPE:
                    eventID = "E52532";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_NOT_IN_MAIN_LANES:
                    eventID = "E52533";
                    break;
                case PuMGroup2AdasWarning1_2Status::UNAVAILABLE_WITH_ACTV_WARNING:
                    eventID = "E52534";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_SUPPORTED_ON_IC_JC:
                    eventID = "E52535";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_IN_SUPPORTED_LANE_TYPE:
                    eventID = "E52536";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_SUPPORTED_COUNTRY:
                    eventID = "E52599";
                    break;
                case PuMGroup2AdasWarning1_2Status::SENSORS_IN_SELF_CHECK:
                    eventID = "E52537";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_GEAR_D:
                    eventID = "E52538";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_OFF_TO_AUTO_STOP:
                    eventID = "E52539";
                    break;
                case PuMGroup2AdasWarning1_2Status::INCREASE_THE_DISTANCE:
                    eventID = "E52540";
                    break;
                case PuMGroup2AdasWarning1_2Status::DECREASE_VEHICLE_SPEED:
                    eventID = "E52541";
                    break;
                case PuMGroup2AdasWarning1_2Status::CENTER_THE_VEHICLE_IN_LANE:
                    eventID = "E52542";
                    break;
                case PuMGroup2AdasWarning1_2Status::RSPA_IN_OPERATION:
                    eventID = "E52543";
                    break;
                case PuMGroup2AdasWarning1_2Status::INDUCE_ESC_ON:
                    eventID = "E52544";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_UNFOLD_SIDE_MIRRORS:
                    eventID = "E52622";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_TO_CHANGE_LANE_AT_NIGHT:
                    eventID = "E52623";
                    break;
                case PuMGroup2AdasWarning1_2Status::TYPE_B1_INFORM:
                    eventID = "E52624";
                    break;
                case PuMGroup2AdasWarning1_2Status::TYPE_B2_INFORM:
                    eventID = "E52625";
                    break;
                case PuMGroup2AdasWarning1_2Status::AGGRESSIVE_DRIVING_DETECTED:
                    eventID = "E52546";
                    break;
                case PuMGroup2AdasWarning1_2Status::SENSOR_AUTO_CALIBRATION:
                    eventID = "E52590";
                    break;
                case PuMGroup2AdasWarning1_2Status::MONITORING_HDP_STABILITY:
                    eventID = "E52591";
                    break;
                case PuMGroup2AdasWarning1_2Status::INFORM_DEACTIVATION_IN_OVERRIDE:
                    eventID = "E52547";
                    break;
                case PuMGroup2AdasWarning1_2Status::IMPACT_DETECTED:
                    eventID = "E52548";
                    break;
                case PuMGroup2AdasWarning1_2Status::ACCEL_INPUT_WITH_HANDS_OFF:
                    eventID = "E52549";
                    break;
                case PuMGroup2AdasWarning1_2Status::GEAR_SHIFTER_INPUT_DETECTED:
                    eventID = "E52550";
                    break;
                case PuMGroup2AdasWarning1_2Status::BRAKE_INPUT_WITH_HANDS_OFF:
                    eventID = "E52551";
                    break;
                case PuMGroup2AdasWarning1_2Status::VEHICLE_START_INPUT_DETECTED:
                    eventID = "E52552";
                    break;
                case PuMGroup2AdasWarning1_2Status::STATIONARY_VEHICLE_STOP:
                    eventID = "E52553";
                    break;
                case PuMGroup2AdasWarning1_2Status::UNSUPPORTED_TUNNEL_AHEAD:
                    eventID = "E52640";
                    break;
                case PuMGroup2AdasWarning1_2Status::TRAFFIC_CONGESTION_CLEARED:
                    eventID = "E52600";
                    break;
                case PuMGroup2AdasWarning1_2Status::IC_JC_AHEAD:
                    eventID = "E52554";
                    break;
                case PuMGroup2AdasWarning1_2Status::SUPPORTED_LANE_TYPE_ENDING:
                    eventID = "E52555";
                    break;
                case PuMGroup2AdasWarning1_2Status::SUPPORTED_ROAD_TYPE_ENDING:
                    eventID = "E52556";
                    break;
                case PuMGroup2AdasWarning1_2Status::DETECTION_OF_LANE_DEPARTURE:
                    eventID = "E52557";
                    break;
                case PuMGroup2AdasWarning1_2Status::MAXIMUM_SPEED_EXCEEDED:
                    eventID = "E52558";
                    break;
                case PuMGroup2AdasWarning1_2Status::ABNORMAL_OP_DETECTED:
                    eventID = "E52559";
                    break;
                case PuMGroup2AdasWarning1_2Status::WIPER_LIGHT_INPUT_DETECTED:
                    eventID = "E52560";
                    break;
                case PuMGroup2AdasWarning1_2Status::DEFOGGING_OFF_DETECTED:
                    eventID = "E52561";
                    break;
                case PuMGroup2AdasWarning1_2Status::HAZARD_OR_TURN_SIGNAL_ON_DETECTED:
                    eventID = "E52601";
                    break;
                case PuMGroup2AdasWarning1_2Status::EVASIVE_STEERING_ACTIVE:
                    eventID = "E52602";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_DETECTED_IN_PATH_VEHICLE_AT_NIGHT:
                    eventID = "E52626";
                    break;
                case PuMGroup2AdasWarning1_2Status::HDP_FAILURE:
                    eventID = "E52562";
                    break;
                case PuMGroup2AdasWarning1_2Status::SAFETY_FUNCTION_ACTIVATED:
                    eventID = "E52563";
                    break;
                case PuMGroup2AdasWarning1_2Status::CAMERA_OBSCURED:
                    eventID = "E52564";
                    break;
                case PuMGroup2AdasWarning1_2Status::RADAR_BLOCKED:
                    eventID = "E52565";
                    break;
                case PuMGroup2AdasWarning1_2Status::LIDAR_BLOCKED:
                    eventID = "E52566";
                    break;
                case PuMGroup2AdasWarning1_2Status::AIRBAG_WARNING_ON:
                    eventID = "E52567";
                    break;
                case PuMGroup2AdasWarning1_2Status::TRAILER_DETECTED:
                    eventID = "E52592";
                    break;
                case PuMGroup2AdasWarning1_2Status::HIGH_OUTSIDE_TEMPERATURE:
                    eventID = "E52603";
                    break;
                case PuMGroup2AdasWarning1_2Status::LOW_OUTSIDE_TEMPERATURE:
                    eventID = "E52604";
                    break;
                case PuMGroup2AdasWarning1_2Status::UNAVAILABLE_RC_AREA:
                    eventID = "E52627";
                    break;
                case PuMGroup2AdasWarning1_2Status::TOLLGATE_NOT_SUPPORTED:
                    eventID = "E52568";
                    break;
                case PuMGroup2AdasWarning1_2Status::DRIVER_NOT_IN_POSITION:
                    eventID = "E52569";
                    break;
                case PuMGroup2AdasWarning1_2Status::ONCOMING_VEHICLE_DETECTED:
                    eventID = "E52570";
                    break;
                case PuMGroup2AdasWarning1_2Status::EMERGENCY_VEHICLE_DETECTED:
                    eventID = "E52571";
                    break;
                case PuMGroup2AdasWarning1_2Status::NON_VEHICLE_DETECTED:
                    eventID = "E52572";
                    break;
                case PuMGroup2AdasWarning1_2Status::RAIN_OR_SNOW_DETECTED:
                    eventID = "E52573";
                    break;
                case PuMGroup2AdasWarning1_2Status::SLIPPERY_ROAD:
                    eventID = "E52574";
                    break;
                case PuMGroup2AdasWarning1_2Status::ROAD_CONSTRUCTION_DETECTED:
                    eventID = "E52605";
                    break;
                case PuMGroup2AdasWarning1_2Status::PEDESTRIAN_DETECTED:
                    eventID = "E52606";
                    break;
                case PuMGroup2AdasWarning1_2Status::UNAVAILABLE_SEAT_CHANGE_DETECTED:
                    eventID = "E52607";
                    break;
                case PuMGroup2AdasWarning1_2Status::FOLDED_SIDE_MIRRORS:
                    eventID = "E52628";
                    break;
                case PuMGroup2AdasWarning1_2Status::ROAD_INFORMATION_IS_INVALID:
                    eventID = "E52575";
                    break;
                case PuMGroup2AdasWarning1_2Status::LANE_NOT_DETECTED:
                    eventID = "E52576";
                    break;
                case PuMGroup2AdasWarning1_2Status::DRIVER_NOT_DETECTED:
                    eventID = "E52577";
                    break;
                case PuMGroup2AdasWarning1_2Status::PAY_ATTENTION_TO_THE_DRIVING:
                    eventID = "E52578";
                    break;
                case PuMGroup2AdasWarning1_2Status::NEED_FOR_IN_PATH_VEHICLE_WITH_LOW_LIGHT:
                    eventID = "E52629";
                    break;
                case PuMGroup2AdasWarning1_2Status::TYPE_D1_INFORM:
                    eventID = "E52630";
                    break;
                case PuMGroup2AdasWarning1_2Status::CHECKPOINT_NOT_SUPPORTED:
                    eventID = "E52641";
                    break;
                case PuMGroup2AdasWarning1_2Status::NOT_IN_SUPPORTED_TUNNEL:
                    eventID = "E52642";
                    break;
                case PuMGroup2AdasWarning1_2Status::LOW_FUEL:
                    eventID = "E52579";
                    break;
                case PuMGroup2AdasWarning1_2Status::LOW_TIRE_PRESSURE:
                    eventID = "E52580";
                    break;
                case PuMGroup2AdasWarning1_2Status::DOOR_OPEN:
                    eventID = "E52581";
                    break;
                case PuMGroup2AdasWarning1_2Status::TRUNK_OPEN:
                    eventID = "E52582";
                    break;
                case PuMGroup2AdasWarning1_2Status::HOOD_OPEN:
                    eventID = "E52583";
                    break;
                case PuMGroup2AdasWarning1_2Status::FASTEN_SEAT_BELT:
                    eventID = "E52584";
                    break;
                case PuMGroup2AdasWarning1_2Status::PARKING_BRAKE_ACTIVATED:
                    eventID = "E52585";
                    break;
                case PuMGroup2AdasWarning1_2Status::LOW_BATTERY_EV:
                    eventID = "E52586";
                    break;
                case PuMGroup2AdasWarning1_2Status::POSTPONE_HDP_OFF_DURING_EM:
                    eventID = "E52608";
                    break;
                case PuMGroup2AdasWarning1_2Status::LIFTGATE_OPEN:
                    eventID = "E52609";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_2Status.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_2StatusStat(
                SFCADAS_Driving_NewEventPuMGroup2AdasWarning1_2StatusStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_2StatusID(eventID);
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_2StatusStat(
                SFCADAS_Driving_NewEventPuMGroup2AdasWarning1_2StatusStat::ON);
        }
    }

    // 5.3.3 2그룹 미니 팝업 1-3
    void updateEventGroup2MiniPopup1_3() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true) {
            switch (mPuMGroup2AdasWarning1_3Status) {
                case PuMGroup2AdasWarning1_3Status::STEERING_CONTROL_DETECTED:
                    eventID = "E52644";
                    break;
                case PuMGroup2AdasWarning1_3Status::ACCELERATOR_PEDAL_CONTROL_DETECTED:
                    eventID = "E52645";
                    break;
                case PuMGroup2AdasWarning1_3Status::CONDITION_NOT_MET:
                    eventID = "E52646";
                    break;
                case PuMGroup2AdasWarning1_3Status::MAXIMUM_SPEED_EXCEEDED:
                    eventID = "E52647";
                    break;
                case PuMGroup2AdasWarning1_3Status::MAXIMUM_SET_SPEED_EXCEEDED:
                    eventID = "E52648";
                    break;
                case PuMGroup2AdasWarning1_3Status::IC_JC_AHEAD:
                    eventID = "E52649";
                    break;
                case PuMGroup2AdasWarning1_3Status::TOLLGATE_AHEAD:
                    eventID = "E52650";
                    break;
                case PuMGroup2AdasWarning1_3Status::NOT_SUPPORTED_AREA_AHEAD:
                    eventID = "E52651";
                    break;
                case PuMGroup2AdasWarning1_3Status::NOT_SUPPORTED_ROAD_AHEAD:
                    eventID = "E52652";
                    break;
                case PuMGroup2AdasWarning1_3Status::CURVED_ROAD:
                    eventID = "E52653";
                    break;
                case PuMGroup2AdasWarning1_3Status::PEDESTRIAN_OR_CYCLIST_DETECTED:
                    eventID = "E52654";
                    break;
                case PuMGroup2AdasWarning1_3Status::LANE_DEPARTURE_DETECTED:
                    eventID = "E52655";
                    break;
                case PuMGroup2AdasWarning1_3Status::DRIVER_NOT_DETECTED:
                    eventID = "E52656";
                    break;
                case PuMGroup2AdasWarning1_3Status::DEACTIVATION_LANE_FOLLOWING_ASSIST:
                    eventID = "E52657";
                    break;
                case PuMGroup2AdasWarning1_3Status::DEACTIVATION_HIGHWAY_DRIVING_ASSIST:
                    eventID = "E52658";
                    break;
                case PuMGroup2AdasWarning1_3Status::SUGGEST_LANE_CHANGE_TO_LEFT_SIDE:
                    eventID = "E52659";
                    break;
                case PuMGroup2AdasWarning1_3Status::SUGGEST_LANE_CHANGE_TO_RIGHT_SIDE:
                    eventID = "E52660";
                    break;
                case PuMGroup2AdasWarning1_3Status::TUNNEL_AHEAD:
                    eventID = "E52661";
                    break;
                case PuMGroup2AdasWarning1_3Status::UNSUPPORTED_LANE_AHEAD:
                    eventID = "E52662";
                    break;
                case PuMGroup2AdasWarning1_3Status::UNSUPPORTED_WEATHER_CONDITIONS:
                    eventID = "E52663";
                    break;
                case PuMGroup2AdasWarning1_3Status::ROAD_CONSTRUCTION_AHEAD:
                    eventID = "E52664";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_Driving_New.Event.PuMGroup2AdasWarning1_3Status.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_3StatusStat(
                SFCADAS_Driving_NewEventPuMGroup2AdasWarning1_3StatusStat::OFF);
            flush();
        }
        if (eventID.empty() == false) {
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_3StatusID(eventID);
            setSFCADAS_Driving_NewEventPuMGroup2AdasWarning1_3StatusStat(
                SFCADAS_Driving_NewEventPuMGroup2AdasWarning1_3StatusStat::ON);
        }
    }

    // 6. 사운드 로직
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
                // default
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

    // 6.1 경고음 1-3
    void updateSoundAdasWarning1_3Status() {
        std::string soundID;
        SFCADAS_Driving_NewSoundAdasWarning1_3Type type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::NONE;
        HUInt64 count = 0;
        if (mIsIgnOn == true) {
            if (mSoundAdasWarning1_3Status == SoundAdasWarning1_3Status::SPECIAL_SOUND_1_AUTO) {
                if (mAdasOverSpeedWarnOnOffStatus == AdasOverSpeedWarningOnOffStatus::NON_OPTION) {
                    if (mIslaOption2Status == IslaOption2Status::INVALID) {
                        soundID = "SND_ADAS_MSLA_Auto_Ancient";
                        type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::INFINITE;
                    } else if (mIslaOption2Status != IslaOption2Status::INVALID) {
                        soundID = "SND_ADAS_MSLA_Auto_Old";
                        type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::INFINITE;
                    } else {
                        // default
                    }
                } else if (mAdasOverSpeedWarnOnOffStatus != AdasOverSpeedWarningOnOffStatus::NON_OPTION) {
                    soundID = "SND_ADAS_MSLA_Auto";
                    type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::INFINITE;
                } else {
                    // default
                }
            } else if (mSoundAdasWarning1_3Status == SoundAdasWarning1_3Status::ALARM_SOUND_1_MEDIUM) {
                soundID = "SND_ADAS_PopUpInform1_Fixed";
                type = SFCADAS_Driving_NewSoundAdasWarning1_3Type::REPEAT_COUNT;
                count = 1;
            } else {
                // default
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

    // 6.1 경고음 1-4
    void updateSoundAdasWarning1_4Status() {
        SFCADAS_Driving_NewSoundAdasWarning1_4Stat stat = SFCADAS_Driving_NewSoundAdasWarning1_4Stat::OFF;
        if (mIsIgnOn == true && mSoundAdasWarning1_4Status == SoundAdasWarning1_4Status::ALARM_SOUND_2_MEDIUM) {
            stat = SFCADAS_Driving_NewSoundAdasWarning1_4Stat::ON;
        }
        setSFCADAS_Driving_NewSoundAdasWarning1_4Stat(stat);
    }

    // 6.1 경고음 1-5
    void updateSoundAdasWarning1_5Status() {
        SFCADAS_Driving_NewSoundAdasWarning1_5Stat stat = SFCADAS_Driving_NewSoundAdasWarning1_5Stat::OFF;
        if (mIsIgnOn == true && mSoundAdasWarning1_5Status == SoundAdasWarning1_5Status::SPECIAL_SOUND_4_AUTO) {
            stat = SFCADAS_Driving_NewSoundAdasWarning1_5Stat::ON;
        }
        setSFCADAS_Driving_NewSoundAdasWarning1_5Stat(stat);
    }

    // 6.1 경고음 2-1
    void updateSoundAdasWarning2_1Status() {
        SFCADAS_Driving_NewSoundAdasWarning2_1Stat stat = SFCADAS_Driving_NewSoundAdasWarning2_1Stat::OFF;
        if (mSoundAdasWarning2_1Status == SoundAdasWarning2_1Status::SPECIAL_SOUND_5_AUTO ||
            mSoundAdasWarning2_1Status == SoundAdasWarning2_1Status::SPECIAL_SOUND_5_AUTO_SEW) {
            stat = SFCADAS_Driving_NewSoundAdasWarning2_1Stat::ON;
        }
        setSFCADAS_Driving_NewSoundAdasWarning2_1Stat(stat);
    }

    void updateConstantMainBrakeLight() {
        SFCADAS_Driving_NewConstantMainBrakeLightActiveStat stat = SFCADAS_Driving_NewConstantMainBrakeLightActiveStat::OFF;

        if (mIsIgnOn == true && (mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::STEADY_ON ||
                                 mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::BLINK_ON)) {
            stat = SFCADAS_Driving_NewConstantMainBrakeLightActiveStat::ON;
        }
        setSFCADAS_Driving_NewConstantMainBrakeLightActiveStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    enum class Private_HDPRedundancyStatus_1 : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    enum class Private_HDPRedundancyStatus_2 : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    using Private_ISLAAddtnlSign = SFCADAS_Driving_NewConstantISLAAddtnlSignStat;
    using ConfigAdasFCA = ArgumentsConfigFca1Changed::Inter_ConfigAdasFCA;
    using ConfigAdasFCA2 = ArgumentsConfigFca2Changed::Inter_ConfigAdasFCA2;
    using ConfigAdasLKA = ArgumentsConfigLkaChanged::Inter_ConfigAdasLKA;
    using ConfigAdasDAW = ArgumentsConfigDawChanged::Inter_ConfigAdasDAW;
    using ConfigILCU = ArgumentsConfigIlcuIfsChanged::Inter_ConfigILCU;
    using ConfigIFS = ArgumentsConfigIlcuIfsChanged::Inter_ConfigIFS;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigAdasISLA = ArgumentsConfigIslaChanged::Inter_ConfigAdasISLA;
    using ConfigNewBCW = ArgumentsConfigBcwChanged::Inter_ConfigNewBCW;
    using ConfigNewHBA = ArgumentsConfigHbaChanged::Inter_ConfigNewHBA;
    using ConfigAdasSCC = ArgumentsConfigSccChanged::Inter_ConfigAdasSCC;
    using ConfigAdasLFA = ArgumentsConfigLfaChanged::Inter_ConfigAdasLFA;
    using ConfigAdasHDA = ArgumentsConfigHdaChanged::Inter_ConfigAdasHDA;
    using ConfigAdasHDA2 = ArgumentsConfigHda2Changed::Inter_ConfigAdasHDA2;
    using ConfigAdasHDA3 = ArgumentsConfigHda3Changed::Inter_ConfigAdasHDA3;
    using ConfigAdasHDP = ArgumentsConfigHdpChanged::Inter_ConfigAdasHDP;
    using ConfigAdasEmergencyStop = ArgumentsConfigEmergencyStopChanged::Inter_ConfigAdasEmergencyStop;
    using ConfigHDPName = ArgumentsConfigHDPNameChanged::Inter_ConfigHDPName;

    using DISAddtnlSign = ArgumentsInter_DISAddtnlSign::Inter_DISAddtnlSign;
    using ForwardSafetySymbolStatus = ArgumentsForwardSafetySymbolStatusChanged::Input_ForwardSafetySymbolStatus;
    using EmergencySteeringSymbolStatus = ArgumentsEmergencySteeringSymbolStatusChanged::Input_EmergencySteeringSymbolStatus;
    using LaneSafetySymbolStatus = ArgumentsLaneSafetySymbolStatusChanged::Input_LaneSafetySymbolStatus;
    using HbaSymbolStatus = ArgumentsHbaSymbolStatusChanged::Input_HbaSymbolStatus;
    using SmvFrontObjectStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvFrontObjectStatus;
    using SmvNsccSymbol2Status = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvNsccSymbol2Status;
    using SmvVehicleDistanceLevelValue = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvVehicleDistanceLevelValue;
    using SmvVehicleDistanceLevelStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvVehicleDistanceLevelStatus;
    using SmvHostVehicleStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvHostVehicleStatus;
    using SmvSetSpeedStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvSetSpeedStatus;
    using SmvHdaSymbolStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvHdaSymbolStatus;
    using SmvIslaSetSpeedSymbolStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvIslaSetSpeedSymbolStatus;
    using SmvNsccSymbolStatus = ArgumentsSmvADASSummeryInfoStatusChanged::Input_SmvNsccSymbolStatus;
    using SmvLfaSymbolStatus = ArgumentsSmvLfaSymbolStatusChanged::Input_SmvLfaSymbolStatus;
    using SmvLcaLeftSymbolStatus = ArgumentsSmvLcaLeftSymbolStatusChanged::Input_SmvLcaLeftSymbolStatus;
    using SmvLcaRightSymbolStatus = ArgumentsSmvLcaRightSymbolStatusChanged::Input_SmvLcaRightSymbolStatus;
    using DriverHandsOnOffStatus = ArgumentsDriverHandsOnOffStatusStatusChanged::Input_MvDriverHandsOnOffStatus;
    using MvDrivingLaneCenterLineStatus = ArgumentsMvDrivingLaneCenterLineStatusChanged::Input_MvDrivingLaneCenterLineStatus;
    using MvLeftLineStatus = ArgumentsMvLeftLineStatusChanged::Input_MvLeftLineStatus;
    using MvRightLineStatus = ArgumentsMvRightLineStatusChanged::Input_MvRightLineStatus;
    using MvVehicleDistanceStatus = ArgumentsMvVehicleDistanceStatusChanged::Input_MvVehicleDistanceStatus;
    using MvCenterRoadSurfaceStatus = ArgumentsMvCenterRoadSurfaceStatusChanged::Input_MvCenterRoadSurfaceStatus;
    using MvLeftRoadSurfaceStatus = ArgumentsMvLeftRoadSurfaceStatusChanged::Input_MvLeftRoadSurfaceStatus;
    using MvRightRoadSurfaceStatus = ArgumentsMvRightRoadSurfaceStatusChanged::Input_MvRightRoadSurfaceStatus;
    using MvFrontObjectStatus = ArgumentsMvFrontObjectStatusChanged::Input_MvFrontObjectStatus;
    using MvFrontLeftObjectStatus = ArgumentsMvFrontLeftObjectStatusChanged::Input_MvFrontLeftObjectStatus;
    using MvFrontRightObjectStatus = ArgumentsMvFrontRightObjectStatusChanged::Input_MvFrontRightObjectStatus;
    using MvLeftObjectStatus = ArgumentsMvLeftObjectStatusChanged::Input_MvLeftObjectStatus;
    using MvRightObjectStatus = ArgumentsMvRightObjectStatusChanged::Input_MvRightObjectStatus;
    using MvRearLeftRadarWave1Status = ArgumentsMvRearLeftRadarWaveStatusChanged::Input_MvRearLeftRadarWave1Status;
    using MvRearLeftRadarWave2Status = ArgumentsMvRearLeftRadarWaveStatusChanged::Input_MvRearLeftRadarWave2Status;
    using MvRearRightRadarWave1Status = ArgumentsMvRearRightRadarWaveStatusChanged::Input_MvRearRightRadarWave1Status;
    using MvRearRightRadarWave2Status = ArgumentsMvRearRightRadarWaveStatusChanged::Input_MvRearRightRadarWave2Status;
    using MvLeftLaneChangeDirectionStatus =
        ArgumentsMvLeftLaneChangeDirectionStatusChanged::Input_MvLeftLaneChangeDirectionStatus;
    using MvRightLaneChangeDirectionStatus =
        ArgumentsMvRightLaneChangeDirectionStatusChanged::Input_MvRightLaneChangeDirectionStatus;
    using IslaSpeedLimitTrafficSignStatus =
        ArgumentsIslaSpeedLimitTrafficSignStatusChanged::Input_IslaSpeedLimitTrafficSignStatus;
    using IslaAdditionalTrafficSignStatus =
        ArgumentsIslaAdditionalTrafficSignStatusChanged::Input_IslaAdditionalTrafficSignStatus;
    using IslaSupplementaryTrafficSignStatus =
        ArgumentsIslaSupplementaryTrafficSignStatusChanged::Input_IslaSupplementaryTrafficSignStatus;
    using PuFGroup1AdasWarning1_1Status = ArgumentsPuFGroup1AdasWarning1_1StatusChanged::Input_PuFGroup1AdasWarning1_1Status;
    using PuFGroup1AdasWarning1_2Status = ArgumentsPuFGroup1AdasWarning1_2StatusChanged::Input_PuFGroup1AdasWarning1_2Status;
    using PuFGroup1AdasWarning2_1Status = ArgumentsPuFGroup1AdasWarning2_1StatusChanged::Input_PuFGroup1AdasWarning2_1Status;
    using PuFGroup4AdasWarning1_1Status = ArgumentsPuFGroup4AdasWarning1_1StatusChanged::Input_PuFGroup4AdasWarning1_1Status;
    using PuFGroup7ForwardSideSafetyFailureStatus =
        ArgumentsPuFGroup7ForwardSideSafetyFailureStatusChanged::Input_PuFGroup7ForwardSideSafetyFailureStatus;
    using PuFGroup7ForwardSafetyFailureStatus =
        ArgumentsPuFGroup7ForwardSafetyFailureStatusChanged::Input_PuFGroup7ForwardSafetyFailureStatus;
    using PuFGroup7LaneSafetyFailureStatus =
        ArgumentsPuFGroup7LaneSafetyFailureStatusChanged::Input_PuFGroup7LaneSafetyFailureStatus;
    using PuFGroup7BlindSpotSafetyFailureStatus =
        ArgumentsPuFGroup7BlindSpotSafetyFailureStatusChanged::Input_PuFGroup7BlindSpotSafetyFailureStatus;
    using PuFGroup7OutsideMirrorSymbolFailureStatus =
        ArgumentsPuFGroup7OutsideMirrorSymbolFailureStatusChanged::Input_PuFGroup7OutsideMirrorSymbolFailureStatus;
    using PuFGroup7IslaFailureStatus = ArgumentsPuFGroup7IslaFailureStatusChanged::Input_PuFGroup7IslaFailureStatus;
    using PuFGroup7DawFailureStatus = ArgumentsPuFGroup7DawFailureStatusChanged::Input_PuFGroup7DawFailureStatus;
    using PuFGroup7HbaFailureStatus = ArgumentsPuFGroup7HbaFailureStatusChanged::Input_PuFGroup7HbaFailureStatus;
    using PuFGroup7SccFailureStatus = ArgumentsPuFGroup7SccFailureStatusChanged::Input_PuFGroup7SccFailureStatus;
    using PuFGroup7LcaFailureStatus = ArgumentsPuFGroup7LcaFailureStatusChanged::Input_PuFGroup7LcaFailureStatus;
    using PuMGroup2AdasWarning1_1Status = ArgumentsPuMGroup2AdasWarning1_1StatusChanged::Input_PuMGroup2AdasWarning1_1Status;
    using MvHostVehicle1Status = ArgumentsMvHostVehicleStatusChanged::Input_MvHostVehicle1Status;
    using MvHostVehicle2Status = ArgumentsMvHostVehicleStatusChanged::Input_MvHostVehicle2Status;
    using IlcuLhActWarnStatus = ArgumentsIlcuActWarnStatusChanged::Input_IlcuLhActWarnStatus;
    using IlcuRhActWarnStatus = ArgumentsIlcuActWarnStatusChanged::Input_IlcuRhActWarnStatus;
    using IfsIndiReqStatus = ArgumentsIfsIndiReqStatusChanged::Input_IfsIndiReqStatus;
    using MvFrontRadarWaveStatus = ArgumentsMvFrontRadarWaveStatusChanged::Input_MvFrontRadarWaveStatus;
    using MvFrontObstacleStatus = ArgumentsMvFrontObstacleStatusChanged::Input_MvFrontObstacleStatus;
    using PuFGroup7LfaFailureStatus = ArgumentsPuFGroup7LfaFailureStatusChanged::Input_PuFGroup7LfaFailureStatus;
    using PuFGroup7HdaFailureStatus = ArgumentsPuFGroup7HdaFailureStatusChanged::Input_PuFGroup7HdaFailureStatus;
    using PuFGroup7HdpFailureStatus = ArgumentsPuFGroup7HdpFailureStatusChanged::Input_PuFGroup7HdpFailureStatus;
    using IslaTrafficSignCountryInformationStatus =
        ArgumentsIslaTrafficSignCountryInformationStatusChanged::Input_IslaTrafficSignCountryInformationStatus;
    using PuFGroup7DriverAssistFailure1Status =
        ArgumentsPuFGroup7DriverAssistFailure1StatusChanged::Input_PuFGroup7DriverAssistFailure1Status;
    using PuFGroup7DriverAssistFailure2Status =
        ArgumentsPuFGroup7DriverAssistFailure2StatusChanged::Input_PuFGroup7DriverAssistFailure2Status;
    using SoundAdasWarning1_1Status = ArgumentsSoundAdasWarning1_1StatusChanged::Input_SoundAdasWarning1_1Status;
    using SoundAdasWarning1_2Status = ArgumentsSoundAdasWarning1_2StatusChanged::Input_SoundAdasWarning1_2Status;
    using SoundAdasWarning1_3Status = ArgumentsSoundAdasWarning1_3StatusChanged::Input_SoundAdasWarning1_3Status;
    using SoundAdasWarning1_4Status = ArgumentsSoundAdasWarning1_4StatusChanged::Input_SoundAdasWarning1_4Status;
    using SoundAdasWarning1_5Status = ArgumentsSoundAdasWarning1_5StatusChanged::Input_SoundAdasWarning1_5Status;
    using SoundAdasWarning2_1Status = ArgumentsSoundAdasWarning2_1StatusChanged::Input_SoundAdasWarning2_1Status;
    using DriveAttentionWarningSymbolStatus =
        ArgumentsDriveAttentionWarningSymbolStatusChanged::Input_DriveAttentionWarningSymbolStatus;
    using PuMGroup2AdasWarning1_2Status = ArgumentsPuMGroup2AdasWarning1_2StatusChanged::Input_PuMGroup2AdasWarning1_2Status;
    using BGHDPStatus = ArgumentsBgHDPStatusChanged::Input_BGHDPStatus;
    using HDP_RDDWarningState = ArgumentsHdpRDDWarningStateChanged::Input_HDP_RDDWarningState;
    using ESC_HDPEngagedStatus = ArgumentsEscHDPEngagedStatusChanged::Input_ESC_HDPEngagedStatus;
    using RCU_HDPEngagedStatus = ArgumentsRcuHDPEngagedStatusChanged::Input_RCU_HDPEngagedStatus;
    using ESC_HDPFaultStatus = ArgumentsHdpFaultStatusChanged::Input_ESC_HDPFaultStatus;
    using RCU_HDPFaultStatus = ArgumentsHdpFaultStatusChanged::Input_RCU_HDPFaultStatus;
    using InterTelltaleIccDrow = ArgumentsInterTelltaleIccDrowChanged::Inter_TelltaleIccDrow;
    using BrakeLampControlCommand_Status = ArgumentsBrakeLampControlCommand_StatusChanged::Input_BrakeLampControlCommand_Status;
    using PuFGroup7MRMFailureStatus = ArgumentsPuFGroup7MRMFailureStatusChanged::Input_PuFGroup7MRMFailureStatus;
    using MvCircleAroundHostVehicleStatus =
        ArgumentsMvCircleAroundHostVehicleStatusChanged::Input_MVCircleAroundHostVehicleStatus;

    using HmiMenuSwitchStatus = ArgumentsHmiMenuSwitchStatusValueChanged::Inter_HmiMenuSwitchStatus;

    using ExtTurnLeftLampStatus = ArgumentsExtLampInputValueChanged::Input_ExtTurnLeftLampStatus;
    using ExtTurnRightLampStatus = ArgumentsExtLampInputValueChanged::Input_ExtTurnRightLampStatus;
    using ExtLowBeamOnReqStatus = ArgumentsExtLampInputValueChanged::Input_ExtLowBeamOnReqStatus;
    using ExtHighBeamOnReqStatus = ArgumentsExtLampInputValueChanged::Input_ExtHighBeamOnReqStatus;
    using IslaWarningSoundSymboleStatus = ArgumentsIslaWarningSoundSymboleStatusChanged::Input_IslaWarningSoundSymbolStatus;
    using PuMGroup2AdasWarning1_3Status = ArgumentsPuMGroup2AdasWarning1_3StatusChanged::Input_PuMGroup2AdasWarning1_3Status;
    using PuFGroup7HandsFreeFailureStatus =
        ArgumentsPuFGroup7HandsFreeFailureStatusChanged::Input_PuFGroup7HandsFreeFailureStatus;
    using IslaOption2Status = ArgumentsSoundAdasWarning1_3StatusChanged::Input_IslaOption2Status;
    using AdasOverSpeedWarningOnOffStatus = ArgumentsSoundAdasWarning1_3StatusChanged::Input_AdasOverSpeedWarnOnOffStatus;

    using IslaNextSpeedLimitTrafficSignStatus =
        ArgumentsIslaNextSpeedLimitTrafficSignStatusChanged::Input_IslaNextSpeedLimitTrafficSignStatus;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using IslaSupplementaryTrafficSign2Status =
        ArgumentsIslaSupplementaryTrafficSign2StatusChanged::Input_IslaSupplementaryTrafficSign2Status;

    using MvLaneChangeRectangleStatus = ArgumentsMvLaneChangeRectangleStatusChanged::Input_MvLaneChangeRectangleStatus;
    using MvFrontObject01Status = ArgumentsMvFrontObject01Changed::Input_MvFrontObject01Status;
    using MvFrontObjectClassification01Status = ArgumentsMvFrontObject01Changed::Input_MvFrontObjectClassification01Status;

    using MvSurroundingObject01Status = ArgumentsMvSurroundingObject01to05StatusChanged::Input_MvSurroundingObject01Status;
    using MvSurroundingObject02Status = ArgumentsMvSurroundingObject01to05StatusChanged::Input_MvSurroundingObject02Status;
    using MvSurroundingObject03Status = ArgumentsMvSurroundingObject01to05StatusChanged::Input_MvSurroundingObject03Status;
    using MvSurroundingObject04Status = ArgumentsMvSurroundingObject01to05StatusChanged::Input_MvSurroundingObject04Status;
    using MvSurroundingObject05Status = ArgumentsMvSurroundingObject01to05StatusChanged::Input_MvSurroundingObject05Status;
    using MvSurroundingObject06Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject06Status;
    using MvSurroundingObject07Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject07Status;
    using MvSurroundingObject08Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject08Status;
    using MvSurroundingObject09Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject09Status;
    using MvSurroundingObject10Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject10Status;
    using MvSurroundingObject11Status = ArgumentsMvSurroundingObject06to11StatusChanged::Input_MvSurroundingObject11Status;
    using MvSurroundingObject12Status = ArgumentsMvSurroundingObject12to14StatusChanged::Input_MvSurroundingObject12Status;
    using MvSurroundingObject13Status = ArgumentsMvSurroundingObject12to14StatusChanged::Input_MvSurroundingObject13Status;
    using MvSurroundingObject14Status = ArgumentsMvSurroundingObject12to14StatusChanged::Input_MvSurroundingObject14Status;
    using MvSurroundingObjectClassification01Status =
        ArgumentsMvSurroundingObjectClassification01to05StatusChanged::Input_MvSurroundingObjectClassification01Status;
    using MvSurroundingObjectClassification02Status =
        ArgumentsMvSurroundingObjectClassification01to05StatusChanged::Input_MvSurroundingObjectClassification02Status;
    using MvSurroundingObjectClassification03Status =
        ArgumentsMvSurroundingObjectClassification01to05StatusChanged::Input_MvSurroundingObjectClassification03Status;
    using MvSurroundingObjectClassification04Status =
        ArgumentsMvSurroundingObjectClassification01to05StatusChanged::Input_MvSurroundingObjectClassification04Status;
    using MvSurroundingObjectClassification05Status =
        ArgumentsMvSurroundingObjectClassification01to05StatusChanged::Input_MvSurroundingObjectClassification05Status;
    using MvSurroundingObjectClassification06Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification06Status;
    using MvSurroundingObjectClassification07Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification07Status;
    using MvSurroundingObjectClassification08Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification08Status;
    using MvSurroundingObjectClassification09Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification09Status;
    using MvSurroundingObjectClassification10Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification10Status;
    using MvSurroundingObjectClassification11Status =
        ArgumentsMvSurroundingObjectClassification06to11StatusChanged::Input_MvSurroundingObjectClassification11Status;
    using MvSurroundingObjectClassification12Status =
        ArgumentsMvSurroundingObjectClassification12to14StatusChanged::Input_MvSurroundingObjectClassification12Status;
    using MvSurroundingObjectClassification13Status =
        ArgumentsMvSurroundingObjectClassification12to14StatusChanged::Input_MvSurroundingObjectClassification13Status;
    using MvSurroundingObjectClassification14Status =
        ArgumentsMvSurroundingObjectClassification12to14StatusChanged::Input_MvSurroundingObjectClassification14Status;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigNewBCW mConfigNewBCW = ConfigNewBCW::OFF;
    ConfigILCU mConfigILCU = ConfigILCU::OFF;
    ConfigIFS mConfigIFS = ConfigIFS::OFF;
    ConfigNewHBA mConfigNewHBA = ConfigNewHBA::OFF;
    ConfigAdasDAW mConfigAdasDAW = ConfigAdasDAW::OFF;
    ConfigAdasFCA mConfigAdasFCA = ConfigAdasFCA::OFF;
    ConfigAdasFCA2 mConfigAdasFCA2 = ConfigAdasFCA2::OFF;
    ConfigAdasHDA mConfigAdasHDA = ConfigAdasHDA::OFF;
    ConfigAdasHDA2 mConfigAdasHDA2 = ConfigAdasHDA2::OFF;
    ConfigAdasHDA3 mConfigAdasHDA3 = ConfigAdasHDA3::OFF;
    ConfigAdasHDP mConfigAdasHDP = ConfigAdasHDP::OFF;
    ConfigAdasISLA mConfigAdasISLA = ConfigAdasISLA::OFF;
    ConfigAdasLFA mConfigAdasLFA = ConfigAdasLFA::OFF;
    ConfigAdasLKA mConfigAdasLKA = ConfigAdasLKA::OFF;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigAdasEmergencyStop mConfigAdasEmergencyStop = ConfigAdasEmergencyStop::OFF;
    ConfigHDPName mConfigHDPName = ConfigHDPName::TYPE_A;

    DISAddtnlSign mDISAddtnlSign = DISAddtnlSign::OFF;
    ForwardSafetySymbolStatus mForwardSafetySymbolStatus = ForwardSafetySymbolStatus::OFF;
    EmergencySteeringSymbolStatus mEmergencySteeringSymbolStatus = EmergencySteeringSymbolStatus::OFF;
    LaneSafetySymbolStatus mLaneSafetySymbolStatus = LaneSafetySymbolStatus::OFF;
    HbaSymbolStatus mHbaSymbolStatus = HbaSymbolStatus::OFF;
    SmvFrontObjectStatus mSmvFrontObjectStatus = SmvFrontObjectStatus::OFF;
    SmvNsccSymbol2Status mSmvNsccSymbol2Status = SmvNsccSymbol2Status::OFF;
    SmvVehicleDistanceLevelValue mSmvVehicleDistanceLevelValue = SmvVehicleDistanceLevelValue::OFF;
    SmvVehicleDistanceLevelStatus mSmvVehicleDistanceLevelStatus = SmvVehicleDistanceLevelStatus::OFF;
    SmvHostVehicleStatus mSmvHostVehicleStatus = SmvHostVehicleStatus::OFF;
    SmvSetSpeedStatus mSmvSetSpeedStatus = SmvSetSpeedStatus::OFF;
    SmvHdaSymbolStatus mSmvHdaSymbolStatus = SmvHdaSymbolStatus::OFF;
    SmvIslaSetSpeedSymbolStatus mSmvIslaSetSpeedSymbolStatus = SmvIslaSetSpeedSymbolStatus::OFF;
    SmvNsccSymbolStatus mSmvNsccSymbolStatus = SmvNsccSymbolStatus::OFF;
    SmvLfaSymbolStatus mSmvLfaSymbolStatus = SmvLfaSymbolStatus::OFF;
    SmvLcaLeftSymbolStatus mSmvLcaLeftSymbolStatus = SmvLcaLeftSymbolStatus::OFF;
    SmvLcaRightSymbolStatus mSmvLcaRightSymbolStatus = SmvLcaRightSymbolStatus::OFF;
    DriverHandsOnOffStatus mDriverHandsOnOffStatus = DriverHandsOnOffStatus::OFF;
    MvDrivingLaneCenterLineStatus mMvDrivingLaneCenterLineStatus = MvDrivingLaneCenterLineStatus::OFF;
    MvLeftLineStatus mMvLeftLineStatus = MvLeftLineStatus::OFF;
    MvRightLineStatus mMvRightLineStatus = MvRightLineStatus::OFF;
    MvVehicleDistanceStatus mMvVehicleDistanceStatus = MvVehicleDistanceStatus::OFF;
    MvCenterRoadSurfaceStatus mMvCenterRoadSurfaceStatus = MvCenterRoadSurfaceStatus::OFF;
    MvLeftRoadSurfaceStatus mMvLeftRoadSurfaceStatus = MvLeftRoadSurfaceStatus::OFF;
    MvRightRoadSurfaceStatus mMvRightRoadSurfaceStatus = MvRightRoadSurfaceStatus::OFF;
    MvFrontObjectStatus mMvFrontObjectStatus = MvFrontObjectStatus::OFF;
    MvFrontLeftObjectStatus mMvFrontLeftObjectStatus = MvFrontLeftObjectStatus::OFF;
    MvFrontRightObjectStatus mMvFrontRightObjectStatus = MvFrontRightObjectStatus::OFF;
    MvLeftObjectStatus mMvLeftObjectStatus = MvLeftObjectStatus::OFF;
    MvRightObjectStatus mMvRightObjectStatus = MvRightObjectStatus::OFF;
    MvRearLeftRadarWave1Status mMvRearLeftRadarWave1Status = MvRearLeftRadarWave1Status::OFF;
    MvRearLeftRadarWave2Status mMvRearLeftRadarWave2Status = MvRearLeftRadarWave2Status::OFF;
    MvRearRightRadarWave1Status mMvRearRightRadarWave1Status = MvRearRightRadarWave1Status::OFF;
    MvRearRightRadarWave2Status mMvRearRightRadarWave2Status = MvRearRightRadarWave2Status::OFF;
    MvLeftLaneChangeDirectionStatus mMvLeftLaneChangeDirectionStatus = MvLeftLaneChangeDirectionStatus::OFF;
    MvRightLaneChangeDirectionStatus mMvRightLaneChangeDirectionStatus = MvRightLaneChangeDirectionStatus::OFF;
    IslaSpeedLimitTrafficSignStatus mIslaSpeedLimitTrafficSignStatus = IslaSpeedLimitTrafficSignStatus::OFF;
    IslaAdditionalTrafficSignStatus mIslaAdditionalTrafficSignStatus = IslaAdditionalTrafficSignStatus::OFF;
    IslaSupplementaryTrafficSignStatus mIslaSupplementaryTrafficSignStatus = IslaSupplementaryTrafficSignStatus::OFF;
    PuFGroup1AdasWarning1_1Status mPuFGroup1AdasWarning1_1Status = PuFGroup1AdasWarning1_1Status::OFF;
    PuFGroup1AdasWarning1_2Status mPuFGroup1AdasWarning1_2Status = PuFGroup1AdasWarning1_2Status::OFF;
    PuFGroup1AdasWarning2_1Status mPuFGroup1AdasWarning2_1Status = PuFGroup1AdasWarning2_1Status::OFF;
    PuFGroup4AdasWarning1_1Status mPuFGroup4AdasWarning1_1Status = PuFGroup4AdasWarning1_1Status::OFF;
    PuFGroup7ForwardSideSafetyFailureStatus mPuFGroup7ForwardSideSafetyFailureStatus =
        PuFGroup7ForwardSideSafetyFailureStatus::OFF;
    PuFGroup7ForwardSafetyFailureStatus mPuFGroup7ForwardSafetyFailureStatus = PuFGroup7ForwardSafetyFailureStatus::OFF;
    PuFGroup7LaneSafetyFailureStatus mPuFGroup7LaneSafetyFailureStatus = PuFGroup7LaneSafetyFailureStatus::OFF;
    PuFGroup7BlindSpotSafetyFailureStatus mPuFGroup7BlindSpotSafetyFailureStatus = PuFGroup7BlindSpotSafetyFailureStatus::OFF;
    PuFGroup7OutsideMirrorSymbolFailureStatus mPuFGroup7OutsideMirrorSymbolFailureStatus =
        PuFGroup7OutsideMirrorSymbolFailureStatus::OFF;
    PuFGroup7IslaFailureStatus mPuFGroup7IslaFailureStatus = PuFGroup7IslaFailureStatus::OFF;
    PuFGroup7DawFailureStatus mPuFGroup7DawFailureStatus = PuFGroup7DawFailureStatus::OFF;
    PuFGroup7HbaFailureStatus mPuFGroup7HbaFailureStatus = PuFGroup7HbaFailureStatus::OFF;
    PuFGroup7SccFailureStatus mPuFGroup7SccFailureStatus = PuFGroup7SccFailureStatus::OFF;
    PuFGroup7LcaFailureStatus mPuFGroup7LcaFailureStatus = PuFGroup7LcaFailureStatus::OFF;
    PuMGroup2AdasWarning1_1Status mPuMGroup2AdasWarning1_1Status = PuMGroup2AdasWarning1_1Status::OFF;
    Private_ISLAAddtnlSign mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::NONE;
    MvHostVehicle1Status mMvHostVehicle1Status = MvHostVehicle1Status::OFF;
    MvHostVehicle2Status mMvHostVehicle2Status = MvHostVehicle2Status::OFF;
    IlcuLhActWarnStatus mIlcuLhActWarnStatus = IlcuLhActWarnStatus::OFF;
    IlcuRhActWarnStatus mIlcuRhActWarnStatus = IlcuRhActWarnStatus::OFF;
    IfsIndiReqStatus mIfsIndiReqStatus = IfsIndiReqStatus::OFF;
    MvFrontRadarWaveStatus mMvFrontRadarWaveStatus = MvFrontRadarWaveStatus::OFF;
    MvFrontObstacleStatus mMvFrontObstacleStatus = MvFrontObstacleStatus::OFF;
    PuFGroup7LfaFailureStatus mPuFGroup7LfaFailureStatus = PuFGroup7LfaFailureStatus::OFF;
    PuFGroup7HdaFailureStatus mPuFGroup7HdaFailureStatus = PuFGroup7HdaFailureStatus::OFF;
    PuFGroup7HdpFailureStatus mPuFGroup7HdpFailureStatus = PuFGroup7HdpFailureStatus::OFF;
    IslaTrafficSignCountryInformationStatus mIslaTrafficSignCountryInformationStatus =
        IslaTrafficSignCountryInformationStatus::INVALID;

    DriveAttentionWarningSymbolStatus mDriveAttentionWarningSymbolStatus = DriveAttentionWarningSymbolStatus::OFF;
    BGHDPStatus mBGHDPStatus = BGHDPStatus::OFF;
    PuMGroup2AdasWarning1_2Status mPuMGroup2AdasWarning1_2Status = PuMGroup2AdasWarning1_2Status::OFF;

    PuFGroup7DriverAssistFailure1Status mPuFGroup7DriverAssistFailure1Status = PuFGroup7DriverAssistFailure1Status::OFF;
    PuFGroup7DriverAssistFailure2Status mPuFGroup7DriverAssistFailure2Status = PuFGroup7DriverAssistFailure2Status::OFF;

    InterTelltaleIccDrow mInterTelltaleIccDrow = InterTelltaleIccDrow::OFF;

    SoundAdasWarning1_1Status mSoundAdasWarning1_1Status = SoundAdasWarning1_1Status::OFF;
    SoundAdasWarning1_2Status mSoundAdasWarning1_2Status = SoundAdasWarning1_2Status::OFF;
    SoundAdasWarning1_3Status mSoundAdasWarning1_3Status = SoundAdasWarning1_3Status::OFF;
    SoundAdasWarning1_4Status mSoundAdasWarning1_4Status = SoundAdasWarning1_4Status::OFF;
    SoundAdasWarning1_5Status mSoundAdasWarning1_5Status = SoundAdasWarning1_5Status::OFF;
    SoundAdasWarning2_1Status mSoundAdasWarning2_1Status = SoundAdasWarning2_1Status::OFF;

    HDP_RDDWarningState mHDP_RDDWarningState = HDP_RDDWarningState::INVALID;
    ESC_HDPEngagedStatus mESC_HDPEngagedStatus = ESC_HDPEngagedStatus::INVALID;
    ESC_HDPEngagedStatus mPrevESC_HDPEngagedStatus = ESC_HDPEngagedStatus::INVALID;
    RCU_HDPEngagedStatus mRCU_HDPEngagedStatus = RCU_HDPEngagedStatus::INVALID;
    RCU_HDPEngagedStatus mPrevRCU_HDPEngagedStatus = RCU_HDPEngagedStatus::INVALID;

    ESC_HDPFaultStatus mESC_HDPFaultStatus = ESC_HDPFaultStatus::INVALID;
    RCU_HDPFaultStatus mRCU_HDPFaultStatus = RCU_HDPFaultStatus::INVALID;

    Private_HDPRedundancyStatus_1 mPrivate_HDPRedundancyStatus_1 = Private_HDPRedundancyStatus_1::OFF;
    Private_HDPRedundancyStatus_2 mPrivate_HDPRedundancyStatus_2 = Private_HDPRedundancyStatus_2::OFF;

    BrakeLampControlCommand_Status mBrakeLampControlCommand_Status = BrakeLampControlCommand_Status::OFF;

    PuFGroup7MRMFailureStatus mPuFGroup7MRMFailureStatus = PuFGroup7MRMFailureStatus::OFF;
    MvCircleAroundHostVehicleStatus mMvCircleAroundHostVehicleStatus = MvCircleAroundHostVehicleStatus::OFF;

    ExtTurnLeftLampStatus mExtTurnLeftLampStatus = ExtTurnLeftLampStatus::OFF;
    ExtTurnRightLampStatus mExtTurnRightLampStatus = ExtTurnRightLampStatus::OFF;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    ExtHighBeamOnReqStatus mExtHighBeamOnReqStatus = ExtHighBeamOnReqStatus::OFF;
    IslaWarningSoundSymboleStatus mIslaWarningSoundSymboleStatus = IslaWarningSoundSymboleStatus::OFF;
    IslaOption2Status mIslaOption2Status = IslaOption2Status::OFF;
    PuMGroup2AdasWarning1_3Status mPuMGroup2AdasWarning1_3Status = PuMGroup2AdasWarning1_3Status::OFF;
    PuFGroup7HandsFreeFailureStatus mPuFGroup7HandsFreeFailureStatus = PuFGroup7HandsFreeFailureStatus::OFF;
    AdasOverSpeedWarningOnOffStatus mAdasOverSpeedWarnOnOffStatus = AdasOverSpeedWarningOnOffStatus::DISABLE;

    IslaNextSpeedLimitTrafficSignStatus mIslaNextSpeedLimitTrafficSignStatus = IslaNextSpeedLimitTrafficSignStatus::OFF;
    HUInt64 mIslaNextSpeedLimitTrafficSignValue = 0;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;
    HUInt64 mIslaNextSupportTrafficSignValue = 0;
    IslaSupplementaryTrafficSign2Status mIslaSupplementaryTrafficSign2Status = IslaSupplementaryTrafficSign2Status::OFF;

    MvLaneChangeRectangleStatus mMvLaneChangeRectangleStatus = MvLaneChangeRectangleStatus::OFF;
    HUInt64 mMvLaneChangeRectangleLateralPositionValue = 0;
    MvFrontObject01Status mMvFrontObject01Status = MvFrontObject01Status::OFF;
    MvFrontObjectClassification01Status mMvFrontObjectClassification01Status = MvFrontObjectClassification01Status::OFF;
    HUInt64 mMvFrontObjectLateralPosition01Value = 0;
    HUInt64 mMvFrontObjectLongitudinalPosition01Value = 0;
    HUInt64 mMvFrontObjectHeadingAngle01Value = 0;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
    static constexpr size_t kType0 = 0;
    static constexpr size_t kType1 = 1;
    static constexpr size_t kType2 = 2;
    static constexpr size_t kSurroundingObjectSize = 14;
    static constexpr size_t kSurroundingObjectInfoElementSize = 3;

    HUInt64 mMvFrontObjectLateralPositionValue = 0;
    HUInt64 mMvFrontObjectLongitudinalPositionValue = 0;
    HUInt64 mMvFrontLeftObjectLateralPositionValue = 0;
    HUInt64 mMvFrontLeftObjectLongitudinalPositionValue = 0;
    HUInt64 mMvFrontRightObjectLateralPositionValue = 0;
    HUInt64 mMvFrontRightObjectLongitudinalPositionValue = 0;
    HUInt64 mMvLeftObjectLateralPositionValue = 0;
    HUInt64 mMvLeftObjectLongitudinalPositionValue = 0;
    HUInt64 mMvRightObjectLateralPositionValue = 0;
    HUInt64 mMvRightObjectLongitudinalPositionValue = 0;
    HUInt64 mSmvSetSpeedValue = 0;
    HUInt64 mMvLeftLineOffsetValue = 0;
    HUInt64 mMvDrivingLaneRadiusValue = 0;
    HUInt64 mMvRightLineOffsetValue = 0;
    HUInt64 mMvCenterRoadSurfaceDistanceValue = 0;
    HUInt64 mIslaSpeedLimitTrafficSignValue = 0;
    HUInt64 mMvVehicleDistanceValue = 0;
    HUInt64 mMvFrontObstacleLateralPositionValue = 0;
    HUInt64 mMvFrontObstacleLongitudinalPositionValue = 0;

    MvSurroundingObject01Status mMvSurroundingObject01Status = MvSurroundingObject01Status::OFF;
    MvSurroundingObject02Status mMvSurroundingObject02Status = MvSurroundingObject02Status::OFF;
    MvSurroundingObject03Status mMvSurroundingObject03Status = MvSurroundingObject03Status::OFF;
    MvSurroundingObject04Status mMvSurroundingObject04Status = MvSurroundingObject04Status::OFF;
    MvSurroundingObject05Status mMvSurroundingObject05Status = MvSurroundingObject05Status::OFF;
    MvSurroundingObject06Status mMvSurroundingObject06Status = MvSurroundingObject06Status::OFF;
    MvSurroundingObject07Status mMvSurroundingObject07Status = MvSurroundingObject07Status::OFF;
    MvSurroundingObject08Status mMvSurroundingObject08Status = MvSurroundingObject08Status::OFF;
    MvSurroundingObject09Status mMvSurroundingObject09Status = MvSurroundingObject09Status::OFF;
    MvSurroundingObject10Status mMvSurroundingObject10Status = MvSurroundingObject10Status::OFF;
    MvSurroundingObject11Status mMvSurroundingObject11Status = MvSurroundingObject11Status::OFF;
    MvSurroundingObject12Status mMvSurroundingObject12Status = MvSurroundingObject12Status::OFF;
    MvSurroundingObject13Status mMvSurroundingObject13Status = MvSurroundingObject13Status::OFF;
    MvSurroundingObject14Status mMvSurroundingObject14Status = MvSurroundingObject14Status::OFF;
    MvSurroundingObject01Status mPrevMvSurroundingObject01Status = MvSurroundingObject01Status::OFF;
    MvSurroundingObject02Status mPrevMvSurroundingObject02Status = MvSurroundingObject02Status::OFF;
    MvSurroundingObject03Status mPrevMvSurroundingObject03Status = MvSurroundingObject03Status::OFF;
    MvSurroundingObject04Status mPrevMvSurroundingObject04Status = MvSurroundingObject04Status::OFF;
    MvSurroundingObject05Status mPrevMvSurroundingObject05Status = MvSurroundingObject05Status::OFF;
    MvSurroundingObject06Status mPrevMvSurroundingObject06Status = MvSurroundingObject06Status::OFF;
    MvSurroundingObject07Status mPrevMvSurroundingObject07Status = MvSurroundingObject07Status::OFF;
    MvSurroundingObject08Status mPrevMvSurroundingObject08Status = MvSurroundingObject08Status::OFF;
    MvSurroundingObject09Status mPrevMvSurroundingObject09Status = MvSurroundingObject09Status::OFF;
    MvSurroundingObject10Status mPrevMvSurroundingObject10Status = MvSurroundingObject10Status::OFF;
    MvSurroundingObject11Status mPrevMvSurroundingObject11Status = MvSurroundingObject11Status::OFF;
    MvSurroundingObject12Status mPrevMvSurroundingObject12Status = MvSurroundingObject12Status::OFF;
    MvSurroundingObject13Status mPrevMvSurroundingObject13Status = MvSurroundingObject13Status::OFF;
    MvSurroundingObject14Status mPrevMvSurroundingObject14Status = MvSurroundingObject14Status::OFF;
    MvSurroundingObjectClassification01Status mMvSurroundingObjectClassification01Status =
        MvSurroundingObjectClassification01Status::OFF;
    MvSurroundingObjectClassification02Status mMvSurroundingObjectClassification02Status =
        MvSurroundingObjectClassification02Status::OFF;
    MvSurroundingObjectClassification03Status mMvSurroundingObjectClassification03Status =
        MvSurroundingObjectClassification03Status::OFF;
    MvSurroundingObjectClassification04Status mMvSurroundingObjectClassification04Status =
        MvSurroundingObjectClassification04Status::OFF;
    MvSurroundingObjectClassification05Status mMvSurroundingObjectClassification05Status =
        MvSurroundingObjectClassification05Status::OFF;
    MvSurroundingObjectClassification06Status mMvSurroundingObjectClassification06Status =
        MvSurroundingObjectClassification06Status::OFF;
    MvSurroundingObjectClassification07Status mMvSurroundingObjectClassification07Status =
        MvSurroundingObjectClassification07Status::OFF;
    MvSurroundingObjectClassification08Status mMvSurroundingObjectClassification08Status =
        MvSurroundingObjectClassification08Status::OFF;
    MvSurroundingObjectClassification09Status mMvSurroundingObjectClassification09Status =
        MvSurroundingObjectClassification09Status::OFF;
    MvSurroundingObjectClassification10Status mMvSurroundingObjectClassification10Status =
        MvSurroundingObjectClassification10Status::OFF;
    MvSurroundingObjectClassification11Status mMvSurroundingObjectClassification11Status =
        MvSurroundingObjectClassification11Status::OFF;
    MvSurroundingObjectClassification12Status mMvSurroundingObjectClassification12Status =
        MvSurroundingObjectClassification12Status::OFF;
    MvSurroundingObjectClassification13Status mMvSurroundingObjectClassification13Status =
        MvSurroundingObjectClassification13Status::OFF;
    MvSurroundingObjectClassification14Status mMvSurroundingObjectClassification14Status =
        MvSurroundingObjectClassification14Status::OFF;
    HUInt64 mMvSurroundingObjectHeadingAngle01Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle02Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle03Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle04Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle05Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle06Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle07Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle08Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle09Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle10Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle11Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle12Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle13Value = 0;
    HUInt64 mMvSurroundingObjectHeadingAngle14Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition01Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition02Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition03Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition04Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition05Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition06Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition07Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition08Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition09Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition10Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition11Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition12Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition13Value = 0;
    HUInt64 mMvSurroundingObjectLateralPosition14Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition01Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition02Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition03Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition04Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition05Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition06Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition07Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition08Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition09Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition10Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition11Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition12Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition13Value = 0;
    HUInt64 mMvSurroundingObjectLongitudinalPosition14Value = 0;
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

    SFCADAS_Driving_NewInter_AdasViewFixStatus mInter_AdasViewFixStatus = SFCADAS_Driving_NewInter_AdasViewFixStatus::NONE;
    HmiMenuSwitchStatus mHmiMenuSwitchStatus = HmiMenuSwitchStatus::UN_PRESSED;

    HBool mIsIgnOn = false;
    HBool mPriorityISLAFlag = false;
    HBool mPriorityDISFlag = false;

    ssfs::SFCTimer<ADAS_Driving_New> mOneShotTimerPriorityISLA;
    ssfs::SFCTimer<ADAS_Driving_New> mOneShotTimerPriorityDIS;
    static constexpr uint32_t kTimerInterval300ms = 300u;

    HUInt64 mInfoDDTResidualDistanceValue = 0;

    // mSurroundingObjectColorInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: MvSurroundingObject01Status, ..., MvSurroundingObject14Status
    // Type1(for Type0's prev values): MvSurroundingObject01Status, ..., MvSurroundingObject14Status
    // Type2: std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor14Stat&)>
    std::tuple<
        std::tuple<std::reference_wrapper<MvSurroundingObject01Status>, std::reference_wrapper<MvSurroundingObject01Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor01Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject02Status>, std::reference_wrapper<MvSurroundingObject02Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor02Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject03Status>, std::reference_wrapper<MvSurroundingObject03Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor03Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject04Status>, std::reference_wrapper<MvSurroundingObject04Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor04Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject05Status>, std::reference_wrapper<MvSurroundingObject05Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor05Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject06Status>, std::reference_wrapper<MvSurroundingObject06Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor06Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject07Status>, std::reference_wrapper<MvSurroundingObject07Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor07Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject08Status>, std::reference_wrapper<MvSurroundingObject08Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor08Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject09Status>, std::reference_wrapper<MvSurroundingObject09Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor09Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject10Status>, std::reference_wrapper<MvSurroundingObject10Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor10Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject11Status>, std::reference_wrapper<MvSurroundingObject11Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor11Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject12Status>, std::reference_wrapper<MvSurroundingObject12Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor12Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject13Status>, std::reference_wrapper<MvSurroundingObject13Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor13Stat&)>>,
        std::tuple<std::reference_wrapper<MvSurroundingObject14Status>, std::reference_wrapper<MvSurroundingObject14Status>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectColor14Stat&)>>>
        mSurroundingObjectColorInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mMvSurroundingObject01Status), std::ref(mPrevMvSurroundingObject01Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor01Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject02Status), std::ref(mPrevMvSurroundingObject02Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor02Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject03Status), std::ref(mPrevMvSurroundingObject03Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor03Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject04Status), std::ref(mPrevMvSurroundingObject04Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor04Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject05Status), std::ref(mPrevMvSurroundingObject05Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor05Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject06Status), std::ref(mPrevMvSurroundingObject06Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor06Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject07Status), std::ref(mPrevMvSurroundingObject07Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor07Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject08Status), std::ref(mPrevMvSurroundingObject08Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor08Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject09Status), std::ref(mPrevMvSurroundingObject09Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor09Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject10Status), std::ref(mPrevMvSurroundingObject10Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor10Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject11Status), std::ref(mPrevMvSurroundingObject11Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor11Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject12Status), std::ref(mPrevMvSurroundingObject12Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor12Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject13Status), std::ref(mPrevMvSurroundingObject13Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor13Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObject14Status), std::ref(mPrevMvSurroundingObject14Status),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectColor14Stat,
                                      this, std::placeholders::_1)));

    // mSurroundingObjectClassInfoTuple = std::tuple<std::pair<Type0, Type1>, ... >
    // Type0: MvSurroundingObjectClassification01Status, ..., MvSurroundingObjectClassification14Status
    // Type1: std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass14Stat&)>
    std::tuple<std::pair<std::reference_wrapper<MvSurroundingObjectClassification01Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass01Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification02Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass02Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification03Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass03Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification04Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass04Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification05Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass05Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification06Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass06Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification07Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass07Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification08Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass08Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification09Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass09Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification10Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass10Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification11Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass11Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification12Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass12Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification13Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass13Stat&)>>,
               std::pair<std::reference_wrapper<MvSurroundingObjectClassification14Status>,
                         std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectClass14Stat&)>>>
        mSurroundingObjectClassInfoTuple = std::make_tuple(
            std::make_pair(std::ref(mMvSurroundingObjectClassification01Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass01Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification02Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass02Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification03Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass03Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification04Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass04Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification05Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass05Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification06Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass06Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification07Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass07Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification08Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass08Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification09Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass09Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification10Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass10Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification11Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass11Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification12Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass12Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification13Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass13Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mMvSurroundingObjectClassification14Status),
                           std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectClass14Stat,
                                     this, std::placeholders::_1)));

    // mSurroundingObjectLatInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat02Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat03Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat04Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat05Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat06Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat07Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat08Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat09Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat10Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat11Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat12Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat13Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Stat&)>,
                          std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLatInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition01Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat01Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition02Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat02Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat02Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition03Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat03Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat03Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition04Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat04Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat04Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition05Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat05Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat05Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition06Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat06Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat06Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition07Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat07Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat07Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition08Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat08Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat08Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition09Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat09Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat09Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition10Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat10Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat10Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition11Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat11Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat11Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition12Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat12Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat12Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition13Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat13Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat13Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLateralPosition14Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLat14Value,
                                      this, std::placeholders::_1)));

    // mSurroundingObjectLongInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong02Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong03Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong04Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong05Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong06Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong07Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong08Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong09Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong10Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong11Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong12Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong13Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Stat&)>,
                          std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLongInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition01Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong01Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition02Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong02Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong02Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition03Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong03Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong03Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition04Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong04Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong04Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition05Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong05Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong05Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition06Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong06Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong06Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition07Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong07Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong07Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition08Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong08Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong08Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition09Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong09Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong09Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition10Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong10Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong10Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition11Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong11Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong11Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition12Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong12Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong12Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition13Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong13Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong13Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mMvSurroundingObjectLongitudinalPosition14Value),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectLong14Value,
                                      this, std::placeholders::_1)));

    // mSurroundingObjectHeadingAngleInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Stat&)>
    // Type2: std::function<std::string(const HUInt64&)>
    std::tuple<
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle02Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle03Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle04Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle05Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle06Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle07Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle08Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle09Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle10Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle11Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle12Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle13Stat&)>,
                   std::function<std::string(const HUInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Stat&)>,
                   std::function<std::string(const HUInt64&)>>>
        mSurroundingObjectHeadingAngleInfoTuple = std::make_tuple(
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle01Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle01Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle02Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle02Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle02Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle03Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle03Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle03Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle04Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle04Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle04Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle05Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle05Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle05Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle06Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle06Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle06Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle07Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle07Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle07Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle08Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle08Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle08Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle09Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle09Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle09Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle10Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle10Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle10Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle11Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle11Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle11Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle12Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle12Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle12Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle13Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle13Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle13Value, this,
                          std::placeholders::_1)),
            std::make_tuple(
                std::ref(mMvSurroundingObjectHeadingAngle14Value),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Stat, this,
                          std::placeholders::_1),
                std::bind(&ADAS_Driving_NewBase::setSFCADAS_Driving_NewConstantADASViewSurroundingObjectHeadingAngle14Value, this,
                          std::placeholders::_1)));
};

}  // namespace ccos

#endif  // SFSS_ADAS_Driving_New_H
