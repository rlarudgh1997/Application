/**
 * @file Aux_Battery_Warning_CV.hpp
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
#ifndef SFSS_Aux_Battery_Warning_CV_H
#define SFSS_Aux_Battery_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Aux_Battery_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 4.0.0
// Spec Version : v0.21
// Reference : [CV080] Aux_Battery_Warning_CV. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class Aux_Battery_Warning_CV : public Aux_Battery_Warning_CVBase {
public:
    Aux_Battery_Warning_CV() = default;
    ~Aux_Battery_Warning_CV() override = default;
    Aux_Battery_Warning_CV(const Aux_Battery_Warning_CV& other) = delete;
    Aux_Battery_Warning_CV(Aux_Battery_Warning_CV&& other) noexcept = delete;
    Aux_Battery_Warning_CV& operator=(const Aux_Battery_Warning_CV& other) = delete;
    Aux_Battery_Warning_CV& operator=(Aux_Battery_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAux_Battery_Warning_CVEventBEQOperatingStatusID("E70801");
        setSFCAux_Battery_Warning_CVEventBEQOperatingStatusLinkedSoundID("SND_PopUpWarn2");
        setSFCAux_Battery_Warning_CVEventBEQOperatingStatusLinkedSoundType(
            SFCAux_Battery_Warning_CVEventBEQOperatingStatusLinkedSoundType::REPEAT_COUNT);
        setSFCAux_Battery_Warning_CVEventBEQOperatingStatusLinkedSoundRepeatCount(1);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCID("E70802");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCLinkedSoundID("SND_PopUpWarn1");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCLinkedSoundType(
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCLinkedSoundType::REPEAT_COUNT);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCLinkedSoundRepeatCount(1);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCUID("E70803");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCULinkedSoundID("SND_PopUpWarn1");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCULinkedSoundType(
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCULinkedSoundType::REPEAT_COUNT);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCULinkedSoundRepeatCount(1);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2ID("E70804");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2LinkedSoundID("SND_PopUpWarn1");
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2LinkedSoundType(
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2LinkedSoundType::REPEAT_COUNT);
        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2LinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // Implement handler code here
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAll();
    }

    void onBeqOperatingStatusChanged(const ArgumentsBeqOperatingStatusChanged& args) {
        mBEQOperatingStatus = args.mInput_BEQOperatingStatus;
        updateTelltaleBEQOperWarnStat();
        updateTelltaleBatChgStat();
        updateTelltaleServiceLamp();
        updateEventBEQOperatingStat();
    }

    void onAuxBatteryWarnReqChanged(const ArgumentsAuxBatteryWarnReqChanged& args) {
        mAuxBatteryWarnReqFromCVLDC = args.mInput_AuxBatteryWarnReqFromCVLDC;
        mAuxBatteryWarnReqFromEMS = args.mInput_AuxBatteryWarnReqFromEMS;
        mAuxBatteryWarnReqFromICCU = args.mInput_AuxBatteryWarnReqFromICCU;
        updateTelltaleBatChgStat();
    }

    void onServiceIndiReqStatusFromLDCChanged(const ArgumentsServiceIndiReqStatusFromLDCChanged& args) {
        mServiceIndiReqStatusFromLDC = args.mInput_ServiceIndiReqStatusFromLDC;
        updateTelltaleServiceLamp();
        updateEventServiceIndiReqStatusFromLDC();
    }

    void onServiceIndiReqStatusFromICCUChanged(const ArgumentsServiceIndiReqStatusFromICCUChanged& args) {
        mServiceIndiReqStatusFromICCU = args.mInput_ServiceIndiReqStatusFromICCU;
        updateTelltaleServiceLamp();
        updateEventmServiceIndiReqStatusFromICCU();
    }

    void onServiceIndiReqStatusFromICCU2Changed(const ArgumentsServiceIndiReqStatusFromICCU2Changed& args) {
        mServiceIndiReqStatusFromICCU2 = args.mInput_ServiceIndiReqStatusFromICCU2;
        updateEventServiceIndiReqStatusFromICCU2();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateTelltaleBatChgStat();
        updateTelltaleServiceLamp();
    }

    void onConfigICCUChanged(const ArgumentsConfigICCUChanged& args) {
        mConfigICCU = args.mInter_ConfigICCU;
        updateTelltaleBEQOperWarnStat();
        updateTelltaleBatChgStat();
        updateTelltaleServiceLamp();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateTelltaleBEQOperWarnStat();
        updateTelltaleBatChgStat();
        updateTelltaleServiceLamp();
        updateEventBEQOperatingStat();
        updateEventServiceIndiReqStatusFromLDC();
        updateEventmServiceIndiReqStatusFromICCU();
        updateEventServiceIndiReqStatusFromICCU2();
    }

    void updateTelltaleBEQOperWarnStat() {
        SFCAux_Battery_Warning_CVTelltaleBEQOperWarnStatusStat stat = SFCAux_Battery_Warning_CVTelltaleBEQOperWarnStatusStat::OFF;

        if (mIsIgnOn == true && mConfigICCU == ConfigICCU::OFF &&
            (mBEQOperatingStatus == BEQOperatingStatus::WARNING || mBEQOperatingStatus == BEQOperatingStatus::MESSAGE_TIMEOUT)) {
            stat = SFCAux_Battery_Warning_CVTelltaleBEQOperWarnStatusStat::ON;
        }

        setSFCAux_Battery_Warning_CVTelltaleBEQOperWarnStatusStat(stat);
    }

    void updateTelltaleBatChgStat() {
        SFCAux_Battery_Warning_CVTelltaleBatChgStat stat = SFCAux_Battery_Warning_CVTelltaleBatChgStat::OFF;

        if (mIsIgnOn == true) {
            if (((mBEQOperatingStatus == BEQOperatingStatus::WARNING ||
                  mBEQOperatingStatus == BEQOperatingStatus::MESSAGE_TIMEOUT) &&
                 (mConfigVehicleType == ConfigVehicleType::ICV || mConfigVehicleType == ConfigVehicleType::EV ||
                  mConfigVehicleType == ConfigVehicleType::FCEV) &&
                 mConfigICCU == ConfigICCU::OFF) ||
                ((mAuxBatteryWarnReqFromCVLDC == AuxBatteryWarnReqFromCVLDC::ON ||
                  mAuxBatteryWarnReqFromCVLDC == AuxBatteryWarnReqFromCVLDC::MESSAGE_TIMEOUT) &&
                 (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV)) ||
                (mAuxBatteryWarnReqFromEMS == AuxBatteryWarnReqFromEMS::ON && mConfigVehicleType == ConfigVehicleType::ICV) ||
                (mAuxBatteryWarnReqFromICCU == AuxBatteryWarnReqFromICCU::ON && mConfigVehicleType == ConfigVehicleType::EV &&
                 mConfigICCU == ConfigICCU::ON)) {
                stat = SFCAux_Battery_Warning_CVTelltaleBatChgStat::ON;
            }
        }

        setSFCAux_Battery_Warning_CVTelltaleBatChgStat(stat);
    }

    void updateTelltaleServiceLamp() {
        SFCAux_Battery_Warning_CVTelltaleSrvLmpxEVStat stat = SFCAux_Battery_Warning_CVTelltaleSrvLmpxEVStat::OFF;

        if (mIsIgnOn == true) {
            if (((mBEQOperatingStatus == BEQOperatingStatus::WARNING ||
                  mBEQOperatingStatus == BEQOperatingStatus::MESSAGE_TIMEOUT) &&
                 (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV) &&
                 mConfigICCU == ConfigICCU::OFF) ||
                (mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON &&
                 (mConfigVehicleType == ConfigVehicleType::EV || mConfigVehicleType == ConfigVehicleType::FCEV)) ||
                (mConfigVehicleType == ConfigVehicleType::EV &&
                 mServiceIndiReqStatusFromICCU == ServiceIndiReqStatusFromICCU::ON && mConfigICCU == ConfigICCU::ON)) {
                stat = SFCAux_Battery_Warning_CVTelltaleSrvLmpxEVStat::ON;
            }
        }

        setSFCAux_Battery_Warning_CVTelltaleSrvLmpxEVStat(stat);
    }

    void updateEventBEQOperatingStat() {
        SFCAux_Battery_Warning_CVEventBEQOperatingStatusStat stat = SFCAux_Battery_Warning_CVEventBEQOperatingStatusStat::OFF;

        if (mIsIgnOn == true && mBEQOperatingStatus == BEQOperatingStatus::WARNING) {
            stat = SFCAux_Battery_Warning_CVEventBEQOperatingStatusStat::ON;
        }

        setSFCAux_Battery_Warning_CVEventBEQOperatingStatusStat(stat);
    }

    void updateEventServiceIndiReqStatusFromLDC() {
        SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCStat stat =
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCStat::OFF;

        if (mIsIgnOn == true && mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON) {
            stat = SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCStat::ON;
        }

        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromLDCStat(stat);
    }

    void updateEventmServiceIndiReqStatusFromICCU() {
        SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCUStat stat =
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCUStat::OFF;

        if (mIsIgnOn == true && mServiceIndiReqStatusFromICCU == ServiceIndiReqStatusFromICCU::ON) {
            stat = SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCUStat::ON;
        }

        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCUStat(stat);
    }

    void updateEventServiceIndiReqStatusFromICCU2() {
        SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2Stat stat =
            SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2Stat::OFF;

        if (mIsIgnOn == true && mServiceIndiReqStatusFromICCU2 == ServiceIndiReqStatusFromICCU2::ON) {
            stat = SFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2Stat::ON;
        }

        setSFCAux_Battery_Warning_CVEventServiceIndiReqStatusFromICCU2Stat(stat);
    }

    using ConfigICCU = ArgumentsConfigICCUChanged::Inter_ConfigICCU;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using BEQOperatingStatus = ArgumentsBeqOperatingStatusChanged::Input_BEQOperatingStatus;
    using AuxBatteryWarnReqFromCVLDC = ArgumentsAuxBatteryWarnReqChanged::Input_AuxBatteryWarnReqFromCVLDC;
    using AuxBatteryWarnReqFromEMS = ArgumentsAuxBatteryWarnReqChanged::Input_AuxBatteryWarnReqFromEMS;
    using AuxBatteryWarnReqFromICCU = ArgumentsAuxBatteryWarnReqChanged::Input_AuxBatteryWarnReqFromICCU;
    using ServiceIndiReqStatusFromLDC = ArgumentsServiceIndiReqStatusFromLDCChanged::Input_ServiceIndiReqStatusFromLDC;
    using ServiceIndiReqStatusFromICCU = ArgumentsServiceIndiReqStatusFromICCUChanged::Input_ServiceIndiReqStatusFromICCU;
    using ServiceIndiReqStatusFromICCU2 = ArgumentsServiceIndiReqStatusFromICCU2Changed::Input_ServiceIndiReqStatusFromICCU2;

    HBool mIsIgnOn = false;

    ConfigICCU mConfigICCU = ConfigICCU::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    BEQOperatingStatus mBEQOperatingStatus = BEQOperatingStatus::NORMAL;
    AuxBatteryWarnReqFromCVLDC mAuxBatteryWarnReqFromCVLDC = AuxBatteryWarnReqFromCVLDC::OFF;
    AuxBatteryWarnReqFromEMS mAuxBatteryWarnReqFromEMS = AuxBatteryWarnReqFromEMS::OFF;
    AuxBatteryWarnReqFromICCU mAuxBatteryWarnReqFromICCU = AuxBatteryWarnReqFromICCU::OFF;
    ServiceIndiReqStatusFromLDC mServiceIndiReqStatusFromLDC = ServiceIndiReqStatusFromLDC::OFF;
    ServiceIndiReqStatusFromICCU mServiceIndiReqStatusFromICCU = ServiceIndiReqStatusFromICCU::OFF;
    ServiceIndiReqStatusFromICCU2 mServiceIndiReqStatusFromICCU2 = ServiceIndiReqStatusFromICCU2::OFF;
};

}  // namespace ccos

#endif  // SFSS_Aux_Battery_Warning_CV_H
