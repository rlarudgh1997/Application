/**
 * @file Sunroof_Window_Open_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_Sunroof_Window_Open_2_0_H
#define SFSS_Sunroof_Window_Open_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Sunroof_Window_Open_2_0Base.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [CD171] Sunroof_Window_Open_2_0. Contains : Event. Recommend: VALUE_CHANGED
class Sunroof_Window_Open_2_0 : public Sunroof_Window_Open_2_0Base {
public:
    Sunroof_Window_Open_2_0() = default;
    ~Sunroof_Window_Open_2_0() override = default;
    Sunroof_Window_Open_2_0(const Sunroof_Window_Open_2_0& other) = delete;
    Sunroof_Window_Open_2_0(Sunroof_Window_Open_2_0&& other) noexcept = delete;
    Sunroof_Window_Open_2_0& operator=(const Sunroof_Window_Open_2_0& other) = delete;
    Sunroof_Window_Open_2_0& operator=(Sunroof_Window_Open_2_0&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSunroof_Window_Open_2_0EventOpenWarningLinkedSoundID("SND_SunroofWindowOpenWarn");
        setSFCSunroof_Window_Open_2_0EventOpenWarningLinkedSoundType(
            SFCSunroof_Window_Open_2_0EventOpenWarningLinkedSoundType::DURATION_MINMAX);
        setSFCSunroof_Window_Open_2_0EventOpenWarningLinkedSoundDurationMin(0);
        setSFCSunroof_Window_Open_2_0EventOpenWarningLinkedSoundDurationMax(4000);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
        mIgnElapsedPrev = mIgnElapsed;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
        mIgnElapsedPrev = mIgnElapsed;
    }

    void onWindoewOpenSideStatusChanged(const ArgumentsWindoewOpenSideStatusChanged& args) {
        mWindowOpenDriverSideStatus = args.mInput_WindowOpenDriverSideStatus;
        mWindowOpenAssistSideStatus = args.mInput_WindowOpenAssistSideStatus;
        mWindowOpenRearLeftSideStatus = args.mInput_WindowOpenRearLeftSideStatus;
        mWindowOpenRearRightSideStatus = args.mInput_WindowOpenRearRightSideStatus;
        updateEvent();
    }

    void onSunroofOpenStatusChanged(const ArgumentsSunroofOpenStatusChanged& args) {
        mSunroofFrontOpenStatusFromRCM = args.mInput_SunroofFrontOpenStatusFromRCM;
        mSunroofFrontOpenStatusFromPDCex = args.mInput_SunroofFrontOpenStatusFromPDCex;
        mSunroofRearOpenStatusFromRCM = args.mInput_SunroofRearOpenStatusFromRCM;
        mSunroofRearOpenStatusFromPDCex = args.mInput_SunroofRearOpenStatusFromPDCex;
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEvent() {
        std::string prevEventID;
        std::string eventID;
        if (mIgnElapsedPrev == IgnElapsed::ON_0ms && mIgnElapsed == IgnElapsed::OFF_0ms) {
            if ((mWindowOpenDriverSideStatus == WindowOpenDriverSideStatus::OPEN ||
                 mWindowOpenAssistSideStatus == WindowOpenAssistSideStatus::OPEN ||
                 mWindowOpenRearLeftSideStatus == WindowOpenRearLeftSideStatus::OPEN ||
                 mWindowOpenRearRightSideStatus == WindowOpenRearRightSideStatus::OPEN) &&
                (mSunroofFrontOpenStatusFromRCM == SunroofFrontOpenStatusFromRCM::OPEN ||
                 mSunroofFrontOpenStatusFromPDCex == SunroofFrontOpenStatusFromPDCex::OPEN ||
                 mSunroofRearOpenStatusFromRCM == SunroofRearOpenStatusFromRCM::OPEN ||
                 mSunroofRearOpenStatusFromPDCex == SunroofRearOpenStatusFromPDCex::OPEN)) {
                eventID = "E00204";
            } else if (mWindowOpenDriverSideStatus == WindowOpenDriverSideStatus::OPEN ||
                       mWindowOpenAssistSideStatus == WindowOpenAssistSideStatus::OPEN ||
                       mWindowOpenRearLeftSideStatus == WindowOpenRearLeftSideStatus::OPEN ||
                       mWindowOpenRearRightSideStatus == WindowOpenRearRightSideStatus::OPEN) {
                eventID = "E00202";
            } else if (mSunroofFrontOpenStatusFromRCM == SunroofFrontOpenStatusFromRCM::OPEN ||
                       mSunroofFrontOpenStatusFromPDCex == SunroofFrontOpenStatusFromPDCex::OPEN ||
                       mSunroofRearOpenStatusFromRCM == SunroofRearOpenStatusFromRCM::OPEN ||
                       mSunroofRearOpenStatusFromPDCex == SunroofRearOpenStatusFromPDCex::OPEN) {
                eventID = "E00203";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.Sunroof_Window_Open_2_0.Event.OpenWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCSunroof_Window_Open_2_0EventOpenWarningStat(SFCSunroof_Window_Open_2_0EventOpenWarningStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCSunroof_Window_Open_2_0EventOpenWarningID(eventID);
            setSFCSunroof_Window_Open_2_0EventOpenWarningStat(SFCSunroof_Window_Open_2_0EventOpenWarningStat::ON);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using WindowOpenDriverSideStatus = ArgumentsWindoewOpenSideStatusChanged::Input_WindowOpenDriverSideStatus;
    using WindowOpenAssistSideStatus = ArgumentsWindoewOpenSideStatusChanged::Input_WindowOpenAssistSideStatus;
    using WindowOpenRearLeftSideStatus = ArgumentsWindoewOpenSideStatusChanged::Input_WindowOpenRearLeftSideStatus;
    using WindowOpenRearRightSideStatus = ArgumentsWindoewOpenSideStatusChanged::Input_WindowOpenRearRightSideStatus;
    using SunroofFrontOpenStatusFromRCM = ArgumentsSunroofOpenStatusChanged::Input_SunroofFrontOpenStatusFromRCM;
    using SunroofFrontOpenStatusFromPDCex = ArgumentsSunroofOpenStatusChanged::Input_SunroofFrontOpenStatusFromPDCex;
    using SunroofRearOpenStatusFromRCM = ArgumentsSunroofOpenStatusChanged::Input_SunroofRearOpenStatusFromRCM;
    using SunroofRearOpenStatusFromPDCex = ArgumentsSunroofOpenStatusChanged::Input_SunroofRearOpenStatusFromPDCex;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    IgnElapsed mIgnElapsedPrev = IgnElapsed::NONE;
    WindowOpenDriverSideStatus mWindowOpenDriverSideStatus = WindowOpenDriverSideStatus::CLOSE;
    WindowOpenAssistSideStatus mWindowOpenAssistSideStatus = WindowOpenAssistSideStatus::CLOSE;
    WindowOpenRearLeftSideStatus mWindowOpenRearLeftSideStatus = WindowOpenRearLeftSideStatus::CLOSE;
    WindowOpenRearRightSideStatus mWindowOpenRearRightSideStatus = WindowOpenRearRightSideStatus::CLOSE;
    SunroofFrontOpenStatusFromRCM mSunroofFrontOpenStatusFromRCM = SunroofFrontOpenStatusFromRCM::CLOSE;
    SunroofFrontOpenStatusFromPDCex mSunroofFrontOpenStatusFromPDCex = SunroofFrontOpenStatusFromPDCex::CLOSE;
    SunroofRearOpenStatusFromRCM mSunroofRearOpenStatusFromRCM = SunroofRearOpenStatusFromRCM::CLOSE;
    SunroofRearOpenStatusFromPDCex mSunroofRearOpenStatusFromPDCex = SunroofRearOpenStatusFromPDCex::CLOSE;
};

}  // namespace ccos

#endif  // SFSS_Sunroof_Window_Open_2_0_H
