/**
 * @file ISG.hpp
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
#ifndef SFSS_ISG_H
#define SFSS_ISG_H
#define LOG_TAG "SFC(ISG)"
#define DLOG_ENABLED gEnableSFCLog

#include "ISGBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 6.0.1
// Reference : [PT300] ISG. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class ISG : public ISGBase {
public:
    ISG() = default;
    ~ISG() override = default;
    ISG(const ISG& other) = delete;
    ISG(ISG&& other) noexcept = delete;
    ISG& operator=(const ISG& other) = delete;
    ISG& operator=(ISG&& other) noexcept = delete;

    void onInitialize() override {
        setSFCISGSoundISGID("SND_ISGWarn");
        setSFCISGSoundISGType(SFCISGSoundISGType::INFINITE);
        if (mRepeatTimerISGAbleTime.create(kTimerISGAbleTimeInterval1s, this, &ISG::onRepeatTimerISGAbleTimeCallback, true) ==
            false) {
            DWarning() << "Timer mRepeatTimerISGAbleTime creation failed!";
        }
        if (mRepeatTimerISGAbleTimeCurrentTrip.create(kTimerISGAbleTimeInterval1s, this,
                                                      &ISG::onRepeatTimerISGAbleTimeCurrentTripCallback, true) == false) {
            DWarning() << "Timer mRepeatTimerISGAbleTimeCurrentTrip creation failed in onInitialize()";
        }
        if (mRepeatTimerISGAbleTimeSinceReset.create(kTimerISGAbleTimeInterval1s, this,
                                                     &ISG::onRepeatTimerISGAbleTimeSinceResetCallback, true) == false) {
            DWarning() << "Timer mRepeatTimerISGAbleTimeSinceReset creation failed in onInitialize()";
        }
        if (mRepeatTimerISGAbleTimeSinceRefueling.create(kTimerISGAbleTimeInterval1s, this,
                                                         &ISG::onRepeatTimerISGAbleTimeSinceRefuelingCallback, true) == false) {
            DWarning() << "Timer mRepeatTimerISGAbleTimeSinceRefueling creation failed in onInitialize()";
        }
        if (mOneShotTimerTelltale.create(kTimerISGTelltaleInterval5s, this, &ISG::onOneShotTimerTelltaleCallback, false) ==
            false) {
            DWarning() << "Timer mOneShotTimerTelltale creation failed in onInitialize()";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstantIsgDisableCondStat();
        runIsgAllTimer();
        requestUpdateTelltale();
        updateEvent();
        updateSound();
    }

    void onIgnElapseOn3000msChanged(const ArgumentsIgnElapseOn3000msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3000ms;
        requestUpdateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        requestUpdateTelltale();
        updateEvent();
        updateSound();
        updateConstantIsgDisableCondStat();
        runIsgAllTimer();
        memoryIsgAbleTime();
    }

    void onConfigIsgChanged(const ArgumentsConfigIsgChanged& args) {
        mConfigISG = args.mInter_ConfigISG;
        if (mConfigISG != ConfigISG::ON) {
            setAllSignalOff();
        } else {
            updateConstantIsgDisableCondStat();
            updateConstantIsgPageDisplayStat();
            runIsgAllTimer();
            requestUpdateTelltale();
            updateEvent();
            updateSound();
        }
    }

    void onIsgAutoStopTimeManualResetReqChanged(const ArgumentsIsgAutoStopTimeManualResetReqChanged& args) {
        mAutoStopTimeManualResetReq = args.mInter_AutoStopTimeManualResetReq;
        resetISGAbleTime();
        updateConstantIsgAbleTime();
        mPrevAutoStopTimeManualResetReq = mAutoStopTimeManualResetReq;
    }

    void onEngineISGStatChanged(const ArgumentsEngineISGStatChanged& args) {
        mEngineISGStatus1 = args.mInput_EngineISGStatus1;
        mEngineISGStatus2 = args.mInput_EngineISGStatus2;
        runIsgAllTimer();
        requestUpdateTelltale();
    }

    void onIsgWarnSoundStatChanged(const ArgumentsIsgWarnSoundStatChanged& args) {
        mIsgWarnSoundStatus = args.mInput_IsgWarnSoundStatus;
        updateSound();
    }

    void onIsgOperStatDispReqStatChanged(const ArgumentsIsgOperStatDispReqStatChanged& args) {
        mIsgOperStatDispReqStatus = args.mInput_IsgOperStatDispReqStatus;
        updateConstantIsgDisableCondStat();
    }

    void onIsgWarnDispReqStatChanged(const ArgumentsIsgWarnDispReqStatChanged& args) {
        mIsgWarnDispReqStatus = args.mInput_IsgWarnDispReqStatus;
        updateEvent();
    }

    void onPtLogicControlType(const ArgumentsPtLogicControlType& args) {
        mConfigPTLogicControlType = args.mInter_PTLogicControlType;
        runIsgAllTimer();
        updateConstantIsgAbleTime();
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeHourValue, mPrivate_IsgAbleTimeHourValue);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeMinValue, mPrivate_IsgAbleTimeMinValue);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeSecValue, mPrivate_IsgAbleTimeSecValue);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeHourCurrentTrip, mPrivate_IsgAbleTimeHourCurrentTrip);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeMinCurrentTrip, mPrivate_IsgAbleTimeMinCurrentTrip);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeSecCurrentTrip, mPrivate_IsgAbleTimeSecCurrentTrip);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeHourSinceReset, mPrivate_IsgAbleTimeHourSinceReset);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeMinSinceReset, mPrivate_IsgAbleTimeMinSinceReset);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeSecSinceReset, mPrivate_IsgAbleTimeSecSinceReset);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeHourSinceRefueling, mPrivate_IsgAbleTimeHourSinceRefueling);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeMinSinceRefueling, mPrivate_IsgAbleTimeMinSinceRefueling);
            GETCACHEDVALUE(SFC.Extension.Memory.Inter_IsgAbleTimeSecSinceRefueling, mPrivate_IsgAbleTimeSecSinceRefueling);
            updateConstantIsgAbleTime();
            runIsgAllTimer();
        }
    }

    void onIsgOperationTimeChanged(const ArgumentsIsgOperationTimeChanged& args) {
        mIsgAbleTimeHourValue = args.mInput_IsgOperationTimeHourValue;
        mIsgAbleTimeMinValue = args.mInput_IsgOperationTimeMinValue;
        if (mIsgAbleTimeMinValue > 59) {
            mIsgAbleTimeMinValue = 59;
        }
        mIsgAbleTimeSecValue = args.mInput_IsgOperationTimeSecValue;
        if (mIsgAbleTimeSecValue > 59) {
            mIsgAbleTimeSecValue = 59;
        }
        updateConstantIsgAbleTime();
    }

    void onAccumResetReqChanged(const ArgumentsAccumResetReqChanged& args) {
        mAccumInfoResetReq = args.mInter_AccumInfoResetReq;
        resetISGSinceReset();
        updateConstantIsgAbleTime();
        mPrevAccumInfoResetReq = mAccumInfoResetReq;
    }

    void onAfterRefuelInfoResetReqChanged(const ArgumentsAfterRefuelInfoResetReqChanged& args) {
        mAfterRefuelInfoResetReq = args.mInter_AfterRefuelInfoResetReq;
        resetISGSinceRefueling();
        updateConstantIsgAbleTime();
        mPrevAfterRefuelInfoResetReq = mAfterRefuelInfoResetReq;
    }

    void onDriveInfoResetReqChanged(const ArgumentsDriveInfoResetReqChanged& args) {
        mDriveInfoResetReq = args.mInter_DriveInfoResetReq;
        resetISGCurrentTrip();
        updateConstantIsgAbleTime();
        mPrevDriveInfoResetReq = mDriveInfoResetReq;
    }

    void onAutoStopTimerSetStatusChanged(const ArgumentsAutoStopTimerSetStatusChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInter_AutoStopTimerSetStatus) == true) {
            mAutoStopTimerSetStatus = args.mInter_AutoStopTimerSetStatus;
            updateConstantIsgPageDisplayStat();
            updateConstantIsgAbleTime();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void setAllSignalOff() {
        setSFCISGConstantIsgDisableCondStat(SFCISGConstantIsgDisableCondStat::OFF);
        setSFCISGSoundISGStat(SFCISGSoundISGStat::OFF);
        setSFCISGEventISGStat(SFCISGEventISGStat::OFF);
        mRepeatTimerISGAbleTime.stop();
        mRepeatTimerISGAbleTimeCurrentTrip.stop();
        mRepeatTimerISGAbleTimeSinceReset.stop();
        mRepeatTimerISGAbleTimeSinceRefueling.stop();
        mOneShotTimerTelltale.stop();
        setSFCISGTelltaleISGStat(SFCISGTelltaleISGStat::OFF);
        setSFCISGTelltaleISGBlinkValueA(SFCISGTelltaleISGBlinkValueA::OFF);
        setSFCISGTelltaleISGBlinkValueB(SFCISGTelltaleISGBlinkValueB::OFF);
        mIsTelltaleBlink2GreenOff = false;
    }

    void memoryIsgAbleTime() {
        if (mAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeHourValue", mPrivate_IsgAbleTimeHourValue);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeMinValue", mPrivate_IsgAbleTimeMinValue);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeSecValue", mPrivate_IsgAbleTimeSecValue);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeHourCurrentTrip", mPrivate_IsgAbleTimeHourCurrentTrip);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeMinCurrentTrip", mPrivate_IsgAbleTimeMinCurrentTrip);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeSecCurrentTrip", mPrivate_IsgAbleTimeSecCurrentTrip);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeHourSinceReset", mPrivate_IsgAbleTimeHourSinceReset);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeMinSinceReset", mPrivate_IsgAbleTimeMinSinceReset);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeSecSinceReset", mPrivate_IsgAbleTimeSecSinceReset);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeHourSinceRefueling", mPrivate_IsgAbleTimeHourSinceRefueling);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeMinSinceRefueling", mPrivate_IsgAbleTimeMinSinceRefueling);
            updateValue("SFC.Extension.Memory.Inter_IsgAbleTimeSecSinceRefueling", mPrivate_IsgAbleTimeSecSinceRefueling);
        }
    }

    void updateConstantIsgPageDisplayStat() {
        const auto iter = mIsgPageMap.find(mAutoStopTimerSetStatus);
        if (mConfigISG == ConfigISG::ON && iter != mIsgPageMap.end()) {
            setSFCISGConstantIsgPageDisplayStat(iter->second);
        }
    }

    void updateConstantIsgDisableCondStat() {
        if (mConfigISG == ConfigISG::ON && mIsIgnOn == true) {
            SFCISGConstantIsgDisableCondStat stat = SFCISGConstantIsgDisableCondStat::OFF;
            const auto iter = mISGOperStatMap.find(mIsgOperStatDispReqStatus);
            if (iter != mISGOperStatMap.end()) {
                stat = iter->second;
            }
            setSFCISGConstantIsgDisableCondStat(stat);
            updateConstantIsgAbleTime();
        }
    }

    void updateConstantIsgAbleTime() {
        HBool valueRenewed = false;
        HUInt64 hourValue = 0;
        HUInt64 minValue = 0;
        HUInt64 secValue = 0;

        if (mConfigISG == ConfigISG::ON) {
            if (mAutoStopTimerSetStatus == AutoStopTimerSetStatus::MANUAL_RESET) {
                if (mConfigPTLogicControlType == ConfigPTLogicControlType::OFF) {
                    hourValue = mPrivate_IsgAbleTimeHourValue;
                    minValue = mPrivate_IsgAbleTimeMinValue;
                    secValue = mPrivate_IsgAbleTimeSecValue;
                    valueRenewed = true;
                } else if (mConfigPTLogicControlType == ConfigPTLogicControlType::ON) {
                    hourValue = mIsgAbleTimeHourValue;
                    minValue = mIsgAbleTimeMinValue;
                    secValue = mIsgAbleTimeSecValue;
                    valueRenewed = true;
                } else {
                    // no operation
                }
            } else if (mAutoStopTimerSetStatus == AutoStopTimerSetStatus::CURRENT_TRIP) {
                hourValue = mPrivate_IsgAbleTimeHourCurrentTrip;
                minValue = mPrivate_IsgAbleTimeMinCurrentTrip;
                secValue = mPrivate_IsgAbleTimeSecCurrentTrip;
                valueRenewed = true;
            } else if (mAutoStopTimerSetStatus == AutoStopTimerSetStatus::SINCE_RESET) {
                hourValue = mPrivate_IsgAbleTimeHourSinceReset;
                minValue = mPrivate_IsgAbleTimeMinSinceReset;
                secValue = mPrivate_IsgAbleTimeSecSinceReset;
                valueRenewed = true;
            } else if (mAutoStopTimerSetStatus == AutoStopTimerSetStatus::SINCE_REFUELING) {
                hourValue = mPrivate_IsgAbleTimeHourSinceRefueling;
                minValue = mPrivate_IsgAbleTimeMinSinceRefueling;
                secValue = mPrivate_IsgAbleTimeSecSinceRefueling;
                valueRenewed = true;
            } else {
                // no operation
            }

            if (valueRenewed == true) {
                setSFCISGConstantIsgAbleTimeHourValue(hourValue);
                setSFCISGConstantIsgAbleTimeMinValue(minValue);
                setSFCISGConstantIsgAbleTimeSecValue(secValue);
            }
        }
    }

    void updateSound() {
        SFCISGSoundISGStat stat = SFCISGSoundISGStat::OFF;
        if (mConfigISG == ConfigISG::ON && mIsIgnOn == true && mIsgWarnSoundStatus == IsgWarnSoundStatus::WARN) {
            stat = SFCISGSoundISGStat::ON;
        }
        setSFCISGSoundISGStat(stat);
    }

    void updateEvent() {
        std::string eventID;
        if (mConfigISG == ConfigISG::ON && mIsIgnOn == true) {
            const auto iter = mWarnDispStatIdMap.find(mIsgWarnDispReqStatus);
            if (iter != mWarnDispStatIdMap.end()) {
                eventID = iter->second;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ISG.Event.ISG.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCISGEventISGStat(SFCISGEventISGStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCISGEventISGID(eventID);
            setSFCISGEventISGStat(SFCISGEventISGStat::ON);
        }
    }

    void innerUpdateTelltale() {
        SFCISGTelltaleISGStat stat = SFCISGTelltaleISGStat::OFF;
        SFCISGTelltaleISGBlinkValueA statA = SFCISGTelltaleISGBlinkValueA::OFF;
        SFCISGTelltaleISGBlinkValueB statB = SFCISGTelltaleISGBlinkValueB::OFF;
        bool isGreenOff = false;
        if (mConfigISG == ConfigISG::ON && mIgnElapsed == IgnElapsed::ON_3000ms) {
            if (mEngineISGStatus1 == EngineISGStatus1::AUTO_STOP &&
                (mEngineISGStatus2 == EngineISGStatus2::OFF || mEngineISGStatus2 == EngineISGStatus2::BAT_DISABLE)) {
                stat = SFCISGTelltaleISGStat::GREEN;
            } else if (mEngineISGStatus1 == EngineISGStatus1::AUTO_RESTART && mEngineISGStatus2 == EngineISGStatus2::OFF) {
                stat = SFCISGTelltaleISGStat::BLINK2;
                statA = SFCISGTelltaleISGBlinkValueA::GREEN;
                statB = SFCISGTelltaleISGBlinkValueB::OFF;
                isGreenOff = true;
            } else if (mEngineISGStatus1 == EngineISGStatus1::SYS_ERROR && mEngineISGStatus2 == EngineISGStatus2::OFF) {
                stat = SFCISGTelltaleISGStat::YELLOW;
            } else if ((mEngineISGStatus1 == EngineISGStatus1::ISG_PROHIBIT && mEngineISGStatus2 == EngineISGStatus2::OFF) ||
                       (mEngineISGStatus1 == EngineISGStatus1::OFF && mEngineISGStatus2 != EngineISGStatus2::TEST_MODE)) {
                stat = SFCISGTelltaleISGStat::WHITE;
            } else if (mEngineISGStatus1 == EngineISGStatus1::OFF && mEngineISGStatus2 == EngineISGStatus2::TEST_MODE) {
                stat = SFCISGTelltaleISGStat::BLINK2;
                statA = SFCISGTelltaleISGBlinkValueA::GREEN;
                statB = SFCISGTelltaleISGBlinkValueB::YELLOW;
            } else {
                // no operation
            }
        }
        setSFCISGTelltaleISGStat(stat);
        setSFCISGTelltaleISGBlinkValueA(statA);
        setSFCISGTelltaleISGBlinkValueB(statB);
        mIsTelltaleBlink2GreenOff = isGreenOff;
    }

    void onOneShotTimerTelltaleCallback() {
        innerUpdateTelltale();
        flush();
    }

    inline const bool isTelltaleBlink2GreenOff() {
        return (mConfigISG == ConfigISG::ON && mIgnElapsed == IgnElapsed::ON_3000ms &&
                mEngineISGStatus1 == EngineISGStatus1::AUTO_RESTART && mEngineISGStatus2 == EngineISGStatus2::OFF);
    }

    // EXNCP-29202 에서, Blink2 Green, Off 이더라도 Ign Off일때는 Off로 전환하도록 회신 받음
    void requestUpdateTelltale() {
        if (isTelltaleBlink2GreenOff() == true) {
            setSFCISGTelltaleISGStat(SFCISGTelltaleISGStat::BLINK2);
            setSFCISGTelltaleISGBlinkValueA(SFCISGTelltaleISGBlinkValueA::GREEN);
            setSFCISGTelltaleISGBlinkValueB(SFCISGTelltaleISGBlinkValueB::OFF);
            if (mIsTelltaleBlink2GreenOff == false) {
                mIsTelltaleBlink2GreenOff = true;
                // Green 된 후 1회만 5초 대기 설정
                if (mOneShotTimerTelltale.isRunning() == false) {
                    if (mOneShotTimerTelltale.start() == false) {
                        DWarning() << "Timer mOneShotTimerTelltale starting failed in runISGAbleTimer()";
                    }
                }
            }
        } else if (mOneShotTimerTelltale.isRunning() == true) {
            if (mConfigISG == ConfigISG::ON && mIgnElapsed == IgnElapsed::ON_3000ms) {
                // 5초 타이머 콜백이 알아서 동작함.
                // no operation
            } else {  // 타이머 동작중이나 Ign이나 ConfigISG 조건에 변화생겨 OFF로 처리.
                mOneShotTimerTelltale.stop();
                setSFCISGTelltaleISGStat(SFCISGTelltaleISGStat::OFF);
                setSFCISGTelltaleISGBlinkValueA(SFCISGTelltaleISGBlinkValueA::OFF);
                setSFCISGTelltaleISGBlinkValueB(SFCISGTelltaleISGBlinkValueB::OFF);
                mIsTelltaleBlink2GreenOff = false;
            }
        } else {  // 타이머 동작도, green 동작조건도 아님.
            innerUpdateTelltale();
        }
    }

    inline void calTimeVal(HUInt64& hourValue, HUInt64& minValue, HUInt64& secValue) {
        secValue++;
        if (secValue > 59) {
            secValue = 0;
            minValue++;
            if (minValue > 59) {
                minValue = 0;
                hourValue++;
            }
        }
    }

    void onRepeatTimerISGAbleTimeCallback() {
        calTimeVal(mPrivate_IsgAbleTimeHourValue, mPrivate_IsgAbleTimeMinValue, mPrivate_IsgAbleTimeSecValue);
        // 불필요한 updateConstantIsgAbleTime()의 타이머 콜백 내 호출과 flush를 줄이기 위함
        if (checkIsgAbleTimeUpdateCondition(AutoStopTimerSetStatus::MANUAL_RESET) == true) {
            updateConstantIsgAbleTime();
            flush();
        }
    }

    void onRepeatTimerISGAbleTimeCurrentTripCallback() {
        calTimeVal(mPrivate_IsgAbleTimeHourCurrentTrip, mPrivate_IsgAbleTimeMinCurrentTrip, mPrivate_IsgAbleTimeSecCurrentTrip);
        if (checkIsgAbleTimeUpdateCondition(AutoStopTimerSetStatus::CURRENT_TRIP) == true) {
            updateConstantIsgAbleTime();
            flush();
        }
    }

    void onRepeatTimerISGAbleTimeSinceResetCallback() {
        calTimeVal(mPrivate_IsgAbleTimeHourSinceReset, mPrivate_IsgAbleTimeMinSinceReset, mPrivate_IsgAbleTimeSecSinceReset);
        if (checkIsgAbleTimeUpdateCondition(AutoStopTimerSetStatus::SINCE_RESET) == true) {
            updateConstantIsgAbleTime();
            flush();
        }
    }

    void onRepeatTimerISGAbleTimeSinceRefuelingCallback() {
        calTimeVal(mPrivate_IsgAbleTimeHourSinceRefueling, mPrivate_IsgAbleTimeMinSinceRefueling,
                   mPrivate_IsgAbleTimeSecSinceRefueling);
        if (checkIsgAbleTimeUpdateCondition(AutoStopTimerSetStatus::SINCE_REFUELING) == true) {
            updateConstantIsgAbleTime();
            flush();
        }
    }

    void runIsgAllTimer() {
        requestStartISGAbleTimeTimer();
        requestStartISGAbleCurrentTripTimer();
        requestStartISGAbleSinceResetTimer();
        requestStartISGAbleSinceRefuelingTimer();
    }

    void requestStartISGAbleTimeTimer(const HBool& resetRequest = false) {
        runISGAbleTimer(mRepeatTimerISGAbleTime, "mRepeatTimerISGAbleTime", resetRequest);
    }

    void requestStartISGAbleCurrentTripTimer(const HBool& resetRequest = false) {
        runISGAbleTimer(mRepeatTimerISGAbleTimeCurrentTrip, "mRepeatTimerISGAbleTimeCurrentTrip", resetRequest);
    }

    void requestStartISGAbleSinceResetTimer(const HBool& resetRequest = false) {
        runISGAbleTimer(mRepeatTimerISGAbleTimeSinceReset, "mRepeatTimerISGAbleTimeSinceReset", resetRequest);
    }

    void requestStartISGAbleSinceRefuelingTimer(const HBool& resetRequest = false) {
        runISGAbleTimer(mRepeatTimerISGAbleTimeSinceRefueling, "mRepeatTimerISGAbleTimeSinceRefueling", resetRequest);
    }

    void resetISGAbleTime() {
        if (calResetISGTimeValue(mPrevAutoStopTimeManualResetReq, mAutoStopTimeManualResetReq, mPrivate_IsgAbleTimeHourValue,
                                 mPrivate_IsgAbleTimeMinValue, mPrivate_IsgAbleTimeSecValue) == true) {
            requestStartISGAbleTimeTimer(true);
        }
    }

    void resetISGCurrentTrip() {
        if (calResetISGTimeValue(mPrevDriveInfoResetReq, mDriveInfoResetReq, mPrivate_IsgAbleTimeHourCurrentTrip,
                                 mPrivate_IsgAbleTimeMinCurrentTrip, mPrivate_IsgAbleTimeSecCurrentTrip) == true) {
            requestStartISGAbleCurrentTripTimer(true);
        }
    }

    void resetISGSinceReset() {
        if (calResetISGTimeValue(mPrevAccumInfoResetReq, mAccumInfoResetReq, mPrivate_IsgAbleTimeHourSinceReset,
                                 mPrivate_IsgAbleTimeMinSinceReset, mPrivate_IsgAbleTimeSecSinceReset) == true) {
            requestStartISGAbleSinceResetTimer(true);
        }
    }

    void resetISGSinceRefueling() {
        if (calResetISGTimeValue(mPrevAfterRefuelInfoResetReq, mAfterRefuelInfoResetReq, mPrivate_IsgAbleTimeHourSinceRefueling,
                                 mPrivate_IsgAbleTimeMinSinceRefueling, mPrivate_IsgAbleTimeSecSinceRefueling) == true) {
            requestStartISGAbleSinceRefuelingTimer(true);
        }
    }

    template <typename T1>
    void runISGAbleTimer(T1& timerInstance, const std::string& timerNameToPutLog, const HBool& resetRequest = false) {
        if (mConfigISG == ConfigISG::ON && mEngineISGStatus1 == EngineISGStatus1::AUTO_STOP &&
            mEngineISGStatus2 == EngineISGStatus2::OFF) {
            // 타이머 구동조건이면 중복되지 않게 타이머 실행, 구동조건일때 이미 구동중이면 별도 연산 안해도 됨.
            if (timerInstance.isRunning() == false) {
                if (timerInstance.start() == false) {
                    DWarning() << "Timer " << timerNameToPutLog << " starting failed in runISGAbleTimer()";
                }
            } else if (resetRequest == true && timerInstance.isRunning() == true) {
                // 각 항목별 리셋요청 받았을 경우 타이머도 0초부터 다시 시작한다.
                if (timerInstance.restart() == false) {
                    DWarning() << "Timer " << timerNameToPutLog << " restarting failed in runISGAbleTimer() with resetRequest";
                }
            } else {
                // no operation
            }
        } else {
            timerInstance.stop();
        }
    }

    //불필요한 updateConstantIsgAbleTime()의 타이머 콜백 내 호출을 줄이기 위함
    template <typename T2>
    HBool checkIsgAbleTimeUpdateCondition(const T2& compVal) {
        return (mConfigISG == ConfigISG::ON && mAutoStopTimerSetStatus == compVal);
    }

    template <typename T3>
    inline HBool calResetISGTimeValue(const T3& prevResetReq, const T3& resetReq, HUInt64& hourValue, HUInt64& minValue,
                                      HUInt64& secValue) {
        if (prevResetReq == T3::DEFAULT && resetReq == T3::RESET_REQ) {
            hourValue = 0;
            minValue = 0;
            secValue = 0;
            return true;
        }
        return false;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_3000ms,
        OFF_0ms,
        MAX
    };

    static constexpr uint32_t kTimerISGAbleTimeInterval1s = 1000u;
    static constexpr uint32_t kTimerISGTelltaleInterval5s = 5000u;

    using ConfigISG = ArgumentsConfigIsgChanged::Inter_ConfigISG;
    using EngineISGStatus1 = ArgumentsEngineISGStatChanged::Input_EngineISGStatus1;
    using EngineISGStatus2 = ArgumentsEngineISGStatChanged::Input_EngineISGStatus2;
    using IsgWarnSoundStatus = ArgumentsIsgWarnSoundStatChanged::Input_IsgWarnSoundStatus;
    using IsgOperStatDispReqStatus = ArgumentsIsgOperStatDispReqStatChanged::Input_IsgOperStatDispReqStatus;
    using IsgWarnDispReqStatus = ArgumentsIsgWarnDispReqStatChanged::Input_IsgWarnDispReqStatus;
    using AutoStopTimeManualResetReq = ArgumentsIsgAutoStopTimeManualResetReqChanged::Inter_AutoStopTimeManualResetReq;
    using ConfigPTLogicControlType = ArgumentsPtLogicControlType::Inter_PTLogicControlType;
    using AccumInfoResetReq = ArgumentsAccumResetReqChanged::Inter_AccumInfoResetReq;
    using AfterRefuelInfoResetReq = ArgumentsAfterRefuelInfoResetReqChanged::Inter_AfterRefuelInfoResetReq;
    using DriveInfoResetReq = ArgumentsDriveInfoResetReqChanged::Inter_DriveInfoResetReq;
    using AutoStopTimerSetStatus = ArgumentsAutoStopTimerSetStatusChanged::Inter_AutoStopTimerSetStatus;

    HBool mAEMInitializeComplete = false;
    HBool mIsIgnOn = false;
    HBool mIsTelltaleBlink2GreenOff = false;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigISG mConfigISG = ConfigISG::OFF;
    ConfigPTLogicControlType mConfigPTLogicControlType = ConfigPTLogicControlType::OFF;
    EngineISGStatus1 mEngineISGStatus1 = EngineISGStatus1::OFF;
    EngineISGStatus2 mEngineISGStatus2 = EngineISGStatus2::OFF;
    IsgWarnSoundStatus mIsgWarnSoundStatus = IsgWarnSoundStatus::OFF;
    IsgOperStatDispReqStatus mIsgOperStatDispReqStatus = IsgOperStatDispReqStatus::OFF;
    IsgWarnDispReqStatus mIsgWarnDispReqStatus = IsgWarnDispReqStatus::OFF;
    AutoStopTimeManualResetReq mAutoStopTimeManualResetReq = AutoStopTimeManualResetReq::NONE;
    AutoStopTimeManualResetReq mPrevAutoStopTimeManualResetReq = AutoStopTimeManualResetReq::NONE;
    AccumInfoResetReq mAccumInfoResetReq = AccumInfoResetReq::NONE;
    AccumInfoResetReq mPrevAccumInfoResetReq = AccumInfoResetReq::NONE;
    AfterRefuelInfoResetReq mAfterRefuelInfoResetReq = AfterRefuelInfoResetReq::NONE;
    AfterRefuelInfoResetReq mPrevAfterRefuelInfoResetReq = AfterRefuelInfoResetReq::NONE;
    DriveInfoResetReq mDriveInfoResetReq = DriveInfoResetReq::NONE;
    DriveInfoResetReq mPrevDriveInfoResetReq = DriveInfoResetReq::NONE;
    AutoStopTimerSetStatus mAutoStopTimerSetStatus =
        AutoStopTimerSetStatus::MANUAL_RESET;  // Settings[CD140] v0.18 사양서 기준 P318, Auto Stop 설정 기본값은 MANUAL_RESET

    std::map<IsgOperStatDispReqStatus, SFCISGConstantIsgDisableCondStat> mISGOperStatMap = {
        {IsgOperStatDispReqStatus::NO_ISG_ENGINE_CONTROL, SFCISGConstantIsgDisableCondStat::DISABLED_ENGINE_CONTROL},
        {IsgOperStatDispReqStatus::NO_ISG_BATTERY_CHARGE, SFCISGConstantIsgDisableCondStat::DISABLED_BATTERY_CHARGING},
        {IsgOperStatDispReqStatus::NO_ISG_STEEP_ROADWAY, SFCISGConstantIsgDisableCondStat::DISABLED_STEEP_ROADWAY},
        {IsgOperStatDispReqStatus::NO_ISG_TEMPERATURE, SFCISGConstantIsgDisableCondStat::DISABLED_TEMPERATURE},
        {IsgOperStatDispReqStatus::NO_ISG_DRIVEMODE, SFCISGConstantIsgDisableCondStat::DISABLED_DRIVEMODE},
        {IsgOperStatDispReqStatus::NO_ISG_HOOD_OPEN, SFCISGConstantIsgDisableCondStat::DISABLED_HOOD_OPEN},
        {IsgOperStatDispReqStatus::NO_ISG_DOOR_OPEN, SFCISGConstantIsgDisableCondStat::DISABLED_DOOR_OPEN},
        {IsgOperStatDispReqStatus::NO_ISG_SEATBELT, SFCISGConstantIsgDisableCondStat::DISABLED_SEATBELT_UNFASTENED},
        {IsgOperStatDispReqStatus::NO_ISG_WHEEL_ALIGN, SFCISGConstantIsgDisableCondStat::DISABLED_WHEEL_UNALIGNED},
        {IsgOperStatDispReqStatus::NO_ISG_LOW_SPEED, SFCISGConstantIsgDisableCondStat::DISABLED_LOW_SPEED},
        {IsgOperStatDispReqStatus::NO_ISG_GEAR_POSITION_D_N, SFCISGConstantIsgDisableCondStat::DISABLED_GEAR_POSITION_AT},
        {IsgOperStatDispReqStatus::NO_ISG_GEAR_POSITION_N, SFCISGConstantIsgDisableCondStat::DISABLED_GEAR_POSITION_MT},
        {IsgOperStatDispReqStatus::NO_ISG_PRESS_BRAKE, SFCISGConstantIsgDisableCondStat::DISABLED_PRESS_BRAKE},
        {IsgOperStatDispReqStatus::NO_ISG_TCU_CONTROL, SFCISGConstantIsgDisableCondStat::DISABLED_TRANSMISSION_MODE},
        {IsgOperStatDispReqStatus::AUTO_START_VEHICLE_AHEAD, SFCISGConstantIsgDisableCondStat::DISABLED_AUTO_START},
        {IsgOperStatDispReqStatus::NO_ISG_STOP_SIGN, SFCISGConstantIsgDisableCondStat::DISABLED_STOP_SIGN},
        {IsgOperStatDispReqStatus::NO_ISG_YIELD_SIGN, SFCISGConstantIsgDisableCondStat::DISABLED_YIELD_SIGN},
        {IsgOperStatDispReqStatus::NO_ISG_ROUNDABOUT_SIGN, SFCISGConstantIsgDisableCondStat::DISABLED_ROUNDABOUT_SIGN},
        {IsgOperStatDispReqStatus::NO_ISG_WAITING_TURN, SFCISGConstantIsgDisableCondStat::DISABLED_WAITING_TURN},
        {IsgOperStatDispReqStatus::NO_ISG_SPEEDSIGN, SFCISGConstantIsgDisableCondStat::DISABLED_SPEED_SIGN}};

    std::map<IsgWarnDispReqStatus, std::string> mWarnDispStatIdMap = {{IsgWarnDispReqStatus::START_ON_P, "E25001"},
                                                                      {IsgWarnDispReqStatus::START_ON_CLUTCH, "E25002"},
                                                                      {IsgWarnDispReqStatus::START_ON_BRAKE_CLUTCH, "E25008"},
                                                                      {IsgWarnDispReqStatus::START_ON_BRAKE, "E25009"},
                                                                      {IsgWarnDispReqStatus::PRESS_BRAKE, "E25004"},
                                                                      {IsgWarnDispReqStatus::SHIFT_TO_NEUTRAL, "E25005"},
                                                                      {IsgWarnDispReqStatus::RELEASE_BRAKE, "E25003"},
                                                                      {IsgWarnDispReqStatus::PRESS_CLUTCH, "E25006"}};

    std::map<AutoStopTimerSetStatus, SFCISGConstantIsgPageDisplayStat> mIsgPageMap = {
        {AutoStopTimerSetStatus::MANUAL_RESET, SFCISGConstantIsgPageDisplayStat::MANUAL_RESET},
        {AutoStopTimerSetStatus::CURRENT_TRIP, SFCISGConstantIsgPageDisplayStat::CURRENT_TRIP},
        {AutoStopTimerSetStatus::SINCE_RESET, SFCISGConstantIsgPageDisplayStat::SINCE_RESET},
        {AutoStopTimerSetStatus::SINCE_REFUELING, SFCISGConstantIsgPageDisplayStat::SINCE_REFUELING},
    };

    HUInt64 mPrivate_IsgAbleTimeHourValue = 0;
    HUInt64 mPrivate_IsgAbleTimeMinValue = 0;
    HUInt64 mPrivate_IsgAbleTimeSecValue = 0;
    HUInt64 mPrivate_IsgAbleTimeHourCurrentTrip = 0;
    HUInt64 mPrivate_IsgAbleTimeMinCurrentTrip = 0;
    HUInt64 mPrivate_IsgAbleTimeSecCurrentTrip = 0;
    HUInt64 mPrivate_IsgAbleTimeHourSinceReset = 0;
    HUInt64 mPrivate_IsgAbleTimeMinSinceReset = 0;
    HUInt64 mPrivate_IsgAbleTimeSecSinceReset = 0;
    HUInt64 mPrivate_IsgAbleTimeHourSinceRefueling = 0;
    HUInt64 mPrivate_IsgAbleTimeMinSinceRefueling = 0;
    HUInt64 mPrivate_IsgAbleTimeSecSinceRefueling = 0;
    HUInt64 mIsgAbleTimeHourValue = 0;
    HUInt64 mIsgAbleTimeMinValue = 0;
    HUInt64 mIsgAbleTimeSecValue = 0;

    ssfs::SFCTimer<ISG> mRepeatTimerISGAbleTime;
    ssfs::SFCTimer<ISG> mRepeatTimerISGAbleTimeCurrentTrip;
    ssfs::SFCTimer<ISG> mRepeatTimerISGAbleTimeSinceReset;
    ssfs::SFCTimer<ISG> mRepeatTimerISGAbleTimeSinceRefueling;
    ssfs::SFCTimer<ISG> mOneShotTimerTelltale;
};

}  // namespace ccos

#endif  // SFSS_ISG_H
