/**
 * @file TPMS.hpp
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
#ifndef SFSS_TPMS_H
#define SFSS_TPMS_H

#define DLOG_ENABLED gEnableSFCLog

#include <bitset>
#include <cmath>
#include "TPMSBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 9.0.3
// Reference : [CH180] TPMS. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class TPMS : public TPMSBase {
public:
    TPMS() = default;
    ~TPMS() override = default;
    TPMS(const TPMS& other) = delete;
    TPMS(TPMS&& other) noexcept = delete;
    TPMS& operator=(const TPMS& other) = delete;
    TPMS& operator=(TPMS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCTPMSEventOverPressureID("E31316");
        setSFCTPMSEventOverPressureLinkedSoundID("SND_PopUpWarn1");
        setSFCTPMSEventOverPressureLinkedSoundType(SFCTPMSEventOverPressureLinkedSoundType::REPEAT_COUNT);
        setSFCTPMSEventOverPressureLinkedSoundRepeatCount(1);

        if (mOneShotTimerAfterBlink4s.create(kTimerAfterBlink1Interval4s, this, &TPMS::onOneShotTimerAfterBlink, false) ==
            false) {
            DWarning() << "[TPMS] mOneShotTimerAfterBlink4s Create Failed";
        }
        if (mOneShotTimerAfterBlink70s.create(kTimerAfterBlink2Interval70s, this, &TPMS::onOneShotTimerAfterBlink, false) ==
            false) {
            DWarning() << "[TPMS] mOneShotTimerAfterBlink70s Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mTelltaleBlink70sStatus = false;
        mTelltaleBlink4sStatus = false;
        updateTelltaleDelayProcessing();
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltalePreWarning();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigTpms = args.mInter_ConfigTPMS;
        mConfigArea = args.mInter_ConfigArea;
        updateAll();
    }

    void onTpmsChanged(const ArgumentsTpmsChanged& args) {
        if (ISTIMEOUT(args.mInput_TpmsWarnStatus) == true) {
            mTpmsWarnStatus = TpmsWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mTpmsWarnStatus = args.mInput_TpmsWarnStatus;
        }
        if (ISTIMEOUT(args.mInput_TpmsTreadWarnStatus) == true) {
            mTpmsTreadWarnStatus = TpmsTreadWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mTpmsTreadWarnStatus = args.mInput_TpmsTreadWarnStatus;
        }
        mTpmsFrontLeftPosWarnStatus = args.mInput_TpmsFrontLeftPosWarnStatus;
        mTpmsFrontRightPosWarnStatus = args.mInput_TpmsFrontRightPosWarnStatus;
        mTpmsRearLeftPosWarnStatus = args.mInput_TpmsRearLeftPosWarnStatus;
        mTpmsRearRightPosWarnStatus = args.mInput_TpmsRearRightPosWarnStatus;
        mTpmsSensorStatus = args.mInput_TpmsSensorStatus;
        mTpmsPressureUnitStatus = args.mInput_TpmsPressureUnitStatus;
        mFL = args.mInput_TpmsFrontLeftTirePressureValue;
        mFR = args.mInput_TpmsFrontRightTirePressureValue;
        mRL = args.mInput_TpmsRearLeftTirePressureValue;
        mRR = args.mInput_TpmsRearRightTirePressureValue;
        updateAll();
    }

    void onDispReqStatusChanged(const ArgumentsDispReqStatusChanged& args) {
        mTpmsPressureSetDispReqStatus = args.mInput_TpmsPressureSetDispReqStatus;
        updateIndirectEvent();
    }

    void onTpmsOverPressWarnStatusChanged(const ArgumentsTpmsOverPressWarnStatusChanged& args) {
        mTpmsOverPressWarnStatus = args.mInput_TpmsOverPressWarnStatus;
        updateOverPressureEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        if (mIgnElapsed == IgnElapsed::ON_3500ms || mIgnElapsed == IgnElapsed::OFF_0ms) {
            updateTelltale();
        }
        updateConstant();
        updateEvent();
    }

    void updateTelltaleDelayProcessing() {
        setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::OFF);
    }

    void updateTelltalePreWarning() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::OFF);
            return;
        }
        setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::ON);
    }

    void onOneShotTimerAfterBlink() {
        // Set telltale stat after blink
        setSFCTPMSTelltaleTPMSStat(mTelltaleStatAfterBlink);
        flush();
        mTelltaleStatAfterBlink = SFCTPMSTelltaleTPMSStat::NONE;
    }

    void updateTelltale() {
        bool timerClearStatus4s = true;
        bool timerClearStatus70s = true;
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            mOneShotTimerAfterBlink4s.stop();
            mOneShotTimerAfterBlink70s.stop();
            setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::OFF);
            return;
        }

        if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::NORMAL) {
                setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::OFF);
                mTelltaleBlink70sStatus = false;
                mTelltaleBlink4sStatus = false;
            } else if (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::LOW_PRESSURE) {
                setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::ON);
                mTelltaleBlink70sStatus = false;
                mTelltaleBlink4sStatus = false;
            } else if (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::RESET) {
                timerClearStatus4s = false;
                timerClearStatus70s = true;
                if (mTelltaleBlink4sStatus == false) {
                    setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::BLINK1);
                    setSFCTPMSTelltaleTPMSBlinkValueA(SFCTPMSTelltaleTPMSBlinkValueA::ON);
                    setSFCTPMSTelltaleTPMSBlinkValueB(SFCTPMSTelltaleTPMSBlinkValueB::OFF);
                    mTelltaleStatAfterBlink = SFCTPMSTelltaleTPMSStat::OFF;
                    if (mOneShotTimerAfterBlink4s.isRunning() == false && mOneShotTimerAfterBlink4s.start() == false) {
                        DWarning() << "[TPMS] mOneShotTimerAfterBlink Start With kTimerAfterBlink1Interval4s Failed";
                    }
                    mTelltaleBlink70sStatus = false;
                    mTelltaleBlink4sStatus = true;
                }
            } else if (mTpmsWarnStatus == TpmsWarnStatus::VIRGIN) {
                setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::BLINK5);
                setSFCTPMSTelltaleTPMSBlinkValueA(SFCTPMSTelltaleTPMSBlinkValueA::BLINK1);
                setSFCTPMSTelltaleTPMSBlinkValueASubValueA(SFCTPMSTelltaleTPMSBlinkValueASubValueA::ON);
                setSFCTPMSTelltaleTPMSBlinkValueASubValueB(SFCTPMSTelltaleTPMSBlinkValueASubValueB::OFF);
                setSFCTPMSTelltaleTPMSBlinkValueB(SFCTPMSTelltaleTPMSBlinkValueB::OFF);
                mTelltaleBlink70sStatus = false;
                mTelltaleBlink4sStatus = false;
            } else {  // Others
                if (mConfigArea == Inter_ConfigArea::CHINA) {
                    setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::BLINK2);
                    setSFCTPMSTelltaleTPMSBlinkValueA(SFCTPMSTelltaleTPMSBlinkValueA::ON);
                    setSFCTPMSTelltaleTPMSBlinkValueB(SFCTPMSTelltaleTPMSBlinkValueB::OFF);
                    mTelltaleBlink70sStatus = false;
                    mTelltaleBlink4sStatus = false;
                } else {
                    timerClearStatus4s = true;
                    timerClearStatus70s = false;
                    if (mTelltaleBlink70sStatus == false) {  // 동일 조건이 만족할 경우 1회만 동작, 다른 조건이 만족할 경우
                                                             // false로 수정해서 재진입시 동작하도록 처리
                        setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::BLINK2);
                        setSFCTPMSTelltaleTPMSBlinkValueA(SFCTPMSTelltaleTPMSBlinkValueA::ON);
                        setSFCTPMSTelltaleTPMSBlinkValueB(SFCTPMSTelltaleTPMSBlinkValueB::OFF);
                        mTelltaleStatAfterBlink = SFCTPMSTelltaleTPMSStat::ON;
                        if (mOneShotTimerAfterBlink70s.isRunning() == false && mOneShotTimerAfterBlink70s.start() == false) {
                            DWarning() << "[TPMS] mOneShotTimerAfterBlink Start With kTimerAfterBlink2Interval70s Failed";
                        }
                        mTelltaleBlink70sStatus = true;
                        mTelltaleBlink4sStatus = false;
                    }
                }
            }
        } else {
            setSFCTPMSTelltaleTPMSStat(SFCTPMSTelltaleTPMSStat::OFF);
        }
        if (timerClearStatus4s == true) {
            mOneShotTimerAfterBlink4s.stop();
            mTelltaleBlink4sStatus = false;
        }
        if (timerClearStatus70s == true) {
            mOneShotTimerAfterBlink70s.stop();
            mTelltaleBlink70sStatus = false;
        }
    }

    void updateConstant() {
        updateLowPressureConstant();
        updatePressureAndUnitConstant();
        updateBeforeDrivingConstant();
        updatePressureMonitorConstant();
    }

    void updateLowPressureConstant() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            return;
        }

        mLowPressureConstantStat.reset();
        // FL Warning
        switch (mTpmsFrontLeftPosWarnStatus) {
            case TpmsFrontLeftPosWarnStatus::ON:
                setSFCTPMSConstantLowPressureFLStat(SFCTPMSConstantLowPressureFLStat::ON);
                mLowPressureConstantStat.set(kLowPressureStatFLIndex, true);
                break;
            case TpmsFrontLeftPosWarnStatus::OFF:
                setSFCTPMSConstantLowPressureFLStat(SFCTPMSConstantLowPressureFLStat::OFF);
                break;
            default:
                break;
        }
        // FR Warning
        switch (mTpmsFrontRightPosWarnStatus) {
            case TpmsFrontRightPosWarnStatus::ON:
                setSFCTPMSConstantLowPressureFRStat(SFCTPMSConstantLowPressureFRStat::ON);
                mLowPressureConstantStat.set(kLowPressureStatFRIndex, true);
                break;
            case TpmsFrontRightPosWarnStatus::OFF:
                setSFCTPMSConstantLowPressureFRStat(SFCTPMSConstantLowPressureFRStat::OFF);
                break;
            default:
                break;
        }
        // RL Warning
        switch (mTpmsRearLeftPosWarnStatus) {
            case TpmsRearLeftPosWarnStatus::ON:
                setSFCTPMSConstantLowPressureRLStat(SFCTPMSConstantLowPressureRLStat::ON);
                mLowPressureConstantStat.set(kLowPressureStatRLIndex, true);
                break;
            case TpmsRearLeftPosWarnStatus::OFF:
                setSFCTPMSConstantLowPressureRLStat(SFCTPMSConstantLowPressureRLStat::OFF);
                break;
            default:
                break;
        }
        // RR Warning
        switch (mTpmsRearRightPosWarnStatus) {
            case TpmsRearRightPosWarnStatus::ON:
                setSFCTPMSConstantLowPressureRRStat(SFCTPMSConstantLowPressureRRStat::ON);
                mLowPressureConstantStat.set(kLowPressureStatRRIndex, true);
                break;
            case TpmsRearRightPosWarnStatus::OFF:
                setSFCTPMSConstantLowPressureRRStat(SFCTPMSConstantLowPressureRRStat::OFF);
                break;
            default:
                break;
        }
    }

    void updatePressureAndUnitConstant() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            return;
        }

        // pressure**Stat
        if (mTpmsSensorStatus == TpmsSensorStatus::ACT_PRESSURE_ON) {
            HUInt64 pressureDisplayOffCount = 0;
            if (mFL == 0xFF) {
                setSFCTPMSConstantPressureFLStat(SFCTPMSConstantPressureFLStat::DISPLAY_OFF);
                pressureDisplayOffCount++;
            } else {
                setSFCTPMSConstantPressureFLStat(SFCTPMSConstantPressureFLStat::VALUE);
            }
            if (mFR == 0xFF) {
                setSFCTPMSConstantPressureFRStat(SFCTPMSConstantPressureFRStat::DISPLAY_OFF);
                pressureDisplayOffCount++;
            } else {
                setSFCTPMSConstantPressureFRStat(SFCTPMSConstantPressureFRStat::VALUE);
            }
            if (mRL == 0xFF) {
                setSFCTPMSConstantPressureRLStat(SFCTPMSConstantPressureRLStat::DISPLAY_OFF);
                pressureDisplayOffCount++;
            } else {
                setSFCTPMSConstantPressureRLStat(SFCTPMSConstantPressureRLStat::VALUE);
            }
            if (mRR == 0xFF) {
                setSFCTPMSConstantPressureRRStat(SFCTPMSConstantPressureRRStat::DISPLAY_OFF);
                pressureDisplayOffCount++;
            } else {
                setSFCTPMSConstantPressureRRStat(SFCTPMSConstantPressureRRStat::VALUE);
            }
            // pressureUnit
            switch (mTpmsPressureUnitStatus) {
                case TpmsPressureUnitStatus::PSI:
                    setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::PSI);
                    calculatePressureValue(kFactorPsi, kPsiMax);
                    break;
                case TpmsPressureUnitStatus::KPA:
                    setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::KPA);
                    calculatePressureValue(kFactorKpa, kKpaMax);
                    break;
                case TpmsPressureUnitStatus::BAR:
                    setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::BAR);
                    calculatePressureValue(kFactorBar, kBarMax);
                    break;
                case TpmsPressureUnitStatus::INVALID:
                    setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::DISPLAY_OFF);
                    // pressure**Stat
                    setSFCTPMSConstantPressureFLStat(SFCTPMSConstantPressureFLStat::DISPLAY_OFF);
                    setSFCTPMSConstantPressureFRStat(SFCTPMSConstantPressureFRStat::DISPLAY_OFF);
                    setSFCTPMSConstantPressureRLStat(SFCTPMSConstantPressureRLStat::DISPLAY_OFF);
                    setSFCTPMSConstantPressureRRStat(SFCTPMSConstantPressureRRStat::DISPLAY_OFF);
                    break;
                default:
                    break;
            }
            if (pressureDisplayOffCount == 4) {
                setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::DISPLAY_OFF);
            }

            setSFCTPMSConstantPressureFLValue(mFLFactored);
            setSFCTPMSConstantPressureFRValue(mFRFactored);
            setSFCTPMSConstantPressureRLValue(mRLFactored);
            setSFCTPMSConstantPressureRRValue(mRRFactored);
        } else {
            setSFCTPMSConstantPressureFLStat(SFCTPMSConstantPressureFLStat::DISPLAY_OFF);
            setSFCTPMSConstantPressureFRStat(SFCTPMSConstantPressureFRStat::DISPLAY_OFF);
            setSFCTPMSConstantPressureRLStat(SFCTPMSConstantPressureRLStat::DISPLAY_OFF);
            setSFCTPMSConstantPressureRRStat(SFCTPMSConstantPressureRRStat::DISPLAY_OFF);
            setSFCTPMSConstantPressureUnitStat(SFCTPMSConstantPressureUnitStat::DISPLAY_OFF);
        }
    }

    void calculatePressureValue(const HDouble factor, const HDouble max) {
        mFLFactored = factoredPressureValue(mFL, factor, max);
        mFRFactored = factoredPressureValue(mFR, factor, max);
        mRLFactored = factoredPressureValue(mRL, factor, max);
        mRRFactored = factoredPressureValue(mRR, factor, max);
    }

    inline HDouble factoredPressureValue(const HUInt64 tirePresure, const HDouble factor, const HDouble max) const {
        HDouble factoredValue = static_cast<HDouble>(tirePresure) * factor;
        return std::min(factoredValue, max);
    }

    void updateBeforeDrivingConstant() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            return;
        }

        if (mIgnElapsed >= IgnElapsed::ON_0ms && mTpmsWarnStatus == TpmsWarnStatus::NORMAL &&
            mTpmsTreadWarnStatus == TpmsTreadWarnStatus::NORMAL && mTpmsSensorStatus == TpmsSensorStatus::INACTIVE) {
            setSFCTPMSConstantPressureBeforeDrivingStat(SFCTPMSConstantPressureBeforeDrivingStat::ON);
        } else {
            setSFCTPMSConstantPressureBeforeDrivingStat(SFCTPMSConstantPressureBeforeDrivingStat::OFF);
        }
    }

    void updatePressureMonitorConstant() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            return;
        }

        SFCTPMSConstantPressureMonitorPageStat result = SFCTPMSConstantPressureMonitorPageStat::NORMAL;
        if (mIgnElapsed >= IgnElapsed::ON_0ms) {
            if ((mTpmsWarnStatus == TpmsWarnStatus::NORMAL &&
                 (mTpmsTreadWarnStatus == TpmsTreadWarnStatus::ERROR || mTpmsTreadWarnStatus == TpmsTreadWarnStatus::RESET)) ||
                (mTpmsWarnStatus == TpmsWarnStatus::ERROR) || (mTpmsWarnStatus == TpmsWarnStatus::VIRGIN) ||
                (mTpmsWarnStatus == TpmsWarnStatus::FAIL) || (mTpmsWarnStatus == TpmsWarnStatus::UNHANDLED_TIMEOUT)) {
                result = SFCTPMSConstantPressureMonitorPageStat::ERROR;
            } else if ((mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::LOW_PRESSURE) &&
                       (mTpmsFrontLeftPosWarnStatus == TpmsFrontLeftPosWarnStatus::OFF &&
                        mTpmsFrontRightPosWarnStatus == TpmsFrontRightPosWarnStatus::OFF &&
                        mTpmsRearLeftPosWarnStatus == TpmsRearLeftPosWarnStatus::OFF &&
                        mTpmsRearRightPosWarnStatus == TpmsRearRightPosWarnStatus::OFF)) {
                result = SFCTPMSConstantPressureMonitorPageStat::LOW_PRESSURE_SYMBOL_ONLY;
            } else if ((mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::LOW_PRESSURE) &&
                       (mTpmsFrontLeftPosWarnStatus == TpmsFrontLeftPosWarnStatus::ON ||
                        mTpmsFrontRightPosWarnStatus == TpmsFrontRightPosWarnStatus::ON ||
                        mTpmsRearLeftPosWarnStatus == TpmsRearLeftPosWarnStatus::ON ||
                        mTpmsRearRightPosWarnStatus == TpmsRearRightPosWarnStatus::ON)) {
                result = SFCTPMSConstantPressureMonitorPageStat::LOW_PRESSURE_DETAILED;
            } else {
                // Do nothing
            }
        }
        setSFCTPMSConstantPressureMonitorPageStat(result);
    }

    void updateEvent() {
        updateOverPressureEvent();
        updatePressureEvent();
        updateIndirectEvent();
    }

    void updatePressureEvent() {
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            setEventPressureStatOff();
            return;
        }
        if (mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::NONE || mIgnElapsed == IgnElapsed::MAX) {
            setEventPressureStatOff();
            return;
        }

        if (mTpmsWarnStatus == TpmsWarnStatus::ERROR || mTpmsWarnStatus == TpmsWarnStatus::FAIL ||
            (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::ERROR) ||
            mTpmsWarnStatus == TpmsWarnStatus::UNHANDLED_TIMEOUT) {
            setEventPressureStatOn("E31301", "SND_PopUpWarn1");
        } else if (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::LOW_PRESSURE) {
            switch (mTpmsSensorStatus) {
                case TpmsSensorStatus::INACTIVE:
                case TpmsSensorStatus::ACT_PRESSURE_ON:
                case TpmsSensorStatus::ACT_PRESSURE_OFF:
                    if (mLowPressureConstantStat.count() == 0) {
                        setEventPressureStatOn("E31302", "SND_PopUpWarn1");
                    } else {
                        static std::bitset<4> prevLowPressureStat{};
                        static TpmsSensorStatus prevSensorStat{};
                        setEventPressureStatOn("E31303", "SND_PopUpWarn1");
                        if (prevLowPressureStat != mLowPressureConstantStat || prevSensorStat != mTpmsSensorStatus) {
                            mUpdatedGroupSet.emplace("Event1");
                        }

                        prevLowPressureStat = mLowPressureConstantStat;
                        prevSensorStat = mTpmsSensorStatus;
                    }
                    break;
                case TpmsSensorStatus::ACT_INDIRECT_SWITCH_SET:
                    if (mLowPressureConstantStat.count() == 0) {
                        setEventPressureStatOn("E31304", "SND_PopUpWarn1");
                    } else if (mLowPressureConstantStat.count() == 1) {
                        if (mTpmsFrontLeftPosWarnStatus == TpmsFrontLeftPosWarnStatus::ON) {
                            setEventPressureStatOn("E31305", "SND_PopUpWarn1");
                        } else if (mTpmsFrontRightPosWarnStatus == TpmsFrontRightPosWarnStatus::ON) {
                            setEventPressureStatOn("E31306", "SND_PopUpWarn1");
                        } else if (mTpmsRearLeftPosWarnStatus == TpmsRearLeftPosWarnStatus::ON) {
                            setEventPressureStatOn("E31307", "SND_PopUpWarn1");
                        } else if (mTpmsRearRightPosWarnStatus == TpmsRearRightPosWarnStatus::ON) {
                            setEventPressureStatOn("E31308", "SND_PopUpWarn1");
                        } else {
                            setEventPressureStatOff();
                        }
                    } else {
                        setEventPressureStatOff();
                    }
                    break;
                case TpmsSensorStatus::ACT_INDIRECT_USM_SET:
                    if (mLowPressureConstantStat.count() == 0) {
                        setEventPressureStatOn("E31317", "SND_PopUpWarn1");
                    } else if (mLowPressureConstantStat.count() == 1) {
                        if (mTpmsFrontLeftPosWarnStatus == TpmsFrontLeftPosWarnStatus::ON) {
                            setEventPressureStatOn("E31318", "SND_PopUpWarn1");
                        } else if (mTpmsFrontRightPosWarnStatus == TpmsFrontRightPosWarnStatus::ON) {
                            setEventPressureStatOn("E31319", "SND_PopUpWarn1");
                        } else if (mTpmsRearLeftPosWarnStatus == TpmsRearLeftPosWarnStatus::ON) {
                            setEventPressureStatOn("E31320", "SND_PopUpWarn1");
                        } else if (mTpmsRearRightPosWarnStatus == TpmsRearRightPosWarnStatus::ON) {
                            setEventPressureStatOn("E31321", "SND_PopUpWarn1");
                        } else {
                            setEventPressureStatOff();
                        }
                    } else {
                        setEventPressureStatOff();
                    }
                    break;
                default:
                    setEventPressureStatOff();
                    break;
            }
        } else if (mTpmsWarnStatus == TpmsWarnStatus::VIRGIN) {
            switch (mTpmsSensorStatus) {
                case TpmsSensorStatus::INACTIVE:
                case TpmsSensorStatus::ACT_PRESSURE_ON:
                case TpmsSensorStatus::ACT_PRESSURE_OFF:
                    setEventPressureStatOn("E31309", "SND_PopUpWarn1");
                    break;
                case TpmsSensorStatus::ACT_INDIRECT_SWITCH_SET:
                case TpmsSensorStatus::ACT_INDIRECT_USM_SET:
                    setEventPressureStatOn("E31310", "SND_PopUpWarn1");
                    break;
                default:
                    setEventPressureStatOff();
                    break;
            }
        } else if (mTpmsWarnStatus == TpmsWarnStatus::NORMAL && mTpmsTreadWarnStatus == TpmsTreadWarnStatus::RESET) {
            switch (mTpmsSensorStatus) {
                case TpmsSensorStatus::ACT_INDIRECT_SWITCH_SET:
                case TpmsSensorStatus::ACT_INDIRECT_USM_SET:
                    setEventPressureStatOn("E31311", "SND_PopUpInform2");
                    break;
                default:
                    setEventPressureStatOff();
                    break;
            }
        } else {
            setEventPressureStatOff();
        }
    }

    void setEventPressureStatOff() {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.TPMS.Event.Pressure.ID, prevEventID);
        if (!prevEventID.empty()) {
            DDebug() << "SFCTPMSEventPressureStat::OFF ";
            setSFCTPMSEventPressureStat(SFCTPMSEventPressureStat::OFF);
        }
    }

    void setEventPressureStatOn(const std::string& eventID, const std::string& soundID) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.TPMS.Event.Pressure.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            DDebug() << "SFCTPMSEventPressureStat::OFF ";
            setSFCTPMSEventPressureStat(SFCTPMSEventPressureStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCTPMSEventPressureID(eventID);
            setSFCTPMSEventPressureLinkedSoundID(soundID);
            setSFCTPMSEventPressureStat(SFCTPMSEventPressureStat::ON);
            setSFCTPMSEventPressureLinkedSoundType(SFCTPMSEventPressureLinkedSoundType::REPEAT_COUNT);
            setSFCTPMSEventPressureLinkedSoundRepeatCount(1);
        }
    }

    void updateIndirectEvent() {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.TPMS.Event.indirectPressure.ID, prevEventID);
        if (mConfigTpms != Inter_ConfigTPMS::ON) {
            if (!prevEventID.empty()) {
                DDebug() << "SFCTPMSEventPressureStat::OFF ";
                setSFCTPMSEventIndirectPressureStat(SFCTPMSEventIndirectPressureStat::OFF);
            }
            return;
        }
        if (mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::NONE || mIgnElapsed == IgnElapsed::MAX) {
            if (!prevEventID.empty()) {
                DDebug() << "SFCTPMSEventPressureStat::OFF ";
                setSFCTPMSEventIndirectPressureStat(SFCTPMSEventIndirectPressureStat::OFF);
            }
            return;
        }

        std::string eventID;
        std::string soundID = "SND_PopUpInform1";
        SFCTPMSEventIndirectPressureLinkedSoundType soundType = SFCTPMSEventIndirectPressureLinkedSoundType::NONE;

        switch (mTpmsPressureSetDispReqStatus) {
            case TpmsPressureSetDispReqStatus::SELECTION:
                eventID = "E31312";
                break;
            case TpmsPressureSetDispReqStatus::COMPLETE:
                eventID = "E31313";
                soundType = SFCTPMSEventIndirectPressureLinkedSoundType::REPEAT_COUNT;
                soundID = "SND_PopUpInform1";
                break;
            case TpmsPressureSetDispReqStatus::INCOMPLETE:
                eventID = "E31314";
                soundType = SFCTPMSEventIndirectPressureLinkedSoundType::REPEAT_COUNT;
                soundID = "SND_PopUpWarn1";
                break;
            case TpmsPressureSetDispReqStatus::NEED_TO_STOP:
                eventID = "E31315";
                soundType = SFCTPMSEventIndirectPressureLinkedSoundType::REPEAT_COUNT;
                soundID = "SND_PopUpWarn1";
                break;
            default:
                break;
        }

        if (!prevEventID.empty() && prevEventID != eventID) {
            DDebug() << "SFCE_LSDEventE_LSDStat::OFF ";
            setSFCTPMSEventIndirectPressureStat(SFCTPMSEventIndirectPressureStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCTPMSEventIndirectPressureID(eventID);
            setSFCTPMSEventIndirectPressureStat(SFCTPMSEventIndirectPressureStat::ON);
            setSFCTPMSEventIndirectPressureLinkedSoundID(soundID);
            setSFCTPMSEventIndirectPressureLinkedSoundType(soundType);
            setSFCTPMSEventIndirectPressureLinkedSoundRepeatCount(1);
        }
    }

    void updateOverPressureEvent() {
        if (mConfigTpms == Inter_ConfigTPMS::ON && mIgnElapsed >= IgnElapsed::ON_0ms &&
            mTpmsOverPressWarnStatus == TpmsOverPressWarnStatus::ON) {
            setSFCTPMSEventOverPressureStat(SFCTPMSEventOverPressureStat::ON);
        } else {
            setSFCTPMSEventOverPressureStat(SFCTPMSEventOverPressureStat::OFF);
        }
    }

    using Inter_ConfigArea = ArgumentsConfigChanged::Inter_ConfigArea;
    using Inter_ConfigTPMS = ArgumentsConfigChanged::Inter_ConfigTPMS;
    using TpmsTreadWarnStatus = ArgumentsTpmsChanged::Input_TpmsTreadWarnStatus;
    using TpmsWarnStatus = ArgumentsTpmsChanged::Input_TpmsWarnStatus;
    using TpmsFrontLeftPosWarnStatus = ArgumentsTpmsChanged::Input_TpmsFrontLeftPosWarnStatus;
    using TpmsFrontRightPosWarnStatus = ArgumentsTpmsChanged::Input_TpmsFrontRightPosWarnStatus;
    using TpmsRearLeftPosWarnStatus = ArgumentsTpmsChanged::Input_TpmsRearLeftPosWarnStatus;
    using TpmsRearRightPosWarnStatus = ArgumentsTpmsChanged::Input_TpmsRearRightPosWarnStatus;
    using TpmsSensorStatus = ArgumentsTpmsChanged::Input_TpmsSensorStatus;
    using TpmsPressureUnitStatus = ArgumentsTpmsChanged::Input_TpmsPressureUnitStatus;
    using TpmsPressureSetDispReqStatus = ArgumentsDispReqStatusChanged::Input_TpmsPressureSetDispReqStatus;
    using TpmsOverPressWarnStatus = ArgumentsTpmsOverPressWarnStatusChanged::Input_TpmsOverPressWarnStatus;

    Inter_ConfigArea mConfigArea = Inter_ConfigArea::DOM;
    Inter_ConfigTPMS mConfigTpms = Inter_ConfigTPMS::OFF;
    TpmsTreadWarnStatus mTpmsTreadWarnStatus = TpmsTreadWarnStatus::NORMAL;
    TpmsWarnStatus mTpmsWarnStatus = TpmsWarnStatus::NORMAL;
    TpmsFrontLeftPosWarnStatus mTpmsFrontLeftPosWarnStatus = TpmsFrontLeftPosWarnStatus::OFF;
    TpmsFrontRightPosWarnStatus mTpmsFrontRightPosWarnStatus = TpmsFrontRightPosWarnStatus::OFF;
    TpmsRearLeftPosWarnStatus mTpmsRearLeftPosWarnStatus = TpmsRearLeftPosWarnStatus::OFF;
    TpmsRearRightPosWarnStatus mTpmsRearRightPosWarnStatus = TpmsRearRightPosWarnStatus::OFF;
    TpmsSensorStatus mTpmsSensorStatus = TpmsSensorStatus::INVALID;
    TpmsPressureUnitStatus mTpmsPressureUnitStatus = TpmsPressureUnitStatus::INVALID;
    TpmsPressureSetDispReqStatus mTpmsPressureSetDispReqStatus = TpmsPressureSetDispReqStatus::OFF;
    TpmsOverPressWarnStatus mTpmsOverPressWarnStatus = TpmsOverPressWarnStatus::OFF;

    static constexpr HDouble kFactorPsi = 1;
    static constexpr HDouble kFactorKpa = 5;
    static constexpr HDouble kFactorBar = 0.1;
    static constexpr HDouble kPsiMax = 254;
    static constexpr HDouble kKpaMax = 999;
    static constexpr HDouble kBarMax = 9.9;
    static constexpr uint32_t kTimerAfterBlink1Interval4s = 4000u;
    static constexpr uint32_t kTimerAfterBlink2Interval70s = 70000u;

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HUInt64 mFL = 0xFF;
    HUInt64 mFR = 0xFF;
    HUInt64 mRL = 0xFF;
    HUInt64 mRR = 0xFF;

    HDouble mFLFactored = -1.0;
    HDouble mFRFactored = -1.0;
    HDouble mRLFactored = -1.0;
    HDouble mRRFactored = -1.0;

    static constexpr HUInt64 kLowPressureStatFLIndex = 0;
    static constexpr HUInt64 kLowPressureStatFRIndex = 1;
    static constexpr HUInt64 kLowPressureStatRLIndex = 2;
    static constexpr HUInt64 kLowPressureStatRRIndex = 3;
    std::bitset<4> mLowPressureConstantStat;  // {FL, FR, RL, RR}

    bool mTelltaleBlink70sStatus = false;
    bool mTelltaleBlink4sStatus = false;
    ssfs::SFCTimer<TPMS> mOneShotTimerAfterBlink4s;
    ssfs::SFCTimer<TPMS> mOneShotTimerAfterBlink70s;
    SFCTPMSTelltaleTPMSStat mTelltaleStatAfterBlink = SFCTPMSTelltaleTPMSStat::NONE;
};

}  // namespace ccos

#endif  // SFSS_TPMS_H
