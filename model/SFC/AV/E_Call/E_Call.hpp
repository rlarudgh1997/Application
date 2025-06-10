/**
 * @file E_Call.hpp
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
#ifndef SFSS_E_Call_H
#define SFSS_E_Call_H

#define DLOG_ENABLED gEnableSFCLog

#include "E_CallBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 4.0.3
// Reference : [AV020] E_Call. Contains : Telltale. Recommend: VALUE_CHANGED
class E_Call : public E_CallBase {
public:
    E_Call() = default;
    ~E_Call() override = default;
    E_Call(const E_Call& other) = delete;
    E_Call(E_Call&& other) noexcept = delete;
    E_Call& operator=(const E_Call& other) = delete;
    E_Call& operator=(E_Call&& other) noexcept = delete;

    void onInitialize() override {
        setSFCE_CallTelltaleEcallStat(SFCE_CallTelltaleEcallStat::OFF);
        if (mOneShotTimerBatteryElapsed60s.create(kTimerBatteryElapsedInterval60s, this, &E_Call::onOneShotTimerBatteryElapsed60s,
                                                  false) == false) {
            DWarning() << "[E_Call] mOneShotTimerBatteryElapsed60s Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        if (mOneShotTimerBatteryElapsed60s.restart() == false) {
            DWarning() << "[E_Call] mOneShotTimerBatteryElapsed60s restart Failed";
        }
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        mOneShotTimerBatteryElapsed60s.stop();
        mMonitoring = true;
        updatePrivateEcallBootingMonitoring();
    }

    void onConfigEcallChanged(const ArgumentsConfigEcallChanged& args) {
        mConfigECALL = args.mInter_ConfigECALL;
        updateTelltale();
    }

    void onEcallWarnChanged(const ArgumentsEcallWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_ECallWarnStatus) == true) {
            mECallWarnStatus = ECallWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mECallWarnStatus = args.mInput_ECallWarnStatus;
        }
        updatePrivateEcallBootingMonitoring();
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void onOneShotTimerBatteryElapsed60s() {
        mMonitoring = false;
        updatePrivateEcallBootingMonitoring();
        updateTelltale();
        flush();
    }

    void updatePrivateEcallBootingMonitoring() {
        if (mMonitoring == true) {
            if (mECallWarnStatus == ECallWarnStatus::UNHANDLED_TIMEOUT) {
                mPrivateECallBootingMonitoring = PrivateEcallBootingMonitoring::MONITORING_STATUS;
            } else {
                mPrivateECallBootingMonitoring = PrivateEcallBootingMonitoring::ACTIVATION_STATUS;
            }
        } else {
            mPrivateECallBootingMonitoring = PrivateEcallBootingMonitoring::ACTIVATION_STATUS;
        }
    }

    void updateTelltale() {
        if (mConfigECALL == ConfigECALL::OFF) {
            return;
        }

        mPrivateECallWarnStatus = PrivateECallWarnStatus::NONE;
        switch (mIgnElapsed) {
            case IgnElapsed::OFF_0ms:
                switch (mECallWarnStatus) {
                    case ECallWarnStatus::WARN:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::ON;
                        break;
                    default:
                        // case ECallWarnStatus::NORMAL:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::OFF;
                        break;
                }
                break;
            case IgnElapsed::ON_3500ms:
                switch (mECallWarnStatus) {
                    case ECallWarnStatus::WARN:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::ON;
                        break;
                    case ECallWarnStatus::UNHANDLED_TIMEOUT:
                        if (mPrivateECallBootingMonitoring == PrivateEcallBootingMonitoring::ACTIVATION_STATUS) {
                            mPrivateECallWarnStatus = PrivateECallWarnStatus::ON;
                        }
                        break;
                    default:
                        // case ECallWarnStatus::NORMAL:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::OFF;
                        break;
                }
                break;
            case IgnElapsed::ON_0ms:
                switch (mECallWarnCurrentStatus) {
                    case SFCE_CallTelltaleEcallStat::ON:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::ON;
                        break;
                    default:
                        // case SFCE_CallTelltaleEcallStat::OFF:
                        mPrivateECallWarnStatus = PrivateECallWarnStatus::OFF;
                        break;
                }
                break;
            case IgnElapsed::ON_500ms:
                mPrivateECallWarnStatus = PrivateECallWarnStatus::ON;
                break;
            default:
                break;
        }

        if (mPrivateECallWarnStatus == PrivateECallWarnStatus::ON) {
            setSFCE_CallTelltaleEcallStat(SFCE_CallTelltaleEcallStat::ON);
            mECallWarnCurrentStatus = SFCE_CallTelltaleEcallStat::ON;
        } else {  // OFF or NONE
            setSFCE_CallTelltaleEcallStat(SFCE_CallTelltaleEcallStat::OFF);
            mECallWarnCurrentStatus = SFCE_CallTelltaleEcallStat::OFF;
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    enum class PrivateECallWarnStatus : HUInt64 {
        NONE,
        OFF,
        ON,
        MAX
    };

    enum class PrivateEcallBootingMonitoring : HUInt64 {
        NONE,
        MONITORING_STATUS,
        ACTIVATION_STATUS,
        MAX
    };

    static constexpr uint32_t kTimerBatteryElapsedInterval60s = 60 * 1000;

    using ConfigECALL = ArgumentsConfigEcallChanged::Inter_ConfigECALL;
    using ECallWarnStatus = ArgumentsEcallWarnChanged::Input_ECallWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    bool mMonitoring = true;
    ConfigECALL mConfigECALL = ConfigECALL::OFF;
    ECallWarnStatus mECallWarnStatus = ECallWarnStatus::NORMAL;
    SFCE_CallTelltaleEcallStat mECallWarnCurrentStatus = SFCE_CallTelltaleEcallStat::NONE;
    PrivateECallWarnStatus mPrivateECallWarnStatus = PrivateECallWarnStatus::NONE;
    PrivateEcallBootingMonitoring mPrivateECallBootingMonitoring = PrivateEcallBootingMonitoring::NONE;

    ssfs::SFCTimer<E_Call> mOneShotTimerBatteryElapsed60s;
};

}  // namespace ccos

#endif  // SFSS_E_Call_H
