/**
 * @file Special_Vehicle.hpp
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
#ifndef SFSS_Special_Vehicle_H
#define SFSS_Special_Vehicle_H

#define DLOG_ENABLED gEnableSFCLog

#include "Special_VehicleBase.hpp"

namespace ccos {

// SFC Version : 7.0.0
// Spec Version : v0.24
// Reference : [CV050] Special_Vehicle. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Special_Vehicle : public Special_VehicleBase {
public:
    Special_Vehicle() = default;
    ~Special_Vehicle() override = default;
    Special_Vehicle(const Special_Vehicle& other) = delete;
    Special_Vehicle(Special_Vehicle&& other) noexcept = delete;
    Special_Vehicle& operator=(const Special_Vehicle& other) = delete;
    Special_Vehicle& operator=(Special_Vehicle&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSpecial_VehicleEventUpperBodyStatWarningID("E70501");
        setSFCSpecial_VehicleEventUpperBodyStatWarningLinkedSoundID("SND_PopUpWarn1");
        setSFCSpecial_VehicleEventUpperBodyStatWarningLinkedSoundType(
            SFCSpecial_VehicleEventUpperBodyStatWarningLinkedSoundType::REPEAT_COUNT);
        setSFCSpecial_VehicleEventUpperBodyStatWarningLinkedSoundRepeatCount(1);

        setSFCSpecial_VehicleEventPTOFaultECStatusID("E70502");
        setSFCSpecial_VehicleEventPTOFaultECStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCSpecial_VehicleEventPTOFaultECStatusLinkedSoundType(
            SFCSpecial_VehicleEventPTOFaultECStatusLinkedSoundType::REPEAT_COUNT);
        setSFCSpecial_VehicleEventPTOFaultECStatusLinkedSoundRepeatCount(1);

        setSFCSpecial_VehicleEventCouplerKingPinRlsSwStatusID("E70503");
        setSFCSpecial_VehicleEventCouplerKingPinRlsSwStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCSpecial_VehicleEventCouplerKingPinRlsSwStatusLinkedSoundType(
            SFCSpecial_VehicleEventCouplerKingPinRlsSwStatusLinkedSoundType::REPEAT_COUNT);
        setSFCSpecial_VehicleEventCouplerKingPinRlsSwStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateTelltale_Liftgate();
        updateTelltale_EPTO();
        updateTelltale_TMPTO();
        updateEvent();
        updateEvent_PTOFaultECStatus();
        updateEvent_CouplerKingPinRlsSwStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateTelltale_Liftgate();
        updateTelltale_EPTO();
        updateTelltale_TMPTO();
        updateEvent();
        updateEvent_PTOFaultECStatus();
        updateEvent_CouplerKingPinRlsSwStatus();
    }

    void onConfigLiftgateChanged(const ArgumentsConfigLiftgateChanged& args) {
        mConfigLiftgate = args.mInter_ConfigLiftgate;
        updateTelltale_Liftgate();
        updateEvent();
    }

    void onUpperBodyStatusChanged(const ArgumentsUpperBodyStatusChanged& args) {
        mUpperBodyStatus = args.mInput_UpperBodyStatus;
        updateTelltale_Liftgate();
        updateEvent();
    }

    void onEptoStatusChanged(const ArgumentsEptoStatusChanged& args) {
        mEPTOStatus = args.mInput_EPTOStatus;
        updateTelltale_EPTO();
    }

    void onTmptoStatusChanged(const ArgumentsTmptoStatusChanged& args) {
        mTMPTOStatus = args.mInput_TMPTOStatus;
        updateTelltale_TMPTO();
    }

    void onPtoFaultECStatusChanged(const ArgumentsPtoFaultECStatusChanged& args) {
        mPTOFaultECStatus = args.mInput_PTOFaultECStatus;
        updateEvent_PTOFaultECStatus();
    }

    void onCouplerKingPinRlsSwStatusChanged(const ArgumentsCouplerKingPinRlsSwStatusChanged& args) {
        mCouplerKingPinRlsSwStatus = args.mInput_CouplerKingPinRlsSwStatus;
        updateEvent_CouplerKingPinRlsSwStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale_Liftgate() {
        SFCSpecial_VehicleTelltaleLiftgateStat stat = SFCSpecial_VehicleTelltaleLiftgateStat::OFF;

        if (mIsIgnOn == true && mConfigLiftgate == ConfigLiftgate::APPLY && mUpperBodyStatus == UpperBodyStatus::ON) {
            stat = SFCSpecial_VehicleTelltaleLiftgateStat::ON;
        }

        setSFCSpecial_VehicleTelltaleLiftgateStat(stat);
    }

    void updateTelltale_EPTO() {
        SFCSpecial_VehicleTelltaleEPTOStat stat = SFCSpecial_VehicleTelltaleEPTOStat::OFF;

        if (mIsIgnOn == true) {
            if (mEPTOStatus == EPTOStatus::GREEN) {
                stat = SFCSpecial_VehicleTelltaleEPTOStat::GREEN;
            } else if (mEPTOStatus == EPTOStatus::RED) {
                stat = SFCSpecial_VehicleTelltaleEPTOStat::RED;
            } else {
                // no operation
            }
        }

        setSFCSpecial_VehicleTelltaleEPTOStat(stat);
    }

    void updateTelltale_TMPTO() {
        SFCSpecial_VehicleTelltaleTMPTOStat stat = SFCSpecial_VehicleTelltaleTMPTOStat::OFF;

        if (mTMPTOStatus == TMPTOStatus::ON && mIsIgnOn == true) {
            stat = SFCSpecial_VehicleTelltaleTMPTOStat::ON;
        }

        setSFCSpecial_VehicleTelltaleTMPTOStat(stat);
    }

    void updateEvent() {
        SFCSpecial_VehicleEventUpperBodyStatWarningStat stat = SFCSpecial_VehicleEventUpperBodyStatWarningStat::OFF;

        if (mIsIgnOn == true && mConfigLiftgate == ConfigLiftgate::NOT_APPLY && mUpperBodyStatus == UpperBodyStatus::ON) {
            stat = SFCSpecial_VehicleEventUpperBodyStatWarningStat::ON;
        }
        setSFCSpecial_VehicleEventUpperBodyStatWarningStat(stat);
    }

    void updateEvent_PTOFaultECStatus() {
        SFCSpecial_VehicleEventPTOFaultECStatusStat stat = SFCSpecial_VehicleEventPTOFaultECStatusStat::OFF;

        if (mIsIgnOn == true && mPTOFaultECStatus == PTOFaultECStatus::ON) {
            stat = SFCSpecial_VehicleEventPTOFaultECStatusStat::ON;
        }
        setSFCSpecial_VehicleEventPTOFaultECStatusStat(stat);
    }

    void updateEvent_CouplerKingPinRlsSwStatus() {
        SFCSpecial_VehicleEventCouplerKingPinRlsSwStatusStat stat = SFCSpecial_VehicleEventCouplerKingPinRlsSwStatusStat::OFF;

        if (mIsIgnOn == true && mCouplerKingPinRlsSwStatus == CouplerKingPinRlsSwStatus::ON) {
            stat = SFCSpecial_VehicleEventCouplerKingPinRlsSwStatusStat::ON;
        }
        setSFCSpecial_VehicleEventCouplerKingPinRlsSwStatusStat(stat);
    }

    HBool mIsIgnOn = false;

    using ConfigLiftgate = ArgumentsConfigLiftgateChanged::Inter_ConfigLiftgate;
    using UpperBodyStatus = ArgumentsUpperBodyStatusChanged::Input_UpperBodyStatus;
    using EPTOStatus = ArgumentsEptoStatusChanged::Input_EPTOStatus;
    using TMPTOStatus = ArgumentsTmptoStatusChanged::Input_TMPTOStatus;
    using PTOFaultECStatus = ArgumentsPtoFaultECStatusChanged::Input_PTOFaultECStatus;
    using CouplerKingPinRlsSwStatus = ArgumentsCouplerKingPinRlsSwStatusChanged::Input_CouplerKingPinRlsSwStatus;

    ConfigLiftgate mConfigLiftgate = ConfigLiftgate::NOT_APPLY;
    UpperBodyStatus mUpperBodyStatus = UpperBodyStatus::OFF;
    EPTOStatus mEPTOStatus = EPTOStatus::OFF;
    TMPTOStatus mTMPTOStatus = TMPTOStatus::OFF;
    PTOFaultECStatus mPTOFaultECStatus = PTOFaultECStatus::OFF;
    CouplerKingPinRlsSwStatus mCouplerKingPinRlsSwStatus = CouplerKingPinRlsSwStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Special_Vehicle_H
