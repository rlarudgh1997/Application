/**
 * @file Set_Speed.hpp
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
#ifndef SFSS_Set_Speed_H
#define SFSS_Set_Speed_H

#define DLOG_ENABLED gEnableSFCLog

#include "Set_SpeedBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.34
// Reference : [CV600] Set_Speed. Contains : Constant. Recommend: VALUE_CHANGED
class Set_Speed : public Set_SpeedBase {
public:
    Set_Speed() = default;
    ~Set_Speed() override = default;
    Set_Speed(const Set_Speed& other) = delete;
    Set_Speed(Set_Speed&& other) noexcept = delete;
    Set_Speed& operator=(const Set_Speed& other) = delete;
    Set_Speed& operator=(Set_Speed&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        if (updateValueIfValid(mIsAemInitComplete, args.mInter_AEMInitializeComplete) == true) {
            _DInfo() << __FUNCTION__ << " AEM Initialize Complete!!!";
        } else {
            _DInfo() << __FUNCTION__ << " AEM is Not Initialize";
        }
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIsIgnOn = true;
        updatePrivateConstantPositionStat();
        updateConstantSetSpeedColor();
        updatePrivateConstantMinToleranceSpeedStat();
        updatePrivateConstantMaxToleranceSpeedStat();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIsIgnOn = false;
        updatePrivateConstantPositionStat();
        updateConstantSetSpeedColor();
        updatePrivateConstantMinToleranceSpeedStat();
        updatePrivateConstantMaxToleranceSpeedStat();
    }

    void onEngineCcSmvStatusChanged(const ArgumentsEngineCcSmvStatusChanged& args) {
        mEngineCcActive = args.mInput_EngineCcActive;
        mSmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        updatePrivateConstantPositionStat();
        updateConstantSetSpeedColor();
        updatePrivateConstantMinToleranceSpeedStat();
        updatePrivateConstantMaxToleranceSpeedStat();
    }

    void onSrsRetarderStatusChanged(const ArgumentsSrsRetarderStatusChanged& args) {
        mSRSActiveStatus = args.mInput_SRSActiveStatus;
        mRetarderSwitchStatus = args.mInput_RetarderSwitchStatus;
        updatePrivateConstantPositionStat();
        updateConstantSetSpeedColor();
    }

    void onEngineCcSmvSetSpeedChanged(const ArgumentsEngineCcSmvSetSpeedChanged& args) {
        mEngineCcSetSpeed = args.mInput_EngineCcSetSpeed;
        mSmvSetSpeedValue = args.mInput_SmvSetSpeedValue;
        updatePrivateConstantPositionStat();
        updatePrivateConstantMinToleranceSpeedStat();
        updatePrivateConstantMaxToleranceSpeedStat();
    }

    void onSrsBremsomatSetSpeedValueChanged(const ArgumentsSrsBremsomatSetSpeedValueChanged& args) {
        mSRSSetSpeedValue = args.mInput_SRSSetSpeedValue;
        mBremsomatSetSpeedValue = args.mInput_BremsomatSetSpeedValue;
        updatePrivateConstantPositionStat();
    }

    void onMccDhcChanged(const ArgumentsMccDhcChanged& args) {
        mMCCStatus = args.mInput_MCCStatus;
        mDHCState = args.mInput_DHCState;
        mMCCMinSpeedValue = args.mInput_MCCMinSpeedValue;
        mDHCToleranceValue = args.mInput_DHCToleranceValue;
        updatePrivateConstantMinToleranceSpeedStat();
        updatePrivateConstantMaxToleranceSpeedStat();
    }

    void onTripDistanceUnitChanged(const ArgumentsTripDistanceUnitChanged& args) {
        mTripDistanceUnit = args.mInter_TripDistanceUnit;
        updateConstantSetSpeedPosition();
        updateConstantMinToleranceSpeed();
        updateConstantMaxToleranceSpeed();
    }

    void onParamChanged(const ArgumentsParamChanged& args) {
        mPar_CCSetSpeedKmTolerance = args.mPar_CCSetSpeedKmTolerance;
        mPar_CCSetSpeedMileTolerance = args.mPar_CCSetSpeedMileTolerance;
        mPar_SRSSetSpeedKmTolerance = args.mPar_SRSSetSpeedKmTolerance;
        mPar_SRSSetSpeedMileTolerance = args.mPar_SRSSetSpeedMileTolerance;
        mPar_BremsomatSetSpeedKmTolerance = args.mPar_BremsomatSetSpeedKmTolerance;
        mPar_BremsomatSetSpeedMileTolerance = args.mPar_BremsomatSetSpeedMileTolerance;

        if (mIsAemInitComplete == true) {
            updateConstantSetSpeedPosition();
            updateConstantMinToleranceSpeed();
            updateConstantMaxToleranceSpeed();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updatePrivateConstantPositionStat() {
        // Default : 0xOFF
        mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::OFF;
        if (mIsIgnOn == true) {
            if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN || mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::SCC_SET;
            } else {
                if (mEngineCcActive == EngineCcActive::ON) {
                    mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::CC_SET;
                } else if (mEngineCcActive == EngineCcActive::OFF && mSRSActiveStatus == SRSActiveStatus::ON) {
                    mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::SRS_SET;
                } else if (mEngineCcActive == EngineCcActive::OFF && mSRSActiveStatus == SRSActiveStatus::OFF &&
                           mRetarderSwitchStatus == RetarderSwitchStatus::STEP1) {
                    mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::BREMSOMAT_SET;
                }
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateSet_SpeedSetSpeedPositionStat(
            static_cast<SFCPrivateSet_SpeedSetSpeedPositionStat>(mPrivateSetSpeedPositionStat));
#endif
        updateConstantSetSpeedPosition();
    }

    void updateConstantSetSpeedPosition() {
        // Default : 0xFFF
        HDouble value = 0xFFF;
        switch (mPrivateSetSpeedPositionStat) {
            case PrivateSetSpeedPositionStat::CC_SET:
                if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) + mPar_CCSetSpeedKmTolerance;
                } else if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) * kFactor_ConvKmToMi + mPar_CCSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            case PrivateSetSpeedPositionStat::SCC_SET:
                if (0x0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 0xFF) {
                    value = static_cast<HDouble>(mSmvSetSpeedValue);
                }
                break;
            case PrivateSetSpeedPositionStat::SRS_SET:
                if (0x0 <= mSRSSetSpeedValue && mSRSSetSpeedValue <= 0xFF && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mSRSSetSpeedValue) + mPar_SRSSetSpeedKmTolerance;
                } else if (0x0 <= mSRSSetSpeedValue && mSRSSetSpeedValue <= 0xFF && mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mSRSSetSpeedValue) * kFactor_ConvKmToMi + mPar_SRSSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            case PrivateSetSpeedPositionStat::BREMSOMAT_SET:
                if (0x0 <= mBremsomatSetSpeedValue && mBremsomatSetSpeedValue <= 0xFF) {
                    value = mBremsomatSetSpeedValue;
                }
                if (0x0 <= mBremsomatSetSpeedValue && mBremsomatSetSpeedValue <= 0xFF &&
                    mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mBremsomatSetSpeedValue) + mPar_BremsomatSetSpeedKmTolerance;
                } else if (0x0 <= mBremsomatSetSpeedValue && mBremsomatSetSpeedValue <= 0xFF &&
                           mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value =
                        static_cast<HDouble>(mBremsomatSetSpeedValue) * kFactor_ConvKmToMi + mPar_BremsomatSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            default:
                // no operation
                break;
        }
        setSFCSet_SpeedConstantSetSpeedPositionValue(static_cast<HUInt64>(value));
    }

    void updateConstantSetSpeedColor() {
        // Default : OFF
        SFCSet_SpeedConstantSetSpeedColorStat stat = SFCSet_SpeedConstantSetSpeedColorStat::OFF;
        if (mIsIgnOn == true) {
            if (mEngineCcActive == EngineCcActive::ON || mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN ||
                mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                // IGN: ON && (EngineCcActive: ON || SmvSetSpeedStatus: GREEN, WHITE)
                stat = SFCSet_SpeedConstantSetSpeedColorStat::GREEN;
            } else if (mSRSActiveStatus == SRSActiveStatus::ON || mRetarderSwitchStatus == RetarderSwitchStatus::STEP1) {
                // IGN: ON && EngineCcActive: OFF && SmvSetSpeedStatus: !(GREEN, WHITE) &&
                // (SRSActiveStatus: ON || RetarderSwitchStatus: STEP1)
                stat = SFCSet_SpeedConstantSetSpeedColorStat::RED;
            } else {
                // no operation
            }
        }
        setSFCSet_SpeedConstantSetSpeedColorStat(stat);
    }

    void updatePrivateConstantMinToleranceSpeedStat() {
        // Default : 0xOFF
        mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::OFF;
        if (mIsIgnOn == true) {
            if (mEngineCcActive == EngineCcActive::ON && mSmvSetSpeedStatus != SmvSetSpeedStatus::GREEN &&
                mSmvSetSpeedStatus != SmvSetSpeedStatus::WHITE) {
                if (mMCCStatus == MCCStatus::ON) {
                    // IGN: ON && EngineCcActive: ON && MCCStatus: ON
                    mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::CC_MCC_ACTIVE;
                } else if (mMCCStatus == MCCStatus::OFF && mDHCState == DHCState::ACTIVE) {
                    // IGN: ON && EngineCcActive: ON && MCCStatus: OFF && DHCState: ACTIVE
                    mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::CC_DHC_ACTIVE;
                } else {
                    // no operation
                }
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN || mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                if (mMCCStatus == MCCStatus::ON) {
                    // IGN: ON && EngineCcActive: OFF && SmvSetSpeedStatus: GREEN, WHITE && MCCStatus: ON
                    mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::SCC_MCC_ACTIVE;
                } else if (mMCCStatus == MCCStatus::OFF && mDHCState == DHCState::ACTIVE) {
                    // IGN: ON && EngineCcActive: OFF && SmvSetSpeedStatus: GREEN, WHITE && MCCStatus: OFF && DHCState: ACTIVE
                    mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::SCC_DHC_ACTIVE;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
#ifdef ENABLE_GCOV_ON
        setSFCPrivateSet_SpeedMinToleranceSpeedStat(
            static_cast<SFCPrivateSet_SpeedMinToleranceSpeedStat>(mPrivateMinToleranceSpeedStat));
#endif
        updateConstantMinToleranceSpeed();
    }

    void updateConstantMinToleranceSpeed() {
        // Default : 0xFFF
        HDouble value = 0xFFF;
        switch (mPrivateMinToleranceSpeedStat) {
            case PrivateMinToleranceSpeedStat::CC_MCC_ACTIVE:
            case PrivateMinToleranceSpeedStat::SCC_MCC_ACTIVE:
                if (0x0 <= mMCCMinSpeedValue && mMCCMinSpeedValue <= 0xFF && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mMCCMinSpeedValue) + mPar_CCSetSpeedKmTolerance;
                } else if (0x0 <= mMCCMinSpeedValue && mMCCMinSpeedValue <= 0xFF && mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mMCCMinSpeedValue) * kFactor_ConvKmToMi + mPar_CCSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            case PrivateMinToleranceSpeedStat::CC_DHC_ACTIVE:
                if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) + mPar_CCSetSpeedKmTolerance;
                } else if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) * kFactor_ConvKmToMi + mPar_CCSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            case PrivateMinToleranceSpeedStat::SCC_DHC_ACTIVE:
                if (0x0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 0xFF) {
                    value = mSmvSetSpeedValue;
                }
                break;
            default:
                // no operation
                break;
        }
        setSFCSet_SpeedConstantMinToleranceSpeedValue(static_cast<HUInt64>(value));
    }

    void updatePrivateConstantMaxToleranceSpeedStat() {
        // Default : 0xOFFs
        mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::OFF;
        if (mIsIgnOn == true) {
            if (mEngineCcActive == EngineCcActive::ON && mSmvSetSpeedStatus != SmvSetSpeedStatus::GREEN &&
                mSmvSetSpeedStatus != SmvSetSpeedStatus::WHITE) {
                if (mDHCState == DHCState::ACTIVE) {
                    // IGN: ON && EngineCcActive: ON && DHCState: ACTIVE
                    mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::CC_DHC_ACTIVE;
                } else if (mDHCState != DHCState::ACTIVE && mMCCStatus == MCCStatus::ON) {
                    // IGN: ON && EngineCcActive: ON && DHCState: !ACTIVE && MCCStatus: ON
                    mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::CC_MCC_ACTIVE;
                } else {
                    // no operation
                }
            } else if (mSmvSetSpeedStatus == SmvSetSpeedStatus::GREEN || mSmvSetSpeedStatus == SmvSetSpeedStatus::WHITE) {
                if (mDHCState == DHCState::ACTIVE) {
                    // IGN: ON && EngineCcActive: OFF && SmvSetSpeedStatus: GREEN, WHITE && DHCState: ACTIVE
                    mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::SCC_DHC_ACTIVE;
                } else if (mDHCState != DHCState::ACTIVE && mMCCStatus == MCCStatus::ON) {
                    // IGN: ON && EngineCcActive: OFF && SmvSetSpeedStatus: GREEN, WHITE && DHCState: !ACTIVE && MCCStatus: ON
                    mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::SCC_MCC_ACTIVE;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }
        updateConstantMaxToleranceSpeed();
#ifdef ENABLE_GCOV_ON
        setSFCPrivateSet_SpeedMaxToleranceSpeedStat(
            static_cast<SFCPrivateSet_SpeedMaxToleranceSpeedStat>(mPrivateMaxToleranceSpeedStat));
#endif
    }

    void updateConstantMaxToleranceSpeed() {
        // Default : 0xFFF
        HDouble value = 0xFFF;
        switch (mPrivateMaxToleranceSpeedStat) {
            case PrivateMaxToleranceSpeedStat::CC_MCC_ACTIVE:
                if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) + mPar_CCSetSpeedKmTolerance;
                } else if (0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF && mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) * kFactor_ConvKmToMi + mPar_CCSetSpeedMileTolerance;
                } else {
                    // no operation
                }
                break;
            case PrivateMaxToleranceSpeedStat::CC_DHC_ACTIVE:
                if ((0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF) &&
                    (0x0 <= mDHCToleranceValue && mDHCToleranceValue <= 0x1F) && mTripDistanceUnit == TripDistanceUnit::KM) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) + mPar_CCSetSpeedKmTolerance +
                            static_cast<HDouble>(mDHCToleranceValue);
                } else if ((0x0 <= mEngineCcSetSpeed && mEngineCcSetSpeed <= 0xFF) &&
                           (0x0 <= mDHCToleranceValue && mDHCToleranceValue <= 0x1F) &&
                           mTripDistanceUnit == TripDistanceUnit::MILE) {
                    value = static_cast<HDouble>(mEngineCcSetSpeed) * kFactor_ConvKmToMi + mPar_CCSetSpeedMileTolerance +
                            static_cast<HDouble>(mDHCToleranceValue);
                } else {
                    // no operation
                }
                break;
            case PrivateMaxToleranceSpeedStat::SCC_MCC_ACTIVE:
                if (0x0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 0xFF) {
                    value = mSmvSetSpeedValue;
                }
                break;
            case PrivateMaxToleranceSpeedStat::SCC_DHC_ACTIVE:
                if ((0x0 <= mSmvSetSpeedValue && mSmvSetSpeedValue <= 0xFF) &&
                    (0x0 <= mDHCToleranceValue && mDHCToleranceValue <= 0x1F)) {
                    value = mSmvSetSpeedValue + mDHCToleranceValue;
                }
                break;
            default:
                // no operation
                break;
        }
        setSFCSet_SpeedConstantMaxToleranceSpeedValue(static_cast<HUInt64>(value));
    }

    using EngineCcActive = ArgumentsEngineCcSmvStatusChanged::Input_EngineCcActive;
    using SmvSetSpeedStatus = ArgumentsEngineCcSmvStatusChanged::Input_SmvSetSpeedStatus;
    using SRSActiveStatus = ArgumentsSrsRetarderStatusChanged::Input_SRSActiveStatus;
    using RetarderSwitchStatus = ArgumentsSrsRetarderStatusChanged::Input_RetarderSwitchStatus;
    using MCCStatus = ArgumentsMccDhcChanged::Input_MCCStatus;
    using DHCState = ArgumentsMccDhcChanged::Input_DHCState;
    using PrivateSetSpeedPositionStat = SFCPrivateSet_SpeedSetSpeedPositionStat;
    using PrivateMinToleranceSpeedStat = SFCPrivateSet_SpeedMinToleranceSpeedStat;
    using PrivateMaxToleranceSpeedStat = SFCPrivateSet_SpeedMaxToleranceSpeedStat;
    using TripDistanceUnit = ArgumentsTripDistanceUnitChanged::Inter_TripDistanceUnit;

    HBool mIsAemInitComplete = false;
    HBool mIsIgnOn = false;
    EngineCcActive mEngineCcActive = EngineCcActive::OFF;
    SmvSetSpeedStatus mSmvSetSpeedStatus = SmvSetSpeedStatus::OFF;
    SRSActiveStatus mSRSActiveStatus = SRSActiveStatus::OFF;
    RetarderSwitchStatus mRetarderSwitchStatus = RetarderSwitchStatus::OFF;
    MCCStatus mMCCStatus = MCCStatus::OFF;
    DHCState mDHCState = DHCState::OFF;
    PrivateSetSpeedPositionStat mPrivateSetSpeedPositionStat = PrivateSetSpeedPositionStat::OFF;
    PrivateMinToleranceSpeedStat mPrivateMinToleranceSpeedStat = PrivateMinToleranceSpeedStat::OFF;
    PrivateMaxToleranceSpeedStat mPrivateMaxToleranceSpeedStat = PrivateMaxToleranceSpeedStat::OFF;
    TripDistanceUnit mTripDistanceUnit = TripDistanceUnit::KM;

    HUInt64 mEngineCcSetSpeed = 0xFFF;
    HUInt64 mSmvSetSpeedValue = 0xFFF;
    HUInt64 mSRSSetSpeedValue = 0xFFF;
    HUInt64 mBremsomatSetSpeedValue = 0xFFF;
    HUInt64 mMCCMinSpeedValue = 0xFFF;
    HUInt64 mDHCToleranceValue = 0xFFF;

    HDouble mPar_CCSetSpeedKmTolerance = 0;
    HDouble mPar_CCSetSpeedMileTolerance = 0;
    HDouble mPar_SRSSetSpeedKmTolerance = 0;
    HDouble mPar_SRSSetSpeedMileTolerance = 0;
    HDouble mPar_BremsomatSetSpeedKmTolerance = 0;
    HDouble mPar_BremsomatSetSpeedMileTolerance = 0;

    static constexpr HDouble kFactor_ConvKmToMi = 0.6213592233009709;
};

}  // namespace ccos

#endif  // SFSS_Set_Speed_H
