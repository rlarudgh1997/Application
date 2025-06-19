/**
 * @file Digital_Clock.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_Digital_Clock_H
#define SFSS_Digital_Clock_H

#define DLOG_ENABLED gEnableSFCLog

#include "Digital_ClockBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.16
// Reference : [CV680] Digital_Clock. Contains : Constant. Recommend: VALUE_CHANGED
class Digital_Clock : public Digital_ClockBase {
public:
    Digital_Clock() = default;
    ~Digital_Clock() override = default;
    Digital_Clock(const Digital_Clock& other) = delete;
    Digital_Clock(Digital_Clock&& other) noexcept = delete;
    Digital_Clock& operator=(const Digital_Clock& other) = delete;
    Digital_Clock& operator=(Digital_Clock&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn0ms) == false) {
            return;
        }

        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff0ms) == false) {
            return;
        }

        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onHuHoursValueChanged(const ArgumentsHuHoursValueChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInput_HUHoursValue) == false) {
            return;
        }

        mHUHoursValue = args.mInput_HUHoursValue;
        updateConstantHuTimeFormat();
        updateConstantHoursValue();
        updateOutputCLUHoursValue();
    }

    void onHuMinutesChagned(const ArgumentsHuMinutesChagned& args) {
        if (ISINITIALIZEDSIGNAL(args.mInput_HUMinutesValue) == false) {
            return;
        }

        mHUMinutesValue = args.mInput_HUMinutesValue;
        updateConstantMinutesValue();
        updateOutputCLUMinutesValue();
    }

    void onHuSecondsChanged(const ArgumentsHuSecondsChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInput_HUSecondsValue) == false) {
            return;
        }

        mHUSecondsValue = args.mInput_HUSecondsValue;
        updateOutputCLUSecondsValue();
    }

    void onHuTimeFormatStatusChanged(const ArgumentsHuTimeFormatStatusChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInput_HUTimeFormatStatus) == false) {
            return;
        }

        mHUTimeFormatStatus = args.mInput_HUTimeFormatStatus;
        updateConstantHuTimeFormat();
        updateConstantHoursValue();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateConstantHuTimeFormat();
        updateConstantHoursValue();
        updateConstantMinutesValue();
        updateOutputCLUHoursValue();
        updateOutputCLUMinutesValue();
        updateOutputCLUSecondsValue();
    }

    void updateConstantHuTimeFormat() {
        SFCDigital_ClockConstantTimeFormatStat stat = SFCDigital_ClockConstantTimeFormatStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mHUTimeFormatStatus == HUTimeFormatStatus::HOUR_12) {
                if (mHUHoursValue >= 0 && mHUHoursValue <= 11) {
                    stat = SFCDigital_ClockConstantTimeFormatStat::AM;
                } else if (mHUHoursValue >= 12 && mHUHoursValue <= 23) {
                    stat = SFCDigital_ClockConstantTimeFormatStat::PM;
                } else {
                    // stat OFF(Default)
                }
            }
        }

        setSFCDigital_ClockConstantTimeFormatStat(stat);
    }

    void updateConstantHoursValue() {
        HUInt64 value = kDefaultTimeOffValue;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mHUTimeFormatStatus == HUTimeFormatStatus::HOUR_12) {
                if (mHUHoursValue == 0) {
                    value = 12;
                } else if (mHUHoursValue >= 1 && mHUHoursValue <= 12) {
                    value = mHUHoursValue;
                } else if (mHUHoursValue >= 13 && mHUHoursValue <= 23) {
                    value = mHUHoursValue - 12;
                } else {
                    // value = 255(Default)
                }
            } else if (mHUTimeFormatStatus == HUTimeFormatStatus::HOUR_24 && mHUHoursValue >= 0 && mHUHoursValue <= 23) {
                value = mHUHoursValue;
            } else {
                // value = 255(Default)
            }
        }

        setSFCDigital_ClockConstantHoursValue(value);
    }

    void updateConstantMinutesValue() {
        HUInt64 value = kDefaultTimeOffValue;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mHUMinutesValue >= 0 && mHUMinutesValue <= 59) {
            value = mHUMinutesValue;
        }

        setSFCDigital_ClockConstantMinutesValue(value);
    }

    void updateOutputCLUHoursValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mActuatorOutputAddrList.emplace_back(Vehicle.CV.Digital_Clock.Output_CLUHoursValue, mHUHoursValue);
            flushActuatorOutput();
        }
    }

    void updateOutputCLUMinutesValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mActuatorOutputAddrList.emplace_back(Vehicle.CV.Digital_Clock.Output_CLUMinutesValue, mHUMinutesValue);
            flushActuatorOutput();
        }
    }

    void updateOutputCLUSecondsValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mActuatorOutputAddrList.emplace_back(Vehicle.CV.Digital_Clock.Output_CLUSecondsValue, mHUSecondsValue * 4);
            flushActuatorOutput();
        }
    }

    void flushActuatorOutput() {
        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using HUTimeFormatStatus = ArgumentsHuTimeFormatStatusChanged::Input_HUTimeFormatStatus;

    static constexpr HUInt64 kDefaultTimeOffValue = 255;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HUTimeFormatStatus mHUTimeFormatStatus = HUTimeFormatStatus::OFF;
    HUInt64 mHUHoursValue = 0;
    HUInt64 mHUMinutesValue = 0;
    HUInt64 mHUSecondsValue = 0;
    AddressValueList mActuatorOutputAddrList;
};

}  // namespace ccos

#endif  // SFSS_Digital_Clock_H
