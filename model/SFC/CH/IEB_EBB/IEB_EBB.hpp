/**
 * @file IEB_EBB.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2020  Hyundai Motor Company,
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
#ifndef SFSS_IEB_EBB_H
#define SFSS_IEB_EBB_H

#define DLOG_ENABLED gEnableSFCLog

#include "IEB_EBBBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 1.0.0
// Reference : [CH100] IEB_EBB. Contains : Event. Recommend: VALUE_CHANGED
class IEB_EBB : public IEB_EBBBase {
public:
    IEB_EBB() = default;
    ~IEB_EBB() override = default;
    IEB_EBB(const IEB_EBB& other) = delete;
    IEB_EBB(IEB_EBB&& other) noexcept = delete;
    IEB_EBB& operator=(const IEB_EBB& other) = delete;
    IEB_EBB& operator=(IEB_EBB&& other) noexcept = delete;

    void onInitialize() override {
        setSFCIEB_EBBEventE30701ID("E30701");
        setSFCIEB_EBBEventE30701LinkedSoundID("SND_PopUpWarn1");
        setSFCIEB_EBBEventE30701LinkedSoundType(SFCIEB_EBBEventE30701LinkedSoundType::REPEAT_COUNT);
        setSFCIEB_EBBEventE30701LinkedSoundRepeatCount(1);
        setSFCIEB_EBBEventE30702ID("E30702");
        setSFCIEB_EBBEventE30702LinkedSoundID("SND_PopUpWarn2");
        setSFCIEB_EBBEventE30702LinkedSoundType(SFCIEB_EBBEventE30702LinkedSoundType::DURATION);
        setSFCIEB_EBBEventE30702LinkedSoundRepeatCount(0);
        setSFCIEB_EBBEventE30702LinkedSoundDuration(10000);

        if (mOneShotTimerE30701On30msStatus.create(kTimerIebSysWarnStatusInterval30ms, this,
                                                   &IEB_EBB::onOneShotTimerE30701On30msStatus, false) == false) {
            DWarning() << "[IEB_EBB] mOneShotTimerE30701On30msStatus Create Failed";
        }
        if (mOneShotTimerE30702On30msStatus.create(kTimerIebSysWarnStatusInterval30ms, this,
                                                   &IEB_EBB::onOneShotTimerE30702On30msStatus, false) == false) {
            DWarning() << "[IEB_EBB] mOneShotTimerE30702On30msStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent30701();
        updateEvent30702();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent30701();
        updateEvent30702();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigIEB = args.mInter_ConfigIEB;
        mConfigEBB = args.mInter_ConfigEBB;
        updateEvent30701();
        updateEvent30702();
    }

    void onIebSysWarnChanged(const ArgumentsIebSysWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_IebSysWarnStatus) == true) {
            mIebSysWarnStatus = IebSysWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mIebSysWarnStatus = args.mInput_IebSysWarnStatus;
        }
        updateEvent30701();
        updateEvent30702();
    }

    void onEbbSysWarnChanged(const ArgumentsEbbSysWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_EbbSysWarnStatus) == true) {
            mEbbSysWarnStatus = EbbSysWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mEbbSysWarnStatus = args.mInput_EbbSysWarnStatus;
        }
        updateEvent30701();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void onOneShotTimerE30701On30msStatus() {
        if (mIebSysWarnStatus == IebSysWarnStatus::WARN1) {
            setSFCIEB_EBBEventE30701Stat(SFCIEB_EBBEventE30701Stat::ON);
            flush();
        }
    }

    void updateEvent30701() {
        SFCIEB_EBBEventE30701Stat e30701Stat = SFCIEB_EBBEventE30701Stat::OFF;

        if ((mConfigIEB == ConfigIEB::ON || mConfigEBB == ConfigEBB::ON) && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIebSysWarnStatus == IebSysWarnStatus::WARN1) {
                if (mOneShotTimerE30701On30msStatus.start() == false) {
                    DWarning() << "[IEB_EBB] mOneShotTimerE30701On30msStatus Start Failed";
                }
            } else {
                mOneShotTimerE30701On30msStatus.stop();
                if (mConfigEBB == ConfigEBB::ON &&
                    (mEbbSysWarnStatus == EbbSysWarnStatus::WARN1 || mEbbSysWarnStatus == EbbSysWarnStatus::UNHANDLED_TIMEOUT)) {
                    e30701Stat = SFCIEB_EBBEventE30701Stat::ON;
                }
            }
        } else {
            mOneShotTimerE30701On30msStatus.stop();
        }
        setSFCIEB_EBBEventE30701Stat(e30701Stat);
    }

    void onOneShotTimerE30702On30msStatus() {
        if (mIebSysWarnStatus == IebSysWarnStatus::WARN2) {
            setSFCIEB_EBBEventE30702Stat(SFCIEB_EBBEventE30702Stat::ON);
            flush();
        }
    }

    void updateEvent30702() {
        SFCIEB_EBBEventE30702Stat e30702Stat = SFCIEB_EBBEventE30702Stat::OFF;

        if ((mConfigIEB == ConfigIEB::ON || mConfigEBB == ConfigEBB::ON) && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIebSysWarnStatus == IebSysWarnStatus::WARN2) {
                if (mOneShotTimerE30702On30msStatus.start() == false) {
                    DWarning() << "[IEB_EBB] mOneShotTimerE30702On30msStatus Start Failed";
                }
            } else {
                mOneShotTimerE30702On30msStatus.stop();
                if (mConfigIEB == ConfigIEB::ON && mIebSysWarnStatus == IebSysWarnStatus::UNHANDLED_TIMEOUT) {
                    e30702Stat = SFCIEB_EBBEventE30702Stat::ON;
                }
            }
        } else {
            mOneShotTimerE30702On30msStatus.stop();
        }
        setSFCIEB_EBBEventE30702Stat(e30702Stat);
    }

    static constexpr uint32_t kTimerIebSysWarnStatusInterval30ms = 30u;

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using ConfigIEB = ArgumentsConfigChanged::Inter_ConfigIEB;
    using ConfigEBB = ArgumentsConfigChanged::Inter_ConfigEBB;
    using IebSysWarnStatus = ArgumentsIebSysWarnChanged::Input_IebSysWarnStatus;
    using EbbSysWarnStatus = ArgumentsEbbSysWarnChanged::Input_EbbSysWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigIEB mConfigIEB = ConfigIEB::OFF;
    ConfigEBB mConfigEBB = ConfigEBB::OFF;
    IebSysWarnStatus mIebSysWarnStatus = IebSysWarnStatus::OFF;
    EbbSysWarnStatus mEbbSysWarnStatus = EbbSysWarnStatus::OFF;
    ssfs::SFCTimer<IEB_EBB> mOneShotTimerE30701On30msStatus;
    ssfs::SFCTimer<IEB_EBB> mOneShotTimerE30702On30msStatus;
};

}  // namespace ccos

#endif  // SFSS_IEB_EBB_H
