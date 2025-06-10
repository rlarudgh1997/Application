/**
 * @file Input_Output_Processing.hpp
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
#ifndef SFSS_Input_Output_Processing_H
#define SFSS_Input_Output_Processing_H

#define DLOG_ENABLED gEnableSFCLog

#include "Input_Output_ProcessingBase.hpp"
#include <SFCTimer.h>

#include <array>

namespace ccos {

// SFC Version : 44.0.2
// Reference : [CD060] Input_Output_Processing. Contains : Constant. Recommend: VALUE_CHANGED
class Input_Output_Processing : public Input_Output_ProcessingBase {
public:
    Input_Output_Processing() = default;
    ~Input_Output_Processing() override = default;
    Input_Output_Processing(const Input_Output_Processing& other) = delete;
    Input_Output_Processing(Input_Output_Processing&& other) noexcept = delete;
    Input_Output_Processing& operator=(const Input_Output_Processing& other) = delete;
    Input_Output_Processing& operator=(Input_Output_Processing&& other) noexcept = delete;

    void onInitialize() override {
        setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(0.0);

        if (mOneShotTimerFuelErrorDetectionStatus.create(kTimerFuelErrorDetectionInterval120s, this,
                                                         &Input_Output_Processing::onOneShotTimerFuelErrorDetectionStatus,
                                                         false) == false) {
            DWarning() << "[Input_Output_Processing] mOneShotTimerFuelErrorDetectionStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnStatus = true;

        processPrivate_FuelErrorDetectionStatus();
        processInter_FuelErrorDetectionStatus();
        processIsgActiveStatus();
        processEngineOnStatus();
        processWheelSpeedValue();
        processValidWheelCount();
        processWheelSpeedNoiseStatus();
        processActualVehicleSpeed();
        processDrivingOnStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnStatus = false;

        processIsgActiveStatus();
        processEngineOnStatus();
        processWheelSpeedValue();
        processValidWheelCount();
        processWheelSpeedNoiseStatus();
        processActualVehicleSpeed();
        processDrivingOnStatus();
    }

    void onWheelSpeedValueChanged(const ArgumentsWheelSpeedValueChanged& args) {
        mWheelSpeedFrontLeftValueSource = args.mInput_WheelSpeedFrontLeftValue;
        mWheelSpeedFrontRightValueSource = args.mInput_WheelSpeedFrontRightValue;
        mWheelSpeedRearLeftValueSource = args.mInput_WheelSpeedRearLeftValue;
        mWheelSpeedRearRightValueSource = args.mInput_WheelSpeedRearRightValue;

        processWheelSpeedValue();
        processValidWheelCount();
        processWheelSpeedNoiseStatus();
        processActualVehicleSpeed();
        processDrivingOnStatus();
    }

    void onEvServiceModeChanged(const ArgumentsEvServiceModeChanged& args) {
        // true on EV Types. NOT ICV Type
        if (updateValueIfValid(mEvServiceModeStatus, args.mInput_EvServiceModeStatus) == true) {
            processValidWheelCount();
            processActualVehicleSpeed();
            processDrivingOnStatus();
        }
    }

    void onEngineOnStatusDataChanged(const ArgumentsEngineOnStatusDataChanged& args) {
        mVehicleReadyStatus = args.mInput_VehicleReadyStatus;
        mSscControlStatus = args.mInput_SscControlStatus;

        processEngineOnStatus();
    }

    void onIsgActiveStatusDataChanged(const ArgumentsIsgActiveStatusDataChanged& args) {
        mEngineISGStatus1 = args.mInput_EngineISGStatus1;
        mEngineISGStatus2 = args.mInput_EngineISGStatus2;

        processIsgActiveStatus();
    }

    void onRheostatSwitchChanged(const ArgumentsRheostatSwitchChanged& args) {
        mRheostatSwUpStatus = args.mInput_RheostatSwUpStatus;
        mRheostatSwDownStatus = args.mInput_RheostatSwDownStatus;
        mRheostatSwitchReqStatus = args.mInput_RheostatSwitchReqStatus;

        updateRheostatSwitchStatus();
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        HDouble mInter_TargetFuelEcoValue = 0.0;

        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            // Note(andre): mArrPar_FuelSender[0] <- Not Used. padding space.
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

            // Note(andre): mArrPar_FuelSenderVolume[0] <- Not Used. padding space.
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

            processPrivate_FuelErrorDetectionStatus();
            processInter_FuelErrorDetectionStatus();
            processPrivateCurrentFuelSection();

            // Memory -> Inter
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_TargetFuelEcoValue, mInter_TargetFuelEcoValue);
            setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(mInter_TargetFuelEcoValue);
            DDebug() << "LoadFuelValue(mInter_TargetFuelEcoValue): " << static_cast<HDouble>(mInter_TargetFuelEcoValue);
        }

        // Call flush() to apply all updated value.
    }

    void onFuelSenderChanged(const ArgumentsFuelSenderChanged& args) {
        mFuelSenderValue = static_cast<HDouble>(args.mInput_FuelSenderValue) * 0.001;

        if (mAEMInitializeComplete == true) {
            processPrivate_FuelErrorDetectionStatus();
            processInter_FuelErrorDetectionStatus();
            processPrivateCurrentFuelSection();
        }
    }

    void onTargetFuelEconomyValueChanged(const ArgumentsTargetFuelEconomyValueChanged& args) {
        // 5.5.4.3.7.2 Value Changed
        if (ISTIMEOUT(args.mInput_TargetFuelEconomyValue) == true) {
            mTargetFuelEconomyValue = 0.0;
        } else {
            mTargetFuelEconomyValue = static_cast<HDouble>(args.mInput_TargetFuelEconomyValue);
        }
        if (almostEqual(mTargetFuelEconomyValue, 0.0) == false && mIgnStatus == true) {
            setSFCInput_Output_ProcessingInter_FuelEcoTargetValue(mTargetFuelEconomyValue * 0.1);
            // 5.5.4.3.7.3 Save and Load Logic
            if (mAEMInitializeComplete == true) {
                if (almostEqual(mTargetFuelEconomyValue, mPrevTargetFuelEconomyValue) == false) {
                    // Inter -> Memory
                    updateValue("SFC.Extension.Memory.Inter_TargetFuelEcoValue", mTargetFuelEconomyValue * 0.1);
                    DDebug() << "SaveFuelValue(mInter_TargetFuelEcoValue): "
                             << static_cast<HDouble>(mTargetFuelEconomyValue * 0.1);
                }
            }
            mPrevTargetFuelEconomyValue = mTargetFuelEconomyValue;
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class WheelSpeedNoiseStatus {
        NONE = 0x0,
        NOISE = 0x1,
        NORMAL = 0x2
    };

    enum class FuelErrorDetectionStatus {
        NORMAL = 0x0,
        HOLD = 0x1,
        ERROR = 0x2
    };

    void processPrivateCurrentFuelSection() {
        // Note(andre): mArrPar_FuelSender[0], mArrPar_FuelSenderVolume[0] <- Not Used. padding space.
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

        if (mPrivateCurrentFuelSection == 0) {
            setSFCInput_Output_ProcessingInter_FuelInputLevelValue(mArrPar_FuelSenderVolume[1]);
        } else if (mPrivateCurrentFuelSection == 11) {
            setSFCInput_Output_ProcessingInter_FuelInputLevelValue(mArrPar_FuelSenderVolume[11]);
        } else {
            mPrivate_CurrentFuelSectionInterval =
                mArrPar_FuelSender[mPrivateCurrentFuelSection + 1] - mArrPar_FuelSender[mPrivateCurrentFuelSection];
            HDouble ret = (mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection + 1] -
                           mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection]) *
                              (mFuelSenderValue - mArrPar_FuelSender[mPrivateCurrentFuelSection]) /
                              mPrivate_CurrentFuelSectionInterval +
                          mArrPar_FuelSenderVolume[mPrivateCurrentFuelSection];
            setSFCInput_Output_ProcessingInter_FuelInputLevelValue(ret);
        }
    }

    void processPrivate_FuelErrorDetectionStatus() {
        if (mIgnStatus == true) {
            if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::NORMAL &&
                ((mFuelSenderValue <= mPar_FuelSenderShort) || (mFuelSenderValue >= mPar_FuelSenderOpen))) {
                mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::HOLD;
            }
        } else {
            mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
        }

        if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::NORMAL) {
            mOneShotTimerFuelErrorDetectionStatus.stop();
        } else if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::HOLD) {
            if (!((mFuelSenderValue <= mPar_FuelSenderShort) || (mFuelSenderValue >= mPar_FuelSenderOpen))) {
                mOneShotTimerFuelErrorDetectionStatus.stop();
            } else if (((mFuelSenderValue <= mPar_FuelSenderShort) || (mFuelSenderValue >= mPar_FuelSenderOpen)) &&
                       mOneShotTimerFuelErrorDetectionStatus.isRunning() == false) {
                mOneShotTimerFuelErrorDetectionStatus.setInterval(kTimerFuelErrorDetectionInterval120s);
                if (mOneShotTimerFuelErrorDetectionStatus.start() == false) {
                    DWarning() << "[Input_Output_Processing] mOneShotTimerFuelErrorDetectionStatus Start Failed";
                }
            } else {
                // maintain the status
            }
            if ((mFuelSenderValue >= mArrPar_FuelSender[1]) && (mFuelSenderValue <= mArrPar_FuelSender[11])) {
                mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
                processInter_FuelErrorDetectionStatus();
            }
        } else if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::ERROR) {
            if (!((mFuelSenderValue >= mArrPar_FuelSender[1]) && (mFuelSenderValue <= mArrPar_FuelSender[11]))) {
                mOneShotTimerFuelErrorDetectionStatus.stop();
            } else if (((mFuelSenderValue >= mArrPar_FuelSender[1]) && (mFuelSenderValue <= mArrPar_FuelSender[11])) &&
                       mOneShotTimerFuelErrorDetectionStatus.isRunning() == false) {
                mOneShotTimerFuelErrorDetectionStatus.setInterval(kTimerFuelErrorDetectionInterval20s);
                if (mOneShotTimerFuelErrorDetectionStatus.start() == false) {
                    DWarning() << "[Input_Output_Processing] mOneShotTimerFuelErrorDetectionStatus Start Failed";
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
    }

    void onOneShotTimerFuelErrorDetectionStatus() {
        if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::HOLD) {
            mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::ERROR;
        } else if (mPrevInter_FuelErrorDetectionStatus == FuelErrorDetectionStatus::ERROR) {
            mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
        } else {
            // HOLD or NORMAL. No timer signal
        }
        DDebug() << "Fuel Timer End";
        processInter_FuelErrorDetectionStatus();
        flush();
    }

    void processInter_FuelErrorDetectionStatus() {
        switch (mPrevInter_FuelErrorDetectionStatus) {
            case FuelErrorDetectionStatus::NORMAL:
                setSFCInput_Output_ProcessingInter_FuelErrorDetectionStatus(
                    SFCInput_Output_ProcessingInter_FuelErrorDetectionStatus::NORMAL);
                setSFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus(
                    SFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus::NORMAL);
                break;
            case FuelErrorDetectionStatus::HOLD:
                setSFCInput_Output_ProcessingInter_FuelErrorDetectionStatus(
                    SFCInput_Output_ProcessingInter_FuelErrorDetectionStatus::HOLD);
                // Holding Engineering status NORMAL
                break;
            case FuelErrorDetectionStatus::ERROR:
                setSFCInput_Output_ProcessingInter_FuelErrorDetectionStatus(
                    SFCInput_Output_ProcessingInter_FuelErrorDetectionStatus::ERROR);
                if (mFuelSenderValue >= mPar_FuelSenderOpen) {
                    setSFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus(
                        SFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus::OPEN);
                } else if (mFuelSenderValue <= mPar_FuelSenderShort) {
                    setSFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus(
                        SFCInput_Output_ProcessingEngineeringInter_FuelErrorDetectionStatus::SHORT);
                } else {
                    // ERROR
                }
                break;
            default:
                break;
        }
        DDebug() << "mPrevInter_FuelErrorDetectionStatus :" << static_cast<HUInt64>(mPrevInter_FuelErrorDetectionStatus);
    }

    // 4.3.1.1 Rheostat Switch Input determination
    void updateRheostatSwitchStatus() {
        SFCInput_Output_ProcessingInter_RheostatSwitchStatus stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::DEFAULT;

        if ((mRheostatSwUpStatus == ArgumentsRheostatSwitchChanged::Input_RheostatSwUpStatus::ON &&
             mRheostatSwDownStatus == ArgumentsRheostatSwitchChanged::Input_RheostatSwDownStatus::OFF) ||
            mRheostatSwitchReqStatus == RheostatSwitchReqStatus::UP) {
            stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::UP;
        } else if ((mRheostatSwUpStatus == ArgumentsRheostatSwitchChanged::Input_RheostatSwUpStatus::OFF &&
                    mRheostatSwDownStatus == ArgumentsRheostatSwitchChanged::Input_RheostatSwDownStatus::ON) ||
                   mRheostatSwitchReqStatus == RheostatSwitchReqStatus::DOWN) {
            stat = SFCInput_Output_ProcessingInter_RheostatSwitchStatus::DOWN;
        } else {
            // Other condition (DEFAULT)
        }
        setSFCInput_Output_ProcessingInter_RheostatSwitchStatus(stat);
    }

    // 차속 계산 - 유효 값 판단
    void processWheelSpeedValue() {
        SpeedValidity speedValidity = SpeedValidity::VALID;

        if (mIgnStatus == true) {
            if (mWheelSpeedFrontLeftValueSource < kThresholdWheelSpeedValid) {
                mPrivate_WheelSpeedFrontLeftValue = mWheelSpeedFrontLeftValueSource;
            } else {
                mPrivate_WheelSpeedFrontLeftValue = 0;
            }

            if (mWheelSpeedFrontRightValueSource < kThresholdWheelSpeedValid) {
                mPrivate_WheelSpeedFrontRightValue = mWheelSpeedFrontRightValueSource;
            } else {
                mPrivate_WheelSpeedFrontRightValue = 0;
            }

            if (mWheelSpeedRearLeftValueSource < kThresholdWheelSpeedValid) {
                mPrivate_WheelSpeedRearLeftValue = mWheelSpeedRearLeftValueSource;
            } else {
                mPrivate_WheelSpeedRearLeftValue = 0;
            }

            if (mWheelSpeedRearRightValueSource < kThresholdWheelSpeedValid) {
                mPrivate_WheelSpeedRearRightValue = mWheelSpeedRearRightValueSource;
            } else {
                mPrivate_WheelSpeedRearRightValue = 0;
            }

            if ((mWheelSpeedFrontLeftValueSource > kThresholdWheelSpeedValid - 1) &&
                (mWheelSpeedFrontRightValueSource > kThresholdWheelSpeedValid - 1) &&
                (mWheelSpeedRearLeftValueSource > kThresholdWheelSpeedValid - 1) &&
                (mWheelSpeedRearRightValueSource > kThresholdWheelSpeedValid - 1)) {
                speedValidity = SpeedValidity::INVALID;
            }
        } else {
            mPrivate_WheelSpeedFrontLeftValue = 0;
            mPrivate_WheelSpeedFrontRightValue = 0;
            mPrivate_WheelSpeedRearLeftValue = 0;
            mPrivate_WheelSpeedRearRightValue = 0;
        }

        setSFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity(speedValidity);
    }

    // 차속 계산 - 유효한 휠 속도 개수 산정
    void processValidWheelCount() {
        if (mIgnStatus == true) {
            mPrivate_WheelSpeedFrontLeftValid = (mPrivate_WheelSpeedFrontLeftValue > 0) ? 1 : 0;
            mPrivate_WheelSpeedFrontRightValid = (mPrivate_WheelSpeedFrontRightValue > 0) ? 1 : 0;
            mPrivate_WheelSpeedRearLeftValid = (mPrivate_WheelSpeedRearLeftValue > 0) ? 1 : 0;
            mPrivate_WheelSpeedRearRightValid = (mPrivate_WheelSpeedRearRightValue > 0) ? 1 : 0;
        } else {
            mPrivate_WheelSpeedFrontLeftValid = 0;
            mPrivate_WheelSpeedFrontRightValid = 0;
            mPrivate_WheelSpeedRearLeftValid = 0;
            mPrivate_WheelSpeedRearRightValid = 0;
        }

        switch (mEvServiceModeStatus) {
            case EvServiceModeStatus::NORMAL:
                mPrivate_WheelSpeedValidCount = mPrivate_WheelSpeedFrontLeftValid + mPrivate_WheelSpeedFrontRightValid +
                                                mPrivate_WheelSpeedRearLeftValid + mPrivate_WheelSpeedRearRightValid;
                break;
            case EvServiceModeStatus::TEST_MODE_F:
                mPrivate_WheelSpeedValidCount = mPrivate_WheelSpeedFrontLeftValid + mPrivate_WheelSpeedFrontRightValid;
                break;
            case EvServiceModeStatus::TEST_MODE_R:
                mPrivate_WheelSpeedValidCount = mPrivate_WheelSpeedRearLeftValid + mPrivate_WheelSpeedRearRightValid;
                break;
            default:
                break;
        }
    }

    // 차속 계산 - 노이즈 판단 처리
    void processWheelSpeedNoiseStatus() {
        HUInt8 count = (mWheelSpeedFrontLeftValueSource == 0 ? 1 : 0) + (mWheelSpeedFrontRightValueSource == 0 ? 1 : 0) +
                       (mWheelSpeedRearLeftValueSource == 0 ? 1 : 0) + (mWheelSpeedRearRightValueSource == 0 ? 1 : 0);

        if (mIgnStatus == true && count >= kThresholdWheelSpeedNoise) {
            mWheelSpeedNoiseStatus = WheelSpeedNoiseStatus::NOISE;
        } else {
            mWheelSpeedNoiseStatus = WheelSpeedNoiseStatus::NORMAL;
        }
    }

    // 차속 계산 - 최종 속도 계산
    void processActualVehicleSpeed() {
        HDouble ret_val = 0;

        if (mIgnStatus == true && mWheelSpeedNoiseStatus == WheelSpeedNoiseStatus::NORMAL) {
            // Note(andre) : EvServiceModeStatus is always Normal on ICV Type (Jira: EXNCP-253)
            if (mEvServiceModeStatus == EvServiceModeStatus::NORMAL) {
                if (1 <= mPrivate_WheelSpeedValidCount && mPrivate_WheelSpeedValidCount <= 4) {
                    ret_val = (static_cast<HDouble>(mPrivate_WheelSpeedFrontLeftValue) +
                               static_cast<HDouble>(mPrivate_WheelSpeedFrontRightValue) +
                               static_cast<HDouble>(mPrivate_WheelSpeedRearLeftValue) +
                               static_cast<HDouble>(mPrivate_WheelSpeedRearRightValue)) /
                              mDivFactorByValidCount.at(mPrivate_WheelSpeedValidCount);
                }
            } else if (mEvServiceModeStatus == EvServiceModeStatus::TEST_MODE_F) {
                if (1 <= mPrivate_WheelSpeedValidCount && mPrivate_WheelSpeedValidCount <= 2) {
                    ret_val = (static_cast<HDouble>(mPrivate_WheelSpeedFrontLeftValue) +
                               static_cast<HDouble>(mPrivate_WheelSpeedFrontRightValue)) /
                              mDivFactorByValidCount.at(mPrivate_WheelSpeedValidCount);
                }
            } else if (mEvServiceModeStatus == EvServiceModeStatus::TEST_MODE_R) {
                if (1 <= mPrivate_WheelSpeedValidCount && mPrivate_WheelSpeedValidCount <= 2) {
                    ret_val = (static_cast<HDouble>(mPrivate_WheelSpeedRearLeftValue) +
                               static_cast<HDouble>(mPrivate_WheelSpeedRearRightValue)) /
                              mDivFactorByValidCount.at(mPrivate_WheelSpeedValidCount);
                }
            } else {
                // No process any.
            }
        }

        mActualVehicleSpeed = ret_val;
        setSFCInput_Output_ProcessingInter_ActualVehicleSpeed(mActualVehicleSpeed);
        // Inter_ActualVehicleSpeed >= 10.0
        if (surelyGreaterThan(mActualVehicleSpeed, kActualVehicleSpeedValue10) == true ||
            almostEqual(mActualVehicleSpeed, kActualVehicleSpeedValue10) == true) {
            setSFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10(
                SFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10::OVER);
        } else {
            setSFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10(
                SFCInput_Output_ProcessingInter_ActualVehicleSpeedCompareValueTo10::UNDER);
        }

        // For Fuel_System Speed Range Stat
        if (mActualVehicleSpeed < 20) {
            setSFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat(
                SFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat::SPEED_0_20);
        } else if (mActualVehicleSpeed >= 20 && mActualVehicleSpeed < 80) {
            setSFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat(
                SFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat::SPEED_20_80);
        } else if (mActualVehicleSpeed >= 80 && mActualVehicleSpeed < 120) {
            setSFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat(
                SFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat::SPEED_80_120);
        } else if (mActualVehicleSpeed >= 120 && mActualVehicleSpeed < 150) {
            setSFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat(
                SFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat::SPEED_120_150);
        } else {
            setSFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat(
                SFCPrivateInput_Output_ProcessingActualVehicleSpeedRangeStat::SPEED_OVER_150);
        }
    }

    // 주행, 정차, 시동 판단
    void processDrivingOnStatus() {
        DrivingOnStatus retVal = mPrevDrivingOnStatus;

        if (mIgnStatus == true) {
            if (mPrevDrivingOnStatus == DrivingOnStatus::OFF) {
                if (mActualVehicleSpeed >= 1.5) {
                    retVal = DrivingOnStatus::ON;
                }
            } else if (mPrevDrivingOnStatus == DrivingOnStatus::ON) {
                if (mActualVehicleSpeed <= 1.0) {
                    retVal = DrivingOnStatus::OFF;
                }
            } else {
                // OFF
            }
        } else {
            retVal = DrivingOnStatus::OFF;
        }

        if (retVal != mPrevDrivingOnStatus) {
            mPrevDrivingOnStatus = retVal;
            setSFCInput_Output_ProcessingInter_DrivingOnStatus(mPrevDrivingOnStatus);
        }
    }

    // 시동 판단
    void processEngineOnStatus() {
        if (mIgnStatus == true &&
            (mVehicleReadyStatus == VehicleReadyStatus::READY || mSscControlStatus == SscControlStatus::ON)) {
            mEngineOnStatus = EngineOnStatus::ON;
        } else {
            mEngineOnStatus = EngineOnStatus::OFF;
        }
        setSFCInput_Output_ProcessingInter_EngineOnStatus(mEngineOnStatus);
    }

    // ISG 활성화 판단
    void processIsgActiveStatus() {
        if (mIgnStatus == true &&
            ((mEngineISGStatus1 == EngineISGStatus1::AUTO_STOP || mEngineISGStatus1 == EngineISGStatus1::AUTO_RESTART ||
              mEngineISGStatus1 == EngineISGStatus1::NOT_RESTARTABLE) ||
             (mEngineISGStatus2 == EngineISGStatus2::RAPID_ACCEL))) {
            mISGActiveStatus = ISGActiveStatus::ON;
        } else {
            mISGActiveStatus = ISGActiveStatus::OFF;
        }
        setSFCInput_Output_ProcessingInter_ISGActiveStatus(mISGActiveStatus);
    }

    enum class RheostatSwitchStatus {
        DEFAULT = 0x0,
        UP = 0x1,
        DOWN = 0x2
    };

    HBool mIgnStatus = false;
    HBool mAEMInitializeComplete = false;

    using IOProc = ccos::Input_Output_Processing;

    using EngineISGStatus1 = IOProc::ArgumentsIsgActiveStatusDataChanged::Input_EngineISGStatus1;
    using EngineISGStatus2 = IOProc::ArgumentsIsgActiveStatusDataChanged::Input_EngineISGStatus2;
    using ISGActiveStatus = IOProc::SFCInput_Output_ProcessingInter_ISGActiveStatus;
    EngineISGStatus1 mEngineISGStatus1 = EngineISGStatus1::OFF;
    EngineISGStatus2 mEngineISGStatus2 = EngineISGStatus2::OFF;
    ISGActiveStatus mISGActiveStatus = ISGActiveStatus::NONE;

    using VehicleReadyStatus = IOProc::ArgumentsEngineOnStatusDataChanged::Input_VehicleReadyStatus;
    using SscControlStatus = IOProc::ArgumentsEngineOnStatusDataChanged::Input_SscControlStatus;
    using EngineOnStatus = IOProc::SFCInput_Output_ProcessingInter_EngineOnStatus;
    VehicleReadyStatus mVehicleReadyStatus = VehicleReadyStatus::OFF;
    SscControlStatus mSscControlStatus = SscControlStatus::OFF;
    EngineOnStatus mEngineOnStatus = EngineOnStatus::NONE;

    using SpeedValidity = IOProc::SFCInput_Output_ProcessingInter_ActualVehicleSpeedValidity;
    HUInt64 mWheelSpeedFrontLeftValueSource = 0;
    HUInt64 mWheelSpeedFrontRightValueSource = 0;
    HUInt64 mWheelSpeedRearLeftValueSource = 0;
    HUInt64 mWheelSpeedRearRightValueSource = 0;

    HUInt64 mPrivate_WheelSpeedFrontLeftValue = 0;
    HUInt64 mPrivate_WheelSpeedFrontRightValue = 0;
    HUInt64 mPrivate_WheelSpeedRearLeftValue = 0;
    HUInt64 mPrivate_WheelSpeedRearRightValue = 0;

    HUInt64 mPrivate_WheelSpeedFrontLeftValid = 0;
    HUInt64 mPrivate_WheelSpeedFrontRightValid = 0;
    HUInt64 mPrivate_WheelSpeedRearLeftValid = 0;
    HUInt64 mPrivate_WheelSpeedRearRightValid = 0;

    HUInt64 mPrivate_WheelSpeedValidCount = 0;

    WheelSpeedNoiseStatus mWheelSpeedNoiseStatus = WheelSpeedNoiseStatus::NONE;

    using EvServiceModeStatus = IOProc::ArgumentsEvServiceModeChanged::Input_EvServiceModeStatus;
    EvServiceModeStatus mEvServiceModeStatus = EvServiceModeStatus::NORMAL;

    HDouble mActualVehicleSpeed = 0.0;

    using DrivingOnStatus = IOProc::SFCInput_Output_ProcessingInter_DrivingOnStatus;
    DrivingOnStatus mPrevDrivingOnStatus = DrivingOnStatus::NONE;

    static constexpr HUInt64 kThresholdWheelSpeedValid = 16383;
    static constexpr HUInt64 kThresholdWheelSpeedNoise = 3;
    static constexpr HDouble kActualVehicleSpeedValue10 = 10.0;

    const std::map<HUInt64, HDouble> mDivFactorByValidCount = {
        {1, 1.0 * 32.0}, {2, 2.0 * 32.0}, {3, 3.0 * 32.0}, {4, 4.0 * 32.0}};

    HDouble mFuelSenderValue = 0;
    HUInt64 mPrivateCurrentFuelSection = 11;
    std::array<HDouble, 12> mArrPar_FuelSender = {
        0.0,
    };
    std::array<HDouble, 12> mArrPar_FuelSenderVolume = {
        0.0,
    };
    HDouble mPrivate_CurrentFuelSectionInterval = 0;
    HDouble mPar_FuelSenderShort = 0;
    HDouble mPar_FuelSenderOpen = 0;
    FuelErrorDetectionStatus mPrevInter_FuelErrorDetectionStatus = FuelErrorDetectionStatus::NORMAL;
    ssfs::SFCTimer<Input_Output_Processing> mOneShotTimerFuelErrorDetectionStatus;
    static constexpr uint32_t kTimerFuelErrorDetectionInterval120s = 120000;
    static constexpr uint32_t kTimerFuelErrorDetectionInterval20s = 20000;

    using RheostatSwUpStatus = ArgumentsRheostatSwitchChanged::Input_RheostatSwUpStatus;
    using RheostatSwDownStatus = ArgumentsRheostatSwitchChanged::Input_RheostatSwDownStatus;
    using RheostatSwitchReqStatus = ArgumentsRheostatSwitchChanged::Input_RheostatSwitchReqStatus;
    RheostatSwUpStatus mRheostatSwUpStatus = RheostatSwUpStatus::OFF;
    RheostatSwDownStatus mRheostatSwDownStatus = RheostatSwDownStatus::OFF;
    RheostatSwitchReqStatus mRheostatSwitchReqStatus = RheostatSwitchReqStatus::DEFAULT;

    HDouble mTargetFuelEconomyValue = 0.0;
    HDouble mPrevTargetFuelEconomyValue = 0.0;
};

}  // namespace ccos

#endif  // SFSS_Input_Output_Processing_H
