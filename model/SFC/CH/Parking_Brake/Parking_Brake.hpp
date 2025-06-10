/**
 * @file Parking_Brake.hpp
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
#ifndef SFSS_Parking_Brake_H
#define SFSS_Parking_Brake_H

#define DLOG_ENABLED gEnableSFCLog

#include "Parking_BrakeBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Reference : [CH130] Parking_Brake. Contains : Telltale. Recommend: VALUE_CHANGED
class Parking_Brake : public Parking_BrakeBase {
public:
    Parking_Brake() = default;
    ~Parking_Brake() override = default;
    Parking_Brake(const Parking_Brake& other) = delete;
    Parking_Brake(Parking_Brake&& other) noexcept = delete;
    Parking_Brake& operator=(const Parking_Brake& other) = delete;
    Parking_Brake& operator=(Parking_Brake&& other) noexcept = delete;

    void onInitialize() override {
        setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigABS = args.mInter_ConfigABS;
        mConfigEPB = args.mInter_ConfigEPB;
        mConfigESC = args.mInter_ConfigESC;
        mConfigIEB = args.mInter_ConfigIEB;
        mConfigEBB = args.mInter_ConfigEBB;
        updateTelltale();
    }

    void onTelltaleSignalChanged(const ArgumentsTelltaleSignalChanged& args) {
        if (ISTIMEOUT(args.mInput_BrakeFluidWarnStatus) == true) {
            mBrakeFluidWarnStatus = BrakeFluidWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mBrakeFluidWarnStatus = args.mInput_BrakeFluidWarnStatus;
        }
        if (ISTIMEOUT(args.mInput_EbdWarnStatus) == true) {
            mEbdWarnStatus = EbdWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mEbdWarnStatus = args.mInput_EbdWarnStatus;
        }
        mEpbFastenedStatus = args.mInput_EpbFastenedStatus;
        if (ISTIMEOUT(args.mInput_IebWarnStatus) == true) {
            mIebWarnStatus = IebWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mIebWarnStatus = args.mInput_IebWarnStatus;
        }
        mParkBrakeSwitchStatus = args.mInput_ParkBrakeSwitchStatus;
        if (ISTIMEOUT(args.mInput_EbbWarnStatus) == true) {
            mEbbWarnStatus = EbbWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mEbbWarnStatus = args.mInput_EbbWarnStatus;
        }
        updateTelltale();
    }

private:
    void updateBatteryOnTelltaleStatus() {
        if (mConfigEPB == ConfigEPB::ON && mEpbFastenedStatus == EpbFastenedStatus::APPLY) {
            mTelltaleOutputStatus = TelltaleOutputStatus::ON;
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
        } else if (mConfigEPB == ConfigEPB::ON && mEpbFastenedStatus == EpbFastenedStatus::DYNAMIC_BRAKING) {
            mTelltaleOutputStatus = TelltaleOutputStatus::BLINKING;
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::BLINK2);
            setSFCParking_BrakeTelltaleParkingBrakeBlinkValueA(SFCParking_BrakeTelltaleParkingBrakeBlinkValueA::ON);
            setSFCParking_BrakeTelltaleParkingBrakeBlinkValueB(SFCParking_BrakeTelltaleParkingBrakeBlinkValueB::OFF);
        } else {
            mTelltaleOutputStatus = TelltaleOutputStatus::OFF;
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::OFF);
        }
    }

    void updateAfter3500msTelltale() {
        if (mTelltaleOutputStatus == TelltaleOutputStatus::OFF) {
            if ((mEbdWarnStatus == EbdWarnStatus::ON || mEbdWarnStatus == EbdWarnStatus::UNHANDLED_TIMEOUT) &&
                (mConfigABS == ConfigABS::ON || mConfigESC == ConfigESC::ON)) {
                setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
            } else if (mParkBrakeSwitchStatus == ParkBrakeSwitchStatus::PARK) {
                setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
            } else if (mBrakeFluidWarnStatus == BrakeFluidWarnStatus::ON ||
                       mBrakeFluidWarnStatus == BrakeFluidWarnStatus::UNHANDLED_TIMEOUT) {
                setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
            } else if (mConfigIEB == ConfigIEB::ON &&
                       (mIebWarnStatus == IebWarnStatus::ON || mIebWarnStatus == IebWarnStatus::UNHANDLED_TIMEOUT)) {
                setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
            } else if (mConfigEBB == ConfigEBB::ON &&
                       (mEbbWarnStatus == EbbWarnStatus::ON || mEbbWarnStatus == EbbWarnStatus::UNHANDLED_TIMEOUT)) {
                setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
            } else {
                // OFF
            }
        }
    }

    void updateTelltale() {
        if (mTelltaleOutputStatus == TelltaleOutputStatus::OFF && mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::OFF);
        } else if (mTelltaleOutputStatus == TelltaleOutputStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
        } else if (mIgnElapsed == IgnElapsed::ON_500ms) {
            setSFCParking_BrakeTelltaleParkingBrakeStat(SFCParking_BrakeTelltaleParkingBrakeStat::ON);
        } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
            updateBatteryOnTelltaleStatus();
            updateAfter3500msTelltale();
        } else {
            updateBatteryOnTelltaleStatus();
        }
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    enum class TelltaleOutputStatus : HUInt64 {
        ON,
        BLINKING,
        OFF
    };

    using ConfigABS = ArgumentsConfigChanged::Inter_ConfigABS;
    using ConfigEPB = ArgumentsConfigChanged::Inter_ConfigEPB;
    using ConfigESC = ArgumentsConfigChanged::Inter_ConfigESC;
    using ConfigIEB = ArgumentsConfigChanged::Inter_ConfigIEB;
    using ConfigEBB = ArgumentsConfigChanged::Inter_ConfigEBB;
    using BrakeFluidWarnStatus = ArgumentsTelltaleSignalChanged::Input_BrakeFluidWarnStatus;
    using EbdWarnStatus = ArgumentsTelltaleSignalChanged::Input_EbdWarnStatus;
    using EpbFastenedStatus = ArgumentsTelltaleSignalChanged::Input_EpbFastenedStatus;
    using IebWarnStatus = ArgumentsTelltaleSignalChanged::Input_IebWarnStatus;
    using ParkBrakeSwitchStatus = ArgumentsTelltaleSignalChanged::Input_ParkBrakeSwitchStatus;
    using EbbWarnStatus = ArgumentsTelltaleSignalChanged::Input_EbbWarnStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigABS mConfigABS = ConfigABS::OFF;
    ConfigEPB mConfigEPB = ConfigEPB::OFF;
    ConfigESC mConfigESC = ConfigESC::OFF;
    ConfigIEB mConfigIEB = ConfigIEB::OFF;
    ConfigEBB mConfigEBB = ConfigEBB::OFF;
    BrakeFluidWarnStatus mBrakeFluidWarnStatus = BrakeFluidWarnStatus::OFF;
    EbdWarnStatus mEbdWarnStatus = EbdWarnStatus::OFF;
    EpbFastenedStatus mEpbFastenedStatus = EpbFastenedStatus::RELEASE;
    IebWarnStatus mIebWarnStatus = IebWarnStatus::OFF;
    ParkBrakeSwitchStatus mParkBrakeSwitchStatus = ParkBrakeSwitchStatus::UNPARK;
    EbbWarnStatus mEbbWarnStatus = EbbWarnStatus::OFF;
    TelltaleOutputStatus mTelltaleOutputStatus = TelltaleOutputStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Parking_Brake_H
