/**
 * @file Intro_Outro_CV.hpp
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
#ifndef SFSS_Intro_Outro_CV_H
#define SFSS_Intro_Outro_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Intro_Outro_CVBase.hpp"

namespace ccos {

// SFC Version : 3.0.2
// Spec Version : v0.28
// Reference : [CV210] Intro_Outro_CV. Contains : Event, Sound. Recommend: VALUE_CHANGED
class Intro_Outro_CV : public Intro_Outro_CVBase {
public:
    Intro_Outro_CV() = default;
    ~Intro_Outro_CV() override = default;
    Intro_Outro_CV(const Intro_Outro_CV& other) = delete;
    Intro_Outro_CV(Intro_Outro_CV&& other) noexcept = delete;
    Intro_Outro_CV& operator=(const Intro_Outro_CV& other) = delete;
    Intro_Outro_CV& operator=(Intro_Outro_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCIntro_OutroEventWelcomeID("E00001");
        setSFCIntro_OutroEventSystemCheckID("E00002");
        setSFCIntro_OutroEventWelcomeLinkedSoundRepeatCount(1);
        setSFCIntro_OutroEventGoodbyeLinkedSoundRepeatCount(1);
        setSFCIntro_OutroSoundPowerOffID("SND_PowerOff");
        setSFCIntro_OutroSoundPowerOffType(SFCIntro_OutroSoundPowerOffType::REPEAT_COUNT);
        setSFCIntro_OutroSoundPowerOffRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == false) {
            return;
        }
        mIgnElapsedCurrentStatus = IgnElapsed::ON_0ms;
        updateEventWelcome();
        updateEventSystemCheck();
        updateEventGoodbye();
        updateEventGoodbyeADMOFF();
        updateSound();
    }

    void onIgnElapseOn4000msChanged(const ArgumentsIgnElapseOn4000msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn4000ms);
        if (initState == false) {
            return;
        }
        mIgnElapsedCurrentStatus = IgnElapsed::ON_4000ms;
        updateEventWelcome();
        updateEventSystemCheck();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == false) {
            return;
        }
        if (mIgnElapsedCurrentStatus == IgnElapsed::ON_0ms || mIgnElapsedCurrentStatus == IgnElapsed::ON_4000ms) {
            // NOTE: ign on -> off시에만 ign1 off 0ms 설정됨.
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_0ms;
            updateEventSystemCheck();
            updateSound();
        }
    }

    void onIgnElapseOff1000msChanged(const ArgumentsIgnElapseOff1000msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff1000ms);
        if (initState == false) {
            return;
        }
        // IGN off에서 B+ reset시 바로 goodbye가 표출되어 B+ reset시 IGN on -> IGN off 0ms -> IGN off 1000ms로 순차적으로
        // 인가되었을 때만 goodbye표시 (EXNCP-33878)
        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_0ms) {
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_1000ms;
            updateEventSystemCheck();
            updateEventGoodbyeADMOFF();
        }
    }

    void onIgnElapseOff1500msChanged(const ArgumentsIgnElapseOff1500msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff1500ms);
        if (initState == false) {
            return;
        }
        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_0ms || mIgnElapsedCurrentStatus == IgnElapsed::OFF_1000ms) {
            mIgnElapsedCurrentStatus = IgnElapsed::OFF_1500ms;
            updateEventGoodbye();
        }
    }

    void onVehicleTypeChanged(const ArgumentsVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateSound();
    }

    void onWelcomeReadyStatusChanged(const ArgumentsWelcomeReadyStatusChanged& args) {
        mHeadUnitWelcomeReadyStatus = args.mInput_HeadUnitWelcomeReadyStatus;
        updateEventWelcome();
        updateEventGoodbye();
        updateEventGoodbyeADMOFF();
        mPrevHeadUnitWelcomeReadyStatus = mHeadUnitWelcomeReadyStatus;
    }

    void onConfigAvnWelcomeSupportChanged(const ArgumentsConfigAvnWelcomeSupportChanged& args) {
        mConfigAvnWelcomeSupport = args.mInter_ConfigAvnWelcomeSupport;
        updateEventWelcome();
        updateEventGoodbye();
        updateEventGoodbyeADMOFF();
    }

    void onWelcomeSoundSetStatusChanged(const ArgumentsWelcomeSoundSetStatusChanged& args) {
        mWelcomeSoundSetStatus = args.mInter_WelcomeSoundSetStatus;
        updateEventWelcome();
        updateEventGoodbye();
        updateEventGoodbyeADMOFF();
    }

    void onPrivateWelcomeStatusChanged(const ArgumentsPrivateWelcomeStatusChanged& args) {
        mPrivateWelcomeProcessStatus = args.mInter_PrivateWelcomeProcessStatus;
        updateInterAvnWelcomeSoundUsmSetStatus();
        updateEventWelcome();
        mPrevPrivateWelcomeProcessStatus = mPrivateWelcomeProcessStatus;
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.3 AVN 웰컴 사운드 USM 설정 판단
    void updateInterAvnWelcomeSoundUsmSetStatus() {
        if (mInterAvnWelcomeSoundUsmSetStatus == SFCIntro_OutroInter_AvnWelcomeSoundUsmSetStatus::OFF &&
            mPrevPrivateWelcomeProcessStatus != PrivateWelcomeProcessStatus::WELCOME &&
            mPrivateWelcomeProcessStatus == PrivateWelcomeProcessStatus::WELCOME) {
            mInterAvnWelcomeSoundUsmSetStatus = SFCIntro_OutroInter_AvnWelcomeSoundUsmSetStatus::ON;
            setSFCIntro_OutroInter_AvnWelcomeSoundUsmSetStatus(mInterAvnWelcomeSoundUsmSetStatus);
        }
    }

    // 5.3.1 Welcome
    void updateEventWelcome() {
        SFCIntro_OutroEventWelcomeStat stat = SFCIntro_OutroEventWelcomeStat::OFF;
        std::string linkedSoundID;
        SFCIntro_OutroEventWelcomeLinkedSoundType linkedSoundType = SFCIntro_OutroEventWelcomeLinkedSoundType::NONE;

        // NOTE: ign off && ign off 1000ms && ign none
        // NOTE: ign on -> off 되었을 때 ign off 상태가 되므로 ap 부팅 시 ign none인경우에도 로직 수행 필요
        // NOTE: ign none은 ap 부팅(B+ Reset 또는 Sleep/Wakeup)을 의미
        if (mIgnElapsedCurrentStatus != IgnElapsed::ON_0ms || mIgnElapsedCurrentStatus != IgnElapsed::ON_4000ms) {
            if ((mPrevPrivateWelcomeProcessStatus != PrivateWelcomeProcessStatus::WELCOME) &&
                (mPrivateWelcomeProcessStatus == PrivateWelcomeProcessStatus::WELCOME)) {
                if ((mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) &&
                    (mHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::WELCOME_READY)) {
                    if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                        linkedSoundID = "SND_Welcome";
                        linkedSoundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
                    }
                    stat = SFCIntro_OutroEventWelcomeStat::ON;
                } else if (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF) {
                    if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                        linkedSoundID = "SND_Welcome";
                        linkedSoundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
                    }
                    stat = SFCIntro_OutroEventWelcomeStat::ON;
                } else {
                    // no operation
                }
            } else if (mPrevHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::OFF &&
                       mHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::WELCOME_READY &&
                       mPrivateWelcomeProcessStatus == PrivateWelcomeProcessStatus::WELCOME &&
                       mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) {
                if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                    linkedSoundID = "SND_Welcome";
                    linkedSoundType = SFCIntro_OutroEventWelcomeLinkedSoundType::REPEAT_COUNT;
                }
                stat = SFCIntro_OutroEventWelcomeStat::ON;
            } else {
                // no operation
            }
        }

        setSFCIntro_OutroEventWelcomeStat(stat);
        setSFCIntro_OutroEventWelcomeLinkedSoundID(linkedSoundID);
        setSFCIntro_OutroEventWelcomeLinkedSoundType(linkedSoundType);
    }

    // 5.3.2 System Check
    // IGN1 [off -> on] only
    void updateEventSystemCheck() {
        SFCIntro_OutroEventSystemCheckStat stat = SFCIntro_OutroEventSystemCheckStat::OFF;

        if (mIgnElapsedCurrentStatus == IgnElapsed::ON_0ms) {
            stat = SFCIntro_OutroEventSystemCheckStat::ON;
        }
        setSFCIntro_OutroEventSystemCheckStat(stat);
    }

    // 5.3.3 Goodbye - Driving Information
    void updateEventGoodbye() {
        std::string eventID = "E00004";
        SFCIntro_OutroEventGoodbyeStat stat = SFCIntro_OutroEventGoodbyeStat::OFF;
        std::string linkedSoundID;
        SFCIntro_OutroEventGoodbyeLinkedSoundType linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::NONE;

        if (mIgnElapsedCurrentStatus == IgnElapsed::OFF_1500ms) {
            if (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF &&
                mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE &&
                mHeadUnitWelcomeReadyStatus != HeadUnitWelcomeReadyStatus::GOODBYE_READY) {
                stat = SFCIntro_OutroEventGoodbyeStat::ON;
                linkedSoundID = "SND_Goodbye";
                linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::REPEAT_COUNT;
            } else if ((mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::OFF &&
                        mWelcomeSoundSetStatus == WelcomeSoundSetStatus::DISABLE) ||
                       (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON &&
                        mHeadUnitWelcomeReadyStatus != HeadUnitWelcomeReadyStatus::GOODBYE_READY)) {
                stat = SFCIntro_OutroEventGoodbyeStat::ON;
            } else {
                // OFF
            }
        }

        setSFCIntro_OutroEventGoodbyeID(eventID);
        setSFCIntro_OutroEventGoodbyeStat(stat);
        setSFCIntro_OutroEventGoodbyeLinkedSoundID(linkedSoundID);
        setSFCIntro_OutroEventGoodbyeLinkedSoundType(linkedSoundType);
    }

    // 5.3.4 Goodbye - ADM OFF
    void updateEventGoodbyeADMOFF() {
        std::string eventID = "E00005";
        SFCIntro_OutroEventGoodbyeStat stat = SFCIntro_OutroEventGoodbyeStat::OFF;
        std::string linkedSoundID;
        SFCIntro_OutroEventGoodbyeLinkedSoundType linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::NONE;

        if ((mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE ||
             mWelcomeSoundSetStatus == WelcomeSoundSetStatus::DISABLE) &&
            (mConfigAvnWelcomeSupport == ConfigAvnWelcomeSupport::ON) &&
            (mHeadUnitWelcomeReadyStatus == HeadUnitWelcomeReadyStatus::GOODBYE_READY) &&
            (mIgnElapsedCurrentStatus == IgnElapsed::OFF_1000ms)) {
            if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::ENABLE) {
                linkedSoundID = "SND_Goodbye";
                linkedSoundType = SFCIntro_OutroEventGoodbyeLinkedSoundType::REPEAT_COUNT;
            }
            stat = SFCIntro_OutroEventGoodbyeStat::ON;
        }
        setSFCIntro_OutroEventGoodbyeID(eventID);
        setSFCIntro_OutroEventGoodbyeStat(stat);
        setSFCIntro_OutroEventGoodbyeLinkedSoundID(linkedSoundID);
        setSFCIntro_OutroEventGoodbyeLinkedSoundType(linkedSoundType);
    }

    // 5.4 사운드 로직
    void updateSound() {
        SFCIntro_OutroSoundPowerOffStat stat = SFCIntro_OutroSoundPowerOffStat::OFF;

        if (mWelcomeSoundSetStatus == WelcomeSoundSetStatus::DISABLE && mIgnElapsedCurrentStatus == IgnElapsed::OFF_0ms) {
            stat = SFCIntro_OutroSoundPowerOffStat::ON;
        }
        setSFCIntro_OutroSoundPowerOffStat(stat);
    }

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
    using ConfigAvnWelcomeSupport = ArgumentsConfigAvnWelcomeSupportChanged::Inter_ConfigAvnWelcomeSupport;
    using HeadUnitWelcomeReadyStatus = ArgumentsWelcomeReadyStatusChanged::Input_HeadUnitWelcomeReadyStatus;
    using WelcomeSoundSetStatus = ArgumentsWelcomeSoundSetStatusChanged::Inter_WelcomeSoundSetStatus;
    using PrivateWelcomeProcessStatus = ArgumentsPrivateWelcomeStatusChanged::Inter_PrivateWelcomeProcessStatus;

    IgnElapsed mIgnElapsedCurrentStatus = IgnElapsed::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigAvnWelcomeSupport mConfigAvnWelcomeSupport = ConfigAvnWelcomeSupport::OFF;
    PrivateWelcomeProcessStatus mPrivateWelcomeProcessStatus = PrivateWelcomeProcessStatus::OFF;
    PrivateWelcomeProcessStatus mPrevPrivateWelcomeProcessStatus = PrivateWelcomeProcessStatus::OFF;
    HeadUnitWelcomeReadyStatus mHeadUnitWelcomeReadyStatus = HeadUnitWelcomeReadyStatus::OFF;
    HeadUnitWelcomeReadyStatus mPrevHeadUnitWelcomeReadyStatus = HeadUnitWelcomeReadyStatus::OFF;
    WelcomeSoundSetStatus mWelcomeSoundSetStatus = WelcomeSoundSetStatus::DISABLE;
    SFCIntro_OutroInter_AvnWelcomeSoundUsmSetStatus mInterAvnWelcomeSoundUsmSetStatus =
        SFCIntro_OutroInter_AvnWelcomeSoundUsmSetStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Intro_Outro_CV_H
