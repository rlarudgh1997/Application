/**
 * @file DPF_CV.hpp
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
#ifndef SFSS_DPF_CV_H
#define SFSS_DPF_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "DPF_CVBase.hpp"

namespace ccos {

// SFC Version : 10.0.1
// Spec Version : v0.34
// Reference : [CV500] DPF_CV. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class DPF_CV : public DPF_CVBase {
public:
    DPF_CV() = default;
    ~DPF_CV() override = default;
    DPF_CV(const DPF_CV& other) = delete;
    DPF_CV(DPF_CV&& other) noexcept = delete;
    DPF_CV& operator=(const DPF_CV& other) = delete;
    DPF_CV& operator=(DPF_CV&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1 Event - DPFRegenStopStatus
        setSFCDPF_CVEventEngineDPFRegenStopLampID("E75001");
        setSFCDPF_CVEventEngineDPFRegenStopLampLinkedSoundID("SND_PopUpWarn1");
        setSFCDPF_CVEventEngineDPFRegenStopLampLinkedSoundType(SFCDPF_CVEventEngineDPFRegenStopLampLinkedSoundType::REPEAT_COUNT);
        setSFCDPF_CVEventEngineDPFRegenStopLampLinkedSoundRepeatCount(1);

        // 5.3.4 Event - DPFRegenPopupStatus
        setSFCDPF_CVEventDPFRegenPopupID("E75006");
        setSFCDPF_CVEventDPFRegenPopupLinkedSoundID("SND_PopUpWarn2");
        setSFCDPF_CVEventDPFRegenPopupLinkedSoundType(SFCDPF_CVEventDPFRegenPopupLinkedSoundType::DURATION);
        setSFCDPF_CVEventDPFRegenPopupLinkedSoundDuration(kDPFRegenPopupDuration);

        // 5.3.5 Event - EngineTVSRwarning
        setSFCDPF_CVEventEngineTVSRwarningID("E75007");
        setSFCDPF_CVEventEngineTVSRwarningLinkedSoundID("SND_PopUpInform2");
        setSFCDPF_CVEventEngineTVSRwarningLinkedSoundType(SFCDPF_CVEventEngineTVSRwarningLinkedSoundType::REPEAT_COUNT);
        setSFCDPF_CVEventEngineTVSRwarningLinkedSoundRepeatCount(1);

        // 5.1.1 Telltale - DPFAutoRegenStatus
        setSFCDPF_CVTelltaleDPFAutoRegenBlinkValueA(SFCDPF_CVTelltaleDPFAutoRegenBlinkValueA::YELLOW);
        setSFCDPF_CVTelltaleDPFAutoRegenBlinkValueB(SFCDPF_CVTelltaleDPFAutoRegenBlinkValueB::OFF);

        // 5.1.2 Telltale - DPFRegenStopStatus
        setSFCDPF_CVTelltaleDPFRegenStopBlinkValueA(SFCDPF_CVTelltaleDPFRegenStopBlinkValueA::YELLOW);
        setSFCDPF_CVTelltaleDPFRegenStopBlinkValueB(SFCDPF_CVTelltaleDPFRegenStopBlinkValueB::OFF);

        // 5.1.3 Telltale - DPFCoolingFanStatus
        setSFCDPF_CVTelltaleDPFCoolingFanBlinkValueA(SFCDPF_CVTelltaleDPFCoolingFanBlinkValueA::ON);
        setSFCDPF_CVTelltaleDPFCoolingFanBlinkValueB(SFCDPF_CVTelltaleDPFCoolingFanBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateTelltaleDPFCoolingFanStatus();
        updateConstantSootGauge();
        updateEventEngineDPFRegenStopLamp();
        updateEventSootLevelWarningStatus();
        updateEventDPFManualRegenWarning();
        updateEventDPFAutoRegenWarning();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateTelltaleDPFAutoRegenStatus();
        updateTelltaleDPFRegenStopStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateTelltaleDPFAutoRegenStatus();
        updateTelltaleDPFRegenStopStatus();
        updateTelltaleDPFCoolingFanStatus();
        updateConstantSootGauge();
        updateEventEngineDPFRegenStopLamp();
        updateEventSootLevelWarningStatus();
        updateEventDPFManualRegenWarning();
        updateEventDPFAutoRegenWarning();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleDPFAutoRegenStatus();
        updateTelltaleDPFRegenStopStatus();
        updateTelltaleDPFCoolingFanStatus();
        updateConstantSootGauge();
        updateEventEngineDPFRegenStopLamp();
        updateEventSootLevelWarningStatus();
        updateEventDPFManualRegenWarning();
        updateEventDPFAutoRegenWarning();
    }

    void onConfigFuelTypeChanged(const ArgumentsConfigFuelTypeChanged& args) {
        mConfigFuelTypeCV = args.mInter_ConfigFuelTypeCV;
        updateTelltaleDPFAutoRegenStatus();
        updateTelltaleDPFRegenStopStatus();
        updateTelltaleDPFCoolingFanStatus();
        updateConstantSootGauge();
        updateEventEngineDPFRegenStopLamp();
        updateEventSootLevelWarningStatus();
        updateEventDPFManualRegenWarning();
        updateEventDPFAutoRegenWarning();
    }

    void onConfigVehicleTypeCVChanged(const ArgumentsConfigVehicleTypeCVChanged& args) {
        mConfigVehicleTypeCV = args.mInter_ConfigVehicleTypeCV;

        updateTelltaleDPFCoolingFanStatus();
    }

    void onDpfAutoRegenStatusChanged(const ArgumentsDpfAutoRegenStatusChanged& args) {
        mDPFAutoRegenStatus = args.mInput_DPFAutoRegenStatus;
        updateTelltaleDPFAutoRegenStatus();
    }

    void onDpfManualRegenStatusChanged(const ArgumentsDpfManualRegenStatusChanged& args) {
        mDPFManualRegenStatus = args.mInput_DPFManualRegenStatus;
        updateTelltaleDPFAutoRegenStatus();
        updateEventDPFManualRegenWarning();
    }

    void onDpfCoolingFanStatusChanged(const ArgumentsDpfCoolingFanStatusChanged& args) {
        mDPFCoolingFanStatus = args.mInput_DPFCoolingFanStatus;
        updateTelltaleDPFCoolingFanStatus();
    }

    void onSootLevelValueChanged(const ArgumentsSootLevelValueChanged& args) {
        mSootLevelValue = args.mInput_SootLevelValue;
        updateConstantSootGauge();
        updateEventSootLevelWarningStatus();
    }

    void onEngineDpfRegenStopLampChanged(const ArgumentsEngineDpfRegenStopLampChanged& args) {
        mEngineDPFRegenStopLamp = args.mInput_EngineDPFRegenStopLamp;
        updateTelltaleDPFRegenStopStatus();
        updateEventEngineDPFRegenStopLamp();
    }

    void onEngineTVSRwarningChanged(const ArgumentsEngineTVSRwarningChanged& args) {
        mEngineTVSRwarning = args.mInput_EngineTVSRwarning;
        updateEventDPFAutoRegenWarning();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleDPFAutoRegenStatus() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        SFCDPF_CVTelltaleDPFAutoRegenStat stat = SFCDPF_CVTelltaleDPFAutoRegenStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mDPFManualRegenStatus == DPFManualRegenStatus::ON) {
            stat = SFCDPF_CVTelltaleDPFAutoRegenStat::YELLOW;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mDPFAutoRegenStatus == DPFAutoRegenStatus::ON &&
                   mDPFManualRegenStatus == DPFManualRegenStatus::OFF) {
            stat = SFCDPF_CVTelltaleDPFAutoRegenStat::GREEN;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms && mDPFManualRegenStatus == DPFManualRegenStatus::BLINK) {
            stat = SFCDPF_CVTelltaleDPFAutoRegenStat::BLINK2;
        } else {
            // OFF
        }
        setSFCDPF_CVTelltaleDPFAutoRegenStat(stat);
    }

    void updateTelltaleDPFRegenStopStatus() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        SFCDPF_CVTelltaleDPFRegenStopStat stat = SFCDPF_CVTelltaleDPFRegenStopStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mEngineDPFRegenStopLamp == EngineDPFRegenStopLamp::ON) {
            stat = SFCDPF_CVTelltaleDPFRegenStopStat::BLINK2;
        }
        setSFCDPF_CVTelltaleDPFRegenStopStat(stat);
    }

    void updateTelltaleDPFCoolingFanStatus() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        SFCDPF_CVTelltaleDPFCoolingFanStat stat = SFCDPF_CVTelltaleDPFCoolingFanStat::OFF;
        if (mIsIgnOn == true && mConfigVehicleTypeCV == ConfigVehicleTypeCV::BUS) {
            if (mDPFCoolingFanStatus == DPFCoolingFanStatus::ON) {
                stat = SFCDPF_CVTelltaleDPFCoolingFanStat::ON;
            } else if (mDPFCoolingFanStatus == DPFCoolingFanStatus::BLINK2) {
                stat = SFCDPF_CVTelltaleDPFCoolingFanStat::BLINK2;
            } else {
                // no operation
            }
        }
        setSFCDPF_CVTelltaleDPFCoolingFanStat(stat);
    }

    void updateConstantSootGauge() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }

        SFCDPF_CVConstantSootGaugeStat stat = SFCDPF_CVConstantSootGaugeStat::SEG0;
        if (mIsIgnOn == true && 0 <= mSootLevelValue && mSootLevelValue <= 10) {
            stat = static_cast<SFCDPF_CVConstantSootGaugeStat>(mSootLevelValue);
        }
        setSFCDPF_CVConstantSootGaugeStat(stat);
    }

    void updateEventEngineDPFRegenStopLamp() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        SFCDPF_CVEventEngineDPFRegenStopLampStat stat = SFCDPF_CVEventEngineDPFRegenStopLampStat::OFF;
        if (mIsIgnOn == true && mEngineDPFRegenStopLamp == EngineDPFRegenStopLamp::ON) {
            stat = SFCDPF_CVEventEngineDPFRegenStopLampStat::ON;
        }
        setSFCDPF_CVEventEngineDPFRegenStopLampStat(stat);
    }

    void updateEventSootLevelWarningStatus() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        std::string eventID;
        std::string linkedSoundID;
        SFCDPF_CVEventDPFSootLevelWarningLinkedSoundType linkedSoundType = SFCDPF_CVEventDPFSootLevelWarningLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;
        HUInt64 durantion = 0;

        if (mIsIgnOn == true) {
            if (mSootLevelValue == 6 || mSootLevelValue == 7) {
                eventID = "E75003";
                linkedSoundID = "SND_PopUpInform2";
                linkedSoundType = SFCDPF_CVEventDPFSootLevelWarningLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mSootLevelValue == 8 || mSootLevelValue == 9) {
                eventID = "E75004";
                linkedSoundID = "SND_PopUpWarn1";
                linkedSoundType = SFCDPF_CVEventDPFSootLevelWarningLinkedSoundType::REPEAT_COUNT;
                repeatCount = 1;
            } else if (mSootLevelValue == 10) {
                eventID = "E75005";
                linkedSoundID = "SND_PopUpWarn2";
                linkedSoundType = SFCDPF_CVEventDPFSootLevelWarningLinkedSoundType::DURATION;
                durantion = kDPFSootLevelDuration;
            } else {
                // do nothing
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.DPF_CV.Event.DPFSootLevelWarning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCDPF_CVEventDPFSootLevelWarningStat(SFCDPF_CVEventDPFSootLevelWarningStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCDPF_CVEventDPFSootLevelWarningID(eventID);
            setSFCDPF_CVEventDPFSootLevelWarningStat(SFCDPF_CVEventDPFSootLevelWarningStat::ON);
            setSFCDPF_CVEventDPFSootLevelWarningLinkedSoundID(linkedSoundID);
            setSFCDPF_CVEventDPFSootLevelWarningLinkedSoundType(linkedSoundType);
            setSFCDPF_CVEventDPFSootLevelWarningLinkedSoundRepeatCount(repeatCount);
            setSFCDPF_CVEventDPFSootLevelWarningLinkedSoundDuration(durantion);
        }
    }

    void updateEventDPFManualRegenWarning() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        SFCDPF_CVEventDPFRegenPopupStat stat = SFCDPF_CVEventDPFRegenPopupStat::OFF;
        if (mIsIgnOn == true && mDPFManualRegenStatus == DPFManualRegenStatus::BLINK) {
            stat = SFCDPF_CVEventDPFRegenPopupStat::ON;
        }
        setSFCDPF_CVEventDPFRegenPopupStat(stat);
    }

    void updateEventDPFAutoRegenWarning() {
        if (mConfigVehicleType != ConfigVehicleType::ICV || mConfigFuelTypeCV != ConfigFuelTypeCV::DSL) {
            return;
        }
        SFCDPF_CVEventEngineTVSRwarningStat stat = SFCDPF_CVEventEngineTVSRwarningStat::OFF;
        if (mIsIgnOn == true && mEngineTVSRwarning == EngineTVSRwarning::ON) {
            stat = SFCDPF_CVEventEngineTVSRwarningStat::ON;
        }
        setSFCDPF_CVEventEngineTVSRwarningStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        MAX
    };

    using DPFAutoRegenStatus = ArgumentsDpfAutoRegenStatusChanged::Input_DPFAutoRegenStatus;
    using DPFManualRegenStatus = ArgumentsDpfManualRegenStatusChanged::Input_DPFManualRegenStatus;
    using DPFCoolingFanStatus = ArgumentsDpfCoolingFanStatusChanged::Input_DPFCoolingFanStatus;
    using EngineDPFRegenStopLamp = ArgumentsEngineDpfRegenStopLampChanged::Input_EngineDPFRegenStopLamp;
    using EngineTVSRwarning = ArgumentsEngineTVSRwarningChanged::Input_EngineTVSRwarning;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigFuelTypeCV = ArgumentsConfigFuelTypeChanged::Inter_ConfigFuelTypeCV;
    using ConfigVehicleTypeCV = ArgumentsConfigVehicleTypeCVChanged::Inter_ConfigVehicleTypeCV;

    static constexpr HUInt64 kDPFRegenPopupDuration = 2000;  // 2s
    static constexpr HUInt64 kDPFSootLevelDuration = 2000;   // 2s

    bool mIsIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HUInt64 mSootLevelValue = 0;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigFuelTypeCV mConfigFuelTypeCV = ConfigFuelTypeCV::DSL;
    ConfigVehicleTypeCV mConfigVehicleTypeCV = ConfigVehicleTypeCV::TRUCK;

    DPFAutoRegenStatus mDPFAutoRegenStatus = DPFAutoRegenStatus::OFF;
    DPFManualRegenStatus mDPFManualRegenStatus = DPFManualRegenStatus::OFF;
    DPFCoolingFanStatus mDPFCoolingFanStatus = DPFCoolingFanStatus::OFF;
    EngineDPFRegenStopLamp mEngineDPFRegenStopLamp = EngineDPFRegenStopLamp::OFF;
    EngineTVSRwarning mEngineTVSRwarning = EngineTVSRwarning::OFF;
};

}  // namespace ccos

#endif  // SFSS_DPF_CV_H
