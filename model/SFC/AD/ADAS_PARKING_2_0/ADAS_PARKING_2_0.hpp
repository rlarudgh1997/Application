/**
 * @file ADAS_PARKING_2_0.hpp
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
#ifndef SFSS_ADAS_PARKING_2_0_H
#define SFSS_ADAS_PARKING_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "ADAS_PARKING_2_0Base.hpp"

namespace ccos {

// SFC Version : 19.0.0
// Reference : [AD091] ADAS_PARKING_2_0. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_PARKING_2_0 : public ADAS_PARKING_2_0Base {
public:
    ADAS_PARKING_2_0() = default;
    ~ADAS_PARKING_2_0() override = default;
    ADAS_PARKING_2_0(const ADAS_PARKING_2_0& other) = delete;
    ADAS_PARKING_2_0(ADAS_PARKING_2_0&& other) noexcept = delete;
    ADAS_PARKING_2_0& operator=(const ADAS_PARKING_2_0& other) = delete;
    ADAS_PARKING_2_0& operator=(ADAS_PARKING_2_0&& other) noexcept = delete;

    void onInitialize() override {
        setSFCADAS_PARKING_2_0EventGroup7Failure10LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_2_0EventGroup7Failure10LinkedSoundType(
            SFCADAS_PARKING_2_0EventGroup7Failure10LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0EventGroup7Failure10LinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_2_0EventGroup7Failure11LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_2_0EventGroup7Failure11LinkedSoundType(
            SFCADAS_PARKING_2_0EventGroup7Failure11LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0EventGroup7Failure11LinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_2_0EventGroup7CCWFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_2_0EventGroup7CCWFailureLinkedSoundType(
            SFCADAS_PARKING_2_0EventGroup7CCWFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0EventGroup7CCWFailureLinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_2_0EventGroup7TimeoutID("E53016");
        setSFCADAS_PARKING_2_0EventGroup7TimeoutLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_2_0EventGroup7TimeoutLinkedSoundType(
            SFCADAS_PARKING_2_0EventGroup7TimeoutLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0EventGroup7TimeoutLinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_2_0SoundWarningLevelType(SFCADAS_PARKING_2_0SoundWarningLevelType::INFINITE);
        setSFCADAS_PARKING_2_0SoundPASysWarnSoundType(SFCADAS_PARKING_2_0SoundPASysWarnSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0SoundPASysWarnSoundRepeatCount(1);
        setSFCADAS_PARKING_2_0SoundWarningSensorFailID("SND_ADAS_PDWSensorFail_Fixed");
        setSFCADAS_PARKING_2_0SoundWarningSensorFailType(SFCADAS_PARKING_2_0SoundWarningSensorFailType::REPEAT_COUNT);
        setSFCADAS_PARKING_2_0SoundWarningSensorFailRepeatCount(1);
        setSFCADAS_PARKING_2_0SoundWarningTailgateOpenedID("SND_ADAS_PDWTailgate_Opened");
        setSFCADAS_PARKING_2_0SoundWarningTailgateOpenedType(SFCADAS_PARKING_2_0SoundWarningTailgateOpenedType::INTERVAL);
        setSFCADAS_PARKING_2_0SoundWarningTailgateOpenedInterval(kInterval_1000ms);
        setSFCADAS_PARKING_2_0EventPdwSystemFailureID("NT51013");
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateInterRspaToggleInformationStatus();
        updateInterRspaParkingDirectionInformationStatus();
        updateInterRspaParkingSlotLocationStatus();
        updateInterRspaOperationInformationStatus();
        updateRSPAParkingDirectionStat();
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAll();
    }

    void onCarBodyDisplayStatusChanged(const ArgumentsCarBodyDisplayStatusChanged& args) {
        mVCTopviewControlStatus = args.mInput_VCTopviewControlStatus;
        updateCarBody();
    }

    void onPdwSoundMuteStatusChanged(const ArgumentsPdwSoundMuteStatusChanged& args) {
        mAdasPdwWarningSoundMuteStatus = args.mInput_AdasPdwWarningSoundMuteStatus;
        mIbuPdwWarningSoundMuteStatus = args.mInput_IbuPdwWarningSoundMuteStatus;
        updatePdwSoundMute();
    }

    void onParkFrontMasterWarningStatusChanged(const ArgumentsParkFrontMasterWarningStatusChanged& args) {
        mAdasFrontMasterWarningDisplayStatus = args.mInput_AdasFrontMasterWarningDisplayStatus;
        mIbuFrontMasterWarningDisplayStatus = args.mInput_IbuFrontMasterWarningDisplayStatus;
        updateParkFrontMasterWarning();
    }

    void onParkRearMasterWarningStatusChanged(const ArgumentsParkRearMasterWarningStatusChanged& args) {
        mAdasRearMasterWarningDisplayStatus = args.mInput_AdasRearMasterWarningDisplayStatus;
        mIbuRearMasterWarningDisplayStatus = args.mInput_IbuRearMasterWarningDisplayStatus;
        updateParkRearMasterWarning();
    }

    void onParkLeftMasterWarningStatusChanged(const ArgumentsParkLeftMasterWarningStatusChanged& args) {
        mAdasLeftMasterWarningDisplayStatus = args.mInput_AdasLeftMasterWarningDisplayStatus;
        updateParkLeftMasterWarning();
    }

    void onParkRightMasterWarningStatusChanged(const ArgumentsParkRightMasterWarningStatusChanged& args) {
        mAdasRightMasterWarningDisplayStatus = args.mInput_AdasRightMasterWarningDisplayStatus;
        updateParkRightMasterWarning();
    }

    void onAdasPdwFrontContourLineLevel01ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel01ValueChanged& args) {
        mAdasPdwFrontContourLineLevel01Value = args.mInput_AdasPdwFrontContourLineLevel01Value;
        updateConstantFrontContourLevel01Stat();
    }

    void onAdasPdwFrontContourLineLevel02ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel02ValueChanged& args) {
        mAdasPdwFrontContourLineLevel02Value = args.mInput_AdasPdwFrontContourLineLevel02Value;
        updateConstantFrontContourLevel02Stat();
    }

    void onAdasPdwFrontContourLineLevel03ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel03ValueChanged& args) {
        mAdasPdwFrontContourLineLevel03Value = args.mInput_AdasPdwFrontContourLineLevel03Value;
        updateConstantFrontContourLevel03Stat();
    }

    void onAdasPdwFrontContourLineLevel04ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel04ValueChanged& args) {
        mAdasPdwFrontContourLineLevel04Value = args.mInput_AdasPdwFrontContourLineLevel04Value;
        updateConstantFrontContourLevel04Stat();
    }

    void onAdasPdwFrontContourLineLevel05ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel05ValueChanged& args) {
        mAdasPdwFrontContourLineLevel05Value = args.mInput_AdasPdwFrontContourLineLevel05Value;
        updateConstantFrontContourLevel05Stat();
    }

    void onAdasPdwFrontContourLineLevel06ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel06ValueChanged& args) {
        mAdasPdwFrontContourLineLevel06Value = args.mInput_AdasPdwFrontContourLineLevel06Value;
        updateConstantFrontContourLevel06Stat();
    }

    void onAdasPdwFrontContourLineLevel07ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel07ValueChanged& args) {
        mAdasPdwFrontContourLineLevel07Value = args.mInput_AdasPdwFrontContourLineLevel07Value;
        updateConstantFrontContourLevel07Stat();
    }

    void onAdasPdwFrontContourLineLevel08ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel08ValueChanged& args) {
        mAdasPdwFrontContourLineLevel08Value = args.mInput_AdasPdwFrontContourLineLevel08Value;
        updateConstantFrontContourLevel08Stat();
    }

    void onAdasPdwFrontContourLineLevel09ValueChanged(const ArgumentsAdasPdwFrontContourLineLevel09ValueChanged& args) {
        mAdasPdwFrontContourLineLevel09Value = args.mInput_AdasPdwFrontContourLineLevel09Value;
        updateConstantFrontContourLevel09Stat();
    }

    void onIbuPdwFrontContourLineLevel01ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel01ValueChanged& args) {
        mIbuPdwFrontContourLineLevel01Value = args.mInput_IbuPdwFrontContourLineLevel01Value;
        updateConstantFrontContourLevel01Stat();
    }

    void onIbuPdwFrontContourLineLevel02ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel02ValueChanged& args) {
        mIbuPdwFrontContourLineLevel02Value = args.mInput_IbuPdwFrontContourLineLevel02Value;
        updateConstantFrontContourLevel02Stat();
    }

    void onIbuPdwFrontContourLineLevel03ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel03ValueChanged& args) {
        mIbuPdwFrontContourLineLevel03Value = args.mInput_IbuPdwFrontContourLineLevel03Value;
        updateConstantFrontContourLevel03Stat();
    }

    void onIbuPdwFrontContourLineLevel04ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel04ValueChanged& args) {
        mIbuPdwFrontContourLineLevel04Value = args.mInput_IbuPdwFrontContourLineLevel04Value;
        updateConstantFrontContourLevel04Stat();
    }

    void onIbuPdwFrontContourLineLevel05ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel05ValueChanged& args) {
        mIbuPdwFrontContourLineLevel05Value = args.mInput_IbuPdwFrontContourLineLevel05Value;
        updateConstantFrontContourLevel05Stat();
    }

    void onIbuPdwFrontContourLineLevel06ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel06ValueChanged& args) {
        mIbuPdwFrontContourLineLevel06Value = args.mInput_IbuPdwFrontContourLineLevel06Value;
        updateConstantFrontContourLevel06Stat();
    }

    void onIbuPdwFrontContourLineLevel07ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel07ValueChanged& args) {
        mIbuPdwFrontContourLineLevel07Value = args.mInput_IbuPdwFrontContourLineLevel07Value;
        updateConstantFrontContourLevel07Stat();
    }

    void onIbuPdwFrontContourLineLevel08ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel08ValueChanged& args) {
        mIbuPdwFrontContourLineLevel08Value = args.mInput_IbuPdwFrontContourLineLevel08Value;
        updateConstantFrontContourLevel08Stat();
    }

    void onIbuPdwFrontContourLineLevel09ValueChanged(const ArgumentsIbuPdwFrontContourLineLevel09ValueChanged& args) {
        mIbuPdwFrontContourLineLevel09Value = args.mInput_IbuPdwFrontContourLineLevel09Value;
        updateConstantFrontContourLevel09Stat();
    }

    void onAdasPdwRearContourLineLevel01ValueChanged(const ArgumentsAdasPdwRearContourLineLevel01ValueChanged& args) {
        mAdasPdwRearContourLineLevel01Value = args.mInput_AdasPdwRearContourLineLevel01Value;
        updateConstantRearContourLevel01Stat();
    }

    void onAdasPdwRearContourLineLevel02ValueChanged(const ArgumentsAdasPdwRearContourLineLevel02ValueChanged& args) {
        mAdasPdwRearContourLineLevel02Value = args.mInput_AdasPdwRearContourLineLevel02Value;
        updateConstantRearContourLevel02Stat();
    }

    void onAdasPdwRearContourLineLevel03ValueChanged(const ArgumentsAdasPdwRearContourLineLevel03ValueChanged& args) {
        mAdasPdwRearContourLineLevel03Value = args.mInput_AdasPdwRearContourLineLevel03Value;
        updateConstantRearContourLevel03Stat();
    }

    void onAdasPdwRearContourLineLevel04ValueChanged(const ArgumentsAdasPdwRearContourLineLevel04ValueChanged& args) {
        mAdasPdwRearContourLineLevel04Value = args.mInput_AdasPdwRearContourLineLevel04Value;
        updateConstantRearContourLevel04Stat();
    }

    void onAdasPdwRearContourLineLevel05ValueChanged(const ArgumentsAdasPdwRearContourLineLevel05ValueChanged& args) {
        mAdasPdwRearContourLineLevel05Value = args.mInput_AdasPdwRearContourLineLevel05Value;
        updateConstantRearContourLevel05Stat();
    }

    void onAdasPdwRearContourLineLevel06ValueChanged(const ArgumentsAdasPdwRearContourLineLevel06ValueChanged& args) {
        mAdasPdwRearContourLineLevel06Value = args.mInput_AdasPdwRearContourLineLevel06Value;
        updateConstantRearContourLevel06Stat();
    }

    void onAdasPdwRearContourLineLevel07ValueChanged(const ArgumentsAdasPdwRearContourLineLevel07ValueChanged& args) {
        mAdasPdwRearContourLineLevel07Value = args.mInput_AdasPdwRearContourLineLevel07Value;
        updateConstantRearContourLevel07Stat();
    }

    void onAdasPdwRearContourLineLevel08ValueChanged(const ArgumentsAdasPdwRearContourLineLevel08ValueChanged& args) {
        mAdasPdwRearContourLineLevel08Value = args.mInput_AdasPdwRearContourLineLevel08Value;
        updateConstantRearContourLevel08Stat();
    }

    void onAdasPdwRearContourLineLevel09ValueChanged(const ArgumentsAdasPdwRearContourLineLevel09ValueChanged& args) {
        mAdasPdwRearContourLineLevel09Value = args.mInput_AdasPdwRearContourLineLevel09Value;
        updateConstantRearContourLevel09Stat();
    }

    void onIbuPdwRearContourLineLevel01ValueChanged(const ArgumentsIbuPdwRearContourLineLevel01ValueChanged& args) {
        mIbuPdwRearContourLineLevel01Value = args.mInput_IbuPdwRearContourLineLevel01Value;
        updateConstantRearContourLevel01Stat();
    }

    void onIbuPdwRearContourLineLevel02ValueChanged(const ArgumentsIbuPdwRearContourLineLevel02ValueChanged& args) {
        mIbuPdwRearContourLineLevel02Value = args.mInput_IbuPdwRearContourLineLevel02Value;
        updateConstantRearContourLevel02Stat();
    }

    void onIbuPdwRearContourLineLevel03ValueChanged(const ArgumentsIbuPdwRearContourLineLevel03ValueChanged& args) {
        mIbuPdwRearContourLineLevel03Value = args.mInput_IbuPdwRearContourLineLevel03Value;
        updateConstantRearContourLevel03Stat();
    }

    void onIbuPdwRearContourLineLevel04ValueChanged(const ArgumentsIbuPdwRearContourLineLevel04ValueChanged& args) {
        mIbuPdwRearContourLineLevel04Value = args.mInput_IbuPdwRearContourLineLevel04Value;
        updateConstantRearContourLevel04Stat();
    }

    void onIbuPdwRearContourLineLevel05ValueChanged(const ArgumentsIbuPdwRearContourLineLevel05ValueChanged& args) {
        mIbuPdwRearContourLineLevel05Value = args.mInput_IbuPdwRearContourLineLevel05Value;
        updateConstantRearContourLevel05Stat();
    }

    void onIbuPdwRearContourLineLevel06ValueChanged(const ArgumentsIbuPdwRearContourLineLevel06ValueChanged& args) {
        mIbuPdwRearContourLineLevel06Value = args.mInput_IbuPdwRearContourLineLevel06Value;
        updateConstantRearContourLevel06Stat();
    }

    void onIbuPdwRearContourLineLevel07ValueChanged(const ArgumentsIbuPdwRearContourLineLevel07ValueChanged& args) {
        mIbuPdwRearContourLineLevel07Value = args.mInput_IbuPdwRearContourLineLevel07Value;
        updateConstantRearContourLevel07Stat();
    }

    void onIbuPdwRearContourLineLevel08ValueChanged(const ArgumentsIbuPdwRearContourLineLevel08ValueChanged& args) {
        mIbuPdwRearContourLineLevel08Value = args.mInput_IbuPdwRearContourLineLevel08Value;
        updateConstantRearContourLevel08Stat();
    }

    void onIbuPdwRearContourLineLevel09ValueChanged(const ArgumentsIbuPdwRearContourLineLevel09ValueChanged& args) {
        mIbuPdwRearContourLineLevel09Value = args.mInput_IbuPdwRearContourLineLevel09Value;
        updateConstantRearContourLevel09Stat();
    }

    void onAdasPdwLeftContourLineLevel01ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel01ValueChanged& args) {
        mAdasPdwLeftContourLineLevel01Value = args.mInput_AdasPdwLeftContourLineLevel01Value;
        updateConstantLeftContourLevel01Stat();
    }

    void onAdasPdwLeftContourLineLevel02ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel02ValueChanged& args) {
        mAdasPdwLeftContourLineLevel02Value = args.mInput_AdasPdwLeftContourLineLevel02Value;
        updateConstantLeftContourLevel02Stat();
    }

    void onAdasPdwLeftContourLineLevel03ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel03ValueChanged& args) {
        mAdasPdwLeftContourLineLevel03Value = args.mInput_AdasPdwLeftContourLineLevel03Value;
        updateConstantLeftContourLevel03Stat();
    }

    void onAdasPdwLeftContourLineLevel04ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel04ValueChanged& args) {
        mAdasPdwLeftContourLineLevel04Value = args.mInput_AdasPdwLeftContourLineLevel04Value;
        updateConstantLeftContourLevel04Stat();
    }

    void onAdasPdwLeftContourLineLevel05ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel05ValueChanged& args) {
        mAdasPdwLeftContourLineLevel05Value = args.mInput_AdasPdwLeftContourLineLevel05Value;
        updateConstantLeftContourLevel05Stat();
    }

    void onAdasPdwLeftContourLineLevel06ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel06ValueChanged& args) {
        mAdasPdwLeftContourLineLevel06Value = args.mInput_AdasPdwLeftContourLineLevel06Value;
        updateConstantLeftContourLevel06Stat();
    }

    void onAdasPdwLeftContourLineLevel07ValueChanged(const ArgumentsAdasPdwLeftContourLineLevel07ValueChanged& args) {
        mAdasPdwLeftContourLineLevel07Value = args.mInput_AdasPdwLeftContourLineLevel07Value;
        updateConstantLeftContourLevel07Stat();
    }

    void onAdasPdwRightContourLineLevel01ValueChanged(const ArgumentsAdasPdwRightContourLineLevel01ValueChanged& args) {
        mAdasPdwRightContourLineLevel01Value = args.mInput_AdasPdwRightContourLineLevel01Value;
        updateConstantRightContourLevel01Stat();
    }

    void onAdasPdwRightContourLineLevel02ValueChanged(const ArgumentsAdasPdwRightContourLineLevel02ValueChanged& args) {
        mAdasPdwRightContourLineLevel02Value = args.mInput_AdasPdwRightContourLineLevel02Value;
        updateConstantRightContourLevel02Stat();
    }

    void onAdasPdwRightContourLineLevel03ValueChanged(const ArgumentsAdasPdwRightContourLineLevel03ValueChanged& args) {
        mAdasPdwRightContourLineLevel03Value = args.mInput_AdasPdwRightContourLineLevel03Value;
        updateConstantRightContourLevel03Stat();
    }

    void onAdasPdwRightContourLineLevel04ValueChanged(const ArgumentsAdasPdwRightContourLineLevel04ValueChanged& args) {
        mAdasPdwRightContourLineLevel04Value = args.mInput_AdasPdwRightContourLineLevel04Value;
        updateConstantRightContourLevel04Stat();
    }

    void onAdasPdwRightContourLineLevel05ValueChanged(const ArgumentsAdasPdwRightContourLineLevel05ValueChanged& args) {
        mAdasPdwRightContourLineLevel05Value = args.mInput_AdasPdwRightContourLineLevel05Value;
        updateConstantRightContourLevel05Stat();
    }

    void onAdasPdwRightContourLineLevel06ValueChanged(const ArgumentsAdasPdwRightContourLineLevel06ValueChanged& args) {
        mAdasPdwRightContourLineLevel06Value = args.mInput_AdasPdwRightContourLineLevel06Value;
        updateConstantRightContourLevel06Stat();
    }

    void onAdasPdwRightContourLineLevel07ValueChanged(const ArgumentsAdasPdwRightContourLineLevel07ValueChanged& args) {
        mAdasPdwRightContourLineLevel07Value = args.mInput_AdasPdwRightContourLineLevel07Value;
        updateConstantRightContourLevel07Stat();
    }

    void onAdasPdwSystemFailureStatusChanged(const ArgumentsAdasPdwSystemFailureStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_AdasPdwSystemFailureStatus) == true) {
            mAdasPdwSystemFailureStatus = AdasPdwSystemFailureStatus::UNHANDLED_TIMEOUT;
        } else {
            mAdasPdwSystemFailureStatus = args.mInput_AdasPdwSystemFailureStatus;
        }
        updateEventAdasPdwSystemFailure();
        updateEventPdwSystemFailure();
    }

    void onIbuPdwSystemFailureStatusChanged(const ArgumentsIbuPdwSystemFailureStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_IbuPdwSystemFailureStatus) == true) {
            mIbuPdwSystemFailureStatus = IbuPdwSystemFailureStatus::UNHANDLED_TIMEOUT;
        } else {
            mIbuPdwSystemFailureStatus = args.mInput_IbuPdwSystemFailureStatus;
        }
        updateEventIbuPdwSystemFailure();
        updateEventPdwSystemFailure();
    }

    void onAdasPcaSystemFailureStatusChanged(const ArgumentsAdasPcaSystemFailureStatusChanged& args) {
        mAdasPcaSystemFailureStatus = args.mInput_AdasPcaSystemFailureStatus;
        updateEventAdasPcaSystemFailure();
    }

    void onRccaFailure2StatusChanged(const ArgumentsRccaFailure2StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_NTFGroup7RccaFailure2Status) == true) {
            mNTFGroup7RccaFailure2Status = NTFGroup7RccaFailure2Status::UNHANDLED_TIMEOUT;
        } else {
            mNTFGroup7RccaFailure2Status = args.mInput_NTFGroup7RccaFailure2Status;
        }
        updateEventRccaFailure();
    }

    void onExitSafetyFailure2StatusChanged(const ArgumentsExitSafetyFailure2StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_NTFGroup7ExitSafetyFailure2Status) == true) {
            mNTFGroup7ExitSafetyFailure2Status = NTFGroup7ExitSafetyFailure2Status::UNHANDLED_TIMEOUT;
        } else {
            mNTFGroup7ExitSafetyFailure2Status = args.mInput_NTFGroup7ExitSafetyFailure2Status;
        }
        updateEventExitSafetyFailure();
    }

    void onVcTopviewScaleStatusChanged(const ArgumentsVcTopviewScaleStatusChanged& args) {
        mVCTopviewScaleStatus = args.mInput_VCTopviewScaleStatus;
        updateConstantTopViewScale();
    }

    void onMraButtunDisplayStatusChanged(const ArgumentsMraButtunDisplayStatusChanged& args) {
        mMraButtunDisplayStatus = args.mInput_MraButtunDisplayStatus;
        updateConstantMRAButton();
    }

    void onRspaButtunDisplayStatusChanged(const ArgumentsRspaButtunDisplayStatusChanged& args) {
        mRspaButtunDisplayStatus = args.mInput_RspaButtunDisplayStatus;
        updateConstantRSPAEntryButton();
    }

    void onVcRspaFunctionDisplayStatusChanged(const ArgumentsVcRspaFunctionDisplayStatusChanged& args) {
        mVCRspaFunctionDisplayStatus = args.mInput_VCRspaFunctionDisplayStatus;
        updateConstantRSPAEntryDirection();
    }

    void onPcaFrontLeftWarningDisplayStatusChanged(const ArgumentsPcaFrontLeftWarningDisplayStatusChanged& args) {
        mPcaFrontLeftWarningDisplayStatus = args.mInput_PcaFrontLeftWarningDisplayStatus;
        updateParkFrontLeftMasterWarning();
    }

    void onPcaFrontRightWarningDisplayStatusChanged(const ArgumentsPcaFrontRightWarningDisplayStatusChanged& args) {
        mPcaFrontRightWarningDisplayStatus = args.mInput_PcaFrontRightWarningDisplayStatus;
        updateParkFrontRightMasterWarning();
    }

    void onPcaRearLeftWarningDisplayStatusChanged(const ArgumentsPcaRearLeftWarningDisplayStatusChanged& args) {
        mPcaRearLeftWarningDisplayStatus = args.mInput_PcaRearLeftWarningDisplayStatus;
        updateParkRearLeftMasterWarning();
    }

    void onPcaRearRightWarningDisplayStatusChanged(const ArgumentsPcaRearRightWarningDisplayStatusChanged& args) {
        mPcaRearRightWarningDisplayStatus = args.mInput_PcaRearRightWarningDisplayStatus;
        updateParkRearRightMasterWarning();
    }

    void onAdasParkingControlTelltaleStatusChanged(const ArgumentsAdasParkingControlTelltaleStatusChanged& args) {
        mINDAdasParkingControlSymbolStatus = args.mInput_INDAdasParkingControlSymbolStatus;
        mINDAdasParkingProgressBarValue = args.mInput_INDAdasParkingProgressBarValue;
        updateAdasParkingControlTelltale();
    }

    void onEvent7GroupFailure10StatusChanged(const ArgumentsEvent7GroupFailure10StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure10Status) == true) {
            mPuFGroup7DriverAssistFailure10Status = PuFGroup7DriverAssistFailure10Status::UNHANDLED_TIMEOUT;
        } else {
            mPuFGroup7DriverAssistFailure10Status = args.mInput_PuFGroup7DriverAssistFailure10Status;
        }
        updateEvent7GroupFailure10();
        updateEvent7GroupTimeout();
    }

    void onEvent7GroupFailure11StatusChanged(const ArgumentsEvent7GroupFailure11StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure11Status) == true) {
            mPuFGroup7DriverAssistFailure11Status = PuFGroup7DriverAssistFailure11Status::UNHANDLED_TIMEOUT;
        } else {
            mPuFGroup7DriverAssistFailure11Status = args.mInput_PuFGroup7DriverAssistFailure11Status;
        }
        updateEvent7GroupFailure11();
        updateEvent7GroupTimeout();
    }

    void onEvent7GroupCCWFailureStatusChanged(const ArgumentsEvent7GroupCCWFailureStatusChanged& args) {
        mCCWFailureStatus = args.mInput_CCWFailureStatus;
        updateEvent7GroupCCWFailure();
    }

    void onEvent2GroupChanged(const ArgumentsEvent2GroupChanged& args) {
        mPcaModeDisplayStatus = args.mInput_PcaModeDisplayStatus;
        updateEvent2Group();
    }

    void onClusterHMIDisplayStatusChanged(const ArgumentsClusterHMIDisplayStatusChanged& args) {
        mClusterHMIDisplayStatus = args.mInput_ClusterHMIDisplayStatus;
        updateEvent4GroupMRA();
    }

    void onRspaEntryAvnHmiDisplayChanged(const ArgumentsRspaEntryAvnHmiDisplayChanged& args) {
        mRspaEntryAvnHmiDisplay = args.mInput_RspaEntryAvnHmiDisplay;
        updateEvent4GroupRSPA();
    }

    void onWarningSoundChanged(const ArgumentsWarningSoundChanged& args) {
        mPaFrontCenterWarnSoundStatus = args.mInput_PaFrontCenterWarnSoundStatus;
        mPdwFrontCenterWarnSoundStatus = args.mInput_PdwFrontCenterWarnSoundStatus;
        mCCWWarnSoundStatus = args.mInput_CCWWarnSoundStatus;
        mCCWWarnSoundStatusFromSAC = args.mInput_CCWWarnSoundStatusFromSAC;
        updateSoundWarn();
    }

    void onSysWarnSoundChanged(const ArgumentsSysWarnSoundChanged& args) {
        mPaWarnSoundStatus = args.mInput_PaWarnSoundStatus;
        mPdwCheckSoundStatus = args.mInput_PdwCheckSoundStatus;
        updateSoundPASysWarn();
        updateSoundPDWSensorFail();
        updateSoundPDWTailgateOpened();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigAdasPDW = args.mInter_ConfigAdasPDW;
        mConfigNewRCCW = args.mInter_ConfigNewRCCW;
        mConfigAdasPrkPDW = args.mInter_ConfigAdasPrkPDW;
        mConfigBdcPDW = args.mInter_ConfigBdcPDW;
        updateEventRccaFailure();
        updateEventPdwSystemFailure();
        updateEvent7GroupTimeout();
    }

    void onVcRspaUnparkingLeftDirectionStatusChanged(const ArgumentsVcRspaUnparkingLeftDirectionStatusChanged& args) {
        mVcRspaUnparkingLeftDirectionStatus = args.mInput_VCRspaUnparkingLeftDirectionStatus;
        updateRspaUnparkingLeftDirectionStat();
    }

    void onVcRspaUnparkingRightDirectionStatusChanged(const ArgumentsVcRspaUnparkingRightDirectionStatusChanged& args) {
        mVcRspaUnparkingRightDirectionStatus = args.mInput_VCRspaUnparkingRightDirectionStatus;
        updateRspaUnparkingRightDirectionStat();
    }

    void onRspaParkingSlotValueChanged(const ArgumentsRspaParkingSlotValueChanged& args) {
        mRspaParkingSlotValue = args.mInput_RspaParkingSlotValue;
        updateInterRspaParkingSlotIdentificationStatus();
        updateInterRspaToggleInformationStatus();
        updateInterRspaParkingDirectionInformationStatus();
        updateInterRspaParkingSlotLocationStatus();
        updateInterRspaOperationInformationStatus();
    }

    void onVcRspaAvailableStatusChanged(const ArgumentsVcRspaAvailableStatusChanged& args) {
        mVCRspaAvailableStatus = args.mInput_VCRspaAvailableStatus;
        updateAdasParkingAvailableStat();
    }

    void onVcAdasParkingPathDisplayStatusChanged(const ArgumentsVcAdasParkingPathDisplayStatusChanged& args) {
        mVCAdasParkingPathDisplayStatus = args.mInput_VCAdasParkingPathDisplayStatus;
        updateAdasParkingPathStat();
        updateAdasParkingMappingPathStat();
        updateAdasParkingEntirePathStat();
        updateAdasParkingRemainedDistanceStat();
    }

    void onMpaButtunDisplayStatusChanged(const ArgumentsMpaButtunDisplayStatusChanged& args) {
        mMpaButtunDisplayStatus = args.mInput_MpaButtunDisplayStatus;
        updateMpaButtonStat();
    }

    void onMpaRecordButtunDisplayStatusChanged(const ArgumentsMpaRecordButtunDisplayStatusChanged& args) {
        mMpaRecordButtunDisplayStatus = args.mInput_MpaRecordButtunDisplayStatus;
        updateMpaRecordButtonStat();
    }

    void onIndAdasParkingRemainedDistanceValueChanged(const ArgumentsIndAdasParkingRemainedDistanceValueChanged& args) {
        mIndAdasParkingRemainedDistanceValue = args.mInput_INDAdasParkingRemainedDistanceValue;
        updateAdasParkingRemainedDistanceStat();
    }

    void onBirdViewInfoChanged(const ArgumentsBirdViewInfoChanged& args) {
        mNTFGroup1AdasWarning1_1Status = args.mInput_NTFGroup1AdasWarning1_1Status;
        mNTFGroup4AdasWarning1_1Status = args.mInput_NTFGroup4AdasWarning1_1Status;
        updateBirdViewDisplayStat();
    }

    void onEventPcaRccwWarningPopupChanged(const ArgumentsEventPcaRccwWarningPopupChanged& args) {
        mAdasPcaWarningPhraseStatus = args.mInput_AdasPcaWarningPhraseStatus;
        mPuPvLabelStatus = args.mInput_PuPvLabelStatus;
        updateEventPcaRccwWarningPopup();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateInterRspaParkingSlotIdentificationStatus();
        updateParkFrontMasterWarning();
        updateParkRearMasterWarning();
        updateParkLeftMasterWarning();
        updateParkRightMasterWarning();
        updateEventAdasPdwSystemFailure();
        updateEventIbuPdwSystemFailure();
        updateEventAdasPcaSystemFailure();
        updateEventRccaFailure();
        updateEventExitSafetyFailure();
        updateEventPdwSystemFailure();
        updateConstantRSPAEntryButton();
        updateConstantRSPAEntryDirection();
        updateParkFrontLeftMasterWarning();
        updateParkFrontRightMasterWarning();
        updateParkRearLeftMasterWarning();
        updateParkRearRightMasterWarning();
        updateRspaUnparkingLeftDirectionStat();
        updateRspaUnparkingRightDirectionStat();
        updateAdasParkingAvailableStat();
        updateAdasParkingPathStat();
        updateMpaButtonStat();
        updateMpaRecordButtonStat();
        updateAdasParkingMappingPathStat();
        updateAdasParkingEntirePathStat();
        updateAdasParkingRemainedDistanceStat();
        updateEvent7GroupFailure10();
        updateEvent7GroupFailure11();
        updateEvent7GroupCCWFailure();
        updateEvent2Group();
        updateEvent7GroupTimeout();
        updateEvent4GroupMRA();
        updateEvent4GroupRSPA();
        updateSoundWarn();
        updateSoundPASysWarn();
        updateSoundPDWSensorFail();
        updateSoundPDWTailgateOpened();
        updatePdwSoundMute();
        updateAllContourLevelStat();
        updateConstantTopViewScale();
        updateConstantMRAButton();
        updateAdasParkingControlTelltale();
    }

    void updateAllContourLevelStat() {
        updateConstantFrontContourLevel01Stat();
        updateConstantFrontContourLevel02Stat();
        updateConstantFrontContourLevel03Stat();
        updateConstantFrontContourLevel04Stat();
        updateConstantFrontContourLevel05Stat();
        updateConstantFrontContourLevel06Stat();
        updateConstantFrontContourLevel07Stat();
        updateConstantFrontContourLevel08Stat();
        updateConstantFrontContourLevel09Stat();
        updateConstantRearContourLevel01Stat();
        updateConstantRearContourLevel02Stat();
        updateConstantRearContourLevel03Stat();
        updateConstantRearContourLevel04Stat();
        updateConstantRearContourLevel05Stat();
        updateConstantRearContourLevel06Stat();
        updateConstantRearContourLevel07Stat();
        updateConstantRearContourLevel08Stat();
        updateConstantRearContourLevel09Stat();
        updateConstantLeftContourLevel01Stat();
        updateConstantLeftContourLevel02Stat();
        updateConstantLeftContourLevel03Stat();
        updateConstantLeftContourLevel04Stat();
        updateConstantLeftContourLevel05Stat();
        updateConstantLeftContourLevel06Stat();
        updateConstantLeftContourLevel07Stat();
        updateConstantRightContourLevel01Stat();
        updateConstantRightContourLevel02Stat();
        updateConstantRightContourLevel03Stat();
        updateConstantRightContourLevel04Stat();
        updateConstantRightContourLevel05Stat();
        updateConstantRightContourLevel06Stat();
        updateConstantRightContourLevel07Stat();
    }

    // 5.4.4.2.1 Parking Slot ID & 5.4.5.1.16.4.1 Slot ID
    void updateInterRspaParkingSlotIdentificationStatus() {
        SFCADAS_PARKING_2_0Inter_RspaParkingSlotIdentificationStatus stat =
            SFCADAS_PARKING_2_0Inter_RspaParkingSlotIdentificationStatus::OFF;

        if (mIsIgnOn == true) {
            switch (mRspaParkingSlotValue & 0xF) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    stat = static_cast<SFCADAS_PARKING_2_0Inter_RspaParkingSlotIdentificationStatus>(mRspaParkingSlotValue);
                    break;
                default:
                    break;
            }
        }

        setSFCADAS_PARKING_2_0Inter_RspaParkingSlotIdentificationStatus(stat);
        setSFCADAS_PARKING_2_0ConstantRSPAParkingSlotIdentificationStat(
            static_cast<SFCADAS_PARKING_2_0ConstantRSPAParkingSlotIdentificationStat>(stat));
    }

    // 5.4.4.2.2 Toggle Information & 5.4.5.1.16.4.2 Toggle Information
    void updateInterRspaToggleInformationStatus() {
        if (mIsIgnOn == true) {
            SFCADAS_PARKING_2_0Inter_RspaToggleInformationStatus interStat =
                SFCADAS_PARKING_2_0Inter_RspaToggleInformationStatus::ON;
            mConstantRspaToggleInformationStat = SFCADAS_PARKING_2_0ConstantRspaToggleInformationStat::ON;
            if (((mRspaParkingSlotValue >> 4) & 1) == 0) {
                interStat = SFCADAS_PARKING_2_0Inter_RspaToggleInformationStatus::OFF;
                mConstantRspaToggleInformationStat = SFCADAS_PARKING_2_0ConstantRspaToggleInformationStat::OFF;
            }
            setSFCADAS_PARKING_2_0Inter_RspaToggleInformationStatus(interStat);
            setSFCADAS_PARKING_2_0ConstantRspaToggleInformationStat(mConstantRspaToggleInformationStat);
        }
    }

    // 5.4.4.2.3 Parking Direction Information
    void updateInterRspaParkingDirectionInformationStatus() {
        if (mIsIgnOn == true) {
            mInterRspaParkingDirectionInformationStatus = SFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus::FORWARD;
            if (((mRspaParkingSlotValue >> 5) & 1) == 0) {
                mInterRspaParkingDirectionInformationStatus =
                    SFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus::BACKWARD;
            }
            setSFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus(mInterRspaParkingDirectionInformationStatus);
            updateRSPAParkingDirectionStat();
        }
    }

    // 5.4.4.2.4 Parking Slot Location
    void updateInterRspaParkingSlotLocationStatus() {
        if (mIsIgnOn == true) {
            mInterRspaParkingSlotLocationStatus = SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus::RIGHT;
            if (((mRspaParkingSlotValue >> 6) & 1) == 0) {
                mInterRspaParkingSlotLocationStatus = SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus::LEFT;
            }
            setSFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus(mInterRspaParkingSlotLocationStatus);
            updateRSPAParkingDirectionStat();
        }
    }

    // 5.4.4.2.5 Operation Information & 5.4.5.1.16.4.4 동작 상태
    void updateInterRspaOperationInformationStatus() {
        if (mIsIgnOn == true) {
            mInterRspaOperationInformationStatus = SFCADAS_PARKING_2_0Inter_RspaOperationInformationStatus::ON;
            if (((mRspaParkingSlotValue >> 7) & 1) == 0) {
                mInterRspaOperationInformationStatus = SFCADAS_PARKING_2_0Inter_RspaOperationInformationStatus::OFF;
            }

            setSFCADAS_PARKING_2_0Inter_RspaOperationInformationStatus(mInterRspaOperationInformationStatus);
            setSFCADAS_PARKING_2_0ConstantRSPAOperationInformationStat(
                static_cast<SFCADAS_PARKING_2_0ConstantRSPAOperationInformationStat>(mInterRspaOperationInformationStatus));
        }
    }

    // 5.4.5.1.1 VIEW 표시 (1)
    // 5.4.5.1.1.1 TOP VIEW
    void updateCarBody() {
        SFCADAS_PARKING_2_0ConstantParkCarBodyDisplayStat stat = SFCADAS_PARKING_2_0ConstantParkCarBodyDisplayStat::OFF;

        if (mVCTopviewControlStatus == VCTopviewControlStatus::TOPVIEW_ON) {
            stat = SFCADAS_PARKING_2_0ConstantParkCarBodyDisplayStat::ON;
        }
        setSFCADAS_PARKING_2_0ConstantParkCarBodyDisplayStat(stat);
    }

    // 5.4.5.1.1.2 BIRD VIEW
    void updateBirdViewDisplayStat() {
        SFCADAS_PARKING_2_0ConstantBirdViewDisplayStat stat = SFCADAS_PARKING_2_0ConstantBirdViewDisplayStat::OFF;
        if (mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::WARNING_FRONT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::WARNING_FRONT_LEFT_CROSSING ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::WARNING_FRONT_RIGHT_CROSSING ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::BRAKING_FRONT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_LEFT_CROSSING ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::BRAKING_FRONT_RIGHT_CROSSING ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_FRONT_LEFT_PATH ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_FRONT_RIGHT_PATH ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_REAR_LEFT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::STEERING_REAR_RIGHT ||
            mNTFGroup1AdasWarning1_1Status == NTFGroup1AdasWarning1_1Status::END_OF_ASSIST ||
            mNTFGroup4AdasWarning1_1Status == NTFGroup4AdasWarning1_1Status::SCC_ATTENTION_WARNING ||
            mNTFGroup4AdasWarning1_1Status == NTFGroup4AdasWarning1_1Status::DAW_LEAD_VEHICLE_DEPARTURE) {
            stat = SFCADAS_PARKING_2_0ConstantBirdViewDisplayStat::ON;
        }
        setSFCADAS_PARKING_2_0ConstantBirdViewDisplayStat(stat);
    }

    // 5.4.5.1.2 PDW 경고음 ON/OFF (2) - IMG_CONN_PDWSoundMute_Stat
    void updatePdwSoundMute() {
        SFCADAS_PARKING_2_0ConstantPDWSoundMuteStat stat = SFCADAS_PARKING_2_0ConstantPDWSoundMuteStat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwWarningSoundMuteStatus == AdasPdwWarningSoundMuteStatus::DISABLED ||
                mIbuPdwWarningSoundMuteStatus == IbuPdwWarningSoundMuteStatus::DISABLED) {
                stat = SFCADAS_PARKING_2_0ConstantPDWSoundMuteStat::DISABLE;
            } else if (mAdasPdwWarningSoundMuteStatus == AdasPdwWarningSoundMuteStatus::MUTE_OFF ||
                       mIbuPdwWarningSoundMuteStatus == IbuPdwWarningSoundMuteStatus::MUTE_OFF) {
                stat = SFCADAS_PARKING_2_0ConstantPDWSoundMuteStat::MUTE_OFF;
            } else if (mAdasPdwWarningSoundMuteStatus == AdasPdwWarningSoundMuteStatus::MUTE_ON ||
                       mIbuPdwWarningSoundMuteStatus == IbuPdwWarningSoundMuteStatus::MUTE_ON) {
                stat = SFCADAS_PARKING_2_0ConstantPDWSoundMuteStat::MUTE_ON;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantPDWSoundMuteStat(stat);
    }

    // 5.4.5.1.3 PDW 전방 고장 표시 (충돌/고장/가림) (3)
    void updateParkFrontMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkFrontMasterWarningStat stat = SFCADAS_PARKING_2_0ConstantParkFrontMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasFrontMasterWarningDisplayStatus == AdasFrontMasterWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkFrontMasterWarningStat::RED;
            } else if (mAdasFrontMasterWarningDisplayStatus == AdasFrontMasterWarningDisplayStatus::FAILURE_BLOCKAGE ||
                       mIbuFrontMasterWarningDisplayStatus == IbuFrontMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                stat = SFCADAS_PARKING_2_0ConstantParkFrontMasterWarningStat::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkFrontMasterWarningStat(stat);
    }

    // 5.4.5.1.4 PDW 후방 고장 표시 (충돌/고장/가림) (4)
    void updateParkRearMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkRearMasterWarningStat stat = SFCADAS_PARKING_2_0ConstantParkRearMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasRearMasterWarningDisplayStatus == AdasRearMasterWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkRearMasterWarningStat::RED;
            } else if (mAdasRearMasterWarningDisplayStatus == AdasRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE ||
                       mIbuRearMasterWarningDisplayStatus == IbuRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                stat = SFCADAS_PARKING_2_0ConstantParkRearMasterWarningStat::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkRearMasterWarningStat(stat);
    }

    // 5.4.5.1.5 PDW 좌측방 고장 표시 (충돌/고장/가림) (5)
    void updateParkLeftMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkLeftSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkLeftSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasLeftMasterWarningDisplayStatus == AdasLeftMasterWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkLeftSideMasterWarningStat::RED;
            } else if (mAdasLeftMasterWarningDisplayStatus == AdasLeftMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                stat = SFCADAS_PARKING_2_0ConstantParkLeftSideMasterWarningStat::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkLeftSideMasterWarningStat(stat);
    }

    // 5.4.5.1.6 PDW 우측방 고장 표시 (충돌/고장/가림) (6)
    void updateParkRightMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkRightSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkRightSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasRightMasterWarningDisplayStatus == AdasRightMasterWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkRightSideMasterWarningStat::RED;
            } else if (mAdasRightMasterWarningDisplayStatus == AdasRightMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                stat = SFCADAS_PARKING_2_0ConstantParkRightSideMasterWarningStat::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkRightSideMasterWarningStat(stat);
    }

    // 5.4.5.1.7 Contour 경고 라인 표시 (7)
    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(01)
    void updateConstantFrontContourLevel01Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel01Value == AdasPdwFrontContourLineLevel01Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel01Value == IbuPdwFrontContourLineLevel01Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel01Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(02)
    void updateConstantFrontContourLevel02Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel02Value == AdasPdwFrontContourLineLevel02Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel02Value == IbuPdwFrontContourLineLevel02Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel02Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(03)
    void updateConstantFrontContourLevel03Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel03Value == AdasPdwFrontContourLineLevel03Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel03Value == IbuPdwFrontContourLineLevel03Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel03Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(04)
    void updateConstantFrontContourLevel04Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel04Value == AdasPdwFrontContourLineLevel04Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel04Value == IbuPdwFrontContourLineLevel04Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel04Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(05)
    void updateConstantFrontContourLevel05Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel05Value == AdasPdwFrontContourLineLevel05Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel05Value == IbuPdwFrontContourLineLevel05Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel05Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(06)
    void updateConstantFrontContourLevel06Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel06Value == AdasPdwFrontContourLineLevel06Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel06Value == IbuPdwFrontContourLineLevel06Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel06Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(07)
    void updateConstantFrontContourLevel07Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel07Value == AdasPdwFrontContourLineLevel07Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel07Value == IbuPdwFrontContourLineLevel07Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel07Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(08)
    void updateConstantFrontContourLevel08Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel08Value == AdasPdwFrontContourLineLevel08Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel08Value == IbuPdwFrontContourLineLevel08Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel08Stat(stat);
    }

    // 5.4.5.1.7.1 전방 표시 01 ~ 09 - IMG_CONN_FrontContourLevel_Stat(09)
    void updateConstantFrontContourLevel09Stat() {
        SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_4 ||
                mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_4;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_5 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_5;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_6 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_6;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_7 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_7;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_8 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_8;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_9 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_9;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_10 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_10;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_11 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_11;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_12 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_12;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_13 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_14 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_13;
            } else if (mAdasPdwFrontContourLineLevel09Value == AdasPdwFrontContourLineLevel09Value::LEVEL_15 ||
                       mIbuPdwFrontContourLineLevel09Value == IbuPdwFrontContourLineLevel09Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantFrontContourLevel09Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(01)
    void updateConstantRearContourLevel01Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel01Value == AdasPdwRearContourLineLevel01Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel01Value == IbuPdwRearContourLineLevel01Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel01Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel01Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(02)
    void updateConstantRearContourLevel02Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel02Value == AdasPdwRearContourLineLevel02Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel02Value == IbuPdwRearContourLineLevel02Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel02Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel02Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(03)
    void updateConstantRearContourLevel03Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel03Value == AdasPdwRearContourLineLevel03Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel03Value == IbuPdwRearContourLineLevel03Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel03Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel03Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(04)
    void updateConstantRearContourLevel04Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel04Value == AdasPdwRearContourLineLevel04Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel04Value == IbuPdwRearContourLineLevel04Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel04Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel04Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(05)
    void updateConstantRearContourLevel05Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel05Value == AdasPdwRearContourLineLevel05Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel05Value == IbuPdwRearContourLineLevel05Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel05Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel05Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(06)
    void updateConstantRearContourLevel06Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel06Value == AdasPdwRearContourLineLevel06Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel06Value == IbuPdwRearContourLineLevel06Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel06Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel06Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(07)
    void updateConstantRearContourLevel07Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel07Value == AdasPdwRearContourLineLevel07Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel07Value == IbuPdwRearContourLineLevel07Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel07Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel07Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(08)
    void updateConstantRearContourLevel08Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel08Value == AdasPdwRearContourLineLevel08Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel08Value == IbuPdwRearContourLineLevel08Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel08Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel08Stat(stat);
    }

    // 5.4.5.1.7.2 후방 표시 01 ~ 09 - IMG_CONN_RearContourLevel_Stat(09)
    void updateConstantRearContourLevel09Stat() {
        SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_4 ||
                mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_4;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_5 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_5;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_6 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_6;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_7 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_7;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_8 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_8;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_9 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_9;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_10 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_10;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_11 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_11;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_12 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_12;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_13 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_14 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_13;
            } else if (mAdasPdwRearContourLineLevel09Value == AdasPdwRearContourLineLevel09Value::LEVEL_15 ||
                       mIbuPdwRearContourLineLevel09Value == IbuPdwRearContourLineLevel09Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRearContourLevel09Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRearContourLevel09Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(01)
    void updateConstantLeftContourLevel01Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel01Value == AdasPdwLeftContourLineLevel01Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel01Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(02)
    void updateConstantLeftContourLevel02Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel02Value == AdasPdwLeftContourLineLevel02Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel02Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(03)
    void updateConstantLeftContourLevel03Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel03Value == AdasPdwLeftContourLineLevel03Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel03Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(04)
    void updateConstantLeftContourLevel04Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel04Value == AdasPdwLeftContourLineLevel04Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel04Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(05)
    void updateConstantLeftContourLevel05Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel05Value == AdasPdwLeftContourLineLevel05Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel05Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(06)
    void updateConstantLeftContourLevel06Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel06Value == AdasPdwLeftContourLineLevel06Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel06Stat(stat);
    }

    // 5.4.5.1.7.3 좌측방 표시 01 ~ 07 - IMG_CONN_LeftContourLevel_Stat(07)
    void updateConstantLeftContourLevel07Stat() {
        SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_4;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_5;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_6;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_7;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_8;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_9;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_10;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_11;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_12;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwLeftContourLineLevel07Value == AdasPdwLeftContourLineLevel07Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantLeftContourLevel07Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(01)
    void updateConstantRightContourLevel01Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel01Value == AdasPdwRightContourLineLevel01Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel01Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel01Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(02)
    void updateConstantRightContourLevel02Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel02Value == AdasPdwRightContourLineLevel02Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel02Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel02Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(03)
    void updateConstantRightContourLevel03Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel03Value == AdasPdwRightContourLineLevel03Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel03Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel03Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(04)
    void updateConstantRightContourLevel04Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel04Value == AdasPdwRightContourLineLevel04Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel04Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel04Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(05)
    void updateConstantRightContourLevel05Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel05Value == AdasPdwRightContourLineLevel05Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel05Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel05Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(06)
    void updateConstantRightContourLevel06Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel06Value == AdasPdwRightContourLineLevel06Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel06Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel06Stat(stat);
    }

    // 5.4.5.1.7.4 우측방 표시 01 ~ 07 - IMG_CONN_RightContourLevel_Stat(07)
    void updateConstantRightContourLevel07Stat() {
        SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_4) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_4;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_5) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_5;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_6) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_6;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_7) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_7;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_8) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_8;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_9) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_9;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_10) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_10;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_11) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_11;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_12) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_12;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_13) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_14) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_13;
            } else if (mAdasPdwRightContourLineLevel07Value == AdasPdwRightContourLineLevel07Value::LEVEL_15) {
                stat = SFCADAS_PARKING_2_0ConstantRightContourLevel07Stat::LEVEL_13;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantRightContourLevel07Stat(stat);
    }

    // 5.4.5.1.8 자율 주차 점검 메시지
    // 5.4.5.1.8.1 주차 거리 경고 시스템 고장 (ADAS_PRK)
    void updateEventAdasPdwSystemFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "NT51001";
            } else if (mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::PDW_FAILURE) {
                eventID = "NT51002";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.PdwSystemFailure_PRK.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_PRKStat(SFCADAS_PARKING_2_0EventPdwSystemFailure_PRKStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_PRKID(eventID);
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_PRKStat(SFCADAS_PARKING_2_0EventPdwSystemFailure_PRKStat::ON);
        }
    }

    // 5.4.5.1.8.2 주차 거리 경고 시스템 고장 (BDC)
    void updateEventIbuPdwSystemFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "NT51003";
            } else if (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::PDW_FAILURE) {
                eventID = "NT51004";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.PdwSystemFailure_IBU.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_IBUStat(SFCADAS_PARKING_2_0EventPdwSystemFailure_IBUStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_IBUID(eventID);
            setSFCADAS_PARKING_2_0EventPdwSystemFailure_IBUStat(SFCADAS_PARKING_2_0EventPdwSystemFailure_IBUStat::ON);
        }
    }

    // 5.4.5.1.8.3 주차 충돌방지 보조 시스템 고장
    void updateEventAdasPcaSystemFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mAdasPcaSystemFailureStatus == AdasPcaSystemFailureStatus::CAMERA_BLOCKAGE) {
                eventID = "NT51005";
            } else if (mAdasPcaSystemFailureStatus == AdasPcaSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "NT51006";
            } else if (mAdasPcaSystemFailureStatus == AdasPcaSystemFailureStatus::PCA_FAILURE) {
                eventID = "NT51007";
            } else if (mAdasPcaSystemFailureStatus == AdasPcaSystemFailureStatus::LIDAR_BLOCKAGE) {
                eventID = "NT51008";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.PcaSystemFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventPcaSystemFailureStat(SFCADAS_PARKING_2_0EventPcaSystemFailureStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventPcaSystemFailureID(eventID);
            setSFCADAS_PARKING_2_0EventPcaSystemFailureStat(SFCADAS_PARKING_2_0EventPcaSystemFailureStat::ON);
        }
    }

    // 5.4.5.1.8.4 후방 교차 안전 고장
    void updateEventRccaFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if ((mNTFGroup7RccaFailure2Status == NTFGroup7RccaFailure2Status::UNHANDLED_TIMEOUT &&
                 mConfigNewRCCW == ConfigNewRCCW::ON) ||
                (mNTFGroup7RccaFailure2Status == NTFGroup7RccaFailure2Status::FAILURE)) {
                eventID = "NT51009";
            } else if (mNTFGroup7RccaFailure2Status == NTFGroup7RccaFailure2Status::RADAR_BLOCKAGE) {
                eventID = "NT51010";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.RccaFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventRccaFailureStat(SFCADAS_PARKING_2_0EventRccaFailureStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventRccaFailureID(eventID);
            setSFCADAS_PARKING_2_0EventRccaFailureStat(SFCADAS_PARKING_2_0EventRccaFailureStat::ON);
        }
    }

    // 5.4.5.1.8.5 하차 안전 고장
    void updateEventExitSafetyFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if ((mNTFGroup7ExitSafetyFailure2Status == NTFGroup7ExitSafetyFailure2Status::UNHANDLED_TIMEOUT) ||
                (mNTFGroup7ExitSafetyFailure2Status == NTFGroup7ExitSafetyFailure2Status::FAILURE)) {
                eventID = "NT51011";
            } else if (mNTFGroup7ExitSafetyFailure2Status == NTFGroup7ExitSafetyFailure2Status::RADAR_BLOCKAGE) {
                eventID = "NT51012";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.ExitSafetyFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventExitSafetyFailureStat(SFCADAS_PARKING_2_0EventExitSafetyFailureStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventExitSafetyFailureID(eventID);
            setSFCADAS_PARKING_2_0EventExitSafetyFailureStat(SFCADAS_PARKING_2_0EventExitSafetyFailureStat::ON);
        }
    }

    // 5.4.5.1.8.6 주차 거리 경고 시스템 Timeout 고장 (ADAS_PRK, BDC)
    void updateEventPdwSystemFailure() {
        std::string eventID;
        SFCADAS_PARKING_2_0EventPdwSystemFailureStat stat = SFCADAS_PARKING_2_0EventPdwSystemFailureStat::OFF;

        if (mIsIgnOn == true) {
            if ((mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasPrkPDW == ConfigAdasPrkPDW::ON) ||
                (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigBdcPDW == ConfigBdcPDW::ON)) {
                stat = SFCADAS_PARKING_2_0EventPdwSystemFailureStat::ON;
            }
        }

        setSFCADAS_PARKING_2_0EventPdwSystemFailureStat(stat);
    }

    // 5.4.5.1.9 PARKING TOP VIEW SCALE
    void updateConstantTopViewScale() {
        SFCADAS_PARKING_2_0ConstantTopViewScaleStat stat = SFCADAS_PARKING_2_0ConstantTopViewScaleStat::PERCENT_100;
        if (mIsIgnOn == true) {
            if (mVCTopviewScaleStatus == VCTopviewScaleStatus::PERCENT_50) {
                stat = SFCADAS_PARKING_2_0ConstantTopViewScaleStat::PERCENT_50;
            } else if (mVCTopviewScaleStatus == VCTopviewScaleStatus::PERCENT_100) {
                stat = SFCADAS_PARKING_2_0ConstantTopViewScaleStat::PERCENT_100;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantTopViewScaleStat(stat);
    }

    // 5.4.5.1.10 MRA 기능 버튼
    void updateConstantMRAButton() {
        SFCADAS_PARKING_2_0ConstantMRAButtonStat stat = SFCADAS_PARKING_2_0ConstantMRAButtonStat::OFF;
        if (mIsIgnOn == true) {
            if (mMraButtunDisplayStatus == MraButtunDisplayStatus::MRA_PATH_BUTTON) {
                stat = SFCADAS_PARKING_2_0ConstantMRAButtonStat::MRA_PATH;
            } else if (mMraButtunDisplayStatus == MraButtunDisplayStatus::ENABLE_MRA_START_BUTTON) {
                stat = SFCADAS_PARKING_2_0ConstantMRAButtonStat::MRA_START;
            } else if (mMraButtunDisplayStatus == MraButtunDisplayStatus::CANCEL_MRA_BUTTON) {
                stat = SFCADAS_PARKING_2_0ConstantMRAButtonStat::MRA_CANCEL;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantMRAButtonStat(stat);
    }

    // 5.4.5.1.11 RSPA(ENTRY)
    // 5.4.5.1.11.1 RSPA(ENTRY) 기능 버튼
    void updateConstantRSPAEntryButton() {
        SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::OFF;

        if (mIsIgnOn == true) {
            switch (mRspaButtunDisplayStatus) {
                case RspaButtunDisplayStatus::REMOTE_F_R_OPERATION_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::REMOTE_ON;
                    break;
                case RspaButtunDisplayStatus::CANCEL_RSPA_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::RSPA_E_CANCEL;
                    break;
                case RspaButtunDisplayStatus::PARKING_OPERATION_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::PARKING;
                    break;
                case RspaButtunDisplayStatus::UNPARKING_OPERATION_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::UNPARKING;
                    break;
                case RspaButtunDisplayStatus::RESUME_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat::RESUME;
                    break;
                default:
                    break;
            }
        }

        setSFCADAS_PARKING_2_0ConstantRSPAEntryButtonStat(stat);
    }

    // 5.4.5.1.11.2 RSPA(ENTRY) 원격 전/후진 상태
    void updateConstantRSPAEntryDirection() {
        SFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat stat = SFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat::OFF;

        if (mIsIgnOn == true) {
            switch (mVCRspaFunctionDisplayStatus) {
                case VCRspaFunctionDisplayStatus::READY:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat::READY;
                    break;
                case VCRspaFunctionDisplayStatus::FORWARD:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat::FORWARD;
                    break;
                case VCRspaFunctionDisplayStatus::BACKWARD:
                    stat = SFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat::BACKWARD;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantRSPAEntryDirectionStat(stat);
    }

    // 5.4.5.1.12 PCA 전방 좌측 경고 표시(충돌/고장/가림)
    void updateParkFrontLeftMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkFrontLeftSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkFrontLeftSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mPcaFrontLeftWarningDisplayStatus == PcaFrontLeftWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkFrontLeftSideMasterWarningStat::RED;
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkFrontLeftSideMasterWarningStat(stat);
    }

    // 5.4.5.1.13 PCA 전방 우측 경고 표시(충돌/고장/가림)
    void updateParkFrontRightMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkFrontRightSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkFrontRightSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mPcaFrontRightWarningDisplayStatus == PcaFrontRightWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkFrontRightSideMasterWarningStat::RED;
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkFrontRightSideMasterWarningStat(stat);
    }

    // 5.4.5.1.14 PCA 후방 좌측 경고 표시(충돌/고장/가림)
    void updateParkRearLeftMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkRearLeftSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkRearLeftSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mPcaRearLeftWarningDisplayStatus == PcaRearLeftWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkRearLeftSideMasterWarningStat::RED;
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkRearLeftSideMasterWarningStat(stat);
    }

    // 5.4.5.1.15 PCA 후방 우측 경고 표시(충돌/고장/가림)
    void updateParkRearRightMasterWarning() {
        SFCADAS_PARKING_2_0ConstantParkRearRightSideMasterWarningStat stat =
            SFCADAS_PARKING_2_0ConstantParkRearRightSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            if (mPcaRearRightWarningDisplayStatus == PcaRearRightWarningDisplayStatus::WARNING_ASSIST) {
                stat = SFCADAS_PARKING_2_0ConstantParkRearRightSideMasterWarningStat::RED;
            }
        }
        setSFCADAS_PARKING_2_0ConstantParkRearRightSideMasterWarningStat(stat);
    }

    // 5.4.5.1.16 RSPA3
    // 5.4.5.1.16.1 좌측 출차 버튼
    void updateRspaUnparkingLeftDirectionStat() {
        SFCADAS_PARKING_2_0ConstantRspaUnparkingLeftDirectionStat stat =
            SFCADAS_PARKING_2_0ConstantRspaUnparkingLeftDirectionStat::OFF;

        if (mIsIgnOn == true &&
            mVcRspaUnparkingLeftDirectionStatus == VcRspaUnparkingLeftDirectionStatus::LEFT_DIRECTION_BUTTON_DISPLAY) {
            stat = SFCADAS_PARKING_2_0ConstantRspaUnparkingLeftDirectionStat::UNPARKING_LEFT;
        }

        setSFCADAS_PARKING_2_0ConstantRspaUnparkingLeftDirectionStat(stat);
    }

    // 5.4.5.1.16.2 우측 출차 버튼
    void updateRspaUnparkingRightDirectionStat() {
        SFCADAS_PARKING_2_0ConstantRspaUnparkingRightDirectionStat stat =
            SFCADAS_PARKING_2_0ConstantRspaUnparkingRightDirectionStat::OFF;

        if (mIsIgnOn == true &&
            mVcRspaUnparkingRightDirectionStatus == VcRspaUnparkingRightDirectionStatus::RIGHT_DIRECTION_BUTTON_DISPLAY) {
            stat = SFCADAS_PARKING_2_0ConstantRspaUnparkingRightDirectionStat::UNPARKING_RIGHT;
        }

        setSFCADAS_PARKING_2_0ConstantRspaUnparkingRightDirectionStat(stat);
    }

    // 5.4.5.1.16.3 주차 가능 공간
    // 5.4.5.1.16.3.1 칼라
    void updateAdasParkingAvailableStat() {
        SFCADAS_PARKING_2_0ConstantAdasParkingAvailableStat stat = SFCADAS_PARKING_2_0ConstantAdasParkingAvailableStat::OFF;
        if (mIsIgnOn == true) {
            switch (mVCRspaAvailableStatus) {
                case VCRspaAvailableStatus::AVAILABLE:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingAvailableStat::AVAILABLE;
                    break;
                case VCRspaAvailableStatus::UNAVAILABLE:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingAvailableStat::UNAVAILABLE;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantAdasParkingAvailableStat(stat);
    }

    // 5.4.5.1.16.4.3 주차 방향
    void updateRSPAParkingDirectionStat() {
        if (mIsIgnOn == true) {
            if (mInterRspaParkingDirectionInformationStatus ==
                SFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus::BACKWARD) {
                if (mInterRspaParkingSlotLocationStatus == SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus::LEFT) {
                    setSFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat(
                        SFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat::BACKWARD_LEFT);
                } else {
                    setSFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat(
                        SFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat::BACKWARD_RIGHT);
                }
            } else {
                if (mInterRspaParkingSlotLocationStatus == SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus::LEFT) {
                    setSFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat(
                        SFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat::FORWARD_LEFT);
                } else {
                    setSFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat(
                        SFCADAS_PARKING_2_0ConstantRSPAParkingDirectionStat::FORWARD_RIGHT);
                }
            }
        }
    }

    // 5.4.5.1.17 MPA
    // 5.4.5.1.17.1 MPA 기능 버튼
    void updateMpaButtonStat() {
        SFCADAS_PARKING_2_0ConstantMPAButtonStat stat = SFCADAS_PARKING_2_0ConstantMPAButtonStat::OFF;
        if (mIsIgnOn == true) {
            switch (mMpaButtunDisplayStatus) {
                case MpaButtunDisplayStatus::MPA_PATH_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantMPAButtonStat::MPA_PATH;
                    break;
                case MpaButtunDisplayStatus::ENABLE_MPA_START_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantMPAButtonStat::MPA_START;
                    break;
                case MpaButtunDisplayStatus::CANCEL_MPA_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantMPAButtonStat::MPA_CANCEL;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantMPAButtonStat(stat);
    }

    // 5.4.5.1.17.2 MPA 경로 저장 버튼
    void updateMpaRecordButtonStat() {
        SFCADAS_PARKING_2_0ConstantMPARecordButtonStat stat = SFCADAS_PARKING_2_0ConstantMPARecordButtonStat::OFF;
        if (mIsIgnOn == true) {
            switch (mMpaRecordButtunDisplayStatus) {
                case MpaRecordButtunDisplayStatus::DISABLED_MPA_RECORD_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantMPARecordButtonStat::DISABLED;
                    break;
                case MpaRecordButtunDisplayStatus::ENABLED_MPA_RECORD_BUTTON:
                    stat = SFCADAS_PARKING_2_0ConstantMPARecordButtonStat::ENABLED;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantMPARecordButtonStat(stat);
    }

    // 5.4.5.1.18 자율주차 보조 뷰
    // 5.4.5.1.18.1 실시간 경로 표시(MRA, RSPA3, MPA)
    // 5.4.5.1.18.1.1 칼라
    void updateAdasParkingPathStat() {
        SFCADAS_PARKING_2_0ConstantAdasParkingPathStat stat = SFCADAS_PARKING_2_0ConstantAdasParkingPathStat::OFF;
        if (mIsIgnOn == true) {
            switch (mVCAdasParkingPathDisplayStatus) {
                case VCAdasParkingPathDisplayStatus::DISABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingPathStat::DISABLED;
                    break;
                case VCAdasParkingPathDisplayStatus::ENABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingPathStat::ENABLED;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantAdasParkingPathStat(stat);
    }

    // 5.4.5.1.18.2 저장 중 경로 표시(MPA)
    // 5.4.5.1.18.2.1 칼라
    void updateAdasParkingMappingPathStat() {
        SFCADAS_PARKING_2_0ConstantAdasParkingMappingPathStat stat = SFCADAS_PARKING_2_0ConstantAdasParkingMappingPathStat::OFF;
        if (mIsIgnOn == true) {
            switch (mVCAdasParkingPathDisplayStatus) {
                case VCAdasParkingPathDisplayStatus::DISABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingMappingPathStat::DISABLED;
                    break;
                case VCAdasParkingPathDisplayStatus::ENABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingMappingPathStat::ENABLED;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantAdasParkingMappingPathStat(stat);
    }

    // 5.4.5.1.18.3 전체 경로 표시(MPA)
    // 5.4.5.1.18.3.1 칼라
    void updateAdasParkingEntirePathStat() {
        SFCADAS_PARKING_2_0ConstantAdasParkingEntirePathStat stat = SFCADAS_PARKING_2_0ConstantAdasParkingEntirePathStat::OFF;
        if (mIsIgnOn == true) {
            switch (mVCAdasParkingPathDisplayStatus) {
                case VCAdasParkingPathDisplayStatus::DISABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingEntirePathStat::DISABLED;
                    break;
                case VCAdasParkingPathDisplayStatus::ENABLED:
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingEntirePathStat::ENABLED;
                    break;
                default:
                    break;
            }
        }
        setSFCADAS_PARKING_2_0ConstantAdasParkingEntirePathStat(stat);
    }

    // 5.4.5.2 Telltale/Indicator
    // 5.4.5.2.1 자율주차 작동 상태
    void updateAdasParkingControlTelltale() {
        SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat stat =
            SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat::OFF;
        SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStatOptional statOptional =
            SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStatOptional::OFF;
        if (mIsIgnOn == true) {
            if (mINDAdasParkingControlSymbolStatus == INDAdasParkingControlSymbolStatus::DISABLED) {
                stat = SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat::GRAY;
            } else if (mINDAdasParkingControlSymbolStatus == INDAdasParkingControlSymbolStatus::ENABLED &&
                       !(mINDAdasParkingProgressBarValue >= 0x0 && mINDAdasParkingProgressBarValue <= 0x64)) {
                stat = SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat::GREEN;
            } else if (mINDAdasParkingControlSymbolStatus == INDAdasParkingControlSymbolStatus::ENABLED &&
                       (mINDAdasParkingProgressBarValue >= 0x0 && mINDAdasParkingProgressBarValue <= 0x64)) {
                stat = SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat::OPTIONAL;
                statOptional = SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStatOptional::GREEN_PROGRESS;
            } else if (mINDAdasParkingControlSymbolStatus == INDAdasParkingControlSymbolStatus::ALERT) {
                stat = SFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat::AMBER;
            } else {
                // OFF
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStat(stat);
        setSFCADAS_PARKING_2_0TelltaleAdasParkingControlTelltaleStatOptional(statOptional);
    }

    // 5.4.5.2.1 잔여 경로 거리 표시
    void updateAdasParkingRemainedDistanceStat() {
        SFCADAS_PARKING_2_0ConstantAdasParkingRemainedDistanceStat stat =
            SFCADAS_PARKING_2_0ConstantAdasParkingRemainedDistanceStat::OFF;
        HUInt64 value = 0xFFFF;
        if (mIsIgnOn == true) {
            if (mIndAdasParkingRemainedDistanceValue >= 0 && mIndAdasParkingRemainedDistanceValue <= 20000) {
                if (mVCAdasParkingPathDisplayStatus == VCAdasParkingPathDisplayStatus::DISABLED ||
                    mVCAdasParkingPathDisplayStatus == VCAdasParkingPathDisplayStatus::ENABLED) {
                    stat = SFCADAS_PARKING_2_0ConstantAdasParkingRemainedDistanceStat::VALUE;
                    value = mIndAdasParkingRemainedDistanceValue;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCADAS_PARKING_2_0ConstantAdasParkingRemainedDistanceStat(stat);
        setSFCADAS_PARKING_2_0ConstantAdasParkingRemainedDistanceValue(value);
    }

    // 5.4.5.3 이벤트 표시 동작 로직
    // 5.4.5.3.1 7그룹 풀 팝업 – 운전자 보조 시스템 고장 3 (ADAS_PRK)
    void updateEvent7GroupFailure10() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::FAILURE) {
                eventID = "E53016";
            } else if (mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::CAMERA_BLOCKAGE) {
                eventID = "E53017";
            } else if (mPuFGroup7DriverAssistFailure10Status ==
                       PuFGroup7DriverAssistFailure10Status::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "E53018";
            } else if (mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::LIDAR_BLOCKAGE) {
                eventID = "E53021";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.Group7Failure10.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup7Failure10Stat(SFCADAS_PARKING_2_0EventGroup7Failure10Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup7Failure10ID(eventID);
            setSFCADAS_PARKING_2_0EventGroup7Failure10Stat(SFCADAS_PARKING_2_0EventGroup7Failure10Stat::ON);
        }
    }

    // 5.4.5.3.2 7그룹 풀 팝업 – 운전자 보조 시스템 고장 4 (BDC)
    void updateEvent7GroupFailure11() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::FAILURE) {
                eventID = "E53019";
            } else if (mPuFGroup7DriverAssistFailure11Status ==
                       PuFGroup7DriverAssistFailure11Status::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "E53020";
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.Group7Failure11.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup7Failure11Stat(SFCADAS_PARKING_2_0EventGroup7Failure11Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup7Failure11ID(eventID);
            setSFCADAS_PARKING_2_0EventGroup7Failure11Stat(SFCADAS_PARKING_2_0EventGroup7Failure11Stat::ON);
        }
    }

    // 5.4.5.3.3 7그룹 풀 팝업 – 주차 거리 경고 시스템 고장 (CCW)
    void updateEvent7GroupCCWFailure() {
        std::string eventID;
        std::string prevEventID;

        if (mIsIgnOn == true) {
            if (mCCWFailureStatus == CCWFailureStatus::FAILURE) {
                eventID = "E53022";
            } else if (mCCWFailureStatus == CCWFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "E53023";
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.Group7CCWFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup7CCWFailureStat(SFCADAS_PARKING_2_0EventGroup7CCWFailureStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup7CCWFailureID(eventID);
            setSFCADAS_PARKING_2_0EventGroup7CCWFailureStat(SFCADAS_PARKING_2_0EventGroup7CCWFailureStat::ON);
        }
    }

    // 5.4.5.3.4 2그룹 미니 팝업 - PCA ON/OFF
    void updateEvent2Group() {
        std::string eventID;
        static std::string lastActiveEventID;

        if (mIsIgnOn == true) {
            switch (mPcaModeDisplayStatus) {
                case PcaModeDisplayStatus::PCA_OFF:
                    eventID = "E53007";
                    break;
                case PcaModeDisplayStatus::PCA_ON:
                    eventID = "E53008";
                    break;
                default:
                    break;
            }
        }

        // 의도적으로 Event A On, B On 발생 시 flush를
        // Event B ON -> Event A OFF 순서로 전달하기 위해 다른 모듈과 출력 코드를 다르게 적용합니다.
        // A OFF 전달이 노드의 최종 상태가 되므로, Active 되어 있는 B Event의 추후 OFF 처리를 위하여
        // lastActiveEventID를 사용했습니다.
        // std::string prevEventID;
        // GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.2Group.ID, prevEventID);

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup2ID(eventID);
            setSFCADAS_PARKING_2_0EventGroup2Stat(SFCADAS_PARKING_2_0EventGroup2Stat::ON);
            flushLastGroup();
        } else {
            setSFCADAS_PARKING_2_0EventGroup2ID(lastActiveEventID);
            setSFCADAS_PARKING_2_0EventGroup2Stat(SFCADAS_PARKING_2_0EventGroup2Stat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup2ID(lastActiveEventID);
            setSFCADAS_PARKING_2_0EventGroup2Stat(SFCADAS_PARKING_2_0EventGroup2Stat::OFF);
            flushLastGroup();
        }
        lastActiveEventID = eventID;
    }

    // 5.4.5.3.5 7그룹 풀 팝업 – 운전자 보조 시스템 Timeout 고장 (ADAS_PRK, BDC)
    void updateEvent7GroupTimeout() {
        SFCADAS_PARKING_2_0EventGroup7TimeoutStat stat = SFCADAS_PARKING_2_0EventGroup7TimeoutStat::OFF;

        if (mIsIgnOn == true) {
            if ((mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::UNHANDLED_TIMEOUT &&
                 mConfigAdasPrkPDW == ConfigAdasPrkPDW::ON) ||
                (mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::UNHANDLED_TIMEOUT &&
                 mConfigBdcPDW == ConfigBdcPDW::ON)) {
                stat = SFCADAS_PARKING_2_0EventGroup7TimeoutStat::ON;
            }
        }
        setSFCADAS_PARKING_2_0EventGroup7TimeoutStat(stat);
    }

    // 5.4.5.3.6 4그룹 풀 팝업 – MRA 동작
    void updateEvent4GroupMRA() {
        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;
        SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            switch (mClusterHMIDisplayStatus) {
                case ClusterHMIDisplayStatus::REQUEST_MRA_START_BUTTON_INPUT:
                    eventID = "E53024";
                    break;
                case ClusterHMIDisplayStatus::REQUEST_HANDS_OFF:
                    eventID = "E53025";
                    break;
                case ClusterHMIDisplayStatus::BACKWARD_STEERING_CONTROL:
                    eventID = "E53026";
                    break;
                case ClusterHMIDisplayStatus::NEAR_TARGET_DESTINATION:
                    eventID = "E53027";
                    break;
                case ClusterHMIDisplayStatus::NEAR_LIMIT_SPEED:
                    eventID = "E53028";
                    break;
                case ClusterHMIDisplayStatus::REQUEST_VEHICLE_STOP_AND_BUTTON_INPUT:
                    eventID = "E53029";
                    break;
                case ClusterHMIDisplayStatus::CANCEL_BUTTON_INPUT:
                    eventID = "E53030";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::GEAR_SHIFT:
                    eventID = "E53031";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::EXCEED_LIMIT_SPEED:
                    eventID = "E53032";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::STANDSTILL_TIMEOUT:
                    eventID = "E53033";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::STEERING_WHEEL_OVERRIDE:
                    eventID = "E53034";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::INTERRUPT_SAFETY_FUNCTION:
                    eventID = "E53035";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::WHEEL_SLIP:
                    eventID = "E53036";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::PATH_DEVIATION:
                    eventID = "E53037";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::TRAILER_MODE:
                    eventID = "E53038";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::VISIT_SERVICE_CENTER:
                    eventID = "E53039";
                    linkedSoundID = "SND_ADAS_PDWSensorFail_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::DOOR_OPEN:
                    eventID = "E53040";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::TRUNK_OPEN:
                    eventID = "E53041";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::TAILGATE_OPEN:
                    eventID = "E53042";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::HOOD_OPEN:
                    eventID = "E53043";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::OUTSIDE_MIRROR_FOLDING:
                    eventID = "E53044";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::REFER_TO_THE_OWNERS_MANUAL:
                    eventID = "E53045";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case ClusterHMIDisplayStatus::ARRIVE_DESTINATION:
                    eventID = "E53046";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.Group4MRA.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup4MRAStat(SFCADAS_PARKING_2_0EventGroup4MRAStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup4MRAID(eventID);
            setSFCADAS_PARKING_2_0EventGroup4MRAStat(SFCADAS_PARKING_2_0EventGroup4MRAStat::ON);
            setSFCADAS_PARKING_2_0EventGroup4MRALinkedSoundID(linkedSoundID);
            setSFCADAS_PARKING_2_0EventGroup4MRALinkedSoundType(linkedSoundType);
            setSFCADAS_PARKING_2_0EventGroup4MRALinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.4.5.3.7 4그룹 풀 팝업 – RSPA(ENTRY) 동작
    void updateEvent4GroupRSPA() {
        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;
        SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType linkedSoundType =
            SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            switch (mRspaEntryAvnHmiDisplay) {
                case RspaEntryAvnHmiDisplay::REMOTE_MOVING:
                    eventID = "E53047";
                    break;
                case RspaEntryAvnHmiDisplay::MOVING_FORWARD:
                    eventID = "E53048";
                    break;
                case RspaEntryAvnHmiDisplay::MOVING_BACKWARD:
                    eventID = "E53049";
                    break;
                case RspaEntryAvnHmiDisplay::REMOTE_CONTROL_RANGE_EXCEEDED:
                    eventID = "E53050";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::HOLD_DOWN_BUTTON_ON_KEY_FOB:
                    eventID = "E53051";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CHECK_TAILGATE_CLOSED:
                    eventID = "E53052";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CHECK_TRUNK_CLOSED:
                    eventID = "E53053";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CHECK_DOOR_CLOSED:
                    eventID = "E53054";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::MONITOR_SURROUNDING_AREA:
                    eventID = "E53055";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_TRAILER_MODE:
                    eventID = "E53056";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_READ_MANUAL:
                    eventID = "E53057";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_SLOPE_UP:
                    eventID = "E53058";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_SLOPE_DOWN:
                    eventID = "E53059";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_USS_BLOCKAGE:
                    eventID = "E53060";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::NOT_WORKING_CONDITION_DCT_OVERTEMP:
                    eventID = "E53061";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_CANCEL_INPUT:
                    eventID = "E53062";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_READ_MANUAL:
                    eventID = "E53063";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_TRAILER_MODE:
                    eventID = "E53064";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_PDW_OFF:
                    eventID = "E53065";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_REVERSE_MOVEMENT:
                    eventID = "E53066";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CHECK_VISIT_TECH_CENTER:
                    eventID = "E53067";
                    linkedSoundID = "SND_ADAS_PDWSensorFail_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_SLOPE_UP:
                    eventID = "E53068";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_SLOPE_DOWN:
                    eventID = "E53069";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_SURROUNDING_AREA:
                    eventID = "E53070";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_CHECK_ROAD_CONDITION:
                    eventID = "E53071";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_EXCEED_OPERATING_DISTANCE:
                    eventID = "E53072";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_EXCEED_OPERATION_TIME:
                    eventID = "E53073";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_REMOTE_START_BUTTON_INPUT:
                    eventID = "E53074";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_B_ALARM:
                    eventID = "E53075";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_TM_OPERATION:
                    eventID = "E53076";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_EPB_OPERATION:
                    eventID = "E53077";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_PEDAL_OPERATION:
                    eventID = "E53078";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_HOOD_OPEN:
                    eventID = "E53079";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DETECT_STEERING_WHEEL_OPERATION:
                    eventID = "E53080";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_USS_BLOCKAGE:
                    eventID = "E53081";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CANCELLED_DCT_OVERTEMP:
                    eventID = "E53082";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::REMOTE_MOVING_COMPLETED:
                    eventID = "E53083";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::REMOTE_PARKING:
                    eventID = "E53084";
                    break;
                case RspaEntryAvnHmiDisplay::PARKING_COMPLETED:
                    eventID = "E53085";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::PULL_OUT_COMPLETED:
                case RspaEntryAvnHmiDisplay::PULL_OUT_COMPLETED_MANUALLY:
                    eventID = "E53086";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::RSPA_CANCELLED_OPERATE_VEHICLE_MANUALLY:
                    eventID = "E53087";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::PARKING_COMPLETED_MANUALLY:
                    eventID = "E53088";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::RSPA_PAUSED_SEAT_BELT:
                    eventID = "E53089";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::OPERATION_GUIDE_RELEASE_BRAKE:
                    eventID = "E53090";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::RSPA_CANCELLED_NOT_STOP:
                    eventID = "E53091";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::RSPA_CANCELLED_UNFASTEN_SEATBELT:
                    eventID = "E53092";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::RSPA_CANCELLED_CHECK_SVM_CAMERA:
                    eventID = "E53093";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::CHECKING_SPACE_STOP_THE_VEHICLE:
                    eventID = "E53094";
                    break;
                case RspaEntryAvnHmiDisplay::CHECKING_COMPLETED_SELECT_PULL_OUT:
                    eventID = "E53095";
                    break;
                case RspaEntryAvnHmiDisplay::OPERATION_GUIDE_REINPUT_SMART_KEY:
                    eventID = "E53096";
                    linkedSoundID = "SND_ADAS_PAParkComplete_Fixed";
                    linkedSoundType = SFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case RspaEntryAvnHmiDisplay::SELECT_OPERATING_MODE:
                    eventID = "E53097";
                    break;
                case RspaEntryAvnHmiDisplay::PARKING_MODE_GUIDE_SMART_PARKING:
                    eventID = "E53098";
                    break;
                case RspaEntryAvnHmiDisplay::SMART_PULL_OUT:
                    eventID = "E53099";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.Group4RSPA.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventGroup4RSPAStat(SFCADAS_PARKING_2_0EventGroup4RSPAStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventGroup4RSPAID(eventID);
            setSFCADAS_PARKING_2_0EventGroup4RSPAStat(SFCADAS_PARKING_2_0EventGroup4RSPAStat::ON);
            setSFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundID(linkedSoundID);
            setSFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundType(linkedSoundType);
            setSFCADAS_PARKING_2_0EventGroup4RSPALinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.4.5.3.8 PCA/RCCW 경고 팝업
    void updateEventPcaRccwWarningPopup() {
        std::string eventID;
        std::string prevEventID;

        if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::ASSIST || mPuPvLabelStatus == PuPvLabelStatus::ASSIST) {
            eventID = "E53100";
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::WARNING ||
                   mPuPvLabelStatus == PuPvLabelStatus::WARNING) {
            eventID = "E53101";
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::BRAKE_HOLD ||
                   mPuPvLabelStatus == PuPvLabelStatus::BRAKE_HOLD) {
            eventID = "E53102";
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::ASSIST_ENDED ||
                   mPuPvLabelStatus == PuPvLabelStatus::END) {
            eventID = "E53103";
        } else {
            // no operation
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Event.PcaRccwWarningPopup.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_2_0EventPcaRccwWarningPopupStat(SFCADAS_PARKING_2_0EventPcaRccwWarningPopupStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_2_0EventPcaRccwWarningPopupID(eventID);
            setSFCADAS_PARKING_2_0EventPcaRccwWarningPopupStat(SFCADAS_PARKING_2_0EventPcaRccwWarningPopupStat::ON);
        }
    }

    // 5.4.6.1 Warning Sound
    void updateSoundWarn() {
        std::string soundID;

        if (mIsIgnOn == true) {
            if (mPaFrontCenterWarnSoundStatus == PaFrontCenterWarnSoundStatus::THIRD_WARN ||
                mPdwFrontCenterWarnSoundStatus == PdwFrontCenterWarnSoundStatus::THIRD_WARN) {
                soundID = "SND_ADAS_PDWLevel3_Auto";
            } else if (mPaFrontCenterWarnSoundStatus == PaFrontCenterWarnSoundStatus::SECOND_WARN ||
                       mPdwFrontCenterWarnSoundStatus == PdwFrontCenterWarnSoundStatus::SECOND_WARN) {
                soundID = "SND_ADAS_PDWLevel2_Auto";
            } else if (mPaFrontCenterWarnSoundStatus == PaFrontCenterWarnSoundStatus::FIRST_WARN ||
                       mPdwFrontCenterWarnSoundStatus == PdwFrontCenterWarnSoundStatus::FIRST_WARN) {
                soundID = "SND_ADAS_PDWLevel1_Auto";
            } else if (mCCWWarnSoundStatus == CCWWarnSoundStatus::THIRD_WARN ||
                       mCCWWarnSoundStatusFromSAC == CCWWarnSoundStatusFromSAC::THIRD_WARN) {
                soundID = "SND_ADAS_PDWLevel3_Auto";
            } else if (mCCWWarnSoundStatus == CCWWarnSoundStatus::SECOND_WARN ||
                       mCCWWarnSoundStatusFromSAC == CCWWarnSoundStatusFromSAC::SECOND_WARN) {
                soundID = "SND_ADAS_PDWLevel2_Auto";
            } else if (mCCWWarnSoundStatus == CCWWarnSoundStatus::FIRST_WARN ||
                       mCCWWarnSoundStatusFromSAC == CCWWarnSoundStatusFromSAC::FIRST_WARN) {
                soundID = "SND_ADAS_PDWLevel1_Auto";
            } else {
                // OFF
            }
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Sound.WarningLevel.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_PARKING_2_0SoundWarningLevelStat(SFCADAS_PARKING_2_0SoundWarningLevelStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_PARKING_2_0SoundWarningLevelID(soundID);
            setSFCADAS_PARKING_2_0SoundWarningLevelStat(SFCADAS_PARKING_2_0SoundWarningLevelStat::ON);
        }
    }

    // 5.4.6.2 System warning Sound (1) - PA Parking Complete, PA Scan Complete
    void updateSoundPASysWarn() {
        std::string soundID;
        std::string prevSoundID;

        if (mIsIgnOn == true) {
            switch (mPaWarnSoundStatus) {
                case PaWarnSoundStatus::PA_PARK_COMPLETE:
                case PaWarnSoundStatus::PA_SYS_ABORT:
                    soundID = "SND_ADAS_PAParkComplete_Fixed";
                    break;
                case PaWarnSoundStatus::PA_SCAN_COMPLETE:
                    soundID = "SND_ADAS_PAScanComplete_Fixed";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.ADAS_PARKING_2_0.Sound.PASysWarnSound.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_PARKING_2_0SoundPASysWarnSoundStat(SFCADAS_PARKING_2_0SoundPASysWarnSoundStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_PARKING_2_0SoundPASysWarnSoundID(soundID);
            setSFCADAS_PARKING_2_0SoundPASysWarnSoundStat(SFCADAS_PARKING_2_0SoundPASysWarnSoundStat::ON);
        }
    }

    // 5.4.6.2 System warning Sound (2) - PDW/PA Sensor Fail
    void updateSoundPDWSensorFail() {
        SFCADAS_PARKING_2_0SoundWarningSensorFailStat stat = SFCADAS_PARKING_2_0SoundWarningSensorFailStat::OFF;

        if ((mPdwCheckSoundStatus == PdwCheckSoundStatus::SENSOR_FAIL_WARN ||
             mPaWarnSoundStatus == PaWarnSoundStatus::SENSOR_FAIL_WARN) &&
            mIsIgnOn == true) {
            stat = SFCADAS_PARKING_2_0SoundWarningSensorFailStat::ON;
        }
        setSFCADAS_PARKING_2_0SoundWarningSensorFailStat(stat);
    }

    // 5.4.6.2 System warning Sound (3) - PDW Tailgate Opened
    void updateSoundPDWTailgateOpened() {
        SFCADAS_PARKING_2_0SoundWarningTailgateOpenedStat stat = SFCADAS_PARKING_2_0SoundWarningTailgateOpenedStat::OFF;

        if ((mPdwCheckSoundStatus == PdwCheckSoundStatus::PDW_R_DISABLE_SOUND ||
             mPaWarnSoundStatus == PaWarnSoundStatus::PDW_R_DEACTIVATED_WARNING_ALARM) &&
            mIsIgnOn == true) {
            stat = SFCADAS_PARKING_2_0SoundWarningTailgateOpenedStat::ON;
        }
        setSFCADAS_PARKING_2_0SoundWarningTailgateOpenedStat(stat);
    }

    template <typename T>
    inline void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);

        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    HBool mIsIgnOn = false;

    using VCTopviewControlStatus = ArgumentsCarBodyDisplayStatusChanged::Input_VCTopviewControlStatus;

    using AdasPdwWarningSoundMuteStatus = ArgumentsPdwSoundMuteStatusChanged::Input_AdasPdwWarningSoundMuteStatus;
    using IbuPdwWarningSoundMuteStatus = ArgumentsPdwSoundMuteStatusChanged::Input_IbuPdwWarningSoundMuteStatus;

    using AdasFrontMasterWarningDisplayStatus =
        ArgumentsParkFrontMasterWarningStatusChanged::Input_AdasFrontMasterWarningDisplayStatus;
    using IbuFrontMasterWarningDisplayStatus =
        ArgumentsParkFrontMasterWarningStatusChanged::Input_IbuFrontMasterWarningDisplayStatus;
    using AdasRearMasterWarningDisplayStatus =
        ArgumentsParkRearMasterWarningStatusChanged::Input_AdasRearMasterWarningDisplayStatus;
    using IbuRearMasterWarningDisplayStatus =
        ArgumentsParkRearMasterWarningStatusChanged::Input_IbuRearMasterWarningDisplayStatus;
    using AdasLeftMasterWarningDisplayStatus =
        ArgumentsParkLeftMasterWarningStatusChanged::Input_AdasLeftMasterWarningDisplayStatus;
    using AdasRightMasterWarningDisplayStatus =
        ArgumentsParkRightMasterWarningStatusChanged::Input_AdasRightMasterWarningDisplayStatus;

    using AdasPdwFrontContourLineLevel01Value =
        ArgumentsAdasPdwFrontContourLineLevel01ValueChanged::Input_AdasPdwFrontContourLineLevel01Value;
    using AdasPdwFrontContourLineLevel02Value =
        ArgumentsAdasPdwFrontContourLineLevel02ValueChanged::Input_AdasPdwFrontContourLineLevel02Value;
    using AdasPdwFrontContourLineLevel03Value =
        ArgumentsAdasPdwFrontContourLineLevel03ValueChanged::Input_AdasPdwFrontContourLineLevel03Value;
    using AdasPdwFrontContourLineLevel04Value =
        ArgumentsAdasPdwFrontContourLineLevel04ValueChanged::Input_AdasPdwFrontContourLineLevel04Value;
    using AdasPdwFrontContourLineLevel05Value =
        ArgumentsAdasPdwFrontContourLineLevel05ValueChanged::Input_AdasPdwFrontContourLineLevel05Value;
    using AdasPdwFrontContourLineLevel06Value =
        ArgumentsAdasPdwFrontContourLineLevel06ValueChanged::Input_AdasPdwFrontContourLineLevel06Value;
    using AdasPdwFrontContourLineLevel07Value =
        ArgumentsAdasPdwFrontContourLineLevel07ValueChanged::Input_AdasPdwFrontContourLineLevel07Value;
    using AdasPdwFrontContourLineLevel08Value =
        ArgumentsAdasPdwFrontContourLineLevel08ValueChanged::Input_AdasPdwFrontContourLineLevel08Value;
    using AdasPdwFrontContourLineLevel09Value =
        ArgumentsAdasPdwFrontContourLineLevel09ValueChanged::Input_AdasPdwFrontContourLineLevel09Value;
    using IbuPdwFrontContourLineLevel01Value =
        ArgumentsIbuPdwFrontContourLineLevel01ValueChanged::Input_IbuPdwFrontContourLineLevel01Value;
    using IbuPdwFrontContourLineLevel02Value =
        ArgumentsIbuPdwFrontContourLineLevel02ValueChanged::Input_IbuPdwFrontContourLineLevel02Value;
    using IbuPdwFrontContourLineLevel03Value =
        ArgumentsIbuPdwFrontContourLineLevel03ValueChanged::Input_IbuPdwFrontContourLineLevel03Value;
    using IbuPdwFrontContourLineLevel04Value =
        ArgumentsIbuPdwFrontContourLineLevel04ValueChanged::Input_IbuPdwFrontContourLineLevel04Value;
    using IbuPdwFrontContourLineLevel05Value =
        ArgumentsIbuPdwFrontContourLineLevel05ValueChanged::Input_IbuPdwFrontContourLineLevel05Value;
    using IbuPdwFrontContourLineLevel06Value =
        ArgumentsIbuPdwFrontContourLineLevel06ValueChanged::Input_IbuPdwFrontContourLineLevel06Value;
    using IbuPdwFrontContourLineLevel07Value =
        ArgumentsIbuPdwFrontContourLineLevel07ValueChanged::Input_IbuPdwFrontContourLineLevel07Value;
    using IbuPdwFrontContourLineLevel08Value =
        ArgumentsIbuPdwFrontContourLineLevel08ValueChanged::Input_IbuPdwFrontContourLineLevel08Value;
    using IbuPdwFrontContourLineLevel09Value =
        ArgumentsIbuPdwFrontContourLineLevel09ValueChanged::Input_IbuPdwFrontContourLineLevel09Value;
    using AdasPdwRearContourLineLevel01Value =
        ArgumentsAdasPdwRearContourLineLevel01ValueChanged::Input_AdasPdwRearContourLineLevel01Value;
    using AdasPdwRearContourLineLevel02Value =
        ArgumentsAdasPdwRearContourLineLevel02ValueChanged::Input_AdasPdwRearContourLineLevel02Value;
    using AdasPdwRearContourLineLevel03Value =
        ArgumentsAdasPdwRearContourLineLevel03ValueChanged::Input_AdasPdwRearContourLineLevel03Value;
    using AdasPdwRearContourLineLevel04Value =
        ArgumentsAdasPdwRearContourLineLevel04ValueChanged::Input_AdasPdwRearContourLineLevel04Value;
    using AdasPdwRearContourLineLevel05Value =
        ArgumentsAdasPdwRearContourLineLevel05ValueChanged::Input_AdasPdwRearContourLineLevel05Value;
    using AdasPdwRearContourLineLevel06Value =
        ArgumentsAdasPdwRearContourLineLevel06ValueChanged::Input_AdasPdwRearContourLineLevel06Value;
    using AdasPdwRearContourLineLevel07Value =
        ArgumentsAdasPdwRearContourLineLevel07ValueChanged::Input_AdasPdwRearContourLineLevel07Value;
    using AdasPdwRearContourLineLevel08Value =
        ArgumentsAdasPdwRearContourLineLevel08ValueChanged::Input_AdasPdwRearContourLineLevel08Value;
    using AdasPdwRearContourLineLevel09Value =
        ArgumentsAdasPdwRearContourLineLevel09ValueChanged::Input_AdasPdwRearContourLineLevel09Value;
    using IbuPdwRearContourLineLevel01Value =
        ArgumentsIbuPdwRearContourLineLevel01ValueChanged::Input_IbuPdwRearContourLineLevel01Value;
    using IbuPdwRearContourLineLevel02Value =
        ArgumentsIbuPdwRearContourLineLevel02ValueChanged::Input_IbuPdwRearContourLineLevel02Value;
    using IbuPdwRearContourLineLevel03Value =
        ArgumentsIbuPdwRearContourLineLevel03ValueChanged::Input_IbuPdwRearContourLineLevel03Value;
    using IbuPdwRearContourLineLevel04Value =
        ArgumentsIbuPdwRearContourLineLevel04ValueChanged::Input_IbuPdwRearContourLineLevel04Value;
    using IbuPdwRearContourLineLevel05Value =
        ArgumentsIbuPdwRearContourLineLevel05ValueChanged::Input_IbuPdwRearContourLineLevel05Value;
    using IbuPdwRearContourLineLevel06Value =
        ArgumentsIbuPdwRearContourLineLevel06ValueChanged::Input_IbuPdwRearContourLineLevel06Value;
    using IbuPdwRearContourLineLevel07Value =
        ArgumentsIbuPdwRearContourLineLevel07ValueChanged::Input_IbuPdwRearContourLineLevel07Value;
    using IbuPdwRearContourLineLevel08Value =
        ArgumentsIbuPdwRearContourLineLevel08ValueChanged::Input_IbuPdwRearContourLineLevel08Value;
    using IbuPdwRearContourLineLevel09Value =
        ArgumentsIbuPdwRearContourLineLevel09ValueChanged::Input_IbuPdwRearContourLineLevel09Value;
    using AdasPdwLeftContourLineLevel01Value =
        ArgumentsAdasPdwLeftContourLineLevel01ValueChanged::Input_AdasPdwLeftContourLineLevel01Value;
    using AdasPdwLeftContourLineLevel02Value =
        ArgumentsAdasPdwLeftContourLineLevel02ValueChanged::Input_AdasPdwLeftContourLineLevel02Value;
    using AdasPdwLeftContourLineLevel03Value =
        ArgumentsAdasPdwLeftContourLineLevel03ValueChanged::Input_AdasPdwLeftContourLineLevel03Value;
    using AdasPdwLeftContourLineLevel04Value =
        ArgumentsAdasPdwLeftContourLineLevel04ValueChanged::Input_AdasPdwLeftContourLineLevel04Value;
    using AdasPdwLeftContourLineLevel05Value =
        ArgumentsAdasPdwLeftContourLineLevel05ValueChanged::Input_AdasPdwLeftContourLineLevel05Value;
    using AdasPdwLeftContourLineLevel06Value =
        ArgumentsAdasPdwLeftContourLineLevel06ValueChanged::Input_AdasPdwLeftContourLineLevel06Value;
    using AdasPdwLeftContourLineLevel07Value =
        ArgumentsAdasPdwLeftContourLineLevel07ValueChanged::Input_AdasPdwLeftContourLineLevel07Value;
    using AdasPdwRightContourLineLevel01Value =
        ArgumentsAdasPdwRightContourLineLevel01ValueChanged::Input_AdasPdwRightContourLineLevel01Value;
    using AdasPdwRightContourLineLevel02Value =
        ArgumentsAdasPdwRightContourLineLevel02ValueChanged::Input_AdasPdwRightContourLineLevel02Value;
    using AdasPdwRightContourLineLevel03Value =
        ArgumentsAdasPdwRightContourLineLevel03ValueChanged::Input_AdasPdwRightContourLineLevel03Value;
    using AdasPdwRightContourLineLevel04Value =
        ArgumentsAdasPdwRightContourLineLevel04ValueChanged::Input_AdasPdwRightContourLineLevel04Value;
    using AdasPdwRightContourLineLevel05Value =
        ArgumentsAdasPdwRightContourLineLevel05ValueChanged::Input_AdasPdwRightContourLineLevel05Value;
    using AdasPdwRightContourLineLevel06Value =
        ArgumentsAdasPdwRightContourLineLevel06ValueChanged::Input_AdasPdwRightContourLineLevel06Value;
    using AdasPdwRightContourLineLevel07Value =
        ArgumentsAdasPdwRightContourLineLevel07ValueChanged::Input_AdasPdwRightContourLineLevel07Value;

    using AdasPdwSystemFailureStatus = ArgumentsAdasPdwSystemFailureStatusChanged::Input_AdasPdwSystemFailureStatus;
    using IbuPdwSystemFailureStatus = ArgumentsIbuPdwSystemFailureStatusChanged::Input_IbuPdwSystemFailureStatus;
    using AdasPcaSystemFailureStatus = ArgumentsAdasPcaSystemFailureStatusChanged::Input_AdasPcaSystemFailureStatus;
    using NTFGroup7RccaFailure2Status = ArgumentsRccaFailure2StatusChanged::Input_NTFGroup7RccaFailure2Status;
    using NTFGroup7ExitSafetyFailure2Status = ArgumentsExitSafetyFailure2StatusChanged::Input_NTFGroup7ExitSafetyFailure2Status;
    using VCTopviewScaleStatus = ArgumentsVcTopviewScaleStatusChanged::Input_VCTopviewScaleStatus;
    using MraButtunDisplayStatus = ArgumentsMraButtunDisplayStatusChanged::Input_MraButtunDisplayStatus;
    using RspaButtunDisplayStatus = ArgumentsRspaButtunDisplayStatusChanged::Input_RspaButtunDisplayStatus;
    using VCRspaFunctionDisplayStatus = ArgumentsVcRspaFunctionDisplayStatusChanged::Input_VCRspaFunctionDisplayStatus;
    using PcaFrontLeftWarningDisplayStatus =
        ArgumentsPcaFrontLeftWarningDisplayStatusChanged::Input_PcaFrontLeftWarningDisplayStatus;
    using PcaFrontRightWarningDisplayStatus =
        ArgumentsPcaFrontRightWarningDisplayStatusChanged::Input_PcaFrontRightWarningDisplayStatus;
    using PcaRearLeftWarningDisplayStatus =
        ArgumentsPcaRearLeftWarningDisplayStatusChanged::Input_PcaRearLeftWarningDisplayStatus;
    using PcaRearRightWarningDisplayStatus =
        ArgumentsPcaRearRightWarningDisplayStatusChanged::Input_PcaRearRightWarningDisplayStatus;
    using INDAdasParkingControlSymbolStatus =
        ArgumentsAdasParkingControlTelltaleStatusChanged::Input_INDAdasParkingControlSymbolStatus;
    using PuFGroup7DriverAssistFailure10Status =
        ArgumentsEvent7GroupFailure10StatusChanged::Input_PuFGroup7DriverAssistFailure10Status;
    using PuFGroup7DriverAssistFailure11Status =
        ArgumentsEvent7GroupFailure11StatusChanged::Input_PuFGroup7DriverAssistFailure11Status;
    using CCWFailureStatus = ArgumentsEvent7GroupCCWFailureStatusChanged::Input_CCWFailureStatus;
    using PcaModeDisplayStatus = ArgumentsEvent2GroupChanged::Input_PcaModeDisplayStatus;
    using ClusterHMIDisplayStatus = ArgumentsClusterHMIDisplayStatusChanged::Input_ClusterHMIDisplayStatus;
    using RspaEntryAvnHmiDisplay = ArgumentsRspaEntryAvnHmiDisplayChanged::Input_RspaEntryAvnHmiDisplay;
    using PaFrontCenterWarnSoundStatus = ArgumentsWarningSoundChanged::Input_PaFrontCenterWarnSoundStatus;
    using PdwFrontCenterWarnSoundStatus = ArgumentsWarningSoundChanged::Input_PdwFrontCenterWarnSoundStatus;
    using CCWWarnSoundStatus = ArgumentsWarningSoundChanged::Input_CCWWarnSoundStatus;
    using CCWWarnSoundStatusFromSAC = ArgumentsWarningSoundChanged::Input_CCWWarnSoundStatusFromSAC;

    using PaWarnSoundStatus = ArgumentsSysWarnSoundChanged::Input_PaWarnSoundStatus;
    using PdwCheckSoundStatus = ArgumentsSysWarnSoundChanged::Input_PdwCheckSoundStatus;

    using ConfigAdasPDW = ArgumentsConfigChanged::Inter_ConfigAdasPDW;
    using ConfigNewRCCW = ArgumentsConfigChanged::Inter_ConfigNewRCCW;
    using ConfigAdasPrkPDW = ArgumentsConfigChanged::Inter_ConfigAdasPrkPDW;
    using ConfigBdcPDW = ArgumentsConfigChanged::Inter_ConfigBdcPDW;
    using VcRspaUnparkingLeftDirectionStatus =
        ArgumentsVcRspaUnparkingLeftDirectionStatusChanged::Input_VCRspaUnparkingLeftDirectionStatus;
    using VcRspaUnparkingRightDirectionStatus =
        ArgumentsVcRspaUnparkingRightDirectionStatusChanged::Input_VCRspaUnparkingRightDirectionStatus;
    using VCRspaAvailableStatus = ArgumentsVcRspaAvailableStatusChanged::Input_VCRspaAvailableStatus;
    using VCAdasParkingPathDisplayStatus = ArgumentsVcAdasParkingPathDisplayStatusChanged::Input_VCAdasParkingPathDisplayStatus;
    using MpaButtunDisplayStatus = ArgumentsMpaButtunDisplayStatusChanged::Input_MpaButtunDisplayStatus;
    using MpaRecordButtunDisplayStatus = ArgumentsMpaRecordButtunDisplayStatusChanged::Input_MpaRecordButtunDisplayStatus;
    using NTFGroup1AdasWarning1_1Status = ArgumentsBirdViewInfoChanged::Input_NTFGroup1AdasWarning1_1Status;
    using NTFGroup4AdasWarning1_1Status = ArgumentsBirdViewInfoChanged::Input_NTFGroup4AdasWarning1_1Status;
    using AdasPcaWarningPhraseStatus = ArgumentsEventPcaRccwWarningPopupChanged::Input_AdasPcaWarningPhraseStatus;
    using PuPvLabelStatus = ArgumentsEventPcaRccwWarningPopupChanged::Input_PuPvLabelStatus;
    VCTopviewControlStatus mVCTopviewControlStatus = VCTopviewControlStatus::OFF;

    AdasPdwWarningSoundMuteStatus mAdasPdwWarningSoundMuteStatus = AdasPdwWarningSoundMuteStatus::OFF;
    IbuPdwWarningSoundMuteStatus mIbuPdwWarningSoundMuteStatus = IbuPdwWarningSoundMuteStatus::OFF;

    AdasFrontMasterWarningDisplayStatus mAdasFrontMasterWarningDisplayStatus = AdasFrontMasterWarningDisplayStatus::OFF;
    IbuFrontMasterWarningDisplayStatus mIbuFrontMasterWarningDisplayStatus = IbuFrontMasterWarningDisplayStatus::OFF;
    AdasRearMasterWarningDisplayStatus mAdasRearMasterWarningDisplayStatus = AdasRearMasterWarningDisplayStatus::OFF;
    IbuRearMasterWarningDisplayStatus mIbuRearMasterWarningDisplayStatus = IbuRearMasterWarningDisplayStatus::OFF;
    AdasLeftMasterWarningDisplayStatus mAdasLeftMasterWarningDisplayStatus = AdasLeftMasterWarningDisplayStatus::OFF;
    AdasRightMasterWarningDisplayStatus mAdasRightMasterWarningDisplayStatus = AdasRightMasterWarningDisplayStatus::OFF;

    AdasPdwFrontContourLineLevel01Value mAdasPdwFrontContourLineLevel01Value = AdasPdwFrontContourLineLevel01Value::OFF;
    AdasPdwFrontContourLineLevel02Value mAdasPdwFrontContourLineLevel02Value = AdasPdwFrontContourLineLevel02Value::OFF;
    AdasPdwFrontContourLineLevel03Value mAdasPdwFrontContourLineLevel03Value = AdasPdwFrontContourLineLevel03Value::OFF;
    AdasPdwFrontContourLineLevel04Value mAdasPdwFrontContourLineLevel04Value = AdasPdwFrontContourLineLevel04Value::OFF;
    AdasPdwFrontContourLineLevel05Value mAdasPdwFrontContourLineLevel05Value = AdasPdwFrontContourLineLevel05Value::OFF;
    AdasPdwFrontContourLineLevel06Value mAdasPdwFrontContourLineLevel06Value = AdasPdwFrontContourLineLevel06Value::OFF;
    AdasPdwFrontContourLineLevel07Value mAdasPdwFrontContourLineLevel07Value = AdasPdwFrontContourLineLevel07Value::OFF;
    AdasPdwFrontContourLineLevel08Value mAdasPdwFrontContourLineLevel08Value = AdasPdwFrontContourLineLevel08Value::OFF;
    AdasPdwFrontContourLineLevel09Value mAdasPdwFrontContourLineLevel09Value = AdasPdwFrontContourLineLevel09Value::OFF;
    IbuPdwFrontContourLineLevel01Value mIbuPdwFrontContourLineLevel01Value = IbuPdwFrontContourLineLevel01Value::OFF;
    IbuPdwFrontContourLineLevel02Value mIbuPdwFrontContourLineLevel02Value = IbuPdwFrontContourLineLevel02Value::OFF;
    IbuPdwFrontContourLineLevel03Value mIbuPdwFrontContourLineLevel03Value = IbuPdwFrontContourLineLevel03Value::OFF;
    IbuPdwFrontContourLineLevel04Value mIbuPdwFrontContourLineLevel04Value = IbuPdwFrontContourLineLevel04Value::OFF;
    IbuPdwFrontContourLineLevel05Value mIbuPdwFrontContourLineLevel05Value = IbuPdwFrontContourLineLevel05Value::OFF;
    IbuPdwFrontContourLineLevel06Value mIbuPdwFrontContourLineLevel06Value = IbuPdwFrontContourLineLevel06Value::OFF;
    IbuPdwFrontContourLineLevel07Value mIbuPdwFrontContourLineLevel07Value = IbuPdwFrontContourLineLevel07Value::OFF;
    IbuPdwFrontContourLineLevel08Value mIbuPdwFrontContourLineLevel08Value = IbuPdwFrontContourLineLevel08Value::OFF;
    IbuPdwFrontContourLineLevel09Value mIbuPdwFrontContourLineLevel09Value = IbuPdwFrontContourLineLevel09Value::OFF;
    AdasPdwRearContourLineLevel01Value mAdasPdwRearContourLineLevel01Value = AdasPdwRearContourLineLevel01Value::OFF;
    AdasPdwRearContourLineLevel02Value mAdasPdwRearContourLineLevel02Value = AdasPdwRearContourLineLevel02Value::OFF;
    AdasPdwRearContourLineLevel03Value mAdasPdwRearContourLineLevel03Value = AdasPdwRearContourLineLevel03Value::OFF;
    AdasPdwRearContourLineLevel04Value mAdasPdwRearContourLineLevel04Value = AdasPdwRearContourLineLevel04Value::OFF;
    AdasPdwRearContourLineLevel05Value mAdasPdwRearContourLineLevel05Value = AdasPdwRearContourLineLevel05Value::OFF;
    AdasPdwRearContourLineLevel06Value mAdasPdwRearContourLineLevel06Value = AdasPdwRearContourLineLevel06Value::OFF;
    AdasPdwRearContourLineLevel07Value mAdasPdwRearContourLineLevel07Value = AdasPdwRearContourLineLevel07Value::OFF;
    AdasPdwRearContourLineLevel08Value mAdasPdwRearContourLineLevel08Value = AdasPdwRearContourLineLevel08Value::OFF;
    AdasPdwRearContourLineLevel09Value mAdasPdwRearContourLineLevel09Value = AdasPdwRearContourLineLevel09Value::OFF;
    IbuPdwRearContourLineLevel01Value mIbuPdwRearContourLineLevel01Value = IbuPdwRearContourLineLevel01Value::OFF;
    IbuPdwRearContourLineLevel02Value mIbuPdwRearContourLineLevel02Value = IbuPdwRearContourLineLevel02Value::OFF;
    IbuPdwRearContourLineLevel03Value mIbuPdwRearContourLineLevel03Value = IbuPdwRearContourLineLevel03Value::OFF;
    IbuPdwRearContourLineLevel04Value mIbuPdwRearContourLineLevel04Value = IbuPdwRearContourLineLevel04Value::OFF;
    IbuPdwRearContourLineLevel05Value mIbuPdwRearContourLineLevel05Value = IbuPdwRearContourLineLevel05Value::OFF;
    IbuPdwRearContourLineLevel06Value mIbuPdwRearContourLineLevel06Value = IbuPdwRearContourLineLevel06Value::OFF;
    IbuPdwRearContourLineLevel07Value mIbuPdwRearContourLineLevel07Value = IbuPdwRearContourLineLevel07Value::OFF;
    IbuPdwRearContourLineLevel08Value mIbuPdwRearContourLineLevel08Value = IbuPdwRearContourLineLevel08Value::OFF;
    IbuPdwRearContourLineLevel09Value mIbuPdwRearContourLineLevel09Value = IbuPdwRearContourLineLevel09Value::OFF;
    AdasPdwLeftContourLineLevel01Value mAdasPdwLeftContourLineLevel01Value = AdasPdwLeftContourLineLevel01Value::OFF;
    AdasPdwLeftContourLineLevel02Value mAdasPdwLeftContourLineLevel02Value = AdasPdwLeftContourLineLevel02Value::OFF;
    AdasPdwLeftContourLineLevel03Value mAdasPdwLeftContourLineLevel03Value = AdasPdwLeftContourLineLevel03Value::OFF;
    AdasPdwLeftContourLineLevel04Value mAdasPdwLeftContourLineLevel04Value = AdasPdwLeftContourLineLevel04Value::OFF;
    AdasPdwLeftContourLineLevel05Value mAdasPdwLeftContourLineLevel05Value = AdasPdwLeftContourLineLevel05Value::OFF;
    AdasPdwLeftContourLineLevel06Value mAdasPdwLeftContourLineLevel06Value = AdasPdwLeftContourLineLevel06Value::OFF;
    AdasPdwLeftContourLineLevel07Value mAdasPdwLeftContourLineLevel07Value = AdasPdwLeftContourLineLevel07Value::OFF;
    AdasPdwRightContourLineLevel01Value mAdasPdwRightContourLineLevel01Value = AdasPdwRightContourLineLevel01Value::OFF;
    AdasPdwRightContourLineLevel02Value mAdasPdwRightContourLineLevel02Value = AdasPdwRightContourLineLevel02Value::OFF;
    AdasPdwRightContourLineLevel03Value mAdasPdwRightContourLineLevel03Value = AdasPdwRightContourLineLevel03Value::OFF;
    AdasPdwRightContourLineLevel04Value mAdasPdwRightContourLineLevel04Value = AdasPdwRightContourLineLevel04Value::OFF;
    AdasPdwRightContourLineLevel05Value mAdasPdwRightContourLineLevel05Value = AdasPdwRightContourLineLevel05Value::OFF;
    AdasPdwRightContourLineLevel06Value mAdasPdwRightContourLineLevel06Value = AdasPdwRightContourLineLevel06Value::OFF;
    AdasPdwRightContourLineLevel07Value mAdasPdwRightContourLineLevel07Value = AdasPdwRightContourLineLevel07Value::OFF;

    AdasPdwSystemFailureStatus mAdasPdwSystemFailureStatus = AdasPdwSystemFailureStatus::OFF;
    IbuPdwSystemFailureStatus mIbuPdwSystemFailureStatus = IbuPdwSystemFailureStatus::OFF;
    AdasPcaSystemFailureStatus mAdasPcaSystemFailureStatus = AdasPcaSystemFailureStatus::OFF;
    NTFGroup7RccaFailure2Status mNTFGroup7RccaFailure2Status = NTFGroup7RccaFailure2Status::OFF;
    NTFGroup7ExitSafetyFailure2Status mNTFGroup7ExitSafetyFailure2Status = NTFGroup7ExitSafetyFailure2Status::OFF;
    VCTopviewScaleStatus mVCTopviewScaleStatus = VCTopviewScaleStatus::OFF;
    MraButtunDisplayStatus mMraButtunDisplayStatus = MraButtunDisplayStatus::OFF;
    RspaButtunDisplayStatus mRspaButtunDisplayStatus = RspaButtunDisplayStatus::OFF;
    VCRspaFunctionDisplayStatus mVCRspaFunctionDisplayStatus = VCRspaFunctionDisplayStatus::OFF;
    PcaFrontLeftWarningDisplayStatus mPcaFrontLeftWarningDisplayStatus = PcaFrontLeftWarningDisplayStatus::OFF;
    PcaFrontRightWarningDisplayStatus mPcaFrontRightWarningDisplayStatus = PcaFrontRightWarningDisplayStatus::OFF;
    PcaRearLeftWarningDisplayStatus mPcaRearLeftWarningDisplayStatus = PcaRearLeftWarningDisplayStatus::OFF;
    PcaRearRightWarningDisplayStatus mPcaRearRightWarningDisplayStatus = PcaRearRightWarningDisplayStatus::OFF;
    INDAdasParkingControlSymbolStatus mINDAdasParkingControlSymbolStatus = INDAdasParkingControlSymbolStatus::OFF;
    HUInt64 mINDAdasParkingProgressBarValue = 0;

    PuFGroup7DriverAssistFailure10Status mPuFGroup7DriverAssistFailure10Status = PuFGroup7DriverAssistFailure10Status::OFF;
    PuFGroup7DriverAssistFailure11Status mPuFGroup7DriverAssistFailure11Status = PuFGroup7DriverAssistFailure11Status::OFF;
    CCWFailureStatus mCCWFailureStatus = CCWFailureStatus::OFF;
    PcaModeDisplayStatus mPcaModeDisplayStatus = PcaModeDisplayStatus::OFF;
    ClusterHMIDisplayStatus mClusterHMIDisplayStatus = ClusterHMIDisplayStatus::OFF;
    RspaEntryAvnHmiDisplay mRspaEntryAvnHmiDisplay = RspaEntryAvnHmiDisplay::OFF;

    PaFrontCenterWarnSoundStatus mPaFrontCenterWarnSoundStatus = PaFrontCenterWarnSoundStatus::OFF;
    PdwFrontCenterWarnSoundStatus mPdwFrontCenterWarnSoundStatus = PdwFrontCenterWarnSoundStatus::OFF;
    CCWWarnSoundStatus mCCWWarnSoundStatus = CCWWarnSoundStatus::OFF;
    CCWWarnSoundStatusFromSAC mCCWWarnSoundStatusFromSAC = CCWWarnSoundStatusFromSAC::OFF;
    PaWarnSoundStatus mPaWarnSoundStatus = PaWarnSoundStatus::OFF;
    PdwCheckSoundStatus mPdwCheckSoundStatus = PdwCheckSoundStatus::OFF;

    ConfigAdasPDW mConfigAdasPDW = ConfigAdasPDW::OFF;
    ConfigNewRCCW mConfigNewRCCW = ConfigNewRCCW::OFF;
    ConfigAdasPrkPDW mConfigAdasPrkPDW = ConfigAdasPrkPDW::OFF;
    ConfigBdcPDW mConfigBdcPDW = ConfigBdcPDW::OFF;
    static constexpr HUInt64 kInterval_1000ms = 1000;

    AddressValueList mActuatorOutputAddrList;
    VcRspaUnparkingLeftDirectionStatus mVcRspaUnparkingLeftDirectionStatus = VcRspaUnparkingLeftDirectionStatus::OFF;
    VcRspaUnparkingRightDirectionStatus mVcRspaUnparkingRightDirectionStatus = VcRspaUnparkingRightDirectionStatus::OFF;
    HUInt64 mRspaParkingSlotValue = 0;
    SFCADAS_PARKING_2_0ConstantRspaToggleInformationStat mConstantRspaToggleInformationStat =
        SFCADAS_PARKING_2_0ConstantRspaToggleInformationStat::ON;
    VCRspaAvailableStatus mVCRspaAvailableStatus = VCRspaAvailableStatus::OFF;
    SFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus mInterRspaParkingDirectionInformationStatus =
        SFCADAS_PARKING_2_0Inter_RspaParkingDirectionInformationStatus::FORWARD;
    SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus mInterRspaParkingSlotLocationStatus =
        SFCADAS_PARKING_2_0Inter_RspaParkingSlotLocationStatus::RIGHT;
    SFCADAS_PARKING_2_0Inter_RspaOperationInformationStatus mInterRspaOperationInformationStatus =
        SFCADAS_PARKING_2_0Inter_RspaOperationInformationStatus::OFF;
    VCAdasParkingPathDisplayStatus mVCAdasParkingPathDisplayStatus = VCAdasParkingPathDisplayStatus::OFF;
    MpaButtunDisplayStatus mMpaButtunDisplayStatus = MpaButtunDisplayStatus::OFF;
    MpaRecordButtunDisplayStatus mMpaRecordButtunDisplayStatus = MpaRecordButtunDisplayStatus::OFF;
    HUInt64 mIndAdasParkingRemainedDistanceValue = 0xFFFF;
    NTFGroup1AdasWarning1_1Status mNTFGroup1AdasWarning1_1Status = NTFGroup1AdasWarning1_1Status::OFF;
    NTFGroup4AdasWarning1_1Status mNTFGroup4AdasWarning1_1Status = NTFGroup4AdasWarning1_1Status::OFF;
    AdasPcaWarningPhraseStatus mAdasPcaWarningPhraseStatus = AdasPcaWarningPhraseStatus::OFF;
    PuPvLabelStatus mPuPvLabelStatus = PuPvLabelStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_ADAS_PARKING_2_0_H
