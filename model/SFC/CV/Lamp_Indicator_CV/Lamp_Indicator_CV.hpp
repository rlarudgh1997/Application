/**
 * @file Lamp_Indicator_CV.hpp
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
#ifndef SFSS_Lamp_Indicator_CV_H
#define SFSS_Lamp_Indicator_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Lamp_Indicator_CVBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.11
// Reference : [CV840] Lamp_Indicator_CV. Contains : Telltale. Recommend: VALUE_CHANGED
class Lamp_Indicator_CV : public Lamp_Indicator_CVBase {
public:
    Lamp_Indicator_CV() = default;
    ~Lamp_Indicator_CV() override = default;
    Lamp_Indicator_CV(const Lamp_Indicator_CV& other) = delete;
    Lamp_Indicator_CV(Lamp_Indicator_CV&& other) noexcept = delete;
    Lamp_Indicator_CV& operator=(const Lamp_Indicator_CV& other) = delete;
    Lamp_Indicator_CV& operator=(Lamp_Indicator_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLamp_Indicator_CVSoundTickID("SND_Tick");
        setSFCLamp_Indicator_CVSoundTickType(SFCLamp_Indicator_CVSoundTickType::REPEAT_COUNT);
        setSFCLamp_Indicator_CVSoundTickRepeatCount(1);
        setSFCLamp_Indicator_CVSoundTockID("SND_Tock");
        setSFCLamp_Indicator_CVSoundTockType(SFCLamp_Indicator_CVSoundTockType::REPEAT_COUNT);
        setSFCLamp_Indicator_CVSoundTockRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleFrontFog();
        updateTelltaleRearFog();
        updateTelltaleHighBeam();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleFrontFog();
        updateTelltaleRearFog();
        updateTelltaleHighBeam();
    }

    void onFrontFogChanged(const ArgumentsFrontFogChanged& args) {
        mFrFogLmpReqStatus = args.mInput_FrFogLmpReqStatus;
        updateTelltaleFrontFog();
    }

    void onRearFogChanged(const ArgumentsRearFogChanged& args) {
        mRrFogLmpReqStatus = args.mInput_RrFogLmpReqStatus;
        updateTelltaleRearFog();
    }

    void onHighLowBeamChanged(const ArgumentsHighLowBeamChanged& args) {
        mHdLmpHiReqStatus = args.mInput_HdLmpHiReqStatus;
        mHdLmpLoReqStatus = args.mInput_HdLmpLoReqStatus;
        updateTelltaleHighBeam();
        updateTelltaleLowBeam();
    }

    void onTailLampChanged(const ArgumentsTailLampChanged& args) {
        mExtrnlTailLmpReqStatus = args.mInput_ExtrnlTailLmpReqStatus;
        updateTelltaleTailLamp();
    }

    void onTurnSignalLampChanged(const ArgumentsTurnSignalLampChanged& args) {
        mLeftTurnSignalLampStatus = args.mInput_LeftTurnSignalLampStatus;
        mRightTurnSignalLampStatus = args.mInput_RightTurnSignalLampStatus;
        updateTelltaleTurnLampAndSound();
        updateTelltaleTrailerTurnLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.1.1 Front Fog Lamp Ind
    void updateTelltaleFrontFog() {
        SFCLamp_Indicator_CVTelltaleFrontFogStat stat = SFCLamp_Indicator_CVTelltaleFrontFogStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mFrFogLmpReqStatus == FrFogLmpReqStatus::ON) {
            stat = SFCLamp_Indicator_CVTelltaleFrontFogStat::ON;
        }
        setSFCLamp_Indicator_CVTelltaleFrontFogStat(stat);
    }

    // 5.1.2 High Beam Lamp Ind
    void updateTelltaleHighBeam() {
        SFCLamp_Indicator_CVTelltaleHighBeamStat stat = SFCLamp_Indicator_CVTelltaleHighBeamStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mHdLmpHiReqStatus == HdLmpHiReqStatus::ON) {
            stat = SFCLamp_Indicator_CVTelltaleHighBeamStat::ON;
        }
        setSFCLamp_Indicator_CVTelltaleHighBeamStat(stat);
    }

    // 5.1.3 Low Beam Lamp Ind
    void updateTelltaleLowBeam() {
        SFCLamp_Indicator_CVTelltaleLowBeamStat stat = SFCLamp_Indicator_CVTelltaleLowBeamStat::OFF;
        if (mHdLmpHiReqStatus == HdLmpHiReqStatus::OFF && mHdLmpLoReqStatus == HdLmpLoReqStatus::ON) {
            stat = SFCLamp_Indicator_CVTelltaleLowBeamStat::ON;
        }
        setSFCLamp_Indicator_CVTelltaleLowBeamStat(stat);
    }

    // 5.1.4 Rear Fog Lamp Ind
    void updateTelltaleRearFog() {
        SFCLamp_Indicator_CVTelltaleRearFogStat stat = SFCLamp_Indicator_CVTelltaleRearFogStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms && mRrFogLmpReqStatus == RrFogLmpReqStatus::ON) {
            stat = SFCLamp_Indicator_CVTelltaleRearFogStat::ON;
        }
        setSFCLamp_Indicator_CVTelltaleRearFogStat(stat);
    }

    // 5.1.5 Tail Lamp Ind
    void updateTelltaleTailLamp() {
        SFCLamp_Indicator_CVTelltaleTailLampStat stat = SFCLamp_Indicator_CVTelltaleTailLampStat::OFF;
        if (mExtrnlTailLmpReqStatus == ExtrnlTailLmpReqStatus::ON) {
            stat = SFCLamp_Indicator_CVTelltaleTailLampStat::ON;
        }
        setSFCLamp_Indicator_CVTelltaleTailLampStat(stat);
    }

    // 5.1.6 Turn Signal Lamp Ind
    // 5.4 사운드 로직 / Sound Logic
    void updateTelltaleTurnLampAndSound() {
        static LeftTurnSignalLampStatus preLeftStat = LeftTurnSignalLampStatus::OFF;
        static RightTurnSignalLampStatus preRightStat = RightTurnSignalLampStatus::OFF;

        std::string soundIDLeft;
        std::string soundIDRight;
        SFCLamp_Indicator_CVTelltaleTurnLeftStat leftStat = SFCLamp_Indicator_CVTelltaleTurnLeftStat::OFF;
        SFCLamp_Indicator_CVTelltaleTurnRightStat rightStat = SFCLamp_Indicator_CVTelltaleTurnRightStat::OFF;

        if ((mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::ON) ||
            (mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::ON_WITH_TRAILER)) {
            leftStat = SFCLamp_Indicator_CVTelltaleTurnLeftStat::ON;
        } else {
            leftStat = SFCLamp_Indicator_CVTelltaleTurnLeftStat::OFF;
        }
        if ((mRightTurnSignalLampStatus == RightTurnSignalLampStatus::ON) ||
            (mRightTurnSignalLampStatus == RightTurnSignalLampStatus::ON_WITH_TRAILER)) {
            rightStat = SFCLamp_Indicator_CVTelltaleTurnRightStat::ON;
        } else {
            rightStat = SFCLamp_Indicator_CVTelltaleTurnRightStat::OFF;
        }

        if (preLeftStat == LeftTurnSignalLampStatus::OFF &&
            (mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::ON ||
             mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::ON_WITH_TRAILER)) {
            soundIDLeft = "SND_Tick";
        } else if ((preLeftStat == LeftTurnSignalLampStatus::ON || preLeftStat == LeftTurnSignalLampStatus::ON_WITH_TRAILER) &&
                   mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::OFF) {
            soundIDLeft = "SND_Tock";
        } else {
            // no operation
        }

        if (preRightStat == RightTurnSignalLampStatus::OFF &&
            (mRightTurnSignalLampStatus == RightTurnSignalLampStatus::ON ||
             mRightTurnSignalLampStatus == RightTurnSignalLampStatus::ON_WITH_TRAILER)) {
            soundIDRight = "SND_Tick";
        } else if ((preRightStat == RightTurnSignalLampStatus::ON ||
                    preRightStat == RightTurnSignalLampStatus::ON_WITH_TRAILER) &&
                   mRightTurnSignalLampStatus == RightTurnSignalLampStatus::OFF) {
            soundIDRight = "SND_Tock";
        } else {
            // no operation
        }

        preLeftStat = mLeftTurnSignalLampStatus;
        preRightStat = mRightTurnSignalLampStatus;

        setSFCLamp_Indicator_CVTelltaleTurnLeftStat(leftStat);
        setSFCLamp_Indicator_CVTelltaleTurnRightStat(rightStat);

        if (soundIDLeft == "SND_Tick" || soundIDRight == "SND_Tick") {
            setSFCLamp_Indicator_CVSoundTickStat(SFCLamp_Indicator_CVSoundTickStat::ON);
        } else {
            setSFCLamp_Indicator_CVSoundTickStat(SFCLamp_Indicator_CVSoundTickStat::OFF);
        }
        flushLastGroup(true);
        if (soundIDLeft == "SND_Tock" || soundIDRight == "SND_Tock") {
            setSFCLamp_Indicator_CVSoundTockStat(SFCLamp_Indicator_CVSoundTockStat::ON);
        } else {
            setSFCLamp_Indicator_CVSoundTockStat(SFCLamp_Indicator_CVSoundTockStat::OFF);
        }
        flushLastGroup(true);
    }

    // 5.1.7 Trailer Turn Signal Lamp Ind
    void updateTelltaleTrailerTurnLamp() {
        SFCLamp_Indicator_CVTelltaleTrailerTurnLeftStat leftStat = SFCLamp_Indicator_CVTelltaleTrailerTurnLeftStat::OFF;
        SFCLamp_Indicator_CVTelltaleTrailerTurnRightStat rightStat = SFCLamp_Indicator_CVTelltaleTrailerTurnRightStat::OFF;

        if (mLeftTurnSignalLampStatus == LeftTurnSignalLampStatus::ON_WITH_TRAILER) {
            leftStat = SFCLamp_Indicator_CVTelltaleTrailerTurnLeftStat::ON;
        } else {
            leftStat = SFCLamp_Indicator_CVTelltaleTrailerTurnLeftStat::OFF;
        }

        if (mRightTurnSignalLampStatus == RightTurnSignalLampStatus::ON_WITH_TRAILER) {
            rightStat = SFCLamp_Indicator_CVTelltaleTrailerTurnRightStat::ON;
        } else {
            rightStat = SFCLamp_Indicator_CVTelltaleTrailerTurnRightStat::OFF;
        }

        setSFCLamp_Indicator_CVTelltaleTrailerTurnLeftStat(leftStat);
        setSFCLamp_Indicator_CVTelltaleTrailerTurnRightStat(rightStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_500ms,
        MAX
    };

    using FrFogLmpReqStatus = ArgumentsFrontFogChanged::Input_FrFogLmpReqStatus;
    using RrFogLmpReqStatus = ArgumentsRearFogChanged::Input_RrFogLmpReqStatus;
    using HdLmpHiReqStatus = ArgumentsHighLowBeamChanged::Input_HdLmpHiReqStatus;
    using HdLmpLoReqStatus = ArgumentsHighLowBeamChanged::Input_HdLmpLoReqStatus;
    using ExtrnlTailLmpReqStatus = ArgumentsTailLampChanged::Input_ExtrnlTailLmpReqStatus;
    using LeftTurnSignalLampStatus = ArgumentsTurnSignalLampChanged::Input_LeftTurnSignalLampStatus;
    using RightTurnSignalLampStatus = ArgumentsTurnSignalLampChanged::Input_RightTurnSignalLampStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    FrFogLmpReqStatus mFrFogLmpReqStatus = FrFogLmpReqStatus::OFF;
    RrFogLmpReqStatus mRrFogLmpReqStatus = RrFogLmpReqStatus::OFF;
    HdLmpHiReqStatus mHdLmpHiReqStatus = HdLmpHiReqStatus::OFF;
    HdLmpLoReqStatus mHdLmpLoReqStatus = HdLmpLoReqStatus::OFF;
    ExtrnlTailLmpReqStatus mExtrnlTailLmpReqStatus = ExtrnlTailLmpReqStatus::OFF;
    LeftTurnSignalLampStatus mLeftTurnSignalLampStatus = LeftTurnSignalLampStatus::OFF;
    RightTurnSignalLampStatus mRightTurnSignalLampStatus = RightTurnSignalLampStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Lamp_Indicator_CV_H
