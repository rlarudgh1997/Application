/**
 * @file Regen_System.hpp
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
#ifndef SFSS_Regen_System_H
#define SFSS_Regen_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Regen_SystemBase.hpp"

namespace ccos {

// SFC Version : 23.0.0
// Reference : [PT340] Regen_System. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class Regen_System : public Regen_SystemBase {
public:
    Regen_System() = default;
    ~Regen_System() override = default;
    Regen_System(const Regen_System& other) = delete;
    Regen_System(Regen_System&& other) noexcept = delete;
    Regen_System& operator=(const Regen_System& other) = delete;
    Regen_System& operator=(Regen_System&& other) noexcept = delete;

    void onInitialize() override {
        setSFCRegen_SystemEventEvRegenBrakeFunctionWarnLinkedSoundType(
            SFCRegen_SystemEventEvRegenBrakeFunctionWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventEvRegenBrakeFunctionWarnLinkedSoundRepeatCount(1);

        setSFCRegen_SystemEventEvRegenBrakeChcWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCRegen_SystemEventEvRegenBrakeChcWarnLinkedSoundType(
            SFCRegen_SystemEventEvRegenBrakeChcWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventEvRegenBrakeChcWarnLinkedSoundRepeatCount(1);

        setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnLinkedSoundType(
            SFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnLinkedSoundRepeatCount(1);

        setSFCRegen_SystemEventEvSmartRegenFailWarnID("E22404");
        setSFCRegen_SystemEventEvSmartRegenFailWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCRegen_SystemEventEvSmartRegenFailWarnLinkedSoundType(
            SFCRegen_SystemEventEvSmartRegenFailWarnLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventEvSmartRegenFailWarnLinkedSoundRepeatCount(1);

        setSFCRegen_SystemEventPaddleStopAssistControlWarnStatusID("E22409");
        setSFCRegen_SystemEventPaddleStopAssistControlWarnStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCRegen_SystemEventPaddleStopAssistControlWarnStatusLinkedSoundType(
            SFCRegen_SystemEventPaddleStopAssistControlWarnStatusLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventPaddleStopAssistControlWarnStatusLinkedSoundRepeatCount(1);

        setSFCRegen_SystemEventIpedalWarnStatusLinkedSoundType(SFCRegen_SystemEventIpedalWarnStatusLinkedSoundType::REPEAT_COUNT);
        setSFCRegen_SystemEventIpedalWarnStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateConstantAll();
        updateEventBrakeFunctionWarn();
        updateEventBrakeChcWarn();
        updateEventBrakePaddleNotActiveWarn();
        updateEventFailWarn();
        updateEventIPedalWarn();
        updateTelltaleIPedalStat();
        updateEventPaddleStopAssistControlWarn();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateConstantAll();
        updateEventBrakeFunctionWarn();
        updateEventBrakeChcWarn();
        updateEventBrakePaddleNotActiveWarn();
        updateEventFailWarn();
        updateTelltaleIPedalStat();
        updateEventIPedalWarn();
        updateEventPaddleStopAssistControlWarn();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        DDebug() << "Inter_ConfigVehicleType: " << static_cast<HUInt64>(args.mInter_ConfigVehicleType);
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEventBrakePaddleNotActiveWarn();
    }

    void onConfigIPedalTypeChanged(const ArgumentsConfigIPedalTypeChanged& args) {
        DDebug() << "Inter_ConfigIPedalType: " << static_cast<HUInt64>(args.mInter_ConfigIPedalType);
        mInter_ConfigIPedalType = args.mInter_ConfigIPedalType;
        updateConstantRegenRegenSystemStepStat();
    }

    void onSmartRegenSystemChanged(const ArgumentsSmartRegenSystemChanged& args) {
        if (ISTIMEOUT(args.mInput_RegenVehicleStopStatus) == false) {
            mRegenVehicleStopStatus = args.mInput_RegenVehicleStopStatus;
        } else {
            mRegenVehicleStopStatus = RegenVehicleStopStatus::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_RegenSmartVehicleStatus1) == false) {
            mRegenSmartVehicleStatus1 = args.mInput_RegenSmartVehicleStatus1;
        } else {
            mRegenSmartVehicleStatus1 = RegenSmartVehicleStatus1::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_RegenSmartVehicleStatus2) == false) {
            mRegenSmartVehicleStatus2 = args.mInput_RegenSmartVehicleStatus2;
        } else {
            mRegenSmartVehicleStatus2 = RegenSmartVehicleStatus2::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_RegenSmartOnReqStatus) == false) {
            mRegenSmartOnReqStatus = args.mInput_RegenSmartOnReqStatus;
        } else {
            mRegenSmartOnReqStatus = RegenSmartOnReqStatus::UNHANDLED_TIMEOUT;
        }
        if (ISTIMEOUT(args.mInput_RegenSmartLevelValue) == false) {
            mRegenSmartLevelValue = args.mInput_RegenSmartLevelValue;
        } else {
            mRegenSmartLevelValue = kRegenSmartLevelTimeoutValue;
        }
        updateConstantSmartRegenSystem();
    }

    void onBrakeFunctionWarnStatusChanged(const ArgumentsBrakeFunctionWarnStatusChanged& args) {
        mBrakeFunctionWarnStatus = args.mInput_EvRegenBrakeFunctionWarnStatus;
        updateEventBrakeFunctionWarn();
    }

    void onBrakeChcWarnStatusChanged(const ArgumentsBrakeChcWarnStatusChanged& args) {
        mBrakeChcWarnStatus = args.mInput_EvRegenBrakeChcWarnStatus;
        updateEventBrakeChcWarn();
    }

    void onBrakePaddleNotActiveWarnStatusChanged(const ArgumentsBrakePaddleNotActiveWarnStatusChanged& args) {
        mBrakePaddleNotActive = args.mInput_EvRegenBrakePaddleNotActiveWarnStatus;
        updateEventBrakePaddleNotActiveWarn();
    }

    void onFailWarnStatusChanged(const ArgumentsFailWarnStatusChanged& args) {
        mFailWarnStatus = args.mInput_EvSmartRegenFailWarnStatus;
        updateEventFailWarn();
    }

    void onIPedalIndiReqStatusChanged(const ArgumentsIPedalIndiReqStatusChanged& args) {
        mIPedalIndiReqStatus = args.mInput_IPedalIndiReqStatus;
        updateTelltaleIPedalStat();
    }

    void onIPedalWarnStatusChanged(const ArgumentsIPedalWarnStatusChanged& args) {
        mIPedalWarnStatus = args.mInput_IPedalWarnStatus;
        updateEventIPedalWarn();
    }

    void onPaddleStopAssistControlWarnStatusChanged(const ArgumentsPaddleStopAssistControlWarnStatusChanged& args) {
        mPaddleStopAssistControlWarnStatus = args.mInput_PaddleStopAssistControlWarnStatus;
        updateEventPaddleStopAssistControlWarn();
    }

    void onNPedalLvlStatusChanged(const ArgumentsNPedalLvlStatusChanged& args) {
        mNpedalLvlStatus = args.mInput_RegenNpedalLvlStatus;
        mIsNpedalLvlStatusTimeout = ISTIMEOUT(args.mInput_RegenNpedalLvlStatus);
        updateConstantRegenNpedalLvlStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantAll() {
        updateConstantSmartRegenSystem();
        updateConstantRegenNpedalLvlStat();
    }

    void updateConstantSmartRegenSystem() {
        updateConstantRegenRegenSystemStepStat();
        updateConstantRegenStatusStat();
        updateConstantRegenFrontRecognizedStat();
        updateConstantRegenStepValue();
    }

    void updateTelltaleIPedalStat() {
        SFCRegen_SystemTelltaleIpedalStat telltaleStat = SFCRegen_SystemTelltaleIpedalStat::OFF;
        SFCRegen_SystemTelltaleIpedalStatOptional telltaleStatOptional = SFCRegen_SystemTelltaleIpedalStatOptional::NONE;
        if (mIsIgnOn == true && mIPedalIndiReqStatus == IPedalIndiReqStatus::ON) {
            telltaleStat = SFCRegen_SystemTelltaleIpedalStat::ON;
        } else if (mIsIgnOn == true && mIPedalIndiReqStatus == IPedalIndiReqStatus::INACTIVE) {
            telltaleStat = SFCRegen_SystemTelltaleIpedalStat::OPTIONAL;
            telltaleStatOptional = SFCRegen_SystemTelltaleIpedalStatOptional::INACTIVE;
        } else {
            // no operation
        }
        setSFCRegen_SystemTelltaleIpedalStat(telltaleStat);
        setSFCRegen_SystemTelltaleIpedalStatOptional(telltaleStatOptional);
    }

    void updateConstantRegenRegenSystemStepStat() {
        SFCRegen_SystemConstantRegenSystemStepStat stat = SFCRegen_SystemConstantRegenSystemStepStat::APPLIED_STEP4;
        if (mRegenVehicleStopStatus == RegenVehicleStopStatus::APPLIED_STEP3 ||
            mInter_ConfigIPedalType == Inter_ConfigIPedalType::GEN_SECOND) {
            stat = SFCRegen_SystemConstantRegenSystemStepStat::APPLIED_STEP3;
        }
        setSFCRegen_SystemConstantRegenSystemStepStat(stat);
    }

    void updateConstantRegenStatusStat() {
        SFCRegen_SystemConstantRegenStatusStat statValue = SFCRegen_SystemConstantRegenStatusStat::DISPLAY_OFF;
        // EXNCP-17752, 사양서상 5가지 입력 중 하나라도 TIMEOUT이면 Don't care 입력이라 하더라도 모두 DISPLAY_OFF 되도록 함.
        if (checkNoTimeoutSignalAndIgnOn() == true) {
            if (mRegenVehicleStopStatus == RegenVehicleStopStatus::STOP && mRegenSmartLevelValue != 63) {
                statValue = SFCRegen_SystemConstantRegenStatusStat::STOP;
            } else if (mRegenVehicleStopStatus == RegenVehicleStopStatus::MAX && mRegenSmartLevelValue != 63) {
                statValue = SFCRegen_SystemConstantRegenStatusStat::MAX;
            } else if (mRegenSmartLevelValue == 63) {
                // Do Nothing To Set DISPLAY_OFF
            } else if (mRegenVehicleStopStatus != RegenVehicleStopStatus::STOP &&
                       mRegenVehicleStopStatus != RegenVehicleStopStatus::MAX) {
                if (mRegenSmartOnReqStatus == RegenSmartOnReqStatus::OFF) {
                    if (0 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 9) {
                        statValue = SFCRegen_SystemConstantRegenStatusStat::MANUAL_LV0;
                    } else if (10 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 19) {
                        statValue = SFCRegen_SystemConstantRegenStatusStat::MANUAL_LV1;
                    } else if (20 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 29) {
                        statValue = SFCRegen_SystemConstantRegenStatusStat::MANUAL_LV2;
                    } else if (30 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 39) {
                        statValue = SFCRegen_SystemConstantRegenStatusStat::MANUAL_LV3;
                    } else if (mRegenSmartLevelValue == 40) {
                        statValue = SFCRegen_SystemConstantRegenStatusStat::MAX;
                    } else {
                        // DISPLAY_OFF
                    }
                } else if ((mRegenSmartOnReqStatus == RegenSmartOnReqStatus::ON ||
                            mRegenSmartOnReqStatus == RegenSmartOnReqStatus::INACTIVE) &&
                           0 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 40) {
                    statValue = SFCRegen_SystemConstantRegenStatusStat::AUTO;
                } else {
                    // DISPLAY_OFF
                }
            } else {
                // DISPLAY_OFF
            }
        }
        setSFCRegen_SystemConstantRegenStatusStat(statValue);
    }

    void updateConstantRegenFrontRecognizedStat() {
        SFCRegen_SystemConstantRegenFrontRecognizedStat statValue = SFCRegen_SystemConstantRegenFrontRecognizedStat::DISPLAY_OFF;
        // EXNCP-17752, 사양서상 5가지 입력 중 하나라도 TIMEOUT이면 Don't care 입력이라 하더라도 모두 DISPLAY_OFF 되도록 함.
        if (checkNoTimeoutSignalAndIgnOn() == true) {
            if (mRegenSmartLevelValue == 63) {
                // Do Nothing To Set DISPLAY_OFF
            } else if (mRegenSmartOnReqStatus == RegenSmartOnReqStatus::OFF) {
                statValue = SFCRegen_SystemConstantRegenFrontRecognizedStat::UNRECOGNIZED;
            } else if (mRegenSmartOnReqStatus == RegenSmartOnReqStatus::ON) {
                if (mRegenSmartVehicleStatus1 == RegenSmartVehicleStatus1::ACTIVE ||
                    mRegenSmartVehicleStatus2 == RegenSmartVehicleStatus2::ACTIVE) {
                    statValue = SFCRegen_SystemConstantRegenFrontRecognizedStat::RECOGNIZED;
                } else {
                    statValue = SFCRegen_SystemConstantRegenFrontRecognizedStat::UNRECOGNIZED;
                }
            } else if (mRegenSmartOnReqStatus == RegenSmartOnReqStatus::INACTIVE) {
                statValue = SFCRegen_SystemConstantRegenFrontRecognizedStat::INACTIVE;
            } else {
                // DISPLAY_OFF
            }
        }
        setSFCRegen_SystemConstantRegenFrontRecognizedStat(statValue);
    }

    void updateConstantRegenStepValue() {
        SFCRegen_SystemConstantRegenStepStat statValue = SFCRegen_SystemConstantRegenStepStat::DISPLAY_OFF;
        // EXNCP-17752, 사양서상 5가지 입력 중 하나라도 TIMEOUT이면 Don't care 입력이라 하더라도 모두 DISPLAY_OFF 되도록 함.
        if (checkNoTimeoutSignalAndIgnOn() == true) {
            if (mRegenSmartLevelValue == 63) {
                // Do Nothing To Set DISPLAY_OFF
            } else if (mRegenSmartOnReqStatus == RegenSmartOnReqStatus::OFF) {
                if (0 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 9) {
                    statValue = SFCRegen_SystemConstantRegenStepStat::MANUAL_LV0;
                } else if (10 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 19) {
                    statValue = SFCRegen_SystemConstantRegenStepStat::MANUAL_LV1;
                } else if (20 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 29) {
                    statValue = SFCRegen_SystemConstantRegenStepStat::MANUAL_LV2;
                } else if (30 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 39) {
                    statValue = SFCRegen_SystemConstantRegenStepStat::MANUAL_LV3;
                } else if (mRegenSmartLevelValue == 40) {
                    statValue = SFCRegen_SystemConstantRegenStepStat::MANUAL_MAX;
                } else {
                    // DISPLAY_OFF
                }
            } else if ((mRegenSmartOnReqStatus == RegenSmartOnReqStatus::ON ||
                        mRegenSmartOnReqStatus == RegenSmartOnReqStatus::INACTIVE) &&
                       0 <= mRegenSmartLevelValue && mRegenSmartLevelValue <= 40) {
                auto value = static_cast<HDouble>(mRegenSmartLevelValue) / 10.0;
                setSFCRegen_SystemConstantRegenStepValue(value);
                statValue = SFCRegen_SystemConstantRegenStepStat::VALUE;
            } else {
                // DISPLAY_OFF
            }
        }
        setSFCRegen_SystemConstantRegenStepStat(statValue);
    }

    void updateConstantRegenNpedalLvlStat() {
        SFCRegen_SystemConstantNpedalLvlStat lvlStat = SFCRegen_SystemConstantNpedalLvlStat::DISPLAY_OFF;
        if (mIsIgnOn == true && mIsNpedalLvlStatusTimeout == false) {
            switch (mNpedalLvlStatus) {
                case NpedalLvlStatus::NPEDAL_LV1:
                    lvlStat = SFCRegen_SystemConstantNpedalLvlStat::NPEDAL_LV1;
                    break;
                case NpedalLvlStatus::NPEDAL_LV2:
                    lvlStat = SFCRegen_SystemConstantNpedalLvlStat::NPEDAL_LV2;
                    break;
                case NpedalLvlStatus::NPEDAL_LV3:
                    lvlStat = SFCRegen_SystemConstantNpedalLvlStat::NPEDAL_LV3;
                    break;
                default:
                    break;
            }
        }
        setSFCRegen_SystemConstantNpedalLvlStat(lvlStat);
    }

    void updateEventBrakeFunctionWarn() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat eventStat = SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat::OFF;

        if (mIsIgnOn == true) {
            if (mBrakeFunctionWarnStatus == BrakeFunctionWarnStatus::WARN) {
                eventID = "E22401";
                linkedSoundID = "SND_PopUpWarn1";
                eventStat = SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat::ON;
            } else if (mBrakeFunctionWarnStatus == BrakeFunctionWarnStatus::WARN_LOWTEMP) {
                eventID = "E22411";
                linkedSoundID = "SND_PopUpWarn1";
                eventStat = SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat::ON;
            } else if (mBrakeFunctionWarnStatus == BrakeFunctionWarnStatus::REGEN_RESTORED) {
                eventID = "E22410";
                linkedSoundID = "SND_PopUpInform1";
                eventStat = SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat::ON;
            } else {
                // nothing
            }
        }

        GETCACHEDVALUE(SFC.Regen_System.Event.EvRegenBrakeFunctionWarn.ID, prevEventID);
        if ((prevEventID.empty() == false) && (prevEventID != eventID)) {
            setSFCRegen_SystemEventEvRegenBrakeFunctionWarnStat(SFCRegen_SystemEventEvRegenBrakeFunctionWarnStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCRegen_SystemEventEvRegenBrakeFunctionWarnID(eventID);
            setSFCRegen_SystemEventEvRegenBrakeFunctionWarnStat(eventStat);
            setSFCRegen_SystemEventEvRegenBrakeFunctionWarnLinkedSoundID(linkedSoundID);
        }
    }

    void updateEventBrakeChcWarn() {
        std::string prevEventID;
        std::string eventID;
        SFCRegen_SystemEventEvRegenBrakeChcWarnStat eventStat = SFCRegen_SystemEventEvRegenBrakeChcWarnStat::OFF;

        if (mIsIgnOn == true) {
            if (mBrakeChcWarnStatus == BrakeChcWarnStatus::CHC_ON_RESTORED) {
                eventID = "E22414";
                eventStat = SFCRegen_SystemEventEvRegenBrakeChcWarnStat::ON;
            } else if (mBrakeChcWarnStatus == BrakeChcWarnStatus::CHC_ON_LOWTEMP) {
                eventID = "E22415";
                eventStat = SFCRegen_SystemEventEvRegenBrakeChcWarnStat::ON;
            } else if (mBrakeChcWarnStatus == BrakeChcWarnStatus::CHC_OFF_BRAKEPAD) {
                eventID = "E22416";
                eventStat = SFCRegen_SystemEventEvRegenBrakeChcWarnStat::ON;
            } else if (mBrakeChcWarnStatus == BrakeChcWarnStatus::CHC_OFF_INTERVNETION) {
                eventID = "E22417";
                eventStat = SFCRegen_SystemEventEvRegenBrakeChcWarnStat::ON;
            } else {
                // nothing
            }
        }

        GETCACHEDVALUE(SFC.Regen_System.Event.EvRegenBrakeChcWarn.ID, prevEventID);
        if ((prevEventID.empty() == false) && (prevEventID != eventID)) {
            setSFCRegen_SystemEventEvRegenBrakeChcWarnStat(SFCRegen_SystemEventEvRegenBrakeChcWarnStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCRegen_SystemEventEvRegenBrakeChcWarnID(eventID);
            setSFCRegen_SystemEventEvRegenBrakeChcWarnStat(eventStat);
        }
    }

    void updateEventBrakePaddleNotActiveWarn() {
        std::string prevEventID;
        std::string eventID;

        if (mBrakePaddleNotActive == BrakePaddleNotActive::WARN && mIsIgnOn) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) {
                eventID = "E22402";
            } else if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::HEV ||
                       mInter_ConfigVehicleType == Inter_ConfigVehicleType::PHEV) {
                eventID = "E22408";
            } else {
                // nothing
            }
        }

        GETCACHEDVALUE(SFC.Regen_System.Event.EvRegenBrakePaddleNotActiveWarn.ID, prevEventID);
        if ((prevEventID.empty() == false) && (prevEventID != eventID)) {
            setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnStat(
                SFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnID(eventID);
            setSFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnStat(
                SFCRegen_SystemEventEvRegenBrakePaddleNotActiveWarnStat::ON);
        }
    }

    void updateEventFailWarn() {
        SFCRegen_SystemEventEvSmartRegenFailWarnStat eventStat = SFCRegen_SystemEventEvSmartRegenFailWarnStat::OFF;
        if (mIsIgnOn == true && mFailWarnStatus == FailWarnStatus::ON) {
            eventStat = SFCRegen_SystemEventEvSmartRegenFailWarnStat::ON;
        }
        setSFCRegen_SystemEventEvSmartRegenFailWarnStat(eventStat);
    }

    void updateEventIPedalWarn() {
        std::string prevEventID;
        std::string eventID;
        std::string linkedSoundID;
        SFCRegen_SystemEventIpedalWarnStatusStat eventStat = SFCRegen_SystemEventIpedalWarnStatusStat::OFF;

        if (mIsIgnOn == true) {
            if (mIPedalWarnStatus == IPedalWarnStatus::CONDITION_NOT_MET) {
                eventID = "E22407";
                linkedSoundID = "SND_PopUpWarn1";
                eventStat = SFCRegen_SystemEventIpedalWarnStatusStat::ON;
            } else if (mIPedalWarnStatus == IPedalWarnStatus::IPEDAL_ON_BYSPEED) {
                eventID = "E22405";
                linkedSoundID = "SND_PopUpInform1";
                eventStat = SFCRegen_SystemEventIpedalWarnStatusStat::ON;
            } else if (mIPedalWarnStatus == IPedalWarnStatus::SMARTREGEN_ON_BYSPEED) {
                eventID = "E22412";
                linkedSoundID = "SND_PopUpInform1";
                eventStat = SFCRegen_SystemEventIpedalWarnStatusStat::ON;
            } else if (mIPedalWarnStatus == IPedalWarnStatus::SMARTREGEN_OFF_BYSPEED) {
                eventID = "E22413";
                linkedSoundID = "SND_PopUpInform1";
                eventStat = SFCRegen_SystemEventIpedalWarnStatusStat::ON;
            } else {
                // nothing
            }
        }

        GETCACHEDVALUE(SFC.Regen_System.Event.IpedalWarnStatus.ID, prevEventID);
        if ((prevEventID.empty() == false) && (prevEventID != eventID)) {
            setSFCRegen_SystemEventIpedalWarnStatusStat(SFCRegen_SystemEventIpedalWarnStatusStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCRegen_SystemEventIpedalWarnStatusID(eventID);
            setSFCRegen_SystemEventIpedalWarnStatusStat(eventStat);
            setSFCRegen_SystemEventIpedalWarnStatusLinkedSoundID(linkedSoundID);
        }
    }

    void updateEventPaddleStopAssistControlWarn() {
        SFCRegen_SystemEventPaddleStopAssistControlWarnStatusStat eventStat =
            SFCRegen_SystemEventPaddleStopAssistControlWarnStatusStat::OFF;
        if (mIsIgnOn == true && mPaddleStopAssistControlWarnStatus == PaddleStopAssistControlWarnStatus::CONDITION_NOT_MET) {
            eventStat = SFCRegen_SystemEventPaddleStopAssistControlWarnStatusStat::ON;
        }
        setSFCRegen_SystemEventPaddleStopAssistControlWarnStatusStat(eventStat);
    }

    // EXNCP-17752, 사양서상 5가지 입력 중 하나라도 TIMEOUT이면 Don't care 입력이라 하더라도 모두 DISPLAY_OFF 되도록 함.
    inline HBool checkNoTimeoutSignalAndIgnOn() const {
        return mRegenVehicleStopStatus != RegenVehicleStopStatus::UNHANDLED_TIMEOUT &&
               mRegenSmartVehicleStatus1 != RegenSmartVehicleStatus1::UNHANDLED_TIMEOUT &&
               mRegenSmartVehicleStatus2 != RegenSmartVehicleStatus2::UNHANDLED_TIMEOUT &&
               mRegenSmartOnReqStatus != RegenSmartOnReqStatus::UNHANDLED_TIMEOUT &&
               mRegenSmartLevelValue != kRegenSmartLevelTimeoutValue && mIsIgnOn;
    }

    using Inter_ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using Inter_ConfigIPedalType = ArgumentsConfigIPedalTypeChanged::Inter_ConfigIPedalType;
    using IPedalIndiReqStatus = ArgumentsIPedalIndiReqStatusChanged::Input_IPedalIndiReqStatus;
    using IPedalWarnStatus = ArgumentsIPedalWarnStatusChanged::Input_IPedalWarnStatus;
    using RegenVehicleStopStatus = ArgumentsSmartRegenSystemChanged::Input_RegenVehicleStopStatus;
    using RegenSmartVehicleStatus1 = ArgumentsSmartRegenSystemChanged::Input_RegenSmartVehicleStatus1;
    using RegenSmartVehicleStatus2 = ArgumentsSmartRegenSystemChanged::Input_RegenSmartVehicleStatus2;
    using RegenSmartOnReqStatus = ArgumentsSmartRegenSystemChanged::Input_RegenSmartOnReqStatus;
    using BrakeFunctionWarnStatus = ArgumentsBrakeFunctionWarnStatusChanged::Input_EvRegenBrakeFunctionWarnStatus;
    using BrakeChcWarnStatus = ArgumentsBrakeChcWarnStatusChanged::Input_EvRegenBrakeChcWarnStatus;
    using BrakePaddleNotActive = ArgumentsBrakePaddleNotActiveWarnStatusChanged::Input_EvRegenBrakePaddleNotActiveWarnStatus;
    using FailWarnStatus = ArgumentsFailWarnStatusChanged::Input_EvSmartRegenFailWarnStatus;
    using NpedalLvlStatus = ArgumentsNPedalLvlStatusChanged::Input_RegenNpedalLvlStatus;
    using PaddleStopAssistControlWarnStatus =
        ArgumentsPaddleStopAssistControlWarnStatusChanged::Input_PaddleStopAssistControlWarnStatus;

    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;
    Inter_ConfigIPedalType mInter_ConfigIPedalType = Inter_ConfigIPedalType::GEN_FIRST;
    IPedalIndiReqStatus mIPedalIndiReqStatus = IPedalIndiReqStatus::OFF;
    IPedalWarnStatus mIPedalWarnStatus = IPedalWarnStatus::OFF;
    RegenVehicleStopStatus mRegenVehicleStopStatus = RegenVehicleStopStatus::OFF;
    RegenSmartVehicleStatus1 mRegenSmartVehicleStatus1 = RegenSmartVehicleStatus1::OFF;
    RegenSmartVehicleStatus2 mRegenSmartVehicleStatus2 = RegenSmartVehicleStatus2::OFF;
    RegenSmartOnReqStatus mRegenSmartOnReqStatus = RegenSmartOnReqStatus::OFF;
    BrakeFunctionWarnStatus mBrakeFunctionWarnStatus = BrakeFunctionWarnStatus::NO_WARN;
    BrakeChcWarnStatus mBrakeChcWarnStatus = BrakeChcWarnStatus::NO_WARN;
    BrakePaddleNotActive mBrakePaddleNotActive = BrakePaddleNotActive::NO_WARN;
    FailWarnStatus mFailWarnStatus = FailWarnStatus::OFF;
    NpedalLvlStatus mNpedalLvlStatus = NpedalLvlStatus::DISPLAY_OFF;
    PaddleStopAssistControlWarnStatus mPaddleStopAssistControlWarnStatus = PaddleStopAssistControlWarnStatus::OFF;
    HBool mIsIgnOn = false;
    HBool mIsNpedalLvlStatusTimeout = false;
    HUInt64 mRegenSmartLevelValue = 63;

    static constexpr HUInt64 kRegenSmartLevelTimeoutValue = 0xFF;
};

}  // namespace ccos

#endif  // SFSS_Regen_System_H
