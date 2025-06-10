/**
 * @file EPB_Auto_Hold.hpp
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
#ifndef SFSS_EPB_Auto_Hold_H
#define SFSS_EPB_Auto_Hold_H

#define DLOG_ENABLED gEnableSFCLog

#include "EPB_Auto_HoldBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 5.0.0
// Reference : [CH090] EPB_Auto_Hold. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class EPB_Auto_Hold : public EPB_Auto_HoldBase {
public:
    EPB_Auto_Hold() = default;
    ~EPB_Auto_Hold() override = default;
    EPB_Auto_Hold(const EPB_Auto_Hold& other) = delete;
    EPB_Auto_Hold(EPB_Auto_Hold&& other) noexcept = delete;
    EPB_Auto_Hold& operator=(const EPB_Auto_Hold& other) = delete;
    EPB_Auto_Hold& operator=(EPB_Auto_Hold&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::OFF);
        setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::OFF);
        setSFCEPB_Auto_HoldSoundEPBID("SND_EPBUncomfortAlarm");
        setSFCEPB_Auto_HoldSoundEPBStat(SFCEPB_Auto_HoldSoundEPBStat::OFF);
        setSFCEPB_Auto_HoldSoundEPBType(SFCEPB_Auto_HoldSoundEPBType::INFINITE);
        setSFCEPB_Auto_HoldSoundAVHID("SND_AVHAlarm");
        setSFCEPB_Auto_HoldSoundAVHStat(SFCEPB_Auto_HoldSoundAVHStat::OFF);
        setSFCEPB_Auto_HoldSoundAVHType(SFCEPB_Auto_HoldSoundAVHType::INFINITE);
        if (mOneShotTimerNeutralPositionParkStatus.create(kTimerEventInterval700ms, this,
                                                          &EPB_Auto_Hold::onOneShotTimerNeutralPositionPark, false) == false) {
            DWarning() << "[EPB_Auto_Hold] mOneShotTimerNeutralPositionParkStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateEpbTelltale();
        updateAutoHoldTelltale();
        updateEvent();
        updateEpbSound();
        updateAvhSound();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateEpbTelltale();
        updateAutoHoldTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        mIsIgnOn = true;
        updateEpbTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateEpbTelltale();
        updateAutoHoldTelltale();
        updateEvent();
        updateEpbSound();
        updateAvhSound();
    }

    void onConfigEPBChanged(const ArgumentsConfigEPBChanged& args) {
        mConfigEPB = args.mInter_ConfigEPB;
        updateEpbTelltale();
        updateAutoHoldTelltale();
        updateEvent();
        updateEpbSound();
        updateAvhSound();
    }

    void onEpbFailWarnChanged(const ArgumentsEpbFailWarnChanged& args) {
        if (ISTIMEOUT(args.mInput_EpbFailWarnStatus) == true) {
            mEpbFailWarnStatus = EpbFailWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mEpbFailWarnStatus = args.mInput_EpbFailWarnStatus;
        }
        updateEpbTelltale();
    }

    void onAutoHoldChanged(const ArgumentsAutoHoldChanged& args) {
        mAutoHoldStatus = args.mInput_AutoHoldStatus;
        if (ISTIMEOUT(args.mInput_AutoHoldStatus) == true) {
            mAutoHoldStatus = AutoHoldStatus::UNHANDLED_TIMEOUT;
        }
        updateAutoHoldTelltale();
    }

    void onEventValueChanged(const ArgumentsEventValueChanged& args) {
        mEpbDisplayReqStatus = args.mInput_EpbDisplayReqStatus;
        mAutoHoldDisplayReqStatus = args.mInput_AutoHoldDisplayReqStatus;
        updateEvent();
        mCurrentEpbDisplayReqStatus = mEpbDisplayReqStatus;
    }

    void onEpbWarnSoundChanged(const ArgumentsEpbWarnSoundChanged& args) {
        mEpbWarnSoundStatus = args.mInput_EpbWarnSoundStatus;
        updateEpbSound();
    }

    void onAvhWarnSoundChanged(const ArgumentsAvhWarnSoundChanged& args) {
        mAvhWarnSoundStatus = args.mInput_AvhWarnSoundStatus;
        updateAvhSound();
    }

    void onSbwWarnStatusFromSCUChanged(const ArgumentsSbwWarnStatusFromSCUChanged& args) {
        mSbwWarnStatusFromSCU = args.mInput_SbwWarnStatusFromSCU;
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAfterPreWarnEpb() {
        switch (mEpbFailWarnStatus) {
            case EpbFailWarnStatus::FAILURE:
            case EpbFailWarnStatus::UNHANDLED_TIMEOUT:
                setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::ON);
                break;
            case EpbFailWarnStatus::EOL_ERROR:
                setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::BLINK2);
                setSFCEPB_Auto_HoldTelltaleEPBBlinkValueA(SFCEPB_Auto_HoldTelltaleEPBBlinkValueA::ON);
                setSFCEPB_Auto_HoldTelltaleEPBBlinkValueB(SFCEPB_Auto_HoldTelltaleEPBBlinkValueB::OFF);
                break;
            default:
                setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::OFF);
                break;
        }
    }

    void updateAfter500msOffAutoHold() {
        switch (mAutoHoldStatus) {
            case AutoHoldStatus::ACTIVE:
                setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::GREEN);
                break;
            case AutoHoldStatus::READY:
                setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::WHITE);
                break;
            case AutoHoldStatus::FAILURE:
            case AutoHoldStatus::UNHANDLED_TIMEOUT:
                setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::YELLOW);
                break;
            default:
                setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::OFF);
                break;
        }
    }

    void updateEpbTelltale() {
        if (mConfigEPB == ConfigEPB::ON && mIgnElapsed == IgnElapsed::ON_500ms) {
            setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::ON);
        } else if (mConfigEPB == ConfigEPB::ON && mIgnElapsed == IgnElapsed::ON_3500ms) {
            updateAfterPreWarnEpb();
        } else {
            setSFCEPB_Auto_HoldTelltaleEPBStat(SFCEPB_Auto_HoldTelltaleEPBStat::OFF);
        }
    }

    void updateAutoHoldTelltale() {
        if (mConfigEPB == ConfigEPB::ON && (mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms)) {
            updateAfter500msOffAutoHold();
        } else {
            setSFCEPB_Auto_HoldTelltaleAUTOHOLDStat(SFCEPB_Auto_HoldTelltaleAUTOHOLDStat::OFF);
        }
    }

    void onOneShotTimerNeutralPositionPark() {
        std::string prevEventID;
        GETCACHEDVALUE(SFC.EPB_Auto_Hold.Event.EpbAutoHold.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != "E30606") {
            setSFCEPB_Auto_HoldEventEpbAutoHoldStat(SFCEPB_Auto_HoldEventEpbAutoHoldStat::OFF);
            flushLastGroup();
        }

        setSFCEPB_Auto_HoldEventEpbAutoHoldID("E30606");
        setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundID("SND_PopUpInform2");
        setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType(SFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType::REPEAT_COUNT);
        setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundRepeatCount(1);
        setSFCEPB_Auto_HoldEventEpbAutoHoldStat(SFCEPB_Auto_HoldEventEpbAutoHoldStat::ON);
        flush();
    }

    void updateEvent() {
        std::string eventID;
        std::string soundID;

        if (mEpbDisplayReqStatus != EpbDisplayReqStatus::NEUTAL_POSION_PARK) {
            mOneShotTimerNeutralPositionParkStatus.stop();
        }

        if (mConfigEPB == ConfigEPB::ON) {
            if (mEpbDisplayReqStatus != EpbDisplayReqStatus::MAINTENANCE_MODE &&
                mEpbDisplayReqStatus != EpbDisplayReqStatus::BRAKE_TO_RELEASE_EPB &&
                mEpbDisplayReqStatus != EpbDisplayReqStatus::NEUTAL_POSION_PARK &&
                mEpbDisplayReqStatus != EpbDisplayReqStatus::CHANGE_GEAR_TO_RELEASE_EPB &&
                mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::AVH_AUTO_CANCELED && mIsIgnOn == true) {
                eventID = "E30601";
            } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::MAINTENANCE_MODE && mIsIgnOn == true) {
                eventID = "E30602";
                soundID = "SND_PopUpInform2";
            } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::BRAKE_TO_RELEASE_EPB && mIsIgnOn == true) {
                eventID = "E30605";
                soundID = "SND_PopUpInform2";
            } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::NEUTAL_POSION_PARK) {  // Should work when B+ on
                if (mCurrentEpbDisplayReqStatus != mEpbDisplayReqStatus) {
                    if (mOneShotTimerNeutralPositionParkStatus.start() == false) {
                        DWarning() << "[EPB_Auto_Hold] mOneShotTimerNeutralPositionParkStatus Start Failed";
                    }
                }
                return;
            } else if (mSbwWarnStatusFromSCU == SbwWarnStatusFromSCU::AUTOHOLD_EPB_RELEASE_N) {
                eventID = "E30606";
                soundID = "SND_PopUpInform2";
            } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::CHANGE_GEAR_TO_RELEASE_EPB && mIsIgnOn == true) {
                eventID = "E30614";
                soundID = "SND_PopUpInform1";
            } else if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::AVH_OFF && mIsIgnOn == true) {
                if (mEpbDisplayReqStatus == EpbDisplayReqStatus::AUTO_CANCEL_IMPOSSIBLE) {
                    eventID = "E30603";
                    soundID = "SND_PopUpWarn1";
                } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::AUTO_CANCEL_IMPOSSIBLE_IN_CHINA) {
                    eventID = "E30604";
                    soundID = "SND_PopUpWarn1";
                } else if (mEpbDisplayReqStatus == EpbDisplayReqStatus::EPB_APPLIED) {
                    eventID = "E30607";
                    soundID = "SND_PopUpInform2";
                } else {
                    // no operation
                }
            } else {
                if (mEpbDisplayReqStatus == EpbDisplayReqStatus::OFF && mIsIgnOn == true) {
                    if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::BRAKE_TO_DEACTIVATE_AVH) {
                        eventID = "E30608";
                        soundID = "SND_PopUpInform2";
                    } else if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::AVH_NO_ACT_DUE_TO_DOOR_HOOD_TRUNK) {
                        eventID = "E30609";
                        soundID = "SND_PopUpWarn1";
                    } else if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::AVH_NO_ACT_DUE_TO_DOOR_HOOD) {
                        eventID = "E30610";
                        soundID = "SND_PopUpWarn1";
                    } else if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::BRAKE_CLEANING_ON) {
                        eventID = "E30612";
                        soundID = "SND_PopUpInform1";
                    } else if (mAutoHoldDisplayReqStatus == AutoHoldDisplayReqStatus::BRAKE_CLEANING_OFF) {
                        eventID = "E30613";
                        soundID = "SND_PopUpInform1";
                    } else {
                        // no operation
                    }
                }
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.EPB_Auto_Hold.Event.EpbAutoHold.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEPB_Auto_HoldEventEpbAutoHoldStat(SFCEPB_Auto_HoldEventEpbAutoHoldStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            if (soundID.empty() == true) {
                setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType(SFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType::NONE);
            } else {
                setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType(SFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundType::REPEAT_COUNT);
                setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundRepeatCount(1);
            }
            setSFCEPB_Auto_HoldEventEpbAutoHoldID(eventID);
            setSFCEPB_Auto_HoldEventEpbAutoHoldLinkedSoundID(soundID);
            setSFCEPB_Auto_HoldEventEpbAutoHoldStat(SFCEPB_Auto_HoldEventEpbAutoHoldStat::ON);
        }
    }

    void updateEpbSound() {
        if (mConfigEPB == ConfigEPB::ON && mEpbWarnSoundStatus == EpbWarnSoundStatus::WARN && mIsIgnOn == true) {
            setSFCEPB_Auto_HoldSoundEPBStat(SFCEPB_Auto_HoldSoundEPBStat::ON);
        } else {
            setSFCEPB_Auto_HoldSoundEPBStat(SFCEPB_Auto_HoldSoundEPBStat::OFF);
        }
    }

    void updateAvhSound() {
        if (mConfigEPB == ConfigEPB::ON && mAvhWarnSoundStatus == AvhWarnSoundStatus::WARN && mIsIgnOn == true) {
            setSFCEPB_Auto_HoldSoundAVHStat(SFCEPB_Auto_HoldSoundAVHStat::ON);
        } else {
            setSFCEPB_Auto_HoldSoundAVHStat(SFCEPB_Auto_HoldSoundAVHStat::OFF);
        }
    }

    static constexpr uint32_t kTimerEventInterval700ms = 700u;

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using ConfigEPB = ArgumentsConfigEPBChanged::Inter_ConfigEPB;
    using EpbFailWarnStatus = ArgumentsEpbFailWarnChanged::Input_EpbFailWarnStatus;
    using AutoHoldStatus = ArgumentsAutoHoldChanged::Input_AutoHoldStatus;
    using EpbDisplayReqStatus = ArgumentsEventValueChanged::Input_EpbDisplayReqStatus;
    using AutoHoldDisplayReqStatus = ArgumentsEventValueChanged::Input_AutoHoldDisplayReqStatus;
    using EpbWarnSoundStatus = ArgumentsEpbWarnSoundChanged::Input_EpbWarnSoundStatus;
    using AvhWarnSoundStatus = ArgumentsAvhWarnSoundChanged::Input_AvhWarnSoundStatus;
    using SbwWarnStatusFromSCU = ArgumentsSbwWarnStatusFromSCUChanged::Input_SbwWarnStatusFromSCU;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    ConfigEPB mConfigEPB = ConfigEPB::OFF;
    EpbFailWarnStatus mEpbFailWarnStatus = EpbFailWarnStatus::NORMAL;
    AutoHoldStatus mAutoHoldStatus = AutoHoldStatus::OFF;
    EpbDisplayReqStatus mEpbDisplayReqStatus = EpbDisplayReqStatus::OFF;
    EpbDisplayReqStatus mCurrentEpbDisplayReqStatus = EpbDisplayReqStatus::OFF;
    AutoHoldDisplayReqStatus mAutoHoldDisplayReqStatus = AutoHoldDisplayReqStatus::AVH_OFF;
    EpbWarnSoundStatus mEpbWarnSoundStatus = EpbWarnSoundStatus::OFF;
    AvhWarnSoundStatus mAvhWarnSoundStatus = AvhWarnSoundStatus::OFF;
    ssfs::SFCTimer<EPB_Auto_Hold> mOneShotTimerNeutralPositionParkStatus;
    SbwWarnStatusFromSCU mSbwWarnStatusFromSCU = SbwWarnStatusFromSCU::OFF;
};

}  // namespace ccos

#endif  // SFSS_EPB_Auto_Hold_H
