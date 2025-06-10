/**
 * @file AFS_DBL_2_0.hpp
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
#ifndef SFSS_AFS_DBL_2_0_H
#define SFSS_AFS_DBL_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "AFS_DBL_2_0Base.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [CH041] AFS&DBL 2.0. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class AFS_DBL_2_0 : public AFS_DBL_2_0Base {
public:
    AFS_DBL_2_0() = default;
    ~AFS_DBL_2_0() override = default;
    AFS_DBL_2_0(const AFS_DBL_2_0& other) = delete;
    AFS_DBL_2_0(AFS_DBL_2_0&& other) noexcept = delete;
    AFS_DBL_2_0& operator=(const AFS_DBL_2_0& other) = delete;
    AFS_DBL_2_0& operator=(AFS_DBL_2_0&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAFS_DBL_2_0TelltaleAFS_DBLBlinkValueA(SFCAFS_DBL_2_0TelltaleAFS_DBLBlinkValueA::ON);
        setSFCAFS_DBL_2_0TelltaleAFS_DBLBlinkValueB(SFCAFS_DBL_2_0TelltaleAFS_DBLBlinkValueB::OFF);

        setSFCAFS_DBL_2_0EventBlockedCameraID("E31501");
        setSFCAFS_DBL_2_0EventBlockedCameraLinkedSoundID("SND_PopUpWarn1");
        setSFCAFS_DBL_2_0EventBlockedCameraLinkedSoundType(SFCAFS_DBL_2_0EventBlockedCameraLinkedSoundType::REPEAT_COUNT);
        setSFCAFS_DBL_2_0EventBlockedCameraLinkedSoundRepeatCount(1);

        setSFCAFS_DBL_2_0EventSystemCheckID("E31502");
        setSFCAFS_DBL_2_0EventSystemCheckLinkedSoundID("SND_PopUpWarn1");
        setSFCAFS_DBL_2_0EventSystemCheckLinkedSoundType(SFCAFS_DBL_2_0EventSystemCheckLinkedSoundType::REPEAT_COUNT);
        setSFCAFS_DBL_2_0EventSystemCheckLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        mIsIgnOn = true;
        updateAfsDblTelltale();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        mIsIgnOn = true;
        updateAfsDblTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_3500ms;
        mIsIgnOn = true;
        updateAfsDblTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateAfsDblTelltale();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInterConfigIFSfromILCU = args.mInter_ConfigIFSfromILCU;
        updateAfsDblTelltale();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onAfsDblTelltaleChanged(const ArgumentsAfsDblTelltaleChanged& args) {
        if (ISTIMEOUT(args.mInput_IlcuLhWarnStatus) == true) {
            mInputIlcuLhWarnStatus = InputIlcuLhWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputIlcuLhWarnStatus = args.mInput_IlcuLhWarnStatus;
        }
        if (ISTIMEOUT(args.mInput_IlcuRhWarnStatus) == true) {
            mInputIlcuRhWarnStatus = InputIlcuRhWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputIlcuRhWarnStatus = args.mInput_IlcuRhWarnStatus;
        }
        updateAfsDblTelltale();
        updateSystemCheckEvent();
    }

    void onIntelligentLightingStatusChanged(const ArgumentsIntelligentLightingStatusChanged& args) {
        mInputILCURhActWarnStatus = args.mInput_IlcuRhActWarnStatus;
        mInputILCULhActWarnStatus = args.mInput_IlcuLhActWarnStatus;
        updateBlockedCameraEvent();
    }

private:
    void updateAfsDblTelltale() {
        SFCAFS_DBL_2_0TelltaleAFS_DBLStat stat = SFCAFS_DBL_2_0TelltaleAFS_DBLStat::OFF;
        if (mIgnitionElapsed == IgnitionElapsed::ON_500ms) {  // Prewarning
            if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                stat = SFCAFS_DBL_2_0TelltaleAFS_DBLStat::ON;
            }
        } else if (mIgnitionElapsed == IgnitionElapsed::ON_3500ms) {
            if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                if (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::BLINKING ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::BLINKING) {
                    stat = SFCAFS_DBL_2_0TelltaleAFS_DBLStat::BLINK4;
                } else if (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::IFS_ON ||
                           mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::IFS_ON ||
                           mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::AFS_ON ||
                           mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::AFS_ON ||
                           mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::UNHANDLED_TIMEOUT ||
                           mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::UNHANDLED_TIMEOUT) {
                    stat = SFCAFS_DBL_2_0TelltaleAFS_DBLStat::ON;
                } else {
                    // OFF
                }
            } else {
                // OFF
            }
        } else {
            // OFF
        }
        setSFCAFS_DBL_2_0TelltaleAFS_DBLStat(stat);
    }

    void updateBlockedCameraEvent() {
        SFCAFS_DBL_2_0EventBlockedCameraStat stat = SFCAFS_DBL_2_0EventBlockedCameraStat::OFF;
        if (mIsIgnOn == true) {
            if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON &&
                (mInputILCULhActWarnStatus == InputILCULhActWarnStatus::BLOCKAGE ||
                 mInputILCURhActWarnStatus == InputILCURhActWarnStatus::BLOCKAGE)) {
                stat = SFCAFS_DBL_2_0EventBlockedCameraStat::ON;
            }
        }
        setSFCAFS_DBL_2_0EventBlockedCameraStat(stat);
    }

    void updateSystemCheckEvent() {
        SFCAFS_DBL_2_0EventSystemCheckStat stat = SFCAFS_DBL_2_0EventSystemCheckStat::OFF;

        if (mIsIgnOn == true) {
            if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                if (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::IFS_ON ||
                    mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::UNHANDLED_TIMEOUT ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::IFS_ON ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::UNHANDLED_TIMEOUT) {
                    stat = SFCAFS_DBL_2_0EventSystemCheckStat::ON;
                }
            }
        }

        setSFCAFS_DBL_2_0EventSystemCheckStat(stat);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using InterConfigIFSfromILCU = ArgumentsConfigChanged::Inter_ConfigIFSfromILCU;
    using InputIlcuLhWarnStatus = ArgumentsAfsDblTelltaleChanged::Input_IlcuLhWarnStatus;
    using InputIlcuRhWarnStatus = ArgumentsAfsDblTelltaleChanged::Input_IlcuRhWarnStatus;
    using InputILCULhActWarnStatus = ArgumentsIntelligentLightingStatusChanged::Input_IlcuLhActWarnStatus;
    using InputILCURhActWarnStatus = ArgumentsIntelligentLightingStatusChanged::Input_IlcuRhActWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    InterConfigIFSfromILCU mInterConfigIFSfromILCU = InterConfigIFSfromILCU::OFF;
    InputIlcuLhWarnStatus mInputIlcuLhWarnStatus = InputIlcuLhWarnStatus::OFF;
    InputIlcuRhWarnStatus mInputIlcuRhWarnStatus = InputIlcuRhWarnStatus::OFF;
    InputILCULhActWarnStatus mInputILCULhActWarnStatus = InputILCULhActWarnStatus::OFF;
    InputILCURhActWarnStatus mInputILCURhActWarnStatus = InputILCURhActWarnStatus::OFF;

    HBool mIsIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_AFS_DBL_2_0_H
