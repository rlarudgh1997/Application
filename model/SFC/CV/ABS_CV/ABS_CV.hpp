/**
 * @file ABS_CV.hpp
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
#ifndef SFSS_ABS_CV_H
#define SFSS_ABS_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "ABS_CVBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 4.0.1
// Spec Version : v0.35
// Reference : [CV330] ABS_CV. Contains : Telltale. Recommend: VALUE_CHANGED
class ABS_CV : public ABS_CVBase {
public:
    ABS_CV() = default;
    ~ABS_CV() override = default;
    ABS_CV(const ABS_CV& other) = delete;
    ABS_CV(ABS_CV&& other) noexcept = delete;
    ABS_CV& operator=(const ABS_CV& other) = delete;
    ABS_CV& operator=(ABS_CV&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerEBSConditions.create(kEBSConditionsInterval, this, &ABS_CV::onOneShotTimerEBSConditions, false) ==
            false) {
            DWarning() << "[ABS_CV] onOneShotTimerEBSConditions Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateTelltale();
    }

    void onSignalABSEBSChanged(const ArgumentsSignalABSEBSChanged& args) {
        mABSWarningLampStatus = args.mInput_ABSWarningLampStatus;
        mABSEBSAmberWarningSignal = args.mInput_ABSEBSAmberWarningSignal;
        mABSFullyOperationalSignal = args.mInput_ABSFullyOperationalSignal;
        updateTelltale();
    }

    void onConfigEBSChanged(const ArgumentsConfigEBSChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;
        updateTelltale();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
    }

private:
    void updateTelltale() {
        SFCABS_CVTelltaleABS_CVStat stat = SFCABS_CVTelltaleABS_CVStat::OFF;
        HBool isTimerStatus = false;

        if (mConfigEBS == ConfigEBS::ABS || mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC) {
            if (mConfigBrakeAirType == ConfigBrakeAirType::OFF) {
                if (mIgnElapsed == IgnElapsed::ON_500ms) {
                    stat = SFCABS_CVTelltaleABS_CVStat::ON;
                } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                    if (mABSWarningLampStatus == ABSWarningLampStatus::ON ||
                        mABSWarningLampStatus == ABSWarningLampStatus::MESSAGE_TIMEOUT) {
                        stat = SFCABS_CVTelltaleABS_CVStat::ON;
                    }
                } else {
                    // no operation
                }
            } else if (mConfigBrakeAirType != ConfigBrakeAirType::OFF) {
                if (mIgnElapsed == IgnElapsed::ON_500ms) {
                    stat = SFCABS_CVTelltaleABS_CVStat::ON;
                } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                    if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::ON &&
                        mABSFullyOperationalSignal == ABSFullyOperationalSignal::OFF) {
                        isTimerStatus = true;
                    } else if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::MESSAGE_TIMEOUT &&
                               mABSFullyOperationalSignal == ABSFullyOperationalSignal::MESSAGE_TIMEOUT) {
                        stat = SFCABS_CVTelltaleABS_CVStat::ON;
                    } else {
                        // no operation
                    }
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        } else if (mConfigEBS == ConfigEBS::EBS || mConfigEBS == ConfigEBS::EBSNASR || mConfigEBS == ConfigEBS::EBSNVDC) {
            if (mConfigBrakeAirType != ConfigBrakeAirType::OFF) {
                if (mIgnElapsed == IgnElapsed::ON_500ms) {
                    stat = SFCABS_CVTelltaleABS_CVStat::ON;
                } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                    if (mConfigArea != ConfigArea::USA) {
                        if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::ON &&
                            mABSFullyOperationalSignal == ABSFullyOperationalSignal::OFF) {
                            isTimerStatus = true;
                        } else if (mABSEBSAmberWarningSignal == ABSEBSAmberWarningSignal::MESSAGE_TIMEOUT &&
                                   mABSFullyOperationalSignal == ABSFullyOperationalSignal::MESSAGE_TIMEOUT) {
                            stat = SFCABS_CVTelltaleABS_CVStat::ON;
                        } else {
                            // no operation
                        }
                    } else if (mConfigArea == ConfigArea::USA) {
                        if (mABSFullyOperationalSignal == ABSFullyOperationalSignal::OFF) {
                            isTimerStatus = true;
                        } else if (mABSFullyOperationalSignal == ABSFullyOperationalSignal::MESSAGE_TIMEOUT) {
                            stat = SFCABS_CVTelltaleABS_CVStat::ON;
                        } else {
                            // no operation
                        }
                    } else {
                        // no operation
                    }
                } else {
                    // no operation
                }
            }
        } else {
            // no operation
        }

        if (isTimerStatus == false) {
            mOneShotTimerEBSConditions.stop();
            setSFCABS_CVTelltaleABS_CVStat(stat);
        } else {
            if (mOneShotTimerEBSConditions.isRunning() == false) {
                setSFCABS_CVTelltaleABS_CVStat(SFCABS_CVTelltaleABS_CVStat::OFF);
                if (mOneShotTimerEBSConditions.start() == false) {
                    DWarning() << "[ABS_CV] mOneShotTimerEBSConditions Start Failed";
                }
            }
        }
    }

    void onOneShotTimerEBSConditions() {
        setSFCABS_CVTelltaleABS_CVStat(SFCABS_CVTelltaleABS_CVStat::ON);
        flush();
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ABSEBSAmberWarningSignal = ArgumentsSignalABSEBSChanged::Input_ABSEBSAmberWarningSignal;
    using ABSFullyOperationalSignal = ArgumentsSignalABSEBSChanged::Input_ABSFullyOperationalSignal;
    using ABSWarningLampStatus = ArgumentsSignalABSEBSChanged::Input_ABSWarningLampStatus;
    using ConfigEBS = ArgumentsConfigEBSChanged::Inter_ConfigEBS;
    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;

    ConfigEBS mConfigEBS = ConfigEBS::ABS;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ABSEBSAmberWarningSignal mABSEBSAmberWarningSignal = ABSEBSAmberWarningSignal::OFF;
    ABSFullyOperationalSignal mABSFullyOperationalSignal = ABSFullyOperationalSignal::OFF;
    ABSWarningLampStatus mABSWarningLampStatus = ABSWarningLampStatus::OFF;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::OFF;

    ssfs::SFCTimer<ABS_CV> mOneShotTimerEBSConditions;
    static constexpr HUInt64 kEBSConditionsInterval = 500u;  // 500ms
};

}  // namespace ccos

#endif  // SFSS_ABS_CV_H
