/**
 * @file MFSW.hpp
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
#ifndef SFSS_MFSW_H
#define SFSS_MFSW_H

#define DLOG_ENABLED gEnableSFCLog

#include "MFSWBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 7.1.0
// Reference : [EC110] MFSW. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class MFSW : public MFSWBase {
public:
    MFSW() = default;
    ~MFSW() override = default;
    MFSW(const MFSW& other) = delete;
    MFSW(MFSW&& other) noexcept = delete;
    MFSW& operator=(const MFSW& other) = delete;
    MFSW& operator=(MFSW&& other) noexcept = delete;

    void onInitialize() override {
        setSFCMFSWEventLightSwitchID("E60901");
        setSFCMFSWEventFrontWiperSwitchID("E60902");
        setSFCMFSWEventRearWiperSwitchID("E60903");
        setSFCMFSWEventHeadLampOffAlarmLinkedSoundID("SND_PopUpInform2");
        setSFCMFSWEventHeadLampOffAlarmLinkedSoundType(SFCMFSWEventHeadLampOffAlarmLinkedSoundType::REPEAT_COUNT);
        setSFCMFSWEventHeadLampOffAlarmLinkedSoundRepeatCount(1);
        if (mOneShotTimerHeadLampOff10sStatus.create(kTimerHeadLampOffInterval10s, this,
                                                     &MFSW::onOneShotTimerHeadLampStatus10sStatusTimeout, false) == false) {
            DWarning() << "[MFSW] mOneShotTimerHeadLampOff10sStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateLightSwitchPopupTypeStat();
        updateLightSwitchSelectStat();
        updateEventLightSW();

        updateWiperFrSwitchPopupTypeStat();
        updateWiperFrontSwitchSelectStat();
        updateWiperIntSwitchSelectStat();
        updateEventFrWiperSW();

        updateWiperRrSwitchPopupTypeStat();
        updateWiperRearSwitchSelectStat();
        updateEventRrWiperSW();

        updateEventHeadLampOffAlarm();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateLightSwitchPopupTypeStat();
        updateLightSwitchSelectStat();
        updateEventLightSW();

        updateWiperFrSwitchPopupTypeStat();
        updateWiperFrontSwitchSelectStat();
        updateWiperIntSwitchSelectStat();
        updateEventFrWiperSW();

        updateWiperRrSwitchPopupTypeStat();
        updateWiperRearSwitchSelectStat();
        updateEventRrWiperSW();

        updateEventHeadLampOffAlarm();
    }

    void onMfswCANTypeAppliedStatusChanged(const ArgumentsMfswCANTypeAppliedStatusChanged& args) {
        mMfswCANTypeAppliedStatus = args.mInput_MfswCANTypeAppliedStatus;

        updatePrivateLightSwStatus();
        updateLightSwitchSelectStat();
        updateEventLightSW();
        updateLightSwitchPopupTypeStat();

        updatePrivateWiperFrSwStatus();
        updatePrivateWiperIntSwStatus();
        updateWiperFrSwitchPopupTypeStat();
        updateWiperRrSwitchPopupTypeStat();
        updateWiperFrontSwitchSelectStat();
        updateWiperIntSwitchSelectStat();
        updateEventFrWiperSW();

        updatePrivateWiperRrSwStatus();
        updateWiperRearSwitchSelectStat();
        updateEventRrWiperSW();
    }

    void onLightSwStatusChanged(const ArgumentsLightSwStatusChanged& args) {
        mLightSwitchStatusFromIBU = args.mInput_LightSwitchStatusFromIBU;
        mLightSwitchStatusFromMFSW = args.mInput_LightSwitchStatusFromMFSW;
        mLightPopUpReq = args.mInput_LightPopUpReq;

        updatePrivateLightSwStatus();
        updateLightSwitchSelectStat();
        updateEventLightSW();
    }

    void onLightPopUpMsgTypeChanged(const ArgumentsLightPopUpMsgTypeChanged& args) {
        mLightPopUpMsgType = args.mInput_LightPopUpMsgType;

        updateLightSwitchPopupTypeStat();
    }

    void onWiperSwFrontStatusChanged(const ArgumentsWiperSwFrontStatusChanged& args) {
        mWiperSwitchFrontStatusFromIBU = args.mInput_WiperSwitchFrontStatusFromIBU;
        mWiperSwitchFrontStatusFromMFSW = args.mInput_WiperSwitchFrontStatusFromMFSW;

        updatePrivateWiperFrSwStatus();
        updateWiperFrontSwitchSelectStat();
        updateWiperIntSwitchSelectStat();
        updateEventFrWiperSW();
    }

    void onWiperSwIntStepStatusChanged(const ArgumentsWiperSwIntStepStatusChanged& args) {
        mWiperSwitchIntStepStatusFromIBU = args.mInput_WiperSwitchIntStepStatusFromIBU;
        mWiperSwitchIntStepStatusFromMFSW = args.mInput_WiperSwitchIntStepStatusFromMFSW;

        updatePrivateWiperIntSwStatus();
        updateWiperIntSwitchSelectStat();
        updateEventFrWiperSW();
    }

    void onWiperSwRearStatusChanged(const ArgumentsWiperSwRearStatusChanged& args) {
        mWiperSwitchRearStatusFromMFSW = args.mInput_WiperSwitchRearStatusFromMFSW;
        mWiperSwitchRearStatusFromIBU = args.mInput_WiperSwitchRearStatusFromIBU;

        updatePrivateWiperRrSwStatus();
        updateWiperRearSwitchSelectStat();
        updateEventRrWiperSW();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigAutolight = args.mInter_ConfigAutolight;
        mConfigAutoWiper = args.mInter_ConfigAutoWiper;
        mConfigMfswSymbol = args.mInter_ConfigMfswSymbol;
        mConfigArea = args.mInter_ConfigArea;

        updateLightSwitchPopupTypeStat();
        updateWiperFrSwitchPopupTypeStat();
        updateWiperRrSwitchPopupTypeStat();
        updateEventHeadLampOffAlarm();
    }

    void onActualVehicleSpeedCompareValueTo10Changed(const ArgumentsActualVehicleSpeedCompareValueTo10Changed& args) {
        mActualVehicleSpeedCompareValueTo10 = args.mInter_ActualVehicleSpeedCompareValueTo10;
        updateEventHeadLampOffAlarm();
    }

    void onHeadLampOffAlarmStatusChanged(const ArgumentsHeadLampOffAlarmStatusChanged& args) {
        mAutolightSensorNightStatus = args.mInput_AutolightSensorNightStatus;
        mExtLowBeamOnReqStatus = args.mInput_ExtLowBeamOnReqStatus;
        mLamp_HdLmpLoOffPopUpReq = args.mInput_Lamp_HdLmpLoOffPopUpReq;
        updateEventHeadLampOffAlarm();
    }

    void onMfswPopupSetStatusChanged(const ArgumentsMfswPopupSetStatusChanged& args) {
        mMfswPopupSetStatus = args.mInter_MfswPopupSetStatus;

        updateEventLightSW();
        updateEventFrWiperSW();
        updateEventRrWiperSW();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventHeadLampOffAlarm() {
        std::string prevEventID;
        std::string eventID;
        SFCMFSWEventHeadLampOffAlarmStat stat = SFCMFSWEventHeadLampOffAlarmStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigArea != ConfigArea::CAN &&
                mActualVehicleSpeedCompareValueTo10 == ActualVehicleSpeedCompareValueTo10::OVER &&
                mAutolightSensorNightStatus == AutolightSensorNightStatus::NIGHT &&
                mPrivateLightSwStatus == PrivateLightSwStatusEnum::OFF && mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::OFF &&
                mLamp_HdLmpLoOffPopUpReq == Lamp_HdLmpLoOffPopUpReq::OLD) {
                if (mOneShotTimerHeadLampOff10sStatus.start() == false) {
                    DWarning() << "[MFSW] mOneShotTimerHeadLampOff10sStatus Start Failed";
                }
            } else if (mLamp_HdLmpLoOffPopUpReq == Lamp_HdLmpLoOffPopUpReq::ON) {
                mOneShotTimerHeadLampOff10sStatus.stop();
                eventID = "E60905";
                stat = SFCMFSWEventHeadLampOffAlarmStat::ON;
            } else {
                mOneShotTimerHeadLampOff10sStatus.stop();
            }
        } else {
            mOneShotTimerHeadLampOff10sStatus.stop();
        }

        GETCACHEDVALUE(SFC.MFSW.Event.HeadLampOffAlarm.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCMFSWEventHeadLampOffAlarmStat(SFCMFSWEventHeadLampOffAlarmStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false && mOneShotTimerHeadLampOff10sStatus.isRunning() == false) {
            setSFCMFSWEventHeadLampOffAlarmID(eventID);
            setSFCMFSWEventHeadLampOffAlarmStat(stat);
        }
    }

    void onOneShotTimerHeadLampStatus10sStatusTimeout() {
        if (mConfigArea != ConfigArea::CAN && mActualVehicleSpeedCompareValueTo10 == ActualVehicleSpeedCompareValueTo10::OVER &&
            mAutolightSensorNightStatus == AutolightSensorNightStatus::NIGHT &&
            mPrivateLightSwStatus == PrivateLightSwStatusEnum::OFF && mIgnElapsed == IgnElapsed::ON_0ms &&
            mExtLowBeamOnReqStatus == ExtLowBeamOnReqStatus::OFF && mLamp_HdLmpLoOffPopUpReq == Lamp_HdLmpLoOffPopUpReq::OLD) {
            setSFCMFSWEventHeadLampOffAlarmID("E60904");
            setSFCMFSWEventHeadLampOffAlarmStat(SFCMFSWEventHeadLampOffAlarmStat::ON);
            flush();
        }
    }

    void updatePrivateLightSwStatus() {
        if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF) {
            switch (mLightSwitchStatusFromIBU) {
                case LightSwitchStatusFromIBU::TAIL_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::TAIL_ON;
                    break;
                case LightSwitchStatusFromIBU::HEAD_LOW_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::HEAD_LOW_ON;
                    break;
                case LightSwitchStatusFromIBU::AUTO_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::AUTO_ON;
                    break;
                case LightSwitchStatusFromIBU::OFF:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
            }
        } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED) {
            switch (mLightSwitchStatusFromMFSW) {
                case LightSwitchStatusFromMFSW::TAIL_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::TAIL_ON;
                    break;
                case LightSwitchStatusFromMFSW::HEAD_LOW_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::HEAD_LOW_ON;
                    break;
                case LightSwitchStatusFromMFSW::AUTO_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::AUTO_ON;
                    break;
                case LightSwitchStatusFromMFSW::OFF:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
            }
        } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
            switch (mLightPopUpReq) {
                case LightPopUpReq::TAIL_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::TAIL_ON;
                    break;
                case LightPopUpReq::HEAD_LOW_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::HEAD_LOW_ON;
                    break;
                case LightPopUpReq::AUTO_ON:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::AUTO_ON;
                    break;
                case LightPopUpReq::OFF:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
                    break;
            }
        } else {
            mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
        }
        updateEventHeadLampOffAlarm();
    }

    void updatePrivateWiperFrSwStatus() {
        if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF) {
            switch (mWiperSwitchFrontStatusFromIBU) {
                case WiperSwitchFrontStatusFromIBU::INT_OR_AUTO:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::INT_OR_AUTO;
                    break;
                case WiperSwitchFrontStatusFromIBU::LOW_ON:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::LOW_ON;
                    break;
                case WiperSwitchFrontStatusFromIBU::HIGH_ON:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::HIGH_ON;
                    break;
                case WiperSwitchFrontStatusFromIBU::OFF:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
                    break;
            }
        } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED ||
                   mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
            switch (mWiperSwitchFrontStatusFromMFSW) {
                case WiperSwitchFrontStatusFromMFSW::INT_OR_AUTO:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::INT_OR_AUTO;
                    break;
                case WiperSwitchFrontStatusFromMFSW::LOW_ON:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::LOW_ON;
                    break;
                case WiperSwitchFrontStatusFromMFSW::HIGH_ON:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::HIGH_ON;
                    break;
                case WiperSwitchFrontStatusFromMFSW::OFF:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
                    break;
            }
        } else {
            mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
        }
    }

    void updatePrivateWiperIntSwStatus() {
        if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF) {
            switch (mWiperSwitchIntStepStatusFromIBU) {
                case WiperSwitchIntStepStatusFromIBU::STEP1:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP1;
                    break;
                case WiperSwitchIntStepStatusFromIBU::STEP2:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP2;
                    break;
                case WiperSwitchIntStepStatusFromIBU::STEP3:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP3;
                    break;
                case WiperSwitchIntStepStatusFromIBU::STEP4:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP4;
                    break;
                case WiperSwitchIntStepStatusFromIBU::STEP5:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP5;
                    break;
                case WiperSwitchIntStepStatusFromIBU::OFF:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
                    break;
            }
        } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED ||
                   mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
            switch (mWiperSwitchIntStepStatusFromMFSW) {
                case WiperSwitchIntStepStatusFromMFSW::STEP1:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP1;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP2:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP2;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP3:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP3;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP4:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP4;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::STEP5:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::STEP5;
                    break;
                case WiperSwitchIntStepStatusFromMFSW::OFF:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
                    break;
            }
        } else {
            mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
        }
    }

    void updatePrivateWiperRrSwStatus() {
        if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF) {
            switch (mWiperSwitchRearStatusFromIBU) {
                case WiperSwitchRearStatusFromIBU::LOW_ON:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::LOW_ON;
                    break;
                case WiperSwitchRearStatusFromIBU::HIGH_ON:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::HIGH_ON;
                    break;
                case WiperSwitchRearStatusFromIBU::OFF:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
                    break;
            }
        } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED ||
                   mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
            switch (mWiperSwitchRearStatusFromMFSW) {
                case WiperSwitchRearStatusFromMFSW::LOW_ON:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::LOW_ON;
                    break;
                case WiperSwitchRearStatusFromMFSW::HIGH_ON:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::HIGH_ON;
                    break;
                case WiperSwitchRearStatusFromMFSW::OFF:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
                    break;
                default:
                    mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
                    break;
            }
        } else {
            mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
        }
    }

    void updateLightSwitchPopupTypeStat() {
        SFCMFSWConstantLightSwitchPopupTypeStat popupType{SFCMFSWConstantLightSwitchPopupTypeStat::NON_AUTO_TEXT};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF ||
                mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED) {
                if (mConfigAutolight == ConfigAutolight::ON && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantLightSwitchPopupTypeStat::AUTO_SYMBOL;
                } else if (mConfigAutolight == ConfigAutolight::ON &&
                           (mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP || mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN)) {
                    popupType = SFCMFSWConstantLightSwitchPopupTypeStat::AUTO_TEXT;
                } else if (mConfigAutolight == ConfigAutolight::OFF && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantLightSwitchPopupTypeStat::NON_AUTO_SYMBOL;
                } else {
                    // NON_AUTO_TEXT
                }
            } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD &&
                       mConfigAutolight == ConfigAutolight::ON) {
                if (mLightPopUpMsgType == LightPopUpMsgType::TYPE_IGNOFF_AND_P) {
                    if (mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                        popupType = SFCMFSWConstantLightSwitchPopupTypeStat::AUTO_SYMBOL_2ND;
                    } else if (mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP ||
                               mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN) {
                        popupType = SFCMFSWConstantLightSwitchPopupTypeStat::AUTO_TEXT_2ND;
                    } else {
                        // NON_AUTO_TEXT
                    }
                } else if (mLightPopUpMsgType == LightPopUpMsgType::TYPE_P_RELEASE) {
                    if (mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP || mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP ||
                        mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN) {
                        popupType = SFCMFSWConstantLightSwitchPopupTypeStat::AUTO_NOT_P;
                    }
                } else {
                    // NON_AUTO_TEXT
                }
            } else {
                // NON_AUTO_TEXT
            }
        }
        setSFCMFSWConstantLightSwitchPopupTypeStat(popupType);
    }

    void updateWiperFrSwitchPopupTypeStat() {
        SFCMFSWConstantWiperFrSwitchPopupTypeStat popupType{SFCMFSWConstantWiperFrSwitchPopupTypeStat::NON_AUTO_UP_TEXT_2ND};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF ||
                mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED) {
                if (mConfigAutoWiper == ConfigAutoWiper::ON && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::AUTO_UP;
                } else if (mConfigAutoWiper == ConfigAutoWiper::ON && mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::AUTO_UP_TEXT;
                } else if (mConfigAutoWiper == ConfigAutoWiper::ON && mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::AUTO_DOWN;
                } else if (mConfigAutoWiper == ConfigAutoWiper::OFF && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::NON_AUTO_UP;
                } else if (mConfigAutoWiper == ConfigAutoWiper::OFF && mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::NON_AUTO_UP_TEXT;
                } else if (mConfigAutoWiper == ConfigAutoWiper::OFF && mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::NON_AUTO_DOWN;
                } else {
                    // NON_AUTO_UP_TEXT_2ND
                }
            } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
                if (mConfigAutoWiper == ConfigAutoWiper::ON && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::AUTO_UP_2ND;
                } else if (mConfigAutoWiper == ConfigAutoWiper::ON && mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::AUTO_UP_TEXT_2ND;
                } else if (mConfigAutoWiper == ConfigAutoWiper::OFF && mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperFrSwitchPopupTypeStat::NON_AUTO_UP_2ND;
                } else {
                    // NON_AUTO_UP_TEXT_2ND
                }
            } else {
                // NON_AUTO_UP_TEXT_2ND
            }
        }
        setSFCMFSWConstantWiperFrSwitchPopupTypeStat(popupType);
    }

    void updateWiperRrSwitchPopupTypeStat() {
        SFCMFSWConstantWiperRrSwitchPopupTypeStat popupType{SFCMFSWConstantWiperRrSwitchPopupTypeStat::TEXT_2ND};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::OFF ||
                mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED) {
                if (mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperRrSwitchPopupTypeStat::SYMBOL;
                } else if (mConfigMfswSymbol == ConfigMfswSymbol::TEXT_UP || mConfigMfswSymbol == ConfigMfswSymbol::TEXT_DOWN) {
                    popupType = SFCMFSWConstantWiperRrSwitchPopupTypeStat::TEXT;
                } else {
                    // TEXT_2ND
                }
            } else if (mMfswCANTypeAppliedStatus == MfswCANTypeAppliedStatus::APPLIED_LIGHT_CPAD) {
                if (mConfigMfswSymbol == ConfigMfswSymbol::SYMBOL_UP) {
                    popupType = SFCMFSWConstantWiperRrSwitchPopupTypeStat::SYMBOL_2ND;
                } else {
                    // TEXT_2ND
                }
            } else {
                // TEXT_2ND
            }
        } else {
            // TEXT_2ND
        }
        setSFCMFSWConstantWiperRrSwitchPopupTypeStat(popupType);
    }

    void updateLightSwitchSelectStat() {
        SFCMFSWConstantLightSwitchSelectStat selectStat{SFCMFSWConstantLightSwitchSelectStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mPrivateLightSwStatus) {
                case PrivateLightSwStatusEnum::TAIL_ON:
                    selectStat = SFCMFSWConstantLightSwitchSelectStat::TAIL_ON;
                    break;
                case PrivateLightSwStatusEnum::HEAD_LOW_ON:
                    selectStat = SFCMFSWConstantLightSwitchSelectStat::HEAD_LOW_ON;
                    break;
                case PrivateLightSwStatusEnum::AUTO_ON:
                    selectStat = SFCMFSWConstantLightSwitchSelectStat::AUTO_ON;
                    break;
                default:
                    break;
            }
        }
        setSFCMFSWConstantLightSwitchSelectStat(selectStat);
    }

    void updateWiperFrontSwitchSelectStat() {
        SFCMFSWConstantWiperFrontSwitchSelectStat selectStat{SFCMFSWConstantWiperFrontSwitchSelectStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mPrivateWiperFrSwStatus) {
                case PrivateWiperFrSwStatusEnum::INT_OR_AUTO:
                    selectStat = SFCMFSWConstantWiperFrontSwitchSelectStat::INT_OR_AUTO;
                    break;
                case PrivateWiperFrSwStatusEnum::LOW_ON:
                    selectStat = SFCMFSWConstantWiperFrontSwitchSelectStat::LOW_ON;
                    break;
                case PrivateWiperFrSwStatusEnum::HIGH_ON:
                    selectStat = SFCMFSWConstantWiperFrontSwitchSelectStat::HIGH_ON;
                    break;
                default:
                    break;
            }
        }
        setSFCMFSWConstantWiperFrontSwitchSelectStat(selectStat);
    }

    void updateWiperRearSwitchSelectStat() {
        SFCMFSWConstantWiperRearSwitchSelectStat selectStat{SFCMFSWConstantWiperRearSwitchSelectStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mPrivateWiperRrSwStatus) {
                case PrivateWiperRrSwStatusEnum::LOW_ON:
                    selectStat = SFCMFSWConstantWiperRearSwitchSelectStat::LOW_ON;
                    break;
                case PrivateWiperRrSwStatusEnum::HIGH_ON:
                    selectStat = SFCMFSWConstantWiperRearSwitchSelectStat::HIGH_ON;
                    break;
                default:
                    break;
            }
        }
        setSFCMFSWConstantWiperRearSwitchSelectStat(selectStat);
    }

    void updateWiperIntSwitchSelectStat() {
        SFCMFSWConstantWiperIntSwitchSelectStat selectStat{SFCMFSWConstantWiperIntSwitchSelectStat::DISPLAY_OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms && mPrivateWiperFrSwStatus == PrivateWiperFrSwStatusEnum::INT_OR_AUTO) {
            switch (mPrivateWiperIntSwStatus) {
                case PrivateWiperIntSwStatusEnum::STEP1:
                    selectStat = SFCMFSWConstantWiperIntSwitchSelectStat::STEP1;
                    break;
                case PrivateWiperIntSwStatusEnum::STEP2:
                    selectStat = SFCMFSWConstantWiperIntSwitchSelectStat::STEP2;
                    break;
                case PrivateWiperIntSwStatusEnum::STEP3:
                    selectStat = SFCMFSWConstantWiperIntSwitchSelectStat::STEP3;
                    break;
                case PrivateWiperIntSwStatusEnum::STEP4:
                    selectStat = SFCMFSWConstantWiperIntSwitchSelectStat::STEP4;
                    break;
                case PrivateWiperIntSwStatusEnum::STEP5:
                    selectStat = SFCMFSWConstantWiperIntSwitchSelectStat::STEP5;
                    break;
                default:
                    break;
            }
        }
        setSFCMFSWConstantWiperIntSwitchSelectStat(selectStat);
    }

    void updateEventLightSW() {
        SFCMFSWEventLightSwitchStat eventStatus{SFCMFSWEventLightSwitchStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms && mMfswPopupSetStatus == MfswPopupSetStatus::ON &&
            mPrevPrivateLightSwStatus != mPrivateLightSwStatus) {
            eventStatus = SFCMFSWEventLightSwitchStat::ON;
        }
        setSFCMFSWEventLightSwitchStat(eventStatus);

        mPrevPrivateLightSwStatus = mPrivateLightSwStatus;
    }

    void updateEventFrWiperSW() {
        SFCMFSWEventFrontWiperSwitchStat eventStatus{SFCMFSWEventFrontWiperSwitchStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms && mMfswPopupSetStatus == MfswPopupSetStatus::ON &&
            (mPrevPrivateWiperFrSwStatus != mPrivateWiperFrSwStatus ||
             (mPrivateWiperFrSwStatus == PrivateWiperFrSwStatusEnum::INT_OR_AUTO &&
              mPrevPrivateWiperIntSwStatus != mPrivateWiperIntSwStatus))) {
            eventStatus = SFCMFSWEventFrontWiperSwitchStat::ON;
        }
        setSFCMFSWEventFrontWiperSwitchStat(eventStatus);

        mPrevPrivateWiperFrSwStatus = mPrivateWiperFrSwStatus;
        mPrevPrivateWiperIntSwStatus = mPrivateWiperIntSwStatus;
    }

    void updateEventRrWiperSW() {
        SFCMFSWEventRearWiperSwitchStat eventStatus{SFCMFSWEventRearWiperSwitchStat::OFF};

        if (mIgnElapsed == IgnElapsed::ON_0ms && mMfswPopupSetStatus == MfswPopupSetStatus::ON &&
            mPrevPrivateWiperRrSwStatus != mPrivateWiperRrSwStatus) {
            eventStatus = SFCMFSWEventRearWiperSwitchStat::ON;
        }
        setSFCMFSWEventRearWiperSwitchStat(eventStatus);

        mPrevPrivateWiperRrSwStatus = mPrivateWiperRrSwStatus;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class PrivateLightSwStatusEnum : uint64_t {
        TAIL_ON = 0x0,
        HEAD_LOW_ON = 0x1,
        AUTO_ON = 0x2,
        OFF = 0x3,
        MAX = 0x4
    };

    enum class PrivateWiperFrSwStatusEnum : uint64_t {
        INT_OR_AUTO = 0x0,
        LOW_ON = 0x1,
        HIGH_ON = 0x2,
        OFF = 0x3,
        MAX = 0x4
    };

    enum class PrivateWiperIntSwStatusEnum : uint64_t {
        STEP1 = 0x0,
        STEP2 = 0x1,
        STEP3 = 0x2,
        STEP4 = 0x3,
        STEP5 = 0x4,
        OFF = 0x5,
        MAX = 0x6
    };

    enum class PrivateWiperRrSwStatusEnum : uint64_t {
        LOW_ON = 0x0,
        HIGH_ON = 0x1,
        OFF = 0x2,
        MAX = 0x3
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using MfswCANTypeAppliedStatus = ArgumentsMfswCANTypeAppliedStatusChanged::Input_MfswCANTypeAppliedStatus;
    MfswCANTypeAppliedStatus mMfswCANTypeAppliedStatus = MfswCANTypeAppliedStatus::OFF;

    using LightSwitchStatusFromIBU = ArgumentsLightSwStatusChanged::Input_LightSwitchStatusFromIBU;
    using LightSwitchStatusFromMFSW = ArgumentsLightSwStatusChanged::Input_LightSwitchStatusFromMFSW;
    using LightPopUpReq = ArgumentsLightSwStatusChanged::Input_LightPopUpReq;
    using LightPopUpMsgType = ArgumentsLightPopUpMsgTypeChanged::Input_LightPopUpMsgType;
    LightSwitchStatusFromIBU mLightSwitchStatusFromIBU = LightSwitchStatusFromIBU::OFF;
    LightSwitchStatusFromMFSW mLightSwitchStatusFromMFSW = LightSwitchStatusFromMFSW::OFF;
    LightPopUpReq mLightPopUpReq = LightPopUpReq::OFF;
    LightPopUpMsgType mLightPopUpMsgType = LightPopUpMsgType::OFF;
    PrivateLightSwStatusEnum mPrevPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;
    PrivateLightSwStatusEnum mPrivateLightSwStatus = PrivateLightSwStatusEnum::OFF;

    using WiperSwitchFrontStatusFromIBU = ArgumentsWiperSwFrontStatusChanged::Input_WiperSwitchFrontStatusFromIBU;
    using WiperSwitchFrontStatusFromMFSW = ArgumentsWiperSwFrontStatusChanged::Input_WiperSwitchFrontStatusFromMFSW;
    WiperSwitchFrontStatusFromIBU mWiperSwitchFrontStatusFromIBU = WiperSwitchFrontStatusFromIBU::OFF;
    WiperSwitchFrontStatusFromMFSW mWiperSwitchFrontStatusFromMFSW = WiperSwitchFrontStatusFromMFSW::OFF;
    PrivateWiperFrSwStatusEnum mPrevPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;
    PrivateWiperFrSwStatusEnum mPrivateWiperFrSwStatus = PrivateWiperFrSwStatusEnum::OFF;

    using WiperSwitchIntStepStatusFromIBU = ArgumentsWiperSwIntStepStatusChanged::Input_WiperSwitchIntStepStatusFromIBU;
    using WiperSwitchIntStepStatusFromMFSW = ArgumentsWiperSwIntStepStatusChanged::Input_WiperSwitchIntStepStatusFromMFSW;
    WiperSwitchIntStepStatusFromIBU mWiperSwitchIntStepStatusFromIBU = WiperSwitchIntStepStatusFromIBU::OFF;
    WiperSwitchIntStepStatusFromMFSW mWiperSwitchIntStepStatusFromMFSW = WiperSwitchIntStepStatusFromMFSW::OFF;
    PrivateWiperIntSwStatusEnum mPrevPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;
    PrivateWiperIntSwStatusEnum mPrivateWiperIntSwStatus = PrivateWiperIntSwStatusEnum::OFF;

    using WiperSwitchRearStatusFromIBU = ArgumentsWiperSwRearStatusChanged::Input_WiperSwitchRearStatusFromIBU;
    using WiperSwitchRearStatusFromMFSW = ArgumentsWiperSwRearStatusChanged::Input_WiperSwitchRearStatusFromMFSW;
    WiperSwitchRearStatusFromIBU mWiperSwitchRearStatusFromIBU = WiperSwitchRearStatusFromIBU::OFF;
    WiperSwitchRearStatusFromMFSW mWiperSwitchRearStatusFromMFSW = WiperSwitchRearStatusFromMFSW::OFF;
    PrivateWiperRrSwStatusEnum mPrevPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;
    PrivateWiperRrSwStatusEnum mPrivateWiperRrSwStatus = PrivateWiperRrSwStatusEnum::OFF;

    using ConfigAutolight = ArgumentsConfigChanged::Inter_ConfigAutolight;
    using ConfigAutoWiper = ArgumentsConfigChanged::Inter_ConfigAutoWiper;
    using ConfigMfswSymbol = ArgumentsConfigChanged::Inter_ConfigMfswSymbol;
    using ConfigArea = ArgumentsConfigChanged::Inter_ConfigArea;
    ConfigAutolight mConfigAutolight = ConfigAutolight::OFF;
    ConfigAutoWiper mConfigAutoWiper = ConfigAutoWiper::OFF;
    ConfigMfswSymbol mConfigMfswSymbol = ConfigMfswSymbol::TEXT_DOWN;
    ConfigArea mConfigArea = ConfigArea::DOM;

    using ActualVehicleSpeedCompareValueTo10 =
        ArgumentsActualVehicleSpeedCompareValueTo10Changed::Inter_ActualVehicleSpeedCompareValueTo10;
    ActualVehicleSpeedCompareValueTo10 mActualVehicleSpeedCompareValueTo10 = ActualVehicleSpeedCompareValueTo10::UNDER;

    // Factor
    static constexpr uint32_t kTimerHeadLampOffInterval10s = 10000u;

    ssfs::SFCTimer<MFSW> mOneShotTimerHeadLampOff10sStatus;

    using AutolightSensorNightStatus = ArgumentsHeadLampOffAlarmStatusChanged::Input_AutolightSensorNightStatus;
    AutolightSensorNightStatus mAutolightSensorNightStatus = AutolightSensorNightStatus::OFF;
    using MfswPopupSetStatus = ArgumentsMfswPopupSetStatusChanged::Inter_MfswPopupSetStatus;
    MfswPopupSetStatus mMfswPopupSetStatus = MfswPopupSetStatus::OFF;

    using ExtLowBeamOnReqStatus = ArgumentsHeadLampOffAlarmStatusChanged::Input_ExtLowBeamOnReqStatus;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    using Lamp_HdLmpLoOffPopUpReq = ArgumentsHeadLampOffAlarmStatusChanged::Input_Lamp_HdLmpLoOffPopUpReq;
    Lamp_HdLmpLoOffPopUpReq mLamp_HdLmpLoOffPopUpReq = Lamp_HdLmpLoOffPopUpReq::OLD;
};

}  // namespace ccos

#endif  // SFSS_MFSW_H
