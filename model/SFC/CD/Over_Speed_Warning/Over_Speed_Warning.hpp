/**
 * @file Over_Speed_Warning.hpp
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
#ifndef SFSS_Over_Speed_Warning_H
#define SFSS_Over_Speed_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Over_Speed_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 4.0.0
// Reference : [CD110] Over Speed Warning. Contains : Telltale, Event, Sound. Recommend: VALUE_CHANGED
class Over_Speed_Warning : public Over_Speed_WarningBase {
public:
    Over_Speed_Warning() = default;
    ~Over_Speed_Warning() override = default;
    Over_Speed_Warning(const Over_Speed_Warning& other) = delete;
    Over_Speed_Warning(Over_Speed_Warning&& other) noexcept = delete;
    Over_Speed_Warning& operator=(const Over_Speed_Warning& other) = delete;
    Over_Speed_Warning& operator=(Over_Speed_Warning&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerIdToUpdatePrivateWarnStatusMid.create(kTimerUpdatePrivateWarnStatusInterval1s, this,
                                                               &Over_Speed_Warning::onOneShotTimerUpdatePrivateWarnStatusMid,
                                                               false) == false) {
            DWarning() << "[Over_Speed_Warning] mOneShotTimerIdToUpdatePrivateWarnStatusMid Create Failed";
        }
        setSFCOver_Speed_WarningSoundOverSpeedWarnStatusRepeatCount(kRepeatCount_1Time);
        setSFCOver_Speed_WarningSoundOverSpeedWarnStatusInterval(kInterval_100s);
        setSFCOver_Speed_WarningTelltaleOverSpeedWarnBlinkValueA(SFCOver_Speed_WarningTelltaleOverSpeedWarnBlinkValueA::ON);
        setSFCOver_Speed_WarningTelltaleOverSpeedWarnBlinkValueB(SFCOver_Speed_WarningTelltaleOverSpeedWarnBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onVariantChanged(const ArgumentsVariantChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        setPrivateProperties();
        updateTelltale();
        updateEventAndSound();
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        setPrivateProperties();
        updateTelltale();
        updateEventAndSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        setPrivateProperties();
        updateTelltale();
        updateEventAndSound();
    }

    void onSpeedValueChanged(const ArgumentsSpeedValueChanged& args) {
        mSpeedValue = args.mInter_DisplaySpeedValueKPH;
        setPrivateProperties();
        updateTelltale();
        updateEventAndSound();
    }

private:
    void registerTimerToUpdatePrivateWarnStatusMid() {
        if (mOneShotTimerIdToUpdatePrivateWarnStatusMid.start() == false) {
            DWarning() << "[Over_Speed_Warning] mOneShotTimerIdToUpdatePrivateWarnStatusMid Start Failed";
        }
    }

    void unregisterTimerToUpdatePrivateWarnStatusMid() {
        mOneShotTimerIdToUpdatePrivateWarnStatusMid.stop();
    }

    // mOneShotTimerIdToUpdatePrivateWarnStatusMid
    void onOneShotTimerUpdatePrivateWarnStatusMid() {
        unregisterTimerToUpdatePrivateWarnStatusMid();
        if (mIsIgnOn == false || mPrivateWarnStatusMid != WarnStatus::WARN_ON) {
            DWarning() << "Can't update Private_OverSpeedWarnStatusMid in WarnStatus::WARN_ON";
        }
        mPrivateWarnStatusMid = WarnStatus::OFF;
        updateTelltale();
        updateEventAndSound();
        flush();
    }

    void setPrivateProperties() {
        if (mIsIgnOn == true) {
            if (mConfigArea == ConfigArea::MID) {
                // 4.1 중동사양 경고 상태 판단 - Part1
                if (mPrivateWarnStatusMid == WarnStatus::OFF && mSpeedValue >= kSpeedValueKPH120) {
                    unregisterTimerToUpdatePrivateWarnStatusMid();
                    mPrivateWarnStatusMid = WarnStatus::WARN_ON;
                } else if (mPrivateWarnStatusMid == WarnStatus::WARN_ON && mSpeedValue < kSpeedValueKPH118) {
                    registerTimerToUpdatePrivateWarnStatusMid();
                } else {
                    // no operation
                }
            } else if (mConfigArea == ConfigArea::INDIA) {
                if (mSpeedValue >= kSpeedValueKPH120) {
                    mPrivateWarnStatusIndia = WarnStatus::WARN2_ON;
                } else if (mPrivateWarnStatusIndia == WarnStatus::OFF && mSpeedValue >= kSpeedValueKPH80) {
                    mPrivateWarnStatusIndia = WarnStatus::WARN1_ON;
                } else if (mSpeedValue < kSpeedValueKPH78) {
                    mPrivateWarnStatusIndia = WarnStatus::OFF;
                } else if (mPrivateWarnStatusIndia == WarnStatus::WARN2_ON && mSpeedValue < kSpeedValueKPH118) {
                    mPrivateWarnStatusIndia = WarnStatus::WARN3_ON;
                } else {
                    // no operation
                }
            } else {
                DWarning() << "Not applicable to operating conditions";
            }
        } else {
            mPrivateWarnStatusMid = WarnStatus::OFF;
            mPrivateWarnStatusIndia = WarnStatus::OFF;
        }
        // 4.1 중동사양 경고 상태 판단 - Part2
        if (mIsIgnOn == false || mConfigArea != ConfigArea::MID || mPrivateWarnStatusMid != WarnStatus::WARN_ON ||
            mSpeedValue >= kSpeedValueKPH118) {
            unregisterTimerToUpdatePrivateWarnStatusMid();
        }
    }

    void updateTelltale() {
        // 5.1 Telltale
        SFCOver_Speed_WarningTelltaleOverSpeedWarnStat telltaleStat = SFCOver_Speed_WarningTelltaleOverSpeedWarnStat::OFF;
        if (mIsIgnOn == true && mConfigArea == ConfigArea::MID && mPrivateWarnStatusMid == WarnStatus::WARN_ON) {
            telltaleStat = SFCOver_Speed_WarningTelltaleOverSpeedWarnStat::BLINK2;
        }
        setSFCOver_Speed_WarningTelltaleOverSpeedWarnStat(telltaleStat);
    }

    void updateEventAndSound() {
        std::string eventID;
        std::string prevEventID;
        std::string soundID;
        std::string prevSoundID;
        SFCOver_Speed_WarningSoundOverSpeedWarnStatusType soundType = SFCOver_Speed_WarningSoundOverSpeedWarnStatusType::NONE;

        if (mIsIgnOn == true) {
            if (mConfigArea == ConfigArea::INDIA) {
                if (mPrivateWarnStatusIndia == WarnStatus::WARN1_ON) {
                    eventID = "E00401";
                    soundID = "SND_OverSpeedWarnIndia1";
                    soundType = SFCOver_Speed_WarningSoundOverSpeedWarnStatusType::INTERVAL;
                } else if (mPrivateWarnStatusIndia == WarnStatus::WARN3_ON) {
                    eventID = "E00403";
                    soundID = "SND_OverSpeedWarnIndia1";
                    soundType = SFCOver_Speed_WarningSoundOverSpeedWarnStatusType::INTERVAL_SKIP_FIRST;
                } else if (mPrivateWarnStatusIndia == WarnStatus::WARN2_ON) {
                    eventID = "E00402";
                    soundID = "SND_OverSpeedWarnIndia2";
                    soundType = SFCOver_Speed_WarningSoundOverSpeedWarnStatusType::INFINITE;
                } else {
                    // no operation
                }
            } else if (mConfigArea == ConfigArea::MID && mPrivateWarnStatusMid == WarnStatus::WARN_ON) {
                // eventID is empty;
                soundID = "SND_OverSpeedWarn";
                soundType = SFCOver_Speed_WarningSoundOverSpeedWarnStatusType::REPEAT_COUNT;
            } else {
                // no operation
            }
        }

        GETCACHEDVALUE(SFC.Over_Speed_Warning.Event.OverSpeedWarnStatus.ID, prevEventID);
        GETCACHEDVALUE(SFC.Over_Speed_Warning.Sound.OverSpeedWarnStatus.ID, prevSoundID);
        if ((prevSoundID.empty() == false && prevSoundID != soundID) ||
            (prevEventID.empty() == false && prevEventID != eventID)) {
            setSFCOver_Speed_WarningSoundOverSpeedWarnStatusStat(SFCOver_Speed_WarningSoundOverSpeedWarnStatusStat::OFF);
            setSFCOver_Speed_WarningEventOverSpeedWarnStatusStat(SFCOver_Speed_WarningEventOverSpeedWarnStatusStat::OFF);
            flushLastGroup();
        }

        if (soundID.empty() == false) {
            setSFCOver_Speed_WarningSoundOverSpeedWarnStatusID(soundID);
            setSFCOver_Speed_WarningSoundOverSpeedWarnStatusStat(SFCOver_Speed_WarningSoundOverSpeedWarnStatusStat::ON);
            setSFCOver_Speed_WarningSoundOverSpeedWarnStatusType(soundType);
        }
        if (eventID.empty() == false) {
            setSFCOver_Speed_WarningEventOverSpeedWarnStatusID(eventID);
            setSFCOver_Speed_WarningEventOverSpeedWarnStatusStat(SFCOver_Speed_WarningEventOverSpeedWarnStatusStat::ON);
        }
    }

private:
    enum class WarnStatus : uint64_t {
        WARN_ON = 0x0,
        WARN1_ON = 0x1,
        WARN2_ON = 0x2,
        WARN3_ON = 0x3,
        OFF = 0x4,
    };

    using ConfigArea = ArgumentsVariantChanged::Inter_ConfigArea;

    HBool mIsIgnOn = false;
    ConfigArea mConfigArea = ConfigArea::DOM;
    WarnStatus mPrivateWarnStatusMid = WarnStatus::OFF;
    WarnStatus mPrivateWarnStatusIndia = WarnStatus::OFF;
    HDouble mSpeedValue = 0.0;
    ssfs::SFCTimer<Over_Speed_Warning> mOneShotTimerIdToUpdatePrivateWarnStatusMid;

    static constexpr uint32_t kTimerUpdatePrivateWarnStatusInterval1s = 1000u;
    static constexpr HDouble kSpeedValueKPH80 = 80.0;
    static constexpr HDouble kSpeedValueKPH120 = 120.0;
    static constexpr HDouble kSpeedValueKPH118 = 118.0;
    static constexpr HDouble kSpeedValueKPH78 = 78.0;
    static constexpr HUInt64 kInterval_100s = 100000;
    static constexpr HUInt64 kRepeatCount_1Time = 1;
};

}  // namespace ccos

#endif  // SFSS_Over_Speed_Warning_H
