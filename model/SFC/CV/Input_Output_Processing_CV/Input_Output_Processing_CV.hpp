/**
 * @file Input_Output_Processing_CV.hpp
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
#ifndef SFSS_Input_Output_Processing_CV_H
#define SFSS_Input_Output_Processing_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Input_Output_Processing_CVBase.hpp"
#include <SFCTimer.h>

#include <array>

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.18
// Reference : [CV100] Input_Output_Processing_CV. Contains : Constant. Recommend: VALUE_CHANGED
class Input_Output_Processing_CV : public Input_Output_Processing_CVBase {
public:
    Input_Output_Processing_CV() = default;
    ~Input_Output_Processing_CV() override = default;
    Input_Output_Processing_CV(const Input_Output_Processing_CV& other) = delete;
    Input_Output_Processing_CV(Input_Output_Processing_CV&& other) noexcept = delete;
    Input_Output_Processing_CV& operator=(const Input_Output_Processing_CV& other) = delete;
    Input_Output_Processing_CV& operator=(Input_Output_Processing_CV&& other) noexcept = delete;

    void onInitialize() override {
        // 4.3.7.1 초기값 (공장 초기값)
        setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(0.0);

        if (mOneShotTimerFuelErrorDetectionStatus.create(kTimerFuelErrorDetectionInterval120s, this,
                                                         &Input_Output_Processing_CV::onOneShotTimerFuelErrorDetectionStatus,
                                                         false) == false) {
            DWarning() << "[Input_Output_Processing_CV] mOneShotTimerFuelErrorDetectionStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            // mArrPar_FuelSender[0] <- Not Used. padding space.
            mArrPar_FuelSender[1] = args.mPar_FuelSender1;
            mArrPar_FuelSender[2] = args.mPar_FuelSender2;
            mArrPar_FuelSender[3] = args.mPar_FuelSender3;
            mArrPar_FuelSender[4] = args.mPar_FuelSender4;
            mArrPar_FuelSender[5] = args.mPar_FuelSender5;
            mArrPar_FuelSender[6] = args.mPar_FuelSender6;
            mArrPar_FuelSender[7] = args.mPar_FuelSender7;
            mArrPar_FuelSender[8] = args.mPar_FuelSender8;
            mArrPar_FuelSender[9] = args.mPar_FuelSender9;
            mArrPar_FuelSender[10] = args.mPar_FuelSender10;
            mArrPar_FuelSender[11] = args.mPar_FuelSender11;

            // mArrPar_FuelSenderVolume[0] <- Not Used. padding space.
            mArrPar_FuelSenderVolume[1] = args.mPar_FuelSenderVolume1;
            mArrPar_FuelSenderVolume[2] = args.mPar_FuelSenderVolume2;
            mArrPar_FuelSenderVolume[3] = args.mPar_FuelSenderVolume3;
            mArrPar_FuelSenderVolume[4] = args.mPar_FuelSenderVolume4;
            mArrPar_FuelSenderVolume[5] = args.mPar_FuelSenderVolume5;
            mArrPar_FuelSenderVolume[6] = args.mPar_FuelSenderVolume6;
            mArrPar_FuelSenderVolume[7] = args.mPar_FuelSenderVolume7;
            mArrPar_FuelSenderVolume[8] = args.mPar_FuelSenderVolume8;
            mArrPar_FuelSenderVolume[9] = args.mPar_FuelSenderVolume9;
            mArrPar_FuelSenderVolume[10] = args.mPar_FuelSenderVolume10;
            mArrPar_FuelSenderVolume[11] = args.mPar_FuelSenderVolume11;

            mPar_FuelSenderShort = args.mPar_FuelSenderShort;
            mPar_FuelSenderOpen = args.mPar_FuelSenderOpen;

            updatePrivateCurrentFuelSection();
            updateInter_FuelErrorDetectionStatus();

            // 4.3.7.3 판단값 저장 및 불러오기 (Memory -> Inter)
            HDouble targetFuelEcoValue = 0.0;
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_TargetFuelEcoValue, targetFuelEcoValue);
            setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(targetFuelEcoValue);
            DDebug() << "LoadFuelValue(targetFuelEcoValue): " << static_cast<HDouble>(targetFuelEcoValue);
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        if (mAEMInitializeComplete == true) {
            updateInter_FuelErrorDetectionStatus();
        }
        updateInter_ActualVehicleSpeedValidity();
        updateInter_ActualVehicleSpeed();
        updateDrivingOnStatus();
        updateEngineOnStatus();
        updateIsgActiveStatus();
        updateTelematicsActiveStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        if (mAEMInitializeComplete == true) {
            updateInter_FuelErrorDetectionStatus();
        }
        updateInter_ActualVehicleSpeedValidity();
        updateInter_ActualVehicleSpeed();
        updateDrivingOnStatus();
        updateEngineOnStatus();
        updateIsgActiveStatus();
        updateTelematicsActiveStatus();
    }

    void onFuelSenderChanged(const ArgumentsFuelSenderChanged& args) {
        mFuelSenderValue = static_cast<HDouble>(args.mInput_FuelSenderValue) * 0.001;

        if (mAEMInitializeComplete == true) {
            updatePrivateCurrentFuelSection();
            updateInter_FuelErrorDetectionStatus();
        }
    }

    void onRheostatSwitchChanged(const ArgumentsRheostatSwitchChanged& args) {
        mRheostatSwUpStatus = args.mInput_RheostatSwUpStatus;
        mRheostatSwDownStatus = args.mInput_RheostatSwDownStatus;
        updateInter_RheostatSwitchStatus();
    }

    void onWheelSpeedValueChanged(const ArgumentsWheelSpeedValueChanged& args) {
        mWheelBasedVehicleSpeedValue = args.mInput_WheelBasedVehicleSpeedValue;
        updateInter_ActualVehicleSpeedValidity();
        updateInter_ActualVehicleSpeed();
        updateDrivingOnStatus();
    }

    void onEngineOnStatusDataChanged(const ArgumentsEngineOnStatusDataChanged& args) {
        mVehicleReadyStatus = args.mInput_VehicleReadyStatus;
        updateEngineOnStatus();
    }

    void onIsgActiveStatusDataChanged(const ArgumentsIsgActiveStatusDataChanged& args) {
        mEngineISGStatus1 = args.mInput_EngineISGStatus1;
        mEngineISGStatus2 = args.mInput_EngineISGStatus2;
        updateIsgActiveStatus();
    }

    void onTelematicsActiveStatusDataChanged(const ArgumentsTelematicsActiveStatusDataChanged& args) {
        if (ISTIMEOUT(args.mInput_TelematicsActiveStatus) == true) {
            mTelematicsActiveStatus = TelematicsActiveStatus::UNHANDLED_TIMEOUT;
        } else {
            mTelematicsActiveStatus = args.mInput_TelematicsActiveStatus;
        }

        updateTelematicsActiveStatus();
    }

    void onTargetFuelEconomyValueChanged(const ArgumentsTargetFuelEconomyValueChanged& args) {
        HUInt64 value = 0;
        //
        if (updateValueIfValid(value, args.mInput_TargetFuelEconomyValue) == true) {
            mTargetFuelEconomyValue = static_cast<HDouble>(value);
            updateAndSaveMemoryFuelEcoTargetValue();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.1.3.3.2 연료 구간별 Parameter 값 불러오기
    // 4.1.3.3.3 연료량 계산
    void updatePrivateCurrentFuelSection() {
        // mArrPar_FuelSender[0], mArrPar_FuelSenderVolume[0] <- Not Used. padding space.
        if (mFuelSenderValue <= mArrPar_FuelSender[1]) {
            mPrivateCurrentFuelSection = 0;
        } else if (mArrPar_FuelSender[1] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[2]) {
            mPrivateCurrentFuelSection = 1;
        } else if (mArrPar_FuelSender[2] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[3]) {
            mPrivateCurrentFuelSection = 2;
        } else if (mArrPar_FuelSender[3] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[4]) {
            mPrivateCurrentFuelSection = 3;
        } else if (mArrPar_FuelSender[4] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[5]) {
            mPrivateCurrentFuelSection = 4;
        } else if (mArrPar_FuelSender[5] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[6]) {
            mPrivateCurrentFuelSection = 5;
        } else if (mArrPar_FuelSender[6] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[7]) {
            mPrivateCurrentFuelSection = 6;
        } else if (mArrPar_FuelSender[7] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[8]) {
            mPrivateCurrentFuelSection = 7;
        } else if (mArrPar_FuelSender[8] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[9]) {
            mPrivateCurrentFuelSection = 8;
        } else if (mArrPar_FuelSender[9] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[10]) {
            mPrivateCurrentFuelSection = 9;
        } else if (mArrPar_FuelSender[10] < mFuelSenderValue && mFuelSenderValue <= mArrPar_FuelSender[11]) {
            mPrivateCurrentFuelSection = 10;
        } else if (mArrPar_FuelSender[11] < mFuelSenderValue) {
            mPrivateCurrentFuelSection = 11;
        } else {
            // no operation
        }

        HDouble value = 0.0;

        if (mPrivateCurrentFuelSection == 0) {
            value = mArrPar_FuelSenderVolume[1];
        } else if (mPrivateCurrentFuelSection == 11) {
            value = mArrPar_FuelSenderVolume[11];
        } else {
            mPrivate_CurrentFuelSectionInterval =
                mArrPar_FuelSender[mPrivateCurrentFuelSection + 1] - mArrPar_FuelSender[mPrivateCurrentFuelSection];
            value = (mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection + 1] -
                     mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection]) *
                        (mFuelSenderValue - mArrPar_FuelSender[mPrivateCurrentFuelSection]) /
                        mPrivate_CurrentFuelSectionInterval +
                    mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection];
        }
        setSFCInput_Output_ProcessingInter_FuelInputLevelValue(value);
    }

    // 4.1.3.3.4 연료 입력 오류 판단 처리
    void updateInter_FuelErrorDetectionStatus() {
        if (mIsIgnOn == true) {
            if (mPrevFuelErrorDetectionStatus == FuelErrorDetectionStatus::NORMAL &&
                (mFuelSenderValue <= mPar_FuelSenderShort || mFuelSenderValue >= mPar_FuelSenderOpen)) {
                mFuelErrorDetectionStatus = FuelErrorDetectionStatus::HOLD;
                mOneShotTimerFuelErrorDetectionStatus.setInterval(kTimerFuelErrorDetectionInterval120s);
                if (mOneShotTimerFuelErrorDetectionStatus.start() == false) {
                    DWarning() << "[Input_Output_Processing_CV] mOneShotTimerFuelErrorDetectionStatus[120s] Start Failed";
                }
            } else if (mPrevFuelErrorDetectionStatus == FuelErrorDetectionStatus::HOLD &&
                       (mFuelSenderValue >= mArrPar_FuelSender[1] && mFuelSenderValue <= mArrPar_FuelSender[11])) {
                mFuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
                mOneShotTimerFuelErrorDetectionStatus.stop();
            } else if (mPrevFuelErrorDetectionStatus == FuelErrorDetectionStatus::ERROR &&
                       (mFuelSenderValue >= mArrPar_FuelSender[1] && mFuelSenderValue <= mArrPar_FuelSender[11])) {
                mOneShotTimerFuelErrorDetectionStatus.setInterval(kTimerFuelErrorDetectionInterval20s);
                if (mOneShotTimerFuelErrorDetectionStatus.start() == false) {
                    DWarning() << "[Input_Output_Processing_CV] mOneShotTimerFuelErrorDetectionStatus[20s] Start Failed";
                }
            } else {
                // no operation
            }
        } else {
            mFuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
            mOneShotTimerFuelErrorDetectionStatus.stop();
        }
        setSFCInput_Output_ProcessingInter_FuelErrorDetectionStatus(mFuelErrorDetectionStatus);
        mPrevFuelErrorDetectionStatus = mFuelErrorDetectionStatus;
    }

    void onOneShotTimerFuelErrorDetectionStatus() {
        if (mPrevFuelErrorDetectionStatus == FuelErrorDetectionStatus::HOLD) {
            mFuelErrorDetectionStatus = FuelErrorDetectionStatus::ERROR;
        } else if (mPrevFuelErrorDetectionStatus == FuelErrorDetectionStatus::ERROR) {
            mFuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
        } else {
            // no operation
        }

        setSFCInput_Output_ProcessingInter_FuelErrorDetectionStatus(mFuelErrorDetectionStatus);
        mPrevFuelErrorDetectionStatus = mFuelErrorDetectionStatus;
        flush();
    }

    // 4.3.1.1 Rheostat Switch Input determination
    void updateInter_RheostatSwitchStatus() {
        SFCInput_Output_ProcessingInter_RheostatSwitchStatus stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::DEFAULT;

        if (mRheostatSwUpStatus == RheostatSwUpStatus::ON && mRheostatSwDownStatus == RheostatSwDownStatus::OFF) {
            stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::UP;
        } else if (mRheostatSwUpStatus == RheostatSwUpStatus::OFF && mRheostatSwDownStatus == RheostatSwDownStatus::ON) {
            stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::DOWN;
        } else {
            // no operation
        }
        setSFCInput_Output_ProcessingInter_RheostatSwitchStatus(stat);
    }

    // 4.3.2.1 유효값 판단
    void updateInter_ActualVehicleSpeedValidity() {
        mActualVehicleSpeedValidity = SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity::VALID;
        if (mIsIgnOn == true && mWheelBasedVehicleSpeedValue > kWheelBasedVehicleSpeedValidValue) {
            mActualVehicleSpeedValidity = SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity::INVALID;
        }
        setSFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity(mActualVehicleSpeedValidity);
    }

    // 4.3.2.4 최종 속도 계산
    void updateInter_ActualVehicleSpeed() {
        if (mIsIgnOn == true &&
            mActualVehicleSpeedValidity == SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity::VALID) {
            mActualVehicleSpeed = static_cast<HDouble>(mWheelBasedVehicleSpeedValue) / 256.0;
        } else {
            mActualVehicleSpeed = 0.0;
        }
        updateInterActualVehicleSpeedCompareValueTo10();  // 4.3.8 차속 10kph 이상 판단
        setSFCInput_Output_ProcessingInter_ActualVehicleSpeed(mActualVehicleSpeed);
    }

    // 4.3.3 주행, 정차, 시동 판단
    void updateDrivingOnStatus() {
        DrivingOnStatus stat = mPrevDrivingOnStatus;

        if (mIsIgnOn == true) {
            if (mPrevDrivingOnStatus == DrivingOnStatus::OFF) {
                if (mActualVehicleSpeed >= 1.5) {
                    stat = DrivingOnStatus::ON;
                }
            } else if (mPrevDrivingOnStatus == DrivingOnStatus::ON) {
                if (mActualVehicleSpeed <= 1.0) {
                    stat = DrivingOnStatus::OFF;
                }
            } else {
                // no operation
            }
        } else {
            stat = DrivingOnStatus::OFF;
        }

        setSFCInput_Output_ProcessingInter_DrivingOnStatus(stat);
        mPrevDrivingOnStatus = stat;
    }

    // 4.3.4 시동 판단
    void updateEngineOnStatus() {
        SFCInput_Output_ProcessingInter_EngineOnStatus stat = SFCInput_Output_ProcessingInter_EngineOnStatus::OFF;

        if (mIsIgnOn == true && mVehicleReadyStatus == VehicleReadyStatus::READY) {
            stat = SFCInput_Output_ProcessingInter_EngineOnStatus::ON;
        }
        setSFCInput_Output_ProcessingInter_EngineOnStatus(stat);
    }

    // 4.3.5 ISG 활성화 판단
    void updateIsgActiveStatus() {
        SFCInput_Output_ProcessingInter_ISGActiveStatus stat = SFCInput_Output_ProcessingInter_ISGActiveStatus::OFF;

        if (mIsIgnOn == true &&
            ((mEngineISGStatus1 == EngineISGStatus1::AUTO_STOP || mEngineISGStatus1 == EngineISGStatus1::AUTO_RESTART ||
              mEngineISGStatus1 == EngineISGStatus1::NOT_RESTARTABLE) ||
             (mEngineISGStatus2 == EngineISGStatus2::RAPID_ACCEL))) {
            stat = SFCInput_Output_ProcessingInter_ISGActiveStatus::ON;
        }
        setSFCInput_Output_ProcessingInter_ISGActiveStatus(stat);
    }

    // 4.3.6 CCS 개통 여부 판단
    void updateTelematicsActiveStatus() {
        if (mIsIgnOn == false) {
            return;
        }
        SFCInput_Output_ProcessingInter_TelematicsActiveStatus stat = mPrevTelematicsActiveStatus;

        if (mTelematicsActiveStatus == TelematicsActiveStatus::ACTIVE &&
            mPrevTelematicsActiveStatus == SFCInput_Output_ProcessingInter_TelematicsActiveStatus::INACTIVE) {
            stat = SFCInput_Output_ProcessingInter_TelematicsActiveStatus::ACTIVE;
        } else if (mTelematicsActiveStatus == TelematicsActiveStatus::UNHANDLED_TIMEOUT &&
                   mPrevTelematicsActiveStatus == SFCInput_Output_ProcessingInter_TelematicsActiveStatus::ACTIVE) {
            stat = SFCInput_Output_ProcessingInter_TelematicsActiveStatus::ACTIVE;
        } else if (mTelematicsActiveStatus == TelematicsActiveStatus::INACTIVE) {
            stat = SFCInput_Output_ProcessingInter_TelematicsActiveStatus::INACTIVE;
        } else {
            // no operation
        }

        setSFCInput_Output_ProcessingInter_TelematicsActiveStatus(stat);
        mPrevTelematicsActiveStatus = stat;
    }

    // 4.3.7.2 판단값 변경
    // 4.3.7.3 판단값 저장 및 불러오기 (Inter -> Memory)
    void updateAndSaveMemoryFuelEcoTargetValue() {
        if (almostEqual(mTargetFuelEconomyValue, 0.0) == false && mIsIgnOn == true) {
            setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(mTargetFuelEconomyValue * 0.1);
            // Save and Load Logic
            if (mAEMInitializeComplete == true) {
                if (almostEqual(mTargetFuelEconomyValue, mPrevTargetFuelEconomyValue) == false) {
                    // Inter -> Memory
                    updateValue("SFC.Extension.Memory.Inter_TargetFuelEcoValue", mTargetFuelEconomyValue * 0.1);
                    DDebug() << "SaveFuelValue(mInter_TargetFuelEcoValue): " << mTargetFuelEconomyValue * 0.1;
                }
            }
            mPrevTargetFuelEconomyValue = mTargetFuelEconomyValue;
        }
    }

    // 4.3.8 차속 10kph 이상 판단
    void updateInterActualVehicleSpeedCompareValueTo10() {
        if (surelyGreaterThan(mActualVehicleSpeed, kActualVehicleSpeedValue10) == true ||
            almostEqual(mActualVehicleSpeed, kActualVehicleSpeedValue10) == true) {
            setSFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10(
                SFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10::OVER);
        } else {
            setSFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10(
                SFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10::UNDER);
        }
    }

    HBool mAEMInitializeComplete = false;
    HBool mIsIgnOn = false;

    using FuelErrorDetectionStatus = Input_Output_Processing_CV::SFCInput_Output_ProcessingInter_FuelErrorDetectionStatus;
    using RheostatSwUpStatus = ArgumentsRheostatSwitchChanged::Input_RheostatSwUpStatus;
    using RheostatSwDownStatus = ArgumentsRheostatSwitchChanged::Input_RheostatSwDownStatus;
    using DrivingOnStatus = Input_Output_Processing_CV::SFCInput_Output_ProcessingInter_DrivingOnStatus;
    using VehicleReadyStatus = ArgumentsEngineOnStatusDataChanged::Input_VehicleReadyStatus;
    using EngineISGStatus1 = ArgumentsIsgActiveStatusDataChanged::Input_EngineISGStatus1;
    using EngineISGStatus2 = ArgumentsIsgActiveStatusDataChanged::Input_EngineISGStatus2;
    using TelematicsActiveStatus = ArgumentsTelematicsActiveStatusDataChanged::Input_TelematicsActiveStatus;

    FuelErrorDetectionStatus mFuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
    FuelErrorDetectionStatus mPrevFuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
    RheostatSwUpStatus mRheostatSwUpStatus = RheostatSwUpStatus::OFF;
    RheostatSwDownStatus mRheostatSwDownStatus = RheostatSwDownStatus::OFF;
    DrivingOnStatus mPrevDrivingOnStatus = DrivingOnStatus::NONE;
    VehicleReadyStatus mVehicleReadyStatus = VehicleReadyStatus::OFF;
    EngineISGStatus1 mEngineISGStatus1 = EngineISGStatus1::OFF;
    EngineISGStatus2 mEngineISGStatus2 = EngineISGStatus2::OFF;
    TelematicsActiveStatus mTelematicsActiveStatus = TelematicsActiveStatus::INACTIVE;
    SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity mActualVehicleSpeedValidity =
        SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity::INVALID;
    SFCInput_Output_ProcessingInter_TelematicsActiveStatus mPrevTelematicsActiveStatus =
        SFCInput_Output_ProcessingInter_TelematicsActiveStatus::INACTIVE;

    HUInt64 mPrivateCurrentFuelSection = 11;
    HUInt64 mWheelBasedVehicleSpeedValue = 0;
    HDouble mPrivate_CurrentFuelSectionInterval = 0.0;
    HDouble mFuelSenderValue = 0.0;
    HDouble mPar_FuelSenderShort = 0.0;
    HDouble mPar_FuelSenderOpen = 0.0;
    HDouble mActualVehicleSpeed = 0.0;
    HDouble mTargetFuelEconomyValue = 0.0;
    HDouble mPrevTargetFuelEconomyValue = 0.0;
    std::array<HDouble, 12> mArrPar_FuelSender = {
        0.0,
    };
    std::array<HDouble, 12> mArrPar_FuelSenderVolume = {
        0.0,
    };

    ssfs::SFCTimer<Input_Output_Processing_CV> mOneShotTimerFuelErrorDetectionStatus;
    static constexpr uint32_t kTimerFuelErrorDetectionInterval120s = 120000u;
    static constexpr uint32_t kTimerFuelErrorDetectionInterval20s = 20000u;
    static constexpr uint32_t kWheelBasedVehicleSpeedValidValue = 0xFAFF;
    static constexpr HDouble kActualVehicleSpeedValue10 = 10.0;
};

}  // namespace ccos

#endif  // SFSS_Input_Output_Processing_CV_H
