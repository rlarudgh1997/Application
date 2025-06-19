/**
 * @file Parking_Brake_CV.hpp
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
#ifndef SFSS_Parking_Brake_CV_H
#define SFSS_Parking_Brake_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Parking_Brake_CVBase.hpp"

namespace ccos {

// SFC Version : 14.0.0
// Spec Version : v0.28
// Reference : [CV360] Parking_Brake_CV. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Parking_Brake_CV : public Parking_Brake_CVBase {
public:
    Parking_Brake_CV() = default;
    ~Parking_Brake_CV() override = default;
    Parking_Brake_CV(const Parking_Brake_CV& other) = delete;
    Parking_Brake_CV(Parking_Brake_CV&& other) noexcept = delete;
    Parking_Brake_CV& operator=(const Parking_Brake_CV& other) = delete;
    Parking_Brake_CV& operator=(Parking_Brake_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCParking_Brake_CVTelltaleParking_Brake_CVBlinkValueA(SFCParking_Brake_CVTelltaleParking_Brake_CVBlinkValueA::ON);
        setSFCParking_Brake_CVTelltaleParking_Brake_CVBlinkValueB(SFCParking_Brake_CVTelltaleParking_Brake_CVBlinkValueB::OFF);
        setSFCParking_Brake_CVEventEPBReleaseInhibitInfoLinkedSoundType(
            SFCParking_Brake_CVEventEPBReleaseInhibitInfoLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBReleaseInhibitInfoLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCParking_Brake_CVEventEPBSystemCheckWarningID("E73604");
        setSFCParking_Brake_CVEventEPBSystemCheckWarningLinkedSoundID("SND_PopUpWarn2");
        setSFCParking_Brake_CVEventEPBSystemCheckWarningLinkedSoundType(
            SFCParking_Brake_CVEventEPBSystemCheckWarningLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBSystemCheckWarningLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCParking_Brake_CVEventEPBAutoLockInfoID("E73605");
        setSFCParking_Brake_CVEventEPBAutoLockInfoLinkedSoundID("SND_PopUpInform2");
        setSFCParking_Brake_CVEventEPBAutoLockInfoLinkedSoundType(
            SFCParking_Brake_CVEventEPBAutoLockInfoLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBAutoLockInfoLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCParking_Brake_CVEventEPBNotReleasedID("E73606");
        setSFCParking_Brake_CVEventEPBNotReleasedLinkedSoundID("SND_PopUpInform2");
        setSFCParking_Brake_CVEventEPBNotReleasedLinkedSoundType(
            SFCParking_Brake_CVEventEPBNotReleasedLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBNotReleasedLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCParking_Brake_CVEventEPBWorkshopModeID("E73607");
        setSFCParking_Brake_CVEventEPBWorkshopModeLinkedSoundID("SND_PopUpInform2");
        setSFCParking_Brake_CVEventEPBWorkshopModeLinkedSoundType(
            SFCParking_Brake_CVEventEPBWorkshopModeLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBWorkshopModeLinkedSoundRepeatCount(kEventRepeatCount);
        setSFCParking_Brake_CVEventEPBTrailerWorkshopModeID("E73608");
        setSFCParking_Brake_CVEventEPBTrailerWorkshopModeLinkedSoundID("SND_PopUpInform2");
        setSFCParking_Brake_CVEventEPBTrailerWorkshopModeLinkedSoundType(
            SFCParking_Brake_CVEventEPBTrailerWorkshopModeLinkedSoundType::REPEAT_COUNT);
        setSFCParking_Brake_CVEventEPBTrailerWorkshopModeLinkedSoundRepeatCount(kEventRepeatCount);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleParkingLamp();
        updateEventEPBReleaseInhibitInfo();
        updateEventEPBSystemCheckWarning();
        updateEventEPBAutoLockInfo();
        updateEventEPBNotReleased();
        updateEventEPBWorkShopMode();
        updateEventEPBTrailerWorkshopMode();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleParkingLamp();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltaleParkingLamp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleParkingLamp();
        updateEventEPBReleaseInhibitInfo();
        updateEventEPBSystemCheckWarning();
        updateEventEPBAutoLockInfo();
        updateEventEPBNotReleased();
        updateEventEPBWorkShopMode();
        updateEventEPBTrailerWorkshopMode();
    }

    void onConfigEPBChanged(const ArgumentsConfigEPBChanged& args) {
        mConfigEPB = args.mInter_ConfigEPB;
        updateTelltaleParkingLamp();
    }

    void onParkingSwitchStatusChanged(const ArgumentsParkingSwitchStatusChanged& args) {
        mParkingSwitchStatus = args.mInput_ParkingSwitchStatus;
        updateTelltaleParkingLamp();
    }

    void onEpbWarningStatusChanged(const ArgumentsEpbWarningStatusChanged& args) {
        mEPBWarningStatus = args.mInput_EPBWarningStatus;
        updateTelltaleParkingLamp();
    }

    void onEpbReleaseInhibitReasonStatusChanged(const ArgumentsEpbReleaseInhibitReasonStatusChanged& args) {
        mEPBReleaseInhibitReasonStatus = args.mInput_EPBReleaseInhibitReasonStatus;
        updateEventEPBReleaseInhibitInfo();
    }

    void onEpbCheckStatusChanged(const ArgumentsEpbCheckStatusChanged& args) {
        mEPBCheckStatus = args.mInput_EPBCheckStatus;
        updateEventEPBSystemCheckWarning();
    }

    void onEpbAutoLockInfoChanged(const ArgumentsEpbAutoLockInfoChanged& args) {
        mStateOfEPBStatus = args.mInput_StateOfEPBStatus;
        mEPBExtnAppRqStatus = args.mInput_EPBExtnAppRqStatus;
        mEPBExtnAppRqStatusFromVCU = args.mInput_EPBExtnAppRqStatusFromVCU;
        updateEventEPBAutoLockInfo();
    }

    void onEpbNotReleasedStatusChanged(const ArgumentsEpbNotReleasedStatusChanged& args) {
        mEPBNotReleasedStatus = args.mInput_EPBNotReleasedStatus;
        updateEventEPBNotReleased();
    }

    void onEpbRedStopStatusChanged(const ArgumentsEpbRedStopStatusChanged& args) {
        mEPBRedStopStatus = args.mInput_EPBRedStopStatus;
        updateTelltaleParkingLamp();
    }

    void onEpbWorkingModeStatusChanged(const ArgumentsEpbWorkingModeStatusChanged& args) {
        mEPBWorkingModeStatus = args.mInput_EPBWorkingModeStatus;
        updateEventEPBWorkShopMode();
    }

    void onEpbTrailerTestStatusChanged(const ArgumentsEpbTrailerTestStatusChanged& args) {
        mEPBTrailerTestStatus = args.mInput_EPBTrailerTestStatus;
        updateEventEPBTrailerWorkshopMode();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.1.1 파킹 브레이크 작동등
    void updateTelltaleParkingLamp() {
        SFCParking_Brake_CVTelltaleParking_Brake_CVStat stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::OFF;
        if ((mIgnElapsed == IgnElapsed::OFF_0ms || mIgnElapsed == IgnElapsed::ON_0ms) &&
            mParkingSwitchStatus == ParkingSwitchStatus::ON) {
            stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_0ms && mParkingSwitchStatus == ParkingSwitchStatus::OFF) {
            stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::OFF;
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mParkingSwitchStatus == ParkingSwitchStatus::ON) {
                stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::ON;
            } else if (mConfigEPB == ConfigEPB::ON && mParkingSwitchStatus == ParkingSwitchStatus::OFF &&
                       ((mEPBWarningStatus == EPBWarningStatus::WARNING ||
                         mEPBWarningStatus == EPBWarningStatus::MESSAGE_TIMEOUT) &&
                        (mEPBRedStopStatus == EPBRedStopStatus::ON || mEPBRedStopStatus == EPBRedStopStatus::MESSAGE_TIMEOUT))) {
                stat = SFCParking_Brake_CVTelltaleParking_Brake_CVStat::BLINK1;
            }
        } else {
            // no operation
        }
        setSFCParking_Brake_CVTelltaleParking_Brake_CVStat(stat);
    }

    // 5.3.1 EPB Release Inhibit Info
    void updateEventEPBReleaseInhibitInfo() {
        std::string eventID, soundID;

        if (mIsIgnOn == true) {
            if (mEPBReleaseInhibitReasonStatus == EPBReleaseInhibitReasonStatus::INHIBIT1) {
                eventID = "E73603";
                soundID = "SND_PopUpInform1";
            } else if (mEPBReleaseInhibitReasonStatus == EPBReleaseInhibitReasonStatus::INHIBIT2) {
                eventID = "E73609";
                soundID = "SND_PopUpWarn1";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Parking_Brake_CV.Event.EPBReleaseInhibitInfo.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCParking_Brake_CVEventEPBReleaseInhibitInfoStat(SFCParking_Brake_CVEventEPBReleaseInhibitInfoStat::OFF);
            flushLastGroup();
        }

        if (eventID.empty() == false) {
            setSFCParking_Brake_CVEventEPBReleaseInhibitInfoLinkedSoundID(soundID);
            setSFCParking_Brake_CVEventEPBReleaseInhibitInfoID(eventID);
            setSFCParking_Brake_CVEventEPBReleaseInhibitInfoStat(SFCParking_Brake_CVEventEPBReleaseInhibitInfoStat::ON);
        }
    }

    // 5.3.2 EPB System Check Warning
    void updateEventEPBSystemCheckWarning() {
        SFCParking_Brake_CVEventEPBSystemCheckWarningStat stat = SFCParking_Brake_CVEventEPBSystemCheckWarningStat::OFF;

        if (mIsIgnOn == true && mEPBCheckStatus == EPBCheckStatus::CHECK) {
            stat = SFCParking_Brake_CVEventEPBSystemCheckWarningStat::ON;
        }
        setSFCParking_Brake_CVEventEPBSystemCheckWarningStat(stat);
    }

    // 5.3.3 EPB Auto Lock Info
    void updateEventEPBAutoLockInfo() {
        SFCParking_Brake_CVEventEPBAutoLockInfoStat stat = SFCParking_Brake_CVEventEPBAutoLockInfoStat::OFF;

        if (mIsIgnOn == true && mStateOfEPBStatus == StateOfEPBStatus::ON &&
            (mEPBExtnAppRqStatus == EPBExtnAppRqStatus::ON || mEPBExtnAppRqStatusFromVCU == EPBExtnAppRqStatusFromVCU::ON)) {
            stat = SFCParking_Brake_CVEventEPBAutoLockInfoStat::ON;
        }
        setSFCParking_Brake_CVEventEPBAutoLockInfoStat(stat);
    }

    // 5.3.4 EPB Not Released
    void updateEventEPBNotReleased() {
        SFCParking_Brake_CVEventEPBNotReleasedStat stat = SFCParking_Brake_CVEventEPBNotReleasedStat::OFF;

        if (mIsIgnOn == true && mEPBNotReleasedStatus == EPBNotReleasedStatus::ON) {
            stat = SFCParking_Brake_CVEventEPBNotReleasedStat::ON;
        }
        setSFCParking_Brake_CVEventEPBNotReleasedStat(stat);
    }

    // 5.3.5 EPB Workshop Mode
    void updateEventEPBWorkShopMode() {
        SFCParking_Brake_CVEventEPBWorkshopModeStat stat = SFCParking_Brake_CVEventEPBWorkshopModeStat::OFF;

        if (mIsIgnOn == true && mEPBWorkingModeStatus == EPBWorkingModeStatus::ON) {
            stat = SFCParking_Brake_CVEventEPBWorkshopModeStat::ON;
        }
        setSFCParking_Brake_CVEventEPBWorkshopModeStat(stat);
    }

    // 5.3.6 EPB TrailerWorkshop Mode
    void updateEventEPBTrailerWorkshopMode() {
        SFCParking_Brake_CVEventEPBTrailerWorkshopModeStat stat = SFCParking_Brake_CVEventEPBTrailerWorkshopModeStat::OFF;

        if (mIsIgnOn == true && mEPBTrailerTestStatus == EPBTrailerTestStatus::ON) {
            stat = SFCParking_Brake_CVEventEPBTrailerWorkshopModeStat::ON;
        }
        setSFCParking_Brake_CVEventEPBTrailerWorkshopModeStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    HBool mIsIgnOn = false;
    using ConfigEPB = ArgumentsConfigEPBChanged::Inter_ConfigEPB;
    using ParkingSwitchStatus = ArgumentsParkingSwitchStatusChanged::Input_ParkingSwitchStatus;
    using EPBCheckStatus = ArgumentsEpbCheckStatusChanged::Input_EPBCheckStatus;
    using EPBWarningStatus = ArgumentsEpbWarningStatusChanged::Input_EPBWarningStatus;
    using EPBReleaseInhibitReasonStatus = ArgumentsEpbReleaseInhibitReasonStatusChanged::Input_EPBReleaseInhibitReasonStatus;
    using StateOfEPBStatus = ArgumentsEpbAutoLockInfoChanged::Input_StateOfEPBStatus;
    using EPBExtnAppRqStatus = ArgumentsEpbAutoLockInfoChanged::Input_EPBExtnAppRqStatus;
    using EPBExtnAppRqStatusFromVCU = ArgumentsEpbAutoLockInfoChanged::Input_EPBExtnAppRqStatusFromVCU;
    using EPBNotReleasedStatus = ArgumentsEpbNotReleasedStatusChanged::Input_EPBNotReleasedStatus;
    using EPBRedStopStatus = ArgumentsEpbRedStopStatusChanged::Input_EPBRedStopStatus;
    using EPBWorkingModeStatus = ArgumentsEpbWorkingModeStatusChanged::Input_EPBWorkingModeStatus;
    using EPBTrailerTestStatus = ArgumentsEpbTrailerTestStatusChanged::Input_EPBTrailerTestStatus;

    ConfigEPB mConfigEPB = ConfigEPB::OFF;
    ParkingSwitchStatus mParkingSwitchStatus = ParkingSwitchStatus::OFF;
    EPBCheckStatus mEPBCheckStatus = EPBCheckStatus::NORMAL;
    EPBWarningStatus mEPBWarningStatus = EPBWarningStatus::NORMAL;
    EPBReleaseInhibitReasonStatus mEPBReleaseInhibitReasonStatus = EPBReleaseInhibitReasonStatus::OFF;
    StateOfEPBStatus mStateOfEPBStatus = StateOfEPBStatus::OFF;
    EPBExtnAppRqStatus mEPBExtnAppRqStatus = EPBExtnAppRqStatus::OFF;
    EPBExtnAppRqStatusFromVCU mEPBExtnAppRqStatusFromVCU = EPBExtnAppRqStatusFromVCU::OFF;
    EPBNotReleasedStatus mEPBNotReleasedStatus = EPBNotReleasedStatus::OFF;
    EPBRedStopStatus mEPBRedStopStatus = EPBRedStopStatus::OFF;
    EPBWorkingModeStatus mEPBWorkingModeStatus = EPBWorkingModeStatus::OFF;
    EPBTrailerTestStatus mEPBTrailerTestStatus = EPBTrailerTestStatus::OFF;

    static constexpr HUInt64 kEventRepeatCount = 1u;
};

}  // namespace ccos

#endif  // SFSS_Parking_Brake_CV_H
