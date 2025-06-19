/**
 * @file ECAS.hpp
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
#ifndef SFSS_ECAS_H
#define SFSS_ECAS_H

#define DLOG_ENABLED gEnableSFCLog

#include "ECASBase.hpp"

namespace ccos {

// SFC Version : 18.0.2
// Spec Version : v0.30
// Reference : [CV150] ECAS. Contains : Telltale, Constant. Recommend: VALUE_CHANGED
class ECAS : public ECASBase {
public:
    ECAS() = default;
    ~ECAS() override = default;
    ECAS(const ECAS& other) = delete;
    ECAS(ECAS&& other) noexcept = delete;
    ECAS& operator=(const ECAS& other) = delete;
    ECAS& operator=(ECAS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCECASTelltaleECASBlinkValueB(SFCECASTelltaleECASBlinkValueB::OFF);
        setSFCECASTelltaleBUSBlinkValueB(SFCECASTelltaleBUSBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateTelltaleECAS();
        updateTelltaleAxleMotion();
        updateTelltaleBusEcasStat();
        updateConstantAxleWeightPageStatus();
        updateConstantAxleWeightValues();
        updateConstantVehicleAxleStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateTelltaleECAS();
        updateTelltaleAxleMotion();
        updateTelltaleBusEcasStat();
        updateConstantAxleWeightPageStatus();
        updateConstantAxleWeightValues();
        updateConstantVehicleAxleStatus();
    }

    void onConfigVehicleTypeCVChanged(const ArgumentsConfigVehicleTypeCVChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;
        updateTelltaleECAS();
        updateTelltaleBusEcasStat();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateConstantAxleWeightValues();
        updateConstantVehicleAxleStatus();
    }

    void onConfigAxleWeightChanged(const ArgumentsConfigAxleWeightChanged& args) {
        mConfigAxleWeight = args.mInter_ConfigAxleWeight;
        updateConstantAxleWeightPageStatus();
    }

    void onConfigAxleInfoChanged(const ArgumentsConfigAxleInfoChanged& args) {
        mConfigAxleInfo = args.mInter_ConfigAxleInfo;
        updatePrivateAxleValues();
        updateConstantVehicleAxleStatus();
        updateConstantAxleWeightValues();
    }

    void onConfigSuspTypeChanged(const ArgumentsConfigSuspTypeChanged& args) {
        mConfigSuspType = args.mInter_ConfigSuspType;
        updatePrivateAxleValues();
        updateTelltaleECAS();
        updateTelltaleAxleMotion();
        updateConstantAxleWeightValues();
    }

    void onWeightUnitStatChanged(const ArgumentsWeightUnitStatChanged& args) {
        mWeightUnitStat = args.mInter_WeightUnit_stat;
        updatePrivateAxleWeightFactor();
        updateConstantAxleWeightPageStatus();
        updateConstantAxleWeightValues();
    }

    void onVehicleSpeedValueChanged(const ArgumentsVehicleSpeedValueChanged& args) {
        mWheelBasedVehicleSpeedValue = args.mInput_WheelBasedVehicleSpeedValue;
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateConstantAxleWeightValues();
    }

    void onAxleWeightChanged(const ArgumentsAxleWeightChanged& args) {
        if (ISTIMEOUT(args.mInput_AxleWeight) == true) {
            mIsAxleWeightTimeout = true;
        } else {
            mIsAxleWeightTimeout = false;
        }
        mAxleLocation = args.mInput_AxleLocation;
        mAxleInputWeightValue = args.mInput_AxleWeight;
        // WeightValue 는 AxleLocation과 함께 순차적(콜백에서는 사실상 Pair)으로 들어옴.
        // 어떤 Axle에서의 WeightValue인지를 결정하고 값을 가지고 있어야 함.
        // 중량 factor, ConfigAxleInfo, Private_AxleWeightValidStatus 의 변경에 따른
        // IMG_AxleWeightAxle1~5_value 갱신을 용이하게 하기 위함임.
        updatePrivateWeightValueFromEachLocation(args.mInput_AxleLocation, args.mInput_AxleWeight);
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateConstantAxleWeightValues();
    }

    void onTelltaleStatus1Changed(const ArgumentsTelltaleStatus1Changed& args) {
        if (ISTIMEOUT(args.mInput_ECASTelltaleStatus1) == true) {
            mTelltaleStatus1 = TelltaleStatus1::MESSAGE_TIMEOUT;
        } else {
            mTelltaleStatus1 = args.mInput_ECASTelltaleStatus1;
        }
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateTelltaleECAS();
        updateConstantAxleWeightValues();
    }

    void onTelltaleStatus2Changed(const ArgumentsTelltaleStatus2Changed& args) {
        mTelltaleStatus2 = args.mInput_ECASTelltaleStatus2;
        updateTelltaleAxleMotion();
    }

    void onBusEcasStatChanged(const ArgumentsBusEcasStatChanged& args) {
        mECASTelltaleStatus3 = args.mInput_ECASTelltaleStatus3;
        mKneelingStatus = args.mInput_KneelingStatus;
        updateTelltaleBusEcasStat();
    }

    void onAlasTelltaleStatus1Changed(const ArgumentsAlasTelltaleStatus1Changed& args) {
        mALASTelltaleStatus1 = args.mInput_ALASTelltaleStatus1;
        updatePrivateAxleWeightValidStatus();
        updatePrivateAxleValues();
        updateTelltaleECAS();
        updateConstantAxleWeightValues();
    }

    void onAlasTelltaleStatus2Changed(const ArgumentsAlasTelltaleStatus2Changed& args) {
        mALASTelltaleStatus2 = args.mInput_ALASTelltaleStatus2;
        updateTelltaleAxleMotion();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    using AxleLocation = ArgumentsAxleWeightChanged::Input_AxleLocation;
    inline void updatePrivateWeightValueFromEachLocation(const AxleLocation& loc, const HUInt64& value) {
        if (loc == AxleLocation::FRONT1) {
            mPrivateAxleWeightFront1Value = value;
#ifdef ENABLE_GCOV_ON
            setSFCPrivateECASAxleFront1WeightValue(mPrivateAxleWeightFront1Value);
#endif
        } else if (loc == AxleLocation::REAR1) {
            mPrivateAxleWeightRear1Value = value;
#ifdef ENABLE_GCOV_ON
            setSFCPrivateECASAxleRear1WeightValue(mPrivateAxleWeightRear1Value);
#endif
        } else if (loc == AxleLocation::REAR2) {
            mPrivateAxleWeightRear2Value = value;
#ifdef ENABLE_GCOV_ON
            setSFCPrivateECASAxleRear2WeightValue(mPrivateAxleWeightRear2Value);
#endif
        } else if (loc == AxleLocation::REAR3) {
            mPrivateAxleWeightRear3Value = value;
#ifdef ENABLE_GCOV_ON
            setSFCPrivateECASAxleRear3WeightValue(mPrivateAxleWeightRear3Value);
#endif
        } else {
            // no operation
        }
    }

    void updatePrivateAxleWeightFactor() {
        if (mWeightUnitStat == WeightUnitStat::POUND) {
            mPrivateAxleWeightFactor = kAxleWeightx1000LbsFactor;
        } else {
            mPrivateAxleWeightFactor = kAxleWeightTonFactor;
        }

#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxleWeight_Factor(mPrivateAxleWeightFactor);
#endif
    }

    void updateTelltaleECAS() {
        SFCECASTelltaleECASStat stat = SFCECASTelltaleECASStat::OFF;
        SFCECASTelltaleECASStatOptional statOpt = SFCECASTelltaleECASStatOptional::OFF;
        SFCECASTelltaleECASBlinkValueA blinkA = SFCECASTelltaleECASBlinkValueA::OFF;
        SFCECASTelltaleECASBlinkValueAOptional blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::NONE;

        if (mIsIgnOn == true) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK &&
                (mConfigSuspType == ConfigSuspType::AIR_ALL || mConfigSuspType == ConfigSuspType::AIR_REAR)) {
                switch (mTelltaleStatus1) {
                    case TelltaleStatus1::FR_UP_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::FR_UP_ON;
                        break;
                    case TelltaleStatus1::FR_LOW_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::FR_LOW_ON;
                        break;
                    case TelltaleStatus1::RR_UP_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::RR_UP_ON;
                        break;
                    case TelltaleStatus1::RR_LOW_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::RR_LOW_ON;
                        break;
                    case TelltaleStatus1::ALL_UP_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::ALL_UP_ON;
                        break;
                    case TelltaleStatus1::ALL_LOW_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::ALL_LOW_ON;
                        break;
                    case TelltaleStatus1::FR_UP_RR_LOW_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::FR_UP_RR_LOW_ON;
                        break;
                    case TelltaleStatus1::FR_LOW_RR_UP_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::FR_LOW_RR_UP_ON;
                        break;
                    case TelltaleStatus1::TO_NORMAL_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::TO_NORMAL_ON;
                        break;
                    case TelltaleStatus1::FR_UP_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::FR_UP_ON;
                        break;
                    case TelltaleStatus1::FR_LOW_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::FR_LOW_ON;
                        break;
                    case TelltaleStatus1::RR_UP_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::RR_UP_ON;
                        break;
                    case TelltaleStatus1::RR_LOW_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::RR_LOW_ON;
                        break;
                    case TelltaleStatus1::ALL_UP_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::ALL_UP_ON;
                        break;
                    case TelltaleStatus1::ALL_LOW_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::ALL_LOW_ON;
                        break;
                    case TelltaleStatus1::TO_NORMAL_BLINK:
                        stat = SFCECASTelltaleECASStat::BLINK4;
                        blinkA = SFCECASTelltaleECASBlinkValueA::OPTIONAL;
                        blinkAOpt = SFCECASTelltaleECASBlinkValueAOptional::TO_NORMAL_ON;
                        break;
                    case TelltaleStatus1::ABOVE_SPEED_LIMIT:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::ABOVE_SPEED_LIMIT;
                        break;
                    case TelltaleStatus1::AXLE_LOAD_LIMIT:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::AXLE_LOAD_LIMIT;
                        break;
                    case TelltaleStatus1::EXCEED_TAG_AXLE_LOAD:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::EXCEED_TAG_AXLE_LOAD;
                        break;
                    case TelltaleStatus1::CHECK_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::CHECK_ON;
                        break;
                    default:
                        // default: OFF
                        break;
                }
            } else if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::TRUCK && mConfigSuspType == ConfigSuspType::SPRING_ALAS) {
                switch (mALASTelltaleStatus1) {
                    case ALASTelltaleStatus1::ABOVE_SPEED_LIMIT:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::ABOVE_SPEED_LIMIT;
                        break;
                    case ALASTelltaleStatus1::AXLE_LOAD_LIMIT:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::AXLE_LOAD_LIMIT;
                        break;
                    case ALASTelltaleStatus1::EXCEED_TAG_AXLE_LOAD:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::EXCEED_TAG_AXLE_LOAD;
                        break;
                    case ALASTelltaleStatus1::CHECK_ON:
                        stat = SFCECASTelltaleECASStat::OPTIONAL;
                        statOpt = SFCECASTelltaleECASStatOptional::CHECK_ON;
                        break;
                    default:
                        // default: OFF
                        break;
                }
            } else {
                // no operation
            }
        }

        setSFCECASTelltaleECASStat(stat);
        setSFCECASTelltaleECASStatOptional(statOpt);
        setSFCECASTelltaleECASBlinkValueA(blinkA);
        setSFCECASTelltaleECASBlinkValueAOptional(blinkAOpt);
        // setSFCECASTelltaleECASBlinkValueB 는 고정 OFF이므로 onInitialize 에 위치.
    }

    void updateTelltaleAxleMotion() {
        SFCECASTelltaleAxleMotionStat stat = SFCECASTelltaleAxleMotionStat::OFF;
        SFCECASTelltaleAxleMotionStatOptional statOpt = SFCECASTelltaleAxleMotionStatOptional::OFF;
        if (mIsIgnOn == true && (mConfigSuspType == ConfigSuspType::AIR_ALL || mConfigSuspType == ConfigSuspType::AIR_REAR)) {
            if (mTelltaleStatus2 == TelltaleStatus2::AXLE_UP_ON) {
                stat = SFCECASTelltaleAxleMotionStat::OPTIONAL;
                statOpt = SFCECASTelltaleAxleMotionStatOptional::AXLE_UP_ON;
            } else if (mTelltaleStatus2 == TelltaleStatus2::LOAD_TRANSFER_ON) {
                stat = SFCECASTelltaleAxleMotionStat::OPTIONAL;
                statOpt = SFCECASTelltaleAxleMotionStatOptional::LOAD_TRANSFER_ON;
            } else {
                // no operation
            }
        } else if (mIsIgnOn == true && mConfigSuspType == ConfigSuspType::SPRING_ALAS) {
            if (mALASTelltaleStatus2 == ALASTelltaleStatus2::AXLE_UP_ON) {
                stat = SFCECASTelltaleAxleMotionStat::OPTIONAL;
                statOpt = SFCECASTelltaleAxleMotionStatOptional::AXLE_UP_ON;
            } else if (mALASTelltaleStatus2 == ALASTelltaleStatus2::LOAD_TRANSFER_ON) {
                stat = SFCECASTelltaleAxleMotionStat::OPTIONAL;
                statOpt = SFCECASTelltaleAxleMotionStatOptional::LOAD_TRANSFER_ON;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCECASTelltaleAxleMotionStat(stat);
        setSFCECASTelltaleAxleMotionStatOptional(statOpt);
    }

    void updateConstantAxleWeightPageStatus() {
        SFCECASConstantAxleWeightPageStat stat = SFCECASConstantAxleWeightPageStat::OFF;
        if (mIsIgnOn == true && mConfigAxleWeight != ConfigAxleWeight::OFF) {
            if (mWeightUnitStat == WeightUnitStat::POUND) {
                stat = SFCECASConstantAxleWeightPageStat::LBS_ON;
            } else if (mWeightUnitStat == WeightUnitStat::KG) {
                stat = SFCECASConstantAxleWeightPageStat::TON_ON;
            } else {
                // no operation
            }
        }
        setSFCECASConstantAxleWeightPageStat(stat);
    }

    void updatePrivateAxleValues() {
        updatePrivateAxle1Value();
        updatePrivateAxle2Value();
        updatePrivateAxle3Value();
        updatePrivateAxle4Value();
        updatePrivateAxle5Value();
    }

    void updateConstantAxleWeightValues() {
        updateConstantAxleWeight1Value();
        updateConstantAxleWeight2Value();
        updateConstantAxleWeight3Value();
        updateConstantAxleWeight4Value();
        updateConstantAxleWeight5Value();
    }

    // 5.2.5 축 하중값 표시 를 위한 별도 Private 계산용
    // EXNCP-35523 에 따라 ConfigAxleInfo 를 우선 비교, 이후 ValidStatus 비교
    // ConfigAxleInfo 를 보고 어떤 AxleWeightValue를 출력할지, NO_Display(0xFF, 255) 를 출력할지 정하며
    // ValidStatus 를 보고 BAR(0) 출력을 정함.
    // PrivateAxleWeightValidStatus에 IGN On/Off Status를 포함하므로 별도 Ign 비교 로직 추가 안함.
    void updatePrivateAxle1Value() {
        mPrivateAxle1WeightValue = kAxlePrivateNoDisplay;
        if (mConfigSuspType == ConfigSuspType::AIR_ALL &&
            (mConfigAxleInfo == ConfigAxleInfo::T4X2 || mConfigAxleInfo == ConfigAxleInfo::T6X2 ||
             mConfigAxleInfo == ConfigAxleInfo::T6X2_TAG || mConfigAxleInfo == ConfigAxleInfo::T6X4)) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle1WeightValue = mPrivateAxleWeightFront1Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle1WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxle1WeightValue(mPrivateAxle1WeightValue);
#endif
    }

    void updatePrivateAxle2Value() {
        mPrivateAxle2WeightValue = kAxlePrivateNoDisplay;
        if (mConfigAxleInfo == ConfigAxleInfo::T4X2 || mConfigAxleInfo == ConfigAxleInfo::T6X2 ||
            mConfigAxleInfo == ConfigAxleInfo::T6X2_TAG || mConfigAxleInfo == ConfigAxleInfo::T6X4 ||
            mConfigAxleInfo == ConfigAxleInfo::T8X4_PUSHER) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle2WeightValue = mPrivateAxleWeightRear1Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle2WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxle2WeightValue(mPrivateAxle2WeightValue);
#endif
    }

    void updatePrivateAxle3Value() {
        mPrivateAxle3WeightValue = kAxlePrivateNoDisplay;
        if (mConfigAxleInfo == ConfigAxleInfo::T6X2 || mConfigAxleInfo == ConfigAxleInfo::T6X2_TAG ||
            mConfigAxleInfo == ConfigAxleInfo::T6X4 || mConfigAxleInfo == ConfigAxleInfo::T8X4_PUSHER) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle3WeightValue = mPrivateAxleWeightRear2Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle3WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        } else if (mConfigAxleInfo == ConfigAxleInfo::T8X4 || mConfigAxleInfo == ConfigAxleInfo::T10X4_PUSHER ||
                   mConfigAxleInfo == ConfigAxleInfo::T10X4_TAG) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle3WeightValue = mPrivateAxleWeightRear1Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle3WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxle3WeightValue(mPrivateAxle3WeightValue);
#endif
    }

    void updatePrivateAxle4Value() {
        mPrivateAxle4WeightValue = kAxlePrivateNoDisplay;
        if (mConfigAxleInfo == ConfigAxleInfo::T8X4 || mConfigAxleInfo == ConfigAxleInfo::T10X4_PUSHER ||
            mConfigAxleInfo == ConfigAxleInfo::T10X4_TAG) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle4WeightValue = mPrivateAxleWeightRear2Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle4WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        } else if (mConfigAxleInfo == ConfigAxleInfo::T8X4_PUSHER) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle4WeightValue = mPrivateAxleWeightRear3Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle4WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxle4WeightValue(mPrivateAxle4WeightValue);
#endif
    }

    void updatePrivateAxle5Value() {
        mPrivateAxle5WeightValue = kAxlePrivateNoDisplay;
        // EXNCP-35523 - ConfigAxleInfo 사양 변경
        if (mConfigAxleInfo == ConfigAxleInfo::T10X4_PUSHER || mConfigAxleInfo == ConfigAxleInfo::T10X4_TAG) {
            if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::VALID) {
                mPrivateAxle5WeightValue = mPrivateAxleWeightRear3Value;
            } else if (mPrivateAxleWeightValidStatus == PrivateAxleWeightValidStatus::INVALID) {
                mPrivateAxle5WeightValue = kAxlePrivateBarInvalid;
            } else {
                // no operation
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxle5WeightValue(mPrivateAxle5WeightValue);
#endif
    }

    void updatePrivateAxleWeightValidStatus() {
        if (mIsIgnOn == false ||
            (mConfigArea != ConfigArea::USA && mTelltaleStatus1 != TelltaleStatus1::NORMAL &&
             mALASTelltaleStatus1 != ALASTelltaleStatus1::NORMAL) ||
            (mWheelBasedVehicleSpeedValue > kVehicleSpeed5KPH) || (mIsAxleWeightTimeout == true)) {
            mPrivateAxleWeightValidStatus = PrivateAxleWeightValidStatus::INVALID;
        } else {
            mPrivateAxleWeightValidStatus = PrivateAxleWeightValidStatus::VALID;
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateECASAxleWeightValid_Status(mPrivateAxleWeightValidStatus);
#endif
    }

    // AxleWeight 구하는 함수.
    // Round off to 1 decimal, 소수점 첫째자리까지 반올림 추가.
    // 0xFF(NoDisplay)일때는 그대로리턴.
    inline HDouble calAxleWeight(const HUInt64& axleValue) {
        if (axleValue >= 0x0000 && axleValue <= 0xFB8F) {
            return round(static_cast<HDouble>(axleValue) * mPrivateAxleWeightFactor * 10.0) / 10.0;
        } else if (axleValue >= 0xFB90 && axleValue <= kAxlePrivateBarInvalid) {
            return kAxleConstantBarInvalid;
        } else {
            return kAxleConstantNoDisplay;
        }
    }

    // Private 값 계산한 부분에 Factor, Roundoff 적용하여 출력.
    void updateConstantAxleWeight1Value() {
        setSFCECASConstantAxle1WeightValue(calAxleWeight(mPrivateAxle1WeightValue));
    }

    void updateConstantAxleWeight2Value() {
        setSFCECASConstantAxle2WeightValue(calAxleWeight(mPrivateAxle2WeightValue));
    }

    void updateConstantAxleWeight3Value() {
        setSFCECASConstantAxle3WeightValue(calAxleWeight(mPrivateAxle3WeightValue));
    }

    void updateConstantAxleWeight4Value() {
        setSFCECASConstantAxle4WeightValue(calAxleWeight(mPrivateAxle4WeightValue));
    }

    void updateConstantAxleWeight5Value() {
        setSFCECASConstantAxle5WeightValue(calAxleWeight(mPrivateAxle5WeightValue));
    }

    void updateConstantVehicleAxleStatus() {
        if (mIsIgnOn == true) {
            if (mConfigAxleInfo == ConfigAxleInfo::T6X2_TAG && mConfigArea != ConfigArea::EEC) {
                setSFCECASConstantAxleStat(SFCECASConstantAxleStat::T6X2_TAG_DOUBLE);
                return;
            }
            const auto& it = mAxleStatMap.find(mConfigAxleInfo);
            if (it != mAxleStatMap.end()) {
                setSFCECASConstantAxleStat(it->second);
            }
        }
    }

    void updateTelltaleBusEcasStat() {
        SFCECASTelltaleBUSStat stat = SFCECASTelltaleBUSStat::OFF;
        SFCECASTelltaleBUSStatOptional statOpt = SFCECASTelltaleBUSStatOptional::NONE;
        SFCECASTelltaleBUSBlinkValueA blinkA = SFCECASTelltaleBUSBlinkValueA::OFF;
        SFCECASTelltaleBUSBlinkValueAOptional blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::NONE;

        if (mIsIgnOn == true) {
            if (mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) {
                if (mKneelingStatus == KneelingStatus::OFF) {
                    switch (mECASTelltaleStatus3) {
                        case ECASTelltaleStatus3::UP_ON:
                            stat = SFCECASTelltaleBUSStat::OPTIONAL;
                            statOpt = SFCECASTelltaleBUSStatOptional::UP_ON;
                            break;
                        case ECASTelltaleStatus3::LOW_ON:
                            stat = SFCECASTelltaleBUSStat::OPTIONAL;
                            statOpt = SFCECASTelltaleBUSStatOptional::LOW_ON;
                            break;
                        case ECASTelltaleStatus3::TO_NORMAL_ON:
                            stat = SFCECASTelltaleBUSStat::OPTIONAL;
                            statOpt = SFCECASTelltaleBUSStatOptional::TO_NORMAL_ON;
                            break;
                        case ECASTelltaleStatus3::KNEELING_LOW_ON:
                            stat = SFCECASTelltaleBUSStat::OPTIONAL;
                            statOpt = SFCECASTelltaleBUSStatOptional::KNEELING_LOW_ON;
                            break;
                        case ECASTelltaleStatus3::UP_BLINK:
                            stat = SFCECASTelltaleBUSStat::BLINK2;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::UP_ON;
                            break;
                        case ECASTelltaleStatus3::LOW_BLINK:
                            stat = SFCECASTelltaleBUSStat::BLINK2;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::LOW_ON;
                            break;
                        case ECASTelltaleStatus3::TO_NORMAL_BLINK:
                            stat = SFCECASTelltaleBUSStat::BLINK2;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::TO_NORMAL_ON;
                            break;
                        case ECASTelltaleStatus3::KNEELING_LOW_BLINK:
                            stat = SFCECASTelltaleBUSStat::BLINK2;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::KNEELING_LOW_ON;
                            break;
                        case ECASTelltaleStatus3::KNEELING_UP_BLINK:
                            stat = SFCECASTelltaleBUSStat::BLINK2;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::KNEELING_UP_ON;
                            break;
                        case ECASTelltaleStatus3::KNEELING_ABORTED:
                            stat = SFCECASTelltaleBUSStat::BLINK4;
                            blinkA = SFCECASTelltaleBUSBlinkValueA::OPTIONAL;
                            blinkAOpt = SFCECASTelltaleBUSBlinkValueAOptional::KNEELING_UP_ON;
                            break;
                        case ECASTelltaleStatus3::CHECK_ON:
                            stat = SFCECASTelltaleBUSStat::OPTIONAL;
                            statOpt = SFCECASTelltaleBUSStatOptional::CHECK_ON;
                            break;
                        default:
                            // default: OFF
                            break;
                    }
                } else if (mKneelingStatus == KneelingStatus::ON) {
                    stat = SFCECASTelltaleBUSStat::OPTIONAL;
                    statOpt = SFCECASTelltaleBUSStatOptional::KNEELING_LOW_ON;
                } else {
                    // no operation
                }
            }
        }

        setSFCECASTelltaleBUSStat(stat);
        setSFCECASTelltaleBUSStatOptional(statOpt);
        setSFCECASTelltaleBUSBlinkValueA(blinkA);
        setSFCECASTelltaleBUSBlinkValueAOptional(blinkAOpt);
        // setSFCECASTelltaleBUSBlinkValueB 는 고정 OFF이므로 onInitialize 에 위치.
    }

    using ConfigVehicleTypeCV = ArgumentsConfigVehicleTypeCVChanged::Inter_ConfigVehicleTypeCV;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigAxleWeight = ArgumentsConfigAxleWeightChanged::Inter_ConfigAxleWeight;
    using ConfigAxleInfo = ArgumentsConfigAxleInfoChanged::Inter_ConfigAxleInfo;
    using ConfigSuspType = ArgumentsConfigSuspTypeChanged::Inter_ConfigSuspType;
    using WeightUnitStat = ArgumentsWeightUnitStatChanged::Inter_WeightUnit_stat;
    using TelltaleStatus1 = ArgumentsTelltaleStatus1Changed::Input_ECASTelltaleStatus1;
    using TelltaleStatus2 = ArgumentsTelltaleStatus2Changed::Input_ECASTelltaleStatus2;
    using PrivateAxleWeightValidStatus = SFCPrivateECASAxleWeightValid_Status;
    using AxleStatMap = std::unordered_map<ConfigAxleInfo, SFCECASConstantAxleStat>;
    using ECASTelltaleStatus3 = ArgumentsBusEcasStatChanged::Input_ECASTelltaleStatus3;
    using KneelingStatus = ArgumentsBusEcasStatChanged::Input_KneelingStatus;
    using ALASTelltaleStatus1 = ArgumentsAlasTelltaleStatus1Changed::Input_ALASTelltaleStatus1;
    using ALASTelltaleStatus2 = ArgumentsAlasTelltaleStatus2Changed::Input_ALASTelltaleStatus2;

    AxleStatMap mAxleStatMap = {{ConfigAxleInfo::T4X2, SFCECASConstantAxleStat::T4X2},
                                {ConfigAxleInfo::T6X2, SFCECASConstantAxleStat::T6X2},
                                {ConfigAxleInfo::T6X2_TAG, SFCECASConstantAxleStat::T6X2_TAG},
                                {ConfigAxleInfo::T6X4, SFCECASConstantAxleStat::T6X4},
                                {ConfigAxleInfo::T8X4, SFCECASConstantAxleStat::T8X4},
                                {ConfigAxleInfo::T8X4_PUSHER, SFCECASConstantAxleStat::T8X4_PUSHER},
                                {ConfigAxleInfo::T10X4_PUSHER, SFCECASConstantAxleStat::T10X4_PUSHER},
                                {ConfigAxleInfo::T10X4_TAG, SFCECASConstantAxleStat::T10X4_TAG}};

    HBool mIsIgnOn = false;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigAxleWeight mConfigAxleWeight = ConfigAxleWeight::OFF;
    ConfigAxleInfo mConfigAxleInfo = ConfigAxleInfo::T4X2;
    WeightUnitStat mWeightUnitStat = WeightUnitStat::USM_OFF;
    AxleLocation mAxleLocation = AxleLocation::NOT_AVAILABLE;
    TelltaleStatus1 mTelltaleStatus1 = TelltaleStatus1::OFF;
    TelltaleStatus2 mTelltaleStatus2 = TelltaleStatus2::OFF;
    HUInt64 mWheelBasedVehicleSpeedValue = 0;
    HUInt64 mAxleInputWeightValue = 0;
    HUInt64 mPrivateAxleWeightFront1Value = 0;
    HUInt64 mPrivateAxleWeightRear1Value = 0;
    HUInt64 mPrivateAxleWeightRear2Value = 0;
    HUInt64 mPrivateAxleWeightRear3Value = 0;
    HUInt64 mPrivateAxle1WeightValue = 0;
    HUInt64 mPrivateAxle2WeightValue = 0;
    HUInt64 mPrivateAxle3WeightValue = 0;
    HUInt64 mPrivateAxle4WeightValue = 0;
    HUInt64 mPrivateAxle5WeightValue = 0;
    HBool mIsAxleWeightTimeout = false;
    HDouble mPrivateAxleWeightFactor = kAxleWeightTonFactor;  // DOM이 시스템 기본이므로 DOM 기준으로 Factor 초기값 설정
    PrivateAxleWeightValidStatus mPrivateAxleWeightValidStatus = PrivateAxleWeightValidStatus::INVALID;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;
    ECASTelltaleStatus3 mECASTelltaleStatus3 = ECASTelltaleStatus3::OFF;
    KneelingStatus mKneelingStatus = KneelingStatus::OFF;
    ALASTelltaleStatus1 mALASTelltaleStatus1 = ALASTelltaleStatus1::OFF;
    ALASTelltaleStatus2 mALASTelltaleStatus2 = ALASTelltaleStatus2::OFF;
    ConfigSuspType mConfigSuspType = ConfigSuspType::DISABLE;

    static constexpr HDouble kAxleWeightTonFactor = 50.0 / 100000.0;                // 0.0005
    static constexpr HDouble kAxleWeightx1000LbsFactor = 11.0 / 10000.0;            // 0.0011
    static constexpr HUInt64 kAxlePrivateBarInvalid = 0xFFFE;                       // --- INVALID
    static constexpr HUInt64 kAxlePrivateNoDisplay = 0xFFFF;                        // NO DISPLAY
    static constexpr HDouble kAxleConstantBarInvalid = static_cast<HDouble>(0xFE);  // INVALID
    static constexpr HDouble kAxleConstantNoDisplay = static_cast<HDouble>(0xFF);   // NO DISPLAY
    static constexpr HUInt64 kVehicleSpeed5KPH = 1280;                              // 5 * 256[KPH]
    static constexpr HUInt64 kMaximumAxleWeightFB90h = 0xFB90;
};
}  // namespace ccos

#endif  // SFSS_ECAS_H
