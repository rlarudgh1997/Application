/**
 * @file WPC_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_WPC_CV_H
#define SFSS_WPC_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "WPC_CVBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.15
// Reference : [CV670] WPC_CV. Contains : Event. Recommend: VALUE_CHANGED
class WPC_CV : public WPC_CVBase {
public:
    WPC_CV() = default;
    ~WPC_CV() override = default;
    WPC_CV(const WPC_CV& other) = delete;
    WPC_CV(WPC_CV&& other) noexcept = delete;
    WPC_CV& operator=(const WPC_CV& other) = delete;
    WPC_CV& operator=(WPC_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCWPC_CVEventWPCPopupID("E76701");
        setSFCWPC_CVEventWPCPopupLinkedSoundID("SND_PopUpInform2");
        setSFCWPC_CVEventWPCPopupLinkedSoundType(SFCWPC_CVEventWPCPopupLinkedSoundType::REPEAT_COUNT);
        setSFCWPC_CVEventWPCPopupLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateEvent();
    }

    void onConfigWPCChanged(const ArgumentsConfigWPCChanged& args) {
        mConfigWPC = args.mInter_ConfigWPC;

        updatePrivateDrvAstDoorOpenStatus();
        updatePrivateBusDoorOpenStatus();
        updateEvent();
    }

    void onDrvAstDoorOpenStatusChanged(const ArgumentsDrvAstDoorOpenStatusChanged& args) {
        mIMUDrvDoorOpenStatus = args.mInput_IMUDrvDoorOpenStatus;
        mIMUAstDoorOpenStatus = args.mInput_IMUAstDoorOpenStatus;

        updatePrivateDrvAstDoorOpenStatus();
        updateEvent();
    }

    void onBusDoorOpenStatusChanged(const ArgumentsBusDoorOpenStatusChanged& args) {
        mIMUFrDoorOpenStatus = args.mInput_IMUFrDoorOpenStatus;
        mIMUMidDoorOpenStatus = args.mInput_IMUMidDoorOpenStatus;
        mIMURrDoorOpenStatus = args.mInput_IMURrDoorOpenStatus;

        updatePrivateBusDoorOpenStatus();
        updateEvent();
    }

    void onWpcPopUpStatusChanged(const ArgumentsWpcPopUpStatusChanged& args) {
        mWPCPopUpStatus = args.mInput_WPCPopUpStatus;

        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateDrvAstDoorOpenStatus() {
        if (mConfigWPC != ConfigWPC::ON) {
            return;
        }

        mPrivate_DrvAstDoorOpenStatus = Private_DrvAstDoorOpenStatus::CLOSE;
        if (mIMUDrvDoorOpenStatus == IMUDrvDoorOpenStatus::OPEN || mIMUAstDoorOpenStatus == IMUAstDoorOpenStatus::OPEN) {
            mPrivate_DrvAstDoorOpenStatus = Private_DrvAstDoorOpenStatus::OPEN;
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateWPC_CVDrvAstDoorOpenStat(static_cast<SFCPrivateWPC_CVDrvAstDoorOpenStat>(mPrivate_DrvAstDoorOpenStatus));
#endif
    }

    void updatePrivateBusDoorOpenStatus() {
        if (mConfigWPC != ConfigWPC::ON) {
            return;
        }
        mPrivate_BusDoorOpenStatus = Private_BusDoorOpenStatus::CLOSE;
        if (mIMUFrDoorOpenStatus == IMUFrDoorOpenStatus::OPEN || mIMUMidDoorOpenStatus == IMUMidDoorOpenStatus::OPEN ||
            mIMURrDoorOpenStatus == IMURrDoorOpenStatus::OPEN) {
            mPrivate_BusDoorOpenStatus = Private_BusDoorOpenStatus::OPEN;
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateWPC_CVBusDoorOpenStat(static_cast<SFCPrivateWPC_CVBusDoorOpenStat>(mPrivate_BusDoorOpenStatus));
#endif
    }

    void updateEvent() {
        if (mConfigWPC != ConfigWPC::ON) {
            return;
        }

        SFCWPC_CVEventWPCPopupStat eventStat = SFCWPC_CVEventWPCPopupStat::OFF;
        if (mIgnElapsed == IgnElapsed::OFF_0ms) {
            if ((mPrivate_BusDoorOpenStatus == Private_BusDoorOpenStatus::OPEN ||
                 mPrivate_DrvAstDoorOpenStatus == Private_DrvAstDoorOpenStatus::OPEN) &&
                mWPCPopUpStatus == WPCPopUpStatus::CELLPHONE_ON_PAD) {
                eventStat = SFCWPC_CVEventWPCPopupStat::ON;
            } else if (mWPCPopUpStatus == WPCPopUpStatus::CELLPHONE_REMIND) {
                eventStat = SFCWPC_CVEventWPCPopupStat::ON;
            } else {
                // no operation
            }
        }

        setSFCWPC_CVEventWPCPopupStat(eventStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class Private_DrvAstDoorOpenStatus : HUInt64 {
        NONE,
        CLOSE,
        OPEN,
        MAX
    };

    enum class Private_BusDoorOpenStatus : HUInt64 {
        NONE,
        CLOSE,
        OPEN,
        MAX
    };

    using ConfigWPC = ArgumentsConfigWPCChanged::Inter_ConfigWPC;
    using WPCPopUpStatus = ArgumentsWpcPopUpStatusChanged::Input_WPCPopUpStatus;
    using IMUDrvDoorOpenStatus = ArgumentsDrvAstDoorOpenStatusChanged::Input_IMUDrvDoorOpenStatus;
    using IMUAstDoorOpenStatus = ArgumentsDrvAstDoorOpenStatusChanged::Input_IMUAstDoorOpenStatus;
    using IMUFrDoorOpenStatus = ArgumentsBusDoorOpenStatusChanged::Input_IMUFrDoorOpenStatus;
    using IMUMidDoorOpenStatus = ArgumentsBusDoorOpenStatusChanged::Input_IMUMidDoorOpenStatus;
    using IMURrDoorOpenStatus = ArgumentsBusDoorOpenStatusChanged::Input_IMURrDoorOpenStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigWPC mConfigWPC = ConfigWPC::OFF;
    WPCPopUpStatus mWPCPopUpStatus = WPCPopUpStatus::OFF;
    IMUDrvDoorOpenStatus mIMUDrvDoorOpenStatus = IMUDrvDoorOpenStatus::OFF;
    IMUAstDoorOpenStatus mIMUAstDoorOpenStatus = IMUAstDoorOpenStatus::OFF;
    IMUFrDoorOpenStatus mIMUFrDoorOpenStatus = IMUFrDoorOpenStatus::OFF;
    IMUMidDoorOpenStatus mIMUMidDoorOpenStatus = IMUMidDoorOpenStatus::OFF;
    IMURrDoorOpenStatus mIMURrDoorOpenStatus = IMURrDoorOpenStatus::OFF;
    Private_DrvAstDoorOpenStatus mPrivate_DrvAstDoorOpenStatus = Private_DrvAstDoorOpenStatus::CLOSE;
    Private_BusDoorOpenStatus mPrivate_BusDoorOpenStatus = Private_BusDoorOpenStatus::CLOSE;
};

}  // namespace ccos

#endif  // SFSS_WPC_CV_H
