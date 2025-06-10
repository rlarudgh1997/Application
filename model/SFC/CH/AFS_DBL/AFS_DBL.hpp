/**
 * @file AFS_DBL.hpp
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
#ifndef SFSS_AFS_DBL_H
#define SFSS_AFS_DBL_H

#define DLOG_ENABLED gEnableSFCLog

#include "AFS_DBLBase.hpp"

namespace ccos {

// SFC Version : 6.0.2
// Reference : [CH040] AFS&DBL. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class AFS_DBL : public AFS_DBLBase {
public:
    AFS_DBL() = default;
    ~AFS_DBL() override = default;
    AFS_DBL(const AFS_DBL& other) = delete;
    AFS_DBL(AFS_DBL&& other) noexcept = delete;
    AFS_DBL& operator=(const AFS_DBL& other) = delete;
    AFS_DBL& operator=(AFS_DBL&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAFS_DBLTelltaleAFS_DBLBlinkValueA(SFCAFS_DBLTelltaleAFS_DBLBlinkValueA::ON);
        setSFCAFS_DBLTelltaleAFS_DBLBlinkValueB(SFCAFS_DBLTelltaleAFS_DBLBlinkValueB::OFF);

        setSFCAFS_DBLEventBlockedCameraID("E31501");
        setSFCAFS_DBLEventBlockedCameraLinkedSoundID("SND_PopUpWarn1");
        setSFCAFS_DBLEventBlockedCameraLinkedSoundType(SFCAFS_DBLEventBlockedCameraLinkedSoundType::REPEAT_COUNT);
        setSFCAFS_DBLEventBlockedCameraLinkedSoundRepeatCount(1);

        setSFCAFS_DBLEventSystemCheckID("E31502");
        setSFCAFS_DBLEventSystemCheckLinkedSoundID("SND_PopUpWarn1");
        setSFCAFS_DBLEventSystemCheckLinkedSoundType(SFCAFS_DBLEventSystemCheckLinkedSoundType::REPEAT_COUNT);
        setSFCAFS_DBLEventSystemCheckLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_0ms;
        updateAfsDblTelltaleChanged();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_500ms;
        updateAfsDblTelltaleChanged();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::ON_3500ms;
        updateAfsDblTelltaleChanged();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnitionElapsed = IgnitionElapsed::OFF_0ms;
        updateAfsDblTelltaleChanged();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mInterConfigAFS = args.mInter_ConfigAFS;
        mInterConfigIFS = args.mInter_ConfigIFS;
        mInterConfigIFSfromILCU = args.mInter_ConfigIFSfromILCU;
        updateAfsDblTelltaleChanged();
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

    void onAfsDblTelltaleChanged(const ArgumentsAfsDblTelltaleChanged& args) {
        if (ISTIMEOUT(args.mInput_AfsStatus) == true) {
            mInputAfsStatus = InputAfsStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputAfsStatus = args.mInput_AfsStatus;
        }
        if (ISTIMEOUT(args.mInput_IfsWarnStatus) == true) {
            mInputIfsWarnStatus = InputIfsWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputIfsWarnStatus = args.mInput_IfsWarnStatus;
        }
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
        updateAfsDblTelltaleChanged();
        updateSystemCheckEvent();
    }

    void onIntelligentLightingStatusChanged(const ArgumentsIntelligentLightingStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_IfsSysStatus) == true) {
            mInputIfsSysStatus = InputIfsSysStatus::UNHANDLED_TIMEOUT;
        } else {
            mInputIfsSysStatus = args.mInput_IfsSysStatus;
        }
        mInputILCURhActWarnStatus = args.mInput_IlcuRhActWarnStatus;
        mInputILCULhActWarnStatus = args.mInput_IlcuLhActWarnStatus;
        updateBlockedCameraEvent();
        updateSystemCheckEvent();
    }

private:
    void updateAfsDblTelltaleChanged() {
        SFCAFS_DBLTelltaleAFS_DBLStat stat = SFCAFS_DBLTelltaleAFS_DBLStat::OFF;
        if (mIgnitionElapsed == IgnitionElapsed::ON_3500ms) {
            if ((mInterConfigIFS == InterConfigIFS::ON && mInputIfsWarnStatus == InputIfsWarnStatus::BLINKING) ||
                (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON &&
                 (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::BLINKING ||
                  mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::BLINKING))) {
                stat = SFCAFS_DBLTelltaleAFS_DBLStat::BLINK4;
            } else if (mInterConfigAFS == InterConfigAFS::ON && mInputAfsStatus == InputAfsStatus::NOT_CALIBRATED) {
                stat = SFCAFS_DBLTelltaleAFS_DBLStat::BLINK1;
            } else if (mInterConfigAFS == InterConfigAFS::ON && mInputAfsStatus == InputAfsStatus::AFS_OFF) {
                stat = SFCAFS_DBLTelltaleAFS_DBLStat::BLINK2;
            } else if ((mInterConfigAFS == InterConfigAFS::ON &&
                        (mInputAfsStatus == InputAfsStatus::DEFECTIVE || mInputAfsStatus == InputAfsStatus::UNHANDLED_TIMEOUT)) ||
                       (mInterConfigIFS == InterConfigIFS::ON &&
                        ((mInputIfsWarnStatus == InputIfsWarnStatus::IFS_ON ||
                          mInputIfsWarnStatus == InputIfsWarnStatus::UNHANDLED_TIMEOUT) ||
                         mInputIfsWarnStatus == InputIfsWarnStatus::AFS_ON))) {
                stat = SFCAFS_DBLTelltaleAFS_DBLStat::ON;
            } else if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                if (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::IFS_ON ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::IFS_ON ||
                    mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::AFS_ON ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::AFS_ON ||
                    mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::UNHANDLED_TIMEOUT ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::UNHANDLED_TIMEOUT) {
                    stat = SFCAFS_DBLTelltaleAFS_DBLStat::ON;
                }
            } else {
                // OFF
            }
        } else if (mIgnitionElapsed == IgnitionElapsed::ON_500ms) {  // Prewarning
            if (mInterConfigAFS == InterConfigAFS::ON || mInterConfigIFS == InterConfigIFS::ON ||
                mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                stat = SFCAFS_DBLTelltaleAFS_DBLStat::ON;
            }
        } else {
            // OFF
        }
        setSFCAFS_DBLTelltaleAFS_DBLStat(stat);
    }

    void updateBlockedCameraEvent() {
        SFCAFS_DBLEventBlockedCameraStat stat = SFCAFS_DBLEventBlockedCameraStat::OFF;
        if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if ((mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON &&
                 (mInputILCULhActWarnStatus == InputILCULhActWarnStatus::BLOCKAGE ||
                  mInputILCURhActWarnStatus == InputILCURhActWarnStatus::BLOCKAGE)) ||
                (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::OFF && mInterConfigIFS == InterConfigIFS::ON &&
                 mInputIfsSysStatus == InputIfsSysStatus::BLOCKAGE)) {
                stat = SFCAFS_DBLEventBlockedCameraStat::ON;
            }
        }
        setSFCAFS_DBLEventBlockedCameraStat(stat);
    }

    void updateSystemCheckEvent() {
        SFCAFS_DBLEventSystemCheckStat stat = SFCAFS_DBLEventSystemCheckStat::OFF;

        if (mIgnitionElapsed >= IgnitionElapsed::ON_0ms) {
            if (mInterConfigIFSfromILCU == InterConfigIFSfromILCU::ON) {
                if (mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::IFS_ON ||
                    mInputIlcuLhWarnStatus == InputIlcuLhWarnStatus::UNHANDLED_TIMEOUT ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::IFS_ON ||
                    mInputIlcuRhWarnStatus == InputIlcuRhWarnStatus::UNHANDLED_TIMEOUT) {
                    stat = SFCAFS_DBLEventSystemCheckStat::ON;
                }
            }
            if (mInterConfigIFS == InterConfigIFS::ON) {
                if (mInputIfsSysStatus == InputIfsSysStatus::UNHANDLED_TIMEOUT ||
                    mInputIfsWarnStatus == InputIfsWarnStatus::IFS_ON) {
                    stat = SFCAFS_DBLEventSystemCheckStat::ON;
                }
            }
        }

        setSFCAFS_DBLEventSystemCheckStat(stat);
    }

    enum class IgnitionElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using InterConfigAFS = ArgumentsConfigChanged::Inter_ConfigAFS;
    using InterConfigIFS = ArgumentsConfigChanged::Inter_ConfigIFS;
    using InterConfigIFSfromILCU = ArgumentsConfigChanged::Inter_ConfigIFSfromILCU;
    using InputAfsStatus = ArgumentsAfsDblTelltaleChanged::Input_AfsStatus;
    using InputIfsWarnStatus = ArgumentsAfsDblTelltaleChanged::Input_IfsWarnStatus;
    using InputIlcuLhWarnStatus = ArgumentsAfsDblTelltaleChanged::Input_IlcuLhWarnStatus;
    using InputIlcuRhWarnStatus = ArgumentsAfsDblTelltaleChanged::Input_IlcuRhWarnStatus;

    using InputIfsSysStatus = ArgumentsIntelligentLightingStatusChanged::Input_IfsSysStatus;
    using InputILCULhActWarnStatus = ArgumentsIntelligentLightingStatusChanged::Input_IlcuLhActWarnStatus;
    using InputILCURhActWarnStatus = ArgumentsIntelligentLightingStatusChanged::Input_IlcuRhActWarnStatus;

    IgnitionElapsed mIgnitionElapsed = IgnitionElapsed::NONE;
    InterConfigAFS mInterConfigAFS = InterConfigAFS::OFF;
    InterConfigIFS mInterConfigIFS = InterConfigIFS::OFF;
    InterConfigIFSfromILCU mInterConfigIFSfromILCU = InterConfigIFSfromILCU::OFF;
    InputAfsStatus mInputAfsStatus = InputAfsStatus::NORMAL;
    InputIfsWarnStatus mInputIfsWarnStatus = InputIfsWarnStatus::OFF;
    InputIlcuLhWarnStatus mInputIlcuLhWarnStatus = InputIlcuLhWarnStatus::OFF;
    InputIlcuRhWarnStatus mInputIlcuRhWarnStatus = InputIlcuRhWarnStatus::OFF;

    // Intelligent Front Lighting system stat
    // InputIfsSysStatus mInputIfsSysStatus = InputIfsSysStatus::NOT_ACTIVE;
    InputIfsSysStatus mInputIfsSysStatus = InputIfsSysStatus::OFF;
    InputILCULhActWarnStatus mInputILCULhActWarnStatus = InputILCULhActWarnStatus::OFF;
    InputILCURhActWarnStatus mInputILCURhActWarnStatus = InputILCURhActWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_AFS_DBL_H
