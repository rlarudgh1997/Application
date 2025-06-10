/**
 * @file EV_Battery_Warning.hpp
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
#ifndef SFSS_EV_Battery_Warning_H
#define SFSS_EV_Battery_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "EV_Battery_WarningBase.hpp"

namespace ccos {

// SFC Version : 9.0.0
// Reference : [PT160] EV_Battery_Warning . Contains : Telltale, Constant, Event . Recommend: VALUE_CHANGED
class EV_Battery_Warning : public EV_Battery_WarningBase {
public:
    EV_Battery_Warning() = default;
    ~EV_Battery_Warning() override = default;
    EV_Battery_Warning(const EV_Battery_Warning& other) = delete;
    EV_Battery_Warning(EV_Battery_Warning&& other) noexcept = delete;
    EV_Battery_Warning& operator=(const EV_Battery_Warning& other) = delete;
    EV_Battery_Warning& operator=(EV_Battery_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEV_Battery_WarningTelltaleEvBatWarnStat(SFCEV_Battery_WarningTelltaleEvBatWarnStat::OFF);
        setSFCEV_Battery_WarningEventEvBatteryLowWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Battery_WarningEventEvBatteryLowWarnLinkedSoundType(
            SFCEV_Battery_WarningEventEvBatteryLowWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventEvBatteryLowWarnLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnID("E21303");
        setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnLinkedSoundType(
            SFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnID("E21304");
        setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnLinkedSoundType(
            SFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCEV_Battery_WarningEventEvBatteryHighTempWarnID("E21305");
        setSFCEV_Battery_WarningEventEvBatteryHighTempWarnLinkedSoundID("SND_PopUpWarn2");
        setSFCEV_Battery_WarningEventEvBatteryHighTempWarnLinkedSoundType(
            SFCEV_Battery_WarningEventEvBatteryHighTempWarnLinkedSoundType::DURATION_MINMAX);
        setSFCEV_Battery_WarningEventEvBatteryHighTempWarnLinkedSoundDurationMin(kEventDurationMin);
        setSFCEV_Battery_WarningEventEvBatteryHighTempWarnLinkedSoundDurationMax(kEventDurationMax);
        setSFCEV_Battery_WarningEventAuxBatteryLowWarnLinkedSoundID("SND_PopUpWarn2");
        setSFCEV_Battery_WarningEventAuxBatteryLowWarnLinkedSoundType(
            SFCEV_Battery_WarningEventAuxBatteryLowWarnLinkedSoundType::DURATION_MINMAX);
        setSFCEV_Battery_WarningEventAuxBatteryLowWarnLinkedSoundDurationMin(kEventDurationMin);
        setSFCEV_Battery_WarningEventAuxBatteryLowWarnLinkedSoundDurationMax(kEventDurationMax);
        setSFCEV_Battery_WarningEventEvBatteryConditioningOperLinkedSoundID("SND_PopUpInform1");
        setSFCEV_Battery_WarningEventEvBatteryConditioningOperLinkedSoundType(
            SFCEV_Battery_WarningEventEvBatteryConditioningOperLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventEvBatteryConditioningOperLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusLinkedSoundType(
            SFCEV_Battery_WarningEventEvBatteryPreHeatStatusLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCEV_Battery_WarningEventPreventOverheatModeStatusLinkedSoundType(
            SFCEV_Battery_WarningEventPreventOverheatModeStatusLinkedSoundType::REPEAT_COUNT);
        setSFCEV_Battery_WarningEventPreventOverheatModeStatusLinkedSoundRepeatCount(kEventRepeatCount);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstantEvBatteryConditioningSymbol();
        updateConstantEVPowerSaveModeStatus();
        updateTelltale();
        updateEventEvBatteryLowWarn();
        updateEventEvPowerLimitByBatTempWarn();
        updateEventEvPowerLimitByOutTempWarn();
        updateEventEvBatteryHighTempWarn();
        updateEventAuxBatteryLowWarn();
        updateEventEvBatteryConditioningOper();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstantEvBatteryConditioningSymbol();
        updateConstantEVPowerSaveModeStatus();
        updateTelltale();
        updateEventEvBatteryLowWarn();
        updateEventEvPowerLimitByBatTempWarn();
        updateEventEvPowerLimitByOutTempWarn();
        updateEventEvBatteryHighTempWarn();
        updateEventAuxBatteryLowWarn();
        updateEventEvBatteryConditioningOper();
    }

    void onTelltaleVsmChanged(const ArgumentsTelltaleVsmChanged& args) {
        if (ISTIMEOUT(args.mInput_EvLowSocWarnStatus) == true) {
            mEvLowSocWarnStatus = EvLowSocWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mEvLowSocWarnStatus = args.mInput_EvLowSocWarnStatus;
        }

        updateTelltale();
    }

    void onEvBatteryLowWarnChanged(const ArgumentsEvBatteryLowWarnChanged& args) {
        mEvBatteryLowWarnStatus = args.mInput_EvBatteryLowWarnStatus;
        updateEventEvBatteryLowWarn();
    }

    void onEvPowerLimitByBatTempWarnChanged(const ArgumentsEvPowerLimitByBatTempWarnChanged& args) {
        mEvPowerLimitByBatTempWarnStatus = args.mInput_EvPowerLimitByBatTempWarnStatus;
        updateEventEvPowerLimitByBatTempWarn();
    }

    void onEvPowerLimitByOutTempWarnChanged(const ArgumentsEvPowerLimitByOutTempWarnChanged& args) {
        mEvPowerLimitByOutTempWarnStatus = args.mInput_EvPowerLimitByOutTempWarnStatus;
        updateEventEvPowerLimitByOutTempWarn();
    }

    void onEvBatteryHighTempWarnChanged(const ArgumentsEvBatteryHighTempWarnChanged& args) {
        mEvBatteryHighTempWarnStatus = args.mInput_EvBatteryHighTempWarnStatus;
        updateEventEvBatteryHighTempWarn();
    }

    void onAuxBatteryLowWarnChanged(const ArgumentsAuxBatteryLowWarnChanged& args) {
        mAuxBatteryLowWarnStatus = args.mInput_AuxBatteryLowWarnStatus;
        updateEventAuxBatteryLowWarn();
    }

    void onEvBatteryConditioningOperChanged(const ArgumentsEvBatteryConditioningOperChanged& args) {
        mEvBatteryConditioningOperStatus = args.mInput_EvBatteryConditioningOperStatus;
        updateEventEvBatteryConditioningOper();
    }

    void onEvBatteryConditioningSymbolChanged(const ArgumentsEvBatteryConditioningSymbolChanged& args) {
        mEvBatteryConditioningSymbolstatus = args.mInput_EvBatteryConditioningSymbolstatus;
        updateConstantEvBatteryConditioningSymbol();
    }

    void onEvBatteryPreHeatMaxTimeValueChanged(const ArgumentsEvBatteryPreHeatMaxTimeValueChanged& args) {
        mEvBatteryPreHeatMaxTimeValue = args.mInput_EvBatteryPreHeatMaxTimeValue;
        updateConstantEvBatteryPreHeatMaxTime();
    }

    void onEvBatteryPreHeatStatusChanged(const ArgumentsEvBatteryPreHeatStatusChanged& args) {
        mEvBatteryPreHeatStatus = args.mInput_EvBatteryPreHeatStatus;
        updateEventEvBatteryPreHeatStatus();
    }

    void onPreventOverheatModeStatusChanged(const ArgumentsPreventOverheatModeStatusChanged& args) {
        mPreventOverheatModeStatus = args.mInput_PreventOverheatModeStatus;
        updateEventPreventOverheatModeStatus();
    }

    void onPwrSavModeStatusChanged(const ArgumentsPwrSavModeStatusChanged& args) {
        mPwrSavModeStatus = args.mInput_PwrSavModeStatus;
        updateConstantEVPowerSaveModeStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms &&
            (mEvLowSocWarnStatus == EvLowSocWarnStatus::WARN1 || mEvLowSocWarnStatus == EvLowSocWarnStatus::WARN2 ||
             mEvLowSocWarnStatus == EvLowSocWarnStatus::WARN3 || mEvLowSocWarnStatus == EvLowSocWarnStatus::SOC_UNAVAILABLE)) {
            setSFCEV_Battery_WarningTelltaleEvBatWarnStat(SFCEV_Battery_WarningTelltaleEvBatWarnStat::ON);
        } else {
            setSFCEV_Battery_WarningTelltaleEvBatWarnStat(SFCEV_Battery_WarningTelltaleEvBatWarnStat::OFF);
        }
    }

    void updateConstantEvBatteryConditioningSymbol() {
        SFCEV_Battery_WarningConstantEvBatConSymbolStat stat = SFCEV_Battery_WarningConstantEvBatConSymbolStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEvBatteryConditioningSymbolstatus == EvBatteryConditioningSymbolstatus::ON) {
                stat = SFCEV_Battery_WarningConstantEvBatConSymbolStat::ON;
            } else if (mEvBatteryConditioningSymbolstatus == EvBatteryConditioningSymbolstatus::ON_COOLING) {
                stat = SFCEV_Battery_WarningConstantEvBatConSymbolStat::ON_COOLING;
            } else {
                // no operation
            }
        }

        setSFCEV_Battery_WarningConstantEvBatConSymbolStat(stat);
    }

    void updateConstantEvBatteryPreHeatMaxTime() {
        if (mEvBatteryPreHeatMaxTimeValue >= 0 && mEvBatteryPreHeatMaxTimeValue <= 254) {
            setSFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeValue(mEvBatteryPreHeatMaxTimeValue);
            setSFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeStat(
                SFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeStat::VALUE);
        } else {
            setSFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeValue(255);
            setSFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeStat(
                SFCEV_Battery_WarningConstantEvBatteryPreHeatMaxTimeStat::UNMEASURABLE);
        }
    }

    void updateEventEvBatteryLowWarn() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEvBatteryLowWarnStatus == EvBatteryLowWarnStatus::WARN_LV1) {
                if (mOnceFlag == false) {
                    mOnceFlag = true;
                    eventID = "E21301";
                }
            } else if (mEvBatteryLowWarnStatus == EvBatteryLowWarnStatus::WARN_LV2) {
                eventID = "E21302";
            } else {
                // nothing
            }
        }
        if (mIgnElapsed != IgnElapsed::ON_0ms && mIgnElapsed != IgnElapsed::ON_500ms) {
            mOnceFlag = false;
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Battery_Warning.Event.EvBatteryLowWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Battery_WarningEventEvBatteryLowWarnStat(SFCEV_Battery_WarningEventEvBatteryLowWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Battery_WarningEventEvBatteryLowWarnID(eventID);
            setSFCEV_Battery_WarningEventEvBatteryLowWarnStat(SFCEV_Battery_WarningEventEvBatteryLowWarnStat::ON);
        }
    }

    void updateEventEvPowerLimitByBatTempWarn() {
        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) &&
            mEvPowerLimitByBatTempWarnStatus == EvPowerLimitByBatTempWarnStatus::ON) {
            setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnStat(
                SFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnStat::ON);
        } else {
            setSFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnStat(
                SFCEV_Battery_WarningEventEvPowerLimitByBatTempWarnStat::OFF);
        }
    }

    void updateEventEvPowerLimitByOutTempWarn() {
        if (mIgnElapsed == IgnElapsed::OFF_0ms && mEvPowerLimitByOutTempWarnStatus == EvPowerLimitByOutTempWarnStatus::ON) {
            setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnStat(
                SFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnStat::ON);
        } else {
            setSFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnStat(
                SFCEV_Battery_WarningEventEvPowerLimitByOutTempWarnStat::OFF);
        }
    }

    void updateEventEvBatteryHighTempWarn() {
        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) &&
            mEvBatteryHighTempWarnStatus == EvBatteryHighTempWarnStatus::ON) {
            setSFCEV_Battery_WarningEventEvBatteryHighTempWarnStat(SFCEV_Battery_WarningEventEvBatteryHighTempWarnStat::ON);
        } else {
            setSFCEV_Battery_WarningEventEvBatteryHighTempWarnStat(SFCEV_Battery_WarningEventEvBatteryHighTempWarnStat::OFF);
        }
    }

    void updateEventAuxBatteryLowWarn() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mAuxBatteryLowWarnStatus == AuxBatteryLowWarnStatus::WARN_LV1) {
                eventID = "E21306";
            } else if (mAuxBatteryLowWarnStatus == AuxBatteryLowWarnStatus::WARN_LV2) {
                eventID = "E21307";
            } else {
                // nothing
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Battery_Warning.Event.AuxBatteryLowWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Battery_WarningEventAuxBatteryLowWarnStat(SFCEV_Battery_WarningEventAuxBatteryLowWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Battery_WarningEventAuxBatteryLowWarnID(eventID);
            setSFCEV_Battery_WarningEventAuxBatteryLowWarnStat(SFCEV_Battery_WarningEventAuxBatteryLowWarnStat::ON);
        }
    }

    void updateEventEvBatteryConditioningOper() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEvBatteryConditioningOperStatus == EvBatteryConditioningOperStatus::ACT_WINTER_MODE) {
                eventID = "E21308";
            } else if (mEvBatteryConditioningOperStatus == EvBatteryConditioningOperStatus::ACT_PRE) {
                eventID = "E21309";
            } else if (mEvBatteryConditioningOperStatus == EvBatteryConditioningOperStatus::COMPLETE) {
                eventID = "E21310";
            } else if (mEvBatteryConditioningOperStatus == EvBatteryConditioningOperStatus::FIN_LOWSOC) {
                eventID = "E21313";
            } else {
                // nothing
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Battery_Warning.Event.EvBatteryConditioningOper.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Battery_WarningEventEvBatteryConditioningOperStat(
                SFCEV_Battery_WarningEventEvBatteryConditioningOperStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Battery_WarningEventEvBatteryConditioningOperID(eventID);
            setSFCEV_Battery_WarningEventEvBatteryConditioningOperStat(
                SFCEV_Battery_WarningEventEvBatteryConditioningOperStat::ON);
        }
    }

    void updateEventEvBatteryPreHeatStatus() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mEvBatteryPreHeatStatus == EvBatteryPreHeatStatus::HEATING_ON) {
                eventID = "E21311";
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mEvBatteryPreHeatStatus == EvBatteryPreHeatStatus::HEATING_FINISH) {
                eventID = "E21312";
                linkedSoundID = "SND_PopUpInform2";
            } else {
                // nothing
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Battery_Warning.Event.EvBatteryPreHeatStatus.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusStat(SFCEV_Battery_WarningEventEvBatteryPreHeatStatusStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusID(eventID);
            setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusLinkedSoundID(linkedSoundID);
            setSFCEV_Battery_WarningEventEvBatteryPreHeatStatusStat(SFCEV_Battery_WarningEventEvBatteryPreHeatStatusStat::ON);
        }
    }

    void updateEventPreventOverheatModeStatus() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mPreventOverheatModeStatus == PreventOverheatModeStatus::MODE_ON) {
                eventID = "E21314";
                linkedSoundID = "SND_PopUpInform1";
            } else if (mPreventOverheatModeStatus == PreventOverheatModeStatus::MODE_COMPLETE) {
                eventID = "E21315";
                linkedSoundID = "SND_PopUpInform1";
            } else if (mPreventOverheatModeStatus == PreventOverheatModeStatus::CHANGE_GEAR) {
                eventID = "E21316";
                linkedSoundID = "SND_PopUpWarn1";
            } else {
                // nothing
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EV_Battery_Warning.Event.PreventOverheatModeStatus.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEV_Battery_WarningEventPreventOverheatModeStatusStat(
                SFCEV_Battery_WarningEventPreventOverheatModeStatusStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEV_Battery_WarningEventPreventOverheatModeStatusID(eventID);
            setSFCEV_Battery_WarningEventPreventOverheatModeStatusLinkedSoundID(linkedSoundID);
            setSFCEV_Battery_WarningEventPreventOverheatModeStatusStat(
                SFCEV_Battery_WarningEventPreventOverheatModeStatusStat::ON);
        }
    }

    void updateConstantEVPowerSaveModeStatus() {
        SFCEV_Battery_WarningConstantEvPowerSaveModeStat stat = SFCEV_Battery_WarningConstantEvPowerSaveModeStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mPwrSavModeStatus == PwrSavModeStatus::ON_SAVE) {
                stat = SFCEV_Battery_WarningConstantEvPowerSaveModeStat::ON_SAVEMODE;
            }
        }
        setSFCEV_Battery_WarningConstantEvPowerSaveModeStat(stat);
    }

    static constexpr HUInt64 kEventRepeatCount = 1;
    static constexpr HUInt64 kEventDurationMin = 0;
    static constexpr HUInt64 kEventDurationMax = 10000;
    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using EvLowSocWarnStatus = ArgumentsTelltaleVsmChanged::Input_EvLowSocWarnStatus;
    using EvBatteryLowWarnStatus = ArgumentsEvBatteryLowWarnChanged::Input_EvBatteryLowWarnStatus;
    using EvPowerLimitByBatTempWarnStatus = ArgumentsEvPowerLimitByBatTempWarnChanged::Input_EvPowerLimitByBatTempWarnStatus;
    using EvPowerLimitByOutTempWarnStatus = ArgumentsEvPowerLimitByOutTempWarnChanged::Input_EvPowerLimitByOutTempWarnStatus;
    using EvBatteryHighTempWarnStatus = ArgumentsEvBatteryHighTempWarnChanged::Input_EvBatteryHighTempWarnStatus;
    using AuxBatteryLowWarnStatus = ArgumentsAuxBatteryLowWarnChanged::Input_AuxBatteryLowWarnStatus;
    using EvBatteryConditioningOperStatus = ArgumentsEvBatteryConditioningOperChanged::Input_EvBatteryConditioningOperStatus;
    using EvBatteryConditioningSymbolstatus =
        ArgumentsEvBatteryConditioningSymbolChanged::Input_EvBatteryConditioningSymbolstatus;
    using EvBatteryPreHeatStatus = ArgumentsEvBatteryPreHeatStatusChanged::Input_EvBatteryPreHeatStatus;
    using PreventOverheatModeStatus = ArgumentsPreventOverheatModeStatusChanged::Input_PreventOverheatModeStatus;
    using PwrSavModeStatus = ArgumentsPwrSavModeStatusChanged::Input_PwrSavModeStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mOnceFlag = false;
    EvLowSocWarnStatus mEvLowSocWarnStatus = EvLowSocWarnStatus::OFF;
    EvBatteryLowWarnStatus mEvBatteryLowWarnStatus = EvBatteryLowWarnStatus::OFF;
    EvPowerLimitByBatTempWarnStatus mEvPowerLimitByBatTempWarnStatus = EvPowerLimitByBatTempWarnStatus::OFF;
    EvPowerLimitByOutTempWarnStatus mEvPowerLimitByOutTempWarnStatus = EvPowerLimitByOutTempWarnStatus::OFF;
    EvBatteryHighTempWarnStatus mEvBatteryHighTempWarnStatus = EvBatteryHighTempWarnStatus::OFF;
    AuxBatteryLowWarnStatus mAuxBatteryLowWarnStatus = AuxBatteryLowWarnStatus::OFF;
    EvBatteryConditioningOperStatus mEvBatteryConditioningOperStatus = EvBatteryConditioningOperStatus::OFF;
    EvBatteryConditioningSymbolstatus mEvBatteryConditioningSymbolstatus = EvBatteryConditioningSymbolstatus::OFF;
    HUInt64 mEvBatteryPreHeatMaxTimeValue = 0;
    EvBatteryPreHeatStatus mEvBatteryPreHeatStatus = EvBatteryPreHeatStatus::OFF;
    PreventOverheatModeStatus mPreventOverheatModeStatus = PreventOverheatModeStatus::OFF;
    PwrSavModeStatus mPwrSavModeStatus = PwrSavModeStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_EV_Battery_Warning_H
