/**
 * @file ADAS_Parking_NEW.hpp
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
#ifndef SFSS_ADAS_Parking_NEW_H
#define SFSS_ADAS_Parking_NEW_H

#define DLOG_ENABLED gEnableSFCLog

#include "ADAS_Parking_NEWBase.hpp"

namespace ccos {

// SFC Version : 29.0.1
// Reference : [AD090] ADAS_Parking_NEW. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_Parking_NEW : public ADAS_Parking_NEWBase {
public:
    ADAS_Parking_NEW() = default;
    ~ADAS_Parking_NEW() override = default;
    ADAS_Parking_NEW(const ADAS_Parking_NEW& other) = delete;
    ADAS_Parking_NEW(ADAS_Parking_NEW&& other) noexcept = delete;
    ADAS_Parking_NEW& operator=(const ADAS_Parking_NEW& other) = delete;
    ADAS_Parking_NEW& operator=(ADAS_Parking_NEW&& other) noexcept = delete;

    void onInitialize() override {
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure10LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure10LinkedSoundType(
            SFCADAS_PARKING_NEWEventEvent7GroupFailure10LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure10LinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure11LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure11LinkedSoundType(
            SFCADAS_PARKING_NEWEventEvent7GroupFailure11LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWEventEvent7GroupFailure11LinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureLinkedSoundType(
            SFCADAS_PARKING_NEWEventEvent7GroupCCWFailureLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureLinkedSoundRepeatCount(1);
        setSFCADAS_PARKING_NEWSoundWarningLevelType(SFCADAS_PARKING_NEWSoundWarningLevelType::INFINITE);
        setSFCADAS_PARKING_NEWSoundPASysWarnSoundType(SFCADAS_PARKING_NEWSoundPASysWarnSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWSoundPASysWarnSoundRepeatCount(1);
        setSFCADAS_PARKING_NEWSoundWarningSensorFailID("SND_ADAS_PDWSensorFail_Fixed");
        setSFCADAS_PARKING_NEWSoundWarningSensorFailType(SFCADAS_PARKING_NEWSoundWarningSensorFailType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWSoundWarningSensorFailRepeatCount(1);
        setSFCADAS_PARKING_NEWSoundWarningTailgateOpenedID("SND_ADAS_PDWTailgate_Opened");
        setSFCADAS_PARKING_NEWSoundWarningTailgateOpenedType(SFCADAS_PARKING_NEWSoundWarningTailgateOpenedType::INTERVAL);
        setSFCADAS_PARKING_NEWSoundWarningTailgateOpenedInterval(1000);
        setSFCADAS_PARKING_NEWEventEvent7GroupTimeoutID("E53016");
        setSFCADAS_PARKING_NEWEventEvent7GroupTimeoutLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_NEWEventEvent7GroupTimeoutLinkedSoundType(
            SFCADAS_PARKING_NEWEventEvent7GroupTimeoutLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_NEWEventEvent7GroupTimeoutLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAll();
    }

    void onCarBodyDisplayStatusChanged(const ArgumentsCarBodyDisplayStatusChanged& args) {
        mAdasCarBodyDisplayStatus = args.mInput_AdasCarBodyDisplayStatus;
        mIbuCarBodyDisplayStatus = args.mInput_IbuCarBodyDisplayStatus;
        mPuPvHostVehicleStatus = args.mInput_PuPvHostVehicleStatus;
        mCCWCarBodyDisplayStatus = args.mInput_CCWCarBodyDisplayStatus;
        mCCWCarBodyDisplayStatusFromSAC = args.mInput_CCWCarBodyDisplayStatusFromSAC;
        updateCarBody();
    }

    void onParkingDisplayStatusChanged(const ArgumentsParkingDisplayStatusChanged& args) {
        mAdasFrontLeftWarning3PartDisplayStatus = args.mInput_AdasFrontLeftWarning3PartDisplayStatus;
        mAdasFrontCenterWarning3PartDisplayStatus = args.mInput_AdasFrontCenterWarning3PartDisplayStatus;
        mAdasFrontRightWaring3PartDisplayStatus = args.mInput_AdasFrontRightWaring3PartDisplayStatus;
        mIbuFrontLeftWarning3PartDisplayStatus = args.mInput_IbuFrontLeftWarning3PartDisplayStatus;
        mIbuFrontCenterWarning3PartDisplayStatus = args.mInput_IbuFrontCenterWarning3PartDisplayStatus;
        mIbuFrontRightWaring3PartDisplayStatus = args.mInput_IbuFrontRightWarning3PartDisplayStatus;

        mAdasRearLeftWarning3PartDisplayStatus = args.mInput_AdasRearLeftWarning3PartDisplayStatus;
        mAdasRearCenterWarning3PartDisplayStatus = args.mInput_AdasRearCenterWarning3PartDisplayStatus;
        mAdasRearRightWarning3PartDisplayStatus = args.mInput_AdasRearRightWarning3PartDisplayStatus;
        mIbuRearLeftWarning3PartDisplayStatus = args.mInput_IbuRearLeftWarning3PartDisplayStatus;
        mIbuRearCenterWarning3PartDisplayStatus = args.mInput_IbuRearCenterWarning3PartDisplayStatus;
        mIbuRearRightWarning3PartDisplayStatus = args.mInput_IbuRearRightWarning3PartDisplayStatus;

        mAdasLeftSideWarning1PartDisplayStatus = args.mInput_AdasLeftSideWarning1PartDisplayStatus;
        mAdasRightSideWarning1PartDisplayStatus = args.mInput_AdasRightSideWarning1PartDisplayStatus;

        mAdasLeftSideFrontWarning2PartDisplayStatus = args.mInput_AdasLeftSideFrontWarning2PartDisplayStatus;
        mAdasRightSideFrontWarning2PartDisplayStatus = args.mInput_AdasRightSideFrontWarning2PartDisplayStatus;
        mAdasLeftSideRearWarning2PartDisplayStatus = args.mInput_AdasLeftSideRearWarning2PartDisplayStatus;
        mAdasRightSideRearWarning2PartDisplayStatus = args.mInput_AdasRightSideRearWarning2PartDisplayStatus;
        updateFrontWarning3Part();
        updateRearWarning3Part();
        updateSideWarning1Part();
        updateSideWarning2Part();
    }

    void onMasterWarningDisplayStatusChanged(const ArgumentsMasterWarningDisplayStatusChanged& args) {
        mAdasFrontMasterWarningDisplayStatus = args.mInput_AdasFrontMasterWarningDisplayStatus;
        mAdasRearMasterWarningDisplayStatus = args.mInput_AdasRearMasterWarningDisplayStatus;
        mAdasLeftMasterWarningDisplayStatus = args.mInput_AdasLeftMasterWarningDisplayStatus;
        mAdasRightMasterWarningDisplayStatus = args.mInput_AdasRightMasterWarningDisplayStatus;
        mIbuFrontMasterWarningDisplayStatus = args.mInput_IbuFrontMasterWarningDisplayStatus;
        mIbuRearMasterWarningDisplayStatus = args.mInput_IbuRearMasterWarningDisplayStatus;
        updateMasterWarning();
    }

    void onPcaBoundaryLaneStatusChanged(const ArgumentsPcaBoundaryLaneStatusChanged& args) {
        mAdasPcaFrontBoundaryLaneStatus = args.mInput_AdasPcaFrontBoundaryLaneStatus;
        mAdasPcaRearBoundaryLaneStatus = args.mInput_AdasPcaRearBoundaryLaneStatus;
        mAdasPcaLeftBoundaryLaneStatus = args.mInput_AdasPcaLeftBoundaryLaneStatus;
        mAdasPcaRightBoundaryLaneStatus = args.mInput_AdasPcaRightBoundaryLaneStatus;
        updateBoundaryLane();
    }

    void onPcaWarningPhraseStatusChanged(const ArgumentsPcaWarningPhraseStatusChanged& args) {
        mAdasPcaWarningPhraseStatus = args.mInput_AdasPcaWarningPhraseStatus;
        mPuPvLabelStatus = args.mInput_PuPvLabelStatus;
        mPdwWarningPhraseStatus = args.mInput_PdwWarningPhraseStatus;
        updatePhrase();
    }

    void onPuPvRearObjectMovingDirectionStatus(const ArgumentsPuPvRearObjectMovingDirectionStatus& args) {
        mPuPvRearLeftObjectMovingDirectionStatus = args.mInput_PuPvRearLeftObjectMovingDirectionStatus;
        mPuPvRearRightObjectMovingDirectionStatus = args.mInput_PuPvRearRightObjectMovingDirectionStatus;
        updateRccwWarning();
    }

    void onAdasPdwSystemFailureStatusChanged(const ArgumentsAdasPdwSystemFailureStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_AdasPdwSystemFailureStatus) == true) {
            mAdasPdwSystemFailureStatus = AdasPdwSystemFailureStatus::UNHANDLED_TIMEOUT;
        } else {
            mAdasPdwSystemFailureStatus = args.mInput_AdasPdwSystemFailureStatus;
        }
        updatePdwSystemFailurePRK();
        updatePdwSystemFailure();
    }

    void onIbuPdwSystemFailureStatusChanged(const ArgumentsIbuPdwSystemFailureStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_IbuPdwSystemFailureStatus) == true) {
            mIbuPdwSystemFailureStatus = IbuPdwSystemFailureStatus::UNHANDLED_TIMEOUT;
        } else {
            mIbuPdwSystemFailureStatus = args.mInput_IbuPdwSystemFailureStatus;
        }
        updatePdwSystemFailureIBU();
        updatePdwSystemFailure();
    }

    void onAdasPcaSystemFailureStatusChanged(const ArgumentsAdasPcaSystemFailureStatusChanged& args) {
        mAdasPcaSystemFailureStatus = args.mInput_AdasPcaSystemFailureStatus;
        updatePcaSystemFailure();
    }

    void onRccwSysFailStatusChanged(const ArgumentsRccwSysFailStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PuFGroup7RccaFailureStatus) == true) {
            mPuFGroup7RccaFailureStatus = PuFGroup7RccaFailureStatus::UNHANDLED_TIMEOUT;
        } else {
            mPuFGroup7RccaFailureStatus = args.mInput_PuFGroup7RccaFailureStatus;
        }
        mConfigNewRCCW = args.mInter_ConfigNewRCCW;
        updateRccaFailure();
    }

    void onExitSafetyFailureStatusChanged(const ArgumentsExitSafetyFailureStatusChanged& args) {
        mPuFGroup7ExitSafetyFailureStatus = args.mInput_PuFGroup7ExitSafetyFailureStatus;
        updateExitSafetyFailure();
    }

    void onFailure10StatusChanged(const ArgumentsFailure10StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure10Status) == true) {
            mPuFGroup7DriverAssistFailure10Status = PuFGroup7DriverAssistFailure10Status::UNHANDLED_TIMEOUT;
        } else {
            mPuFGroup7DriverAssistFailure10Status = args.mInput_PuFGroup7DriverAssistFailure10Status;
        }
        updateEventFailure10();
        updateEvent7GroupTimeout();
    }

    void onFailure11StatusChanged(const ArgumentsFailure11StatusChanged& args) {
        if (ISTIMEOUT(args.mInput_PuFGroup7DriverAssistFailure11Status) == true) {
            mPuFGroup7DriverAssistFailure11Status = PuFGroup7DriverAssistFailure11Status::UNHANDLED_TIMEOUT;
        } else {
            mPuFGroup7DriverAssistFailure11Status = args.mInput_PuFGroup7DriverAssistFailure11Status;
        }
        updateEventFailure11();
        updateEvent7GroupTimeout();
    }

    void onCCWFailureStatusChanged(const ArgumentsCCWFailureStatusChanged& args) {
        mCCWFailureStatus = args.mInput_CCWFailureStatus;
        mCCWFailureStatusFromSAC = args.mInput_CCWFailureStatusFromSAC;
        updateEventCCWFailure();
    }

    void onEvent4GroupChanged(const ArgumentsEvent4GroupChanged& args) {
        mAdasPopupBoundaryLaneColorStatus = args.mInput_AdasPopupBoundaryLaneColorStatus;
        mIbuPopupBoundaryLaneColorStatus = args.mInput_IbuPopupBoundaryLaneColorStatus;
        mPuPvOutlineColorStatus = args.mInput_PuPvOutlineColorStatus;
        mCCWPopupBoundaryLaneColorStatus = args.mInput_CCWPopupBoundaryLaneColorStatus;
        mCCWPopupBoundaryLaneColorStatusFromSAC = args.mInput_CCWPopupBoundaryLaneColorStatusFromSAC;
        updateEvent4Group();
    }

    void onEvent2GroupChanged(const ArgumentsEvent2GroupChanged& args) {
        mPcaModeDisplayStatus = args.mInput_PcaModeDisplayStatus;
        updateEvent2Group();
    }

    void onWarnSoundChanged(const ArgumentsWarnSoundChanged& args) {
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

    void onCCWWarningChanged(const ArgumentsCCWWarningChanged& args) {
        mCCWRearWarningLevelStatus = args.mInput_CCWRearWarningLevelStatus;
        mCCWRearWarningLevelStatusFromSAC = args.mInput_CCWRearWarningLevelStatusFromSAC;
        updateParkCCWRearWarn();
    }

    void onADASPDWStatusChanged(const ArgumentsADASPDWStatusChanged& arg) {
        mConfigAdasPDW = arg.mInter_ConfigAdasPDW;
    }

    void onADASPRKPDWStatusChanged(const ArgumentsADASPRKPDWStatusChanged& arg) {
        mConfigAdasPrkPDW = arg.mInter_ConfigAdasPrkPDW;
        updatePdwSystemFailure();
        updateEvent7GroupTimeout();
    }

    void onBDCPDWStatusChanged(const ArgumentsBDCPDWStatusChanged& arg) {
        mConfigBdcPDW = arg.mInter_ConfigBdcPDW;
        updatePdwSystemFailure();
        updateEvent7GroupTimeout();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateFrontWarning3Part();
        updateRearWarning3Part();
        updateSideWarning1Part();
        updateSideWarning2Part();
        updateMasterWarning();
        updateBoundaryLane();
        updatePdwSystemFailurePRK();
        updatePdwSystemFailureIBU();
        updatePdwSystemFailure();
        updatePcaSystemFailure();
        updateRccaFailure();
        updateExitSafetyFailure();
        updateParkCCWRearWarn();
        updateEvent4Group();
        updateEventFailure10();
        updateEventFailure11();
        updateEventCCWFailure();
        updateEvent2Group();
        updateEvent7GroupTimeout();
        updateSoundWarn();
        updateSoundPASysWarn();
        updateSoundPDWSensorFail();
        updateSoundPDWTailgateOpened();
    }

    // 4.1.1
    void updateCarBody() {
        if (mAdasCarBodyDisplayStatus == AdasCarBodyDisplayStatus::ON ||
            mIbuCarBodyDisplayStatus == IbuCarBodyDisplayStatus::ON || mPuPvHostVehicleStatus == PuPvHostVehicleStatus::ON ||
            mCCWCarBodyDisplayStatus == CCWCarBodyDisplayStatus::ON ||
            mCCWCarBodyDisplayStatusFromSAC == CCWCarBodyDisplayStatusFromSAC::ON) {
            setSFCADAS_PARKING_NEWConstantParkCarBodyDisplayStat(SFCADAS_PARKING_NEWConstantParkCarBodyDisplayStat::ON);
        } else {
            setSFCADAS_PARKING_NEWConstantParkCarBodyDisplayStat(SFCADAS_PARKING_NEWConstantParkCarBodyDisplayStat::OFF);
        }
    }

    // 4.1.2 ~ 4
    void updateFrontWarning3Part() {
        SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat statFL =
            SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat statFC =
            SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat statFR =
            SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasFrontLeftWarning3PartDisplayStatus == AdasFrontLeftWarning3PartDisplayStatus::WARN_3RD ||
                mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_3RD) {
                statFL = SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::WARN_3RD;
            } else if (mAdasFrontLeftWarning3PartDisplayStatus == AdasFrontLeftWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_2ND) {
                statFL = SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::WARN_2ND;
            } else if (mAdasFrontLeftWarning3PartDisplayStatus == AdasFrontLeftWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_1ST) {
                statFL = SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::WARN_1ST;
            } else if (mAdasFrontLeftWarning3PartDisplayStatus == AdasFrontLeftWarning3PartDisplayStatus::GRAY ||
                       mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::GRAY) {
                statFL = SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::GRAY;
            } else {
                statFL = SFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat::OFF;
            }

            if (mAdasFrontCenterWarning3PartDisplayStatus == AdasFrontCenterWarning3PartDisplayStatus::WARN_3RD ||
                mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_3RD) {
                statFC = SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::WARN_3RD;
            } else if (mAdasFrontCenterWarning3PartDisplayStatus == AdasFrontCenterWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_2ND) {
                statFC = SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::WARN_2ND;
            } else if (mAdasFrontCenterWarning3PartDisplayStatus == AdasFrontCenterWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_1ST) {
                statFC = SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::WARN_1ST;
            } else if (mAdasFrontCenterWarning3PartDisplayStatus == AdasFrontCenterWarning3PartDisplayStatus::GRAY ||
                       mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::GRAY) {
                statFC = SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::GRAY;
            } else {
                statFC = SFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat::OFF;
            }

            if (mAdasFrontRightWaring3PartDisplayStatus == AdasFrontRightWaring3PartDisplayStatus::WARN_3RD ||
                mIbuFrontRightWaring3PartDisplayStatus == IbuFrontRightWaring3PartDisplayStatus::WARN_3RD) {
                statFR = SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::WARN_3RD;
            } else if (mAdasFrontRightWaring3PartDisplayStatus == AdasFrontRightWaring3PartDisplayStatus::WARN_2ND ||
                       mIbuFrontRightWaring3PartDisplayStatus == IbuFrontRightWaring3PartDisplayStatus::WARN_2ND) {
                statFR = SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::WARN_2ND;
            } else if (mAdasFrontRightWaring3PartDisplayStatus == AdasFrontRightWaring3PartDisplayStatus::WARN_1ST ||
                       mIbuFrontRightWaring3PartDisplayStatus == IbuFrontRightWaring3PartDisplayStatus::WARN_1ST) {
                statFR = SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::WARN_1ST;
            } else if (mAdasFrontRightWaring3PartDisplayStatus == AdasFrontRightWaring3PartDisplayStatus::GRAY ||
                       mIbuFrontRightWaring3PartDisplayStatus == IbuFrontRightWaring3PartDisplayStatus::GRAY) {
                statFR = SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::GRAY;
            } else {
                statFR = SFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat::OFF;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkFrontLeftWarning3PartStat(statFL);
        setSFCADAS_PARKING_NEWConstantParkFrontCenterWarning3PartStat(statFC);
        setSFCADAS_PARKING_NEWConstantParkFrontRightWarning3PartStat(statFR);
    }

    // 4.1.5 ~ 7
    void updateRearWarning3Part() {
        SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat statRL =
            SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat statRC =
            SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat statRR =
            SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::OFF;

        if (mIsIgnOn == true) {
            if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_3RD) {
                statRL = SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::WARN_3RD;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_2ND) {
                statRL = SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::WARN_2ND;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_1ST) {
                statRL = SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::WARN_1ST;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::GRAY ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::GRAY) {
                statRL = SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::GRAY;
            } else {
                statRL = SFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat::OFF;
            }

            if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_3RD) {
                statRC = SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::WARN_3RD;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_2ND) {
                statRC = SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::WARN_2ND;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_1ST) {
                statRC = SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::WARN_1ST;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::GRAY ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::GRAY) {
                statRC = SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::GRAY;
            } else {
                statRC = SFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat::OFF;
            }

            if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_3RD) {
                statRR = SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::WARN_3RD;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_2ND) {
                statRR = SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::WARN_2ND;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_1ST) {
                statRR = SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::WARN_1ST;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::GRAY ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::GRAY) {
                statRR = SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::GRAY;
            } else {
                statRR = SFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat::OFF;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkRearLeftWarning3PartStat(statRL);
        setSFCADAS_PARKING_NEWConstantParkRearCenterWarning3PartStat(statRC);
        setSFCADAS_PARKING_NEWConstantParkRearRightWarning3PartStat(statRR);
    }

    // 4.1.8 ~ 9
    void updateSideWarning1Part() {
        SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat statLS =
            SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat statRS =
            SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::OFF;

        if (mIsIgnOn == true) {
            // 4.1.8
            switch (mAdasLeftSideWarning1PartDisplayStatus) {
                case AdasLeftSideWarning1PartDisplayStatus::WARN_1ST:
                    statLS = SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::WARN_1ST;
                    break;
                case AdasLeftSideWarning1PartDisplayStatus::WARN_2ND:
                    statLS = SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::WARN_2ND;
                    break;
                case AdasLeftSideWarning1PartDisplayStatus::WARN_3RD:
                    statLS = SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::WARN_3RD;
                    break;
                case AdasLeftSideWarning1PartDisplayStatus::GRAY:
                    statLS = SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::GRAY;
                    break;
                default:
                    statLS = SFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat::OFF;
                    break;
            }
            // 4.1.9
            switch (mAdasRightSideWarning1PartDisplayStatus) {
                case AdasRightSideWarning1PartDisplayStatus::WARN_1ST:
                    statRS = SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::WARN_1ST;
                    break;
                case AdasRightSideWarning1PartDisplayStatus::WARN_2ND:
                    statRS = SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::WARN_2ND;
                    break;
                case AdasRightSideWarning1PartDisplayStatus::WARN_3RD:
                    statRS = SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::WARN_3RD;
                    break;
                case AdasRightSideWarning1PartDisplayStatus::GRAY:
                    statRS = SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::GRAY;
                    break;
                default:
                    statRS = SFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkLeftSideWarning1PartStat(statLS);
        setSFCADAS_PARKING_NEWConstantParkRightSideWarning1PartStat(statRS);
    }

    // 4.1.10 ~ 13
    void updateSideWarning2Part() {
        SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat statLSF =
            SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat statRSF =
            SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat statLSR =
            SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat statRSR =
            SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::OFF;

        if (mIsIgnOn == true) {
            // 4.1.10
            switch (mAdasLeftSideFrontWarning2PartDisplayStatus) {
                case AdasLeftSideFrontWarning2PartDisplayStatus::WARN_1ST:
                    statLSF = SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::WARN_1ST;
                    break;
                case AdasLeftSideFrontWarning2PartDisplayStatus::WARN_2ND:
                    statLSF = SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::WARN_2ND;
                    break;
                case AdasLeftSideFrontWarning2PartDisplayStatus::WARN_3RD:
                    statLSF = SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::WARN_3RD;
                    break;
                case AdasLeftSideFrontWarning2PartDisplayStatus::GRAY:
                    statLSF = SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::GRAY;
                    break;
                default:
                    statLSF = SFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat::OFF;
                    break;
            }
            // 4.1.11
            switch (mAdasRightSideFrontWarning2PartDisplayStatus) {
                case AdasRightSideFrontWarning2PartDisplayStatus::WARN_1ST:
                    statRSF = SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::WARN_1ST;
                    break;
                case AdasRightSideFrontWarning2PartDisplayStatus::WARN_2ND:
                    statRSF = SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::WARN_2ND;
                    break;
                case AdasRightSideFrontWarning2PartDisplayStatus::WARN_3RD:
                    statRSF = SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::WARN_3RD;
                    break;
                case AdasRightSideFrontWarning2PartDisplayStatus::GRAY:
                    statRSF = SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::GRAY;
                    break;
                default:
                    statRSF = SFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat::OFF;
                    break;
            }
            // 4.1.12
            switch (mAdasLeftSideRearWarning2PartDisplayStatus) {
                case AdasLeftSideRearWarning2PartDisplayStatus::WARN_1ST:
                    statLSR = SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::WARN_1ST;
                    break;
                case AdasLeftSideRearWarning2PartDisplayStatus::WARN_2ND:
                    statLSR = SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::WARN_2ND;
                    break;
                case AdasLeftSideRearWarning2PartDisplayStatus::WARN_3RD:
                    statLSR = SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::WARN_3RD;
                    break;
                case AdasLeftSideRearWarning2PartDisplayStatus::GRAY:
                    statLSR = SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::GRAY;
                    break;
                default:
                    statLSR = SFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat::OFF;
                    break;
            }
            // 4.1.13
            switch (mAdasRightSideRearWarning2PartDisplayStatus) {
                case AdasRightSideRearWarning2PartDisplayStatus::WARN_1ST:
                    statRSR = SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::WARN_1ST;
                    break;
                case AdasRightSideRearWarning2PartDisplayStatus::WARN_2ND:
                    statRSR = SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::WARN_2ND;
                    break;
                case AdasRightSideRearWarning2PartDisplayStatus::WARN_3RD:
                    statRSR = SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::WARN_3RD;
                    break;
                case AdasRightSideRearWarning2PartDisplayStatus::GRAY:
                    statRSR = SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::GRAY;
                    break;
                default:
                    statRSR = SFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkLeftSideFrontWarning2PartStat(statLSF);
        setSFCADAS_PARKING_NEWConstantParkRightSideFrontWarning2PartStat(statRSF);
        setSFCADAS_PARKING_NEWConstantParkLeftSideRearWarning2PartStat(statLSR);
        setSFCADAS_PARKING_NEWConstantParkRightSideRearWarning2PartStat(statRSR);
    }

    // 4.1.14 ~ 17
    void updateMasterWarning() {
        SFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat statFM = SFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRearMasterWarningStat statRM = SFCADAS_PARKING_NEWConstantParkRearMasterWarningStat::OFF;
        SFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat statLSM =
            SFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat::OFF;
        SFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat statRSM =
            SFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat::OFF;

        if (mIsIgnOn == true) {
            // 4.1.14
            if (mAdasFrontMasterWarningDisplayStatus == AdasFrontMasterWarningDisplayStatus::WARNING_ASSIST) {
                statFM = SFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat::RED;
            } else if (mAdasFrontMasterWarningDisplayStatus == AdasFrontMasterWarningDisplayStatus::FAILURE_BLOCKAGE ||
                       mIbuFrontMasterWarningDisplayStatus == IbuFrontMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                statFM = SFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat::YELLOW;
            } else {
                statFM = SFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat::OFF;
            }
            // 4.1.15
            if (mAdasRearMasterWarningDisplayStatus == AdasRearMasterWarningDisplayStatus::WARNING_ASSIST) {
                statRM = SFCADAS_PARKING_NEWConstantParkRearMasterWarningStat::RED;
            } else if (mAdasRearMasterWarningDisplayStatus == AdasRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE ||
                       mIbuRearMasterWarningDisplayStatus == IbuRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                statRM = SFCADAS_PARKING_NEWConstantParkRearMasterWarningStat::YELLOW;
            } else {
                statRM = SFCADAS_PARKING_NEWConstantParkRearMasterWarningStat::OFF;
            }
            // 4.1.16
            switch (mAdasLeftMasterWarningDisplayStatus) {
                case AdasLeftMasterWarningDisplayStatus::WARNING_ASSIST:
                    statLSM = SFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat::RED;
                    break;
                case AdasLeftMasterWarningDisplayStatus::FAILURE_BLOCKAGE:
                    statLSM = SFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat::YELLOW;
                    break;
                default:
                    statLSM = SFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat::OFF;
                    break;
            }

            // 4.1.17
            switch (mAdasRightMasterWarningDisplayStatus) {
                case AdasRightMasterWarningDisplayStatus::WARNING_ASSIST:
                    statRSM = SFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat::RED;
                    break;
                case AdasRightMasterWarningDisplayStatus::FAILURE_BLOCKAGE:
                    statRSM = SFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat::YELLOW;
                    break;
                default:
                    statRSM = SFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkFrontMasterWarningStat(statFM);
        setSFCADAS_PARKING_NEWConstantParkRearMasterWarningStat(statRM);
        setSFCADAS_PARKING_NEWConstantParkLeftSideMasterWarningStat(statLSM);
        setSFCADAS_PARKING_NEWConstantParkRightSideMasterWarningStat(statRSM);
    }

    // 4.1.18 ~ 21
    void updateBoundaryLane() {
        SFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat statFB =
            SFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat::OFF;
        SFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat statRB =
            SFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat::OFF;
        SFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat statLSB =
            SFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat::OFF;
        SFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat statRSB =
            SFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat::OFF;

        if (mIsIgnOn == true) {
            // 4.1.18
            switch (mAdasPcaFrontBoundaryLaneStatus) {
                case AdasPcaFrontBoundaryLaneStatus::READY:
                    statFB = SFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat::READY;
                    break;
                case AdasPcaFrontBoundaryLaneStatus::ASSIST:
                    statFB = SFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat::ASSIST;
                    break;
                default:
                    statFB = SFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat::OFF;
                    break;
            }
            // 4.1.19
            switch (mAdasPcaRearBoundaryLaneStatus) {
                case AdasPcaRearBoundaryLaneStatus::READY:
                    statRB = SFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat::READY;
                    break;
                case AdasPcaRearBoundaryLaneStatus::ASSIST:
                    statRB = SFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat::ASSIST;
                    break;
                default:
                    statRB = SFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat::OFF;
                    break;
            }
            // 4.1.20
            switch (mAdasPcaLeftBoundaryLaneStatus) {
                case AdasPcaLeftBoundaryLaneStatus::READY:
                    statLSB = SFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat::READY;
                    break;
                case AdasPcaLeftBoundaryLaneStatus::ASSIST:
                    statLSB = SFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat::ASSIST;
                    break;
                default:
                    statLSB = SFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat::OFF;
                    break;
            }
            // 4.1.21
            switch (mAdasPcaRightBoundaryLaneStatus) {
                case AdasPcaRightBoundaryLaneStatus::READY:
                    statRSB = SFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat::READY;
                    break;
                case AdasPcaRightBoundaryLaneStatus::ASSIST:
                    statRSB = SFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat::ASSIST;
                    break;
                default:
                    statRSB = SFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantParkPcaFrontBoundaryLaneStat(statFB);
        setSFCADAS_PARKING_NEWConstantParkPcaRearBoundaryLaneStat(statRB);
        setSFCADAS_PARKING_NEWConstantParkPcaleftBoundaryLaneStat(statLSB);
        setSFCADAS_PARKING_NEWConstantParkPcaRightBoundaryLaneStat(statRSB);
    }

    // 4.1.22
    void updatePhrase() {
        if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::ASSIST || mPuPvLabelStatus == PuPvLabelStatus::ASSIST) {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::ASSIST);
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::WARNING ||
                   mPuPvLabelStatus == PuPvLabelStatus::WARNING) {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::WARNING);
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::BRAKE_HOLD ||
                   mPuPvLabelStatus == PuPvLabelStatus::BRAKE_HOLD) {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(
                SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::BRAKE_HOLD);
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::ASSIST_ENDED ||
                   mPuPvLabelStatus == PuPvLabelStatus::END) {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(
                SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::ASSIST_ENDED);
        } else if (mAdasPcaWarningPhraseStatus == AdasPcaWarningPhraseStatus::PDW_R_DEACTIVATED ||
                   mPdwWarningPhraseStatus == PdwWarningPhraseStatus::PDW_R_DISABLE_WITH_TG_OPEN) {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(
                SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::TAILGATE_OPENED);
        } else {
            setSFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat(SFCADAS_PARKING_NEWConstantParkPCAWarningPhraseStat::OFF);
        }
    }

    // 4.1.23
    void updateRccwWarning() {
        if (mPuPvRearLeftObjectMovingDirectionStatus == PuPvRearLeftObjectMovingDirectionStatus::WARNING_ASSIST) {
            setSFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat::WARN);
        } else if (mPuPvRearLeftObjectMovingDirectionStatus == PuPvRearLeftObjectMovingDirectionStatus::READY) {
            setSFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat::STANDBY);
        } else if (mPuPvRearLeftObjectMovingDirectionStatus == PuPvRearLeftObjectMovingDirectionStatus::FAIL) {
            setSFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat::ERROR);
        } else if (mPuPvRearLeftObjectMovingDirectionStatus == PuPvRearLeftObjectMovingDirectionStatus::WARNING_ASSIST_PASSING) {
            setSFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat::WARN_PASSING);
        } else {
            setSFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWLeftSymbolStat::OFF);
        }

        if (mPuPvRearRightObjectMovingDirectionStatus == PuPvRearRightObjectMovingDirectionStatus::WARNING_ASSIST) {
            setSFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat::WARN);
        } else if (mPuPvRearRightObjectMovingDirectionStatus == PuPvRearRightObjectMovingDirectionStatus::READY) {
            setSFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat::STANDBY);
        } else if (mPuPvRearRightObjectMovingDirectionStatus == PuPvRearRightObjectMovingDirectionStatus::FAIL) {
            setSFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat::ERROR);
        } else if (mPuPvRearRightObjectMovingDirectionStatus ==
                   PuPvRearRightObjectMovingDirectionStatus::WARNING_ASSIST_PASSING) {
            setSFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat(
                SFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat::WARN_PASSING);
        } else {
            setSFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat(SFCADAS_PARKING_NEWConstantParkRCCWRightSymbolStat::OFF);
        }
    }

    // 4.1.24
    void updateParkCCWRearWarn() {
        SFCADAS_PARKING_NEWConstantRearWarningLevelStat stat = SFCADAS_PARKING_NEWConstantRearWarningLevelStat::OFF;
        if (mIsIgnOn == true) {
            if (mCCWRearWarningLevelStatus == CCWRearWarningLevelStatus::WARN_1ST ||
                mCCWRearWarningLevelStatusFromSAC == CCWRearWarningLevelStatusFromSAC::WARN_1ST) {
                stat = SFCADAS_PARKING_NEWConstantRearWarningLevelStat::WARN_1ST;
            } else if (mCCWRearWarningLevelStatus == CCWRearWarningLevelStatus::WARN_2ND ||
                       mCCWRearWarningLevelStatusFromSAC == CCWRearWarningLevelStatusFromSAC::WARN_2ND) {
                stat = SFCADAS_PARKING_NEWConstantRearWarningLevelStat::WARN_2ND;
            } else if (mCCWRearWarningLevelStatus == CCWRearWarningLevelStatus::WARN_3RD ||
                       mCCWRearWarningLevelStatusFromSAC == CCWRearWarningLevelStatusFromSAC::WARN_3RD) {
                stat = SFCADAS_PARKING_NEWConstantRearWarningLevelStat::WARN_3RD;
            } else if (mCCWRearWarningLevelStatus == CCWRearWarningLevelStatus::GRAY ||
                       mCCWRearWarningLevelStatusFromSAC == CCWRearWarningLevelStatusFromSAC::GRAY) {
                stat = SFCADAS_PARKING_NEWConstantRearWarningLevelStat::GRAY;
            } else {
                // N.A
            }
        }
        setSFCADAS_PARKING_NEWConstantRearWarningLevelStat(stat);
    }

    // 4.1.25.1
    void updatePdwSystemFailurePRK() {
        SFCADAS_PARKING_NEWConstantPdwSystemFailurePRKStat stat = SFCADAS_PARKING_NEWConstantPdwSystemFailurePRKStat::OFF;
        if (mIsIgnOn == true) {
            if (mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                stat = SFCADAS_PARKING_NEWConstantPdwSystemFailurePRKStat::ULTRASONIC_SENSOR_BLOCKAGE;
            } else if (mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::PDW_FAILURE) {
                stat = SFCADAS_PARKING_NEWConstantPdwSystemFailurePRKStat::PDW_FAILURE;
            } else {
                // N.A
            }
        }
        setSFCADAS_PARKING_NEWConstantPdwSystemFailurePRKStat(stat);
    }

    // 4.1.25.2
    void updatePdwSystemFailureIBU() {
        SFCADAS_PARKING_NEWConstantPdwSystemFailureIBUStat stat = SFCADAS_PARKING_NEWConstantPdwSystemFailureIBUStat::OFF;
        if (mIsIgnOn == true) {
            if (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE) {
                stat = SFCADAS_PARKING_NEWConstantPdwSystemFailureIBUStat::ULTRASONIC_SENSOR_BLOCKAGE;
            } else if (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::PDW_FAILURE) {
                stat = SFCADAS_PARKING_NEWConstantPdwSystemFailureIBUStat::PDW_FAILURE;
            } else {
                // N.A
            }
        }
        setSFCADAS_PARKING_NEWConstantPdwSystemFailureIBUStat(stat);
    }

    // 4.1.25.6
    void updatePdwSystemFailure() {
        SFCADAS_PARKING_NEWConstantPdwSystemFailureStat stat = SFCADAS_PARKING_NEWConstantPdwSystemFailureStat::OFF;
        if (mIsIgnOn == true) {
            if ((mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigAdasPrkPDW == ConfigAdasPrkPDW::ON) ||
                (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigBdcPDW == ConfigBdcPDW::ON)) {
                stat = SFCADAS_PARKING_NEWConstantPdwSystemFailureStat::PDW_FAILURE;
            } else {
                // N.A
            }
        }
        setSFCADAS_PARKING_NEWConstantPdwSystemFailureStat(stat);
    }

    // 4.1.25.3
    void updatePcaSystemFailure() {
        SFCADAS_PARKING_NEWConstantPcaSystemFailureStat stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::OFF;
        if (mIsIgnOn == true) {
            switch (mAdasPcaSystemFailureStatus) {
                case AdasPcaSystemFailureStatus::CAMERA_BLOCKAGE:
                    stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::CAMERA_BLOCKAGE;
                    break;
                case AdasPcaSystemFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE:
                    stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::ULTRASONIC_SENSOR_BLOCKAGE;
                    break;
                case AdasPcaSystemFailureStatus::PCA_FAILURE:
                    stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::PCA_FAILURE;
                    break;
                case AdasPcaSystemFailureStatus::LIDAR_BLOCKAGE:
                    stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::LIDAR_BLOCKAGE;
                    break;
                default:
                    stat = SFCADAS_PARKING_NEWConstantPcaSystemFailureStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantPcaSystemFailureStat(stat);
    }

    // 4.1.25.4
    void updateRccaFailure() {
        SFCADAS_PARKING_NEWConstantRccaFailureStat stat = SFCADAS_PARKING_NEWConstantRccaFailureStat::OFF;
        if (mIsIgnOn == true) {
            if (mPuFGroup7RccaFailureStatus == PuFGroup7RccaFailureStatus::FAILURE ||
                (mPuFGroup7RccaFailureStatus == PuFGroup7RccaFailureStatus::UNHANDLED_TIMEOUT &&
                 mConfigNewRCCW == ConfigNewRCCW::ON)) {
                stat = SFCADAS_PARKING_NEWConstantRccaFailureStat::FAILURE;
            }
            if (mPuFGroup7RccaFailureStatus == PuFGroup7RccaFailureStatus::RADAR_BLOCKAGE) {
                stat = SFCADAS_PARKING_NEWConstantRccaFailureStat::RADAR_BLOCKAGE;
            }
        }
        setSFCADAS_PARKING_NEWConstantRccaFailureStat(stat);
    }

    // 4.1.25.5
    void updateExitSafetyFailure() {
        SFCADAS_PARKING_NEWConstantExitSafetyFailureStat stat = SFCADAS_PARKING_NEWConstantExitSafetyFailureStat::OFF;
        if (mIsIgnOn == true) {
            switch (mPuFGroup7ExitSafetyFailureStatus) {
                case PuFGroup7ExitSafetyFailureStatus::FAILURE:
                    stat = SFCADAS_PARKING_NEWConstantExitSafetyFailureStat::FAILURE;
                    break;
                case PuFGroup7ExitSafetyFailureStatus::RADAR_BLOCKAGE:
                    stat = SFCADAS_PARKING_NEWConstantExitSafetyFailureStat::RADAR_BLOCKAGE;
                    break;
                default:
                    stat = SFCADAS_PARKING_NEWConstantExitSafetyFailureStat::OFF;
                    break;
            }
        }
        setSFCADAS_PARKING_NEWConstantExitSafetyFailureStat(stat);
    }

    // 4.2.1
    void updateEvent4Group() {
        std::string eventID;
        static std::string lastActiveEventID;
        if (mIsIgnOn == true) {
            if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::RED ||
                mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::RED ||
                mPuPvOutlineColorStatus == PuPvOutlineColorStatus::RED) {
                eventID = "E53001";
            } else if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::AMBER ||
                       mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::AMBER ||
                       mPuPvOutlineColorStatus == PuPvOutlineColorStatus::AMBER) {
                eventID = "E53009";
            } else if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::WHITE ||
                       mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::WHITE ||
                       mPuPvOutlineColorStatus == PuPvOutlineColorStatus::WHITE) {
                eventID = "E53010";
            } else if (mCCWPopupBoundaryLaneColorStatus == CCWPopupBoundaryLaneColorStatus::RED ||
                       mCCWPopupBoundaryLaneColorStatusFromSAC == CCWPopupBoundaryLaneColorStatusFromSAC::RED) {
                eventID = "E53001";
            } else if (mCCWPopupBoundaryLaneColorStatus == CCWPopupBoundaryLaneColorStatus::AMBER ||
                       mCCWPopupBoundaryLaneColorStatusFromSAC == CCWPopupBoundaryLaneColorStatusFromSAC::AMBER) {
                eventID = "E53009";
            } else if (mCCWPopupBoundaryLaneColorStatus == CCWPopupBoundaryLaneColorStatus::WHITE ||
                       mCCWPopupBoundaryLaneColorStatusFromSAC == CCWPopupBoundaryLaneColorStatusFromSAC::WHITE) {
                eventID = "E53010";
            } else {
                // no operation
            }
        } else {
            if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::RED ||
                mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::RED ||
                mPuPvOutlineColorStatus == PuPvOutlineColorStatus::RED) {
                eventID = "E53015";
            }
        }

        // 의도적으로 Event A On, B On 발생시 flush를
        // Event B ON -> Event A OFF 순서로 전달하기 위해 다른 모듈과 출력 코드를 다르게 적용합니다.
        // A OFF 전달이 노드의 최종 상태가 되므로, Active 되어 있는 B Event의 추후 OFF 처리를 위하여
        // lastActiveEventID를 사용했습니다.
        // std::string prevEventID;
        // GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event4Group.ID, prevEventID);

        if (eventID.empty() == false) {
            // NOTE: (A) (jira issue: EXNCP-42393)
            setSFCADAS_PARKING_NEWEventEvent4GroupID(eventID);
            setSFCADAS_PARKING_NEWEventEvent4GroupStat(SFCADAS_PARKING_NEWEventEvent4GroupStat::ON);
            flushLastGroup();
        } else {
            setSFCADAS_PARKING_NEWEventEvent4GroupID(lastActiveEventID);
            setSFCADAS_PARKING_NEWEventEvent4GroupStat(SFCADAS_PARKING_NEWEventEvent4GroupStat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_PARKING_NEWEventEvent4GroupID(lastActiveEventID);
            setSFCADAS_PARKING_NEWEventEvent4GroupStat(SFCADAS_PARKING_NEWEventEvent4GroupStat::OFF);
            flushLastGroup();
            // NOTE: update to final Event On value = (A) (jira issue: EXNCP-42393)
            setSFCADAS_PARKING_NEWEventEvent4GroupID(eventID);
            setSFCADAS_PARKING_NEWEventEvent4GroupStat(SFCADAS_PARKING_NEWEventEvent4GroupStat::ON);
            flushLastGroup();
        }

        lastActiveEventID = eventID;
    }

    // 4.2.2
    void updateEventFailure10() {
        std::string eventID;
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
                // N.A
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure10.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure10Stat(SFCADAS_PARKING_NEWEventEvent7GroupFailure10Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure10ID(eventID);
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure10Stat(SFCADAS_PARKING_NEWEventEvent7GroupFailure10Stat::ON);
        }
    }

    // 4.2.3
    void updateEventFailure11() {
        std::string eventID;
        if (mIsIgnOn == true) {
            if (mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::FAILURE) {
                eventID = "E53019";
            } else if (mPuFGroup7DriverAssistFailure11Status ==
                       PuFGroup7DriverAssistFailure11Status::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "E53020";
            } else {
                // N.A
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event7GroupFailure11.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure11Stat(SFCADAS_PARKING_NEWEventEvent7GroupFailure11Stat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure11ID(eventID);
            setSFCADAS_PARKING_NEWEventEvent7GroupFailure11Stat(SFCADAS_PARKING_NEWEventEvent7GroupFailure11Stat::ON);
        }
    }

    // 4.2.4
    void updateEventCCWFailure() {
        std::string eventID;
        if (mIsIgnOn == true) {
            if (mCCWFailureStatus == CCWFailureStatus::FAILURE || mCCWFailureStatusFromSAC == CCWFailureStatusFromSAC::FAILURE) {
                eventID = "E53022";
            } else if (mCCWFailureStatus == CCWFailureStatus::ULTRASONIC_SENSOR_BLOCKAGE ||
                       mCCWFailureStatusFromSAC == CCWFailureStatusFromSAC::ULTRASONIC_SENSOR_BLOCKAGE) {
                eventID = "E53023";
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event7GroupCCWFailure.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureStat(SFCADAS_PARKING_NEWEventEvent7GroupCCWFailureStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureID(eventID);
            setSFCADAS_PARKING_NEWEventEvent7GroupCCWFailureStat(SFCADAS_PARKING_NEWEventEvent7GroupCCWFailureStat::ON);
        }
    }

    // 4.2.5
    void updateEvent2Group() {
        std::string eventID;
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

        // 의도적으로 Event A On, B On 발생시 flush를
        // Event B ON -> Event A OFF 순서로 전달하기 위해 다른 모듈과 출력 코드를 다르게 적용합니다.
        // A OFF 전달이 노드의 최종 상태가 되므로, Active 되어 있는 B Event의 추후 OFF 처리를 위하여
        // lastActiveEventID를 사용했습니다.
        // std::string prevEventID;
        // GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Event.Event4Group.ID, prevEventID);

        static std::string lastActiveEventID;
        if (eventID.empty() == false) {
            setSFCADAS_PARKING_NEWEventEvent2GroupID(eventID);
            setSFCADAS_PARKING_NEWEventEvent2GroupStat(SFCADAS_PARKING_NEWEventEvent2GroupStat::ON);
            flushLastGroup();
        } else {
            setSFCADAS_PARKING_NEWEventEvent2GroupID(lastActiveEventID);
            setSFCADAS_PARKING_NEWEventEvent2GroupStat(SFCADAS_PARKING_NEWEventEvent2GroupStat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_PARKING_NEWEventEvent2GroupID(lastActiveEventID);
            setSFCADAS_PARKING_NEWEventEvent2GroupStat(SFCADAS_PARKING_NEWEventEvent2GroupStat::OFF);
            flushLastGroup();
        }
        lastActiveEventID = eventID;
    }

    // 4.2.6
    void updateEvent7GroupTimeout() {
        SFCADAS_PARKING_NEWEventEvent7GroupTimeoutStat stat = SFCADAS_PARKING_NEWEventEvent7GroupTimeoutStat::OFF;
        if (mIsIgnOn == true) {
            if ((mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::UNHANDLED_TIMEOUT &&
                 mConfigAdasPrkPDW == ConfigAdasPrkPDW::ON) ||
                (mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::UNHANDLED_TIMEOUT &&
                 mConfigBdcPDW == ConfigBdcPDW::ON)) {
                stat = SFCADAS_PARKING_NEWEventEvent7GroupTimeoutStat::ON;
            } else {
                // N.A
            }
        }
        setSFCADAS_PARKING_NEWEventEvent7GroupTimeoutStat(stat);
    }

    // 5.1
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
        GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Sound.WarningLevel.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_PARKING_NEWSoundWarningLevelStat(SFCADAS_PARKING_NEWSoundWarningLevelStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_PARKING_NEWSoundWarningLevelID(soundID);
            setSFCADAS_PARKING_NEWSoundWarningLevelStat(SFCADAS_PARKING_NEWSoundWarningLevelStat::ON);
        }
    }

    // 5.2 Input_PaWarnSoundStatus
    void updateSoundPASysWarn() {
        std::string soundID;

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

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_NEW.Sound.PASysWarnSound.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_PARKING_NEWSoundPASysWarnSoundStat(SFCADAS_PARKING_NEWSoundPASysWarnSoundStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_PARKING_NEWSoundPASysWarnSoundID(soundID);
            setSFCADAS_PARKING_NEWSoundPASysWarnSoundStat(SFCADAS_PARKING_NEWSoundPASysWarnSoundStat::ON);
        }
    }

    // 5.2 Input_PdwCheckSoundStatus
    void updateSoundPDWSensorFail() {
        SFCADAS_PARKING_NEWSoundWarningSensorFailStat soundStatus{SFCADAS_PARKING_NEWSoundWarningSensorFailStat::OFF};
        if ((mPdwCheckSoundStatus == PdwCheckSoundStatus::SENSOR_FAIL_WARN ||
             mPaWarnSoundStatus == PaWarnSoundStatus::SENSOR_FAIL_WARN) &&
            mIsIgnOn == true) {
            soundStatus = SFCADAS_PARKING_NEWSoundWarningSensorFailStat::ON;
        }

        setSFCADAS_PARKING_NEWSoundWarningSensorFailStat(soundStatus);
    }

    void updateSoundPDWTailgateOpened() {
        SFCADAS_PARKING_NEWSoundWarningTailgateOpenedStat soundStatus{SFCADAS_PARKING_NEWSoundWarningTailgateOpenedStat::OFF};
        if ((mPdwCheckSoundStatus == PdwCheckSoundStatus::PDW_R_DISABLE_SOUND ||
             mPaWarnSoundStatus == PaWarnSoundStatus::PDW_R_DEACTIVATED_WARNING_ALARM) &&
            mIsIgnOn == true) {
            soundStatus = SFCADAS_PARKING_NEWSoundWarningTailgateOpenedStat::ON;
        }

        setSFCADAS_PARKING_NEWSoundWarningTailgateOpenedStat(soundStatus);
    }

    HBool mIsIgnOn = false;

    using AdasCarBodyDisplayStatus = ArgumentsCarBodyDisplayStatusChanged::Input_AdasCarBodyDisplayStatus;
    using IbuCarBodyDisplayStatus = ArgumentsCarBodyDisplayStatusChanged::Input_IbuCarBodyDisplayStatus;
    using PuPvHostVehicleStatus = ArgumentsCarBodyDisplayStatusChanged::Input_PuPvHostVehicleStatus;
    using CCWCarBodyDisplayStatus = ArgumentsCarBodyDisplayStatusChanged::Input_CCWCarBodyDisplayStatus;
    using CCWCarBodyDisplayStatusFromSAC = ArgumentsCarBodyDisplayStatusChanged::Input_CCWCarBodyDisplayStatusFromSAC;
    AdasCarBodyDisplayStatus mAdasCarBodyDisplayStatus = AdasCarBodyDisplayStatus::OFF;
    IbuCarBodyDisplayStatus mIbuCarBodyDisplayStatus = IbuCarBodyDisplayStatus::OFF;
    PuPvHostVehicleStatus mPuPvHostVehicleStatus = PuPvHostVehicleStatus::OFF;
    CCWCarBodyDisplayStatus mCCWCarBodyDisplayStatus = CCWCarBodyDisplayStatus::OFF;
    CCWCarBodyDisplayStatusFromSAC mCCWCarBodyDisplayStatusFromSAC = CCWCarBodyDisplayStatusFromSAC::OFF;

    using AdasFrontCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasFrontCenterWarning3PartDisplayStatus;
    using AdasFrontLeftWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasFrontLeftWarning3PartDisplayStatus;
    using AdasFrontRightWaring3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasFrontRightWaring3PartDisplayStatus;
    using IbuFrontCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontCenterWarning3PartDisplayStatus;
    using IbuFrontLeftWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontLeftWarning3PartDisplayStatus;
    using IbuFrontRightWaring3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontRightWarning3PartDisplayStatus;
    AdasFrontCenterWarning3PartDisplayStatus mAdasFrontCenterWarning3PartDisplayStatus =
        AdasFrontCenterWarning3PartDisplayStatus::OFF;
    AdasFrontLeftWarning3PartDisplayStatus mAdasFrontLeftWarning3PartDisplayStatus = AdasFrontLeftWarning3PartDisplayStatus::OFF;
    AdasFrontRightWaring3PartDisplayStatus mAdasFrontRightWaring3PartDisplayStatus = AdasFrontRightWaring3PartDisplayStatus::OFF;
    IbuFrontCenterWarning3PartDisplayStatus mIbuFrontCenterWarning3PartDisplayStatus =
        IbuFrontCenterWarning3PartDisplayStatus::OFF;
    IbuFrontLeftWarning3PartDisplayStatus mIbuFrontLeftWarning3PartDisplayStatus = IbuFrontLeftWarning3PartDisplayStatus::OFF;
    IbuFrontRightWaring3PartDisplayStatus mIbuFrontRightWaring3PartDisplayStatus = IbuFrontRightWaring3PartDisplayStatus::OFF;

    using AdasRearCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearCenterWarning3PartDisplayStatus;
    using AdasRearLeftWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearLeftWarning3PartDisplayStatus;
    using AdasRearRightWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearRightWarning3PartDisplayStatus;
    using IbuRearCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuRearCenterWarning3PartDisplayStatus;
    using IbuRearLeftWarning3PartDisplayStatus = ArgumentsParkingDisplayStatusChanged::Input_IbuRearLeftWarning3PartDisplayStatus;
    using IbuRearRightWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuRearRightWarning3PartDisplayStatus;
    AdasRearCenterWarning3PartDisplayStatus mAdasRearCenterWarning3PartDisplayStatus =
        AdasRearCenterWarning3PartDisplayStatus::OFF;
    AdasRearLeftWarning3PartDisplayStatus mAdasRearLeftWarning3PartDisplayStatus = AdasRearLeftWarning3PartDisplayStatus::OFF;
    AdasRearRightWarning3PartDisplayStatus mAdasRearRightWarning3PartDisplayStatus = AdasRearRightWarning3PartDisplayStatus::OFF;
    IbuRearCenterWarning3PartDisplayStatus mIbuRearCenterWarning3PartDisplayStatus = IbuRearCenterWarning3PartDisplayStatus::OFF;
    IbuRearLeftWarning3PartDisplayStatus mIbuRearLeftWarning3PartDisplayStatus = IbuRearLeftWarning3PartDisplayStatus::OFF;
    IbuRearRightWarning3PartDisplayStatus mIbuRearRightWarning3PartDisplayStatus = IbuRearRightWarning3PartDisplayStatus::OFF;

    using AdasLeftSideWarning1PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasLeftSideWarning1PartDisplayStatus;
    using AdasRightSideWarning1PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRightSideWarning1PartDisplayStatus;
    AdasLeftSideWarning1PartDisplayStatus mAdasLeftSideWarning1PartDisplayStatus = AdasLeftSideWarning1PartDisplayStatus::OFF;
    AdasRightSideWarning1PartDisplayStatus mAdasRightSideWarning1PartDisplayStatus = AdasRightSideWarning1PartDisplayStatus::OFF;

    using AdasLeftSideFrontWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasLeftSideFrontWarning2PartDisplayStatus;
    using AdasLeftSideRearWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasLeftSideRearWarning2PartDisplayStatus;
    using AdasRightSideFrontWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRightSideFrontWarning2PartDisplayStatus;
    using AdasRightSideRearWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRightSideRearWarning2PartDisplayStatus;
    AdasLeftSideFrontWarning2PartDisplayStatus mAdasLeftSideFrontWarning2PartDisplayStatus =
        AdasLeftSideFrontWarning2PartDisplayStatus::OFF;
    AdasLeftSideRearWarning2PartDisplayStatus mAdasLeftSideRearWarning2PartDisplayStatus =
        AdasLeftSideRearWarning2PartDisplayStatus::OFF;
    AdasRightSideFrontWarning2PartDisplayStatus mAdasRightSideFrontWarning2PartDisplayStatus =
        AdasRightSideFrontWarning2PartDisplayStatus::OFF;
    AdasRightSideRearWarning2PartDisplayStatus mAdasRightSideRearWarning2PartDisplayStatus =
        AdasRightSideRearWarning2PartDisplayStatus::OFF;

    using AdasFrontMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasFrontMasterWarningDisplayStatus;
    using AdasLeftMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasLeftMasterWarningDisplayStatus;
    using AdasRearMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasRearMasterWarningDisplayStatus;
    using AdasRightMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasRightMasterWarningDisplayStatus;
    using IbuFrontMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_IbuFrontMasterWarningDisplayStatus;
    using IbuRearMasterWarningDisplayStatus = ArgumentsMasterWarningDisplayStatusChanged::Input_IbuRearMasterWarningDisplayStatus;
    AdasFrontMasterWarningDisplayStatus mAdasFrontMasterWarningDisplayStatus = AdasFrontMasterWarningDisplayStatus::OFF;
    AdasLeftMasterWarningDisplayStatus mAdasLeftMasterWarningDisplayStatus = AdasLeftMasterWarningDisplayStatus::OFF;
    AdasRearMasterWarningDisplayStatus mAdasRearMasterWarningDisplayStatus = AdasRearMasterWarningDisplayStatus::OFF;
    AdasRightMasterWarningDisplayStatus mAdasRightMasterWarningDisplayStatus = AdasRightMasterWarningDisplayStatus::OFF;
    IbuFrontMasterWarningDisplayStatus mIbuFrontMasterWarningDisplayStatus = IbuFrontMasterWarningDisplayStatus::OFF;
    IbuRearMasterWarningDisplayStatus mIbuRearMasterWarningDisplayStatus = IbuRearMasterWarningDisplayStatus::OFF;

    using AdasPcaFrontBoundaryLaneStatus = ArgumentsPcaBoundaryLaneStatusChanged::Input_AdasPcaFrontBoundaryLaneStatus;
    using AdasPcaRearBoundaryLaneStatus = ArgumentsPcaBoundaryLaneStatusChanged::Input_AdasPcaRearBoundaryLaneStatus;
    using AdasPcaLeftBoundaryLaneStatus = ArgumentsPcaBoundaryLaneStatusChanged::Input_AdasPcaLeftBoundaryLaneStatus;
    using AdasPcaRightBoundaryLaneStatus = ArgumentsPcaBoundaryLaneStatusChanged::Input_AdasPcaRightBoundaryLaneStatus;
    AdasPcaFrontBoundaryLaneStatus mAdasPcaFrontBoundaryLaneStatus = AdasPcaFrontBoundaryLaneStatus::OFF;
    AdasPcaRearBoundaryLaneStatus mAdasPcaRearBoundaryLaneStatus = AdasPcaRearBoundaryLaneStatus::OFF;
    AdasPcaLeftBoundaryLaneStatus mAdasPcaLeftBoundaryLaneStatus = AdasPcaLeftBoundaryLaneStatus::OFF;
    AdasPcaRightBoundaryLaneStatus mAdasPcaRightBoundaryLaneStatus = AdasPcaRightBoundaryLaneStatus::OFF;

    using AdasPcaWarningPhraseStatus = ArgumentsPcaWarningPhraseStatusChanged::Input_AdasPcaWarningPhraseStatus;
    using PuPvLabelStatus = ArgumentsPcaWarningPhraseStatusChanged::Input_PuPvLabelStatus;
    using PdwWarningPhraseStatus = ArgumentsPcaWarningPhraseStatusChanged::Input_PdwWarningPhraseStatus;
    AdasPcaWarningPhraseStatus mAdasPcaWarningPhraseStatus = AdasPcaWarningPhraseStatus::OFF;
    PuPvLabelStatus mPuPvLabelStatus = PuPvLabelStatus::OFF;
    PdwWarningPhraseStatus mPdwWarningPhraseStatus = PdwWarningPhraseStatus::OFF;

    using PuPvRearLeftObjectMovingDirectionStatus =
        ArgumentsPuPvRearObjectMovingDirectionStatus::Input_PuPvRearLeftObjectMovingDirectionStatus;
    using PuPvRearRightObjectMovingDirectionStatus =
        ArgumentsPuPvRearObjectMovingDirectionStatus::Input_PuPvRearRightObjectMovingDirectionStatus;

    PuPvRearLeftObjectMovingDirectionStatus mPuPvRearLeftObjectMovingDirectionStatus =
        PuPvRearLeftObjectMovingDirectionStatus::OFF;
    PuPvRearRightObjectMovingDirectionStatus mPuPvRearRightObjectMovingDirectionStatus =
        PuPvRearRightObjectMovingDirectionStatus::OFF;

    using AdasPdwSystemFailureStatus = ArgumentsAdasPdwSystemFailureStatusChanged::Input_AdasPdwSystemFailureStatus;
    AdasPdwSystemFailureStatus mAdasPdwSystemFailureStatus = AdasPdwSystemFailureStatus::OFF;

    using IbuPdwSystemFailureStatus = ArgumentsIbuPdwSystemFailureStatusChanged::Input_IbuPdwSystemFailureStatus;
    IbuPdwSystemFailureStatus mIbuPdwSystemFailureStatus = IbuPdwSystemFailureStatus::OFF;

    using AdasPcaSystemFailureStatus = ArgumentsAdasPcaSystemFailureStatusChanged::Input_AdasPcaSystemFailureStatus;
    AdasPcaSystemFailureStatus mAdasPcaSystemFailureStatus = AdasPcaSystemFailureStatus::OFF;

    using PuFGroup7RccaFailureStatus = ArgumentsRccwSysFailStatusChanged::Input_PuFGroup7RccaFailureStatus;
    using ConfigNewRCCW = ArgumentsRccwSysFailStatusChanged::Inter_ConfigNewRCCW;
    PuFGroup7RccaFailureStatus mPuFGroup7RccaFailureStatus = PuFGroup7RccaFailureStatus::OFF;
    ConfigNewRCCW mConfigNewRCCW = ConfigNewRCCW::OFF;

    using PuFGroup7ExitSafetyFailureStatus = ArgumentsExitSafetyFailureStatusChanged::Input_PuFGroup7ExitSafetyFailureStatus;
    PuFGroup7ExitSafetyFailureStatus mPuFGroup7ExitSafetyFailureStatus = PuFGroup7ExitSafetyFailureStatus::OFF;

    using AdasPopupBoundaryLaneColorStatus = ArgumentsEvent4GroupChanged::Input_AdasPopupBoundaryLaneColorStatus;
    using IbuPopupBoundaryLaneColorStatus = ArgumentsEvent4GroupChanged::Input_IbuPopupBoundaryLaneColorStatus;
    using PuPvOutlineColorStatus = ArgumentsEvent4GroupChanged::Input_PuPvOutlineColorStatus;
    using CCWPopupBoundaryLaneColorStatus = ArgumentsEvent4GroupChanged::Input_CCWPopupBoundaryLaneColorStatus;
    using CCWPopupBoundaryLaneColorStatusFromSAC = ArgumentsEvent4GroupChanged::Input_CCWPopupBoundaryLaneColorStatusFromSAC;
    AdasPopupBoundaryLaneColorStatus mAdasPopupBoundaryLaneColorStatus = AdasPopupBoundaryLaneColorStatus::OFF;
    IbuPopupBoundaryLaneColorStatus mIbuPopupBoundaryLaneColorStatus = IbuPopupBoundaryLaneColorStatus::OFF;
    PuPvOutlineColorStatus mPuPvOutlineColorStatus = PuPvOutlineColorStatus::OFF;
    CCWPopupBoundaryLaneColorStatus mCCWPopupBoundaryLaneColorStatus = CCWPopupBoundaryLaneColorStatus::OFF;
    CCWPopupBoundaryLaneColorStatusFromSAC mCCWPopupBoundaryLaneColorStatusFromSAC = CCWPopupBoundaryLaneColorStatusFromSAC::OFF;

    using PuFGroup7DriverAssistFailure10Status = ArgumentsFailure10StatusChanged::Input_PuFGroup7DriverAssistFailure10Status;
    PuFGroup7DriverAssistFailure10Status mPuFGroup7DriverAssistFailure10Status = PuFGroup7DriverAssistFailure10Status::OFF;

    using CCWFailureStatus = ArgumentsCCWFailureStatusChanged::Input_CCWFailureStatus;
    using CCWFailureStatusFromSAC = ArgumentsCCWFailureStatusChanged::Input_CCWFailureStatusFromSAC;
    CCWFailureStatus mCCWFailureStatus = CCWFailureStatus::OFF;
    CCWFailureStatusFromSAC mCCWFailureStatusFromSAC = CCWFailureStatusFromSAC::OFF;

    using PuFGroup7DriverAssistFailure11Status = ArgumentsFailure11StatusChanged::Input_PuFGroup7DriverAssistFailure11Status;
    PuFGroup7DriverAssistFailure11Status mPuFGroup7DriverAssistFailure11Status = PuFGroup7DriverAssistFailure11Status::OFF;

    using PcaModeDisplayStatus = ArgumentsEvent2GroupChanged::Input_PcaModeDisplayStatus;
    PcaModeDisplayStatus mPcaModeDisplayStatus = PcaModeDisplayStatus::OFF;

    using PaFrontCenterWarnSoundStatus = ArgumentsWarnSoundChanged::Input_PaFrontCenterWarnSoundStatus;
    PaFrontCenterWarnSoundStatus mPaFrontCenterWarnSoundStatus = PaFrontCenterWarnSoundStatus::OFF;

    using PdwFrontCenterWarnSoundStatus = ArgumentsWarnSoundChanged::Input_PdwFrontCenterWarnSoundStatus;
    PdwFrontCenterWarnSoundStatus mPdwFrontCenterWarnSoundStatus = PdwFrontCenterWarnSoundStatus::OFF;

    using CCWWarnSoundStatus = ArgumentsWarnSoundChanged::Input_CCWWarnSoundStatus;
    CCWWarnSoundStatus mCCWWarnSoundStatus = CCWWarnSoundStatus::OFF;
    using CCWWarnSoundStatusFromSAC = ArgumentsWarnSoundChanged::Input_CCWWarnSoundStatusFromSAC;
    CCWWarnSoundStatusFromSAC mCCWWarnSoundStatusFromSAC = CCWWarnSoundStatusFromSAC::OFF;

    using PaWarnSoundStatus = ArgumentsSysWarnSoundChanged::Input_PaWarnSoundStatus;
    using PdwCheckSoundStatus = ArgumentsSysWarnSoundChanged::Input_PdwCheckSoundStatus;
    PaWarnSoundStatus mPaWarnSoundStatus = PaWarnSoundStatus::OFF;
    PdwCheckSoundStatus mPdwCheckSoundStatus = PdwCheckSoundStatus::OFF;

    using CCWRearWarningLevelStatus = ArgumentsCCWWarningChanged::Input_CCWRearWarningLevelStatus;
    using CCWRearWarningLevelStatusFromSAC = ArgumentsCCWWarningChanged::Input_CCWRearWarningLevelStatusFromSAC;

    CCWRearWarningLevelStatus mCCWRearWarningLevelStatus = CCWRearWarningLevelStatus::OFF;
    CCWRearWarningLevelStatusFromSAC mCCWRearWarningLevelStatusFromSAC = CCWRearWarningLevelStatusFromSAC::OFF;

    using ConfigAdasPDW = ArgumentsADASPDWStatusChanged::Inter_ConfigAdasPDW;
    ConfigAdasPDW mConfigAdasPDW = ConfigAdasPDW::OFF;

    using ConfigAdasPrkPDW = ArgumentsADASPRKPDWStatusChanged::Inter_ConfigAdasPrkPDW;
    ConfigAdasPrkPDW mConfigAdasPrkPDW = ConfigAdasPrkPDW::OFF;

    using ConfigBdcPDW = ArgumentsBDCPDWStatusChanged::Inter_ConfigBdcPDW;
    ConfigBdcPDW mConfigBdcPDW = ConfigBdcPDW::OFF;
};

}  // namespace ccos

#endif  // SFSS_ADAS_Parking_NEW_H
