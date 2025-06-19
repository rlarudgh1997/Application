/**
 * @file EBS.hpp
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
#ifndef SFSS_EBS_H
#define SFSS_EBS_H

#define DLOG_ENABLED gEnableSFCLog

#include "EBSBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.35
// Reference : [CV130] EBS. Contains : Telltale. Recommend: VALUE_CHANGED
class EBS : public EBSBase {
public:
    EBS() = default;
    ~EBS() override = default;
    EBS(const EBS& other) = delete;
    EBS(EBS&& other) noexcept = delete;
    EBS& operator=(const EBS& other) = delete;
    EBS& operator=(EBS&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerAmberWarningSignal.create(kAmberWarningSignalInterval, this, &EBS::onOneShotTimerAmberWarningSignal,
                                                   false) == false) {
            DWarning() << "[EBS] mOneShotTimerAmberWarningSignal Create Failed";
        }
        if (mOneShotTimerRedWarningSignal500ms.create(kRedWarningSignal500msInterval, this,
                                                      &EBS::onOneShotTimerRedWarningSignal500ms, false) == false) {
            DWarning() << "[EBS] mOneShotTimerRedWarningSignal500ms Create Failed";
        }
        if (mOneShotTimerRedWarningSignal300ms.create(kRedWarningSignal300msInterval, this,
                                                      &EBS::onOneShotTimerRedWarningSignal300ms, false) == false) {
            DWarning() << "[EBS] mOneShotTimerRedWarningSignal300ms Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleEBSAmberLamp();
        updateTelltaleEBSRedLamp();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleEBSAmberLamp();
        updateTelltaleEBSRedLamp();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltaleEBSAmberLamp();
        updateTelltaleEBSRedLamp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleEBSAmberLamp();
        updateTelltaleEBSRedLamp();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;
        mIsConfigOn = (mConfigEBS == ConfigEBS::EBS || mConfigEBS == ConfigEBS::EBSNASR || mConfigEBS == ConfigEBS::EBSNVDC);
        updateTelltaleEBSAmberLamp();
        updateTelltaleEBSRedLamp();
    }

    void onAbsEBSAmberWarningChanged(const ArgumentsAbsEBSAmberWarningChanged& args) {
        mABSEBSAmberWarningSignal = args.mInput_ABSEBSAmberWarningSignal;
        updateTelltaleEBSAmberLamp();
    }

    void onEbsRedWarningChanged(const ArgumentsEbsRedWarningChanged& args) {
        mEBSRedWarningSignal = args.mInput_EBSRedWarningSignal;
        updateTelltaleEBSRedLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTelltaleEBSAmberLamp() {
        if (!mIsConfigOn) {
            return;
        }
        SFCEBSTelltaleEBSAmberLampStat stat = SFCEBSTelltaleEBSAmberLampStat::OFF;
        HBool isTimerStatus = false;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCEBSTelltaleEBSAmberLampStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::ON) {
                isTimerStatus = true;
            } else if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::MESSAGE_TIMEOUT) {
                stat = SFCEBSTelltaleEBSAmberLampStat::ON;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        if (isTimerStatus == false) {
            mOneShotTimerAmberWarningSignal.stop();
            setSFCEBSTelltaleEBSAmberLampStat(stat);
        } else {
            if (mOneShotTimerAmberWarningSignal.isRunning() == false) {
                setSFCEBSTelltaleEBSAmberLampStat(SFCEBSTelltaleEBSAmberLampStat::OFF);
                if (mOneShotTimerAmberWarningSignal.start() == false) {
                    DWarning() << "[EBS] mOneShotTimerAmberWarningSignal Start Failed";
                }
            }
        }
    }

    void updateTelltaleEBSRedLamp() {
        if (!mIsConfigOn) {
            return;
        }
        SFCEBSTelltaleEBSRedLampStat stat = SFCEBSTelltaleEBSRedLampStat::OFF;
        RedWarningTimerStatus timerStatus = RedWarningTimerStatus::STOP;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCEBSTelltaleEBSRedLampStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mEBSRedWarningSignal == EBSRedWarningSignal::ON) {
                timerStatus = RedWarningTimerStatus::ON_500ms;
            } else if (mEBSRedWarningSignal == EBSRedWarningSignal::MESSAGE_TIMEOUT) {
                stat = SFCEBSTelltaleEBSRedLampStat::ON;
            } else if (mEBSRedWarningSignal == EBSRedWarningSignal::OFF) {
                timerStatus = RedWarningTimerStatus::OFF_300ms;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        if (timerStatus == RedWarningTimerStatus::STOP) {
            mOneShotTimerRedWarningSignal500ms.stop();
            mOneShotTimerRedWarningSignal300ms.stop();
            setSFCEBSTelltaleEBSRedLampStat(stat);
        } else if (timerStatus == RedWarningTimerStatus::ON_500ms) {
            mOneShotTimerRedWarningSignal300ms.stop();
            if (mOneShotTimerRedWarningSignal500ms.isRunning() == false) {
                setSFCEBSTelltaleEBSRedLampStat(SFCEBSTelltaleEBSRedLampStat::OFF);
                if (mOneShotTimerRedWarningSignal500ms.start() == false) {
                    DWarning() << "[EBS] mOneShotTimerRedWarningSignal500ms Start Failed";
                }
            }
        } else if (timerStatus == RedWarningTimerStatus::OFF_300ms) {
            mOneShotTimerRedWarningSignal500ms.stop();
            if (mOneShotTimerRedWarningSignal300ms.isRunning() == false) {
                setSFCEBSTelltaleEBSRedLampStat(SFCEBSTelltaleEBSRedLampStat::OFF);
                if (mOneShotTimerRedWarningSignal300ms.start() == false) {
                    DWarning() << "[EBS] mOneShotTimerRedWarningSignal300ms Start Failed";
                }
            }
        } else {
            // no operation
        }
    }

    void onOneShotTimerAmberWarningSignal() {
        if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::ON) {
            setSFCEBSTelltaleEBSAmberLampStat(SFCEBSTelltaleEBSAmberLampStat::ON);
            flush();
        }
    }

    void onOneShotTimerRedWarningSignal500ms() {
        if (mEBSRedWarningSignal == EBSRedWarningSignal::ON) {
            setSFCEBSTelltaleEBSRedLampStat(SFCEBSTelltaleEBSRedLampStat::ON);
            flush();
        }
    }

    void onOneShotTimerRedWarningSignal300ms() {
        if (mEBSRedWarningSignal == EBSRedWarningSignal::OFF) {
            setSFCEBSTelltaleEBSRedLampStat(SFCEBSTelltaleEBSRedLampStat::OFF);
            flush();
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    enum class RedWarningTimerStatus : HUInt64 {
        NONE,
        ON_500ms,
        OFF_300ms,
        STOP,
        MAX
    };

    using ConfigEBS = ArgumentsConfigChanged::Inter_ConfigEBS;
    using ABSEBSAmberWarningSignal = ArgumentsAbsEBSAmberWarningChanged::Input_ABSEBSAmberWarningSignal;
    using EBSRedWarningSignal = ArgumentsEbsRedWarningChanged::Input_EBSRedWarningSignal;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigEBS mConfigEBS = ConfigEBS::EBSNVDC;
    ABSEBSAmberWarningSignal mABSEBSAmberWarningSignal = ABSEBSAmberWarningSignal::OFF;
    EBSRedWarningSignal mEBSRedWarningSignal = EBSRedWarningSignal::OFF;
    bool mIsConfigOn = false;

    ssfs::SFCTimer<EBS> mOneShotTimerAmberWarningSignal;
    ssfs::SFCTimer<EBS> mOneShotTimerRedWarningSignal500ms;
    ssfs::SFCTimer<EBS> mOneShotTimerRedWarningSignal300ms;

    static constexpr HUInt64 kAmberWarningSignalInterval = 500u;     // 500ms
    static constexpr HUInt64 kRedWarningSignal500msInterval = 500u;  // 500ms
    static constexpr HUInt64 kRedWarningSignal300msInterval = 300u;  // 300ms
};

}  // namespace ccos

#endif  // SFSS_EBS_H
