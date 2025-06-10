/**
 * @file Drive_Mode_2_0.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024 Hyundai Motor Company,
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
#ifndef SFSS_Drive_Mode_2_0_H
#define SFSS_Drive_Mode_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Drive_Mode_2_0Base.hpp"
#include "Drive_Mode_2_0Types.hpp"
#include <functional>
#include <bitset>

namespace ccos {

// SFC Version : 1.1.2
// Reference : [CD021] Drive_Mode_2_0. Contains : Telltale. Recommend: VALUE_CHANGED
class Drive_Mode_2_0 : public Drive_Mode_2_0Base {
public:
    Drive_Mode_2_0() = default;
    ~Drive_Mode_2_0() override = default;
    Drive_Mode_2_0(const Drive_Mode_2_0& other) = delete;
    Drive_Mode_2_0(Drive_Mode_2_0&& other) noexcept = delete;
    Drive_Mode_2_0& operator=(const Drive_Mode_2_0& other) = delete;
    Drive_Mode_2_0& operator=(Drive_Mode_2_0&& other) noexcept = delete;

    void timerCallbackInter_TerrainModeSelectStatus() {
        mInter_TerrainModeSelectStatus.setNeedFlushed(true);
        processSink(false);
    }

    void timerCallbackInter_DynamicControlPopupStatus() {
        mInter_DynamicControlPopupStatus.setNeedFlushed(true);
        processSink(false);
    }

    void onInitialize() override {
        mInter_TerrainModeSelectStatus.setCallback(std::bind(&Drive_Mode_2_0::timerCallbackInter_TerrainModeSelectStatus, this));
        mInter_DynamicControlPopupStatus.setCallback(
            std::bind(&Drive_Mode_2_0::timerCallbackInter_DynamicControlPopupStatus, this));
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAEMInitComplete, args.mInter_AEMInitializeComplete)) {
            if (mIsAEMInitComplete == true) {
                HUInt64 driveModeSelectStatus = 0;
                HResult result = getCachedValue("SFC.Extension.Memory.Inter_DriveModeSelectStatus", driveModeSelectStatus);
                if (result == HResult::OK) {
                    mInter_DriveModeSelectStatus.setValue(
                        static_cast<SFCDrive_Mode_2_0Inter_DriveModeSelectStatus>(driveModeSelectStatus));

                    mInter_DriveModeSelectStatus.mPrivate_BasicModeList.setCurrentMode(
                        static_cast<SFCDrive_Mode_2_0Inter_DriveModeSelectStatus>(driveModeSelectStatus));
                    _DInfo() << __FUNCTION__ << " Inter_DriveModeSelectStatus: " << driveModeSelectStatus;
                } else {
                    _DInfo() << __FUNCTION__ << " Inter_DriveModeSelectStatus: NORMAL (default)";
                }
                mInter_DriveModeSelectStatus.setNeedFlushed(true);
                mInter_TerrainModeSelectStatus.setNeedFlushed(true);
                processSink(false);
            }
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOn0ms);
        if (initState == true) {
            onIgnChanged(true);
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mElapsedOff0ms);
        if (initState == true || mIgnStates == true) {
            onIgnChanged(false);
            memoryDriveModeSelectStatus();
        }
    }

    void onConfigDriveModeOptionChanged(const ArgumentsConfigDriveModeOptionChanged& args) {
        mInter_ConfigDriveModeOption = args.mInter_ConfigDriveModeOption;
        processSink(false);
    }

    void onConfigDriveModeOptionWithVpcChanged(const ArgumentsConfigDriveModeOptionWithVpcChanged& args) {
        mInter_ConfigDriveModeOptionWithVpc = args.mInter_ConfigDriveModeOptionWithVpc;
        processSink(false);
    }

    void onAwdModeDriveModeControlStatusChanged(const ArgumentsAwdModeDriveModeControlStatusChanged& args) {
        mInput_AwdModeDriveModeControlStatus = args.mInput_AwdModeDriveModeControlStatus;
        processSink(false);
    }

    void onConfigOipDriveModeChanged(const ArgumentsConfigOipDriveModeChanged& args) {
        mInter_ConfigOipDriveMode_Eco = args.mInter_ConfigOipDriveMode_Eco;
        mInter_ConfigOipDriveMode_Normal = args.mInter_ConfigOipDriveMode_Normal;
        mInter_ConfigOipDriveMode_Sport = args.mInter_ConfigOipDriveMode_Sport;
        mInter_ConfigOipDriveMode_SportPlus = args.mInter_ConfigOipDriveMode_SportPlus;
        mInter_ConfigOipDriveMode_Chauffeur = args.mInter_ConfigOipDriveMode_Chauffeur;
        mInter_ConfigOipDriveMode_My = args.mInter_ConfigOipDriveMode_My;
        mInter_ConfigCustomSmart = args.mInter_ConfigCustomSmart;
        mInter_ConfigOipDriveMode_Snow = args.mInter_ConfigOipDriveMode_Snow;
        processSink(false);
    }

    void onDynamicControlForOipSwitchStatusChanged(const ArgumentsDynamicControlForOipSwitchStatusChanged& args) {
        mInput_DynamicControlForOipSwitchStatus = args.mInput_DynamicControlForOipSwitchStatus;
        processSink(false);
    }

    void onDmicInfoChanged(const ArgumentsDmicInfoChanged& args) {
        mInput_DmicTerrainModeMainSwitchStatus = args.mInput_DmicTerrainModeMainSwitchStatus;
        mInput_DmicDriveModeSwitchStatus = args.mInput_DmicDriveModeSwitchStatus;
        mInput_DmicCustomDriveModeSetStatus = args.mInput_DmicCustomDriveModeSetStatus;
        if (ISTIMEOUT(args.mInput_DmicTerrainModeSwitchStatus) == true) {
            mInput_DmicTerrainModeSwitchStatus = EInput_DmicTerrainModeSwitchStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_DmicTerrainModeSwitchStatus = args.mInput_DmicTerrainModeSwitchStatus;
        }
        processSink(false);
    }

    void onDriveModeChangeReqFromAVNTChanged(const ArgumentsDriveModeChangeReqFromAVNTChanged& args) {
        mInter_DriveModeChangeReqFromAVNT = args.mInter_DriveModeChangeReqFromAVNT;
        processSink(false);
    }

    void onIgnOnOffKeepDriveModeStatusChanged(const ArgumentsIgnOnOffKeepDriveModeStatusChanged& args) {
        mInter_ConfigEcoMode = args.mInter_ConfigEcoMode;
        mInter_ConfigDriveModeKeepEco = args.mInter_ConfigDriveModeKeepEco;
        mInter_ConfigDriveModeKeepNormal = args.mInter_ConfigDriveModeKeepNormal;
        mInter_ConfigDriveModeKeepSport = args.mInter_ConfigDriveModeKeepSport;
        mInter_ConfigDriveModeDefault = args.mInter_ConfigDriveModeDefault;
        processSink(false);
    }

    void onOffRoadModeEnableStatusChanged(const ArgumentsOffRoadModeEnableStatusChanged& args) {
        mInter_ConfigOffRoad = args.mInter_ConfigOffRoad;
        mInput_TerrainModeErrorStatus = args.mInput_TerrainModeErrorStatus;
        processSink(false);
    }

    void onConfigTerrainModeChanged(const ArgumentsConfigTerrainModeChanged& args) {
        mInter_ConfigTerrainSnow = args.mInter_ConfigTerrainSnow;
        mInter_ConfigTerrainMud = args.mInter_ConfigTerrainMud;
        mInter_ConfigTerrainSand = args.mInter_ConfigTerrainSand;
        mInter_ConfigTerrainAuto = args.mInter_ConfigTerrainAuto;
        mInter_ConfigTerrainDeepSnow = args.mInter_ConfigTerrainDeepSnow;
        mInter_ConfigTerrainRock = args.mInter_ConfigTerrainRock;
        mInter_ConfigTerrainDesert = args.mInter_ConfigTerrainDesert;
        processSink(false);
    }

    void onSmartDriveModeStatusChanged(const ArgumentsSmartDriveModeStatusChanged& args) {
        mInput_SmartDriveModeStatus = args.mInput_SmartDriveModeStatus;
        processSink(false);
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.2.5 오프로드 모드 리스트 타입
    // NOTE: Input_AwdModeDriveModeControlStatus 시그널에서 특정 bit의 true/false 상태를 return 해주는 함수.
    inline bool checkBitValue(const HUInt64& awdModeDriveModeControlStatusNumber, const HUInt64& bitIndex) const {
        return static_cast<bool>(std::bitset<9>(awdModeDriveModeControlStatusNumber)[bitIndex]);
    }

    void memoryDriveModeSelectStatus() {
        // 4.2 기본 모드 현재 모드 판단 (IGN1 OFF시 현재 드라이브 모드 상태 저장)
        if (mIsAEMInitComplete == true && (mInter_ConfigDriveModeOption == EInter_ConfigDriveModeOption::ON ||
                                           mInter_ConfigDriveModeOptionWithVpc == EInter_ConfigDriveModeOptionWithVpc::ON)) {
            updateValue("SFC.Extension.Memory.Inter_DriveModeSelectStatus",
                        static_cast<HUInt64>(mInter_DriveModeSelectStatus.selfValue()));
        }
    }

    void onIgnChanged(const bool& ign) {
        if (mIgnStates != ign) {
            mIgnStates = ign;

            mPrivate_BasicModeEcoAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeNormalAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeSportAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeMyAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeSmartAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeSnowAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeChauffeurAvailableStatus.mIgnStatus = ign;
            mPrivate_BasicModeSportPlusAvailableStatus.mIgnStatus = ign;
            mInter_DriveModeSelectStatus.mIgnStatus = ign;
            mPrivate_TerrainModeSnowAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeMudAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeSandAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeAutoAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeDeepSnowAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeRockAvailableStatus.mIgnStatus = ign;
            mPrivate_TerrainModeDesertAvailableStatus.mIgnStatus = ign;
            mPrivate_OffroadEvent.mIgnStatus = ign;
            mInter_TerrainModeSelectStatus.mIgnStatus = ign;
            mInter_DynamicControlPopupStatus.mIgnStatus = ign;
            mInter_DriveModeChangeResultStatus.mIgnStatus = ign;
            mTelltaleDriveModeStat.mIgnStatus = ign;

            processSink(ign);
        }
    }

    void processSink(const bool& forceUpdate) {
        if (mInter_ConfigDriveModeOption != EInter_ConfigDriveModeOption::ON &&
            mInter_ConfigDriveModeOptionWithVpc != EInter_ConfigDriveModeOptionWithVpc::ON) {
            return;
        }

        bool hasToFlush = processSinkCalculation();
        if (hasToFlush == true) {
            flush(true);
        }
        hasToFlush = processSinkTelltale() || hasToFlush;
        if (hasToFlush == true) {
            DDebug() << "flushed";
            flush();
        }
        processSinkExternal(forceUpdate);
    }

    void processSinkExternal(const bool& forceUpdate) {
        mOutput_DriveModeSwitchStatus.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mOutput_DriveModeSwitchStatus.renew();

        mOutput_CustomDriveModeSetStatus.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mOutput_CustomDriveModeSetStatus.renew();

        mOutput_TerrainModeSwitchStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mOutput_TerrainModeSwitchStatus.renew();

        mOutput_TerrainModeMainSwitchStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mOutput_TerrainModeMainSwitchStatus.renew();

        if (mOutput_DriveModeSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_DriveModeSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode_2_0.Output_DriveModeSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_DriveModeSwitchStatus.selfValue()));
        }

        if (mOutput_CustomDriveModeSetStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_CustomDriveModeSetStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode_2_0.Output_CustomDriveModeSetStatus,
                                                 static_cast<HUInt64>(mOutput_CustomDriveModeSetStatus.selfValue()));
        }

        if (mOutput_TerrainModeSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_TerrainModeSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode_2_0.Output_TerrainModeSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_TerrainModeSwitchStatus.selfValue()));
        }
        if (mOutput_TerrainModeMainSwitchStatus.isNeedFlushed() == true || forceUpdate == true) {
            mOutput_TerrainModeMainSwitchStatus.setNeedFlushed(false);
            mActuatorOutputAddrList.emplace_back(Vehicle.CD.Drive_Mode_2_0.Output_TerrainModeMainSwitchStatus,
                                                 static_cast<HUInt64>(mOutput_TerrainModeMainSwitchStatus.selfValue()));
        }

        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    bool processSinkCalculation() {
        bool hasToFlush = false;

        mPrivate_BasicModeSwitchableStatus.mInter_ConfigDriveModeOptionWithVpc = mInter_ConfigDriveModeOptionWithVpc;
        mPrivate_BasicModeSwitchableStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mPrivate_BasicModeSwitchableStatus.renew();

        mPrivate_BasicModeForSwKeySwitchableStatus.mInter_ConfigDriveModeOptionWithVpc = mInter_ConfigDriveModeOptionWithVpc;
        mPrivate_BasicModeForSwKeySwitchableStatus.renew();

        mPrivate_BasicModeEcoAvailableStatus.mInter_ConfigOipDriveMode_Eco = mInter_ConfigOipDriveMode_Eco;
        mPrivate_BasicModeEcoAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeEcoBitIndex);
        mPrivate_BasicModeEcoAvailableStatus.renew();

        mPrivate_BasicModeNormalAvailableStatus.mInter_ConfigOipDriveMode_Normal = mInter_ConfigOipDriveMode_Normal;
        mPrivate_BasicModeNormalAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeNormalBitIndex);
        mPrivate_BasicModeNormalAvailableStatus.renew();

        mPrivate_BasicModeSportAvailableStatus.mInter_ConfigOipDriveMode_Sport = mInter_ConfigOipDriveMode_Sport;
        mPrivate_BasicModeSportAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeSportBitIndex);
        mPrivate_BasicModeSportAvailableStatus.renew();

        mPrivate_BasicModeMyAvailableStatus.mInter_ConfigOipDriveMode_My = mInter_ConfigOipDriveMode_My;
        mPrivate_BasicModeMyAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kDriveModeMyDriveBitIndex);
        mPrivate_BasicModeMyAvailableStatus.renew();

        mPrivate_BasicModeSmartAvailableStatus.mInter_ConfigCustomSmart = mInter_ConfigCustomSmart;
        mPrivate_BasicModeSmartAvailableStatus.renew();

        mPrivate_BasicModeSnowAvailableStatus.mInter_ConfigOipDriveMode_Snow = mInter_ConfigOipDriveMode_Snow;
        mPrivate_BasicModeSnowAvailableStatus.renew();

        mPrivate_BasicModeChauffeurAvailableStatus.mInter_ConfigOipDriveMode_Chauffeur = mInter_ConfigOipDriveMode_Chauffeur;
        mPrivate_BasicModeChauffeurAvailableStatus.renew();

        mPrivate_BasicModeSportPlusAvailableStatus.mInter_ConfigOipDriveMode_SportPlus = mInter_ConfigOipDriveMode_SportPlus;
        mPrivate_BasicModeSportPlusAvailableStatus.renew();

        mInter_DriveModeSelectStatus.mPrivate_BasicModeEcoAvailableStatus = mPrivate_BasicModeEcoAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeNormalAvailableStatus = mPrivate_BasicModeNormalAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSportAvailableStatus = mPrivate_BasicModeSportAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeMyAvailableStatus = mPrivate_BasicModeMyAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSmartAvailableStatus = mPrivate_BasicModeSmartAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSnowAvailableStatus = mPrivate_BasicModeSnowAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeChauffeurAvailableStatus = mPrivate_BasicModeChauffeurAvailableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSportPlusAvailableStatus = mPrivate_BasicModeSportPlusAvailableStatus;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeOptionWithVpc = mInter_ConfigDriveModeOptionWithVpc;
        mInter_DriveModeSelectStatus.mInput_DmicTerrainModeMainSwitchStatus = mInput_DmicTerrainModeMainSwitchStatus;
        mInter_DriveModeSelectStatus.mInput_DmicDriveModeSwitchStatus = mInput_DmicDriveModeSwitchStatus;
        mInter_DriveModeSelectStatus.mInput_DmicCustomDriveModeSetStatus = mInput_DmicCustomDriveModeSetStatus;
        mInter_DriveModeSelectStatus.mInput_DynamicControlForOipSwitchStatus = mInput_DynamicControlForOipSwitchStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeSwitchableStatus = mPrivate_BasicModeSwitchableStatus;
        mInter_DriveModeSelectStatus.mPrivate_BasicModeForSwKeySwitchableStatus = mPrivate_BasicModeForSwKeySwitchableStatus;
        mInter_DriveModeSelectStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_DriveModeSelectStatus.mInter_ConfigEcoMode = mInter_ConfigEcoMode;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepEco = mInter_ConfigDriveModeKeepEco;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepNormal = mInter_ConfigDriveModeKeepNormal;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeKeepSport = mInter_ConfigDriveModeKeepSport;
        mInter_DriveModeSelectStatus.mInter_ConfigDriveModeDefault = mInter_ConfigDriveModeDefault;
        mInter_DriveModeSelectStatus.renew();

        mPrivate_TerrainModeSnowAvailableStatus.mInter_ConfigTerrainSnow = mInter_ConfigTerrainSnow;
        mPrivate_TerrainModeSnowAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeSnowBitIndex);
        mPrivate_TerrainModeSnowAvailableStatus.renew();

        mPrivate_TerrainModeMudAvailableStatus.mInter_ConfigTerrainMud = mInter_ConfigTerrainMud;
        mPrivate_TerrainModeMudAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeMudBitIndex);
        mPrivate_TerrainModeMudAvailableStatus.renew();

        mPrivate_TerrainModeSandAvailableStatus.mInter_ConfigTerrainSand = mInter_ConfigTerrainSand;
        mPrivate_TerrainModeSandAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeSandBitIndex);
        mPrivate_TerrainModeSandAvailableStatus.renew();

        mPrivate_TerrainModeAutoAvailableStatus.mInter_ConfigTerrainAuto = mInter_ConfigTerrainAuto;
        mPrivate_TerrainModeAutoAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeAutoBitIndex);
        mPrivate_TerrainModeAutoAvailableStatus.renew();

        mPrivate_TerrainModeDeepSnowAvailableStatus.mInter_ConfigTerrainDeepSnow = mInter_ConfigTerrainDeepSnow;
        mPrivate_TerrainModeDeepSnowAvailableStatus.renew();

        mPrivate_TerrainModeRockAvailableStatus.mInter_ConfigTerrainRock = mInter_ConfigTerrainRock;
        mPrivate_TerrainModeRockAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeRockBitIndex);
        mPrivate_TerrainModeRockAvailableStatus.renew();

        mPrivate_TerrainModeDesertAvailableStatus.mInter_ConfigTerrainDesert = mInter_ConfigTerrainDesert;
        mPrivate_TerrainModeDesertAvailableStatus.mInput_AwdModeDriveModeControlStatus =
            checkBitValue(mInput_AwdModeDriveModeControlStatus, kTerrainModeDesertBitIndex);
        mPrivate_TerrainModeDesertAvailableStatus.renew();

        mPrivate_OffroadEvent.mInput_DynamicControlForOipSwitchStatus = mInput_DynamicControlForOipSwitchStatus;
        mPrivate_OffroadEvent.renew();

        mInter_TerrainModeSelectStatus.mInter_ConfigDriveModeOptionWithVpc = mInter_ConfigDriveModeOptionWithVpc;
        mInter_TerrainModeSelectStatus.mInter_ConfigOffRoad = mInter_ConfigOffRoad;
        mInter_TerrainModeSelectStatus.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeSnowAvailableStatus = mPrivate_TerrainModeSnowAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeMudAvailableStatus = mPrivate_TerrainModeMudAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeSandAvailableStatus = mPrivate_TerrainModeSandAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeAutoAvailableStatus = mPrivate_TerrainModeAutoAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeDeepSnowAvailableStatus = mPrivate_TerrainModeDeepSnowAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeRockAvailableStatus = mPrivate_TerrainModeRockAvailableStatus;
        mInter_TerrainModeSelectStatus.mPrivate_TerrainModeDesertAvailableStatus = mPrivate_TerrainModeDesertAvailableStatus;
        mInter_TerrainModeSelectStatus.mInput_DmicTerrainModeMainSwitchStatus = mInput_DmicTerrainModeMainSwitchStatus;
        mInter_TerrainModeSelectStatus.mInput_DmicTerrainModeSwitchStatus = mInput_DmicTerrainModeSwitchStatus;
        mInter_TerrainModeSelectStatus.mPrivate_OffroadEvent = mPrivate_OffroadEvent;
        mInter_TerrainModeSelectStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_TerrainModeSelectStatus.renew();

        mInter_DynamicControlPopupStatus.mInter_ConfigDriveModeOptionWithVpc = mInter_ConfigDriveModeOptionWithVpc;
        mInter_DynamicControlPopupStatus.mInter_ConfigOffRoad = mInter_ConfigOffRoad;
        mInter_DynamicControlPopupStatus.mInput_TerrainModeErrorStatus = mInput_TerrainModeErrorStatus;
        mInter_DynamicControlPopupStatus.mPrivate_OffroadEvent = mPrivate_OffroadEvent;
        mInter_DynamicControlPopupStatus.renew();

        mInter_DriveModeChangeResultStatus.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mInter_DriveModeChangeResultStatus.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mInter_DriveModeChangeResultStatus.mInter_DriveModeChangeReqFromAVNT = mInter_DriveModeChangeReqFromAVNT;
        mInter_DriveModeChangeResultStatus.mIsDriveModeChangeRequestedFromAVNT =
            mInter_DriveModeSelectStatus.mIsDriveModeChangeRequestedFromAVNT;
        mInter_DriveModeChangeResultStatus.mIsTerrainModeChangeRequestedFromAVNT =
            mInter_TerrainModeSelectStatus.mIsTerrainModeChangeRequestedFromAVNT;
        mInter_DriveModeChangeResultStatus.renew();

        if (mInter_DriveModeSelectStatus.isNeedFlushed() == true) {
            mInter_DriveModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_Mode_2_0Inter_DriveModeSelectStatus(mInter_DriveModeSelectStatus);
        }

        if (mInter_TerrainModeSelectStatus.isNeedFlushed() == true) {
            mInter_TerrainModeSelectStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_Mode_2_0Inter_TerrainModeSelectStatus(mInter_TerrainModeSelectStatus);
        }

        if (mInter_DynamicControlPopupStatus.isNeedFlushed() == true) {
            mInter_DynamicControlPopupStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_Mode_2_0Inter_DynamicControlPopupStatus(mInter_DynamicControlPopupStatus);
        }

        if (mInter_DriveModeChangeResultStatus.isNeedFlushed() == true ||
            mInter_DriveModeChangeResultStatus.mIsForcedFlush == true) {
            mInter_DriveModeChangeResultStatus.setNeedFlushed(false);
            hasToFlush = true;
            setSFCDrive_Mode_2_0Inter_DriveModeChangeResultStatus(mInter_DriveModeChangeResultStatus);
        }

        if (hasToFlush == true) {
            DDebug() << "[HIT] Calculation Logic";
        }
        return hasToFlush;
    }

    bool processSinkTelltale() {
        bool hasToFlush = false;

        mTelltaleDriveModeStat.mInter_TerrainModeSelectStatus = mInter_TerrainModeSelectStatus;
        mTelltaleDriveModeStat.mInter_DriveModeSelectStatus = mInter_DriveModeSelectStatus;
        mTelltaleDriveModeStat.mInput_SmartDriveModeStatus = mInput_SmartDriveModeStatus;
        mTelltaleDriveModeStat.renew();

        if (mTelltaleDriveModeStat.isNeedFlushed() == true) {
            mTelltaleDriveModeStat.setNeedFlushed(false);
            hasToFlush = true;
            if (mTelltaleDriveModeStat == ETelltaleDriveModeStat::OFF) {
                setSFCDrive_Mode_2_0TelltaleDriveModeStat(SFCDrive_Mode_2_0TelltaleDriveModeStat::OFF);
            } else {
                setSFCDrive_Mode_2_0TelltaleDriveModeStat(SFCDrive_Mode_2_0TelltaleDriveModeStat::OPTIONAL);
                setSFCDrive_Mode_2_0TelltaleDriveModeStatOptional(mTelltaleDriveModeStat);
            }
        }
        if (hasToFlush == true) {
            DDebug() << "[HIT] Telltale Logic";
        }
        return hasToFlush;
    }
    static constexpr uint64_t kDriveModeEcoBitIndex = 0;
    static constexpr uint64_t kDriveModeNormalBitIndex = 1;
    static constexpr uint64_t kDriveModeSportBitIndex = 2;
    static constexpr uint64_t kDriveModeMyDriveBitIndex = 3;
    static constexpr uint64_t kTerrainModeSnowBitIndex = 6;
    static constexpr uint64_t kTerrainModeMudBitIndex = 4;
    static constexpr uint64_t kTerrainModeSandBitIndex = 5;
    static constexpr uint64_t kTerrainModeDesertBitIndex = 5;
    static constexpr uint64_t kTerrainModeAutoBitIndex = 8;
    static constexpr uint64_t kTerrainModeRockBitIndex = 7;
    bool mIgnStates = false;

    EInter_ConfigDriveModeOption mInter_ConfigDriveModeOption = EInter_ConfigDriveModeOption::OFF;
    EInter_ConfigDriveModeOptionWithVpc mInter_ConfigDriveModeOptionWithVpc = EInter_ConfigDriveModeOptionWithVpc::OFF;
    Private_BasicModeSwitchableStatus mPrivate_BasicModeSwitchableStatus;
    Private_BasicModeForSwKeySwitchableStatus mPrivate_BasicModeForSwKeySwitchableStatus;
    HUInt64 mInput_AwdModeDriveModeControlStatus = 0;
    EInter_ConfigOipDriveMode_Eco mInter_ConfigOipDriveMode_Eco = EInter_ConfigOipDriveMode_Eco::OFF;
    EInter_ConfigOipDriveMode_Normal mInter_ConfigOipDriveMode_Normal = EInter_ConfigOipDriveMode_Normal::OFF;
    EInter_ConfigOipDriveMode_Sport mInter_ConfigOipDriveMode_Sport = EInter_ConfigOipDriveMode_Sport::OFF;
    EInter_ConfigOipDriveMode_My mInter_ConfigOipDriveMode_My = EInter_ConfigOipDriveMode_My::OFF;
    EInter_ConfigCustomSmart mInter_ConfigCustomSmart = EInter_ConfigCustomSmart::OFF;
    EInter_ConfigOipDriveMode_Snow mInter_ConfigOipDriveMode_Snow = EInter_ConfigOipDriveMode_Snow::OFF;
    EInter_ConfigOipDriveMode_Chauffeur mInter_ConfigOipDriveMode_Chauffeur = EInter_ConfigOipDriveMode_Chauffeur::OFF;
    EInter_ConfigOipDriveMode_SportPlus mInter_ConfigOipDriveMode_SportPlus = EInter_ConfigOipDriveMode_SportPlus::OFF;
    Private_BasicModeEcoAvailableStatus mPrivate_BasicModeEcoAvailableStatus;
    Private_BasicModeNormalAvailableStatus mPrivate_BasicModeNormalAvailableStatus;
    Private_BasicModeSportAvailableStatus mPrivate_BasicModeSportAvailableStatus;
    Private_BasicModeMyAvailableStatus mPrivate_BasicModeMyAvailableStatus;
    Private_BasicModeSmartAvailableStatus mPrivate_BasicModeSmartAvailableStatus;
    Private_BasicModeSnowAvailableStatus mPrivate_BasicModeSnowAvailableStatus;
    Private_BasicModeChauffeurAvailableStatus mPrivate_BasicModeChauffeurAvailableStatus;
    Private_BasicModeSportPlusAvailableStatus mPrivate_BasicModeSportPlusAvailableStatus;
    EInput_DynamicControlForOipSwitchStatus mInput_DynamicControlForOipSwitchStatus =
        EInput_DynamicControlForOipSwitchStatus::UN_PRESSED;
    EInput_DmicTerrainModeMainSwitchStatus mInput_DmicTerrainModeMainSwitchStatus;
    EInput_DmicDriveModeSwitchStatus mInput_DmicDriveModeSwitchStatus;
    EInput_DmicCustomDriveModeSetStatus mInput_DmicCustomDriveModeSetStatus;
    EInput_DmicTerrainModeSwitchStatus mInput_DmicTerrainModeSwitchStatus;
    EInter_DriveModeChangeReqFromAVNT mInter_DriveModeChangeReqFromAVNT = EInter_DriveModeChangeReqFromAVNT::NONE;
    EInter_ConfigEcoMode mInter_ConfigEcoMode = EInter_ConfigEcoMode::OFF;
    EInter_ConfigDriveModeKeepEco mInter_ConfigDriveModeKeepEco = EInter_ConfigDriveModeKeepEco::OFF;
    EInter_ConfigDriveModeKeepNormal mInter_ConfigDriveModeKeepNormal = EInter_ConfigDriveModeKeepNormal::OFF;
    EInter_ConfigDriveModeKeepSport mInter_ConfigDriveModeKeepSport = EInter_ConfigDriveModeKeepSport::OFF;
    EInter_ConfigDriveModeDefault mInter_ConfigDriveModeDefault = EInter_ConfigDriveModeDefault::NORMAL;
    Inter_DriveModeSelectStatus mInter_DriveModeSelectStatus = Inter_DriveModeSelectStatus(EInter_DriveModeSelectStatus::NORMAL);
    EInter_ConfigOffRoad mInter_ConfigOffRoad = EInter_ConfigOffRoad::OFF;
    EInput_TerrainModeErrorStatus mInput_TerrainModeErrorStatus = EInput_TerrainModeErrorStatus::NORMAL;
    EInter_ConfigTerrainSnow mInter_ConfigTerrainSnow = EInter_ConfigTerrainSnow::OFF;
    EInter_ConfigTerrainMud mInter_ConfigTerrainMud = EInter_ConfigTerrainMud::OFF;
    EInter_ConfigTerrainSand mInter_ConfigTerrainSand = EInter_ConfigTerrainSand::OFF;
    EInter_ConfigTerrainAuto mInter_ConfigTerrainAuto = EInter_ConfigTerrainAuto::OFF;
    EInter_ConfigTerrainDeepSnow mInter_ConfigTerrainDeepSnow = EInter_ConfigTerrainDeepSnow::OFF;
    EInter_ConfigTerrainRock mInter_ConfigTerrainRock = EInter_ConfigTerrainRock::OFF;
    EInter_ConfigTerrainDesert mInter_ConfigTerrainDesert = EInter_ConfigTerrainDesert::OFF;
    Private_TerrainModeSnowAvailableStatus mPrivate_TerrainModeSnowAvailableStatus;
    Private_TerrainModeMudAvailableStatus mPrivate_TerrainModeMudAvailableStatus;
    Private_TerrainModeSandAvailableStatus mPrivate_TerrainModeSandAvailableStatus;
    Private_TerrainModeAutoAvailableStatus mPrivate_TerrainModeAutoAvailableStatus;
    Private_TerrainModeDeepSnowAvailableStatus mPrivate_TerrainModeDeepSnowAvailableStatus;
    Private_TerrainModeRockAvailableStatus mPrivate_TerrainModeRockAvailableStatus;
    Private_TerrainModeDesertAvailableStatus mPrivate_TerrainModeDesertAvailableStatus;
    Inter_TerrainModeSelectStatus mInter_TerrainModeSelectStatus =
        Inter_TerrainModeSelectStatus(EInter_TerrainModeSelectStatus::OFF);
    Private_OffroadEvent mPrivate_OffroadEvent;
    Output_DriveModeSwitchStatus mOutput_DriveModeSwitchStatus;
    Output_CustomDriveModeSetStatus mOutput_CustomDriveModeSetStatus;
    Output_TerrainModeSwitchStatus mOutput_TerrainModeSwitchStatus;
    Output_TerrainModeMainSwitchStatus mOutput_TerrainModeMainSwitchStatus;
    Inter_DynamicControlPopupStatus mInter_DynamicControlPopupStatus;
    Inter_DriveModeChangeResultStatus mInter_DriveModeChangeResultStatus =
        Inter_DriveModeChangeResultStatus(EInter_DriveModeChangeResultStatus::INVALID_OPERATION);
    EInput_SmartDriveModeStatus mInput_SmartDriveModeStatus = EInput_SmartDriveModeStatus::INACTIVE;
    TelltaleDriveModeStat mTelltaleDriveModeStat;
    AddressValueList mActuatorOutputAddrList;
    bool mIsAEMInitComplete = false;
};  // Class Drive_Mode_2_0

}  // namespace ccos

#endif  // SFSS_Drive_Mode_2_0_H
