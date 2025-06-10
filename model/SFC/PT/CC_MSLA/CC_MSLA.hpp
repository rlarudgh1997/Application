/**
 * @file CC_MSLA.hpp
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
#ifndef SFSS_CC_MSLA_H
#define SFSS_CC_MSLA_H

#define DLOG_ENABLED gEnableSFCLog

#include <cstdlib>

#include <Vehicle.h>
#include "CC_MSLABase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 23.0.1
// Reference : [PT050] CC_MSLA. Contains : Telltale, Constant, Event, Sound. Recommend: VALUE_CHANGED
class CC_MSLA : public CC_MSLABase {
public:
    CC_MSLA() = default;
    ~CC_MSLA() override = default;
    CC_MSLA(const CC_MSLA& other) = delete;
    CC_MSLA(CC_MSLA&& other) noexcept = delete;
    CC_MSLA& operator=(const CC_MSLA& other) = delete;
    CC_MSLA& operator=(CC_MSLA&& other) noexcept = delete;

    void onInitialize() override {
        setSFCCC_MSLAConstantCrawlOperationBlinkValueB(SFCCC_MSLAConstantCrawlOperationBlinkValueB::OFF);
        setSFCCC_MSLAEventCcMslaSummaryID("E20207");
        setSFCCC_MSLAEventCcCruiseInfoDisplayLinkedSoundID("SND_PopUpInform1");
        setSFCCC_MSLAEventCcCruiseInfoDisplayLinkedSoundType(SFCCC_MSLAEventCcCruiseInfoDisplayLinkedSoundType::REPEAT_COUNT);
        setSFCCC_MSLAEventCcCruiseInfoDisplayLinkedSoundRepeatCount(1);
        setSFCCC_MSLAEventCcNccDisplayLinkedSoundType(SFCCC_MSLAEventCcNccDisplayLinkedSoundType::REPEAT_COUNT);
        setSFCCC_MSLAEventCcNccDisplayLinkedSoundRepeatCount(1);
        setSFCCC_MSLAEventMslaLinkedSoundID("SND_PopUpInform1");
        setSFCCC_MSLAEventMslaLinkedSoundType(SFCCC_MSLAEventMslaLinkedSoundType::REPEAT_COUNT);
        setSFCCC_MSLAEventMslaLinkedSoundRepeatCount(1u);
        setSFCCC_MSLAEventCrawlActiveID("E20211");
        setSFCCC_MSLASoundMslaID("SND_MSLA");
        setSFCCC_MSLASoundMslaType(SFCCC_MSLASoundMslaType::REPEAT_MINMAX);
        setSFCCC_MSLASoundMslaRepeatMin(3);
        setSFCCC_MSLASoundMslaRepeatMax(15);
        if (mOneShotTimerSummaryEventOff.create(kTimerSummaryEventOffInterval160ms, this, &CC_MSLA::onOneShotTimerSummaryEventOff,
                                                false) == false) {
            DWarning() << "[CC_MSLA] mOneShotTimerSummaryEventOff Create Failed";
        }
        if (mOneShotTimerMslaoverride.create(kTimerMslaoverrideTriggerInterval50ms, this, &CC_MSLA::onOneShotTimerMslaoverride,
                                             false) == false) {
            DWarning() << "[CC_MSLA] mOneShotTimerMslaoverride Create Failed";
        }
        if (mOneShotTimerNoData.create(kTimerNoDataInterval10ms, this, &CC_MSLA::onOneShotTimerNoData, false) == false) {
            DWarning() << "[CC_MSLA] mOneShotTimerNoData Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateInterCcOnOffStat();
        updateConstantCcColor();
        updateInterMslaOnOffStat();
        updateConstantMslaColor();
        updatePrivateCcSldSetSpeedValue();
        updateInterCcActiveStat();
        updateInterMslaActiveStat();
        updateInterCcSldMaxSpeedValue();
        updateInterMslaoverride();
        updateConstantSpeed();
        updateConstantSpeedUnit();
        updateConstantNaviCruise();
        updateConstantCrawlOperationStatus();

        updateEventCcCruiseInfoDisplay();
        updateEventCcNccDisplay();
        updateEventMsla();
        updateEventCrawlActive();
        updateEventCrawlWarn();

        updateSound();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateInterCcOnOffStat();
        updateConstantCcColor();
        updateInterMslaOnOffStat();
        updateConstantMslaColor();
        updatePrivateCcSldSetSpeedValue();
        updateInterCcActiveStat();
        updateInterMslaActiveStat();
        updateInterCcSldMaxSpeedValue();
        updateInterMslaoverride();
        updateConstantSpeed();
        updateConstantSpeedUnit();
        updateConstantNaviCruise();
        updateConstantCrawlOperationStatus();

        updateEventCcCruiseInfoDisplay();
        updateEventCcNccDisplay();
        updateEventMsla();
        updateEventCrawlActive();
        updateEventCrawlWarn();

        updateSound();
    }

    void onCcOnOffStatChanged(const ArgumentsCcOnOffStatChanged& args) {
        mInput_CruiseMainIndReqStatus = args.mInput_CruiseMainIndReqStatus;
        mInter_ConfigAdasSCC = args.mInter_ConfigAdasSCC;

        updateInterCcOnOffStat();
        updateConstantCcColor();
        updatePrivateCcSldSetSpeedValue();
        updateInterCcActiveStat();
        updateConstantSpeed();
        updateConstantSpeedUnit();
    }

    void onCcSldMaxSpeedValueChanged(const ArgumentsCcSldMaxSpeedValueChanged& args) {
        mInter_ConfigVehicleType = args.mInter_ConfigVehicleType;

        updateInterCcSldMaxSpeedValue();
        updatePrivateCcSldSetSpeedValue();
        updateInterMslaoverride();
        updateConstantSpeed();
        updateConstantSpeedUnit();
    }

    void onCcDisplayChagned(const ArgumentsCcDisplayChagned& args) {
        mInput_CruiseSetIndReqStatus = args.mInput_CruiseSetIndReqStatus;

        updateConstantCcColor();
        updatePrivateCcSldSetSpeedValue();
        updateInterCcActiveStat();
        updateConstantSpeed();
        updateConstantSpeedUnit();
    }

    void onNccDisplayChanged(const ArgumentsNccDisplayChanged& args) {
        updateValueIfValid(mInput_NccCameraOperStatus, args.mInput_NccCameraOperStatus);  // HEV, PHEV

        updateConstantNaviCruise();
    }

    void onSpeedStatusChanged(const ArgumentsSpeedStatusChanged& args) {
        updateValueIfValid(mInter_TripDistanceUnit, args.mInter_TripDistanceUnit);
        if (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM || mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE) {
            mSpeedDigitalValue = args.mSpeedDigitalValue;
        } else {
            DError() << "TripDistanceUnit is not set.";
            return;
        }
        updateValueIfValid(mInput_CruiseOverrideReqStatus, args.mInput_CruiseOverrideReqStatus);  // EV, HEV, PHEV
        if (ISTIMEOUT(args.mInput_CcSldSetSpeedValue) == true) {
            mInput_CcSldSetSpeedValue = kTimeOut;
        } else {
            mInput_CcSldSetSpeedValue = args.mInput_CcSldSetSpeedValue;
        }
        if (ISTIMEOUT(args.mInput_CcOnlySetSpeedValue) == true) {
            mInput_CcOnlySetSpeedValue = kTimeOut;
        } else {
            mInput_CcOnlySetSpeedValue = args.mInput_CcOnlySetSpeedValue;
        }
        if (ISTIMEOUT(args.mInput_SpeedLimitActiveStatus) == true) {
            mInput_SpeedLimitActiveStatus = Input_SpeedLimitActiveStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_SpeedLimitActiveStatus = args.mInput_SpeedLimitActiveStatus;
        }
        if (ISTIMEOUT(args.mInput_SpeedLimitReadyStatus) == true) {
            mInput_SpeedLimitReadyStatus = Input_SpeedLimitReadyStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_SpeedLimitReadyStatus = args.mInput_SpeedLimitReadyStatus;
        }
        if (ISTIMEOUT(args.mInput_SmvSetSpeedStatus) == true) {
            mInput_SmvSetSpeedStatus = Input_SmvSetSpeedStatus::UNHANDLED_TIMEOUT;
        } else {
            mInput_SmvSetSpeedStatus = args.mInput_SmvSetSpeedStatus;
        }

        updateInterMslaOnOffStat();
        updateConstantMslaColor();
        updateInterMslaActiveStat();
        updatePrivateCcSldSetSpeedValue();
        updateInterMslaoverride();
        updateConstantSpeed();
        updateConstantSpeedUnit();

        updateSound();
    }

    void onSpeedLimitInfoDisplayReqStatusChanged(const ArgumentsSpeedLimitInfoDisplayReqStatusChanged& args) {
        mInput_SpeedLimitInfoDisplayReqStatus = args.mInput_SpeedLimitInfoDisplayReqStatus;

        updateEventMsla();
    }

    void onNccDisplayReqStatusChanged(const ArgumentsNccDisplayReqStatusChanged& args) {
        updateValueIfValid(mInput_NccDisplayReqStatus, args.mInput_NccDisplayReqStatus);  // EV, HEV, PHEV

        updateEventCcNccDisplay();
    }

    void onCruiseInfoDisplayReqStatusChanged(const ArgumentsCruiseInfoDisplayReqStatusChanged& args) {
        mInput_CruiseInfoDisplayReqStatus = args.mInput_CruiseInfoDisplayReqStatus;

        updateEventCcCruiseInfoDisplay();
    }

    void onIslaManualSetSpeedChanged(const ArgumentsIslaManualSetSpeedChanged& args) {
        mISLAManualSetSpeedStat = args.mISLAManualSetSpeedStat;

        updateConstantISLAManualSetSpeedStat();
    }

    void onCrawlActiveChanged(const ArgumentsCrawlActiveChanged& args) {
        mCrawlActiveStatus = args.mInput_CrawlActiveStatus;
        mCrawlActiveStepStatus = args.mInput_CrawlActiveStepStatus;
        updateConstantCrawlOperationStatus();
        updateEventCrawlActive();
    }

    void onCrawlWarnStatus(const ArgumentsCrawlWarnStatus& args) {
        mCrawlWarnStatus = args.mInput_CrawlWarnStatus;
        updateEventCrawlWarn();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4.2 Inter_Mslaoverride
    void updateInterMslaoverride() {
        HBool mslaoverrideOn = false;

        if (mIgnElapsed == IgnElapsed::ON_0ms && mInput_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::ON &&
            mInput_SpeedLimitReadyStatus == Input_SpeedLimitReadyStatus::NORMAL) {
            if (mInter_Mslaoverride == Mslaoverride::OFF && hysteresisIncrease() == true) {
                mslaoverrideOn = true;
            } else if (mInter_Mslaoverride == Mslaoverride::ON && hysteresisDecrease() == true) {
                mslaoverrideOn = false;
            } else {
                // Other case (Stay Inter_Mslaoverride status)
                return;
            }
        } else {
            mslaoverrideOn = false;
        }

        if (mslaoverrideOn == true) {
            if (mOneShotTimerMslaoverride.start() == false) {
                DWarning() << "[CC_MSLA] mOneShotTimerMslaoverride Start Failed";
            }
        } else {
            mOneShotTimerMslaoverride.stop();
            mInter_Mslaoverride = Mslaoverride::OFF;
            setSFCCC_MSLAInter_Mslaoverride(mInter_Mslaoverride);
        }
    }

    // 4.3 Inter_CcOnOffStat
    void updateInterCcOnOffStat() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mInter_ConfigAdasSCC == Inter_ConfigAdasSCC::OFF &&
            mInput_CruiseMainIndReqStatus == Input_CruiseMainIndReqStatus::ON) {
            mInter_CcOnOffStat = CcOnOffStat::ON;
        } else {
            mInter_CcOnOffStat = CcOnOffStat::OFF;
        }
        setSFCCC_MSLAInter_CcOnOffStat(mInter_CcOnOffStat);
        updateEventCcMslaSummary();
    }

    // 4.3 Inter_MslaOnOffStat
    void updateInterMslaOnOffStat() {
        if (mInput_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            mInter_MslaOnOffStat = MslaOnOffStat::ON;
        } else {
            mInter_MslaOnOffStat = MslaOnOffStat::OFF;
        }
        setSFCCC_MSLAInter_MslaOnOffStat(mInter_MslaOnOffStat);
        updateEventCcMslaSummary();
    }

    // Private_CcSldSetSpeedValue
    void updatePrivateCcSldSetSpeedValue() {
        if ((mConstantCcColorStat == SFCCC_MSLAConstantCcColorStat::CRUISE_ACTIVE ||
             mConstantCcColorStat == SFCCC_MSLAConstantCcColorStat::CRUISE_PAUSE) &&
            (mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
             mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) &&
            (mConstantMslaColorStat == SFCCC_MSLAConstantMslaColorStat::DISPLAY_OFF)) {
            mPrivate_CcSldSetSpeedValue = mInput_CcOnlySetSpeedValue;
        } else {
            mPrivate_CcSldSetSpeedValue = mInput_CcSldSetSpeedValue;
        }
    }

    // Inter_CcActiveStat
    void updateInterCcActiveStat() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_CruiseMainIndReqStatus == Input_CruiseMainIndReqStatus::ON &&
                mConstantCcColorStat == SFCCC_MSLAConstantCcColorStat::CRUISE_ACTIVE) {
                mInter_CcActiveStat = CcActiveStat::ON;
            } else {
                mInter_CcActiveStat = CcActiveStat::OFF;
            }
        } else {
            mInter_CcActiveStat = CcActiveStat::OFF;
        }
        setSFCCC_MSLAInter_CcActiveStat(mInter_CcActiveStat);
    }

    // Inter_MslaActiveStat
    void updateInterMslaActiveStat() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::ON &&
                (mConstantMslaColorStat == SFCCC_MSLAConstantMslaColorStat::MSLA_ACTIVE ||
                 mConstantMslaColorStat == SFCCC_MSLAConstantMslaColorStat::MSLA_ISLA_ON)) {
                mInter_MslaActiveStat = MslaActiveStat::ON;
            } else {
                mInter_MslaActiveStat = MslaActiveStat::OFF;
            }
        } else {
            mInter_MslaActiveStat = MslaActiveStat::OFF;
        }

        setSFCCC_MSLAInter_MslaActiveStat(mInter_MslaActiveStat);
    }

    // 4.4 Inter_CcSldMaxSpeedValue
    void updateInterCcSldMaxSpeedValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInter_ConfigVehicleType == Inter_ConfigVehicleType::ICV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::EV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::FCEV ||
                mInter_ConfigVehicleType == Inter_ConfigVehicleType::EREV) {
                mInter_CcSldMaxSpeedValue = 510;
            } else {
                mInter_CcSldMaxSpeedValue = 254;
            }
        }
        setSFCCC_MSLAInter_CcSldMaxSpeedValue(mInter_CcSldMaxSpeedValue);
    }

    // 5.2.1.1 CC Symbol & Color
    void updateConstantCcColor() {
        if (mInter_CcOnOffStat == CcOnOffStat::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mInput_CruiseSetIndReqStatus) {
                case Input_CruiseSetIndReqStatus::OFF:
                    mConstantCcColorStat = SFCCC_MSLAConstantCcColorStat::CRUISE_PAUSE;
                    break;
                case Input_CruiseSetIndReqStatus::ON:
                    mConstantCcColorStat = SFCCC_MSLAConstantCcColorStat::CRUISE_ACTIVE;
                    break;
                default:
                    mConstantCcColorStat = SFCCC_MSLAConstantCcColorStat::DISPLAY_OFF;
                    break;
            }
        } else {
            mConstantCcColorStat = SFCCC_MSLAConstantCcColorStat::DISPLAY_OFF;
        }
        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
            DDebug() << "Hold 160ms, Stay Status(Not Display off)";
        } else {
            DDebug() << "Not exist Timer, Normal excuting";
            setSFCCC_MSLAConstantCcColorStat(mConstantCcColorStat);
        }
    }

    // 5.2.1.2 MSLA Symbol & Color
    void updateConstantMslaColor() {
        mConstantMslaColorStat = SFCCC_MSLAConstantMslaColorStat::DISPLAY_OFF;
        if (mInter_MslaOnOffStat == MslaOnOffStat::ON && mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_SpeedLimitReadyStatus == Input_SpeedLimitReadyStatus::READY) {
                mConstantMslaColorStat = SFCCC_MSLAConstantMslaColorStat::MSLA_PAUSE;
            } else if (mInput_SpeedLimitReadyStatus == Input_SpeedLimitReadyStatus::NORMAL) {
                if (mInput_SmvSetSpeedStatus == Input_SmvSetSpeedStatus::GREEN) {
                    mConstantMslaColorStat = SFCCC_MSLAConstantMslaColorStat::MSLA_ISLA_ON;
                } else {
                    mConstantMslaColorStat = SFCCC_MSLAConstantMslaColorStat::MSLA_ACTIVE;
                }
            } else {
                // no operation
            }
        } else {
            // no operation
        }
        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
        } else {
            setSFCCC_MSLAConstantMslaColorStat(mConstantMslaColorStat);
        }
    }

    // 5.2.2 Navi Cruise
    void updateConstantNaviCruise() {
        SFCCC_MSLAConstantNaviCruiseStat naviCruiseStat{SFCCC_MSLAConstantNaviCruiseStat::DISPLAY_OFF};
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mInput_NccCameraOperStatus) {
                case Input_NccCameraOperStatus::READY:
                    naviCruiseStat = SFCCC_MSLAConstantNaviCruiseStat::READY;
                    break;
                case Input_NccCameraOperStatus::ACT:
                    naviCruiseStat = SFCCC_MSLAConstantNaviCruiseStat::ACTIVE;
                    break;
                default:
                    break;
            }
        }
        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
        } else {
            setSFCCC_MSLAConstantNaviCruiseStat(naviCruiseStat);
        }
    }

    // 5.2.2.1 CRAWL Operation Status
    void updateConstantCrawlOperationStatus() {
        bool isStatUpdate = true;
        bool isStepUpdate = true;

        SFCCC_MSLAConstantCrawlOperationStat stat = SFCCC_MSLAConstantCrawlOperationStat::OFF;
        SFCCC_MSLAConstantCrawlOperationStepStat step = SFCCC_MSLAConstantCrawlOperationStepStat::OFF;
        SFCCC_MSLAConstantCrawlOperationBlinkValueA blinkValueA = SFCCC_MSLAConstantCrawlOperationBlinkValueA::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mCrawlActiveStatus) {
                case CrawlActiveStatus::READY:
                    stat = SFCCC_MSLAConstantCrawlOperationStat::READY;
                    break;
                case CrawlActiveStatus::ACTIVE:
                    stat = SFCCC_MSLAConstantCrawlOperationStat::ACTIVE;
                    break;
                case CrawlActiveStatus::OVERRIDE:
                    stat = SFCCC_MSLAConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCCC_MSLAConstantCrawlOperationBlinkValueA::WHITE;
                    break;
                case CrawlActiveStatus::PAUSE:
                    stat = SFCCC_MSLAConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCCC_MSLAConstantCrawlOperationBlinkValueA::ACTIVE;
                    break;
                case CrawlActiveStatus::TEMP:
                    stat = SFCCC_MSLAConstantCrawlOperationStat::BLINK2;
                    blinkValueA = SFCCC_MSLAConstantCrawlOperationBlinkValueA::YELLOW;
                    break;
                default:
                    isStatUpdate = false;
                    break;
            }

            switch (mCrawlActiveStepStatus) {
                case CrawlActiveStepStatus::STEP1:
                    step = SFCCC_MSLAConstantCrawlOperationStepStat::STEP1;
                    break;
                case CrawlActiveStepStatus::STEP2:
                    step = SFCCC_MSLAConstantCrawlOperationStepStat::STEP2;
                    break;
                case CrawlActiveStepStatus::STEP3:
                    step = SFCCC_MSLAConstantCrawlOperationStepStat::STEP3;
                    break;
                case CrawlActiveStepStatus::STEP4:
                    step = SFCCC_MSLAConstantCrawlOperationStepStat::STEP4;
                    break;
                case CrawlActiveStepStatus::STEP5:
                    step = SFCCC_MSLAConstantCrawlOperationStepStat::STEP5;
                    break;
                default:
                    isStepUpdate = false;
                    break;
            }
        } else {
            isStatUpdate = false;
            isStepUpdate = false;
        }

        if (isStatUpdate == true && isStepUpdate == true) {
            setSFCCC_MSLAConstantCrawlOperationStat(stat);
            setSFCCC_MSLAConstantCrawlOperationStepStat(step);
            setSFCCC_MSLAConstantCrawlOperationBlinkValueA(blinkValueA);
        } else {
            setSFCCC_MSLAConstantCrawlOperationStat(SFCCC_MSLAConstantCrawlOperationStat::OFF);
            setSFCCC_MSLAConstantCrawlOperationStepStat(SFCCC_MSLAConstantCrawlOperationStepStat::OFF);
            setSFCCC_MSLAConstantCrawlOperationBlinkValueA(SFCCC_MSLAConstantCrawlOperationBlinkValueA::OFF);
        }
    }

    // 5.2.3 Speed
    void updateConstantSpeed(HBool enterFromTimerCallback = false) {
        SFCCC_MSLAConstantCcMslaSpeedStat ccMslaSpeedStat{SFCCC_MSLAConstantCcMslaSpeedStat::DISPLAY_OFF};
        SFCCC_MSLAConstantCcMslaSpeedBlinkValueA ccMslaSpeedBlinkValueA{SFCCC_MSLAConstantCcMslaSpeedBlinkValueA::NONE};
        SFCCC_MSLAConstantCcMslaSpeedBlinkValueB ccMslaSpeedBlinkValueB{SFCCC_MSLAConstantCcMslaSpeedBlinkValueB::NONE};
        HUInt64 ccSldSetSpeedValue = 0;

        if ((mInter_CcOnOffStat == CcOnOffStat::ON || mInter_MslaOnOffStat == MslaOnOffStat::ON) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivate_CcSldSetSpeedValue != kTimeOut) {
                if (mPrivate_CcSldSetSpeedValue == 0U || mPrivate_CcSldSetSpeedValue == mInter_CcSldMaxSpeedValue + 1) {
                    if (enterFromTimerCallback == true) {
                        ccMslaSpeedStat = SFCCC_MSLAConstantCcMslaSpeedStat::NO_DATA;
                        ccSldSetSpeedValue = mPrivate_CcSldSetSpeedValue;
                    } else {
                        if (mOneShotTimerNoData.start() == false) {
                            DWarning() << "[CC_MSLA] mOneShotTimerNoData Start Failed";
                        }
                        return;
                    }
                } else if (1u <= mPrivate_CcSldSetSpeedValue && mPrivate_CcSldSetSpeedValue <= mInter_CcSldMaxSpeedValue) {
                    if ((mInter_MslaActiveStat == MslaActiveStat::ON && mInter_Mslaoverride == Mslaoverride::ON) ||
                        (mInter_MslaActiveStat == MslaActiveStat::OFF && mInter_CcActiveStat == CcActiveStat::ON &&
                         mInput_CruiseOverrideReqStatus == Input_CruiseOverrideReqStatus::OVERRIDE)) {
                        ccMslaSpeedStat = SFCCC_MSLAConstantCcMslaSpeedStat::BLINK2;
                        ccMslaSpeedBlinkValueA = SFCCC_MSLAConstantCcMslaSpeedBlinkValueA::VALUE;
                        ccMslaSpeedBlinkValueB = SFCCC_MSLAConstantCcMslaSpeedBlinkValueB::DISPLAY_OFF;
                        ccSldSetSpeedValue = mPrivate_CcSldSetSpeedValue;
                    } else {
                        ccMslaSpeedStat = SFCCC_MSLAConstantCcMslaSpeedStat::ON;
                        ccSldSetSpeedValue = mPrivate_CcSldSetSpeedValue;
                    }
                } else {
                    // default value(DISPLAY_OFF, NONE)
                }
            }
        }
        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
        } else {
            setSFCCC_MSLAConstantCcMslaSpeedStat(ccMslaSpeedStat);
            setSFCCC_MSLAConstantCcMslaSpeedBlinkValueA(ccMslaSpeedBlinkValueA);
            setSFCCC_MSLAConstantCcMslaSpeedBlinkValueB(ccMslaSpeedBlinkValueB);
            setSFCCC_MSLAConstantCcMslaSpeedValue(ccSldSetSpeedValue);
            if (enterFromTimerCallback == true) {
                flushLastGroup();
            }
        }
    }

    // 5.2.4 Speed Unit
    void updateConstantSpeedUnit() {
        ccos::HUInt64 ccMslaSpeedUnit = 0;
        getInternalCachedValue("SFC.CC_MSLA.Constant.CcMslaSpeed.Stat", ccMslaSpeedUnit);
        SFCCC_MSLAConstantCcMslaSpeedUnitStat ccMslaSpeedUnitStat{SFCCC_MSLAConstantCcMslaSpeedUnitStat::DISPLAY_OFF};

        if (ccMslaSpeedUnit != static_cast<ccos::HUInt64>(SFCCC_MSLAConstantCcMslaSpeedStat::DISPLAY_OFF) &&
            mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mInter_TripDistanceUnit) {
                case Inter_TripDistanceUnit::KM:
                    ccMslaSpeedUnitStat = SFCCC_MSLAConstantCcMslaSpeedUnitStat::KPH;
                    break;
                case Inter_TripDistanceUnit::MILE:
                    ccMslaSpeedUnitStat = SFCCC_MSLAConstantCcMslaSpeedUnitStat::MPH;
                    break;
                default:
                    break;
            }
        }
        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
        } else {
            setSFCCC_MSLAConstantCcMslaSpeedUnitStat(ccMslaSpeedUnitStat);
        }
    }

    // ISLAManualSetSpeed는 ADAS_Driving_NEW와 CC_MSLA에 중복하여 존재함.
    // 사양서에 정의된 "CC&MSLA 요약 정보"와 연관 될 경우 SFC.CC_MSLA.Constant.ISLAManualSetSpeed.Stat 노드를 사용해야 함.
    // (이력 : EXNCP-31245)
    void updateConstantISLAManualSetSpeedStat() {
        SFCCC_MSLAConstantISLAManualSetSpeedStat stat = SFCCC_MSLAConstantISLAManualSetSpeedStat::OFF;

        stat = static_cast<SFCCC_MSLAConstantISLAManualSetSpeedStat>(mISLAManualSetSpeedStat);

        if (stat <= SFCCC_MSLAConstantISLAManualSetSpeedStat::NONE || stat >= SFCCC_MSLAConstantISLAManualSetSpeedStat::MAX) {
            stat = SFCCC_MSLAConstantISLAManualSetSpeedStat::OFF;
        }

        if (ccMslaSummary == static_cast<ccos::HUInt64>(SFCCC_MSLAEventCcMslaSummaryStat::ON) &&
            mOneShotTimerSummaryEventOff.isRunning() == true) {
            // no operation
        } else {
            setSFCCC_MSLAConstantISLAManualSetSpeedStat(stat);
        }
    }

    // 5.3.1.1 CC Summary
    void onOneShotTimerSummaryEventOff() {
        updateEventCcMslaSummary();
        updateConstantCcColor();
        updateConstantMslaColor();
        updatePrivateCcSldSetSpeedValue();
        updateInterCcSldMaxSpeedValue();
        updateInterMslaoverride();
        updateInterCcActiveStat();
        updateInterMslaActiveStat();
        updateConstantSpeed(true);
        updateConstantSpeedUnit();
        updateConstantNaviCruise();
        updateConstantISLAManualSetSpeedStat();
        flush();
    }

    void onOneShotTimerMslaoverride() {
        if (hysteresisIncrease() == true) {
            mInter_Mslaoverride = Mslaoverride::ON;
            setSFCCC_MSLAInter_Mslaoverride(mInter_Mslaoverride);
            updateConstantSpeed();
            updateSound();
            flush();
        }
    }

    void onOneShotTimerNoData() {
        updateConstantSpeed(true);
        updateConstantSpeedUnit();
        flush();
    }

    // 5.3.1.2 CRAWL
    void updateEventCrawlActive() {
        SFCCC_MSLAEventCrawlActiveStat stat = SFCCC_MSLAEventCrawlActiveStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mCrawlActiveStatus == CrawlActiveStatus::READY || mCrawlActiveStatus == CrawlActiveStatus::ACTIVE ||
                mCrawlActiveStatus == CrawlActiveStatus::OVERRIDE || mCrawlActiveStatus == CrawlActiveStatus::PAUSE ||
                mCrawlActiveStatus == CrawlActiveStatus::TEMP) {
                if (mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP1 ||
                    mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP2 ||
                    mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP3 ||
                    mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP4 ||
                    mCrawlActiveStepStatus == CrawlActiveStepStatus::STEP5) {
                    stat = SFCCC_MSLAEventCrawlActiveStat::ON;
                }
            }
        }
        setSFCCC_MSLAEventCrawlActiveStat(stat);
    }

    void updateEventCcMslaSummary() {
        if (mOneShotTimerSummaryEventOff.isRunning() == true) {
            mOneShotTimerSummaryEventOff.stop();
        }
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            // NOTE(ksh): noteMSLA has higher priority. EXNCP-20557
            if (mInput_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::ON) {
                setSFCCC_MSLAEventCcMslaSummaryStat(SFCCC_MSLAEventCcMslaSummaryStat::ON);
            } else if (mInput_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::OFF &&
                       mPrev_Input_SpeedLimitActiveStatus == Input_SpeedLimitActiveStatus::ON) {
                if (mOneShotTimerSummaryEventOff.start() == false) {
                    DWarning() << "[CC_MSLA] mOneShotTimerSummaryEventOff Start Failed";
                }
            } else if (mInter_ConfigAdasSCC == Inter_ConfigAdasSCC::OFF &&
                       mInput_CruiseMainIndReqStatus == Input_CruiseMainIndReqStatus::ON) {
                setSFCCC_MSLAEventCcMslaSummaryStat(SFCCC_MSLAEventCcMslaSummaryStat::ON);
            } else if (mInter_ConfigAdasSCC == Inter_ConfigAdasSCC::OFF &&
                       mInput_CruiseMainIndReqStatus == Input_CruiseMainIndReqStatus::OFF &&
                       mPrev_Input_CruiseMainIndReqStatus == Input_CruiseMainIndReqStatus::ON) {
                if (mOneShotTimerSummaryEventOff.start() == false) {
                    DWarning() << "[CC_MSLA] mOneShotTimerSummaryEventOff Start Failed";
                }
            } else {
                setSFCCC_MSLAEventCcMslaSummaryStat(SFCCC_MSLAEventCcMslaSummaryStat::OFF);
            }
            mPrev_Input_SpeedLimitActiveStatus = mInput_SpeedLimitActiveStatus;
            mPrev_Input_CruiseMainIndReqStatus = mInput_CruiseMainIndReqStatus;
        } else {
            setSFCCC_MSLAEventCcMslaSummaryStat(SFCCC_MSLAEventCcMslaSummaryStat::OFF);
        }

        getInternalCachedValue("SFC.CC_MSLA.Event.CcMslaSummary.Stat", ccMslaSummary);
    }

    // 5.3.2.1 Event CC
    void updateEventCcCruiseInfoDisplay() {
        std::string eventID;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_CruiseInfoDisplayReqStatus == Input_CruiseInfoDisplayReqStatus::AUTO_CANCELED) {
                eventID = "E20201";
            } else if (mInput_CruiseInfoDisplayReqStatus == Input_CruiseInfoDisplayReqStatus::NO_CONDITION) {
                eventID = "E20202";
            } else {
                // no operation
            }
        }
        std::string prevEventID;
        getInternalCachedValue("SFC.CC_MSLA.Event.CcCruiseInfoDisplay.ID", prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCCC_MSLAEventCcCruiseInfoDisplayStat(SFCCC_MSLAEventCcCruiseInfoDisplayStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCCC_MSLAEventCcCruiseInfoDisplayID(eventID);
            setSFCCC_MSLAEventCcCruiseInfoDisplayStat(SFCCC_MSLAEventCcCruiseInfoDisplayStat::ON);
        }
    }

    void updateEventCcNccDisplay() {
        std::string eventID;
        std::string linkedSoundID;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_NccDisplayReqStatus == Input_NccDisplayReqStatus::SWITCH_CONTROL_REQ) {
                eventID = "E20203";
                linkedSoundID = "SND_PopUpInform2";
            } else if (mInput_NccDisplayReqStatus == Input_NccDisplayReqStatus::BRAKE_CONTROL_REQ) {
                eventID = "E20204";
                linkedSoundID = "SND_PopUpWarn1";
            } else {
                // no operation
            }
        }
        std::string prevEventID;
        getInternalCachedValue("SFC.CC_MSLA.Event.CcNccDisplay.ID", prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCCC_MSLAEventCcNccDisplayStat(SFCCC_MSLAEventCcNccDisplayStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCCC_MSLAEventCcNccDisplayID(eventID);
            setSFCCC_MSLAEventCcNccDisplayLinkedSoundID(linkedSoundID);
            setSFCCC_MSLAEventCcNccDisplayStat(SFCCC_MSLAEventCcNccDisplayStat::ON);
        }
    }

    // 5.3.2.2 Event MSLA
    void updateEventMsla() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mInput_SpeedLimitInfoDisplayReqStatus == Input_SpeedLimitInfoDisplayReqStatus::AUTO_CANCELED) {
                setEventMslaOn("E20205");
            } else if (mInput_SpeedLimitInfoDisplayReqStatus == Input_SpeedLimitInfoDisplayReqStatus::NO_CONDITION) {
                setEventMslaOn("E20206");
            } else {
                setEventMslaOff();
            }
        } else {
            setEventMslaOff();
        }
    }

    void setEventMslaOff() {
        std::string prevEventID;
        getInternalCachedValue("SFC.CC_MSLA.Event.Msla.ID", prevEventID);
        if (prevEventID.empty() == false) {
            setSFCCC_MSLAEventMslaStat(SFCCC_MSLAEventMslaStat::OFF);
        }
    }

    void setEventMslaOn(const std::string& eventID) {
        std::string prevEventID;
        getInternalCachedValue("SFC.CC_MSLA.Event.Msla.ID", prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCCC_MSLAEventMslaStat(SFCCC_MSLAEventMslaStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCCC_MSLAEventMslaID(eventID);
            setSFCCC_MSLAEventMslaStat(SFCCC_MSLAEventMslaStat::ON);
        }
    }

    // 5.3.2.3 X-TREK(CRAWL)
    void updateEventCrawlWarn() {
        std::string eventID;
        std::string linkedSoundID;
        SFCCC_MSLAEventCrawlWarnLinkedSoundType linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::NONE;
        HUInt64 repeatCount = 0;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mCrawlWarnStatus) {
                case CrawlWarnStatus::CONDITION_NOT_MET_STOP:
                    eventID = "E20212";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_DOOR:
                    eventID = "E20213";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_BELT:
                    eventID = "E20214";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_TRAILER:
                    eventID = "E20215";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_AUTOHOLD:
                    eventID = "E20216";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_BRAKE:
                    eventID = "E20217";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_TOW:
                    eventID = "E20218";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_OTHERS:
                    eventID = "E20219";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::READY_FOOT_OFF:
                    eventID = "E20220";
                    break;
                case CrawlWarnStatus::DISABLE_TEMPORARY:
                    eventID = "E20221";
                    break;
                case CrawlWarnStatus::DISABLE_TEMPORARY_OVERSPEED:
                    eventID = "E20222";
                    break;
                case CrawlWarnStatus::UNSTUCK_CHANGE_STEP:
                    eventID = "E20223";
                    break;
                case CrawlWarnStatus::DISABLE_WARN:
                    eventID = "E20224";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_OVERSPEED:
                    eventID = "E20225";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_TRANSMISSION:
                    eventID = "E20226";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_MANUAL:
                    eventID = "E20227";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_BRAKE:
                    eventID = "E20228";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_OVERTIME:
                    eventID = "E20229";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::ERROR:
                    eventID = "E20230";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_GEAR:
                    eventID = "E20231";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::DISABLE_WARN_AUTOHOLD:
                    eventID = "E20232";
                    linkedSoundID = "SND_PopUpWarn1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                case CrawlWarnStatus::CONDITION_NOT_MET_SCC:
                    eventID = "E20233";
                    linkedSoundID = "SND_PopUpInform1";
                    linkedSoundType = SFCCC_MSLAEventCrawlWarnLinkedSoundType::REPEAT_COUNT;
                    repeatCount = 1;
                    break;
                default:
                    break;
            }
        }
        std::string prevEventID;
        getInternalCachedValue("SFC.CC_MSLA.Event.CrawlWarn.ID", prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCCC_MSLAEventCrawlWarnStat(SFCCC_MSLAEventCrawlWarnStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCCC_MSLAEventCrawlWarnID(eventID);
            setSFCCC_MSLAEventCrawlWarnStat(SFCCC_MSLAEventCrawlWarnStat::ON);
            setSFCCC_MSLAEventCrawlWarnLinkedSoundID(linkedSoundID);
            setSFCCC_MSLAEventCrawlWarnLinkedSoundType(linkedSoundType);
            setSFCCC_MSLAEventCrawlWarnLinkedSoundRepeatCount(repeatCount);
        }
    }

    // 5.4 Sound
    void updateSound() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mInter_Mslaoverride == Mslaoverride::ON) {
            setSFCCC_MSLASoundMslaStat(SFCCC_MSLASoundMslaStat::ON);
        } else {
            setSFCCC_MSLASoundMslaStat(SFCCC_MSLASoundMslaStat::OFF);
        }
    }

    inline HBool hysteresisIncrease() const {
        HInt64 speedSubtractionValue = static_cast<HInt64>(mSpeedDigitalValue) - static_cast<HInt64>(mPrivate_CcSldSetSpeedValue);
        return (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM && speedSubtractionValue >= 3) ||
               (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE && speedSubtractionValue >= 2);
    }

    inline HBool hysteresisDecrease() const {
        HInt64 speedSubtractionValue = static_cast<HInt64>(mSpeedDigitalValue) - static_cast<HInt64>(mPrivate_CcSldSetSpeedValue);
        return (mInter_TripDistanceUnit == Inter_TripDistanceUnit::KM && speedSubtractionValue <= 0) ||
               (mInter_TripDistanceUnit == Inter_TripDistanceUnit::MILE && speedSubtractionValue <= 0);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using Mslaoverride = SFCCC_MSLAInter_Mslaoverride;
    using CcOnOffStat = SFCCC_MSLAInter_CcOnOffStat;
    using MslaOnOffStat = SFCCC_MSLAInter_MslaOnOffStat;
    using CcActiveStat = SFCCC_MSLAInter_CcActiveStat;
    using MslaActiveStat = SFCCC_MSLAInter_MslaActiveStat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using Inter_ConfigAdasSCC = ArgumentsCcOnOffStatChanged::Inter_ConfigAdasSCC;
    using Input_CruiseMainIndReqStatus = ArgumentsCcOnOffStatChanged::Input_CruiseMainIndReqStatus;
    Inter_ConfigAdasSCC mInter_ConfigAdasSCC = Inter_ConfigAdasSCC::OFF;
    Input_CruiseMainIndReqStatus mInput_CruiseMainIndReqStatus = Input_CruiseMainIndReqStatus::OFF;
    Input_CruiseMainIndReqStatus mPrev_Input_CruiseMainIndReqStatus = Input_CruiseMainIndReqStatus::OFF;

    using Input_SpeedLimitActiveStatus = ArgumentsSpeedStatusChanged::Input_SpeedLimitActiveStatus;
    Input_SpeedLimitActiveStatus mInput_SpeedLimitActiveStatus = Input_SpeedLimitActiveStatus::OFF;
    Input_SpeedLimitActiveStatus mPrev_Input_SpeedLimitActiveStatus = Input_SpeedLimitActiveStatus::OFF;

    using Inter_ConfigVehicleType = ArgumentsCcSldMaxSpeedValueChanged::Inter_ConfigVehicleType;
    Inter_ConfigVehicleType mInter_ConfigVehicleType = Inter_ConfigVehicleType::ICV;

    using Input_CruiseSetIndReqStatus = ArgumentsCcDisplayChagned::Input_CruiseSetIndReqStatus;
    Input_CruiseSetIndReqStatus mInput_CruiseSetIndReqStatus = Input_CruiseSetIndReqStatus::OFF;

    using Input_SpeedLimitReadyStatus = ArgumentsSpeedStatusChanged::Input_SpeedLimitReadyStatus;
    Input_SpeedLimitReadyStatus mInput_SpeedLimitReadyStatus = Input_SpeedLimitReadyStatus::NORMAL;

    using Input_SmvSetSpeedStatus = ArgumentsSpeedStatusChanged::Input_SmvSetSpeedStatus;
    Input_SmvSetSpeedStatus mInput_SmvSetSpeedStatus = Input_SmvSetSpeedStatus::OFF;

    using Input_NccCameraOperStatus = ArgumentsNccDisplayChanged::Input_NccCameraOperStatus;
    Input_NccCameraOperStatus mInput_NccCameraOperStatus = Input_NccCameraOperStatus::OFF;

    using Inter_TripDistanceUnit = ArgumentsSpeedStatusChanged::Inter_TripDistanceUnit;
    using Input_CruiseOverrideReqStatus = ArgumentsSpeedStatusChanged::Input_CruiseOverrideReqStatus;

    Inter_TripDistanceUnit mInter_TripDistanceUnit = Inter_TripDistanceUnit::NONE;
    Input_CruiseOverrideReqStatus mInput_CruiseOverrideReqStatus = Input_CruiseOverrideReqStatus::OFF;

    using Input_SpeedLimitInfoDisplayReqStatus =
        ArgumentsSpeedLimitInfoDisplayReqStatusChanged::Input_SpeedLimitInfoDisplayReqStatus;
    Input_SpeedLimitInfoDisplayReqStatus mInput_SpeedLimitInfoDisplayReqStatus = Input_SpeedLimitInfoDisplayReqStatus::OFF;

    using Input_NccDisplayReqStatus = ArgumentsNccDisplayReqStatusChanged::Input_NccDisplayReqStatus;
    Input_NccDisplayReqStatus mInput_NccDisplayReqStatus = Input_NccDisplayReqStatus::OFF;

    using Input_CruiseInfoDisplayReqStatus = ArgumentsCruiseInfoDisplayReqStatusChanged::Input_CruiseInfoDisplayReqStatus;
    Input_CruiseInfoDisplayReqStatus mInput_CruiseInfoDisplayReqStatus = Input_CruiseInfoDisplayReqStatus::OFF;

    using ISLAManualSetSpeedStat = ArgumentsIslaManualSetSpeedChanged::ISLAManualSetSpeedStat;
    ISLAManualSetSpeedStat mISLAManualSetSpeedStat = ISLAManualSetSpeedStat::OFF;

    using CrawlActiveStatus = ArgumentsCrawlActiveChanged::Input_CrawlActiveStatus;
    CrawlActiveStatus mCrawlActiveStatus = CrawlActiveStatus::OFF;

    using CrawlActiveStepStatus = ArgumentsCrawlActiveChanged::Input_CrawlActiveStepStatus;
    CrawlActiveStepStatus mCrawlActiveStepStatus = CrawlActiveStepStatus::OFF;

    using CrawlWarnStatus = ArgumentsCrawlWarnStatus::Input_CrawlWarnStatus;
    CrawlWarnStatus mCrawlWarnStatus = CrawlWarnStatus::OFF;

    static constexpr uint64_t kUINT64_MAX = 0xFFFFFFFF;
    static constexpr uint64_t kTimeOut = kUINT64_MAX;
    static constexpr uint32_t kTimerMslaoverrideTriggerInterval50ms = 50u;
    static constexpr uint32_t kTimerSummaryEventOffInterval160ms = 160u;
    static constexpr uint32_t kTimerNoDataInterval10ms = 10u;
    ssfs::SFCTimer<CC_MSLA> mOneShotTimerMslaoverride;
    ssfs::SFCTimer<CC_MSLA> mOneShotTimerSummaryEventOff;
    ssfs::SFCTimer<CC_MSLA> mOneShotTimerNoData;
    HUInt64 mSpeedDigitalValue = 0;
    HUInt64 mInput_CcSldSetSpeedValue = 0;
    HUInt64 mInput_CcOnlySetSpeedValue = 0;
    HUInt64 mPrivate_CcSldSetSpeedValue = 0;
    HUInt64 mInter_CcSldMaxSpeedValue = 0;
    HUInt64 ccMslaSummary = 0;

    Mslaoverride mInter_Mslaoverride = Mslaoverride::OFF;
    CcActiveStat mInter_CcActiveStat = CcActiveStat::OFF;
    MslaActiveStat mInter_MslaActiveStat = MslaActiveStat::OFF;
    SFCCC_MSLAConstantCcColorStat mConstantCcColorStat = SFCCC_MSLAConstantCcColorStat::NONE;
    SFCCC_MSLAConstantMslaColorStat mConstantMslaColorStat = SFCCC_MSLAConstantMslaColorStat::NONE;
    CcOnOffStat mInter_CcOnOffStat = CcOnOffStat::OFF;
    MslaOnOffStat mInter_MslaOnOffStat = MslaOnOffStat::OFF;
    SFCCC_MSLAEventCcMslaSummaryStat isEventStatus = SFCCC_MSLAEventCcMslaSummaryStat::OFF;
};

}  // namespace ccos

#endif  // SFSS_CC_MSLA_H
