/**
 * @file E_LSD.hpp
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
#ifndef SFSS_E_LSD_H
#define SFSS_E_LSD_H

#define DLOG_ENABLED gEnableSFCLog

#include "E_LSDBase.hpp"

namespace ccos {

// SFC Version : 0.1.2
// Reference : [CH080] E_LSD. Contains : Event. Recommend: VALUE_CHANGED
class E_LSD : public E_LSDBase {
public:
    E_LSD() = default;
    ~E_LSD() override = default;
    E_LSD(const E_LSD& other) = delete;
    E_LSD(E_LSD&& other) noexcept = delete;
    E_LSD& operator=(const E_LSD& other) = delete;
    E_LSD& operator=(E_LSD&& other) noexcept = delete;

    void onInitialize() override {
        setSFCE_LSDEventE_LSDLinkedSoundID("SND_PopUpWarn1");
        setSFCE_LSDEventE_LSDLinkedSoundType(SFCE_LSDEventE_LSDLinkedSoundType::REPEAT_COUNT);
        setSFCE_LSDEventE_LSDLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateELSDEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateELSDEvent();
    }

    void onValueChanged(const ArgumentsValueChanged& args) {
        DDebug() << "Inter_ConfigELSD " << static_cast<HUInt64>(args.mInter_ConfigELSD);
        DDebug() << "Input_ELsdWarnStatus " << static_cast<HUInt64>(args.mInput_ELsdWarnStatus);
        mInterConfigELSD = args.mInter_ConfigELSD;
        if (ISTIMEOUT(args.mInput_ELsdWarnStatus) == true) {
            mInputELsdWarnStatus = InputELsdWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputELsdWarnStatus = args.mInput_ELsdWarnStatus;
        }
        updateELSDEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnitionElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using InterConfigELSD = ArgumentsValueChanged::Inter_ConfigELSD;
    using InputELsdWarnStatus = ArgumentsValueChanged::Input_ELsdWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
    InterConfigELSD mInterConfigELSD = InterConfigELSD::OFF;
    InputELsdWarnStatus mInputELsdWarnStatus = InputELsdWarnStatus::FAIL;

    void updateELSDEvent() {
        std::string eventID;
        if (mIgnitionElapsed == IgnitionElapsed::ON_0ms && mInterConfigELSD == InterConfigELSD::ON) {
            if (mInputELsdWarnStatus == InputELsdWarnStatus::FAIL ||
                mInputELsdWarnStatus == InputELsdWarnStatus::UNHANDLED_TIMEOUT) {
                DDebug() << "E30501 FAIL SFCE_LSDEventE_LSDStat::ON";
                eventID = "E30501";
            } else if (mInputELsdWarnStatus == InputELsdWarnStatus::OVERHEAT) {
                DDebug() << "E30502 OVERHEAT SFCE_LSDEventE_LSDStat::ON";
                eventID = "E30502";
            } else {
                // no operation
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.E_LSD.Event.E_LSD.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            DDebug() << "SFCE_LSDEventE_LSDStat::OFF ";
            setSFCE_LSDEventE_LSDStat(SFCE_LSDEventE_LSDStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCE_LSDEventE_LSDID(eventID);
            setSFCE_LSDEventE_LSDStat(SFCE_LSDEventE_LSDStat::ON);
        }
    }
};

}  // namespace ccos

#endif  // SFSS_E_LSD_H
