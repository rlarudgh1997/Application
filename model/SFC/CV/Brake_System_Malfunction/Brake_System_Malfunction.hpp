/**
 * @file Brake_System_Malfunction.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Brake_System_Malfunction_H
#define SFSS_Brake_System_Malfunction_H

#define DLOG_ENABLED gEnableSFCLog

#include "Brake_System_MalfunctionBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.26
// Reference : [CV810] Brake_System_Malfunction. Contains : Telltale. Recommend: VALUE_CHANGED
class Brake_System_Malfunction : public Brake_System_MalfunctionBase {
public:
    Brake_System_Malfunction() = default;
    ~Brake_System_Malfunction() override = default;
    Brake_System_Malfunction(const Brake_System_Malfunction& other) = delete;
    Brake_System_Malfunction(Brake_System_Malfunction&& other) noexcept = delete;
    Brake_System_Malfunction& operator=(const Brake_System_Malfunction& other) = delete;
    Brake_System_Malfunction& operator=(Brake_System_Malfunction&& other) noexcept = delete;

    void onInitialize() override {
        setSFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampBlinkValueA(
            SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampBlinkValueA::ON);
        setSFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampBlinkValueB(
            SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltaleBrakeMalfSymbolLamp();
        updateTelltaleBrakeMalfTextLamp();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltaleBrakeMalfSymbolLamp();
        updateTelltaleBrakeMalfTextLamp();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltaleBrakeMalfSymbolLamp();
        updateTelltaleBrakeMalfTextLamp();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltaleBrakeMalfSymbolLamp();
        updateTelltaleBrakeMalfTextLamp();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onConfigBrakeAirTypeChanged(const ArgumentsConfigBrakeAirTypeChanged& args) {
        mConfigBrakeAirType = args.mInter_ConfigBrakeAirType;
        updateTelltaleBrakeMalfSymbolLamp();
    }

    void onBrakeMalfunctionSymbolChanged(const ArgumentsBrakeMalfunctionSymbolChanged& args) {
        mConfigEBS = args.mInter_ConfigEBS;
        mConfigEPB = args.mInter_ConfigEPB;
        mEBDLampStatus = args.mInput_EBDLampStatus;
        mBrakeAirStatus = args.mInput_BrakeAirStatus;
        mBrakeOilStatus = args.mInput_BrakeOilStatus;
        mEPBWarningStatus = args.mInput_EPBWarningStatus;
        mEPBRedStopStatus = args.mInput_EPBRedStopStatus;
        updateTelltaleBrakeMalfSymbolLamp();
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleRetarderMalLampStatChanged(const ArgumentsTelltaleRetarderMalLampStatChanged& args) {
        mTelltaleRetarderMalLampStat = args.mRetarderMalLampStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleABSTrailerLampStatChanged(const ArgumentsTelltaleABSTrailerLampStatChanged& args) {
        mTelltaleABSTrailerLampStat = args.mABSTrailerLampStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleParking_Brake_CVStatChanged(const ArgumentsTelltaleParking_Brake_CVStatChanged& args) {
        mTelltaleParking_Brake_CVStat = args.mParking_Brake_CVStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleEBSAmberLampStatChanged(const ArgumentsTelltaleEBSAmberLampStatChanged& args) {
        mTelltaleEBSAmberLampStat = args.mEBSAmberLampStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleEBSRedLampStatChanged(const ArgumentsTelltaleEBSRedLampStatChanged& args) {
        mTelltaleEBSRedLampStat = args.mEBSRedLampStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleABS_CVStatChanged(const ArgumentsTelltaleABS_CVStatChanged& args) {
        mTelltaleABS_CVStat = args.mABS_CVStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void onTelltaleLWSStatChanged(const ArgumentsTelltaleLWSStatChanged& args) {
        mTelltaleLWSStat = args.mLWSStat;
        updateTelltaleBrakeMalfTextLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleBrakeMalfSymbolLamp() {
        SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat stat =
            SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::ON;
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            if (mBrakeAirStatus == BrakeAirStatus::LOW || mBrakeOilStatus == BrakeOilStatus::LOW) {
                stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::ON;
            } else {
                if ((mConfigEBS == ConfigEBS::ABS || mConfigEBS == ConfigEBS::ABSNASR || mConfigEBS == ConfigEBS::ABSNVDC) &&
                    mConfigBrakeAirType == ConfigBrakeAirType::OFF &&
                    (mEBDLampStatus == EBDLampStatus::ON || mEBDLampStatus == EBDLampStatus::MESSAGE_TIMEOUT)) {
                    stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::ON;
                } else if (((mConfigEBS == ConfigEBS::ABS || mConfigEBS == ConfigEBS::ABSNASR ||
                             mConfigEBS == ConfigEBS::ABSNVDC) &&
                            ((mConfigBrakeAirType == ConfigBrakeAirType::OFF && mEBDLampStatus == EBDLampStatus::OFF) ||
                             (mConfigBrakeAirType == ConfigBrakeAirType::TYPE_1 ||
                              mConfigBrakeAirType == ConfigBrakeAirType::TYPE_2)) &&
                            mBrakeAirStatus == BrakeAirStatus::NORMAL && mBrakeOilStatus == BrakeOilStatus::NORMAL &&
                            mConfigEPB == ConfigEPB::ON &&
                            ((mEPBWarningStatus == EPBWarningStatus::WARNING ||
                              mEPBWarningStatus == EPBWarningStatus::MESSAGE_TIMEOUT) ||
                             (mEPBRedStopStatus == EPBRedStopStatus::ON ||
                              mEPBRedStopStatus == EPBRedStopStatus::MESSAGE_TIMEOUT))) ||
                           (mConfigEBS != ConfigEBS::ABS && mConfigEBS != ConfigEBS::ABSNASR &&
                            mConfigEBS != ConfigEBS::ABSNVDC && mBrakeAirStatus == BrakeAirStatus::NORMAL &&
                            mBrakeOilStatus == BrakeOilStatus::NORMAL && mConfigEPB == ConfigEPB::ON &&
                            ((mEPBWarningStatus == EPBWarningStatus::WARNING ||
                              mEPBWarningStatus == EPBWarningStatus::MESSAGE_TIMEOUT) ||
                             (mEPBRedStopStatus == EPBRedStopStatus::ON ||
                              mEPBRedStopStatus == EPBRedStopStatus::MESSAGE_TIMEOUT)))) {
                    stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::BLINK1;
                } else {
                    // defualt OFF
                }
            }
        } else {
            // defualt OFF
        }
        mTelltaleBrakeMalfSysmbolLampstat = stat;
        setSFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat(stat);
    }

    void updateTelltaleBrakeMalfTextLamp() {
        SFCBrake_System_MalfunctionTelltaleBrakeMalfTextLampStat stat =
            SFCBrake_System_MalfunctionTelltaleBrakeMalfTextLampStat::OFF;

        if (mConfigArea == ConfigArea::USA) {
            if (mIgnElapsed == IgnElapsed::ON_500ms) {
                stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfTextLampStat::ON;
            } else if (mIgnElapsed == IgnElapsed::ON_3500ms &&
                       (mTelltaleBrakeMalfSysmbolLampstat == SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::ON ||
                        mTelltaleRetarderMalLampStat == TelltaleRetarderMalLampStat::ON ||
                        mTelltaleABSTrailerLampStat == TelltaleABSTrailerLampStat::ON ||
                        mTelltaleParking_Brake_CVStat == TelltaleParking_Brake_CVStat::ON ||
                        mTelltaleEBSAmberLampStat == TelltaleEBSAmberLampStat::ON ||
                        mTelltaleEBSRedLampStat == TelltaleEBSRedLampStat::ON || mTelltaleABS_CVStat == TelltaleABS_CVStat::ON ||
                        mTelltaleLWSStat == TelltaleLWSStat::ON)) {
                stat = SFCBrake_System_MalfunctionTelltaleBrakeMalfTextLampStat::ON;
            } else {
                // defualt OFF
            }
        }
        setSFCBrake_System_MalfunctionTelltaleBrakeMalfTextLampStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        MAX
    };

    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigBrakeAirType = ArgumentsConfigBrakeAirTypeChanged::Inter_ConfigBrakeAirType;
    using ConfigEBS = ArgumentsBrakeMalfunctionSymbolChanged::Inter_ConfigEBS;
    using ConfigEPB = ArgumentsBrakeMalfunctionSymbolChanged::Inter_ConfigEPB;
    using BrakeAirStatus = ArgumentsBrakeMalfunctionSymbolChanged::Input_BrakeAirStatus;
    using BrakeOilStatus = ArgumentsBrakeMalfunctionSymbolChanged::Input_BrakeOilStatus;
    using EPBWarningStatus = ArgumentsBrakeMalfunctionSymbolChanged::Input_EPBWarningStatus;
    using EPBRedStopStatus = ArgumentsBrakeMalfunctionSymbolChanged::Input_EPBRedStopStatus;
    using EBDLampStatus = ArgumentsBrakeMalfunctionSymbolChanged::Input_EBDLampStatus;
    using TelltaleRetarderMalLampStat = ArgumentsTelltaleRetarderMalLampStatChanged::RetarderMalLampStat;
    using TelltaleABSTrailerLampStat = ArgumentsTelltaleABSTrailerLampStatChanged::ABSTrailerLampStat;
    using TelltaleParking_Brake_CVStat = ArgumentsTelltaleParking_Brake_CVStatChanged::Parking_Brake_CVStat;
    using TelltaleEBSAmberLampStat = ArgumentsTelltaleEBSAmberLampStatChanged::EBSAmberLampStat;
    using TelltaleEBSRedLampStat = ArgumentsTelltaleEBSRedLampStatChanged::EBSRedLampStat;
    using TelltaleABS_CVStat = ArgumentsTelltaleABS_CVStatChanged::ABS_CVStat;
    using TelltaleLWSStat = ArgumentsTelltaleLWSStatChanged::LWSStat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigBrakeAirType mConfigBrakeAirType = ConfigBrakeAirType::OFF;
    ConfigEBS mConfigEBS = ConfigEBS::EBSNVDC;
    ConfigEPB mConfigEPB = ConfigEPB::OFF;
    BrakeAirStatus mBrakeAirStatus = BrakeAirStatus::NORMAL;
    BrakeOilStatus mBrakeOilStatus = BrakeOilStatus::NORMAL;
    EPBWarningStatus mEPBWarningStatus = EPBWarningStatus::NORMAL;
    EPBRedStopStatus mEPBRedStopStatus = EPBRedStopStatus::OFF;
    EBDLampStatus mEBDLampStatus = EBDLampStatus::OFF;
    TelltaleRetarderMalLampStat mTelltaleRetarderMalLampStat = TelltaleRetarderMalLampStat::OFF;
    TelltaleABSTrailerLampStat mTelltaleABSTrailerLampStat = TelltaleABSTrailerLampStat::OFF;
    TelltaleParking_Brake_CVStat mTelltaleParking_Brake_CVStat = TelltaleParking_Brake_CVStat::OFF;
    TelltaleEBSAmberLampStat mTelltaleEBSAmberLampStat = TelltaleEBSAmberLampStat::OFF;
    TelltaleEBSRedLampStat mTelltaleEBSRedLampStat = TelltaleEBSRedLampStat::OFF;
    TelltaleABS_CVStat mTelltaleABS_CVStat = TelltaleABS_CVStat::OFF;
    TelltaleLWSStat mTelltaleLWSStat = TelltaleLWSStat::OFF;
    SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat mTelltaleBrakeMalfSysmbolLampstat =
        SFCBrake_System_MalfunctionTelltaleBrakeMalfSysmbolLampStat::OFF;
};

}  // namespace ccos

#endif  // SFSS_Brake_System_Malfunction_H
