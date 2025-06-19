/**
 * @file WIPER_LIGHT_Status.hpp
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
#ifndef SFSS_WIPER_LIGHT_Status_H
#define SFSS_WIPER_LIGHT_Status_H

#define DLOG_ENABLED gEnableSFCLog

#include "WIPER_LIGHT_StatusBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.24
// Reference : [CV570] WIPER LIGHT Status. Contains : Constant, Event. Recommend: VALUE_CHANGED
class WIPER_LIGHT_Status : public WIPER_LIGHT_StatusBase {
public:
    WIPER_LIGHT_Status() = default;
    ~WIPER_LIGHT_Status() override = default;
    WIPER_LIGHT_Status(const WIPER_LIGHT_Status& other) = delete;
    WIPER_LIGHT_Status(WIPER_LIGHT_Status&& other) noexcept = delete;
    WIPER_LIGHT_Status& operator=(const WIPER_LIGHT_Status& other) = delete;
    WIPER_LIGHT_Status& operator=(WIPER_LIGHT_Status&& other) noexcept = delete;

    void onInitialize() override {
        setSFCWIPER_LIGHT_StatusEventLightSwitchControlID("E75701");
        setSFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlID("E75702");
        setSFCWIPER_LIGHT_StatusEventLowWasherWarnID("E75703");
        setSFCWIPER_LIGHT_StatusEventLowWasherWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCWIPER_LIGHT_StatusEventLowWasherWarnLinkedSoundType(
            SFCWIPER_LIGHT_StatusEventLowWasherWarnLinkedSoundType::REPEAT_COUNT);
        setSFCWIPER_LIGHT_StatusEventLowWasherWarnLinkedSoundRepeatCount(1);

        if (mOneShotTimerLowWasherWarnStatusCV10sTimeout.create(
                kTimerInterval10s, this, &WIPER_LIGHT_Status::onOneShotTimerLowWasherWarnStatusCV10sTimeout, false) == false) {
            DWarning() << "[WIPER_LIGHT_Status] mOneShotTimerLowWasherWarnStatusCV10sTimeout Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn0ms) == true) {
            mIsIgnOn = true;
            updateConstantLightSwitchPopupTypeStat();
            updateConstantWiperFrSwitchPopupTypeStat();
            updateConstantLightSwitchSelectStat();
            updateConstantWiperFrontSwitchSelectStat();
            updateConstantWiperIntSwitchSelectStat();
            updateEventLightSwitchControlStat();
            updateEventFrontWiperSwitchControlStat();
            updateEventLowWasherWarn();
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff0ms) == true) {
            mIsIgnOn = false;
            updateConstantLightSwitchPopupTypeStat();
            updateConstantLightSwitchSelectStat();
            updateConstantWiperFrontSwitchSelectStat();
            updateConstantWiperIntSwitchSelectStat();
            updateEventLightSwitchControlStat();
            updateEventFrontWiperSwitchControlStat();
            updateEventLowWasherWarn();
        }
    }

    void onConfigAutolightChanged(const ArgumentsConfigAutolightChanged& args) {
        if (updateValueIfValid(mConfigAutolight, args.mInter_ConfigAutolight) == true) {
            updateConstantLightSwitchPopupTypeStat();
        }
    }

    void onLightSwitchStatusFromMFSWChanged(const ArgumentsLightSwitchStatusFromMFSWChanged& args) {
        if (updateValueIfValid(mLightSwitchStatusFromMFSW, args.mInput_LightSwitchStatusFromMFSW) == true) {
            updateConstantLightSwitchSelectStat();
            updateEventLightSwitchControlStat();
        }
    }

    void onWiperSwitchFrontStatusFromMFSWChanged(const ArgumentsWiperSwitchFrontStatusFromMFSWChanged& args) {
        if (updateValueIfValid(mWiperSwitchFrontStatusFromMFSW, args.mInput_WiperSwitchFrontStatusFromMFSW) == true) {
            updateConstantWiperFrontSwitchSelectStat();
            updateConstantWiperIntSwitchSelectStat();
            updateEventFrontWiperSwitchControlStat();
        }
    }

    void onWiperSwitchIntStepStatusFromMFSWChanged(const ArgumentsWiperSwitchIntStepStatusFromMFSWChanged& args) {
        if (updateValueIfValid(mWiperSwitchIntStepStatusFromMFSW, args.mInput_WiperSwitchIntStepStatusFromMFSW) == true) {
            updateConstantWiperIntSwitchSelectStat();
            updateEventFrontWiperSwitchControlStat();
        }
    }

    void onLowWasherWarnStatusCVChanged(const ArgumentsLowWasherWarnStatusCVChanged& args) {
        if (updateValueIfValid(mLowWasherWarnStatusCV, args.mInput_LowWasherWarnStatusCV) == true) {
            updateEventLowWasherWarn();
        }
    }

    void onMfswPopupSetStatusChanged(const ArgumentsMfswPopupSetStatusChanged& args) {
        if (updateValueIfValid(mMfswPopupSetStatus, args.mInter_MfswPopupSetStatus) == true) {
            updateEventLightSwitchControlStat();
            updateEventFrontWiperSwitchControlStat();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.2.1 라이트 스위치 리스트 타입
    void updateConstantLightSwitchPopupTypeStat() {
        SFCWIPER_LIGHT_StatusConstantLightSwitchPopupTypeStat stat =
            SFCWIPER_LIGHT_StatusConstantLightSwitchPopupTypeStat::AUTO_TEXT;
        if (mIsIgnOn == true && mConfigAutolight == ConfigAutolight::OFF) {
            stat = SFCWIPER_LIGHT_StatusConstantLightSwitchPopupTypeStat::NON_AUTO_TEXT;
        }
        setSFCWIPER_LIGHT_StatusConstantLightSwitchPopupTypeStat(stat);
    }

    // 5.2.2 앞유리 와이퍼 스위치 리스트 타입
    void updateConstantWiperFrSwitchPopupTypeStat() {
        if (mIsIgnOn == true) {
            setSFCWIPER_LIGHT_StatusConstantWiperFrSwitchPopupTypeStat(
                SFCWIPER_LIGHT_StatusConstantWiperFrSwitchPopupTypeStat::NON_AUTO_UP_TEXT);
        }
    }

    // 5.2.3 라이트 스위치 선택 상태
    void updateConstantLightSwitchSelectStat() {
        SFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat stat = SFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat::OFF;
        if (mIsIgnOn == true) {
            switch (mLightSwitchStatusFromMFSW) {
                case LightSwitchStatusFromMFSW::TAIL_ON:
                    stat = SFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat::TAIL_ON;
                    break;
                case LightSwitchStatusFromMFSW::HEAD_LOW_ON:
                    stat = SFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat::HEAD_LOW_ON;
                    break;
                case LightSwitchStatusFromMFSW::AUTO_ON:
                    stat = SFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat::AUTO_ON;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCWIPER_LIGHT_StatusConstantLightSwitchSelectStat(stat);
    }

    // 5.2.4 앞유리 와이퍼 스위치 선택 상태
    void updateConstantWiperFrontSwitchSelectStat() {
        SFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat stat =
            SFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat::OFF;
        if (mIsIgnOn == true) {
            switch (mWiperSwitchFrontStatusFromMFSW) {
                case WiperSwitchFrontStatusFromMFSW::INT_OR_AUTO:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat::INT_OR_AUTO;
                    break;
                case WiperSwitchFrontStatusFromMFSW::LOW_ON:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat::LOW_ON;
                    break;
                case WiperSwitchFrontStatusFromMFSW::HIGH_ON:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat::HIGH_ON;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCWIPER_LIGHT_StatusConstantWiperFrontSwitchSelectStat(stat);
    }

    // 5.2.5 와이퍼 INT/AUTO 스위치 선택 상태
    void updateConstantWiperIntSwitchSelectStat() {
        SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat stat =
            SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::DISPLAY_OFF;
        if (mIsIgnOn == true && mWiperSwitchFrontStatusFromMFSW == WiperSwitchFrontStatusFromMFSW::INT_OR_AUTO) {
            switch (mWiperSwitchIntStepStatusFromMFSW) {
                case WiperSwitchIntStepStatusFromMFSW::STEP1:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::STEP1;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP2:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::STEP2;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP3:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::STEP3;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP4:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::STEP4;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP5:
                    stat = SFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat::STEP5;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCWIPER_LIGHT_StatusConstantWiperIntSwitchSelectStat(stat);
    }

    // 5.3.1 라이트 스위치 조작
    void updateEventLightSwitchControlStat() {
        SFCWIPER_LIGHT_StatusEventLightSwitchControlStat stat = SFCWIPER_LIGHT_StatusEventLightSwitchControlStat::OFF;
        if (mIsIgnOn == true && mMfswPopupSetStatus == MfswPopupSetStatus::ON &&
            mLightSwitchStatusFromMFSW != mPrevLightSwitchStatusFromMFSW) {
            stat = SFCWIPER_LIGHT_StatusEventLightSwitchControlStat::ON;
        }
        setSFCWIPER_LIGHT_StatusEventLightSwitchControlStat(stat);
        mPrevLightSwitchStatusFromMFSW = mLightSwitchStatusFromMFSW;
    }

    // 5.3.2 앞유리 와이퍼 스위치 조작
    void updateEventFrontWiperSwitchControlStat() {
        SFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlStat stat = SFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlStat::OFF;
        if (mIsIgnOn == true && mMfswPopupSetStatus == MfswPopupSetStatus::ON) {
            if (mWiperSwitchFrontStatusFromMFSW != mPrevWiperSwitchFrontStatusFromMFSW) {
                stat = SFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlStat::ON;
            } else if (mWiperSwitchFrontStatusFromMFSW == WiperSwitchFrontStatusFromMFSW::INT_OR_AUTO &&
                       mWiperSwitchIntStepStatusFromMFSW != mPrevWiperSwitchIntStepStatusFromMFSW) {
                stat = SFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlStat::ON;
            } else {
                // no operation
            }
        }
        setSFCWIPER_LIGHT_StatusEventFrontWiperSwitchControlStat(stat);
        mPrevWiperSwitchFrontStatusFromMFSW = mWiperSwitchFrontStatusFromMFSW;
        mPrevWiperSwitchIntStepStatusFromMFSW = mWiperSwitchIntStepStatusFromMFSW;
    }

    // 5.3.3 워셔액 부족 알림
    void updateEventLowWasherWarn() {
        SFCWIPER_LIGHT_StatusEventLowWasherWarnStat stat = SFCWIPER_LIGHT_StatusEventLowWasherWarnStat::OFF;
        if (mIsIgnOn == true && mLowWasherWarnStatusCV == LowWasherWarnStatusCV::ON) {
            if (mOneShotTimerLowWasherWarnStatusCV10sTimeout.start() == false) {
                DWarning() << "[WIPER_LIGHT_Status] mOneShotTimerLowWasherWarnStatusCV10sTimeout Start Failed";
            }
        } else {
            mOneShotTimerLowWasherWarnStatusCV10sTimeout.stop();
            setSFCWIPER_LIGHT_StatusEventLowWasherWarnStat(stat);
        }
    }

    void onOneShotTimerLowWasherWarnStatusCV10sTimeout() {
        if (mIsIgnOn == true && mLowWasherWarnStatusCV == LowWasherWarnStatusCV::ON) {
            SFCWIPER_LIGHT_StatusEventLowWasherWarnStat stat = SFCWIPER_LIGHT_StatusEventLowWasherWarnStat::ON;

            setSFCWIPER_LIGHT_StatusEventLowWasherWarnStat(stat);
            flush();
        }
    }

    using ConfigAutolight = ArgumentsConfigAutolightChanged::Inter_ConfigAutolight;
    using LightSwitchStatusFromMFSW = ArgumentsLightSwitchStatusFromMFSWChanged::Input_LightSwitchStatusFromMFSW;
    using WiperSwitchFrontStatusFromMFSW = ArgumentsWiperSwitchFrontStatusFromMFSWChanged::Input_WiperSwitchFrontStatusFromMFSW;
    using WiperSwitchIntStepStatusFromMFSW =
        ArgumentsWiperSwitchIntStepStatusFromMFSWChanged::Input_WiperSwitchIntStepStatusFromMFSW;
    using LowWasherWarnStatusCV = ArgumentsLowWasherWarnStatusCVChanged::Input_LowWasherWarnStatusCV;
    using MfswPopupSetStatus = ArgumentsMfswPopupSetStatusChanged::Inter_MfswPopupSetStatus;

    static constexpr uint32_t kTimerInterval10s = 10000;

    HBool mIsIgnOn = false;
    ConfigAutolight mConfigAutolight = ConfigAutolight::OFF;
    LightSwitchStatusFromMFSW mLightSwitchStatusFromMFSW = LightSwitchStatusFromMFSW::OFF;
    WiperSwitchFrontStatusFromMFSW mWiperSwitchFrontStatusFromMFSW = WiperSwitchFrontStatusFromMFSW::OFF;
    WiperSwitchIntStepStatusFromMFSW mWiperSwitchIntStepStatusFromMFSW = WiperSwitchIntStepStatusFromMFSW::OFF;
    LowWasherWarnStatusCV mLowWasherWarnStatusCV = LowWasherWarnStatusCV::OFF;
    MfswPopupSetStatus mMfswPopupSetStatus = MfswPopupSetStatus::OFF;

    LightSwitchStatusFromMFSW mPrevLightSwitchStatusFromMFSW = LightSwitchStatusFromMFSW::OFF;
    WiperSwitchFrontStatusFromMFSW mPrevWiperSwitchFrontStatusFromMFSW = WiperSwitchFrontStatusFromMFSW::OFF;
    WiperSwitchIntStepStatusFromMFSW mPrevWiperSwitchIntStepStatusFromMFSW = WiperSwitchIntStepStatusFromMFSW::OFF;

    ssfs::SFCTimer<WIPER_LIGHT_Status> mOneShotTimerLowWasherWarnStatusCV10sTimeout;
};

}  // namespace ccos

#endif  // SFSS_WIPER_LIGHT_Status_H
