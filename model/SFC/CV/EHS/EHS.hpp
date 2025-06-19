/**
 * @file EHS.hpp
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
#ifndef SFSS_EHS_H
#define SFSS_EHS_H

#define DLOG_ENABLED gEnableSFCLog

#include "EHSBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 9.0.1
// Spec Version : v0.24
// Reference : [CV170] EHS. Contains : Telltale, Sound. Recommend: VALUE_CHANGED
class EHS : public EHSBase {
public:
    EHS() = default;
    ~EHS() override = default;
    EHS(const EHS& other) = delete;
    EHS(EHS&& other) noexcept = delete;
    EHS& operator=(const EHS& other) = delete;
    EHS& operator=(EHS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEHSTelltaleEHSLampBlinkValueA(SFCEHSTelltaleEHSLampBlinkValueA::AMBER);
        setSFCEHSTelltaleEHSLampBlinkValueB(SFCEHSTelltaleEHSLampBlinkValueB::OFF);
        setSFCEHSSoundEHSReleaseID("SND_PopUpInform2");
        setSFCEHSSoundEHSReleaseStat(SFCEHSSoundEHSReleaseStat::OFF);
        setSFCEHSSoundEHSReleaseType(SFCEHSSoundEHSReleaseType::INTERVAL);
        setSFCEHSSoundEHSReleaseInterval(kSoundInterval);

        if (mOneShotTimerEHSStatus.create(kEHSStatInterval, this, &EHS::onOneShotTimerEHSStatus, false) == false) {
            DWarning() << "[EBS] mOneShotTimerEHSStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateSound();
    }

    void onConfigEBSChanged(const ArgumentsConfigEBSChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;
        updateTelltale();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;
        updateTelltale();
    }

    void onEhsSignalChanged(const ArgumentsEhsSignalChanged& args) {
        mEHSStatus = args.mInput_EHSStatus;
        updateTelltale();
        updateSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        HBool isTimerStatus = false;
        SFCEHSTelltaleEHSLampStat telltaleStatus = SFCEHSTelltaleEHSLampStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mEHSStatus) {
                case EHSStatus::BLINK_RELEASE:
                    telltaleStatus = SFCEHSTelltaleEHSLampStat::BLINK1;
                    break;
                case EHSStatus::RED:
                    if (mConfigEBS == ConfigEBS::ABS || mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC) {
                        if (mConfigBrakeAirType != ConfigBrakeAirType::OFF) {
                            telltaleStatus = SFCEHSTelltaleEHSLampStat::RED;
                        }
                    }
                    break;
                case EHSStatus::AMBER:
                    if (mConfigEBS == ConfigEBS::ABS || mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC) {
                        if (mConfigBrakeAirType == ConfigBrakeAirType::OFF) {
                            telltaleStatus = SFCEHSTelltaleEHSLampStat::AMBER;
                        } else {
                            isTimerStatus = true;
                        }
                    } else if (mConfigEBS == ConfigEBS::EBS || mConfigEBS == ConfigEBS::EBSNASR ||
                               mConfigEBS == ConfigEBS::EBSNVDC) {
                        if (mConfigBrakeAirType != ConfigBrakeAirType::OFF) {
                            isTimerStatus = true;
                        }
                    } else {
                        // no operation
                    }
                    break;
                default:
                    // no operation
                    break;
            }
        }

        if (isTimerStatus == false) {
            mOneShotTimerEHSStatus.stop();
            setSFCEHSTelltaleEHSLampStat(telltaleStatus);
        } else {
            if (mOneShotTimerEHSStatus.isRunning() == false) {
                setSFCEHSTelltaleEHSLampStat(SFCEHSTelltaleEHSLampStat::OFF);
                if (mOneShotTimerEHSStatus.start() == false) {
                    DWarning() << "[EBS] mOneShotTimerEHSStatus Start Failed";
                }
            }
        }
    }

    void onOneShotTimerEHSStatus() {
        if (mEHSStatus == EHSStatus::AMBER) {
            setSFCEHSTelltaleEHSLampStat(SFCEHSTelltaleEHSLampStat::AMBER);
            flush();
        }
    }

    void updateSound() {
        SFCEHSSoundEHSReleaseStat soundStatus = SFCEHSSoundEHSReleaseStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mEHSStatus == EHSStatus::BLINK_RELEASE) {
            soundStatus = SFCEHSSoundEHSReleaseStat::ON;
        }

        setSFCEHSSoundEHSReleaseStat(soundStatus);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    ssfs::SFCTimer<EHS> mOneShotTimerEHSStatus;

    static constexpr HUInt64 kEHSStatInterval = 600;
    static constexpr HUInt64 kSoundInterval = 240;

    using ConfigEBS = ArgumentsConfigEBSChanged::Inter_ConfigEBS;
    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;
    using EHSStatus = ArgumentsEhsSignalChanged::Input_EHSStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigEBS mConfigEBS = ConfigEBS::EBSNVDC;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::OFF;
    EHSStatus mEHSStatus = EHSStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_EHS_H
