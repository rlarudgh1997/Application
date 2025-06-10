/**
 * @file ICC.hpp
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
#ifndef SFSS_ICC_H
#define SFSS_ICC_H

#define DLOG_ENABLED gEnableSFCLog

#include "ICCBase.hpp"

namespace ccos {

// SFC Version : 8.0.0
// Reference : [AV080] ICC. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class ICC : public ICCBase {
public:
    ICC() = default;
    ~ICC() override = default;
    ICC(const ICC& other) = delete;
    ICC(ICC&& other) noexcept = delete;
    ICC& operator=(const ICC& other) = delete;
    ICC& operator=(ICC&& other) noexcept = delete;

    void onInitialize() override {
        setSFCICCSoundIccWarningID("SND_ICC_PopUpWarn");
        setSFCICCSoundIccWarningType(SFCICCSoundIccWarningType::INFINITE);

        setSFCICCSoundIccWarning2ID("SND_ICC_PopUpWarn2");
        setSFCICCSoundIccWarning2Type(SFCICCSoundIccWarning2Type::INFINITE);

        setSFCICCSoundIccVolAutoWarnID("SND_ICC_VolAutoWarn");
        setSFCICCSoundIccVolAutoWarnType(SFCICCSoundIccVolAutoWarnType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        isIgnOn = true;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        isIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        isIgnOn = true;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        isIgnOn = false;
        updateAll();
    }

    void onIccWarningStatus(const ArgumentsIccWarningStatus& args) {
        mIccWarnStatus = args.mInput_IccWarningStatus;
        updateEvent();
    }

    void onIccWarningSmblDrowStatusChanged(const ArgumentsIccWarningSmblDrowStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_IccWarningSmblDrowStatus) == true) {
            mIccWarningSmblDrowStatus = IccWarningSmblDrowStatus::UNHANDLED_TIMEOUT;
        } else {
            mIccWarningSmblDrowStatus = args.mInput_IccWarningSmblDrowStatus;
        }
        updateInterTelltaleIccDrow();
    }

    void onIccWarningSmblDistStatusChanged(const ArgumentsIccWarningSmblDistStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_IccWarningSmblDistStatus) == true) {
            mIccWarningSmblDistStatus = IccWarningSmblDistStatus::UNHANDLED_TIMEOUT;
        } else {
            mIccWarningSmblDistStatus = args.mInput_IccWarningSmblDistStatus;
        }

        if (ISTIMEOUT(args.mInput_IccWarningSmblDistExtStatus) == true) {
            mIccWarningSmblDistExtStatus = IccWarningSmblDistExtStatus::UNHANDLED_TIMEOUT;
        } else {
            mIccWarningSmblDistExtStatus = args.mInput_IccWarningSmblDistExtStatus;
        }
        mConfigICC = args.mInter_ConfigICC;
        updateTelltale();
    }

    void onIccWarningSoundStatusChanged(const ArgumentsIccWarningSoundStatusChanged& args) {
        mIccWarningSoundStatus = args.mInput_IccWarningSoundStatus;
        updateSound();
    }

    void onIccWarningSound2StatusChanged(const ArgumentsIccWarningSound2StatusChanged& args) {
        mIccWarningSound2Status = args.mInput_IccWarningSound2Status;
        updateSound();
    }

    void onIccWarningAutoSoundStatusChanged(const ArgumentsIccWarningAutoSoundStatusChanged& args) {
        mIccWarningAutoSoundStatus = args.mInput_IccWarningAutoSoundStatus;
        updateSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateInterTelltaleIccDrow();
        updateTelltale();
        updateEvent();
        updateSound();
    }

    void updateInterTelltaleIccDrow() {
        SFCICCInter_TelltaleIccDrow stat = SFCICCInter_TelltaleIccDrow::OFF;
        if (mIccWarningSmblDrowStatus == IccWarningSmblDrowStatus::ON && isIgnOn == true) {
            stat = SFCICCInter_TelltaleIccDrow::ON;
        }
        setSFCICCInter_TelltaleIccDrow(stat);
    }

    void updateTelltale() {
        SFCICCTelltaleIccDistractionStat stat = SFCICCTelltaleIccDistractionStat::OFF;
        if (mConfigICC == ConfigICC::ON && mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCICCTelltaleIccDistractionStat::RED;
        } else if (mIccWarningSmblDistStatus == IccWarningSmblDistStatus::UNHANDLED_TIMEOUT && mConfigICC == ConfigICC::ON &&
                   mIgnElapsed == IgnElapsed::ON_3500ms) {
            stat = SFCICCTelltaleIccDistractionStat::RED;
        } else if (mIccWarningSmblDistStatus == IccWarningSmblDistStatus::ON && mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mIccWarningSmblDistExtStatus == IccWarningSmblDistExtStatus::OFF ||
                mIccWarningSmblDistExtStatus == IccWarningSmblDistExtStatus::UNHANDLED_TIMEOUT) {
                stat = SFCICCTelltaleIccDistractionStat::RED;
            } else if (mIccWarningSmblDistExtStatus == IccWarningSmblDistExtStatus::ON) {
                stat = SFCICCTelltaleIccDistractionStat::AMBER;
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCICCTelltaleIccDistractionStat(stat);
    }

    void updateEvent() {
        std::string eventID;
        std::string linkedSoundID;
        SFCICCEventIccWarnLinkedSoundType linkedSoundType = SFCICCEventIccWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        if (isIgnOn == true) {
            switch (mIccWarnStatus) {
                case IccWarnStatus::DDMS_WARN:
                    eventID = "E40800";
                    break;
                case IccWarnStatus::DISTRACTION_WARN:
                    eventID = "E40801";
                    break;
                case IccWarnStatus::DROWSINESS_WARN:
                    eventID = "E40802";
                    break;
                case IccWarnStatus::BLOCKAGE_WARN:
                    eventID = "E40803";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCICCEventIccWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case IccWarnStatus::FAILURE_WARN:
                    eventID = "E40804";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCICCEventIccWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case IccWarnStatus::DEGRADATION_WARN:
                    eventID = "E40805";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCICCEventIccWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case IccWarnStatus::DDMS_WARN2:
                    eventID = "E40806";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ICC.Event.IccWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCICCEventIccWarnStat(SFCICCEventIccWarnStat::OFF);  // Turn previous event off
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCICCEventIccWarnID(eventID);
            setSFCICCEventIccWarnStat(SFCICCEventIccWarnStat::ON);
            setSFCICCEventIccWarnLinkedSoundID(linkedSoundID);
            setSFCICCEventIccWarnLinkedSoundType(linkedSoundType);
            setSFCICCEventIccWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateSound() {
        if (mIccWarningSoundStatus == IccWarningSoundStatus::ON && isIgnOn == true) {
            setSFCICCSoundIccWarningStat(SFCICCSoundIccWarningStat::ON);
        } else {
            setSFCICCSoundIccWarningStat(SFCICCSoundIccWarningStat::OFF);
        }

        if (mIccWarningSound2Status == IccWarningSound2Status::ON && isIgnOn == true) {
            setSFCICCSoundIccWarning2Stat(SFCICCSoundIccWarning2Stat::ON);
        } else {
            setSFCICCSoundIccWarning2Stat(SFCICCSoundIccWarning2Stat::OFF);
        }

        if (mIccWarningAutoSoundStatus == IccWarningAutoSoundStatus::ON && isIgnOn == true) {
            setSFCICCSoundIccVolAutoWarnStat(SFCICCSoundIccVolAutoWarnStat::ON);
        } else {
            setSFCICCSoundIccVolAutoWarnStat(SFCICCSoundIccVolAutoWarnStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using IccWarnStatus = ArgumentsIccWarningStatus::Input_IccWarningStatus;
    using IccWarningSmblDrowStatus = ArgumentsIccWarningSmblDrowStatusChanged::Input_IccWarningSmblDrowStatus;
    using IccWarningSmblDistStatus = ArgumentsIccWarningSmblDistStatusChanged::Input_IccWarningSmblDistStatus;
    using IccWarningSmblDistExtStatus = ArgumentsIccWarningSmblDistStatusChanged::Input_IccWarningSmblDistExtStatus;
    using ConfigICC = ArgumentsIccWarningSmblDistStatusChanged::Inter_ConfigICC;
    using IccWarningSoundStatus = ArgumentsIccWarningSoundStatusChanged::Input_IccWarningSoundStatus;
    using IccWarningSound2Status = ArgumentsIccWarningSound2StatusChanged::Input_IccWarningSound2Status;
    using IccWarningAutoSoundStatus = ArgumentsIccWarningAutoSoundStatusChanged::Input_IccWarningAutoSoundStatus;

    IccWarnStatus mIccWarnStatus = IccWarnStatus::OFF;
    IccWarningSmblDrowStatus mIccWarningSmblDrowStatus = IccWarningSmblDrowStatus::OFF;
    IccWarningSmblDistStatus mIccWarningSmblDistStatus = IccWarningSmblDistStatus::OFF;
    IccWarningSmblDistExtStatus mIccWarningSmblDistExtStatus = IccWarningSmblDistExtStatus::OFF;
    IccWarningSoundStatus mIccWarningSoundStatus = IccWarningSoundStatus::OFF;
    IccWarningSound2Status mIccWarningSound2Status = IccWarningSound2Status::OFF;
    IccWarningAutoSoundStatus mIccWarningAutoSoundStatus = IccWarningAutoSoundStatus::OFF;
    ConfigICC mConfigICC = ConfigICC::OFF;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HBool isIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_ICC_H
