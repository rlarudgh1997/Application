/**
 * @file ADAS_PARKING_CV.hpp
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
#ifndef SFSS_ADAS_PARKING_CV_H
#define SFSS_ADAS_PARKING_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "ADAS_PARKING_CVBase.hpp"

namespace ccos {

// SFC Version : 4.0.1
// Spec Version : v0.21
// Reference : [CV710] ADAS_PARKING_CV. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class ADAS_PARKING_CV : public ADAS_PARKING_CVBase {
public:
    ADAS_PARKING_CV() = default;
    ~ADAS_PARKING_CV() override = default;
    ADAS_PARKING_CV(const ADAS_PARKING_CV& other) = delete;
    ADAS_PARKING_CV(ADAS_PARKING_CV&& other) noexcept = delete;
    ADAS_PARKING_CV& operator=(const ADAS_PARKING_CV& other) = delete;
    ADAS_PARKING_CV& operator=(ADAS_PARKING_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10ID("E77103");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10LinkedSoundType(
            SFCADAS_PARKING_CVEventEvent7GroupFailure10LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10LinkedSoundRepeatCount(1);

        setSFCADAS_PARKING_CVEventEvent7GroupFailure11ID("E77104");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11LinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11LinkedSoundType(
            SFCADAS_PARKING_CVEventEvent7GroupFailure11LinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11LinkedSoundRepeatCount(1);

        setSFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutID("E77105");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutLinkedSoundType(
            SFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutLinkedSoundRepeatCount(1);

        setSFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutID("E77106");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutLinkedSoundID("SND_PopUpWarn1");
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutLinkedSoundType(
            SFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutLinkedSoundType::REPEAT_COUNT);
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutLinkedSoundRepeatCount(1);

        setSFCADAS_PARKING_CVSoundWarningLevelType(SFCADAS_PARKING_CVSoundWarningLevelType::INFINITE);
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

    void onConfigPDWChanged(const ArgumentsConfigPDWChanged& args) {
        mConfigPDW = args.mInter_ConfigPDW;
        updateAll();
    }

    void onConfigSEWNEARChanged(const ArgumentsConfigSEWNEARChanged& args) {
        mConfigSEWNEAR = args.mInter_ConfigSEWNEAR;
        updateAll();
    }

    void onConfigADASParkingEcuTypeChanged(const ArgumentsConfigADASParkingEcuTypeChanged& args) {
        mConfigADASParkingEcuType = args.mInter_ConfigADASParkingEcuType;
        updateEventFailure10Timeout();
        updateEventFailure11Timeout();
    }

    void onCarBodyDisplayStatusChanged(const ArgumentsCarBodyDisplayStatusChanged& args) {
        mAdasCarBodyDisplayStatus = args.mInput_AdasCarBodyDisplayStatus;
        mIbuCarBodyDisplayStatus = args.mInput_IbuCarBodyDisplayStatus;
        updateCarBody();
    }

    void onParkingDisplayStatusChanged(const ArgumentsParkingDisplayStatusChanged& args) {
        mIbuFrontLeftWarning3PartDisplayStatus = args.mInput_IbuFrontLeftWarning3PartDisplayStatus;
        mIbuFrontCenterWarning3PartDisplayStatus = args.mInput_IbuFrontCenterWarning3PartDisplayStatus;
        mIbuFrontRightWarning3PartDisplayStatus = args.mInput_IbuFrontRightWarning3PartDisplayStatus;

        mAdasRearLeftWarning3PartDisplayStatus = args.mInput_AdasRearLeftWarning3PartDisplayStatus;
        mAdasRearCenterWarning3PartDisplayStatus = args.mInput_AdasRearCenterWarning3PartDisplayStatus;
        mAdasRearRightWarning3PartDisplayStatus = args.mInput_AdasRearRightWarning3PartDisplayStatus;
        mIbuRearLeftWarning3PartDisplayStatus = args.mInput_IbuRearLeftWarning3PartDisplayStatus;
        mIbuRearCenterWarning3PartDisplayStatus = args.mInput_IbuRearCenterWarning3PartDisplayStatus;
        mIbuRearRightWarning3PartDisplayStatus = args.mInput_IbuRearRightWarning3PartDisplayStatus;

        mAdasSewNearFrontRightWarning2PartDisplayStatus = args.mInput_AdasSewNearFrontRightWarning2PartDisplayStatus;

        mAdasSewNearFrontDoorWarning2PartDisplayStatus = args.mInput_AdasSewNearFrontDoorWarning2PartDisplayStatus;
        mAdasSewNearMidDoorWarning2PartDisplayStatus = args.mInput_AdasSewNearMidDoorWarning2PartDisplayStatus;
        mAdasSewNearRearDoorWarning2PartDisplayStatus = args.mInput_AdasSewNearRearDoorWarning2PartDisplayStatus;

        mAdasSewNearFrontLeftWarning2PartDisplayStatus = args.mInput_AdasSewNearFrontLeftWarning2PartDisplayStatus;

        mAdasSewNearFrontDoorRHDWarning2PartDisplayStatus = args.mInput_AdasSewNearFrontDoorRHDWarning2PartDisplayStatus;
        mAdasSewNearMidDoorRHDWarning2PartDisplayStatus = args.mInput_AdasSewNearMidDoorRHDWarning2PartDisplayStatus;
        mAdasSewNearRearDoorRHDWarning2PartDisplayStatus = args.mInput_AdasSewNearRearDoorRHDWarning2PartDisplayStatus;

        updateFrontWarning3Part();
        updateRearWarning3Part();
        updateFrontRightWarning2Part();
        updateDoorWarning2Part();
        updateFrontLeftWarning2Part();
        updateDoorWarning2PartRHD();
    }

    void onMasterWarningDisplayStatusChanged(const ArgumentsMasterWarningDisplayStatusChanged& args) {
        mAdasRearMasterWarningDisplayStatus = args.mInput_AdasRearMasterWarningDisplayStatus;
        mAdasRightMasterWarningDisplayStatus = args.mInput_AdasRightMasterWarningDisplayStatus;
        mIbuFrontMasterWarningDisplayStatus = args.mInput_IbuFrontMasterWarningDisplayStatus;
        mIbuRearMasterWarningDisplayStatus = args.mInput_IbuRearMasterWarningDisplayStatus;
        mAdasLeftMasterWarningDisplayStatus = args.mInput_AdasLeftMasterWarningDisplayStatus;
        updateMasterWarning();
    }

    void onAdasPdwSystemFailureStatusChanged(const ArgumentsAdasPdwSystemFailureStatusChanged& args) {
        mAdasPdwSystemFailureStatus = args.mInput_AdasPdwSystemFailureStatus;
        updatePdwSystemFailurePRK();
    }

    void onAdasSewNearSystemFailureStatusChanged(const ArgumentsAdasSewNearSystemFailureStatusChanged& args) {
        mAdasSewNearSystemFailureStatus = args.mInput_AdasSewNearSystemFailureStatus;
        updateSewNearSystemFailurePRK();
    }

    void onIbuPdwSystemFailureStatusChanged(const ArgumentsIbuPdwSystemFailureStatusChanged& args) {
        mIbuPdwSystemFailureStatus = args.mInput_IbuPdwSystemFailureStatus;
        updatePdwSystemFailureIBU();
    }

    void onFailure10StatusChanged(const ArgumentsFailure10StatusChanged& args) {
        mPuFGroup7DriverAssistFailure10Status = args.mInput_PuFGroup7DriverAssistFailure10Status;
        updateEventFailure10();
        updateEventFailure10Timeout();
    }

    void onFailure11StatusChanged(const ArgumentsFailure11StatusChanged& args) {
        mPuFGroup7DriverAssistFailure11Status = args.mInput_PuFGroup7DriverAssistFailure11Status;
        updateEventFailure11();
        updateEventFailure11Timeout();
    }

    void onTransmissionCurrentGearStatusChanged(const ArgumentsTransmissionCurrentGearStatusChanged& args) {
        mTransmissionCurrentGearStatus = args.mInput_TransmissionCurrentGearStatus;
        updateEventFailure10Timeout();
        updateEventFailure11Timeout();
    }

    void onEvent4GroupChanged(const ArgumentsEvent4GroupChanged& args) {
        mAdasPopupBoundaryLaneColorStatus = args.mInput_AdasPopupBoundaryLaneColorStatus;
        mIbuPopupBoundaryLaneColorStatus = args.mInput_IbuPopupBoundaryLaneColorStatus;
        updateEvent4Group();
    }

    void onWarnSoundChanged(const ArgumentsWarnSoundChanged& args) {
        mPaFrontCenterWarnSoundStatus = args.mInput_PaFrontCenterWarnSoundStatus;
        mPdwFrontCenterWarnSoundStatus = args.mInput_PdwFrontCenterWarnSoundStatus;
        updateSoundWarn();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateCarBody();
        updateFrontWarning3Part();
        updateRearWarning3Part();
        updateFrontRightWarning2Part();
        updateDoorWarning2Part();
        updateFrontLeftWarning2Part();
        updateDoorWarning2PartRHD();
        updateMasterWarning();
        updatePdwSystemFailurePRK();
        updateSewNearSystemFailurePRK();
        updatePdwSystemFailureIBU();
        updateEvent4Group();
        updateEventFailure10();
        updateEventFailure11();
        updateEventFailure10Timeout();
        updateEventFailure11Timeout();
        updateSoundWarn();
    }

    // 4.1.1 PDW/SEW-NEAR 카바디 표시 (1)
    void updateCarBody() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkCarBodyDisplayStat stat = SFCADAS_PARKING_CVConstantParkCarBodyDisplayStat::OFF;
        if (mIsIgnOn == true && (mAdasCarBodyDisplayStatus == AdasCarBodyDisplayStatus::ON ||
                                 mIbuCarBodyDisplayStatus == IbuCarBodyDisplayStatus::ON)) {
            stat = SFCADAS_PARKING_CVConstantParkCarBodyDisplayStat::ON;
        }
        setSFCADAS_PARKING_CVConstantParkCarBodyDisplayStat(stat);
    }

    // 4.1.2 ~ 4.1.4
    void updateFrontWarning3Part() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat statFL =
            SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat statFC =
            SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat statFR =
            SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat::OFF;
        if (mIsIgnOn == true && mConfigPDW == ConfigPDW::PDWFR) {
            // 4.1.2 PDW 전좌측방 경고 표시 (3 분할 경고 표시 사양) (2)
            if (mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_3RD) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat::WARN_3RD;
            } else if (mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_2ND) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat::WARN_2ND;
            } else if (mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::WARN_1ST) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat::WARN_1ST;
            } else if (mIbuFrontLeftWarning3PartDisplayStatus == IbuFrontLeftWarning3PartDisplayStatus::GRAY) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.3 PDW 전방 중앙 경고 표시 (3 분할 경고 표시 사양) (3)
            if (mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_3RD) {
                statFC = SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat::WARN_3RD;
            } else if (mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_2ND) {
                statFC = SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat::WARN_2ND;
            } else if (mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::WARN_1ST) {
                statFC = SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat::WARN_1ST;
            } else if (mIbuFrontCenterWarning3PartDisplayStatus == IbuFrontCenterWarning3PartDisplayStatus::GRAY) {
                statFC = SFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.4 PDW 전우측방 경고 표시 (3 분할 경고 표시 사양) (4)
            if (mIbuFrontRightWarning3PartDisplayStatus == IbuFrontRightWarning3PartDisplayStatus::WARN_3RD) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat::WARN_3RD;
            } else if (mIbuFrontRightWarning3PartDisplayStatus == IbuFrontRightWarning3PartDisplayStatus::WARN_2ND) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat::WARN_2ND;
            } else if (mIbuFrontRightWarning3PartDisplayStatus == IbuFrontRightWarning3PartDisplayStatus::WARN_1ST) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat::WARN_1ST;
            } else if (mIbuFrontRightWarning3PartDisplayStatus == IbuFrontRightWarning3PartDisplayStatus::GRAY) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontLeftWarning3PartStat(statFL);
        setSFCADAS_PARKING_CVConstantParkFrontCenterWarning3PartStat(statFC);
        setSFCADAS_PARKING_CVConstantParkFrontRightWarning3PartStat(statFR);
    }

    // 4.1.5 ~ 4.1.7
    void updateRearWarning3Part() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat statRL =
            SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat statRC =
            SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat statRR =
            SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat::OFF;
        if (mIsIgnOn == true && (mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR)) {
            // 4.1.5 PDW 후좌측방 경고 표시 (3 분할 경고 표시 사양) (5)
            if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_3RD) {
                statRL = SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat::WARN_3RD;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_2ND) {
                statRL = SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat::WARN_2ND;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::WARN_1ST) {
                statRL = SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat::WARN_1ST;
            } else if (mAdasRearLeftWarning3PartDisplayStatus == AdasRearLeftWarning3PartDisplayStatus::GRAY ||
                       mIbuRearLeftWarning3PartDisplayStatus == IbuRearLeftWarning3PartDisplayStatus::GRAY) {
                statRL = SFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.6 PDW 후방 중앙 경고 표시 (3 분할 경고 표시 사양) (6)
            if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_3RD) {
                statRC = SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat::WARN_3RD;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_2ND) {
                statRC = SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat::WARN_2ND;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::WARN_1ST) {
                statRC = SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat::WARN_1ST;
            } else if (mAdasRearCenterWarning3PartDisplayStatus == AdasRearCenterWarning3PartDisplayStatus::GRAY ||
                       mIbuRearCenterWarning3PartDisplayStatus == IbuRearCenterWarning3PartDisplayStatus::GRAY) {
                statRC = SFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.7 PDW 후우측방 경고 표시 (3 분할 경고 표시 사양) (7)
            if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_3RD ||
                mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_3RD) {
                statRR = SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat::WARN_3RD;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_2ND ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_2ND) {
                statRR = SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat::WARN_2ND;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::WARN_1ST ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::WARN_1ST) {
                statRR = SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat::WARN_1ST;
            } else if (mAdasRearRightWarning3PartDisplayStatus == AdasRearRightWarning3PartDisplayStatus::GRAY ||
                       mIbuRearRightWarning3PartDisplayStatus == IbuRearRightWarning3PartDisplayStatus::GRAY) {
                statRR = SFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkRearLeftWarning3PartStat(statRL);
        setSFCADAS_PARKING_CVConstantParkRearCenterWarning3PartStat(statRC);
        setSFCADAS_PARKING_CVConstantParkRearRightWarning3PartStat(statRR);
    }

    // 4.1.8 SEW-NEAR 전우측방 경고 표시 (2 분할 경고 표시 사양) (8)
    void updateFrontRightWarning2Part() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat statFR =
            SFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat::OFF;
        if (mIsIgnOn == true && mConfigSEWNEAR == ConfigSEWNEAR::ON) {
            if (mAdasSewNearFrontRightWarning2PartDisplayStatus == AdasSewNearFrontRightWarning2PartDisplayStatus::WARN_2ND) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearFrontRightWarning2PartDisplayStatus ==
                       AdasSewNearFrontRightWarning2PartDisplayStatus::WARN_1ST) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearFrontRightWarning2PartDisplayStatus == AdasSewNearFrontRightWarning2PartDisplayStatus::GRAY) {
                statFR = SFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontRightWarning2PartStat(statFR);
    }

    // 4.1.9 ~ 4.1.11
    void updateDoorWarning2Part() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat statFD =
            SFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat ::OFF;
        SFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat statMD = SFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat statRD =
            SFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat::OFF;
        if (mIsIgnOn == true && mConfigSEWNEAR == ConfigSEWNEAR::ON) {
            // 4.1.9 SEW-NEAR 전문 경고 표시 (2 분할 경고 표시 사양) (9)
            if (mAdasSewNearFrontDoorWarning2PartDisplayStatus == AdasSewNearFrontDoorWarning2PartDisplayStatus::WARN_2ND) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearFrontDoorWarning2PartDisplayStatus ==
                       AdasSewNearFrontDoorWarning2PartDisplayStatus::WARN_1ST) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearFrontDoorWarning2PartDisplayStatus == AdasSewNearFrontDoorWarning2PartDisplayStatus::GRAY) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.10 SEW-NEAR 중문 경고 표시 (2 분할 경고 표시 사양) (10)
            if (mAdasSewNearMidDoorWarning2PartDisplayStatus == AdasSewNearMidDoorWarning2PartDisplayStatus::WARN_2ND) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearMidDoorWarning2PartDisplayStatus == AdasSewNearMidDoorWarning2PartDisplayStatus::WARN_1ST) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearMidDoorWarning2PartDisplayStatus == AdasSewNearMidDoorWarning2PartDisplayStatus::GRAY) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.11 SEW-NEAR 후문 경고 표시 (2 분할 경고 표시 사양) (11)
            if (mAdasSewNearRearDoorWarning2PartDisplayStatus == AdasSewNearRearDoorWarning2PartDisplayStatus::WARN_2ND) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearRearDoorWarning2PartDisplayStatus == AdasSewNearRearDoorWarning2PartDisplayStatus::WARN_1ST) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearRearDoorWarning2PartDisplayStatus == AdasSewNearRearDoorWarning2PartDisplayStatus::GRAY) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontDoorWarning2PartStat(statFD);
        setSFCADAS_PARKING_CVConstantParkMidDoorWarning2PartStat(statMD);
        setSFCADAS_PARKING_CVConstantParkRearDoorWarning2PartStat(statRD);
    }

    // 4.1.12 SEW-NEAR 전좌측방 경고 표시 - RHD (2 분할 경고 표시 사양) (12)
    void updateFrontLeftWarning2Part() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat statFL =
            SFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat::OFF;
        if (mIsIgnOn == true && mConfigSEWNEAR == ConfigSEWNEAR::ON) {
            if (mAdasSewNearFrontLeftWarning2PartDisplayStatus == AdasSewNearFrontLeftWarning2PartDisplayStatus::WARN_2ND) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearFrontLeftWarning2PartDisplayStatus ==
                       AdasSewNearFrontLeftWarning2PartDisplayStatus::WARN_1ST) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearFrontLeftWarning2PartDisplayStatus == AdasSewNearFrontLeftWarning2PartDisplayStatus::GRAY) {
                statFL = SFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontLeftWarning2PartStat(statFL);
    }

    // 4.1.13 ~ 4.1.15
    void updateDoorWarning2PartRHD() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat statFD =
            SFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat ::OFF;
        SFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat statMD =
            SFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat::OFF;
        SFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat statRD =
            SFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat::OFF;
        if (mIsIgnOn == true && mConfigSEWNEAR == ConfigSEWNEAR::ON) {
            // 4.1.13 SEW-NEAR 전문 경고 표시 - RHD (2 분할 경고 표시 사양) (13)
            if (mAdasSewNearFrontDoorRHDWarning2PartDisplayStatus == AdasSewNearFrontDoorRHDWarning2PartDisplayStatus::WARN_2ND) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearFrontDoorRHDWarning2PartDisplayStatus ==
                       AdasSewNearFrontDoorRHDWarning2PartDisplayStatus::WARN_1ST) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearFrontDoorRHDWarning2PartDisplayStatus ==
                       AdasSewNearFrontDoorRHDWarning2PartDisplayStatus::GRAY) {
                statFD = SFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.14 SEW-NEAR 중문 경고 표시 - RHD (2 분할 경고 표시 사양) (14)
            if (mAdasSewNearMidDoorRHDWarning2PartDisplayStatus == AdasSewNearMidDoorRHDWarning2PartDisplayStatus::WARN_2ND) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearMidDoorRHDWarning2PartDisplayStatus ==
                       AdasSewNearMidDoorRHDWarning2PartDisplayStatus::WARN_1ST) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearMidDoorRHDWarning2PartDisplayStatus == AdasSewNearMidDoorRHDWarning2PartDisplayStatus::GRAY) {
                statMD = SFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }

            // 4.1.15 SEW-NEAR 후문 경고 표시 – RHD (2 분할 경고 표시 사양) (15)
            if (mAdasSewNearRearDoorRHDWarning2PartDisplayStatus == AdasSewNearRearDoorRHDWarning2PartDisplayStatus::WARN_2ND) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat::WARN_2ND;
            } else if (mAdasSewNearRearDoorRHDWarning2PartDisplayStatus ==
                       AdasSewNearRearDoorRHDWarning2PartDisplayStatus::WARN_1ST) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat::WARN_1ST;
            } else if (mAdasSewNearRearDoorRHDWarning2PartDisplayStatus ==
                       AdasSewNearRearDoorRHDWarning2PartDisplayStatus::GRAY) {
                statRD = SFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat::GRAY;
            } else {
                // Default : OFF
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontDoorRHDWarning2PartStat(statFD);
        setSFCADAS_PARKING_CVConstantParkMidDoorRHDWarning2PartStat(statMD);
        setSFCADAS_PARKING_CVConstantParkRearDoorRHDWarning2PartStat(statRD);
    }

    // 4.1.16 ~ 4.1.18
    void updateMasterWarning() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantParkFrontMasterWarningStat statFM = SFCADAS_PARKING_CVConstantParkFrontMasterWarningStat::OFF;
        SFCADAS_PARKING_CVConstantParkRearMasterWarningStat statRM = SFCADAS_PARKING_CVConstantParkRearMasterWarningStat::OFF;
        SFCADAS_PARKING_CVConstantParkRightSideMasterWarningStat statRSM =
            SFCADAS_PARKING_CVConstantParkRightSideMasterWarningStat::OFF;
        SFCADAS_PARKING_CVConstantParkLeftSideMasterWarningStat statLSM =
            SFCADAS_PARKING_CVConstantParkLeftSideMasterWarningStat::OFF;
        if (mIsIgnOn == true) {
            // 4.1.16 PDW 전방 경고 표시 (고장/가림) (16)
            if (mConfigPDW == ConfigPDW::PDWFR &&
                mIbuFrontMasterWarningDisplayStatus == IbuFrontMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                statFM = SFCADAS_PARKING_CVConstantParkFrontMasterWarningStat::YELLOW;
            }
            // 4.1.17 PDW 후방 경고 표시 (고장/가림) (17)
            if ((mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR) &&
                (mAdasRearMasterWarningDisplayStatus == AdasRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE ||
                 mIbuRearMasterWarningDisplayStatus == IbuRearMasterWarningDisplayStatus::FAILURE_BLOCKAGE)) {
                statRM = SFCADAS_PARKING_CVConstantParkRearMasterWarningStat::YELLOW;
            }
            // 4.1.18 SEW-NEAR 우측방 경고 표시 (고장/가림) (18)
            if (mConfigSEWNEAR == ConfigSEWNEAR::ON &&
                mAdasRightMasterWarningDisplayStatus == AdasRightMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                statRSM = SFCADAS_PARKING_CVConstantParkRightSideMasterWarningStat::YELLOW;
            }
            // 4.1.19 SEW-NEAR 좌측방 경고 표시 - RHD (고장/가림) (19)
            if (mConfigSEWNEAR == ConfigSEWNEAR::ON &&
                mAdasLeftMasterWarningDisplayStatus == AdasLeftMasterWarningDisplayStatus::FAILURE_BLOCKAGE) {
                statLSM = SFCADAS_PARKING_CVConstantParkLeftSideMasterWarningStat::YELLOW;
            }
        }
        setSFCADAS_PARKING_CVConstantParkFrontMasterWarningStat(statFM);
        setSFCADAS_PARKING_CVConstantParkRearMasterWarningStat(statRM);
        setSFCADAS_PARKING_CVConstantParkRightSideMasterWarningStat(statRSM);
        setSFCADAS_PARKING_CVConstantParkLeftSideMasterWarningStat(statLSM);
    }

    // 4.1.20.1 주차 거리 경고 시스템 고장 (ADAS_CMC)
    void updatePdwSystemFailurePRK() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantPdwSystemFailurePRKStat stat = SFCADAS_PARKING_CVConstantPdwSystemFailurePRKStat::OFF;
        if (mIsIgnOn == true && (mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR)) {
            if (mAdasPdwSystemFailureStatus == AdasPdwSystemFailureStatus::PDW_FAILURE) {
                stat = SFCADAS_PARKING_CVConstantPdwSystemFailurePRKStat::PDW_FAILURE;
            }
        }
        setSFCADAS_PARKING_CVConstantPdwSystemFailurePRKStat(stat);
    }

    // 4.1.20.2 안전 하차 경고 시스템 고장 (ADAS_CMC)
    void updateSewNearSystemFailurePRK() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantSewNearSystemFailure_PRKStat stat = SFCADAS_PARKING_CVConstantSewNearSystemFailure_PRKStat::OFF;
        if (mIsIgnOn == true && mConfigSEWNEAR == ConfigSEWNEAR::ON) {
            if (mAdasSewNearSystemFailureStatus == AdasSewNearSystemFailureStatus::SEW_NEAR_FAILURE) {
                stat = SFCADAS_PARKING_CVConstantSewNearSystemFailure_PRKStat::SEW_NEAR_FAILURE;
            }
        }
        setSFCADAS_PARKING_CVConstantSewNearSystemFailure_PRKStat(stat);
    }

    // 4.1.20.3 주차 거리 경고 시스템 고장 (IBU)
    void updatePdwSystemFailureIBU() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVConstantPdwSystemFailureIBUStat stat = SFCADAS_PARKING_CVConstantPdwSystemFailureIBUStat::OFF;
        if (mIsIgnOn == true && (mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR)) {
            if (mIbuPdwSystemFailureStatus == IbuPdwSystemFailureStatus::PDW_FAILURE) {
                stat = SFCADAS_PARKING_CVConstantPdwSystemFailureIBUStat::PDW_FAILURE;
            }
        }
        setSFCADAS_PARKING_CVConstantPdwSystemFailureIBUStat(stat);
    }

    // 4.2.1 4 그룹 풀 팝업 - PDW/SEW-NEAR 동작
    void updateEvent4Group() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        std::string eventID;
        static std::string lastActiveEventID;
        if (mIsIgnOn == true) {
            if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::RED ||
                mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::RED) {
                eventID = "E77100";
            } else if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::AMBER ||
                       mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::AMBER) {
                eventID = "E77101";
            } else if (mAdasPopupBoundaryLaneColorStatus == AdasPopupBoundaryLaneColorStatus::WHITE ||
                       mIbuPopupBoundaryLaneColorStatus == IbuPopupBoundaryLaneColorStatus::WHITE) {
                eventID = "E77102";
            } else {
                // no operation
            }
        }

        // 의도적으로 Event A On, B On 발생시 flush를
        // Event B ON -> Event A OFF 순서로 전달하기 위해 다른 모듈과 출력 코드를 다르게 적용합니다.
        // A OFF 전달이 노드의 최종 상태가 되므로, Active 되어 있는 B Event의 추후 OFF 처리를 위하여
        // lastActiveEventID를 사용했습니다.
        // std::string prevEventID;
        // GETCACHEDVALUE(SFC.ADAS_PARKING_CV.Event.Event4Group.ID, prevEventID);
        if (eventID.empty() == false) {
            setSFCADAS_PARKING_CVEventEvent4GroupID(eventID);
            setSFCADAS_PARKING_CVEventEvent4GroupStat(SFCADAS_PARKING_CVEventEvent4GroupStat::ON);
            flushLastGroup();
        } else {
            setSFCADAS_PARKING_CVEventEvent4GroupID(lastActiveEventID);
            setSFCADAS_PARKING_CVEventEvent4GroupStat(SFCADAS_PARKING_CVEventEvent4GroupStat::OFF);
        }

        if (lastActiveEventID.empty() == false && eventID.empty() == false && lastActiveEventID != eventID) {
            setSFCADAS_PARKING_CVEventEvent4GroupID(lastActiveEventID);
            setSFCADAS_PARKING_CVEventEvent4GroupStat(SFCADAS_PARKING_CVEventEvent4GroupStat::OFF);
            flushLastGroup();
        }

        lastActiveEventID = eventID;
    }

    // 4.2.2 7 그룹 풀 팝업 – 운전자 보조 시스템 고장 3 (ADAS_CMC)
    void updateEventFailure10() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVEventEvent7GroupFailure10Stat stat = SFCADAS_PARKING_CVEventEvent7GroupFailure10Stat::OFF;
        if (mIsIgnOn == true) {
            if (mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::FAILURE) {
                stat = SFCADAS_PARKING_CVEventEvent7GroupFailure10Stat::ON;
            }
        }
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10Stat(stat);
    }

    // 4.2.3 7 그룹 풀 팝업 – 운전자 보조 시스템 고장 4 (IBU)
    void updateEventFailure11() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVEventEvent7GroupFailure11Stat stat = SFCADAS_PARKING_CVEventEvent7GroupFailure11Stat::OFF;
        if (mIsIgnOn == true) {
            if (mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::FAILURE) {
                stat = SFCADAS_PARKING_CVEventEvent7GroupFailure11Stat::ON;
            }
        }
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11Stat(stat);
    }

    // 4.2.4 7 그룹 풀 팝업 - 운전자 보조 시스템 고장 (ADAS_PRK timeout)
    void updateEventFailure10Timeout() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutStat stat = SFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutStat::OFF;
        if (mIsIgnOn == true) {
            if ((mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR) &&
                mConfigADASParkingEcuType == ConfigADASParkingEcuType::ADAS_PRK &&
                mPuFGroup7DriverAssistFailure10Status == PuFGroup7DriverAssistFailure10Status::MESSAGE_TIMEOUT &&
                (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R1 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R2 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R3 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R4)) {
                stat = SFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutStat::ON;
            }
        }
        setSFCADAS_PARKING_CVEventEvent7GroupFailure10TimeoutStat(stat);
    }

    // 4.2.5 7 그룹 풀 팝업 - 운전자 보조 시스템 고장 (IBU timeout)
    void updateEventFailure11Timeout() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

        SFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutStat stat = SFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutStat::OFF;
        if (mIsIgnOn == true) {
            if ((mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR) &&
                mConfigADASParkingEcuType == ConfigADASParkingEcuType::IBU_PDW &&
                mPuFGroup7DriverAssistFailure11Status == PuFGroup7DriverAssistFailure11Status::MESSAGE_TIMEOUT &&
                (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R1 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R2 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R3 ||
                 mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R4)) {
                stat = SFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutStat::ON;
            }
        }
        setSFCADAS_PARKING_CVEventEvent7GroupFailure11TimeoutStat(stat);
    }

    // 4.3.1 Warning Sound
    void updateSoundWarn() {
        if (isAvailableADASPARKINGCV() == false) {
            return;
        }

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
            } else {
                // OFF
            }
        }

        std::string prevSoundID;
        GETCACHEDVALUE(SFC.ADAS_PARKING_CV.Sound.WarningLevel.ID, prevSoundID);
        if (prevSoundID.empty() == false && prevSoundID != soundID) {
            setSFCADAS_PARKING_CVSoundWarningLevelStat(SFCADAS_PARKING_CVSoundWarningLevelStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCADAS_PARKING_CVSoundWarningLevelID(soundID);
            setSFCADAS_PARKING_CVSoundWarningLevelStat(SFCADAS_PARKING_CVSoundWarningLevelStat::ON);
        }
    }

    inline bool isAvailableADASPARKINGCV() {
        return mConfigPDW == ConfigPDW::PDWFR || mConfigPDW == ConfigPDW::PDWR || mConfigSEWNEAR == ConfigSEWNEAR::ON;
    }

    HBool mIsIgnOn = false;
    using ConfigPDW = ArgumentsConfigPDWChanged::Inter_ConfigPDW;
    ConfigPDW mConfigPDW = ConfigPDW::OFF;

    using ConfigSEWNEAR = ArgumentsConfigSEWNEARChanged::Inter_ConfigSEWNEAR;
    ConfigSEWNEAR mConfigSEWNEAR = ConfigSEWNEAR::OFF;

    using ConfigADASParkingEcuType = ArgumentsConfigADASParkingEcuTypeChanged::Inter_ConfigADASParkingEcuType;
    ConfigADASParkingEcuType mConfigADASParkingEcuType = ConfigADASParkingEcuType::OFF;

    using AdasCarBodyDisplayStatus = ArgumentsCarBodyDisplayStatusChanged::Input_AdasCarBodyDisplayStatus;
    using IbuCarBodyDisplayStatus = ArgumentsCarBodyDisplayStatusChanged::Input_IbuCarBodyDisplayStatus;
    AdasCarBodyDisplayStatus mAdasCarBodyDisplayStatus = AdasCarBodyDisplayStatus::OFF;
    IbuCarBodyDisplayStatus mIbuCarBodyDisplayStatus = IbuCarBodyDisplayStatus::OFF;

    using IbuFrontLeftWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontLeftWarning3PartDisplayStatus;
    using IbuFrontCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontCenterWarning3PartDisplayStatus;
    using IbuFrontRightWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuFrontRightWarning3PartDisplayStatus;
    using AdasRearLeftWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearLeftWarning3PartDisplayStatus;
    using AdasRearCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearCenterWarning3PartDisplayStatus;
    using AdasRearRightWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasRearRightWarning3PartDisplayStatus;
    using IbuRearLeftWarning3PartDisplayStatus = ArgumentsParkingDisplayStatusChanged::Input_IbuRearLeftWarning3PartDisplayStatus;
    using IbuRearCenterWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuRearCenterWarning3PartDisplayStatus;
    using IbuRearRightWarning3PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_IbuRearRightWarning3PartDisplayStatus;
    using AdasSewNearFrontRightWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearFrontRightWarning2PartDisplayStatus;
    using AdasSewNearFrontDoorWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearFrontDoorWarning2PartDisplayStatus;
    using AdasSewNearMidDoorWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearMidDoorWarning2PartDisplayStatus;
    using AdasSewNearRearDoorWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearRearDoorWarning2PartDisplayStatus;
    using AdasSewNearFrontLeftWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearFrontLeftWarning2PartDisplayStatus;
    using AdasSewNearFrontDoorRHDWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearFrontDoorRHDWarning2PartDisplayStatus;
    using AdasSewNearMidDoorRHDWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearMidDoorRHDWarning2PartDisplayStatus;
    using AdasSewNearRearDoorRHDWarning2PartDisplayStatus =
        ArgumentsParkingDisplayStatusChanged::Input_AdasSewNearRearDoorRHDWarning2PartDisplayStatus;
    IbuFrontLeftWarning3PartDisplayStatus mIbuFrontLeftWarning3PartDisplayStatus = IbuFrontLeftWarning3PartDisplayStatus::OFF;
    IbuFrontCenterWarning3PartDisplayStatus mIbuFrontCenterWarning3PartDisplayStatus =
        IbuFrontCenterWarning3PartDisplayStatus::OFF;
    IbuFrontRightWarning3PartDisplayStatus mIbuFrontRightWarning3PartDisplayStatus = IbuFrontRightWarning3PartDisplayStatus::OFF;
    AdasRearLeftWarning3PartDisplayStatus mAdasRearLeftWarning3PartDisplayStatus = AdasRearLeftWarning3PartDisplayStatus::OFF;
    AdasRearCenterWarning3PartDisplayStatus mAdasRearCenterWarning3PartDisplayStatus =
        AdasRearCenterWarning3PartDisplayStatus::OFF;
    AdasRearRightWarning3PartDisplayStatus mAdasRearRightWarning3PartDisplayStatus = AdasRearRightWarning3PartDisplayStatus::OFF;
    IbuRearLeftWarning3PartDisplayStatus mIbuRearLeftWarning3PartDisplayStatus = IbuRearLeftWarning3PartDisplayStatus::OFF;
    IbuRearCenterWarning3PartDisplayStatus mIbuRearCenterWarning3PartDisplayStatus = IbuRearCenterWarning3PartDisplayStatus::OFF;
    IbuRearRightWarning3PartDisplayStatus mIbuRearRightWarning3PartDisplayStatus = IbuRearRightWarning3PartDisplayStatus::OFF;
    AdasSewNearFrontRightWarning2PartDisplayStatus mAdasSewNearFrontRightWarning2PartDisplayStatus =
        AdasSewNearFrontRightWarning2PartDisplayStatus::OFF;
    AdasSewNearFrontDoorWarning2PartDisplayStatus mAdasSewNearFrontDoorWarning2PartDisplayStatus =
        AdasSewNearFrontDoorWarning2PartDisplayStatus::OFF;
    AdasSewNearMidDoorWarning2PartDisplayStatus mAdasSewNearMidDoorWarning2PartDisplayStatus =
        AdasSewNearMidDoorWarning2PartDisplayStatus::OFF;
    AdasSewNearRearDoorWarning2PartDisplayStatus mAdasSewNearRearDoorWarning2PartDisplayStatus =
        AdasSewNearRearDoorWarning2PartDisplayStatus::OFF;
    AdasSewNearFrontLeftWarning2PartDisplayStatus mAdasSewNearFrontLeftWarning2PartDisplayStatus =
        AdasSewNearFrontLeftWarning2PartDisplayStatus::OFF;
    AdasSewNearFrontDoorRHDWarning2PartDisplayStatus mAdasSewNearFrontDoorRHDWarning2PartDisplayStatus =
        AdasSewNearFrontDoorRHDWarning2PartDisplayStatus::OFF;
    AdasSewNearMidDoorRHDWarning2PartDisplayStatus mAdasSewNearMidDoorRHDWarning2PartDisplayStatus =
        AdasSewNearMidDoorRHDWarning2PartDisplayStatus::OFF;
    AdasSewNearRearDoorRHDWarning2PartDisplayStatus mAdasSewNearRearDoorRHDWarning2PartDisplayStatus =
        AdasSewNearRearDoorRHDWarning2PartDisplayStatus::OFF;

    using AdasRearMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasRearMasterWarningDisplayStatus;
    using AdasRightMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasRightMasterWarningDisplayStatus;
    using IbuFrontMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_IbuFrontMasterWarningDisplayStatus;
    using IbuRearMasterWarningDisplayStatus = ArgumentsMasterWarningDisplayStatusChanged::Input_IbuRearMasterWarningDisplayStatus;
    using AdasLeftMasterWarningDisplayStatus =
        ArgumentsMasterWarningDisplayStatusChanged::Input_AdasLeftMasterWarningDisplayStatus;
    AdasRearMasterWarningDisplayStatus mAdasRearMasterWarningDisplayStatus = AdasRearMasterWarningDisplayStatus::OFF;
    AdasRightMasterWarningDisplayStatus mAdasRightMasterWarningDisplayStatus = AdasRightMasterWarningDisplayStatus::OFF;
    IbuFrontMasterWarningDisplayStatus mIbuFrontMasterWarningDisplayStatus = IbuFrontMasterWarningDisplayStatus::OFF;
    IbuRearMasterWarningDisplayStatus mIbuRearMasterWarningDisplayStatus = IbuRearMasterWarningDisplayStatus::OFF;
    AdasLeftMasterWarningDisplayStatus mAdasLeftMasterWarningDisplayStatus = AdasLeftMasterWarningDisplayStatus::OFF;

    using AdasPdwSystemFailureStatus = ArgumentsAdasPdwSystemFailureStatusChanged::Input_AdasPdwSystemFailureStatus;
    AdasPdwSystemFailureStatus mAdasPdwSystemFailureStatus = AdasPdwSystemFailureStatus::OFF;

    using AdasSewNearSystemFailureStatus = ArgumentsAdasSewNearSystemFailureStatusChanged::Input_AdasSewNearSystemFailureStatus;
    AdasSewNearSystemFailureStatus mAdasSewNearSystemFailureStatus = AdasSewNearSystemFailureStatus::OFF;

    using IbuPdwSystemFailureStatus = ArgumentsIbuPdwSystemFailureStatusChanged::Input_IbuPdwSystemFailureStatus;
    IbuPdwSystemFailureStatus mIbuPdwSystemFailureStatus = IbuPdwSystemFailureStatus::OFF;

    using PuFGroup7DriverAssistFailure10Status = ArgumentsFailure10StatusChanged::Input_PuFGroup7DriverAssistFailure10Status;
    PuFGroup7DriverAssistFailure10Status mPuFGroup7DriverAssistFailure10Status = PuFGroup7DriverAssistFailure10Status::OFF;

    using PuFGroup7DriverAssistFailure11Status = ArgumentsFailure11StatusChanged::Input_PuFGroup7DriverAssistFailure11Status;
    PuFGroup7DriverAssistFailure11Status mPuFGroup7DriverAssistFailure11Status = PuFGroup7DriverAssistFailure11Status::OFF;

    using TransmissionCurrentGearStatus = ArgumentsTransmissionCurrentGearStatusChanged::Input_TransmissionCurrentGearStatus;
    TransmissionCurrentGearStatus mTransmissionCurrentGearStatus = TransmissionCurrentGearStatus::NO_DISPLAY;

    using AdasPopupBoundaryLaneColorStatus = ArgumentsEvent4GroupChanged::Input_AdasPopupBoundaryLaneColorStatus;
    using IbuPopupBoundaryLaneColorStatus = ArgumentsEvent4GroupChanged::Input_IbuPopupBoundaryLaneColorStatus;
    AdasPopupBoundaryLaneColorStatus mAdasPopupBoundaryLaneColorStatus = AdasPopupBoundaryLaneColorStatus::OFF;
    IbuPopupBoundaryLaneColorStatus mIbuPopupBoundaryLaneColorStatus = IbuPopupBoundaryLaneColorStatus::OFF;

    using PaFrontCenterWarnSoundStatus = ArgumentsWarnSoundChanged::Input_PaFrontCenterWarnSoundStatus;
    using PdwFrontCenterWarnSoundStatus = ArgumentsWarnSoundChanged::Input_PdwFrontCenterWarnSoundStatus;
    PaFrontCenterWarnSoundStatus mPaFrontCenterWarnSoundStatus = PaFrontCenterWarnSoundStatus::OFF;
    PdwFrontCenterWarnSoundStatus mPdwFrontCenterWarnSoundStatus = PdwFrontCenterWarnSoundStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_ADAS_PARKING_CV_H
