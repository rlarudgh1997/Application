/**
 * @file PDC.hpp
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
#ifndef SFSS_PDC_H
#define SFSS_PDC_H

#define DLOG_ENABLED gEnableSFCLog

#include "PDCBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.22
// Reference : [CV900] PDC. Contains : Constant, Event, Sound. Recommend: VALUE_CHANGED
class PDC : public PDCBase {
public:
    PDC() = default;
    ~PDC() override = default;
    PDC(const PDC& other) = delete;
    PDC(PDC&& other) noexcept = delete;
    PDC& operator=(const PDC& other) = delete;
    PDC& operator=(PDC&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.3 배터리 방전 경고
        setSFCPDCEventPDCBatteryDischargeID("E79004");
        setSFCPDCEventPDCBatteryDischargeLinkedSoundID("SND_PopUpWarn1");
        setSFCPDCEventPDCBatteryDischargeLinkedSoundType(SFCPDCEventPDCBatteryDischargeLinkedSoundType::REPEAT_COUNT);
        setSFCPDCEventPDCBatteryDischargeLinkedSoundRepeatCount(1);

        setSFCPDCSoundWaterPumpID("SND_WaterPump");
        setSFCPDCSoundWaterPumpType(SFCPDCSoundWaterPumpType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateConstantPDC24VSocValue();
        updateConstantPDC12VSocValue();
        updateEventPDCPowerAutoCutModeStatus();
        updateEventPDCBatteryDischargeStatus();
        updateSoundWaterPumpStatus();
        updatePrivate24VBatteryVoltageValue();
        updateConstant24VBatteryVoltageValue();
        updatePrivate12VBatteryVoltageValue();
        updateConstant12VBatteryVoltageValue();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateConstantPDC24VSocValue();
        updateConstantPDC12VSocValue();
        updateEventPDCPowerAutoCutModeStatus();
        updateEventPDCBatteryDischargeStatus();
        updateSoundWaterPumpStatus();
        updatePrivate24VBatteryVoltageValue();
        updateConstant24VBatteryVoltageValue();
        updatePrivate12VBatteryVoltageValue();
        updateConstant12VBatteryVoltageValue();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateConstantPDC12VSocValue();
        updateConstant12VBatteryVoltageValue();
    }

    void onPdc24VSocValueChanged(const ArgumentsPdc24VSocValueChanged& args) {
        mPdc24VSocValue = args.mInput_PDC24VSocValue;
        updateConstantPDC24VSocValue();
    }

    void onPdc12VSocValueChanged(const ArgumentsPdc12VSocValueChanged& args) {
        mPdc12VSocValue = args.mInput_PDC12VSocValue;
        updateConstantPDC12VSocValue();
    }

    void onPdc24VVoltageValueChanged(const ArgumentsPdc24VVoltageValueChanged& args) {
        mPdc24VVoltageValue = args.mInput_PDC24VVoltageValue;
        updatePrivate24VBatteryVoltageValue();
        updateConstant24VBatteryVoltageValue();
    }

    void onPdc12VVoltageValueChanged(const ArgumentsPdc12VVoltageValueChanged& args) {
        mPdc12VVoltageValue = args.mInput_PDC12VVoltageValue;
        updatePrivate12VBatteryVoltageValue();
        updateConstant12VBatteryVoltageValue();
    }

    void onPdcPowerAutoCutModeStatusChanged(const ArgumentsPdcPowerAutoCutModeStatusChanged& args) {
        mPDCPowerAutoCutModeStatus = args.mInput_PDCPowerAutoCutModeStatus;
        updateEventPDCPowerAutoCutModeStatus();
    }

    void onPdcBatteryDischargeStatusChanged(const ArgumentsPdcBatteryDischargeStatusChanged& args) {
        mPDCBatteryDischargeStatus = args.mInput_PDCBatteryDischargeStatus;
        updateEventPDCBatteryDischargeStatus();
    }

    void onWaterPumpStatusChanged(const ArgumentsWaterPumpStatusChanged& args) {
        mWaterPumpStatus = args.mInput_WaterPumpStatus;
        updateSoundWaterPumpStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateConstantPDC24VSocValue() {
        HUInt64 value = kPDC24VSocValue_254;

        if (mIgnElapsed == IgnElapsed::ON_0ms && (0 <= mPdc24VSocValue && mPdc24VSocValue <= 100)) {
            value = mPdc24VSocValue;
        }
        setSFCPDCConstantPDC24VBatterySocValue(value);
    }

    void updateConstantPDC12VSocValue() {
        HUInt64 value = kPDC12VSocValue_255;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) {
                if (0 <= mPdc12VSocValue && mPdc12VSocValue <= 100) {
                    value = mPdc12VSocValue;
                } else {  // !(0 ~ 100)
                    value = kPDC12VSocValue_254;
                }
            }
        }
        setSFCPDCConstantPDC12VBatterySocValue(value);
    }

    void updatePrivate24VBatteryVoltageValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mPrivate24VBatteryVoltageValue = std::floor((mPdc24VVoltageValue * 0.001) * 10.0) / 10.0;
        }
    }

    void updatePrivate12VBatteryVoltageValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            mPrivate12VBatteryVoltageValue = std::floor(((mPdc12VVoltageValue * 0.001) + 6.0) * 10.0) / 10.0;
        }
    }

    void updateConstant24VBatteryVoltageValue() {
        HDouble value = 0;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivate24VBatteryVoltageValue < 16.0) {
                value = 16.0;
            } else if ((mPrivate24VBatteryVoltageValue >= 16.0) && (mPrivate24VBatteryVoltageValue <= 32.0)) {
                value = mPrivate24VBatteryVoltageValue;
            } else if (mPrivate24VBatteryVoltageValue > 32.0) {
                value = 32.0;
            } else {
                // no operation
            }
        }
        setSFCPDCConstantPDC24VBatteryVoltageValue(value);
    }

    void updateConstant12VBatteryVoltageValue() {
        HDouble value = 0;
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV)) {
            if (mPrivate12VBatteryVoltageValue < 8.0) {
                value = 8.0;
            } else if ((mPrivate12VBatteryVoltageValue >= 8.0) && (mPrivate12VBatteryVoltageValue <= 16.0)) {
                value = mPrivate12VBatteryVoltageValue;
            } else if (mPrivate12VBatteryVoltageValue > 16.0) {
                value = 16.0;
            } else {
                // no operation
            }
        }
        setSFCPDCConstantPDC12VBatteryVoltageValue(value);
    }

    void updateEventPDCPowerAutoCutModeStatus() {
        // E79001 ~ E79002
        std::string eventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPDCPowerAutoCutModeStatus == PDCPowerAutoCutModeStatus::FACTORY_MODE) {
                eventID = "E79001";
            } else if (mPDCPowerAutoCutModeStatus == PDCPowerAutoCutModeStatus::DELIVERY_MODE) {
                eventID = "E79002";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.PDC.Event.PDCPowerAutoCutMode.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCPDCEventPDCPowerAutoCutModeStat(SFCPDCEventPDCPowerAutoCutModeStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCPDCEventPDCPowerAutoCutModeID(eventID);
            setSFCPDCEventPDCPowerAutoCutModeStat(SFCPDCEventPDCPowerAutoCutModeStat::ON);
        }
    }

    void updateEventPDCBatteryDischargeStatus() {
        // E79004
        SFCPDCEventPDCBatteryDischargeStat stat = SFCPDCEventPDCBatteryDischargeStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mPDCBatteryDischargeStatus == PDCBatteryDischargeStatus::ON) {
            stat = SFCPDCEventPDCBatteryDischargeStat::ON;
        }
        setSFCPDCEventPDCBatteryDischargeStat(stat);
    }

    void updateSoundWaterPumpStatus() {
        // SND_WaterPump
        SFCPDCSoundWaterPumpStat stat = SFCPDCSoundWaterPumpStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mWaterPumpStatus == WaterPumpStatus::ON) {
            stat = SFCPDCSoundWaterPumpStat::ON;
        }
        setSFCPDCSoundWaterPumpStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using PDCPowerAutoCutModeStatus = ArgumentsPdcPowerAutoCutModeStatusChanged::Input_PDCPowerAutoCutModeStatus;
    using PDCBatteryDischargeStatus = ArgumentsPdcBatteryDischargeStatusChanged::Input_PDCBatteryDischargeStatus;
    using WaterPumpStatus = ArgumentsWaterPumpStatusChanged::Input_WaterPumpStatus;

    static constexpr HUInt64 kPDC24VSocValue_254 = 254;
    static constexpr HUInt64 kPDC12VSocValue_254 = 254;
    static constexpr HUInt64 kPDC12VSocValue_255 = 255;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    HUInt64 mPdc24VSocValue = 0;
    HUInt64 mPdc12VSocValue = 0;
    HDouble mPdc24VVoltageValue = 0;
    HDouble mPdc12VVoltageValue = 0;
    HDouble mPrivate24VBatteryVoltageValue = 0;
    HDouble mPrivate12VBatteryVoltageValue = 0;
    PDCPowerAutoCutModeStatus mPDCPowerAutoCutModeStatus = PDCPowerAutoCutModeStatus::OFF;
    PDCBatteryDischargeStatus mPDCBatteryDischargeStatus = PDCBatteryDischargeStatus::OFF;
    WaterPumpStatus mWaterPumpStatus = WaterPumpStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_PDC_H
