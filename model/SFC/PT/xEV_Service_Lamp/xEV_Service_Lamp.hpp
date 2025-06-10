/**
 * @file xEV_Service_Lamp.hpp
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
#ifndef SFSS_xEV_Service_Lamp_H
#define SFSS_xEV_Service_Lamp_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_Service_LampBase.hpp"

namespace ccos {

// SFC Version : 8.0.0
// Reference : [PT200] xEV_Service_Lamp. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class xEV_Service_Lamp : public xEV_Service_LampBase {
public:
    xEV_Service_Lamp() = default;
    ~xEV_Service_Lamp() override = default;
    xEV_Service_Lamp(const xEV_Service_Lamp& other) = delete;
    xEV_Service_Lamp(xEV_Service_Lamp&& other) noexcept = delete;
    xEV_Service_Lamp& operator=(const xEV_Service_Lamp& other) = delete;
    xEV_Service_Lamp& operator=(xEV_Service_Lamp&& other) noexcept = delete;

    void onInitialize() override {
        setSFCXEV_Service_LampEventSrvlampWarnEvID("E21501");
        setSFCXEV_Service_LampEventSrvlampWarnEvLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_LampEventSrvlampWarnEvLinkedSoundType(
            SFCXEV_Service_LampEventSrvlampWarnEvLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_LampEventSrvlampWarnEvLinkedSoundRepeatCount(1);

        setSFCXEV_Service_LampEventSrvlampWarnHevID("E21502");
        setSFCXEV_Service_LampEventSrvlampWarnHevLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_LampEventSrvlampWarnHevLinkedSoundType(
            SFCXEV_Service_LampEventSrvlampWarnHevLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_LampEventSrvlampWarnHevLinkedSoundRepeatCount(1);

        setSFCXEV_Service_LampEventSrvlampWarnChgFcevID("E21503");
        setSFCXEV_Service_LampEventSrvlampWarnChgFcevLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_LampEventSrvlampWarnChgFcevLinkedSoundType(
            SFCXEV_Service_LampEventSrvlampWarnChgFcevLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_LampEventSrvlampWarnChgFcevLinkedSoundRepeatCount(1);

        setSFCXEV_Service_LampEventSrvlampWarnErevID("E21504");
        setSFCXEV_Service_LampEventSrvlampWarnErevLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_LampEventSrvlampWarnErevLinkedSoundType(
            SFCXEV_Service_LampEventSrvlampWarnErevLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_LampEventSrvlampWarnErevLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateAll();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateAll();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateAll();
    }

    void onSrvlampWarnChanged(const ArgumentsSrvlampWarnChanged& args) {
        mServiceIndiReqStatusFromHCU = args.mInput_ServiceIndiReqStatusFromHCU;
        mServiceIndiReqStatusFromMCU = args.mInput_ServiceIndiReqStatusFromMCU;
        mServiceIndiReqStatusFromMCU2 = args.mInput_ServiceIndiReqStatusFromMCU2;
        mServiceIndiReqStatusFromBMS = args.mInput_ServiceIndiReqStatusFromBMS;
        mServiceIndiReqStatusFromLDC = args.mInput_ServiceIndiReqStatusFromLDC;
        mServiceIndiReqStatusFromALDC = args.mInput_ServiceIndiReqStatusFromALDC;
        mServiceIndiReqStatusFromTCU = args.mInput_ServiceIndiReqStatusFromTCU;
        mServiceIndiReqStatusFromDATC = args.mInput_ServiceIndiReqStatusFromDATC;
        mServiceIndiReqStatusFromVCU = args.mInput_ServiceIndiReqStatusFromVCU;
        mServiceIndiReqStatusFromEVSCU = args.mInput_ServiceIndiReqStatusFromEVSCU;
        mServiceIndiReqStatusFromWCCU = args.mInput_ServiceIndiReqStatusFromWCCU;
        mServiceIndiReqStatusFromBHDC = args.mInput_ServiceIndiReqStatusFromBHDC;
        mServiceIndiReqStatusFromFACU = args.mInput_ServiceIndiReqStatusFromFACU;
        mServiceIndiReqStatusFromFCU = args.mInput_ServiceIndiReqStatusFromFCU;
        mServiceIndiReqStatusFromHMU = args.mInput_ServiceIndiReqStatusFromHMU;
        mServiceIndiReqStatusFromFSVM = args.mInput_ServiceIndiReqStatusFromFSVM;
        updatePrivate_SrvlampWarnEv();
        updatePrivate_SrvlampWarnHev();
        updatePrivate_SrvlampWarnChgFcev();
        updatePrivate_SrvlampWarnErev();
        updateTelltaleSrvLmpEv();
        updateTelltaleSrvLmpHev();
        updateTelltaleSrvLmpFcev();
        updateTelltaleSrvLmpErev();
        updateEvent();
    }

    void onSrvlampWarnChgChanged(const ArgumentsSrvlampWarnChgChanged& args) {
        mServiceIndiReqStatusFromOBC = args.mInput_ServiceIndiReqStatusFromOBC;
        mServiceIndiReqStatusFromVCMS = args.mInput_ServiceIndiReqStatusFromVCMS;
        updatePrivate_SrvlampWarnChgEv();
        updatePrivate_SrvlampWarnChgPhev();
        updateTelltaleSrvLmpEv();
        updateTelltaleSrvLmpHev();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updatePrivate_SrvlampWarnEv();
        updatePrivate_SrvlampWarnChgEv();
        updatePrivate_SrvlampWarnHev();
        updatePrivate_SrvlampWarnChgPhev();
        updatePrivate_SrvlampWarnChgFcev();
        updatePrivate_SrvlampWarnErev();
        updateTelltaleSrvLmpEv();
        updateTelltaleSrvLmpHev();
        updateTelltaleSrvLmpFcev();
        updateTelltaleSrvLmpErev();
        updateEvent();
    }

    void updatePrivate_SrvlampWarnEv() {
        if (mConfigVehicleType == ConfigVehicleType::EV && (mServiceIndiReqStatusFromVCU == ServiceIndiReqStatusFromVCU::ON ||
                                                            mServiceIndiReqStatusFromBMS == ServiceIndiReqStatusFromBMS::ON ||
                                                            mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON ||
                                                            mServiceIndiReqStatusFromALDC == ServiceIndiReqStatusFromALDC::ON ||
                                                            mServiceIndiReqStatusFromDATC == ServiceIndiReqStatusFromDATC::ON ||
                                                            mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON ||
                                                            mServiceIndiReqStatusFromMCU2 == ServiceIndiReqStatusFromMCU2::ON ||
                                                            mServiceIndiReqStatusFromEVSCU == ServiceIndiReqStatusFromEVSCU::ON ||
                                                            mServiceIndiReqStatusFromWCCU == ServiceIndiReqStatusFromWCCU::ON)) {
            mPrivate_SrvlampWarnEv = Private_SrvlampWarnEv::ON;
        } else {
            mPrivate_SrvlampWarnEv = Private_SrvlampWarnEv::OFF;
        }
    }

    void updatePrivate_SrvlampWarnChgEv() {
        if (mConfigVehicleType == ConfigVehicleType::EV && (mServiceIndiReqStatusFromOBC == ServiceIndiReqStatusFromOBC::ON ||
                                                            mServiceIndiReqStatusFromVCMS == ServiceIndiReqStatusFromVCMS::ON)) {
            mPrivate_SrvlampWarnChgEv = Private_SrvlampWarnChgEv::ON;
        } else {
            mPrivate_SrvlampWarnChgEv = Private_SrvlampWarnChgEv::OFF;
        }
    }

    void updatePrivate_SrvlampWarnHev() {
        if ((mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV) &&
            (mServiceIndiReqStatusFromHCU == ServiceIndiReqStatusFromHCU::ON ||
             mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON ||
             mServiceIndiReqStatusFromMCU2 == ServiceIndiReqStatusFromMCU2::ON ||
             mServiceIndiReqStatusFromBMS == ServiceIndiReqStatusFromBMS::ON ||
             mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON ||
             mServiceIndiReqStatusFromTCU == ServiceIndiReqStatusFromTCU::ON ||
             mServiceIndiReqStatusFromDATC == ServiceIndiReqStatusFromDATC::ON)) {
            mPrivate_SrvlampWarnHev = Private_SrvlampWarnHev::ON;
        } else {
            mPrivate_SrvlampWarnHev = Private_SrvlampWarnHev::OFF;
        }
    }

    void updatePrivate_SrvlampWarnChgFcev() {
        if (mConfigVehicleType == ConfigVehicleType::FCEV &&
            (mServiceIndiReqStatusFromBMS == ServiceIndiReqStatusFromBMS::ON ||
             mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON ||
             mServiceIndiReqStatusFromALDC == ServiceIndiReqStatusFromALDC::ON ||
             mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON ||
             mServiceIndiReqStatusFromMCU2 == ServiceIndiReqStatusFromMCU2::ON ||
             mServiceIndiReqStatusFromDATC == ServiceIndiReqStatusFromDATC::ON ||
             mServiceIndiReqStatusFromBHDC == ServiceIndiReqStatusFromBHDC::ON ||
             mServiceIndiReqStatusFromFACU == ServiceIndiReqStatusFromFACU::ON ||
             mServiceIndiReqStatusFromFCU == ServiceIndiReqStatusFromFCU::ON ||
             mServiceIndiReqStatusFromHMU == ServiceIndiReqStatusFromHMU::ON ||
             mServiceIndiReqStatusFromFSVM == ServiceIndiReqStatusFromFSVM::ON)) {
            mPrivate_SrvlampWarnChgFcev = Private_SrvlampWarnChgFcev::ON;
        } else {
            mPrivate_SrvlampWarnChgFcev = Private_SrvlampWarnChgFcev::OFF;
        }
    }

    void updatePrivate_SrvlampWarnErev() {
        if (mConfigVehicleType == ConfigVehicleType::EREV &&
            (mServiceIndiReqStatusFromVCU == ServiceIndiReqStatusFromVCU::ON ||
             mServiceIndiReqStatusFromBMS == ServiceIndiReqStatusFromBMS::ON ||
             mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON ||
             mServiceIndiReqStatusFromALDC == ServiceIndiReqStatusFromALDC::ON ||
             mServiceIndiReqStatusFromDATC == ServiceIndiReqStatusFromDATC::ON ||
             mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON ||
             mServiceIndiReqStatusFromMCU2 == ServiceIndiReqStatusFromMCU2::ON ||
             mServiceIndiReqStatusFromEVSCU == ServiceIndiReqStatusFromEVSCU::ON ||
             mServiceIndiReqStatusFromWCCU == ServiceIndiReqStatusFromWCCU::ON)) {
            mPrivate_SrvlampWarnErev = Private_SrvlampWarnErev::ON;
        } else {
            mPrivate_SrvlampWarnErev = Private_SrvlampWarnErev::OFF;
        }
    }

    void updatePrivate_SrvlampWarnChgPhev() {
        if (mConfigVehicleType == ConfigVehicleType::PHEV && mServiceIndiReqStatusFromOBC == ServiceIndiReqStatusFromOBC::ON) {
            mPrivate_SrvlampWarnChgPhev = Private_SrvlampWarnChgPhev::ON;
        } else {
            mPrivate_SrvlampWarnChgPhev = Private_SrvlampWarnChgPhev::OFF;
        }
    }

    void updateTelltaleSrvLmpEv() {
        SFCXEV_Service_LampTelltaleSrvLmpEvStat telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpEvStat::OFF;
        if ((mIgnElapsed == IgnElapsed::ON_500ms && mConfigVehicleType == ConfigVehicleType::EV) ||
            (mIgnElapsed == IgnElapsed::ON_3500ms && mPrivate_SrvlampWarnEv == Private_SrvlampWarnEv::ON) ||
            mPrivate_SrvlampWarnChgEv == Private_SrvlampWarnChgEv::ON) {
            telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpEvStat::ON;
        }
        setSFCXEV_Service_LampTelltaleSrvLmpEvStat(telltaleStat);
    }

    void updateTelltaleSrvLmpHev() {
        SFCXEV_Service_LampTelltaleSrvLmpHevStat telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpHevStat::OFF;
        if (((mConfigVehicleType == ConfigVehicleType::HEV || mConfigVehicleType == ConfigVehicleType::PHEV) &&
             mIgnElapsed == IgnElapsed::ON_500ms) ||
            (mIgnElapsed == IgnElapsed::ON_3500ms && mPrivate_SrvlampWarnHev == Private_SrvlampWarnHev::ON) ||
            mPrivate_SrvlampWarnChgPhev == Private_SrvlampWarnChgPhev::ON) {
            telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpHevStat::ON;
        }
        setSFCXEV_Service_LampTelltaleSrvLmpHevStat(telltaleStat);
    }

    void updateTelltaleSrvLmpFcev() {
        SFCXEV_Service_LampTelltaleSrvLmpFcevStat telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpFcevStat::OFF;
        if ((mIgnElapsed == IgnElapsed::ON_500ms && mConfigVehicleType == ConfigVehicleType::FCEV) ||
            (mIgnElapsed == IgnElapsed::ON_3500ms && mPrivate_SrvlampWarnChgFcev == Private_SrvlampWarnChgFcev::ON)) {
            telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpFcevStat::ON;
        }
        setSFCXEV_Service_LampTelltaleSrvLmpFcevStat(telltaleStat);
    }

    void updateTelltaleSrvLmpErev() {
        SFCXEV_Service_LampTelltaleSrvLmpErevStat telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpErevStat::OFF;
        if ((mIgnElapsed == IgnElapsed::ON_500ms && mConfigVehicleType == ConfigVehicleType::EREV) ||
            (mIgnElapsed == IgnElapsed::ON_3500ms && mPrivate_SrvlampWarnErev == Private_SrvlampWarnErev::ON)) {
            telltaleStat = SFCXEV_Service_LampTelltaleSrvLmpErevStat::ON;
        }
        setSFCXEV_Service_LampTelltaleSrvLmpErevStat(telltaleStat);
    }

    void updateEvent() {
        updateServiceLampWarnEV();
        updateServiceLampWarnHEV();
        updateServiceLampWarnFCEV();
        updateServiceLampWarnEREV();
    }

    void updateServiceLampWarnEV() {
        SFCXEV_Service_LampEventSrvlampWarnEvStat eventStat = SFCXEV_Service_LampEventSrvlampWarnEvStat::OFF;
        if (mIsIgnOn == true && mPrivate_SrvlampWarnEv == Private_SrvlampWarnEv::ON) {
            eventStat = SFCXEV_Service_LampEventSrvlampWarnEvStat::ON;
        }
        setSFCXEV_Service_LampEventSrvlampWarnEvStat(eventStat);
    }

    void updateServiceLampWarnHEV() {
        SFCXEV_Service_LampEventSrvlampWarnHevStat eventStat = SFCXEV_Service_LampEventSrvlampWarnHevStat::OFF;
        if (mIsIgnOn == true && mPrivate_SrvlampWarnHev == Private_SrvlampWarnHev::ON) {
            eventStat = SFCXEV_Service_LampEventSrvlampWarnHevStat::ON;
        }
        setSFCXEV_Service_LampEventSrvlampWarnHevStat(eventStat);
    }
    void updateServiceLampWarnFCEV() {
        SFCXEV_Service_LampEventSrvlampWarnChgFcevStat eventStat = SFCXEV_Service_LampEventSrvlampWarnChgFcevStat::OFF;
        if (mIsIgnOn == true && mPrivate_SrvlampWarnChgFcev == Private_SrvlampWarnChgFcev::ON) {
            eventStat = SFCXEV_Service_LampEventSrvlampWarnChgFcevStat::ON;
        }
        setSFCXEV_Service_LampEventSrvlampWarnChgFcevStat(eventStat);
    }

    void updateServiceLampWarnEREV() {
        SFCXEV_Service_LampEventSrvlampWarnErevStat eventStat = SFCXEV_Service_LampEventSrvlampWarnErevStat::OFF;
        if (mIsIgnOn == true && mPrivate_SrvlampWarnErev == Private_SrvlampWarnErev::ON) {
            eventStat = SFCXEV_Service_LampEventSrvlampWarnErevStat::ON;
        }
        setSFCXEV_Service_LampEventSrvlampWarnErevStat(eventStat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    enum class Private_EnumOnOff : HUInt64 {
        NONE,
        OFF,
        ON,
        MAX
    };

    using Private_SrvlampWarnEv = Private_EnumOnOff;
    using Private_SrvlampWarnChgEv = Private_EnumOnOff;
    using Private_SrvlampWarnHev = Private_EnumOnOff;
    using Private_SrvlampWarnChgPhev = Private_EnumOnOff;
    using Private_SrvlampWarnChgFcev = Private_EnumOnOff;
    using Private_SrvlampWarnErev = Private_EnumOnOff;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ServiceIndiReqStatusFromHCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromHCU;
    using ServiceIndiReqStatusFromMCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromMCU;
    using ServiceIndiReqStatusFromMCU2 = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromMCU2;
    using ServiceIndiReqStatusFromBMS = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromBMS;
    using ServiceIndiReqStatusFromLDC = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromLDC;
    using ServiceIndiReqStatusFromALDC = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromALDC;
    using ServiceIndiReqStatusFromTCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromTCU;
    using ServiceIndiReqStatusFromDATC = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromDATC;
    using ServiceIndiReqStatusFromVCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromVCU;
    using ServiceIndiReqStatusFromEVSCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromEVSCU;
    using ServiceIndiReqStatusFromOBC = ArgumentsSrvlampWarnChgChanged::Input_ServiceIndiReqStatusFromOBC;
    using ServiceIndiReqStatusFromVCMS = ArgumentsSrvlampWarnChgChanged::Input_ServiceIndiReqStatusFromVCMS;
    using ServiceIndiReqStatusFromWCCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromWCCU;
    using ServiceIndiReqStatusFromBHDC = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromBHDC;
    using ServiceIndiReqStatusFromFACU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromFACU;
    using ServiceIndiReqStatusFromFCU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromFCU;
    using ServiceIndiReqStatusFromHMU = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromHMU;
    using ServiceIndiReqStatusFromFSVM = ArgumentsSrvlampWarnChanged::Input_ServiceIndiReqStatusFromFSVM;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ServiceIndiReqStatusFromHCU mServiceIndiReqStatusFromHCU = ServiceIndiReqStatusFromHCU::OFF;
    ServiceIndiReqStatusFromMCU mServiceIndiReqStatusFromMCU = ServiceIndiReqStatusFromMCU::OFF;
    ServiceIndiReqStatusFromMCU2 mServiceIndiReqStatusFromMCU2 = ServiceIndiReqStatusFromMCU2::OFF;
    ServiceIndiReqStatusFromBMS mServiceIndiReqStatusFromBMS = ServiceIndiReqStatusFromBMS::OFF;
    ServiceIndiReqStatusFromLDC mServiceIndiReqStatusFromLDC = ServiceIndiReqStatusFromLDC::OFF;
    ServiceIndiReqStatusFromALDC mServiceIndiReqStatusFromALDC = ServiceIndiReqStatusFromALDC::OFF;
    ServiceIndiReqStatusFromTCU mServiceIndiReqStatusFromTCU = ServiceIndiReqStatusFromTCU::OFF;
    ServiceIndiReqStatusFromDATC mServiceIndiReqStatusFromDATC = ServiceIndiReqStatusFromDATC::OFF;
    ServiceIndiReqStatusFromVCU mServiceIndiReqStatusFromVCU = ServiceIndiReqStatusFromVCU::OFF;
    ServiceIndiReqStatusFromEVSCU mServiceIndiReqStatusFromEVSCU = ServiceIndiReqStatusFromEVSCU::OFF;
    ServiceIndiReqStatusFromOBC mServiceIndiReqStatusFromOBC = ServiceIndiReqStatusFromOBC::OFF;
    ServiceIndiReqStatusFromVCMS mServiceIndiReqStatusFromVCMS = ServiceIndiReqStatusFromVCMS::OFF;
    ServiceIndiReqStatusFromWCCU mServiceIndiReqStatusFromWCCU = ServiceIndiReqStatusFromWCCU::OFF;
    ServiceIndiReqStatusFromBHDC mServiceIndiReqStatusFromBHDC = ServiceIndiReqStatusFromBHDC::OFF;
    ServiceIndiReqStatusFromFACU mServiceIndiReqStatusFromFACU = ServiceIndiReqStatusFromFACU::OFF;
    ServiceIndiReqStatusFromFCU mServiceIndiReqStatusFromFCU = ServiceIndiReqStatusFromFCU::OFF;
    ServiceIndiReqStatusFromHMU mServiceIndiReqStatusFromHMU = ServiceIndiReqStatusFromHMU::OFF;
    ServiceIndiReqStatusFromFSVM mServiceIndiReqStatusFromFSVM = ServiceIndiReqStatusFromFSVM::OFF;
    HBool mIsIgnOn = false;
    Private_SrvlampWarnEv mPrivate_SrvlampWarnEv = Private_SrvlampWarnEv::OFF;
    Private_SrvlampWarnChgEv mPrivate_SrvlampWarnChgEv = Private_SrvlampWarnChgEv::OFF;
    Private_SrvlampWarnHev mPrivate_SrvlampWarnHev = Private_SrvlampWarnHev::OFF;
    Private_SrvlampWarnChgPhev mPrivate_SrvlampWarnChgPhev = Private_SrvlampWarnChgPhev::OFF;
    Private_SrvlampWarnChgFcev mPrivate_SrvlampWarnChgFcev = Private_SrvlampWarnChgFcev::OFF;
    Private_SrvlampWarnErev mPrivate_SrvlampWarnErev = Private_SrvlampWarnErev::OFF;
};

}  // namespace ccos

#endif  // SFSS_xEV_Service_Lamp_H
