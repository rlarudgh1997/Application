/**
 * @file Transmission_Indicator_CV.hpp
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
#ifndef SFSS_Transmission_Indicator_CV_H
#define SFSS_Transmission_Indicator_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Transmission_Indicator_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 17.0.0
// Spec Version : v0.36
// Reference : [CV400] Transmission_Indicator_CV. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class Transmission_Indicator_CV : public Transmission_Indicator_CVBase {
public:
    Transmission_Indicator_CV() = default;
    ~Transmission_Indicator_CV() override = default;
    Transmission_Indicator_CV(const Transmission_Indicator_CV& other) = delete;
    Transmission_Indicator_CV(Transmission_Indicator_CV&& other) noexcept = delete;
    Transmission_Indicator_CV& operator=(const Transmission_Indicator_CV& other) = delete;
    Transmission_Indicator_CV& operator=(Transmission_Indicator_CV&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerSound300msTimeout.create(kTimerInterval300ms, this,
                                                  &Transmission_Indicator_CV::onOneShotTimerSound300msTimeout, false) == false) {
            DWarning() << "[Transmission_Indicator_CV] mOneShotTimerSound300msTimeout Create Failed";
        }
        if (mCurrentGearStatOneShotTimer200msTimeout.create(kTimerInterval200ms, this,
                                                            &Transmission_Indicator_CV::onCurrentGearStatOneShotTimer200msTimeout,
                                                            false) == false) {
            DWarning() << "[Transmission_Indicator_CV] mCurrentGearStatOneShotTimer200msTimeout Create Failed";
        }
        if (mAutomaticManualStatOneShotTimer200msTimeout.create(
                kTimerInterval200ms, this, &Transmission_Indicator_CV::onAutomaticManualStatOneShotTimer200msTimeout, false) ==
            false) {
            DWarning() << "[Transmission_Indicator_CV] mAutomaticManualStatOneShotTimer200msTimeout Create Failed";
        }

        setSFCTransmission_Indicator_CVEventShiftingToStageDMotorID("E74001");
        setSFCTransmission_Indicator_CVEventShiftingToStageNMotorID("E74002");
        setSFCTransmission_Indicator_CVEventShiftingToStageRMotorID("E74003");
        setSFCTransmission_Indicator_CVEventShiftingToStageDAllisonATMID("E74004");
        setSFCTransmission_Indicator_CVEventShiftingToStageNAllisonATMID("E74005");
        setSFCTransmission_Indicator_CVEventShiftingToStageRAllisonATMID("E74006");
        setSFCTransmission_Indicator_CVSoundCurrentGearPositionID("SND_RPosition");
        setSFCTransmission_Indicator_CVSoundCurrentGearPositionType(
            SFCTransmission_Indicator_CVSoundCurrentGearPositionType::REPEAT_COUNT);
        setSFCTransmission_Indicator_CVSoundCurrentGearPositionRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateTelltaleOffRoadStatus();
        updateTelltaleRockingFreeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionPowerModeStatus();
        updateConstantTransmissionAutomaticManualStatus();
        updateEventShiftingToStageDMotor();
        updateEventShiftingToStageNMotor();
        updateEventShiftingToStageRMotor();
        updateEventShiftingToStageDAllisonATM();
        updateEventShiftingToStageNAllisonATM();
        updateEventShiftingToStageRAllisonATM();
        updateSoundRPosition();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateTelltaleOffRoadStatus();
        updateTelltaleRockingFreeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionPowerModeStatus();
        updateConstantTransmissionAutomaticManualStatus();
        updateEventShiftingToStageDMotor();
        updateEventShiftingToStageNMotor();
        updateEventShiftingToStageRMotor();
        updateEventShiftingToStageDAllisonATM();
        updateEventShiftingToStageNAllisonATM();
        updateEventShiftingToStageRAllisonATM();
        updateSoundRPosition();
    }

    void onConfigTMTypeChanged(const ArgumentsConfigTMTypeChanged& args) {
        mConfigTMType = args.mInter_ConfigTMType;

        updatePrivateAllisonTransmissionModeStatus();
        updateTelltaleOffRoadStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionPowerModeStatus();
        updateConstantTransmissionAutomaticManualStatus();
        updateEventShiftingToStageDMotor();
        updateEventShiftingToStageNMotor();
        updateEventShiftingToStageRMotor();
        updateEventShiftingToStageDAllisonATM();
        updateEventShiftingToStageNAllisonATM();
        updateEventShiftingToStageRAllisonATM();
    }

    void onTmAutomaticManualStatusChanged(const ArgumentsTmAutomaticManualStatusChanged& args) {
        mTMAutomaticManualStatus = args.mInput_TMAutomaticManualStatus;

        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
    }

    void onConfigSpeedLimit70MPHChanged(const ArgumentsConfigSpeedLimit70MPHChanged& args) {
        mConfigSpeedLimit70MPH = args.mInter_ConfigSpeedLimit70MPH;

        updatePrivateAllisonTransmissionModeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
    }

    void onConfigCVPRJTNameChanged(const ArgumentsConfigCVPRJTNameChanged& args) {
        mConfigCVPRJTName = args.mInter_ConfigCVPRJTName;

        updatePrivateAllisonTransmissionModeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updatePrivateAllisonTransmissionModeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
    }

    void onConfigVehicleTypeCVChanged(const ArgumentsConfigVehicleTypeCVChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;

        updateTelltaleOffRoadStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionPowerModeStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
    }

    void onManualModeIndcatorStatusChanged(const ArgumentsManualModeIndcatorStatusChanged& args) {
        mManualModeIndcatorStatus = args.mInput_ManualModeIndcatorStatus;

        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
    }

    void onTransmissionCurrentGearStatusChanged(const ArgumentsTransmissionCurrentGearStatusChanged& args) {
        mTransmissionCurrentGearStatus = args.mInput_TransmissionCurrentGearStatus;

        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
        updateEventShiftingToStageDMotor();
        updateEventShiftingToStageNMotor();
        updateEventShiftingToStageRMotor();
        updateSoundRPosition();
        mPrevTransmissionCurrentGearStatus = mTransmissionCurrentGearStatus;
    }

    void onTransmissionRequestedRangeStatusChanged(const ArgumentsTransmissionRequestedRangeStatusChanged& args) {
        mTransmissionRequestedRangeStatus = args.mInput_TransmissionRequestedRangeStatus;

        updatePrivateAllisonTransmissionModeStatus();
        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
    }

    void onOffRoadStatusChanged(const ArgumentsOffRoadStatusChanged& args) {
        mOffRoadStatus = args.mInput_OffRoadStatus;

        updateTelltaleOffRoadStatus();
    }

    void onRockingFreeStatusChanged(const ArgumentsRockingFreeStatusChanged& args) {
        mRockingFreeStatus = args.mInput_RockingFreeStatus;

        updateTelltaleRockingFreeStatus();
    }

    void onBusEcoRollStatusChanged(const ArgumentsBusEcoRollStatusChanged& args) {
        mBusEcoRollStatus = args.mInput_BusEcoRollStatus;

        updateConstantTransmissionCurrentGearStatus();
    }

    void onTruckEcoRollStatusChanged(const ArgumentsTruckEcoRollStatusChanged& args) {
        mTruckEcoRollStatus = args.mInput_TruckEcoRollStatus;

        updateConstantTransmissionCurrentGearStatus();
    }

    void onManoueveringStatusChanged(const ArgumentsManoueveringStatusChanged& args) {
        mManoueveringStatus = args.mInput_ManoueveringStatus;

        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionAutomaticManualStatus();
    }

    void onSwitchRequestedGearStatusChanged(const ArgumentsSwitchRequestedGearStatusChanged& args) {
        mSwitchRequestedGearStatus = args.mInput_SwitchRequestedGearStatus;

        updateConstantTransmissionCurrentGearStatus();
        updateConstantTransmissionTopGearStatus();
        updateEventShiftingToStageDAllisonATM();
        updateEventShiftingToStageNAllisonATM();
        updateEventShiftingToStageRAllisonATM();
        mPrevSwitchRequestedGearStatus = mSwitchRequestedGearStatus;
    }

    void onTransmissionZfPowerModeChanged(const ArgumentsTransmissionZfPowerModeChanged& args) {
        mZFPowerMode = args.mInput_ZFPowerMode;

        updateConstantTransmissionPowerModeStatus();
    }

    void onTransmissionSelectedGearStatusChanged(const ArgumentsTransmissionSelectedGearStatusChanged& args) {
        mTransmissionSelectedGearStatus = args.mInput_TransmissionSelectedGearStatus;

        updateConstantTransmissionCurrentGearStatus();
    }

    void onZfSelectModeStatusChanged(const ArgumentsZfSelectModeStatusChanged& args) {
        mZFSelectModeStatus = args.mInput_ZFSelectModeStatus;

        updateConstantTransmissionCurrentGearStatus();
    }

    void onAllisonSelectModeStatusChanged(const ArgumentsAllisonSelectModeStatusChanged& args) {
        mAllisonSelectModeStatus = args.mInput_AllisonSelectModeStatus;

        updateConstantTransmissionCurrentGearStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1 Allison T/M Auto-Manual Mode
    void updatePrivateAllisonTransmissionModeStatus() {
        if (mConfigTMType != ConfigTMType::ATM_ALLISON) {
            return;
        }

        mPrivateAllisonTransmissionModeStatus = PrivateAllisonTransmissionModeStatus::MANUAL;
        if (mConfigCVPRJTName == ConfigCVPRJTName::QZ) {
            if (mConfigVehicleType == ConfigVehicleType::FCEV) {
                if (mConfigSpeedLimit70MPH == ConfigSpeedLimit70MPH::OFF &&
                    mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_1 &&
                    mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_2 &&
                    mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_3 &&
                    mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_4) {
                    mPrivateAllisonTransmissionModeStatus = PrivateAllisonTransmissionModeStatus::AUTOMATIC;
                } else if (mConfigSpeedLimit70MPH == ConfigSpeedLimit70MPH::ON &&
                           mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_1 &&
                           mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_2 &&
                           mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_3 &&
                           mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_4 &&
                           mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_5) {
                    mPrivateAllisonTransmissionModeStatus = PrivateAllisonTransmissionModeStatus::AUTOMATIC;
                } else {
                    // no operation
                }
            }
        } else if (mConfigCVPRJTName == ConfigCVPRJTName::QT || mConfigCVPRJTName == ConfigCVPRJTName::CS) {
            if (mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_1 &&
                mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_2 &&
                mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_3 &&
                mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_4 &&
                mTransmissionRequestedRangeStatus != TransmissionRequestedRangeStatus::IND_5) {
                mPrivateAllisonTransmissionModeStatus = PrivateAllisonTransmissionModeStatus::AUTOMATIC;
            }
        } else {
            // no operation
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateTransmission_Indicator_CVAllisonTransmissionModeStat(
            static_cast<SFCPrivateTransmission_Indicator_CVAllisonTransmissionModeStat>(mPrivateAllisonTransmissionModeStatus));
#endif
    }

    // 5.1.1 OFF ROAD
    void updateTelltaleOffRoadStatus() {
        SFCTransmission_Indicator_CVTelltaleTelltaleOffRoadStat stat =
            SFCTransmission_Indicator_CVTelltaleTelltaleOffRoadStat::OFF;
        if (mIsIgnOn == true && mOffRoadStatus == OffRoadStatus::ON && mConfigTMType == ConfigTMType::AMT_ZF &&
            mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK) {
            stat = SFCTransmission_Indicator_CVTelltaleTelltaleOffRoadStat::ON;
        }
        setSFCTransmission_Indicator_CVTelltaleTelltaleOffRoadStat(stat);
    }

    // 5.1.2 ROCKING FREE
    void updateTelltaleRockingFreeStatus() {
        SFCTransmission_Indicator_CVTelltaleTelltaleRockingFreeStat stat =
            SFCTransmission_Indicator_CVTelltaleTelltaleRockingFreeStat::OFF;
        if (mIsIgnOn == true && mRockingFreeStatus == RockingFreeStatus::ON) {
            stat = SFCTransmission_Indicator_CVTelltaleTelltaleRockingFreeStat::ON;
        }
        setSFCTransmission_Indicator_CVTelltaleTelltaleRockingFreeStat(stat);
    }

    // 5.2.1.1 현재 기어 단수 표시
    void updateConstantTransmissionCurrentGearStatus() {
        bool timerOff = true;
        if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS && mConfigTMType == ConfigTMType::ATM_ZF) {
            updateZfBusStatus();
        } else if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mConfigTMType == ConfigTMType::AMT_ZF) {
            timerOff = false;
            updateZfTruckAmtStatus();
        } else if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mConfigTMType == ConfigTMType::ATM_ZF) {
            updateZfTruckAtmStatus();
        } else if (mConfigTMType == ConfigTMType::ATM_ALLISON && mConfigCVPRJTName != ConfigCVPRJTName::QV) {
            updateAllisonAtmStatusNotQV();
        } else if (mConfigTMType == ConfigTMType::ATM_ALLISON && mConfigCVPRJTName == ConfigCVPRJTName::QV) {
            updateAllisonAtmStatusQV();
        } else if (mConfigTMType == ConfigTMType::MTM) {
            updateMtmStatus();
        } else if (mConfigTMType == ConfigTMType::MOTOR) {
            updateMotorStatus();
        } else {
            // no operation
        }
        if (timerOff == true) {
            mCurrentGearStatOneShotTimer200msTimeout.stop();
        }
    }

    // 5.2.1.1.1 ZF BUS
    void updateZfBusStatus() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mBusEcoRollStatus == BusEcoRollStatus::ON &&
                mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
                mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_6 &&
                mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_1 &&
                mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_2 &&
                mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_3) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_E;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_6 &&
                       mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_1 &&
                       mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_2 &&
                       mSwitchRequestedGearStatus != SwitchRequestedGearStatus::DOWN_3) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_D;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_1 &&
                       (mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_1 ||
                        mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_2 ||
                        mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_3)) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_1;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_2 &&
                       (mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_1 ||
                        mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_2)) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_2;
            } else if (mBusEcoRollStatus == BusEcoRollStatus::OFF &&
                       mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_3 &&
                       mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_1) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_3;
            } else {
                // no operation
            }
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.1.2 ZF TRUCK_AMT
    void updateZfTruckAmtStatus() {
        bool timerOff = true;
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                if (mTruckEcoRollStatus == TruckEcoRollStatus::ON) {
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_E;
                } else {
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
                }
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                if (mManoueveringStatus == ManoueveringStatus::OFF) {
                    mConstantCurrentGearStat =
                        static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionCurrentGearStatus);
                } else {
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_RM;
                }
            } else if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_1) {
                if (mManoueveringStatus == ManoueveringStatus::OFF) {
                    timerOff = false;
                    if (mCurrentGearStatOneShotTimer200msTimeout.start() == false) {
                        DWarning() << "[Transmission_Indicator_CV] mCurrentGearStatOneShotTimer200msTimeout Start Failed";
                    }
                } else {
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_DM;
                }
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_2 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_12) {
                if (mManoueveringStatus == ManoueveringStatus::OFF) {
                    mConstantCurrentGearStat =
                        static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionCurrentGearStatus);
                } else {
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_DM;
                }
            } else {
                // no operation
            }
        }
        if (timerOff == true) {
            mCurrentGearStatOneShotTimer200msTimeout.stop();
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    void onCurrentGearStatOneShotTimer200msTimeout() {
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_1);
        flush();
    }

    // 5.2.1.1.3 ZF TRUCK_ATM
    void updateZfTruckAtmStatus() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            switch (mTransmissionCurrentGearStatus) {
                case TransmissionCurrentGearStatus::IND_N:
                    if (mTruckEcoRollStatus == TruckEcoRollStatus::ON) {
                        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_E;
                    } else {
                        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
                    }
                    break;
                case TransmissionCurrentGearStatus::IND_R1:
                case TransmissionCurrentGearStatus::IND_R2:
                case TransmissionCurrentGearStatus::IND_R3:
                case TransmissionCurrentGearStatus::IND_R4:
                    mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
                    break;
                case TransmissionCurrentGearStatus::IND_1:
                case TransmissionCurrentGearStatus::IND_2:
                case TransmissionCurrentGearStatus::IND_3:
                case TransmissionCurrentGearStatus::IND_4:
                case TransmissionCurrentGearStatus::IND_5:
                case TransmissionCurrentGearStatus::IND_6:
                case TransmissionCurrentGearStatus::IND_7:
                case TransmissionCurrentGearStatus::IND_8:
                    if (mTMAutomaticManualStatus == TMAutomaticManualStatus::AUTOMATIC) {
                        if (mZFSelectModeStatus == ZFSelectModeStatus::OFF) {
                            mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_D;
                        } else if (mZFSelectModeStatus == ZFSelectModeStatus::ON) {
                            if (mTransmissionSelectedGearStatus >= TransmissionSelectedGearStatus::IND_1 &&
                                mTransmissionSelectedGearStatus <= TransmissionSelectedGearStatus::IND_8) {
                                mConstantCurrentGearStat = static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(
                                    mTransmissionSelectedGearStatus);
                            }
                        } else {
                            // no operation
                        }
                    } else if (mTMAutomaticManualStatus == TMAutomaticManualStatus::MANUAL) {
                        mConstantCurrentGearStat =
                            static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionCurrentGearStatus);
                    } else {
                        // no operation
                    }
                    break;
                default:
                    break;
            }
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.1.4 ALLISON ATM
    // 5.2.1.1.4.1 ALLISON ATM : QV 외 차종
    void updateAllisonAtmStatusNotQV() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
            } else if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_P) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_P;
            } else if (mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_N &&
                       mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R2 &&
                       mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R3 &&
                       mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R4 &&
                       mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_P &&
                       mPrivateAllisonTransmissionModeStatus == PrivateAllisonTransmissionModeStatus::AUTOMATIC) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_D;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_9 &&
                       mPrivateAllisonTransmissionModeStatus == PrivateAllisonTransmissionModeStatus::MANUAL) {
                mConstantCurrentGearStat =
                    static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionCurrentGearStatus);
            } else {
                // no operation
            }
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.1.4.2 ALLISON ATM : QV 차종
    void updateAllisonAtmStatusQV() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_9) {
                if (mManualModeIndcatorStatus == ManualModeIndcatorStatus::AUTO) {
                    if (mAllisonSelectModeStatus == AllisonSelectModeStatus::OFF) {
                        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_D;
                    } else if (mAllisonSelectModeStatus == AllisonSelectModeStatus::ON &&
                               (mTransmissionSelectedGearStatus >= TransmissionSelectedGearStatus::IND_1 &&
                                mTransmissionSelectedGearStatus <= TransmissionSelectedGearStatus::IND_9)) {
                        mConstantCurrentGearStat =
                            static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionSelectedGearStatus);
                    } else {
                        // no operation
                    }
                } else if (mManualModeIndcatorStatus == ManualModeIndcatorStatus::MANUAL) {
                    mConstantCurrentGearStat =
                        static_cast<SFCTransmission_Indicator_CVConstantCurrentGearStat>(mTransmissionCurrentGearStatus);
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.1.5 MTM
    void updateMtmStatus() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
            } else {
                // no operation
            }
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.1.6 감속기 & 모터변속기 일체형
    void updateMotorStatus() {
        mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_N;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_R;
            } else if (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
                       mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_6) {
                mConstantCurrentGearStat = SFCTransmission_Indicator_CVConstantCurrentGearStat::IND_D;
            } else {
                // no operation
            }
        }
        setSFCTransmission_Indicator_CVConstantCurrentGearStat(mConstantCurrentGearStat);
    }

    // 5.2.1.2 탑단 제한 기어 표시
    void updateConstantTransmissionTopGearStatus() {
        SFCTransmission_Indicator_CVConstantTransmissionTopGearStat stat =
            SFCTransmission_Indicator_CVConstantTransmissionTopGearStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigTMType == ConfigTMType::ATM_ALLISON &&
                mPrivateAllisonTransmissionModeStatus == PrivateAllisonTransmissionModeStatus::MANUAL &&
                mTransmissionRequestedRangeStatus >= TransmissionRequestedRangeStatus::IND_1 &&
                mTransmissionRequestedRangeStatus <= TransmissionRequestedRangeStatus::IND_5) {
                stat =
                    static_cast<SFCTransmission_Indicator_CVConstantTransmissionTopGearStat>(mTransmissionRequestedRangeStatus);
            } else if (mConfigTMType == ConfigTMType::ATM_ZF && mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_3 &&
                       mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) {
                stat = SFCTransmission_Indicator_CVConstantTransmissionTopGearStat::IND_1;
            } else if (mConfigTMType == ConfigTMType::ATM_ZF && mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_2 &&
                       mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) {
                stat = SFCTransmission_Indicator_CVConstantTransmissionTopGearStat::IND_2;
            } else if (mConfigTMType == ConfigTMType::ATM_ZF && mSwitchRequestedGearStatus == SwitchRequestedGearStatus::DOWN_1 &&
                       mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) {
                stat = SFCTransmission_Indicator_CVConstantTransmissionTopGearStat::IND_3;
            } else {
                // no operation
            }
        }
        setSFCTransmission_Indicator_CVConstantTransmissionTopGearStat(stat);
    }

    // 5.2.1.3 TM Power Mode 표시
    void updateConstantTransmissionPowerModeStatus() {
        SFCTransmission_Indicator_CVConstantTransmissionPowerModeStat stat =
            SFCTransmission_Indicator_CVConstantTransmissionPowerModeStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ZF && mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS &&
            mZFPowerMode == ZFPowerMode::POWER) {
            stat = SFCTransmission_Indicator_CVConstantTransmissionPowerModeStat::MODE_P;
        }
        setSFCTransmission_Indicator_CVConstantTransmissionPowerModeStat(stat);
    }

    // 5.2.1.4 Auto/Manual 표시
    void updateConstantTransmissionAutomaticManualStatus() {
        bool timerOff = true;
        SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat stat =
            SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat::OFF;
        if (mIsIgnOn == true) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK) {
                if (mConfigTMType == ConfigTMType::AMT_ZF) {
                    if (mManoueveringStatus == ManoueveringStatus::OFF) {
                        if (mTMAutomaticManualStatus == TMAutomaticManualStatus::MANUAL &&
                            mTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_N) {
                            timerOff = false;
                            if (mAutomaticManualStatOneShotTimer200msTimeout.start() == false) {
                                DWarning()
                                    << "[Transmission_Indicator_CV] mAutomaticManualStatOneShotTimer200msTimeout Start Failed";
                            }
                        } else if (mTMAutomaticManualStatus == TMAutomaticManualStatus::AUTOMATIC) {
                            stat = SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat::AUTOMATIC;
                        } else {
                            // no operation
                        }
                    }
                } else if (mConfigTMType == ConfigTMType::ATM_ZF) {
                    if (mTMAutomaticManualStatus == TMAutomaticManualStatus::MANUAL) {
                        stat = SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat::MANUAL;
                    }
                } else if (mConfigTMType == ConfigTMType::ATM_ALLISON) {
                    if (mManualModeIndcatorStatus == ManualModeIndcatorStatus::MANUAL &&
                        mConfigCVPRJTName == ConfigCVPRJTName::QV) {
                        stat = SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat::MANUAL;
                    }
                } else {
                    // no operation
                }
            }
        }
        if (timerOff == true) {
            mAutomaticManualStatOneShotTimer200msTimeout.stop();
        }
        setSFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat(stat);
    }

    void onAutomaticManualStatOneShotTimer200msTimeout() {
        setSFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat(
            SFCTransmission_Indicator_CVConstantTransmissionAutomaticManualStat::MANUAL);
        flush();
    }

    // 5.3.1.1 D 단으로 변속시 - MOTOR
    void updateEventShiftingToStageDMotor() {
        SFCTransmission_Indicator_CVEventShiftingToStageDMotorStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageDMotorStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::MOTOR &&
            (mPrevTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_N &&
             mPrevTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4) &&
            (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
             mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_12)) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageDMotorStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageDMotorStat(stat);
    }

    // 5.3.1.2 N 단으로 변속시 - MOTOR
    void updateEventShiftingToStageNMotor() {
        SFCTransmission_Indicator_CVEventShiftingToStageNMotorStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageNMotorStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::MOTOR &&
            ((mPrevTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
              mPrevTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_12) ||
             (mPrevTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
              mPrevTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4)) &&
            mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_N) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageNMotorStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageNMotorStat(stat);
    }

    // 5.3.1.3 R 단으로 변속시 - MOTOR
    void updateEventShiftingToStageRMotor() {
        SFCTransmission_Indicator_CVEventShiftingToStageRMotorStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageRMotorStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::MOTOR &&
            (mPrevTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_1 &&
             mPrevTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_N) &&
            (mTransmissionCurrentGearStatus >= TransmissionCurrentGearStatus::IND_R1 &&
             mTransmissionCurrentGearStatus <= TransmissionCurrentGearStatus::IND_R4)) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageRMotorStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageRMotorStat(stat);
    }

    // 5.3.1.4 D 단으로 변속시 - Allison ATM
    void updateEventShiftingToStageDAllisonATM() {
        SFCTransmission_Indicator_CVEventShiftingToStageDAllisonATMStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageDAllisonATMStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ALLISON &&
            (mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_N ||
             mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_R) &&
            mSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_D) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageDAllisonATMStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageDAllisonATMStat(stat);
    }

    // 5.3.1.5 N 단으로 변속시 - Allison ATM
    void updateEventShiftingToStageNAllisonATM() {
        SFCTransmission_Indicator_CVEventShiftingToStageNAllisonATMStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageNAllisonATMStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ALLISON &&
            (mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_D ||
             mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_R) &&
            mSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_N) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageNAllisonATMStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageNAllisonATMStat(stat);
    }

    // 5.3.1.6 R 단으로 변속시 - Allison ATM
    void updateEventShiftingToStageRAllisonATM() {
        SFCTransmission_Indicator_CVEventShiftingToStageRAllisonATMStat stat =
            SFCTransmission_Indicator_CVEventShiftingToStageRAllisonATMStat::OFF;
        if (mIsIgnOn == true && mConfigTMType == ConfigTMType::ATM_ALLISON &&
            (mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_D ||
             mPrevSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_N) &&
            mSwitchRequestedGearStatus == SwitchRequestedGearStatus::IND_R) {
            stat = SFCTransmission_Indicator_CVEventShiftingToStageRAllisonATMStat::ON;
        }
        setSFCTransmission_Indicator_CVEventShiftingToStageRAllisonATMStat(stat);
    }

    // 5.4.1 R 단 진입음
    void updateSoundRPosition() {
        if (mIsIgnOn == true && ((mPrevTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R1 &&
                                  mPrevTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R2 &&
                                  mPrevTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R3 &&
                                  mPrevTransmissionCurrentGearStatus != TransmissionCurrentGearStatus::IND_R4) &&
                                 (mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R1 ||
                                  mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R2 ||
                                  mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R3 ||
                                  mTransmissionCurrentGearStatus == TransmissionCurrentGearStatus::IND_R4))) {
            setSFCTransmission_Indicator_CVSoundCurrentGearPositionStat(
                SFCTransmission_Indicator_CVSoundCurrentGearPositionStat::OFF);
            if (mOneShotTimerSound300msTimeout.start() == false) {
                DWarning() << "[Transmission_Indicator_CV] mOneShotTimerSound300msTimeout Start Failed";
            }
        } else {
            mOneShotTimerSound300msTimeout.stop();
            setSFCTransmission_Indicator_CVSoundCurrentGearPositionStat(
                SFCTransmission_Indicator_CVSoundCurrentGearPositionStat::OFF);
        }
    }

    void onOneShotTimerSound300msTimeout() {
        DDebug() << "onOneShotTimerSound300msTimeout()";
        setSFCTransmission_Indicator_CVSoundCurrentGearPositionStat(SFCTransmission_Indicator_CVSoundCurrentGearPositionStat::ON);
        flush();
    }

    enum class PrivateAllisonTransmissionModeStatus : HUInt64 {
        NONE,
        AUTOMATIC,
        MANUAL,
        MAX
    };

    HBool mIsIgnOn = false;
    using ConfigSpeedLimit70MPH = ArgumentsConfigSpeedLimit70MPHChanged::Inter_ConfigSpeedLimit70MPH;
    using ConfigTMType = ArgumentsConfigTMTypeChanged::Inter_ConfigTMType;
    using TransmissionRequestedRangeStatus =
        ArgumentsTransmissionRequestedRangeStatusChanged::Input_TransmissionRequestedRangeStatus;
    using ManualModeIndcatorStatus = ArgumentsManualModeIndcatorStatusChanged::Input_ManualModeIndcatorStatus;
    using TMAutomaticManualStatus = ArgumentsTmAutomaticManualStatusChanged::Input_TMAutomaticManualStatus;
    using TransmissionCurrentGearStatus = ArgumentsTransmissionCurrentGearStatusChanged::Input_TransmissionCurrentGearStatus;
    using ConfigCVPRJTName = ArgumentsConfigCVPRJTNameChanged::Inter_ConfigCVPRJTName;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigVehicleTypeCV = ArgumentsConfigVehicleTypeCVChanged::Inter_ConfigVehicleTypeCV;
    using OffRoadStatus = ArgumentsOffRoadStatusChanged::Input_OffRoadStatus;
    using RockingFreeStatus = ArgumentsRockingFreeStatusChanged::Input_RockingFreeStatus;
    using BusEcoRollStatus = ArgumentsBusEcoRollStatusChanged::Input_BusEcoRollStatus;
    using SwitchRequestedGearStatus = ArgumentsSwitchRequestedGearStatusChanged::Input_SwitchRequestedGearStatus;
    using TruckEcoRollStatus = ArgumentsTruckEcoRollStatusChanged::Input_TruckEcoRollStatus;
    using ManoueveringStatus = ArgumentsManoueveringStatusChanged::Input_ManoueveringStatus;
    using ZFPowerMode = ArgumentsTransmissionZfPowerModeChanged::Input_ZFPowerMode;
    using TransmissionSelectedGearStatus = ArgumentsTransmissionSelectedGearStatusChanged::Input_TransmissionSelectedGearStatus;
    using ZFSelectModeStatus = ArgumentsZfSelectModeStatusChanged::Input_ZFSelectModeStatus;
    using AllisonSelectModeStatus = ArgumentsAllisonSelectModeStatusChanged::Input_AllisonSelectModeStatus;

    ConfigSpeedLimit70MPH mConfigSpeedLimit70MPH = ConfigSpeedLimit70MPH::OFF;
    ConfigTMType mConfigTMType = ConfigTMType::MTM;
    ConfigCVPRJTName mConfigCVPRJTName = ConfigCVPRJTName::QZ;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
    ManualModeIndcatorStatus mManualModeIndcatorStatus = ManualModeIndcatorStatus::MANUAL;
    OffRoadStatus mOffRoadStatus = OffRoadStatus::OFF;
    RockingFreeStatus mRockingFreeStatus = RockingFreeStatus::OFF;
    BusEcoRollStatus mBusEcoRollStatus = BusEcoRollStatus::OFF;
    SwitchRequestedGearStatus mSwitchRequestedGearStatus = SwitchRequestedGearStatus::OFF;
    SwitchRequestedGearStatus mPrevSwitchRequestedGearStatus = SwitchRequestedGearStatus::OFF;
    TruckEcoRollStatus mTruckEcoRollStatus = TruckEcoRollStatus::OFF;
    ManoueveringStatus mManoueveringStatus = ManoueveringStatus::OFF;
    ZFPowerMode mZFPowerMode = ZFPowerMode::OFF;

    TransmissionRequestedRangeStatus mTransmissionRequestedRangeStatus = TransmissionRequestedRangeStatus::NO_DISPLAY;
    TMAutomaticManualStatus mTMAutomaticManualStatus = TMAutomaticManualStatus::OFF;
    TransmissionCurrentGearStatus mTransmissionCurrentGearStatus = TransmissionCurrentGearStatus::NO_DISPLAY;
    TransmissionCurrentGearStatus mPrevTransmissionCurrentGearStatus = TransmissionCurrentGearStatus::NO_DISPLAY;

    PrivateAllisonTransmissionModeStatus mPrivateAllisonTransmissionModeStatus = PrivateAllisonTransmissionModeStatus::NONE;
    SFCTransmission_Indicator_CVConstantCurrentGearStat mConstantCurrentGearStat =
        SFCTransmission_Indicator_CVConstantCurrentGearStat::OFF;

    ssfs::SFCTimer<Transmission_Indicator_CV> mOneShotTimerSound300msTimeout;
    static constexpr uint32_t kTimerInterval300ms = 300u;
    ssfs::SFCTimer<Transmission_Indicator_CV> mCurrentGearStatOneShotTimer200msTimeout;
    ssfs::SFCTimer<Transmission_Indicator_CV> mAutomaticManualStatOneShotTimer200msTimeout;
    static constexpr uint32_t kTimerInterval200ms = 200u;

    TransmissionSelectedGearStatus mTransmissionSelectedGearStatus = TransmissionSelectedGearStatus::NO_DISPLAY;
    ZFSelectModeStatus mZFSelectModeStatus = ZFSelectModeStatus::OFF;
    AllisonSelectModeStatus mAllisonSelectModeStatus = AllisonSelectModeStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Transmission_Indicator_CV_H
