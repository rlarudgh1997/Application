/**
 * @file Lamp_Indicator.hpp
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
#ifndef SFSS_Lamp_Indicator_H
#define SFSS_Lamp_Indicator_H

#define DLOG_ENABLED gEnableSFCLog

#include "Lamp_IndicatorBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 3.0.0
// Reference : [EC100] Lamp Indicator. Contains : Telltale, Sound. Recommend: VALUE_CHANGED
class Lamp_Indicator : public Lamp_IndicatorBase {
public:
    Lamp_Indicator() = default;
    ~Lamp_Indicator() override = default;
    Lamp_Indicator(const Lamp_Indicator& other) = delete;
    Lamp_Indicator(Lamp_Indicator&& other) noexcept = delete;
    Lamp_Indicator& operator=(const Lamp_Indicator& other) = delete;
    Lamp_Indicator& operator=(Lamp_Indicator&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLamp_IndicatorSoundTickID("SND_Tick");
        setSFCLamp_IndicatorSoundTickType(SFCLamp_IndicatorSoundTickType::REPEAT_COUNT);
        setSFCLamp_IndicatorSoundTickRepeatCount(1);
        setSFCLamp_IndicatorSoundTockID("SND_Tock");
        setSFCLamp_IndicatorSoundTockType(SFCLamp_IndicatorSoundTockType::REPEAT_COUNT);
        setSFCLamp_IndicatorSoundTockRepeatCount(1);

        if (mOneShotTimerTailLampOnReqStatus.create(kTimerTelltaleInterval200ms, this,
                                                    &Lamp_Indicator::onOneShotTimerTailLampOnReq, false) == false) {
            DWarning() << "[Lamp_Indicator] mOneShotTimerTailLampOnReqStatus Create Failed";
        }
        if (mOneShotTimerFrontFogLampStatus.create(kTimerTelltaleInterval200ms, this, &Lamp_Indicator::onOneShotTimerFrontFog,
                                                   false) == false) {
            DWarning() << "[Lamp_Indicator] mOneShotTimerFrontFogLampStatus Create Failed";
        }
        if (mOneShotTimerLowBeamOnReqStatus.create(kTimerTelltaleInterval200ms, this, &Lamp_Indicator::onOneShotTimerLowBeamOnReq,
                                                   false) == false) {
            DWarning() << "[Lamp_Indicator] mOneShotTimerLowBeamOnReqStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updatePreWarnTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updatePreWarnTelltale();
    }

    void onTelltaleSignalChanged(const ArgumentsTelltaleSignalChanged& args) {
        mExtRearFogLampStatus = args.mInput_ExtRearFogLampStatus;
        mExtHighBeamOnReqStatus = args.mInput_ExtHighBeamOnReqStatus;
        updateTailLampOnReqTimer(mExtTailLampOnReqStatus, args.mInput_ExtTailLampOnReqStatus);
        updateFrontFogLampTimer(mExtFrontFogLampStatus, args.mInput_ExtFrontFogLampStatus);
        updateLowBeamOnReqTimer(mExtLowBeamOnReqStatus, args.mInput_ExtLowBeamOnReqStatus);
        updatePreWarnTelltale();
    }

    void onTurnChanged(const ArgumentsTurnChanged& args) {
        mExtTurnLeftLampStatus = args.mInput_ExtTurnLeftLampStatus;
        mExtTurnRightLampStatus = args.mInput_ExtTurnRightLampStatus;
        mExtTurnLeftLampStatusFromBDC = args.mInput_ExtTurnLeftLampStatusFromBDC;
        mExtTurnRightLampStatusFromBDC = args.mInput_ExtTurnRightLampStatusFromBDC;
        updateTurnTelltaleAndSound();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTurnTelltaleAndSound() {
        static ExtTurnLeftLampStatus prevLeftStat = ExtTurnLeftLampStatus::OFF;
        static ExtTurnRightLampStatus prevRightStat = ExtTurnRightLampStatus::OFF;
        static ExtTurnLeftLampStatusFromBDC prevLefgFromBDCStat = ExtTurnLeftLampStatusFromBDC::OFF;
        static ExtTurnRightLampStatusFromBDC prevRightFromBDCStat = ExtTurnRightLampStatusFromBDC::OFF;

        std::string soundIDLeft;
        std::string soundIDRight;
        SFCLamp_IndicatorTelltaleTurnSignalLeftStat leftStat;
        SFCLamp_IndicatorTelltaleTurnSignalRightStat rightStat;

        if (mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::ON ||
            mExtTurnLeftLampStatusFromBDC == ExtTurnLeftLampStatusFromBDC::ON) {
            leftStat = SFCLamp_IndicatorTelltaleTurnSignalLeftStat::ON;
        } else {
            leftStat = SFCLamp_IndicatorTelltaleTurnSignalLeftStat::OFF;
        }
        if (mExtTurnRightLampStatus == ExtTurnRightLampStatus::ON ||
            mExtTurnRightLampStatusFromBDC == ExtTurnRightLampStatusFromBDC::ON) {
            rightStat = SFCLamp_IndicatorTelltaleTurnSignalRightStat::ON;
        } else {
            rightStat = SFCLamp_IndicatorTelltaleTurnSignalRightStat::OFF;
        }

        if ((prevLeftStat != mExtTurnLeftLampStatus && mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::ON) ||
            (prevLefgFromBDCStat != mExtTurnLeftLampStatusFromBDC &&
             mExtTurnLeftLampStatusFromBDC == ExtTurnLeftLampStatusFromBDC::ON)) {
            soundIDLeft = "SND_Tick";
        } else if ((prevLeftStat != mExtTurnLeftLampStatus && mExtTurnLeftLampStatus == ExtTurnLeftLampStatus::OFF) ||
                   (prevLefgFromBDCStat != mExtTurnLeftLampStatusFromBDC &&
                    mExtTurnLeftLampStatusFromBDC == ExtTurnLeftLampStatusFromBDC::OFF)) {
            soundIDLeft = "SND_Tock";
        } else {
            // no operation
        }

        if ((prevRightStat != mExtTurnRightLampStatus && mExtTurnRightLampStatus == ExtTurnRightLampStatus::ON) ||
            (prevRightFromBDCStat != mExtTurnRightLampStatusFromBDC &&
             mExtTurnRightLampStatusFromBDC == ExtTurnRightLampStatusFromBDC::ON)) {
            soundIDRight = "SND_Tick";
        } else if ((prevRightStat != mExtTurnRightLampStatus && mExtTurnRightLampStatus == ExtTurnRightLampStatus::OFF) ||
                   (prevRightFromBDCStat != mExtTurnRightLampStatusFromBDC &&
                    mExtTurnRightLampStatusFromBDC == ExtTurnRightLampStatusFromBDC::OFF)) {
            soundIDRight = "SND_Tock";
        } else {
            // no operation
        }

        prevLeftStat = mExtTurnLeftLampStatus;
        prevRightStat = mExtTurnRightLampStatus;
        prevLefgFromBDCStat = mExtTurnLeftLampStatusFromBDC;
        prevRightFromBDCStat = mExtTurnRightLampStatusFromBDC;

        setSFCLamp_IndicatorTelltaleTurnSignalLeftStat(leftStat);
        setSFCLamp_IndicatorTelltaleTurnSignalRightStat(rightStat);

        if (soundIDLeft == "SND_Tick" || soundIDRight == "SND_Tick") {
            setSFCLamp_IndicatorSoundTickStat(SFCLamp_IndicatorSoundTickStat::ON);
        } else {
            setSFCLamp_IndicatorSoundTickStat(SFCLamp_IndicatorSoundTickStat::OFF);
        }
        flushLastGroup(true);
        if (soundIDLeft == "SND_Tock" || soundIDRight == "SND_Tock") {
            setSFCLamp_IndicatorSoundTockStat(SFCLamp_IndicatorSoundTockStat::ON);
        } else {
            setSFCLamp_IndicatorSoundTockStat(SFCLamp_IndicatorSoundTockStat::OFF);
        }
        flushLastGroup(true);
    }

    void updatePreWarnTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mExtHighBeamOnReqStatus == ExtHighBeamOnReqStatus::ON) {
                setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::OFF);
                flush();
                setSFCLamp_IndicatorTelltaleHighBeamStat(SFCLamp_IndicatorTelltaleHighBeamStat::ON);
            } else {
                setSFCLamp_IndicatorTelltaleHighBeamStat(SFCLamp_IndicatorTelltaleHighBeamStat::OFF);
                if (mExtTailLampOnReqStatus200ms == ExtTailLampOnReqStatus::ON &&
                    mExtLowBeamOnReqStatus200ms == ExtLowBeamOnReqStatus::ON) {
                    setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::ON);
                }
            }
            if (mExtRearFogLampStatus == ExtRearFogLampStatus::ON) {
                setSFCLamp_IndicatorTelltaleRearFogStat(SFCLamp_IndicatorTelltaleRearFogStat::ON);
            } else {
                setSFCLamp_IndicatorTelltaleRearFogStat(SFCLamp_IndicatorTelltaleRearFogStat::OFF);
            }
        } else {
            setSFCLamp_IndicatorTelltaleHighBeamStat(SFCLamp_IndicatorTelltaleHighBeamStat::OFF);
            setSFCLamp_IndicatorTelltaleRearFogStat(SFCLamp_IndicatorTelltaleRearFogStat::OFF);
            if (mExtTailLampOnReqStatus200ms == ExtTailLampOnReqStatus::ON &&
                mExtLowBeamOnReqStatus200ms == ExtLowBeamOnReqStatus::ON &&
                mExtHighBeamOnReqStatus != ExtHighBeamOnReqStatus::ON) {
                setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::ON);
            } else {
                setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::OFF);
            }
        }
    }

    void updateRelated200msTelltale() {
        if (mExtTailLampOnReqStatus200ms == ExtTailLampOnReqStatus::ON) {
            setSFCLamp_IndicatorTelltaleTailLampStat(SFCLamp_IndicatorTelltaleTailLampStat::ON);
        } else {
            setSFCLamp_IndicatorTelltaleTailLampStat(SFCLamp_IndicatorTelltaleTailLampStat::OFF);
        }
        if (mExtTailLampOnReqStatus200ms == ExtTailLampOnReqStatus::ON &&
            mExtFrontFogLampStatus200ms == ExtFrontFogLampStatus::ON) {
            setSFCLamp_IndicatorTelltaleFrontFogStat(SFCLamp_IndicatorTelltaleFrontFogStat::ON);
        } else {
            setSFCLamp_IndicatorTelltaleFrontFogStat(SFCLamp_IndicatorTelltaleFrontFogStat::OFF);
        }
        if (mExtTailLampOnReqStatus200ms == ExtTailLampOnReqStatus::ON && mExtHighBeamOnReqStatus != ExtHighBeamOnReqStatus::ON &&
            mExtLowBeamOnReqStatus200ms == ExtLowBeamOnReqStatus::ON) {
            setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::ON);
        } else {
            setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::OFF);
        }
    }

    void onOneShotTimerTailLampOnReq() {
        mExtTailLampOnReqStatus200ms = ExtTailLampOnReqStatus::ON;
        updateRelated200msTelltale();
        flush();
    }

    void onOneShotTimerFrontFog() {
        mExtFrontFogLampStatus200ms = ExtFrontFogLampStatus::ON;
        updateRelated200msTelltale();
        flush();
    }

    void onOneShotTimerLowBeamOnReq() {
        mExtLowBeamOnReqStatus200ms = ExtLowBeamOnReqStatus::ON;
        updateRelated200msTelltale();
        flush();
    }

    void updateTailLampOnReqTimer(ArgumentsTelltaleSignalChanged::Input_ExtTailLampOnReqStatus& currentArgs,
                                  const ArgumentsTelltaleSignalChanged::Input_ExtTailLampOnReqStatus& newArgs) {
        if (newArgs == ExtTailLampOnReqStatus::ON) {
            if (currentArgs != newArgs) {
                if (mOneShotTimerTailLampOnReqStatus.start() == false) {
                    DWarning() << "[Lamp_Indicator] mOneShotTimerTailLampOnReqStatus Start Failed";
                }
            }
        } else {
            mOneShotTimerTailLampOnReqStatus.stop();
            mExtTailLampOnReqStatus200ms = ExtTailLampOnReqStatus::OFF;
            setSFCLamp_IndicatorTelltaleTailLampStat(SFCLamp_IndicatorTelltaleTailLampStat::OFF);
            setSFCLamp_IndicatorTelltaleFrontFogStat(SFCLamp_IndicatorTelltaleFrontFogStat::OFF);
            setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::OFF);
        }
        currentArgs = newArgs;
    }

    void updateFrontFogLampTimer(ArgumentsTelltaleSignalChanged::Input_ExtFrontFogLampStatus& currentArgs,
                                 const ArgumentsTelltaleSignalChanged::Input_ExtFrontFogLampStatus& newArgs) {
        if (newArgs == ExtFrontFogLampStatus::ON) {
            if (currentArgs != newArgs) {
                if (mOneShotTimerFrontFogLampStatus.start() == false) {
                    DWarning() << "[Lamp_Indicator] mOneShotTimerFrontFogLampStatus Start Failed";
                }
            }
        } else {
            mOneShotTimerFrontFogLampStatus.stop();
            mExtFrontFogLampStatus200ms = ExtFrontFogLampStatus::OFF;
            setSFCLamp_IndicatorTelltaleFrontFogStat(SFCLamp_IndicatorTelltaleFrontFogStat::OFF);
        }
        currentArgs = newArgs;
    }

    void updateLowBeamOnReqTimer(ArgumentsTelltaleSignalChanged::Input_ExtLowBeamOnReqStatus& currentArgs,
                                 const ArgumentsTelltaleSignalChanged::Input_ExtLowBeamOnReqStatus& newArgs) {
        if (newArgs == ExtLowBeamOnReqStatus::ON) {
            if (currentArgs != newArgs) {
                if (mOneShotTimerLowBeamOnReqStatus.start() == false) {
                    DWarning() << "[Lamp_Indicator] mOneShotTimerLowBeamOnReqStatus Start Failed";
                }
            }
        } else {
            mOneShotTimerLowBeamOnReqStatus.stop();
            mExtLowBeamOnReqStatus200ms = ExtLowBeamOnReqStatus::OFF;
            setSFCLamp_IndicatorTelltaleLowBeamStat(SFCLamp_IndicatorTelltaleLowBeamStat::OFF);
        }
        currentArgs = newArgs;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    static constexpr uint32_t kTimerTelltaleInterval200ms = 200u;

    using ExtFrontFogLampStatus = ArgumentsTelltaleSignalChanged::Input_ExtFrontFogLampStatus;
    using ExtLowBeamOnReqStatus = ArgumentsTelltaleSignalChanged::Input_ExtLowBeamOnReqStatus;
    using ExtRearFogLampStatus = ArgumentsTelltaleSignalChanged::Input_ExtRearFogLampStatus;
    using ExtHighBeamOnReqStatus = ArgumentsTelltaleSignalChanged::Input_ExtHighBeamOnReqStatus;
    using ExtTurnLeftLampStatus = ArgumentsTurnChanged::Input_ExtTurnLeftLampStatus;
    using ExtTurnRightLampStatus = ArgumentsTurnChanged::Input_ExtTurnRightLampStatus;
    using ExtTurnLeftLampStatusFromBDC = ArgumentsTurnChanged::Input_ExtTurnLeftLampStatusFromBDC;
    using ExtTurnRightLampStatusFromBDC = ArgumentsTurnChanged::Input_ExtTurnRightLampStatusFromBDC;
    using ExtTailLampOnReqStatus = ArgumentsTelltaleSignalChanged::Input_ExtTailLampOnReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ExtTailLampOnReqStatus mExtTailLampOnReqStatus = ExtTailLampOnReqStatus::OFF;
    ExtTailLampOnReqStatus mExtTailLampOnReqStatus200ms = ExtTailLampOnReqStatus::OFF;
    ExtFrontFogLampStatus mExtFrontFogLampStatus = ExtFrontFogLampStatus::OFF;
    ExtFrontFogLampStatus mExtFrontFogLampStatus200ms = ExtFrontFogLampStatus::OFF;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus = ExtLowBeamOnReqStatus::OFF;
    ExtLowBeamOnReqStatus mExtLowBeamOnReqStatus200ms = ExtLowBeamOnReqStatus::OFF;
    ExtRearFogLampStatus mExtRearFogLampStatus = ExtRearFogLampStatus::OFF;
    ExtHighBeamOnReqStatus mExtHighBeamOnReqStatus = ExtHighBeamOnReqStatus::OFF;
    ExtTurnLeftLampStatus mExtTurnLeftLampStatus = ExtTurnLeftLampStatus::OFF;
    ExtTurnRightLampStatus mExtTurnRightLampStatus = ExtTurnRightLampStatus::OFF;
    ExtTurnLeftLampStatusFromBDC mExtTurnLeftLampStatusFromBDC = ExtTurnLeftLampStatusFromBDC::OFF;
    ExtTurnRightLampStatusFromBDC mExtTurnRightLampStatusFromBDC = ExtTurnRightLampStatusFromBDC::OFF;

    ssfs::SFCTimer<Lamp_Indicator> mOneShotTimerTailLampOnReqStatus;
    ssfs::SFCTimer<Lamp_Indicator> mOneShotTimerFrontFogLampStatus;
    ssfs::SFCTimer<Lamp_Indicator> mOneShotTimerLowBeamOnReqStatus;
};

}  // namespace ccos

#endif  // SFSS_Lamp_Indicator_H
