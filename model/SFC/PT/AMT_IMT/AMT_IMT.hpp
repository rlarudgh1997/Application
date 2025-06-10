/**
 * @file AMT_IMT.hpp
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
#ifndef SFSS_AMT_IMT_H
#define SFSS_AMT_IMT_H

#define DLOG_ENABLED gEnableSFCLog

#include "AMT_IMTBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Reference : [PT010] AMT&IMT. Contains : Event, Sound. Recommend: VALUE_CHANGED
class AMT_IMT : public AMT_IMTBase {
public:
    AMT_IMT() = default;
    ~AMT_IMT() override = default;
    AMT_IMT(const AMT_IMT& other) = delete;
    AMT_IMT(AMT_IMT&& other) noexcept = delete;
    AMT_IMT& operator=(const AMT_IMT& other) = delete;
    AMT_IMT& operator=(AMT_IMT&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAMT_IMTEventImtVehicleOnGuideReqID("E20701");
        setSFCAMT_IMTEventImtVehicleOnGuideReqLinkedSoundID("SND_PopUpInform2");
        setSFCAMT_IMTEventImtVehicleOnGuideReqLinkedSoundType(SFCAMT_IMTEventImtVehicleOnGuideReqLinkedSoundType::REPEAT_COUNT);
        setSFCAMT_IMTEventImtVehicleOnGuideReqLinkedSoundRepeatCount(1);

        setSFCAMT_IMTEventImtClutchOpenWarnID("E20702");
        setSFCAMT_IMTEventImtClutchOpenWarnLinkedSoundID("SND_PopUpWarn1");

        setSFCAMT_IMTEventAmtGearShiftWarnID("E20704");
        setSFCAMT_IMTEventAmtGearShiftWarnLinkedSoundID("SND_PopUpInform1");
        setSFCAMT_IMTEventAmtGearShiftWarnLinkedSoundType(SFCAMT_IMTEventAmtGearShiftWarnLinkedSoundType::REPEAT_COUNT);
        setSFCAMT_IMTEventAmtGearShiftWarnLinkedSoundRepeatCount(1);

        setSFCAMT_IMTEventEngineOverRunWarnID("E20703");
        setSFCAMT_IMTEventEngineOverRunWarnLinkedSoundID("SND_PopUpWarn2");
        setSFCAMT_IMTEventEngineOverRunWarnLinkedSoundType(SFCAMT_IMTEventEngineOverRunWarnLinkedSoundType::INFINITE);
        setSFCAMT_IMTEventEngineOverRunWarnLinkedSoundRepeatCount(0);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateImtVehicleOnGuideReqStatEvent();
        updateImtClutchOpenWarnStatEvent();
        updateAmtGearShiftWarnStatEvent();
        updateEngineOverRunWarnStatEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateImtVehicleOnGuideReqStatEvent();
        updateImtClutchOpenWarnStatEvent();
        updateAmtGearShiftWarnStatEvent();
        updateEngineOverRunWarnStatEvent();
    }

    // TODO(SHJ): Sound[T.B.D signal] 향후 적용 필요
    void onImtVehicleOnGuideReqStatChanged(const ArgumentsImtVehicleOnGuideReqStatChanged& args) {
        mImtVehicleOnGuideReqStatus = args.mInput_ImtVehicleOnGuideReqStatus;
        updateImtVehicleOnGuideReqStatEvent();
    }

    void onImtClutchOpenWarnStatChanged(const ArgumentsImtClutchOpenWarnStatChanged& args) {
        mImtClutchOpenWarnStatus = args.mInput_ImtClutchOpenWarnStatus;
        updateImtClutchOpenWarnStatEvent();
    }

    void onAmtGearShiftWarnStatChanged(const ArgumentsAmtGearShiftWarnStatChanged& args) {
        mAmtGearShiftWarnStatus = args.mInput_AmtGearShiftWarnStatus;
        updateAmtGearShiftWarnStatEvent();
    }

    void onEngineOverRunWarnStatChanged(const ArgumentsEngineOverRunWarnStatChanged& args) {
        mEngineOverRunWarnStatus = args.mInput_EngineOverRunWarnStatus;
        updateEngineOverRunWarnStatEvent();
    }

private:
    void updateImtVehicleOnGuideReqStatEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mImtVehicleOnGuideReqStatus == ImtVehicleOnGuideReqStatus::ON) {
            setSFCAMT_IMTEventImtVehicleOnGuideReqStat(SFCAMT_IMTEventImtVehicleOnGuideReqStat::ON);
        } else {
            setSFCAMT_IMTEventImtVehicleOnGuideReqStat(SFCAMT_IMTEventImtVehicleOnGuideReqStat::OFF);
        }
    }

    void updateImtClutchOpenWarnStatEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mImtClutchOpenWarnStatus == ImtClutchOpenWarnStatus::WARN1) {
                setSFCAMT_IMTEventImtClutchOpenWarnStat(SFCAMT_IMTEventImtClutchOpenWarnStat::ON);
                setSFCAMT_IMTEventImtClutchOpenWarnLinkedSoundType(SFCAMT_IMTEventImtClutchOpenWarnLinkedSoundType::REPEAT_COUNT);
                setSFCAMT_IMTEventImtClutchOpenWarnLinkedSoundRepeatCount(3);
            } else if (mImtClutchOpenWarnStatus == ImtClutchOpenWarnStatus::WARN2) {
                setSFCAMT_IMTEventImtClutchOpenWarnStat(SFCAMT_IMTEventImtClutchOpenWarnStat::ON);
                setSFCAMT_IMTEventImtClutchOpenWarnLinkedSoundType(SFCAMT_IMTEventImtClutchOpenWarnLinkedSoundType::INFINITE);
            } else {
                setSFCAMT_IMTEventImtClutchOpenWarnStat(SFCAMT_IMTEventImtClutchOpenWarnStat::OFF);
            }
        } else {
            setSFCAMT_IMTEventImtClutchOpenWarnStat(SFCAMT_IMTEventImtClutchOpenWarnStat::OFF);
        }
    }

    void updateAmtGearShiftWarnStatEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mAmtGearShiftWarnStatus == AmtGearShiftWarnStatus::WARN) {
            setSFCAMT_IMTEventAmtGearShiftWarnStat(SFCAMT_IMTEventAmtGearShiftWarnStat::ON);
        } else {
            setSFCAMT_IMTEventAmtGearShiftWarnStat(SFCAMT_IMTEventAmtGearShiftWarnStat::OFF);
        }
    }

    void updateEngineOverRunWarnStatEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mEngineOverRunWarnStatus == EngineOverRunWarnStatus::ON) {
            setSFCAMT_IMTEventEngineOverRunWarnStat(SFCAMT_IMTEventEngineOverRunWarnStat::ON);
        } else {
            setSFCAMT_IMTEventEngineOverRunWarnStat(SFCAMT_IMTEventEngineOverRunWarnStat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using ImtVehicleOnGuideReqStatus = ArgumentsImtVehicleOnGuideReqStatChanged::Input_ImtVehicleOnGuideReqStatus;
    using ImtClutchOpenWarnStatus = ArgumentsImtClutchOpenWarnStatChanged::Input_ImtClutchOpenWarnStatus;
    using AmtGearShiftWarnStatus = ArgumentsAmtGearShiftWarnStatChanged::Input_AmtGearShiftWarnStatus;
    using EngineOverRunWarnStatus = ArgumentsEngineOverRunWarnStatChanged::Input_EngineOverRunWarnStatus;

    ImtVehicleOnGuideReqStatus mImtVehicleOnGuideReqStatus = ImtVehicleOnGuideReqStatus::OFF;
    ImtClutchOpenWarnStatus mImtClutchOpenWarnStatus = ImtClutchOpenWarnStatus::OFF;
    AmtGearShiftWarnStatus mAmtGearShiftWarnStatus = AmtGearShiftWarnStatus::NO_WARN;
    EngineOverRunWarnStatus mEngineOverRunWarnStatus = EngineOverRunWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_AMT_IMT_H
