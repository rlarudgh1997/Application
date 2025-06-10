/**
 * @file SBW.hpp
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
#ifndef SFSS_SBW_H
#define SFSS_SBW_H

#define DLOG_ENABLED gEnableSFCLog

#include "SBWBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 12.0.1
// Reference : [PT350] SBW. Contains : Constant, Event. Recommend: VALUE_CHANGED
class SBW : public SBWBase {
public:
    SBW() = default;
    ~SBW() override = default;
    SBW(const SBW& other) = delete;
    SBW(SBW&& other) noexcept = delete;
    SBW& operator=(const SBW& other) = delete;
    SBW& operator=(SBW&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerLvrWrnSbwShifterMsgTimeOut.create(
                kTimerLvrWarnMsgInterval2s, this, &SBW::onOneShotTimerLvrWarnSbwShifterMsgTimeOutChanged, false) == false) {
            DWarning() << "[SBW] mOneShotTimerLvrWrnSbwShifterMsgTimeOut Create Failed";
        }

        if (mOneShotTimerLvrWrnSbwMsgTimeOut.create(kTimerLvrWarnMsgInterval2s, this,
                                                    &SBW::onOneShotTimerLvrWarnSbwMsgTimeOutChanged, false) == false) {
            DWarning() << "[SBW] mOneShotTimerLvrWrnSbwMsgTimeOut Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsNStayModeToNormalVCU = false;
        mIsNStayModeToNormalTCU = false;
        mIsNStayModeToNormalSCU = false;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstant();
        updateEventSbwWarnStatusFromTCU();
        updateEventSbwWarnStatusFromSCU();
        updateEventSbwWarnStatusFromEVSCU();
        updateEventLvrWrnSbwShifter();
        updateEventLvrWrnSbw();
        updateEventNPositionStayModeStatusFromVCU();
        updateEventNPositionStayModeStatusFromTCU();
        updateEventNPositionStayModeStatusFromSCU();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstant();
        updateEventSbwWarnStatusFromTCU();
        updateEventSbwWarnStatusFromSCU();
        updateEventSbwWarnStatusFromEVSCU();
        updateEventLvrWrnSbwShifter();
        updateEventLvrWrnSbw();
        updateEventNPositionStayModeStatusFromVCU();
        updateEventNPositionStayModeStatusFromTCU();
        updateEventNPositionStayModeStatusFromSCU();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigSBW = args.mInter_ConfigSBW;
        mConfigFwSBW = args.mInter_ConfigFwSBW;
        mConfigRwSBW = args.mInter_ConfigRwSBW;
        updateConstant();
        updateEventSbwWarnStatusFromTCU();
        updateEventSbwWarnStatusFromSCU();
        updateEventSbwWarnStatusFromVCU();
        updateEventSbwWarnStatusFromEVSCU();
        updateEventLvrWrnSbwShifter();
        updateEventLvrWrnSbw();
        updateEventNPositionStayModeStatusFromVCU();
        updateEventNPositionStayModeStatusFromTCU();
        updateEventNPositionStayModeStatusFromSCU();
    }

    void onDriverPositionTypeChanged(const ArgumentsDriverPositionTypeChanged& args) {
        mDriverPositionType = args.mInput_DriverPositionTypeStatus;
        updateConstant();
    }

    void onSbwWarnFromTCUChanged(const ArgumentsSbwWarnFromTCUChanged& args) {
        mSbwWarnStatusFromTCU = args.mInput_SbwWarnStatusFromTCU;
        updateEventSbwWarnStatusFromTCU();
    }

    void onSbwWarnFromSCUChanged(const ArgumentsSbwWarnFromSCUChanged& args) {
        mSbwWarnStatusFromSCU = args.mInput_SbwWarnStatusFromSCU;
        updateEventSbwWarnStatusFromSCU();
    }

    void onSbwWarnFromVCUChanged(const ArgumentsSbwWarnFromVCUChanged& args) {
        mSbwWarnStatusFromVCU = args.mInput_SbwWarnStatusFromVCU;
        updateEventSbwWarnStatusFromVCU();
    }

    void onSbwWarnFromEVSCUChanged(const ArgumentsSbwWarnFromEVSCUChanged& args) {
        mSbwWarnStatusFromEVSCU = args.mInput_SbwWarnStatusFromEVSCU;
        updateEventSbwWarnStatusFromEVSCU();
    }

    void onLvrWrnSbwShifterChanged(const ArgumentsLvrWrnSbwShifterChanged& args) {
        if (ISTIMEOUT(args.mInput_SbwLeverWarnStatusFromSbwShifter) == true) {
            mSbwLeverWarnStatusFromSbwShifter = SbwLeverWarnStatusFromSbwShifter::UNHANDLED_TIMEOUT;
        } else {
            mSbwLeverWarnStatusFromSbwShifter = args.mInput_SbwLeverWarnStatusFromSbwShifter;
        }

        updateEventLvrWrnSbwShifter();

        // In case of MESSAGE_TIMEOUT, flush() is called after 2000ms
        if (mSbwLeverWarnStatusFromSbwShifter != SbwLeverWarnStatusFromSbwShifter::UNHANDLED_TIMEOUT) {
            mOneShotTimerLvrWrnSbwShifterMsgTimeOut.stop();
        }
    }

    void onLvrWrnSbwChanged(const ArgumentsLvrWrnSbwChanged& args) {
        if (ISTIMEOUT(args.mInput_SbwLeverWarnStatusFromSBW) == true) {
            mSbwLeverWarnStatusFromSBW = SbwLeverWarnStatusFromSBW::UNHANDLED_TIMEOUT;
        } else {
            mSbwLeverWarnStatusFromSBW = args.mInput_SbwLeverWarnStatusFromSBW;
        }

        updateEventLvrWrnSbw();

        // In case of MESSAGE_TIMEOUT, flush() is called after 2000ms
        if (mSbwLeverWarnStatusFromSBW != SbwLeverWarnStatusFromSBW::UNHANDLED_TIMEOUT) {
            mOneShotTimerLvrWrnSbwMsgTimeOut.stop();
        }
    }

    void onNModeSettingFromVCUChanged(const ArgumentsNModeSettingFromVCUChanged& args) {
        mIsNStayModeToNormalVCU = false;

        if ((mNPositionStayModeStatusFromVCU == NPositionStayModeStatusFromVCU::N_STAY_MODE) &&
            (args.mInput_NPositionStayModeStatusFromVCU == NPositionStayModeStatusFromVCU::NORMAL)) {
            mIsNStayModeToNormalVCU = true;
        }

        mNPositionStayModeStatusFromVCU = args.mInput_NPositionStayModeStatusFromVCU;

        updateEventNPositionStayModeStatusFromVCU();
    }

    void onNModeSettingFromTCUChanged(const ArgumentsNModeSettingFromTCUChanged& args) {
        mIsNStayModeToNormalTCU = false;

        if ((mNPositionStayModeStatusFromTCU == NPositionStayModeStatusFromTCU::N_STAY_MODE) &&
            (args.mInput_NPositionStayModeStatusFromTCU == NPositionStayModeStatusFromTCU::NORMAL)) {
            mIsNStayModeToNormalTCU = true;
        }

        mNPositionStayModeStatusFromTCU = args.mInput_NPositionStayModeStatusFromTCU;

        updateEventNPositionStayModeStatusFromTCU();
    }

    void onNModeSettingFromSCUChanged(const ArgumentsNModeSettingFromSCUChanged& args) {
        mIsNStayModeToNormalSCU = false;

        if ((mNPositionStayModeStatusFromSCU == NPositionStayModeStatusFromSCU::N_STAY_MODE) &&
            (args.mInput_NPositionStayModeStatusFromSCU == NPositionStayModeStatusFromSCU::NORMAL)) {
            mIsNStayModeToNormalSCU = true;
        }

        mNPositionStayModeStatusFromSCU = args.mInput_NPositionStayModeStatusFromSCU;

        updateEventNPositionStayModeStatusFromSCU();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void onOneShotTimerLvrWarnSbwShifterMsgTimeOutChanged() {
        updateSFCSBWEventLvrWrnSbwShifter("E22841");
        flush();
    }

    void onOneShotTimerLvrWarnSbwMsgTimeOutChanged() {
        updateSFCSBWEventLvrWrnSbw("E22841", "SND_PopUpWarn1");
        flush();
    }

    void updateConstant() {
        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                if (mDriverPositionType == DriverPositionType::LHD) {
                    setSFCSBWConstantSBWStat(SFCSBWConstantSBWStat::LHD);
                } else {
                    setSFCSBWConstantSBWStat(SFCSBWConstantSBWStat::RHD);
                }
            }
        }
    }

    void updateEventSbwWarnStatusFromTCU() {
        std::string eventID;
        std::string linkedSoundID;
        SFCSBWEventSbwWarnFromTCULinkedSoundType linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::NONE;
        HUInt64 linkedSoundCount = 0;
        HUInt64 linkedSoundMin = 0;
        HUInt64 linkedSoundMax = 0;

        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::CONDUCT_MAINTENANCE) {
                    eventID = "E22841";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::GEAR_CHANGE_CONDITION_NOT_MET) {
                    eventID = "E22806";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::INFINITE;
                    linkedSoundCount = 0;
                } else if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::BRAKE_TO_CHANGE_GEAR) {
                    eventID = "E22807";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::STOP_BEFORE_SHIFT_P) {
                    eventID = "E22808";
                    linkedSoundID = "SND_PopUpWarn2";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::REPEAT_MINMAX;
                    linkedSoundMin = 0;
                    linkedSoundMax = 12;
                    linkedSoundCount = 0;
                } else if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::GEAR_ALREADY_SELECTED) {
                    eventID = "E22828";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else {
                    // NONE
                }
            } else {
                if (mSbwWarnStatusFromTCU == SbwWarnStatusFromTCU::GEAR_ALREADY_SELECTED) {
                    eventID = "E22828";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCSBWEventSbwWarnFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                }
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.SbwWarnFromTCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventSbwWarnFromTCUStat(SFCSBWEventSbwWarnFromTCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventSbwWarnFromTCUID(eventID);
            setSFCSBWEventSbwWarnFromTCUStat(SFCSBWEventSbwWarnFromTCUStat::ON);
            setSFCSBWEventSbwWarnFromTCULinkedSoundID(linkedSoundID);
            setSFCSBWEventSbwWarnFromTCULinkedSoundType(linkedSoundType);
            setSFCSBWEventSbwWarnFromTCULinkedSoundRepeatMin(linkedSoundMin);
            setSFCSBWEventSbwWarnFromTCULinkedSoundRepeatMax(linkedSoundMax);
            setSFCSBWEventSbwWarnFromTCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    void updateEventSbwWarnStatusFromSCU() {
        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                updateEventSbwWarnStatusFromSCUIgnOn();
            } else {
                updateEventSbwWarnStatusFromSCUPowerOn();
            }
        } else {
            updateSFCSBWEventSbwWarnFromSCU("", "", SFCSBWEventSbwWarnFromSCULinkedSoundType::NONE, 0);
        }
    }

    void updateEventSbwWarnStatusFromSCUIgnOn() {
        if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::SYS_CHK) {
            updateSFCSBWEventSbwWarnFromSCU("E22851", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else {
            updateEventSbwWarnStatusFromSCUPowerOn();
        }
    }

    void updateEventSbwWarnStatusFromSCUPowerOn() {
        if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::PARK_MALFUNCTION) {
            updateSFCSBWEventSbwWarnFromSCU("E22809", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::GEAR_CHANGE_CONDITION_NOT_MET) {
            updateSFCSBWEventSbwWarnFromSCU("E22877", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::INFINITE, 0);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::BRAKE_TO_CHANGE_GEAR) {
            updateSFCSBWEventSbwWarnFromSCU("E22811", "SND_PopUpInform2", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::STOP_BEFORE_SHIFT_P) {
            updateSFCSBWEventSbwWarnFromSCU("E22812", "SND_PopUpWarn2", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_MINMAX,
                                            0, 0, 12);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::BRAKE_AND_N ||
                   mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::BRAKE_AND_P_RELEASE) {
            updateSFCSBWEventSbwWarnFromSCU("E22816", "SND_PopUpInform2", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::ALREADY_SELECTED_R) {
            updateSFCSBWEventSbwWarnFromSCU("E22815", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::GEAR_CHANGE_UNAVAILABLE) {
            updateSFCSBWEventSbwWarnFromSCU("E22817", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::UNLOCK_AND_CHANGE_GEAR) {
            updateSFCSBWEventSbwWarnFromSCU("E22819", "SND_PopUpInform2", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::START_AND_CHANGE_GEAR) {
            updateSFCSBWEventSbwWarnFromSCU("E22820", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::ALREADY_SELECTED_P) {
            updateSFCSBWEventSbwWarnFromSCU("E22813", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::ALREADY_SELECTED_N) {
            updateSFCSBWEventSbwWarnFromSCU("E22821", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::ALREADY_SELECTED_D) {
            updateSFCSBWEventSbwWarnFromSCU("E22822", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::AVOID_SHIFTING) {
            updateSFCSBWEventSbwWarnFromSCU("E22818", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::ALREADY_D_FOR_M) {
            updateSFCSBWEventSbwWarnFromSCU("E22878", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromSCULinkedSoundType::REPEAT_COUNT,
                                            1);
        } else {
            updateSFCSBWEventSbwWarnFromSCU("", "", SFCSBWEventSbwWarnFromSCULinkedSoundType::NONE, 0);
        }
    }

    void updateSFCSBWEventSbwWarnFromSCU(const std::string& eventID, const std::string& linkedSoundID,
                                         const SFCSBWEventSbwWarnFromSCULinkedSoundType& linkedSoundType,
                                         const HUInt64& linkedSoundCount, const HUInt64& linkedSoundMin = 0,
                                         const HUInt64& linkedSoundMax = 0) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.SbwWarnFromSCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventSbwWarnFromSCUStat(SFCSBWEventSbwWarnFromSCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventSbwWarnFromSCUID(eventID);
            setSFCSBWEventSbwWarnFromSCUStat(SFCSBWEventSbwWarnFromSCUStat::ON);
            setSFCSBWEventSbwWarnFromSCULinkedSoundID(linkedSoundID);
            setSFCSBWEventSbwWarnFromSCULinkedSoundType(linkedSoundType);
            setSFCSBWEventSbwWarnFromSCULinkedSoundRepeatMin(linkedSoundMin);
            setSFCSBWEventSbwWarnFromSCULinkedSoundRepeatMax(linkedSoundMax);
            setSFCSBWEventSbwWarnFromSCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    // IGN ON조건이 제거되어 함수 일원화
    void updateEventSbwWarnStatusFromVCU() {
        if (mConfigSBW == ConfigSBW::ON) {
            if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::PARK_MALFUNCTION) {
                updateSFCSBWEventSbwWarnFromVCU("E22809", "SND_PopUpWarn1",
                                                SFCSBWEventSbwWarnFromVCULinkedSoundType::REPEAT_COUNT, 1);
            } else if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::GEAR_CHANGE_CONDITION_NOT_MET) {
                updateSFCSBWEventSbwWarnFromVCU("E22810", "SND_PopUpWarn1", SFCSBWEventSbwWarnFromVCULinkedSoundType::INFINITE,
                                                0);
            } else if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::BRAKE_TO_CHANGE_GEAR) {
                updateSFCSBWEventSbwWarnFromVCU("E22811", "SND_PopUpInform2",
                                                SFCSBWEventSbwWarnFromVCULinkedSoundType::REPEAT_COUNT, 1);
            } else if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::STOP_BEFORE_SHIFT_P) {
                updateSFCSBWEventSbwWarnFromVCU("E22812", "SND_PopUpWarn2",
                                                SFCSBWEventSbwWarnFromVCULinkedSoundType::REPEAT_MINMAX, 0, 0, 12);
            } else if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::BRAKE_AND_P_RELEASE) {
                updateSFCSBWEventSbwWarnFromVCU("E22827", "SND_PopUpInform2",
                                                SFCSBWEventSbwWarnFromVCULinkedSoundType::REPEAT_COUNT, 1);
            } else if (mSbwWarnStatusFromVCU == SbwWarnStatusFromVCU::GEAR_ALREADY_SELECTED) {
                updateSFCSBWEventSbwWarnFromVCU("E22828", "SND_PopUpWarn1",
                                                SFCSBWEventSbwWarnFromVCULinkedSoundType::REPEAT_COUNT, 1);
            } else {
                updateSFCSBWEventSbwWarnFromVCU("", "", SFCSBWEventSbwWarnFromVCULinkedSoundType::NONE, 0);
            }
        } else {
            updateSFCSBWEventSbwWarnFromVCU("", "", SFCSBWEventSbwWarnFromVCULinkedSoundType::NONE, 0);
        }
    }

    void updateSFCSBWEventSbwWarnFromVCU(const std::string& eventID, const std::string& linkedSoundID,
                                         const SFCSBWEventSbwWarnFromVCULinkedSoundType& linkedSoundType,
                                         const HUInt64& linkedSoundCount, const HUInt64& linkedSoundMin = 0,
                                         const HUInt64& linkedSoundMax = 0) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.SbwWarnFromVCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventSbwWarnFromVCUStat(SFCSBWEventSbwWarnFromVCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventSbwWarnFromVCUID(eventID);
            setSFCSBWEventSbwWarnFromVCUStat(SFCSBWEventSbwWarnFromVCUStat::ON);
            setSFCSBWEventSbwWarnFromVCULinkedSoundID(linkedSoundID);
            setSFCSBWEventSbwWarnFromVCULinkedSoundType(linkedSoundType);
            setSFCSBWEventSbwWarnFromVCULinkedSoundRepeatMin(linkedSoundMin);
            setSFCSBWEventSbwWarnFromVCULinkedSoundRepeatMax(linkedSoundMax);
            setSFCSBWEventSbwWarnFromVCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    void updateEventSbwWarnStatusFromEVSCU() {
        if (mConfigSBW == ConfigSBW::ON && mSbwWarnStatusFromEVSCU == SbwWarnStatusFromEVSCU::AVOID_SHIFTING) {
            setSFCSBWEventSbwWarnFromEVSCUStat(SFCSBWEventSbwWarnFromEVSCUStat::ON);
        } else {
            setSFCSBWEventSbwWarnFromEVSCUStat(SFCSBWEventSbwWarnFromEVSCUStat::OFF);
        }

        setSFCSBWEventSbwWarnFromEVSCUID("E22818");
        setSFCSBWEventSbwWarnFromEVSCULinkedSoundID("SND_PopUpWarn1");
        setSFCSBWEventSbwWarnFromEVSCULinkedSoundType(SFCSBWEventSbwWarnFromEVSCULinkedSoundType::REPEAT_COUNT);
        setSFCSBWEventSbwWarnFromEVSCULinkedSoundRepeatCount(1);
    }

    void updateEventLvrWrnSbwShifter() {
        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                updateEventLvrWrnSbwShifterIgnOn();
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
                updateEventLvrWrnSbwShifterIgnOff();
            } else {
                // nothing to do
            }
        } else {
            updateSFCSBWEventLvrWrnSbwShifter("");
        }
    }

    void updateEventLvrWrnSbwShifterIgnOn() {
        if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::UNHANDLED_TIMEOUT &&
            mConfigFwSBW == ConfigFwSBW::ON) {
            if (mOneShotTimerLvrWrnSbwShifterMsgTimeOut.restart() == false) {
                DWarning() << "[SBW] mOneShotTimerLvrWrnSbwShifterMsgTimeOut Restart Failed";
            }

        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_SHIFT_CONTROL) {
            updateSFCSBWEventLvrWrnSbwShifter("E22829");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_P_RELEASE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22830");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_P_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22831");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_D_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22832");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::STUCK_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22833");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::STUCK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22834");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22836");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CONTROL_MANUALLY_DIAL_SPHERE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22852");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_SURROUNDING_DIAL_SPHERE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22853");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::STUCK_SHIFT_LEVER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22838");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_SHIFT_LEVER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22839");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CONTROL_MANUALLY_COLUMN_ROTARY) {
            updateSFCSBWEventLvrWrnSbwShifter("E22855");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::TRY_OPERATE_RESTART_DIAL_SPHERE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22858");
        } else {
            updateSFCSBWEventLvrWrnSbwShifter("");
        }
    }

    void updateEventLvrWrnSbwShifterIgnOff() {
        if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_SHIFT_CONTROL) {
            updateSFCSBWEventLvrWrnSbwShifter("E22859");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_P_RELEASE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22860");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_P_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22861");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_D_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22862");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::STUCK_BUTTON) {
            updateSFCSBWEventLvrWrnSbwShifter("E22863");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::STUCK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22864");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbwShifter("E22866");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::CHECK_SURROUNDING_DIAL_SPHERE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22854");
        } else if (mSbwLeverWarnStatusFromSbwShifter == SbwLeverWarnStatusFromSbwShifter::TRY_OPERATE_RESTART_DIAL_SPHERE) {
            updateSFCSBWEventLvrWrnSbwShifter("E22870");
        } else {
            updateSFCSBWEventLvrWrnSbwShifter("");
        }
    }

    void updateSFCSBWEventLvrWrnSbwShifter(const std::string& eventID) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.LvrWrnSbwShifter.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventLvrWrnSbwShifterStat(SFCSBWEventLvrWrnSbwShifterStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventLvrWrnSbwShifterID(eventID);
            setSFCSBWEventLvrWrnSbwShifterStat(SFCSBWEventLvrWrnSbwShifterStat::ON);
            setSFCSBWEventLvrWrnSbwShifterLinkedSoundID("SND_PopUpWarn1");
            setSFCSBWEventLvrWrnSbwShifterLinkedSoundType(SFCSBWEventLvrWrnSbwShifterLinkedSoundType::REPEAT_COUNT);
            setSFCSBWEventLvrWrnSbwShifterLinkedSoundRepeatCount(1);
        }
    }

    void updateEventLvrWrnSbw() {
        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                updateEventLvrWrnSbwIgnOn();
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
                updateEventLvrWrnSbwIgnOff();
            } else {
                // Do nothing
            }
        } else {
            updateSFCSBWEventLvrWrnSbw("", "");
        }
    }

    void updateEventLvrWrnSbwIgnOn() {
        if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::SYS_FAIL && mConfigRwSBW == ConfigRwSBW::ON) {
            updateSFCSBWEventLvrWrnSbw("E22841", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::UNHANDLED_TIMEOUT &&
                   mConfigRwSBW == ConfigRwSBW::ON) {
            if (mOneShotTimerLvrWrnSbwMsgTimeOut.restart() == false) {
                DWarning() << "[SBW] mOneShotTimerLvrWrnSbwMsgTimeOut Restart Failed";
            }
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbw("E22842", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_P_BUTTON) {
            updateSFCSBWEventLvrWrnSbw("E22843", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_P_RELEASE) {
            updateSFCSBWEventLvrWrnSbw("E22845", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::STUCK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbw("E22846", "SND_PopUpWarn1");
        } else {
            updateEventLvrWrnSbwPowerOn();
        }
    }

    void updateEventLvrWrnSbwIgnOff() {
        if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::SYS_FAIL) {
            updateSFCSBWEventLvrWrnSbw("E22871", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbw("E22872", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_P_BUTTON) {
            updateSFCSBWEventLvrWrnSbw("E22873", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::CHECK_P_RELEASE) {
            updateSFCSBWEventLvrWrnSbw("E22875", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::STUCK_ROTARY_SHIFTER) {
            updateSFCSBWEventLvrWrnSbw("E22876", "SND_PopUpWarn1");
        } else {
            updateEventLvrWrnSbwPowerOn();
        }
    }

    void updateEventLvrWrnSbwPowerOn() {
        if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::BRAKE_AND_P_RELEASE) {
            updateSFCSBWEventLvrWrnSbw("E22844", "SND_PopUpInform2");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::BRAKE_TO_CHANGE_GEAR) {
            updateSFCSBWEventLvrWrnSbw("E22847", "SND_PopUpInform2");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::GEAR_ALREADY_SELECTED) {
            updateSFCSBWEventLvrWrnSbw("E22848", "SND_PopUpWarn1");
        } else if (mSbwLeverWarnStatusFromSBW == SbwLeverWarnStatusFromSBW::ROTARY_TURNED_WHILE_P_PRESSED) {
            updateSFCSBWEventLvrWrnSbw("E22849", "SND_PopUpWarn1");
        } else {
            updateSFCSBWEventLvrWrnSbw("", "");
        }
    }

    void updateSFCSBWEventLvrWrnSbw(const std::string& eventID, const std::string& linkedSoundID) {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.LvrWrnSbw.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventLvrWrnSbwStat(SFCSBWEventLvrWrnSbwStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventLvrWrnSbwID(eventID);
            setSFCSBWEventLvrWrnSbwStat(SFCSBWEventLvrWrnSbwStat::ON);
            setSFCSBWEventLvrWrnSbwLinkedSoundID(linkedSoundID);
            setSFCSBWEventLvrWrnSbwLinkedSoundType(SFCSBWEventLvrWrnSbwLinkedSoundType::REPEAT_COUNT);
            setSFCSBWEventLvrWrnSbwLinkedSoundRepeatCount(1);
        }
    }

    void updateEventNPositionStayModeStatusFromVCU() {
        std::string eventID;
        std::string linkedSoundID;
        SFCSBWEventNModeSettingFromVCULinkedSoundType linkedSoundType = SFCSBWEventNModeSettingFromVCULinkedSoundType::NONE;
        HUInt64 linkedSoundCount = 0;

        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                if (mNPositionStayModeStatusFromVCU == NPositionStayModeStatusFromVCU::STANDBY) {
                    eventID = "E22801";
                    linkedSoundID = "";
                    linkedSoundType = SFCSBWEventNModeSettingFromVCULinkedSoundType::NONE;
                    linkedSoundCount = 0;
                } else if (mNPositionStayModeStatusFromVCU == NPositionStayModeStatusFromVCU::N_STAY_MODE) {
                    eventID = "E22802";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCSBWEventNModeSettingFromVCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else if (mIsNStayModeToNormalVCU) {
                    eventID = "E22803";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromVCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else {
                    // NONE
                }
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
                if (mNPositionStayModeStatusFromVCU == NPositionStayModeStatusFromVCU::N_STAY_MODE) {
                    eventID = "E22804";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromVCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                }
            } else {
                // NONE
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.NModeSettingFromVCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventNModeSettingFromVCUStat(SFCSBWEventNModeSettingFromVCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventNModeSettingFromVCUID(eventID);
            setSFCSBWEventNModeSettingFromVCUStat(SFCSBWEventNModeSettingFromVCUStat::ON);
            setSFCSBWEventNModeSettingFromVCULinkedSoundID(linkedSoundID);
            setSFCSBWEventNModeSettingFromVCULinkedSoundType(linkedSoundType);
            setSFCSBWEventNModeSettingFromVCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    void updateEventNPositionStayModeStatusFromTCU() {
        std::string eventID;
        std::string linkedSoundID;
        SFCSBWEventNModeSettingFromTCULinkedSoundType linkedSoundType = SFCSBWEventNModeSettingFromTCULinkedSoundType::NONE;
        HUInt64 linkedSoundCount = 0;

        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                if (mNPositionStayModeStatusFromTCU == NPositionStayModeStatusFromTCU::STANDBY) {
                    eventID = "E22801";
                    linkedSoundID = "";
                    linkedSoundType = SFCSBWEventNModeSettingFromTCULinkedSoundType::NONE;
                    linkedSoundCount = 0;
                } else if (mNPositionStayModeStatusFromTCU == NPositionStayModeStatusFromTCU::N_STAY_MODE) {
                    eventID = "E22802";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCSBWEventNModeSettingFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else if (mIsNStayModeToNormalTCU) {
                    eventID = "E22803";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else {
                    // NONE
                }
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
                if (mNPositionStayModeStatusFromTCU == NPositionStayModeStatusFromTCU::N_STAY_MODE) {
                    eventID = "E22804";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromTCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                }
            } else {
                // NONE
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.NModeSettingFromTCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventNModeSettingFromTCUStat(SFCSBWEventNModeSettingFromTCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventNModeSettingFromTCUID(eventID);
            setSFCSBWEventNModeSettingFromTCUStat(SFCSBWEventNModeSettingFromTCUStat::ON);
            setSFCSBWEventNModeSettingFromTCULinkedSoundID(linkedSoundID);
            setSFCSBWEventNModeSettingFromTCULinkedSoundType(linkedSoundType);
            setSFCSBWEventNModeSettingFromTCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    void updateEventNPositionStayModeStatusFromSCU() {
        std::string eventID;
        std::string linkedSoundID;
        SFCSBWEventNModeSettingFromSCULinkedSoundType linkedSoundType = SFCSBWEventNModeSettingFromSCULinkedSoundType::NONE;
        HUInt64 linkedSoundCount = 0;

        if (mConfigSBW == ConfigSBW::ON) {
            if (mIgnElapsed == IgnElapsed::ON_0ms) {
                if (mNPositionStayModeStatusFromSCU == NPositionStayModeStatusFromSCU::STANDBY) {
                    eventID = "E22801";
                    linkedSoundID = "";
                    linkedSoundType = SFCSBWEventNModeSettingFromSCULinkedSoundType::NONE;
                    linkedSoundCount = 0;
                } else if (mNPositionStayModeStatusFromSCU == NPositionStayModeStatusFromSCU::N_STAY_MODE) {
                    eventID = "E22802";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCSBWEventNModeSettingFromSCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else if (mIsNStayModeToNormalSCU) {
                    eventID = "E22803";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromSCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                } else {
                    // NONE
                }
            } else if (mIgnElapsed == IgnElapsed::OFF_0ms) {
                if (mNPositionStayModeStatusFromSCU == NPositionStayModeStatusFromSCU::N_STAY_MODE) {
                    eventID = "E22804";
                    linkedSoundID = "SND_PopUpInform2";
                    linkedSoundType = SFCSBWEventNModeSettingFromSCULinkedSoundType::REPEAT_COUNT;
                    linkedSoundCount = 1;
                }
            } else {
                // NONE
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.SBW.Event.NModeSettingFromSCU.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCSBWEventNModeSettingFromSCUStat(SFCSBWEventNModeSettingFromSCUStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCSBWEventNModeSettingFromSCUID(eventID);
            setSFCSBWEventNModeSettingFromSCUStat(SFCSBWEventNModeSettingFromSCUStat::ON);
            setSFCSBWEventNModeSettingFromSCULinkedSoundID(linkedSoundID);
            setSFCSBWEventNModeSettingFromSCULinkedSoundType(linkedSoundType);
            setSFCSBWEventNModeSettingFromSCULinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    static constexpr uint32_t kTimerLvrWarnMsgInterval2s = 2000u;

    using ConfigSBW = ArgumentsConfigChanged::Inter_ConfigSBW;
    using ConfigFwSBW = ArgumentsConfigChanged::Inter_ConfigFwSBW;
    using ConfigRwSBW = ArgumentsConfigChanged::Inter_ConfigRwSBW;
    using DriverPositionType = ArgumentsDriverPositionTypeChanged::Input_DriverPositionTypeStatus;
    using SbwWarnStatusFromTCU = ArgumentsSbwWarnFromTCUChanged::Input_SbwWarnStatusFromTCU;
    using SbwWarnStatusFromSCU = ArgumentsSbwWarnFromSCUChanged::Input_SbwWarnStatusFromSCU;
    using SbwWarnStatusFromVCU = ArgumentsSbwWarnFromVCUChanged::Input_SbwWarnStatusFromVCU;
    using SbwWarnStatusFromEVSCU = ArgumentsSbwWarnFromEVSCUChanged::Input_SbwWarnStatusFromEVSCU;
    using SbwLeverWarnStatusFromSbwShifter = ArgumentsLvrWrnSbwShifterChanged::Input_SbwLeverWarnStatusFromSbwShifter;
    using SbwLeverWarnStatusFromSBW = ArgumentsLvrWrnSbwChanged::Input_SbwLeverWarnStatusFromSBW;
    using NPositionStayModeStatusFromVCU = ArgumentsNModeSettingFromVCUChanged::Input_NPositionStayModeStatusFromVCU;
    using NPositionStayModeStatusFromTCU = ArgumentsNModeSettingFromTCUChanged::Input_NPositionStayModeStatusFromTCU;
    using NPositionStayModeStatusFromSCU = ArgumentsNModeSettingFromSCUChanged::Input_NPositionStayModeStatusFromSCU;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigSBW mConfigSBW = ConfigSBW::OFF;
    ConfigFwSBW mConfigFwSBW = ConfigFwSBW::OFF;
    ConfigRwSBW mConfigRwSBW = ConfigRwSBW::OFF;
    DriverPositionType mDriverPositionType = DriverPositionType::LHD;
    SbwWarnStatusFromTCU mSbwWarnStatusFromTCU = SbwWarnStatusFromTCU::OFF;
    SbwWarnStatusFromSCU mSbwWarnStatusFromSCU = SbwWarnStatusFromSCU::OFF;
    SbwWarnStatusFromVCU mSbwWarnStatusFromVCU = SbwWarnStatusFromVCU::OFF;
    SbwWarnStatusFromEVSCU mSbwWarnStatusFromEVSCU = SbwWarnStatusFromEVSCU::OFF;
    SbwLeverWarnStatusFromSbwShifter mSbwLeverWarnStatusFromSbwShifter = SbwLeverWarnStatusFromSbwShifter::OFF;  // 전륜
    SbwLeverWarnStatusFromSBW mSbwLeverWarnStatusFromSBW = SbwLeverWarnStatusFromSBW::OFF;                       // 후륜
    NPositionStayModeStatusFromVCU mNPositionStayModeStatusFromVCU = NPositionStayModeStatusFromVCU::INVALID;
    NPositionStayModeStatusFromTCU mNPositionStayModeStatusFromTCU = NPositionStayModeStatusFromTCU::INVALID;
    NPositionStayModeStatusFromSCU mNPositionStayModeStatusFromSCU = NPositionStayModeStatusFromSCU::INVALID;

    HBool mIsNStayModeToNormalVCU = false;
    HBool mIsNStayModeToNormalTCU = false;
    HBool mIsNStayModeToNormalSCU = false;

    ssfs::SFCTimer<SBW> mOneShotTimerLvrWrnSbwShifterMsgTimeOut;
    ssfs::SFCTimer<SBW> mOneShotTimerLvrWrnSbwMsgTimeOut;
};

}  // namespace ccos

#endif  // SFSS_SBW_H
