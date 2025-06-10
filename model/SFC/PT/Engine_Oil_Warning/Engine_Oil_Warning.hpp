/**
 * @file Engine_Oil_Warning.hpp
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
#ifndef SFSS_Engine_Oil_Warning_H
#define SFSS_Engine_Oil_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_Oil_WarningBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 7.0.0
// Reference : [PT130] Engine_Oil_Warning. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Engine_Oil_Warning : public Engine_Oil_WarningBase {
public:
    Engine_Oil_Warning() = default;
    ~Engine_Oil_Warning() override = default;
    Engine_Oil_Warning(const Engine_Oil_Warning& other) = delete;
    Engine_Oil_Warning(Engine_Oil_Warning&& other) noexcept = delete;
    Engine_Oil_Warning& operator=(const Engine_Oil_Warning& other) = delete;
    Engine_Oil_Warning& operator=(Engine_Oil_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngine_Oil_WarningEventEngineOilLevelWarningID("E21001");
        setSFCEngine_Oil_WarningEventEngineOilLevelWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCEngine_Oil_WarningEventEngineOilLevelWarningLinkedSoundType(
            SFCEngine_Oil_WarningEventEngineOilLevelWarningLinkedSoundType::REPEAT_COUNT);
        setSFCEngine_Oil_WarningEventEngineOilLevelWarningLinkedSoundRepeatCount(1);
        setSFCEngine_Oil_WarningEventEngineOilChangeLinkedSoundType(
            SFCEngine_Oil_WarningEventEngineOilChangeLinkedSoundType::REPEAT_COUNT);
        setSFCEngine_Oil_WarningEventEngineOilChangeLinkedSoundRepeatCount(1);
        if (mOneShotTimerOilPressureWarnCANON500msStatus.create(kTimerTelltaleInterval500ms, this,
                                                                &Engine_Oil_Warning::onOneShotTimer500msOilPressureWarnCANStatus,
                                                                false) == false) {
            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnCANON500msStatus Create Failed";
        }
        if (mOneShotTimerOilPressureWarnCANON3000msStatus.create(
                kTimerTelltaleInterval3s, this, &Engine_Oil_Warning::onOneShotTimer3000msOilPressureWarnCANStatus, false) ==
            false) {
            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnCANON3000msStatus Create Failed";
        }
        if (mOneShotTimerOilPressureWarnHWON500msStatus.create(kTimerTelltaleInterval500ms, this,
                                                               &Engine_Oil_Warning::onOneShotTimer500msOilPressureWarnHWStatus,
                                                               false) == false) {
            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnHWON500msStatus Create Failed";
        }
        if (mOneShotTimerOilPressureWarnHWON3000msStatus.create(kTimerTelltaleInterval3s, this,
                                                                &Engine_Oil_Warning::onOneShotTimer3000msOilPressureWarnHWStatus,
                                                                false) == false) {
            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnHWON3000msStatus Create Failed";
        }

        if (mOneShotTimerISGStat3000msStatus.create(kTimerTelltaleInterval3s, this,
                                                    &Engine_Oil_Warning::onOneShotTimerIsg3000msTelltale, false) == false) {
            DWarning() << "[Engine_Oil_Warning] mOneShotTimerISGStat3000msStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        controlTelltaleTimer();
        updateTelltale();
        updateEventOilLevelWarn();
        updateEventOilChange();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        controlTelltaleTimer();
        updateTelltale();
        updateEventOilLevelWarn();
        updateEventOilChange();
    }

    void onIsgActiveStatusChanged(const ArgumentsIsgActiveStatusChanged& args) {
        mISGPrevActiveStatus = mISGCurrentActiveStatus;
        mISGCurrentActiveStatus = args.mInter_ISGActiveStatus;
        if (mISGPrevActiveStatus == ISGActiveStatus::ON && mISGCurrentActiveStatus == ISGActiveStatus::OFF) {
            mIsISGStatusChangedfromOnToOffWithin3000ms = true;
            if (mOneShotTimerISGStat3000msStatus.start() == false) {
                DWarning() << "[Engine_Oil_Warning] mOneShotTimerISGStat3000msStatus Start Failed";
            }
        } else {
            mIsISGStatusChangedfromOnToOffWithin3000ms = false;
            mOneShotTimerISGStat3000msStatus.stop();
        }
        updateTelltale();
    }

    void onEngineStatusChanged(const ArgumentsEngineStatusChanged& args) {
        mEngineOnStatus = args.mInter_EngineOnStatus;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onHevEngineStatusChanged(const ArgumentsHevEngineStatusChanged& args) {
        mHevEngineOperStatus = args.mInput_HevEngineOperStatus;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onEngineOilPressureWarnCANStatusChanged(const ArgumentsEngineOilPressureWarnCANStatusChanged& args) {
        mEngineOilPressureWarnCANStatus = args.mInput_EngineOilPressureWarnCANStatus;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onEngineOilPressureWarnHWStatusChanged(const ArgumentsEngineOilPressureWarnHWStatusChanged& args) {
        mEngineOilPressureWarnHWStatus = args.mInput_EngineOilPressureWarnHWStatus;
        controlTelltaleTimer();
        updateTelltale();
    }

    void onEngineOilLevelChanged(const ArgumentsEngineOilLevelChanged& args) {
        mEngineOilLevelWarnStatus = args.mInput_EngineOilLevelWarnStatus;
        updateEventOilLevelWarn();
    }

    void onEngineOilLifeChanged(const ArgumentsEngineOilLifeChanged& args) {
        mEngineOilLifeWarnStatus = args.mInput_EngineOilLifeWarnStatus;
        updateEventOilChange();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCEngine_Oil_WarningTelltaleEngOilPressureStat telltaleStat = SFCEngine_Oil_WarningTelltaleEngOilPressureStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mIsISGStatusChangedfromOnToOffWithin3000ms == true && mISGPrevActiveStatus == ISGActiveStatus::ON &&
                mISGCurrentActiveStatus == ISGActiveStatus::OFF) {
                // no operation, OFF
            } else if (mISGCurrentActiveStatus == ISGActiveStatus::OFF) {
                if (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV ||
                    mConfigVehicleType == ConfigVehicleType::EREV) {
                    if (mEngineOnStatus == EngineOnStatus::OFF) {
                        if ((mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON &&
                             (mIsOilPressureWarnCANOnAfter500ms == true ||
                              (mIsISGStatusChangedfromOnToOffWithin3000ms == false &&
                               mOneShotTimerOilPressureWarnCANON500msStatus.isRunning() == false))) ||
                            (mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON &&
                             (mIsOilPressureWarnHWOnAfter500ms == true ||
                              (mIsISGStatusChangedfromOnToOffWithin3000ms == false &&
                               mOneShotTimerOilPressureWarnHWON500msStatus.isRunning() == false)))) {
                            telltaleStat = SFCEngine_Oil_WarningTelltaleEngOilPressureStat::ON;
                        }
                    } else if (mEngineOnStatus == EngineOnStatus::ON) {
                        if (mHevEngineOperStatus == HevEngineOperStatus::IDLE_SPEED ||
                            mHevEngineOperStatus == HevEngineOperStatus::PART_LOAD ||
                            mHevEngineOperStatus == HevEngineOperStatus::PULL ||
                            mHevEngineOperStatus == HevEngineOperStatus::FUEL_CUT_OFF) {
                            if ((mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON &&
                                 (mIsOilPressureWarnCANOnAfter3000ms == true ||
                                  (mIsISGStatusChangedfromOnToOffWithin3000ms == false &&
                                   mOneShotTimerOilPressureWarnCANON3000msStatus.isRunning() == false))) ||
                                (mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON &&
                                 (mIsOilPressureWarnHWOnAfter3000ms == true ||
                                  (mIsISGStatusChangedfromOnToOffWithin3000ms == false &&
                                   mOneShotTimerOilPressureWarnHWON3000msStatus.isRunning() == false)))) {
                                telltaleStat = SFCEngine_Oil_WarningTelltaleEngOilPressureStat::ON;
                            }
                        }
                    } else {
                        // no operation
                    }
                } else if (mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON ||
                           mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON) {
                    telltaleStat = SFCEngine_Oil_WarningTelltaleEngOilPressureStat::ON;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
        setSFCEngine_Oil_WarningTelltaleEngOilPressureStat(telltaleStat);
    }

    void updateEventOilLevelWarn() {
        SFCEngine_Oil_WarningEventEngineOilLevelWarningStat eventStat = SFCEngine_Oil_WarningEventEngineOilLevelWarningStat::OFF;
        if (mIsIgnOn == true && mEngineOilLevelWarnStatus == EngineOilLevelWarnStatus::ON) {
            eventStat = SFCEngine_Oil_WarningEventEngineOilLevelWarningStat::ON;
        }
        setSFCEngine_Oil_WarningEventEngineOilLevelWarningStat(eventStat);
    }

    void updateEventOilChange() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIsIgnOn == true) {
            if (mEngineOilLifeWarnStatus == EngineOilLifeWarnStatus::WARN1) {
                eventID = "E20901";
                linkedSoundID = "SND_PopUpInform1";
            } else if (mEngineOilLifeWarnStatus == EngineOilLifeWarnStatus::WARN2) {
                eventID = "E20902";
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mEngineOilLifeWarnStatus == EngineOilLifeWarnStatus::CHECK_URGENT) {
                eventID = "E20903";
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mEngineOilLifeWarnStatus == EngineOilLifeWarnStatus::CHECK_GUIDE) {
                eventID = "E20904";
                linkedSoundID = "SND_PopUpWarn1";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Engine_Oil_Warning.Event.EngineOilChange.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEngine_Oil_WarningEventEngineOilChangeStat(SFCEngine_Oil_WarningEventEngineOilChangeStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEngine_Oil_WarningEventEngineOilChangeStat(SFCEngine_Oil_WarningEventEngineOilChangeStat::ON);
            setSFCEngine_Oil_WarningEventEngineOilChangeID(eventID);
            setSFCEngine_Oil_WarningEventEngineOilChangeLinkedSoundID(linkedSoundID);
        }
    }

    void controlTelltaleTimer() {
        OilPressureWarnTimerOffType offType = OilPressureWarnTimerOffType::ALL;
        if ((mIgnElapsed == IgnElapsed::ON_500ms) && mISGCurrentActiveStatus == ISGActiveStatus::OFF &&
            mIsISGStatusChangedfromOnToOffWithin3000ms == false &&
            (mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV ||
             mConfigVehicleType == ConfigVehicleType::EREV)) {
            if (mEngineOnStatus == EngineOnStatus::ON) {
                if (mHevEngineOperStatus == HevEngineOperStatus::IDLE_SPEED ||
                    mHevEngineOperStatus == HevEngineOperStatus::PART_LOAD || mHevEngineOperStatus == HevEngineOperStatus::PULL ||
                    mHevEngineOperStatus == HevEngineOperStatus::FUEL_CUT_OFF) {
                    if (mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON) {
                        offType = OilPressureWarnTimerOffType::TIMER_500MS;
                        if (mOneShotTimerOilPressureWarnCANON3000msStatus.start() == false) {
                            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnCANON3000msStatus Start Failed";
                        }
                    }

                    if (mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON) {
                        offType = OilPressureWarnTimerOffType::TIMER_500MS;
                        if (mOneShotTimerOilPressureWarnHWON3000msStatus.start() == false) {
                            DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnHWON3000msStatus Start Failed";
                        }
                    }
                }
            } else if (mEngineOnStatus == EngineOnStatus::OFF) {
                if ((mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON)) {
                    offType = OilPressureWarnTimerOffType::TIMER_3000MS;
                    if (mOneShotTimerOilPressureWarnCANON500msStatus.start() == false) {
                        DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnCANON500msStatus Start Failed";
                    }
                }
                if ((mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON)) {
                    offType = OilPressureWarnTimerOffType::TIMER_3000MS;
                    if (mOneShotTimerOilPressureWarnHWON500msStatus.start() == false) {
                        DWarning() << "[Engine_Oil_Warning] mOneShotTimerOilPressureWarnHWON500msStatus Start Failed";
                    }
                }
            } else {
                // nothing
            }
        }

        stopTimer(offType);
    }

    enum class OilPressureWarnTimerOffType : HUInt64 {
        NONE,
        ALL,
        TIMER_500MS,
        TIMER_3000MS,
        MAX
    };

    void stopTimer(const OilPressureWarnTimerOffType& type) {
        if (type == OilPressureWarnTimerOffType::ALL) {
            mOneShotTimerOilPressureWarnCANON3000msStatus.stop();
            mOneShotTimerOilPressureWarnHWON3000msStatus.stop();
            mOneShotTimerOilPressureWarnCANON500msStatus.stop();
            mOneShotTimerOilPressureWarnHWON500msStatus.stop();
        } else if (type == OilPressureWarnTimerOffType::TIMER_500MS) {
            mOneShotTimerOilPressureWarnCANON500msStatus.stop();
            mOneShotTimerOilPressureWarnHWON500msStatus.stop();
        } else if (type == OilPressureWarnTimerOffType::TIMER_3000MS) {
            mOneShotTimerOilPressureWarnCANON3000msStatus.stop();
            mOneShotTimerOilPressureWarnHWON3000msStatus.stop();
        } else {
            // nothing
        }
    }

    void onOneShotTimer500msOilPressureWarnCANStatus() {
        if (mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON) {
            mIsOilPressureWarnCANOnAfter500ms = true;
        }
        updateTelltale();
        mIsOilPressureWarnCANOnAfter500ms = false;
        flush();
    }

    void onOneShotTimer3000msOilPressureWarnCANStatus() {
        if (mEngineOilPressureWarnCANStatus == EngineOilPressureWarnCANStatus::ON) {
            mIsOilPressureWarnCANOnAfter3000ms = true;
        }
        updateTelltale();
        mIsOilPressureWarnCANOnAfter3000ms = false;
        flush();
    }

    void onOneShotTimer500msOilPressureWarnHWStatus() {
        if (mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON) {
            mIsOilPressureWarnHWOnAfter500ms = true;
        }
        updateTelltale();
        mIsOilPressureWarnHWOnAfter500ms = false;
        flush();
    }

    void onOneShotTimer3000msOilPressureWarnHWStatus() {
        if (mEngineOilPressureWarnHWStatus == EngineOilPressureWarnHWStatus::ON) {
            mIsOilPressureWarnHWOnAfter3000ms = true;
        }
        updateTelltale();
        mIsOilPressureWarnHWOnAfter3000ms = false;
        flush();
    }

    void onOneShotTimerIsg3000msTelltale() {
        if (mISGPrevActiveStatus == ISGActiveStatus::ON && mISGCurrentActiveStatus == ISGActiveStatus::OFF &&
            mIsISGStatusChangedfromOnToOffWithin3000ms == true) {
            mIsISGStatusChangedfromOnToOffWithin3000ms = false;
        }
        controlTelltaleTimer();
        updateTelltale();
        flush();
    }

    static constexpr uint32_t kTimerTelltaleInterval500ms = 500u;
    static constexpr uint32_t kTimerTelltaleInterval3s = 3000u;

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ISGActiveStatus = ArgumentsIsgActiveStatusChanged::Inter_ISGActiveStatus;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using EngineOnStatus = ArgumentsEngineStatusChanged::Inter_EngineOnStatus;
    using HevEngineOperStatus = ArgumentsHevEngineStatusChanged::Input_HevEngineOperStatus;
    using EngineOilPressureWarnCANStatus = ArgumentsEngineOilPressureWarnCANStatusChanged::Input_EngineOilPressureWarnCANStatus;
    using EngineOilPressureWarnHWStatus = ArgumentsEngineOilPressureWarnHWStatusChanged::Input_EngineOilPressureWarnHWStatus;
    using EngineOilLevelWarnStatus = ArgumentsEngineOilLevelChanged::Input_EngineOilLevelWarnStatus;
    using EngineOilLifeWarnStatus = ArgumentsEngineOilLifeChanged::Input_EngineOilLifeWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    HBool mIsIgnOn = false;
    ISGActiveStatus mISGPrevActiveStatus = ISGActiveStatus::NONE;
    ISGActiveStatus mISGCurrentActiveStatus = ISGActiveStatus::NONE;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    EngineOnStatus mEngineOnStatus = EngineOnStatus::OFF;
    HevEngineOperStatus mHevEngineOperStatus = HevEngineOperStatus::ENGINE_STOP;
    EngineOilPressureWarnCANStatus mEngineOilPressureWarnCANStatus = EngineOilPressureWarnCANStatus::OFF;
    EngineOilPressureWarnHWStatus mEngineOilPressureWarnHWStatus = EngineOilPressureWarnHWStatus::OFF;
    EngineOilLevelWarnStatus mEngineOilLevelWarnStatus = EngineOilLevelWarnStatus::OFF;
    EngineOilLifeWarnStatus mEngineOilLifeWarnStatus = EngineOilLifeWarnStatus::OFF;
    ssfs::SFCTimer<Engine_Oil_Warning> mOneShotTimerISGStat3000msStatus;
    ssfs::SFCTimer<Engine_Oil_Warning> mOneShotTimerOilPressureWarnCANON500msStatus;
    ssfs::SFCTimer<Engine_Oil_Warning> mOneShotTimerOilPressureWarnCANON3000msStatus;
    ssfs::SFCTimer<Engine_Oil_Warning> mOneShotTimerOilPressureWarnHWON500msStatus;
    ssfs::SFCTimer<Engine_Oil_Warning> mOneShotTimerOilPressureWarnHWON3000msStatus;
    HBool mIsISGStatusChangedfromOnToOffWithin3000ms = false;
    HBool mIsOilPressureWarnCANOnAfter500ms = false;
    HBool mIsOilPressureWarnCANOnAfter3000ms = false;
    HBool mIsOilPressureWarnHWOnAfter500ms = false;
    HBool mIsOilPressureWarnHWOnAfter3000ms = false;
};

}  // namespace ccos

#endif  // SFSS_Engine_Oil_Warning_H
