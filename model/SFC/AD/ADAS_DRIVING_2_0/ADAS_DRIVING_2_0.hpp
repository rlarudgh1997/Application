/**
 * @file ADAS_DRIVING_2_0.hpp
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
#ifndef SFSS_ADAS_DRIVING_2_0_H
#define SFSS_ADAS_DRIVING_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "ADAS_DRIVING_2_0Base.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 28.0.0
// Reference : [AD081] ADAS_DRIVING_2_0. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_DRIVING_2_0 : public ADAS_DRIVING_2_0Base {
public:
    ADAS_DRIVING_2_0() = default;
    ~ADAS_DRIVING_2_0() override = default;
    ADAS_DRIVING_2_0(const ADAS_DRIVING_2_0& other) = delete;
    ADAS_DRIVING_2_0(ADAS_DRIVING_2_0&& other) noexcept = delete;
    ADAS_DRIVING_2_0& operator=(const ADAS_DRIVING_2_0& other) = delete;
    ADAS_DRIVING_2_0& operator=(ADAS_DRIVING_2_0&& other) noexcept = delete;

    void onInitialize() override {
        // 5.2.5.3.7 하이빔 보조 고장 - NT50015
        setSFCADAS_DRIVING_2_0EventHbaFailureID("NT50015");

        // 5.2.5.3.9 차로 유지 보조 고장 - NT50018
        setSFCADAS_DRIVING_2_0EventLfaFailureID("NT50018");

        // 5.2.5.3.10 고속도로 주행 보조 고장 - NT50019
        setSFCADAS_DRIVING_2_0EventHdaFailureID("NT50019");

        // 5.2.5.4.3 1그룹 풀 팝업 2-1 (RR_C_RDR, ADAS_DRV)
        setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1ID("E52030");
        setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1LinkedSoundType(
            SFCADAS_DRIVING_2_0EventGroup1FullPopup2_1LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1LinkedSoundRepeatCount(1);

        // 5.2.5.4.6 7그룹 풀 팝업 – 사이드 미러 고장
        setSFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureID("E52054");
        setSFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundType(
            SFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureLinkedSoundRepeatCount(1);

        // 5.2.5.4.7 7그룹 풀 팝업 – 운전자 보조 시스템 고장 1
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure1LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure1LinkedSoundType(
            SFCADAS_DRIVING_2_0EventDriverAssistFailure1LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure1LinkedSoundRepeatCount(1);

        // 5.2.5.4.8 7그룹 풀 팝업 – 운전자 보조 시스템 고장 2
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure2LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure2LinkedSoundType(
            SFCADAS_DRIVING_2_0EventDriverAssistFailure2LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_DRIVING_2_0EventDriverAssistFailure2LinkedSoundRepeatCount(1);

        // 5.2.6.1 경고음 1-1
        setSFCADAS_DRIVING_2_0SoundWarning1_1Type(SFCADAS_DRIVING_2_0SoundWarning1_1Type::INFINITE);

        // 5.2.6.4 경고음 1-4
        setSFCADAS_DRIVING_2_0SoundWarning1_4ID("SND_ADAS_PopUpInform2_Fixed");
        setSFCADAS_DRIVING_2_0SoundWarning1_4Type(SFCADAS_DRIVING_2_0SoundWarning1_4Type::REPEAT_COUNT);
        setSFCADAS_DRIVING_2_0SoundWarning1_4RepeatCount(1);

        // 5.2.6.5 경고음 1-5
        setSFCADAS_DRIVING_2_0SoundWarning1_5ID("SND_ADAS_LKAWarn_Auto");
        setSFCADAS_DRIVING_2_0SoundWarning1_5Type(SFCADAS_DRIVING_2_0SoundWarning1_5Type::INFINITE);

        // 5.2.6.6 경고음 2-1
        setSFCADAS_DRIVING_2_0SoundWarning2_1ID("SND_ADAS_BCW_Auto");
        setSFCADAS_DRIVING_2_0SoundWarning2_1Type(SFCADAS_DRIVING_2_0SoundWarning2_1Type::INFINITE);

        if (mOneShotTimerPriorityISLA.create(kTimerInterval300ms, this, &ADAS_DRIVING_2_0::onOneShotTimerISLAChanged, false) ==
            false) {
            DWarning() << "[ADAS_DRIVING_2_0] mOneShotTimerPriorityISLA Create Failed";
        }

        if (mOneShotTimerPriorityDIS.create(kTimerInterval300ms, this, &ADAS_DRIVING_2_0::onOneShotTimerDISChanged, false) ==
            false) {
            DWarning() << "[ADAS_DRIVING_2_0] mOneShotTimerPriorityDIS Create Failed";
        }

#ifdef ENABLE_GCOV_ON
        mOneShotTimerPriorityDIS.setTimerStatusCallback(&ADAS_DRIVING_2_0::updateOneShotTimerPriorityDISStatus);
        mOneShotTimerPriorityISLA.setTimerStatusCallback(&ADAS_DRIVING_2_0::updateOneShotTimerPriorityISLAStatus);
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
        updateEventGroup7FullPopup1();
        updateConstantForwardSafetyFailureStat();
    }

    void onConfigFca2Changed(const ArgumentsConfigFca2Changed& args) {
        mConfigAdasFCA2 = args.mInter_ConfigAdasFCA2;
        updateTelltaleFCA();
        updateTelltaleELK();
        updateConstantForwardSafetyFailureStat();
        updateConstantForwardSideSafetyFailureStat();
    }

    void onConfigLkaChanged(const ArgumentsConfigLkaChanged& args) {
        mConfigAdasLKA = args.mInter_ConfigAdasLKA;
        updateTelltaleLKA();
        updateEventGroup7FullPopup1();
        updateConstantLaneSafetyFailureStat();
    }

    void onConfigDawChanged(const ArgumentsConfigDawChanged& args) {
        mConfigAdasDAW = args.mInter_ConfigAdasDAW;
        updateTelltaleDAW();
        updateConstantDawFailureStat();
    }

    void onConfigIlcuIfsChanged(const ArgumentsConfigIlcuIfsChanged& args) {
        mConfigILCU = args.mInter_ConfigILCU;
        mConfigIFS = args.mInter_ConfigIFS;
        updateTelltaleHBA();
    }

    void onConfigHbaChanged(const ArgumentsConfigHbaChanged& args) {
        mConfigNewHBA = args.mInter_ConfigNewHBA;
        updateTelltaleHBA();
        updateConstantHbaFailureStat();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateConstantISLAMainSpeedStat();
    }

    void onConfigIslaChanged(const ArgumentsConfigIslaChanged& args) {
        mConfigAdasISLA = args.mInter_ConfigAdasISLA;
        updateConstantISLAMainSpeedStat();
        updateConstantIslaFailureStat();
    }

    void onConfigBcwChanged(const ArgumentsConfigBcwChanged& args) {
        mConfigNewBCW = args.mInter_ConfigNewBCW;
        updateEventGroup7FullPopup2();
        updateConstantBlindSpotSafetyFailureStat();
    }

    void onConfigAdasSCCChanged(const ArgumentsConfigAdasSCCChanged& args) {
        mConfigAdasSCC = args.mInter_ConfigAdasSCC;
        updateConstantSccFailureStat();
    }

    void onConfigAdasLFAChanged(const ArgumentsConfigAdasLFAChanged& args) {
        mConfigAdasLFA = args.mInter_ConfigAdasLFA;
        updateConstantLfaFailureStat();
    }

    void onConfigAdasHDAChanged(const ArgumentsConfigAdasHDAChanged& args) {
        mConfigAdasHDA = args.mInter_ConfigAdasHDA;
        updateConstantHdaFailureStat();
    }

    void onConfigAdasHDA2Changed(const ArgumentsConfigAdasHDA2Changed& args) {
        mConfigAdasHDA2 = args.mInter_ConfigAdasHDA2;
        updateConstantHdaFailureStat();
        updateConstantLcaFailureStat();
    }

    void onConfigAdasHDA3Changed(const ArgumentsConfigAdasHDA3Changed& args) {
        mConfigAdasHDA3 = args.mInter_ConfigAdasHDA3;
        updateConstantHdaFailureStat();
        updateConstantLcaFailureStat();
        updateConstantHdaPlusFailureStat();
    }

    void onConfigAdasEmergencyStopChanged(const ArgumentsConfigAdasEmergencyStopChanged& args) {
        mConfigAdasEmergencyStop = args.mInter_ConfigAdasEmergencyStop;
        updateConstantEmergencyStopFailureStat();
    }

    void onDisAddtnlSignChanged(const ArgumentsDisAddtnlSignChanged& args) {
        mDISAddtnlSign = args.mInter_DISAddtnlSign;
        updateConstantFinalAddtnlSignStat();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;
        updateConstantISLANextSupportTrafficSignUnitStat();
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onIndIslaWarningSoundSymbolStatusChanged(const ArgumentsIndIslaWarningSoundSymbolStatusChanged& args) {
        mINDIslaWarningSoundSymbolStatus = args.mInput_INDIslaWarningSoundSymbolStatus;
        updateConstantIslaSoundSymbolStat();
    }

    void onIndForwardSafetySymbolStatusChanged(const ArgumentsIndForwardSafetySymbolStatusChanged& args) {
        mINDForwardSafetySymbolStatus = args.mInput_INDForwardSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_INDForwardSafetySymbolStatus) == true) {
            mINDForwardSafetySymbolStatus = INDForwardSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleFCA();
    }

    void onIndEmergencySteeringSymbolStatusChanged(const ArgumentsIndEmergencySteeringSymbolStatusChanged& args) {
        mINDEmergencySteeringSymbolStatus = args.mInput_INDEmergencySteeringSymbolStatus;
        if (ISTIMEOUT(args.mInput_INDEmergencySteeringSymbolStatus) == true) {
            mINDEmergencySteeringSymbolStatus = INDEmergencySteeringSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleELK();
    }

    void onIndLaneSafetySymbolStatusChanged(const ArgumentsIndLaneSafetySymbolStatusChanged& args) {
        mINDLaneSafetySymbolStatus = args.mInput_INDLaneSafetySymbolStatus;
        if (ISTIMEOUT(args.mInput_INDLaneSafetySymbolStatus) == true) {
            mINDLaneSafetySymbolStatus = INDLaneSafetySymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleLKA();
    }

    void onIndDriveAttentionWarningSymbolStatusChanged(const ArgumentsIndDriveAttentionWarningSymbolStatusChanged& args) {
        mINDDriveAttentionWarningSymbolStatus = args.mInput_INDDriveAttentionWarningSymbolStatus;
        if (ISTIMEOUT(args.mInput_INDDriveAttentionWarningSymbolStatus) == true) {
            mINDDriveAttentionWarningSymbolStatus = INDDriveAttentionWarningSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleDAW();
    }

    void onInterTelltaleIccDrowChanged(const ArgumentsInterTelltaleIccDrowChanged& args) {
        mInterTelltaleIccDrow = args.mInter_TelltaleIccDrow;
        updateTelltaleDAW();
    }

    void onIlcuActWarnStatusChanged(const ArgumentsIlcuActWarnStatusChanged& args) {
        mIlcuLhActWarnStatus = args.mInput_IlcuLhActWarnStatus;
        mIlcuRhActWarnStatus = args.mInput_IlcuRhActWarnStatus;
        updateTelltaleHBA();
    }

    void onIndLfaSymbolStatusChanged(const ArgumentsIndLfaSymbolStatusChanged& args) {
        mINDLfaSymbol1Status = args.mInput_INDLfaSymbol1Status;
        mINDLfaSymbol2Status = args.mInput_INDLfaSymbol2Status;
        updateTelltaleLfaHandsOnOffTelltaleStat();
    }

    void onIndLcaLeftSymbolStatusChanged(const ArgumentsIndLcaLeftSymbolStatusChanged& args) {
        mINDLcaLeftSymbol1Status = args.mInput_INDLcaLeftSymbol1Status;
        mINDLcaLeftSymbol2Status = args.mInput_INDLcaLeftSymbol2Status;
        updateConstantLCALeftTelltaleStat();
    }

    void onIndLcaRightSymbolStatusChanged(const ArgumentsIndLcaRightSymbolStatusChanged& args) {
        mINDLcaRightSymbol1Status = args.mInput_INDLcaRightSymbol1Status;
        mINDLcaRightSymbol2Status = args.mInput_INDLcaRightSymbol2Status;
        updateConstantLCARightTelltaleStat();
    }

    void onIndDriverHandsOnOffStatusChanged(const ArgumentsIndDriverHandsOnOffStatusChanged& args) {
        mINDDriverHandsOnOff1Status = args.mInput_INDDriverHandsOnOff1Status;
        mINDDriverHandsOnOff2Status = args.mInput_INDDriverHandsOnOff2Status;
        updateTelltaleLfaHandsOnOffTelltaleStat();
        updateConstantLCALeftTelltaleStat();
        updateConstantLCARightTelltaleStat();
    }

    void onIndSetSpeedStatusChanged(const ArgumentsIndSetSpeedStatusChanged& args) {
        mINDSetSpeed1Status = args.mInput_INDSetSpeed1Status;
        mINDSetSpeed2Status = args.mInput_INDSetSpeed2Status;
        updateConstantADASSetSpeedColorStat();
    }

    void onIndSetSpeedValueChanged(const ArgumentsIndSetSpeedValueChanged& args) {
        mINDSetSpeed1Value = args.mInput_INDSetSpeed1Value;
        mINDSetSpeed2Value = args.mInput_INDSetSpeed2Value;
        if (ISTIMEOUT(args.mInput_INDSetSpeed1Value) == true) {
            mINDSetSpeed1Value = kTimeOut;
        }
        if (ISTIMEOUT(args.mInput_INDSetSpeed2Value) == true) {
            mINDSetSpeed2Value = kTimeOut;
        }
        updateConstantADASSetSpeedValue();
    }

    void onIndSccSymbolStatusChanged(const ArgumentsIndSccSymbolStatusChanged& args) {
        mINDSccSymbol1Status = args.mInput_INDSccSymbol1Status;
        mINDSccSymbol2Status = args.mInput_INDSccSymbol2Status;
        updateConstantSccSymbolStat();
    }

    void onIndNsccSymbolStatusChanged(const ArgumentsIndNsccSymbolStatusChanged& args) {
        mINDNsccSymbolStatus = args.mInput_INDNsccSymbolStatus;
        updateConstantNsccSymbolStat();
    }

    void onBgstausUppBarChanged(const ArgumentsBgstausUppBarChanged& args) {
        mBGStausUppBar = args.mInput_BGStausUppBar;
        updateConstantBackgroundUpperBarStat();
    }

    void onBgstausLeftBarChanged(const ArgumentsBgstausLeftBarChanged& args) {
        mBGStausLeftBar = args.mInput_BGStausLeftBar;
        updateConstantBackgroundLeftStat();
    }

    void onBgstausRightBarChanged(const ArgumentsBgstausRightBarChanged& args) {
        mBGStausRightBar = args.mInput_BGStausRightBar;
        updateConstantBackgroundRightStat();
    }

    void onNtfGroup7ForwardSafetyFailureStatusChanged(const ArgumentsNtfGroup7ForwardSafetyFailureStatusChanged& args) {
        mNTFGroup7ForwardSafetyFailureStatus = args.mInput_NTFGroup7ForwardSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7ForwardSafetyFailureStatus) == true) {
            mNTFGroup7ForwardSafetyFailureStatus = NTFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantForwardSafetyFailureStat();
    }

    void onNtfGroup7ForwardSideSafetyFailureStatusChanged(const ArgumentsNtfGroup7ForwardSideSafetyFailureStatusChanged& args) {
        mNTFGroup7ForwardSideSafetyFailureStatus = args.mInput_NTFGroup7ForwardSideSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7ForwardSideSafetyFailureStatus) == true) {
            mNTFGroup7ForwardSideSafetyFailureStatus = NTFGroup7ForwardSideSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantForwardSideSafetyFailureStat();
    }

    void onNtfGroup7LaneSafetyFailureStatusChanged(const ArgumentsNtfGroup7LaneSafetyFailureStatusChanged& args) {
        mNTFGroup7LaneSafetyFailureStatus = args.mInput_NTFGroup7LaneSafetyFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7LaneSafetyFailureStatus) == true) {
            mNTFGroup7LaneSafetyFailureStatus = NTFGroup7LaneSafetyFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLaneSafetyFailureStat();
    }

    void onNtfGroup7BlindSpotSafetyFailure2StatusChanged(const ArgumentsNtfGroup7BlindSpotSafetyFailure2StatusChanged& args) {
        mNTFGroup7BlindSpotSafetyFailure2Status = args.mInput_NTFGroup7BlindSpotSafetyFailure2Status;
        if (ISTIMEOUT(args.mInput_NTFGroup7BlindSpotSafetyFailure2Status) == true) {
            mNTFGroup7BlindSpotSafetyFailure2Status = NTFGroup7BlindSpotSafetyFailure2Status::UNHANDLED_TIMEOUT;
        }
        updateConstantBlindSpotSafetyFailureStat();
        updateEventGroup7FullPopupBlindSpotSafetyFailure();
    }

    void onNtfGroup7IslaFailureStatusChanged(const ArgumentsNtfGroup7IslaFailureStatusChanged& args) {
        mNTFGroup7IslaFailureStatus = args.mInput_NTFGroup7IslaFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7IslaFailureStatus) == true) {
            mNTFGroup7IslaFailureStatus = NTFGroup7IslaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantIslaFailureStat();
    }

    void onNtfGroup7DawFailureStatusChanged(const ArgumentsNtfGroup7DawFailureStatusChanged& args) {
        mNTFGroup7DawFailureStatus = args.mInput_NTFGroup7DawFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7DawFailureStatus) == true) {
            mNTFGroup7DawFailureStatus = NTFGroup7DawFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantDawFailureStat();
    }

    void onNtfGroup7HbaFailureStatusChanged(const ArgumentsNtfGroup7HbaFailureStatusChanged& args) {
        mNTFGroup7HbaFailureStatus = args.mInput_NTFGroup7HbaFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7HbaFailureStatus) == true) {
            mNTFGroup7HbaFailureStatus = NTFGroup7HbaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHbaFailureStat();
    }

    void onNtfGroup7SccFailureStatusChanged(const ArgumentsNtfGroup7SccFailureStatusChanged& args) {
        mNTFGroup7SccFailureStatus = args.mInput_NTFGroup7SccFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7SccFailureStatus) == true) {
            mNTFGroup7SccFailureStatus = NTFGroup7SccFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantSccFailureStat();
    }

    void onNtfGroup7LfaFailureStatusChanged(const ArgumentsNtfGroup7LfaFailureStatusChanged& args) {
        mNTFGroup7LfaFailureStatus = args.mInput_NTFGroup7LfaFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7LfaFailureStatus) == true) {
            mNTFGroup7LfaFailureStatus = NTFGroup7LfaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLfaFailureStat();
    }

    void onNtfGroup7HdaFailureStatusChanged(const ArgumentsNtfGroup7HdaFailureStatusChanged& args) {
        mNTFGroup7HdaFailureStatus = args.mInput_NTFGroup7HdaFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7HdaFailureStatus) == true) {
            mNTFGroup7HdaFailureStatus = NTFGroup7HdaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHdaFailureStat();
    }

    void onNtfGroup7HdaPlusFailureStatusChanged(const ArgumentsNtfGroup7HdaPlusFailureStatusChanged& args) {
        mNTFGroup7HdaPlusFailureStatus = args.mInput_NTFGroup7HdaPlusFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7HdaPlusFailureStatus) == true) {
            mNTFGroup7HdaPlusFailureStatus = NTFGroup7HdaPlusFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantHdaPlusFailureStat();
    }

    void onNtfGroup7LcaFailureStatusChanged(const ArgumentsNtfGroup7LcaFailureStatusChanged& args) {
        mNTFGroup7LcaFailureStatus = args.mInput_NTFGroup7LcaFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7LcaFailureStatus) == true) {
            mNTFGroup7LcaFailureStatus = NTFGroup7LcaFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantLcaFailureStat();
    }

    void onNtfGroup7MRMFailureStatusChanged(const ArgumentsNtfGroup7MRMFailureStatusChanged& args) {
        mNTFGroup7MRMFailureStatus = args.mInput_NTFGroup7MRMFailureStatus;
        if (ISTIMEOUT(args.mInput_NTFGroup7MRMFailureStatus) == true) {
            mNTFGroup7MRMFailureStatus = NTFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantEmergencyStopFailureStat();
    }

    void onIfsIndiReqStatusChanged(const ArgumentsIfsIndiReqStatusChanged& args) {
        mIfsIndiReqStatus = args.mInput_IfsIndiReqStatus;
        if (ISTIMEOUT(args.mInput_IfsIndiReqStatus) == true) {
            mIfsIndiReqStatus = IfsIndiReqStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleHBA();
    }

    void onIndHbaSymbolStatusChanged(const ArgumentsIndHbaSymbolStatusChanged& args) {
        mINDHbaSymbolStatus = args.mInput_INDHbaSymbolStatus;
        if (ISTIMEOUT(args.mInput_INDHbaSymbolStatus) == true) {
            mINDHbaSymbolStatus = INDHbaSymbolStatus::UNHANDLED_TIMEOUT;
        }
        updateTelltaleHBA();
    }

    void onVcLeftLineStatusChanged(const ArgumentsVcLeftLineStatusChanged& args) {
        mVCLeftLineStatus = args.mInput_VCLeftLineStatus;
        if (ISTIMEOUT(args.mInput_VCLeftLineStatus) == true) {
            mVCLeftLineStatus = VCLeftLineStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantADASLeftLaneStat();
    }

    void onVcLeftLineOffsetValueChanged(const ArgumentsVcLeftLineOffsetValueChanged& args) {
        mVCLeftLineOffsetValue = args.mInput_VCLeftLineOffsetValue;
        if (ISTIMEOUT(args.mInput_VCLeftLineOffsetValue) == true) {
            mVCLeftLineOffsetValue = kTimeOut;
        }
        updateConstantADASLeftLineLatPosValue();
        updateConstantADASCenterRoadLeftLatPosValue();
    }

    void onVcDrivingLaneRadiusValueChanged(const ArgumentsVcDrivingLaneRadiusValueChanged& args) {
        mVCDrivingLaneRadiusValue = args.mInput_VCDrivingLaneRadiusValue;
        if (ISTIMEOUT(args.mInput_VCDrivingLaneRadiusValue) == true) {
            mVCDrivingLaneRadiusValue = kTimeOut;
        }
        updateConstantADASLaneRoadCvrtLvlStat();
    }

    void onVcRightLineStatusChanged(const ArgumentsVcRightLineStatusChanged& args) {
        mVCRightLineStatus = args.mInput_VCRightLineStatus;
        if (ISTIMEOUT(args.mInput_VCRightLineStatus) == true) {
            mVCRightLineStatus = VCRightLineStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantADASRightLaneStat();
    }

    void onVcRightLineOffsetValueChanged(const ArgumentsVcRightLineOffsetValueChanged& args) {
        mVCRightLineOffsetValue = args.mInput_VCRightLineOffsetValue;
        if (ISTIMEOUT(args.mInput_VCRightLineOffsetValue) == true) {
            mVCRightLineOffsetValue = kTimeOut;
        }
        updateConstantADASRightLineLatPosValue();
        updateConstantADASCenterRoadRightLatPosValue();
    }

    void onVcVehicleDistanceStatusChanged(const ArgumentsVcVehicleDistanceStatusChanged& args) {
        mVCVehicleDistanceStatus = args.mInput_VCVehicleDistanceStatus;
        updateConstantADASHeadwayDistColorStat();
    }

    void onVcVehicleDistanceLevelValueChanged(const ArgumentsVcVehicleDistanceLevelValueChanged& args) {
        mVCVehicleDistanceLevelValue = args.mInput_VCVehicleDistanceLevelValue;
        updateConstantADASVehicleDistanceSetVal();
    }

    void onVcVehicleDistanceValueChanged(const ArgumentsVcVehicleDistanceValueChanged& args) {
        mVCVehicleDistanceValue = args.mInput_VCVehicleDistanceValue;
        updateConstantADASHeadwayDistValue();
    }

    void onVcCenterRoadSurfaceStatusChanged(const ArgumentsVcCenterRoadSurfaceStatusChanged& args) {
        mVCCenterRoadSurfaceStatus = args.mInput_VCCenterRoadSurfaceStatus;
        updateConstantADASCenterRoadStat();
    }

    void onVcSurroundingObject01to08StatusChanged(const ArgumentsVcSurroundingObject01to08StatusChanged& args) {
        // SFCDEV-26: Precodition을 함께 확인하는 사양이기 때문에, 값이 변경되었을 경우에만 update를 수행함
        mVCSurroundingObject01Status = args.mInput_VCSurroundingObject01Status;
        mVCSurroundingObject02Status = args.mInput_VCSurroundingObject02Status;
        mVCSurroundingObject03Status = args.mInput_VCSurroundingObject03Status;
        mVCSurroundingObject04Status = args.mInput_VCSurroundingObject04Status;
        mVCSurroundingObject05Status = args.mInput_VCSurroundingObject05Status;
        mVCSurroundingObject06Status = args.mInput_VCSurroundingObject06Status;
        mVCSurroundingObject07Status = args.mInput_VCSurroundingObject07Status;
        mVCSurroundingObject08Status = args.mInput_VCSurroundingObject08Status;
        if (mPrevVCSurroundingObject01Status != mVCSurroundingObject01Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject01Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor01Stat>();
            mPrevVCSurroundingObject01Status = mVCSurroundingObject01Status;
        }
        if (mPrevVCSurroundingObject02Status != mVCSurroundingObject02Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject02Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor02Stat>();
            mPrevVCSurroundingObject02Status = mVCSurroundingObject02Status;
        }
        if (mPrevVCSurroundingObject03Status != mVCSurroundingObject03Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject03Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor03Stat>();
            mPrevVCSurroundingObject03Status = mVCSurroundingObject03Status;
        }
        if (mPrevVCSurroundingObject04Status != mVCSurroundingObject04Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject04Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor04Stat>();
            mPrevVCSurroundingObject04Status = mVCSurroundingObject04Status;
        }
        if (mPrevVCSurroundingObject05Status != mVCSurroundingObject05Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject05Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor05Stat>();
            mPrevVCSurroundingObject05Status = mVCSurroundingObject05Status;
        }
        if (mPrevVCSurroundingObject06Status != mVCSurroundingObject06Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject06Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor06Stat>();
            mPrevVCSurroundingObject06Status = mVCSurroundingObject06Status;
        }
        if (mPrevVCSurroundingObject07Status != mVCSurroundingObject07Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject07Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor07Stat>();
            mPrevVCSurroundingObject07Status = mVCSurroundingObject07Status;
        }
        if (mPrevVCSurroundingObject08Status != mVCSurroundingObject08Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject08Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor08Stat>();
            mPrevVCSurroundingObject08Status = mVCSurroundingObject08Status;
        }
    }

    void onVcSurroundingObject09to15StatusChanged(const ArgumentsVcSurroundingObject09to15StatusChanged& args) {
        // SFCDEV-26: Precodition을 함께 확인하는 사양이기 때문에, 값이 변경되었을 경우에만 update를 수행함
        mVCSurroundingObject09Status = args.mInput_VCSurroundingObject09Status;
        mVCSurroundingObject10Status = args.mInput_VCSurroundingObject10Status;
        mVCSurroundingObject11Status = args.mInput_VCSurroundingObject11Status;
        mVCSurroundingObject12Status = args.mInput_VCSurroundingObject12Status;
        mVCSurroundingObject13Status = args.mInput_VCSurroundingObject13Status;
        mVCSurroundingObject14Status = args.mInput_VCSurroundingObject14Status;
        mVCSurroundingObject15Status = args.mInput_VCSurroundingObject15Status;
        if (mPrevVCSurroundingObject09Status != mVCSurroundingObject09Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject09Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor09Stat>();
            mPrevVCSurroundingObject09Status = mVCSurroundingObject09Status;
        }
        if (mPrevVCSurroundingObject10Status != mVCSurroundingObject10Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject10Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor10Stat>();
            mPrevVCSurroundingObject10Status = mVCSurroundingObject10Status;
        }
        if (mPrevVCSurroundingObject11Status != mVCSurroundingObject11Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject11Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor11Stat>();
            mPrevVCSurroundingObject11Status = mVCSurroundingObject11Status;
        }
        if (mPrevVCSurroundingObject12Status != mVCSurroundingObject12Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject12Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor12Stat>();
            mPrevVCSurroundingObject12Status = mVCSurroundingObject12Status;
        }
        if (mPrevVCSurroundingObject13Status != mVCSurroundingObject13Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject13Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor13Stat>();
            mPrevVCSurroundingObject13Status = mVCSurroundingObject13Status;
        }
        if (mPrevVCSurroundingObject14Status != mVCSurroundingObject14Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject14Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor14Stat>();
            mPrevVCSurroundingObject14Status = mVCSurroundingObject14Status;
        }
        if (mPrevVCSurroundingObject15Status != mVCSurroundingObject15Status) {
            updateConstantADASViewSurroundingObjectColor<VCSurroundingObject15Status,
                                                         SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor15Stat>();
            mPrevVCSurroundingObject15Status = mVCSurroundingObject15Status;
        }
    }

    void onVcSurroundingObjectClassification01to08StatusChanged(
        const ArgumentsVcSurroundingObjectClassification01to08StatusChanged& args) {
        mVCSurroundingObjectClassification01Status = args.mInput_VCSurroundingObjectClassification01Status;
        mVCSurroundingObjectClassification02Status = args.mInput_VCSurroundingObjectClassification02Status;
        mVCSurroundingObjectClassification03Status = args.mInput_VCSurroundingObjectClassification03Status;
        mVCSurroundingObjectClassification04Status = args.mInput_VCSurroundingObjectClassification04Status;
        mVCSurroundingObjectClassification05Status = args.mInput_VCSurroundingObjectClassification05Status;
        mVCSurroundingObjectClassification06Status = args.mInput_VCSurroundingObjectClassification06Status;
        mVCSurroundingObjectClassification07Status = args.mInput_VCSurroundingObjectClassification07Status;
        mVCSurroundingObjectClassification08Status = args.mInput_VCSurroundingObjectClassification08Status;
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification01Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass01Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification02Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass02Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification03Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass03Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification04Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass04Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification05Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass05Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification06Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass06Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification07Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass07Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification08Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass08Stat>();
    }

    void onVcSurroundingObjectClassification09to15StatusChanged(
        const ArgumentsVcSurroundingObjectClassification09to15StatusChanged& args) {
        mVCSurroundingObjectClassification09Status = args.mInput_VCSurroundingObjectClassification09Status;
        mVCSurroundingObjectClassification10Status = args.mInput_VCSurroundingObjectClassification10Status;
        mVCSurroundingObjectClassification11Status = args.mInput_VCSurroundingObjectClassification11Status;
        mVCSurroundingObjectClassification12Status = args.mInput_VCSurroundingObjectClassification12Status;
        mVCSurroundingObjectClassification13Status = args.mInput_VCSurroundingObjectClassification13Status;
        mVCSurroundingObjectClassification14Status = args.mInput_VCSurroundingObjectClassification14Status;
        mVCSurroundingObjectClassification15Status = args.mInput_VCSurroundingObjectClassification15Status;
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification09Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass09Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification10Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass10Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification11Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass11Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification12Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass12Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification13Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass13Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification14Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass14Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification15Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass15Stat>();
    }

    void onVcSurroundingObjectLateralPosition01to08ValueChanged(
        const ArgumentsVcSurroundingObjectLateralPosition01to08ValueChanged& args) {
        mVCSurroundingObjectLateralPosition01Value = args.mInput_VCSurroundingObjectLateralPosition01Value;
        mVCSurroundingObjectLateralPosition02Value = args.mInput_VCSurroundingObjectLateralPosition02Value;
        mVCSurroundingObjectLateralPosition03Value = args.mInput_VCSurroundingObjectLateralPosition03Value;
        mVCSurroundingObjectLateralPosition04Value = args.mInput_VCSurroundingObjectLateralPosition04Value;
        mVCSurroundingObjectLateralPosition05Value = args.mInput_VCSurroundingObjectLateralPosition05Value;
        mVCSurroundingObjectLateralPosition06Value = args.mInput_VCSurroundingObjectLateralPosition06Value;
        mVCSurroundingObjectLateralPosition07Value = args.mInput_VCSurroundingObjectLateralPosition07Value;
        mVCSurroundingObjectLateralPosition08Value = args.mInput_VCSurroundingObjectLateralPosition08Value;
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat02Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat03Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat04Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat05Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat06Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat07Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat08Stat>();
    }

    void onVcSurroundingObjectLateralPosition09to15ValueChanged(
        const ArgumentsVcSurroundingObjectLateralPosition09to15ValueChanged& args) {
        mVCSurroundingObjectLateralPosition09Value = args.mInput_VCSurroundingObjectLateralPosition09Value;
        mVCSurroundingObjectLateralPosition10Value = args.mInput_VCSurroundingObjectLateralPosition10Value;
        mVCSurroundingObjectLateralPosition11Value = args.mInput_VCSurroundingObjectLateralPosition11Value;
        mVCSurroundingObjectLateralPosition12Value = args.mInput_VCSurroundingObjectLateralPosition12Value;
        mVCSurroundingObjectLateralPosition13Value = args.mInput_VCSurroundingObjectLateralPosition13Value;
        mVCSurroundingObjectLateralPosition14Value = args.mInput_VCSurroundingObjectLateralPosition14Value;
        mVCSurroundingObjectLateralPosition15Value = args.mInput_VCSurroundingObjectLateralPosition15Value;
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat09Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat10Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat11Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat12Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat13Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat14Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Stat>();
    }

    void onVcSurroundingObjectLongitudinalPosition01to08ValueChanged(
        const ArgumentsVcSurroundingObjectLongitudinalPosition01to08ValueChanged& args) {
        mVCSurroundingObjectLongitudinalPosition01Value = args.mInput_VCSurroundingObjectLongitudinalPosition01Value;
        mVCSurroundingObjectLongitudinalPosition02Value = args.mInput_VCSurroundingObjectLongitudinalPosition02Value;
        mVCSurroundingObjectLongitudinalPosition03Value = args.mInput_VCSurroundingObjectLongitudinalPosition03Value;
        mVCSurroundingObjectLongitudinalPosition04Value = args.mInput_VCSurroundingObjectLongitudinalPosition04Value;
        mVCSurroundingObjectLongitudinalPosition05Value = args.mInput_VCSurroundingObjectLongitudinalPosition05Value;
        mVCSurroundingObjectLongitudinalPosition06Value = args.mInput_VCSurroundingObjectLongitudinalPosition06Value;
        mVCSurroundingObjectLongitudinalPosition07Value = args.mInput_VCSurroundingObjectLongitudinalPosition07Value;
        mVCSurroundingObjectLongitudinalPosition08Value = args.mInput_VCSurroundingObjectLongitudinalPosition08Value;
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong02Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong03Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong04Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong05Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong06Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong07Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong08Stat>();
    }

    void onVcSurroundingObjectLongitudinalPosition09to15ValueChanged(
        const ArgumentsVcSurroundingObjectLongitudinalPosition09to15ValueChanged& args) {
        mVCSurroundingObjectLongitudinalPosition09Value = args.mInput_VCSurroundingObjectLongitudinalPosition09Value;
        mVCSurroundingObjectLongitudinalPosition10Value = args.mInput_VCSurroundingObjectLongitudinalPosition10Value;
        mVCSurroundingObjectLongitudinalPosition11Value = args.mInput_VCSurroundingObjectLongitudinalPosition11Value;
        mVCSurroundingObjectLongitudinalPosition12Value = args.mInput_VCSurroundingObjectLongitudinalPosition12Value;
        mVCSurroundingObjectLongitudinalPosition13Value = args.mInput_VCSurroundingObjectLongitudinalPosition13Value;
        mVCSurroundingObjectLongitudinalPosition14Value = args.mInput_VCSurroundingObjectLongitudinalPosition14Value;
        mVCSurroundingObjectLongitudinalPosition15Value = args.mInput_VCSurroundingObjectLongitudinalPosition15Value;
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong09Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong10Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong11Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong12Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong13Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong14Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Stat>();
    }

    void onVcFrontRadarWaveStatusChanged(const ArgumentsVcFrontRadarWaveStatusChanged& args) {
        mVCFrontRadarWaveStatus = args.mInput_VCFrontRadarWaveStatus;
        updateConstantADASViewFrontWarnStat();
    }

    void onVcFrontLeftRadarWaveStatusChanged(const ArgumentsVcFrontLeftRadarWaveStatusChanged& args) {
        mVCFrontLeftRadarWaveStatus = args.mInput_VCFrontLeftRadarWaveStatus;
        updateConstantADASViewFrontLeftWarnStat();
    }

    void onVcFrontRightRadarWaveStatusChanged(const ArgumentsVcFrontRightRadarWaveStatusChanged& args) {
        mVCFrontRightRadarWaveStatus = args.mInput_VCFrontRightRadarWaveStatus;
        updateConstantADASViewFrontRightWarnStat();
    }

    void onVcRearLeftRadarWaveStatusChanged(const ArgumentsVcRearLeftRadarWaveStatusChanged& args) {
        mVCRearLeftRadarWave1Status = args.mInput_VCRearLeftRadarWave1Status;
        mVCRearLeftRadarWave2Status = args.mInput_VCRearLeftRadarWave2Status;
        updateConstantADASViewRearLeftWarnStat();
    }

    void onVcRearRightRadarWaveStatusChanged(const ArgumentsVcRearRightRadarWaveStatusChanged& args) {
        mVCRearRightRadarWave1Status = args.mInput_VCRearRightRadarWave1Status;
        mVCRearRightRadarWave2Status = args.mInput_VCRearRightRadarWave2Status;
        updateConstantADASViewRearRightWarnStat();
    }

    void onVcLaneChangeRectangleStatusChanged(const ArgumentsVcLaneChangeRectangleStatusChanged& args) {
        mVCLaneChangeRectangleStatus = args.mInput_VCLaneChangeRectangleStatus;
        updateConstantADASViewLaneChangeRectangleStat();
    }

    void onVcLaneChangeRectangleLateralPositionValueChanged(
        const ArgumentsVcLaneChangeRectangleLateralPositionValueChanged& args) {
        mVCLaneChangeRectangleLateralPositionValue = args.mInput_VCLaneChangeRectangleLateralPositionValue;
        updateConstantADASViewLaneChangeRectangleLatVal();
        updateConstantADASViewLeftArrowLatValue();
        updateConstantADASViewRightArrowLatValue();
    }

    void onVcLeftLeftLineStatusChanged(const ArgumentsVcLeftLeftLineStatusChanged& args) {
        mVCLeftLeftLineStatus = args.mInput_VCLeftLeftLineStatus;
        updateConstantADASLeftLeftLaneStat();
    }

    void onVcLeftLeftLineOffsetValueChanged(const ArgumentsVcLeftLeftLineOffsetValueChanged& args) {
        mVCLeftLeftLineOffsetValue = args.mInput_VCLeftLeftLineOffsetValue;
        updateConstantADASLeftLeftLineLatPosVal();
    }

    void onVcRightRightLineStatusChanged(const ArgumentsVcRightRightLineStatusChanged& args) {
        mVCRightRightLineStatus = args.mInput_VCRightRightLineStatus;
        updateConstantADASRightRightLaneStat();
    }

    void onVcRightRightLineOffsetValueChanged(const ArgumentsVcRightRightLineOffsetValueChanged& args) {
        mVCRightRightLineOffsetValue = args.mInput_VCRightRightLineOffsetValue;
        updateConstantADASRightRightLineLatPosVal();
    }

    void onVcRearLeftObjectMovingDirection2StatusChanged(const ArgumentsVcRearLeftObjectMovingDirection2StatusChanged& args) {
        mVCRearLeftObjectMovingDirection2Status = args.mInput_VCRearLeftObjectMovingDirection2Status;
        updateConstantADASViewRearLeftCollisionWarningStat();
    }

    void onVcRearRightObjectMovingDirection2StatusChanged(const ArgumentsVcRearRightObjectMovingDirection2StatusChanged& args) {
        mVCRearRightObjectMovingDirection2Status = args.mInput_VCRearRightObjectMovingDirection2Status;
        updateConstantADASViewRearRightCollisionWarningStat();
    }

    void onVcLaneChangeAvailableStatusChanged(const ArgumentsVcLaneChangeAvailableStatusChanged& args) {
        mVCLaneChangeAvailableStatus = args.mInput_VCLaneChangeAvailableStatus;
        updateConstantADASViewLaneChangeAvailableStat();
    }

    void onBrakeLampControlCommand_StatusChanged(const ArgumentsBrakeLampControlCommand_StatusChanged& args) {
        mBrakeLampControlCommand_Status = args.mInput_BrakeLampControlCommand_Status;
        updateConstantADASViewMainBrakeLightActiveStat();
    }

    void onExtLampInputValueChanged(const ArgumentsExtLampInputValueChanged& args) {
        mExtTurnLeftLampStatus = args.mInput_ExtTurnLeftLampStatus;
        mExtTurnRightLampStatus = args.mInput_ExtTurnRightLampStatus;
        mExtLowBeamOnReqStatus = args.mInput_ExtLowBeamOnReqStatus;
        mExtHighBeamOnReqStatus = args.mInput_ExtHighBeamOnReqStatus;
        updateConstantADASViewExteriorLampStat();
    }

    void onVcFrontLeftObjectMovingDirectionStatusChanged(const ArgumentsVcFrontLeftObjectMovingDirectionStatusChanged& args) {
        mVCFrontLeftObjectMovingDirectionStatus = args.mInput_VCFrontLeftObjectMovingDirectionStatus;
        updateConstantADASViewFrontLeftCollisionWarningStat();
    }

    void onVcFrontRightObjectMovingDirectionStatusChanged(const ArgumentsVcFrontRightObjectMovingDirectionStatusChanged& args) {
        mVCFrontRightObjectMovingDirectionStatus = args.mInput_VCFrontRightObjectMovingDirectionStatus;
        updateConstantADASViewFrontRightCollisionWarningStat();
    }

    void onVcEmergencyStreeingStatusChanged(const ArgumentsVcEmergencyStreeingStatusChanged& args) {
        mVCEmergencyStreeingStatus = args.mInput_VCEmergencyStreeingStatus;
        updateConstantADASViewEmergencySteeringStat();
    }

    void onVcAdditionalStatusChanged(const ArgumentsVcAdditionalStatusChanged& args) {
        mVCAdditionalStatus = args.mInput_VCAdditionalStatus;
        updateConstantADASViewAdditionalVehicleStat();
    }

    void onVcLeftLaneChangeDirectionStatusChanged(const ArgumentsVcLeftLaneChangeDirectionStatusChanged& args) {
        mVCLeftLaneChangeDirectionStatus = args.mInput_VCLeftLaneChangeDirectionStatus;
        updateConstantADASViewLeftArrowStat();
    }

    void onVcRightLaneChangeDirectionStatusChanged(const ArgumentsVcRightLaneChangeDirectionStatusChanged& args) {
        mVCRightLaneChangeDirectionStatus = args.mInput_VCRightLaneChangeDirectionStatus;
        updateConstantADASViewRightArrowStat();
    }

    void onIndIslaSpeedLimitTrafficSignStatusChanged(const ArgumentsIndIslaSpeedLimitTrafficSignStatusChanged& args) {
        mINDIslaSpeedLimitTrafficSignStatus = args.mInput_INDIslaSpeedLimitTrafficSignStatus;
        if (ISTIMEOUT(args.mInput_INDIslaSpeedLimitTrafficSignStatus) == true) {
            mINDIslaSpeedLimitTrafficSignStatus = INDIslaSpeedLimitTrafficSignStatus::UNHANDLED_TIMEOUT;
        }
        updateConstantISLAMainSpeedStat();
    }

    void onIndIslaTrafficSignCountryInformationStatusChanged(
        const ArgumentsIndIslaTrafficSignCountryInformationStatusChanged& args) {
        mINDIslaTrafficSignCountryInformationStatus = args.mInput_INDIslaTrafficSignCountryInformationStatus;
        updateConstantISLAMainSpeedStat();
        updatePrivateISLAAddtnlSign();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
    }

    void onIndIslaSpeedLimitTrafficSignValueChanged(const ArgumentsIndIslaSpeedLimitTrafficSignValueChanged& args) {
        mINDIslaSpeedLimitTrafficSignValue = args.mInput_INDIslaSpeedLimitTrafficSignValue;
        updateConstantISLAMainSignStat();
        updateConstantISLAMainSpeedStat();
    }

    void onIndIslaAdditionalTrafficSignStatusChanged(const ArgumentsIndIslaAdditionalTrafficSignStatusChanged& args) {
        mINDIslaAdditionalTrafficSignStatus = args.mInput_INDIslaAdditionalTrafficSignStatus;
        updatePrivateISLAAddtnlSign();
    }

    void onIndIslaSupplementaryTrafficSignStatusChanged(const ArgumentsIndIslaSupplementaryTrafficSignStatusChanged& args) {
        mINDIslaSupplementaryTrafficSignStatus = args.mInput_INDIslaSupplementaryTrafficSignStatus;
        updateConstantISLAAddSignStat();
    }

    void onIndIslaNextSpeedLimitTrafficSignStatusChanged(const ArgumentsIndIslaNextSpeedLimitTrafficSignStatusChanged& args) {
        mINDIslaNextSpeedLimitTrafficSignStatus = args.mInput_INDIslaNextSpeedLimitTrafficSignStatus;
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
    }

    void onIndIslaNextSpeedLimitTrafficSignValueChanged(const ArgumentsIndIslaNextSpeedLimitTrafficSignValueChanged& args) {
        mINDIslaNextSpeedLimitTrafficSignValue = args.mInput_INDIslaNextSpeedLimitTrafficSignValue;
        updateConstantISLANextSpeedLimitTrafficSignVal();
    }

    void onIndIslaNextSupportTrafficSignValueChanged(const ArgumentsIndIslaNextSupportTrafficSignValueChanged& args) {
        mINDIslaNextSupportTrafficSignValue = args.mInput_INDIslaNextSupportTrafficSignValue;
        updateConstantISLANextSupportTrafficSignStat();
    }

    void onIndIslaSupplementaryTrafficSign2StatusChanged(const ArgumentsIndIslaSupplementaryTrafficSign2StatusChanged& args) {
        mINDIslaSupplementaryTrafficSign2Status = args.mInput_INDIslaSupplementaryTrafficSign2Status;
        updateConstantISLAAddSign2Stat();
    }

    void onNtfGroup1AdasWarning1_1StatusChanged(const ArgumentsNtfGroup1AdasWarning1_1StatusChanged& args) {
        mNTFGroup1AdasWarning1_1Status = args.mInput_NTFGroup1AdasWarning1_1Status;
        updateEventGroup1FullPopup1_1();
    }

    void onNtfGroup1AdasWarning1_2StatusChanged(const ArgumentsNtfGroup1AdasWarning1_2StatusChanged& args) {
        mNTFGroup1AdasWarning1_2Status = args.mInput_NTFGroup1AdasWarning1_2Status;
        updateEventGroup1FullPopup1_2();
    }

    void onNtfGroup1AdasWarning2_1StatusChanged(const ArgumentsNtfGroup1AdasWarning2_1StatusChanged& args) {
        mNTFGroup1AdasWarning2_1Status = args.mInput_NTFGroup1AdasWarning2_1Status;
        updateEventGroup1FullPopup2_1();
    }

    void onNtfGroup4AdasWarning1_1StatusChanged(const ArgumentsNtfGroup4AdasWarning1_1StatusChanged& args) {
        mNTFGroup4AdasWarning1_1Status = args.mInput_NTFGroup4AdasWarning1_1Status;
        updateEventGroup4FullPopup1_1();
    }

    void onNtfGroup7OutsideMirrorSymbolFailure2StatusChanged(
        const ArgumentsNtfGroup7OutsideMirrorSymbolFailure2StatusChanged& args) {
        mNTFGroup7OutsideMirrorSymbolFailure2Status = args.mInput_NTFGroup7OutsideMirrorSymbolFailure2Status;
        updateEventGroup7FullPopupOutsideMirrorSymbolFailure();
    }

    void onNtfGroup7DriverAssistFailure1StatusChanged(const ArgumentsNtfGroup7DriverAssistFailure1StatusChanged& args) {
        mNTFGroup7DriverAssistFailure1Status = args.mInput_NTFGroup7DriverAssistFailure1Status;
        if (ISTIMEOUT(args.mInput_NTFGroup7DriverAssistFailure1Status) == true) {
            mNTFGroup7DriverAssistFailure1Status = NTFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopup1();
    }

    void onNtfGroup7DriverAssistFailure2StatusChanged(const ArgumentsNtfGroup7DriverAssistFailure2StatusChanged& args) {
        mNTFGroup7DriverAssistFailure2Status = args.mInput_NTFGroup7DriverAssistFailure2Status;
        if (ISTIMEOUT(args.mInput_NTFGroup7DriverAssistFailure2Status) == true) {
            mNTFGroup7DriverAssistFailure2Status = NTFGroup7DriverAssistFailure2Status::UNHANDLED_TIMEOUT;
        }
        updateEventGroup7FullPopup2();
    }

    void onNtmGroup2AdasWarning1_1StatusChanged(const ArgumentsNtmGroup2AdasWarning1_1StatusChanged& args) {
        mNTMGroup2AdasWarning1_1Status = args.mInput_NTMGroup2AdasWarning1_1Status;
        updateEventGroup2MiniPopup1_1();
    }

    void onNtmGroup2AdasWarning1_2StatusChanged(const ArgumentsNtmGroup2AdasWarning1_2StatusChanged& args) {
        mNTMGroup2AdasWarning1_2Status = args.mInput_NTMGroup2AdasWarning1_2Status;
        updateEventGroup2MiniPopup1_2();
    }

    void onNtfHandsOffStatusChanged(const ArgumentsNtfHandsOffStatusChanged& args) {
        mNTFHandsOffStatus = args.mInput_NTFHandsOffStatus;
        updateEventNTFHandsOff();
    }

    void onSoundWarning1_1StatusChanged(const ArgumentsSoundWarning1_1StatusChanged& args) {
        mSoundWarning1_1Status = args.mInput_SoundWarning1_1Status;
        updateSoundWarning1_1Status();
    }

    void onSoundWarning1_2StatusChanged(const ArgumentsSoundWarning1_2StatusChanged& args) {
        mSoundWarning1_2Status = args.mInput_SoundWarning1_2Status;
        updateSoundWarning1_2Status();
    }

    void onSoundWarning1_4StatusChanged(const ArgumentsSoundWarning1_4StatusChanged& args) {
        mSoundWarning1_4Status = args.mInput_SoundWarning1_4Status;
        updateSoundWarning1_4Status();
    }

    void onSoundWarning1_5StatusChanged(const ArgumentsSoundWarning1_5StatusChanged& args) {
        mSoundWarning1_5Status = args.mInput_SoundWarning1_5Status;
        updateSoundWarning1_5Status();
    }

    void onSoundWarning2_1StatusChanged(const ArgumentsSoundWarning2_1StatusChanged& args) {
        mSoundWarning2_1Status = args.mInput_SoundWarning2_1Status;
        updateSoundWarning2_1Status();
    }

    void onSoundWarning1_3StatusChanged(const ArgumentsSoundWarning1_3StatusChanged& args) {
        mSoundWarning1_3Status = args.mInput_SoundWarning1_3Status;
        mIslaOption2Status = args.mInput_IslaOption2Status;
        mAdasOverSpeedWarnOnOffStatus = args.mInput_AdasOverSpeedWarnOnOffStatus;
        updateSoundWarning1_3Status();
    }

    void onOneShotTimerISLAChanged() {
        mPriorityISLAFlag = false;
        updateConstantFinalAddtnlSignStat();
        flush();
    }

    void onOneShotTimerDISChanged() {
        mPriorityDISFlag = false;
        updateConstantFinalAddtnlSignStat();
        flush();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
#ifdef ENABLE_GCOV_ON
    void updateOneShotTimerPriorityDISStatus(const ssfs::SFCTimerStatus& status) {
        setSFCADAS_DRIVING_2_0TimerOneShotTimerPriorityDISStat(
            static_cast<SFCADAS_DRIVING_2_0TimerOneShotTimerPriorityDISStat>(status));
    }

    void updateOneShotTimerPriorityISLAStatus(const ssfs::SFCTimerStatus& status) {
        setSFCADAS_DRIVING_2_0TimerOneShotTimerPriorityISLAStat(
            static_cast<SFCADAS_DRIVING_2_0TimerOneShotTimerPriorityISLAStat>(status));
    }
#endif

    void updateAll() {
        updateAllConstant();
        updateAllTelltale();
        updateAllEvent();
        updateAllSound();
    }

    void updateAllConstant() {
        updateConstantLCALeftTelltaleStat();
        updateConstantLCARightTelltaleStat();
        updateConstantADASSetSpeedColorStat();
        updateConstantADASSetSpeedValue();
        updateConstantSccSymbolStat();
        updateConstantNsccSymbolStat();
        updateConstantBackgroundUpperBarStat();
        updateConstantBackgroundLeftStat();
        updateConstantBackgroundRightStat();
        updateConstantForwardSafetyFailureStat();
        updateConstantForwardSideSafetyFailureStat();
        updateConstantLaneSafetyFailureStat();
        updateConstantBlindSpotSafetyFailureStat();
        updateConstantIslaFailureStat();
        updateConstantDawFailureStat();
        updateConstantHbaFailureStat();
        updateConstantSccFailureStat();
        updateConstantLfaFailureStat();
        updateConstantHdaFailureStat();
        updateConstantLcaFailureStat();
        updateConstantHdaPlusFailureStat();
        updateConstantEmergencyStopFailureStat();
        updateConstantADASLeftLaneStat();
        updateConstantADASLeftLineLatPosValue();
        updateConstantADASLaneRoadCvrtLvlStat();
        updateConstantADASRightLaneStat();
        updateConstantADASRightLineLatPosValue();
        updateConstantADASHeadwayDistColorStat();
        updateConstantADASVehicleDistanceSetVal();
        updateConstantADASHeadwayDistValue();
        updateConstantADASCenterRoadStat();
        updateConstantADASCenterRoadLeftLatPosValue();
        updateConstantADASCenterRoadRightLatPosValue();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject01Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor01Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject02Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor02Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject03Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor03Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject04Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor04Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject05Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor05Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject06Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor06Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject07Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor07Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject08Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor08Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject09Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor09Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject10Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor10Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject11Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor11Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject12Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor12Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject13Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor13Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject14Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor14Stat>();
        updateConstantADASViewSurroundingObjectColor<VCSurroundingObject15Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor15Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification01Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass01Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification02Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass02Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification03Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass03Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification04Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass04Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification05Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass05Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification06Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass06Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification07Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass07Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification08Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass08Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification09Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass09Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification10Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass10Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification11Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass11Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification12Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass12Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification13Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass13Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification14Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass14Stat>();
        updateConstantADASViewSurroundingObjectClass<VCSurroundingObjectClassification15Status,
                                                     SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass15Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat02Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat03Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat04Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat05Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat06Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat07Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat08Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat09Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat10Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat11Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat12Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat13Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat14Stat>();
        updateConstantADASViewSurroundingObjectLat<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong02Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong03Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong04Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong05Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong06Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong07Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong08Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong09Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong10Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong11Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong12Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong13Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong14Stat>();
        updateConstantADASViewSurroundingObjectLong<SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Stat>();
        updateConstantADASViewFrontWarnStat();
        updateConstantADASViewFrontLeftWarnStat();
        updateConstantADASViewFrontRightWarnStat();
        updateConstantADASViewRearLeftWarnStat();
        updateConstantADASViewRearRightWarnStat();
        updateConstantADASViewLaneChangeRectangleStat();
        updateConstantADASViewLaneChangeRectangleLatVal();
        updateConstantADASViewMainBrakeLightActiveStat();
        updateConstantADASViewExteriorLampStat();
        updateConstantADASViewFrontLeftCollisionWarningStat();
        updateConstantADASViewFrontRightCollisionWarningStat();
        updateConstantADASViewEmergencySteeringStat();
        updateConstantADASViewAdditionalVehicleStat();
        updateConstantADASViewLeftArrowStat();
        updateConstantADASViewLeftArrowLatValue();
        updateConstantADASViewRightArrowStat();
        updateConstantADASViewRightArrowLatValue();
        updateConstantADASLeftLeftLaneStat();
        updateConstantADASLeftLeftLineLatPosVal();
        updateConstantADASRightRightLaneStat();
        updateConstantADASRightRightLineLatPosVal();
        updateConstantADASViewRearLeftCollisionWarningStat();
        updateConstantADASViewRearRightCollisionWarningStat();
        updateConstantADASViewLaneChangeAvailableStat();
        updateConstantISLAMainSpeedStat();
        updateConstantISLAMainSignStat();
        updatePrivateISLAAddtnlSign();  // updateConstantFinalAddtnlSignStat();
        updateConstantISLAAddSignStat();
        updateConstantIslaSoundSymbolStat();
        updateConstantISLANextSpeedLimitTrafficSignTypeStat();
        updateConstantISLANextSpeedLimitTrafficSignVal();
        updateConstantISLANextSupportTrafficSignUnitStat();
        updateConstantISLANextSupportTrafficSignStat();
        updateConstantISLAAddSign2Stat();
    }

    void updateAllTelltale() {
        updateTelltaleFCA();
        updateTelltaleELK();
        updateTelltaleLKA();
        updateTelltaleDAW();
        updateTelltaleHBA();
        updateTelltaleLfaHandsOnOffTelltaleStat();
    }

    void updateAllEvent() {
        updateEventGroup1FullPopup1_1();
        updateEventGroup1FullPopup1_2();
        updateEventGroup1FullPopup2_1();
        updateEventGroup4FullPopup1_1();
        updateEventGroup7FullPopupBlindSpotSafetyFailure();
        updateEventGroup7FullPopupOutsideMirrorSymbolFailure();
        updateEventGroup7FullPopup1();
        updateEventGroup7FullPopup2();
        updateEventGroup2MiniPopup1_1();
        updateEventGroup2MiniPopup1_2();
        updateEventNTFHandsOff();
    }

    void updateAllSound() {
        updateSoundWarning1_2Status();
        updateSoundWarning1_3Status();
        updateSoundWarning1_4Status();
        updateSoundWarning1_5Status();
    }

    // 5.2.5 표시 로직
    // 5.2.5.1 Telltale
    // 5.2.5.1.1 FCA - IMG_CONN_TelltaleFCA_stat
    void updateTelltaleFCA() {
        SFCADAS_DRIVING_2_0TelltaleFCAStat fcaStat = SFCADAS_DRIVING_2_0TelltaleFCAStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mINDForwardSafetySymbolStatus == INDForwardSafetySymbolStatus::AMBER ||
                (mINDForwardSafetySymbolStatus == INDForwardSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON))) {
                fcaStat = SFCADAS_DRIVING_2_0TelltaleFCAStat::AMBER;
            } else if (mINDForwardSafetySymbolStatus == INDForwardSafetySymbolStatus::RED) {
                fcaStat = SFCADAS_DRIVING_2_0TelltaleFCAStat::RED;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms &&
                   (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON)) {
            fcaStat = SFCADAS_DRIVING_2_0TelltaleFCAStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0TelltaleFCAStat(fcaStat);
    }

    // 5.2.5.1.2 ELK - IMG_CONN_TelltaleELK_stat
    void updateTelltaleELK() {
        SFCADAS_DRIVING_2_0TelltaleELKStat elkStat = SFCADAS_DRIVING_2_0TelltaleELKStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mINDEmergencySteeringSymbolStatus == INDEmergencySteeringSymbolStatus::RED) {
                elkStat = SFCADAS_DRIVING_2_0TelltaleELKStat::RED;
            } else if (mINDEmergencySteeringSymbolStatus == INDEmergencySteeringSymbolStatus::AMBER ||
                       (mINDEmergencySteeringSymbolStatus == INDEmergencySteeringSymbolStatus::UNHANDLED_TIMEOUT &&
                        mConfigAdasFCA2 == ConfigAdasFCA2::ON)) {
                elkStat = SFCADAS_DRIVING_2_0TelltaleELKStat::AMBER;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasFCA2 == ConfigAdasFCA2::ON) {
            elkStat = SFCADAS_DRIVING_2_0TelltaleELKStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0TelltaleELKStat(elkStat);
    }

    // 5.2.5.1.3 LKA - IMG_CONN_TelltaleLKA_stat
    void updateTelltaleLKA() {
        SFCADAS_DRIVING_2_0TelltaleLKAStat lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::OFF;
        SFCADAS_DRIVING_2_0TelltaleLKAStatOptional statOptional = SFCADAS_DRIVING_2_0TelltaleLKAStatOptional::OFF;
        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mINDLaneSafetySymbolStatus == INDLaneSafetySymbolStatus::GRAY) {
                lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::GRAY;
            } else if (mINDLaneSafetySymbolStatus == INDLaneSafetySymbolStatus::GREEN) {
                lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::GREEN;
            } else if (mINDLaneSafetySymbolStatus == INDLaneSafetySymbolStatus::GRAY_OFF) {
                lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::OPTIONAL;
                statOptional = SFCADAS_DRIVING_2_0TelltaleLKAStatOptional::GRAY_OFF;
            } else if (mINDLaneSafetySymbolStatus == INDLaneSafetySymbolStatus::AMBER ||
                       (mINDLaneSafetySymbolStatus == INDLaneSafetySymbolStatus::UNHANDLED_TIMEOUT &&
                        mConfigAdasLKA == ConfigAdasLKA::ON)) {
                lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::AMBER;
            } else {
                // OFF
            }
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasLKA == ConfigAdasLKA::ON) {
            lkaStat = SFCADAS_DRIVING_2_0TelltaleLKAStat::AMBER;
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0TelltaleLKAStat(lkaStat);
        setSFCADAS_DRIVING_2_0TelltaleLKAStatOptional(statOptional);
    }

    // 5.2.5.1.4 DAW - IMG_CONN_TelltaleDAW_stat
    void updateTelltaleDAW() {
        SFCADAS_DRIVING_2_0TelltaleDAWStat dawStat = SFCADAS_DRIVING_2_0TelltaleDAWStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigAdasDAW == ConfigAdasDAW::ON) {
            dawStat = SFCADAS_DRIVING_2_0TelltaleDAWStat::AMBER;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mINDDriveAttentionWarningSymbolStatus == INDDriveAttentionWarningSymbolStatus::UNHANDLED_TIMEOUT &&
                mConfigAdasDAW == ConfigAdasDAW::ON) {
                dawStat = SFCADAS_DRIVING_2_0TelltaleDAWStat::AMBER;
            } else if (mINDDriveAttentionWarningSymbolStatus == INDDriveAttentionWarningSymbolStatus::AMBER) {
                dawStat = SFCADAS_DRIVING_2_0TelltaleDAWStat::AMBER;
            } else if (mInterTelltaleIccDrow == InterTelltaleIccDrow::ON) {
                dawStat = SFCADAS_DRIVING_2_0TelltaleDAWStat::AMBER;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0TelltaleDAWStat(dawStat);
    }

    // 5.2.5.1.5 HBA - IMG_CONN_TelltaleHBA_stat
    void updateTelltaleHBA() {
        SFCADAS_DRIVING_2_0TelltaleHBAStat hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigILCU == ConfigILCU::ON && (mIlcuLhActWarnStatus == IlcuLhActWarnStatus::HBA_INDI_ON ||
                                                  mIlcuRhActWarnStatus == IlcuRhActWarnStatus::HBA_INDI_ON)) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::GREEN;
            } else if (mConfigILCU == ConfigILCU::ON && (mIlcuLhActWarnStatus == IlcuLhActWarnStatus::STANDBY ||
                                                         mIlcuRhActWarnStatus == IlcuRhActWarnStatus::STANDBY)) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::WHITE;
            } else if (mConfigIFS == ConfigIFS::ON && mIfsIndiReqStatus == IfsIndiReqStatus::ON) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::GREEN;
            } else if (mConfigIFS == ConfigIFS::ON && mIfsIndiReqStatus == IfsIndiReqStatus::STANDBY) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::WHITE;
            } else if (mConfigNewHBA == ConfigNewHBA::ON && mINDHbaSymbolStatus == INDHbaSymbolStatus::GREEN) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::GREEN;
            } else if (mConfigNewHBA == ConfigNewHBA::ON && mINDHbaSymbolStatus == INDHbaSymbolStatus::GRAY) {
                hbaStat = SFCADAS_DRIVING_2_0TelltaleHBAStat::WHITE;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0TelltaleHBAStat(hbaStat);
    }

    // 5.2.5.1.6 LFA - IMG_CONN_LfaHandsOnOffTelltale_Stat
    void updateTelltaleLfaHandsOnOffTelltaleStat() {
        SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if (mINDLfaSymbol2Status == INDLfaSymbol2Status::OFF) {
                    if (mINDLfaSymbol1Status == INDLfaSymbol1Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GRAY;
                    } else if (mINDLfaSymbol1Status == INDLfaSymbol1Status::GRAY_EXPANSION) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GRAY_EXPENSION;
                    } else if (mINDLfaSymbol1Status == INDLfaSymbol1Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GREEN;
                    } else if (mINDLfaSymbol1Status == INDLfaSymbol1Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_BLUE;
                    } else {
                        // OFF
                    }
                } else if (mINDLfaSymbol1Status == INDLfaSymbol1Status::OFF) {
                    if (mINDLfaSymbol2Status == INDLfaSymbol2Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GRAY;
                    } else if (mINDLfaSymbol2Status == INDLfaSymbol2Status::GRAY_EXPANSION) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GRAY_EXPENSION;
                    } else if (mINDLfaSymbol2Status == INDLfaSymbol2Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_GREEN;
                    } else if (mINDLfaSymbol2Status == INDLfaSymbol2Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::LFA_BLUE;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else if (mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::HANDS_ON_AMBER) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::HANDS_ON_AMBER;
                } else if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::HANDS_ON_RED) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::HANDS_ON_RED;
                } else {
                    // OFF
                }
            } else if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF) {
                if (mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::HANDS_ON_AMBER) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::HANDS_ON_AMBER;
                } else if (mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::HANDS_ON_RED) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::HANDS_ON_RED;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat(stat);
    }

    // 5.2.5.1.7 LCA
    // 5.2.5.1.7.1 차로변경 보조 심벌_왼쪽 - IMG_CONN_LCALeftTelltale_Stat
    void updateConstantLCALeftTelltaleStat() {
        SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if (mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::OFF) {
                    if (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::GRAY;
                    } else if (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::GREEN;
                    } else if (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::BLUE;
                    } else {
                        // OFF
                    }
                } else if (mINDLcaLeftSymbol1Status == INDLcaLeftSymbol1Status::OFF) {
                    if (mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::GRAY;
                    } else if (mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::GREEN;
                    } else if (mINDLcaLeftSymbol2Status == INDLcaLeftSymbol2Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat::BLUE;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
            // Others로 인한 OFF 처리는 사양서의 다음 조건을 포함함
            // 조건1: (Input_INDDriverHandsOnOff1Status==HANDS_ON_AMBER||HANDS_ON_RED) && Input_INDDriverHandsOnOff2Status==OFF
            // 조건2: (Input_INDDriverHandsOnOff2Status==HANDS_ON_AMBER||HANDS_ON_RED) && Input_INDDriverHandsOnOff1Status==OFF
        }
        setSFCADAS_DRIVING_2_0ConstantLCALeftTelltaleStat(stat);
    }

    // 5.2.5.1.7.2 차로변경 보조 심벌_오른쪽 - IMG_CONN_LCARightTelltale_Stat
    void updateConstantLCARightTelltaleStat() {
        SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDDriverHandsOnOff1Status == INDDriverHandsOnOff1Status::OFF &&
                mINDDriverHandsOnOff2Status == INDDriverHandsOnOff2Status::OFF) {
                if (mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::OFF) {
                    if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::GRAY;
                    } else if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::GREEN;
                    } else if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::BLUE;
                    } else {
                        // OFF
                    }
                } else if (mINDLcaRightSymbol1Status == INDLcaRightSymbol1Status::OFF) {
                    if (mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::GRAY) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::GRAY;
                    } else if (mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::GREEN) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::GREEN;
                    } else if (mINDLcaRightSymbol2Status == INDLcaRightSymbol2Status::BLUE) {
                        stat = SFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat::BLUE;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
            // Others로 인한 OFF 처리는 사양서의 다음 조건을 포함함
            // 조건1: (Input_INDDriverHandsOnOff1Status==HANDS_ON_AMBER||HANDS_ON_RED) && Input_INDDriverHandsOnOff2Status==OFF
            // 조건2: (Input_INDDriverHandsOnOff2Status==HANDS_ON_AMBER||HANDS_ON_RED) && Input_INDDriverHandsOnOff1Status==OFF
        }
        setSFCADAS_DRIVING_2_0ConstantLCARightTelltaleStat(stat);
    }

    // 5.2.5.1.8 SCC
    // 5.2.5.1.8.1 설정속도
    // 5.2.5.1.8.1.1 칼라 - IMG_CONN_ADASSetSpeed_Stat
    void updateConstantADASSetSpeedColorStat() {
        SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mINDSetSpeed2Status == INDSetSpeed2Status::OFF) {
                if (mINDSetSpeed1Status == INDSetSpeed1Status::GRAY) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::GRAY;
                } else if (mINDSetSpeed1Status == INDSetSpeed1Status::GREEN) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::GREEN;
                } else if (mINDSetSpeed1Status == INDSetSpeed1Status::BLACK) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::BLACK;
                } else if (mINDSetSpeed1Status == INDSetSpeed1Status::BLUE) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::BLUE;
                } else {
                    // OFF
                }
            } else if (mINDSetSpeed1Status == INDSetSpeed1Status::OFF) {
                if (mINDSetSpeed2Status == INDSetSpeed2Status::GRAY) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::GRAY;
                } else if (mINDSetSpeed2Status == INDSetSpeed2Status::GREEN) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::GREEN;
                } else if (mINDSetSpeed2Status == INDSetSpeed2Status::BLACK) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::BLACK;
                } else if (mINDSetSpeed2Status == INDSetSpeed2Status::BLUE) {
                    stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::BLUE;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat(stat);
    }

    // 5.2.5.1.8.1.2 속도값 - IMG_CONN_ADASSetSpeed_Value
    void updateConstantADASSetSpeedValue() {
        SFCADAS_DRIVING_2_0ConstantADASSetSpeedStat stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedStat::OFF;
        HUInt64 value = 0;
        bool isUpdate = true;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mINDSetSpeed2Value > 254 || mINDSetSpeed2Value == kTimeOut) && mPrevINDSetSpeed1Value != mINDSetSpeed1Value &&
                (0 <= mINDSetSpeed1Value && mINDSetSpeed1Value <= 254)) {
                // ESTRACK20-1501: 사양의 'INVALID`는 0~254 이외의 값(timeout 포함)을 의미함
                stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedStat::VALUE;
                value = mINDSetSpeed1Value;
            } else if ((mINDSetSpeed1Value > 254 || mINDSetSpeed1Value == kTimeOut) &&
                       mPrevINDSetSpeed2Value != mINDSetSpeed2Value && (0 <= mINDSetSpeed2Value && mINDSetSpeed2Value <= 254)) {
                // ESTRACK20-1501: 사양의 'INVALID`는 0~254 이외의 값(timeout 포함)을 의미함
                stat = SFCADAS_DRIVING_2_0ConstantADASSetSpeedStat::VALUE;
                value = mINDSetSpeed2Value;
            } else if (mINDSetSpeed1Value > 254 && mINDSetSpeed2Value > 254) {
                // no operation
                // default OFF
            } else {
                isUpdate = false;
            }
        } else {
            // OFF
        }

        if (isUpdate == true) {
            setSFCADAS_DRIVING_2_0ConstantADASSetSpeedStat(stat);
            setSFCADAS_DRIVING_2_0ConstantADASSetSpeedValue(value);
        }
        mPrevINDSetSpeed1Value = mINDSetSpeed1Value;
        mPrevINDSetSpeed2Value = mINDSetSpeed2Value;
    }

    // 5.2.5.1.8.2 심벌 - IMG_CONN_SccSymbol_Stat
    void updateConstantSccSymbolStat() {
        SFCADAS_DRIVING_2_0ConstantSccSymbolStat stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mINDSccSymbol2Status == INDSccSymbol2Status::OFF) {
                if (mINDSccSymbol1Status == INDSccSymbol1Status::GRAY) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::GRAY;
                } else if (mINDSccSymbol1Status == INDSccSymbol1Status::GREEN) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::GREEN;
                } else if (mINDSccSymbol1Status == INDSccSymbol1Status::BLUE) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::BLUE;
                } else {
                    // OFF
                }
            } else if (mINDSccSymbol1Status == INDSccSymbol1Status::OFF) {
                if (mINDSccSymbol2Status == INDSccSymbol2Status::GRAY) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::GRAY;
                } else if (mINDSccSymbol2Status == INDSccSymbol2Status::GREEN) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::GREEN;
                } else if (mINDSccSymbol2Status == INDSccSymbol2Status::BLUE) {
                    stat = SFCADAS_DRIVING_2_0ConstantSccSymbolStat::BLUE;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantSccSymbolStat(stat);
    }

    // 5.2.5.1.9 NSCC - IMG_CONN_NsccSymbol_Stat
    void updateConstantNsccSymbolStat() {
        SFCADAS_DRIVING_2_0ConstantNsccSymbolStat stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDNsccSymbolStatus == INDNsccSymbolStatus::SAFETY_ZONE) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::SAFETY_ZONE;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::CURVE) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::CURVE;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::IC_AND_JC_LEFT) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::IC_AND_JC_LEFT;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::IC_AND_JC_RIGHT) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::IC_AND_JC_RIGHT;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::TOLLGATE) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::TOLLGATE;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::ROUNDABOUT) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::ROUNDABOUT;
            } else if (mINDNsccSymbolStatus == INDNsccSymbolStatus::SPEED_BUMP) {
                stat = SFCADAS_DRIVING_2_0ConstantNsccSymbolStat::SPEED_BUMP;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantNsccSymbolStat(stat);
    }

    // Constant
    // 5.2.5.2 표시 구성요소 정보 처리 로직
    // 5.2.5.2.4 ADAS 배경 제어
    // 5.2.5.2.4.1 클러스터 상태 표시선_상단 - IMG_CONN_BackgroundUpperBar_Stat
    void updateConstantBackgroundUpperBarStat() {
        SFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat stat = SFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mBGStausUppBar == BGStausUppBar::AMBER) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat::AMBER;
            } else if (mBGStausUppBar == BGStausUppBar::RED) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat::RED;
            } else if (mBGStausUppBar == BGStausUppBar::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat::GREEN;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantBackgroundUpperBarStat(stat);
    }

    // 5.2.5.2.4.2 클러스터 상태 표시선_왼쪽 - IMG_CONN_BackgroundLeft_Stat
    void updateConstantBackgroundLeftStat() {
        SFCADAS_DRIVING_2_0ConstantBackgroundLeftStat stat = SFCADAS_DRIVING_2_0ConstantBackgroundLeftStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mBGStausLeftBar == BGStausLeftBar::AMBER) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundLeftStat::AMBER;
            } else if (mBGStausLeftBar == BGStausLeftBar::RED) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundLeftStat::RED;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantBackgroundLeftStat(stat);
    }

    // 5.2.5.2.4.3 클러스터 상태 표시선_오른쪽 - IMG_CONN_BackgroundRight_Stat
    void updateConstantBackgroundRightStat() {
        SFCADAS_DRIVING_2_0ConstantBackgroundRightStat stat = SFCADAS_DRIVING_2_0ConstantBackgroundRightStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mBGStausRightBar == BGStausRightBar::AMBER) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundRightStat::AMBER;
            } else if (mBGStausRightBar == BGStausRightBar::RED) {
                stat = SFCADAS_DRIVING_2_0ConstantBackgroundRightStat::RED;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantBackgroundRightStat(stat);
    }

    // 5.2.5.3 자율 주행 점검 메시지
    // 5.2.5.3.1 전방 안전 고장 - NT50001,NT50002,NT50003
    void updateConstantForwardSafetyFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7ForwardSafetyFailureStatus == NTFGroup7ForwardSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasFCA == ConfigAdasFCA::ON || mConfigAdasFCA2 == ConfigAdasFCA2::ON)) ||
                mNTFGroup7ForwardSafetyFailureStatus == NTFGroup7ForwardSafetyFailureStatus::FAILURE) {
                ntID = "NT50001";
            } else if (mNTFGroup7ForwardSafetyFailureStatus == NTFGroup7ForwardSafetyFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50002";
            } else if (mNTFGroup7ForwardSafetyFailureStatus == NTFGroup7ForwardSafetyFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50003";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.ForwardSafetyFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventForwardSafetyFailureStat(SFCADAS_DRIVING_2_0EventForwardSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventForwardSafetyFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventForwardSafetyFailureStat(SFCADAS_DRIVING_2_0EventForwardSafetyFailureStat::ON);
        }
    }

    // 5.2.5.3.2 전방/측방 안전 고장 - NT50004,NT50005,NT50006
    void updateConstantForwardSideSafetyFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7ForwardSideSafetyFailureStatus == NTFGroup7ForwardSideSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasFCA2 == ConfigAdasFCA2::ON) ||
                mNTFGroup7ForwardSideSafetyFailureStatus == NTFGroup7ForwardSideSafetyFailureStatus::FAILURE) {
                ntID = "NT50004";
            } else if (mNTFGroup7ForwardSideSafetyFailureStatus == NTFGroup7ForwardSideSafetyFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50005";
            } else if (mNTFGroup7ForwardSideSafetyFailureStatus == NTFGroup7ForwardSideSafetyFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50006";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.ForwardSideSafetyFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventForwardSideSafetyFailureStat(SFCADAS_DRIVING_2_0EventForwardSideSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventForwardSideSafetyFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventForwardSideSafetyFailureStat(SFCADAS_DRIVING_2_0EventForwardSideSafetyFailureStat::ON);
        }
    }

    // 5.2.5.3.3 차로 안전 고장 - NT50007,NT50008
    void updateConstantLaneSafetyFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7LaneSafetyFailureStatus == NTFGroup7LaneSafetyFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasLKA == ConfigAdasLKA::ON) ||
                mNTFGroup7LaneSafetyFailureStatus == NTFGroup7LaneSafetyFailureStatus::FAILURE) {
                ntID = "NT50007";
            } else if (mNTFGroup7LaneSafetyFailureStatus == NTFGroup7LaneSafetyFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50008";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.LaneSafetyFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventLaneSafetyFailureStat(SFCADAS_DRIVING_2_0EventLaneSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventLaneSafetyFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventLaneSafetyFailureStat(SFCADAS_DRIVING_2_0EventLaneSafetyFailureStat::ON);
        }
    }

    // 5.2.5.3.4 후측방 안전 고장 - NT50009,NT50010
    void updateConstantBlindSpotSafetyFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if (mConfigNewBCW == ConfigNewBCW::ON &&
                mNTFGroup7BlindSpotSafetyFailure2Status == NTFGroup7BlindSpotSafetyFailure2Status::UNHANDLED_TIMEOUT) {
                ntID = "NT50009";
            } else if (mNTFGroup7BlindSpotSafetyFailure2Status == NTFGroup7BlindSpotSafetyFailure2Status::FAILURE) {
                ntID = "NT50009";
            } else if (mNTFGroup7BlindSpotSafetyFailure2Status == NTFGroup7BlindSpotSafetyFailure2Status::RADAR_BLOCKAGE) {
                ntID = "NT50010";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.BlindSpotSafetyFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventBlindSpotSafetyFailureStat(SFCADAS_DRIVING_2_0EventBlindSpotSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventBlindSpotSafetyFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventBlindSpotSafetyFailureStat(SFCADAS_DRIVING_2_0EventBlindSpotSafetyFailureStat::ON);
        }
    }

    // 5.2.5.3.5 속도 제한 고장 - NT50011,NT50012
    void updateConstantIslaFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7IslaFailureStatus == NTFGroup7IslaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasISLA == ConfigAdasISLA::ON) ||
                mNTFGroup7IslaFailureStatus == NTFGroup7IslaFailureStatus::FAILURE) {
                ntID = "NT50011";
            } else if (mNTFGroup7IslaFailureStatus == NTFGroup7IslaFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50012";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.IslaFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventIslaFailureStat(SFCADAS_DRIVING_2_0EventIslaFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventIslaFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventIslaFailureStat(SFCADAS_DRIVING_2_0EventIslaFailureStat::ON);
        }
    }

    // 5.2.5.3.6 부주의 운전 경고 고장 - NT50013,NT50014
    void updateConstantDawFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7DawFailureStatus == NTFGroup7DawFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasDAW == ConfigAdasDAW::ON) ||
                mNTFGroup7DawFailureStatus == NTFGroup7DawFailureStatus::FAILURE) {
                ntID = "NT50013";
            } else if (mNTFGroup7DawFailureStatus == NTFGroup7DawFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50014";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.DawFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventDawFailureStat(SFCADAS_DRIVING_2_0EventDawFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventDawFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventDawFailureStat(SFCADAS_DRIVING_2_0EventDawFailureStat::ON);
        }
    }

    // 5.2.5.3.7 하이빔 보조 고장 - NT50015
    void updateConstantHbaFailureStat() {
        SFCADAS_DRIVING_2_0EventHbaFailureStat stat = SFCADAS_DRIVING_2_0EventHbaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if (mNTFGroup7HbaFailureStatus == NTFGroup7HbaFailureStatus::FAILURE ||
                (mNTFGroup7HbaFailureStatus == NTFGroup7HbaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigNewHBA == ConfigNewHBA::ON)) {
                stat = SFCADAS_DRIVING_2_0EventHbaFailureStat::ON;
            }
        }
        setSFCADAS_DRIVING_2_0EventHbaFailureStat(stat);
    }

    // 5.2.5.3.8 스마트 크루즈 컨트롤 고장 - NT50016,NT50017
    void updateConstantSccFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if (mNTFGroup7SccFailureStatus == NTFGroup7SccFailureStatus::FAILURE ||
                (mNTFGroup7SccFailureStatus == NTFGroup7SccFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasSCC == ConfigAdasSCC::ON)) {
                ntID = "NT50016";
            } else if (mNTFGroup7SccFailureStatus == NTFGroup7SccFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50017";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.SccFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventSccFailureStat(SFCADAS_DRIVING_2_0EventSccFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventSccFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventSccFailureStat(SFCADAS_DRIVING_2_0EventSccFailureStat::ON);
        }
    }

    // 5.2.5.3.9 차로 유지 보조 고장 - NT50018
    void updateConstantLfaFailureStat() {
        SFCADAS_DRIVING_2_0EventLfaFailureStat stat = SFCADAS_DRIVING_2_0EventLfaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if (mNTFGroup7LfaFailureStatus == NTFGroup7LfaFailureStatus::FAILURE ||
                (mNTFGroup7LfaFailureStatus == NTFGroup7LfaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasLFA == ConfigAdasLFA::ON)) {
                stat = SFCADAS_DRIVING_2_0EventLfaFailureStat::ON;
            }
        }
        setSFCADAS_DRIVING_2_0EventLfaFailureStat(stat);
    }

    // 5.2.5.3.10 고속도로 주행 보조 고장 - NT50019
    void updateConstantHdaFailureStat() {
        SFCADAS_DRIVING_2_0EventHdaFailureStat stat = SFCADAS_DRIVING_2_0EventHdaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7HdaFailureStatus == NTFGroup7HdaFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA == ConfigAdasHDA::ON || mConfigAdasHDA2 == ConfigAdasHDA2::ON ||
                  mConfigAdasHDA3 == ConfigAdasHDA3::ON)) ||
                mNTFGroup7HdaFailureStatus == NTFGroup7HdaFailureStatus::FAILURE) {
                stat = SFCADAS_DRIVING_2_0EventHdaFailureStat::ON;
            }
        }
        setSFCADAS_DRIVING_2_0EventHdaFailureStat(stat);
    }

    // 5.2.5.3.11 차로 변경 보조 고장 - NT50020,NT50021,NT50022
    void updateConstantLcaFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7LcaFailureStatus == NTFGroup7LcaFailureStatus::UNHANDLED_TIMEOUT &&
                 (mConfigAdasHDA2 == ConfigAdasHDA2::ON || mConfigAdasHDA3 == ConfigAdasHDA3::ON)) ||
                mNTFGroup7LcaFailureStatus == NTFGroup7LcaFailureStatus::FAILURE) {
                ntID = "NT50020";
            } else if (mNTFGroup7LcaFailureStatus == NTFGroup7LcaFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50021";
            } else if (mNTFGroup7LcaFailureStatus == NTFGroup7LcaFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50022";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.LcaFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventLcaFailureStat(SFCADAS_DRIVING_2_0EventLcaFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventLcaFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventLcaFailureStat(SFCADAS_DRIVING_2_0EventLcaFailureStat::ON);
        }
    }

    // 5.2.5.3.12 고속도로 주행 보조 플러스 고장 - NT50023,NT50024,NT50025
    void updateConstantHdaPlusFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if (mNTFGroup7HdaPlusFailureStatus == NTFGroup7HdaPlusFailureStatus::FAILURE ||
                (mNTFGroup7HdaPlusFailureStatus == NTFGroup7HdaPlusFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasHDA3 == ConfigAdasHDA3::ON)) {
                ntID = "NT50023";
            } else if (mNTFGroup7HdaPlusFailureStatus == NTFGroup7HdaPlusFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50024";
            } else if (mNTFGroup7HdaPlusFailureStatus == NTFGroup7HdaPlusFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50025";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.HdaPlusFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventHdaPlusFailureStat(SFCADAS_DRIVING_2_0EventHdaPlusFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventHdaPlusFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventHdaPlusFailureStat(SFCADAS_DRIVING_2_0EventHdaPlusFailureStat::ON);
        }
    }

    // 5.2.5.3.13 비상 정지 기능 고장 - NT50026,NT50027,NT50028
    void updateConstantEmergencyStopFailureStat() {
        std::string prevNTID;
        std::string ntID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7MRMFailureStatus == NTFGroup7MRMFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasEmergencyStop == ConfigAdasEmergencyStop::ON) ||
                mNTFGroup7MRMFailureStatus == NTFGroup7MRMFailureStatus::FAILURE) {
                ntID = "NT50026";
            } else if (mNTFGroup7MRMFailureStatus == NTFGroup7MRMFailureStatus::CAMERA_BLOCKAGE) {
                ntID = "NT50027";
            } else if (mNTFGroup7MRMFailureStatus == NTFGroup7MRMFailureStatus::RADAR_BLOCKAGE) {
                ntID = "NT50028";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.EmergencyStopFailure.ID, prevNTID);
        if (prevNTID.empty() == false && prevNTID != ntID) {
            setSFCADAS_DRIVING_2_0EventEmergencyStopFailureStat(SFCADAS_DRIVING_2_0EventEmergencyStopFailureStat::OFF);
            flushLastGroup();
        }
        if (ntID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventEmergencyStopFailureID(ntID);
            setSFCADAS_DRIVING_2_0EventEmergencyStopFailureStat(SFCADAS_DRIVING_2_0EventEmergencyStopFailureStat::ON);
        }
    }

    // 5.2.5.2.1 주행 보조 뷰_CAN
    // 5.2.5.2.1.1 차선-왼쪽 (1)
    // 5.2.5.2.1.1.1 칼라 - IMG_CONN_ADASLeftLane_Stat
    void updateConstantADASLeftLaneStat() {
        SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLeftLineStatus == VCLeftLineStatus::LIGHT_GRAY || mVCLeftLineStatus == VCLeftLineStatus::UNHANDLED_TIMEOUT) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::LIGHT_GRAY;
            } else if (mVCLeftLineStatus == VCLeftLineStatus::DARK_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::DARK_GRAY;
            } else if (mVCLeftLineStatus == VCLeftLineStatus::AMBER) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::AMBER;
            } else if (mVCLeftLineStatus == VCLeftLineStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::GREEN;
            } else if (mVCLeftLineStatus == VCLeftLineStatus::DARK_GRAY_DASH) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::DARK_GRAY_DASH;
            } else if (mVCLeftLineStatus == VCLeftLineStatus::GREEN_DASH) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLaneStat::GREEN_DASH;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASLeftLaneStat(stat);
    }

    // 5.2.5.2.1.1.2 횡위치 - IMG_CONN_ADASLeftLineLatPos_Val
    void updateConstantADASLeftLineLatPosValue() {
        SFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosStat stat = SFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mVCLeftLineOffsetValue && mVCLeftLineOffsetValue <= 63) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosStat::VALUE;
                value = mVCLeftLineOffsetValue;
            } else if (mVCLeftLineOffsetValue == kTimeOut) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosStat::VALUE;
                value = 15;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASLeftLineLatPosValue(value);
    }

    // 5.2.5.2.1.1.3 차선-왼쪽 (1) 곡률 - IMG_CONN_ADASLaneCvrtLvl_Val
    // 5.2.5.2.1.2.3 차선-오른쪽 (2) 곡률 - IMG_CONN_ADASLaneCvrtLvl_Val
    // 5.2.5.2.1.3.3 차간 거리 (3) 곡률 - IMG_CONN_ADASLaneCvrtLvl_Val
    // 5.2.5.2.1.4.4 노면-중앙 (4) 곡률 - IMG_CONN_ADASRoadCvrtLvl_Val
    // 5.2.5.2.1.23.3 곡률 - IMG_CONN_ADASLaneCvrtLvl_Val
    // 5.2.5.2.1.24.3 곡률 - IMG_CONN_ADASLaneCvrtLvl_Val
    void updateConstantADASLaneRoadCvrtLvlStat() {
        SFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat laneStat = SFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat::OFF;
        SFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat roadStat = SFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mVCDrivingLaneRadiusValue == 0 || mVCDrivingLaneRadiusValue == kTimeOut) {
                laneStat = SFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat::STRAIGHT;
                roadStat = SFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat::STRAIGHT;
            } else if (1 <= mVCDrivingLaneRadiusValue && mVCDrivingLaneRadiusValue <= 15) {
                laneStat = SFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat::LEFT_VALUE;
                roadStat = SFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat::LEFT_VALUE;
                value = mVCDrivingLaneRadiusValue;
            } else if (16 <= mVCDrivingLaneRadiusValue && mVCDrivingLaneRadiusValue <= 30) {
                laneStat = SFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat::RIGHT_VALUE;
                roadStat = SFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat::RIGHT_VALUE;
                value = mVCDrivingLaneRadiusValue - 15;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlStat(laneStat);
        setSFCADAS_DRIVING_2_0ConstantADASLaneCvrtLvlValue(value);
        setSFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlStat(roadStat);
        setSFCADAS_DRIVING_2_0ConstantADASRoadCvrtLvlValue(value);
    }

    // 5.2.5.2.1.2 차선-오른쪽 (2)
    // 5.2.5.2.1.2.1 칼라 - IMG_CONN_ADASRightLane_Stat
    void updateConstantADASRightLaneStat() {
        SFCADAS_DRIVING_2_0ConstantADASRightLaneStat stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCRightLineStatus == VCRightLineStatus::LIGHT_GRAY ||
                mVCRightLineStatus == VCRightLineStatus::UNHANDLED_TIMEOUT) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::LIGHT_GRAY;
            } else if (mVCRightLineStatus == VCRightLineStatus::DARK_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::DARK_GRAY;
            } else if (mVCRightLineStatus == VCRightLineStatus::AMBER) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::AMBER;
            } else if (mVCRightLineStatus == VCRightLineStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::GREEN;
            } else if (mVCRightLineStatus == VCRightLineStatus::DARK_GRAY_DASH) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::DARK_GRAY_DASH;
            } else if (mVCRightLineStatus == VCRightLineStatus::GREEN_DASH) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLaneStat::GREEN_DASH;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASRightLaneStat(stat);
    }

    // 5.2.5.2.1.2.2 횡위치 - IMG_CONN_ADASRightLineLatPos_Val
    void updateConstantADASRightLineLatPosValue() {
        SFCADAS_DRIVING_2_0ConstantADASRightLineLatPosStat stat = SFCADAS_DRIVING_2_0ConstantADASRightLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mVCRightLineOffsetValue && mVCRightLineOffsetValue <= 63) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLineLatPosStat::VALUE;
                value = mVCRightLineOffsetValue;
            } else if (mVCRightLineOffsetValue == kTimeOut) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightLineLatPosStat::VALUE;
                value = 15;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASRightLineLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASRightLineLatPosValue(value);
    }

    // 5.2.5.2.1.3 차간 거리 (3)
    // 5.2.5.2.1.3.1 칼라 - IMG_CONN_ADASHeadwayDist_Stat
    void updateConstantADASHeadwayDistColorStat() {
        SFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCVehicleDistanceStatus == VCVehicleDistanceStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat::GREEN;
            } else if (mVCVehicleDistanceStatus == VCVehicleDistanceStatus::GREEN_DECEL) {
                stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat::GREEN_DECEL;
            } else if (mVCVehicleDistanceStatus == VCVehicleDistanceStatus::GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat::GRAY;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASHeadwayDistColorStat(stat);
    }

    // 5.2.5.2.1.3.4 단계 - IMG_CONN_ADASVehicleDistanceSet_Val
    void updateConstantADASVehicleDistanceSetVal() {
        SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat stat =
            SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCVehicleDistanceLevelValue == VCVehicleDistanceLevelValue::LEVEL1) {
                stat = SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat::LEVEL1;
            } else if (mVCVehicleDistanceLevelValue == VCVehicleDistanceLevelValue::LEVEL2) {
                stat = SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat::LEVEL2;
            } else if (mVCVehicleDistanceLevelValue == VCVehicleDistanceLevelValue::LEVEL3) {
                stat = SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat::LEVEL3;
            } else if (mVCVehicleDistanceLevelValue == VCVehicleDistanceLevelValue::LEVEL4) {
                stat = SFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat::LEVEL4;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASVehicleDistanceSetValStat(stat);
    }

    // 5.2.5.2.1.3.2 종위치 - IMG_CONN_ADASHeadwayDist_Val
    void updateConstantADASHeadwayDistValue() {
        SFCADAS_DRIVING_2_0ConstantADASHeadwayDistStat stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mVCVehicleDistanceValue && mVCVehicleDistanceValue <= 1500) {
                stat = SFCADAS_DRIVING_2_0ConstantADASHeadwayDistStat::VALUE;
                value = mVCVehicleDistanceValue;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASHeadwayDistStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASHeadwayDistValue(value);
    }

    // 5.2.5.2.1.4 노면-중앙 (4)
    // 5.2.5.2.1.4.1 칼라 - IMG_CONN_ADASCenterRoad_Stat
    void updateConstantADASCenterRoadStat() {
        SFCADAS_DRIVING_2_0ConstantADASCenterRoadStat stat = SFCADAS_DRIVING_2_0ConstantADASCenterRoadStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCCenterRoadSurfaceStatus == VCCenterRoadSurfaceStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASCenterRoadStat::GREEN;
            } else if (mVCCenterRoadSurfaceStatus == VCCenterRoadSurfaceStatus::GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASCenterRoadStat::GRAY;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASCenterRoadStat(stat);
    }

    // 5.2.5.2.1.4.2 좌측 횡위치 - IMG_CONN_ADASCenterRoadLeftLatPos_Val
    void updateConstantADASCenterRoadLeftLatPosValue() {
        HUInt64 value = 0;
        SFCADAS_DRIVING_2_0ConstantADASCenterRoadLeftLatPosStat stat =
            SFCADAS_DRIVING_2_0ConstantADASCenterRoadLeftLatPosStat::OFF;
        if (mIsIgnOn == true && (0 <= mVCLeftLineOffsetValue && mVCLeftLineOffsetValue <= 63)) {
            stat = SFCADAS_DRIVING_2_0ConstantADASCenterRoadLeftLatPosStat::VALUE;
            value = mVCLeftLineOffsetValue;
        }
        setSFCADAS_DRIVING_2_0ConstantADASCenterRoadLeftLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASCenterRoadLeftLatPosValue(value);
    }

    // 5.2.5.2.1.4.3 우측 횡위치 - IMG_CONN_ADASCenterRoadRightLatPos_Val
    void updateConstantADASCenterRoadRightLatPosValue() {
        SFCADAS_DRIVING_2_0ConstantADASCenterRoadRightLatPosStat stat =
            SFCADAS_DRIVING_2_0ConstantADASCenterRoadRightLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (0 <= mVCRightLineOffsetValue && mVCRightLineOffsetValue <= 63) {
                stat = SFCADAS_DRIVING_2_0ConstantADASCenterRoadRightLatPosStat::VALUE;
                value = mVCRightLineOffsetValue;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASCenterRoadRightLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASCenterRoadRightLatPosValue(value);
    }

    // 5.2.5.2.1.5 주변 물체 01~15_Extended (5)
    // 5.2.5.2.1.5.1 칼라 - IMG_CONN_ADASViewSurroundingObjectColor[NN]_Stat
    template <typename InputSignalType, typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectColor() {
        if (std::tuple_size<decltype(mSurroundingObjectColorInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of mSurroundingObjectColorInfoTuple";
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
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of surroundingObjectColorInfoElement";
            return;
        }
        InputSignalType vcSurroundingObjectStatus = std::get<kType0>(surroundingObjectColorInfoElement);
        InputSignalType prevVcSurroundingObjectStatus = std::get<kType1>(surroundingObjectColorInfoElement);
        OutputSignalType stat = OutputSignalType::OFF;
        if (mIsIgnOn == true) {
            if (prevVcSurroundingObjectStatus != InputSignalType::NO_DISPLAY &&
                vcSurroundingObjectStatus == InputSignalType::WHITE) {
                stat = OutputSignalType::WHITE;
            } else if (vcSurroundingObjectStatus == InputSignalType::BLACK) {
                stat = OutputSignalType::BLACK;
            } else if (vcSurroundingObjectStatus == InputSignalType::BLACK_UNDER_BAR) {
                stat = OutputSignalType::BLACK_UNDER_BAR;
            } else if (vcSurroundingObjectStatus == InputSignalType::RED) {
                stat = OutputSignalType::RED;
            } else if (prevVcSurroundingObjectStatus == InputSignalType::NO_DISPLAY &&
                       vcSurroundingObjectStatus == InputSignalType::WHITE) {
                stat = OutputSignalType::FADE_IN;
            } else if (prevVcSurroundingObjectStatus == InputSignalType::WHITE &&
                       vcSurroundingObjectStatus == InputSignalType::NO_DISPLAY) {
                stat = OutputSignalType::FADE_OUT;
            } else {
                // OFF
            }
        }
        std::get<kType2>(surroundingObjectColorInfoElement)(stat);
    }

    // 5.2.5.2.1.5.2 타입 - IMG_CONN_ADASViewSurroundingObjectClass[NN]_Stat
    template <typename InputSignalType, typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectClass() {
        if (std::tuple_size<decltype(mSurroundingObjectClassInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of mSurroundingObjectClassInfoTuple";
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
        if (mIsIgnOn == true) {
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
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::EMERGENCY_VEHICLE_COMMERCIAL) {
                stat = OutputSignalType::EMERGENCY_VEHICLE_COMMERCIAL;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::ANIMAL) {
                stat = OutputSignalType::ANIMAL;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::TUBULAR_MARKER) {
                stat = OutputSignalType::TUBULAR_MARKER;
            } else if (vcSurroundingObjectClassificationStatus == InputSignalType::PE_BARREL) {
                stat = OutputSignalType::PE_BARREL;
            } else {
                // OFF
            }
        }
        surroundingObjectClassInfoElement.second(stat);
    }

    // 5.2.5.2.1.5.3 횡위치 - IMG_CONN_ADASViewSurroundingObjectLat[NN]_Val
    template <typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectLat() {
        if (std::tuple_size<decltype(mSurroundingObjectLatInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of mSurroundingObjectLatInfoTuple";
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
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of surroundingObjectLatInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 vcSurroundingObjectLateralPositionValue = std::get<kType0>(surroundingObjectLatInfoElement).get();
        if (mIsIgnOn == true) {
            if (vcSurroundingObjectLateralPositionValue >= 0 && vcSurroundingObjectLateralPositionValue <= 300) {
                stat = OutputSignalType::LEFT;
                value = static_cast<HInt64>(vcSurroundingObjectLateralPositionValue);
            } else if (vcSurroundingObjectLateralPositionValue >= 1024 && vcSurroundingObjectLateralPositionValue <= 1323) {
                stat = OutputSignalType::RIGHT;
                value = 1023 - static_cast<HInt64>(vcSurroundingObjectLateralPositionValue);  // 1024~1323 : -1~-300
            } else {
                // OFF
            }
        }
        std::get<kType1>(surroundingObjectLatInfoElement)(stat);
        std::get<kType2>(surroundingObjectLatInfoElement)(value);
    }

    // 5.2.5.2.1.5.4 종위치 - IMG_CONN_ADASViewSurroundingObjectLong[NN]_Val
    template <typename OutputSignalType>
    void updateConstantADASViewSurroundingObjectLong() {
        if (std::tuple_size<decltype(mSurroundingObjectLongInfoTuple)>::value != kSurroundingObjectSize) {
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of mSurroundingObjectLongInfoTuple";
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
            DWarning() << "[ADAS_DRIVING_2_0] Invalied tuple size of surroundingObjectLongInfoElement";
            return;
        }
        OutputSignalType stat = OutputSignalType::OFF;
        HInt64 value = 0;
        HUInt64 vcSurroundingObjectLongitudinalPositionValue = std::get<kType0>(surroundingObjectLongInfoElement).get();
        if (mIsIgnOn == true) {
            if (vcSurroundingObjectLongitudinalPositionValue >= 0 && vcSurroundingObjectLongitudinalPositionValue <= 1500) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(vcSurroundingObjectLongitudinalPositionValue);
            } else if (vcSurroundingObjectLongitudinalPositionValue >= 1846 &&
                       vcSurroundingObjectLongitudinalPositionValue <= 2045) {
                stat = OutputSignalType::VALUE;
                value = static_cast<HInt64>(vcSurroundingObjectLongitudinalPositionValue) - 2046;
            } else {
                // OFF
            }
        }
        std::get<kType1>(surroundingObjectLongInfoElement)(stat);
        std::get<kType2>(surroundingObjectLongInfoElement)(value);
    }

    // 5.2.5.2.1.6 레이더 파형-전방 (6) - IMG_CONN_ADASViewFrontWarn_Stat
    void updateConstantADASViewFrontWarnStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewFrontWarnStat stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCFrontRadarWaveStatus == VCFrontRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontWarnStat::RED_WAVE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewFrontWarnStat(stat);
    }

    // 5.2.5.2.1.7 레이더 파형-전방 왼쪽 (7) - IMG_CONN_ADASViewFrontLeftWarn_Stat
    void updateConstantADASViewFrontLeftWarnStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftWarnStat stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCFrontLeftRadarWaveStatus == VCFrontLeftRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftWarnStat::RED_WAVE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewFrontLeftWarnStat(stat);
    }

    // 5.2.5.2.1.8 레이더 파형-전방 오른쪽 (8) - IMG_CONN_ADASViewFrontRightWarn_Stat
    void updateConstantADASViewFrontRightWarnStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewFrontRightWarnStat stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontRightWarnStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCFrontRightRadarWaveStatus == VCFrontRightRadarWaveStatus::RED_WAVE) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontRightWarnStat::RED_WAVE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewFrontRightWarnStat(stat);
    }

    // 5.2.5.2.1.9 레이더 파형-후방 왼쪽 (9) - IMG_CONN_ADASViewRearLeftWarn_Stat
    void updateConstantADASViewRearLeftWarnStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewRearLeftWarnStat stat = SFCADAS_DRIVING_2_0ConstantADASViewRearLeftWarnStat::OFF;
        if (mIsIgnOn == true) {
            if ((mVCRearLeftRadarWave2Status == VCRearLeftRadarWave2Status::OFF &&
                 mVCRearLeftRadarWave1Status == VCRearLeftRadarWave1Status::RED_WAVE) ||
                (mVCRearLeftRadarWave1Status == VCRearLeftRadarWave1Status::OFF &&
                 mVCRearLeftRadarWave2Status == VCRearLeftRadarWave2Status::RED_WAVE)) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearLeftWarnStat::RED_WAVE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRearLeftWarnStat(stat);
    }

    // 5.2.5.2.1.10 레이더 파형-후방 오른쪽 (10) - IMG_CONN_ADASViewRearRightWarn_Stat
    void updateConstantADASViewRearRightWarnStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewRearRightWarnStat stat = SFCADAS_DRIVING_2_0ConstantADASViewRearRightWarnStat::OFF;
        if (mIsIgnOn == true) {
            if ((mVCRearRightRadarWave2Status == VCRearRightRadarWave2Status::OFF &&
                 mVCRearRightRadarWave1Status == VCRearRightRadarWave1Status::RED_WAVE) ||
                (mVCRearRightRadarWave1Status == VCRearRightRadarWave1Status::OFF &&
                 mVCRearRightRadarWave2Status == VCRearRightRadarWave2Status::RED_WAVE)) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearRightWarnStat::RED_WAVE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRearRightWarnStat(stat);
    }

    // 5.2.5.2.1.11 차로 변경 방향 제안 (11)
    // 5.2.5.2.1.11.1 타입 - IMG_CONN_ADASViewLaneChangeRectangle_Stat
    void updateConstantADASViewLaneChangeRectangleStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLaneChangeRectangleStatus == VCLaneChangeRectangleStatus::DOTTED_LINE_SUGGESTION) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleStat::DOTTED_LINE_SUGGESTION;
            } else if (mVCLaneChangeRectangleStatus == VCLaneChangeRectangleStatus::DOTTED_LINE_ASSIST) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleStat::DOTTED_LINE_ASSIST;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleStat(stat);
    }

    // 5.2.5.2.1.11.2 횡위치 - IMG_CONN_ADASViewLaneChangeRectangleLat_Val
    void updateConstantADASViewLaneChangeRectangleLatVal() {
        HUInt64 value = 0;
        SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat::CENTER;
                value = mVCLaneChangeRectangleLateralPositionValue;
            } else if (mVCLaneChangeRectangleLateralPositionValue >= 1 && mVCLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat::RIGHT_VALUE;
                value = mVCLaneChangeRectangleLateralPositionValue;
            } else if (mVCLaneChangeRectangleLateralPositionValue >= 0x42 && mVCLaneChangeRectangleLateralPositionValue <= 0x7F) {
                // 0x7f -> -1, 0x42 -> -62
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat::LEFT_VALUE;
                value = (mVCLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASViewLaneChangeRectangleLatValue(value);
    }

    // EXNCP-44758: 사양서의 CAN 항목과 ETHERNET 항목에 중복으로 작성됨
    // 5.2.5.2.1.12 자차 제동등 (12) - IMG_CONN_ADASViewMainBrakeLightActive_Stat
    // 5.2.5.2.2.11 자차 제동등 (14)
    void updateConstantADASViewMainBrakeLightActiveStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewMainBrakeLightActiveStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewMainBrakeLightActiveStat::OFF;

        if (mIsIgnOn == true && (mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::STEADY_ON ||
                                 mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::BLINK_ON)) {
            stat = SFCADAS_DRIVING_2_0ConstantADASViewMainBrakeLightActiveStat::ON;
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewMainBrakeLightActiveStat(stat);
    }

    // EXNCP-44758: 사양서의 CAN 항목과 ETHERNET 항목에 중복으로 작성됨
    // 5.2.5.2.1.13 자차 방향 지시등-왼쪽 (13) - IMG_CONN_ADASViewMainExteriorTurnLeftLampActive_Stat
    // 5.2.5.2.2.19 자차 방향 지시등-왼쪽 (24)
    // 5.2.5.2.1.14 자차 방향 지시등-오른쪽 (14) - IMG_CONN_ADASViewMainExteriorTurnRightLampActive_Stat
    // 5.2.5.2.2.20 자차 방향 지시등-오른쪽 (25)
    // 5.2.5.2.1.15 자차 전조등 (15) - IMG_CONN_ADASViewMainExteriorLowBeamActive_Stat
    // 5.2.5.2.2.12 자차 전조등 (15)
    // 5.2.5.2.1.16 자차 상향등 (16) - IMG_CONN_ADASViewMainExteriorHighBeamActive_Stat
    // 5.2.5.2.2.13 자차 상향등 (16)
    void updateConstantADASViewExteriorLampStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat extTurnLeftStat =
            SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat::OFF;
        SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat extTurnRightStat =
            SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat::OFF;
        SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorLowBeamActiveStat extLowBeamStat =
            SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorLowBeamActiveStat::OFF;
        SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorHighBeamActiveStat extHighBeamStat =
            SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorHighBeamActiveStat::OFF;
        if (mIsIgnOn == true) {
            if (mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::ON) {
                extTurnLeftStat = SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat::ON;
            }
            if (mExtTurnRightLampStatus == ExtTurnRightLampStatus::ON) {
                extTurnRightStat = SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat::ON;
            }
            if (mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::ON) {
                extLowBeamStat = SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorLowBeamActiveStat::ON;
            }
            if (mExtHighBeamOnReqStatus == ExtHighBeamOnReqStatus::ON) {
                extHighBeamStat = SFCADAS_DRIVING_2_0ConstantADASViewMainExteriorHighBeamActiveStat::ON;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnLeftLampActiveStat(extTurnLeftStat);
        setSFCADAS_DRIVING_2_0ConstantADASViewMainExteriorTurnRightLampActiveStat(extTurnRightStat);
        setSFCADAS_DRIVING_2_0ConstantADASViewMainExteriorLowBeamActiveStat(extLowBeamStat);
        setSFCADAS_DRIVING_2_0ConstantADASViewMainExteriorHighBeamActiveStat(extHighBeamStat);
    }

    // 5.2.5.2.1.17 충돌 경고-전방 왼쪽 (17) - IMG_CONN_ADASViewFrontLeftCollisionWarning_Stat
    void updateConstantADASViewFrontLeftCollisionWarningStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftCollisionWarningStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftCollisionWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCFrontLeftObjectMovingDirectionStatus == VCFrontLeftObjectMovingDirectionStatus::RED_ARROW) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftCollisionWarningStat::RED_ARROW;
            } else if (mVCFrontLeftObjectMovingDirectionStatus == VCFrontLeftObjectMovingDirectionStatus::RED_ARROW_ANIMATION) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontLeftCollisionWarningStat::RED_ARROW_ANIMATION;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewFrontLeftCollisionWarningStat(stat);
    }

    // 5.2.5.2.1.18 충돌 경고-전방 오른쪽 (18) - IMG_CONN_ADASViewFrontRightCollisionWarning_Stat
    void updateConstantADASViewFrontRightCollisionWarningStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewFrontRightCollisionWarningStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewFrontRightCollisionWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCFrontRightObjectMovingDirectionStatus == VCFrontRightObjectMovingDirectionStatus::RED_ARROW) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontRightCollisionWarningStat::RED_ARROW;
            } else if (mVCFrontRightObjectMovingDirectionStatus == VCFrontRightObjectMovingDirectionStatus::RED_ARROW_ANIMATION) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewFrontRightCollisionWarningStat::RED_ARROW_ANIMATION;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewFrontRightCollisionWarningStat(stat);
    }

    // 5.2.5.2.1.19 긴급조향 (19) - IMG_CONN_ADASViewEmergencySteering_Stat
    void updateConstantADASViewEmergencySteeringStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewEmergencySteeringStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewEmergencySteeringStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::LEFT_DISPLAY ||
                mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::LEFT_DISPLAY_WITH_ESA) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewEmergencySteeringStat::LEFT;
            } else if (mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::RIGHT_DISPLAY ||
                       mVCEmergencyStreeingStatus == VCEmergencyStreeingStatus::RIGHT_DISPLAY_WITH_ESA) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewEmergencySteeringStat::RIGHT;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewEmergencySteeringStat(stat);
    }

    // 5.2.5.2.1.20 전방 물체 추가 상태 (20) - IMG_CONN_ADASViewAdditionalVehicle_Stat
    void updateConstantADASViewAdditionalVehicleStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewAdditionalVehicleStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewAdditionalVehicleStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCAdditionalStatus == VCAdditionalStatus::LEAD_VEHICLE_DEPARTURE_DISPLAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewAdditionalVehicleStat::LEAD_VEHICLE_DEPARTURE;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewAdditionalVehicleStat(stat);
    }

    // 5.2.5.2.1.21 차로 변경 방향-왼쪽 (21)
    // 5.2.5.2.1.21.1 타입 - IMG_CONN_ADASViewLeftArrow_Stat
    void updateConstantADASViewLeftArrowStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowStat stat = SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLeftLaneChangeDirectionStatus == VCLeftLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowStat::GREEN_ARROW_ANIMATION;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewLeftArrowStat(stat);
    }

    // 5.2.5.2.1.21.2 횡위치 - IMG_CONN_ADASViewLeftArrowLat_Val
    void updateConstantADASViewLeftArrowLatValue() {
        SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatStat stat = SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatStat::CENTER;
            } else if (mVCLaneChangeRectangleLateralPositionValue >= 0x42 && mVCLaneChangeRectangleLateralPositionValue <= 0x7F) {
                // 0x7f -> -1, 0x42 -> -62
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatStat::LEFT_VALUE;
                value = (mVCLaneChangeRectangleLateralPositionValue ^ 0x7f) + 1;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASViewLeftArrowLatValue(value);
    }

    // 5.2.5.2.1.22 차로 변경 방향-오른쪽 (22)
    // 5.2.5.2.1.22.1 타입 - IMG_CONN_ADASViewRightArrow_Stat
    void updateConstantADASViewRightArrowStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewRightArrowStat stat = SFCADAS_DRIVING_2_0ConstantADASViewRightArrowStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCRightLaneChangeDirectionStatus == VCRightLaneChangeDirectionStatus::GREEN_ARROW_ANIMATION) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRightArrowStat::GREEN_ARROW_ANIMATION;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRightArrowStat(stat);
    }

    // 5.2.5.2.1.22.2 횡위치 - IMG_CONN_ADASViewRightArrowLat_Val
    void updateConstantADASViewRightArrowLatValue() {
        SFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatStat stat = SFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mVCLaneChangeRectangleLateralPositionValue == 0) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatStat::CENTER;
            } else if (mVCLaneChangeRectangleLateralPositionValue >= 1 && mVCLaneChangeRectangleLateralPositionValue <= 62) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatStat::RIGHT_VALUE;
                value = mVCLaneChangeRectangleLateralPositionValue;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASViewRightArrowLatValue(value);
    }

    // 5.2.5.2.1.23 차선-왼쪽의 왼쪽 (23)
    // 5.2.5.2.1.23.1 칼라 - IMG_CONN_ADASLeftLeftLane_Stat
    void updateConstantADASLeftLeftLaneStat() {
        SFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::LIGHT_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat::LIGHT_GRAY;
            } else if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::DARK_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat::DARK_GRAY;
            } else if (mVCLeftLeftLineStatus == VCLeftLeftLineStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat::GREEN;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASLeftLeftLaneStat(stat);
    }

    // 5.2.5.2.1.23.2 횡위치 - IMG_CONN_ADASLeftLeftLineLatPos_Val
    void updateConstantADASLeftLeftLineLatPosVal() {
        SFCADAS_DRIVING_2_0ConstantADASLeftLeftLineLatPosStat stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mVCLeftLeftLineOffsetValue >= 0 && mVCLeftLeftLineOffsetValue <= 63) {
                stat = SFCADAS_DRIVING_2_0ConstantADASLeftLeftLineLatPosStat::VALUE;
                value = mVCLeftLeftLineOffsetValue;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASLeftLeftLineLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASLeftLeftLineLatPosValue(value);
    }

    // 5.2.5.2.1.24 차선-오른쪽의 오른쪽 (24)
    // 5.2.5.2.1.24.1 칼라 - IMG_CONN_ADASRightRightLane_Stat
    void updateConstantADASRightRightLaneStat() {
        SFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat stat = SFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCRightRightLineStatus == VCRightRightLineStatus::LIGHT_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat::LIGHT_GRAY;
            } else if (mVCRightRightLineStatus == VCRightRightLineStatus::DARK_GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat::DARK_GRAY;
            } else if (mVCRightRightLineStatus == VCRightRightLineStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat::GREEN;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASRightRightLaneStat(stat);
    }

    // 5.2.5.2.1.24.2 횡위치 - IMG_CONN_ADASRightRightLineLatPos_Val
    void updateConstantADASRightRightLineLatPosVal() {
        SFCADAS_DRIVING_2_0ConstantADASRightRightLineLatPosStat stat =
            SFCADAS_DRIVING_2_0ConstantADASRightRightLineLatPosStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mVCRightRightLineOffsetValue >= 0 && mVCRightRightLineOffsetValue <= 63) {
                stat = SFCADAS_DRIVING_2_0ConstantADASRightRightLineLatPosStat::VALUE;
                value = mVCRightRightLineOffsetValue;
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASRightRightLineLatPosStat(stat);
        setSFCADAS_DRIVING_2_0ConstantADASRightRightLineLatPosValue(value);
    }

    // 5.2.5.2.1.25 충돌 경고-후방 왼쪽 (25) - IMG_CONN_ADASViewRearLeftCollisionWarning_Stat
    void updateConstantADASViewRearLeftCollisionWarningStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCRearLeftObjectMovingDirection2Status == VCRearLeftObjectMovingDirection2Status::WARNING_CROSSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat::WARNING_CROSSING;
            } else if (mVCRearLeftObjectMovingDirection2Status == VCRearLeftObjectMovingDirection2Status::FAILURE_CROSSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat::FAILURE_CROSSING;
            } else if (mVCRearLeftObjectMovingDirection2Status == VCRearLeftObjectMovingDirection2Status::WARING_PASSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat::WARING_PASSING;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRearLeftCollisionWarningStat(stat);
    }

    // 5.2.5.2.1.26 충돌 경고-후방 오른쪽 (26) - IMG_CONN_ADASViewRearRightCollisionWarning_Stat
    void updateConstantADASViewRearRightCollisionWarningStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCRearRightObjectMovingDirection2Status == VCRearRightObjectMovingDirection2Status::WARNING_CROSSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat::WARNING_CROSSING;
            } else if (mVCRearRightObjectMovingDirection2Status == VCRearRightObjectMovingDirection2Status::FAILURE_CROSSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat::FAILURE_CROSSING;
            } else if (mVCRearRightObjectMovingDirection2Status == VCRearRightObjectMovingDirection2Status::WARING_PASSING) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat::WARING_PASSING;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewRearRightCollisionWarningStat(stat);
    }

    // 5.2.5.2.1.27 차로 변경 가능 상태 (27) - IMG_CONN_ADASViewLaneChangeAvailable_Stat
    void updateConstantADASViewLaneChangeAvailableStat() {
        SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeAvailableStat stat =
            SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeAvailableStat::OFF;
        if (mIsIgnOn == true) {
            if (mVCLaneChangeAvailableStatus == VCLaneChangeAvailableStatus::GRAY) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeAvailableStat::GRAY;
            } else if (mVCLaneChangeAvailableStatus == VCLaneChangeAvailableStatus::GREEN) {
                stat = SFCADAS_DRIVING_2_0ConstantADASViewLaneChangeAvailableStat::GREEN;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantADASViewLaneChangeAvailableStat(stat);
    }

    // 5.2.5.2.3 교통 표지판
    // 5.2.5.2.3.1 메인 차속 표지판 (①)
    // 5.2.5.2.3.1.1 형상 - IMG_CONN_ISLAMainSpeed_Stat
    void updateConstantISLAMainSpeedStat() {
        SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::OFF;
        if (mConfigAdasISLA == ConfigAdasISLA::ON) {
            if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                if (mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::BLACK_TEXT) {
                    if (mINDIslaSpeedLimitTrafficSignValue == 253) {
                        stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::UNLIMITED;
                    } else if (mINDIslaTrafficSignCountryInformationStatus ==
                                   INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                               mINDIslaTrafficSignCountryInformationStatus ==
                                   INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                               mINDIslaTrafficSignCountryInformationStatus ==
                                   INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                               mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                               mINDIslaTrafficSignCountryInformationStatus ==
                                   INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                        if (mINDIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE3_INVALID;
                        } else if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE3_VALID;
                        } else {
                            // OFF
                        }
                    } else if (mINDIslaTrafficSignCountryInformationStatus ==
                               INDIslaTrafficSignCountryInformationStatus::CANADA) {
                        if (mINDIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE4_INVALID;
                        } else if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE4_VALID;
                        } else {
                            // OFF
                        }
                    } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                        if (mINDIslaSpeedLimitTrafficSignValue == 0) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE5_INVALID;
                        } else if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE5_VALID;
                        } else {
                            // OFF
                        }
                    } else {
                        // OFF
                    }
                } else if (mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::RED_TEXT) {
                    if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                        if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE6_VALID;
                        }
                    } else if (mINDIslaTrafficSignCountryInformationStatus ==
                               INDIslaTrafficSignCountryInformationStatus::CANADA) {
                        if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE7_VALID;
                        }
                    } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                        if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                            stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE8_VALID;
                        }
                    } else {
                        // OFF
                    }
                } else if (mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::AMBER ||
                           mINDIslaSpeedLimitTrafficSignStatus == INDIslaSpeedLimitTrafficSignStatus::UNHANDLED_TIMEOUT) {
                    if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                        stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE2_INVALID;
                    } else if (!(mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA)) {
                        stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE1_INVALID;
                    } else {
                        // OFF
                    }
                } else {
                    // OFF
                }
            } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
                if (mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA) {
                    stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE2_INVALID;
                } else if (!(mConfigArea == ConfigArea::CAN || mConfigArea == ConfigArea::USA)) {
                    stat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::TYPE1_INVALID;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat(stat);
        mISLAMainSpeedStat = stat;
    }

    // 5.2.5.2.3.1.2 제한 속도 - IMG_CONN_ISLAMainSign_Val
    void updateConstantISLAMainSignStat() {
        SFCADAS_DRIVING_2_0ConstantISLAMainSignStat stat = SFCADAS_DRIVING_2_0ConstantISLAMainSignStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (1 <= mINDIslaSpeedLimitTrafficSignValue && mINDIslaSpeedLimitTrafficSignValue <= 252) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAMainSignStat::VALUE;
                value = mINDIslaSpeedLimitTrafficSignValue;
            } else if (mINDIslaSpeedLimitTrafficSignValue == 0) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAMainSignStat::INVALID;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLAMainSignStat(stat);
        setSFCADAS_DRIVING_2_0ConstantISLAMainSignValue(value);
    }

    // 5.2.5.2.3.2 부가 표지판 (②)
    // 5.2.5.2.3.2.1 형상 - Private_ISLAAddtnlSign
    void updatePrivateISLAAddtnlSign() {
        mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
        if (mIsIgnOn == true) {
            if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::SCHOOL_CROSSING) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_AUS;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::SCHOOL_CROSS_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::STOP) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::STOP_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::YIELD) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_AUS;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::YIELD_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::DO_NOT_PASS_LHD) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_LHD_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::DO_NOT_PASS_RHD) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_PASS_RHD_EU;
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::DO_NOT_ENTER) {
                mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::DO_NOT_ENTER;
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::ROUNDABOUT_LHD) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_LHD_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::ROUNDABOUT_RHD) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                    mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::ROUNDABOUT_RHD_AUS;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::RIGHT_CURVE) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_CURVE_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::LEFT_CURVE) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_CURVE_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::RIGHT_LEFT_CURVES) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::RIGHT_LEFT_CURVES_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::LEFT_RIGHT_CURVES) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::LEFT_RIGHT_CURVES_CN;
                } else {
                    // OFF
                }
            } else if (mINDIslaAdditionalTrafficSignStatus == INDIslaAdditionalTrafficSignStatus::PEDESTRIAN_CROSSING) {
                if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_DOM;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_EU;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_AUS;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA ||
                           mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_NA;
                } else if (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA) {
                    mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::PEDESTRIAN_CROSSING_CN;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        }
        updateConstantFinalAddtnlSignStat();
    }

    // 5.2.5.2.3.2.2 ISLA/DIS 우선 순위 판단 로직 - IMG_CONN_FinalAddtnlSign_Stat
    void updateConstantFinalAddtnlSignStat() {
        static Private_ISLAAddtnlSign prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
        static DISAddtnlSign prevDISAddtnlSign = DISAddtnlSign::OFF;
        if (mIsIgnOn == true) {
            if (mPrivate_ISLAAddtnlSign == Private_ISLAAddtnlSign::OFF) {
                if (mDISAddtnlSign == DISAddtnlSign::OFF) {
                    if (!mPriorityDISFlag && !mPriorityISLAFlag) {
                        setSFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat(SFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat::OFF);
                        setSFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat(
                            SFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat::OFF);
                        setSFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat(
                            SFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat::OFF);
                        prevDISAddtnlSign = DISAddtnlSign::OFF;
                        prevPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::OFF;
                    }
                } else {
                    // DISAddtnlSign → at least 300ms
                    if (!mPriorityISLAFlag && !mPriorityDISFlag) {
                        setSFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat(
                            static_cast<SFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat>(mDISAddtnlSign));
                        setSFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat(
                            SFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat::DISADDTNLSIGN_STAT);

                        mOneShotTimerPriorityDIS.stop();

                        if (prevDISAddtnlSign != mDISAddtnlSign) {
                            mPriorityDISFlag = true;
                            if (mOneShotTimerPriorityDIS.start() == false) {
                                DWarning() << "[ADAS_DRIVING_2_0] mOneShotTimerPriorityDIS Start Failed";
                            }
                            prevDISAddtnlSign = mDISAddtnlSign;
                        }
                    }
                }
            } else {
                // Private_ISLAAddtnlSign → at least 300ms
                if (!mPriorityISLAFlag && !mPriorityDISFlag) {
                    setSFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat(
                        SFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat::ISLAADDTNLSIGN_STAT);
                    setSFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat(mPrivate_ISLAAddtnlSign);

                    mOneShotTimerPriorityISLA.stop();

                    if (prevPrivate_ISLAAddtnlSign != mPrivate_ISLAAddtnlSign) {
                        mPriorityISLAFlag = true;
                        if (mOneShotTimerPriorityISLA.start() == false) {
                            DWarning() << "[ADAS_DRIVING_2_0] mOneShotTimerPriorityISLA Start Failed";
                        }
                        prevPrivate_ISLAAddtnlSign = mPrivate_ISLAAddtnlSign;
                    }
                }
            }
        } else {
            setSFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat(SFCADAS_DRIVING_2_0ConstantFinalAddtnlSignStat::OFF);
            setSFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat(SFCADAS_DRIVING_2_0ConstantFinalDISAddtnlSignStat::OFF);
            setSFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat(SFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat::OFF);
        }
    }

    // 5.2.5.2.3.3 보조 표지판 (③) - IMG_CONN_ISLAAddSign_Stat
    void updateConstantISLAAddSignStat() {
        SFCADAS_DRIVING_2_0ConstantISLAAddSignStat stat = SFCADAS_DRIVING_2_0ConstantISLAAddSignStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::RAIN) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSignStat::RAIN;
            } else if (mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::SNOW) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSignStat::SNOW;
            } else if (mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::SNOW_RAIN) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSignStat::SNOW_RAIN;
            } else if (mINDIslaSupplementaryTrafficSignStatus == INDIslaSupplementaryTrafficSignStatus::TRAILER) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSignStat::TRAILER;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLAAddSignStat(stat);
    }

    // 5.2.5.2.3.4 과속 경고음 심볼 (④) - IMG_CONN_ISLASoundSymbol_Stat
    // 5.2.5.2.3.4.1 ON/OFF 심볼
    void updateConstantIslaSoundSymbolStat() {
        SFCADAS_DRIVING_2_0ConstantISLASoundSymbolStat stat = SFCADAS_DRIVING_2_0ConstantISLASoundSymbolStat::OFF;
        if (mIsIgnOn == true) {
            if (mINDIslaWarningSoundSymbolStatus == INDIslaWarningSoundSymbolStatus::DISABLE) {
                stat = SFCADAS_DRIVING_2_0ConstantISLASoundSymbolStat::DISABLE;
            } else if (mINDIslaWarningSoundSymbolStatus == INDIslaWarningSoundSymbolStatus::ENABLE) {
                stat = SFCADAS_DRIVING_2_0ConstantISLASoundSymbolStat::ENABLE;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLASoundSymbolStat(stat);
    }

    // 5.2.5.2.3.5 사전 제한 속도 표지판 (⑤)
    // 5.2.5.2.3.5.1 형상 - IMG_CONN_ISLANextSpeedLimitTrafficSign_Stat
    void updateConstantISLANextSpeedLimitTrafficSignTypeStat() {
        SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat stat =
            SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::OFF;
        if (mIsIgnOn == true) {
            if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                 mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE1;
            } else if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE2;
            } else if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::WHITE_DISPLAY) &&
                       (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::WHITE_TYPE3;
            } else if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::DOMESTIC ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::EUROPE ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::AUSTRALIA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CHINA ||
                        mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::RUSSIA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE1;
            } else if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::CANADA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE2;
            } else if ((mINDIslaNextSpeedLimitTrafficSignStatus == INDIslaNextSpeedLimitTrafficSignStatus::GREEN_DISPLAY) &&
                       (mINDIslaTrafficSignCountryInformationStatus == INDIslaTrafficSignCountryInformationStatus::USA)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat::GREEN_TYPE3;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignTypeStat(stat);
    }

    // 5.2.5.2.3.5.2 제한 속도 - IMG_CONN_ISLANextSpeedLimitTrafficSign_Val
    void updateConstantISLANextSpeedLimitTrafficSignVal() {
        SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignStat stat =
            SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if (mINDIslaNextSpeedLimitTrafficSignValue == 0) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignStat::INVALID;
            } else if (1 <= mINDIslaNextSpeedLimitTrafficSignValue && mINDIslaNextSpeedLimitTrafficSignValue <= 252) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignStat::VALUE;
                value = mINDIslaNextSpeedLimitTrafficSignValue;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignStat(stat);
        setSFCADAS_DRIVING_2_0ConstantISLANextSpeedLimitTrafficSignValue(value);
    }

    // 5.2.5.2.3.6 사전 제한 속도 보조 표지판 (⑥)
    // 5.2.5.2.3.6.1 형상 - IMG_CONN_ISLANextSupportTrafficSign_Stat
    void updateConstantISLANextSupportTrafficSignUnitStat() {
        SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignUnitStat stat =
            SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignUnitStat::OFF;
        if (mIsIgnOn == true) {
            if (mTripDistanceUnit == TripDistanceUnit::KM) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignUnitStat::METER;
            } else if (mTripDistanceUnit == TripDistanceUnit::MILE) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignUnitStat::FEET;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignUnitStat(stat);
    }
    // 5.2.5.2.3.6.2 거리 - IMG_CONN_ISLANextSupportTrafficSign_Val
    void updateConstantISLANextSupportTrafficSignStat() {
        SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignStat stat =
            SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignStat::OFF;
        HUInt64 value = 0;
        if (mIsIgnOn == true) {
            if ((1 <= mINDIslaNextSupportTrafficSignValue && mINDIslaNextSupportTrafficSignValue <= 50) &&
                (mTripDistanceUnit == TripDistanceUnit::KM)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignStat::VALUE;
                value = mINDIslaNextSupportTrafficSignValue * 10;
            } else if ((1 <= mINDIslaNextSupportTrafficSignValue && mINDIslaNextSupportTrafficSignValue <= 50) &&
                       (mTripDistanceUnit == TripDistanceUnit::MILE)) {
                stat = SFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignStat::VALUE;
                value = mINDIslaNextSupportTrafficSignValue * 30;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignStat(stat);
        setSFCADAS_DRIVING_2_0ConstantISLANextSupportTrafficSignValue(value);
    }

    // 5.2.5.2.3.7 보조 표지판 2(⑦) - IMG_CONN_ISLAAddSign2_Stat
    void updateConstantISLAAddSign2Stat() {
        SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat stat = SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat::OFF;
        if (mIsIgnOn == true) {
            if (mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::RAIN) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat::RAIN;
            } else if (mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::SNOW) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat::SNOW;
            } else if (mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::SNOW_RAIN) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat::SNOW_RAIN;
            } else if (mINDIslaSupplementaryTrafficSign2Status == INDIslaSupplementaryTrafficSign2Status::TRAILER) {
                stat = SFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat::TRAILER;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0ConstantISLAAddSign2Stat(stat);
    }

    // 5.2.5.4 이벤트 표시 동작 로직
    // 5.2.5.4.1 1그룹 풀 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEventGroup1FullPopup1_1() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            switch (mNTFGroup1AdasWarning1_1Status) {
                case NTFGroup1AdasWarning1_1Status::WARNING_FRONT:
                    eventID = "E52002";
                    break;
                case NTFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT:
                    eventID = "E52003";
                    break;
                case NTFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT:
                    eventID = "E52004";
                    break;
                case NTFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT_CROSSING:
                    eventID = "E52005";
                    break;
                case NTFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT_CROSSING:
                    eventID = "E52006";
                    break;
                case NTFGroup1AdasWarning1_1Status::BRAKING_FRONT:
                    eventID = "E52007";
                    break;
                case NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT:
                    eventID = "E52008";
                    break;
                case NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT:
                    eventID = "E52009";
                    break;
                case NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT_CROSSING:
                    eventID = "E52010";
                    break;
                case NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT_CROSSING:
                    eventID = "E52011";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT:
                    eventID = "E52012";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT:
                    eventID = "E52013";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT_PATH:
                    eventID = "E52014";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT_PATH:
                    eventID = "E52015";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_REAR_LEFT:
                    eventID = "E52016";
                    break;
                case NTFGroup1AdasWarning1_1Status::STEERING_REAR_RIGHT:
                    eventID = "E52017";
                    break;
                case NTFGroup1AdasWarning1_1Status::END_OF_ASSIST:
                    eventID = "E52018";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group1FullPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_1ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup1_1Stat::ON);
        }
    }

    // 5.2.5.4.2 1그룹 풀 팝업 1-2 (FR_CMR, ADAS_DRV)
    void updateEventGroup1FullPopup1_2() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundType linkedSoundType =
            SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (mIsIgnOn == true) {
            switch (mNTFGroup1AdasWarning1_2Status) {
                case NTFGroup1AdasWarning1_2Status::HANDS_OFF_WARN_LV1:
                    eventID = "E52019";
                    break;
                case NTFGroup1AdasWarning1_2Status::HANDS_OFF_WARN_LV2:
                    eventID = "E52020";
                    break;
                case NTFGroup1AdasWarning1_2Status::LFA_AUTOMATIC_OFF:
                    eventID = "E52021";
                    break;
                case NTFGroup1AdasWarning1_2Status::HDA_AUTOMATIC_OFF:
                    eventID = "E52022";
                    break;
                case NTFGroup1AdasWarning1_2Status::DAW_TAKE_REST:
                    eventID = "E52023";
                    break;
                case NTFGroup1AdasWarning1_2Status::LCA_MANUAL_ON_REQUEST:
                    eventID = "E52069";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup1AdasWarning1_2Status::TD_LV2:
                    eventID = "E52073";
                    break;
                case NTFGroup1AdasWarning1_2Status::EMERGENCY_STOP:
                    eventID = "E52113";
                    break;
                case NTFGroup1AdasWarning1_2Status::EMERGENCY_STOP_INACTIVE:
                    eventID = "E52110";
                    break;
                case NTFGroup1AdasWarning1_2Status::HDA_HF_MANUAL_ON_REQUEST:
                    eventID = "E52114";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL1:
                    eventID = "E52116";
                    break;
                case NTFGroup1AdasWarning1_2Status::HANDS_ON_REQUEST_LEVEL2:
                    eventID = "E52117";
                    break;
                case NTFGroup1AdasWarning1_2Status::DAW_TAKE_REST_WITH_OK_BUTTON:
                    eventID = "E52119";
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
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat::ON);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundID(linkedSoundID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundType(linkedSoundType);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2LinkedSoundRepeatCount(repeatCount);
            flushLastGroup();
        } else {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2ID(lastActiveEventID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2ID(lastActiveEventID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup1_2Stat::OFF);
            flushLastGroup();
        }

        lastActiveEventID = eventID;
    }

    // 5.2.5.4.3 1그룹 풀 팝업 2-1 (RR_C_RDR, ADAS_DRV)
    void updateEventGroup1FullPopup2_1() {
        std::string prevEventID;
        std::string eventID;
        SFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat stat = SFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat::OFF;
        if (mNTFGroup1AdasWarning2_1Status == NTFGroup1AdasWarning2_1Status::WARNING_CHECK_BLIND_SPOT) {
            stat = SFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat::ON;
            if (mIsIgnOn == true) {
                eventID = "E52030";
            } else {
                eventID = "E52094";
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group1FullPopup2_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat(SFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup1FullPopup2_1Stat(stat);
        }
    }

    // 5.2.5.4.4 4그룹 풀 팝업 1-1 (FR_CMR, ADAS_DRV)
    void updateEventGroup4FullPopup1_1() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType linkedSoundType =
            SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (mIsIgnOn == true) {
            switch (mNTFGroup4AdasWarning1_1Status) {
                case NTFGroup4AdasWarning1_1Status::SCC_MRM:
                    eventID = "E52031";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::INFINITE;
                    break;
                case NTFGroup4AdasWarning1_1Status::SCC_ATTENTION_WARNING:
                    eventID = "E52032";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::SCC_AUTOMATIC_OFF:
                    eventID = "E52033";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::SCC_CONDITION_NOT_MET:
                    eventID = "E52034";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET:
                    eventID = "E52040";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::SCC_STOP_CONTROL:
                    eventID = "E52035";
                    break;
                case NTFGroup4AdasWarning1_1Status::CONNECTING_TRAILER_OFF:
                    eventID = "E52036";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::DAW_LEAD_VEHICLE_DEPARTURE:
                    eventID = "E52037";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_REGULATIONS:
                    eventID = "E52038";
                    break;
                case NTFGroup4AdasWarning1_1Status::OFF_DUE_TO_LIMITATION_ON_DRIVER_MONITORING:
                    eventID = "E52109";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::SCC_CONDITION_NOT_MET_BY_MRM:
                    eventID = "E52111";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::LFA_CONDITION_NOT_MET_BY_EMERGENCY_STOP:
                    eventID = "E52118";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case NTFGroup4AdasWarning1_1Status::HANDS_FREE_INITIAL_ACTIVATION:
                    eventID = "E52115";
                    break;
                case NTFGroup4AdasWarning1_1Status::COMMON_INHIBIT_OFF_IN_IIHS:
                    eventID = "E52122";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group4FullPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup4FullPopup1_1Stat::ON);
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundID(linkedSoundID);
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundType(linkedSoundType);
            setSFCADAS_DRIVING_2_0EventGroup4FullPopup1_1LinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.2.5.4.5 7그룹 풀 팝업 – 후측방 안전 고장
    void updateEventGroup7FullPopupBlindSpotSafetyFailure() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType linkedSoundType =
            SFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (mIsIgnOn == true) {
            if (mNTFGroup7BlindSpotSafetyFailure2Status == NTFGroup7BlindSpotSafetyFailure2Status::CONNECTING_TRAILER_OFF) {
                eventID = "E52050";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mNTFGroup7BlindSpotSafetyFailure2Status == NTFGroup7BlindSpotSafetyFailure2Status::DISPLAY_OFF) {
                eventID = "E52051";
            } else {
                // OFF
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group7FullPopupBlindSpotSafetyFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureStat(
                SFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureStat(
                SFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureStat::ON);
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundID(linkedSoundID);
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundType(linkedSoundType);
            setSFCADAS_DRIVING_2_0EventGroup7FullPopupBlindSpotSafetyFailureLinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.2.5.4.6 7그룹 풀 팝업 – 사이드 미러 고장
    void updateEventGroup7FullPopupOutsideMirrorSymbolFailure() {
        SFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureStat eventStat =
            SFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureStat::OFF;
        if (mIsIgnOn == true) {
            if (mNTFGroup7OutsideMirrorSymbolFailure2Status == NTFGroup7OutsideMirrorSymbolFailure2Status::FAILURE) {
                eventStat = SFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureStat::ON;
            } else {
                // OFF
            }
        }
        setSFCADAS_DRIVING_2_0EventGroup7FullPopupOutsideMirrorSymbolFailureStat(eventStat);
    }

    // 5.2.5.4.7 7그룹 풀 팝업 – 운전자 보조 시스템 고장 1
    void updateEventGroup7FullPopup1() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7DriverAssistFailure1Status == NTFGroup7DriverAssistFailure1Status::UNHANDLED_TIMEOUT &&
                 (mConfigAdasLKA == ConfigAdasLKA::ON || mConfigAdasFCA == ConfigAdasFCA::ON)) ||
                (mNTFGroup7DriverAssistFailure1Status == NTFGroup7DriverAssistFailure1Status::FAILURE)) {
                eventID = "E52088";
            } else if (mNTFGroup7DriverAssistFailure1Status == NTFGroup7DriverAssistFailure1Status::CAMERA_BLOCKAGE) {
                eventID = "E52089";
            } else if (mNTFGroup7DriverAssistFailure1Status == NTFGroup7DriverAssistFailure1Status::RADAR_BLOCKAGE) {
                eventID = "E52090";
            } else if (mNTFGroup7DriverAssistFailure1Status == NTFGroup7DriverAssistFailure1Status::CAMERA_RADAR_BLOCKAGE) {
                eventID = "E52091";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.DriverAssistFailure1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure1Stat(SFCADAS_DRIVING_2_0EventDriverAssistFailure1Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure1ID(eventID);
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure1Stat(SFCADAS_DRIVING_2_0EventDriverAssistFailure1Stat::ON);
        }
    }

    // 5.2.5.4.8 7그룹 풀 팝업 – 운전자 보조 시스템 고장 2
    void updateEventGroup7FullPopup2() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        if (mIsIgnOn == true) {
            if ((mNTFGroup7DriverAssistFailure2Status == NTFGroup7DriverAssistFailure2Status::UNHANDLED_TIMEOUT &&
                 mConfigNewBCW == ConfigNewBCW::ON) ||
                (mNTFGroup7DriverAssistFailure2Status == NTFGroup7DriverAssistFailure2Status::FAILURE)) {
                eventID = "E52092";
            } else if (mNTFGroup7DriverAssistFailure2Status == NTFGroup7DriverAssistFailure2Status::RADAR_BLOCKAGE) {
                eventID = "E52093";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.DriverAssistFailure2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure2Stat(SFCADAS_DRIVING_2_0EventDriverAssistFailure2Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure2ID(eventID);
            setSFCADAS_DRIVING_2_0EventDriverAssistFailure2Stat(SFCADAS_DRIVING_2_0EventDriverAssistFailure2Stat::ON);
        }
    }

    // 5.2.5.4.9 2그룹 미니 팝업 1-1
    void updateEventGroup2MiniPopup1_1() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            switch (mNTMGroup2AdasWarning1_1Status) {
                case NTMGroup2AdasWarning1_1Status::MSLA_AUTOSET_ON:
                    eventID = "E52501";
                    break;
                case NTMGroup2AdasWarning1_1Status::MSLA_AUTOSET_SPEED_CHANGED:
                    eventID = "E52502";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_AUTOSET_ON:
                    eventID = "E52503";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_AUTOSET_SPEED_CHANGED:
                    eventID = "E52504";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV1:
                    eventID = "E52505";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV2:
                    eventID = "E52506";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV3:
                    eventID = "E52507";
                    break;
                case NTMGroup2AdasWarning1_1Status::SCC_DISTANCE_LV4:
                    eventID = "E52508";
                    break;
                case NTMGroup2AdasWarning1_1Status::NSCC_WARNING:
                    eventID = "E52509";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_COLLISION:
                    eventID = "E52510";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_CONDITION_NOT_MET:
                    eventID = "E52511";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_DRIVING_LANE_ANALYZING:
                    eventID = "E52512";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_CURVED_ROAD:
                    eventID = "E52513";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_NARROW_LANE:
                    eventID = "E52514";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_NOT_OPERATIONAL_SECTION:
                    eventID = "E52515";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_HAZARD_LIGHT:
                    eventID = "E52516";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_LOW_SPEED:
                    eventID = "E52517";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_HANDS_OFF:
                    eventID = "E52518";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_UNAVAILABLE_LANE:
                    eventID = "E52519";
                    break;
                case NTMGroup2AdasWarning1_1Status::LCA_STEERING_CONTROL_DETECTED:
                    eventID = "E52520";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group2MiniPopup1_1.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup2MiniPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup2MiniPopup1_1Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup2MiniPopup1_1ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup2MiniPopup1_1Stat(SFCADAS_DRIVING_2_0EventGroup2MiniPopup1_1Stat::ON);
        }
    }

    // 5.2.5.4.10 2그룹 미니 팝업 1-2
    void updateEventGroup2MiniPopup1_2() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            switch (mNTMGroup2AdasWarning1_2Status) {
                case NTMGroup2AdasWarning1_2Status::STEERING_CONTROL_DETECTED:
                    eventID = "E52644";
                    break;
                case NTMGroup2AdasWarning1_2Status::ACCELERATOR_PEDAL_CONTROL_DETECTED:
                    eventID = "E52645";
                    break;
                case NTMGroup2AdasWarning1_2Status::CONDITION_NOT_MET:
                    eventID = "E52646";
                    break;
                case NTMGroup2AdasWarning1_2Status::MAXIMUM_SPEED_EXCEEDED:
                    eventID = "E52647";
                    break;
                case NTMGroup2AdasWarning1_2Status::MAXIMUM_SET_SPEED_EXCEEDED:
                    eventID = "E52648";
                    break;
                case NTMGroup2AdasWarning1_2Status::IC_JC_AHEAD:
                    eventID = "E52649";
                    break;
                case NTMGroup2AdasWarning1_2Status::TOLLGATE_AHEAD:
                    eventID = "E52650";
                    break;
                case NTMGroup2AdasWarning1_2Status::NOT_SUPPORTED_AREA_AHEAD:
                    eventID = "E52651";
                    break;
                case NTMGroup2AdasWarning1_2Status::SUPPORTED_ROAD_ENDING_AHEAD:
                    eventID = "E52652";
                    break;
                case NTMGroup2AdasWarning1_2Status::CURVED_ROAD:
                    eventID = "E52653";
                    break;
                case NTMGroup2AdasWarning1_2Status::PEDESTRIAN_OR_CYCLIST_DETECTED:
                    eventID = "E52654";
                    break;
                case NTMGroup2AdasWarning1_2Status::LANE_DEPARTURE_DETECTED:
                    eventID = "E52655";
                    break;
                case NTMGroup2AdasWarning1_2Status::DRIVER_NOT_DETECTED:
                    eventID = "E52656";
                    break;
                case NTMGroup2AdasWarning1_2Status::DEACTIVATION_LANE_FOLLOWING_ASSIST:
                    eventID = "E52657";
                    break;
                case NTMGroup2AdasWarning1_2Status::DEACTIVATION_HIGHWAY_DRIVING_ASSIST:
                    eventID = "E52658";
                    break;
                case NTMGroup2AdasWarning1_2Status::SUGGEST_LANE_CHANGE_TO_LEFT_SIDE:
                    eventID = "E52659";
                    break;
                case NTMGroup2AdasWarning1_2Status::SUGGEST_LANE_CHANGE_TO_RIGHT_SIDE:
                    eventID = "E52660";
                    break;
                case NTMGroup2AdasWarning1_2Status::UNSUPPORTED_WEATHER_CONDITIONS:
                    eventID = "E52663";
                    break;
                case NTMGroup2AdasWarning1_2Status::ROAD_CONSTRUCTION_AHEAD:
                    eventID = "E52664";
                    break;
                default:
                    break;
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.Group2Warning1_2.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventGroup2Warning1_2Stat(SFCADAS_DRIVING_2_0EventGroup2Warning1_2Stat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventGroup2Warning1_2ID(eventID);
            setSFCADAS_DRIVING_2_0EventGroup2Warning1_2Stat(SFCADAS_DRIVING_2_0EventGroup2Warning1_2Stat::ON);
        }
    }

    // 5.2.5.4.11 대형 풀 팝업
    void updateEventNTFHandsOff() {
        std::string prevEventID;
        std::string eventID;
        if (mIsIgnOn == true) {
            if (mNTFHandsOffStatus == NTFHandsOffStatus::SCC2_DCA) {
                eventID = "E52120";
            } else if (mNTFHandsOffStatus == NTFHandsOffStatus::SCC2_EMERGENCY_STOP) {
                eventID = "E52121";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Event.LargeFullPopUp.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_DRIVING_2_0EventLargeFullPopUpStat(SFCADAS_DRIVING_2_0EventLargeFullPopUpStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_DRIVING_2_0EventLargeFullPopUpID(eventID);
            setSFCADAS_DRIVING_2_0EventLargeFullPopUpStat(SFCADAS_DRIVING_2_0EventLargeFullPopUpStat::ON);
        }
    }

    // 5.2.6 사운드 로직
    // 5.2.6.1 경고음 1-1
    void updateSoundWarning1_1Status() {
        std::string soundID;
        if (mSoundWarning1_1Status == SoundWarning1_1Status::WARNING_SOUND_2_AUTO) {
            soundID = "SND_ADAS_PopUpWarn2_Auto";
        } else if (mSoundWarning1_1Status == SoundWarning1_1Status::WARNING_SOUND_3_AUTO) {
            soundID = "SND_ADAS_PopUpWarn3_Auto";
        } else {
            // no operation
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Sound.Warning1_1.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_DRIVING_2_0SoundWarning1_1Stat(SFCADAS_DRIVING_2_0SoundWarning1_1Stat::OFF);
            flushLastGroup();
        }
        if (soundID.empty() == false) {
            setSFCADAS_DRIVING_2_0SoundWarning1_1ID(soundID);
            setSFCADAS_DRIVING_2_0SoundWarning1_1Stat(SFCADAS_DRIVING_2_0SoundWarning1_1Stat::ON);
        }
    }

    // 5.2.6.2 경고음 1-2
    void updateSoundWarning1_2Status() {
        std::string soundID;
        SFCADAS_DRIVING_2_0SoundWarning1_2Type type = SFCADAS_DRIVING_2_0SoundWarning1_2Type::NONE;
        HUInt64 count = 0;
        if (mIsIgnOn == true) {
            if (mSoundWarning1_2Status == SoundWarning1_2Status::WARNING_SOUND_1_MEDIUM) {
                soundID = "SND_ADAS_PopUpWarn1_Fixed";
                type = SFCADAS_DRIVING_2_0SoundWarning1_2Type::REPEAT_COUNT;
                count = 1;
            } else if (mSoundWarning1_2Status == SoundWarning1_2Status::WARNING_SOUND_1_AUTO) {
                soundID = "SND_ADAS_PopUpWarn1_Auto";
                type = SFCADAS_DRIVING_2_0SoundWarning1_2Type::INFINITE;
            } else if (mSoundWarning1_2Status == SoundWarning1_2Status::WARNING_SOUND_2_AUTO) {
                soundID = "SND_ADAS_PopUpWarn2_Auto";
                type = SFCADAS_DRIVING_2_0SoundWarning1_2Type::INFINITE;
            } else {
                // OFF
            }
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Sound.Warning1_2.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_DRIVING_2_0SoundWarning1_2Stat(SFCADAS_DRIVING_2_0SoundWarning1_2Stat::OFF);
            flushLastGroup();
        }
        if (soundID.empty() == false) {
            setSFCADAS_DRIVING_2_0SoundWarning1_2ID(soundID);
            setSFCADAS_DRIVING_2_0SoundWarning1_2Stat(SFCADAS_DRIVING_2_0SoundWarning1_2Stat::ON);
            setSFCADAS_DRIVING_2_0SoundWarning1_2Type(type);
            setSFCADAS_DRIVING_2_0SoundWarning1_2RepeatCount(count);
        }
    }

    // 5.2.6.3 경고음 1-3
    void updateSoundWarning1_3Status() {
        std::string soundID;
        SFCADAS_DRIVING_2_0SoundWarning1_3Type type = SFCADAS_DRIVING_2_0SoundWarning1_3Type::NONE;
        HUInt64 count = 0;
        if (mIsIgnOn == true) {
            if (mSoundWarning1_3Status == SoundWarning1_3Status::SPECIAL_SOUND_1_AUTO) {
                if (mAdasOverSpeedWarnOnOffStatus == AdasOverSpeedWarnOnOffStatus::NON_OPTION) {
                    if (mIslaOption2Status == IslaOption2Status::INVALID) {
                        soundID = "SND_ADAS_MSLA_Auto_Ancient";
                        type = SFCADAS_DRIVING_2_0SoundWarning1_3Type::INFINITE;
                    } else if (mIslaOption2Status != IslaOption2Status::INVALID) {
                        soundID = "SND_ADAS_MSLA_Auto_Old";
                        type = SFCADAS_DRIVING_2_0SoundWarning1_3Type::INFINITE;
                    } else {
                        // OFF
                    }
                } else if (mAdasOverSpeedWarnOnOffStatus != AdasOverSpeedWarnOnOffStatus::NON_OPTION) {
                    soundID = "SND_ADAS_MSLA_Auto";
                    type = SFCADAS_DRIVING_2_0SoundWarning1_3Type::INFINITE;
                } else {
                    // OFF
                }
            } else if (mSoundWarning1_3Status == SoundWarning1_3Status::ALARM_SOUND_1_MEDIUM) {
                soundID = "SND_ADAS_PopUpInform1_Fixed";
                type = SFCADAS_DRIVING_2_0SoundWarning1_3Type::REPEAT_COUNT;
                count = 1;
            } else {
                // OFF
            }
        }
        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_DRIVING_2_0.Sound.Warning1_3.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_DRIVING_2_0SoundWarning1_3Stat(SFCADAS_DRIVING_2_0SoundWarning1_3Stat::OFF);
            flushLastGroup();
        }
        if (soundID.empty() == false) {
            setSFCADAS_DRIVING_2_0SoundWarning1_3ID(soundID);
            setSFCADAS_DRIVING_2_0SoundWarning1_3Stat(SFCADAS_DRIVING_2_0SoundWarning1_3Stat::ON);
            setSFCADAS_DRIVING_2_0SoundWarning1_3Type(type);
            setSFCADAS_DRIVING_2_0SoundWarning1_3RepeatCount(count);
        }
    }

    // 5.2.6.4 경고음 1-4
    void updateSoundWarning1_4Status() {
        SFCADAS_DRIVING_2_0SoundWarning1_4Stat stat = SFCADAS_DRIVING_2_0SoundWarning1_4Stat::OFF;
        if (mIsIgnOn == true && mSoundWarning1_4Status == SoundWarning1_4Status::ALARM_SOUND_2_MEDIUM) {
            stat = SFCADAS_DRIVING_2_0SoundWarning1_4Stat::ON;
        }
        setSFCADAS_DRIVING_2_0SoundWarning1_4Stat(stat);
    }

    // 5.2.6.5 경고음 1-5
    void updateSoundWarning1_5Status() {
        SFCADAS_DRIVING_2_0SoundWarning1_5Stat stat = SFCADAS_DRIVING_2_0SoundWarning1_5Stat::OFF;
        if (mIsIgnOn == true && mSoundWarning1_5Status == SoundWarning1_5Status::SPECIAL_SOUND_4_AUTO) {
            stat = SFCADAS_DRIVING_2_0SoundWarning1_5Stat::ON;
        }
        setSFCADAS_DRIVING_2_0SoundWarning1_5Stat(stat);
    }

    // 5.2.6.6 경고음 2-1
    void updateSoundWarning2_1Status() {
        SFCADAS_DRIVING_2_0SoundWarning2_1Stat stat = SFCADAS_DRIVING_2_0SoundWarning2_1Stat::OFF;
        if (mSoundWarning2_1Status == SoundWarning2_1Status::SPECIAL_SOUND_5_AUTO ||
            mSoundWarning2_1Status == SoundWarning2_1Status::SPECIAL_SOUND_5_AUTO_SEW) {
            stat = SFCADAS_DRIVING_2_0SoundWarning2_1Stat::ON;
        }
        setSFCADAS_DRIVING_2_0SoundWarning2_1Stat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using Private_ISLAAddtnlSign = SFCADAS_DRIVING_2_0ConstantFinalISLAAddtnlSignStat;
    using ConfigAdasFCA = ArgumentsConfigFca1Changed::Inter_ConfigAdasFCA;
    using ConfigAdasFCA2 = ArgumentsConfigFca2Changed::Inter_ConfigAdasFCA2;
    using ConfigAdasLKA = ArgumentsConfigLkaChanged::Inter_ConfigAdasLKA;
    using ConfigAdasDAW = ArgumentsConfigDawChanged::Inter_ConfigAdasDAW;
    using ConfigILCU = ArgumentsConfigIlcuIfsChanged::Inter_ConfigILCU;
    using ConfigIFS = ArgumentsConfigIlcuIfsChanged::Inter_ConfigIFS;
    using ConfigNewHBA = ArgumentsConfigHbaChanged::Inter_ConfigNewHBA;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigAdasISLA = ArgumentsConfigIslaChanged::Inter_ConfigAdasISLA;
    using ConfigNewBCW = ArgumentsConfigBcwChanged::Inter_ConfigNewBCW;
    using ConfigAdasSCC = ArgumentsConfigAdasSCCChanged::Inter_ConfigAdasSCC;
    using ConfigAdasLFA = ArgumentsConfigAdasLFAChanged::Inter_ConfigAdasLFA;
    using ConfigAdasHDA = ArgumentsConfigAdasHDAChanged::Inter_ConfigAdasHDA;
    using ConfigAdasHDA2 = ArgumentsConfigAdasHDA2Changed::Inter_ConfigAdasHDA2;
    using ConfigAdasHDA3 = ArgumentsConfigAdasHDA3Changed::Inter_ConfigAdasHDA3;
    using ConfigAdasEmergencyStop = ArgumentsConfigAdasEmergencyStopChanged::Inter_ConfigAdasEmergencyStop;
    using DISAddtnlSign = ArgumentsDisAddtnlSignChanged::Inter_DISAddtnlSign;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;
    using INDIslaWarningSoundSymbolStatus =
        ArgumentsIndIslaWarningSoundSymbolStatusChanged::Input_INDIslaWarningSoundSymbolStatus;
    using INDForwardSafetySymbolStatus = ArgumentsIndForwardSafetySymbolStatusChanged::Input_INDForwardSafetySymbolStatus;
    using INDEmergencySteeringSymbolStatus =
        ArgumentsIndEmergencySteeringSymbolStatusChanged::Input_INDEmergencySteeringSymbolStatus;
    using INDLaneSafetySymbolStatus = ArgumentsIndLaneSafetySymbolStatusChanged::Input_INDLaneSafetySymbolStatus;
    using INDDriveAttentionWarningSymbolStatus =
        ArgumentsIndDriveAttentionWarningSymbolStatusChanged::Input_INDDriveAttentionWarningSymbolStatus;
    using InterTelltaleIccDrow = ArgumentsInterTelltaleIccDrowChanged::Inter_TelltaleIccDrow;
    using IlcuLhActWarnStatus = ArgumentsIlcuActWarnStatusChanged::Input_IlcuLhActWarnStatus;
    using IlcuRhActWarnStatus = ArgumentsIlcuActWarnStatusChanged::Input_IlcuRhActWarnStatus;
    using INDLfaSymbol1Status = ArgumentsIndLfaSymbolStatusChanged::Input_INDLfaSymbol1Status;
    using INDLfaSymbol2Status = ArgumentsIndLfaSymbolStatusChanged::Input_INDLfaSymbol2Status;
    using INDLcaLeftSymbol1Status = ArgumentsIndLcaLeftSymbolStatusChanged::Input_INDLcaLeftSymbol1Status;
    using INDLcaLeftSymbol2Status = ArgumentsIndLcaLeftSymbolStatusChanged::Input_INDLcaLeftSymbol2Status;
    using INDLcaRightSymbol1Status = ArgumentsIndLcaRightSymbolStatusChanged::Input_INDLcaRightSymbol1Status;
    using INDLcaRightSymbol2Status = ArgumentsIndLcaRightSymbolStatusChanged::Input_INDLcaRightSymbol2Status;
    using INDDriverHandsOnOff1Status = ArgumentsIndDriverHandsOnOffStatusChanged::Input_INDDriverHandsOnOff1Status;
    using INDDriverHandsOnOff2Status = ArgumentsIndDriverHandsOnOffStatusChanged::Input_INDDriverHandsOnOff2Status;
    using INDSetSpeed1Status = ArgumentsIndSetSpeedStatusChanged::Input_INDSetSpeed1Status;
    using INDSetSpeed2Status = ArgumentsIndSetSpeedStatusChanged::Input_INDSetSpeed2Status;
    using INDSccSymbol1Status = ArgumentsIndSccSymbolStatusChanged::Input_INDSccSymbol1Status;
    using INDSccSymbol2Status = ArgumentsIndSccSymbolStatusChanged::Input_INDSccSymbol2Status;
    using INDNsccSymbolStatus = ArgumentsIndNsccSymbolStatusChanged::Input_INDNsccSymbolStatus;
    using BGStausUppBar = ArgumentsBgstausUppBarChanged::Input_BGStausUppBar;
    using BGStausLeftBar = ArgumentsBgstausLeftBarChanged::Input_BGStausLeftBar;
    using BGStausRightBar = ArgumentsBgstausRightBarChanged::Input_BGStausRightBar;
    using NTFGroup7ForwardSafetyFailureStatus =
        ArgumentsNtfGroup7ForwardSafetyFailureStatusChanged::Input_NTFGroup7ForwardSafetyFailureStatus;
    using NTFGroup7ForwardSideSafetyFailureStatus =
        ArgumentsNtfGroup7ForwardSideSafetyFailureStatusChanged::Input_NTFGroup7ForwardSideSafetyFailureStatus;
    using NTFGroup7LaneSafetyFailureStatus =
        ArgumentsNtfGroup7LaneSafetyFailureStatusChanged::Input_NTFGroup7LaneSafetyFailureStatus;
    using NTFGroup7BlindSpotSafetyFailure2Status =
        ArgumentsNtfGroup7BlindSpotSafetyFailure2StatusChanged::Input_NTFGroup7BlindSpotSafetyFailure2Status;
    using NTFGroup7IslaFailureStatus = ArgumentsNtfGroup7IslaFailureStatusChanged::Input_NTFGroup7IslaFailureStatus;
    using NTFGroup7DawFailureStatus = ArgumentsNtfGroup7DawFailureStatusChanged::Input_NTFGroup7DawFailureStatus;
    using NTFGroup7HbaFailureStatus = ArgumentsNtfGroup7HbaFailureStatusChanged::Input_NTFGroup7HbaFailureStatus;
    using NTFGroup7SccFailureStatus = ArgumentsNtfGroup7SccFailureStatusChanged::Input_NTFGroup7SccFailureStatus;
    using NTFGroup7LfaFailureStatus = ArgumentsNtfGroup7LfaFailureStatusChanged::Input_NTFGroup7LfaFailureStatus;
    using NTFGroup7HdaFailureStatus = ArgumentsNtfGroup7HdaFailureStatusChanged::Input_NTFGroup7HdaFailureStatus;
    using NTFGroup7HdaPlusFailureStatus = ArgumentsNtfGroup7HdaPlusFailureStatusChanged::Input_NTFGroup7HdaPlusFailureStatus;
    using NTFGroup7LcaFailureStatus = ArgumentsNtfGroup7LcaFailureStatusChanged::Input_NTFGroup7LcaFailureStatus;
    using NTFGroup7MRMFailureStatus = ArgumentsNtfGroup7MRMFailureStatusChanged::Input_NTFGroup7MRMFailureStatus;
    using IfsIndiReqStatus = ArgumentsIfsIndiReqStatusChanged::Input_IfsIndiReqStatus;
    using INDHbaSymbolStatus = ArgumentsIndHbaSymbolStatusChanged::Input_INDHbaSymbolStatus;
    using VCLeftLineStatus = ArgumentsVcLeftLineStatusChanged::Input_VCLeftLineStatus;
    using VCRightLineStatus = ArgumentsVcRightLineStatusChanged::Input_VCRightLineStatus;
    using VCVehicleDistanceStatus = ArgumentsVcVehicleDistanceStatusChanged::Input_VCVehicleDistanceStatus;
    using VCVehicleDistanceLevelValue = ArgumentsVcVehicleDistanceLevelValueChanged::Input_VCVehicleDistanceLevelValue;
    using VCCenterRoadSurfaceStatus = ArgumentsVcCenterRoadSurfaceStatusChanged::Input_VCCenterRoadSurfaceStatus;

    using VCSurroundingObject01Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject01Status;
    using VCSurroundingObject02Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject02Status;
    using VCSurroundingObject03Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject03Status;
    using VCSurroundingObject04Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject04Status;
    using VCSurroundingObject05Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject05Status;
    using VCSurroundingObject06Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject06Status;
    using VCSurroundingObject07Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject07Status;
    using VCSurroundingObject08Status = ArgumentsVcSurroundingObject01to08StatusChanged::Input_VCSurroundingObject08Status;
    using VCSurroundingObject09Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject09Status;
    using VCSurroundingObject10Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject10Status;
    using VCSurroundingObject11Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject11Status;
    using VCSurroundingObject12Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject12Status;
    using VCSurroundingObject13Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject13Status;
    using VCSurroundingObject14Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject14Status;
    using VCSurroundingObject15Status = ArgumentsVcSurroundingObject09to15StatusChanged::Input_VCSurroundingObject15Status;
    using VCSurroundingObjectClassification01Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification01Status;
    using VCSurroundingObjectClassification02Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification02Status;
    using VCSurroundingObjectClassification03Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification03Status;
    using VCSurroundingObjectClassification04Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification04Status;
    using VCSurroundingObjectClassification05Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification05Status;
    using VCSurroundingObjectClassification06Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification06Status;
    using VCSurroundingObjectClassification07Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification07Status;
    using VCSurroundingObjectClassification08Status =
        ArgumentsVcSurroundingObjectClassification01to08StatusChanged::Input_VCSurroundingObjectClassification08Status;
    using VCSurroundingObjectClassification09Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification09Status;
    using VCSurroundingObjectClassification10Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification10Status;
    using VCSurroundingObjectClassification11Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification11Status;
    using VCSurroundingObjectClassification12Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification12Status;
    using VCSurroundingObjectClassification13Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification13Status;
    using VCSurroundingObjectClassification14Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification14Status;
    using VCSurroundingObjectClassification15Status =
        ArgumentsVcSurroundingObjectClassification09to15StatusChanged::Input_VCSurroundingObjectClassification15Status;
    using VCFrontRadarWaveStatus = ArgumentsVcFrontRadarWaveStatusChanged::Input_VCFrontRadarWaveStatus;
    using VCFrontLeftRadarWaveStatus = ArgumentsVcFrontLeftRadarWaveStatusChanged::Input_VCFrontLeftRadarWaveStatus;
    using VCFrontRightRadarWaveStatus = ArgumentsVcFrontRightRadarWaveStatusChanged::Input_VCFrontRightRadarWaveStatus;
    using VCRearLeftRadarWave1Status = ArgumentsVcRearLeftRadarWaveStatusChanged::Input_VCRearLeftRadarWave1Status;
    using VCRearLeftRadarWave2Status = ArgumentsVcRearLeftRadarWaveStatusChanged::Input_VCRearLeftRadarWave2Status;
    using VCRearRightRadarWave1Status = ArgumentsVcRearRightRadarWaveStatusChanged::Input_VCRearRightRadarWave1Status;
    using VCRearRightRadarWave2Status = ArgumentsVcRearRightRadarWaveStatusChanged::Input_VCRearRightRadarWave2Status;
    using VCLaneChangeRectangleStatus = ArgumentsVcLaneChangeRectangleStatusChanged::Input_VCLaneChangeRectangleStatus;
    using VCLeftLeftLineStatus = ArgumentsVcLeftLeftLineStatusChanged::Input_VCLeftLeftLineStatus;
    using VCRightRightLineStatus = ArgumentsVcRightRightLineStatusChanged::Input_VCRightRightLineStatus;
    using VCRearLeftObjectMovingDirection2Status =
        ArgumentsVcRearLeftObjectMovingDirection2StatusChanged::Input_VCRearLeftObjectMovingDirection2Status;
    using VCRearRightObjectMovingDirection2Status =
        ArgumentsVcRearRightObjectMovingDirection2StatusChanged::Input_VCRearRightObjectMovingDirection2Status;
    using VCLaneChangeAvailableStatus = ArgumentsVcLaneChangeAvailableStatusChanged::Input_VCLaneChangeAvailableStatus;
    using BrakeLampControlCommand_Status = ArgumentsBrakeLampControlCommand_StatusChanged::Input_BrakeLampControlCommand_Status;
    using ExtTurnLeftLampStatus = ArgumentsExtLampInputValueChanged::Input_ExtTurnLeftLampStatus;
    using ExtTurnRightLampStatus = ArgumentsExtLampInputValueChanged::Input_ExtTurnRightLampStatus;
    using ExtLowBeamOnReqStatus = ArgumentsExtLampInputValueChanged::Input_ExtLowBeamOnReqStatus;
    using ExtHighBeamOnReqStatus = ArgumentsExtLampInputValueChanged::Input_ExtHighBeamOnReqStatus;
    using VCFrontLeftObjectMovingDirectionStatus =
        ArgumentsVcFrontLeftObjectMovingDirectionStatusChanged::Input_VCFrontLeftObjectMovingDirectionStatus;
    using VCFrontRightObjectMovingDirectionStatus =
        ArgumentsVcFrontRightObjectMovingDirectionStatusChanged::Input_VCFrontRightObjectMovingDirectionStatus;
    using VCEmergencyStreeingStatus = ArgumentsVcEmergencyStreeingStatusChanged::Input_VCEmergencyStreeingStatus;
    using VCAdditionalStatus = ArgumentsVcAdditionalStatusChanged::Input_VCAdditionalStatus;
    using VCLeftLaneChangeDirectionStatus =
        ArgumentsVcLeftLaneChangeDirectionStatusChanged::Input_VCLeftLaneChangeDirectionStatus;
    using VCRightLaneChangeDirectionStatus =
        ArgumentsVcRightLaneChangeDirectionStatusChanged::Input_VCRightLaneChangeDirectionStatus;
    using INDIslaSpeedLimitTrafficSignStatus =
        ArgumentsIndIslaSpeedLimitTrafficSignStatusChanged::Input_INDIslaSpeedLimitTrafficSignStatus;
    using INDIslaTrafficSignCountryInformationStatus =
        ArgumentsIndIslaTrafficSignCountryInformationStatusChanged::Input_INDIslaTrafficSignCountryInformationStatus;
    using INDIslaAdditionalTrafficSignStatus =
        ArgumentsIndIslaAdditionalTrafficSignStatusChanged::Input_INDIslaAdditionalTrafficSignStatus;
    using INDIslaSupplementaryTrafficSignStatus =
        ArgumentsIndIslaSupplementaryTrafficSignStatusChanged::Input_INDIslaSupplementaryTrafficSignStatus;
    using INDIslaNextSpeedLimitTrafficSignStatus =
        ArgumentsIndIslaNextSpeedLimitTrafficSignStatusChanged::Input_INDIslaNextSpeedLimitTrafficSignStatus;
    using INDIslaSupplementaryTrafficSign2Status =
        ArgumentsIndIslaSupplementaryTrafficSign2StatusChanged::Input_INDIslaSupplementaryTrafficSign2Status;

    using NTFGroup1AdasWarning1_1Status = ArgumentsNtfGroup1AdasWarning1_1StatusChanged::Input_NTFGroup1AdasWarning1_1Status;
    using NTFGroup1AdasWarning1_2Status = ArgumentsNtfGroup1AdasWarning1_2StatusChanged::Input_NTFGroup1AdasWarning1_2Status;
    using NTFGroup1AdasWarning2_1Status = ArgumentsNtfGroup1AdasWarning2_1StatusChanged::Input_NTFGroup1AdasWarning2_1Status;
    using NTFGroup4AdasWarning1_1Status = ArgumentsNtfGroup4AdasWarning1_1StatusChanged::Input_NTFGroup4AdasWarning1_1Status;
    using NTFGroup7OutsideMirrorSymbolFailure2Status =
        ArgumentsNtfGroup7OutsideMirrorSymbolFailure2StatusChanged::Input_NTFGroup7OutsideMirrorSymbolFailure2Status;
    using NTFGroup7DriverAssistFailure1Status =
        ArgumentsNtfGroup7DriverAssistFailure1StatusChanged::Input_NTFGroup7DriverAssistFailure1Status;
    using NTFGroup7DriverAssistFailure2Status =
        ArgumentsNtfGroup7DriverAssistFailure2StatusChanged::Input_NTFGroup7DriverAssistFailure2Status;
    using NTMGroup2AdasWarning1_1Status = ArgumentsNtmGroup2AdasWarning1_1StatusChanged::Input_NTMGroup2AdasWarning1_1Status;
    using NTMGroup2AdasWarning1_2Status = ArgumentsNtmGroup2AdasWarning1_2StatusChanged::Input_NTMGroup2AdasWarning1_2Status;
    using NTFHandsOffStatus = ArgumentsNtfHandsOffStatusChanged::Input_NTFHandsOffStatus;
    using SoundWarning1_1Status = ArgumentsSoundWarning1_1StatusChanged::Input_SoundWarning1_1Status;
    using SoundWarning1_2Status = ArgumentsSoundWarning1_2StatusChanged::Input_SoundWarning1_2Status;
    using SoundWarning1_3Status = ArgumentsSoundWarning1_3StatusChanged::Input_SoundWarning1_3Status;
    using SoundWarning1_4Status = ArgumentsSoundWarning1_4StatusChanged::Input_SoundWarning1_4Status;
    using SoundWarning1_5Status = ArgumentsSoundWarning1_5StatusChanged::Input_SoundWarning1_5Status;
    using SoundWarning2_1Status = ArgumentsSoundWarning2_1StatusChanged::Input_SoundWarning2_1Status;
    using IslaOption2Status = ArgumentsSoundWarning1_3StatusChanged::Input_IslaOption2Status;
    using AdasOverSpeedWarnOnOffStatus = ArgumentsSoundWarning1_3StatusChanged::Input_AdasOverSpeedWarnOnOffStatus;

    static constexpr size_t kType0 = 0;
    static constexpr size_t kType1 = 1;
    static constexpr size_t kType2 = 2;
    static constexpr size_t kSurroundingObjectSize = 15;
    static constexpr size_t kSurroundingObjectInfoElementSize = 3;

    HBool mIsIgnOn = false;
    HBool mPriorityISLAFlag = false;
    HBool mPriorityDISFlag = false;

    ssfs::SFCTimer<ADAS_DRIVING_2_0> mOneShotTimerPriorityISLA;
    ssfs::SFCTimer<ADAS_DRIVING_2_0> mOneShotTimerPriorityDIS;
    static constexpr uint32_t kTimerInterval300ms = 300u;
    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigAdasFCA mConfigAdasFCA = ConfigAdasFCA::OFF;
    ConfigAdasFCA2 mConfigAdasFCA2 = ConfigAdasFCA2::OFF;
    ConfigAdasLKA mConfigAdasLKA = ConfigAdasLKA::OFF;
    ConfigAdasDAW mConfigAdasDAW = ConfigAdasDAW::OFF;
    ConfigILCU mConfigILCU = ConfigILCU::OFF;
    ConfigIFS mConfigIFS = ConfigIFS::OFF;
    ConfigNewHBA mConfigNewHBA = ConfigNewHBA::OFF;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigAdasISLA mConfigAdasISLA = ConfigAdasISLA::OFF;
    ConfigNewBCW mConfigNewBCW = ConfigNewBCW::OFF;
    ConfigAdasSCC mConfigAdasSCC = ConfigAdasSCC::OFF;
    ConfigAdasLFA mConfigAdasLFA = ConfigAdasLFA::OFF;
    ConfigAdasHDA mConfigAdasHDA = ConfigAdasHDA::OFF;
    ConfigAdasHDA2 mConfigAdasHDA2 = ConfigAdasHDA2::OFF;
    ConfigAdasHDA3 mConfigAdasHDA3 = ConfigAdasHDA3::OFF;
    ConfigAdasEmergencyStop mConfigAdasEmergencyStop = ConfigAdasEmergencyStop::OFF;
    DISAddtnlSign mDISAddtnlSign = DISAddtnlSign::OFF;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;
    INDIslaWarningSoundSymbolStatus mINDIslaWarningSoundSymbolStatus = INDIslaWarningSoundSymbolStatus::OFF;
    INDForwardSafetySymbolStatus mINDForwardSafetySymbolStatus = INDForwardSafetySymbolStatus::OFF;
    INDEmergencySteeringSymbolStatus mINDEmergencySteeringSymbolStatus = INDEmergencySteeringSymbolStatus::OFF;
    INDLaneSafetySymbolStatus mINDLaneSafetySymbolStatus = INDLaneSafetySymbolStatus::OFF;
    INDDriveAttentionWarningSymbolStatus mINDDriveAttentionWarningSymbolStatus = INDDriveAttentionWarningSymbolStatus::OFF;
    InterTelltaleIccDrow mInterTelltaleIccDrow = InterTelltaleIccDrow::OFF;
    IlcuLhActWarnStatus mIlcuLhActWarnStatus = IlcuLhActWarnStatus::OFF;
    IlcuRhActWarnStatus mIlcuRhActWarnStatus = IlcuRhActWarnStatus::OFF;
    INDLfaSymbol1Status mINDLfaSymbol1Status = INDLfaSymbol1Status::OFF;
    INDLfaSymbol2Status mINDLfaSymbol2Status = INDLfaSymbol2Status::OFF;
    INDLcaLeftSymbol1Status mINDLcaLeftSymbol1Status = INDLcaLeftSymbol1Status::OFF;
    INDLcaLeftSymbol2Status mINDLcaLeftSymbol2Status = INDLcaLeftSymbol2Status::OFF;
    INDLcaRightSymbol1Status mINDLcaRightSymbol1Status = INDLcaRightSymbol1Status::OFF;
    INDLcaRightSymbol2Status mINDLcaRightSymbol2Status = INDLcaRightSymbol2Status::OFF;
    INDDriverHandsOnOff1Status mINDDriverHandsOnOff1Status = INDDriverHandsOnOff1Status::OFF;
    INDDriverHandsOnOff2Status mINDDriverHandsOnOff2Status = INDDriverHandsOnOff2Status::OFF;
    IfsIndiReqStatus mIfsIndiReqStatus = IfsIndiReqStatus::OFF;
    INDHbaSymbolStatus mINDHbaSymbolStatus = INDHbaSymbolStatus::OFF;
    INDSetSpeed1Status mINDSetSpeed1Status = INDSetSpeed1Status::OFF;
    INDSetSpeed2Status mINDSetSpeed2Status = INDSetSpeed2Status::OFF;
    HUInt64 mINDSetSpeed1Value = 255;
    HUInt64 mINDSetSpeed2Value = 255;
    HUInt64 mPrevINDSetSpeed1Value = 255;
    HUInt64 mPrevINDSetSpeed2Value = 255;
    INDSccSymbol1Status mINDSccSymbol1Status = INDSccSymbol1Status::OFF;
    INDSccSymbol2Status mINDSccSymbol2Status = INDSccSymbol2Status::OFF;
    INDNsccSymbolStatus mINDNsccSymbolStatus = INDNsccSymbolStatus::OFF;
    BGStausUppBar mBGStausUppBar = BGStausUppBar::OFF;
    BGStausLeftBar mBGStausLeftBar = BGStausLeftBar::OFF;
    BGStausRightBar mBGStausRightBar = BGStausRightBar::OFF;
    NTFGroup7ForwardSafetyFailureStatus mNTFGroup7ForwardSafetyFailureStatus = NTFGroup7ForwardSafetyFailureStatus::OFF;
    NTFGroup7ForwardSideSafetyFailureStatus mNTFGroup7ForwardSideSafetyFailureStatus =
        NTFGroup7ForwardSideSafetyFailureStatus::OFF;
    NTFGroup7LaneSafetyFailureStatus mNTFGroup7LaneSafetyFailureStatus = NTFGroup7LaneSafetyFailureStatus::OFF;
    NTFGroup7BlindSpotSafetyFailure2Status mNTFGroup7BlindSpotSafetyFailure2Status = NTFGroup7BlindSpotSafetyFailure2Status::OFF;
    NTFGroup7IslaFailureStatus mNTFGroup7IslaFailureStatus = NTFGroup7IslaFailureStatus::OFF;
    NTFGroup7DawFailureStatus mNTFGroup7DawFailureStatus = NTFGroup7DawFailureStatus::OFF;
    NTFGroup7HbaFailureStatus mNTFGroup7HbaFailureStatus = NTFGroup7HbaFailureStatus::OFF;
    NTFGroup7SccFailureStatus mNTFGroup7SccFailureStatus = NTFGroup7SccFailureStatus::OFF;
    NTFGroup7LfaFailureStatus mNTFGroup7LfaFailureStatus = NTFGroup7LfaFailureStatus::OFF;
    NTFGroup7HdaFailureStatus mNTFGroup7HdaFailureStatus = NTFGroup7HdaFailureStatus::OFF;
    NTFGroup7HdaPlusFailureStatus mNTFGroup7HdaPlusFailureStatus = NTFGroup7HdaPlusFailureStatus::OFF;
    NTFGroup7LcaFailureStatus mNTFGroup7LcaFailureStatus = NTFGroup7LcaFailureStatus::OFF;
    NTFGroup7MRMFailureStatus mNTFGroup7MRMFailureStatus = NTFGroup7MRMFailureStatus::OFF;
    VCLeftLineStatus mVCLeftLineStatus = VCLeftLineStatus::OFF;
    HUInt64 mVCLeftLineOffsetValue = 0;
    HUInt64 mVCDrivingLaneRadiusValue = 0;
    VCRightLineStatus mVCRightLineStatus = VCRightLineStatus::OFF;
    HUInt64 mVCRightLineOffsetValue = 0;
    VCVehicleDistanceStatus mVCVehicleDistanceStatus = VCVehicleDistanceStatus::OFF;
    VCVehicleDistanceLevelValue mVCVehicleDistanceLevelValue = VCVehicleDistanceLevelValue::OFF;
    HUInt64 mVCVehicleDistanceValue = 0;
    VCCenterRoadSurfaceStatus mVCCenterRoadSurfaceStatus = VCCenterRoadSurfaceStatus::OFF;
    HUInt64 mVCFrontObjectLateralPosition01Value = 0;
    HUInt64 mVCFrontObjectLongitudinalPosition01Value = 0;

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
    VCFrontRadarWaveStatus mVCFrontRadarWaveStatus = VCFrontRadarWaveStatus::OFF;
    VCFrontLeftRadarWaveStatus mVCFrontLeftRadarWaveStatus = VCFrontLeftRadarWaveStatus::OFF;
    VCFrontRightRadarWaveStatus mVCFrontRightRadarWaveStatus = VCFrontRightRadarWaveStatus::OFF;
    VCRearLeftRadarWave1Status mVCRearLeftRadarWave1Status = VCRearLeftRadarWave1Status::OFF;
    VCRearLeftRadarWave2Status mVCRearLeftRadarWave2Status = VCRearLeftRadarWave2Status::OFF;
    VCRearRightRadarWave1Status mVCRearRightRadarWave1Status = VCRearRightRadarWave1Status::OFF;
    VCRearRightRadarWave2Status mVCRearRightRadarWave2Status = VCRearRightRadarWave2Status::OFF;
    VCLaneChangeRectangleStatus mVCLaneChangeRectangleStatus = VCLaneChangeRectangleStatus::OFF;
    HUInt64 mVCLaneChangeRectangleLateralPositionValue = 0;
    VCLeftLeftLineStatus mVCLeftLeftLineStatus = VCLeftLeftLineStatus::OFF;
    HUInt64 mVCLeftLeftLineOffsetValue = 0;
    VCRightRightLineStatus mVCRightRightLineStatus = VCRightRightLineStatus::OFF;
    HUInt64 mVCRightRightLineOffsetValue = 0;
    VCRearLeftObjectMovingDirection2Status mVCRearLeftObjectMovingDirection2Status = VCRearLeftObjectMovingDirection2Status::OFF;
    VCRearRightObjectMovingDirection2Status mVCRearRightObjectMovingDirection2Status =
        VCRearRightObjectMovingDirection2Status::OFF;
    VCLaneChangeAvailableStatus mVCLaneChangeAvailableStatus = VCLaneChangeAvailableStatus::OFF;
    BrakeLampControlCommand_Status mBrakeLampControlCommand_Status = BrakeLampControlCommand_Status::OFF;
    HUInt64 mVCFrontObstacleLateralPositionValue = 0;
    HUInt64 mVCFrontObstacleLongitudinalPositionValue = 0;
    ExtTurnLeftLampStatus mExtTurnLeftLampStatus = ExtTurnLeftLampStatus::OFF;
    ExtTurnRightLampStatus mExtTurnRightLampStatus = ExtTurnRightLampStatus::OFF;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    ExtHighBeamOnReqStatus mExtHighBeamOnReqStatus = ExtHighBeamOnReqStatus::OFF;
    VCFrontLeftObjectMovingDirectionStatus mVCFrontLeftObjectMovingDirectionStatus = VCFrontLeftObjectMovingDirectionStatus::OFF;
    VCFrontRightObjectMovingDirectionStatus mVCFrontRightObjectMovingDirectionStatus =
        VCFrontRightObjectMovingDirectionStatus::OFF;
    VCEmergencyStreeingStatus mVCEmergencyStreeingStatus = VCEmergencyStreeingStatus::OFF;
    VCAdditionalStatus mVCAdditionalStatus = VCAdditionalStatus::OFF;
    VCLeftLaneChangeDirectionStatus mVCLeftLaneChangeDirectionStatus = VCLeftLaneChangeDirectionStatus::OFF;
    VCRightLaneChangeDirectionStatus mVCRightLaneChangeDirectionStatus = VCRightLaneChangeDirectionStatus::OFF;
    INDIslaSpeedLimitTrafficSignStatus mINDIslaSpeedLimitTrafficSignStatus = INDIslaSpeedLimitTrafficSignStatus::OFF;
    INDIslaTrafficSignCountryInformationStatus mINDIslaTrafficSignCountryInformationStatus =
        INDIslaTrafficSignCountryInformationStatus::DEFAULT;
    HUInt64 mINDIslaSpeedLimitTrafficSignValue = 0;
    INDIslaAdditionalTrafficSignStatus mINDIslaAdditionalTrafficSignStatus = INDIslaAdditionalTrafficSignStatus::OFF;
    INDIslaSupplementaryTrafficSignStatus mINDIslaSupplementaryTrafficSignStatus = INDIslaSupplementaryTrafficSignStatus::OFF;
    INDIslaNextSpeedLimitTrafficSignStatus mINDIslaNextSpeedLimitTrafficSignStatus = INDIslaNextSpeedLimitTrafficSignStatus::OFF;
    HUInt64 mINDIslaNextSpeedLimitTrafficSignValue = 0;
    HUInt64 mINDIslaNextSupportTrafficSignValue = 0;
    INDIslaSupplementaryTrafficSign2Status mINDIslaSupplementaryTrafficSign2Status = INDIslaSupplementaryTrafficSign2Status::OFF;
    NTFGroup1AdasWarning1_1Status mNTFGroup1AdasWarning1_1Status = NTFGroup1AdasWarning1_1Status::OFF;
    NTFGroup1AdasWarning1_2Status mNTFGroup1AdasWarning1_2Status = NTFGroup1AdasWarning1_2Status::OFF;
    NTFGroup1AdasWarning2_1Status mNTFGroup1AdasWarning2_1Status = NTFGroup1AdasWarning2_1Status::OFF;
    NTFGroup4AdasWarning1_1Status mNTFGroup4AdasWarning1_1Status = NTFGroup4AdasWarning1_1Status::OFF;
    NTFGroup7OutsideMirrorSymbolFailure2Status mNTFGroup7OutsideMirrorSymbolFailure2Status =
        NTFGroup7OutsideMirrorSymbolFailure2Status::OFF;
    NTFGroup7DriverAssistFailure1Status mNTFGroup7DriverAssistFailure1Status = NTFGroup7DriverAssistFailure1Status::OFF;
    NTFGroup7DriverAssistFailure2Status mNTFGroup7DriverAssistFailure2Status = NTFGroup7DriverAssistFailure2Status::OFF;
    NTMGroup2AdasWarning1_1Status mNTMGroup2AdasWarning1_1Status = NTMGroup2AdasWarning1_1Status::OFF;
    NTMGroup2AdasWarning1_2Status mNTMGroup2AdasWarning1_2Status = NTMGroup2AdasWarning1_2Status::OFF;
    NTFHandsOffStatus mNTFHandsOffStatus = NTFHandsOffStatus::OFF;
    SoundWarning1_1Status mSoundWarning1_1Status = SoundWarning1_1Status::OFF;
    SoundWarning1_2Status mSoundWarning1_2Status = SoundWarning1_2Status::OFF;
    SoundWarning1_3Status mSoundWarning1_3Status = SoundWarning1_3Status::OFF;
    SoundWarning1_4Status mSoundWarning1_4Status = SoundWarning1_4Status::OFF;
    SoundWarning1_5Status mSoundWarning1_5Status = SoundWarning1_5Status::OFF;
    SoundWarning2_1Status mSoundWarning2_1Status = SoundWarning2_1Status::OFF;
    IslaOption2Status mIslaOption2Status = IslaOption2Status::OFF;
    AdasOverSpeedWarnOnOffStatus mAdasOverSpeedWarnOnOffStatus = AdasOverSpeedWarnOnOffStatus::NON_OPTION;
    Private_ISLAAddtnlSign mPrivate_ISLAAddtnlSign = Private_ISLAAddtnlSign::NONE;

    SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat mSummaryADASFrontVehicleStat =
        SFCADAS_DRIVING_2_0ConstantADASFrontVehicleStat::OFF;
    SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat mSummaryADASSetSpeedColorStat =
        SFCADAS_DRIVING_2_0ConstantADASSetSpeedColorStat::OFF;

    SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat mISLAMainSpeedStat = SFCADAS_DRIVING_2_0ConstantISLAMainSpeedStat::OFF;

    // mSurroundingObjectColorInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: VCSurroundingObject01Status, ..., VCSurroundingObject15Status
    // Type1(for Type0's prev values): VCSurroundingObject01Status, ..., VCSurroundingObject15Status
    // Type2: std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor15Stat&)>
    std::tuple<
        std::tuple<std::reference_wrapper<VCSurroundingObject01Status>, std::reference_wrapper<VCSurroundingObject01Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor01Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject02Status>, std::reference_wrapper<VCSurroundingObject02Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor02Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject03Status>, std::reference_wrapper<VCSurroundingObject03Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor03Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject04Status>, std::reference_wrapper<VCSurroundingObject04Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor04Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject05Status>, std::reference_wrapper<VCSurroundingObject05Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor05Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject06Status>, std::reference_wrapper<VCSurroundingObject06Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor06Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject07Status>, std::reference_wrapper<VCSurroundingObject07Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor07Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject08Status>, std::reference_wrapper<VCSurroundingObject08Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor08Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject09Status>, std::reference_wrapper<VCSurroundingObject09Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor09Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject10Status>, std::reference_wrapper<VCSurroundingObject10Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor10Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject11Status>, std::reference_wrapper<VCSurroundingObject11Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor11Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject12Status>, std::reference_wrapper<VCSurroundingObject12Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor12Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject13Status>, std::reference_wrapper<VCSurroundingObject13Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor13Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject14Status>, std::reference_wrapper<VCSurroundingObject14Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor14Stat&)>>,
        std::tuple<std::reference_wrapper<VCSurroundingObject15Status>, std::reference_wrapper<VCSurroundingObject15Status>,
                   std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor15Stat&)>>>
        mSurroundingObjectColorInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mVCSurroundingObject01Status), std::ref(mPrevVCSurroundingObject01Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor01Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject02Status), std::ref(mPrevVCSurroundingObject02Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor02Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject03Status), std::ref(mPrevVCSurroundingObject03Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor03Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject04Status), std::ref(mPrevVCSurroundingObject04Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor04Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject05Status), std::ref(mPrevVCSurroundingObject05Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor05Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject06Status), std::ref(mPrevVCSurroundingObject06Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor06Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject07Status), std::ref(mPrevVCSurroundingObject07Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor07Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject08Status), std::ref(mPrevVCSurroundingObject08Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor08Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject09Status), std::ref(mPrevVCSurroundingObject09Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor09Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject10Status), std::ref(mPrevVCSurroundingObject10Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor10Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject11Status), std::ref(mPrevVCSurroundingObject11Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor11Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject12Status), std::ref(mPrevVCSurroundingObject12Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor12Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject13Status), std::ref(mPrevVCSurroundingObject13Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor13Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject14Status), std::ref(mPrevVCSurroundingObject14Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor14Stat,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObject15Status), std::ref(mPrevVCSurroundingObject15Status),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectColor15Stat,
                                      this, std::placeholders::_1)));

    // mSurroundingObjectClassInfoTuple = std::tuple<std::pair<Type0, Type1>, ... >
    // Type0: VCSurroundingObjectClassification01Status, ..., VCSurroundingObjectClassification15Status
    // Type1: std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass15Stat&)>
    std::tuple<std::pair<std::reference_wrapper<VCSurroundingObjectClassification01Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass01Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification02Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass02Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification03Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass03Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification04Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass04Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification05Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass05Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification06Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass06Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification07Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass07Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification08Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass08Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification09Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass09Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification10Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass10Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification11Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass11Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification12Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass12Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification13Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass13Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification14Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass14Stat&)>>,
               std::pair<std::reference_wrapper<VCSurroundingObjectClassification15Status>,
                         std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass15Stat&)>>>
        mSurroundingObjectClassInfoTuple = std::make_tuple(
            std::make_pair(std::ref(mVCSurroundingObjectClassification01Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass01Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification02Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass02Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification03Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass03Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification04Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass04Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification05Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass05Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification06Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass06Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification07Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass07Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification08Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass08Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification09Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass09Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification10Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass10Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification11Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass11Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification12Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass12Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification13Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass13Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification14Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass14Stat,
                                     this, std::placeholders::_1)),
            std::make_pair(std::ref(mVCSurroundingObjectClassification15Status),
                           std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectClass15Stat,
                                     this, std::placeholders::_1)));

    // mSurroundingObjectLatInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat02Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat03Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat04Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat05Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat06Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat07Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat08Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat09Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat10Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat11Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat12Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat13Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat14Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Stat&)>,
                          std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLatInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition01Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat01Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition02Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat02Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat02Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition03Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat03Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat03Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition04Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat04Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat04Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition05Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat05Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat05Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition06Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat06Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat06Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition07Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat07Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat07Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition08Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat08Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat08Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition09Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat09Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat09Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition10Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat10Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat10Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition11Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat11Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat11Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition12Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat12Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat12Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition13Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat13Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat13Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition14Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat14Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat14Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLateralPosition15Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLat15Value,
                                      this, std::placeholders::_1)));

    // mSurroundingObjectLongInfoTuple = std::tuple<std::tuple<Type0, Type1, Type2>, ... >
    // Type0: std::reference_wrapper<HUInt64>
    // Type1: std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Stat&)>, ...,
    //        std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Stat&)>
    // Type2: std::function<std::string(const HInt64&)>
    std::tuple<std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong02Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong03Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong04Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong05Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong06Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong07Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong08Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong09Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong10Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong11Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong12Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong13Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong14Stat&)>,
                          std::function<std::string(const HInt64&)>>,
               std::tuple<std::reference_wrapper<HUInt64>,
                          std::function<std::string(const SFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Stat&)>,
                          std::function<std::string(const HInt64&)>>>
        mSurroundingObjectLongInfoTuple = std::make_tuple(
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition01Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong01Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition02Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong02Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong02Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition03Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong03Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong03Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition04Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong04Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong04Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition05Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong05Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong05Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition06Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong06Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong06Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition07Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong07Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong07Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition08Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong08Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong08Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition09Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong09Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong09Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition10Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong10Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong10Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition11Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong11Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong11Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition12Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong12Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong12Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition13Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong13Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong13Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition14Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong14Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong14Value,
                                      this, std::placeholders::_1)),
            std::make_tuple(std::ref(mVCSurroundingObjectLongitudinalPosition15Value),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Stat,
                                      this, std::placeholders::_1),
                            std::bind(&ADAS_DRIVING_2_0Base::setSFCADAS_DRIVING_2_0ConstantADASViewSurroundingObjectLong15Value,
                                      this, std::placeholders::_1)));
};

}  // namespace ccos

#endif  // SFSS_ADAS_DRIVING_2_0_H
