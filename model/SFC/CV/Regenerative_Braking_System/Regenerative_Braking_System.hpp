/**
 * @file Regenerative_Braking_System.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_Regenerative_Braking_System_H
#define SFSS_Regenerative_Braking_System_H

#define DLOG_ENABLED gEnableSFCLog

#include "Regenerative_Braking_SystemBase.hpp"

namespace ccos {

// SFC Version : 6.0.0
// Spec Version : v0.30
// Reference : [CV960] Regenerative_Braking_System. Contains : Telltale, Contant, Event. Recommend: VALUE_CHANGED
class Regenerative_Braking_System : public Regenerative_Braking_SystemBase {
public:
    Regenerative_Braking_System() = default;
    ~Regenerative_Braking_System() override = default;
    Regenerative_Braking_System(const Regenerative_Braking_System& other) = delete;
    Regenerative_Braking_System(Regenerative_Braking_System&& other) noexcept = delete;
    Regenerative_Braking_System& operator=(const Regenerative_Braking_System& other) = delete;
    Regenerative_Braking_System& operator=(Regenerative_Braking_System&& other) noexcept = delete;

    void onInitialize() override {
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverID("E79601");
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverLinkedSoundID("SND_PopUpWarn1");
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverLinkedSoundType(
            SFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverLinkedSoundType::REPEAT_COUNT);
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverLinkedSoundRepeatCount(1);

        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCID("E79602");
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCLinkedSoundID("SND_PopUpWarn1");
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCLinkedSoundType(
            SFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCLinkedSoundType::REPEAT_COUNT);
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCLinkedSoundRepeatCount(1);

        setSFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaID("E79603");
        setSFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaLinkedSoundID("SND_PopUpWarn1");
        setSFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaLinkedSoundType(
            SFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaLinkedSoundType::REPEAT_COUNT);
        setSFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaLinkedSoundRepeatCount(1);

        setSFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaID("E79604");
        setSFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaLinkedSoundID("SND_PopUpInform1");
        setSFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaLinkedSoundType(
            SFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaLinkedSoundType::REPEAT_COUNT);
        setSFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaLinkedSoundRepeatCount(1);
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

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;

        updateTelltaleRegenBrake();
        updateTelltaleRegenBrakeNA();
    }

    void onConfigSRSChanged(const ArgumentsConfigSRSChanged& args) {
        mConfigSRS = args.mInter_ConfigSRS;

        updateTelltaleRegenBrake();
        updateTelltaleRegenBrakeNA();
        updateContantSmartRegenBrakeGaugeTypeStat();
        updateContantSmartRegenBrakeStat();
        updateContantSmartRegenBrakeGagueValue();
    }

    void onRegenBrakeStatusChanged(const ArgumentsRegenBrakeStatusChanged& args) {
        mRegenBrakeStatus = args.mInput_RegenBrakeStatus;

        updateTelltaleRegenBrake();
    }

    void onRegenLimitStatus_LeverChanged(const ArgumentsRegenLimitStatus_LeverChanged& args) {
        mRegenLimitStatus_Lever = args.mInput_RegenLimitStatus_Lever;

        updateEventRegenLimitStatus_Lever();
    }

    void onRegenLimitStatus_SOCChanged(const ArgumentsRegenLimitStatus_SOCChanged& args) {
        mRegenLimitStatus_SOC = args.mInput_RegenLimitStatus_SOC;

        updateEventRegenLimitStatus_SOC();
    }

    void onVcu_SmartRegen_WarnMsgStaChanged(const ArgumentsVcu_SmartRegen_WarnMsgStaChanged& args) {
        mVCU_SmartRegen_WarnMsgSta = args.mInput_VCU_SmartRegen_WarnMsgSta;

        updateEventVCU_SmartRegen_WarnMsgStaStat();
    }

    void onVcu_PdlWarnMsgStaChanged(const ArgumentsVcu_PdlWarnMsgStaChanged& args) {
        mVCU_PdlWarnMsgSta = args.mInput_VCU_PdlWarnMsgSta;

        updateEventVCU_PdlWarnMsgStaStat();
    }

    void onVcu_SmartRegen_OnStaChanged(const ArgumentsVcu_SmartRegen_OnStaChanged& args) {
        mVCU_SmartRegen_OnSta = args.mInput_VCU_SmartRegen_OnSta;

        updateContantSmartRegenBrakeGaugeTypeStat();
        updateContantSmartRegenBrakeStat();
        updateContantSmartRegenBrakeGagueValue();
    }

    void onVcu_RegenLvlValChanged(const ArgumentsVcu_RegenLvlValChanged& args) {
        mVCU_RegenLvlVal = args.mInput_VCU_RegenLvlVal;

        updateContantSmartRegenBrakeStat();
        updateContantSmartRegenBrakeGagueValue();
    }

    void onVcu_SmartRegen_RdrCstActStaChanged(const ArgumentsVcu_SmartRegen_RdrCstActStaChanged& args) {
        mVCU_SmartRegen_RdrCstActSta = args.mInput_VCU_SmartRegen_RdrCstActSta;

        updateContantSmartRegenBrakeStat();
    }

    void onVcu_SmartRegen_MapCstActStaChanged(const ArgumentsVcu_SmartRegen_MapCstActStaChanged& args) {
        mVCU_SmartRegen_MapCstActSta = args.mInput_VCU_SmartRegen_MapCstActSta;

        updateContantSmartRegenBrakeStat();
    }

    void onRetarderSwitchStatusChanged(const ArgumentsRetarderSwitchStatusChanged& args) {
        mRetarderSwitchStatus = args.mInput_RetarderSwitchStatus;

        updateTelltaleRegenBrakeNA();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateAll() {
        updateTelltaleRegenBrake();
        updateTelltaleRegenBrakeNA();
        updateEventRegenLimitStatus_Lever();
        updateEventRegenLimitStatus_SOC();
        updateEventVCU_SmartRegen_WarnMsgStaStat();
        updateEventVCU_PdlWarnMsgStaStat();
        updateContantSmartRegenBrakeGaugeTypeStat();
        updateContantSmartRegenBrakeStat();
        updateContantSmartRegenBrakeGagueValue();
    }

    void updateTelltaleRegenBrake() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemTelltaleRegenBrakeStat stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStat::OFF;
        SFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional statOpt =
            SFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional::OFF;

        if (mIsIgnOn == true && mConfigArea != ConfigArea::USA && mConfigSRS == ConfigSRS::OFF) {
            if (mRegenBrakeStatus == RegenBrakeStatus::READY) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional::READY;
            } else if (mRegenBrakeStatus == RegenBrakeStatus::STEP1) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional::STEP1;
            } else if (mRegenBrakeStatus == RegenBrakeStatus::STEP2) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional::STEP2;
            } else {
                // no operation
            }
        }
        setSFCRegenerative_Braking_SystemTelltaleRegenBrakeStat(stat);
        setSFCRegenerative_Braking_SystemTelltaleRegenBrakeStatOptional(statOpt);
    }

    void updateTelltaleRegenBrakeNA() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OFF;
        SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional statOpt =
            SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::OFF;

        if (mIsIgnOn == true && mConfigArea == ConfigArea::USA && mConfigSRS == ConfigSRS::OFF) {
            if (mRetarderSwitchStatus == RetarderSwitchStatus::READY) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::READY;
            } else if (mRetarderSwitchStatus == RetarderSwitchStatus::STEP1) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::AUTO;
            } else if (mRetarderSwitchStatus == RetarderSwitchStatus::STEP2) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::STEP1;
            } else if (mRetarderSwitchStatus == RetarderSwitchStatus::STEP3) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::STEP2;
            } else if (mRetarderSwitchStatus == RetarderSwitchStatus::STEP4) {
                stat = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat::OPTIONAL;
                statOpt = SFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional::STEP3;
            } else {
                // no operation
            }
        }
        setSFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStat(stat);
        setSFCRegenerative_Braking_SystemTelltaleRegenBrakeNAStatOptional(statOpt);
    }

    void updateContantSmartRegenBrakeStat() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat stat =
            SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::OFF;

        if (mIsIgnOn == true && mConfigSRS == ConfigSRS::ON) {
            if (mVCU_SmartRegen_OnSta == VCU_SmartRegen_OnSta::AUTO) {
                if (mVCU_SmartRegen_RdrCstActSta == VCU_SmartRegen_RdrCstActSta::ACTIVE ||
                    mVCU_SmartRegen_MapCstActSta == VCU_SmartRegen_MapCstActSta::ACTIVE) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::AUTO_ACT;
                } else if (mVCU_SmartRegen_RdrCstActSta == VCU_SmartRegen_RdrCstActSta::INACTIVE &&
                           mVCU_SmartRegen_MapCstActSta == VCU_SmartRegen_MapCstActSta::INACTIVE) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::AUTO_INACT;
                } else {
                    // no operation
                }
            } else if (mVCU_SmartRegen_OnSta == VCU_SmartRegen_OnSta::MANUAL) {
                if (mVCU_RegenLvlVal >= 0x0 && mVCU_RegenLvlVal <= 0x09) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::LV0;
                } else if (mVCU_RegenLvlVal >= 0xA && mVCU_RegenLvlVal <= 0x13) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::LV1;
                } else if (mVCU_RegenLvlVal >= 0x14 && mVCU_RegenLvlVal <= 0x1D) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::LV2;
                } else if (mVCU_RegenLvlVal >= 0x1E && mVCU_RegenLvlVal <= 0x27) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::LV3;
                } else if (mVCU_RegenLvlVal == 0x28) {
                    stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeStat::LV4;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
        setSFCRegenerative_Braking_SystemContantSmartRegenBrakeStat(stat);
    }

    void updateContantSmartRegenBrakeGaugeTypeStat() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemContantSmartRegenBrakeGaugeTypeStat stat =
            SFCRegenerative_Braking_SystemContantSmartRegenBrakeGaugeTypeStat::OFF;

        if (mIsIgnOn == true && mConfigSRS == ConfigSRS::ON) {
            if (mVCU_SmartRegen_OnSta == VCU_SmartRegen_OnSta::MANUAL) {
                stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeGaugeTypeStat::STEP;
            } else if (mVCU_SmartRegen_OnSta == VCU_SmartRegen_OnSta::AUTO) {
                stat = SFCRegenerative_Braking_SystemContantSmartRegenBrakeGaugeTypeStat::LINEAR;
            } else {
                // no operation
            }
        }
        setSFCRegenerative_Braking_SystemContantSmartRegenBrakeGaugeTypeStat(stat);
    }

    void updateContantSmartRegenBrakeGagueValue() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        HDouble value = static_cast<HDouble>(0xFF);

        if (mIsIgnOn == true && mConfigSRS == ConfigSRS::ON && mVCU_SmartRegen_OnSta == VCU_SmartRegen_OnSta::AUTO &&
            mVCU_RegenLvlVal >= 0x0 && mVCU_RegenLvlVal <= 0x28) {
            value = static_cast<HDouble>(mVCU_RegenLvlVal) * 2.5;
        }
        setSFCRegenerative_Braking_SystemContantSmartRegenBrakeGagueValue(value);
    }

    void updateEventRegenLimitStatus_Lever() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverStat stat =
            SFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverStat::OFF;

        if (mIsIgnOn == true && mRegenLimitStatus_Lever == RegenLimitStatus_Lever::LIMIT) {
            stat = SFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverStat::ON;
        }
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_LeverStat(stat);
    }

    void updateEventRegenLimitStatus_SOC() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCStat stat =
            SFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCStat::OFF;

        if (mIsIgnOn == true && mRegenLimitStatus_SOC == RegenLimitStatus_SOC::LIMIT) {
            stat = SFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCStat::ON;
        }
        setSFCRegenerative_Braking_SystemEventRegenLimitStatus_SOCStat(stat);
    }

    void updateEventVCU_SmartRegen_WarnMsgStaStat() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaStat stat =
            SFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaStat::OFF;

        if (mIsIgnOn == true && mVCU_SmartRegen_WarnMsgSta == VCU_SmartRegen_WarnMsgSta::ON) {
            stat = SFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaStat::ON;
        }
        setSFCRegenerative_Braking_SystemEventVCU_SmartRegen_WarnMsgStaStat(stat);
    }

    void updateEventVCU_PdlWarnMsgStaStat() {
        if (mConfigVehicleType != ConfigVehicleType::FCEV && mConfigVehicleType != ConfigVehicleType::EV) {
            return;
        }
        SFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaStat stat =
            SFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaStat::OFF;

        if (mIsIgnOn == true && mVCU_PdlWarnMsgSta == VCU_PdlWarnMsgSta::ON) {
            stat = SFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaStat::ON;
        }
        setSFCRegenerative_Braking_SystemEventVCU_PdlWarnMsgStaStat(stat);
    }

    using RegenBrakeStatus = ArgumentsRegenBrakeStatusChanged::Input_RegenBrakeStatus;
    using RegenLimitStatus_Lever = ArgumentsRegenLimitStatus_LeverChanged::Input_RegenLimitStatus_Lever;
    using RegenLimitStatus_SOC = ArgumentsRegenLimitStatus_SOCChanged::Input_RegenLimitStatus_SOC;
    using ConfigVehicleType = ArgumentsConfigVehicleTypeChanged::Inter_ConfigVehicleType;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;
    using ConfigSRS = ArgumentsConfigSRSChanged::Inter_ConfigSRS;
    using VCU_SmartRegen_OnSta = ArgumentsVcu_SmartRegen_OnStaChanged::Input_VCU_SmartRegen_OnSta;
    using VCU_SmartRegen_RdrCstActSta = ArgumentsVcu_SmartRegen_RdrCstActStaChanged::Input_VCU_SmartRegen_RdrCstActSta;
    using VCU_SmartRegen_MapCstActSta = ArgumentsVcu_SmartRegen_MapCstActStaChanged::Input_VCU_SmartRegen_MapCstActSta;
    using VCU_SmartRegen_WarnMsgSta = ArgumentsVcu_SmartRegen_WarnMsgStaChanged::Input_VCU_SmartRegen_WarnMsgSta;
    using VCU_PdlWarnMsgSta = ArgumentsVcu_PdlWarnMsgStaChanged::Input_VCU_PdlWarnMsgSta;
    using RetarderSwitchStatus = ArgumentsRetarderSwitchStatusChanged::Input_RetarderSwitchStatus;

    HBool mIsIgnOn = false;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigArea mConfigArea = ConfigArea::DOM;
    ConfigSRS mConfigSRS = ConfigSRS::OFF;
    RegenBrakeStatus mRegenBrakeStatus = RegenBrakeStatus::OFF;
    RegenLimitStatus_Lever mRegenLimitStatus_Lever = RegenLimitStatus_Lever::OFF;
    RegenLimitStatus_SOC mRegenLimitStatus_SOC = RegenLimitStatus_SOC::OFF;
    VCU_SmartRegen_OnSta mVCU_SmartRegen_OnSta = VCU_SmartRegen_OnSta::OFF;
    VCU_SmartRegen_RdrCstActSta mVCU_SmartRegen_RdrCstActSta = VCU_SmartRegen_RdrCstActSta::OFF;
    VCU_SmartRegen_MapCstActSta mVCU_SmartRegen_MapCstActSta = VCU_SmartRegen_MapCstActSta::OFF;
    VCU_SmartRegen_WarnMsgSta mVCU_SmartRegen_WarnMsgSta = VCU_SmartRegen_WarnMsgSta::OFF;
    VCU_PdlWarnMsgSta mVCU_PdlWarnMsgSta = VCU_PdlWarnMsgSta::OFF;
    HUInt64 mVCU_RegenLvlVal = 0;
    RetarderSwitchStatus mRetarderSwitchStatus = RetarderSwitchStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Regenerative_Braking_System_H
