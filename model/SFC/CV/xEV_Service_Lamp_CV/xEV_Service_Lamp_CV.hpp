/**
 * @file xEV_Service_Lamp_CV.hpp
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
#ifndef SFSS_xEV_Service_Lamp_CV_H
#define SFSS_xEV_Service_Lamp_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "xEV_Service_Lamp_CVBase.hpp"

namespace ccos {

// SFC Version : 13.0.1
// Spec Version : v0.34
// Reference : [CV430] xEV_Service_Lamp_CV. Contains : Event. Recommend: VALUE_CHANGED
class xEV_Service_Lamp_CV : public xEV_Service_Lamp_CVBase {
public:
    xEV_Service_Lamp_CV() = default;
    ~xEV_Service_Lamp_CV() override = default;
    xEV_Service_Lamp_CV(const xEV_Service_Lamp_CV& other) = delete;
    xEV_Service_Lamp_CV(xEV_Service_Lamp_CV&& other) noexcept = delete;
    xEV_Service_Lamp_CV& operator=(const xEV_Service_Lamp_CV& other) = delete;
    xEV_Service_Lamp_CV& operator=(xEV_Service_Lamp_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCXEV_Service_Lamp_CVEventSrvlampWarnEvID("E74301");
        setSFCXEV_Service_Lamp_CVEventSrvlampWarnEvLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvlampWarnEvLinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvlampWarnEvLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvlampWarnEvLinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCID("E74303");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCLinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCLinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2ID("E74304");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCUID("E74305");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCULinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCULinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCULinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMUID("E74306");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMULinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMULinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMULinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1ID("E74307");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2ID("E74308");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCULinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCULinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCULinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1ID("E74311");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2ID("E74312");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvID("E74313");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvLinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvLinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvLinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvLinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1ID("E74314");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1LinkedSoundRepeatCount(1);

        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2ID("E74315");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2LinkedSoundID("SND_PopUpWarn1");
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2LinkedSoundType(
            SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2LinkedSoundType::REPEAT_COUNT);
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2LinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEventSrvLmpWarnEV();
        updateEventSrvIndiReqFromFDC();
        updateEventSrvIndiReqFromFDC2();
        updateEventSrvIndiReqFromFCU();
        updateEventSrvIndiReqFromHMU();
        updateEventSrvIndiReqFromFACU1();
        updateEventSrvIndiReqFromFACU2();
        updateEventSrvIndiReqFromMCU();
        updateEventSrvIndiReqFromFSVM1();
        updateEventSrvIndiReqFromFSVM2();
        updateEventSrvIndiReqFromAuxInv();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEventSrvLmpWarnEV();
        updateEventSrvIndiReqFromFDC();
        updateEventSrvIndiReqFromFDC2();
        updateEventSrvIndiReqFromFCU();
        updateEventSrvIndiReqFromHMU();
        updateEventSrvIndiReqFromFACU1();
        updateEventSrvIndiReqFromFACU2();
        updateEventSrvIndiReqFromMCU();
        updateEventSrvIndiReqFromFSVM1();
        updateEventSrvIndiReqFromFSVM2();
        updateEventSrvIndiReqFromAuxInv();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updatePrivateSrvlampWarnEv();
        updateEventSrvLmpWarnEV();
        updateEventSrvIndiReqFromFDC();
        updateEventSrvIndiReqFromFDC2();
        updateEventSrvIndiReqFromFCU();
        updateEventSrvIndiReqFromHMU();
        updateEventSrvIndiReqFromFACU1();
        updateEventSrvIndiReqFromFACU2();
        updateEventSrvIndiReqFromMCU();
        updateEventSrvIndiReqFromFSVM1();
        updateEventSrvIndiReqFromFSVM2();
        updateEventSrvIndiReqFromAuxInv();
        updateEventSrvIndiReqFromFCC1();
        updateEventSrvIndiReqFromFCC2();
    }

    void onConfigHMU2Changed(const ArgumentsConfigHMU2Changed& args) {
        mConfigHMU2 = args.mInter_ConfigHMU2;
        updateEventSrvIndiReqFromHMU();
    }

    void onConfigCVPRJTNameChanged(const ArgumentsConfigCVPRJTNameChanged& args) {
        mConfigCVPRJTName = args.mInter_ConfigCVPRJTName;
        updatePrivateSrvlampWarnEv();
        updateEventSrvLmpWarnEV();
        updateEventSrvIndiReqFromMCU();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updatePrivateSrvlampWarnEv();
        updateEventSrvLmpWarnEV();
    }

    void onSrvlampWarnEvFcevChanged(const ArgumentsSrvlampWarnEvFcevChanged& args) {
        mServiceIndiReqStatusFromMCU = args.mInput_ServiceIndiReqStatusFromMCU;
        mServiceIndiReqStatusFromLDC = args.mInput_ServiceIndiReqStatusFromLDC;
        mServiceIndiReqStatusFromVCU = args.mInput_ServiceIndiReqStatusFromVCU;
        mServiceIndiReqStatusFromFCC1 = args.mInput_ServiceIndiReqStatusFromFCC1;
        mServiceIndiReqStatusFromFCC2 = args.mInput_ServiceIndiReqStatusFromFCC2;
        mServiceIndiReqStatusFromAuxInv = args.mInput_ServiceIndiReqStatusFromAuxInv;
        mVCMSServiceLampReqStatus = args.mInput_VCMSServiceLampReqStatus;
        updatePrivateSrvlampWarnEv();
        updateEventSrvLmpWarnEV();
        updateEventSrvIndiReqFromMCU();
        updateEventSrvIndiReqFromAuxInv();
        updateEventSrvIndiReqFromFCC1();
        updateEventSrvIndiReqFromFCC2();
    }

    void onSrvIndiReqFromFDCChanged(const ArgumentsSrvIndiReqFromFDCChanged& args) {
        mServiceIndiReqStatusFromFDC = args.mInput_ServiceIndiReqStatusFromFDC;
        updateEventSrvIndiReqFromFDC();
    }

    void onSrvIndiReqFromFDC2Changed(const ArgumentsSrvIndiReqFromFDC2Changed& args) {
        mServiceIndiReqStatusFromFDC2 = args.mInput_ServiceIndiReqStatusFromFDC2;
        updateEventSrvIndiReqFromFDC2();
    }

    void onSrvIndiReqFromFCUChanged(const ArgumentsSrvIndiReqFromFCUChanged& args) {
        mServiceIndiReqStatusFromFCU = args.mInput_ServiceIndiReqStatusFromFCU;
        updateEventSrvIndiReqFromFCU();
    }

    void onSrvIndiReqFromFCU2Changed(const ArgumentsSrvIndiReqFromFCU2Changed& args) {
        mServiceIndiReqStatusFromFCU2 = args.mInput_ServiceIndiReqStatusFromFCU2;
        updateEventSrvIndiReqFromFCU();
    }

    void onSrvIndiReqFromHMUChanged(const ArgumentsSrvIndiReqFromHMUChanged& args) {
        mServiceIndiReqStatusFromHMU = args.mInput_ServiceIndiReqStatusFromHMU;
        updateEventSrvIndiReqFromHMU();
    }

    void onSrvIndiReqFromHMU2Changed(const ArgumentsSrvIndiReqFromHMU2Changed& args) {
        mServiceIndiReqStatusFromHMU2 = args.mInput_ServiceIndiReqStatusFromHMU2;
        updateEventSrvIndiReqFromHMU();
    }

    void onSrvIndiReqFromFACU1Changed(const ArgumentsSrvIndiReqFromFACU1Changed& args) {
        mServiceIndiReqStatusFromFACU1 = args.mInput_ServiceIndiReqStatusFromFACU1;
        updateEventSrvIndiReqFromFACU1();
    }

    void onSrvIndiReqFromFACU2Changed(const ArgumentsSrvIndiReqFromFACU2Changed& args) {
        mServiceIndiReqStatusFromFACU2 = args.mInput_ServiceIndiReqStatusFromFACU2;
        updateEventSrvIndiReqFromFACU2();
    }

    void onSrvIndiReqFromFSVM1Changed(const ArgumentsSrvIndiReqFromFSVM1Changed& args) {
        mServiceIndiReqStatusFromFSVM1 = args.mInput_ServiceIndiReqStatusFromFSVM1;
        updateEventSrvIndiReqFromFSVM1();
    }

    void onSrvIndiReqFromFSVM2Changed(const ArgumentsSrvIndiReqFromFSVM2Changed& args) {
        mServiceIndiReqStatusFromFSVM2 = args.mInput_ServiceIndiReqStatusFromFSVM2;
        updateEventSrvIndiReqFromFSVM2();
    }

    void onAmberStopLampStatus_AuxInvChanged(const ArgumentsAmberStopLampStatus_AuxInvChanged& args) {
        mAmberStopLampStatus_AuxInv = args.mInput_AmberStopLampStatus_AuxInv;
        updateEventSrvIndiReqFromAuxInv();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateSrvlampWarnEv() {
        mPrivateSrvlampWarnEv = PrivateSrvlampWarnEv::OFF;
        if (mConfigVehicleType == ConfigVehicleType::EV) {
            if (mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON ||
                mServiceIndiReqStatusFromLDC == ServiceIndiReqStatusFromLDC::ON ||
                mServiceIndiReqStatusFromVCU == ServiceIndiReqStatusFromVCU::ON ||
                mServiceIndiReqStatusFromAuxInv == ServiceIndiReqStatusFromAuxInv::ON ||
                mVCMSServiceLampReqStatus == VCMSServiceLampReqStatus::ON ||
                (mConfigCVPRJTName == ConfigCVPRJTName::GY && mConfigArea == ConfigArea::JAPAN &&
                 (mServiceIndiReqStatusFromFCC1 == ServiceIndiReqStatusFromFCC1::ON ||
                  mServiceIndiReqStatusFromFCC1 == ServiceIndiReqStatusFromFCC1::MESSAGE_TIMEOUT ||
                  mServiceIndiReqStatusFromFCC2 == ServiceIndiReqStatusFromFCC2::ON ||
                  mServiceIndiReqStatusFromFCC2 == ServiceIndiReqStatusFromFCC2::MESSAGE_TIMEOUT))) {
                mPrivateSrvlampWarnEv = PrivateSrvlampWarnEv::ON;
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateXEV_Service_Lamp_CVSrvlampWarnEvStat(
            static_cast<SFCPrivateXEV_Service_Lamp_CVSrvlampWarnEvStat>(mPrivateSrvlampWarnEv));
#endif
    }

    void updateEventSrvLmpWarnEV() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvlampWarnEvStat stat = SFCXEV_Service_Lamp_CVEventSrvlampWarnEvStat::OFF;
        if (mIsIgnOn == true && mPrivateSrvlampWarnEv == PrivateSrvlampWarnEv::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvlampWarnEvStat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvlampWarnEvStat(stat);
    }

    void updateEventSrvIndiReqFromFDC() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCStat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCStat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFDC == ServiceIndiReqStatusFromFDC::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCStat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDCStat(stat);
    }

    void updateEventSrvIndiReqFromFDC2() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2Stat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFDC2 == ServiceIndiReqStatusFromFDC2::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFDC2Stat(stat);
    }

    void updateEventSrvIndiReqFromFCU() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCUStat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCUStat::OFF;
        if (mIsIgnOn) {
            if (mServiceIndiReqStatusFromFCU == ServiceIndiReqStatusFromFCU::ON ||
                mServiceIndiReqStatusFromFCU2 == ServiceIndiReqStatusFromFCU2::ON) {
                stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCUStat::ON;
            }
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCUStat(stat);
    }

    void updateEventSrvIndiReqFromHMU() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMUStat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMUStat::OFF;
        if (mIsIgnOn == true &&
            (mServiceIndiReqStatusFromHMU == ServiceIndiReqStatusFromHMU::ON ||
             (mServiceIndiReqStatusFromHMU2 == ServiceIndiReqStatusFromHMU2::ON && mConfigHMU2 == ConfigHMU2::ON))) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMUStat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromHMUStat(stat);
    }

    void updateEventSrvIndiReqFromFACU1() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1Stat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFACU1 == ServiceIndiReqStatusFromFACU1::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU1Stat(stat);
    }

    void updateEventSrvIndiReqFromFACU2() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2Stat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFACU2 == ServiceIndiReqStatusFromFACU2::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFACU2Stat(stat);
    }

    void updateEventSrvIndiReqFromMCU() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;

        if (mIsIgnOn == true && mServiceIndiReqStatusFromMCU == ServiceIndiReqStatusFromMCU::ON) {
            if (mConfigCVPRJTName != ConfigCVPRJTName::QZ) {
                eventID = "E74310";
                linkedSoundID = "SND_PopUpWarn1";
            } else {  // mConfigCVPRJTName == ConfigCVPRJTName::QZ
                eventID = "E74316";
                linkedSoundID = "SND_PopUpWarn2";
            }
        }
        GETCACHEDVALUE(SFC.xEV_Service_Lamp_CV.Event.SrvIndiReqFromMCU.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCUStat(SFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCUStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCUID(eventID);
            setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCULinkedSoundID(linkedSoundID);
            setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCUStat(SFCXEV_Service_Lamp_CVEventSrvIndiReqFromMCUStat::ON);
        }
    }

    void updateEventSrvIndiReqFromFSVM1() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1Stat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFSVM1 == ServiceIndiReqStatusFromFSVM1::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM1Stat(stat);
    }

    void updateEventSrvIndiReqFromFSVM2() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2Stat::OFF;
        if (mIsIgnOn == true && mServiceIndiReqStatusFromFSVM2 == ServiceIndiReqStatusFromFSVM2::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFSVM2Stat(stat);
    }

    void updateEventSrvIndiReqFromAuxInv() {
        if (mConfigVehicleType != ConfigVehicleType::EV && mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvStat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvStat::OFF;
        if (mIsIgnOn == true && (mServiceIndiReqStatusFromAuxInv == ServiceIndiReqStatusFromAuxInv::ON ||
                                 mAmberStopLampStatus_AuxInv == AmberStopLampStatus_AuxInv::ON)) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvStat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromAuxInvStat(stat);
    }

    void updateEventSrvIndiReqFromFCC1() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1Stat::OFF;
        if (mServiceIndiReqStatusFromFCC1 == ServiceIndiReqStatusFromFCC1::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC1Stat(stat);
    }

    void updateEventSrvIndiReqFromFCC2() {
        if (mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }

        SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2Stat stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2Stat::OFF;
        if (mServiceIndiReqStatusFromFCC2 == ServiceIndiReqStatusFromFCC2::ON) {
            stat = SFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2Stat::ON;
        }
        setSFCXEV_Service_Lamp_CVEventSrvIndiReqFromFCC2Stat(stat);
    }

    enum class PrivateSrvlampWarnEv : HUInt64 {
        NONE,
        ON,
        OFF,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigHMU2 = ArgumentsConfigHMU2Changed::Inter_ConfigHMU2;
    using ServiceIndiReqStatusFromMCU = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromMCU;
    using ServiceIndiReqStatusFromLDC = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromLDC;
    using ServiceIndiReqStatusFromVCU = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromVCU;
    using ServiceIndiReqStatusFromFCC1 = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromFCC1;
    using ServiceIndiReqStatusFromFCC2 = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromFCC2;
    using ServiceIndiReqStatusFromAuxInv = ArgumentsSrvlampWarnEvFcevChanged::Input_ServiceIndiReqStatusFromAuxInv;
    using VCMSServiceLampReqStatus = ArgumentsSrvlampWarnEvFcevChanged::Input_VCMSServiceLampReqStatus;
    using ServiceIndiReqStatusFromFDC = ArgumentsSrvIndiReqFromFDCChanged::Input_ServiceIndiReqStatusFromFDC;
    using ServiceIndiReqStatusFromFDC2 = ArgumentsSrvIndiReqFromFDC2Changed::Input_ServiceIndiReqStatusFromFDC2;
    using ServiceIndiReqStatusFromFCU = ArgumentsSrvIndiReqFromFCUChanged::Input_ServiceIndiReqStatusFromFCU;
    using ServiceIndiReqStatusFromFCU2 = ArgumentsSrvIndiReqFromFCU2Changed::Input_ServiceIndiReqStatusFromFCU2;
    using ServiceIndiReqStatusFromHMU = ArgumentsSrvIndiReqFromHMUChanged::Input_ServiceIndiReqStatusFromHMU;
    using ServiceIndiReqStatusFromHMU2 = ArgumentsSrvIndiReqFromHMU2Changed::Input_ServiceIndiReqStatusFromHMU2;
    using ServiceIndiReqStatusFromFACU1 = ArgumentsSrvIndiReqFromFACU1Changed::Input_ServiceIndiReqStatusFromFACU1;
    using ServiceIndiReqStatusFromFACU2 = ArgumentsSrvIndiReqFromFACU2Changed::Input_ServiceIndiReqStatusFromFACU2;
    using ServiceIndiReqStatusFromFSVM1 = ArgumentsSrvIndiReqFromFSVM1Changed::Input_ServiceIndiReqStatusFromFSVM1;
    using ServiceIndiReqStatusFromFSVM2 = ArgumentsSrvIndiReqFromFSVM2Changed::Input_ServiceIndiReqStatusFromFSVM2;
    using AmberStopLampStatus_AuxInv = ArgumentsAmberStopLampStatus_AuxInvChanged::Input_AmberStopLampStatus_AuxInv;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigCVPRJTName = ArgumentsConfigCVPRJTNameChanged::Inter_ConfigCVPRJTName;

    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigHMU2 mConfigHMU2 = ConfigHMU2::OFF;
    ServiceIndiReqStatusFromMCU mServiceIndiReqStatusFromMCU = ServiceIndiReqStatusFromMCU::OFF;
    ServiceIndiReqStatusFromLDC mServiceIndiReqStatusFromLDC = ServiceIndiReqStatusFromLDC::OFF;
    ServiceIndiReqStatusFromVCU mServiceIndiReqStatusFromVCU = ServiceIndiReqStatusFromVCU::OFF;
    ServiceIndiReqStatusFromFCC1 mServiceIndiReqStatusFromFCC1 = ServiceIndiReqStatusFromFCC1::OFF;
    ServiceIndiReqStatusFromFCC2 mServiceIndiReqStatusFromFCC2 = ServiceIndiReqStatusFromFCC2::OFF;
    ServiceIndiReqStatusFromAuxInv mServiceIndiReqStatusFromAuxInv = ServiceIndiReqStatusFromAuxInv::OFF;
    VCMSServiceLampReqStatus mVCMSServiceLampReqStatus = VCMSServiceLampReqStatus::OFF;
    ServiceIndiReqStatusFromFDC mServiceIndiReqStatusFromFDC = ServiceIndiReqStatusFromFDC::OFF;
    ServiceIndiReqStatusFromFDC2 mServiceIndiReqStatusFromFDC2 = ServiceIndiReqStatusFromFDC2::OFF;
    ServiceIndiReqStatusFromFCU mServiceIndiReqStatusFromFCU = ServiceIndiReqStatusFromFCU::OFF;
    ServiceIndiReqStatusFromFCU2 mServiceIndiReqStatusFromFCU2 = ServiceIndiReqStatusFromFCU2::OFF;
    ServiceIndiReqStatusFromHMU mServiceIndiReqStatusFromHMU = ServiceIndiReqStatusFromHMU::OFF;
    ServiceIndiReqStatusFromHMU2 mServiceIndiReqStatusFromHMU2 = ServiceIndiReqStatusFromHMU2::OFF;
    ServiceIndiReqStatusFromFACU1 mServiceIndiReqStatusFromFACU1 = ServiceIndiReqStatusFromFACU1::OFF;
    ServiceIndiReqStatusFromFACU2 mServiceIndiReqStatusFromFACU2 = ServiceIndiReqStatusFromFACU2::OFF;
    ServiceIndiReqStatusFromFSVM1 mServiceIndiReqStatusFromFSVM1 = ServiceIndiReqStatusFromFSVM1::OFF;
    ServiceIndiReqStatusFromFSVM2 mServiceIndiReqStatusFromFSVM2 = ServiceIndiReqStatusFromFSVM2::OFF;
    AmberStopLampStatus_AuxInv mAmberStopLampStatus_AuxInv = AmberStopLampStatus_AuxInv::OFF;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigCVPRJTName mConfigCVPRJTName = ConfigCVPRJTName::QZ;

    PrivateSrvlampWarnEv mPrivateSrvlampWarnEv = PrivateSrvlampWarnEv::OFF;
    HBool mIsIgnOn = false;
};

}  // namespace ccos

#endif  // SFSS_xEV_Service_Lamp_CV_H
