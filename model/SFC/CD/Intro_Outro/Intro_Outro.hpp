/**
 * @file Intro_Outro.hpp
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
#ifndef SFSS_Intro_Outro_H
#define SFSS_Intro_Outro_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>

#include "Intro_OutroBase.hpp"

namespace ccos {

// SFC Version : 16.1.0
// Reference : [CD070] Intro_Outro. Contains : Event, Constant, Sound. Recommend: VALUE_CHANGED
class Intro_Outro : public Intro_OutroBase {
public:
    Intro_Outro() = default;
    ~Intro_Outro() override = default;
    Intro_Outro(const Intro_Outro& other) = delete;
    Intro_Outro(Intro_Outro&& other) noexcept = delete;
    Intro_Outro& operator=(const Intro_Outro& other) = delete;
    Intro_Outro& operator=(Intro_Outro&& other) noexcept = delete;

    void onInitialize() override {
        setSFCIntro_OutroEventWelcomeID("E00001");
        setSFCIntro_OutroEventWelcomeLinkedSoundRepeatCount(1);
        setSFCIntro_OutroEventGoodbyeLinkedSoundRepeatCount(1);
        setSFCIntro_OutroSoundPowerOffID("SND_PowerOff");
        setSFCIntro_OutroSoundPowerOffType(SFCIntro_OutroSoundPowerOffType::REPEAT_COUNT);
        setSFCIntro_OutroSoundPowerOffRepeatCount(1);
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == false) {
            return;
        }
        mIgnElapsedCurrentStatus = IgnElapsed::ON_0ms;
        mGoodbyeADMOffFlushFlag = false;
        updateEventStat(IgnElapsed::ON_0ms);
        updateSoundPowerOff();
        systemCheckDecision();
        goodByeInformation();
        updateEventGoodbyeADMOFF();
    }

    void onIgnElapseOn4000msChanged(const ArgumentsIgnElapseOn4000msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn4000ms);
        if (initState == false) {
            return;
        }
        mIgnElapsedCurrentStatus = IgnElapsed::ON_4000ms;
        updateEventStat(IgnElapsed::ON_4000ms);
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == false) {
            return;
        }
        if (mIgnElapsedCurrentStatus == IgnElapsed::ON_0ms || mIgnElapsedCurrentStatus == IgnElapsed::ON_4000ms) {
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_0ms;
            updateEventStat(IgnElapsed::OFF_0ms);
            updateSoundPowerOff();
        }
    }

    void onIgnElapseOff1000msChanged(const ArgumentsIgnElapseOff1000msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff1000ms) == false) {
            return;
        }

        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_0ms) {
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_1000ms;
            updateEventGoodbyeADMOFF();
        }
    }

    void onIgnElapseOff1500msChanged(const ArgumentsIgnElapseOff1500msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff1500ms);
        if (initState == false) {
            return;
        }
        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_1000ms) {
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_1500ms;
            goodByeInformation();
        }
    }

    void onSignalTimeOut(const NodeAddress& /* nodeAddress */) override {
        // no operation
    }

    void onVehicleTypeChanged(const ArgumentsVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        reservationInformationChanged();
    }

    void onEvChargeSchedEnabledStatusChanged(const ArgumentsEvChargeSchedEnabledStatusChanged& args) {
        mEvChargeSchedEnabledStatus = args.mInput_EvChargeSchedEnabledStatus;
        reservationInformationChanged();
    }

    void onEvClimateSchedEnabledStatusChanged(const ArgumentsEvClimateSchedEnabledStatusChanged& args) {
        mEvClimateSchedEnabledStatus = args.mInput_EvClimateSchedEnabledStatus;
        reservationInformationChanged();
    }

    void onWelcomeReadyStatusChanged(const ArgumentsWelcomeReadyStatusChanged& args) {
        mHeadUnitWelcomeReadyStatus = args.mInput_HeadUnitWelcomeReadyStatus;
        if (mIgnElapsedCurrentStatus == IgnElapsed::ON_0ms || mIgnElapsedCurrentStatus == IgnElapsed::ON_4000ms) {
            mPrevHeadUnitWelcomeReadyStatus = args.mInput_HeadUnitWelcomeReadyStatus;
            return;
        }

        // TODO(LDG) - 사양서와 선언된 enum이 다름, 사양서 :on off, signal list : OFF, READY
        std::string linkedSoundID;
        SFCIntro_OutroEventWelcomeLinkedSoundType soundType(SFCIntro_OutroEventWelcomeLinkedSoundType::NONE);

        if (mPrevHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::OFF &&
            args.mInput_HeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::WELCOME_READY &&
            mPrivateWelcomeProcessStatus == PrivateWelcomeProcessStatus::WELCOME &&
            mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) {
            if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                linkedSoundID = "SND_Welcome";
                soundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
            }
            setSFCIntro_OutroEventWelcomeStat(SFCIntro_OutroEventWelcomeStat::ON);
            setSFCIntro_OutroEventWelcomeLinkedSoundID(linkedSoundID);
            setSFCIntro_OutroEventWelcomeLinkedSoundType(soundType);
            setSFCIntro_OutroEventWelcomeLinkedSoundRepeatCount(1);
        }

        updateEventGoodbyeADMOFF();
        mPrevHeadUnitWelcomeReadyStatus = args.mInput_HeadUnitWelcomeReadyStatus;
    }

    void onConfigAvnWelcomeSupportChanged(const ArgumentsConfigAvnWelcomeSupportChanged& args) {
        mConfigAvnWelcomeSupport = args.mInter_ConfigAvnWelcomeSupport;
        updateEventGoodbyeADMOFF();
    }

    void onWelcomeSoundSetStatusChanged(const ArgumentsWelcomeSoundSetStatusChanged& args) {
        mWelcomeSoundSetStatus = args.mInter_WelcomeSoundSetStatus;
        updateEventGoodbyeADMOFF();
    }

    void onPrivateWelcomeStatusChanged(const ArgumentsPrivateWelcomeStatusChanged& args) {
        updateEventWelcome(args.mInter_PrivateWelcomeProcessStatus);
        mPrivateWelcomeProcessStatus = args.mInter_PrivateWelcomeProcessStatus;
    }

    void onFcevPurAirStatusChanged(const ArgumentsFcevPurAirStatusChanged& args) {
        mFcevPurAirStatus = args.mInput_FcevPurAirStatus;
        updateConstantPurificationAir();
    }

    void onFcevCO2RdcStatusChanged(const ArgumentsFcevCO2RdcStatusChanged& args) {
        mFcevCO2RdcStatus = args.mInput_FcevCO2RdcStatus;
        updateConstantCO2Reduction();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_4000ms,
        OFF_0ms,
        OFF_1000ms,
        OFF_1500ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsVehicleTypeChanged::Inter_ConfigVehicleType;
    using EvChargeSchedEnabledStatus = ArgumentsEvChargeSchedEnabledStatusChanged::Input_EvChargeSchedEnabledStatus;
    using EvClimateSchedEnabledStatus = ArgumentsEvClimateSchedEnabledStatusChanged::Input_EvClimateSchedEnabledStatus;
    using ConfigAvnWelcomeSupport = ArgumentsConfigAvnWelcomeSupportChanged::Inter_ConfigAvnWelcomeSupport;
    using HeadUnitWelcomeReadyStatus = ArgumentsWelcomeReadyStatusChanged::Input_HeadUnitWelcomeReadyStatus;
    using WelcomeSoundSetStatus = ArgumentsWelcomeSoundSetStatusChanged::Inter_WelcomeSoundSetStatus;
    using PrivateWelcomeProcessStatus = ArgumentsPrivateWelcomeStatusChanged::Inter_PrivateWelcomeProcessStatus;

    void updateEventStat(const IgnElapsed& ignStatus) {
        if (ignStatus == IgnElapsed::ON_0ms) {
            setSFCIntro_OutroEventGoodbyeStat(SFCIntro_OutroEventGoodbyeStat::OFF);
        } else {
            setSFCIntro_OutroEventSystemCheckStat(SFCIntro_OutroEventSystemCheckStat::OFF);
        }

        if (ignStatus == IgnElapsed::ON_0ms || ignStatus == IgnElapsed::ON_4000ms) {
            setSFCIntro_OutroEventWelcomeStat(SFCIntro_OutroEventWelcomeStat::OFF);
        }
    }

    void reservationInformationChanged() {
        if ((mConfigVehicleType == ConfigVehicleType::PHEV) || (mConfigVehicleType == ConfigVehicleType::EV)) {
            if (mEvChargeSchedEnabledStatus == EvChargeSchedEnabledStatus::ON) {
                setSFCIntro_OutroConstantResvChargeStat(SFCIntro_OutroConstantResvChargeStat::ON);
            } else if (mEvChargeSchedEnabledStatus == EvChargeSchedEnabledStatus::OFF) {
                setSFCIntro_OutroConstantResvChargeStat(SFCIntro_OutroConstantResvChargeStat::OFF);
            } else {
                setSFCIntro_OutroConstantResvChargeStat(SFCIntro_OutroConstantResvChargeStat::DISPLAY_OFF);
            }
        } else {
            setSFCIntro_OutroConstantResvChargeStat(SFCIntro_OutroConstantResvChargeStat::DISPLAY_OFF);
        }
        if (mConfigVehicleType == ConfigVehicleType::EV) {
            if (mEvClimateSchedEnabledStatus == EvClimateSchedEnabledStatus::ON) {
                setSFCIntro_OutroConstantResvClimateStat(SFCIntro_OutroConstantResvClimateStat::ON);
            } else if (mEvClimateSchedEnabledStatus == EvClimateSchedEnabledStatus::OFF) {
                setSFCIntro_OutroConstantResvClimateStat(SFCIntro_OutroConstantResvClimateStat::OFF);
            } else {
                setSFCIntro_OutroConstantResvClimateStat(SFCIntro_OutroConstantResvClimateStat::DISPLAY_OFF);
            }
        } else {
            setSFCIntro_OutroConstantResvClimateStat(SFCIntro_OutroConstantResvClimateStat::DISPLAY_OFF);
        }
    }

    void updateConstantPurificationAir() {
        HDouble value = static_cast<HDouble>(mFcevPurAirStatus) * 0.1;
        setSFCIntro_OutroConstantPurificationAirValue(value);
    }

    void updateConstantCO2Reduction() {
        HDouble value = static_cast<HDouble>(mFcevCO2RdcStatus) * 0.1;
        setSFCIntro_OutroConstantCO2ReductionValue(value);
    }

    void systemCheckDecision() {
        // IGN1 [off -> on] only
        setSFCIntro_OutroEventSystemCheckID("E00002");
        setSFCIntro_OutroEventSystemCheckStat(SFCIntro_OutroEventSystemCheckStat::ON);
        flush();
    }

    void updateEventGoodbyeADMOFF() {
        std::string eventID = "E00005";
        SFCIntro_OutroEventGoodbyeStat eventStatus{SFCIntro_OutroEventGoodbyeStat::OFF};
        std::string linkedSoundID;
        SFCIntro_OutroEventGoodbyeLinkedSoundType linkedSoundType{SFCIntro_OutroEventGoodbyeLinkedSoundType::NONE};

        if ((mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE ||
             mWelcomeSoundSetStatus == WelcomeSoundSetStatus::DISABLE) &&
            (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) &&
            (mHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::GOODBYE_READY) &&
            (mIgnElapsedCurrentStatus == IgnElapsed::OFF_1000ms || mIgnElapsedCurrentStatus == IgnElapsed::OFF_1500ms)) {
            if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                linkedSoundID = "SND_Goodbye";
                linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::REPEAT_COUNT;
            }
            eventStatus = SFCIntro_OutroEventGoodbyeStat::ON;
        }

        if (eventStatus == SFCIntro_OutroEventGoodbyeStat::ON) {
            mGoodbyeADMOffFlushFlag = true;
        }

        setSFCIntro_OutroEventGoodbyeID(eventID);
        setSFCIntro_OutroEventGoodbyeStat(eventStatus);
        setSFCIntro_OutroEventGoodbyeLinkedSoundID(linkedSoundID);
        setSFCIntro_OutroEventGoodbyeLinkedSoundType(linkedSoundType);
    }

    void goodByeInformation() {
        if (mGoodbyeADMOffFlushFlag == true) {
            return;
        }

        std::string eventID = "E00004";
        SFCIntro_OutroEventGoodbyeStat eventStatus{SFCIntro_OutroEventGoodbyeStat::OFF};
        std::string soundID;
        SFCIntro_OutroEventGoodbyeLinkedSoundType linkedSoundType{SFCIntro_OutroEventGoodbyeLinkedSoundType::NONE};

        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_1500ms) {
            if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE &&
                mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF) {
                // SND_Goodbye
                eventStatus = SFCIntro_OutroEventGoodbyeStat::ON;
                soundID = "SND_Goodbye";
                linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::REPEAT_COUNT;
            } else if (((mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF) &&
                        (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::DISABLE)) ||
                       ((mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) &&
                        (mHeadUnitWelcomeReadyStatus != HeadUnitWelcomeReadyStatus::GOODBYE_READY))) {
                // SND Empty
                eventStatus = SFCIntro_OutroEventGoodbyeStat::ON;
            } else {
                // OFF
            }
        }

        setSFCIntro_OutroEventGoodbyeID(eventID);
        setSFCIntro_OutroEventGoodbyeStat(eventStatus);
        setSFCIntro_OutroEventGoodbyeLinkedSoundID(soundID);
        setSFCIntro_OutroEventGoodbyeLinkedSoundType(linkedSoundType);
    }

    void updateEventWelcome(const PrivateWelcomeProcessStatus& nextState) {
        std::string linkedSoundID;
        SFCIntro_OutroEventWelcomeLinkedSoundType soundType(SFCIntro_OutroEventWelcomeLinkedSoundType::NONE);

        if ((mPrivateWelcomeProcessStatus != PrivateWelcomeProcessStatus::WELCOME) &&
            (nextState == PrivateWelcomeProcessStatus::WELCOME)) {
            if ((mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) &&
                (mPrevHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::WELCOME_READY)) {
                if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                    linkedSoundID = "SND_Welcome";
                    soundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
                }
                setSFCIntro_OutroEventWelcomeStat(SFCIntro_OutroEventWelcomeStat::ON);
                setSFCIntro_OutroEventWelcomeLinkedSoundID(linkedSoundID);
                setSFCIntro_OutroEventWelcomeLinkedSoundType(soundType);
            } else if (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF) {
                if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                    linkedSoundID = "SND_Welcome";
                    soundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
                }
                setSFCIntro_OutroEventWelcomeStat(SFCIntro_OutroEventWelcomeStat::ON);
                setSFCIntro_OutroEventWelcomeLinkedSoundID(linkedSoundID);
                setSFCIntro_OutroEventWelcomeLinkedSoundType(soundType);
            } else {
                // nothing todo
            }
        } else if ((mPrivateWelcomeProcessStatus == PrivateWelcomeProcessStatus::WELCOME) &&
                   (nextState == PrivateWelcomeProcessStatus::WELCOME)) {
            // nothing todo
        } else {
            setSFCIntro_OutroEventWelcomeStat(SFCIntro_OutroEventWelcomeStat::OFF);
            setSFCIntro_OutroEventWelcomeLinkedSoundID(linkedSoundID);
            setSFCIntro_OutroEventWelcomeLinkedSoundType(soundType);
        }
    }

    void updateSoundPowerOff() {
        SFCIntro_OutroSoundPowerOffStat stat = SFCIntro_OutroSoundPowerOffStat::OFF;

        if (mConfigVehicleType != ConfigVehicleType::ICV && mIgnElapsedCurrentStatus == IgnElapsed::OFF_0ms) {
            stat = SFCIntro_OutroSoundPowerOffStat::ON;
        }
        setSFCIntro_OutroSoundPowerOffStat(stat);
    }

    IgnElapsed mIgnElapsedCurrentStatus = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EvChargeSchedEnabledStatus mEvChargeSchedEnabledStatus = EvChargeSchedEnabledStatus::OFF;
    EvClimateSchedEnabledStatus mEvClimateSchedEnabledStatus = EvClimateSchedEnabledStatus::OFF;
    ConfigAvnWelcomeSupport mConfigAvnWelcomeSupport = ConfigAvnWelcomeSupport::OFF;
    PrivateWelcomeProcessStatus mPrivateWelcomeProcessStatus = PrivateWelcomeProcessStatus::OFF;
    HeadUnitWelcomeReadyStatus mHeadUnitWelcomeReadyStatus = HeadUnitWelcomeReadyStatus::OFF;
    HeadUnitWelcomeReadyStatus mPrevHeadUnitWelcomeReadyStatus = HeadUnitWelcomeReadyStatus::OFF;
    WelcomeSoundSetStatus mWelcomeSoundSetStatus = WelcomeSoundSetStatus::DISABLE;
    HBool mGoodbyeADMOffFlushFlag = false;
    HUInt64 mFcevPurAirStatus = 0;
    HUInt64 mFcevCO2RdcStatus = 0;
};

}  // namespace ccos

#endif  // SFSS_Intro_Outro_H
