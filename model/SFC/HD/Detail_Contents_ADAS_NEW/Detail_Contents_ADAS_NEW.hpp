/**
 * @file Detail_Contents_ADAS_NEW.hpp
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
#ifndef SFSS_Detail_Contents_ADAS_NEW_H
#define SFSS_Detail_Contents_ADAS_NEW_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_ADAS_NEWBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 62.0.1
// Reference : [HD170] Detail_Contents_ADAS_NEW. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Detail_Contents_ADAS_NEW : public Detail_Contents_ADAS_NEWBase {
public:
    Detail_Contents_ADAS_NEW() = default;
    ~Detail_Contents_ADAS_NEW() override = default;
    Detail_Contents_ADAS_NEW(const Detail_Contents_ADAS_NEW& other) = delete;
    Detail_Contents_ADAS_NEW(Detail_Contents_ADAS_NEW&& other) noexcept = delete;
    Detail_Contents_ADAS_NEW& operator=(const Detail_Contents_ADAS_NEW& other) = delete;
    Detail_Contents_ADAS_NEW& operator=(Detail_Contents_ADAS_NEW&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDetail_Contents_ADAS_NEWEventCrawlID("E71200");
        setSFCDetail_Contents_ADAS_NEWEventCcMslaSummaryID("E71203");
        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799ID("E71799");
        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71800ID("E71800");
        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71801ID("E71801");
        if (mOneShotTimerSummaryEventExpired.create(kTimerSummaryEventExpiredInterval160ms, this,
                                                    &Detail_Contents_ADAS_NEW::onOneShotTimerSummaryEventExpired,
                                                    false) == false) {
            DWarning() << "[Detail_Contents_ADAS_NEW] mOneShotTimerSummaryEventExpired Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        if (mIsConfigOn != true) {
            return;
        }

        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        if (mIsConfigOn != true) {
            return;
        }

        updateAll();
    }

    void onCcMslaActiveStatChanged(const ArgumentsCcMslaActiveStatChanged& args) {
        mCcActiveStat = args.mInter_CcActiveStat;
        mMslaActiveStat = args.mInter_MslaActiveStat;

        if (mIsConfigOn != true) {
            return;
        }

        updateCcMslaSpeed();
    }

    void onCcSldMaxSpeedValueChanged(const ArgumentsCcSldMaxSpeedValueChanged& args) {
        mCcSldMaxSpeedValue = args.mInter_CcSldMaxSpeedValue;
        if (mIsConfigOn != true) {
            return;
        }
        updateCcMslaSpeed();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updatePrivateCcSldSpeedValue();
        updateCcSpeedStat();
    }

    void onConfigOnChanged(const ArgumentsConfigOnChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        mIsConfigOn = (mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON);
        if (mIsConfigOn != true) {
            return;
        }

        updateAll();
    }

    void onConfigHDPNameChanged(const ArgumentsConfigHDPNameChanged& args) {
        mConfigHDPName = args.mInter_ConfigHDPName;
        if (mIsConfigOn != true) {
            return;
        }
        updateHdaHdpTelltale();
        updateHdpPopups();
    }

    void onConfigSccChanged(const ArgumentsConfigSccChanged& args) {
        mConfigAdasScc = args.mInter_ConfigAdasSCC;

        updateEventCcMslaSummaryStat();
        updateIslaManualSetSpeedForCcMslaStat();
    }

    void onMslaOverrideChanged(const ArgumentsMslaOverrideChanged& args) {
        mMslaOverride = args.mInter_Mslaoverride;
        if (mIsConfigOn != true) {
            return;
        }

        updateCcMslaSpeed();
    }

    void onCrawlActiveStatusChanged(const ArgumentsCrawlActiveStatusChanged& args) {
        mCrawlActiveStatus = args.mInput_CrawlActiveStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateCrawlOperationStatus();
        updateCrawlOperationStepStatus();
        updateCrawlSummaryEvent();
    }

    void onCrawlActiveStepStatusChanged(const ArgumentsCrawlActiveStepStatusChanged& args) {
        mCrawlActiveStepStatus = args.mInput_CrawlActiveStepStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateCrawlOperationStatus();
        updateCrawlOperationStepStatus();
        updateCrawlSummaryEvent();
    }

    void onCcStatChanged(const ArgumentsCcStatChanged& args) {
        mCcOnOffStat = args.mInter_CcOnOffStat;
        updateCcSpeedStat();
    }

    void onMslaStatChanged(const ArgumentsMslaStatChanged& args) {
        mMslaOnOffStat = args.mInter_MslaOnOffStat;
        updateMslaSpeedStat();
    }

    void onCcSetIndStatusChanged(const ArgumentsCcSetIndStatusChanged& args) {
        mCruiseSetIndReqStatus = args.mInput_CruiseSetIndReqStatus;
        updateCcSpeedStat();
    }

    void onCcMslaChanged(const ArgumentsCcMslaChanged& args) {
        if (ISTIMEOUT(args.mInput_CcOnlySetSpeedValue) == true) {
            mCcOnlySetSpeedValue = kTimeOut;
        } else {
            mCcOnlySetSpeedValue = args.mInput_CcOnlySetSpeedValue;
        }
        if (ISTIMEOUT(args.mInput_CcSldSetSpeedValue) == true) {
            mCcSldSetSpeedValue = kTimeOut;
        } else {
            mCcSldSetSpeedValue = args.mInput_CcSldSetSpeedValue;
        }
        mCruiseMainIndReqStatus = args.mInput_CruiseMainIndReqStatus;
        mCruiseOverrideReqStatus = args.mInput_CruiseOverrideReqStatus;
        mSpeedLimitActiveStatus = args.mInput_SpeedLimitActiveStatus;
        mSpeedLimitReadyStatus = args.mInput_SpeedLimitReadyStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updatePrivateCcSldSpeedValue();
        updateEventCcMslaSummary();
        updateCcMslaSpeed();
        updateIslaManualSetSpeedForCcMslaStat();
    }

    void onCenterRoadChanged(const ArgumentsCenterRoadChanged& args) {
        mMvCenterRoadSurfaceDistanceValue = args.mInput_MvCenterRoadSurfaceDistanceValue;
        mMvCenterRoadSurfaceStatus = args.mInput_MvCenterRoadSurfaceStatus;
        mMvLeftLineOffsetValue = args.mInput_MvLeftLineOffsetValue;
        mMvRightLineOffsetValue = args.mInput_MvRightLineOffsetValue;
        mMvDrivingLaneRadiusValue = args.mInput_MvDrivingLaneRadiusValue;

        if (ISTIMEOUT(args.mInput_MvDrivingLaneRadiusValue) == true) {
            mMvDrivingLaneRadiusValue = kTimeOut;
        }

        if (ISTIMEOUT(args.mInput_MvLeftLineOffsetValue) == true) {
            mMvLeftLineOffsetValue = kTimeOut;
        }

        if (ISTIMEOUT(args.mInput_MvRightLineOffsetValue) == true) {
            mMvRightLineOffsetValue = kTimeOut;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateCenterRoad();
        updateLaneCvrtLvlValue();
        updateRoadCvrtLvlValue();
        updateLeftLatPosValue();
        updateRightLatPosValue();
    }

    void onFrontLeftObjectPositionChanged(const ArgumentsFrontLeftObjectPositionChanged& args) {
        mMvFrontLeftObjectLateralPositionValue = args.mInput_MvFrontLeftObjectLateralPositionValue;
        mMvFrontLeftObjectLongitudinalPositionValue = args.mInput_MvFrontLeftObjectLongitudinalPositionValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontLeftVehicleLatValue();
        udpateConstantADASViewFrontLeftVehicleLongValue();
    }

    void onFrontLeftObjectChanged(const ArgumentsFrontLeftObjectChanged& args) {
        mMvFrontLeftObjectStatus = args.mInput_MvFrontLeftObjectStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontLeftVehicleStat();
    }

    void onFrontObjectChanged(const ArgumentsFrontObjectChanged& args) {
        mMvFrontObjectLateralPositionValue = args.mInput_MvFrontObjectLateralPositionValue;
        mMvFrontObjectLongitudinalPositionValue = args.mInput_MvFrontObjectLongitudinalPositionValue;
        mMvFrontObjectStatus = args.mInput_MvFrontObjectStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateFrontObject();
    }

    void onFrontRightObjectPositionChanged(const ArgumentsFrontRightObjectPositionChanged& args) {
        mMvFrontRightObjectLateralPositionValue = args.mInput_MvFrontRightObjectLateralPositionValue;
        mMvFrontRightObjectLongitudinalPositionValue = args.mInput_MvFrontRightObjectLongitudinalPositionValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontRightVehicleLatValue();
        udpateConstantADASViewFrontRightVehicleLongValue();
    }

    void onFrontRightObjectChanged(const ArgumentsFrontRightObjectChanged& args) {
        mMvFrontRightObjectStatus = args.mInput_MvFrontRightObjectStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontRightVehicleStat();
    }

    void onLeftLineChanged(const ArgumentsLeftLineChanged& args) {
        mMvLeftLaneChangeDirectionStatus = args.mInput_MvLeftLaneChangeDirectionStatus;
        mMvLeftLineStatus = args.mInput_MvLeftLineStatus;
        mMvLeftRoadSurfaceStatus = args.mInput_MvLeftRoadSurfaceStatus;

        if (ISTIMEOUT(args.mInput_MvLeftLineStatus) == true) {
            mMvLeftLineStatus = MvLeftLineStatus::UNHANDLED_TIMEOUT;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateLeftLane();
        updateLeftRoad();
    }

    void onLeftObjectChanged(const ArgumentsLeftObjectChanged& args) {
        mMvLeftObjectLateralPositionValue = args.mInput_MvLeftObjectLateralPositionValue;
        mMvLeftObjectLongitudinalPositionValue = args.mInput_MvLeftObjectLongitudinalPositionValue;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLeftVehicleLatValue();
        udpateConstantADASViewLeftVehicleLongValue();
    }

    void onLeftObjectStatusChanged(const ArgumentsLeftObjectStatusChanged& args) {
        mMvLeftObjectStatus = args.mInput_MvLeftObjectStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLeftVehicleStat();
    }

    void onRadarWaveStatusChanged(const ArgumentsRadarWaveStatusChanged& args) {
        mMvFrontRadarWaveStatus = args.mInput_MvFrontRadarWaveStatus;
        mMvRearLeftRadarWave1Status = args.mInput_MvRearLeftRadarWave1Status;
        mMvRearLeftRadarWave2Status = args.mInput_MvRearLeftRadarWave2Status;
        mMvRearRightRadarWave1Status = args.mInput_MvRearRightRadarWave1Status;
        mMvRearRightRadarWave2Status = args.mInput_MvRearRightRadarWave2Status;
        if (mIsConfigOn != true) {
            return;
        }

        updateRadarWave();
    }

    void onRightLineChanged(const ArgumentsRightLineChanged& args) {
        mMvRightLaneChangeDirectionStatus = args.mInput_MvRightLaneChangeDirectionStatus;
        mMvRightLineStatus = args.mInput_MvRightLineStatus;
        mMvRightRoadSurfaceStatus = args.mInput_MvRightRoadSurfaceStatus;

        if (ISTIMEOUT(args.mInput_MvRightLineStatus) == true) {
            mMvRightLineStatus = MvRightLineStatus::UNHANDLED_TIMEOUT;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateRightLane();
        updateRightRoad();
    }

    void onRightObjectChanged(const ArgumentsRightObjectChanged& args) {
        mMvRightObjectLateralPositionValue = args.mInput_MvRightObjectLateralPositionValue;
        mMvRightObjectLongitudinalPositionValue = args.mInput_MvRightObjectLongitudinalPositionValue;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewRightVehicleLatValue();
        udpateConstantADASViewRightVehicleLongValue();
    }

    void onRightObjectStatusChanged(const ArgumentsRightObjectStatusChanged& args) {
        mMvRightObjectStatus = args.mInput_MvRightObjectStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewRightVehicleStat();
    }

    void onMvFrontObstacleStatusChanged(const ArgumentsMvFrontObstacleStatusChanged& args) {
        mMvFrontObstacleStatus = args.mInput_MvFrontObstacleStatus;
        if (mIsConfigOn != true) {
            return;
        }
        updateADASViewFrontObstacleColorStat();
    }

    void onMvFrontObstacleLateralPositionValueChanged(const ArgumentsMvFrontObstacleLateralPositionValueChanged& args) {
        mMvFrontObstacleLateralPositionValue = args.mInput_MvFrontObstacleLateralPositionValue;
        if (mIsConfigOn != true) {
            return;
        }
        updateADASViewFrontObstacleLatPosStat();
    }

    void onMvFrontObstacleLongitudinalPositionValueChanged(const ArgumentsMvFrontObstacleLongitudinalPositionValueChanged& args) {
        mMvFrontObstacleLongitudinalPositionValue = args.mInput_MvFrontObstacleLongitudinalPositionValue;
        if (mIsConfigOn != true) {
            return;
        }
        updateADASViewFrontObstacleLongPosStat();
    }

    void onVehicleDistanceChanged(const ArgumentsVehicleDistanceChanged& args) {
        mMvVehicleDistanceStatus = args.mInput_MvVehicleDistanceStatus;
        mMvVehicleDistanceValue = args.mInput_MvVehicleDistanceValue;
        mNccCameraOperStatus = args.mInput_NccCameraOperStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateHeadwayVehicleDistance();
        updateConstantNaviCruiseStat();
    }

    void onGroup1AdasWarning1_1StatusChanged(const ArgumentsGroup1AdasWarning1_1StatusChanged& args) {
        mGroup1AdasWarning1_1Status = args.mInput_PuFGroup1AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        update1Group1_1Event();
    }

    void onGroup1AdasWarning1_2StatusChanged(const ArgumentsGroup1AdasWarning1_2StatusChanged& args) {
        mGroup1AdasWarning1_2Status = args.mInput_PuFGroup1AdasWarning1_2Status;

        if (mIsConfigOn != true) {
            return;
        }

        update1Group1_2Event();
    }

    void onGroup4AdasWarning1_1StatusChanged(const ArgumentsGroup4AdasWarning1_1StatusChanged& args) {
        mGroup4AdasWarning1_1Status = args.mInput_PuFGroup4AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        update4Group1_1Event();
    }

    void onGroup2AdasWarning1_1StatusChanged(const ArgumentsGroup2AdasWarning1_1StatusChanged& args) {
        mGroup2AdasWarning1_1Status = args.mInput_PuMGroup2AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        update2Group1_1Event();
    }

    void onGroup2AdasWarning1_2StatusChanged(const ArgumentsGroup2AdasWarning1_2StatusChanged& args) {
        mGroup2AdasWarning1_2Status = args.mInput_PuMGroup2AdasWarning1_2Status;

        if (mIsConfigOn != true) {
            return;
        }

        update2Group1_2Event();
    }

    void onGroup2AdasWarning1_3StatusChanged(const ArgumentsGroup2AdasWarning1_3StatusChanged& args) {
        mGroup2AdasWarning1_3Status = args.mInput_PuMGroup2AdasWarning1_3Status;

        if (mIsConfigOn != true) {
            return;
        }

        update2Group1_3Event();
    }

    void onCruiseSymbolChanged(const ArgumentsCruiseSymbolChanged& args) {
        // TODO(CJH): CRUISE SYMBOL 사양서 상 사양협의 중
        // 5.11.5.2.1.2.1 CRUISE 심볼
        mSccOperatingStatus = args.mInput_SccOperatingStatus;
        mSccTargetSpeedSettingValue = args.mInput_SccTargetSpeedSettingValue;
    }

    void onSmvChanged(const ArgumentsSmvChanged& args) {
        mSmvHdaSymbolStatus = args.mInput_SmvHdaSymbolStatus;
        mSmvIslaSetSpeedSymbolStatus = args.mInput_SmvIslaSetSpeedSymbolStatus;
        mSmvNsccSymbolStatus = args.mInput_SmvNsccSymbolStatus;
        mSmvDrvAsstHUDSymbStatus = args.mInput_SmvDrvAsstHUDSymbStatus;
        mSmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        mSmvSetSpeedValue = args.mInput_SmvSetSpeedValue;

        if (ISTIMEOUT(args.mInput_SmvSetSpeedStatus) == true) {
            mSmvSetSpeedStatus = SmvSetSpeedStatus::UNHANDLED_TIMEOUT;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateAdasSummaryInfo();
        updateConstantAdasCruise();
        updateMslaSpeedStat();
    }

    void onMvDriverHandsOnOffStatusChanged(const ArgumentsMvDriverHandsOnOffStatusChanged& args) {
        mMvDriverHandsOnOffStatus = args.mInput_MvDriverHandsOnOffStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateHandsOnOffTelltaleStat();
    }

    void onMvLaneChangedRectangleStatus(const ArgumentsMvLaneChangedRectangleStatus& args) {
        mMvLaneChangeRectangleStatus = args.mInput_MvLaneChangeRectangleStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewLaneChangeRectangle();
    }

    void onMvLaneChangedRectangleLateralPositionValue(const ArgumentsMvLaneChangedRectangleLateralPositionValue& args) {
        mMvLaneChangeRectangleLateralPositionValue = args.mInput_MvLaneChangeRectangleLateralPositionValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewLaneChangeRectangleLatValue();
        updateADASViewLeftArrowLatValue();
        updateADASViewRightArrowLatValue();
    }

    void onIsMvFrontObject01StatusChanged(const ArgumentsIsMvFrontObject01StatusChanged& args) {
        mMvFrontObject01Status = args.mInput_MvFrontObject01Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontObjectColor01Stat();
    }

    void onIsMvFrontObjectClassification01StatusChanged(const ArgumentsIsMvFrontObjectClassification01StatusChanged& args) {
        mMvFrontObjectClassification01Status = args.mInput_MvFrontObjectClassification01Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontObjectClass01Stat();
    }

    void onIsMvFrontObjectLateralPosition01ValueChanged(const ArgumentsIsMvFrontObjectLateralPosition01ValueChanged& args) {
        mMvFrontObjectLateralPosition01Value = args.mInput_MvFrontObjectLateralPosition01Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontObjectLat01();
    }

    void onIsMvFrontObjectLongitudinalPosition01ValueChanged(
        const ArgumentsIsMvFrontObjectLongitudinalPosition01ValueChanged& args) {
        mMvFrontObjectLongitudinalPosition01Value = args.mInput_MvFrontObjectLongitudinalPosition01Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontObjectLong01();
    }

    void onIsMvFrontObjectHeadingAngle01ValueChanged(const ArgumentsIsMvFrontObjectHeadingAngle01ValueChanged& args) {
        mMvFrontObjectHeadingAngle01Value = args.mInput_MvFrontObjectHeadingAngle01Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontObjectHeadingAngle01();
    }

    void onMvSurroundingObject01to05StatusChanged(const ArgumentsMvSurroundingObject01to05StatusChanged& args) {
        mPrevMvSurroundingObject01Status = mMvSurroundingObject01Status;
        mPrevMvSurroundingObject02Status = mMvSurroundingObject02Status;
        mPrevMvSurroundingObject03Status = mMvSurroundingObject03Status;
        mPrevMvSurroundingObject04Status = mMvSurroundingObject04Status;
        mPrevMvSurroundingObject05Status = mMvSurroundingObject05Status;

        mMvSurroundingObject01Status = args.mInput_MvSurroundingObject01Status;
        mMvSurroundingObject02Status = args.mInput_MvSurroundingObject02Status;
        mMvSurroundingObject03Status = args.mInput_MvSurroundingObject03Status;
        mMvSurroundingObject04Status = args.mInput_MvSurroundingObject04Status;
        mMvSurroundingObject05Status = args.mInput_MvSurroundingObject05Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectColor01();
        updateADASViewSurroundingObjectColor02();
        updateADASViewSurroundingObjectColor03();
        updateADASViewSurroundingObjectColor04();
        updateADASViewSurroundingObjectColor05();
    }

    void onMvSurroundingObject06to11StatusChanged(const ArgumentsMvSurroundingObject06to11StatusChanged& args) {
        mPrevMvSurroundingObject06Status = mMvSurroundingObject06Status;
        mPrevMvSurroundingObject07Status = mMvSurroundingObject07Status;
        mPrevMvSurroundingObject08Status = mMvSurroundingObject08Status;
        mPrevMvSurroundingObject09Status = mMvSurroundingObject09Status;
        mPrevMvSurroundingObject10Status = mMvSurroundingObject10Status;
        mPrevMvSurroundingObject11Status = mMvSurroundingObject11Status;

        mMvSurroundingObject06Status = args.mInput_MvSurroundingObject06Status;
        mMvSurroundingObject07Status = args.mInput_MvSurroundingObject07Status;
        mMvSurroundingObject08Status = args.mInput_MvSurroundingObject08Status;
        mMvSurroundingObject09Status = args.mInput_MvSurroundingObject09Status;
        mMvSurroundingObject10Status = args.mInput_MvSurroundingObject10Status;
        mMvSurroundingObject11Status = args.mInput_MvSurroundingObject11Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectColor06();
        updateADASViewSurroundingObjectColor07();
        updateADASViewSurroundingObjectColor08();
        updateADASViewSurroundingObjectColor09();
        updateADASViewSurroundingObjectColor10();
        updateADASViewSurroundingObjectColor11();
    }

    void onMvSurroundingObject12to14StatusChanged(const ArgumentsMvSurroundingObject12to14StatusChanged& args) {
        mPrevMvSurroundingObject12Status = mMvSurroundingObject12Status;
        mPrevMvSurroundingObject13Status = mMvSurroundingObject13Status;
        mPrevMvSurroundingObject14Status = mMvSurroundingObject14Status;

        mMvSurroundingObject12Status = args.mInput_MvSurroundingObject12Status;
        mMvSurroundingObject13Status = args.mInput_MvSurroundingObject13Status;
        mMvSurroundingObject14Status = args.mInput_MvSurroundingObject14Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectColor12();
        updateADASViewSurroundingObjectColor13();
        updateADASViewSurroundingObjectColor14();
    }

    void onMvSurroundingObjectClassification01to05StatusChanged(
        const ArgumentsMvSurroundingObjectClassification01to05StatusChanged& args) {
        mMvSurroundingObjectClassification01Status = args.mInput_MvSurroundingObjectClassification01Status;
        mMvSurroundingObjectClassification02Status = args.mInput_MvSurroundingObjectClassification02Status;
        mMvSurroundingObjectClassification03Status = args.mInput_MvSurroundingObjectClassification03Status;
        mMvSurroundingObjectClassification04Status = args.mInput_MvSurroundingObjectClassification04Status;
        mMvSurroundingObjectClassification05Status = args.mInput_MvSurroundingObjectClassification05Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectClass01();
        updateADASViewSurroundingObjectClass02();
        updateADASViewSurroundingObjectClass03();
        updateADASViewSurroundingObjectClass04();
        updateADASViewSurroundingObjectClass05();
    }

    void onMvSurroundingObjectClassification06to11StatusChanged(
        const ArgumentsMvSurroundingObjectClassification06to11StatusChanged& args) {
        mMvSurroundingObjectClassification06Status = args.mInput_MvSurroundingObjectClassification06Status;
        mMvSurroundingObjectClassification07Status = args.mInput_MvSurroundingObjectClassification07Status;
        mMvSurroundingObjectClassification08Status = args.mInput_MvSurroundingObjectClassification08Status;
        mMvSurroundingObjectClassification09Status = args.mInput_MvSurroundingObjectClassification09Status;
        mMvSurroundingObjectClassification10Status = args.mInput_MvSurroundingObjectClassification10Status;
        mMvSurroundingObjectClassification11Status = args.mInput_MvSurroundingObjectClassification11Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectClass06();
        updateADASViewSurroundingObjectClass07();
        updateADASViewSurroundingObjectClass08();
        updateADASViewSurroundingObjectClass09();
        updateADASViewSurroundingObjectClass10();
        updateADASViewSurroundingObjectClass11();
    }

    void onMvSurroundingObjectClassification12to14StatusChanged(
        const ArgumentsMvSurroundingObjectClassification12to14StatusChanged& args) {
        mMvSurroundingObjectClassification12Status = args.mInput_MvSurroundingObjectClassification12Status;
        mMvSurroundingObjectClassification13Status = args.mInput_MvSurroundingObjectClassification13Status;
        mMvSurroundingObjectClassification14Status = args.mInput_MvSurroundingObjectClassification14Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectClass12();
        updateADASViewSurroundingObjectClass13();
        updateADASViewSurroundingObjectClass14();
    }

    void onMvSurroundingObjectHeadingAngle01to05ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle01to05ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle01Value = args.mInput_MvSurroundingObjectHeadingAngle01Value;
        mMvSurroundingObjectHeadingAngle02Value = args.mInput_MvSurroundingObjectHeadingAngle02Value;
        mMvSurroundingObjectHeadingAngle03Value = args.mInput_MvSurroundingObjectHeadingAngle03Value;
        mMvSurroundingObjectHeadingAngle04Value = args.mInput_MvSurroundingObjectHeadingAngle04Value;
        mMvSurroundingObjectHeadingAngle05Value = args.mInput_MvSurroundingObjectHeadingAngle05Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectHeadingAngle01();
        updateADASViewSurroundingObjectHeadingAngle02();
        updateADASViewSurroundingObjectHeadingAngle03();
        updateADASViewSurroundingObjectHeadingAngle04();
        updateADASViewSurroundingObjectHeadingAngle05();
    }

    void onMvSurroundingObjectHeadingAngle06to11ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle06to11ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle06Value = args.mInput_MvSurroundingObjectHeadingAngle06Value;
        mMvSurroundingObjectHeadingAngle07Value = args.mInput_MvSurroundingObjectHeadingAngle07Value;
        mMvSurroundingObjectHeadingAngle08Value = args.mInput_MvSurroundingObjectHeadingAngle08Value;
        mMvSurroundingObjectHeadingAngle09Value = args.mInput_MvSurroundingObjectHeadingAngle09Value;
        mMvSurroundingObjectHeadingAngle10Value = args.mInput_MvSurroundingObjectHeadingAngle10Value;
        mMvSurroundingObjectHeadingAngle11Value = args.mInput_MvSurroundingObjectHeadingAngle11Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectHeadingAngle06();
        updateADASViewSurroundingObjectHeadingAngle07();
        updateADASViewSurroundingObjectHeadingAngle08();
        updateADASViewSurroundingObjectHeadingAngle09();
        updateADASViewSurroundingObjectHeadingAngle10();
        updateADASViewSurroundingObjectHeadingAngle11();
    }

    void onMvSurroundingObjectHeadingAngle12to14ValueChanged(
        const ArgumentsMvSurroundingObjectHeadingAngle12to14ValueChanged& args) {
        mMvSurroundingObjectHeadingAngle12Value = args.mInput_MvSurroundingObjectHeadingAngle12Value;
        mMvSurroundingObjectHeadingAngle13Value = args.mInput_MvSurroundingObjectHeadingAngle13Value;
        mMvSurroundingObjectHeadingAngle14Value = args.mInput_MvSurroundingObjectHeadingAngle14Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectHeadingAngle12();
        updateADASViewSurroundingObjectHeadingAngle13();
        updateADASViewSurroundingObjectHeadingAngle14();
    }

    void onMvSurroundingObjectLateralPosition01to05ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition01to05ValueChanged& args) {
        mMvSurroundingObjectLateralPosition01Value = args.mInput_MvSurroundingObjectLateralPosition01Value;
        mMvSurroundingObjectLateralPosition02Value = args.mInput_MvSurroundingObjectLateralPosition02Value;
        mMvSurroundingObjectLateralPosition03Value = args.mInput_MvSurroundingObjectLateralPosition03Value;
        mMvSurroundingObjectLateralPosition04Value = args.mInput_MvSurroundingObjectLateralPosition04Value;
        mMvSurroundingObjectLateralPosition05Value = args.mInput_MvSurroundingObjectLateralPosition05Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLat01();
        updateADASViewSurroundingObjectLat02();
        updateADASViewSurroundingObjectLat03();
        updateADASViewSurroundingObjectLat04();
        updateADASViewSurroundingObjectLat05();
    }

    void onMvSurroundingObjectLateralPosition06to11ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition06to11ValueChanged& args) {
        mMvSurroundingObjectLateralPosition06Value = args.mInput_MvSurroundingObjectLateralPosition06Value;
        mMvSurroundingObjectLateralPosition07Value = args.mInput_MvSurroundingObjectLateralPosition07Value;
        mMvSurroundingObjectLateralPosition08Value = args.mInput_MvSurroundingObjectLateralPosition08Value;
        mMvSurroundingObjectLateralPosition09Value = args.mInput_MvSurroundingObjectLateralPosition09Value;
        mMvSurroundingObjectLateralPosition10Value = args.mInput_MvSurroundingObjectLateralPosition10Value;
        mMvSurroundingObjectLateralPosition11Value = args.mInput_MvSurroundingObjectLateralPosition11Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLat06();
        updateADASViewSurroundingObjectLat07();
        updateADASViewSurroundingObjectLat08();
        updateADASViewSurroundingObjectLat09();
        updateADASViewSurroundingObjectLat10();
        updateADASViewSurroundingObjectLat11();
    }

    void onMvSurroundingObjectLateralPosition12to14ValueChanged(
        const ArgumentsMvSurroundingObjectLateralPosition12to14ValueChanged& args) {
        mMvSurroundingObjectLateralPosition12Value = args.mInput_MvSurroundingObjectLateralPosition12Value;
        mMvSurroundingObjectLateralPosition13Value = args.mInput_MvSurroundingObjectLateralPosition13Value;
        mMvSurroundingObjectLateralPosition14Value = args.mInput_MvSurroundingObjectLateralPosition14Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLat12();
        updateADASViewSurroundingObjectLat13();
        updateADASViewSurroundingObjectLat14();
    }

    void onMvSurroundingObjectLongitudinalPosition01to05ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition01to05ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition01Value = args.mInput_MvSurroundingObjectLongitudinalPosition01Value;
        mMvSurroundingObjectLongitudinalPosition02Value = args.mInput_MvSurroundingObjectLongitudinalPosition02Value;
        mMvSurroundingObjectLongitudinalPosition03Value = args.mInput_MvSurroundingObjectLongitudinalPosition03Value;
        mMvSurroundingObjectLongitudinalPosition04Value = args.mInput_MvSurroundingObjectLongitudinalPosition04Value;
        mMvSurroundingObjectLongitudinalPosition05Value = args.mInput_MvSurroundingObjectLongitudinalPosition05Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLong01();
        updateADASViewSurroundingObjectLong02();
        updateADASViewSurroundingObjectLong03();
        updateADASViewSurroundingObjectLong04();
        updateADASViewSurroundingObjectLong05();
    }

    void onMvSurroundingObjectLongitudinalPosition06to11ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition06to11ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition06Value = args.mInput_MvSurroundingObjectLongitudinalPosition06Value;
        mMvSurroundingObjectLongitudinalPosition07Value = args.mInput_MvSurroundingObjectLongitudinalPosition07Value;
        mMvSurroundingObjectLongitudinalPosition08Value = args.mInput_MvSurroundingObjectLongitudinalPosition08Value;
        mMvSurroundingObjectLongitudinalPosition09Value = args.mInput_MvSurroundingObjectLongitudinalPosition09Value;
        mMvSurroundingObjectLongitudinalPosition10Value = args.mInput_MvSurroundingObjectLongitudinalPosition10Value;
        mMvSurroundingObjectLongitudinalPosition11Value = args.mInput_MvSurroundingObjectLongitudinalPosition11Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLong06();
        updateADASViewSurroundingObjectLong07();
        updateADASViewSurroundingObjectLong08();
        updateADASViewSurroundingObjectLong09();
        updateADASViewSurroundingObjectLong10();
        updateADASViewSurroundingObjectLong11();
    }

    void onMvSurroundingObjectLongitudinalPosition12to14ValueChanged(
        const ArgumentsMvSurroundingObjectLongitudinalPosition12to14ValueChanged& args) {
        mMvSurroundingObjectLongitudinalPosition12Value = args.mInput_MvSurroundingObjectLongitudinalPosition12Value;
        mMvSurroundingObjectLongitudinalPosition13Value = args.mInput_MvSurroundingObjectLongitudinalPosition13Value;
        mMvSurroundingObjectLongitudinalPosition14Value = args.mInput_MvSurroundingObjectLongitudinalPosition14Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateADASViewSurroundingObjectLong12();
        updateADASViewSurroundingObjectLong13();
        updateADASViewSurroundingObjectLong14();
    }

    void onSmvLfaSymbolStatusChanged(const ArgumentsSmvLfaSymbolStatusChanged& args) {
        mSmvLfaSymbolStatus = args.mInput_SmvLfaSymbolStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantLFATelltaleStat();
    }

    void onInfoDisplayReqChanged(const ArgumentsInfoDisplayReqChanged& args) {
        mCruiseInfoDisplayReqStatus = args.mInput_CruiseInfoDisplayReqStatus;
        mSpeedLimitInfoDisplayReqStatus = args.mInput_SpeedLimitInfoDisplayReqStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateMslaEvent();
        updateCcEvent();
    }

    void onHdpRDDWarningStateChanged(const ArgumentsHdpRDDWarningStateChanged& args) {
        mHdpRDDWarningState = args.mInput_HDP_RDDWarningState;
        if (mIsConfigOn != true) {
            return;
        }

        update1Group3_1Event_E71799();
        update1Group3_1Event_E71800();
        update1Group3_1Event_E71801();
    }

    void onEscHDPEngagedStatusChanged(const ArgumentsEscHDPEngagedStatusChanged& args) {
        mEscHDPEngagedStatus = args.mInput_ESC_HDPEngagedStatus;
        if (mIsConfigOn != true) {
            return;
        }

        update1Group3_1Event_E71799();
    }

    void onRcuHDPEngagedStatusChanged(const ArgumentsRcuHDPEngagedStatusChanged& args) {
        mRcuHDPEngagedStatus = args.mInput_RCU_HDPEngagedStatus;
        if (mIsConfigOn != true) {
            return;
        }

        update1Group3_1Event_E71799();
    }

    void onHudBcwInfoOnOffStatusChanged(const ArgumentsHudBcwInfoOnOffStatusChanged& args) {
        mHudBcwInfoOnOffStatus = args.mInter_HudBcwInfoOnOffStatus;
        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewRearRightWarnStat();
        updateConstantADASViewRearLeftWarnStat();
    }

    void onHudSccAdasInfoOnOffStatusChanged(const ArgumentsHudSccAdasInfoOnOffStatusChanged& args) {
        mHudSccInfoOnOffStatus = args.mInter_HudSCCInfoOnOffStatus;
        mHudAdasInfoOnOffStatus = args.mInter_HudAdasInfoOnOffStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateCenterRoad();
        updateRoadCvrtLvlValue();
        updateLeftLatPosValue();
        updateRightLatPosValue();
        updateFrontLeftObject();
        updateFrontObject();
        updateFrontRightObject();
        updateLeftLane();
        updateLeftRoad();
        updateLeftObject();
        updateRadarWave();
        updateRightLane();
        updateRightRoad();
        updateRightObject();
        updateADASViewFrontObstacleColorStat();
        updateAdasSummaryInfo();
        updateHandsOnOffTelltaleStat();
        updateADASViewLaneChangeRectangle();
        updateADASViewLaneChangeRectangleLatValue();
        updateADASViewLeftArrowLatValue();
        updateADASViewRightArrowLatValue();
        updateConstantADASViewFrontObjectColor01Stat();
        updateConstantADASViewFrontObjectClass01Stat();
        updateConstantADASViewFrontObjectLat01();
        updateConstantADASViewFrontObjectLong01();
        updateConstantADASViewFrontObjectHeadingAngle01();
        updateADASViewSurroundingObjectColor01();
        updateADASViewSurroundingObjectColor02();
        updateADASViewSurroundingObjectColor03();
        updateADASViewSurroundingObjectColor04();
        updateADASViewSurroundingObjectColor05();
        updateADASViewSurroundingObjectColor06();
        updateADASViewSurroundingObjectColor07();
        updateADASViewSurroundingObjectColor08();
        updateADASViewSurroundingObjectColor09();
        updateADASViewSurroundingObjectColor10();
        updateADASViewSurroundingObjectColor11();
        updateADASViewSurroundingObjectColor12();
        updateADASViewSurroundingObjectColor13();
        updateADASViewSurroundingObjectColor14();
        updateADASViewSurroundingObjectClass01();
        updateADASViewSurroundingObjectClass02();
        updateADASViewSurroundingObjectClass03();
        updateADASViewSurroundingObjectClass04();
        updateADASViewSurroundingObjectClass05();
        updateADASViewSurroundingObjectClass06();
        updateADASViewSurroundingObjectClass07();
        updateADASViewSurroundingObjectClass08();
        updateADASViewSurroundingObjectClass09();
        updateADASViewSurroundingObjectClass10();
        updateADASViewSurroundingObjectClass11();
        updateADASViewSurroundingObjectClass12();
        updateADASViewSurroundingObjectClass13();
        updateADASViewSurroundingObjectClass14();
        updateADASViewSurroundingObjectLat01();
        updateADASViewSurroundingObjectLat02();
        updateADASViewSurroundingObjectLat03();
        updateADASViewSurroundingObjectLat04();
        updateADASViewSurroundingObjectLat05();
        updateADASViewSurroundingObjectLat06();
        updateADASViewSurroundingObjectLat07();
        updateADASViewSurroundingObjectLat08();
        updateADASViewSurroundingObjectLat09();
        updateADASViewSurroundingObjectLat10();
        updateADASViewSurroundingObjectLat11();
        updateADASViewSurroundingObjectLat12();
        updateADASViewSurroundingObjectLat13();
        updateADASViewSurroundingObjectLat14();
        updateADASViewSurroundingObjectLong01();
        updateADASViewSurroundingObjectLong02();
        updateADASViewSurroundingObjectLong03();
        updateADASViewSurroundingObjectLong04();
        updateADASViewSurroundingObjectLong05();
        updateADASViewSurroundingObjectLong06();
        updateADASViewSurroundingObjectLong07();
        updateADASViewSurroundingObjectLong08();
        updateADASViewSurroundingObjectLong09();
        updateADASViewSurroundingObjectLong10();
        updateADASViewSurroundingObjectLong11();
        updateADASViewSurroundingObjectLong12();
        updateADASViewSurroundingObjectLong13();
        updateADASViewSurroundingObjectLong14();
        updateADASViewSurroundingObjectHeadingAngle01();
        updateADASViewSurroundingObjectHeadingAngle02();
        updateADASViewSurroundingObjectHeadingAngle03();
        updateADASViewSurroundingObjectHeadingAngle04();
        updateADASViewSurroundingObjectHeadingAngle05();
        updateADASViewSurroundingObjectHeadingAngle06();
        updateADASViewSurroundingObjectHeadingAngle07();
        updateADASViewSurroundingObjectHeadingAngle08();
        updateADASViewSurroundingObjectHeadingAngle09();
        updateADASViewSurroundingObjectHeadingAngle10();
        updateADASViewSurroundingObjectHeadingAngle11();
        updateADASViewSurroundingObjectHeadingAngle12();
        updateADASViewSurroundingObjectHeadingAngle13();
        updateADASViewSurroundingObjectHeadingAngle14();
        updateConstantAdasCruise();
        updateConstantLFATelltaleStat();
        updateHeadwayVehicleDistance();
        updateMslaEvent();
        updateCcEvent();
        updateAdasPopupEvent();
    }

    void onHudBcwUnderAdasInfoOnOffStatusChanged(const ArgumentsHudBcwUnderAdasInfoOnOffStatusChanged& args) {
        mHudBcwUnderAdasInfoOnOffStatus = args.mInter_HudBcwUnderAdasInfoOnOffStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewRearRightWarnStat();
        updateConstantADASViewRearLeftWarnStat();
    }

    void onCrawlWarnStatusChanged(const ArgumentsCrawlWarnStatusChanged& args) {
        mCrawlWarnStatus = args.mInput_CrawlWarnStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateCrawlWarnEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateCenterRoad();
        updateLaneCvrtLvlValue();
        updateRoadCvrtLvlValue();
        updateLeftLatPosValue();
        updateRightLatPosValue();
        updateFrontLeftObject();
        updateFrontObject();
        updateFrontRightObject();
        updateLeftLane();
        updateLeftRoad();
        updateLeftObject();
        updateRadarWave();
        updateRightLane();
        updateRightRoad();
        updateRightObject();
        updateADASViewFrontObstacleColorStat();
        updateADASViewFrontObstacleLatPosStat();
        updateADASViewFrontObstacleLongPosStat();
        updateAdasSummaryInfo();
        updateHandsOnOffTelltaleStat();
        updateADASViewLaneChangeRectangle();
        updateADASViewLaneChangeRectangleLatValue();
        updateADASViewLeftArrowLatValue();
        updateADASViewRightArrowLatValue();
        updateConstantADASViewFrontObjectColor01Stat();
        updateConstantADASViewFrontObjectClass01Stat();
        updateConstantADASViewFrontObjectLat01();
        updateConstantADASViewFrontObjectLong01();
        updateConstantADASViewFrontObjectHeadingAngle01();
        updateADASViewSurroundingObjectColor01();
        updateADASViewSurroundingObjectColor02();
        updateADASViewSurroundingObjectColor03();
        updateADASViewSurroundingObjectColor04();
        updateADASViewSurroundingObjectColor05();
        updateADASViewSurroundingObjectColor06();
        updateADASViewSurroundingObjectColor07();
        updateADASViewSurroundingObjectColor08();
        updateADASViewSurroundingObjectColor09();
        updateADASViewSurroundingObjectColor10();
        updateADASViewSurroundingObjectColor11();
        updateADASViewSurroundingObjectColor12();
        updateADASViewSurroundingObjectColor13();
        updateADASViewSurroundingObjectColor14();
        updateADASViewSurroundingObjectClass01();
        updateADASViewSurroundingObjectClass02();
        updateADASViewSurroundingObjectClass03();
        updateADASViewSurroundingObjectClass04();
        updateADASViewSurroundingObjectClass05();
        updateADASViewSurroundingObjectClass06();
        updateADASViewSurroundingObjectClass07();
        updateADASViewSurroundingObjectClass08();
        updateADASViewSurroundingObjectClass09();
        updateADASViewSurroundingObjectClass10();
        updateADASViewSurroundingObjectClass11();
        updateADASViewSurroundingObjectClass12();
        updateADASViewSurroundingObjectClass13();
        updateADASViewSurroundingObjectClass14();
        updateADASViewSurroundingObjectLat01();
        updateADASViewSurroundingObjectLat02();
        updateADASViewSurroundingObjectLat03();
        updateADASViewSurroundingObjectLat04();
        updateADASViewSurroundingObjectLat05();
        updateADASViewSurroundingObjectLat06();
        updateADASViewSurroundingObjectLat07();
        updateADASViewSurroundingObjectLat08();
        updateADASViewSurroundingObjectLat09();
        updateADASViewSurroundingObjectLat10();
        updateADASViewSurroundingObjectLat11();
        updateADASViewSurroundingObjectLat12();
        updateADASViewSurroundingObjectLat13();
        updateADASViewSurroundingObjectLat14();
        updateADASViewSurroundingObjectLong01();
        updateADASViewSurroundingObjectLong02();
        updateADASViewSurroundingObjectLong03();
        updateADASViewSurroundingObjectLong04();
        updateADASViewSurroundingObjectLong05();
        updateADASViewSurroundingObjectLong06();
        updateADASViewSurroundingObjectLong07();
        updateADASViewSurroundingObjectLong08();
        updateADASViewSurroundingObjectLong09();
        updateADASViewSurroundingObjectLong10();
        updateADASViewSurroundingObjectLong11();
        updateADASViewSurroundingObjectLong12();
        updateADASViewSurroundingObjectLong13();
        updateADASViewSurroundingObjectLong14();
        updateADASViewSurroundingObjectHeadingAngle01();
        updateADASViewSurroundingObjectHeadingAngle02();
        updateADASViewSurroundingObjectHeadingAngle03();
        updateADASViewSurroundingObjectHeadingAngle04();
        updateADASViewSurroundingObjectHeadingAngle05();
        updateADASViewSurroundingObjectHeadingAngle06();
        updateADASViewSurroundingObjectHeadingAngle07();
        updateADASViewSurroundingObjectHeadingAngle08();
        updateADASViewSurroundingObjectHeadingAngle09();
        updateADASViewSurroundingObjectHeadingAngle10();
        updateADASViewSurroundingObjectHeadingAngle11();
        updateADASViewSurroundingObjectHeadingAngle12();
        updateADASViewSurroundingObjectHeadingAngle13();
        updateADASViewSurroundingObjectHeadingAngle14();
        updateConstantLFATelltaleStat();
        updateHeadwayVehicleDistance();
        updateCcMslaSpeed();
        updateMslaEvent();
        updateCcEvent();
        updateConstantNaviCruiseStat();
        updateCrawlOperationStatus();
        updateCrawlOperationStepStatus();
        updateConstantAdasCruise();
        updateCrawlSummaryEvent();
        updateAdasPopupEvent();
        updateHdpPopups();
        updateCrawlWarnEvent();
    }

    void updateAdasPopupEvent() {
        updateAdasPopupEventHudSccInfo();
        update1Group1_1Event();
        update1Group3_1Event_E71799();
        update1Group3_1Event_E71800();
        update1Group3_1Event_E71801();
    }

    void updateAdasPopupEventHudSccInfo() {
        update1Group1_2Event();
        update4Group1_1Event();
        update2Group1_1Event();
        update2Group1_2Event();
        update2Group1_3Event();
    }

    // 5.10.4.1 동력 타입 별 표시 신호 통합
    void updatePrivateCcSldSpeedValue() {
        HUInt64 ccSldSetSpeed = mCcSldSetSpeedValue;

        if (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
            ccSldSetSpeed = mCcOnlySetSpeedValue;
        }

        mPrivateCcSldSetSpeedValue = ccSldSetSpeed;
    }

    // 5.10.5.1.1.1.1 CC 심볼
    void updateCcSymbol() {
        SFCDetail_Contents_ADAS_NEWConstantCcColorStat stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::DISPLAY_OFF;
        SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueA blinkA =
            SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueA::NONE;
        SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueB blinkB =
            SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueB::NONE;

        if (mIsIgnOn == true && mCcOnOffStat == CcOnOffStat::ON) {
            if (mCruiseSetIndReqStatus == CruiseSetIndReqStatus::ON) {
                switch (mCcSpeedStat) {
                    case CcSpeedStat::DISPLAY_OFF:
                        stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::CRUISE_ACTIVE_FULL;
                        break;
                    case CcSpeedStat::BLINK2:
                        stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueA::CRUISE_ACTIVE_HALF;
                        blinkB = SFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueB::DISPLAY_OFF;
                        break;
                    case CcSpeedStat::ACTIVATE:
                        stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::CRUISE_ACTIVE_HALF;
                        break;
                    default:
                        break;
                }
            } else if (mCruiseSetIndReqStatus == CruiseSetIndReqStatus::OFF) {
                switch (mCcSpeedStat) {
                    case CcSpeedStat::DISPLAY_OFF_DEACTIVATE:
                    case CcSpeedStat::DISPLAY_OFF:
                        stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::CRUISE_PAUSE_FULL;
                        break;
                    case CcSpeedStat::DEACTIVATE:
                        stat = SFCDetail_Contents_ADAS_NEWConstantCcColorStat::CRUISE_PAUSE_HALF;
                        break;
                    default:
                        break;
                }
            } else {
                // DISPLAY_OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantCcColorStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueA(blinkA);
        setSFCDetail_Contents_ADAS_NEWConstantCcColorBlinkValueB(blinkB);
    }

    // 5.10.5.1.1.1.2 MSLA 심볼
    void updateMslaSymbol() {
        SFCDetail_Contents_ADAS_NEWConstantMslaColorStat stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::DISPLAY_OFF;
        SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueA blinkA =
            SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueA::NONE;
        SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueB blinkB =
            SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueB::NONE;

        if (mIsIgnOn == true) {
            if (mMslaOnOffStat == MslaOnOffStat::ON) {
                if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::NORMAL) {
                    switch (mMslaSpeedStat) {
                        case MslaSpeedStat::DISPLAY_OFF:
                            stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::CRUISE_ACTIVE_FULL;
                            break;
                        case MslaSpeedStat::BLINK2:
                            stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::BLINK2;
                            blinkA = SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueA::CRUISE_ACTIVE_HALF;
                            blinkB = SFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueB::DISPLAY_OFF;
                            break;
                        case MslaSpeedStat::ACTIVATE:
                        case MslaSpeedStat::ISLA_ON:
                            stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::CRUISE_ACTIVE_HALF;
                            break;
                        default:
                            break;
                    }
                } else if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::READY) {
                    switch (mMslaSpeedStat) {
                        case MslaSpeedStat::DISPLAY_OFF_DEACTIVATE:
                        case MslaSpeedStat::DISPLAY_OFF:
                            stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::CRUISE_PAUSE_FULL;
                            break;
                        case MslaSpeedStat::DEACTIVATE:
                            stat = SFCDetail_Contents_ADAS_NEWConstantMslaColorStat::CRUISE_PAUSE_HALF;
                            break;
                        default:
                            break;
                    }
                } else {
                    // DISPLAY OFF
                }
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantMslaColorStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueA(blinkA);
        setSFCDetail_Contents_ADAS_NEWConstantMslaColorBlinkValueB(blinkB);
    }

    // 5.10.5.1.1.1.3 NCC 표시
    void updateConstantNaviCruiseStat(HBool enterFromTimerCallback = false) {
        if (mOneShotTimerSummaryEventExpired.isRunning() == true && enterFromTimerCallback == false) {
            return;
        }

        SFCDetail_Contents_ADAS_NEWConstantNaviCruiseStat naviCruiseStat =
            SFCDetail_Contents_ADAS_NEWConstantNaviCruiseStat::DISPLAY_OFF;
        if (mIsIgnOn == true) {
            if (mNccCameraOperStatus == NccCameraOperStatus::READY) {
                naviCruiseStat = SFCDetail_Contents_ADAS_NEWConstantNaviCruiseStat::READY;
            } else if (mNccCameraOperStatus == NccCameraOperStatus::ACT) {
                naviCruiseStat = SFCDetail_Contents_ADAS_NEWConstantNaviCruiseStat::ACTIVE;
            } else {
                // DISPLAY_OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantNaviCruiseStat(naviCruiseStat);
    }

    // 5.10.5.1.1.1.4 CRAWL 표시
    // 5.10.5.1.1.1.4.1 CRAWL 동작 상태 표시 status
    void updateCrawlOperationStatus() {
        SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::OFF;
        SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA blinkValueA =
            SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA::OFF;

        if (mIsIgnOn == true) {
            if (isCrawlActivationStep() == true) {
                if (mCrawlActiveStatus == CrawlActiveStatus::READY) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::READY;
                } else if (mCrawlActiveStatus == CrawlActiveStatus::ACTIVE) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::ACTIVE;
                } else if (mCrawlActiveStatus == CrawlActiveStatus::OVERRIDE) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA::WHITE;
                } else if (mCrawlActiveStatus == CrawlActiveStatus::PAUSE) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA::ACTIVE;
                } else if (mCrawlActiveStatus == CrawlActiveStatus::TEMP) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA::YELLOW;
                } else {
                    // OFF
                }
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantCrawlOperationStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueA(blinkValueA);
        setSFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueB(
            SFCDetail_Contents_ADAS_NEWConstantCrawlOperationBlinkValueB::OFF);
    }

    // 5.10.5.1.1.1.4.1 CRAWL 동작 상태 표시 step
    void updateCrawlOperationStepStatus() {
        SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat stat =
            SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::OFF;

        if (mIsIgnOn == true) {
            if (isCrawlActivationStatus() == true) {
                if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP1) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::STEP1;
                } else if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP2) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::STEP2;
                } else if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP3) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::STEP3;
                } else if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP4) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::STEP4;
                } else if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP5) {
                    stat = SFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat::STEP5;
                } else {
                    // OFF
                }
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantCrawlOperationStepStat(stat);
    }

    // 5.10.5.1.1.1.5 속도 표시
    void updateCcMslaSpeed(HBool enterFromTimerCallback = false) {
        if (mOneShotTimerSummaryEventExpired.isRunning() == true && enterFromTimerCallback == false) {
            return;
        }

        updateCcSpeedStat();
        updateMslaSpeedStat();
    }

    // 5.10.5.1.1.1.5.2 CC 속도 표시(값)
    void updateCcSpeedValue() {
        if (mIsConfigOn != true) {
            return;
        }
        if (mOneShotTimerSummaryEventExpired.isRunning() == true) {
            return;
        }

        if (mIsIgnOn && mCcSpeedStat != SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::DISPLAY_OFF) {
            setSFCDetail_Contents_ADAS_NEWConstantCcSpeedValue(mPrivateCcSldSetSpeedValue);
        } else {
            setSFCDetail_Contents_ADAS_NEWConstantCcSpeedValue(0);
        }
    }

    // 5.10.5.1.1.1.5.2 MSLA 속도 표시(값)
    void updateMslaSpeedValue() {
        if (mIsConfigOn != true) {
            return;
        }
        if (mOneShotTimerSummaryEventExpired.isRunning() == true) {
            return;
        }

        if (mIsIgnOn == true && mMslaSpeedStat != SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::DISPLAY_OFF) {
            setSFCDetail_Contents_ADAS_NEWConstantMslaSpeedValue(mCcSldSetSpeedValue);
        } else {
            setSFCDetail_Contents_ADAS_NEWConstantMslaSpeedValue(0);
        }
    }

    // 5.10.5.1.1.1.5.1 속도 표시(상태)
    void updateCcSpeedStat() {
        if (mIsConfigOn != true) {
            return;
        }
        if (mOneShotTimerSummaryEventExpired.isRunning() == true) {
            return;
        }

        SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat resState = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::DISPLAY_OFF;
        SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueA resBlinkA =
            SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueA::NONE;
        SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueB resBlinkB =
            SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueB::NONE;

        if (mIsIgnOn == true && mPrivateCcSldSetSpeedValue != kTimeOut && mCcOnOffStat == CcOnOffStat::ON) {
            if (mCruiseSetIndReqStatus == CruiseSetIndReqStatus::ON &&
                (mCcActiveStat == CcActiveStat::ON && mCruiseOverrideReqStatus == CruiseOverrideReqStatus::OVERRIDE)) {
                if (1 <= mPrivateCcSldSetSpeedValue && mPrivateCcSldSetSpeedValue <= mCcSldMaxSpeedValue) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::BLINK2;
                    resBlinkA = SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueA::ACTIVATE;
                    resBlinkB = SFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueB::DISPLAY_OFF;
                } else {
                    // DISPLAY_OFF
                }
            } else if (1 <= mPrivateCcSldSetSpeedValue && mPrivateCcSldSetSpeedValue <= mCcSldMaxSpeedValue &&
                       mCruiseSetIndReqStatus == CruiseSetIndReqStatus::ON &&
                       mCruiseOverrideReqStatus == CruiseOverrideReqStatus::OFF) {
                resState = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::ACTIVATE;
            } else if (mCruiseSetIndReqStatus == CruiseSetIndReqStatus::OFF) {
                if (mPrivateCcSldSetSpeedValue == 0 || (mPrivateCcSldSetSpeedValue == (mCcSldMaxSpeedValue + 1))) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::DISPLAY_OFF_DEACTIVATE;
                } else if (1 <= mPrivateCcSldSetSpeedValue && mPrivateCcSldSetSpeedValue <= mCcSldMaxSpeedValue) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat::DEACTIVATE;
                } else {
                    // DISPLAY_OFF
                }
            } else {
                // DISPLAY_OFF
            }
        }
        mCcSpeedStat = resState;
        setSFCDetail_Contents_ADAS_NEWConstantCcSpeedStat(mCcSpeedStat);
        setSFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueA(resBlinkA);
        setSFCDetail_Contents_ADAS_NEWConstantCcSpeedBlinkValueB(resBlinkB);

        updateCcSpeedValue();
        updateCcSymbol();
    }

    // 5.10.5.1.1.1.5.1 MSLA 속도 표시(상태)
    void updateMslaSpeedStat() {
        if (mIsConfigOn != true) {
            return;
        }
        if (mOneShotTimerSummaryEventExpired.isRunning() == true) {
            return;
        }

        SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::DISPLAY_OFF;
        SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueA resBlinkA =
            SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueA::NONE;
        SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueB resBlinkB =
            SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueB::NONE;

        if (mIsIgnOn == true && mCcSldSetSpeedValue != kTimeOut && mMslaOnOffStat == MslaOnOffStat::ON) {
            if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::NORMAL &&
                (mMslaActiveStat == MslaActiveStat::ON && mMslaOverride == MslaOverride::ON)) {
                if (1 <= mCcSldSetSpeedValue && mCcSldSetSpeedValue <= mCcSldMaxSpeedValue) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::BLINK2;
                    resBlinkA = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueA::ACTIVATE;
                    resBlinkB = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueB::DISPLAY_OFF;
                } else {
                    // Impossible
                }
            } else if (1 <= mCcSldSetSpeedValue && mCcSldSetSpeedValue <= mCcSldMaxSpeedValue &&
                       mSpeedLimitReadyStatus == SpeedLimitReadyStatus::NORMAL && mMslaOverride == MslaOverride::OFF) {
                if (mSmvSetSpeedStatus != SmvSetSpeedStatus::GREEN ||
                    mSmvSetSpeedStatus == SmvSetSpeedStatus::UNHANDLED_TIMEOUT) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::ACTIVATE;
                } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::ISLA_ON;
                } else {
                    // Impossible
                }
            } else if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::READY) {
                if (mCcSldSetSpeedValue == 0 || (mCcSldSetSpeedValue == (mCcSldMaxSpeedValue + 1))) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::DISPLAY_OFF_DEACTIVATE;
                } else if (1 <= mCcSldSetSpeedValue && mCcSldSetSpeedValue <= mCcSldMaxSpeedValue) {
                    resState = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat::DEACTIVATE;
                } else {
                    // Impossible
                }
            } else {
                // DISPLAY_OFF
            }
        }
        mMslaSpeedStat = resState;
        setSFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat(mMslaSpeedStat);
        setSFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueA(resBlinkA);
        setSFCDetail_Contents_ADAS_NEWConstantMslaSpeedBlinkValueB(resBlinkB);

        updateMslaSymbol();
        updateMslaSpeedValue();
    }

    // 5.10.5.1.1.1.6 설정속도 조절 심볼
    void updateIslaManualSetSpeedForCcMslaStat() {
        SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat islaManualSetSpeedForCcMslaStat =
            SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::OFF;
        HBool ccMsla = ((mConfigAdasScc == ConfigAdasScc::OFF && mCruiseMainIndReqStatus == CruiseMainIndReqStatus::ON) ||
                        mSpeedLimitActiveStatus == SpeedLimitActiveStatus::ON);

        if (mIsIgnOn == true && ccMsla == true) {
            if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_WHITE) {
                islaManualSetSpeedForCcMslaStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::PLUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_WHITE) {
                islaManualSetSpeedForCcMslaStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::MINUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_GREEN) {
                islaManualSetSpeedForCcMslaStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::PLUS_GREEN;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_GREEN) {
                islaManualSetSpeedForCcMslaStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::MINUS_GREEN;
            } else {
                // OFF
            }
        }

        if (islaManualSetSpeedForCcMslaStat == SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat::OFF) {
            mIsIslaManualSetSpeedForCcMslaStatOn = false;
        } else {
            mIsIslaManualSetSpeedForCcMslaStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedForCcMslaStat(islaManualSetSpeedForCcMslaStat);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.1.2 주행 보조 요약
    void updateAdasSummaryInfo(HBool enterFromTimerCallback = false) {
        if (mOneShotTimerSummaryEventExpired.isRunning() == true && enterFromTimerCallback == false) {
            return;
        }

        updateConstantAdasSetSpeed();
        updateHdaHdpTelltale();
        updateNsccAutoSetTelltale();
        updateIslaManualSetSpeedStat();
        updateIslaManualSetSpeedForCcMslaStat();
    }

    // 5.10.5.2 이벤트 표시 동작 로직
    // 5.10.5.2.1 ADAS 요약 정보 표시
    void updateEventCcMslaSummary() {
        if (mIsIgnOn == true) {
            if (mConfigAdasScc == ConfigAdasScc::OFF && mCruiseMainIndReqStatus == CruiseMainIndReqStatus::OFF &&
                mPrevCruiseMainIndReqStatus == CruiseMainIndReqStatus::ON) {
                if (mOneShotTimerSummaryEventExpired.restart() == false) {
                    DWarning() << "[Detail_Contents_ADAS_NEW] mOneShotTimerSummaryEventExpired Restart Failed";
                }
            } else if (mSpeedLimitActiveStatus == SpeedLimitActiveStatus::OFF &&
                       mPrevSpeedLimitActiveStatus == SpeedLimitActiveStatus::ON) {
                if (mOneShotTimerSummaryEventExpired.restart() == false) {
                    DWarning() << "[Detail_Contents_ADAS_NEW] mOneShotTimerSummaryEventExpired Restart Failed";
                }
            } else {
                mOneShotTimerSummaryEventExpired.stop();
                updateCcMslaSpeed();
                updateConstantNaviCruiseStat();
                updateAdasSummaryInfo();
            }
            mPrevSpeedLimitActiveStatus = mSpeedLimitActiveStatus;
            mPrevCruiseMainIndReqStatus = mCruiseMainIndReqStatus;
        }

        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.2 이벤트 표시 동작 로직
    // 5.10.5.2.1 ADAS 요약 정보 표시
    void updateEventCcMslaSummaryStat() {
        SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat summaryStat = SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat::OFF;
        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mSpeedLimitActiveStatus == SpeedLimitActiveStatus::ON) {
                summaryStat = SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat::ON;
            } else if (mConfigAdasScc == ConfigAdasScc::OFF && mCruiseMainIndReqStatus == CruiseMainIndReqStatus::ON) {
                summaryStat = SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat::ON;
            } else if (isAdasSummaryOn() == true) {
                summaryStat = SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat::ON;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat(summaryStat);
    }

    // 5.10.5.1.1.2.1 CRUISE 심볼 (1-1)
    void updateConstantAdasCruise() {
        SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvDrvAsstHUDSymbStatus == SmvDrvAsstHUDSymbStatus::GRAY) {
                if (1 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::GRAY_HALF;
                } else {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::GRAY_FULL;
                }
            } else if (mSmvDrvAsstHUDSymbStatus == SmvDrvAsstHUDSymbStatus::GREEN) {
                if (1 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::GREEN_HALF;
                } else {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::GREEN_FULL;
                }
            } else if (mSmvDrvAsstHUDSymbStatus == SmvDrvAsstHUDSymbStatus::WHITE) {
                if (1 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::WHITE_HALF;
                } else {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::WHITE_FULL;
                }
            } else if (mSmvDrvAsstHUDSymbStatus == SmvDrvAsstHUDSymbStatus::CYAN) {
                if (1 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 254) {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::CYAN_HALF;
                } else {
                    adasCruiseStat = SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::CYAN_FULL;
                }
            } else {
                // OFF
            }
        }

        if (adasCruiseStat == SFCDetail_Contents_ADAS_NEWConstantADASCruiseStat::OFF) {
            mIsAdasCruiseStatOn = false;
        } else {
            mIsAdasCruiseStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASCruiseStat(adasCruiseStat);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.1.2.2 설정 속도 (1-2)
    // 5.10.5.1.1.2.2.1 칼라
    // 5.10.5.1.1.2.2.2 속도값
    void updateConstantAdasSetSpeed() {
        SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat adasSetSpeedStat =
            SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::OFF;
        SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedValueStat adasSetSpeedValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedValueStat::OFF;
        HUInt64 adasSetSpeedValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GRAY) {
                adasSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::GRAY;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN) {
                adasSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::GREEN;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                adasSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::WHITE;
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::CYAN) {
                adasSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::CYAN;
            } else {
                // OFF
            }

            if (mSmvSetSpeedValue >= 1 && mSmvSetSpeedValue <= 254) {
                adasSetSpeedValueStat = SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedValueStat::VALUE;
                adasSetSpeedValue = mSmvSetSpeedValue;
            }
        }

        if (adasSetSpeedStat == SFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat::OFF) {
            mIsAdasSetSpeedStatOn = false;
        } else {
            mIsAdasSetSpeedStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASSetSpeedStat(adasSetSpeedStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASSetSpeedValueStat(adasSetSpeedValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASSetSpeedValue(adasSetSpeedValue);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.1.2.3 HDA/HDP 심볼 (고속도로 주행 보조 및 고속도로 자율주행 심볼) (1-3)
    void updateHdaHdpTelltale() {
        SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat hdaHdpTelltaleStat =
            SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GRAY) {
                hdaHdpTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::GRAY;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::GREEN) {
                hdaHdpTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::GREEN;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::WHITE) {
                hdaHdpTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::WHITE;
            } else if (mSmvHdaSymbolStatus == SmvHdaSymbolStatus::CYAN) {
                if (mConfigHDPName == ConfigHDPName::TYPE_A) {
                    hdaHdpTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::CYAN;
                } else if (mConfigHDPName == ConfigHDPName::TYPE_B) {
                    hdaHdpTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::CYAN_B;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }

        if (hdaHdpTelltaleStat == SFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat::OFF) {
            mIsHdaHdpTelltaleStatOn = false;
        } else {
            mIsHdaHdpTelltaleStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantHDAHDPTelltaleStat(hdaHdpTelltaleStat);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.2.22 HDP 상품명 구분 사양
    void updateHdpPopups() {
        if (mConfigHDPName == ConfigHDPName::TYPE_B && mIsIgnOn == true) {
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71770Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71770Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71774Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71774Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71776Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71776Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71800Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71800Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71801Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71801Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71739Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71739Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71740Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71740Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71741Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71741Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71742Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71742Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71747Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71747Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72523Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72523Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72524Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72524Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72534Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72534Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72527Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72527Value::TYPE_B);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72616Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72616Value::TYPE_B);
        } else {
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71770Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71770Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71774Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71774Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71776Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71776Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71800Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71800Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71801Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71801Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71739Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71739Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71740Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71740Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71741Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71741Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71742Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71742Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E71747Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E71747Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72523Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72523Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72524Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72524Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72534Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72534Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72527Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72527Value::TYPE_A);
            setSFCDetail_Contents_ADAS_NEWConstantHDP_E72616Value(SFCDetail_Contents_ADAS_NEWConstantHDP_E72616Value::TYPE_A);
        }
    }

    // 5.10.5.1.1.2.4 NSCC 자동 속도 조절 심볼 (1-4)
    void updateNsccAutoSetTelltale() {
        SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat nsccAutoSetTelltaleStat =
            SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::GRAY) {
                nsccAutoSetTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat::GRAY;
            } else if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::GREEN) {
                nsccAutoSetTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat::GREEN;
            } else if (mSmvNsccSymbolStatus == SmvNsccSymbolStatus::WHITE) {
                nsccAutoSetTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat::WHITE;
            } else {
                // OFF
            }
        }

        if (nsccAutoSetTelltaleStat == SFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat::OFF) {
            mIsNsccAutoSetTelltaleStatOn = false;
        } else {
            mIsNsccAutoSetTelltaleStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantNSCCAutoSetTelltaleStat(nsccAutoSetTelltaleStat);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.1.2.5 설정속도 조절 심볼 (1-5)
    void updateIslaManualSetSpeedStat() {
        SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat islaManualSetSpeedStat =
            SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_WHITE) {
                islaManualSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::PLUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_WHITE) {
                islaManualSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::MINUS_WHITE;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::PLUS_GREEN) {
                islaManualSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::PLUS_GREEN;
            } else if (mSmvIslaSetSpeedSymbolStatus == SmvIslaSetSpeedSymbolStatus::MINUS_GREEN) {
                islaManualSetSpeedStat = SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::MINUS_GREEN;
            } else {
                // OFF
            }
        }

        if (islaManualSetSpeedStat == SFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat::OFF) {
            mIsIslaManualSetSpeedStatOn = false;
        } else {
            mIsIslaManualSetSpeedStatOn = true;
        }

        setSFCDetail_Contents_ADAS_NEWConstantISLAManualSetSpeedStat(islaManualSetSpeedStat);
        updateEventCcMslaSummaryStat();
    }

    // 5.10.5.1.1.2.6 HDA3 Hands Free/On 심볼
    void updateHandsOnOffTelltaleStat() {
        SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat handsOnOffStat =
            SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_FREE_CYAN) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::FREE_CYAN;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_FREE_GREEN) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::FREE_GREEN;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_ON_CYAN) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::ON_CYAN;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_ON_GRAY) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::ON_GRAY;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_ON_AMBER) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::ON_AMBER;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_ON_RED) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::ON_RED;
            } else if (mMvDriverHandsOnOffStatus == MvDriverHandsOnOffStatus::HANDS_ON_WHITE) {
                handsOnOffStat = SFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat::ON_WHITE;
            } else {
                // OFF
            }
        } else {
            // OFF
        }

        setSFCDetail_Contents_ADAS_NEWConstantHandsOnOffTelltaleStat(handsOnOffStat);
    }

    // 5.10.5.1.2.20 차로 변경 방향 보존/제안 (3-6)
    // 5.10.5.1.2.20.1 보조/제안
    void updateADASViewLaneChangeRectangle() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLaneChangeRectangleStatus == MvLaneChangeRectangleStatus::DOTTED_LINE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleStat::DOTTED_LINE;
            } else if (mMvLaneChangeRectangleStatus == MvLaneChangeRectangleStatus::SOLID_LINE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleStat::SOLID_LINE;
            } else {
                // OFF
            }
        } else {
            // OFF
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleStat(stat);
    }

    // 5.10.5.1.2.20.2 횡위치
    void updateADASViewLaneChangeRectangleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat::CENTER;
                value = 0;
            } else if (1 <= mMvLaneChangeRectangleLateralPositionValue && mMvLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat::RIGHT;
                value = mMvLaneChangeRectangleLateralPositionValue;
            } else if (0x42 <= mMvLaneChangeRectangleLateralPositionValue && mMvLaneChangeRectangleLateralPositionValue <= 0x7f) {
                // 0x7f -> 1, 0x42 -> 62
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat::LEFT;
                value = (mMvLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValueStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewLaneChangeRectangleLatValue(value);
    }

    // 5.10.5.1.2.13 전방 물체 확장 (2-7)
    // 5.10.5.1.2.13.1 칼라
    void updateConstantADASViewFrontObjectColor01Stat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectColor01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectColor01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObject01Status == MvFrontObject01Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectColor01Stat::DARK;
            } else if (mMvFrontObject01Status == MvFrontObject01Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectColor01Stat::BRIGHT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectColor01Stat(stat);
    }

    // 5.10.5.1.2.13.2 타입
    void updateConstantADASViewFrontObjectClass01Stat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::UNKNOWN;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::PASSENGER_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::PEDESTRIAN;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::CYCLIST;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::RUBBER_CONE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::EMERGENCY_VEHICLE;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::ANIMAL;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::TUBULAR_MARKER;
            } else if (mMvFrontObjectClassification01Status == MvFrontObjectClassification01Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectClass01Stat(stat);
    }

    // 5.10.5.1.2.13.3 횡위치
    void updateConstantADASViewFrontObjectLat01() {
        HInt64 value = 0;
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectLateralPosition01Value >= 0 && mMvFrontObjectLateralPosition01Value <= 300) {
                value = mMvFrontObjectLateralPosition01Value;
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Stat::LEFT;
            } else if (mMvFrontObjectLateralPosition01Value >= 1024 && mMvFrontObjectLateralPosition01Value <= 1323) {
                value = 1023 - mMvFrontObjectLateralPosition01Value;  // 1024~1323 : -1~-300
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Stat::RIGHT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Value(value);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLat01Stat(stat);
    }

    // 5.10.5.1.2.13.4 종위치
    void updateConstantADASViewFrontObjectLong01() {
        HInt64 value = 0;
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectLongitudinalPosition01Value >= 0 && mMvFrontObjectLongitudinalPosition01Value <= 1500) {
                value = mMvFrontObjectLongitudinalPosition01Value;
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Stat::VALUE;
            } else if (mMvFrontObjectLongitudinalPosition01Value >= 1846 && mMvFrontObjectLongitudinalPosition01Value <= 2045) {
                value = mMvFrontObjectLongitudinalPosition01Value - 2046;
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Stat::VALUE;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Value(value);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectLong01Stat(stat);
    }

    // 5.10.5.1.2.13.5 방향
    void updateConstantADASViewFrontObjectHeadingAngle01() {
        HUInt64 value = 0;
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectHeadingAngle01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectHeadingAngle01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectHeadingAngle01Value >= 0 && mMvFrontObjectHeadingAngle01Value <= 72) {
                value = mMvFrontObjectHeadingAngle01Value;
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectHeadingAngle01Stat::VALUE;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectHeadingAngle01Value(value);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObjectHeadingAngle01Stat(stat);
    }

    // 5.10.5.1.2.14 주변 물체-확장 (2-8)
    // 5.10.5.1.2.14.1 칼라
    void updateADASViewSurroundingObjectColor01() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject01Status != MvSurroundingObject01Status::NO_DISPLAY &&
                mMvSurroundingObject01Status == MvSurroundingObject01Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat::DARK;
            } else if (mMvSurroundingObject01Status == MvSurroundingObject01Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject01Status == MvSurroundingObject01Status::NO_DISPLAY &&
                       mMvSurroundingObject01Status == MvSurroundingObject01Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject01Status == MvSurroundingObject01Status::DARK &&
                       mMvSurroundingObject01Status == MvSurroundingObject01Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor01Stat(stat);
    }

    void updateADASViewSurroundingObjectColor02() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject02Status != MvSurroundingObject02Status::NO_DISPLAY &&
                mMvSurroundingObject02Status == MvSurroundingObject02Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat::DARK;
            } else if (mMvSurroundingObject02Status == MvSurroundingObject02Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject02Status == MvSurroundingObject02Status::NO_DISPLAY &&
                       mMvSurroundingObject02Status == MvSurroundingObject02Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject02Status == MvSurroundingObject02Status::DARK &&
                       mMvSurroundingObject02Status == MvSurroundingObject02Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor02Stat(stat);
    }

    void updateADASViewSurroundingObjectColor03() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject03Status != MvSurroundingObject03Status::NO_DISPLAY &&
                mMvSurroundingObject03Status == MvSurroundingObject03Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat::DARK;
            } else if (mMvSurroundingObject03Status == MvSurroundingObject03Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject03Status == MvSurroundingObject03Status::NO_DISPLAY &&
                       mMvSurroundingObject03Status == MvSurroundingObject03Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject03Status == MvSurroundingObject03Status::DARK &&
                       mMvSurroundingObject03Status == MvSurroundingObject03Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor03Stat(stat);
    }

    void updateADASViewSurroundingObjectColor04() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject04Status != MvSurroundingObject04Status::NO_DISPLAY &&
                mMvSurroundingObject04Status == MvSurroundingObject04Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat::DARK;
            } else if (mMvSurroundingObject04Status == MvSurroundingObject04Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject04Status == MvSurroundingObject04Status::NO_DISPLAY &&
                       mMvSurroundingObject04Status == MvSurroundingObject04Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject04Status == MvSurroundingObject04Status::DARK &&
                       mMvSurroundingObject04Status == MvSurroundingObject04Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor04Stat(stat);
    }

    void updateADASViewSurroundingObjectColor05() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject05Status != MvSurroundingObject05Status::NO_DISPLAY &&
                mMvSurroundingObject05Status == MvSurroundingObject05Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat::DARK;
            } else if (mMvSurroundingObject05Status == MvSurroundingObject05Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject05Status == MvSurroundingObject05Status::NO_DISPLAY &&
                       mMvSurroundingObject05Status == MvSurroundingObject05Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject05Status == MvSurroundingObject05Status::DARK &&
                       mMvSurroundingObject05Status == MvSurroundingObject05Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor05Stat(stat);
    }

    void updateADASViewSurroundingObjectColor06() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject06Status != MvSurroundingObject06Status::NO_DISPLAY &&
                mMvSurroundingObject06Status == MvSurroundingObject06Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat::DARK;
            } else if (mMvSurroundingObject06Status == MvSurroundingObject06Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject06Status == MvSurroundingObject06Status::NO_DISPLAY &&
                       mMvSurroundingObject06Status == MvSurroundingObject06Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject06Status == MvSurroundingObject06Status::DARK &&
                       mMvSurroundingObject06Status == MvSurroundingObject06Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor06Stat(stat);
    }

    void updateADASViewSurroundingObjectColor07() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject07Status != MvSurroundingObject07Status::NO_DISPLAY &&
                mMvSurroundingObject07Status == MvSurroundingObject07Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat::DARK;
            } else if (mMvSurroundingObject07Status == MvSurroundingObject07Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject07Status == MvSurroundingObject07Status::NO_DISPLAY &&
                       mMvSurroundingObject07Status == MvSurroundingObject07Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject07Status == MvSurroundingObject07Status::DARK &&
                       mMvSurroundingObject07Status == MvSurroundingObject07Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor07Stat(stat);
    }

    void updateADASViewSurroundingObjectColor08() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject08Status != MvSurroundingObject08Status::NO_DISPLAY &&
                mMvSurroundingObject08Status == MvSurroundingObject08Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat::DARK;
            } else if (mMvSurroundingObject08Status == MvSurroundingObject08Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject08Status == MvSurroundingObject08Status::NO_DISPLAY &&
                       mMvSurroundingObject08Status == MvSurroundingObject08Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject08Status == MvSurroundingObject08Status::DARK &&
                       mMvSurroundingObject08Status == MvSurroundingObject08Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor08Stat(stat);
    }

    void updateADASViewSurroundingObjectColor09() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject09Status != MvSurroundingObject09Status::NO_DISPLAY &&
                mMvSurroundingObject09Status == MvSurroundingObject09Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat::DARK;
            } else if (mMvSurroundingObject09Status == MvSurroundingObject09Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject09Status == MvSurroundingObject09Status::NO_DISPLAY &&
                       mMvSurroundingObject09Status == MvSurroundingObject09Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject09Status == MvSurroundingObject09Status::DARK &&
                       mMvSurroundingObject09Status == MvSurroundingObject09Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor09Stat(stat);
    }

    void updateADASViewSurroundingObjectColor10() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject10Status != MvSurroundingObject10Status::NO_DISPLAY &&
                mMvSurroundingObject10Status == MvSurroundingObject10Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat::DARK;
            } else if (mMvSurroundingObject10Status == MvSurroundingObject10Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject10Status == MvSurroundingObject10Status::NO_DISPLAY &&
                       mMvSurroundingObject10Status == MvSurroundingObject10Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject10Status == MvSurroundingObject10Status::DARK &&
                       mMvSurroundingObject10Status == MvSurroundingObject10Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor10Stat(stat);
    }

    void updateADASViewSurroundingObjectColor11() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject11Status != MvSurroundingObject11Status::NO_DISPLAY &&
                mMvSurroundingObject11Status == MvSurroundingObject11Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat::DARK;
            } else if (mMvSurroundingObject11Status == MvSurroundingObject11Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject11Status == MvSurroundingObject11Status::NO_DISPLAY &&
                       mMvSurroundingObject11Status == MvSurroundingObject11Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject11Status == MvSurroundingObject11Status::DARK &&
                       mMvSurroundingObject11Status == MvSurroundingObject11Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor11Stat(stat);
    }

    void updateADASViewSurroundingObjectColor12() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject12Status != MvSurroundingObject12Status::NO_DISPLAY &&
                mMvSurroundingObject12Status == MvSurroundingObject12Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat::DARK;
            } else if (mMvSurroundingObject12Status == MvSurroundingObject12Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject12Status == MvSurroundingObject12Status::NO_DISPLAY &&
                       mMvSurroundingObject12Status == MvSurroundingObject12Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject12Status == MvSurroundingObject12Status::DARK &&
                       mMvSurroundingObject12Status == MvSurroundingObject12Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor12Stat(stat);
    }

    void updateADASViewSurroundingObjectColor13() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject13Status != MvSurroundingObject13Status::NO_DISPLAY &&
                mMvSurroundingObject13Status == MvSurroundingObject13Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat::DARK;
            } else if (mMvSurroundingObject13Status == MvSurroundingObject13Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject13Status == MvSurroundingObject13Status::NO_DISPLAY &&
                       mMvSurroundingObject13Status == MvSurroundingObject13Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject13Status == MvSurroundingObject13Status::DARK &&
                       mMvSurroundingObject13Status == MvSurroundingObject13Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor13Stat(stat);
    }

    void updateADASViewSurroundingObjectColor14() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvSurroundingObject14Status != MvSurroundingObject14Status::NO_DISPLAY &&
                mMvSurroundingObject14Status == MvSurroundingObject14Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat::DARK;
            } else if (mMvSurroundingObject14Status == MvSurroundingObject14Status::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat::BRIGHT;
            } else if (mPrevMvSurroundingObject14Status == MvSurroundingObject14Status::NO_DISPLAY &&
                       mMvSurroundingObject14Status == MvSurroundingObject14Status::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat::FADE_IN;
            } else if (mPrevMvSurroundingObject14Status == MvSurroundingObject14Status::DARK &&
                       mMvSurroundingObject14Status == MvSurroundingObject14Status::NO_DISPLAY) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat::FADE_OUT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectColor14Stat(stat);
    }

    // 5.10.5.1.2.14.2 타입
    void updateADASViewSurroundingObjectClass01() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification01Status ==
                       MvSurroundingObjectClassification01Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification01Status ==
                       MvSurroundingObjectClassification01Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification01Status ==
                       MvSurroundingObjectClassification01Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification01Status ==
                       MvSurroundingObjectClassification01Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification01Status == MvSurroundingObjectClassification01Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass01Stat(stat);
    }

    void updateADASViewSurroundingObjectClass02() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification02Status ==
                       MvSurroundingObjectClassification02Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification02Status ==
                       MvSurroundingObjectClassification02Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification02Status ==
                       MvSurroundingObjectClassification02Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification02Status ==
                       MvSurroundingObjectClassification02Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification02Status == MvSurroundingObjectClassification02Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass02Stat(stat);
    }

    void updateADASViewSurroundingObjectClass03() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification03Status ==
                       MvSurroundingObjectClassification03Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification03Status ==
                       MvSurroundingObjectClassification03Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification03Status ==
                       MvSurroundingObjectClassification03Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification03Status ==
                       MvSurroundingObjectClassification03Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification03Status == MvSurroundingObjectClassification03Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass03Stat(stat);
    }

    void updateADASViewSurroundingObjectClass04() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification04Status ==
                       MvSurroundingObjectClassification04Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification04Status ==
                       MvSurroundingObjectClassification04Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification04Status ==
                       MvSurroundingObjectClassification04Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification04Status ==
                       MvSurroundingObjectClassification04Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification04Status == MvSurroundingObjectClassification04Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass04Stat(stat);
    }

    void updateADASViewSurroundingObjectClass05() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification05Status ==
                       MvSurroundingObjectClassification05Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification05Status ==
                       MvSurroundingObjectClassification05Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification05Status ==
                       MvSurroundingObjectClassification05Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification05Status ==
                       MvSurroundingObjectClassification05Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification05Status == MvSurroundingObjectClassification05Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass05Stat(stat);
    }

    void updateADASViewSurroundingObjectClass06() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification06Status ==
                       MvSurroundingObjectClassification06Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification06Status ==
                       MvSurroundingObjectClassification06Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification06Status ==
                       MvSurroundingObjectClassification06Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification06Status ==
                       MvSurroundingObjectClassification06Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification06Status == MvSurroundingObjectClassification06Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass06Stat(stat);
    }

    void updateADASViewSurroundingObjectClass07() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification07Status ==
                       MvSurroundingObjectClassification07Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification07Status ==
                       MvSurroundingObjectClassification07Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification07Status ==
                       MvSurroundingObjectClassification07Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification07Status ==
                       MvSurroundingObjectClassification07Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification07Status == MvSurroundingObjectClassification07Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass07Stat(stat);
    }

    void updateADASViewSurroundingObjectClass08() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification08Status ==
                       MvSurroundingObjectClassification08Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification08Status ==
                       MvSurroundingObjectClassification08Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification08Status ==
                       MvSurroundingObjectClassification08Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification08Status ==
                       MvSurroundingObjectClassification08Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification08Status == MvSurroundingObjectClassification08Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass08Stat(stat);
    }

    void updateADASViewSurroundingObjectClass09() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification09Status ==
                       MvSurroundingObjectClassification09Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification09Status ==
                       MvSurroundingObjectClassification09Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification09Status ==
                       MvSurroundingObjectClassification09Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification09Status ==
                       MvSurroundingObjectClassification09Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification09Status == MvSurroundingObjectClassification09Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass09Stat(stat);
    }

    void updateADASViewSurroundingObjectClass10() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification10Status ==
                       MvSurroundingObjectClassification10Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification10Status ==
                       MvSurroundingObjectClassification10Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification10Status ==
                       MvSurroundingObjectClassification10Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification10Status ==
                       MvSurroundingObjectClassification10Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification10Status == MvSurroundingObjectClassification10Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass10Stat(stat);
    }

    void updateADASViewSurroundingObjectClass11() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification11Status ==
                       MvSurroundingObjectClassification11Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification11Status ==
                       MvSurroundingObjectClassification11Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification11Status ==
                       MvSurroundingObjectClassification11Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification11Status ==
                       MvSurroundingObjectClassification11Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification11Status == MvSurroundingObjectClassification11Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass11Stat(stat);
    }

    void updateADASViewSurroundingObjectClass12() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification12Status ==
                       MvSurroundingObjectClassification12Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification12Status ==
                       MvSurroundingObjectClassification12Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification12Status ==
                       MvSurroundingObjectClassification12Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification12Status ==
                       MvSurroundingObjectClassification12Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification12Status == MvSurroundingObjectClassification12Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass12Stat(stat);
    }

    void updateADASViewSurroundingObjectClass13() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification13Status ==
                       MvSurroundingObjectClassification13Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification13Status ==
                       MvSurroundingObjectClassification13Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification13Status ==
                       MvSurroundingObjectClassification13Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification13Status ==
                       MvSurroundingObjectClassification13Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification13Status == MvSurroundingObjectClassification13Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass13Stat(stat);
    }

    void updateADASViewSurroundingObjectClass14() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::UNKNOWN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::UNKNOWN;
            } else if (mMvSurroundingObjectClassification14Status ==
                       MvSurroundingObjectClassification14Status::PASSENGER_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::PASSENGER_VEHICLE;
            } else if (mMvSurroundingObjectClassification14Status ==
                       MvSurroundingObjectClassification14Status::COMMERCIAL_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::COMMERCIAL_VEHICLE;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::PEDESTRIAN) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::PEDESTRIAN;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::CYCLIST) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::CYCLIST;
            } else if (mMvSurroundingObjectClassification14Status ==
                       MvSurroundingObjectClassification14Status::MOTOR_CYCLIST_OR_PTW) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::MOTOR_CYCLIST_OR_PTW;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::RUBBER_CONE;
            } else if (mMvSurroundingObjectClassification14Status ==
                       MvSurroundingObjectClassification14Status::EMERGENCY_VEHICLE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::EMERGENCY_VEHICLE;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::ANIMAL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::ANIMAL;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::TUBULAR_MARKER) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::TUBULAR_MARKER;
            } else if (mMvSurroundingObjectClassification14Status == MvSurroundingObjectClassification14Status::PE_BARREL) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat::PE_BARREL;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectClass14Stat(stat);
    }

    // 5.10.5.1.2.14.3 횡위치
    void updateADASViewSurroundingObjectLat01() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition01Value >= 0 && mMvSurroundingObjectLateralPosition01Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition01Value;
            } else if (mMvSurroundingObjectLateralPosition01Value >= 1024 && mMvSurroundingObjectLateralPosition01Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition01Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat01Value(value);
    }

    void updateADASViewSurroundingObjectLat02() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition02Value >= 0 && mMvSurroundingObjectLateralPosition02Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition02Value;
            } else if (mMvSurroundingObjectLateralPosition02Value >= 1024 && mMvSurroundingObjectLateralPosition02Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition02Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat02Value(value);
    }

    void updateADASViewSurroundingObjectLat03() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition03Value >= 0 && mMvSurroundingObjectLateralPosition03Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition03Value;
            } else if (mMvSurroundingObjectLateralPosition03Value >= 1024 && mMvSurroundingObjectLateralPosition03Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition03Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat03Value(value);
    }

    void updateADASViewSurroundingObjectLat04() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition04Value >= 0 && mMvSurroundingObjectLateralPosition04Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition04Value;
            } else if (mMvSurroundingObjectLateralPosition04Value >= 1024 && mMvSurroundingObjectLateralPosition04Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition04Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat04Value(value);
    }

    void updateADASViewSurroundingObjectLat05() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition05Value >= 0 && mMvSurroundingObjectLateralPosition05Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition05Value;
            } else if (mMvSurroundingObjectLateralPosition05Value >= 1024 && mMvSurroundingObjectLateralPosition05Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition05Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat05Value(value);
    }

    void updateADASViewSurroundingObjectLat06() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition06Value >= 0 && mMvSurroundingObjectLateralPosition06Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition06Value;
            } else if (mMvSurroundingObjectLateralPosition06Value >= 1024 && mMvSurroundingObjectLateralPosition06Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition06Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat06Value(value);
    }

    void updateADASViewSurroundingObjectLat07() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition07Value >= 0 && mMvSurroundingObjectLateralPosition07Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition07Value;
            } else if (mMvSurroundingObjectLateralPosition07Value >= 1024 && mMvSurroundingObjectLateralPosition07Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition07Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat07Value(value);
    }

    void updateADASViewSurroundingObjectLat08() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition08Value >= 0 && mMvSurroundingObjectLateralPosition08Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition08Value;
            } else if (mMvSurroundingObjectLateralPosition08Value >= 1024 && mMvSurroundingObjectLateralPosition08Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition08Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat08Value(value);
    }

    void updateADASViewSurroundingObjectLat09() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition09Value >= 0 && mMvSurroundingObjectLateralPosition09Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition09Value;
            } else if (mMvSurroundingObjectLateralPosition09Value >= 1024 && mMvSurroundingObjectLateralPosition09Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition09Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat09Value(value);
    }

    void updateADASViewSurroundingObjectLat10() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition10Value >= 0 && mMvSurroundingObjectLateralPosition10Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition10Value;
            } else if (mMvSurroundingObjectLateralPosition10Value >= 1024 && mMvSurroundingObjectLateralPosition10Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition10Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat10Value(value);
    }

    void updateADASViewSurroundingObjectLat11() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition11Value >= 0 && mMvSurroundingObjectLateralPosition11Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition11Value;
            } else if (mMvSurroundingObjectLateralPosition11Value >= 1024 && mMvSurroundingObjectLateralPosition11Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition11Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat11Value(value);
    }

    void updateADASViewSurroundingObjectLat12() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition12Value >= 0 && mMvSurroundingObjectLateralPosition12Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition12Value;
            } else if (mMvSurroundingObjectLateralPosition12Value >= 1024 && mMvSurroundingObjectLateralPosition12Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition12Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat12Value(value);
    }

    void updateADASViewSurroundingObjectLat13() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition13Value >= 0 && mMvSurroundingObjectLateralPosition13Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition13Value;
            } else if (mMvSurroundingObjectLateralPosition13Value >= 1024 && mMvSurroundingObjectLateralPosition13Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition13Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat13Value(value);
    }

    void updateADASViewSurroundingObjectLat14() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLateralPosition14Value >= 0 && mMvSurroundingObjectLateralPosition14Value <= 300) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Stat::LEFT;
                value = mMvSurroundingObjectLateralPosition14Value;
            } else if (mMvSurroundingObjectLateralPosition14Value >= 1024 && mMvSurroundingObjectLateralPosition14Value <= 1323) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Stat::RIGHT;
                value = 1023 - mMvSurroundingObjectLateralPosition14Value;  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLat14Value(value);
    }

    // 5.10.5.1.2.14.4 종위치
    void updateADASViewSurroundingObjectLong01() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition01Value >= 0 && mMvSurroundingObjectLongitudinalPosition01Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition01Value;
            } else if (mMvSurroundingObjectLongitudinalPosition01Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition01Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition01Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong01Value(value);
    }

    void updateADASViewSurroundingObjectLong02() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition02Value >= 0 && mMvSurroundingObjectLongitudinalPosition02Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition02Value;
            } else if (mMvSurroundingObjectLongitudinalPosition02Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition02Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition02Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong02Value(value);
    }

    void updateADASViewSurroundingObjectLong03() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition03Value >= 0 && mMvSurroundingObjectLongitudinalPosition03Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition03Value;
            } else if (mMvSurroundingObjectLongitudinalPosition03Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition03Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition03Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong03Value(value);
    }

    void updateADASViewSurroundingObjectLong04() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition04Value >= 0 && mMvSurroundingObjectLongitudinalPosition04Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition04Value;
            } else if (mMvSurroundingObjectLongitudinalPosition04Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition04Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition04Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong04Value(value);
    }

    void updateADASViewSurroundingObjectLong05() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition05Value >= 0 && mMvSurroundingObjectLongitudinalPosition05Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition05Value;
            } else if (mMvSurroundingObjectLongitudinalPosition05Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition05Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition05Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong05Value(value);
    }

    void updateADASViewSurroundingObjectLong06() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition06Value >= 0 && mMvSurroundingObjectLongitudinalPosition06Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition06Value;
            } else if (mMvSurroundingObjectLongitudinalPosition06Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition06Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition06Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong06Value(value);
    }

    void updateADASViewSurroundingObjectLong07() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition07Value >= 0 && mMvSurroundingObjectLongitudinalPosition07Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition07Value;
            } else if (mMvSurroundingObjectLongitudinalPosition07Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition07Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition07Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong07Value(value);
    }

    void updateADASViewSurroundingObjectLong08() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition08Value >= 0 && mMvSurroundingObjectLongitudinalPosition08Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition08Value;
            } else if (mMvSurroundingObjectLongitudinalPosition08Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition08Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition08Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong08Value(value);
    }

    void updateADASViewSurroundingObjectLong09() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition09Value >= 0 && mMvSurroundingObjectLongitudinalPosition09Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition09Value;
            } else if (mMvSurroundingObjectLongitudinalPosition09Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition09Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition09Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong09Value(value);
    }

    void updateADASViewSurroundingObjectLong10() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition10Value >= 0 && mMvSurroundingObjectLongitudinalPosition10Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition10Value;
            } else if (mMvSurroundingObjectLongitudinalPosition10Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition10Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition10Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong10Value(value);
    }

    void updateADASViewSurroundingObjectLong11() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition11Value >= 0 && mMvSurroundingObjectLongitudinalPosition11Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition11Value;
            } else if (mMvSurroundingObjectLongitudinalPosition11Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition11Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition11Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong11Value(value);
    }

    void updateADASViewSurroundingObjectLong12() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition12Value >= 0 && mMvSurroundingObjectLongitudinalPosition12Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition12Value;
            } else if (mMvSurroundingObjectLongitudinalPosition12Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition12Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition12Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong12Value(value);
    }

    void updateADASViewSurroundingObjectLong13() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition13Value >= 0 && mMvSurroundingObjectLongitudinalPosition13Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition13Value;
            } else if (mMvSurroundingObjectLongitudinalPosition13Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition13Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition13Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong13Value(value);
    }

    void updateADASViewSurroundingObjectLong14() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Stat::OFF;
        HInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectLongitudinalPosition14Value >= 0 && mMvSurroundingObjectLongitudinalPosition14Value <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition14Value;
            } else if (mMvSurroundingObjectLongitudinalPosition14Value >= 1846 &&
                       mMvSurroundingObjectLongitudinalPosition14Value <= 2045) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Stat::VALUE;
                value = mMvSurroundingObjectLongitudinalPosition14Value - 2046;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectLong14Value(value);
    }

    // 5.10.5.1.2.14.5 방향
    void updateADASViewSurroundingObjectHeadingAngle01() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle01Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle01Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle01Value >= 0 && mMvSurroundingObjectHeadingAngle01Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle01Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle01Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle01Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle01Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle02() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle02Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle02Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle02Value >= 0 && mMvSurroundingObjectHeadingAngle02Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle02Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle02Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle02Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle02Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle03() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle03Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle03Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle03Value >= 0 && mMvSurroundingObjectHeadingAngle03Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle03Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle03Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle03Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle03Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle04() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle04Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle04Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle04Value >= 0 && mMvSurroundingObjectHeadingAngle04Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle04Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle04Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle04Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle04Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle05() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle05Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle05Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle05Value >= 0 && mMvSurroundingObjectHeadingAngle05Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle05Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle05Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle05Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle05Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle06() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle06Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle06Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle06Value >= 0 && mMvSurroundingObjectHeadingAngle06Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle06Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle06Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle06Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle06Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle07() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle07Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle07Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle07Value >= 0 && mMvSurroundingObjectHeadingAngle07Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle07Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle07Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle07Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle07Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle08() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle08Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle08Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle08Value >= 0 && mMvSurroundingObjectHeadingAngle08Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle08Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle08Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle08Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle08Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle09() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle09Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle09Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle09Value >= 0 && mMvSurroundingObjectHeadingAngle09Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle09Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle09Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle09Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle09Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle10() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle10Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle10Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle10Value >= 0 && mMvSurroundingObjectHeadingAngle10Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle10Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle10Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle10Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle10Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle11() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle11Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle11Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle11Value >= 0 && mMvSurroundingObjectHeadingAngle11Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle11Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle11Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle11Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle11Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle12() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle12Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle12Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle12Value >= 0 && mMvSurroundingObjectHeadingAngle12Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle12Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle12Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle12Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle12Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle13() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle13Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle13Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle13Value >= 0 && mMvSurroundingObjectHeadingAngle13Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle13Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle13Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle13Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle13Value(value);
    }

    void updateADASViewSurroundingObjectHeadingAngle14() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle14Stat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle14Stat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvSurroundingObjectHeadingAngle14Value >= 0 && mMvSurroundingObjectHeadingAngle14Value <= 72) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle14Stat::VALUE;
                value = mMvSurroundingObjectHeadingAngle14Value;
            }
        }
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle14Stat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewSurroundingObjectHeadingAngle14Value(value);
    }

    // 5.10.5.1.2.1 차선-왼쪽 (1-2)
    void updateLeftLane() {
        updateConstantAdasLeftLaneStat();
        updateConstantADASViewLeftArrowStat();
    }

    // 5.10.5.1.2.1.1 칼라
    void updateConstantAdasLeftLaneStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat adasLeftLaneStat =
            SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::OFF;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mMvLeftLineStatus == MvLeftLineStatus::GRAY || mMvLeftLineStatus == MvLeftLineStatus::UNHANDLED_TIMEOUT) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::GRAY;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::WHITE) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::WHITE;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::AMBER) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::AMBER;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::GREEN) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::GREEN;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::WHITE_DASH) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::WHITE_DASH;
            } else if (mMvLeftLineStatus == MvLeftLineStatus::GREEN_DASH) {
                adasLeftLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat::GREEN_DASH;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASLeftLaneStat(adasLeftLaneStat);
    }

    // 5.10.5.1.2.1.2 횡위치
    void updateConstantADASLeftLineLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValueStat adasLeftLineLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValueStat::OFF;
        HUInt64 adasLeftLineLatPosValue = 0;

        if (mIsIgnOn == true) {
            if (mMvLeftLineOffsetValue == kTimeOut) {
                adasLeftLineLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValueStat::VALUE;
                adasLeftLineLatPosValue = 15;
            } else if (mMvLeftLineOffsetValue >= 0 && mMvLeftLineOffsetValue <= 45) {
                adasLeftLineLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValueStat::VALUE;
                adasLeftLineLatPosValue = mMvLeftLineOffsetValue;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValueStat(adasLeftLineLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASLeftLineLatPosValue(adasLeftLineLatPosValue);
    }

    // 5.10.5.1.2.2 차선-오른쪽 (1-3)
    void updateRightLane() {
        updateConstantAdasRightLaneStat();
        updateConstantADASViewRightArrowStat();
    }

    // 5.10.5.1.2.2.1 칼라
    void updateConstantAdasRightLaneStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat adasRightLaneStat =
            SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::OFF;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mMvRightLineStatus == MvRightLineStatus::GRAY || mMvRightLineStatus == MvRightLineStatus::UNHANDLED_TIMEOUT) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::GRAY;
            } else if (mMvRightLineStatus == MvRightLineStatus::WHITE) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::WHITE;
            } else if (mMvRightLineStatus == MvRightLineStatus::AMBER) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::AMBER;
            } else if (mMvRightLineStatus == MvRightLineStatus::GREEN) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::GREEN;
            } else if (mMvRightLineStatus == MvRightLineStatus::WHITE_DASH) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::WHITE_DASH;
            } else if (mMvRightLineStatus == MvRightLineStatus::GREEN_DASH) {
                adasRightLaneStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat::GREEN_DASH;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASRightLaneStat(adasRightLaneStat);
    }

    // 5.10.5.1.2.2.2 횡위치
    void updateConstantADASRightLineLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValueStat adasRightLineLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValueStat::OFF;
        HUInt64 adasRightLineLatPosValue = 0;

        if (mIsIgnOn == true) {
            if (mMvRightLineOffsetValue == kTimeOut) {
                adasRightLineLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValueStat::VALUE;
                adasRightLineLatPosValue = 15;
            } else if (mMvRightLineOffsetValue >= 0 && mMvRightLineOffsetValue <= 45) {
                adasRightLineLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValueStat::VALUE;
                adasRightLineLatPosValue = mMvRightLineOffsetValue;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValueStat(adasRightLineLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASRightLineLatPosValue(adasRightLineLatPosValue);
    }

    // 5.10.5.1.2.3 차간 거리 (1-4)
    void updateHeadwayVehicleDistance() {
        updateConstantAdasHeadwayDistStat();
        updateConstantAdasHeadwayDistValue();
    }

    // 5.10.5.1.2.3.1 칼라
    void updateConstantAdasHeadwayDistStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistStat adasHeadwayDistStat =
            SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvVehicleDistanceStatus == MvVehicleDistanceStatus::MAIN) {
                adasHeadwayDistStat = SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistStat::BRAND;
            } else if (mMvVehicleDistanceStatus == MvVehicleDistanceStatus::WHITE) {
                adasHeadwayDistStat = SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistStat::WHITE;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistStat(adasHeadwayDistStat);
    }

    // 5.10.5.1.2.3.2 종위치
    void updateConstantAdasHeadwayDistValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValueStat adasHeadwayDistValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValueStat::OFF;
        HUInt64 adasHeadwayDistValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvVehicleDistanceValue >= 0 && mMvVehicleDistanceValue <= 1000) {
                adasHeadwayDistValueStat = SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValueStat::VALUE;
                adasHeadwayDistValue = mMvVehicleDistanceValue;
            } else if (mMvVehicleDistanceValue > 1000 && mMvVehicleDistanceValue <= 1500) {
                adasHeadwayDistValueStat = SFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValueStat::VALUE;
                adasHeadwayDistValue = 1000;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValueStat(adasHeadwayDistValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASHeadwayDistValue(adasHeadwayDistValue);
    }

    void updateLeftLatPosValue() {
        updateConstantADASCenterRoadLeftLatPosValue();
        updateConstantADASLeftLineLatPosValue();
        updateConstantADASLeftRoadLatPosValue();
    }

    void updateRightLatPosValue() {
        updateConstantADASCenterRoadRightLatPosValue();
        updateConstantADASRightLineLatPosValue();
        updateConstantADASRightRoadLatPosValue();
    }

    // 5.10.5.1.2.1.3 차선-왼쪽 (1-2) 곡률
    // 5.10.5.1.2.2.3 차선-오른쪽 (1-3) 곡률
    void updateLaneCvrtLvlValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat valueStatLane =
            SFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mMvDrivingLaneRadiusValue == 0 || mMvDrivingLaneRadiusValue == kTimeOut) {
                valueStatLane = SFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat::STRAIGHT;
            } else if (mMvDrivingLaneRadiusValue >= 1 && mMvDrivingLaneRadiusValue <= 15) {
                valueStatLane = SFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat::LEFT_VALUE;
                value = mMvDrivingLaneRadiusValue;
            } else if (mMvDrivingLaneRadiusValue >= 16 && mMvDrivingLaneRadiusValue <= 30) {
                valueStatLane = SFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat::RIGHT_VALUE;
                value = mMvDrivingLaneRadiusValue - 15;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValueStat(valueStatLane);
        setSFCDetail_Contents_ADAS_NEWConstantADASLaneCvrtLvlValue(value);
    }

    // 5.10.5.1.2.4.5 노면-중앙 (1-5) 곡률
    // 5.10.5.1.2.5.3 노면-왼쪽 (1-6) 곡률
    // 5.10.5.1.2.6.3 노면-오른쪽 (1-7) 곡률
    void updateRoadCvrtLvlValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat valueStatRoad =
            SFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvDrivingLaneRadiusValue == 0 || mMvDrivingLaneRadiusValue == kTimeOut) {
                valueStatRoad = SFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat::STRAIGHT;
            } else if (mMvDrivingLaneRadiusValue >= 1 && mMvDrivingLaneRadiusValue <= 15) {
                valueStatRoad = SFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat::LEFT_VALUE;
                value = mMvDrivingLaneRadiusValue;
            } else if (mMvDrivingLaneRadiusValue >= 16 && mMvDrivingLaneRadiusValue <= 30) {
                valueStatRoad = SFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat::RIGHT_VALUE;
                value = mMvDrivingLaneRadiusValue - 15;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValueStat(valueStatRoad);
        setSFCDetail_Contents_ADAS_NEWConstantADASRoadCvrtLvlValue(value);
    }

    void updateCenterRoad() {
        updateConstantAdasCenterRoadStat();
        updateConstantADASCenterRoadLongValue();
        updateConstantADASCenterRoadRightLatPosValue();
    }

    // 5.10.5.1.2.4 노면-중앙 (1-5)
    // 5.10.5.1.2.4.1 칼라
    void updateConstantAdasCenterRoadStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat adasCenterRoadStat =
            SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::GREEN) {
                adasCenterRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat::GREEN;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::GRAY) {
                adasCenterRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat::GRAY;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::CYAN) {
                adasCenterRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat::CYAN;
            } else if (mMvCenterRoadSurfaceStatus == MvCenterRoadSurfaceStatus::AMBER) {
                adasCenterRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat::AMBER;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadStat(adasCenterRoadStat);
    }

    // 5.10.5.1.2.4.2 종위치
    void updateConstantADASCenterRoadLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValueStat adasCenterRoadLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValueStat::OFF;
        HUInt64 adasCenterRoadLongValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvCenterRoadSurfaceDistanceValue >= 0 && mMvCenterRoadSurfaceDistanceValue <= 1000) {
                adasCenterRoadLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValueStat::VALUE;
                adasCenterRoadLongValue = mMvCenterRoadSurfaceDistanceValue;
            } else if (mMvCenterRoadSurfaceDistanceValue > 1000 && mMvCenterRoadSurfaceDistanceValue <= 1500) {
                adasCenterRoadLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValueStat::VALUE;
                adasCenterRoadLongValue = 1000;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValueStat(adasCenterRoadLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLongValue(adasCenterRoadLongValue);
    }

    // 5.10.5.1.2.4.3 좌측 횡위치
    void updateConstantADASCenterRoadLeftLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLeftLatPosValueStat adasCenterRoadLeftLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLeftLatPosValueStat::OFF;
        HUInt64 adasCenterRoadLeftLatPosValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftLineOffsetValue >= 0 && mMvLeftLineOffsetValue <= 45) {
                adasCenterRoadLeftLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLeftLatPosValueStat::VALUE;
                adasCenterRoadLeftLatPosValue = mMvLeftLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLeftLatPosValueStat(adasCenterRoadLeftLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadLeftLatPosValue(adasCenterRoadLeftLatPosValue);
    }

    // 5.10.5.1.2.4.4 우측 횡위치
    void updateConstantADASCenterRoadRightLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadRightLatPosValueStat adasCenterRoadRightLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadRightLatPosValueStat::OFF;
        HUInt64 adasCenterRoadRightLatPosValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvRightLineOffsetValue >= 0 && mMvRightLineOffsetValue <= 45) {
                adasCenterRoadRightLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASCenterRoadRightLatPosValueStat::VALUE;
                adasCenterRoadRightLatPosValue = mMvRightLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadRightLatPosValueStat(adasCenterRoadRightLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASCenterRoadRightLatPosValue(adasCenterRoadRightLatPosValue);
    }

    void updateLeftRoad() {
        updateConstantADASLeftRoadStat();
    }

    // 5.10.5.1.2.5 노면-왼쪽 (1-6)
    // 5.10.5.1.2.5.1 칼라
    void updateConstantADASLeftRoadStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadStat adasLeftRoadStat =
            SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftRoadSurfaceStatus == MvLeftRoadSurfaceStatus::GREEN) {
                adasLeftRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadStat::GREEN;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASLeftRoadStat(adasLeftRoadStat);
    }

    // 5.10.5.1.2.5.2 횡위치
    void updateConstantADASLeftRoadLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadLatPosValueStat adasLeftRoadLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadLatPosValueStat::OFF;
        HUInt64 adasLeftRoadLatPosValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftLineOffsetValue >= 0 && mMvLeftLineOffsetValue <= 45) {
                adasLeftRoadLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASLeftRoadLatPosValueStat::VALUE;
                adasLeftRoadLatPosValue = mMvLeftLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASLeftRoadLatPosValueStat(adasLeftRoadLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASLeftRoadLatPosValue(adasLeftRoadLatPosValue);
    }

    void updateRightRoad() {
        updateConstantADASRightRoadStat();
    }

    // 5.10.5.1.2.6 노면-오른쪽 (1-7)
    // 5.10.5.1.2.6.1 칼라
    void updateConstantADASRightRoadStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASRightRoadStat adasRightRoadStat =
            SFCDetail_Contents_ADAS_NEWConstantADASRightRoadStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true && mMvRightRoadSurfaceStatus == MvRightRoadSurfaceStatus::GREEN) {
            adasRightRoadStat = SFCDetail_Contents_ADAS_NEWConstantADASRightRoadStat::GREEN;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASRightRoadStat(adasRightRoadStat);
    }

    // 5.10.5.1.2.6.2 횡위치
    void updateConstantADASRightRoadLatPosValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASRightRoadLatPosValueStat adasRightRoadLatPosValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASRightRoadLatPosValueStat::OFF;
        HUInt64 adasRightRoadLatPosValue = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true &&
            (mMvRightLineOffsetValue >= 0 && mMvRightLineOffsetValue <= 45)) {
            adasRightRoadLatPosValueStat = SFCDetail_Contents_ADAS_NEWConstantADASRightRoadLatPosValueStat::VALUE;
            adasRightRoadLatPosValue = mMvRightLineOffsetValue;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASRightRoadLatPosValueStat(adasRightRoadLatPosValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASRightRoadLatPosValue(adasRightRoadLatPosValue);
    }

    // 5.10.5.1.2.7 스티어링휠 심볼 (2-1)
    void updateConstantLFATelltaleStat() {
        SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat lfaTelltaleStat =
            SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::GRAY) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::GRAY;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::GREEN) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::GREEN;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::WHITE) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::WHITE;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::CYAN) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::CYAN;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::GRAY_TYPE_B) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::GRAY_TYPE_B;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::GREEN_TYPE_B) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::GREEN_TYPE_B;
            } else if (mSmvLfaSymbolStatus == SmvLfaSymbolStatus::WHITE_TYPE_B) {
                lfaTelltaleStat = SFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat::WHITE_TYPE_B;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantLFATelltaleStat(lfaTelltaleStat);
    }

    void updateFrontObject() {
        updateConstantADASViewFrontVehicleStat();
        udpateConstantADASViewFrontVehicleLatValue();
        udpateConstantADASViewFrontVehicleLongValue();
    }

    // 5.10.5.1.2.8 전방 물체-정면 (2-2)
    // 5.10.5.1.2.8.1 칼라
    void updateConstantADASViewFrontVehicleStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat adasViewFrontVehicleStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_PEDESTRIAN) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_PEDESTRIAN;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_PEDESTRIAN) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_PEDESTRIAN;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_CYCLIST) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_CYCLIST;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_CYCLIST) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_CYCLIST;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::DARK_RUBBER_CONE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::DARK_RUBBER_CONE;
            } else if (mMvFrontObjectStatus == MvFrontObjectStatus::BRIGHT_RUBBER_CONE) {
                adasViewFrontVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat::BRIGHT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleStat(adasViewFrontVehicleStat);
    }

    // 5.10.5.1.2.8.2 횡위치
    void udpateConstantADASViewFrontVehicleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat valueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectLateralPositionValue == 0) {
                valueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat::CENTER;
            } else if (mMvFrontObjectLateralPositionValue >= 1 && mMvFrontObjectLateralPositionValue <= 45) {
                valueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat::RIGHT_VALUE;
                value = mMvFrontObjectLateralPositionValue;
            } else if (mMvFrontObjectLateralPositionValue >= 83 &&  // -45 ~ -1
                       mMvFrontObjectLateralPositionValue <= 127) {
                valueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat::LEFT_VALUE;
                value = labs(mMvFrontObjectLateralPositionValue - 128);
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValueStat(valueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLatValue(value);
    }

    // 5.10.5.1.2.8.3 종위치
    void udpateConstantADASViewFrontVehicleLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValueStat adasViewFrontVehicleLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValueStat::OFF;
        HUInt64 adasViewFrontVehicleLongValue = 0;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObjectLongitudinalPositionValue >= 0 && mMvFrontObjectLongitudinalPositionValue <= 1000) {
                adasViewFrontVehicleLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValueStat::VALUE;
                adasViewFrontVehicleLongValue = mMvFrontObjectLongitudinalPositionValue;
            } else if (mMvFrontObjectLongitudinalPositionValue > 1000 && mMvFrontObjectLongitudinalPositionValue <= 1500) {
                adasViewFrontVehicleLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValueStat::VALUE;
                adasViewFrontVehicleLongValue = 1000;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValueStat(adasViewFrontVehicleLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontVehicleLongValue(adasViewFrontVehicleLongValue);
    }

    void updateFrontLeftObject() {
        updateConstantADASViewFrontLeftVehicleStat();
        updateConstantADASViewFrontLeftVehicleLatValue();
        udpateConstantADASViewFrontLeftVehicleLongValue();
    }

    // 5.10.5.1.2.9 주변 물체-전방 왼쪽 (2-3)
    // 5.10.5.1.2.9.1 칼라
    void updateConstantADASViewFrontLeftVehicleStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat adasViewFrontLeftVehicleStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_PEDESTRIAN;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_PEDESTRIAN) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_PEDESTRIAN;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_PEDESTRIAN;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_PEDESTRIAN &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_PEDESTRIAN;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_CYCLIST;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_CYCLIST) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_CYCLIST;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_CYCLIST;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_CYCLIST &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_CYCLIST;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontLeftObjectStatus != MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE) {
                adasViewFrontLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::DARK_RUBBER_CONE;
            } else if (mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::BRIGHT_RUBBER_CONE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::BRIGHT_RUBBER_CONE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_IN_RUBBER_CONE;
            } else if (mPrevMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::DARK_RUBBER_CONE &&
                       mMvFrontLeftObjectStatus == MvFrontLeftObjectStatus::NO_DISPLAY) {
                adasViewFrontLeftVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        mPrevMvFrontLeftObjectStatus = mMvFrontLeftObjectStatus;
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleStat(adasViewFrontLeftVehicleStat);
    }

    // 5.10.5.1.2.9.2 횡위치
    void updateConstantADASViewFrontLeftVehicleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLatValueStat adasViewFrontLeftVehicleLatValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLatValueStat::OFF;
        HUInt64 adasViewFrontLeftVehicleLatValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true &&
            (mMvFrontLeftObjectLateralPositionValue >= 0 && mMvFrontLeftObjectLateralPositionValue <= 45)) {
            adasViewFrontLeftVehicleLatValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLatValueStat::VALUE;
            adasViewFrontLeftVehicleLatValue = mMvFrontLeftObjectLateralPositionValue;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLatValueStat(adasViewFrontLeftVehicleLatValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLatValue(adasViewFrontLeftVehicleLatValue);
    }

    // 5.10.5.1.2.9.3 종위치
    void udpateConstantADASViewFrontLeftVehicleLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat adasViewFrontLeftVehicleLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat::OFF;
        HInt64 adasViewFrontLeftVehicleLongValue = 0LL;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontLeftObjectLongitudinalPositionValue >= 0 && mMvFrontLeftObjectLongitudinalPositionValue <= 1000) {
                adasViewFrontLeftVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat::VALUE;
                adasViewFrontLeftVehicleLongValue = static_cast<HInt64>(mMvFrontLeftObjectLongitudinalPositionValue);
            } else if (mMvFrontLeftObjectLongitudinalPositionValue > 1000 &&
                       mMvFrontLeftObjectLongitudinalPositionValue <= 1500) {
                adasViewFrontLeftVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat::VALUE;
                adasViewFrontLeftVehicleLongValue = 1000LL;
            } else if (mMvFrontLeftObjectLongitudinalPositionValue >= 1996 &&
                       mMvFrontLeftObjectLongitudinalPositionValue <= 2045) {
                adasViewFrontLeftVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat::VALUE;
                adasViewFrontLeftVehicleLongValue = static_cast<HInt64>(mMvFrontLeftObjectLongitudinalPositionValue) - 2046LL;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValueStat(adasViewFrontLeftVehicleLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontLeftVehicleLongValue(adasViewFrontLeftVehicleLongValue);
    }

    void updateFrontRightObject() {
        updateConstantADASViewFrontRightVehicleStat();
        updateConstantADASViewFrontRightVehicleLatValue();
        udpateConstantADASViewFrontRightVehicleLongValue();
    }

    // 5.10.5.1.2.10 주변 물체-전방 오른쪽 (2-4)
    // 5.10.5.1.2.10.1 칼라
    void updateConstantADASViewFrontRightVehicleStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat adasViewFrontRightVehicleStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_PEDESTRIAN;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_PEDESTRIAN) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_PEDESTRIAN;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_PEDESTRIAN;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_PEDESTRIAN &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_PEDESTRIAN;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_CYCLIST;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_CYCLIST) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_CYCLIST;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST) {
                adasViewFrontRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_CYCLIST;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_CYCLIST &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_CYCLIST;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (mPrevMvFrontRightObjectStatus != MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::DARK_RUBBER_CONE;
            } else if (mMvFrontRightObjectStatus == MvFrontRightObjectStatus::BRIGHT_RUBBER_CONE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::BRIGHT_RUBBER_CONE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_IN_RUBBER_CONE;
            } else if (mPrevMvFrontRightObjectStatus == MvFrontRightObjectStatus::DARK_RUBBER_CONE &&
                       mMvFrontRightObjectStatus == MvFrontRightObjectStatus::NO_DISPLAY) {
                adasViewFrontRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        mPrevMvFrontRightObjectStatus = mMvFrontRightObjectStatus;
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleStat(adasViewFrontRightVehicleStat);
    }

    // 5.10.5.1.2.10.2 횡위치
    void updateConstantADASViewFrontRightVehicleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLatValueStat adasViewFrontRightVehicleLatValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLatValueStat::OFF;
        HUInt64 adasViewFrontRightVehicleLatValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontRightObjectLateralPositionValue >= 0 && mMvFrontRightObjectLateralPositionValue <= 45) {
                adasViewFrontRightVehicleLatValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLatValueStat::VALUE;
                adasViewFrontRightVehicleLatValue = mMvFrontRightObjectLateralPositionValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLatValueStat(adasViewFrontRightVehicleLatValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLatValue(adasViewFrontRightVehicleLatValue);
    }

    // 5.10.5.1.2.10.3 종위치
    void udpateConstantADASViewFrontRightVehicleLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat adasViewFrontRightVehicleLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat::OFF;
        HInt64 adasViewFrontRightVehicleLongValue = 0LL;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontRightObjectLongitudinalPositionValue >= 0 && mMvFrontRightObjectLongitudinalPositionValue <= 1000) {
                adasViewFrontRightVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat::VALUE;
                adasViewFrontRightVehicleLongValue = static_cast<HInt64>(mMvFrontRightObjectLongitudinalPositionValue);
            } else if ((mMvFrontRightObjectLongitudinalPositionValue > 1000) &&
                       (mMvFrontRightObjectLongitudinalPositionValue <= 1500)) {
                adasViewFrontRightVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat::VALUE;
                adasViewFrontRightVehicleLongValue = 1000LL;
            } else if ((mMvFrontRightObjectLongitudinalPositionValue >= 1996) &&
                       (mMvFrontRightObjectLongitudinalPositionValue <= 2045)) {
                adasViewFrontRightVehicleLongValueStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat::VALUE;
                adasViewFrontRightVehicleLongValue = static_cast<HInt64>(mMvFrontRightObjectLongitudinalPositionValue) - 2046LL;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValueStat(adasViewFrontRightVehicleLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontRightVehicleLongValue(adasViewFrontRightVehicleLongValue);
    }

    void updateLeftObject() {
        updateConstantADASViewLeftVehicleStat();
        updateConstantADASViewLeftVehicleLatValue();
        udpateConstantADASViewLeftVehicleLongValue();
    }

    // 5.10.5.1.2.11 주변 물체-왼쪽 (2-5)
    // 5.10.5.1.2.11.1 칼라
    void updateConstantADASViewLeftVehicleStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat adasViewLeftVehicleStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY && mMvLeftObjectStatus == MvLeftObjectStatus::DARK) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_PEDESTRIAN;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_PEDESTRIAN) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_PEDESTRIAN;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_PEDESTRIAN;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_PEDESTRIAN &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_PEDESTRIAN;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_CYCLIST;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_CYCLIST) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_CYCLIST;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_CYCLIST;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_CYCLIST &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_CYCLIST;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (mPrevMvLeftObjectStatus != MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::DARK_RUBBER_CONE;
            } else if (mMvLeftObjectStatus == MvLeftObjectStatus::BRIGHT_RUBBER_CONE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::BRIGHT_RUBBER_CONE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_IN_RUBBER_CONE;
            } else if (mPrevMvLeftObjectStatus == MvLeftObjectStatus::DARK_RUBBER_CONE &&
                       mMvLeftObjectStatus == MvLeftObjectStatus::NO_DISPLAY) {
                adasViewLeftVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        mPrevMvLeftObjectStatus = mMvLeftObjectStatus;
        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleStat(adasViewLeftVehicleStat);
    }

    // 5.10.5.1.2.11.2 횡위치
    void updateConstantADASViewLeftVehicleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLatValueStat adasViewLeftVehicleLatValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLatValueStat::OFF;
        HUInt64 adasViewLeftVehicleLatValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftObjectLateralPositionValue >= 0 && mMvLeftObjectLateralPositionValue <= 45) {
                adasViewLeftVehicleLatValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLatValueStat::VALUE;
                adasViewLeftVehicleLatValue = mMvLeftObjectLateralPositionValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLatValueStat(adasViewLeftVehicleLatValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLatValue(adasViewLeftVehicleLatValue);
    }

    // 5.10.5.1.2.11.3 종위치
    void udpateConstantADASViewLeftVehicleLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLongValueStat adasViewLeftVehicleLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLongValueStat::OFF;
        HUInt64 adasViewLeftVehicleLongValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftObjectLongitudinalPositionValue >= 0 && mMvLeftObjectLongitudinalPositionValue <= 150) {
                adasViewLeftVehicleLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLongValueStat::VALUE;
                adasViewLeftVehicleLongValue = mMvLeftObjectLongitudinalPositionValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLongValueStat(adasViewLeftVehicleLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftVehicleLongValue(adasViewLeftVehicleLongValue);
    }

    void updateRightObject() {
        updateConstantADASViewRightVehicleStat();
        updateConstantADASViewRightVehicleLatValue();
        udpateConstantADASViewRightVehicleLongValue();
    }

    // 5.10.5.1.2.21 전방 장애물
    // 5.10.5.1.2.21.1 칼라
    void updateADASViewFrontObstacleColorStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvFrontObstacleStatus == MvFrontObstacleStatus::DARK) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat::DARK;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::BRIGHT) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat::BRIGHT;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::DARK_RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat::DARK_RUBBER_CONE;
            } else if (mMvFrontObstacleStatus == MvFrontObstacleStatus::BRIGHT_RUBBER_CONE) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat::BRIGHT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleStat(stat);
    }

    // 5.10.5.1.2.21.2 횡위치
    void updateADASViewFrontObstacleLatPosStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mMvFrontObstacleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat::CENTER;
            } else if (1 <= mMvFrontObstacleLateralPositionValue && mMvFrontObstacleLateralPositionValue <= 45) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat::RIGHT_VALUE;
                value = mMvFrontObstacleLateralPositionValue;
            } else if (83 <= mMvFrontObstacleLateralPositionValue && mMvFrontObstacleLateralPositionValue <= 127) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat::LEFT_VALUE;
                value = labs(mMvFrontObstacleLateralPositionValue - 128);
            } else {
                // default
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLatPosValue(value);
    }

    // 5.10.5.1.2.21.3 종위치
    void updateADASViewFrontObstacleLongPosStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (0 <= mMvFrontObstacleLongitudinalPositionValue && mMvFrontObstacleLongitudinalPositionValue <= 1000) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosStat::VALUE;
                value = mMvFrontObstacleLongitudinalPositionValue;
            } else if (1001 <= mMvFrontObstacleLongitudinalPositionValue && mMvFrontObstacleLongitudinalPositionValue <= 1500) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosStat::VALUE;
                value = 1000;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontObstacleLongPosValue(value);
    }

    // 5.10.5.1.2.12 주변 물체-오른쪽 (2-6)
    // 5.10.5.1.2.12.1 칼라
    void updateConstantADASViewRightVehicleStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat adasViewRightVehicleStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::OFF;
        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                mMvRightObjectStatus == MvRightObjectStatus::DARK) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_PASSENGER_VEHICLE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_PASSENGER_VEHICLE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_PASSENGER_VEHICLE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_PASSENGER_VEHICLE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_PASSENGER_VEHICLE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_PASSENGER_VEHICLE;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_COMMERCIAL_VEHICLE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_COMMERCIAL_VEHICLE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE) {
                adasViewRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_COMMERCIAL_VEHICLE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_COMMERCIAL_VEHICLE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat =
                    SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_COMMERCIAL_VEHICLE;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_PEDESTRIAN;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_PEDESTRIAN) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_PEDESTRIAN;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_PEDESTRIAN;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_PEDESTRIAN &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_PEDESTRIAN;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_CYCLIST;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_CYCLIST) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_CYCLIST;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_CYCLIST;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_CYCLIST &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_CYCLIST;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_MOTORCYCLE_PTW;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_MOTORCYCLE_PTW) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_MOTORCYCLE_PTW;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_MOTORCYCLE_PTW;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_MOTORCYCLE_PTW &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_MOTORCYCLE_PTW;
            } else if (mPrevMvRightObjectStatus != MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::DARK_RUBBER_CONE;
            } else if (mMvRightObjectStatus == MvRightObjectStatus::BRIGHT_RUBBER_CONE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::BRIGHT_RUBBER_CONE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY &&
                       mMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_IN_RUBBER_CONE;
            } else if (mPrevMvRightObjectStatus == MvRightObjectStatus::DARK_RUBBER_CONE &&
                       mMvRightObjectStatus == MvRightObjectStatus::NO_DISPLAY) {
                adasViewRightVehicleStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat::FADE_OUT_RUBBER_CONE;
            } else {
                // OFF
            }
        }

        mPrevMvRightObjectStatus = mMvRightObjectStatus;
        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleStat(adasViewRightVehicleStat);
    }

    // 5.10.5.1.2.12.2 횡위치
    void updateConstantADASViewRightVehicleLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLatValueStat adasViewRightVehicleLatValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLatValueStat::OFF;
        HUInt64 adasViewRightVehicleLatValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvRightObjectLateralPositionValue >= 0 && mMvRightObjectLateralPositionValue <= 45) {
                adasViewRightVehicleLatValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLatValueStat::VALUE;
                adasViewRightVehicleLatValue = mMvRightObjectLateralPositionValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLatValueStat(adasViewRightVehicleLatValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLatValue(adasViewRightVehicleLatValue);
    }

    // 5.10.5.1.2.12.3 종위치
    void udpateConstantADASViewRightVehicleLongValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLongValueStat adasViewRightVehicleLongValueStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLongValueStat::OFF;
        HUInt64 adasViewRightVehicleLongValue = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvRightObjectLongitudinalPositionValue >= 0 && mMvRightObjectLongitudinalPositionValue <= 150) {
                adasViewRightVehicleLongValueStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLongValueStat::VALUE;
                adasViewRightVehicleLongValue = mMvRightObjectLongitudinalPositionValue;
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLongValueStat(adasViewRightVehicleLongValueStat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightVehicleLongValue(adasViewRightVehicleLongValue);
    }

    void updateRadarWave() {
        updateConstantADASViewFrontWarnStat();
        updateConstantADASViewRearLeftWarnStat();
        updateConstantADASViewRearRightWarnStat();
    }

    // 5.10.5.1.2.15 레이더 파형-전방 (3-1)
    void updateConstantADASViewFrontWarnStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewFrontWarnStat adasViewFrontWarnStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewFrontWarnStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true &&
            mMvFrontRadarWaveStatus == MvFrontRadarWaveStatus::RED_WAVE_WARNING_SYMBOL) {
            adasViewFrontWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewFrontWarnStat::WAVE_WARNING;
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewFrontWarnStat(adasViewFrontWarnStat);
    }

    // 5.10.5.1.2.16 레이더 파형-후방 왼쪽 (3-2)
    void updateConstantADASViewRearLeftWarnStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat adasViewRearLeftWarnStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat::OFF;

        if (mIsIgnOn == true) {
            if (mHudBcwInfoOnOffStatus == HudBcwInfoOnOffStatus::ENABLE) {
                if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                    mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                    adasViewRearLeftWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat::WAVE_WARNING;
                } else if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE ||
                           mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE) {
                    adasViewRearLeftWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat::WAVE;
                } else {
                    // OFF
                }
            } else if (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
                if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                    (mHudBcwUnderAdasInfoOnOffStatus == HudBcwUnderAdasInfoOnOffStatus::ENABLE &&
                     mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE_WARNING_SYMBOL)) {
                    adasViewRearLeftWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat::WAVE_WARNING;
                } else if (mMvRearLeftRadarWave1Status == MvRearLeftRadarWave1Status::RED_WAVE ||
                           (mHudBcwUnderAdasInfoOnOffStatus == HudBcwUnderAdasInfoOnOffStatus::ENABLE &&
                            mMvRearLeftRadarWave2Status == MvRearLeftRadarWave2Status::RED_WAVE)) {
                    adasViewRearLeftWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat::WAVE;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRearLeftWarnStat(adasViewRearLeftWarnStat);
    }

    // 5.10.5.1.2.17 레이더 파형-후방 오른쪽 (3-3)
    void updateConstantADASViewRearRightWarnStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat adasViewRearRightWarnStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat::OFF;

        if (mIsIgnOn == true) {
            if (mHudBcwInfoOnOffStatus == HudBcwInfoOnOffStatus::ENABLE) {
                if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                    mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE_WARNING_SYMBOL) {
                    adasViewRearRightWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat::WAVE_WARNING;
                } else if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE ||
                           mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE) {
                    adasViewRearRightWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat::WAVE;
                } else {
                    // OFF
                }
            } else if (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
                if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE_WARNING_SYMBOL ||
                    (mHudBcwUnderAdasInfoOnOffStatus == HudBcwUnderAdasInfoOnOffStatus::ENABLE &&
                     mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE_WARNING_SYMBOL)) {
                    adasViewRearRightWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat::WAVE_WARNING;
                } else if (mMvRearRightRadarWave1Status == MvRearRightRadarWave1Status::RED_WAVE ||
                           (mHudBcwUnderAdasInfoOnOffStatus == HudBcwUnderAdasInfoOnOffStatus::ENABLE &&
                            mMvRearRightRadarWave2Status == MvRearRightRadarWave2Status::RED_WAVE)) {
                    adasViewRearRightWarnStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat::WAVE;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRearRightWarnStat(adasViewRearRightWarnStat);
    }

    // 5.10.5.1.2.18 차로 변경 방향-왼쪽 (3-4)
    // 5.10.5.1.2.18.1 타입
    void updateConstantADASViewLeftArrowStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowStat adasViewLeftArrowStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLeftLaneChangeDirectionStatus == MvLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                adasViewLeftArrowStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowStat::GREEN_ANIMATION;
            } else if (mMvLeftLaneChangeDirectionStatus == MvLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION_TYPE_B) {
                adasViewLeftArrowStat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowStat::GREEN_ANIMATION_TYPE_B;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowStat(adasViewLeftArrowStat);
    }

    // 5.10.5.1.2.18.2 횡위치
    void updateADASViewLeftArrowLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValueStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValueStat::CENTER;
                value = 0;
            } else if (0x42 <= mMvLaneChangeRectangleLateralPositionValue && mMvLaneChangeRectangleLateralPositionValue <= 0x7f) {
                // 0x7f -> 1, 0x42 -> 62
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValueStat::LEFT;
                value = (mMvLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValueStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewLeftArrowLatValue(value);
    }

    // 5.10.5.1.2.19 차로 변경 방향-오른쪽 (3-5)
    // 5.10.5.1.2.19.1 타입
    void updateConstantADASViewRightArrowStat() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowStat adasViewRightArrowStat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowStat::OFF;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvRightLaneChangeDirectionStatus == MvRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                adasViewRightArrowStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowStat::GREEN_ANIMATION;
            } else if (mMvRightLaneChangeDirectionStatus == MvRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION_TYPE_B) {
                adasViewRightArrowStat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowStat::GREEN_ANIMATION_TYPE_B;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowStat(adasViewRightArrowStat);
    }

    // 5.10.5.1.2.19.2 횡위치
    void updateADASViewRightArrowLatValue() {
        SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValueStat stat =
            SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mMvLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValueStat::CENTER;
                value = 0;
            } else if (1 <= mMvLaneChangeRectangleLateralPositionValue && mMvLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValueStat::RIGHT;
                value = mMvLaneChangeRectangleLateralPositionValue;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValueStat(stat);
        setSFCDetail_Contents_ADAS_NEWConstantADASViewRightArrowLatValue(value);
    }

    // 5.10.5.2.2 CRAWL 요약 정보 표시
    void updateCrawlSummaryEvent() {
        SFCDetail_Contents_ADAS_NEWEventCrawlStat stat = SFCDetail_Contents_ADAS_NEWEventCrawlStat::OFF;

        if (mIsIgnOn == true) {
            if (isCrawlActivationStatus() == true && isCrawlActivationStep() == true) {
                stat = SFCDetail_Contents_ADAS_NEWEventCrawlStat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_NEWEventCrawlStat(stat);
    }

    // 5.10.5.2.3 ADAS 팝업
    // 5.10.5.2.3.1 CC/MSLA 팝업
    void updateCcEvent() {
        std::string eventId;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mCruiseInfoDisplayReqStatus == CruiseInfoDisplayReqStatus::AUTO_CANCELED) {
                eventId = "E71204";
            } else if (mCruiseInfoDisplayReqStatus == CruiseInfoDisplayReqStatus::NO_CONDITION) {
                eventId = "E71205";
            } else {
                // no operation
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Cc.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventCcStat(SFCDetail_Contents_ADAS_NEWEventCcStat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventCcID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventCcStat(SFCDetail_Contents_ADAS_NEWEventCcStat::ON);
        }
    }

    void updateMslaEvent() {
        std::string eventId;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mSpeedLimitInfoDisplayReqStatus == SpeedLimitInfoDisplayReqStatus::AUTO_CANCELED) {
                eventId = "E71208";
            } else if (mSpeedLimitInfoDisplayReqStatus == SpeedLimitInfoDisplayReqStatus::NO_CONDITION) {
                eventId = "E71209";
            } else {
                // no operation
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Msla.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventMslaStat(SFCDetail_Contents_ADAS_NEWEventMslaStat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventMslaID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventMslaStat(SFCDetail_Contents_ADAS_NEWEventMslaStat::ON);
        }
    }

    // 5.10.5.2.3.2 1그룹 팝업 1-1 (FR_CMR, ADAS_DRV)
    void update1Group1_1Event() {
        std::string eventId;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            switch (mGroup1AdasWarning1_1Status) {
                case Group1AdasWarning1_1Status::WARNING_FRONT:
                    eventId = "E71702";
                    break;
                case Group1AdasWarning1_1Status::WARNING_FRONT_LEFT:
                    eventId = "E71703";
                    break;
                case Group1AdasWarning1_1Status::WARNING_FRONT_RIGHT:
                    eventId = "E71704";
                    break;
                case Group1AdasWarning1_1Status::WARNING_FRONT_LEFT_CROSSING:
                    eventId = "E71705";
                    break;
                case Group1AdasWarning1_1Status::WARNING_FRONT_RIGHT_CROSSING:
                    eventId = "E71706";
                    break;
                case Group1AdasWarning1_1Status::BRAKING_FRONT:
                    eventId = "E71707";
                    break;
                case Group1AdasWarning1_1Status::BRAKING_FRONT_LEFT:
                    eventId = "E71708";
                    break;
                case Group1AdasWarning1_1Status::BRAKING_FRONT_RIGHT:
                    eventId = "E71709";
                    break;
                case Group1AdasWarning1_1Status::BRAKING_FRONT_LEFT_CROSSING:
                    eventId = "E71710";
                    break;
                case Group1AdasWarning1_1Status::BRAKING_FRONT_RIGHT_CROSSING:
                    eventId = "E71711";
                    break;
                case Group1AdasWarning1_1Status::STEERING_FRONT_LEFT:
                    eventId = "E71712";
                    break;
                case Group1AdasWarning1_1Status::STEERING_FRONT_RIGHT:
                    eventId = "E71713";
                    break;
                case Group1AdasWarning1_1Status::STEERING_FRONT_LEFT_PATH:
                    eventId = "E71714";
                    break;
                case Group1AdasWarning1_1Status::STEERING_FRONT_RIGHT_PATH:
                    eventId = "E71715";
                    break;
                case Group1AdasWarning1_1Status::STEERING_REAR_LEFT:
                    eventId = "E71716";
                    break;
                case Group1AdasWarning1_1Status::STEERING_REAR_RIGHT:
                    eventId = "E71717";
                    break;
                case Group1AdasWarning1_1Status::END_OF_ASSIST:
                    eventId = "E71718";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_1Group1.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group1Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group1Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group1ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group1Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group1Stat::ON);
        }
    }

    // 5.10.5.2.3.3 1그룹 팝업 1-2 (FR_CMR, ADAS_DRV)
    void update1Group1_2Event() {
        std::string eventId;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            switch (mGroup1AdasWarning1_2Status) {
                case Group1AdasWarning1_2Status::HANDS_OFF_WARN_LV1:
                    eventId = "E71719";
                    break;
                case Group1AdasWarning1_2Status::HANDS_OFF_WARN_LV2:
                    eventId = "E71720";
                    break;
                case Group1AdasWarning1_2Status::LFA_AUTOMATIC_OFF:
                    eventId = "E71721";
                    break;
                case Group1AdasWarning1_2Status::HDA_AUTOMATIC_OFF:
                    eventId = "E71722";
                    break;
                case Group1AdasWarning1_2Status::DAW_TAKE_REST:
                    eventId = "E71723";
                    break;
                case Group1AdasWarning1_2Status::LCA_MANUAL_ON_REQUEST:
                    eventId = "E71769";
                    break;
                case Group1AdasWarning1_2Status::HDP_EM:
                    eventId = "E71770";
                    break;
                case Group1AdasWarning1_2Status::HDP_MRM:
                    eventId = "E71771";
                    break;
                case Group1AdasWarning1_2Status::EMERGENCY_STOP:
                    eventId = "E71777";
                    break;
                case Group1AdasWarning1_2Status::HDA_HF_MANUAL_ON_REQUEST:
                    eventId = "E71778";
                    break;
                case Group1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL1:
                    eventId = "E71780";
                    break;
                case Group1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL2:
                    eventId = "E71781";
                    break;
                case Group1AdasWarning1_2Status::TD_LV2_IN_BIGGER:
                    eventId = "E71782";
                    break;
                case Group1AdasWarning1_2Status::EMERGENCY_STOP_ACTIVE_IN_BIGGER:
                    eventId = "E71783";
                    break;
                case Group1AdasWarning1_2Status::DAW_TAKE_REST_WITH_OK_BUTTON:
                    eventId = "E71784";
                    break;
                case Group1AdasWarning1_2Status::HDP_TD_LV1:
                    eventId = "E71772";
                    break;
                case Group1AdasWarning1_2Status::HDP_TD_LV2:
                    eventId = "E71773";
                    break;
                case Group1AdasWarning1_2Status::HDP_INACTIVE:
                    eventId = "E71774";
                    break;
                case Group1AdasWarning1_2Status::HDP_WARNING:
                    eventId = "E71775";
                    break;
                case Group1AdasWarning1_2Status::HDP_CONDITION_NO_MET:
                    eventId = "E71776";
                    break;
                case Group1AdasWarning1_2Status::HDP_EM_IN_DDT:
                    eventId = "E71796";
                    break;
                case Group1AdasWarning1_2Status::HDP_SET_THE_WIPER_LIGHT_AUTO:
                    eventId = "E71797";
                    break;
                case Group1AdasWarning1_2Status::HDP_BE_PREPARED_TO_TAKEOVER:
                    eventId = "E71798";
                    break;
                case Group1AdasWarning1_2Status::EMERGENCY_STOP_INACTIVE:
                    eventId = "E71810";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_2Group1.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group1Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_2Group1Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group1ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group1Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_2Group1Stat::ON);
        }
    }

    // 5.10.5.2.3.4 1그룹 팝업 3-1 (RDD)
    void update1Group3_1Event_E71799() {
        SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat stat =
            SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat::OFF;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mHdpRDDWarningState == HdpRDDWarningState::MRM_WARNING_REQUEST) {
                stat = SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat::ON;
            } else if (mEscHDPEngagedStatus == EscHDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
                       mEscHDPEngagedStatus == EscHDPEngagedStatus::MRM_WITH_IEB_ALONE ||
                       mEscHDPEngagedStatus == EscHDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
                       mEscHDPEngagedStatus == EscHDPEngagedStatus::MRM_WITH_RCU_ALONE) {
                stat = SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat::ON;
            } else if (mRcuHDPEngagedStatus == RcuHDPEngagedStatus::ENGAGED_SUB_CONTROLLER_IEB ||
                       mRcuHDPEngagedStatus == RcuHDPEngagedStatus::MRM_WITH_IEB_ALONE ||
                       mRcuHDPEngagedStatus == RcuHDPEngagedStatus::ENGAGED_SUB_CONTROLLER_RCU ||
                       mRcuHDPEngagedStatus == RcuHDPEngagedStatus::MRM_WITH_RCU_ALONE) {
                stat = SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat::ON;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71799Stat(stat);
    }

    void update1Group3_1Event_E71800() {
        SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71800Stat stat =
            SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71800Stat::OFF;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mHdpRDDWarningState == HdpRDDWarningState::EM_WARNING_REQUEST) {
                stat = SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71800Stat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71800Stat(stat);
    }

    void update1Group3_1Event_E71801() {
        SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71801Stat stat =
            SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71801Stat::OFF;

        if (mIsIgnOn == true && (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE ||
                                 mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::INVALID)) {
            if (mHdpRDDWarningState == HdpRDDWarningState::MRM_STOP) {
                stat = SFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71801Stat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_NEWEventGroup1FullPopup3_1_E71801Stat(stat);
    }

    // 5.10.5.2.3.5 4그룹 팝업 1-1 (FR_CMR, ADAS_DRV)
    void update4Group1_1Event() {
        std::string eventId;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_MRM) {
                eventId = "E71731";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_ATTENTION_WARNING) {
                eventId = "E71732";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_AUTOMATIC_OFF) {
                eventId = "E71733";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_CONDITION_NOT_MET) {
                eventId = "E71734";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_STOP_CONTROL) {
                eventId = "E71735";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::CONNECTING_TRAILER_OFF) {
                eventId = "E71736";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::DAW_LEAD_VEHICLE_DEPARTURE) {
                eventId = "E71737";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_REGULATIONS) {
                eventId = "E71738";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDP_INITIAL_ACTIVATION) {
                eventId = "E71739";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDP_CHECK_PATH) {
                eventId = "E71740";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDP_INACTIVE) {
                eventId = "E71741";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDP_LV2_CONDITION_NOT_MET) {
                eventId = "E71742";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDA_START_POP_UP) {
                eventId = "E71743";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::SCC_CONDITION_NOT_MET_BY_MRM) {
                eventId = "E71744";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::LFA_CONDITION_NOT_MET_BY_EMERGENCY_STOP) {
                eventId = "E71745";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::LFA_CONDITION_NOT_MET) {
                eventId = "E71746";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HDP_NOT_APPLIED) {
                eventId = "E71747";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::OFF_DUE_TO_LIMITATION_ON_DRIVER_MONITORING) {
                eventId = "E71748";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_IIHS) {
                eventId = "E71749";
            } else if (mGroup4AdasWarning1_1Status == Group4AdasWarning1_1Status::HANDS_FREE_INITIAL_ACTIVATION) {
                eventId = "E71779";
            } else {
                // do nothing
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_1Group4.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group4Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group4Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group4ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group4Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group4Stat::ON);
        }
    }

    // 5.10.5.2.3.6 2그룹 미니 팝업 1-1
    void update2Group1_1Event() {
        std::string eventId;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            switch (mGroup2AdasWarning1_1Status) {
                case Group2AdasWarning1_1Status::MSLA_AUTOSET_ON:
                    eventId = "E72501";
                    break;
                case Group2AdasWarning1_1Status::MSLA_AUTOSET_SPEED_CHANGED:
                    eventId = "E72502";
                    break;
                case Group2AdasWarning1_1Status::SCC_AUTOSET_ON:
                    eventId = "E72503";
                    break;
                case Group2AdasWarning1_1Status::SCC_AUTOSET_SPEED_CHANGED:
                    eventId = "E72504";
                    break;
                case Group2AdasWarning1_1Status::SCC_DISTANCE_LV1:
                    eventId = "E72505";
                    break;
                case Group2AdasWarning1_1Status::SCC_DISTANCE_LV2:
                    eventId = "E72506";
                    break;
                case Group2AdasWarning1_1Status::SCC_DISTANCE_LV3:
                    eventId = "E72507";
                    break;
                case Group2AdasWarning1_1Status::SCC_DISTANCE_LV4:
                    eventId = "E72508";
                    break;
                case Group2AdasWarning1_1Status::SCC_ML_DISTANCE_LV1:
                    eventId = "E72631";
                    break;
                case Group2AdasWarning1_1Status::SCC_ML_DISTANCE_LV2:
                    eventId = "E72632";
                    break;
                case Group2AdasWarning1_1Status::SCC_ML_DISTANCE_LV3:
                    eventId = "E72633";
                    break;
                case Group2AdasWarning1_1Status::SCC_ML_DISTANCE_LV4:
                    eventId = "E72634";
                    break;
                case Group2AdasWarning1_1Status::NSCC_WARNING:
                    eventId = "E72509";
                    break;
                case Group2AdasWarning1_1Status::LCA_COLLISION:
                    eventId = "E72510";
                    break;
                case Group2AdasWarning1_1Status::LCA_CONDITION_NOT_MET:
                    eventId = "E72511";
                    break;
                case Group2AdasWarning1_1Status::LCA_DRIVING_LANE_ANALYZING:
                    eventId = "E72512";
                    break;
                case Group2AdasWarning1_1Status::LCA_CURVED_ROAD:
                    eventId = "E72513";
                    break;
                case Group2AdasWarning1_1Status::LCA_NARROW_LANE:
                    eventId = "E72514";
                    break;
                case Group2AdasWarning1_1Status::LCA_NOT_OPERATIONAL_SECTION:
                    eventId = "E72515";
                    break;
                case Group2AdasWarning1_1Status::LCA_HAZARD_LIGHT:
                    eventId = "E72516";
                    break;
                case Group2AdasWarning1_1Status::LCA_LOW_SPEED:
                    eventId = "E72517";
                    break;
                case Group2AdasWarning1_1Status::LCA_HANDS_OFF:
                    eventId = "E72518";
                    break;
                case Group2AdasWarning1_1Status::LCA_UNAVAILABLE_LANE:
                    eventId = "E72519";
                    break;
                case Group2AdasWarning1_1Status::LCA_STEERING_CONTROL_DETECTED:
                    eventId = "E72520";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_1Group2.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group2Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group2ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_1Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_1Group2Stat::ON);
        }
    }

    // 5.10.5.2.3.7 2그룹 미니 팝업 1-2
    void update2Group1_2Event() {
        std::string eventId;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            switch (mGroup2AdasWarning1_2Status) {
                case Group2AdasWarning1_2Status::STOP_THE_ACCELERATOR_PEDAL_INPUT:
                    eventId = "E72521";
                    break;
                case Group2AdasWarning1_2Status::STOP_THE_BRAKE_PEDAL_INPUT:
                    eventId = "E72522";
                    break;
                case Group2AdasWarning1_2Status::INABILITY_TO_SET_ADAS_FUNCTION:
                    eventId = "E72523";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_SHORTKEY_INPUT:
                    eventId = "E72524";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_UNABLE_ACC_OVERRIDE:
                    eventId = "E72529";
                    break;
                case Group2AdasWarning1_2Status::TURN_OFF_HAZARD_LIGHTS_AND_TURN_SIGNAL:
                    eventId = "E72530";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_KEEP_REACHABLE_RANGE:
                    eventId = "E72610";
                    break;
                case Group2AdasWarning1_2Status::INFORM_THE_SET_SPEED_CHANGED:
                    eventId = "E72525";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_AUTO_ACTIVATE_DEFOG:
                    eventId = "E72526";
                    break;
                case Group2AdasWarning1_2Status::SET_THE_WIPER_LIGHT_AUTO:
                    eventId = "E72545";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_MERGING_LANES:
                    eventId = "E72531";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_CONSTRUCTION:
                    eventId = "E72532";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_SPEED_DOWN_FOR_SENSOR_RANGE:
                    eventId = "E72533";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_PLANNED_EVENT:
                    eventId = "E72534";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_IC_JC_AHEAD:
                    eventId = "E72611";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_TOLLGATE_AHEAD:
                    eventId = "E72612";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_RC_AREA_HEAD:
                    eventId = "E72613";
                    break;
                case Group2AdasWarning1_2Status::EMERGENCY_CORRIDOR:
                    eventId = "E72614";
                    break;
                case Group2AdasWarning1_2Status::LOW_SPEED_TO_TRAFFIC_FLOW:
                    eventId = "E72615";
                    break;
                case Group2AdasWarning1_2Status::SPEED_DOWN_FOR_NIGHT:
                    eventId = "E72616";
                    break;
                case Group2AdasWarning1_2Status::LIMIT_SPEED_FOR_MERGING_LANE:
                    eventId = "E72617";
                    break;
                case Group2AdasWarning1_2Status::LIMIT_SPEED_FOR_ROAD_CONSTRUCTION:
                    eventId = "E72618";
                    break;
                case Group2AdasWarning1_2Status::LIMIT_SPEED_FOR_SAFETY:
                    eventId = "E72619";
                    break;
                case Group2AdasWarning1_2Status::INDUCE_HDP_LONGKEY_INPUT:
                    eventId = "E72527";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_TUNNEL_AHEAD:
                    eventId = "E72635";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_CHECKPOINTS_AHEAD:
                    eventId = "E72636";
                    break;
                case Group2AdasWarning1_2Status::INFORM_TO_UNSUPPORTED_LANE_AHEAD:
                    eventId = "E72639";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_2Group2.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_2Group2Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group2ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_2Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_2Group2Stat::ON);
        }
    }

    // 5.10.5.2.3.8 2그룹 미니 팝업 1-3
    void update2Group1_3Event() {
        std::string eventId;

        if (mIsIgnOn == true && isHudSccAdasInfoOnOff() == true) {
            switch (mGroup2AdasWarning1_3Status) {
                case Group2AdasWarning1_3Status::STEERING_CONTROL_DETECTED:
                    eventId = "E72644";
                    break;
                case Group2AdasWarning1_3Status::ACCELERATOR_PEDAL_CONTROL_DETECTED:
                    eventId = "E72645";
                    break;
                case Group2AdasWarning1_3Status::CONDITION_NOT_MET:
                    eventId = "E72646";
                    break;
                case Group2AdasWarning1_3Status::MAXIMUM_SPEED_EXCEEDED:
                    eventId = "E72647";
                    break;
                case Group2AdasWarning1_3Status::MAXIMUM_SET_SPEED_EXCEEDED:
                    eventId = "E72648";
                    break;
                case Group2AdasWarning1_3Status::IC_JC_AHEAD:
                    eventId = "E72649";
                    break;
                case Group2AdasWarning1_3Status::TOLLGATE_AHEAD:
                    eventId = "E72650";
                    break;
                case Group2AdasWarning1_3Status::NOT_SUPPORTED_AREA_AHEAD:
                    eventId = "E72651";
                    break;
                case Group2AdasWarning1_3Status::NOT_SUPPORTED_ROAD_AHEAD:
                    eventId = "E72652";
                    break;
                case Group2AdasWarning1_3Status::CURVED_ROAD:
                    eventId = "E72653";
                    break;
                case Group2AdasWarning1_3Status::PEDESTRIAN_OR_CYCLIST_DETECTED:
                    eventId = "E72654";
                    break;
                case Group2AdasWarning1_3Status::LANE_DEPARTURE_DETECTED:
                    eventId = "E72655";
                    break;
                case Group2AdasWarning1_3Status::DRIVER_NOT_DETECTED:
                    eventId = "E72656";
                    break;
                case Group2AdasWarning1_3Status::DEACTIVATION_LANE_FOLLOWING_ASSIST:
                    eventId = "E72657";
                    break;
                case Group2AdasWarning1_3Status::DEACTIVATION_HIGHWAY_DRIVING_ASSIST:
                    eventId = "E72658";
                    break;
                case Group2AdasWarning1_3Status::SUGGEST_LANE_CHANGE_TO_LEFT_SIDE:
                    eventId = "E72659";
                    break;
                case Group2AdasWarning1_3Status::SUGGEST_LANE_CHANGE_TO_RIGHT_SIDE:
                    eventId = "E72660";
                    break;
                case Group2AdasWarning1_3Status::TUNNEL_AHEAD:
                    eventId = "E72661";
                    break;
                case Group2AdasWarning1_3Status::UNSUPPORTED_LANE_AHEAD:
                    eventId = "E72662";
                    break;
                case Group2AdasWarning1_3Status::UNSUPPORTED_WEATHER_CONDITIONS:
                    eventId = "E72663";
                    break;
                case Group2AdasWarning1_3Status::ROAD_CONSTRUCTION_AHEAD:
                    eventId = "E72664";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.Event1_3Group2.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_3Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_3Group2Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_NEWEventEvent1_3Group2ID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventEvent1_3Group2Stat(SFCDetail_Contents_ADAS_NEWEventEvent1_3Group2Stat::ON);
        }
    }

    // 5.10.5.2.4 CRAWL 팝업
    void updateCrawlWarnEvent() {
        std::string eventId;

        if (mIsIgnOn == true) {
            switch (mCrawlWarnStatus) {
                case CrawlWarnStatus::CONDITION_NOT_MET_STOP:
                    eventId = "E70212";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_DOOR:
                    eventId = "E70213";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_BELT:
                    eventId = "E70214";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_TRAILER:
                    eventId = "E70215";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_AUTOHOLD:
                    eventId = "E70216";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_BRAKE:
                    eventId = "E70217";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_TOW:
                    eventId = "E70218";
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_OTHERS:
                    eventId = "E70219";
                    break;
                case CrawlWarnStatus::READY_FOOT_OFF:
                    eventId = "E70220";
                    break;
                case CrawlWarnStatus::DISABLE_TEMPORARY:
                    eventId = "E70221";
                    break;
                case CrawlWarnStatus::DISABLE_TEMPORARY_OVERSPEED:
                    eventId = "E70222";
                    break;
                case CrawlWarnStatus::UNSTUCK_CHANGE_STEP:
                    eventId = "E70223";
                    break;
                case CrawlWarnStatus::DISABLE_WARN:
                    eventId = "E70224";
                    break;
                case CrawlWarnStatus::DISABLE_WARN_OVERSPEED:
                    eventId = "E70225";
                    break;
                case CrawlWarnStatus::DISABLE_WARN_TRANSMISSION:
                    eventId = "E70226";
                    break;
                case CrawlWarnStatus::DISABLE_WARN_MANUAL:
                    eventId = "E70227";
                    break;
                case CrawlWarnStatus::DISABLE_WARN_BRAKE:
                    eventId = "E70228";
                    break;
                case CrawlWarnStatus::DISABLE_WARN_OVERTIME:
                    eventId = "E70229";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_NEW.Event.CrawlWarn.ID, prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_NEWEventCrawlWarnStat(SFCDetail_Contents_ADAS_NEWEventCrawlWarnStat::OFF);
            flushLastGroup();
        }
        if (eventId.empty() == false) {
            setSFCDetail_Contents_ADAS_NEWEventCrawlWarnID(eventId);
            setSFCDetail_Contents_ADAS_NEWEventCrawlWarnStat(SFCDetail_Contents_ADAS_NEWEventCrawlWarnStat::ON);
        }
    }

    void onOneShotTimerSummaryEventExpired() {
        setSFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat(SFCDetail_Contents_ADAS_NEWEventCcMslaSummaryStat::OFF);
        updateCcMslaSpeed(true);
        updateConstantNaviCruiseStat(true);
        updateAdasSummaryInfo(true);
        flush();
    }

    inline HBool isHudSccAdasInfoOnOff() const {
        return (mHudSccInfoOnOffStatus == HudSccInfoOnOffStatus::ENABLE ||
                mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE);
    }

    inline HBool isCrawlActivationStatus() const {
        return (mCrawlActiveStatus == CrawlActiveStatus::READY) || (mCrawlActiveStatus == CrawlActiveStatus::ACTIVE) ||
               (mCrawlActiveStatus == CrawlActiveStatus::OVERRIDE) || (mCrawlActiveStatus == CrawlActiveStatus::PAUSE) ||
               (mCrawlActiveStatus == CrawlActiveStatus::TEMP);
    }

    inline HBool isCrawlActivationStep() const {
        return (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP1) ||
               (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP2) ||
               (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP3) ||
               (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP4) ||
               (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP5);
    }

    // 5.10.5.2.1 ADAS 요약 정보 표시 - ADAS 주행 보조 요약 정보
    HBool isAdasSummaryOn() {
        HBool isAdasSummaryStatusOn = false;

        if (mIsAdasCruiseStatOn == true || mIsAdasSetSpeedStatOn == true || mIsHdaHdpTelltaleStatOn == true ||
            mIsNsccAutoSetTelltaleStatOn == true || mIsIslaManualSetSpeedStatOn == true ||
            mIsIslaManualSetSpeedForCcMslaStatOn == true) {
            isAdasSummaryStatusOn = true;
        }

        return isAdasSummaryStatusOn;
    }

    using CcActiveStat = ArgumentsCcMslaActiveStatChanged::Inter_CcActiveStat;
    using MslaActiveStat = ArgumentsCcMslaActiveStatChanged::Inter_MslaActiveStat;
    using ConfigAdasScc = ArgumentsConfigSccChanged::Inter_ConfigAdasSCC;
    using MslaOverride = ArgumentsMslaOverrideChanged::Inter_Mslaoverride;
    using CrawlActiveStatus = ArgumentsCrawlActiveStatusChanged::Input_CrawlActiveStatus;
    using CrawlActiveStepStatus = ArgumentsCrawlActiveStepStatusChanged::Input_CrawlActiveStepStatus;
    using CruiseMainIndReqStatus = ArgumentsCcMslaChanged::Input_CruiseMainIndReqStatus;
    using CruiseOverrideReqStatus = ArgumentsCcMslaChanged::Input_CruiseOverrideReqStatus;
    using SpeedLimitActiveStatus = ArgumentsCcMslaChanged::Input_SpeedLimitActiveStatus;
    using SpeedLimitReadyStatus = ArgumentsCcMslaChanged::Input_SpeedLimitReadyStatus;
    using MvCenterRoadSurfaceStatus = ArgumentsCenterRoadChanged::Input_MvCenterRoadSurfaceStatus;
    using MvFrontLeftObjectStatus = ArgumentsFrontLeftObjectChanged::Input_MvFrontLeftObjectStatus;
    using MvFrontObjectStatus = ArgumentsFrontObjectChanged::Input_MvFrontObjectStatus;
    using MvFrontRightObjectStatus = ArgumentsFrontRightObjectChanged::Input_MvFrontRightObjectStatus;
    using MvLeftLaneChangeDirectionStatus = ArgumentsLeftLineChanged::Input_MvLeftLaneChangeDirectionStatus;
    using MvLeftLineStatus = ArgumentsLeftLineChanged::Input_MvLeftLineStatus;
    using MvLeftRoadSurfaceStatus = ArgumentsLeftLineChanged::Input_MvLeftRoadSurfaceStatus;
    using MvLeftObjectStatus = ArgumentsLeftObjectStatusChanged::Input_MvLeftObjectStatus;
    using MvFrontRadarWaveStatus = ArgumentsRadarWaveStatusChanged::Input_MvFrontRadarWaveStatus;
    using MvRearLeftRadarWave1Status = ArgumentsRadarWaveStatusChanged::Input_MvRearLeftRadarWave1Status;
    using MvRearLeftRadarWave2Status = ArgumentsRadarWaveStatusChanged::Input_MvRearLeftRadarWave2Status;
    using MvRearRightRadarWave1Status = ArgumentsRadarWaveStatusChanged::Input_MvRearRightRadarWave1Status;
    using MvRearRightRadarWave2Status = ArgumentsRadarWaveStatusChanged::Input_MvRearRightRadarWave2Status;
    using MvRightLaneChangeDirectionStatus = ArgumentsRightLineChanged::Input_MvRightLaneChangeDirectionStatus;
    using MvRightLineStatus = ArgumentsRightLineChanged::Input_MvRightLineStatus;
    using MvRightRoadSurfaceStatus = ArgumentsRightLineChanged::Input_MvRightRoadSurfaceStatus;
    using MvRightObjectStatus = ArgumentsRightObjectStatusChanged::Input_MvRightObjectStatus;
    using MvFrontObstacleStatus = ArgumentsMvFrontObstacleStatusChanged::Input_MvFrontObstacleStatus;
    using MvVehicleDistanceStatus = ArgumentsVehicleDistanceChanged::Input_MvVehicleDistanceStatus;
    using NccCameraOperStatus = ArgumentsVehicleDistanceChanged::Input_NccCameraOperStatus;
    using SccOperatingStatus = ArgumentsCruiseSymbolChanged::Input_SccOperatingStatus;
    using SmvHdaSymbolStatus = ArgumentsSmvChanged::Input_SmvHdaSymbolStatus;
    using SmvIslaSetSpeedSymbolStatus = ArgumentsSmvChanged::Input_SmvIslaSetSpeedSymbolStatus;
    using MvLaneChangeRectangleStatus = ArgumentsMvLaneChangedRectangleStatus::Input_MvLaneChangeRectangleStatus;
    using MvFrontObject01Status = ArgumentsIsMvFrontObject01StatusChanged::Input_MvFrontObject01Status;
    using MvFrontObjectClassification01Status =
        ArgumentsIsMvFrontObjectClassification01StatusChanged::Input_MvFrontObjectClassification01Status;
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
    using SmvNsccSymbolStatus = ArgumentsSmvChanged::Input_SmvNsccSymbolStatus;
    using SmvDrvAsstHUDSymbStatus = ArgumentsSmvChanged::Input_SmvDrvAsstHUDSymbStatus;
    using SmvSetSpeedStatus = ArgumentsSmvChanged::Input_SmvSetSpeedStatus;
    using SmvLfaSymbolStatus = ArgumentsSmvLfaSymbolStatusChanged::Input_SmvLfaSymbolStatus;
    using MvDriverHandsOnOffStatus = ArgumentsMvDriverHandsOnOffStatusChanged::Input_MvDriverHandsOnOffStatus;
    using CruiseInfoDisplayReqStatus = ArgumentsInfoDisplayReqChanged::Input_CruiseInfoDisplayReqStatus;
    using SpeedLimitInfoDisplayReqStatus = ArgumentsInfoDisplayReqChanged::Input_SpeedLimitInfoDisplayReqStatus;
    using HdpRDDWarningState = ArgumentsHdpRDDWarningStateChanged::Input_HDP_RDDWarningState;
    using EscHDPEngagedStatus = ArgumentsEscHDPEngagedStatusChanged::Input_ESC_HDPEngagedStatus;
    using RcuHDPEngagedStatus = ArgumentsRcuHDPEngagedStatusChanged::Input_RCU_HDPEngagedStatus;
    using CcSpeedStat = SFCDetail_Contents_ADAS_NEWConstantCcSpeedStat;
    using MslaSpeedStat = SFCDetail_Contents_ADAS_NEWConstantMslaSpeedStat;
    using Group1AdasWarning1_1Status = ArgumentsGroup1AdasWarning1_1StatusChanged::Input_PuFGroup1AdasWarning1_1Status;
    using Group1AdasWarning1_2Status = ArgumentsGroup1AdasWarning1_2StatusChanged::Input_PuFGroup1AdasWarning1_2Status;
    using Group4AdasWarning1_1Status = ArgumentsGroup4AdasWarning1_1StatusChanged::Input_PuFGroup4AdasWarning1_1Status;
    using Group2AdasWarning1_1Status = ArgumentsGroup2AdasWarning1_1StatusChanged::Input_PuMGroup2AdasWarning1_1Status;
    using Group2AdasWarning1_2Status = ArgumentsGroup2AdasWarning1_2StatusChanged::Input_PuMGroup2AdasWarning1_2Status;
    using Group2AdasWarning1_3Status = ArgumentsGroup2AdasWarning1_3StatusChanged::Input_PuMGroup2AdasWarning1_3Status;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigHUD = ArgumentsConfigOnChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigOnChanged::Inter_ConfigNewHUD;
    using ConfigHDPName = ArgumentsConfigHDPNameChanged::Inter_ConfigHDPName;
    using HudBcwInfoOnOffStatus = ArgumentsHudBcwInfoOnOffStatusChanged::Inter_HudBcwInfoOnOffStatus;
    using HudSccInfoOnOffStatus = ArgumentsHudSccAdasInfoOnOffStatusChanged::Inter_HudSCCInfoOnOffStatus;
    using HudAdasInfoOnOffStatus = ArgumentsHudSccAdasInfoOnOffStatusChanged::Inter_HudAdasInfoOnOffStatus;
    using HudBcwUnderAdasInfoOnOffStatus = ArgumentsHudBcwUnderAdasInfoOnOffStatusChanged::Inter_HudBcwUnderAdasInfoOnOffStatus;
    using CcOnOffStat = ArgumentsCcStatChanged::Inter_CcOnOffStat;
    using MslaOnOffStat = ArgumentsMslaStatChanged::Inter_MslaOnOffStat;
    using CruiseSetIndReqStatus = ArgumentsCcSetIndStatusChanged::Input_CruiseSetIndReqStatus;
    using CrawlWarnStatus = ArgumentsCrawlWarnStatusChanged::Input_CrawlWarnStatus;

    CcActiveStat mCcActiveStat = CcActiveStat::OFF;
    MslaActiveStat mMslaActiveStat = MslaActiveStat::OFF;
    ConfigAdasScc mConfigAdasScc = ConfigAdasScc::OFF;
    MslaOverride mMslaOverride = MslaOverride::OFF;
    CrawlActiveStatus mCrawlActiveStatus = CrawlActiveStatus::OFF;
    CrawlActiveStepStatus mCrawlActiveStepStatus = CrawlActiveStepStatus::OFF;
    CruiseMainIndReqStatus mCruiseMainIndReqStatus = CruiseMainIndReqStatus::OFF;
    CruiseMainIndReqStatus mPrevCruiseMainIndReqStatus = CruiseMainIndReqStatus::OFF;
    CruiseOverrideReqStatus mCruiseOverrideReqStatus = CruiseOverrideReqStatus::OFF;
    SpeedLimitActiveStatus mSpeedLimitActiveStatus = SpeedLimitActiveStatus::OFF;
    SpeedLimitActiveStatus mPrevSpeedLimitActiveStatus = SpeedLimitActiveStatus::OFF;
    SpeedLimitReadyStatus mSpeedLimitReadyStatus = SpeedLimitReadyStatus::NORMAL;
    MvCenterRoadSurfaceStatus mMvCenterRoadSurfaceStatus = MvCenterRoadSurfaceStatus::OFF;
    MvFrontLeftObjectStatus mMvFrontLeftObjectStatus = MvFrontLeftObjectStatus::OFF;
    MvFrontLeftObjectStatus mPrevMvFrontLeftObjectStatus = MvFrontLeftObjectStatus::OFF;
    MvFrontObjectStatus mMvFrontObjectStatus = MvFrontObjectStatus::OFF;
    MvFrontRightObjectStatus mMvFrontRightObjectStatus = MvFrontRightObjectStatus::OFF;
    MvFrontRightObjectStatus mPrevMvFrontRightObjectStatus = MvFrontRightObjectStatus::OFF;
    MvLeftLaneChangeDirectionStatus mMvLeftLaneChangeDirectionStatus = MvLeftLaneChangeDirectionStatus::OFF;
    MvLeftLineStatus mMvLeftLineStatus = MvLeftLineStatus::OFF;
    MvLeftRoadSurfaceStatus mMvLeftRoadSurfaceStatus = MvLeftRoadSurfaceStatus::OFF;
    MvLeftObjectStatus mMvLeftObjectStatus = MvLeftObjectStatus::OFF;
    MvLeftObjectStatus mPrevMvLeftObjectStatus = MvLeftObjectStatus::OFF;
    MvFrontRadarWaveStatus mMvFrontRadarWaveStatus = MvFrontRadarWaveStatus::OFF;
    MvRearLeftRadarWave1Status mMvRearLeftRadarWave1Status = MvRearLeftRadarWave1Status::OFF;
    MvRearLeftRadarWave2Status mMvRearLeftRadarWave2Status = MvRearLeftRadarWave2Status::OFF;
    MvRearRightRadarWave1Status mMvRearRightRadarWave1Status = MvRearRightRadarWave1Status::OFF;
    MvRearRightRadarWave2Status mMvRearRightRadarWave2Status = MvRearRightRadarWave2Status::OFF;
    MvRightLaneChangeDirectionStatus mMvRightLaneChangeDirectionStatus = MvRightLaneChangeDirectionStatus::OFF;
    MvRightLineStatus mMvRightLineStatus = MvRightLineStatus::OFF;
    MvRightRoadSurfaceStatus mMvRightRoadSurfaceStatus = MvRightRoadSurfaceStatus::OFF;
    MvRightObjectStatus mMvRightObjectStatus = MvRightObjectStatus::OFF;
    MvRightObjectStatus mPrevMvRightObjectStatus = MvRightObjectStatus::OFF;
    MvFrontObstacleStatus mMvFrontObstacleStatus = MvFrontObstacleStatus::OFF;
    MvVehicleDistanceStatus mMvVehicleDistanceStatus = MvVehicleDistanceStatus::OFF;
    NccCameraOperStatus mNccCameraOperStatus = NccCameraOperStatus::OFF;
    SccOperatingStatus mSccOperatingStatus = SccOperatingStatus::OFF;
    SmvHdaSymbolStatus mSmvHdaSymbolStatus = SmvHdaSymbolStatus::OFF;
    SmvIslaSetSpeedSymbolStatus mSmvIslaSetSpeedSymbolStatus = SmvIslaSetSpeedSymbolStatus::OFF;
    MvLaneChangeRectangleStatus mMvLaneChangeRectangleStatus = MvLaneChangeRectangleStatus::OFF;
    MvFrontObject01Status mMvFrontObject01Status = MvFrontObject01Status::OFF;
    MvFrontObjectClassification01Status mMvFrontObjectClassification01Status = MvFrontObjectClassification01Status::OFF;
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
    SmvNsccSymbolStatus mSmvNsccSymbolStatus = SmvNsccSymbolStatus::OFF;
    SmvDrvAsstHUDSymbStatus mSmvDrvAsstHUDSymbStatus = SmvDrvAsstHUDSymbStatus::OFF;
    SmvSetSpeedStatus mSmvSetSpeedStatus = SmvSetSpeedStatus::OFF;
    SmvLfaSymbolStatus mSmvLfaSymbolStatus = SmvLfaSymbolStatus::OFF;
    MvDriverHandsOnOffStatus mMvDriverHandsOnOffStatus = MvDriverHandsOnOffStatus::OFF;
    CruiseInfoDisplayReqStatus mCruiseInfoDisplayReqStatus = CruiseInfoDisplayReqStatus::OFF;
    SpeedLimitInfoDisplayReqStatus mSpeedLimitInfoDisplayReqStatus = SpeedLimitInfoDisplayReqStatus::OFF;
    HdpRDDWarningState mHdpRDDWarningState = HdpRDDWarningState::INVALID;
    EscHDPEngagedStatus mEscHDPEngagedStatus = EscHDPEngagedStatus::INVALID;
    RcuHDPEngagedStatus mRcuHDPEngagedStatus = RcuHDPEngagedStatus::INVALID;
    Group1AdasWarning1_1Status mGroup1AdasWarning1_1Status = Group1AdasWarning1_1Status::OFF;
    Group1AdasWarning1_2Status mGroup1AdasWarning1_2Status = Group1AdasWarning1_2Status::OFF;
    Group4AdasWarning1_1Status mGroup4AdasWarning1_1Status = Group4AdasWarning1_1Status::OFF;
    Group2AdasWarning1_1Status mGroup2AdasWarning1_1Status = Group2AdasWarning1_1Status::OFF;
    Group2AdasWarning1_2Status mGroup2AdasWarning1_2Status = Group2AdasWarning1_2Status::OFF;
    Group2AdasWarning1_3Status mGroup2AdasWarning1_3Status = Group2AdasWarning1_3Status::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    ConfigHDPName mConfigHDPName = ConfigHDPName::TYPE_A;
    HudBcwInfoOnOffStatus mHudBcwInfoOnOffStatus = HudBcwInfoOnOffStatus::DISABLE;
    HudSccInfoOnOffStatus mHudSccInfoOnOffStatus = HudSccInfoOnOffStatus::DISABLE;
    HudBcwUnderAdasInfoOnOffStatus mHudBcwUnderAdasInfoOnOffStatus = HudBcwUnderAdasInfoOnOffStatus::DISABLE;
    HudAdasInfoOnOffStatus mHudAdasInfoOnOffStatus = HudAdasInfoOnOffStatus::DISABLE;
    CcOnOffStat mCcOnOffStat = CcOnOffStat::OFF;
    MslaOnOffStat mMslaOnOffStat = MslaOnOffStat::OFF;
    CruiseSetIndReqStatus mCruiseSetIndReqStatus = CruiseSetIndReqStatus::UNHANDLED_TIMEOUT;
    CrawlWarnStatus mCrawlWarnStatus = CrawlWarnStatus::OFF;

    HBool mIsIgnOn = false;
    HBool mIsConfigOn = false;

    HUInt64 mPrivateCcSldSetSpeedValue = 0;
    HUInt64 mCcOnlySetSpeedValue = 0;
    HUInt64 mCcSldSetSpeedValue = 0;
    HUInt64 mMvCenterRoadSurfaceDistanceValue = 0;
    HUInt64 mMvLeftLineOffsetValue = 0;
    HUInt64 mMvRightLineOffsetValue = 0;
    HUInt64 mMvDrivingLaneRadiusValue = 0;
    HUInt64 mMvFrontLeftObjectLateralPositionValue = 0;
    HUInt64 mMvFrontLeftObjectLongitudinalPositionValue = 0;
    HUInt64 mMvFrontObjectLateralPositionValue = 0;
    HUInt64 mMvFrontObjectLongitudinalPositionValue = 0;
    HUInt64 mMvFrontRightObjectLateralPositionValue = 0;
    HUInt64 mMvFrontRightObjectLongitudinalPositionValue = 0;
    HUInt64 mMvLeftObjectLateralPositionValue = 0;
    HUInt64 mMvLeftObjectLongitudinalPositionValue = 0;
    HUInt64 mMvRightObjectLateralPositionValue = 0;
    HUInt64 mMvRightObjectLongitudinalPositionValue = 0;
    HUInt64 mMvFrontObstacleLateralPositionValue = 0;
    HUInt64 mMvFrontObstacleLongitudinalPositionValue = 0;
    HUInt64 mMvLaneChangeRectangleLateralPositionValue = 0;
    HUInt64 mMvVehicleDistanceValue = 0;
    HUInt64 mSccTargetSpeedSettingValue = 0;
    HUInt64 mSmvSetSpeedValue = 0;
    HUInt64 mCcSldMaxSpeedValue = 0;
    HUInt64 mMvFrontObjectLateralPosition01Value = 0;
    HUInt64 mMvFrontObjectLongitudinalPosition01Value = 0;
    HUInt64 mMvFrontObjectHeadingAngle01Value = 0;
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
    ssfs::SFCTimer<Detail_Contents_ADAS_NEW> mOneShotTimerSummaryEventExpired;

    CcSpeedStat mCcSpeedStat = CcSpeedStat::NONE;
    MslaSpeedStat mMslaSpeedStat = MslaSpeedStat::NONE;

    HBool mIsAdasCruiseStatOn = false;
    HBool mIsAdasSetSpeedStatOn = false;
    HBool mIsHdaHdpTelltaleStatOn = false;
    HBool mIsIslaManualSetSpeedStatOn = false;
    HBool mIsIslaManualSetSpeedForCcMslaStatOn = false;
    HBool mIsNsccAutoSetTelltaleStatOn = false;

    static constexpr uint32_t kTimerSummaryEventExpiredInterval160ms = 160u;
    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_ADAS_NEW_H
