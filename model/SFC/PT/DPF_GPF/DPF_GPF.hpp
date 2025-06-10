/**
 * @file DPF_GPF.hpp
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
#ifndef SFSS_DPF_GPF_H
#define SFSS_DPF_GPF_H

#define DLOG_ENABLED gEnableSFCLog

#include "DPF_GPFBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT080] DPF_GPF. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class DPF_GPF : public DPF_GPFBase {
public:
    DPF_GPF() = default;
    ~DPF_GPF() override = default;
    DPF_GPF(const DPF_GPF& other) = delete;
    DPF_GPF(DPF_GPF&& other) noexcept = delete;
    DPF_GPF& operator=(const DPF_GPF& other) = delete;
    DPF_GPF& operator=(DPF_GPF&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDPF_GPFEventDpfWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCDPF_GPFEventDpfWarnLinkedSoundType(SFCDPF_GPFEventDpfWarnLinkedSoundType::REPEAT_COUNT);
        setSFCDPF_GPFEventDpfWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateEvent();
    }

    void onDpfWarnChanged(const ArgumentsDpfWarnChanged& args) {
        mDpfWarnStatus = args.mInput_DpfWarnStatus;
        updateTelltale();
        updateEvent();
    }

private:
    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDpfWarnStatus == DpfWarnStatus::WARN_LEVEL1) {
                setSFCDPF_GPFTelltaleDpfWarnStat(SFCDPF_GPFTelltaleDpfWarnStat::ON);
            } else if (mDpfWarnStatus == DpfWarnStatus::WARN_LEVEL2) {
                setSFCDPF_GPFTelltaleDpfWarnStat(SFCDPF_GPFTelltaleDpfWarnStat::BLINK2);
                setSFCDPF_GPFTelltaleDpfWarnBlinkValueA(SFCDPF_GPFTelltaleDpfWarnBlinkValueA::ON);
                setSFCDPF_GPFTelltaleDpfWarnBlinkValueB(SFCDPF_GPFTelltaleDpfWarnBlinkValueB::OFF);
            } else {
                setSFCDPF_GPFTelltaleDpfWarnStat(SFCDPF_GPFTelltaleDpfWarnStat::OFF);
            }
        } else {
            setSFCDPF_GPFTelltaleDpfWarnStat(SFCDPF_GPFTelltaleDpfWarnStat::OFF);
        }
    }

    void updateEvent() {
        std::string prevEventID;
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mDpfWarnStatus == DpfWarnStatus::WARN_LEVEL2) {
                eventID = "E20401";
            } else if (mDpfWarnStatus == DpfWarnStatus::WARN_LEVEL1) {
                eventID = "E20402";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.DPF_GPF.Event.DpfWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCDPF_GPFEventDpfWarnStat(SFCDPF_GPFEventDpfWarnStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCDPF_GPFEventDpfWarnID(eventID);
            setSFCDPF_GPFEventDpfWarnStat(SFCDPF_GPFEventDpfWarnStat::ON);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using DpfWarnStatus = ArgumentsDpfWarnChanged::Input_DpfWarnStatus;
    DpfWarnStatus mDpfWarnStatus = DpfWarnStatus::NORMAL;
};

}  // namespace ccos

#endif  // SFSS_DPF_GPF_H
