/**
 * @file Sunroof_Window_Open.hpp
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
#ifndef SFSS_Sunroof_Window_Open_H
#define SFSS_Sunroof_Window_Open_H

#define DLOG_ENABLED gEnableSFCLog

#include "Sunroof_Window_OpenBase.hpp"

namespace ccos {

// SFC Version : 10.0.0
// Reference : [CD170] Sunroof_Window_Open. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Sunroof_Window_Open : public Sunroof_Window_OpenBase {
public:
    Sunroof_Window_Open() = default;
    ~Sunroof_Window_Open() override = default;
    Sunroof_Window_Open(const Sunroof_Window_Open& other) = delete;
    Sunroof_Window_Open(Sunroof_Window_Open&& other) noexcept = delete;
    Sunroof_Window_Open& operator=(const Sunroof_Window_Open& other) = delete;
    Sunroof_Window_Open& operator=(Sunroof_Window_Open&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSunroof_Window_OpenEventOpenWarningID("E00201");
        setSFCSunroof_Window_OpenEventOpenWarningLinkedSoundID("SND_SunroofWindowOpenWarn");
        setSFCSunroof_Window_OpenEventOpenWarningLinkedSoundType(
            SFCSunroof_Window_OpenEventOpenWarningLinkedSoundType::DURATION_MINMAX);
        setSFCSunroof_Window_OpenEventOpenWarningLinkedSoundDurationMin(0);
        setSFCSunroof_Window_OpenEventOpenWarningLinkedSoundDurationMax(4000);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsedPrev = mIgnElapsed;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAllConstant();
        updateEvent();

        mIgnElapsedPrev = IgnElapsed::NONE;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsedPrev = mIgnElapsed;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAllConstant();
        updateEvent();

        mIgnElapsedPrev = IgnElapsed::NONE;
    }

    void onConfigSunroofTypeChanged(const ArgumentsConfigSunroofTypeChanged& args) {
        mConfigSunroofType = args.mInter_ConfigSunroofType;
        updateSunroofType();
    }

    void onSunroofFrontOpenStatusChanged(const ArgumentsSunroofFrontOpenStatusChanged& args) {
        mSunroofFrontStat = args.mInput_SunroofFrontOpenStatus;
        mSunroofFrontStatFromRCM = args.mInput_SunroofFrontOpenStatusFromRCM;
        mSunroofFrontFromPDCex = args.mInput_SunroofFrontOpenStatusFromPDCex;
        updateSunroofFrontOpen();
        updateEvent();
    }

    void onSunroofRearOpenStatusChanged(const ArgumentsSunroofRearOpenStatusChanged& args) {
        mSunroofRearFromRCM = args.mInput_SunroofRearOpenStatusFromRCM;
        mSunroofRearFromPDCex = args.mInput_SunroofRearOpenStatusFromPDCex;
        updateSunroofRearOpen();
        updateEvent();
    }

    void onWindowRearOpenChanged(const ArgumentsWindowRearOpenChanged& args) {
        mWindowRearLeftSideStat = args.mInput_WindowOpenRearLeftSideStatus;
        mWindowRearRightSideStat = args.mInput_WindowOpenRearRightSideStatus;
        updateRearWindowOpen();
        updateEvent();
    }

    void onPosAndWindowFrontChanged(const ArgumentsPosAndWindowFrontChanged& args) {
        mDriverPositionTypeStat = args.mInput_DriverPositionTypeStatus;
        mDriverSideStat = args.mInput_WindowOpenDriverSideStatus;
        mAssistSideStat = args.mInput_WindowOpenAssistSideStatus;
        updateFrontWindowOpen();
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEvent() {
        if (mIgnElapsedPrev == IgnElapsed::ON_0ms && mIgnElapsed == IgnElapsed::OFF_0ms) {
            if (mPrivateWinFrontLeftStat == WindowStatus::OPEN || mPrivateWinFrontRightStat == WindowStatus::OPEN ||
                mWindowRearLeftSideStat == WindowRearLeftSideStat::OPEN ||
                mWindowRearRightSideStat == WindowRearRightSideStat::OPEN ||
                mPrivateSunroofFrontOpenStatus == Private_SunroofOpenStatus::OPEN ||
                mPrivateSunroofRearOpenStatus == Private_SunroofOpenStatus::OPEN) {
                setSFCSunroof_Window_OpenEventOpenWarningStat(SFCSunroof_Window_OpenEventOpenWarningStat::ON);
            } else {
                setSFCSunroof_Window_OpenEventOpenWarningStat(SFCSunroof_Window_OpenEventOpenWarningStat::OFF);
            }
        } else {
            setSFCSunroof_Window_OpenEventOpenWarningStat(SFCSunroof_Window_OpenEventOpenWarningStat::OFF);
        }
    }

    void updateAllConstant() {
        updateSunroofFrontOpen();
        updateSunroofRearOpen();
        updateRearWindowOpen();
        updateFrontWindowOpen();
    }

    void updateSunroofFrontOpen() {
        // 4.2 전석 선루프 열림 상태 신호 추상화
        if (mSunroofFrontStat == SunroofFrontStat::OPEN || mSunroofFrontStatFromRCM == SunroofFrontStatFromRCM::OPEN ||
            mSunroofFrontFromPDCex == SunroofFrontFromPDCex::OPEN) {
            mPrivateSunroofFrontOpenStatus = Private_SunroofOpenStatus::OPEN;
        } else {
            mPrivateSunroofFrontOpenStatus = Private_SunroofOpenStatus::CLOSE;
        }

        // 5.2.1 열림상태 - IMG_SunroofFrontOpen_stat
        if (mPrivateSunroofFrontOpenStatus == Private_SunroofOpenStatus::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCSunroof_Window_OpenConstantSunroofFrontOpenStat(SFCSunroof_Window_OpenConstantSunroofFrontOpenStat::OPEN);
        } else {
            setSFCSunroof_Window_OpenConstantSunroofFrontOpenStat(SFCSunroof_Window_OpenConstantSunroofFrontOpenStat::CLOSE);
        }
    }

    void updateSunroofRearOpen() {
        // 4.3 후석 선루프 열림 상태 신호 추상화
        if (mSunroofRearFromRCM == SunroofRearFromRCM::OPEN || mSunroofRearFromPDCex == SunroofRearFromPDCex::OPEN) {
            mPrivateSunroofRearOpenStatus = Private_SunroofOpenStatus::OPEN;
        } else {
            mPrivateSunroofRearOpenStatus = Private_SunroofOpenStatus::CLOSE;
        }

        // 5.2.1 열림상태 - IMG_SunroofRearOpen_stat
        if (mPrivateSunroofRearOpenStatus == Private_SunroofOpenStatus::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCSunroof_Window_OpenConstantSunroofRearOpenStat(SFCSunroof_Window_OpenConstantSunroofRearOpenStat::OPEN);
        } else {
            setSFCSunroof_Window_OpenConstantSunroofRearOpenStat(SFCSunroof_Window_OpenConstantSunroofRearOpenStat::CLOSE);
        }
    }

    void updateRearWindowOpen() {
        if (mWindowRearLeftSideStat == WindowRearLeftSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCSunroof_Window_OpenConstantWindowOpenRearLeftStat(SFCSunroof_Window_OpenConstantWindowOpenRearLeftStat::OPEN);
        } else {
            setSFCSunroof_Window_OpenConstantWindowOpenRearLeftStat(SFCSunroof_Window_OpenConstantWindowOpenRearLeftStat::CLOSE);
        }

        if (mWindowRearRightSideStat == WindowRearRightSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
            setSFCSunroof_Window_OpenConstantWindowOpenRearRightStat(SFCSunroof_Window_OpenConstantWindowOpenRearRightStat::OPEN);
        } else {
            setSFCSunroof_Window_OpenConstantWindowOpenRearRightStat(
                SFCSunroof_Window_OpenConstantWindowOpenRearRightStat::CLOSE);
        }
    }

    void updateFrontWindowOpen() {
        if (mDriverPositionTypeStat == DriverPositionTypeStat::LHD) {
            if (mDriverSideStat == WindowDriverSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
                mPrivateWinFrontLeftStat = WindowStatus::OPEN;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat::OPEN);
            } else {
                mPrivateWinFrontLeftStat = WindowStatus::CLOSE;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat::CLOSE);
            }
            if (mAssistSideStat == WindowAssistSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
                mPrivateWinFrontRightStat = WindowStatus::OPEN;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontRightStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontRightStat::OPEN);
            } else {
                mPrivateWinFrontRightStat = WindowStatus::CLOSE;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontRightStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontRightStat::CLOSE);
            }
        } else if (mDriverPositionTypeStat == DriverPositionTypeStat::RHD) {
            if (mDriverSideStat == WindowDriverSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
                mPrivateWinFrontRightStat = WindowStatus::OPEN;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontRightStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontRightStat::OPEN);
            } else {
                mPrivateWinFrontRightStat = WindowStatus::CLOSE;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontRightStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontRightStat::CLOSE);
            }
            if (mAssistSideStat == WindowAssistSideStat::OPEN && mIgnElapsed == IgnElapsed::OFF_0ms) {
                mPrivateWinFrontLeftStat = WindowStatus::OPEN;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat::OPEN);
            } else {
                mPrivateWinFrontLeftStat = WindowStatus::CLOSE;
                setSFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat(
                    SFCSunroof_Window_OpenConstantWindowOpenFrontLeftStat::CLOSE);
            }
        } else {
            DError() << "Driver Position Should Be LHD or RHD";
        }
    }

    void updateSunroofType() {
        SFCSunroof_Window_OpenConstantSunroofTypeStat stat = SFCSunroof_Window_OpenConstantSunroofTypeStat::SINGLE;

        if (mConfigSunroofType == ConfigSunroofType::DUAL) {
            stat = SFCSunroof_Window_OpenConstantSunroofTypeStat::DUAL;
        }
        setSFCSunroof_Window_OpenConstantSunroofTypeStat(stat);
    }

    enum class WindowStatus : HUInt64 {
        CLOSE = 0x0,
        OPEN = 0x1,
        MAX = 0x2
    };

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class Private_SunroofOpenStatus : HUInt64 {
        NONE,
        OPEN,
        CLOSE,
        MAX
    };

    using ConfigSunroofType = ArgumentsConfigSunroofTypeChanged::Inter_ConfigSunroofType;
    using DriverPositionTypeStat = ArgumentsPosAndWindowFrontChanged::Input_DriverPositionTypeStatus;
    using WindowDriverSideStat = ArgumentsPosAndWindowFrontChanged::Input_WindowOpenDriverSideStatus;
    using WindowAssistSideStat = ArgumentsPosAndWindowFrontChanged::Input_WindowOpenAssistSideStatus;
    using WindowRearLeftSideStat = ArgumentsWindowRearOpenChanged::Input_WindowOpenRearLeftSideStatus;
    using WindowRearRightSideStat = ArgumentsWindowRearOpenChanged::Input_WindowOpenRearRightSideStatus;
    using SunroofFrontStat = ArgumentsSunroofFrontOpenStatusChanged::Input_SunroofFrontOpenStatus;
    using SunroofFrontStatFromRCM = ArgumentsSunroofFrontOpenStatusChanged::Input_SunroofFrontOpenStatusFromRCM;
    using SunroofFrontFromPDCex = ArgumentsSunroofFrontOpenStatusChanged::Input_SunroofFrontOpenStatusFromPDCex;
    using SunroofRearFromRCM = ArgumentsSunroofRearOpenStatusChanged::Input_SunroofRearOpenStatusFromRCM;
    using SunroofRearFromPDCex = ArgumentsSunroofRearOpenStatusChanged::Input_SunroofRearOpenStatusFromPDCex;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    IgnElapsed mIgnElapsedPrev = IgnElapsed::NONE;
    ConfigSunroofType mConfigSunroofType = ConfigSunroofType::SINGLE;
    WindowStatus mPrivateWinFrontLeftStat = WindowStatus::MAX;
    WindowStatus mPrivateWinFrontRightStat = WindowStatus::MAX;
    WindowDriverSideStat mDriverSideStat = WindowDriverSideStat::CLOSE;
    WindowAssistSideStat mAssistSideStat = WindowAssistSideStat::CLOSE;
    DriverPositionTypeStat mDriverPositionTypeStat = DriverPositionTypeStat::LHD;
    Private_SunroofOpenStatus mPrivateSunroofFrontOpenStatus = Private_SunroofOpenStatus::CLOSE;
    Private_SunroofOpenStatus mPrivateSunroofRearOpenStatus = Private_SunroofOpenStatus::CLOSE;
    WindowRearLeftSideStat mWindowRearLeftSideStat = WindowRearLeftSideStat::CLOSE;
    WindowRearRightSideStat mWindowRearRightSideStat = WindowRearRightSideStat::CLOSE;
    SunroofFrontStat mSunroofFrontStat = SunroofFrontStat::CLOSE;
    SunroofFrontStatFromRCM mSunroofFrontStatFromRCM = SunroofFrontStatFromRCM::CLOSE;
    SunroofFrontFromPDCex mSunroofFrontFromPDCex = SunroofFrontFromPDCex::CLOSE;
    SunroofRearFromRCM mSunroofRearFromRCM = SunroofRearFromRCM::CLOSE;
    SunroofRearFromPDCex mSunroofRearFromPDCex = SunroofRearFromPDCex::CLOSE;
};

}  // namespace ccos

#endif  // SFSS_Sunroof_Window_Open_H
