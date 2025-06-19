/**
 * @file Engine_Room_Warning.hpp
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
#ifndef SFSS_Engine_Room_Warning_H
#define SFSS_Engine_Room_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Engine_Room_WarningBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.10
// Reference : [CV520] Engine_Room_Warning. Contains : Event. Recommend: VALUE_CHANGED
class Engine_Room_Warning : public Engine_Room_WarningBase {
public:
    Engine_Room_Warning() = default;
    ~Engine_Room_Warning() override = default;
    Engine_Room_Warning(const Engine_Room_Warning& other) = delete;
    Engine_Room_Warning(Engine_Room_Warning&& other) noexcept = delete;
    Engine_Room_Warning& operator=(const Engine_Room_Warning& other) = delete;
    Engine_Room_Warning& operator=(Engine_Room_Warning&& other) noexcept = delete;

    void onInitialize() override {
        setSFCEngine_Room_WarningEventEngine_Room_WarningLinkedSoundID("SND_PopUpWarn2");
        setSFCEngine_Room_WarningEventEngine_Room_WarningLinkedSoundType(
            SFCEngine_Room_WarningEventEngine_Room_WarningLinkedSoundType::INFINITE);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEvent();
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigVehicleType = args.mInter_ConfigVehicleType;
        updateEvent();
    }

    void onEngineRoomFireWarnStatusChanged(const ArgumentsEngineRoomFireWarnStatusChanged& args) {
        mEngineRoomFireWarnStatus = args.mInput_EngineRoomFireWarnStatus;
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEvent() {
        std::string prevEventID;
        std::string eventID;

        if (mIsIgnOn == true && mEngineRoomFireWarnStatus == EngineRoomFireWarnStatus::ON) {
            switch (mConfigVehicleType) {
                case ConfigVehicleType::ICV:
                    eventID = "E75201";
                    break;
                case ConfigVehicleType::EV:
                case ConfigVehicleType::FCEV:
                    eventID = "E75202";
                    break;
                default:
                    break;
            }
        }

        GETCACHEDVALUE(SFC.Engine_Room_Warning.Event.Engine_Room_Warning.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCEngine_Room_WarningEventEngine_Room_WarningStat(SFCEngine_Room_WarningEventEngine_Room_WarningStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCEngine_Room_WarningEventEngine_Room_WarningID(eventID);
            setSFCEngine_Room_WarningEventEngine_Room_WarningStat(SFCEngine_Room_WarningEventEngine_Room_WarningStat::ON);
        }
    }

    bool mIsIgnOn = false;

    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    using EngineRoomFireWarnStatus = ArgumentsEngineRoomFireWarnStatusChanged::Input_EngineRoomFireWarnStatus;
    EngineRoomFireWarnStatus mEngineRoomFireWarnStatus = EngineRoomFireWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Engine_Room_Warning_H
