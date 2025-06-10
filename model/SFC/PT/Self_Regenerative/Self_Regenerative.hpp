/**
 * @file Self_Regenerative.hpp
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
#ifndef SFSS_Self_Regenerative_H
#define SFSS_Self_Regenerative_H

#define DLOG_ENABLED gEnableSFCLog

#include "Self_RegenerativeBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 1.0.0
// Reference : [PT510] Self_Regenerative. Contains : Constant, Event. Recommend: VALUE_CHANGED
class Self_Regenerative : public Self_RegenerativeBase {
public:
    Self_Regenerative() = default;
    ~Self_Regenerative() override = default;
    Self_Regenerative(const Self_Regenerative& other) = delete;
    Self_Regenerative(Self_Regenerative&& other) noexcept = delete;
    Self_Regenerative& operator=(const Self_Regenerative& other) = delete;
    Self_Regenerative& operator=(Self_Regenerative&& other) noexcept = delete;

    void onInitialize() override {
        setSFCSelf_RegenerativeEventSelfRgnNeedID("E24501");
        setSFCSelf_RegenerativeEventSelfRgnNeedLinkedSoundID("SND_PopUpWarn1");
        setSFCSelf_RegenerativeEventSelfRgnNeedLinkedSoundType(SFCSelf_RegenerativeEventSelfRgnNeedLinkedSoundType::REPEAT_COUNT);
        setSFCSelf_RegenerativeEventSelfRgnNeedLinkedSoundRepeatCount(1);

        setSFCSelf_RegenerativeEventSelfRgnComPleteID("E24502");
        setSFCSelf_RegenerativeEventSelfRgnComPleteLinkedSoundID("SND_PopUpWarn1");
        setSFCSelf_RegenerativeEventSelfRgnComPleteLinkedSoundType(
            SFCSelf_RegenerativeEventSelfRgnComPleteLinkedSoundType::REPEAT_COUNT);
        setSFCSelf_RegenerativeEventSelfRgnComPleteLinkedSoundRepeatCount(1);

        setSFCSelf_RegenerativeEventSelfRgnNotConditionMetID("E24503");
        setSFCSelf_RegenerativeEventSelfRgnNotConditionMetLinkedSoundID("SND_PopUpWarn1");
        setSFCSelf_RegenerativeEventSelfRgnNotConditionMetLinkedSoundType(
            SFCSelf_RegenerativeEventSelfRgnNotConditionMetLinkedSoundType::REPEAT_COUNT);
        setSFCSelf_RegenerativeEventSelfRgnNotConditionMetLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateSelfRgnOnOffSetReq();
        updateSelfRgnSystem();
        updateEventSelfRgnNeed();
        updateEventSelfRgnComPlete();
        updateEventSelfRgnNotConditionMet();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateSelfRgnOnOffSetReq();
        updateEventSelfRgnNeed();
        updateEventSelfRgnComPlete();
        updateEventSelfRgnNotConditionMet();
    }

    void onSelfRgnWarnStatusChanged(const ArgumentsSelfRgnWarnStatusChanged& args) {
        mSelfRgnWarnStatus = args.mInput_SelfRgnWarnStatus;
        updateEventSelfRgnNeed();
        updateEventSelfRgnComPlete();
    }

    void onSelfRgnOperationStatusChanged(const ArgumentsSelfRgnOperationStatusChanged& args) {
        mSelfRgnOperationStatus = args.mInput_SelfRgnOperationStatus;
        updateSelfRgnOnOffSetReq();
        updateSelfRgnSystem();
    }

    void onSelfRgnProgressMinValueChanged(const ArgumentsSelfRgnProgressMinValueChanged& args) {
        mSelfRgnProgressMinValue = args.mInput_SelfRgnProgressMinValue;
        updateSelfRgnSystem();
    }

    void onSelfRgnNoConditionStatusChanged(const ArgumentsSelfRgnNoConditionStatusChanged& args) {
        mSelfRgnNoConditionStatus = args.mInput_SelfRgnNoConditionStatus;
        updateEventSelfRgnNotConditionMet();
    }

    void onSelfRgnWarnPageStatusChanged(const ArgumentsSelfRgnWarnPageStatusChanged& args) {
        mSelfRgnWarnPageStatus = args.mInput_SelfRgnWarnPageStatus;
        updateSelfRgnSystem();
    }

    void onSelfRgnOnoffReqChanged(const ArgumentsSelfRgnOnoffReqChanged& args) {
        mSelfRgnOnoffReq = args.mInter_SelfRgnOnoffReq;
        updateSelfRgnOnOffSetReq();
        mPrevSelfRgnOnoffReq = mSelfRgnOnoffReq;
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 4 연산 로직
    void updateSelfRgnOnOffSetReq() {
        Output_SelfRgnOnOffSetReq req = Output_SelfRgnOnOffSetReq::DEFAULT;
        if (mIsIgnOn == true && mPrevSelfRgnOnoffReq != SelfRgnOnoffReq::ON_REQ && mSelfRgnOnoffReq == SelfRgnOnoffReq::ON_REQ) {
            if (mSelfRgnOperationStatus == SelfRgnOperationStatus::STOP) {
                req = Output_SelfRgnOnOffSetReq::ONGOING;
            } else if (mSelfRgnOperationStatus == SelfRgnOperationStatus::ONGOING) {
                req = Output_SelfRgnOnOffSetReq::STOP;
            } else {
                // no operation
            }
        }
        updateActuatorValue(Vehicle.PT.Self_Regenerative.Output_SelfRgnOnOffSetReq, static_cast<HUInt64>(req));
        flushActuatorOutput();
    }

    // 5.2.1 자가 재생 모드
    void updateSelfRgnSystem() {
        SFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat operationStat =
            SFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat::DISPLAY_OFF;
        SFCSelf_RegenerativeConstantSelfRgnSystem_QueStat queStat =
            SFCSelf_RegenerativeConstantSelfRgnSystem_QueStat::DISPLAY_OFF;
        SFCSelf_RegenerativeConstantSelfRgnSystem_TimeStat timeStat =
            SFCSelf_RegenerativeConstantSelfRgnSystem_TimeStat::DISPLAY_OFF;
        HUInt64 timeValue = 0;
        if (mIsIgnOn == true) {
            if (mSelfRgnOperationStatus != SelfRgnOperationStatus::ONGOING) {
                if (mSelfRgnWarnPageStatus != SelfRgnWarnPageStatus::STOP_SELF_RGN) {
                    operationStat = SFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat::NEED;
                } else {
                    operationStat = SFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat::FAIL;
                }
                queStat = SFCSelf_RegenerativeConstantSelfRgnSystem_QueStat::PRESS_ONGOING;
            } else {
                operationStat = SFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat::ONGOING;
                queStat = SFCSelf_RegenerativeConstantSelfRgnSystem_QueStat::PRESS_STOP;
                if (mSelfRgnProgressMinValue >= 1 && mSelfRgnProgressMinValue <= 100) {
                    timeStat = SFCSelf_RegenerativeConstantSelfRgnSystem_TimeStat::ON;
                    timeValue = mSelfRgnProgressMinValue;
                } else if (mSelfRgnProgressMinValue == 0 ||
                           (mSelfRgnProgressMinValue >= 101 && mSelfRgnProgressMinValue <= 255)) {
                    timeStat = SFCSelf_RegenerativeConstantSelfRgnSystem_TimeStat::UNMEASURABLE;
                } else {
                    // no operation
                }
            }
            setSFCSelf_RegenerativeConstantSelfRgnSystem_OperationStat(operationStat);
            setSFCSelf_RegenerativeConstantSelfRgnSystem_QueStat(queStat);
            setSFCSelf_RegenerativeConstantSelfRgnSystem_TimeStat(timeStat);
            setSFCSelf_RegenerativeConstantSelfRgnSystem_TimeValue(timeValue);
        }
    }

    // 5.3 이벤트 표시 동작 로직
    // E24501
    void updateEventSelfRgnNeed() {
        SFCSelf_RegenerativeEventSelfRgnNeedStat eventStat = SFCSelf_RegenerativeEventSelfRgnNeedStat::OFF;
        if (mIsIgnOn == true && mSelfRgnWarnStatus == SelfRgnWarnStatus::NEED) {
            eventStat = SFCSelf_RegenerativeEventSelfRgnNeedStat::ON;
        }
        setSFCSelf_RegenerativeEventSelfRgnNeedStat(eventStat);
    }

    // E24502
    void updateEventSelfRgnComPlete() {
        SFCSelf_RegenerativeEventSelfRgnComPleteStat eventStat = SFCSelf_RegenerativeEventSelfRgnComPleteStat::OFF;
        if (mIsIgnOn == true && mSelfRgnWarnStatus == SelfRgnWarnStatus::COMPLETE) {
            eventStat = SFCSelf_RegenerativeEventSelfRgnComPleteStat::ON;
        }
        setSFCSelf_RegenerativeEventSelfRgnComPleteStat(eventStat);
    }

    // E24503
    void updateEventSelfRgnNotConditionMet() {
        SFCSelf_RegenerativeEventSelfRgnNotConditionMetStat eventStat = SFCSelf_RegenerativeEventSelfRgnNotConditionMetStat::OFF;
        if (mIsIgnOn == true && mSelfRgnNoConditionStatus == SelfRgnNoConditionStatus::NOT_CONDITION_MET) {
            eventStat = SFCSelf_RegenerativeEventSelfRgnNotConditionMetStat::ON;
        }
        setSFCSelf_RegenerativeEventSelfRgnNotConditionMetStat(eventStat);
    }

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (mActuatorOutputAddrList.empty() == false) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    using SelfRgnWarnStatus = ArgumentsSelfRgnWarnStatusChanged::Input_SelfRgnWarnStatus;
    using SelfRgnOperationStatus = ArgumentsSelfRgnOperationStatusChanged::Input_SelfRgnOperationStatus;
    using SelfRgnNoConditionStatus = ArgumentsSelfRgnNoConditionStatusChanged::Input_SelfRgnNoConditionStatus;
    using SelfRgnWarnPageStatus = ArgumentsSelfRgnWarnPageStatusChanged::Input_SelfRgnWarnPageStatus;
    using SelfRgnOnoffReq = ArgumentsSelfRgnOnoffReqChanged::Inter_SelfRgnOnoffReq;
    using Output_SelfRgnOnOffSetReq = decltype(Vehicle.PT.Self_Regenerative.Output_SelfRgnOnOffSetReq)::TYPE;

    HBool mIsIgnOn = false;
    SelfRgnWarnStatus mSelfRgnWarnStatus = SelfRgnWarnStatus::OFF;
    SelfRgnOperationStatus mSelfRgnOperationStatus = SelfRgnOperationStatus::OFF;
    HUInt64 mSelfRgnProgressMinValue = 0;
    SelfRgnNoConditionStatus mSelfRgnNoConditionStatus = SelfRgnNoConditionStatus::OFF;
    SelfRgnWarnPageStatus mSelfRgnWarnPageStatus = SelfRgnWarnPageStatus::OFF;
    SelfRgnOnoffReq mSelfRgnOnoffReq = SelfRgnOnoffReq::DEFAULT;
    SelfRgnOnoffReq mPrevSelfRgnOnoffReq = SelfRgnOnoffReq::DEFAULT;
    AddressValueList mActuatorOutputAddrList;
};

}  // namespace ccos

#endif  // SFSS_Self_Regenerative_H
