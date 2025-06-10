/**
 * @file AWD.hpp
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
#ifndef SFSS_AWD_H
#define SFSS_AWD_H

#define DLOG_ENABLED gEnableSFCLog

#include "AWDBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 21.0.0
// Reference : [PT030] AWD. Contains : Telltale, Event, Constant. Recommend: VALUE_CHANGED
class AWD : public AWDBase {
public:
    AWD() = default;
    ~AWD() override = default;
    AWD(const AWD& other) = delete;
    AWD(AWD&& other) noexcept = delete;
    AWD& operator=(const AWD& other) = delete;
    AWD& operator=(AWD&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerPitchRollDoNotDisplay.create(kTimerPitchRollDoNotDisplay, this,
                                                      &AWD::onOneShotTimerPitchRollDoNotDisplayTimeout, false) == false) {
            DWarning() << "[AWD] mOneShotTimerPitchRollDoNotDisplay Create Failed";
        }
        updateRpmGaugeBar();
        updateEngineOilTemp();
        setSFCAWDTelltaleLock_4WDBlinkValueA(SFCAWDTelltaleLock_4WDBlinkValueA::OPTIONAL);
        setSFCAWDTelltaleLock_4WDBlinkValueB(SFCAWDTelltaleLock_4WDBlinkValueB::OPTIONAL);
        setSFCAWDTelltaleAWDModeBlinkValueA(SFCAWDTelltaleAWDModeBlinkValueA::OPTIONAL);
        setSFCAWDTelltaleAWDModeBlinkValueB(SFCAWDTelltaleAWDModeBlinkValueB::OFF);
        setSFCAWDEventAWDID("E25101");
        setSFCAWDEventAWDLinkedSoundID("SND_PopUpWarn1");
        setSFCAWDEventAWDLinkedSoundType(SFCAWDEventAWDLinkedSoundType::REPEAT_COUNT);
        setSFCAWDEventAWDLinkedSoundRepeatCount(1);
        setSFCAWDEventAWDStat(SFCAWDEventAWDStat::OFF);
        setSFCAWDEventEldOperationID("E25108");
        setSFCAWDEventEldOperationLinkedSoundID("SND_PopUpWarn1");
        setSFCAWDEventEldOperationLinkedSoundType(SFCAWDEventEldOperationLinkedSoundType::REPEAT_COUNT);
        setSFCAWDEventEldOperationLinkedSoundRepeatCount(1);
        setSFCAWDEventEldWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCAWDEventEldWarningLinkedSoundType(SFCAWDEventEldWarningLinkedSoundType::REPEAT_COUNT);
        setSFCAWDEventEldWarningLinkedSoundRepeatCount(1);
        setSFCAWDEventAwdModeWarnOnOffStatusLinkedSoundType(SFCAWDEventAwdModeWarnOnOffStatusLinkedSoundType::REPEAT_COUNT);
        setSFCAWDEventAwdModeWarnOnOffStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_500ms;
        update4WDLockTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfig4WD = args.mInter_Config4WD;
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        mConfigELD = args.mInter_ConfigELD;
        updateAll();
    }

    void onConfigEawdHevChanged(const ArgumentsConfigEawdHevChanged& args) {
        mConfigEawdHev = args.mInter_ConfigEawdHev;
        updateFrontTorqueConstant();
        updateRearTorqueConstant();
    }

    void onAwdErrorStatChanged(const ArgumentsAwdErrorStatChanged& args) {
        if (ISTIMEOUT(args.mInput_AwdErrorStatus) == true) {
            mAwdErrorStatus = AwdErrorStatus::UNHANDLED_TIMEOUT;
        } else {
            mAwdErrorStatus = args.mInput_AwdErrorStatus;
        }
        updateAWDEvent();
    }

    void onAwdLockActStatChanged(const ArgumentsAwdLockActStatChanged& args) {
        mAwdLockActStatus = args.mInput_AwdLockActStatus;
        update4WDLockTelltale();
    }

    void onAwdFrontTorqueValueChanged(const ArgumentsAwdFrontTorqueValueChanged& args) {
        mAwdFrontTorqueValue = args.mInput_AwdFrontTorqueValue;
        mAwdFrontTorqueDisplayValue = args.mInput_AwdFrontTorqueDisplayValue;
        if (ISTIMEOUT(args.mInput_AwdFrontTorqueValue) == true) {
            mAwdFrontTorqueValue_timeout = true;
        } else {
            mAwdFrontTorqueValue_timeout = false;
        }
        if (ISTIMEOUT(args.mInput_AwdFrontTorqueDisplayValue) == true) {
            mAwdFrontTorqueDisplayValue_timeout = true;
        } else {
            mAwdFrontTorqueDisplayValue_timeout = false;
        }
        updateFrontTorqueConstant();
    }

    void onAwdRearTorqueValueChanged(const ArgumentsAwdRearTorqueValueChanged& args) {
        mAwdRearTorqueValue = args.mInput_AwdRearTorqueValue;
        mAwdRearTorqueDisplayValue = args.mInput_AwdRearTorqueDisplayValue;
        if (ISTIMEOUT(args.mInput_AwdRearTorqueValue) == true) {
            mAwdRearTorqueValue_timeout = true;
        } else {
            mAwdRearTorqueValue_timeout = false;
        }
        if (ISTIMEOUT(args.mInput_AwdRearTorqueDisplayValue) == true) {
            mAwdRearTorqueDisplayValue_timeout = true;
        } else {
            mAwdRearTorqueDisplayValue_timeout = false;
        }
        updateRearTorqueConstant();
    }

    void onAwdSteerAngleChanged(const ArgumentsAwdSteerAngleChanged& args) {
        mSteerAngleValue = static_cast<HDouble>(args.mInput_SteerAngleValue);
        updateSteerAngleConstant();
    }

    void onEldOperationStatusChanged(const ArgumentsEldOperationStatusChanged& args) {
        mEldOperationStatus = args.mInput_EldOperationStatus;
        update4WDLockTelltale();
        updateEldOperationEvent();
        updateEldLockStat();
    }

    void onEldWarningStatusChanged(const ArgumentsEldWarningStatusChanged& args) {
        mEldWarningStatus = args.mInput_EldWarningStatus;
        updateEldWarningEvent();
    }

    void onTireAngleBySteerValChanged(const ArgumentsTireAngleBySteerValChanged& args) {
        if (ISTIMEOUT(args.mInput_TireAngleBySteerVal) == true) {
            mTireAngleBySteerVal_timeout = true;
            mTireAngleBySteerVal = 0;
        } else {
            mTireAngleBySteerVal_timeout = false;
            mTireAngleBySteerVal = static_cast<HInt64>(args.mInput_TireAngleBySteerVal);
        }

        updateConstantTireAngDisplay();
    }

    void onOffRoadPitchValueChanged(const ArgumentsOffRoadPitchValueChanged& args) {
        mOffRoadPitchValue = args.mInput_OffRoadPitchValue;
        updateOffRoadPitchValue();
    }

    void onOffRoadRollValueChanged(const ArgumentsOffRoadRollValueChanged& args) {
        mOffRoadRollValue = args.mInput_OffRoadRollValue;
        updateOffRoadRollValue();
    }

    void onAwdModeStatChanged(const ArgumentsAwdModeStatChanged& args) {
        mAwdMode2HStatus = args.mInput_AwdMode2HStatus;
        mAwdMode4AStatus = args.mInput_AwdMode4AStatus;
        mAwdMode4HStatus = args.mInput_AwdMode4HStatus;
        mAwdMode4LStatus = args.mInput_AwdMode4LStatus;
        updateTelltaleAWDMode();
    }

    void onTransmissionOilTempValueChanged(const ArgumentsTransmissionOilTempValueChanged& args) {
        mTransmissionOilTempValue = args.mInput_TransmissionOilTempValue;
        if (ISTIMEOUT(args.mInput_TransmissionOilTempValue) == true) {
            mTransmissionOilTempValue_timeout = true;
        } else {
            mTransmissionOilTempValue_timeout = false;
        }
        updateTransmissionOilTemp();
    }

    void onOutTempUnitStatusChanged(const ArgumentsOutTempUnitStatusChanged& args) {
        mOutTempUnitStatus = args.mInput_OutTempUnitStatus;
        if (ISTIMEOUT(args.mInput_OutTempUnitStatus) == true) {
            mOutTempUnitStatus_timeout = true;
        } else {
            mOutTempUnitStatus_timeout = false;
        }
        updateTransmissionOilTemp();
    }

    void onAwdModeWarnOnOffStatusChanged(const ArgumentsAwdModeWarnOnOffStatusChanged& args) {
        mAwdModeWarnOnOffStatus = args.mInput_AwdModeWarnOnOffStatus;
        updateEventAwdModeWarnOnOffStatus();
    }

    void onEawdHevErrorStatusChanged(const ArgumentsEawdHevErrorStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_EawdHevErrorStatus) == true) {
            mEawdHevErrorStatus = EawdHevErrorStatus::UNHANDLED_TIMEOUT;
        } else {
            mEawdHevErrorStatus = args.mInput_EawdHevErrorStatus;
        }
        updateAWDEvent();
    }

    void onDisplaySpeedValueKPHChanged(const ArgumentsDisplaySpeedValueKPHChanged& args) {
        // 사양서에서는 timeout을 고려하게 되어있지만 고려하지 않는 이유는 첫째로 SFC가 보내주는 신호이고,
        // 둘째, Input_Output_Processing에서 CAN 차속이 timeout이 왔다면, Input_Output_Processing은 SFC node값에 0을 보내기
        // 때문입니다.
        // mInter_DisplaySpeedValueKPH가 0이오면 사양의 "120보다 크다면" 조건을 를 만족하지 못하므로
        // IMG_PitchRollDoNotDisplay_stat는 OFF로 나가서 올바르게 동작합니다.
        mDisplaySpeedValueKPH = args.mInter_DisplaySpeedValueKPH;
        updatePitchRollDoNotDisplayStatus();
    }

    void onRpmValueChanged(const ArgumentsRpmValueChanged& args) {
        mRpmValue = args.mRpmValue;
        updateRpmGaugeBar();
    }

    void onRpmDampStatChanged(const ArgumentsRpmDampStatChanged& args) {
        mRpmDampStat = args.mRpmDampStat;
        updateRpmGaugeBar();
    }

    void onMaxRpmStatChanged(const ArgumentsMaxRpmStatChanged& args) {
        mMaxRpmStat = args.mMaxRpmStat;
        updateRpmGaugeBar();
    }

    void onRpmRedZoneRangeStatChanged(const ArgumentsRpmRedZoneRangeStatChanged& args) {
        mRpmRedZoneRangeStat = args.mRedZoneNbrandStat;
        updateRpmGaugeBar();
    }

    void onEngineOilTempValueChanged(const ArgumentsEngineOilTempValueChanged& args) {
        mEngineOilTempValue = args.mEngineOilTempValue;
        updateEngineOilTemp();
    }

    void onEngineOilTempUnitStatChanged(const ArgumentsEngineOilTempUnitStatChanged& args) {
        mEngineOilTempUnitStat = args.mEngineOilTempUnitStat;
        updateEngineOilTemp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateAWDEvent();
        updateEldOperationEvent();
        updateEldWarningEvent();
        update4WDLockTelltale();
        updateSteerAngleConstant();
        updateConstantTireAngDisplay();
        updateFrontTorqueConstant();
        updateRearTorqueConstant();
        updateEldLockStat();
        updateEventAwdModeWarnOnOffStatus();
        updateTelltaleAWDMode();
    }

    void updateAWDEvent() {
        SFCAWDEventAWDStat status = SFCAWDEventAWDStat::OFF;
        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true) {
            if (mAwdErrorStatus == AwdErrorStatus::ERROR || mEawdHevErrorStatus == EawdHevErrorStatus::ERROR ||
                (mAwdErrorStatus == AwdErrorStatus::UNHANDLED_TIMEOUT &&
                 mEawdHevErrorStatus == EawdHevErrorStatus::UNHANDLED_TIMEOUT)) {
                status = SFCAWDEventAWDStat::ON;
            } else {
                // nothing
            }
        } else {
            // nothing
        }
        setSFCAWDEventAWDStat(status);
    }

    void updateEldOperationEvent() {
        SFCAWDEventEldOperationStat stat = SFCAWDEventEldOperationStat::OFF;

        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true && mEldOperationStatus == EldOperationStatus::ELD_ERR) {
            stat = SFCAWDEventEldOperationStat::ON;
        }

        setSFCAWDEventEldOperationStat(stat);
    }

    void updateEldWarningEvent() {
        std::string eventId;

        if (mIsIgnOn == true && mConfig4WD == Config4WD::ON) {
            switch (mEldWarningStatus) {
                case EldWarningStatus::NOT_CONNECT:
                    eventId = "E25102";
                    break;
                case EldWarningStatus::NOT_CONNECT_DETAIL:
                    eventId = "E25103";
                    break;
                case EldWarningStatus::NOT_CANCEL:
                    eventId = "E25104";
                    break;
                case EldWarningStatus::NOT_CANCEL_DETAIL:
                    eventId = "E25105";
                    break;
                case EldWarningStatus::AUTO_CANCEL:
                    eventId = "E25106";
                    break;
                case EldWarningStatus::AUTO_CONNECT:
                    eventId = "E25107";
                    break;
                case EldWarningStatus::CHECK_TIRE_SIZE:
                    eventId = "E25112";
                    break;
                case EldWarningStatus::CHECK_DRIVEMODE:
                    eventId = "E25113";
                    break;
                case EldWarningStatus::NOT_MET_CONDITION:
                    eventId = "E25114";
                    break;
                case EldWarningStatus::CANNOT_DISCONNECT:
                    eventId = "E25115";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        getInternalCachedValue("SFC.AWD.Event.EldWarning.ID", prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCAWDEventEldWarningStat(SFCAWDEventEldWarningStat::OFF);
            flushLastGroup();
        }

        if (eventId.empty() == false) {
            setSFCAWDEventEldWarningID(eventId);
            setSFCAWDEventEldWarningStat(SFCAWDEventEldWarningStat::ON);
        }
    }

    void update4WDLockTelltale() {
        SFCAWDTelltaleLock_4WDStat stat = SFCAWDTelltaleLock_4WDStat::OFF;
        SFCAWDTelltaleLock_4WDStatOptional statOptional = SFCAWDTelltaleLock_4WDStatOptional::OFF;
        SFCAWDTelltaleLock_4WDBlinkValueAOptional blinkValueAOptional = SFCAWDTelltaleLock_4WDBlinkValueAOptional::OFF;
        SFCAWDTelltaleLock_4WDBlinkValueBOptional blinkValueBOptional = SFCAWDTelltaleLock_4WDBlinkValueBOptional::OFF;

        if (mConfig4WD == Config4WD::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms && mConfigELD == ConfigELD::OFF) {
                // OFF
            } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
                if (mConfigELD == ConfigELD::OFF) {
                    if (mAwdLockActStatus == AwdLockActStatus::LOCK) {
                        stat = SFCAWDTelltaleLock_4WDStat::ON;
                    }
                } else {  // ConfigELD ON
                    if (mAwdLockActStatus == AwdLockActStatus::OFF) {
                        if (mEldOperationStatus == EldOperationStatus::ELD_ON) {
                            stat = SFCAWDTelltaleLock_4WDStat::OPTIONAL;
                            statOptional = SFCAWDTelltaleLock_4WDStatOptional::OFF_AWD_ON_ELD;
                        } else if (mEldOperationStatus == EldOperationStatus::ELD_BLINK) {
                            stat = SFCAWDTelltaleLock_4WDStat::BLINK1;
                            blinkValueAOptional = SFCAWDTelltaleLock_4WDBlinkValueAOptional::OFF_AWD_OFF_ELD;
                            blinkValueBOptional = SFCAWDTelltaleLock_4WDBlinkValueBOptional::OFF_AWD_ON_ELD;
                        } else {
                            // OFF
                        }
                    } else {  // mAwdLockActStatus == AwdLockActStatus::LOCK
                        if (mEldOperationStatus == EldOperationStatus::OFF) {
                            stat = SFCAWDTelltaleLock_4WDStat::OPTIONAL;
                            statOptional = SFCAWDTelltaleLock_4WDStatOptional::AWD_ON_ELD_OFF;
                        } else if (mEldOperationStatus == EldOperationStatus::ELD_ON) {
                            stat = SFCAWDTelltaleLock_4WDStat::OPTIONAL;
                            statOptional = SFCAWDTelltaleLock_4WDStatOptional::AWD_ON_ELD_ON;
                        } else if (mEldOperationStatus == EldOperationStatus::ELD_BLINK) {
                            stat = SFCAWDTelltaleLock_4WDStat::BLINK1;
                            blinkValueAOptional = SFCAWDTelltaleLock_4WDBlinkValueAOptional::AWD_ON_ELD_OFF;
                            blinkValueBOptional = SFCAWDTelltaleLock_4WDBlinkValueBOptional::AWD_ON_ELD_ON;
                        } else {
                            // OFF
                        }
                    }
                }
            } else {
                // OFF
            }
        }

        setSFCAWDTelltaleLock_4WDStat(stat);
        setSFCAWDTelltaleLock_4WDStatOptional(statOptional);
        setSFCAWDTelltaleLock_4WDBlinkValueAOptional(blinkValueAOptional);
        setSFCAWDTelltaleLock_4WDBlinkValueBOptional(blinkValueBOptional);
    }

    void updateTelltaleAWDMode() {
        SFCAWDTelltaleAWDModeStat stat = SFCAWDTelltaleAWDModeStat::OFF;
        SFCAWDTelltaleAWDModeStatOptional statOptional = SFCAWDTelltaleAWDModeStatOptional::OFF;
        SFCAWDTelltaleAWDModeBlinkValueAOptional blinkAOptional = SFCAWDTelltaleAWDModeBlinkValueAOptional::OFF;

        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true) {
            if (mAwdMode2HStatus == AwdMode2HStatus::ON && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::OPTIONAL;
                statOptional = SFCAWDTelltaleAWDModeStatOptional::MODE_2H;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::BLINK && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                       mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::BLINK2;
                blinkAOptional = SFCAWDTelltaleAWDModeBlinkValueAOptional::MODE_2H;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::ON &&
                       mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::OPTIONAL;
                statOptional = SFCAWDTelltaleAWDModeStatOptional::MODE_4A;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::BLINK &&
                       mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::BLINK2;
                blinkAOptional = SFCAWDTelltaleAWDModeBlinkValueAOptional::MODE_4A;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                       mAwdMode4HStatus == AwdMode4HStatus::ON && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::OPTIONAL;
                statOptional = SFCAWDTelltaleAWDModeStatOptional::MODE_4H;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                       mAwdMode4HStatus == AwdMode4HStatus::BLINK && mAwdMode4LStatus == AwdMode4LStatus::OFF) {
                stat = SFCAWDTelltaleAWDModeStat::BLINK2;
                blinkAOptional = SFCAWDTelltaleAWDModeBlinkValueAOptional::MODE_4H;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                       mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::ON) {
                stat = SFCAWDTelltaleAWDModeStat::OPTIONAL;
                statOptional = SFCAWDTelltaleAWDModeStatOptional::MODE_4L;
            } else if (mAwdMode2HStatus == AwdMode2HStatus::OFF && mAwdMode4AStatus == AwdMode4AStatus::OFF &&
                       mAwdMode4HStatus == AwdMode4HStatus::OFF && mAwdMode4LStatus == AwdMode4LStatus::BLINK) {
                stat = SFCAWDTelltaleAWDModeStat::BLINK2;
                blinkAOptional = SFCAWDTelltaleAWDModeBlinkValueAOptional::MODE_4L;
            } else {
                // OFF
            }
        }

        setSFCAWDTelltaleAWDModeStat(stat);
        setSFCAWDTelltaleAWDModeStatOptional(statOptional);
        setSFCAWDTelltaleAWDModeBlinkValueAOptional(blinkAOptional);
    }

    void updateSteerAngleConstant() {
        static SFCAWDConstantSteerAngleValStat status = SFCAWDConstantSteerAngleValStat::VALUE_0_DEGREE;

        calcSteerAngDegree();

        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true) {
            if ((status != SFCAWDConstantSteerAngleValStat::VALUE_0_DEGREE && mPrivate_SteerAngDegree >= -15 &&
                 mPrivate_SteerAngDegree <= 15) ||
                (status == SFCAWDConstantSteerAngleValStat::VALUE_0_DEGREE && mPrivate_SteerAngDegree > -25 &&
                 mPrivate_SteerAngDegree < 25)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_0_DEGREE;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_LEFT && mPrivate_SteerAngDegree >= 25 &&
                        mPrivate_SteerAngDegree <= 75) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_LEFT && mPrivate_SteerAngDegree > 15 &&
                        mPrivate_SteerAngDegree < 85)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_LEFT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_LEFT && mPrivate_SteerAngDegree >= 85 &&
                        mPrivate_SteerAngDegree <= 160) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_LEFT && mPrivate_SteerAngDegree > 75 &&
                        mPrivate_SteerAngDegree < 170)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_LEFT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_LEFT && mPrivate_SteerAngDegree >= 170 &&
                        mPrivate_SteerAngDegree <= 255) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_LEFT && mPrivate_SteerAngDegree > 160 &&
                        mPrivate_SteerAngDegree < 265)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_LEFT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_LEFT && mPrivate_SteerAngDegree >= 265 &&
                        mPrivate_SteerAngDegree <= 365) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_LEFT && mPrivate_SteerAngDegree > 255 &&
                        mPrivate_SteerAngDegree < 375)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_LEFT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_LEFT && mPrivate_SteerAngDegree >= 375) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_LEFT && mPrivate_SteerAngDegree > 365 &&
                        mPrivate_SteerAngDegree <= 375)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_LEFT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_RIGHT && mPrivate_SteerAngDegree >= -75 &&
                        mPrivate_SteerAngDegree <= -25) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_RIGHT && mPrivate_SteerAngDegree > -85 &&
                        mPrivate_SteerAngDegree < -15)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_6_DEGREE_RIGHT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_RIGHT && mPrivate_SteerAngDegree >= -160 &&
                        mPrivate_SteerAngDegree <= -85) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_RIGHT && mPrivate_SteerAngDegree > -170 &&
                        mPrivate_SteerAngDegree < -75)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_12_DEGREE_RIGHT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_RIGHT && mPrivate_SteerAngDegree >= -255 &&
                        mPrivate_SteerAngDegree <= -170) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_RIGHT && mPrivate_SteerAngDegree > -265 &&
                        mPrivate_SteerAngDegree < -160)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_18_DEGREE_RIGHT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_RIGHT && mPrivate_SteerAngDegree >= -365 &&
                        mPrivate_SteerAngDegree <= -265) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_RIGHT && mPrivate_SteerAngDegree > -375 &&
                        mPrivate_SteerAngDegree < -255)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_24_DEGREE_RIGHT;
            } else if ((status != SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_RIGHT && mPrivate_SteerAngDegree <= -375) ||
                       (status == SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_RIGHT && mPrivate_SteerAngDegree < -365)) {
                status = SFCAWDConstantSteerAngleValStat::VALUE_30_DEGREE_RIGHT;
            } else {
                // nothing
            }
        }
        setSFCAWDConstantSteerAngleValStat(status);
    }

    void updateConstantTireAngDisplay() {
        SFCAWDConstantTireAngDisplayStat stat = SFCAWDConstantTireAngDisplayStat::DISPLAY_OFF;
        HInt64 value = 0;
        if (mConfig4WD == Config4WD::ON && mTireAngleBySteerVal_timeout == false) {
            if (mTireAngleBySteerVal <= 125) {
                stat = SFCAWDConstantTireAngDisplayStat::ON;
                value = mTireAngleBySteerVal - 63;
            }
        }

        setSFCAWDConstantTireAngDisplayStat(stat);
        setSFCAWDConstantTireAngDisplayValue(value);
    }

    void updateFrontTorqueConstant() {
        SFCAWDConstantFrontTorqueValStat status = SFCAWDConstantFrontTorqueValStat::SEG0;
        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true) {
            switch (mConfigVehicleType) {
                case ConfigVehicleType::ICV:
                case ConfigVehicleType::HEV:
                case ConfigVehicleType::PHEV:
                    if (mConfigEawdHev != ConfigEawdHev::ON) {
                        status = getAwdTorqueValue<SFCAWDConstantFrontTorqueValStat>(false, mAwdFrontTorqueValue,
                                                                                     mAwdFrontTorqueValue_timeout);
                    } else {
                        status = getAwdTorqueValue<SFCAWDConstantFrontTorqueValStat>(true, mAwdFrontTorqueDisplayValue,
                                                                                     mAwdFrontTorqueDisplayValue_timeout);
                    }
                    break;
                case ConfigVehicleType::EV:
                case ConfigVehicleType::FCEV:
                case ConfigVehicleType::EREV:
                    status = getAwdTorqueValue<SFCAWDConstantFrontTorqueValStat>(true, mAwdFrontTorqueDisplayValue,
                                                                                 mAwdFrontTorqueDisplayValue_timeout);
                    break;
                default:
                    break;
            }
        }
        setSFCAWDConstantFrontTorqueValStat(status);
    }

    void updateRearTorqueConstant() {
        SFCAWDConstantRearTorqueValStat status = SFCAWDConstantRearTorqueValStat::SEG0;
        if (mConfig4WD == Config4WD::ON && mIsIgnOn == true) {
            switch (mConfigVehicleType) {
                case ConfigVehicleType::ICV:
                case ConfigVehicleType::HEV:
                case ConfigVehicleType::PHEV:
                    if (mConfigEawdHev != ConfigEawdHev::ON) {
                        status = getAwdTorqueValue<SFCAWDConstantRearTorqueValStat>(false, mAwdRearTorqueValue,
                                                                                    mAwdRearTorqueValue_timeout);
                    } else {
                        status = getAwdTorqueValue<SFCAWDConstantRearTorqueValStat>(true, mAwdRearTorqueDisplayValue,
                                                                                    mAwdRearTorqueDisplayValue_timeout);
                    }
                    break;
                case ConfigVehicleType::EV:
                case ConfigVehicleType::FCEV:
                case ConfigVehicleType::EREV:
                    status = getAwdTorqueValue<SFCAWDConstantRearTorqueValStat>(true, mAwdRearTorqueDisplayValue,
                                                                                mAwdRearTorqueDisplayValue_timeout);
                    break;
                default:
                    break;
            }
        }
        setSFCAWDConstantRearTorqueValStat(status);
    }

    void calcSteerAngDegree() {
        if (mSteerAngleValue < 32767) {
            mPrivate_SteerAngDegree = mSteerAngleValue * 0.1;
        } else if (mSteerAngleValue > 32767) {
            mPrivate_SteerAngDegree = (mSteerAngleValue - 65536) * 0.1;
        } else {
            // nothing
        }
    }

    void updateEldLockStat() {
        SFCAWDConstantEldLockStat status = SFCAWDConstantEldLockStat::OFF;
        if (mEldOperationStatus == EldOperationStatus::ELD_ON) {
            status = SFCAWDConstantEldLockStat::ON;
        } else if (mEldOperationStatus == EldOperationStatus::DISPLAY_OFF) {
            status = SFCAWDConstantEldLockStat::DISPLAY_OFF;
        } else {
            // nothing
        }
        setSFCAWDConstantEldLockStat(status);
    }

    void updateOffRoadPitchValue() {
        SFCAWDConstantOffRoadPitchStat status = SFCAWDConstantOffRoadPitchStat::OFF;
        HInt64 private_OffRoadPitchValue = 0;
        if (28 <= mOffRoadPitchValue && mOffRoadPitchValue <= 228) {
            status = SFCAWDConstantOffRoadPitchStat::ON;
            private_OffRoadPitchValue = round(atan((static_cast<HDouble>(mOffRoadPitchValue) - 128) / 100) * 180 / kPI);
        }
        setSFCAWDConstantOffRoadPitchStat(status);
        setSFCAWDConstantOffRoadPitchValue(private_OffRoadPitchValue);
    }

    void updateOffRoadRollValue() {
        SFCAWDConstantOffRoadRollStat status = SFCAWDConstantOffRoadRollStat::OFF;
        HInt64 private_OffRoadRollValue = 0;
        if (28 <= mOffRoadRollValue && mOffRoadRollValue <= 228) {
            status = SFCAWDConstantOffRoadRollStat::ON;
            private_OffRoadRollValue = round(atan((static_cast<HDouble>(mOffRoadRollValue) - 128) / 100) * 180 / kPI);
        }
        setSFCAWDConstantOffRoadRollStat(status);
        setSFCAWDConstantOffRoadRollValue(private_OffRoadRollValue);
    }

    void updatePitchRollDoNotDisplayStatus() {
        if (mDisplaySpeedValueKPH >= 120.0 && mPitchRollDoNotDisplayStat == SFCAWDConstantPitchRollDoNotDisplayStat::OFF) {
            mOneShotTimerPitchRollDoNotDisplay.stop();
            mPitchRollDoNotDisplayStat = SFCAWDConstantPitchRollDoNotDisplayStat::ON;
            setSFCAWDConstantPitchRollDoNotDisplayStat(mPitchRollDoNotDisplayStat);
        } else if (mDisplaySpeedValueKPH < 118.0 && mPitchRollDoNotDisplayStat == SFCAWDConstantPitchRollDoNotDisplayStat::ON) {
            if (mOneShotTimerPitchRollDoNotDisplay.isRunning() == false) {
                if (mOneShotTimerPitchRollDoNotDisplay.start() == false) {
                    DWarning() << "[AWD] mOneShotTimerPitchRollDoNotDisplay Start Failed";
                }
            }
        } else {
            mOneShotTimerPitchRollDoNotDisplay.stop();
        }
    }

    void updateRpmGaugeBar() {
        setSFCAWDConstantOffroadContentsRpmValue(mRpmValue);
        setSFCAWDConstantOffroadContentsRpmDampStat(static_cast<SFCAWDConstantOffroadContentsRpmDampStat>(mRpmDampStat));
        setSFCAWDConstantOffroadContentsMaxRpmStat(static_cast<SFCAWDConstantOffroadContentsMaxRpmStat>(mMaxRpmStat));
        setSFCAWDConstantOffroadContentsRpmRedZoneRangeStat(
            static_cast<SFCAWDConstantOffroadContentsRpmRedZoneRangeStat>(mRpmRedZoneRangeStat));
    }

    void updateEngineOilTemp() {
        setSFCAWDConstantOffroadContentsEngineOilValue(mEngineOilTempValue);
        setSFCAWDConstantOffroadContentsEngineOilunitStat(
            static_cast<SFCAWDConstantOffroadContentsEngineOilunitStat>(mEngineOilTempUnitStat));
    }

    void updateTransmissionOilTemp() {
        SFCAWDConstantTransmissionOilTempStat stat = SFCAWDConstantTransmissionOilTempStat::OFF;
        HInt64 value = 0;
        if (mTransmissionOilTempValue_timeout == false && mOutTempUnitStatus_timeout == false) {
            if (mTransmissionOilTempValue <= 254 && 0 <= mTransmissionOilTempValue) {
                if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS) {
                    stat = SFCAWDConstantTransmissionOilTempStat::ON_CELCIUS;
                    value = round(static_cast<HDouble>(mTransmissionOilTempValue) - 40.0);
                } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT) {
                    stat = SFCAWDConstantTransmissionOilTempStat::ON_FAHRENHEIT;
                    value = round((static_cast<HDouble>(mTransmissionOilTempValue) - 40.0) * (9.0 / 5.0) + 32.0);
                } else {
                    // others
                }
            }
        }
        setSFCAWDConstantTransmissionOilTempStat(stat);
        setSFCAWDConstantTransmissionOilTempValue(value);
    }

    void updateEventAwdModeWarnOnOffStatus() {
        std::string eventId;
        std::string linkedSoundId;

        if (mIsIgnOn == true && mConfig4WD == Config4WD::ON) {
            switch (mAwdModeWarnOnOffStatus) {
                case AwdModeWarnOnOffStatus::WARN_STOP_N_RETRY:
                    eventId = "E25109";
                    linkedSoundId = "SND_PopUpInform1";
                    break;
                case AwdModeWarnOnOffStatus::WARN_CANCEL:
                    eventId = "E25110";
                    linkedSoundId = "SND_PopUpWarn1";
                    break;
                case AwdModeWarnOnOffStatus::WARN_VEHICLE_STOP:
                    eventId = "E25111";
                    linkedSoundId = "SND_PopUpWarn1";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventId;
        getInternalCachedValue("SFC.AWD.Event.AwdModeWarnOnOffStatus.ID", prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCAWDEventAwdModeWarnOnOffStatusStat(SFCAWDEventAwdModeWarnOnOffStatusStat::OFF);
            flushLastGroup();
        }

        if (eventId.empty() == false) {
            setSFCAWDEventAwdModeWarnOnOffStatusID(eventId);
            setSFCAWDEventAwdModeWarnOnOffStatusLinkedSoundID(linkedSoundId);
            setSFCAWDEventAwdModeWarnOnOffStatusStat(SFCAWDEventAwdModeWarnOnOffStatusStat::ON);
        }
    }

    template <typename T>
    T getAwdTorqueValue(const HBool& isVehicleEV, const HUInt64& awdTorqueVal, const HBool& isTimeout) {
        T torqueVal;

        if (isVehicleEV == true) {
            if (isTimeout == true || awdTorqueVal == 0) {
                torqueVal = T::SEG0;
            } else if (awdTorqueVal >= 1 && awdTorqueVal <= 10) {
                torqueVal = T::SEG1;
            } else if (awdTorqueVal >= 11 && awdTorqueVal <= 20) {
                torqueVal = T::SEG2;
            } else if (awdTorqueVal >= 21 && awdTorqueVal <= 30) {
                torqueVal = T::SEG3;
            } else if (awdTorqueVal >= 31 && awdTorqueVal <= 40) {
                torqueVal = T::SEG4;
            } else if (awdTorqueVal >= 41 && awdTorqueVal <= 50) {
                torqueVal = T::SEG5;
            } else if (awdTorqueVal >= 51 && awdTorqueVal <= 60) {
                torqueVal = T::SEG6;
            } else if (awdTorqueVal >= 61 && awdTorqueVal <= 70) {
                torqueVal = T::SEG7;
            } else if (awdTorqueVal >= 71 && awdTorqueVal <= 80) {
                torqueVal = T::SEG8;
            } else if (awdTorqueVal >= 81 && awdTorqueVal <= 90) {
                torqueVal = T::SEG9;
            } else if (awdTorqueVal >= 91 && awdTorqueVal <= 127) {
                torqueVal = T::SEG10;
            } else {
                torqueVal = T::SEG0;
            }
        } else {
            if (isTimeout == true || (awdTorqueVal >= 0 && awdTorqueVal <= 1)) {
                torqueVal = T::SEG0;
            } else if (awdTorqueVal >= 2 && awdTorqueVal <= 6) {
                torqueVal = T::SEG1;
            } else if (awdTorqueVal >= 7 && awdTorqueVal <= 11) {
                torqueVal = T::SEG2;
            } else if (awdTorqueVal >= 12 && awdTorqueVal <= 16) {
                torqueVal = T::SEG3;
            } else if (awdTorqueVal >= 17 && awdTorqueVal <= 21) {
                torqueVal = T::SEG4;
            } else if (awdTorqueVal >= 22 && awdTorqueVal <= 26) {
                torqueVal = T::SEG5;
            } else if (awdTorqueVal >= 27 && awdTorqueVal <= 31) {
                torqueVal = T::SEG6;
            } else if (awdTorqueVal >= 32 && awdTorqueVal <= 36) {
                torqueVal = T::SEG7;
            } else if (awdTorqueVal >= 37 && awdTorqueVal <= 41) {
                torqueVal = T::SEG8;
            } else if (awdTorqueVal >= 42 && awdTorqueVal <= 46) {
                torqueVal = T::SEG9;
            } else if (awdTorqueVal >= 47 && awdTorqueVal <= 100) {
                torqueVal = T::SEG10;
            } else {
                torqueVal = T::SEG0;
            }
        }
        return torqueVal;
    }

    void onOneShotTimerPitchRollDoNotDisplayTimeout() {
        mPitchRollDoNotDisplayStat = SFCAWDConstantPitchRollDoNotDisplayStat::OFF;
        setSFCAWDConstantPitchRollDoNotDisplayStat(mPitchRollDoNotDisplayStat);
        flush();
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using Config4WD = ArgumentsConfigChanged::Inter_Config4WD;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigELD = ArgumentsConfigChanged::Inter_ConfigELD;
    using ConfigEawdHev = ArgumentsConfigEawdHevChanged::Inter_ConfigEawdHev;
    using AwdErrorStatus = ArgumentsAwdErrorStatChanged::Input_AwdErrorStatus;
    using AwdLockActStatus = ArgumentsAwdLockActStatChanged::Input_AwdLockActStatus;
    using EldOperationStatus = ArgumentsEldOperationStatusChanged::Input_EldOperationStatus;
    using EldWarningStatus = ArgumentsEldWarningStatusChanged::Input_EldWarningStatus;
    using AwdMode2HStatus = ArgumentsAwdModeStatChanged::Input_AwdMode2HStatus;
    using AwdMode4AStatus = ArgumentsAwdModeStatChanged::Input_AwdMode4AStatus;
    using AwdMode4HStatus = ArgumentsAwdModeStatChanged::Input_AwdMode4HStatus;
    using AwdMode4LStatus = ArgumentsAwdModeStatChanged::Input_AwdMode4LStatus;
    using OutTempUnitStatus = ArgumentsOutTempUnitStatusChanged::Input_OutTempUnitStatus;
    using AwdModeWarnOnOffStatus = ArgumentsAwdModeWarnOnOffStatusChanged::Input_AwdModeWarnOnOffStatus;
    using EawdHevErrorStatus = ArgumentsEawdHevErrorStatusChanged::Input_EawdHevErrorStatus;
    using RpmDampStat = ArgumentsRpmDampStatChanged::RpmDampStat;
    using MaxRpmStat = ArgumentsMaxRpmStatChanged::MaxRpmStat;
    using RpmRedZoneRangeStat = ArgumentsRpmRedZoneRangeStatChanged::RedZoneNbrandStat;
    using EngineOilTempUnitStat = ArgumentsEngineOilTempUnitStatChanged::EngineOilTempUnitStat;

    HBool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    Config4WD mConfig4WD = Config4WD::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigELD mConfigELD = ConfigELD::OFF;
    ConfigEawdHev mConfigEawdHev = ConfigEawdHev::OFF;
    AwdErrorStatus mAwdErrorStatus = AwdErrorStatus::OFF;
    AwdLockActStatus mAwdLockActStatus = AwdLockActStatus::OFF;
    EldOperationStatus mEldOperationStatus = EldOperationStatus::OFF;
    EldWarningStatus mEldWarningStatus = EldWarningStatus::OFF;
    AwdMode2HStatus mAwdMode2HStatus = AwdMode2HStatus::OFF;
    AwdMode4AStatus mAwdMode4AStatus = AwdMode4AStatus::OFF;
    AwdMode4HStatus mAwdMode4HStatus = AwdMode4HStatus::OFF;
    AwdMode4LStatus mAwdMode4LStatus = AwdMode4LStatus::OFF;
    HUInt64 mAwdFrontTorqueValue = 0;
    HUInt64 mAwdFrontTorqueDisplayValue = 0;
    HUInt64 mAwdRearTorqueValue = 0;
    HInt64 mTireAngleBySteerVal = 0;
    HUInt64 mAwdRearTorqueDisplayValue = 0;
    HUInt64 mOffRoadPitchValue = 0;
    HUInt64 mOffRoadRollValue = 0;
    OutTempUnitStatus mOutTempUnitStatus = OutTempUnitStatus::ERROR;
    HUInt64 mTransmissionOilTempValue = 0;
    HBool mAwdFrontTorqueValue_timeout = false;
    HBool mAwdFrontTorqueDisplayValue_timeout = false;
    HBool mAwdRearTorqueValue_timeout = false;
    HBool mAwdRearTorqueDisplayValue_timeout = false;
    HBool mTransmissionOilTempValue_timeout = false;
    HBool mOutTempUnitStatus_timeout = false;
    HDouble mSteerAngleValue = 0.0;
    HDouble mPrivate_SteerAngDegree = 0.0;
    HBool mTireAngleBySteerVal_timeout = false;
    AwdModeWarnOnOffStatus mAwdModeWarnOnOffStatus = AwdModeWarnOnOffStatus::OFF;
    EawdHevErrorStatus mEawdHevErrorStatus = EawdHevErrorStatus::OFF;
    HDouble mDisplaySpeedValueKPH = 0.0;
    HDouble mRpmValue = 0.0;
    RpmDampStat mRpmDampStat = RpmDampStat::NO_DAMPING;
    MaxRpmStat mMaxRpmStat = MaxRpmStat::RPM6000;
    RpmRedZoneRangeStat mRpmRedZoneRangeStat = RpmRedZoneRangeStat::STEP_FROM4000;
    HInt64 mEngineOilTempValue = 0;
    EngineOilTempUnitStat mEngineOilTempUnitStat = EngineOilTempUnitStat::DISPLAY_OFF;
    ssfs::SFCTimer<AWD> mOneShotTimerPitchRollDoNotDisplay;
    SFCAWDConstantPitchRollDoNotDisplayStat mPitchRollDoNotDisplayStat = SFCAWDConstantPitchRollDoNotDisplayStat::OFF;

    static constexpr HDouble kPI = 3.141592;
    static constexpr uint32_t kTimerPitchRollDoNotDisplay = 1000u;
};

}  // namespace ccos

#endif  // SFSS_AWD_H
