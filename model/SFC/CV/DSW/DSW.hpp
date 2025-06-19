/**
 * @file DSW.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_DSW_H
#define SFSS_DSW_H

#define DLOG_ENABLED gEnableSFCLog

#include "DSWBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Spec Version : v0.20
// Reference : [CV920] DSW. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class DSW : public DSWBase {
public:
    DSW() = default;
    ~DSW() override = default;
    DSW(const DSW& other) = delete;
    DSW(DSW&& other) noexcept = delete;
    DSW& operator=(const DSW& other) = delete;
    DSW& operator=(DSW&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDSWSoundDSWWarnID("SND_DSWWarn");
        setSFCDSWSoundDSWWarnType(SFCDSWSoundDSWWarnType::INFINITE);
        setSFCDSWSound2DSWWarnID("SND_DSWWarn2");
        setSFCDSWSound2DSWWarnType(SFCDSWSound2DSWWarnType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateAll();
    }

    void onDswWarnSymbolDrowStatusChanged(const ArgumentsDswWarnSymbolDrowStatusChanged& args) {
        mDSWWarnSymbolDrowStatus = args.mInput_DSWWarnSymbolDrowStatus;
        updateInterIccDrow();
    }

    void onDswWarnSymbolDistStatusChanged(const ArgumentsDswWarnSymbolDistStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_DSWWarnSymbolDistStatus) == true) {
            mDSWWarnSymbolDistStatus = DSWWarnSymbolDistStatus::UNHANDLED_TIMEOUT;
        } else {
            mDSWWarnSymbolDistStatus = args.mInput_DSWWarnSymbolDistStatus;
        }
        updateTelltale();
    }

    void onDswWarnStatusChanged(const ArgumentsDswWarnStatusChanged& args) {
        mDSWWarnStatus = args.mInput_DSWWarnStatus;
        updateEvent();
    }

    void onDswWarnSoundStatusChanged(const ArgumentsDswWarnSoundStatusChanged& args) {
        mDSWWarnSoundStatus = args.mInput_DSWWarnSoundStatus;
        updateSound();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigDSW = args.mInter_ConfigDSW;
        updateAll();
    }

    void onDswWarnSound2StatusChanged(const ArgumentsDswWarnSound2StatusChanged& args) {
        mDSWWarnSound2Status = args.mInput_DSWWarnSound2Status;
        updateSound2();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateInterIccDrow();
        updateTelltale();
        updateEvent();
        updateSound();
        updateSound2();
    }

    void updateInterIccDrow() {
        if (mConfigDSW != ConfigDSW::ON) {
            return;
        }

        SFCDSWInter_TelltaleIccDrow stat = SFCDSWInter_TelltaleIccDrow::OFF;

        if (mIsIgnOn == true && mDSWWarnSymbolDrowStatus == DSWWarnSymbolDrowStatus::ON) {
            stat = SFCDSWInter_TelltaleIccDrow::ON;
        }

        setSFCDSWInter_TelltaleIccDrow(stat);
    }

    void updateTelltale() {
        if (mConfigDSW != ConfigDSW::ON) {
            return;
        }

        SFCDSWTelltaleICCStat stat = SFCDSWTelltaleICCStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            // Telltale OFF
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCDSWTelltaleICCStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mDSWWarnSymbolDistStatus == DSWWarnSymbolDistStatus::ON ||
                mDSWWarnSymbolDistStatus == DSWWarnSymbolDistStatus::UNHANDLED_TIMEOUT) {
                stat = SFCDSWTelltaleICCStat::ON;
            }
        } else {
            // Telltale OFF
        }

        setSFCDSWTelltaleICCStat(stat);
    }

    void updateEvent() {
        if (mConfigDSW != ConfigDSW::ON) {
            return;
        }

        std::string eventId;
        std::string linkedSoundId;
        SFCDSWEventDSWWarnStatusLinkedSoundType linkedSoundType = SFCDSWEventDSWWarnStatusLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIsIgnOn == true) {
            switch (mDSWWarnStatus) {
                case DSWWarnStatus::DISTRACTION_WARN:
                    eventId = "E79201";
                    linkedSoundId = "SND_PopUpWarn1";
                    linkedSoundType = SFCDSWEventDSWWarnStatusLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case DSWWarnStatus::BLOCKAGE_WARN:
                    eventId = "E79203";
                    linkedSoundId = "SND_PopUpWarn1";
                    linkedSoundType = SFCDSWEventDSWWarnStatusLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case DSWWarnStatus::FAILURE_WARN:
                    eventId = "E79204";
                    linkedSoundId = "SND_PopUpWarn1";
                    linkedSoundType = SFCDSWEventDSWWarnStatusLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case DSWWarnStatus::DEGRADATION_WARN:
                    eventId = "E79205";
                    linkedSoundId = "SND_PopUpWarn1";
                    linkedSoundType = SFCDSWEventDSWWarnStatusLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case DSWWarnStatus::DDMS_WARN:
                    eventId = "E79206";
                    break;
                default:
                    // no operation
                    break;
            }
        }

        std::string prevEventId;
        GETCACHEDVALUE(SFC.DSW.Event.DSWWarnStatus.ID, prevEventId);
        if (prevEventId.empty() == false && prevEventId != eventId) {
            setSFCDSWEventDSWWarnStatusStat(SFCDSWEventDSWWarnStatusStat::OFF);
            flushLastGroup();
        }
        if (eventId.empty() == false) {
            setSFCDSWEventDSWWarnStatusID(eventId);
            setSFCDSWEventDSWWarnStatusStat(SFCDSWEventDSWWarnStatusStat::ON);
            setSFCDSWEventDSWWarnStatusLinkedSoundID(linkedSoundId);
            setSFCDSWEventDSWWarnStatusLinkedSoundType(linkedSoundType);
            setSFCDSWEventDSWWarnStatusLinkedSoundRepeatCount(repeatCount);
        }
    }

    void updateSound() {
        if (mConfigDSW != ConfigDSW::ON) {
            return;
        }

        SFCDSWSoundDSWWarnStat stat = SFCDSWSoundDSWWarnStat::OFF;

        if (mIsIgnOn == true && mDSWWarnSoundStatus == DSWWarnSoundStatus::ON) {
            stat = SFCDSWSoundDSWWarnStat::ON;
        }

        setSFCDSWSoundDSWWarnStat(stat);
    }

    void updateSound2() {
        if (mConfigDSW != ConfigDSW::ON) {
            return;
        }

        SFCDSWSound2DSWWarnStat stat = SFCDSWSound2DSWWarnStat::OFF;

        if (mIsIgnOn == true && mDSWWarnSound2Status == DSWWarnSound2Status::ON) {
            stat = SFCDSWSound2DSWWarnStat::ON;
        }

        setSFCDSWSound2DSWWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using DSWWarnSymbolDrowStatus = ArgumentsDswWarnSymbolDrowStatusChanged::Input_DSWWarnSymbolDrowStatus;
    using DSWWarnSymbolDistStatus = ArgumentsDswWarnSymbolDistStatusChanged::Input_DSWWarnSymbolDistStatus;
    using DSWWarnStatus = ArgumentsDswWarnStatusChanged::Input_DSWWarnStatus;
    using DSWWarnSoundStatus = ArgumentsDswWarnSoundStatusChanged::Input_DSWWarnSoundStatus;
    using DSWWarnSound2Status = ArgumentsDswWarnSound2StatusChanged::Input_DSWWarnSound2Status;
    using ConfigDSW = ArgumentsConfigChanged::Inter_ConfigDSW;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HBool mIsIgnOn = false;
    DSWWarnSymbolDrowStatus mDSWWarnSymbolDrowStatus = DSWWarnSymbolDrowStatus::OFF;
    DSWWarnSymbolDistStatus mDSWWarnSymbolDistStatus = DSWWarnSymbolDistStatus::OFF;
    DSWWarnStatus mDSWWarnStatus = DSWWarnStatus::OFF;
    DSWWarnSoundStatus mDSWWarnSoundStatus = DSWWarnSoundStatus::OFF;
    DSWWarnSound2Status mDSWWarnSound2Status = DSWWarnSound2Status::OFF;
    ConfigDSW mConfigDSW = ConfigDSW::OFF;
};

}  // namespace ccos

#endif  // SFSS_DSW_H
