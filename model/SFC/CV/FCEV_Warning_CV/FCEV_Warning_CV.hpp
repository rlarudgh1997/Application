/**
 * @file FCEV_Warning_CV.hpp
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
#ifndef SFSS_FCEV_Warning_CV_H
#define SFSS_FCEV_Warning_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "FCEV_Warning_CVBase.hpp"

namespace ccos {

// SFC Version : 7.0.1
// Spec Version : v0.34
// Reference : [CV450] FCEV_Warning_CV. Contains : Event. Recommend: VALUE_CHANGED
class FCEV_Warning_CV : public FCEV_Warning_CVBase {
public:
    FCEV_Warning_CV() = default;
    ~FCEV_Warning_CV() override = default;
    FCEV_Warning_CV(const FCEV_Warning_CV& other) = delete;
    FCEV_Warning_CV(FCEV_Warning_CV&& other) noexcept = delete;
    FCEV_Warning_CV& operator=(const FCEV_Warning_CV& other) = delete;
    FCEV_Warning_CV& operator=(FCEV_Warning_CV&& other) noexcept = delete;

    void onInitialize() override {
        setSFCFCEV_Warning_CVEventH2SensorFaultID("E74501");
        setSFCFCEV_Warning_CVEventH2SensorFaultLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Warning_CVEventH2SensorFaultLinkedSoundType(SFCFCEV_Warning_CVEventH2SensorFaultLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Warning_CVEventH2SensorFaultLinkedSoundRepeatCount(1);
        setSFCFCEV_Warning_CVEventH2LeakOffID("E74502");
        setSFCFCEV_Warning_CVEventH2LeakOffLinkedSoundID("SND_PopUpWarn1");
        setSFCFCEV_Warning_CVEventH2LeakOffLinkedSoundType(SFCFCEV_Warning_CVEventH2LeakOffLinkedSoundType::REPEAT_COUNT);
        setSFCFCEV_Warning_CVEventH2LeakOffLinkedSoundRepeatCount(1);
        setSFCFCEV_Warning_CVEventH2LeakSD_REQID("E74503");
        setSFCFCEV_Warning_CVEventH2LeakSD_REQLinkedSoundID("SND_PopUpWarn2");
        setSFCFCEV_Warning_CVEventH2LeakSD_REQLinkedSoundType(SFCFCEV_Warning_CVEventH2LeakSD_REQLinkedSoundType::INFINITE);
        setSFCFCEV_Warning_CVEventH2LeakPERoomLinkedSoundType(SFCFCEV_Warning_CVEventH2LeakPERoomLinkedSoundType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateAll();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateAll();
    }

    void onH2SensorFaultStatusFromFCUChanged(const ArgumentsH2SensorFaultStatusFromFCUChanged& args) {
        mH2SensorFaultStatusFromFCU = args.mInput_H2SensorFaultStatusFromFCU;
        mH2SensorFaultStatusFromFCU2 = args.mInput_H2SensorFaultStatusFromFCU2;
        updateEventH2SensorFault();
    }

    void onH2LeakWarnStatusFromFCUChanged(const ArgumentsH2LeakWarnStatusFromFCUChanged& args) {
        mH2LeakWarnStatusFromFCU = args.mInput_H2LeakWarnStatusFromFCU;
        mH2LeakWarnStatusFromFCU2 = args.mInput_H2LeakWarnStatusFromFCU2;
        updateEventH2LeakOff();
        updateEventH2LeakSDREQ();
    }

    void onH2SensorFaultStatusFromHMUChanged(const ArgumentsH2SensorFaultStatusFromHMUChanged& args) {
        mH2SensorFaultStatusFromHMU = args.mInput_H2SensorFaultStatusFromHMU;
        updateEventH2SensorFault();
    }

    void onH2LeakWarnStatusFromHMUChanged(const ArgumentsH2LeakWarnStatusFromHMUChanged& args) {
        mH2LeakWarnStatusFromHMU = args.mInput_H2LeakWarnStatusFromHMU;
        updateEventH2LeakOff();
        updateEventH2LeakSDREQ();
    }

    void onH2LeakIndiReqStatusFromHMUChanged(const ArgumentsH2LeakIndiReqStatusFromHMUChanged& args) {
        mH2LeakIndiReqStatusFromHMU = args.mInput_H2LeakIndiReqStatusFromHMU;
        updateEventH2LeakOff();
        updateEventH2LeakSDREQ();
    }

    void onFcevWaterReleaseWarnStatusChanged(const ArgumentsFcevWaterReleaseWarnStatusChanged& args) {
        mFcevWaterReleaseWarnStatus_FCU1 = args.mInput_FcevWaterReleaseWarnStatus_FCU1;
        mFcevWaterReleaseWarnStatus_FCU2 = args.mInput_FcevWaterReleaseWarnStatus_FCU2;
        updateEventWaterReleaseWarn();
    }

    void onH2LeakWarnLv1StatusFromVCUChanged(const ArgumentsH2LeakWarnLv1StatusFromVCUChanged& args) {
        mH2LeakWarnLv1StatusFromVCU = args.mInput_H2LeakWarnLv1StatusFromVCU;
        updateEventH2LeakPERoom();
    }

    void onH2LeakWarnLv2StatusFromVCUChanged(const ArgumentsH2LeakWarnLv2StatusFromVCUChanged& args) {
        mH2LeakWarnLv2StatusFromVCU = args.mInput_H2LeakWarnLv2StatusFromVCU;
        updateEventH2LeakPERoom();
    }

    void onH2SensorFaultStatusFromVCUChanged(const ArgumentsH2SensorFaultStatusFromVCUChanged& args) {
        mH2SensorFaultStatusFromVCU = args.mInput_H2SensorFaultStatusFromVCU;
        updateEventH2SensorFault();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateEventH2SensorFault();
        updateEventH2LeakOff();
        updateEventH2LeakSDREQ();
        updateEventH2LeakPERoom();
        updateEventWaterReleaseWarn();
    }

    void updateEventH2SensorFault() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCFCEV_Warning_CVEventH2SensorFaultStat stat = SFCFCEV_Warning_CVEventH2SensorFaultStat::OFF;
        if (mIsIgnOn == true) {
            if (mH2SensorFaultStatusFromFCU == H2SensorFaultStatusFromFCU::FAULT ||
                mH2SensorFaultStatusFromFCU2 == H2SensorFaultStatusFromFCU2::FAULT ||
                mH2SensorFaultStatusFromHMU == H2SensorFaultStatusFromHMU::FAULT ||
                mH2SensorFaultStatusFromVCU == H2SensorFaultStatusFromVCU::FAULT) {
                stat = SFCFCEV_Warning_CVEventH2SensorFaultStat::ON;
            }
        }
        setSFCFCEV_Warning_CVEventH2SensorFaultStat(stat);
    }

    void updateEventH2LeakOff() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCFCEV_Warning_CVEventH2LeakOffStat stat = SFCFCEV_Warning_CVEventH2LeakOffStat::OFF;
        if (mIsIgnOn == true) {
            if (mH2LeakWarnStatusFromFCU == H2LeakWarnStatusFromFCU::LEAK_DETECT ||
                mH2LeakWarnStatusFromFCU2 == H2LeakWarnStatusFromFCU2::LEAK_DETECT ||
                ((mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::LEAK &&
                  mH2LeakWarnStatusFromHMU == H2LeakWarnStatusFromHMU::OFF))) {
                stat = SFCFCEV_Warning_CVEventH2LeakOffStat::ON;
            }
        }
        setSFCFCEV_Warning_CVEventH2LeakOffStat(stat);
    }

    void updateEventH2LeakSDREQ() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }
        SFCFCEV_Warning_CVEventH2LeakSD_REQStat stat = SFCFCEV_Warning_CVEventH2LeakSD_REQStat::OFF;
        if (mIsIgnOn == true) {
            if (mH2LeakWarnStatusFromFCU == H2LeakWarnStatusFromFCU::LEAK_EMERGENCY ||
                mH2LeakWarnStatusFromFCU2 == H2LeakWarnStatusFromFCU2::LEAK_EMERGENCY ||
                (mH2LeakIndiReqStatusFromHMU == H2LeakIndiReqStatusFromHMU::LEAK &&
                 mH2LeakWarnStatusFromHMU == H2LeakWarnStatusFromHMU::SD_REQ)) {
                stat = SFCFCEV_Warning_CVEventH2LeakSD_REQStat::ON;
            }
        }
        setSFCFCEV_Warning_CVEventH2LeakSD_REQStat(stat);
    }

    void updateEventH2LeakPERoom() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;
        std::string linkedSoundID;

        if (mIsIgnOn == true) {
            if (mH2LeakWarnLv1StatusFromVCU == H2LeakWarnLv1StatusFromVCU::LEAK &&
                mH2LeakWarnLv2StatusFromVCU != H2LeakWarnLv2StatusFromVCU::LEAK) {
                eventID = "E74504";
                linkedSoundID = "SND_PopUpWarn1";
            } else if (mH2LeakWarnLv2StatusFromVCU == H2LeakWarnLv2StatusFromVCU::LEAK) {
                eventID = "E74505";
                linkedSoundID = "SND_PopUpWarn2";
            } else {
                // no operation
            }
        }
        GETCACHEDVALUE(SFC.FCEV_Warning_CV.Event.H2LeakPERoom.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFCEV_Warning_CVEventH2LeakPERoomStat(SFCFCEV_Warning_CVEventH2LeakPERoomStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFCEV_Warning_CVEventH2LeakPERoomID(eventID);
            setSFCFCEV_Warning_CVEventH2LeakPERoomLinkedSoundID(linkedSoundID);
            setSFCFCEV_Warning_CVEventH2LeakPERoomStat(SFCFCEV_Warning_CVEventH2LeakPERoomStat::ON);
        }
    }

    void updateEventWaterReleaseWarn() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV) {
            return;
        }

        std::string eventID;
        std::string prevEventID;

        // 사양서 내용 중 '!= (상위 조건 값)'은 하기와 같은 우선순위를 나타내기 위함이며
        // WATER_RLS_ONGOING > WATER_RLS_COMPLETE > WAKEUP_FN_ONGING > WAKEUP_FN_COMPLETE
        // 코드는 상위 라인부터 수행되므로 '!= (상위 조건 값)'은 별도 기입하지 않음.
        if (mIsIgnOn == true && (mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_ONGOING ||
                                 mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_ONGOING)) {
            eventID = "E74506";
        } else if (mIsIgnOn == true &&
                   (mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_COMPLETE ||
                    mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_COMPLETE)) {
            eventID = "E74507";
        } else if ((mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WAKEUP_FN_ONGOING &&
                    (mFcevWaterReleaseWarnStatus_FCU2 != FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_ONGOING &&
                     mFcevWaterReleaseWarnStatus_FCU2 != FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_COMPLETE)) ||
                   ((mFcevWaterReleaseWarnStatus_FCU1 != FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_ONGOING &&
                     mFcevWaterReleaseWarnStatus_FCU1 != FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_COMPLETE) &&
                    mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WAKEUP_FN_ONGOING)) {
            eventID = "E74508";
        } else if ((mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WAKEUP_FN_COMPLETE &&
                    (mFcevWaterReleaseWarnStatus_FCU2 != FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_ONGOING &&
                     mFcevWaterReleaseWarnStatus_FCU2 != FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_COMPLETE &&
                     mFcevWaterReleaseWarnStatus_FCU2 != FcevWaterReleaseWarnStatus_FCU2::WAKEUP_FN_ONGOING)) ||
                   ((mFcevWaterReleaseWarnStatus_FCU1 != FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_ONGOING &&
                     mFcevWaterReleaseWarnStatus_FCU1 != FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_COMPLETE &&
                     mFcevWaterReleaseWarnStatus_FCU1 != FcevWaterReleaseWarnStatus_FCU1::WAKEUP_FN_ONGOING) &&
                    mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WAKEUP_FN_COMPLETE)) {
            eventID = "E74509";
        } else if (mIsIgnOn == false &&
                   (mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_ONGOING ||
                    mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_ONGOING)) {
            eventID = "E74510";
        } else if (mIsIgnOn == false &&
                   (mFcevWaterReleaseWarnStatus_FCU1 == FcevWaterReleaseWarnStatus_FCU1::WATER_RLS_COMPLETE ||
                    mFcevWaterReleaseWarnStatus_FCU2 == FcevWaterReleaseWarnStatus_FCU2::WATER_RLS_COMPLETE)) {
            eventID = "E74511";
        } else {
            // no operation
        }

        GETCACHEDVALUE(SFC.FCEV_Warning_CV.Event.WaterReleaseWarn.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCFCEV_Warning_CVEventWaterReleaseWarnStat(SFCFCEV_Warning_CVEventWaterReleaseWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCFCEV_Warning_CVEventWaterReleaseWarnID(eventID);
            setSFCFCEV_Warning_CVEventWaterReleaseWarnStat(SFCFCEV_Warning_CVEventWaterReleaseWarnStat::ON);
        }
    }

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using H2SensorFaultStatusFromFCU = ArgumentsH2SensorFaultStatusFromFCUChanged::Input_H2SensorFaultStatusFromFCU;
    using H2SensorFaultStatusFromFCU2 = ArgumentsH2SensorFaultStatusFromFCUChanged::Input_H2SensorFaultStatusFromFCU2;
    using H2LeakWarnStatusFromFCU = ArgumentsH2LeakWarnStatusFromFCUChanged::Input_H2LeakWarnStatusFromFCU;
    using H2LeakWarnStatusFromFCU2 = ArgumentsH2LeakWarnStatusFromFCUChanged::Input_H2LeakWarnStatusFromFCU2;
    using H2SensorFaultStatusFromHMU = ArgumentsH2SensorFaultStatusFromHMUChanged::Input_H2SensorFaultStatusFromHMU;
    using H2LeakWarnStatusFromHMU = ArgumentsH2LeakWarnStatusFromHMUChanged::Input_H2LeakWarnStatusFromHMU;
    using H2LeakIndiReqStatusFromHMU = ArgumentsH2LeakIndiReqStatusFromHMUChanged::Input_H2LeakIndiReqStatusFromHMU;
    using FcevWaterReleaseWarnStatus_FCU1 = ArgumentsFcevWaterReleaseWarnStatusChanged::Input_FcevWaterReleaseWarnStatus_FCU1;
    using FcevWaterReleaseWarnStatus_FCU2 = ArgumentsFcevWaterReleaseWarnStatusChanged::Input_FcevWaterReleaseWarnStatus_FCU2;
    using H2LeakWarnLv1StatusFromVCU = ArgumentsH2LeakWarnLv1StatusFromVCUChanged::Input_H2LeakWarnLv1StatusFromVCU;
    using H2LeakWarnLv2StatusFromVCU = ArgumentsH2LeakWarnLv2StatusFromVCUChanged::Input_H2LeakWarnLv2StatusFromVCU;
    using H2SensorFaultStatusFromVCU = ArgumentsH2SensorFaultStatusFromVCUChanged::Input_H2SensorFaultStatusFromVCU;

    HBool mIsIgnOn = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    H2SensorFaultStatusFromFCU mH2SensorFaultStatusFromFCU = H2SensorFaultStatusFromFCU::OFF;
    H2SensorFaultStatusFromFCU2 mH2SensorFaultStatusFromFCU2 = H2SensorFaultStatusFromFCU2::OFF;
    H2LeakWarnStatusFromFCU mH2LeakWarnStatusFromFCU = H2LeakWarnStatusFromFCU::OFF;
    H2LeakWarnStatusFromFCU2 mH2LeakWarnStatusFromFCU2 = H2LeakWarnStatusFromFCU2::OFF;
    H2SensorFaultStatusFromHMU mH2SensorFaultStatusFromHMU = H2SensorFaultStatusFromHMU::OFF;
    H2LeakWarnStatusFromHMU mH2LeakWarnStatusFromHMU = H2LeakWarnStatusFromHMU::OFF;
    H2LeakIndiReqStatusFromHMU mH2LeakIndiReqStatusFromHMU = H2LeakIndiReqStatusFromHMU::OFF;
    FcevWaterReleaseWarnStatus_FCU1 mFcevWaterReleaseWarnStatus_FCU1 = FcevWaterReleaseWarnStatus_FCU1::OFF;
    FcevWaterReleaseWarnStatus_FCU2 mFcevWaterReleaseWarnStatus_FCU2 = FcevWaterReleaseWarnStatus_FCU2::OFF;
    H2LeakWarnLv1StatusFromVCU mH2LeakWarnLv1StatusFromVCU = H2LeakWarnLv1StatusFromVCU::NORMAL;
    H2LeakWarnLv2StatusFromVCU mH2LeakWarnLv2StatusFromVCU = H2LeakWarnLv2StatusFromVCU::NORMAL;
    H2SensorFaultStatusFromVCU mH2SensorFaultStatusFromVCU = H2SensorFaultStatusFromVCU::NORMAL;
};

}  // namespace ccos

#endif  // SFSS_FCEV_Warning_CV_H
