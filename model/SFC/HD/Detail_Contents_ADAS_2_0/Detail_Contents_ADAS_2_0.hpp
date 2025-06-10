/**
 * @file Detail_Contents_ADAS_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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
#ifndef SFSS_Detail_Contents_ADAS_2_0_H
#define SFSS_Detail_Contents_ADAS_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_ADAS_2_0Base.hpp"

namespace ccos {

// SFC Version : 17.2.0
// Reference : [HD171] Detail_Contents_ADAS_2_0. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Detail_Contents_ADAS_2_0 : public Detail_Contents_ADAS_2_0Base {
public:
    Detail_Contents_ADAS_2_0() = default;
    ~Detail_Contents_ADAS_2_0() override = default;
    Detail_Contents_ADAS_2_0(const Detail_Contents_ADAS_2_0& other) = delete;
    Detail_Contents_ADAS_2_0(Detail_Contents_ADAS_2_0&& other) noexcept = delete;
    Detail_Contents_ADAS_2_0& operator=(const Detail_Contents_ADAS_2_0& other) = delete;
    Detail_Contents_ADAS_2_0& operator=(Detail_Contents_ADAS_2_0&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDetail_Contents_ADAS_2_0EventCrawlID("E71200");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantNaviCruiseStatus();
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        if (mIsConfigOn != true) {
            return;
        }

        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigAdasScc = args.mInter_ConfigAdasSCC;
        mIsConfigOn = (mConfigNewHUD == ConfigNewHUD::ON || mConfigHUD == ConfigHUD::ON);

        if (mIsConfigOn != true) {
            return;
        }

        updatePrivateCcSldSetSpeedValue();
        updateConstantNaviCruiseStatus();
        updateAll();
    }

    void onCcColorStatChanged(const ArgumentsCcColorStatChanged& args) {
        mCcColorStat = args.mCcColorStat;

        if (mIsConfigOn != true) {
            return;
        }

        updatePrivateCcSldSetSpeedValue();
    }

    void onHudAdasInfoOnOffStatusChanged(const ArgumentsHudAdasInfoOnOffStatusChanged& args) {
        mHudAdasInfoOnOffStatus = args.mInter_HudAdasInfoOnOffStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantNsccSymbolStatus();
        updateConstantSccSymbolStatus();
        updateConstantADASSetSpeedStatus();
        updateConstantADASSetSpeedValue();
        updateConstantLfaHandsOnOffTelltaleStatus();
        updateConstantLCALeftTelltaleStatus();
        updateConstantLCARightTelltaleStatus();
        updateConstantADASLeftLaneStatus();
        updateConstantADASRightLaneStatus();
        updateConstantADASHeadwayDistStatus();
        updateConstantADASVehicleDistanceSetValue();
        updateConstantADASCenterRoadStatus();
        updateConstantADASCenterRoadLeftLatPosValue();
        updateConstantADASCenterRoadRightLatPosValue();
        updateConstantADASRoadCvrtLvlValue();
        updateConstantADASLeftLeftLaneStat();
        updateConstantADASRightRightLaneStat();
        updateConstantADASViewFrontLeftCollisionWarningStatus();
        updateConstantADASViewFrontRightCollisionWarningStatus();
        updateConstantADASViewFrontWarnStatus();
        updateConstantADASViewFrontLeftWarnStatus();
        updateConstantADASViewFrontRightWarnStatus();
        updateConstantADASViewRearLeftWarnStatus();
        updateConstantADASViewRearRightWarnStatus();
        updateConstantADASViewLeftArrowStatus();
        updateConstantADASViewLeftArrowLatValue();
        updateConstantADASViewRightArrowStatus();
        updateConstantADASViewRightArrowLatValue();
        updateConstantADASViewLaneChangeRectangle();
        updateConstantADASViewLaneChangeRectangleLatValue();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor02Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor03Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor04Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor05Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor06Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor07Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor08Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor09Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor10Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor11Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor12Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor13Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor14Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass02Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass03Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass04Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass05Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass06Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass07Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass08Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass09Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass10Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass11Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass12Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass13Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass14Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat>();
        updateConstantADASViewEmergencySteeringStat();
        updateEvent1GroupPopup1_2();
        updateEvent4GroupPopup1_1();
        updateEvent2GroupPopup1_1();
        updateEvent2GroupPopup1_2();
        updateEventLargeFullPopup();
    }

    void onCcOnlySetSpeedValueChanged(const ArgumentsCcOnlySetSpeedValueChanged& args) {
        if (ISTIMEOUT(args.mInput_CcOnlySetSpeedValue) == true) {
            mCcOnlySetSpeedValue = kTimeOut;
        } else {
            mCcOnlySetSpeedValue = args.mInput_CcOnlySetSpeedValue;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updatePrivateCcSldSetSpeedValue();
    }

    void onCcSldSetSpeedValueChanged(const ArgumentsCcSldSetSpeedValueChanged& args) {
        if (ISTIMEOUT(args.mInput_CcSldSetSpeedValue) == true) {
            mCcSldSetSpeedValue = kTimeOut;
        } else {
            mCcSldSetSpeedValue = args.mInput_CcSldSetSpeedValue;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updatePrivateCcSldSetSpeedValue();
        updateConstantCcMslaSpeedStatus();
    }

    void onCcSldMaxSpeedValueChanged(const ArgumentsCcSldMaxSpeedValueChanged& args) {
        mCcSldMaxSpeedValue = args.mInter_CcSldMaxSpeedValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantCcMslaSpeedStatus();
    }

    void onCcChanged(const ArgumentsCcChanged& args) {
        mCcOnOffStat = args.mInter_CcOnOffStat;
        mCruiseSetIndReqStatus = args.mInput_CruiseSetIndReqStatus;
        mCcActiveStat = args.mInter_CcActiveStat;
        mCruiseOverrideReqStatus = args.mInput_CruiseOverrideReqStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantCcSymbol();
        updateConstantCcMslaSpeedStatus();
    }

    void onMslaChanged(const ArgumentsMslaChanged& args) {
        mMslaOnOffStat = args.mInter_MslaOnOffStat;
        mSpeedLimitReadyStatus = args.mInput_SpeedLimitReadyStatus;
        mSmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        mMslaActiveStat = args.mInter_MslaActiveStat;
        mMslaOverride = args.mInter_Mslaoverride;

        if (ISTIMEOUT(args.mInput_SmvSetSpeedStatus) == true) {
            mSmvSetSpeedStatus = SmvSetSpeedStatus::UNHANDLED_TIMEOUT;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantMslaSymbol();
        updateConstantCcMslaSpeedStatus();
    }

    void onNccCameraOperStatusChanged(const ArgumentsNccCameraOperStatusChanged& args) {
        mNccCameraOperStatus = args.mInput_NccCameraOperStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantNaviCruiseStatus();
    }

    void onCrawlActiveChanged(const ArgumentsCrawlActiveChanged& args) {
        mCrawlActiveStatus = args.mInput_CrawlActiveStatus;
        mCrawlActiveStepStatus = args.mInput_CrawlActiveStepStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantCrawlOperation();
        updateEventCrawlSummary();
    }

    void onIndNsccSymbolStatusChanged(const ArgumentsIndNsccSymbolStatusChanged& args) {
        mINDNsccSymbolStatus = args.mInput_INDNsccSymbolStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantNsccSymbolStatus();
    }

    void onIndSccSymbolStatusChanged(const ArgumentsIndSccSymbolStatusChanged& args) {
        mINDSccSymbol1Status = args.mInput_INDSccSymbol1Status;
        mINDSccSymbol2Status = args.mInput_INDSccSymbol2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantSccSymbolStatus();
    }

    void onIndSetSpeedStatusChanged(const ArgumentsIndSetSpeedStatusChanged& args) {
        mINDSetSpeed1Status = args.mInput_INDSetSpeed1Status;
        mINDSetSpeed2Status = args.mInput_INDSetSpeed2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASSetSpeedStatus();
    }

    void onIndSetSpeedValueChanged(const ArgumentsIndSetSpeedValueChanged& args) {
        mINDSetSpeed1Value = args.mInput_INDSetSpeed1Value;
        mINDSetSpeed2Value = args.mInput_INDSetSpeed2Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASSetSpeedValue();
    }

    void onIndLfaSymbolStatusChanged(const ArgumentsIndLfaSymbolStatusChanged& args) {
        mINDLfaSymbol1Status = args.mInput_INDLfaSymbol1Status;
        mINDLfaSymbol2Status = args.mInput_INDLfaSymbol2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantLfaHandsOnOffTelltaleStatus();
    }

    void onIndDriverHandsOnOffStatusChanged(const ArgumentsIndDriverHandsOnOffStatusChanged& args) {
        mINDDriverHandsOnOff1Status = args.mInput_INDDriverHandsOnOff1Status;
        mINDDriverHandsOnOff2Status = args.mInput_INDDriverHandsOnOff2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantLfaHandsOnOffTelltaleStatus();
        updateConstantLCALeftTelltaleStatus();
        updateConstantLCARightTelltaleStatus();
    }

    void onIndLcaLeftSymbolStatusChanged(const ArgumentsIndLcaLeftSymbolStatusChanged& args) {
        mINDLcaLeftSymbol1Status = args.mInput_INDLcaLeftSymbol1Status;
        mINDLcaLeftSymbol2Status = args.mInput_INDLcaLeftSymbol2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantLCALeftTelltaleStatus();
    }

    void onIndLcaRightSymbolStatusChanged(const ArgumentsIndLcaRightSymbolStatusChanged& args) {
        mINDLcaRightSymbol1Status = args.mInput_INDLcaRightSymbol1Status;
        mINDLcaRightSymbol2Status = args.mInput_INDLcaRightSymbol2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantLCARightTelltaleStatus();
    }

    void onVcLeftLineStatusChanged(const ArgumentsVcLeftLineStatusChanged& args) {
        mVCLeftLineStatus = args.mInput_VCLeftLineStatus;
        mVCLeftLineOffsetValue = args.mInput_VCLeftLineOffsetValue;

        if (ISTIMEOUT(args.mInput_VCLeftLineStatus) == true) {
            mVCLeftLineStatus = VCLeftLineStatus::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_VCLeftLineOffsetValue) == true) {
            mVCLeftLineOffsetValue = kTimeOut;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASLeftLaneStatus();
        updateConstantADASLeftLineLatPosValue();
        updateConstantADASCenterRoadLeftLatPosValue();
    }

    void onVcDrivingLaneRadiusValueChanged(const ArgumentsVcDrivingLaneRadiusValueChanged& args) {
        mVCDrivingLaneRadiusValue = args.mInput_VCDrivingLaneRadiusValue;

        if (ISTIMEOUT(args.mInput_VCDrivingLaneRadiusValue) == true) {
            mVCDrivingLaneRadiusValue = kTimeOut;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASLaneCvrtLvlValue();
        updateConstantADASRoadCvrtLvlValue();
    }

    void onVcRightLineStatusChanged(const ArgumentsVcRightLineStatusChanged& args) {
        mVCRightLineStatus = args.mInput_VCRightLineStatus;
        mVCRightLineOffsetValue = args.mInput_VCRightLineOffsetValue;

        if (ISTIMEOUT(args.mInput_VCRightLineStatus) == true) {
            mVCRightLineStatus = VCRightLineStatus::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_VCRightLineOffsetValue) == true) {
            mVCRightLineOffsetValue = kTimeOut;
        }

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASRightLaneStatus();
        updateConstantADASRightLineLatPosValue();
        updateConstantADASCenterRoadRightLatPosValue();
    }

    void onVcLeftLeftLineStatusChanged(const ArgumentsVcLeftLeftLineStatusChanged& args) {
        mVCLeftLeftLineStatus = args.mInput_VCLeftLeftLineStatus;
        mVCLeftLeftLineOffsetValue = args.mInput_VCLeftLeftLineOffsetValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASLeftLeftLaneStat();
        updateConstantADASLeftLeftLineLatPosValue();
    }

    void onVcRightRightLineStatusChanged(const ArgumentsVcRightRightLineStatusChanged& args) {
        mVCRightRightLineStatus = args.mInput_VCRightRightLineStatus;
        mVCRightRightLineOffsetValue = args.mInput_VCRightRightLineOffsetValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASRightRightLaneStat();
        updateConstantADASRightRightLineLatPosValue();
    }

    void onVcVehicleDistanceChanged(const ArgumentsVcVehicleDistanceChanged& args) {
        mVCVehicleDistanceStatus = args.mInput_VCVehicleDistanceStatus;
        mVCVehicleDistanceValue = args.mInput_VCVehicleDistanceValue;
        mVCVehicleDistanceLevelValue = args.mInput_VCVehicleDistanceLevelValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASHeadwayDistStatus();
        updateConstantADASHeadwayDistValue();
        updateConstantADASVehicleDistanceSetValue();
    }

    void onVcCenterRoadSurfaceStatusChanged(const ArgumentsVcCenterRoadSurfaceStatusChanged& args) {
        mVCCenterRoadSurfaceStatus = args.mInput_VCCenterRoadSurfaceStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASCenterRoadStatus();
    }

    void onVcObjectMovingDirectionStatusChanged(const ArgumentsVcObjectMovingDirectionStatusChanged& args) {
        mVCFrontLeftObjectMovingDirectionStatus = args.mInput_VCFrontLeftObjectMovingDirectionStatus;
        mVCFrontRightObjectMovingDirectionStatus = args.mInput_VCFrontRightObjectMovingDirectionStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontLeftCollisionWarningStatus();
        updateConstantADASViewFrontRightCollisionWarningStatus();
    }

    void onRadarWaveStatusChanged(const ArgumentsRadarWaveStatusChanged& args) {
        mVCFrontRadarWaveStatus = args.mInput_VCFrontRadarWaveStatus;
        mVCFrontLeftRadarWaveStatus = args.mInput_VCFrontLeftRadarWaveStatus;
        mVCFrontRightRadarWaveStatus = args.mInput_VCFrontRightRadarWaveStatus;
        mVCRearLeftRadarWave1Status = args.mInput_VCRearLeftRadarWave1Status;
        mVCRearLeftRadarWave2Status = args.mInput_VCRearLeftRadarWave2Status;
        mVCRearRightRadarWave1Status = args.mInput_VCRearRightRadarWave1Status;
        mVCRearRightRadarWave2Status = args.mInput_VCRearRightRadarWave2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewFrontWarnStatus();
        updateConstantADASViewFrontLeftWarnStatus();
        updateConstantADASViewFrontRightWarnStatus();
        updateConstantADASViewRearLeftWarnStatus();
        updateConstantADASViewRearRightWarnStatus();
    }

    void onVcLeftLaneChangeDirectionStatusChanged(const ArgumentsVcLeftLaneChangeDirectionStatusChanged& args) {
        mVCLeftLaneChangeDirectionStatus = args.mInput_VCLeftLaneChangeDirectionStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLeftArrowStatus();
    }

    void onVcLaneChangeRectangleLateralPositionValueChanged(
        const ArgumentsVcLaneChangeRectangleLateralPositionValueChanged& args) {
        mVCLaneChangeRectangleLateralPositionValue = args.mInput_VCLaneChangeRectangleLateralPositionValue;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLeftArrowLatValue();
        updateConstantADASViewRightArrowLatValue();
        updateConstantADASViewLaneChangeRectangleLatValue();
    }

    void onVcRightLaneChangeDirectionStatusChanged(const ArgumentsVcRightLaneChangeDirectionStatusChanged& args) {
        mVCRightLaneChangeDirectionStatus = args.mInput_VCRightLaneChangeDirectionStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewRightArrowStatus();
    }

    void onVcLaneChangedRectangleStatus(const ArgumentsVcLaneChangedRectangleStatus& args) {
        mVCLaneChangeRectangleStatus = args.mInput_VCLaneChangeRectangleStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLaneChangeRectangle();
    }

    void onVcLaneChangeAvailableStatusChanged(const ArgumentsVcLaneChangeAvailableStatusChanged& args) {
        mVCLaneChangeAvailableStatus = args.mInput_VCLaneChangeAvailableStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewLaneChangeAvailableStatus();
    }

    void onVcSurroundingObject01to05StatusChanged(const ArgumentsVcSurroundingObject01to05StatusChanged& args) {
        mVCSurroundingObject01Status = args.mInput_VCSurroundingObject01Status;
        mVCSurroundingObject02Status = args.mInput_VCSurroundingObject02Status;
        mVCSurroundingObject03Status = args.mInput_VCSurroundingObject03Status;
        mVCSurroundingObject04Status = args.mInput_VCSurroundingObject04Status;
        mVCSurroundingObject05Status = args.mInput_VCSurroundingObject05Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor02Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor03Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor04Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor05Stat>();

        mPrevVCSurroundingObject01Status = mVCSurroundingObject01Status;
        mPrevVCSurroundingObject02Status = mVCSurroundingObject02Status;
        mPrevVCSurroundingObject03Status = mVCSurroundingObject03Status;
        mPrevVCSurroundingObject04Status = mVCSurroundingObject04Status;
        mPrevVCSurroundingObject05Status = mVCSurroundingObject05Status;
    }

    void onVcSurroundingObject06to10StatusChanged(const ArgumentsVcSurroundingObject06to10StatusChanged& args) {
        mVCSurroundingObject06Status = args.mInput_VCSurroundingObject06Status;
        mVCSurroundingObject07Status = args.mInput_VCSurroundingObject07Status;
        mVCSurroundingObject08Status = args.mInput_VCSurroundingObject08Status;
        mVCSurroundingObject09Status = args.mInput_VCSurroundingObject09Status;
        mVCSurroundingObject10Status = args.mInput_VCSurroundingObject10Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor06Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor07Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor08Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor09Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor10Stat>();

        mPrevVCSurroundingObject06Status = mVCSurroundingObject06Status;
        mPrevVCSurroundingObject07Status = mVCSurroundingObject07Status;
        mPrevVCSurroundingObject08Status = mVCSurroundingObject08Status;
        mPrevVCSurroundingObject09Status = mVCSurroundingObject09Status;
        mPrevVCSurroundingObject10Status = mVCSurroundingObject10Status;
        mPrevVCSurroundingObject11Status = mVCSurroundingObject11Status;
    }

    void onVcSurroundingObject11to15StatusChanged(const ArgumentsVcSurroundingObject11to15StatusChanged& args) {
        mVCSurroundingObject11Status = args.mInput_VCSurroundingObject11Status;
        mVCSurroundingObject12Status = args.mInput_VCSurroundingObject12Status;
        mVCSurroundingObject13Status = args.mInput_VCSurroundingObject13Status;
        mVCSurroundingObject14Status = args.mInput_VCSurroundingObject14Status;
        mVCSurroundingObject15Status = args.mInput_VCSurroundingObject15Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor11Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor12Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor13Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor14Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat>();

        mPrevVCSurroundingObject11Status = mVCSurroundingObject11Status;
        mPrevVCSurroundingObject12Status = mVCSurroundingObject12Status;
        mPrevVCSurroundingObject13Status = mVCSurroundingObject13Status;
        mPrevVCSurroundingObject14Status = mVCSurroundingObject14Status;
        mPrevVCSurroundingObject15Status = mVCSurroundingObject15Status;
    }

    void onVcSurroundingObjectClassification01to05StatusChanged(
        const ArgumentsVcSurroundingObjectClassification01to05StatusChanged& args) {
        mVCSurroundingObjectClassification01Status = args.mInput_VCSurroundingObjectClassification01Status;
        mVCSurroundingObjectClassification02Status = args.mInput_VCSurroundingObjectClassification02Status;
        mVCSurroundingObjectClassification03Status = args.mInput_VCSurroundingObjectClassification03Status;
        mVCSurroundingObjectClassification04Status = args.mInput_VCSurroundingObjectClassification04Status;
        mVCSurroundingObjectClassification05Status = args.mInput_VCSurroundingObjectClassification05Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass02Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass03Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass04Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass05Stat>();
    }

    void onVcSurroundingObjectClassification06to10StatusChanged(
        const ArgumentsVcSurroundingObjectClassification06to10StatusChanged& args) {
        mVCSurroundingObjectClassification06Status = args.mInput_VCSurroundingObjectClassification06Status;
        mVCSurroundingObjectClassification07Status = args.mInput_VCSurroundingObjectClassification07Status;
        mVCSurroundingObjectClassification08Status = args.mInput_VCSurroundingObjectClassification08Status;
        mVCSurroundingObjectClassification09Status = args.mInput_VCSurroundingObjectClassification09Status;
        mVCSurroundingObjectClassification10Status = args.mInput_VCSurroundingObjectClassification10Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass06Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass07Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass08Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass09Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass10Stat>();
    }

    void onVcSurroundingObjectClassification11to15StatusChanged(
        const ArgumentsVcSurroundingObjectClassification11to15StatusChanged& args) {
        mVCSurroundingObjectClassification11Status = args.mInput_VCSurroundingObjectClassification11Status;
        mVCSurroundingObjectClassification12Status = args.mInput_VCSurroundingObjectClassification12Status;
        mVCSurroundingObjectClassification13Status = args.mInput_VCSurroundingObjectClassification13Status;
        mVCSurroundingObjectClassification14Status = args.mInput_VCSurroundingObjectClassification14Status;
        mVCSurroundingObjectClassification15Status = args.mInput_VCSurroundingObjectClassification15Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass11Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass12Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass13Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass14Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat>();
    }

    void onVcSurroundingObjectLateralPosition01to05ValueChanged(
        const ArgumentsVcSurroundingObjectLateralPosition01to05ValueChanged& args) {
        mVCSurroundingObjectLateralPosition01Value = args.mInput_VCSurroundingObjectLateralPosition01Value;
        mVCSurroundingObjectLateralPosition02Value = args.mInput_VCSurroundingObjectLateralPosition02Value;
        mVCSurroundingObjectLateralPosition03Value = args.mInput_VCSurroundingObjectLateralPosition03Value;
        mVCSurroundingObjectLateralPosition04Value = args.mInput_VCSurroundingObjectLateralPosition04Value;
        mVCSurroundingObjectLateralPosition05Value = args.mInput_VCSurroundingObjectLateralPosition05Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Stat>();
    }

    void onVcSurroundingObjectLateralPosition06to10ValueChanged(
        const ArgumentsVcSurroundingObjectLateralPosition06to10ValueChanged& args) {
        mVCSurroundingObjectLateralPosition06Value = args.mInput_VCSurroundingObjectLateralPosition06Value;
        mVCSurroundingObjectLateralPosition07Value = args.mInput_VCSurroundingObjectLateralPosition07Value;
        mVCSurroundingObjectLateralPosition08Value = args.mInput_VCSurroundingObjectLateralPosition08Value;
        mVCSurroundingObjectLateralPosition09Value = args.mInput_VCSurroundingObjectLateralPosition09Value;
        mVCSurroundingObjectLateralPosition10Value = args.mInput_VCSurroundingObjectLateralPosition10Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Stat>();
    }

    void onVcSurroundingObjectLateralPosition11to15ValueChanged(
        const ArgumentsVcSurroundingObjectLateralPosition11to15ValueChanged& args) {
        mVCSurroundingObjectLateralPosition11Value = args.mInput_VCSurroundingObjectLateralPosition11Value;
        mVCSurroundingObjectLateralPosition12Value = args.mInput_VCSurroundingObjectLateralPosition12Value;
        mVCSurroundingObjectLateralPosition13Value = args.mInput_VCSurroundingObjectLateralPosition13Value;
        mVCSurroundingObjectLateralPosition14Value = args.mInput_VCSurroundingObjectLateralPosition14Value;
        mVCSurroundingObjectLateralPosition15Value = args.mInput_VCSurroundingObjectLateralPosition15Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat>();
    }

    void onVcSurroundingObjectLongitudinalPosition01to05ValueChanged(
        const ArgumentsVcSurroundingObjectLongitudinalPosition01to05ValueChanged& args) {
        mVCSurroundingObjectLongitudinalPosition01Value = args.mInput_VCSurroundingObjectLongitudinalPosition01Value;
        mVCSurroundingObjectLongitudinalPosition02Value = args.mInput_VCSurroundingObjectLongitudinalPosition02Value;
        mVCSurroundingObjectLongitudinalPosition03Value = args.mInput_VCSurroundingObjectLongitudinalPosition03Value;
        mVCSurroundingObjectLongitudinalPosition04Value = args.mInput_VCSurroundingObjectLongitudinalPosition04Value;
        mVCSurroundingObjectLongitudinalPosition05Value = args.mInput_VCSurroundingObjectLongitudinalPosition05Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Stat>();
    }

    void onVcSurroundingObjectLongitudinalPosition06to10ValueChanged(
        const ArgumentsVcSurroundingObjectLongitudinalPosition06to10ValueChanged& args) {
        mVCSurroundingObjectLongitudinalPosition06Value = args.mInput_VCSurroundingObjectLongitudinalPosition06Value;
        mVCSurroundingObjectLongitudinalPosition07Value = args.mInput_VCSurroundingObjectLongitudinalPosition07Value;
        mVCSurroundingObjectLongitudinalPosition08Value = args.mInput_VCSurroundingObjectLongitudinalPosition08Value;
        mVCSurroundingObjectLongitudinalPosition09Value = args.mInput_VCSurroundingObjectLongitudinalPosition09Value;
        mVCSurroundingObjectLongitudinalPosition10Value = args.mInput_VCSurroundingObjectLongitudinalPosition10Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Stat>();
    }

    void onVcSurroundingObjectLongitudinalPosition11to15ValueChanged(
        const ArgumentsVcSurroundingObjectLongitudinalPosition11to15ValueChanged& args) {
        mVCSurroundingObjectLongitudinalPosition11Value = args.mInput_VCSurroundingObjectLongitudinalPosition11Value;
        mVCSurroundingObjectLongitudinalPosition12Value = args.mInput_VCSurroundingObjectLongitudinalPosition12Value;
        mVCSurroundingObjectLongitudinalPosition13Value = args.mInput_VCSurroundingObjectLongitudinalPosition13Value;
        mVCSurroundingObjectLongitudinalPosition14Value = args.mInput_VCSurroundingObjectLongitudinalPosition14Value;
        mVCSurroundingObjectLongitudinalPosition15Value = args.mInput_VCSurroundingObjectLongitudinalPosition15Value;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat>();
    }

    void onVcEmergencyStreeingStatusChanged(const ArgumentsVcEmergencyStreeingStatusChanged& args) {
        mVCEmergencyStreeingStatus = args.mInput_VCEmergencyStreeingStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewEmergencySteeringStat();
    }

    void onVcAdditionalStatusChanged(const ArgumentsVcAdditionalStatusChanged& args) {
        mVCAdditionalStatus = args.mInput_VCAdditionalStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewAdditionalVehicleStat();
    }

    void onBrakeLampControlCommandStatusChanged(const ArgumentsBrakeLampControlCommandStatusChanged& args) {
        mBrakeLampControlCommand_Status = args.mInput_BrakeLampControlCommand_Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewMainBrakeLightActiveStatus();
    }

    void onExtLowBeamOnReqStatusChanged(const ArgumentsExtLowBeamOnReqStatusChanged& args) {
        mExtLowBeamOnReqStatus = args.mInput_ExtLowBeamOnReqStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewMainExteriorLowBeamActiveStatus();
    }

    void onExtHighBeamOnReqStatusChanged(const ArgumentsExtHighBeamOnReqStatusChanged& args) {
        mExtHighBeamOnReqStatus = args.mInput_ExtHighBeamOnReqStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewMainExteriorHighBeamActiveStatus();
    }

    void onExtTurnLeftLampStatusChanged(const ArgumentsExtTurnLeftLampStatusChanged& args) {
        mExtTurnLeftLampStatus = args.mInput_ExtTurnLeftLampStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewMainExteriorTurnLeftLampActiveStatus();
    }

    void onExtTurnRightLampStatusChanged(const ArgumentsExtTurnRightLampStatusChanged& args) {
        mExtTurnRightLampStatus = args.mInput_ExtTurnRightLampStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateConstantADASViewMainExteriorTurnRightLampActiveStatus();
    }

    void onInfoDisplayReqChanged(const ArgumentsInfoDisplayReqChanged& args) {
        mCruiseInfoDisplayReqStatus = args.mInput_CruiseInfoDisplayReqStatus;
        mSpeedLimitInfoDisplayReqStatus = args.mInput_SpeedLimitInfoDisplayReqStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateEventCcPopup();
        updateEventMslaPopup();
    }

    void onGroup1AdasWarning1_1StatusChanged(const ArgumentsGroup1AdasWarning1_1StatusChanged& args) {
        mGroup1AdasWarning1_1Status = args.mInput_NTFGroup1AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateEvent1GroupPopup1_1();
    }

    void onGroup1AdasWarning1_2StatusChanged(const ArgumentsGroup1AdasWarning1_2StatusChanged& args) {
        mGroup1AdasWarning1_2Status = args.mInput_NTFGroup1AdasWarning1_2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateEvent1GroupPopup1_2();
    }

    void onGroup4AdasWarning1_1StatusChanged(const ArgumentsGroup4AdasWarning1_1StatusChanged& args) {
        mGroup4AdasWarning1_1Status = args.mInput_NTFGroup4AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateEvent4GroupPopup1_1();
    }

    void onGroup2AdasWarning1_1StatusChanged(const ArgumentsGroup2AdasWarning1_1StatusChanged& args) {
        mGroup2AdasWarning1_1Status = args.mInput_NTMGroup2AdasWarning1_1Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateEvent2GroupPopup1_1();
    }

    void onGroup2AdasWarning1_2StatusChanged(const ArgumentsGroup2AdasWarning1_2StatusChanged& args) {
        mGroup2AdasWarning1_2Status = args.mInput_NTMGroup2AdasWarning1_2Status;

        if (mIsConfigOn != true) {
            return;
        }

        updateEvent2GroupPopup1_2();
    }

    void onNtfHandsOffStatusChanged(const ArgumentsNtfHandsOffStatusChanged& args) {
        mNTFHandsOffStatus = args.mInput_NTFHandsOffStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateEventLargeFullPopup();
    }

    void onCrawlWarnStatusChanged(const ArgumentsCrawlWarnStatusChanged& args) {
        mCrawlWarnStatus = args.mInput_CrawlWarnStatus;

        if (mIsConfigOn != true) {
            return;
        }

        updateEventCrawlPopup();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateConstantCcSymbol();
        updateConstantMslaSymbol();
        updateConstantCcMslaSpeedStatus();
        updateConstantCrawlOperation();
        updateConstantNsccSymbolStatus();
        updateConstantSccSymbolStatus();
        updateConstantADASSetSpeedStatus();
        updateConstantADASSetSpeedValue();
        updateConstantLfaHandsOnOffTelltaleStatus();
        updateConstantLCALeftTelltaleStatus();
        updateConstantLCARightTelltaleStatus();
        updateConstantADASLeftLaneStatus();
        updateConstantADASLeftLineLatPosValue();
        updateConstantADASRightLaneStatus();
        updateConstantADASRightLineLatPosValue();
        updateConstantADASHeadwayDistStatus();
        updateConstantADASHeadwayDistValue();
        updateConstantADASLaneCvrtLvlValue();
        updateConstantADASVehicleDistanceSetValue();
        updateConstantADASCenterRoadStatus();
        updateConstantADASCenterRoadLeftLatPosValue();
        updateConstantADASCenterRoadRightLatPosValue();
        updateConstantADASRoadCvrtLvlValue();
        updateConstantADASLeftLeftLaneStat();
        updateConstantADASLeftLeftLineLatPosValue();
        updateConstantADASRightRightLaneStat();
        updateConstantADASRightRightLineLatPosValue();
        updateConstantADASViewFrontLeftCollisionWarningStatus();
        updateConstantADASViewFrontRightCollisionWarningStatus();
        updateConstantADASViewFrontWarnStatus();
        updateConstantADASViewFrontLeftWarnStatus();
        updateConstantADASViewFrontRightWarnStatus();
        updateConstantADASViewRearLeftWarnStatus();
        updateConstantADASViewRearRightWarnStatus();
        updateConstantADASViewLeftArrowStatus();
        updateConstantADASViewLeftArrowLatValue();
        updateConstantADASViewRightArrowStatus();
        updateConstantADASViewRightArrowLatValue();
        updateConstantADASViewLaneChangeRectangle();
        updateConstantADASViewLaneChangeRectangleLatValue();
        updateConstantADASViewLaneChangeAvailableStatus();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor02Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor03Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor04Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor05Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor06Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor07Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor08Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor09Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor10Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor11Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor12Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor13Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor14Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification01Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification02Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass02Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification03Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass03Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification04Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass04Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification05Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass05Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification06Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass06Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification07Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass07Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification08Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass08Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification09Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass09Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification10Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass10Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification11Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass11Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification12Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass12Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification13Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass13Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification14Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass14Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification15Status,
                                                     SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat>();
        updateConstantADASViewEmergencySteeringStat();
        updateConstantADASViewAdditionalVehicleStat();
        updateConstantADASViewMainBrakeLightActiveStatus();
        updateConstantADASViewMainExteriorLowBeamActiveStatus();
        updateConstantADASViewMainExteriorHighBeamActiveStatus();
        updateConstantADASViewMainExteriorTurnLeftLampActiveStatus();
        updateConstantADASViewMainExteriorTurnRightLampActiveStatus();
        updateEventCrawlSummary();
        updateEventCcPopup();
        updateEventMslaPopup();
        updateEvent1GroupPopup1_1();
        updateEvent1GroupPopup1_2();
        updateEvent4GroupPopup1_1();
        updateEvent2GroupPopup1_1();
        updateEvent2GroupPopup1_2();
        updateEventLargeFullPopup();
        updateEventCrawlPopup();
    }

    // 4.1 동력 타입 별 표시 신호 통합
    void updatePrivateCcSldSetSpeedValue() {
        HUInt64 ccSldSetSpeed = mCcSldSetSpeedValue;

        if ((mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV ||
             mConfigVehicleType == ConfigVehicleType::EREV) &&
            (mCcColorStat == CcColorStat::CRUISE_PAUSE || mCcColorStat == CcColorStat::CRUISE_ACTIVE)) {
            ccSldSetSpeed = mCcOnlySetSpeedValue;
        }

        mPrivateCcSldSetSpeedValue = ccSldSetSpeed;
        updateConstantCcMslaSpeedStatus();
    }

    // 5.1.1 CC/MSLA/CRAWL 인디케이터
    // 5.1.1.1 CC 심볼 칼라 표시
    void updateConstantCcSymbol() {
        SFCDetail_Contents_ADAS_2_0ConstantCcColorStat stat = SFCDetail_Contents_ADAS_2_0ConstantCcColorStat::DISPLAY_OFF;

        if (mIsIgnOn == true && mCcOnOffStat == CcOnOffStat::ON) {
            switch (mCruiseSetIndReqStatus) {
                case CruiseSetIndReqStatus::OFF:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantCcColorStat::CRUISE_PAUSE;
                    break;
                case CruiseSetIndReqStatus::ON:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantCcColorStat::CRUISE_ACTIVE;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantCcColorStat(stat);
    }

    // 5.1.1.2 MSLA 심볼 칼라 표시
    void updateConstantMslaSymbol() {
        SFCDetail_Contents_ADAS_2_0ConstantMslaColorStat stat = SFCDetail_Contents_ADAS_2_0ConstantMslaColorStat::DISPLAY_OFF;

        if (mIsIgnOn == true && mMslaOnOffStat == MslaOnOffStat::ON) {
            if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::READY) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantMslaColorStat::MSLA_PAUSE;
            } else if (mSpeedLimitReadyStatus == SpeedLimitReadyStatus::NORMAL) {
                if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantMslaColorStat::MSLA_ISLA_ON;
                } else {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantMslaColorStat::MSLA_ACTIVE;
                }
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantMslaColorStat(stat);
    }

    // 5.1.1.3 속도 표시
    // 5.1.1.3.1 속도 표시(상태)
    void updateConstantCcMslaSpeedStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat stat = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat::DISPLAY_OFF;
        SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueA blinkA =
            SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueA::NONE;
        SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueB blinkB =
            SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueB::NONE;

        if (mIsIgnOn == true && (mCcOnOffStat == CcOnOffStat::ON || mMslaOnOffStat == MslaOnOffStat::ON)) {
            if (mPrivateCcSldSetSpeedValue != kTimeOut) {
                if (mPrivateCcSldSetSpeedValue == 0 || (mPrivateCcSldSetSpeedValue == (mCcSldMaxSpeedValue + 1))) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat::NO_DATA;
                } else if (1 <= mPrivateCcSldSetSpeedValue && mPrivateCcSldSetSpeedValue <= mCcSldMaxSpeedValue) {
                    if ((mMslaActiveStat == MslaActiveStat::OFF && mCcActiveStat == CcActiveStat::ON &&
                         mCruiseOverrideReqStatus == CruiseOverrideReqStatus::OVERRIDE) ||
                        (mMslaActiveStat == MslaActiveStat::ON && mMslaOverride == MslaOverride::ON)) {
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueA::ON;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueB::DISPLAY_OFF;
                    } else {
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat::ON;
                    }
                }
            }
        }
        mCcMslaSpeedStat = stat;
        setSFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat(mCcMslaSpeedStat);
        setSFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueA(blinkA);
        setSFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedBlinkValueB(blinkB);
        updateConstantCcMslaSpeedValue();
    }

    // 5.1.1.3.2 속도 표시(값)
    void updateConstantCcMslaSpeedValue() {
        if (mIsIgnOn == true && mCcMslaSpeedStat != SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat::DISPLAY_OFF) {
            setSFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedValue(mPrivateCcSldSetSpeedValue);
        } else {
            setSFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedValue(0);
        }
    }

    // 5.1.1.3.3 NCC 표시
    void updateConstantNaviCruiseStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantNaviCruiseStat stat = SFCDetail_Contents_ADAS_2_0ConstantNaviCruiseStat::DISPLAY_OFF;

        if (mIsIgnOn == true) {
            switch (mNccCameraOperStatus) {
                case NccCameraOperStatus::READY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNaviCruiseStat::READY;
                    break;
                case NccCameraOperStatus::ACT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNaviCruiseStat::ACTIVE;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantNaviCruiseStat(stat);
    }

    // 5.1.1.3.4 CRAWL 표시
    // 5.1.1.3.4.1 CRAWL 동작 상태 표시
    void updateConstantCrawlOperation() {
        SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::OFF;
        SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA blinkA =
            SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::OFF;
        SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB blinkB =
            SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
        SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat step =
            SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::OFF;

        if (mIsIgnOn == true) {
            if (mCrawlActiveStatus == CrawlActiveStatus::READY) {
                switch (mCrawlActiveStepStatus) {
                    case CrawlActiveStepStatus::STEP1:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::READY;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP1;
                        break;
                    case CrawlActiveStepStatus::STEP2:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::READY;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP2;
                        break;
                    case CrawlActiveStepStatus::STEP3:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::READY;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP3;
                        break;
                    case CrawlActiveStepStatus::STEP4:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::READY;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP4;
                        break;
                    case CrawlActiveStepStatus::STEP5:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::READY;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP5;
                        break;
                    default:
                        // no operation
                        break;
                }
            } else if (mCrawlActiveStatus == CrawlActiveStatus::ACTIVE) {
                switch (mCrawlActiveStepStatus) {
                    case CrawlActiveStepStatus::STEP1:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::ACTIVE;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP1;
                        break;
                    case CrawlActiveStepStatus::STEP2:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::ACTIVE;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP2;
                        break;
                    case CrawlActiveStepStatus::STEP3:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::ACTIVE;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP3;
                        break;
                    case CrawlActiveStepStatus::STEP4:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::ACTIVE;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP4;
                        break;
                    case CrawlActiveStepStatus::STEP5:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::ACTIVE;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP5;
                        break;
                    default:
                        // no operation
                        break;
                }
            } else if (mCrawlActiveStatus == CrawlActiveStatus::OVERRIDE) {
                switch (mCrawlActiveStepStatus) {
                    case CrawlActiveStepStatus::STEP1:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::WHITE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP1;
                        break;
                    case CrawlActiveStepStatus::STEP2:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::WHITE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP2;
                        break;
                    case CrawlActiveStepStatus::STEP3:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::WHITE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP3;
                        break;
                    case CrawlActiveStepStatus::STEP4:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::WHITE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP4;
                        break;
                    case CrawlActiveStepStatus::STEP5:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::WHITE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP5;
                        break;
                    default:
                        // no operation
                        break;
                }
            } else if (mCrawlActiveStatus == CrawlActiveStatus::PAUSE) {
                switch (mCrawlActiveStepStatus) {
                    case CrawlActiveStepStatus::STEP1:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::ACTIVE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP1;
                        break;
                    case CrawlActiveStepStatus::STEP2:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::ACTIVE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP2;
                        break;
                    case CrawlActiveStepStatus::STEP3:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::ACTIVE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP3;
                        break;
                    case CrawlActiveStepStatus::STEP4:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::ACTIVE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP4;
                        break;
                    case CrawlActiveStepStatus::STEP5:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::ACTIVE;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP5;
                        break;
                    default:
                        // no operation
                        break;
                }
            } else if (mCrawlActiveStatus == CrawlActiveStatus::TEMP) {
                switch (mCrawlActiveStepStatus) {
                    case CrawlActiveStepStatus::STEP1:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::YELLOW;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP1;
                        break;
                    case CrawlActiveStepStatus::STEP2:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::YELLOW;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP2;
                        break;
                    case CrawlActiveStepStatus::STEP3:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::YELLOW;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP3;
                        break;
                    case CrawlActiveStepStatus::STEP4:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::YELLOW;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP4;
                        break;
                    case CrawlActiveStepStatus::STEP5:
                        stat = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat::BLINK2;
                        blinkA = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA::YELLOW;
                        blinkB = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB::OFF;
                        step = SFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat::STEP5;
                        break;
                    default:
                        // no operation
                        break;
                }
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueA(blinkA);
        setSFCDetail_Contents_ADAS_2_0ConstantCrawlOperationBlinkValueB(blinkB);
        setSFCDetail_Contents_ADAS_2_0ConstantCrawlOperationStepStat(step);
    }

    // 5.1.2 주행 보조 인디케이터
    // 5.1.2.1 NSCC 제어 대상 (1)
    void updateConstantNsccSymbolStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mINDNsccSymbolStatus) {
                case INDNsccSymbolStatus::SAFETY_ZONE:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::SAFETY_ZONE;
                    break;
                case INDNsccSymbolStatus::CURVE:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::CURVE;
                    break;
                case INDNsccSymbolStatus::IC_AND_JC_LEFT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::IC_AND_JC_LEFT;
                    break;
                case INDNsccSymbolStatus::IC_AND_JC_RIGHT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::IC_AND_JC_RIGHT;
                    break;
                case INDNsccSymbolStatus::TOLLGATE:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::TOLLGATE;
                    break;
                case INDNsccSymbolStatus::ROUNDABOUT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::ROUNDABOUT;
                    break;
                case INDNsccSymbolStatus::SPEED_BUMP:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat::SPEED_BUMP;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantNsccSymbolStat(stat);
    }

    // 5.1.2.2 SCC 기능 상태 (2)
    void updateConstantSccSymbolStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat stat = SFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if ((mINDSccSymbol1Status == INDSccSymbol1Status::GRAY && mINDSccSymbol2Status == INDSccSymbol2Status::OFF) ||
                (mINDSccSymbol1Status == INDSccSymbol1Status::OFF && mINDSccSymbol2Status == INDSccSymbol2Status::GRAY)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat::GRAY;
            } else if ((mINDSccSymbol1Status == INDSccSymbol1Status::GREEN && mINDSccSymbol2Status == INDSccSymbol2Status::OFF) ||
                       (mINDSccSymbol1Status == INDSccSymbol1Status::OFF && mINDSccSymbol2Status == INDSccSymbol2Status::GREEN)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat::GREEN;
            } else if ((mINDSccSymbol1Status == INDSccSymbol1Status::BLUE && mINDSccSymbol2Status == INDSccSymbol2Status::OFF) ||
                       (mINDSccSymbol1Status == INDSccSymbol1Status::OFF && mINDSccSymbol2Status == INDSccSymbol2Status::BLUE)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat::BLUE;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantSccSymbolStat(stat);
    }

    // 5.1.2.3 설정 속도 및 속도 단위 (3)
    // 5.1.2.3.1 칼라
    void updateConstantADASSetSpeedStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if ((mINDSetSpeed1Status == INDSetSpeed1Status::GRAY && mINDSetSpeed2Status == INDSetSpeed2Status::OFF) ||
                (mINDSetSpeed1Status == INDSetSpeed1Status::OFF && mINDSetSpeed2Status == INDSetSpeed2Status::GRAY)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat::GRAY;
            } else if ((mINDSetSpeed1Status == INDSetSpeed1Status::GREEN && mINDSetSpeed2Status == INDSetSpeed2Status::OFF) ||
                       (mINDSetSpeed1Status == INDSetSpeed1Status::OFF && mINDSetSpeed2Status == INDSetSpeed2Status::GREEN)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat::GREEN;
            } else if ((mINDSetSpeed1Status == INDSetSpeed1Status::BLACK && mINDSetSpeed2Status == INDSetSpeed2Status::OFF) ||
                       (mINDSetSpeed1Status == INDSetSpeed1Status::OFF && mINDSetSpeed2Status == INDSetSpeed2Status::BLACK)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat::BLACK;
            } else if ((mINDSetSpeed1Status == INDSetSpeed1Status::BLUE && mINDSetSpeed2Status == INDSetSpeed2Status::OFF) ||
                       (mINDSetSpeed1Status == INDSetSpeed1Status::OFF && mINDSetSpeed2Status == INDSetSpeed2Status::BLUE)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat::BLUE;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedStat(stat);
    }

    // 5.1.2.3.2 속도값
    void updateConstantADASSetSpeedValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if ((mINDSetSpeed1Value >= 0 && mINDSetSpeed1Value <= 254) &&
                !(mINDSetSpeed2Value >= 0 && mINDSetSpeed2Value <= 254)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValueStat::VALUE;
                value = mINDSetSpeed1Value;
            } else if (!(mINDSetSpeed1Value >= 0 && mINDSetSpeed1Value <= 254) &&
                       (mINDSetSpeed2Value >= 0 && mINDSetSpeed2Value <= 254)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValueStat::VALUE;
                value = mINDSetSpeed2Value;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASSetSpeedValue(value);
    }

    // 5.1.2.4 LFA 기능 상태 (4), (5)
    void updateConstantLfaHandsOnOffTelltaleStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if ((mINDLfaSymbol1Status == INDLfaSymbol1Status::GRAY && mINDLfaSymbol2Status == INDLfaSymbol2Status::OFF) ||
                    (mINDLfaSymbol1Status == INDLfaSymbol1Status::OFF && mINDLfaSymbol2Status == INDLfaSymbol2Status::GRAY)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::LFA_GRAY;
                } else if ((mINDLfaSymbol1Status == INDLfaSymbol1Status::GRAY_EXPANSION &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::OFF) ||
                           (mINDLfaSymbol1Status == INDLfaSymbol1Status::OFF &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::GRAY_EXPANSION)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::LFA_GRAY_EXPANSION;
                } else if ((mINDLfaSymbol1Status == INDLfaSymbol1Status::GREEN &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::OFF) ||
                           (mINDLfaSymbol1Status == INDLfaSymbol1Status::OFF &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::GREEN)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::LFA_GREEN;
                } else if ((mINDLfaSymbol1Status == INDLfaSymbol1Status::BLUE &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::OFF) ||
                           (mINDLfaSymbol1Status == INDLfaSymbol1Status::OFF &&
                            mINDLfaSymbol2Status == INDLfaSymbol2Status::BLUE)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::LFA_BLUE;
                } else {
                    // no operation
                }
            } else if ((mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::HANDS_ON_AMBER &&
                        mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) ||
                       (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                        mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::HANDS_ON_AMBER)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::HANDS_ON_AMBER;
            } else if ((mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::HANDS_ON_RED &&
                        mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) ||
                       (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                        mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::HANDS_ON_RED)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat::HANDS_ON_RED;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantLfaHandsOnOffTelltaleStat(stat);
    }

    // 5.1.2.5 LCA (6)
    // 5.1.2.5.1 차로 변경 보조 심볼-왼쪽
    void updateConstantLCALeftTelltaleStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat stat = SFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if ((mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::GRAY &&
                     mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::OFF) ||
                    (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::OFF &&
                     mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::GRAY)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat::GRAY;
                } else if ((mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::GREEN &&
                            mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::OFF) ||
                           (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::OFF &&
                            mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::GREEN)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat::GREEN;
                } else if ((mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::BLUE &&
                            mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::OFF) ||
                           (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::OFF &&
                            mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::BLUE)) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat::BLUE;
                } else {
                    // no operation
                }
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantLCALeftTelltaleStat(stat);
    }

    // 5.1.2.5.2 차로 변경 보조 심볼-오른쪽
    void updateConstantLCARightTelltaleStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::GRAY ||
                    mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::GRAY) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat::GRAY;
                } else if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::GREEN ||
                           mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::GREEN) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat::GREEN;
                } else if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::BLUE ||
                           mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::BLUE) {
                    stat = SFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat::BLUE;
                } else {
                    // no operation
                }
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantLCARightTelltaleStat(stat);
    }

    // 5.1.3 주행 보조 뷰 - CAN
    // 5.1.3.1 차선-왼쪽 (1-2)
    // 5.1.3.1.1 칼라
    void updateConstantADASLeftLaneStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCLeftLineStatus) {
                case VCLeftLineStatus::LIGHT_GRAY:
                case VCLeftLineStatus::UNHANDLED_TIMEOUT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::LIGHT_GRAY;
                    break;
                case VCLeftLineStatus::DARK_GRAY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::DARK_GRAY;
                    break;
                case VCLeftLineStatus::AMBER:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::AMBER;
                    break;
                case VCLeftLineStatus::GREEN:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::GREEN;
                    break;
                case VCLeftLineStatus::DARK_GRAY_DASH:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::DARK_GRAY_DASH;
                    break;
                case VCLeftLineStatus::GREEN_DASH:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat::GREEN_DASH;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLaneStat(stat);
    }

    // 5.1.3.1.2 횡위치
    void updateConstantADASLeftLineLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCLeftLineOffsetValue >= 0 && mVCLeftLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValueStat::VALUE;
                value = mVCLeftLineOffsetValue;
            } else if (mVCLeftLineOffsetValue == kTimeOut) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValueStat::VALUE;
                value = 15;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLineLatPosValue(value);
    }

    // 5.1.3.2 차선-오른쪽 (1-3)
    // 5.1.3.2.1 칼라
    void updateConstantADASRightLaneStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCRightLineStatus) {
                case VCRightLineStatus::LIGHT_GRAY:
                case VCRightLineStatus::UNHANDLED_TIMEOUT:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::LIGHT_GRAY;
                    break;
                case VCRightLineStatus::DARK_GRAY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::DARK_GRAY;
                    break;
                case VCRightLineStatus::AMBER:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::AMBER;
                    break;
                case VCRightLineStatus::GREEN:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::GREEN;
                    break;
                case VCRightLineStatus::DARK_GRAY_DASH:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::DARK_GRAY_DASH;
                    break;
                case VCRightLineStatus::GREEN_DASH:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat::GREEN_DASH;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASRightLaneStat(stat);
    }

    // 5.1.3.2.2 횡위치
    void updateConstantADASRightLineLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCRightLineOffsetValue == kTimeOut) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValueStat::VALUE;
                value = 15;
            } else if (mVCRightLineOffsetValue >= 0 && mVCRightLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValueStat::VALUE;
                value = mVCRightLineOffsetValue;
            } else {
                // OFF
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASRightLineLatPosValue(value);
    }

    // 5.1.3.3 차간 거리 (1-4)
    // 5.1.3.3.1 칼라
    void updateConstantADASHeadwayDistStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat stat = SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCVehicleDistanceStatus) {
                case VCVehicleDistanceStatus::GREEN:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat::GREEN;
                    break;
                case VCVehicleDistanceStatus::GREEN_DECEL:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat::GREEN_DECEL;
                    break;
                case VCVehicleDistanceStatus::GRAY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat::GRAY;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistStat(stat);
    }

    // 5.1.3.3.2 종위치
    void updateConstantADASHeadwayDistValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCVehicleDistanceValue >= 0 && mVCVehicleDistanceValue <= 1500) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistValueStat::VALUE;
                value = mVCVehicleDistanceValue;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASHeadwayDistValue(value);
    }

    // 5.1.3.1.3 차선-왼쪽 (1-2) 곡률
    // 5.1.3.2.3 차선-오른쪽 (1-3) 곡률
    // 5.1.3.3.3 차간 거리 (1-4) 곡률
    // 5.1.3.5.3 왼쪽의 왼쪽 (1-8) 곡률
    // 5.1.3.6.3 오른쪽의 오른쪽 (1-9) 곡률
    void updateConstantADASLaneCvrtLvlValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCDrivingLaneRadiusValue == 0 || mVCDrivingLaneRadiusValue == kTimeOut) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat::STRAIGHT;
            } else if (mVCDrivingLaneRadiusValue >= 1 && mVCDrivingLaneRadiusValue <= 15) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat::LEFT_VALUE;
                value = mVCDrivingLaneRadiusValue;
            } else if (mVCDrivingLaneRadiusValue >= 16 && mVCDrivingLaneRadiusValue <= 30) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat::RIGHT_VALUE;
                value = mVCDrivingLaneRadiusValue - 15;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASLaneCvrtLvlValue(value);
    }

    // 5.1.3.3.4 단계
    void updateConstantADASVehicleDistanceSetValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCVehicleDistanceLevelValue) {
                case VCVehicleDistanceLevelValue::LEVEL1:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue::LEVEL1;
                    break;
                case VCVehicleDistanceLevelValue::LEVEL2:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue::LEVEL2;
                    break;
                case VCVehicleDistanceLevelValue::LEVEL3:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue::LEVEL3;
                    break;
                case VCVehicleDistanceLevelValue::LEVEL4:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue::LEVEL4;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASVehicleDistanceSetValue(stat);
    }

    // 5.1.3.4 노면-중앙 (1-5)
    // 5.1.3.4.1 칼라
    void updateConstantADASCenterRoadStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadStat stat = SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCCenterRoadSurfaceStatus) {
                case VCCenterRoadSurfaceStatus::GREEN:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadStat::GREEN;
                    break;
                case VCCenterRoadSurfaceStatus::GRAY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadStat::GRAY;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadStat(stat);
    }

    // 5.1.3.4.2 좌측 횡위치
    void updateConstantADASCenterRoadLeftLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadLeftLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadLeftLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLeftLineOffsetValue >= 0 && mVCLeftLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadLeftLatPosValueStat::VALUE;
                value = mVCLeftLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadLeftLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadLeftLatPosValue(value);
    }

    // 5.1.3.4.3 우측 횡위치
    void updateConstantADASCenterRoadRightLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadRightLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadRightLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCRightLineOffsetValue >= 0 && mVCRightLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadRightLatPosValueStat::VALUE;
                value = mVCRightLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadRightLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASCenterRoadRightLatPosValue(value);
    }

    // 5.1.3.4.4 곡률
    void updateConstantADASRoadCvrtLvlValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCDrivingLaneRadiusValue == 0 || mVCDrivingLaneRadiusValue == kTimeOut) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat::STRAIGHT;
            } else if (mVCDrivingLaneRadiusValue >= 1 && mVCDrivingLaneRadiusValue <= 15) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat::LEFT_VALUE;
                value = mVCDrivingLaneRadiusValue;
            } else if (mVCDrivingLaneRadiusValue >= 16 && mVCDrivingLaneRadiusValue <= 30) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat::RIGHT_VALUE;
                value = mVCDrivingLaneRadiusValue - 15;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASRoadCvrtLvlValue(value);
    }

    // 5.1.3.5 차선 - 왼쪽의 왼쪽 (1-8)
    // 5.1.3.5.1 칼라
    void updateConstantADASLeftLeftLaneStat() {
        SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat::OFF;
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::LIGHT_GRAY) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat::LIGHT_GRAY;
            } else if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::DARK_GRAY) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat::DARK_GRAY;
            } else if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::GREEN) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat::GREEN;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLaneStat(stat);
    }

    // 5.1.3.5.2 횡위치
    void updateConstantADASLeftLeftLineLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLineLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLineLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCLeftLeftLineOffsetValue >= 0 && mVCLeftLeftLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLineLatPosValueStat::VALUE;
                value = mVCLeftLeftLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLineLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASLeftLeftLineLatPosValue(value);
    }

    // 5.1.3.6 차선 - 오른쪽의 오른쪽 (1-9)
    // 5.1.3.6.1 칼라
    void updateConstantADASRightRightLaneStat() {
        SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat::OFF;
        if (mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCRightRightLineStatus == VCRightRightLineStatus::LIGHT_GRAY) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat::LIGHT_GRAY;
            } else if (mVCRightRightLineStatus == VCRightRightLineStatus::DARK_GRAY) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat::DARK_GRAY;
            } else if (mVCRightRightLineStatus == VCRightRightLineStatus::GREEN) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat::GREEN;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASRightRightLaneStat(stat);
    }

    // 5.1.3.6.2 횡위치
    void updateConstantADASRightRightLineLatPosValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLineLatPosValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLineLatPosValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true) {
            if (mVCRightRightLineOffsetValue >= 0 && mVCRightRightLineOffsetValue <= 63) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASRightRightLineLatPosValueStat::VALUE;
                value = mVCRightRightLineOffsetValue;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASRightRightLineLatPosValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASRightRightLineLatPosValue(value);
    }

    // 5.1.3.7 충돌 경고 - 전방 왼쪽 (2-1)
    void updateConstantADASViewFrontLeftCollisionWarningStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftCollisionWarningStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftCollisionWarningStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCFrontLeftObjectMovingDirectionStatus) {
                case VCFrontLeftObjectMovingDirectionStatus::RED_ARROW:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftCollisionWarningStat::RED_ARROW;
                    break;
                case VCFrontLeftObjectMovingDirectionStatus::RED_ARROW_ANIMATION:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftCollisionWarningStat::RED_ARROW_ANI;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftCollisionWarningStat(stat);
    }

    // 5.1.3.8 충돌 경고 - 전방 오른쪽 (2-2)
    void updateConstantADASViewFrontRightCollisionWarningStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightCollisionWarningStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightCollisionWarningStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCFrontRightObjectMovingDirectionStatus) {
                case VCFrontRightObjectMovingDirectionStatus::RED_ARROW:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightCollisionWarningStat::RED_ARROW;
                    break;
                case VCFrontRightObjectMovingDirectionStatus::RED_ARROW_ANIMATION:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightCollisionWarningStat::RED_ARROW_ANI;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightCollisionWarningStat(stat);
    }

    // 5.1.3.9 레이더 파형 - 전방 (2-3)
    void updateConstantADASViewFrontWarnStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontWarnStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontWarnStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE &&
            mVCFrontRadarWaveStatus == VCFrontRadarWaveStatus::RED_WAVE) {
            stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontWarnStat::WAVE_WARNING;
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewFrontWarnStat(stat);
    }

    // 5.1.3.10 레이더 파형 - 전방 왼쪽 (2-4)
    void updateConstantADASViewFrontLeftWarnStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftWarnStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftWarnStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE &&
            mVCFrontLeftRadarWaveStatus == VCFrontLeftRadarWaveStatus::RED_WAVE) {
            stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftWarnStat::RED_WAVE;
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewFrontLeftWarnStat(stat);
    }

    // 5.1.3.11 레이더 파형 - 전방 오른쪽 (2-5)
    void updateConstantADASViewFrontRightWarnStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightWarnStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightWarnStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE &&
            mVCFrontRightRadarWaveStatus == VCFrontRightRadarWaveStatus::RED_WAVE) {
            stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightWarnStat::RED_WAVE;
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewFrontRightWarnStat(stat);
    }

    // 5.1.3.12 레이더 파형 - 후방 왼쪽 (2-6)
    void updateConstantADASViewRearLeftWarnStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewRearLeftWarnStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewRearLeftWarnStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if ((mVCRearLeftRadarWave1Status == VCRearLeftRadarWave1Status::RED_WAVE &&
                 mVCRearLeftRadarWave2Status == VCRearLeftRadarWave2Status::OFF) ||
                (mVCRearLeftRadarWave1Status == VCRearLeftRadarWave1Status::OFF &&
                 mVCRearLeftRadarWave2Status == VCRearLeftRadarWave2Status::RED_WAVE)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewRearLeftWarnStat::RED_WAVE;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewRearLeftWarnStat(stat);
    }

    // 5.1.3.13 레이더 파형 - 후방 오른쪽 (2-7)
    void updateConstantADASViewRearRightWarnStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewRearRightWarnStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewRearRightWarnStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if ((mVCRearRightRadarWave1Status == VCRearRightRadarWave1Status::RED_WAVE &&
                 mVCRearRightRadarWave2Status == VCRearRightRadarWave2Status::OFF) ||
                (mVCRearRightRadarWave1Status == VCRearRightRadarWave1Status::OFF &&
                 mVCRearRightRadarWave2Status == VCRearRightRadarWave2Status::RED_WAVE)) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewRearRightWarnStat::RED_WAVE;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewRearRightWarnStat(stat);
    }

    // 5.1.3.14 차로 변경 방향 보조 - 왼쪽 (2-8)
    // 5.1.3.14.1 타입
    void updateConstantADASViewLeftArrowStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLeftLaneChangeDirectionStatus == VCLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowStat::GREEN_ARROW_ANI;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowStat(stat);
    }

    // 5.1.3.14.2 횡위치
    void updateConstantADASViewLeftArrowLatValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValueStat::CENTER;
                value = 0;
            } else if (0x42 <= mVCLaneChangeRectangleLateralPositionValue && mVCLaneChangeRectangleLateralPositionValue <= 0x7f) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValueStat::LEFT;
                value = (mVCLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLeftArrowLatValue(value);
    }

    // 5.1.3.15 차로 변경 방향 보조-오른쪽 (2-9)
    // 5.1.3.15.1 타입
    void updateConstantADASViewRightArrowStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCRightLaneChangeDirectionStatus == VCRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowStat::GREEN_ARROW_ANI;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowStat(stat);
    }

    // 5.1.3.15.2 횡위치
    void updateConstantADASViewRightArrowLatValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValueStat::CENTER;
                value = 0;
            } else if (1 <= mVCLaneChangeRectangleLateralPositionValue && mVCLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValueStat::RIGHT;
                value = mVCLaneChangeRectangleLateralPositionValue;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewRightArrowLatValue(value);
    }

    // 5.1.3.16 차로 변경 방향 제안 (2-12)
    // 5.1.3.16.1 보조/제안
    void updateConstantADASViewLaneChangeRectangle() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleStat::OFF;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mVCLaneChangeRectangleStatus) {
                case VCLaneChangeRectangleStatus::DOTTED_LINE_SUGGESTION:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleStat::DOTTED_LINE_SUGGESTION;
                    break;
                case VCLaneChangeRectangleStatus::DOTTED_LINE_ASSIST:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleStat::DOTTED_LINE_ASSIST;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleStat(stat);
    }

    // 5.1.3.16.2 횡위치
    void updateConstantADASViewLaneChangeRectangleLatValue() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat::OFF;
        HUInt64 value = 0;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat::CENTER;
                value = 0;
            } else if (1 <= mVCLaneChangeRectangleLateralPositionValue && mVCLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat::RIGHT;
                value = mVCLaneChangeRectangleLateralPositionValue;
            } else if (0x42 <= mVCLaneChangeRectangleLateralPositionValue && mVCLaneChangeRectangleLateralPositionValue <= 0x7f) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat::LEFT;
                value = (mVCLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // no operation
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValueStat(stat);
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeRectangleLatValue(value);
    }

    // 5.1.3.17 차로 변경 가능 상태 (2-X)
    void updateConstantADASViewLaneChangeAvailableStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeAvailableStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeAvailableStat::OFF;

        if (mIsIgnOn == true) {
            switch (mVCLaneChangeAvailableStatus) {
                case VCLaneChangeAvailableStatus::GRAY:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeAvailableStat::GRAY;
                    break;
                case VCLaneChangeAvailableStatus::GREEN:
                    stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeAvailableStat::GREEN;
                    break;
                default:
                    // no operation
                    break;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewLaneChangeAvailableStat(stat);
    }

    // 5.1.3.18 주변 물체 01 ~ 15_Extended (3-3,4,5,6,7,8)
    // 5.1.3.18.1 칼라
    template <typename InputSignalType, typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectColor() {
        if (std::tuple_size<decltype(mSurroundingObjectColorInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of mSurroundingObjectColorInfoTuple";
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
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of surroundingObjectColorInfoElement";
            return;
        }
        InputSignalType vcSurroundingObjectStatus = std::get<kType0>(surroundingObjectColorInfoElement);
        InputSignalType prevVCSurroundingObjectStatus = std::get<kType1>(surroundingObjectColorInfoElement);
        OutputSignalType stat = OutputSignalType::OFF;
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (prevVCSurroundingObjectStatus != InputSignalType::NO_DISPLAY &&
                vcSurroundingObjectStatus == InputSignalType::WHITE) {
                stat = OutputSignalType::WHITE;
            } else if (vcSurroundingObjectStatus == InputSignalType::BLACK) {
                stat = OutputSignalType::BLACK;
            } else if (vcSurroundingObjectStatus == InputSignalType::BLACK_UNDER_BAR) {
                stat = OutputSignalType::BLACK_UNDER_BAR;
            } else if (vcSurroundingObjectStatus == InputSignalType::RED) {
                stat = OutputSignalType::RED;
            } else if (prevVCSurroundingObjectStatus == InputSignalType::NO_DISPLAY &&
                       vcSurroundingObjectStatus == InputSignalType::WHITE) {
                stat = OutputSignalType::FADE_IN;
            } else if (prevVCSurroundingObjectStatus == InputSignalType::WHITE &&
                       vcSurroundingObjectStatus == InputSignalType::NO_DISPLAY) {
                stat = OutputSignalType::FADE_OUT;
            } else {
                // no operation
            }
        }
        std::get<kType2>(surroundingObjectColorInfoElement)(stat);
    }

    // 5.1.3.18.2 타입
    template <typename InputSignalType, typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectClass() {
        if (std::tuple_size<decltype(mSurroundingObjectClassInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of mSurroundingObjectClassInfoTuple";
            return;
        }
        // mSurroundingObjectClassInfoTuple에서 std::pair<Type0, Type1>으로 해당 type의 pair를 가져옵니다.
        // Type0(On condition에 필요한 멤버변수의 레퍼런스): std::reference_wrapper<InputSignalType>
        // Type1(Set output stat를 위한 함수 레퍼런스): std::function<std::string(const OutputSignalType&)>
        std::pair surroundingObjectClassInfoElement =
            std::get<std::pair<std::reference_wrapper<InputSignalType>, std::function<std::string(const OutputSignalType&)>>>(
                mSurroundingObjectClassInfoTuple);
        InputSignalType vcSurroundingObjectClassificationStatus = surroundingObjectClassInfoElement.first.get();
        OutputSignalType stat = OutputSignalType::OFF;
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (vcSurroundingObjectClassificationStatus == InputSignalType::UNKNOWN) {
                stat = OutputSignalType::UNKNOWN;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::PASSENGER_VEHICLE) {
                stat = OutputSignalType::PASSENGER_VEHICLE;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::COMMERCIAL_VEHICLE) {
                stat = OutputSignalType::COMMERCIAL_VEHICLE;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::COMMERCIAL_VEHICLE_2) {
                stat = OutputSignalType::COMMERCIAL_VEHICLE_2;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::PEDESTRIAN) {
                stat = OutputSignalType::PEDESTRIAN;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::CYCLIST) {
                stat = OutputSignalType::CYCLIST;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::MOTOR_CYCLIST_OR_PTW) {
                stat = OutputSignalType::MOTOR_CYCLIST_OR_PTW;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::RUBBER_CONE) {
                stat = OutputSignalType::RUBBER_CONE;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::EMERGENCY_VEHICLE_PASSENGER) {
                stat = OutputSignalType::EMERGENCY_VEHICLE_PASSENGER;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::ANIMAL) {
                stat = OutputSignalType::ANIMAL;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::TUBULAR_MARKER) {
                stat = OutputSignalType::TUBULAR_MARKER;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::PE_BARREL) {
                stat = OutputSignalType::PE_BARREL;
            } else {
                // no operation
            }
        }
        surroundingObjectClassInfoElement.second(stat);
    }

    // 5.1.3.18.3 횡위치
    template <typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectLat() {
        if (std::tuple_size<decltype(mSurroundingObjectLatInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of mSurroundingObjectLatInfoTuple";
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
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of surroundingObjectLatInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 vcSurroundingObjectLateralPositionValue = std::get<kType0>(surroundingObjectLatInfoElement).get();
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (vcSurroundingObjectLateralPositionValue >= 0 && vcSurroundingObjectLateralPositionValue <= 300) {
                stat = OutputSignalType::LEFT;
                value = static_cast<HInt64>(vcSurroundingObjectLateralPositionValue);
            } else if (vcSurroundingObjectLateralPositionValue >= 1024 && vcSurroundingObjectLateralPositionValue <= 1323) {
                stat = OutputSignalType::RIGHT;
                value = 1023 - static_cast<HInt64>(vcSurroundingObjectLateralPositionValue);  // 1024~1323 : -1~-300
            } else {
                // no operation
            }
        }
        std::get<kType1>(surroundingObjectLatInfoElement)(stat);
        std::get<kType2>(surroundingObjectLatInfoElement)(value);
    }

    // 5.1.3.18.4 종위치
    template <typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectLong() {
        if (std::tuple_size<decltype(mSurroundingObjectLongInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of mSurroundingObjectLongInfoTuple";
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
            DWarning() << "[Detail_Contents_ADAS_2_0] Invalied tuple size of surroundingObjectLongInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 vcSurroundingObjectLongitudinalPositionValue = std::get<kType0>(surroundingObjectLongInfoElement).get();
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (vcSurroundingObjectLongitudinalPositionValue >= 0 && vcSurroundingObjectLongitudinalPositionValue <= 1500) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(vcSurroundingObjectLongitudinalPositionValue);
            } else if (vcSurroundingObjectLongitudinalPositionValue >= 1846 &&
                       vcSurroundingObjectLongitudinalPositionValue <= 2045) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(vcSurroundingObjectLongitudinalPositionValue) - 2046;
            } else {
                // no operation
            }
        }
        std::get<kType1>(surroundingObjectLongInfoElement)(stat);
        std::get<kType2>(surroundingObjectLongInfoElement)(value);
    }

    // 5.1.3.19 긴급 조향
    void updateConstantADASViewEmergencySteeringStat() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewEmergencySteeringStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewEmergencySteeringStat::OFF;
        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            if (mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::LEFT_DISPLAY ||
                mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::LEFT_DISPLAY_WITH_ESA) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewEmergencySteeringStat::LEFT;
            } else if (mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::RIGHT_DISPLAY ||
                       mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::RIGHT_DISPLAY_WITH_ESA) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewEmergencySteeringStat::RIGHT;
            } else {
                // no operation
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewEmergencySteeringStat(stat);
    }

    // 5.1.3.20 전방 물체 추가 상태
    void updateConstantADASViewAdditionalVehicleStat() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewAdditionalVehicleStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewAdditionalVehicleStat::OFF;
        if (mIsIgnOn == true && mVCAdditionalStatus == VCAdditionalStatus::LEAD_VEHICLE_DEPARTURE_DISPLAY) {
            stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewAdditionalVehicleStat::LEAD_VEHICLE_DEPARTURE;
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewAdditionalVehicleStat(stat);
    }

    // 5.1.4.14 자차 제동등 (14)
    void updateConstantADASViewMainBrakeLightActiveStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewMainBrakeLightActiveStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewMainBrakeLightActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::STEADY_ON ||
                mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::BLINK_ON) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewMainBrakeLightActiveStat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewMainBrakeLightActiveStat(stat);
    }

    // 5.1.4.15 자차 전조등 (15)
    void updateConstantADASViewMainExteriorLowBeamActiveStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorLowBeamActiveStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorLowBeamActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::ON) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorLowBeamActiveStat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorLowBeamActiveStat(stat);
    }

    // 5.1.4.16 자차 상향등 (16)
    void updateConstantADASViewMainExteriorHighBeamActiveStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorHighBeamActiveStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorHighBeamActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mExtHighBeamOnReqStatus == ExtHighBeamOnReqStatus::ON) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorHighBeamActiveStat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorHighBeamActiveStat(stat);
    }

    // 5.1.4.19 자차 방향 지시등 - 왼쪽 (24)
    void updateConstantADASViewMainExteriorTurnLeftLampActiveStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::ON) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat::ON;
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat(stat);
    }

    // 5.1.4.20 자차 방향 지시등 - 오른쪽 (25)
    void updateConstantADASViewMainExteriorTurnRightLampActiveStatus() {
        SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat stat =
            SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat::OFF;

        if (mIsIgnOn == true) {
            if (mExtTurnRightLampStatus == ExtTurnRightLampStatus::ON) {
                stat = SFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat::ON;
            }
        }
        setSFCDetail_Contents_ADAS_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat(stat);
    }

    // 5.2.2 CRAWL 요약 정보 표시
    void updateEventCrawlSummary() {
        SFCDetail_Contents_ADAS_2_0EventCrawlStat stat = SFCDetail_Contents_ADAS_2_0EventCrawlStat::OFF;

        if (mIsIgnOn == true) {
            if ((mCrawlActiveStatus == CrawlActiveStatus::READY || mCrawlActiveStatus == CrawlActiveStatus::ACTIVE ||
                 mCrawlActiveStatus == CrawlActiveStatus::OVERRIDE || mCrawlActiveStatus == CrawlActiveStatus::PAUSE ||
                 mCrawlActiveStatus == CrawlActiveStatus::TEMP) &&
                (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP1 ||
                 mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP2 ||
                 mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP3 ||
                 mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP4 ||
                 mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP5)) {
                stat = SFCDetail_Contents_ADAS_2_0EventCrawlStat::ON;
            }
        }

        setSFCDetail_Contents_ADAS_2_0EventCrawlStat(stat);
    }

    // 5.10.5.2.3 ADAS 팝업
    // 5.10.5.2.3.1 CC/MSLA 팝업
    void updateEventCcPopup() {
        std::string eventId;

        if (mIsIgnOn == true) {
            switch (mCruiseInfoDisplayReqStatus) {
                case CruiseInfoDisplayReqStatus::AUTO_CANCELED:
                    eventId = "E71204";
                    break;
                case CruiseInfoDisplayReqStatus::NO_CONDITION:
                    eventId = "E71205";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Cc.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventCcStat(SFCDetail_Contents_ADAS_2_0EventCcStat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventCcID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventCcStat(SFCDetail_Contents_ADAS_2_0EventCcStat::ON);
        }
    }

    void updateEventMslaPopup() {
        std::string eventId;

        if (mIsIgnOn == true) {
            switch (mSpeedLimitInfoDisplayReqStatus) {
                case SpeedLimitInfoDisplayReqStatus::AUTO_CANCELED:
                    eventId = "E71208";
                    break;
                case SpeedLimitInfoDisplayReqStatus::NO_CONDITION:
                    eventId = "E71209";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Msla.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventMslaStat(SFCDetail_Contents_ADAS_2_0EventMslaStat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventMslaID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventMslaStat(SFCDetail_Contents_ADAS_2_0EventMslaStat::ON);
        }
    }

    // 5.2.3.2 1그룹 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEvent1GroupPopup1_1() {
        std::string eventId;

        if (mIsIgnOn == true) {
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
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Event1_1Group1.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group1Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group1Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group1ID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group1Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group1Stat::ON);
        }
    }

    // 5.2.3.3 1그룹 팝업 1-2 (FR_CMR, ADAS_DRV)
    void updateEvent1GroupPopup1_2() {
        std::string eventId;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
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
                case Group1AdasWarning1_2Status::DAW_TAKE_REST_WITH_OK_BUTTON:
                    eventId = "E71784";
                    break;
                case Group1AdasWarning1_2Status::TD_LV2:
                    eventId = "E71773";
                    break;
                case Group1AdasWarning1_2Status::EMERGENCY_STOP_INACTIVE:
                    eventId = "E71810";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Event1_2Group1.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group1Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_2Group1Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group1ID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group1Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_2Group1Stat::ON);
        }
    }

    // 5.2.3.4 4그룹 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEvent4GroupPopup1_1() {
        std::string eventId;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mGroup4AdasWarning1_1Status) {
                case Group4AdasWarning1_1Status::SCC_MRM:
                    eventId = "E71731";
                    break;
                case Group4AdasWarning1_1Status::SCC_ATTENTION_WARNING:
                    eventId = "E71732";
                    break;
                case Group4AdasWarning1_1Status::SCC_AUTOMATIC_OFF:
                    eventId = "E71733";
                    break;
                case Group4AdasWarning1_1Status::SCC_CONDITION_NOT_MET:
                    eventId = "E71734";
                    break;
                case Group4AdasWarning1_1Status::LFA_CONDITION_NOT_MET:
                    eventId = "E71746";
                    break;
                case Group4AdasWarning1_1Status::SCC_STOP_CONTROL:
                    eventId = "E71735";
                    break;
                case Group4AdasWarning1_1Status::CONNECTING_TRAILER_OFF:
                    eventId = "E71736";
                    break;
                case Group4AdasWarning1_1Status::DAW_LEAD_VEHICLE_DEPARTURE:
                    eventId = "E71737";
                    break;
                case Group4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_REGULATIONS:
                    eventId = "E71738";
                    break;
                case Group4AdasWarning1_1Status::OFF_DUE_TO_LIMITATION_ON_DRIVER_MONITORING:
                    eventId = "E71748";
                    break;
                case Group4AdasWarning1_1Status::SCC_CONDITION_NOT_MET_BY_MRM:
                    eventId = "E71744";
                    break;
                case Group4AdasWarning1_1Status::HANDS_FREE_INITIAL_ACTIVATION:
                    eventId = "E71779";
                    break;
                case Group4AdasWarning1_1Status::LFA_CONDITION_NOT_MET_BY_EMERGENCY_STOP:
                    eventId = "E71745";
                    break;
                case Group4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_IIHS:
                    eventId = "E71749";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Event1_1Group4.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group4Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group4Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group4ID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group4Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group4Stat::ON);
        }
    }

    // 5.2.3.5 2그룹 미니 팝업 1-1
    void updateEvent2GroupPopup1_1() {
        std::string eventId;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
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
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Event1_1Group2.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group2Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group2Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group2ID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventEvent1_1Group2Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_1Group2Stat::ON);
        }
    }

    // 5.2.3.6 2그룹 미니 팝업 1-2
    void updateEvent2GroupPopup1_2() {
        std::string eventId;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mGroup2AdasWarning1_2Status) {
                case Group2AdasWarning1_2Status::STEERING_CONTROL_DETECTED:
                    eventId = "E72644";
                    break;
                case Group2AdasWarning1_2Status::ACCELERATOR_PEDAL_CONTROL_DETECTED:
                    eventId = "E72645";
                    break;
                case Group2AdasWarning1_2Status::CONDITION_NOT_MET:
                    eventId = "E72646";
                    break;
                case Group2AdasWarning1_2Status::MAXIMUM_SPEED_EXCEEDED:
                    eventId = "E72647";
                    break;
                case Group2AdasWarning1_2Status::MAXIMUM_SET_SPEED_EXCEEDED:
                    eventId = "E72648";
                    break;
                case Group2AdasWarning1_2Status::IC_JC_AHEAD:
                    eventId = "E72649";
                    break;
                case Group2AdasWarning1_2Status::TOLLGATE_AHEAD:
                    eventId = "E72650";
                    break;
                case Group2AdasWarning1_2Status::NOT_SUPPORTED_AREA_AHEAD:
                    eventId = "E72651";
                    break;
                case Group2AdasWarning1_2Status::SUPPORTED_ROAD_ENDING_AHEAD:
                    eventId = "E72652";
                    break;
                case Group2AdasWarning1_2Status::CURVED_ROAD:
                    eventId = "E72653";
                    break;
                case Group2AdasWarning1_2Status::PEDESTRIAN_OR_CYCLIST_DETECTED:
                    eventId = "E72654";
                    break;
                case Group2AdasWarning1_2Status::LANE_DEPARTURE_DETECTED:
                    eventId = "E72655";
                    break;
                case Group2AdasWarning1_2Status::DRIVER_NOT_DETECTED:
                    eventId = "E72656";
                    break;
                case Group2AdasWarning1_2Status::DEACTIVATION_LANE_FOLLOWING_ASSIST:
                    eventId = "E72657";
                    break;
                case Group2AdasWarning1_2Status::DEACTIVATION_HIGHWAY_DRIVING_ASSIST:
                    eventId = "E72658";
                    break;
                case Group2AdasWarning1_2Status::SUGGEST_LANE_CHANGE_TO_LEFT_SIDE:
                    eventId = "E72659";
                    break;
                case Group2AdasWarning1_2Status::SUGGEST_LANE_CHANGE_TO_RIGHT_SIDE:
                    eventId = "E72660";
                    break;
                case Group2AdasWarning1_2Status::UNSUPPORTED_WEATHER_CONDITIONS:
                    eventId = "E72663";
                    break;
                case Group2AdasWarning1_2Status::ROAD_CONSTRUCTION_AHEAD:
                    eventId = "E72664";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.Event1_2Group2.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group2Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_2Group2Stat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group2ID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventEvent1_2Group2Stat(SFCDetail_Contents_ADAS_2_0EventEvent1_2Group2Stat::ON);
        }
    }

    // 5.2.3.7 대형 풀팝업
    void updateEventLargeFullPopup() {
        std::string eventId;

        if (mIsIgnOn == true && mHudAdasInfoOnOffStatus == HudAdasInfoOnOffStatus::ENABLE) {
            switch (mNTFHandsOffStatus) {
                case NTFHandsOffStatus::SCC2_DCA:
                    eventId = "E71782";
                    break;
                case NTFHandsOffStatus::SCC2_EMERGENCY_STOP:
                    eventId = "E71783";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.LargeFullPopup.ID, prevEventId);
        if (!prevEventId.empty() && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventLargeFullPopupStat(SFCDetail_Contents_ADAS_2_0EventLargeFullPopupStat::OFF);
            flushLastGroup();
        }
        if (!eventId.empty()) {
            setSFCDetail_Contents_ADAS_2_0EventLargeFullPopupID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventLargeFullPopupStat(SFCDetail_Contents_ADAS_2_0EventLargeFullPopupStat::ON);
        }
    }

    // 5.2.4 CRAWL 팝업
    void updateEventCrawlPopup() {
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
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.Detail_Contents_ADAS_2_0.Event.CrawlWarn.ID, prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCDetail_Contents_ADAS_2_0EventCrawlWarnStat(SFCDetail_Contents_ADAS_2_0EventCrawlWarnStat::OFF);
            flushLastGroup();
        }
        if (eventId.empty() == false) {
            setSFCDetail_Contents_ADAS_2_0EventCrawlWarnID(eventId);
            setSFCDetail_Contents_ADAS_2_0EventCrawlWarnStat(SFCDetail_Contents_ADAS_2_0EventCrawlWarnStat::ON);
        }
    }

    using ConfigNewHUD = ArgumentsConfigChanged::Inter_ConfigNewHUD;
    using ConfigHUD = ArgumentsConfigChanged::Inter_ConfigHUD;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigAdasScc = ArgumentsConfigChanged::Inter_ConfigAdasSCC;
    using CcColorStat = ArgumentsCcColorStatChanged::CcColorStat;
    using CcMslaSpeedStat = SFCDetail_Contents_ADAS_2_0ConstantCcMslaSpeedStat;
    using HudAdasInfoOnOffStatus = ArgumentsHudAdasInfoOnOffStatusChanged::Inter_HudAdasInfoOnOffStatus;
    using CcOnOffStat = ArgumentsCcChanged::Inter_CcOnOffStat;
    using CruiseSetIndReqStatus = ArgumentsCcChanged::Input_CruiseSetIndReqStatus;
    using CcActiveStat = ArgumentsCcChanged::Inter_CcActiveStat;
    using CruiseOverrideReqStatus = ArgumentsCcChanged::Input_CruiseOverrideReqStatus;
    using MslaOnOffStat = ArgumentsMslaChanged::Inter_MslaOnOffStat;
    using SpeedLimitReadyStatus = ArgumentsMslaChanged::Input_SpeedLimitReadyStatus;
    using SmvSetSpeedStatus = ArgumentsMslaChanged::Input_SmvSetSpeedStatus;
    using MslaActiveStat = ArgumentsMslaChanged::Inter_MslaActiveStat;
    using MslaOverride = ArgumentsMslaChanged::Inter_Mslaoverride;
    using NccCameraOperStatus = ArgumentsNccCameraOperStatusChanged::Input_NccCameraOperStatus;
    using CrawlActiveStatus = ArgumentsCrawlActiveChanged::Input_CrawlActiveStatus;
    using CrawlActiveStepStatus = ArgumentsCrawlActiveChanged::Input_CrawlActiveStepStatus;
    using INDNsccSymbolStatus = ArgumentsIndNsccSymbolStatusChanged::Input_INDNsccSymbolStatus;
    using INDSccSymbol1Status = ArgumentsIndSccSymbolStatusChanged::Input_INDSccSymbol1Status;
    using INDSccSymbol2Status = ArgumentsIndSccSymbolStatusChanged::Input_INDSccSymbol2Status;
    using INDSetSpeed1Status = ArgumentsIndSetSpeedStatusChanged::Input_INDSetSpeed1Status;
    using INDSetSpeed2Status = ArgumentsIndSetSpeedStatusChanged::Input_INDSetSpeed2Status;
    using INDLfaSymbol1Status = ArgumentsIndLfaSymbolStatusChanged::Input_INDLfaSymbol1Status;
    using INDLfaSymbol2Status = ArgumentsIndLfaSymbolStatusChanged::Input_INDLfaSymbol2Status;
    using INDDriverHandsOnOff1Status = ArgumentsIndDriverHandsOnOffStatusChanged::Input_INDDriverHandsOnOff1Status;
    using INDDriverHandsOnOff2Status = ArgumentsIndDriverHandsOnOffStatusChanged::Input_INDDriverHandsOnOff2Status;
    using INDLcaLeftSymbol1Status = ArgumentsIndLcaLeftSymbolStatusChanged::Input_INDLcaLeftSymbol1Status;
    using INDLcaLeftSymbol2Status = ArgumentsIndLcaLeftSymbolStatusChanged::Input_INDLcaLeftSymbol2Status;
    using INDLcaRightSymbol1Status = ArgumentsIndLcaRightSymbolStatusChanged::Input_INDLcaRightSymbol1Status;
    using INDLcaRightSymbol2Status = ArgumentsIndLcaRightSymbolStatusChanged::Input_INDLcaRightSymbol2Status;
    using VCLeftLineStatus = ArgumentsVcLeftLineStatusChanged::Input_VCLeftLineStatus;
    using VCRightLineStatus = ArgumentsVcRightLineStatusChanged::Input_VCRightLineStatus;
    using VCLeftLeftLineStatus = ArgumentsVcLeftLeftLineStatusChanged::Input_VCLeftLeftLineStatus;
    using VCRightRightLineStatus = ArgumentsVcRightRightLineStatusChanged::Input_VCRightRightLineStatus;
    using VCVehicleDistanceStatus = ArgumentsVcVehicleDistanceChanged::Input_VCVehicleDistanceStatus;
    using VCVehicleDistanceLevelValue = ArgumentsVcVehicleDistanceChanged::Input_VCVehicleDistanceLevelValue;
    using VCCenterRoadSurfaceStatus = ArgumentsVcCenterRoadSurfaceStatusChanged::Input_VCCenterRoadSurfaceStatus;
    using VCFrontLeftObjectMovingDirectionStatus =
        ArgumentsVcObjectMovingDirectionStatusChanged::Input_VCFrontLeftObjectMovingDirectionStatus;
    using VCFrontRightObjectMovingDirectionStatus =
        ArgumentsVcObjectMovingDirectionStatusChanged::Input_VCFrontRightObjectMovingDirectionStatus;
    using VCFrontRadarWaveStatus = ArgumentsRadarWaveStatusChanged::Input_VCFrontRadarWaveStatus;
    using VCFrontLeftRadarWaveStatus = ArgumentsRadarWaveStatusChanged::Input_VCFrontLeftRadarWaveStatus;
    using VCFrontRightRadarWaveStatus = ArgumentsRadarWaveStatusChanged::Input_VCFrontRightRadarWaveStatus;
    using VCRearLeftRadarWave1Status = ArgumentsRadarWaveStatusChanged::Input_VCRearLeftRadarWave1Status;
    using VCRearLeftRadarWave2Status = ArgumentsRadarWaveStatusChanged::Input_VCRearLeftRadarWave2Status;
    using VCRearRightRadarWave1Status = ArgumentsRadarWaveStatusChanged::Input_VCRearRightRadarWave1Status;
    using VCRearRightRadarWave2Status = ArgumentsRadarWaveStatusChanged::Input_VCRearRightRadarWave2Status;
    using VCLeftLaneChangeDirectionStatus =
        ArgumentsVcLeftLaneChangeDirectionStatusChanged::Input_VCLeftLaneChangeDirectionStatus;
    using VCLaneChangeRectangleStatus = ArgumentsVcLaneChangedRectangleStatus::Input_VCLaneChangeRectangleStatus;
    using VCRightLaneChangeDirectionStatus =
        ArgumentsVcRightLaneChangeDirectionStatusChanged::Input_VCRightLaneChangeDirectionStatus;
    using VCLaneChangeAvailableStatus = ArgumentsVcLaneChangeAvailableStatusChanged::Input_VCLaneChangeAvailableStatus;
    using VCSurroundingObject01Status = ArgumentsVcSurroundingObject01to05StatusChanged::Input_VCSurroundingObject01Status;
    using VCSurroundingObject02Status = ArgumentsVcSurroundingObject01to05StatusChanged::Input_VCSurroundingObject02Status;
    using VCSurroundingObject03Status = ArgumentsVcSurroundingObject01to05StatusChanged::Input_VCSurroundingObject03Status;
    using VCSurroundingObject04Status = ArgumentsVcSurroundingObject01to05StatusChanged::Input_VCSurroundingObject04Status;
    using VCSurroundingObject05Status = ArgumentsVcSurroundingObject01to05StatusChanged::Input_VCSurroundingObject05Status;
    using VCSurroundingObject06Status = ArgumentsVcSurroundingObject06to10StatusChanged::Input_VCSurroundingObject06Status;
    using VCSurroundingObject07Status = ArgumentsVcSurroundingObject06to10StatusChanged::Input_VCSurroundingObject07Status;
    using VCSurroundingObject08Status = ArgumentsVcSurroundingObject06to10StatusChanged::Input_VCSurroundingObject08Status;
    using VCSurroundingObject09Status = ArgumentsVcSurroundingObject06to10StatusChanged::Input_VCSurroundingObject09Status;
    using VCSurroundingObject10Status = ArgumentsVcSurroundingObject06to10StatusChanged::Input_VCSurroundingObject10Status;
    using VCSurroundingObject11Status = ArgumentsVcSurroundingObject11to15StatusChanged::Input_VCSurroundingObject11Status;
    using VCSurroundingObject12Status = ArgumentsVcSurroundingObject11to15StatusChanged::Input_VCSurroundingObject12Status;
    using VCSurroundingObject13Status = ArgumentsVcSurroundingObject11to15StatusChanged::Input_VCSurroundingObject13Status;
    using VCSurroundingObject14Status = ArgumentsVcSurroundingObject11to15StatusChanged::Input_VCSurroundingObject14Status;
    using VCSurroundingObject15Status = ArgumentsVcSurroundingObject11to15StatusChanged::Input_VCSurroundingObject15Status;
    using VCSurroundingObjectClassification01Status =
        ArgumentsVcSurroundingObjectClassification01to05StatusChanged::Input_VCSurroundingObjectClassification01Status;
    using VCSurroundingObjectClassification02Status =
        ArgumentsVcSurroundingObjectClassification01to05StatusChanged::Input_VCSurroundingObjectClassification02Status;
    using VCSurroundingObjectClassification03Status =
        ArgumentsVcSurroundingObjectClassification01to05StatusChanged::Input_VCSurroundingObjectClassification03Status;
    using VCSurroundingObjectClassification04Status =
        ArgumentsVcSurroundingObjectClassification01to05StatusChanged::Input_VCSurroundingObjectClassification04Status;
    using VCSurroundingObjectClassification05Status =
        ArgumentsVcSurroundingObjectClassification01to05StatusChanged::Input_VCSurroundingObjectClassification05Status;
    using VCSurroundingObjectClassification06Status =
        ArgumentsVcSurroundingObjectClassification06to10StatusChanged::Input_VCSurroundingObjectClassification06Status;
    using VCSurroundingObjectClassification07Status =
        ArgumentsVcSurroundingObjectClassification06to10StatusChanged::Input_VCSurroundingObjectClassification07Status;
    using VCSurroundingObjectClassification08Status =
        ArgumentsVcSurroundingObjectClassification06to10StatusChanged::Input_VCSurroundingObjectClassification08Status;
    using VCSurroundingObjectClassification09Status =
        ArgumentsVcSurroundingObjectClassification06to10StatusChanged::Input_VCSurroundingObjectClassification09Status;
    using VCSurroundingObjectClassification10Status =
        ArgumentsVcSurroundingObjectClassification06to10StatusChanged::Input_VCSurroundingObjectClassification10Status;
    using VCSurroundingObjectClassification11Status =
        ArgumentsVcSurroundingObjectClassification11to15StatusChanged::Input_VCSurroundingObjectClassification11Status;
    using VCSurroundingObjectClassification12Status =
        ArgumentsVcSurroundingObjectClassification11to15StatusChanged::Input_VCSurroundingObjectClassification12Status;
    using VCSurroundingObjectClassification13Status =
        ArgumentsVcSurroundingObjectClassification11to15StatusChanged::Input_VCSurroundingObjectClassification13Status;
    using VCSurroundingObjectClassification14Status =
        ArgumentsVcSurroundingObjectClassification11to15StatusChanged::Input_VCSurroundingObjectClassification14Status;
    using VCSurroundingObjectClassification15Status =
        ArgumentsVcSurroundingObjectClassification11to15StatusChanged::Input_VCSurroundingObjectClassification15Status;
    using VCEmergencyStreeingStatus = ArgumentsVcEmergencyStreeingStatusChanged::Input_VCEmergencyStreeingStatus;
    using VCAdditionalStatus = ArgumentsVcAdditionalStatusChanged::Input_VCAdditionalStatus;
    using BrakeLampControlCommand_Status = ArgumentsBrakeLampControlCommandStatusChanged::Input_BrakeLampControlCommand_Status;
    using ExtLowBeamOnReqStatus = ArgumentsExtLowBeamOnReqStatusChanged::Input_ExtLowBeamOnReqStatus;
    using ExtHighBeamOnReqStatus = ArgumentsExtHighBeamOnReqStatusChanged::Input_ExtHighBeamOnReqStatus;
    using ExtTurnLeftLampStatus = ArgumentsExtTurnLeftLampStatusChanged::Input_ExtTurnLeftLampStatus;
    using ExtTurnRightLampStatus = ArgumentsExtTurnRightLampStatusChanged::Input_ExtTurnRightLampStatus;
    using CruiseInfoDisplayReqStatus = ArgumentsInfoDisplayReqChanged::Input_CruiseInfoDisplayReqStatus;
    using SpeedLimitInfoDisplayReqStatus = ArgumentsInfoDisplayReqChanged::Input_SpeedLimitInfoDisplayReqStatus;
    using Group1AdasWarning1_1Status = ArgumentsGroup1AdasWarning1_1StatusChanged::Input_NTFGroup1AdasWarning1_1Status;
    using Group1AdasWarning1_2Status = ArgumentsGroup1AdasWarning1_2StatusChanged::Input_NTFGroup1AdasWarning1_2Status;
    using Group4AdasWarning1_1Status = ArgumentsGroup4AdasWarning1_1StatusChanged::Input_NTFGroup4AdasWarning1_1Status;
    using Group2AdasWarning1_1Status = ArgumentsGroup2AdasWarning1_1StatusChanged::Input_NTMGroup2AdasWarning1_1Status;
    using Group2AdasWarning1_2Status = ArgumentsGroup2AdasWarning1_2StatusChanged::Input_NTMGroup2AdasWarning1_2Status;
    using NTFHandsOffStatus = ArgumentsNtfHandsOffStatusChanged::Input_NTFHandsOffStatus;
    using CrawlWarnStatus = ArgumentsCrawlWarnStatusChanged::Input_CrawlWarnStatus;

    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigAdasScc mConfigAdasScc = ConfigAdasScc::OFF;
    CcColorStat mCcColorStat = CcColorStat::DISPLAY_OFF;
    CcMslaSpeedStat mCcMslaSpeedStat = CcMslaSpeedStat::DISPLAY_OFF;
    HudAdasInfoOnOffStatus mHudAdasInfoOnOffStatus = HudAdasInfoOnOffStatus::DISABLE;
    CcOnOffStat mCcOnOffStat = CcOnOffStat::OFF;
    CruiseSetIndReqStatus mCruiseSetIndReqStatus = CruiseSetIndReqStatus::UNHANDLED_TIMEOUT;
    CcActiveStat mCcActiveStat = CcActiveStat::OFF;
    CruiseOverrideReqStatus mCruiseOverrideReqStatus = CruiseOverrideReqStatus::OFF;
    MslaOnOffStat mMslaOnOffStat = MslaOnOffStat::OFF;
    SpeedLimitReadyStatus mSpeedLimitReadyStatus = SpeedLimitReadyStatus::NORMAL;
    SmvSetSpeedStatus mSmvSetSpeedStatus = SmvSetSpeedStatus::OFF;
    MslaActiveStat mMslaActiveStat = MslaActiveStat::OFF;
    MslaOverride mMslaOverride = MslaOverride::OFF;
    NccCameraOperStatus mNccCameraOperStatus = NccCameraOperStatus::OFF;
    CrawlActiveStatus mCrawlActiveStatus = CrawlActiveStatus::OFF;
    CrawlActiveStepStatus mCrawlActiveStepStatus = CrawlActiveStepStatus::OFF;
    INDNsccSymbolStatus mINDNsccSymbolStatus = INDNsccSymbolStatus::OFF;
    INDSccSymbol1Status mINDSccSymbol1Status = INDSccSymbol1Status::OFF;
    INDSccSymbol2Status mINDSccSymbol2Status = INDSccSymbol2Status::OFF;
    INDSetSpeed1Status mINDSetSpeed1Status = INDSetSpeed1Status::OFF;
    INDSetSpeed2Status mINDSetSpeed2Status = INDSetSpeed2Status::OFF;
    INDLfaSymbol1Status mINDLfaSymbol1Status = INDLfaSymbol1Status::OFF;
    INDLfaSymbol2Status mINDLfaSymbol2Status = INDLfaSymbol2Status::OFF;
    INDDriverHandsOnOff1Status mINDDriverHandsOnOff1Status = INDDriverHandsOnOff1Status::OFF;
    INDDriverHandsOnOff2Status mINDDriverHandsOnOff2Status = INDDriverHandsOnOff2Status::OFF;
    INDLcaLeftSymbol1Status mINDLcaLeftSymbol1Status = INDLcaLeftSymbol1Status::OFF;
    INDLcaLeftSymbol2Status mINDLcaLeftSymbol2Status = INDLcaLeftSymbol2Status::OFF;
    INDLcaRightSymbol1Status mINDLcaRightSymbol1Status = INDLcaRightSymbol1Status::OFF;
    INDLcaRightSymbol2Status mINDLcaRightSymbol2Status = INDLcaRightSymbol2Status::OFF;
    VCLeftLineStatus mVCLeftLineStatus = VCLeftLineStatus::OFF;
    VCRightLineStatus mVCRightLineStatus = VCRightLineStatus::OFF;
    VCLeftLeftLineStatus mVCLeftLeftLineStatus = VCLeftLeftLineStatus::OFF;
    VCRightRightLineStatus mVCRightRightLineStatus = VCRightRightLineStatus::OFF;
    VCVehicleDistanceStatus mVCVehicleDistanceStatus = VCVehicleDistanceStatus::OFF;
    VCVehicleDistanceLevelValue mVCVehicleDistanceLevelValue = VCVehicleDistanceLevelValue::OFF;
    VCCenterRoadSurfaceStatus mVCCenterRoadSurfaceStatus = VCCenterRoadSurfaceStatus::OFF;
    VCFrontLeftObjectMovingDirectionStatus mVCFrontLeftObjectMovingDirectionStatus = VCFrontLeftObjectMovingDirectionStatus::OFF;
    VCFrontRightObjectMovingDirectionStatus mVCFrontRightObjectMovingDirectionStatus =
        VCFrontRightObjectMovingDirectionStatus::OFF;
    VCFrontRadarWaveStatus mVCFrontRadarWaveStatus = VCFrontRadarWaveStatus::OFF;
    VCFrontLeftRadarWaveStatus mVCFrontLeftRadarWaveStatus = VCFrontLeftRadarWaveStatus::OFF;
    VCFrontRightRadarWaveStatus mVCFrontRightRadarWaveStatus = VCFrontRightRadarWaveStatus::OFF;
    VCRearLeftRadarWave1Status mVCRearLeftRadarWave1Status = VCRearLeftRadarWave1Status::OFF;
    VCRearLeftRadarWave2Status mVCRearLeftRadarWave2Status = VCRearLeftRadarWave2Status::OFF;
    VCRearRightRadarWave1Status mVCRearRightRadarWave1Status = VCRearRightRadarWave1Status::OFF;
    VCRearRightRadarWave2Status mVCRearRightRadarWave2Status = VCRearRightRadarWave2Status::OFF;
    VCLeftLaneChangeDirectionStatus mVCLeftLaneChangeDirectionStatus = VCLeftLaneChangeDirectionStatus::OFF;
    VCLaneChangeRectangleStatus mVCLaneChangeRectangleStatus = VCLaneChangeRectangleStatus::OFF;
    VCRightLaneChangeDirectionStatus mVCRightLaneChangeDirectionStatus = VCRightLaneChangeDirectionStatus::OFF;
    VCLaneChangeAvailableStatus mVCLaneChangeAvailableStatus = VCLaneChangeAvailableStatus::OFF;
    VCSurroundingObject01Status mVCSurroundingObject01Status = VCSurroundingObject01Status::OFF;
    VCSurroundingObject02Status mVCSurroundingObject02Status = VCSurroundingObject02Status::OFF;
    VCSurroundingObject03Status mVCSurroundingObject03Status = VCSurroundingObject03Status::OFF;
    VCSurroundingObject04Status mVCSurroundingObject04Status = VCSurroundingObject04Status::OFF;
    VCSurroundingObject05Status mVCSurroundingObject05Status = VCSurroundingObject05Status::OFF;
    VCSurroundingObject06Status mVCSurroundingObject06Status = VCSurroundingObject06Status::OFF;
    VCSurroundingObject07Status mVCSurroundingObject07Status = VCSurroundingObject07Status::OFF;
    VCSurroundingObject08Status mVCSurroundingObject08Status = VCSurroundingObject08Status::OFF;
    VCSurroundingObject09Status mVCSurroundingObject09Status = VCSurroundingObject09Status::OFF;
    VCSurroundingObject10Status mVCSurroundingObject10Status = VCSurroundingObject10Status::OFF;
    VCSurroundingObject11Status mVCSurroundingObject11Status = VCSurroundingObject11Status::OFF;
    VCSurroundingObject12Status mVCSurroundingObject12Status = VCSurroundingObject12Status::OFF;
    VCSurroundingObject13Status mVCSurroundingObject13Status = VCSurroundingObject13Status::OFF;
    VCSurroundingObject14Status mVCSurroundingObject14Status = VCSurroundingObject14Status::OFF;
    VCSurroundingObject15Status mVCSurroundingObject15Status = VCSurroundingObject15Status::OFF;
    VCSurroundingObject01Status mPrevVCSurroundingObject01Status = VCSurroundingObject01Status::OFF;
    VCSurroundingObject02Status mPrevVCSurroundingObject02Status = VCSurroundingObject02Status::OFF;
    VCSurroundingObject03Status mPrevVCSurroundingObject03Status = VCSurroundingObject03Status::OFF;
    VCSurroundingObject04Status mPrevVCSurroundingObject04Status = VCSurroundingObject04Status::OFF;
    VCSurroundingObject05Status mPrevVCSurroundingObject05Status = VCSurroundingObject05Status::OFF;
    VCSurroundingObject06Status mPrevVCSurroundingObject06Status = VCSurroundingObject06Status::OFF;
    VCSurroundingObject07Status mPrevVCSurroundingObject07Status = VCSurroundingObject07Status::OFF;
    VCSurroundingObject08Status mPrevVCSurroundingObject08Status = VCSurroundingObject08Status::OFF;
    VCSurroundingObject09Status mPrevVCSurroundingObject09Status = VCSurroundingObject09Status::OFF;
    VCSurroundingObject10Status mPrevVCSurroundingObject10Status = VCSurroundingObject10Status::OFF;
    VCSurroundingObject11Status mPrevVCSurroundingObject11Status = VCSurroundingObject11Status::OFF;
    VCSurroundingObject12Status mPrevVCSurroundingObject12Status = VCSurroundingObject12Status::OFF;
    VCSurroundingObject13Status mPrevVCSurroundingObject13Status = VCSurroundingObject13Status::OFF;
    VCSurroundingObject14Status mPrevVCSurroundingObject14Status = VCSurroundingObject14Status::OFF;
    VCSurroundingObject15Status mPrevVCSurroundingObject15Status = VCSurroundingObject15Status::OFF;
    VCSurroundingObjectClassification01Status mVCSurroundingObjectClassification01Status =
        VCSurroundingObjectClassification01Status::OFF;
    VCSurroundingObjectClassification02Status mVCSurroundingObjectClassification02Status =
        VCSurroundingObjectClassification02Status::OFF;
    VCSurroundingObjectClassification03Status mVCSurroundingObjectClassification03Status =
        VCSurroundingObjectClassification03Status::OFF;
    VCSurroundingObjectClassification04Status mVCSurroundingObjectClassification04Status =
        VCSurroundingObjectClassification04Status::OFF;
    VCSurroundingObjectClassification05Status mVCSurroundingObjectClassification05Status =
        VCSurroundingObjectClassification05Status::OFF;
    VCSurroundingObjectClassification06Status mVCSurroundingObjectClassification06Status =
        VCSurroundingObjectClassification06Status::OFF;
    VCSurroundingObjectClassification07Status mVCSurroundingObjectClassification07Status =
        VCSurroundingObjectClassification07Status::OFF;
    VCSurroundingObjectClassification08Status mVCSurroundingObjectClassification08Status =
        VCSurroundingObjectClassification08Status::OFF;
    VCSurroundingObjectClassification09Status mVCSurroundingObjectClassification09Status =
        VCSurroundingObjectClassification09Status::OFF;
    VCSurroundingObjectClassification10Status mVCSurroundingObjectClassification10Status =
        VCSurroundingObjectClassification10Status::OFF;
    VCSurroundingObjectClassification11Status mVCSurroundingObjectClassification11Status =
        VCSurroundingObjectClassification11Status::OFF;
    VCSurroundingObjectClassification12Status mVCSurroundingObjectClassification12Status =
        VCSurroundingObjectClassification12Status::OFF;
    VCSurroundingObjectClassification13Status mVCSurroundingObjectClassification13Status =
        VCSurroundingObjectClassification13Status::OFF;
    VCSurroundingObjectClassification14Status mVCSurroundingObjectClassification14Status =
        VCSurroundingObjectClassification14Status::OFF;
    VCSurroundingObjectClassification15Status mVCSurroundingObjectClassification15Status =
        VCSurroundingObjectClassification15Status::OFF;
    VCEmergencyStreeingStatus mVCEmergencyStreeingStatus = VCEmergencyStreeingStatus::OFF;
    VCAdditionalStatus mVCAdditionalStatus = VCAdditionalStatus::OFF;
    BrakeLampControlCommand_Status mBrakeLampControlCommand_Status = BrakeLampControlCommand_Status::OFF;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    ExtHighBeamOnReqStatus mExtHighBeamOnReqStatus = ExtHighBeamOnReqStatus::OFF;
    ExtTurnLeftLampStatus mExtTurnLeftLampStatus = ExtTurnLeftLampStatus::OFF;
    ExtTurnRightLampStatus mExtTurnRightLampStatus = ExtTurnRightLampStatus::OFF;
    CruiseInfoDisplayReqStatus mCruiseInfoDisplayReqStatus = CruiseInfoDisplayReqStatus::OFF;
    SpeedLimitInfoDisplayReqStatus mSpeedLimitInfoDisplayReqStatus = SpeedLimitInfoDisplayReqStatus::OFF;
    Group1AdasWarning1_1Status mGroup1AdasWarning1_1Status = Group1AdasWarning1_1Status::OFF;
    Group1AdasWarning1_2Status mGroup1AdasWarning1_2Status = Group1AdasWarning1_2Status::OFF;
    Group4AdasWarning1_1Status mGroup4AdasWarning1_1Status = Group4AdasWarning1_1Status::OFF;
    Group2AdasWarning1_1Status mGroup2AdasWarning1_1Status = Group2AdasWarning1_1Status::OFF;
    Group2AdasWarning1_2Status mGroup2AdasWarning1_2Status = Group2AdasWarning1_2Status::OFF;
    NTFHandsOffStatus mNTFHandsOffStatus = NTFHandsOffStatus::OFF;
    CrawlWarnStatus mCrawlWarnStatus = CrawlWarnStatus::OFF;

    HBool mIsIgnOn = false;
    HBool mIsConfigOn = false;

    HUInt64 mPrivateCcSldSetSpeedValue = 0;
    HUInt64 mCcOnlySetSpeedValue = 0;
    HUInt64 mCcSldSetSpeedValue = 0;
    HUInt64 mCcSldMaxSpeedValue = 0;
    HUInt64 mINDSetSpeed1Value = 0;
    HUInt64 mINDSetSpeed2Value = 0;
    HUInt64 mVCLeftLineOffsetValue = 0;
    HUInt64 mVCDrivingLaneRadiusValue = 0;
    HUInt64 mVCRightLineOffsetValue = 0;
    HUInt64 mVCVehicleDistanceValue = 0;
    HUInt64 mVCLaneChangeRectangleLateralPositionValue = 0;
    HUInt64 mVCSurroundingObjectLateralPosition01Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition02Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition03Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition04Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition05Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition06Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition07Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition08Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition09Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition10Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition11Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition12Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition13Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition14Value = 0;
    HUInt64 mVCSurroundingObjectLateralPosition15Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition01Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition02Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition03Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition04Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition05Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition06Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition07Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition08Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition09Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition10Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition11Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition12Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition13Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition14Value = 0;
    HUInt64 mVCSurroundingObjectLongitudinalPosition15Value = 0;
    HUInt64 mVCLeftLeftLineOffsetValue = 0;
    HUInt64 mVCRightRightLineOffsetValue = 0;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
    static constexpr size_t kType0 = 0;
    static constexpr size_t kType1 = 1;
    static constexpr size_t kType2 = 2;
    static constexpr size_t kSurroundingObjectSize = 15;
    static constexpr size_t kSurroundingObjectInfoElementSize = 3;

    // mSurroundingObjectColorInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: VCSurroundingObject01Status, ..., VCSurroundingObject15Status
    // Type1(for Type0's prev values): VCSurroundingObject01Status, ..., VCSurroundingObject15Status
    // Type2: std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat&5>, ...,
    //        std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat&)>
    std::tuple<
        std::tuple<std::reference_wrapper<VCSurroundingObject01Status>, std::reference_wrapper<VCSurroundingObject01Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject02Status>, std::reference_wrapper<VCSurroundingObject02Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor02Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject03Status>, std::reference_wrapper<VCSurroundingObject03Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor03Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject04Status>, std::reference_wrapper<VCSurroundingObject04Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor04Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject05Status>, std::reference_wrapper<VCSurroundingObject05Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor05Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject06Status>, std::reference_wrapper<VCSurroundingObject06Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor06Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject07Status>, std::reference_wrapper<VCSurroundingObject07Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor07Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject08Status>, std::reference_wrapper<VCSurroundingObject08Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor08Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject09Status>, std::reference_wrapper<VCSurroundingObject09Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor09Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject10Status>, std::reference_wrapper<VCSurroundingObject10Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor10Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject11Status>, std::reference_wrapper<VCSurroundingObject11Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor11Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject12Status>, std::reference_wrapper<VCSurroundingObject12Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor12Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject13Status>, std::reference_wrapper<VCSurroundingObject13Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor13Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject14Status>, std::reference_wrapper<VCSurroundingObject14Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor14Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject15Status>, std::reference_wrapper<VCSurroundingObject15Status>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat&)>>>
        mSurroundingObjectColorInfoTuple = std::make_tuple(
            std::make_tuple(
                std::ref(mVCSurroundingObject01Status), std::ref(mPrevVCSurroundingObject01Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor01Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject02Status), std::ref(mPrevVCSurroundingObject02Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor02Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject03Status), std::ref(mPrevVCSurroundingObject03Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor03Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject04Status), std::ref(mPrevVCSurroundingObject04Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor04Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject05Status), std::ref(mPrevVCSurroundingObject05Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor05Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject06Status), std::ref(mPrevVCSurroundingObject06Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor06Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject07Status), std::ref(mPrevVCSurroundingObject07Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor07Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject08Status), std::ref(mPrevVCSurroundingObject08Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor08Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject09Status), std::ref(mPrevVCSurroundingObject09Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor09Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject10Status), std::ref(mPrevVCSurroundingObject10Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor10Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject11Status), std::ref(mPrevVCSurroundingObject11Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor11Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject12Status), std::ref(mPrevVCSurroundingObject12Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor12Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject13Status), std::ref(mPrevVCSurroundingObject13Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor13Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject14Status), std::ref(mPrevVCSurroundingObject14Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor14Stat,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObject15Status), std::ref(mPrevVCSurroundingObject15Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectColor15Stat,
                    this, std::placeholders::_1)));

    // mSurroundingObjectClassInfoTuple = std::tuple<std::pair<Type0, Type1>, ... >
    // Type0: VCSurroundingObjectClassification01Status, ..., VCSurroundingObjectClassification15Status
    // Type1: std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat&)>, ...,
    //        std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat&)>
    std::tuple<
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification01Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification02Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass02Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification03Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass03Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification04Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass04Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification05Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass05Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification06Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass06Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification07Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass07Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification08Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass08Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification09Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass09Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification10Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass10Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification11Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass11Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification12Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass12Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification13Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass13Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification14Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass14Stat&)>>,
        std::pair<std::reference_wrapper<VCSurroundingObjectClassification15Status>,
                  std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat&)>>>
        mSurroundingObjectClassInfoTuple = std::make_tuple(
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification01Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass01Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification02Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass02Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification03Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass03Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification04Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass04Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification05Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass05Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification06Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass06Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification07Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass07Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification08Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass08Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification09Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass09Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification10Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass10Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification11Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass11Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification12Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass12Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification13Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass13Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification14Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass14Stat,
                    this, std::placeholders::_1)),
            std::make_pair(
                std::ref(mVCSurroundingObjectClassification15Status),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectClass15Stat,
                    this, std::placeholders::_1)));

    // mSurroundingObjectLatInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat&)>, ...,
    //        std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat&)>,
                   std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLatInfoTuple = std::make_tuple(
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition01Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat01Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition02Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat02Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition03Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat03Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition04Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat04Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition05Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat05Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition06Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat06Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition07Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat07Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition08Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat08Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition09Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat09Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition10Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat10Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition11Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat11Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition12Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat12Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition13Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat13Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition14Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat14Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLateralPosition15Value),
                std::bind(&Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Stat,
                          this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLat15Value,
                    this, std::placeholders::_1)));

    // mSurroundingObjectLongInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat&)>, ...,
    //        std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Stat&)>,
                   std::function<std::string(const HInt64&)>>,
        std::tuple<std::reference_wrapper<HUInt64>,
                   std::function<std::string(const SFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat&)>,
                   std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLongInfoTuple = std::make_tuple(
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition01Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong01Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition02Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong02Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition03Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong03Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition04Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong04Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition05Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong05Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition06Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong06Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition07Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong07Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition08Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong08Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition09Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong09Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition10Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong10Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition11Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong11Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition12Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong12Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition13Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong13Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition14Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong14Value,
                    this, std::placeholders::_1)),
            std::make_tuple(
                std::ref(mVCSurroundingObjectLongitudinalPosition15Value),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Stat,
                    this, std::placeholders::_1),
                std::bind(
                    &Detail_Contents_ADAS_2_0Base::setSFCDetail_Contents_ADAS_2_0ConstantADASViewSurroundingObjectLong15Value,
                    this, std::placeholders::_1)));
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_ADAS_2_0_H
