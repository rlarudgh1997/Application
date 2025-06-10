/**
 * @file Convenience_Device_Control_Screen.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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
#ifndef SFSS_Convenience_Device_Control_Screen_H
#define SFSS_Convenience_Device_Control_Screen_H

#define DLOG_ENABLED gEnableSFCLog

#include "Convenience_Device_Control_ScreenBase.hpp"

namespace ccos {

// SFC Version : 5.0.0
// Reference : [CD220] Convenience_Device_Control_Screen. Contains : Constant. Recommend: VALUE_CHANGED
class Convenience_Device_Control_Screen : public Convenience_Device_Control_ScreenBase {
public:
    Convenience_Device_Control_Screen() = default;
    ~Convenience_Device_Control_Screen() override = default;
    Convenience_Device_Control_Screen(const Convenience_Device_Control_Screen& other) = delete;
    Convenience_Device_Control_Screen(Convenience_Device_Control_Screen&& other) noexcept = delete;
    Convenience_Device_Control_Screen& operator=(const Convenience_Device_Control_Screen& other) = delete;
    Convenience_Device_Control_Screen& operator=(Convenience_Device_Control_Screen&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onConfigBdcPlatformChanged(const ArgumentsConfigBdcPlatformChanged& args) {
        mConfigBdcPlatform = args.mInter_ConfigBdcPlatform;
        updatePrivate_TrunkTailgateOpenControlStatus();
    }

    void onConfigSBCMChanged(const ArgumentsConfigSBCMChanged& args) {
        mConfigSBCM = args.mInter_ConfigSBCM;
        updateConstantDoorLockControl();
    }

    void onConfigTrunkTailgateMenuChanged(const ArgumentsConfigTrunkTailgateMenuChanged& args) {
        mConfigTrunkTailgateMenu = args.mInter_ConfigTrunkTailgateMenu;
        updateConstantTrunkTailgateControl();
    }

    void onConfigFrunkMenuChanged(const ArgumentsConfigFrunkMenuChanged& args) {
        mConfigFrunkMenu = args.mInter_ConfigFrunkMenu;
        updateConstantFrunkOpenButton();
    }

    void onConfigChargingPortOpenMenuChanged(const ArgumentsConfigChargingPortOpenMenuChanged& args) {
        mConfigChargingPortOpenMenu = args.mInter_ConfigChargingPortOpenMenu;
        updateConstantChargingFirstPortOpenControl();
        updateConstantChargingSecondPortOpenControl();
    }

    void onConfigSideMirrorMenuChanged(const ArgumentsConfigSideMirrorMenuChanged& args) {
        mConfigSideMirrorMenu = args.mInter_ConfigSideMirrorMenu;
        DDebug() << "mConfigSideMirrorMenu : " << static_cast<HUInt64>(mConfigSideMirrorMenu);

        updateConstantSideMirrorFoldingControl();
    }

    void onConfigSunroofSmartGlassOptionChanged(const ArgumentsConfigSunroofSmartGlassOptionChanged& args) {
        mConfigSunroofSmartGlassOption = args.mInter_ConfigSunroofSmartGlassOption;
        updateConstantVehicleViewSunroofSmartGlassType();
    }

    void onTrunkTailgateMotorOpenStatusFromBdc2ndChanged(const ArgumentsTrunkTailgateMotorOpenStatusFromBdc2ndChanged& args) {
        mTrunkTailgateMotorOpenStatusFromBdc2nd = args.mInput_TrunkTailgateMotorOpenStatusFromBdc2nd;
        updatePrivate_TrunkTailgateOpenControlStatus();
    }

    void onTrunkTailgateMotorStatusFromBdc3rdChanged(const ArgumentsTrunkTailgateMotorStatusFromBdc3rdChanged& args) {
        mTrunkTailgateMotorStatusFromBdc3rd = args.mInput_TrunkTailgateMotorStatusFromBdc3rd;
        updatePrivate_TrunkTailgateOpenControlStatus();
    }

    void onTrunkTailgateOpenStatusFromBdc3rdChanged(const ArgumentsTrunkTailgateOpenStatusFromBdc3rdChanged& args) {
        mTrunkTailgateOpenStatusFromBdc3rd = args.mInput_TrunkTailgateOpenStatusFromBdc3rd;
        updatePrivate_TrunkTailgateOpenControlStatus();
    }

    void onDoorLockDriverSideStatusFromSBCMChanged(const ArgumentsDoorLockDriverSideStatusFromSBCMChanged& args) {
        mDoorLockDriverSideStatusFromSBCM = args.mInput_DoorLockDriverSideStatusFromSBCM;
        updateConstantDoorLockControl();
    }

    void onDoorLockAssistSideStatusFromSBCMChanged(const ArgumentsDoorLockAssistSideStatusFromSBCMChanged& args) {
        mDoorLockAssistSideStatusFromSBCM = args.mInput_DoorLockAssistSideStatusFromSBCM;
        updateConstantDoorLockControl();
    }

    void onDoorLockRearLeftSideStatusFromSBCMChanged(const ArgumentsDoorLockRearLeftSideStatusFromSBCMChanged& args) {
        mDoorLockRearLeftSideStatusFromSBCM = args.mInput_DoorLockRearLeftSideStatusFromSBCM;
        updateConstantDoorLockControl();
    }

    void onDoorLockRearRightSideStatusFromSBCMChanged(const ArgumentsDoorLockRearRightSideStatusFromSBCMChanged& args) {
        mDoorLockRearRightSideStatusFromSBCM = args.mInput_DoorLockRearRightSideStatusFromSBCM;
        updateConstantDoorLockControl();
    }

    void onDoorLockDriverSideStatusChanged(const ArgumentsDoorLockDriverSideStatusChanged& args) {
        mDoorLockDriverSideStatus = args.mInput_DoorLockDriverSideStatus;
        updateConstantDoorLockControl();
    }

    void onDoorLockAssistSideStatusChanged(const ArgumentsDoorLockAssistSideStatusChanged& args) {
        mDoorLockAssistSideStatus = args.mInput_DoorLockAssistSideStatus;
        updateConstantDoorLockControl();
    }

    void onDoorLockRearLeftSideStatusChanged(const ArgumentsDoorLockRearLeftSideStatusChanged& args) {
        mDoorLockRearLeftSideStatus = args.mInput_DoorLockRearLeftSideStatus;
        updateConstantDoorLockControl();
    }

    void onDoorLockRearRightSideStatusChanged(const ArgumentsDoorLockRearRightSideStatusChanged& args) {
        mDoorLockRearRightSideStatus = args.mInput_DoorLockRearRightSideStatus;
        updateConstantDoorLockControl();
    }

    void onPtgmOnOffStatusChanged(const ArgumentsPtgmOnOffStatusChanged& args) {
        mPtgmOnOffStatus = args.mInput_PtgmOnOffStatus;
        updateConstantTrunkTailgateControl();
    }

    void onPtgmFromBdcOnOffStatusChanged(const ArgumentsPtgmFromBdcOnOffStatusChanged& args) {
        mPtgmFromBdcOnOffStatus = args.mInput_PtgmFromBdcOnOffStatus;
        updateConstantTrunkTailgateControl();
    }

    void onEvChargeDoorOpenStatusChanged(const ArgumentsEvChargeDoorOpenStatusChanged& args) {
        mEvChargeDoorOpenStatus = args.mInput_EvChargeDoorOpenStatus;
        updateConstantChargingFirstPortOpenControl();
        updateConstantVehicleViewChargingFirstPortOpen();
    }

    void onEvChargeSecondDoorOpenStatusChanged(const ArgumentsEvChargeSecondDoorOpenStatusChanged& args) {
        mEvChargeSecondDoorOpenStatus = args.mInput_EvChargeSecondDoorOpenStatus;
        updateConstantChargingSecondPortOpenControl();
    }

    void onOutMirrorFoldStatusFromBCMChanged(const ArgumentsOutMirrorFoldStatusFromBCMChanged& args) {
        mOutMirrorFoldStatusFromBCM = args.mInput_OutMirrorFoldStatusFromBCM;
        DDebug() << "mOutMirrorFoldStatusFromBCM : " << static_cast<HUInt64>(mOutMirrorFoldStatusFromBCM);

        updateConstantVehicleViewSideMirrorFoldAction();
    }

    void onOutMirrorUnfoldStatusFromBCMChanged(const ArgumentsOutMirrorUnfoldStatusFromBCMChanged& args) {
        mOutMirrorUnfoldStatusFromBCM = args.mInput_OutMirrorUnfoldStatusFromBCM;
        DDebug() << "mOutMirrorUnfoldStatusFromBCM : " << static_cast<HUInt64>(mOutMirrorUnfoldStatusFromBCM);

        updateConstantVehicleViewSideMirrorUnfoldAction();
    }

    void onBrakeLampControlCommand_StatusChanged(const ArgumentsBrakeLampControlCommand_StatusChanged& args) {
        mBrakeLampControlCommand_Status = args.mInput_BrakeLampControlCommand_Status;
        updateConstantVehicleViewBrakeLight();
    }

    void onEvFastChargeFailWarnStatusFromVCMSChanged(const ArgumentsEvFastChargeFailWarnStatusFromVCMSChanged& args) {
        mEvFastChargeFailWarnStatusFromVCMS = args.mInput_EvFastChargeFailWarnStatusFromVCMS;
        updateConstantVehicleViewChargingCableConnect();
    }

    void onTelltaleEvChgCableStatChanged(const ArgumentsTelltaleEvChgCableStatChanged& args) {
        mTelltaleEvChgCableStat = args.mEvChgCableStat;
        updateConstantVehicleViewChargingCableConnect();
    }

    void onPtgmOptionInfoStatusChanged(const ArgumentsPtgmOptionInfoStatusChanged& args) {
        mPtgmOptionInfoStatus = args.mInput_PtgmOptionInfoStatus;
        DDebug() << "mPtgmOptionInfoStatus : " << static_cast<HUInt64>(mPtgmOptionInfoStatus);

        updateConstantVehicleViewTrunkOpen();
    }

    void onPowerTrunkOpenPositionValueChanged(const ArgumentsPowerTrunkOpenPositionValueChanged& args) {
        mPowerTrunkOpenPositionValue = args.mInput_PowerTrunkOpenPositionValue;
        DDebug() << "mPowerTrunkOpenPositionValue : " << static_cast<HUInt64>(mPowerTrunkOpenPositionValue);

        updateConstantVehicleViewTrunkOpen();
    }

    void onTrunkOpenStatusChanged(const ArgumentsTrunkOpenStatusChanged& args) {
        mTrunkOpenStatus = args.mInput_TrunkOpenStatus;
        DDebug() << "mTrunkOpenStatus : " << static_cast<HUInt64>(mTrunkOpenStatus);

        updateConstantVehicleViewTrunkOpen();
    }

    void onSmartRoofGlassZoneOnOffStatusChanged(const ArgumentsSmartRoofGlassZoneOnOffStatusChanged& args) {
        mSmartRoofGlassZone1OnOffStatus = args.mInput_SmartRoofGlassZone1OnOffStatus;
        mSmartRoofGlassZone2OnOffStatus = args.mInput_SmartRoofGlassZone2OnOffStatus;
        mSmartRoofGlassZone3OnOffStatus = args.mInput_SmartRoofGlassZone3OnOffStatus;
        mSmartRoofGlassZone4OnOffStatus = args.mInput_SmartRoofGlassZone4OnOffStatus;
        mSmartRoofGlassZone5OnOffStatus = args.mInput_SmartRoofGlassZone5OnOffStatus;
        mSmartRoofGlassZone6OnOffStatus = args.mInput_SmartRoofGlassZone6OnOffStatus;
        mSmartRoofGlassZone7OnOffStatus = args.mInput_SmartRoofGlassZone7OnOffStatus;
        mSmartRoofGlassZone8OnOffStatus = args.mInput_SmartRoofGlassZone8OnOffStatus;
        mSmartRoofGlassZone9OnOffStatus = args.mInput_SmartRoofGlassZone9OnOffStatus;
        mSmartRoofGlassZone10OnOffStatus = args.mInput_SmartRoofGlassZone10OnOffStatus;
        updateConstantVehicleViewSunroofSmartGlassZone();
    }

    void onSmartRoofGlassZoneFaultStatusChanged(const ArgumentsSmartRoofGlassZoneFaultStatusChanged& args) {
        mSmartRoofGlassZone1FaultStatus = args.mInput_SmartRoofGlassZone1FaultStatus;
        mSmartRoofGlassZone2FaultStatus = args.mInput_SmartRoofGlassZone2FaultStatus;
        mSmartRoofGlassZone3FaultStatus = args.mInput_SmartRoofGlassZone3FaultStatus;
        mSmartRoofGlassZone4FaultStatus = args.mInput_SmartRoofGlassZone4FaultStatus;
        mSmartRoofGlassZone5FaultStatus = args.mInput_SmartRoofGlassZone5FaultStatus;
        mSmartRoofGlassZone6FaultStatus = args.mInput_SmartRoofGlassZone6FaultStatus;
        mSmartRoofGlassZone7FaultStatus = args.mInput_SmartRoofGlassZone7FaultStatus;
        mSmartRoofGlassZone8FaultStatus = args.mInput_SmartRoofGlassZone8FaultStatus;
        mSmartRoofGlassZone9FaultStatus = args.mInput_SmartRoofGlassZone9FaultStatus;
        mSmartRoofGlassZone10FaultStatus = args.mInput_SmartRoofGlassZone10FaultStatus;
        updateConstantVehicleViewSunroofSmartGlassZone();
    }

    void onSunroofFrontGlassOpenPositionValueChanged(const ArgumentsSunroofFrontGlassOpenPositionValueChanged& args) {
        mSunroofFrontGlassOpenPositionValue = args.mInput_SunroofFrontGlassOpenPositionValue;
        updateConstantVehicleViewSunroofFrontGlassOpen();
    }

    void onSunroofFrontBlindOpenPositionValueChanged(const ArgumentsSunroofFrontBlindOpenPositionValueChanged& args) {
        mSunroofFrontBlindOpenPositionValue = args.mInput_SunroofFrontBlindOpenPositionValue;
        updateConstantVehicleViewSunroofFrontBlindOpen();
    }

    void onSunroofRearGlassOpenPositionValueChanged(const ArgumentsSunroofRearGlassOpenPositionValueChanged& args) {
        mSunroofRearGlassOpenPositionValue = args.mInput_SunroofRearGlassOpenPositionValue;
        updateConstantVehicleViewSunroofRearGlassOpen();
    }

    void onSunroofRearBlindOpenPositionValueChanged(const ArgumentsSunroofRearBlindOpenPositionValueChanged& args) {
        mSunroofRearBlindOpenPositionValue = args.mInput_SunroofRearBlindOpenPositionValue;
        updateConstantVehicleViewSunroofRearBlindOpen();
    }

    void onSunroofFrontGlassStatusChanged(const ArgumentsSunroofFrontGlassStatusChanged& args) {
        mSunroofFrontGlassStatus = args.mInput_SunroofFrontGlassStatus;
        updateConstantVehicleViewSunroofFrontGlassOpen();
        updateConstantVehicleViewSunroofFrontBlindOpen();
    }

    void onSunroofRearGlassStatusChanged(const ArgumentsSunroofRearGlassStatusChanged& args) {
        mSunroofRearGlassStatus = args.mInput_SunroofRearGlassStatus;
        updateConstantVehicleViewSunroofRearGlassOpen();
        updateConstantVehicleViewSunroofRearBlindOpen();
    }

    void onFrunkOpenControlDisableStatusChanged(const ArgumentsFrunkOpenControlDisableStatusChanged& args) {
        mFrunkOpenControlDisableStatus1 = args.mInput_FrunkOpenControlDisableStatus1;
        mFrunkOpenControlDisableStatus2 = args.mInput_FrunkOpenControlDisableStatus2;
        updateConstantFrunkOpenButton();
    }

    void onHoodOpenStatusChanged(const ArgumentsHoodOpenStatusChanged& args) {
        mElecHoodOptionStatus = args.mInput_ElecHoodOptionStatus;
        mHoodOpenStatus = args.mInput_HoodOpenStatus;
        mElecHoodOpenStatus = args.mInput_ElecHoodOpenStatus;
        mFrunkOpenStatus = args.mInput_FrunkOpenStatus;
        updateConstantVehicleViewHoodOpen();
    }

    void onWindowDriverOpenPositionValueChanged(const ArgumentsWindowDriverOpenPositionValueChanged& args) {
        mWindowDriverOpenPositionValue = args.mInput_WindowDriverOpenPositionValue;
        mWindowDriverErrorStatus = args.mInput_WindowDriverErrorStatus;
        updateConstantVehicleViewWindowDriverOpen();
    }

    void onWindowAssistOpenPositionValueChanged(const ArgumentsWindowAssistOpenPositionValueChanged& args) {
        mWindowAssistOpenPositionValue = args.mInput_WindowAssistOpenPositionValue;
        mWindowAssistErrorStatus = args.mInput_WindowAssistErrorStatus;
        updateConstantVehicleViewWindowAssistOpen();
    }

    void onWindowRearLeftOpenPositionValueChanged(const ArgumentsWindowRearLeftOpenPositionValueChanged& args) {
        mWindowRearLeftOpenPositionValue = args.mInput_WindowRearLeftOpenPositionValue;
        mWindowRearLeftErrorStatus = args.mInput_WindowRearLeftErrorStatus;
        updateConstantVehicleViewWindowRearLeftOpen();
    }

    void onWindowRearRightOpenPositionValueChanged(const ArgumentsWindowRearRightOpenPositionValueChanged& args) {
        mWindowRearRightOpenPositionValue = args.mInput_WindowRearRightOpenPositionValue;
        mWindowRearRightErrorStatus = args.mInput_WindowRearRightErrorStatus;
        updateConstantVehicleViewWindowRearRightOpen();
    }

    void onConfigWindowOpenDisplayOptionChanged(const ArgumentsConfigWindowOpenDisplayOptionChanged& args) {
        mConfigWindowOpenDisplayOption = args.mInter_ConfigWindowOpenDisplayOption;
        updateConstantVehicleViewWindowOpenDispEnable();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4 연산 로직
    // 4.2 Trunk&TailGate 작동 상태 추상화
    void updatePrivate_TrunkTailgateOpenControlStatus() {
        if (mConfigBdcPlatform == ConfigBdcPlatform::BDC_SECOND) {
            switch (mTrunkTailgateMotorOpenStatusFromBdc2nd) {
                case TrunkTailgateMotorOpenStatusFromBdc2nd::OPENING:
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::OPENING;
                    break;
                case TrunkTailgateMotorOpenStatusFromBdc2nd::CLOSING:
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::CLOSING;
                    break;
                case TrunkTailgateMotorOpenStatusFromBdc2nd::OPEN_LV1... TrunkTailgateMotorOpenStatusFromBdc2nd::OPEN_FULL:
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::OPEN;
                    break;
                case TrunkTailgateMotorOpenStatusFromBdc2nd::CLOSE:
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::CLOSE;
                    break;
                default:
                    // no operation
                    break;
            }
        } else if (mConfigBdcPlatform == ConfigBdcPlatform::BDC_THIRD) {
            if (mTrunkTailgateMotorStatusFromBdc3rd == TrunkTailgateMotorStatusFromBdc3rd::OPENING) {
                mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::OPENING;
            } else if (mTrunkTailgateMotorStatusFromBdc3rd == TrunkTailgateMotorStatusFromBdc3rd::CLOSING) {
                mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::CLOSING;
            } else {
                if (mTrunkTailgateOpenStatusFromBdc3rd == TrunkTailgateOpenStatusFromBdc3rd::OPEN) {
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::OPEN;
                } else if (mTrunkTailgateOpenStatusFromBdc3rd == TrunkTailgateOpenStatusFromBdc3rd::CLOSE) {
                    mPrivate_TrunkTailgateOpenControlStatus = Private_TrunkTailgateOpenControlStatus::CLOSE;
                } else {
                    // no operation
                }
            }
        } else {
            // no operation
        }
        updateConstantTrunkTailgateControl();
    }

    // 4.3 상태 변경 요청 송출
    // 4.3.1 Door Lock/Unlock Request             : SFC 구현 X (Touch 입력을 조건으로 하여 application 내 조치 필요)
    // 4.3.2 Trunk&TailGate Open/Close Request    : SFC 구현 X (Touch 입력을 조건으로 하여 application 내 조치 필요)
    // 4.3.3 Frunk Open Request                   : SFC 구현 X (Touch 입력을 조건으로 하여 application 내 조치 필요)
    // 4.3.4 EV Charging Door Open/Close Request  : SFC 구현 X (Touch 입력을 조건으로 하여 application 내 조치 필요)
    // 4.3.5 Side Mirror Folding/Unfolding Request: SFC 구현 X (Touch 입력을 조건으로 하여 application 내 조치 필요)

    // 5 표시 로직
    // 5.1 Telltale: N/A

    // 5.2 표시 구성요소 정보 처리 로직
    // 5.2.1 Controller 표시 정보
    // 5.2.1.1 Door Lock Control
    void updateConstantDoorLockControl() {
        SFCConvenience_Device_Control_ScreenConstantDoorLockControlStat stat =
            SFCConvenience_Device_Control_ScreenConstantDoorLockControlStat::UNLOCK;
        if (mConfigSBCM == ConfigSBCM::ON) {
            if (mDoorLockDriverSideStatusFromSBCM == DoorLockDriverSideStatusFromSBCM::LOCK &&
                mDoorLockAssistSideStatusFromSBCM == DoorLockAssistSideStatusFromSBCM::LOCK &&
                mDoorLockRearLeftSideStatusFromSBCM == DoorLockRearLeftSideStatusFromSBCM::LOCK &&
                mDoorLockRearRightSideStatusFromSBCM == DoorLockRearRightSideStatusFromSBCM::LOCK) {
                stat = SFCConvenience_Device_Control_ScreenConstantDoorLockControlStat::LOCK;
            }
        } else {
            if (mDoorLockDriverSideStatus == DoorLockDriverSideStatus::LOCK &&
                mDoorLockAssistSideStatus == DoorLockAssistSideStatus::LOCK &&
                mDoorLockRearLeftSideStatus == DoorLockRearLeftSideStatus::LOCK &&
                mDoorLockRearRightSideStatus == DoorLockRearRightSideStatus::LOCK) {
                stat = SFCConvenience_Device_Control_ScreenConstantDoorLockControlStat::LOCK;
            }
        }
        setSFCConvenience_Device_Control_ScreenConstantDoorLockControlStat(stat);
    }

    // 5.2.1.2 Trunk&TailGate Open Control
    void updateConstantTrunkTailgateControl() {
        if (mConfigTrunkTailgateMenu == ConfigTrunkTailgateMenu::APPLIED) {
            if (mPtgmOnOffStatus == PtgmOnOffStatus::DISABLE || mPtgmFromBdcOnOffStatus == PtgmFromBdcOnOffStatus::DISABLE) {
                setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                    SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::DEACTIVATE);
            } else {
                switch (mPrivate_TrunkTailgateOpenControlStatus) {
                    case Private_TrunkTailgateOpenControlStatus::OPENING:
                        setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                            SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::OPENING);
                        break;
                    case Private_TrunkTailgateOpenControlStatus::CLOSING:
                        setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                            SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::CLOSING);
                        break;
                    case Private_TrunkTailgateOpenControlStatus::OPEN:
                        setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                            SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::OPEN);
                        break;
                    case Private_TrunkTailgateOpenControlStatus::CLOSE:
                        setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                            SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::CLOSE);
                        break;
                    default:
                        // no operation
                        break;
                }
            }
        } else if (mConfigTrunkTailgateMenu == ConfigTrunkTailgateMenu::NOT_APPLIED) {
            setSFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat(
                SFCConvenience_Device_Control_ScreenConstantTrunkTailgateControlStat::NOT_APPLIED);
        } else {
            // no operation
        }
    }

    // 5.2.1.3 Frunk Open Control
    void updateConstantFrunkOpenButton() {
        SFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat stat =
            SFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat::NONE;

        if (mConfigFrunkMenu == ConfigFrunkMenu::APPLIED) {
            if (mFrunkOpenControlDisableStatus1 == FrunkOpenControlDisableStatus1::DISABLE_ON ||
                mFrunkOpenControlDisableStatus2 == FrunkOpenControlDisableStatus2::DISABLE_ON) {
                stat = SFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat::DEACTIVATE;
            } else if (mFrunkOpenControlDisableStatus1 == FrunkOpenControlDisableStatus1::OFF &&
                       mFrunkOpenControlDisableStatus2 == FrunkOpenControlDisableStatus2::OFF) {
                stat = SFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat::ACTIVATE;
            } else {
                // no operation
            }
        } else if (mConfigFrunkMenu == ConfigFrunkMenu::NOT_APPLIED) {
            stat = SFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat::NOT_APPLIED;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantFrunkOpenButtonStat(stat);
    }

    // 5.2.1.4 EV Charging Door Open Control
    // 5.2.1.4.1 One Port or First Port Spec
    void updateConstantChargingFirstPortOpenControl() {
        if (mConfigChargingPortOpenMenu == ConfigChargingPortOpenMenu::APPLIED_ONE_PORT ||
            mConfigChargingPortOpenMenu == ConfigChargingPortOpenMenu::APPLIED_TWO_PORT) {
            if (mEvChargeDoorOpenStatus == EvChargeDoorOpenStatus::OPEN) {
                setSFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat(
                    SFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat::OPEN);
            } else if (mEvChargeDoorOpenStatus == EvChargeDoorOpenStatus::CLOSE) {
                setSFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat(
                    SFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat::CLOSE);
            } else {
                // no operation
            }
        } else if (mConfigChargingPortOpenMenu == ConfigChargingPortOpenMenu::NOT_APPLIED) {
            setSFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat(
                SFCConvenience_Device_Control_ScreenConstantChargingFirstPortOpenControlStat::NOT_APPLIED);
        } else {
            // no operation
        }
    }

    // 5.2.1.4.2 Second Port Spec
    void updateConstantChargingSecondPortOpenControl() {
        if (mConfigChargingPortOpenMenu == ConfigChargingPortOpenMenu::APPLIED_TWO_PORT) {
            if (mEvChargeSecondDoorOpenStatus == EvChargeSecondDoorOpenStatus::OPEN) {
                setSFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat(
                    SFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat::OPEN);
            } else if (mEvChargeSecondDoorOpenStatus == EvChargeSecondDoorOpenStatus::CLOSE) {
                setSFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat(
                    SFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat::CLOSE);
            } else {
                // no operation
            }
        } else {
            setSFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat(
                SFCConvenience_Device_Control_ScreenConstantChargingSecondPortOpenControlStat::NOT_APPLIED);
        }
    }

    // 5.2.1.5 Side Mirror Fold Control
    void updateConstantSideMirrorFoldingControl() {
        if (mConfigSideMirrorMenu == ConfigSideMirrorMenu::APPLIED) {
            setSFCConvenience_Device_Control_ScreenConstantSideMirrorFoldingControlStat(
                SFCConvenience_Device_Control_ScreenConstantSideMirrorFoldingControlStat::APPLIED);
        } else if (mConfigSideMirrorMenu == ConfigSideMirrorMenu::NOT_APPLIED) {
            setSFCConvenience_Device_Control_ScreenConstantSideMirrorFoldingControlStat(
                SFCConvenience_Device_Control_ScreenConstantSideMirrorFoldingControlStat::NOT_APPLIED);
        } else {
            // no operation
        }
    }

    // 5.2.2 장치 상태 표시
    // 5.2.2.1 EV charging door Open status
    void updateConstantVehicleViewChargingFirstPortOpen() {
        if (mEvChargeDoorOpenStatus == EvChargeDoorOpenStatus::OPEN) {
            setSFCConvenience_Device_Control_ScreenConstantVehicleViewChargingFirstPortOpenStat(
                SFCConvenience_Device_Control_ScreenConstantVehicleViewChargingFirstPortOpenStat::OPEN);
        } else if (mEvChargeDoorOpenStatus == EvChargeDoorOpenStatus::CLOSE) {
            setSFCConvenience_Device_Control_ScreenConstantVehicleViewChargingFirstPortOpenStat(
                SFCConvenience_Device_Control_ScreenConstantVehicleViewChargingFirstPortOpenStat::CLOSE);
        } else {
            // no operation
        }
    }

    // 5.2.2.2 Side Mirror Open Status
    void updateConstantVehicleViewSideMirrorFoldAction() {
        switch (mOutMirrorFoldStatusFromBCM) {
            case OutMirrorFoldStatusFromBCM::FOLD:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat::FOLD);
                break;
            case OutMirrorFoldStatusFromBCM::FOLDING:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat::FOLDING);
                break;
            case OutMirrorFoldStatusFromBCM::OFF:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorFoldActionStat::OFF);
                break;
            default:
                // no operation
                break;
        }
    }

    void updateConstantVehicleViewSideMirrorUnfoldAction() {
        switch (mOutMirrorUnfoldStatusFromBCM) {
            case OutMirrorUnfoldStatusFromBCM::UNFOLD:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat::UNFOLD);
                break;
            case OutMirrorUnfoldStatusFromBCM::UNFOLDING:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat::UNFOLDING);
                break;
            case OutMirrorUnfoldStatusFromBCM::OFF:
                setSFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat(
                    SFCConvenience_Device_Control_ScreenConstantVehicleViewSideMirrorUnfoldActionStat::OFF);
                break;
            default:
                // no operation
                break;
        }
    }

    // 5.2.2.3 Brake Lamp Status
    void updateConstantVehicleViewBrakeLight() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewBrakeLightStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewBrakeLightStat::OFF;
        if (mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::STEADY_ON ||
            mBrakeLampControlCommand_Status == BrakeLampControlCommand_Status::BLINK_ON) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewBrakeLightStat::ON;
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewBrakeLightStat(stat);
    }

    // 5.2.2.4 EV Charging Cable Connect Status
    void updateConstantVehicleViewChargingCableConnect() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewChargingCableConnectStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewChargingCableConnectStat::OFF;
        if (mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_SCHEDULE ||
            mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_NORMAL ||
            mEvFastChargeFailWarnStatusFromVCMS == EvFastChargeFailWarnStatusFromVCMS::CHARGING_WIRELESS ||
            mTelltaleEvChgCableStat == TelltaleEvChgCableStat::ON) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewChargingCableConnectStat::ON;
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewChargingCableConnectStat(stat);
    }

    // 5.2.2.5 Trunk&Tailgate Open Status
    void updateConstantVehicleViewTrunkOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat::CLOSE;
        HUInt64 value = 0;
        if (mTrunkOpenStatus == TrunkOpenStatus::CRC_ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat::ERROR;
        } else if (mPtgmOptionInfoStatus == PtgmOptionInfoStatus::POWER_TRUNK ||
                   mPtgmOptionInfoStatus == PtgmOptionInfoStatus::POWER_TAILGATE) {
            value = mPowerTrunkOpenPositionValue;
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat::OPEN;
        } else if (mTrunkOpenStatus == TrunkOpenStatus::OPEN) {
            value = kPowerTrunkOpenMaxPosition;
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat::OPEN;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenValue(value);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewTrunkOpenStat(stat);
    }

    // 5.2.2.6 Sunroof Status
    // IMG_VehicleViewSunroofSmartGlassType_stat
    void updateConstantVehicleViewSunroofSmartGlassType() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::NONE;
        switch (mConfigSunroofSmartGlassOption) {
            case ConfigSunroofSmartGlassOption::SEG1_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG1;
                break;
            case ConfigSunroofSmartGlassOption::SEG2_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG2;
                break;
            case ConfigSunroofSmartGlassOption::SEG3_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG3;
                break;
            case ConfigSunroofSmartGlassOption::SEG4_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG4;
                break;
            case ConfigSunroofSmartGlassOption::SEG5_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG5;
                break;
            case ConfigSunroofSmartGlassOption::SEG6_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG6;
                break;
            case ConfigSunroofSmartGlassOption::SEG7_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG7;
                break;
            case ConfigSunroofSmartGlassOption::SEG8_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG8;
                break;
            case ConfigSunroofSmartGlassOption::SEG9_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG9;
                break;
            case ConfigSunroofSmartGlassOption::SEG10_APPLIED:
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat::SEG10;
                break;
            default:
                // no operation
                break;
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassTypeStat(stat);
    }

    // IMG_VehicleViewSunroofSmartGlassZone{N}_stat
    template <typename OutputSignalType, typename InputSignalType1, typename InputSignalType2>
    OutputSignalType getSunroofSmartGlassType(const InputSignalType1& smartRoofGlassZoneOnOffStatus,
                                              const InputSignalType2& smartRoofGlassZoneFaultStatus) {
        OutputSignalType stat = OutputSignalType::OFF;
        if (smartRoofGlassZoneFaultStatus == InputSignalType2::NORMAL &&
            smartRoofGlassZoneOnOffStatus == InputSignalType1::CLEAR_ON) {
            stat = OutputSignalType::CLEAR;
        } else if (smartRoofGlassZoneFaultStatus == InputSignalType2::FAULT) {
            stat = OutputSignalType::ERROR;
        } else {
            // no operation
        }
        return stat;
    }

    void updateConstantVehicleViewSunroofSmartGlassZone() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone1Stat stat1 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone1Stat>(
                mSmartRoofGlassZone1OnOffStatus, mSmartRoofGlassZone1FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone1Stat(stat1);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone2Stat stat2 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone2Stat>(
                mSmartRoofGlassZone2OnOffStatus, mSmartRoofGlassZone2FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone2Stat(stat2);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone3Stat stat3 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone3Stat>(
                mSmartRoofGlassZone3OnOffStatus, mSmartRoofGlassZone3FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone3Stat(stat3);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone4Stat stat4 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone4Stat>(
                mSmartRoofGlassZone4OnOffStatus, mSmartRoofGlassZone4FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone4Stat(stat4);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone5Stat stat5 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone5Stat>(
                mSmartRoofGlassZone5OnOffStatus, mSmartRoofGlassZone5FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone5Stat(stat5);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone6Stat stat6 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone6Stat>(
                mSmartRoofGlassZone6OnOffStatus, mSmartRoofGlassZone6FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone6Stat(stat6);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone7Stat stat7 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone7Stat>(
                mSmartRoofGlassZone7OnOffStatus, mSmartRoofGlassZone7FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone7Stat(stat7);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone8Stat stat8 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone8Stat>(
                mSmartRoofGlassZone8OnOffStatus, mSmartRoofGlassZone8FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone8Stat(stat8);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone9Stat stat9 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone9Stat>(
                mSmartRoofGlassZone9OnOffStatus, mSmartRoofGlassZone9FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone9Stat(stat9);

        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone10Stat stat10 =
            getSunroofSmartGlassType<SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone10Stat>(
                mSmartRoofGlassZone10OnOffStatus, mSmartRoofGlassZone10FaultStatus);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofSmartGlassZone10Stat(stat10);
    }

    // IMG_VehicleViewSunroofFrontGlassOpen_stat
    void updateConstantVehicleViewSunroofFrontGlassOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenStat::CLOSE;
        HUInt64 value = 0;
        if (mSunroofFrontGlassStatus == SunroofFrontGlassStatus::NORMAL &&
            (mSunroofFrontGlassOpenPositionValue >= 0 && mSunroofFrontGlassOpenPositionValue <= 100)) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenStat::OPEN;
            value = mSunroofFrontGlassOpenPositionValue;
        } else if (mSunroofFrontGlassStatus == SunroofFrontGlassStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontGlassOpenValue(value);
    }

    // IMG_VehicleViewSunroofFrontBlindOpen_stat
    void updateConstantVehicleViewSunroofFrontBlindOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenStat::CLOSE;
        HUInt64 value = 0;
        if (mSunroofFrontGlassStatus == SunroofFrontGlassStatus::NORMAL &&
            (mSunroofFrontBlindOpenPositionValue >= 0 && mSunroofFrontBlindOpenPositionValue <= 100)) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenStat::OPEN;
            value = mSunroofFrontBlindOpenPositionValue;
        } else if (mSunroofFrontGlassStatus == SunroofFrontGlassStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofFrontBlindOpenValue(value);
    }

    // IMG_VehicleViewSunroofRearGlassOpen_stat
    void updateConstantVehicleViewSunroofRearGlassOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenStat::CLOSE;
        HUInt64 value = 0;
        if (mSunroofRearGlassStatus == SunroofRearGlassStatus::NORMAL &&
            (mSunroofRearGlassOpenPositionValue >= 0 && mSunroofRearGlassOpenPositionValue <= 100)) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenStat::OPEN;
            value = mSunroofRearGlassOpenPositionValue;
        } else if (mSunroofRearGlassStatus == SunroofRearGlassStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearGlassOpenValue(value);
    }

    // IMG_VehicleViewSunroofRearBlindOpen_stat
    void updateConstantVehicleViewSunroofRearBlindOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenStat::CLOSE;
        HUInt64 value = 0;
        if (mSunroofRearGlassStatus == SunroofRearGlassStatus::NORMAL &&
            (mSunroofRearBlindOpenPositionValue >= 0 && mSunroofRearBlindOpenPositionValue <= 100)) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenStat::OPEN;
            value = mSunroofRearBlindOpenPositionValue;
        } else if (mSunroofRearGlassStatus == SunroofRearGlassStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewSunroofRearBlindOpenValue(value);
    }

    // 5.2.2.7 Hood(Frunk 포함) Open Status
    // IMG_VehicleViewHoodOpen_stat
    void updateConstantVehicleViewHoodOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat::CLOSE;

        if (mElecHoodOptionStatus == ElecHoodOptionStatus::NOT_APPLIED) {
            if (mHoodOpenStatus == HoodOpenStatus::OPEN) {
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat::OPEN_FULL;
            }
        } else if (mElecHoodOptionStatus == ElecHoodOptionStatus::APPLIED) {
            if (mFrunkOpenStatus != FrunkOpenStatus::RELEASE_HALF && mFrunkOpenStatus != FrunkOpenStatus::ERROR) {
                if (mElecHoodOpenStatus == ElecHoodOpenStatus::OPEN) {
                    stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat::OPEN_FULL;
                }
            } else if (mFrunkOpenStatus == FrunkOpenStatus::RELEASE_HALF) {
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat::OPEN_HALF;
            } else if (mFrunkOpenStatus == FrunkOpenStatus::ERROR) {
                stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat::ERROR;
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewHoodOpenStat(stat);
    }

    // 5.2.2.8 Window Open Status
    // IMG_VehicleViewWindowDriverOpen_stat
    void updateConstantVehicleViewWindowDriverOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenStat::NONE;
        HUInt64 value = 0;

        if ((mWindowDriverOpenPositionValue >= 0 && mWindowDriverOpenPositionValue <= 100) &&
            mWindowDriverErrorStatus != WindowDriverErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenStat::OPEN;
            value = mWindowDriverOpenPositionValue;
        } else if (mWindowDriverErrorStatus == WindowDriverErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowDriverOpenValue(value);
    }

    // IMG_VehicleViewWindowAssistOpen_stat
    void updateConstantVehicleViewWindowAssistOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenStat::NONE;
        HUInt64 value = 0;

        if ((mWindowAssistOpenPositionValue >= 0 && mWindowAssistOpenPositionValue <= 100) &&
            mWindowAssistErrorStatus != WindowAssistErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenStat::OPEN;
            value = mWindowAssistOpenPositionValue;
        } else if (mWindowAssistErrorStatus == WindowAssistErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowAssistOpenValue(value);
    }

    // IMG_VehicleViewWindowRearLeftOpen_stat
    void updateConstantVehicleViewWindowRearLeftOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenStat::NONE;
        HUInt64 value = 0;

        if ((mWindowRearLeftOpenPositionValue >= 0 && mWindowRearLeftOpenPositionValue <= 100) &&
            mWindowRearLeftErrorStatus != WindowRearLeftErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenStat::OPEN;
            value = mWindowRearLeftOpenPositionValue;
        } else if (mWindowRearLeftErrorStatus == WindowRearLeftErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearLeftOpenValue(value);
    }

    // IMG_VehicleViewWindowRearRightOpen_stat
    void updateConstantVehicleViewWindowRearRightOpen() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenStat::NONE;
        HUInt64 value = 0;

        if ((mWindowRearRightOpenPositionValue >= 0 && mWindowRearRightOpenPositionValue <= 100) &&
            mWindowRearRightErrorStatus != WindowRearRightErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenStat::OPEN;
            value = mWindowRearRightOpenPositionValue;
        } else if (mWindowRearRightErrorStatus == WindowRearRightErrorStatus::ERROR) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenStat::ERROR;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenStat(stat);
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowRearRightOpenValue(value);
    }

    // IMG_VehicleViewWindowOpenDispEnable_stat
    void updateConstantVehicleViewWindowOpenDispEnable() {
        SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowOpenDispEnableStat stat =
            SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowOpenDispEnableStat::NONE;

        if (mConfigWindowOpenDisplayOption == ConfigWindowOpenDisplayOption::APPLIED) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowOpenDispEnableStat::ENABLE;
        } else if (mConfigWindowOpenDisplayOption == ConfigWindowOpenDisplayOption::NOT_APPLIED) {
            stat = SFCConvenience_Device_Control_ScreenConstantVehicleViewWindowOpenDispEnableStat::DISABLE;
        } else {
            // no operation
        }
        setSFCConvenience_Device_Control_ScreenConstantVehicleViewWindowOpenDispEnableStat(stat);
    }

    // 5.3 이벤트 표시 동작 로직: N/A
    // 5.4 사운드 로직: N/A

    // Inter_Config variables
    using ConfigBdcPlatform = ArgumentsConfigBdcPlatformChanged::Inter_ConfigBdcPlatform;
    using ConfigSBCM = ArgumentsConfigSBCMChanged::Inter_ConfigSBCM;
    using ConfigTrunkTailgateMenu = ArgumentsConfigTrunkTailgateMenuChanged::Inter_ConfigTrunkTailgateMenu;
    using ConfigFrunkMenu = ArgumentsConfigFrunkMenuChanged::Inter_ConfigFrunkMenu;
    using ConfigChargingPortOpenMenu = ArgumentsConfigChargingPortOpenMenuChanged::Inter_ConfigChargingPortOpenMenu;
    using ConfigSideMirrorMenu = ArgumentsConfigSideMirrorMenuChanged::Inter_ConfigSideMirrorMenu;
    using ConfigSunroofSmartGlassOption = ArgumentsConfigSunroofSmartGlassOptionChanged::Inter_ConfigSunroofSmartGlassOption;

    ConfigBdcPlatform mConfigBdcPlatform = ConfigBdcPlatform::BDC_SECOND;
    ConfigSBCM mConfigSBCM = ConfigSBCM::OFF;
    ConfigTrunkTailgateMenu mConfigTrunkTailgateMenu = ConfigTrunkTailgateMenu::NOT_APPLIED;
    ConfigFrunkMenu mConfigFrunkMenu = ConfigFrunkMenu::NOT_APPLIED;
    ConfigChargingPortOpenMenu mConfigChargingPortOpenMenu = ConfigChargingPortOpenMenu::NOT_APPLIED;
    ConfigSideMirrorMenu mConfigSideMirrorMenu = ConfigSideMirrorMenu::NOT_APPLIED;
    ConfigSunroofSmartGlassOption mConfigSunroofSmartGlassOption = ConfigSunroofSmartGlassOption::NOT_APPLIED;

    // Input_Vsm variables
    using TrunkTailgateMotorOpenStatusFromBdc2nd =
        ArgumentsTrunkTailgateMotorOpenStatusFromBdc2ndChanged::Input_TrunkTailgateMotorOpenStatusFromBdc2nd;
    using TrunkTailgateMotorStatusFromBdc3rd =
        ArgumentsTrunkTailgateMotorStatusFromBdc3rdChanged::Input_TrunkTailgateMotorStatusFromBdc3rd;
    using TrunkTailgateOpenStatusFromBdc3rd =
        ArgumentsTrunkTailgateOpenStatusFromBdc3rdChanged::Input_TrunkTailgateOpenStatusFromBdc3rd;
    using DoorLockDriverSideStatusFromSBCM =
        ArgumentsDoorLockDriverSideStatusFromSBCMChanged::Input_DoorLockDriverSideStatusFromSBCM;
    using DoorLockAssistSideStatusFromSBCM =
        ArgumentsDoorLockAssistSideStatusFromSBCMChanged::Input_DoorLockAssistSideStatusFromSBCM;
    using DoorLockRearLeftSideStatusFromSBCM =
        ArgumentsDoorLockRearLeftSideStatusFromSBCMChanged::Input_DoorLockRearLeftSideStatusFromSBCM;
    using DoorLockRearRightSideStatusFromSBCM =
        ArgumentsDoorLockRearRightSideStatusFromSBCMChanged::Input_DoorLockRearRightSideStatusFromSBCM;
    using DoorLockDriverSideStatus = ArgumentsDoorLockDriverSideStatusChanged::Input_DoorLockDriverSideStatus;
    using DoorLockAssistSideStatus = ArgumentsDoorLockAssistSideStatusChanged::Input_DoorLockAssistSideStatus;
    using DoorLockRearLeftSideStatus = ArgumentsDoorLockRearLeftSideStatusChanged::Input_DoorLockRearLeftSideStatus;
    using DoorLockRearRightSideStatus = ArgumentsDoorLockRearRightSideStatusChanged::Input_DoorLockRearRightSideStatus;
    using PtgmOnOffStatus = ArgumentsPtgmOnOffStatusChanged::Input_PtgmOnOffStatus;
    using PtgmFromBdcOnOffStatus = ArgumentsPtgmFromBdcOnOffStatusChanged::Input_PtgmFromBdcOnOffStatus;
    using PtgmOptionInfoStatus = ArgumentsPtgmOptionInfoStatusChanged::Input_PtgmOptionInfoStatus;
    using TrunkOpenStatus = ArgumentsTrunkOpenStatusChanged::Input_TrunkOpenStatus;
    using EvChargeDoorOpenStatus = ArgumentsEvChargeDoorOpenStatusChanged::Input_EvChargeDoorOpenStatus;
    using EvChargeSecondDoorOpenStatus = ArgumentsEvChargeSecondDoorOpenStatusChanged::Input_EvChargeSecondDoorOpenStatus;
    using OutMirrorFoldStatusFromBCM = ArgumentsOutMirrorFoldStatusFromBCMChanged::Input_OutMirrorFoldStatusFromBCM;
    using OutMirrorUnfoldStatusFromBCM = ArgumentsOutMirrorUnfoldStatusFromBCMChanged::Input_OutMirrorUnfoldStatusFromBCM;
    using BrakeLampControlCommand_Status = ArgumentsBrakeLampControlCommand_StatusChanged::Input_BrakeLampControlCommand_Status;
    using EvFastChargeFailWarnStatusFromVCMS =
        ArgumentsEvFastChargeFailWarnStatusFromVCMSChanged::Input_EvFastChargeFailWarnStatusFromVCMS;
    using SmartRoofGlassZone1OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone1OnOffStatus;
    using SmartRoofGlassZone2OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone2OnOffStatus;
    using SmartRoofGlassZone3OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone3OnOffStatus;
    using SmartRoofGlassZone4OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone4OnOffStatus;
    using SmartRoofGlassZone5OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone5OnOffStatus;
    using SmartRoofGlassZone6OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone6OnOffStatus;
    using SmartRoofGlassZone7OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone7OnOffStatus;
    using SmartRoofGlassZone8OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone8OnOffStatus;
    using SmartRoofGlassZone9OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone9OnOffStatus;
    using SmartRoofGlassZone10OnOffStatus = ArgumentsSmartRoofGlassZoneOnOffStatusChanged::Input_SmartRoofGlassZone10OnOffStatus;
    using SmartRoofGlassZone1FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone1FaultStatus;
    using SmartRoofGlassZone2FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone2FaultStatus;
    using SmartRoofGlassZone3FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone3FaultStatus;
    using SmartRoofGlassZone4FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone4FaultStatus;
    using SmartRoofGlassZone5FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone5FaultStatus;
    using SmartRoofGlassZone6FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone6FaultStatus;
    using SmartRoofGlassZone7FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone7FaultStatus;
    using SmartRoofGlassZone8FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone8FaultStatus;
    using SmartRoofGlassZone9FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone9FaultStatus;
    using SmartRoofGlassZone10FaultStatus = ArgumentsSmartRoofGlassZoneFaultStatusChanged::Input_SmartRoofGlassZone10FaultStatus;
    using SunroofFrontGlassStatus = ArgumentsSunroofFrontGlassStatusChanged::Input_SunroofFrontGlassStatus;
    using SunroofRearGlassStatus = ArgumentsSunroofRearGlassStatusChanged::Input_SunroofRearGlassStatus;
    using FrunkOpenControlDisableStatus1 = ArgumentsFrunkOpenControlDisableStatusChanged::Input_FrunkOpenControlDisableStatus1;
    using FrunkOpenControlDisableStatus2 = ArgumentsFrunkOpenControlDisableStatusChanged::Input_FrunkOpenControlDisableStatus2;
    using ElecHoodOptionStatus = ArgumentsHoodOpenStatusChanged::Input_ElecHoodOptionStatus;
    using HoodOpenStatus = ArgumentsHoodOpenStatusChanged::Input_HoodOpenStatus;
    using ElecHoodOpenStatus = ArgumentsHoodOpenStatusChanged::Input_ElecHoodOpenStatus;
    using FrunkOpenStatus = ArgumentsHoodOpenStatusChanged::Input_FrunkOpenStatus;
    using WindowDriverErrorStatus = ArgumentsWindowDriverOpenPositionValueChanged::Input_WindowDriverErrorStatus;
    using WindowAssistErrorStatus = ArgumentsWindowAssistOpenPositionValueChanged::Input_WindowAssistErrorStatus;
    using WindowRearLeftErrorStatus = ArgumentsWindowRearLeftOpenPositionValueChanged::Input_WindowRearLeftErrorStatus;
    using WindowRearRightErrorStatus = ArgumentsWindowRearRightOpenPositionValueChanged::Input_WindowRearRightErrorStatus;
    using ConfigWindowOpenDisplayOption = ArgumentsConfigWindowOpenDisplayOptionChanged::Inter_ConfigWindowOpenDisplayOption;

    TrunkTailgateMotorOpenStatusFromBdc2nd mTrunkTailgateMotorOpenStatusFromBdc2nd =
        TrunkTailgateMotorOpenStatusFromBdc2nd::CLOSE;
    TrunkTailgateMotorStatusFromBdc3rd mTrunkTailgateMotorStatusFromBdc3rd = TrunkTailgateMotorStatusFromBdc3rd::OFF;
    TrunkTailgateOpenStatusFromBdc3rd mTrunkTailgateOpenStatusFromBdc3rd = TrunkTailgateOpenStatusFromBdc3rd::OFF;
    DoorLockDriverSideStatusFromSBCM mDoorLockDriverSideStatusFromSBCM = DoorLockDriverSideStatusFromSBCM::UNLOCK;
    DoorLockAssistSideStatusFromSBCM mDoorLockAssistSideStatusFromSBCM = DoorLockAssistSideStatusFromSBCM::UNLOCK;
    DoorLockRearLeftSideStatusFromSBCM mDoorLockRearLeftSideStatusFromSBCM = DoorLockRearLeftSideStatusFromSBCM::UNLOCK;
    DoorLockRearRightSideStatusFromSBCM mDoorLockRearRightSideStatusFromSBCM = DoorLockRearRightSideStatusFromSBCM::UNLOCK;
    DoorLockDriverSideStatus mDoorLockDriverSideStatus = DoorLockDriverSideStatus::UNLOCK;
    DoorLockAssistSideStatus mDoorLockAssistSideStatus = DoorLockAssistSideStatus::UNLOCK;
    DoorLockRearLeftSideStatus mDoorLockRearLeftSideStatus = DoorLockRearLeftSideStatus::UNLOCK;
    DoorLockRearRightSideStatus mDoorLockRearRightSideStatus = DoorLockRearRightSideStatus::UNLOCK;
    PtgmOnOffStatus mPtgmOnOffStatus = PtgmOnOffStatus::NON_OPTION;
    PtgmFromBdcOnOffStatus mPtgmFromBdcOnOffStatus = PtgmFromBdcOnOffStatus::NON_OPTION;
    PtgmOptionInfoStatus mPtgmOptionInfoStatus = PtgmOptionInfoStatus::NON_OPTION;
    HUInt64 mPowerTrunkOpenPositionValue = 0;
    TrunkOpenStatus mTrunkOpenStatus = TrunkOpenStatus::CLOSE;
    EvChargeDoorOpenStatus mEvChargeDoorOpenStatus = EvChargeDoorOpenStatus::CLOSE;
    EvChargeSecondDoorOpenStatus mEvChargeSecondDoorOpenStatus = EvChargeSecondDoorOpenStatus::CLOSE;
    OutMirrorFoldStatusFromBCM mOutMirrorFoldStatusFromBCM = OutMirrorFoldStatusFromBCM::OFF;
    OutMirrorUnfoldStatusFromBCM mOutMirrorUnfoldStatusFromBCM = OutMirrorUnfoldStatusFromBCM::OFF;
    BrakeLampControlCommand_Status mBrakeLampControlCommand_Status = BrakeLampControlCommand_Status::OFF;
    EvFastChargeFailWarnStatusFromVCMS mEvFastChargeFailWarnStatusFromVCMS = EvFastChargeFailWarnStatusFromVCMS::OFF;
    SmartRoofGlassZone1OnOffStatus mSmartRoofGlassZone1OnOffStatus = SmartRoofGlassZone1OnOffStatus::OFF;
    SmartRoofGlassZone2OnOffStatus mSmartRoofGlassZone2OnOffStatus = SmartRoofGlassZone2OnOffStatus::OFF;
    SmartRoofGlassZone3OnOffStatus mSmartRoofGlassZone3OnOffStatus = SmartRoofGlassZone3OnOffStatus::OFF;
    SmartRoofGlassZone4OnOffStatus mSmartRoofGlassZone4OnOffStatus = SmartRoofGlassZone4OnOffStatus::OFF;
    SmartRoofGlassZone5OnOffStatus mSmartRoofGlassZone5OnOffStatus = SmartRoofGlassZone5OnOffStatus::OFF;
    SmartRoofGlassZone6OnOffStatus mSmartRoofGlassZone6OnOffStatus = SmartRoofGlassZone6OnOffStatus::OFF;
    SmartRoofGlassZone7OnOffStatus mSmartRoofGlassZone7OnOffStatus = SmartRoofGlassZone7OnOffStatus::OFF;
    SmartRoofGlassZone8OnOffStatus mSmartRoofGlassZone8OnOffStatus = SmartRoofGlassZone8OnOffStatus::OFF;
    SmartRoofGlassZone9OnOffStatus mSmartRoofGlassZone9OnOffStatus = SmartRoofGlassZone9OnOffStatus::OFF;
    SmartRoofGlassZone10OnOffStatus mSmartRoofGlassZone10OnOffStatus = SmartRoofGlassZone10OnOffStatus::OFF;
    SmartRoofGlassZone1FaultStatus mSmartRoofGlassZone1FaultStatus = SmartRoofGlassZone1FaultStatus::NORMAL;
    SmartRoofGlassZone2FaultStatus mSmartRoofGlassZone2FaultStatus = SmartRoofGlassZone2FaultStatus::NORMAL;
    SmartRoofGlassZone3FaultStatus mSmartRoofGlassZone3FaultStatus = SmartRoofGlassZone3FaultStatus::NORMAL;
    SmartRoofGlassZone4FaultStatus mSmartRoofGlassZone4FaultStatus = SmartRoofGlassZone4FaultStatus::NORMAL;
    SmartRoofGlassZone5FaultStatus mSmartRoofGlassZone5FaultStatus = SmartRoofGlassZone5FaultStatus::NORMAL;
    SmartRoofGlassZone6FaultStatus mSmartRoofGlassZone6FaultStatus = SmartRoofGlassZone6FaultStatus::NORMAL;
    SmartRoofGlassZone7FaultStatus mSmartRoofGlassZone7FaultStatus = SmartRoofGlassZone7FaultStatus::NORMAL;
    SmartRoofGlassZone8FaultStatus mSmartRoofGlassZone8FaultStatus = SmartRoofGlassZone8FaultStatus::NORMAL;
    SmartRoofGlassZone9FaultStatus mSmartRoofGlassZone9FaultStatus = SmartRoofGlassZone9FaultStatus::NORMAL;
    SmartRoofGlassZone10FaultStatus mSmartRoofGlassZone10FaultStatus = SmartRoofGlassZone10FaultStatus::NORMAL;
    HUInt64 mSunroofFrontGlassOpenPositionValue = 0;
    HUInt64 mSunroofFrontBlindOpenPositionValue = 0;
    HUInt64 mSunroofRearGlassOpenPositionValue = 0;
    HUInt64 mSunroofRearBlindOpenPositionValue = 0;
    SunroofFrontGlassStatus mSunroofFrontGlassStatus = SunroofFrontGlassStatus::NORMAL;
    SunroofRearGlassStatus mSunroofRearGlassStatus = SunroofRearGlassStatus::NORMAL;
    FrunkOpenControlDisableStatus1 mFrunkOpenControlDisableStatus1 = FrunkOpenControlDisableStatus1::OFF;
    FrunkOpenControlDisableStatus2 mFrunkOpenControlDisableStatus2 = FrunkOpenControlDisableStatus2::OFF;
    ElecHoodOptionStatus mElecHoodOptionStatus = ElecHoodOptionStatus::NOT_APPLIED;
    HoodOpenStatus mHoodOpenStatus = HoodOpenStatus::CLOSE;
    ElecHoodOpenStatus mElecHoodOpenStatus = ElecHoodOpenStatus::CLOSE;
    FrunkOpenStatus mFrunkOpenStatus = FrunkOpenStatus::OFF;
    WindowDriverErrorStatus mWindowDriverErrorStatus = WindowDriverErrorStatus::NORMAL;
    WindowAssistErrorStatus mWindowAssistErrorStatus = WindowAssistErrorStatus::NORMAL;
    WindowRearLeftErrorStatus mWindowRearLeftErrorStatus = WindowRearLeftErrorStatus::NORMAL;
    WindowRearRightErrorStatus mWindowRearRightErrorStatus = WindowRearRightErrorStatus::NORMAL;
    HUInt64 mWindowDriverOpenPositionValue = 0;
    HUInt64 mWindowAssistOpenPositionValue = 0;
    HUInt64 mWindowRearLeftOpenPositionValue = 0;
    HUInt64 mWindowRearRightOpenPositionValue = 0;
    ConfigWindowOpenDisplayOption mConfigWindowOpenDisplayOption = ConfigWindowOpenDisplayOption::NOT_APPLIED;

    // Constant variables
    static constexpr HUInt64 kPowerTrunkOpenMaxPosition = 100;

    // Input_SFC variables
    using TelltaleEvChgCableStat = ArgumentsTelltaleEvChgCableStatChanged::EvChgCableStat;

    TelltaleEvChgCableStat mTelltaleEvChgCableStat = TelltaleEvChgCableStat::OFF;

    // Private variables
    enum class Private_TrunkTailgateOpenControlStatus : HUInt64 {
        NONE,
        CLOSING,
        OPENING,
        CLOSE,
        OPEN,
        MAX
    };

    Private_TrunkTailgateOpenControlStatus mPrivate_TrunkTailgateOpenControlStatus =
        Private_TrunkTailgateOpenControlStatus::CLOSE;
};

}  // namespace ccos

#endif  // SFSS_Convenience_Device_Control_Screen_H
