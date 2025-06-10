/**
 * @file Seatbelt_Warning.hpp
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
#ifndef SFSS_Seatbelt_Warning_H
#define SFSS_Seatbelt_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Seatbelt_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 3.0.4
// Reference : [EC160] Seatbelt_Warning. Contains : Constant, Telltale, Sound. Recommend: VALUE_CHANGED
class Seatbelt_Warning : public Seatbelt_WarningBase {
public:
    Seatbelt_Warning() = default;
    ~Seatbelt_Warning() override = default;
    Seatbelt_Warning(const Seatbelt_Warning& other) = delete;
    Seatbelt_Warning(Seatbelt_Warning&& other) noexcept = delete;
    Seatbelt_Warning& operator=(const Seatbelt_Warning& other) = delete;
    Seatbelt_Warning& operator=(Seatbelt_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSeatbelt_WarningSoundSeatbeltWarningID("SND_SeatBelt");
        setSFCSeatbelt_WarningSoundSeatbeltWarningType(SFCSeatbelt_WarningSoundSeatbeltWarningType::REPEAT_COUNT);
        setSFCSeatbelt_WarningSoundSeatbeltWarningRepeatCount(1);
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep1ID("SND_SeatBeltStep1");
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep1Type(SFCSeatbelt_WarningSoundSeatbeltWarningStep1Type::REPEAT_COUNT);
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep1RepeatCount(1);
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep2ID("SND_SeatBeltStep2");
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep2Type(SFCSeatbelt_WarningSoundSeatbeltWarningStep2Type::REPEAT_COUNT);
        setSFCSeatbelt_WarningSoundSeatbeltWarningStep2RepeatCount(1);

        if (mOneShotTimerPrivateRow2WarnTime.create(kTimerK2WarnTempOffInterval2s, this,
                                                    &Seatbelt_Warning::onOneShotTimerPrivateRow2WarnTime, false) == false) {
            DWarning() << "[Seatbelt_Warning] mOneShotTimerPrivateRow2WarnTime Create Failed";
        }
        if (mOneShotTimerPrivateRow3WarnTime.create(kTimerK3WarnTempOffInterval2s, this,
                                                    &Seatbelt_Warning::onOneShotTimerPrivateRow3WarnTime, false) == false) {
            DWarning() << "[Seatbelt_Warning] mOneShotTimerPrivateRow3WarnTime Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateAll();
    }

    void onSeatbeltDriverChanged(const ArgumentsSeatbeltDriverChanged& args) {
        mSeatBeltIndiDriverStatus = args.mInput_SeatBeltIndiDriverStatus;
        updateTelltale();
    }

    void onSeatbeltWarnChanged(const ArgumentsSeatbeltWarnChanged& args) {
        mSeatBeltWarnSoundStatus = args.mInput_SeatBeltWarnSoundStatus;
        mSeatBeltWarnStepSoundStatus = args.mInput_SeatBeltWarnStepSoundStatus;
        updateSeatbeltWarningSound();
        updateSeatbeltWarningStep1Sound();
        updateSeatbeltWarningStep2Sound();
    }

    void on3rdStatusChanged(const Arguments3rdStatusChanged& args) {
        mSeatBeltIndi3rdCenterStatus = args.mInput_SeatBeltIndi3rdCenterStatus;
        mSeatBeltIndi3rdLeftStatus = args.mInput_SeatBeltIndi3rdLeftStatus;
        mSeatBeltIndi3rdRightStatus = args.mInput_SeatBeltIndi3rdRightStatus;
        updatePrivateRearSeatbeltRow3WarnStatus();
        updateRearSeatbeltRow3Stat();
        updateSeat3rdRearLeftStat();
        updateSeat3rdRearCenterStat();
        updateSeat3rdRearRightStat();
    }

    void on2rdStatusChanged(const Arguments2rdStatusChanged& args) {
        mSeatBeltIndiRearCenterStatus = args.mInput_SeatBeltIndiRearCenterStatus;
        mSeatBeltIndiRearLeftStatus = args.mInput_SeatBeltIndiRearLeftStatus;
        mSeatBeltIndiRearRightStatus = args.mInput_SeatBeltIndiRearRightStatus;
        updatePrivateRearSeatbeltRow2WarnStatus();
        updatePrivateRearSeatbeltRow3WarnStatus();
        updateRearSeatbeltRow2Stat();
        updateRearSeatbeltRow3Stat();
        updateSeat2ndRearLeftStat();
        updateSeat2ndRearCenterStat();
        updateSeat2ndRearRightStat();
    }

    void onConfigRSBRChanged(const ArgumentsConfigRSBRChanged& args) {
        mConfigRSBR = args.mInter_ConfigRSBR;
        updateRearSeatbeltRow2Stat();
        updateRearSeatbeltRow3Stat();
        updateRearSeatbeltLayoutStat();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updatePrivateRearSeatbeltRow2WarnStatus();
        updatePrivateRearSeatbeltRow3WarnStatus();
        updateRearSeatbeltRow2Stat();
        updateRearSeatbeltRow3Stat();
        updateRearSeatbeltLayoutStat();
        updateTelltale();
        updateSeat2ndRearLeftStat();
        updateSeat2ndRearCenterStat();
        updateSeat2ndRearRightStat();
        updateSeat3rdRearLeftStat();
        updateSeat3rdRearCenterStat();
        updateSeat3rdRearRightStat();
        updateSeatbeltWarningSound();
        updateSeatbeltWarningStep1Sound();
        updateSeatbeltWarningStep2Sound();
    }

    void updatePrivateRearSeatbeltRow2WarnStatus() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::SEATED ||
                mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::SEATED ||
                mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::SEATED) {
                mOneShotTimerPrivateRow2WarnTime.stop();
                m2WarnTempOffStatus = false;
                mPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::WARN_ON;
            } else if ((mCurrentPrivateRearSeatbeltRow2WarnStatus == PrivateRearSeatbeltRow2WarnStatus::WARN_ON) &&
                       (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::BELTED ||
                        mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::OFF ||
                        mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::BELTED ||
                        mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::OFF ||
                        mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::BELTED ||
                        mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::OFF)) {
                mPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::TEMP_OFF;

                if (mOneShotTimerPrivateRow2WarnTime.start() == false) {
                    DWarning() << "[Seatbelt_Warning] mOneShotTimerPrivateRow2WarnTime Start Failed";
                }
            } else if ((mCurrentPrivateRearSeatbeltRow2WarnStatus == PrivateRearSeatbeltRow2WarnStatus::TEMP_OFF &&
                        m2WarnTempOffStatus)) {
                m2WarnTempOffStatus = false;
                mPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::WARN_OFF;
            } else {
                // not specified in Section 5.14.4.1.1
            }
        } else {
            mOneShotTimerPrivateRow2WarnTime.stop();
            m2WarnTempOffStatus = false;
            mPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::WARN_OFF;
        }
        mCurrentPrivateRearSeatbeltRow2WarnStatus = mPrivateRearSeatbeltRow2WarnStatus;
    }

    void onOneShotTimerPrivateRow2WarnTime() {
        m2WarnTempOffStatus = true;
        updatePrivateRearSeatbeltRow2WarnStatus();
        updateRearSeatbeltRow2Stat();
        flush();
    }

    void updatePrivateRearSeatbeltRow3WarnStatus() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::SEATED ||
                mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::SEATED ||
                mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::SEATED ||
                mSeatBeltIndi3rdLeftStatus == SeatBeltIndi3rdLeftStatus::SEATED ||
                mSeatBeltIndi3rdCenterStatus == SeatBeltIndi3rdCenterStatus::SEATED ||
                mSeatBeltIndi3rdRightStatus == SeatBeltIndi3rdRightStatus::SEATED) {
                mOneShotTimerPrivateRow3WarnTime.stop();
                m3WarnTempOffStatus = false;
                mPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::WARN_ON;
            } else if ((mCurrentPrivateRearSeatbeltRow3WarnStatus == PrivateRearSeatbeltRow3WarnStatus::WARN_ON) &&
                       (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::BELTED ||
                        mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::OFF ||
                        mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::BELTED ||
                        mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::OFF ||
                        mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::BELTED ||
                        mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::OFF ||
                        mSeatBeltIndi3rdLeftStatus == SeatBeltIndi3rdLeftStatus::BELTED ||
                        mSeatBeltIndi3rdLeftStatus == SeatBeltIndi3rdLeftStatus::OFF ||
                        mSeatBeltIndi3rdCenterStatus == SeatBeltIndi3rdCenterStatus::BELTED ||
                        mSeatBeltIndi3rdCenterStatus == SeatBeltIndi3rdCenterStatus::OFF ||
                        mSeatBeltIndi3rdRightStatus == SeatBeltIndi3rdRightStatus::BELTED ||
                        mSeatBeltIndi3rdRightStatus == SeatBeltIndi3rdRightStatus::OFF)) {
                mPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::TEMP_OFF;
                if (mOneShotTimerPrivateRow3WarnTime.start() == false) {
                    DWarning() << "[Seatbelt_Warning] mOneShotTimerPrivateRow3WarnTime Start Failed";
                }
            } else if (mCurrentPrivateRearSeatbeltRow3WarnStatus == PrivateRearSeatbeltRow3WarnStatus::TEMP_OFF &&
                       m3WarnTempOffStatus) {
                mPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::WARN_OFF;
            } else {
                // not specified in Section 5.14.4.1.2
            }
        } else {
            mOneShotTimerPrivateRow3WarnTime.stop();
            m3WarnTempOffStatus = false;
            mPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::WARN_OFF;
        }
        mCurrentPrivateRearSeatbeltRow3WarnStatus = mPrivateRearSeatbeltRow3WarnStatus;
    }

    void onOneShotTimerPrivateRow3WarnTime() {
        m3WarnTempOffStatus = true;
        updatePrivateRearSeatbeltRow3WarnStatus();
        updateRearSeatbeltRow3Stat();
        flush();
    }

    void updateRearSeatbeltLayoutStat() {
        SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::NONE;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantRearSeatbeltLayoutStat(SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::NONE);
            return;
        }

        switch (mConfigRSBR) {
            case ConfigRSBR::R2_P2:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL2;
                break;
            case ConfigRSBR::R2_P3:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL3;
                break;
            case ConfigRSBR::R2_P2_R3_P2:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL2_ROW3_COL2;
                break;
            case ConfigRSBR::R2_P3_R3_P2:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL3_ROW3_COL2;
                break;
            case ConfigRSBR::R2_P2_R3_P3:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL2_ROW3_COL3;
                break;
            case ConfigRSBR::R2_P3_R3_P3:
                stat = SFCSeatbelt_WarningConstantRearSeatbeltLayoutStat::ROW2_COL3_ROW3_COL3;
                break;
            default:
                break;
        }
        setSFCSeatbelt_WarningConstantRearSeatbeltLayoutStat(stat);
    }

    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSeatBeltIndiDriverStatus == SeatBeltIndiDriverStatus::ON) {
            setSFCSeatbelt_WarningTelltaleSeatbeltStat(SFCSeatbelt_WarningTelltaleSeatbeltStat::ON);
        } else {
            setSFCSeatbelt_WarningTelltaleSeatbeltStat(SFCSeatbelt_WarningTelltaleSeatbeltStat::OFF);
        }
    }

    void updateRearSeatbeltRow2Stat() {
        SFCSeatbelt_WarningConstantRearSeatbeltRow2Stat stat = SFCSeatbelt_WarningConstantRearSeatbeltRow2Stat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantRearSeatbeltRow2Stat(SFCSeatbelt_WarningConstantRearSeatbeltRow2Stat::OFF);
            return;
        }

        if ((mConfigRSBR == ConfigRSBR::R2_P2 || mConfigRSBR == ConfigRSBR::R2_P3) &&
            (mPrivateRearSeatbeltRow2WarnStatus == PrivateRearSeatbeltRow2WarnStatus::WARN_ON ||
             mPrivateRearSeatbeltRow2WarnStatus == PrivateRearSeatbeltRow2WarnStatus::TEMP_OFF)) {
            stat = SFCSeatbelt_WarningConstantRearSeatbeltRow2Stat::ON;
        }
        setSFCSeatbelt_WarningConstantRearSeatbeltRow2Stat(stat);
    }

    void updateRearSeatbeltRow3Stat() {
        SFCSeatbelt_WarningConstantRearSeatbeltRow3Stat stat = SFCSeatbelt_WarningConstantRearSeatbeltRow3Stat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantRearSeatbeltRow3Stat(SFCSeatbelt_WarningConstantRearSeatbeltRow3Stat::OFF);
            return;
        }

        if ((mConfigRSBR == ConfigRSBR::R2_P2_R3_P2 || mConfigRSBR == ConfigRSBR::R2_P3_R3_P2 ||
             mConfigRSBR == ConfigRSBR::R2_P2_R3_P3 || mConfigRSBR == ConfigRSBR::R2_P3_R3_P3) &&
            (mPrivateRearSeatbeltRow3WarnStatus == PrivateRearSeatbeltRow3WarnStatus::WARN_ON ||
             mPrivateRearSeatbeltRow3WarnStatus == PrivateRearSeatbeltRow3WarnStatus::TEMP_OFF)) {
            stat = SFCSeatbelt_WarningConstantRearSeatbeltRow3Stat::ON;
        }
        setSFCSeatbelt_WarningConstantRearSeatbeltRow3Stat(stat);
    }

    void updateSeat2ndRearLeftStat() {
        SFCSeatbelt_WarningConstantSeat2ndRearLeftStat stat = SFCSeatbelt_WarningConstantSeat2ndRearLeftStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat2ndRearLeftStat(SFCSeatbelt_WarningConstantSeat2ndRearLeftStat::OFF);
            return;
        }

        if (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearLeftStat::SEATED;
        } else if (mSeatBeltIndiRearLeftStatus == SeatBeltIndiRearLeftStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearLeftStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat2ndRearLeftStat(stat);
    }

    void updateSeat2ndRearCenterStat() {
        SFCSeatbelt_WarningConstantSeat2ndRearCenterStat stat = SFCSeatbelt_WarningConstantSeat2ndRearCenterStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat2ndRearCenterStat(SFCSeatbelt_WarningConstantSeat2ndRearCenterStat::OFF);
            return;
        }

        if (mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearCenterStat::SEATED;
        } else if (mSeatBeltIndiRearCenterStatus == SeatBeltIndiRearCenterStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearCenterStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat2ndRearCenterStat(stat);
    }

    void updateSeat2ndRearRightStat() {
        SFCSeatbelt_WarningConstantSeat2ndRearRightStat stat = SFCSeatbelt_WarningConstantSeat2ndRearRightStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat2ndRearRightStat(SFCSeatbelt_WarningConstantSeat2ndRearRightStat::OFF);
            return;
        }

        if (mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearRightStat::SEATED;
        } else if (mSeatBeltIndiRearRightStatus == SeatBeltIndiRearRightStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat2ndRearRightStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat2ndRearRightStat(stat);
    }

    void updateSeat3rdRearLeftStat() {
        SFCSeatbelt_WarningConstantSeat3rdRearLeftStat stat = SFCSeatbelt_WarningConstantSeat3rdRearLeftStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat3rdRearLeftStat(SFCSeatbelt_WarningConstantSeat3rdRearLeftStat::OFF);
            return;
        }

        if (mSeatBeltIndi3rdLeftStatus == SeatBeltIndi3rdLeftStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearLeftStat::SEATED;
        } else if (mSeatBeltIndi3rdLeftStatus == SeatBeltIndi3rdLeftStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearLeftStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat3rdRearLeftStat(stat);
    }

    void updateSeat3rdRearCenterStat() {
        SFCSeatbelt_WarningConstantSeat3rdRearCenterStat stat = SFCSeatbelt_WarningConstantSeat3rdRearCenterStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat3rdRearCenterStat(SFCSeatbelt_WarningConstantSeat3rdRearCenterStat::OFF);
            return;
        }

        if (mSeatBeltIndi3rdCenterStatus == SeatBeltIndi3rdCenterStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearCenterStat::SEATED;
        } else if (mSeatBeltIndi3rdCenterStatus == SeatBeltIndi3rdCenterStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearCenterStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat3rdRearCenterStat(stat);
    }

    void updateSeat3rdRearRightStat() {
        SFCSeatbelt_WarningConstantSeat3rdRearRightStat stat = SFCSeatbelt_WarningConstantSeat3rdRearRightStat::OFF;
        if (mIgnElapsed != IgnElapsed::ON_0ms) {
            setSFCSeatbelt_WarningConstantSeat3rdRearRightStat(SFCSeatbelt_WarningConstantSeat3rdRearRightStat::OFF);
            return;
        }

        if (mSeatBeltIndi3rdRightStatus == SeatBeltIndi3rdRightStatus::SEATED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearRightStat::SEATED;
        } else if (mSeatBeltIndi3rdRightStatus == SeatBeltIndi3rdRightStatus::BELTED) {
            stat = SFCSeatbelt_WarningConstantSeat3rdRearRightStat::BELTED;
        } else {
            // not specified in Section 5.14.5.2.3
        }
        setSFCSeatbelt_WarningConstantSeat3rdRearRightStat(stat);
    }

    void updateSeatbeltWarningSound() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSeatBeltWarnSoundStatus == SeatBeltWarnSoundStatus::ON) {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStat(SFCSeatbelt_WarningSoundSeatbeltWarningStat::ON);
        } else {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStat(SFCSeatbelt_WarningSoundSeatbeltWarningStat::OFF);
        }
    }

    void updateSeatbeltWarningStep1Sound() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && (mSeatBeltWarnStepSoundStatus == SeatBeltWarnStepSoundStatus::PRE_WARN ||
                                                  mSeatBeltWarnStepSoundStatus == SeatBeltWarnStepSoundStatus::WARN1 ||
                                                  mSeatBeltWarnStepSoundStatus == SeatBeltWarnStepSoundStatus::WARN2)) {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStep1Stat(SFCSeatbelt_WarningSoundSeatbeltWarningStep1Stat::ON);
        } else {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStep1Stat(SFCSeatbelt_WarningSoundSeatbeltWarningStep1Stat::OFF);
        }
    }

    void updateSeatbeltWarningStep2Sound() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSeatBeltWarnStepSoundStatus == SeatBeltWarnStepSoundStatus::WARN3) {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStep2Stat(SFCSeatbelt_WarningSoundSeatbeltWarningStep2Stat::ON);
        } else {
            setSFCSeatbelt_WarningSoundSeatbeltWarningStep2Stat(SFCSeatbelt_WarningSoundSeatbeltWarningStep2Stat::OFF);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    enum class PrivateRearSeatbeltRow2WarnStatus : HUInt64 {
        NONE,
        WARN_ON,
        TEMP_OFF,
        WARN_OFF,
        MAX
    };
    enum class PrivateRearSeatbeltRow3WarnStatus : HUInt64 {
        NONE,
        WARN_ON,
        TEMP_OFF,
        WARN_OFF,
        MAX
    };

    using SeatBeltIndiDriverStatus = ArgumentsSeatbeltDriverChanged::Input_SeatBeltIndiDriverStatus;
    using SeatBeltWarnSoundStatus = ArgumentsSeatbeltWarnChanged::Input_SeatBeltWarnSoundStatus;
    using SeatBeltWarnStepSoundStatus = ArgumentsSeatbeltWarnChanged::Input_SeatBeltWarnStepSoundStatus;
    using SeatBeltIndi3rdCenterStatus = Arguments3rdStatusChanged::Input_SeatBeltIndi3rdCenterStatus;
    using SeatBeltIndi3rdLeftStatus = Arguments3rdStatusChanged::Input_SeatBeltIndi3rdLeftStatus;
    using SeatBeltIndi3rdRightStatus = Arguments3rdStatusChanged::Input_SeatBeltIndi3rdRightStatus;
    using SeatBeltIndiRearCenterStatus = Arguments2rdStatusChanged::Input_SeatBeltIndiRearCenterStatus;
    using SeatBeltIndiRearLeftStatus = Arguments2rdStatusChanged::Input_SeatBeltIndiRearLeftStatus;
    using SeatBeltIndiRearRightStatus = Arguments2rdStatusChanged::Input_SeatBeltIndiRearRightStatus;
    using ConfigRSBR = ArgumentsConfigRSBRChanged::Inter_ConfigRSBR;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    PrivateRearSeatbeltRow2WarnStatus mPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::NONE;
    PrivateRearSeatbeltRow2WarnStatus mCurrentPrivateRearSeatbeltRow2WarnStatus = PrivateRearSeatbeltRow2WarnStatus::NONE;
    PrivateRearSeatbeltRow3WarnStatus mPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::NONE;
    PrivateRearSeatbeltRow3WarnStatus mCurrentPrivateRearSeatbeltRow3WarnStatus = PrivateRearSeatbeltRow3WarnStatus::NONE;
    SeatBeltIndiDriverStatus mSeatBeltIndiDriverStatus = SeatBeltIndiDriverStatus::OFF;
    SeatBeltWarnSoundStatus mSeatBeltWarnSoundStatus = SeatBeltWarnSoundStatus::OFF;
    SeatBeltWarnStepSoundStatus mSeatBeltWarnStepSoundStatus = SeatBeltWarnStepSoundStatus::OFF;
    SeatBeltIndi3rdCenterStatus mSeatBeltIndi3rdCenterStatus = SeatBeltIndi3rdCenterStatus::OFF;
    SeatBeltIndi3rdLeftStatus mSeatBeltIndi3rdLeftStatus = SeatBeltIndi3rdLeftStatus::OFF;
    SeatBeltIndi3rdRightStatus mSeatBeltIndi3rdRightStatus = SeatBeltIndi3rdRightStatus::OFF;
    SeatBeltIndiRearCenterStatus mSeatBeltIndiRearCenterStatus = SeatBeltIndiRearCenterStatus::OFF;
    SeatBeltIndiRearLeftStatus mSeatBeltIndiRearLeftStatus = SeatBeltIndiRearLeftStatus::OFF;
    SeatBeltIndiRearRightStatus mSeatBeltIndiRearRightStatus = SeatBeltIndiRearRightStatus::OFF;
    ConfigRSBR mConfigRSBR = ConfigRSBR::NONE;
    static constexpr uint32_t kTimerK2WarnTempOffInterval2s = 2000;
    static constexpr uint32_t kTimerK3WarnTempOffInterval2s = 2000;
    ssfs::SFCTimer<Seatbelt_Warning> mOneShotTimerPrivateRow2WarnTime;
    ssfs::SFCTimer<Seatbelt_Warning> mOneShotTimerPrivateRow3WarnTime;
    HBool m2WarnTempOffStatus = false;
    HBool m3WarnTempOffStatus = false;
};

}  // namespace ccos

#endif  // SFSS_Seatbelt_Warning_H
