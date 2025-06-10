/**
 * @file PHEV_Mode.hpp
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
#ifndef SFSS_PHEV_Mode_H
#define SFSS_PHEV_Mode_H

#define DLOG_ENABLED gEnableSFCLog

#include "PHEV_ModeBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [PT330] PHEV_Mode. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class PHEV_Mode : public PHEV_ModeBase {
public:
    PHEV_Mode() = default;
    ~PHEV_Mode() override = default;
    PHEV_Mode(const PHEV_Mode& other) = delete;
    PHEV_Mode(PHEV_Mode&& other) noexcept = delete;
    PHEV_Mode& operator=(const PHEV_Mode& other) = delete;
    PHEV_Mode& operator=(PHEV_Mode&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIgnStat = true;
        updateEvent();
        updateTelltale();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateEvent();
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIgnStat = false;
        updateEvent();
        updateTelltale();
    }

    void onConfigVehicleTypeChanged(const ArgumentsConfigVehicleTypeChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEvent();
        updateTelltale();
    }

    void onPhevDriveIndReqChanged(const ArgumentsPhevDriveIndReqChanged& args) {
        mPhevDriveIndReq = args.mInput_PhevDriveModeIndiReqStatus;
        updateTelltale();
    }

    void onPhevDriveInfoChanged(const ArgumentsPhevDriveInfoChanged& args) {
        mPhevDriveInfo = args.mInput_PhevDriveModeChangeInfoStatus;
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_500ms && mConfigVehicleType == ConfigVehicleType::PHEV) {
            switch (mPhevDriveIndReq) {
                case PhevDriveIndReq::HEV:
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStat(SFCPHEV_ModeTelltalePHEVCDCSModeStat::OPTIONAL);
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStatOptional(SFCPHEV_ModeTelltalePHEVCDCSModeStatOptional::HEV);
                    break;
                case PhevDriveIndReq::AUTO:
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStat(SFCPHEV_ModeTelltalePHEVCDCSModeStat::OPTIONAL);
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStatOptional(SFCPHEV_ModeTelltalePHEVCDCSModeStatOptional::AUTO);
                    break;
                case PhevDriveIndReq::EV_PLUS:
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStat(SFCPHEV_ModeTelltalePHEVCDCSModeStat::OPTIONAL);
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStatOptional(SFCPHEV_ModeTelltalePHEVCDCSModeStatOptional::EV_PLUS);
                    break;
                default:
                    setSFCPHEV_ModeTelltalePHEVCDCSModeStat(SFCPHEV_ModeTelltalePHEVCDCSModeStat::OFF);
                    break;
            }
        } else {
            setSFCPHEV_ModeTelltalePHEVCDCSModeStat(SFCPHEV_ModeTelltalePHEVCDCSModeStat::OFF);
        }
    }

    void updateEvent() {
        std::string eventID;
        std::string linkedSoundID;
        SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType linkedSoundType =
            SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType::REPEAT_COUNT;
        HUInt64 linkedSoundCount = 1;

        if (mIgnStat && mConfigVehicleType == ConfigVehicleType::PHEV) {
            switch (mPhevDriveInfo) {
                case PhevDriveInfo::EV_MODE_ON:
                    eventID = "E22301";
                    linkedSoundType = SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType::NONE;
                    linkedSoundCount = 0;
                    break;
                case PhevDriveInfo::HEV_MODE_ON:
                    eventID = "E22302";
                    linkedSoundType = SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType::NONE;
                    linkedSoundCount = 0;
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_LOW_SOC:
                    eventID = "E22303";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_LOW_TEMP:
                    eventID = "E22304";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::CHANGE_HEV_MODE_DUE_TO_LOW_TEMP:
                    eventID = "E22305";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::CHANGE_HEV_MODE_DUE_TO_HIGH_TEMP:
                    eventID = "E22306";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_HIGH_TEMP:
                    eventID = "E22307";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::CHANGE_HEV_MODE_DUE_TO_AIR_CON:
                    eventID = "E22308";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_AIR_CON:
                    eventID = "E22309";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::AUTO_MODE_ON:
                    eventID = "E22310";
                    linkedSoundType = SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType::NONE;
                    linkedSoundCount = 0;
                    break;
                case PhevDriveInfo::CHANGE_HEV_MODE_DUE_TO_ENGINE_PROTECTION:
                    eventID = "E22311";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_ENGINE_PROTECTION:
                    eventID = "E22312";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::MAINTAIN_HEV_MODE_DUE_TO_SPORT_MODE:
                    eventID = "E22313";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::CHANGE_MANUAL_MODE_DUE_TO_LOW_SOC:
                    eventID = "E22314";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::CHANGE_HEV_MODE_DUE_TO_SELF_DIAGNOSIS:
                    eventID = "E22315";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::EV_PLUS_MODE_ON:
                    eventID = "E22316";
                    linkedSoundType = SFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType::NONE;
                    linkedSoundCount = 0;
                    break;
                case PhevDriveInfo::EV_PLUS_NOT_AVAILABLE:
                    eventID = "E22317";
                    linkedSoundID = "SND_PopUpWarn1";
                    break;
                case PhevDriveInfo::CHANGE_EV_MODE_DUE_TO_AIR_CON:
                    eventID = "E22318";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::CHANGE_EV_MODE_DUE_TO_LOW_TEMP:
                    eventID = "E22319";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                case PhevDriveInfo::CHANGE_EV_MODE_DUE_TO_HIGH_TEMP:
                    eventID = "E22320";
                    linkedSoundID = "SND_PopUpInform2";
                    break;
                default:
                    break;
            }
        } else {
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.PHEV_Mode.Event.PhevDriveModeChangeInfo.ID, prevEventID);
        if ((!prevEventID.empty()) && (prevEventID != eventID)) {
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoStat(SFCPHEV_ModeEventPhevDriveModeChangeInfoStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoID(eventID);
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoStat(SFCPHEV_ModeEventPhevDriveModeChangeInfoStat::ON);
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundID(linkedSoundID);
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundType(linkedSoundType);
            setSFCPHEV_ModeEventPhevDriveModeChangeInfoLinkedSoundRepeatCount(linkedSoundCount);
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using PhevDriveIndReq = ArgumentsPhevDriveIndReqChanged::Input_PhevDriveModeIndiReqStatus;
    using PhevDriveInfo = ArgumentsPhevDriveInfoChanged::Input_PhevDriveModeChangeInfoStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    PhevDriveIndReq mPhevDriveIndReq = PhevDriveIndReq::OFF;
    PhevDriveInfo mPhevDriveInfo = PhevDriveInfo::OFF;
    HBool mIgnStat = false;
};

}  // namespace ccos

#endif  // SFSS_PHEV_Mode_H
